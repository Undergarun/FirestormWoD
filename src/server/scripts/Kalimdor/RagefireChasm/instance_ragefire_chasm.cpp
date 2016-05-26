#include "ScriptMgr.h"
#include "ragefire_chasm.h"
#include "InstanceScript.h"

#define MAX_ENCOUNTER 4

class instance_ragefire_chasm : public InstanceMapScript
{
public:
    instance_ragefire_chasm() : InstanceMapScript("instance_ragefire_chasm", 389) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_ragefire_chasm_InstanceMapScript(map);
    }

    struct instance_ragefire_chasm_InstanceMapScript : public InstanceScript
    {
        instance_ragefire_chasm_InstanceMapScript(Map* map) : InstanceScript(map) {}

        void Initialize()
        {
            SetBossNumber(MAX_ENCOUNTER);

            teamInInstance = 0;
        }

        void BeforePlayerEnter(Player* player)
        {
            if (!teamInInstance)
                teamInInstance = player->GetTeam();
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch (creature->GetEntry())
            {
                case NPC_COMMANDER_BAGRAN:
                case NPC_INVOKER_XORENTH:
                case NPC_KORKRON_SCOUT:
                case NPC_KORKRON_ELITE:
                    if (teamInInstance == ALLIANCE)
                    {
                        // hide creature
                        creature->SetPhaseMask(2, true);
                    }
                    break;
            }

        }

        void OnGameObjectCreate(GameObject* go)
        {
            
        }

        uint64 GetData64(uint32 data)
        {
            return 0;
        }

    private:
        uint32 teamInInstance;

    };
};

void AddSC_instance_ragefire_chasm()
{
    new instance_ragefire_chasm();
}
