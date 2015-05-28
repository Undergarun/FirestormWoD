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
    }

    using npc_CostanHighwallAI = SimpleSequenceCosmeticScriptAI<&npc_CostanHighwallAIData::FnLevel1, &npc_CostanHighwallAIData::FnLevel2, &npc_CostanHighwallAIData::FnLevel3>;

    class npc_CostanHighwall : public CreatureScript
    {
        public:
            /// Constructor
            npc_CostanHighwall();

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
            virtual CreatureAI * GetAI(Creature * p_Creature) const override;

    };

    //////////////////////////////////////////////////////////////////////////
    /// 77775 - Kaya Solasen                                              ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_KayaSolasenAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;
    }

    using npc_KayaSolasenAI = SimpleSequenceCosmeticScriptAI<&npc_KayaSolasenAIData::FnLevel1, &npc_KayaSolasenAIData::FnLevel2, &npc_KayaSolasenAIData::FnLevel3>;

    class npc_KayaSolasen : public CreatureScript
    {
        public:
            /// Constructor
            npc_KayaSolasen();

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
            virtual CreatureAI * GetAI(Creature * p_Creature) const override;

    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_GEM_BOUTIQUE_ALLIANCE_HPP_GARRISON