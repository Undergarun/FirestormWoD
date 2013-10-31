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
            ReadSpellRemoveLog(ref packet);
        }

        [Parser(Opcode.SMSG_PERIODICAURALOG)]
        public static void HandlePeriodicAuraLog(Packet packet)
        {
            ReadPeriodicAuraLog(ref packet);
        }

        [Parser(Opcode.SMSG_SPELLNONMELEEDAMAGELOG)]
        public static void HandleSpellNonMeleeDmgLog(Packet packet)
        {
            ReadSpellNonMeleeDamageLog(ref packet);
        }

        [Parser(Opcode.SMSG_SPELLHEALLOG)]
        public static void HandleSpellHealLog(Packet packet)
        {
            ReadSpellHealLog(ref packet);
        }

        [Parser(Opcode.SMSG_SPELLENERGIZELOG)]
        public static void HandleSpellEnergizeLog(Packet packet)
        {
            ReadSpellEnergizeLog(ref packet);
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

            // Missed abosrb, resist, blocked
            packet.ReadUInt32("Damage");
            packet.ReadUInt32("Unk uint32");
            packet.ReadEnum<SpellHitType>("HitType", TypeCode.Int32, index);
            packet.ReadUInt32("Unk uint32");
            packet.ReadEntryWithName<UInt32>(StoreNameType.Spell, "Spell ID", index);
            packet.ReadUInt32("Overkill");
            packet.ReadUInt32("Unk uint32");
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
    }
}
