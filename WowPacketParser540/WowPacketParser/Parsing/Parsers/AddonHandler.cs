using WowPacketParser.Enums;
using WowPacketParser.Misc;

namespace WowPacketParser.Parsing.Parsers
{
    public static class AddonHandler
    {
        private static int _addonCount = -1;

        public static void ReadClientAddonsList(ref Packet packet)
        {
            var decompCount = packet.ReadInt32();
            packet = packet.Inflate(decompCount, false);

            if (ClientVersion.AddedInVersion(ClientVersionBuild.V3_0_8_9464))
            {
                var count = packet.ReadInt32("Addons Count");
                _addonCount = count;

                for (var i = 0; i < count; i++)
                {
                    packet.ReadCString("Name", i);
                    packet.ReadBoolean("Uses public key", i);
                    packet.ReadInt32("Public key CRC", i);
                    packet.ReadInt32("URL file CRC", i);
                }

                packet.ReadTime("Time");
            }
            else
            {
                int count = 0;

                while (packet.Position != packet.Length)
                {
                    packet.ReadCString("Name");
                    packet.ReadBoolean("Enabled");
                    packet.ReadInt32("CRC");
                    packet.ReadInt32("Unk Int32");

                    count++;
                }

                _addonCount = count;
            }
        }

        [Parser(Opcode.SMSG_ADDON_INFO)]
        public static void HandleServerAddonsList(Packet packet)
        {
            if (ClientVersion.AddedInVersion(ClientVersionBuild.V5_3_0_17128))
            {
                var addonCount = packet.ReadBits("Addons Size", 23);
                var usePublicKey = new bool[addonCount];
                var sendCrc = new bool[addonCount];

                for (var i = 0; i < addonCount; i++)
                {
                    var hasString = packet.ReadBit("hasString", i);

                    usePublicKey[i] = packet.ReadBit("Use Public Key", i);

                    if (hasString)
                        packet.ReadBits("unk 2 - string size", 8);

                    sendCrc[i] = packet.ReadBit("Use CRC", i);
                }

                packet.ReadBits("Unk bit counter", 18);

                for (var i = 0; i < addonCount; i++)
                {
                    var crcpub = sendCrc[i];

                    var unk3 = false;
                    if (unk3)
                        packet.ReadWoWString(0);

                    if (usePublicKey[i])
                    {
                        var pubKey = packet.ReadBytes(256);
                        packet.WriteLine("[{0}] Name MD5: {1}", i, Utilities.ByteArrayToHexString(pubKey));
                    }

                    if (crcpub)
                    {
                        packet.ReadByte("unk byte");
                        packet.ReadInt32("Unk Int32", i);
                    }

                    packet.ReadByte("State");
                }
            }

            // This packet requires _addonCount from CMSG_AUTH_SESSION to be parsed.
            if (_addonCount == -1)
            {
                packet.WriteLine("CMSG_AUTH_SESSION was not received - cannot successfully parse this packet.");
                packet.ReadToEnd();
                return;
            }

            if (ClientVersion.AddedInVersion(ClientVersionBuild.V3_0_8_9464))
            {
                var bannedCount = packet.ReadInt32("Banned Addons Count");

                for (var i = 0; i < bannedCount; i++)
                {
                    packet.ReadInt32("ID", i);

                    var unkStr2 = packet.ReadBytes(16);
                    packet.WriteLine("[{0}] Name MD5: {1}", i, Utilities.ByteArrayToHexString(unkStr2));

                    var unkStr3 = packet.ReadBytes(16);
                    packet.WriteLine("[{0}] Version MD5: {1}", i, Utilities.ByteArrayToHexString(unkStr3));

                    packet.ReadTime("Time", i);

                    if (ClientVersion.AddedInVersion(ClientVersionBuild.V3_3_3a_11723))
                        packet.ReadInt32("Is banned", i);
                }
            }
        }

        // Changed on 4.3.0, bitshifted
        [Parser(Opcode.CMSG_ADDON_REGISTERED_PREFIXES, ClientVersionBuild.V4_1_0_13914, ClientVersionBuild.V4_3_0_15005)]
        public static void HandleAddonPrefixes(Packet packet)
        {
            var count = packet.ReadUInt32("Count");
            for (var i = 0; i < count; ++i)
                packet.ReadCString("Addon", i);
        }

        [Parser(Opcode.CMSG_ADDON_REGISTERED_PREFIXES, ClientVersionBuild.V4_3_0_15005)]
        public static void HandleAddonPrefixes434(Packet packet)
        {
            var count = packet.ReadBits("Count", 25);
            var lengths = new int[count];
            for (var i = 0; i < count; ++i)
                lengths[i] = (int)packet.ReadBits(5);

            for (var i = 0; i < count; ++i)
                packet.ReadWoWString("Addon", lengths[i], i);
        }

        [Parser(Opcode.CMSG_UNREGISTER_ALL_ADDON_PREFIXES)]
        public static void HandleAddonNull(Packet packet)
        {
        }
    }
}
