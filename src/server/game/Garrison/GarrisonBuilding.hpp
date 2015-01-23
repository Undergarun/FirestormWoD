////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_MGR_BUILDING_HPP_GARRISON
#define GARRISON_MGR_BUILDING_HPP_GARRISON

namespace MS { namespace Garrison 
{
    /// Garrison building
    struct GarrisonBuilding
    {
        uint32 DatabaseID;      ///< Building DB ID
        uint32 PlotInstanceID;  ///< Plot instance ID
        uint32 BuildingID;      ///< Building ID (GarrBuilding.db2)
        uint32 SpecID;          ///< Building spec ID
        uint32 TimeBuiltStart;  ///< Time built start
        uint32 TimeBuiltEnd;    ///< Time build end

        bool Active;            ///< Is building active
        bool BuiltNotified;     ///< Is built notified
    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_MGR_BUILDING_HPP_GARRISON