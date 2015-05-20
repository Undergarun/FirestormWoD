////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_GEM_BOUTIQUE_HORDE_HPP_GARRISON
#define GARRISON_GEM_BOUTIQUE_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 79832 - Dorogarr                                                  ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_DorogarrAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;
    }

    using npc_DorogarrlAI = SimpleSequenceCosmeticScriptAI<&npc_DorogarrAIData::FnLevel1, &npc_DorogarrAIData::FnLevel2, &npc_DorogarrAIData::FnLevel3>;

    class npc_Dorogarr : public CreatureScript
    {
        public:
            /// Constructor
            npc_Dorogarr();

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
    /// 79830 - Elrondir Surrion                                          ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_ElrondirSurrionAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;
    }

    using npc_ElrondirSurrionAI = SimpleSequenceCosmeticScriptAI<&npc_ElrondirSurrionAIData::FnLevel1, &npc_ElrondirSurrionAIData::FnLevel2, &npc_ElrondirSurrionAIData::FnLevel3>;

    class npc_ElrondirSurrion : public CreatureScript
    {
        public:
            /// Constructor
            npc_ElrondirSurrion();

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

#endif  ///< GARRISON_GEM_BOUTIQUE_HORDE_HPP_GARRISON