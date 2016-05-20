////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/// TradeData

TradeData* TradeData::GetTraderData() const
{
    return m_trader->GetTradeData();
}

Item* TradeData::GetItem(TradeSlots slot) const
{
    if (slot >= TRADE_SLOT_COUNT)
        return nullptr;
    
    return m_items[slot] ? m_player->GetItemByGuid(m_items[slot]) : nullptr;
}

bool TradeData::HasItem(uint64 itemGuid) const
{
    for (uint8 i = 0; i < TRADE_SLOT_COUNT; ++i)
        if (m_items[i] == itemGuid)
            return true;
    
    return false;
}

TradeSlots TradeData::GetTradeSlotForItem(uint64 itemGuid) const
{
    for (uint8 i = 0; i < TRADE_SLOT_COUNT; ++i)
        if (m_items[i] == itemGuid)
            return TradeSlots(i);
    
    return TRADE_SLOT_INVALID;
}

Item* TradeData::GetSpellCastItem() const
{
    return m_spellCastItem ? m_player->GetItemByGuid(m_spellCastItem) : NULL;
}

void TradeData::SetItem(TradeSlots slot, Item* item)
{
    uint64 itemGuid = item ? item->GetGUID() : 0;
    
    if (m_items[slot] == itemGuid)
        return;
    
    m_items[slot] = itemGuid;
    
    SetAccepted(false);
    GetTraderData()->SetAccepted(false);
    
    Update();
    
    // need remove possible trader spell applied to changed item
    if (slot == TRADE_SLOT_TRADED_COUNT)
        GetTraderData()->SetSpell(0);
    
    // need remove possible player spell applied (possible move reagent)
    SetSpell(0);
}

void TradeData::SetSpell(uint32 spell_id, Item* castItem /*= NULL*/)
{
    uint64 itemGuid = castItem ? castItem->GetGUID() : 0;
    
    if (m_spell == spell_id && m_spellCastItem == itemGuid)
        return;
    
    m_spell = spell_id;
    m_spellCastItem = itemGuid;
    
    SetAccepted(false);
    GetTraderData()->SetAccepted(false);
    
    Update(true);                                           // send spell info to item owner
    Update(false);                                          // send spell info to caster self
}

void TradeData::SetMoney(uint64 money)
{
    if (m_money == money)
        return;
    
    m_money = money;
    
    SetAccepted(false);
    GetTraderData()->SetAccepted(false);
    
    Update(true);
}

void TradeData::Update(bool forTarget /*= true*/)
{
    if (forTarget)
        m_trader->GetSession()->SendUpdateTrade(true);      // player state for trader
    else
        m_player->GetSession()->SendUpdateTrade(false);     // player state for player
}

void TradeData::SetAccepted(bool state, bool crosssend /*= false*/)
{
    m_accepted = state;
    
    if (!state)
    {
        if (crosssend)
            m_trader->GetSession()->SendTradeStatus(TRADE_STATUS_UNACCEPTED);
        else
            m_player->GetSession()->SendTradeStatus(TRADE_STATUS_UNACCEPTED);
    }
}
