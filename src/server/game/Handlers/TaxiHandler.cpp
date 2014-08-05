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

void WorldSession::SendTaxiStatus(uint64 p_Guid)
{
    // cheating checks
    Creature* l_Unit = GetPlayer()->GetMap()->GetCreature(p_Guid);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WorldSession::SendTaxiStatus - Unit (GUID: %u) not found.", uint32(GUID_LOPART(p_Guid)));
        return;
    }

    uint32 l_CurrentLocation = sObjectMgr->GetNearestTaxiNode(l_Unit->GetPositionX(), l_Unit->GetPositionY(), l_Unit->GetPositionZ(), l_Unit->GetMapId(), GetPlayer()->GetTeam());

    // not found nearest
    if (l_CurrentLocation == 0)
        return;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: current location %u ", l_CurrentLocation);

    WorldPacket l_TaxiNodeStatusMsg(SMSG_TAXI_NODE_STATUS, 9);

    l_TaxiNodeStatusMsg.appendPackGUID(p_Guid);
    l_TaxiNodeStatusMsg.WriteBits(GetPlayer()->m_taxi.IsTaximaskNodeKnown(l_CurrentLocation) ? 1 : 0, 2);
    l_TaxiNodeStatusMsg.FlushBits();

    SendPacket(&l_TaxiNodeStatusMsg);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_TAXI_NODE_STATUS");
}

void WorldSession::HandleTaxiQueryAvailableNodes(WorldPacket& p_RecvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_TAXIQUERYAVAILABLENODES");

    uint64 l_UnitGuid;

    p_RecvPacket.readPackGUID(l_UnitGuid);

    // cheating checks
    Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWith(l_UnitGuid, UNIT_NPC_FLAG_FLIGHTMASTER);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleTaxiQueryAvailableNodes - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(l_UnitGuid)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    // unknown taxi node case
    if (SendLearnNewTaxiNode(l_Unit))
        return;

    // known taxi node case
    SendTaxiMenu(l_Unit);
}

void WorldSession::SendTaxiMenu(Creature* p_Unit)
{
    // find current node
    uint32 l_CurrentLocation = sObjectMgr->GetNearestTaxiNode(p_Unit->GetPositionX(), p_Unit->GetPositionY(), p_Unit->GetPositionZ(), p_Unit->GetMapId(), GetPlayer()->GetTeam());

    if (!l_CurrentLocation)
        return;

    bool l_LastTaxiCheaterState = GetPlayer()->isTaxiCheater();
    bool l_HasData              = true;

    if (p_Unit->GetEntry() == 29480)
        GetPlayer()->SetTaxiCheater(true); // Grimwing in Ebon Hold, special case. NOTE: Not perfect, Zul'Aman should not be included according to WoWhead, and I think taxicheat includes it.

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_TAXINODE_STATUS_QUERY %u ", l_CurrentLocation);

    WorldPacket l_Data(SMSG_SHOW_TAXI_NODES, (4 + 8 + 4 + 8 * 4));

    l_Data.WriteBit(l_HasData);       // hasData
    l_Data.FlushBits();
    l_Data << uint32(TaxiMaskSize);

    if (l_HasData)
    {
        l_Data.appendPackGUID(p_Unit->GetGUID());
        l_Data << uint32(l_CurrentLocation);
    }

    GetPlayer()->m_taxi.AppendTaximaskTo(l_Data, GetPlayer()->isTaxiCheater());

    SendPacket(&l_Data);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_SHOW_TAXI_NODES");

    GetPlayer()->SetTaxiCheater(l_LastTaxiCheaterState);
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

bool WorldSession::SendLearnNewTaxiNode(Creature * p_Unit)
{
    // find current node
    uint32 l_CurrentLocation = sObjectMgr->GetNearestTaxiNode(p_Unit->GetPositionX(), p_Unit->GetPositionY(), p_Unit->GetPositionZ(), p_Unit->GetMapId(), GetPlayer()->GetTeam());

    if (l_CurrentLocation == 0)
        return true;                                        // `true` send to avoid WorldSession::SendTaxiMenu call with one more curlock seartch with same false result.

    if (GetPlayer()->m_taxi.SetTaximaskNode(l_CurrentLocation))
    {
        WorldPacket l_NewTaxiPathMsg(SMSG_NEW_TAXI_PATH, 0);
        SendPacket(&l_NewTaxiPathMsg);

        WorldPacket l_TaxiNodeStatusMsg(SMSG_TAXI_NODE_STATUS, 9);

        l_TaxiNodeStatusMsg.appendPackGUID(p_Unit->GetGUID());
        l_TaxiNodeStatusMsg.WriteBits(GetPlayer()->m_taxi.IsTaximaskNodeKnown(l_CurrentLocation) ? 1 : 0, 2);

        SendPacket(&l_TaxiNodeStatusMsg);

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

void WorldSession::HandleActivateTaxiExpressOpcode(WorldPacket& p_RecvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_ACTIVATETAXIEXPRESS");

    uint64 l_UnitGuid;
    uint32 l_NodeCount;

    std::vector<uint32> l_Nodes;

    p_RecvPacket.readPackGUID(l_UnitGuid);
    p_RecvPacket >> l_NodeCount;

    for (uint32 l_I = 0; l_I < l_NodeCount; ++l_I)
        l_Nodes.push_back(p_RecvPacket.read<uint32>());

    Creature* l_Npc = GetPlayer()->GetNPCIfCanInteractWith(l_UnitGuid, UNIT_NPC_FLAG_FLIGHTMASTER);

    if (!l_Npc)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleActivateTaxiExpressOpcode - Unit (GUID: %u) not found or you can't interact with it.", uint32(GUID_LOPART(l_UnitGuid)));
        return;
    }

    if (l_Nodes.empty())
        return;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_ACTIVATETAXIEXPRESS from %d to %d", l_Nodes.front(), l_Nodes.back());

    GetPlayer()->ActivateTaxiPathTo(l_Nodes, l_Npc);
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
                m_Player->GetSession()->SendPacket(&data);
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

void WorldSession::HandleActivateTaxiOpcode(WorldPacket& p_RecvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_ACTIVATETAXI");

    uint64 l_VendorGuid;
    std::vector<uint32> l_Nodes;

    l_Nodes.resize(2);

    p_RecvPacket.readPackGUID(l_VendorGuid);
    p_RecvPacket >> l_Nodes[0] >> l_Nodes[1];

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_ACTIVATETAXI from %d to %d", l_Nodes[0], l_Nodes[1]);

    Creature * l_Npc = GetPlayer()->GetNPCIfCanInteractWith(l_VendorGuid, UNIT_NPC_FLAG_FLIGHTMASTER);

    if (!l_Npc)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleActivateTaxiOpcode - Unit (GUID: %u) not found or you can't interact with it.", uint32(GUID_LOPART(l_VendorGuid)));
        return;
    }

    GetPlayer()->ActivateTaxiPathTo(l_Nodes, l_Npc);
}

void WorldSession::SendActivateTaxiReply(ActivateTaxiReply p_Reply)
{
    WorldPacket l_Data(SMSG_ACTIVATE_TAXI_REPLY);
    l_Data.WriteBits(p_Reply, 4);
    l_Data.FlushBits();
    SendPacket(&l_Data);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_ACTIVATE_TAXI_REPLY");
}
