////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "GarrisonNPC.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

#include <random>

namespace MS { namespace Garrison 
{
    /// Constructor
    npc_GarrisonFord::npc_GarrisonFord()
        : CreatureScript("npc_GarrisonFord")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the creature.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    bool npc_GarrisonFord::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        if (!p_Player->GetGarrison())
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Create me a garrison.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        else
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Delete my garrison.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }
    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_GarrisonFord::OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        if (!p_Player->GetGarrison())
        {
            p_Player->CLOSE_GOSSIP_MENU();
            p_Player->CreateGarrison();

            uint32 l_MovieID    = p_Player->GetGarrison()->GetGarrisonSiteLevelEntry()->CreationMovie;
            uint32 l_MapID      = p_Player->GetGarrison()->GetGarrisonSiteLevelEntry()->MapID;
            uint32 l_TeamID     = p_Player->GetTeamId();

            p_Player->AddMovieDelayedTeleport(l_MovieID, l_MapID,   MS::Garrison::gGarrisonCreationCoords[l_TeamID][0],
                                                                    MS::Garrison::gGarrisonCreationCoords[l_TeamID][1],
                                                                    MS::Garrison::gGarrisonCreationCoords[l_TeamID][2],
                                                                    MS::Garrison::gGarrisonCreationCoords[l_TeamID][3]);
            p_Player->SendMovieStart(l_MovieID);

            if (l_TeamID == TEAM_ALLIANCE && p_Player->GetQuestStatus(Quests::QUEST_ETABLISH_YOUR_GARRISON_A) != QUEST_STATUS_REWARDED)
            {
                p_Player->AddQuest(sObjectMgr->GetQuestTemplate(Quests::QUEST_ETABLISH_YOUR_GARRISON_A), p_Creature);
                p_Player->CompleteQuest(MS::Garrison::Quests::QUEST_ETABLISH_YOUR_GARRISON_A);
            }
            else if (l_TeamID == TEAM_HORDE && p_Player->GetQuestStatus(Quests::QUEST_ETABLISH_YOUR_GARRISON_H) != QUEST_STATUS_REWARDED)
            {
                p_Player->AddQuest(sObjectMgr->GetQuestTemplate(Quests::QUEST_ETABLISH_YOUR_GARRISON_H), p_Creature);
                p_Player->CompleteQuest(Quests::QUEST_ETABLISH_YOUR_GARRISON_H);
            }

            /// HACK until shadowmoon quest are done : add follower Qiana Moonshadow / Olin Umberhide
            p_Player->GetGarrison()->AddFollower(34);
            p_Player->GetGarrison()->AddFollower(89);
            p_Player->GetGarrison()->AddFollower(92);

            /// HACK until quest : add barracks plan
            p_Player->GetGarrison()->LearnBlueprint(26);
        }
        else
        {
            if (p_Player->GetCurrency(GARRISON_CURRENCY_ID, false))
                p_Player->ModifyCurrency(GARRISON_CURRENCY_ID, -(int32)p_Player->GetCurrency(GARRISON_CURRENCY_ID, false));

            p_Player->DeleteGarrison();
        }

        return true;
    }

}   ///< namespace Garrison
}   ///< namespace MS

void AddSC_Garrison_NPC()
{
    new MS::Garrison::npc_GarrisonFord;
    new MS::Garrison::npc_GarrisonCartRope;
    new MS::Garrison::npc_AssistantBrightstone;
    new MS::Garrison::npc_ShellyHamby;
    new MS::Garrison::npc_BarosAlexsom;
    new MS::Garrison::npc_VindicatorMaraad;
    new MS::Garrison::npc_LunarfallLaborer;
    new MS::Garrison::npc_FrostwallPeon("npc_FrostwallPeon_Dynamic");
    new MS::Garrison::npc_FrostwallPeon("npc_FrostwallPeon");
    new MS::Garrison::npc_Skaggit;
}