////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_THE_TANNERY_ALLIANCE_HPP_GARRISON
#define GARRISON_THE_TANNERY_ALLIANCE_HPP_GARRISON

#include "../../GarrisonScriptData.hpp"
#include "../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 77383 - Anders Longstitch                                         ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_AndersLongstitchAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;
    }

    using npc_AndersLongstitchAI = SimpleSequenceCosmeticScriptAI<&npc_AndersLongstitchAIData::FnLevel1, &npc_AndersLongstitchAIData::FnLevel2, &npc_AndersLongstitchAIData::FnLevel3>;

    class npc_AndersLongstitch : public CreatureScript
    {
        public:
            /// Constructor
            npc_AndersLongstitch();

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
    /// 78207 - Marianne Levine                                           ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_MarianneLevineAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;
    }

    using npc_MarianneLevineAI = SimpleSequenceCosmeticScriptAI<&npc_MarianneLevineAIData::FnLevel1, &npc_MarianneLevineAIData::FnLevel2, &npc_MarianneLevineAIData::FnLevel3>;

    class npc_MarianneLevine : public CreatureScript
    {
        public:
            /// Constructor
            npc_MarianneLevine();

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

#endif  ///< GARRISON_THE_TANNERY_ALLIANCE_HPP_GARRISON