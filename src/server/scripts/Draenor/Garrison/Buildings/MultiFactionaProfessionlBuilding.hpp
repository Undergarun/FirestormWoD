////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_MULTIFACTIONAL_PROFESSION_BUILDING_HPP_GARRISON
#define GARRISON_MULTIFACTIONAL_PROFESSION_BUILDING_HPP_GARRISON

#include "../GarrisonScriptData.hpp"
#include "../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"
#include "SharedDefines.h"

#define GET_QUEST_ID(p) p->GetTeamId() == TeamId::TEAM_ALLIANCE ? t_QuestIDA : t_QuestIDH

namespace MS { namespace Garrison 
{
    /// Simple profession building NPC script
    /// @t_ScriptName   : Script name
    /// @t_Skill        : Building target skill type
    /// @t_QuestIDA     : Building work order quest -- Alliance
    /// @t_QuestIDH     : Building work order quest -- Horde
    /// @t_SetupLevel1  : Function pour initializing sequence for level 1 building
    /// @t_SetupLevel2  : Function pour initializing sequence for level 2 building
    /// @t_SetupLevel3  : Function pour initializing sequence for level 3 building
    template<char const* t_ScriptName, SkillType t_Skill, uint32 t_QuestIDA, uint32 t_QuestIDH, InitSequenceFunction* t_SetupLevel1, InitSequenceFunction* t_SetupLevel2, InitSequenceFunction* t_SetupLevel3> 
    class MultiFactionalBuilding_WorkOrderNPC : public SimpleSequenceCosmeticScript<t_ScriptName, t_SetupLevel1, t_SetupLevel2, t_SetupLevel3>
    {
        public:
            /// Constructor
            MultiFactionalBuilding_WorkOrderNPC()
                : SimpleSequenceCosmeticScript<t_ScriptName, t_SetupLevel1, t_SetupLevel2, t_SetupLevel3>()
            {
            }

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target GameObject instance
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
            {
                uint32 l_QuestID = GET_QUEST_ID(p_Player);

                if (!l_QuestID && p_Creature->AI() && p_Creature->GetScriptName() == CreatureScript::GetName())
                {
                    reinterpret_cast<GarrisonNPCAI*>(p_Creature->AI())->SendShipmentCrafterUI(p_Player);
                    return true;
                }

                if (p_Player->HasQuest(l_QuestID) && !p_Player->IsQuestRewarded(l_QuestID))
                    p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(l_QuestID, 4);

                if (p_Player->HasQuest(l_QuestID) || p_Player->IsQuestRewarded(l_QuestID))
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to place an order.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

                p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

                return true;
            }

            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action) override
            {
                if (p_Creature->AI() && p_Creature->GetScriptName() == CreatureScript::GetName())
                    reinterpret_cast<GarrisonNPCAI*>(p_Creature->AI())->SendShipmentCrafterUI(p_Player);

                return true;
            }

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Simple profession building NPC script
    /// @t_ScriptName   : Script name
    /// @t_Skill        : Building target skill type
    /// @t_QuestID      : Building work order quest
    /// @t_SetupLevel1  : Function pour initializing sequence for level 1 building
    /// @t_SetupLevel2  : Function pour initializing sequence for level 2 building
    /// @t_SetupLevel3  : Function pour initializing sequence for level 3 building
    template<char const* t_ScriptName, SkillType t_Skill, uint32 t_QuestIDA, uint32 t_QuestIDH, uint32 t_CreatureEntry, InitSequenceFunction* t_SetupLevel1, InitSequenceFunction* t_SetupLevel2, InitSequenceFunction* t_SetupLevel3>
    class MultiFactionalBuilding_SkillNPC : public SimpleSequenceCosmeticScript<t_ScriptName, t_SetupLevel1, t_SetupLevel2, t_SetupLevel3>
    {
        public:
            /// Constructor
            MultiFactionalBuilding_SkillNPC()
                : SimpleSequenceCosmeticScript<t_ScriptName, t_SetupLevel1, t_SetupLevel2, t_SetupLevel3>()
            {
            }

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target GameObject instance
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
            {
                uint32 l_QuestID = GET_QUEST_ID(p_Player);

                if (!l_QuestID && p_Player && p_Creature && p_Creature->AI() && p_Creature->GetScriptName() == CreatureScript::GetName())
                {
                    reinterpret_cast<GarrisonNPCAI*>(p_Creature->AI())->SendShipmentCrafterUI(p_Player);
                    return true;
                }

                if (p_Player->IsQuestRewarded(l_QuestID) && p_Player->HasSkill(t_Skill) && t_Skill)
                    p_Player->GetSession()->SendListInventory(p_Creature->GetGUID());
                else
                {
                    if (!p_Player->HasQuest(l_QuestID) && !p_Player->IsQuestRewarded(l_QuestID))
                        p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(l_QuestID, 4);

                    if (!p_Player->HasSkill(t_Skill) && t_Skill)
                        p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I need you to do something for me.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

                    p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
                }

                return true;
            }

            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action) override
            {
                p_Player->CLOSE_GOSSIP_MENU();

                if (t_Skill && !p_Player->HasSkill(t_Skill))
                {
                    if (p_Creature->AI() && p_Creature->GetScriptName() == CreatureScript::GetName())
                    {
                        GarrisonNPCAI* l_AI = reinterpret_cast<GarrisonNPCAI*>(p_Creature->AI());
                        l_AI->SetRecipes(m_Recipes, t_Skill);
                        l_AI->SendTradeSkillUI(p_Player);
                    }
                }

                return true;
            }

        private:
            std::vector<RecipesConditions> m_Recipes;

    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_MULTIFACTIONAL_PROFESSION_BUILDING_HPP_GARRISON