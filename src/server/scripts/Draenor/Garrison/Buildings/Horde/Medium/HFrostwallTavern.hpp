////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef GARRISON_FROSTWALL_TAVERN_HORDE_HPP_GARRISON
#define GARRISON_FROSTWALL_TAVERN_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    static const std::vector<SequencePosition> g_QuestGiverHordePositions
    {
        /// In case 1 NPC spawned
        { 5.0805f, 1.8244f, 0.7690f, 4.8670f },
        /// In case 2 NPCs spawned
        { 2.4753f,  4.4374f, 0.7670f, 2.8446f },
        { 5.6733f, -5.6060f, 0.8357f, 5.3108f }
    };

    //////////////////////////////////////////////////////////////////////////
    /// 79828 - Murg                                                       ///
    //////////////////////////////////////////////////////////////////////////

    class npc_MurgAI : public GarrisonNPCAI
    {
        public:
            /// Constructor
            npc_MurgAI(Creature* p_Creature);

            virtual void OnSetPlotInstanceID(uint32 p_PlotInstanceID) override;

            virtual void OnDataReset() override;
    };

    class npc_Murg : public CreatureScript
    {
        public:
            /// Constructor
            npc_Murg();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const;
    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_FROSTWALL_TAVERN_HORDE_HPP_GARRISON
