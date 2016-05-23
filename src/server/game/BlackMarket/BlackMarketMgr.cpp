////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "DatabaseEnv.h"
#include "DBCStores.h"
#include "ScriptMgr.h"
#include "AccountMgr.h"
#include "BlackMarketMgr.h"
#include "Item.h"
#include "Language.h"
#include "Log.h"

// BMAuctionEntry
bool BMAuctionEntry::LoadFromDB(Field* fields)
{
    id          = fields[0].GetUInt32();
    templateId  = fields[1].GetUInt32();
    startTime   = fields[2].GetUInt32();
    bid         = fields[3].GetUInt64();
    bidder      = fields[4].GetUInt32();
    bidderCount = fields[5].GetUInt32();

    bm_template = sBlackMarketMgr->GetTemplate(templateId);
    if (!bm_template)
        return false;
    return true;
}

void BMAuctionEntry::SaveToDB(SQLTransaction& trans)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_BLACKMARKET_AUCTION);
    stmt->setUInt32(0, id);
    stmt->setUInt32(1, templateId);
    stmt->setUInt32(2, startTime);
    stmt->setUInt64(3, bid);
    stmt->setUInt32(4, bidder);
    stmt->setUInt32(5, bidderCount);
    trans->Append(stmt);
}

void BMAuctionEntry::DeleteFromDB(SQLTransaction& trans)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_BLACKMARKET_AUCTION);
    stmt->setUInt32(0, id);
    trans->Append(stmt);
}

void BMAuctionEntry::UpdateToDB(SQLTransaction& trans)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_BLACKMARKET_AUCTION);
    stmt->setUInt64(0, bid);
    stmt->setUInt32(1, bidder);
    stmt->setUInt32(2, bidderCount);
    stmt->setUInt32(3, id);
    trans->Append(stmt);
}

uint32 BMAuctionEntry::TimeLeft()
{
    uint32 endTime = startTime + bm_template->duration;
    uint32 curTime = time(NULL);
    return (endTime >= curTime) ? endTime - curTime : 0;
}

// BlackMarketMgr
BlackMarketMgr::BlackMarketMgr()
{
}

BlackMarketMgr::~BlackMarketMgr()
{
    for (BMAuctionEntryMap::const_iterator itr = GetAuctionsBegin(); itr != GetAuctionsEnd(); ++itr)
        delete itr->second;
    for (BMAuctionTemplateMap::const_iterator itr = GetTemplatesBegin(); itr != GetTemplatesEnd(); ++itr)
        delete itr->second;
}

void BlackMarketMgr::LoadTemplates()
{
    uint32 oldMSTime = getMSTime();

    PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_BLACKMARKET_TEMPLATE);
    PreparedQueryResult result = WorldDatabase.Query(stmt);

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 BlackMarket templates. DB table `blackmarket_template` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        BMAuctionTemplate* bm_template = new BMAuctionTemplate();
        bm_template->id         = fields[0].GetUInt32();
        bm_template->itemEntry  = fields[1].GetUInt32();
        bm_template->itemCount  = fields[2].GetUInt32();
        bm_template->seller     = fields[3].GetUInt32();
        bm_template->startBid   = fields[4].GetUInt64();
        bm_template->duration   = fields[5].GetUInt32();
        bm_template->chance     = fields[6].GetUInt32();

        BMTemplatesMap[bm_template->id] = bm_template;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u BlackMarket templates in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void BlackMarketMgr::LoadAuctions()
{
    uint32 oldMSTime = getMSTime();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_BLACKMARKET_AUCTIONS);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 BlackMarket Auctions. DB table `blackmarket` is empty.");
        return;
    }

    uint32 count = 0;

    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    do
    {
        Field* fields = result->Fetch();

        BMAuctionEntry* auction = new BMAuctionEntry();
        if (!auction->LoadFromDB(fields))
        {
            auction->DeleteFromDB(trans);
            delete auction;
            continue;
        }

        BMAuctionsMap[auction->id] = auction;

        ++count;
    }
    while (result->NextRow());

    CharacterDatabase.CommitTransaction(trans);

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u BlackMarket Auctions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void BlackMarketMgr::Update()
{
    uint32 curTime = time(NULL); ///< curTime is never read 01/18/16

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    // Delete expired auctions
    for (BMAuctionEntryMap::const_iterator itr = GetAuctionsBegin(); itr != GetAuctionsEnd();)
    {
        BMAuctionEntry* auction = itr->second;
        if (auction->IsExpired())
        {
            if (auction->bidder)
                SendAuctionWon(auction, trans);
            auction->DeleteFromDB(trans);
            BMAuctionsMap.erase((itr++)->first);
        }
        else
            ++itr;
    }

    // Add New Auctions;
    int32 add = sWorld->getIntConfig(CONFIG_BLACKMARKET_MAX_AUCTIONS) - BMAuctionsMap.size();
    if (add > 0)
        CreateAuctions(add, trans);

    CharacterDatabase.CommitTransaction(trans);
}

uint32 BlackMarketMgr::GetNewAuctionId()
{
    uint32 newId = 1;
    while (GetAuction(newId))
        ++newId;
    return newId;
}

uint64 BlackMarketMgr::GetAuctionOutBid(uint64 bid)
{
    // The / 10000 and * 10000 prevent sending price with silver or copper
    // Because the client allow only golds for Blackmarket price
    uint64 outbid = CalculatePct((bid / 10000), 5) * 10000;
    return outbid ? outbid : 1;
}

void BlackMarketMgr::CreateAuctions(uint32 number, SQLTransaction& trans)
{
    if (BMTemplatesMap.empty())
        return;

    for (uint32 i = 0; i < number; ++i)
    {
        // Select a template
        std::vector<uint32> templateList;
        uint32 rand = urand(1, 100);

        for (BMAuctionTemplateMap::const_iterator itr = GetTemplatesBegin(); itr != GetTemplatesEnd(); ++itr)
            if (itr->second->chance >= rand)
                templateList.push_back(itr->first);

        for (BMAuctionEntryMap::const_iterator itr = GetAuctionsBegin(); itr != GetAuctionsEnd(); ++itr)
            templateList.erase(std::remove(templateList.begin(), templateList.end(), itr->second->templateId), templateList.end());

        if (templateList.empty())
            continue;

        BMAuctionTemplate* selTemplate = GetTemplate(templateList[urand(0, (templateList.size() - 1))]);
        if (!selTemplate)
            continue;

        BMAuctionEntry* auction = new BMAuctionEntry;
        auction->id = GetNewAuctionId();
        auction->bid = selTemplate->startBid;
        auction->bidder = 0;
        auction->bidderCount = 0;
        auction->startTime = time(NULL) + sWorld->getIntConfig(CONFIG_BLACKMARKET_AUCTION_DELAY)
                                        + urand(0, sWorld->getIntConfig(CONFIG_BLACKMARKET_AUCTION_DELAY_MOD) * 2)
                                        - sWorld->getIntConfig(CONFIG_BLACKMARKET_AUCTION_DELAY_MOD) / 2;

        auction->bm_template = selTemplate;
        auction->templateId = selTemplate->id;

        BMAuctionsMap[auction->id] = auction;
        auction->SaveToDB(trans);
    }
}

void BlackMarketMgr::BuildBlackMarketAuctionsPacket(WorldPacket& p_Data, uint32 p_GuidLow)
{
    uint32 l_ItemCount = 0;
    ByteBuffer l_Datas(2 * 1024);

    for (BMAuctionEntryMap::const_iterator l_Iter = GetAuctionsBegin(); l_Iter != GetAuctionsEnd(); ++l_Iter)
    {
        BMAuctionEntry* l_Auction = l_Iter->second;
        if (l_Auction->IsActive())
        {
            ++l_ItemCount;

            uint64 l_CurrentBid = l_Auction->bidder ? l_Auction->bid : 0;
            uint64 l_MinBid = l_Auction->bidder ? l_Auction->bid + GetAuctionOutBid(l_Auction->bid) : l_Auction->bid;
            uint64 l_MinIncrement = l_Auction->bidder ? l_MinBid - l_CurrentBid : 1;

            l_Datas << int32(l_Auction->id);
            l_Datas << int32(l_Auction->bm_template->seller);

            Item::BuildDynamicItemDatas(l_Datas, l_Auction->bm_template->itemEntry);

            l_Datas << int32(l_Auction->bm_template->itemCount);
            l_Datas << uint64(l_MinBid);
            l_Datas << uint64(l_MinIncrement);
            l_Datas << uint64(l_CurrentBid);
            l_Datas << int32(l_Auction->TimeLeft());
            l_Datas << int32(l_Auction->bidderCount);
            l_Datas.WriteBit(p_GuidLow == l_Auction->bidder); ///< HighBid
        }
    }

    p_Data << int32(time(NULL));
    p_Data << uint32(l_ItemCount);
    p_Data.FlushBits();

    if (l_ItemCount)
        p_Data.append(l_Datas);
}

void BlackMarketMgr::UpdateAuction(BMAuctionEntry* auction, uint64 newPrice, Player* newBidder)
{
    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    if (auction->bidder)
        SendAuctionOutbidded(auction, newPrice, newBidder, trans);

    auction->bid = newPrice;
    auction->bidder = newBidder->GetGUIDLow();
    auction->bidderCount++;

    auction->UpdateToDB(trans);

    CharacterDatabase.CommitTransaction(trans);
}

std::string BMAuctionEntry::BuildAuctionMailSubject(BMMailAuctionAnswers response)
{
    std::ostringstream strm;
    strm << bm_template->itemEntry << ":0:" << response << ':' << id << ':' << bm_template->itemCount;
    return strm.str();
}

std::string BMAuctionEntry::BuildAuctionMailBody(uint32 lowGuid)
{
    std::ostringstream strm;
    strm.width(16);
    strm << std::right << std::hex << MAKE_NEW_GUID(lowGuid, 0, HIGHGUID_PLAYER);   // HIGHGUID_PLAYER always present, even for empty guids
    strm << std::dec << ':' << bid << ':' << 0;
    strm << ':' << 0 << ':' << 0;
    return strm.str();
}

void BlackMarketMgr::SendAuctionOutbidded(BMAuctionEntry* p_Auction, uint64 /*p_NewPrice*/, Player* /*p_NewBidder*/, SQLTransaction& p_Transaction)
{
    Player* l_Bidder = sObjectAccessor->FindPlayer(MAKE_NEW_GUID(p_Auction->bidder, 0, HIGHGUID_PLAYER));
    ItemTemplate const* l_ItemTemplate = sObjectMgr->GetItemTemplate(p_Auction->bm_template->itemEntry);
    if (!l_ItemTemplate)
        return;

    if (l_Bidder)
    {
        WorldPacket l_Data(SMSG_BLACK_MARKET_OUTBID, 200);
        l_Data << int32(0); ///< MarketID

        Item::BuildDynamicItemDatas(l_Data, l_ItemTemplate->ItemId);

        l_Data << uint32(l_ItemTemplate->RandomProperty);
        l_Bidder->GetSession()->SendPacket(&l_Data);
    }

    MailDraft(p_Auction->BuildAuctionMailSubject(BM_AUCTION_OUTBIDDED), p_Auction->BuildAuctionMailBody(p_Auction->bm_template->seller))
    .AddMoney(p_Auction->bid)
    .SendMailTo(p_Transaction, MailReceiver(l_Bidder, p_Auction->bidder), p_Auction, MAIL_CHECK_MASK_COPIED);
}

void BlackMarketMgr::SendAuctionWon(BMAuctionEntry* p_Auction, SQLTransaction& p_Transaction)
{
    uint64 l_BidderGuid = MAKE_NEW_GUID(p_Auction->bidder, 0, HIGHGUID_PLAYER);
    Player* l_Bidder = sObjectAccessor->FindPlayer(l_BidderGuid);
    ItemTemplate const* l_ItemTemplate = sObjectMgr->GetItemTemplate(p_Auction->bm_template->itemEntry);
    if (!l_ItemTemplate)
        return;

    if (l_Bidder)
    {
        WorldPacket l_Data(SMSG_BLACK_MARKET_WON, 12);
        l_Data << int32(0); ///< MarketID

        Item::BuildDynamicItemDatas(l_Data, l_ItemTemplate->ItemId);

        l_Data << uint32(l_ItemTemplate->RandomProperty);
        l_Bidder->GetSession()->SendPacket(&l_Data);
    }

    Item* l_Item = Item::CreateItem(p_Auction->bm_template->itemEntry, p_Auction->bm_template->itemCount, l_Bidder);
    l_Item->SetGuidValue(ITEM_FIELD_OWNER, l_BidderGuid);
    l_Item->SaveToDB(p_Transaction);

    MailDraft(p_Auction->BuildAuctionMailSubject(BM_AUCTION_WON), p_Auction->BuildAuctionMailBody(p_Auction->bidder))
    .AddItem(l_Item)
    .SendMailTo(p_Transaction, MailReceiver(l_Bidder, p_Auction->bidder), MailSender(p_Auction), MAIL_CHECK_MASK_COPIED);
}
