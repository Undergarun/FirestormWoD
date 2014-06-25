using System;
using System.Collections.Generic;
using WowPacketParser.Enums;
using WowPacketParser.Enums.Version;
using WowPacketParser.Misc;
using WowPacketParser.Store;
using WowPacketParser.Store.Objects;

namespace WowPacketParser.Parsing.Parsers
{
    public static class LootHandler
    {
        [Parser(Opcode.SMSG_COIN_REMOVED)]
        public static void HandleCoinRemoved(Packet packet)
        {
            var guid = packet.StartBitStream(1, 3, 4, 0, 5, 6, 2, 7);
            packet.ParseBitStream(guid, 1, 4, 0, 6, 3, 7, 5, 2);

            packet.WriteGuid("Npc GUID", guid);
        }

        [Parser(Opcode.SMSG_LOOT_MONEY_NOTIFY)]
        public static void HandleLootMoneyNotify(Packet packet)
        {
            packet.ReadUInt32("Gold");

            if (ClientVersion.AddedInVersion(ClientType.WrathOfTheLichKing)) // no idea when this was added, doesn't exist in 2.4.1
                packet.ReadBoolean("Solo Loot"); // true = YOU_LOOT_MONEY, false = LOOT_MONEY_SPLIT

            if (ClientVersion.AddedInVersion(ClientVersionBuild.V4_0_6a_13623) && ClientVersion.RemovedInVersion(ClientVersionBuild.V4_3_0_15005)) // remove confirmed for 430
                packet.ReadUInt32("Guild Gold");
        }

        [Parser(Opcode.CMSG_LOOT, ClientVersionBuild.Zero, ClientVersionBuild.V5_1_0_16309)]
        [Parser(Opcode.CMSG_LOOT_RELEASE, ClientVersionBuild.Zero, ClientVersionBuild.V5_1_0_16309)]
        public static void HandleLoot(Packet packet)
        {
            packet.ReadGuid("GUID");
        }

        [Parser(Opcode.CMSG_LOOT, ClientVersionBuild.V5_1_0_16309)]
        public static void HandleLoot510(Packet packet)
        {
            var guid = packet.StartBitStream(1, 2, 7, 3, 6, 0, 4, 5);
            packet.ParseBitStream(guid, 1, 3, 5, 4, 0, 7, 6, 2);
            packet.WriteGuid("GUID", guid);
        }

        [Parser(Opcode.CMSG_LOOT_RELEASE, ClientVersionBuild.V5_1_0_16309)]
        public static void HandleLootRelease510(Packet packet)
        {
            var guid = packet.StartBitStream(4, 0, 6, 2, 3, 7, 1, 5);
            packet.ParseBitStream(guid, 0, 4, 1, 6, 7, 5, 3, 2);
            packet.WriteGuid("GUID", guid);
        }

        [Parser(Opcode.CMSG_LOOT_MASTER_GIVE)]
        public static void HandleLootMasterGive(Packet packet)
        {
            packet.ReadGuid("Loot GUID");
            packet.ReadByte("Slot");
            packet.ReadGuid("Player GUID");
        }

        [Parser(Opcode.CMSG_LOOT_METHOD)]
        public static void HandleLootMethod(Packet packet)
        {
            packet.ReadEnum<LootMethod>("Loot Method", TypeCode.UInt32);
            packet.ReadGuid("Master GUID");
            packet.ReadEnum<ItemQuality>("Loot Threshold", TypeCode.UInt32);
        }

        [Parser(Opcode.CMSG_OPT_OUT_OF_LOOT)]
        public static void HandleOptOutOfLoot(Packet packet)
        {
            if (ClientVersion.AddedInVersion(ClientVersionBuild.V4_3_4_15595))
                packet.ReadBoolean("Always Pass");
            else
                packet.ReadUInt32("Always Pass");
        }

        [Parser(Opcode.SMSG_LOOT_ALL_PASSED)]
        public static void HandleLootAllPassed(Packet packet)
        {
            packet.ReadGuid("GUID");
            packet.ReadUInt32("Slot");
            packet.ReadEntryWithName<UInt32>(StoreNameType.Item, "Entry");
            packet.ReadInt32("Random Property Id");
            packet.ReadInt32("Random Suffix");
        }

        [Parser(Opcode.SMSG_LOOT_LIST)]
        public static void Handle(Packet packet)
        {
            packet.ReadGuid("Creature GUID");
            packet.ReadPackedGuid("Master Loot GUID?");
            packet.ReadPackedGuid("Looter GUID");
        }

        [Parser(Opcode.SMSG_LOOT_RELEASE_RESPONSE)]
        public static void HandleLootReleaseResponse(Packet packet)
        {
            packet.ReadGuid("GUID");
            packet.ReadBoolean("Unk Bool"); // true calls CGUnit_C::UpdateLootAnimKit and CGameUI::CloseLoot
        }

        [Parser(Opcode.SMSG_LOOT_REMOVED)]
        public static void HandleLootRemoved(Packet packet)
        {
            var lootGuid = new byte[8];
            var guid2 = new byte[8];

            packet.StartBitStream(lootGuid, 7, 0, 5);
            guid2[5] = packet.ReadBit();
            lootGuid[6] = packet.ReadBit();
            guid2[2] = packet.ReadBit();
            guid2[3] = packet.ReadBit();
            lootGuid[2] = packet.ReadBit();
            guid2[1] = packet.ReadBit();
            guid2[7] = packet.ReadBit();
            lootGuid[1] = packet.ReadBit();
            guid2[4] = packet.ReadBit();
            guid2[6] = packet.ReadBit();
            lootGuid[4] = packet.ReadBit();
            lootGuid[3] = packet.ReadBit();
            guid2[0] = packet.ReadBit();

            packet.ReadXORBytes(lootGuid, 5, 4);
            packet.ReadXORByte(guid2, 6);
            packet.ReadXORBytes(lootGuid, 6, 7);
            packet.ReadXORBytes(guid2, 3, 0);
            packet.ReadByte("Slot");
            packet.ReadXORBytes(lootGuid, 3, 1);
            packet.ReadXORByte(guid2, 5);
            packet.ReadXORByte(lootGuid, 2);
            packet.ReadXORBytes(guid2, 2, 7, 1, 4);
            packet.ReadXORByte(lootGuid, 0);

            packet.WriteGuid("Loot GUID", lootGuid);
            packet.WriteGuid("Unk GUID", guid2);
        }

        [Parser(Opcode.SMSG_LOOT_RESPONSE)]
        public static void HandleLootResponse(Packet packet)
        {
            var creature = new byte[8];
            var loots = new byte[8];

            var unkbit69 = packet.ReadBit("unkbit69");
            creature[0] = packet.ReadBit();
            creature[1] = packet.ReadBit();
            var unkbit48 = !packet.ReadBit();
            Console.WriteLine("unkbit48: " + unkbit48);
            loots[7] = packet.ReadBit();
            creature[3] = packet.ReadBit();
            loots[2] = packet.ReadBit();
            loots[0] = packet.ReadBit();
            var permission = 4 - packet.ReadBit();
            Console.WriteLine("Permission: " + permission);
            loots[1] = packet.ReadBit();
            var unkbit84 = packet.ReadBit("unkbit84");
            creature[6] = packet.ReadBit();
            var hasLootType = !packet.ReadBit();
            Console.WriteLine("hasLootType: " + hasLootType);
            loots[3] = packet.ReadBit();
            var unkbit32 = !packet.ReadBit();
            Console.WriteLine("unkbit32: " + unkbit32);

            var currenciesShown = packet.ReadBits("currenciesShown", 20);

            creature[7] = packet.ReadBit();
            creature[4] = packet.ReadBit();
            loots[6] = packet.ReadBit();

            var unkBits = new uint[currenciesShown];

            for (int i = 0; i < currenciesShown; i++)
                unkBits[i] = packet.ReadBits("unkBits", 3, i); // 6

            creature[2] = packet.ReadBit();
            var hasGold = !packet.ReadBit();
            Console.WriteLine("hasGold: " + hasGold);
            loots[4] = packet.ReadBit();

            var itemsShown = packet.ReadBits("itemsShown", 19);

            var slot = new bool[itemsShown];
            var slotType = new bool[itemsShown];
            var bit32 = new bool[itemsShown];
            var bits2 = new uint[itemsShown];
            var bits3 = new uint[itemsShown];
            for (int i = 0; i < itemsShown; i++)
            {
                slotType[i] = !packet.ReadBit();
                Console.WriteLine("slotType[" + i + "]: " + slotType[i]);
                slot[i] = !packet.ReadBit();
                Console.WriteLine("slot[" + i + "]: " + slot[i]);
                bits2[i] = packet.ReadBits("bits2", 2, i);
                bit32[i] = packet.ReadBit("bit32", i);
                bits3[i] = packet.ReadBits("bits3", 3, i);
            }

            creature[5] = packet.ReadBit();
            var unk = packet.ReadBit("unk"); // 0

            if (permission != 3)
                packet.ReadByte("Unk Byte"); // 0

            for (int i = 0; i < currenciesShown; i++)
            {
                packet.ReadByte("Slot", i); // 1
                packet.ReadUInt32("Currency ID", i);
                packet.ReadUInt32("Currency count", i);
            }

            packet.ReadXORByte(creature, 3);

            for (int i = 0; i < itemsShown; i++)
            {
                if (slot[i])
                    packet.ReadByte("Slot", i);

                packet.ReadUInt32("Item Count", i);

                if (slotType[i])
                    packet.ReadEnum<LootSlotType>("Slot Type", TypeCode.Byte, i);

                packet.ReadUInt32("Display ID", i);
                packet.ReadUInt32("Random Suffix", i);
                packet.ReadUInt32("Random Property ID", i);
                var bytesCounter = packet.ReadUInt32("BytesCounter", i);

                for (int j = 0; j < bytesCounter; j++)
                    packet.ReadByte("ByteCount", i, j);

                packet.ReadUInt32("Item ID", i);
            }

            packet.ReadXORByte(creature, 0);

            if (hasGold)
                packet.ReadUInt32("Gold");

            if (hasLootType)
                packet.ReadEnum<LootType>("Loot Type", TypeCode.Byte);

            packet.ReadXORByte(loots, 5);
            packet.ReadXORByte(creature, 7);
            packet.ReadXORByte(creature, 6);
            packet.ReadXORByte(loots, 1);

            // 2
            if (unkbit48)
                packet.ReadByte("Unk Byte 48");

            // 17
            if (unkbit32)
                packet.ReadByte("Unk Byte 32");

            packet.ReadXORByte(loots, 0);
            packet.ReadXORByte(creature, 2);
            packet.ReadXORByte(creature, 5);
            packet.ReadXORByte(loots, 7);
            packet.ReadXORByte(loots, 3);
            packet.ReadXORByte(creature, 1);
            packet.ReadXORByte(loots, 2);
            packet.ReadXORByte(creature, 4);
            packet.ReadXORByte(loots, 4);
            packet.ReadXORByte(loots, 6);

            packet.WriteGuid("Creature GUID", creature);
            packet.WriteGuid("Loots GUID", loots);
        }

        [Parser(Opcode.CMSG_LOOT_ROLL)]
        public static void HandleLootRoll(Packet packet)
        {
            packet.ReadGuid("GUID");
            packet.ReadUInt32("Slot");
            packet.ReadEnum<LootRollType>("Roll Type", TypeCode.Byte);
        }

        [Parser(Opcode.SMSG_LOOT_ROLL)]
        public static void HandleLootRollResponse(Packet packet)
        {
            packet.ReadGuid("GUID");
            packet.ReadUInt32("Slot");
            packet.ReadGuid("Player GUID");
            packet.ReadEntryWithName<UInt32>(StoreNameType.Item, "Entry");
            packet.ReadInt32("Random Property Id");
            packet.ReadInt32("Random Suffix");
            if (ClientVersion.AddedInVersion(ClientVersionBuild.V4_3_0_15005))
                packet.ReadInt32("Roll Number");
            else
                packet.ReadByte("Roll Number");
            packet.ReadEnum<LootRollType>("Roll Type", TypeCode.Byte);
            packet.ReadBoolean("Auto Pass");
        }

        [Parser(Opcode.SMSG_LOOT_ROLL_WON)]
        public static void HandleLootWon(Packet packet)
        {
            packet.ReadGuid("GUID");
            packet.ReadUInt32("Slot");
            packet.ReadEntryWithName<UInt32>(StoreNameType.Item, "Entry");
            packet.ReadInt32("Random Property Id");
            packet.ReadInt32("Random Suffix");
            packet.ReadGuid("Player GUID");
            if (ClientVersion.AddedInVersion(ClientVersionBuild.V4_3_0_15005))
                packet.ReadInt32("Roll Number");
            else
                packet.ReadByte("Roll Number");
            packet.ReadEnum<LootRollType>("Roll Type", TypeCode.Byte);
        }

        //[Parser(Opcode.SMSG_LOOT_START_ROLL)]
        public static void HandleStartLoot(Packet packet)
        {
            packet.ReadGuid("GUID");
            packet.ReadEntryWithName<Int32>(StoreNameType.Map, "Map ID");
            if (ClientVersion.AddedInVersion(ClientVersionBuild.V3_3_3_11685)) // probably earlier
                packet.ReadUInt32("Slot");
            packet.ReadEntryWithName<UInt32>(StoreNameType.Item, "Entry");
            packet.ReadInt32("Random Suffix");
            packet.ReadInt32("Random Property Id");
            packet.ReadUInt32("Count");
            packet.ReadUInt32("Roll time");
            packet.ReadEnum<LootVoteFlags>("Roll Vote Mask", TypeCode.Byte);
            if (ClientVersion.AddedInVersion(ClientVersionBuild.V4_2_0_14333))
                packet.ReadByte("unk"); //amount of players? need verification.
        }

        [Parser(Opcode.SMSG_LOOT_SLOT_CHANGED)]
        public static void HandleLootSlotChanged(Packet packet)
        {
            packet.ReadGuid("GUID");
            packet.ReadByte("Slot");
            packet.ReadEntryWithName<UInt32>(StoreNameType.Item, "Entry");
            packet.ReadUInt32("Display ID");
            packet.ReadInt32("Unk UInt32 1");
            packet.ReadInt32("Unk UInt32 2"); // only seen 0
            packet.ReadUInt32("Count");
        }

        [Parser(Opcode.SMSG_LOOT_MASTER_LIST)]
        public static void HandleLootMasterList(Packet packet)
        {
            var count = packet.ReadByte("Count");
            for (var i = 0; i < count; i++)
                packet.ReadGuid("GUID", i);
        }

        [Parser(Opcode.SMSG_LOOT_CONTENTS)] //4.3.4
        public static void HandleLootContents(Packet packet)
        {
            var count1 = packet.ReadBits("Loot Items Count", 21);
            for (var i = 0; i < count1; i++)
            {
                packet.ReadUInt32("Display ID", i);
                packet.ReadInt32("Random Suffix Factor", i);
                packet.ReadInt32("Item Count", i);
                packet.ReadEntryWithName<UInt32>(StoreNameType.Item, "Item Entry", i);
                packet.ReadInt32("Unk Int32", i); // possibly random property id or looted count
            }
        }

        [Parser(Opcode.CMSG_LOOT_CURRENCY)]
        [Parser(Opcode.SMSG_CURRENCY_LOOT_REMOVED)]
        public static void HandleLootCurrency(Packet packet)
        {
            packet.ReadByte("Slot");
        }

        [Parser(Opcode.SMSG_LOOT_CLEAR_MONEY)]
        [Parser(Opcode.CMSG_LOOT_MONEY)]
        public static void HandleNullLoot(Packet packet)
        {
        }
    }
}
