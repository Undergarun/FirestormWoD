////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_TAILORING_EMPORIUM_ALLIANCE_HPP_GARRISON
#define GARRISON_TAILORING_EMPORIUM_ALLIANCE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 77382 - Christopher Macdonald                                     ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_ChristopherMacdonaldData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char gScriptName[];
    }

    using npc_ChristopherMacdonald = SimpleSequenceCosmeticScript<npc_ChristopherMacdonaldData::gScriptName, &npc_ChristopherMacdonaldData::FnLevel1, &npc_ChristopherMacdonaldData::FnLevel2, &npc_ChristopherMacdonaldData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 77778 - Kaylie Macdonald                                          ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_KaylieMacdonaldAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;
    }

    using npc_KaylieMacdonaldAI = SimpleSequenceCosmeticScriptAI<&npc_KaylieMacdonaldAIData::FnLevel1, &npc_KaylieMacdonaldAIData::FnLevel2, &npc_KaylieMacdonaldAIData::FnLevel3>;

    class npc_KaylieMacdonald : public CreatureScript
    {
        public:
            /// Constructor
            npc_KaylieMacdonald();

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target GameObject instance
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            virtual CreatureAI* GetAI(Creature* p_Creature) const override;

    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_TAILORING_EMPORIUM_ALLIANCE_HPP_GARRISON