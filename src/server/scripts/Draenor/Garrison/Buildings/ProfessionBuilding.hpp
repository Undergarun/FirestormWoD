////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_PROFESSION_BUILDING_HPP_GARRISON
#define GARRISON_PROFESSION_BUILDING_HPP_GARRISON

#include "../GarrisonScriptData.hpp"
#include "../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"
#include "SharedDefines.h"

namespace MS { namespace Garrison
{
    /// Simple profession building NPC script
    /// @t_ScriptName   : Script name
    /// @t_Skill        : Building target skill type
    /// @t_QuestID      : Building work order quest
    /// @t_SetupLevel1  : Function pour initializing sequence for level 1 building
    /// @t_SetupLevel2  : Function pour initializing sequence for level 2 building
    /// @t_SetupLevel3  : Function pour initializing sequence for level 3 building
    template<char const* t_ScriptName, SkillType t_Skill, uint32 t_QuestID, InitSequenceFunction* t_SetupLevel1, InitSequenceFunction* t_SetupLevel2, InitSequenceFunction* t_SetupLevel3>
    class ProfessionBuilding_WorkOrderNPC : public SimpleSequenceCosmeticScript<t_ScriptName, t_SetupLevel1, t_SetupLevel2, t_SetupLevel3>
    {
        public:
            /// Constructor
            ProfessionBuilding_WorkOrderNPC()
                : SimpleSequenceCosmeticScript<t_ScriptName, t_SetupLevel1, t_SetupLevel2, t_SetupLevel3>()
            {

            }

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target GameObject instance
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
            {
                if (p_Player->HasQuest(t_QuestID) && !p_Player->IsQuestRewarded(t_QuestID))
                    p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(t_QuestID, 4);

                if (p_Player->HasQuest(t_QuestID) || p_Player->IsQuestRewarded(t_QuestID))
                    p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::DefaultWorkOrder, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

                p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

                return true;
            }

            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 /*p_Action*/) override
            {
                if (p_Player && p_Creature && p_Creature->ToGarrisonNPCAI() && p_Creature->GetScriptName() == CreatureScript::GetName())
                    p_Creature->ToGarrisonNPCAI()->SendShipmentCrafterUI(p_Player);

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
    template<char const* t_ScriptName, SkillType t_Skill, uint32 t_QuestID, uint32 t_CreatureEntry, InitSequenceFunction* t_SetupLevel1, InitSequenceFunction* t_SetupLevel2, InitSequenceFunction* t_SetupLevel3>
    class ProfessionBuilding_SkillNPC : public SimpleSequenceCosmeticScript<t_ScriptName, t_SetupLevel1, t_SetupLevel2, t_SetupLevel3>
    {
        public:
            /// Constructor
            ProfessionBuilding_SkillNPC()
                : SimpleSequenceCosmeticScript<t_ScriptName, t_SetupLevel1, t_SetupLevel2, t_SetupLevel3>()
            {
                m_Recipes = sObjectMgr->GetNpcRecipesConditions(t_CreatureEntry);
            }

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target GameObject instance
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
            {
                if (p_Player->IsQuestRewarded(t_QuestID) && p_Player->HasSkill(t_Skill))
                {
                    p_Player->GetSession()->SendListInventory(p_Creature->GetGUID());
                }
                else
                {
                    if (!p_Player->HasQuest(t_QuestID) && !p_Player->IsQuestRewarded(t_QuestID))
                        p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(t_QuestID, 4);

                    if (!p_Player->HasSkill(t_Skill))
                        p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::DefaultSomething, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
                    else
                        p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::DefaultSomething, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

                    p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
                }

                return true;
            }

            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 p_Action) override
            {
                p_Player->CLOSE_GOSSIP_MENU();

                if (!p_Player->HasSkill(t_Skill) && p_Action == GOSSIP_ACTION_INFO_DEF)
                {
                    if (p_Player && p_Creature && p_Creature->ToGarrisonNPCAI() && p_Creature->GetScriptName() == CreatureScript::GetName())
                    {
                        GarrisonNPCAI* l_AI = p_Creature->ToGarrisonNPCAI();

                        if (l_AI == nullptr)
                            return true;

                        l_AI->SetRecipes(m_Recipes, t_Skill);
                        l_AI->SendTradeSkillUI(p_Player);
                    }
                }
                else if (p_Player->HasSkill(t_Skill) && p_Action == GOSSIP_ACTION_INFO_DEF + 1 && p_Player->GetSession())
                    p_Player->GetSession()->SendListInventory(p_Creature->GetGUID());

                return true;
            }

        private:
            std::vector<RecipesConditions> m_Recipes;
    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_PROFESSION_BUILDING_HPP_GARRISON
