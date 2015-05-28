////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_THE_FORGE_HORDE_HPP_GARRISON
#define GARRISON_THE_FORGE_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 79867 - Orgek Ironhand                                             ///
    //////////////////////////////////////////////////////////////////////////
    class npc_OrgekIronhand : public CreatureScript
    {
        public:
            /// Constructor
            npc_OrgekIronhand();

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target GameObject instance
            virtual bool OnGossipHello(Player * p_Player, Creature * p_Creature) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_OrgekIronhandAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_OrgekIronhandAI(Creature * p_Creature);

            };

    };

    //////////////////////////////////////////////////////////////////////////
    /// 79817 - Kinja                                                      ///
    //////////////////////////////////////////////////////////////////////////
    class npc_Kinja : public CreatureScript
    {
        public:
            /// Constructor
            npc_Kinja();

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
            struct npc_KinjaAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_KinjaAI(Creature * p_Creature);

            };

    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_THE_FORGE_HORDE_HPP_GARRISON