////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "BattlepayMgr.h"

namespace Battlepay
{
    void Manager::LoadFromDatabase()
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Battlepay display info ...");
        LoadDisplayInfos();

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Battlepay products ...");
        LoadProduct();

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Battlepay product groups ...");
        LoadProductGroups();

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Battlepay shop entries ...");
        LoadShopEntires();
    }

    void Manager::LoadDisplayInfos()
    {
        m_DisplayInfos.clear();

        QueryResult l_Result = WorldDatabase.PQuery("SELECT DisplayInfoId, CreatureDisplayInfoID, FileDataID, Name1, Name2, Name3, Flags FROM battlepay_display_info");
        if (!l_Result)
            return;

        do 
        {
            Field* l_Fields = l_Result->Fetch();

            DisplayInfo l_DisplayInfo;
            l_DisplayInfo.CreatureDisplayInfoID = l_Fields[1].GetUInt32();
            l_DisplayInfo.FileDataID            = l_Fields[2].GetInt32();
            l_DisplayInfo.Name1                 = l_Fields[3].GetString();
            l_DisplayInfo.Name2                 = l_Fields[4].GetString();
            l_DisplayInfo.Name3                 = l_Fields[5].GetString();
            l_DisplayInfo.Flags                 = l_Fields[6].GetUInt32();

            m_DisplayInfos.insert(std::make_pair(l_Fields[0].GetUInt32(), l_DisplayInfo));
        } 
        while (l_Result->NextRow());
    }

    void Manager::LoadProductGroups()
    {
        m_ProductGroups.clear();

        QueryResult l_Result = WorldDatabase.PQuery("SELECT GroupID, Name, IconFileDataID, DisplayType, Ordering FROM battlepay_product_group");
        if (!l_Result)
            return;

        do 
        {
            Field* l_Fields = l_Result->Fetch();

            ProductGroup l_ProductGroup;
            l_ProductGroup.GroupID        = l_Fields[0].GetUInt32();
            l_ProductGroup.Name           = l_Fields[1].GetString();
            l_ProductGroup.IconFileDataID = l_Fields[2].GetInt32();
            l_ProductGroup.DisplayType    = l_Fields[3].GetUInt8();
            l_ProductGroup.Ordering       = l_Fields[4].GetInt32();

            m_ProductGroups.push_back(l_ProductGroup);
        } 
        while (l_Result->NextRow());
    }

    void Manager::LoadProduct()
    {
        m_Products.clear();

        QueryResult l_Result = WorldDatabase.PQuery("SELECT ProductID, NormalPriceFixedPoint, CurrentPriceFixedPoint, Type, ChoiceType, Flags, DisplayInfoID FROM battlepay_product");
        if (!l_Result)
            return;

        do
        {
            Field* l_Fields = l_Result->Fetch();

            Product l_Product;
            l_Product.ProductID              = l_Fields[0].GetUInt32();
            l_Product.NormalPriceFixedPoint  = l_Fields[1].GetUInt64();
            l_Product.CurrentPriceFixedPoint = l_Fields[2].GetUInt64();
            l_Product.Type                   = l_Fields[3].GetUInt8();
            l_Product.ChoiceType             = l_Fields[4].GetUInt8();
            l_Product.Flags                  = l_Fields[5].GetUInt32();
            l_Product.DisplayInfoID          = l_Fields[6].GetUInt32();

            m_Products.insert(std::make_pair(l_Product.ProductID, l_Product));
        } 
        while (l_Result->NextRow());

        l_Result = WorldDatabase.PQuery("SELECT ID, ProductID, ItemID, Quantity, DisplayID, HasPet, PetResult FROM battlepay_product_item");
        if (!l_Result)
            return;

        do
        {
            Field* l_Fields = l_Result->Fetch();

            ProductItem l_ProductItem;
            l_ProductItem.ID            = l_Fields[0].GetUInt32();
            l_ProductItem.ItemID        = l_Fields[2].GetUInt32();
            l_ProductItem.Quantity      = l_Fields[3].GetUInt32();
            l_ProductItem.DisplayInfoID = l_Fields[4].GetUInt32();
            l_ProductItem.HasPet        = l_Fields[5].GetBool();
            l_ProductItem.PetResult     = l_Fields[6].GetUInt8();

            uint32 l_ProductID = l_Fields[1].GetUInt32();

            if (m_Products.find(l_ProductID) == m_Products.end())
                continue;

            if (m_DisplayInfos.find(l_ProductItem.DisplayInfoID) == m_DisplayInfos.end())
                continue;

            if (sItemStore.LookupEntry(l_ProductItem.ItemID) == nullptr)
                continue;

            m_Products[l_ProductID].Items.push_back(l_ProductItem);
        } 
        while (l_Result->NextRow());
    }

    void Manager::LoadShopEntires()
    {
        m_ShopEntries.clear();

        QueryResult l_Result = WorldDatabase.PQuery("SELECT EntryID, GroupID, ProductID, Ordering, Flags, BannerType, DisplayInfoID FROM battlepay_shop_entry");
        if (!l_Result)
            return;

        do
        {
            Field* l_Fields = l_Result->Fetch();

            ShopEntry l_ShopEntry;
            l_ShopEntry.EntryID       = l_Fields[0].GetUInt32();
            l_ShopEntry.GroupID       = l_Fields[1].GetUInt32();
            l_ShopEntry.ProductID     = l_Fields[2].GetUInt32();
            l_ShopEntry.Ordering      = l_Fields[3].GetInt32();
            l_ShopEntry.Flags         = l_Fields[4].GetUInt32();
            l_ShopEntry.BannerType    = l_Fields[5].GetUInt8();
            l_ShopEntry.DisplayInfoID = l_Fields[6].GetUInt32();

            m_ShopEntries.push_back(l_ShopEntry);
        } 
        while (l_Result->NextRow());
    }

    ShopCurrency Manager::GetShopCurrency() const
    {
        /// Krw is the battle coins on retail
        /// @TODO: Move that to config files
        return ShopCurrency::Krw;
    }

    bool Manager::IsAvailable() const
    {
        /// @TODO: Move that to config file
        return true;
    }
}