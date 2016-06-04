////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TradeData_h__
#define TradeData_h__

enum TradeSlots
{
    TRADE_SLOT_TRADED_COUNT     = 6,
    TRADE_SLOT_COUNT            = 7,
    TRADE_SLOT_NONTRADED        = 10,
    TRADE_SLOT_INVALID          = -1
};

class TradeData
{
    public:                                                 ///< constructors
        TradeData(Player* player, Player* trader) :
        m_player(player),  m_trader(trader), m_accepted(false), m_acceptProccess(false),
        m_money(0), m_spell(0), m_spellCastItem(0)
        {
            memset(m_items, 0, TRADE_SLOT_COUNT * sizeof(uint64));
        }
    
        Player* GetTrader() const { return m_trader; }
        TradeData* GetTraderData() const;
    
        Item* GetItem(TradeSlots slot) const;
        bool HasItem(uint64 itemGuid) const;
        TradeSlots GetTradeSlotForItem(uint64 itemGuid) const;
        void SetItem(TradeSlots slot, Item* item);
    
        uint32 GetSpell() const { return m_spell; }
        void SetSpell(uint32 spell_id, Item* castItem = NULL);
    
        Item*  GetSpellCastItem() const;
        bool HasSpellCastItem() const { return m_spellCastItem != 0; }
    
        uint64 GetMoney() const { return m_money; }
        void SetMoney(uint64 money);
    
        bool IsAccepted() const { return m_accepted; }
        void SetAccepted(bool state, bool crosssend = false);
    
        bool IsInAcceptProcess() const { return m_acceptProccess; }
        void SetInAcceptProcess(bool state) { m_acceptProccess = state; }
    
    private:                                                ///< internal functions
    
        void Update(bool for_trader = true);
    
    private:                                                ///< fields
    
        Player*    m_player;                                ///< Player who own of this TradeData
        Player*    m_trader;                                ///< Player who trade with m_player
    
        bool       m_accepted;                              ///< m_player press accept for trade list
        bool       m_acceptProccess;                        ///< one from player/trader press accept and this processed
    
        uint64     m_money;                                 ///< m_player place money to trade
    
        uint32     m_spell;                                 ///< m_player apply spell to non-traded slot item
        uint64     m_spellCastItem;                         ///< applied spell casted by item use
    
        uint64     m_items[TRADE_SLOT_COUNT];               ///< traded items from m_player side including non-traded slot
};

#endif // TradeData_h__
