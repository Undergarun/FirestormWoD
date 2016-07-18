////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_DWARVEN_BUNKER_LEVEL_1_DATA_ALLIANCE_HPP_GARRISON
#ifndef CROSS
#define GARRISON_DWARVEN_BUNKER_LEVEL_1_DATA_ALLIANCE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    namespace GussofForgefire_Level1
    {
        namespace MovePointIDs
        {
            enum Type
            {
                Nothing     = 100,
                Stairs1     = 101,
                Stairs2     = 102,
                Stairs3     = 103,
                Stairs4     = 104,
                Anvil       = 105,
                Canon       = 106
            };
        }

        namespace DestPointDuration
        {
            enum
            {
                Nothing     = 50 * IN_MILLISECONDS,
                Stairs4     = 40 * IN_MILLISECONDS,
                Anvil       = 30 * IN_MILLISECONDS,
                Canon       = 30 * IN_MILLISECONDS
            };
        }

        static uint8 Sequence[] =
        {
            MovePointIDs::Nothing,
            MovePointIDs::Stairs1,
            MovePointIDs::Stairs2,
            MovePointIDs::Stairs3,
            MovePointIDs::Stairs4,
            MovePointIDs::Stairs3,
            MovePointIDs::Stairs2,
            MovePointIDs::Stairs1,
            MovePointIDs::Anvil,
            MovePointIDs::Canon,
            MovePointIDs::Anvil,
            MovePointIDs::Canon,
            MovePointIDs::Nothing,
            MovePointIDs::Canon,
            MovePointIDs::Anvil
        };

        static SequencePosition MovePointLoc[] =
        {
            {  -6.5769f, -5.7238f, 0.3151f, 0.9118f },   ///< MovePointIDs::Nothing
            {  -5.2637f,  7.8586f, 0.4547f, 2.5800f },   ///< MovePointIDs::Stairs1
            {  -7.5862f,  9.2789f, 1.7972f, 2.9489f },   ///< MovePointIDs::Stairs2
            { -10.3781f,  8.7117f, 3.1357f, 3.7304f },   ///< MovePointIDs::Stairs3
            { -13.3514f,  9.4382f, 3.2421f, 2.2028f },   ///< MovePointIDs::Stairs4
            {  -0.5560f, -9.9787f, 0.3217f, 4.9244f },   ///< MovePointIDs::Anvil
            {  -8.5564f, -9.4336f, 0.3063f, 4.3707f }    ///< MovePointIDs::Canon
        };
    }

    namespace KristenStoneforge_Level1
    {
        namespace MovePointIDs
        {
            enum Type
            {
                Table       = 100,
                Stairs1     = 101,
                Stairs2     = 102,
                Stairs3     = 103,
                Stairs4     = 104,
                Chest       = 105,
                UpTable     = 106,
                CanonBalls  = 107
            };
        }

        namespace DestPointDuration
        {
            enum
            {
                Table       = 10 * IN_MILLISECONDS,
                Chest       = 10 * IN_MILLISECONDS,
                UpTable     = 20 * IN_MILLISECONDS,
                CanonBalls  =  5 * IN_MILLISECONDS
            };
        }

        static uint8 Sequence[] =
        {
            MovePointIDs::Table,
            MovePointIDs::Stairs1,
            MovePointIDs::Stairs2,
            MovePointIDs::Stairs3,
            MovePointIDs::Stairs4,
            MovePointIDs::Chest,
            MovePointIDs::UpTable,
            MovePointIDs::Chest,
            MovePointIDs::Stairs4,
            MovePointIDs::Stairs3,
            MovePointIDs::Stairs2,
            MovePointIDs::Stairs1,
            MovePointIDs::Table,
            MovePointIDs::Stairs1,
            MovePointIDs::Stairs2,
            MovePointIDs::Stairs3,
            MovePointIDs::Stairs4,
            MovePointIDs::UpTable,
            MovePointIDs::Stairs4,
            MovePointIDs::Stairs3,
            MovePointIDs::Stairs2,
            MovePointIDs::Stairs1,
            MovePointIDs::Table,
            MovePointIDs::CanonBalls
        };

        static SequencePosition MovePointLoc[] =
        {
            {  -7.0744f,  0.6789f, 0.3219f, 2.8745f },   ///< MovePointIDs::Table
            {  -4.2971f,  7.9247f, 0.3240f, 2.2305f },   ///< MovePointIDs::Stairs1
            {  -6.6770f, 10.7132f, 1.8032f, 2.5906f },   ///< MovePointIDs::Stairs2
            {  -9.1449f, 10.8494f, 1.8467f, 3.6941f },   ///< MovePointIDs::Stairs3
            { -11.2239f,  9.5250f, 3.1786f, 3.9061f },   ///< MovePointIDs::Stairs4
            { -16.2015f, -2.1603f, 3.2165f, 3.5134f },   ///< MovePointIDs::Chest
            { -16.5532f,  2.0093f, 3.2329f, 2.9236f },   ///< MovePointIDs::UpTable
            {   1.6793f,  7.7725f, 0.3264f, 0.0019f }    ///< MovePointIDs::CanonBalls
        };
    }

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_DWARVEN_BUNKER_LEVEL_1_DATA_ALLIANCE_HPP_GARRISON
#endif
