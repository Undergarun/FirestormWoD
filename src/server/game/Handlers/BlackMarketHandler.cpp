////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef CROSS
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

void WorldSession::HandleBlackMarketHello(WorldPacket& p_RecvData)
{
    uint64 l_Guid = 0;
    p_RecvData.readPackGUID(l_Guid);

    Creature* l_Vendor = GetPlayer()->GetNPCIfCanInteractWith(l_Guid, UNIT_NPC_FLAG_BLACK_MARKET);
    if (!l_Vendor)
        return;

    // Remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    SendBlackMarketHello(l_Guid);
}

void WorldSession::SendBlackMarketHello(uint64 p_Guid)
{
    WorldPacket l_Data(SMSG_BLACK_MARKET_OPEN_RESULT, 16 + 2 + 1);
    l_Data.appendPackGUID(p_Guid);
    l_Data.WriteBit(true);  ///< Open
    SendPacket(&l_Data);
}

void WorldSession::HandleBlackMarketRequestItems(WorldPacket& p_RecvData)
{
    uint64 l_Guid = 0;
    int32 l_LastUpdateID = 0;

    p_RecvData.readPackGUID(l_Guid);
    p_RecvData >> l_LastUpdateID;

    Creature* l_Vendor = GetPlayer()->GetNPCIfCanInteractWith(l_Guid, UNIT_NPC_FLAG_BLACK_MARKET);
    if (!l_Vendor)
        return;

    SendBlackMarketRequestItemsResult();
}

void WorldSession::SendBlackMarketRequestItemsResult()
{
    WorldPacket l_Data(SMSG_BLACK_MARKET_REQUEST_ITEMS_RESULT, 2 * 1024);
    sBlackMarketMgr->BuildBlackMarketAuctionsPacket(l_Data, m_Player->GetGUIDLow());
    SendPacket(&l_Data);
}

void WorldSession::HandleBlackMarketBid(WorldPacket& p_RecvData)
{
    uint64 l_Guid = 0;
    uint32 l_ItemID, l_MarketID = 0;
    uint64 l_Price = 0;

    p_RecvData.readPackGUID(l_Guid);
    p_RecvData >> l_MarketID;
    p_RecvData >> l_ItemID;

    p_RecvData.read_skip<uint32>(); ///< RandomPropertiesSeed
    p_RecvData.read_skip<uint32>(); ///< RandomPropertiesID
    bool l_HasBonus = p_RecvData.ReadBit();
    bool l_HasModifs = p_RecvData.ReadBit();
    p_RecvData.FlushBits();

    if (l_HasBonus)
    {
        p_RecvData.read_skip<uint8>(); ///< UnkByte
        uint32 l_BonusCount = p_RecvData.read<uint32>();

        for (uint32 l_I = 0; l_I < l_BonusCount; ++l_I)
            p_RecvData.read_skip<uint32>(); ///< BonusID
    }

    if (l_HasModifs)
    {
        uint32 l_ModifierMask = p_RecvData.read<uint32>();
        l_ModifierMask = ExtractBitMaskBitCount(l_ModifierMask);

        for (uint32 l_I = 0; l_I < l_ModifierMask; ++l_I)
            p_RecvData.read_skip<uint32>(); ///< ModifierValue
    }

    p_RecvData >> l_Price;

    if (!l_Price)
        return;

    Creature* l_Vendor = m_Player->GetNPCIfCanInteractWith(l_Guid, UNIT_NPC_FLAG_BLACK_MARKET);
    if (!l_Vendor)
        return;

    BMAuctionEntry* l_Auction = sBlackMarketMgr->GetAuction(l_MarketID);
    if (!l_Auction)
        return;

    if (l_Auction->bidder == m_Player->GetGUIDLow())
        return;

    if (l_Auction->bid >= l_Price && l_Price != l_Auction->bm_template->startBid)
        return;

    if (m_Player->GetMoney() < l_Price)
        return;

    m_Player->ModifyMoney(-int64(l_Price));

    sBlackMarketMgr->UpdateAuction(l_Auction, l_Price, m_Player);

    SendBlackMarketBidResult(l_ItemID, l_MarketID);
    SendBlackMarketRequestItemsResult();
}

void WorldSession::SendBlackMarketBidResult(uint32 p_ItemEntry, uint32 p_AuctionID)
{
    WorldPacket l_Data(SMSG_BLACK_MARKET_BID_ON_ITEM_RESULT, 100);
    l_Data << uint32(p_AuctionID);

    Item::BuildDynamicItemDatas(l_Data, p_ItemEntry);

    l_Data << uint32(BlackMarketBidResult::BidPlaced);    ///< Result OK
    SendPacket(&l_Data);
}
#endif