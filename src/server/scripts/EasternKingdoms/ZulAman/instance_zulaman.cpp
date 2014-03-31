#include "ScriptPCH.h"
#include "zulaman.h"

#define MAX_ENCOUNTER 6

static const DoorData doordata[] = 
{
    {GO_AKILZON_EXIT,           DATA_AKILZON,            DOOR_TYPE_ROOM,     BOUNDARY_NONE},
    {GO_HALAZZI_ENTRANCE,       DATA_HALAZZI,            DOOR_TYPE_ROOM,     BOUNDARY_NONE},
    {GO_HALAZZI_EXIT,           DATA_HALAZZI,            DOOR_TYPE_PASSAGE,  BOUNDARY_NONE},
    {GO_MALACRASS_EXIT,         DATA_HEX_LORD_MALACRASS, DOOR_TYPE_PASSAGE,  BOUNDARY_NONE},
    {GO_DAAKARA_EXIT,           DATA_DAAKARA,            DOOR_TYPE_ROOM,     BOUNDARY_NONE},
};

class instance_zulaman : public InstanceMapScript
{
    public:
        instance_zulaman() : InstanceMapScript("instance_zulaman", 568){}
        
        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_zulaman_InstanceMapScript(map);
        }

        struct instance_zulaman_InstanceMapScript : public InstanceScript
        {
            instance_zulaman_InstanceMapScript(Map* map) : InstanceScript(map) 
            {
                SetBossNumber(MAX_ENCOUNTER);
                LoadDoorData(doordata);

                HexLordGateGUID = 0;
                MainGateGUID    = 0;
                StrangeGongGUID = 0;

                QuestTimer = 0;
                QuestMinute = 21;
                uiMainGate = 0;
                uiVendor1 = 0;
                uiVendor2 = 0;

                for (uint8 i = 0; i < 4; ++i)
                {
                    _hostages[i].realGUID = 0;
                    _hostages[i].corpseGUID = 0;
                    _hostages[i].lootGUID = 0;
                    _hostages[i].state = 0;
                }
            }

            void OnCreatureCreate(Creature* pCreature)
            {
                switch (pCreature->GetEntry())
                {
                    case NPC_AMANISHI_TEMPEST:
                        AmanishiTempestGUID = pCreature->GetGUID();
                        break;
                    case NPC_BAKKALZU:
                        _hostages[0].realGUID = pCreature->GetGUID();
                        _hostages[0].state = 0;
                        break;
                    case NPC_BAKKALZU_CORPSE:
                        _hostages[0].corpseGUID = pCreature->GetGUID();
                        pCreature->SetVisible(false);
                        break;
                    case NPC_HAZLEK:
                        _hostages[1].realGUID = pCreature->GetGUID();
                        _hostages[1].state = 0;
                        break;
                    case NPC_HAZLEK_CORPSE:
                        _hostages[1].corpseGUID = pCreature->GetGUID();
                        pCreature->SetVisible(false);
                        break;
                    case NPC_NORKANI:
                        _hostages[2].realGUID = pCreature->GetGUID();
                        _hostages[2].state = 0;
                        break;
                    case NPC_NORKANI_CORPSE:
                        _hostages[2].corpseGUID = pCreature->GetGUID();
                        pCreature->SetVisible(false);
                        break;
                    case NPC_KASHA:
                        _hostages[3].realGUID = pCreature->GetGUID();
                        _hostages[3].state = 0;
                        break;
                    case NPC_KASHA_CORPSE:
                        _hostages[3].corpseGUID = pCreature->GetGUID();
                        pCreature->SetVisible(false);
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* pGo)
            {
                switch (pGo->GetEntry())
                {
                    case GO_STRANGE_GONG:
                        StrangeGongGUID = pGo->GetGUID();
                        if (uiMainGate == 1)
                            pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                        break;
                    case GO_MAIN_GATE:
                        MainGateGUID = pGo->GetGUID();
                        if (uiMainGate == 1)
                            HandleGameObject(MainGateGUID, true);
                        break;
                    case GO_AKILZON_EXIT:
                        AddDoor(pGo, true);
                        break;
                    case GO_HALAZZI_ENTRANCE:
                        AddDoor(pGo, true);
                        break;
                    case GO_HALAZZI_EXIT:
                        AddDoor(pGo, true);
                        break;
                    case GO_MALACRASS_ENTRANCE:
                        HexLordGateGUID = pGo->GetGUID(); 
                        if (GetBossesDone() >= 2)
                            HandleGameObject(HexLordGateGUID, true, pGo);
                        break;
                    case GO_MALACRASS_EXIT:
                        AddDoor(pGo, true);
                        break;
                    case GO_DAAKARA_EXIT:
                        AddDoor(pGo, true);
                        break;
                    case GO_HAZLEK_TRUNK:
                        _hostages[1].lootGUID = pGo->GetGUID();
                        break;
                    case GO_NORKANI_PACKAGE:
                        _hostages[2].lootGUID = pGo->GetGUID();
                        break;
                    case GO_KASHA_BAG:
                        //_hostages[3].lootGUID = pGo->GetGUID();
                        uiKashaBagGUID = pGo->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            std::string GetSaveData()
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream ss;
                ss << "ZA " << GetBossSaveData() << uiMainGate << " " << QuestMinute << " " << uiVendor1 << " " << uiVendor2 << " ";

                OUT_SAVE_INST_DATA_COMPLETE;
                return ss.str();
            }

            void Load(const char* in)
            {
                if (!in)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(in);

                char dataHead1, dataHead2;

                std::istringstream loadStream(in);
                loadStream >> dataHead1 >> dataHead2;

                if (dataHead1 == 'Z' && dataHead2 == 'A')
                {
                    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
				    {
					    uint32 tmpState;
					    loadStream >> tmpState;
					    if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
						    tmpState = NOT_STARTED;
					    SetBossState(i, EncounterState(tmpState));
				    }
                    loadStream >> uiMainGate;
                    loadStream >> QuestMinute;
                    DoUpdateWorldState(3104, QuestMinute);
                    loadStream >> uiVendor1;
                    loadStream >> uiVendor2;

                } else OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }
            
            bool SetBossState(uint32 id, EncounterState state)
            {
                if (!InstanceScript::SetBossState(id, state))
                    return false;

                if (state == DONE)
                {
                    switch (id)
                    {
                        case DATA_AKILZON:
                        case DATA_NALORAKK:
                        case DATA_JANALAI:
                        case DATA_HALAZZI:
                        {
                            if (id == DATA_AKILZON) QuestMinute += 15;
                            else if (id == DATA_NALORAKK) QuestMinute += 10;
                            DoUpdateWorldState(3106, QuestMinute);

                            if (_hostages[id].state == 0)
                            {
                                _hostages[id].state = 1;
                                if (_hostages[id].lootGUID)
                                    DoRespawnGameObject(_hostages[id].lootGUID, DAY);
                            }

                            uint8 _bosses = GetBossesDone();

                            if (_bosses >= 2)
                                HandleGameObject(HexLordGateGUID, true);

                            if (_bosses >= 4)
                            {
                                if (QuestMinute)
                                {
                                    DoCastSpellOnPlayers(SPELL_ZULAMAN_ACHIEVEMENT);
                                    if (uiKashaBagGUID)
                                        DoRespawnGameObject(uiKashaBagGUID, DAY);
                                    QuestMinute = 0;
                                    DoUpdateWorldState(3104, 0);
                                }
                            }
                            break;
                        }
                        default:
                            break;
                    }
                }
                return true;
            }

            void SetData(uint32 type, uint32 data)
            {
                switch (type)
                {
                    case DATA_MAIN_GATE:
                        uiMainGate = data;
                        if (data == 1)
                        {
                            HandleGameObject(MainGateGUID, true);
                            if (GameObject* pGo = instance->GetGameObject(StrangeGongGUID))
                                pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                            SaveToDB();
                        }
                        break;
                    case DATA_VENDOR_1:
                        uiVendor1 = data;
                        SaveToDB();
                        break;
                    case DATA_VENDOR_2:
                        uiVendor2 = data;
                        SaveToDB();
                        break;
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 type)
            {
                switch (type)
                {
                    case DATA_MAIN_GATE:
                        return MainGateGUID;
                    case DATA_TEMPEST:
                        return AmanishiTempestGUID;
                    case DATA_VENDOR_1:
                        return uiVendor1;
                    case DATA_VENDOR_2:
                        return uiVendor2;
                    default: 
                        return 0;
                }
            }

            void Update(uint32 diff)
            {
                if (QuestMinute)
                {
                    if (QuestTimer <= diff)
                    {
                        QuestMinute--;
                        SaveToDB();
                        QuestTimer += 60000;
                        if (QuestMinute)
                        {
                            DoUpdateWorldState(3104, 1);
                            DoUpdateWorldState(3106, QuestMinute);
                        } 
                        else
                        {
                            DoUpdateWorldState(3104, 0);
                            for (uint8 i = 0; i < 4; ++i)
                            {
                                if (Creature* pHostage = instance->GetCreature(_hostages[i].realGUID))
                                {
                                    if (_hostages[i].state != 0)
                                        continue;

                                    pHostage->DespawnOrUnsummon();
                                    if (Creature* pCorpse = instance->GetCreature(_hostages[i].corpseGUID))
                                        pCorpse->SetVisible(true);
                                    _hostages[i].state = 2;
                                }
                            }
                                    
                        }

                    }
                    QuestTimer -= diff;
                }
            }

        private:
            uint64 HexLordGateGUID;
            uint64 MainGateGUID;
            uint64 StrangeGongGUID;
            uint64 AmanishiTempestGUID;
            uint64 uiKashaBagGUID;

            uint32 uiMainGate;
            uint32 uiVendor1;
            uint32 uiVendor2;
            uint32 QuestTimer;
            uint16 QuestMinute;

            uint8 GetBossesDone()
            {
                uint8 _bosses = 0;
                for (uint8 i = 0; i < 4; ++i)
                    if (GetBossState(i) == DONE)
                        _bosses++;
                return _bosses;
            }

            struct Hostage
            {
                uint64 realGUID;
                uint64 corpseGUID;
                uint64 lootGUID;
                uint8 state; // 0 - neutral, 1 - saved, 2 - killed
            };

            Hostage _hostages[4];
        };
};

void AddSC_instance_zulaman()
{
    new instance_zulaman();
}

