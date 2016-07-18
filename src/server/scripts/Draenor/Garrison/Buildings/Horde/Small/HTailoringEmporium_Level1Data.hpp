////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#ifndef GARRISON_TAILORING_EMPORIUM_LEVEL_1_DATA_HORDE_HPP_GARRISON
#define GARRISON_TAILORING_EMPORIUM_LEVEL_1_DATA_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    namespace Turga_Level1
    {
        namespace MovePointIDs
        {
            enum Type
            {
                Home  = 100,
                Body  = 101,
                Table = 102
            };
        }

        namespace DestPointDuration
        {
            enum
            {
                Home  = 30 * IN_MILLISECONDS,
                Body  = 5  * IN_MILLISECONDS,
                Table = 20 * IN_MILLISECONDS
            };
        }

        static uint8 Sequence[] =
        {
            MovePointIDs::Home,
            MovePointIDs::Body,
            MovePointIDs::Table
        };

        static SequencePosition MovePointLoc[] =
        {
            {  0.8192f,  0.6506f, 0.6788f, 6.2753f },   ///< MovePointIDs::Home
            { -2.5395f,  3.2986f, 0.6788f, 2.4740f },   ///< MovePointIDs::Body
            {  7.2246f, -1.4732f, 0.6545f, 3.9113f }    ///< MovePointIDs::Table
        };
    }

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_TAILORING_EMPORIUM_LEVEL_1_DATA_HORDE_HPP_GARRISON
#endif
