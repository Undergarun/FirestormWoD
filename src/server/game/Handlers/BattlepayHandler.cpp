////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////// 

#include "ObjectMgr.h"
#include "Opcodes.h"
#include "WorldSession.h"
#include "WorldPacket.h"
#include "BattlepayMgr.h"

void WorldSession::HandleBattlepayGetPurchaseList(WorldPacket& p_RecvData)
{
    WorldPacket l_Data(SMSG_BATTLE_PAY_GET_PURCHASE_LIST_RESPONSE);
    l_Data << uint32(0);    ///< Result
    l_Data << uint32(0);    ///< Purchase count

    SendPacket(&l_Data);
}

void WorldSession::HandleBattlepayGetProductListQuery(WorldPacket& p_RecvData)
{
    if (!sBattlepayMgr->IsAvailable())
        return;

    WorldPacket l_Data(SMSG_BATTLE_PAY_GET_DISTRIBUTION_LIST_RESPONSE);
    l_Data << uint32(0);    ///< Result
    l_Data << uint32(0);    ///< Count
    SendPacket(&l_Data);

    l_Data.Initialize(SMSG_BATTLE_PAY_GET_PRODUCT_LIST_RESPONSE);
    l_Data << uint32(0);                                                    ///< Result
    l_Data << uint32(sBattlepayMgr->GetShopCurrency());                     ///< CurrencyID

    l_Data << uint32(sBattlepayMgr->GetProducts().size());
    l_Data << uint32(sBattlepayMgr->GetProductGroups().size());
    l_Data << uint32(sBattlepayMgr->GetShopEntries().size());

    for (auto& l_Iterator : sBattlepayMgr->GetProducts())
    {
        Battlepay::Product const& l_Product = l_Iterator.second;

        l_Data << uint32(l_Product.ProductID);
        l_Data << uint64(l_Product.NormalPriceFixedPoint);
        l_Data << uint64(l_Product.CurrentPriceFixedPoint);
        l_Data << uint32(l_Product.Items.size());
        l_Data << uint8(l_Product.Type);
        l_Data << uint32(l_Product.Flags);

        for (auto& l_ItemProduct : l_Product.Items)
        {
            l_Data << uint32(l_ItemProduct.ID);
            l_Data << uint32(l_ItemProduct.ItemID);
            l_Data << uint32(l_ItemProduct.Quantity);

            l_Data.FlushBits();

            l_Data.WriteBit(l_ItemProduct.DisplayInfoID != 0);
            l_Data.WriteBit(l_ItemProduct.HasPet);
            l_Data.WriteBit(l_ItemProduct.PetResult != 0);

            if (l_ItemProduct.PetResult != 0)
                l_Data.WriteBits(l_ItemProduct.PetResult, 4);

            if (l_ItemProduct.DisplayInfoID != 0)
                sBattlepayMgr->WriteDisplayInfo(l_ItemProduct.DisplayInfoID, l_Data);
        }

        l_Data.FlushBits();
        l_Data.WriteBits(l_Product.ChoiceType, 2);
        l_Data.WriteBit(l_Product.DisplayInfoID != 0);

        if (l_Product.DisplayInfoID != 0)
            sBattlepayMgr->WriteDisplayInfo(l_Product.DisplayInfoID, l_Data);
    }

    for (auto& l_ProductGroup : sBattlepayMgr->GetProductGroups())
    {
        l_Data << uint32(l_ProductGroup.GroupID);
        l_Data << uint32(l_ProductGroup.IconFileDataID);
        l_Data << uint8(l_ProductGroup.DisplayType);
        l_Data << int32(l_ProductGroup.Ordering);

        l_Data.FlushBits();

        l_Data.WriteBits(l_ProductGroup.Name.size(), 8);
        l_Data.WriteString(l_ProductGroup.Name);
    }

    for (auto& l_ShopEntry : sBattlepayMgr->GetShopEntries())
    {
        l_Data << uint32(l_ShopEntry.EntryID);
        l_Data << uint32(l_ShopEntry.GroupID);
        l_Data << uint32(l_ShopEntry.ProductID);
        l_Data << int32(l_ShopEntry.Ordering);
        l_Data << uint32(l_ShopEntry.Flags);
        l_Data << uint8(l_ShopEntry.BannerType);

        l_Data.FlushBits();
        l_Data.WriteBit(l_ShopEntry.DisplayInfoID != 0);

        if (l_ShopEntry.DisplayInfoID != 0)
            sBattlepayMgr->WriteDisplayInfo(l_ShopEntry.DisplayInfoID, l_Data);
    }

    SendPacket(&l_Data);
}

void WorldSession::HandleBattlePayStartPurchase(WorldPacket& p_RecvData)
{
    /// Atm, we can't buy at char login
    Player* l_Player = GetPlayer();
    if (!l_Player)
        return;

    uint64 l_TargetCharacter;
    uint32 l_ProductID;
    uint32 l_ClientToken;

    p_RecvData >> l_ClientToken;
    p_RecvData >> l_ProductID;
    p_RecvData.readPackGUID(l_TargetCharacter);

    /// Can't happen without client modifications, cheater?
    if (l_Player->GetGUID() != l_TargetCharacter)
        return;

    /// Can't happen without client modifications, cheater?
    if (!sBattlepayMgr->ProductExist(l_ProductID))
        return;

    Battlepay::Product const& l_Product = sBattlepayMgr->GetProduct(l_ProductID);
    /// @TODO: Handle price, already have it ...etc and generate a error code (result) if needed

    /// Create new purchase ID
    uint64 l_PurchaseID = sBattlepayMgr->GenerateNewPurchaseID();

    /// Register purchase ID
    /// @TODO: the purchasse ID must be valid only fews minutes
    sBattlepayMgr->RegisterStartPurchaseID(l_Player->GetGUID(), l_PurchaseID);

    WorldPacket l_Data(SMSG_BATTLE_PAY_START_PURCHASE_RESPONSE);
    l_Data << uint64(l_PurchaseID);     ///< Purchase ID
    l_Data << uint32(0);                ///< Result, need to bruteforce the enum to get all values
    l_Data << uint32(l_ClientToken);    ///< Client Token
    SendPacket(&l_Data);

    /// Client need statut 2 first ...
    l_Data.Initialize(SMSG_BATTLE_PAY_PURCHASE_UPDATE);
    l_Data << uint32(1);               ///< Purchase counter

    /// BattlePayPurchase foreach
    {
        l_Data << uint64(l_PurchaseID);
        l_Data << uint32(2);               ///< Need to reverse status (2 : Search wallets in database, 3 : ready)
        l_Data << uint32(0);               ///< Result code, same as SMSG_BATTLE_PAY_START_PURCHASE_RESPONSE Result value ?
        l_Data << uint32(l_ProductID);     ///< Product ID
        l_Data.WriteBits(0, 8);
    }

    SendPacket(&l_Data);

    std::string l_WalletName = "Ashran Points";

    l_Data.Initialize(SMSG_BATTLE_PAY_PURCHASE_UPDATE);
    l_Data << uint32(1);               ///< Purchase counter

    /// BattlePayPurchase foreach
    {
        l_Data << uint64(l_PurchaseID);
        l_Data << uint32(3);               ///< Need to reverse status (2 : Search wallets in database, 3 : ready)
        l_Data << uint32(0);               ///< Result code, same as SMSG_BATTLE_PAY_START_PURCHASE_RESPONSE Result value ?
        l_Data << uint32(l_ProductID);     ///< Product ID
        l_Data.WriteBits(l_WalletName.size(), 8);
        l_Data.WriteString(l_WalletName);
    }

    SendPacket(&l_Data);
}