////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#ifndef _BLACK_MARKET_MGR_H
#define _BLACK_MARKET_MGR_H

#include "Common.h"
#include "DatabaseEnv.h"
#include "DBCStructure.h"

class Item;
class Player;
class WorldPacket;

enum BMMailAuctionAnswers
{
    BM_AUCTION_OUTBIDDED           = 0,
    BM_AUCTION_WON                 = 1
};

/// Last update : 6.0.3 19342
namespace BlackMarketBidResult
{
    enum
    {
        BidPlaced            = 0,
        ItemNotFound         = 1,
        AlreadyBid           = 2,
        HigherBid            = 4,
        NotEnoughMoney       = 7,
        RestrictedAccount    = 9,
        DatabaseError        = 6
    };
}

#define BLACKMARKET_AUCTION_HOUSE 7

struct BMAuctionTemplate
{
    uint32 id;
    uint32 itemEntry;
    uint32 itemCount;
    uint32 seller;
    uint32 duration;
    uint64 startBid;
    uint32 chance;
};

struct BMAuctionEntry
{
    uint32 id;
    uint32 templateId;
    uint32 startTime;
    uint64 bid;
    uint32 bidder;
    uint32 bidderCount;
    BMAuctionTemplate* bm_template;

    // helpers
    void DeleteFromDB(SQLTransaction& trans);
    void SaveToDB(SQLTransaction& trans);
    bool LoadFromDB(Field* fields);
    void UpdateToDB(SQLTransaction& trans);

    uint32 EndTime() { return startTime + bm_template->duration; }
    uint32 TimeLeft();
    bool IsActive() { return time(NULL) > startTime; }
    bool IsExpired() { return EndTime() <= time(NULL); }

    std::string BuildAuctionMailSubject(BMMailAuctionAnswers response);
    std::string BuildAuctionMailBody(uint32 lowGuid);
};

typedef struct BMAuctionTemplate BMAuctionTemplate;
typedef struct BMAuctionEntry BMAuctionEntry;

class BlackMarketMgr
{
    friend class ACE_Singleton<BlackMarketMgr, ACE_Null_Mutex>;

    private:
        BlackMarketMgr();
        ~BlackMarketMgr();

        typedef std::map<uint32, BMAuctionTemplate*> BMAuctionTemplateMap;
        typedef std::map<uint32, BMAuctionEntry*> BMAuctionEntryMap;

        BMAuctionTemplateMap BMTemplatesMap;
        BMAuctionEntryMap BMAuctionsMap;

    public:

        BMAuctionTemplate* GetTemplate(uint32 id) const
        {
            BMAuctionTemplateMap::const_iterator itr = BMTemplatesMap.find(id);
            return itr != BMTemplatesMap.end() ? itr->second : NULL;
        }

        uint32 GetTemplatesCount() { return BMTemplatesMap.size(); }

        BMAuctionTemplateMap::iterator GetTemplatesBegin() { return BMTemplatesMap.begin(); }
        BMAuctionTemplateMap::iterator GetTemplatesEnd() { return BMTemplatesMap.end(); }

        BMAuctionEntry* GetAuction(uint32 id) const
        {
            BMAuctionEntryMap::const_iterator itr = BMAuctionsMap.find(id);
            return itr != BMAuctionsMap.end() ? itr->second : NULL;
        }

        uint32 GetAuctionCount() { return BMAuctionsMap.size(); }

        BMAuctionEntryMap::iterator GetAuctionsBegin() { return BMAuctionsMap.begin(); }
        BMAuctionEntryMap::iterator GetAuctionsEnd() { return BMAuctionsMap.end(); }

        // Auction messages
        void SendAuctionWon(BMAuctionEntry* auction, SQLTransaction& trans);
        void SendAuctionOutbidded(BMAuctionEntry* auction, uint64 newPrice, Player* newBidder, SQLTransaction& trans);

        void LoadTemplates();
        void LoadAuctions();

        uint32 GetNewAuctionId();
        uint64 GetAuctionOutBid(uint64 bid);
        void CreateAuctions(uint32 number, SQLTransaction& trans);
        void UpdateAuction(BMAuctionEntry* auction, uint64 newPrice, Player* newBidder);

        void Update();

        void BuildBlackMarketAuctionsPacket(WorldPacket& p_Data, uint32 p_GuidLow);
};

#define sBlackMarketMgr ACE_Singleton<BlackMarketMgr, ACE_Null_Mutex>::instance()

#endif
#endif
