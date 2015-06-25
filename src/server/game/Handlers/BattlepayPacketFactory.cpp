////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "BattlepayPacketFactory.h"
#include "Chat.h"
#include <sstream>

namespace Battlepay
{
    namespace PacketFactory
    {
        void SendProductList(WorldSession* p_Session)
        {
            WorldPacket l_Data(SMSG_BATTLE_PAY_GET_PRODUCT_LIST_RESPONSE);
            l_Data << uint32(ProductListResult::Available);                         ///< Result
            l_Data << uint32(sBattlepayMgr->GetShopCurrency());                     ///< CurrencyID

            l_Data << uint32(sBattlepayMgr->GetProducts().size());
            l_Data << uint32(sBattlepayMgr->GetProductGroups().size());
            l_Data << uint32(sBattlepayMgr->GetShopEntries().size());

            for (auto& l_Iterator : sBattlepayMgr->GetProducts())
            {
                Battlepay::Product const& l_Product = l_Iterator.second;

                l_Data << uint32(l_Product.ProductID);
                l_Data << uint64(l_Product.NormalPriceFixedPoint * g_CurrencyPrecision);
                l_Data << uint64(l_Product.CurrentPriceFixedPoint * g_CurrencyPrecision);
                l_Data << uint8(l_Product.Type);
                l_Data << uint32(l_Product.Flags);

                l_Data.FlushBits();

                l_Data.WriteBits(l_Product.Items.size(), 7);
                l_Data.WriteBits(2, 7);                         ///< 6.1.2 unk

                l_Data.WriteBit(l_Product.DisplayInfoID != 0);

                for (auto& l_ItemProduct : l_Product.Items)
                {
                    l_Data << uint32(l_ItemProduct.ID);
                    l_Data << uint32(l_ItemProduct.ItemID);
                    l_Data << uint32(l_ItemProduct.Quantity);

                    l_Data.FlushBits();

                    l_Data.WriteBit(l_ItemProduct.DisplayInfoID != 0);
                    l_Data.WriteBit(false); ///< Already has product item
                    l_Data.WriteBit(l_ItemProduct.PetResult != 0);

                    if (l_ItemProduct.PetResult != 0)
                        l_Data.WriteBits(l_ItemProduct.PetResult, 4);

                    if (l_ItemProduct.DisplayInfoID != 0)
                        WriteDisplayInfo(l_ItemProduct.DisplayInfoID, l_Data);
                }

                l_Data.FlushBits();

                if (l_Product.DisplayInfoID != 0)
                    WriteDisplayInfo(l_Product.DisplayInfoID, l_Data);
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
                    Battlepay::PacketFactory::WriteDisplayInfo(l_ShopEntry.DisplayInfoID, l_Data);
            }

            p_Session->SendPacket(&l_Data);
        }

        void WriteDisplayInfo(uint32 p_DisplayInfoID, WorldPacket& p_Packet)
        {
            DisplayInfo const* l_DisplayInfo = sBattlepayMgr->GetDisplayInfo(p_DisplayInfoID);
            if (l_DisplayInfo == nullptr)
                return;

            p_Packet.FlushBits();
            p_Packet.WriteBit(l_DisplayInfo->CreatureDisplayInfoID != 0);
            p_Packet.WriteBit(l_DisplayInfo->FileDataID != 0);
            p_Packet.WriteBits(l_DisplayInfo->Name1.size(), 10);
            p_Packet.WriteBits(l_DisplayInfo->Name2.size(), 10);
            p_Packet.WriteBits(l_DisplayInfo->Name3.size(), 13);
            p_Packet.WriteBit(l_DisplayInfo->Flags != 0);

            if (l_DisplayInfo->CreatureDisplayInfoID != 0)
                p_Packet << uint32(l_DisplayInfo->CreatureDisplayInfoID);

            if (l_DisplayInfo->FileDataID != 0)
                p_Packet << uint32(l_DisplayInfo->FileDataID);

            p_Packet.WriteString(l_DisplayInfo->Name1);
            p_Packet.WriteString(l_DisplayInfo->Name2);
            p_Packet.WriteString(l_DisplayInfo->Name3);

            if (l_DisplayInfo->Flags != 0)
                p_Packet << uint32(l_DisplayInfo->Flags);
        }

        void SendStartPurchaseResponse(WorldSession* p_Session, Battlepay::Purchase const& p_Purchase, Battlepay::PacketFactory::Error const& p_Error)
        {
            WorldPacket l_Data(SMSG_BATTLE_PAY_START_PURCHASE_RESPONSE);
            l_Data << uint64(p_Purchase.PurchaseID);     ///< Purchase ID
            l_Data << uint32(p_Error);                   ///< Result
            l_Data << uint32(p_Purchase.ClientToken);    ///< Client Token

            p_Session->SendPacket(&l_Data);
        }

        void SendPurchaseUpdate(WorldSession* p_Session, Battlepay::Purchase const& p_Purchase, uint32 p_Result)
        {
            /// At the moment, we only use the default wallet (points)
            std::string l_WalletName = sBattlepayMgr->GetDefaultWalletName();

            WorldPacket l_Data(SMSG_BATTLE_PAY_PURCHASE_UPDATE);
            l_Data << uint32(1);               ///< Purchase counter

            /// BattlePayPurchase foreach
            {
                l_Data << uint64(p_Purchase.PurchaseID);
                l_Data << uint32(p_Purchase.Status);      ///< Need to reverse status (2 : Search wallets in database, 3 : ready)
                l_Data << uint32(p_Result);               ///< Result code (see @Error)
                l_Data << uint32(p_Purchase.ProductID);   ///< Product ID
                l_Data.WriteBits(l_WalletName.size(), 8);
                l_Data.WriteString(l_WalletName);
            }

            p_Session->SendPacket(&l_Data);
        }

        void SendConfirmPurchase(WorldSession* p_Session, Battlepay::Purchase const& p_Purchase)
        {
            WorldPacket l_Data(SMSG_BATTLE_PAY_CONFIRM_PURCHASE);
            l_Data << uint64(p_Purchase.PurchaseID);
            l_Data << uint64(p_Purchase.CurrentPrice);
            l_Data << uint32(p_Purchase.ServerToken);

            p_Session->SendPacket(&l_Data);
        }

        /// @TODO
        void SendPurchaseList(WorldSession* p_Session)
        {
            WorldPacket l_Data(SMSG_BATTLE_PAY_GET_PURCHASE_LIST_RESPONSE);
            l_Data << uint32(0);    ///< Result
            l_Data << uint32(0);    ///< Purchase count

            p_Session->SendPacket(&l_Data);
        }

        /// @TODO
        void SendDistributionList(WorldSession* p_Session)
        {
            WorldPacket l_Data(SMSG_BATTLE_PAY_GET_DISTRIBUTION_LIST_RESPONSE);
            l_Data << uint32(0);    ///< Result
            l_Data << uint32(0);    ///< Count

            p_Session->SendPacket(&l_Data);
        }

        void SendPointsBalance(WorldSession* p_Session)
        {
            PreparedStatement* l_Statement = WebDatabase.GetPreparedStatement(WEB_SEL_ACCOUNT_POINTS);
            l_Statement->setUInt32(0, p_Session->GetAccountId());
            l_Statement->setUInt32(1, p_Session->GetAccountId());

            auto l_FuturResult = WebDatabase.AsyncQuery(l_Statement);

            p_Session->AddPrepareStatementCallback(std::make_pair([p_Session](PreparedQueryResult p_Result) -> void
            {
                uint32 l_Balance = 0;
                if (p_Result)
                {
                    Field* l_Fields = p_Result->Fetch();
                    l_Balance = atoi(l_Fields[0].GetCString());
                }

                Player* l_Player = p_Session->GetPlayer();
                if (l_Player == nullptr)
                    return;

                std::ostringstream l_Data;
                l_Data << l_Balance;
                l_Player->SendCustomMessage(GetCustomMessage(CustomMessage::AshranStoreBalance), l_Data);
            }, l_FuturResult), true);

            if (Player* l_Player = p_Session->GetPlayer())
            {
                std::ostringstream l_Data;
                l_Data << p_Session->GetAccountId();
                l_Player->SendCustomMessage(GetCustomMessage(CustomMessage::AshranAccountId), l_Data);
            }
        }
    }
}