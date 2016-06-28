////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_BARRACKS_ALLIANCE_HPP_GARRISON
#ifndef CROSS
#define GARRISON_BARRACKS_ALLIANCE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 77379 - Jonathan Stephens                                          ///
    //////////////////////////////////////////////////////////////////////////
    namespace npc_JonathanStephensData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char gScriptName[];
    }

    using npc_JonathanStephens = SimpleSequenceCosmeticScript<npc_JonathanStephensData::gScriptName, &npc_JonathanStephensData::FnLevel1, &npc_JonathanStephensData::FnLevel2, &npc_JonathanStephensData::FnLevel3>;

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_BARRACKS_ALLIANCE_HPP_GARRISON
#endif
