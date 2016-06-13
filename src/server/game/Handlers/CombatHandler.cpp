////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

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
    uint64 guid;
    recvData.readPackGUID(guid);

    Unit* pEnemy = ObjectAccessor::GetUnit(*m_Player, guid);

    if (!pEnemy)
    {
        // stop attack state at client
        SendAttackStop(NULL);
        return;
    }

    if (!m_Player->IsValidAttackTarget(pEnemy))
    {
        // stop attack state at client
        SendAttackStop(pEnemy);
        return;
    }

    //! Client explicitly checks the following before sending CMSG_ATTACKSWING packet,
    //! so we'll place the same check here. Note that it might be possible to reuse this snippet
    //! in other places as well.
    if (Vehicle* vehicle = m_Player->GetVehicle())
    {
        VehicleSeatEntry const* seat = vehicle->GetSeatForPassenger(m_Player);
        ASSERT(seat);
        if (!(seat->m_flags & VEHICLE_SEAT_FLAG_CAN_ATTACK))
        {
            SendAttackStop(pEnemy);
            return;
        }
    }

    m_Player->Attack(pEnemy, true);
}

void WorldSession::HandleAttackStopOpcode(WorldPacket & /*recvData*/)
{
    GetPlayer()->AttackStop();
}

void WorldSession::HandleSetSheathedOpcode(WorldPacket& recvData)
{
    uint32 sheathed;
    bool l_Animate;

    recvData >> sheathed;
    l_Animate = recvData.ReadBit(); ///< l_Animate is never read 01/18/16

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

    uint64 attackerGUID = GetPlayer()->GetGUID();
    uint64 victimGUID = enemy ? enemy->GetGUID() : 0;

    data.appendPackGUID(attackerGUID);
    data.appendPackGUID(victimGUID);
    data.WriteBit(enemy ? enemy->isDead() : false);
    data.FlushBits();

    SendPacket(&data);
}
