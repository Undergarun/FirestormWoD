////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ObjectMgr.h"                                      // for normalizePlayerName
#include "ChannelMgr.h"
#include "Chat.h"

void WorldSession::HandleJoinChannel(WorldPacket& recvPacket)
{
    uint32 channelId;
    uint32 channelLength = 0;
    uint32 passLength = 0;
    std::string channelName, pass;

    recvPacket >> channelId;

    recvPacket.ReadBit(); // If channel's length is odd ? Not used ...
    recvPacket.ReadBit(); // If pass's length is odd ? Not used ...

    channelLength = recvPacket.ReadBits(7);
    passLength = recvPacket.ReadBits(7);

    channelName = recvPacket.ReadString(channelLength);
    pass = recvPacket.ReadString(passLength);

    if (channelId)
    {
        ChatChannelsEntry const* channel = sChatChannelsStore.LookupEntry(channelId);
        if (!channel)
            return;

        AreaTableEntry const* current_zone = GetAreaEntryByAreaID(m_Player->GetZoneId());
        if (!current_zone)
            return;

        if (!m_Player->CanJoinConstantChannelInZone(channel, current_zone))
            return;
    }

    if (channelName.empty())
        return;

    if (!ChatHandler(this).isValidChatMessage(channelName.c_str()))
        return;

    if (ChannelMgr* cMgr = channelMgr(m_Player->GetTeam()))
    {
        cMgr->team = m_Player->GetTeam();
        if (Channel* chn = cMgr->GetJoinChannel(channelName, channelId))
            chn->Join(m_Player->GetGUID(), pass.c_str());
    }
}

void WorldSession::HandleLeaveChannel(WorldPacket& recvPacket)
{
    uint32 l_ChannelID;
    recvPacket >> l_ChannelID;

    std::string l_ChannelName  = recvPacket.ReadString(recvPacket.ReadBits(7));

    if (l_ChannelName.empty())
        return;

    if (ChannelMgr* l_ChannelMgr = channelMgr(m_Player->GetTeam()))
    {
        if (Channel* l_Channel = l_ChannelMgr->GetChannel(l_ChannelName, m_Player))
            l_Channel->Leave(m_Player->GetGUID(), true);

        l_ChannelMgr->LeftChannel(l_ChannelName);
    }
}

void WorldSession::HandleChannelList(WorldPacket& p_Packet)
{
    std::string l_Channelname = p_Packet.ReadString(p_Packet.ReadBits(7));

    if (!ChatHandler(this).isValidChatMessage(l_Channelname.c_str()))
        return;

    if (ChannelMgr* l_ChannelMgr = channelMgr(m_Player->GetTeam()))
    {
        if (Channel* l_Channel = l_ChannelMgr->GetChannel(l_Channelname, m_Player))
            l_Channel->List(m_Player);
    }
}

void WorldSession::HandleChannelPassword(WorldPacket & p_Packet)
{
    time_t l_Now = time(NULL);
    if (l_Now - m_TimeLastChannelPassCommand < 5)
        return;
    else
        m_TimeLastChannelPassCommand = l_Now;

    uint32 l_ChannelNameLenght  = p_Packet.ReadBits(7);
    uint32 l_PasswordLenght     = p_Packet.ReadBits(7);

    std::string l_ChannelName   = p_Packet.ReadString(l_ChannelNameLenght);
    std::string l_Password      = p_Packet.ReadString(l_PasswordLenght);

    if (ChannelMgr* l_ChannelMgr = channelMgr(m_Player->GetTeam()))
    {
        if (Channel* l_Channel = l_ChannelMgr->GetChannel(l_ChannelName, m_Player))
            l_Channel->Password(m_Player->GetGUID(), l_Password.c_str());
    }
}

void WorldSession::HandleChannelSetOwner(WorldPacket& p_Packet)
{
    time_t l_Now = time(NULL);
    if (l_Now - m_TimeLastChannelSetownerCommand < 5)
        return;
    else
       m_TimeLastChannelSetownerCommand = l_Now;

    uint32 l_ChannelNameLength  = p_Packet.ReadBits(7);
    uint32 l_NewOwnerNameLength = p_Packet.ReadBits(9);

    std::string l_ChannelName  = p_Packet.ReadString(l_ChannelNameLength);
    std::string l_NewOwnerName = p_Packet.ReadString(l_NewOwnerNameLength);

    if (!normalizePlayerName(l_NewOwnerName))
        return;

    if (ChannelMgr * l_ChannelMgr = channelMgr(m_Player->GetTeam()))
    {
        if (Channel * l_Channel = l_ChannelMgr->GetChannel(l_ChannelName, m_Player))
            l_Channel->SetOwner(m_Player->GetGUID(), l_NewOwnerName.c_str());
    }
}

void WorldSession::HandleChannelOwner(WorldPacket& p_Packet)
{
    time_t l_l_Now = time(NULL);
    if (l_l_Now - m_TimeLastChannelOwnerCommand < 5)
        return;
    else
       m_TimeLastChannelOwnerCommand = l_l_Now;

    std::string l_ChannelName = p_Packet.ReadString(p_Packet.ReadBits(7));

    if (ChannelMgr* l_ChannelMgr = channelMgr(m_Player->GetTeam()))
    {
        if (Channel* l_Channel = l_ChannelMgr->GetChannel(l_ChannelName, m_Player))
            l_Channel->SendWhoOwner(m_Player->GetGUID());
    }
}

void WorldSession::HandleChannelModerator(WorldPacket & p_Packet)
{
    time_t l_Now = time(NULL);
    if (l_Now - m_TimeLastChannelModerCommand < 5)
        return;
    else
       m_TimeLastChannelModerCommand = l_Now;

    uint32 l_ChannelNameLength      = p_Packet.ReadBits(7);
    uint32 l_NewModeratorNameLenght = p_Packet.ReadBits(9);

    std::string l_ChannelName       = p_Packet.ReadString(l_ChannelNameLength);
    std::string l_NewModeratorName  = p_Packet.ReadString(l_NewModeratorNameLenght);

    if (!normalizePlayerName(l_NewModeratorName))
        return;

    if (ChannelMgr * l_ChannelMgr = channelMgr(m_Player->GetTeam()))
    {
        if (Channel * l_Channel = l_ChannelMgr->GetChannel(l_ChannelName, m_Player))
            l_Channel->SetModerator(m_Player->GetGUID(), l_NewModeratorName.c_str());
    }
}

void WorldSession::HandleChannelUnmoderator(WorldPacket & p_Packet)
{
    time_t l_Now = time(NULL);
    if (l_Now - m_TimeLastChannelUnmoderCommand < 5)
        return;
    else
       m_TimeLastChannelUnmoderCommand = l_Now;

    uint32 l_ChannelNameLength      = p_Packet.ReadBits(7);
    uint32 l_OldModeratorNameLenght = p_Packet.ReadBits(9);

    std::string l_ChannelName       = p_Packet.ReadString(l_ChannelNameLength);
    std::string l_OldModeratorName  = p_Packet.ReadString(l_OldModeratorNameLenght);

    if (!normalizePlayerName(l_OldModeratorName))
        return;

    if (ChannelMgr * l_ChannelMgr = channelMgr(m_Player->GetTeam()))
    {
        if (Channel * l_Channel = l_ChannelMgr->GetChannel(l_ChannelName, m_Player))
            l_Channel->UnsetModerator(m_Player->GetGUID(), l_OldModeratorName.c_str());
    }
}

void WorldSession::HandleChannelMute(WorldPacket & p_Packet)
{
    time_t l_Now = time(NULL);
    if (l_Now - m_TimeLastChannelMuteCommand < 5)
        return;
    else
        m_TimeLastChannelMuteCommand = l_Now;

    uint32 l_ChannelNameLength      = p_Packet.ReadBits(7);
    uint32 l_MutePlayerNameLength   = p_Packet.ReadBits(9);

    std::string l_ChannelName       = p_Packet.ReadString(l_ChannelNameLength);
    std::string l_MutePlayerName    = p_Packet.ReadString(l_MutePlayerNameLength);

    if (!normalizePlayerName(l_MutePlayerName))
        return;

    if (ChannelMgr * l_ChannelMgr = channelMgr(m_Player->GetTeam()))
    {
        if (Channel * l_Channel = l_ChannelMgr->GetChannel(l_ChannelName, m_Player))
            l_Channel->SetMute(m_Player->GetGUID(), l_MutePlayerName.c_str());
    }
}

void WorldSession::HandleChannelUnmute(WorldPacket & p_Packet)
{
    time_t l_Now = time(NULL);
    if (l_Now - m_TimeLastChannelUnmuteCommand < 5)
        return;
    else
       m_TimeLastChannelUnmuteCommand = l_Now;

    uint32 l_ChannelNameLength      = p_Packet.ReadBits(7);
    uint32 l_MutePlayerNameLength   = p_Packet.ReadBits(9);

    std::string l_ChannelName       = p_Packet.ReadString(l_ChannelNameLength);
    std::string l_MutePlayerName    = p_Packet.ReadString(l_MutePlayerNameLength);

    if (!normalizePlayerName(l_MutePlayerName))
        return;

    if (ChannelMgr * l_ChannelMgr = channelMgr(m_Player->GetTeam()))
    {
        if (Channel * l_Channel = l_ChannelMgr->GetChannel(l_ChannelName, m_Player))
            l_Channel->UnsetMute(m_Player->GetGUID(), l_MutePlayerName.c_str());
    }
}

void WorldSession::HandleChannelInvite(WorldPacket & p_Packet)
{
    time_t l_Now = time(NULL);
    if (l_Now - m_TimeLastChannelInviteCommand < 5)
        return;
    else
       m_TimeLastChannelInviteCommand = l_Now;

    uint32 l_ChannelNameLength  = p_Packet.ReadBits(7);
    uint32 l_PlayerNameLength   = p_Packet.ReadBits(9);

    std::string l_ChannelName   = p_Packet.ReadString(l_ChannelNameLength);
    std::string l_PlayerName    = p_Packet.ReadString(l_PlayerNameLength);

    if (!normalizePlayerName(l_PlayerName))
        return;

    if (!ChatHandler(this).isValidChatMessage(l_ChannelName.c_str()))
        return;

    if (ChannelMgr* l_ChannelMgr = channelMgr(m_Player->GetTeam()))
    {
        if (Channel* l_Channel = l_ChannelMgr->GetChannel(l_ChannelName, m_Player))
            l_Channel->Invite(m_Player->GetGUID(), l_PlayerName.c_str());
    }
}

void WorldSession::HandleChannelKick(WorldPacket& p_Packet)
{
    time_t l_Now = time(NULL);
    if (l_Now - m_TimeLastChannelKickCommand < 5)
        return;
    else
       m_TimeLastChannelKickCommand = l_Now;

    uint32 l_ChannelNameLength  = p_Packet.ReadBits(7);
    uint32 l_PlayerNameLength   = p_Packet.ReadBits(9);

    std::string l_ChannelName   = p_Packet.ReadString(l_ChannelNameLength);
    std::string l_PlayerName    = p_Packet.ReadString(l_PlayerNameLength);

    if (!normalizePlayerName(l_PlayerName))
        return;

    if (ChannelMgr * l_ChannelMgr = channelMgr(m_Player->GetTeam()))
    {
        if (Channel * l_Channel = l_ChannelMgr->GetChannel(l_ChannelName, m_Player))
            l_Channel->Kick(m_Player->GetGUID(), l_PlayerName.c_str());
    }
}

void WorldSession::HandleChannelBan(WorldPacket& p_Packet)
{
    time_t l_Now = time(NULL);
    if (l_Now - m_TimeLastChannelBanCommand < 5)
        return;
    else
       m_TimeLastChannelBanCommand = l_Now;

    uint32 l_ChannelNameLength  = p_Packet.ReadBits(7);
    uint32 l_PlayerNameLength   = p_Packet.ReadBits(9);

    std::string l_ChannelName   = p_Packet.ReadString(l_ChannelNameLength);
    std::string l_PlayerName    = p_Packet.ReadString(l_PlayerNameLength);

    if (!normalizePlayerName(l_PlayerName))
        return;

    if (ChannelMgr * l_ChannelMgr = channelMgr(m_Player->GetTeam()))
    {
        if (Channel * l_Channel = l_ChannelMgr->GetChannel(l_ChannelName, m_Player))
            l_Channel->Ban(m_Player->GetGUID(), l_PlayerName.c_str());
    }
}

void WorldSession::HandleChannelUnban(WorldPacket & p_Packet)
{
    time_t l_Now = time(NULL);
    if (l_Now - m_TimeLastChannelUnbanCommand < 5)
        return;
    else
       m_TimeLastChannelUnbanCommand = l_Now;

    uint32 l_ChannelNameLength  = p_Packet.ReadBits(7);
    uint32 l_PlayerNameLength   = p_Packet.ReadBits(9);

    std::string l_ChannelName   = p_Packet.ReadString(l_ChannelNameLength);
    std::string l_PlayerName    = p_Packet.ReadString(l_PlayerNameLength);

    if (!normalizePlayerName(l_PlayerName))
        return;

    if (ChannelMgr * l_ChannelMgr = channelMgr(m_Player->GetTeam()))
    {
        if (Channel * l_Channel = l_ChannelMgr->GetChannel(l_ChannelName, m_Player))
            l_Channel->UnBan(m_Player->GetGUID(), l_PlayerName.c_str());
    }
}

void WorldSession::HandleChannelAnnouncements(WorldPacket & p_Packet)
{
    time_t l_Now = time(NULL);

    if (l_Now - m_TimeLastChannelAnnounceCommand < 5)
        return;
    else
       m_TimeLastChannelAnnounceCommand = l_Now;

    std::string l_ChannelName = p_Packet.ReadString(p_Packet.ReadBits(7));

    if (ChannelMgr* l_ChannelMgr = channelMgr(m_Player->GetTeam()))
    {
        if (Channel* l_Channel = l_ChannelMgr->GetChannel(l_ChannelName, m_Player))
            l_Channel->Announce(m_Player->GetGUID());
    }
}

void WorldSession::HandleChannelDisplayListQuery(WorldPacket &recvPacket)
{
    // this should be OK because the 2 function _were_ the same
    HandleChannelList(recvPacket);
}
