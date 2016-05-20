////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef GARRISON_FISHING_HUT_HORDE_HPP_GARRISON
#define GARRISON_FISHING_HUT_HORDE_HPP_GARRISON

#include "../../GarrisonScriptData.hpp"
#include "../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 79892 - Mak'jin                                                   ////
    //////////////////////////////////////////////////////////////////////////
    
    class npc_MakJin_Garr : public CreatureScript
    {
        public:
            /// Constructor
            npc_MakJin_Garr();

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target GameObject instance
            virtual bool OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 p_Option) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            virtual CreatureAI* GetAI(Creature* p_Creature) const override;

    };
    
    class npc_MakJin_GarrAI : public GarrisonNPCAI
    {
        public:
            /// Constructor
            npc_MakJin_GarrAI(Creature* p_Creature);
    };
    
}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_FISHING_HUT_HORDE_HPP_GARRISON
