////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
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

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_DWARVEN_BUNKER_ALLIANCE_HPP_GARRISON
