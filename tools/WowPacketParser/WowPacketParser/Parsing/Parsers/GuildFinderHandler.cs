using System;
using WowPacketParser.Enums;
using WowPacketParser.Misc;

namespace WowPacketParser.Parsing.Parsers
{
    public static class GuildFinderHandler
    {

        [Parser(Opcode.CMSG_LF_GUILD_BROWSE)]
        public static void HandleGuildFinderBrowse(Packet packet)
        {
            packet.ReadEnum<GuildFinderOptionsRoles>("Class Roles", TypeCode.UInt32);
            packet.ReadEnum<GuildFinderOptionsAvailability>("Availability", TypeCode.UInt32);
            packet.ReadEnum<GuildFinderOptionsInterest>("Guild Interests", TypeCode.UInt32);
            packet.ReadUInt32("Player Level");
        }

        [Parser(Opcode.CMSG_LF_GUILD_SET_GUILD_POST, ClientVersionBuild.V4_2_2_14545, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildFinderSetGuildPost422(Packet packet)
        {
            packet.ReadBit("Join");
            packet.ReadEnum<GuildFinderOptionsAvailability>("Availability", TypeCode.UInt32);
            packet.ReadEnum<GuildFinderOptionsRoles>("Class Roles", TypeCode.UInt32);
            packet.ReadEnum<GuildFinderOptionsInterest>("Guild Interests", TypeCode.UInt32);
            packet.ReadEnum<GuildFinderOptionsLevel>("Level", TypeCode.UInt32);
            packet.ReadCString("Comment");
        }

        [Parser(Opcode.CMSG_LF_GUILD_SET_GUILD_POST, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildFinderSetGuildPost434(Packet packet)
        {
            packet.ReadEnum<GuildFinderOptionsLevel>("Level", TypeCode.UInt32);
            packet.ReadEnum<GuildFinderOptionsAvailability>("Availability", TypeCode.UInt32);
            packet.ReadEnum<GuildFinderOptionsInterest>("Guild Interests", TypeCode.UInt32);
            packet.ReadEnum<GuildFinderOptionsRoles>("Class Roles", TypeCode.UInt32);
            var length = packet.ReadBits(11);
            packet.ReadBit("Listed");
            packet.ReadWoWString("Comment", length);
        }

        [Parser(Opcode.SMSG_LF_GUILD_POST_UPDATED)]
        public static void HandleGuildFinderPostUpdated(Packet packet)
        {
            var b = packet.ReadBit("Unk Bit"); // Can set settings ?

            if (b != 0)
            {
                var length = packet.ReadBits(11);
                packet.ReadBit("Listed");
                // Flush bits
                packet.ReadEnum<GuildFinderOptionsLevel>("Level", TypeCode.UInt32);
                packet.ReadWoWString("Comment", length);
                packet.ReadInt32("Unk Int32");
                packet.ReadEnum<GuildFinderOptionsAvailability>("Availability", TypeCode.UInt32);
                packet.ReadEnum<GuildFinderOptionsRoles>("Class Roles", TypeCode.UInt32);
                packet.ReadEnum<GuildFinderOptionsInterest>("Guild Interests", TypeCode.UInt32);
            }
        }

        [Parser(Opcode.SMSG_LF_GUILD_COMMAND_RESULT)]
        public static void HandleGuildFinderCommandResult(Packet packet)
        {
            packet.ReadByte("Unk Byte"); // == 0 -> ERR_GUILD_INTERNAL
            packet.ReadInt32("Unk Int32");
        }

        [Parser(Opcode.SMSG_LF_GUILD_BROWSE_UPDATED)]
        public static void HandlerLFGuildBrowseUpdated(Packet packet)
        {
            var count = packet.ReadBits("Count", 18);

            var guids = new byte[count][];
            var guildsLen = new uint[count];
            var commentsLen = new uint[count];

            for (int i = 0; i < count; ++i)
            {
                guids[i] = new byte[8];

                guids[i][4] = packet.ReadBit();
                guildsLen[i] = packet.ReadBits(7);
                guids[i][0] = packet.ReadBit();
                guids[i][1] = packet.ReadBit();
                commentsLen[i] = packet.ReadBits(10);
                guids[i][2] = packet.ReadBit();
                guids[i][7] = packet.ReadBit();
                guids[i][3] = packet.ReadBit();
                guids[i][6] = packet.ReadBit();
                guids[i][5] = packet.ReadBit();
            }

            for (int i = 0; i < count; ++i)
            {
                packet.ReadBoolean("Request Pending", i);
                packet.ReadInt32("Level", i);
                packet.ReadEnum<GuildFinderOptionsInterest>("Guild Interests", TypeCode.UInt32, i);
                packet.ReadInt32("Unk Int 2", i);
                packet.ReadInt32("Tabard Border Style", i);
                packet.ReadXORByte(guids[i], 1);
                packet.ReadXORByte(guids[i], 4);
                packet.ReadInt32("Tabard Emblem Style", i);
                packet.ReadXORByte(guids[i], 3);
                packet.ReadUInt32("Unk Flags");
                packet.ReadInt32("Tabard Emblem Color", i);
                packet.ReadWoWString("Name", guildsLen[i], i);
                packet.ReadEnum<GuildFinderOptionsRoles>("Class Roles", TypeCode.UInt32, i);
                packet.ReadEnum<GuildFinderOptionsAvailability>("Availability", TypeCode.UInt32, i);
                packet.ReadWoWString("Comment", commentsLen[i], i);
                packet.ReadXORByte(guids[i], 6);
                packet.ReadXORByte(guids[i], 7);
                packet.ReadBoolean("Cached", i);
                packet.ReadXORByte(guids[i], 5);
                packet.ReadXORByte(guids[i], 0);
                packet.ReadInt32("Tabard Border Color", i);
                packet.ReadInt32("Number of Members", i);
                packet.ReadInt32("Achievement Points", i);
                packet.ReadXORByte(guids[i], 2);
                packet.ReadInt32("Tabard Background Color", i);

                packet.WriteGuid("Guild Guid", guids[i], i);
            }
        }

        [Parser(Opcode.CMSG_LF_GUILD_GET_RECRUITS)]
        public static void HandlerLFGuildGetRecruits(Packet packet)
        {
            packet.ReadTime("Unk Time");
        }

        [Parser(Opcode.SMSG_LF_GUILD_RECRUIT_LIST_UPDATED)] // 4.3.4
        public static void HandlerLFGuildRecruitListUpdated(Packet packet)
        {
            var count = packet.ReadBits("Count", 20);

            var guids = new byte[count][];
            var strlen = new uint[count][];

            for (int i = 0; i < count; ++i)
            {
                guids[i] = new byte[8];
                strlen[i] = new uint[2];

                strlen[i][0] = packet.ReadBits(11);
                guids[i][2] = packet.ReadBit();
                guids[i][4] = packet.ReadBit();
                guids[i][3] = packet.ReadBit();
                guids[i][7] = packet.ReadBit();
                guids[i][0] = packet.ReadBit();
                strlen[i][1] = packet.ReadBits(7);
                guids[i][5] = packet.ReadBit();
                guids[i][1] = packet.ReadBit();
                guids[i][6] = packet.ReadBit();
            }

            for (int i = 0; i < count; ++i)
            {
                packet.ReadXORByte(guids[i], 4);

                packet.ReadInt32("Unk Int32 1", i); // Is expired ?

                packet.ReadXORByte(guids[i], 3);
                packet.ReadXORByte(guids[i], 0);
                packet.ReadXORByte(guids[i], 1);

                packet.ReadInt32("Player level", i);

                packet.ReadXORByte(guids[i], 6);
                packet.ReadXORByte(guids[i], 7);
                packet.ReadXORByte(guids[i], 2);

                packet.ReadInt32("Time Since", i); // Time (in seconds) since the application was submitted.
                packet.ReadEnum<GuildFinderOptionsAvailability>("Availability", TypeCode.UInt32, i);
                packet.ReadEnum<GuildFinderOptionsRoles>("Class Roles", TypeCode.UInt32, i);
                packet.ReadEnum<GuildFinderOptionsInterest>("Guild Interests", TypeCode.UInt32, i);
                packet.ReadInt32("Time Left", i); // Time (in seconds) until the application will expire.

                packet.ReadWoWString("Character Name", strlen[i][1], i);
                packet.ReadWoWString("Comment", strlen[i][0], i);

                packet.ReadEnum<Class>("Class", TypeCode.UInt32, i);

                packet.ReadXORByte(guids[i], 5);

                packet.WriteGuid("Guid", guids[i], i);
            }

            packet.ReadTime("Unk Time");
        }

        [Parser(Opcode.SMSG_LF_GUILD_MEMBERSHIP_LIST_UPDATED)]
        public static void HandlerLFGuildMembershipListUpdated(Packet packet)
        {
            packet.ReadUInt32("Application left");
            var count = packet.ReadBits("Count", 19);

            var guids = new byte[count][];
            var guildsName = new uint[count];
            var comments = new uint[count];

            for (int i = 0; i < count; ++i)
            {
                guids[i] = new byte[8];

                comments[i] = packet.ReadBits("commentLen", 10, i);
                guildsName[i] = packet.ReadBits("guildLen", 7, i);
                guids[i][2] = packet.ReadBit();
                guids[i][1] = packet.ReadBit();
                guids[i][4] = packet.ReadBit();
                guids[i][0] = packet.ReadBit();
                guids[i][6] = packet.ReadBit();
                guids[i][3] = packet.ReadBit();
                guids[i][5] = packet.ReadBit();
                guids[i][7] = packet.ReadBit();
            }

            for (int i = 0; i < count; ++i)
            {
                packet.ReadInt32("Unk flags", i); // always 50397223 ?
                packet.ReadXORByte(guids[i], 1);
                packet.ReadXORByte(guids[i], 5);
                packet.ReadXORByte(guids[i], 6);
                packet.ReadWoWString("Request comment", comments[i], i);
                packet.ReadXORByte(guids[i], 0);
                packet.ReadXORByte(guids[i], 2);
                packet.ReadEnum<GuildFinderOptionsRoles>("Class Roles", TypeCode.UInt32, i);
                packet.ReadXORByte(guids[i], 4);
                packet.ReadEnum<GuildFinderOptionsAvailability>("Availability", TypeCode.UInt32, i);
                packet.ReadWoWString("Guild name", guildsName[i], i);
                packet.ReadInt32("Time since", i);
                packet.ReadEnum<GuildFinderOptionsInterest>("Guild Interests", TypeCode.UInt32, i);
                packet.ReadInt32("Time Left", i);
                packet.ReadXORByte(guids[i], 7);
                packet.ReadXORByte(guids[i], 3);

                packet.WriteGuid("Guid", guids[i], i);
            }
        }

        [Parser(Opcode.CMSG_LF_GUILD_DECLINE_RECRUIT)] // 4.3.4
        public static void HandleLFGuildDeclineRecruit(Packet packet)
        {
            var guid = packet.StartBitStream(1, 4, 5, 2, 6, 7, 0, 3);
            packet.ParseBitStream(guid, 5, 7, 2, 3, 4, 1, 0, 6);
            packet.WriteGuid("Guid", guid);
        }

        [Parser(Opcode.CMSG_LF_GUILD_REMOVE_RECRUIT)] // 4.3.4
        public static void HandleLFGuildRemoveRecruit(Packet packet)
        {
            var guid = packet.StartBitStream(0, 4, 3, 5, 7, 6, 2, 1);
            packet.ParseBitStream(guid, 4, 0, 3, 6, 5, 1, 2, 7);
            packet.WriteGuid("Guid", guid);
        }

        [Parser(Opcode.CMSG_LF_GUILD_ADD_RECRUIT)]
        public static void HandleLFGuildAddRecruit(Packet packet)
        {
            var guid = new byte[8];

            packet.ReadEnum<GuildFinderOptionsRoles>("Class Roles", TypeCode.UInt32);
            packet.ReadEnum<GuildFinderOptionsInterest>("Guild Interests", TypeCode.UInt32);
            packet.ReadEnum<GuildFinderOptionsAvailability>("Availability", TypeCode.UInt32);

            guid[3] = packet.ReadBit();
            guid[0] = packet.ReadBit();
            guid[6] = packet.ReadBit();
            guid[1] = packet.ReadBit();
            var comment = packet.ReadBits(11);
            guid[5] = packet.ReadBit();
            guid[4] = packet.ReadBit();
            guid[7] = packet.ReadBit();
            var player = packet.ReadBits(7);
            guid[2] = packet.ReadBit();

            packet.ReadXORByte(guid, 4);
            packet.ReadXORByte(guid, 5);
            packet.ReadWoWString("Comment", comment);
            packet.ReadWoWString("Player name", player);
            packet.ReadXORByte(guid, 7);
            packet.ReadXORByte(guid, 2);
            packet.ReadXORByte(guid, 0);
            packet.ReadXORByte(guid, 6);
            packet.ReadXORByte(guid, 1);
            packet.ReadXORByte(guid, 3);

            packet.WriteGuid("Guild GUID", guid);
        }

        [Parser(Opcode.CMSG_LF_GUILD_POST_REQUEST)]
        [Parser(Opcode.CMSG_LF_GUILD_GET_APPLICATIONS)]
        [Parser(Opcode.SMSG_LF_GUILD_APPLICANT_LIST_UPDATED)]
        [Parser(Opcode.SMSG_LF_GUILD_APPLICATIONS_LIST_CHANGED)]
        public static void HandlerLFGuildZeroLength(Packet packet)
        {
        }
    }
}
