////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_HERB_GARDEN_ALLIANCE_HPP_GARRISON
#define GARRISON_HERB_GARDEN_ALLIANCE_HPP_GARRISON

#include "../../GarrisonScriptData.hpp"
#include "../../GarrisonNPC.hpp"
#include "../GatheringBuilding.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    /// Gather FMT : HerbSpawnType PrevSpawnTimeStamp NextSpawnTimeStamp BuildingLevel [for plot in plots]GobEntryOrHerbSpawnState[end_for]

    namespace HerbSpawnType
    {
        enum Type
        {
            Frostweed          = 0,
            Starflower         = 1,
            Fireweed           = 2,
            TaladorOrchid      = 3,
            GorgrondFlytrap    = 4,
            NagrandArrowbloom  = 5,
            Random             = 6
        };
    }

    namespace HerbAction
    {
        enum
        {
            Frostweed         = 0,
            Starflower        = 1,
            Fireweed          = 2,
            TaladorOrchid     = 3,
            GorgrondFlytrap   = 4,
            NagrandArrowbloom = 5
        };
    }

    /// Herbs game object entries
    extern std::vector<uint32> g_AllyHerbsGobsEntry;

    /// FarmSimulator/GoatSimulator positions
    extern std::vector<GatheringPlotInfos> g_AllyHerbGardenFlowerPlot;

    //////////////////////////////////////////////////////////////////////////
    /// 85514 - Olly Nimkip                                                ///
    //////////////////////////////////////////////////////////////////////////
    namespace npc_OllyNimkipAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char ScriptName[];
    }

    using npc_OllyNimkip = GatheringBuilding_WorkOrderNPC<npc_OllyNimkipAIData::ScriptName, Quests::Alliance_ClearingTheGarden, &npc_OllyNimkipAIData::FnLevel1, &npc_OllyNimkipAIData::FnLevel2, &npc_OllyNimkipAIData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    /// 85344 - Naron Bloomthistle                                         ///
    //////////////////////////////////////////////////////////////////////////
    class npc_NaronBloomthistleAI : public GatheringBuildingMaster<&g_AllyHerbGardenFlowerPlot>
    {
        public:
            /// Constructor
            /// @p_Creature : AI Owner
            npc_NaronBloomthistleAI(Creature* p_Creature);

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

    class npc_NaronBloomthistle : public CreatureScript
    {
        public:
            /// Constructor
            npc_NaronBloomthistle();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const override;

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

    };


}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_HERB_GARDEN_ALLIANCE_HPP_GARRISON