/*
* Copyright (C) 2014-20xx AhsranCore <http://www.ashran.com/>
* Copyright (C) 2012-2013 JadeCore <http://www.pandashan.com/>
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "ScriptPCH.h"
#include "config.h"

class item_strongbox : public ItemScript
{
public:
    item_strongbox() : ItemScript("item_strongbox") { }

    bool OnOpen(Player* p_Player, Item* p_Item)
    {
        int l_ItemChance = 0;
        int l_MinMoney = 0;
        int l_MinHonor = 0;

        ItemTemplate const* l_Proto = p_Item->GetTemplate();
        LootTemplate const* l_LootTemplate = LootTemplates_Item.GetLootFor(l_Proto->ItemId);
        if (!l_LootTemplate)
            return false;
            
        switch (l_Proto->ItemId)
        {
            case 120354: //Gold Strongbox A
            case 111598: //Gold Strongbox H
                    l_ItemChance = 40;
                    l_MinMoney = 25;
                    l_MinHonor = 150;
                break;
            case 120355: //Silver Strongbox A
            case 111599: //Silver strongbox H
                    l_ItemChance = 30;
                    l_MinMoney = 10;
                    l_MinHonor = 100;
                break;
            case 120356: //Bronze Strongbox A
            case 111600: //Bronze Strongbox H
                    l_ItemChance = 25;
                    l_MinMoney = 8;
                    l_MinHonor = 70;
                break;
            case 120353: //Steel Strongbox A
            case 119330: //Steel StrongBox H
                    l_ItemChance = 30;
                    l_MinMoney = 2;
                    l_MinHonor = 30;
                break;
        }

        std::list<ItemTemplate const*> l_LootTable;
        std::vector<uint32> l_Items;
        l_LootTemplate->FillAutoAssignationLoot(l_LootTable);
        uint32 l_SpecID = p_Player->GetLootSpecId() ? p_Player->GetLootSpecId() : p_Player->GetSpecializationId(p_Player->GetActiveSpec());
        
         for (ItemTemplate const* l_Template : l_LootTable)
        {
            for (SpecIndex l_ItemSpecID : l_Template->specs)
            {
                if (l_ItemSpecID == l_SpecID)
                    l_Items.push_back(l_Template->ItemId);
            }
        }
        
        // Remove self first because of invetory space
        p_Player->RemoveItemByDelete(p_Item);
        
        if (l_Items.empty())
            return true;

        std::random_shuffle(l_Items.begin(), l_Items.end());

        if (roll_chance_i(l_ItemChance))
        {
            p_Player->AddItem(l_Items[0], 1);
            p_Player->SendDisplayToast(l_Items[0], 1, DISPLAY_TOAST_METHOD_LOOT, TOAST_TYPE_NEW_ITEM, false, false);
        }
        else
        {
            if (roll_chance_i(60))
            {
                int64 l_GoldAmount = urand(l_MinMoney * GOLD , (l_MinMoney + 10) * GOLD) * sWorld->getRate(RATE_DROP_MONEY);
                p_Player->ModifyMoney(l_GoldAmount);
                p_Player->SendDisplayToast(0, l_GoldAmount, DISPLAY_TOAST_METHOD_CURRENCY_OR_GOLD, TOAST_TYPE_MONEY, false, false);
            }
            else
            {
                int32 l_HonorAmount = urand(l_MinHonor, l_MinHonor + 50) * CURRENCY_PRECISION;
                p_Player->ModifyCurrency(CURRENCY_TYPE_HONOR_POINTS, l_HonorAmount);
                p_Player->SendDisplayToast(CURRENCY_TYPE_HONOR_POINTS, l_HonorAmount  / CURRENCY_PRECISION, DISPLAY_TOAST_METHOD_CURRENCY_OR_GOLD, TOAST_TYPE_NEW_CURRENCY, false, false);
            }
        }
        return true;
    }

};

void AddSC_item_strongboxes()
{
    new item_strongbox();
}