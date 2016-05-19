////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef GARRISON_SHIPMENT_MANAGER_HPP_GARRISON
# define GARRISON_SHIPMENT_MANAGER_HPP_GARRISON

#include "Common.h"

namespace MS { namespace Garrison
{
    /// Shipment manager class
    class ShipmentManager
    {
        /// Grant access to ACE_Singleton class
        friend class ACE_Singleton<ShipmentManager, ACE_Null_Mutex>;

        private:
            /// Constructor
            ShipmentManager();
            /// Destructor
            ~ShipmentManager();

        public:
            /// Init shipment manager
            void Init();

            /// Get shipment ID for specific building & player
            uint32 GetShipmentIDForBuilding(uint32 p_BuildingID, Player * p_Target, bool p_ForStartWorkOrder = false);

        private:
            std::map<uint32, uint32> m_ShipmentPerBuildingType;         ///< Shipment ID per building
            std::map<uint32, uint32> m_QuestShipmentPerBuildingType;    ///< Quest shipment ID per building

    };

}   ///< namespace Garrison
}   ///< namespace MS

#define sGarrisonShipmentManager ACE_Singleton<MS::Garrison::ShipmentManager, ACE_Null_Mutex>::instance()

#endif  ///< GARRISON_SHIPMENT_MANAGER_HPP_GARRISON
