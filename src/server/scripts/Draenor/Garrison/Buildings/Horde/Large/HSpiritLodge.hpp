////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_SPIRIT_LODGE_HORDE_HPP_GARRISON
#ifndef CROSS
#define GARRISON_SPIRIT_LODGE_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 91582 - Apprentice Var'Nath                                        ///
    //////////////////////////////////////////////////////////////////////////
    namespace npc_VarshaData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    class npc_Varsha : public CreatureScript
    {
        public:
            /// Constructor
            npc_Varsha();

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target creature instance
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const override;
    };

    class npc_VarshaAI : public GarrisonNPCAI
    {
        public:
            /// Constructor
            npc_VarshaAI(Creature* p_Creature);

            virtual void OnSetPlotInstanceID(uint32 p_PlotInstanceID) override;
    };
}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_SPIRIT_LODGE_HORDE_HPP_GARRISON
#endif
