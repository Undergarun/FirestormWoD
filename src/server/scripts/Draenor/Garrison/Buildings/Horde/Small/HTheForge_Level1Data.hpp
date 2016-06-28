////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#ifndef GARRISON_THE_FORGE_LEVEL_1_DATA_HORDE_HPP_GARRISON
#define GARRISON_THE_FORGE_LEVEL_1_DATA_HORDE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    namespace OrgekIronhand_Level1
    {
        namespace MovePointIDs
        {
            enum Type
            {
                Anvil  = 100,
                Front  = 101,
                Forge1 = 102,
                Forge2 = 103,
                Chest  = 104,
            };
        }

        namespace DestPointDuration
        {
            enum
            {
                Anvil  = 35 * IN_MILLISECONDS,
                Front  = 15 * IN_MILLISECONDS,
                Forge1 = 10 * IN_MILLISECONDS,
                Forge2 = 10 * IN_MILLISECONDS,
                Chest  =  5 * IN_MILLISECONDS,
            };
        }

        static uint8 Sequence[] =
        {
            MovePointIDs::Anvil,
            MovePointIDs::Front,
            MovePointIDs::Forge1,
            MovePointIDs::Forge2,
            MovePointIDs::Front,
            MovePointIDs::Chest,
            MovePointIDs::Forge1,
            MovePointIDs::Forge2,
            MovePointIDs::Front,
        };

        static SequencePosition MovePointLoc[] =
        {
            {  1.1273f, -2.3209f,  0.6197f, 5.4307f },   ///< MovePointIDs::Anvil
            {  4.9331f,  1.2398f,  0.6199f, 0.2502f },   ///< MovePointIDs::Front
            { -0.3698f,  3.0644f,  0.6210f, 1.5305f },   ///< MovePointIDs::Forge1
            {  0.0264f,  2.1578f,  0.6197f, 1.6915f },   ///< MovePointIDs::Forge2
            { 10.0623f, -5.8536f, -0.1225f, 3.9730f },   ///< MovePointIDs::Chest
        };
    }

    namespace Kinja_Level1
    {
        namespace MovePointIDs
        {
            enum Type
            {
                Anvil  = 100,
                Front  = 101,
                Forge1 = 102,
                Forge2 = 103,
            };
        }

        namespace DestPointDuration
        {
            enum
            {
                Anvil  = 60 * IN_MILLISECONDS,
                Front  = 40 * IN_MILLISECONDS,
                Forge1 = 25 * IN_MILLISECONDS,
                Forge2 = 20 * IN_MILLISECONDS,
            };
        }

        static uint8 Sequence[] =
        {
            MovePointIDs::Front,
            MovePointIDs::Anvil,
            MovePointIDs::Forge1,
            MovePointIDs::Forge2,
            MovePointIDs::Front,
            MovePointIDs::Forge1,
            MovePointIDs::Forge2,
            MovePointIDs::Front,
        };

        static SequencePosition MovePointLoc[] =
        {
            {  1.1273f, -2.3209f,  0.6197f, 5.4307f },   ///< MovePointIDs::Anvil
            {  4.9331f,  1.2398f,  0.6199f, 0.2502f },   ///< MovePointIDs::Front
            { -0.3698f,  3.0644f,  0.6210f, 1.5305f },   ///< MovePointIDs::Forge1
            {  0.0264f,  2.1578f,  0.6197f, 1.6915f },   ///< MovePointIDs::Forge2
        };
    }

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_THE_FORGE_LEVEL_1_DATA_HORDE_HPP_GARRISON
#endif
