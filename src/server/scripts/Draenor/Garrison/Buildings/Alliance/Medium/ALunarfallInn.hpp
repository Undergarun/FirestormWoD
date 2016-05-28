////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef GARRISON_LUNARFALL_INN_ALLIANCE_HPP_GARRISON
#define GARRISON_LUNARFALL_INN_ALLIANCE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    static const std::vector<SequencePosition> g_QuestGiverAlliancePositions
    {
        /// LEVEL 3

        /// In case 1 NPC spawned
        { 5.9921f, 1.9356f, 0.3146f, 6.2315f },
        /// In case 2 NPCs spawned
        { 9.8532f, -5.5630f, 0.3617f, 4.6607f },
        { 1.9950f, -3.4458f, 0.3356f, 3.1527f },


        /// LEVEL 2

        /// In case 1 NPC spawned
        { 7.9766f, -3.4628f, 0.1306f, 1.7348f },
        /// In case 2 NPCs spawned
        { 0.2897f, -4.4478f, 0.1349f, 1.4167f },
        { -3.6048f, -1.8366f, 0.1263f, 1.5503f },


        /// LEVEL 3

        /// In case 1 NPC spawned
        { 0.f, 0.f, 0.f, 0.f },
        /// In case 2 NPCs spawned
        { 0.f, 0.f, 0.f, 0.f },
        { 0.f, 0.f, 0.f, 0.f }
    };

    //////////////////////////////////////////////////////////////////////////
    /// 77368 - Madison Clark                                              ///
    //////////////////////////////////////////////////////////////////////////

    class npc_MadisonClarkAI : public GarrisonNPCAI
    {
        public:
            /// Constructor
            npc_MadisonClarkAI(Creature* p_Creature);

            virtual void OnSetPlotInstanceID(uint32 p_PlotInstanceID) override;

            virtual void OnDailyDataReset() override;
    };

    class npc_MadisonClark : public CreatureScript
    {
        public:
            /// Constructor
            npc_MadisonClark();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const;
    };

    class npc_lysa_serion_garr : public CreatureScript
    {
        public:
            /// Constructor
            npc_lysa_serion_garr();

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
            struct npc_lysa_serion_garrAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_lysa_serion_garrAI(Creature* p_Creature);

                /// When the PlotInstance ID is set
                /// @p_BuildingID : Set plot instance ID
                virtual void OnSetPlotInstanceID(uint32 p_PlotInstanceID) override;
            };

    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_LUNARFALL_INN_ALLIANCE_HPP_GARRISON
