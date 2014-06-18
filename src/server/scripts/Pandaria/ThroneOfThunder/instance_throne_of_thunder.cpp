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
#include "throne_of_thunder.h"

Position statuesPos[4] =
{
    { 5946.0f, 6317.5f, 158.083f, 3.923960f }, // NW
    { 5840.0f, 6316.0f, 158.083f, 5.486100f }, // SW
    { 5943.5f, 6211.0f, 158.083f, 2.355830f }, // NE
    { 5839.0f, 6211.5f, 158.083f, 0.781267f }  // SE
};

class instance_throne_of_thunder : public InstanceMapScript
{
    public:
        instance_throne_of_thunder() : InstanceMapScript("instance_throne_of_thunder", 1098) { }

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_throne_of_thunder_InstanceMapScript(map);
        }

        struct instance_throne_of_thunder_InstanceMapScript : public InstanceScript
        {
            instance_throne_of_thunder_InstanceMapScript(Map* map) : InstanceScript(map) { }

            uint64 jinRokhTheBreakerGuid;
            uint64 moguFountainsGuids[4];
            uint64 moguStatuesGuids[4];

            void Initialize()
            {
                SetBossNumber(DATA_MAX_BOSS_DATA);

                jinRokhTheBreakerGuid   = 0;

                for (uint8 i = 0; i < 4; ++i)
                {
                    moguFountainsGuids[i] = 0;
                    moguStatuesGuids[i] = 0;
                }
            }

            void OnCreatureCreate(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case NPC_JIN_ROKH_THE_BREAKER:
                        jinRokhTheBreakerGuid = creature->GetGUID();
                        break;
                    case NPC_STATUE:
                    {
                        Position pos;
                        creature->GetPosition(&pos);

                        if (pos.GetPositionX() == statuesPos[0].GetPositionX() &&
                            pos.GetPositionY() == statuesPos[0].GetPositionY())
                            moguStatuesGuids[0] = creature->GetGUID();
                        else if (pos.GetPositionX() == statuesPos[1].GetPositionX() &&
                                    pos.GetPositionY() == statuesPos[1].GetPositionY())
                            moguStatuesGuids[1] = creature->GetGUID();
                        else if (pos.GetPositionX() == statuesPos[2].GetPositionX() &&
                                    pos.GetPositionY() == statuesPos[2].GetPositionY())
                            moguStatuesGuids[2] = creature->GetGUID();
                        else if (pos.GetPositionX() == statuesPos[3].GetPositionX() &&
                                    pos.GetPositionY() == statuesPos[3].GetPositionY())
                            moguStatuesGuids[3] = creature->GetGUID();
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
                    case GOB_MOGU_FOUNTAIN_NE:
                        moguFountainsGuids[0] = go->GetGUID();
                        go->SetGoState(GO_STATE_READY);
                        break;
                    case GOB_MOGU_FOUNTAIN_NW:
                        moguFountainsGuids[1] = go->GetGUID();
                        go->SetGoState(GO_STATE_READY);
                        break;
                    case GOB_MOGU_FOUNTAIN_SE:
                        moguFountainsGuids[2] = go->GetGUID();
                        go->SetGoState(GO_STATE_READY);
                        break;
                    case GOB_MOGU_FOUNTAIN_SW:
                        moguFountainsGuids[3] = go->GetGUID();
                        go->SetGoState(GO_STATE_READY);
                        break;
                    default:
                        break;
                }
            }

            bool SetBossState(uint32 id, EncounterState state)
            {
                return true;
            }

            void SetData(uint32 type, uint32 data)
            {
                switch (type)
                {
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 type)
            {
                switch (type)
                {
                    default:
                        return 0;
                }
            }

            uint64 GetData64(uint32 type)
            {
                switch (type)
                {
                    case NPC_JIN_ROKH_THE_BREAKER:
                        return jinRokhTheBreakerGuid;
                    case GOB_MOGU_FOUNTAIN_NE:
                        return moguFountainsGuids[0];
                    case GOB_MOGU_FOUNTAIN_NW:
                        return moguFountainsGuids[1];
                    case GOB_MOGU_FOUNTAIN_SE:
                        return moguFountainsGuids[2];
                    case GOB_MOGU_FOUNTAIN_SW:
                        return moguFountainsGuids[3];
                    case DATA_STATUE_0:
                        return moguStatuesGuids[0];
                        break;
                    case DATA_STATUE_1:
                        return moguStatuesGuids[1];
                        break;
                    case DATA_STATUE_2:
                        return moguStatuesGuids[2];
                        break;
                    case DATA_STATUE_3:
                        return moguStatuesGuids[3];
                        break;
                    default:
                        break;
                }

                return 0;
            }

            bool CheckRequiredBosses(uint32 bossId, Player const* player = NULL) const
            {
                if (!InstanceScript::CheckRequiredBosses(bossId, player))
                    return false;

                switch (bossId)
                {
                    case DATA_HORRIDON:
                    case DATA_CONCIL_OF_ELDERS:
                    case DATA_TORTOS:
                    case DATA_MEGAERA:
                    case DATA_JI_KUN:
                    case DATA_DURUMU_THE_FORGOTTEN:
                    case DATA_PRIMORDIUS:
                    case DATA_DARK_ANIMUS:
                    case DATA_IRON_QON:
                    case DATA_TWIN_CONSORTS:
                    case DATA_LEI_SHEN:
                    case DATA_RA_DEN:
                        if (GetBossState(bossId - 1) != DONE)
                            return false;
                    default:
                        break;
                }

                return true;
            }
        };
};

void AddSC_instance_throne_of_thunder()
{
    new instance_throne_of_thunder();
}
