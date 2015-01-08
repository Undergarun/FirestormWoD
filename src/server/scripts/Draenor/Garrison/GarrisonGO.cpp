#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "Garrison.h"

enum
{
    SPELL_RELIEVED              = 162410,
    ACHIEVEMENT_STAYING_REGULAR = 8933,
    QUEST_KEEPING_IT_TOGETHER   = 35176,
    QUEST_WHAT_WE_GOT           = 34824,
};

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
            {
                p_Player->GetGarrison()->RewardGarrisonCache();

                if (p_Player->hasQuest(QUEST_KEEPING_IT_TOGETHER) && p_GameObject)
                    p_Player->QuestObjectiveSatisfy(41327, 1, QUEST_OBJECTIVE_TYPE_CRITERIA, p_GameObject->GetGUID());
                if (p_Player->hasQuest(QUEST_WHAT_WE_GOT) && p_GameObject)
                    p_Player->QuestObjectiveSatisfy(41325, 2, QUEST_OBJECTIVE_TYPE_CRITERIA, p_GameObject->GetGUID());
            }

            return false;
        }

};

/// Garrison cache generic script
class go_garrison_outhouse : public GameObjectScript
{
    public:
        /// Constructor
        go_garrison_outhouse() :
            GameObjectScript("go_garrison_outhouse")
        { 
        
        }

        bool OnGossipHello(Player * p_Player, GameObject * p_GameObject)
        {
            p_Player->CastSpell(p_Player, SPELL_RELIEVED);

            if (!p_Player->GetAchievementMgr().HasAchieved(ACHIEVEMENT_STAYING_REGULAR))
                p_Player->GetAchievementMgr().CompletedAchievement(sAchievementStore.LookupEntry(ACHIEVEMENT_STAYING_REGULAR), nullptr);

            return false;
        }

};

void AddSC_Garrison_GO()
{
    new go_garrison_cache;
    new go_garrison_outhouse;
}