using System;
using WowPacketParser.Enums;
using WowPacketParser.Misc;
using WowPacketParser.Parsing;

namespace WowPacketParserModule.V5_3_0_16981.Parsers
{
    public static class InstanceHandler
    {
        [Parser(Opcode.CMSG_SAVE_CUF_PROFILES)]
        public static void HandleSaveCufProfiles(Packet packet)
        {
            var count = packet.ReadBits("Count", 19);

            var strlen = new uint[count];

            for (int i = 0; i < count; ++i)
            {
                packet.ReadBit("10 player group", i);
                packet.ReadBit("25 player group", i);
                packet.ReadBit("Pets", i);
                packet.ReadBit("Border", i);
                packet.ReadBit("Dispellable debuffs", i);
                packet.ReadBit("15 player group", i);
                packet.ReadBit("Incoming heals", i);
                packet.ReadBit("Unk 156", i);
                strlen[i] = packet.ReadBits("String length", 7, i);
                packet.ReadBit("Talent spec 1", i);
                packet.ReadBit("Keep groups together", i);
                packet.ReadBit("Unk 157", i);
                packet.ReadBit("2 player group", i);
                packet.ReadBit("Main tank and assist", i);
                packet.ReadBit("40 player group", i);
                packet.ReadBit("Unk 145", i);
                packet.ReadBit("Display power bars", i);
                packet.ReadBit("PvE", i);
                packet.ReadBit("3 player group", i);
                packet.ReadBit("Class colors", i);
                packet.ReadBit("Aggro highlight", i);
                packet.ReadBit("PvP", i);
                packet.ReadBit("Talent spec 2", i);
                packet.ReadBit("Debuffs", i);
                packet.ReadBit("Horizontal Groups", i);
                packet.ReadBit("5 player group", i);
            }

            for (int i = 0; i < count; ++i)
            {
                packet.ReadByte("Sort by", i);
                packet.ReadByte("Unk 146", i);
                packet.ReadInt16("Unk 154", i);
                packet.ReadWoWString("Name", (int)strlen[i], i);
                packet.ReadByte("Unk 147", i);
                packet.ReadByte("Health text", i);
                packet.ReadInt16("Unk 152", i);
                packet.ReadInt16("Unk 150", i);
                packet.ReadInt16("Frame height", i);
                packet.ReadByte("Unk 148", i);
                packet.ReadInt16("Frame width", i);
            }
        }

        [Parser(Opcode.SMSG_LOAD_CUF_PROFILES)]
        public static void HandleLoadCUFProfiles(Packet packet)
        {
            var count = packet.ReadBits("Count", 19);

            var strlen = new uint[count];

            for (int i = 0; i < count; ++i)
            {
                packet.ReadBit("Unk 168", i);
                packet.ReadBit("Unk 134", i);
                packet.ReadBit("Unk 142", i);
                packet.ReadBit("Unk 137", i);
                packet.ReadBit("Unk 135", i);
                packet.ReadBit("Unk 163", i);
                packet.ReadBit("Unk 160", i);
                packet.ReadBit("Unk 166", i);
                packet.ReadBit("Unk 139", i);
                packet.ReadBit("Unk 161", i);
                packet.ReadBit("Unk 159", i);
                packet.ReadBit("Unk 143", i);
                packet.ReadBit("Unk 138", i);
                packet.ReadBit("Unk 136", i);
                packet.ReadBit("Unk 167", i);
                packet.ReadBit("Unk 157", i);
                packet.ReadBit("Unk 162", i);
                packet.ReadBit("Unk 141", i);
                packet.ReadBit("Unk 158", i);
                packet.ReadBit("Unk 140", i);
                packet.ReadBit("Unk 144", i);
                strlen[i] = packet.ReadBits("String length", 7, i);
                packet.ReadBit("Unk 145", i);
                packet.ReadBit("Unk 156", i);
                packet.ReadBit("Unk 164", i);
                packet.ReadBit("Unk 165", i);
            }

            for (int i = 0; i < count; ++i)
            {
                packet.ReadInt16("Unk 150", i);
                packet.ReadInt16("Unk 128", i);
                packet.ReadByte("Unk 132", i);
                packet.ReadInt16("Unk 130", i);
                packet.ReadByte("Unk 133", i);
                packet.ReadInt16("Unk 154", i);
                packet.ReadByte("Unk 148", i);
                packet.ReadByte("Unk 147", i);
                packet.ReadByte("Unk 146", i);
                packet.ReadWoWString("Name", (int)strlen[i], i);
                packet.ReadInt16("Unk 152", i);
            }
        }
    }
}
