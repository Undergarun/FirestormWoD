////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_FOLLOWER_HPP_GARRISON
#define GARRISON_FOLLOWER_HPP_GARRISON

#include "Common.h"
#include "ByteBuffer.h"

#include <vector>

namespace MS { namespace Garrison 
{
    /// Garrison follower
    class GarrisonFollower
    {
        public:
            /// Constructor
            GarrisonFollower();

            /// Follower can earn XP
            bool CanXP();

            /// Earn XP
            uint32 EarnXP(uint32 p_XP);

            /// Set new Armor Ilvl
            void SetArmorItemLevel(uint32 p_NewIlvl);

            /// Set new Weapon Ilvl
            void SetWeaponItemLevel(uint32 p_NewIlvl);

            /// Write follower into a packet
            void Write(ByteBuffer & p_Buffer);

            /// Send Follower Update Packet
            void UpdateFollower(Player* p_Player);

        public:
            uint32 DatabaseID;          ///< Database ID
            uint32 FollowerID;          ///< Follower ID (GarrFollower.db2)
            uint32 Quality;             ///< Follower quality
            int32  Level;               ///< Follower level
            int32  ItemLevelWeapon;     ///< Weapon item level
            int32  ItemLevelArmor;      ///< Armor item level
            uint32 XP;                  ///< Follower XP
            uint32 CurrentBuildingID;   ///< Current assigned building ID
            uint32 CurrentMissionID;    ///< Current assigned mission ID
            uint32 Flags;               ///< Follower flags

            std::vector<uint32> Abilities;  ///< Follower abilities

    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_FOLLOWER_HPP_GARRISON