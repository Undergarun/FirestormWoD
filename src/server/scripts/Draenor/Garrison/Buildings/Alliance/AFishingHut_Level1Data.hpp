////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_FISHING_HUT_LEVEL_1_DATA_ALLIANCE_HPP_GARRISON
#ifndef CROSS
#define GARRISON_FISHING_HUT_LEVEL_1_DATA_ALLIANCE_HPP_GARRISON

#include "../../GarrisonScriptData.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    namespace TharisStrongcast_Level1
    {
        namespace MovePointIDs
        {
            enum Type
            {
                Home     = 100,
                Rack     = 101,
                Fishing1 = 102,
                Fishing2 = 103,
            };
        }

        namespace DestPointDuration
        {
            enum
            {
                Home     = 20 * IN_MILLISECONDS,
                Rack     = 20 * IN_MILLISECONDS,
                Fishing1 = 60 * IN_MILLISECONDS,
                Fishing2 = 5 * 60 * IN_MILLISECONDS
            };
        }

        static uint8 Sequence[] =
        {
            MovePointIDs::Home,
            MovePointIDs::Rack,
            MovePointIDs::Fishing1,
            MovePointIDs::Fishing2,
            MovePointIDs::Fishing1,
            MovePointIDs::Fishing2,
            MovePointIDs::Fishing1,
            MovePointIDs::Fishing2,
        };

        static SequencePosition MovePointLoc[] =
        {
            {  3.0280f,  -6.6115f, -0.1323f, 0.8351f },   ///< MovePointIDs::Home
            {  5.1373f,  -6.3564f, -0.0814f, 4.4872f },   ///< MovePointIDs::Rack
            { -0.4577f, -11.7021f, -1.4186f, 4.1966f },   ///< MovePointIDs::Fishing1
            { 19.7749f, -11.4867f, -0.8765f, 5.4886f },   ///< MovePointIDs::Fishing2
        };
    }

    namespace Segumi_Level1
    {
        namespace MovePointIDs
        {
            enum Type
            {
                Home    = 100,
                Logs    = 101,
                Fire    = 102,
                Rack    = 103,
                Mesh    = 104,
                Chests  = 105,
                Home2   = 106,
                Porch   = 107
            };
        }

        namespace DestPointDuration
        {
            enum
            {
                Home    = 30 * IN_MILLISECONDS,
                Logs    = 30 * IN_MILLISECONDS,
                Fire    = 20 * IN_MILLISECONDS,
                Rack    = 30 * IN_MILLISECONDS,
                Mesh    = 15 * IN_MILLISECONDS,
                Chests  = 30 * IN_MILLISECONDS,
                Home2   = 30 * IN_MILLISECONDS
            };
        }

        static uint8 Sequence[] =
        {
            MovePointIDs::Home,
            MovePointIDs::Logs,
            MovePointIDs::Fire,
            MovePointIDs::Rack,
            MovePointIDs::Logs,
            MovePointIDs::Porch,
            MovePointIDs::Mesh,
            MovePointIDs::Chests,
            MovePointIDs::Porch,
            MovePointIDs::Home2,
            MovePointIDs::Rack,
            MovePointIDs::Porch,
            MovePointIDs::Chests,
            MovePointIDs::Porch,
            MovePointIDs::Fire,
            MovePointIDs::Home2,
            MovePointIDs::Fire,
            MovePointIDs::Porch,
            MovePointIDs::Mesh,
            MovePointIDs::Chests,
            MovePointIDs::Porch
        };

        static SequencePosition MovePointLoc[] =
        {
            {  1.9692f,  -2.3473f, -0.0007f, 0.4258f },   ///< MovePointIDs::Home
            { 11.9238f, -10.2916f, -0.1412f, 5.0675f },   ///< MovePointIDs::Logs
            {  5.9388f,  -2.0943f,  0.0006f, 1.4075f },   ///< MovePointIDs::Fire
            { 14.7156f,   1.2301f,  0.0001f, 6.2416f },   ///< MovePointIDs::Rack
            {  3.3521f,   9.6698f, -0.0000f, 2.2440f },   ///< MovePointIDs::Mesh
            {  6.2719f,  11.2433f, -0.0000f, 0.9088f },   ///< MovePointIDs::Chests
            {  2.0680f,   2.2712f,  0.0001f, 5.7665f },   ///< MovePointIDs::Home2
            {  5.9734f,   6.4648f, -0.0001f, 2.2096f }    ///< MovePointIDs::Porch
        };
    }

    namespace RonAshton_Level1
    {
        namespace MovePointIDs
        {
            enum Type
            {
                Home    = 100,
                Hut1    = 101,
                Chests  = 102,
                Fire    = 103,
                Hut2    = 104,
                Mesh    = 105,
                Logs    = 106,
                Porch   = 107
            };
        }

        namespace DestPointDuration
        {
            enum
            {
                Home    = 20 * IN_MILLISECONDS,
                Hut1    = 20 * IN_MILLISECONDS,
                Chests  = 30 * IN_MILLISECONDS,
                Fire    = 10 * IN_MILLISECONDS,
                Hut2    = 20 * IN_MILLISECONDS,
                Mesh    = 15 * IN_MILLISECONDS,
                Logs    = 60 * IN_MILLISECONDS
            };
        }

        static uint8 Sequence[] =
        {
            MovePointIDs::Home,
            MovePointIDs::Hut1,
            MovePointIDs::Porch,
            MovePointIDs::Chests,
            MovePointIDs::Porch,
            MovePointIDs::Fire,
            MovePointIDs::Hut2,
            MovePointIDs::Hut1,
            MovePointIDs::Porch,
            MovePointIDs::Chests,
            MovePointIDs::Porch,
            MovePointIDs::Home,
            MovePointIDs::Porch,
            MovePointIDs::Mesh,
            MovePointIDs::Porch,
            MovePointIDs::Fire,
            MovePointIDs::Porch,
            MovePointIDs::Chests,
            MovePointIDs::Porch,
            MovePointIDs::Hut1,
            MovePointIDs::Logs,
            MovePointIDs::Porch,
            MovePointIDs::Mesh,
            MovePointIDs::Porch,
            MovePointIDs::Hut2,
            MovePointIDs::Logs,
            MovePointIDs::Hut1,
        };

        static SequencePosition MovePointLoc[] =
        {
            { 14.5898f,   1.4911f,  0.0001f, 6.1513f },   ///< MovePointIDs::Home
            {  1.9229f,  -2.3401f,  0.0001f, 0.1705f },   ///< MovePointIDs::Hut1
            {  6.2719f,  11.2433f, -0.0000f, 0.9088f },   ///< MovePointIDs::Chests
            {  5.9388f,  -2.0943f,  0.0006f, 1.4075f },   ///< MovePointIDs::Fire
            {  1.9393f,   2.0729f,  0.0002f, 6.2534f },   ///< MovePointIDs::Hut2
            {  3.3521f,   9.6698f, -0.0000f, 2.2440f },   ///< MovePointIDs::Mesh
            { 11.9238f, -10.2916f, -0.1412f, 5.0675f },   ///< MovePointIDs::Logs
            {  5.9734f,   6.4648f, -0.0001f, 2.2096f }    ///< MovePointIDs::Porch
        };
    }

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_FISHING_HUT_LEVEL_1_DATA_ALLIANCE_HPP_GARRISON
#endif
