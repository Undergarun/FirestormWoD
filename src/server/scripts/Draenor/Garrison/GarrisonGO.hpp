////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
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

            struct ShipmentCurrency
            {
                uint32 CurrencyID     = 0;
                uint32 CurrencyAmount = 0;
            };

            struct ItemReward
            {
                uint32 ItemID;
                uint32 ItemCount;

                ItemReward()
                {
                    ItemID = 0;
                    ItemCount = 0;
                }
            };

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

    /// Garrison deposit generic script
    class go_garrison_deposit : public GameObjectScript
    {
        public:
            /// Constructor
            go_garrison_deposit();

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_GameObject : Target GameObject instance
            bool OnGossipHello(Player* p_Player, GameObject* p_GameObject);

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

    //////////////////////////////////////////////////////////////////////////
    /// 233604 - Small Timber                                              ///
    //////////////////////////////////////////////////////////////////////////

    /// Garrison shipment container generic script
    class go_garrison_timber : public GameObjectScript
    {
        public:
            /// Constructor
            go_garrison_timber();

            enum eDatas
            {
                ActionGossip = 1,
                ChopCount,
                AnimTimer,
                PlayerGuid
            };

            /// Called when a GameObjectAI object is needed for the GameObject.
            /// @p_GameObject : GameObject instance
            GameObjectAI* GetAI(GameObject * p_GameObject) const override;

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_GameObject : Target GameObject instance
            bool OnGossipHello(Player * p_Player, GameObject * p_GameObject) override;

            struct go_garrison_timberAI : public GameObjectAI
            {
                /// Constructor
                go_garrison_timberAI(GameObject * p_GameObject);

                uint32 m_AnimTimer    = 0;
                uint32 m_RefillTimer  = 0;
                uint32 m_RespawnTimer = 0;
                uint32 m_ChopCount    = 0;
                uint64 m_PlayerGuid   = 0;
                std::map<uint32, uint32> m_TimberDisplayIDs;

                virtual void DoAction(const int32 p_Action) override;

                virtual void SetData64(uint32 p_Id, uint64 p_Value) override;

                /// Called when a player opens a gossip dialog with the GameObject.
                /// @p_Player     : Source player instance
                virtual void UpdateAI(uint32 p_Diff) override;
            };
    };

    //////////////////////////////////////////////////////////////////////////
    /// 236911, 236774, 236906, 236910, 236912, 236765                     ///
    /// Deactivated Portals from Mage Tower                                ///
    //////////////////////////////////////////////////////////////////////////

    class go_garrison_deactivated_mage_portal : public GameObjectScript
    {
        public:
            /// Constructor
            go_garrison_deactivated_mage_portal();
            
            virtual bool OnGameObjectSpellCasterUse(const GameObject* p_GameObject, Player* p_User) const override;
    };


    //////////////////////////////////////////////////////////////////////////
    /// 237335, 237132                                                     ///
    //////////////////////////////////////////////////////////////////////////

    /// Garrison Enchanter's Study receptacle
    class go_garrison_essence_font : public GameObjectScript
    {
        public:
            /// Constructor
            go_garrison_essence_font();

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_GameObject : Target GameObject instance
            bool OnGossipHello(Player* p_Player, GameObject* p_GameObject);

    };


    //////////////////////////////////////////////////////////////////////////
    /// 192628                                                             ///
    //////////////////////////////////////////////////////////////////////////

    /// Garrison Enchanter's Study receptacle
    class go_garrison_anvil : public GameObjectScript
    {
        public:
            /// Constructor
            go_garrison_anvil();

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_GameObject : Target GameObject instance
            virtual void OnGameObjectStateChanged(const GameObject* p_GameObject, uint32 p_State) override;

    };
}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_GO_HPP_GARRISON