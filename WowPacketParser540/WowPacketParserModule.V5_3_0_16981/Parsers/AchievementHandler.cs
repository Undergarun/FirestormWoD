using System;
using WowPacketParser.Enums;
using WowPacketParser.Misc;

namespace WowPacketParser.Parsing.Parsers
{
    public static class AchievementHandler
    {
        [Parser(Opcode.SMSG_CRITERIA_UPDATE)]
        public static void HandleCriteriaUpdate(Packet packet)
        {
            var guid2 = new byte[8];

            packet.ReadUInt32("Criteria ID");
            packet.ReadUInt32("Completed");
            packet.ReadUInt32("Timer 1");
            packet.ReadUInt32("Timer 2");
            packet.ReadGuid("Player GUID");
            packet.ReadUInt32("Unk Flag");

            guid2 = packet.StartBitStream(2, 4, 1, 5, 3, 6, 7, 0);

            packet.ParseBitStream(guid2, 7, 0, 6, 5, 2, 1, 4, 3);

            packet.WriteGuid("Counter", guid2);
        }
    }
}
