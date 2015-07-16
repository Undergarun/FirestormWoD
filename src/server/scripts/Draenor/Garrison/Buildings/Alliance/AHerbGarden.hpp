////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_LUNARFALL_EXCAVATION_ALLIANCE_HPP_GARRISON
#define GARRISON_LUNARFALL_EXCAVATION_ALLIANCE_HPP_GARRISON

#include "../../GarrisonScriptData.hpp"
#include "../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 85514 - Olly Nimkip                                                ///
    //////////////////////////////////////////////////////////////////////////
    #pragma region 
    /// Cosmetic data
    namespace npc_OllyNimkipData
    {
        /// Cosmetic init for level 1
        extern InitSequenceFunction FnLevel1;
        /// Cosmetic init for level 2
        extern InitSequenceFunction FnLevel2;
        /// Cosmetic init for level 3
        extern InitSequenceFunction FnLevel3;
    }   ///< namespace npc_OllyNimkipData

    /// Main script
    class npc_OllyNimkip : CreatureScript
    {
        public:
            /// Constructor
            npc_OllyNimkip();

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
            CreatureAI * GetAI(Creature * p_Creature) const override;

    };
    #pragma endregion 

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_HERB_GARDEN_ALLIANCE_HPP_GARRISON