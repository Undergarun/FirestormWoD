#include <ScriptPCH.h>
#include <ScriptMgr.h>

namespace WebShop
{
    enum Language
    {
        GoldLimitReach      = 3000,
        GoldAddNextLogin    = 3001,
        GoldAdded           = 3002,
        DontHaveEnoughSpace = 3003
    };
    
    /// ...of Draenor Spell
    enum ProfessionBookSpells
    {
        Alchemy         = 156614,
        Blacksmithing   = 169923,
        Enchanting      = 161788,
        Engineering     = 161787,
        Inscription     = 161789,
        JewelCrafting   = 169926,
        LeatherWorking  = 169925,
        Tailoring       = 169924,
        FirstAid        = 160329,
        Cooking         = 160360,
        Herbalism       = 160319,
        Mining          = 160315,
        Skinning        = 160321,
        Archaeology     = 158762,
        Fishing         = 160326
    };

    enum ProfessionAdditionalSpells
    {
        Prospecting = 31252,
    };

    class Delivery_Item : public PlayerScript
    {
        private:
            ACE_Based::LockedMap<uint32, QueryResultFuture> m_QueryResultFutures;

        public:
            Delivery_Item() : PlayerScript("WebshopDelivery_Item")
            {
            }

            void OnLogin(Player* p_Player) override
            {
                uint32 l_LowGuid = p_Player->GetGUIDLow();
                QueryResultFuture& l_Callback = m_QueryResultFutures[p_Player->GetGUIDLow()];
                l_Callback = CharacterDatabase.AsyncPQuery("SELECT itemid, ItemBonus, count, transaction FROM webshop_delivery_item WHERE guid = '%u' and delivery = 0", l_LowGuid);
            }

            void OnUpdate(Player* p_Player, uint32 /*p_Diff*/) override
            {
                uint32 l_LowGuid = p_Player->GetGUIDLow();

                auto l_Itr = m_QueryResultFutures.find(l_LowGuid);
                if (l_Itr == m_QueryResultFutures.end())
                    return;

                QueryResultFuture& l_Callback = l_Itr->second;
                if (!l_Callback.ready())
                    return;

                QueryResult l_Result;
                l_Callback.get(l_Result);

                if (!l_Result)
                {
                    l_Callback.cancel();
                    m_QueryResultFutures.erase(l_LowGuid);
                    p_Player->SetStoreDeliveryProccesed(StoreCallback::ItemDelivery);
                    return;
                }

                do
                {
                    Field* l_Field = l_Result->Fetch();
                    uint32 l_ItemID = l_Field[0].GetUInt32();
                    std::string l_Bonuses = l_Field[1].GetString();
                    uint32 l_ItemCount = l_Field[2].GetUInt32();
                    uint32 l_Transaction = l_Field[3].GetUInt32();

                    ItemPosCountVec l_Dest;
                    InventoryResult l_Message = p_Player->CanStoreNewItem(NULL_BAG, NULL_SLOT, l_Dest, l_ItemID, l_ItemCount);

                    if (l_Message != EQUIP_ERR_OK)
                    {
                        ChatHandler(p_Player).PSendSysMessage(WebShop::Language::DontHaveEnoughSpace);
                        continue;
                    }

                    std::list<uint32> l_BonusesID;
                    Tokenizer l_Tokens(l_Bonuses, ',');

                    for (Tokenizer::const_iterator l_Iter = l_Tokens.begin(); l_Iter != l_Tokens.end(); ++l_Iter)
                        l_BonusesID.push_back(uint32(atol(*l_Iter)));

                    Item* l_Item = p_Player->AddItem(l_ItemID, l_ItemCount, l_BonusesID);
                    if (l_Item != nullptr)
                    {
                        l_Item->SetCustomFlags(ItemCustomFlags::FromStore);
                        l_Item->SetState(ItemUpdateState::ITEM_CHANGED, p_Player);
                        CharacterDatabase.PExecute("UPDATE webshop_delivery_item SET delivery = 1 WHERE transaction = %u", l_Transaction);
                    }
                } 
                while (l_Result->NextRow());

                l_Callback.cancel();
                m_QueryResultFutures.erase(l_LowGuid);
                p_Player->SetStoreDeliveryProccesed(StoreCallback::ItemDelivery);
            }
    };

    class Delivery_Gold : public PlayerScript
    {
        private:
            ACE_Based::LockedMap<uint32, QueryResultFuture> m_QueryResultFutures;

        public:
            Delivery_Gold() : PlayerScript("WebshopDelivery_Gold")
            {
            }

            void OnLogin(Player* p_Player) override
            {
                uint32 l_LowGuid = p_Player->GetGUIDLow();
                QueryResultFuture& l_Callback = m_QueryResultFutures[p_Player->GetGUIDLow()];
                l_Callback = CharacterDatabase.AsyncPQuery("SELECT gold, transaction FROM webshop_delivery_gold WHERE guid = '%u' and delivery = 0", l_LowGuid);
            }

            void OnUpdate(Player* p_Player, uint32 /*p_Diff*/) override
            {
                uint32 l_LowGuid = p_Player->GetGUIDLow();

                auto l_Itr = m_QueryResultFutures.find(l_LowGuid);
                if (l_Itr == m_QueryResultFutures.end())
                    return;

                QueryResultFuture& l_Callback = l_Itr->second;
                if (!l_Callback.ready())
                    return;

                QueryResult l_Result;
                l_Callback.get(l_Result);

                if (!l_Result)
                {
                    l_Callback.cancel();
                    m_QueryResultFutures.erase(l_LowGuid);
                    p_Player->SetStoreDeliveryProccesed(StoreCallback::GoldDelivery);
                    return;
                }

                uint64 l_GoldCount = 0;
                do
                {
                    Field* l_Field       = l_Result->Fetch();
                    uint64 l_Gold        = uint64(l_Field[0].GetUInt32()) * GOLD;
                    uint32 l_Transaction = l_Field[1].GetUInt32();

                    if ((p_Player->GetMoney() + l_Gold) > MAX_MONEY_AMOUNT)
                    {
                        ChatHandler(p_Player).PSendSysMessage(WebShop::Language::GoldLimitReach);
                        ChatHandler(p_Player).PSendSysMessage(WebShop::Language::GoldAddNextLogin);
                        break;
                    }

                    l_GoldCount += l_Gold;
                    p_Player->ModifyMoney(l_Gold);

                    CharacterDatabase.PExecute("UPDATE webshop_delivery_gold SET delivery = 1 WHERE transaction = %u", l_Transaction);
                } 
                while (l_Result->NextRow());

                if (l_GoldCount != 0)
                    ChatHandler(p_Player).PSendSysMessage(WebShop::Language::GoldAdded, l_GoldCount / GOLD);

                l_Callback.cancel();
                m_QueryResultFutures.erase(l_LowGuid);
                p_Player->SetStoreDeliveryProccesed(StoreCallback::GoldDelivery);
            }
    };

    class Delivery_Currency : public PlayerScript
    {
        private:
            ACE_Based::LockedMap<uint32, QueryResultFuture> m_QueryResultFutures;

        public:
            Delivery_Currency() : PlayerScript("WebshopDelivery_Currency")
            {
            }

            void OnLogin(Player* p_Player) override
            {
                uint32 l_LowGuid = p_Player->GetGUIDLow();
                QueryResultFuture& l_Callback = m_QueryResultFutures[p_Player->GetGUIDLow()];
                l_Callback = CharacterDatabase.AsyncPQuery("SELECT currency, amount, transaction FROM webshop_delivery_currency WHERE guid = '%u' and delivery = 0", l_LowGuid);
            }

            void OnUpdate(Player* p_Player, uint32 /*p_Diff*/) override
            {
                uint32 l_LowGuid = p_Player->GetGUIDLow();

                auto l_Itr = m_QueryResultFutures.find(l_LowGuid);
                if (l_Itr == m_QueryResultFutures.end())
                    return;

                QueryResultFuture& l_Callback = l_Itr->second;
                if (!l_Callback.ready())
                    return;

                QueryResult l_Result;
                l_Callback.get(l_Result);

                if (!l_Result)
                {
                    l_Callback.cancel();
                    m_QueryResultFutures.erase(l_LowGuid);
                    p_Player->SetStoreDeliveryProccesed(StoreCallback::CurrencyDelivery);
                    return;
                }

                do
                {
                    Field* l_Field       = l_Result->Fetch();
                    uint32 l_Currency    = l_Field[0].GetUInt32();
                    uint32 l_Amount      = l_Field[1].GetUInt32();
                    uint32 l_Transaction = l_Field[2].GetUInt32();

                    CurrencyTypesEntry const* l_CurrencyEntry = sCurrencyTypesStore.LookupEntry(l_Currency);
                    if (!l_CurrencyEntry)
                        continue;

                    l_Amount *= l_CurrencyEntry->GetPrecision();
                    p_Player->ModifyCurrency(l_Currency, l_Amount, true, true, true);

                    CharacterDatabase.PExecute("UPDATE webshop_delivery_currency SET delivery = 1 WHERE transaction = %u", l_Transaction);
                } 
                while (l_Result->NextRow());

                l_Callback.cancel();
                m_QueryResultFutures.erase(l_LowGuid);
                p_Player->SetStoreDeliveryProccesed(StoreCallback::CurrencyDelivery);
            }
    };

    class Delivery_Level : public PlayerScript
    {
        private:
            ACE_Based::LockedMap<uint32, QueryResultFuture> m_QueryResultFutures;

        public:
            Delivery_Level() : PlayerScript("WebshopDelivery_Level")
            {
            }

            void OnLogin(Player* p_Player) override
            {
                uint32 l_LowGuid = p_Player->GetGUIDLow();
                QueryResultFuture& l_Callback = m_QueryResultFutures[p_Player->GetGUIDLow()];
                l_Callback = CharacterDatabase.AsyncPQuery("SELECT level, transaction FROM webshop_delivery_level WHERE guid = '%u' and delivery = 0", l_LowGuid);
            }

            void OnUpdate(Player* p_Player, uint32 /*p_Diff*/) override
            {
                uint32 l_LowGuid = p_Player->GetGUIDLow();

                auto l_Itr = m_QueryResultFutures.find(l_LowGuid);
                if (l_Itr == m_QueryResultFutures.end())
                    return;

                QueryResultFuture& l_Callback = l_Itr->second;
                if (!l_Callback.ready())
                    return;

                QueryResult l_Result;
                l_Callback.get(l_Result);

                if (!l_Result)
                {
                    l_Callback.cancel();
                    m_QueryResultFutures.erase(l_LowGuid);
                    p_Player->SetStoreDeliveryProccesed(StoreCallback::LevelDelivery);
                    return;
                }

                do
                {
                    Field* l_Field       = l_Result->Fetch();
                    uint32 l_Level       = l_Field[0].GetUInt32();
                    uint32 l_Transaction = l_Field[1].GetUInt32();

                    p_Player->GiveLevel(l_Level);

                    CharacterDatabase.PExecute("UPDATE webshop_delivery_level SET delivery = 1 WHERE transaction = %u", l_Transaction);
                } 
                while (l_Result->NextRow());

                l_Callback.cancel();
                m_QueryResultFutures.erase(l_LowGuid);
                p_Player->SetStoreDeliveryProccesed(StoreCallback::LevelDelivery);
            }
    };

    class Delivery_Profession : public PlayerScript
    {
        private:
            ACE_Based::LockedMap<uint32, QueryResultFuture> m_QueryResultFutures;

            const uint32 m_SkillValue   = 700;
            const uint32 m_LevelRequire = 90;

        public:
            Delivery_Profession() : PlayerScript("WebshopDelivery_Profession")
            {
            }

            void OnLogin(Player* p_Player) override
            {
                uint32 l_LowGuid = p_Player->GetGUIDLow();
                QueryResultFuture& l_Callback = m_QueryResultFutures[p_Player->GetGUIDLow()];
                l_Callback = CharacterDatabase.AsyncPQuery("SELECT skill, recipe, transaction FROM webshop_delivery_profession WHERE guid = '%u' and delivery = 0", l_LowGuid);
            }

            void OnUpdate(Player* p_Player, uint32 /*p_Diff*/) override
            {
                uint32 l_LowGuid = p_Player->GetGUIDLow();

                auto l_Itr = m_QueryResultFutures.find(l_LowGuid);
                if (l_Itr == m_QueryResultFutures.end())
                    return;

                QueryResultFuture& l_Callback = l_Itr->second;
                if (!l_Callback.ready())
                    return;

                QueryResult l_Result;
                l_Callback.get(l_Result);

                if (!l_Result)
                {
                    l_Callback.cancel();
                    m_QueryResultFutures.erase(l_LowGuid);
                    p_Player->SetStoreDeliveryProccesed(StoreCallback::ProfessionDelivery);
                    return;
                }

                const std::map<uint32, uint32> l_SkillLearningSpells =
                {
                    { SkillType::SKILL_ALCHEMY, ProfessionBookSpells::Alchemy },
                    { SkillType::SKILL_BLACKSMITHING, ProfessionBookSpells::Blacksmithing },
                    { SkillType::SKILL_ENCHANTING, ProfessionBookSpells::Enchanting },
                    { SkillType::SKILL_ENGINEERING, ProfessionBookSpells::Engineering },
                    { SkillType::SKILL_INSCRIPTION, ProfessionBookSpells::Inscription },
                    { SkillType::SKILL_JEWELCRAFTING, ProfessionBookSpells::JewelCrafting },
                    { SkillType::SKILL_LEATHERWORKING, ProfessionBookSpells::LeatherWorking },
                    { SkillType::SKILL_TAILORING, ProfessionBookSpells::Tailoring },
                    { SkillType::SKILL_FIRST_AID, ProfessionBookSpells::FirstAid },
                    { SkillType::SKILL_COOKING, ProfessionBookSpells::Cooking },
                    { SkillType::SKILL_HERBALISM, ProfessionBookSpells::Herbalism },
                    { SkillType::SKILL_MINING, ProfessionBookSpells::Mining },
                    { SkillType::SKILL_SKINNING, ProfessionBookSpells::Skinning },
                    { SkillType::SKILL_ARCHAEOLOGY, ProfessionBookSpells::Archaeology },
                    { SkillType::SKILL_FISHING, ProfessionBookSpells::Fishing }
                };

                do
                {
                    Field* l_Fields      = l_Result->Fetch();
                    uint32 l_SkillID     = l_Fields[0].GetUInt32();
                    bool   l_Recipe      = l_Fields[1].GetBool();
                    uint32 l_Transaction = l_Fields[2].GetUInt32();

                    auto l_Itr = l_SkillLearningSpells.find(l_SkillID);
                    if (l_Itr == l_SkillLearningSpells.end())
                        continue;

                    uint32 l_SpellID = l_Itr->second;

                    if (p_Player->getLevel() < m_LevelRequire)
                        continue;

                    if (IsPrimaryProfessionSkill(l_SkillID) && !p_Player->HasSkill(l_SkillID) && p_Player->GetFreePrimaryProfessionPoints() == 0)
                        continue;

                    /// Learn the skill to dreanor rank
                    p_Player->CastSpell(p_Player, l_SpellID, true);

                    /// Up skill to 700
                    p_Player->SetSkill(l_SkillID, p_Player->GetSkillStep(l_SkillID), m_SkillValue, m_SkillValue);

                    if (l_Recipe)
                    {
                        const std::list<SkillLineAbilityEntry const*>& l_Abilities = sSpellMgr->GetTradeSpellFromSkill(l_SkillID);
                        for (auto l_Abilitie : l_Abilities)
                        {
                            if (l_Abilitie->spellId > 155748)   ///< last 5.4.7 spellid
                                continue;

                            p_Player->learnSpell(l_Abilitie->spellId, false);
                        }
                    }

                    if (l_SkillID == SkillType::SKILL_JEWELCRAFTING)
                        p_Player->learnSpell(ProfessionAdditionalSpells::Prospecting, false);             ///< Prospecting

                    /// We also need to learn it for herbalism
                    if (l_SkillID == SkillType::SKILL_HERBALISM || l_SkillID == SkillType::SKILL_MINING)
                        p_Player->learnSpell(l_SpellID, false);

                    CharacterDatabase.PExecute("UPDATE webshop_delivery_profession SET delivery = 1 WHERE transaction = %u", l_Transaction);
                } 
                while (l_Result->NextRow());

                l_Callback.cancel();
                m_QueryResultFutures.erase(l_LowGuid);
                p_Player->SetStoreDeliveryProccesed(StoreCallback::ProfessionDelivery);
            }
    };

    class Delivery_Save : public PlayerScript
    {
        public:
            Delivery_Save() : PlayerScript("WebshopDelivery_Save")
            {
            }

            void OnUpdate(Player* p_Player, uint32 /*p_Diff*/) override
            {
                if (p_Player->IsStoreDeliverySaved())
                    return;

                for (uint8 l_I = 0; l_I < StoreCallback::MaxDelivery; l_I++)
                {
                    if (!p_Player->IsStoreDeliveryProccesed((StoreCallback)l_I))
                        return;
                }

                p_Player->SaveToDB();
                p_Player->SetStoreDeliverySaved();
            }
    };
}

#ifndef __clang_analyzer__
void AddSC_Webshop_Delivery()
{
    new WebShop::Delivery_Item();
    new WebShop::Delivery_Gold();
    new WebShop::Delivery_Currency();
    new WebShop::Delivery_Level();
    new WebShop::Delivery_Profession();
    new WebShop::Delivery_Save();
};
#endif