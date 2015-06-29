////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_FISHING_HUT_ALLIANCE_HPP_GARRISON
#define GARRISON_FISHING_HUT_ALLIANCE_HPP_GARRISON

#include "../../GarrisonScriptData.hpp"
#include "../../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    namespace FishingHut
    {
        enum Type
        {
            SpellFishingVisual = 154848
        };
    }

    //////////////////////////////////////////////////////////////////////////
    /// 82436 - Tharis Strongcast                                         ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_TharisStrongcastData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char gScriptName[];
    }

    using npc_TharisStrongcast = SimpleSequenceCosmeticScript<npc_TharisStrongcastData::gScriptName, &npc_TharisStrongcastData::FnLevel1, &npc_TharisStrongcastData::FnLevel2, &npc_TharisStrongcastData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 85708 - Segumi                                                    ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_SegumiData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char gScriptName[];
    }

    using npc_Segumi = SimpleSequenceCosmeticScript<npc_SegumiData::gScriptName, &npc_SegumiData::FnLevel1, &npc_SegumiData::FnLevel2, &npc_SegumiData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 77733 - Ron Ashton                                                ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_RonAshtonData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char gScriptName[];
    }

    using npc_RonAshton = SimpleSequenceCosmeticScript<npc_RonAshtonData::gScriptName, &npc_RonAshtonData::FnLevel1, &npc_RonAshtonData::FnLevel2, &npc_RonAshtonData::FnLevel3>;

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_FISHING_HUT_ALLIANCE_HPP_GARRISON