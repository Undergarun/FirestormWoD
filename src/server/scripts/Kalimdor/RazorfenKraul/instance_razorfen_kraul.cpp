////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Instance_Razorfen_Kraul
SD%Complete:
SDComment:
SDCategory: Razorfen Kraul
EndScriptData */

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "razorfen_kraul.h"

#define WARD_KEEPERS_NR 2

class instance_razorfen_kraul : public InstanceMapScript
{
public:
    instance_razorfen_kraul() : InstanceMapScript("instance_razorfen_kraul", 47) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_razorfen_kraul_InstanceMapScript(map);
    }

    struct instance_razorfen_kraul_InstanceMapScript : public InstanceScript
    {
        instance_razorfen_kraul_InstanceMapScript(Map* map) : InstanceScript(map) {}

        uint64 DoorWardGUID;
        int WardKeeperDeath;

        void Initialize()
        {
            WardKeeperDeath = 0;
            DoorWardGUID = 0;
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
            sLog->outDebug(LOG_FILTER_TSCR, "Instance Razorfen Kraul: GetPlayerInMap, but PlayerList is empty!");
            return NULL;
        }

        void OnGameObjectCreate(GameObject* go)
        {
            switch (go->GetEntry())
            {
                case 21099: DoorWardGUID = go->GetGUID(); break;
            }
        }

        void Update(uint32 diff)
        {
            UpdateOperations(diff);

            if (WardKeeperDeath == WARD_KEEPERS_NR)
                if (GameObject* go = instance->GetGameObject(DoorWardGUID))
                {
                    go->SetUInt32Value(GAMEOBJECT_FIELD_FLAGS, 33);
                    go->SetGoState(GO_STATE_ACTIVE);
                }
        }

        void SetData(uint32 type, uint32 /*data*/)
        {
            switch (type)
            {
                case EVENT_WARD_KEEPER: WardKeeperDeath++; break;
            }
        }

    };

};

#ifndef __clang_analyzer__
void AddSC_instance_razorfen_kraul()
{
    new instance_razorfen_kraul();
}
#endif
