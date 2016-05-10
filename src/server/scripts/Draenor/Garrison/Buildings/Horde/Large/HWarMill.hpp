////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_WAR_MILL_HORDE_HPP_GARRISON
#define GARRISON_WAR_MILL_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 82436 - Grun Lek                                                  ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_GrunLekData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char gScriptName[];
    }

    using npc_GrunLek = SimpleSequenceCosmeticScript<npc_GrunLekData::gScriptName, &npc_GrunLekData::FnLevel1, &npc_GrunLekData::FnLevel2, &npc_GrunLekData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 79781 - Frostwall Grunt
    //////////////////////////////////////////////////////////////////////////
    class npc_FrostWallGrunt : public CreatureScript
    {
        public:
        /// Constructor
        npc_FrostWallGrunt();

        /// Called when a CreatureAI object is needed for the creature.
        /// @p_Creature : Target creature instance
        CreatureAI * GetAI(Creature * p_Creature) const;

        /// Creature AI
        struct npc_FrostWallGruntAI : public CreatureAI
        {
            /// Constructor
            npc_FrostWallGruntAI(Creature * p_Creature);

            /// On AI Update
            /// @p_Diff : Time since last update
            virtual void UpdateAI(const uint32 p_Diff) override;

            /// Set UInt32 value
            /// @p_ID    : Value ID
            /// @p_Value : Value
            virtual void SetData(uint32 p_ID, uint32 p_Value) override;
        };

    };

    //////////////////////////////////////////////////////////////////////////
    /// 87630 - Frostwall Smith
    //////////////////////////////////////////////////////////////////////////
    class npc_FrostWallSmith : public CreatureScript
    {
        public:
        /// Constructor
        npc_FrostWallSmith();

        /// Called when a CreatureAI object is needed for the creature.
        /// @p_Creature : Target creature instance
        CreatureAI * GetAI(Creature * p_Creature) const;

        /// Creature AI
        struct npc_FrostWallSmithAI : public CreatureAI
        {
            /// Constructor
            npc_FrostWallSmithAI(Creature * p_Creature);

            /// On AI Update
            /// @p_Diff : Time since last update
            virtual void UpdateAI(const uint32 p_Diff) override;

            /// Set UInt32 value
            /// @p_ID    : Value ID
            /// @p_Value : Value
            virtual void SetData(uint32 p_ID, uint32 p_Value) override;
        };

    };


}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_WAR_MILL_HORDE_HPP_GARRISON
