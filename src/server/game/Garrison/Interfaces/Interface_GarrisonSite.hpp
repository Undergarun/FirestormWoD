////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef GARRISON_SITE_HPP_INTERFACES
#define GARRISON_SITE_HPP_INTERFACES

#include "Common.h"
#include "GarrisonMgrConstants.hpp"

class Player;
class Quest;

namespace MS { namespace Garrison { namespace Interfaces
{
    /// Garrison site interface
    class GarrisonSite
    {
        public:
            /// When the garrison owner started a quest
            /// @p_Owner : Garrison owner
            /// @p_Quest : Started quest
            virtual void OnQuestStarted(Player* p_Owner, const Quest* p_Quest) = 0;
            /// When the garrison owner reward a quest
            /// @p_Owner : Garrison owner
            /// @p_Quest : Rewarded quest
            virtual void OnQuestReward(Player* p_Owner, const Quest* p_Quest) = 0;
            /// When the garrison owner abandon a quest
            /// @p_Owner : Garrison owner
            /// @p_Quest : Abandoned quest
            virtual void OnQuestAbandon(Player* p_Owner, const Quest* p_Quest) = 0;

            /// Get phase mask
            /// @p_Owner : Garrison owner
            virtual uint32 GetPhaseMask(Player* p_Owner) = 0;

            /// Owner can use the garrison cache ?
            /// @p_Owner : Garrison owner
            virtual bool CanUseGarrisonCache(Player* p_Owner) = 0;

            /// Can upgrade the garrison
            /// @p_Owner                : Garrison owner
            /// @p_CurrentGarrisonLevel : Current garrison level
            virtual bool CanUpgrade(Player* p_Owner, uint32 p_CurrentGarrisonLevel) = 0;
            /// On upgrade the garrison
            /// @p_Owner : Garrison owner
            virtual void OnUpgrade(Player* p_Owner) = 0;

            /// When a mission start
            /// @p_Owner     : Garrison owner
            /// @p_MissionID : Started mission ID
            /// @p_Followers : Followers on the mission
            virtual void OnMissionStart(Player* p_Owner, uint32 p_MissionID, std::vector<uint32> p_Followers) = 0;
            /// When a construction start, compute build time
            /// @p_Owner      : Garrison owner
            /// @p_BuildingID : Purchased building ID
            /// @p_BaseTime   : Default build time
            virtual uint32 OnPrePurchaseBuilding(Player* p_Owner, uint32 p_BuildingID, uint32 p_BaseTime) = 0;
            /// When a construction start
            /// @p_Owner      : Garrison owner
            /// @p_BuildingID : Purchased building ID
            virtual void OnPurchaseBuilding(Player* p_Owner, uint32 p_BuildingID) = 0;
            /// When a building from the same type with higher level is purchased
            /// @p_Owner      : Garrison owner
            /// @p_BuildingID : Purchased building ID
            virtual void OnUpgradeBuilding(Player* p_Owner, uint32 p_BuildingID) = 0;
            /// When a building is activated
            /// @p_Owner      : Garrison owner
            /// @p_BuildingID : Purchased building ID
            virtual void OnBuildingActivated(Player* p_Owner, uint32 p_BuildingID) = 0;
            /// When a building is deleted
            /// @p_Owner        : Garrison owner
            /// @p_BuildingID   : Deleted building ID
            /// @p_BuildingType : Deleted Building Type
            virtual void OnDeleteBuilding(Player* p_Owner, uint32 p_BuildingID, uint32 p_BuildingType, bool p_RemoveForUpgrade) = 0;

            /// When the owner player change level
            /// @p_Level : New owner level
            virtual void OnOwnerLevelChange(uint32 p_Level) = 0;
    };

}   ///< namespace Interfaces
}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_SITE_HPP_INTERFACES
