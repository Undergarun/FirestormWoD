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
#include "GarrisonMgr.hpp"

void WorldSession::HandleTaxiNodeStatusQueryOpcode(WorldPacket& p_RecvData)
{
    uint64 l_UnitGuid;
    p_RecvData.readPackGUID(l_UnitGuid);

    SendTaxiStatus(l_UnitGuid);
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

    WorldPacket l_TaxiNodeStatusMsg(SMSG_TAXI_NODE_STATUS, 16 + 2 + 1);

    l_TaxiNodeStatusMsg.appendPackGUID(p_Guid);
    l_TaxiNodeStatusMsg.WriteBits(GetPlayer()->m_taxi.IsTaximaskNodeKnown(l_CurrentLocation) ? 1 : 0, 2);
    l_TaxiNodeStatusMsg.FlushBits();

    SendPacket(&l_TaxiNodeStatusMsg);
}

void WorldSession::HandleTaxiQueryAvailableNodes(WorldPacket& p_RecvPacket)
{
    Player* l_Player = GetPlayer();
    uint64 l_UnitGuid;

    if (l_Player == nullptr)
        return;

    p_RecvPacket.readPackGUID(l_UnitGuid);

    // cheating checks
    Creature* l_Unit = l_Player->GetNPCIfCanInteractWith(l_UnitGuid, UNIT_NPC_FLAG_FLIGHTMASTER);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleTaxiQueryAvailableNodes - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(l_UnitGuid)));
        return;
    }

    // remove fake death
    if (l_Player->HasUnitState(UNIT_STATE_DIED))
        l_Player->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    l_Player->TalkedToCreature(l_Unit->GetEntry(), l_UnitGuid);

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

    WorldPacket l_Data(SMSG_SHOW_TAXI_NODES, 1 * 1024);

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

        WorldPacket l_TaxiNodeStatusMsg(SMSG_TAXI_NODE_STATUS, 16 + 2 + 1);

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

void WorldSession::HandleMoveSplineDoneOpcode(WorldPacket& p_RecvPacket)
{
    HandleMovementOpcodes(p_RecvPacket);

    uint32 l_SplineID = p_RecvPacket.read<uint32>();

    // in taxi flight packet received in 2 case:
    // 1) end taxi path in far (multi-node) flight
    // 2) switch from one map to other in case multim-map taxi path
    // we need process only (1)

    uint32 curDest = GetPlayer()->m_taxi.GetTaxiDestination();
    if (!curDest)
        return;

    TaxiNodesEntry const* curDestNode = sTaxiNodesStore.LookupEntry(curDest);

    uint32 l_PlayerMapID = GetPlayer()->GetMapId();

    /// Special case for taxi in garrison phased map
    for (uint32 l_I = 0; l_I < sGarrSiteLevelStore.GetNumRows(); ++l_I)
    {
        const GarrSiteLevelEntry * l_Entry = sGarrSiteLevelStore.LookupEntry(l_I);

        if (l_Entry && l_Entry->MapID == l_PlayerMapID)
        {
            l_PlayerMapID = MS::Garrison::Globals::BaseMap;
            break;
        }
    }

    // far teleport case
    if (curDestNode && curDestNode->map_id != l_PlayerMapID)
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
    uint64 l_VendorGuid;
    std::vector<uint32> l_Nodes;

    l_Nodes.resize(2);

    p_RecvPacket.readPackGUID(l_VendorGuid);
    p_RecvPacket >> l_Nodes[1];

    l_Nodes[0] = sObjectMgr->GetNearestTaxiNode(GetPlayer()->GetPositionX(), GetPlayer()->GetPositionY(), GetPlayer()->GetPositionZ(), GetPlayer()->GetMapId(), GetPlayer()->GetTeam());
    if (!l_Nodes[0])
        return;

    Creature * l_Npc = GetPlayer()->GetNPCIfCanInteractWith(l_VendorGuid, UNIT_NPC_FLAG_FLIGHTMASTER);

    if (!l_Npc)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleActivateTaxiOpcode - Unit (GUID: %u) not found or you can't interact with it.", uint32(GUID_LOPART(l_VendorGuid)));
        return;
    }

    GetPlayer()->ActivateTaxiPathTo(l_Nodes, l_Npc);
}

void WorldSession::HandleTaxiRequestEarlyLandingOpcode(WorldPacket & p_Packet)
{
    if (m_Player && !m_Player->m_taxi.empty())
        m_Player->TaxiRequestEarlyLanding();
}

void WorldSession::SendActivateTaxiReply(ActivateTaxiReply p_Reply)
{
    WorldPacket l_Data(SMSG_ACTIVATE_TAXI_REPLY);
    l_Data.WriteBits(p_Reply, 4);
    l_Data.FlushBits();
    SendPacket(&l_Data);
}
