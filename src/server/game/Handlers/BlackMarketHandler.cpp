/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
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

#include "ObjectMgr.h"
#include "Player.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include "BlackMarket/BlackMarketMgr.h"
#include "Log.h"
#include "Language.h"
#include "Opcodes.h"
#include "UpdateMask.h"
#include "Util.h"
#include "AccountMgr.h"

void WorldSession::HandleBlackMarketHello(WorldPacket& recvData)
{
    ObjectGuid guid;

    uint8 bits[8] = { 0, 6, 7, 3, 5, 1, 2, 4 };
    recvData.ReadBitInOrder(guid, bits);

    recvData.FlushBits();

    uint8 bytes[8] = { 5, 1, 2, 6, 0, 7, 3, 4 };
    recvData.ReadBytesSeq(guid, bytes);

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_BLACK_MARKET);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleBlackMarketHello - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(guid)));
        return;
    }

    // Remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    SendBlackMarketHello(guid);
}

void WorldSession::SendBlackMarketHello(uint64 npcGuid)
{
    WorldPacket data(SMSG_BLACK_MARKET_OPEN_RESULT, 9);
    ObjectGuid guid = npcGuid;

    data.WriteBit(1);                // unk

    uint8 bits[8] = { 6, 3, 5, 7, 2, 4, 1, 0 };
    data.WriteBitInOrder(guid, bits);

    uint8 bytes[8] = { 5, 0, 1, 7, 3, 2, 6, 4 };
    data.WriteBytesSeq(guid, bytes);

    SendPacket(&data);
}

void WorldSession::HandleBlackMarketRequestItems(WorldPacket& recvData)
{
    ObjectGuid guid;
    uint32 unk;

    recvData >> unk;

    uint8 bits[8] = { 4, 1, 0, 6, 3, 2, 7, 5 };
    recvData.ReadBitInOrder(guid, bits);

    recvData.FlushBits();

    uint8 bytes[8] = { 0, 7, 6, 4, 3, 5, 1, 2 };
    recvData.ReadBytesSeq(guid, bytes);

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_BLACK_MARKET);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleBlackMarketRequestItems - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(guid)));
        return;
    }

    SendBlackMarketRequestItemsResult();
}

void WorldSession::SendBlackMarketRequestItemsResult()
{
    WorldPacket data(SMSG_BLACK_MARKET_ITEM_RESULT, 9);
    sBlackMarketMgr->BuildBlackMarketAuctionsPacket(data, GetPlayer()->GetGUIDLow());
    SendPacket(&data);
}

void WorldSession::HandleBlackMarketBid(WorldPacket& recvData)
{
    ObjectGuid guid;
    uint32 itemid, id;
    uint64 price;

    recvData >> price;
    recvData >> itemid;
    recvData >> id;

    uint8 bits[8] = { 2, 6, 3, 7, 1, 4, 0, 5 };
    recvData.ReadBitInOrder(guid, bits);

    recvData.FlushBits();

    uint8 bytes[8] = { 5, 1, 3, 0, 2, 6, 7, 4 };
    recvData.ReadBytesSeq(guid, bytes);

    sLog->outDebug(LOG_FILTER_NETWORKIO, ">> HandleBid >> id : %u, price : %u, itemid : %u", id, price, itemid);

    if (!price)
        return;

    Creature* creature = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_BLACK_MARKET);
    if (!creature)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleBlackMarketBid - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(guid)));
        return;
    }

    BMAuctionEntry* auction = sBlackMarketMgr->GetAuction(id);
    if (!auction)
        return;

    if (auction->bidder == GetPlayer()->GetGUIDLow()) // Trying to cheat
        return;

    if (auction->bid >= price && price != auction->bm_template->startBid) // Trying to cheat
        return;

    if (GetPlayer()->GetMoney() < price)
        return;

    GetPlayer()->ModifyMoney(-int64(price));

    sBlackMarketMgr->UpdateAuction(auction, price, GetPlayer());

    SendBlackMarketBidResult(itemid, id);
    SendBlackMarketRequestItemsResult();
}

void WorldSession::SendBlackMarketBidResult(uint32 itemEntry, uint32 auctionId)
{
    WorldPacket data(SMSG_BLACK_MARKET_BID_ON_ITEM_RESULT, 12);

    data << uint32(itemEntry);
    data << uint32(auctionId);
    data << uint32(0);

    SendPacket(&data);
}
