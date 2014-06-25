using System;
using WowPacketParser.Enums;
using WowPacketParser.Misc;

namespace WowPacketParser.Parsing.Parsers
{
    public static class MailHandler
    {
        [Parser(Opcode.SMSG_RECEIVED_MAIL)]
        public static void HandleReceivedMail(Packet packet)
        {
            packet.ReadSingle("Time left"); // Sup with timers in float?
        }

        [Parser(Opcode.SMSG_SHOW_MAILBOX)]
        [Parser(Opcode.CMSG_GET_MAIL_LIST)]
        public static void HandleShowMailbox(Packet packet)
        {
            var mailGuid = new byte[8];

            mailGuid = packet.StartBitStream(2, 3, 7, 6, 0, 5, 1, 4);
            packet.ResetBitReader();
            packet.ParseBitStream(mailGuid, 2, 1, 3, 6, 4, 5, 7, 0);

            packet.WriteGuid("MailBox GUID", mailGuid);
        }

        [Parser(Opcode.CMSG_MAIL_TAKE_MONEY)]
        [Parser(Opcode.CMSG_MAIL_MARK_AS_READ, ClientVersionBuild.Zero, ClientVersionBuild.V4_3_4_15595)]
        [Parser(Opcode.CMSG_MAIL_CREATE_TEXT_ITEM, ClientVersionBuild.Zero, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleTakeMoney(Packet packet)
        {
            packet.ReadGuid("Mailbox GUID");
            packet.ReadUInt32("Mail Id");
            if (ClientVersion.AddedInVersion(ClientVersionBuild.V4_0_6_13596)) // need correct version
                packet.ReadUInt64("Money");
        }

        [Parser(Opcode.CMSG_MAIL_MARK_AS_READ, ClientVersionBuild.V4_3_4_15595)]
        [Parser(Opcode.CMSG_MAIL_CREATE_TEXT_ITEM, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleMarkMail(Packet packet)
        {
            packet.ReadGuid("Mailbox GUID");
            packet.ReadUInt32("Mail Id");
        }

        [Parser(Opcode.CMSG_MAIL_DELETE)]
        public static void HandleMailDelete(Packet packet)
        {
            packet.ReadGuid("Mailbox GUID");
            packet.ReadUInt32("Mail Id");
            packet.ReadUInt32("Template Id");
        }

        [Parser(Opcode.CMSG_MAIL_RETURN_TO_SENDER)]
        public static void HandleMailReturnToSender(Packet packet)
        {
            packet.ReadGuid("Mailbox GUID");
            packet.ReadUInt32("Mail Id");
            packet.ReadGuid("Sender GUID");
        }

        [Parser(Opcode.SMSG_MAIL_LIST_RESULT)]
        public static void HandleMailListResult(Packet packet)
        {
            var mailCount = packet.ReadBits("mailCount", 18);

            var itemCount = new uint[mailCount];
            var hasSenderGuids = new bool[mailCount];
            var senderGuids = new byte[mailCount][];
            var hasSenderEntry = new bool[mailCount];
            var bodyLengths = new uint[mailCount];
            var subjectLengths = new uint[mailCount];

            for (int i = 0; i < mailCount; ++i)
            {
                itemCount[i] = packet.ReadBits("messageSize", 17, i);
                hasSenderGuids[i] = packet.ReadBit("hasSenderGuid", i);

                if (hasSenderGuids[i])
                    senderGuids[i] = packet.StartBitStream(3, 6, 4, 2, 5, 1, 0, 7);

                hasSenderEntry[i] = packet.ReadBit("hasSenderEntry", i);
                bodyLengths[i] = packet.ReadBits("bodyLength", 13, i);
                subjectLengths[i] = packet.ReadBits("subjectLengths", 8, i);

                for (int j = 0; j < itemCount[i]; ++j)
                    packet.ReadBit("Unk Bit", i, j);
            }

            for (int i = 0; i < mailCount; ++i)
            {
                for (int j = 0; j < itemCount[i]; ++j)
                {
                    packet.ReadUInt32("Item Suffix Factor", i, j);
                    packet.ReadUInt32("Item Count", i, j);

                    // MAX_INSPECTED_ENCHANTMENT_SLOT
                    for (int a = 0; a < 8; ++a)
                    {
                        packet.ReadUInt32("Enchant ID", i, j, a);
                        packet.ReadUInt32("Enchant Duration", i, j, a);
                        packet.ReadUInt32("Enchant Charges", i, j, a);
                    }

                    packet.ReadUInt32("Item Max Durability", i, j);
                    packet.ReadByte("Item Slot", i, j);
                    packet.ReadUInt32("Item Durability", i, j);
                    packet.ReadUInt32("Item Low GUID", i, j);
                    packet.ReadUInt32("Item ID", i, j);
                    packet.ReadUInt32("Item Random Property ID", i, j);

                    var bytesCounter = packet.ReadUInt32("Bytes counter", i, j);
                    for (int a = 0; a < bytesCounter; ++a)
                        packet.ReadByte("Unk Byte counter value", i, j, a);

                    packet.ReadUInt32("Spell Charges", i, j);
                }

                packet.ReadUInt32("Mail template ID", i);

                if (hasSenderGuids[i])
                {
                    packet.ParseBitStream(senderGuids[i], 2, 0, 4, 5, 3, 6, 1, 7);
                    packet.WriteGuid("Sender GUID", senderGuids[i], i);
                }

                packet.ReadWoWString("Subject", subjectLengths[i], i);
                packet.ReadUInt32("Package", i); // Package.dbc ID
                packet.ReadSingle("Time Left", i);
                packet.ReadUInt64("COD", i);
                packet.ReadEnum<MailFlags>("Flags", TypeCode.UInt32, i);
                packet.ReadUInt64("Gold", i);
                packet.ReadUInt32("Message ID", i);
                packet.ReadWoWString("Body", bodyLengths[i], i);

                if (hasSenderEntry[i])
                    packet.ReadUInt32("Sender Entry", i);

                packet.ReadEnum<MailType>("Message Type", TypeCode.Byte, i);
                packet.ReadUInt32("Stationery", i); // Stationary.dbc ID
            }

            packet.ReadUInt32("Shown count");
        }

        [Parser(Opcode.MSG_QUERY_NEXT_MAIL_TIME)]
        public static void HandleNullMail(Packet packet)
        {
            if (packet.Direction == Direction.ClientToServer)
                return;

            // Math.Abs(this float) > 0.0f returns whether the player has received a new mail since last visiting a mailbox
            packet.ReadSingle("Time since last time visiting a mailbox (can be < 0.0)");

            var count = packet.ReadUInt32("Count");
            for (var i = 0; i < count; ++i)
            {
                packet.ReadUInt64("GUID", i);
                packet.ReadUInt32("Sender Id", i);
                packet.ReadUInt32("Message type", i);
                packet.ReadUInt32("Stationery", i);
                packet.ReadSingle("Time?", i);
            }
        }

        [Parser(Opcode.SMSG_SEND_MAIL_RESULT)]
        public static void HandleSendMailResult(Packet packet)
        {
            packet.ReadEnum<MailActionType>("Mail Action", TypeCode.UInt32);
            packet.ReadUInt32("Equip Error");
            packet.ReadEnum<MailErrorType>("Mail Error", TypeCode.UInt32);
            packet.ReadUInt32("Item Guid Low");
            packet.ReadUInt32("Item count");
            packet.ReadUInt32("Mail Id");
        }

        [Parser(Opcode.CMSG_SEND_MAIL, ClientVersionBuild.Zero, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleSendMail(Packet packet)
        {
            packet.ReadGuid("Mailbox GUID");
            packet.ReadCString("Receiver");
            packet.ReadCString("Subject");
            packet.ReadCString("Body");
            packet.ReadUInt32("Stationery?");
            packet.ReadUInt32("Unk Uint32");
            var items = packet.ReadByte("Item Count");
            for (var i = 0; i < items; ++i)
            {
                packet.ReadByte("Slot", i);
                packet.ReadGuid("Item GUID", i);
            }
            packet.ReadUInt32("Money");
            packet.ReadUInt32("COD");
            packet.ReadUInt64("Unk Uint64");
            packet.ReadByte("Unk Byte");

        }

        [Parser(Opcode.CMSG_SEND_MAIL, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleSendMail434(Packet packet)
        {
            var guid = new byte[8];
            packet.ReadInt32("Unk Int32"); // MailMessage.packageId ?
            packet.ReadInt32("Stationery?");
            packet.ReadInt64("COD");
            packet.ReadInt64("Money");
            var len2 = packet.ReadBits(12);
            var len1 = packet.ReadBits(9);
            var count = packet.ReadBits("Item Count", 5);
            guid[0] = packet.ReadBit();
            var guid2 = new byte[count][];
            for (var i = 0; i < count; i++)
            {
                guid2[i] = packet.StartBitStream(2, 6, 3, 7, 1, 0, 4, 5);
            }
            guid[3] = packet.ReadBit();
            guid[4] = packet.ReadBit();
            var len3 = packet.ReadBits(7);
            guid[2] = packet.ReadBit();
            guid[6] = packet.ReadBit();
            guid[1] = packet.ReadBit();
            guid[7] = packet.ReadBit();
            guid[5] = packet.ReadBit();

            packet.ReadXORByte(guid, 4);

            for (var i = 0; i < count; i++)
            {
                if (guid2[i][6] != 0) guid2[i][6] = packet.ReadByte();
                if (guid2[i][1] != 0) guid2[i][1] = packet.ReadByte();
                if (guid2[i][7] != 0) guid2[i][7] = packet.ReadByte();
                if (guid2[i][2] != 0) guid2[i][2] = packet.ReadByte();
                packet.ReadByte("Slot", i);
                if (guid2[i][3] != 0) guid2[i][3] = packet.ReadByte();
                if (guid2[i][0] != 0) guid2[i][0] = packet.ReadByte();
                if (guid2[i][4] != 0) guid2[i][4] = packet.ReadByte();
                if (guid2[i][5] != 0) guid2[i][5] = packet.ReadByte();
                packet.WriteGuid("Item Guid", guid2[i], i);
            }

            packet.ReadXORByte(guid, 7);
            packet.ReadXORByte(guid, 3);
            packet.ReadXORByte(guid, 6);
            packet.ReadXORByte(guid, 5);

            packet.ReadWoWString("Subject", len1);
            packet.ReadWoWString("Receiver", len3);

            packet.ReadXORByte(guid, 2);
            packet.ReadXORByte(guid, 0);

            packet.ReadWoWString("Body", len2);

            packet.ReadXORByte(guid, 1);

            packet.WriteGuid("Mailbox Guid", guid);
        }

        [Parser(Opcode.CMSG_MAIL_TAKE_ITEM)]
        public static void HandleMailTakeItem(Packet packet)
        {
            var guid = new byte[8];

            packet.ReadUInt32("Item Low GUID");
            packet.ReadUInt32("Mail Id");

            packet.StartBitStream(guid, 7, 1, 0, 6, 5, 4, 2, 3);
            packet.ResetBitReader();
            packet.ParseBitStream(guid, 1, 0, 7, 6, 3, 5, 2, 4);

            packet.WriteGuid("Guid", guid);
        }

        //CMSG_MAELSTROM_GM_SENT_MAIL
    }
}
