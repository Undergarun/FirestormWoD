////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_LUNARFALL_INN_ALLIANCE_HPP_GARRISON
#define GARRISON_LUNARFALL_INN_ALLIANCE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    static const std::vector<SequencePosition> g_QuestGiverAlliancePositions
    {
        /// In case 1 NPC spawned
        { 5.9921f, 1.9356f, 0.3146f, 6.2315f },
        /// In case 2 NPCs spawned
        { 9.8532f, -5.5630f, 0.3617f, 4.6607f },
        { 1.9950f, -3.4458f, 0.3356f, 3.1527f }
    };

    //////////////////////////////////////////////////////////////////////////
    /// 77368 - Madison Clark                                              ///
    //////////////////////////////////////////////////////////////////////////

    class npc_MadisonClarkAI : public GarrisonNPCAI
    {
        public:
            /// Constructor
            npc_MadisonClarkAI(Creature* p_Creature);

            virtual void OnSetPlotInstanceID(uint32 p_PlotInstanceID) override;

            virtual void OnDailyDataReset() override;
    };

    class npc_MadisonClark : public CreatureScript
    {
        public:
            /// Constructor
            npc_MadisonClark();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI* GetAI(Creature* p_Creature) const;
    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_LUNARFALL_INN_ALLIANCE_HPP_GARRISON