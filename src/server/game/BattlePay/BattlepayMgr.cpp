////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "BattlepayMgr.h"
#include "BattlepayPacketFactory.h"
#include "Chat.h"
#include "ScriptMgr.h"

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

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Battlepay product group locales ...");
        LoadProductGroupLocales();

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Battlepay display info locales ...");
        LoadDisplayInfoLocales();
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

        QueryResult l_Result = WorldDatabase.PQuery("SELECT ProductID, NormalPriceFixedPoint, CurrentPriceFixedPoint, Type, ChoiceType, Flags, DisplayInfoID, ClassMask, ScriptName FROM battlepay_product");
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
            l_Product.ClassMask              = l_Fields[7].GetUInt32();
            l_Product.ScriptName             = l_Fields[8].GetString();

            m_Products.insert(std::make_pair(l_Product.ProductID, l_Product));
        } 
        while (l_Result->NextRow());

        l_Result = WorldDatabase.PQuery("SELECT ID, ProductID, ItemID, Quantity, DisplayID, PetResult FROM battlepay_product_item");
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
            //l_ProductItem.HasPet        = l_Fields[5].GetBool();
            l_ProductItem.PetResult     = l_Fields[5].GetUInt8();

            uint32 l_ProductID = l_Fields[1].GetUInt32();

            if (m_Products.find(l_ProductID) == m_Products.end())
                continue;

            if (l_ProductItem.DisplayInfoID != 0 && m_DisplayInfos.find(l_ProductItem.DisplayInfoID) == m_DisplayInfos.end())
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

    void Manager::LoadProductGroupLocales()
    {
        uint32 l_OldMSTime = getMSTime();

        m_ProductGroupLocales.clear();

        QueryResult l_Result = WorldDatabase.PQuery("SELECT GroupID, Name_loc1, Name_loc2, Name_loc3, Name_loc4, Name_loc5, Name_loc6, Name_loc7, Name_loc8, Name_loc9, Name_loc10 FROM locales_battlepay_product_group");
        if (!l_Result)
            return;

        do
        {
            Field* l_Fields  = l_Result->Fetch();
            uint32 l_GroupID = l_Fields[0].GetUInt32();

            ProductGroupLocale& l_ProductGroupLocale = m_ProductGroupLocales[l_GroupID];

            for (uint8 l_LocaleIdx = 1; l_LocaleIdx < MAX_LOCALES; l_LocaleIdx++)
                sObjectMgr->AddLocaleString(l_Fields[l_LocaleIdx].GetString(), LocaleConstant(l_LocaleIdx), l_ProductGroupLocale.Name);
        }
        while (l_Result->NextRow());

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %lu battlepay_product_group locale strings in %u ms", (unsigned long)m_ProductGroupLocales.size(), GetMSTimeDiffToNow(l_OldMSTime));
    }

    void Manager::LoadDisplayInfoLocales()
    {
        uint32 l_OldMSTime = getMSTime();

        m_DisplayInfoLocales.clear();

        QueryResult l_Result = WorldDatabase.PQuery("SELECT DisplayInfoId, Name_loc1, Name_loc2, Name_loc3, Name_loc4, Name_loc5, Name_loc6, Name_loc7, Name_loc8, Name_loc9, Name_loc10,"
            "Description_loc1, Description_loc2, Description_loc3, Description_loc4, Description_loc5, Description_loc6, Description_loc7, Description_loc8, Description_loc9, Description_loc10 FROM locales_battlepay_display_info");

        if (!l_Result)
            return;

        do
        {
            Field* l_Fields        = l_Result->Fetch();

            uint32 l_FieldIdx      = 0;
            uint32 l_DisplayInfoId = l_Fields[l_FieldIdx++].GetUInt32();

            DisplayInfoLocale& l_DisplayInfoLocale = m_DisplayInfoLocales[l_DisplayInfoId];

            for (uint8 l_LocaleIdx = 1; l_LocaleIdx < MAX_LOCALES; l_LocaleIdx++)
                sObjectMgr->AddLocaleString(l_Fields[l_FieldIdx++].GetString(), LocaleConstant(l_LocaleIdx), l_DisplayInfoLocale.Name);

            for (uint8 l_LocaleIdx = 1; l_LocaleIdx < MAX_LOCALES; l_LocaleIdx++)
                sObjectMgr->AddLocaleString(l_Fields[l_FieldIdx++].GetString(), LocaleConstant(l_LocaleIdx), l_DisplayInfoLocale.Description);
        }
        while (l_Result->NextRow());

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %lu battlepay_product_group locale strings in %u ms", (unsigned long)m_ProductGroupLocales.size(), GetMSTimeDiffToNow(l_OldMSTime));
    }

    /// @TODO
    ShopCurrency Manager::GetShopCurrency() const
    {
        /// Krw is the battle coins on retail
        /// @TODO: Move that to config files
        return ShopCurrency::Krw;
    }

    bool Manager::IsAvailable(WorldSession* p_Session) const
    {
        return sWorld->getBoolConfig(CONFIG_BATTLEPAY_ENABLE) && (int)p_Session->GetSecurity() >= sWorld->getIntConfig(CONFIG_BATTLEPAY_MIN_SECURITY);
    }

    void Manager::SavePurchase(WorldSession* p_Session, Battlepay::Purchase* p_Purchase)
    {
        Battlepay::Product const& l_Product = sBattlepayMgr->GetProduct(p_Purchase->ProductID);
        std::ostringstream l_ItemsText;

        for (Battlepay::ProductItem const& l_ItemProduct : l_Product.Items)
        {
            if (!l_ItemsText.str().empty())
                l_ItemsText << ",";

            l_ItemsText << l_ItemProduct.ItemID;
        }

        Battlepay::DisplayInfo const* l_DisplayInfo = sBattlepayMgr->GetDisplayInfo(l_Product.DisplayInfoID);
        std::string l_Type = "item";

        std::string l_ProductName = "";
        if (l_DisplayInfo != nullptr)
        {
            l_ProductName = l_DisplayInfo->Name1;
            if (Battlepay::DisplayInfoLocale const* l_Locale = sBattlepayMgr->GetDisplayInfoLocale(l_Product.DisplayInfoID))
                ObjectMgr::GetLocaleString(l_Locale->Name, p_Session->GetSessionDbLocaleIndex(), l_ProductName);
        }

        PreparedStatement* l_Statement = WebDatabase.GetPreparedStatement(WEB_INS_PURCHASE);
        l_Statement->setUInt32(0, p_Session->GetAccountId());
        l_Statement->setUInt32(1, sLog->GetRealmID());
        l_Statement->setUInt32(2, p_Session->GetPlayer() ? p_Session->GetPlayer()->GetGUIDLow() : 0);
        l_Statement->setString(3, l_ItemsText.str());
        l_Statement->setString(4, "InGame Shop - " + l_Type + " - " + l_ProductName);
        l_Statement->setUInt32(5, p_Purchase->CurrentPrice);
        l_Statement->setString(6, p_Session->GetRemoteAddress());
        l_Statement->setString(7, l_Type);
        auto l_FuturResult = WebDatabase.AsyncQuery(l_Statement);

        p_Session->AddPrepareStatementCallback(std::make_pair([p_Session](PreparedQueryResult p_Result) -> void
        {
            sBattlepayMgr->OnPrepareStatementCallbackEvent(p_Session, CallbackEvent::SavePurchase);
        }, l_FuturResult), true);
    }

    void Manager::OnPrepareStatementCallbackEvent(WorldSession* p_Session, uint8 p_CallbackEvent)
    {
        switch (p_CallbackEvent)
        {
            case Battlepay::CallbackEvent::SavePurchase:
                Battlepay::PacketFactory::SendPointsBalance(p_Session);
                break;
            default:
                break;
        }
    }

    void Manager::ProcessDelivery(WorldSession* p_Session, Battlepay::Purchase* p_Purchase)
    {
        Battlepay::Product const& l_Product = sBattlepayMgr->GetProduct(p_Purchase->ProductID);

        for (Battlepay::ProductItem const& l_ItemProduct : l_Product.Items)
        {
            if (Player* l_Player = p_Session->GetPlayer())
                l_Player->AddItem(l_ItemProduct.ItemID, l_ItemProduct.Quantity);
        }

        if (!l_Product.ScriptName.empty())
            sScriptMgr->OnBattlePayProductDelivery(p_Session, l_Product);
    }

    void Manager::OnPaymentSucess(uint32 p_AccountId, uint32 p_NewBalance)
    {
        SessionMap const& l_Sessions = sWorld->GetAllSessions();
        auto l_SessionItr = l_Sessions.find(p_AccountId);

        if (l_SessionItr != l_Sessions.end())
        {
            WorldSession* l_Session = l_SessionItr->second;
            Player* l_Player = l_Session->GetPlayer();

            if (l_Player == nullptr)
                return;

            std::ostringstream l_Data;
            l_Data << p_NewBalance;
            l_Player->SendCustomMessage
            (
                PacketFactory::CustomMessage::GetCustomMessage(PacketFactory::CustomMessage::AshranStoreBalance),
                l_Data
            );
        }
    }

    bool Manager::AlreadyOwnProduct(uint32 p_ItemID, Player* p_Player) const
    {
        if (p_Player == nullptr)
            return false;

        ItemTemplate const* l_Item = sObjectMgr->GetItemTemplate(p_ItemID);
        if (l_Item == nullptr)
            return false;

        /// Check mounts - pets
        for (auto l_SpellData : l_Item->Spells)
        {
            if (l_SpellData.SpellTrigger == ITEM_SPELLTRIGGER_LEARN_SPELL_ID && p_Player->HasSpell(l_SpellData.SpellId))
                return true;
        }

        /// Check toys
        if (p_Player->HasToy(p_ItemID))
            return true;

        return false;
    }

    std::string Manager::GetQualityColor(ItemQualities p_Quality) const
    {
        switch (p_Quality)
        {
            case ITEM_QUALITY_POOR:
                return "|cff9d9d9d";
            case ITEM_QUALITY_NORMAL:
                return "|cffffffff";
            default:
            case ITEM_QUALITY_UNCOMMON:
                return "|cff1eff00";
            case ITEM_QUALITY_RARE:
                return "|cff0070dd";
            case ITEM_QUALITY_EPIC:
                return "|cffa335ee";
            case ITEM_QUALITY_LEGENDARY:
                return "|cffff8000";
            case ITEM_QUALITY_ARTIFACT:
                return "|cffe5cc80";
            case ITEM_QUALITY_HEIRLOOM:
                return "|cffe5cc80";
                break;
        }
    }

    std::string Manager::GeneratePackDescription(Battlepay::Product const& p_Product, LocaleConstant p_Locale)
    {
        std::string l_Description;
        for (auto l_Item : p_Product.Items)
        {
            ItemTemplate const* l_ItemTemplate = sObjectMgr->GetItemTemplate(l_Item.ItemID);
            if (l_ItemTemplate == nullptr)
                continue;

            l_Description += GetQualityColor((ItemQualities)l_ItemTemplate->Quality) + l_ItemTemplate->Name1->Get(p_Locale) + "\n";
        }

        return l_Description;
    }

}