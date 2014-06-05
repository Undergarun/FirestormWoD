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

#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Vehicle.h"
#include "Player.h"
#include "Log.h"
#include "ObjectAccessor.h"

void WorldSession::HandleDismissControlledVehicle(WorldPacket &recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_DISMISS_CONTROLLED_VEHICLE");

    uint64 vehicleGUID = m_Player->GetCharmGUID();

    if (!vehicleGUID)                                       // something wrong here...
    {
        recvData.rfinish();                                // prevent warnings spam
        return;
    }

    // Too lazy to parse all data, just read pos and forge pkt
    MovementInfo mi;
    mi.guid = m_Player->GetGUID();
    mi.flags2 = MOVEMENTFLAG2_INTERPOLATED_PITCHING;
    mi.pos.m_positionX = recvData.read<float>();
    mi.pos.m_positionZ = recvData.read<float>();
    mi.pos.m_positionY = recvData.read<float>();
    mi.time = getMSTime();

    WorldPacket data(SMSG_MOVE_UPDATE);
    WorldSession::WriteMovementInfo(data, &mi);
    m_Player->SendMessageToSet(&data, m_Player);

    m_Player->m_movementInfo = mi;

    m_Player->ExitVehicle();

    // prevent warnings spam
    recvData.rfinish();
}

void WorldSession::HandleChangeSeatsOnControlledVehicle(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_CHANGE_SEATS_ON_CONTROLLED_VEHICLE");

    Unit* vehicle_base = GetPlayer()->GetVehicleBase();
    if (!vehicle_base)
    {
        recvData.rfinish();                                // prevent warnings spam
        return;
    }

    VehicleSeatEntry const* seat = GetPlayer()->GetVehicle()->GetSeatForPassenger(GetPlayer());
    if (!seat->CanSwitchFromSeat())
    {
        recvData.rfinish();                                // prevent warnings spam
        sLog->outError(LOG_FILTER_NETWORKIO, "HandleChangeSeatsOnControlledVehicle, Opcode: %u, Player %u tried to switch seats but current seatflags %u don't permit that.",
            recvData.GetOpcode(), GetPlayer()->GetGUIDLow(), seat->m_flags);
        return;
    }

    switch (recvData.GetOpcode())
    {/*
        case CMSG_REQUEST_VEHICLE_PREV_SEAT:
            GetPlayer()->ChangeSeat(-1, false);
            break;
        case CMSG_REQUEST_VEHICLE_NEXT_SEAT:
            GetPlayer()->ChangeSeat(-1, true);
            break;
            */
        case CMSG_CHANGE_SEATS_ON_CONTROLLED_VEHICLE:
        {
            float x, y, z;
            int8 seatId;
            ObjectGuid playerGUID;
            ObjectGuid accessoryGUID;
            ObjectGuid transportGUID;
            bool time2 = false;
            bool time3 = false;
            recvData >> seatId >> y >> x >> z;

            recvData.ReadBit();
            playerGUID[1] = recvData.ReadBit();
            playerGUID[7] = recvData.ReadBit();
            playerGUID[5] = recvData.ReadBit();
            recvData.ReadBit();
            accessoryGUID[6] = recvData.ReadBit();
            accessoryGUID[2] = recvData.ReadBit();
            bool hasUnkFloat = !recvData.ReadBit();
            accessoryGUID[4] = recvData.ReadBit();
            bool hastransport = recvData.ReadBit();
            bool hasUnk = !recvData.ReadBit();
            accessoryGUID[1] = recvData.ReadBit();
            playerGUID[2] = recvData.ReadBit();
            playerGUID[0] = recvData.ReadBit();
            accessoryGUID[0] = recvData.ReadBit();
            accessoryGUID[7] = recvData.ReadBit();
            playerGUID[4] = recvData.ReadBit();
            playerGUID[3] = recvData.ReadBit();
            accessoryGUID[5] = recvData.ReadBit();
            bool hasUnkFloat2 = recvData.ReadBit();
            accessoryGUID[3] = recvData.ReadBit();
            recvData.ReadBit();
            playerGUID[6] = recvData.ReadBit();
            bool unk = recvData.ReadBit();
            bool hasUnk2 = !recvData.ReadBit();
            bool hasUnk3 = !recvData.ReadBit();
            bool hasUnk4 = !recvData.ReadBit();
            bool hasUnkFloat3 = !recvData.ReadBit();
            uint32 unkcounter = recvData.ReadBits(23);
            if (unk)
                recvData.ReadBit();
            if (hastransport)
            {
                transportGUID[6] = recvData.ReadBit();
                transportGUID[1] = recvData.ReadBit();
                transportGUID[3] = recvData.ReadBit();
                transportGUID[4] = recvData.ReadBit();
                transportGUID[2] = recvData.ReadBit();
                transportGUID[5] = recvData.ReadBit();
                transportGUID[0] = recvData.ReadBit();
                time2 = recvData.ReadBit();
                transportGUID[7] = recvData.ReadBit();
                time3 = recvData.ReadBit();
            }

            if (hasUnk)
                recvData.ReadBits(13); // extra mov flags

            if (hasUnk3)
                recvData.ReadBits(30); // mov flags

            recvData.FlushBits();
            recvData.ReadByteSeq(playerGUID[7]);
            recvData.ReadByteSeq(accessoryGUID[4]);
            recvData.ReadByteSeq(playerGUID[4]);
            recvData.ReadByteSeq(playerGUID[1]);
            recvData.ReadByteSeq(accessoryGUID[7]);
            recvData.ReadByteSeq(accessoryGUID[0]);
            for (uint32 i = 0; i < unkcounter; i++)
                recvData.read<uint32>();
            recvData.ReadByteSeq(playerGUID[0]);
            recvData.ReadByteSeq(accessoryGUID[6]);
            recvData.ReadByteSeq(playerGUID[3]);
            recvData.ReadByteSeq(accessoryGUID[5]);
            recvData.ReadByteSeq(playerGUID[2]);
            recvData.ReadByteSeq(accessoryGUID[3]);
            recvData.ReadByteSeq(accessoryGUID[2]);
            recvData.ReadByteSeq(playerGUID[6]);
            recvData.ReadByteSeq(accessoryGUID[1]);



            
            /*recvData >> seatId;
            recvData >> y;
            recvData >> x;
            
            uint64 accessory;        // accessory vehicle guid
            recvData >> accessory;*/

            if (!accessoryGUID)
                GetPlayer()->ChangeSeat(-1, seatId > 0); // prev/next
            else if (Unit* vehUnit = Unit::GetUnit(*GetPlayer(), accessoryGUID))
            {
                if (Vehicle* vehicle = vehUnit->GetVehicleKit())
                    if (vehicle->HasEmptySeat(seatId))
                        vehUnit->HandleSpellClick(GetPlayer(), seatId);
            }
            break;
        }
        case CMSG_REQUEST_VEHICLE_SWITCH_SEAT:
        {
            ObjectGuid guid;

            int8 seatId;
            recvData >> seatId;

            uint8 bitOrder[8] = {2, 7, 4, 3, 0, 5, 1, 6};
            recvData.ReadBitInOrder(guid, bitOrder);

            uint8 byteOrder[8] = {5, 6, 4, 0, 1, 2, 7, 3};
            recvData.ReadBytesSeq(guid, byteOrder);

            if (vehicle_base->GetGUID() == guid)
                GetPlayer()->ChangeSeat(seatId);
            else if (Unit* vehUnit = Unit::GetUnit(*GetPlayer(), guid))
                if (Vehicle* vehicle = vehUnit->GetVehicleKit())
                    if (vehicle->HasEmptySeat(seatId))
                        vehUnit->HandleSpellClick(GetPlayer(), seatId);
            break;
        }
        default:
            break;
    }
}

void WorldSession::HandleEnterPlayerVehicle(WorldPacket& recvData)
{
    // Read guid
    ObjectGuid guid;

    uint8 bitOrder[8] = {0, 5, 1, 4, 2, 7, 3, 6};
    recvData.ReadBitInOrder(guid, bitOrder);

    uint8 byteOrder[8] = {0, 6, 2, 1, 4, 7, 3, 5};
    recvData.ReadBytesSeq(guid, byteOrder);

    if (Player* player = ObjectAccessor::FindPlayer(guid))
    {
        if (!player->GetVehicleKit())
            return;
        if (!player->IsInRaidWith(m_Player))
            return;
        if (!player->IsWithinDistInMap(m_Player, INTERACTION_DISTANCE))
            return;

        m_Player->EnterVehicle(player);
    }
}

void WorldSession::HandleEjectPassenger(WorldPacket& data)
{
    Vehicle* vehicle = m_Player->GetVehicleKit();
    if (!vehicle)
    {
        data.rfinish();                                     // prevent warnings spam
        sLog->outError(LOG_FILTER_NETWORKIO, "HandleEjectPassenger: Player %u is not in a vehicle!", GetPlayer()->GetGUIDLow());
        return;
    }

    ObjectGuid guid;
    uint8 bitOrder[8] = {3, 2, 4, 7, 5, 1, 6, 0};
    data.ReadBitInOrder(guid, bitOrder);
    uint8 byteOrder[8] = {5, 2, 3, 6, 0, 7, 4, 1};
    data.ReadBytesSeq(guid, byteOrder);

    if (IS_PLAYER_GUID(guid))
    {
        Player* player = ObjectAccessor::FindPlayer(guid);
        if (!player)
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Player %u tried to eject player %u from vehicle, but the latter was not found in world!", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
            return;
        }

        if (!player->IsOnVehicle(vehicle->GetBase()))
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Player %u tried to eject player %u, but they are not in the same vehicle", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
            return;
        }

        VehicleSeatEntry const* seat = vehicle->GetSeatForPassenger(player);
        ASSERT(seat);
        if (seat->IsEjectable())
            player->ExitVehicle();
        else
            sLog->outError(LOG_FILTER_NETWORKIO, "Player %u attempted to eject player %u from non-ejectable seat.", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
    }

    else if (IS_CREATURE_GUID(guid))
    {
        Unit* unit = ObjectAccessor::GetUnit(*m_Player, guid);
        if (!unit) // creatures can be ejected too from player mounts
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Player %u tried to eject creature guid %u from vehicle, but the latter was not found in world!", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
            return;
        }

        if (!unit->IsOnVehicle(vehicle->GetBase()))
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Player %u tried to eject unit %u, but they are not in the same vehicle", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
            return;
        }

        VehicleSeatEntry const* seat = vehicle->GetSeatForPassenger(unit);
        ASSERT(seat);
        if (seat->IsEjectable())
        {
            ASSERT(GetPlayer() == vehicle->GetBase());
            unit->ExitVehicle();
        }
        else
            sLog->outError(LOG_FILTER_NETWORKIO, "Player %u attempted to eject creature GUID %u from non-ejectable seat.", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
    }
}

void WorldSession::HandleRequestVehicleExit(WorldPacket& /*recvData*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_REQUEST_VEHICLE_EXIT");

    if (Vehicle* vehicle = GetPlayer()->GetVehicle())
    {
        if (VehicleSeatEntry const* seat = vehicle->GetSeatForPassenger(GetPlayer()))
        {
            if (seat->CanEnterOrExit())
                GetPlayer()->ExitVehicle();
            else
                sLog->outError(LOG_FILTER_NETWORKIO, "Player %u tried to exit vehicle, but seatflags %u (ID: %u) don't permit that.",
                GetPlayer()->GetGUIDLow(), seat->m_ID, seat->m_flags);
        }
    }
}
