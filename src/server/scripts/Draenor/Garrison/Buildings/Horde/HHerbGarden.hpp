////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_HERB_GARDEN_HORDE_HPP_GARRISON
#ifndef CROSS
#define GARRISON_HERB_GARDEN_HORDE_HPP_GARRISON

#include "../../GarrisonScriptData.hpp"
#include "../../GarrisonNPC.hpp"
#include "../GatheringBuilding.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    /// Gather FMT : HerbSpawnType PrevSpawnTimeStamp NextSpawnTimeStamp BuildingLevel [for plot in plots]GobEntryOrHerbSpawnState[end_for]

    /// Herbs game object entries
    extern std::vector<uint32> g_HordeHerbsGobsEntry;

    /// FarmSimulator positions
    extern std::vector<GatheringPlotInfos> g_HordeHerbGardenFlowerPlot;

    //////////////////////////////////////////////////////////////////////////
    /// 85783 - Nali Softsoil                                              ///
    //////////////////////////////////////////////////////////////////////////
    namespace npc_NaliSoftOilAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];
    }

    using npc_NaliSoftOil = GatheringBuilding_WorkOrderNPC<npc_NaliSoftOilAIData::ScriptName, Quests::Horde_ClearingTheGarden, &npc_NaliSoftOilAIData::FnLevel1, &npc_NaliSoftOilAIData::FnLevel2, &npc_NaliSoftOilAIData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 81981 - Tarnon                                                     ///
    //////////////////////////////////////////////////////////////////////////
    class npc_TarnonAI : public GatheringBuildingMaster<&g_HordeHerbGardenFlowerPlot>
    {
        public:
            /// Constructor
            /// @p_Creature : AI Owner
            npc_TarnonAI(Creature* p_Creature);

            virtual void OnPlotInstanceUnload() override;

            /// When the PlotInstance ID is set
            /// @p_BuildingID : Set plot instance ID
            virtual void OnSetPlotInstanceID(uint32 p_PlotInstanceID) override;

            /// On player quest rewarded
            /// @p_Player : Subject
            /// @p_Quest  : Rewarded quest template
            /// @p_Opt    : Quest reward option (eg: Loot choose)
            virtual void sQuestReward(Player* p_Player, Quest const* p_Quest, uint32 p_Opt) override;

            /// @p_Action : Action ID
            virtual void DoAction(int32 const p_Action) override;

            /// Select game object entry for a fresh gathering spawn
            /// @p_MiscData : Misc data
            virtual uint32 SelectGameObjectEntryForGatheringSpawn(uint32 p_MiscData) override;
    };

    class npc_Tarnon : public CreatureScript
    {
        public:
            /// Constructor
            npc_Tarnon();

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target creature instance
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;

            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const override;
    };


}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_HERB_GARDEN_HORDE_HPP_GARRISON
#endif
