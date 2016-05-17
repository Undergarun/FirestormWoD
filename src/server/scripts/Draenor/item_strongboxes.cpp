////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptPCH.h"

class item_strongbox : public ItemScript
{
    public:
        item_strongbox() : ItemScript("item_strongbox") { }

        bool OnOpen(Player* p_Player, Item* p_Item)
        {
            int l_ItemChance        = 0;
            int l_UncommonChance    = 0;
            int l_RareChance        = 0;
            int l_EpicChance        = 0;
            int l_MinMoney          = 0;
            int l_MinHonor          = 0;

            ItemTemplate const* l_Proto = p_Item->GetTemplate();
            LootTemplate const* l_LootTemplate = LootTemplates_Item.GetLootFor(l_Proto->ItemId);
            if (!l_LootTemplate)
                return false;
            
            switch (l_Proto->ItemId)
            {
                case 120354: ///< Gold Strongbox A
                case 126906: ///< Gold Strongbox A - S2
                case 111598: ///< Gold Strongbox H
                case 126901: ///< Gold Strongbox H - S2
                {
                    l_ItemChance        = 40;
                    l_UncommonChance    = 25;
                    l_RareChance        = 75;
                    l_MinMoney          = 25;
                    l_MinHonor          = 150;
                    break;
                }
                case 120355: ///< Silver Strongbox A
                case 126907: ///< Silver Strongbox A - S2
                case 111599: ///< Silver strongbox H
                case 126902: ///< Silver Strongbox H - S2
                {
                    l_ItemChance        = 30;
                    l_UncommonChance    = 50;
                    l_RareChance        = 50;
                    l_MinMoney          = 10;
                    l_MinHonor          = 100;
                    break;
                }
                case 120356: ///< Bronze Strongbox A
                case 126908: ///< Bronze Strongbox A - S2
                case 111600: ///< Bronze Strongbox H
                case 126903: ///< Bronze Strongbox H - S2
                {
                    l_ItemChance        = 25;
                    l_UncommonChance    = 75;
                    l_RareChance        = 25;
                    l_MinMoney          = 8;
                    l_MinHonor          = 70;
                    break;
                }
                case 120353: ///< Steel Strongbox A
                case 126905: ///< Steel Strongbox A - S2
                case 119330: ///< Steel StrongBox H
                case 126904: ///< Steel Strongbox H - S2
                {
                    l_ItemChance    = 30;
                    l_MinMoney      = 2;
                    l_MinHonor      = 30;
                    break;
                }
                case 118065: ///< Gleaming Ashmaul Strongbox (A)
                case 120151: ///< Gleaming Ashmaul Strongbox (H)
                {
                    l_ItemChance = 100;
                    l_RareChance = 90;
                    l_EpicChance = 10;
                    break;
                }
                case 118093: ///< Dented Ashmaul Strongbox (H) - NYI
                case 118094: ///< Dented Ashmaul Strongbox (A) - NYI
                {
                    l_ItemChance = 100;
                    l_RareChance = 100;
                    break;
                }
                case 126919: ///< Champion's Strongbox (A) - weekly RBG reward
                case 126920: ///< Champion's Strongbox (H) - weekly RBG reward
                {
                    l_ItemChance = 100;
                    l_EpicChance = 100;
                    break;
                }
                default:
                    break;
            }

            std::list<ItemTemplate const*> l_LootTable;
            std::vector<uint32> l_Items;
            l_LootTemplate->FillAutoAssignationLoot(l_LootTable);
            uint32 l_SpecID = p_Player->GetLootSpecId() ? p_Player->GetLootSpecId() : p_Player->GetSpecializationId(p_Player->GetActiveSpec());
        
            for (ItemTemplate const* l_Template : l_LootTable)
            {
                if ((l_Template->AllowableClass && !(l_Template->AllowableClass & p_Player->getClassMask())) ||
                    (l_Template->AllowableRace && !(l_Template->AllowableRace & p_Player->getRaceMask())))
                    continue;

                for (SpecIndex l_ItemSpecID : l_Template->specs[1])
                {
                    if (l_ItemSpecID == l_SpecID)
                        l_Items.push_back(l_Template->ItemId);
                }
            }
        
            /// Remove self first because of inventory space
            p_Player->DestroyItem(p_Item->GetBagSlot(), p_Item->GetSlot(), true);

            if (l_Items.empty())
                return true;

            std::random_shuffle(l_Items.begin(), l_Items.end());

            if (roll_chance_i(l_ItemChance))
            {
                uint32 l_ItemID = l_Items[0];

                /// Default quality will be rare
                ItemQualities l_Quality = ItemQualities::ITEM_QUALITY_RARE;
                if (l_EpicChance && roll_chance_i(l_EpicChance))
                    l_Quality = ItemQualities::ITEM_QUALITY_EPIC;
                else if (l_RareChance && roll_chance_i(l_RareChance))
                    l_Quality = ItemQualities::ITEM_QUALITY_RARE;
                else if (l_UncommonChance && roll_chance_i(l_UncommonChance))
                    l_Quality = ItemQualities::ITEM_QUALITY_UNCOMMON;

                for (uint32 l_ID : l_Items)
                {
                    if (ItemTemplate const* l_Template = sObjectMgr->GetItemTemplate(l_ID))
                    {
                        if (l_Template->Quality == l_Quality)
                        {
                            l_ItemID = l_ID;
                            break;
                        }
                    }
                }

                p_Player->AddItem(l_ItemID, 1);
                p_Player->SendDisplayToast(l_ItemID, 1, DISPLAY_TOAST_METHOD_LOOT, TOAST_TYPE_NEW_ITEM, false, false);
            }
            else
            {
                if (roll_chance_i(60))
                {
                    uint32 l_GoldAmount = uint32(urand(l_MinMoney * GOLD , (l_MinMoney + 10) * GOLD) * sWorld->getRate(RATE_DROP_MONEY));
                    p_Player->ModifyMoney(l_GoldAmount);
                    p_Player->SendDisplayToast(0, l_GoldAmount, DISPLAY_TOAST_METHOD_CURRENCY_OR_GOLD, TOAST_TYPE_MONEY, false, false);
                }
                else
                {
                    int32 l_HonorAmount = urand(l_MinHonor, l_MinHonor + 50) * CURRENCY_PRECISION;
                    p_Player->ModifyCurrency(CURRENCY_TYPE_HONOR_POINTS, l_HonorAmount);
                    p_Player->SendDisplayToast(CURRENCY_TYPE_HONOR_POINTS, l_HonorAmount, DISPLAY_TOAST_METHOD_CURRENCY_OR_GOLD, TOAST_TYPE_NEW_CURRENCY, false, false);
                }
            }

            return true;
        }
};

/// Runic Pouch - 123857
class item_runic_pouch : public ItemScript
{
    public:
        item_runic_pouch() : ItemScript("item_runic_pouch") { }

        bool OnOpen(Player* p_Player, Item* p_Item)
        {
            ItemTemplate const* l_Proto = p_Item->GetTemplate();
            LootTemplate const* l_LootTemplate = LootTemplates_Item.GetLootFor(l_Proto->ItemId);
            if (!l_LootTemplate)
                return false;

            std::list<ItemTemplate const*> l_LootTable;
            std::vector<uint32> l_Items;
            l_LootTemplate->FillAutoAssignationLoot(l_LootTable);
            uint32 l_SpecID = p_Player->GetLootSpecId() ? p_Player->GetLootSpecId() : p_Player->GetSpecializationId(p_Player->GetActiveSpec());

            for (ItemTemplate const* l_Template : l_LootTable)
            {
                if ((l_Template->AllowableClass && !(l_Template->AllowableClass & p_Player->getClassMask())) ||
                    (l_Template->AllowableRace && !(l_Template->AllowableRace & p_Player->getRaceMask())))
                    continue;

                for (SpecIndex l_ItemSpecID : l_Template->specs[1])
                {
                    if (l_ItemSpecID == l_SpecID)
                        l_Items.push_back(l_Template->ItemId);
                }
            }

            if (l_Items.empty())
                return true;

            /// Remove self first because of inventory space
            p_Player->DestroyItem(p_Item->GetBagSlot(), p_Item->GetSlot(), true);

            std::random_shuffle(l_Items.begin(), l_Items.end());

            uint32 l_ItemID = l_Items[0];
            uint32 l_Count  = urand(1, 3);  ///< Found on wowhead: http://www.wowhead.com/item=123857/runic-pouch#contains

            p_Player->AddItem(l_ItemID, l_Count);
            p_Player->SendDisplayToast(l_ItemID, l_Count, DISPLAY_TOAST_METHOD_LOOT, TOAST_TYPE_NEW_ITEM, false, false);

            return true;
        }
};

#ifndef __clang_analyzer__
void AddSC_item_strongboxes()
{
    new item_strongbox();
    new item_runic_pouch();
}
#endif
