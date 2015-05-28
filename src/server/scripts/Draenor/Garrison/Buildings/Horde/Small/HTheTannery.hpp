////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_THE_TANNERY_HORDE_HPP_GARRISON
#define GARRISON_THE_TANNERY_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 77383 - Anders Longstitch                                         ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_MurneGreenhoofAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;
    }

    using npc_MurneGreenhoofAI = SimpleSequenceCosmeticScriptAI<&npc_MurneGreenhoofAIData::FnLevel1, &npc_MurneGreenhoofAIData::FnLevel2, &npc_MurneGreenhoofAIData::FnLevel3>;

    class npc_MurneGreenhoof : public CreatureScript
    {
        public:
            /// Constructor
            npc_MurneGreenhoof();

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
    /// 79833 - Yanney                                                    ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_YanneyAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;
    }

    using npc_YanneyAI = SimpleSequenceCosmeticScriptAI<&npc_YanneyAIData::FnLevel1, &npc_YanneyAIData::FnLevel2, &npc_YanneyAIData::FnLevel3>;

    class npc_Yanney : public CreatureScript
    {
        public:
            /// Constructor
            npc_Yanney();

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

#endif  ///< GARRISON_THE_TANNERY_HORDE_HPP_GARRISON