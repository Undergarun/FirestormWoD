////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_BUILDING_SCRIPTS_HPP_GARRISON
# define GARRISON_BUILDING_SCRIPTS_HPP_GARRISON

# include "../GarrisonScriptData.hpp"
# include "../GarrisonNPC.hpp"
# include "GarrisonMgr.hpp"
# include "SharedDefines.h"

namespace MS { namespace Garrison 
{
    /// Scripts directly related to buildings.
    class GarrisonBuildingAuraPlayerScript : PlayerScript
    {
        public:
            /// Constructor
            GarrisonBuildingAuraPlayerScript();

            /// Called when a player updates his area
            /// @p_Player : Player instance
            /// @p_NewZoneID   : New Zone
            /// @p_OldZoneID   : Old Zone
            /// @p_NewAreaID   : New Area
            virtual void OnUpdateZone(Player* p_Player, uint32 p_NewZoneID, uint32 p_OldZoneID, uint32 p_NewAreaID) override;

    };
}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_BUILDING_SCRIPTS_HPP_GARRISON