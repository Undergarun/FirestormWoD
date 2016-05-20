////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SCRIPTING_INTERFACES_GUILD_HPP_INCLUDED
#define SCRIPTING_INTERFACES_GUILD_HPP_INCLUDED

#include "InterfaceBase.hpp"

/// Guild Script Interface
class GuildScript : public ScriptObjectImpl<false>
{
    protected:
        /// Constructor
        /// @p_Name : Script name
        GuildScript(const char * p_Name);

    public:
        /// Called when a member is added to the guild.
        /// @p_Guild  : Guild instance
        /// @p_Player : Added player
        /// @p_Rank   : Added player destination rank
        virtual void OnAddMember(Guild * p_Guild, Player * p_Player, uint8 & p_Rank)
        {
            UNUSED(p_Guild);
            UNUSED(p_Player);
            UNUSED(p_Rank);
        }
        /// Called when a member is removed from the guild.
        /// @p_Guild        : Guild instance
        /// @p_Player       : Removed player
        /// @p_IdDisbanding : Player is removed from a guild disbanding
        /// @p_IsKicked     : Is that removed player kicked
        virtual void OnRemoveMember(Guild * p_Guild, Player * p_Player, bool p_IdDisbanding, bool p_IsKicked)
        {
            UNUSED(p_Guild);
            UNUSED(p_Player);
            UNUSED(p_IdDisbanding);
            UNUSED(p_IsKicked);
        }

        /// Called when the guild MOTD (message of the day) changes.
        /// @p_Guild   : Guild instance
        /// @p_NewMotd : New message of the day
        virtual void OnMOTDChanged(Guild * p_Guild, const std::string & p_NewMotd)
        {
            UNUSED(p_Guild);
            UNUSED(p_NewMotd);
        }
        /// Called when the guild info is altered.
        /// @p_Guild   : Guild instance
        /// @p_NewInfo : New guild info
        virtual void OnInfoChanged(Guild * p_Guild, const std::string & p_NewInfo)
        {
            UNUSED(p_Guild);
            UNUSED(p_NewInfo);
        }

        /// Called when a guild is created.
        /// @p_Guild  : Guild instance
        /// @p_Leader : Guild leader
        /// @p_Name   : Guild Name
        virtual void OnCreate(Guild * p_Guild, Player * p_Leader, const std::string & p_Name)
        {
            UNUSED(p_Guild);
            UNUSED(p_Leader);
            UNUSED(p_Name);
        }
        /// Called when a guild is disbanded.
        /// @p_Guild : Guild instance
        virtual void OnDisband(Guild * p_Guild)
        {
            UNUSED(p_Guild);
        }

        // Called when a guild member withdraws money from a guild bank.
        /// @p_Guild    : Guild instance
        /// @p_Player   : Withdrawer player
        /// @p_Amount   : Dest gold amount
        /// @p_IsRepair : Is repair
        virtual void OnMemberWitdrawMoney(Guild * p_Guild, Player * p_Player, uint64 & p_Amount, bool p_IsRepair)
        {
            UNUSED(p_Guild);
            UNUSED(p_Player);
            UNUSED(p_Amount);
            UNUSED(p_IsRepair);
        }
        // Called when a guild member deposits money in a guild bank.
        /// @p_Guild  : Guild instance
        /// @p_Player : Depositor player
        /// @p_Amount : Dest gold amount
        virtual void OnMemberDepositMoney(Guild * p_Guild, Player * p_Player, uint64 & p_Amount)
        {
            UNUSED(p_Guild);
            UNUSED(p_Player);
            UNUSED(p_Amount);
        }

        /// Called when a guild member moves an item in a guild bank.
        /// @p_Guild         : Guild instance
        /// @p_Player        : Player item mover
        /// @p_Item          : Item instance
        /// @p_IsSrcBank     : Is from guild bank
        /// @p_SrcContainer  : Source Bag
        /// @p_SrcSlotID     : Source Bag slot ID
        /// @p_IsDestBank    : Is to guild bank
        /// @p_DestContainer : Destination Bag
        /// @p_DestSlotID    : Destination Bag slot ID
        virtual void OnItemMove(Guild * p_Guild, Player * p_Player, Item * p_Item, bool p_IsSrcBank, uint8 p_SrcContainer, uint8 p_SrcSlotID, bool p_IsDestBank, uint8 p_DestContainer, uint8 p_DestSlotID)
        {
            UNUSED(p_Guild);
            UNUSED(p_Player);
            UNUSED(p_Item);
            UNUSED(p_IsSrcBank);
            UNUSED(p_SrcContainer);
            UNUSED(p_SrcSlotID);
            UNUSED(p_IsDestBank);
            UNUSED(p_DestContainer);
            UNUSED(p_DestSlotID);
        }

        /// On Guild event
        /// @p_Guild       : Guild instance
        /// @p_EventType   : Event type
        /// @p_PlayerGUID1 : Player GUID 1
        /// @p_PlayerGUID2 : Player GUID 2
        /// @p_NewRank     : New Rank (contextual)
        virtual void OnEvent(Guild * p_Guild, uint8 p_EventType, uint32 p_PlayerGUID1, uint32 p_PlayerGUID2, uint8 p_NewRank)
        {
            UNUSED(p_Guild);
            UNUSED(p_EventType);
            UNUSED(p_PlayerGUID1);
            UNUSED(p_PlayerGUID2);
            UNUSED(p_NewRank);
        }
        /// @p_Guild          : Guild instance
        /// @p_EventType      : Event type
        /// @p_TabID          : Source tab ID
        /// @p_PlayerGUID     : Player GUID
        /// @p_ItemOrMoney    : Item entry or gold amount
        /// @p_ItemStackCount : Item stack count
        /// @p_DestTabID      : Destination tab ID
        virtual void OnBankEvent(Guild * p_Guild, uint8 p_EventType, uint8 p_TabID, uint32 p_PlayerGUID, uint64 p_ItemOrMoney, uint16 p_ItemStackCount, uint8 p_DestTabID)
        {
            UNUSED(p_Guild);
            UNUSED(p_EventType);
            UNUSED(p_TabID);
            UNUSED(p_PlayerGUID);
            UNUSED(p_ItemOrMoney);
            UNUSED(p_ItemStackCount);
            UNUSED(p_DestTabID);
        }

};

#endif  ///< SCRIPTING_INTERFACES_GUILD_HPP_INCLUDED
