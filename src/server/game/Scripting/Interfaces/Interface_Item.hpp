/// Copyright Ashran 2014-2015

#ifndef SCRIPTING_INTERFACES_ITEM
#define SCRIPTING_INTERFACES_ITEM

#include "InterfaceBase.hpp"

namespace MS { namespace Game { namespace Scripting { namespace Interfaces 
{

    /// Item script interface
    class ItemScript : public ScriptObjectImpl<true>
    {
        protected:
            /// Constructor
            /// @p_Name : Script name
            ItemScript(const char * p_Name);

        public:
            /// Called when a dummy spell effect is triggered on the item.
            /// @p_Cast        : Caster unit instance
            /// @p_SpellID     : Dummy effect origin spell ID
            /// @p_EffectIndex : Dummy effect index
            /// @p_Target      : Spell target
            virtual bool OnDummyEffect(Unit* p_Caster, uint32 p_SpellID, SpellEffIndex p_EffectIndex, Item* p_Target)
            {
                UNUSED(p_Caster);
                UNUSED(p_SpellID);
                UNUSED(p_EffectIndex);
                UNUSED(p_Target);

                return false;
            }

            /// Called when a player accepts a quest from the item.
            /// @p_Player : Player who accepted quest from this item
            /// @p_Item   : Item quest owner instance
            /// @p_Quest  : Accepted quest instance
            virtual bool OnQuestAccept(Player * p_Player, Item * p_Item, const Quest * p_Quest)
            {
                UNUSED(p_Player);
                UNUSED(p_Item);
                UNUSED(p_Quest);

                return false; 
            }

            /// Called when a player uses the item.
            /// @p_Player           : Player who use this item
            /// @p_Item             : Used Item instance
            /// @p_SpellCastTargets : Item spell action targets
            virtual bool OnUse(Player* p_Player, Item * p_Item, const SpellCastTargets & p_SpellCastTargets)
            {
                UNUSED(p_Player);
                UNUSED(p_Item);
                //UNUSED(p_SpellCastTargets);

                return false;
            }

            /// Called when the item expires (is destroyed).
            /// @p_Player       : Item destroyer player instance
            /// @p_ItemTemplate : Destroyed item template
            virtual bool OnExpire(Player* p_Player, const ItemTemplate * p_ItemTemplate)
            {
                UNUSED(p_Player);
                UNUSED(p_ItemTemplate);

                return false;
            }

    };

}   ///< Namespace Interfaces
}   ///< Namespace Scripting
}   ///< Namespace Game
}   ///< Namespace MS

#endif  ///< SCRIPTING_INTERFACES_ITEM