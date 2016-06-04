////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include <functional>
#include "ItemEnchantmentMgr.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Common.h"
#include "Util.h"

struct EnchStoreItem
{
    uint32  ench;
    float   chance;

    EnchStoreItem()
        : ench(0), chance(0) {}

    EnchStoreItem(uint32 _ench, float _chance)
        : ench(_ench), chance(_chance) {}
};

typedef std::vector<EnchStoreItem> EnchStoreList;
typedef std::unordered_map<uint32, EnchStoreList> EnchantmentStore;

static EnchantmentStore RandomPropertyItemEnch;
static EnchantmentStore RandomSuffixItemEnch;

void LoadRandomEnchantmentsTable()
{
    uint32 oldMSTime = getMSTime();

    RandomPropertyItemEnch.clear();                                 // for reload case
    RandomSuffixItemEnch.clear();

    //                                                 0      1      2
    QueryResult result = WorldDatabase.Query("SELECT entry, ench, chance, type FROM item_enchantment_template");

    if (result)
    {
        uint32 count = 0;

        do
        {
            Field* fields = result->Fetch();

            uint32 entry = fields[0].GetUInt32();
            uint32 ench = fields[1].GetUInt32();
            float chance = fields[2].GetFloat();
            uint32 type = fields[3].GetUInt32();

            if (chance > 0.000001f && chance <= 100.0f)
            {
                if (type == ENCHANTMENT_RANDOM_SUFFIX)
                    RandomSuffixItemEnch[entry].push_back(EnchStoreItem(ench, chance));
                else if (type == ENCHANTMENT_RANDOM_PROPERTY)
                    RandomPropertyItemEnch[entry].push_back(EnchStoreItem(ench, chance));
            }

            ++count;
        }
        while (result->NextRow());

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Item Enchantment definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    }
    else
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 Item Enchantment definitions. DB table `item_enchantment_template` is empty.");
}

uint32 GetItemEnchantMod(int32 entry, uint32 type)
{
    if (!entry)
        return 0;

    if (entry == -1)
        return 0;

    EnchantmentStore::const_iterator tab = type == ENCHANTMENT_RANDOM_PROPERTY ? RandomPropertyItemEnch.find(entry) : RandomSuffixItemEnch.find(entry) ;
    if (tab == (type == ENCHANTMENT_RANDOM_PROPERTY ? RandomPropertyItemEnch.end() : RandomSuffixItemEnch.end()))
    {
        sLog->outError(LOG_FILTER_SQL, "Item RandomProperty / RandomSuffix id #%u used in `item_template` but it does not have records in `item_enchantment_template` table.", entry);
        return 0;
    }

    double dRoll = rand_chance();
    float fCount = 0;

    for (EnchStoreList::const_iterator ench_iter = tab->second.begin(); ench_iter != tab->second.end(); ++ench_iter)
    {
        fCount += ench_iter->chance;

        if (fCount > dRoll)
            return ench_iter->ench;
    }

    //we could get here only if sum of all enchantment chances is lower than 100%
    dRoll = (irand(0, (int)floor(fCount * 100) + 1)) / 100;
    fCount = 0;

    for (EnchStoreList::const_iterator ench_iter = tab->second.begin(); ench_iter != tab->second.end(); ++ench_iter)
    {
        fCount += ench_iter->chance;

        if (fCount > dRoll)
            return ench_iter->ench;
    }

    return 0;
}

uint32 GenerateEnchSuffixFactor(uint32 item_id)
{
    ItemTemplate const* itemProto = sObjectMgr->GetItemTemplate(item_id);

    if (!itemProto)
        return 0;
    if (!itemProto->RandomSuffix)
        return 0;

    RandomPropertiesPointsEntry const* randomProperty = sRandomPropertiesPointsStore.LookupEntry(itemProto->ItemLevel);
    if (!randomProperty)
        return 0;

    uint32 suffixFactor;
    switch (itemProto->InventoryType)
    {
        // Items of that type don`t have points
        case INVTYPE_NON_EQUIP:
        case INVTYPE_BAG:
        case INVTYPE_TABARD:
        case INVTYPE_AMMO:
        case INVTYPE_QUIVER:
        case INVTYPE_RELIC:
            return 0;
            // Select point coefficient
        case INVTYPE_HEAD:
        case INVTYPE_BODY:
        case INVTYPE_CHEST:
        case INVTYPE_LEGS:
        case INVTYPE_2HWEAPON:
        case INVTYPE_ROBE:
            suffixFactor = 0;
            break;
        case INVTYPE_SHOULDERS:
        case INVTYPE_WAIST:
        case INVTYPE_FEET:
        case INVTYPE_HANDS:
        case INVTYPE_TRINKET:
            suffixFactor = 1;
            break;
        case INVTYPE_NECK:
        case INVTYPE_WRISTS:
        case INVTYPE_FINGER:
        case INVTYPE_SHIELD:
        case INVTYPE_CLOAK:
        case INVTYPE_HOLDABLE:
            suffixFactor = 2;
            break;
        case INVTYPE_WEAPON:
        case INVTYPE_WEAPONMAINHAND:
        case INVTYPE_WEAPONOFFHAND:
            suffixFactor = 3;
            break;
        case INVTYPE_RANGED:
        case INVTYPE_THROWN:
        case INVTYPE_RANGEDRIGHT:
            suffixFactor = 4;
            break;
        default:
            return 0;
    }
    // Select rare/epic modifier
    switch (itemProto->Quality)
    {
        case ITEM_QUALITY_UNCOMMON:
            return randomProperty->UncommonPropertiesPoints[suffixFactor];
        case ITEM_QUALITY_RARE:
            return randomProperty->RarePropertiesPoints[suffixFactor];
        case ITEM_QUALITY_EPIC:
            return randomProperty->EpicPropertiesPoints[suffixFactor];
        case ITEM_QUALITY_LEGENDARY:
        case ITEM_QUALITY_ARTIFACT:
            return 0;                                       // not have random properties
        default:
            break;
    }
    return 0;
}

uint32 CalculateEnchantmentBonus(SpellItemEnchantmentEntry const* p_SpellItemEnchantement, uint32 p_Amount, uint8 p_Index, uint8 p_CharacterLevel)
{
    uint32 l_Amount = p_Amount;

    if (p_SpellItemEnchantement->m_ScalingClass)
    {
        l_Amount = 0;

        int32 l_Unk = (p_SpellItemEnchantement->slot & 0x20) != 0 ? 1 : 60;
        int32 l_ScalingClass = p_SpellItemEnchantement->m_ScalingClass;

        if (p_SpellItemEnchantement->type[p_Index] && p_SpellItemEnchantement->m_ScalingClassRestricted)
            l_ScalingClass = p_SpellItemEnchantement->m_ScalingClassRestricted;

        int32 l_DefaultLevel = p_SpellItemEnchantement->m_MaxLevel;
        if (l_DefaultLevel <= 0)
            l_DefaultLevel = GT_MAX_LEVEL;

        int32 l_Level      = std::max((int)p_CharacterLevel, (int)l_Unk);
        int32 l_FinalLevel = 0;

        if (l_DefaultLevel >= l_Level)
            l_FinalLevel = l_Level;
        else if (p_SpellItemEnchantement->m_MaxLevel <= 0)
            l_FinalLevel = GT_MAX_LEVEL;
        else
            l_FinalLevel = p_SpellItemEnchantement->m_MaxLevel;

        int32 l_ScalingClassIndex = (sChrClassesStore.GetNumRows() - 1) - l_ScalingClass - 1;

        uint32 l_GTSpellScalingRecID = l_ScalingClassIndex * GT_MAX_LEVEL + l_FinalLevel - 1;
        const GtSpellScalingEntry * l_GtScaling = sGtSpellScalingStore.LookupEntry(l_GTSpellScalingRecID);

        if (l_GtScaling)
            l_Amount = l_GtScaling->value * p_SpellItemEnchantement->m_EffectScalingPoints[p_Index];
    }

    return l_Amount;
}


uint32 GetRandomPropertyPoints(uint32 itemLevel, uint32 quality, uint32 inventoryType, uint32 subClass)
{
    uint32 propIndex;

    switch (inventoryType)
    {
        case INVTYPE_HEAD:
        case INVTYPE_BODY:
        case INVTYPE_CHEST:
        case INVTYPE_LEGS:
        case INVTYPE_RANGED:
        case INVTYPE_2HWEAPON:
        case INVTYPE_ROBE:
        case INVTYPE_THROWN:
            propIndex = 0;
            break;
        case INVTYPE_RANGEDRIGHT:
            if (subClass == ITEM_SUBCLASS_WEAPON_WAND)
                propIndex = 3;
            else
                propIndex = 0;
            break;
        case INVTYPE_WEAPON:
        case INVTYPE_WEAPONMAINHAND:
        case INVTYPE_WEAPONOFFHAND:
            propIndex = 3;
            break;
        case INVTYPE_SHOULDERS:
        case INVTYPE_WAIST:
        case INVTYPE_FEET:
        case INVTYPE_HANDS:
        case INVTYPE_TRINKET:
            propIndex = 1;
            break;
        case INVTYPE_NECK:
        case INVTYPE_WRISTS:
        case INVTYPE_FINGER:
        case INVTYPE_SHIELD:
        case INVTYPE_CLOAK:
        case INVTYPE_HOLDABLE:
            propIndex = 2;
            break;
        case INVTYPE_RELIC:
            propIndex = 4;
        default:
            return 0;
    }

    RandomPropertiesPointsEntry const* randPropPointsEntry = sRandomPropertiesPointsStore.LookupEntry(itemLevel);
    if (!randPropPointsEntry)
        return 0;

    switch (quality)
    {
        case ITEM_QUALITY_UNCOMMON:
            return randPropPointsEntry->UncommonPropertiesPoints[propIndex];
        case ITEM_QUALITY_RARE:
        case ITEM_QUALITY_HEIRLOOM:
            return randPropPointsEntry->RarePropertiesPoints[propIndex];
        case ITEM_QUALITY_EPIC:
        case ITEM_QUALITY_LEGENDARY:
            return randPropPointsEntry->EpicPropertiesPoints[propIndex];
    }

    return 0;
}