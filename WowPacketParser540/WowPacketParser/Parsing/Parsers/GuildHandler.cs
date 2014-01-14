using System;
using WowPacketParser.Enums;
using WowPacketParser.Misc;
using Guid = WowPacketParser.Misc.Guid;

namespace WowPacketParser.Parsing.Parsers
{
    public static class GuildHandler
    {
        private static void ReadEmblemInfo(ref Packet packet)
        {
            packet.ReadInt32("Emblem Style");
            packet.ReadInt32("Emblem Color");
            packet.ReadInt32("Emblem Border Style");
            packet.ReadInt32("Emblem Border Color");
            packet.ReadInt32("Emblem Background Color");
        }

        [Parser(Opcode.CMSG_GUILD_ASSIGN_MEMBER_RANK)] // 4.3.4
        public static void HandleGuildAssignMemberRank(Packet packet)
        {
            var guid1 = new byte[8];
            var guid2 = new byte[8];

            packet.ReadUInt32("Rank");

            guid1[1] = packet.ReadBit();
            guid1[7] = packet.ReadBit();
            guid2[4] = packet.ReadBit();
            guid2[2] = packet.ReadBit();
            guid1[4] = packet.ReadBit();
            guid1[5] = packet.ReadBit();
            guid1[6] = packet.ReadBit();
            guid2[1] = packet.ReadBit();
            guid2[7] = packet.ReadBit();
            guid1[2] = packet.ReadBit();
            guid1[3] = packet.ReadBit();
            guid1[0] = packet.ReadBit();
            guid2[6] = packet.ReadBit();
            guid2[3] = packet.ReadBit();
            guid2[0] = packet.ReadBit();
            guid2[5] = packet.ReadBit();

            packet.ReadXORByte(guid1, 0);
            packet.ReadXORByte(guid2, 1);
            packet.ReadXORByte(guid2, 3);
            packet.ReadXORByte(guid2, 5);
            packet.ReadXORByte(guid1, 7);
            packet.ReadXORByte(guid1, 3);
            packet.ReadXORByte(guid2, 0);
            packet.ReadXORByte(guid1, 1);
            packet.ReadXORByte(guid2, 6);
            packet.ReadXORByte(guid1, 2);
            packet.ReadXORByte(guid1, 5);
            packet.ReadXORByte(guid1, 4);
            packet.ReadXORByte(guid2, 2);
            packet.ReadXORByte(guid2, 4);
            packet.ReadXORByte(guid1, 6);
            packet.ReadXORByte(guid2, 7);

            packet.WriteGuid("GUID 1", guid1);
            packet.WriteGuid("GUID 2", guid2);
        }

        [Parser(Opcode.CMSG_GUILD_ROSTER, ClientVersionBuild.V4_0_6_13596, ClientVersionBuild.V4_2_2_14545)]
        public static void HandleGuildRequestRoster406(Packet packet)
        {
            packet.ReadGuid("Guild GUID");
            packet.ReadGuid("Player GUID");
        }

        [Parser(Opcode.CMSG_GUILD_ROSTER, ClientVersionBuild.V4_2_2_14545, ClientVersionBuild.V4_3_0_15005)]
        public static void HandleGuildRosterC422(Packet packet)
        {
            var guid = packet.StartBitStream(7, 3, 2, 6, 5, 4, 1, 0);
            packet.ParseBitStream(guid, 7, 4, 5, 0, 1, 2, 6, 3);
            packet.WriteGuid("Guid", guid);
        }

        [Parser(Opcode.CMSG_GUILD_ROSTER, ClientVersionBuild.V4_3_0_15005, ClientVersionBuild.V4_3_3_15354)]
        public static void HandleGuildRoster430(Packet packet)
        {
            // Seems to have some previous formula, processed GUIDS does not fit any know guid
            // ToDo: Fix this.
            var guid1 = new byte[8];
            var guid2 = new byte[8];
            guid2[5] = packet.ReadBit();
            guid1[0] = packet.ReadBit();
            guid1[3] = packet.ReadBit();
            guid2[4] = packet.ReadBit();
            guid2[1] = packet.ReadBit();
            guid2[0] = packet.ReadBit();
            guid2[6] = packet.ReadBit();
            guid1[5] = packet.ReadBit();
            guid2[3] = packet.ReadBit();
            guid2[7] = packet.ReadBit();
            guid1[6] = packet.ReadBit();
            guid2[2] = packet.ReadBit();
            guid1[7] = packet.ReadBit();
            guid1[2] = packet.ReadBit();
            guid1[4] = packet.ReadBit();
            guid1[1] = packet.ReadBit();

            packet.ReadXORByte(guid2, 6);
            packet.ReadXORByte(guid2, 7);
            packet.ReadXORByte(guid2, 1);
            packet.ReadXORByte(guid2, 0);
            packet.ReadXORByte(guid1, 3);
            packet.ReadXORByte(guid2, 2);
            packet.ReadXORByte(guid1, 5);
            packet.ReadXORByte(guid1, 2);
            packet.ReadXORByte(guid1, 4);
            packet.ReadXORByte(guid2, 3);
            packet.ReadXORByte(guid1, 6);
            packet.ReadXORByte(guid2, 4);
            packet.ReadXORByte(guid1, 0);
            packet.ReadXORByte(guid2, 5);
            packet.ReadXORByte(guid1, 1);
            packet.ReadXORByte(guid1, 7);
            packet.WriteGuid("Guid1", guid1);
            packet.WriteGuid("Guid2", guid2);
        }

        [Parser(Opcode.CMSG_GUILD_ROSTER, ClientVersionBuild.V4_3_3_15354, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildRosterRequest433(Packet packet)
        {
            // Seems to have some previous formula, processed GUIDS does not fit any know guid
            var guid1 = new byte[8];
            var guid2 = new byte[8];
            guid2[6] = packet.ReadBit();
            guid1[7] = packet.ReadBit();
            guid1[5] = packet.ReadBit();
            guid1[6] = packet.ReadBit();
            guid1[1] = packet.ReadBit();
            guid2[3] = packet.ReadBit();
            guid2[5] = packet.ReadBit();
            guid2[2] = packet.ReadBit();
            guid2[1] = packet.ReadBit();
            guid1[2] = packet.ReadBit();
            guid1[3] = packet.ReadBit();
            guid1[0] = packet.ReadBit();
            guid2[7] = packet.ReadBit();
            guid2[0] = packet.ReadBit();
            guid2[4] = packet.ReadBit();
            guid1[4] = packet.ReadBit();

            packet.ReadXORByte(guid1, 3);
            packet.ReadXORByte(guid1, 5);
            packet.ReadXORByte(guid2, 5);
            packet.ReadXORByte(guid1, 2);
            packet.ReadXORByte(guid2, 0);
            packet.ReadXORByte(guid2, 3);
            packet.ReadXORByte(guid2, 4);
            packet.ReadXORByte(guid2, 1);
            packet.ReadXORByte(guid2, 7);
            packet.ReadXORByte(guid1, 1);
            packet.ReadXORByte(guid1, 0);
            packet.ReadXORByte(guid2, 6);
            packet.ReadXORByte(guid1, 4);
            packet.ReadXORByte(guid2, 2);
            packet.ReadXORByte(guid1, 7);
            packet.ReadXORByte(guid1, 6);
            packet.WriteGuid("Guid1", guid1);
            packet.WriteGuid("Guid2", guid2);
        }

        [Parser(Opcode.CMSG_GUILD_ROSTER, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildRosterRequest434(Packet packet)
        {
            // Seems to have some previous formula, processed GUIDS does not fit any know guid
            var guid1 = new byte[8];
            var guid2 = new byte[8];
            guid2[2] = packet.ReadBit();
            guid2[3] = packet.ReadBit();
            guid1[6] = packet.ReadBit();
            guid1[0] = packet.ReadBit();
            guid2[7] = packet.ReadBit();
            guid1[2] = packet.ReadBit();
            guid2[6] = packet.ReadBit();
            guid2[4] = packet.ReadBit();
            guid1[1] = packet.ReadBit();
            guid2[5] = packet.ReadBit();
            guid1[4] = packet.ReadBit();
            guid1[3] = packet.ReadBit();
            guid2[0] = packet.ReadBit();
            guid1[5] = packet.ReadBit();
            guid2[1] = packet.ReadBit();
            guid1[7] = packet.ReadBit();

            packet.ReadXORByte(guid1, 3);
            packet.ReadXORByte(guid2, 4);
            packet.ReadXORByte(guid1, 7);
            packet.ReadXORByte(guid1, 2);
            packet.ReadXORByte(guid1, 4);
            packet.ReadXORByte(guid1, 0);
            packet.ReadXORByte(guid2, 5);
            packet.ReadXORByte(guid1, 1);
            packet.ReadXORByte(guid2, 0);
            packet.ReadXORByte(guid2, 6);
            packet.ReadXORByte(guid1, 5);
            packet.ReadXORByte(guid2, 7);
            packet.ReadXORByte(guid2, 2);
            packet.ReadXORByte(guid2, 3);
            packet.ReadXORByte(guid2, 1);
            packet.ReadXORByte(guid1, 6);
            packet.WriteGuid("Guid1", guid1);
            packet.WriteGuid("Guid2", guid2);
        }

        [Parser(Opcode.SMSG_GUILD_ROSTER)]
        public static void HandleGuildRoster(Packet packet)
        {
            packet.ReadPackedTime("Creation Date");
            packet.ReadUInt32("Accounts Number");
            packet.ReadUInt32("Unk UInt32"); // 0
            packet.ReadUInt32("Weekly Reputation Cap"); // Not used since 5.0.4

            var motdLength = packet.ReadBits("motdLen", 10);
            var size = packet.ReadBits("memberCount", 17);

            var guid = new byte[size][];
            var nameLength = new uint[size];
            var publicLength = new uint[size];
            var officerLength = new uint[size];

            for (var i = 0; i < size; ++i)
            {
                guid[i] = new byte[8];

                packet.ReadBit("Has Authenticator", i);
                guid[i][0] = packet.ReadBit();
                guid[i][7] = packet.ReadBit();
                guid[i][2] = packet.ReadBit();
                packet.ReadBit("Can SoR", i);
                publicLength[i] = packet.ReadBits("publicNoteLen", 8, i);
                guid[i][3] = packet.ReadBit();
                nameLength[i] = packet.ReadBits("nameLen", 6, i);
                guid[i][6] = packet.ReadBit();
                guid[i][4] = packet.ReadBit();
                guid[i][1] = packet.ReadBit();
                guid[i][5] = packet.ReadBit();
                officerLength[i] = packet.ReadBits("officerNoteLen", 8, i);
            }

            var infoLength = packet.ReadBits("infoLen", 11);

            for (var i = 0; i < size; ++i)
            {
                packet.ReadSingle("Last online", i);
                packet.ReadWoWString("Public note", publicLength[i], i);
                packet.ReadXORByte(guid[i], 4);
                packet.ReadByte("Unk Byte", i);
                packet.ReadXORByte(guid[i], 0);
                packet.ReadUInt32("Remaining guild week Rep", i);

                for (var j = 0; j < 2; ++j)
                {
                    var rank = packet.ReadUInt32();
                    var value = packet.ReadUInt32();
                    var id = packet.ReadUInt32();
                    packet.WriteLine("[{0}][{1}] Profession: Id {2} - Value {3} - Rank {4}", i, j, id, value, rank);
                }

                packet.ReadUInt32("Guild Reputation", i);
                packet.ReadWoWString("Name", nameLength[i], i);
                packet.ReadXORByte(guid[i], 6);
                packet.ReadByte("Member Level", i);
                packet.ReadXORByte(guid[i], 5);
                packet.ReadXORByte(guid[i], 1);
                packet.ReadXORByte(guid[i], 3);
                packet.ReadUInt32("Member Achievement Points", i);
                packet.ReadXORByte(guid[i], 7);
                packet.ReadEnum<GuildMemberFlag>("Member Flags", TypeCode.Byte, i);
                packet.ReadUInt32("Member Rank", i);
                packet.ReadEnum<Class>("Member Class", TypeCode.Byte, i);
                packet.ReadXORByte(guid[i], 2);
                packet.ReadUInt64("Total activity", i);
                packet.ReadUInt64("Week activity", i);
                packet.ReadEntryWithName<UInt32>(StoreNameType.Zone, "Zone Id", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadWoWString("Officer note", officerLength[i], i);

                packet.WriteGuid("Guid", guid[i], i);
            }

            packet.ReadWoWString("Guild Info", infoLength);
            packet.ReadWoWString("MOTD", motdLength);
        }

        [Parser(Opcode.SMSG_COMPRESSED_GUILD_ROSTER)]
        public static void HandleCompressedGuildRoster(Packet packet)
        {
            using (var packet2 = packet.Inflate(packet.ReadInt32()))
            {
                HandleGuildRoster(packet2);
            }
        }

        [Parser(Opcode.CMSG_REQUEST_GUILD_PARTY_STATE, ClientVersionBuild.Zero, ClientVersionBuild.V4_3_0_15005)]
        public static void HandleGuildUpdatePartyState(Packet packet)
        {
            packet.ReadGuid("Guild GUID");
            packet.ReadGuid("Player GUID");
        }

        [Parser(Opcode.CMSG_REQUEST_GUILD_PARTY_STATE, ClientVersionBuild.V4_3_0_15005, ClientVersionBuild.V4_3_3_15354)]
        public static void HandleGuildUpdatePartyState430(Packet packet)
        {
            var guid = packet.StartBitStream(6, 5, 2, 4, 1, 3, 7, 0);
            packet.ParseBitStream(guid, 1, 3, 5, 0, 4, 2, 6, 7);
            packet.WriteGuid("Guild Guid", guid);
        }

        [Parser(Opcode.CMSG_REQUEST_GUILD_PARTY_STATE, ClientVersionBuild.V4_3_3_15354, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildUpdatePartyState433(Packet packet)
        {
            var guid = packet.StartBitStream(0, 3, 2, 5, 6, 1, 4, 7);
            packet.ParseBitStream(guid, 2, 5, 3, 0, 1, 4, 7, 6);
            packet.WriteGuid("Guild Guid", guid);
        }

        [Parser(Opcode.CMSG_REQUEST_GUILD_PARTY_STATE, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildUpdatePartyState434(Packet packet)
        {
            var guid = packet.StartBitStream(0, 6, 7, 3, 5, 1, 2, 4);
            packet.ParseBitStream(guid, 6, 3, 2, 1, 5, 0, 7, 4);
            packet.WriteGuid("Guild Guid", guid);
        }

        [Parser(Opcode.SMSG_GUILD_PARTY_STATE_RESPONSE, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildPartyStateResponse434(Packet packet)
        {
            packet.ReadSingle("Guild XP multiplier");
            packet.ReadUInt32("Current guild members");
            packet.ReadUInt32("Needed guild members");
            packet.ReadBit("Is guild group");
        }

        [Parser(Opcode.CMSG_GUILD_QUERY)]
        public static void HandleGuildQuery(Packet packet)
        {
            if (ClientVersion.AddedInVersion(ClientVersionBuild.V4_0_6_13596)) // Not sure when it was changed
            {
                packet.ReadGuid("Guild GUID");
                packet.ReadGuid("Player GUID");
            }
            else
                packet.ReadUInt32("Guild Id");
        }

        [Parser(Opcode.CMSG_GUILD_SET_RANK_PERMISSIONS, ClientVersionBuild.Zero, ClientVersionBuild.V4_0_6_13596)]
        public static void HandleGuildRank(Packet packet)
        {
            packet.ReadUInt32("Rank Id");
            packet.ReadEnum<GuildRankRightsFlag>("Rights", TypeCode.UInt32);
            packet.ReadCString("Name");
            packet.ReadInt32("Money Per Day");
            for (var i = 0; i < 6; i++)
            {
                packet.ReadEnum<GuildBankRightsFlag>("Tab Rights", TypeCode.UInt32, i);
                packet.ReadInt32("Tab Slots", i);
            }
        }

        [Parser(Opcode.CMSG_GUILD_SET_GUILD_MASTER)]
        public static void HandleGuildSetGuildMaster(Packet packet)
        {
            var nameLength = packet.ReadBits(7);
            packet.ReadBit("Is Dethroned"); // Most probably related to guild finder inactivity
            packet.ReadWoWString("New GuildMaster name", nameLength);
        }

        [Parser(Opcode.CMSG_GUILD_SET_RANK_PERMISSIONS, ClientVersionBuild.V4_0_6_13596, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildRank406(Packet packet)
        {
            for (var i = 0; i < 8; ++i)
                packet.ReadUInt32("Bank Slots", i);

            packet.ReadEnum<GuildRankRightsFlag>("Rights", TypeCode.UInt32);

            packet.ReadUInt32("New Rank Id");
            packet.ReadUInt32("Old Rank Id");

            for (var i = 0; i < 8; ++i)
                packet.ReadEnum<GuildBankRightsFlag>("Tab Rights", TypeCode.UInt32, i);

            packet.ReadGuid("Guild GUID");
            packet.ReadEnum<GuildRankRightsFlag>("Old Rights", TypeCode.UInt32);

            packet.ReadInt32("Money Per Day");
            packet.ReadGuid("Player GUID");
            packet.ReadCString("Rank Name");
        }

        [Parser(Opcode.CMSG_GUILD_SET_RANK_PERMISSIONS, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildRank434(Packet packet)
        {
            packet.ReadUInt32("Old Rank Id");
            packet.ReadEnum<GuildRankRightsFlag>("Old Rights", TypeCode.UInt32);
            packet.ReadEnum<GuildRankRightsFlag>("New Rights", TypeCode.UInt32);

            for (var i = 0; i < 8; ++i)
            {
                packet.ReadUInt32("Tab Slot", i);
                packet.ReadEnum<GuildBankRightsFlag>("Tab Rights", TypeCode.UInt32, i);
            }

            packet.ReadUInt32("Money Per Day");
            packet.ReadUInt32("New Rank Id");
            var length = packet.ReadBits(7);
            packet.ReadWoWString("Rank Name", length);
        }

        [Parser(Opcode.SMSG_GUILD_RANKS_UPDATE, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildRanksUpdate(Packet packet)
        {
            var guid1 = new byte[8];
            var guid2 = new byte[8];

            guid1[7] = packet.ReadBit();
            guid1[2] = packet.ReadBit();
            guid2[2] = packet.ReadBit();
            guid1[1] = packet.ReadBit();
            guid2[1] = packet.ReadBit();
            guid2[7] = packet.ReadBit();
            guid2[0] = packet.ReadBit();
            guid2[5] = packet.ReadBit();
            guid2[4] = packet.ReadBit();
            packet.ReadBit("Promote?");
            guid1[5] = packet.ReadBit();
            guid1[0] = packet.ReadBit();
            guid2[6] = packet.ReadBit();
            guid1[3] = packet.ReadBit();
            guid1[6] = packet.ReadBit();
            guid2[3] = packet.ReadBit();
            guid1[4] = packet.ReadBit();

            packet.ReadInt32("Rank Index");

            packet.ReadXORByte(guid1, 3);
            packet.ReadXORByte(guid2, 7);
            packet.ReadXORByte(guid1, 6);
            packet.ReadXORByte(guid1, 2);
            packet.ReadXORByte(guid2, 5);
            packet.ReadXORByte(guid2, 0);
            packet.ReadXORByte(guid1, 7);
            packet.ReadXORByte(guid1, 5);
            packet.ReadXORByte(guid2, 2);
            packet.ReadXORByte(guid2, 1);
            packet.ReadXORByte(guid1, 0);
            packet.ReadXORByte(guid1, 4);
            packet.ReadXORByte(guid1, 1);
            packet.ReadXORByte(guid2, 3);
            packet.ReadXORByte(guid2, 6);
            packet.ReadXORByte(guid2, 4);
            packet.WriteGuid("Guid 1", guid1);
            packet.WriteGuid("Guid 2", guid2);
        }

        [Parser(Opcode.CMSG_GUILD_SWITCH_RANK, ClientVersionBuild.V4_0_6a_13623, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildSwitchRank(Packet packet)
        {
            packet.ReadBit("Direction");
            packet.ReadGuid("Player GUID");
            packet.ReadUInt32("Rank Id");
        }

        [Parser(Opcode.CMSG_GUILD_SWITCH_RANK, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildSwitchRank434(Packet packet)
        {
            packet.ReadUInt32("Rank Id");
            packet.ReadBit("Direction");
        }

        [Parser(Opcode.CMSG_GUILD_DEL_RANK, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildDelRank434(Packet packet)
        {
            packet.ReadUInt32("Rank Id");
        }

        [Parser(Opcode.SMSG_GUILD_RANK, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildRankServer434(Packet packet)
        {
            const int guildBankMaxTabs = 8;
            var count = packet.ReadBits("Count", 17);

            var length = new int[count];

            for (var i = 0; i < count; ++i)
                length[i] = (int)packet.ReadBits(7);

            for (var i = 0; i < count; ++i)
            {
                packet.ReadInt32("Gold Per Day", i);

                for (var j = 0; j < guildBankMaxTabs; ++j)
                {
                    packet.ReadEnum<GuildBankRightsFlag>("Tab Rights", TypeCode.Int32, i, j);
                    packet.ReadInt32("Tab Slots", i, j);
                }

                packet.ReadEnum<GuildRankRightsFlag>("Rights", TypeCode.Int32, i);
                packet.ReadWoWString("Name", length[i], i);
                packet.ReadInt32("Creation Order", i);
                packet.ReadInt32("Rights Order", i);
            }
        }

        [Parser(Opcode.CMSG_GUILD_CREATE)]
        [Parser(Opcode.CMSG_GUILD_INVITE)]
        [Parser(Opcode.CMSG_GUILD_PROMOTE)]
        [Parser(Opcode.CMSG_GUILD_DEMOTE)]
        [Parser(Opcode.CMSG_GUILD_REMOVE, ClientVersionBuild.Zero, ClientVersionBuild.V4_0_6_13596)]
        [Parser(Opcode.CMSG_GUILD_LEADER)]
        [Parser(Opcode.CMSG_GUILD_ADD_RANK, ClientVersionBuild.Zero, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildCreate(Packet packet)
        {
            packet.ReadCString("Name");
        }

        [Parser(Opcode.CMSG_QUERY_GUILD_MEMBERS_FOR_RECIPE)]
        public static void HandleQueryGuildMembersForRecipe(Packet packet)
        {
            var guid = packet.StartBitStream(4, 1, 0, 3, 6, 7, 5, 2);
            packet.ParseBitStream(guid, 1, 6, 5, 0, 3, 7, 2, 4);
            packet.WriteGuid("GUID", guid);
        }

        [Parser(Opcode.SMSG_GUILD_MEMBERS_FOR_RECIPE)]
        public static void HandleGuildMembersForRecipe(Packet packet)
        {
            var count = packet.ReadBits("Count", 26);
            var guid = new byte[count][];

            for (int i = 0; i < count; ++i)
                guid[i] = packet.StartBitStream(2, 3, 1, 6, 0, 7, 4, 5);

            for (int i = 0; i < count; ++i)
            {
                packet.ParseBitStream(guid[i], 1, 5, 6, 7, 2, 3, 0, 4);
                packet.WriteGuid("GUID", guid[i], i);
            }

            packet.ReadEntryWithName<Int32>(StoreNameType.Spell, "Spell ID");
            packet.ReadUInt32("Skill ID");
        }

        [Parser(Opcode.CMSG_QUERY_GUILD_MEMBER_RECIPES)]
        public static void HandleQueryGuildMemberRecipes(Packet packet)
        {
            var guildGuid = new byte[8];
            var guid = new byte[8];

            packet.ReadInt32("Skill ID");

            guid[2] = packet.ReadBit();
            guildGuid[1] = packet.ReadBit();
            guid[1] = packet.ReadBit();
            guildGuid[0] = packet.ReadBit();
            guildGuid[6] = packet.ReadBit();
            guid[7] = packet.ReadBit();
            guildGuid[4] = packet.ReadBit();
            guildGuid[3] = packet.ReadBit();
            guildGuid[7] = packet.ReadBit();
            guid[5] = packet.ReadBit();
            guid[0] = packet.ReadBit();
            guildGuid[5] = packet.ReadBit();
            guid[3] = packet.ReadBit();
            guid[6] = packet.ReadBit();
            guildGuid[2] = packet.ReadBit();
            guid[4] = packet.ReadBit();

            packet.ReadXORByte(guid, 2);
            packet.ReadXORByte(guid, 6);
            packet.ReadXORByte(guildGuid, 4);
            packet.ReadXORByte(guildGuid, 2);
            packet.ReadXORByte(guid, 1);
            packet.ReadXORByte(guildGuid, 7);
            packet.ReadXORByte(guildGuid, 3);
            packet.ReadXORByte(guildGuid, 1);
            packet.ReadXORByte(guid, 3);
            packet.ReadXORByte(guid, 0);
            packet.ReadXORByte(guildGuid, 0);
            packet.ReadXORByte(guid, 7);
            packet.ReadXORByte(guid, 4);
            packet.ReadXORByte(guildGuid, 5);
            packet.ReadXORByte(guildGuid, 6);
            packet.ReadXORByte(guid, 5);

            packet.WriteGuid("Guild GUID", guildGuid);
            packet.WriteGuid("Player GUID", guid);
        }

        [Parser(Opcode.SMSG_GUILD_MEMBER_RECIPES)]
        public static void HandleGuildMemberRecipes(Packet packet)
        {
            var guid = packet.StartBitStream(0, 3, 7, 4, 6, 2, 1, 5);

            packet.ReadXORByte(guid, 0);
            packet.ReadXORByte(guid, 1);
            packet.ReadXORByte(guid, 7);
            packet.ReadXORByte(guid, 4);
            packet.ReadXORByte(guid, 5);

            packet.ReadInt32("Skill value");
            // skip learned recipes masks
            packet.ReadBytes(300);

            packet.ReadXORByte(guid, 3);
            packet.ReadXORByte(guid, 6);
            packet.ReadXORByte(guid, 2);

            packet.ReadInt32("Skill ID");
            packet.ReadInt32("Unk Int32"); // ##

            packet.WriteGuid("GUID", guid);
        }

        [Parser(Opcode.CMSG_GUILD_REMOVE, ClientVersionBuild.V4_0_6_13596, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildRemove406(Packet packet)
        {
            packet.ReadGuid("Target GUID");
            packet.ReadGuid("Removee GUID");
        }

        [Parser(Opcode.CMSG_GUILD_REMOVE, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildRemove434(Packet packet)
        {
            var guid = packet.StartBitStream(6, 5, 4, 0, 1, 3, 7, 2);
            packet.ParseBitStream(guid, 2, 6, 5, 7, 1, 4, 3, 0);
            packet.WriteGuid("GUID", guid);
        }

        [Parser(Opcode.SMSG_GUILD_INVITE, ClientVersionBuild.Zero, ClientVersionBuild.V4_0_6_13596)]
        public static void HandleGuildInvite(Packet packet)
        {
            packet.ReadCString("Invitee Name");
            packet.ReadCString("Guild Name");
        }

        [Parser(Opcode.SMSG_GUILD_INVITE, ClientVersionBuild.V4_0_6_13596, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildInvite406(Packet packet)
        {
            packet.ReadUInt32("Emblem Background Color");
            packet.ReadUInt32("Emblem Border Style");
            packet.ReadUInt32("Emblem Color");
            packet.ReadUInt32("Emblem Texture File");
            packet.ReadGuid("Old Guild GUID");
            packet.ReadCString("Old Guild Name");
            packet.ReadCString("Inviter Name");
            packet.ReadUInt32("Guild Level");
            packet.ReadUInt32("Emblem Border Color");
            packet.ReadGuid("Guild GUID");
            packet.ReadCString("Guild Name");
        }

        [Parser(Opcode.SMSG_GUILD_INVITE, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildInvite434(Packet packet)
        {
            var newGuildGuid = new byte[8];
            var oldGuildGuid = new byte[8];

            packet.ReadInt32("Guild Level");
            packet.ReadInt32("Border Style");
            packet.ReadInt32("Border Color");
            packet.ReadInt32("Emblem Texture");
            packet.ReadInt32("Emblem Background Color");
            packet.ReadInt32("Emblem Color");

            newGuildGuid[3] = packet.ReadBit();
            newGuildGuid[2] = packet.ReadBit();

            var oldGuildNameLength = packet.ReadBits(8);

            newGuildGuid[1] = packet.ReadBit();
            oldGuildGuid[6] = packet.ReadBit();
            oldGuildGuid[4] = packet.ReadBit();
            oldGuildGuid[1] = packet.ReadBit();
            oldGuildGuid[5] = packet.ReadBit();
            oldGuildGuid[7] = packet.ReadBit();
            oldGuildGuid[2] = packet.ReadBit();
            newGuildGuid[7] = packet.ReadBit();
            newGuildGuid[0] = packet.ReadBit();
            newGuildGuid[6] = packet.ReadBit();

            var newGuildNameLength = packet.ReadBits(8);

            oldGuildGuid[3] = packet.ReadBit();
            oldGuildGuid[0] = packet.ReadBit();
            newGuildGuid[5] = packet.ReadBit();

            var inviterNameLength = packet.ReadBits(7);

            newGuildGuid[4] = packet.ReadBit();

            packet.ReadXORByte(newGuildGuid, 1);
            packet.ReadXORByte(oldGuildGuid, 3);
            packet.ReadXORByte(newGuildGuid, 6);
            packet.ReadXORByte(oldGuildGuid, 2);
            packet.ReadXORByte(oldGuildGuid, 1);
            packet.ReadXORByte(newGuildGuid, 0);

            packet.ReadWoWString("Old Guild Name", oldGuildNameLength);

            packet.ReadXORByte(newGuildGuid, 7);
            packet.ReadXORByte(newGuildGuid, 2);

            packet.ReadWoWString("Inviter Name", inviterNameLength);

            packet.ReadXORByte(oldGuildGuid, 7);
            packet.ReadXORByte(oldGuildGuid, 6);
            packet.ReadXORByte(oldGuildGuid, 5);
            packet.ReadXORByte(oldGuildGuid, 0);
            packet.ReadXORByte(newGuildGuid, 4);

            packet.ReadWoWString("New Guild Name", newGuildNameLength);

            packet.ReadXORByte(newGuildGuid, 5);
            packet.ReadXORByte(newGuildGuid, 3);
            packet.ReadXORByte(oldGuildGuid, 4);

            packet.WriteGuid("New Guild Guid", newGuildGuid);
            packet.WriteGuid("Old Guild Guid", oldGuildGuid);
        }

        [Parser(Opcode.SMSG_GUILD_INFO)]
        public static void HandleGuildInfo(Packet packet)
        {
            packet.ReadCString("Name");
            packet.ReadPackedTime("Creation Date");
            packet.ReadUInt32("Number of Players");
            packet.ReadUInt32("Number of Accounts");
        }

        [Parser(Opcode.CMSG_GUILD_MOTD, ClientVersionBuild.Zero, ClientVersionBuild.V4_0_6_13596)]
        public static void HandleGuildMOTD(Packet packet)
        {
            packet.ReadCString("MOTD");
        }

        [Parser(Opcode.CMSG_GUILD_MOTD, ClientVersionBuild.V4_0_6_13596, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildMOTD406(Packet packet)
        {
            packet.ReadGuid("Guild GUID");
            packet.ReadGuid("Player GUID");
            packet.ReadCString("MOTD");
        }

        [Parser(Opcode.CMSG_GUILD_MOTD, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildMOTD434(Packet packet)
        {
            packet.ReadWoWString("MOTD", (int)packet.ReadBits(11));
        }

        [Parser(Opcode.CMSG_GUILD_INFO_TEXT, ClientVersionBuild.Zero, ClientVersionBuild.V4_0_6_13596)]
        public static void HandleSetGuildInfo(Packet packet)
        {
            packet.ReadCString("Text");
        }

        [Parser(Opcode.CMSG_GUILD_INFO_TEXT, ClientVersionBuild.V4_0_6_13596, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildInfo406(Packet packet)
        {
            packet.ReadGuid("Player GUID");
            packet.ReadGuid("Guild GUID");
            packet.ReadCString("Text");
        }

        [Parser(Opcode.CMSG_GUILD_INFO_TEXT, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildInfo434(Packet packet)
        {
            packet.ReadWoWString("Text", (int)packet.ReadBits(12));
        }

        [Parser(Opcode.CMSG_GUILD_CHANGE_NAME_REQUEST, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildNameChange(Packet packet)
        {
            packet.ReadWoWString("New Name", (int)packet.ReadBits(8));
        }

        [Parser(Opcode.SMSG_GUILD_EVENT)]
        public static void HandleGuildEvent(Packet packet)
        {
            if (ClientVersion.AddedInVersion(ClientVersionBuild.V4_0_6a_13623))
                packet.ReadEnum<GuildEventType442>("Event Type", TypeCode.Byte);
            else
                packet.ReadEnum<GuildEventType>("Event Type", TypeCode.Byte);

            var size = packet.ReadByte("Param Count");
            for (var i = 0; i < size; i++)
                packet.ReadCString("Param", i);

            if (packet.CanRead()) // FIXME 4 5 6 16 17 (GuildEventType changed for 4.2.2)
                packet.ReadGuid("GUID");
        }

        [Parser(Opcode.SMSG_GUILD_COMMAND_RESULT)]
        [Parser(Opcode.SMSG_GUILD_COMMAND_RESULT_2, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildCommandResult(Packet packet)
        {
            packet.ReadEnum<GuildCommandType>("Command Type", TypeCode.UInt32);
            packet.ReadCString("Param");
            packet.ReadEnum<GuildCommandError>("Command Result", TypeCode.UInt32);
        }

        [Parser(Opcode.MSG_SAVE_GUILD_EMBLEM)]
        public static void HandleGuildEmblem(Packet packet)
        {
            if (packet.Direction == Direction.ClientToServer)
            {
                packet.ReadGuid("GUID");
                ReadEmblemInfo(ref packet);
            }
            else
                packet.ReadEnum<GuildEmblemError>("Result", TypeCode.UInt32);
        }

        [Parser(Opcode.CMSG_GUILD_SET_PUBLIC_NOTE)]
        public static void HandleGuildSetPublicNote(Packet packet)
        {
            packet.ReadCString("Player Name");
            packet.ReadCString("Public Note");
        }

        [Parser(Opcode.CMSG_GUILD_SET_OFFICER_NOTE)]
        public static void HandleGuildSetOfficerNote(Packet packet)
        {
            packet.ReadCString("Player Name");
            packet.ReadCString("Officer Note");
        }

        [Parser(Opcode.CMSG_GUILD_SET_NOTE, ClientVersionBuild.Zero, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildSetNote(Packet packet)
        {
            packet.ReadBit("Public");
            packet.ReadGuid("Target GUID");
            packet.ReadGuid("Issuer GUID");
            packet.ReadGuid("Guild GUID");
            packet.ReadCString("Note");
        }

        [Parser(Opcode.CMSG_GUILD_SET_NOTE, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildSetNote434(Packet packet)
        {
            var guid = new byte[8];
            guid[1] = packet.ReadBit();
            guid[4] = packet.ReadBit();
            guid[5] = packet.ReadBit();
            guid[3] = packet.ReadBit();
            guid[0] = packet.ReadBit();
            guid[7] = packet.ReadBit();
            packet.ReadBit("Public");
            guid[6] = packet.ReadBit();
            var len = packet.ReadBits("Note Length", 8);
            guid[2] = packet.ReadBit();

            packet.ReadXORByte(guid, 4);
            packet.ReadXORByte(guid, 5);
            packet.ReadXORByte(guid, 0);
            packet.ReadXORByte(guid, 3);
            packet.ReadXORByte(guid, 1);
            packet.ReadXORByte(guid, 6);
            packet.ReadXORByte(guid, 7);
            packet.ReadWoWString("Note", len);
            packet.ReadXORByte(guid, 2);
            packet.WriteGuid("Guid", guid);
        }

        [Parser(Opcode.CMSG_GUILD_BANKER_ACTIVATE)]
        public static void HandleGuildBankerActivate(Packet packet)
        {
            packet.ReadGuid("GUID");
            packet.ReadBoolean("Full Slot List");
        }

        [Parser(Opcode.CMSG_GUILD_BANK_QUERY_TAB)]
        public static void HandleGuildBankQueryTab(Packet packet)
        {
            packet.ReadGuid("GUID");
            if (ClientVersion.RemovedInVersion(ClientVersionBuild.V4_2_2_14545)
                || ClientVersion.AddedInVersion(ClientVersionBuild.V4_3_4_15595))
                packet.ReadByte("Tab Id");
            packet.ReadBoolean("Full Slot List"); // false = only slots updated in last operation are shown. True = all slots updated
        }

        [Parser(Opcode.SMSG_GUILD_BANK_LIST)]
        public static void HandleGuildBankList(Packet packet)
        {
            var itemCount = packet.ReadBits("itemCount", 18);
            var tabSize = packet.ReadBits("tabSize", 21);

            var icons = new uint[tabSize];
            var texts = new uint[tabSize];
            var unk = new bool[tabSize];

            var itemEnchants = new uint[itemCount];
            var unkBits = new bool[itemCount];
            for (int i = 0; i < itemCount; i++)
            {
                itemEnchants[i] = packet.ReadBits("itemEnchant", 21, i);
                unkBits[i] = packet.ReadBit("unkBit", i);
            }

            packet.ReadBit("Unk");

            for (int i = 0; i < tabSize; i++)
            {
                texts[i] = packet.ReadBits("textLen", 7, i);
                icons[i] = packet.ReadBits("iconLen", 8, i);
                icons[i] *= 2;
                unk[i] = packet.ReadBit("unkBit", i);

                if (unk[i])
                    icons[i]++;
            }

            for (int i = 0; i < tabSize; i++)
            {
                packet.ReadWoWString("IconName", icons[i], i);
                packet.ReadWoWString("Text", texts[i], i);
                packet.ReadUInt32("Slot");
            }

            for (int i = 0; i < itemCount; i++)
            {
                packet.ReadUInt32("Unk UInt32", i); // 0 ?
                packet.ReadUInt32("Slot ID", i);
                packet.ReadUInt32("Spell Charges ?", i);
                packet.ReadUInt32("Unk UInt32", i); // 196640, 196608, 229376 ??
                packet.ReadUInt32("Random Suffix", i);
                packet.ReadUInt32("Item ID", i);

                var bytesCounter = packet.ReadUInt32("BytesCounter", i); // Send 0
                for (int j = 0; j < bytesCounter; j++)
                    packet.ReadByte("Unk byte", i, j);

                for (int j = 0; j < itemEnchants[i]; j++)
                {
                    packet.ReadUInt32("Unk UInt32", i, j);
                    packet.ReadUInt32("Unk UInt32", i, j);
                }

                packet.ReadUInt32("Random Property", i);
                packet.ReadUInt32("Unk UInt32", i); // 0 ?
                packet.ReadUInt32("Item Count", i);
            }

            packet.ReadUInt32("Tab ID");
            packet.ReadUInt64("Gold");
            packet.ReadUInt32("Remaining Slots");
        }

        [Parser(Opcode.CMSG_GUILD_BANK_SWAP_ITEMS)]
        public static void HandleGuildBankSwapItems(Packet packet)
        {
            packet.ReadUInt32("Unk Uint32");
            packet.ReadByte("Unk Byte");
            packet.ReadByte("Unk Byte");
            packet.ReadUInt32("Item ID");
            packet.ReadByte("Unk Byte");

            var gobGuid = new byte[8];

            var bit52 = packet.ReadBit("unkBit52");
            gobGuid[3] = packet.ReadBit();
            var bit20 = !packet.ReadBit();
            Console.WriteLine("bit20: " + bit20);
            var bit32 = packet.ReadBit("unkBit32");
            var bit21 = !packet.ReadBit();
            Console.WriteLine("bit21: " + bit21);
            gobGuid[1] = packet.ReadBit();
            var bit51 = !packet.ReadBit();
            Console.WriteLine("bit51: " + bit51);
            gobGuid[4] = packet.ReadBit();
            var bit24 = !packet.ReadBit();
            Console.WriteLine("bit24: " + bit24);
            gobGuid[0] = packet.ReadBit();
            var bit60 = !packet.ReadBit();
            Console.WriteLine("bit60: " + bit60);
            gobGuid[2] = packet.ReadBit();
            gobGuid[7] = packet.ReadBit();
            var bit16 = !packet.ReadBit();
            Console.WriteLine("bit16: " + bit16);
            gobGuid[6] = packet.ReadBit();
            gobGuid[5] = packet.ReadBit();

            packet.ResetBitReader();

            packet.ParseBitStream(gobGuid, 7, 3, 1, 6, 5, 4, 2, 0);

            if (bit24)
                packet.ReadUInt32("Unk bit24 UInt32");

            if (bit51)
                packet.ReadByte("Unk bit51 Byte");

            if (bit16)
                packet.ReadUInt32("Unk bit16 UInt32");

            if (bit20)
                packet.ReadByte("Unk bit20 Byte");

            if (bit60)
                packet.ReadByte("Unk bit60 Byte");

            if (bit21)
                packet.ReadByte("Unk bit21 Byte");

            packet.WriteGuid("Gob GUID", gobGuid);
        }

        [Parser(Opcode.CMSG_GUILD_BANK_BUY_TAB)]
        public static void HandleGuildBankBuyTab(Packet packet)
        {
            packet.ReadGuid("GUID");
            packet.ReadByte("Tab Id");
        }

        [Parser(Opcode.CMSG_GUILD_BANK_UPDATE_TAB)]
        public static void HandleGuildBankUpdateTab(Packet packet)
        {
            packet.ReadGuid("GUID");
            packet.ReadByte("Tab Id");
            packet.ReadCString("Tab Name");
            packet.ReadCString("Tab Icon");
        }

        [Parser(Opcode.CMSG_GUILD_QUERY_RANKS, ClientVersionBuild.Zero, ClientVersionBuild.V4_3_0_15005)]
        [Parser(Opcode.CMSG_GUILD_QUERY_NEWS, ClientVersionBuild.Zero, ClientVersionBuild.V4_3_4_15595)]
        [Parser(Opcode.CMSG_GUILD_REQUEST_MAX_DAILY_XP, ClientVersionBuild.Zero, ClientVersionBuild.V4_3_0_15005)]
        [Parser(Opcode.CMSG_QUERY_GUILD_XP, ClientVersionBuild.Zero, ClientVersionBuild.V4_3_0_15005)]
        [Parser(Opcode.CMSG_GUILD_QUERY_TRADESKILL)]
        public static void HandleGuildRequestMulti(Packet packet)
        {
            packet.ReadGuid("GUID");
        }

        [Parser(Opcode.CMSG_GUILD_REQUEST_MAX_DAILY_XP, ClientVersionBuild.V4_3_0_15005, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildRequestMaxDailyXP430(Packet packet)
        {
            var guid = packet.StartBitStream(6, 5, 2, 3, 0, 4, 7, 1);
            packet.ParseBitStream(guid, 1, 6, 4, 5, 3, 7, 0, 2);
            packet.WriteGuid("GUID", guid);
        }

        [Parser(Opcode.CMSG_QUERY_GUILD_XP, ClientVersionBuild.V4_3_0_15005, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildQueryGuildXP430(Packet packet)
        {
            var guid = packet.StartBitStream(2, 4, 5, 6, 1, 0, 3, 7);
            packet.ParseBitStream(guid, 0, 1, 4, 3, 2, 6, 7, 5);
            packet.WriteGuid("GUID", guid);
        }

        [Parser(Opcode.CMSG_GUILD_REQUEST_MAX_DAILY_XP, ClientVersionBuild.V4_3_4_15595, ClientVersionBuild.V5_1_0_16309)]
        public static void HandleGuildRequestMaxDailyXP434(Packet packet)
        {
            var guid = packet.StartBitStream(0, 3, 5, 1, 4, 6, 7, 2);
            packet.ParseBitStream(guid, 7, 4, 3, 5, 1, 2, 6, 0);
            packet.WriteGuid("GUID", guid);
        }

        [Parser(Opcode.CMSG_GUILD_REQUEST_MAX_DAILY_XP, ClientVersionBuild.V5_1_0_16309)]
        public static void HandleGuildRequestMaxDailyXP510(Packet packet)
        {
            var guid = packet.StartBitStream(5, 3, 6, 4, 7, 2, 1, 0);
            packet.ParseBitStream(guid, 4, 7, 5, 6, 0, 1, 2, 3);
            packet.WriteGuid("GUID", guid);
        }

        [Parser(Opcode.CMSG_QUERY_GUILD_XP, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildQueryGuildXP434(Packet packet)
        {
            var guid = packet.StartBitStream(2, 1, 0, 5, 4, 7, 6, 3);
            packet.ParseBitStream(guid, 7, 2, 3, 6, 1, 5, 0, 4);
            packet.WriteGuid("GUID", guid);
        }

        [Parser(Opcode.CMSG_GUILD_QUERY_NEWS, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildQueryNews434(Packet packet)
        {
            var guid = packet.StartBitStream(4, 2, 6, 3, 5, 0, 1, 7);
            packet.ParseBitStream(guid, 4, 1, 5, 6, 0, 3, 7, 2);
            packet.WriteGuid("GUID", guid);
        }

        [Parser(Opcode.CMSG_GUILD_QUERY_RANKS, ClientVersionBuild.V4_3_0_15005, ClientVersionBuild.V4_3_3_15354)]
        public static void HandleGuildRanks43(Packet packet)
        {
            var guid = packet.StartBitStream(7, 2, 0, 4, 6, 5, 1, 3);
            packet.ParseBitStream(guid, 7, 5, 2, 6, 1, 4, 0, 3);
            packet.WriteGuid("Guid", guid);
        }

        [Parser(Opcode.CMSG_GUILD_QUERY_RANKS, ClientVersionBuild.V4_3_3_15354, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildRanks433(Packet packet)
        {
            var guid = packet.StartBitStream(2, 6, 1, 0, 5, 3, 7, 4);
            packet.ParseBitStream(guid, 3, 6, 5, 4, 0, 7, 2, 1);
            packet.WriteGuid("Guid", guid);
        }

        [Parser(Opcode.CMSG_GUILD_QUERY_RANKS, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildRanks434(Packet packet)
        {
            var guid = packet.StartBitStream(2, 3, 0, 6, 4, 7, 5, 1);
            packet.ParseBitStream(guid, 3, 4, 5, 7, 1, 0, 6, 2);
            packet.WriteGuid("Guid", guid);
        }

        [Parser(Opcode.SMSG_GUILD_XP)]
        public static void HandleGuildXP(Packet packet)
        {
            packet.ReadUInt64("Unk 64");
            packet.ReadUInt64("Unk 64");
            packet.ReadUInt64("Unk 64");
            packet.ReadUInt64("Unk 64");
        }

        [Parser(Opcode.SMSG_GUILD_NEWS_UPDATE)]
        public static void HandleGuildNewsUpdate(Packet packet)
        {
            var size = packet.ReadBits("Size", 19);

            var guidOut = new byte[size][];
            var guidIn = new byte[size][][];
            var count = new uint[size];

            for (int i = 0; i < size; ++i)
            {
                count[i] = packet.ReadBits("Count", 24, i);

                guidIn[i] = new byte[count[i]][];
                for (int j = 0; j < count[i]; ++j)
                    guidIn[i][j] = packet.StartBitStream(5, 2, 6, 0, 1, 4, 3, 7);

                guidOut[i] = new byte[8];

                guidOut[i] = packet.StartBitStream(2, 0, 4, 1, 7, 3, 5, 6);
            }

            for (int i = 0; i < size; ++i)
            {
                packet.ReadXORByte(guidOut[i], 2);

                for (int j = 0; i < count[i]; ++j)
                {
                    packet.ParseBitStream(guidIn[i][j], 4, 0, 1, 5, 3, 7, 2, 6);
                    packet.WriteGuid("Guid", guidIn[i][j], i, j);
                }

                packet.ReadEnum<GuildNewsType>("News Type", TypeCode.Int32, i);
                packet.ReadPackedTime("Time", i);
                packet.ReadXORByte(guidOut[i], 1);
                packet.ReadXORByte(guidOut[i], 4);
                packet.ReadXORByte(guidOut[i], 6);
                packet.ReadUInt32("Flags", i);
                packet.ReadXORByte(guidOut[i], 3);
                packet.ReadXORByte(guidOut[i], 7);
                packet.ReadUInt32("Entry (item/achiev/encounter)", i);
                packet.ReadXORByte(guidOut[i], 5);
                packet.ReadInt32("News Id", i);
                packet.ReadUInt32("Always 0", i);
                packet.ReadXORByte(guidOut[i], 0);

                packet.WriteGuid("Guid", guidOut[i], i);
            }
        }

        [Parser(Opcode.CMSG_QUERY_GUILD_REWARDS)]
        public static void HandleGuildQueryRewards(Packet packet)
        {
            packet.ReadTime("Unk Time");
            if (ClientVersion.RemovedInVersion(ClientVersionBuild.V4_3_4_15595))
                packet.ReadGuid("Player GUID");
        }

        [Parser(Opcode.SMSG_GUILD_REWARDS_LIST)]
        public static void HandleGuildRewardsList(Packet packet)
        {
            packet.ReadPackedTime("Date");
            var counter = packet.ReadBits("counter", 19);

            var achievements = new uint[counter];
            for (int i = 0; i < counter; i++)
                achievements[i] = packet.ReadBits("achievements", 22, i);

            for (int i = 0; i < counter; i++)
            {
                packet.ReadUInt32("Item Id", i);
                packet.ReadUInt64("Price", i);
                packet.ReadEnum<RaceMask>("Race mask", TypeCode.UInt32, i);

                for (var j = 0; j < achievements[i]; j++)
                    packet.ReadUInt32("Achievement Id", i);

                packet.ReadEnum<ReputationRank>("Faction Standing", TypeCode.UInt32, i);
                packet.ReadUInt32("Unk UInt32", i); // 0 ?
            }
        }

        [Parser(Opcode.CMSG_GUILD_BANK_DEPOSIT_MONEY)]
        [Parser(Opcode.CMSG_GUILD_BANK_WITHDRAW_MONEY)]
        public static void HandleGuildBankDepositMoney(Packet packet)
        {
            packet.ReadGuid("GUID");
            if (ClientVersion.AddedInVersion(ClientVersionBuild.V4_0_6a_13623))
                packet.ReadUInt64("Money");
            else
                packet.ReadUInt32("Money");
        }

        [Parser(Opcode.CMSG_GUILD_BANK_QUERY_TEXT, ClientVersionBuild.V4_3_3_15354)]
        public static void HandleQueryGuildBankText(Packet packet)
        {
            packet.ReadUInt32("Tab Id");
        }

        [Parser(Opcode.MSG_QUERY_GUILD_BANK_TEXT, ClientVersionBuild.Zero, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildQueryBankText(Packet packet)
        {
            packet.ReadByte("Tab Id");
            if (packet.Direction == Direction.ServerToClient)
                packet.ReadCString("Text");
        }

        [Parser(Opcode.SMSG_GUILD_BANK_QUERY_TEXT_RESULTS, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildQueryBankText434(Packet packet)
        {
            packet.ReadUInt32("Tab Id");
            packet.ReadWoWString("Text", packet.ReadBits(14));
        }

        [Parser(Opcode.CMSG_SET_GUILD_BANK_TEXT, ClientVersionBuild.Zero, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildSetBankText(Packet packet)
        {
            packet.ReadByte("Tab Id");
            packet.ReadCString("Tab Text");
        }

        [Parser(Opcode.CMSG_SET_GUILD_BANK_TEXT, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildSetBankText434(Packet packet)
        {
            packet.ReadUInt32("Tab Id");
            packet.ReadWoWString("Tab Text", packet.ReadBits(14));
        }

        [Parser(Opcode.MSG_GUILD_PERMISSIONS)]
        public static void HandleGuildPermissions(Packet packet)
        {
            if (packet.Direction == Direction.ClientToServer)
                return;

            packet.ReadUInt32("Rank Id");
            packet.ReadEnum<GuildRankRightsFlag>("Rights", TypeCode.UInt32);
            packet.ReadInt32("Remaining Money");
            packet.ReadByte("Tab size");
            var tabSize = ClientVersion.AddedInVersion(ClientType.Cataclysm) ? 8 : 6;
            for (var i = 0; i < tabSize; i++)
            {
                packet.ReadEnum<GuildBankRightsFlag>("Tab Rights", TypeCode.Int32, i);
                packet.ReadInt32("Tab Slots", i);
            }
        }

        [Parser(Opcode.SMSG_GUILD_PERMISSIONS_QUERY_RESULTS)]
        public static void HandleGuildPermissionsQueryResult(Packet packet)
        {
            packet.ReadInt32("Purchased Tab size");
            packet.ReadEnum<GuildRankRightsFlag>("Rights", TypeCode.UInt32);
            packet.ReadUInt32("Rank Id");
            packet.ReadInt32("Remaining Money");
            packet.ReadBits("Tab size", 21);
            for (var i = 0; i < 8; i++)
            {
                packet.ReadInt32("Tab Slots", i);
                packet.ReadEnum<GuildBankRightsFlag>("Tab Rights", TypeCode.Int32, i);
            }
        }

        [Parser(Opcode.CMSG_GUILD_BANK_MONEY_WITHDRAWN_QUERY)]
        [Parser(Opcode.MSG_GUILD_BANK_MONEY_WITHDRAWN)]
        public static void HandleGuildBankMoneyWithdrawn(Packet packet)
        {
            if (packet.Direction == Direction.ServerToClient)
            {
                if (ClientVersion.AddedInVersion(ClientVersionBuild.V4_0_6a_13623))
                    packet.ReadInt64("Remaining Money");
                else
                    packet.ReadInt32("Remaining Money");
            }
        }

        [Parser(Opcode.SMSG_GUILD_BANK_MONEY_WITHDRAWN)]
        public static void HandleGuildBankMoneyWithdrawnResponse(Packet packet)
        {
            packet.ReadInt64("Remaining Money");
        }

        [Parser(Opcode.MSG_GUILD_EVENT_LOG_QUERY)]
        public static void HandleGuildEventLogQuery(Packet packet)
        {
            if (packet.Direction == Direction.ClientToServer)
                return;

            var size = packet.ReadByte("Log size");
            for (var i = 0; i < size; i++)
            {
                var type = packet.ReadEnum<GuildEventLogType>("Type", TypeCode.Byte);
                packet.ReadGuid("GUID");
                if (type != GuildEventLogType.JoinGuild && type != GuildEventLogType.LeaveGuild)
                    packet.ReadGuid("GUID 2");
                if (type == GuildEventLogType.PromotePlayer || type == GuildEventLogType.DemotePlayer)
                    packet.ReadByte("Rank");
                packet.ReadUInt32("Time Ago");
            }
        }

        [Parser(Opcode.SMSG_GUILD_EVENT_LOG_QUERY)]
        public static void HandleGuildEventLogQuery434(Packet packet)
        {
            var count = packet.ReadBits(23);
            var guid1 = new byte[count][];
            var guid2 = new byte[count][];

            for (var i = 0; i < count; ++i)
            {
                guid1[i] = new byte[8];
                guid2[i] = new byte[8];

                guid1[i][2] = packet.ReadBit();
                guid1[i][4] = packet.ReadBit();
                guid2[i][7] = packet.ReadBit();
                guid2[i][6] = packet.ReadBit();
                guid1[i][3] = packet.ReadBit();
                guid2[i][3] = packet.ReadBit();
                guid2[i][5] = packet.ReadBit();
                guid1[i][7] = packet.ReadBit();
                guid1[i][5] = packet.ReadBit();
                guid1[i][0] = packet.ReadBit();
                guid2[i][4] = packet.ReadBit();
                guid2[i][2] = packet.ReadBit();
                guid2[i][0] = packet.ReadBit();
                guid2[i][1] = packet.ReadBit();
                guid1[i][1] = packet.ReadBit();
                guid1[i][6] = packet.ReadBit();
            }

            for (var i = 0; i < count; ++i)
            {
                packet.ReadXORByte(guid2[i], 3);
                packet.ReadXORByte(guid2[i], 2);
                packet.ReadXORByte(guid2[i], 5);

                packet.ReadByte("Rank", i);

                packet.ReadXORByte(guid2[i], 4);
                packet.ReadXORByte(guid1[i], 0);
                packet.ReadXORByte(guid1[i], 4);

                packet.ReadUInt32("Time ago", i);

                packet.ReadXORByte(guid1[i], 7);
                packet.ReadXORByte(guid1[i], 3);
                packet.ReadXORByte(guid2[i], 0);
                packet.ReadXORByte(guid2[i], 6);
                packet.ReadXORByte(guid2[i], 7);
                packet.ReadXORByte(guid1[i], 5);

                packet.ReadEnum<GuildEventLogType>("Type", TypeCode.Byte, i);

                packet.ReadXORByte(guid2[i], 1);
                packet.ReadXORByte(guid1[i], 2);
                packet.ReadXORByte(guid1[i], 6);
                packet.ReadXORByte(guid1[i], 1);

                packet.WriteLine("[{0}] GUID1: {1}", i, new Guid(BitConverter.ToUInt64(guid1[i], 0)));
                packet.WriteLine("[{0}] GUID2: {1}", i, new Guid(BitConverter.ToUInt64(guid2[i], 0)));
            }
        }

        [Parser(Opcode.SMSG_UNK_GUILD)]
        public static void HandleUnkGuildReceived(Packet packet)
        {
            var guid = new byte[8];

            var bit72 = packet.ReadBit("Unk bit 72");
            var length = packet.ReadBits("nameLength", 6);
            var bit16 = packet.ReadBit("Online state");

            packet.StartBitStream(guid, 0, 2, 6, 7, 3, 4, 5, 1);

            packet.ReadXORByte(guid, 6);
            packet.ReadXORByte(guid, 4);
            packet.ReadWoWString("Player name", length);
            packet.ReadXORByte(guid, 5);
            packet.ReadXORByte(guid, 0);
            packet.ReadXORByte(guid, 7);
            packet.ReadXORByte(guid, 1);
            packet.ReadUInt32("Realm ID");
            packet.ReadXORByte(guid, 2);
            packet.ReadXORByte(guid, 3);

            packet.WriteGuid("Unk GUID", guid);
        }

        [Parser(Opcode.MSG_GUILD_BANK_LOG_QUERY, ClientVersionBuild.Zero, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildBankLogQueryResult(Packet packet)
        {
            packet.ReadByte("Tab Id");
            if (packet.Direction == Direction.ServerToClient)
            {
                var size = packet.ReadByte("Size");
                for (var i = 0; i < size; i++)
                {
                    var type = packet.ReadEnum<GuildBankEventLogType>("Bank Log Event Type", TypeCode.Byte, i);
                    packet.ReadGuid("GUID", i);
                    if (type == GuildBankEventLogType.BuySlot)
                        packet.ReadUInt32("Cost", i);
                    else
                    {
                        if (type == GuildBankEventLogType.DepositMoney ||
                            type == GuildBankEventLogType.WithdrawMoney ||
                            type == GuildBankEventLogType.RepairMoney)
                            packet.ReadUInt32("Money", i);
                        else
                        {
                            packet.ReadUInt32("Item Entry", i);
                            packet.ReadUInt32("Stack Count", i);
                            if (type == GuildBankEventLogType.MoveItem ||
                                type == GuildBankEventLogType.MoveItem2)
                                packet.ReadByte("Tab Id", i);
                        }
                    }
                    packet.ReadUInt32("Time", i);
                }
            }
        }

        [Parser(Opcode.SMSG_GUILD_BANK_LOG_QUERY_RESULTS, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildBankLogQueryResult434(Packet packet)
        {
            var hasWeekCashflow = packet.ReadBit("Has Cash flow Perk");
            var size = packet.ReadBits("Size", 23);
            var hasMoney = new byte[size];
            var tabChanged = new byte[size];
            var stackCount = new byte[size];
            var hasItem = new byte[size];
            var guid = new byte[size][];
            for (var i = 0; i < size; i++)
            {
                guid[i] = new byte[8];
                hasMoney[i] = packet.ReadBit();
                guid[i][4] = packet.ReadBit();
                guid[i][1] = packet.ReadBit();
                hasItem[i] = packet.ReadBit();
                stackCount[i] = packet.ReadBit();
                guid[i][2] = packet.ReadBit();
                guid[i][5] = packet.ReadBit();
                guid[i][3] = packet.ReadBit();
                guid[i][6] = packet.ReadBit();
                guid[i][0] = packet.ReadBit();
                tabChanged[i] = packet.ReadBit(); //unk
                guid[i][7] = packet.ReadBit();
            }
            for (var i = 0; i < size; i++)
            {
                packet.ReadXORByte(guid[i], 6);
                packet.ReadXORByte(guid[i], 1);
                packet.ReadXORByte(guid[i], 5);
                if (stackCount[i] != 0)
                    packet.ReadUInt32("Stack count", i);
                packet.ReadEnum<GuildBankEventLogType>("Bank Log Event Type", TypeCode.Byte, i);
                packet.ReadXORByte(guid[i], 2);
                packet.ReadXORByte(guid[i], 4);
                packet.ReadXORByte(guid[i], 0);
                packet.ReadXORByte(guid[i], 7);
                packet.ReadXORByte(guid[i], 3);

                if (hasItem[i] != 0)
                    packet.ReadUInt32("Item Entry", i);

                packet.ReadInt32("Time", i);

                if (hasMoney[i] != 0)
                    packet.ReadInt64("Money", i);

                if (tabChanged[i] != 0)
                    packet.ReadByte("Dest tab id", i);

                packet.WriteGuid("Guid", guid[i], i);
            }
            packet.ReadUInt32("Tab Id");

            if (hasWeekCashflow)
                packet.ReadInt64("Week Cash Flow Contribution");
        }

        [Parser(Opcode.CMSG_GUILD_BANK_LOG_QUERY, ClientVersionBuild.V4_3_3_15354)]
        public static void HandleGuildBankLogQuery(Packet packet)
        {
            packet.ReadUInt32("Tab Id");
        }

        [Parser(Opcode.SMSG_GUILD_DECLINE)]
        public static void HandleGuildDecline(Packet packet)
        {
            packet.ReadCString("Reason");
            packet.ReadBoolean("Auto decline");
        }

        [Parser(Opcode.SMSG_PETITION_SHOWLIST)]
        public static void HandlePetitionShowList(Packet packet)
        {
            packet.ReadGuid("GUID");
            var counter = packet.ReadByte("Counter");
            for (var i = 0; i < counter; i++)
            {
                packet.ReadUInt32("Index");
                packet.ReadUInt32("Charter Entry");
                packet.ReadUInt32("Charter Display");
                packet.ReadUInt32("Charter Cost");
                packet.ReadUInt32("Unk Uint32 1");
                packet.ReadUInt32("Required signs");
            }
        }

        [Parser(Opcode.CMSG_PETITION_BUY)]
        public static void HandlePetitionBuy(Packet packet)
        {
            packet.ReadGuid("GUID");
            packet.ReadUInt32("Unk UInt32 1");
            packet.ReadUInt64("Unk UInt64 1");
            packet.ReadCString("Name");
            packet.ReadCString("Text");
            packet.ReadUInt32("Unk UInt32 2");
            packet.ReadUInt32("Unk UInt32 3");
            packet.ReadUInt32("Unk UInt32 4");
            packet.ReadUInt32("Unk UInt32 5");
            packet.ReadUInt32("Unk UInt32 6");
            packet.ReadUInt32("Unk UInt32 7");
            packet.ReadUInt32("Unk UInt32 8");
            packet.ReadUInt16("Unk UInt16 1");
            packet.ReadUInt32("Unk UInt32 9");
            packet.ReadUInt32("Unk UInt32 10");
            packet.ReadUInt32("Unk UInt32 11");

            for (var i = 0; i < 10; i++)
                packet.ReadCString("Unk String", i);

            packet.ReadUInt32("Client Index");
            packet.ReadUInt32("Unk UInt32 12");
        }

        [Parser(Opcode.CMSG_PETITION_SHOW_SIGNATURES)]
        public static void HandlePetitionShowSignatures(Packet packet)
        {
            packet.ReadGuid("Petition GUID");
        }

        [Parser(Opcode.SMSG_PETITION_SHOW_SIGNATURES)]
        public static void HandlePetitionShowSignaturesServer(Packet packet)
        {
            packet.ReadGuid("Petition GUID");
            packet.ReadGuid("Owner GUID");
            packet.ReadUInt32("Guild/Team GUID");
            var counter = packet.ReadByte("Sign count");
            for (var i = 0; i < counter; i++)
            {
                packet.ReadGuid("Player GUID", i);
                packet.ReadUInt32("Unk UInt32 1", i);
            }
        }

        [Parser(Opcode.CMSG_PETITION_SIGN)]
        public static void HandlePetitionSign(Packet packet)
        {
            packet.ReadGuid("Petition GUID");
            packet.ReadByte("Unk Byte 1");
        }

        [Parser(Opcode.SMSG_PETITION_SIGN_RESULTS)]
        public static void HandlePetitionSignResult(Packet packet)
        {
            packet.ReadGuid("Petition GUID");
            packet.ReadGuid("Player GUID");
            packet.ReadEnum<PetitionResultType>("Petition Result", TypeCode.UInt32);
        }

        [Parser(Opcode.MSG_PETITION_DECLINE)]
        public static void HandlePetitionDecline(Packet packet)
        {
            packet.ReadGuid(packet.Direction == Direction.ClientToServer ? "Petition GUID" : "Player GUID");
        }

        [Parser(Opcode.CMSG_OFFER_PETITION)]
        public static void HandlePetitionOffer(Packet packet)
        {
            packet.ReadUInt32("Unk UInt3 1");
            packet.ReadGuid("Petition GUID");
            packet.ReadGuid("Target GUID");
        }

        [Parser(Opcode.CMSG_TURN_IN_PETITION)]
        public static void HandlePetitionTurnIn(Packet packet)
        {
            packet.ReadGuid("Petition GUID");
        }

        [Parser(Opcode.SMSG_TURN_IN_PETITION_RESULTS)]
        public static void HandlePetitionTurnInResults(Packet packet)
        {
            packet.ReadEnum<PetitionResultType>("Result", TypeCode.UInt32);
        }

        [Parser(Opcode.CMSG_PETITION_QUERY)]
        public static void HandlePetitionQuery(Packet packet)
        {
            packet.ReadUInt32("Guild/Team GUID");
            packet.ReadGuid("Petition GUID");
        }

        [Parser(Opcode.SMSG_PETITION_QUERY_RESPONSE)]
        public static void HandlePetitionQueryResponse(Packet packet)
        {
            var ownerGuid = new byte[8];

            var isGuild = packet.ReadBit("isGuild");

            if (isGuild)
            {
                ownerGuid[5] = packet.ReadBit();
                var nameLen = (int)packet.ReadBits("nameLen", 7);
                ownerGuid[3] = packet.ReadBit();
                ownerGuid[6] = packet.ReadBit();
                ownerGuid[1] = packet.ReadBit();

                var unkLens = new int[10];
                for (int i = 0; i < 10; ++i)
                    unkLens[i] = (int)packet.ReadBits("unkLens", 6, i);

                ownerGuid[0] = packet.ReadBit();
                var unkLen = (int)packet.ReadBits("unkLen", 12);
                ownerGuid[2] = packet.ReadBit();
                ownerGuid[7] = packet.ReadBit();
                ownerGuid[4] = packet.ReadBit();

                packet.ReadXORByte(ownerGuid, 0);
                packet.ReadUInt32("Unk UInt32");
                packet.ReadUInt32("Unk UInt32");
                packet.ReadUInt32("Unk UInt32");
                packet.ReadWoWString("unkString", unkLen);
                packet.ReadUInt32("Unk UInt32");
                packet.ReadXORByte(ownerGuid, 5);
                packet.ReadUInt32("Unk UInt32");
                packet.ReadXORByte(ownerGuid, 6);
                packet.ReadXORByte(ownerGuid, 4);
                packet.ReadWoWString("name", nameLen);
                packet.ReadXORByte(ownerGuid, 3);
                packet.ReadUInt32("Unk UInt32");
                packet.ReadXORByte(ownerGuid, 2);
                packet.ReadUInt16("Unk UInt16");
                packet.ReadUInt32("Unk UInt32");
                packet.ReadUInt32("Unk UInt32");
                packet.ReadUInt32("Unk UInt32");
                packet.ReadUInt32("Unk UInt32");
                packet.ReadXORByte(ownerGuid, 7);
                packet.ReadXORByte(ownerGuid, 1);

                for (int i = 0; i < 10; ++i)
                    packet.ReadWoWString("unkStrings", unkLens[i], i);

                packet.ReadUInt32("Unk UInt32");
                packet.ReadUInt32("Unk UInt32");
                packet.ReadUInt32("Unk UInt32");

                packet.WriteGuid("Creator GUID", ownerGuid);
            }

            packet.ReadUInt32("Unk UInt32");
        }

        [Parser(Opcode.MSG_PETITION_RENAME)]
        public static void HandlePetitionRename(Packet packet)
        {
            packet.ReadGuid("Petition GUID");
            packet.ReadCString("New Name");
        }

        [Parser(Opcode.SMSG_OFFER_PETITION_ERROR)]
        public static void HandlePetitionError(Packet packet)
        {
            packet.ReadGuid("Petition GUID");
        }

        [Parser(Opcode.SMSG_GUILD_CHALLENGE_UPDATED)]
        public static void HandleGuildChallengeUpdated(Packet packet)
        {
            for (int i = 0; i < 6; ++i)
                packet.ReadInt32("Total Count", i);

            for (int i = 0; i < 6; ++i)
                packet.ReadInt32("Current Count", i);

            for (int i = 0; i < 6; ++i)
                packet.ReadInt32("Guild Experience Reward", i);

            for (int i = 0; i < 6; ++i)
                packet.ReadInt32("Gold Reward Unk 2", i); // requires perk Cash Flow?

            for (int i = 0; i < 6; ++i)
                packet.ReadInt32("Completion Gold Reward", i);
        }

        [Parser(Opcode.SMSG_GUILD_CHALLENGE_COMPLETED, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildChallengeCompleted(Packet packet)
        {
            packet.ReadInt32("Index"); // not confirmed
            packet.ReadInt32("Gold Reward");
            packet.ReadInt32("Current Count");
            packet.ReadInt32("Guild Experience Reward");
            packet.ReadInt32("Total Count");
        }

        [Parser(Opcode.SMSG_GUILD_REPUTATION_WEEKLY_CAP)]
        public static void HandleGuildReputationWeeklyCap(Packet packet)
        {
            packet.ReadUInt32("Cap");
        }

        [Parser(Opcode.CMSG_GUILD_SET_ACHIEVEMENT_TRACKING)]
        public static void HandleGuildSetAchievementTracking(Packet packet)
        {
            var count = packet.ReadBits("Count", 24);
            for (var i = 0; i < count; ++i)
                packet.ReadUInt32("Criteria Id", i);
        }

        [Parser(Opcode.CMSG_QUERY_GUILD_RECIPES, ClientVersionBuild.Zero, ClientVersionBuild.V5_1_0_16309)] // 4.3.4
        public static void HandleQueryGuildRecipes(Packet packet)
        {
            var guid = packet.StartBitStream(5, 6, 1, 4, 2, 7, 0, 3);
            packet.ParseBitStream(guid, 3, 1, 0, 5, 4, 2, 6, 7);
            packet.WriteGuid("Guild Guid", guid);
        }

        [Parser(Opcode.CMSG_QUERY_GUILD_RECIPES, ClientVersionBuild.V5_1_0_16309)]
        public static void HandleQueryGuildRecipes510(Packet packet)
        {
            var guid = packet.StartBitStream(1, 4, 3, 5, 0, 6, 2, 7);
            packet.ParseBitStream(guid, 5, 3, 1, 4, 0, 7, 6, 2);
            packet.WriteGuid("Guild Guid", guid);
        }

        [Parser(Opcode.SMSG_GUILD_RECIPES)]
        public static void HandleGuildRecipes(Packet packet)
        {
            var count = packet.ReadBits("Count", 15);

            for (int i = 0; i < count; ++i)
            {
                packet.ReadUInt32("Skill Id", i);        // iterate all SkillLineAbility.dbc rows:
                for (int j = 0; j < 300; ++j)            // if (entry->skillId != "Skill Id") continue;
                    packet.ReadByte("Bit Index", i, j);  // if (mask[entry->col13 / 8] & (entry->col13 & 0x7)) recipe_spell_id: entry->spellId
            }
        }

        [Parser(Opcode.SMSG_GUILD_MAX_DAILY_XP)]
        [Parser(Opcode.SMSG_GUILD_XP_GAIN)]
        public static void HandleGuildXPResponse(Packet packet)
        {
            packet.ReadInt64("XP");
        }

        [Parser(Opcode.CMSG_GUILD_ACHIEVEMENT_PROGRESS_QUERY)]
        public static void HandleGuildAchievementProgressQuery(Packet packet)
        {
            packet.ReadUInt32("Guild Achievement Id");
        }

        [Parser(Opcode.SMSG_GUILD_UPDATE_ROSTER)]
        public static void HandleGuildUpdateRoster(Packet packet)
        {
            var count = packet.ReadBits("Count", 18);
            var guids = new byte[count][];
            var strlen = new uint[count][];
            var param = new bool[count][];

            for (int i = 0; i < count; ++i)
            {
                guids[i] = new byte[8];
                strlen[i] = new uint[3];
                param[i] = new bool[2];

                strlen[i][2] = packet.ReadBits(7);
                guids[i][5] = packet.ReadBit();
                guids[i][4] = packet.ReadBit();
                guids[i][0] = packet.ReadBit();
                guids[i][7] = packet.ReadBit();
                guids[i][1] = packet.ReadBit();
                guids[i][3] = packet.ReadBit();
                strlen[i][0] = packet.ReadBits(8);
                guids[i][6] = packet.ReadBit();
                param[i][0] = packet.ReadBit();
                guids[i][2] = packet.ReadBit();
                param[i][1] = packet.ReadBit();
                strlen[i][1] = packet.ReadBits(8);
            }

            for (int i = 0; i < count; ++i)
            {
                packet.ReadByte("unk Byte 359", i); // 0 or 1
                packet.ReadUInt32("Remaining guild week Rep", i);

                packet.ReadXORByte(guids[i], 1);

                packet.ReadInt64("Total activity", i);

                packet.ReadXORByte(guids[i], 2);

                for (int j = 0; j < 2; ++j)
                {
                    var rank = packet.ReadUInt32();
                    var id = packet.ReadUInt32();
                    var value = packet.ReadUInt32();
                    packet.WriteLine("[{0}][{1}] Profession: Id {2} - Value {3} - Rank {4}", i, j, id, value, rank);
                }

                packet.ReadXORByte(guids[i], 0);
                packet.ReadXORByte(guids[i], 6);
                packet.ReadXORByte(guids[i], 7);

                packet.ReadInt32("Rank", i);
                packet.ReadWoWString("Public Comment", strlen[i][0], i);
                packet.ReadWoWString("Officers Comment", strlen[i][1], i);

                packet.ReadXORByte(guids[i], 4);
                packet.ReadXORByte(guids[i], 5);

                packet.ReadInt32("Guild Reputation", i);
                packet.ReadInt32("Member Achievement Points", i);
                packet.ReadSingle("Last Online", i);
                packet.ReadInt64("Week activity", i);
                packet.ReadByte("Level", i);
                packet.ReadEnum<Class>("Class", TypeCode.Byte, i);

                packet.ReadXORByte(guids[i], 3);

                packet.ReadByte("unk Byte 356", i); // 0
                packet.ReadEntryWithName<Int32>(StoreNameType.Zone, "Zone Id", i);
                packet.ReadWoWString("Character Name", strlen[i][2], i);
                packet.ReadInt32("unk Int32 44", i);
                packet.WriteLine("Can SoR: {0}", param[i][0], i);
                packet.WriteLine("Has Authenticator: {0}", param[i][1], i);

                packet.WriteGuid("Player Guid", guids[i], i);
            }
        }

        [Parser(Opcode.CMSG_GUILD_INVITE_BY_NAME)]
        public static void HandleGuildInviteByName(Packet packet)
        {
            var count = packet.ReadBits("String Length", 7);
            packet.ReadWoWString("Name", count);
        }

        [Parser(Opcode.SMSG_GUILD_REPUTATION_REACTION_CHANGED, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildReputationReactionChanged(Packet packet)
        {
            var guid = packet.StartBitStream(1, 6, 2, 4, 0, 3, 7, 5);
            packet.ParseBitStream(guid, 4, 6, 5, 7, 2, 0, 3, 1);
            packet.WriteGuid("Guid", guid);
        }

        [Parser(Opcode.CMSG_GUILD_ADD_RANK, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildAddRank(Packet packet)
        {
            packet.ReadUInt32("Rank ID");
            var count = packet.ReadBits(7);
            packet.ReadWoWString("Name", count);
        }

        [Parser(Opcode.SMSG_GUILD_MEMBER_UPDATE_NOTE)]
        public static void HandleGuildMemberUpdateNote(Packet packet)
        {
            var guid = new byte[8];

            guid[7] = packet.ReadBit();
            guid[2] = packet.ReadBit();
            guid[3] = packet.ReadBit();
            var len = packet.ReadBits("Note Text Length", 8);
            guid[5] = packet.ReadBit();
            guid[0] = packet.ReadBit();
            guid[6] = packet.ReadBit();
            guid[4] = packet.ReadBit();
            packet.ReadBit("Public");
            guid[1] = packet.ReadBit();

            packet.ReadXORByte(guid, 3);
            packet.ReadXORByte(guid, 0);
            packet.ReadXORByte(guid, 2);
            packet.ReadXORByte(guid, 5);

            packet.ReadWoWString("Note", len);

            packet.ReadXORByte(guid, 7);
            packet.ReadXORByte(guid, 6);
            packet.ReadXORByte(guid, 1);
            packet.ReadXORByte(guid, 4);

            packet.WriteGuid("Guid", guid);

        }

        [Parser(Opcode.CMSG_GUILD_ACHIEVEMENT_MEMBERS, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildAchievementMembers(Packet packet)
        {
            var guid = new byte[8];
            var guid2 = new byte[8];

            packet.ReadInt32("Achievement Id");

            guid[0] = packet.ReadBit();
            guid2[5] = packet.ReadBit();
            guid2[4] = packet.ReadBit();
            guid2[0] = packet.ReadBit();
            guid2[7] = packet.ReadBit();
            guid[5] = packet.ReadBit();
            guid[7] = packet.ReadBit();
            guid2[3] = packet.ReadBit();
            guid[3] = packet.ReadBit();
            guid2[2] = packet.ReadBit();
            guid[4] = packet.ReadBit();
            guid[1] = packet.ReadBit();
            guid[6] = packet.ReadBit();
            guid2[6] = packet.ReadBit();
            guid[2] = packet.ReadBit();
            guid2[1] = packet.ReadBit();

            packet.ReadXORByte(guid, 0);
            packet.ReadXORByte(guid, 3);
            packet.ReadXORByte(guid2, 2);
            packet.ReadXORByte(guid, 7);
            packet.ReadXORByte(guid, 2);
            packet.ReadXORByte(guid2, 5);
            packet.ReadXORByte(guid2, 0);
            packet.ReadXORByte(guid2, 3);
            packet.ReadXORByte(guid, 5);
            packet.ReadXORByte(guid, 1);
            packet.ReadXORByte(guid2, 4);
            packet.ReadXORByte(guid2, 6);
            packet.ReadXORByte(guid2, 1);
            packet.ReadXORByte(guid2, 7);
            packet.ReadXORByte(guid, 4);
            packet.ReadXORByte(guid, 6);

            packet.WriteGuid("Guild Guid", guid);
            packet.WriteGuid("Player Guid", guid2);

        }

        [Parser(Opcode.SMSG_GUILD_ACHIEVEMENT_MEMBERS, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildAchievementMembersResponse(Packet packet)
        {
            var guid = new byte[8];


            guid[3] = packet.ReadBit();
            guid[4] = packet.ReadBit();
            guid[0] = packet.ReadBit();
            guid[7] = packet.ReadBit();

            var count = packet.ReadBits("Player Count", 26);
            var guid2 = new byte[count][];
            for (var i = 0; i < count; i++)
                guid2[i] = packet.StartBitStream(3,1,4,5,7,0,6,2);

            guid[2] = packet.ReadBit();
            guid[6] = packet.ReadBit();
            guid[5] = packet.ReadBit();
            guid[1] = packet.ReadBit();

            packet.ReadXORByte(guid, 5);

            for (var i = 0; i < count; i++)
            {
                packet.ParseBitStream(guid2[i], 1, 5, 7, 0, 6, 4, 3, 2);
                packet.WriteGuid("Player Guid", guid2[i], i);
            }

            packet.ReadXORByte(guid, 7);
            packet.ReadXORByte(guid, 2);
            packet.ReadXORByte(guid, 4);
            packet.ReadXORByte(guid, 3);
            packet.ReadXORByte(guid, 6);
            packet.ReadXORByte(guid, 0);

            packet.ReadInt32("Achievement Id");

            packet.ReadXORByte(guid, 1);

            packet.WriteGuid("Guild Guid", guid);

        }

        [Parser(Opcode.SMSG_GUILD_ACHIEVEMENT_EARNED, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGuildAchievementEarned(Packet packet)
        {
            var guid = packet.StartBitStream(3, 1, 0, 7, 4, 6, 2, 5);

            packet.ReadXORByte(guid, 2);

            packet.ReadPackedTime("Time");

            packet.ReadXORByte(guid, 0);
            packet.ReadXORByte(guid, 4);
            packet.ReadXORByte(guid, 1);
            packet.ReadXORByte(guid, 3);

            packet.ReadInt32("Achievement Id");

            packet.ReadXORByte(guid, 7);
            packet.ReadXORByte(guid, 5);
            packet.ReadXORByte(guid, 6);

            packet.WriteGuid("Guild Guid", guid);
        }

        [Parser(Opcode.SMSG_GUILD_ACHIEVEMENT_DATA, ClientVersionBuild.Zero, ClientVersionBuild.V4_3_0_15005)]
        public static void HandleGuildAchievementData(Packet packet)
        {
            var cnt = packet.ReadUInt32("Count");
            for (var i = 0; i < cnt; ++i)
                packet.ReadPackedTime("Date", i);

            for (var i = 0; i < cnt; ++i)
                packet.ReadUInt32("Achievement Id", i);
        }

        [Parser(Opcode.SMSG_GUILD_ACHIEVEMENT_DATA, ClientVersionBuild.V4_3_0_15005)]
        public static void HandleGuildAchievementData430(Packet packet)
        {
            var count = packet.ReadBits("Count", 23);
            for (var i = 0; i < count; ++i)
            {
                packet.ReadPackedTime("Date", i);
                packet.ReadUInt32("Achievement Id", i);
            }
        }

        [Parser(Opcode.SMSG_GUILD_CRITERIA_DATA)]
        public static void HandleGuildCriteriaData(Packet packet)
        {
            var count = packet.ReadBits("Count", 19);
            var counter = new byte[count][];
            var guid = new byte[count][];

            for (var i = 0; i < count; ++i)
            {
                counter[i] = new byte[8];
                guid[i] = new byte[8];

                counter[i][3] = packet.ReadBit();
                counter[i][6] = packet.ReadBit();
                guid[i][5] = packet.ReadBit();
                guid[i][4] = packet.ReadBit();
                counter[i][2] = packet.ReadBit();
                guid[i][0] = packet.ReadBit();
                counter[i][7] = packet.ReadBit();
                guid[i][6] = packet.ReadBit();
                guid[i][7] = packet.ReadBit();
                guid[i][3] = packet.ReadBit();
                counter[i][5] = packet.ReadBit();
                counter[i][0] = packet.ReadBit();
                guid[i][2] = packet.ReadBit();
                counter[i][1] = packet.ReadBit();
                counter[i][4] = packet.ReadBit();
                guid[i][1] = packet.ReadBit();
            }

            for (var i = 0; i < count; ++i)
            {
                packet.ReadXORByte(guid[i], 4);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadXORByte(counter[i], 2);
                packet.ReadXORByte(counter[i], 0);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadXORByte(guid[i], 7);
                packet.ReadXORByte(guid[i], 1);
                packet.ReadUInt32("Unk", i);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadXORByte(guid[i], 6);
                packet.ReadXORByte(counter[i], 1);
                packet.ReadXORByte(guid[i], 3);
                packet.ReadXORByte(counter[i], 4);
                packet.ReadUInt32("Unk UInt32", i);
                packet.ReadXORByte(counter[i], 5);
                packet.ReadXORByte(counter[i], 3);
                packet.ReadXORByte(guid[i], 0);
                packet.ReadXORByte(counter[i], 6);
                packet.ReadXORByte(counter[i], 7);
                packet.ReadXORByte(guid[i], 2);
                packet.ReadXORByte(guid[i], 5);

                packet.WriteGuid("Criteria GUID", guid[i], i);
                packet.WriteLine("[{0}] Criteria counter: {1}", i, BitConverter.ToUInt64(counter[i], 0));
            }
        }

        [Parser(Opcode.SMSG_GUILD_MEMBER_DAILY_RESET)]
        [Parser(Opcode.CMSG_GUILD_REQUEST_CHALLENGE_UPDATE)]
        [Parser(Opcode.CMSG_GUILD_ROSTER, ClientVersionBuild.Zero, ClientVersionBuild.V4_0_6_13596)]
        [Parser(Opcode.CMSG_GUILD_ACCEPT)]
        [Parser(Opcode.CMSG_GUILD_DECLINE)]
        [Parser(Opcode.CMSG_GUILD_INFO)]
        [Parser(Opcode.CMSG_GUILD_LEAVE)]
        [Parser(Opcode.CMSG_GUILD_DISBAND)]
        [Parser(Opcode.CMSG_GUILD_DEL_RANK, ClientVersionBuild.Zero, ClientVersionBuild.V4_3_4_15595)]
        [Parser(Opcode.CMSG_GUILD_EVENT_LOG_QUERY)]
        [Parser(Opcode.SMSG_GUILD_CANCEL)] // Fires GUILD_INVITE_CANCEL
        [Parser(Opcode.SMSG_GUILD_INVITE_CANCEL)]
        public static void HandleGuildNull(Packet packet)
        {
            // Just to have guild opcodes together
        }
    }
}
