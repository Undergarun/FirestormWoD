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
#include "DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "UpdateMask.h"
#include "Path.h"
#include "WaypointMovementGenerator.h"

void WorldSession::HandleTaxiNodeStatusQueryOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_TAXINODE_STATUS_QUERY");

    ObjectGuid guid;

    uint8 bitsOrder[8] = { 4, 7, 3, 1, 2, 0, 6, 5 };
    recvData.ReadBitInOrder(guid, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 2, 1, 5, 3, 7, 0, 6, 4 };
    recvData.ReadBytesSeq(guid, bytesOrder);

    SendTaxiStatus(guid);
}

void WorldSession::SendTaxiStatus(uint64 guid)
{
    // cheating checks
    Creature* unit = GetPlayer()->GetMap()->GetCreature(guid);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WorldSession::SendTaxiStatus - Unit (GUID: %u) not found.", uint32(GUID_LOPART(guid)));
        return;
    }

    uint32 curloc = sObjectMgr->GetNearestTaxiNode(unit->GetPositionX(), unit->GetPositionY(), unit->GetPositionZ(), unit->GetMapId(), GetPlayer()->GetTeam());

    // not found nearest
    if (curloc == 0)
        return;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: current location %u ", curloc);

    WorldPacket data(SMSG_TAXI_NODE_STATUS, 9);
    ObjectGuid npcGuid = guid;

    data.WriteBit(npcGuid[3]);
    data.WriteBit(npcGuid[2]);
    data.WriteBit(npcGuid[5]);
    data.WriteBit(npcGuid[0]);
    data.WriteBit(npcGuid[7]);
    data.WriteBits(GetPlayer()->m_taxi.IsTaximaskNodeKnown(curloc) ? 1 : 0, 2);
    data.WriteBit(npcGuid[6]);
    data.WriteBit(npcGuid[4]);
    data.WriteBit(npcGuid[1]);

    uint8 bytesOrder[8] = { 7, 3, 2, 1, 4, 6, 0, 5 };
    data.WriteBytesSeq(npcGuid, bytesOrder);

    SendPacket(&data);
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_TAXI_NODE_STATUS");
}

void WorldSession::HandleTaxiQueryAvailableNodes(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_TAXIQUERYAVAILABLENODES");

    ObjectGuid guid;

    uint8 bitsOrder[8] = { 0, 1, 7, 2, 4, 3, 5, 6 };
    recvData.ReadBitInOrder(guid, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 2, 3, 1, 0, 5, 4, 7, 6 };
    recvData.ReadBytesSeq(guid, bytesOrder);

    // cheating checks
    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_FLIGHTMASTER);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleTaxiQueryAvailableNodes - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(guid)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    // unknown taxi node case
    if (SendLearnNewTaxiNode(unit))
        return;

    // known taxi node case
    SendTaxiMenu(unit);
}

void WorldSession::SendTaxiMenu(Creature* unit)
{
    // find current node
    uint32 curloc = sObjectMgr->GetNearestTaxiNode(unit->GetPositionX(), unit->GetPositionY(), unit->GetPositionZ(), unit->GetMapId(), GetPlayer()->GetTeam());
    if (!curloc)
        return;

    bool lastTaxiCheaterState = GetPlayer()->isTaxiCheater();
    if (unit->GetEntry() == 29480)
        GetPlayer()->SetTaxiCheater(true); // Grimwing in Ebon Hold, special case. NOTE: Not perfect, Zul'Aman should not be included according to WoWhead, and I think taxicheat includes it.

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_TAXINODE_STATUS_QUERY %u ", curloc);

    WorldPacket data(SMSG_SHOW_TAXI_NODES, (4 + 8 + 4 + 8 * 4));
    ByteBuffer dataBuffer;
    ObjectGuid taxiGuid = unit->GetGUID();

    data.WriteBit(1);       // hasData

    uint8 bitsOrder[8] = { 5, 7, 2, 0, 4, 3, 1, 6 };
    data.WriteBitInOrder(taxiGuid, bitsOrder);

    GetPlayer()->m_taxi.AppendTaximaskTo(data, dataBuffer, GetPlayer()->isTaxiCheater());

    data.WriteByteSeq(taxiGuid[2]);
    data.WriteByteSeq(taxiGuid[3]);
    data.WriteByteSeq(taxiGuid[1]);
    data << uint32(curloc);
    data.WriteByteSeq(taxiGuid[0]);
    data.WriteByteSeq(taxiGuid[6]);
    data.WriteByteSeq(taxiGuid[7]);
    data.WriteByteSeq(taxiGuid[5]);
    data.WriteByteSeq(taxiGuid[4]);

    if (dataBuffer.size())
        data.append(dataBuffer);

    SendPacket(&data);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_SHOW_TAXI_NODES");

    GetPlayer()->SetTaxiCheater(lastTaxiCheaterState);
}

void WorldSession::SendDoFlight(uint32 mountDisplayId, uint32 path, uint32 pathNode)
{
    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    while (GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType() == FLIGHT_MOTION_TYPE)
        GetPlayer()->GetMotionMaster()->MovementExpired(false);

    if (mountDisplayId)
        GetPlayer()->Mount(mountDisplayId);

    GetPlayer()->GetMotionMaster()->MoveTaxiFlight(path, pathNode);
}

bool WorldSession::SendLearnNewTaxiNode(Creature* unit)
{
    // find current node
    uint32 curloc = sObjectMgr->GetNearestTaxiNode(unit->GetPositionX(), unit->GetPositionY(), unit->GetPositionZ(), unit->GetMapId(), GetPlayer()->GetTeam());

    if (curloc == 0)
        return true;                                        // `true` send to avoid WorldSession::SendTaxiMenu call with one more curlock seartch with same false result.

    if (GetPlayer()->m_taxi.SetTaximaskNode(curloc))
    {
        WorldPacket msg(SMSG_NEW_TAXI_PATH, 0);
        SendPacket(&msg);

        WorldPacket data(SMSG_TAXI_NODE_STATUS, 9);
        ObjectGuid npcGuid = unit->GetGUID();

        data.WriteBit(npcGuid[3]);
        data.WriteBit(npcGuid[2]);
        data.WriteBit(npcGuid[5]);
        data.WriteBit(npcGuid[0]);
        data.WriteBit(npcGuid[7]);
        data.WriteBits(GetPlayer()->m_taxi.IsTaximaskNodeKnown(curloc) ? 1 : 0, 2);
        data.WriteBit(npcGuid[6]);
        data.WriteBit(npcGuid[4]);
        data.WriteBit(npcGuid[1]);

        uint8 bytesOrder[8] = { 7, 3, 2, 1, 4, 6, 0, 5 };
        data.WriteBytesSeq(npcGuid, bytesOrder);

        SendPacket(&data);

        return true;
    }
    else
        return false;
}

void WorldSession::SendDiscoverNewTaxiNode(uint32 nodeid)
{
    if (GetPlayer()->m_taxi.SetTaximaskNode(nodeid))
    {
        WorldPacket msg(SMSG_NEW_TAXI_PATH, 0);
        SendPacket(&msg);
    }
}

void WorldSession::HandleActivateTaxiExpressOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_ACTIVATETAXIEXPRESS");

    ObjectGuid guid;
    uint32 node_count;

    guid[4] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();
    node_count = recvData.ReadBits(22);
    guid[0] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();

    recvData.FlushBits();

    recvData.ReadByteSeq(guid[2]);
    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[1]);
    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[3]);
    recvData.ReadByteSeq(guid[5]);

    std::vector<uint32> nodes;

    for (uint32 i = 0; i < node_count; ++i)
    {
        uint32 node;
        recvData >> node;
        nodes.push_back(node);
    }

    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[7]);

    Creature* npc = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_FLIGHTMASTER);
    if (!npc)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleActivateTaxiExpressOpcode - Unit (GUID: %u) not found or you can't interact with it.", uint32(GUID_LOPART(guid)));
        return;
    }

    if (nodes.empty())
        return;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_ACTIVATETAXIEXPRESS from %d to %d", nodes.front(), nodes.back());

    GetPlayer()->ActivateTaxiPathTo(nodes, npc);
}

void WorldSession::HandleMoveSplineDoneOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_MOVE_SPLINE_DONE");
    recvData.rfinish();

    // in taxi flight packet received in 2 case:
    // 1) end taxi path in far (multi-node) flight
    // 2) switch from one map to other in case multim-map taxi path
    // we need process only (1)

    uint32 curDest = GetPlayer()->m_taxi.GetTaxiDestination();
    if (!curDest)
        return;

    TaxiNodesEntry const* curDestNode = sTaxiNodesStore.LookupEntry(curDest);

    // far teleport case
    if (curDestNode && curDestNode->map_id != GetPlayer()->GetMapId())
    {
        if (GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType() == FLIGHT_MOTION_TYPE)
        {
            // short preparations to continue flight
            FlightPathMovementGenerator* flight = (FlightPathMovementGenerator*)(GetPlayer()->GetMotionMaster()->top());

            flight->SetCurrentNodeAfterTeleport();
            TaxiPathNodeEntry const& node = flight->GetPath()[flight->GetCurrentNode()];
            flight->SkipCurrentNode();

            GetPlayer()->TeleportTo(curDestNode->map_id, node.x, node.y, node.z, GetPlayer()->GetOrientation());
        }
        return;
    }

    uint32 destinationnode = GetPlayer()->m_taxi.NextTaxiDestination();
    if (destinationnode > 0)                              // if more destinations to go
    {
        // current source node for next destination
        uint32 sourcenode = GetPlayer()->m_taxi.GetTaxiSource();

        // Add to taximask middle hubs in taxicheat mode (to prevent having player with disabled taxicheat and not having back flight path)
        if (GetPlayer()->isTaxiCheater())
        {
            if (GetPlayer()->m_taxi.SetTaximaskNode(sourcenode))
            {
                WorldPacket data(SMSG_NEW_TAXI_PATH, 0);
                _player->GetSession()->SendPacket(&data);
            }
        }

        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Taxi has to go from %u to %u", sourcenode, destinationnode);

        uint32 mountDisplayId = sObjectMgr->GetTaxiMountDisplayId(sourcenode, GetPlayer()->GetTeam());

        uint32 path, cost;
        sObjectMgr->GetTaxiPath(sourcenode, destinationnode, path, cost);

        if (path && mountDisplayId)
            SendDoFlight(mountDisplayId, path, 1);        // skip start fly node
        else
            GetPlayer()->m_taxi.ClearTaxiDestinations();    // clear problematic path and next
        return;
    }
    else
        GetPlayer()->m_taxi.ClearTaxiDestinations();        // not destinations, clear source node

    GetPlayer()->CleanupAfterTaxiFlight();
    GetPlayer()->SetFallInformation(0, GetPlayer()->GetPositionZ());
    if (GetPlayer()->pvpInfo.inHostileArea)
        GetPlayer()->CastSpell(GetPlayer(), 2479, true);
}

void WorldSession::HandleActivateTaxiOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_ACTIVATETAXI");

    ObjectGuid guid;
    std::vector<uint32> nodes;
    nodes.resize(2);

    recvData >> nodes[0] >> nodes[1];

    uint8 bitsOrder[8] = { 6, 0, 4, 1, 5, 2, 3, 7 };
    recvData.ReadBitInOrder(guid, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 3, 0, 7, 1, 2, 6, 5, 4 };
    recvData.ReadBytesSeq(guid, bytesOrder);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_ACTIVATETAXI from %d to %d", nodes[0], nodes[1]);
    Creature* npc = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_FLIGHTMASTER);
    if (!npc)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleActivateTaxiOpcode - Unit (GUID: %u) not found or you can't interact with it.", uint32(GUID_LOPART(guid)));
        return;
    }

    GetPlayer()->ActivateTaxiPathTo(nodes, npc);
}

void WorldSession::SendActivateTaxiReply(ActivateTaxiReply reply)
{
    WorldPacket data(SMSG_ACTIVATE_TAXI_REPLY);
    data.WriteBits(reply, 4);
    data.FlushBits();
    SendPacket(&data);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_ACTIVATE_TAXI_REPLY");
}
