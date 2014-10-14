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

/*
----- Opcodes Not Used yet -----

SMSG_CALENDAR_CLEAR_PENDING_ACTION SendCalendarClearPendingAction()
SMSG_CALENDAR_RAID_LOCKOUT_UPDATED SendCalendarRaidLockoutUpdated(InstanceSave const* save)

----- Opcodes without Sniffs -----
SMSG_CALENDAR_FILTER_GUILD              [ for (... uint32(count) { packguid(???), uint8(???) } ]
SMSG_CALENDAR_ARENA_TEAM                [ for (... uint32(count) { packguid(???), uint8(???) } ]
CMSG_CALENDAR_EVENT_INVITE_NOTES        [ packguid(Invitee), uint64(inviteId), string(Text), Boolean(Unk) ]
SMSG_CALENDAR_EVENT_INVITE_NOTES        [ uint32(unk1), uint32(unk2), uint32(unk3), uint32(unk4), uint32(unk5) ]
SMSG_CALENDAR_EVENT_INVITE_NOTES_ALERT  [ uint64(inviteId), string(Text) ]
SMSG_CALENDAR_EVENT_INVITE_STATUS_ALERT [ Structure unkown ]

*/

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include "InstanceSaveMgr.h"
#include "Log.h"
#include "Opcodes.h"
#include "Player.h"
#include "CalendarMgr.h"
#include "ObjectMgr.h"
#include "ObjectAccessor.h"
#include "DatabaseEnv.h"
#include "Guild.h"

void WorldSession::HandleCalendarGetCalendar(WorldPacket& /*recvData*/)
{
    uint64 guid = m_Player->GetGUID();
    time_t cur_time = time_t(time(NULL));

    ByteBuffer eventsBuffer;
    ByteBuffer invitesBuffer;
    ByteBuffer holidaysBuffer;
    ByteBuffer instancesBuffer;
    ByteBuffer raidsBuffer;
    WorldPacket data(SMSG_CALENDAR_SEND_CALENDAR);

    CalendarEventIdList const& events = sCalendarMgr->GetPlayerEvents(guid);

    data.WriteBits(events.size(), 19);

    for (CalendarEventIdList::const_iterator it = events.begin(); it != events.end(); ++it)
    {
        if (CalendarEvent* calendarEvent = sCalendarMgr->GetEvent(*it))
        {
            ObjectGuid creatorGuid = calendarEvent->GetCreatorGUID();
            ObjectGuid guildGuid = 0;
            if (calendarEvent->GetGuildId())
                guildGuid = MAKE_NEW_GUID(calendarEvent->GetGuildId(), 0, HIGHGUID_GUILD);
            std::string title = calendarEvent->GetTitle();

            data.WriteBits(title.size(), 8);
            data.WriteBit(creatorGuid[1]);
            data.WriteBit(guildGuid[7]);
            data.WriteBit(creatorGuid[0]);
            data.WriteBit(guildGuid[3]);
            data.WriteBit(guildGuid[6]);
            data.WriteBit(guildGuid[5]);
            data.WriteBit(creatorGuid[7]);
            data.WriteBit(creatorGuid[2]);
            data.WriteBit(creatorGuid[6]);
            data.WriteBit(guildGuid[0]);
            data.WriteBit(guildGuid[1]);
            data.WriteBit(guildGuid[2]);
            data.WriteBit(guildGuid[4]);
            data.WriteBit(creatorGuid[4]);
            data.WriteBit(creatorGuid[3]);
            data.WriteBit(creatorGuid[5]);

            eventsBuffer.WriteByteSeq(guildGuid[6]);
            eventsBuffer.WriteByteSeq(guildGuid[7]);
            eventsBuffer << uint32(calendarEvent->GetTime());
            eventsBuffer.WriteByteSeq(creatorGuid[1]);
            eventsBuffer.WriteByteSeq(guildGuid[2]);
            eventsBuffer.WriteByteSeq(creatorGuid[6]);
            eventsBuffer.WriteByteSeq(creatorGuid[4]);
            eventsBuffer.WriteByteSeq(creatorGuid[7]);
            eventsBuffer.WriteByteSeq(guildGuid[3]);
            eventsBuffer.WriteByteSeq(guildGuid[5]);
            eventsBuffer << uint32(calendarEvent->GetDungeonId());
            eventsBuffer.WriteByteSeq(guildGuid[1]);
            eventsBuffer.WriteByteSeq(guildGuid[0]);
            eventsBuffer << uint64(calendarEvent->GetEventId());
            eventsBuffer.WriteByteSeq(creatorGuid[5]);
            eventsBuffer.WriteByteSeq(guildGuid[4]);
            eventsBuffer.WriteByteSeq(creatorGuid[3]);
            eventsBuffer.WriteByteSeq(creatorGuid[0]);

            if (title.size())
                eventsBuffer.append(title.c_str(), title.size());

            eventsBuffer.WriteByteSeq(creatorGuid[2]);
            eventsBuffer << uint32(calendarEvent->GetFlags());
            eventsBuffer << uint8(calendarEvent->GetType());
        }
        else
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "SMSG_CALENDAR_SEND_CALENDAR: No Event found with id [" UI64FMTD "]", *it);

            data.WriteBits(0, 8);
            data.WriteBit(0);
            data.WriteBit(0);
            data.WriteBit(0);
            data.WriteBit(0);
            data.WriteBit(0);
            data.WriteBit(0);
            data.WriteBit(0);
            data.WriteBit(0);
            data.WriteBit(0);
            data.WriteBit(0);
            data.WriteBit(0);
            data.WriteBit(0);
            data.WriteBit(0);
            data.WriteBit(0);
            data.WriteBit(0);
            data.WriteBit(0);

            eventsBuffer << uint32(0);
            eventsBuffer << uint32(0);
            eventsBuffer << uint64(0);
            eventsBuffer << uint32(0);
            eventsBuffer << uint8(0);
        }
    }

    eventsBuffer << uint32(1135753200);                            // Unk Constant date (28.12.2005 07:00)
    eventsBuffer << uint32(secsToTimeBitFields(cur_time));         // server time

    CalendarInviteIdList const& invites = sCalendarMgr->GetPlayerInvites(guid);

    data.WriteBits(invites.size(), 19);

    for (CalendarInviteIdList::const_iterator it = invites.begin(); it != invites.end(); ++it)
    {
        CalendarInvite* invite = sCalendarMgr->GetInvite(*it);
        CalendarEvent* calendarEvent = invite ? sCalendarMgr->GetEvent(invite->GetEventId()) : NULL;

        if (calendarEvent)
        {
            ObjectGuid creatorGuid = invite->GetSenderGUID();

            uint8 bitsOrder[8] = { 1, 6, 2, 3, 5, 7, 4, 0 };
            data.WriteBitInOrder(creatorGuid, bitsOrder);

            invitesBuffer.WriteByteSeq(creatorGuid[7]);
            invitesBuffer << uint8(invite->GetStatus());
            invitesBuffer << uint8(invite->GetRank());
            invitesBuffer.WriteByteSeq(creatorGuid[6]);
            invitesBuffer.WriteByteSeq(creatorGuid[3]);
            invitesBuffer << uint64(invite->GetEventId());
            invitesBuffer.WriteByteSeq(creatorGuid[2]);
            invitesBuffer.WriteByteSeq(creatorGuid[5]);
            invitesBuffer.WriteByteSeq(creatorGuid[1]);
            invitesBuffer << uint8(calendarEvent ? calendarEvent->GetGuildId() : 0);
            invitesBuffer.WriteByteSeq(creatorGuid[4]);
            invitesBuffer.WriteByteSeq(creatorGuid[0]);
            invitesBuffer << uint64(invite->GetInviteId());
        }
        else
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "SMSG_CALENDAR_SEND_CALENDAR: No Invite found with id [" UI64FMTD "]", *it);

            ObjectGuid creatorGuid = invite ? invite->GetSenderGUID() : 0;

            uint8 bitsOrder[8] = { 1, 6, 2, 3, 5, 7, 4, 0 };
            data.WriteBitInOrder(creatorGuid, bitsOrder);

            invitesBuffer << uint8(invite ? invite->GetStatus() : 0);
            invitesBuffer << uint8(invite ? invite->GetRank() : 0);
            invitesBuffer << uint64(invite ? invite->GetEventId() : 0);
            invitesBuffer << uint8(0);
            invitesBuffer << uint64(invite ? invite->GetInviteId() : 0);
        }
    }

    invitesBuffer << uint32(time(NULL));

    uint16 raidCounter = 0;

    ResetTimeByMapDifficultyMap const& raidResets = sInstanceSaveMgr->GetResetTimeMap();
    for (ResetTimeByMapDifficultyMap::const_iterator itr = raidResets.begin(); itr != raidResets.end(); ++itr)
    {
        uint32 mapId = PAIR32_LOPART(itr->first);

        MapEntry const* mapEntry = sMapStore.LookupEntry(mapId);
        if (!mapEntry || !mapEntry->IsRaid())
            continue;

        ++raidCounter;
    }

    data.WriteBits(raidCounter, 20);
    data.WriteBits(/*sHolidaysStore.GetNumRows()*/0, 16);

    /*for (uint32 i = 0; i < sHolidaysStore.GetNumRows(); i++)
    {
        data.WriteBits(0, 6);   // Name length

        for (uint8 j = 0; j < 51; j++)
            holidaysBuffer << uint32(0);
    }*/

    uint32 instancesCounter = 0;

    for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
        for (Player::BoundInstancesMap::const_iterator itr = m_Player->m_boundInstances[i].begin(); itr != m_Player->m_boundInstances[i].end(); ++itr)
            if (itr->second.perm)
                ++instancesCounter;

    data.WriteBits(instancesCounter, 20);

    for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
    {
        for (Player::BoundInstancesMap::const_iterator itr = m_Player->m_boundInstances[i].begin(); itr != m_Player->m_boundInstances[i].end(); ++itr)
        {
            if (itr->second.perm)
            {
                InstanceSave* save = itr->second.save;
                ObjectGuid instanceGUID = MAKE_NEW_GUID(save->GetInstanceId(), 0, HIGHGUID_INSTANCE_SAVE);

                uint8 bitsOrder[8] = { 2, 7, 1, 6, 3, 5, 4, 0 };
                data.WriteBitInOrder(instanceGUID, bitsOrder);

                instancesBuffer << uint32(save->GetMapId());
                instancesBuffer.WriteByteSeq(instanceGUID[1]);
                instancesBuffer << uint32(save->GetDifficulty());
                instancesBuffer.WriteByteSeq(instanceGUID[4]);
                instancesBuffer.WriteByteSeq(instanceGUID[3]);
                instancesBuffer.WriteByteSeq(instanceGUID[7]);
                instancesBuffer << uint32(save->GetResetTime());
                instancesBuffer.WriteByteSeq(instanceGUID[6]);
                instancesBuffer.WriteByteSeq(instanceGUID[5]);
                instancesBuffer.WriteByteSeq(instanceGUID[0]);
                instancesBuffer.WriteByteSeq(instanceGUID[2]);
            }
        }
    }

    for (ResetTimeByMapDifficultyMap::const_iterator itr = raidResets.begin(); itr != raidResets.end(); ++itr)
    {
        uint32 mapId = PAIR32_LOPART(itr->first);

        MapEntry const* mapEntry = sMapStore.LookupEntry(mapId);
        if (!mapEntry || !mapEntry->IsRaid())
            continue;

        raidsBuffer << uint32(mapId);
        raidsBuffer << uint32(0);                       // Offset
        raidsBuffer << uint32(itr->second - cur_time);  // Duration
    }

    data.FlushBits();

    if (instancesBuffer.size())
        data.append(instancesBuffer);
    if (raidsBuffer.size())
        data.append(raidsBuffer);
    if (holidaysBuffer.size())
        data.append(holidaysBuffer);
    if (eventsBuffer.size())
        data.append(eventsBuffer);
    if (invitesBuffer.size())
        data.append(invitesBuffer);

    SendPacket(&data);
}

void WorldSession::HandleCalendarGetEvent(WorldPacket& p_RecvData)
{
    uint64 l_EventId;
    p_RecvData >> l_EventId;

    if (CalendarEvent* l_Event = sCalendarMgr->GetEvent(l_EventId))
        SendCalendarEvent(*l_Event, CALENDAR_SENDTYPE_GET);
}

void WorldSession::HandleCalendarGuildFilter(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_CALENDAR_GUILD_FILTER [" UI64FMTD "]", m_Player->GetGUID());

    int32 unk1, unk2, unk3;
    recvData >> unk1;
    recvData >> unk2;
    recvData >> unk3;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Calendar: CMSG_CALENDAR_GUILD_FILTER - unk1: %d unk2: %d unk3: %d", unk1, unk2, unk3);
}

void WorldSession::HandleCalendarArenaTeam(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_CALENDAR_ARENA_TEAM [" UI64FMTD "]", m_Player->GetGUID());

    int32 unk1;
    recvData >> unk1;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Calendar: CMSG_CALENDAR_ARENA_TEAM - unk1: %d", unk1);
}

void WorldSession::HandleCalendarAddEvent(WorldPacket& p_RecvData)
{
    uint64 l_Guid = m_Player->GetGUID();

    uint8 l_TitleSize, l_Type, l_Status, l_Rank;
    uint32 l_DescSize, l_Invites, l_Flags, l_EventTime, l_UnkTime, l_MaxInvites;
    std::string l_Title, l_Desc;

    // ??
    // bool repeatable;
    // int32 dungeonId;

    p_RecvData >> l_Flags;
    p_RecvData.ReadPackedTime(l_UnkTime);
    p_RecvData.ReadPackedTime(l_EventTime);
    p_RecvData >> l_Type;
    p_RecvData >> l_MaxInvites;
    p_RecvData >> l_TitleSize;

    CalendarAction l_Action;

    l_Action.SetAction(CALENDAR_ACTION_ADD_EVENT);
    l_Action.SetPlayer(m_Player);
    l_Action.Event.SetEventId(sCalendarMgr->GetFreeEventId());
    l_Action.Event.SetCreatorGUID(l_Guid);
    l_Action.Event.SetType(CalendarEventType(l_Type));
    l_Action.Event.SetFlags(l_Flags);
    l_Action.Event.SetTime(l_EventTime);
    l_Action.Event.SetTimeZoneTime(l_UnkTime);
    //l_Action.Event.SetRepeatable(repeatable);
    l_Action.Event.SetMaxInvites(l_MaxInvites);
    //l_Action.Event.SetDungeonId(dungeonId);
    l_Action.Event.SetGuildId((l_Flags & CALENDAR_FLAG_GUILD_ONLY) ? m_Player->GetGuildId() : 0);
    l_Action.Invite.SetEventId(l_Action.Event.GetEventId());
    l_Action.Invite.SetSenderGUID(l_Guid);

    l_Invites = p_RecvData.ReadBits(22);
    l_DescSize = p_RecvData.ReadBits(11);

    std::vector<ObjectGuid> l_InvitedGuids(l_Invites, ObjectGuid(0));
    for (uint32 i = 0; i < l_Invites; ++i)
    {
        uint8 bitsOrder[8] = { 0, 4, 5, 3, 6, 2, 7, 1 };
        p_RecvData.ReadBitInOrder(l_InvitedGuids[i], bitsOrder);
    }

    p_RecvData.FlushBits();

    for (uint32 i = 0; i < l_Invites; ++i)
    {
        p_RecvData.ReadByteSeq(l_InvitedGuids[i][1]);
        p_RecvData.ReadByteSeq(l_InvitedGuids[i][7]);
        p_RecvData >> l_Rank;
        p_RecvData.ReadByteSeq(l_InvitedGuids[i][4]);
        p_RecvData >> l_Status;
        p_RecvData.ReadByteSeq(l_InvitedGuids[i][6]);
        p_RecvData.ReadByteSeq(l_InvitedGuids[i][2]);
        p_RecvData.ReadByteSeq(l_InvitedGuids[i][3]);
        p_RecvData.ReadByteSeq(l_InvitedGuids[i][5]);
        p_RecvData.ReadByteSeq(l_InvitedGuids[i][0]);

        uint32 l_InviteId = sCalendarMgr->GetFreeInviteId();
        l_Action.Event.AddInvite(l_InviteId);
        l_Action.Invite.SetInviteId(l_InviteId);
        l_Action.Invite.SetInvitee(l_InvitedGuids[i]);
        l_Action.Invite.SetStatus(CalendarInviteStatus(l_Status));
        l_Action.Invite.SetRank(CalendarModerationRank(l_Rank));
    }

    l_Desc = p_RecvData.ReadString(l_DescSize);
    l_Title = p_RecvData.ReadString(l_TitleSize);

    l_Action.Event.SetTitle(l_Title);
    l_Action.Event.SetDescription(l_Desc);
    sCalendarMgr->AddAction(l_Action);
}

void WorldSession::HandleCalendarUpdateEvent(WorldPacket& p_RecvData)
{
    ObjectGuid l_InviterGuid, l_GuildGuid;
    uint64 l_DescSize;
    uint32 /*l_EventId, l_InviteId,*/ l_MaxInvits, l_Flags/*, l_DungeonId*/, l_EventTime, l_ZoneTime;
    std::string l_Title, l_Desc;
    uint8 l_Type, l_TitleSize;
    //bool repeatable;

    p_RecvData >> l_Flags;
    p_RecvData.ReadPackedTime(l_ZoneTime);
    p_RecvData.ReadPackedTime(l_EventTime);
    p_RecvData >> l_Type >> l_MaxInvits;

    l_InviterGuid[0] = p_RecvData.ReadBit();
    l_InviterGuid[5] = p_RecvData.ReadBit();
    l_GuildGuid[0] = p_RecvData.ReadBit();
    l_GuildGuid[5] = p_RecvData.ReadBit();
    l_GuildGuid[1] = p_RecvData.ReadBit();
    l_InviterGuid[4] = p_RecvData.ReadBit();
    l_InviterGuid[3] = p_RecvData.ReadBit();
    l_TitleSize = p_RecvData.ReadBits(8);
    l_InviterGuid[6] = p_RecvData.ReadBit();
    l_GuildGuid[3] = p_RecvData.ReadBit();
    l_InviterGuid[7] = p_RecvData.ReadBit();
    l_DescSize = p_RecvData.ReadBits(11);
    l_InviterGuid[1] = p_RecvData.ReadBit();
    l_GuildGuid[2] = p_RecvData.ReadBit();
    l_GuildGuid[7] = p_RecvData.ReadBit();
    l_GuildGuid[4] = p_RecvData.ReadBit();
    l_InviterGuid[2] = p_RecvData.ReadBit();
    l_GuildGuid[6] = p_RecvData.ReadBit();

    p_RecvData.FlushBits();

    p_RecvData.ReadByteSeq(l_GuildGuid[1]);
    p_RecvData.ReadByteSeq(l_GuildGuid[7]);
    p_RecvData.ReadByteSeq(l_InviterGuid[7]);
    p_RecvData.ReadByteSeq(l_GuildGuid[3]);
    p_RecvData.ReadByteSeq(l_InviterGuid[5]);
    p_RecvData.ReadByteSeq(l_GuildGuid[0]);
    p_RecvData.ReadByteSeq(l_GuildGuid[6]);
    p_RecvData.ReadByteSeq(l_InviterGuid[2]);
    l_Title = p_RecvData.ReadString(l_TitleSize);
    p_RecvData.ReadByteSeq(l_GuildGuid[2]);
    p_RecvData.ReadByteSeq(l_GuildGuid[5]);
    p_RecvData.ReadByteSeq(l_InviterGuid[0]);
    p_RecvData.ReadByteSeq(l_InviterGuid[6]);
    p_RecvData.ReadByteSeq(l_InviterGuid[1]);
    p_RecvData.ReadByteSeq(l_GuildGuid[4]);
    l_Desc = p_RecvData.ReadString(l_DescSize);
    p_RecvData.ReadByteSeq(l_InviterGuid[4]);
    p_RecvData.ReadByteSeq(l_InviterGuid[3]);

    CalendarAction l_Action;
    l_Action.SetAction(CALENDAR_ACTION_MODIFY_EVENT);
    l_Action.SetPlayer(m_Player);
    //l_Action.SetInviteId(l_InviteId);
    //l_Action.Event.SetEventId(l_EventId);
    l_Action.Event.SetType((CalendarEventType) l_Type);
    l_Action.Event.SetFlags((CalendarFlags) l_Flags);
    l_Action.Event.SetTime(l_EventTime);
    l_Action.Event.SetTimeZoneTime(l_ZoneTime);
    //l_Action.Event.SetRepeatable(repeatable);
    //l_Action.Event.SetDungeonId(l_DungeonId);
    l_Action.Event.SetTitle(l_Title);
    l_Action.Event.SetDescription(l_Desc);
    l_Action.Event.SetMaxInvites(l_MaxInvits);
    sCalendarMgr->AddAction(l_Action);
}

void WorldSession::HandleCalendarRemoveEvent(WorldPacket& p_RecvData)
{
    uint64 l_EventId, l_InviteId;
    uint32 l_Flags;

    p_RecvData >> l_EventId >> l_InviteId >> l_Flags;

    CalendarAction l_Action;
    l_Action.SetAction(CALENDAR_ACTION_REMOVE_EVENT);
    l_Action.SetPlayer(m_Player);
    l_Action.SetInviteId(l_InviteId);
    l_Action.Event.SetEventId(l_EventId);
    l_Action.Event.SetFlags((CalendarFlags)l_Flags);
    sCalendarMgr->AddAction(l_Action);
}

void WorldSession::HandleCalendarCopyEvent(WorldPacket& recvData)
{
    uint64 guid = m_Player->GetGUID();
    uint64 eventId;
    uint64 inviteId;
    uint32 time;

    recvData >> eventId >> inviteId >> time;
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_CALENDAR_COPY_EVENT [" UI64FMTD "], EventId [" UI64FMTD
        "] inviteId [" UI64FMTD "] Time: %u", guid, eventId, inviteId, time);

    CalendarAction action;
    action.SetAction(CALENDAR_ACTION_COPY_EVENT);
    action.SetPlayer(m_Player);
    action.SetInviteId(inviteId);
    action.Event.SetEventId(eventId);
    action.Event.SetTime(time);

    sCalendarMgr->AddAction(action);
}

void WorldSession::HandleCalendarEventInvite(WorldPacket& p_RecvData)
{
    uint64 l_EventId, l_InviteId, l_Invited;
    uint32 l_NameSize, l_Team;
    std::string l_Name;
    bool l_Creating, l_IsSignUp; // @TODO: Do something with that

    p_RecvData >> l_EventId >> l_InviteId;

    l_IsSignUp = p_RecvData.ReadBit();
    l_NameSize = p_RecvData.ReadBits(8) * 2;
    l_NameSize += p_RecvData.ReadBit();
    l_Creating = p_RecvData.ReadBit();

    p_RecvData.FlushBits();

    l_Name = p_RecvData.ReadString(l_NameSize);

    if (Player* l_Player = sObjectAccessor->FindPlayerByName(l_Name.c_str()))
    {
        l_Invited = l_Player->GetGUID();
        l_Team = l_Player->GetTeam();
    }
    else
    {
        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_RACE_ACC_BY_NAME);
        l_Stmt->setString(0, l_Name);
        if (PreparedQueryResult l_Result = CharacterDatabase.Query(l_Stmt))
        {
            Field* l_Fields = l_Result->Fetch();
            l_Invited = MAKE_NEW_GUID(l_Fields[0].GetUInt32(), 0, HIGHGUID_PLAYER);
            l_Team = Player::TeamForRace(l_Fields[1].GetUInt8());
        }
    }

    if (!l_Invited)
    {
        SendCalendarCommandResult(CALENDAR_ERROR_PLAYER_NOT_FOUND);
        return;
    }

    if (m_Player->GetTeam() != l_Team && !sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_CALENDAR))
    {
        SendCalendarCommandResult(CALENDAR_ERROR_NOT_ALLIED);
        return;
    }

    // TODO: Check ignore, even if offline (db query)
    CalendarAction l_Action;
    l_Action.SetAction(CALENDAR_ACTION_ADD_EVENT_INVITE);
    l_Action.SetPlayer(m_Player);
    l_Action.SetInviteId(l_InviteId);
    l_Action.Invite.SetEventId(l_EventId);
    l_Action.Invite.SetInviteId(sCalendarMgr->GetFreeInviteId());
    l_Action.Invite.SetSenderGUID(m_Player->GetGUID());
    l_Action.Invite.SetInvitee(l_Invited);
    sCalendarMgr->AddAction(l_Action);
}

void WorldSession::HandleCalendarEventSignup(WorldPacket& recvData)
{
    uint64 guid = m_Player->GetGUID();
    uint64 eventId;
    uint8 status;

    recvData >> eventId >> status;
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_CALENDAR_EVENT_SIGNUP [" UI64FMTD "] EventId ["
        UI64FMTD "] Status %u", guid, eventId, status);

    CalendarAction action;
    action.SetAction(CALENDAR_ACTION_SIGNUP_TO_EVENT);
    action.SetPlayer(m_Player);
    action.SetExtraData(GetPlayer()->GetGuildId());
    action.Event.SetEventId(eventId);
    action.Invite.SetStatus((CalendarInviteStatus) status);
    sCalendarMgr->AddAction(action);
}

void WorldSession::HandleCalendarEventRsvp(WorldPacket& recvData)
{
    uint64 guid = m_Player->GetGUID();
    uint64 eventId;
    uint64 inviteId;
    uint8 status;

    recvData >> eventId >> inviteId >> status;
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_CALENDAR_EVENT_RSVP [" UI64FMTD"] EventId ["
        UI64FMTD "], InviteId [" UI64FMTD "], status %u", guid, eventId,
        inviteId, status);

    CalendarAction action;
    action.SetAction(CALENDAR_ACTION_MODIFY_EVENT_INVITE);
    action.SetPlayer(m_Player);
    action.SetInviteId(inviteId);
    action.Invite.SetInviteId(inviteId);
    action.Invite.SetEventId(eventId);
    action.Invite.SetStatus((CalendarInviteStatus) status);

    sCalendarMgr->AddAction(action);
}

void WorldSession::HandleCalendarEventRemoveInvite(WorldPacket& recvData)
{
    uint64 guid = m_Player->GetGUID();
    uint64 invitee;
    uint64 eventId;
    uint64 owninviteId;
    uint64 inviteId;

    recvData.readPackGUID(invitee);
    recvData >> inviteId >> owninviteId >> eventId;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_CALENDAR_EVENT_REMOVE_INVITE ["
        UI64FMTD "] EventId [" UI64FMTD "], OwnInviteId ["
        UI64FMTD "], Invitee ([" UI64FMTD "] id: [" UI64FMTD "])",
        guid, eventId, owninviteId, invitee, inviteId);

    CalendarAction action;
    action.SetAction(CALENDAR_ACTION_REMOVE_EVENT_INVITE);
    action.SetPlayer(m_Player);
    action.SetInviteId(owninviteId);
    action.Invite.SetInviteId(inviteId);
    action.Invite.SetEventId(eventId);
    action.Invite.SetInvitee(invitee);

    sCalendarMgr->AddAction(action);
}

void WorldSession::HandleCalendarEventStatus(WorldPacket& recvData)
{
    uint64 guid = m_Player->GetGUID();
    uint64 invitee;
    uint64 eventId;
    uint64 inviteId;
    uint64 owninviteId;
    uint8 status;

    recvData.readPackGUID(invitee);
    recvData >> eventId >>  inviteId >> owninviteId >> status;
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_CALENDAR_EVENT_STATUS [" UI64FMTD"] EventId ["
        UI64FMTD "] OwnInviteId [" UI64FMTD "], Invitee ([" UI64FMTD "] id: ["
        UI64FMTD "], status %u", guid, eventId, owninviteId, invitee, inviteId, status);

    CalendarAction action;
    action.SetAction(CALENDAR_ACTION_MODIFY_EVENT_INVITE);
    action.SetPlayer(m_Player);
    action.SetInviteId(owninviteId);
    action.Invite.SetInviteId(inviteId);
    action.Invite.SetEventId(eventId);
    action.Invite.SetInvitee(invitee);
    action.Invite.SetStatus((CalendarInviteStatus) status);

    sCalendarMgr->AddAction(action);
}

void WorldSession::HandleCalendarEventModeratorStatus(WorldPacket& recvData)
{
    uint64 guid = m_Player->GetGUID();
    uint64 invitee;
    uint64 eventId;
    uint64 inviteId;
    uint64 owninviteId;
    uint8 status;

    recvData.readPackGUID(invitee);
    recvData >> eventId >>  inviteId >> owninviteId >> status;
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_CALENDAR_EVENT_MODERATOR_STATUS [" UI64FMTD "] EventId ["
        UI64FMTD "] OwnInviteId [" UI64FMTD "], Invitee ([" UI64FMTD "] id: ["
        UI64FMTD "], status %u", guid, eventId, owninviteId, invitee, inviteId, status);

    CalendarAction action;
    action.SetAction(CALENDAR_ACTION_MODIFY_MODERATOR_EVENT_INVITE);
    action.SetPlayer(m_Player);
    action.SetInviteId(owninviteId);
    action.Invite.SetInviteId(inviteId);
    action.Invite.SetEventId(eventId);
    action.Invite.SetInvitee(invitee);
    action.Invite.SetStatus((CalendarInviteStatus) status);

    sCalendarMgr->AddAction(action);
}

void WorldSession::HandleCalendarComplain(WorldPacket& recvData)
{
    uint64 guid = m_Player->GetGUID();
    uint64 eventId;
    uint64 complainGUID;

    recvData >> eventId >> complainGUID;
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_CALENDAR_COMPLAIN [" UI64FMTD "] EventId ["
        UI64FMTD "] guid [" UI64FMTD "]", guid, eventId, complainGUID);
}

void WorldSession::HandleCalendarGetNumPending(WorldPacket& /*recvData*/)
{
    uint64 guid = m_Player->GetGUID();
    uint32 pending = sCalendarMgr->GetPlayerNumPending(guid);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_CALENDAR_GET_NUM_PENDING: [" UI64FMTD
        "] Pending: %u", guid, pending);

    WorldPacket data(SMSG_CALENDAR_SEND_NUM_PENDING, 4);
    data << uint32(pending);
    SendPacket(&data);
}

// ----------------------------------- SEND ------------------------------------
void WorldSession::SendCalendarEvent(CalendarEvent const& p_Event, CalendarSendEventType p_SendType)
{
    WorldPacket l_Data(SMSG_CALENDAR_SEND_EVENT);
    CalendarInviteIdList const& l_Invites = p_Event.GetInviteIdList();
    ObjectGuid l_GuildGuid = m_Player->GetGuild() ? m_Player->GetGuild()->GetGUID() : 0;
    ObjectGuid l_InviterGuid = p_Event.GetCreatorGUID();
    ByteBuffer l_DataBuffer;

    l_Data << uint32(secsToTimeBitFields(p_Event.GetTimeZoneTime()));
    l_Data << uint8(p_Event.GetType());
    l_Data << uint32(p_Event.GetMaxInvites());
    l_Data << uint32(p_Event.GetFlags());
    l_Data << uint32(secsToTimeBitFields(p_Event.GetTime()));
    l_Data << uint8(p_SendType);
    l_Data << uint64(p_Event.GetEventId());

    l_Data.WriteBit(l_GuildGuid[1]);
    l_Data.WriteBits(p_Event.GetTitle().size(), 8);
    l_Data.WriteBits(l_Invites.size(), 20);
    l_Data.WriteBit(l_GuildGuid[5]);
    l_Data.WriteBit(l_InviterGuid[0]);

    for (CalendarInviteIdList::const_iterator it = l_Invites.begin(); it != l_Invites.end(); ++it)
    {
        if (CalendarInvite* l_Invite = sCalendarMgr->GetInvite(*it))
        {
            ObjectGuid l_InvitedGuid = l_Invite->GetInvitee();
            Player* l_Player = ObjectAccessor::FindPlayer(l_InvitedGuid);
            uint8 l_Level = l_Player ? l_Player->getLevel() : Player::GetLevelFromDB(l_InvitedGuid);

            l_Data.WriteBit(l_InvitedGuid[4]);
            l_Data.WriteBit(l_InvitedGuid[2]);
            l_Data.WriteBit(l_InvitedGuid[7]);
            l_Data.WriteBit(l_InvitedGuid[0]);
            l_Data.WriteBit(l_InvitedGuid[1]);
            l_Data.WriteBit(l_InvitedGuid[6]);
            l_Data.WriteBit(l_InvitedGuid[5]);
            l_Data.WriteBits(l_Invite->GetText().size(), 8);
            l_Data.WriteBit(l_InvitedGuid[3]);

            l_DataBuffer << uint8(l_Level);
            l_DataBuffer << uint8(l_Invite->GetStatus());
            l_DataBuffer << uint8(l_Invite->GetRank());
            l_DataBuffer << uint8(p_Event.GetGuildId() != 0);
            l_DataBuffer.WriteByteSeq(l_InvitedGuid[4]);
            l_DataBuffer.WriteByteSeq(l_InvitedGuid[7]);
            l_DataBuffer.WriteByteSeq(l_InvitedGuid[1]);
            l_DataBuffer.WriteByteSeq(l_InvitedGuid[6]);
            l_DataBuffer.WriteByteSeq(l_InvitedGuid[3]);
            l_DataBuffer << uint64(l_Invite->GetInviteId());
            l_DataBuffer.WriteByteSeq(l_InvitedGuid[5]);

            if (l_Invite->GetText().size())
                l_DataBuffer.append(l_Invite->GetText().c_str(), l_Invite->GetText().size());

            l_DataBuffer.WriteByteSeq(l_InvitedGuid[2]);
            l_DataBuffer.WriteByteSeq(l_InvitedGuid[0]);
            l_DataBuffer << uint32(secsToTimeBitFields(l_Invite->GetStatusTime()));
        }
        else
        {
            l_Data.WriteBits(0, 16);

            l_DataBuffer << uint8(0);
            l_DataBuffer << uint8(0);
            l_DataBuffer << uint8(0);
            l_DataBuffer << uint8(0);
            l_DataBuffer << uint64(0);
            l_DataBuffer << uint32(0);
        }
    }

    l_Data.WriteBit(l_InviterGuid[3]);
    l_Data.WriteBit(l_GuildGuid[3]);
    l_Data.WriteBit(l_GuildGuid[0]);
    l_Data.WriteBit(l_InviterGuid[6]);
    l_Data.WriteBit(l_InviterGuid[7]);
    l_Data.WriteBit(l_GuildGuid[2]);
    l_Data.WriteBit(l_GuildGuid[4]);
    l_Data.WriteBit(l_InviterGuid[5]);
    l_Data.WriteBit(l_InviterGuid[4]);
    l_Data.WriteBits(p_Event.GetDescription().size(), 11);
    l_Data.WriteBit(l_GuildGuid[7]);
    l_Data.WriteBit(l_InviterGuid[1]);
    l_Data.WriteBit(l_GuildGuid[6]);
    l_Data.WriteBit(l_InviterGuid[2]);

    l_Data.FlushBits();

    if (l_DataBuffer.size())
        l_Data.append(l_DataBuffer);

    l_Data.WriteByteSeq(l_GuildGuid[2]);
    l_Data.WriteByteSeq(l_GuildGuid[6]);
    l_Data.WriteByteSeq(l_GuildGuid[5]);
    l_Data.WriteByteSeq(l_GuildGuid[1]);
    l_Data.WriteByteSeq(l_InviterGuid[1]);
    l_Data.WriteByteSeq(l_InviterGuid[5]);

    if (p_Event.GetDescription().size())
        l_Data.append(p_Event.GetDescription().c_str(), p_Event.GetDescription().size());

    l_Data.WriteByteSeq(l_InviterGuid[6]);
    l_Data.WriteByteSeq(l_InviterGuid[0]);
    l_Data.WriteByteSeq(l_InviterGuid[2]);
    l_Data.WriteByteSeq(l_InviterGuid[4]);

    if (p_Event.GetTitle().size())
        l_Data.append(p_Event.GetTitle().c_str(), p_Event.GetTitle().size());

    l_Data.WriteByteSeq(l_GuildGuid[0]);
    l_Data.WriteByteSeq(l_InviterGuid[7]);
    l_Data.WriteByteSeq(l_GuildGuid[7]);
    l_Data.WriteByteSeq(l_InviterGuid[3]);
    l_Data.WriteByteSeq(l_GuildGuid[3]);
    l_Data.WriteByteSeq(l_GuildGuid[4]);

    SendPacket(&l_Data);
}

void WorldSession::SendCalendarEventInvite(CalendarInvite const& invite, bool pending)
{
    uint64 guid = m_Player->GetGUID();
    uint64 eventId = invite.GetEventId();
    uint64 inviteId = invite.GetInviteId();
    uint64 invitee = invite.GetInvitee();
    uint8 status = invite.GetStatus();
    uint32 statusTime = invite.GetStatusTime();
    Player* player = ObjectAccessor::FindPlayer(invitee);
    uint8 level = player ? player->getLevel() : Player::GetLevelFromDB(invitee);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_CALENDAR_EVENT_INVITE [" UI64FMTD "] EventId ["
        UI64FMTD "] InviteId [" UI64FMTD "] Invitee [" UI64FMTD "] "
        " Level %u, Status %u, StatusTime %u" , guid, eventId, inviteId,
        invitee, level, status, statusTime);

    WorldPacket data(SMSG_CALENDAR_EVENT_INVITE, 8 + 8 + 8 + 1 + 1 + 1 + (statusTime ? 4 : 0) + 1);
    data.appendPackGUID(invitee);
    data << uint64(eventId);
    data << uint64(inviteId);
    data << uint8(level);
    data << uint8(status);
    if (statusTime)
        data << uint8(1) << uint32(statusTime);
    else
        data << uint8(0);
    data << uint8(pending);

    SendPacket(&data);
}

void WorldSession::SendCalendarEventInviteAlert(CalendarEvent const& p_Event, CalendarInvite const& p_Invite)
{
    WorldPacket l_Data(SMSG_CALENDAR_EVENT_INVITE_ALERT);
    ObjectGuid l_Guid = m_Player->GetGUID();
    ObjectGuid l_GuildGuid = m_Player->GetGuild() ? m_Player->GetGuild()->GetGUID() : 0;
    ObjectGuid l_SenderGuid = p_Invite.GetSenderGUID();

    l_Data << uint32(p_Event.GetFlags());
    l_Data << uint64(p_Event.GetEventId());
    l_Data << uint32(p_Event.GetDungeonId());
    l_Data << uint32(secsToTimeBitFields(p_Event.GetTime()));
    l_Data << uint64(p_Invite.GetInviteId());
    l_Data << uint8(p_Invite.GetStatus());
    l_Data << uint8(p_Event.GetType());
    l_Data << uint8(p_Invite.GetRank());

    l_Data.WriteBit(l_GuildGuid[0]);
    l_Data.WriteBit(l_Guid[3]);
    l_Data.WriteBit(l_GuildGuid[2]);
    l_Data.WriteBit(l_SenderGuid[0]);
    l_Data.WriteBit(l_Guid[6]);
    l_Data.WriteBit(l_SenderGuid[3]);
    l_Data.WriteBit(l_Guid[2]);
    l_Data.WriteBit(l_GuildGuid[7]);
    l_Data.WriteBit(l_Guid[1]);
    l_Data.WriteBit(l_SenderGuid[6]);
    l_Data.WriteBit(l_Guid[0]);
    l_Data.WriteBit(l_SenderGuid[5]);
    l_Data.WriteBit(l_SenderGuid[4]);
    l_Data.WriteBit(l_GuildGuid[5]);
    l_Data.WriteBit(l_SenderGuid[1]);
    l_Data.WriteBit(l_GuildGuid[1]);
    l_Data.WriteBit(l_GuildGuid[6]);
    l_Data.WriteBit(l_SenderGuid[2]);
    l_Data.WriteBit(l_Guid[5]);
    l_Data.WriteBit(l_SenderGuid[7]);
    l_Data.WriteBits(p_Event.GetTitle().size(), 8);
    l_Data.WriteBit(l_GuildGuid[4]);
    l_Data.WriteBit(l_Guid[7]);
    l_Data.WriteBit(l_GuildGuid[3]);
    l_Data.WriteBit(l_Guid[4]);

    l_Data.FlushBits();

    l_Data.WriteByteSeq(l_Guid[4]);
    l_Data.WriteByteSeq(l_GuildGuid[3]);
    l_Data.WriteByteSeq(l_Guid[1]);

    if (p_Event.GetTitle().size())
        l_Data.append(p_Event.GetTitle().c_str(), p_Event.GetTitle().size());

    l_Data.WriteByteSeq(l_SenderGuid[6]);
    l_Data.WriteByteSeq(l_SenderGuid[2]);
    l_Data.WriteByteSeq(l_GuildGuid[7]);
    l_Data.WriteByteSeq(l_GuildGuid[5]);
    l_Data.WriteByteSeq(l_Guid[2]);
    l_Data.WriteByteSeq(l_SenderGuid[5]);
    l_Data.WriteByteSeq(l_Guid[6]);
    l_Data.WriteByteSeq(l_SenderGuid[4]);
    l_Data.WriteByteSeq(l_GuildGuid[1]);
    l_Data.WriteByteSeq(l_SenderGuid[7]);
    l_Data.WriteByteSeq(l_Guid[0]);
    l_Data.WriteByteSeq(l_Guid[3]);
    l_Data.WriteByteSeq(l_GuildGuid[4]);
    l_Data.WriteByteSeq(l_GuildGuid[6]);
    l_Data.WriteByteSeq(l_GuildGuid[0]);
    l_Data.WriteByteSeq(l_SenderGuid[3]);
    l_Data.WriteByteSeq(l_Guid[7]);
    l_Data.WriteByteSeq(l_SenderGuid[0]);
    l_Data.WriteByteSeq(l_Guid[5]);
    l_Data.WriteByteSeq(l_GuildGuid[2]);
    l_Data.WriteByteSeq(l_SenderGuid[1]);

    SendPacket(&l_Data);
}

void WorldSession::SendCalendarEventUpdateAlert(CalendarEvent const& calendarEvent, CalendarSendEventType sendEventType)
{
    uint64 guid = m_Player->GetGUID();
    uint64 eventId = calendarEvent.GetEventId();

    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_CALENDAR_EVENT_UPDATED_ALERT ["
        UI64FMTD "] EventId [" UI64FMTD "]", guid, eventId);


    WorldPacket data(SMSG_CALENDAR_EVENT_UPDATED_ALERT, 1 + 8 + 4 + 4 + 4 + 1 + 4 +
        calendarEvent.GetTitle().size() + calendarEvent.GetDescription().size() + 1 + 4 + 4);
    data << uint8(sendEventType);
    data << uint64(eventId);
    data << uint32(calendarEvent.GetTime());
    data << uint32(calendarEvent.GetFlags());
    data << uint32(calendarEvent.GetTime());
    data << uint8(calendarEvent.GetType());
    data << uint32(calendarEvent.GetDungeonId());
    data << calendarEvent.GetTitle().c_str();
    data << calendarEvent.GetDescription().c_str();
    data << uint8(calendarEvent.GetRepeatable());
    data << uint32(calendarEvent.GetMaxInvites());
    data << uint32(0); // FIXME
    SendPacket(&data);
}

void WorldSession::SendCalendarEventRemovedAlert(CalendarEvent const& p_Event)
{
    WorldPacket l_Data(SMSG_CALENDAR_EVENT_REMOVED_ALERT, 1 + 8 + 1);
    l_Data.WriteBit(true);
    l_Data.FlushBits();
    l_Data << uint64(p_Event.GetEventId());
    l_Data << uint32(secsToTimeBitFields(p_Event.GetTime()));
    SendPacket(&l_Data);
}

void WorldSession::SendCalendarEventStatus(CalendarEvent const& calendarEvent, CalendarInvite const& invite)
{
    uint64 guid = m_Player->GetGUID();
    uint64 eventId = calendarEvent.GetEventId();
    uint64 inviteId = invite.GetInviteId();
    uint64 invitee = invite.GetInvitee();
    uint32 eventTime = (calendarEvent.GetTime());
    uint32 flags = calendarEvent.GetFlags();
    uint8 status = invite.GetStatus();
    uint8 rank = invite.GetRank();
    uint32 statusTime = secsToTimeBitFields(invite.GetStatusTime());


    sLog->outDebug(LOG_FILTER_NETWORKIO, "SMSG_CALENDAR_EVENT_STATUS [" UI64FMTD "] EventId ["
        UI64FMTD "] InviteId [" UI64FMTD "] Invitee [" UI64FMTD "] Time %u "
        "Flags %u, Status %u, Rank %u, StatusTime %u",
        guid, eventId, inviteId, invitee, eventTime, flags, status, rank,
        statusTime);

    WorldPacket data(SMSG_CALENDAR_EVENT_STATUS, 8 + 8 + 4 + 4 + 1 + 1 + 4);
    data.appendPackGUID(invitee);
    data << uint64(eventId);
    data << uint32(eventTime);
    data << uint32(flags);
    data << uint8(status);
    data << uint8(rank);
    data << uint32(statusTime);
    SendPacket(&data);
}

void WorldSession::SendCalendarEventModeratorStatusAlert(CalendarInvite const& p_Invite)
{
    uint64 eventId = p_Invite.GetEventId();
    uint64 invitee = p_Invite.GetInvitee();
    uint8 status = p_Invite.GetStatus();

    WorldPacket l_Data(SMSG_CALENDAR_EVENT_MODERATOR_STATUS_ALERT, 8 + 8 + 1 + 1);
    l_Data.appendPackGUID(invitee);
    l_Data << uint64(eventId);
    l_Data << uint8(status);
    l_Data << uint8(1); // FIXME
    SendPacket(&l_Data);
}

void WorldSession::SendCalendarEventInviteRemoveAlert(CalendarEvent const& p_Event, CalendarInviteStatus p_Status)
{
    WorldPacket l_Data(SMSG_CALENDAR_EVENT_INVITE_REMOVED_ALERT, 8 + 4 + 4 + 1);
    l_Data << uint8(p_Status);
    l_Data << uint32(secsToTimeBitFields(p_Event.GetTime()));
    l_Data << uint64(p_Event.GetEventId());
    l_Data << uint32(p_Event.GetFlags());
    SendPacket(&l_Data);
}

void WorldSession::SendCalendarEventInviteRemove(CalendarInvite const& p_Invite, uint32 p_Flags)
{
    ObjectGuid l_InviteGuid = p_Invite.GetInvitee();
    WorldPacket l_Data(SMSG_CALENDAR_EVENT_INVITE_REMOVED, 8 + 4 + 4 + 1);
    l_Data.WriteBit(true);
    l_Data.FlushBits();

    uint8 l_BitsOrder[8] = { 3, 1, 0, 7, 4, 5, 6, 2 };
    l_Data.WriteBitInOrder(l_InviteGuid, l_BitsOrder);

    l_Data << uint32(p_Flags);

    uint8 l_BytesOrder[8] = { 4, 3, 0, 2, 6, 7, 5, 1 };
    l_Data.WriteBytesSeq(l_InviteGuid, l_BytesOrder);

    l_Data << uint64(p_Invite.GetEventId());

    SendPacket(&l_Data);
}

void WorldSession::SendCalendarClearPendingAction()
{
    WorldPacket data(SMSG_CALENDAR_CLEAR_PENDING_ACTION, 0);
    SendPacket(&data);
}

void WorldSession::SendCalendarCommandResult(CalendarError p_Error, char const* p_Param /*= ""*/)
{
    WorldPacket l_Data(SMSG_CALENDAR_COMMAND_RESULT, 0);
    uint32 l_ParamSize = strlen(p_Param);

    l_Data << uint8(p_Error);
    l_Data << uint8(0); // result

    bool l_Modulo = (l_ParamSize % 2) != 0;
    uint8 l_ClientSize = l_ParamSize - l_Modulo;

    l_Data << uint8(l_ClientSize / 2);
    l_Data.WriteBit(l_Modulo);
    l_Data.FlushBits();

    if (l_ParamSize)
        l_Data.append(p_Param, l_ParamSize);

    SendPacket(&l_Data);
}

void WorldSession::SendCalendarRaidLockout(InstanceSave const* p_Save, bool p_Add)
{
    time_t l_CurrTime = time(NULL);
    ObjectGuid l_InstanceGuid = MAKE_NEW_GUID(p_Save->GetInstanceId(), 0, HIGHGUID_INSTANCE_SAVE);

    WorldPacket l_Data;
    if (p_Add)
    {
        l_Data.Initialize(SMSG_CALENDAR_RAID_LOCKOUT_ADDED);

        uint8 bitsOrder[8] = { 4, 3, 2, 1, 7, 0, 6, 5 };
        l_Data.WriteBitInOrder(l_InstanceGuid, bitsOrder);

        l_Data.WriteByteSeq(l_InstanceGuid[2]);
        l_Data.WriteByteSeq(l_InstanceGuid[1]);
        l_Data.WriteByteSeq(l_InstanceGuid[3]);
        l_Data << uint32(secsToTimeBitFields(l_CurrTime));
        l_Data.WriteByteSeq(l_InstanceGuid[0]);
        l_Data.WriteByteSeq(l_InstanceGuid[5]);
        l_Data << uint32(p_Save->GetMapId());
        l_Data << uint32(p_Save->GetDifficulty());
        l_Data.WriteByteSeq(l_InstanceGuid[7]);
        l_Data.WriteByteSeq(l_InstanceGuid[4]);
        l_Data << uint32(p_Save->GetResetTime() - l_CurrTime);
        l_Data.WriteByteSeq(l_InstanceGuid[6]);
    }
    else
    {
        l_Data.Initialize(SMSG_CALENDAR_RAID_LOCKOUT_REMOVED);

        uint8 bitsOrder[8] = { 6, 0, 7, 3, 5, 1, 2, 4 };
        l_Data.WriteBitInOrder(l_InstanceGuid, bitsOrder);

        l_Data.WriteByteSeq(l_InstanceGuid[0]);
        l_Data.WriteByteSeq(l_InstanceGuid[5]);
        l_Data.WriteByteSeq(l_InstanceGuid[3]);
        l_Data.WriteByteSeq(l_InstanceGuid[1]);
        l_Data.WriteByteSeq(l_InstanceGuid[2]);
        l_Data << uint32(p_Save->GetMapId());
        l_Data.WriteByteSeq(l_InstanceGuid[6]);
        l_Data << uint32(p_Save->GetResetTime() - l_CurrTime);
        l_Data.WriteByteSeq(l_InstanceGuid[4]);
        l_Data.WriteByteSeq(l_InstanceGuid[7]);
    }

    SendPacket(&l_Data);
}

void WorldSession::SendCalendarRaidLockoutUpdated(InstanceSave const* p_Save)
{
    if (!p_Save)
        return;

    time_t l_CurrTime = time_t(time(NULL));

    WorldPacket l_Data(SMSG_CALENDAR_RAID_LOCKOUT_UPDATED, 4 + 4 + 4 + 4 + 8);
    l_Data << uint32(p_Save->GetResetTime() - l_CurrTime);  // New time remaining
    l_Data << uint32(p_Save->GetDifficulty());
    l_Data << secsToTimeBitFields(l_CurrTime);
    l_Data << uint32(0);                                    // Old time remaining
    l_Data << uint32(p_Save->GetMapId());
    SendPacket(&l_Data);
}
