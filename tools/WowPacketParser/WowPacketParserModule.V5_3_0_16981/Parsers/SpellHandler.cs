using System;
using System.Text;
using System.Collections.Generic;
using WowPacketParser.Enums;
using WowPacketParser.Misc;
using WowPacketParser.Store;
using WowPacketParser.Store.Objects;
using WowPacketParser.Parsing;
using CoreParsers = WowPacketParser.Parsing.Parsers;
using Guid = WowPacketParser.Misc.Guid;

namespace WowPacketParserModule.V5_3_0_16981.Parsers
{
    public static class SpellHandler
    {
        [HasSniffData]
        [Parser(Opcode.SMSG_AURA_UPDATE)]
        public static void HandleAuraUpdate(Packet packet)
        {
            var guid = new byte[8];
            var guid2 = new byte[8];

            guid[1] = packet.ReadBit();
            var hasPowerData = packet.ReadBit();

            uint bits3C = 0;
            if (hasPowerData)
            {
                packet.StartBitStream(guid2, 1, 5, 6);
                bits3C = packet.ReadBits(21);
                packet.StartBitStream(guid2, 2, 3, 7, 0, 4);
            }

            var bits4 = packet.ReadBits(24);
            guid[6] = packet.ReadBit();

            var hasAura = new bool[bits4];
            var hasCasterGUID = new bool[bits4];
            var hasDuration = new bool[bits4];
            var hasMaxDuration = new bool[bits4];
            var effectCount = new uint[bits4];
            var casterGUID = new byte[bits4][];
            for (var i = 0; i < bits4; ++i)
            {
                hasAura[i] = packet.ReadBit();
                if (hasAura[i])
                {
                    hasMaxDuration[i] = packet.ReadBit();
                    effectCount[i] = packet.ReadBits(22);
                    hasCasterGUID[i] = packet.ReadBit();
                    if (hasCasterGUID[i])
                    {
                        casterGUID[i] = new byte[8];
                        packet.StartBitStream(casterGUID[i], 5, 7, 4, 1, 6, 0, 3, 2);
                    }
                    hasDuration[i] = packet.ReadBit();
                }
            }
            packet.StartBitStream(guid, 2, 3, 7, 4);
            packet.ReadBit("Is AURA_UPDATE_ALL");
            packet.StartBitStream(guid, 0, 5);
            packet.ResetBitReader();

            var auras = new List<Aura>();
            for (var i = 0; i < bits4; ++i)
            {
                if (hasAura[i])
                {
                    var aura = new Aura();
                    if (hasCasterGUID[i])
                    {
                        packet.ParseBitStream(casterGUID[i], 2, 7, 6, 1, 4, 0, 5, 3);
                        packet.WriteGuid("Caster GUID", casterGUID[i], i);
                        aura.CasterGuid = new Guid(BitConverter.ToUInt64(casterGUID[i], 0));
                    }
                    else
                        aura.CasterGuid = new Guid();

                    aura.Charges = packet.ReadByte("Charges", i);

                    if (hasDuration[i])
                        aura.Duration = packet.ReadInt32("Duration", i);
                    else
                        aura.Duration = 0;

                    aura.SpellId = packet.ReadUInt32("Spell Id", i);
                    aura.AuraFlags = packet.ReadEnum<AuraFlagMoP>("Flags", TypeCode.Byte, i);

                    for (var j = 0; j < effectCount[i]; ++j)
                        packet.ReadSingle("Effect Value", i, j);
                    packet.ReadInt32("Effect Mask", i);

                    if (hasMaxDuration[i])
                        aura.MaxDuration = packet.ReadInt32("Max Duration", i);
                    else
                        aura.MaxDuration = 0;

                    aura.Level = packet.ReadUInt16("Caster Level", i);
                    auras.Add(aura);
                    packet.AddSniffData(StoreNameType.Spell, (int)aura.SpellId, "AURA_UPDATE");
                }
                packet.ReadByte("Slot", i);
            }

            if (hasPowerData)
            {
                packet.ReadXORBytes(guid2, 7, 0);
                for (var i = 0; i < bits3C; ++i)
                {
                    packet.ReadEnum<PowerType>("Power Type", TypeCode.UInt32, i);
                    packet.ReadInt32("Power Value", i);
                }
                packet.ReadXORBytes(guid2, 2, 5);
                packet.ReadInt32("Attack power");
                packet.ReadInt32("Spell power");
                packet.ReadXORBytes(guid2, 6, 4, 3, 1);
                packet.ReadInt32("Current Health");
                packet.WriteGuid("PowerUnitGUID", guid2);
            }
            packet.ParseBitStream(guid, 0, 5, 7, 2, 1, 4, 3, 6);
            packet.WriteGuid("Guid", guid);

            var GUID = new Guid(BitConverter.ToUInt64(guid, 0));
            if (Storage.Objects.ContainsKey(GUID))
            {
                var unit = Storage.Objects[GUID].Item1 as Unit;
                if (unit != null)
                {
                    // If this is the first packet that sends auras
                    // (hopefully at spawn time) add it to the "Auras" field,
                    // if not create another row of auras in AddedAuras
                    // (similar to ChangedUpdateFields)

                    if (unit.Auras == null)
                        unit.Auras = auras;
                    else if (unit.AddedAuras == null)
                        unit.AddedAuras = new List<List<Aura>> { auras };
                    else
                        unit.AddedAuras.Add(auras);
                }
            }
        }

        /*[Parser(Opcode.CMSG_CAST_SPELL)]
        public static void HandleCastSpell(Packet packet)
        {
            var counter = packet.ReadBits(2);
            var unk_bit = !packet.ReadBit();
            for (var i = 0; i < counter; ++i)
                packet.ReadBits("unk value0", 2, i);

            var HasCastCount = !packet.ReadBit();
            packet.ReadBit("Fake bit? Has TargetGUID"); // TargetGUID
            var hasbit1C = !packet.ReadBit();
            var hasMovment = packet.ReadBit();
            var hasbit78 = !packet.ReadBit();
            var hasbitF8 = !packet.ReadBit();
            var hasGUID2 = packet.ReadBit();
            var hasbitFC = !packet.ReadBit();
            var hasbit18 = !packet.ReadBit();
            var hasGUID3 = packet.ReadBit();
            packet.ReadBit("Fake bit? Has GUID0"); // GUID0
            var hasSpellId = !packet.ReadBit();

            var GUID0 = new byte[8];
            var TargetGUID = new byte[8];
            var GUID2 = new byte[8];
            var GUID3 = new byte[8];

            GUID0 = packet.StartBitStream(0, 5, 1, 7, 4, 3, 6, 2);
            if (hasGUID3)
                GUID3 = packet.StartBitStream(2, 5, 3, 7, 4, 1, 0, 6);

            if (hasGUID2)
                GUID2 = packet.StartBitStream(6, 2, 4, 7, 3, 5, 0, 1);

            TargetGUID = packet.StartBitStream(3, 0, 2, 7, 6, 4, 1, 5);

            if (unk_bit)
                packet.ReadEnum<CastFlag>("Cast Flags", 20);

            if (hasbit1C)
                packet.ReadBits("hasbit1C", 5);

            uint len78 = 0;
            if (hasbit78)
                len78 = packet.ReadBits("hasbit78", 7);
            packet.ResetBitReader();

            for (var i = 0; i < counter; ++i)
            {
                packet.ReadInt32("unk value1", i);
                packet.ReadInt32("unk value2", i);
            }

            if (hasGUID3)
            {
                var pos = new Vector3();
                packet.ReadXORBytes(GUID3, 7, 5, 3);
                pos.X = packet.ReadSingle();
                packet.ReadXORBytes(GUID3, 0, 2, 1, 4, 6);
                pos.Z = packet.ReadSingle();
                pos.Y = packet.ReadSingle();
                packet.WriteGuid("GUID3", GUID3);
                packet.WriteLine("Position: {0}", pos);
            }

            packet.ParseBitStream(TargetGUID, 2, 0, 5, 6, 7, 3, 4, 1);
            packet.WriteGuid("Target GUID", TargetGUID);

            if (hasGUID2)
            {
                var pos = new Vector3();
                packet.ReadXORBytes(GUID2, 5, 7);
                pos.Y = packet.ReadSingle();
                pos.X = packet.ReadSingle();
                packet.ReadXORBytes(GUID2, 3, 1);
                pos.Z = packet.ReadSingle();
                packet.ReadXORBytes(GUID2, 2, 6, 4, 0);
                packet.WriteGuid("GUID2", GUID2);
                packet.WriteLine("Position: {0}", pos);
            }

            packet.ParseBitStream(GUID0, 7, 2, 6, 4, 1, 0, 3, 5);
            packet.WriteGuid("GUID0", GUID0);

            if (hasbit78)
                packet.ReadWoWString("String", (int)len78);

            if (HasCastCount)
                packet.ReadByte("Cast Count");

            if (hasbit18)
                packet.ReadInt32("Int18");

            if (hasMovment)
                MovementHandler.ReadClientMovementBlock(ref packet);

            if (hasSpellId)
                packet.ReadInt32("SpellId");

            if (hasbitF8)
                packet.ReadSingle("FloatF8");

            if (hasbitFC)
                packet.ReadSingle("FloatFC");
        }*/

        [Parser(Opcode.CMSG_CAST_SPELL)]
        public static void HandleCastSpell(Packet packet)
        {
            var GUID0 = new byte[8];
            var TargetGUID = new byte[8];
            var GUID2 = new byte[8];
            var GUID3 = new byte[8];

            var hasCastFlags = !packet.ReadBit();
            var hasCountCast = !packet.ReadBit();
            var hasUnk1 = !packet.ReadBit();
            var hasSpellId = !packet.ReadBit();
            var archeologyCounter = packet.ReadBits(2);
            var hasGUID0 = !packet.ReadBit();
            var hasElevation = !packet.ReadBit();
            var hasTargetGUID = !packet.ReadBit();
            var hasGUID3 = packet.ReadBit();
            var hasSpeed = !packet.ReadBit();
            var hasUnkString = !packet.ReadBit(); // string
            var hasGUID2 = packet.ReadBit();
            var hasMovement = packet.ReadBit();
            var hasUnk4 = !packet.ReadBit(); // 5 bits ...

            uint unkStringLenght = 0;

            var archeologyType = new UInt32[archeologyCounter];
            for (var i = 0; i < archeologyCounter; ++i)
                archeologyType[i] = packet.ReadBits("unkCounter", 2, i);

            if (hasMovement)
            {
                // somes shits ...
            }

            if (hasCastFlags)
                packet.ReadEnum<CastFlag>("Cast Flags", 20);

            GUID0 = packet.StartBitStream(2, 1, 3, 6, 5, 4, 7, 0);
            if (hasGUID3)
                GUID3 = packet.StartBitStream(3, 6, 1, 0, 4, 5, 7, 2);
            if (hasGUID2)
                GUID2 = packet.StartBitStream(6, 3, 5, 2, 0, 4, 1, 7);

            TargetGUID = packet.StartBitStream(5, 0, 2, 3, 1, 4, 6, 7);

            if (hasUnkString)
                unkStringLenght = packet.ReadBits(7);

            if (hasUnk4)
                packet.ReadBits(5);

            packet.ResetBitReader();

            var entry = new UInt32[archeologyCounter];
            var usedCount = new UInt32[archeologyCounter];
            for (var i = 0; i < archeologyCounter; ++i)
            {
                // not sure about this order
                entry[i] = packet.ReadUInt32("unk value1", i);
                usedCount[i] = packet.ReadUInt32("unk value2", i);
            }

            if (hasMovement)
            {
                // Some shits ...
            }

            if (hasSpellId)
                packet.ReadUInt32("Spell entry");

            if (hasGUID2)
            {
                var pos = new Vector3();
                packet.ReadXORByte(GUID2, 7);
                pos.X = packet.ReadSingle();
                packet.ReadXORByte(GUID2, 6);
                packet.ReadXORByte(GUID2, 0);
                pos.Y = packet.ReadSingle();
                packet.ReadXORByte(GUID2, 1);
                packet.ReadXORByte(GUID2, 4);
                pos.Z = packet.ReadSingle();
                packet.ReadXORByte(GUID2, 3);
                packet.ReadXORByte(GUID2, 2);
                packet.ReadXORByte(GUID2, 5);
                packet.WriteGuid("GUID2", GUID2);
                packet.WriteLine("Position: {0}", pos);
            }

            if (hasGUID3)
            {
                var pos = new Vector3();
                packet.ReadXORByte(GUID3, 5);
                packet.ReadXORByte(GUID3, 4);
                packet.ReadXORByte(GUID3, 3);
                packet.ReadXORByte(GUID3, 1);
                pos.Z = packet.ReadSingle();
                pos.Y = packet.ReadSingle();
                packet.ReadXORByte(GUID3, 2);
                packet.ReadXORByte(GUID3, 6);
                packet.ReadXORByte(GUID3, 7);
                pos.X = packet.ReadSingle();
                packet.ReadXORByte(GUID3, 0);
                packet.WriteGuid("GUID3", GUID2);
                packet.WriteLine("Position: {0}", pos);
            }

            packet.ParseBitStream(TargetGUID, 7, 2, 6, 0, 4, 5, 1, 3);
            packet.ParseBitStream(GUID0, 1, 0, 2, 3, 5, 6, 7, 4);

            if (hasCountCast)
                packet.ReadSByte("countCast");

            if (hasSpeed)
                packet.ReadSingle("spell speed");

            if (hasUnkString)
                packet.ReadWoWString("String", (int)unkStringLenght);

            if (hasUnk1)
                packet.ReadUInt32("unkInt");

            if (hasElevation)
                packet.ReadSingle("spell elevation");
        }

        [HasSniffData]
        [Parser(Opcode.SMSG_SPELL_START)]
        public static void HandleSpellStart540(Packet packet)
        {
            var PowerUnitGUID = new byte[8];
            var unkGUID6 = new byte[8];
            var sourceTransportGUID = new byte[8];
            var unkGUID4 = new byte[8];
            var destTransportGUID = new byte[8];
            var unkGUID2 = new byte[8];
            var unkGUID1 = new byte[8];
            var unkGUID0 = new byte[8];

            var hasDestTransportGUID = packet.ReadBit();
            unkGUID0[6] = packet.ReadBit(); // 70
            unkGUID0[7] = packet.ReadBit(); // 71
            var hasUnkGUID2 = !packet.ReadBit();
            unkGUID2 = packet.StartBitStream(2, 6, 0, 3, 4, 1, 7, 5);
            unkGUID1[4] = packet.ReadBit(); // 76
            unkGUID0[1] = packet.ReadBit(); // 65
            unkGUID1[2] = packet.ReadBit(); // 74

            var counterUnk1 = packet.ReadBits(24);
            unkGUID0[0] = packet.ReadBit(); // 64
            var counterUnk2 = packet.ReadBits(24);
            unkGUID1[5] = packet.ReadBit(); // 77

            if (hasDestTransportGUID)
                destTransportGUID = packet.StartBitStream(4, 7, 5, 3, 6, 2, 1, 0);

            var hasUnkGUID4 = !packet.ReadBit();
            unkGUID4 = packet.StartBitStream(5, 3, 4, 6, 7, 1, 2, 0);
            var unkBit = !packet.ReadBit();
            var hasUnkInt = !packet.ReadBit();
            var hasUnkByte = !packet.ReadBit();
            var hasUnkInt2 = !packet.ReadBit();
            unkGUID1[7] = packet.ReadBit(); // 79

            var unkGUIDs1 = new byte[counterUnk2][];
            for (var i = 0; i < counterUnk2; ++i)
            {
                unkGUIDs1[i] = new byte[8];
                packet.StartBitStream(unkGUIDs1[i], 5, 3, 1, 4, 6, 0, 2, 7);
            }

            var unkGUIDs2 = new byte[counterUnk1][];
            for (var i = 0; i < counterUnk1; ++i)
            {
                unkGUIDs2[i] = new byte[8];
                packet.StartBitStream(unkGUIDs2[i], 0, 2, 7, 4, 6, 3, 5, 1);
            }

            var hasTargetFlags = !packet.ReadBit();
            var hasUnkInt3 = !packet.ReadBit();

            var powerCount = packet.ReadBits(21);
            var hasPowerData = packet.ReadBit("Has Power Data");
            var hasSourceTransportGUID = packet.ReadBit();
            var hasUnkByte2 = !packet.ReadBit();

            if (hasSourceTransportGUID)
                packet.StartBitStream(sourceTransportGUID, 4, 7, 6, 3, 2, 0, 5, 1);

            var hasUnkByte3 = !packet.ReadBit();
            var hasUnkByte4 = !packet.ReadBit();
            unkGUID0[4] = packet.ReadBit(); // 68

            uint PowerTypeCount = 0;
            if (hasPowerData)
            {
                PowerUnitGUID[4] = packet.ReadBit();
                PowerTypeCount = packet.ReadBits("Power Type Count", 21);
                packet.StartBitStream(PowerUnitGUID, 2, 3, 7, 6, 5, 0, 1);
            }

            var hasUnkFloat1 = !packet.ReadBit();
            var hasUnkInt4 = !packet.ReadBit();

            if (hasTargetFlags)
                packet.ReadEnum<TargetFlag>("Target Flags", 20);

            packet.ReadBits(13);
            unkGUID0[3] = packet.ReadBit(); // 67
            var RuneCooldownCount = packet.ReadBits("Rune Cooldown Count", 3);
            var unkHasGUID6 = !packet.ReadBit();
            unkGUID0[2] = packet.ReadBit(); // 66
            var hasUnkByte5 = !packet.ReadBit();
            unkGUID1[3] = packet.ReadBit(); // 75

            var unkCounter3 = packet.ReadBits(20);
            var unkGUIDs3 = new byte[unkCounter3][];
            for (int i = 0; i < unkCounter3; i++)
            {
                unkGUIDs3[i] = new byte[8];
                packet.StartBitStream(unkGUIDs3[i], 5, 1, 4, 7, 3, 6, 0, 2);
            }

            packet.StartBitStream(unkGUID6, 4, 1, 5, 2, 7, 6, 0, 3);
            unkGUID1[1] = packet.ReadBit(); // 73
            unkGUID0[5] = packet.ReadBit(); // 69
            unkGUID1[6] = packet.ReadBit(); // 78
            var unkCounter4 = packet.ReadBits(25);
            unkGUID1[0] = packet.ReadBit(); // 72

            uint unkStringLenght = 0;
            if (unkBit)
                unkStringLenght = packet.ReadBits(7);

            var hasUnkInt5 = !packet.ReadBit();
            var hasUnkByte6 = packet.ReadBit();

            for (var i = 0; i < unkCounter4; ++i)
            {
                if (packet.ReadBits("bits22[0]", 4, i) == 11)
                    packet.ReadBits("bits22[1]", 4, i);
            }

            packet.ParseBitStream(unkGUID6, 5, 3, 4, 2, 0, 1, 7, 6);
            packet.ReadXORByte(unkGUID0, 2);
            packet.ReadUInt32("cast timer");

            for (int i = 0; i < counterUnk1; i++)
                packet.ParseBitStream(unkGUIDs2[i], 3, 6, 7, 5, 0, 4, 2, 1);

            packet.ReadXORByte(unkGUID0, 6);

            for (int i = 0; i < counterUnk2; i++)
                packet.ParseBitStream(unkGUIDs1[i], 6, 0, 3, 7, 2, 1, 5, 4);

            packet.ReadXORByte(unkGUID1, 0);

            // TARGET_FLAG_SOURCE_LOCATION
            if (hasSourceTransportGUID)
            {
                packet.ParseBitStream(sourceTransportGUID, 6, 7, 3, 0);
                packet.ReadSingle("source Y");
                packet.ParseBitStream(sourceTransportGUID, 1);
                packet.ReadSingle("source Z");
                packet.ParseBitStream(sourceTransportGUID, 4, 2);
                packet.ReadSingle("source X");
                packet.ParseBitStream(sourceTransportGUID, 5);
            }

            if (hasUnkInt)
                packet.ReadUInt32("UnkInt");

            // TARGET_FLAG_DEST_LOCATION
            if (hasDestTransportGUID)
            {
                packet.ParseBitStream(destTransportGUID, 5, 1, 6, 0, 3, 4);
                packet.ReadSingle("Dest x");
                packet.ReadXORByte(destTransportGUID, 2);
                packet.ReadSingle("Dest y");
                packet.ReadSingle("Dest z");
                packet.ReadXORByte(destTransportGUID, 7);
            }

            if (hasUnkByte5)
                packet.ReadSByte("unkByte5");

            packet.ReadXORByte(unkGUID0, 5);
            packet.ReadXORByte(unkGUID0, 7);

            packet.ParseBitStream(unkGUID4, 1, 7, 3, 0, 6, 2, 4, 5);

            if (hasUnkByte6)
            {
                packet.ReadUInt32("hasUnkByte6 1");
                packet.ReadUInt32("hasUnkByte6 2");
            }

            packet.ReadXORByte(unkGUID0, 4);

            for (int i = 0; i < unkCounter3; i++)
            {
                packet.ParseBitStream(unkGUIDs3[i], 4, 5);
                packet.ReadSingle("unkGUIDs3 Y");
                packet.ParseBitStream(unkGUIDs3[i], 0, 1, 2, 3);
                packet.ReadSingle("unkGUIDs3 X");
                packet.ReadSingle("unkGUIDs3 Z");
                packet.ParseBitStream(unkGUIDs3[i], 6, 7);
            }

            if (hasPowerData)
            {
                packet.ReadInt32("Attack power");
                packet.ReadInt32("Spell power");

                for (int i = 0; i < PowerTypeCount; i++)
                {
                    packet.ReadEnum<PowerType>("Power Type", TypeCode.UInt32, i);
                    packet.ReadInt32("Power Value", i);
                }

                packet.ParseBitStream(PowerUnitGUID, 4, 7, 0, 2, 3, 5, 6, 1);
                packet.ReadInt32("Current Heal");
                packet.WriteGuid("PowerUnitGUID", PowerUnitGUID);
            }

            packet.ReadXORByte(unkGUID1, 6);
            packet.ReadXORByte(unkGUID0, 3);

            if (hasUnkByte)
                packet.ReadSByte("hasUnkByte");

            packet.ParseBitStream(unkGUID2, 3, 2, 1, 4, 6, 0, 7, 5);
            packet.ReadXORByte(unkGUID1, 3);

            for (int i = 0; i < RuneCooldownCount; i++)
                packet.ReadSByte("cooldown rune byte");

            if (unkStringLenght > 0)
                packet.ReadWoWString("String:", unkStringLenght);

            packet.ReadSByte("cast count");

            if (hasUnkInt5)
                packet.ReadInt32("UnkInt5");

            packet.ReadXORByte(unkGUID1, 7);

            if (hasUnkInt2)
                packet.ReadInt32("UnkInt2"); // spell entry, look like proc or bonus

            if (hasUnkFloat1)
                packet.ReadSingle("unkFloat1");

            packet.ReadInt32("88 uint");
            packet.ReadXORByte(unkGUID1, 4);
            packet.ReadXORByte(unkGUID1, 1);
            packet.ReadInt32("SpellID");

            if (hasUnkByte4)
                packet.ReadSingle("UnkByte4");

            for (var i = 0; i < powerCount; ++i)
            {
                packet.ReadEnum<PowerType>("Power Type", TypeCode.Byte, i);
                packet.ReadInt32("Power Value", i);
            }

            packet.ReadXORByte(unkGUID0, 0);

            if (hasUnkByte2)
                packet.ReadSByte("unkByte2");

            if (hasUnkByte3)
                packet.ReadSByte("UnkByte3");

            if (hasUnkInt4)
                packet.ReadUInt32("unkInt4");

            packet.ReadXORByte(unkGUID0, 1);

            if (hasUnkInt3)
                packet.ReadUInt32("unkInt3");

            packet.ReadXORByte(unkGUID1, 5);
            packet.ReadXORByte(unkGUID1, 2);
        }

        /*[HasSniffData]
        [Parser(Opcode.SMSG_SPELL_START)]
        public static void HandleSpellStart(Packet packet)
        {
            //var CasterGUID1 = new byte[8]; // 14
            var CasterGUID2 = new byte[8]; // 112-119
            var guid3 = new byte[8]; // 24-31
            var guid4 = new byte[8]; // 16-23
            //var guid5 = new byte[8]; // 98
            var guid6 = new byte[8]; // 416-423
            var DestinationTransportGUID = new byte[8]; // 168-175
            var SourceTransportGUID = new byte[8]; // 136-143
            //var guid9 = new byte[8]; // 18
            var TargetGUID = new byte[8];
            var PowerUnitGUID = new byte[8];

            var bits52 = packet.ReadBits(24);
            var CasterGUID1 = new byte[bits52][];
            for (var i = 0; i < bits52; ++i)
            {
                CasterGUID1[i] = new byte[8];
                packet.StartBitStream(CasterGUID1[i], 2, 5, 4, 7, 6, 0, 3, 1);
            }

            packet.ReadBit("bit28");
            var bit106 = !packet.ReadBit();
            packet.ReadBit("bit30");

            packet.StartBitStream(CasterGUID2, 5, 4, 7, 1, 0, 6, 3, 2);
            packet.StartBitStream(guid4, 5, 6);
            guid3[2] = packet.ReadBit();
            var bit372 = packet.ReadBit();
            packet.StartBitStream(TargetGUID, 0, 3, 1, 5, 6, 2, 7, 4);

            var hasPowerData = packet.ReadBit("Has Power Data"); // bit432
            uint PowerTypeCount = 0;
            if (hasPowerData)
            {
                packet.StartBitStream(PowerUnitGUID, 6, 7, 3, 5, 0, 4, 2, 1);
                PowerTypeCount = packet.ReadBits("Power Type Count", 21);
            }
            packet.StartBitStream(guid3, 6, 0);
            var bit102 = !packet.ReadBit();
            var bit101 = !packet.ReadBit();
            guid4[0] = packet.ReadBit();
            var bits84 = packet.ReadBits(25);
            guid3[7] = packet.ReadBit();
            var hasTargetFlags = !packet.ReadBit();
            var bit368 = !packet.ReadBit();
            var hasRuneStateBefore = !packet.ReadBit();
            guid4[4] = packet.ReadBit();
            guid3[4] = packet.ReadBit();
            var bit91 = !packet.ReadBit();
            var ExtraTargetsCount = packet.ReadBits("Extra Targets Count", 20);

            var ExtraTargetsGUID = new byte[ExtraTargetsCount][];
            for (var i = 0; i < ExtraTargetsCount; i++)
            {
                ExtraTargetsGUID[i] = new byte[8];
                packet.StartBitStream(ExtraTargetsGUID[i], 0, 5, 2, 7, 6, 4, 3, 1);

            }
            packet.ReadBit("bit104");
            var bit90 = !packet.ReadBit();

            for (var i = 0; i < bits84; ++i)
            {
                if (packet.ReadBits("bits22[0]", 4, i) == 11)
                    packet.ReadBits("bits22[1]", 4, i);
            }

            packet.StartBitStream(guid6, 2, 1, 7, 0, 6, 3, 5, 4);
            guid4[7] = packet.ReadBit();
            var HasDestinationData = packet.ReadBit();
            var hasSourceData = packet.ReadBit();
            guid4[2] = packet.ReadBit();

            if (HasDestinationData)
                packet.StartBitStream(DestinationTransportGUID, 3, 7, 1, 0, 5, 6, 4, 2);

            guid4[3] = packet.ReadBit();
            var bit89 = !packet.ReadBit();

            if (hasSourceData)
                packet.StartBitStream(SourceTransportGUID, 5, 4, 3, 2, 0, 6, 7, 1);

            var bit48 = packet.ReadBit();

            if (hasTargetFlags)
                packet.ReadEnum<TargetFlag>("Target Flags", 20);

            var hasRuneStateAfter = !packet.ReadBit();

            int bits48 = 0;
            if (!bit48)
                bits48 = (int)packet.ReadBits("bits48", 7);

            var hasPredictedType = !packet.ReadBit("bit428");
            guid3[3] = packet.ReadBit();
            var bits68 = packet.ReadBits("bits68", 24);
            guid3[1] = packet.ReadBit();

            var guid9 = new byte[bits68][];
            for (var i = 0; i < bits68; ++i)
            {
                guid9[i] = new byte[8];
                packet.StartBitStream(guid9[i], 3, 1, 2, 7, 5, 6, 4, 0);
            }

            var bit384 = !packet.ReadBit("bit384");
            guid3[5] = packet.ReadBit();
            guid4[1] = packet.ReadBit();
            var bits320 = packet.ReadBits("bits320", 21);
            var RuneCooldownCount = packet.ReadBits("Rune Cooldown Count", 3);
            packet.ReadBits("bits11", 12);

            if (HasDestinationData)
            {
                var pos = new Vector3();
                packet.ReadXORBytes(DestinationTransportGUID, 0, 1);
                pos.Z = packet.ReadSingle();
                packet.ReadXORBytes(DestinationTransportGUID, 5, 3);
                pos.Y = packet.ReadSingle();
                packet.ReadXORByte(DestinationTransportGUID, 2);
                pos.X = packet.ReadSingle();
                packet.ReadXORBytes(DestinationTransportGUID, 6, 7, 4);
                packet.WriteGuid("Destination Transport GUID", DestinationTransportGUID);
                packet.WriteLine("Destination Position: {0}", pos);
            }
            packet.ParseBitStream(TargetGUID, 1, 0, 5, 2, 3, 4, 7, 6);
            packet.ReadXORByte(guid4, 3);
            packet.WriteGuid("TargetGUID", TargetGUID);
            if (bit372)
            {
                packet.ReadUInt32("unk95");
                packet.ReadUInt32("unk94");
            }

            for (var i = 0; i < bits68; ++i)
            {
                packet.ParseBitStream(guid9[i], 1, 4, 5, 6, 2, 7, 3, 0);
                packet.WriteGuid("GUID9", guid9[i], i);
            }

            for (var i = 0; i < bits52; ++i)
            {
                packet.ParseBitStream(CasterGUID1[i], 0, 4, 3, 2, 7, 1, 5, 6);
                packet.WriteGuid("CasterGUID1", CasterGUID1[i], i);
            }

            packet.ParseBitStream(CasterGUID2, 4, 5, 7, 0, 1, 3, 2, 6);
            packet.WriteGuid("CasterGUID2", CasterGUID2);

            for (var i = 0; i < bits320; ++i)
            {
                packet.ReadEnum<PowerType>("Power Type", TypeCode.Byte, i);
                packet.ReadInt32("Power Value", i);
            }

            if (hasPowerData)
            {
                packet.ReadXORByte(PowerUnitGUID, 0);
                packet.ReadInt32("Current Health");
                packet.ReadXORByte(PowerUnitGUID, 2);
                packet.ReadInt32("Spell power");
                packet.ReadXORByte(PowerUnitGUID, 5);
                packet.ReadInt32("Attack power");
                packet.ReadXORByte(PowerUnitGUID, 1);

                for (var i = 0; i < PowerTypeCount; ++i)
                {
                    packet.ReadEnum<PowerType>("Power Type", TypeCode.UInt32, i);
                    packet.ReadInt32("Power Value", i);
                }
                packet.ReadXORBytes(PowerUnitGUID, 6, 7, 4, 3);
                packet.WriteGuid("PowerUnitGUID", PowerUnitGUID);
            }

            if (bit89)
                packet.ReadUInt32("unk89");

            packet.ParseBitStream(guid6, 1, 7, 4, 3, 5, 2, 0, 6);
            packet.WriteGuid("GUID6", guid6);
            packet.ReadXORBytes(guid3, 3, 4);

            if (hasSourceData)
            {
                var pos = new Vector3();
                packet.ReadXORBytes(SourceTransportGUID, 2, 4, 1);
                pos.Z = packet.ReadSingle();
                packet.ReadXORBytes(SourceTransportGUID, 0, 5, 3);
                pos.X = packet.ReadSingle();
                pos.Y = packet.ReadSingle();
                packet.ReadXORBytes(SourceTransportGUID, 7, 6);
                packet.WriteGuid("Source Transport GUID", SourceTransportGUID);
                packet.WriteLine("Source Position: {0}", pos);
            }

            for (var i = 0; i < RuneCooldownCount; ++i)
                packet.ReadByte("Rune Cooldown Passed", i);

            for (var i = 0; i < ExtraTargetsCount; ++i)
            {
                var pos = new Vector3();
                packet.ReadXORBytes(ExtraTargetsGUID[i], 4, 2);
                pos.X = packet.ReadSingle();
                packet.ReadXORBytes(ExtraTargetsGUID[i], 5, 7, 0);
                pos.Y = packet.ReadSingle();
                packet.ReadXORBytes(ExtraTargetsGUID[i], 1, 3, 6);
                pos.Z = packet.ReadSingle();
                packet.WriteGuid("Extra Target GUID", ExtraTargetsGUID[i], i);
                packet.WriteLine("[{1}] Position: {0}", pos, i);
            }
            packet.ReadXORByte(guid4, 2);

            if (hasRuneStateBefore)
                packet.ReadByte("Rune State Before");

            if (bit90)
                packet.ReadSingle("float90");

            packet.ReadEnum<CastFlag>("Cast Flags", TypeCode.Int32);
            packet.ReadXORByte(guid3, 2);
            packet.ReadEntryWithName<Int32>(StoreNameType.Spell, "Spell ID");
            packet.ReadXORByte(guid4, 0);
            packet.ReadXORByte(guid3, 5);

            if (hasPredictedType)
                packet.ReadByte("Predicted Type");

            packet.ReadXORBytes(guid3, 0, 6, 1);
            packet.ReadXORByte(guid4, 1);

            if (hasRuneStateAfter)
                packet.ReadByte("Rune State After");

            if (bit101)
                packet.ReadUInt32("unk101");

            packet.ReadXORByte(guid4, 4);

            if (bit368)
                packet.ReadByte("byte368");

            if (bit384)
                packet.ReadByte("byte384");

            packet.ReadWoWString("String48:", bits48);

            packet.ReadXORByte(guid4, 7);
            packet.ReadByte("Cast Count");

            if (bit102)
                packet.ReadUInt32("unk102");

            packet.ReadXORByte(guid4, 6);

            if (bit106)
                packet.ReadUInt32("Heal");

            packet.ReadUInt32("Cast time");
            packet.ReadXORByte(guid4, 5);

            if (bit91)
                packet.ReadUInt32("unk91");

            packet.ReadXORByte(guid3, 7);
            packet.WriteGuid("GUID3", guid3);
            packet.WriteGuid("GUID4", guid4);
        }*/

        [Parser(Opcode.SMSG_INITIAL_SPELLS)]
        public static void HandleInitialSpells(Packet packet)
        {
            var count = packet.ReadBits("Spell Count", 22);
            packet.ReadBit("Unk Bit");

            var spells = new List<uint>((int)count);
            for (var i = 0; i < count; i++)
            {
                var spellId = packet.ReadEntryWithName<UInt32>(StoreNameType.Spell, "Spell ID", i);
                spells.Add((uint)spellId);
            }

            var startSpell = new StartSpell { Spells = spells };

            WoWObject character;
            if (Storage.Objects.TryGetValue(SessionHandler.LoginGuid, out character))
            {
                var player = character as Player;
                if (player != null && player.FirstLogin)
                    Storage.StartSpells.Add(new Tuple<Race, Class>(player.Race, player.Class), startSpell, packet.TimeSpan);
            }
        }

        [Parser(Opcode.SMSG_SPELL_GO)]
        public static void HandleSpellGo540(Packet packet)
        {
            var GUID1 = new byte[8];
            var GUID2 = new byte[8];
            var GUID3 = new byte[8];
            var GUID4 = new byte[8]; // 16-23
            var GUID5 = new byte[8]; // 24-31
            var GUID6 = new byte[8];
            var GUID7 = new byte[8];
            var PowerUnitGUID = new byte[8];

            var hasGUID1 = !packet.ReadBit();
            GUID5[4] = packet.ReadBit();
            var hasBit102 = !packet.ReadBit();
            var hasBit368 = !packet.ReadBit();
            var hasBit90 = !packet.ReadBit();
            var hasGUID2 = !packet.ReadBit();
            GUID4[7] = packet.ReadBit();
            packet.StartBitStream(GUID1, 2, 5, 6, 1, 0, 3, 7, 4);
            GUID5[1] = packet.ReadBit();
            var hasBit337 = !packet.ReadBit();
            packet.StartBitStream(GUID2, 7, 0, 1, 3, 4, 2, 5, 6);
            GUID4[2] = packet.ReadBit();
            var hasTargetFlags = !packet.ReadBit();
            var hasBit384 = !packet.ReadBit();
            GUID5[5] = packet.ReadBit();
            var hasBit428 = !packet.ReadBit();
            var counter320 = packet.ReadBits(21);
            var hasBit91 = !packet.ReadBit();
            var counter11 = packet.ReadBits(13);
            var hasBit380 = packet.ReadBit();
            var hasBit152 = packet.ReadBit(); // HAS GUID 3
            GUID4[3] = packet.ReadBit();
            var counter388 = packet.ReadBits(20);
            GUID4[1] = packet.ReadBit();
            GUID5[0] = packet.ReadBit();
            GUID4[6] = packet.ReadBit();
            GUID4[5] = packet.ReadBit();
            var hasBit101 = !packet.ReadBit();
            var counter68 = packet.ReadBits(24);
            GUID5[3] = packet.ReadBit();
            var hasBit106 = !packet.ReadBit();
            var hasBit48 = !packet.ReadBit();

            if (hasTargetFlags)
                packet.ReadEnum<TargetFlag>("Target Flags", 20);

            GUID5[2] = packet.ReadBit();
            
            // HAS GUID3
            if (hasBit152)
                packet.StartBitStream(GUID3, 2, 4, 6, 3, 7, 5, 1, 0);

            var GUIDS1 = new byte[counter68][];
            for (int i = 0; i < counter68; i++)
            {
                GUIDS1[i] = new byte[8];
                packet.StartBitStream(GUIDS1[i], 6, 7, 0, 2, 5, 4, 1, 3);
            }

            var hasBit184 = packet.ReadBit();
            var hasBit416 = !packet.ReadBit(); // has GUID6

            var GUIDS2 = new byte[counter388][];
            for (int i = 0; i < counter388; i++)
            {
                GUIDS2[i] = new byte[8];
                packet.StartBitStream(GUIDS2[i], 0, 4, 3, 1, 6, 7, 2, 5);
            }

            packet.StartBitStream(GUID6, 4, 6, 7, 0, 1, 2, 3, 5);
            var hasBit356 = !packet.ReadBit();

            if (hasBit184)
                packet.StartBitStream(GUID7, 4, 1, 7, 3, 0, 5, 6, 2);

            var hasPowerUnitGUID = packet.ReadBit(); // HAS GUID8
            var counter52 = packet.ReadBits(24);

            uint PowerTypeCount = 0;
            if (hasPowerUnitGUID)
            {
                packet.StartBitStream(PowerUnitGUID, 7, 4, 0, 6);
                PowerTypeCount = packet.ReadBits(21);
                packet.StartBitStream(PowerUnitGUID, 5, 2, 3, 1);
            }

            var GUIDS3 = new byte[counter52][];
            for (int i = 0; i < counter52; i++)
            {
                GUIDS3[i] = new byte[8];
                packet.StartBitStream(GUIDS3[i], 4, 0, 2, 7, 6, 1, 3, 5);
            }

            GUID4[0] = packet.ReadBit();
            GUID4[4] = packet.ReadBit();
            GUID5[6] = packet.ReadBit();

            var RuneCooldownCount = packet.ReadBits(3);
            var hasBit336 = !packet.ReadBit();
            GUID5[7] = packet.ReadBit();

            var counter84 = packet.ReadBits(25);

            uint unkStringLenght = 0;
            if (hasBit48)
                unkStringLenght = packet.ReadBits(7);

            for (var i = 0; i < counter84; ++i)
            {
                if (packet.ReadBits("bits22[0]", 4, i) == 11) // missCondition
                    packet.ReadBits("bits22[1]", 4, i); // SPELL_MISS_REFLECT
            }

            packet.ReadXORByte(GUID5, 7);

            if (hasPowerUnitGUID)
            {
                //PowerTypeCount
                packet.ParseBitStream(PowerUnitGUID, 3, 5, 1, 4);
                for (int i = 0; i < PowerTypeCount; i++)
                {
                    packet.ReadEnum<PowerType>("Power Type", TypeCode.UInt32, i);
                    packet.ReadInt32("Power Value", i);
                }

                packet.ParseBitStream(PowerUnitGUID, 6, 2);
                packet.ReadInt32("power 1");
                packet.ReadInt32("power 2");
                packet.ReadInt32("power 3");
                packet.ParseBitStream(PowerUnitGUID, 7, 0);
            }

            packet.ParseBitStream(GUID6, 5, 3, 2, 7, 4, 0, 6, 1);

            for (int i = 0; i < counter68; i++)
                packet.ParseBitStream(GUIDS1[i], 7, 1, 0, 4, 2, 5, 6, 3);

            // GUID3 sourcePos
            if (hasBit152)
            {
                packet.ParseBitStream(GUID3, 2, 6, 0, 3, 4);
                packet.ReadSingle("Y");
                packet.ReadSingle("X");
                packet.ReadSingle("Z");
                packet.ParseBitStream(GUID3, 7, 5, 1);
            }

            packet.ParseBitStream(GUID2, 0, 6, 5, 7, 3, 2, 4, 1);
            packet.ReadXORByte(GUID4, 3);

            for (int i = 0; i < counter52; i++)
            {
                packet.ParseBitStream(GUIDS3[i], 6, 0, 7, 1, 2, 5, 3, 4);
                packet.WriteGuid(GUIDS3[i]);
            }

            if (hasBit90)
                packet.ReadSingle("float 1");

            packet.ParseBitStream(GUID1, 3, 1, 5, 0, 7, 6, 4, 2);
            packet.ReadXORByte(GUID5, 3);

            if (hasBit428)
                packet.ReadSByte("428 byte");

            packet.ReadXORByte(GUID4, 4);

            if (hasBit102)
                packet.ReadUInt32("unk uint32 102");

            if (hasBit337)
                packet.ReadSByte("337 byte");

            packet.ReadXORByte(GUID4, 0);
            packet.ReadXORByte(GUID5, 4);

            // destPos
            if (hasBit184)
            {
                packet.ReadSingle("Z");
                packet.ReadXORByte(GUID7, 4);
                packet.ReadSingle("Y");
                packet.ReadSingle("X");
                packet.ParseBitStream(GUID7, 7, 3, 2, 1, 6, 0, 5);
            }

            packet.ReadXORByte(GUID4, 2);
            packet.ReadXORByte(GUID5, 6);

            if (hasBit336)
                packet.ReadSByte("336 byte");

            packet.ReadXORByte(GUID5, 1);
            packet.ReadUInt32("timestamp");

            for (int i = 0; i < counter388; i++)
            {
                packet.ReadXORByte(GUIDS2[i], 2);
                packet.ReadSingle("Y");
                packet.ReadXORByte(GUIDS2[i], 6);
                packet.ReadSingle("Z");
                packet.ReadSingle("X");
                packet.ParseBitStream(GUIDS2[i], 4, 1, 3, 0, 7, 5);
            }

            if (hasBit380)
            {
                packet.ReadUInt32("94 uint32");
                packet.ReadUInt32("32 uint32");
            }

            packet.ReadUInt32("spell ID");

            if (hasBit91)
                packet.ReadUInt32("91 uint32");

            packet.ReadXORByte(GUID4, 6);

            if (hasBit106)
                packet.ReadUInt32("uint32 106");

            for (int i = 0; i < counter320; i++)
            {
                packet.ReadInt32("Power Value", i);
                packet.ReadEnum<PowerType>("Power Type", TypeCode.Byte, i);
            }

            for (int i = 0; i < RuneCooldownCount; i++)
                packet.ReadSingle("rune cooldown");

            if (unkStringLenght > 0)
                packet.ReadWoWString("string unk", (int)unkStringLenght);

            packet.ReadXORByte(GUID4, 1);

            if (hasBit368)
                packet.ReadSByte("byte 368");

            if (hasBit356)
                packet.ReadUInt32("uint 356");

            if (hasBit384)
                packet.ReadSByte("byte 384");

            packet.ReadUInt32("uint 10");
            packet.ReadXORByte(GUID5, 5);

            if (hasBit101)
                packet.ReadUInt32("uint 101");

            packet.ReadXORByte(GUID5, 0);
            packet.ReadXORByte(GUID4, 7);
            packet.ReadXORByte(GUID4, 5);
            packet.ReadXORByte(GUID5, 2);

            packet.ReadSByte("CastCount");
        }

        /*[Parser(Opcode.SMSG_SPELL_GO)]
        public static void HandleSpellGo(Packet packet)
        {
            var guid = new byte[8];
            var guid2 = new byte[8];
            var SourceTransportGUID = new byte[8];
            byte[][] guid4;
            byte[][] guid5;
            var DestinationTransportGUID = new byte[8];
            byte[][] guid7;
            var guid8 = new byte[8];
            var guid9 = new byte[8];
            var guid10 = new byte[8];
            var guid11 = new byte[8];

            uint powerCount = 0;

            guid10[5] = packet.ReadBit();
            var counter = packet.ReadBits(24);
            var hasPredictedType = !packet.ReadBit();
            var bit404 = !packet.ReadBit();
            packet.ReadBit("field_A8");
            guid11[6] = packet.ReadBit();
            var bit432 = !packet.ReadBit();
            var counter3 = packet.ReadBits(20);
            packet.StartBitStream(guid, 2, 1, 0, 4, 5, 6, 3, 7);
            packet.StartBitStream(guid10, 0, 1, 3);

            guid7 = new byte[counter][];
            for (var i = 0; i < counter; ++i)
            {
                guid7[i] = new byte[8];
                packet.StartBitStream(guid7[i], 5, 6, 4, 7, 1, 2, 3, 0);
            }

            var hasCastSchoolImmunities = !packet.ReadBit();
            guid11[5] = packet.ReadBit();
            var hasRuneStateBefore = !packet.ReadBit();
            var powerLeftSelf = packet.ReadBits(21);
            var bit408 = !packet.ReadBit();
            var counter2 = packet.ReadBits(24);
            var hasPowerData = packet.ReadBit();
            packet.StartBitStream(guid10, 6, 2);
            guid11[7] = packet.ReadBit();
            var RuneCooldownCount = packet.ReadBits(3);
            var hasTargetFlags = !packet.ReadBit();
            var hasSourceData = packet.ReadBit();

            if (hasPowerData)
            {
                packet.StartBitStream(guid2, 7, 4, 5, 0, 2, 6, 3, 1);
                powerCount = packet.ReadBits(21);
            }

            var bit412 = !packet.ReadBit();

            if (hasSourceData)
                packet.StartBitStream(SourceTransportGUID, 6, 3, 0, 1, 4, 5, 2, 7);

            guid11[1] = packet.ReadBit();

            guid4 = new byte[counter2][];
            for (var i = 0; i < counter2; ++i)
            {
                guid4[i] = new byte[8];
                packet.StartBitStream(guid4[i], 6, 4, 1, 7, 5, 2, 3, 0);
            }

            guid5 = new byte[counter3][];
            for (var i = 0; i < counter3; ++i)
            {
                guid5[i] = new byte[8];
                packet.StartBitStream(guid5[i], 7, 6, 5, 0, 4, 3, 1, 2);
            }
            var bit416 = !packet.ReadBit();
            guid11[0] = packet.ReadBit();
            packet.ReadBits("int5C", 12);
            packet.StartBitStream(guid10, 7, 4);
            var hasDestinationData = packet.ReadBit();

            if (hasDestinationData)
                packet.StartBitStream(DestinationTransportGUID, 0, 2, 7, 6, 1, 4, 3, 5);

            var hasRuneStateAfter = !packet.ReadBit();
            var hasCastImmunities = !packet.ReadBit();
            guid11[3] = packet.ReadBit();
            var bit420 = packet.ReadBit();
            var hasPredictedSpellId = !packet.ReadBit();
            guid11[4] = packet.ReadBit();
            var unkflag27 = packet.ReadBit();

            int bits7 = 0;
            if (!unkflag27)
                bits7 = (int)packet.ReadBits(7);

            var counter4 = packet.ReadBits(25);
            guid11[2] = packet.ReadBit();

            for (var i = 0; i < counter4; ++i)
            {
                var bits136 = packet.ReadBits("bits136", 4);

                if (bits136 == 11)
                    packet.ReadBits("bits140", 4);
            }

            packet.ReadBit("unk464");
            packet.StartBitStream(guid8, 7, 3, 6, 4, 2, 5, 0, 1);
            packet.ReadBit("unk160");
            packet.StartBitStream(guid9, 3, 5, 2, 1, 4, 0, 6, 7);

            if (hasTargetFlags)
                packet.ReadEnum<TargetFlag>("Target Flags", 20);

            if (hasSourceData)
            {
                var pos = new Vector3();
                packet.ReadXORBytes(SourceTransportGUID, 3, 7, 4);
                pos.X = packet.ReadSingle();
                packet.ReadXORBytes(SourceTransportGUID, 6, 5, 2);
                pos.Z = packet.ReadSingle();
                pos.Y = packet.ReadSingle();
                packet.ReadXORBytes(SourceTransportGUID, 0, 1);
                packet.WriteGuid("Source Transport GUID", SourceTransportGUID);
                packet.WriteLine("Source Position: {0}", pos);
            }


            for (var i = 0; i < counter3; ++i)
            {
                var pos = new Vector3();
                packet.ReadXORBytes(guid5[i], 4, 0, 7, 6, 2, 1);
                pos.X = packet.ReadSingle();
                packet.ReadXORBytes(guid5[i], 5, 3);
                pos.Y = packet.ReadSingle();
                pos.Z = packet.ReadSingle();
                packet.WriteGuid("GUID5", guid5[i], i);
                packet.WriteLine("Position: {0}", pos);
            }

            for (var i = 0; i < counter; ++i)
            {
                packet.ParseBitStream(guid7[i], 2, 1, 5, 3, 7, 0, 4, 6);
                packet.WriteGuid("GUID7", guid7[i], i);
            }

            if (hasDestinationData)
            {
                var pos = new Vector3();
                packet.ReadXORBytes(DestinationTransportGUID, 1, 4);
                pos.Z = packet.ReadSingle();
                packet.ReadXORBytes(DestinationTransportGUID, 0, 7, 3, 5);
                pos.Y = packet.ReadSingle();
                packet.ReadXORByte(DestinationTransportGUID, 6);
                pos.X = packet.ReadSingle();
                packet.ReadXORByte(DestinationTransportGUID, 2);
                packet.WriteGuid("Destination Transport GUID", DestinationTransportGUID);
                packet.WriteLine("Destination Position: {0}", pos);
            }
            if (bit408)
                packet.ReadSingle("float198");

            if (bit416)
                packet.ReadByte("unk.416");

            packet.ParseBitStream(guid8, 5, 0, 2, 7, 6, 3, 4, 1);
            packet.WriteGuid("GUID8", guid8);

            for (var i = 0; i < counter2; ++i)
            {
                packet.ParseBitStream(guid4[i], 6, 3, 1, 0, 2, 4, 7, 5);
                packet.WriteGuid("GUID4", guid4[i], i);
            }

            if (hasPowerData)
            {
                packet.ReadXORByte(guid2, 3);
                packet.ReadUInt32("Spell Power");
                packet.ReadXORBytes(guid2, 6, 4, 7, 0);
                packet.ReadUInt32("Attack Power");
                packet.ReadXORByte(guid2, 2);

                for (var i = 0; i < powerCount; ++i)
                {
                    packet.ReadUInt32("uint32 60");
                    packet.ReadUInt32("uint32 48");
                }

                packet.ReadUInt32("Current Health");
                packet.ReadXORBytes(guid2, 1, 5);
                packet.WriteGuid("GUID2", guid2);
            }

            packet.ParseBitStream(guid9, 2, 3, 4, 7, 5, 1, 6, 0);
            packet.WriteGuid("GUID9", guid9);

            packet.ReadByte("Cast Count");

            packet.ParseBitStream(guid, 0, 1, 4, 5, 7, 6, 2, 3);
            packet.WriteGuid("GUID", guid);

            packet.ReadXORByte(guid10, 7);
            if (hasPredictedType)
                packet.ReadByte("Predicted Type");

            if (hasPredictedSpellId)
                packet.ReadUInt32("Predicted Spell Id");

            packet.ReadXORByte(guid10, 3);

            var bytes = packet.ReadBytes(bits7);
            packet.WriteLine("Bytes {0}", Utilities.ByteArrayToHexString(bytes));

            if (bit404)
                packet.ReadUInt32("uint32 404");

            if (hasCastImmunities)
                packet.ReadUInt32("Cast Immunities");

            packet.ReadXORByte(guid10, 2);
            packet.ReadXORByte(guid11, 0);

            if (bit420)
            {
                packet.ReadUInt32("uint32 424");
                packet.ReadUInt32("uint32 428");
            }

            if (bit412)
                packet.ReadUInt32("uint32 412");

            packet.ReadXORByte(guid11, 2);
            packet.ReadXORBytes(guid10, 0, 1);
            packet.ReadXORByte(guid11, 3);

            for (var i = 0; i < RuneCooldownCount; ++i)
                packet.ReadByte("Rune Cooldown Passed", i);

            if (hasRuneStateAfter)
                packet.ReadByte("Rune State After");

            packet.ReadXORByte(guid11, 1);
            packet.ReadXORByte(guid10, 4);
            packet.ReadEntryWithName<Int32>(StoreNameType.Spell, "Spell ID");

            if (hasRuneStateBefore)
                packet.ReadByte("Rune State Before");

            packet.ReadXORByte(guid10, 6);
            packet.ReadXORByte(guid11, 5);
            packet.ReadUInt32("uint32 88"); // field_58
            packet.ReadXORByte(guid11, 4);
            packet.ReadUInt32("uint32 96"); // field_60

            for (var i = 0; i < powerLeftSelf; ++i)
            {
                packet.ReadInt32("Power Value", i);
                packet.ReadEnum<PowerType>("Power Type", TypeCode.Byte, i);
            }
            packet.ReadXORByte(guid10, 5);
            packet.ReadXORByte(guid11, 6);

            if (hasCastSchoolImmunities)
                packet.ReadUInt32("Cast School Immunities");

            packet.ReadXORByte(guid11, 7);

            if (bit432)
                packet.ReadByte("unk432");

            packet.WriteGuid("GUID10", guid10);
            packet.WriteGuid("GUID11", guid11);
        }*/

        [Parser(Opcode.SMSG_REMOVED_SPELL)]
        public static void HandleRemovedSpell2(Packet packet)
        {
            var count = packet.ReadBits(22);
            for (var i = 0; i < count; ++i)
                packet.ReadEntryWithName<UInt32>(StoreNameType.Spell, "Spell ID", i);
        }
    }
}
