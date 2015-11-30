////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "GarrisonFollower.hpp"
#include "GarrisonMgrConstants.hpp"
#include "DB2Stores.h"

namespace MS { namespace Garrison 
{
    /// Constructor
    GarrisonFollower::GarrisonFollower()
    {
        DatabaseID          = 0;
        FollowerID          = 0;
        Quality             = 0;
        Level               = 0;
        ItemLevelWeapon     = 0;
        ItemLevelArmor      = 0;
        XP                  = 0;
        CurrentBuildingID   = 0;
        CurrentMissionID    = 0;
        Flags               = 0;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Follower can earn XP
    bool GarrisonFollower::CanXP()
    {
        if (Level < Globals::MaxFollowerLevel)
            return true;
        else if (Level == Globals::MaxFollowerLevel && Quality < ITEM_QUALITY_EPIC)
            return true;

        return false;
    }
    /// Earn XP
    uint32 GarrisonFollower::EarnXP(uint32 p_XP)
    {
        uint32 l_AddedXP = 0;
        const GarrFollowerLevelXPEntry * l_LevelData = nullptr;

        for (uint32 l_I = 0; l_I < sGarrFollowerLevelXPStore.GetNumRows(); ++l_I)
        {
            const GarrFollowerLevelXPEntry * l_CurrentLevelData = sGarrFollowerLevelXPStore.LookupEntry(l_I);

            if (l_CurrentLevelData && l_CurrentLevelData->Level == this->Level)
            {
                l_LevelData = l_CurrentLevelData;
                break;
            }
        }

        if (!l_LevelData)
            return 0;

        if (Level < Globals::MaxFollowerLevel)
        {
            if ((p_XP + this->XP) >= l_LevelData->RequiredExperience)
            {
                uint32 l_Value = l_LevelData->RequiredExperience - this->XP;
                this->XP = 0;
                this->Level++;

                return l_Value + EarnXP(p_XP - l_Value);
            }
            else
            {
                this->XP += p_XP;
                return p_XP;
            }
        }
        else if (this->Level == Globals::MaxFollowerLevel && this->Quality < ITEM_QUALITY_EPIC)
        {
            /// These values are not present in DBC
            ///  60 000 XP for ITEM_QUALITY_UNCOMMON -> ITEM_QUALITY_RARE
            /// 120 000 XP for ITEM_QUALITY_RARE     -> ITEM_QUALITY_EPIC
            uint32 l_MaxXP = this->Quality == ITEM_QUALITY_UNCOMMON ? 60000 : 120000;

            if ((this->XP + p_XP) >= l_MaxXP)
            {
                uint32 l_Value = l_MaxXP - this->XP;
                this->XP = 0;
                this->Quality++;

                return l_Value + EarnXP(p_XP - l_Value);
            }
            else
            {
                this->XP += p_XP;
                return p_XP;
            }
        } 

        return 0;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    void GarrisonFollower::SetArmorItemLevel(uint32 p_NewIlvl)
    {
        ItemLevelArmor = p_NewIlvl;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    void GarrisonFollower::SetWeaponItemLevel(uint32 p_NewIlvl)
    {
        ItemLevelWeapon = p_NewIlvl;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// needs SendDirectMessage to player, complete class to get player and garrison
    void GarrisonFollower::UpdateFollower(Player* p_Player)
    {
        WorldPacket l_Update(SMSG_GARRISON_UPDATE_FOLLOWER, 500);
        Write(l_Update);
        p_Player->SendDirectMessage(&l_Update);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Write follower into a packet
    void GarrisonFollower::Write(ByteBuffer & p_Buffer)
    {
        p_Buffer << uint64(this->DatabaseID);
        p_Buffer << uint32(this->FollowerID);
        p_Buffer << uint32(this->Quality);
        p_Buffer << uint32(this->Level);
        p_Buffer << uint32(this->ItemLevelWeapon);
        p_Buffer << uint32(this->ItemLevelArmor);
        p_Buffer << uint32(this->XP);
        p_Buffer << uint32(this->CurrentBuildingID);
        p_Buffer << uint32(this->CurrentMissionID);

        p_Buffer << uint32(this->Abilities.size());
        p_Buffer << uint32(this->Flags);

        for (uint32 l_Y = 0; l_Y < this->Abilities.size(); ++l_Y)
            p_Buffer << int32(this->Abilities[l_Y]);
    }

}   ///< namespace Garrison
}   ///< namespace MS