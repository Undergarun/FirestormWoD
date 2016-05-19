////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_ENGINEERING_WORKS_HORDE_HPP_GARRISON
#define GARRISON_ENGINEERING_WORKS_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "../../ProfessionBuilding.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 79826 - Pozzlow                                                   ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_PozzlowAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];

        extern std::vector<RecipesConditions> Recipes;
    }

    using npc_Pozzlow = ProfessionBuilding_SkillNPC<npc_PozzlowAIData::ScriptName, SKILL_ENGINEERING, Quests::Horde_YourFirstEngineeringWorkOrder, 79826, &npc_PozzlowAIData::FnLevel1, &npc_PozzlowAIData::FnLevel2, &npc_PozzlowAIData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 86696 - Garbra Fizzwonk                                           ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_GarbraFizzwonkAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;
    }

    class npc_GarbraFizzwonk : public CreatureScript
    {
        public:
            /// Constructor
            npc_GarbraFizzwonk();

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target GameObject instance
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;

            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            virtual CreatureAI* GetAI(Creature* p_Creature) const override;

            /// Creature AI
            struct npc_GarbraFizzwonkAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_GarbraFizzwonkAI(Creature* p_Creature);

                uint64 m_OwnerGuid = 0;

                void SetGUID(uint64 p_Guid, int32 p_Id) override;

                /// When the PlotInstance ID is set
                /// @p_BuildingID : Set plot instance ID
                virtual void OnSetPlotInstanceID(uint32 p_PlotInstanceID) override;

                /// When the building ID is set
                /// @p_BuildingID : Set building ID
                virtual void OnSetBuildingID(uint32 p_BuildingID) override
                {
                    m_OnPointReached.clear();

                    GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingID);

                    if (!l_BuildingEntry)
                        return;

                    switch (l_BuildingEntry->Level)
                    {
                        case 1:
                            (npc_GarbraFizzwonkAIData::FnLevel1)(this, me);
                            break;

                        case 2:
                            (npc_GarbraFizzwonkAIData::FnLevel2)(this, me);
                            break;

                        case 3:
                            (npc_GarbraFizzwonkAIData::FnLevel3)(this, me);
                            break;

                        default:
                            break;
                    }
                }
            };

    };
}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_ENGINEERING_WORKS_HORDE_HPP_GARRISON
