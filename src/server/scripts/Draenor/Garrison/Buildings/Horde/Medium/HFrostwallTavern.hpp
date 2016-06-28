////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#ifndef GARRISON_FROSTWALL_TAVERN_HORDE_HPP_GARRISON
#define GARRISON_FROSTWALL_TAVERN_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    static const std::vector<SequencePosition> g_QuestGiverHordePositions
    {
        /// LEVEL 1

        /// In case 1 NPC spawned
        { 5.0805f, 1.8244f, 0.7690f, 4.8670f },
        /// In case 2 NPCs spawned
        { 2.4753f,  4.4374f, 0.7670f, 2.8446f },
        { 5.6733f, -5.6060f, 0.8357f, 5.3108f },


        /// LEVEL 2

        /// In case 1 NPC spawned
        { -10.9676f, -0.2555f, 0.7614f, 5.8506f },
        /// In case 2 NPCs spawned
        { -9.1868f, -6.6976f, 0.7639f, 0.5099f },
        { -9.8265f, 4.4834f, 0.7637f, 5.8553f },


        /// LEVEL 3

        /// In case 1 NPC spawned
        { -14.6658f, 4.3312f, 0.7334f, 5.8089f },
        /// In case 2 NPCs spawned
        { -15.6095f, -0.3702f, 0.7307f, 3.2563f },
        { -16.6565f, -6.5370f, 0.7443f, 6.1780f }
    };

    //////////////////////////////////////////////////////////////////////////
    /// 79828 - Murg                                                       ///
    //////////////////////////////////////////////////////////////////////////

    class npc_MurgAI : public GarrisonNPCAI
    {
        public:
            /// Constructor
            npc_MurgAI(Creature* p_Creature);

            virtual void OnSetPlotInstanceID(uint32 p_PlotInstanceID) override;

            virtual void OnDailyDataReset() override;
    };

    class npc_Murg : public CreatureScript
    {
        public:
            /// Constructor
            npc_Murg();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const;
    };

    class npc_akanja_garr : public CreatureScript
    {
        public:
            /// Constructor
            npc_akanja_garr();

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
            struct npc_akanja_garrAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_akanja_garrAI(Creature* p_Creature);

                /// When the PlotInstance ID is set
                /// @p_BuildingID : Set plot instance ID
                virtual void OnSetPlotInstanceID(uint32 p_PlotInstanceID) override;
            };

    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_FROSTWALL_TAVERN_HORDE_HPP_GARRISON
#endif
