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

void WorldSession::HandleBattlepayGetProductListQuery(WorldPacket& p_RecvData)
{
    if (!sBattlepayMgr->IsAvailable())
        return;

    WorldPacket l_Data(SMSG_BATTLE_PAY_GET_PRODUCT_LIST_RESPONSE); 
    l_Data << uint32(0);                                                    ///< Result
    l_Data << uint32(sBattlepayMgr->GetShopCurrency());                     ///< CurrencyID

    l_Data << uint32(sBattlepayMgr->GetProducts().size());
    l_Data << uint32(sBattlepayMgr->GetProductGroups().size());
    l_Data << uint32(sBattlepayMgr->GetShopEntries().size());

    for (auto& l_Iterator : sBattlepayMgr->GetProducts())
    {
        Battlepay::Product const& l_Product = l_Iterator.second;

        l_Data << uint32(l_Product.ProductID);
        l_Data << uint32(l_Product.NormalPriceFixedPoint);
        l_Data << uint32(l_Product.CurrentPriceFixedPoint);
        l_Data << uint32(l_Product.Items.size());
        l_Data << uint8(l_Product.Type);
        l_Data << uint32(l_Product.Flags);

        for (auto& l_ItemProduct : l_Product.Items)
        {
            l_Data << uint32(l_ItemProduct.ID);
            l_Data << uint32(l_ItemProduct.ItemID);
            l_Data << uint32(l_ItemProduct.Quantity);

            l_Data.WriteBit(l_ItemProduct.DisplayInfoID != 0);
            l_Data.WriteBit(l_ItemProduct.HasPet);
            l_Data.WriteBit(l_ItemProduct.PetResult != 0);

            if (l_ItemProduct.PetResult != 0)
                l_Data.WriteBits(l_ItemProduct.PetResult, 4);

            // WriteDisplayInfo
        }
    }
}