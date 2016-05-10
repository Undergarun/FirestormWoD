////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_TAILORING_EMPORIUM_LEVEL_1_DATA_ALLIANCE_HPP_GARRISON
#define GARRISON_TAILORING_EMPORIUM_LEVEL_1_DATA_ALLIANCE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    namespace ChristopherMacdonald_Level1
    {
        namespace MovePointIDs
        {
            enum Type
            {
                Home      = 100,
                Chest     = 101,
                Armory    = 102,
                Front     = 103,
                Materials = 104
            };
        }
 
        namespace DestPointDuration
        {
            enum
            {
                Home      = 20 * IN_MILLISECONDS,
                Chest     = 20 * IN_MILLISECONDS,
                Armory    = 20 * IN_MILLISECONDS,
                Front     = 50 * IN_MILLISECONDS,
                Materials = 20 * IN_MILLISECONDS
            };
        }
 
        static uint8 Sequence[] =
        {
            MovePointIDs::Home,
            MovePointIDs::Chest,
            MovePointIDs::Armory,
            MovePointIDs::Front,
            MovePointIDs::Materials
        };
 
        static SequencePosition MovePointLoc[] =
        {
            {  0.6727f,  4.6811f, 0.7797f, 5.8480f },  ///< MovePointIDs::Home
            {  1.5651f, -1.3510f, 0.7756f, 3.3543f },  ///< MovePointIDs::Chest
            { -1.8868f,  2.3694f, 0.7609f, 2.9852f },  ///< MovePointIDs::Armory
            {  5.1320f,  3.3638f, 0.7610f, 5.9147f },  ///< MovePointIDs::Front
            {  2.1128f, -6.1175f, 0.7611f, 5.7066f }   ///< MovePointIDs::Materials
        };
    }

    namespace KaylieMacdonald_Level1
    {
        namespace MovePointIDs
        {
            enum Type
            {
                Home  = 100,
                Chest = 101,
                Body  = 102
            };
        }

        namespace DestPointDuration
        {
            enum
            {
                Home  = 30 * IN_MILLISECONDS,
                Chest = 20 * IN_MILLISECONDS,
                Body  = 5 * IN_MILLISECONDS
            };
        }

        static uint8 Sequence[] =
        {
            MovePointIDs::Home,
            MovePointIDs::Chest,
            MovePointIDs::Body
        };

        static SequencePosition MovePointLoc[] =
        {
            {  4.3512f,  3.8832f, 0.7608f, 5.9030f },   ///< MovePointIDs::Home
            {  1.5651f, -1.3510f, 0.7756f, 3.3543f },   ///< MovePointIDs::Chest
            { -2.3853f,  4.9665f, 0.7608f, 3.0363f }    ///< MovePointIDs::Body
        };
    }

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_TAILORING_EMPORIUM_LEVEL_1_DATA_ALLIANCE_HPP_GARRISON
