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
#include "WorldPacket.h"
#include "WorldSession.h"
#include "World.h"
#include "ObjectAccessor.h"
#include "Log.h"
#include "Opcodes.h"
#include "Player.h"
#include "Item.h"
#include "Spell.h"
#include "SocialMgr.h"
#include "Language.h"
#include "AccountMgr.h"
#include "SpellAuraEffects.h"
#include "TradeData.h"

void WorldSession::SendTradeStatus(TradeStatus p_Status)
{
    WorldPacket l_Data(SMSG_TRADE_STATUS);
    l_Data.WriteBit(false);   // Failure for you
    l_Data.WriteBits(p_Status, 5);
    l_Data.WriteBit(false);

    switch (p_Status)
    {
        case TRADE_STATUS_TARGET_LOGGING_OUT:
            l_Data.WriteBit(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            break;
        case TRADE_STATUS_NOT_ON_TAPLIST:
            l_Data << uint8(0); // unk
            break;
        case TRADE_STATUS_INITIATED:
            m_Player->InitializeClientStateIndex();
            l_Data << uint32(1);
            break;
        default:
            break;
    }

    SendPacket(&l_Data);
}

void WorldSession::HandleIgnoreTradeOpcode(WorldPacket& /*p_RecvData*/)
{
    SendTradeStatus(TRADE_STATUS_CANCELLED);
}

void WorldSession::SendUpdateTrade(bool p_WhichPlayer /*= true*/)
{
    TradeData* l_ViewTrade = p_WhichPlayer ? m_Player->GetTradeData()->GetTraderData() : m_Player->GetTradeData();

    uint8 l_Count = 0;
    for (uint8 l_Iter = 0; l_Iter < TRADE_SLOT_COUNT; ++l_Iter)
    {
        if (l_ViewTrade->GetItem(TradeSlots(l_Iter)))
            ++l_Count;
    }

    WorldPacket l_Data(SMSG_TRADE_UPDATED, 1024);
    l_Data << uint8(p_WhichPlayer);
    l_Data << uint32(1);
    l_Data << uint32(m_Player->GetClientStateIndex());
    l_Data << uint32(m_Player->GetClientStateIndex());
    l_Data << uint64(l_ViewTrade->GetMoney());
    l_Data << int32(0); // CurrencyType
    l_Data << int32(0); // CurrencyQuantity
    l_Data << int32(l_ViewTrade->GetSpell());
    l_Data << uint32(l_Count);

    for (uint8 l_Iter = 0; l_Iter < TRADE_SLOT_COUNT; ++l_Iter)
    {
        Item* l_Item = l_ViewTrade->GetItem(TradeSlots(l_Iter));
        if (!l_Item)
            continue;

        uint64 l_GiftCreator = l_Item->GetGuidValue(ITEM_FIELD_GIFT_CREATOR);
        uint64 l_Creator = l_Item->GetGuidValue(ITEM_FIELD_CREATOR);

        l_Data << l_Iter;
        l_Data << uint32(l_Item->GetTemplate()->ItemId);
        l_Data << uint32(l_Item->GetCount());
        l_Data.appendPackGUID(l_GiftCreator);
        l_Data.WriteBit(true);  // IsWrapped - Always true on retail

        Item::BuildDynamicItemDatas(l_Data, l_Item);

        l_Data << int32(0);     // EnchantID
        l_Data << int32(0);     // OnUseEnchantmentID

        for (uint32 l_SlotEnchant = SOCK_ENCHANTMENT_SLOT; l_SlotEnchant < SOCK_ENCHANTMENT_SLOT + MAX_GEM_SOCKETS; ++l_SlotEnchant)
            l_Data << uint32(l_Item->GetEnchantmentId(EnchantmentSlot(l_SlotEnchant)));

        l_Data.appendPackGUID(l_Creator);
        l_Data << int32(l_Item->GetSpellCharges());
        l_Data << int32(l_Item->GetUInt32Value(ITEM_FIELD_MAX_DURABILITY));
        l_Data << int32(l_Item->GetUInt32Value(ITEM_FIELD_DURABILITY));
        l_Data.WriteBit(l_Item->GetTemplate()->LockID != 0);
    }

    SendPacket(&l_Data);
}

//==============================================================
// transfer the items to the players

void WorldSession::moveItems(Item* myItems[], Item* hisItems[])
{
    Player* trader = m_Player->GetTrader();
    if (!trader)
        return;

    for (uint8 i = 0; i < TRADE_SLOT_TRADED_COUNT; ++i)
    {
        ItemPosCountVec traderDst;
        ItemPosCountVec playerDst;
        bool traderCanTrade = (myItems[i] == NULL || trader->CanStoreItem(NULL_BAG, NULL_SLOT, traderDst, myItems[i], false) == EQUIP_ERR_OK);
        bool playerCanTrade = (hisItems[i] == NULL || m_Player->CanStoreItem(NULL_BAG, NULL_SLOT, playerDst, hisItems[i], false) == EQUIP_ERR_OK);
        if (traderCanTrade && playerCanTrade)
        {
            // Ok, if trade item exists and can be stored
            // If we trade in both directions we had to check, if the trade will work before we actually do it
            // A roll back is not possible after we stored it
            if (myItems[i])
            {
                if (!AccountMgr::IsPlayerAccount(m_Player->GetSession()->GetSecurity()) && sWorld->getBoolConfig(CONFIG_GM_LOG_TRADE))
                {
                    sLog->outCommand(m_Player->GetSession()->GetAccountId(), "", m_Player->GetGUIDLow(), m_Player->GetName(),
                                    trader->GetSession()->GetAccountId(), "", trader->GetGUIDLow(), trader->GetName(),
                                    "GM %s (Account: %u) trade: %s (Entry: %d Count: %u) to player: %s (Account: %u)",
                                    m_Player->GetName(), m_Player->GetSession()->GetAccountId(),
                                    myItems[i]->GetTemplate()->Name1->Get(sWorld->GetDefaultDbcLocale()), myItems[i]->GetEntry(), myItems[i]->GetCount(),
                                    trader->GetName(), trader->GetSession()->GetAccountId());
                }

                /*CharacterDatabase.PExecute("INSERT INTO log_trade (id, date, sc_accountid, sc_guid, sc_name, tar_accountid, tar_guid, tar_name, item_name, item_entry, item_count) VALUES (0, NOW(), %u, %u, '%s', %u, %u, '%s', '%s', %u, %u);",
                                            _player->GetSession()->GetAccountId(), _player->GetGUIDLow(), _player->GetName(),
                                            trader->GetSession()->GetAccountId(),  trader->GetGUIDLow(),  trader->GetName(),
                                            myItems[i]->GetTemplate()->Name1.c_str(), myItems[i]->GetEntry(), myItems[i]->GetCount());*/

                // adjust time (depends on /played)
                if (myItems[i]->HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FIELD_FLAG_BOP_TRADEABLE))
                    myItems[i]->SetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME, trader->GetTotalPlayedTime()-(m_Player->GetTotalPlayedTime()-myItems[i]->GetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME)));
                // store
                trader->MoveItemToInventory(traderDst, myItems[i], true, true);
            }
            if (hisItems[i])
            {
                if (!AccountMgr::IsPlayerAccount(trader->GetSession()->GetSecurity()) && sWorld->getBoolConfig(CONFIG_GM_LOG_TRADE))
                {
                    sLog->outCommand(trader->GetSession()->GetAccountId(), "", trader->GetGUIDLow(), trader->GetName(),
                                    m_Player->GetSession()->GetAccountId(), "", m_Player->GetGUIDLow(), m_Player->GetName(),
                                    "GM %s (Account: %u) trade: %s (Entry: %d Count: %u) to player: %s (Account: %u)",
                                    trader->GetName(), trader->GetSession()->GetAccountId(),
                                    hisItems[i]->GetTemplate()->Name1->Get(sWorld->GetDefaultDbcLocale()), hisItems[i]->GetEntry(), hisItems[i]->GetCount(),
                                    m_Player->GetName(), m_Player->GetSession()->GetAccountId());
                }

                /*CharacterDatabase.PExecute("INSERT INTO log_trade (id, date, sc_accountid, sc_guid, sc_name, tar_accountid, tar_guid, tar_name, item_name, item_entry, item_count) VALUES (0, NOW(), %u, %u, '%s', %u, %u, '%s', '%s', %u, %u);",
                                            trader->GetSession()->GetAccountId(),  trader->GetGUIDLow(),  trader->GetName(),
                                            _player->GetSession()->GetAccountId(), _player->GetGUIDLow(), _player->GetName(),
                                            hisItems[i]->GetTemplate()->Name1.c_str(), hisItems[i]->GetEntry(), hisItems[i]->GetCount());*/

                // adjust time (depends on /played)
                if (hisItems[i]->HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FIELD_FLAG_BOP_TRADEABLE))
                    hisItems[i]->SetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME, m_Player->GetTotalPlayedTime()-(trader->GetTotalPlayedTime()-hisItems[i]->GetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME)));
                // store
                m_Player->MoveItemToInventory(playerDst, hisItems[i], true, true);
            }
        }
        else
        {
            // in case of fatal error log error message
            // return the already removed items to the original owner
            if (myItems[i])
            {
                if (!traderCanTrade)
                    sLog->outError(LOG_FILTER_NETWORKIO, "trader can't store item: %u", myItems[i]->GetGUIDLow());
                if (m_Player->CanStoreItem(NULL_BAG, NULL_SLOT, playerDst, myItems[i], false) == EQUIP_ERR_OK)
                    m_Player->MoveItemToInventory(playerDst, myItems[i], true, true);
                else
                    sLog->outError(LOG_FILTER_NETWORKIO, "player can't take item back: %u", myItems[i]->GetGUIDLow());
            }
            // return the already removed items to the original owner
            if (hisItems[i])
            {
                if (!playerCanTrade)
                    sLog->outError(LOG_FILTER_NETWORKIO, "player can't store item: %u", hisItems[i]->GetGUIDLow());
                if (trader->CanStoreItem(NULL_BAG, NULL_SLOT, traderDst, hisItems[i], false) == EQUIP_ERR_OK)
                    trader->MoveItemToInventory(traderDst, hisItems[i], true, true);
                else
                    sLog->outError(LOG_FILTER_NETWORKIO, "trader can't take item back: %u", hisItems[i]->GetGUIDLow());
            }
        }
    }
}

//==============================================================

static void setAcceptTradeMode(TradeData* myTrade, TradeData* hisTrade, Item* *myItems, Item* *hisItems)
{
    myTrade->SetInAcceptProcess(true);
    hisTrade->SetInAcceptProcess(true);

    // store items in local list and set 'in-trade' flag
    for (uint8 i = 0; i < TRADE_SLOT_TRADED_COUNT; ++i)
    {
        if (Item* item = myTrade->GetItem(TradeSlots(i)))
        {
            //Can return NULL
            myItems[i] = item;
            myItems[i]->SetInTrade();
        }

        if (Item* item = hisTrade->GetItem(TradeSlots(i)))
        {
            hisItems[i] = item;
            hisItems[i]->SetInTrade();
        }
    }
}

static void clearAcceptTradeMode(TradeData* myTrade, TradeData* hisTrade)
{
    myTrade->SetInAcceptProcess(false);
    hisTrade->SetInAcceptProcess(false);
}

static void clearAcceptTradeMode(Item* *myItems, Item* *hisItems)
{
    // clear 'in-trade' flag
    for (uint8 i = 0; i < TRADE_SLOT_TRADED_COUNT; ++i)
    {
        if (myItems[i])
            myItems[i]->SetInTrade(false);
        if (hisItems[i])
            hisItems[i]->SetInTrade(false);
    }
}

void WorldSession::HandleAcceptTradeOpcode(WorldPacket& /*p_RecvData*/)
{
    TradeData* l_MyTrade = m_Player->m_trade;
    if (!l_MyTrade)
        return;

    Player* l_Trader = l_MyTrade->GetTrader();

    TradeData* l_HisTrade = l_Trader->m_trade;
    if (!l_HisTrade)
        return;

    Item* l_MyItems[TRADE_SLOT_TRADED_COUNT]  = { NULL, NULL, NULL, NULL, NULL, NULL };
    Item* l_HisItems[TRADE_SLOT_TRADED_COUNT] = { NULL, NULL, NULL, NULL, NULL, NULL };
    bool l_MyCanCompleteTrade = true, l_HisCanCompleteTrade = true;

    // Set before checks for properly undo at problems (it already set in to client)
    l_MyTrade->SetAccepted(true);

    // Not accept case incorrect money amount
    if (!m_Player->HasEnoughMoney(l_MyTrade->GetMoney()))
    {
        SendNotification(LANG_NOT_ENOUGH_GOLD);
        l_MyTrade->SetAccepted(false, true);
        return;
    }

    // Not accept case incorrect money amount
    if (!l_Trader->HasEnoughMoney(l_HisTrade->GetMoney()))
    {
        l_Trader->GetSession()->SendNotification(LANG_NOT_ENOUGH_GOLD);
        l_HisTrade->SetAccepted(false, true);
        return;
    }

    // Not accept if some items now can't be trade (cheating)
    for (uint8 l_Iter = 0; l_Iter < TRADE_SLOT_TRADED_COUNT; ++l_Iter)
    {
        if (Item* l_Item = l_MyTrade->GetItem(TradeSlots(l_Iter)))
        {
            if (!l_Item->CanBeTraded(false, true))
            {
                SendTradeStatus(TRADE_STATUS_CANCELLED);
                return;
            }
            if (l_Item->IsBindedNotWith(l_Trader))
            {
                SendTradeStatus(TRADE_STATUS_NOT_ON_TAPLIST);
                SendTradeStatus(TRADE_STATUS_CANCELLED);
                return;
            }
        }

        if (Item* l_Item = l_HisTrade->GetItem(TradeSlots(l_Iter)))
        {
            if (!l_Item->CanBeTraded(false, true))
            {
                SendTradeStatus(TRADE_STATUS_CANCELLED);
                return;
            }
        }
    }

    if (l_HisTrade->IsAccepted())
    {
        setAcceptTradeMode(l_MyTrade, l_HisTrade, l_MyItems, l_HisItems);

        Spell* l_MySpell = NULL;
        SpellCastTargets l_MyTargets;

        Spell* l_HisSpell = NULL;
        SpellCastTargets l_HisTargets;

        // Not accept if spell can't be casted now (cheating)
        if (uint32 l_MySpellID = l_MyTrade->GetSpell())
        {
            SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(l_MySpellID);
            Item* l_CastItem = l_MyTrade->GetSpellCastItem();

            if (!l_SpellInfo || !l_HisTrade->GetItem(TRADE_SLOT_TRADED_COUNT) || (l_MyTrade->HasSpellCastItem() && !l_CastItem))
            {
                clearAcceptTradeMode(l_MyTrade, l_HisTrade);
                clearAcceptTradeMode(l_MyItems, l_HisItems);

                l_MyTrade->SetSpell(0);
                return;
            }

            l_MySpell = new Spell(m_Player, l_SpellInfo, TRIGGERED_FULL_MASK);
            l_MySpell->m_CastItem = l_CastItem;
            l_MyTargets.SetTradeItemTarget(m_Player);
            l_MySpell->m_targets = l_MyTargets;

            SpellCastResult l_Result = l_MySpell->CheckCast(true);
            if (l_Result != SPELL_CAST_OK)
            {
                l_MySpell->SendCastResult(l_Result);

                clearAcceptTradeMode(l_MyTrade, l_HisTrade);
                clearAcceptTradeMode(l_MyItems, l_HisItems);

                delete l_MySpell;
                l_MyTrade->SetSpell(0);
                return;
            }
        }

        // Not accept if spell can't be casted now (cheating)
        if (uint32 l_HisSpellID = l_HisTrade->GetSpell())
        {
            SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(l_HisSpellID);
            Item* l_CastItem = l_HisTrade->GetSpellCastItem();

            if (!l_SpellInfo || !l_MyTrade->GetItem(TRADE_SLOT_TRADED_COUNT) || (l_HisTrade->HasSpellCastItem() && !l_CastItem))
            {
                delete l_MySpell;
                l_HisTrade->SetSpell(0);

                clearAcceptTradeMode(l_MyTrade, l_HisTrade);
                clearAcceptTradeMode(l_MyItems, l_HisItems);
                return;
            }

            l_HisSpell = new Spell(l_Trader, l_SpellInfo, TRIGGERED_FULL_MASK);
            l_HisSpell->m_CastItem = l_CastItem;
            l_HisTargets.SetTradeItemTarget(l_Trader);
            l_HisSpell->m_targets = l_HisTargets;

            SpellCastResult l_Result = l_HisSpell->CheckCast(true);
            if (l_Result != SPELL_CAST_OK)
            {
                l_HisSpell->SendCastResult(l_Result);

                clearAcceptTradeMode(l_MyTrade, l_HisTrade);
                clearAcceptTradeMode(l_MyItems, l_HisItems);

                delete l_MySpell;
                delete l_HisSpell;

                l_HisTrade->SetSpell(0);
                return;
            }
        }

        // Inform partner client
        l_Trader->GetSession()->SendTradeStatus(TRADE_STATUS_ACCEPTED);

        // Test if item will fit in each inventory
        l_HisCanCompleteTrade = (l_Trader->CanStoreItems(l_MyItems, TRADE_SLOT_TRADED_COUNT) == EQUIP_ERR_OK);
        l_MyCanCompleteTrade = (m_Player->CanStoreItems(l_HisItems, TRADE_SLOT_TRADED_COUNT) == EQUIP_ERR_OK);

        clearAcceptTradeMode(l_MyItems, l_HisItems);

        // In case of missing space report error
        if (!l_MyCanCompleteTrade)
        {
            clearAcceptTradeMode(l_MyTrade, l_HisTrade);

            SendNotification(LANG_NOT_FREE_TRADE_SLOTS);
            l_Trader->GetSession()->SendNotification(LANG_NOT_PARTNER_FREE_TRADE_SLOTS);
            l_MyTrade->SetAccepted(false);
            l_HisTrade->SetAccepted(false);
            delete l_MySpell;
            delete l_HisSpell;
            return;
        }
        else if (!l_HisCanCompleteTrade)
        {
            clearAcceptTradeMode(l_MyTrade, l_HisTrade);

            SendNotification(LANG_NOT_PARTNER_FREE_TRADE_SLOTS);
            l_Trader->GetSession()->SendNotification(LANG_NOT_FREE_TRADE_SLOTS);
            l_MyTrade->SetAccepted(false);
            l_HisTrade->SetAccepted(false);
            delete l_MySpell;
            delete l_HisSpell;
            return;
        }

        // Execute trade: 1. remove
        for (uint8 l_Iter = 0; l_Iter < TRADE_SLOT_TRADED_COUNT; ++l_Iter)
        {
            if (l_MyItems[l_Iter])
            {
                l_MyItems[l_Iter]->SetGuidValue(ITEM_FIELD_GIFT_CREATOR, m_Player->GetGUID());
                m_Player->MoveItemFromInventory(l_MyItems[l_Iter]->GetBagSlot(), l_MyItems[l_Iter]->GetSlot(), true);
            }
            if (l_HisItems[l_Iter])
            {
                l_HisItems[l_Iter]->SetGuidValue(ITEM_FIELD_GIFT_CREATOR, l_Trader->GetGUID());
                l_Trader->MoveItemFromInventory(l_HisItems[l_Iter]->GetBagSlot(), l_HisItems[l_Iter]->GetSlot(), true);
            }
        }

        // Execute trade: 2. store
        moveItems(l_MyItems, l_HisItems);

        // Logging money
        if (sWorld->getBoolConfig(CONFIG_GM_LOG_TRADE))
        {
            if (!AccountMgr::IsPlayerAccount(m_Player->GetSession()->GetSecurity()) && l_MyTrade->GetMoney() > 0)
            {
                sLog->outCommand(m_Player->GetSession()->GetAccountId(), "", m_Player->GetGUIDLow(), m_Player->GetName(),
                                l_Trader->GetSession()->GetAccountId(), "", l_Trader->GetGUIDLow(), l_Trader->GetName(),
                                "GM %s (Account: %u) give money (Amount: %lu) to player: %s (Account: %u)",
                                m_Player->GetName(), m_Player->GetSession()->GetAccountId(),
                                l_MyTrade->GetMoney(),
                                l_Trader->GetName(), l_Trader->GetSession()->GetAccountId());
            }
            if (!AccountMgr::IsPlayerAccount(l_Trader->GetSession()->GetSecurity()) && l_HisTrade->GetMoney() > 0)
            {
                sLog->outCommand(l_Trader->GetSession()->GetAccountId(), "", l_Trader->GetGUIDLow(), l_Trader->GetName(),
                                m_Player->GetSession()->GetAccountId(), "", m_Player->GetGUIDLow(), m_Player->GetName(),
                                "GM %s (Account: %u) give money (Amount: %lu) to player: %s (Account: %u)",
                                l_Trader->GetName(), l_Trader->GetSession()->GetAccountId(),
                                l_HisTrade->GetMoney(),
                                m_Player->GetName(), m_Player->GetSession()->GetAccountId());
            }
        }

        // Update money
        m_Player->ModifyMoney(-int64(l_MyTrade->GetMoney()));
        m_Player->ModifyMoney(l_HisTrade->GetMoney());
        l_Trader->ModifyMoney(-int64(l_HisTrade->GetMoney()));
        l_Trader->ModifyMoney(l_MyTrade->GetMoney());

        if (l_MySpell)
            l_MySpell->prepare(&l_MyTargets);

        if (l_HisSpell)
            l_HisSpell->prepare(&l_HisTargets);

        // Cleanup
        clearAcceptTradeMode(l_MyTrade, l_HisTrade);
        delete m_Player->m_trade;
        m_Player->m_trade = NULL;
        delete l_Trader->m_trade;
        l_Trader->m_trade = NULL;

        // Desynchronized with the other saves here (SaveInventoryAndGoldToDB() not have own transaction guards)
        SQLTransaction l_Transaction = CharacterDatabase.BeginTransaction();
        m_Player->SaveInventoryAndGoldToDB(l_Transaction);
        l_Trader->SaveInventoryAndGoldToDB(l_Transaction);
        CharacterDatabase.CommitTransaction(l_Transaction);

        l_Trader->GetSession()->SendTradeStatus(TRADE_STATUS_COMPLETE);
        SendTradeStatus(TRADE_STATUS_COMPLETE);
    }
    else
        l_Trader->GetSession()->SendTradeStatus(TRADE_STATUS_ACCEPTED);
}

void WorldSession::HandleBusyTradeOpcode(WorldPacket& /*p_RecvData*/)
{
    TradeData* l_Trade = m_Player->m_trade;
    if (!l_Trade)
        return;

    SendTradeStatus(TRADE_STATUS_INITIATED);
}

void WorldSession::HandleUnacceptTradeOpcode(WorldPacket& /*p_RecvData*/)
{
    TradeData* l_MyTrade = m_Player->GetTradeData();
    if (!l_MyTrade)
        return;

    l_MyTrade->SetAccepted(false, true);
}

void WorldSession::HandleBeginTradeOpcode(WorldPacket& /*p_RecvData*/)
{
    TradeData* l_Trade = m_Player->m_trade;
    if (!l_Trade)
        return;

    SendTradeStatus(TRADE_STATUS_INITIATED);
}

void WorldSession::SendCancelTrade()
{
    if (PlayerRecentlyLoggedOut() || PlayerLogout())
        return;

    SendTradeStatus(TRADE_STATUS_CANCELLED);
}

void WorldSession::HandleCancelTradeOpcode(WorldPacket& /*p_RecvData*/)
{
    // Sent also after LOGOUT COMPLETE
    if (m_Player)
        m_Player->TradeCancel(true);
}

void WorldSession::HandleInitiateTradeOpcode(WorldPacket& p_RecvData)
{
    uint64 l_Guid = 0;
    p_RecvData.readPackGUID(l_Guid);

    if (GetPlayer()->m_trade)
        return;

    if (!GetPlayer()->isAlive())
    {
        SendTradeStatus(TRADE_STATUS_DEAD);
        return;
    }

    if (GetPlayer()->HasUnitState(UNIT_STATE_STUNNED))
    {
        SendTradeStatus(TRADE_STATUS_STUNNED);
        return;
    }

    if (isLogingOut())
    {
        SendTradeStatus(TRADE_STATUS_LOGGING_OUT);
        return;
    }

    if (GetPlayer()->isInFlight())
    {
        SendTradeStatus(TRADE_STATUS_TOO_FAR_AWAY);
        return;
    }

    if (GetPlayer()->getLevel() < sWorld->getIntConfig(CONFIG_TRADE_LEVEL_REQ))
    {
        SendNotification(GetTrinityString(LANG_TRADE_REQ), sWorld->getIntConfig(CONFIG_TRADE_LEVEL_REQ));
        return;
    }

    Player* l_Player = ObjectAccessor::FindPlayer(l_Guid);
    if (!l_Player)
    {
        SendTradeStatus(TRADE_STATUS_NO_TARGET);
        return;
    }

    if (l_Player == GetPlayer() || l_Player->m_trade)
    {
        // Ignore
        SendTradeStatus(TRADE_STATUS_PLAYER_BUSY);
        return;
    }

    if (!l_Player->isAlive())
    {
        SendTradeStatus(TRADE_STATUS_TARGET_DEAD);
        return;
    }

    if (l_Player->isInFlight())
    {
        SendTradeStatus(TRADE_STATUS_TOO_FAR_AWAY);
        return;
    }

    if (l_Player->HasUnitState(UNIT_STATE_STUNNED))
    {
        SendTradeStatus(TRADE_STATUS_TARGET_STUNNED);
        return;
    }

    if (l_Player->GetSession()->isLogingOut())
    {
        SendTradeStatus(TRADE_STATUS_TARGET_LOGGING_OUT);
        return;
    }

    if (l_Player->GetSocial()->HasIgnore(GetPlayer()->GetGUIDLow()))
    {
        SendTradeStatus(TRADE_STATUS_PLAYER_IGNORED);
        return;
    }

    if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_TRADE) && l_Player->GetTeam() !=m_Player->GetTeam())
    {
        SendTradeStatus(TRADE_STATUS_WRONG_FACTION);
        return;
    }

    if (!l_Player->IsWithinDistInMap(m_Player, 10.0f, false))
    {
        SendTradeStatus(TRADE_STATUS_TOO_FAR_AWAY);
        return;
    }

    if (l_Player->getLevel() < sWorld->getIntConfig(CONFIG_TRADE_LEVEL_REQ))
    {
        SendNotification(GetTrinityString(LANG_TRADE_OTHER_REQ), sWorld->getIntConfig(CONFIG_TRADE_LEVEL_REQ));
        return;
    }

    // OK start trade
    m_Player->m_trade = new TradeData(m_Player, l_Player);
    l_Player->m_trade = new TradeData(l_Player, m_Player);

    // Send to self
    {
        WorldPacket l_Data(SMSG_TRADE_STATUS);
        l_Data.WriteBit(false);
        l_Data.WriteBits(TRADE_STATUS_PROPOSED, 5);
        l_Data.WriteBit(false);
        l_Data.FlushBits();
        l_Data.appendPackGUID(l_Guid);
        l_Data.appendPackGUID(l_Player->GetSession()->GetWoWAccountGUID());
        SendPacket(&l_Data);
    }

    // Send to target
    {
        WorldPacket l_Data(SMSG_TRADE_STATUS);
        l_Data.WriteBit(false);
        l_Data.WriteBits(TRADE_STATUS_PROPOSED, 5);
        l_Data.WriteBit(false);
        l_Data.FlushBits();
        l_Data.appendPackGUID(m_Player->GetGUID());
        l_Data.appendPackGUID(GetWoWAccountGUID());
        l_Player->GetSession()->SendPacket(&l_Data);
    }
}

void WorldSession::HandleSetTradeGoldOpcode(WorldPacket& p_RecvData)
{
    uint64 l_Gold;
    p_RecvData >> l_Gold;

    TradeData* l_MyTrade = m_Player->GetTradeData();
    if (!l_MyTrade)
        return;

    if (!m_Player->HasEnoughMoney(l_Gold))
        l_Gold = m_Player->GetMoney();

    // Gold can be incorrect, but this is checked at trade finished.
    m_Player->IncreaseClientStateIndex();
    l_MyTrade->SetMoney(l_Gold);
}

void WorldSession::HandleSetTradeItemOpcode(WorldPacket& p_RecvData)
{
    // Send update
    uint8 l_TradeSlot;
    uint8 l_SrcBag;
    uint8 l_SrcSlot;

    p_RecvData >> l_TradeSlot;
    p_RecvData >> l_SrcBag;
    p_RecvData >> l_SrcSlot;

    TradeData* l_MyTrade = m_Player->GetTradeData();
    if (!l_MyTrade)
        return;

    // Invalid slot number
    if (l_TradeSlot >= TRADE_SLOT_COUNT)
    {
        SendTradeStatus(TRADE_STATUS_CANCELLED);
        return;
    }

    // Check cheating, can't fail with correct client operations
    Item* l_Item = m_Player->GetItemByPos(l_SrcBag, l_SrcSlot);
    if (!l_Item || (l_TradeSlot != TRADE_SLOT_TRADED_COUNT && !l_Item->CanBeTraded(false, true)))
    {
        SendTradeStatus(TRADE_STATUS_CANCELLED);
        return;
    }

    uint64 l_ItemGuid = l_Item->GetGUID();

    // Prevent place single item into many trade slots using cheating and client bugs
    if (l_MyTrade->HasItem(l_ItemGuid))
    {
        // Cheating attempt
        SendTradeStatus(TRADE_STATUS_CANCELLED);
        return;
    }

    m_Player->IncreaseClientStateIndex();
    l_MyTrade->SetItem(TradeSlots(l_TradeSlot), l_Item);
}

void WorldSession::HandleClearTradeItemOpcode(WorldPacket& p_RecvData)
{
    uint8 l_TradeSlot;
    p_RecvData >> l_TradeSlot;

    TradeData* l_MyTrade = m_Player->m_trade;
    if (!l_MyTrade)
        return;

    // Invalid slot number
    if (l_TradeSlot >= TRADE_SLOT_COUNT)
        return;

    m_Player->IncreaseClientStateIndex();
    l_MyTrade->SetItem(TradeSlots(l_TradeSlot), NULL);
}
