////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_SPELL_HPP_GARRISON
#define GARRISON_SPELL_HPP_GARRISON

#include "GarrisonScriptData.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    /// Garrison Hearthstone item spell script
    class spell_garrison_hearthstone : public SpellScriptLoader
    {
        public:
            /// Constructor
            spell_garrison_hearthstone();

            /// Spell script
            class spell_garrison_hearthstone_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_garrison_hearthstone_SpellScript)

                /// Function called on server startup, if returns false script won't be used in core
                /// use for: dbc/template data presence/correctness checks
                bool Validate(SpellInfo const* p_SpellEntry) override;

                /// On spell check cast
                SpellCastResult HandleCheckCast();

                /// On spell EFFECT_0 dummy proc
                /// @p_EffectIndex : Effect index
                void HandleDummy(SpellEffIndex p_EffectIndex);

                /// SpellScript/AuraScript interface base
                /// these functions are safe to override, see notes below for usage instructions
                ///
                /// Function in which handler functions are registered, must be implemented in script
                void Register() override;
            };

            /// Should return a fully valid SpellScript pointer.
            SpellScript * GetSpellScript() const override;

    };
    
    /// Garrison Hearthstone item spell script
    class spell_garrison_portal : public SpellScriptLoader
    {
        public:
            /// Constructor
            spell_garrison_portal();

            /// Spell script
            class spell_garrison_portal_SpellScript : public SpellScript
            {
                PrepareSpellScript(spell_garrison_portal_SpellScript)

                /// Function called on server startup, if returns false script won't be used in core
                /// use for: dbc/template data presence/correctness checks
                bool Validate(SpellInfo const* p_SpellEntry) override;

                /// On spell check cast
                SpellCastResult HandleCheckCast();

                /// On spell EFFECT_0 dummy proc
                /// @p_EffectIndex : Effect index
                void HandleDummy(SpellEffIndex p_EffectIndex);

                /// SpellScript/AuraScript interface base
                /// these functions are safe to override, see notes below for usage instructions
                ///
                /// Function in which handler functions are registered, must be implemented in script
                void Register() override;
            };

            /// Should return a fully valid SpellScript pointer.
            SpellScript * GetSpellScript() const override;

    };

    /// Garrison Hearthstone item spell script
    class spell_garrison_shipyard : public SpellScriptLoader
    {
    public:
        /// Constructor
        spell_garrison_shipyard();

        /// Spell script
        class spell_garrison_shipyard_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_garrison_shipyard_SpellScript)

            /// On spell check cast
            SpellCastResult HandleCheckCast();

            /// On spell EFFECT_0 dummy proc
            /// @p_EffectIndex : Effect index
            void HandleDummy(SpellEffIndex p_EffectIndex);

            /// SpellScript/AuraScript interface base
            /// these functions are safe to override, see notes below for usage instructions
            ///
            /// Function in which handler functions are registered, must be implemented in script
            void Register() override;
        };

        /// Should return a fully valid SpellScript pointer.
        SpellScript * GetSpellScript() const override;

    };
}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_SPELL_HPP_GARRISON
