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

void WorldSession::HandleGuildFinderAddRecruit(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_LF_GUILD_ADD_RECRUIT");

    if (sGuildFinderMgr->GetAllMembershipRequestsForPlayer(GetPlayer()->GetGUIDLow()).size() == 10)
        return;

    uint32 classRoles = 0;
    uint32 availability = 0;
    uint32 guildInterests = 0;

    recvPacket >> classRoles >> availability >> guildInterests;

    ObjectGuid guid;

    guid[1] = recvPacket.ReadBit();
    guid[5] = recvPacket.ReadBit();
    guid[2] = recvPacket.ReadBit();
    guid[7] = recvPacket.ReadBit();

    uint16 commentLength = recvPacket.ReadBits(10);

    guid[0] = recvPacket.ReadBit();
    guid[6] = recvPacket.ReadBit();
    guid[4] = recvPacket.ReadBit();
    guid[3] = recvPacket.ReadBit();

    recvPacket.FlushBits();

    uint8 bytesOrder[8] = { 2, 5, 3, 7, 1, 4, 0, 6 };
    recvPacket.ReadBytesSeq(guid, bytesOrder);

    std::string comment = recvPacket.ReadString(commentLength);

    uint32 guildLowGuid = GUID_LOPART(uint64(guid));

    if (!IS_GUILD_GUID(guid))
        return;
    if (!(classRoles & GUILDFINDER_ALL_ROLES) || classRoles > GUILDFINDER_ALL_ROLES)
        return;
    if (!(availability & ALL_WEEK) || availability > ALL_WEEK)
        return;
    if (!(guildInterests & ALL_INTERESTS) || guildInterests > ALL_INTERESTS)
        return;

    MembershipRequest request = MembershipRequest(GetPlayer()->GetGUIDLow(), guildLowGuid, availability, classRoles, guildInterests, comment, time(NULL));
    sGuildFinderMgr->AddMembershipRequest(guildLowGuid, request);
}

void WorldSession::HandleGuildFinderBrowse(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_LF_GUILD_BROWSE");

    uint32 classRoles = 0;
    uint32 availability = 0;
    uint32 guildInterests = 0;
    uint32 playerLevel = 0;
    uint8 lfgLevel = 0;

    recvPacket >> playerLevel >> guildInterests >> availability >> classRoles;

    Player* player = GetPlayer();

    if (playerLevel >= DEFAULT_MAX_LEVEL)
        lfgLevel = ALL_GUILDFINDER_LEVELS;
    else
        lfgLevel = ANY_FINDER_LEVEL;

    LFGuildPlayer settings(player->GetGUIDLow(), classRoles, availability, guildInterests, ANY_FINDER_LEVEL);
    LFGuildStore guildList = sGuildFinderMgr->GetGuildsMatchingSetting(settings, player->GetTeamId());
    uint32 guildCount = 0;

    for (LFGuildStore::const_iterator itr = guildList.begin(); itr != guildList.end(); ++itr)
    {
        LFGuildSettings guildSettings = itr->second;
        Guild* guild = sGuildMgr->GetGuildById(itr->first);

        if (guild != nullptr)
	    guildCount++;
    }


    if (guildCount == 0)
    {
        WorldPacket packet(SMSG_LFGUILD_BROWSE);
        packet.WriteBits(0, 18);
        player->SendDirectMessage(&packet);
        return;
    }

    bool returned = false;

    if (!(classRoles & GUILDFINDER_ALL_ROLES) || classRoles > GUILDFINDER_ALL_ROLES)
        returned = true;
    if (!(availability & ALL_WEEK) || availability > ALL_WEEK)
        returned = true;
    if (!(guildInterests & ALL_INTERESTS) || guildInterests > ALL_INTERESTS)
        returned = true;
    if (playerLevel > sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL) || playerLevel < 1)
        returned = true;

    if (returned)
    {
        WorldPacket packet(SMSG_LFGUILD_BROWSE);
        packet.WriteBits(0, 18);
        player->SendDirectMessage(&packet);
        return;
    }

    ByteBuffer bufferData;
    WorldPacket data(SMSG_LFGUILD_BROWSE);

    data.WriteBits(guildCount, 18);

    for (LFGuildStore::const_iterator itr = guildList.begin(); itr != guildList.end(); ++itr)
    {
        LFGuildSettings guildSettings = itr->second;
        Guild* guild = sGuildMgr->GetGuildById(itr->first);
        if (guild == nullptr)
            continue;

        ObjectGuid guildGUID = ObjectGuid(guild->GetGUID());

        data.WriteBit(guildGUID[4]);
        data.WriteBits(guild->GetName().size(), 7);
        data.WriteBit(guildGUID[0]);
        data.WriteBit(guildGUID[1]);
        data.WriteBits(guildSettings.GetComment().size(), 10);
        data.WriteBit(guildGUID[2]);
        data.WriteBit(guildGUID[7]);
        data.WriteBit(guildGUID[3]);
        data.WriteBit(guildGUID[6]);
        data.WriteBit(guildGUID[5]);

        bufferData << uint8(sGuildFinderMgr->HasRequest(player->GetGUIDLow(), guild->GetGUID()));
        bufferData << guild->GetLevel();
        bufferData << uint32(guildSettings.GetInterests());
        bufferData << uint32(0);                                                                    // Unk
        bufferData << uint32(guild->GetEmblemInfo().GetBorderStyle());
        bufferData.WriteByteSeq(guildGUID[1]);
        bufferData.WriteByteSeq(guildGUID[4]);
        bufferData << uint32(guild->GetEmblemInfo().GetStyle());
        bufferData.WriteByteSeq(guildGUID[3]);
        bufferData << uint32(50397223);                                                              // Unk Flags
        bufferData << uint32(guild->GetEmblemInfo().GetColor());

        if (guild->GetName().size() > 0)
            bufferData.append(guild->GetName().c_str(), guild->GetName().size());

        bufferData << uint32(guildSettings.GetClassRoles());
        bufferData << uint32(guildSettings.GetAvailability());

        if (guildSettings.GetComment().size() > 0)
            bufferData.append(guildSettings.GetComment().c_str(), guildSettings.GetComment().size());

        bufferData.WriteByteSeq(guildGUID[6]);
        bufferData.WriteByteSeq(guildGUID[7]);
        bufferData << uint8(0);                                                                     // Cached
        bufferData.WriteByteSeq(guildGUID[5]);
        bufferData.WriteByteSeq(guildGUID[0]);
        bufferData << uint32(guild->GetEmblemInfo().GetBorderColor());
        bufferData << uint32(guild->GetMembersCount());
        bufferData << uint32(guild->GetAchievementMgr().GetAchievementPoints());
        bufferData.WriteByteSeq(guildGUID[2]);
        bufferData << int32(guild->GetEmblemInfo().GetBackgroundColor());
    }

    data.FlushBits();
    data.append(bufferData);

    player->SendDirectMessage(&data);
}

void WorldSession::HandleGuildFinderDeclineRecruit(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_LF_GUILD_DECLINE_RECRUIT");

    ObjectGuid playerGuid;

    uint8 bitOrder[8] = { 3, 4, 0, 1, 6, 2, 5, 7 };
    recvPacket.ReadBitInOrder(playerGuid, bitOrder);

    recvPacket.FlushBits();

    uint8 byteOrder[8] = { 2, 6, 5, 1, 7, 0, 4, 3 };
    recvPacket.ReadBytesSeq(playerGuid, byteOrder);

    if (!IS_PLAYER_GUID(playerGuid))
        return;

    sGuildFinderMgr->RemoveMembershipRequest(GUID_LOPART(playerGuid), GetPlayer()->GetGuildId());
}

void WorldSession::HandleGuildFinderGetApplications(WorldPacket& /*recvPacket*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_LF_GUILD_GET_APPLICATIONS");

    std::list<MembershipRequest> applicatedGuilds = sGuildFinderMgr->GetAllMembershipRequestsForPlayer(GetPlayer()->GetGUIDLow());
    uint32 applicationsCount = applicatedGuilds.size();

    WorldPacket data(SMSG_LFGUILD_APPLICATIONS_LIST_CHANGED);

    data << uint32(10 - sGuildFinderMgr->CountRequestsFromPlayer(GetPlayer()->GetGUIDLow())); // Applications count left

    data.WriteBits(applicationsCount, 19);

    if (applicationsCount > 0)
    {
        ByteBuffer bufferData;
        for (std::list<MembershipRequest>::const_iterator itr = applicatedGuilds.begin(); itr != applicatedGuilds.end(); ++itr)
        {
            Guild* guild = sGuildMgr->GetGuildById(itr->GetGuildId());

            if (!guild)
                continue;

            LFGuildSettings guildSettings = sGuildFinderMgr->GetGuildSettings(itr->GetGuildId());
            MembershipRequest request = *itr;

            ObjectGuid guildGuid = ObjectGuid(guild->GetGUID());

            data.WriteBits(request.GetComment().size(), 10);
            data.WriteBits(guild->GetName().size(), 7);
            data.WriteBit(guildGuid[2]);
            data.WriteBit(guildGuid[1]);
            data.WriteBit(guildGuid[4]);
            data.WriteBit(guildGuid[0]);
            data.WriteBit(guildGuid[6]);
            data.WriteBit(guildGuid[3]);
            data.WriteBit(guildGuid[5]);
            data.WriteBit(guildGuid[7]);

            bufferData << uint32(50397223);                             // unk Flags
            bufferData.WriteByteSeq(guildGuid[1]);
            bufferData.WriteByteSeq(guildGuid[5]);
            bufferData.WriteByteSeq(guildGuid[6]);

            if (request.GetComment().size() > 0)
                bufferData.append(request.GetComment().c_str(), request.GetComment().size());

            bufferData.WriteByteSeq(guildGuid[0]);
            bufferData.WriteByteSeq(guildGuid[2]);
            bufferData << uint32(guildSettings.GetClassRoles());
            bufferData.WriteByteSeq(guildGuid[4]);
            bufferData << uint32(guildSettings.GetAvailability());

            if (guild->GetName().size() > 0)
                bufferData.append(guild->GetName().c_str(), guild->GetName().size());

            bufferData << uint32(time(NULL) - request.GetSubmitTime()); // Time since application (seconds)
            bufferData << uint32(guildSettings.GetInterests());
            bufferData << uint32(request.GetExpiryTime() - time(NULL)); // Time left to application expiry (seconds)
            bufferData.WriteByteSeq(guildGuid[7]);
            bufferData.WriteByteSeq(guildGuid[3]);
        }

        data.FlushBits();
        data.append(bufferData);
    }

    GetPlayer()->SendDirectMessage(&data);
}

// Lists all recruits for a guild - Misses times
void WorldSession::HandleGuildFinderGetRecruits(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_LF_GUILD_GET_RECRUITS");

    uint32 unkTime = 0;
    recvPacket >> unkTime;

    Player* player = GetPlayer();
    if (!player->GetGuildId())
        return;

    std::vector<MembershipRequest> recruitsList = sGuildFinderMgr->GetAllMembershipRequestsForGuild(player->GetGuildId());
    uint32 recruitCount = recruitsList.size();

    ByteBuffer dataBuffer;
    WorldPacket data(SMSG_LFGUILD_RECRUITS);

    data << uint32(time(NULL)); // Unk time
    data.WriteBits(recruitCount, 18);

    for (std::vector<MembershipRequest>::const_iterator itr = recruitsList.begin(); itr != recruitsList.end(); ++itr)
    {
        MembershipRequest request = *itr;
        ObjectGuid playerGuid(MAKE_NEW_GUID(request.GetPlayerGUID(), 0, HIGHGUID_PLAYER));

        data.WriteBits(request.GetComment().size(), 10);
        data.WriteBit(playerGuid[6]);
        data.WriteBit(playerGuid[3]);
        data.WriteBit(playerGuid[1]);
        data.WriteBit(playerGuid[4]);
        data.WriteBit(playerGuid[0]);
        data.WriteBit(playerGuid[5]);
        data.WriteBit(playerGuid[2]);
        data.WriteBits(request.GetName().size(), 6);
        data.WriteBit(playerGuid[7]);

        dataBuffer << uint32(request.GetClass());
        dataBuffer << uint32(request.GetLevel());
        dataBuffer << uint32(0);
        dataBuffer << uint32(time(NULL) <= request.GetExpiryTime());
        dataBuffer << uint32(request.GetAvailability());
        dataBuffer.WriteByteSeq(playerGuid[7]);
        dataBuffer << uint32(time(NULL) - request.GetSubmitTime()); // Time in seconds since application submitted.

        if (request.GetComment().size() > 0)
            dataBuffer.append(request.GetComment().c_str(), request.GetComment().size());

        dataBuffer.WriteByteSeq(playerGuid[6]);

        if (request.GetName().size() > 0)
            dataBuffer.append(request.GetName().c_str(), request.GetName().size());

        dataBuffer << uint32(request.GetClassRoles());
        dataBuffer.WriteByteSeq(playerGuid[1]);
        dataBuffer << uint32(request.GetInterests());
        dataBuffer << uint32(request.GetExpiryTime() - time(NULL)); // TIme in seconds until application expires.
        dataBuffer.WriteByteSeq(playerGuid[2]);
        dataBuffer.WriteByteSeq(playerGuid[5]);
        dataBuffer.WriteByteSeq(playerGuid[0]);
        dataBuffer.WriteByteSeq(playerGuid[4]);
        dataBuffer.WriteByteSeq(playerGuid[3]);
    }

    data.FlushBits();
    data.append(dataBuffer);

    player->SendDirectMessage(&data);
}

void WorldSession::HandleGuildFinderPostRequest(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_LF_GUILD_POST_REQUEST"); // Empty opcode

    Player* player = GetPlayer();

    if (!player->GetGuildId()) // Player must be in guild
        return;

    bool isGuildMaster = true;
    if (Guild* guild = sGuildMgr->GetGuildById(player->GetGuildId()))
        if (guild->GetLeaderGUID() != player->GetGUID())
            isGuildMaster = false;

    LFGuildSettings settings = sGuildFinderMgr->GetGuildSettings(player->GetGuildId());

    WorldPacket data(SMSG_LFGUILD_POST);
    data.WriteBit(isGuildMaster);

    if (isGuildMaster)
    {
        data.WriteBit(settings.IsListed());
        data.WriteBits(settings.GetComment().size(), 10);

        data << uint32(0); // Unk Int32

        if (settings.GetComment().size() > 0)
            data.append(settings.GetComment().c_str(), settings.GetComment().size());

        data << uint32(settings.GetAvailability());
        data << uint32(settings.GetClassRoles());
        data << uint32(settings.GetLevel());
        data << uint32(settings.GetInterests());
    }
    else
        data.FlushBits();

    player->SendDirectMessage(&data);
}

void WorldSession::HandleGuildFinderRemoveRecruit(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_LF_GUILD_REMOVE_RECRUIT");

    ObjectGuid guildGuid;

    uint8 bitOrder[8] = { 3, 1, 0, 7, 5, 4, 6, 2 };
    recvPacket.ReadBitInOrder(guildGuid, bitOrder);

    recvPacket.FlushBits();

    uint8 byteOrder[8] = { 2, 4, 0, 7, 6, 5, 1, 3 };
    recvPacket.ReadBytesSeq(guildGuid, byteOrder);

    if (!IS_GUILD_GUID(guildGuid))
        return;

    sGuildFinderMgr->RemoveMembershipRequest(GetPlayer()->GetGUIDLow(), GUID_LOPART(guildGuid));
}

// Sent any time a guild master sets an option in the interface and when listing / unlisting his guild
void WorldSession::HandleGuildFinderSetGuildPost(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_LF_GUILD_SET_GUILD_POST");

    uint32 classRoles = 0;
    uint32 availability = 0;
    uint32 guildInterests =  0;
    uint32 level = 0;

    recvPacket >> classRoles >> availability >> guildInterests >> level;
    bool listed = recvPacket.ReadBit();
    // Level sent is zero if untouched, force to any (from interface). Idk why
    if (!level)
        level = ANY_FINDER_LEVEL;

    uint16 length = recvPacket.ReadBits(10);
    recvPacket.FlushBits();
    std::string comment = recvPacket.ReadString(length);

    if (!(classRoles & GUILDFINDER_ALL_ROLES) || classRoles > GUILDFINDER_ALL_ROLES)
        return;
    if (!(availability & ALL_WEEK) || availability > ALL_WEEK)
        return;
    if (!(guildInterests & ALL_INTERESTS) || guildInterests > ALL_INTERESTS)
        return;
    if (!(level & ALL_GUILDFINDER_LEVELS) || level > ALL_GUILDFINDER_LEVELS)
        return;

    Player* player = GetPlayer();

    if (!player->GetGuildId()) // Player must be in guild
        return;

    if (Guild* guild = sGuildMgr->GetGuildById(player->GetGuildId())) // Player must be guild master
        if (guild->GetLeaderGUID() != player->GetGUID())
            return;

    LFGuildSettings settings(listed, player->GetTeamId(), player->GetGuildId(), classRoles, availability, guildInterests, level, comment);
    sGuildFinderMgr->SetGuildSettings(player->GetGuildId(), settings);
}
