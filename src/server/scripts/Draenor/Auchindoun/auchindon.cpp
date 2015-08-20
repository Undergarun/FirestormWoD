
#include "ScriptedCreature.h"
#include "auchindon.hpp"

#define cooldown_mind_spike urand(2000, 5000)
#define cooldown_sever_tendon urand(2000, 5000)
#define cooldown_void_mending urand(2000, 5000)
#define cooldown_heroic_void_mending urand(2000, 5000)
#define cooldown_void_shift urand(2000, 5000)
#define cooldown_shield_bash urand(2000, 5000)
#define cooldown_void_strikes urand(2000, 5000)

Position TuulaniPreMovement01st = { 1538.54f, 2952.99f, 35.239f, 0.041050f };
Position TuulaniPreMovement02nd = { 1591.58f, 2953.37f, 35.239f, 0.021415f };
Position TuulaniPreMovement03rd = { 1646.196f, 2956.79f, 35.060f, 0.975673f };
Position TuulaniMovement2nd = { 1644.27f, 2962.42f, 35.185f, 1.269892f };
Position GuardPositionTalker = { 1645.61f, 2967.25f, 35.167f, 0.024081f };
Position TuulaniMovement3rd = { 1660.10f, 2999.02f, 34.763f, 1.168017f };
Position TuulaniMovement4th = { 1685.99f, 3065.55f, 35.139f, 1.204807f };
Position TuulaniMovement5th = { 1722.35f, 3121.52f, 35.139f, 0.914209f };
Position TuulaniMovement6th = { 1756.30f, 3154.63f, 35.139f, 0.772838f };
Position TuulaniMovement7th = { 1806.26f, 3178.98f, 35.127f, 0.454751f };
Position TuulaniMovement8th = { 1835.91f, 3183.27f, 29.423f, 0.129596f };
Position TuulaniMovement9th = { 1854.28f, 3199.26f, 29.424f, 5.918766f };
Position TuulaniMovement10th = { 1867.89f, 3197.28f, 30.970f, 6.138678f };
Position LightFloatingTrigger[3] =
{
    { 1664.70f, 3011.64f, 41.503f, 4.432930f },
    { 1757.79f, 2952.85f, 44.559f, 6.247188f },
    { 1665.26f, 2893.94f, 41.317f, 5.010184f },
};
// Combat guards
Position Guards[6] =
{
    { 1497.06f, 2948.16f, 35.239f, 2.452730f },
    { 1495.23f, 2945.86f, 35.696f, 2.554010f },
    { 1491.63f, 2944.18f, 36.147f, 2.033919f },
    { 1495.93f, 2956.85f, 35.239f, 3.644247f },
    { 1493.42f, 2958.79f, 35.239f, 3.926992f },
    { 1490.35f, 2959.78f, 35.614f, 3.875940f },
};
// Clapping guards
Position Guards2nd[2] =
{
    { 1506.64f, 2958.66f, 35.238f, 4.793027f },
    { 1504.08f, 2958.45f, 35.238f, 4.793027f },
};
// Warden Patrol, and Warden position to be spawned at
Position Warden1st = { 1510.29f, 2954.78f, 35.238f, 0.091797f };
Position WardenPatrol[2] =
{
    { 1609.98f, 2955.21f, 35.238f },
    { 1508.82f, 2954.96f, 35.238f },
};
// Middle
Position CircularMages[3] =
{
    { 1662.40f, 2939.76f, 35.143f, 1.624580f },
    { 1668.03f, 2941.39f, 35.143f, 2.242950f },
    { 1657.34f, 2965.32f, 35.061f, 5.090852f },
};
Position CircularPriests[2] =
{
    { 1648.36f, 2950.41f, 35.06f, 0.288927f },
    { 1650.16f, 2947.54f, 34.822f, 0.397321f },
};
Position CircularHolies[2] =
{
    { 1653.17f, 2962.95f, 35.061f, 5.514967f },
    { 1650.16f, 2947.54f, 34.822f, 0.397321f },
};
// Left Hallway
Position WardenPatrol2nd[2] =   // ALSO [1] FOR SUMMOn
{
    { 1672.66f, 3022.84f, 35.140f },
    { 1673.99f, 3033.03f, 35.236f },
};
Position AuchenaiMagus2nd[2] =
{
    { 1675.69f, 3068.34f, 35.223f, 2.696660f },
    { 1679.35f, 3075.59f, 35.223f, 2.604600f },
};
Position AuchenaiVigilant[4] =
{
    { 1667.18f, 3077.60f, 36.679f, 5.796140f },
    { 1692.63f, 3118.17f, 36.679f, 5.632550f },
    { 1724.72f, 3153.71f, 36.679f, 5.423370f },
    { 1762.43f, 3183.01f, 36.679f, 5.268510f },
};
Position AuchenaiReader1st = { 1673.96f, 3074.10f, 36.132f, 5.834950f };
Position HopilliteGuardState[2] =
{
    { 1691.04f, 3062.77f, 35.223f, 2.610810f },
    { 1693.10f, 3066.61f, 35.140f, 2.546201f },
};
Position DefenderWhoReadsNearTwoHopilite = { 1691.51f, 3070.65f, 35.223f, 5.132760f };
Position DefenderWhoTalksToSoldiers = { 1709.31f, 3099.06f, 35.139f, 5.945590f };
Position SoulPriestWhoMeditates = { 1701.68f, 3113.02f, 35.223f, 0.0f };
Position MagusWhoCastArcane = { 1710.42f, 3113.87f, 35.223f, 3.386260f };
Position MagusWhoTalksToDefender = { 1728.73f, 3139.19f, 35.223f, 2.261400f };
Position SoulPriestTalksToTwoDefender = { 1745.93f, 3138.90f, 35.223f, 0.293292f };

Position KaatharCrystalPosition = {1909.75f, 3188.70f, 66.786f, 5.401960f};
enum Events
{
    EVENT_BEGIN_RP = 300,
};
enum FixingActions
{
    ACTION_CONFIRM_TUULANI_EVENT = 324266,
};
enum spells_fix
{
    SPELL_VOID_SHIFT_HEALO = 157036,
    SPELL_VOID_SHIFT_DMGO = 157037,
};

class patrol : public BasicEvent
{
public:
    explicit patrol(Unit* unit, int value) : obj(unit), modifier(value)
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
                case 0:
                    obj->GetMotionMaster()->MovePoint(0, WardenPatrol[1].GetPositionX(), WardenPatrol[1].GetPositionY(), WardenPatrol[1].GetPositionZ());
                    obj->m_Events.AddEvent(new patrol(obj, 1), obj->m_Events.CalculateTime(13000));
                    break;
                case 1:
                    obj->GetMotionMaster()->MovePoint(0, WardenPatrol[2].GetPositionX(), WardenPatrol[2].GetPositionY(), WardenPatrol[2].GetPositionZ());
                    obj->m_Events.AddEvent(new patrol(obj, 0), obj->m_Events.CalculateTime(13000));
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
class patrol2nd : public BasicEvent
{
public:
    explicit patrol2nd(Unit* unit, int value) : obj(unit), modifier(value)
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
                case 0:
                    obj->GetMotionMaster()->MovePoint(0, WardenPatrol2nd[1].GetPositionX(), WardenPatrol2nd[1].GetPositionY(), WardenPatrol2nd[1].GetPositionZ());
                    obj->m_Events.AddEvent(new patrol(obj, 1), obj->m_Events.CalculateTime(5000));
                    break;
                case 1:
                    obj->GetMotionMaster()->MovePoint(0, WardenPatrol2nd[2].GetPositionX(), WardenPatrol2nd[2].GetPositionY(), WardenPatrol2nd[2].GetPositionZ());
                    obj->m_Events.AddEvent(new patrol(obj, 0), obj->m_Events.CalculateTime(5000));
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
class clapping : public BasicEvent
{
public:
    explicit clapping(Unit* unit, int value) : obj(unit), modifier(value)
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
                case 0:
                    if (Player* nearest = obj->ToCreature()->FindNearestPlayer(50.0f, true))
                    {
                        obj->ToCreature()->SetFacingToObject(nearest);
                        obj->CastSpell(obj, SPELL_APPLAUD);

                        obj->m_Events.AddEvent(new clapping(obj, 1), obj->m_Events.CalculateTime(6000));
                    }
                    break;
                case 1:
                {
                    obj->RemoveAllAuras();
                    break;
                }
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
class ArcaneBombEvent : public BasicEvent
{
public:
    explicit ArcaneBombEvent(Unit* unit, int value) : obj(unit), modifier(value)
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
                case 0:
                    if (Creature* nearest = obj->FindNearestCreature(TRIGGER_ARCANE_BOMB, 50.0f, true))
                    {
                        obj->CastSpell(nearest, SPELL_ARCANE_BOMB_AREA_TRIGGER);
                        obj->m_Events.AddEvent(new ArcaneBombEvent(obj, 0), obj->m_Events.CalculateTime(6000));
                    }
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
class auchindon_mob_trigger_cosmetic : public CreatureScript
{
public:
    auchindon_mob_trigger_cosmetic() : CreatureScript("auchindon_mob_trigger_cosmetic") {}

    struct auchindon_cosmetic_triggers : public ScriptedAI
    {
        auchindon_cosmetic_triggers(Creature* creature) : ScriptedAI(creature)
        {
            pInstance = creature->GetInstanceScript();

            // Dispensor
            if (!dispensor.empty())
                dispensor.clear();

            //Creature* Tuulani = me->SummonCreature(CREATURE_SOUL_BINDER_TUULANI, 1505.89f, 2952.93f, 35.239f, 0.101096f, TEMPSUMMON_MANUAL_DESPAWN);
           // if (Tuulani)
            //{
             //   dispensor.push_back(Tuulani);
            //}
            // Auchindon Summon Warriors
            for (int i = 0; i <= 5; i++)
            {
                Creature* guard = me->SummonCreature(CREATURE_AUCHENI_DEFENDER, Guards[i], TEMPSUMMON_MANUAL_DESPAWN);

                if (guard)
                {
                    guard->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 505);
                    dispensor.push_back(guard);
                }
            }
            // Auchindon Summon Clappers
            for (int i = 0; i < 2; i++)
            {
                Creature* clapper = me->SummonCreature(CREATURE_AUCHENI_DEFENDER, Guards2nd[i], TEMPSUMMON_MANUAL_DESPAWN);
                clapper->m_Events.AddEvent(new clapping(clapper, 0), clapper->m_Events.CalculateTime(6000));

                dispensor.push_back(clapper);
            }
            /*
            // Auchindon Patroler Warden
            Creature* warden1 = me->SummonCreature(CREATURE_AUCHENI_WARDEN, Warden1st, TEMPSUMMON_MANUAL_DESPAWN);
            if (warden1)
            {
                warden1->m_Events.AddEvent(new patrol(warden1, 0), warden1->m_Events.CalculateTime(2000));
                dispensor.push_back(warden1);
            }
            */
            // Cicrular Mobs - Magus
            for (int i = 0; i < 3; i++)
            {
                Creature* magus = me->SummonCreature(CREATURE_AUCHENI_MAGUS, CircularMages[i], TEMPSUMMON_MANUAL_DESPAWN);
                if (magus)
                {
                    magus->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_HOVER);
                    dispensor.push_back(magus);
                }
            }
            // Cicrular Mobs - Priest
            for (int i = 0; i < 2; i++)
            {
                Creature* priest = me->SummonCreature(CREATURE_AUCHENI_SOUL_PRIEST, CircularPriests[i], TEMPSUMMON_MANUAL_DESPAWN);
                if (priest)
                {
                    priest->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, SPELL_KNEEL);
                    dispensor.push_back(priest);
                }
            }
            // Cicrular Mobs - Cleric
            for (int i = 0; i < 2; i++)
            {
                Creature* Cleric = me->SummonCreature(CREATURE_AUCHENI_SOUL_PRIEST, CircularHolies[i], TEMPSUMMON_MANUAL_DESPAWN);
                if (Cleric)
                {
                    Cleric->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, SPELL_KNEEL);
                    dispensor.push_back(Cleric);
                }
            }
            /*
            // Left Hallway
            // Warden Patroller
            Creature* warden2 = me->SummonCreature(CREATURE_AUCHENI_WARDEN, WardenPatrol2nd[1], TEMPSUMMON_MANUAL_DESPAWN);
            if (warden2)
            {
                warden2->m_Events.AddEvent(new patrol2nd(warden2, 0), warden2->m_Events.CalculateTime(500));
                dispensor.push_back(warden2);
            }
            */
            // Hovering Magus near preacher
            for (int i = 0; i < 2; i++)
            {
                Creature* magus2nd = me->SummonCreature(CREATURE_AUCHENI_MAGUS, AuchenaiMagus2nd[i], TEMPSUMMON_MANUAL_DESPAWN);
                if (magus2nd)
                {
                    magus2nd->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_HOVER);
                    dispensor.push_back(magus2nd);
                }
            }
            // Preacher near 2 magus
            Creature* preacher = me->SummonCreature(CREATURE_AUCHENI_ARBITER, AuchenaiReader1st, TEMPSUMMON_MANUAL_DESPAWN);
            if (preacher)
            {
                preacher->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_READ);
                dispensor.push_back(preacher);
            }
            for (int i = 0; i < 4; i++)
            {
                Creature* vigiliant = me->SummonCreature(CREATURE_AUCHENI_VIGILIANT, AuchenaiVigilant[i], TEMPSUMMON_MANUAL_DESPAWN);
                if (vigiliant)
                {
                    vigiliant->CastSpell(vigiliant, SPELL_GUARD);
                 

                    vigiliant->SetCurrentEquipmentId(77819);

                    vigiliant->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
                    vigiliant->SetFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
                    vigiliant->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    vigiliant->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);

                    dispensor.push_back(vigiliant);
                }
            }
            for (int i = 0; i < 2; i++)
            {
                // Two Guarding Hopilite
                Creature* Hopilite = me->SummonCreature(CREATURE_AUCHENI_HOPLITE, HopilliteGuardState[i], TEMPSUMMON_MANUAL_DESPAWN);
                if (Hopilite)
                {
                    Hopilite->CastSpell(Hopilite, SPELL_GUARD);
                    dispensor.push_back(Hopilite);
                }
            }
            // Defender that reads near two guarding hopilite
            Creature* defenderwhoreads = me->SummonCreature(CREATURE_AUCHENI_DEFENDER, DefenderWhoReadsNearTwoHopilite, TEMPSUMMON_MANUAL_DESPAWN);
            if (defenderwhoreads)
            {
                defenderwhoreads->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_READ);
                dispensor.push_back(defenderwhoreads);
            }
            // Priest who meditates
            Creature* priestwhomeditate = me->SummonCreature(CREATURE_AUCHENI_SOUL_PRIEST, SoulPriestWhoMeditates, TEMPSUMMON_MANUAL_DESPAWN);
            if (priestwhomeditate)
            {
                priestwhomeditate->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_HOVER);
                dispensor.push_back(priestwhomeditate);
            }
            // Magus who cast arcane bomb near hovering priest.
            Creature* maguswhocastarcanebombs = me->SummonCreature(CREATURE_AUCHENI_MAGUS, MagusWhoCastArcane, TEMPSUMMON_MANUAL_DESPAWN);
            if (maguswhocastarcanebombs)
            {
                maguswhocastarcanebombs->CastSpell(maguswhocastarcanebombs, SPELL_ARCANE_CHANNELING);
                maguswhocastarcanebombs->m_Events.AddEvent(new ArcaneBombEvent(maguswhocastarcanebombs, 0), maguswhocastarcanebombs->m_Events.CalculateTime(20000));
                dispensor.push_back(maguswhocastarcanebombs);
            }
            // Magus who talk to defender
            Creature* magus3rd = me->SummonCreature(CREATURE_AUCHENI_MAGUS, MagusWhoTalksToDefender, TEMPSUMMON_MANUAL_DESPAWN);
            if (magus3rd)
            {
                magus3rd->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_TALK);
                dispensor.push_back(magus3rd);
            }
            // Soul Priest who talks to defenders
            Creature* soulpriest2nd = me->SummonCreature(CREATURE_AUCHENI_SOUL_PRIEST, SoulPriestTalksToTwoDefender, TEMPSUMMON_MANUAL_DESPAWN);
            if (soulpriest2nd)
            {
                soulpriest2nd->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_TALK);
                dispensor.push_back(soulpriest2nd);
            }

            for (auto itr : dispensor)
            {
                itr->SetReactState(REACT_PASSIVE);
                itr->SetRespawnDelay(1000000);
                itr->SetCurrentEquipmentId(1);
            }
        }
        std::list<Creature*> dispensor;
        EventMap events;
        InstanceScript* pInstance;
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new auchindon_cosmetic_triggers(creature);
    }
};
class tuulani_1st_event : public BasicEvent
{
public:
    explicit tuulani_1st_event(Unit* unit, int value) : obj(unit), modifier(value)
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
                        switch (modifier)
                        {
                            case 0: // TUULINA         
                            {
                                Nyami->AddAura(SPELL_DARK_FIRE, Nyami);
                                Nyami->AddAura(SPELL_LEVITATE_NYAMI, Nyami);
                                Nyami->SetReactState(REACT_PASSIVE);
                                Nyami->CastSpell(Nyami, SPELL_PRISON_AURA);

                                tuulina->GetAI()->DoAction(ACTION_CONFIRM_TUULANI_EVENT);

                                if (Creature* trigger = Nyami->FindNearestCreature(100055, 40.0f, true))
                                {
                                    trigger->AddAura(SPELL_VOID_FORM_TRIGGER_BUFF, trigger);
                                    Nyami->CastSpell(trigger, SPELL_SHADOW_BEAM);
                                }

                                Creature* guard = obj->SummonCreature(CREATURE_AUCHENI_DEFENDER, GuardPositionTalker, TEMPSUMMON_MANUAL_DESPAWN);
                                if (guard)
                                    guard->CastSpell(guard, SPELL_KNEEL); 

                                tuulina->AI()->Talk(TUULANI_TALK_2);
                                tuulina->GetMotionMaster()->MovePoint(1, TuulaniPreMovement02nd.GetPositionX(), TuulaniPreMovement02nd.GetPositionY(), TuulaniPreMovement02nd.GetPositionZ());                 
                                break;
                            }
                            case 1: // TUULINA
                            {
                                tuulina->AI()->Talk(TUULANI_TALK_3);
                                tuulina->GetMotionMaster()->MovePoint(2, TuulaniPreMovement03rd.GetPositionX(), TuulaniPreMovement03rd.GetPositionY(), TuulaniPreMovement03rd.GetPositionZ());                     
                                break;
                            }
                            case 3: // GUARD
                            {
                                Creature* guard = obj->ToCreature();
                                if (guard)
                                {
                                    guard->RemoveAura(SPELL_KNEEL);
                                    guard->AI()->Talk(AUCHENAI_DEFENDER_TALK_1);
                                    guard->SetFacingToObject(tuulina);
                                    guard->m_Events.AddEvent(new tuulani_1st_event(guard, 4), guard->m_Events.CalculateTime(7000));
                                    break;
                                }
                            }
                            case 4: // TUULANI
                            {
                                tuulina->GetMotionMaster()->MovePoint(3, TuulaniMovement3rd.GetPositionX(), TuulaniMovement3rd.GetPositionY(), TuulaniMovement3rd.GetPositionZ());
                                break;
                            }                     
                            case 5: // TUULANI                    
                            {
                                tuulina->AI()->Talk(TUULANI_TALK_4);
                                tuulina->m_Events.AddEvent(new tuulani_1st_event(tuulina, 6), tuulina->m_Events.CalculateTime(4000));
                                break;
                            }                  
                            case 6:
                            {
                                tuulina->CastSpell(tuulina, SPELL_TUULANI_UNLOCK);
                                tuulina->m_Events.AddEvent(new tuulani_1st_event(tuulina, 7), tuulina->m_Events.CalculateTime(7000));
                                break;
                            }                   
                            case 7:
                            {
                                if (GameObject* nearestdoor = tuulina->FindNearestGameObject(GAMEOBJECT_HOLY_WALL, 60.0f))
                                {
                                    nearestdoor->Delete();
                                }
                                tuulina->AI()->Talk(TUULANI_TALK_0002);
                                tuulina->GetMotionMaster()->MovePoint(4, TuulaniMovement4th.GetPositionX(), TuulaniMovement4th.GetPositionY(), TuulaniMovement4th.GetPositionZ());
                                break;
                            }                     
                            case 8:
                            {
                                tuulina->AI()->Talk(TUULANI_TALK_5);
                                tuulina->GetMotionMaster()->MovePoint(5, TuulaniMovement5th.GetPositionX(), TuulaniMovement5th.GetPositionY(), TuulaniMovement5th.GetPositionZ());
                                break;
                            }                     
                            case 9:
                            {
                                tuulina->GetMotionMaster()->MovePoint(6, TuulaniMovement6th.GetPositionX(), TuulaniMovement6th.GetPositionY(), TuulaniMovement6th.GetPositionZ());
                                break;
                            }               
                            case 10:
                            {
                                tuulina->GetMotionMaster()->MovePoint(7, TuulaniMovement7th.GetPositionX(), TuulaniMovement7th.GetPositionY(), TuulaniMovement7th.GetPositionZ());
                                break;
                            }
                            case 11:
                            {
                                tuulina->GetMotionMaster()->MovePoint(8, TuulaniMovement8th.GetPositionX(), TuulaniMovement8th.GetPositionY(), TuulaniMovement8th.GetPositionZ());
                                break;
                            }
                            case 12:
                            {

                                tuulina->AI()->Talk(TUULANI_TALK_6);
                                tuulina->GetMotionMaster()->MovePoint(9, TuulaniMovement9th.GetPositionX(), TuulaniMovement9th.GetPositionY(), TuulaniMovement9th.GetPositionZ());                    
                                break;
                            }
                            case 13:
                            {
                                tuulina->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
                                tuulina->SetFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
                                tuulina->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                                tuulina->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);

                                tuulina->AI()->Talk(TUULANI_TALK_7);
                                tuulina->GetMotionMaster()->MovePoint(10, TuulaniMovement10th.GetPositionX(), TuulaniMovement10th.GetPositionY(), TuulaniMovement10th.GetPositionZ());   
                                tuulina->m_Events.AddEvent(new tuulani_1st_event(tuulina, 15), tuulina->m_Events.CalculateTime(7000));
                                 break;
                            }
                            case 15:
                            {       
                                 Nyami->AI()->Talk(NYAMI_TALK_1);    
                                 tuulina->m_Events.AddEvent(new tuulani_1st_event(tuulina, 16), tuulina->m_Events.CalculateTime(9000));
                                 break;
                            }                       
                            case 16:
                            {
                                Nyami->AI()->Talk(NYAMI_TALK_2);
                                tuulina->m_Events.AddEvent(new tuulani_1st_event(tuulina, 17), tuulina->m_Events.CalculateTime(9000));
                                break;
                            }                       
                            case 17:
                            {
                                Nyami->AI()->Talk(NYAMI_TALK_3);
                                tuulina->m_Events.AddEvent(new tuulani_1st_event(tuulina, 18), tuulina->m_Events.CalculateTime(9000));
                                break;
                            }                         
                            case 18:
                            {
                                Nyami->AI()->Talk(NYAMI_TALK_4);
                                tuulina->m_Events.AddEvent(new tuulani_1st_event(tuulina, 19), tuulina->m_Events.CalculateTime(9000));
                                break;
                            }                         
                            case 19:
                            {
                                tuulina->AI()->Talk(TUULANI_TALK_8);
                                tuulina->m_Events.AddEvent(new tuulani_1st_event(tuulina, 20), tuulina->m_Events.CalculateTime(9000));
                                break;
                            }
                            case 20:

                            {
                                Nyami->AI()->Talk(NYAMI_TALK_5);
                                tuulina->m_Events.AddEvent(new tuulani_1st_event(tuulina, 21), tuulina->m_Events.CalculateTime(9000));
                                  break;
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
class auchindon_creature_tuulani : public CreatureScript
{
public:
    auchindon_creature_tuulani() : CreatureScript("auchindon_creature_tuulani") {}

    struct auchindon_creatures : public ScriptedAI
    {
 
        auchindon_creatures(Creature* creature) : ScriptedAI(creature)
        {
            pInstance = creature->GetInstanceScript(); 
          //  me->Respawn(true);

            me->setFaction(35);
            Talk(TUULANI_TALK_1);
            me->SetSpeed(MOVE_RUN, 1.5f, true);
            me->SetSpeed(MOVE_WALK, 1.5f, true);
            me->GetMotionMaster()->MovePoint(0, TuulaniPreMovement01st.GetPositionX(), TuulaniPreMovement01st.GetPositionY(), TuulaniPreMovement01st.GetPositionZ());

            events.ScheduleEvent(EVENT_BEGIN_RP, 4000);
        }
        EventMap events;
        InstanceScript* pInstance;
        bool startevent;
        void Reset()
        {       
           // me->m_Events.AddEvent(new tuulani_1st_event(me, 0), me->m_Events.CalculateTime(300));
        }
        void DoAction(int32 const action)
        {
            switch (action)
            {
            case ACTION_CONFIRM_TUULANI_EVENT:
                startevent = true;
                break;
            }
        }
        void MovementInform(uint32 /*type*/, uint32 id)
        {
            switch (id)
            {
            case 0:
               me->m_Events.AddEvent(new tuulani_1st_event(me, 0), me->m_Events.CalculateTime(200));
               break;
            case 1:
                me->m_Events.AddEvent(new tuulani_1st_event(me, 1), me->m_Events.CalculateTime(200));
                break;
            case 2:
                if (Creature* guard = me->FindNearestCreature(CREATURE_AUCHENI_DEFENDER, 10.0F, true))
                    guard->m_Events.AddEvent(new tuulani_1st_event(guard, 3), guard->m_Events.CalculateTime(200));
                break;
            case 3:
                me->m_Events.AddEvent(new tuulani_1st_event(me, 5), me->m_Events.CalculateTime(200));
                break;
            case 4:
                me->m_Events.AddEvent(new tuulani_1st_event(me, 8), me->m_Events.CalculateTime(200));
                break;
            case 5:
                me->m_Events.AddEvent(new tuulani_1st_event(me, 9), me->m_Events.CalculateTime(200));
                break;
            case 6:
                me->m_Events.AddEvent(new tuulani_1st_event(me, 10), me->m_Events.CalculateTime(200));
                break;
            case 7:
                me->m_Events.AddEvent(new tuulani_1st_event(me, 11), me->m_Events.CalculateTime(200));
                break;
            case 8:
                me->m_Events.AddEvent(new tuulani_1st_event(me, 12), me->m_Events.CalculateTime(200));
                break;
            case 9:
                me->m_Events.AddEvent(new tuulani_1st_event(me, 13), me->m_Events.CalculateTime(200));
                break;
            }
        }
        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);

            if (!startevent)
            {
                me->m_Events.AddEvent(new tuulani_1st_event(me, 0), me->m_Events.CalculateTime(0));
            }

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_BEGIN_RP:
                    startevent = false;
                    break;
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
// Trash
class auchindon_creature_sargerei_soulbinder : public CreatureScript
{
public:
    auchindon_creature_sargerei_soulbinder() : CreatureScript("auchindon_creature_sargerei_soulbinder") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* creature) : ScriptedAI(creature)
        {
        }
        InstanceScript* instance = me->GetInstanceScript();
        void Reset()
        {
        }
        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_MIND_SHEAR, 8000);
            events.ScheduleEvent(EVENT_BEND_WILL, 18000);
        }
        void JustDied(Unit* /*killer*/) override
        {
            if (Creature* Kaathar = instance->instance->GetCreature(instance->GetData64(DATA_KATHAAR)))
            {
                if (Kaathar->isAlive())
                    Kaathar->AI()->DoAction(ACTION_COUNT_PRE_1ST_BOSS_KILL);
            }
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case EVENT_MIND_SHEAR:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                    me->CastSpell(target, SPELL_VOID_SHELL);

                events.ScheduleEvent(EVENT_MIND_SHEAR, 8000);
                break;
            case EVENT_BEND_WILL:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                    me->CastSpell(target, SPELL_BEND_WILL);

                events.ScheduleEvent(EVENT_BEND_WILL, 18000);
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
class auchindon_creature_sargerei_cleric : public CreatureScript
{
public:
    auchindon_creature_sargerei_cleric() : CreatureScript("auchindon_creature_sargerei_cleric") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* creature) : ScriptedAI(creature)
        {
        }
        InstanceScript* instance = me->GetInstanceScript();
        void Reset()
        {   
        }
        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_VOID_SHELL, 15000);
        }
        void JustDied(Unit* /*killer*/) override
        {
            if (Creature* Kaathar = instance->instance->GetCreature(instance->GetData64(DATA_KATHAAR)))
            {
                if (Kaathar->isAlive())
                    Kaathar->AI()->DoAction(ACTION_COUNT_PRE_1ST_BOSS_KILL);
            }
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case EVENT_VOID_SHELL:
                me->CastSpell(me, SPELL_VOID_SHELL);
                events.ScheduleEvent(EVENT_VOID_SHELL, urand(12000, 16000));
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
class auchindon_creature_sargerei_ritualist : public CreatureScript
{
public:
    auchindon_creature_sargerei_ritualist() : CreatureScript("auchindon_creature_sargerei_ritualist") { }

    struct auchindon_creatures : public Scripted_NoMovementAI
    {
        auchindon_creatures(Creature* creature) : Scripted_NoMovementAI(creature)
        {
        }
        InstanceScript* instance = me->GetInstanceScript();
        void Reset()
        {
            me->AddAura(SPELL_DARK_FIRE, me);

            if (Creature* trigger = me->SummonCreature(100055, KaatharCrystalPosition, TEMPSUMMON_MANUAL_DESPAWN))
            {
                me->CastSpell(trigger, SPELL_SHADOW_BEAM); // bind triggers      
            }
        }
        void EnterCombat(Unit* attacker)
        {
            me->RemoveAllAuras();
            
            events.ScheduleEvent(EVENT_MIND_SPIKE, urand(7500, 10000));
        }
        void JustDied(Unit* /*killer*/) override
        {
            if (Creature* Kaathar = instance->instance->GetCreature(instance->GetData64(DATA_KATHAAR)))
            {
                if (Kaathar->isAlive())
                    Kaathar->AI()->DoAction(ACTION_COUNT_PRE_1ST_BOSS_KILL);
            }
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case EVENT_MIND_SPIKE:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                {
                    me->CastSpell(target, SPELL_MIND_SPIKE);
                }

                events.ScheduleEvent(EVENT_MIND_SPIKE, urand(7500, 10000));
                break;
            }
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new auchindon_creatures(creature);
    }
};
class auchindon_creature_sargerei_zealot : public CreatureScript
{
public:
    auchindon_creature_sargerei_zealot() : CreatureScript("auchindon_creature_sargerei_zealot") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* creature) : ScriptedAI(creature)
        {
        }

        InstanceScript* instance = me->GetInstanceScript();
        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_SEVER_TENDOM, 5000);
        }
        void JustDied(Unit* /*killer*/) override
        {
            if (Creature* Kaathar = instance->instance->GetCreature(instance->GetData64(DATA_KATHAAR)))
            {
                if (Kaathar->isAlive())
                    Kaathar->AI()->DoAction(ACTION_COUNT_PRE_1ST_BOSS_KILL);
            }
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case EVENT_SEVER_TENDOM:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                    me->CastSpell(target, 157165); // sever tendon

                events.ScheduleEvent(EVENT_SEVER_TENDOM, urand(11000, 17000));
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
class auchindon_creature_sargerei_spirit_tender : public CreatureScript
{
public:
    auchindon_creature_sargerei_spirit_tender() : CreatureScript("auchindon_creature_sargerei_spirit_tender") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* creature) : ScriptedAI(creature)
        {
        }

        InstanceScript* instance = me->GetInstanceScript();
        void Reset()
        {
            events.Reset();
        }
        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_VOID_MENDING, 10000);
            events.ScheduleEvent(EVENT_VOID_SHIFT, 16000);
        }
        void JustDied(Unit* /*killer*/) override
        {
            if (Creature* Kaathar = instance->instance->GetCreature(instance->GetData64(DATA_KATHAAR)))
            {
                if (Kaathar->isAlive())
                    Kaathar->AI()->DoAction(ACTION_COUNT_PRE_1ST_BOSS_KILL);
            }
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case EVENT_VOID_MENDING:
                if (Unit* friendUnit = DoSelectLowestHpFriendly(85)) // heal
                    me->CastSpell(friendUnit, SPELL_VOID_MENDING_DUMMY);

                events.ScheduleEvent(EVENT_VOID_MENDING, 10000);
                break;
            case EVENT_VOID_SHIFT:
                me->CastSpell(me, SPELL_VOID_SHIFT_DUMMY);
                events.ScheduleEvent(EVENT_VOID_SHIFT, 16000);     
                break;
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new auchindon_creatures(creature);
    };
};
class auchindon_creature_sargerei_hopilite : public CreatureScript
{
public:
    auchindon_creature_sargerei_hopilite() : CreatureScript("auchindon_creature_sargerei_hopilite") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* creature) : ScriptedAI(creature)
        {
        }

        InstanceScript* instance = me->GetInstanceScript();
        void Reset()
        {
            events.Reset();
        }
        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_SHIELD_BASH, urand(8000, 12000));
            events.ScheduleEvent(EVENT_VOID_STRIKES, 18000);
        }
        void JustDied(Unit* /*killer*/) override
        {
            if (Creature* Kaathar = instance->instance->GetCreature(instance->GetData64(DATA_KATHAAR)))
            {
                if (Kaathar->isAlive())
                Kaathar->AI()->DoAction(ACTION_COUNT_PRE_1ST_BOSS_KILL);
            }
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case EVENT_SHIELD_BASH:
                me->CastSpell(me->getVictim(), SPELL_SHIELD_BASH);
                events.ScheduleEvent(EVENT_SHIELD_BASH, urand(8000, 12000));
                break;
            case EVENT_VOID_STRIKES:
                me->CastSpell(me->getVictim(), SPELL_VOID_STRIKES);
                events.ScheduleEvent(EVENT_VOID_STRIKES, 18000);
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
class auchindon_creature_sargerei_defender : public CreatureScript
{
public:
    auchindon_creature_sargerei_defender() : CreatureScript("auchindon_creature_sargerei_defender") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* creature) : ScriptedAI(creature)
        {
        }
        InstanceScript* instance = me->GetInstanceScript();
        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_AVENGERS_SHIELD, urand(10000, 16000));
            events.ScheduleEvent(EVENT_CRUSADER_STIRKE, urand(5000, 9000));
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            switch (events.ExecuteEvent())
            {
            case EVENT_AVENGERS_SHIELD:
                if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 1.0f, true))
                {
                    me->CastSpell(random, SPELL_AVENGERS_SHIELD);
                    events.ScheduleEvent(EVENT_AVENGERS_SHIELD, urand(10000, 16000));
                }
                break;
            case EVENT_CRUSADER_STIRKE:
                if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 5.0f, true))
                {
                    me->CastSpell(random, SPELL_CRUSADER_STIRKE);
                    events.ScheduleEvent(EVENT_CRUSADER_STIRKE, urand(5000, 9000));
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
class auchindon_creature_sargerei_magus : public CreatureScript
{
public:
    auchindon_creature_sargerei_magus() : CreatureScript("auchindon_creature_sargerei_magus") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* creature) : ScriptedAI(creature)
        {
        }
        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_ARCANE_BOMB, urand(8000, 16000));

            std::list<Creature*> prisoners;

            me->GetCreatureListWithEntryInGrid(prisoners, CREATURE_AUCHENI_SOUL_PRIEST, 20.0f);

            if (prisoners.empty())
                return;

            for (auto itr : prisoners)
            {
                itr->RemoveAllAuras();
                itr->SetCanFly(false);
                itr->DespawnOrUnsummon(2000);
            }
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            switch (events.ExecuteEvent())
            {
            case EVENT_ARCANE_BOMB:
                me->CastSpell(me, SPELL_ARCANE_BOMB_DUMMY);
                events.ScheduleEvent(EVENT_ARCANE_BOMB, urand(8000, 16000));
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
class auchindon_creature_sargeri_warden : public CreatureScript
{
public:
    auchindon_creature_sargeri_warden() : CreatureScript("auchindon_creature_sargeri_warden") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* creature) : ScriptedAI(creature)
        {
        }
        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_WARDEN_HAMMER, urand(12000, 16000));
            events.ScheduleEvent(EVENT_WARDEN_CHAIN, 5000);
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            switch (events.ExecuteEvent())
            {
            case EVENT_WARDEN_HAMMER:
                if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                {
                    me->CastSpell(random, SPELL_WARDEN_THROW_HAMMER);
                    events.ScheduleEvent(EVENT_WARDEN_HAMMER, urand(12000, 16000));
                    break;
                }
            case EVENT_WARDEN_CHAIN:
                if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                {
                    me->CastSpell(random, 154831);
                    random->CastSpell(me, 154639);
                    random->AddAura(154263, random);
                }
                events.ScheduleEvent(EVENT_WARDEN_CHAIN, 20000);
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
class auchindon_creature_felborne_abyssal : public CreatureScript
{
public:
    auchindon_creature_felborne_abyssal() : CreatureScript("auchindon_creature_felborne_abyssal") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* creature) : ScriptedAI(creature)
        {
        }
        bool fixated;

        void Reset()
        {
            fixated = false;
        }
        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_FIXATE, urand(16000, 20000));
        }
        void DoAction(int32 const action)
        {
            switch (action)
            {
            case AUCTION_DEACTIVATE_FIXATION:
                fixated = false;
                break;
            }
        }
        void OnAddThreat(Unit* victim, float& fThreat, SpellSchoolMask /*schoolMask*/, SpellInfo const /*threatSpell*/)
        {
            if (fixated)
                fThreat = 0;
            return;
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            switch (events.ExecuteEvent())
            {
            case EVENT_FIXATE:
                if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                {
                    me->CastSpell(random, SPELL_FIXATE);
                    fixated = true;
                    events.ScheduleEvent(EVENT_FIXATE, urand(16000, 20000));
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
class auchindon_creature_cackling_pyromaniac : public CreatureScript
{
public:
    auchindon_creature_cackling_pyromaniac() : CreatureScript("auchindon_creature_cackling_pyromaniac") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* creature) : ScriptedAI(creature)
        {
        }

        InstanceScript* instance = me->GetInstanceScript();
        void Reset()
        {
            me->CastSpell(me, SPELL_ABYSSAL_VISUAL);
        }
        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_FELBLAST, 10000);
        }
        void JustDied(Unit* /*killer*/) override
        {
            if (Creature* Trigger = instance->instance->GetCreature(instance->GetData64(DATA_TRIGGER_AZZAKEL_CONTROLLER)))
                if (Trigger->IsWithinDistInMap(me, 30.0f))
                    Trigger->AI()->DoAction(ACTION_COUNT_PRE_3ST_BOSS_KILL);
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            switch (events.ExecuteEvent())
            {
            case EVENT_FELBLAST:
                me->CastSpell(me->getVictim(), SPELL_FELBLAST);
                events.ScheduleEvent(EVENT_FELBLAST, 10000);
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
class auchindon_creature_blazing_pyromaniac : public CreatureScript
{
public:
    auchindon_creature_blazing_pyromaniac() : CreatureScript("auchindon_creature_blazing_pyromaniac") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* creature) : ScriptedAI(creature)
        {
        }

        InstanceScript* instance = me->GetInstanceScript();
        void Reset()
        {
            events.ScheduleEvent(EVENT_RANDOM_MOVE, 2000);
            events.ScheduleEvent(EVENT_CONFLIGRATE, urand(8000, 15000));
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            switch (events.ExecuteEvent())
            {
            case EVENT_RANDOM_MOVE:
                me->GetMotionMaster()->MoveRandom(3.5f);
                events.ScheduleEvent(EVENT_RANDOM_MOVE, 3000);
                break;
            case EVENT_CONFLIGRATE:
                me->CastSpell(me, SPELL_CONFLIGIRATE);
                events.ScheduleEvent(EVENT_CONFLIGRATE, urand(8000, 15000));
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
class auchindon_creature_felguard : public CreatureScript
{
public:
    auchindon_creature_felguard() : CreatureScript("auchindon_creature_felguard") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* creature) : ScriptedAI(creature)
        {
        }

        InstanceScript* instance = me->GetInstanceScript();
        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_FEL_STOMP, 2000);
        }
        void JustDied(Unit* /*killer*/) override
        {
                if (Creature* Trigger = instance->instance->GetCreature(instance->GetData64(DATA_TRIGGER_AZZAKEL_CONTROLLER)))
                    if (Trigger->IsWithinDistInMap(me, 30.0f))
                        Trigger->AI()->DoAction(ACTION_COUNT_PRE_3ST_BOSS_KILL);
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            switch (events.ExecuteEvent())
            {
            case EVENT_FEL_STOMP:
                if (Unit* random = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                {
                    me->CastSpell(random, SPELL_FEL_STOMP);
                    events.ScheduleEvent(EVENT_FEL_STOMP, urand(8000, 16000));
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
class auchindon_spell_void_mending : public SpellScriptLoader
{
public:
    auchindon_spell_void_mending() : SpellScriptLoader("auchindon_spell_void_mending") { }

    class everbloom_spells : public SpellScript
    {
        PrepareSpellScript(everbloom_spells);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster())
                return;

            if (GetHitUnit())
            {
                GetCaster()->AddAura(SPELL_VOID_MENDING_AURA, GetHitUnit());
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(everbloom_spells::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new everbloom_spells();
    }
};
class auchindoun_void_shift : public SpellScriptLoader
{
public:
    auchindoun_void_shift() : SpellScriptLoader("auchindoun_void_shift") { }

    class auchindoun_spells : public SpellScript
    {
        PrepareSpellScript(auchindoun_spells);

        void HandleDummy(SpellEffIndex effIndex)
        {
            if (!GetCaster())
                return;

            std::list<Unit*> PL_list;

            JadeCore::AnyUnitInObjectRangeCheck check(GetCaster(), 30.0f);
            JadeCore::UnitListSearcher<JadeCore::AnyUnitInObjectRangeCheck> searcher(GetCaster(), PL_list, check);
            GetCaster()->VisitNearbyObject(30.0f, searcher);

            if (PL_list.empty())
                return;

            for (std::list<Unit*>::const_iterator it = PL_list.begin(); it != PL_list.end(); ++it)
            {
                if (!(*it))
                    return;

                if ((*it)->GetTypeId() == TYPEID_PLAYER)
                {
                    int32 calc = 5000;
                    GetCaster()->CastCustomSpell((*it), SPELL_VOID_SHIFT_DAMAGE, &calc, NULL, NULL, true, NULL);
                }
                else
                {
                    int32 calc = 8000;
                    GetCaster()->CastCustomSpell((*it), SPELL_VOID_SHIFT_HEALO, &calc, NULL, NULL, true, NULL);
                }
            }

        }

        void Register()
        {
            OnEffectLaunch += SpellEffectFn(auchindoun_spells::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new auchindoun_spells();
    }
};
/*
class spell_sever_tendon : public SpellScriptLoader
{
public:
    spell_sever_tendon() : SpellScriptLoader("spell_sever_tendon") { }

    class spells_auchindon : public SpellScript
    {
        PrepareSpellScript(spells_auchindon);

        void SeverTendonAddAuraAfterDummy()
        {
            if (!GetCaster() && !GetExplTargetUnit())
                return;

            GetCaster()->AddAura(SPELL_SEVER_TENDON_AURA, GetExplTargetUnit());
        }

        void Register()
        {
            AfterCast += SpellCastFn(spells_auchindon::SeverTendonAddAuraAfterDummy);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spells_auchindon();
    }
};
*/
// Called by    SPELL_ARCANE_BOMB_DUMMY = 157652,
class auchindon_spell_arcane_bomb_dummy : public SpellScriptLoader
{
public:
    auchindon_spell_arcane_bomb_dummy() : SpellScriptLoader("auchindon_spell_arcane_bomb_dummy") { }

    class auchindon_spells : public AuraScript
    {
        PrepareAuraScript(auchindon_spells);

        bool Load()
        {
            SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
            spell->Effects[0].TriggerSpell = 157791;
            return true;
        }
        void HandlePeriodic(constAuraEffectPtr /*aurEff*/)
        {
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
class auchindon_spell_void_shell_filter : public SpellScriptLoader
{
public:
    auchindon_spell_void_shell_filter() : SpellScriptLoader("auchindon_spell_void_shell_filter") { }

    class spells_auchindon : public SpellScript
    {
        PrepareSpellScript(spells_auchindon);

        void CorrectTargets(std::list<WorldObject*>& targets)
        {
            if (!GetCaster())
                return;

            targets.clear();

            std::list<Unit*> targetList;
            JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(GetCaster(), GetCaster(), 10.0f);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(GetCaster(), targetList, u_check);
            GetCaster()->VisitNearbyObject(10.0f, searcher);

            if (!targetList.empty())
            {
                for (auto itr : targetList)
                {
                    if (itr->GetTypeId() == TYPEID_PLAYER)
                        return;

                    targets.push_back(itr);
                }
            }
        }
        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spells_auchindon::CorrectTargets, EFFECT_0, TARGET_UNIT_TARGET_ANY);
        }
    };
    SpellScript* GetSpellScript() const
    {
        return new spells_auchindon();
    }
};
class auchindon_spell_fixate : public SpellScriptLoader
{
public:
    auchindon_spell_fixate() : SpellScriptLoader("auchindon_spell_fixate") { }

    class spell_auchindons : public AuraScript
    {
        PrepareAuraScript(spell_auchindons);

        void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (GetTarget())
                {
                    GetCaster()->GetMotionMaster()->MoveFollow(GetTarget(), 0, 0, MOTION_SLOT_ACTIVE);
                    GetCaster()->AddThreat(GetTarget(), 400.0f);
                }
            }
        }
        void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* caster = GetCaster())
            {
                if (GetTarget())
                {
                    GetCaster()->GetMotionMaster()->MovePoint(0, GetTarget()->GetPositionX(), GetTarget()->GetPositionY(), GetTarget()->GetPositionZ());
                }
            }
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_auchindons::OnApply, EFFECT_0, SPELL_AURA_MOD_POSSESS_PET, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_auchindons::OnRemove, EFFECT_0, SPELL_AURA_MOD_POSSESS_PET, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_auchindons();
    }
};
class auchindon_areatrigger_arcane_bomb : public AreaTriggerEntityScript
{
public:
    auchindon_areatrigger_arcane_bomb() : AreaTriggerEntityScript("auchindon_areatrigger_arcane_bomb")
    {
    }

    int diff = 9000;
    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        if (diff <= p_Time)
        {
            Position pos;
            p_AreaTrigger->GetPosition(&pos);
            p_AreaTrigger->SummonCreature(TRIGGER_ARCANE_BOMB_EXPLOSION_NPC, pos, TEMPSUMMON_MANUAL_DESPAWN);

            diff = 20000;
        }
        else
        {
            diff -= p_Time;
        }
    }
    void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
    }

    auchindon_areatrigger_arcane_bomb* GetAI() const
    {
        return new auchindon_areatrigger_arcane_bomb();
    }
};
class auchindon_spell_tuulani_unlock : public SpellScriptLoader
{
public:
    auchindon_spell_tuulani_unlock() : SpellScriptLoader("auchindon_spell_tuulani_unlock") { }

    class spells_auchindon : public SpellScript
    {
        PrepareSpellScript(spells_auchindon);

        void CorrectTargets(std::list<WorldObject*>& targets)
        {
            targets.clear();
            std::list<Creature*> list_targets;

            list_targets.clear();
            GetCaster()->GetCreatureListWithEntryInGrid(list_targets, TRIGGER_LIGHT_WALL_TARGETS, 15.0f);

            for (auto itr : list_targets)
            {
                targets.push_back(itr->ToUnit());
            }
        }
        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spells_auchindon::CorrectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
        }
    };
    SpellScript* GetSpellScript() const
    {
        return new spells_auchindon();
    }
};
// 150905, 158565, 
class auchindon_spell_beam_cosmetic : public SpellScriptLoader
{
public:
    auchindon_spell_beam_cosmetic() : SpellScriptLoader("auchindon_spell_beam_cosmetic") { }

    class spells_auchindon : public SpellScript
    {
        PrepareSpellScript(spells_auchindon);
        bool Load()
        {
            SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
            spell->Effects[0].TargetA = TARGET_UNIT_TARGET_ANY;
            spell->Effects[0].TargetB = 0;
            spell->AttributesEx9 = 0;
            spell->AttributesEx6 = 0;
            spell->AttributesEx5 = 0;
            spell->AttributesEx4 = 0;
            return true;
        }
        void CorrectTargets(std::list<WorldObject*>& targets)
        {
            if (!GetCaster())
                return;

            targets.clear();

            if (Creature* shadowbeamtrigger = GetCaster()->FindNearestCreature(TRIGGER_SHADOW_BEAM, 200.0f, true))
                targets.push_back(shadowbeamtrigger);
        }
        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spells_auchindon::CorrectTargets, EFFECT_0, TARGET_UNIT_NEARBY_ENTRY);
        }
    };
    SpellScript* GetSpellScript() const
    {
        return new spells_auchindon();
    }
};
// Triggers
class auchindon_trigger_arcane_bomb_npc_explosion : public CreatureScript
{
public:
    auchindon_trigger_arcane_bomb_npc_explosion() : CreatureScript("auchindon_trigger_arcane_bomb_npc_explosion") { }

    struct auchinond_triggers : public ScriptedAI
    {
        auchinond_triggers(Creature* creature) : ScriptedAI(creature)
        {
            Reset();
            me->Respawn(true);
        }
        void Reset()
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);

            me->setFaction(16);
            me->CastSpell(me, SPELL_ARCANE_BOMB_DMG, true);
            me->DespawnOrUnsummon(5000);
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new auchinond_triggers(creature);
    }
};
class auchindon_trigger_warden_hammer : public CreatureScript
{
public:
    auchindon_trigger_warden_hammer() : CreatureScript("auchindon_trigger_warden_hammer") { }

    struct auchindon_creatures : public ScriptedAI
    {
        auchindon_creatures(Creature* creature) : ScriptedAI(creature)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->setFaction(16);
        }
        uint32 diffp = 500;
        void UpdateAI(const uint32 diff)
        {
            if (diffp <= diff)
            {
                std::list<Player*> nerbyplayers;

                me->GetPlayerListInGrid(nerbyplayers, 3.0f);
                for (auto itr : nerbyplayers)
                {
                    me->CastSpell(itr, SPELL_WARDEN_HAMMER_LIGHTNING_TEMPO);
                    me->CastSpell(itr, SPELL_WARDEN_HAMMER_DAMAGE);
                }

                diffp = 1000;
            }
            else
                diffp -= diff;
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new auchindon_creatures(creature);
    }
};
void AddSC_AuchindounNEW()
{
    new auchindon_mob_trigger_cosmetic();
    new auchindon_creature_tuulani();
    new auchindon_creature_sargerei_soulbinder();
    new auchindon_creature_sargerei_cleric();
    new auchindon_creature_sargerei_ritualist();
    new auchindon_creature_sargerei_zealot();
    new auchindon_creature_sargerei_spirit_tender();
    new auchindon_creature_sargerei_hopilite();
    new auchindon_creature_felborne_abyssal();
    new auchindon_creature_felguard();
    new auchindon_creature_cackling_pyromaniac();
    new auchindon_creature_blazing_pyromaniac();
    new auchindon_creature_sargeri_warden();
    new auchindon_creature_sargerei_magus();
    new auchindon_creature_sargerei_defender();
    new auchindon_trigger_arcane_bomb_npc_explosion();
    new auchindon_trigger_warden_hammer();
    new auchindon_areatrigger_arcane_bomb();
    new auchindoun_void_shift();
    new auchindon_spell_void_mending();
    new auchindon_spell_void_shell_filter();
    new auchindon_spell_arcane_bomb_dummy();
    new auchindon_spell_fixate();
    new auchindon_spell_tuulani_unlock();
    new auchindon_spell_beam_cosmetic();
}