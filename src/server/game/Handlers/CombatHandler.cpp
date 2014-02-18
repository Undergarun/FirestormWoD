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
    bool unk;

    recvData >> sheathed;
    unk = recvData.ReadBit();

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
    WorldPacket data(SMSG_ATTACK_STOP);

    ObjectGuid victimGUID = enemy ? enemy->GetGUID() : 0;
    ObjectGuid attackerGUID = GetPlayer()->GetGUID();

    data.WriteBit(victimGUID[0]);
    data.WriteBit(attackerGUID[4]);
    data.WriteBit(victimGUID[1]);
    data.WriteBit(attackerGUID[7]);
    data.WriteBit(victimGUID[6]);
    data.WriteBit(victimGUID[3]);

    data.WriteBit(0);                   // Unk bit - updating rotation ?

    data.WriteBit(victimGUID[5]);
    data.WriteBit(attackerGUID[1]);
    data.WriteBit(attackerGUID[0]);
    data.WriteBit(victimGUID[7]);
    data.WriteBit(attackerGUID[6]);
    data.WriteBit(victimGUID[4]);
    data.WriteBit(victimGUID[2]);
    data.WriteBit(attackerGUID[3]);
    data.WriteBit(attackerGUID[2]);
    data.WriteBit(attackerGUID[5]);

    data.WriteByteSeq(attackerGUID[2]);
    data.WriteByteSeq(attackerGUID[7]);
    data.WriteByteSeq(victimGUID[0]);
    data.WriteByteSeq(attackerGUID[5]);
    data.WriteByteSeq(victimGUID[5]);
    data.WriteByteSeq(attackerGUID[3]);
    data.WriteByteSeq(victimGUID[7]);
    data.WriteByteSeq(victimGUID[1]);
    data.WriteByteSeq(victimGUID[3]);
    data.WriteByteSeq(attackerGUID[0]);
    data.WriteByteSeq(victimGUID[4]);
    data.WriteByteSeq(victimGUID[6]);
    data.WriteByteSeq(attackerGUID[1]);
    data.WriteByteSeq(attackerGUID[6]);
    data.WriteByteSeq(victimGUID[2]);
    data.WriteByteSeq(attackerGUID[4]);
    SendPacket(&data);
}
