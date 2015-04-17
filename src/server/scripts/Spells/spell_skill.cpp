////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"
#include "ScriptedCreature.h"

namespace MS { namespace Skill
{

    namespace SpellIDs
    {
        enum 
        {
            SaberfishBroth   = 161001,
            GrilledSaberfish = 161002
        };
    }

    namespace ItemIDs
    {
        enum
        {
            /// Cooking rewards for SpellIDs::SaberfishBroth && SpellIDs::GrilledSaberfish
            BlackrockHam       = 118311,
            ClefthoofSausages  = 118315,
            FatSleeperCakes    = 118319,
            FieryCalamari      = 118320,
            GrilledGulper      = 118317,
            PanSearedTalbuk    = 118312,
            RylakCrepes        = 118314,
            SkulkerChowder     = 118321,
            SteamedScorpion    = 118316,
            SturgeonStew       = 118318
        };
    }

    //////////////////////////////////////////////////////////////////////////
    /// Generic spell script Draenor cooking rewards
    //////////////////////////////////////////////////////////////////////////
    class spell_Cooking_DraenorRecipesRewards : public SpellScriptLoader
    {
        public:
            /// Constructor
            spell_Cooking_DraenorRecipesRewards()
                : SpellScriptLoader("spell_Cooking_DraenorRecipesRewards")
            {

            }

            class spell_Cooking_DraenorRecipesRewards_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_Cooking_DraenorRecipesRewards_SpellScript);

                void HandleAfterCast()
                {
                    Player * l_Player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;

                    if (!l_Player)
                        return;

                    uint32 l_SpellID = GetSpellInfo()->Id;

                    std::vector<uint32> l_RewardItems;

                    if (l_SpellID == SpellIDs::SaberfishBroth || l_SpellID == SpellIDs::GrilledSaberfish)
                    {
                        l_RewardItems.push_back(ItemIDs::BlackrockHam);
                        l_RewardItems.push_back(ItemIDs::ClefthoofSausages);
                        l_RewardItems.push_back(ItemIDs::FatSleeperCakes);
                        l_RewardItems.push_back(ItemIDs::FieryCalamari);
                        l_RewardItems.push_back(ItemIDs::GrilledGulper);
                        l_RewardItems.push_back(ItemIDs::PanSearedTalbuk);
                        l_RewardItems.push_back(ItemIDs::RylakCrepes);
                        l_RewardItems.push_back(ItemIDs::SkulkerChowder);
                        l_RewardItems.push_back(ItemIDs::SteamedScorpion);
                        l_RewardItems.push_back(ItemIDs::SturgeonStew);
                    }

                    /// Can have a reward (Guessed value)
                    if (!roll_chance_i(60))
                        return;

                    uint32 l_RewardChance = 100 / l_RewardItems.size();

                    for (uint32 l_I = 0; l_I < l_RewardItems.size(); ++l_I)
                    {
                        uint32 l_RewardItem = l_RewardItems[l_I];

                        if (roll_chance_i(l_RewardChance))
                        {
                            ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(l_RewardItem);

                            if (!pProto)
                                continue;

                            ItemPosCountVec l_Dest;
                            uint32 l_NoSpace = 0;

                            if (l_Player->CanStoreNewItem(NULL_BAG, NULL_SLOT, l_Dest, l_RewardItem, 1, &l_NoSpace) == EQUIP_ERR_OK)
                            {
                                Item* pItem = l_Player->StoreNewItem(l_Dest, l_RewardItem, true, Item::GenerateItemRandomPropertyId(l_RewardItem));
                                l_Player->SendNewItem(pItem, 1, true, false);
                            }

                            break;
                        }
                    }
                }

                void Register() override
                {
                    AfterCast += SpellCastFn(spell_Cooking_DraenorRecipesRewards_SpellScript::HandleAfterCast);
                }

            };

            /// Should return a fully valid SpellScript pointer.
            SpellScript* GetSpellScript() const override
            {
                return new spell_Cooking_DraenorRecipesRewards_SpellScript();
            }

    };

}   ///< namespace Skill
}   ///< namespace MS


void AddSC_spell_skill()
{
    new MS::Skill::spell_Cooking_DraenorRecipesRewards();
}