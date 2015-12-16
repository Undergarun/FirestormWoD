////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_STABLES_HORDE_HPP_GARRISON
#define GARRISON_STABLES_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 91582 - Apprentice Var'Nath                                        ///
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    class npc_Tormak : public CreatureScript
    {
        public:
            /// Constructor
            npc_Tormak();

            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const;
    };

    class npc_TormakAI : public GarrisonNPCAI
    {
        public:
            /// Constructor
            npc_TormakAI(Creature* p_Creature);

            virtual void OnSetPlotInstanceID(uint32 p_PlotInstanceID) override;
    };
}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_STABLES_HORDE_HPP_GARRISON