////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
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
            virtual void OnQuestStarted(Player * p_Owner, const Quest * p_Quest) = 0;
            /// When the garrison owner reward a quest
            /// @p_Owner : Garrison owner
            /// @p_Quest : Rewarded quest
            virtual void OnQuestReward(Player * p_Owner, const Quest * p_Quest) = 0;
            /// When the garrison owner abandon a quest
            /// @p_Owner : Garrison owner
            /// @p_Quest : Abandoned quest
            virtual void OnQuestAbandon(Player * p_Owner, const Quest * p_Quest) = 0;

            /// Get phase mask
            /// @p_Owner : Garrison owner
            virtual uint32 GetPhaseMask(Player * p_Owner) = 0;

            /// Owner can use the garrison cache ?
            /// @p_Owner : Garrison owner
            virtual bool CanUseGarrisonCache(Player * p_Owner) = 0;

            /// When a mission start
            /// @p_Owner     : Garrison owner
            /// @p_MissionID : Started mission ID
            /// @p_Followers : Followers on the mission
            virtual void OnMissionStart(Player * p_Owner, uint32 p_MissionID, std::vector<uint32> p_Followers) = 0;
            /// When a construction start
            /// @p_Owner      : Garrison owner
            /// @p_BuildingID : Purchased building ID
            virtual void OnPurchaseBuilding(Player * p_Owner, uint32 p_BuildingID) = 0;

    };

}   ///< namespace Interfaces
}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_SITE_HPP_INTERFACES