////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "Chat.h"
#include "AccountMgr.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "BattlepayMgr.h"
#include "BattlepayPacketFactory.h"

class battlepay_commandscript: public CommandScript
{
    public:
        battlepay_commandscript() : CommandScript("battlepay_commandscript") { }

        ChatCommand* GetCommands() const
        {
            static ChatCommand g_BattlepayCommandTable[] =
            {
                { "dumpwebshopcategory",    SEC_ADMINISTRATOR, false, &HandleDumpWebShopCategory,     "", nullptr },
                { "dumppack",               SEC_ADMINISTRATOR, false, &HandleDumpWebShopPackCategory, "", nullptr },
                { "reload",                 SEC_ADMINISTRATOR, false, &HandleReloadBattlePay,         "", nullptr },
                { "dumpiteminfo",           SEC_ADMINISTRATOR, false, &HandleDumpItemInfo,            "", nullptr },
                { nullptr,                  0,                 false, nullptr,                        "", nullptr }
            };

            static ChatCommand g_CommandTable[] =
            {
                 { "battlepay", SEC_ADMINISTRATOR, true, nullptr, "", g_BattlepayCommandTable },
                 { nullptr, 0, false, nullptr, "", nullptr }
            };

            return g_CommandTable;
        }

        static bool HandleDumpWebShopCategory(ChatHandler* /*p_ChatHandler*/, char const* p_Args)
        {
            uint32 l_Category = atoi(strtok((char*)p_Args, " "));
            if (!l_Category)
                return false;

            uint32 l_Group = atoi(strtok(nullptr, " "));
            if (!l_Group)
                return false;

            uint32 l_FlagsFilter = atoi(strtok(nullptr, " "));

            QueryResult l_Result = WebDatabase.PQuery("SELECT TypeEntryOrData, Price FROM api_shop_entries WHERE Parent = %u ORDER by Price DESC", l_Category);
            if (!l_Result)
                return false;

            FILE* l_Output = fopen("./battlepay_webshop.sql", "w+");
            if (!l_Output)
                return false;

            std::ostringstream l_StrBuilder;

            l_StrBuilder << "SET @PRODUCTID := COALESCE((SELECT MAX(ProductID) FROM battlepay_product), 0) + 1;" << std::endl;
            l_StrBuilder << "SET @ORDER := COALESCE((SELECT MAX(Ordering) FROM battlepay_shop_entry WHERE GroupID = " << l_Group << "), 0) + 1;" << std::endl;
            l_StrBuilder << "SET @DISPLAYINFOID := COALESCE((SELECT MAX(DisplayInfoId) FROM battlepay_display_info), 0) + 1;" << std::endl;

            do
            {
                Field* l_Fields    = l_Result->Fetch();
                uint32 l_ItemID    = l_Fields[0].GetUInt32();
                uint32 l_Price     = l_Fields[1].GetUInt32();
                uint32 l_FakePrice = l_Price;

                if (l_FakePrice == 0)
                    l_FakePrice = l_Price;

                ItemTemplate const* l_Item = sObjectMgr->GetItemTemplate(l_ItemID);
                if (l_Item == nullptr)
                    continue;

                uint32 l_CreatureDisplayInfoID = 0;
                uint32 l_FileDataID = 0;

                std::string l_Description[3];

                /// Mount
                if (l_FlagsFilter & 0x01)
                {
                    for (uint32 l_I = 0; l_I < sMountStore.GetNumRows(); ++l_I)
                    {
                        auto l_MountEntry = sMountStore.LookupEntry(l_I);
                        if (!l_MountEntry || l_MountEntry->SpellID != l_Item->Spells[1].SpellId)
                            continue;

                        l_CreatureDisplayInfoID = l_MountEntry->CreatureDisplayID;
                        /*l_Description[0] = l_MountEntry->Description->Get(LocaleConstant::LOCALE_enUS);
                        l_Description[1] = l_MountEntry->Description->Get(LocaleConstant::LOCALE_frFR);
                        l_Description[2] = l_MountEntry->Description->Get(LocaleConstant::LOCALE_esES);*/
                        break;
                    }
                }

                /// BattlePet
                if (l_FlagsFilter & 0x02)
                {
                    // Species
                    for (size_t l_I = 0; l_I < sBattlePetSpeciesStore.GetNumRows(); ++l_I)
                    {
                        BattlePetSpeciesEntry const* l_BattlePet = sBattlePetSpeciesStore.LookupEntry(l_I);
                        if (!l_BattlePet || l_BattlePet->spellId != l_Item->Spells[1].SpellId)
                            continue;

                        CreatureTemplate const* l_CreatureTemplate = sObjectMgr->GetCreatureTemplate(l_BattlePet->entry);
                        if (l_CreatureTemplate == nullptr)
                            continue;

                        l_CreatureDisplayInfoID = l_CreatureTemplate->Modelid1;
                        break;
                    }
                }

                /// Item icon
                if (l_FlagsFilter & 0x04)
                {
                    if (g_ItemFileDataId.find(l_ItemID) != g_ItemFileDataId.end())
                        l_FileDataID = g_ItemFileDataId[l_ItemID];

                    if (l_FileDataID == 0)
                    {
                        ItemEntry const* l_ItemEntry = sItemStore.LookupEntry(l_ItemID);
                        if (l_ItemEntry != nullptr)
                            l_FileDataID = l_ItemEntry->DisplayId;
                    }

                    l_Description[0] = "|itemid|" + std::to_string(l_ItemID) + "|";
                    l_Description[1] = l_Description[0];
                    l_Description[2] = l_Description[1];
                }

                l_StrBuilder << "INSERT INTO `battlepay_shop_entry` (GroupID, ProductID, Ordering, Flags, BannerType, DisplayInfoID) VALUES (" << l_Group << ",@PRODUCTID, @ORDER, 0, 0, 0);" << std::endl;
                l_StrBuilder << "INSERT INTO `battlepay_product` (ProductID, NormalPriceFixedPoint, CurrentPriceFixedPoint, Type, ChoiceType, Flags, DisplayInfoID) VALUES (" << "@PRODUCTID" << "," << l_Price << "," << l_FakePrice << ",0,2,47," << "@DISPLAYINFOID" << ");" << std::endl;
                l_StrBuilder << "INSERT INTO `battlepay_product_item` (ProductID, ItemID, Quantity, DisplayID, PetResult) VALUES (" << "@PRODUCTID" << "," << l_ItemID << ",1,0,0);" << std::endl;
                l_StrBuilder << "INSERT INTO `battlepay_display_info` (DisplayInfoId, CreatureDisplayInfoID, FileDataID, Name1, Name2, Name3, Flags) VALUES (" << "@DISPLAYINFOID" << "," << l_CreatureDisplayInfoID << "," << l_FileDataID << ",\"" << l_Item->Name1->Get(LocaleConstant::LOCALE_enUS) << "\", '',\"" << l_Description[0] << "\", 0);" << std::endl;
                l_StrBuilder << "INSERT INTO `locales_battlepay_display_info` (DisplayInfoID, Name_loc2, Name_loc6, Name_loc7, Description_loc2, Description_loc6, Description_loc7) VALUES (@DISPLAYINFOID,\"" << l_Item->Name1->Get(LocaleConstant::LOCALE_frFR) << "\",\"" << l_Item->Name1->Get(LocaleConstant::LOCALE_esES) << "\",\"" << l_Item->Name1->Get(LocaleConstant::LOCALE_esES) << "\",\"" << l_Description[1] << "\",\"" << l_Description[2] << "\",\"" << l_Description[2] << "\");" << std::endl;
                l_StrBuilder << "SET @PRODUCTID := @PRODUCTID + 1;" << std::endl;
                l_StrBuilder << "SET @ORDER := @ORDER + 1;" << std::endl;
                l_StrBuilder << "SET @DISPLAYINFOID := @DISPLAYINFOID + 1;" << std::endl;
            }
            while (l_Result->NextRow());

            fwrite(l_StrBuilder.str().c_str(), l_StrBuilder.str().length(), 1, l_Output);
            fflush(l_Output);
            fclose(l_Output);

            return true;
        }

        static bool HandleDumpWebShopPackCategory(ChatHandler* /*p_ChatHandler*/, char const* p_Args)
        {
            uint32 l_Category = atoi(strtok((char*)p_Args, " "));
            if (!l_Category)
                return false;

            uint32 l_Group = atoi(strtok(nullptr, " "));
            if (!l_Group)
                return false;

            uint32 l_FlagsFilter = atoi(strtok(nullptr, " ")); ///< l_FlagsFilter is never read 01/18/16

            QueryResult l_Result = WebDatabase.PQuery("SELECT id, price, fake_price, nameUS, filedataID, class_mask FROM shop_pack WHERE category = %u", l_Category);
            if (!l_Result)
                return false;

            FILE* l_Output = fopen("./battlepay_webshop.sql", "w+");
            if (!l_Output)
                return false;

            std::ostringstream l_StrBuilder;

            l_StrBuilder << "SET @PRODUCTID := COALESCE((SELECT MAX(ProductID) FROM battlepay_product), 0) + 1;" << std::endl;
            l_StrBuilder << "SET @ORDER := COALESCE((SELECT MAX(Ordering) FROM battlepay_shop_entry WHERE GroupID = " << l_Group << "), 0) + 1;" << std::endl;
            l_StrBuilder << "SET @DISPLAYINFOID := COALESCE((SELECT MAX(DisplayInfoId) FROM battlepay_display_info), 0) + 1;" << std::endl;

            do
            {
                Field* l_Fields = l_Result->Fetch();
                uint32 l_ID = l_Fields[0].GetUInt32();
                uint32 l_Price = l_Fields[1].GetUInt32();
                uint32 l_FakePrice = l_Fields[2].GetUInt32();
                std::string l_Name = l_Fields[3].GetString();
                uint32 l_FileDataID = l_Fields[4].GetUInt32();
                uint32 l_ClassMask = l_Fields[5].GetUInt32();


                if (l_FakePrice == 0)
                    l_FakePrice = l_Price;

                uint32 l_CreatureDisplayInfoID = 0;

                l_StrBuilder << "INSERT INTO `battlepay_shop_entry` (GroupID, ProductID, Ordering, Flags, BannerType, DisplayInfoID) VALUES (" << l_Group << ",@PRODUCTID, @ORDER, 0, 0, 0);" << std::endl;
                l_StrBuilder << "INSERT INTO `battlepay_product` (ProductID, NormalPriceFixedPoint, CurrentPriceFixedPoint, Type, ChoiceType, Flags, DisplayInfoID, ClassMask) VALUES (" << "@PRODUCTID" << "," << l_Price << "," << l_FakePrice << ",0,2,47," << "@DISPLAYINFOID," << l_ClassMask <<  ");" << std::endl;
                l_StrBuilder << "INSERT INTO `battlepay_display_info` (DisplayInfoId, CreatureDisplayInfoID, FileDataID, Name1, Name2, Name3, Flags) VALUES (" << "@DISPLAYINFOID" << "," << l_CreatureDisplayInfoID << "," << l_FileDataID << ",\"" << l_Name << "\", '',\"" << "" << "\", 0);" << std::endl;

                QueryResult l_Result2 = WebDatabase.PQuery("SELECT id_item from shop_pack_items where id_pack = %u", l_ID);
                if (l_Result2)
                {
                    do
                    {
                        Field* l_Field = l_Result2->Fetch();
                        uint32 l_ItemShopID = l_Field[0].GetUInt32();
                        QueryResult l_Result3 = WebDatabase.PQuery("SELECT itemID from shop_items WHERE id = %u", l_ItemShopID);

                        if (!l_Result3)
                            continue;

                        uint32 l_ItemID = l_Result3->Fetch()[0].GetUInt32();

                        ItemTemplate const* l_Item = sObjectMgr->GetItemTemplate(l_ItemID);
                        if (l_Item == nullptr)
                            continue;

                        l_StrBuilder << "INSERT INTO `battlepay_product_item` (ProductID, ItemID, Quantity, DisplayID, PetResult) VALUES (" << "@PRODUCTID" << "," << l_ItemID << ",1,0,0);" << std::endl;
                    }
                    while (l_Result2->NextRow());
                }

                l_StrBuilder << "SET @PRODUCTID := @PRODUCTID + 1;" << std::endl;
                l_StrBuilder << "SET @ORDER := @ORDER + 1;" << std::endl;
                l_StrBuilder << "SET @DISPLAYINFOID := @DISPLAYINFOID + 1;" << std::endl;
            }
            while (l_Result->NextRow());

            fwrite(l_StrBuilder.str().c_str(), l_StrBuilder.str().length(), 1, l_Output);
            fflush(l_Output);
            fclose(l_Output);

            return true;
        }

        static bool HandleReloadBattlePay(ChatHandler* /*p_ChatHandler*/, char const* /*p_Args*/)
        {
            sBattlepayMgr->LoadFromDatabase();
            return true;
        }

        static bool HandleDumpItemInfo(ChatHandler* /*p_ChatHandler*/, char const* /*p_Args*/)
        {
            FILE* l_Output = fopen("./iteminfo.sql", "w+");
            if (!l_Output)
                return false;

            std::ostringstream l_StrBuilder;

            for (uint32 l_ItemID = 0; l_ItemID < sItemStore.GetNumRows(); l_ItemID++)
            {
                auto l_Item = sObjectMgr->GetItemTemplate(l_ItemID);
                if (l_Item == nullptr)
                    continue;

                uint32 l_FileDataID = 0;
                if (g_ItemFileDataId.find(l_ItemID) != g_ItemFileDataId.end())
                    l_FileDataID = g_ItemFileDataId[l_ItemID];

                if (l_FileDataID == 0)
                {
                    ItemEntry const* l_ItemEntry = sItemStore.LookupEntry(l_ItemID);
                    if (l_ItemEntry != nullptr)
                        l_FileDataID = l_ItemEntry->DisplayId;
                }

                auto l_FileDataEntry = sFileDataStore.LookupEntry(l_FileDataID);
                if (l_FileDataEntry == nullptr)
                    continue;

                std::string l_IconBuffer = l_FileDataEntry->FileName;
                std::transform(l_IconBuffer.begin(), l_IconBuffer.end(), l_IconBuffer.begin(), ::tolower);

                std::string l_Icon = "";
                for (uint32 l_Index = 0; l_Index < l_IconBuffer.size(); l_Index++)
                {
                    if (l_IconBuffer[l_Index] == '.')
                        break;

                    l_Icon += l_IconBuffer[l_Index];
                }

                l_StrBuilder << "INSERT INTO item_webapi (Entry, Display, Quality, InventoryType, ItemLevel, AllowableClass, AllowableRace, Class, SubClass, Icon) VALUES (";
                l_StrBuilder << l_Item->ItemId << "," << l_Item->DisplayInfoID << "," << l_Item->Quality << "," << l_Item->InventoryType << "," << l_Item->ItemLevel << "," << l_Item->AllowableClass << "," << l_Item->AllowableRace << "," << l_Item->Class << "," << l_Item->SubClass << ",'" << l_Icon << "'";
                l_StrBuilder << ");" << std::endl;
            }

            fwrite(l_StrBuilder.str().c_str(), l_StrBuilder.str().length(), 1, l_Output);
            fflush(l_Output);
            fclose(l_Output);

            return true;
        }
};

#ifndef __clang_analyzer__
void AddSC_battlepay_commandscript()
{
    new battlepay_commandscript();
}
#endif
