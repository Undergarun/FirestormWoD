/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Opcodes.h"
#include "WorldSession.h"
#include "WorldPacket.h"
//#include "PacketStructures.h"

void WorldSession::SendAuthResponse(uint8 p_AuthResult, bool p_Queued, uint32 p_QueuePosition)
{
    WorldPacket l_Data(SMSG_AUTH_RESPONSE);

    uint32 l_RealmRaceCount = 15;
    uint32 l_RealmClassCount = 11;

    l_Data << uint8(p_AuthResult);
    l_Data.WriteBit(p_AuthResult == AUTH_OK);
    l_Data.WriteBit(p_Queued);
    l_Data.FlushBits();

    if (p_AuthResult == AUTH_OK)
    {
        l_Data << uint32(0);
        l_Data << uint32(0);
        l_Data << uint32(0);
        l_Data << uint32(0);
        l_Data << uint32(0);
        l_Data << uint8(Expansion());
        l_Data << uint8(Expansion());
        l_Data << uint32(0);
        l_Data << uint32(l_RealmClassCount);
        l_Data << uint32(l_RealmRaceCount);
        l_Data << uint32(0);
        l_Data << uint32(0);

        for (uint32 l_I = 0; l_I < l_RealmRaceCount; l_I++)
        {
            switch (l_I)
            {
                case 0:
                    l_Data << uint8(RACE_HUMAN);
                    l_Data << uint8(0);
                    break;
                case 1:
                    l_Data << uint8(RACE_ORC);
                    l_Data << uint8(0);
                    break;
                case 2:
                    l_Data << uint8(RACE_DWARF);
                    l_Data << uint8(0);
                    break;
                case 3:
                    l_Data << uint8(RACE_NIGHTELF);
                    l_Data << uint8(0);
                    break;
                case 4:
                    l_Data << uint8(RACE_UNDEAD_PLAYER);
                    l_Data << uint8(0);
                    break;
                case 5:
                    l_Data << uint8(RACE_TAUREN);
                    l_Data << uint8(0);
                    break;
                case 6:
                    l_Data << uint8(RACE_GNOME);
                    l_Data << uint8(0);
                    break;
                case 7:
                    l_Data << uint8(RACE_TROLL);
                    l_Data << uint8(0);
                    break;
                case 9:
                    l_Data << uint8(RACE_BLOODELF);
                    l_Data << uint8(1);
                    break;
                case 10:
                    l_Data << uint8(RACE_DRAENEI);
                    l_Data << uint8(1);
                    break;
                case 8:
                    l_Data << uint8(RACE_GOBLIN);
                    l_Data << uint8(3);
                    break;
                case 11:
                    l_Data << uint8(RACE_WORGEN);
                    l_Data << uint8(3);
                    break;
                case 12:
                    l_Data << uint8(RACE_PANDAREN_NEUTRAL);
                    l_Data << uint8(4);
                    break;
                case 13:
                    l_Data << uint8(RACE_PANDAREN_ALLI);
                    l_Data << uint8(4);
                    break;
                case 14:
                    l_Data << uint8(RACE_PANDAREN_HORDE);
                    l_Data << uint8(4);
                    break;
            }
        }

        for (uint32 l_I = 0; l_I < l_RealmClassCount; l_I++)
        {
            switch (l_I)
            {
                case 0:
                    l_Data << uint8(CLASS_WARRIOR);
                    l_Data << uint8(0); // Prebc
                    break;
                case 1:
                    l_Data << uint8(CLASS_PALADIN);
                    l_Data << uint8(0); // Prebc
                    break;
                case 2:
                    l_Data << uint8(CLASS_HUNTER);
                    l_Data << uint8(0); // Prebc
                    break;
                case 3:
                    l_Data << uint8(CLASS_ROGUE);
                    l_Data << uint8(0); // Prebc
                    break;
                case 4:
                    l_Data << uint8(CLASS_PRIEST);
                    l_Data << uint8(0); // Prebc
                    break;
                case 5:
                    l_Data << uint8(CLASS_DEATH_KNIGHT);
                    l_Data << uint8(2); // Wotlk
                    break;
                case 6:
                    l_Data << uint8(CLASS_SHAMAN);
                    l_Data << uint8(0); // Prebc
                    break;
                case 7:
                    l_Data << uint8(CLASS_MAGE);
                    l_Data << uint8(0); // Prebc
                    break;
                case 8:
                    l_Data << uint8(CLASS_WARLOCK);
                    l_Data << uint8(0); // Prebc
                    break;
                case 9:
                    l_Data << uint8(CLASS_DRUID);
                    l_Data << uint8(0); // Prebc
                    break;
                case 10:
                    l_Data << uint8(CLASS_MONK);
                    l_Data << uint8(4); // MoP
                    break;
            }
        }

        l_Data.WriteBit(0);
        l_Data.WriteBit(0);
        l_Data.WriteBit(0);
        l_Data.WriteBit(0);
        l_Data.WriteBit(0);
        l_Data.FlushBits();
    }

    if (p_Queued)
    {
        l_Data << uint32(p_QueuePosition);
        l_Data.WriteBit(p_Queued);
        l_Data.FlushBits();
    }

    SendPacket(&l_Data);
}

void WorldSession::SendClientCacheVersion(uint32 version)
{
    WorldPacket data(SMSG_CACHE_VERSION, 4);
    data << uint32(version);
    SendPacket(&data);
}
