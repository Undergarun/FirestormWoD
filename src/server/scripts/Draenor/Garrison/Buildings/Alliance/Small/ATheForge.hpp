////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_THE_FORGE_ALLIANCE_HPP_GARRISON
#define GARRISON_THE_FORGE_ALLIANCE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 77359 - Auria Irondreamer
    //////////////////////////////////////////////////////////////////////////
    class npc_AuriaIrondreamer : public CreatureScript
    {
        public:
            /// Constructor
            npc_AuriaIrondreamer();

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target GameObject instance
            virtual bool OnGossipHello(Player * p_Player, Creature * p_Creature) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_AuriaIrondreamerAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_AuriaIrondreamerAI(Creature * p_Creature);

            };

    };

    //////////////////////////////////////////////////////////////////////////
    /// 77792 - Yulia Samras
    //////////////////////////////////////////////////////////////////////////
    class npc_YuliaSamras : public CreatureScript
    {
        public:
            /// Constructor
            npc_YuliaSamras();

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target GameObject instance
            virtual bool OnGossipHello(Player * p_Player, Creature * p_Creature) override;
            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_YuliaSamrasAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_YuliaSamrasAI(Creature * p_Creature);

            };

    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_THE_FORGE_ALLIANCE_HPP_GARRISON