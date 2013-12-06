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

void WorldSession::HandleGuildQueryOpcode(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_QUERY");

    ObjectGuid guildGuid;
    ObjectGuid playerGuid;

    playerGuid[2] = recvPacket.ReadBit();
    guildGuid[7] = recvPacket.ReadBit();
    playerGuid[6] = recvPacket.ReadBit();
    playerGuid[1] = recvPacket.ReadBit();
    guildGuid[5] = recvPacket.ReadBit();
    guildGuid[4] = recvPacket.ReadBit();
    guildGuid[6] = recvPacket.ReadBit();
    guildGuid[1] = recvPacket.ReadBit();
    playerGuid[3] = recvPacket.ReadBit();
    guildGuid[2] = recvPacket.ReadBit();
    playerGuid[0] = recvPacket.ReadBit();
    guildGuid[0] = recvPacket.ReadBit();
    playerGuid[4] = recvPacket.ReadBit();
    guildGuid[3] = recvPacket.ReadBit();
    playerGuid[5] = recvPacket.ReadBit();
    playerGuid[7] = recvPacket.ReadBit();


    recvPacket.ReadByteSeq(playerGuid[0]);
    recvPacket.ReadByteSeq(playerGuid[2]);
    recvPacket.ReadByteSeq(playerGuid[7]);
    recvPacket.ReadByteSeq(guildGuid[7]);
    recvPacket.ReadByteSeq(guildGuid[2]);
    recvPacket.ReadByteSeq(playerGuid[3]);
    recvPacket.ReadByteSeq(guildGuid[3]);
    recvPacket.ReadByteSeq(guildGuid[6]);
    recvPacket.ReadByteSeq(guildGuid[0]);
    recvPacket.ReadByteSeq(playerGuid[4]);
    recvPacket.ReadByteSeq(playerGuid[6]);
    recvPacket.ReadByteSeq(guildGuid[1]);
    recvPacket.ReadByteSeq(guildGuid[5]);
    recvPacket.ReadByteSeq(guildGuid[4]);
    recvPacket.ReadByteSeq(playerGuid[5]);
    recvPacket.ReadByteSeq(playerGuid[1]);

    // If guild doesn't exist or player is not part of the guild send error
    if (Guild* guild = sGuildMgr->GetGuildByGuid(guildGuid))
        if (guild->IsMember(playerGuid))
        {
            guild->HandleQuery(this);
            return;
        }

    Guild::SendCommandResult(this, GUILD_CREATE_S, ERR_GUILD_PLAYER_NOT_IN_GUILD);
}

void WorldSession::HandleGuildInviteOpcode(WorldPacket& recvPacket)
{
    time_t now = time(NULL);
    if (now - timeLastGuildInviteCommand < 5)
        return;
    else
       timeLastGuildInviteCommand = now;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_INVITE");

    uint32 nameLength = recvPacket.ReadBits(8);
    nameLength *= 2;

    bool pair = recvPacket.ReadBit();

    if (pair)
        nameLength++;

    recvPacket.FlushBits();
    std::string invitedName = recvPacket.ReadString(nameLength);

    if (normalizePlayerName(invitedName))
        if (Guild* guild = _GetPlayerGuild(this, true))
            guild->HandleInviteMember(this, invitedName);
}

void WorldSession::HandleGuildRemoveOpcode(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_REMOVE");

    ObjectGuid playerGuid;

    uint8 bitOrder[8] = { 7, 0, 6, 3, 1, 2, 4, 5 };
    recvPacket.ReadBitInOrder(playerGuid, bitOrder);

    recvPacket.FlushBits();

    uint8 byteOrder[8] = { 7, 4, 3, 0, 2, 6, 5, 1 };
    recvPacket.ReadBytesSeq(playerGuid, byteOrder);

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleRemoveMember(this, playerGuid);
}

void WorldSession::HandleGuildMasterReplaceOpcode(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_REPLACE_GUILD_MASTER");

    Guild* guild = _GetPlayerGuild(this, true);

    if (!guild)
        return; // Cheat

    uint32 logoutTime = guild->GetMemberLogoutTime(guild->GetLeaderGUID());

    if (!logoutTime)
        return;

    time_t now = time(NULL);

    if (time_t(logoutTime + 3 * MONTH) > now)
        return; // Cheat

    guild->SwitchGuildLeader(GetPlayer()->GetGUID());
}

void WorldSession::HandleGuildAcceptOpcode(WorldPacket& /*recvPacket*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_ACCEPT");
    // Player cannot be in guild
    if (!GetPlayer()->GetGuildId())
        // Guild where player was invited must exist
        if (Guild* guild = sGuildMgr->GetGuildById(GetPlayer()->GetGuildIdInvited()))
            guild->HandleAcceptMember(this);
}

void WorldSession::HandleGuildDeclineOpcode(WorldPacket& /*recvPacket*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_DECLINE");

    GetPlayer()->SetGuildIdInvited(0);
    GetPlayer()->SetInGuild(0);
}

void WorldSession::HandleGuildRosterOpcode(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_ROSTER");

    recvPacket.rfinish();

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleRoster(this);
}

void WorldSession::HandleGuildAssignRankOpcode(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_ASSIGN_MEMBER_RANK");

    ObjectGuid targetGuid;

    uint32 rankId;
    recvPacket >> rankId;

    uint8 bitOrder[8] = { 7, 2, 5, 6, 0, 4, 3, 1 };
    recvPacket.ReadBitInOrder(targetGuid, bitOrder);

    recvPacket.FlushBits();

    uint8 byteOrder[8] = { 0, 6, 5, 2, 4, 1, 3, 7 };
    recvPacket.ReadBytesSeq(targetGuid, byteOrder);

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleSetMemberRank(this, targetGuid, GetPlayer()->GetGUID(), rankId);
}

void WorldSession::HandleGuildLeaveOpcode(WorldPacket& /*recvPacket*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_LEAVE");

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleLeaveMember(this);
}

void WorldSession::HandleGuildDisbandOpcode(WorldPacket& /*recvPacket*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_DISBAND");

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleDisband(this);
}

void WorldSession::HandleGuildLeaderOpcode(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_LEADER");

    std::string name;
    std::string realName;
    uint32 len = recvPacket.ReadBits(8);
    len *= 2;

    bool pair = recvPacket.ReadBit();
    if (pair)
        len++;

    recvPacket.FlushBits();

    name = recvPacket.ReadString(len);
    realName.resize(name.size());

    size_t pos = name.find('-');
    if (pos > 0)
    {
        for (size_t i = 0; i < name.size(); i++)
            if (i <= pos)
                realName[i] = name[i];

        if (normalizePlayerName(name))
            if (Guild* guild = _GetPlayerGuild(this, true))
                guild->HandleSetLeader(this, realName);
    }
    else
    {
        if (normalizePlayerName(name))
            if (Guild* guild = _GetPlayerGuild(this, true))
                guild->HandleSetLeader(this, name);
    }
}

void WorldSession::HandleGuildMOTDOpcode(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_MOTD");

    uint32 motdLength = recvPacket.ReadBits(10);
    recvPacket.FlushBits();
    std::string motd = recvPacket.ReadString(motdLength);

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleSetMOTD(this, motd);
}

void WorldSession::HandleSwapRanks(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_SWITCH_RANK");

    uint32 id = 0;
    bool up = false;

    recvPacket >> id;
    up = recvPacket.ReadBit();
    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleSwapRanks(this, id, up);
}

void WorldSession::HandleGuildSetNoteOpcode(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_SET_NOTE");

    ObjectGuid playerGuid;
    playerGuid[6] = recvPacket.ReadBit();
    playerGuid[5] = recvPacket.ReadBit();
    playerGuid[0] = recvPacket.ReadBit();
    playerGuid[1] = recvPacket.ReadBit();
    playerGuid[2] = recvPacket.ReadBit();
    playerGuid[3] = recvPacket.ReadBit();
    bool type = recvPacket.ReadBit();      // 0 == Officer, 1 == Public
    playerGuid[4] = recvPacket.ReadBit();
    uint32 noteLength = recvPacket.ReadBits(8);
    playerGuid[7] = recvPacket.ReadBit();

    recvPacket.FlushBits();

    recvPacket.ReadByteSeq(playerGuid[5]);
    recvPacket.ReadByteSeq(playerGuid[7]);
    recvPacket.ReadByteSeq(playerGuid[2]);
    recvPacket.ReadByteSeq(playerGuid[3]);
    recvPacket.ReadByteSeq(playerGuid[4]);

    std::string note = recvPacket.ReadString(noteLength);

    recvPacket.ReadByteSeq(playerGuid[1]);
    recvPacket.ReadByteSeq(playerGuid[0]);
    recvPacket.ReadByteSeq(playerGuid[6]);

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleSetMemberNote(this, note, playerGuid, type);
}

void WorldSession::HandleGuildQueryRanksOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_QUERY_RANKS");

    ObjectGuid guildGuid;

    uint8 bitOrder[8] = { 4, 1, 6, 7, 5, 3, 0, 2 };
    recvData.ReadBitInOrder(guildGuid, bitOrder);

    recvData.FlushBits();

    uint8 byteOrder[8] = { 0, 3, 4, 5, 6, 1, 2, 7 };
    recvData.ReadBytesSeq(guildGuid, byteOrder);

    if (Guild* guild = sGuildMgr->GetGuildByGuid(guildGuid))
        if (guild->IsMember(_player->GetGUID()))
            guild->HandleGuildRanks(this);
}

void WorldSession::HandleGuildAddRankOpcode(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_ADD_RANK");

    uint32 rankId;
    recvPacket >> rankId;

    uint32 length = recvPacket.ReadBits(7);

    recvPacket.FlushBits();

    std::string rankName = recvPacket.ReadString(length);

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleAddNewRank(this, rankName); //, rankId);
}

void WorldSession::HandleGuildDelRankOpcode(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_DEL_RANK");

    uint32 rankId;
    recvPacket >> rankId;

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleRemoveRank(this, rankId);
}

void WorldSession::HandleGuildChangeInfoTextOpcode(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_INFO_TEXT");

    uint32 length = recvPacket.ReadBits(12);

    recvPacket.FlushBits();

    std::string info = recvPacket.ReadString(length / 2);

    if (Guild* guild = _GetPlayerGuild(this, true))
        guild->HandleSetInfo(this, info);
}

void WorldSession::HandleSaveGuildEmblemOpcode(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received MSG_SAVE_GUILD_EMBLEM");

    uint64 vendorGuid;
    recvPacket >> vendorGuid;

    EmblemInfo emblemInfo;
    emblemInfo.ReadPacket(recvPacket);

    if (GetPlayer()->GetNPCIfCanInteractWith(vendorGuid, UNIT_NPC_FLAG_TABARDDESIGNER))
    {
        // Remove fake death
        if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
            GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

        if (Guild* guild = _GetPlayerGuild(this))
            guild->HandleSetEmblem(this, emblemInfo);
        else
            // "You are not part of a guild!";
            Guild::SendSaveEmblemResult(this, ERR_GUILDEMBLEM_NOGUILD);
    }
    else
    {
        // "That's not an emblem vendor!"
        Guild::SendSaveEmblemResult(this, ERR_GUILDEMBLEM_INVALIDVENDOR);
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleSaveGuildEmblemOpcode - Unit (GUID: %u) not found or you can't interact with him.", GUID_LOPART(vendorGuid));
    }
}

void WorldSession::HandleGuildEventLogQueryOpcode(WorldPacket& /* recvPacket */)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_EVENT_LOG_QUERY)");

    if (Guild* guild = _GetPlayerGuild(this))
        guild->SendEventLog(this);
}

void WorldSession::HandleGuildBankMoneyWithdrawn(WorldPacket& /* recvData */)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_MONEY_WITHDRAWN_QUERY)");

    if (Guild* guild = _GetPlayerGuild(this))
        guild->SendMoneyInfo(this);
}

void WorldSession::HandleGuildPermissions(WorldPacket& /* recvData */)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_PERMISSIONS)");

    if (Guild* guild = _GetPlayerGuild(this))
        guild->SendPermissions(this);
}

// Called when clicking on Guild bank gameobject
void WorldSession::HandleGuildBankerActivate(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANKER_ACTIVATE)");

    ObjectGuid GoGuid;
    bool fullSlotList;

    GoGuid[1] = recvData.ReadBit();
    GoGuid[7] = recvData.ReadBit();
    GoGuid[6] = recvData.ReadBit();
    GoGuid[3] = recvData.ReadBit();
    GoGuid[5] = recvData.ReadBit();
    GoGuid[0] = recvData.ReadBit();
    fullSlotList = recvData.ReadBit(); // 0 = only slots updated in last operation are shown. 1 = all slots updated
    GoGuid[4] = recvData.ReadBit();
    GoGuid[2] = recvData.ReadBit();

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 0, 2, 6, 5, 3, 7, 4, 1 };
    recvData.ReadBytesSeq(GoGuid, bytesOrder);

    if (GetPlayer()->GetGameObjectIfCanInteractWith(GoGuid, GAMEOBJECT_TYPE_GUILD_BANK))
    {
        if (Guild* guild = _GetPlayerGuild(this))
            guild->SendBankList(this, 0, true, true);
        else
            Guild::SendCommandResult(this, GUILD_UNK1, ERR_GUILD_PLAYER_NOT_IN_GUILD);
    }
}

// Called when opening guild bank tab only (first one)
void WorldSession::HandleGuildBankQueryTab(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_QUERY_TAB)");

    ObjectGuid GoGuid;
    uint8 tabId;
    bool fullSlotList;

    recvData >> tabId;

    GoGuid[3] = recvData.ReadBit();
    GoGuid[1] = recvData.ReadBit();
    GoGuid[4] = recvData.ReadBit();
    GoGuid[0] = recvData.ReadBit();
    GoGuid[6] = recvData.ReadBit();
    GoGuid[5] = recvData.ReadBit();
    GoGuid[7] = recvData.ReadBit();
    fullSlotList = recvData.ReadBit(); // 0 = only slots updated in last operation are shown. 1 = all slots updated
    GoGuid[2] = recvData.ReadBit();

    uint8 bytesOrder[8] = { 2, 5, 6, 0, 1, 4, 7, 3 };
    recvData.ReadBytesSeq(GoGuid, bytesOrder);

    if (GetPlayer()->GetGameObjectIfCanInteractWith(GoGuid, GAMEOBJECT_TYPE_GUILD_BANK))
        if (Guild* guild = _GetPlayerGuild(this))
            guild->SendBankList(this, tabId, true, false);
}

void WorldSession::HandleGuildBankDepositMoney(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_DEPOSIT_MONEY)");

    ObjectGuid goGuid;
    uint64 money;

    recvData >> money;

    uint8 bitsOrder[8] = { 4, 3, 5, 6, 1, 0, 2, 7 };
    recvData.ReadBitInOrder(goGuid, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 1, 0, 4, 5, 2, 7, 3, 6 };
    recvData.ReadBytesSeq(goGuid, bytesOrder);

    if (GetPlayer()->GetGameObjectIfCanInteractWith(goGuid, GAMEOBJECT_TYPE_GUILD_BANK))
        if (money && GetPlayer()->HasEnoughMoney(money))
            if (Guild* guild = _GetPlayerGuild(this))
                guild->HandleMemberDepositMoney(this, money);
}

void WorldSession::HandleGuildBankWithdrawMoney(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_WITHDRAW_MONEY)");

    uint64 money;
    ObjectGuid GoGuid;

    recvData >> money;

    uint8 bitsOrder[8] = { 7, 6, 5, 0, 4, 3, 1, 2 };
    recvData.ReadBitInOrder(GoGuid, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 2, 4, 6, 7, 3, 1, 0, 5 };
    recvData.ReadBytesSeq(GoGuid, bytesOrder);

    if (money)
        if (GetPlayer()->GetGameObjectIfCanInteractWith(GoGuid, GAMEOBJECT_TYPE_GUILD_BANK))
            if (Guild* guild = _GetPlayerGuild(this))
                guild->HandleMemberWithdrawMoney(this, money);
}

void WorldSession::HandleGuildBankSwapItems(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_SWAP_ITEMS)");

    ObjectGuid GoGuid;
    uint32 amountSplited = 0;
    uint32 originalItemId = 0;
    uint32 itemId = 0;
    uint32 autostoreCount = 0;
    uint8 srcTabSlot = 0;
    uint8 toChar = 0;
    uint8 srcTabId = 0;
    uint8 dstTabId = 0;
    uint8 dstTabSlot = 0;
    uint8 plrSlot = 0;
    uint8 plrBag = 0;
    bool bit52 = false;
    bool hasDstTab = false;
    bool bankToBank = false;
    bool hasSrcTabSlot = false;
    bool hasPlrSlot = false;
    bool hasItemId = false;
    bool hasPlrBag = false;
    bool autostore = false;

    recvData >> amountSplited;
    recvData >> dstTabSlot >> toChar;
    recvData >> originalItemId;
    recvData >> srcTabId;

    bit52 = recvData.ReadBit();
    GoGuid[3] = recvData.ReadBit();
    hasDstTab = !recvData.ReadBit();
    bankToBank = recvData.ReadBit();
    hasSrcTabSlot = !recvData.ReadBit();
    GoGuid[1] = recvData.ReadBit();
    hasPlrSlot = !recvData.ReadBit();
    GoGuid[4] = recvData.ReadBit();
    hasItemId = !recvData.ReadBit();
    GoGuid[0] = recvData.ReadBit();
    hasPlrBag = !recvData.ReadBit();
    GoGuid[2] = recvData.ReadBit();
    GoGuid[7] = recvData.ReadBit();
    autostore = !recvData.ReadBit();
    GoGuid[6] = recvData.ReadBit();
    GoGuid[5] = recvData.ReadBit();

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 7, 3, 1, 6, 5, 4, 2, 0 };
    recvData.ReadBytesSeq(GoGuid, bytesOrder);

    if (hasItemId)
        recvData >> itemId;

    if (hasPlrSlot)
        recvData >> plrSlot;

    if (autostore)
        recvData >> autostoreCount;

    if (hasDstTab)
        recvData >> dstTabId;

    if (hasPlrBag)
        recvData >> plrBag;

    if (hasSrcTabSlot)
        recvData >> srcTabSlot;

    if (!GetPlayer()->GetGameObjectIfCanInteractWith(GoGuid, GAMEOBJECT_TYPE_GUILD_BANK))
    {
        recvData.rfinish();                   // Prevent additional spam at rejected packet
        return;
    }

    Guild* guild = _GetPlayerGuild(this);
    if (!guild)
    {
        recvData.rfinish();                   // Prevent additional spam at rejected packet
        return;
    }

    if (bankToBank)
        guild->SwapItems(GetPlayer(), dstTabId, srcTabSlot, srcTabId, dstTabSlot, amountSplited);
    else if (autostore)
        guild->AutoStoreItemInInventory(GetPlayer(), srcTabId, dstTabSlot, autostoreCount);
    else
    {
        // Player <-> Bank
        // Allow to work with inventory only
        if (!Player::IsInventoryPos(plrBag, plrSlot) && !(plrBag == NULL_BAG && plrSlot == NULL_SLOT))
            GetPlayer()->SendEquipError(EQUIP_ERR_INTERNAL_BAG_ERROR, NULL);
        else
            guild->SwapItemsWithInventory(GetPlayer(), toChar, srcTabId, dstTabSlot, plrBag, plrSlot, amountSplited);
    }
}

void WorldSession::HandleGuildBankBuyTab(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_BUY_TAB)");

    uint8 tabId;
    ObjectGuid GoGuid;
    recvData >> tabId;

    uint8 bitsOrder[8] = { 1, 3, 7, 2, 4, 0, 5, 6 };
    recvData.ReadBitInOrder(GoGuid, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 3, 1, 6, 5, 4, 2, 7, 0 };
    recvData.ReadBytesSeq(GoGuid, bytesOrder);

    // Only for SPELL_EFFECT_UNLOCK_GUILD_VAULT_TAB, this prevent cheating
    if (tabId > 5)
        return;

    if (!GoGuid || GetPlayer()->GetGameObjectIfCanInteractWith(GoGuid, GAMEOBJECT_TYPE_GUILD_BANK))
        if (Guild* guild = _GetPlayerGuild(this))
            guild->HandleBuyBankTab(this, tabId);
}

void WorldSession::HandleGuildBankUpdateTab(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (CMSG_GUILD_BANK_UPDATE_TAB)");

    bool pair;
    uint8 tabId;
    ObjectGuid GoGuid;
    uint32 nameLen, iconLen;
    std::string name;
    std::string icon;

    recvData >> tabId;

    nameLen = recvData.ReadBits(7);
    iconLen = recvData.ReadBits(8);
    iconLen *= 2;

    pair = recvData.ReadBit();

    if (pair)
        iconLen++;

    uint8 bitsOrder[8] = { 0, 2, 6, 7, 3, 4, 5, 1 };
    recvData.ReadBitInOrder(GoGuid, bitsOrder);

    recvData.FlushBits();

    recvData.ReadByteSeq(GoGuid[6]);
    recvData.ReadByteSeq(GoGuid[4]);

    icon = recvData.ReadString(iconLen);

    recvData.ReadByteSeq(GoGuid[5]);
    recvData.ReadByteSeq(GoGuid[0]);
    recvData.ReadByteSeq(GoGuid[7]);
    recvData.ReadByteSeq(GoGuid[1]);

    name = recvData.ReadString(nameLen);

    recvData.ReadByteSeq(GoGuid[2]);
    recvData.ReadByteSeq(GoGuid[3]);

    if (!name.empty() && !icon.empty())
        if (GetPlayer()->GetGameObjectIfCanInteractWith(GoGuid, GAMEOBJECT_TYPE_GUILD_BANK))
            if (Guild* guild = _GetPlayerGuild(this))
                guild->HandleSetBankTabInfo(this, tabId, name, icon);
}

void WorldSession::HandleGuildBankLogQuery(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received (MSG_GUILD_BANK_LOG_QUERY)");

    uint32 tabId;
    recvData >> tabId;

    if (Guild* guild = _GetPlayerGuild(this))
        guild->SendBankLog(this, tabId);
}

void WorldSession::HandleQueryGuildBankTabText(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_BANK_QUERY_TEXT");

    uint32 tabId;
    recvData >> tabId;

    if (Guild* guild = _GetPlayerGuild(this))
        guild->SendBankTabText(this, tabId);
}

void WorldSession::HandleSetGuildBankTabText(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_SET_GUILD_BANK_TEXT");

    uint32 tabId;
    recvData >> tabId;

    uint32 textLen = recvData.ReadBits(14);

    recvData.FlushBits();

    std::string text = recvData.ReadString(textLen);

    if (Guild* guild = _GetPlayerGuild(this))
        guild->SetBankTabText(tabId, text);
}

void WorldSession::HandleGuildQueryXPOpcode(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_QUERY_GUILD_XP");

    ObjectGuid guildGuid;

    uint8 bitOrder[8] = { 2, 1, 6, 4, 3, 7, 0, 5 };
    recvPacket.ReadBitInOrder(guildGuid, bitOrder);

    recvPacket.FlushBits();

    uint8 byteOrder[8] = { 6, 0, 1, 3, 4, 7, 5, 2 };
    recvPacket.ReadBytesSeq(guildGuid, byteOrder);

    if (Guild* guild = sGuildMgr->GetGuildByGuid(guildGuid))
        if (guild->IsMember(_player->GetGUID()))
            guild->SendGuildXP(this);
}

void WorldSession::HandleGuildSetRankPermissionsOpcode(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_SET_RANK_PERMISSIONS");

    Guild* guild = _GetPlayerGuild(this, true);
    if (!guild)
    {
        recvPacket.rfinish();
        return;
    }

    uint32 unk;
    uint32 rankId;
    uint32 oldRights;
    uint32 newRights;
    uint32 moneyPerDay;

    recvPacket >> rankId;
    recvPacket >> newRights;
    recvPacket >> moneyPerDay;

    GuildBankRightsAndSlotsVec rightsAndSlots(GUILD_BANK_MAX_TABS);
    for (uint8 tabId = 0; tabId < GUILD_BANK_MAX_TABS; ++tabId)
    {
        uint32 bankRights;
        uint32 slots;

        recvPacket >> bankRights;
        recvPacket >> slots;

        rightsAndSlots[tabId] = GuildBankRightsAndSlots(uint8(bankRights), slots);
    }

    recvPacket >> unk;
    recvPacket >> oldRights;
    uint32 nameLength = recvPacket.ReadBits(7);
    recvPacket.FlushBits();
    std::string rankName = recvPacket.ReadString(nameLength);

    guild->HandleSetRankInfo(this, rankId, rankName, newRights, moneyPerDay, rightsAndSlots);
}

void WorldSession::HandleGuildRequestPartyState(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_REQUEST_PARTY_STATE");

    ObjectGuid guildGuid;

    uint8 bitOrder[8] = { 0, 1, 2, 6, 5, 7, 3, 4 };
    recvData.ReadBitInOrder(guildGuid, bitOrder);

    recvData.FlushBits();

    uint8 byteOrder[8] = { 4, 1, 6, 7, 2, 3, 5, 0 };
    recvData.ReadBytesSeq(guildGuid, byteOrder);

    if (Guild* guild = sGuildMgr->GetGuildByGuid(guildGuid))
        guild->HandleGuildPartyRequest(this);
}

void WorldSession::HandleGuildRequestMaxDailyXP(WorldPacket& recvPacket)
{
    ObjectGuid guildGuid;

    uint8 bitOrder[8] = {2, 5, 3, 7, 4, 1, 0, 6};
    recvPacket.ReadBitInOrder(guildGuid, bitOrder);

    uint8 byteOrder[8] = {7, 3, 2, 1, 0, 5, 6, 4};
    recvPacket.ReadBytesSeq(guildGuid, byteOrder);

    if (Guild* guild = sGuildMgr->GetGuildByGuid(guildGuid))
    {
        if (guild->IsMember(_player->GetGUID()))
        {
            WorldPacket data(SMSG_GUILD_MAX_DAILY_XP, 8);
            data << uint64(sWorld->getIntConfig(CONFIG_GUILD_DAILY_XP_CAP));
            SendPacket(&data);
        }
    }
}

void WorldSession::HandleAutoDeclineGuildInvites(WorldPacket& recvPacket)
{
    bool enable;
    enable = recvPacket.ReadBit();

    GetPlayer()->ApplyModFlag(PLAYER_FLAGS, PLAYER_FLAGS_AUTO_DECLINE_GUILD, enable);
}

void WorldSession::HandleGuildRewardsQueryOpcode(WorldPacket& recvPacket)
{
    uint32 unk = 0;
    recvPacket >> unk;

    if (Guild* guild = sGuildMgr->GetGuildById(_player->GetGuildId()))
    {
        std::vector<GuildReward> const& rewards = sGuildMgr->GetGuildRewards();

        WorldPacket data(SMSG_GUILD_REWARDS_LIST);
        ByteBuffer dataBuffer;

        data << uint32(time(NULL));
        data.WriteBits(rewards.size(), 19);

        for (uint32 i = 0; i < rewards.size(); i++)
        {
            data.WriteBits(rewards[i].AchievementId > 0 ? 1 : 0, 22);

            dataBuffer << uint32(rewards[i].Entry);
            dataBuffer << uint64(rewards[i].Price);
            dataBuffer << uint32(rewards[i].Racemask);

            if (rewards[i].AchievementId)
                dataBuffer << uint32(rewards[i].AchievementId);

            dataBuffer << uint32(rewards[i].Standing);
            dataBuffer << uint32(0);
        }

        data.FlushBits();
        data.append(dataBuffer);
        SendPacket(&data);
    }
}

void WorldSession::HandleGuildQueryNewsOpcode(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_QUERY_NEWS");

    ObjectGuid guildGuid;

    uint8 bitsOrder[8] = { 7, 3, 4, 1, 0, 6, 2, 5 };
    recvPacket.ReadBitInOrder(guildGuid, bitsOrder);

    recvPacket.FlushBits();

    uint8 bytesOrder[8] = { 2, 7, 6, 4, 3, 1, 0, 5 };
    recvPacket.ReadBytesSeq(guildGuid, bytesOrder);

    if (Guild* guild = sGuildMgr->GetGuildByGuid(guildGuid))
    {
        if (guild->IsMember(_player->GetGUID()))
        {
            WorldPacket data;
            guild->GetNewsLog().BuildNewsData(data);
            SendPacket(&data);
        }
    }
}

void WorldSession::HandleGuildNewsUpdateStickyOpcode(WorldPacket& recvPacket)
{
    uint32 newsId;
    bool sticky;
    ObjectGuid guid;

    recvPacket >> newsId;

    guid[6] = recvPacket.ReadBit();
    guid[7] = recvPacket.ReadBit();
    guid[1] = recvPacket.ReadBit();
    sticky = recvPacket.ReadBit();
    guid[2] = recvPacket.ReadBit();
    guid[5] = recvPacket.ReadBit();
    guid[0] = recvPacket.ReadBit();
    guid[3] = recvPacket.ReadBit();
    guid[4] = recvPacket.ReadBit();

    uint8 byteOrder[8] = {0, 7, 2, 3, 6, 5, 1, 4};
    recvPacket.ReadBytesSeq(guid, byteOrder);

    if (Guild* guild = sGuildMgr->GetGuildById(_player->GetGuildId()))
    {
        if (GuildNewsEntry* newsEntry = guild->GetNewsLog().GetNewsById(newsId))
        {
            if (sticky)
                newsEntry->Flags |= 1;
            else
                newsEntry->Flags &= ~1;
            WorldPacket data;
            guild->GetNewsLog().BuildNewsData(newsId, *newsEntry, data);
            SendPacket(&data);
        }
    }
}

void WorldSession::HandleGuildRequestChallengeUpdate(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_REQUEST_CHALLENGE_UPDATE");

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

void WorldSession::HandleGuildRequestGuildRecipes(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GUILD_REQUEST_CHALLENGE_UPDATE");

    ObjectGuid guildGuid;

    uint8 bitsOrder[8] = { 1, 7, 4, 5, 2, 6, 0, 3 };
    recvPacket.ReadBitInOrder(guildGuid, bitsOrder);

    recvPacket.FlushBits();

    uint8 bytesOrder[8] = { 7, 0, 2, 3, 1, 5, 4, 6 };
    recvPacket.ReadBytesSeq(guildGuid, bytesOrder);

    if (Guild* guild = sGuildMgr->GetGuildByGuid(guildGuid))
        if (guild->IsMember(_player->GetGUID()))
            guild->SendGuildRecipes(this);
}