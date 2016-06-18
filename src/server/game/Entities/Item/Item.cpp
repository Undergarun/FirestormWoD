////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Item.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "DatabaseEnv.h"
#include "ItemEnchantmentMgr.h"
#include "SpellMgr.h"
#include "SpellInfo.h"
#include "ScriptMgr.h"
#include "ConditionMgr.h"
#include "DB2Stores.h"
#ifdef CROSS
#include "InterRealmMgr.h"
#endif /* CROSS */

// @issue : #260
void AddItemsSetItem(Player* player, Item* item)
{
    ItemTemplate const* proto = item->GetTemplate();
    uint32 setid = proto->ItemSet;

    ItemSetEntry const* set = sItemSetStore.LookupEntry(setid);

    if (!set)
    {
        //sLog->outError(LOG_FILTER_SQL, "Item set %u for item (id %u) not found, mods not applied.", setid, proto->ItemId);
        return;
    }

    ItemSetSpellsByItemID::const_iterator iter = sItemSetSpellsByItemIDStore.find(setid);
    if (iter == sItemSetSpellsByItemIDStore.end())
        return;

    std::list<ItemSetSpellEntry const*> const& setSpells = iter->second;

    if (set->required_skill_id && player->GetSkillValue(set->required_skill_id) < set->required_skill_value)
        return;

    ItemSetEffect* eff = NULL;

    for (size_t x = 0; x < player->ItemSetEff.size(); ++x)
    {
        if (player->ItemSetEff[x] && player->ItemSetEff[x]->setid == setid)
        {
            eff = player->ItemSetEff[x];
            break;
        }
    }

    if (!eff)
    {
        eff = new ItemSetEffect();
        eff->setid = setid;

        size_t x = 0;
        for (; x < player->ItemSetEff.size(); ++x)
            if (!player->ItemSetEff[x])
                break;

        if (x < player->ItemSetEff.size())
            player->ItemSetEff[x]=eff;
        else
            player->ItemSetEff.push_back(eff);
    }

    ++eff->item_count;

    int i = 0;
    for (auto setSpell : setSpells)
    {
        if (!setSpell->SpellID)
            continue;

        if (setSpell->SpecializationID && setSpell->SpecializationID != player->GetSpecializationId(player->GetActiveSpec()))
            continue;

        //not enough for  spell
        if (setSpell->PieceRequirement > eff->item_count)

        if (i < MAX_ITEM_SET_SPELLS)
            continue;

          //new spell
        for (uint32 y = 0; y < MAX_ITEM_SET_SPELLS; ++y)
        {
            if (!eff->spells[y])                             // free slot
            {
                SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(setSpell->SpellID);
                if (!spellInfo)
                {
                    sLog->outError(LOG_FILTER_PLAYER_ITEMS, "WORLD: unknown spell id %u in items set %u effects", setSpell->SpellID, setid);
                    break;
                }

                // spell casted only if fit form requirement, in other case will casted at form change
                player->ApplyEquipSpell(spellInfo, NULL, true);
                eff->spells[y] = spellInfo;
                break;
            }
        }
    }
}

void RemoveItemsSetItem(Player*player, ItemTemplate const* proto)
{
    uint32 setid = proto->ItemSet;

    /// If set ID = 0, we don't need to search for ItemSetEntry
    if (!setid)
        return;

    ItemSetEntry const* set = sItemSetStore.LookupEntry(setid);
    if (!set)
    {
        sLog->outError(LOG_FILTER_SQL, "Item set #%u for item #%u not found, mods not removed.", setid, proto->ItemId);
        return;
    }

    ItemSetEffect* eff = NULL;
    size_t setindex = 0;
    for (; setindex < player->ItemSetEff.size(); setindex++)
    {
        if (player->ItemSetEff[setindex] && player->ItemSetEff[setindex]->setid == setid)
        {
            eff = player->ItemSetEff[setindex];
            break;
        }
    }

    // can be in case now enough skill requirement for set appling but set has been appliend when skill requirement not enough
    if (!eff)
        return;

    --eff->item_count;

    ItemSetSpellsByItemID::const_iterator iter = sItemSetSpellsByItemIDStore.find(setid);
    if (iter == sItemSetSpellsByItemIDStore.end())
        return;

    std::list<ItemSetSpellEntry const*> const& setSpells = iter->second;

    for (auto setSpell : setSpells)
    {
        if (!setSpell->SpellID)
            continue;

        if (setSpell->SpecializationID && setSpell->SpecializationID != player->GetSpecializationId(player->GetActiveSpec()))
            continue;

        // enough for spell
        if (setSpell->PieceRequirement <= eff->item_count)
            continue;

        for (uint32 z = 0; z < MAX_ITEM_SET_SPELLS; z++)
        {
            if (eff->spells[z] && eff->spells[z]->Id == setSpell->SpellID)
            {
                // spell can be not active if not fit form requirement
                player->ApplyEquipSpell(eff->spells[z], NULL, false);
                eff->spells[z]=NULL;
                break;
            }
        }
    }

    if (!eff->item_count)                                    //all items of a set were removed
    {
        ASSERT(eff == player->ItemSetEff[setindex]);
        delete eff;
        player->ItemSetEff[setindex] = NULL;
    }
}

bool ItemCanGoIntoBag(ItemTemplate const* pProto, ItemTemplate const* pBagProto)
{
    if (!pProto || !pBagProto)
        return false;

    switch (pBagProto->Class)
    {
        case ITEM_CLASS_CONTAINER:
            switch (pBagProto->SubClass)
            {
                case ITEM_SUBCLASS_CONTAINER:
                    return true;
                case ITEM_SUBCLASS_SOUL_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_SOUL_SHARDS))
                        return false;
                    return true;
                case ITEM_SUBCLASS_HERB_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_HERBS))
                        return false;
                    return true;
                case ITEM_SUBCLASS_ENCHANTING_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_ENCHANTING_SUPPLIES))
                        return false;
                    return true;
                case ITEM_SUBCLASS_MINING_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_MINING_SUPPLIES))
                        return false;
                    return true;
                case ITEM_SUBCLASS_ENGINEERING_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_ENGINEERING_SUPPLIES))
                        return false;
                    return true;
                case ITEM_SUBCLASS_GEM_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_GEMS))
                        return false;
                    return true;
                case ITEM_SUBCLASS_LEATHERWORKING_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_LEATHERWORKING_SUPPLIES))
                        return false;
                    return true;
                case ITEM_SUBCLASS_INSCRIPTION_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_INSCRIPTION_SUPPLIES))
                        return false;
                    return true;
                case ITEM_SUBCLASS_TACKLE_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_FISHING_SUPPLIES))
                        return false;
                    return true;
                case ITEM_SUBCLASS_COOKING_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_COOKING_SUPPLIES))
                        return false;
                    return true;
                default:
                    return false;
            }
        case ITEM_CLASS_QUIVER:
            switch (pBagProto->SubClass)
            {
                case ITEM_SUBCLASS_QUIVER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_ARROWS))
                        return false;
                    return true;
                case ITEM_SUBCLASS_AMMO_POUCH:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_BULLETS))
                        return false;
                    return true;
                default:
                    return false;
            }
    }
    return false;
}

Item::Item()
{
    m_objectType |= TYPEMASK_ITEM;
    m_objectTypeId = TYPEID_ITEM;

    m_updateFlag = 0;

    m_valuesCount = ITEM_END;
    m_slot = 0;
    uState = ITEM_NEW;
    uQueuePos = -1;
    m_container = NULL;
    m_lootGenerated = false;
    mb_in_trade = false;
    m_lastPlayedTimeUpdate = time(NULL);

    m_refundRecipient = 0;
    m_paidMoney = 0;
    m_paidExtendedCost = 0;

    // Fuck default constructor, i don't trust it
    m_text = "";

    m_CustomFlags = 0;

    _dynamicValuesCount = ITEM_DYNAMIC_END;

    memset(m_Modifiers, 0, sizeof (m_Modifiers));
}

bool RemoveItemByDelete(Player* p_Player, Item* p_Item)
{
    for (uint8 i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; ++i)
    {
        if (Item* l_Item = p_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
        {
            if (l_Item == p_Item)
            {
                p_Player->RemoveItem(INVENTORY_SLOT_BAG_0, i, false);
                return true;
            }
        }
    }

    for (int i = BANK_SLOT_ITEM_START; i < BANK_SLOT_BAG_END; ++i)
    {
        if (Item* l_Item = p_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
        {
            if (l_Item == p_Item)
            {
                p_Player->RemoveItem(INVENTORY_SLOT_BAG_0, i, false);
                return true;
            }
        }
    }

    for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
    {
        if (Bag* l_Bag = p_Player->GetBagByPos(i))
        {
            for (uint32 j = 0; j < l_Bag->GetBagSize(); ++j)
            {
                if (Item* pItem = l_Bag->GetItemByPos(j))
                {
                    if (pItem == p_Item)
                    {
                        l_Bag->RemoveItem(j, false);
                        return true;
                    }
                }
            }
        }
    }

    for (uint8 i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; ++i)
    {
        if (Bag* l_Bag = p_Player->GetBagByPos(i))
        {
            for (uint32 j = 0; j < l_Bag->GetBagSize(); ++j)
            {
                if (Item* pItem = l_Bag->GetItemByPos(j))
                {
                    if (pItem == p_Item)
                    {
                        l_Bag->RemoveItem(j, false);
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

Item::~Item()
{
    // WARNING : THAT CHECK MAY CAUSE LAGS !
    if (Player * plr = GetOwner())
    {
        if (RemoveItemByDelete(plr, this))
        {
            ACE_Stack_Trace l_Trace;
            sLog->outAshran("Item %u on player guid %u is in destructor, and pointer is still referenced in player's data ...", GetEntry(), plr->GetGUIDLow());
            sLog->outAshran("Stack Trace : %s", l_Trace.c_str());
        }
    }
}

bool Item::Create(uint32 guidlow, uint32 itemid, Player const* owner)
{
    Object::_Create(guidlow, 0, HIGHGUID_ITEM);

    SetEntry(itemid);
    SetObjectScale(1.0f);

#ifdef CROSS
    if (owner)
    {
        uint32 l_LocalRealmGuid = InterRealmClient::GetIRClient((Player*)owner)->GenerateLocalRealmLowGuid(HIGHGUID_ITEM);
        if (l_LocalRealmGuid == 0)
            ((Player*)owner)->AddItemToGuidSync(GetGUID());
        else
            SetRealGUID(MAKE_NEW_GUID(l_LocalRealmGuid, 0, HIGHGUID_ITEM));
    }

#endif /* CROSS */
    SetGuidValue(ITEM_FIELD_OWNER, owner ? owner->GetGUID() : 0);
    SetGuidValue(ITEM_FIELD_CONTAINED_IN, owner ? owner->GetGUID() : 0);

    ItemTemplate const* itemProto = sObjectMgr->GetItemTemplate(itemid);
    if (!itemProto)
        return false;

    loot.SetSource(GetGUID());

    // For Item Upgrade
    /*if (CanUpgrade())
    {
        if (IsPvPItem())
        {
            if (itemProto->Quality == ITEM_QUALITY_EPIC)
                SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 2, 456);
            else
                SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 2, 453);
        }
        else
        {
            if (itemProto->Quality == ITEM_QUALITY_EPIC)
                SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 2, 445);
            else if (itemProto->Quality == ITEM_QUALITY_LEGENDARY)
                SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 2, 465);
            else
                SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 2, 451);
        }

        SetFlag(ITEM_FIELD_MODIFIERS_MASK, 0x1 | ITEM_TRANSMOGRIFIED | 0x4);
    }*/

    SetUInt32Value(ITEM_FIELD_STACK_COUNT, 1);
    SetUInt32Value(ITEM_FIELD_MAX_DURABILITY, itemProto->MaxDurability);
    SetUInt32Value(ITEM_FIELD_DURABILITY, itemProto->MaxDurability);

    for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
        SetSpellCharges(i, itemProto->Spells[i].SpellCharges);

    SetUInt32Value(ITEM_FIELD_EXPIRATION, itemProto->Duration);
    SetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME, 0);
    return true;
}

// Returns true if Item is a bag AND it is not empty.
// Returns false if Item is not a bag OR it is an empty bag.
bool Item::IsNotEmptyBag() const
{
    if (Bag const* bag = ToBag())
        return !bag->IsEmpty();
    return false;
}

void Item::UpdateDuration(Player* owner, uint32 diff)
{
    if (!GetUInt32Value(ITEM_FIELD_EXPIRATION))
        return;

    sLog->outDebug(LOG_FILTER_PLAYER_ITEMS, "Item::UpdateDuration Item (Entry: %u Duration %u Diff %u)", GetEntry(), GetUInt32Value(ITEM_FIELD_EXPIRATION), diff);

    if (GetUInt32Value(ITEM_FIELD_EXPIRATION) <= diff)
    {
        sScriptMgr->OnItemExpire(owner, GetTemplate());
        owner->DestroyItem(GetBagSlot(), GetSlot(), true);
        return;
    }

    SetUInt32Value(ITEM_FIELD_EXPIRATION, GetUInt32Value(ITEM_FIELD_EXPIRATION) - diff);
    SetState(ITEM_CHANGED, owner);                          // save new time in database
}

void Item::SaveToDB(SQLTransaction& trans)
{
#ifdef CROSS
    auto l_Database = GetOwner() ? GetOwner()->GetRealmDatabase() : nullptr;
    if (l_Database == nullptr)  ///< Happen only in case of guild bank, and guild bank aren't handle cross-side
        return;

#endif /* CROSS */
    bool isInTransaction = trans.get() != nullptr;
    if (!isInTransaction)
#ifndef CROSS
        trans = CharacterDatabase.BeginTransaction();
#else /* CROSS */
        trans = l_Database->BeginTransaction();
#endif /* CROSS */

#ifndef CROSS
    uint32 guid = GetGUIDLow();
#else /* CROSS */
    uint32 guid = GetRealGUIDLow();
#endif /* CROSS */
    switch (uState)
    {
        case ITEM_NEW:
        case ITEM_CHANGED:
        {
            uint8 index = 0;
#ifndef CROSS
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(uState == ITEM_NEW ? CHAR_REP_ITEM_INSTANCE : CHAR_UPD_ITEM_INSTANCE);
#else /* CROSS */
            PreparedStatement* stmt = l_Database->GetPreparedStatement(uState == ITEM_NEW ? CHAR_REP_ITEM_INSTANCE : CHAR_UPD_ITEM_INSTANCE);
#endif /* CROSS */
            stmt->setUInt32(  index, GetEntry());
#ifndef CROSS
            stmt->setUInt32(++index, GUID_LOPART(GetOwnerGUID()));
#else /* CROSS */
            stmt->setUInt32(++index, GetOwner()->GetRealGUIDLow());
#endif /* CROSS */
            stmt->setUInt32(++index, GUID_LOPART(GetGuidValue(ITEM_FIELD_CREATOR)));
            stmt->setUInt32(++index, GUID_LOPART(GetGuidValue(ITEM_FIELD_GIFT_CREATOR)));
            stmt->setUInt32(++index, GetCount());
            stmt->setUInt32(++index, GetUInt32Value(ITEM_FIELD_EXPIRATION));

            std::ostringstream ssSpells;
            for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
                ssSpells << GetSpellCharges(i) << ' ';
            stmt->setString(++index, ssSpells.str());

            stmt->setUInt32(++index, GetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS));

            std::ostringstream ssEnchants;
            for (uint8 i = 0; i < MAX_ENCHANTMENT_SLOT; ++i)
            {
                ssEnchants << GetEnchantmentId(EnchantmentSlot(i)) << ' ';
                ssEnchants << GetEnchantmentDuration(EnchantmentSlot(i)) << ' ';
                ssEnchants << GetEnchantmentCharges(EnchantmentSlot(i)) << ' ';
            }
            stmt->setString(++index, ssEnchants.str());

            stmt->setInt16 (++index, GetItemRandomPropertyId());
            stmt->setUInt32(++index, GetModifier(eItemModifiers::TransmogItemID) | (GetModifier(eItemModifiers::TransmogAppearanceMod) << 24));

            std::ostringstream ssBonuses;
            std::vector<uint32> bonuses = GetAllItemBonuses();
            for (uint8 i = 0; i < bonuses.size(); ++i)
            {
                if (!bonuses[i])
                    continue;

                ssBonuses << bonuses[i] << ' ';
            }

            stmt->setString(++index, ssBonuses.str());
            stmt->setUInt32(++index, GetModifier(eItemModifiers::UpgradeID));
            stmt->setUInt16(++index, GetUInt32Value(ITEM_FIELD_DURABILITY));
            stmt->setUInt32(++index, GetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME));
            stmt->setString(++index, m_text);
            stmt->setUInt32(++index, m_CustomFlags);
            stmt->setUInt32(++index, GetModifier(eItemModifiers::EnchantIllusion));
            stmt->setUInt32(++index, guid);

            trans->Append(stmt);

            if ((uState == ITEM_CHANGED) && HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FIELD_FLAG_WRAPPED))
            {
#ifndef CROSS
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GIFT_OWNER);
                stmt->setUInt32(0, GUID_LOPART(GetOwnerGUID()));
#else /* CROSS */
                stmt = l_Database->GetPreparedStatement(CHAR_UPD_GIFT_OWNER);
                stmt->setUInt32(0, GetOwner()->GetRealGUIDLow());
#endif /* CROSS */
                stmt->setUInt32(1, guid);
                trans->Append(stmt);
            }
            break;
        }
        case ITEM_REMOVED:
        {
#ifndef CROSS
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_INSTANCE);
#else /* CROSS */
            PreparedStatement* stmt = l_Database->GetPreparedStatement(CHAR_DEL_ITEM_INSTANCE);
#endif /* CROSS */
            stmt->setUInt32(0, guid);
            trans->Append(stmt);

            if (HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FIELD_FLAG_WRAPPED))
            {
#ifndef CROSS
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GIFT);
#else /* CROSS */
                stmt = l_Database->GetPreparedStatement(CHAR_DEL_GIFT);
#endif /* CROSS */
                stmt->setUInt32(0, guid);
                trans->Append(stmt);
            }

            if (!isInTransaction)
#ifndef CROSS
                CharacterDatabase.CommitTransaction(trans);
#else /* CROSS */
                l_Database->CommitTransaction(trans);
#endif /* CROSS */

            delete this;
            return;
        }
        case ITEM_UNCHANGED:
            break;
    }

    SetState(ITEM_UNCHANGED);

    if (!isInTransaction)
#ifndef CROSS
        CharacterDatabase.CommitTransaction(trans);
#else /* CROSS */
        l_Database->CommitTransaction(trans);
#endif /* CROSS */
}

bool Item::LoadFromDB(uint32 guid, uint64 owner_guid, Field* fields, uint32 entry)
{
    //                                              0                1          2       3        4        5         6               7              8            9            10          11         12     13     14                15
    //result = CharacterDatabase.PQuery("SELECT creatorGuid, giftCreatorGuid, count, duration, charges, flags, enchantments, randomPropertyId, transmogrifyId, bonuses, upgradeId, durability, playedTime, text, custom_flags, enchantIllusionId FROM item_instance WHERE guid = '%u'", guid);

    // create item before any checks for store correct guid
    // and allow use "FSetState(ITEM_REMOVED); SaveToDB();" for deleting item from DB
#ifndef CROSS
    Object::_Create(guid, 0, HIGHGUID_ITEM);
#else /* CROSS */
    uint32 new_guid = sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM);
    SetRealGUID(MAKE_NEW_GUID(guid, 0, HIGHGUID_ITEM));
    Object::_Create(new_guid, 0, HIGHGUID_ITEM);
#endif /* CROSS */

    // Set entry, MUST be before proto check
    SetEntry(entry);
    SetObjectScale(1.0f);

    ItemTemplate const* proto = GetTemplate();
    if (!proto)
        return false;

    // set owner (not if item is only loaded for gbank/auction/mail
    if (owner_guid != 0)
        SetOwnerGUID(owner_guid);

    bool need_save = false;                                 // need explicit save data at load fixes
    SetGuidValue(ITEM_FIELD_CREATOR, MAKE_NEW_GUID(fields[0].GetUInt32(), 0, HIGHGUID_PLAYER));
    SetGuidValue(ITEM_FIELD_GIFT_CREATOR, MAKE_NEW_GUID(fields[1].GetUInt32(), 0, HIGHGUID_PLAYER));
    SetCount(fields[2].GetUInt32());

    uint32 duration = fields[3].GetUInt32();
    SetUInt32Value(ITEM_FIELD_EXPIRATION, duration);
    // update duration if need, and remove if not need
    if ((proto->Duration == 0) != (duration == 0))
    {
        SetUInt32Value(ITEM_FIELD_EXPIRATION, proto->Duration);
        need_save = true;
    }

    Tokenizer tokens(fields[4].GetString(), ' ', MAX_ITEM_PROTO_SPELLS);
    if (tokens.size() == MAX_ITEM_PROTO_SPELLS)
        for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
            SetSpellCharges(i, atoi(tokens[i]));

    SetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS, fields[5].GetUInt32());
    // Remove bind flag for items vs NO_BIND set
    if (IsSoulBound() && proto->Bonding == NO_BIND)
    {
        ApplyModFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FIELD_FLAG_SOULBOUND, false);
        need_save = true;
    }

    std::string enchants = fields[6].GetString();
    _LoadIntoDataField(enchants.c_str(), ITEM_FIELD_ENCHANTMENT, MAX_ENCHANTMENT_SLOT * MAX_ENCHANTMENT_OFFSET, false);

    if (uint32 l_TransmogID = fields[8].GetInt32())
    {
        if (sObjectMgr->GetItemTemplate(l_TransmogID))
        {
            SetModifier(eItemModifiers::TransmogAppearanceMod, (l_TransmogID >> 24) & 0xFF);
            SetModifier(eItemModifiers::TransmogItemID, l_TransmogID & 0xFFFFFF);
        }
    }

    Tokenizer bonusTokens(fields[9].GetString(), ' ');
        for (uint8 i = 0; i < bonusTokens.size(); ++i)
            AddItemBonus(atoi(bonusTokens[i]));

    SetModifier(eItemModifiers::UpgradeID, fields[10].GetUInt32());

    SetInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID, fields[7].GetInt16());
    // recalculate suffix factor
    if (GetItemRandomPropertyId() < 0)
        UpdateItemSuffixFactor();

    uint32 durability = fields[11].GetUInt16();
    SetUInt32Value(ITEM_FIELD_DURABILITY, durability);
    // update max durability (and durability) if need
    SetUInt32Value(ITEM_FIELD_MAX_DURABILITY, proto->MaxDurability);
    if (durability > proto->MaxDurability)
    {
        SetUInt32Value(ITEM_FIELD_DURABILITY, proto->MaxDurability);
        need_save = true;
    }

    SetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME, fields[12].GetUInt32());
    SetText(fields[13].GetString());
    SetCustomFlags(fields[14].GetUInt32());

    SetModifier(eItemModifiers::EnchantIllusion, fields[15].GetUInt32());

    if (need_save)                                           // normal item changed state set not work at loading
    {
#ifndef CROSS
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ITEM_INSTANCE_ON_LOAD);
        stmt->setUInt32(0, GetUInt32Value(ITEM_FIELD_EXPIRATION));
        stmt->setUInt32(1, GetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS));
        stmt->setUInt32(2, GetUInt32Value(ITEM_FIELD_DURABILITY));
        stmt->setUInt32(3, guid);
        CharacterDatabase.Execute(stmt);
#else /* CROSS */
        Player* l_Owner = GetOwner();
        InterRealmDatabasePool* l_Database = l_Owner ? l_Owner->GetRealmDatabase() : nullptr;

        if (l_Database)
        {
            PreparedStatement* stmt = l_Database->GetPreparedStatement(CHAR_UPD_ITEM_INSTANCE_ON_LOAD);
            stmt->setUInt32(0, GetUInt32Value(ITEM_FIELD_EXPIRATION));
            stmt->setUInt32(1, GetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS));
            stmt->setUInt32(2, GetUInt32Value(ITEM_FIELD_DURABILITY));
            stmt->setUInt32(3, guid);
            l_Database->Execute(stmt);
        }
#endif /* CROSS */
    }

    return true;
}

/*static*/
#ifndef CROSS
void Item::DeleteFromDB(SQLTransaction& trans, uint32 itemGuid)
#else /* CROSS */
void Item::DeleteFromDB(SQLTransaction& trans, uint32 itemGuid, uint32 realmId)
#endif /* CROSS */
{
#ifndef CROSS
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_INSTANCE);
#else /* CROSS */
    InterRealmClient* l_IRClient = sInterRealmMgr->GetClientByRealmNumber(realmId);
    if (!l_IRClient)
        return;

    InterRealmDatabasePool* l_Database = l_IRClient->GetDatabase();
    PreparedStatement* stmt = l_Database->GetPreparedStatement(CHAR_DEL_ITEM_INSTANCE);
#endif /* CROSS */
    stmt->setUInt32(0, itemGuid);
    trans->Append(stmt);
}

void Item::DeleteFromDB(SQLTransaction& trans)
{
#ifndef CROSS
    DeleteFromDB(trans, GetGUIDLow());
#else /* CROSS */
    Player* l_Owner = GetOwner();
    if (!l_Owner)
        return; ///< GuildBank only

    DeleteFromDB(trans, GetRealGUIDLow(), l_Owner->GetSession()->GetInterRealmNumber());
#endif /* CROSS */
}

/*static*/
#ifndef CROSS
void Item::DeleteFromInventoryDB(SQLTransaction& trans, uint32 itemGuid)
#else /* CROSS */
void Item::DeleteFromInventoryDB(SQLTransaction& trans, uint32 itemGuid, uint32 realmId)
#endif /* CROSS */
{
#ifndef CROSS
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_INVENTORY_BY_ITEM);
#else /* CROSS */
    InterRealmClient* l_IRClient = sInterRealmMgr->GetClientByRealmNumber(realmId);
    if (!l_IRClient)
        return;

    InterRealmDatabasePool* l_Database = l_IRClient->GetDatabase();
    PreparedStatement* stmt = l_Database->GetPreparedStatement(CHAR_DEL_CHAR_INVENTORY_BY_ITEM);
#endif /* CROSS */
    stmt->setUInt32(0, itemGuid);
    trans->Append(stmt);
}

void Item::DeleteFromInventoryDB(SQLTransaction& trans)
{
#ifndef CROSS
    DeleteFromInventoryDB(trans, GetGUIDLow());
#else /* CROSS */
    Player* l_Owner = GetOwner();
    if (!l_Owner)
        return; ///< GuildBank only

    DeleteFromInventoryDB(trans, GetRealGUIDLow(), l_Owner->GetSession()->GetInterRealmNumber());
#endif /* CROSS */
}

ItemTemplate const* Item::GetTemplate() const
{
    return sObjectMgr->GetItemTemplate(GetEntry());
}

Player* Item::GetOwner()const
{
    return ObjectAccessor::FindPlayerInOrOutOfWorld(GetOwnerGUID());
}

uint32 Item::GetSkill() const
{
    return GetTemplate()->GetSkill();
}

void Item::GenerateItemBonus(uint32 p_ItemId, ItemContext p_Context, std::vector<uint32>& p_ItemBonus, bool p_OnlyDifficulty /*= false*/)
{
    auto l_ItemTemplate = sObjectMgr->GetItemTemplate(p_ItemId);
    if (l_ItemTemplate == nullptr)
        return;

    /// Process item bonus group
    for (auto l_ItemBonusGroupID : l_ItemTemplate->m_ItemBonusGroups)
    {
        auto l_ItemBonusGroup = sObjectMgr->GetItemBonusGroup(l_ItemBonusGroupID);
        if (l_ItemBonusGroup == nullptr)
            continue;

        p_ItemBonus.push_back(l_ItemBonusGroup->at(urand(0, l_ItemBonusGroup->size() - 1)));
    }

    /// Item bonus per item are store in ItemXBonusTree.db2
    if (sItemBonusTreeByID.find(p_ItemId) == sItemBonusTreeByID.end())
        return;

    /// Step one : search for generic bonus we can find in DB2 (90, 92, 94, 95, 97, heroic)
    auto& l_ItemBonusTree = sItemBonusTreeByID[p_ItemId];
    std::for_each(l_ItemBonusTree.begin(), l_ItemBonusTree.end(), [&p_Context, &p_ItemBonus](ItemXBonusTreeEntry const* p_ItemXBonusTree) -> void
    {
        /// Lookup for the right bonus
        for (uint32 l_Index = 0; l_Index < sItemBonusTreeNodeStore.GetNumRows(); l_Index++)
        {
            auto l_ItemBonusTreeNode = sItemBonusTreeNodeStore.LookupEntry(l_Index);
            if (l_ItemBonusTreeNode == nullptr)
                continue;

            if (l_ItemBonusTreeNode->Category != p_ItemXBonusTree->ItemBonusTreeCategory)
                continue;

            if (l_ItemBonusTreeNode->Context != uint32(p_Context))
                continue;

            auto l_BonusId = l_ItemBonusTreeNode->ItemBonusEntry;

            /// If no bonusId, we must use LinkedCategory to try to find it ...
            /// ItemBonusTreeNode.db2 havn't full data (6.0.3 19116), in somes case we can't find the item bonus
            /// Maybe we can have full data by bruteforcing on retail ?
            if (l_BonusId == 0)
            {
                for (uint32 l_LinkedIndex = 0; l_LinkedIndex < sItemBonusTreeNodeStore.GetNumRows(); l_LinkedIndex++)
                {
                    auto l_LinkedItemBonusTreeNode = sItemBonusTreeNodeStore.LookupEntry(l_LinkedIndex);
                    if (l_LinkedItemBonusTreeNode == nullptr)
                        continue;

                    if (l_LinkedItemBonusTreeNode->Category != l_ItemBonusTreeNode->LinkedCategory)
                        continue;

                    l_BonusId = l_LinkedItemBonusTreeNode->ItemBonusEntry;
                    break;
                }
            }

            if (l_BonusId != 0)
                p_ItemBonus.push_back(l_BonusId);
        }
    });

    /// Step two : Roll for stats bonus (Avoidance, Leech & Speed)
    /// Atm, i can't find percentage chance to have stats but it's same pretty low (~ 10%)
    /// Item can have only on stat bonus, and it's only in dungeon/raid
    if (p_Context == ItemContext::RaidNormal
        || p_Context == ItemContext::RaidHeroic
        || p_Context == ItemContext::RaidLfr
        || p_Context == ItemContext::RaidMythic
        || p_Context == ItemContext::DungeonLevelUp1
        || p_Context == ItemContext::DungeonLevelUp2
        || p_Context == ItemContext::DungeonLevelUp3
        || p_Context == ItemContext::DungeonLevelUp4
        || p_Context == ItemContext::DungeonNormal
        || p_Context == ItemContext::DungeonHeroic)             ///< Only in dungeon & raid
    {
        std::vector<uint32> l_StatsBonus =
        {
            ItemBonus::Stats::Avoidance,
            ItemBonus::Stats::Speed,
            ItemBonus::Stats::Leech
        };

        if (p_Context == ItemContext::RaidNormal ||
            p_Context == ItemContext::RaidMythic ||
            p_Context == ItemContext::RaidHeroic ||
            p_Context == ItemContext::RaidLfr)
            l_StatsBonus.push_back(ItemBonus::Stats::Indestructible);

        if (roll_chance_f(ItemBonus::Chances::Stats) && !p_OnlyDifficulty)
        {
            /// Could be a good thing to improve performance to declare one random generator somewhere and always use the same instead of declare it new one for each std::shuffle call
            /// Note for developers : std::random_shuffle is c based and will be removed soon (c++x14), so it's a good tips to always use std::shuffle instead
            std::random_device l_RandomDevice;
            std::mt19937 l_RandomGenerator(l_RandomDevice());
            std::shuffle(l_StatsBonus.begin(), l_StatsBonus.end(), l_RandomGenerator);

            p_ItemBonus.push_back(*l_StatsBonus.begin());
        }
    }

    /// Step tree : Roll for Warforged & Prismatic Socket
    /// That roll happen only in heroic dungeons & raid
    /// Exaclty like stats, we don't know the chance to have that kind of bonus ...
    if ((p_Context == ItemContext::DungeonHeroic ||
        p_Context == ItemContext::RaidNormal ||
        p_Context == ItemContext::RaidHeroic ||
        p_Context == ItemContext::RaidMythic ||
        p_Context == ItemContext::RaidLfr) && !p_OnlyDifficulty)
    {
        if (roll_chance_f(ItemBonus::Chances::Warforged))
            p_ItemBonus.push_back(ItemBonus::HeroicOrRaid::Warforged);

        if (roll_chance_f(ItemBonus::Chances::PrismaticSocket))
            p_ItemBonus.push_back(ItemBonus::HeroicOrRaid::PrismaticSocket);
    }
}

void Item::BuildDynamicItemDatas(WorldPacket& p_Datas, Item const* p_Item, ItemContext p_Context /*= ItemContext::None*/)
{
    if (p_Item == nullptr)
    {
        p_Datas << uint32(0);                       ///< Item ID
        p_Datas << uint32(0);                       ///< Random Properties Seed
        p_Datas << uint32(0);                       ///< Random Properties ID
        p_Datas.WriteBit(false);                    ///< Has Item Bonuses
        p_Datas.WriteBit(false);                    ///< Has Modifications
        p_Datas.FlushBits();
        return;
    }

    std::vector<uint32> l_Bonuses = p_Item->GetAllItemBonuses();
    std::vector<uint32> l_Modifications = p_Item->GetDynamicValues(EItemDynamicFields::ITEM_DYNAMIC_FIELD_MODIFIERS);

    p_Datas << uint32(p_Item->GetEntry());                  ///< Item ID
    p_Datas << uint32(p_Item->GetItemSuffixFactor());       ///< Random Properties Seed
    p_Datas << uint32(p_Item->GetItemRandomPropertyId());   ///< Random Properties ID
    p_Datas.WriteBit(l_Bonuses.size() != 0);                ///< Has Item Bonuses
    p_Datas.WriteBit(l_Modifications.size() != 0);          ///< Has Modifications
    p_Datas.FlushBits();

    /// Item bonuses
    if (l_Bonuses.size() != 0)
    {
        p_Datas << uint8(p_Context);                        ///< Context
        p_Datas << uint32(l_Bonuses.size());
        for (auto& l_BonusId : l_Bonuses)
            p_Datas << uint32(l_BonusId);
    }

    /// Item modifications
    if (l_Modifications.size() != 0)
    {
        p_Datas << uint32(p_Item->GetUInt32Value(ITEM_FIELD_MODIFIERS_MASK));
        for (auto l_Modifier : l_Modifications)
            p_Datas << uint32(l_Modifier);
    }
}

void Item::BuildDynamicItemDatas(ByteBuffer& p_Datas, Item const* p_Item)
{
    if (p_Item == nullptr)
    {
        p_Datas << uint32(0);                       ///< Item ID
        p_Datas << uint32(0);                       ///< Random Properties Seed
        p_Datas << uint32(0);                       ///< Random Properties ID
        p_Datas.WriteBit(false);                    ///< Has Item Bonuses
        p_Datas.WriteBit(false);                    ///< Has Modifications
        p_Datas.FlushBits();
        return;
    }

    std::vector<uint32> l_Bonuses = p_Item->GetAllItemBonuses();
    std::vector<uint32> l_Modifications = p_Item->GetDynamicValues(EItemDynamicFields::ITEM_DYNAMIC_FIELD_MODIFIERS);

    p_Datas << uint32(p_Item->GetEntry());                  ///< Item ID
    p_Datas << uint32(p_Item->GetItemSuffixFactor());       ///< Random Properties Seed
    p_Datas << uint32(p_Item->GetItemRandomPropertyId());   ///< Random Properties ID
    p_Datas.WriteBit(l_Bonuses.size() != 0);                ///< Has Item Bonuses
    p_Datas.WriteBit(l_Modifications.size() != 0);          ///< Has Modifications
    p_Datas.FlushBits();

    /// Item bonuses
    if (l_Bonuses.size() != 0)
    {
        p_Datas << uint8(0);                                ///< Context
        p_Datas << uint32(l_Bonuses.size());
        for (auto& l_BonusId : l_Bonuses)
            p_Datas << uint32(l_BonusId);
    }

    /// Item modifications
    if (l_Modifications.size() != 0)
    {
        p_Datas << uint32(p_Item->GetUInt32Value(ITEM_FIELD_MODIFIERS_MASK));
        for (auto l_Modifier : l_Modifications)
            p_Datas << uint32(l_Modifier);
    }
}

void Item::BuildDynamicItemDatas(WorldPacket& p_Datas, VoidStorageItem const p_Item)
{
    p_Datas << uint32(p_Item.ItemEntry);            ///< Item ID
    p_Datas << uint32(p_Item.ItemSuffixFactor);     ///< Random Properties Seed
    p_Datas << uint32(p_Item.ItemRandomPropertyId); ///< Random Properties ID
    p_Datas.WriteBit(p_Item.Bonuses.size() != 0);   ///< Has Item Bonuses
    p_Datas.WriteBit(false);                        ///< Has Modifications
    p_Datas.FlushBits();

    /// Item bonuses
    if (p_Item.Bonuses.size() != 0)
    {
        p_Datas << uint8(0);                        ///< Context
        p_Datas << uint32(p_Item.Bonuses.size());

        for (auto& l_BonusId : p_Item.Bonuses)
            p_Datas << uint32(l_BonusId);
    }
}

void Item::BuildDynamicItemDatas(ByteBuffer& p_Datas, LootItem const p_Item)
{
    std::vector<uint32> l_Bonuses = p_Item.itemBonuses;

    p_Datas << uint32(p_Item.itemid);               ///< Item ID
    p_Datas << uint32(p_Item.randomSuffix);         ///< Random Properties Seed
    p_Datas << uint32(p_Item.randomPropertyId);     ///< Random Properties ID
    p_Datas.WriteBit(l_Bonuses.size() != 0);        ///< Has Item Bonuses
    p_Datas.WriteBit(false);                        ///< Has Modifications
    p_Datas.FlushBits();

    /// Item bonuses
    if (l_Bonuses.size() != 0)
    {
        p_Datas << uint8(0);                        ///< Context
        p_Datas << uint32(l_Bonuses.size());
        for (auto& l_BonusId : l_Bonuses)
            p_Datas << uint32(l_BonusId);
    }
}

void Item::BuildDynamicItemDatas(ByteBuffer& p_Datas, uint32 p_Entry, std::vector<uint32> p_ItemBonuses)
{
    ItemTemplate const* l_Template = sObjectMgr->GetItemTemplate(p_Entry);
    if (l_Template == nullptr)
    {
        p_Datas << uint32(p_Entry);                 ///< Item ID
        p_Datas << uint32(0);                       ///< Random Properties Seed
        p_Datas << uint32(0);                       ///< Random Properties ID
        p_Datas.WriteBit(false);                    ///< Has Item Bonuses
        p_Datas.WriteBit(false);                    ///< Has Modifications
        p_Datas.FlushBits();
        return;
    }

    p_Datas << uint32(p_Entry);                     ///< Item ID
    p_Datas << uint32(l_Template->RandomProperty);  ///< Random Properties Seed
    p_Datas << uint32(l_Template->RandomSuffix);    ///< Random Properties ID

    p_Datas.WriteBit(p_ItemBonuses.size() != 0);    ///< Has Item Bonuses
    p_Datas.WriteBit(false);                        ///< Has Modifications
    p_Datas.FlushBits();

    /// Item bonuses
    if (p_ItemBonuses.size() != 0)
    {
        p_Datas << uint8(0);                        ///< Context
        p_Datas << uint32(p_ItemBonuses.size());
        for (auto& l_BonusId : p_ItemBonuses)
            p_Datas << uint32(l_BonusId);
    }
}

void Item::BuildDynamicItemDatas(WorldPacket& p_Datas, uint32 p_Entry, std::vector<uint32> p_ItemBonuses)
{
    ItemTemplate const* l_Template = sObjectMgr->GetItemTemplate(p_Entry);
    if (l_Template == nullptr)
    {
        p_Datas << uint32(p_Entry);                 ///< Item ID
        p_Datas << uint32(0);                       ///< Random Properties Seed
        p_Datas << uint32(0);                       ///< Random Properties ID
        p_Datas.WriteBit(false);                    ///< Has Item Bonuses
        p_Datas.WriteBit(false);                    ///< Has Modifications
        p_Datas.FlushBits();
        return;
    }

    p_Datas << uint32(p_Entry);                     ///< Item ID
    p_Datas << uint32(l_Template->RandomProperty);  ///< Random Properties Seed
    p_Datas << uint32(l_Template->RandomSuffix);    ///< Random Properties ID

    p_Datas.WriteBit(p_ItemBonuses.size() != 0);    ///< Has Item Bonuses
    p_Datas.WriteBit(false);                        ///< Has Modifications
    p_Datas.FlushBits();

    /// Item bonuses
    if (p_ItemBonuses.size() != 0)
    {
        p_Datas << uint8(0);                        ///< Context
        p_Datas << uint32(p_ItemBonuses.size());
        for (auto& l_BonusId : p_ItemBonuses)
            p_Datas << uint32(l_BonusId);
    }
}

int32 Item::GenerateItemRandomPropertyId(uint32 item_id)
{
    ItemTemplate const* itemProto = sObjectMgr->GetItemTemplate(item_id);

    if (!itemProto)
        return 0;

    // item must have one from this field values not null if it can have random enchantments
    if ((!itemProto->RandomProperty) && (!itemProto->RandomSuffix))
        return 0;

    // item can have not null only one from field values
    if ((itemProto->RandomProperty) && (itemProto->RandomSuffix))
    {
        sLog->outError(LOG_FILTER_SQL, "Item template %u have RandomProperty == %u and RandomSuffix == %u, but must have one from field =0", itemProto->ItemId, itemProto->RandomProperty, itemProto->RandomSuffix);
        return 0;
    }

    // RandomProperty case
    if (itemProto->RandomProperty)
    {
        uint32 randomPropId = GetItemEnchantMod(itemProto->RandomProperty, ENCHANTMENT_RANDOM_PROPERTY);
        ItemRandomPropertiesEntry const* random_id = sItemRandomPropertiesStore.LookupEntry(randomPropId);
        if (!random_id)
        {
            sLog->outError(LOG_FILTER_SQL, "Enchantment id #%u used but it doesn't have records in 'ItemRandomProperties.dbc'", randomPropId);
            return 0;
        }

        return random_id->ID;
    }
    // RandomSuffix case
    else
    {
        uint32 randomPropId = GetItemEnchantMod(itemProto->RandomSuffix, ENCHANTMENT_RANDOM_SUFFIX);
        ItemRandomSuffixEntry const* random_id = sItemRandomSuffixStore.LookupEntry(randomPropId);
        if (!random_id)
        {
            sLog->outError(LOG_FILTER_SQL, "Enchantment id #%u used but it doesn't have records in sItemRandomSuffixStore.", randomPropId);
            return 0;
        }

        return -int32(random_id->ID);
    }
}

void Item::SetItemRandomProperties(int32 randomPropId)
{
    if (!randomPropId)
        return;

    if (randomPropId > 0)
    {
        ItemRandomPropertiesEntry const* item_rand = sItemRandomPropertiesStore.LookupEntry(randomPropId);
        if (item_rand)
        {
            if (GetInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID) != int32(item_rand->ID))
            {
                SetInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID, item_rand->ID);
                SetState(ITEM_CHANGED, GetOwner());
            }
            for (uint32 i = PROP_ENCHANTMENT_SLOT_1; i < PROP_ENCHANTMENT_SLOT_1 + 3; ++i)
                SetEnchantment(EnchantmentSlot(i), item_rand->enchant_id[i - PROP_ENCHANTMENT_SLOT_1], 0, 0);
        }
    }
    else
    {
        ItemRandomSuffixEntry const* item_rand = sItemRandomSuffixStore.LookupEntry(-randomPropId);
        if (item_rand)
        {
            if (GetInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID) != -int32(item_rand->ID) ||
                !GetItemSuffixFactor())
            {
                SetInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID, -int32(item_rand->ID));
                UpdateItemSuffixFactor();
                SetState(ITEM_CHANGED, GetOwner());
            }

            for (uint32 i = PROP_ENCHANTMENT_SLOT_0; i <= PROP_ENCHANTMENT_SLOT_4; ++i)
                SetEnchantment(EnchantmentSlot(i), item_rand->enchant_id[i - PROP_ENCHANTMENT_SLOT_0], 0, 0);
        }
    }
}

void Item::UpdateItemSuffixFactor()
{
    uint32 suffixFactor = GenerateEnchSuffixFactor(GetEntry());
    if (GetItemSuffixFactor() == suffixFactor)
        return;
    SetUInt32Value(ITEM_FIELD_PROPERTY_SEED, suffixFactor);
}

void Item::SetState(ItemUpdateState state, Player* forplayer)
{
    if (uState == ITEM_NEW && state == ITEM_REMOVED)
    {
        // pretend the item never existed
        RemoveFromUpdateQueueOf(forplayer);
        forplayer->DeleteRefundReference(GetGUIDLow());
        delete this;
        return;
    }
    if (state != ITEM_UNCHANGED)
    {
        // new items must stay in new state until saved
        if (uState != ITEM_NEW)
            uState = state;

        AddToUpdateQueueOf(forplayer);
    }
    else
    {
        // unset in queue
        // the item must be removed from the queue manually
        uQueuePos = -1;
        uState = ITEM_UNCHANGED;
    }
}

void Item::AddToUpdateQueueOf(Player* player)
{
    if (IsInUpdateQueue())
        return;

    ASSERT(player != NULL);

    if (player->GetGUID() != GetOwnerGUID())
    {
        sLog->outDebug(LOG_FILTER_PLAYER_ITEMS, "Item::AddToUpdateQueueOf - Owner's guid (%u) and player's guid (%u) don't match!", GUID_LOPART(GetOwnerGUID()), player->GetGUIDLow());
        return;
    }

    if (player->m_itemUpdateQueueBlocked)
        return;

    player->m_itemUpdateQueue.push_back(this);
    uQueuePos = player->m_itemUpdateQueue.size()-1;
}

void Item::RemoveFromUpdateQueueOf(Player* player)
{
    if (!IsInUpdateQueue())
        return;

    //ASSERT(player != NULL)

    if (player && player->GetGUID() != GetOwnerGUID())
    {
        sLog->outDebug(LOG_FILTER_PLAYER_ITEMS, "Item::RemoveFromUpdateQueueOf - Owner's guid (%u) and player's guid (%u) don't match!", GUID_LOPART(GetOwnerGUID()), player->GetGUIDLow());
        return;
    }

    if (player && player->m_itemUpdateQueueBlocked)
        return;

    if (player)
        player->m_itemUpdateQueue[uQueuePos] = NULL;

    uQueuePos = -1;
}

uint8 Item::GetBagSlot() const
{
    return m_container ? m_container->GetSlot() : uint8(INVENTORY_SLOT_BAG_0);
}

bool Item::IsEquipped() const
{
    return !IsInBag() && m_slot < EQUIPMENT_SLOT_END;
}

bool Item::CanBeTraded(bool mail, bool trade) const
{
    if (m_lootGenerated)
        return false;

    if ((!mail || !IsBoundAccountWide()) && (IsSoulBound() && (!HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FIELD_FLAG_BOP_TRADEABLE) || !trade)))
        return false;

    if (IsBag() && (Player::IsBagPos(GetPos()) || !((Bag const*)this)->IsEmpty()))
        return false;

    if (Player* owner = GetOwner())
    {
        if (owner->CanUnequipItem(GetPos(), false) != EQUIP_ERR_OK)
            return false;
        if (owner->GetLootGUID() == GetGUID())
            return false;
    }

    if (IsBoundByEnchant())
        return false;

    return true;
}

bool Item::HasEnchantRequiredSkill(const Player* player) const
{
    // Check all enchants for required skill
    for (uint32 enchant_slot = PERM_ENCHANTMENT_SLOT; enchant_slot < MAX_ENCHANTMENT_SLOT; ++enchant_slot)
    {
        if (enchant_slot > ENGINEERING_ENCHANTMENT_SLOT && enchant_slot < PROP_ENCHANTMENT_SLOT_0)    // not holding enchantment id
            continue;

        if (uint32 enchant_id = GetEnchantmentId(EnchantmentSlot(enchant_slot)))
            if (SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id))
                if (enchantEntry->requiredSkill && player->GetSkillValue(enchantEntry->requiredSkill) < enchantEntry->requiredSkillValue)
                    return false;
    }

  return true;
}

uint32 Item::GetEnchantRequiredLevel() const
{
    uint32 level = 0;

    // Check all enchants for required level
    for (uint32 enchant_slot = PERM_ENCHANTMENT_SLOT; enchant_slot < MAX_ENCHANTMENT_SLOT; ++enchant_slot)
    {
        if (enchant_slot > ENGINEERING_ENCHANTMENT_SLOT && enchant_slot < PROP_ENCHANTMENT_SLOT_0)    // not holding enchantment id
            continue;

        if (uint32 enchant_id = GetEnchantmentId(EnchantmentSlot(enchant_slot)))
            if (SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id))
                if (enchantEntry->requiredLevel > level)
                    level = enchantEntry->requiredLevel;
    }

    return level;
}

bool Item::IsBoundByEnchant() const
{
    // Check all enchants for soulbound
    for (uint32 enchant_slot = PERM_ENCHANTMENT_SLOT; enchant_slot < MAX_ENCHANTMENT_SLOT; ++enchant_slot)
    {
        if (enchant_slot > ENGINEERING_ENCHANTMENT_SLOT && enchant_slot < PROP_ENCHANTMENT_SLOT_0)    // not holding enchantment id
            continue;

        if (uint32 enchant_id = GetEnchantmentId(EnchantmentSlot(enchant_slot)))
            if (SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id))
                if (enchantEntry->slot & ENCHANTMENT_CAN_SOULBOUND)
                    return true;
    }

    return false;
}

InventoryResult Item::CanBeMergedPartlyWith(ItemTemplate const* proto) const
{
    // not allow merge looting currently items
    if (m_lootGenerated)
        return EQUIP_ERR_LOOT_GONE;

    // check item type
    if (GetEntry() != proto->ItemId)
        return EQUIP_ERR_CANT_STACK;

    // check free space (full stacks can't be target of merge
    if (GetCount() >= proto->GetMaxStackSize())
        return EQUIP_ERR_CANT_STACK;

    return EQUIP_ERR_OK;
}

bool Item::IsFitToSpellRequirements(SpellInfo const* spellInfo) const
{
    ItemTemplate const* proto = GetTemplate();

    if (spellInfo->EquippedItemClass != -1)                 // -1 == any item class
    {
        // Special case - accept vellum for armor/weapon requirements
        if ((spellInfo->EquippedItemClass == ITEM_CLASS_ARMOR ||
            spellInfo->EquippedItemClass == ITEM_CLASS_WEAPON) && proto->IsVellum())
            if (spellInfo->IsAbilityOfSkillType(SKILL_ENCHANTING)) // only for enchanting spells
                return true;

        if (spellInfo->EquippedItemClass != int32(proto->Class))
            return false;                                   //  wrong item class

        if (spellInfo->EquippedItemSubClassMask != 0)        // 0 == any subclass
        {
            if ((spellInfo->EquippedItemSubClassMask & (1 << proto->SubClass)) == 0)
                return false;                               // subclass not present in mask
        }
    }

    if (spellInfo->EquippedItemInventoryTypeMask != 0)       // 0 == any inventory type
    {
        // Special case - accept weapon type for main and offhand requirements
        if ((proto->InventoryType == INVTYPE_WEAPON || proto->InventoryType == INVTYPE_2HWEAPON) &&
            (spellInfo->EquippedItemInventoryTypeMask & (1 << INVTYPE_WEAPONMAINHAND) ||
             spellInfo->EquippedItemInventoryTypeMask & (1 << INVTYPE_WEAPONOFFHAND)))
            return true;
        else if ((spellInfo->EquippedItemInventoryTypeMask & (1 << proto->InventoryType)) == 0)
            return false;                                   // inventory type not present in mask
    }

    return true;
}

void Item::SetEnchantment(EnchantmentSlot slot, uint32 id, uint32 duration, uint32 charges)
{
    // Better lost small time at check in comparison lost time at item save to DB.
    if ((GetEnchantmentId(slot) == id) && (GetEnchantmentDuration(slot) == duration) && (GetEnchantmentCharges(slot) == charges))
        return;

    SetUInt32Value(ITEM_FIELD_ENCHANTMENT + slot*MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_ID_OFFSET, id);
    SetUInt32Value(ITEM_FIELD_ENCHANTMENT + slot*MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_DURATION_OFFSET, duration);
    SetUInt32Value(ITEM_FIELD_ENCHANTMENT + slot*MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_CHARGES_OFFSET, charges);
    SetState(ITEM_CHANGED, GetOwner());
}

void Item::SetEnchantmentDuration(EnchantmentSlot slot, uint32 duration, Player* owner)
{
    if (GetEnchantmentDuration(slot) == duration)
        return;

    SetUInt32Value(ITEM_FIELD_ENCHANTMENT + slot*MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_DURATION_OFFSET, duration);
    SetState(ITEM_CHANGED, owner);
    // Cannot use GetOwner() here, has to be passed as an argument to avoid freeze due to hashtable locking
}

void Item::SetEnchantmentCharges(EnchantmentSlot slot, uint32 charges)
{
    if (GetEnchantmentCharges(slot) == charges)
        return;

    SetUInt32Value(ITEM_FIELD_ENCHANTMENT + slot*MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_CHARGES_OFFSET, charges);
    SetState(ITEM_CHANGED, GetOwner());
}

void Item::ClearEnchantment(EnchantmentSlot slot)
{
    if (!GetEnchantmentId(slot))
        return;

    for (uint8 x = 0; x < MAX_ENCHANTMENT_SPELLS; ++x)
        SetUInt32Value(ITEM_FIELD_ENCHANTMENT + slot*MAX_ENCHANTMENT_OFFSET + x, 0);
    SetState(ITEM_CHANGED, GetOwner());
}

bool Item::GemsFitSockets() const
{
    for (uint32 enchant_slot = SOCK_ENCHANTMENT_SLOT; enchant_slot < SOCK_ENCHANTMENT_SLOT+MAX_GEM_SOCKETS; ++enchant_slot)
    {
        uint8 SocketColor = GetTemplate()->Socket[enchant_slot-SOCK_ENCHANTMENT_SLOT].Color;

        if (!SocketColor) // no socket slot
            continue;

        uint32 enchant_id = GetEnchantmentId(EnchantmentSlot(enchant_slot));
        if (!enchant_id) // no gems on this socket
            return false;

        SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
        if (!enchantEntry) // invalid gem id on this socket
            return false;

        uint8 GemColor = 0;

        uint32 gemid = enchantEntry->GemID;
        if (gemid)
        {
            ItemTemplate const* gemProto = sObjectMgr->GetItemTemplate(gemid);
            if (gemProto)
            {
                GemPropertiesEntry const* gemProperty = sGemPropertiesStore.LookupEntry(gemProto->GemProperties);
                if (gemProperty)
                    GemColor = gemProperty->color;
            }
        }

        if (!(GemColor & SocketColor)) // bad gem color on this socket
            return false;
    }
    return true;
}

uint8 Item::GetGemCountWithID(uint32 GemID) const
{
    uint8 count = 0;
    for (uint32 enchant_slot = SOCK_ENCHANTMENT_SLOT; enchant_slot < SOCK_ENCHANTMENT_SLOT+MAX_GEM_SOCKETS; ++enchant_slot)
    {
        uint32 enchant_id = GetEnchantmentId(EnchantmentSlot(enchant_slot));
        if (!enchant_id)
            continue;

        SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
        if (!enchantEntry)
            continue;

        if (GemID == enchantEntry->GemID)
            ++count;
    }
    return count;
}

uint8 Item::GetGemCountWithLimitCategory(uint32 limitCategory) const
{
    uint8 count = 0;
    for (uint32 enchant_slot = SOCK_ENCHANTMENT_SLOT; enchant_slot < SOCK_ENCHANTMENT_SLOT+MAX_GEM_SOCKETS; ++enchant_slot)
    {
        uint32 enchant_id = GetEnchantmentId(EnchantmentSlot(enchant_slot));
        if (!enchant_id)
            continue;

        SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
        if (!enchantEntry)
            continue;

        ItemTemplate const* gemProto = sObjectMgr->GetItemTemplate(enchantEntry->GemID);
        if (!gemProto)
            continue;

        if (gemProto->ItemLimitCategory == limitCategory)
            ++count;
    }
    return count;
}

bool Item::IsLimitedToAnotherMapOrZone(uint32 cur_mapId, uint32 cur_zoneId) const
{
    ItemTemplate const* proto = GetTemplate();
    return proto && ((proto->Map && proto->Map != cur_mapId) || (proto->Area && proto->Area != cur_zoneId));
}

void Item::SendTimeUpdate(Player* p_Owner)
{
    uint32 l_Duration = GetUInt32Value(ITEM_FIELD_EXPIRATION);
    if (!l_Duration)
        return;

    WorldPacket l_Data(SMSG_ITEM_TIME_UPDATE, (8+4));
    l_Data.appendPackGUID(GetGUID());
    l_Data << l_Duration;
    p_Owner->GetSession()->SendPacket(&l_Data);
}

Item* Item::CreateItem(uint32 item, uint32 count, Player const* player)
{
    if (count < 1)
        return NULL;                                        //don't create item at zero count

    ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(item);
    if (pProto)
    {
        if (count > pProto->GetMaxStackSize())
            count = pProto->GetMaxStackSize();

        ASSERT(count != 0 && "pProto->Stackable == 0 but checked at loading already");

        Item* pItem = NewItemOrBag(pProto);
        if (pItem->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM), item, player))
        {
            pItem->SetCount(count);
            return pItem;
        }
        else
            delete pItem;
    }
    else
        ASSERT(false);
    return NULL;
}

Item* Item::CloneItem(uint32 p_Count, Player const* p_Player) const
{
    Item* l_NewItem = CreateItem(GetEntry(), p_Count, p_Player);
    if (!l_NewItem)
        return NULL;

    l_NewItem->SetGuidValue(ITEM_FIELD_CREATOR,           GetGuidValue(ITEM_FIELD_CREATOR));
    l_NewItem->SetGuidValue(ITEM_FIELD_GIFT_CREATOR,      GetGuidValue(ITEM_FIELD_GIFT_CREATOR));
    l_NewItem->SetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS,   GetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS) & ~(ITEM_FIELD_FLAG_REFUNDABLE | ITEM_FIELD_FLAG_BOP_TRADEABLE));
    l_NewItem->SetUInt32Value(ITEM_FIELD_EXPIRATION,      GetUInt32Value(ITEM_FIELD_EXPIRATION));

    // player CAN be NULL in which case we must not update random properties because that accesses player's item update queue
#ifndef CROSS
    if (p_Player && p_Player->IsInWorld())
#else /* CROSS */
    if (p_Player)
#endif /* CROSS */
        l_NewItem->SetItemRandomProperties(GetItemRandomPropertyId());

    l_NewItem->AddItemBonuses(GetAllItemBonuses());

    return l_NewItem;
}

bool Item::IsBindedNotWith(Player const* player) const
{
    // not binded item
    if (!IsSoulBound())
        return false;

    // own item
    if (GetOwnerGUID() == player->GetGUID())
        return false;

    if (HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FIELD_FLAG_BOP_TRADEABLE))
        if (allowedGUIDs.find(player->GetGUIDLow()) != allowedGUIDs.end())
            return false;

    // BOA item case
    if (IsBoundAccountWide())
        return false;

    return true;
}

void Item::BuildUpdate(UpdateDataMapType& data_map)
{
    if (Player* owner = GetOwner())
        BuildFieldsUpdate(owner, data_map);
    ClearUpdateMask(false);
}

void Item::BuildDynamicValuesUpdate(uint8 p_UpdateType, ByteBuffer* p_Data, Player* p_Target) const
{
    if (p_Target == nullptr)
        return;

    ByteBuffer l_FieldBuffer;
    UpdateMask l_UpdateMask;

    l_UpdateMask.SetCount(_dynamicValuesCount);

    uint32* l_Flags = nullptr;
    uint32 l_VisibleFlags = GetDynamicUpdateFieldData(p_Target, l_Flags);

    for (uint16 l_I = 0; l_I < _dynamicValuesCount; ++l_I)
    {
        ByteBuffer l_Buffer;
        std::vector<uint32> const& l_Values = _dynamicValues[l_I];

        if (_fieldNotifyFlags & l_Flags[l_I] ||
            ((p_UpdateType == OBJECT_UPDATE_TYPE::UPDATETYPE_VALUES ? _dynamicChangesMask.GetBit(l_I) : !l_Values.empty()) && (l_Flags[l_I] & l_VisibleFlags)) ||
            (l_I == EItemDynamicFields::ITEM_DYNAMIC_FIELD_MODIFIERS &&
            (p_UpdateType == OBJECT_UPDATE_TYPE::UPDATETYPE_VALUES ? _changesMask.GetBit(EItemFields::ITEM_FIELD_MODIFIERS_MASK) : GetUInt32Value(ITEM_FIELD_MODIFIERS_MASK) != 0)))
        {
            l_UpdateMask.SetBit(l_I);

            UpdateMask l_ArrayMask;
            if (l_I != EItemDynamicFields::ITEM_DYNAMIC_FIELD_MODIFIERS)
            {
                l_ArrayMask.SetCount(l_Values.size());

                for (std::size_t l_J = 0; l_J < l_Values.size(); ++l_J)
                {
                    if (p_UpdateType != OBJECT_UPDATE_TYPE::UPDATETYPE_VALUES || _dynamicChangesArrayMask[l_I].GetBit(l_J))
                    {
                        l_ArrayMask.SetBit(l_J);
                        l_Buffer << uint32(l_Values[l_J]);
                    }
                }
            }
            else
            {
                uint32 l_Count = 0;
                l_ArrayMask.SetCount(eItemModifiers::MaxItemModifiers);

                for (uint32 l_J = 0; l_J < eItemModifiers::MaxItemModifiers; ++l_J)
                {
                    if (uint32 l_Modifier = m_Modifiers[l_J])
                    {
                        l_ArrayMask.SetBit(l_Count++);
                        l_Buffer << uint32(l_Modifier);
                    }
                }
            }

            l_FieldBuffer << uint8(l_ArrayMask.GetBlockCount());
            l_ArrayMask.AppendToPacket(&l_FieldBuffer);
            l_FieldBuffer.append(l_Buffer);
        }
    }

    *p_Data << uint8(l_UpdateMask.GetBlockCount());
    l_UpdateMask.AppendToPacket(p_Data);
    p_Data->append(l_FieldBuffer);
}

void Item::SaveRefundDataToDB()
{
#ifndef CROSS
    SQLTransaction trans = CharacterDatabase.BeginTransaction();
#else /* CROSS */
    Player* l_Owner = GetOwner();
    if (!l_Owner)
        return;

    InterRealmDatabasePool* l_Database = l_Owner->GetRealmDatabase();
#endif /* CROSS */

#ifndef CROSS
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_REFUND_INSTANCE);
    stmt->setUInt32(0, GetGUIDLow());
#else /* CROSS */
    SQLTransaction trans = l_Database->BeginTransaction();

    PreparedStatement* stmt = l_Database->GetPreparedStatement(CHAR_DEL_ITEM_REFUND_INSTANCE);
    stmt->setUInt32(0, GetRealGUIDLow());
#endif /* CROSS */
    trans->Append(stmt);

#ifndef CROSS
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_ITEM_REFUND_INSTANCE);
    stmt->setUInt32(0, GetGUIDLow());
#else /* CROSS */
    stmt = l_Database->GetPreparedStatement(CHAR_INS_ITEM_REFUND_INSTANCE);
    stmt->setUInt32(0, GetRealGUIDLow());
#endif /* CROSS */
    stmt->setUInt32(1, GetRefundRecipient());
    stmt->setUInt32(2, GetPaidMoney());
    stmt->setUInt16(3, uint16(GetPaidExtendedCost()));
    trans->Append(stmt);

#ifndef CROSS
    CharacterDatabase.CommitTransaction(trans);
#else /* CROSS */
    l_Database->CommitTransaction(trans);
#endif /* CROSS */
}

void Item::DeleteRefundDataFromDB(SQLTransaction* trans)
{
    if (trans && trans->get() != nullptr)
    {
#ifndef CROSS
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_REFUND_INSTANCE);
        stmt->setUInt32(0, GetGUIDLow());
        (*trans)->Append(stmt);
#else /* CROSS */
        Player* l_Owner = GetOwner();
        if (!l_Owner)
            return;
#endif /* CROSS */

#ifdef CROSS
        InterRealmDatabasePool* l_Database = l_Owner->GetRealmDatabase();
        PreparedStatement* stmt = l_Database->GetPreparedStatement(CHAR_DEL_ITEM_REFUND_INSTANCE);
        stmt->setUInt32(0, GetRealGUIDLow());
        (*trans)->Append(stmt);
#endif /* CROSS */
    }
}

void Item::SetNotRefundable(Player* owner, bool changestate /*=true*/, SQLTransaction* trans /*=NULL*/)
{
    if (!HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FIELD_FLAG_REFUNDABLE))
        return;

    RemoveFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FIELD_FLAG_REFUNDABLE);
    // Following is not applicable in the trading procedure
    if (changestate)
        SetState(ITEM_CHANGED, owner);

    SetRefundRecipient(0);
    SetPaidMoney(0);
    SetPaidExtendedCost(0);
    DeleteRefundDataFromDB(trans);

    owner->DeleteRefundReference(GetGUIDLow());
}

void Item::UpdatePlayedTime(Player* owner)
{
    /*  Here we update our played time
        We simply add a number to the current played time,
        based on the time elapsed since the last update hereof.
    */
    // Get current played time
    uint32 current_playtime = GetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME);
    // Calculate time elapsed since last played time update
    time_t curtime = time(NULL);
    uint32 elapsed = uint32(curtime - m_lastPlayedTimeUpdate);
    uint32 new_playtime = current_playtime + elapsed;
    // Check if the refund timer has expired yet
    if (new_playtime <= 2*HOUR)
    {
        // No? Proceed.
        // Update the data field
        SetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME, new_playtime);
        // Flag as changed to get saved to DB
        SetState(ITEM_CHANGED, owner);
        // Speaks for itself
        m_lastPlayedTimeUpdate = curtime;
        return;
    }
    // Yes
    SetNotRefundable(owner);
}

uint32 Item::GetPlayedTime()
{
    time_t curtime = time(NULL);
    uint32 elapsed = uint32(curtime - m_lastPlayedTimeUpdate);
    return GetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME) + elapsed;
}

bool Item::IsRefundExpired()
{
    return (GetPlayedTime() > 2*HOUR);
}

void Item::SetSoulboundTradeable(AllowedLooterSet& allowedLooters)
{
    SetFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FIELD_FLAG_BOP_TRADEABLE);
    allowedGUIDs = allowedLooters;
}

void Item::ClearSoulboundTradeable(Player* currentOwner)
{
    RemoveFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FIELD_FLAG_BOP_TRADEABLE);
    if (allowedGUIDs.empty())
        return;

    allowedGUIDs.clear();
    SetState(ITEM_CHANGED, currentOwner);
#ifndef CROSS
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_BOP_TRADE);
    stmt->setUInt32(0, GetGUIDLow());
    CharacterDatabase.Execute(stmt);
#else /* CROSS */

    Player* l_Owner = GetOwner();
    if (!l_Owner)
        return;

    InterRealmDatabasePool* l_Database = l_Owner->GetRealmDatabase();

    PreparedStatement* stmt = l_Database->GetPreparedStatement(CHAR_DEL_ITEM_BOP_TRADE);
    stmt->setUInt32(0, GetRealGUIDLow());
    l_Database->Execute(stmt);
#endif /* CROSS */
}

bool Item::CheckSoulboundTradeExpire()
{
    // called from owner's update - GetOwner() MUST be valid
    if (GetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME) + 2*HOUR < GetOwner()->GetTotalPlayedTime())
    {
        ClearSoulboundTradeable(GetOwner());
        return true; // remove from tradeable list
    }

    return false;
}

bool Item::SubclassesCompatible(ItemTemplate const* p_Transmogrifier, ItemTemplate const* p_Transmogrified)
{
    ///   Source     Destination
    if (!p_Transmogrifier || !p_Transmogrified)
        return false;

    /// Patch 5.2 - Throne of Thunder
    /// One-Handed
    /// One-handed axes, maces, and swords can be Transmogrified to each other.
    if ((p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_AXE ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_MACE ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_SWORD) &&
        (p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_AXE ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_MACE ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_SWORD))
        return true;

    /// Two-Handed
    /// Two-handed axes, maces, and swords can be Transmogrified to each other.
    /// WoD Patch 6.0.2 (2014-10-14): Polearms and Staves can now be used to transmogrify Two-Handed Axes, Maces and Swords, and vice versa.
    if ((p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_AXE2 ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_MACE2 ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_STAFF ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_POLEARM ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_SWORD2) &&
        (p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_AXE2 ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_MACE2 ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_STAFF ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_POLEARM ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_SWORD2))
        return true;

    /// Ranged
    if ((p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_BOW ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_GUN ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_CROSSBOW) &&
        (p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_BOW ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_GUN ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_CROSSBOW))
        return true;

    return false;
}

bool Item::CanTransmogrifyItemWithItem(ItemTemplate const* p_Transmogrifier, ItemTemplate const* p_Transmogrified)
{
    if (!p_Transmogrified || !p_Transmogrifier)
        return false;

    if (p_Transmogrified->ItemId == p_Transmogrifier->ItemId)
        return false;

    if (!p_Transmogrifier->CanTransmogrify() || !p_Transmogrified->CanBeTransmogrified())
        return false;

    if (p_Transmogrified->InventoryType == INVTYPE_BAG ||
        p_Transmogrified->InventoryType == INVTYPE_RELIC ||
        p_Transmogrified->InventoryType == INVTYPE_BODY ||
        p_Transmogrified->InventoryType == INVTYPE_FINGER ||
        p_Transmogrified->InventoryType == INVTYPE_TRINKET ||
        p_Transmogrified->InventoryType == INVTYPE_AMMO ||
        p_Transmogrified->InventoryType == INVTYPE_QUIVER ||
        p_Transmogrified->InventoryType == INVTYPE_NON_EQUIP ||
        p_Transmogrified->InventoryType == INVTYPE_TABARD)
        return false;

    if (p_Transmogrifier->InventoryType == INVTYPE_BAG ||
        p_Transmogrifier->InventoryType == INVTYPE_RELIC ||
        p_Transmogrifier->InventoryType == INVTYPE_BODY ||
        p_Transmogrifier->InventoryType == INVTYPE_FINGER ||
        p_Transmogrifier->InventoryType == INVTYPE_TRINKET ||
        p_Transmogrifier->InventoryType == INVTYPE_AMMO ||
        p_Transmogrifier->InventoryType == INVTYPE_QUIVER ||
        p_Transmogrifier->InventoryType == INVTYPE_NON_EQUIP ||
        p_Transmogrifier->InventoryType == INVTYPE_TABARD)
        return false;

    if (p_Transmogrified->Class != p_Transmogrifier->Class)
        return false;

    if (p_Transmogrified->SubClass != ITEM_SUBCLASS_ARMOR_COSMETIC && (p_Transmogrified->Class != ITEM_CLASS_WEAPON || !p_Transmogrifier->IsRangedWeapon() || !p_Transmogrified->IsRangedWeapon()) &&
        (p_Transmogrified->SubClass != p_Transmogrifier->SubClass && !Item::SubclassesCompatible(p_Transmogrified, p_Transmogrifier)))
        return false;

    if (p_Transmogrified->InventoryType != p_Transmogrifier->InventoryType)
    {
        if (p_Transmogrified->Class == ITEM_CLASS_WEAPON && p_Transmogrifier->Class == ITEM_CLASS_WEAPON)
        {
            if (!((p_Transmogrified->InventoryType == INVTYPE_WEAPON || p_Transmogrified->InventoryType == INVTYPE_WEAPONMAINHAND ||
                p_Transmogrified->InventoryType == INVTYPE_WEAPONOFFHAND || p_Transmogrified->InventoryType == INVTYPE_RANGED || p_Transmogrified->InventoryType == INVTYPE_RANGEDRIGHT) &&
                (p_Transmogrifier->InventoryType == INVTYPE_WEAPON || p_Transmogrifier->InventoryType == INVTYPE_WEAPONMAINHAND ||
                p_Transmogrifier->InventoryType == INVTYPE_WEAPONOFFHAND || p_Transmogrifier->InventoryType == INVTYPE_RANGED || p_Transmogrifier->InventoryType == INVTYPE_RANGEDRIGHT)))
                return false;
        }
        else if (p_Transmogrified->Class == ITEM_CLASS_ARMOR && p_Transmogrifier->Class == ITEM_CLASS_ARMOR)
        {
            if (!((p_Transmogrified->InventoryType == INVTYPE_CHEST || p_Transmogrified->InventoryType == INVTYPE_ROBE) &&
                (p_Transmogrifier->InventoryType == INVTYPE_CHEST || p_Transmogrifier->InventoryType == INVTYPE_ROBE)))
                return false;
        }
    }

    /// Check armor types
    if (p_Transmogrified->SubClass != ITEM_SUBCLASS_ARMOR_COSMETIC && (p_Transmogrified->Class == ITEM_CLASS_ARMOR || p_Transmogrifier->Class == ITEM_CLASS_ARMOR))
    {
        uint32 skill1 = p_Transmogrified->GetSkill();
        uint32 skill2 = p_Transmogrifier->GetSkill();

        if ((skill1 == SKILL_PLATE_MAIL || skill1 == SKILL_LEATHER ||
            skill1 == SKILL_MAIL || skill1 == SKILL_CLOTH) ||
            (skill2 == SKILL_PLATE_MAIL || skill2 == SKILL_LEATHER ||
            skill2 == SKILL_MAIL || skill2 == SKILL_CLOTH))
            if (skill1 != skill2)
                return false;
    }

    return true;
}

bool Item::HasStats() const
{
    if (GetItemRandomPropertyId() != 0)
        return true;

    ItemTemplate const* proto = GetTemplate();
    for (uint8 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
        if (proto->ItemStat[i].ItemStatValue != 0)
            return true;

    return false;
}

bool Item::HasSpells() const
{
    ItemTemplate const* proto = GetTemplate();
    for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
        if (proto->Spells[i].SpellId != 0)
            return true;

    return false;
}

// used by mail items, transmog cost, stationeryinfo and others
uint32 Item::GetSellPrice(ItemTemplate const* proto, bool& normalSellPrice)
{
    normalSellPrice = true;

    if (proto->Flags2 & ITEM_FLAG2_HAS_NORMAL_PRICE)
    {
        return proto->BuyPrice;
    }
    else
    {
        ImportPriceQualityEntry const* qualityPrice = sImportPriceQualityStore.LookupEntry(proto->Quality + 1);
        ItemPriceBaseEntry const* basePrice = sItemPriceBaseStore.LookupEntry(proto->ItemLevel);

        if (!qualityPrice || !basePrice)
            return 0;

        float qualityFactor = qualityPrice->Factor;
        float baseFactor = 0.0f;

        uint32 inventoryType = proto->InventoryType;

        if (inventoryType == INVTYPE_WEAPON ||
            inventoryType == INVTYPE_2HWEAPON ||
            inventoryType == INVTYPE_WEAPONMAINHAND ||
            inventoryType == INVTYPE_WEAPONOFFHAND ||
            inventoryType == INVTYPE_RANGED ||
            inventoryType == INVTYPE_THROWN ||
            inventoryType == INVTYPE_RANGEDRIGHT)
            baseFactor = basePrice->WeaponFactor;
        else
            baseFactor = basePrice->ArmorFactor;

        if (inventoryType == INVTYPE_ROBE)
            inventoryType = INVTYPE_CHEST;

        float typeFactor = 0.0f;
        uint8 wepType = 0xFF;

        switch (inventoryType)
        {
            case INVTYPE_HEAD:
            case INVTYPE_SHOULDERS:
            case INVTYPE_CHEST:
            case INVTYPE_WAIST:
            case INVTYPE_LEGS:
            case INVTYPE_FEET:
            case INVTYPE_WRISTS:
            case INVTYPE_HANDS:
            case INVTYPE_CLOAK:
            {
                ImportPriceArmorEntry const* armorPrice = sImportPriceArmorStore.LookupEntry(inventoryType);
                if (!armorPrice)
                    return 0;

                switch (proto->SubClass)
                {
                    case ITEM_SUBCLASS_ARMOR_MISCELLANEOUS:
                    case ITEM_SUBCLASS_ARMOR_CLOTH:
                    {
                        typeFactor = armorPrice->ClothFactor;
                        break;
                    }
                    case ITEM_SUBCLASS_ARMOR_LEATHER:
                    {
                        typeFactor = armorPrice->ClothFactor;
                        break;
                    }
                    case ITEM_SUBCLASS_ARMOR_MAIL:
                    {
                        typeFactor = armorPrice->ClothFactor;
                        break;
                    }
                    case ITEM_SUBCLASS_ARMOR_PLATE:
                    {
                        typeFactor = armorPrice->ClothFactor;
                        break;
                    }
                    default:
                    {
                        return 0;
                    }
                }

                break;
            }
            case INVTYPE_SHIELD:
            {
                ImportPriceShieldEntry const* shieldPrice = sImportPriceShieldStore.LookupEntry(1); // it only has two rows, it's unclear which is the one used
                if (!shieldPrice)
                    return 0;

                typeFactor = shieldPrice->Factor;
                break;
            }
            case INVTYPE_WEAPONMAINHAND:
                wepType = 0;             // unk enum, fall back wepType is never read 01/18/16
            case INVTYPE_WEAPONOFFHAND:
                wepType = 1;             // unk enum, fall back
            case INVTYPE_WEAPON:
                wepType = 2;             // unk enum, fall back
            case INVTYPE_2HWEAPON:
                wepType = 3;             // unk enum, fall back
            case INVTYPE_RANGED:
            case INVTYPE_RANGEDRIGHT:
            case INVTYPE_RELIC:
            {
                wepType = 4;             // unk enum

                ImportPriceWeaponEntry const* weaponPrice = sImportPriceWeaponStore.LookupEntry(wepType + 1);
                if (!weaponPrice)
                    return 0;

                typeFactor = weaponPrice->Factor;
                break;
            }
            default:
                return proto->BuyPrice;
        }

        normalSellPrice = false;
        return (uint32)(qualityFactor * proto->Unk430_2 * proto->Unk430_1 * typeFactor * baseFactor);
    }
}

uint32 Item::GetSpecialPrice(ItemTemplate const* proto, uint32 minimumPrice /*= 10000*/)
{
    uint32 cost = 0;

    if (proto->Flags2 & ITEM_FLAG2_HAS_NORMAL_PRICE)
        cost = proto->SellPrice;
    else
    {
        bool normalPrice;
        cost = Item::GetSellPrice(proto, normalPrice);

        if (!normalPrice)
        {
            if (proto->BuyCount <= 1)
            {
                ItemClassEntry const* classEntry = sItemClassStore.LookupEntry(proto->Class);
                if (classEntry)
                    cost *= classEntry->PriceFactor;
                else
                    cost = 0;
            }
            else
                cost /= 4 * proto->BuyCount;
        }
        else
            cost = proto->SellPrice;
    }

    if (cost < minimumPrice)
        cost = minimumPrice;

    return cost;
}

int32 Item::GetReforgableStat(ItemModType statType) const
{
    ItemTemplate const* proto = GetTemplate();
    for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
        if (proto->ItemStat[i].ItemStatType == (uint32)statType)
            return proto->ItemStat[i].ItemStatValue;

    int32 randomPropId = GetItemRandomPropertyId();
    if (!randomPropId)
        return 0;

    if (randomPropId < 0)
    {
        ItemRandomSuffixEntry const* randomSuffix = sItemRandomSuffixStore.LookupEntry(-randomPropId);
        if (!randomSuffix)
            return 0;

        for (uint32 e = PROP_ENCHANTMENT_SLOT_0; e <= PROP_ENCHANTMENT_SLOT_4; ++e)
        {
            if (SpellItemEnchantmentEntry const* enchant = sSpellItemEnchantmentStore.LookupEntry(GetEnchantmentId(EnchantmentSlot(e))))
            {
                for (uint32 f = 0; f < MAX_ENCHANTMENT_SPELLS; ++f)
                {
                    if (enchant->type[f] == ITEM_ENCHANTMENT_TYPE_STAT && enchant->spellid[f] == (uint32)statType)
                    {
                        for (int k = 0; k < 5; ++k)
                        {
                            if (randomSuffix->enchant_id[k] == enchant->ID)
                                return int32((randomSuffix->prefix[k] * GetItemSuffixFactor()) / 10000);
                        }
                    }
                }
            }
        }
    }
    else
    {
        ItemRandomPropertiesEntry const* randomProp = sItemRandomPropertiesStore.LookupEntry(randomPropId);
        if (!randomProp)
            return 0;

        for (uint32 e = PROP_ENCHANTMENT_SLOT_0; e <= PROP_ENCHANTMENT_SLOT_4; ++e)
        {
            if (SpellItemEnchantmentEntry const* enchant = sSpellItemEnchantmentStore.LookupEntry(GetEnchantmentId(EnchantmentSlot(e))))
            {
                for (uint32 f = 0; f < MAX_ENCHANTMENT_SPELLS; ++f)
                {
                    if (enchant->type[f] == ITEM_ENCHANTMENT_TYPE_STAT && enchant->spellid[f] == (uint32)statType)
                    {
                        for (int k = 0; k < MAX_ENCHANTMENT_SPELLS; ++k)
                        {
                            if (randomProp->enchant_id[k] == enchant->ID)
                                return int32(enchant->amount[k]);
                        }
                    }
                }
            }
        }
    }

    return 0;
}

bool Item::IsPotion() const
{
    return GetTemplate()->IsPotion();
}

bool Item::IsPvPItem() const
{
    ItemTemplate const* proto = GetTemplate();
    if (!proto)
        return false;

    for (uint8 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
    {
        uint32 stat = proto->ItemStat[i].ItemStatType;
        if (stat == ITEM_MOD_PVP_POWER || stat == ITEM_MOD_RESILIENCE_RATING)
            return true;
    }

    for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
    {
        int32 spell = proto->Spells[i].SpellId;
        if (spell == 132586 || spell == 139891)
            return true;
    }

    return false;
}

bool Item::IsStuffItem() const
{
    ItemTemplate const* proto = GetTemplate();
    if (!proto)
        return false;

    uint32 invType = proto->InventoryType;

    switch (invType)
    {
        case INVTYPE_NON_EQUIP:
        case INVTYPE_BODY:
        case INVTYPE_BAG:
        case INVTYPE_TABARD:
        case INVTYPE_AMMO:
        case INVTYPE_THROWN:
        case INVTYPE_QUIVER:
        case INVTYPE_RELIC:
            return false;
        default:
            return true;
    }
}

bool Item::CanUpgrade() const
{
    ItemTemplate const* proto = GetTemplate();
    if (!proto)
        return false;

    if (proto->ItemLevel < 458)
        return false;

    if (proto->Quality == ITEM_QUALITY_LEGENDARY && !IsLegendaryCloak())
        return false;

    if (!IsStuffItem())
        return false;

    if (proto->Class == ITEM_CLASS_WEAPON && proto->SubClass == ITEM_SUBCLASS_WEAPON_FISHING_POLE)
        return false;

    if (!HasStats() && proto->InventoryType != INVTYPE_TRINKET)
        return false;

    // PvP item can't be upgraded after Season 12
    if (IsPvPItem() && proto->ItemLevel > 483)
        return false;

    return true;
}

bool Item::IsLegendaryCloak() const
{
    ItemTemplate const* proto = GetTemplate();
    if (!proto)
        return false;

    switch (proto->ItemId)
    {
        case 102245: // Qian-Le, Courage of Niuzao
        case 102246: // Xing-Ho, Breath of Yu'lon
        case 102247: // Jina-Kang, Kindness of Chi-Ji
        case 102248: // Fen-Yu, Fury of Xuen
        case 102249: // Gong-Lu, Strength of Xuen
        case 102250: // Qian-Ying, Fortitude of Niuzao
            return true;
        default:
            break;
    }

    return false;
}

float ItemTemplate::GetScalingDamageValue(uint32 ilvl) const
{
    if (Quality > ITEM_QUALITY_HEIRLOOM)
        return 0.0f;

    ItemDamageEntry const* damageEntry = NULL;

    switch (InventoryType)
    {
        case INVTYPE_WEAPON:
        case INVTYPE_WEAPONMAINHAND:
        case INVTYPE_WEAPONOFFHAND:
            if (Flags2 & ITEM_FLAG2_CASTER_WEAPON)
            {
                damageEntry =  sItemDamageOneHandCasterStore.LookupEntry(ilvl);
                break;
            }
            damageEntry = sItemDamageOneHandStore.LookupEntry(ilvl);
            break;
    case INVTYPE_RANGED:
    case INVTYPE_THROWN:
    case INVTYPE_RANGEDRIGHT:
        if (SubClass < 4)
        {
            if (Flags2 & ITEM_FLAG2_CASTER_WEAPON)
            {
                damageEntry = sItemDamageTwoHandCasterStore.LookupEntry(ilvl);
                break;
            }
            damageEntry = sItemDamageTwoHandStore.LookupEntry(ilvl);
            break;
        }
        else if (SubClass == 19)
        {
            damageEntry = sItemDamageOneHandCasterStore.LookupEntry(ilvl);
            break;
        }
        else
        {
            if (Flags2 & ITEM_FLAG2_CASTER_WEAPON)
            {
                damageEntry = sItemDamageTwoHandCasterStore.LookupEntry(ilvl);
                break;
            }
            damageEntry = sItemDamageTwoHandStore.LookupEntry(ilvl);
            break;
        }
    case INVTYPE_AMMO:
        damageEntry = sItemDamageAmmoStore.LookupEntry(ilvl);
        break;
    case INVTYPE_2HWEAPON:
        if (Flags2 & ITEM_FLAG2_CASTER_WEAPON)
        {
            damageEntry = sItemDamageTwoHandCasterStore.LookupEntry(ilvl);
            break;
        }
        damageEntry = sItemDamageTwoHandStore.LookupEntry(ilvl);
        break;
    default:
        break;
    }
    return damageEntry ? damageEntry->DPS[Quality == ITEM_QUALITY_HEIRLOOM ? ITEM_QUALITY_RARE : Quality] : 0.0f;
}

int32 ItemTemplate::GetRandomPointsOffset() const
{
    switch (InventoryType)
    {
        case INVTYPE_NECK:
        case INVTYPE_WRISTS:
        case INVTYPE_FINGER:
        case INVTYPE_SHIELD:
        case INVTYPE_CLOAK:
        case INVTYPE_HOLDABLE:
            return 2;
        case INVTYPE_SHOULDERS:
        case INVTYPE_WAIST:
        case INVTYPE_FEET:
        case INVTYPE_HANDS:
        case INVTYPE_TRINKET:
            return 1;
        case INVTYPE_WEAPON:
        case INVTYPE_WEAPONMAINHAND:
        case INVTYPE_WEAPONOFFHAND:
            return 3;
        case INVTYPE_RANGEDRIGHT:
            return 3 * (SubClass == 19 ? 1 : 0);
        case INVTYPE_RELIC:
            return 4;
        case INVTYPE_HEAD:
        case INVTYPE_BODY:
        case INVTYPE_CHEST:
        case INVTYPE_LEGS:
        case INVTYPE_RANGED:
        case INVTYPE_2HWEAPON:
        case INVTYPE_ROBE:
        case INVTYPE_THROWN:
            return 0;
        default:
            return -1;
    }
}

uint32 ItemTemplate::CalculateScalingStatDBCValue(uint32 ilvl) const
{
    int32 offset = GetRandomPointsOffset();
    if (offset == -1)
        return 0;

    RandomPropertiesPointsEntry const* randProperty = sRandomPropertiesPointsStore.LookupEntry(ilvl);

    if (!randProperty)
        return 0;

    switch (Quality)
    {
        case ITEM_QUALITY_UNCOMMON:
        return randProperty->UncommonPropertiesPoints[offset];
        case ITEM_QUALITY_RARE:
        case ITEM_QUALITY_HEIRLOOM:
            return randProperty->RarePropertiesPoints[offset];
        case ITEM_QUALITY_EPIC:
            return randProperty->EpicPropertiesPoints[offset];
        default:
            return 0;
    }
}

float ItemTemplate::GetSocketCost(uint32 ilvl) const
{
    gtItemSocketCostPerLevelEntry const* socket = sgtItemSocketCostPerLevelStore.LookupEntry(ilvl);
    return socket ? socket->cost : 0.0f;
}

int32 ItemTemplate::CalculateStatScaling(uint32 index, uint32 ilvl) const
{
    _ItemStat const& itemStat = ItemStat[index];
    return CalculateStatScaling(itemStat.ScalingValue, itemStat.SocketCostRate, ilvl);
}

int32 ItemTemplate::CalculateStatScaling(int32 scalingValue, float socketCost, uint32 ilvl) const
{
    return floor((((float)scalingValue * (float)CalculateScalingStatDBCValue(ilvl) * 0.000099999997f) - (GetSocketCost(ilvl) * socketCost)) + 0.5f);
}

uint32 ItemTemplate::CalculateArmorScaling(uint32 ilvl) const
{
    if (ilvl == ItemLevel)
        return Armor;

    uint32 quality = Quality == ITEM_QUALITY_HEIRLOOM ? ITEM_QUALITY_RARE : Quality;

    if (Class != ITEM_CLASS_ARMOR || SubClass != ITEM_SUBCLASS_ARMOR_SHIELD)
    {
        if (InventoryType == 1 || InventoryType == 5 || InventoryType == 3 || InventoryType == 7 || InventoryType == 8 || InventoryType == 9 || InventoryType == 10 || InventoryType == 6 || InventoryType == 16 || InventoryType == 20)
        {
            ItemArmorQualityEntry const* armorQuality = sItemArmorQualityStore.LookupEntry(ilvl);
            ItemArmorTotalEntry const* armorTotal = sItemArmorTotalStore.LookupEntry(ilvl);
            ArmorLocationEntry const* armorLoc = sArmorLocationStore.LookupEntry(InventoryType == 20 ? 5 : InventoryType);

            if (SubClass == 0 || SubClass > 4)
                return 0.0f;

            if (armorQuality == nullptr || armorTotal == nullptr || armorLoc == nullptr)
                return 0.0f;

            return (int)floor(armorQuality->Value[quality] * armorTotal->Value[SubClass - 1] * armorLoc->Value[SubClass - 1] + 0.5f);
        }
        return 0;
    }
    else
    {
        ItemArmorShieldEntry const* shieldEntry = sItemArmorShieldStore.LookupEntry(ilvl);
        if (shieldEntry == nullptr)
            return 0;

        return shieldEntry->Value[quality];
    }
}

void ItemTemplate::CalculateMinMaxDamageScaling(uint32 ilvl, uint32& minDamage, uint32& maxDamage) const
{
    minDamage = 0;
    maxDamage = 0;

    if (ilvl == ItemLevel)
    {
        minDamage = DamageMin;
        maxDamage = DamageMax;
        return;
    }

    if (!IsWeapon())
        return;

    float weaponMinDamageCalc = (float)Delay * GetScalingDamageValue(ilvl) * 0.001f;
    float weaponMaxDamageCalc = (((StatScalingFactor * 0.5f) + 1.0f) * weaponMinDamageCalc) + 0.5f;

    if (Delay != 0)
    {
        float delayModifier = 1000.0f / (float)Delay;
        float midCalc = (delayModifier * ((1.0f - (StatScalingFactor * 0.5f)) * weaponMinDamageCalc)) + ArmorDamageModifier;
        midCalc = midCalc > 1.0f ? midCalc : 1.0f;
        float delayCoeff = 1.0f / delayModifier;
        minDamage = floor((delayCoeff * midCalc) + 0.5f);
        maxDamage = floor((delayCoeff * ((delayModifier * weaponMaxDamageCalc) + ArmorDamageModifier)) + 0.5f);
    }
    else
    {
        maxDamage = floor(weaponMaxDamageCalc + 0.5f);
        minDamage = floor(((1.0f - (StatScalingFactor * 0.5f)) * weaponMinDamageCalc) + 0.5f);
    }
}

bool Item::AddItemBonus(uint32 p_ItemBonusId)
{
    if (!GetItemBonusesByID(p_ItemBonusId))
        return false;

    if (HasItemBonus(p_ItemBonusId))
        return false;

    SetDynamicValue(ITEM_DYNAMIC_FIELD_BONUS_LIST_IDS, GetAllItemBonuses().size(), p_ItemBonusId);
    return true;
}

void Item::AddItemBonuses(std::vector<uint32> const& p_ItemBonuses)
{
    if (!p_ItemBonuses.size())
        return;

    for (uint32 i = 0; i < p_ItemBonuses.size(); i++)
        AddItemBonus(p_ItemBonuses[i]);
}

bool Item::HasItemBonus(uint32 p_ItemBonusId) const
{
    std::vector<uint32> const& l_BonusList = GetAllItemBonuses();
    for (auto& l_Bonus : l_BonusList)
        if (l_Bonus == p_ItemBonusId)
            return true;
    return false;
}

bool Item::HasItemBonusType(ItemBonusType p_Type) const
{
    std::vector<uint32> const& l_BonusList = GetAllItemBonuses();
    for (auto& l_Bonus : l_BonusList)
    {
        std::vector<ItemBonusEntry const*> const* l_Bonuses = GetItemBonusesByID(l_Bonus);
        if (l_Bonuses == nullptr)
            continue;

        for (uint32 l_I = 0; l_I < l_Bonuses->size(); ++l_I)
        {
            ItemBonusEntry const* l_ItemSubBonus = (*l_Bonuses)[l_I];
            if (!l_ItemSubBonus)
                continue;

            if (l_ItemSubBonus->Type == p_Type)
                return true;
        }
    }

    return false;
}

bool Item::RemoveItemBonus(uint32 p_ItemBonusId)
{
    std::vector<uint32> const& l_BonusList = GetAllItemBonuses();

    for (uint32 i = 0; i < l_BonusList.size(); i++)
    {
        if (l_BonusList[i] == p_ItemBonusId && p_ItemBonusId)
        {
            RemoveDynamicValue(ITEM_DYNAMIC_FIELD_BONUS_LIST_IDS, i);
            return true;
        }
    }

    return false;
}

void Item::RemoveAllItemBonuses()
{
    std::vector<uint32> const& l_BonusList = GetAllItemBonuses();
    for (auto& l_Bonus : l_BonusList)
        if (l_Bonus)
            RemoveItemBonus(l_Bonus);
}

std::vector<uint32> const& Item::GetAllItemBonuses() const
{
    return GetDynamicValues(ITEM_DYNAMIC_FIELD_BONUS_LIST_IDS);
}

uint32 Item::GetItemLevelBonusFromItemBonuses() const
{
    uint32 itemLevel = 0;
    for (auto l_Bonus : GetAllItemBonuses())
    {
        if (!l_Bonus)
            continue;

        std::vector<ItemBonusEntry const*> const* l_ItemBonus = GetItemBonusesByID(l_Bonus);
        if (!l_ItemBonus)
            continue;

        for (uint32 i = 0; i < l_ItemBonus->size(); i++)
        {
            ItemBonusEntry const* l_ItemSubBonus = (*l_ItemBonus)[i];
            if (!l_ItemSubBonus)
                break;

            if (l_ItemSubBonus->Type == ITEM_BONUS_MODIFY_ITEM_LEVEL)
                itemLevel += l_ItemSubBonus->Value[0];
        }
    }

    return itemLevel;
}

uint16 Item::GetVisibleAppearanceModID() const
{
    if (GetModifier(eItemModifiers::TransmogItemID))
        return uint16(GetModifier(eItemModifiers::TransmogAppearanceMod));

    return uint16(GetAppearanceModID());
}

uint16 Item::GetAppearanceModID() const
{
    uint32 l_Appearance = 0;

    for (uint32 l_Bonus : GetDynamicValues(ITEM_DYNAMIC_FIELD_BONUS_LIST_IDS))
    {
        std::vector<ItemBonusEntry const*> const* l_Bonuses = GetItemBonusesByID(l_Bonus);
        if (l_Bonuses == nullptr)
            continue;

        for (uint32 l_I = 0; l_I < l_Bonuses->size(); l_I++)
        {
            ItemBonusEntry const* l_ItemSubBonus = (*l_Bonuses)[l_I];
            if (!l_ItemSubBonus)
                continue;

            if (l_ItemSubBonus->Type == ITEM_BONUS_MODIFY_APPEARANCE && l_ItemSubBonus->Value[0] > l_Appearance)
                l_Appearance = l_ItemSubBonus->Value[0];
        }
    }

    return (uint16)l_Appearance;
}

uint32 Item::GetVisibleEntry() const
{
    if (uint32 l_Transmogrification = GetModifier(eItemModifiers::TransmogItemID))
        return l_Transmogrification;

    return GetEntry();
}

uint32 Item::GetVisibleEnchantmentId() const
{
    if (uint32 l_EnchantIllusion = GetModifier(eItemModifiers::EnchantIllusion))
        return l_EnchantIllusion;

    return GetEnchantmentId(PERM_ENCHANTMENT_SLOT);
}

uint16 Item::GetVisibleItemVisual() const
{
    if (SpellItemEnchantmentEntry const* l_Enchant = sSpellItemEnchantmentStore.LookupEntry(GetVisibleEnchantmentId()))
        return l_Enchant->itemVisualID;

    return 0;
}

void Item::SetModifier(eItemModifiers p_Modifier, uint32 p_Value)
{
    if (p_Modifier >= eItemModifiers::MaxItemModifiers)
        return;

    m_Modifiers[p_Modifier] = p_Value;
    ApplyModFlag(EItemFields::ITEM_FIELD_MODIFIERS_MASK, 1 << p_Modifier, p_Value != 0);
#ifdef CROSS
}

uint64 Item::GetRealOwnerGUID() const
{
    if (Player* l_Owner = GetOwner())
        return l_Owner->GetRealGUID();
    return 0;
#endif /* CROSS */
}

bool Item::SubclassesCompatibleForRandomWeapon(ItemTemplate const* p_Transmogrifier, ItemTemplate const* p_Transmogrified)
{
    if (!p_Transmogrifier || !p_Transmogrified)
        return false;

    /// One-Handed can be transmogrified to each other
    if ((p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_AXE ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_MACE ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_SWORD ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_DAGGER ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_FIST_WEAPON) &&
        (p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_AXE ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_MACE ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_SWORD ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_DAGGER ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_FIST_WEAPON))
        return true;

    /// Two-Handed
    /// Two-handed axes, maces, and swords can be Transmogrified to each other.
    if ((p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_AXE2 ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_MACE2 ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_SWORD2 ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_POLEARM ||
        p_Transmogrifier->SubClass == ITEM_SUBCLASS_WEAPON_STAFF) &&
        (p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_AXE2 ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_MACE2 ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_SWORD2 ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_POLEARM ||
        p_Transmogrified->SubClass == ITEM_SUBCLASS_WEAPON_STAFF))
        return true;

    return false;
}

bool Item::CanTransmogrifyIntoRandomWeapon(ItemTemplate const* p_Transmogrifier, ItemTemplate const* p_Transmogrified)
{
    if (!p_Transmogrified || !p_Transmogrifier)
        return false;

    if (p_Transmogrified->ItemId == p_Transmogrifier->ItemId)
        return false;

    if (p_Transmogrified->Class != ITEM_CLASS_WEAPON || p_Transmogrifier->Class != ITEM_CLASS_WEAPON)
        return false;

    if (p_Transmogrified->InventoryType != INVTYPE_2HWEAPON &&
        p_Transmogrified->InventoryType != INVTYPE_WEAPONMAINHAND &&
        p_Transmogrified->InventoryType != INVTYPE_WEAPONOFFHAND)
        return false;

    if (p_Transmogrifier->InventoryType != INVTYPE_2HWEAPON &&
        p_Transmogrifier->InventoryType != INVTYPE_WEAPONMAINHAND &&
        p_Transmogrifier->InventoryType != INVTYPE_WEAPONOFFHAND)
        return false;

    if (!p_Transmogrifier->CanTransmogrify() || !p_Transmogrified->CanBeTransmogrified())
        return false;

    if (!SubclassesCompatibleForRandomWeapon(p_Transmogrifier, p_Transmogrified))
        return false;

    return true;
}

void Item::RandomWeaponTransmogrificationFromPrimaryBag(Player* p_Player, Item* p_Transmogrified, bool p_Apply)
{
    if (!p_Player || !p_Transmogrified)
        return;

    uint8 l_TransmogrifiedItemSlot = p_Transmogrified->GetSlot();
    /// Wrong transmogrified item, we can change just weapons
    if (l_TransmogrifiedItemSlot != EQUIPMENT_SLOT_MAINHAND && l_TransmogrifiedItemSlot != EQUIPMENT_SLOT_OFFHAND)
        return;

    /// Apply transmogrification on weapon
    if (p_Apply)
    {
        /// Find item template of Transmogrified weapon
        ItemTemplate const* l_TransmogrifiedTemplate = p_Transmogrified->GetTemplate();
        if (!l_TransmogrifiedTemplate)
            return;

        /// Select random weapon from primary bag
        uint32 l_RandomWeaponId = p_Player->GetRandomWeaponFromPrimaryBag(l_TransmogrifiedTemplate);

        /// Find item template of Transmogrifier weapon
        ItemTemplate const* l_TransmogrifierTemplate = sObjectMgr->GetItemTemplate(l_RandomWeaponId);
        if (!l_TransmogrifierTemplate)
            return;

        /// Find item in player inventory
        Item* l_Transmogrifier = p_Player->GetItemByEntry(l_TransmogrifiedTemplate->ItemId);
        if (!l_Transmogrifier)
            return;

        /// Apply transmogrification on weapon
        p_Transmogrified->SetModifier(eItemModifiers::TransmogItemID, l_TransmogrifiedTemplate->ItemId);
        p_Player->SetVisibleItemSlot(l_TransmogrifiedItemSlot, p_Transmogrified);

        p_Transmogrified->UpdatePlayedTime(p_Player);

        p_Transmogrified->SetOwnerGUID(p_Player->GetGUID());
        p_Transmogrified->SetNotRefundable(p_Player);
        p_Transmogrified->ClearSoulboundTradeable(p_Player);

        /// Some rules for Transmogrifier weapon
        if (l_Transmogrifier != nullptr)
        {
            if (l_TransmogrifierTemplate->Bonding == BIND_WHEN_EQUIPED || l_TransmogrifierTemplate->Bonding == BIND_WHEN_USE)
                l_Transmogrifier->SetBinding(true);

            l_Transmogrifier->SetOwnerGUID(p_Player->GetGUID());
            l_Transmogrifier->SetNotRefundable(p_Player);
            l_Transmogrifier->ClearSoulboundTradeable(p_Player);
        }
    }
    /// Remove transmogrification from weapon
    else
    {
        p_Transmogrified->SetModifier(eItemModifiers::TransmogItemID, 0);
        p_Player->SetVisibleItemSlot(l_TransmogrifiedItemSlot, p_Transmogrified);
    }
}

uint32 Item::GetEnchantItemVisualId(EnchantmentSlot p_Slot) const
{
    SpellItemEnchantmentEntry const* l_Enchantement = sSpellItemEnchantmentStore.LookupEntry(GetEnchantmentId(p_Slot));
    if (l_Enchantement == nullptr)
        return 0;

    /// Special handler for SPELL_EFFECT_APPLY_ENCHANT_ILLUSION that should change visual effect of item
    SpellItemEnchantmentEntry const* l_EnchantementIllusion = sSpellItemEnchantmentStore.LookupEntry(GetEnchantmentId(BONUS_ENCHANTMENT_SLOT));
    if (l_EnchantementIllusion != nullptr)
    {
        uint32 l_IllusionVisualID = l_EnchantementIllusion->itemVisualID;
        if (l_IllusionVisualID != 0)
            return l_IllusionVisualID;
    }

    return l_Enchantement->itemVisualID;
}
