using System;
using WowPacketParser.Enums;
using WowPacketParser.Misc;
using WowPacketParser.Parsing;

namespace WowPacketParserModule.V5_3_0_16981.Parsers
{
    public static class ItemHandler
    {
        [Parser(Opcode.CMSG_REQUEST_HOTFIX)]
        public static void HandleItemRequestHotfix(Packet packet)
        {
            packet.ReadUInt32("Type");
            var count = packet.ReadBits("Count", 23);
            var guidBytes = new byte[count][];
            for (var i = 0; i < count; ++i)
                guidBytes[i] = packet.StartBitStream(3, 4, 7, 2, 5, 1, 6, 0);

            for (var i = 0; i < count; ++i)
            {
                packet.ReadXORByte(guidBytes[i], 6);
                packet.ReadXORByte(guidBytes[i], 1);
                packet.ReadXORByte(guidBytes[i], 2);

                packet.ReadEntryWithName<UInt32>(StoreNameType.Item, "Entry", i);

                packet.ReadXORByte(guidBytes[i], 4);
                packet.ReadXORByte(guidBytes[i], 5);
                packet.ReadXORByte(guidBytes[i], 7);
                packet.ReadXORByte(guidBytes[i], 0);
                packet.ReadXORByte(guidBytes[i], 3);

                packet.WriteGuid("GUID", guidBytes[i], i);
            }
        }

        [Parser(Opcode.SMSG_SET_PROFICIENCY)]
        public static void HandleSetProficency(Packet packet)
        {
            var guid = new byte[8];

            guid[0] = packet.ReadBit();
            guid[1] = packet.ReadBit();
            guid[5] = packet.ReadBit();
            var strLen = packet.ReadBits("strLen", 7);
            guid[7] = packet.ReadBit();
            guid[6] = packet.ReadBit();
            guid[3] = packet.ReadBit();
            guid[4] = packet.ReadBit();
            guid[2] = packet.ReadBit();

            packet.ReadXORByte(guid, 1);
            packet.ReadXORByte(guid, 3);
            packet.ReadXORByte(guid, 5);
            packet.ReadXORByte(guid, 2);
            packet.ReadWoWString("String", strLen);
            packet.ReadXORByte(guid, 4);
            packet.ReadUInt32("Unk UInt32");
            packet.ReadXORByte(guid, 0);
            packet.ReadXORByte(guid, 6);
            packet.ReadByte("Unk Byte");
            packet.ReadByte("Unk Byte");
            packet.ReadXORByte(guid, 7);

            packet.WriteGuid("Guid", guid);
        }

        [Parser(Opcode.SMSG_ITEM_ENCHANT_TIME_UPDATE)]
        public static void HandleItemEnchantTimeUpdate(Packet packet)
        {
            var playerGuid = new byte[8];
            var itemGuid = new byte[8];

            playerGuid[3] = packet.ReadBit();
            itemGuid[4] = packet.ReadBit();
            playerGuid[0] = packet.ReadBit();
            itemGuid[7] = packet.ReadBit();
            playerGuid[2] = packet.ReadBit();
            itemGuid[6] = packet.ReadBit();
            playerGuid[6] = packet.ReadBit();
            playerGuid[1] = packet.ReadBit();
            itemGuid[2] = packet.ReadBit();
            playerGuid[7] = packet.ReadBit();
            itemGuid[3] = packet.ReadBit();
            itemGuid[1] = packet.ReadBit();
            playerGuid[5] = packet.ReadBit();
            itemGuid[5] = packet.ReadBit();
            itemGuid[0] = packet.ReadBit();
            playerGuid[4] = packet.ReadBit();

            packet.ReadUInt32("Unk UInt32");
            packet.ReadXORByte(itemGuid, 2);
            packet.ReadXORByte(itemGuid, 3);
            packet.ReadXORByte(playerGuid, 7);
            packet.ReadXORByte(itemGuid, 0);
            packet.ReadUInt32("Unk UInt32");
            packet.ReadXORByte(playerGuid, 3);
            packet.ReadXORByte(itemGuid, 6);
            packet.ReadXORByte(playerGuid, 6);
            packet.ReadXORByte(playerGuid, 4);
            packet.ReadXORByte(playerGuid, 2);
            packet.ReadXORByte(itemGuid, 1);
            packet.ReadXORByte(playerGuid, 5);
            packet.ReadXORByte(itemGuid, 5);
            packet.ReadXORByte(itemGuid, 4);
            packet.ReadXORByte(itemGuid, 7);
            packet.ReadXORByte(playerGuid, 0);
            packet.ReadXORByte(playerGuid, 1);

            packet.WriteGuid("Item GUID", itemGuid);
            packet.WriteGuid("Player GUID", playerGuid);
        }
    }
}
