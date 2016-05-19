////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef GARRISON_FOLLOWER_HPP_GARRISON
# define GARRISON_FOLLOWER_HPP_GARRISON

#include "Common.h"
#include "ByteBuffer.h"


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
            uint32 EarnXP(uint32 p_XP, Player* p_SendUpdateTo = nullptr);

            /// Set new Armor Ilvl
            void SetArmorItemLevel(uint32 p_NewIlvl);

            /// Set new Weapon Ilvl
            void SetWeaponItemLevel(uint32 p_NewIlvl);

            /// Write follower into a packet
            void Write(ByteBuffer & p_Buffer) const;

            /// Get the DBC Entry
            GarrFollowerEntry const* GetEntry() const;

            /// Returns true if follower is a ship
            bool IsShip() const;

            /// Returns true if follower is a npc
            bool IsNPC() const;

            /// Sends follower update
            void SendFollowerUpdate(WorldSession* p_Session) const;

            /// Sends follower update
            void SendFollowerUpdate(Player* p_Player) const;

            /// Gets the real name of the follower
            std::string GetRealName(uint32 p_FactionIndex) const;

            /// Returns the required amount of xp for next level
            uint32 GetRequiredLevelUpXP() const;

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
            std::string ShipName;       ///< Ship Name - empty for default

            std::vector<uint32> Abilities;  ///< Follower abilities

        private:
            uint32 _EarnXP(uint32 p_XP);

    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_FOLLOWER_HPP_GARRISON
