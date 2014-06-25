using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using WowPacketParser.Enums;
using WowPacketParser.Misc;
using WowPacketParser.Store;
using WowPacketParser.Store.Objects;

namespace WowPacketParser.Parsing.Parsers
{
    public static class PetHandler
    {
        public static void ReadPetFlags(ref Packet packet)
        {
            var petModeFlag = packet.ReadUInt32();
            packet.WriteLine("React state: " + (ReactState)((petModeFlag >> 8) & 0xFF));
            packet.WriteLine("Command state: " + (CommandState)((petModeFlag >> 16) & 0xFF));
            packet.WriteLine("Flag: " + (PetModeFlags)(petModeFlag & 0xFFFF0000));
        }

        [Parser(Opcode.SMSG_PET_SPELLS)]
        public static void HandlePetSpells(Packet packet)
        {
            var guid = packet.ReadGuid("GUID");
            // Equal to "Clear spells" pre cataclysm
            if (guid.Full == 0)
                return;

            if (ClientVersion.AddedInVersion(ClientVersionBuild.V3_1_0_9767))
                packet.ReadEnum<CreatureFamily>("Pet Family", TypeCode.UInt16); // vehicles -> 0

            if (ClientVersion.AddedInVersion(ClientVersionBuild.V5_1_0_16309))
                packet.ReadUInt16("Unk UInt16");

            packet.ReadUInt32("Expiration Time");

            ReadPetFlags(ref packet);

            var isPet = guid.GetHighType() == HighGuidType.Pet;
            var isVehicle = guid.GetHighType() == HighGuidType.Vehicle;
            var isMinion = guid.GetHighType() == HighGuidType.Unit;
            const int maxCreatureSpells = 10;
            var spells = new List<uint>(maxCreatureSpells);
            for (var i = 0; i < maxCreatureSpells; i++) // Read pet/vehicle spell ids
            {
                var spell16 = packet.ReadUInt16();
                var spell8 = packet.ReadByte();
                var spellId = spell16 + (spell8 << 16);
                var slot = packet.ReadByte();

                var s = new StringBuilder("[");
                s.Append(i).Append("] ").Append("Spell/Action: ");
                if (spellId <= 4)
                    s.Append(spellId);
                else
                    s.Append(StoreGetters.GetName(StoreNameType.Spell, spellId));
                s.Append(" slot: ").Append(slot);
                packet.WriteLine(s.ToString());

                // Spells for pets are on DBCs; also no entry in guid
                // We don't need the actions sent for minions (slots lower than 8)
                if (!isPet && (isVehicle || (isMinion && slot >= 8)))
                    spells.Add((uint)spellId);
            }

            if (spells.Count != 0)
            {
                SpellsX spellsCr;
                spellsCr.Spells = spells.ToArray();
                Storage.SpellsX.Add(guid.GetEntry(), spellsCr, packet.TimeSpan);
            }

            var spellCount = packet.ReadByte("Spell Count"); // vehicles -> 0, pets -> != 0. Could this be auras?
            for (var i = 0; i < spellCount; i++)
            {
                packet.ReadEntryWithName<UInt16>(StoreNameType.Spell, "Spell", i);
                packet.ReadInt16("Active", i);
            }

            var cdCount = packet.ReadByte("Cooldown count");
            for (var i = 0; i < cdCount; i++)
            {
                if (ClientVersion.AddedInVersion(ClientVersionBuild.V3_1_0_9767))
                    packet.ReadEntryWithName<UInt32>(StoreNameType.Spell, "Spell", i);
                else
                    packet.ReadEntryWithName<UInt16>(StoreNameType.Spell, "Spell", i);

                packet.ReadUInt16("Category", i);
                packet.ReadUInt32("Cooldown", i);
                packet.ReadUInt32("Category Cooldown", i);
            }

            if (ClientVersion.AddedInVersion(ClientVersionBuild.V5_1_0_16309))
            {
                var unkLoopCounter = packet.ReadByte("Unk count");
                for (var i = 0; i < unkLoopCounter; i++)
                {
                    packet.ReadUInt32("Unk UInt32 1", i);
                    packet.ReadByte("Unk Byte", i);
                    packet.ReadUInt32("Unk UInt32 2", i);
                }
            }
        }

        [Parser(Opcode.SMSG_PET_TAME_FAILURE)]
        public static void HandlePetTameFailure(Packet packet)
        {
            packet.ReadEnum<PetTameFailureReason>("Reason", TypeCode.Byte);
        }

        [Parser(Opcode.CMSG_PET_NAME_QUERY)]
        public static void HandlePetNameQuery(Packet packet)
        {
            var petNumber = new byte[8];
            var petGuid = new byte[8];

            petGuid[5] = packet.ReadBit();
            petNumber[3] = packet.ReadBit();
            petGuid[6] = packet.ReadBit();
            petNumber[5] = packet.ReadBit();
            petNumber[7] = packet.ReadBit();
            petGuid[2] = packet.ReadBit();
            petGuid[4] = packet.ReadBit();
            petNumber[2] = packet.ReadBit();
            petGuid[3] = packet.ReadBit();
            petNumber[1] = packet.ReadBit();
            petGuid[7] = packet.ReadBit();
            petNumber[6] = packet.ReadBit();
            petGuid[1] = packet.ReadBit();
            petGuid[0] = packet.ReadBit();
            petNumber[4] = packet.ReadBit();
            petNumber[0] = packet.ReadBit();

            packet.ResetBitReader();

            packet.ReadXORByte(petNumber, 5);
            packet.ReadXORByte(petGuid, 4);
            packet.ReadXORByte(petGuid, 3);
            packet.ReadXORByte(petNumber, 7);
            packet.ReadXORByte(petNumber, 4);
            packet.ReadXORByte(petGuid, 5);
            packet.ReadXORByte(petGuid, 2);
            packet.ReadXORByte(petGuid, 0);
            packet.ReadXORByte(petGuid, 6);
            packet.ReadXORByte(petNumber, 2);
            packet.ReadXORByte(petNumber, 0);
            packet.ReadXORByte(petNumber, 6);
            packet.ReadXORByte(petGuid, 1);
            packet.ReadXORByte(petNumber, 3);
            packet.ReadXORByte(petGuid, 7);
            packet.ReadXORByte(petNumber, 1);

            Console.WriteLine("petNumber: " + BitConverter.ToUInt64(petNumber, 0));
            packet.WriteGuid("Pet Guid", petGuid);
        }

        [Parser(Opcode.SMSG_PET_NAME_QUERY_RESPONSE)]
        public static void HandlePetNameQueryResponse(Packet packet)
        {
            packet.ReadUInt64("Pet number");

            var declined = packet.ReadBit() != 0;
            Console.WriteLine("Declined: " + declined);

            if (declined)
            {
                var names = new uint[5];
                for (var i = 0; i < 5; i++)
                    names[i] = packet.ReadBits("Declined Name Length", 7, i);

                var unkbit = packet.ReadBit() != 0;
                Console.WriteLine("unkbit: " + unkbit);

                var nameLen = packet.ReadBits("Name Length", 8);

                var realNames = new string[5];
                for (var i = 0; i < 5; i++)
                    realNames[i] = packet.ReadWoWString("Pet Declined Name", names[i], i);

                packet.ReadUInt32("Pet name timestamp");
                packet.ReadWoWString("Pet Name", nameLen);
            }

            /*
            var number = packet.ReadInt32("Pet number").ToString(CultureInfo.InvariantCulture);

            var petName = packet.ReadCString("Pet name");
            if (petName.Length == 0)
            {
                packet.ReadBytes(7); // 0s
                return;
            }

            var guidArray = (from pair in StoreGetters.NameDict where Equals(pair.Value, number) select pair.Key).ToList();
            foreach (var guid in guidArray)
                StoreGetters.NameDict[guid] = petName;

            packet.ReadTime("Time");
            var declined = packet.ReadBoolean("Declined");

            const int maxDeclinedNameCases = 5;

            if (declined)
                for (var i = 0; i < maxDeclinedNameCases; i++)
                    packet.ReadCString("Declined name", i);
            */
        }

        [Parser(Opcode.SMSG_PET_MODE)]
        public static void HandlePetMode(Packet packet)
        {
            packet.ReadGuid("Guid");
            ReadPetFlags(ref packet);
        }

        [Parser(Opcode.SMSG_PET_ACTION_SOUND)]
        public static void HandlePetSound(Packet packet)
        {
            packet.ReadGuid("GUID");
            packet.ReadInt32("Sound ID");
        }

        [Parser(Opcode.SMSG_PET_DISMISS_SOUND)]
        public static void HandlePetDismissSound(Packet packet)
        {
            packet.ReadInt32("Sound ID"); // CreatureSoundData.dbc - iRefID_soundPetDismissID
            packet.ReadVector3("Position");
        }

        [Parser(Opcode.CMSG_PET_SET_ACTION)]
        public static void HandlePetSetAction(Packet packet)
        {
            var i = 0;
            packet.ReadGuid("GUID");
            while (packet.CanRead())
            {
                packet.ReadUInt32("Position", i);
                var action = (uint)packet.ReadUInt16() + (packet.ReadByte() << 16);
                packet.WriteLine("[{0}] Action: {1}", i, action);
                packet.ReadEnum<ActionButtonType>("Type", TypeCode.Byte, i++);
            }
        }

        [Parser(Opcode.CMSG_PET_ACTION)]
        public static void HandlePetAction(Packet packet)
        {
            var playerGuid = new byte[8];
            var petGuid = new byte[8];

            packet.ReadUInt32("Datas");
            packet.ReadSingle("X");
            packet.ReadSingle("Z");
            packet.ReadSingle("Y");

            playerGuid[0] = packet.ReadBit();
            playerGuid[2] = packet.ReadBit();
            playerGuid[6] = packet.ReadBit();
            playerGuid[1] = packet.ReadBit();
            petGuid[6] = packet.ReadBit();
            petGuid[4] = packet.ReadBit();
            petGuid[0] = packet.ReadBit();
            petGuid[1] = packet.ReadBit();
            petGuid[3] = packet.ReadBit();
            playerGuid[3] = packet.ReadBit();
            petGuid[2] = packet.ReadBit();
            playerGuid[4] = packet.ReadBit();
            petGuid[5] = packet.ReadBit();
            petGuid[7] = packet.ReadBit();
            playerGuid[5] = packet.ReadBit();
            playerGuid[7] = packet.ReadBit();

            packet.ResetBitReader();

            packet.ReadXORByte(petGuid, 7);
            packet.ReadXORByte(playerGuid, 7);
            packet.ReadXORByte(petGuid, 6);
            packet.ReadXORByte(petGuid, 0);
            packet.ReadXORByte(petGuid, 3);
            packet.ReadXORByte(playerGuid, 1);
            packet.ReadXORByte(petGuid, 2);
            packet.ReadXORByte(petGuid, 1);
            packet.ReadXORByte(playerGuid, 2);
            packet.ReadXORByte(playerGuid, 5);
            packet.ReadXORByte(playerGuid, 6);
            packet.ReadXORByte(playerGuid, 0);
            packet.ReadXORByte(playerGuid, 3);
            packet.ReadXORByte(petGuid, 4);
            packet.ReadXORByte(petGuid, 5);
            packet.ReadXORByte(playerGuid, 4);

            packet.WriteGuid("Player GUID", playerGuid);
            packet.WriteGuid("Pet GUID", petGuid);


            /*packet.ReadGuid("GUID");
            var action = (uint)packet.ReadUInt16() + (packet.ReadByte() << 16);
            packet.WriteLine("Action: {0}", action);
            packet.ReadEnum<ActionButtonType>("Type", TypeCode.Byte);
            packet.ReadGuid("GUID");
            if (ClientVersion.AddedInVersion(ClientVersionBuild.V4_0_6_13596))
                packet.ReadVector3("Position");*/
        }

        [Parser(Opcode.CMSG_PET_CANCEL_AURA)]
        public static void HandlePetCancelAura(Packet packet)
        {
            packet.ReadGuid("GUID");
            packet.ReadEntryWithName<Int32>(StoreNameType.Spell, "Spell ID");
        }

        [Parser(Opcode.SMSG_PET_LEARNED_SPELL)]
        [Parser(Opcode.SMSG_PET_REMOVED_SPELL)]
        public static void HandlePetSpellsLearnedRemoved(Packet packet)
        {
            packet.ReadEntryWithName<Int32>(StoreNameType.Spell, "Spell ID");
        }

        [Parser(Opcode.SMSG_PET_ACTION_FEEDBACK)]
        public static void HandlePetActionFeedback(Packet packet)
        {
            var state = packet.ReadEnum<PetFeedback>("Pet state", TypeCode.Byte);

            switch (state)
            {
                case PetFeedback.NothingToAttack:
                    if (ClientVersion.AddedInVersion(ClientType.Cataclysm) || packet.CanRead())
                        packet.ReadEntryWithName<Int32>(StoreNameType.Spell, "Spell ID");
                    break;
                case PetFeedback.CantAttackTarget:
                    if (ClientVersion.AddedInVersion(ClientType.Cataclysm))
                        packet.ReadEntryWithName<Int32>(StoreNameType.Spell, "Spell ID");    // sub_8ADA60 2nd parameter is SpellID, check sub_8B22C0
                    break;
            }
        }

        [Parser(Opcode.CMSG_PET_STOP_ATTACK)]
        [Parser(Opcode.CMSG_DISMISS_CRITTER)]
        [Parser(Opcode.CMSG_PET_ABANDON)]
        public static void HandlePetMiscGuid(Packet packet)
        {
            packet.ReadGuid("GUID");
        }

        [Parser(Opcode.SMSG_PET_UPDATE_COMBO_POINTS)]
        public static void HandlePetUpdateComboPoints(Packet packet)
        {
            packet.ReadPackedGuid("Guid 1");
            packet.ReadPackedGuid("Guid 2");
            packet.ReadByte("Combo points");
        }

        [Parser(Opcode.SMSG_PET_GUIDS)]
        public static void HandlePetGuids(Packet packet)
        {
            var count = packet.ReadInt32("Count");
            for (var i = 0; i < count; ++i)
                packet.ReadGuid("Guid", i);
        }

        [Parser(Opcode.MSG_LIST_STABLED_PETS)]
        public static void HandleListStabledPets(Packet packet)
        {
            packet.ReadGuid("GUID");

            if (packet.Direction == Direction.ClientToServer)
                return;

            var count = packet.ReadByte("Count");
            packet.ReadByte("Stable Slots");

            for (var i = 0; i < count; i++)
            {
                if (ClientVersion.AddedInVersion(ClientVersionBuild.V4_2_2_14545)) // not verified
                    packet.ReadInt32("Pet Slot", i);

                packet.ReadInt32("Pet Number", i);
                packet.ReadEntryWithName<UInt32>(StoreNameType.Unit, "Pet Entry", i);
                packet.ReadInt32("Pet Level", i);
                packet.ReadCString("Pet Name", i);
                packet.ReadByte("Stable Type", i); // 1 = current, 2/3 = in stable
            }
        }

        [Parser(Opcode.CMSG_PET_CAST_SPELL)]
        public static void HandlePetCastSpell(Packet packet)
        {
            packet.ReadGuid("GUID");
            packet.ReadByte("Cast Count");
            packet.ReadEntryWithName<Int32>(StoreNameType.Spell, "Spell ID");
            var castFlags = packet.ReadEnum<CastFlag>("Cast Flags", TypeCode.Byte);
            SpellHandler.ReadSpellCastTargets(ref packet);
            if (castFlags.HasAnyFlag(CastFlag.HasTrajectory))
                SpellHandler.HandleSpellMissileAndMove(ref packet);
        }

        [Parser(Opcode.CMSG_REQUEST_PET_INFO)]
        public static void HandlePetNull(Packet packet)
        {
        }

        [Parser(Opcode.SMSG_PET_ADDED)] // 4.3.4
        public static void HandlePetAdded(Packet packet)
        {
            packet.ReadInt32("Pet Level");
            packet.ReadInt32("Pet Slot");
            packet.ReadByte("Stable Type");
            packet.ReadEntryWithName<UInt32>(StoreNameType.Unit, "Entry");
            packet.ReadInt32("Pet Number");

            var len = packet.ReadBits(8);
            packet.ReadWoWString("Pet Name", len);
        }

        [Parser(Opcode.CMSG_PET_RENAME)]
        public static void HandlePetRename(Packet packet)
        {
            packet.ReadGuid("Pet Guid");
            packet.ReadCString("Name");
            var declined = packet.ReadBoolean("Is Declined");
            if (declined)
                for (var i = 0; i < 5; ++i)
                    packet.ReadCString("Declined Name", i);
        }

        [Parser(Opcode.CMSG_PET_SPELL_AUTOCAST)]
        public static void HandlePetSpellAutocast(Packet packet)
        {
            packet.ReadGuid("Pet Guid");
            packet.ReadEntryWithName<UInt32>(StoreNameType.Spell, "Spell Id");
            packet.ReadByte("State");
        }
    }
}
