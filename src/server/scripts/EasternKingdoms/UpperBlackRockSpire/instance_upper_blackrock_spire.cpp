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
                m_RunesToDisable            = 5;
                m_CommanderTharbekGuid      = 0;
                m_WarlordZaelaGuid          = 0;
            }

            uint64 m_PreOrebenderDoorGuid;
            uint64 m_OrebenderEntranceGuid;
            uint64 m_OrebenderGorashanGuid;
            uint8  m_ThunderingCacophonyCasted;
            uint8  m_RunesToDisable;

            uint64 m_CommanderTharbekGuid;
            uint64 m_SpawnDoorGuid;

            uint64 m_WarlordZaelaGuid;

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
                        if (p_State != DONE)
                            m_ThunderingCacophonyCasted = 0;
                        break;
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
                        if (!m_RunesToDisable)
                            break;

                        --m_RunesToDisable;

                        Unit* l_Orebender = sObjectAccessor->FindUnit(m_OrebenderGorashanGuid);
                        if (!m_RunesToDisable && l_Orebender != nullptr)
                        {
                            if (GameObject* l_PreDoor = GameObject::GetGameObject(*l_Orebender, m_PreOrebenderDoorGuid))
                                l_PreDoor->SetGoState(GO_STATE_ACTIVE);
                            if (GameObject* l_Entrance = GameObject::GetGameObject(*l_Orebender, m_OrebenderEntranceGuid))
                                l_Entrance->SetGoState(GO_STATE_ACTIVE);
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

