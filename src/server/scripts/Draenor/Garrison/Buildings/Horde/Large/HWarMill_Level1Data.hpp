////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef GARRISON_WAR_MILL_LEVEL_1_DATA_HORDE_HPP_GARRISON
#define GARRISON_WAR_MILL_LEVEL_1_DATA_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    namespace GrunLek
    {
        namespace MovePointIDs
        {
            enum Type
            {
                Out     = 100,
                Carpet  = 101,
                Chest   = 102,
                Table   = 103
            };
        }

        namespace DestPointDuration
        {
            enum
            {
                Out     = 10 * IN_MILLISECONDS,
                Carpet  = 7  * IN_MILLISECONDS,
                Chest   = 5  * IN_MILLISECONDS,
                Table   = 20 * IN_MILLISECONDS
            };
        }

        static uint8 Sequence[] =
        {
            MovePointIDs::Out,
            MovePointIDs::Carpet,
            MovePointIDs::Chest,
            MovePointIDs::Carpet,
            MovePointIDs::Chest,
            MovePointIDs::Table
        };

        static SequencePosition MovePointLoc[] =
        {
            { 17.9442f, -1.0791f, 0.3122f, 6.0833f },   ///< MovePointIDs::Out
            {  3.2029f, -0.1679f, 1.1216f, 0.1070f },   ///< MovePointIDs::Carpet
            { -0.3188f,  0.1821f, 1.1216f, 3.2722f },   ///< MovePointIDs::Chest
            {  4.3978f,  1.8508f, 1.1218f, 2.2001f }    ///< MovePointIDs::Table
        };
    }

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_WAR_MILL_LEVEL_1_DATA_HORDE_HPP_GARRISON
