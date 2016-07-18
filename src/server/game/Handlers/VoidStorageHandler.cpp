////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "World.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Log.h"
#include "Opcodes.h"
#include "Player.h"

void WorldSession::SendVoidStorageTransferResult(VoidTransferError p_Result)
{
    WorldPacket l_Data(SMSG_VOID_TRANSFER_RESULT, 4);
    l_Data << uint32(p_Result);

    SendPacket(&l_Data);
}

void WorldSession::HandleVoidStorageUnlock(WorldPacket & p_Packet)
{
    uint64 l_NpcGUID = 0;

    p_Packet.readPackGUID(l_NpcGUID);

    Creature* l_Unit = m_Player->GetNPCIfCanInteractWith(l_NpcGUID, UNIT_NPC_FLAG_VAULTKEEPER);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageUnlock - Unit (GUID: %u) not found or player can't interact with it.", GUID_LOPART(l_NpcGUID));
        return;
    }

    if (m_Player->IsVoidStorageUnlocked())
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageUnlock - Player (GUID: %u, name: %s) tried to unlock void storage a 2nd time.", m_Player->GetGUIDLow(), m_Player->GetName());
        return;
    }

    m_Player->ModifyMoney(-int64(VOID_STORAGE_UNLOCK));
    m_Player->UnlockVoidStorage();
}

void WorldSession::HandleVoidStorageQuery(WorldPacket & p_Packet)
{
    uint64 l_NpcGUID;

    p_Packet.readPackGUID(l_NpcGUID);

    Creature* l_Unit = m_Player->GetNPCIfCanInteractWith(l_NpcGUID, UNIT_NPC_FLAG_VAULTKEEPER | UNIT_NPC_FLAG_TRANSMOGRIFIER);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageQuery - Unit (GUID: %u) not found or player can't interact with it.", GUID_LOPART(l_NpcGUID));
        return;
    }

    if (!m_Player->IsVoidStorageUnlocked())
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageQuery - Player (GUID: %u, name: %s) queried void storage without unlocking it.", m_Player->GetGUIDLow(), m_Player->GetName());
        return;
    }

    uint8 l_ItemCount = 0;
    for (uint8 l_I = 0; l_I < VOID_STORAGE_MAX_SLOT; ++l_I)
    {
        if (m_Player->GetVoidStorageItem(l_I))
            ++l_ItemCount;
    }

    WorldPacket l_Data(SMSG_VOID_STORAGE_CONTENTS, 5 * 1024);

    l_Data.WriteBits(l_ItemCount, 8);
    l_Data.FlushBits();

    for (uint8 l_I = 0; l_I < VOID_STORAGE_MAX_SLOT; ++l_I)
    {
        VoidStorageItem* l_Item = m_Player->GetVoidStorageItem(l_I);

        if (!l_Item)
            continue;

        uint64 l_ItemGUID       = MAKE_NEW_GUID(l_Item->ItemId | 0xF0000000, 0, HIGHGUID_ITEM);
        uint64 l_CreatorGUID    = l_Item->CreatorGuid;

        l_Data.appendPackGUID(l_ItemGUID);
        l_Data.appendPackGUID(l_CreatorGUID);

        l_Data << uint32(l_I);

        Item::BuildDynamicItemDatas(l_Data, l_Item->ItemEntry, l_Item->Bonuses);
    }

    SendPacket(&l_Data);
}

void WorldSession::HandleVoidStorageTransfer(WorldPacket & p_Packet)
{
    uint64 l_NpcGUID           = 0;
    uint32 l_DepositCount      = 0;
    uint32 l_WithdrawalCount   = 0;

    p_Packet.readPackGUID(l_NpcGUID);
    p_Packet >> l_DepositCount;
    p_Packet >> l_WithdrawalCount;

    std::vector<uint64> l_Deposits(l_DepositCount);
    std::vector<uint64> l_Withdrawals(l_WithdrawalCount);

    for (uint32 l_I = 0; l_I < l_DepositCount; ++l_I)
        p_Packet.readPackGUID(l_Deposits[l_I]);

    for (uint32 l_I = 0; l_I < l_WithdrawalCount; ++l_I)
        p_Packet.readPackGUID(l_Withdrawals[l_I]);

    if (l_WithdrawalCount > VOID_STORAGE_MAX_WITHDRAW)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageTransfer - Player (GUID: %u, name: %s) wants to withdraw more than 9 items (%u).", m_Player->GetGUIDLow(), m_Player->GetName(), l_WithdrawalCount);
        return;
    }

    if (l_DepositCount > VOID_STORAGE_MAX_WITHDRAW)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageTransfer - Player (GUID: %u, name: %s) wants to deposit more than 9 items (%u).", m_Player->GetGUIDLow(), m_Player->GetName(), l_DepositCount);
        return;
    }

    Creature* l_Unit = m_Player->GetNPCIfCanInteractWith(l_NpcGUID, UNIT_NPC_FLAG_VAULTKEEPER);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageTransfer - Unit (GUID: %u) not found or player can't interact with it.", GUID_LOPART(l_NpcGUID));
        return;
    }

    if (!m_Player->IsVoidStorageUnlocked())
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageTransfer - Player (GUID: %u, name: %s) queried void storage without unlocking it.", m_Player->GetGUIDLow(), m_Player->GetName());
        return;
    }

    if (l_Deposits.size() > m_Player->GetNumOfVoidStorageFreeSlots())
    {
        SendVoidStorageTransferResult(VOID_TRANSFER_ERROR_FULL);
        return;
    }

    uint32 l_FreeBagSlots = 0;
    if (l_Withdrawals.size() != 0)
        l_FreeBagSlots = m_Player->GetBagsFreeSlots();

    if (l_Withdrawals.size() > l_FreeBagSlots)
    {
        SendVoidStorageTransferResult(VOID_TRANSFER_ERROR_INVENTORY_FULL);
        return;
    }

    if (!m_Player->HasEnoughMoney(uint64(l_Deposits.size() * VOID_STORAGE_STORE_ITEM)))
    {
        SendVoidStorageTransferResult(VOID_TRANSFER_ERROR_NOT_ENOUGH_MONEY);
        return;
    }

    std::pair<VoidStorageItem, uint8> l_DepositItemsSecond[VOID_STORAGE_MAX_DEPOSIT];
    uint8 l_DepositCountSecond = 0;

    for (std::vector<uint64>::iterator l_It = l_Deposits.begin(); l_It != l_Deposits.end(); ++l_It)
    {
        Item* l_Item = m_Player->GetItemByGuid(*l_It);

        if (!l_Item)
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageTransfer - Player (GUID: %u, name: %s) wants to deposit an invalid item (item guid: " UI64FMTD ").", m_Player->GetGUIDLow(), m_Player->GetName(), uint64(*l_It));
            continue;
        }

        VoidStorageItem itemVS(sObjectMgr->GenerateVoidStorageItemId(), l_Item->GetEntry(), l_Item->GetGuidValue(ITEM_FIELD_CREATOR), l_Item->GetItemRandomPropertyId(), l_Item->GetItemSuffixFactor(), l_Item->GetAllItemBonuses());

        uint8 l_Slot = m_Player->AddVoidStorageItem(itemVS);

        l_DepositItemsSecond[l_DepositCountSecond++] = std::make_pair(itemVS, l_Slot);

        m_Player->DestroyItem(l_Item->GetBagSlot(), l_Item->GetSlot(), true);
    }

    int64 l_Cost = l_DepositCountSecond * VOID_STORAGE_STORE_ITEM;

    m_Player->ModifyMoney(-l_Cost);

    uint64 l_WithdrawItemsSecond[VOID_STORAGE_MAX_WITHDRAW];
    uint8 l_WithdrawCountSecond = 0;

    for (std::vector<uint64>::iterator l_It = l_Withdrawals.begin(); l_It != l_Withdrawals.end(); ++l_It)
    {
        uint8 l_Slot;
        VoidStorageItem * l_VoidStorageItem = m_Player->GetVoidStorageItem(GUID_LOPART(*l_It) & 0x0FFFFFFF, l_Slot);

        if (!l_VoidStorageItem)
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageTransfer - Player (GUID: %u, name: %s) tried to withdraw an invalid item (id: %U)", m_Player->GetGUIDLow(), m_Player->GetName(), GUID_LOPART(*l_It) & 0x0FFFFFFF);
            continue;
        }

        ItemPosCountVec l_Destination;
        InventoryResult l_EquipMsg = m_Player->CanStoreNewItem(NULL_BAG, NULL_SLOT, l_Destination, l_VoidStorageItem->ItemEntry, 1);

        if (l_EquipMsg != EQUIP_ERR_OK)
        {
            SendVoidStorageTransferResult(VOID_TRANSFER_ERROR_INVENTORY_FULL);
            sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidStorageTransfer - Player (GUID: %u, name: %s) couldn't withdraw item id %u because inventory was full.", m_Player->GetGUIDLow(), m_Player->GetName(), GUID_LOPART(*l_It) & 0x0FFFFFFF);
            return;
        }

        Item * l_Item = m_Player->StoreNewItem(l_Destination, l_VoidStorageItem->ItemEntry, true, l_VoidStorageItem->ItemRandomPropertyId);
        l_Item->SetGuidValue(ITEM_FIELD_CREATOR, MAKE_NEW_GUID(l_VoidStorageItem->CreatorGuid, 0, HIGHGUID_PLAYER));
        l_Item->AddItemBonuses(l_VoidStorageItem->Bonuses);
        l_Item->SetBinding(true);
        m_Player->SendNewItem(l_Item, 1, false, false, false);

        l_WithdrawItemsSecond[l_WithdrawCountSecond++] = *l_It;

        m_Player->DeleteVoidStorageItem(l_Slot);
    }

    WorldPacket l_Data(SMSG_VOID_STORAGE_TRANSFER_CHANGES, 500);

    l_Data.WriteBits(l_DepositCountSecond, 4);
    l_Data.WriteBits(l_WithdrawCountSecond, 4);
    l_Data.FlushBits();

    for (uint8 l_I = 0; l_I < l_DepositCountSecond; ++l_I)
    {
        uint64 l_ItemGUID = MAKE_NEW_GUID(l_DepositItemsSecond[l_I].first.ItemId | 0xF0000000, 0, HIGHGUID_ITEM);
        uint64 l_CreatorGUID = l_DepositItemsSecond[l_I].first.CreatorGuid;

        l_Data.appendPackGUID(l_ItemGUID);
        l_Data.appendPackGUID(l_CreatorGUID);

        l_Data << uint32(l_DepositItemsSecond[l_I].second);                         ///< Slot

        Item::BuildDynamicItemDatas(l_Data, l_DepositItemsSecond[l_I].first);
    }

    for (uint8 l_I = 0; l_I < l_WithdrawCountSecond; ++l_I)
        l_Data.appendPackGUID(l_WithdrawItemsSecond[l_I]);

    SendPacket(&l_Data);

    SendVoidStorageTransferResult(VOID_TRANSFER_ERROR_NO_ERROR);

    SQLTransaction l_Transaction = SessionRealmDatabase.BeginTransaction();
    m_Player->SaveInventoryAndGoldToDB(l_Transaction);
    SessionRealmDatabase.CommitTransaction(l_Transaction);
}

void WorldSession::HandleVoidSwapItem(WorldPacket & p_Packet)
{
    uint64 l_NpcGUID        = 0;
    uint64 l_VoidItemGUID   = 0;
    uint32 l_DstSlot        = 0;

    p_Packet.readPackGUID(l_NpcGUID);
    p_Packet.readPackGUID(l_VoidItemGUID);
    p_Packet >> l_DstSlot;

    Creature* l_Unit = m_Player->GetNPCIfCanInteractWith(l_NpcGUID, UNIT_NPC_FLAG_VAULTKEEPER);
    
    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidSwapItem - Unit (GUID: %u) not found or player can't interact with it.", GUID_LOPART(l_NpcGUID));
        return;
    }

    if (!m_Player->IsVoidStorageUnlocked())
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidSwapItem - Player (GUID: %u, name: %s) queried void storage without unlocking it.", m_Player->GetGUIDLow(), m_Player->GetName());
        return;
    }

    uint8 l_OldSlot;
    if (!m_Player->GetVoidStorageItem(GUID_LOPART(l_VoidItemGUID) & 0x0FFFFFFF, l_OldSlot))
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleVoidSwapItem - Player (GUID: %u, name: %s) requested swapping an invalid item (slot: %u, itemid: %u).", m_Player->GetGUIDLow(), m_Player->GetName(), l_DstSlot, GUID_LOPART(l_VoidItemGUID) & 0x0FFFFFFF);
        return;
    }

    bool l_UsedSrcSlot  = m_Player->GetVoidStorageItem(l_OldSlot) != NULL; // should be always true ///> True or not l_UsedSrcSlot is never read 01/18/16
    bool l_UsedDestSlot = m_Player->GetVoidStorageItem(l_DstSlot) != NULL;

    uint64 l_ItemIdDest = 0;

    if (l_UsedDestSlot)
        l_ItemIdDest = MAKE_NEW_GUID(m_Player->GetVoidStorageItem(l_DstSlot)->ItemId | 0xF0000000, 0, HIGHGUID_ITEM);

    if (!m_Player->SwapVoidStorageItem(l_OldSlot, l_DstSlot))
    {
        SendVoidStorageTransferResult(VOID_TRANSFER_ERROR_INTERNAL_ERROR_1);
        return;
    }

    WorldPacket l_Data(SMSG_VOID_ITEM_SWAP_RESPONSE);

    l_Data.appendPackGUID(l_VoidItemGUID);
    l_Data << uint32(l_DstSlot);
    l_Data.appendPackGUID(l_ItemIdDest);
    l_Data << uint32(l_OldSlot);

    SendPacket(&l_Data);
}
