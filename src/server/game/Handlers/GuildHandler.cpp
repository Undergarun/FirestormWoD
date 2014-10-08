/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "World.h"
#include "ObjectMgr.h"
#include "GuildMgr.h"
#include "Log.h"
#include "Opcodes.h"
#include "Guild.h"
#include "GossipDef.h"
#include "SocialMgr.h"

// Helper for getting guild object of session's player.
// If guild does not exist, sends error (if necessary).
inline Guild* _GetPlayerGuild(WorldSession* session, bool sendError = false)
{
    if (uint32 guildId = session->GetPlayer()->GetGuildId())    // If guild id = 0, player is not in guild
        if (Guild* guild = sGuildMgr->GetGuildById(guildId))   // Find guild by id
            return guild;
    if (sendError)
        Guild::SendCommandResult(session, GUILD_CREATE_S, ERR_GUILD_PLAYER_NOT_IN_GUILD);
    return NULL;
}

void WorldSession::HandleQueryGuildInfoOpcode(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_QUERY_GUILD_INFO");

    uint64 l_GuildGuid  = 0;
    uint64 l_PlayerGuid = 0;

    p_Packet.readPackGUID(l_GuildGuid);
    p_Packet.readPackGUID(l_PlayerGuid);

    /// If guild doesn't exist or player is not part of the guild send error
    if (Guild * l_Guild = sGuildMgr->GetGuildByGuid(l_GuildGuid))
    {
        if (l_Guild->IsMember(l_PlayerGuid))
        {
            l_Guild->HandleQuery(this);
            return;
        }
    }

    Guild::SendCommandResult(this, GUILD_CREATE_S, ERR_GUILD_PLAYER_NOT_IN_GUILD);
}

void WorldSession::HandleGuildInviteByNameOpcode(WorldPacket& p_Packet)
{
    time_t l_Now = time(NULL);
    if (l_Now - m_TimeLastGuildInviteCommand < 5)
        return;
    else
       m_TimeLastGuildInviteCommand = l_Now;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_INVITE_BY_NAME");

    uint32 l_NameLenght = 0;
    std::string l_Name = "";

    l_NameLenght    = p_Packet.ReadBits(9);
    l_Name          = p_Packet.ReadString(l_NameLenght);

    if (normalizePlayerName(l_Name))
        if (Guild* guild = _GetPlayerGuild(this, true))
            guild->HandleInviteMember(this, l_Name);
}

void WorldSession::HandleGuildOfficierRemoveMemberOpcode(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_OFFICER_REMOVE_MEMBER");

    uint64 l_Removee = 0;

    p_Packet.readPackGUID(l_Removee);

    if (Guild * l_Guild = _GetPlayerGuild(this, true))
        l_Guild->HandleRemoveMember(this, l_Removee);
}

void WorldSession::HandleGuildMasterReplaceOpcode(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_REPLACE_GUILD_MASTER");

    Guild * l_Guild = _GetPlayerGuild(this, true);

    if (!l_Guild)
        return; ///< Cheat

    uint32 l_LogoutTime = l_Guild->GetMemberLogoutTime(l_Guild->GetLeaderGUID());

    if (!l_LogoutTime)
        return;

    time_t l_Now = time(NULL);

    if (time_t(l_LogoutTime + 3 * MONTH) > l_Now)
        return; ///< Cheat

    l_Guild->SwitchGuildLeader(GetPlayer()->GetGUID());
}

void WorldSession::HandleAcceptGuildInviteOpcode(WorldPacket& /*recvPacket*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_ACCEPT_GUILD_INVITE");

    /// Player cannot be in guild
    if (!GetPlayer()->GetGuildId())
        /// Guild where player was invited must exist
        if (Guild * l_Guild = sGuildMgr->GetGuildById(GetPlayer()->GetGuildIdInvited()))
            l_Guild->HandleAcceptMember(this);
}

void WorldSession::HandleGuildDeclineInvitationsOpcode(WorldPacket& /*recvPacket*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_DECLINE");

    GetPlayer()->SetGuildIdInvited(0);
    GetPlayer()->SetInGuild(0);
}

void WorldSession::HandleGuildRosterOpcode(WorldPacket & p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_ROSTER");

    if (Guild * l_Guild = _GetPlayerGuild(this, true))
        l_Guild->HandleRoster(this);
}

void WorldSession::HandleGuildAssignRankOpcode(WorldPacket & p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_ASSIGN_MEMBER_RANK");

    uint64 l_Member = 0;
    uint32 l_RankOrder = 0;

    p_Packet.readPackGUID(l_Member);
    p_Packet >> l_RankOrder;

    if (Guild * l_Guild = _GetPlayerGuild(this, true))
        l_Guild->HandleSetMemberRank(this, l_Member, GetPlayer()->GetGUID(), l_RankOrder);
}

void WorldSession::HandleGuildLeaveOpcode(WorldPacket& /*recvPacket*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_LEAVE");

    if (Guild * l_Guild = _GetPlayerGuild(this, true))
        l_Guild->HandleLeaveMember(this);
}

void WorldSession::HandleGuildDeleteOpcode(WorldPacket& /*recvPacket*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_DELETE");

    if (Guild * l_Guild = _GetPlayerGuild(this, true))
        l_Guild->HandleDisband(this);
}

void WorldSession::HandleGuildSetGuildMasterOpcode(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_SET_GUILD_MASTER");

    std::string l_NewMasterName     = "";
    std::string l_NewMasterRealName = "";
    uint32 l_NewMasterNameLenght = 0;

    l_NewMasterNameLenght = recvPacket.ReadBits(9);

    l_NewMasterName = recvPacket.ReadString(l_NewMasterNameLenght);
    l_NewMasterRealName.resize(l_NewMasterName.size());

    size_t l_Position = l_NewMasterName.find('-');
    if (l_Position > 0)
    {
        for (size_t l_I = 0; l_I < l_NewMasterName.size(); l_I++)
            if (l_I <= l_Position)
                l_NewMasterRealName[l_I] = l_NewMasterName[l_I];

        if (normalizePlayerName(l_NewMasterName))
            if (Guild * l_Guild = _GetPlayerGuild(this, true))
                l_Guild->HandleSetLeader(this, l_NewMasterRealName);
    }
    else
    {
        if (normalizePlayerName(l_NewMasterName))
            if (Guild * l_Guild = _GetPlayerGuild(this, true))
                l_Guild->HandleSetLeader(this, l_NewMasterName);
    }
}

void WorldSession::HandleGuildUpdateMOTDTextOpcode(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_UPDATE_MOTD_TEXT");

    uint32 l_MotdTextLenght = 0;
    std::string l_MotdText = "";

    l_MotdTextLenght = p_Packet.ReadBits(10);
    l_MotdText = p_Packet.ReadString(l_MotdTextLenght);

    if (Guild * l_Guild = _GetPlayerGuild(this, true))
        l_Guild->HandleSetMOTD(this, l_MotdText);
}

void WorldSession::HandleShiftRanks(WorldPacket & p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_SHIFT_RANK");

    uint32 l_RankOrder = 0;
    bool l_ShiftUp = false;

    p_Packet >> l_RankOrder;
    l_ShiftUp = p_Packet.ReadBit();

    if (Guild * l_Guild = _GetPlayerGuild(this, true))
        l_Guild->HandleSwapRanks(this, l_RankOrder, l_ShiftUp);
}

void WorldSession::HandleGuildSetMemberNoteOpcode(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_SET_MEMBER_NOTE");

    uint64 l_NoteeGUID = 0;
    uint32 l_NoteLenght = 0;
    bool l_IsPublic = false;

    std::string l_Note = "";

    l_NoteLenght    = p_Packet.ReadBits(8);
    l_IsPublic      = p_Packet.ReadBit();

    l_Note = p_Packet.ReadString(l_NoteLenght);

    if (Guild * l_Guild = _GetPlayerGuild(this, true))
        l_Guild->HandleSetMemberNote(this, l_Note, l_NoteeGUID, l_IsPublic);
}

void WorldSession::HandleGuildGetRanksOpcode(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_GET_RANKS");

    uint64 l_GuildGUID = 0;

    p_Packet.readPackGUID(l_GuildGUID);

    if (Guild * l_Guild = sGuildMgr->GetGuildByGuid(l_GuildGUID))
        if (l_Guild->IsMember(m_Player->GetGUID()))
            l_Guild->HandleGuildRanks(this);
}

void WorldSession::HandleGuildAddRankOpcode(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_ADD_RANK");

    uint32 l_RankOrder = 0;
    uint32 l_NameLength = 0;
    std::string l_Name = "";

    l_NameLength = p_Packet.ReadBits(7);

    p_Packet >> l_RankOrder;

    l_Name = p_Packet.ReadString(l_NameLength);

    if (Guild * l_Guild = _GetPlayerGuild(this, true))
        l_Guild->HandleAddNewRank(this, l_Name); //, rankId);
}

void WorldSession::HandleGuildDeleteRankOpcode(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_DELETE_RANK");

    uint32 l_RankOrder = 0;

    p_Packet >> l_RankOrder;

    if (Guild * l_Guild = _GetPlayerGuild(this, true))
        l_Guild->HandleRemoveRank(this, l_RankOrder);
}

void WorldSession::HandleGuildUpdateInfoTextOpcode(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_UPDATE_INFO_TEXT");

    uint32 length = recvPacket.ReadBits(12);

    recvPacket.FlushBits();

    std::string info = recvPacket.ReadString(length / 2);

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleSetInfo(this, info);
}

void WorldSession::HandlePlayerSaveGuildEmblemOpcode(WorldPacket & p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_PLAYER_SAVE_GUILD_EMBLEM");

    uint64 l_Vendor = 0;

    EmblemInfo l_EmblemInfo;
    l_EmblemInfo.ReadPacket(p_Packet);

    Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWith(l_Vendor, UNIT_NPC_FLAG_TABARDDESIGNER);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandlePlayerSaveGuildEmblemOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_Vendor)));
        return;
    }

    /// Remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    if (Guild* guild = _GetPlayerGuild(this))
        guild->HandleSetEmblem(this, l_EmblemInfo);
    else
        /// "You are not part of a guild!";
        Guild::SendSaveEmblemResult(this, ERR_GUILDEMBLEM_NOGUILD);
}

void WorldSession::HandleGuildEventLogQueryOpcode(WorldPacket& /* recvPacket */)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_EVENT_LOG_QUERY)");

    if (Guild * l_Guild = _GetPlayerGuild(this))
        l_Guild->SendEventLog(this);
}

void WorldSession::HandleGuildBankRemainingWithdrawMoneyQueryOpcode(WorldPacket& /* recvData */)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_MONEY_WITHDRAWN_QUERY)");

    if (Guild* guild = _GetPlayerGuild(this))
        guild->SendMoneyInfo(this);
}

void WorldSession::HandleGuildPermissionsQueryOpcode(WorldPacket& /* recvData */)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_PERMISSIONS_QUERY)");

    if (Guild * l_Guild = _GetPlayerGuild(this))
        l_Guild->SendPermissions(this);
}

/// Called when clicking on Guild bank gameobject
void WorldSession::HandleGuildBankActivate(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_ACTIVATE)");

    uint64 l_Banker = 0;
    bool l_FullUpdate = false;

    p_Packet.readPackGUID(l_Banker);
    l_FullUpdate = p_Packet.ReadBit();  ///< 0 = only slots updated in last operation are shown. 1 = all slots updated

    if (GetPlayer()->GetGameObjectIfCanInteractWith(l_Banker, GAMEOBJECT_TYPE_GUILD_BANK))
    {
        if (Guild* l_Guild = _GetPlayerGuild(this))
            l_Guild->SendBankList(this, 0, true, true);
        else
            Guild::SendCommandResult(this, GUILD_BANK, ERR_GUILD_PLAYER_NOT_IN_GUILD);
    }
}

// Called when opening guild bank tab only (first one)
void WorldSession::HandleGuildBankQueryTab(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_QUERY_TAB)");

    uint64 l_Banker = 0;
    uint8 l_Tab = 0;
    bool l_FullUpdate = false;;

    p_Packet.readPackGUID(l_Banker);
    p_Packet >> l_Tab;
    l_FullUpdate = p_Packet.ReadBit();      ///< 0 = only slots updated in last operation are shown. 1 = all slots updated

    if (GetPlayer()->GetGameObjectIfCanInteractWith(l_Banker, GAMEOBJECT_TYPE_GUILD_BANK))
    {
        if (Guild * l_Guild = _GetPlayerGuild(this))
        {
            l_Guild->SendBankList(this, l_Tab, true, false);
            l_Guild->SendMoneyInfo(this);
        }
    }
}

void WorldSession::HandleGuildBankDepositMoney(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_DEPOSIT_MONEY)");

    uint64 l_Banker = 0;
    uint64 l_Money = 0;

    p_Packet.readPackGUID(l_Banker);
    p_Packet >> l_Money;

    if (GetPlayer()->GetGameObjectIfCanInteractWith(l_Banker, GAMEOBJECT_TYPE_GUILD_BANK))
    {
        if (l_Money && GetPlayer()->HasEnoughMoney(l_Money))
        {
            if (Guild * l_Guild = _GetPlayerGuild(this))
            {
                uint64 l_Amount = l_Guild->GetBankMoney();

                if ((l_Amount + l_Money) > MAX_MONEY_AMOUNT)
                    l_Guild->SendCommandResult(this, GUILD_BANK, ERR_GUILD_TOO_MUCH_MONEY);
                else
                    l_Guild->HandleMemberDepositMoney(this, l_Money);
            }
        }
    }
}

void WorldSession::HandleGuildBankWithdrawMoney(WorldPacket & p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_WITHDRAW_MONEY)");

    uint64 l_Banker;
    uint64 l_Money;

    p_Packet.readPackGUID(l_Banker);
    p_Packet >> l_Money;

    if (l_Money)
    {
        if (GetPlayer()->GetGameObjectIfCanInteractWith(l_Banker, GAMEOBJECT_TYPE_GUILD_BANK))
        {
            if (Guild * l_Guild = _GetPlayerGuild(this))
                l_Guild->HandleMemberWithdrawMoney(this, l_Money);
        }
    }
}

void WorldSession::HandleGuildBankSwapItems(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_SWAP_ITEMS)");

    uint64 l_Banker = 0;

    uint32 l_ItemID        = 0;
    uint32 l_ItemID1       = 0;
    uint32 l_BankItemCount = 0;
    uint32 l_StackCount    = 0;

    uint8 l_BankTab           = 0;
    uint8 l_BankSlot          = 0;
    uint8 l_BankTab1          = 0;
    uint8 l_BankSlot1         = 0;
    uint8 l_ContainerSlot     = 0;
    uint8 l_ContainerItemSlot = 0;
    uint8 l_ToSlot            = 0;

    bool l_BankOnly  = false;
    bool l_AutoStore = false;

    p_Packet.readPackGUID(l_Banker);
    p_Packet >> l_BankTab;
    p_Packet >> l_BankSlot;
    p_Packet >> l_ItemID;
    p_Packet >> l_BankTab1;
    p_Packet >> l_BankSlot1;
    p_Packet >> l_ItemID1;
    p_Packet >> l_BankItemCount;
    p_Packet >> l_ContainerSlot;
    p_Packet >> l_ContainerItemSlot;
    p_Packet >> l_ToSlot;
    p_Packet >> l_StackCount;

    l_BankOnly = p_Packet.ReadBit();
    l_AutoStore = p_Packet.ReadBit();

    if (!GetPlayer()->GetGameObjectIfCanInteractWith(l_Banker, GAMEOBJECT_TYPE_GUILD_BANK))
        return;

    Guild * l_Guild = _GetPlayerGuild(this);

    if (!l_Guild)
        return;

    if (l_BankOnly)
        l_Guild->SwapItems(GetPlayer(), l_BankTab1, l_BankSlot, l_BankTab, l_BankSlot1, l_StackCount);
    else if (l_AutoStore)
        l_Guild->AutoStoreItemInInventory(GetPlayer(), l_BankTab, l_BankSlot1, l_BankItemCount);
    else
    {
        /// Player <-> Bank
        /// Allow to work with inventory only
        if (!Player::IsInventoryPos(l_ContainerSlot, l_ContainerItemSlot) && !(l_ContainerSlot == NULL_BAG && l_ContainerItemSlot == NULL_SLOT))
            GetPlayer()->SendEquipError(EQUIP_ERR_INTERNAL_BAG_ERROR, NULL);
        else
            l_Guild->SwapItemsWithInventory(GetPlayer(), l_ToSlot, l_BankTab, l_BankSlot1, l_ContainerSlot, l_ContainerItemSlot, l_StackCount);
    }
}

void WorldSession::HandleGuildBankBuyTab(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_BUY_TAB)");

    uint64 l_Banker = 0;
    uint8 l_BankTab = 0;

    p_Packet.readPackGUID(l_Banker);
    p_Packet >> l_BankTab;

    /// Only for SPELL_EFFECT_UNLOCK_GUILD_VAULT_TAB, this prevent cheating
    if (l_BankTab > 5)
        return;

    if (!l_Banker || GetPlayer()->GetGameObjectIfCanInteractWith(l_Banker, GAMEOBJECT_TYPE_GUILD_BANK))
        if (Guild * l_Guild = _GetPlayerGuild(this))
            l_Guild->HandleBuyBankTab(this, l_BankTab);
}

void WorldSession::HandleGuildBankUpdateTab(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_UPDATE_TAB)");

    uint64 l_Banker = 0;
    uint32 l_NameLenght = 0;
    uint32 l_IconLenght = 0;
    uint8 l_BankTab;
    std::string l_Name;
    std::string l_Icon;

    p_Packet.readPackGUID(l_Banker);
    p_Packet >> l_BankTab;

    l_NameLenght = p_Packet.ReadBits(7);
    l_IconLenght = p_Packet.ReadBits(9);

    l_Name = p_Packet.ReadString(l_NameLenght);
    l_Icon = p_Packet.ReadString(l_IconLenght);

    if (!l_Name.empty() && !l_Icon.empty())
    {
        if (GetPlayer()->GetGameObjectIfCanInteractWith(l_Banker, GAMEOBJECT_TYPE_GUILD_BANK))
        {
            if (Guild * l_Guild = _GetPlayerGuild(this))
                l_Guild->HandleSetBankTabInfo(this, l_BankTab, l_Name, l_Icon);
        }
    }
}

void WorldSession::HandleGuildBankLogQuery(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_LOG_QUERY)");

    uint32 l_TabID = 0;

    p_Packet >> l_TabID;

    if (Guild * l_Guild = _GetPlayerGuild(this))
        l_Guild->SendBankLog(this, l_TabID);
}

void WorldSession::HandleQueryGuildBankTextQuery(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_BANK_TEXT_QUERY");

    uint32 l_Tab = 0;

    p_Packet >> l_Tab;

    if (Guild * l_Guild = _GetPlayerGuild(this))
        l_Guild->SendBankTabText(this, l_Tab);
}

void WorldSession::HandleSetGuildBankTabText(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_BANK_SET_TAB_TEXT");

    uint32 l_Tab = 0;
    uint32 l_TabTextLenght = 0;
    std::string l_TabText = "";

    p_Packet >> l_Tab;
    l_TabTextLenght = p_Packet.ReadBits(14);
    l_TabText       = p_Packet.ReadString(l_TabTextLenght);

    if (Guild* guild = _GetPlayerGuild(this))
        guild->SetBankTabText(l_Tab, l_TabText);
}

void WorldSession::HandleGuildSetRankPermissionsOpcode(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_SET_RANK_PERMISSIONS");

    Guild * l_Guild = _GetPlayerGuild(this, true);

    if (!l_Guild)
    {
        p_Packet.rfinish();
        return;
    }

    uint32 l_RankOrder          = 0;
    uint32 l_RankID             = 0;
    uint32 l_OldFlags           = 0;
    uint32 l_Flags              = 0;
    uint32 l_RankNameLenght     = 0;
    uint32 l_WithdrawGoldLimit  = 0;

    std::string l_RankName = "";

    p_Packet >> l_RankID;
    p_Packet >> l_RankOrder;
    p_Packet >> l_Flags;
    p_Packet >> l_OldFlags;
    p_Packet >> l_WithdrawGoldLimit;

    GuildBankRightsAndSlotsVec l_RightsAndSlots(GUILD_BANK_MAX_TABS);
    for (uint8 l_TabID = 0; l_TabID < GUILD_BANK_MAX_TABS; ++l_TabID)
    {
        uint32 l_TabFlags;
        uint32 l_TabWithdrawItemLimit;

        p_Packet >> l_TabFlags;
        p_Packet >> l_TabWithdrawItemLimit;

        l_RightsAndSlots[l_TabID] = GuildBankRightsAndSlots(uint8(l_TabFlags), l_TabWithdrawItemLimit);
    }

    l_RankNameLenght    = p_Packet.ReadBits(7);
    l_RankName          = p_Packet.ReadString(l_RankNameLenght);

    l_Guild->HandleSetRankInfo(this, l_RankID, l_RankName, l_Flags, l_WithdrawGoldLimit, l_RightsAndSlots);
}

void WorldSession::HandleRequestGuildPartyState(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_REQUEST_GUILD_PARTY_STATE");

    uint64 l_GuildGUID = 0;

    p_Packet.readPackGUID(l_GuildGUID);

    if (Guild * l_Guild = sGuildMgr->GetGuildByGuid(l_GuildGUID))
        l_Guild->HandleGuildPartyRequest(this);
}

void WorldSession::HandleAutoDeclineGuildInvites(WorldPacket& recvPacket)
{
    bool enable;
    enable = recvPacket.ReadBit();

    GetPlayer()->ApplyModFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_AUTO_DECLINE_GUILD, enable);
}

void WorldSession::HandleRequestGuildRewardsListOpcode(WorldPacket& p_Packet)
{
    uint32 l_CurrentVersion = 0;

    p_Packet >> l_CurrentVersion;

    if (Guild * l_Guild = sGuildMgr->GetGuildById(m_Player->GetGuildId()))
    {
        std::vector<GuildReward> const& l_Rewards = sGuildMgr->GetGuildRewards();

        WorldPacket l_Data(SMSG_GUILD_REWARDS_LIST);

        l_Data << uint32(time(NULL));
        l_Data << uint32(l_Rewards.size());

        for (uint32 i = 0; i < l_Rewards.size(); i++)
        {
            l_Data << uint32(l_Rewards[i].Entry);
            l_Data << uint32(l_Rewards[i].AchievementId > 0 ? 1 : 0);
            l_Data << uint32(l_Rewards[i].Racemask);
            l_Data << uint32(0);
            l_Data << uint32(l_Rewards[i].Standing);
            l_Data << uint64(l_Rewards[i].Price);

            if (l_Rewards[i].AchievementId)
                l_Data << uint32(l_Rewards[i].AchievementId);
        }

        l_Data.FlushBits();

        SendPacket(&l_Data);
    }
}

void WorldSession::HandleGuildQueryNewsOpcode(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_QUERY_NEWS");

    uint64 l_GuildGUID = 0;

    p_Packet.readPackGUID(l_GuildGUID);

    if (Guild * l_Guild = sGuildMgr->GetGuildByGuid(l_GuildGUID))
    {
        if (l_Guild->IsMember(m_Player->GetGUID()))
        {
            WorldPacket l_Data;
            l_Guild->GetNewsLog().BuildNewsData(l_Data);

            SendPacket(&l_Data);
        }
    }
}

void WorldSession::HandleGuildNewsUpdateStickyOpcode(WorldPacket& p_Packet)
{
    uint64 l_GuildGUID  = 0;
    uint32 l_NewsID     = 0;
    bool l_Sticky = false;

    p_Packet.readPackGUID(l_GuildGUID);
    p_Packet >> l_NewsID;

    l_Sticky = p_Packet.ReadBit();

    if (Guild * l_Guild = sGuildMgr->GetGuildById(m_Player->GetGuildId()))
    {
        if (GuildNewsEntry * l_NewsEntry = l_Guild->GetNewsLog().GetNewsById(l_NewsID))
        {
            if (l_Sticky)
                l_NewsEntry->Flags |= 1;
            else
                l_NewsEntry->Flags &= ~1;

            WorldPacket l_Data;
            l_Guild->GetNewsLog().BuildNewsData(l_NewsID, *l_NewsEntry, l_Data);

            SendPacket(&l_Data);
        }
    }
}

void WorldSession::HandleGuildChallengeUpdateRequest(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_CHALLENGE_UPDATE_REQUEST");

    GuildChallengeRewardData const& reward = sObjectMgr->GetGuildChallengeRewardData();

    WorldPacket data(SMSG_GUILD_CHALLENGE_UPDATED, 5*6*4);

    for (uint8 i = 0; i < CHALLENGE_MAX; i++)
        data << uint32(reward[i].ChallengeCount);

    for (uint8 i = 0; i < CHALLENGE_MAX; i++)
        data << uint32(0);                      // Current count : @TODO : New system ! Guild challenge

    for (uint8 i = 0; i < CHALLENGE_MAX; i++)
        data << uint32(reward[i].Experience);

    for (uint8 i = 0; i < CHALLENGE_MAX; i++)
        data << uint32(reward[i].Gold2);

    for (uint8 i = 0; i < CHALLENGE_MAX; i++)
        data << uint32(reward[i].Gold);

    SendPacket(&data);
}

void WorldSession::HandleGuildRequestGuildRecipes(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_REQUEST_CHALLENGE_UPDATE");

    uint64 l_GuildGUID = 0;

    p_Packet.readPackGUID(l_GuildGUID);

    if (Guild * l_Guild = sGuildMgr->GetGuildByGuid(l_GuildGUID))
    {
        if (l_Guild->IsMember(m_Player->GetGUID()))
            l_Guild->SendGuildRecipes(this);
    }
}