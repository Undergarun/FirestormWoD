/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "Common.h"
#include "Log.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "ObjectAccessor.h"
#include "CreatureAI.h"
#include "ObjectDefines.h"
#include "Vehicle.h"
#include "VehicleDefines.h"

void WorldSession::HandleAttackSwingOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;

    uint8 bitsOrder[8] = { 7, 6, 4, 3, 5, 0, 2, 1 };
    recvData.ReadBitInOrder(guid, bitsOrder);

    uint8 bytesOrder[8] = { 6, 3, 2, 5, 4, 7, 1, 0 };
    recvData.ReadBytesSeq(guid, bytesOrder);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_ATTACKSWING Message guidlow:%u guidhigh:%u", GUID_LOPART(guid), GUID_HIPART(guid));

    Unit* pEnemy = ObjectAccessor::GetUnit(*_player, guid);

    if (!pEnemy)
    {
        // stop attack state at client
        SendAttackStop(NULL);
        return;
    }

    if (!_player->IsValidAttackTarget(pEnemy))
    {
        // stop attack state at client
        SendAttackStop(pEnemy);
        return;
    }

    //! Client explicitly checks the following before sending CMSG_ATTACKSWING packet,
    //! so we'll place the same check here. Note that it might be possible to reuse this snippet
    //! in other places as well.
    if (Vehicle* vehicle = _player->GetVehicle())
    {
        VehicleSeatEntry const* seat = vehicle->GetSeatForPassenger(_player);
        ASSERT(seat);
        if (!(seat->m_flags & VEHICLE_SEAT_FLAG_CAN_ATTACK))
        {
            SendAttackStop(pEnemy);
            return;
        }
    }

    _player->Attack(pEnemy, true);
}

void WorldSession::HandleAttackStopOpcode(WorldPacket & /*recvData*/)
{
    GetPlayer()->AttackStop();
}

void WorldSession::HandleSetSheathedOpcode(WorldPacket& recvData)
{
    uint32 sheathed;
    recvData >> sheathed;

    //sLog->outDebug(LOG_FILTER_PACKETIO, "WORLD: Recvd CMSG_SETSHEATHED Message guidlow:%u value1:%u", GetPlayer()->GetGUIDLow(), sheathed);

    if (sheathed >= MAX_SHEATH_STATE)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Unknown sheath state %u ??", sheathed);
        return;
    }

    GetPlayer()->SetSheath(SheathState(sheathed));
}

void WorldSession::SendAttackStop(Unit const* enemy)
{
    WorldPacket data(SMSG_ATTACKSTOP);

    ObjectGuid attackerGuid = GetPlayer()->GetGUID();
    ObjectGuid victimGuid = enemy->GetGUID();

    data.WriteBits(1, 21); // unk 5.4.0

    uint8 playerOrder[8] = { 7, 4, 3, 2, 6, 1, 0, 5 };
    uint8 victimOrder[8] = { 2, 7, 4, 0, 1, 6, 3, 5 };

    data.WriteBitInOrder(attackerGuid, playerOrder);
    data.WriteBitInOrder(victimGuid, victimOrder);

    data.WriteByteSeq(attackerGuid[2]);
    data.WriteByteSeq(attackerGuid[5]);
    data.WriteByteSeq(attackerGuid[6]);
    data.WriteByteSeq(attackerGuid[0]);
    data.WriteByteSeq(attackerGuid[1]);
    data.WriteByteSeq(attackerGuid[4]);

    data << uint32(0);                                      // unk, can be 1 also

    data.WriteByteSeq(attackerGuid[7]);
    data.WriteByteSeq(attackerGuid[3]);

    uint8 bytesOrder[8] = { 1, 0, 6, 3, 2, 7, 5, 4 };
    data.WriteBytesSeq(victimGuid, bytesOrder);

    SendPacket(&data);
}
