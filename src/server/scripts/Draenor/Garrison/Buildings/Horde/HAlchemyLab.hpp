////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_ALCHEMY_LAB_HORDE_HPP_GARRISON
#define GARRISON_ALCHEMY_LAB_HORDE_HPP_GARRISON

#include "../../GarrisonScriptData.hpp"
#include "../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 79813 - Albert De Hyde                                            ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_AlbertDeHydeAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;
    }

    using npc_AlbertDeHydeAI = SimpleSequenceCosmeticScriptAI<&npc_AlbertDeHydeAIData::FnLevel1, &npc_AlbertDeHydeAIData::FnLevel2, &npc_AlbertDeHydeAIData::FnLevel3>;

    class npc_AlbertDeHyde : public CreatureScript
    {
        public:
            /// Constructor
            npc_AlbertDeHyde();

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
    /// 77791 - Keyana Tone                                               ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_KeyanaToneAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;
    }

    using npc_KeyanaToneAI = SimpleSequenceCosmeticScriptAI<&npc_KeyanaToneAIData::FnLevel1, &npc_KeyanaToneAIData::FnLevel2, &npc_KeyanaToneAIData::FnLevel3>;

    class npc_KeyanaTone : public CreatureScript
    {
        public:
            /// Constructor
            npc_KeyanaTone();

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

#endif  ///< GARRISON_ALCHEMY_LAB_HORDE_HPP_GARRISON