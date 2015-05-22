////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_ALCHEMY_LAB_HORDE_HPP_GARRISON
#define GARRISON_ALCHEMY_LAB_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "../../../GarrisonNPC.hpp"
#include "../../ProfessionBuilding.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 79813 - Albert De Hyde                                            ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_AlbertDeHydeAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char gScriptName[];
    }

    using npc_AlbertDeHyde = SimpleSequenceCosmeticScript<npc_AlbertDeHydeAIData::gScriptName, &npc_AlbertDeHydeAIData::FnLevel1, &npc_AlbertDeHydeAIData::FnLevel2, &npc_AlbertDeHydeAIData::FnLevel3>;

    //////////////////////////////////////////////////////////////////////////
    /// 79814 - Keyana Tone                                               ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_KeyanaToneAIData
    {
        extern InitSequenceFunction FnLevel1;
        extern InitSequenceFunction FnLevel2;
        extern InitSequenceFunction FnLevel3;

        extern char gScriptName[];
    }

    using npc_KeyanaTone = SimpleSequenceCosmeticScript<npc_KeyanaToneAIData::gScriptName,&npc_KeyanaToneAIData::FnLevel1, &npc_KeyanaToneAIData::FnLevel2, &npc_KeyanaToneAIData::FnLevel3>;

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_ALCHEMY_LAB_HORDE_HPP_GARRISON