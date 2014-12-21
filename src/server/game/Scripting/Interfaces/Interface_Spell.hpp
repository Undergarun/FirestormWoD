/// Copyright Ashran 2014-2015

#ifndef SCRIPTING_INTERFACES_SPELL
#define SCRIPTING_INTERFACES_SPELL

#include "InterfaceBase.hpp"

/// Spell script interface
class SpellScriptLoader : public ScriptObjectImpl<true>
{
    protected:
        /// Constructor
        /// @p_Name : Script name
        SpellScriptLoader(const char * p_Name);

    public:
        /// Should return a fully valid SpellScript pointer.
        virtual SpellScript* GetSpellScript() const
        {
            return nullptr;
        }
        /// Should return a fully valid AuraScript pointer.
        virtual AuraScript* GetAuraScript() const
        {
            return nullptr;
        }

};

#endif  ///< SCRIPTING_INTERFACES_SPELL