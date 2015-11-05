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
        if (p_Player->HasQuest(Quests::QUEST_BUILD_YOUR_BARRACKS) && p_Item && p_Item->GetEntry() == Items::ITEM_GARRISON_BLUEPRINT_BARRACKS_LEVEL1)
            p_Player->QuestObjectiveSatisfy(39015, 1, QUEST_OBJECTIVE_TYPE_CRITERIA_TREE);
        else if (p_Item->GetVisibleEntry() == 114677)
        {
            uint64 l_PlayerGuid = p_Player->GetGUID();
            uint64 l_ItemGuid = p_Item->GetGUID();
            p_Player->ModifyCurrency(824, 30);

            p_Player->AddCriticalOperation([l_PlayerGuid, l_ItemGuid]() -> void
            {
                if (Player* l_Player = sObjectAccessor->FindPlayer(l_PlayerGuid))
                {
                    uint32 l_DestroyCount = 1;

                    if (Item* l_Item = l_Player->GetItemByGuid(l_ItemGuid))
                        l_Player->DestroyItemCount(l_Item, l_DestroyCount, true);
                }
            });
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