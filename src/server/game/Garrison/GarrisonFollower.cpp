////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "GarrisonFollower.hpp"
#include "GarrisonMgrConstants.hpp"
#include "DB2Stores.h"
#include "GarrisonMgr.hpp"
#include "Player.h"
#include "Object.h"

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

    uint32 GarrisonFollower::GetRequiredLevelUpXP() const
    {
        if (IsNPC())
        {
            if (Level < Globals::MaxFollowerLevel)
            {
                for (uint32 l_I = 0; l_I < sGarrFollowerLevelXPStore.GetNumRows(); ++l_I)
                {
                    const GarrFollowerLevelXPEntry* l_CurrentLevelData = sGarrFollowerLevelXPStore.LookupEntry(l_I);

                    if (l_CurrentLevelData && l_CurrentLevelData->Level == Level) ///< Comparison of integers of different signs: 'const uint32' (aka 'const unsigned int') and 'const int32' (aka 'const int')
                    {
                        return l_CurrentLevelData->RequiredExperience;
                    }
                }
            }
            else if (Level == Globals::MaxFollowerLevel && Quality < ITEM_QUALITY_EPIC)
            {
                /// These values are not present in DBC
                ///  60 000 XP for ITEM_QUALITY_UNCOMMON -> ITEM_QUALITY_RARE
                /// 120 000 XP for ITEM_QUALITY_RARE     -> ITEM_QUALITY_EPIC
                return Quality == ITEM_QUALITY_UNCOMMON ? 60000 : 120000;
            }
        }
        else if (Quality < ITEM_QUALITY_EPIC)
        {
            /// These values are not present in DBC
            ///   5 000 XP for ITEM_QUALITY_UNCOMMON -> ITEM_QUALITY_RARE
            /// 200 000 XP for ITEM_QUALITY_RARE     -> ITEM_QUALITY_EPI
            return Quality == ITEM_QUALITY_UNCOMMON ? 5000 : 40000;
        }

        return 0;
    }

    /// Earn XP
    uint32 GarrisonFollower::EarnXP(uint32 p_XP, Player* p_SendUpdateTo /* = nullptr */)
    {
        if (!CanXP() || !p_XP)
            return 0;

        ByteBuffer l_Buff(150);
        Write(l_Buff);

        uint32 l_OldQuality = Quality;
        uint32 l_LearnedXP = _EarnXP(p_XP);

        if (Quality != l_OldQuality)
           const_cast<Manager*>(p_SendUpdateTo->GetGarrison())->GenerateFollowerAbilities(DatabaseID, false, true, true, true);

        if (l_LearnedXP && p_SendUpdateTo)
        {
            WorldPacket l_Update(SMSG_GARRISON_FOLLOWER_CHANGED_XP, 500);
            l_Update << uint32(l_LearnedXP);    ///< Earned XP
            l_Update << uint32(0);              ///< Type ??? if == 2 then GARRISON_FOLLOWER_XP_ADDED_SHIPMENT
            l_Update.append(l_Buff);            ///< Old Follower
            Write(l_Update);                    ///< New Follower
            p_SendUpdateTo->SendDirectMessage(&l_Update);
        }

        return l_LearnedXP;
    }

    uint32 GarrisonFollower::_EarnXP(uint32 p_XP)
    {
        uint32 l_MaxXP = GetRequiredLevelUpXP();

        if (!l_MaxXP)
            return 0;

        if (Level < Globals::MaxFollowerLevel)
        {
            if ((p_XP + this->XP) >= l_MaxXP)
            {
                uint32 l_Value = l_MaxXP - this->XP;
                this->XP = 0;
                this->Level++;

                return l_Value + _EarnXP(p_XP - l_Value);
            }
            else
            {
                this->XP += p_XP;
                return p_XP;
            }
        }
        else if (this->Level == Globals::MaxFollowerLevel && this->Quality < ITEM_QUALITY_EPIC)
        {
            if ((this->XP + p_XP) >= l_MaxXP)
            {
                uint32 l_Value = l_MaxXP - this->XP;
                this->XP = 0;
                this->Quality++;

                return l_Value + _EarnXP(p_XP - l_Value);
            }
            else
            {
                this->XP += p_XP;
                return p_XP;
            }
        }

        return 0;
    }

    GarrFollowerEntry const* GarrisonFollower::GetEntry() const
    {
        return sGarrFollowerStore.LookupEntry(FollowerID);
    }

    bool GarrisonFollower::IsShip() const
    {
        GarrFollowerEntry const* l_Entry = GetEntry();
        return l_Entry && l_Entry->Type == FollowerType::Ship;
    }

    bool GarrisonFollower::IsNPC() const
    {
        GarrFollowerEntry const* l_Entry = GetEntry();
        return l_Entry && l_Entry->Type == FollowerType::NPC;
    }

    void GarrisonFollower::SendFollowerUpdate(WorldSession* p_Session) const
    {
        WorldPacket l_Data(SMSG_GARRISON_UPDATE_FOLLOWER, 500);
        l_Data << uint32(PurchaseBuildingResults::Ok);
        Write(l_Data);
        p_Session->SendPacket(&l_Data);
    }

    void GarrisonFollower::SendFollowerUpdate(Player* p_Player) const
    {
        SendFollowerUpdate(p_Player->GetSession());
    }

    std::string GarrisonFollower::GetRealName(uint32 p_FactionIndex) const
    {
        auto l_Entry = GetEntry();

        if (!l_Entry)
            return "";

        if (ShipName.length())
            return ShipName;

        CreatureTemplate const* l_CreatureTemplate = sObjectMgr->GetCreatureTemplate(l_Entry->CreatureID[p_FactionIndex]);
        return l_CreatureTemplate ? l_CreatureTemplate->Name : "";
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Write follower into a packet
    void GarrisonFollower::Write(ByteBuffer& p_Buffer) const
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

        p_Buffer.WriteBits(this->ShipName.length(), 7);
        p_Buffer.FlushBits();
        p_Buffer.WriteString(this->ShipName);
    }
}   ///< namespace Garrison
}   ///< namespace MS
