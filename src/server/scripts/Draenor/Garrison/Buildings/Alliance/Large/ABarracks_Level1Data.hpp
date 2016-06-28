////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_BARRACKS_LEVEL_1_DATA_ALLIANCE_HPP_GARRISON
#ifndef CROSS
#define GARRISON_BARRACKS_LEVEL_1_DATA_ALLIANCE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    namespace JonathanStephens_Level1
    {
        namespace MovePointIDs
        {
            enum Type
            {
                Table       = 100,
                Chair       = 101,
                Armory      = 102,
                Middle      = 103,
                Middle2     = 104,
                BackBed     = 105,
                Shields     = 106
            };
        }

        namespace DestPointDuration
        {
            enum
            {
                Table   = 35 * IN_MILLISECONDS,
                Chair   = 35 * IN_MILLISECONDS,
                Armory  = 35 * IN_MILLISECONDS,
                Middle  = 35 * IN_MILLISECONDS,
                Middle2 = 35 * IN_MILLISECONDS,
                BackBed = 35 * IN_MILLISECONDS,
                Shields = 35 * IN_MILLISECONDS
            };
        }

        static uint8 Sequence[] =
        {
            MovePointIDs::Table,
            MovePointIDs::Chair,
            MovePointIDs::Armory,
            MovePointIDs::Middle,
            MovePointIDs::Chair,
            MovePointIDs::Middle2,
            MovePointIDs::BackBed,
            MovePointIDs::Shields,
            MovePointIDs::Middle2
        };

        static SequencePosition MovePointLoc[] =
        {
            {  16.1678f, -2.8809f, 0.3448f, 5.6532f },   ///< MovePointIDs::Table
            {  16.2702f, -4.5576f, 0.3113f, 5.6924f },   ///< MovePointIDs::Chair
            {  14.7902f, -5.4950f, 0.3528f, 4.6047f },   ///< MovePointIDs::Armory
            {  11.9206f, -0.0325f, 0.3153f, 0.0446f },   ///< MovePointIDs::Middle
            {  13.8977f,  0.1359f, 0.3319f, 0.1153f },   ///< MovePointIDs::Middle2
            {   6.0455f,  2.9463f, 0.3333f, 0.0132f },   ///< MovePointIDs::BackBed
            {  14.3000f,  3.3470f, 0.3473f, 5.7820f }    ///< MovePointIDs::Shields
        };
    }

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_BARRACKS_LEVEL_1_DATA_ALLIANCE_HPP_GARRISON
#endif
