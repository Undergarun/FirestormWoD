////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_QUEST_HPP_GARRISON
#define GARRISON_QUEST_HPP_GARRISON

#include "GarrisonScriptData.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    /// Misc script for some quest
    class GarrisonQuestPlayerScript : PlayerScript
    {
        public:
            /// Constructor
            GarrisonQuestPlayerScript();

            /// Called when a player loot an item
            /// @p_Player : Player instance
            /// @p_Item   : New looted item instance
            virtual void OnItemLooted(Player* p_Player, Item* p_Item) override;

    };

    /// Learning - 161384
    class spell_learning_blueprint: public SpellScriptLoader
    {
        public:
            /// Constructor
            spell_learning_blueprint();

            class spell_learning_blueprint_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_learning_blueprint_SpellScript);

                /// After cast
                void HandleAfterCast();

                /// SpellScript/AuraScript interface base
                /// these functions are safe to override, see notes below for usage instructions
                ///
                /// Function in which handler functions are registered, must be implemented in script
                void Register();

            };

            /// Should return a fully valid SpellScript pointer.
            SpellScript* GetSpellScript() const;

    };

    class playerScript_Garrison_Portals_Phases : public PlayerScript
    {
        public:
            playerScript_Garrison_Portals_Phases() : PlayerScript("playerScript_Garrison_Portals_Phases") { }

            void UpdateGarrisonPhaseMask(Player* p_Player);

            void UpdateDraenorPhaseMask(Player* p_Player);

            void OnUpdateZone(Player* p_Player, uint32 p_NewZoneId, uint32 p_OldZoneID, uint32 p_NewAreaId) override;

            void OnLogin(Player* p_Player) override;

            void OnQuestCleared(Player* p_Player, Quest const* p_Quest) override;

            void OnQuestReward(Player* p_Player, const Quest* p_Quest) override;
    };

    class playerScript_Garrison_Quests_Phases : public PlayerScript
    {
        public:
            playerScript_Garrison_Quests_Phases() : PlayerScript("playerScript_Garrison_Quests_Phases") { }

            void OnItemDestroyed(Player* p_Player, Item* p_Item) override;

            void OnQuestAccept(Player* p_Player, const Quest* p_Quest) override;

            void OnQuestReward(Player* p_Player, const Quest* p_Quest) override;

            void OnQuestAbandon(Player* p_Player, const Quest* p_Quest) override;
    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_QUEST_HPP_GARRISON
