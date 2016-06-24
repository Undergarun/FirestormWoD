////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_MISSION_HPP_GARRISON
#define GARRISON_MISSION_HPP_GARRISON

#include "GarrisonMgrConstants.hpp"

namespace MS { namespace Garrison
{
    /// Garrison mission
    struct GarrisonMission
    {
        uint32 DatabaseID;          ///< Mission DB ID
        uint32 MissionID;           ///< Mission ID (GarrMission.db2)
        uint32 OfferTime;           ///< Offer time
        uint32 OfferMaxDuration;    ///< Offer max duration
        uint32 StartTime;           ///< Start time
        Mission::State State;  ///< Mission state
    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_MISSION_HPP_GARRISON
