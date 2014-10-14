/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#include "WorldSession.h"
#include "WorldPacket.h"
#include "Object.h"
#include "SharedDefines.h"
#include "GuildFinderMgr.h"
#include "GuildMgr.h"

void WorldSession::HandleGuildFinderAddRecruit(WorldPacket & p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_LF_GUILD_ADD_RECRUIT");

    if (sGuildFinderMgr->GetAllMembershipRequestsForPlayer(GetPlayer()->GetGUIDLow()).size() == 10)
        return;

    std::string l_Comment = "";

    uint64 l_GuildGUID = 0;

    uint32 l_PlayStyle      = 0;
    uint32 l_Availability   = 0;
    uint32 l_ClassRoles     = 0;
    uint32 l_CommentLength  = 0;

    p_Packet.readPackGUID(l_GuildGUID);
    p_Packet >> l_ClassRoles;
    p_Packet >> l_Availability;
    p_Packet >> l_PlayStyle;

    l_CommentLength = p_Packet.ReadBits(10);

    l_Comment = p_Packet.ReadString(l_CommentLength);

    uint32 l_GuildLowGuid = GUID_LOPART(uint64(l_GuildGUID));

    if (!IS_GUILD_GUID(l_GuildGUID))
        return;

    if (!(l_ClassRoles & GUILDFINDER_ALL_ROLES) || l_ClassRoles > GUILDFINDER_ALL_ROLES)
        return;

    if (!(l_Availability & ALL_WEEK) || l_Availability > ALL_WEEK)
        return;

    if (!(l_PlayStyle & ALL_INTERESTS) || l_PlayStyle > ALL_INTERESTS)
        return;

    MembershipRequest l_Request = MembershipRequest(GetPlayer()->GetGUIDLow(), l_GuildLowGuid, l_Availability, l_ClassRoles, l_PlayStyle, l_Comment, time(NULL));
    sGuildFinderMgr->AddMembershipRequest(l_GuildLowGuid, l_Request);
}

void WorldSession::HandleGuildFinderBrowse(WorldPacket & p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_LF_GUILD_BROWSE");

    uint32 l_ClassRoles = 0;
    uint32 l_Availability = 0;
    uint32 l_PlayStyle = 0;
    uint32 l_CharacterLevel = 0;
    uint8 l_LfgLevel = 0;

    p_Packet >> l_PlayStyle;
    p_Packet >> l_Availability;
    p_Packet >> l_ClassRoles;
    p_Packet >> l_CharacterLevel;

    if (l_CharacterLevel >= DEFAULT_MAX_LEVEL)
        l_LfgLevel = ALL_GUILDFINDER_LEVELS;
    else
        l_LfgLevel = ANY_FINDER_LEVEL;

    LFGuildPlayer l_Settings(m_Player->GetGUIDLow(), l_ClassRoles, l_Availability, l_PlayStyle, ANY_FINDER_LEVEL);

    LFGuildStore l_GuildList = sGuildFinderMgr->GetGuildsMatchingSetting(l_Settings, m_Player->GetTeamId());
    uint32       l_GuildCount = 0;

    for (LFGuildStore::const_iterator l_It = l_GuildList.begin(); l_It != l_GuildList.end(); ++l_It)
    {
        LFGuildSettings l_GuildSettings = l_It->second;
        Guild * l_Guild = sGuildMgr->GetGuildById(l_It->first);

        if (l_Guild != nullptr)
	        l_GuildCount++;
    }

    if (l_GuildCount == 0)
    {
        WorldPacket l_Response(SMSG_LFGUILD_BROWSE);
        l_Response << uint32(0);

        m_Player->SendDirectMessage(&l_Response);

        return;
    }

    bool returned = false;

    if (!(l_ClassRoles & GUILDFINDER_ALL_ROLES) || l_ClassRoles > GUILDFINDER_ALL_ROLES)
        returned = true;

    if (!(l_Availability & ALL_WEEK) || l_Availability > ALL_WEEK)
        returned = true;

    if (!(l_PlayStyle & ALL_INTERESTS) || l_PlayStyle > ALL_INTERESTS)
        returned = true;

    if (l_CharacterLevel > sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL) || l_CharacterLevel < 1)
        returned = true;

    if (returned)
    {
        WorldPacket l_Response(SMSG_LFGUILD_BROWSE);
        l_Response << uint32(0);

        m_Player->SendDirectMessage(&l_Response);

        return;
    }

    WorldPacket l_Response(SMSG_LFGUILD_BROWSE);
    l_Response << uint32(l_GuildCount);

    for (LFGuildStore::const_iterator l_It = l_GuildList.begin(); l_It != l_GuildList.end(); ++l_It)
    {
        LFGuildSettings l_GuildSettings = l_It->second;

        Guild * l_Guild = sGuildMgr->GetGuildById(l_It->first);

        if (l_Guild == nullptr)
            continue;

        uint64 l_GuildGUID = ObjectGuid(l_Guild->GetGUID());

        l_Response.WriteBits(l_Guild->GetName().size(), 7);                                                 ///< Guild Name
        l_Response.WriteBits(l_GuildSettings.GetComment().size(), 10);                                      ///< Comment
        l_Response.FlushBits();

        l_Response.appendPackGUID(l_GuildGUID);                                                             ///< Guild GUID

        l_Response << uint32(realmID);                                                                      ///< GuildVirtualRealm
        l_Response << uint32(l_Guild->GetMembersCount());                                                   ///< GuildMembers
        l_Response << uint32(l_Guild->GetAchievementMgr().GetAchievementPoints());                          ///< GuildAchievementPoints
        l_Response << uint32(l_GuildSettings.GetInterests());                                               ///< PlayStyle
        l_Response << uint32(l_GuildSettings.GetAvailability());                                            ///< Availability
        l_Response << uint32(l_GuildSettings.GetClassRoles());                                              ///< ClassRoles
        l_Response << uint32(l_GuildSettings.GetLevel());                                                   ///< LevelRange
        l_Response << uint32(l_Guild->GetEmblemInfo().GetStyle());                                          ///< EmblemStyle
        l_Response << uint32(l_Guild->GetEmblemInfo().GetColor());                                          ///< EmblemColor
        l_Response << uint32(l_Guild->GetEmblemInfo().GetBorderStyle());                                    ///< BorderStyle
        l_Response << uint32(l_Guild->GetEmblemInfo().GetBorderColor());                                    ///< BorderColor
        l_Response << int32(l_Guild->GetEmblemInfo().GetBackgroundColor());                                 ///< Background
        l_Response << uint8(0);                                                                             ///< Cached
        l_Response << uint8(sGuildFinderMgr->HasRequest(m_Player->GetGUIDLow(), l_Guild->GetGUID()));       ///< Membership Requested

        l_Response.WriteString(l_Guild->GetName());                                                         ///< Guild Name
        l_Response.WriteString(l_GuildSettings.GetComment());                                               ///< Comment
    }

    m_Player->SendDirectMessage(&l_Response);
}

void WorldSession::HandleGuildFinderDeclineRecruit(WorldPacket & p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_LF_GUILD_DECLINE_RECRUIT");

    uint64 l_RecruitGUID = 0;

    p_Packet.readPackGUID(l_RecruitGUID);

    if (!IS_PLAYER_GUID(l_RecruitGUID))
        return;

    sGuildFinderMgr->RemoveMembershipRequest(GUID_LOPART(l_RecruitGUID), GetPlayer()->GetGuildId());
}

void WorldSession::HandleGuildFinderGetApplications(WorldPacket & /*recvPacket*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_LF_GUILD_GET_APPLICATIONS");

    std::list<MembershipRequest> l_ApplicatedGuilds = sGuildFinderMgr->GetAllMembershipRequestsForPlayer(GetPlayer()->GetGUIDLow());

    WorldPacket l_Data(SMSG_LFGUILD_APPLICATIONS);

    l_Data << uint32(10 - sGuildFinderMgr->CountRequestsFromPlayer(m_Player->GetGUIDLow()));  ///< NumRemaining
    l_Data << uint32(l_ApplicatedGuilds.size());                                              ///< Application Count

    for (std::list<MembershipRequest>::const_iterator l_It = l_ApplicatedGuilds.begin(); l_It != l_ApplicatedGuilds.end(); ++l_It)
    {
        Guild * l_Guild = sGuildMgr->GetGuildById(l_It->GetGuildId());

        if (!l_Guild)
            continue;

        LFGuildSettings l_GuildSettings = sGuildFinderMgr->GetGuildSettings(l_It->GetGuildId());
        MembershipRequest l_Request = *l_It;

        l_Data.appendPackGUID(l_Guild->GetGUID());                    ///< GuildGUID

        l_Data << uint32(realmID);                                    ///< Guild Virtual Realm
        l_Data << uint32(l_GuildSettings.GetClassRoles());            ///< Class Roles
        l_Data << uint32(l_GuildSettings.GetInterests());             ///< Play Style
        l_Data << uint32(l_GuildSettings.GetAvailability());          ///< Availability
        l_Data << uint32(time(NULL) - l_Request.GetSubmitTime());     ///< Seconds Since Created
        l_Data << uint32(l_Request.GetExpiryTime() - time(NULL));     ///< Seconds Until Expiration

        l_Data.WriteBits(l_Guild->GetName().size(), 7);               ///< Guild Name
        l_Data.WriteBits(l_Request.GetComment().size(), 10);          ///< Comment
        l_Data.FlushBits();

        l_Data.WriteString(l_Guild->GetName());                       ///< Guild Name
        l_Data.WriteString(l_Request.GetComment());                   ///< Comment
    }

    m_Player->SendDirectMessage(&l_Data);
}

/// Lists all recruits for a guild - Misses times
void WorldSession::HandleGuildFinderGetRecruits(WorldPacket & p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_LF_GUILD_GET_RECRUITS");

    uint32 l_LastUpdate = 0;

    p_Packet >> l_LastUpdate;

    if (!m_Player->GetGuildId())
        return;

    std::vector<MembershipRequest> l_RecruitsList = sGuildFinderMgr->GetAllMembershipRequestsForGuild(m_Player->GetGuildId());

    WorldPacket l_Data(SMSG_LFGUILD_RECRUITS, 500);

    l_Data << uint32(l_RecruitsList.size());                            ///< Recruits Count
    l_Data << uint32(time(NULL));                                       ///< Update Time

    for (std::vector<MembershipRequest>::const_iterator l_It = l_RecruitsList.begin(); l_It != l_RecruitsList.end(); ++l_It)
    {
        MembershipRequest l_Request = *l_It;

        uint64 l_PlayerGUID(MAKE_NEW_GUID(l_Request.GetPlayerGUID(), 0, HIGHGUID_PLAYER));
        uint8  l_Gender = sObjectAccessor->FindPlayer(l_PlayerGUID) ? sObjectAccessor->FindPlayer(l_PlayerGUID)->getGender() : 0;

        l_Data.appendPackGUID(l_PlayerGUID);                            ///< Recruit GUID

        l_Data << uint32(realmID);                                      ///< Recruit Virtual Realm
        l_Data << uint32(l_Request.GetClass());                         ///< Character Class
        l_Data << uint32(l_Gender);                                     ///< Character Gender
        l_Data << uint32(l_Request.GetLevel());                         ///< Character Level
        l_Data << uint32(l_Request.GetClassRoles());                    ///< Class Roles
        l_Data << uint32(l_Request.GetInterests());                     ///< Play Style
        l_Data << uint32(l_Request.GetAvailability());                  ///< Availability
        l_Data << uint32(time(NULL) - l_Request.GetSubmitTime());       ///< Seconds Since Created
        l_Data << uint32(l_Request.GetExpiryTime() - time(NULL));       ///< Seconds Until Expiration

        l_Data.WriteBits(l_Request.GetName().size(), 6);                ///< Name
        l_Data.WriteBits(l_Request.GetComment().size(), 10);            ///< Comment
        l_Data.FlushBits();

        l_Data.WriteString(l_Request.GetName());                        ///< Name
        l_Data.WriteString(l_Request.GetComment());                     ///< Comment
    }

    m_Player->SendDirectMessage(&l_Data);
}

void WorldSession::HandleGuildFinderPostRequest(WorldPacket & p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_LF_GUILD_POST_REQUEST");
    
    /// Player must be in guild
    if (!m_Player->GetGuildId())
        return;

    bool l_IsGuildMaster = true;

    if (Guild * l_Guild = sGuildMgr->GetGuildById(m_Player->GetGuildId()))
    {
        if (l_Guild->GetLeaderGUID() != m_Player->GetGUID())
            l_IsGuildMaster = false;
    }

    LFGuildSettings l_Settings = sGuildFinderMgr->GetGuildSettings(m_Player->GetGuildId());

    WorldPacket l_Response(SMSG_LFGUILD_POST);
    l_Response.WriteBit(l_IsGuildMaster);                           ///< Has Post
    l_Response.FlushBits();

    if (l_IsGuildMaster)
    {
        l_Response.WriteBit(l_Settings.IsListed());                 ///< Active
        l_Response.WriteBits(l_Settings.GetComment().size(), 10);   ///< Comment
        l_Response.FlushBits();

        l_Response << uint32(l_Settings.GetInterests());            ///< Play Style
        l_Response << uint32(l_Settings.GetAvailability());         ///< Availability
        l_Response << uint32(l_Settings.GetClassRoles());           ///< Class Roles
        l_Response << uint32(l_Settings.GetLevel());                ///< Level Range
        l_Response << uint32(0);                                    ///< Seconds Remaining

        l_Response.WriteString(l_Settings.GetComment());            ///< Comment
    }
    else
        l_Response.FlushBits();

    m_Player->SendDirectMessage(&l_Response);
}

void WorldSession::HandleGuildFinderRemoveRecruit(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_LF_GUILD_REMOVE_RECRUIT");

    uint64 l_GuildGUID = 0;

    p_Packet.readPackGUID(l_GuildGUID);

    if (!IS_GUILD_GUID(l_GuildGUID))
        return;

    sGuildFinderMgr->RemoveMembershipRequest(GetPlayer()->GetGUIDLow(), GUID_LOPART(l_GuildGUID));
}

/// Sent any time a guild master sets an option in the interface and when listing / unlisting his guild
void WorldSession::HandleGuildFinderSetGuildPost(WorldPacket & p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_LF_GUILD_SET_GUILD_POST");

    std::string l_Comment = "";

    uint32 l_ClassRoles    = 0;
    uint32 l_Availability  = 0;
    uint32 l_PlayStyle     = 0;
    uint32 l_LevelRange    = 0;
    uint32 l_CommentLenght = 0;

    bool l_Active = false;

    p_Packet >> l_PlayStyle;
    p_Packet >> l_Availability;
    p_Packet >> l_ClassRoles;
    p_Packet >> l_LevelRange;

    l_Active        = p_Packet.ReadBit();
    l_CommentLenght = p_Packet.ReadBits(10);

    l_Comment = p_Packet.ReadString(l_CommentLenght);

    /// Level sent is zero if untouched, force to any (from interface). Idk why
    if (!l_LevelRange)
        l_LevelRange = ANY_FINDER_LEVEL;

    if (!(l_ClassRoles & GUILDFINDER_ALL_ROLES) || l_ClassRoles > GUILDFINDER_ALL_ROLES)
        return;
    if (!(l_Availability & ALL_WEEK) || l_Availability > ALL_WEEK)
        return;
    if (!(l_PlayStyle & ALL_INTERESTS) || l_PlayStyle > ALL_INTERESTS)
        return;
    if (!(l_LevelRange & ALL_GUILDFINDER_LEVELS) || l_LevelRange > ALL_GUILDFINDER_LEVELS)
        return;

    /// Player must be in guild
    if (!m_Player->GetGuildId())
        return;

    /// Player must be guild master
    if (Guild* l_Guild = sGuildMgr->GetGuildById(m_Player->GetGuildId()))
    {
        if (l_Guild->GetLeaderGUID() != m_Player->GetGUID())
            return;
    }

    LFGuildSettings settings(l_Active, m_Player->GetTeamId(), m_Player->GetGuildId(), l_ClassRoles, l_Availability, l_PlayStyle, l_LevelRange, l_Comment);
    sGuildFinderMgr->SetGuildSettings(m_Player->GetGuildId(), settings);
}
