////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_DWARVEN_BUNKER_ALLIANCE_HPP_GARRISON
#define GARRISON_DWARVEN_BUNKER_ALLIANCE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 84594 - Gussof Forgefire                                           ///
    //////////////////////////////////////////////////////////////////////////
    namespace npc_GussofForgefireData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char gScriptName[];
    }

    using npc_GussofForgefire = SimpleSequenceCosmeticScript<npc_GussofForgefireData::gScriptName, &npc_GussofForgefireData::FnLevel1, &npc_GussofForgefireData::FnLevel2, &npc_GussofForgefireData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 77377 - KristenStone forge                                         ///
    //////////////////////////////////////////////////////////////////////////
    namespace npc_KristenStoneforgeData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char gScriptName[];
    }

    using npc_KristenStoneforge = SimpleSequenceCosmeticScript<npc_KristenStoneforgeData::gScriptName, &npc_KristenStoneforgeData::FnLevel1, &npc_KristenStoneforgeData::FnLevel2, &npc_KristenStoneforgeData::FnLevel3>;
    
    //////////////////////////////////////////////////////////////////////////
    /// 89065 - Dalana Clarke
    //////////////////////////////////////////////////////////////////////////
    class npc_DalanaClarke_Garr : public CreatureScript
    {
        public:
        /// Constructor
            npc_DalanaClarke_Garr();

        /// Called when a player opens a gossip dialog with the GameObject.
        /// @p_Player     : Source player instance
        /// @p_Creature   : Target GameObject instance
        virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override;

        /// Called when a player selects a gossip item in the creature's gossip menu.
        /// @p_Player   : Source player instance
        /// @p_Creature : Target creature instance
        /// @p_Sender   : Sender menu
        /// @p_Action   : Action
        virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action) override;

        /// Called when a CreatureAI object is needed for the creature.
        /// @p_Creature : Target creature instance
        CreatureAI* GetAI(Creature* p_Creature) const;

        /// Creature AI
        struct npc_DalanaClarke_GarrAI : public GarrisonNPCAI
        {
            /// Constructor
            npc_DalanaClarke_GarrAI(Creature* p_Creature);
        };

    };
}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_DWARVEN_BUNKER_ALLIANCE_HPP_GARRISON