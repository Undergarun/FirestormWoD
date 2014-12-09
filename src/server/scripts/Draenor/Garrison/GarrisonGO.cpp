#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "Garrison.h"

/// Garrison cache generic script
class go_garrison_cache : public GameObjectScript
{
    public:
        /// Constructor
        go_garrison_cache() : 
            GameObjectScript("go_garrison_cache")
        { 
        
        }

        bool OnGossipHello(Player * p_Player, GameObject * p_GameObject)
        {
            if (p_Player->GetGarrison())
                p_Player->GetGarrison()->RewardGarrisonCache();

            return false;
        }

};

void AddSC_Garrison_GO()
{
    new go_garrison_cache;
}