#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "Garrison.h"

class PlayerScript_Garrison : public PlayerScript
{
    public:
        PlayerScript_Garrison()
            : PlayerScript("PlayerScript_Garrison")
        {

        }

        /// Called when player rewards some quest
        virtual void OnQuestReward(Player* p_Player, const Quest* p_Quest) override
        {
            if (p_Player && p_Player->IsInGarrison() && p_Quest)
                p_Player->GetGarrison()->OnQuestReward(p_Quest);
        }

};

void AddSC_Garrison()
{
    new PlayerScript_Garrison();
}