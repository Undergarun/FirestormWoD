using WowPacketParser.Enums;
using WowPacketParser.Misc;

namespace WowPacketParser.Parsing.Parsers
{
    public static class EquipmentSetHandler
    {
        private const int NumSlots = 19;

        public static void ReadSetInfo(ref Packet packet)
        {
            packet.ReadPackedGuid("Set ID");
            packet.ReadInt32("Index");
            packet.ReadCString("Set Name");
            packet.ReadCString("Set Icon");

            for (var j = 0; j < NumSlots; j++)
                packet.ReadPackedGuid("Item GUID " + j);
        }

        [Parser(Opcode.SMSG_EQUIPMENT_SET_LIST)]
        public static void HandleEquipmentSetList(Packet packet)
        {
            var count = packet.ReadInt32("Count");

            for (var i = 0; i < count; i++)
                ReadSetInfo(ref packet);
        }

        [Parser(Opcode.CMSG_EQUIPMENT_SET_SAVE)]
        public static void HandleEquipmentSetSave(Packet packet)
        {
            ReadSetInfo(ref packet);
        }

        [Parser(Opcode.SMSG_EQUIPMENT_SET_SAVED)]
        public static void HandleEquipmentSetSaved(Packet packet)
        {
            packet.ReadInt32("Index");

            packet.ReadPackedGuid("Set ID");
        }

        [Parser(Opcode.CMSG_EQUIPMENT_SET_USE)]
        public static void HandleEquipmentSetUse(Packet packet)
        {
            var srcBag = new int[19];
            var srcSlot = new int[19];
            var itemGuid = new byte[19][];

            for (int i = 0; i < 19; i++)
            {
                srcSlot[i] = packet.ReadByte("srcSlot", i);
                srcBag[i] = packet.ReadByte("srcBag", i);
            }

            for (int i = 0; i < 19; i++)
            {
                itemGuid[i] = new byte[8];

                packet.StartBitStream(itemGuid[i], 2, 5, 0, 1, 3, 6, 4, 7);
            }

            var unkCounter = packet.ReadBits("unkCounter", 2);

            for (int i = 0; i < unkCounter; i++)
            {
                packet.ReadBit("unkBit", i);
                packet.ReadBit("unkBit2", i);
            }

            packet.ResetBitReader();

            for (int i = 0; i < 19; i++)
            {
                packet.ParseBitStream(itemGuid[i], 4, 1, 6, 5, 3, 2, 0, 7);

                packet.WriteGuid("Item GUID", itemGuid[i], i);
            }

            for (int i = 0; i < unkCounter; i++)
            {
                packet.ReadByte("unkByte", i);
                packet.ReadByte("unkByte2", i);
            }
        }

        [Parser(Opcode.SMSG_DUMP_OBJECTS_DATA)]
        public static void HandleEquipmentSetUseResult(Packet packet)
        {
            packet.ReadByte("Result");
        }

        [Parser(Opcode.CMSG_DELETEEQUIPMENT_SET)]
        public static void HandleEquipmentSetDelete(Packet packet)
        {
            packet.ReadPackedGuid("Set ID");
        }
    }
}
