////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_TRADING_POST_ALLIANCE_HPP_GARRISON
#define GARRISON_TRADING_POST_ALLIANCE_HPP_GARRISON

#include "../../GarrisonScriptData.hpp"
#include "../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 87212 - Fayla Fairfeather                                          ///
    //////////////////////////////////////////////////////////////////////////
    class npc_TraderJoseph : public CreatureScript
    {
        public:
            /// Constructor
            npc_TraderJoseph();

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target GameObject instance
            virtual bool OnGossipHello(Player * p_Player, Creature * p_Creature) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            virtual CreatureAI * GetAI(Creature * p_Creature) const override;

            /// Creature AI
            struct npc_TraderJosephAI : public GarrisonNPCAI
            {
                /// Constructor
                npc_TraderJosephAI(Creature * p_Creature);

            };
    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_TRADING_POST_ALLIANCE_HPP_GARRISON