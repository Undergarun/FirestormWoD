using System;
using WowPacketParser.Enums;
using WowPacketParser.Misc;
using WowPacketParser.Parsing;
using WowPacketParser.Store;
using WowPacketParser.Store.Objects;
using Guid = WowPacketParser.Misc.Guid;
using ChatMessageType530 = WowPacketParserModule.V5_3_0_16981.Enums.ChatMessageType;

namespace WowPacketParserModule.V5_3_0_16981.Parsers
{
    public static class ChatHandler
    {
        [Parser(Opcode.SMSG_MESSAGECHAT)]
        public static void HandleServerChatMessage(Packet packet)
        {
            var GroupGUID = new byte[8];
            var GuildGUID = new byte[8];
            var ReceiverGUID = new byte[8];
            var SenderGUID = new byte[8];

            var bit5269 = packet.ReadBit("bit5269"); // unk
            var hasText = !packet.ReadBit();
            System.Console.WriteLine("hasText: " + hasText);
            var bit5256 = !packet.ReadBit();
            System.Console.WriteLine("bit5256: " + bit5256);

            var hasSender = !packet.ReadBit();
            System.Console.WriteLine("hasSender: " + hasSender);
            var hasSenderGuid = packet.ReadBit("hasSenderGuid");
            packet.StartBitStream(SenderGUID, 2, 4, 0, 6, 1, 3, 5, 7);

            var hasGroupGuid = packet.ReadBit("hasGroupGuid");
            packet.StartBitStream(GroupGUID, 6, 0, 4, 1, 2, 3, 7, 5);

            var hasPrefix = !packet.ReadBit();
            System.Console.WriteLine("hasPrefix: " + hasPrefix);
            var bit5268 = packet.ReadBit("bit5268");
            var sendRealmId = !packet.ReadBit();
            System.Console.WriteLine("sendRealmId: " + sendRealmId);
            var hasBit5264 = !packet.ReadBit();
            System.Console.WriteLine("hasBit5264: " + hasBit5264);

            int senderName = 0;
            if (hasSender)
                senderName = (int)packet.ReadBits("SenderName", 11);

            var hasReceiverGuid = packet.ReadBit("hasReceiverGuid");
            packet.StartBitStream(ReceiverGUID, 4, 0, 6, 7, 5, 1, 3, 2);

            int prefixLen = 0;
            if (hasPrefix)
                prefixLen = (int)packet.ReadBits("prefixLength", 5);

            var hasReceiver = !packet.ReadBit();
            System.Console.WriteLine("hasReceiver: " + hasReceiver);
            var hasChatTag = !packet.ReadBit();
            System.Console.WriteLine("hasChatTag: " + hasChatTag);

            int textLen = 0;
            if (hasText)
                textLen = (int)packet.ReadBits("textLength", 12);

            var hasLang = !packet.ReadBit();
            System.Console.WriteLine("hasLang: " + hasLang);

            if (hasChatTag)
                packet.ReadEnum<ChatTag>("Chat Tag", 9);

            var hasGuildGuid = packet.ReadBit("hasGuildGuid");

            int receiverLen = 0;
            if (hasReceiver)
                receiverLen = (int)packet.ReadBits("receiverLength", 11);

            packet.StartBitStream(GuildGUID, 0, 2, 1, 4, 6, 7, 5, 3);

            var hasChannel = !packet.ReadBit();
            System.Console.WriteLine("hasChannel: " + hasChannel);
            int channelLen = 0;
            if (hasChannel)
                channelLen = (int)packet.ReadBits("ChannelLength", 7);

            if (hasChannel)
                packet.ReadWoWString("Channel Name", channelLen);

            if (hasSender)
                packet.ReadWoWString("Sender Name", senderName);

            packet.ParseBitStream(GroupGUID, 6, 7, 1, 2, 4, 3, 0, 5);
            packet.ParseBitStream(ReceiverGUID, 0, 4, 1, 3, 5, 7, 2, 6);

            packet.ReadEnum<ChatMessageType530>("Chat type", TypeCode.Byte);

            packet.ParseBitStream(SenderGUID, 7, 6, 5, 4, 0, 2, 1, 3);

            if (hasPrefix)
                packet.ReadWoWString("Addon Message Prefix", prefixLen);

            if (sendRealmId)
                packet.ReadUInt32("RealmID");

            packet.ParseBitStream(GuildGUID, 1, 0, 3, 7, 6, 5, 2, 4);

            if (hasReceiver)
                packet.ReadWoWString("Receiver Name", receiverLen);

            if (bit5256)
                packet.ReadUInt32("bit 5256 uint32 value");

            if (hasLang)
                packet.ReadEnum<Language>("Language", TypeCode.Byte);

            if (hasText)
                packet.ReadWoWString("Text", textLen);

            if (hasBit5264)
                packet.ReadInt32("bit5264 int32 value");
        }

        /*[Parser(Opcode.SMSG_MESSAGECHAT)]
        public static void HandleServerChatMessage(Packet packet)
        {
            var text = new CreatureText();
            var GroupGUID = new byte[8];
            var GuildGUID = new byte[8];
            var ReceiverGUID = new byte[8];
            var SenderGUID = new byte[8];

            var bit1316 = !packet.ReadBit();
            packet.ReadBit("bit5269");
            packet.ReadBit(); // fake bit
            var hasSender = !packet.ReadBit();
            packet.StartBitStream(GroupGUID, 6, 1, 7, 5, 4, 3, 2, 0);
            packet.ReadBit(); // fake bit
            var hasChannel = !packet.ReadBit();
            packet.StartBitStream(ReceiverGUID, 1, 5, 7, 4, 2, 0, 6, 3);
            packet.ReadBit("bit5268");
            var hasReceiver = !packet.ReadBit();

            int receiverLen = 0;
            if (hasReceiver)
                receiverLen = (int)packet.ReadBits(11);

            packet.ReadBit(); // fake bit
            packet.StartBitStream(GuildGUID, 0, 6, 1, 5, 7, 3, 4, 2);
            var hasText = !packet.ReadBit();
            var hasPrefix = !packet.ReadBit();

            int senderName = 0;
            if (hasSender)
                senderName = (int)packet.ReadBits(11);

            int textLen = 0;
            if (hasText)
                textLen = (int)packet.ReadBits(12);

            var hasConstTime = !packet.ReadBit();
            var hasAchi = !packet.ReadBit();
            packet.ReadBit(); // fake bit
            packet.StartBitStream(SenderGUID, 5, 4, 1, 0, 6, 2, 7, 3);

            int channelLen = 0;
            if (hasChannel)
                channelLen = (int)packet.ReadBits(7);

            var bit2630 = !packet.ReadBit();
            if (bit2630)
                packet.ReadEnum<ChatTag>("Chat Tag", 9);

            var hasLang = !packet.ReadBit();
            int prefixLen = 0;
            if (hasPrefix)
                prefixLen = (int)packet.ReadBits(5);

            if (hasPrefix)
                packet.ReadWoWString("Addon Message Prefix", prefixLen);

            packet.ParseBitStream(GuildGUID, 3, 1, 5, 4, 6, 2, 0, 7);
            packet.ParseBitStream(ReceiverGUID, 7, 4, 2, 3, 1, 5, 6, 0);
            packet.ParseBitStream(SenderGUID, 5, 0, 7, 4, 3, 2, 1, 6);
            packet.ParseBitStream(GroupGUID, 3, 5, 2, 6, 4, 0, 1, 7);

            packet.WriteGuid("SenderGUID", SenderGUID);
            packet.WriteGuid("ReceiverGUID", ReceiverGUID);
            packet.WriteGuid("GroupGUID", GroupGUID);
            packet.WriteGuid("GuildGUID", GuildGUID);

            uint entry = 0;
            var guid = new Guid(BitConverter.ToUInt64(SenderGUID, 0));
            if (guid.GetObjectType() == ObjectType.Unit)
                entry = guid.GetEntry();

            if (hasAchi)
                packet.ReadInt32("Achievement");

            if (hasReceiver)
                packet.ReadWoWString("Receiver Name", receiverLen);

            text.Type = (ChatMessageType)packet.ReadEnum<ChatMessageType530>("Chat type", TypeCode.Byte);
            if (hasText)
                text.Text = packet.ReadWoWString("Text", textLen);

            if (hasConstTime)
                packet.ReadInt32("Constant time");

            if (bit1316)
                packet.ReadSingle("float1316");

            if (hasChannel)
                packet.ReadWoWString("Channel Name", channelLen);

            if (hasSender)
            {
                if (entry != 0)
                    text.Comment = packet.ReadWoWString("Sender Name", senderName);
                else
                    packet.ReadWoWString("Sender Name", senderName);
            }

            if (hasLang)
                text.Language = packet.ReadEnum<Language>("Language", TypeCode.Byte);

            if (entry != 0)
                Storage.CreatureTexts.Add(entry, text, packet.TimeSpan);
        }*/

        [Parser(Opcode.CMSG_TEXT_EMOTE)]
        public static void HandleTextEmote(Packet packet)
        {
            var guid = new byte[8];

            packet.ReadEnum<EmoteTextType>("Text Emote ID", TypeCode.Int32);
            packet.ReadEnum<EmoteType>("Emote ID", TypeCode.Int32);

            packet.StartBitStream(guid, 3, 4, 5, 1, 6, 2, 0, 7);
            packet.ParseBitStream(guid, 6, 7, 4, 5, 2, 1, 3, 0);
            packet.WriteGuid("GUID", guid);
        }

        [Parser(Opcode.SMSG_TEXT_EMOTE)]
        public static void HandleTextEmoteServer(Packet packet)
        {
            var guid = new byte[8];
            var guid2 = new byte[8];

            guid2[0] = packet.ReadBit();
            guid[3] = packet.ReadBit();
            guid[4] = packet.ReadBit();
            guid2[6] = packet.ReadBit();
            guid2[7] = packet.ReadBit();
            guid2[3] = packet.ReadBit();
            guid[6] = packet.ReadBit();
            guid[7] = packet.ReadBit();
            guid2[5] = packet.ReadBit();
            guid2[2] = packet.ReadBit();
            guid2[1] = packet.ReadBit();
            guid[0] = packet.ReadBit();
            guid2[4] = packet.ReadBit();
            guid[1] = packet.ReadBit();
            guid[5] = packet.ReadBit();
            guid[2] = packet.ReadBit();

            packet.ReadXORByte(guid2, 4);
            packet.ReadXORByte(guid2, 5);
            packet.ReadXORByte(guid2, 1);
            packet.ReadXORByte(guid, 6);
            packet.ReadEnum<EmoteTextType>("Text Emote ID", TypeCode.Int32);
            packet.ReadXORByte(guid, 7);
            packet.ReadXORByte(guid, 1);
            packet.ReadXORByte(guid, 4);
            packet.ReadEnum<EmoteType>("Emote ID", TypeCode.Int32);
            packet.ReadXORByte(guid, 0);
            packet.ReadXORByte(guid2, 7);
            packet.ReadXORByte(guid2, 3);
            packet.ReadXORByte(guid, 2);
            packet.ReadXORByte(guid2, 6);
            packet.ReadXORByte(guid2, 2);
            packet.ReadXORByte(guid, 5);
            packet.ReadXORByte(guid2, 0);
            packet.ReadXORByte(guid, 3);

            packet.WriteGuid("GUID", guid);
            packet.WriteGuid("Guid 2", guid2);
        }

        [Parser(Opcode.CMSG_CHANNEL_LIST)]
        public static void HandleChannelList(Packet packet)
        {
            packet.ReadUInt32("Flags");
            var password = packet.ReadBits(7);
            packet.ReadBit();
            var length = packet.ReadBits(7);
            packet.ReadBit();

            packet.ReadWoWString("Password", password);
            packet.ReadWoWString("Channel Name", length);

        }

        [Parser(Opcode.CMSG_MESSAGECHAT_GUILD)]
        [Parser(Opcode.CMSG_MESSAGECHAT_INSTANCE)]
        [Parser(Opcode.CMSG_MESSAGECHAT_OFFICER)]
        [Parser(Opcode.CMSG_MESSAGECHAT_PARTY)]
        [Parser(Opcode.CMSG_MESSAGECHAT_RAID)]
        [Parser(Opcode.CMSG_MESSAGECHAT_RAID_WARNING)]
        [Parser(Opcode.CMSG_MESSAGECHAT_SAY)]
        [Parser(Opcode.CMSG_MESSAGECHAT_YELL)]
        public static void HandleClientChatMessage(Packet packet)
        {
            packet.ReadEnum<Language>("Language", TypeCode.Int32);
            var len = packet.ReadBits(8);
            packet.ReadWoWString("Message", len);
        }

        [Parser(Opcode.CMSG_MESSAGECHAT_AFK)]
        [Parser(Opcode.CMSG_MESSAGECHAT_DND)]
        [Parser(Opcode.CMSG_MESSAGECHAT_EMOTE)]
        public static void HandleClientChatMessage2(Packet packet)
        {
            var len = packet.ReadBits(8);
            packet.ReadWoWString("Message", len);
        }

        [Parser(Opcode.CMSG_MESSAGECHAT_CHANNEL)]
        public static void HandleClientChatMessageChannel(Packet packet)
        {
            packet.ReadEnum<Language>("Language", TypeCode.Int32);
            var msgLen = packet.ReadBits(8);
            var channelNameLen = packet.ReadBits(9);

            packet.ReadWoWString("Message", msgLen);
            packet.ReadWoWString("Channel Name", channelNameLen);
        }

        [Parser(Opcode.CMSG_MESSAGECHAT_WHISPER)]
        public static void HandleClientChatMessageWhisper(Packet packet)
        {
            packet.ReadEnum<Language>("Language", TypeCode.Int32);
            var recvName = packet.ReadBits(9);
            var msgLen = packet.ReadBits(8);

            packet.ReadWoWString("Message", msgLen);
            packet.ReadWoWString("Receivers Name", recvName);
        }

        [Parser(Opcode.SMSG_DEFENSE_MESSAGE)]
        public static void HandleDefenseMessage(Packet packet)
        {
            packet.ReadEntryWithName<Int32>(StoreNameType.Zone, "Zone Id");
            packet.ReadBits("Message Length?", 12);
            packet.ReadCString("Message");
        }
    }
}
