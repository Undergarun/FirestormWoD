/*
* Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com>
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ScriptPCH.h"
#include "upper_blackrock_spire.h"
#include "ObjectAccessor.h"

static const DoorData doordata[] = 
{
    { GOB_OREBENDER_ENTRANCE,   DATA_OREBENDER_GORASHAN,    DOOR_TYPE_ROOM,     BOUNDARY_NONE },
    { GOB_OREBENDER_EXIT,       DATA_OREBENDER_GORASHAN,    DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
    { GOB_OREBENDER_EXIT,       DATA_KYRAK_THE_CORRUPTOR,   DOOR_TYPE_ROOM,     BOUNDARY_NONE },
    { GOB_KYRAK_EXIT_01,        DATA_KYRAK_THE_CORRUPTOR,   DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
    { GOB_KYRAK_EXIT_02,        DATA_KYRAK_THE_CORRUPTOR,   DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
    { GOB_THARBEK_EXIT,         DATA_COMMANDER_THARBEK,     DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
    { 0,                        0,                          DOOR_TYPE_ROOM,     0             }  // EOF
};

class instance_upper_blackrock_spire : public InstanceMapScript
{
    public:
        instance_upper_blackrock_spire() : InstanceMapScript("instance_upper_blackrock_spire", 1358) { }

        struct instance_upper_blackrock_spireInstanceMapScript : public InstanceScript
        {
            instance_upper_blackrock_spireInstanceMapScript(Map* p_Map) : InstanceScript(p_Map)
            {
                SetBossNumber(DATA_MAX_ENCOUNTERS);
                LoadDoorData(doordata);

                m_PreOrebenderDoorGuid      = 0;
                m_OrebenderEntranceGuid     = 0;
                m_OrebenderGorashanGuid     = 0;
                m_ThunderingCacophonyCasted = 0;
                m_RunesDisabled             = 0;
                m_CommanderTharbekGuid      = 0;
                m_WarlordZaelaGuid          = 0;

                m_BeginningTime             = 0;
                m_CanUpdate                 = false;

                m_PlayerScenarioState.clear();

                m_InstanceGuid              = MAKE_NEW_GUID(instance->GetId(), 0, HIGHGUID_INSTANCE_SAVE);
            }

            uint64 m_PreOrebenderDoorGuid;
            uint64 m_OrebenderEntranceGuid;
            uint64 m_OrebenderGorashanGuid;
            uint8  m_ThunderingCacophonyCasted;
            uint8  m_RunesDisabled;

            uint64 m_CommanderTharbekGuid;
            uint64 m_SpawnDoorGuid;

            uint64 m_WarlordZaelaGuid;

            uint32 m_BeginningTime;
            bool   m_CanUpdate;

            std::map<uint32, bool> m_PlayerScenarioState;

            ObjectGuid m_InstanceGuid;

            void OnCreatureCreate(Creature* p_Creature)
            {
                switch (p_Creature->GetEntry())
                {
                    case NPC_OREBENDER_GORASHAN:
                        m_OrebenderGorashanGuid = p_Creature->GetGUID();
                        break;
                    case NPC_COMMANDER_THARBEK:
                        m_CommanderTharbekGuid = p_Creature->GetGUID();
                        break;
                    case NPC_WARLORD_ZAELA:
                        m_WarlordZaelaGuid = p_Creature->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* p_Gameobject)
            {
                switch (p_Gameobject->GetEntry())
                {
                    case GOB_EMBERSEER_IN:
                        m_PreOrebenderDoorGuid = p_Gameobject->GetGUID();
                        break;
                    case GOB_OREBENDER_ENTRANCE:
                        m_OrebenderEntranceGuid = p_Gameobject->GetGUID();
                        AddDoor(p_Gameobject, true);
                        break;
                    case GOB_OREBENDER_EXIT:
                    case GOB_KYRAK_EXIT_01:
                    case GOB_KYRAK_EXIT_02:
                    case GOB_THARBEK_EXIT:
                        AddDoor(p_Gameobject, true);
                        break;
                    case GOB_THARBEK_SPAWN_DOOR:
                        m_SpawnDoorGuid = p_Gameobject->GetGUID();
                        break;
                    default:
                        break;
                }
            }
            
            bool SetBossState(uint32 p_ID, EncounterState p_State)
            {
                if (!InstanceScript::SetBossState(p_ID, p_State))
                    return false;

                switch (p_ID)
                {
                    case DATA_OREBENDER_GORASHAN:
                    {
                        switch (p_State)
                        {
                            case DONE:
                                SendScenarioProgressUpdate(CriteriaProgressData(SCENARIO_UBRS_BOSS_1, 1, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                                break;
                            default:
                                m_ThunderingCacophonyCasted = 0;
                                break;
                        }

                        break;
                    }
                    case DATA_KYRAK_THE_CORRUPTOR:
                        SendScenarioProgressUpdate(CriteriaProgressData(SCENARIO_UBRS_BOSS_2, 1, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                        break;
                    case DATA_COMMANDER_THARBEK:
                    {
                        SendScenarioProgressUpdate(CriteriaProgressData(SCENARIO_UBRS_BOSS_3, 1, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                        ScenarioData l_Datas(SCENARIO_UBRS_ID, 2, 0, 0, 0, 15, 0, false);
                        l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_1, 9, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                        l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_2, 5, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                        l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_3, 8, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                        l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_4, 3, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                        l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_5, 5, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                        l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_6, 0, m_InstanceGuid, time(NULL), time(NULL), m_BeginningTime, 8));
                        l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_7, 5, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                        l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_8, 2, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                        l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_9, 3, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                        l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_10, 2, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                        l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_11, 2, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                        l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_12, 11, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                        l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_13, 4, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                        l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_14, 5, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                        l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_15, 4, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                        SendScenarioState(l_Datas);
                        break;
                    }
                    default:
                        break;
                }

                return true;
            }

            void SetData(uint32 p_Type, uint32 p_Data)
            {
                switch (p_Type)
                {
                    case DATA_MAGNETS_ACHIEVEMENT:
                        ++m_ThunderingCacophonyCasted;
                        break;
                    case DATA_RUNES_DISABLED:
                    {
                        if (m_RunesDisabled >= 5)
                            break;

                        ++m_RunesDisabled;

                        SendScenarioProgressUpdate(CriteriaProgressData(SCENARIO_UBRS_RUNES, m_RunesDisabled, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));

                        Unit* l_Orebender = sObjectAccessor->FindUnit(m_OrebenderGorashanGuid);
                        if (m_RunesDisabled >= 5 && l_Orebender != nullptr)
                        {
                            if (GameObject* l_PreDoor = GameObject::GetGameObject(*l_Orebender, m_PreOrebenderDoorGuid))
                                l_PreDoor->SetGoState(GO_STATE_ACTIVE);
                            if (GameObject* l_Entrance = GameObject::GetGameObject(*l_Orebender, m_OrebenderEntranceGuid))
                                l_Entrance->SetGoState(GO_STATE_ACTIVE);

                            m_BeginningTime = 0;

                            ScenarioData l_Datas(SCENARIO_UBRS_ID, 1, 0, 0, 0, 7, 0, false);
                            l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_1, 7, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                            l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_2, 5, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                            l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_3, 6, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                            l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_4, 3, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                            l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_5, 3, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                            l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_6, 0, m_InstanceGuid, time(NULL), time(NULL), m_BeginningTime, 8));
                            l_Datas.AddCriteriaProgress(CriteriaProgressData(SCENARIO_UBRS_TRANS_7, 1, m_InstanceGuid, time(NULL), m_BeginningTime, m_BeginningTime, 0));
                            SendScenarioState(l_Datas);
                            break;
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 p_Type)
            {
                switch (p_Type)
                {
                    case DATA_MAGNETS_ACHIEVEMENT:
                        return m_ThunderingCacophonyCasted;
                    default:
                        break;
                }

                return 0;
            }

            uint64 GetData64(uint32 p_Type)
            {
                switch (p_Type)
                {
                    case NPC_OREBENDER_GORASHAN:
                        return m_OrebenderGorashanGuid;
                    case NPC_COMMANDER_THARBEK:
                        return m_CommanderTharbekGuid;
                    case NPC_WARLORD_ZAELA:
                        return m_WarlordZaelaGuid;
                    case GOB_THARBEK_SPAWN_DOOR:
                        return m_SpawnDoorGuid;
                    default:
                        return 0;
                }
            }

            void OnPlayerEnter(Player* p_Player)
            {
                if (!p_Player->IsInWorld())
                    return;

                if (m_PlayerScenarioState.find(p_Player->GetGUIDLow()) != m_PlayerScenarioState.end())
                    return;

                m_PlayerScenarioState.insert(std::make_pair(p_Player->GetGUIDLow(), true));
                SendScenarioState(ScenarioData(SCENARIO_UBRS_ID, 0), p_Player);
                m_CanUpdate = true;
            }

            void Update(uint32 p_Diff)
            {
                if (!m_CanUpdate)
                    return;

                m_BeginningTime += p_Diff;
            }
        };

        InstanceScript* GetInstanceScript(InstanceMap* p_Map) const
        {
            return new instance_upper_blackrock_spireInstanceMapScript(p_Map);
        }
};

void AddSC_instance_upper_blackrock_spire()
{
    new instance_upper_blackrock_spire();
}

