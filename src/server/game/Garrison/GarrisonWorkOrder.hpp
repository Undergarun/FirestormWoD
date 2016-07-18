////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_MGR_WORKORDER_HPP_GARRISON
#ifndef CROSS
#define GARRISON_MGR_WORKORDER_HPP_GARRISON

namespace MS { namespace Garrison
{
    /// Garrison work order
    struct GarrisonWorkOrder
    {
        uint32 DatabaseID;      ///< Building DB ID
        uint32 PlotInstanceID;  ///< Plot instance ID
        uint32 ShipmentID;      ///< Shipment ID (CharShipment.db2)
        uint32 CreationTime;    ///< Timestamp of creation
        uint32 CompleteTime;    ///< Timestamp of completion
    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_MGR_WORKORDER_HPP_GARRISON
#endif
