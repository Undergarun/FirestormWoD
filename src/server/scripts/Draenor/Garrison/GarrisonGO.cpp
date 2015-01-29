////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "GarrisonGO.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    /// Constructor
    go_garrison_cache::go_garrison_cache()
        : GameObjectScript("go_garrison_cache")
    { 

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_GameObject : Target GameObject instance
    bool go_garrison_cache::OnGossipHello(Player * p_Player, GameObject * p_GameObject)
    {
        if (p_Player->GetGarrison())
        {
            p_Player->GetGarrison()->RewardGarrisonCache();

            /// Alliance
            if (p_Player->HasQuest(Quests::QUEST_KEEPING_IT_TOGETHER) && p_GameObject)
                p_Player->QuestObjectiveSatisfy(41327, 1, QUEST_OBJECTIVE_TYPE_CRITERIA, p_GameObject->GetGUID());

            /// Horde
            if (p_Player->HasQuest(Quests::QUEST_WHAT_WE_GOT) && p_GameObject)
            {
                p_Player->QuestObjectiveSatisfy(41325, 2, QUEST_OBJECTIVE_TYPE_CRITERIA, p_GameObject->GetGUID());

                Creature * l_Creature = p_Player->FindNearestCreature(NPCs::NPC_LADY_SENA, 15);

                if (l_Creature)
                    l_Creature->AI()->Talk(0);
            }
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    go_garrison_outhouse::go_garrison_outhouse()
        : GameObjectScript("go_garrison_outhouse")
    { 

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_GameObject : Target GameObject instance
    bool go_garrison_outhouse::OnGossipHello(Player * p_Player, GameObject * p_GameObject)
    {
        p_Player->CastSpell(p_Player, MS::Garrison::Spells::SPELL_RELIEVED);

        if (!p_Player->GetAchievementMgr().HasAchieved(Achievements::ACHIEVEMENT_STAYING_REGULAR))
            p_Player->GetAchievementMgr().CompletedAchievement(sAchievementStore.LookupEntry(Achievements::ACHIEVEMENT_STAYING_REGULAR), nullptr);

        return false;
    }

}   ///< namespace Garrison
}   ///< namespace MS

void AddSC_Garrison_GO()
{
    new MS::Garrison::go_garrison_cache;
    new MS::Garrison::go_garrison_outhouse;
}