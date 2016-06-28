////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#ifndef TRINITY_CALENDARMGR_H
#define TRINITY_CALENDARMGR_H

#include "Common.h"
#include "DatabaseEnv.h"
#include "WorldPacket.h"

enum CalendarMailAnswers
{
    CALENDAR_EVENT_REMOVED_MAIL_SUBJECT     = 0,
    CALENDAR_INVITE_REMOVED_MAIL_SUBJECT    = 0x100
};

enum CalendarFlags
{
    CALENDAR_FLAG_ALL_ALLOWED       = 0x001,
    CALENDAR_FLAG_INVITES_LOCKED    = 0x010,
    CALENDAR_FLAG_WITHOUT_INVITES   = 0x040,
    CALENDAR_FLAG_GUILD_EVENT       = 0x400
};

enum CalendarModerationRank
{
    CALENDAR_RANK_PLAYER    = 0,
    CALENDAR_RANK_MODERATOR = 1,
    CALENDAR_RANK_OWNER     = 2
};

enum CalendarSendEventType
{
    CALENDAR_SENDTYPE_GET   = 0,
    CALENDAR_SENDTYPE_ADD   = 1,
    CALENDAR_SENDTYPE_COPY  = 2
};

enum CalendarEventType
{
    CALENDAR_TYPE_RAID      = 0,
    CALENDAR_TYPE_DUNGEON   = 1,
    CALENDAR_TYPE_PVP       = 2,
    CALENDAR_TYPE_MEETING   = 3,
    CALENDAR_TYPE_OTHER     = 4
};

enum CalendarRepeatType
{
    CALENDAR_REPEAT_NEVER       = 0,
    CALENDAR_REPEAT_WEEKLY      = 1,
    CALENDAR_REPEAT_BIWEEKLY    = 2,
    CALENDAR_REPEAT_MONTHLY     = 3
};

enum CalendarInviteStatus
{
    CALENDAR_STATUS_INVITED         = 0,
    CALENDAR_STATUS_ACCEPTED        = 1,
    CALENDAR_STATUS_DECLINED        = 2,
    CALENDAR_STATUS_CONFIRMED       = 3,
    CALENDAR_STATUS_OUT             = 4,
    CALENDAR_STATUS_STANDBY         = 5,
    CALENDAR_STATUS_SIGNED_UP       = 6,
    CALENDAR_STATUS_NOT_SIGNED_UP   = 7,
    CALENDAR_STATUS_TENTATIVE       = 8,
    CALENDAR_STATUS_REMOVED         = 9     // correct name?
};

enum CalendarError
{
    CALENDAR_OK                                 = 0,
    CALENDAR_ERROR_GUILD_EVENTS_EXCEEDED        = 1,
    CALENDAR_ERROR_EVENTS_EXCEEDED              = 2,
    CALENDAR_ERROR_SELF_INVITES_EXCEEDED        = 3,
    CALENDAR_ERROR_OTHER_INVITES_EXCEEDED       = 4,
    CALENDAR_ERROR_PERMISSIONS                  = 5,
    CALENDAR_ERROR_EVENT_INVALID                = 6,
    CALENDAR_ERROR_NOT_INVITED                  = 7,
    CALENDAR_ERROR_INTERNAL                     = 8,
    CALENDAR_ERROR_GUILD_PLAYER_NOT_IN_GUILD    = 9,
    CALENDAR_ERROR_ALREADY_INVITED_TO_EVENT_S   = 10,
    CALENDAR_ERROR_PLAYER_NOT_FOUND             = 11,
    CALENDAR_ERROR_NOT_ALLIED                   = 12,
    CALENDAR_ERROR_IGNORING_YOU_S               = 13,
    CALENDAR_ERROR_INVITES_EXCEEDED             = 14,
    CALENDAR_ERROR_INVALID_DATE                 = 16,
    CALENDAR_ERROR_INVALID_TIME                 = 17,
    CALENDAR_ERROR_NEEDS_TITLE                  = 19,
    CALENDAR_ERROR_EVENT_PASSED                 = 20,
    CALENDAR_ERROR_EVENT_LOCKED                 = 21,
    CALENDAR_ERROR_DELETE_CREATOR_FAILED        = 22,
    CALENDAR_ERROR_SYSTEM_DISABLED              = 24,
    CALENDAR_ERROR_RESTRICTED_ACCOUNT           = 25,
    CALENDAR_ERROR_ARENA_EVENTS_EXCEEDED        = 26,
    CALENDAR_ERROR_RESTRICTED_LEVEL             = 27,
    CALENDAR_ERROR_USER_SQUELCHED               = 28,
    CALENDAR_ERROR_NO_INVITE                    = 29,
    CALENDAR_ERROR_EVENT_WRONG_SERVER           = 36,
    CALENDAR_ERROR_INVITE_WRONG_SERVER          = 37,
    CALENDAR_ERROR_NO_GUILD_INVITES             = 38,
    CALENDAR_ERROR_INVALID_SIGNUP               = 39,
    CALENDAR_ERROR_NO_MODERATOR                 = 40
};

#define CALENDAR_MAX_EVENTS         30
#define CALENDAR_MAX_GUILD_EVENTS   100
#define CALENDAR_MAX_INVITES        100

struct CalendarInvite
{
    public:
        CalendarInvite(CalendarInvite const& p_CalendarInvite, uint64 p_Invite, uint64 p_Event)
        {
            m_InviteID = p_Invite;
            m_EventID = p_Event;
            m_Invitee = p_CalendarInvite.GetInviteeGUID();
            m_SenderGUID = p_CalendarInvite.GetSenderGUID();
            m_StatusTime = p_CalendarInvite.GetStatusTime();
            m_Status = p_CalendarInvite.GetStatus();
            m_Rank = p_CalendarInvite.GetRank();
            m_Text = p_CalendarInvite.GetText();
        }

        CalendarInvite() : m_InviteID(1), m_EventID(0), m_Invitee(0), m_SenderGUID(0), m_StatusTime(time(NULL)),
            m_Status(CalendarInviteStatus::CALENDAR_STATUS_INVITED), m_Rank(CalendarModerationRank::CALENDAR_RANK_PLAYER), m_Text("") { }

        CalendarInvite(uint64 p_Invite, uint64 p_Event, uint64 p_Invitee, uint64 p_Sender, time_t p_StatusTime,
            CalendarInviteStatus p_Status, CalendarModerationRank p_Rank, std::string p_Text) :
            m_InviteID(p_Invite), m_EventID(p_Event), m_Invitee(p_Invitee), m_SenderGUID(p_Sender), m_StatusTime(p_StatusTime),
            m_Status(p_Status), m_Rank(p_Rank), m_Text(p_Text) { }

        ~CalendarInvite();

        void SetInviteId(uint64 p_InviteID) { m_InviteID = p_InviteID; }
        uint64 GetInviteId() const { return m_InviteID; }

        void SetEventId(uint64 p_EventID) { m_EventID = p_EventID; }
        uint64 GetEventId() const { return m_EventID; }

        void SetSenderGUID(uint64 p_Guid) { m_SenderGUID = p_Guid; }
        uint64 GetSenderGUID() const { return m_SenderGUID; }

        void SetInvitee(uint64 p_Guid) { m_Invitee = p_Guid; }
        uint64 GetInviteeGUID() const { return m_Invitee; }

        void SetStatusTime(time_t p_StatusTime) { m_StatusTime = p_StatusTime; }
        time_t GetStatusTime() const { return m_StatusTime; }

        void SetText(const std::string& p_Text) { m_Text = p_Text; }
        std::string GetText() const { return m_Text; }

        void SetStatus(CalendarInviteStatus p_Status) { m_Status = p_Status; }
        CalendarInviteStatus GetStatus() const { return m_Status; }

        void SetRank(CalendarModerationRank p_Rank) { m_Rank = p_Rank; }
        CalendarModerationRank GetRank() const { return m_Rank; }

    private:
        uint64 m_InviteID;
        uint64 m_EventID;
        uint64 m_Invitee;
        uint64 m_SenderGUID;
        time_t m_StatusTime;
        CalendarInviteStatus m_Status;
        CalendarModerationRank m_Rank;
        std::string m_Text;
};

struct CalendarEvent
{
    public:
        CalendarEvent(CalendarEvent const& p_CalendarEvent, uint64 p_EventID)
        {
            m_EventID = p_EventID;
            m_CreatorGUID = p_CalendarEvent.GetCreatorGUID();
            m_GuildID = p_CalendarEvent.GetGuildId();
            m_Type = p_CalendarEvent.GetType();
            m_DungeonID = p_CalendarEvent.GetDungeonId();
            m_EventTime = p_CalendarEvent.GetEventTime();
            m_Flags = p_CalendarEvent.GetFlags();
            m_TimeZone = p_CalendarEvent.GetTimeZoneTime();
            m_Title = p_CalendarEvent.GetTitle();
            m_Description = p_CalendarEvent.GetDescription();
        }

        CalendarEvent(uint64 p_Event, uint64 p_Creator, uint32 p_Guild, CalendarEventType p_Type, int32 p_Dungeon,
            uint32 p_EventTime, uint32 p_Flags, uint32 p_TimeZone, std::string p_Title, std::string p_Description) :
            m_EventID(p_Event), m_CreatorGUID(p_Creator), m_GuildID(p_Guild), m_Type(p_Type), m_DungeonID(p_Dungeon),
            m_EventTime(p_EventTime), m_Flags(p_Flags), m_TimeZone(p_TimeZone), m_Title(p_Title),
            m_Description(p_Description) { }

        CalendarEvent() : m_EventID(1), m_CreatorGUID(), m_GuildID(0), m_Type(CalendarEventType::CALENDAR_TYPE_OTHER), m_DungeonID(-1), m_EventTime(0),
            m_Flags(0), m_TimeZone(0), m_Title(""), m_Description("") { }

        ~CalendarEvent();

        void SetEventId(uint64 p_EventID) { m_EventID = p_EventID; }
        uint64 GetEventId() const { return m_EventID; }

        void SetCreatorGUID(uint64 p_Guid) { m_CreatorGUID = p_Guid; }
        uint64 GetCreatorGUID() const { return m_CreatorGUID; }

        void SetGuildId(uint32 p_GuildID) { m_GuildID = p_GuildID; }
        uint32 GetGuildId() const { return m_GuildID; }

        void SetTitle(const std::string& p_Title) { m_Title = p_Title; }
        std::string GetTitle() const { return m_Title; }

        void SetDescription(const std::string& p_Description) { m_Description = p_Description; }
        std::string GetDescription() const { return m_Description; }

        void SetType(CalendarEventType p_Type) { m_Type = p_Type; }
        CalendarEventType GetType() const { return m_Type; }

        void SetDungeonId(int32 p_DungeonID) { m_DungeonID = p_DungeonID; }
        int32 GetDungeonId() const { return m_DungeonID; }

        void SetEventTime(uint32 p_EventTime) { m_EventTime = p_EventTime; }
        uint32 GetEventTime() const { return m_EventTime; }

        void SetFlags(uint32 p_Flags) { m_Flags = p_Flags; }
        uint32 GetFlags() const { return m_Flags; }

        void SetTimeZoneTime(uint32 p_TimeZone) { m_TimeZone = p_TimeZone; }
        uint32 GetTimeZoneTime() const { return m_TimeZone; }

        bool IsGuildEvent() const { return (m_Flags & CALENDAR_FLAG_GUILD_EVENT) != 0; }
        bool IsGuildAnnouncement() const { return (m_Flags & CALENDAR_FLAG_WITHOUT_INVITES) != 0; }

        std::string BuildCalendarMailSubject(uint64 p_Remover) const;
        std::string BuildCalendarMailBody() const;

    private:
        uint64 m_EventID;
        uint64 m_CreatorGUID;
        uint32 m_GuildID;
        CalendarEventType m_Type;
        int32 m_DungeonID;
        uint32 m_EventTime;
        uint32 m_Flags;
        uint32 m_TimeZone;
        std::string m_Title;
        std::string m_Description;
};

typedef std::vector<CalendarInvite*> CalendarInviteStore;
typedef std::set<CalendarEvent*> CalendarEventStore;
typedef std::map<uint64 /* eventId */, CalendarInviteStore > CalendarEventInviteStore;

class CalendarMgr
{
    private:
        CalendarMgr();
        ~CalendarMgr();

        CalendarEventStore m_Events;
        CalendarEventInviteStore m_Invites;

        std::deque<uint64> m_FreeEventIDs;
        std::deque<uint64> m_FreeInviteIDs;
        uint64 m_MaxEventID;
        uint64 m_MaxInviteID;

    public:
        static CalendarMgr* instance()
        {
            static CalendarMgr instance;
            return &instance;
        }

        void LoadFromDB();

        CalendarEvent* GetEvent(uint64 p_EventID) const;
        CalendarEventStore const& GetEvents() const { return m_Events; }
        CalendarEventStore GetPlayerEvents(uint64 p_Guid);

        CalendarInvite* GetInvite(uint64 p_InviteID) const;
        CalendarEventInviteStore const& GetInvites() const { return m_Invites; }
        CalendarInviteStore const& GetEventInvites(uint64 p_EventID);
        CalendarInviteStore GetPlayerInvites(uint64 p_Guid);

        void FreeEventId(uint64 p_ID);
        uint64 GetFreeEventId();
        void FreeInviteId(uint64 p_ID);
        uint64 GetFreeInviteId();

        uint32 GetPlayerNumPending(uint64 p_Guid);

        void AddEvent(CalendarEvent* p_CalendarEvent, CalendarSendEventType p_SendType);
        void AddInvite(CalendarEvent* p_CalendarEvent, CalendarInvite* p_Invite);
        void AddInvite(CalendarEvent* p_CalendarEvent, CalendarInvite* p_Invite, SQLTransaction& p_Transaction);
        void RemoveEvent(uint64 p_EventID, uint64 p_Remover);
        void RemoveInvite(uint64 p_InviteID, uint64 p_EventID, uint64 p_Remover);
        void UpdateEvent(CalendarEvent* p_CalendarEvent);
        void UpdateInvite(CalendarInvite* p_Invite);
        void UpdateInvite(CalendarInvite* p_Invite, SQLTransaction& p_Transaction);

        void RemoveAllPlayerEventsAndInvites(uint64 p_Guid);
        void RemovePlayerGuildEventsAndSignups(uint64 p_Guid, uint32 p_GuildID);

        /////////////////////////////////////////////////////////////////////
        /// UpdateStatus - Must be sent to Creator
        void SendCalendarEventInvite(CalendarInvite const& p_Invite);
        void SendCalendarEventInviteStatus(uint64 p_Guid, CalendarEvent const& p_CalendarEvent, CalendarInvite const& p_Invite);
        void SendCalendarEventInviteRemove(CalendarEvent const& p_CalendarEvent, CalendarInvite const& p_Invite, uint32 p_Flags);
        /////////////////////////////////////////////////////////////////////

        /////////////////////////////////////////////////////////////////////
        /// StatusAlert - Must be sent to concerned players
        void SendCalendarEventInviteAlert(CalendarEvent const& p_CalendarEvent, CalendarInvite const& p_Invite);
        void SendCalendarEventInviteRemoveAlert(uint64 p_Guid, CalendarEvent const& p_CalendarEvent, CalendarInviteStatus p_Status);
        void SendCalendarEventRemovedAlert(CalendarEvent const& p_CalendarEvent);
        void SendCalendarEventUpdateAlert(CalendarEvent const& p_CalendarEvent, uint32 p_OldEventTime);
        void SendCalendarEventInviteStatusAlert(CalendarEvent const& p_CalendarEvent, CalendarInvite const& p_Invite);
        /////////////////////////////////////////////////////////////////////

        void SendCalendarEventModeratorStatus(CalendarEvent const& p_CalendarEvent, CalendarInvite const& p_Invite, uint64 p_Target);
        void SendCalendarEvent(uint64 p_Guid, CalendarEvent const& p_CalendarEvent, CalendarSendEventType p_SendType);
        void SendCalendarClearPendingAction(uint64 p_Guid);
        void SendCalendarCommandResult(uint64 p_Guid, CalendarError p_Error, char const* p_Param = NULL);
        void SendPacketToAllEventRelatives(WorldPacket& p_Packet, CalendarEvent const& p_CalendarEvent, bool p_ToAll = false);
};

#define sCalendarMgr CalendarMgr::instance()

#endif
#endif
