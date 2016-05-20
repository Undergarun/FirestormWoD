////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Instance - Sethekk Halls
SD%Complete: 50
SDComment: Instance Data for Sethekk Halls instance
SDCategory: Auchindoun, Sethekk Halls
EndScriptData */

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "sethekk_halls.h"

enum eEnums
{
    NPC_ANZU   = 23035,
    IKISS_DOOR = 177203
};

class instance_sethekk_halls : public InstanceMapScript
{
public:
    instance_sethekk_halls() : InstanceMapScript("instance_sethekk_halls", 556) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_sethekk_halls_InstanceMapScript(map);
    }

    struct instance_sethekk_halls_InstanceMapScript : public InstanceScript
    {
        instance_sethekk_halls_InstanceMapScript(Map* map) : InstanceScript(map) {}

        uint32 AnzuEncounter;
        uint64 m_uiIkissDoorGUID;

        void Initialize()
        {
            AnzuEncounter = NOT_STARTED;
            m_uiIkissDoorGUID = 0;
        }

        void OnCreatureCreate(Creature* creature)
        {
            if (creature->GetEntry() == NPC_ANZU)
            {
                if (AnzuEncounter >= IN_PROGRESS)
                    creature->DisappearAndDie();
                else
                    AnzuEncounter = IN_PROGRESS;
            }
        }

        void OnGameObjectCreate(GameObject* go)
        {
             if (go->GetEntry() == IKISS_DOOR)
                m_uiIkissDoorGUID = go->GetGUID();
        }

        void SetData(uint32 type, uint32 data)
        {
            switch (type)
            {
                case DATA_IKISSDOOREVENT:
                    if (data == DONE)
                        DoUseDoorOrButton(m_uiIkissDoorGUID, DAY*IN_MILLISECONDS);
                    break;
                case TYPE_ANZU_ENCOUNTER:
                    AnzuEncounter = data;
                    break;
            }
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_instance_sethekk_halls()
{
    new instance_sethekk_halls();
}
#endif
