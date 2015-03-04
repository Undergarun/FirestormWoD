////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AshranMgr.hpp"

/// Commander Anne Dunworthy - 84173
class npc_ashran_commander_anne_dunworthy : public CreatureScript
{
    public:
        npc_ashran_commander_anne_dunworthy() : CreatureScript("npc_ashran_commander_anne_dunworthy") { }

        enum eQuest
        {
            WelcomeToAshran = 36119
        };

        bool OnGossipHello(Player* p_Player, Creature* p_Creature)
        {
            if (p_Player->GetQuestStatus(eQuest::WelcomeToAshran) == QuestStatus::QUEST_STATUS_INCOMPLETE)
                p_Player->CompleteQuest(eQuest::WelcomeToAshran);

            return false;
        }
};

/// General Ushet Wolfbarger - 84473
class npc_ashran_general_ushet_wolfbarger : public CreatureScript
{
    public:
        npc_ashran_general_ushet_wolfbarger() : CreatureScript("npc_ashran_general_ushet_wolfbarger") { }

        enum eDatas
        {
            WelcomeToAshran = 36196
        };

        bool OnGossipHello(Player* p_Player, Creature* p_Creature)
        {
            if (p_Player->GetQuestStatus(eDatas::WelcomeToAshran) == QuestStatus::QUEST_STATUS_INCOMPLETE)
                p_Player->CompleteQuest(eDatas::WelcomeToAshran);

            return false;
        }
};

void AddSC_AshranQuest()
{
    new npc_ashran_commander_anne_dunworthy();
    new npc_ashran_general_ushet_wolfbarger();
}