////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_THE_TANNERY_HORDE_HPP_GARRISON
#define GARRISON_THE_TANNERY_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "../../ProfessionBuilding.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 77383 - Anders Longstitch                                         ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_MurneGreenhoofAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];
    }

    using npc_MurneGreenhoof = ProfessionBuilding_SkillNPC<npc_MurneGreenhoofAIData::ScriptName, SKILL_LEATHERWORKING, Quests::Horde_YourFirstLeatherworkingWorkOrder, 79834, &npc_MurneGreenhoofAIData::FnLevel1, &npc_MurneGreenhoofAIData::FnLevel2, &npc_MurneGreenhoofAIData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 79833 - Yanney                                                    ////
    //////////////////////////////////////////////////////////////////////////

    class npc_Yanney : public CreatureScript
    {
        public:
            /// Constructor
            npc_Yanney();

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target GameObject instance
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;

            virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            virtual CreatureAI* GetAI(Creature* p_Creature) const override;

            /// Creature AI
            struct npc_YanneyAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_YanneyAI(Creature* p_Creature);

                uint64 m_OwnerGuid = 0;

                void SetGUID(uint64 p_Guid, int32 p_Id) override;

                /// When the PlotInstance ID is set
                /// @p_BuildingID : Set plot instance ID
                virtual void OnSetPlotInstanceID(uint32 p_PlotInstanceID) override;
            };
    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_THE_TANNERY_HORDE_HPP_GARRISON
