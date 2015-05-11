#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "iron_docks.h"
#include "Vehicle.h"

enum ETalks
{
    TALK_ZOGGOSH_03 = 12, // Sir.. they're messing with the Iron Stars (44050)
    TALK_KORAMAR_010= 25, // Hah! will you look on that, maybe I understimated them.. Zoggosh, prepare Skuloc let them have a taste of what he has to offer. (46901)
    TALK_ZOGGOSH_04 = 13, // Igniting the cannon sir! (44051)
    TALK_ZOGGOSH_05 = 14, // Sir.. he's out of ammunition. I'm reloading.. I'm reloading!! (44052)
    TALK_KORAMAR_04 = 26, // Fire! Fire! Aim... then fire.. (46903)
    TALK_KORAMAR_05 = 27, // What is wrong with you?! keep firing you all! (46904)
};
enum Eskills
{
    SPELL_CANNON_BARRAGE_EXPLOSION_FULL = 168541,
};
class iron_stars_event : public BasicEvent
{
public:
    explicit iron_stars_event(Unit* unit, int value) : obj(unit), modifier(value)
    {
    }

    bool Execute(uint64 /*currTime*/, uint32 /*diff*/)
    {
        if (InstanceScript* instance = obj->GetInstanceScript())
        {
            if (Creature* Zoggosh = instance->instance->GetCreature(instance->GetData64(DATA_ZUGGOSH)))
            {
                if (Creature* Koramar = instance->instance->GetCreature(instance->GetData64(DATA_KORAMAR)))
                {
                    switch (modifier)
                    {
                    case 0:
                        Zoggosh->AI()->Talk(TALK_ZOGGOSH_03);
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
class bombard_event : public BasicEvent
{
public:
    explicit bombard_event(Unit* unit, int value) : obj(unit), modifier(value)
    {
    }

    bool Execute(uint64 /*currTime*/, uint32 /*diff*/)
    {
        if (InstanceScript* instance = obj->GetInstanceScript())
        {
            if (Creature* Zoggosh = instance->instance->GetCreature(instance->GetData64(DATA_ZUGGOSH)))
            {
                if (Creature* Koramar = instance->instance->GetCreature(instance->GetData64(DATA_KORAMAR)))
                {
                    if (Creature* Skulloc = instance->instance->GetCreature(instance->GetData64(DATA_SKULLOC)))
                    {
                        switch (modifier)
                        {
                        case 0:
                            Koramar->AI()->Talk(TALK_KORAMAR_010);
                            Koramar->m_Events.AddEvent(new bombard_event(Koramar, 1), Koramar->m_Events.CalculateTime(8000));
                            break;
                        case 1:
                            Zoggosh->AI()->Talk(TALK_ZOGGOSH_04);
                            Zoggosh->m_Events.AddEvent(new bombard_event(Zoggosh, 9), Zoggosh->m_Events.CalculateTime(2000));
                            break;
                        case 9:
                            if (Creature* turret = Skulloc->GetVehicleKit()->GetPassenger(0)->ToCreature())
                            {
                                turret->CastSpell(turret, 168537);
                                turret->m_Events.AddEvent(new bombard_event(turret, 9), turret->m_Events.CalculateTime(7000));
                            }
                            break;
                        }
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

class instance_iron_docks : public InstanceMapScript
{
public:
    instance_iron_docks() : InstanceMapScript("instance_iron_docks", 1195) { }

    struct instance_iron_docks_InstanceMapScript : public InstanceScript
    {
        instance_iron_docks_InstanceMapScript(InstanceMap* map) : InstanceScript(map)  { }

        InstanceScript* Pinstance = this;
        EventMap events;
        uint64 makogGUID;
        uint64 duguruGUID;
        uint64 noxxGUID;
        uint64 nokgarGUID;
        uint64 wolfGUID;
        uint64 oshirGUID;
        uint64 skullocGUID;
        uint64 zoggoshGUID;
        uint64 koramarGUID;
        uint64 darunaGUID;
        uint64 gwarnokGUID;
        uint64 olugarGUID;
        uint64 turretGUID;
        bool secondEvent; // iron stars
        bool thirdEvent;

        void Initialize()
        {
            SetBossNumber(4);
            // bosses
            makogGUID = 0;
            duguruGUID = 0;
            noxxGUID = 0;
            nokgarGUID = 0;
            wolfGUID = 0;
            oshirGUID = 0;
            skullocGUID = 0;
            // mini bosses
            darunaGUID = 0;
            gwarnokGUID = 0;
            olugarGUID = 0;
            // triggers
            // gobs
            // creatures
            turretGUID = 0;
            zoggoshGUID = 0;
            koramarGUID = 0;
            secondEvent = false;
            thirdEvent = false;
        }
        void OnCreatureCreate(Creature* creature)
        {
            switch (creature->GetEntry())
            {
            case BOSS_FLESHRENDER_NOKGAR:
                nokgarGUID = creature->GetGUID();
                break;
            case BOSS_OSHIR:
                oshirGUID = creature->GetGUID();
                break;
            case BOSS_SKULLOC:
                skullocGUID = creature->GetGUID();
                break;
            case NPC_NOX:
                noxxGUID = creature->GetGUID();
                break;
            case NPC_MAKOGG:
                makogGUID = creature->GetGUID();
                break;
            case NPC_DUGURU:
                duguruGUID = creature->GetGUID();
                break;
            case BOSS_DREADFANG:
                wolfGUID = creature->GetGUID();
                break;
            case NPC_ZOGGOSH:
                zoggoshGUID = creature->GetGUID();
                break;
            case NPC_KORAMAR:
                koramarGUID = creature->GetGUID();
                break;      
            case NPC_CHAMPION_DRUNA:
                darunaGUID = creature->GetGUID();
                break;
            case NPC_PITWARDEN_GWARNOK:
                gwarnokGUID = creature->GetGUID();
                break;
            case NPC_SIEGE_MASTER_OLUGAR:
                olugarGUID = creature->GetGUID();
                break;
            case BOSS_TURRET:
                turretGUID = creature->GetGUID();
                break;
            default:
                break;
            }
        }
        void OnUnitDeath(Unit* unit)
        {
            Creature* creature = unit->ToCreature();
            if (!creature)
                return;

            switch (creature->GetEntry())
            {
                // Achievement ACHIEVEMENT_MILITARSTIC_EXPANSIONIST = 9083,
            case BOSS_FLESHRENDER_NOKGAR:
            {
                if (Creature* daruna = instance->GetCreature(Pinstance->GetData64(DATA_DARUNA)))
                    if (Creature* olugar = instance->GetCreature(Pinstance->GetData64(DATA_OLUGAR)))
                        if (Creature* gwarnok = instance->GetCreature(Pinstance->GetData64(DATA_GWARNOK)))
                            if (gwarnok->isAlive() && olugar->isAlive() && daruna->isAlive())
                            {
                                Pinstance->DoCompleteAchievement(9083);                           
                            }
                break;
            }
            default:
                break;
            }
        }
        void OnGameObjectCreate(GameObject* go)
        {
            switch (go->GetEntry())
            {
            default:
                break;
            }
        }
        void OnGameObjectRemove(GameObject* go)
        {
            switch (go->GetEntry())
            {
 
            default:
                break;
            }
        }
        void SetData(uint32 type, uint32 data)
        {
            switch (type)
            {
                case DATA_SECOND_EVENT:
                    if (!secondEvent)
                    {
                        secondEvent = true;

                        if (Creature* Skullock = instance->GetCreature(Pinstance->GetData64(DATA_SKULLOC)))
                            Skullock->m_Events.AddEvent(new iron_stars_event(Skullock, 0), Skullock->m_Events.CalculateTime(8000));
                        break;
                    }       
                case DATA_THIRD_EVENT:
                {
                    if (!thirdEvent)
                    {
                        thirdEvent = true;

                        if(Creature* Skullock = instance->GetCreature(Pinstance->GetData64(DATA_SKULLOC)))
                            Skullock->m_Events.AddEvent(new bombard_event(Skullock, 0), Skullock->m_Events.CalculateTime(8000));
                        break;
                    }
                    break;
                }   
            }
        }
        void Update(uint32 diff)
        {
            Events.Update(diff);

            while (uint32 eventId = Events.ExecuteEvent())
            {
                switch (eventId)
                {
                default:
                    break;
                }
            }
        }
        uint32 GetData(uint32 type)
        {
            switch (type)
            {
            default:
                break;
            }

            return 0;
        }
        uint64 GetData64(uint32 type)
        {
            switch (type)
            {
                // Bosses
            case DATA_NOKGAR:
                return nokgarGUID;
                break;
            case DATA_OSHIR:
                return oshirGUID;
                break;
            case DATA_SKULLOC:
                return skullocGUID;
                break;
            case DATA_GRIMRAIL_DUGURU:
                return duguruGUID;
                break;
            case DATA_GRIMRAIL_MAKOGG:
                return makogGUID;
                break;
            case DATA_GRIMRAIL_NOXX:
                return noxxGUID;
                break;
            case DATA_MOUNT_WOLF:
                return wolfGUID;
                break;
            case DATA_TURRET:
                return turretGUID;
                break;
                // Mini Bosses
            case DATA_DARUNA:
                return darunaGUID;
                break;
            case DATA_GWARNOK:
                return gwarnokGUID;
                break;
            case DATA_OLUGAR:
                return olugarGUID;
                break;
                // RP
            case DATA_ZUGGOSH:
                return zoggoshGUID;
                break;
            case DATA_KORAMAR:
                return koramarGUID;
                break;
            }
            return 0;
        }
        bool SetBossState(uint32 type, EncounterState state)
        {
            if (!InstanceScript::SetBossState(type, state))
                return false;

            switch (type)
            {
                /*
            case DATA_GRIMRAIL_MAKOGG:
                if (state == DONE)
                    if (Creature* Skullock = instance->GetCreature(Pinstance->GetData64(DATA_SKULLOC)))
                        Skullock->m_Events.AddEvent(new aftergrimrail_event(Skullock, 0), Skullock->m_Events.CalculateTime(8000));
                break;
                */
            }

            return true;
        }
        bool IsWipe()
        {
            Map::PlayerList const& PlayerList = instance->GetPlayers();

            if (PlayerList.isEmpty())
            {
                return true;
            }

            return false;
        }

    protected:
        EventMap Events;

    };

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_iron_docks_InstanceMapScript(map);
    }
};

void AddSC_instance_iron_docks()
{
    new instance_iron_docks();
}
