/*
 * Copyright (C) 2008-20XX JadeCore <http://www.pandashan.com>
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

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "VMapFactory.h"
#include "heart_of_fear.h"

DoorData const doorData[] =
{
    {GOB_ANTECHAMBER_DOOR_ENTRANCE, 0,              DOOR_TYPE_ROOM,     BOUNDARY_S},
    {GOB_ANTECHAMBER_DOOR_EXIT,     0,              DOOR_TYPE_ROOM,     BOUNDARY_E},
    {GOB_ORATIUM_DOOR_ENTRANCE,     0,              DOOR_TYPE_ROOM,     BOUNDARY_W},
    {GOB_QUARTERS_DOOR_ENTRANCE,    DATA_ZORLOK,    DOOR_TYPE_PASSAGE,  BOUNDARY_S},
    {GOB_QUARTERS_DOOR_EXIT,        DATA_TAYAK,     DOOR_TYPE_PASSAGE,  BOUNDARY_W},
    {GOB_STAIRWAYS_DOOR_EXIT,       0,              DOOR_TYPE_ROOM,     BOUNDARY_N},
    {GOB_BALCONY_DOOR_EXIT,         DATA_MELJARAK,  DOOR_TYPE_PASSAGE,  BOUNDARY_S},
    {GOB_ATRIUM_DOOR_ENTRANCE,      0,              DOOR_TYPE_ROOM,     BOUNDARY_N},
    {GOB_ATRIUM_DOOR_EXIT,          0,              DOOR_TYPE_ROOM,     BOUNDARY_W},
    {GOB_SANCTUM_DOOR_ENTRANCE,     0,              DOOR_TYPE_ROOM,     BOUNDARY_E},
    {GOB_HEARTOFFEAR_DOOR_ENTRANCE, DATA_UNSOK,     DOOR_TYPE_PASSAGE,  BOUNDARY_E},
    {0,         0,              DOOR_TYPE_ROOM,     0}, // EOF
};

class instance_heart_of_fear : public InstanceMapScript
{
    public:
        instance_heart_of_fear() : InstanceMapScript("instance_heart_of_fear", 1009) { }

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_heart_of_fear_InstanceMapScript(map);
        }

        struct instance_heart_of_fear_InstanceMapScript : public InstanceScript
        {
            instance_heart_of_fear_InstanceMapScript(Map* map) : InstanceScript(map) {}

            // Boss GUIDs
            uint64 zorlokGuid;
            uint64 tayakGuid;
            uint64 garalonGuid;
            uint64 meljarakGuid;
            uint64 unsokGuid;
            uint64 shekzeerGuid;

            // Add GUIDs
            uint64 stormSpiritGuid;

            // Special Doors GUIDs
            uint64 zorlokEntranceDoorGuid;
            uint64 tayakEntranceDoorGuid;
            uint64 tayakExitDoorGuid;
            uint64 garalonEntranceDoorGuid;
            uint64 meljarakExitDoorGuid;
            uint64 unsokEntranceDoorGuid;
            uint64 shekzeerEntranceDoorGuid;

            void Initialize()
            {
                SetBossNumber(DATA_MAX_BOSS_DATA);
                LoadDoorData(doorData);

                zorlokGuid      = 0;
                tayakGuid       = 0;
                garalonGuid     = 0;
                meljarakGuid    = 0;
                unsokGuid       = 0;
                shekzeerGuid    = 0;

                zorlokEntranceDoorGuid      = 0;
                tayakEntranceDoorGuid       = 0;
                tayakExitDoorGuid           = 0;
                garalonEntranceDoorGuid     = 0;
                meljarakExitDoorGuid        = 0;
                unsokEntranceDoorGuid       = 0;
                shekzeerEntranceDoorGuid    = 0;
            }

            void OnCreatureCreate(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case NPC_ZORLOK:
                        zorlokGuid = creature->GetGUID();
                        break;
                    case NPC_STORM_SPIRIT:
                        stormSpiritGuid = creature->GetGUID();
                        break;
                    case NPC_TAYAK:
                        tayakGuid = creature->GetGUID();
                        break;
                    case NPC_GARALON:
                        garalonGuid = creature->GetGUID();
                        break;
                    case NPC_MELJARAK:
                        meljarakGuid = creature->GetGUID();
                        break;
                    case NPC_UNSOK:
                        unsokGuid = creature->GetGUID();
                        break;
                    case NPC_SHEKZEER:
                        shekzeerGuid = creature->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go)
            {
                switch (go->GetEntry())
                {
                    // Generic doors
                    case GOB_ANTECHAMBER_DOOR_ENTRANCE:
                    case GOB_ANTECHAMBER_DOOR_EXIT:
                    case GOB_ATRIUM_DOOR_ENTRANCE:
                    case GOB_ATRIUM_DOOR_EXIT:
                        AddDoor(go, true);
                        break;
                    // Specific doors
                    case GOB_ORATIUM_DOOR_ENTRANCE:
                        AddDoor(go, true);
                        zorlokEntranceDoorGuid = go->GetGUID();
                        break;
                    case GOB_QUARTERS_DOOR_ENTRANCE:
                        AddDoor(go, true);
                        tayakEntranceDoorGuid = go->GetGUID();
                        break;
                    case GOB_QUARTERS_DOOR_EXIT:
                        AddDoor(go, true);
                        tayakExitDoorGuid = go->GetGUID();
                        break;
                    case GOB_STAIRWAYS_DOOR_EXIT:
                        AddDoor(go, true);
                        garalonEntranceDoorGuid = go->GetGUID();
                        break;
                    case GOB_BALCONY_DOOR_EXIT:
                        AddDoor(go, true);
                        meljarakExitDoorGuid = go->GetGUID();
                        break;
                    case GOB_SANCTUM_DOOR_ENTRANCE:
                        AddDoor(go, true);
                        unsokEntranceDoorGuid = go->GetGUID();
                        break;
                    case GOB_HEARTOFFEAR_DOOR_ENTRANCE:
                        AddDoor(go, true);
                        shekzeerEntranceDoorGuid = go->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            bool SetBossState(uint32 id, EncounterState state)
            {
                if (!InstanceScript::SetBossState(id, state))
                    return false;
                /*
                switch (id)
                {

                    default:
                        break;
                }
                */
                return true;
            }

            uint64 GetData64(uint32 type)
            {
                switch (type)
                {
                    // --- Creatures ---
                    case NPC_ZORLOK:
                        return zorlokGuid;
                    case NPC_STORM_SPIRIT:
                        return stormSpiritGuid;
                    case NPC_TAYAK:
                        return tayakGuid;
                    case NPC_GARALON:
                        return garalonGuid;
                    case NPC_MELJARAK:
                        return meljarakGuid;
                    case NPC_UNSOK:
                        return unsokGuid;
                    case NPC_SHEKZEER:
                        return shekzeerGuid;
                    // --- GameObjects ---
                    case GOB_ORATIUM_DOOR_ENTRANCE:
                        return zorlokEntranceDoorGuid;
                    case GOB_QUARTERS_DOOR_ENTRANCE:
                        return tayakEntranceDoorGuid;
                    case GOB_QUARTERS_DOOR_EXIT:
                        return tayakExitDoorGuid;
                    case GOB_STAIRWAYS_DOOR_EXIT:
                        return garalonEntranceDoorGuid;
                    case GOB_BALCONY_DOOR_EXIT:
                        return meljarakExitDoorGuid;
                    case GOB_SANCTUM_DOOR_ENTRANCE:
                        return unsokEntranceDoorGuid;
                    case GOB_HEARTOFFEAR_DOOR_ENTRANCE:
                        return shekzeerEntranceDoorGuid;
                    default:
                        break;
                }

                return 0;
            }

            bool IsWipe()
            {
                Map::PlayerList const& PlayerList = instance->GetPlayers();

                if (PlayerList.isEmpty())
                    return true;

                return false;
            }

            bool CheckRequiredBosses(uint32 bossId, Player const* player = NULL) const
            {
                if (!InstanceScript::CheckRequiredBosses(bossId, player))
                    return false;

                switch (bossId)
                {
                    case DATA_TAYAK:
                    case DATA_GARALON:
                    case DATA_MELJARAK:
                    case DATA_UNSOK:
                    case DATA_SHEKZEER:
                        if (GetBossState(bossId - 1) != DONE)
                            return false;
                    default:
                        break;
                }

                return true;
            }
        };
};

void AddSC_instance_heart_of_fear()
{
    new instance_heart_of_fear();
}
