using System;
using WowPacketParser.Enums;
using WowPacketParser.Misc;
using WowPacketParser.Parsing;

namespace WowPacketParserModule.V5_3_0_16981.Parsers
{
    public static class ReputationHandler
    {
        [Parser(Opcode.SMSG_INITIALIZE_FACTIONS)]
        public static void HandleInitializeFactions(Packet packet)
        {
            for (var i = 0; i < 256; i++)
            {
                packet.ReadEnum<ReputationRank>("Faction Standing", TypeCode.UInt32, i);
                packet.ReadEnum<FactionFlag>("Faction Flags", TypeCode.Byte, i);
            }

            Console.WriteLine("Standing and Flags Parsed !");

            for (var i = 0; i < 256; i++)
            {
                var bit1296 = packet.ReadBit("Count", i);
            }

            Console.WriteLine("SMSG_INITIALIZE_FACTION Parsed !");
        }
    }
}
