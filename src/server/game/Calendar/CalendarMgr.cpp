////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "CalendarMgr.h"
#include "QueryResult.h"
#include "Log.h"
#include "Player.h"
#include "GuildMgr.h"
#include "ObjectAccessor.h"
#include "Opcodes.h"
#include "WowTime.hpp"

/////////////////////////////////////////////////////////////////////
/// CalendarInvite
CalendarInvite::~CalendarInvite()
{
    sCalendarMgr->FreeInviteId(m_InviteID);
}
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
/// CalendarEvent
CalendarEvent::~CalendarEvent()
{
    sCalendarMgr->FreeEventId(m_EventID);
}

std::string CalendarEvent::BuildCalendarMailSubject(uint64 p_Remover) const
{
    std::ostringstream l_Stream;
    l_Stream << p_Remover << ':' << m_Title;
    return l_Stream.str();
}

std::string CalendarEvent::BuildCalendarMailBody() const
{
    std::ostringstream l_Stream;
    l_Stream << uint32(MS::Utilities::WowTime::Encode(m_EventTime));
    return l_Stream.str();
}
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
/// CalendarMgr
CalendarMgr::CalendarMgr() : m_MaxEventID(0), m_MaxInviteID(0) { }

CalendarMgr::~CalendarMgr()
{
    for (CalendarEventStore::iterator l_Itr = m_Events.begin(); l_Itr != m_Events.end(); ++l_Itr)
        delete *l_Itr;

    for (CalendarEventInviteStore::iterator l_Itr = m_Invites.begin(); l_Itr != m_Invites.end(); ++l_Itr)
    {
        for (CalendarInviteStore::iterator l_Iter = l_Itr->second.begin(); l_Iter != l_Itr->second.end(); ++l_Iter)
            delete *l_Iter;
    }
}

void CalendarMgr::LoadFromDB()
{
    uint32 l_Count = 0;
    m_MaxEventID = 0;
    m_MaxInviteID = 0;

    //                                                       0   1        2      3            4     5        6          7      8
    if (QueryResult l_Result = CharacterDatabase.Query("SELECT id, creator, title, description, type, dungeon, eventtime, flags, time2 FROM calendar_events"))
    {
        do
        {
            Field* l_Fields = l_Result->Fetch();

            uint64 l_EventID = l_Fields[0].GetUInt64();
            uint64 l_Sender = MAKE_NEW_GUID(l_Fields[1].GetUInt32(), 0, HIGHGUID_PLAYER);
            std::string l_Title = l_Fields[2].GetString();
            std::string l_Description = l_Fields[3].GetString();
            CalendarEventType l_Type = CalendarEventType(l_Fields[4].GetUInt8());
            int32 l_DungeonID = l_Fields[5].GetInt32();
            uint32 l_EventTime = l_Fields[6].GetUInt32();
            uint32 l_Flags = l_Fields[7].GetUInt32();
            uint32 l_TimeZone = l_Fields[8].GetUInt32();
            uint32 l_GuildID = 0;

            if (l_Flags & CALENDAR_FLAG_GUILD_EVENT || l_Flags & CALENDAR_FLAG_WITHOUT_INVITES)
                l_GuildID = Player::GetGuildIdFromDB(l_Sender);

            CalendarEvent* l_Event = new CalendarEvent(l_EventID, l_Sender, l_GuildID, l_Type, l_DungeonID, l_EventTime, l_Flags, l_TimeZone, l_Title, l_Description);
            m_Events.insert(l_Event);

            m_MaxEventID = std::max(m_MaxEventID, l_EventID);

            ++l_Count;
        }
        while (l_Result->NextRow());
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u calendar events", l_Count);
    l_Count = 0;

    //                                                       0   1      2        3       4       5           6     7
    if (QueryResult l_Result = CharacterDatabase.Query("SELECT id, event, invitee, sender, status, statustime, rank, text FROM calendar_invites"))
    {
        do
        {
            Field* l_Fields = l_Result->Fetch();

            uint64 l_InviteID = l_Fields[0].GetUInt64();
            uint64 l_EventID = l_Fields[1].GetUInt64();
            uint64 l_Invitee = MAKE_NEW_GUID(l_Fields[2].GetUInt32(), 0, HIGHGUID_PLAYER);
            uint64 l_Sender = MAKE_NEW_GUID(l_Fields[3].GetUInt32(), 0, HIGHGUID_PLAYER);
            CalendarInviteStatus l_Status = CalendarInviteStatus(l_Fields[4].GetUInt8());
            uint32 l_StatusTime = l_Fields[5].GetUInt32();
            CalendarModerationRank l_Rank = CalendarModerationRank(l_Fields[6].GetUInt8());
            std::string l_Text = l_Fields[7].GetString();

            CalendarInvite* l_Invite = new CalendarInvite(l_InviteID, l_EventID, l_Invitee, l_Sender, l_StatusTime, l_Status, l_Rank, l_Text);
            m_Invites[l_EventID].push_back(l_Invite);

            m_MaxInviteID = std::max(m_MaxInviteID, l_InviteID);

            ++l_Count;
        }
        while (l_Result->NextRow());
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u calendar invites", l_Count);

    for (uint64 l_I = 1; l_I < m_MaxEventID; ++l_I)
    {
        if (!GetEvent(l_I))
            m_FreeEventIDs.push_back(l_I);
    }

    for (uint64 l_I = 1; l_I < m_MaxInviteID; ++l_I)
    {
        if (!GetInvite(l_I))
            m_FreeInviteIDs.push_back(l_I);
    }
}

CalendarEvent* CalendarMgr::GetEvent(uint64 p_EventID) const
{
    for (CalendarEventStore::const_iterator l_Iter = m_Events.begin(); l_Iter != m_Events.end(); ++l_Iter)
    {
        if ((*l_Iter)->GetEventId() == p_EventID)
            return *l_Iter;
    }

    return nullptr;
}

CalendarEventStore CalendarMgr::GetPlayerEvents(uint64 p_Guid)
{
    CalendarEventStore l_Events;

    for (CalendarEventInviteStore::const_iterator l_Iter = m_Invites.begin(); l_Iter != m_Invites.end(); ++l_Iter)
    {
        for (CalendarInviteStore::const_iterator l_I = l_Iter->second.begin(); l_I != l_Iter->second.end(); ++l_I)
        {
            if ((*l_I)->GetInviteeGUID() == p_Guid)
            {
                if (CalendarEvent* l_Event = GetEvent(l_Iter->first)) // NULL check added as attempt to fix #11512
                    l_Events.insert(l_Event);
            }
        }
    }

    if (Player* l_Player = ObjectAccessor::FindPlayer(p_Guid))
    {
        for (CalendarEventStore::const_iterator l_Iter = m_Events.begin(); l_Iter != m_Events.end(); ++l_Iter)
        {
            if ((*l_Iter)->GetGuildId() == l_Player->GetGuildId())
                l_Events.insert(*l_Iter);
        }
    }

    return l_Events;
}

CalendarInvite* CalendarMgr::GetInvite(uint64 p_InviteID) const
{
    for (CalendarEventInviteStore::const_iterator l_Iter = m_Invites.begin(); l_Iter != m_Invites.end(); ++l_Iter)
    {
        for (CalendarInviteStore::const_iterator l_I = l_Iter->second.begin(); l_I != l_Iter->second.end(); ++l_I)
        {
            if ((*l_I)->GetInviteId() == p_InviteID)
                return *l_I;
        }
    }

    return nullptr;
}

CalendarInviteStore const& CalendarMgr::GetEventInvites(uint64 p_EventID)
{
    return m_Invites[p_EventID];
}

CalendarInviteStore CalendarMgr::GetPlayerInvites(uint64 p_Guid)
{
    CalendarInviteStore l_Invites;

    for (CalendarEventInviteStore::const_iterator l_Iter = m_Invites.begin(); l_Iter != m_Invites.end(); ++l_Iter)
    {
        for (CalendarInviteStore::const_iterator l_I = l_Iter->second.begin(); l_I != l_Iter->second.end(); ++l_I)
        {
            if ((*l_I)->GetInviteeGUID() == p_Guid)
                l_Invites.push_back(*l_I);
        }
    }

    return l_Invites;
}

void CalendarMgr::FreeEventId(uint64 p_ID)
{
    if (p_ID == m_MaxEventID)
        --m_MaxEventID;
    else
        m_FreeEventIDs.push_back(p_ID);
}

uint64 CalendarMgr::GetFreeEventId()
{
    if (m_FreeEventIDs.empty())
        return ++m_MaxEventID;

    uint64 eventId = m_FreeEventIDs.front();
    m_FreeEventIDs.pop_front();
    return eventId;
}

void CalendarMgr::FreeInviteId(uint64 p_ID)
{
    if (p_ID == m_MaxInviteID)
        --m_MaxInviteID;
    else
        m_FreeInviteIDs.push_back(p_ID);
}

uint64 CalendarMgr::GetFreeInviteId()
{
    if (m_FreeInviteIDs.empty())
        return ++m_MaxInviteID;

    uint64 inviteId = m_FreeInviteIDs.front();
    m_FreeInviteIDs.pop_front();
    return inviteId;
}

uint32 CalendarMgr::GetPlayerNumPending(uint64 p_Guid)
{
    CalendarInviteStore const& l_Invites = GetPlayerInvites(p_Guid);

    uint32 l_NumPending = 0;
    for (CalendarInviteStore::const_iterator l_Iter = l_Invites.begin(); l_Iter != l_Invites.end(); ++l_Iter)
    {
        switch ((*l_Iter)->GetStatus())
        {
            case CALENDAR_STATUS_INVITED:
            case CALENDAR_STATUS_TENTATIVE:
            case CALENDAR_STATUS_NOT_SIGNED_UP:
                ++l_NumPending;
                break;
            default:
                break;
        }
    }

    return l_NumPending;
}

void CalendarMgr::AddEvent(CalendarEvent* p_CalendarEvent, CalendarSendEventType p_SendType)
{
    m_Events.insert(p_CalendarEvent);
    UpdateEvent(p_CalendarEvent);
    SendCalendarEvent(p_CalendarEvent->GetCreatorGUID(), *p_CalendarEvent, p_SendType);
}

void CalendarMgr::AddInvite(CalendarEvent* p_CalendarEvent, CalendarInvite* p_Invite)
{
    SQLTransaction l_DummyTrans;
    AddInvite(p_CalendarEvent, p_Invite, l_DummyTrans);
}

void CalendarMgr::AddInvite(CalendarEvent* p_CalendarEvent, CalendarInvite* p_Invite, SQLTransaction& p_Transaction)
{
    if (!p_CalendarEvent->IsGuildAnnouncement())
        SendCalendarEventInvite(*p_Invite);

    if (!p_CalendarEvent->IsGuildEvent() || p_Invite->GetInviteeGUID() == p_CalendarEvent->GetCreatorGUID())
        SendCalendarEventInviteAlert(*p_CalendarEvent, *p_Invite);

    if (!p_CalendarEvent->IsGuildAnnouncement())
    {
        m_Invites[p_Invite->GetEventId()].push_back(p_Invite);
        UpdateInvite(p_Invite, p_Transaction);
    }
}

void CalendarMgr::RemoveEvent(uint64 p_EventID, uint64 p_Remover)
{
    CalendarEvent* l_CalendarEvent = GetEvent(p_EventID);
    if (!l_CalendarEvent)
    {
        SendCalendarCommandResult(p_Remover, CalendarError::CALENDAR_ERROR_EVENT_INVALID);
        return;
    }

    SendCalendarEventRemovedAlert(*l_CalendarEvent);

    SQLTransaction l_Transaction = CharacterDatabase.BeginTransaction();
    PreparedStatement* l_Statement;
    MailDraft l_Mail(l_CalendarEvent->BuildCalendarMailSubject(p_Remover), l_CalendarEvent->BuildCalendarMailBody());

    CalendarInviteStore& l_EventInvites = m_Invites[p_EventID];
    for (size_t l_Iter = 0; l_Iter < l_EventInvites.size(); ++l_Iter)
    {
        CalendarInvite* l_Invite = l_EventInvites[l_Iter];
        l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CALENDAR_INVITE);
        l_Statement->setUInt64(0, l_Invite->GetInviteId());
        l_Transaction->Append(l_Statement);

        // guild events only? check invite status here?
        // When an event is deleted, all invited (accepted/declined? - verify) guilder's are notified via in-game mail.
        if (p_Remover && l_Invite->GetInviteeGUID() != p_Remover)
            l_Mail.SendMailTo(l_Transaction, MailReceiver(GUID_LOPART(l_Invite->GetInviteeGUID())), l_CalendarEvent, MAIL_CHECK_MASK_COPIED);

        delete l_Invite;
    }

    m_Invites.erase(p_EventID);

    l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CALENDAR_EVENT);
    l_Statement->setUInt64(0, p_EventID);
    l_Transaction->Append(l_Statement);
    CharacterDatabase.CommitTransaction(l_Transaction);

    delete l_CalendarEvent;
    m_Events.erase(l_CalendarEvent);
}

void CalendarMgr::RemoveInvite(uint64 p_InviteID, uint64 p_EventID, uint64 /*p_Remover*/)
{
    CalendarEvent* l_CalendarEvent = GetEvent(p_EventID);
    if (!l_CalendarEvent)
        return;

    CalendarInviteStore::iterator l_Iter = m_Invites[p_EventID].begin();
    for (; l_Iter != m_Invites[p_EventID].end(); ++l_Iter)
    {
        if ((*l_Iter)->GetInviteId() == p_InviteID)
            break;
    }

    if (l_Iter == m_Invites[p_EventID].end())
        return;

    SQLTransaction l_Transaction = CharacterDatabase.BeginTransaction();
    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CALENDAR_INVITE);
    l_Statement->setUInt64(0, (*l_Iter)->GetInviteId());
    l_Transaction->Append(l_Statement);
    CharacterDatabase.CommitTransaction(l_Transaction);

    if (!l_CalendarEvent->IsGuildEvent())
        SendCalendarEventInviteRemoveAlert((*l_Iter)->GetInviteeGUID(), *l_CalendarEvent, (CalendarInviteStatus)CALENDAR_STATUS_REMOVED);

    SendCalendarEventInviteRemove(*l_CalendarEvent, **l_Iter, l_CalendarEvent->GetFlags());

    delete *l_Iter;
    m_Invites[p_EventID].erase(l_Iter);
}

void CalendarMgr::UpdateEvent(CalendarEvent* p_CalendarEvent)
{
    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_REP_CALENDAR_EVENT);
    l_Statement->setUInt64(0, p_CalendarEvent->GetEventId());
    l_Statement->setUInt32(1, GUID_LOPART(p_CalendarEvent->GetCreatorGUID()));
    l_Statement->setString(2, p_CalendarEvent->GetTitle());
    l_Statement->setString(3, p_CalendarEvent->GetDescription());
    l_Statement->setUInt8(4, p_CalendarEvent->GetType());
    l_Statement->setInt32(5, p_CalendarEvent->GetDungeonId());
    l_Statement->setUInt32(6, uint32(p_CalendarEvent->GetEventTime()));
    l_Statement->setUInt32(7, p_CalendarEvent->GetFlags());
    l_Statement->setUInt32(8, p_CalendarEvent->GetTimeZoneTime()); // correct?
    CharacterDatabase.Execute(l_Statement);
}

void CalendarMgr::UpdateInvite(CalendarInvite* p_Invite)
{
    SQLTransaction l_DummyTrans;
    UpdateInvite(p_Invite, l_DummyTrans);
}

void CalendarMgr::UpdateInvite(CalendarInvite* p_Invite, SQLTransaction& p_Transaction)
{
    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_REP_CALENDAR_INVITE);
    l_Statement->setUInt64(0, p_Invite->GetInviteId());
    l_Statement->setUInt64(1, p_Invite->GetEventId());
    l_Statement->setUInt32(2, GUID_LOPART(p_Invite->GetInviteeGUID()));
    l_Statement->setUInt32(3, GUID_LOPART(p_Invite->GetSenderGUID()));
    l_Statement->setUInt8(4, p_Invite->GetStatus());
    l_Statement->setUInt32(5, uint32(p_Invite->GetStatusTime()));
    l_Statement->setUInt8(6, p_Invite->GetRank());
    l_Statement->setString(7, p_Invite->GetText());
    CharacterDatabase.ExecuteOrAppend(p_Transaction, l_Statement);
}

void CalendarMgr::RemoveAllPlayerEventsAndInvites(uint64 p_Guid)
{
    for (CalendarEventStore::const_iterator l_Iter = m_Events.begin(); l_Iter != m_Events.end(); ++l_Iter)
    {
        if ((*l_Iter)->GetCreatorGUID() == p_Guid)
            RemoveEvent((*l_Iter)->GetEventId(), 0); // don't send mail if removing a character
    }

    CalendarInviteStore l_Invites = GetPlayerInvites(p_Guid);
    for (CalendarInviteStore::const_iterator l_Iter = l_Invites.begin(); l_Iter != l_Invites.end(); ++l_Iter)
        RemoveInvite((*l_Iter)->GetInviteId(), (*l_Iter)->GetEventId(), p_Guid);
}

void CalendarMgr::RemovePlayerGuildEventsAndSignups(uint64 p_Guid, uint32 p_GuildID)
{
    for (CalendarEventStore::const_iterator l_Iter = m_Events.begin(); l_Iter != m_Events.end(); ++l_Iter)
    {
        if ((*l_Iter)->GetCreatorGUID() == p_Guid && ((*l_Iter)->IsGuildEvent() || (*l_Iter)->IsGuildAnnouncement()))
            RemoveEvent((*l_Iter)->GetEventId(), p_Guid);
    }

    CalendarInviteStore l_Invites = GetPlayerInvites(p_Guid);
    for (CalendarInviteStore::const_iterator l_Iter = l_Invites.begin(); l_Iter != l_Invites.end(); ++l_Iter)
    {
        if (CalendarEvent* l_Event = GetEvent((*l_Iter)->GetEventId()))
        {
            if (l_Event->IsGuildEvent() && l_Event->GetGuildId() == p_GuildID)
                RemoveInvite((*l_Iter)->GetInviteId(), (*l_Iter)->GetEventId(), p_Guid);
        }
    }
}

/////////////////////////////////////////////////////////////////////
/// UpdateStatus - Must be sent to Creator
void CalendarMgr::SendCalendarEventInvite(CalendarInvite const& p_Invite)
{
    if (Player* l_Player = ObjectAccessor::FindPlayer(p_Invite.GetSenderGUID()))
    {
        CalendarEvent* l_CalendarEvent = GetEvent(p_Invite.GetEventId());
        uint32 l_StatusTime = p_Invite.GetStatusTime();

        uint64 l_Invitee = p_Invite.GetInviteeGUID();
        Player* l_InvitePlayer = ObjectAccessor::FindPlayer(l_Invitee);

        uint8 l_Level = l_InvitePlayer ? l_InvitePlayer->getLevel() : Player::GetLevelFromDB(l_Invitee);

        WorldPacket l_Data(SMSG_CALENDAR_EVENT_INVITE);
        l_Data.appendPackGUID(l_Invitee);
        l_Data << uint64(p_Invite.GetEventId());
        l_Data << uint64(p_Invite.GetInviteId());
        l_Data << uint8(l_Level);
        l_Data << uint8(p_Invite.GetStatus());
        l_Data << uint8(l_CalendarEvent ? l_CalendarEvent->GetType() : 0);
        l_Data << uint32(MS::Utilities::WowTime::Encode(l_StatusTime));
        l_Data.WriteBit(l_StatusTime == 946684800);
        l_Data.FlushBits();
        l_Player->SendDirectMessage(&l_Data);
    }
}

void CalendarMgr::SendCalendarEventInviteStatus(uint64 p_Guid, CalendarEvent const& p_CalendarEvent, CalendarInvite const& p_Invite)
{
    if (Player* l_Player = ObjectAccessor::FindPlayer(p_Guid))
    {
        WorldPacket l_Data(SMSG_CALENDAR_EVENT_INVITE_STATUS);
        l_Data.appendPackGUID(p_Invite.GetInviteeGUID());
        l_Data << uint64(p_CalendarEvent.GetEventId());
        l_Data << uint32(MS::Utilities::WowTime::Encode(p_CalendarEvent.GetEventTime()));
        l_Data << uint32(p_CalendarEvent.GetFlags());
        l_Data << uint8(p_Invite.GetStatus());
        l_Data << uint32(MS::Utilities::WowTime::Encode(time(NULL)));
        l_Data.WriteBit(true);  ///< ClearPending
        l_Player->SendDirectMessage(&l_Data);
    }
}

void CalendarMgr::SendCalendarEventInviteRemove(CalendarEvent const& p_CalendarEvent, CalendarInvite const& p_Invite, uint32 p_Flags)
{
    if (Player* l_Player = ObjectAccessor::FindPlayer(p_CalendarEvent.GetCreatorGUID()))
    {
        WorldPacket l_Data(SMSG_CALENDAR_EVENT_INVITE_REMOVED, 8 + 4 + 4 + 1);
        l_Data.appendPackGUID(p_Invite.GetInviteeGUID());
        l_Data << uint64(p_Invite.GetEventId());
        l_Data << uint32(p_Flags);
        l_Data.WriteBit(true);  ///< ClearPending
        l_Player->SendDirectMessage(&l_Data);
    }
}
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
/// StatusAlert - Must be sent to concerned players
void CalendarMgr::SendCalendarEventInviteAlert(CalendarEvent const& p_CalendarEvent, CalendarInvite const& p_Invite)
{
    Guild* l_Guild = sGuildMgr->GetGuildById(p_CalendarEvent.GetGuildId());
    WorldPacket l_Data(SMSG_CALENDAR_EVENT_INVITE_ALERT);

    l_Data << uint64(p_CalendarEvent.GetEventId());
    l_Data << uint32(MS::Utilities::WowTime::Encode(p_CalendarEvent.GetEventTime()));
    l_Data << uint32(p_CalendarEvent.GetFlags());
    l_Data << uint8(p_CalendarEvent.GetType());
    l_Data << int32(p_CalendarEvent.GetDungeonId());
    l_Data.appendPackGUID(l_Guild ? l_Guild->GetGUID() : 0);
    l_Data << uint64(p_Invite.GetInviteId());
    l_Data << uint8(p_Invite.GetStatus());
    l_Data << uint8(p_Invite.GetRank());
    l_Data.appendPackGUID(p_Invite.GetSenderGUID());
    l_Data.appendPackGUID(p_CalendarEvent.GetCreatorGUID());
    l_Data.WriteBits(p_CalendarEvent.GetTitle().size(), 8);
    l_Data.FlushBits();
    l_Data.WriteString(p_CalendarEvent.GetTitle());

    if ((p_CalendarEvent.IsGuildEvent() || p_CalendarEvent.IsGuildAnnouncement()) && l_Guild)
        l_Guild->BroadcastPacket(&l_Data);
    else
    {
        if (Player* l_Player = ObjectAccessor::FindPlayer(p_Invite.GetInviteeGUID()))
            l_Player->SendDirectMessage(&l_Data);
    }
}

void CalendarMgr::SendCalendarEventInviteRemoveAlert(uint64 p_Guid, CalendarEvent const& p_CalendarEvent, CalendarInviteStatus p_Status)
{
    if (Player* l_Player = ObjectAccessor::FindPlayer(p_Guid))
    {
        WorldPacket l_Data(SMSG_CALENDAR_EVENT_INVITE_REMOVED_ALERT, 8 + 4 + 4 + 1);
        l_Data << uint64(p_CalendarEvent.GetEventId());
        l_Data << uint32(p_CalendarEvent.GetFlags());
        l_Data << uint32(MS::Utilities::WowTime::Encode(p_CalendarEvent.GetEventTime()));
        l_Data << uint8(p_Status);
        l_Player->SendDirectMessage(&l_Data);
    }
}

void CalendarMgr::SendCalendarEventRemovedAlert(CalendarEvent const& p_CalendarEvent)
{
    WorldPacket l_Data(SMSG_CALENDAR_EVENT_REMOVED_ALERT, 1 + 8 + 1);
    l_Data << uint64(p_CalendarEvent.GetEventId());
    l_Data << uint32(MS::Utilities::WowTime::Encode(p_CalendarEvent.GetEventTime()));
    l_Data.WriteBit(true);  ///< ClearPending
    SendPacketToAllEventRelatives(l_Data, p_CalendarEvent, true);
}

void CalendarMgr::SendCalendarEventUpdateAlert(CalendarEvent const& p_CalendarEvent, uint32 p_OldEventTime)
{
    WorldPacket l_Data(SMSG_CALENDAR_EVENT_UPDATED_ALERT);
    l_Data << uint64(p_CalendarEvent.GetEventId());
    l_Data << uint32(MS::Utilities::WowTime::Encode(p_OldEventTime));
    l_Data << uint32(MS::Utilities::WowTime::Encode(p_CalendarEvent.GetEventTime()));
    l_Data << uint32(0);    ///< LockDate
    l_Data << uint32(p_CalendarEvent.GetFlags());
    l_Data << int32(p_CalendarEvent.GetDungeonId());
    l_Data << uint8(p_CalendarEvent.GetType());

    l_Data.WriteBits(p_CalendarEvent.GetTitle().size(), 8);
    l_Data.WriteBits(p_CalendarEvent.GetDescription().size(), 11);
    l_Data.WriteBit(true);  ///< ClearPending
    l_Data.FlushBits();
    l_Data.WriteString(p_CalendarEvent.GetTitle());
    l_Data.WriteString(p_CalendarEvent.GetDescription());
    SendPacketToAllEventRelatives(l_Data, p_CalendarEvent);
}

void CalendarMgr::SendCalendarEventInviteStatusAlert(CalendarEvent const& p_CalendarEvent, CalendarInvite const& p_Invite)
{
    WorldPacket l_Data(SMSG_CALENDAR_EVENT_INVITE_STATUS_ALERT);
    l_Data << uint64(p_CalendarEvent.GetEventId());
    l_Data << uint32(p_CalendarEvent.GetFlags());
    l_Data << uint32(p_CalendarEvent.GetEventTime());
    l_Data << uint8(p_Invite.GetStatus());
    SendPacketToAllEventRelatives(l_Data, p_CalendarEvent);
}
/////////////////////////////////////////////////////////////////////

void CalendarMgr::SendCalendarEventModeratorStatus(CalendarEvent const& p_CalendarEvent, CalendarInvite const& p_Invite, uint64 p_Target)
{
    if (Player* l_Player = ObjectAccessor::FindPlayer(p_Target))
    {
        WorldPacket l_Data(SMSG_CALENDAR_EVENT_MODERATOR_STATUS);
        l_Data.appendPackGUID(p_Invite.GetInviteeGUID());
        l_Data << uint64(p_CalendarEvent.GetEventId());
        l_Data << uint8(p_Invite.GetRank());
        l_Data.WriteBit(p_Target == p_CalendarEvent.GetCreatorGUID());  ///< ClearPending
        l_Player->SendDirectMessage(&l_Data);
    }
}

void CalendarMgr::SendCalendarEvent(uint64 p_Guid, CalendarEvent const& p_CalendarEvent, CalendarSendEventType p_SendType)
{
    Player* l_Player = ObjectAccessor::FindPlayer(p_Guid);
    if (!l_Player)
        return;

    CalendarInviteStore const& l_InvitList = m_Invites[p_CalendarEvent.GetEventId()];
    Guild* l_Guild = sGuildMgr->GetGuildById(p_CalendarEvent.GetGuildId());

    WorldPacket l_Data(SMSG_CALENDAR_SEND_EVENT, 1024);
    l_Data << uint8(p_SendType);
    l_Data.appendPackGUID(p_CalendarEvent.GetCreatorGUID());
    l_Data << uint64(p_CalendarEvent.GetEventId());
    l_Data << uint8(p_CalendarEvent.GetType());
    l_Data << int32(p_CalendarEvent.GetDungeonId());
    l_Data << uint32(p_CalendarEvent.GetFlags());
    l_Data << uint32(MS::Utilities::WowTime::Encode(p_CalendarEvent.GetEventTime()));
    l_Data << uint32(MS::Utilities::WowTime::Encode(p_CalendarEvent.GetTimeZoneTime()));
    l_Data.appendPackGUID(l_Guild ? l_Guild->GetGUID() : 0);
    l_Data << uint32(l_InvitList.size());

    for (CalendarInviteStore::const_iterator l_Iter = l_InvitList.begin(); l_Iter != l_InvitList.end(); ++l_Iter)
    {
        CalendarInvite const* l_Invite = (*l_Iter);
        Player* l_Invitee = ObjectAccessor::FindPlayer(l_Invite->GetInviteeGUID());
        uint8 l_Level = l_Invitee ? l_Invitee->getLevel() : Player::GetLevelFromDB(l_Invite->GetInviteeGUID());

        l_Data.appendPackGUID(l_Invite->GetInviteeGUID());
        l_Data << uint64(l_Invite->GetInviteId());
        l_Data << uint8(l_Level);
        l_Data << uint8(l_Invite->GetStatus());
        l_Data << uint8(l_Invite->GetRank());
        l_Data << uint8(p_CalendarEvent.GetGuildId() != 0);
        l_Data << uint32(MS::Utilities::WowTime::Encode(l_Invite->GetStatusTime()));
        l_Data.WriteBits(l_Invite->GetText().size(), 8);
        l_Data.WriteString(l_Invite->GetText());
        l_Data.FlushBits();
    }

    l_Data.WriteBits(p_CalendarEvent.GetTitle().size(), 8);
    l_Data.WriteBits(p_CalendarEvent.GetDescription().size(), 11);
    l_Data.FlushBits();
    l_Data.WriteString(p_CalendarEvent.GetTitle());
    l_Data.WriteString(p_CalendarEvent.GetDescription());

    l_Player->SendDirectMessage(&l_Data);
}

void CalendarMgr::SendCalendarClearPendingAction(uint64 p_Guid)
{
    if (Player* l_Player = ObjectAccessor::FindPlayer(p_Guid))
    {
        WorldPacket l_Data(SMSG_CALENDAR_CLEAR_PENDING_ACTION, 0);
        l_Player->SendDirectMessage(&l_Data);
    }
}

void CalendarMgr::SendCalendarCommandResult(uint64 p_Guid, CalendarError p_Error, char const* p_Param /*= NULL*/)
{
    if (Player* l_Player = ObjectAccessor::FindPlayer(p_Guid))
    {
        WorldPacket l_Data(SMSG_CALENDAR_COMMAND_RESULT, 0);
        l_Data << uint8(0);             ///< Command
        l_Data << uint8(p_Error);       ///< Result

        switch (p_Error)
        {
            case CALENDAR_ERROR_OTHER_INVITES_EXCEEDED:
            case CALENDAR_ERROR_ALREADY_INVITED_TO_EVENT_S:
            case CALENDAR_ERROR_IGNORING_YOU_S:
                l_Data.WriteBits(strlen(p_Param) / 2, 8);
                l_Data.WriteBit(strlen(p_Param) % 2);
                l_Data.FlushBits();
                l_Data.WriteString(p_Param);
                break;
            default:
                l_Data.WriteBits(0, 9);
                break;
        }

        l_Player->SendDirectMessage(&l_Data);
    }
}

void CalendarMgr::SendPacketToAllEventRelatives(WorldPacket& p_Packet, CalendarEvent const& p_CalendarEvent, bool p_ToAll /*= false*/)
{
    // Send packet to all guild members
    if (p_CalendarEvent.IsGuildEvent() || p_CalendarEvent.IsGuildAnnouncement())
    {
        if (Guild* l_Guild = sGuildMgr->GetGuildById(p_CalendarEvent.GetGuildId()))
            l_Guild->BroadcastPacket(&p_Packet);
    }

    // Send packet to all invitees if event is non-guild, in other case only to non-guild invitees (packet was broadcasted for them)
    CalendarInviteStore l_Invites = m_Invites[p_CalendarEvent.GetEventId()];
    for (CalendarInviteStore::iterator l_Iter = l_Invites.begin(); l_Iter != l_Invites.end(); ++l_Iter)
    {
        if ((*l_Iter)->GetInviteeGUID() == p_CalendarEvent.GetCreatorGUID() && !p_ToAll)
            continue;

        if (Player* l_Player = ObjectAccessor::FindPlayer((*l_Iter)->GetInviteeGUID()))
        {
            if (!p_CalendarEvent.IsGuildEvent() || (p_CalendarEvent.IsGuildEvent() && l_Player->GetGuildId() != p_CalendarEvent.GetGuildId()))
                l_Player->SendDirectMessage(&p_Packet);
        }
    }
}
/////////////////////////////////////////////////////////////////////
#endif
