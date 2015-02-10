////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_MGR_WORKORDER_HPP_GARRISON
#define GARRISON_MGR_WORKORDER_HPP_GARRISON

namespace MS { namespace Garrison 
{
    /// Garrison work order
    struct GarrisonWorkOrder
    {
        uint32 DatabaseID;      ///< Building DB ID
        uint32 PlotInstanceID;  ///< Plot instance ID
        uint32 ShipmentID;      ///< Shipment ID (CharShipment.db2)
        uint32 CompleteTime;    ///< Timestamp of completion

        /// Get list of 
        static std::vector<uint32> GetShipmentsForBuilding(uint32 p_BuildingRecID);
    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_MGR_WORKORDER_HPP_GARRISON