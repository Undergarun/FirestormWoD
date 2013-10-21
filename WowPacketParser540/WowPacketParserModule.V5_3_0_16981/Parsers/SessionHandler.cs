using System;
using System.Text;
using WowPacketParser.Enums;
using WowPacketParser.Misc;
using WowPacketParser.Parsing;
using Guid = WowPacketParser.Misc.Guid;
using CoreParsers = WowPacketParser.Parsing.Parsers;

namespace WowPacketParserModule.V5_3_0_16981.Parsers
{
    public static class SessionHandler
    {
        public static Guid LoginGuid;

        [Parser(Opcode.SMSG_AUTH_CHALLENGE)]
        public static void HandleServerAuthChallenge(Packet packet)
        {
            packet.ReadUInt32("Key pt1");
            packet.ReadUInt32("Key pt2");
            packet.ReadUInt32("Key pt3");
            packet.ReadUInt32("Key pt4");
            packet.ReadUInt32("Key pt5");
            packet.ReadUInt32("Key pt6");
            packet.ReadUInt32("Key pt7");
            packet.ReadUInt32("Key pt8");
            packet.ReadUInt32("Server Seed");
            packet.ReadByte("Unk Byte");
        }

        [Parser(Opcode.CMSG_AUTH_SESSION)]
        public static void HandleAuthSession(Packet packet)
        {
            var sha = new byte[20];
            packet.ReadUInt32("unk 1");
            sha[4] = packet.ReadByte();
            packet.ReadUInt32("unk 2");
            packet.ReadByte("unk Byte 1");
            sha[19] = packet.ReadByte();
            sha[12] = packet.ReadByte();
            sha[9] = packet.ReadByte();
            sha[6] = packet.ReadByte();
            sha[18] = packet.ReadByte();
            sha[17] = packet.ReadByte();
            sha[8] = packet.ReadByte();
            sha[13] = packet.ReadByte();
            sha[1] = packet.ReadByte();
            sha[10] = packet.ReadByte();
            sha[11] = packet.ReadByte();
            sha[15] = packet.ReadByte();
            packet.ReadUInt32("seed");
            sha[3] = packet.ReadByte();
            sha[14] = packet.ReadByte();
            sha[7] = packet.ReadByte();
            packet.ReadUInt64("unk 64");
            packet.ReadByte("unk Byte 2");
            packet.ReadUInt32("unk 3");
            sha[5] = packet.ReadByte();
            sha[0] = packet.ReadByte();
            packet.ReadUInt16("build client");
            sha[16] = packet.ReadByte();
            sha[2] = packet.ReadByte();
            packet.ReadUInt32("unk 4");
            using (var addons = new Packet(packet.ReadBytes(packet.ReadInt32("addon size")), packet.Opcode, packet.Time, packet.Direction, packet.Number, packet.Writer, packet.FileName))
            {
                var pkt2 = addons;
                CoreParsers.AddonHandler.ReadClientAddonsList(ref pkt2);
            }

            uint nameLenght = packet.ReadBits(12);
            packet.ReadBit();
            packet.ResetBitReader();
            packet.WriteLine("Account name: {0}", Encoding.UTF8.GetString(packet.ReadBytes((int)nameLenght)));
            packet.WriteLine("Proof SHA-1 Hash: " + Utilities.ByteArrayToHexString(sha));
        }

        /*[Parser(Opcode.CMSG_AUTH_SESSION)]
        public static void HandleAuthSession(Packet packet)
        {
            var sha = new byte[20];
            packet.ReadUInt32("UInt32 2");//16
            sha[8] = packet.ReadByte();//40
            sha[13] = packet.ReadByte();//45
            sha[3] = packet.ReadByte();//35
            packet.ReadUInt32("UInt32 3");//28
            sha[6] = packet.ReadByte();//38
            packet.ReadEnum<ClientVersionBuild>("Client Build", TypeCode.Int16);//20
            sha[2] = packet.ReadByte();//34
            sha[0] = packet.ReadByte();//32
            sha[7] = packet.ReadByte();//39
            sha[11] = packet.ReadByte();//43
            packet.ReadUInt32("UInt32 4");//56
            sha[5] = packet.ReadByte();//37
            sha[15] = packet.ReadByte();//47
            sha[14] = packet.ReadByte();//46
            sha[12] = packet.ReadByte();//44
            packet.ReadInt64("Int64");//64,68
            packet.ReadByte("Unk Byte");//61
            packet.ReadUInt32("Client seed");//52
            packet.ReadUInt32("UInt32 1");//24
            sha[1] = packet.ReadByte();//33
            sha[9] = packet.ReadByte();//41
            sha[4] = packet.ReadByte();//36
            sha[17] = packet.ReadByte();//49
            sha[16] = packet.ReadByte();//48
            sha[19] = packet.ReadByte();//51
            sha[18] = packet.ReadByte();//50
            sha[10] = packet.ReadByte();//42
            packet.ReadByte("Unk Byte");//60

            using (var addons = new Packet(packet.ReadBytes(packet.ReadInt32()), packet.Opcode, packet.Time, packet.Direction, packet.Number, packet.Writer, packet.FileName))
            {
                var pkt2 = addons;
                CoreParsers.AddonHandler.ReadClientAddonsList(ref pkt2);
            }

            packet.ReadBit("Unk bit");
            var size = (int)packet.ReadBits(11);
            packet.ResetBitReader();
            packet.WriteLine("Account name: {0}", Encoding.UTF8.GetString(packet.ReadBytes(size)));
            packet.WriteLine("Proof SHA-1 Hash: " + Utilities.ByteArrayToHexString(sha));
        }*/

        [Parser(Opcode.SMSG_AUTH_RESPONSE)]
        public static void HandleAuthResponse540(Packet packet)
        {
            uint classCounter = 0;
            uint raceCounter = 0;
            var has112bit = false;
            uint unkCounter1 = 0;
            var hasBit116 = false;
            uint unkCounter5 = 0;
            uint unkStringLenght1 = 0;
            uint unkStringLenght2 = 0;

            var isQueued = packet.ReadBit();
            var hasAccountData = packet.ReadBit();

            if (hasAccountData)
            {
                unkCounter1 = packet.ReadBits(21);
                var has108bit = packet.ReadBit();
                classCounter = packet.ReadBits(23); // class count ?
                var has54bit = packet.ReadBit();
                has112bit = packet.ReadBit();

                for (int i = 0; i < unkCounter1; i++)
                {
                    var unkCounter3 = packet.ReadBits(23);
                    var unkNb = packet.ReadBits(7);
                    var unkCounter4 = packet.ReadBits(22);
                }

                unkCounter5 = packet.ReadBits(21);
                for (int i = 0; i < unkCounter5; i++)
                {
                    unkStringLenght1 = packet.ReadBits("unk", 8);
                    var unk2 = packet.ReadBit("unk 2");
                    unkStringLenght2 = packet.ReadBits("unk 3", 8);
                }

                hasBit116 = packet.ReadBit();
                raceCounter = packet.ReadBits(23); // race counter
            }

            if (isQueued)
            {
                var hasBit128 = packet.ReadBit(); // ??? need to be 0
                if (isQueued) // blizz ...
                    packet.ReadUInt32("position in queue");
            }

            if (hasAccountData)
            {
                for (int i = 0; i < classCounter; i++)
                {
                    packet.ReadEnum<Class>("Class", TypeCode.Byte, i);
                    packet.ReadEnum<ClientType>("Class Expansion", TypeCode.Byte, i);
                }

                packet.ReadUInt32("unk uint32 48");

                for (int i = 0; i < raceCounter; i++)
                {
                    packet.ReadByte("is In extension");
                    packet.ReadEnum<Race>("Race", TypeCode.Byte, i);
                }

                packet.ReadUInt32("unk 40"); // time left in mins
                packet.ReadUInt32("unk 56"); // unk (0)

                if (has112bit)
                    packet.ReadUInt16("unk 110");

                // fuck it
                /*for (int i = 0; i < unkCounter1; i++)
                {
                }*/

                packet.ReadByte("unk 53"); // exp player
                packet.ReadByte("unk 52"); // exp account

                if (hasBit116)
                    packet.ReadUInt16("unk 114");

                for (int i = 0; i < unkCounter5; i++)
                {
                    packet.ReadWoWString("string2", (int)unkStringLenght2); // realm name
                    packet.ReadWoWString("string1", (int)unkStringLenght1); // realm name
                    packet.ReadUInt32("unk 28 uint32"); // realm guid flags ??
                }

                packet.ReadUInt32("unk 20");
                packet.ReadUInt32("unk 44"); // 0
            }

            packet.ReadByte("byte 16"); // auth code
        }

       // [Parser(Opcode.SMSG_AUTH_RESPONSE)]
        public static void HandleAuthResponse(Packet packet)
        {
            var count = 0u;
            var count1 = 0u;
            var isQueued = packet.ReadBit("Is In Queue");

            if (isQueued)
                packet.ReadBit("unk0");

            var hasAccountData = packet.ReadBit("Has Account Data");

            if (hasAccountData)
            {
                packet.ReadBit("Unk 1");
                packet.ReadBit("Unk 2");
                count1 = packet.ReadBits("Race Activation Count", 23);
                packet.ReadBit("Unk 3");
                packet.ReadBits("Unkbits", 21);
                count = packet.ReadBits("Class Activation Count", 23);
                packet.ReadBits("Unkbits", 22);
                packet.ReadBit("Unk 4");

            }
            packet.ResetBitReader();

            if (hasAccountData)
            {
                packet.ReadByte("unk");
                for (var i = 0; i < count; ++i)
                {
                    packet.ReadEnum<ClientType>("Class Expansion", TypeCode.Byte, i);
                    packet.ReadEnum<Class>("Class", TypeCode.Byte, i);
                }

                packet.ReadInt16("UnkInt16 1");
                packet.ReadInt16("UnkInt16 2");

                for (var i = 0; i < count1; ++i)
                {
                    packet.ReadEnum<ClientType>("Race Expansion", TypeCode.Byte, i);
                    packet.ReadEnum<Race>("Race", TypeCode.Byte, i);
                }

                packet.ReadUInt32("Unk 8");
                packet.ReadUInt32("Unk 9");
                packet.ReadUInt32("Unk 10");


                packet.ReadEnum<ClientType>("Account Expansion", TypeCode.Byte);
                packet.ReadEnum<ClientType>("Player Expansion", TypeCode.Byte);

            }

            packet.ReadEnum<ResponseCode>("Auth Code", TypeCode.Byte);

            if (isQueued)
                packet.ReadUInt32("Unk 11");
        }

        [Parser(Opcode.SMSG_MOTD)]
        public static void HandleMessageOfTheDay(Packet packet)
        {
            var lineCount = packet.ReadBits("Line Count", 4);
            var lineLength = new int[lineCount];
            for (var i = 0; i < lineCount; i++)
                lineLength[i] = (int)packet.ReadBits("lineLength x", 7);

            for (var i = 0; i < lineCount; i++)
                packet.ReadWoWString("Line", lineLength[i], i);
        }

        [Parser(Opcode.CMSG_PLAYER_LOGIN)]
        public static void HandlePlayerLogin(Packet packet)
        {
            packet.ReadSingle("Unk Float");
            var guid = packet.StartBitStream(3, 4, 0, 6, 7, 1, 2, 5);
            packet.ParseBitStream(guid, 0, 3, 7, 6, 1, 2, 4, 5);
            LoginGuid = new Guid(BitConverter.ToUInt64(guid, 0));
            packet.WriteGuid("Guid", guid);
        }

        [Parser(Opcode.SMSG_LOGOUT_COMPLETE)]
        public static void HandleLogoutComplete(Packet packet)
        {
            LoginGuid = new Guid(0);
        }
    }
}
