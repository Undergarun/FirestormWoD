using System;
using WowPacketParser.Enums;
using WowPacketParser.Misc;

namespace WowPacketParser.Parsing.Parsers
{
    public static class CombatHandler
    {

        [Parser(Opcode.CMSG_ATTACKSWING)]
        public static void HandleAttackSwing(Packet packet)
        {
            packet.ReadGuid("GUID");
        }

        [Parser(Opcode.SMSG_DUEL_REQUESTED)]
        public static void HandleDuelRequested(Packet packet)
        {
            var guid = new byte[8];

            packet.StartBitStream(guid, 6, 2, 4, 1, 5, 3, 0, 7);
            packet.ResetBitReader();
            packet.ParseBitStream(guid, 5, 2, 3, 6, 1, 0, 4, 7);

            packet.WriteGuid("GUID", guid);
        }

        [Parser(Opcode.SMSG_DUEL_COMPLETE)]
        public static void HandleDuelComplete(Packet packet)
        {
            packet.ReadBoolean("Abnormal finish");
        }

        [Parser(Opcode.SMSG_DUEL_WINNER)]
        public static void HandleDuelWinner(Packet packet)
        {
            packet.ReadBoolean("Abnormal finish");
            if (ClientVersion.AddedInVersion(ClientVersionBuild.V4_2_2_14545)) // Probably earlier
            {
                packet.ReadCString("Name");
                packet.ReadCString("Opponent Name");
            }
            else
            {
                packet.ReadCString("Opponent Name");
                packet.ReadCString("Name");
            }
        }

        [Parser(Opcode.SMSG_DUEL_COUNTDOWN)]
        public static void HandleDuelCountDown(Packet packet)
        {
            packet.ReadInt32("Timer");
        }

        [Parser(Opcode.SMSG_RESET_RANGED_COMBAT_TIMER)]
        public static void HandleResetRangedCombatTimer(Packet packet)
        {
            packet.ReadInt32("Timer");
        }

        [Parser(Opcode.CMSG_TOGGLE_PVP)]
        public static void HandleTogglePvP(Packet packet)
        {
            // this opcode can be used in two ways: Either set new status explicitly or toggle old status
            if (packet.CanRead())
                packet.ReadBoolean("Enable");
        }

        [Parser(Opcode.SMSG_PVP_CREDIT)]
        public static void HandlePvPCredit(Packet packet)
        {
            packet.ReadUInt32("Honor");
            packet.ReadGuid("GUID");
            packet.ReadInt32("Rank");
        }

        [Parser(Opcode.CMSG_SETSHEATHED)]
        public static void HandleSetSheathed(Packet packet)
        {
            packet.ReadEnum<SheathState>("Sheath", TypeCode.Int32);
        }

        [Parser(Opcode.SMSG_PARTYKILLLOG)]
        public static void HandlePartyKillLog(Packet packet)
        {
            packet.ReadGuid("Player GUID");
            packet.ReadGuid("Victim GUID");
        }

        [Parser(Opcode.SMSG_AI_REACTION)]
        public static void HandleAIReaction(Packet packet)
        {
            packet.ReadGuid("GUID");
            packet.ReadEnum<AIReaction>("Reaction", TypeCode.Int32);
        }

        [Parser(Opcode.SMSG_UPDATE_COMBO_POINTS)]
        public static void HandleUpdateComboPoints(Packet packet)
        {
            packet.ReadPackedGuid("GUID");
            packet.ReadByte("Combo Points");
        }

        [Parser(Opcode.SMSG_ENVIRONMENTALDAMAGELOG)]
        public static void HandleEnvirenmentalDamageLog(Packet packet)
        {
            var playerGuid = new byte[8];
            var playerPowerGuid = new byte[8];

            packet.StartBitStream(playerGuid, 6, 1, 5, 0, 3, 7, 2);
            var hasPowerData = packet.ReadBit("hasPowerData");
            playerGuid[4] = packet.ReadBit();

            if (hasPowerData)
            {
                packet.StartBitStream(playerPowerGuid, 6, 5, 2, 1, 0, 7);
                var counter = packet.ReadBits("counter", 21);
                packet.StartBitStream(playerPowerGuid, 4, 3);

                if (hasPowerData)
                {
                    packet.ReadXORByte(playerPowerGuid, 1);
                    packet.ReadUInt32("Unk UInt32");
                    packet.ReadXORByte(playerPowerGuid, 2);
                    packet.ReadUInt32("Unk UInt32");
                    packet.ReadXORByte(playerPowerGuid, 5);

                    for (int i = 0; i < counter; ++i)
                    {
                        packet.ReadEnum<PowerType>("Power type", TypeCode.UInt32, i);
                        packet.ReadUInt32("Power Amount", i);
                    }
                    
                    packet.ReadXORByte(playerPowerGuid, 7);
                    packet.ReadXORByte(playerPowerGuid, 6);
                    packet.ReadXORByte(playerPowerGuid, 3);
                    packet.ReadXORByte(playerPowerGuid, 4);
                    packet.ReadXORByte(playerPowerGuid, 0);
                    packet.ReadUInt32("Unk UInt32");

                    packet.WriteGuid("Player Power GUID", playerPowerGuid);
                }
            }
            
            packet.ReadUInt32("Unk UInt32");
            packet.ReadUInt32("Unk UInt32");
            packet.ReadXORByte(playerGuid, 4);
            packet.ReadXORByte(playerGuid, 2);
            packet.ReadXORByte(playerGuid, 5);
            packet.ReadXORByte(playerGuid, 7);
            packet.ReadXORByte(playerGuid, 1);
            packet.ReadUInt32("Unk UInt32");
            packet.ReadXORByte(playerGuid, 3);
            packet.ReadXORByte(playerGuid, 6);
            packet.ReadEnum<EnvironmentDamage>("Type", TypeCode.Byte);
            packet.ReadXORByte(playerGuid, 0);

            packet.WriteGuid("Player GUID", playerGuid);
        }

        [Parser(Opcode.SMSG_CANCEL_AUTO_REPEAT)]
        public static void HandleCancelAutoRepeat(Packet packet)
        {
            packet.ReadPackedGuid("Target GUID");
        }

        [Parser(Opcode.SMSG_ATTACKSTART)]
        public static void HandleAttackStartStart(Packet packet)
        {
            packet.ReadGuid("GUID");
            packet.ReadGuid("Victim GUID");
        }

        [Parser(Opcode.SMSG_ATTACKSTOP)]
        [Parser(Opcode.SMSG_COMBAT_EVENT_FAILED)]
        public static void HandleAttackStartStop(Packet packet)
        {
            packet.ReadPackedGuid("GUID");
            packet.ReadPackedGuid("Victim GUID");
            packet.ReadInt32("Unk int"); // Has something to do with facing?
        }

        [Parser(Opcode.SMSG_ATTACKERSTATEUPDATE, ClientVersionBuild.V4_0_6_13596)]
        public static void HandleAttackerStateUpdate406(Packet packet)
        {
            var hitInfo = packet.ReadEnum<SpellHitInfo>("HitInfo", TypeCode.Int32);
            packet.ReadPackedGuid("AttackerGUID");
            packet.ReadPackedGuid("TargetGUID");
            packet.ReadInt32("Damage");
            packet.ReadInt32("OverDamage");

            var subDmgCount = packet.ReadByte("Count");
            for (var i = 0; i < subDmgCount; ++i)
            {
                packet.ReadInt32("SchoolMask", i);
                packet.ReadSingle("Float Damage", i);
                packet.ReadInt32("Int Damage", i);
            }

            if (hitInfo.HasAnyFlag(SpellHitInfo.HITINFO_PARTIAL_ABSORB | SpellHitInfo.HITINFO_FULL_ABSORB))
                for (var i = 0; i < subDmgCount; ++i)
                    packet.ReadInt32("Damage Absorbed", i);

                if (hitInfo.HasAnyFlag(SpellHitInfo.HITINFO_PARTIAL_RESIST | SpellHitInfo.HITINFO_FULL_RESIST))
                    for (var i = 0; i < subDmgCount; ++i)
                        packet.ReadInt32("Damage Resisted", i);

            packet.ReadEnum<VictimStates>("VictimState", TypeCode.Byte);
            packet.ReadInt32("Unk Attacker State 0");

            packet.ReadEntryWithName<Int32>(StoreNameType.Spell, "Melee Spell ID ");

            if (hitInfo.HasAnyFlag(SpellHitInfo.HITINFO_BLOCK))
                packet.ReadInt32("Block Amount");

            if (hitInfo.HasAnyFlag(SpellHitInfo.HITINFO_RAGE_GAIN))
                packet.ReadInt32("Rage Gained");

            if (hitInfo.HasAnyFlag(SpellHitInfo.HITINFO_UNK0))
            {
                packet.ReadInt32("Unk Attacker State 3 1");
                packet.ReadSingle("Unk Attacker State 3 2");
                packet.ReadSingle("Unk Attacker State 3 3");
                packet.ReadSingle("Unk Attacker State 3 4");
                packet.ReadSingle("Unk Attacker State 3 5");
                packet.ReadSingle("Unk Attacker State 3 6");
                packet.ReadSingle("Unk Attacker State 3 7");
                packet.ReadSingle("Unk Attacker State 3 8");
                packet.ReadSingle("Unk Attacker State 3 9");
                packet.ReadSingle("Unk Attacker State 3 10");
                packet.ReadSingle("Unk Attacker State 3 11");
                packet.ReadInt32("Unk Attacker State 3 12");
                packet.ReadInt32("Unk Attacker State 3 13");
            }

            if (ClientVersion.AddedInVersion(ClientVersionBuild.V5_1_0_16309))
                if (hitInfo.HasAnyFlag(SpellHitInfo.HITINFO_BLOCK | SpellHitInfo.HITINFO_UNK12))
                    packet.ReadSingle("Unk Float");
        }

        [Parser(Opcode.SMSG_ATTACKERSTATEUPDATE, ClientVersionBuild.Zero, ClientVersionBuild.V4_0_6_13596)]
        public static void HandleAttackerStateUpdate(Packet packet)
        {
            var hitInfo = packet.ReadEnum<SpellHitInfo>("HitInfo", TypeCode.Int32);
            packet.ReadPackedGuid("AttackerGUID");
            packet.ReadPackedGuid("TargetGUID");
            packet.ReadInt32("Damage");

            if (ClientVersion.AddedInVersion(ClientVersionBuild.V3_0_3_9183))
                packet.ReadInt32("OverDamage");

            var subDmgCount = packet.ReadByte();
            for (var i = 0; i < subDmgCount; ++i)
            {
                packet.ReadInt32("SchoolMask", i);
                packet.ReadSingle("Float Damage", i);
                packet.ReadInt32("Int Damage", i);

                if (ClientVersion.RemovedInVersion(ClientVersionBuild.V3_0_3_9183) ||
                    hitInfo.HasAnyFlag(SpellHitInfo.HITINFO_PARTIAL_ABSORB | SpellHitInfo.HITINFO_FULL_ABSORB))
                    packet.ReadInt32("Damage Absorbed", i);

                if (ClientVersion.RemovedInVersion(ClientVersionBuild.V3_0_3_9183) ||
                    hitInfo.HasAnyFlag(SpellHitInfo.HITINFO_PARTIAL_RESIST | SpellHitInfo.HITINFO_FULL_RESIST))
                    packet.ReadInt32("Damage Resisted", i);
            }

            var victimStateTypeCode = ClientVersion.AddedInVersion(ClientVersionBuild.V3_0_3_9183) ? TypeCode.Byte : TypeCode.Int32;
            packet.ReadEnum<VictimStates>("VictimState", victimStateTypeCode);
            packet.ReadInt32("Unk Attacker State 0");

            packet.ReadEntryWithName<Int32>(StoreNameType.Spell, "Melee Spell ID ");

            if (ClientVersion.RemovedInVersion(ClientVersionBuild.V3_0_3_9183) ||
                hitInfo.HasAnyFlag(SpellHitInfo.HITINFO_BLOCK))
                packet.ReadInt32("Block Amount");

            if (hitInfo.HasAnyFlag(SpellHitInfo.HITINFO_RAGE_GAIN))
                packet.ReadInt32("Rage Gained");

            if (hitInfo.HasAnyFlag(SpellHitInfo.HITINFO_UNK0))
            {
                packet.ReadInt32("Unk Attacker State 3 1");
                packet.ReadSingle("Unk Attacker State 3 2");
                packet.ReadSingle("Unk Attacker State 3 3");
                packet.ReadSingle("Unk Attacker State 3 4");
                packet.ReadSingle("Unk Attacker State 3 5");
                packet.ReadSingle("Unk Attacker State 3 6");
                packet.ReadSingle("Unk Attacker State 3 7");
                packet.ReadSingle("Unk Attacker State 3 8");
                packet.ReadSingle("Unk Attacker State 3 9");
                packet.ReadSingle("Unk Attacker State 3 10");
                packet.ReadSingle("Unk Attacker State 3 11");
                packet.ReadInt32("Unk Attacker State 3 12");
                packet.ReadInt32("Unk Attacker State 3 13");
                packet.ReadInt32("Unk Attacker State 3 14");
            }
        }

        [Parser(Opcode.SMSG_CANCEL_COMBAT)]
        public static void HandleCancelCombat(Packet packet)
        {
            var guid = new byte[8];
            packet.StartBitStream(guid, 2, 1, 7, 0, 5, 3, 4, 6);

            packet.ReadByte("Unk byte");

            packet.ReadXORByte(guid, 7);
            packet.ReadXORByte(guid, 4);
            packet.ReadXORByte(guid, 5);

            packet.ReadByte("Unk byte");
            packet.ReadUInt32("Unk uint32");

            packet.ReadXORByte(guid, 3);
            packet.ReadXORByte(guid, 6);
            packet.ReadXORByte(guid, 0);
            packet.ReadXORByte(guid, 1);
            packet.ReadXORByte(guid, 2);
        }

        [Parser(Opcode.SMSG_DUEL_OUTOFBOUNDS)]
        [Parser(Opcode.CMSG_ATTACKSTOP)]
        [Parser(Opcode.SMSG_ATTACKSWING_NOTINRANGE)]
        [Parser(Opcode.SMSG_ATTACKSWING_BADFACING)]
        [Parser(Opcode.SMSG_ATTACKSWING_DEADTARGET)]
        [Parser(Opcode.SMSG_ATTACKSWING_CANT_ATTACK)]
        public static void HandleCombatNull(Packet packet)
        {
        }
    }
}
