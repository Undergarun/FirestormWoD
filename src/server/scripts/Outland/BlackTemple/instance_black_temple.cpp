////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Instance_Black_Temple
SD%Complete: 100
SDComment: Instance Data Scripts and functions to acquire mobs and set encounter status for use in various Black Temple Scripts
SDCategory: Black Temple
EndScriptData */

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "black_temple.h"

#define MAX_ENCOUNTER      9

/* Black Temple encounters:
0 - High Warlord Naj'entus event
1 - Supremus Event
2 - Shade of Akama Event
3 - Teron Gorefiend Event
4 - Gurtogg Bloodboil Event
5 - Reliquary Of Souls Event
6 - Mother Shahraz Event
7 - Illidari Council Event
8 - Illidan Stormrage Event
*/

class instance_black_temple : public InstanceMapScript
{
public:
    instance_black_temple() : InstanceMapScript("instance_black_temple", 564) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_black_temple_InstanceMapScript(map);
    }

    struct instance_black_temple_InstanceMapScript : public InstanceScript
    {
        instance_black_temple_InstanceMapScript(Map* map) : InstanceScript(map) {}

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string str_data;

        uint64 Najentus;
        uint64 Akama;                                           // This is the Akama that starts the Illidan encounter.
        uint64 Akama_Shade;                                     // This is the Akama that starts the Shade of Akama encounter.
        uint64 ShadeOfAkama;
        uint64 Supremus;
        uint64 LadyMalande;
        uint64 GathiosTheShatterer;
        uint64 HighNethermancerZerevor;
        uint64 VerasDarkshadow;
        uint64 IllidariCouncil;
        uint64 BloodElfCouncilVoice;
        uint64 IllidanStormrage;

        uint64 NajentusGate;
        uint64 MainTempleDoors;
        uint64 ShadeOfAkamaDoor;
        uint64 CommonDoor;//Teron
        uint64 TeronDoor;
        uint64 GuurtogDoor;
        uint64 MotherDoor;
        uint64 TempleDoor;//Befor mother
        uint64 CouncilDoor;
        uint64 SimpleDoor;//council
        uint64 IllidanGate;
        uint64 IllidanDoor[2];

        void Initialize()
        {
            memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

            Najentus = 0;
            Akama = 0;
            Akama_Shade = 0;
            ShadeOfAkama = 0;
            Supremus = 0;
            LadyMalande = 0;
            GathiosTheShatterer = 0;
            HighNethermancerZerevor = 0;
            VerasDarkshadow = 0;
            IllidariCouncil = 0;
            BloodElfCouncilVoice = 0;
            IllidanStormrage = 0;

            NajentusGate    = 0;
            MainTempleDoors = 0;
            ShadeOfAkamaDoor= 0;
            CommonDoor              = 0;//teron
            TeronDoor               = 0;
            GuurtogDoor             = 0;
            MotherDoor              = 0;
            TempleDoor              = 0;
            SimpleDoor              = 0;//Bycouncil
            CouncilDoor             = 0;
            IllidanGate     = 0;
            IllidanDoor[0]  = 0;
            IllidanDoor[1]  = 0;
        }

        bool IsEncounterInProgress() const
        {
            for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                if (m_auiEncounter[i] == IN_PROGRESS)
                    return true;

            return false;
        }

        Player* GetPlayerInMap()
        {
            Map::PlayerList const& players = instance->GetPlayers();

            if (!players.isEmpty())
            {
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                {
                    if (Player* player = itr->getSource())
                        return player;
                }
            }

            sLog->outDebug(LOG_FILTER_TSCR, "Instance Black Temple: GetPlayerInMap, but PlayerList is empty!");
            return NULL;
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch (creature->GetEntry())
            {
            case 22887:    Najentus = creature->GetGUID();                  break;
            case 23089:    Akama = creature->GetGUID();                     break;
            case 22990:    Akama_Shade = creature->GetGUID();               break;
            case 22841:    ShadeOfAkama = creature->GetGUID();              break;
            case 22898:    Supremus = creature->GetGUID();                  break;
            case 22917:    IllidanStormrage = creature->GetGUID();          break;
            case 22949:    GathiosTheShatterer = creature->GetGUID();       break;
            case 22950:    HighNethermancerZerevor = creature->GetGUID();   break;
            case 22951:    LadyMalande = creature->GetGUID();               break;
            case 22952:    VerasDarkshadow = creature->GetGUID();           break;
            case 23426:    IllidariCouncil = creature->GetGUID();           break;
            case 23499:    BloodElfCouncilVoice = creature->GetGUID();      break;
            }
        }

        void OnGameObjectCreate(GameObject* go)
        {
            switch (go->GetEntry())
            {
            case 185483:
                NajentusGate = go->GetGUID();// Gate past Naj'entus (at the entrance to Supermoose's courtyards)
                if (m_auiEncounter[0] == DONE)
                    HandleGameObject(0, true, go);
                break;

            case 185882:
                MainTempleDoors = go->GetGUID();// Main Temple Doors - right past Supermoose (Supremus)
                if (m_auiEncounter[1] == DONE)
                    HandleGameObject(0, true, go);
                break;

            case 185478:
                ShadeOfAkamaDoor = go->GetGUID();
                break;

            case 185480:
                CommonDoor = go->GetGUID();
                if (m_auiEncounter[3] == DONE)
                    HandleGameObject(0, true, go);
                break;

            case 186153:
                TeronDoor = go->GetGUID();
                if (m_auiEncounter[3] == DONE)
                    HandleGameObject(0, true, go);
                break;

            case 185892:
                GuurtogDoor = go->GetGUID();
                if (m_auiEncounter[4] == DONE)
                    HandleGameObject(0, true, go);
                break;

            case 185479:
                TempleDoor = go->GetGUID();
                if (m_auiEncounter[5] == DONE)
                    HandleGameObject(0, true, go);
                break;

            case 185482:
                MotherDoor = go->GetGUID();
                if (m_auiEncounter[6] == DONE)
                    HandleGameObject(0, true, go);
                break;

            case 185481:
                CouncilDoor = go->GetGUID();
                if (m_auiEncounter[7] == DONE)
                    HandleGameObject(0, true, go);
                break;

            case 186152:
                SimpleDoor = go->GetGUID();
                if (m_auiEncounter[7] == DONE)
                    HandleGameObject(0, true, go);
                break;

            case 185905:
                IllidanGate = go->GetGUID(); // Gate leading to Temple Summit
                break;

            case 186261:
                IllidanDoor[0] = go->GetGUID(); // Right door at Temple Summit
                break;

            case 186262:
                IllidanDoor[1] = go->GetGUID(); // Left door at Temple Summit
                break;
            }
        }

        uint64 GetData64(uint32 identifier)
        {
            switch (identifier)
            {
            case DATA_HIGH_WARLORD_NAJENTUS:        return Najentus;
            case DATA_AKAMA:                        return Akama;
            case DATA_AKAMA_SHADE:                  return Akama_Shade;
            case DATA_SHADE_OF_AKAMA:               return ShadeOfAkama;
            case DATA_SUPREMUS:                     return Supremus;
            case DATA_ILLIDAN_STORMRAGE:            return IllidanStormrage;
            case DATA_GATHIOS_THE_SHATTERER:        return GathiosTheShatterer;
            case DATA_HIGH_NETHERMANCER_ZEREVOR:    return HighNethermancerZerevor;
            case DATA_LADY_MALANDE:                 return LadyMalande;
            case DATA_VERAS_DARKSHADOW:             return VerasDarkshadow;
            case DATA_ILLIDARI_COUNCIL:             return IllidariCouncil;
            case DATA_GAMEOBJECT_NAJENTUS_GATE:     return NajentusGate;
            case DATA_GAMEOBJECT_ILLIDAN_GATE:      return IllidanGate;
            case DATA_GAMEOBJECT_ILLIDAN_DOOR_R:    return IllidanDoor[0];
            case DATA_GAMEOBJECT_ILLIDAN_DOOR_L:    return IllidanDoor[1];
            case DATA_GAMEOBJECT_SUPREMUS_DOORS:    return MainTempleDoors;
            case DATA_BLOOD_ELF_COUNCIL_VOICE:      return BloodElfCouncilVoice;
            }

            return 0;
        }

        void SetData(uint32 type, uint32 data)
        {
            switch (type)
            {
            case DATA_HIGH_WARLORD_NAJENTUS_EVENT:
                if (data == DONE)
                    HandleGameObject(NajentusGate, true);
                m_auiEncounter[0] = data;
                break;
            case DATA_SUPREMUS_EVENT:
                if (data == DONE)
                    HandleGameObject(NajentusGate, true);
                m_auiEncounter[1] = data;
                break;
            case DATA_SHADE_OF_AKAMA_EVENT:
                if (data == IN_PROGRESS)
                    HandleGameObject(ShadeOfAkamaDoor, false);
                else
                    HandleGameObject(ShadeOfAkamaDoor, true);
                m_auiEncounter[2] = data;
                break;
            case DATA_TERON_GOREFIEND_EVENT:
                if (data == IN_PROGRESS)
                {
                    HandleGameObject(TeronDoor, false);
                    HandleGameObject(CommonDoor, false);
                }
                else
                {
                    HandleGameObject(TeronDoor, true);
                    HandleGameObject(CommonDoor, true);
                }
                m_auiEncounter[3] = data;
                break;
            case DATA_GURTOGG_BLOOD_BOIL_EVENT:
                if (data == DONE)
                    HandleGameObject(GuurtogDoor, true);
                m_auiEncounter[4] = data;
                break;
            case DATA_RELIQUARY_OF_SOULS_EVENT:
                if (data == DONE)
                    HandleGameObject(TempleDoor, true);
                m_auiEncounter[5] = data;
                break;
            case DATA_MOTHER_SHAHRAZ_EVENT:
                if (data == DONE)
                    HandleGameObject(MotherDoor, true);
                m_auiEncounter[6] = data;
                break;
            case DATA_ILLIDARI_COUNCIL_EVENT:
                if (data == IN_PROGRESS)
                {
                    HandleGameObject(CouncilDoor, false);
                    HandleGameObject(SimpleDoor, false);
                }
                else
                {
                    HandleGameObject(CouncilDoor, true);
                    HandleGameObject(SimpleDoor, true);
                }
                m_auiEncounter[7] = data;
                break;
            case DATA_ILLIDAN_STORMRAGE_EVENT:
                m_auiEncounter[8] = data;
                break;
            }

            if (data == DONE)
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << m_auiEncounter[0] << ' ' << m_auiEncounter[1] << ' '
                    << m_auiEncounter[2] << ' ' << m_auiEncounter[3] << ' ' << m_auiEncounter[4]
                << ' ' << m_auiEncounter[5] << ' ' << m_auiEncounter[6] << ' ' << m_auiEncounter[7]
                << ' ' << m_auiEncounter[8];

                str_data = saveStream.str();

                SaveToDB();
                OUT_SAVE_INST_DATA_COMPLETE;
            }
        }

        uint32 GetData(uint32 type)
        {
            switch (type)
            {
            case DATA_HIGH_WARLORD_NAJENTUS_EVENT:       return m_auiEncounter[0];
            case DATA_SUPREMUS_EVENT:                    return m_auiEncounter[1];
            case DATA_SHADE_OF_AKAMA_EVENT:              return m_auiEncounter[2];
            case DATA_TERON_GOREFIEND_EVENT:             return m_auiEncounter[3];
            case DATA_GURTOGG_BLOOD_BOIL_EVENT:          return m_auiEncounter[4];
            case DATA_RELIQUARY_OF_SOULS_EVENT:          return m_auiEncounter[5];
            case DATA_MOTHER_SHAHRAZ_EVENT:              return m_auiEncounter[6];
            case DATA_ILLIDARI_COUNCIL_EVENT:            return m_auiEncounter[7];
            case DATA_ILLIDAN_STORMRAGE_EVENT:           return m_auiEncounter[8];
            }

            return 0;
        }

       std::string GetSaveData()
        {
            return str_data;
        }

        void Load(const char* in)
        {
            if (!in)
            {
                OUT_LOAD_INST_DATA_FAIL;
                return;
            }

            OUT_LOAD_INST_DATA(in);

            std::istringstream loadStream(in);
            loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2]
            >> m_auiEncounter[3] >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6]
            >> m_auiEncounter[7] >> m_auiEncounter[8];

            for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                if (m_auiEncounter[i] == IN_PROGRESS)
                    m_auiEncounter[i] = NOT_STARTED;

            OUT_LOAD_INST_DATA_COMPLETE;
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_instance_black_temple()
{
    new instance_black_temple();
}
#endif
