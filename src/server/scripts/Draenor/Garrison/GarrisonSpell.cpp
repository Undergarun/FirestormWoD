////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "GarrisonSpell.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    /// Constructor
    spell_garrison_hearthstone::spell_garrison_hearthstone()
        : SpellScriptLoader("spell_garrison_hearthstone")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Function called on server startup, if returns false script won't be used in core
    /// use for: dbc/template data presence/correctness checks
    bool spell_garrison_hearthstone::spell_garrison_hearthstone_SpellScript::Validate(SpellInfo const* p_SpellEntry)
    {
        if (!sSpellMgr->GetSpellInfo(Spells::SPELL_GARRISON_HEARTHSTONE))
            return false;

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On spell check cast
    SpellCastResult spell_garrison_hearthstone::spell_garrison_hearthstone_SpellScript::HandleCheckCast()
    {
        Player * l_Player = GetCaster()->ToPlayer();

        if (GetCaster()->isInCombat() && l_Player)
        {
            l_Player->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, GetSpell()->m_CastItem, NULL);
            return SPELL_FAILED_CUSTOM_ERROR;
        }

        if (l_Player && !l_Player->GetGarrison())
            return SPELL_FAILED_GARRISON_NOT_OWNED;

        return SPELL_CAST_OK;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On spell EFFECT_0 dummy proc
    /// @p_EffectIndex : Effect index
    void spell_garrison_hearthstone::spell_garrison_hearthstone_SpellScript::HandleDummy(SpellEffIndex p_EffectIndex)
    {
        Player * l_Player = GetCaster()->ToPlayer();

        if (l_Player && l_Player->GetGarrison() && l_Player->GetGarrison()->GetGarrisonSiteLevelEntry())
        {
            Manager * l_Garrison = l_Player->GetGarrison();

            uint32 l_Index = (l_Garrison->GetGarrisonFactionIndex() * GARRISON_MAX_LEVEL) + (l_Garrison->GetGarrisonSiteLevelEntry()->Level - 1);

            float l_X = gGarrisonHearthStoneTeleportCoord[l_Index][0];
            float l_Y = gGarrisonHearthStoneTeleportCoord[l_Index][1];
            float l_Z = gGarrisonHearthStoneTeleportCoord[l_Index][2];
            float l_O = gGarrisonHearthStoneTeleportCoord[l_Index][3];

            l_Player->TeleportTo(l_Garrison->GetGarrisonSiteLevelEntry()->MapID, l_X, l_Y, l_Z, l_O);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// SpellScript/AuraScript interface base
    /// these functions are safe to override, see notes below for usage instructions
    ///
    /// Function in which handler functions are registered, must be implemented in script
    void spell_garrison_hearthstone::spell_garrison_hearthstone_SpellScript::Register()
    {
        OnCheckCast += SpellCheckCastFn(spell_garrison_hearthstone_SpellScript::HandleCheckCast);
        OnEffectHit += SpellEffectFn(spell_garrison_hearthstone_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Should return a fully valid SpellScript pointer.
    SpellScript * spell_garrison_hearthstone::GetSpellScript() const
    {
        return new spell_garrison_hearthstone_SpellScript();
    }

}   ///< namespace Garrison
}   ///< namespace MS

void AddSC_Garrison()
{
    new MS::Garrison::spell_garrison_hearthstone();
}