////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Instance_Mechanar
SD%Complete: 100
SDComment:
SDCategory: Mechanar
EndScriptData */

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "mechanar.h"

#define MAX_ENCOUNTER      1

class instance_mechanar : public InstanceMapScript
{
    public:
        instance_mechanar()
            : InstanceMapScript("instance_mechanar", 554)
        {
        }

        struct instance_mechanar_InstanceMapScript : public InstanceScript
        {
            instance_mechanar_InstanceMapScript(Map* map) : InstanceScript(map) {}

            uint32 m_auiEncounter[MAX_ENCOUNTER];
            uint64 pGobGuid;

            void Initialize()
            {
                memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
                pGobGuid = 0;
            }

            bool IsEncounterInProgress() const
            {
                for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    if (m_auiEncounter[i] == IN_PROGRESS)
                        return true;

                return false;
            }

            uint32 GetData(uint32 type)
            {
                switch (type)
                {
                case DATA_NETHERMANCER_EVENT:   return m_auiEncounter[0];
                }

                return false;
            }

            uint64 GetData64(uint32 /*identifier*/)
            {
                return pGobGuid;
            }

            void OnGameObjectCreate(GameObject* go)
            {
                if (go->GetEntry() == GOB_CACHE_LEGION)
                    pGobGuid = go->GetGUID();
            }

            void SetData(uint32 type, uint32 data)
            {
                switch (type)
                {
                case DATA_NETHERMANCER_EVENT:   m_auiEncounter[0] = data;   break;
                }
            }
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_mechanar_InstanceMapScript(map);
        }
};

#ifndef __clang_analyzer__
void AddSC_instance_mechanar()
{
    new instance_mechanar;
}
#endif
