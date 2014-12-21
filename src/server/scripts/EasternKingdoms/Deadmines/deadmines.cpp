#include "ScriptPCH.h"
#include "deadmines.h"

enum Adds
{
    // quest
    NPC_EDWIN_CANCLEEF_1    = 42697, 
    NPC_ALLIANCE_ROGUE      = 42700,
    NPC_VANESSA_VANCLEEF_1  = 42371, // little
};

class go_defias_cannon : public GameObjectScript
{
    public:
        go_defias_cannon() : GameObjectScript("go_defias_cannon") { }

        bool OnGossipHello(Player* pPlayer, GameObject* pGo)
        {
            InstanceScript* pInstance = pGo->GetInstanceScript();
            if (!pInstance)
                return false;
            //if (pInstance->GetData(DATA_CANNON_EVENT) != CANNON_NOT_USED)
                //return false ;

            pInstance->SetData(DATA_CANNON_EVENT, CANNON_BLAST_INITIATED);
            return false;
        }
};

void AddSC_deadmines()
{
    new go_defias_cannon();
}