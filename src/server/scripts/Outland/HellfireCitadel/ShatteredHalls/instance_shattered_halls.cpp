////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Instance_Shattered_Halls
SD%Complete: 50
SDComment: currently missing info about door. instance not complete
SDCategory: Hellfire Citadel, Shattered Halls
EndScriptData */

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "shattered_halls.h"

#define MAX_ENCOUNTER  2

#define DOOR_NETHEKURSE     1

class instance_shattered_halls : public InstanceMapScript
{
    public:
        instance_shattered_halls()
            : InstanceMapScript("instance_shattered_halls", 540)
        {
        }
        struct instance_shattered_halls_InstanceMapScript : public InstanceScript
        {
            instance_shattered_halls_InstanceMapScript(Map* map) : InstanceScript(map) {}

            uint32 m_auiEncounter[MAX_ENCOUNTER];
            uint64 nethekurseGUID;
            uint64 nethekurseDoorGUID;

            void Initialize()
            {
                memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

                nethekurseGUID = 0;
                nethekurseDoorGUID = 0;
            }

            void OnGameObjectCreate(GameObject* go)
            {
                switch (go->GetEntry())
                {
                    case DOOR_NETHEKURSE:
                        nethekurseDoorGUID = go->GetGUID();
                        break;
                }
            }

            void OnCreatureCreate(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case 16807:
                        nethekurseGUID = creature->GetGUID();
                        break;
                }
            }

            void SetData(uint32 type, uint32 data)
            {
                switch (type)
                {
                    case TYPE_NETHEKURSE:
                        m_auiEncounter[0] = data;
                        break;
                    case TYPE_OMROGG:
                        m_auiEncounter[1] = data;
                        break;
                }
            }

            uint32 GetData(uint32 type)
            {
                switch (type)
                {
                    case TYPE_NETHEKURSE:
                        return m_auiEncounter[0];
                    case TYPE_OMROGG:
                        return m_auiEncounter[1];
                }
                return 0;
            }

            uint64 GetData64(uint32 data)
            {
                switch (data)
                {
                    case DATA_NETHEKURSE:
                        return nethekurseGUID;
                    case DATA_NETHEKURSE_DOOR:
                        return nethekurseDoorGUID;
                }
                return 0;
            }
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_shattered_halls_InstanceMapScript(map);
        }
};

#ifndef __clang_analyzer__
void AddSC_instance_shattered_halls()
{
    new instance_shattered_halls();
}
#endif
