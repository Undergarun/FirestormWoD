////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_THE_FORGE_LEVEL_1_DATA_ALLIANCE_HPP_GARRISON
#ifndef CROSS
#define GARRISON_THE_FORGE_LEVEL_1_DATA_ALLIANCE_HPP_GARRISON

#include "../../../GarrisonScriptData.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    namespace AuriaIrondreamer_Level1
    {
        namespace MovePointIDs
        {
            enum Type
            {
                ForgeOre        = 100,
                ForgeFront      = 101,
                Workorder       = 102,
                WorkorderDown   = 103,
                WorkorderFront  = 104,
                ChestLeft       = 105,
            };
        }

        namespace DestPointDuration
        {
            enum
            {
                ForgeOre       = 35 * IN_MILLISECONDS,
                ForgeFront     = 35 * IN_MILLISECONDS,
                Workorder      = 35 * IN_MILLISECONDS,
                WorkorderDown  =  3 * IN_MILLISECONDS,
                WorkorderFront = 35 * IN_MILLISECONDS,
                ChestLeft      = 35 * IN_MILLISECONDS,
            };
        }

        static uint8 Sequence[] =
        {
            MovePointIDs::ForgeOre,
            MovePointIDs::ForgeFront,
            MovePointIDs::ForgeOre,
            MovePointIDs::Workorder,
            MovePointIDs::ForgeFront,
            MovePointIDs::Workorder,
            MovePointIDs::WorkorderDown,
            MovePointIDs::WorkorderFront,
            MovePointIDs::ChestLeft
        };

        static SequencePosition MovePointLoc[] =
        {
            { -2.0217f,  4.1932f, 0.7731f, 2.5023f },   ///< MovePointIDs::ForgeOre
            { -0.6606f,  2.6584f, 0.7726f, 6.1426f },   ///< MovePointIDs::ForgeFront
            {  5.6272f, -3.0714f, 0.8021f, 0.1579f },   ///< MovePointIDs::Workorder
            {  9.1293f, -1.9385f, 0.0000f, 3.6411f },   ///< MovePointIDs::WorkorderDown
            {  9.1293f, -1.9385f, 0.0000f, 0.3935f },   ///< MovePointIDs::WorkorderFront
            {  1.6735f, -4.8942f, 0.7983f, 4.6229f },   ///< MovePointIDs::ChestLeft
        };
    }

    namespace YuliaSamras_Level1
    {
        namespace MovePointIDs
        {
            enum Type
            {
                Workorder = 100,
            };
        }

        namespace DestPointDuration
        {
            enum
            {
                Workorder = 35 * IN_MILLISECONDS,
            };
        }

        static uint8 Sequence[] =
        {
            MovePointIDs::Workorder,
        };

        static SequencePosition MovePointLoc[] =
        {
            { 5.6272f, -3.0714f, 0.8021f, 0.1579f },   ///< MovePointIDs::Workorder
        };
    }

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_THE_FORGE_LEVEL_1_DATA_ALLIANCE_HPP_GARRISON
#endif
