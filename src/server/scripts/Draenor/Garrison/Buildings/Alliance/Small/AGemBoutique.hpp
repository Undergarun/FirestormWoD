////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_GEM_BOUTIQUE_ALLIANCE_HPP_GARRISON
#define GARRISON_GEM_BOUTIQUE_ALLIANCE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "../../ProfessionBuilding.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 77356 - Costan Highwall                                           ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_CostanHighwallAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];

        extern std::vector<RecipesConditions> Recipes;
    }

    using npc_CostanHighwall = ProfessionBuilding_SkillNPC<npc_CostanHighwallAIData::ScriptName, SKILL_JEWELCRAFTING, Quests::Alliance_YourFirstJewelcraftingWorkOrder, 77356, &npc_CostanHighwallAIData::FnLevel1, &npc_CostanHighwallAIData::FnLevel2, &npc_CostanHighwallAIData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 77775 - Kaya Solasen                                              ////
    //////////////////////////////////////////////////////////////////////////

    class npc_KayaSolasen : public CreatureScript
    {
        public:
            /// Constructor
            npc_KayaSolasen();

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target GameObject instance
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;

            virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            virtual CreatureAI* GetAI(Creature* p_Creature) const override;

            /// Creature AI
            struct npc_KayaSolasenAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_KayaSolasenAI(Creature* p_Creature);

                uint64 m_OwnerGuid = 0;

                void SetGUID(uint64 p_Guid, int32 p_Id) override;

                /// When the PlotInstance ID is set
                /// @p_BuildingID : Set plot instance ID
                virtual void OnSetPlotInstanceID(uint32 p_PlotInstanceID) override;
            };
    };
}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_GEM_BOUTIQUE_ALLIANCE_HPP_GARRISON