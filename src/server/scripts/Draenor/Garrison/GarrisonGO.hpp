////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_GO_HPP_GARRISON
#define GARRISON_GO_HPP_GARRISON

#include "GarrisonScriptData.hpp"
#include "GarrisonMgr.hpp"
#include "GameObjectAI.h"

namespace MS { namespace Garrison 
{
    std::vector<uint32> g_HerbEntries
    {
        Items::ItemFireWeed,
        Items::ItemFrostWeed,
        Items::ItemGorgrondFlytrap,
        Items::ItemNagrandArrowbloom,
        Items::ItemTaladorOrchid,
        Items::ItemStarflower
    };

    /// Garrison cache generic script
    class go_garrison_cache : public GameObjectScript
    {
        public:
            /// Constructor
            go_garrison_cache();

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_GameObject : Target GameObject instance
            bool OnGossipHello(Player * p_Player, GameObject * p_GameObject);

    };

    /// Garrison cache generic script
    class go_garrison_outhouse : public GameObjectScript
    {
        public:
            /// Constructor
            go_garrison_outhouse();

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_GameObject : Target GameObject instance
            bool OnGossipHello(Player * p_Player, GameObject * p_GameObject);

    };

    /// Garrison shipment container generic script
    class go_garrison_shipment_container : public GameObjectScript
    {
        public:
            /// Constructor
            go_garrison_shipment_container();

            /// Called when a GameObjectAI object is needed for the GameObject.
            /// @p_GameObject : GameObject instance
            GameObjectAI* GetAI(GameObject * p_GameObject) const override;

            struct go_garrison_shipment_containerAI : public GameObjectAI
            {
                /// Constructor
                go_garrison_shipment_containerAI(GameObject * p_GameObject);

                /// Called when a player opens a gossip dialog with the GameObject.
                /// @p_Player     : Source player instance
                bool GossipHello(Player * p_Player) override;
            };

    };

    /// Garrison herb generic script
    class go_garrison_herb : public GameObjectScript
    {
        public:
            /// Constructor
            go_garrison_herb();

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_GameObject : Target GameObject instance
            bool OnGossipHello(Player * p_Player, GameObject * p_GameObject);

    };

    //////////////////////////////////////////////////////////////////////////
    /// 234186 - Iron Trap                                                 ///
    //////////////////////////////////////////////////////////////////////////

    /// Garrison shipment container generic script
    class gob_IronTrap_Garrison : public GameObjectScript
    {
        public:
            /// Constructor
            gob_IronTrap_Garrison();

            /// Called when a GameObjectAI object is needed for the GameObject.
            /// @p_GameObject : GameObject instance
            GameObjectAI* GetAI(GameObject* p_Gob) const override;

            struct gob_IronTrap_GarrisonAI : public GameObjectAI
            {
                /// Constructor
                gob_IronTrap_GarrisonAI(GameObject* p_Gob);

                uint32 m_UpdateTimer;

                virtual void Reset() override;

                virtual void UpdateAI(uint32 p_Diff) override;
            };
    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_GO_HPP_GARRISON