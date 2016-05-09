////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

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
#include "TaxiPathGraph.h"
#include "GarrisonMgr.hpp"

void WorldSession::HandleEnableTaxiNodeOpcode(WorldPacket& p_RecvData)
{
    uint64 l_UnitGuid;
    p_RecvData.readPackGUID(l_UnitGuid);

    Creature* unit = GetPlayer()->GetMap()->GetCreature(l_UnitGuid);
    SendLearnNewTaxiNode(unit);
}

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
    uint32 l_Status = 0;

    if (!l_CurrentLocation)
        l_Status = TAXISTATUS_NONE;
    else if (l_Unit->GetReactionTo(GetPlayer()) >= REP_NEUTRAL)
        l_Status = GetPlayer()->m_taxi.IsTaximaskNodeKnown(l_CurrentLocation) ? TAXISTATUS_LEARNED : TAXISTATUS_UNLEARNED;
    else
        l_Status = TAXISTATUS_NOT_ELIGIBLE;

    WorldPacket l_TaxiNodeStatusMsg(SMSG_TAXI_NODE_STATUS, 16 + 2 + 1);

    l_TaxiNodeStatusMsg.appendPackGUID(p_Guid);
    l_TaxiNodeStatusMsg.WriteBits(l_Status, 2);
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
    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(l_UnitGuid, UNIT_NPC_FLAG_FLIGHTMASTER);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleTaxiQueryAvailableNodes - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(l_UnitGuid)));
        return;
    }
    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    l_Player->TalkedToCreature(unit->GetEntry(), l_UnitGuid);

    // unknown taxi node case
    if (SendLearnNewTaxiNode(unit))
        return;

    // known taxi node case
    SendTaxiMenu(unit);
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
        l_TaxiNodeStatusMsg.WriteBits(TAXISTATUS_LEARNED, 2);

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

void WorldSession::HandleActivateTaxiOpcode(WorldPacket& p_RecvPacket)
{
    uint64 l_VendorGuid;
    uint32 l_PacketNode = 0;
    
    p_RecvPacket.readPackGUID(l_VendorGuid);
    p_RecvPacket >> l_PacketNode;

    Creature * l_Npc = GetPlayer()->GetNPCIfCanInteractWith(l_VendorGuid, UNIT_NPC_FLAG_FLIGHTMASTER);

    if (!l_Npc)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleActivateTaxiOpcode - Unit (GUID: %u) not found or you can't interact with it.", uint32(GUID_LOPART(l_VendorGuid)));
        return;
    }

    uint32 curloc = sObjectMgr->GetNearestTaxiNode(l_Npc->GetPositionX(), l_Npc->GetPositionY(), l_Npc->GetPositionZ(), l_Npc->GetMapId(), GetPlayer()->GetTeam());
    if (!curloc)
        return;

    TaxiNodesEntry const* from = sTaxiNodesStore.LookupEntry(curloc);
    TaxiNodesEntry const* to = sTaxiNodesStore.LookupEntry(l_PacketNode);
    if (!to)
        return;

    if (!GetPlayer()->isTaxiCheater())
    {
        if (!GetPlayer()->m_taxi.IsTaximaskNodeKnown(curloc) || !GetPlayer()->m_taxi.IsTaximaskNodeKnown(l_PacketNode))
        {
            SendActivateTaxiReply(ERR_TAXINOTVISITED);
            return;
        }
    }

    std::vector<uint32> nodes;
    sTaxiPathGraph.GetCompleteNodeRoute(from, to, GetPlayer(), nodes);
    GetPlayer()->ActivateTaxiPathTo(nodes, l_Npc);
}

void WorldSession::SendActivateTaxiReply(ActivateTaxiReply p_Reply)
{
    WorldPacket l_Data(SMSG_ACTIVATE_TAXI_REPLY);
    l_Data.WriteBits(p_Reply, 4);
    l_Data.FlushBits();
    SendPacket(&l_Data);
}

void WorldSession::HandleTaxiRequestEarlyLandingOpcode(WorldPacket & /*p_Packet*/)
{
    if (GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType() == FLIGHT_MOTION_TYPE)
    {
        if (GetPlayer()->m_taxi.RequestEarlyLanding())
        {
            FlightPathMovementGenerator* flight = static_cast<FlightPathMovementGenerator*>(GetPlayer()->GetMotionMaster()->top());
            flight->LoadPath(GetPlayer(), flight->GetPath()[flight->GetCurrentNode()]->NodeIndex);
            flight->Reset(GetPlayer());
        }
    }
}

void WorldSession::HandleMoveSplineDoneOpcode(WorldPacket& p_RecvPacket)
{
    HandleMovementOpcodes(p_RecvPacket);

    if (p_RecvPacket.rpos() != p_RecvPacket.size())
        p_RecvPacket.read_skip<uint32>();

    /// in taxi flight packet received in 2 case:
    /// 1) end taxi path in far (multi-node) flight
    /// 2) switch from one map to other in case multim-map taxi path
    /// we need process only (1)

    uint32 curDest = GetPlayer()->m_taxi.GetTaxiDestination();
    if (curDest)
    {
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

        /// Far teleport case
        if (curDestNode && curDestNode->MapID != l_PlayerMapID)
        {
            if (GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType() == FLIGHT_MOTION_TYPE)
            {
                /// Short preparations to continue flight
                FlightPathMovementGenerator* flight = (FlightPathMovementGenerator*)(GetPlayer()->GetMotionMaster()->top());

                flight->SetCurrentNodeAfterTeleport();
                TaxiPathNodeEntry const* node = flight->GetPath()[flight->GetCurrentNode()];
                flight->SkipCurrentNode();

                GetPlayer()->TeleportTo(curDestNode->MapID, node->x, node->y, node->z, GetPlayer()->GetOrientation());
            }
        }

        return;
    }

    /// At this point only 1 node is expected (final destination)
    if (GetPlayer()->m_taxi.GetPath().size() != 1)
        return;

    GetPlayer()->CleanupAfterTaxiFlight();
    GetPlayer()->SetFallInformation(0, GetPlayer()->GetPositionZ());
    if (GetPlayer()->pvpInfo.inHostileArea)
        GetPlayer()->CastSpell(GetPlayer(), 2479, true);
}