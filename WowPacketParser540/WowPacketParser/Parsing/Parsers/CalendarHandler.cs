using System;
using WowPacketParser.Enums;
using WowPacketParser.Misc;

namespace WowPacketParser.Parsing.Parsers
{
    public static class CalendarHandler
    {
        [Parser(Opcode.SMSG_CALENDAR_SEND_CALENDAR)]
        public static void HandleSendCalendar(Packet packet)
        {
            var eventCount = packet.ReadBits("Event count", 19);

            var titleLengths = new uint[eventCount];
            var eventGuildGuids = new byte[eventCount][];
            var eventCreatorGuids = new byte[eventCount][];
            for (int i = 0; i < eventCount; i++)
            {
                eventGuildGuids[i] = new byte[8];
                eventCreatorGuids[i] = new byte[8];

                titleLengths[i] = packet.ReadBits("titleLength", 8, i);

                eventCreatorGuids[i][1] = packet.ReadBit();
                eventGuildGuids[i][7] = packet.ReadBit();
                eventCreatorGuids[i][0] = packet.ReadBit();
                eventGuildGuids[i][3] = packet.ReadBit();
                eventGuildGuids[i][6] = packet.ReadBit();
                eventGuildGuids[i][5] = packet.ReadBit();
                eventCreatorGuids[i][7] = packet.ReadBit();
                eventCreatorGuids[i][2] = packet.ReadBit();
                eventCreatorGuids[i][6] = packet.ReadBit();
                eventGuildGuids[i][0] = packet.ReadBit();
                eventGuildGuids[i][1] = packet.ReadBit();
                eventGuildGuids[i][2] = packet.ReadBit();
                eventGuildGuids[i][4] = packet.ReadBit();
                eventCreatorGuids[i][4] = packet.ReadBit();
                eventCreatorGuids[i][3] = packet.ReadBit();
                eventCreatorGuids[i][5] = packet.ReadBit();
            }

            var invCount = packet.ReadBits("Invite Count", 19);

            var invCreatorGuids = new byte[invCount][];
            for (int i = 0; i < invCount; i++)
            {
                invCreatorGuids[i] = new byte[8];

                packet.StartBitStream(invCreatorGuids[i], 1, 6, 2, 3, 5, 7, 4, 0);
            }

            var raidResetCount = packet.ReadBits("Raid Reset Count", 20);
            var holidayCount = packet.ReadBits("Holiday Count", 16);

            var holidayLengths = new uint[holidayCount];
            for (int i = 0; i < holidayCount; i++)
                holidayLengths[i] = packet.ReadBits("holidayLength", 6, i);

            var instanceResetCount = packet.ReadBits("instanceResetCount", 20);

            var unkGuids = new byte[instanceResetCount][];
            for (int i = 0; i < instanceResetCount; i++)
            {
                unkGuids[i] = new byte[8];

                packet.StartBitStream(unkGuids[i], 2, 7, 1, 6, 3, 5, 4, 0);
            }

            for (int i = 0; i < instanceResetCount; i++)
            {
                packet.ReadUInt32("Unk UInt32", i); // Map ID, Difficulty or Time Left
                packet.ReadXORByte(unkGuids[i], 1);
                packet.ReadUInt32("Unk UInt32", i); // Map ID, Difficulty or Time Left
                packet.ReadXORByte(unkGuids[i], 4);
                packet.ReadXORByte(unkGuids[i], 3);
                packet.ReadXORByte(unkGuids[i], 7);
                packet.ReadUInt32("Unk UInt32", i); // Map ID, Difficulty or Time Left
                packet.ReadXORByte(unkGuids[i], 6);
                packet.ReadXORByte(unkGuids[i], 5);
                packet.ReadXORByte(unkGuids[i], 0);
                packet.ReadXORByte(unkGuids[i], 2);

                packet.WriteGuid("Instance GUID", unkGuids[i], i);
            }

            for (int i = 0; i < raidResetCount; i++)
            {
                packet.ReadEntryWithName<UInt32>(StoreNameType.Map, "Map Id");
                packet.ReadUInt32("Unk Time", i);
                packet.ReadUInt32("Time Left", i);
            }

            for (int i = 0; i < holidayCount; i++)
            {
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);

                packet.ReadWoWString("holidayName", holidayLengths[i], i);

                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadUInt32("Unk UInt32", i);
            }

            for (int i = 0; i < eventCount; i++)
            {
                packet.ReadXORByte(eventGuildGuids[i], 6);
                packet.ReadXORByte(eventGuildGuids[i], 7);
                packet.ReadUInt32("Unk UInt32", i); // Time, Flags or Dungeon ID
                packet.ReadXORByte(eventCreatorGuids[i], 1);
                packet.ReadXORByte(eventGuildGuids[i], 2);
                packet.ReadXORByte(eventCreatorGuids[i], 6);
                packet.ReadXORByte(eventCreatorGuids[i], 4);
                packet.ReadXORByte(eventCreatorGuids[i], 7);
                packet.ReadXORByte(eventGuildGuids[i], 3);
                packet.ReadXORByte(eventGuildGuids[i], 5);
                packet.ReadUInt32("Unk UInt32", i); // Time, Flags or Dungeon ID
                packet.ReadXORByte(eventGuildGuids[i], 1);
                packet.ReadXORByte(eventGuildGuids[i], 0);
                packet.ReadUInt64("Event ID", i);
                packet.ReadXORByte(eventCreatorGuids[i], 5);
                packet.ReadXORByte(eventGuildGuids[i], 4);
                packet.ReadXORByte(eventCreatorGuids[i], 3);
                packet.ReadXORByte(eventCreatorGuids[i], 0);

                packet.ReadWoWString("Event Title", titleLengths[i], i);

                packet.ReadXORByte(eventCreatorGuids[i], 2);
                packet.ReadUInt32("Unk UInt32", i); // Time, Flags or Dungeon ID
                packet.ReadEnum<CalendarEventType>("Event Type", TypeCode.Byte, i);

                packet.WriteGuid("Creator GUID", eventCreatorGuids[i], i);
                packet.WriteGuid("Guild GUID", eventGuildGuids[i], i);
            }

            packet.ReadUInt32("Unk UInt32"); // Zone Time ? or Constant Date
            packet.ReadUInt32("Unk UInt32"); // Zone Time ? or Constant Date

            for (int i = 0; i < invCount; i++)
            {
                packet.ReadXORByte(invCreatorGuids[i], 7);
                packet.ReadByte("Unk Byte", i); // Status, Rank or Guild ID
                packet.ReadByte("Unk Byte", i); // Status, Rank or Guild ID
                packet.ReadXORByte(invCreatorGuids[i], 6);
                packet.ReadXORByte(invCreatorGuids[i], 3);
                packet.ReadUInt64("Unk UInt64"); // Event ID or Invite ID
                packet.ReadXORByte(invCreatorGuids[i], 2);
                packet.ReadXORByte(invCreatorGuids[i], 5);
                packet.ReadXORByte(invCreatorGuids[i], 1);
                packet.ReadByte("Unk Byte", i); // Status, Rank or Guild ID
                packet.ReadXORByte(invCreatorGuids[i], 4);
                packet.ReadXORByte(invCreatorGuids[i], 0);
                packet.ReadUInt64("Unk UInt64"); // Event ID or Invite ID

                packet.WriteGuid("InvCreator GUID", invCreatorGuids[i], i);
            }

            packet.ReadTime("Current Time");
        }

        [Parser(Opcode.CMSG_CALENDAR_GET_EVENT)]
        public static void HandleGetCalendarEvent(Packet packet)
        {
            packet.ReadInt64("Event ID");
        }

        [Parser(Opcode.SMSG_CALENDAR_SEND_EVENT)]
        public static void HandleSendCalendarEvent(Packet packet)
        {
            packet.ReadEnum<CalendarSendEventType>("Send Event Type", TypeCode.Byte);
            packet.ReadPackedGuid("Creator GUID");
            packet.ReadInt64("Event ID");
            packet.ReadCString("Title");
            packet.ReadCString("Description");
            packet.ReadEnum<CalendarEventType>("Event Type", TypeCode.Byte);
            packet.ReadEnum<CalendarRepeatType>("Repeat Type", TypeCode.Byte);
            packet.ReadInt32("Max Invites");
            packet.ReadEntryWithName<Int32>(StoreNameType.LFGDungeon, "Dungeon ID");
            packet.ReadEnum<CalendarFlag>("Event Flags", TypeCode.Int32);
            packet.ReadPackedTime("Event Time");
            packet.ReadPackedTime("Unk PackedTime");
            if (ClientVersion.AddedInVersion(ClientType.Cataclysm))
                packet.ReadGuid("Guild Guid");
            else
                packet.ReadInt32("Guild");

            var invCount = packet.ReadInt32("Invite Count");

            for (var i = 0; i < invCount; i++)
            {
                packet.ReadPackedGuid("Invitee GUID", i);
                packet.ReadByte("Player Level", i);
                packet.ReadEnum<CalendarEventStatus>("Status", TypeCode.Byte, i);
                packet.ReadEnum<CalendarModerationRank>("Moderation Rank", TypeCode.Byte, i);
                packet.ReadBoolean("Guild Member", i);
                packet.ReadInt64("Invite ID", i);
                packet.ReadPackedTime("Status Time", i);
                packet.ReadCString("Invite Text", i);
            }
        }

        [Parser(Opcode.CMSG_CALENDAR_GUILD_FILTER)]
        public static void HandleCalendarGuildFilter(Packet packet)
        {
            packet.ReadInt32("Min Level");
            packet.ReadInt32("Max Level");
            packet.ReadInt32("Min Rank");
        }

        [Parser(Opcode.CMSG_CALENDAR_ARENA_TEAM)]
        public static void HandleCalendarArenaTeam(Packet packet)
        {
            packet.ReadInt32("Unk Int32 1");
        }

        [Parser(Opcode.SMSG_CALENDAR_ARENA_TEAM)]
        [Parser(Opcode.SMSG_CALENDAR_FILTER_GUILD)]
        public static void HandleCalendarFilters(Packet packet)
        {
            var count = packet.ReadInt32("Count");

            for (var i = 0; i < count; i++)
            {
                packet.ReadPackedGuid("GUID", i);
                packet.ReadByte("Unk Byte", i);
            }
        }

        [Parser(Opcode.CMSG_CALENDAR_ADD_EVENT)]
        public static void HandleAddCalendarEvent(Packet packet)
        {
            packet.ReadCString("Title");
            packet.ReadCString("Description");
            packet.ReadEnum<CalendarEventType>("Event Type", TypeCode.Byte);
            packet.ReadEnum<CalendarRepeatType>("Repeat Type", TypeCode.Byte);
            packet.ReadInt32("Max Invites");
            packet.ReadEntryWithName<Int32>(StoreNameType.LFGDungeon, "Dungeon ID");
            packet.ReadPackedTime("Event Time");
            packet.ReadPackedTime("Unk PackedTime");

            var flags = packet.ReadEnum<CalendarFlag>("Event Flags", TypeCode.Int32);

            if ((flags & CalendarFlag.GuildAnnouncement) != 0)
                return;

            var count = packet.ReadInt32("Invite Count");

            for (var i = 0; i < count; i++)
            {
                packet.ReadPackedGuid("Creator GUID");
                packet.ReadEnum<CalendarEventStatus>("Status", TypeCode.Byte);
                packet.ReadEnum<CalendarModerationRank>("Moderation Rank", TypeCode.Byte);
            }
        }

        [Parser(Opcode.CMSG_CALENDAR_UPDATE_EVENT)]
        public static void HandleUpdateCalendarEvent(Packet packet)
        {
            packet.ReadInt64("Event ID");
            packet.ReadInt64("Invite ID");
            packet.ReadCString("Title");
            packet.ReadCString("Description");
            packet.ReadEnum<CalendarEventType>("Event Type", TypeCode.Byte);
            packet.ReadEnum<CalendarRepeatType>("Repeat Type", TypeCode.Byte);
            packet.ReadInt32("Max Invites");
            packet.ReadEntryWithName<Int32>(StoreNameType.LFGDungeon, "Dungeon ID");
            packet.ReadPackedTime("Event Time");
            packet.ReadPackedTime("Unk PackedTime");
            packet.ReadEnum<CalendarFlag>("Event Flags", TypeCode.Int32);
        }

        [Parser(Opcode.CMSG_CALENDAR_REMOVE_EVENT)]
        public static void HandleRemoveCalendarEvent(Packet packet)
        {
            packet.ReadInt64("Event ID");
            packet.ReadInt64("Invite ID");
            packet.ReadEnum<CalendarFlag>("Event Flags", TypeCode.Int32);
        }

        [Parser(Opcode.CMSG_CALENDAR_COPY_EVENT)]
        public static void HandleCopyCalendarEvent(Packet packet)
        {
            packet.ReadInt64("Event ID");
            packet.ReadInt64("Invite ID");
            packet.ReadPackedTime("Event Time");
        }

        [Parser(Opcode.CMSG_CALENDAR_EVENT_INVITE)]
        public static void HandleAddCalendarEventInvite(Packet packet)
        {
            packet.ReadInt64("Event ID");
            packet.ReadInt64("Invite ID");
            packet.ReadCString("Name");
            packet.ReadEnum<CalendarEventStatus>("Status", TypeCode.Byte);
            packet.ReadEnum<CalendarModerationRank>("Moderation Rank", TypeCode.Byte);
        }

        [Parser(Opcode.SMSG_CALENDAR_EVENT_INVITE)]
        public static void HandleSendCalendarEventInvite(Packet packet)
        {
            packet.ReadPackedGuid("Invitee GUID");
            packet.ReadInt64("Event ID");
            packet.ReadInt64("Invite ID");
            packet.ReadByte("Player Level");
            packet.ReadEnum<CalendarEventStatus>("Status", TypeCode.Byte);

            if (packet.ReadBoolean("Has Confirm Time"))
                packet.ReadPackedTime("Confirm Time");

            packet.ReadBoolean("Guild Event");
        }

        [Parser(Opcode.CMSG_CALENDAR_EVENT_INVITE_NOTES)]
        public static void HandleCalendarUpdateInviteNotes(Packet packet)
        {
            packet.ReadPackedGuid("Invitee GUID");
            packet.ReadInt64("Invite ID");
            packet.ReadCString("Invite Text");
            packet.ReadBoolean("Unk Boolean");
        }

        [Parser(Opcode.SMSG_CALENDAR_EVENT_INVITE_NOTES_ALERT)]
        public static void HandleCalendarUpdateInviteNotesAlert(Packet packet)
        {
            packet.ReadInt64("Invite ID");
            packet.ReadCString("Invite Text");
        }

        [Parser(Opcode.SMSG_CALENDAR_EVENT_INVITE_NOTES)]
        public static void HandleCalendarSendUpdatedInviteNotes(Packet packet)
        {
            packet.ReadPackedGuid("Invitee GUID");
            packet.ReadInt64("Invite ID");
            packet.ReadCString("Invite Text");
            packet.ReadBoolean("Unk bool");
        }

        [Parser(Opcode.CMSG_CALENDAR_EVENT_REMOVE_INVITE)]
        public static void HandleRemoveCalendarEventInvite(Packet packet)
        {
            packet.ReadPackedGuid("GUID");
            packet.ReadInt64("Invite ID");
            packet.ReadInt64("Unk int64 1");
            packet.ReadInt64("Event ID");
        }

        [Parser(Opcode.SMSG_CALENDAR_EVENT_INVITE_REMOVED)]
        public static void HandleSendCalendarEventInviteRemoved(Packet packet)
        {
            packet.ReadPackedGuid("GUID");
            packet.ReadInt64("Invite ID");
            packet.ReadEnum<CalendarFlag>("Event Flags", TypeCode.Int32);
            packet.ReadByte("Unk Byte");
        }

        [Parser(Opcode.CMSG_CALENDAR_EVENT_STATUS)]
        public static void HandleCalendarEventStatus(Packet packet)
        {
            packet.ReadPackedGuid("Invitee GUID");
            packet.ReadInt64("Event ID");
            packet.ReadInt64("Invite ID");
            packet.ReadInt64("Owner Invite ID"); // sender's invite id?
            packet.ReadEnum<CalendarEventStatus>("Status", TypeCode.Int32);
        }

        [Parser(Opcode.CMSG_CALENDAR_EVENT_MODERATOR_STATUS)]
        public static void HandleCalendarEventModeratorStatus(Packet packet)
        {
            packet.ReadPackedGuid("Invitee GUID");
            packet.ReadInt64("Event ID");
            packet.ReadInt64("Invite ID");
            packet.ReadInt64("Owner Invite ID"); // sender's invite id?
            packet.ReadEnum<CalendarModerationRank>("Rank", TypeCode.Int32);
        }

        [Parser(Opcode.SMSG_CALENDAR_EVENT_STATUS)]
        public static void HandleSendCalendarEventStatus(Packet packet)
        {
            packet.ReadPackedGuid("GUID");
            packet.ReadInt64("Event ID");
            packet.ReadPackedTime("Event Time");
            packet.ReadEnum<CalendarFlag>("Event Flags", TypeCode.Int32);
            packet.ReadEnum<CalendarEventStatus>("Status", TypeCode.Byte);
            packet.ReadEnum<CalendarModerationRank>("Moderation Rank", TypeCode.Byte);
            packet.ReadPackedTime("Event status change time");
        }

        [Parser(Opcode.SMSG_CALENDAR_EVENT_MODERATOR_STATUS_ALERT)]
        public static void HandleSendCalendarEventModeratorStatus(Packet packet)
        {
            packet.ReadPackedGuid("Invitee GUID");
            packet.ReadInt64("Event ID");
            packet.ReadEnum<CalendarModerationRank>("Rank", TypeCode.Byte);
            packet.ReadBoolean("Unk Boolean");
        }

        [Parser(Opcode.CMSG_CALENDAR_COMPLAIN, ClientVersionBuild.Zero, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleCalendarComplain(Packet packet)
        {
            packet.ReadInt64("Event ID");
            packet.ReadGuid("GUID");
        }

        [Parser(Opcode.CMSG_CALENDAR_COMPLAIN, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleCalendarComplain434(Packet packet)
        {
            packet.ReadGuid("GUID");
            packet.ReadInt64("Event ID");
            packet.ReadInt64("Invite ID");
        }

        [Parser(Opcode.SMSG_CALENDAR_SEND_NUM_PENDING)]
        public static void HandleSendCalendarNumPending(Packet packet)
        {
            packet.ReadInt32("Pending Invites");
        }

        [Parser(Opcode.CMSG_CALENDAR_EVENT_RSVP)]
        public static void HandleCalendarRsvp(Packet packet)
        {
            packet.ReadInt64("Event ID");
            packet.ReadInt64("Invite ID");
            packet.ReadEnum<CalendarEventStatus>("Status", TypeCode.Int32);
        }

        [Parser(Opcode.SMSG_CALENDAR_RAID_LOCKOUT_ADDED)]
        public static void HandleRaidLockoutAdded(Packet packet)
        {
            packet.ReadPackedTime("Time");
            packet.ReadEntryWithName<Int32>(StoreNameType.Map, "Map ID");
            packet.ReadEnum<MapDifficulty>("Difficulty", TypeCode.Int32);
            packet.ReadInt32("Reset Time");
            packet.ReadGuid("Instance ID");
        }

        [Parser(Opcode.SMSG_CALENDAR_RAID_LOCKOUT_REMOVED)]
        public static void HandleRaidLockoutRemoved(Packet packet)
        {
            packet.ReadEntryWithName<Int32>(StoreNameType.Map, "Map ID");
            packet.ReadEnum<MapDifficulty>("Difficulty", TypeCode.Int32);
            packet.ReadInt32("Reset Time");
            packet.ReadGuid("Instance ID");
        }

        [Parser(Opcode.SMSG_CALENDAR_RAID_LOCKOUT_UPDATED)]
        public static void HandleCalendarRaidLockoutUpdated(Packet packet)
        {
            packet.ReadPackedTime("Time");
            packet.ReadInt32("Map ID");
            packet.ReadInt32("Difficulty");
            packet.ReadInt32("Time changed (in seconds)");
            packet.ReadInt32("Reset time");
        }

        [Parser(Opcode.SMSG_CALENDAR_EVENT_INVITE_ALERT)]
        public static void HandleCalendarEventInviteAlert(Packet packet)
        {
            packet.ReadInt64("Event ID");
            packet.ReadCString("Title");
            packet.ReadPackedTime("Time");
            packet.ReadEnum<CalendarFlag>("Event Flags", TypeCode.Int32);
            packet.ReadEnum<CalendarEventType>("Type", TypeCode.Int32);
            packet.ReadEntryWithName<Int32>(StoreNameType.LFGDungeon, "Dungeon ID");
            packet.ReadInt64("Invite ID");
            if (ClientVersion.AddedInVersion(ClientVersionBuild.V4_3_0a_15050))
            {
                packet.ReadGuid("Guild GUID");
            }
            packet.ReadEnum<CalendarEventStatus>("Status", TypeCode.Byte);
            packet.ReadEnum<CalendarModerationRank>("Moderation Rank", TypeCode.Byte);
            packet.ReadPackedGuid("Creator GUID");
            packet.ReadPackedGuid("Sender GUID");
        }

        [Parser(Opcode.SMSG_CALENDAR_EVENT_REMOVED_ALERT)]
        public static void HandleCalendarEventRemoveAlert(Packet packet)
        {
            packet.ReadByte("Unk (RemovedAlert)");
            packet.ReadInt64("Event ID");
            packet.ReadPackedTime("Event Time");
        }

        [Parser(Opcode.SMSG_CALENDAR_EVENT_UPDATED_ALERT)]
        public static void HandleCalendarEventUpdateAlert(Packet packet)
        {
            packet.ReadByte("Unk byte (UpdatedAlert)");
            packet.ReadInt64("Event ID");
            packet.ReadPackedTime("Event Time");
            packet.ReadEnum<CalendarFlag>("Event Flags", TypeCode.Int32);
            packet.ReadPackedTime("Time2");
            packet.ReadEnum<CalendarEventType>("Event Type", TypeCode.Byte);
            packet.ReadEntryWithName<Int32>(StoreNameType.LFGDungeon, "Dungeon ID");
            packet.ReadCString("Title");
            packet.ReadCString("Description");
            packet.ReadEnum<CalendarRepeatType>("Repeat Type", TypeCode.Byte);
            packet.ReadInt32("Max Invites");
            packet.ReadInt32("Unk int32 (UpdatedAlert)");
        }

        [Parser(Opcode.SMSG_CALENDAR_COMMAND_RESULT)]
        public static void HandleCalendarCommandResult(Packet packet)
        {
            packet.ReadInt32("UnkInt1");
            packet.ReadCString("UnkString1");
            packet.ReadCString("Param 1"); // if %s is used in the error message
            packet.ReadEnum<CalendarError>("Error", TypeCode.Int32);
        }

        [Parser(Opcode.SMSG_CALENDAR_EVENT_INVITE_REMOVED_ALERT)]
        public static void HandleCalendarEventInviteRemoveAlert(Packet packet)
        {
            packet.ReadInt64("Event ID");
            packet.ReadPackedTime("Event Time");
            packet.ReadEnum<CalendarFlag>("Event Flags", TypeCode.Int32);
            packet.ReadEnum<CalendarEventStatus>("Status", TypeCode.Byte);
        }

        [Parser(Opcode.SMSG_CALENDAR_EVENT_INVITE_STATUS_ALERT)]
        public static void HandleCalendarEventInviteStatusAlert(Packet packet)
        {
            packet.ReadInt64("Event ID");
            packet.ReadPackedTime("Event Time");
            packet.ReadInt32("Unk flag"); // (v38 & 0x440) != 0
            packet.ReadBoolean("DeletePendingInvite");
        }

        [Parser(Opcode.CMSG_CALENDAR_EVENT_SIGNUP)]
        public static void HandleCalendarEventSignup(Packet packet)
        {
            packet.ReadInt64("Event ID");
            packet.ReadEnum<CalendarEventStatus>("Status", TypeCode.Byte);
        }

        [Parser(Opcode.CMSG_CALENDAR_GET_CALENDAR)]
        [Parser(Opcode.CMSG_CALENDAR_GET_NUM_PENDING)]
        [Parser(Opcode.SMSG_CALENDAR_CLEAR_PENDING_ACTION)]
        public static void HandleCalenderNull(Packet packet)
        {
        }
    }
}
