////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Vehicle.h"
#include "Player.h"
#include "Log.h"
#include "ObjectAccessor.h"

void WorldSession::HandleDismissControlledVehicle(WorldPacket& p_RecvData)
{
    if (!m_Player->GetCharmGUID())
    {
        p_RecvData.rfinish();
        return;
    }

    uint64 l_VehicleGuid = 0;
    p_RecvData.readPackGUID(l_VehicleGuid);

    if (l_VehicleGuid != m_Player->GetCharmGUID())
    {
        p_RecvData.rfinish();
        return;
    }

    p_RecvData.read_skip<uint32>();

    // Too lazy to parse all data, just read pos and forge pkt
    MovementInfo mi;
    mi.guid = m_Player->GetGUID();
    mi.flags2 = MOVEMENTFLAG2_INTERPOLATED_PITCHING;
    mi.pos.m_positionX = p_RecvData.read<float>();
    mi.pos.m_positionZ = p_RecvData.read<float>();
    mi.pos.m_positionY = p_RecvData.read<float>();
    mi.pos.m_orientation = p_RecvData.read<float>();
    mi.time = getMSTime();

    WorldPacket data(SMSG_MOVE_UPDATE);
    WorldSession::WriteMovementInfo(data, &mi);
    m_Player->SendMessageToSet(&data, m_Player);

    m_Player->m_movementInfo = mi;

    m_Player->ExitVehicle();

    p_RecvData.rfinish();
}

void WorldSession::HandleChangeSeatsOnControlledVehicle(WorldPacket& p_RecvData)
{
    Unit* l_VehicleBase = GetPlayer()->GetVehicleBase();
    if (!l_VehicleBase)
    {
        p_RecvData.rfinish();
        return;
    }

    VehicleSeatEntry const* l_VehicleSeat = GetPlayer()->GetVehicle()->GetSeatForPassenger(GetPlayer());
    if (!l_VehicleSeat->CanSwitchFromSeat())
    {
        p_RecvData.rfinish();
        sLog->outError(LOG_FILTER_NETWORKIO, "HandleChangeSeatsOnControlledVehicle, Opcode: %u, Player %u tried to switch seats but current seatflags %u don't permit that.",
            p_RecvData.GetOpcode(), GetPlayer()->GetGUIDLow(), l_VehicleSeat->m_flags);
        return;
    }

    switch (p_RecvData.GetOpcode())
    {
        case CMSG_REQUEST_VEHICLE_NEXT_SEAT:
        {
            m_Player->ChangeSeat(-1);
            SendCancelVehicleRideAura();
            break;
        }
        case CMSG_REQUEST_VEHICLE_PREV_SEAT:
        {
            m_Player->ChangeSeat(-1, false);
            SendCancelVehicleRideAura();
            break;
        }
        case CMSG_CHANGE_SEATS_ON_CONTROLLED_VEHICLE:
        {
            HandleMovementOpcodes(p_RecvData);
            int8 l_Seat;
            uint64 l_AccessoryGuid = 0;

            p_RecvData.readPackGUID(l_AccessoryGuid);
            p_RecvData >> l_Seat;

            if (!l_AccessoryGuid)
                GetPlayer()->ChangeSeat(-1, l_Seat > 0); // prev/next
            else if (Unit* l_VehicleUnit = Unit::GetUnit(*GetPlayer(), l_AccessoryGuid))
            {
                if (Vehicle* l_Vehicle = l_VehicleUnit->GetVehicleKit())
                {
                    if (l_Vehicle->HasEmptySeat(l_Seat))
                        l_VehicleUnit->HandleSpellClick(GetPlayer(), l_Seat);
                }
            }

            SendCancelVehicleRideAura();
            break;
        }
        case CMSG_REQUEST_VEHICLE_SWITCH_SEAT:
        {
            uint64 l_Guid = 0;

            int8 l_SeatID;
            p_RecvData.readPackGUID(l_Guid);
            p_RecvData >> l_SeatID;

            if (l_VehicleBase->GetGUID() == l_Guid)
                GetPlayer()->ChangeSeat(l_SeatID);
            else if (Unit* l_VehicleUnit = Unit::GetUnit(*GetPlayer(), l_Guid))
            {
                if (Vehicle* l_Vehicle = l_VehicleUnit->GetVehicleKit())
                {
                    if (l_Vehicle->HasEmptySeat(l_SeatID))
                        l_VehicleUnit->HandleSpellClick(GetPlayer(), l_SeatID);
                }
            }

            SendCancelVehicleRideAura();
            break;
        }
        default:
            break;
    }
}

void WorldSession::HandleEnterPlayerVehicle(WorldPacket& p_RecvData)
{
    uint64 l_Guid = 0;
    p_RecvData.readPackGUID(l_Guid);

    if (Player* l_Player = ObjectAccessor::FindPlayer(l_Guid))
    {
        /// Already in a vehicle or target not in a vehicle
        if (m_Player->GetVehicleKit() || !l_Player->GetVehicleKit())
            return;

        /// Only players in a group/raid can mount as passengers
        if (!m_Player->IsInRaidWith(l_Player))
            return;

        /// Distance check
        if (!m_Player->IsWithinDistInMap(l_Player, INTERACTION_DISTANCE))
            return;

        if (m_Player->CanMountAsPassenger(l_Player))
        {
            m_Player->EnterVehicle(l_Player);
            SendCancelVehicleRideAura();
        }
    }
}

void WorldSession::HandleEjectPassenger(WorldPacket& p_RecvData)
{
    Vehicle* vehicle = m_Player->GetVehicleKit();
    if (!vehicle)
    {
        p_RecvData.rfinish();
        sLog->outError(LOG_FILTER_NETWORKIO, "HandleEjectPassenger: Player %u is not in a vehicle!", GetPlayer()->GetGUIDLow());
        return;
    }

    uint64 l_Guid = 0;
    p_RecvData.readPackGUID(l_Guid);

    if (IS_PLAYER_GUID(l_Guid))
    {
        Player* l_Player = ObjectAccessor::FindPlayer(l_Guid);
        if (!l_Player)
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Player %u tried to eject player %u from vehicle, but the latter was not found in world!", GetPlayer()->GetGUIDLow(), GUID_LOPART(l_Guid));
            return;
        }

        if (!l_Player->IsOnVehicle(vehicle->GetBase()))
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Player %u tried to eject player %u, but they are not in the same vehicle", GetPlayer()->GetGUIDLow(), GUID_LOPART(l_Guid));
            return;
        }

        VehicleSeatEntry const* l_VehicleSeat = vehicle->GetSeatForPassenger(l_Player);
        ASSERT(l_VehicleSeat);
        if (l_VehicleSeat->IsEjectable())
            l_Player->ExitVehicle();
        else
            sLog->outError(LOG_FILTER_NETWORKIO, "Player %u attempted to eject player %u from non-ejectable seat.", GetPlayer()->GetGUIDLow(), GUID_LOPART(l_Guid));
    }

    else if (IS_CREATURE_GUID(l_Guid))
    {
        Unit* l_Unit = ObjectAccessor::GetUnit(*m_Player, l_Guid);
        if (!l_Unit) // creatures can be ejected too from player mounts
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Player %u tried to eject creature guid %u from vehicle, but the latter was not found in world!", GetPlayer()->GetGUIDLow(), GUID_LOPART(l_Guid));
            return;
        }

        if (!l_Unit->IsOnVehicle(vehicle->GetBase()))
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "Player %u tried to eject unit %u, but they are not in the same vehicle", GetPlayer()->GetGUIDLow(), GUID_LOPART(l_Guid));
            return;
        }

        VehicleSeatEntry const* l_VehicleSeat = vehicle->GetSeatForPassenger(l_Unit);
        ASSERT(l_VehicleSeat);
        if (l_VehicleSeat->IsEjectable())
        {
            ASSERT(GetPlayer() == vehicle->GetBase());
            l_Unit->ExitVehicle();
        }
        else
            sLog->outError(LOG_FILTER_NETWORKIO, "Player %u attempted to eject creature GUID %u from non-ejectable seat.", GetPlayer()->GetGUIDLow(), GUID_LOPART(l_Guid));
    }
}

void WorldSession::HandleRequestVehicleExit(WorldPacket& /*p_RecvData*/)
{
    if (Vehicle* l_Vehicle = GetPlayer()->GetVehicle())
    {
        if (VehicleSeatEntry const* l_VehicleSeat = l_Vehicle->GetSeatForPassenger(GetPlayer()))
        {
            if (l_VehicleSeat->CanEnterOrExit())
                GetPlayer()->ExitVehicle();
            else
                sLog->outError(LOG_FILTER_NETWORKIO, "Player %u tried to exit vehicle, but seatflags %u (ID: %u) don't permit that.", GetPlayer()->GetGUIDLow(), l_VehicleSeat->m_ID, l_VehicleSeat->m_flags);
        }
    }
}

void WorldSession::SendCancelVehicleRideAura()
{
    WorldPacket l_Data(SMSG_ON_CANCEL_EXPECTED_RIDE_VEHICLE_AURA, 0);
    SendPacket(&l_Data);
}
