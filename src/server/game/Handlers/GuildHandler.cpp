////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
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
    uint64 l_GuildGuid  = 0;
    uint64 l_PlayerGuid = 0;

    p_Packet.readPackGUID(l_GuildGuid);
    p_Packet.readPackGUID(l_PlayerGuid);

    /// If guild doesn't exist or player is not part of the guild send error
    if (Guild* l_Guild = sGuildMgr->GetGuildByGuid(l_GuildGuid))
    {
        if (l_Guild->IsMember(l_PlayerGuid))
        {
            l_Guild->HandleQuery(this);

            if (m_Player && m_Player->IsInWorld() && l_Guild->IsMember(m_Player->GetGUID()))
                l_Guild->SendBankList(this, 0, false, true);

            return;
        }
    }
    else if (!GetInterRealmBG())
       Guild::SendCommandResult(this, GUILD_CREATE_S, ERR_GUILD_PLAYER_NOT_IN_GUILD);
}

void WorldSession::HandleGuildInviteByNameOpcode(WorldPacket& p_Packet)
{
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
    uint64 l_Removee = 0;

    p_Packet.readPackGUID(l_Removee);

    if (Guild* l_Guild = _GetPlayerGuild(this, true))
        l_Guild->HandleRemoveMember(this, l_Removee);
}

void WorldSession::HandleGuildMasterReplaceOpcode(WorldPacket& /*p_Packet*/)
{
    Guild* l_Guild = _GetPlayerGuild(this, true);
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
    if (Guild* l_Guild = m_Player->GetGuild())
        l_Guild->HandleLeaveMember(this);

    /// Player cannot be in guild
    if (!m_Player->GetGuildId())
    {
        /// Guild where player was invited must exist
        if (Guild* l_Guild = sGuildMgr->GetGuildById(GetPlayer()->GetGuildIdInvited()))
            l_Guild->HandleAcceptMember(this);
    }
}

void WorldSession::HandleGuildDeclineInvitationsOpcode(WorldPacket& /*recvPacket*/)
{
    GetPlayer()->SetGuildIdInvited(0);
}

void WorldSession::HandleGuildRosterOpcode(WorldPacket& /*p_Packet*/)
{
    if (Guild* l_Guild = _GetPlayerGuild(this, true))
        l_Guild->HandleRoster(this);
}

void WorldSession::HandleGuildAssignRankOpcode(WorldPacket& p_Packet)
{
    uint64 l_Member = 0;
    uint32 l_RankOrder = 0;

    p_Packet.readPackGUID(l_Member);
    p_Packet >> l_RankOrder;

    if (Guild* l_Guild = _GetPlayerGuild(this, true))
        l_Guild->HandleSetMemberRank(this, l_Member, GetPlayer()->GetGUID(), l_RankOrder);
}

void WorldSession::HandleGuildLeaveOpcode(WorldPacket& /*recvPacket*/)
{
    if (Guild* l_Guild = _GetPlayerGuild(this, true))
        l_Guild->HandleLeaveMember(this);
}

void WorldSession::HandleGuildDeleteOpcode(WorldPacket& /*recvPacket*/)
{
    if (Guild* l_Guild = _GetPlayerGuild(this, true))
        l_Guild->HandleDisband(this);
}

void WorldSession::HandleGuildSetGuildMasterOpcode(WorldPacket& recvPacket)
{
    uint32 l_NewMasterNameLenght = 0;
    std::string l_NewMasterName  = "";

    l_NewMasterNameLenght = recvPacket.ReadBits(9);
    l_NewMasterName = recvPacket.ReadString(l_NewMasterNameLenght);

    // Extract real player name from PlayerName-Ysondre-EU
    size_t l_Position = l_NewMasterName.find('-');
    if (l_Position != std::string::npos)
        l_NewMasterName = l_NewMasterName.substr(0, l_Position);

    if (normalizePlayerName(l_NewMasterName))
    {
        if (Guild* l_Guild = _GetPlayerGuild(this, true))
            l_Guild->HandleSetLeader(this, l_NewMasterName);
    }
}

void WorldSession::HandleGuildUpdateMOTDTextOpcode(WorldPacket& p_Packet)
{
    uint32 l_MotdTextLenght = 0;
    std::string l_MotdText = "";

    l_MotdTextLenght = p_Packet.ReadBits(10);
    l_MotdText = p_Packet.ReadString(l_MotdTextLenght);

    if (Guild* l_Guild = _GetPlayerGuild(this, true))
        l_Guild->HandleSetMOTD(this, l_MotdText);
}

void WorldSession::HandleShiftRanks(WorldPacket& p_Packet)
{
    uint32 l_RankOrder = 0;
    bool l_ShiftUp = false;

    p_Packet >> l_RankOrder;
    l_ShiftUp = p_Packet.ReadBit();

    if (Guild* l_Guild = _GetPlayerGuild(this, true))
        l_Guild->HandleSwapRanks(this, l_RankOrder, l_ShiftUp);
}

void WorldSession::HandleGuildSetMemberNoteOpcode(WorldPacket& p_Packet)
{
    uint64 l_NoteeGUID = 0;
    uint32 l_NoteLenght = 0;
    bool l_IsPublic = false;

    std::string l_Note = "";

    p_Packet.readPackGUID(l_NoteeGUID);
    l_NoteLenght    = p_Packet.ReadBits(8);
    l_IsPublic      = p_Packet.ReadBit();

    l_Note = p_Packet.ReadString(l_NoteLenght);

    if (Guild* l_Guild = _GetPlayerGuild(this, true))
        l_Guild->HandleSetMemberNote(this, l_Note, l_NoteeGUID, l_IsPublic);
}

void WorldSession::HandleGuildGetRanksOpcode(WorldPacket& p_Packet)
{
    uint64 l_GuildGUID = 0;

    p_Packet.readPackGUID(l_GuildGUID);

    if (Guild* l_Guild = sGuildMgr->GetGuildByGuid(l_GuildGUID))
    {
        if (l_Guild->IsMember(m_Player->GetGUID()))
            l_Guild->HandleGuildRanks(this);
    }
}

void WorldSession::HandleGuildAddRankOpcode(WorldPacket& p_Packet)
{
    uint32 l_RankOrder = 0;
    uint32 l_NameLength = 0;
    std::string l_Name = "";

    l_NameLength = p_Packet.ReadBits(7);

    p_Packet >> l_RankOrder;

    l_Name = p_Packet.ReadString(l_NameLength);

    if (Guild* l_Guild = _GetPlayerGuild(this, true))
        l_Guild->HandleAddNewRank(this, l_Name); //, rankId);
}

void WorldSession::HandleGuildDeleteRankOpcode(WorldPacket& p_Packet)
{
    uint32 l_RankOrder = 0;

    p_Packet >> l_RankOrder;

    if (Guild* l_Guild = _GetPlayerGuild(this, true))
        l_Guild->HandleRemoveRank(this, l_RankOrder);
}

void WorldSession::HandleGuildUpdateInfoTextOpcode(WorldPacket& p_Packet)
{
    uint32 l_Length = p_Packet.ReadBits(11);
    p_Packet.FlushBits();
    std::string l_Info = p_Packet.ReadString(l_Length);

    if (Guild* l_Guild = _GetPlayerGuild(this, true))
        l_Guild->HandleSetInfo(this, l_Info);
}

void WorldSession::HandlePlayerSaveGuildEmblemOpcode(WorldPacket& p_Packet)
{
    ///< WTF ?
    uint64 l_PlayerGuid = 0;
    p_Packet.readPackGUID(l_PlayerGuid);

    EmblemInfo l_EmblemInfo;
    l_EmblemInfo.ReadPacket(p_Packet);

    /// Remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    if (Guild* l_Guid = _GetPlayerGuild(this))
        l_Guid->HandleSetEmblem(this, l_EmblemInfo);
    else
        /// "You are not part of a guild!";
        Guild::SendSaveEmblemResult(this, ERR_GUILDEMBLEM_NOGUILD);
}

void WorldSession::HandleGuildEventLogQueryOpcode(WorldPacket& /*p_Packet*/)
{
    if (Guild* l_Guild = _GetPlayerGuild(this))
        l_Guild->SendEventLog(this);
}

void WorldSession::HandleGuildBankRemainingWithdrawMoneyQueryOpcode(WorldPacket& /* recvData */)
{
    if (Guild* guild = _GetPlayerGuild(this))
        guild->SendMoneyInfo(this);
}

void WorldSession::HandleGuildPermissionsQueryOpcode(WorldPacket& /* recvData */)
{
    if (Guild* l_Guild = _GetPlayerGuild(this))
        l_Guild->SendPermissions(this);
}

/// Called when clicking on Guild bank gameobject or Banker NpC
void WorldSession::HandleGuildBankActivate(WorldPacket& p_Packet)
{
    uint64 l_Banker   = 0;
    bool l_FullUpdate = false;

    p_Packet.readPackGUID(l_Banker);
    l_FullUpdate = p_Packet.ReadBit();  ///< 0 = only slots updated in last operation are shown. 1 = all slots updated ///> l_FullUpdate is never read 01/18/16

    if (IS_GAMEOBJECT_GUID(l_Banker))
    {
        if (!m_Player->GetGameObjectIfCanInteractWith(l_Banker, GAMEOBJECT_TYPE_GUILD_BANK))
            return;
    }
    else if (IS_UNIT_GUID(l_Banker))
    {
        if (!m_Player->GetNPCIfCanInteractWith(l_Banker, UNIT_NPC_FLAG_GUILD_BANKER))
            return;
    }

    if (Guild* l_Guild = _GetPlayerGuild(this))
    {
        m_Player->SetInteractionStatus(l_Banker, InteractionStatus::GuildBanq);
        l_Guild->SendBankList(this, 0, true, true);
    }
    else
        Guild::SendCommandResult(this, GUILD_BANK, ERR_GUILD_PLAYER_NOT_IN_GUILD);
}

// Called when opening guild bank tab only (first one)
void WorldSession::HandleGuildBankQueryTab(WorldPacket& p_Packet)
{
    uint64 l_Banker = 0;
    uint8 l_Tab = 0;
    bool l_FullUpdate = false;

    p_Packet.readPackGUID(l_Banker);
    p_Packet >> l_Tab;
    l_FullUpdate = p_Packet.ReadBit();      ///< 0 = only slots updated in last operation are shown. 1 = all slots updated ///< l_FullUpdate is never read 01/18/16

    if (GetPlayer()->GetGameObjectIfCanInteractWith(l_Banker, GAMEOBJECT_TYPE_GUILD_BANK))
    {
        if (Guild* l_Guild = _GetPlayerGuild(this))
        {
            l_Guild->SendBankList(this, l_Tab, true, true);
            l_Guild->SendMoneyInfo(this);
        }
    }
}

void WorldSession::HandleGuildBankDepositMoney(WorldPacket& p_Packet)
{
    uint64 l_Banker = 0;
    uint64 l_Money = 0;

    p_Packet.readPackGUID(l_Banker);
    p_Packet >> l_Money;

    if (m_Player->GetGameObjectIfCanInteractWith(l_Banker, GAMEOBJECT_TYPE_GUILD_BANK))
    {
        if (l_Money && m_Player->HasEnoughMoney(l_Money))
        {
            if (Guild* l_Guild = _GetPlayerGuild(this))
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

void WorldSession::HandleGuildBankWithdrawMoney(WorldPacket& p_Packet)
{
    uint64 l_Banker;
    uint64 l_Money;

    p_Packet.readPackGUID(l_Banker);
    p_Packet >> l_Money;

    if (l_Money)
    {
        if (GetPlayer()->GetGameObjectIfCanInteractWith(l_Banker, GAMEOBJECT_TYPE_GUILD_BANK))
        {
            if (Guild* l_Guild = _GetPlayerGuild(this))
                l_Guild->HandleMemberWithdrawMoney(this, l_Money);
        }
    }
}

void WorldSession::HandleGuildBankSwapItems(WorldPacket& p_Packet)
{
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

    Guild* l_Guild = _GetPlayerGuild(this);
    if (!l_Guild)
        return;

    if (l_BankOnly)
        l_Guild->SwapItems(GetPlayer(), l_BankTab1, l_BankSlot1, l_BankTab, l_BankSlot, l_StackCount);
    else if (l_AutoStore)
        l_Guild->AutoStoreItemInInventory(GetPlayer(), l_BankTab, l_BankSlot, l_BankItemCount);
    else
    {
        /// Player <-> Bank
        /// Allow to work with inventory only
        if (!Player::IsInventoryPos(l_ContainerSlot, l_ContainerItemSlot) && !(l_ContainerSlot == NULL_BAG && l_ContainerItemSlot == NULL_SLOT))
            GetPlayer()->SendEquipError(EQUIP_ERR_INTERNAL_BAG_ERROR, NULL);
        else
            l_Guild->SwapItemsWithInventory(GetPlayer(), l_ToSlot, l_BankTab, l_BankSlot, l_ContainerSlot, l_ContainerItemSlot, l_StackCount);
    }
}

void WorldSession::HandleGuildBankBuyTab(WorldPacket& p_Packet)
{
    uint64 l_Banker = 0;
    uint8 l_BankTab = 0;

    p_Packet.readPackGUID(l_Banker);
    p_Packet >> l_BankTab;

    /// Only for SPELL_EFFECT_UNLOCK_GUILD_VAULT_TAB, this prevent cheating
    if (l_BankTab > 5)
        return;

    if (!l_Banker || GetPlayer()->GetGameObjectIfCanInteractWith(l_Banker, GAMEOBJECT_TYPE_GUILD_BANK))
    {
        if (Guild* l_Guild = _GetPlayerGuild(this))
            l_Guild->HandleBuyBankTab(this, l_BankTab);
    }
}

void WorldSession::HandleGuildBankUpdateTab(WorldPacket& p_Packet)
{
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
            if (Guild* l_Guild = _GetPlayerGuild(this))
                l_Guild->HandleSetBankTabInfo(this, l_BankTab, l_Name, l_Icon);
        }
    }
}

void WorldSession::HandleGuildBankLogQuery(WorldPacket& p_Packet)
{
    uint32 l_TabID = 0;

    p_Packet >> l_TabID;

    if (Guild* l_Guild = _GetPlayerGuild(this))
        l_Guild->SendBankLog(this, l_TabID);
}

void WorldSession::HandleQueryGuildBankTextQuery(WorldPacket& p_Packet)
{
    uint32 l_Tab = 0;

    p_Packet >> l_Tab;

    if (Guild* l_Guild = _GetPlayerGuild(this))
        l_Guild->SendBankTabText(this, l_Tab);
}

void WorldSession::HandleSetGuildBankTabText(WorldPacket& p_Packet)
{
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
    Guild* l_Guild = _GetPlayerGuild(this, true);
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
    uint64 l_GuildGUID = 0;
    p_Packet.readPackGUID(l_GuildGUID);

    if (Guild* l_Guild = sGuildMgr->GetGuildByGuid(l_GuildGUID))
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
    uint32 l_CurrentVersion = p_Packet.read<uint32>(); ///< l_currentversion is never read 01/18/16

    if (sGuildMgr->GetGuildById(m_Player->GetGuildId()) != nullptr)
    {
        std::vector<GuildReward> const& l_Rewards = sGuildMgr->GetGuildRewards();

        WorldPacket l_Data(SMSG_GUILD_REWARDS_LIST, 5 * 1024);

        l_Data << uint32(time(NULL));
        l_Data << uint32(l_Rewards.size());

        for (uint32 l_Iter = 0; l_Iter < l_Rewards.size(); l_Iter++)
        {
            l_Data << uint32(l_Rewards[l_Iter].Entry);                      ///< ItemID
            l_Data << uint32(0);                                            ///< Unk
            l_Data << uint32(l_Rewards[l_Iter].AchievementId > 0 ? 1 : 0);  ///< AchievementsRequired
            l_Data << uint32(l_Rewards[l_Iter].Racemask);                   ///< RaceMask
            l_Data << uint32(0);                                            ///< MinGuildLevel
            l_Data << uint32(l_Rewards[l_Iter].Standing);                   ///< MinGuildRep
            l_Data << uint64(l_Rewards[l_Iter].Price);                      ///< Cost

            if (l_Rewards[l_Iter].AchievementId)
                l_Data << uint32(l_Rewards[l_Iter].AchievementId);
        }

        l_Data.FlushBits();

        SendPacket(&l_Data);
    }
}

void WorldSession::HandleGuildQueryNewsOpcode(WorldPacket& p_Packet)
{
    uint64 l_GuildGUID = 0;

    p_Packet.readPackGUID(l_GuildGUID);

    if (Guild* l_Guild = sGuildMgr->GetGuildByGuid(l_GuildGUID))
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

    if (Guild* l_Guild = sGuildMgr->GetGuildById(m_Player->GetGuildId()))
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

void WorldSession::HandleGuildChallengeUpdateRequest(WorldPacket& /*p_RecvData*/)
{
    const GuildChallengeRewardData& l_Reward = sObjectMgr->GetGuildChallengeRewardData();

    WorldPacket l_Data(SMSG_GUILD_CHALLENGE_UPDATED, 5*6*4);

    for (uint8 l_I = 0; l_I < ChallengeMax; l_I++)
    {
        if (Player* l_Player = GetPlayer())
        {
            if (!l_Player->GetGuild())
                l_Data << uint32(0);
            else
            {
                int32 l_ChallengeAmount = l_Player->GetGuild()->GetChallengeCount(l_I);
                l_Data << uint32(l_ChallengeAmount);
            }
        }
    }

    for (uint8 l_I = 0; l_I < ChallengeMax; l_I++)
        l_Data << uint32(l_Reward[l_I].ChallengeCount);

    for (uint8 l_I = 0; l_I < ChallengeMax; l_I++)
        l_Data << uint32(l_Reward[l_I].Experience);

    for (uint8 l_I = 0; l_I < ChallengeMax; l_I++)
        l_Data << uint32(l_Reward[l_I].Gold);

    SendPacket(&l_Data);
}

void WorldSession::HandleGuildRequestGuildRecipes(WorldPacket& p_Packet)
{
    uint64 l_GuildGUID = 0;
    p_Packet.readPackGUID(l_GuildGUID);

    if (Guild* l_Guild = sGuildMgr->GetGuildByGuid(l_GuildGUID))
    {
        if (l_Guild->IsMember(m_Player->GetGUID()))
            l_Guild->SendGuildRecipes(this);
    }
}
#endif
