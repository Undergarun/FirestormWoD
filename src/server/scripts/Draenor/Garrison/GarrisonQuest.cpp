////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "GarrisonQuest.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonScriptData.hpp"

namespace MS { namespace Garrison 
{
    /// Constructor
    GarrisonQuestPlayerScript::GarrisonQuestPlayerScript()
        : PlayerScript("GarrisonQuestPlayerScript")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player loot an item
    /// @p_Player : Player instance
    /// @p_Item   : New looted item instance
    void GarrisonQuestPlayerScript::OnItemLooted(Player* p_Player, Item* p_Item)
    {
        uint32 l_NoSpaceForCount = 0;
        ItemPosCountVec l_Destination;

        if (p_Player->HasQuest(Quests::QUEST_BUILD_YOUR_BARRACKS) && p_Item && p_Item->GetEntry() == Items::ITEM_GARRISON_BLUEPRINT_BARRACKS_LEVEL1)
        {
            p_Player->QuestObjectiveSatisfy(39015, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE);
            return;
        }

        switch (p_Item->GetVisibleEntry())
        {
            case Items::ItemGarrisonResources:
            {
                uint64 l_PlayerGuid = p_Player->GetGUID();
                uint64 l_ItemGuid = p_Item->GetGUID();
                p_Player->ModifyCurrency(CurrencyTypes::CURRENCY_TYPE_GARRISON_RESSOURCES, 30);

                p_Player->AddCriticalOperation([l_PlayerGuid, l_ItemGuid]() -> void
                {
                    if (Player* l_Player = sObjectAccessor->FindPlayer(l_PlayerGuid))
                    {
                        uint32 l_DestroyCount = 1;

                        if (Item* l_Item = l_Player->GetItemByGuid(l_ItemGuid))
                            l_Player->DestroyItemCount(l_Item, l_DestroyCount, true);
                    }
                });
                break;
            }
            case Items::ItemMine:
            {
                uint64 l_PlayerGuid  = p_Player->GetGUID();
                uint64 l_ItemGuid    = p_Item->GetGUID();
                uint32 l_RewardID    = roll_chance_i(50) ? Items::ItemTrueIronOre : Items::ItemBlackrockOre;
                uint32 l_RewardCount = 5;

                p_Player->AddCriticalOperation([l_PlayerGuid, l_ItemGuid]() -> void
                {
                    if (Player* l_Player = sObjectAccessor->FindPlayer(l_PlayerGuid))
                    {
                        uint32 l_DestroyCount = 1;

                        if (Item* l_Item = l_Player->GetItemByGuid(l_ItemGuid))
                            l_Player->DestroyItemCount(l_Item, l_DestroyCount, true);
                    }
                });

                InventoryResult l_Message = p_Player->CanStoreNewItem(NULL_BAG, NULL_SLOT, l_Destination, l_RewardID, l_RewardCount, &l_NoSpaceForCount);

                if (l_Message == EQUIP_ERR_OK)
                {
                    if (Item* l_Item = p_Player->StoreNewItem(l_Destination, l_RewardID, true, Item::GenerateItemRandomPropertyId(l_RewardID)))
                        p_Player->SendNewItem(l_Item, l_RewardCount, true, false, false);
                }
                else
                    p_Player->SendEquipError(l_Message, nullptr, nullptr, l_RewardID);

                if (roll_chance_i(30))
                    p_Player->ModifyCurrency(CurrencyTypes::CURRENCY_TYPE_APEXIS_CRYSTAL, roll_chance_i(25) ? urand(5, 10) : urand(1, 5));

                break;
            }
            default:
                break;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    spell_learning_blueprint::spell_learning_blueprint()
        : SpellScriptLoader("spell_learning_blueprint")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// After cast
    void spell_learning_blueprint::spell_learning_blueprint_SpellScript::HandleAfterCast()
    {
        if (GetCaster() && GetCaster()->ToPlayer())
        {
            Player * l_Player = GetCaster()->ToPlayer();

            if (l_Player->GetGarrison() && l_Player->HasQuest(Quests::QUEST_BUILD_YOUR_BARRACKS))
                l_Player->QuestObjectiveSatisfy(39012, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE, l_Player->GetGUID());
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// SpellScript/AuraScript interface base
    /// these functions are safe to override, see notes below for usage instructions
    ///
    /// Function in which handler functions are registered, must be implemented in script
    void spell_learning_blueprint::spell_learning_blueprint_SpellScript::Register()
    {
        AfterCast += SpellCastFn(spell_learning_blueprint_SpellScript::HandleAfterCast);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Should return a fully valid SpellScript pointer.
    SpellScript * spell_learning_blueprint::GetSpellScript() const
    {
        return new spell_learning_blueprint_SpellScript();
    }
}   ///< namespace Garrison
}   ///< namespace MS

void AddSC_Garrison_Quest()
{
    new MS::Garrison::GarrisonQuestPlayerScript;
    new MS::Garrison::spell_learning_blueprint;
}