////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "InstanceSaveMgr.h"
#include "Log.h"
#include "Opcodes.h"
#include "Player.h"
#include "SocialMgr.h"
#include "CalendarMgr.h"
#include "ObjectMgr.h"
#include "ObjectAccessor.h"
#include "DatabaseEnv.h"
#include "GuildMgr.h"
#include "WorldSession.h"
#include "WowTime.hpp"

void WorldSession::HandleCalendarGetCalendar(WorldPacket& /*p_RecvData*/)
{
    if (!m_Player)
        return;

    uint64 l_Guid = m_Player->GetGUID();
    uint32 l_Now = time(NULL);

    WorldPacket l_Data(SMSG_CALENDAR_SEND_CALENDAR, 30 * 1024);
    l_Data << uint32(l_Now);
    l_Data << uint32(MS::Utilities::WowTime::Encode(l_Now));
    l_Data << uint32(sWorld->GetServerRegionID());

    CalendarInviteStore l_Invites = sCalendarMgr->GetPlayerInvites(l_Guid);
    CalendarEventStore l_Events = sCalendarMgr->GetPlayerEvents(l_Guid);
    l_Data << uint32(l_Invites.size());
    l_Data << uint32(l_Events.size());

    uint32 l_RaidLockoutCount = 0;

    for (uint8 l_Iter = 0; l_Iter < Difficulty::MaxDifficulties; ++l_Iter)
    {
        for (Player::BoundInstancesMap::const_iterator l_Itr = m_Player->m_boundInstances[l_Iter].begin(); l_Itr != m_Player->m_boundInstances[l_Iter].end(); ++l_Itr)
        {
            if (l_Itr->second.perm)
                ++l_RaidLockoutCount;
        }
    }

    l_Data << uint32(l_RaidLockoutCount);

    uint32 l_RaidResetCount = 0;

    ResetTimeByMapDifficultyMap const& l_RaidReset = sInstanceSaveMgr->GetResetTimeMap();
    for (ResetTimeByMapDifficultyMap::const_iterator l_Iter = l_RaidReset.begin(); l_Iter != l_RaidReset.end(); ++l_Iter)
    {
        uint32 l_MapID = PAIR32_LOPART(l_Iter->first);

        MapEntry const* l_MapEntry = sMapStore.LookupEntry(l_MapID);
        if (!l_MapEntry || !l_MapEntry->IsRaid())
            continue;

        ++l_RaidResetCount;
    }

    l_Data << uint32(l_RaidResetCount);

    for (CalendarInviteStore::const_iterator l_Iter = l_Invites.begin(); l_Iter != l_Invites.end(); ++l_Iter)
    {
        l_Data << uint64((*l_Iter)->GetEventId());
        l_Data << uint64((*l_Iter)->GetInviteId());
        l_Data << uint8((*l_Iter)->GetStatus());
        l_Data << uint8((*l_Iter)->GetRank());

        if (CalendarEvent* l_CalendarEvent = sCalendarMgr->GetEvent((*l_Iter)->GetEventId()))
            l_Data << uint8(l_CalendarEvent ? l_CalendarEvent->GetGuildId() : 0);
        else
            l_Data << uint8(0);

        l_Data.appendPackGUID((*l_Iter)->GetSenderGUID());
    }

    for (CalendarEventStore::const_iterator l_Iter = l_Events.begin(); l_Iter != l_Events.end(); ++l_Iter)
    {
        CalendarEvent* l_CalendarEvent = *l_Iter;

        std::string l_Title = l_CalendarEvent->GetTitle();
        uint64 l_GuildGUID = 0;
        if (l_CalendarEvent->GetGuildId())
            l_GuildGUID = MAKE_NEW_GUID(l_CalendarEvent->GetGuildId(), 0, HIGHGUID_GUILD);

        l_Data << uint64(l_CalendarEvent->GetEventId());
        l_Data << uint8(l_CalendarEvent->GetType());
        l_Data << uint32(MS::Utilities::WowTime::Encode(l_CalendarEvent->GetEventTime()));
        l_Data << uint32(l_CalendarEvent->GetFlags());
        l_Data << uint32(l_CalendarEvent->GetDungeonId());
        l_Data.appendPackGUID(l_GuildGUID);
        l_Data.appendPackGUID(l_CalendarEvent->GetCreatorGUID());
        l_Data.WriteBits(l_Title.size(), 8);
        l_Data.FlushBits();
        l_Data.WriteString(l_Title);
    }

    for (uint8 l_Iter = 0; l_Iter < Difficulty::MaxDifficulties; ++l_Iter)
    {
        for (Player::BoundInstancesMap::const_iterator l_Itr = m_Player->m_boundInstances[l_Iter].begin(); l_Itr != m_Player->m_boundInstances[l_Iter].end(); ++l_Itr)
        {
            if (l_Itr->second.perm)
            {
                InstanceSave* l_Save = l_Itr->second.save;
                l_Data << uint64(l_Save->GetInstanceId());
                l_Data << uint32(l_Save->GetMapId());
                l_Data << uint32(l_Save->GetDifficultyID());
                l_Data << uint32(l_Save->GetResetTime());
            }
        }
    }

    for (ResetTimeByMapDifficultyMap::const_iterator l_Iter = l_RaidReset.begin(); l_Iter != l_RaidReset.end(); ++l_Iter)
    {
        uint32 l_MapID = PAIR32_LOPART(l_Iter->first);

        MapEntry const* l_MapEntry = sMapStore.LookupEntry(l_MapID);
        if (!l_MapEntry || !l_MapEntry->IsRaid())
            continue;

        l_Data << uint32(l_MapID);
        l_Data << uint32(l_Iter->second - l_Now);  // Duration
        l_Data << uint32(0);                       // Offset
    }

    SendPacket(&l_Data);
}

void WorldSession::HandleCalendarGetEvent(WorldPacket& p_RecvData)
{
    uint64 l_EventId;
    p_RecvData >> l_EventId;

    if (CalendarEvent* l_CalendarEvent = sCalendarMgr->GetEvent(l_EventId))
        sCalendarMgr->SendCalendarEvent(m_Player->GetGUID(), *l_CalendarEvent, (CalendarSendEventType)CALENDAR_SENDTYPE_GET);
    else
        sCalendarMgr->SendCalendarCommandResult(m_Player->GetGUID(), (CalendarError)CALENDAR_ERROR_EVENT_INVALID);
}

void WorldSession::HandleCalendarGuildFilter(WorldPacket& p_RecvData)
{
    uint32 minLevel;
    uint32 maxLevel;
    uint32 minRank;

    p_RecvData >> minLevel >> maxLevel >> minRank;

    /*if (Guild* guild = sGuildMgr->GetGuildById(m_Player->GetGuildId()))
        guild->MassInviteToEvent(this, minLevel, maxLevel, minRank);*/
}

void WorldSession::HandleCalendarAddEvent(WorldPacket& p_RecvData)
{
    uint64 l_Guid = m_Player->GetGUID();

    uint8 l_TitleSize, l_Type;
    uint32 l_DescSize, l_Invites, l_Flags, l_EventTime, l_MaxInvites, l_DungeonID;
    std::string l_Title, l_Desc;

    l_TitleSize = p_RecvData.ReadBits(8);
    l_DescSize = p_RecvData.ReadBits(11);
    p_RecvData.FlushBits();
    p_RecvData >> l_Type;
    p_RecvData >> l_DungeonID;
    p_RecvData.ReadPackedTime(l_EventTime);
    p_RecvData >> l_Flags;
    p_RecvData >> l_Invites;
    l_Title = p_RecvData.ReadString(l_TitleSize);
    l_Desc = p_RecvData.ReadString(l_DescSize);

    std::vector<uint64> l_InvitedGuids(l_Invites, 0);
    std::vector<uint8> l_InvitedStatus(l_Invites, 0);
    std::vector<uint8> l_InvitedRanks(l_Invites, 0);
    for (uint32 l_Iter = 0; l_Iter < l_Invites; ++l_Iter)
    {
        p_RecvData.readPackGUID(l_InvitedGuids[l_Iter]);
        p_RecvData >> l_InvitedStatus[l_Iter];
        p_RecvData >> l_InvitedRanks[l_Iter];
    }

    p_RecvData >> l_MaxInvites;

    CalendarEvent* l_CalendarEvent = new CalendarEvent(sCalendarMgr->GetFreeEventId(), l_Guid, 0, CalendarEventType(l_Type), l_DungeonID,
        l_EventTime, l_Flags, 0, l_Title, l_Desc);

    if (l_CalendarEvent->IsGuildEvent() || l_CalendarEvent->IsGuildAnnouncement())
    {
        if (Player* l_Creator = ObjectAccessor::FindPlayer(l_Guid))
            l_CalendarEvent->SetGuildId(l_Creator->GetGuildId());
    }

    if (l_CalendarEvent->IsGuildAnnouncement())
    {
        // 946684800 is 01/01/2000 00:00:00 - default response time
        CalendarInvite* l_Invite = new CalendarInvite(0, l_CalendarEvent->GetEventId(), 0, l_Guid, 946684800, CALENDAR_STATUS_NOT_SIGNED_UP, CALENDAR_RANK_PLAYER, "");
        sCalendarMgr->AddInvite(l_CalendarEvent, l_Invite);
    }
    else
    {
        SQLTransaction l_Transaction;
        if (l_Invites > 1)
            l_Transaction = CharacterDatabase.BeginTransaction();

        for (uint32 l_Iter = 0; l_Iter < l_Invites && l_Iter < l_MaxInvites; ++l_Iter)
        {
            // 946684800 is 01/01/2000 00:00:00 - default response time
            CalendarInvite* l_Invite = new CalendarInvite(sCalendarMgr->GetFreeInviteId(), l_CalendarEvent->GetEventId(), l_InvitedGuids[l_Iter], l_Guid, 946684800, CalendarInviteStatus(l_InvitedStatus[l_Iter]), CalendarModerationRank(l_InvitedRanks[l_Iter]), "");
            sCalendarMgr->AddInvite(l_CalendarEvent, l_Invite, l_Transaction);
        }

        if (l_Invites > 1)
            CharacterDatabase.CommitTransaction(l_Transaction);
    }

    sCalendarMgr->AddEvent(l_CalendarEvent, (CalendarSendEventType)CALENDAR_SENDTYPE_ADD);
}

void WorldSession::HandleCalendarUpdateEvent(WorldPacket& p_RecvData)
{
    uint64 l_EventID, l_InviteID;
    std::string l_Title;
    std::string l_Description;
    uint8 l_Type;
    uint32 l_MaxInvites;
    int32 l_DungeonID;
    uint32 l_EventTime, l_Flags, l_TitleLen, l_DescLen;

    p_RecvData >> l_EventID >> l_InviteID;
    p_RecvData >> l_Type;
    p_RecvData >> l_DungeonID;
    p_RecvData.ReadPackedTime(l_EventTime);
    p_RecvData >> l_Flags;

    l_TitleLen = p_RecvData.ReadBits(8);
    l_DescLen = p_RecvData.ReadBits(11);
    p_RecvData.FlushBits();

    l_Title = p_RecvData.ReadString(l_TitleLen);
    l_Description = p_RecvData.ReadString(l_DescLen);

    p_RecvData >> l_MaxInvites;

    if (CalendarEvent* l_CalendarEvent = sCalendarMgr->GetEvent(l_EventID))
    {
        uint32 l_OldEventTime = l_CalendarEvent->GetEventTime();

        l_CalendarEvent->SetType(CalendarEventType(l_Type));
        l_CalendarEvent->SetFlags(l_Flags);
        l_CalendarEvent->SetEventTime(l_EventTime);
        l_CalendarEvent->SetDungeonId(l_DungeonID);
        l_CalendarEvent->SetTitle(l_Title);
        l_CalendarEvent->SetDescription(l_Description);

        sCalendarMgr->UpdateEvent(l_CalendarEvent);
        sCalendarMgr->SendCalendarEventUpdateAlert(*l_CalendarEvent, l_OldEventTime);
    }
    else
        sCalendarMgr->SendCalendarCommandResult(m_Player->GetGUID(), CALENDAR_ERROR_EVENT_INVALID);
}

void WorldSession::HandleCalendarRemoveEvent(WorldPacket& p_RecvData)
{
    uint64 l_EventID = p_RecvData.read<uint64>();
    p_RecvData.rfinish(); // Skip flags & invite ID, we don't use them

    sCalendarMgr->RemoveEvent(l_EventID, m_Player->GetGUID());
}

void WorldSession::HandleCalendarCopyEvent(WorldPacket& p_RecvData)
{
    uint64 l_EventID = p_RecvData.read<uint64>();
    uint64 l_InviteID = p_RecvData.read<uint64>(); ///< l_inviteID is never read 01/18/16
    uint32 l_EventTime = 0;
    p_RecvData.ReadPackedTime(l_EventTime);

    if (CalendarEvent* l_OldEvent = sCalendarMgr->GetEvent(l_EventID))
    {
        CalendarEvent* l_NewEvent = new CalendarEvent(*l_OldEvent, sCalendarMgr->GetFreeEventId());
        l_NewEvent->SetEventTime(l_EventTime);
        sCalendarMgr->AddEvent(l_NewEvent, CALENDAR_SENDTYPE_COPY);

        CalendarInviteStore l_Invites = sCalendarMgr->GetEventInvites(l_EventID);
        SQLTransaction l_Transaction;
        if (l_Invites.size() > 1)
            l_Transaction = CharacterDatabase.BeginTransaction();

        for (CalendarInviteStore::const_iterator l_Iter = l_Invites.begin(); l_Iter != l_Invites.end(); ++l_Iter)
            sCalendarMgr->AddInvite(l_NewEvent, new CalendarInvite(**l_Iter, sCalendarMgr->GetFreeInviteId(), l_NewEvent->GetEventId()), l_Transaction);

        if (l_Invites.size() > 1)
            CharacterDatabase.CommitTransaction(l_Transaction);
    }
    else
        sCalendarMgr->SendCalendarCommandResult(m_Player->GetGUID(), CALENDAR_ERROR_EVENT_INVALID);
}

void WorldSession::HandleCalendarEventInvite(WorldPacket& p_RecvData)
{
    uint64 l_PlayerGUID = m_Player->GetGUID();
    std::string l_Name;

    uint64 l_EventID = p_RecvData.read<uint64>();
    uint64 l_InviteID = p_RecvData.read<uint64>(); ///< l_inviteID is never read 01/18/16
    uint8 l_NameLen = p_RecvData.ReadBits(8) * 2;
    l_NameLen += p_RecvData.ReadBit();
    bool l_IsPreInvite = p_RecvData.ReadBit();
    bool l_IsGuildEvent = p_RecvData.ReadBit();

    p_RecvData.FlushBits();

    l_Name = p_RecvData.ReadString(l_NameLen);

    uint64 l_InviteeGUID = 0;
    uint32 l_InviteeTeam = 0;
    uint32 l_InviteeGuildID = 0;

    if (Player* l_Player = ObjectAccessor::FindPlayerByName(l_Name.c_str()))
    {
        // Invitee is online
        l_InviteeGUID = l_Player->GetGUID();
        l_InviteeTeam = l_Player->GetTeam();
        l_InviteeGuildID = l_Player->GetGuildId();
    }
    else
    {
        // Invitee offline, get data from database
        PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_RACE_ACC_BY_NAME);
        l_Statement->setString(0, l_Name);
        if (PreparedQueryResult l_Result = CharacterDatabase.Query(l_Statement))
        {
            Field* l_Fields = l_Result->Fetch();
            l_InviteeGUID = MAKE_NEW_GUID(l_Fields[0].GetUInt32(), 0, HIGHGUID_PLAYER);
            l_InviteeTeam = Player::TeamForRace(l_Fields[1].GetUInt8());
            l_InviteeGuildID = Player::GetGuildIdFromDB(l_InviteeGUID);
        }
    }

    if (!l_InviteeGUID)
    {
        sCalendarMgr->SendCalendarCommandResult(l_PlayerGUID, CALENDAR_ERROR_PLAYER_NOT_FOUND);
        return;
    }

    if (m_Player->GetTeam() != l_InviteeTeam && !sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_CALENDAR))
    {
        sCalendarMgr->SendCalendarCommandResult(l_PlayerGUID, CALENDAR_ERROR_NOT_ALLIED);
        return;
    }

    if (!l_IsPreInvite)
    {
        if (CalendarEvent* l_CalendarEvent = sCalendarMgr->GetEvent(l_EventID))
        {
            if (l_CalendarEvent->IsGuildEvent() && l_CalendarEvent->GetGuildId() == l_InviteeGuildID)
            {
                // we can't invite guild members to guild events
                sCalendarMgr->SendCalendarCommandResult(l_PlayerGUID, CALENDAR_ERROR_NO_GUILD_INVITES);
                return;
            }

            // 946684800 is 01/01/2000 00:00:00 - default response time
            CalendarInvite* l_Invite = new CalendarInvite(sCalendarMgr->GetFreeInviteId(), l_EventID, l_InviteeGUID, l_PlayerGUID, 946684800, CALENDAR_STATUS_INVITED, CALENDAR_RANK_PLAYER, "");
            sCalendarMgr->AddInvite(l_CalendarEvent, l_Invite);
            sCalendarMgr->SendCalendarClearPendingAction(m_Player->GetGUID());
        }
        else
            sCalendarMgr->SendCalendarCommandResult(l_PlayerGUID, CALENDAR_ERROR_EVENT_INVALID);
    }
    else
    {
        if (l_IsGuildEvent && l_InviteeGuildID == m_Player->GetGuildId())
        {
            sCalendarMgr->SendCalendarCommandResult(l_PlayerGUID, CALENDAR_ERROR_NO_GUILD_INVITES);
            return;
        }

        // 946684800 is 01/01/2000 00:00:00 - default response time
        CalendarInvite l_Invite(0, 0, l_InviteeGUID, l_PlayerGUID, 946684800, CALENDAR_STATUS_INVITED, CALENDAR_RANK_PLAYER, "");
        sCalendarMgr->SendCalendarEventInvite(l_Invite);
    }
}

void WorldSession::HandleCalendarEventSignup(WorldPacket& p_RecvData)
{
    uint64 l_Guid = m_Player->GetGUID();
    uint64 l_EventID = 0;
    bool l_Tentative = false;

    p_RecvData >> l_EventID;
    l_Tentative = p_RecvData.ReadBit();

    if (CalendarEvent* l_Event = sCalendarMgr->GetEvent(l_EventID))
    {
        if (l_Event->IsGuildEvent() && l_Event->GetGuildId() != m_Player->GetGuildId())
        {
            sCalendarMgr->SendCalendarCommandResult(l_Guid, CALENDAR_ERROR_GUILD_PLAYER_NOT_IN_GUILD);
            return;
        }

        CalendarInviteStatus l_Status = l_Tentative ? CALENDAR_STATUS_TENTATIVE : CALENDAR_STATUS_SIGNED_UP;
        CalendarInvite* l_Invite = new CalendarInvite(sCalendarMgr->GetFreeInviteId(), l_EventID, l_Guid, l_Guid, time(NULL), l_Status, CALENDAR_RANK_PLAYER, "");
        sCalendarMgr->AddInvite(l_Event, l_Invite);
        sCalendarMgr->SendCalendarClearPendingAction(l_Guid);
    }
    else
        sCalendarMgr->SendCalendarCommandResult(l_Guid, CALENDAR_ERROR_EVENT_INVALID);
}

void WorldSession::HandleCalendarEventRsvp(WorldPacket& p_RecvData)
{
    uint64 l_Guid = m_Player->GetGUID();
    uint64 l_EventID, l_InviteID = 0;
    uint8 l_Status = 0;

    p_RecvData >> l_EventID >> l_InviteID >> l_Status;

    if (CalendarEvent* l_Event = sCalendarMgr->GetEvent(l_EventID))
    {
        // I think we still should be able to remove self from locked events
        if (l_Status != CALENDAR_STATUS_REMOVED && l_Event->GetFlags() & CALENDAR_FLAG_INVITES_LOCKED)
        {
            sCalendarMgr->SendCalendarCommandResult(l_Guid, CALENDAR_ERROR_EVENT_LOCKED);
            return;
        }

        if (CalendarInvite* l_Invite = sCalendarMgr->GetInvite(l_InviteID))
        {
            l_Invite->SetStatus(CalendarInviteStatus(l_Status));
            l_Invite->SetStatusTime(time(NULL));

            sCalendarMgr->UpdateInvite(l_Invite);
            sCalendarMgr->SendCalendarEventInviteStatus(l_Event->GetCreatorGUID(), *l_Event, *l_Invite);
            sCalendarMgr->SendCalendarEventInviteStatus(l_Invite->GetInviteeGUID(), *l_Event, *l_Invite);
            sCalendarMgr->SendCalendarEventInviteStatusAlert(*l_Event, *l_Invite);
            sCalendarMgr->SendCalendarClearPendingAction(l_Guid);
        }
        else
            sCalendarMgr->SendCalendarCommandResult(l_Guid, CALENDAR_ERROR_NO_INVITE);
    }
    else
        sCalendarMgr->SendCalendarCommandResult(l_Guid, CALENDAR_ERROR_EVENT_INVALID);
}

void WorldSession::HandleCalendarEventRemoveInvite(WorldPacket& p_RecvData)
{
    uint64 l_Invitee, l_EventID, l_OwnerInviteID, l_InviteID = 0;

    p_RecvData.readPackGUID(l_Invitee);
    p_RecvData >> l_InviteID >> l_OwnerInviteID >> l_EventID;

    if (CalendarEvent* l_Event = sCalendarMgr->GetEvent(l_EventID))
    {
        if (l_Event->GetCreatorGUID() == l_Invitee)
        {
            sCalendarMgr->SendCalendarCommandResult(m_Player->GetGUID(), CALENDAR_ERROR_DELETE_CREATOR_FAILED);
            return;
        }

        sCalendarMgr->RemoveInvite(l_InviteID, l_EventID, m_Player->GetGUID());
    }
    else
        sCalendarMgr->SendCalendarCommandResult(m_Player->GetGUID(), CALENDAR_ERROR_NO_INVITE);
}

void WorldSession::HandleCalendarEventStatus(WorldPacket& p_RecvData)
{
    uint64 l_Invitee, l_EventID, l_InviteID, l_OwnerInviteID = 0;
    uint8 l_Status = 0;

    p_RecvData.readPackGUID(l_Invitee);
    p_RecvData >> l_EventID >> l_InviteID >> l_OwnerInviteID >> l_Status;

    if (CalendarEvent* l_Event = sCalendarMgr->GetEvent(l_EventID))
    {
        if (CalendarInvite* l_Invite = sCalendarMgr->GetInvite(l_InviteID))
        {
            l_Invite->SetStatus((CalendarInviteStatus)l_Status);

            sCalendarMgr->UpdateInvite(l_Invite);
            sCalendarMgr->SendCalendarEventInviteStatus(l_Event->GetCreatorGUID(), *l_Event, *l_Invite);
            sCalendarMgr->SendCalendarEventInviteStatusAlert(*l_Event, *l_Invite);
            sCalendarMgr->SendCalendarClearPendingAction(l_Invitee);
        }
        else
            sCalendarMgr->SendCalendarCommandResult(m_Player->GetGUID(), CALENDAR_ERROR_NO_INVITE);
    }
    else
        sCalendarMgr->SendCalendarCommandResult(m_Player->GetGUID(), CALENDAR_ERROR_EVENT_INVALID);
}

void WorldSession::HandleCalendarEventModeratorStatus(WorldPacket& p_RecvData)
{
    uint64 l_Invitee, l_EventID, l_InviteID, l_OwnerInviteID = 0;
    uint8 l_Rank = 0;

    p_RecvData.readPackGUID(l_Invitee);
    p_RecvData >> l_EventID >> l_InviteID >> l_OwnerInviteID >> l_Rank;

    if (CalendarEvent* l_Event = sCalendarMgr->GetEvent(l_EventID))
    {
        if (CalendarInvite* l_Invite = sCalendarMgr->GetInvite(l_InviteID))
        {
            l_Invite->SetRank(CalendarModerationRank(l_Rank));
            sCalendarMgr->UpdateInvite(l_Invite);
            sCalendarMgr->SendCalendarEventModeratorStatus(*l_Event, *l_Invite, m_Player->GetGUID());
            sCalendarMgr->SendCalendarEventModeratorStatus(*l_Event, *l_Invite, l_Invite->GetInviteeGUID());
        }
        else
            sCalendarMgr->SendCalendarCommandResult(m_Player->GetGUID(), CALENDAR_ERROR_NO_INVITE);
    }
    else
        sCalendarMgr->SendCalendarCommandResult(m_Player->GetGUID(), CALENDAR_ERROR_EVENT_INVALID);
}

void WorldSession::HandleCalendarComplain(WorldPacket& p_RecvData)
{
    uint64 l_EventID, l_InviteID, l_ComplainGUID = 0;

    p_RecvData.readPackGUID(l_ComplainGUID);
    p_RecvData >> l_EventID >> l_InviteID;

    sCalendarMgr->SendCalendarClearPendingAction(m_Player->GetGUID());
}

void WorldSession::HandleCalendarGetNumPending(WorldPacket& /*p_RecvData*/)
{
    uint32 l_Pending = sCalendarMgr->GetPlayerNumPending(m_Player->GetGUID());

    WorldPacket l_Data(SMSG_CALENDAR_SEND_NUM_PENDING, 4);
    l_Data << uint32(l_Pending);
    SendPacket(&l_Data);
}

void WorldSession::HandleSetSavedInstanceExtend(WorldPacket& /*p_RecvData*/)
{
    /*uint32 mapId, difficulty;
    uint8 toggleExtend;
    p_RecvData >> mapId >> difficulty >> toggleExtend;

    InstancePlayerBind* instanceBind = _player->GetBoundInstance(mapId, Difficulty(difficulty));
    if (!instanceBind || !instanceBind->save)
    return;
    InstanceSave* save = instanceBind->save;
    // http://www.wowwiki.com/Instance_Lock_Extension
    // SendCalendarRaidLockoutUpdated(save);
    */
}

// ----------------------------------- SEND ------------------------------------
void WorldSession::SendCalendarRaidLockout(InstanceSave const* /*save*/, bool /*add*/)
{
    /*uint32 currTime = time(NULL);

    WorldPacket data(SMSG_CALENDAR_RAID_LOCKOUT_REMOVED, (add ? 4 : 0) + 4 + 4 + 4 + 8);
    if (add)
    {
        data.SetOpcode(SMSG_CALENDAR_RAID_LOCKOUT_ADDED);
        data.AppendPackedTime(currTime);
    }

    data << uint32(save->GetMapId());
    data << uint32(save->GetDifficulty());
    data << uint32(save->GetResetTime() - currTime);
    data << uint64(save->GetInstanceId());
    SendPacket(&data);*/
}
#endif
