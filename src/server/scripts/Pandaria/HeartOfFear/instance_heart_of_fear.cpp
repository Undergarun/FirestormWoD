////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "VMapFactory.h"
#include "heart_of_fear.h"

DoorData const doorData[] =
{
    {GOB_ANTECHAMBER_DOOR_ENTRANCE, 0,              DOOR_TYPE_ROOM,     BOUNDARY_S},
    {GOB_ANTECHAMBER_DOOR_EXIT,     0,              DOOR_TYPE_ROOM,     BOUNDARY_E},
    {GOB_ORATIUM_DOOR_ENTRANCE,     DATA_ZORLOK,    DOOR_TYPE_ROOM,     BOUNDARY_W},
    {GOB_QUARTERS_DOOR_ENTRANCE,    DATA_ZORLOK,    DOOR_TYPE_PASSAGE,  BOUNDARY_S},
    {GOB_QUARTERS_DOOR_EXIT,        DATA_TAYAK,     DOOR_TYPE_PASSAGE,  BOUNDARY_W},
    {GOB_STAIRWAYS_DOOR_EXIT,       0,              DOOR_TYPE_ROOM,     BOUNDARY_N},
    {GOB_BALCONY_DOOR_EXIT,         DATA_MELJARAK,  DOOR_TYPE_PASSAGE,  BOUNDARY_S},
    {GOB_ATRIUM_DOOR_ENTRANCE,      0,              DOOR_TYPE_ROOM,     BOUNDARY_N},
    {GOB_ATRIUM_DOOR_EXIT,          0,              DOOR_TYPE_ROOM,     BOUNDARY_W},
    {GOB_SANCTUM_DOOR_ENTRANCE,     0,              DOOR_TYPE_ROOM,     BOUNDARY_E},
    {GOB_HEARTOFFEAR_DOOR_ENTRANCE, DATA_UNSOK,     DOOR_TYPE_PASSAGE,  BOUNDARY_E},
    {0,                             0,              DOOR_TYPE_ROOM,     0}, // EOF
};

class instance_heart_of_fear : public InstanceMapScript
{
    public:
        instance_heart_of_fear() : InstanceMapScript("instance_heart_of_fear", 1009) { }

        enum eMisc
        {
            HeartOfFearSecondPart = 530
        };

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

            // Special Doors GUIDs
            uint64 zorlokEntranceDoorGuid;
            uint64 tayakEntranceDoorGuid;
            uint64 tayakExitDoorGuid;
            uint64 garalonEntranceDoorGuid;
            uint64 meljarakExitDoorGuid;
            uint64 unsokEntranceDoorGuid;
            uint64 shekzeerEntranceDoorGuid;

            // Shek'zeer Gameobjects
            uint64 empressChamberGuid;
            uint64 mandidQueenCeilGuid;

            bool m_SecondPartInitialized;

            void Initialize() override
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

                m_SecondPartInitialized = false;
            }

            void OnCreatureCreate(Creature* creature) override
            {
                switch (creature->GetEntry())
                {
                    case NPC_ZORLOK:
                        zorlokGuid = creature->GetGUID();
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

            void OnGameObjectCreate(GameObject* go) override
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
                        go->SetGoState(GO_STATE_READY);
                        garalonEntranceDoorGuid = go->GetGUID();
                        break;
                    case GOB_BALCONY_DOOR_EXIT:
                        AddDoor(go, true);
                        go->SetGoState(GO_STATE_READY);
                        meljarakExitDoorGuid = go->GetGUID();
                        break;
                    case GOB_SANCTUM_DOOR_ENTRANCE:
                        AddDoor(go, true);
                        go->SetGoState(GO_STATE_READY);
                        unsokEntranceDoorGuid = go->GetGUID();
                        break;
                    case GOB_HEARTOFFEAR_DOOR_ENTRANCE:
                        AddDoor(go, true);
                        go->SetGoState(GO_STATE_READY);
                        shekzeerEntranceDoorGuid = go->GetGUID();
                        break;
                    case GOB_EMPRESS_CHAMBER:
                        empressChamberGuid = go->GetGUID();
                        break;
                    case GOB_MANTID_QUEEN_CEIL:
                        mandidQueenCeilGuid = go->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectRemove(GameObject* go) override
            {
                switch (go->GetEntry())
                {
                    // Generic doors
                    case GOB_ANTECHAMBER_DOOR_ENTRANCE:
                    case GOB_ANTECHAMBER_DOOR_EXIT:
                    case GOB_ATRIUM_DOOR_ENTRANCE:
                    case GOB_ATRIUM_DOOR_EXIT:
                    case GOB_ORATIUM_DOOR_ENTRANCE:
                    case GOB_QUARTERS_DOOR_ENTRANCE:
                    case GOB_QUARTERS_DOOR_EXIT:
                    case GOB_STAIRWAYS_DOOR_EXIT:
                    case GOB_BALCONY_DOOR_EXIT:
                    case GOB_SANCTUM_DOOR_ENTRANCE:
                    case GOB_HEARTOFFEAR_DOOR_ENTRANCE:
                        AddDoor(go, false);
                        break;
                    default:
                        break;
                }
            }

            bool SetBossState(uint32 id, EncounterState state) override
            {
                if (!InstanceScript::SetBossState(id, state))
                    return false;

                return true;
            }

            uint64 GetData64(uint32 type) override
            {
                switch (type)
                {
                    // --- Creatures ---
                    case NPC_ZORLOK:
                        return zorlokGuid;
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
                    case GOB_EMPRESS_CHAMBER:
                        return empressChamberGuid;
                    case GOB_MANTID_QUEEN_CEIL:
                        return mandidQueenCeilGuid;
                    default:
                        break;
                }

                return 0;
            }

            bool IsWipe() override
            {
                Map::PlayerList const& PlayerList = instance->GetPlayers();

                if (PlayerList.isEmpty())
                    return true;

                std::list<Player*> servantList;
                servantList.clear();

                for (Map::PlayerList::const_iterator Itr = PlayerList.begin(); Itr != PlayerList.end(); ++Itr)
                {
                    Player* player = Itr->getSource();

                    if (!player)
                        continue;

                    if (player->isAlive() && !player->isGameMaster() && !player->HasAura(SPELL_CONVERT_SERVANT))
                        return false;
                    else if (player->HasAura(SPELL_CONVERT_SERVANT))
                        servantList.push_back(player);
                }

                // Killing the servant players
                if (!servantList.empty())
                    if (Creature* shekzeer = instance->GetCreature(GetData64(NPC_SHEKZEER)))
                        for (Player* servant : servantList)
                            shekzeer->Kill(servant);

                return true;
            }

            bool CheckRequiredBosses(uint32 bossId, Player const* player = NULL) const override
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

            void OnPlayerEnter(Player* p_Player) override
            {
                p_Player->GetMap()->SetObjectVisibility(350.0f);

                if (!m_SecondPartInitialized && instance->IsLFR())
                {
                    uint32 l_DungeonID = p_Player->GetGroup() ? sLFGMgr->GetDungeon(p_Player->GetGroup()->GetGUID()) : 0;
                    if (l_DungeonID == eMisc::HeartOfFearSecondPart)
                    {
                        m_SecondPartInitialized = true;

                        if (Creature* l_Garalon = Creature::GetCreature(*p_Player, garalonGuid))
                        {
                            std::list<Unit*> l_TrashMobs;

                            JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(l_Garalon, l_Garalon, 50.0f);
                            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(l_Garalon, l_TrashMobs, l_Check);
                            l_Garalon->VisitNearbyObject(50.0f, l_Searcher);

                            for (Unit* l_Unit : l_TrashMobs)
                            {
                                if (l_Unit->ToCreature() == nullptr)
                                    continue;

                                /// Basic settings
                                l_Unit->SetVisible(false);
                                l_Unit->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                                l_Unit->ToCreature()->SetReactState(REACT_PASSIVE);
                            }

                            std::list<GameObject*> l_DoorList;
                            GetGameObjectListWithEntryInGrid(l_DoorList, l_Garalon, GOB_DOOR_TO_MELJARAK, 200.0f);

                            for (GameObject* l_Door : l_DoorList)
                                l_Door->SetGoState(GO_STATE_ACTIVE);
                        }
                    }
                }
            }

            void OnPlayerExit(Player* p_Player) override
            {
                if (p_Player->HasAura(SPELL_RESHAPE_LIFE))
                    p_Player->RemoveAura(SPELL_RESHAPE_LIFE);
            }
        };
};

#ifndef __clang_analyzer__
void AddSC_instance_heart_of_fear()
{
    new instance_heart_of_fear();
}
#endif