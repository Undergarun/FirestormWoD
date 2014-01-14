using System;
using WowPacketParser.Enums;
using WowPacketParser.Enums.Version;
using WowPacketParser.Misc;

namespace WowPacketParser.Parsing.Parsers
{
    public static class GroupHandler
    {
        [Parser(Opcode.CMSG_SET_EVERYONE_IS_ASSISTANT)]
        public static void HandleEveryoneIsAssistant(Packet packet)
        {
            packet.ReadBit("Active");
        }

        [Parser(Opcode.CMSG_GROUP_SET_ROLES, ClientVersionBuild.Zero, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGroupSetRoles(Packet packet)
        {
            packet.ReadUInt32("Role");
            packet.ReadGuid("GUID");
        }

        [Parser(Opcode.CMSG_GROUP_SET_ROLES, ClientVersionBuild.V4_3_4_15595)]
        public static void HandleGroupSetRoles434(Packet packet)
        {
            packet.ReadEnum<LfgRoleFlag>("Role", TypeCode.Int32);
            var guid = packet.StartBitStream(2, 6, 3, 7, 5, 1, 0, 4);
            packet.ParseBitStream(guid, 6, 4, 1, 3, 0, 5, 2, 7);
            packet.WriteGuid("Guid", guid);

        }

        [Parser(Opcode.SMSG_PARTY_UPDATE)]
        public static void HandlePartyUpdate(Packet packet)
        {
            var leaderGuid = new byte[8];
            leaderGuid[1] = packet.ReadBit();
            var looterGUID = new byte[8];
            var groupdGuid = new byte[8];
            groupdGuid[7] = packet.ReadBit();
            var membersCount = packet.ReadBits("members Count", 21);
            var membersGuid = new byte[membersCount][];

            var nameLenght = new uint[membersCount];

            for (int i = 0; i < membersCount; i++)
            {
                membersGuid[i] = new byte[8];

                nameLenght[i] = packet.ReadBits("nameLength", 6, i);
                membersGuid[i][3] = packet.ReadBit();
                membersGuid[i][0] = packet.ReadBit();
                membersGuid[i][4] = packet.ReadBit();
                membersGuid[i][7] = packet.ReadBit();
                membersGuid[i][6] = packet.ReadBit();
                membersGuid[i][1] = packet.ReadBit();
                membersGuid[i][5] = packet.ReadBit();
                membersGuid[i][2] = packet.ReadBit();
            }

            leaderGuid[2] = packet.ReadBit();
            groupdGuid[0] = packet.ReadBit();
            var bit44 = packet.ReadBit("bit 44"); // unk, may be related to automatic loot distribution
            var hasLooterdata = packet.ReadBit("hasLooterData");

            if (hasLooterdata)
            {
                looterGUID[2] = packet.ReadBit();
                looterGUID[0] = packet.ReadBit();
                looterGUID[3] = packet.ReadBit();
                looterGUID[7] = packet.ReadBit();
                looterGUID[4] = packet.ReadBit();
                looterGUID[1] = packet.ReadBit();
                looterGUID[6] = packet.ReadBit();
                looterGUID[5] = packet.ReadBit();
            }

            groupdGuid[2] = packet.ReadBit();
            groupdGuid[1] = packet.ReadBit();
            groupdGuid[6] = packet.ReadBit();
            leaderGuid[7] = packet.ReadBit();
            groupdGuid[4] = packet.ReadBit();
            leaderGuid[4] = packet.ReadBit();
            leaderGuid[3] = packet.ReadBit();
            groupdGuid[3] = packet.ReadBit();

            if (bit44) // unk, may be related to automatic loot distribution
            {
                var bit43 = packet.ReadBit("bit43");
                var bit32 = packet.ReadBit("bit32");
            }

            leaderGuid[6] = packet.ReadBit();
            var sendDifficultyInfo = packet.ReadBit("sendDifficultyInfo");
            leaderGuid[5] = packet.ReadBit();
            leaderGuid[0] = packet.ReadBit();

            groupdGuid[5] = packet.ReadBit();

            if (bit44) // unk, may be related to automatic loot distribution
            {
                packet.ReadUInt32("unk uint32 1");
                packet.ReadByte("unk byte 1");
                packet.ReadSingle("unk float 1");
                packet.ReadByte("unk byte 2");
                packet.ReadByte("unk byte 3");
                packet.ReadByte("unk byte 4");
                packet.ReadByte("unk byte 5");
                packet.ReadUInt32("unk uint32 2");
            }

            for (int i = 0; i < membersCount; i++)
            {
                packet.ReadXORByte(membersGuid[i], 5);
                packet.ReadEnum<MemberFlags>("Member Flags", TypeCode.Byte, i);
                packet.ReadEnum<LfgRoleFlag>("Member Role", TypeCode.Byte, i);
                packet.ReadXORByte(membersGuid[i], 3);
                packet.ReadXORByte(membersGuid[i], 6);
                packet.ReadByte("Group ID", i);
                packet.ReadWoWString("Member Name", (int)nameLenght[i], i);
                packet.ReadXORByte(membersGuid[i], 0);
                packet.ReadXORByte(membersGuid[i], 2);
                packet.ReadEnum<MemberStatus>("Member Status", TypeCode.Byte, i);
                packet.ReadXORByte(membersGuid[i], 7);
                packet.ReadXORByte(membersGuid[i], 1);
                packet.ReadXORByte(membersGuid[i], 4);
                packet.WriteGuid("Member GUID", membersGuid[i], i);
            }

            packet.ReadUInt32("Party Update Counter");

            if (hasLooterdata)
            {
                packet.ReadXORByte(looterGUID, 0);
                packet.ReadXORByte(looterGUID, 1);
                packet.ReadXORByte(looterGUID, 3);
                packet.ReadXORByte(looterGUID, 7);
                packet.ReadXORByte(looterGUID, 6);
                packet.ReadXORByte(looterGUID, 2);
                packet.ReadEnum<LootMethod>("Loot Method", TypeCode.Byte);
                packet.ReadXORByte(looterGUID, 5);
                packet.ReadByte("m_lootThreshold");
                packet.ReadXORByte(looterGUID, 4);
                packet.WriteGuid("looterGUID", looterGUID);
            }

            packet.ReadXORByte(groupdGuid, 3);
            packet.ReadByte("isLFGGroup");
            packet.ReadXORByte(leaderGuid, 7);

            if (sendDifficultyInfo)
            {
                packet.ReadEnum<DifficultyTypes>("Raid Difficulty", TypeCode.UInt32);
                packet.ReadEnum<DifficultyTypes>("Dungeon Difficulty", TypeCode.UInt32);
            }

            packet.ReadXORByte(groupdGuid, 0);
            packet.ReadXORByte(groupdGuid, 6);
            packet.ReadXORByte(leaderGuid, 3);
            packet.ReadEnum<GroupTypes>("GroupType", TypeCode.Byte);
            packet.ReadXORByte(leaderGuid, 1);
            packet.ReadXORByte(leaderGuid, 0);
            packet.ReadXORByte(leaderGuid, 2);
            packet.ReadXORByte(leaderGuid, 4);
            packet.ReadXORByte(leaderGuid, 6);
            packet.ReadByte("group type");
            packet.ReadUInt32("MemberCount (-1)");
            packet.ReadXORByte(groupdGuid, 2);
            packet.ReadXORByte(groupdGuid, 1);
            packet.ReadXORByte(groupdGuid, 4);
            packet.ReadXORByte(groupdGuid, 7);
            packet.ReadXORByte(groupdGuid, 5);
            packet.ReadXORByte(leaderGuid, 5);

            packet.WriteGuid("Group guid", groupdGuid);
            packet.WriteGuid("Leader guid", leaderGuid);
        }

        [Parser(Opcode.SMSG_ROLE_CHANGED_INFORM)]
        public static void HandleRoleChanedInform(Packet packet)
        {
            var assignerGuid = new byte[8];
            var targetGuid = new byte[8];

            packet.ReadEnum<LfgRoleFlag>("Old role", TypeCode.UInt32);
            packet.ReadByte("Unk Byte");
            packet.ReadEnum<LfgRoleFlag>("New role", TypeCode.UInt32);

            targetGuid[0] = packet.ReadBit();
            targetGuid[3] = packet.ReadBit();
            assignerGuid[1] = packet.ReadBit();
            assignerGuid[7] = packet.ReadBit();
            targetGuid[5] = packet.ReadBit();
            assignerGuid[4] = packet.ReadBit();
            assignerGuid[3] = packet.ReadBit();
            targetGuid[2] = packet.ReadBit();
            targetGuid[7] = packet.ReadBit();
            targetGuid[6] = packet.ReadBit();
            assignerGuid[6] = packet.ReadBit();
            targetGuid[4] = packet.ReadBit();
            assignerGuid[0] = packet.ReadBit();
            targetGuid[1] = packet.ReadBit();
            assignerGuid[5] = packet.ReadBit();
            assignerGuid[2] = packet.ReadBit();

            packet.ReadXORByte(assignerGuid, 3);
            packet.ReadXORByte(targetGuid, 2);
            packet.ReadXORByte(targetGuid, 6);
            packet.ReadXORByte(assignerGuid, 1);
            packet.ReadXORByte(targetGuid, 4);
            packet.ReadXORByte(assignerGuid, 0);
            packet.ReadXORByte(targetGuid, 1);
            packet.ReadXORByte(assignerGuid, 6);
            packet.ReadXORByte(assignerGuid, 2);
            packet.ReadXORByte(targetGuid, 7);
            packet.ReadXORByte(targetGuid, 5);
            packet.ReadXORByte(targetGuid, 3);
            packet.ReadXORByte(assignerGuid, 4);
            packet.ReadXORByte(assignerGuid, 7);
            packet.ReadXORByte(targetGuid, 0);
            packet.ReadXORByte(targetGuid, 5);

            packet.WriteGuid("Target GUID", targetGuid);
            packet.WriteGuid("Assigner GUID", assignerGuid);
        }

        [Parser(Opcode.SMSG_PARTY_MEMBER_STATS)]
        [Parser(Opcode.SMSG_PARTY_MEMBER_STATS_FULL)]
        public static void HandlePartyMemberStats(Packet packet)
        {
            /*
            -- UNK PACKET
            var count = packet.ReadBits("counter", 18);

            var unkGuid = new byte[count][];
            var unkLen = new uint[count];
            var unkLen2 = new uint[count];
            var count2 = new uint[count];
            var bit28 = new bool[count];
            var bit16 = new bool[count];

            for (int i = 0; i < count; i++)
            {
                unkGuid[i] = new byte[8];

                count2[i] = packet.ReadBits("counter2", 17, i);
                bit16[i] = packet.ReadBit("bit16", i);

                if (bit16[i])
                    packet.StartBitStream(unkGuid[i], 3, 6, 4, 2, 5, 1, 0, 7);

                bit28[i] = packet.ReadBit("bit28", i);
                unkLen[i] = packet.ReadBits("unkLen", 13, i);
                unkLen2[i] = packet.ReadBits("unkLen2", 8, i);

                for (int j = 0; j < count2[i]; j++)
                    packet.ReadBit("unkBit", i, j);
            }

            for (int i = 0; i < count; i++)
            {
                for (int j = 0; j < count2[i]; j++)
                {
                    packet.ReadUInt32("unk UInt32", i, j);
                    packet.ReadUInt32("unk UInt32", i, j);

                    for (int y = 0; y < 8; y++)
                    {
                        packet.ReadUInt32("unk UInt32", i, j, y);
                        packet.ReadUInt32("unk UInt32", i, j, y);
                        packet.ReadUInt32("unk UInt32", i, j, y);
                    }

                    packet.ReadUInt32("unk UInt32", i, j);
                    packet.ReadByte("unk Byte", i, j);
                    packet.ReadUInt32("unk UInt32", i, j);
                    packet.ReadUInt32("unk UInt32", i, j);
                    packet.ReadUInt32("unk UInt32", i, j);
                    packet.ReadUInt32("unk UInt32", i, j);
                    var bytesCounter = packet.ReadUInt32("unk UInt32", i, j);
                    if (bytesCounter > 0)
                        packet.ReadBytes((int)bytesCounter);
                    packet.ReadUInt32("unk UInt32", i, j);
                }

                packet.ReadUInt32("unk UInt32", i);

                if (bit16[i])
                {
                    packet.ParseBitStream(unkGuid[i], 2, 0, 4, 5, 3, 6, 1, 7);
                    packet.WriteGuid("Unk GUID", unkGuid[i], i);
                }

                packet.ReadWoWString("unkString2", unkLen2[i], i);
                packet.ReadUInt32("unk UInt32", i);
                packet.ReadUInt32("unk UInt32", i);
                packet.ReadUInt64("unk UInt64", i);
                packet.ReadUInt32("unk UInt32", i);
                packet.ReadUInt64("unk UInt64", i);
                packet.ReadUInt32("unk UInt32", i);
                packet.ReadWoWString("unkString", unkLen[i], i);

                if (bit28[i])
                    packet.ReadUInt32("unk UInt32", i);

                packet.ReadByte("unk Byte", i);
                packet.ReadUInt32("unk UInt32", i);
            }

            packet.ReadUInt32("unk UInt32");*/

            if (ClientVersion.AddedInVersion(ClientType.WrathOfTheLichKing) &&
                packet.Opcode == Opcodes.GetOpcode(Opcode.SMSG_PARTY_MEMBER_STATS_FULL))
                packet.ReadBoolean("Add arena opponent");

            packet.ReadPackedGuid("GUID");
            var updateFlags = packet.ReadEnum<GroupUpdateFlag>("Update Flags", TypeCode.UInt32);

            if (updateFlags.HasFlag(GroupUpdateFlag.Status))
                packet.ReadEnum<GroupMemberStatusFlag>("Status", TypeCode.Int16);

            if (updateFlags.HasFlag(GroupUpdateFlag.CurrentHealth))
            {
                if (ClientVersion.AddedInVersion(ClientType.WrathOfTheLichKing))
                    packet.ReadInt32("Current Health");
                else
                    packet.ReadUInt16("Current Health");
            }

            if (updateFlags.HasFlag(GroupUpdateFlag.MaxHealth))
            {
                if (ClientVersion.AddedInVersion(ClientType.WrathOfTheLichKing))
                    packet.ReadInt32("Max Health");
                else
                    packet.ReadUInt16("Max Health");
            }

            if (updateFlags.HasFlag(GroupUpdateFlag.PowerType))
                packet.ReadEnum<PowerType>("Power type", TypeCode.Byte);

            if (updateFlags.HasFlag(GroupUpdateFlag.CurrentPower))
                packet.ReadInt16("Current Power");

            if (updateFlags.HasFlag(GroupUpdateFlag.MaxPower))
                packet.ReadInt16("Max Power");

            if (updateFlags.HasFlag(GroupUpdateFlag.Level))
                packet.ReadInt16("Level");

            if (updateFlags.HasFlag(GroupUpdateFlag.Zone))
                packet.ReadEntryWithName<Int16>(StoreNameType.Zone, "Zone Id");

            if (updateFlags.HasFlag(GroupUpdateFlag.Position))
            {
                packet.ReadInt16("Position X");
                packet.ReadInt16("Position Y");
            }

            if (updateFlags.HasFlag(GroupUpdateFlag.Auras))
            {
                var auraMask = packet.ReadUInt64("Aura Mask");

                var maxAura = ClientVersion.AddedInVersion(ClientType.WrathOfTheLichKing) ? 64 : 56;
                for (var i = 0; i < maxAura; ++i)
                {
                    if ((auraMask & (1ul << i)) == 0)
                        continue;

                    if (ClientVersion.AddedInVersion(ClientType.WrathOfTheLichKing))
                        packet.ReadEntryWithName<UInt32>(StoreNameType.Spell, "Spell Id", i);
                    else
                        packet.ReadEntryWithName<UInt16>(StoreNameType.Spell, "Spell Id", i);

                    packet.ReadEnum<AuraFlag>("Aura Flags", TypeCode.Byte, i);
                }
            }

            if (updateFlags.HasFlag(GroupUpdateFlag.PetGuid))
                packet.ReadGuid("Pet GUID");

            if (updateFlags.HasFlag(GroupUpdateFlag.PetName))
                packet.ReadCString("Pet Name");

            if (updateFlags.HasFlag(GroupUpdateFlag.PetModelId))
                packet.ReadInt16("Pet Modelid");

            if (updateFlags.HasFlag(GroupUpdateFlag.PetCurrentHealth))
            {
                if (ClientVersion.AddedInVersion(ClientType.WrathOfTheLichKing))
                    packet.ReadInt32("Pet Current Health");
                else
                    packet.ReadUInt16("Pet Current Health");
            }

            if (updateFlags.HasFlag(GroupUpdateFlag.PetMaxHealth))
            {
                if (ClientVersion.AddedInVersion(ClientType.WrathOfTheLichKing))
                    packet.ReadInt32("Pet Max Health");
                else
                    packet.ReadUInt16("Pet Max Health");
            }

            if (updateFlags.HasFlag(GroupUpdateFlag.PetPowerType))
                packet.ReadEnum<PowerType>("Pet Power type", TypeCode.Byte);

            if (updateFlags.HasFlag(GroupUpdateFlag.PetCurrentPower))
                packet.ReadInt16("Pet Current Power");

            if (updateFlags.HasFlag(GroupUpdateFlag.PetMaxPower))
                packet.ReadInt16("Pet Max Power");

            if (updateFlags.HasFlag(GroupUpdateFlag.PetAuras))
            {
                var auraMask = packet.ReadUInt64("Pet Auramask");

                var maxAura = ClientVersion.AddedInVersion(ClientType.WrathOfTheLichKing) ? 64 : 56;
                for (var i = 0; i < maxAura; ++i)
                {
                    if ((auraMask & (1ul << i)) == 0)
                        continue;

                    if (ClientVersion.AddedInVersion(ClientType.WrathOfTheLichKing))
                        packet.ReadEntryWithName<UInt32>(StoreNameType.Spell, "Spell Id", i);
                    else
                        packet.ReadEntryWithName<UInt16>(StoreNameType.Spell, "Spell Id", i);

                    packet.ReadEnum<AuraFlag>("Aura Flags", TypeCode.Byte, i);
                }
            }

            if (ClientVersion.AddedInVersion(ClientType.WrathOfTheLichKing) && // no idea when this was added exactly, doesn't exist in 2.4.1
                updateFlags.HasFlag(GroupUpdateFlag.VehicleSeat))
                packet.ReadInt32("Vehicle Seat");
        }

        [Parser(Opcode.CMSG_GROUP_SET_LEADER)]
        public static void HandleGroupSetLeader(Packet packet)
        {
            packet.ReadGuid("GUID");
        }

        [Parser(Opcode.SMSG_GROUP_SET_LEADER)]
        [Parser(Opcode.SMSG_GROUP_DECLINE)]
        public static void HandleGroupDecline(Packet packet)
        {
            packet.ReadCString("Name");
        }

        [Parser(Opcode.CMSG_GROUP_INVITE)]
        public static void HandleGroupInvite540(Packet packet)
        {
            var guid = new byte[8];

            packet.ReadInt32("Unk Int32"); // Always 0 ?
            packet.ReadInt32("CrossRealm UInt32 ?"); // Non-zero in cross realm parties (1383)
            packet.ReadByte("CrossRealmByte ?");

            guid[7] = packet.ReadBit();
            guid[0] = packet.ReadBit();
            guid[4] = packet.ReadBit();
            guid[3] = packet.ReadBit();
            guid[6] = packet.ReadBit();

            var realmLen = packet.ReadBits("RealmNameLen", 9);
            //packet.ReadBits("Unk value", 8);
            var unkBit2 = packet.ReadBit("HasRealmName");

            guid[2] = packet.ReadBit();
            guid[5] = packet.ReadBit();
            guid[1] = packet.ReadBit();

            var nameLen = packet.ReadBits("NameLen", 9);
            //packet.ReadBits("Unk value", 8);
            var unkBit = packet.ReadBit("HasName");

            packet.ResetBitReader();

            packet.ReadXORByte(guid, 0);
            packet.ReadXORByte(guid, 4);
            packet.ReadXORByte(guid, 5);
            packet.ReadXORByte(guid, 7);
            packet.ReadXORByte(guid, 1);

            packet.ReadWoWString("RealmName", realmLen);

            packet.ReadXORByte(guid, 3);

            packet.ReadWoWString("Name", nameLen);

            packet.ReadXORByte(guid, 6);
            packet.ReadXORByte(guid, 2);

            packet.WriteGuid("Guid", guid); // Non-zero in cross realm parties
        }

        [Parser(Opcode.SMSG_PARTY_COMMAND_RESULT)]
        public static void HandleGroupCommandResult(Packet packet)
        {
            packet.ReadUInt32("Unk uint32"); // 0 or 2
            var name = packet.ReadCString();
            Console.WriteLine("Player: " + name);

            packet.ReadUInt32("Result");
            packet.ReadUInt32("Unk uint32"); // 0
            var guid = packet.ReadGuid("Guid");
        }

        [Parser(Opcode.SMSG_GROUP_INVITE)]
        public static void HandleGroupInviteSmsg(Packet packet)
        {
            var guid = new byte[8];

            var isAlreadyInGroup = packet.ReadBit("isAlreadyInGroup");
            var bit297 = packet.ReadBit("bit297");
            guid[4] = packet.ReadBit();
            var nameLen = packet.ReadBits("NameLen", 6);
            guid[2] = packet.ReadBit();
            var bit33 = packet.ReadBit("bit33");
            var bit32 = packet.ReadBit("bit32");
            guid[3] = packet.ReadBit();
            guid[7] = packet.ReadBit();
            var realmLen = 2 * packet.ReadBits(8);
            Console.WriteLine("realmLen: " + realmLen);
            var pair = packet.ReadBit("pair");
            if (pair)
                realmLen++;
            guid[0] = packet.ReadBit();
            guid[5] = packet.ReadBit();
            var counter = packet.ReadBits("Counter", 22);
            guid[6] = packet.ReadBit();
            guid[1] = packet.ReadBit();

            packet.ReadUInt32("Unk uint32");
            packet.ReadUInt64("Unk uint64");

            packet.ReadXORByte(guid, 3);
            packet.ReadXORByte(guid, 5);
            packet.ReadUInt32("Unk uint32");
            packet.ReadWoWString("RealmName", realmLen);
            packet.ReadWoWString("Name", nameLen);
            packet.ReadUInt32("Unk uint32");
            packet.ReadXORByte(guid, 7);
            packet.ReadXORByte(guid, 4);
            packet.ReadXORByte(guid, 1);

            for (var i = 0; i < counter; i++)
                packet.ReadInt32("Unk Int 32", i);

            packet.ReadXORByte(guid, 2);
            packet.ReadXORByte(guid, 6);
            packet.ReadXORByte(guid, 0);

            packet.WriteGuid("Guid", guid);
        }

        [Parser(Opcode.CMSG_GROUP_UNINVITE_GUID)]
        public static void HandleGroupUninviteGuid(Packet packet)
        {
            packet.ReadGuid("GUID");
            packet.ReadCString("Reason");
        }

        [Parser(Opcode.CMSG_GROUP_ACCEPT)]
        public static void HandleGroupAccept(Packet packet)
        {
            packet.ReadInt32("Unk Int32");
        }

        [Parser(Opcode.CMSG_GROUP_ACCEPT_DECLINE)]
        public static void HandleGroupAcceptDecline(Packet packet)
        {
            packet.ReadBit("Accepted");
            // 7 0 bits here
            packet.ReadUInt32("Unknown");
        }

        [Parser(Opcode.MSG_RANDOM_ROLL)]
        public static void HandleRandomRollPackets(Packet packet)
        {
            packet.ReadInt32("Minimum");
            packet.ReadInt32("Maximum");

            if (packet.Direction == Direction.ClientToServer)
                return;

            packet.ReadInt32("Roll");
            packet.ReadGuid("GUID");
        }

        [Parser(Opcode.CMSG_REQUEST_PARTY_MEMBER_STATS)]
        public static void HandleRequestPartyMemberStats(Packet packet)
        {
            packet.ReadGuid("GUID");
        }

        [Parser(Opcode.SMSG_RAID_GROUP_ONLY)]
        public static void HandleRaidGroupOnly(Packet packet)
        {
            packet.ReadInt32("Time left");
            packet.ReadEnum<InstanceStatus>("Group Type Status?", TypeCode.Int32);
        }

        [Parser(Opcode.SMSG_REAL_GROUP_UPDATE)]
        public static void HandleRealGroupUpdate(Packet packet)
        {
            packet.ReadByte("Unk Byte");
            packet.ReadUInt32("Member Count");
            packet.ReadGuid("GUID");
        }

        [Parser(Opcode.CMSG_GROUP_CHANGE_SUB_GROUP)]
        public static void HandleGroupChangesubgroup(Packet packet)
        {
            packet.ReadCString("Name");
            packet.ReadByte("Group");
        }

        [Parser(Opcode.MSG_RAID_READY_CHECK)]
        public static void HandleRaidReadyCheck(Packet packet)
        {
            if (packet.Direction == Direction.ClientToServer)
            {
                // Packet is sent in two different methods. One sends a byte and one doesn't
                if (packet.CanRead())
                    packet.ReadBoolean("Ready");
            }
            else
                packet.ReadGuid("GUID");
        }

        [Parser(Opcode.MSG_RAID_READY_CHECK_CONFIRM)]
        public static void HandleRaidReadyCheckConfirm(Packet packet)
        {
            packet.ReadGuid("GUID");
            packet.ReadBoolean("Ready");
        }

        [Parser(Opcode.MSG_MINIMAP_PING)]
        public static void HandleServerMinimapPing(Packet packet)
        {
            if (packet.Direction == Direction.ServerToClient)
                packet.ReadGuid("GUID");

            var position = packet.ReadVector2();
            packet.WriteLine("Position: " + position);
        }

        [Parser(Opcode.CMSG_GROUP_RAID_CONVERT)]
        public static void HandleGroupRaidConvert(Packet packet)
        {
            if (ClientVersion.AddedInVersion(ClientType.Cataclysm))
                packet.ReadBoolean("ToRaid");
        }

        [Parser(Opcode.CMSG_GROUP_SWAP_SUB_GROUP)]
        public static void HandleGroupSwapSubGroup(Packet packet)
        {
            packet.ReadCString("Player 1 name");
            packet.ReadCString("Player 2 name");
        }

        [Parser(Opcode.CMSG_GROUP_ASSISTANT_LEADER)]
        public static void HandleGroupAssistantLeader(Packet packet)
        {
            packet.ReadGuid("GUID");
            packet.ReadBoolean("Promote"); // False = demote
        }

        [Parser(Opcode.MSG_PARTY_ASSIGNMENT)]
        public static void HandlePartyAssigment(Packet packet)
        {
            //if (packet.Direction == Direction.ClientToServer)
            packet.ReadByte("Assigment");
            packet.ReadBoolean("Apply");
            packet.ReadGuid("Guid");
        }

        [Parser(Opcode.SMSG_GROUP_SET_ROLE)] // 4.3.4
        public static void HandleGroupSetRole(Packet packet)
        {
            var guid1 = new byte[8];
            var guid2 = new byte[8];
            guid1[1] = packet.ReadBit();
            guid2[0] = packet.ReadBit();
            guid2[2] = packet.ReadBit();
            guid2[4] = packet.ReadBit();
            guid2[7] = packet.ReadBit();
            guid2[3] = packet.ReadBit();
            guid1[7] = packet.ReadBit();
            guid2[5] = packet.ReadBit();

            guid1[5] = packet.ReadBit();
            guid1[4] = packet.ReadBit();
            guid1[3] = packet.ReadBit();
            guid2[6] = packet.ReadBit();
            guid1[2] = packet.ReadBit();
            guid1[6] = packet.ReadBit();
            guid2[1] = packet.ReadBit();
            guid1[0] = packet.ReadBit();

            packet.ReadXORByte(guid1, 7);
            packet.ReadXORByte(guid2, 3);
            packet.ReadXORByte(guid1, 6);
            packet.ReadXORByte(guid2, 4);
            packet.ReadXORByte(guid2, 0);
            packet.ReadEnum<LfgRoleFlag>("New Roles", TypeCode.Int32);
            packet.ReadXORByte(guid2, 6);
            packet.ReadXORByte(guid2, 2);
            packet.ReadXORByte(guid1, 0);

            packet.ReadXORByte(guid1, 4);
            packet.ReadXORByte(guid2, 1);
            packet.ReadXORByte(guid1, 3);
            packet.ReadXORByte(guid1, 5);
            packet.ReadXORByte(guid1, 2);
            packet.ReadXORByte(guid2, 5);
            packet.ReadXORByte(guid2, 7);
            packet.ReadXORByte(guid1, 1);

            packet.ReadEnum<LfgRoleFlag>("Old Roles", TypeCode.Int32);
            packet.WriteGuid("Assigner Guid", guid1);
            packet.WriteGuid("Target Guid", guid2);
        }

        [Parser(Opcode.SMSG_RAID_MARKERS_CHANGED)]
        public static void HandleRaidMarkersChanged(Packet packet)
        {
            packet.ReadUInt32("Unk Uint32");
        }

        [Parser(Opcode.CMSG_GROUP_INVITE_RESPONSE)]
        public static void HandleGroupInviteResponse434(Packet packet)
        {
            packet.ReadByte("Skipped byte");
            packet.ReadBit("Bit 1");
            var bit2 = packet.ReadBit("Bit 2");

            if (bit2)
                packet.ReadUInt32("Unk Uint32");
        }

        [Parser(Opcode.SMSG_RAID_SUMMON_FAILED)] // 4.3.4
        public static void HandleRaidSummonFailed(Packet packet)
        {
            var count = packet.ReadBits("Count", 23);

            var guids = new byte[count][];

            for (int i = 0; i < count; ++i)
                guids[i] = packet.StartBitStream(5, 3, 1, 7, 2, 0, 6, 4);

            for (int i = 0; i < count; ++i)
            {
                packet.ReadXORByte(guids[i], 4);
                packet.ReadXORByte(guids[i], 2);
                packet.ReadXORByte(guids[i], 0);
                packet.ReadXORByte(guids[i], 6);
                packet.ReadXORByte(guids[i], 5);

                packet.ReadEnum<RaidSummonFail>("Error", TypeCode.Int32, i);

                packet.ReadXORByte(guids[i], 7);
                packet.ReadXORByte(guids[i], 3);
                packet.ReadXORByte(guids[i], 1);

                packet.WriteGuid("Guid", guids[i], i);
            }

        }

        [Parser(Opcode.CMSG_GROUP_DISBAND)]
        [Parser(Opcode.SMSG_GROUP_DESTROYED)]
        [Parser(Opcode.SMSG_GROUP_UNINVITE)]
        [Parser(Opcode.CMSG_GROUP_DECLINE)]
        [Parser(Opcode.MSG_RAID_READY_CHECK_FINISHED)]
        [Parser(Opcode.CMSG_REQUEST_RAID_INFO)]
        [Parser(Opcode.CMSG_GROUP_REQUEST_JOIN_UPDATES)]
        public static void HandleGroupNull(Packet packet)
        {
        }
    }
}
