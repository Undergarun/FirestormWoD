using System;
using System.IO;
using WowPacketParser.Enums;
using WowPacketParser.Enums.Version;
using WowPacketParser.Misc;
using WowPacketParser.Store;
using WowPacketParser.Store.Objects;

namespace WowPacketParser.Parsing.Parsers
{
    public static class CombatLogHandler
    {
        [Parser(Opcode.SMSG_COMBAT_LOG_MULTIPLE)]
        public static void HandleCombatLogMultiple(Packet packet)
        {
            var count = packet.ReadInt32("Count");
            var unk1 = packet.ReadInt32();

            for (var i = 0; i < count; i++)
            {
                var unk2 = packet.ReadInt32();
                packet.WriteLine("["+ i+ "] Unknown: {0}", unk1 - unk2);

                var opcode = Opcodes.GetOpcode(packet.ReadInt32());
                packet.WriteLine("Opcode: " + opcode);
                switch (opcode)
                {
                    case Opcode.SMSG_SPELLHEALLOG:
                    {
                        ReadSpellHealLog(ref packet, i);
                        break;
                    }
                    case Opcode.SMSG_SPELLENERGIZELOG:
                    {
                        ReadSpellEnergizeLog(ref packet, i);
                        break;
                    }
                    case Opcode.SMSG_PERIODICAURALOG:
                    {
                        ReadPeriodicAuraLog(ref packet, i); // sub_5EEE10
                        break;
                    }
                    case Opcode.SMSG_SPELLLOGEXECUTE:
                    {
                        ReadSpellLogExecute(ref packet, i);
                        break;
                    }
                    case Opcode.SMSG_SPELLNONMELEEDAMAGELOG:
                    {
                        ReadSpellNonMeleeDamageLog(ref packet, i);
                        break;
                    }
                    case Opcode.SMSG_SPELLLOGMISS:
                    {
                        ReadSpellMissLog(ref packet, i);
                        break;
                    }
                    case Opcode.SMSG_SPELLSTEALLOG:
                    case Opcode.SMSG_SPELLDISPELLOG:
                    case Opcode.SMSG_SPELLBREAKLOG:
                    {
                        ReadSpellRemoveLog(ref packet, i);
                        break;
                    }
                    default:
                        throw new InvalidDataException("Unknown Spell Log Type/Opcode: " + opcode);
                }
            }
        }

        [Parser(Opcode.SMSG_SPELLSTEALLOG)]
        [Parser(Opcode.SMSG_SPELLDISPELLOG)]
        [Parser(Opcode.SMSG_SPELLBREAKLOG)]
        public static void HandleSpellRemoveLog(Packet packet)
        {
            var targetGuid = new byte[8];
            var casterGuid = new byte[8];
            var targetPowerGuid = new byte[8];

            targetGuid[5] = packet.ReadBit();
            casterGuid[5] = packet.ReadBit();
            casterGuid[4] = packet.ReadBit();
            targetGuid[2] = packet.ReadBit();
            casterGuid[0] = packet.ReadBit();
            targetGuid[6] = packet.ReadBit();
            var bit80 = packet.ReadBit("bit80");
            casterGuid[6] = packet.ReadBit();

            var powerCounter = 0;
            if (bit80)
            {
                targetPowerGuid[1] = packet.ReadBit();
                targetPowerGuid[6] = packet.ReadBit();
                targetPowerGuid[3] = packet.ReadBit();
                powerCounter = (int)packet.ReadBits("powerCounter", 21);
                targetPowerGuid[2] = packet.ReadBit();
                targetPowerGuid[0] = packet.ReadBit();
                targetPowerGuid[7] = packet.ReadBit();
                targetPowerGuid[4] = packet.ReadBit();
                targetPowerGuid[5] = packet.ReadBit();
            }

            targetGuid[4] = packet.ReadBit();
            targetGuid[1] = packet.ReadBit();
            casterGuid[2] = packet.ReadBit();
            targetGuid[0] = packet.ReadBit();
            casterGuid[3] = packet.ReadBit();
            targetGuid[3] = packet.ReadBit();
            casterGuid[1] = packet.ReadBit();
            targetGuid[7] = packet.ReadBit();
            casterGuid[7] = packet.ReadBit();

            packet.ReadXORByte(targetGuid, 0);
            packet.ReadXORByte(targetGuid, 4);
            packet.ReadXORByte(casterGuid, 3);
            packet.ReadXORByte(targetGuid, 3);

            if (bit80)
            {
                packet.ReadXORByte(targetPowerGuid, 5);
                packet.ReadUInt32("Unk UInt32");
                packet.ReadXORByte(targetPowerGuid, 1);
                packet.ReadXORByte(targetPowerGuid, 7);
                packet.ReadUInt32("Unk UInt32");
                packet.ReadXORByte(targetPowerGuid, 6);
                packet.ReadUInt32("Unk UInt32");
                packet.ReadXORByte(targetPowerGuid, 4);
                packet.ReadXORByte(targetPowerGuid, 0);

                for (int i = 0; i < powerCounter; ++i)
                {
                    packet.ReadUInt32("Power Type", i);
                    packet.ReadUInt32("Power Amount", i);
                }

                packet.ReadXORByte(targetPowerGuid, 2);
                packet.ReadXORByte(targetPowerGuid, 3);

                packet.WriteGuid("Target Power GUID", targetPowerGuid);
            }

            packet.ReadUInt32("Interrupt Spell");
            packet.ReadXORByte(casterGuid, 4);
            packet.ReadXORByte(targetGuid, 1);
            packet.ReadXORByte(casterGuid, 7);
            packet.ReadXORByte(casterGuid, 0);
            packet.ReadXORByte(targetGuid, 5);
            packet.ReadUInt32("Interrupted Spell");
            packet.ReadXORByte(casterGuid, 1);
            packet.ReadXORByte(casterGuid, 5);
            packet.ReadXORByte(targetGuid, 7);
            packet.ReadXORByte(casterGuid, 6);
            packet.ReadXORByte(targetGuid, 6);
            packet.ReadXORByte(targetGuid, 2);
            packet.ReadXORByte(casterGuid, 2);

            packet.WriteGuid("Caster GUID", targetGuid);
            packet.WriteGuid("Target GUID", casterGuid);
        }

        [Parser(Opcode.SMSG_SPELL_COOLDOWN)]
        public static void HandleSpellCooldown(Packet packet)
        {
            var guid = new byte[8];

            guid[1] = packet.ReadBit(); // 1 - 1
            guid[5] = packet.ReadBit(); // 0 - 0
            var count = packet.ReadBits("count", 21); // 32 - 1
            guid[0] = packet.ReadBit(); // 1 - 1
            guid[6] = packet.ReadBit(); // 0 - 0
            guid[3] = packet.ReadBit(); // 0 - 0
            guid[7] = packet.ReadBit(); // 1 - 1
            guid[4] = packet.ReadBit(); // 0 - 0
            guid[2] = packet.ReadBit(); // 1 - 1
            var bit32 = packet.ReadBit("bit32"); // !0 - !1

            packet.ReadXORByte(guid, 7); // 0 - 0

            for (int i = 0; i < count; ++i)
            {
                packet.ReadUInt32("Spell ID");
                packet.ReadUInt32("Cooldown (ms)");
            }

            packet.ReadXORByte(guid, 5);
            packet.ReadXORByte(guid, 4);
            packet.ReadXORByte(guid, 6);

            if (bit32)
                packet.ReadByte("Unk Byte"); // 06

            packet.ReadXORByte(guid, 1); // E4 - E4
            packet.ReadXORByte(guid, 2); // 7A - 7A
            packet.ReadXORByte(guid, 0); // 61 - 61
            packet.ReadXORByte(guid, 3);

            packet.WriteGuid("Guid", guid);
        }

        [Parser(Opcode.SMSG_PERIODICAURALOG)]
        public static void HandlePeriodicAuraLog(Packet packet)
        {
            var targetGuid = new byte[8];
            var playerGuid = new byte[8];
            var playerPowerGuid = new byte[8];
            var powerCount = 0;

            playerGuid[7] = packet.ReadBit();
            var hasPowerValue = packet.ReadBit("hasPowerValue");
            playerGuid[3] = packet.ReadBit();
            playerGuid[5] = packet.ReadBit();

            if (hasPowerValue)
            {
                playerPowerGuid[4] = packet.ReadBit();
                playerPowerGuid[6] = packet.ReadBit();
                playerPowerGuid[7] = packet.ReadBit();
                playerPowerGuid[1] = packet.ReadBit();
                playerPowerGuid[0] = packet.ReadBit();
                playerPowerGuid[2] = packet.ReadBit();
                powerCount = (int)packet.ReadBits("powerCount", 21);
                playerPowerGuid[5] = packet.ReadBit();
                playerPowerGuid[3] = packet.ReadBit();
            }

            playerGuid[6] = packet.ReadBit();
            targetGuid[1] = packet.ReadBit();
            var count = packet.ReadBits("count", 21);
            playerGuid[2] = packet.ReadBit();
            playerGuid[1] = packet.ReadBit();
            targetGuid[2] = packet.ReadBit();
            targetGuid[4] = packet.ReadBit();
            playerGuid[4] = packet.ReadBit();

            var isCrit = new bool[count];
            var hasOverkill = new bool[count];
            var hasAbsorb = new bool[count];
            var hasSchoolMask = new bool[count];
            var hasOverHeal = new bool[count];
            for (int i = 0; i < count; ++i)
            {
                isCrit[i] = packet.ReadBit("isCrit", i);
                hasOverkill[i] = !packet.ReadBit();
                Console.WriteLine("[" + i + "] hasOverkill: " + hasOverkill[i]);
                hasAbsorb[i] = !packet.ReadBit();
                Console.WriteLine("[" + i + "] hasAbsorb: " + hasAbsorb[i]);
                hasSchoolMask[i] = !packet.ReadBit();
                Console.WriteLine("[" + i + "] hasSchoolMask: " + hasSchoolMask[i]);
                hasOverHeal[i] = !packet.ReadBit();
                Console.WriteLine("[" + i + "] hasOverHeal: " + hasOverHeal[i]);
            }

            playerGuid[0] = packet.ReadBit();
            targetGuid[0] = packet.ReadBit();
            targetGuid[7] = packet.ReadBit();
            targetGuid[3] = packet.ReadBit();
            targetGuid[6] = packet.ReadBit();
            targetGuid[5] = packet.ReadBit();

            packet.ReadXORByte(targetGuid, 5);
            packet.ReadXORByte(playerGuid, 7);

            for (int i = 0; i < count; ++i)
            {
                if (hasOverHeal[i])
                    packet.ReadUInt32("OverHeal", i);

                packet.ReadUInt32("AuraType", i);

                if (hasOverkill[i])
                    packet.ReadUInt32("Overkill", i);
                if (hasAbsorb[i])
                    packet.ReadUInt32("Absorb", i);

                packet.ReadUInt32("Amount", i);

                if (hasSchoolMask[i])
                    packet.ReadEnum<SpellSchoolMask>("SpellSchoolMask", TypeCode.UInt32);
            }

            packet.ReadXORByte(targetGuid, 0);
            packet.ReadXORByte(targetGuid, 4);
            packet.ReadXORByte(targetGuid, 2);

            if (hasPowerValue)
            {
                packet.ReadXORByte(playerPowerGuid, 0);
                packet.ReadXORByte(playerPowerGuid, 3);
                packet.ReadUInt32("Unk UInt32");
                packet.ReadUInt32("Unk UInt32");
                packet.ReadXORByte(playerPowerGuid, 4);
                packet.ReadXORByte(playerPowerGuid, 1);
                packet.ReadXORByte(playerPowerGuid, 5);
                packet.ReadXORByte(playerPowerGuid, 2);
                packet.ReadUInt32("Current Health");

                for (int i = 0; i < powerCount; ++i)
                {
                    packet.ReadUInt32("Power Amount", i);
                    packet.ReadEnum<PowerType>("Power type", TypeCode.Int32);
                }

                packet.ReadXORByte(playerPowerGuid, 6);
                packet.ReadXORByte(playerPowerGuid, 7);

                packet.WriteGuid("Power Player GUID", playerPowerGuid);
            }

            packet.ReadXORByte(playerGuid, 4);
            packet.ReadXORByte(playerGuid, 3);
            packet.ReadXORByte(targetGuid, 1);
            packet.ReadXORByte(playerGuid, 0);
            packet.ReadXORByte(playerGuid, 5);
            packet.ReadUInt32("Spell ID");
            packet.ReadXORByte(playerGuid, 1);
            packet.ReadXORByte(targetGuid, 7);
            packet.ReadXORByte(playerGuid, 2);
            packet.ReadXORByte(targetGuid, 6);
            packet.ReadXORByte(targetGuid, 3);
            packet.ReadXORByte(playerGuid, 6);

            packet.WriteGuid("Target GUID", targetGuid);
            packet.WriteGuid("Player GUID", playerGuid);
        }

        [Parser(Opcode.SMSG_SPELLNONMELEEDAMAGELOG)]
        public static void HandleSpellNonMeleeDmgLog(Packet packet)
        {
            ReadSpellNonMeleeDamageLog(ref packet);
        }

        [Parser(Opcode.SMSG_SPELLHEALLOG)]
        public static void HandleSpellHealLog(Packet packet)
        {
            var targetGuid = new byte[8];
            var casterGuid = new byte[8];
            var playerPowerGuid = new byte[8];

            var isCrit = packet.ReadBit("isCrit");
            casterGuid[5] = packet.ReadBit();
            targetGuid[4] = packet.ReadBit();
            targetGuid[2] = packet.ReadBit();
            targetGuid[7] = packet.ReadBit();
            targetGuid[0] = packet.ReadBit();
            var isDebug = packet.ReadBit("isDebug");
            casterGuid[1] = packet.ReadBit();
            targetGuid[6] = packet.ReadBit();
            targetGuid[5] = packet.ReadBit();
            casterGuid[7] = packet.ReadBit();
            targetGuid[3] = packet.ReadBit();
            casterGuid[0] = packet.ReadBit();
            casterGuid[2] = packet.ReadBit();
            casterGuid[3] = packet.ReadBit();
            var hasPowerData = packet.ReadBit("hasPowerData");
            var isDebug2 = packet.ReadBit("isDebug2");

            var powerCount = 0;
            if (hasPowerData)
            {
                playerPowerGuid[0] = packet.ReadBit();
                playerPowerGuid[4] = packet.ReadBit();
                playerPowerGuid[6] = packet.ReadBit();
                playerPowerGuid[2] = packet.ReadBit();
                playerPowerGuid[5] = packet.ReadBit();
                powerCount = (int)packet.ReadBits("powerCount", 21);
                playerPowerGuid[1] = packet.ReadBit();
                playerPowerGuid[7] = packet.ReadBit();
                playerPowerGuid[3] = packet.ReadBit();
            }

            casterGuid[6] = packet.ReadBit();
            casterGuid[4] = packet.ReadBit();
            targetGuid[1] = packet.ReadBit();

            packet.ReadXORByte(targetGuid, 4);

            if (isDebug2)
                packet.ReadSingle("Unk Float [DEBUG2]");

            packet.ReadXORByte(casterGuid, 6);
            packet.ReadUInt32("Absorb");

            if (hasPowerData)
            {
                packet.ReadUInt32("Unk UInt32");
                packet.ReadUInt32("Current Health");
                packet.ReadUInt32("Unk UInt32");
                packet.ReadXORByte(playerPowerGuid, 3);

                for (int i = 0; i < powerCount; ++i)
                {
                    packet.ReadEnum<PowerType>("Power type", TypeCode.UInt32, i);
                    packet.ReadUInt32("Power Amount", i);
                }

                packet.ReadXORByte(playerPowerGuid, 0);
                packet.ReadXORByte(playerPowerGuid, 1);
                packet.ReadXORByte(playerPowerGuid, 7);
                packet.ReadXORByte(playerPowerGuid, 2);
                packet.ReadXORByte(playerPowerGuid, 4);
                packet.ReadXORByte(playerPowerGuid, 6);
                packet.ReadXORByte(playerPowerGuid, 5);

                packet.WriteGuid("Player Power GUID", playerPowerGuid);
            }

            packet.ReadXORByte(targetGuid, 6);
            packet.ReadXORByte(targetGuid, 0);
            packet.ReadXORByte(casterGuid, 1);
            packet.ReadXORByte(casterGuid, 3);
            packet.ReadXORByte(casterGuid, 6);
            packet.ReadXORByte(targetGuid, 5);
            packet.ReadXORByte(casterGuid, 0);
            packet.ReadXORByte(targetGuid, 7);
            packet.ReadUInt32("HealAmount");
            packet.ReadXORByte(casterGuid, 5);
            packet.ReadXORByte(targetGuid, 2);
            packet.ReadXORByte(casterGuid, 2);

            if (isDebug)
                packet.ReadSingle("Unk Float [DEBUG]");

            packet.ReadXORByte(targetGuid, 3);
            packet.ReadXORByte(casterGuid, 4);
            packet.ReadUInt32("Spell ID");
            packet.ReadXORByte(targetGuid, 1);
            packet.ReadUInt32("OverHeal");

            packet.WriteGuid("Target GUID", targetGuid);
            packet.WriteGuid("Caster GUID", casterGuid);
        }

        [Parser(Opcode.SMSG_SPELLENERGIZELOG)]
        public static void HandleSpellEnergizeLog(Packet packet)
        {
            var targetGuid = new byte[8];
            var casterGuid = new byte[8];
            var playerPowerGuid = new byte[8];

            casterGuid[2] = packet.ReadBit();
            casterGuid[5] = packet.ReadBit();
            casterGuid[0] = packet.ReadBit();
            casterGuid[1] = packet.ReadBit();
            targetGuid[1] = packet.ReadBit();
            casterGuid[4] = packet.ReadBit();
            var hasPowerData = packet.ReadBit("hasPowerData");
            targetGuid[0] = packet.ReadBit();

            var powerCount = 0;
            if (hasPowerData)
            {
                playerPowerGuid[1] = packet.ReadBit();
                playerPowerGuid[0] = packet.ReadBit();
                playerPowerGuid[2] = packet.ReadBit();
                playerPowerGuid[5] = packet.ReadBit();
                powerCount = (int)packet.ReadBits("powerCount", 21);
                playerPowerGuid[7] = packet.ReadBit();
                playerPowerGuid[3] = packet.ReadBit();
                playerPowerGuid[4] = packet.ReadBit();
                playerPowerGuid[6] = packet.ReadBit();
            }

            targetGuid[3] = packet.ReadBit();
            targetGuid[5] = packet.ReadBit();
            casterGuid[6] = packet.ReadBit();
            targetGuid[4] = packet.ReadBit();
            targetGuid[2] = packet.ReadBit();
            targetGuid[7] = packet.ReadBit();
            casterGuid[3] = packet.ReadBit();
            targetGuid[6] = packet.ReadBit();
            casterGuid[7] = packet.ReadBit();

            if (hasPowerData)
            {
                packet.ReadUInt32("Unk UInt32");
                packet.ReadUInt32("Current Health");
                packet.ReadXORByte(playerPowerGuid, 6);
                packet.ReadXORByte(playerPowerGuid, 0);
                packet.ReadXORByte(playerPowerGuid, 1);
                packet.ReadXORByte(playerPowerGuid, 2);
                packet.ReadXORByte(playerPowerGuid, 7);
                packet.ReadXORByte(playerPowerGuid, 5);
                packet.ReadXORByte(playerPowerGuid, 3);

                for (int i = 0; i < powerCount; ++i)
                {
                    packet.ReadUInt32("Power Amount", i);
                    packet.ReadEnum<PowerType>("Power type", TypeCode.UInt32, i);
                }

                packet.ReadXORByte(playerPowerGuid, 4);
                packet.ReadUInt32("Unk UInt32");

                packet.WriteGuid("Player Power GUID", playerPowerGuid);
            }

            packet.ReadXORByte(targetGuid, 3);
            packet.ReadUInt32("Amount");
            packet.ReadXORByte(casterGuid, 4);
            packet.ReadXORByte(casterGuid, 5);
            packet.ReadXORByte(casterGuid, 2);
            packet.ReadXORByte(targetGuid, 0);
            packet.ReadXORByte(targetGuid, 6);
            packet.ReadXORByte(casterGuid, 7);
            packet.ReadXORByte(casterGuid, 6);
            packet.ReadUInt32("Spell ID");
            packet.ReadXORByte(casterGuid, 3);
            packet.ReadEnum<PowerType>("Power type", TypeCode.UInt32);
            packet.ReadXORByte(targetGuid, 7);
            packet.ReadXORByte(targetGuid, 2);
            packet.ReadXORByte(targetGuid, 4);
            packet.ReadXORByte(targetGuid, 1);
            packet.ReadXORByte(casterGuid, 1);
            packet.ReadXORByte(targetGuid, 5);
            packet.ReadXORByte(casterGuid, 0);

            packet.WriteGuid("Target GUID", targetGuid);
            packet.WriteGuid("Caster GUID", casterGuid);
        }

        [Parser(Opcode.SMSG_SPELLLOGMISS)]
        public static void HandleSpellLogMiss(Packet packet)
        {
            ReadSpellMissLog(ref packet);
        }

        [Parser(Opcode.SMSG_SPELLLOGEXECUTE)]
        public static void HandleSpellLogExecute(Packet packet)
        {
            ReadSpellLogExecute(ref packet);
        }

        // Unknown opcode name(s)
        private static void ReadSpellRemoveLog(ref Packet packet, int index = -1)
        {
            var casterGuid = new byte[8];
            var targetGuid = new byte[8];
            var unkGuid = new byte[8];

            casterGuid[5] = packet.ReadBit();
            targetGuid[5] = packet.ReadBit();
            targetGuid[4] = packet.ReadBit();
            casterGuid[2] = packet.ReadBit();
            targetGuid[0] = packet.ReadBit();
            casterGuid[6] = packet.ReadBit();
            var bit80 = packet.ReadBit("bit80");
            targetGuid[6] = packet.ReadBit();

            var counter = 0;
            if (bit80)
            {
                unkGuid[1] = packet.ReadBit();
                unkGuid[6] = packet.ReadBit();
                unkGuid[3] = packet.ReadBit();
                counter = (int)packet.ReadBits("counter", 21);
                unkGuid[2] = packet.ReadBit();
                unkGuid[0] = packet.ReadBit();
                unkGuid[7] = packet.ReadBit();
                unkGuid[4] = packet.ReadBit();
                unkGuid[5] = packet.ReadBit();
            }

            casterGuid[4] = packet.ReadBit();
            casterGuid[1] = packet.ReadBit();
            targetGuid[2] = packet.ReadBit();
            casterGuid[0] = packet.ReadBit();
            targetGuid[3] = packet.ReadBit();
            casterGuid[1] = packet.ReadBit();
            targetGuid[7] = packet.ReadBit();
            casterGuid[7] = packet.ReadBit();

            packet.ReadXORByte(casterGuid, 0);
            packet.ReadXORByte(casterGuid, 4);
            packet.ReadXORByte(targetGuid, 3);
            packet.ReadXORByte(casterGuid, 3);

            if (bit80)
            {
                packet.ReadXORByte(unkGuid, 5);
                packet.ReadUInt32("Unk UInt32");
                packet.ReadXORByte(unkGuid, 1);
                packet.ReadXORByte(unkGuid, 7);
                packet.ReadUInt32("Unk UInt32");
                packet.ReadXORByte(unkGuid, 6);
                packet.ReadUInt32("Unk UInt32");
                packet.ReadXORByte(unkGuid, 4);
                packet.ReadXORByte(unkGuid, 0);

                for (int i = 0; i < counter; ++i)
                {
                    packet.ReadUInt32("Unk UInt32", i);
                    packet.ReadUInt32("Unk UInt32", i);
                }

                packet.ReadXORByte(unkGuid, 2);
                packet.ReadXORByte(unkGuid, 3);

                packet.WriteGuid("Unk GUID", unkGuid);
            }

            packet.ReadUInt32("Unk UInt32");
            packet.ReadXORByte(targetGuid, 4);
            packet.ReadXORByte(casterGuid, 1);
            packet.ReadXORByte(targetGuid, 7);
            packet.ReadXORByte(targetGuid, 0);
            packet.ReadXORByte(casterGuid, 5);
            packet.ReadUInt32("Unk UInt32");
            packet.ReadXORByte(targetGuid, 1);
            packet.ReadXORByte(targetGuid, 5);
            packet.ReadXORByte(casterGuid, 7);
            packet.ReadXORByte(targetGuid, 6);
            packet.ReadXORByte(casterGuid, 6);
            packet.ReadXORByte(casterGuid, 2);
            packet.ReadXORByte(targetGuid, 2);

            packet.WriteGuid("Caster GUID", casterGuid);
            packet.WriteGuid("Target GUID", targetGuid);

            return;



            packet.ReadPackedGuid("Target GUID", index);
            packet.ReadPackedGuid("Caster GUID", index); // Can be 0
            packet.ReadEntryWithName<Int32>(StoreNameType.Spell, "Spell", index); // Can be 0
            var debug = packet.ReadBoolean("Debug Output", index);
            var count = packet.ReadInt32("Count", index);

            for (int i = 0; i < count; i++)
            {
                packet.ReadEntryWithName<Int32>(StoreNameType.Spell, "Spell", index, i);
                packet.ReadByte("Unknown Byte/Bool", index, i);
            }

            if (debug)
            {
                packet.ReadInt32("Unk int32");
                packet.ReadInt32("Unk int32");
            }
        }

        private static void ReadSpellLogExecute(ref Packet packet, int index = -1)
        {
            packet.ReadPackedGuid("Caster GUID", index);
            packet.ReadEntryWithName<Int32>(StoreNameType.Spell, "Spell ID", index);
            var count = packet.ReadInt32("Count", index); // v47

            for (int i = 0; i < count; i++)
            {
                var type = packet.ReadEnum<SpellEffect>("Spell Effect", TypeCode.Int32, index, i);
                var count2 = packet.ReadInt32("Count", index, i);
                for (int j = 0; j < count2; j++)
                {
                    switch (type)
                    {
                        case SpellEffect.PowerDrain:
                        case SpellEffect.PowerBurn:
                        {
                            packet.ReadPackedGuid("Target GUID", index, i, j);
                            packet.ReadInt32("Power taken", index, i, j);
                            packet.ReadInt32("Power type", index, i, j);
                            packet.ReadSingle("Multiplier", index, i, j);
                            break;
                        }
                        case SpellEffect.AddExtraAttacks:
                        {
                            packet.ReadPackedGuid("Target GUID", index, i, j);
                            packet.ReadInt32("Amount", index, i, j);
                            break;
                        }
                        case SpellEffect.InterruptCast:
                        {
                            packet.ReadPackedGuid("Target GUID", index, i, j);
                            packet.ReadEntryWithName<Int32>(StoreNameType.Spell, "Interrupted Spell ID", index, i, j);
                            break;
                        }
                        case SpellEffect.DurabilityDamage:
                        {
                            packet.ReadPackedGuid("Target GUID", index, i, j);
                            packet.ReadEntryWithName<Int32>(StoreNameType.Item, "Item", index, i, j);
                            packet.ReadInt32("Slot", index, i, j);
                            break;
                        }
                        case SpellEffect.OpenLock:
                        {
                            packet.ReadPackedGuid("Target", i, j);
                            break;
                        }
                        case SpellEffect.CreateItem:
                        case SpellEffect.CreateRandomItem:
                        case SpellEffect.CreateItem2:
                        {
                            packet.ReadEntryWithName<Int32>(StoreNameType.Item, "Created Item", index, i, j);
                            break;
                        }
                        case SpellEffect.Summon:
                        case SpellEffect.TransDoor:
                        case SpellEffect.SummonPet:
                        case SpellEffect.SummonObjectWild:
                        case SpellEffect.CreateHouse:
                        case SpellEffect.Duel:
                        case SpellEffect.SummonObjectSlot1:
                        case SpellEffect.SummonObjectSlot2:
                        case SpellEffect.SummonObjectSlot3:
                        case SpellEffect.SummonObjectSlot4:
                        case SpellEffect.Unk171:
                        {
                            var guid = packet.ReadPackedGuid("Summoned GUID", index, i, j);

                            WoWObject obj;
                            if (Storage.Objects.TryGetValue(guid, out obj))
                                obj.ForceTemporarySpawn = true;

                            break;
                        }
                        case SpellEffect.FeedPet:
                        {
                            packet.ReadInt32("Unknown Int32", index, i, j);
                            break;
                        }
                        case SpellEffect.DismissPet:
                        {
                            packet.ReadPackedGuid("GUID", index, i, j);
                            break;
                        }
                        case SpellEffect.Resurrect:
                        case SpellEffect.ResurrectNew:
                        case SpellEffect.RessurectAOE:
                        {
                            packet.ReadPackedGuid("GUID", index, i, j);
                            break;
                        }
                        default:
                            throw new InvalidDataException("Unknown Spell Effect: " + type);
                    }
                }
            }
        }

        private static void ReadPeriodicAuraLog(ref Packet packet, int index = -1)
        {
            packet.ReadPackedGuid("Target GUID", index);
            packet.ReadPackedGuid("Caster GUID", index);
            packet.ReadEntryWithName<Int32>(StoreNameType.Spell, "Spell ID", index);
            var count = packet.ReadInt32("Count", index);

            for (var i = 0; i < count; i++)
            {
                var aura = packet.ReadEnum<AuraType>("Aura Type", TypeCode.UInt32, index);
                switch (aura)
                {
                    case AuraType.PeriodicDamage:
                    case AuraType.PeriodicDamagePercent:
                    {
                        packet.ReadUInt32("Damage", index);

                        if (ClientVersion.AddedInVersion(ClientVersionBuild.V3_0_2_9056))
                            packet.ReadUInt32("Over damage", index);

                        packet.ReadUInt32("Spell Proto", index);
                        packet.ReadUInt32("Absorb", index);
                        packet.ReadUInt32("Resist", index);

                        if (ClientVersion.AddedInVersion(ClientVersionBuild.V3_1_2_9901))
                            packet.ReadByte("Critical", index);

                        break;
                    }
                    case AuraType.PeriodicHeal:
                    case AuraType.ObsModHealth:
                    {
                        packet.ReadUInt32("Damage", index);

                        if (ClientVersion.AddedInVersion(ClientVersionBuild.V3_0_2_9056))
                            packet.ReadUInt32("Over damage", index);

                        if (ClientVersion.AddedInVersion(ClientVersionBuild.V3_3_0_10958))
                            // no idea when this was added exactly
                            packet.ReadUInt32("Absorb", index);

                        if (ClientVersion.AddedInVersion(ClientVersionBuild.V3_1_2_9901))
                            packet.ReadByte("Critical", index);

                        break;
                    }
                    case AuraType.ObsModPower:
                    case AuraType.PeriodicEnergize:
                    {
                        packet.ReadEnum<PowerType>("Power type", TypeCode.Int32, index);
                        packet.ReadUInt32("Amount", index);
                        break;
                    }
                    case AuraType.PeriodicManaLeech:
                    {
                        packet.ReadEnum<PowerType>("Power type", TypeCode.Int32, index);
                        packet.ReadUInt32("Amount", index);
                        packet.ReadSingle("Gain multiplier", index);
                        break;
                    }
                }
            }
        }

        private static void ReadSpellNonMeleeDamageLog(ref Packet packet, int index = -1)
        {
            var caster = new byte[8];
            var target = new byte[8];
            var caster2 = new byte[8];

            packet.ReadUInt32("Damage");
            packet.ReadUInt32("Resist");
            packet.ReadEnum<SpellHitType>("HitType", TypeCode.Int32, index);
            packet.ReadUInt32("Blocked");
            packet.ReadEntryWithName<UInt32>(StoreNameType.Spell, "Spell ID", index);
            packet.ReadUInt32("Overkill");
            packet.ReadUInt32("Absorb");
            packet.ReadEnum<SpellSchoolMask>("SpellSchoolMask", TypeCode.Byte, index);

            // Must send 0 ?
            var debugFlag = packet.ReadBit() != 0;
            Console.WriteLine("Debug Flag: " + debugFlag);

            target[1] = packet.ReadBit();
            target[7] = packet.ReadBit();
            caster[1] = packet.ReadBit();

            var table = new byte[10];
            if (debugFlag)
            {
                table[7] = packet.ReadBit();
                table[0] = packet.ReadBit();
                table[3] = packet.ReadBit();
                table[5] = packet.ReadBit();
                table[4] = packet.ReadBit();
                table[9] = packet.ReadBit();
                table[2] = packet.ReadBit();
                table[1] = packet.ReadBit();
                table[6] = packet.ReadBit();
                table[8] = packet.ReadBit();
            }

            var bit68 = packet.ReadBit() != 0;
            Console.WriteLine("bit 68: " + bit68);

            caster[5] = packet.ReadBit();
            caster[0] = packet.ReadBit();
            caster[3] = packet.ReadBit();
            caster[6] = packet.ReadBit();
            caster[2] = packet.ReadBit();
            target[6] = packet.ReadBit();
            target[5] = packet.ReadBit();
            target[4] = packet.ReadBit();

            var hasGuid = packet.ReadBit() != 0;
            Console.WriteLine("hasGuid: " + hasGuid);

            uint counter = 0;
            if (hasGuid)
            {
                caster2[1] = packet.ReadBit();
                caster2[5] = packet.ReadBit();
                caster2[7] = packet.ReadBit();
                caster2[4] = packet.ReadBit();
                caster2[6] = packet.ReadBit();
                caster2[3] = packet.ReadBit();

                counter = packet.ReadBits("Counter", 21);

                caster2[2] = packet.ReadBit();
                caster2[0] = packet.ReadBit();
            }

            caster[7] = packet.ReadBit();
            target[3] = packet.ReadBit();
            caster[4] = packet.ReadBit();
            target[0] = packet.ReadBit();
            target[2] = packet.ReadBit();

            var bit80 = packet.ReadBit() != 0;
            Console.WriteLine("bit80: " + bit80);

            if (debugFlag)
            {
                packet.ReadSingle("Unk float 6 1");
                packet.ReadSingle("Unk float 6 2");
                packet.ReadSingle("Unk float 6 3");
                packet.ReadSingle("Unk float 6 4");
                packet.ReadSingle("Unk float 6 5");
                packet.ReadSingle("Unk float 6 6");
                packet.ReadSingle("Unk float 6 7");
                packet.ReadSingle("Unk float 6 8");
                packet.ReadSingle("Unk float 6 9");
                packet.ReadSingle("Unk float 6 10");
            }

            packet.ReadXORByte(target, 7);

            if (hasGuid)
            {
                packet.ReadXORByte(caster2, 3);

                packet.ReadUInt32("Caster's Attack Power");

                packet.ReadXORByte(caster2, 5);
                packet.ReadXORByte(caster2, 1);
                packet.ReadXORByte(caster2, 7);

                for (int i = 0; i < counter; i++)
                {
                    packet.ReadUInt32("Caster's Mana");
                    packet.ReadUInt32("Caster's Unk Power");
                }

                packet.ReadXORByte(caster2, 4);
                packet.ReadXORByte(caster2, 0);

                packet.ReadUInt32("Caster's Health");

                packet.ReadXORByte(caster2, 6);

                packet.ReadUInt32("Caster's Base Spell Power");

                packet.ReadXORByte(caster2, 2);
            }

            packet.ReadXORByte(caster, 2);
            packet.ReadXORByte(caster, 1);
            packet.ReadXORByte(target, 1);
            packet.ReadXORByte(caster, 4);
            packet.ReadXORByte(caster, 7);
            packet.ReadXORByte(target, 6);
            packet.ReadXORByte(caster, 5);
            packet.ReadXORByte(target, 2);
            packet.ReadXORByte(target, 3);
            packet.ReadXORByte(caster, 6);
            packet.ReadXORByte(target, 0);
            packet.ReadXORByte(target, 4);
            packet.ReadXORByte(target, 5);
            packet.ReadXORByte(caster, 3);
            packet.ReadXORByte(caster, 0);

            packet.WriteGuid("caster GUID", caster);
            packet.WriteGuid("target GUID", target);
            packet.WriteGuid("caster2 GUID", caster2);

            /*
            packet.ReadPackedGuid("Target GUID", index);
            packet.ReadPackedGuid("Caster GUID", index);
            packet.ReadUInt32("Damage", index);

            if (ClientVersion.AddedInVersion(ClientVersionBuild.V3_0_3_9183))
                packet.ReadInt32("Overkill", index);

            packet.ReadByte("SchoolMask", index);
            packet.ReadUInt32("Absorb", index);
            packet.ReadUInt32("Resist", index);
            packet.ReadBoolean("Show spellname in log", index);
            packet.ReadByte("Unk byte", index);
            packet.ReadUInt32("Blocked", index);
            var type = packet.ReadEnum<SpellHitType>("HitType", TypeCode.Int32, index);

            if (packet.ReadBoolean("Debug output", index))
            {
                if (!type.HasAnyFlag(SpellHitType.SPELL_HIT_TYPE_UNK4))
                {
                    if (type.HasAnyFlag(SpellHitType.SPELL_HIT_TYPE_UNK1))
                    {
                        packet.ReadSingle("Unk float 1 1");
                        packet.ReadSingle("Unk float 1 2");
                    }

                    if (type.HasAnyFlag(SpellHitType.SPELL_HIT_TYPE_UNK3))
                    {
                        packet.ReadSingle("Unk float 3 1");
                        packet.ReadSingle("Unk float 3 2");
                    }

                    if (type.HasAnyFlag(SpellHitType.SPELL_HIT_TYPE_UNK6))
                    {
                        packet.ReadSingle("Unk float 6 1");
                        packet.ReadSingle("Unk float 6 2");
                        packet.ReadSingle("Unk float 6 3");
                        packet.ReadSingle("Unk float 6 4");
                        packet.ReadSingle("Unk float 6 5");
                        packet.ReadSingle("Unk float 6 6");
                    }
                }
            }
            */
        }

        private static void ReadSpellHealLog(ref Packet packet, int index = -1)
        {
            packet.ReadPackedGuid("Target GUID", index);
            packet.ReadPackedGuid("Caster GUID", index);
            packet.ReadEntryWithName<UInt32>(StoreNameType.Spell, "Spell ID", index);
            packet.ReadUInt32("Damage", index);

            if (ClientVersion.AddedInVersion(ClientVersionBuild.V3_0_3_9183))
                packet.ReadUInt32("Overheal", index);

            if (ClientVersion.AddedInVersion(ClientVersionBuild.V3_0_3_9183)) // no idea when this was added exactly
                packet.ReadUInt32("Absorb", index);

            packet.ReadBoolean("Critical", index);

            if (packet.ReadBoolean("Debug output", index))
            {
                packet.ReadSingle("Unk float", index);
                packet.ReadSingle("Unk float 2", index);
            }
        }

        private static void ReadSpellEnergizeLog(ref Packet packet, int index = -1)
        {
            packet.ReadPackedGuid("Target GUID", index);
            packet.ReadPackedGuid("Caster GUID", index);
            packet.ReadEntryWithName<UInt32>(StoreNameType.Spell, "Spell ID", index);
            packet.ReadEnum<PowerType>("Power type", TypeCode.UInt32, index);
            packet.ReadUInt32("Amount", index);
        }

        private static void ReadSpellMissLog(ref Packet packet, int index = -1)
        {
            packet.ReadEntryWithName<UInt32>(StoreNameType.Spell, "Spell ID", index);
            packet.ReadGuid("Caster GUID", index);
            var debug = packet.ReadBoolean("Debug output", index);

            var count = packet.ReadUInt32("Target Count", index);
            for (var i = 0; i < count; ++i)
            {
                packet.ReadGuid("Target GUID", index);
                packet.ReadEnum<SpellMissType>("Miss Info", TypeCode.Byte, index);
                if (debug)
                {
                    packet.ReadSingle("Unk float");
                    packet.ReadSingle("Unk float");
                }
            }
        }

        [Parser(Opcode.SMSG_SPELLDAMAGESHIELD)]
        public static void ReadSpellDamageShield(Packet packet)
        {
            packet.ReadGuid("Victim");
            packet.ReadGuid("Caster");
            packet.ReadEntryWithName<UInt32>(StoreNameType.Spell, "Spell Id");
            packet.ReadInt32("Damage");
            packet.ReadInt32("Overkill");
            packet.ReadInt32("SpellSchoolMask");

            if (ClientVersion.AddedInVersion(ClientVersionBuild.V4_0_6_13596))
                packet.ReadInt32("Resisted Damage");
        }

        [Parser(Opcode.SMSG_SPELL_BATTLE_PET_REVIVE)]
        public static void ReadSpellUnknow(Packet packet)
        {
            var guid = new byte[8];

            var count = packet.ReadBits("count", 21);
            var bit40 = !packet.ReadBit();
            Console.WriteLine("bit40: " + bit40);
            packet.StartBitStream(guid, 4, 2, 5, 6, 0, 3, 7, 1);

            for (int i = 0; i < count; ++i)
            {
                packet.ReadUInt32("Spell ID", i);   // 125439 - Revive Battle Pets
                packet.ReadUInt32("Unk UInt32", i); // 180000
            }

            packet.ReadXORByte(guid, 4);
            if (bit40)
                packet.ReadByte("Unk Byte");

            packet.ParseBitStream(guid, 1, 5, 7, 6, 0, 2, 3);

            packet.WriteGuid("Guid", guid);
        }

        [Parser(Opcode.SMSG_SPELL_UNK_LOG)]
        public static void ReadSpellUnkLog(Packet packet)
        {
            var targetGuid = new byte[8];
            var casterGuid = new byte[8];
            var playerPowerGuid = new byte[8];

            casterGuid[2] = packet.ReadBit();
            targetGuid[2] = packet.ReadBit();
            targetGuid[1] = packet.ReadBit();
            var bit32 = packet.ReadBit("bit32");
            casterGuid[0] = packet.ReadBit();
            casterGuid[5] = packet.ReadBit();
            var bit108 = packet.ReadBit("bit108");
            targetGuid[7] = packet.ReadBit();
            targetGuid[5] = packet.ReadBit();
            casterGuid[7] = packet.ReadBit();
            targetGuid[0] = packet.ReadBit();
            targetGuid[4] = packet.ReadBit();
            casterGuid[1] = packet.ReadBit();
            targetGuid[3] = packet.ReadBit();
            casterGuid[3] = packet.ReadBit();
            targetGuid[6] = packet.ReadBit();
            var hasPowerData = packet.ReadBit("hasPowerData");
            var counter = packet.ReadBits("counter", 22);

            var powerCount = 0;
            if (hasPowerData)
            {
                playerPowerGuid[2] = packet.ReadBit();
                playerPowerGuid[5] = packet.ReadBit();
                playerPowerGuid[7] = packet.ReadBit();
                playerPowerGuid[6] = packet.ReadBit();
                powerCount = (int)packet.ReadBits("powerCount", 21);
                playerPowerGuid[4] = packet.ReadBit();
                playerPowerGuid[3] = packet.ReadBit();
                playerPowerGuid[0] = packet.ReadBit();
                playerPowerGuid[1] = packet.ReadBit();
            }

            casterGuid[6] = packet.ReadBit();
            casterGuid[4] = packet.ReadBit();

            var bit4 = new bool[counter];
            var bit12 = new bool[counter];
            var bit20 = new bool[counter];
            for (int i = 0; i < counter; ++i)
            {
                bit12[i] = packet.ReadBit("bit12", i);
                bit20[i] = packet.ReadBit("bit20", i);
                bit4[i] = packet.ReadBit("bit4", i);
            }

            for (int i = 0; i < counter; ++i)
            {
                if (bit12[i])
                    packet.ReadUInt32("Unk UInt32 [12]", i);

                if (bit20[i])
                    packet.ReadUInt32("Unk UInt32 [20]", i);

                packet.ReadUInt32("Spell ID");
            }

            if (hasPowerData)
            {
                packet.ReadXORByte(playerPowerGuid, 3);
                packet.ReadUInt32("Unk UInt32");
                packet.ReadUInt32("Current Health");
                packet.ReadXORByte(playerPowerGuid, 6);
                packet.ReadXORByte(playerPowerGuid, 7);

                for (int i = 0; i < powerCount; ++i)
                {
                    packet.ReadUInt32("Power Amount", i);
                    packet.ReadEnum<PowerType>("Power type", TypeCode.UInt32, i);
                }

                packet.ReadUInt32("Unk UInt32");
                packet.ReadXORByte(playerPowerGuid, 2);
                packet.ReadXORByte(playerPowerGuid, 4);
                packet.ReadXORByte(playerPowerGuid, 1);
                packet.ReadXORByte(playerPowerGuid, 5);
                packet.ReadXORByte(playerPowerGuid, 0);

                packet.WriteGuid("Player Power GUID", playerPowerGuid);
            }

            packet.ReadUInt32("Unk UInt32");
            packet.ReadXORByte(targetGuid, 0);
            packet.ReadXORByte(casterGuid, 5);
            packet.ReadXORByte(casterGuid, 6);
            packet.ReadXORByte(casterGuid, 4);
            packet.ReadXORByte(targetGuid, 3);
            packet.ReadXORByte(casterGuid, 3);
            packet.ReadXORByte(targetGuid, 4);
            packet.ReadXORByte(targetGuid, 5);
            packet.ReadXORByte(targetGuid, 7);
            packet.ReadXORByte(casterGuid, 2);
            packet.ReadXORByte(targetGuid, 2);
            packet.ReadXORByte(casterGuid, 0);
            packet.ReadXORByte(targetGuid, 1);
            packet.ReadXORByte(targetGuid, 6);
            packet.ReadXORByte(casterGuid, 1);
            packet.ReadXORByte(casterGuid, 7);

            packet.WriteGuid("Caster GUID", casterGuid);
            packet.WriteGuid("Target GUID", targetGuid);
        }

        [Parser(Opcode.SMSG_SPELL_UNK_LOG2)]
        public static void ReadSpellUnkLog2(Packet packet)
        {
            var targetGuid = new byte[8];
            var casterGuid = new byte[8];

            targetGuid[2] = packet.ReadBit();
            casterGuid[1] = packet.ReadBit();
            var bit40 = packet.ReadBit("bit40");
            targetGuid[1] = packet.ReadBit();
            casterGuid[4] = packet.ReadBit();
            casterGuid[0] = packet.ReadBit();
            targetGuid[3] = packet.ReadBit();
            casterGuid[2] = packet.ReadBit();
            casterGuid[3] = packet.ReadBit();
            targetGuid[5] = packet.ReadBit();
            targetGuid[7] = packet.ReadBit();
            casterGuid[6] = packet.ReadBit();
            targetGuid[0] = packet.ReadBit();
            targetGuid[6] = packet.ReadBit();
            casterGuid[5] = packet.ReadBit();
            targetGuid[4] = packet.ReadBit();
            casterGuid[7] = packet.ReadBit();

            packet.ReadXORByte(casterGuid, 0);
            packet.ReadXORByte(casterGuid, 5);
            packet.ReadXORByte(casterGuid, 1);
            packet.ReadXORByte(targetGuid, 1);
            packet.ReadXORByte(targetGuid, 5);
            packet.ReadXORByte(casterGuid, 6);

            if (bit40)
            {
                packet.ReadUInt32("Unk UInt32");
                packet.ReadUInt32("Unk UInt32");
            }

            packet.ReadUInt32("Unk UInt32");
            packet.ReadUInt32("Unk UInt32");
            packet.ReadXORByte(targetGuid, 0);
            packet.ReadXORByte(targetGuid, 2);
            packet.ReadXORByte(casterGuid, 2);
            packet.ReadXORByte(casterGuid, 3);
            packet.ReadXORByte(casterGuid, 7);
            packet.ReadXORByte(targetGuid, 3);
            packet.ReadXORByte(targetGuid, 4);
            packet.ReadXORByte(targetGuid, 7);
            packet.ReadUInt32("Unk UInt32");
            packet.ReadXORByte(casterGuid, 4);
            packet.ReadXORByte(targetGuid, 6);

            packet.WriteGuid("Caster GUID", casterGuid);
            packet.WriteGuid("Target GUID", targetGuid);
        }
    }
}
