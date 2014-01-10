using System;
using WowPacketParser.Enums;
using WowPacketParser.Misc;
using WowPacketParser.Parsing;

namespace WowPacketParserModule.V5_3_0_16981.Parsers
{
    public static class GuildHandler
    {
        [Parser(Opcode.SMSG_GUILD_QUERY_RESPONSE)]
        public static void HandleGuildQueryResponse(Packet packet)
        {
            var guid1 = new byte[8];
            var guid2 = new byte[8];

            packet.StartBitStream(guid1, 6, 2, 0, 3, 4, 1, 5);

            var hasData = packet.ReadBit("hasData");

            int nameLen = 0;
            int rankCount = 0;
            int[] rankName = null;
            if (hasData)
            {
                packet.StartBitStream(guid2, 0);
                nameLen = (int)packet.ReadBits("nameLength", 7);
                rankCount = (int)packet.ReadBits("Rank count", 21);
                rankName = new int[rankCount];

                for (var i = 0; i < rankCount; ++i)
                    rankName[i] = (int)packet.ReadBits("rankNameLength", 7);

                packet.StartBitStream(guid2, 1, 2, 5, 3, 7, 4, 6);

                guid1[7] = packet.ReadBit();

                packet.ReadUInt32("Emblem Style");

                for (var i = 0; i < rankCount; ++i)
                {
                    packet.ReadUInt32("Creation Order", i);
                    packet.ReadWoWString("Rank Name", rankName[i], i);
                    packet.ReadUInt32("Rights Order", i);
                }

                packet.ReadXORByte(guid2, 1);

                packet.ReadUInt32("Realm ID");
                packet.ReadUInt32("Emblem Color");
                packet.ReadUInt32("BackGround Color");
                packet.ReadUInt32("Border Style");
                packet.ReadXORByte(guid2, 0);
                packet.ReadUInt32("Border Color");
                packet.ReadXORByte(guid2, 6);
                packet.ReadWoWString("Guild Name", nameLen);
                packet.ReadXORByte(guid2, 5);
                packet.ReadXORByte(guid2, 3);
                packet.ReadXORByte(guid2, 2);
                packet.ReadXORByte(guid2, 7);
                packet.ReadXORByte(guid2, 4);
                packet.ReadXORBytes(guid1, 4);
                packet.ReadXORBytes(guid1, 1);
                packet.ReadXORBytes(guid1, 0);
                packet.ReadXORBytes(guid1, 3);
                packet.ReadXORBytes(guid1, 5);
                packet.ReadXORBytes(guid1, 7);
                packet.ReadXORBytes(guid1, 6);
                packet.ReadXORBytes(guid1, 2);

                packet.WriteGuid("Guild GUID", guid2);
                packet.WriteGuid("Player GUID", guid1);
            }
        }

        [Parser(Opcode.SMSG_PLAYER_TABAR_VENDOR_ACTIVATE)]
        public static void HandlePlayerTabarVendorActivate(Packet packet)
        {
            packet.ReadUInt32("Realm ID");
            var bytesCount = packet.ReadUInt32("bytes Count");

            for (int i = 0; i < bytesCount; i++)
                packet.ReadByte("Unk Byte", i);

            var guid = new byte[8];

            guid[1] = packet.ReadBit();
            var bit32 = packet.ReadBit("Bit 32");
            guid[7] = packet.ReadBit();
            guid[2] = packet.ReadBit();
            guid[6] = packet.ReadBit();
            guid[3] = packet.ReadBit();
            guid[4] = packet.ReadBit();
            guid[5] = packet.ReadBit();
            guid[0] = packet.ReadBit();
            var bit33 = packet.ReadBit("Bit 33");

            packet.ReadXORByte(guid, 6);
            packet.ReadXORByte(guid, 1);
            packet.ReadXORByte(guid, 4);
            packet.ReadXORByte(guid, 2);
            packet.ReadXORByte(guid, 5);
            packet.ReadXORByte(guid, 0);
            packet.ReadXORByte(guid, 3);
            packet.ReadXORByte(guid, 7);

            packet.WriteGuid("Player GUID", guid);
        }
    }
}
