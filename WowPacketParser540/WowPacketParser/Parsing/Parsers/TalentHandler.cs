using System;
using WowPacketParser.Misc;
using WowPacketParser.Enums;
using WowPacketParser.Enums.Version;

namespace WowPacketParser.Parsing.Parsers
{
    public static class TalentHandler
    {
        public static void ReadTalentInfo(ref Packet packet)
        {
            packet.ReadUInt32("Free Talent count");
            var speccount = packet.ReadByte("Spec count");
            packet.ReadByte("Active Spec");
            for (var i = 0; i < speccount; ++i)
            {
                if (ClientVersion.AddedInVersion(ClientVersionBuild.V4_0_6a_13623))
                    packet.ReadUInt32("TalentBranchSpec", i);
                var count2 = packet.ReadByte("Spec Talent Count", i);
                for (var j = 0; j < count2; ++j)
                {
                    packet.ReadUInt32("Talent Id", i, j);
                    packet.ReadByte("Rank", i, j);
                }

                var glyphs = packet.ReadByte("Glyph count");
                for (var j = 0; j < glyphs; ++j)
                    packet.ReadUInt16("Glyph", i, j);
            }
        }

        public static void ReadInspectPart(ref Packet packet)
        {
            var slotMask = packet.ReadUInt32("Slot Mask");
            var slot = 0;
            while (slotMask > 0)
            {
                if ((slotMask & 0x1) > 0)
                {
                    var name = "[" + (EquipmentSlotType)slot + "] ";
                    packet.ReadEntryWithName<UInt32>(StoreNameType.Item, name + "Item Entry");
                    var enchantMask = packet.ReadUInt16();
                    if (enchantMask > 0)
                    {
                        var enchantName = name + "Item Enchantments: ";
                        while (enchantMask > 0)
                        {
                            if ((enchantMask & 0x1) > 0)
                            {
                                enchantName += packet.ReadUInt16();
                                if (enchantMask > 1)
                                    enchantName += ", ";
                            }
                            enchantMask >>= 1;
                        }
                        packet.WriteLine(enchantName);
                    }
                    packet.ReadUInt16(name + "Unk Uint16");
                    packet.ReadPackedGuid(name + "Creator GUID");
                    packet.ReadUInt32(name + "Unk Uint32");
                }
                ++slot;
                slotMask >>= 1;
            }
        }

        [Parser(Opcode.SMSG_TALENTS_INVOLUNTARILY_RESET)]
        public static void HandleTalentsInvoluntarilyReset(Packet packet)
        {
            packet.ReadByte("Unk Byte");
        }

        [Parser(Opcode.SMSG_INSPECT_TALENT)]
        public static void HandleInspectTalent(Packet packet)
        {
            var playerGuid = new byte[8];
            var guildGuid = new byte[8];

            playerGuid[7] = packet.ReadBit();
            playerGuid[3] = packet.ReadBit();

            var hasGuild = packet.ReadBit() != 0;
            Console.WriteLine("hasGuild :" + hasGuild);

            if (hasGuild)
            {
                guildGuid[6] = packet.ReadBit();
                guildGuid[7] = packet.ReadBit();
                guildGuid[4] = packet.ReadBit();
                guildGuid[5] = packet.ReadBit();
                guildGuid[2] = packet.ReadBit();
                guildGuid[3] = packet.ReadBit();
                guildGuid[1] = packet.ReadBit();
                guildGuid[0] = packet.ReadBit();
            }

            var talentCount = packet.ReadBits("talentCount", 23);
            var glyphCount = packet.ReadBits("glyphCount", 23);

            playerGuid[5] = packet.ReadBit();
            playerGuid[2] = packet.ReadBit();
            playerGuid[6] = packet.ReadBit();

            var itemCount = packet.ReadBits("itemCount", 20);
            var enchantmentCount = new uint[itemCount];

            var unkBit = new bool[itemCount];
            var unkBit2 = new bool[itemCount];
            var itemCreator = new byte[itemCount][];
            // EQUIPMENT_SLOT_END
            for (int i = 0; i < itemCount; i++)
            {
                itemCreator[i] = new byte[8];

                itemCreator[i][0] = packet.ReadBit();

                unkBit[i] = packet.ReadBit();
                Console.WriteLine("unkBit: " + unkBit[i]);

                itemCreator[i][6] = packet.ReadBit();
                itemCreator[i][7] = packet.ReadBit();

                unkBit2[i] = packet.ReadBit();
                Console.WriteLine("unkBit2: " + unkBit2[i]);

                itemCreator[i][3] = packet.ReadBit();
                itemCreator[i][2] = packet.ReadBit();
                itemCreator[i][1] = packet.ReadBit();

                enchantmentCount[i] = packet.ReadBits("enchantmentCount", 21, i);

                var unkBit3 = packet.ReadBit() != 0;
                Console.WriteLine("unkBit3: " + unkBit3);

                itemCreator[i][5] = packet.ReadBit();
                itemCreator[i][4] = packet.ReadBit();
            }

            playerGuid[4] = packet.ReadBit();
            playerGuid[1] = packet.ReadBit();
            playerGuid[0] = packet.ReadBit();

            for (int i = 0; i < itemCount; i++)
            {
                if (unkBit2[i])
                    packet.ReadUInt16("Unk UInt16", i);

                packet.ReadXORByte(itemCreator[i], 3);
                packet.ReadXORByte(itemCreator[i], 4);

                // MAX_ENCHANTMENT_SLOT
                for (int j = 0; j < enchantmentCount[i]; j++)
                {
                    packet.ReadByte("Unk byte", i, j);
                    packet.ReadUInt32("Enchant ID", i, j);
                }

                if (unkBit[i])
                    packet.ReadUInt32("Unk uint32", i);

                packet.ReadXORByte(itemCreator[i], 0);
                packet.ReadXORByte(itemCreator[i], 2);
                packet.ReadXORByte(itemCreator[i], 6);

                var byteCounter = packet.ReadUInt32("Unk uint32", i);
                if (byteCounter > 0)
                    packet.ReadBytes((int)byteCounter);

                packet.ReadXORByte(itemCreator[i], 1);
                packet.ReadXORByte(itemCreator[i], 7);
                packet.ReadXORByte(itemCreator[i], 5);

                packet.ReadByte("iterator", i); // i ?
                packet.ReadUInt32("Item ID", i);

                packet.WriteGuid("Creator GUID", itemCreator[i], i);
            }

            packet.ReadXORByte(playerGuid, 7);
            packet.ReadXORByte(playerGuid, 2);
            packet.ReadXORByte(playerGuid, 5);
            packet.ReadXORByte(playerGuid, 0);

            packet.ReadUInt32("Specialization ID");

            if (hasGuild)
            {
                packet.ReadUInt32("Level");

                packet.ReadXORByte(guildGuid, 1);
                packet.ReadXORByte(guildGuid, 3);

                packet.ReadUInt32("Member count");

                packet.ReadXORByte(guildGuid, 6);
                packet.ReadXORByte(guildGuid, 2);
                packet.ReadXORByte(guildGuid, 5);
                packet.ReadXORByte(guildGuid, 4);
                packet.ReadXORByte(guildGuid, 0);
                packet.ReadXORByte(guildGuid, 7);

                packet.ReadUInt64("Experience");

                packet.WriteGuid("Guild GUID", guildGuid);
            }

            packet.ReadXORByte(playerGuid, 6);
            packet.ReadXORByte(playerGuid, 4);
            packet.ReadXORByte(playerGuid, 2);
            packet.ReadXORByte(playerGuid, 3);

            packet.WriteGuid("Player GUID", playerGuid);

            for (int i = 0; i < talentCount; i++)
                packet.ReadUInt16("talent ID");

            for (int i = 0; i < glyphCount; i++)
                packet.ReadUInt16("glyph ID");
        }

        [Parser(Opcode.SMSG_INSPECT_RESULTS_UPDATE)]
        public static void HandleInspectResultsUpdate(Packet packet)
        {
            if (ClientVersion.AddedInVersion(ClientVersionBuild.V4_3_4_15595)) // confirmed for 4.3.4
                packet.ReadPackedGuid("GUID");
            else if (ClientVersion.AddedInVersion(ClientVersionBuild.V4_0_6a_13623))
                packet.ReadGuid("GUID");
            else
                packet.ReadPackedGuid("GUID");

            ReadInspectPart(ref packet);
        }

        [Parser(Opcode.MSG_TALENT_WIPE_CONFIRM)]
        public static void HandleTalent(Packet packet)
        {
            packet.ReadGuid("GUID");
            if (packet.Direction == Direction.ServerToClient)
                packet.ReadUInt32("Gold");
        }

        [Parser(Opcode.MSG_RESPEC_WIPE_CONFIRM)]
        public static void HandleRespecWipeConfirm(Packet packet)
        {
            packet.ReadByte("Spec Group");
            var guid = packet.StartBitStream(5, 3, 2, 7, 0, 6, 1, 4);
            packet.ParseBitStream(guid, 0, 1, 2, 3, 5, 6, 7, 4);
            packet.WriteGuid("GUID", guid);
        }

        [Parser(Opcode.SMSG_TALENTS_INFO, ClientVersionBuild.Zero, ClientVersionBuild.V5_1_0_16309)]
        public static void HandleTalentsInfo(Packet packet)
        {
            var pet = packet.ReadBoolean("Pet Talents");
            if (pet)
            {
                packet.ReadUInt32("Unspent Talent");
                var count = packet.ReadByte("Talent Count");
                for (var i = 0; i < count; ++i)
                {
                    packet.ReadUInt32("Talent ID", i);
                    packet.ReadByte("Rank", i);
                }
            }
            else
                ReadTalentInfo(ref packet);
        }

        [Parser(Opcode.SMSG_TALENTS_INFO, ClientVersionBuild.V5_1_0_16309)]
        public static void ReadTalentInfo510(Packet packet)
        {
            var specCount = packet.ReadByte("Spec Group count");
            packet.ReadByte("Active Spec Group");

            for (var i = 0; i < specCount; ++i)
            {
                packet.ReadUInt32("Spec Id", i);

                var spentTalents = packet.ReadByte("Spec Talent Count", i);
                for (var j = 0; j < spentTalents; ++j)
                    packet.ReadUInt16("Talent Id", i, j);

                var glyphCount = packet.ReadByte("Glyph count", i);
                for (var j = 0; j < glyphCount; ++j)
                    packet.ReadUInt16("Glyph", i, j);
            }
        }

        [Parser(Opcode.CMSG_LEARN_PREVIEW_TALENTS, ClientVersionBuild.Zero, ClientVersionBuild.V4_3_4_15595)]
        [Parser(Opcode.CMSG_LEARN_PREVIEW_TALENTS_PET, ClientVersionBuild.Zero, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleTalentPreviewTalents(Packet packet)
        {
            if (packet.Opcode == Opcodes.GetOpcode(Opcode.CMSG_LEARN_PREVIEW_TALENTS_PET))
                packet.ReadGuid("GUID");

            var count = packet.ReadUInt32("Talent Count");
            for (var i = 0; i < count; ++i)
            {
                packet.ReadUInt32("Talent ID", i);
                packet.ReadUInt32("Rank", i);
            }
        }

        [Parser(Opcode.CMSG_LEARN_PREVIEW_TALENTS, ClientVersionBuild.V4_3_4_15595)]
        [Parser(Opcode.CMSG_LEARN_PREVIEW_TALENTS_PET, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleTalentPreviewTalents434(Packet packet)
        {
            if (packet.Opcode == Opcodes.GetOpcode(Opcode.CMSG_LEARN_PREVIEW_TALENTS_PET))
                packet.ReadGuid("GUID");
            else
                packet.ReadUInt32("Tab Page");

            var count = packet.ReadUInt32("Talent Count");
            for (var i = 0; i < count; ++i)
            {
                packet.ReadUInt32("Talent ID", i);
                packet.ReadUInt32("Rank", i);
            }
        }

        [Parser(Opcode.CMSG_LEARN_TALENT)]
        public static void HandleLearnTalent(Packet packet)
        {
            packet.ReadUInt32("Talent ID");
            packet.ReadUInt32("Rank");
        }

        [Parser(Opcode.CMSG_LEARN_TALENTS)] // 5.4.0
        public static void HandleLearnTalents(Packet packet)
        {
            var talentCount = packet.ReadBits("Learned Talents Count", 23);

            packet.ResetBitReader();

            for (int i = 0; i < talentCount; i++)
                packet.ReadUInt16("Talent Id", i);
        }

        [Parser(Opcode.SMSG_TALENTS_ERROR)]
        public static void HandleTalentError(Packet packet)
        {
            packet.ReadEnum<TalentError>("Talent Error", TypeCode.Int32);
        }

        [Parser(Opcode.CMSG_SET_SPECIALIZATION)]
        public static void HandleSetSpec(Packet packet)
        {
            packet.ReadInt32("Spec Group Id");
        }

        //[Parser(Opcode.CMSG_UNLEARN_TALENTS)]

        //[Parser(Opcode.CMSG_PET_LEARN_TALENT)]
        //[Parser(Opcode.CMSG_PET_UNLEARN_TALENTS)]
        //[Parser(Opcode.CMSG_SET_ACTIVE_TALENT_GROUP_OBSOLETE)]
        //[Parser(Opcode.CMSG_SET_PRIMARY_TALENT_TREE)] 4.0.6a
    }
}
