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
        Player* l_Player = GetCaster()->ToPlayer();

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
            Manager* l_Garrison = l_Player->GetGarrison();

            uint32 l_Index = (l_Garrison->GetGarrisonFactionIndex() * MS::Garrison::Globals::MaxLevel) + (l_Garrison->GetGarrisonSiteLevelEntry()->Level - 1);

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
    SpellScript* spell_garrison_hearthstone::GetSpellScript() const
    {
        return new spell_garrison_hearthstone_SpellScript();
    }

    /// Constructor
    spell_garrison_portal::spell_garrison_portal()
        : SpellScriptLoader("spell_garrison_portal")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Function called on server startup, if returns false script won't be used in core
    /// use for: dbc/template data presence/correctness checks
    bool spell_garrison_portal::spell_garrison_portal_SpellScript::Validate(SpellInfo const* p_SpellEntry)
    {
        if (!sSpellMgr->GetSpellInfo(Spells::SpellGarrisonPortal))
            return false;

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On spell check cast
    SpellCastResult spell_garrison_portal::spell_garrison_portal_SpellScript::HandleCheckCast()
    {
        Player* l_Player = GetCaster()->ToPlayer();

        if (l_Player && !l_Player->GetGarrison())
            return SPELL_FAILED_GARRISON_NOT_OWNED;

        return SPELL_CAST_OK;
    }

    /// Constructor
    spell_garrison_shipyard::spell_garrison_shipyard() : SpellScriptLoader("spell_garrison_shipyard")
    {
    }

    SpellCastResult spell_garrison_shipyard::spell_garrison_shipyard_SpellScript::HandleCheckCast()
    {
        Player * l_Player = GetCaster()->ToPlayer();

        if (!l_Player || l_Player->isInCombat())
        {
            l_Player->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, GetSpell()->m_CastItem, NULL);
            return SPELL_FAILED_CUSTOM_ERROR;
        }

        Garrison::Manager* l_Garrison = l_Player->GetGarrison();

        if (!l_Player->GetGarrison())
            return SPELL_FAILED_GARRISON_NOT_OWNED;

        if (l_Garrison->HasShipyard())
            return SPELL_FAILED_GARRISON_NOT_OWNED;

        auto l_SiteLevelEntry = l_Garrison->GetGarrisonSiteLevelEntry();

        if (!l_SiteLevelEntry || l_SiteLevelEntry->Level != 3)
            return SPELL_FAILED_GARRISON_NOT_OWNED;

        return SPELL_CAST_OK;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On spell EFFECT_0 dummy proc
    /// @p_EffectIndex : Effect index
    void spell_garrison_portal::spell_garrison_portal_SpellScript::HandleDummy(SpellEffIndex p_EffectIndex)
    {
        if (!GetCaster())
            return;

        if (Player* l_Player = GetCaster()->ToPlayer())
        {
            if (MS::Garrison::Manager* l_GarrisonMgr = l_Player->GetGarrison())
            {
                switch (l_GarrisonMgr->GetGarrisonLevel())
                {
                    case 1:
                    {
                        if (l_Player->GetTeamId() == TEAM_ALLIANCE)
                            l_Player->TeleportTo(1158, 1847.2951f, 263.9954f, 78.3144f, 5.148287f); ///< Alliance Garrison Lvl 1
                        else
                            l_Player->TeleportTo(1152, 5561.7416f, 4509.7124f, 132.6979f, 3.5397f); ///< Horde Garrison Lvl 1
                        break;
                    }
                    case 2:
                    {
                        if (l_Player->GetTeamId() == TEAM_ALLIANCE)
                            l_Player->TeleportTo(1331, 1945.741f, 319.8971f, 90.4052f, 1.1545f); ///< Alliance Garrison Lvl 2
                        else
                            l_Player->TeleportTo(1330, 5568.2348f, 4590.6333f, 140.9079f, 2.2988f); ///< Horde Garrison Lvl 2
                        break;
                   }
                   case 3: ///< Garrison lvl 3 not released yet
                   default:
                        break;
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// SpellScript/AuraScript interface base
    /// these functions are safe to override, see notes below for usage instructions
    ///
    /// Function in which handler functions are registered, must be implemented in script
    void spell_garrison_portal::spell_garrison_portal_SpellScript::Register()
    {
        OnCheckCast += SpellCheckCastFn(spell_garrison_portal_SpellScript::HandleCheckCast);
        OnEffectHit += SpellEffectFn(spell_garrison_portal_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Should return a fully valid SpellScript pointer.
    SpellScript* spell_garrison_portal::GetSpellScript() const
    {
        return new spell_garrison_portal_SpellScript();
    }

    void spell_garrison_shipyard::spell_garrison_shipyard_SpellScript::HandleDummy(SpellEffIndex p_EffectIndex)
    {
        Player * l_Player = GetCaster()->ToPlayer();

        if (!l_Player)
            return;

        Garrison::Manager* l_Garrison = l_Player->GetGarrison();

        if (!l_Garrison)
            return;

        l_Garrison->CreateShipyardBySpell();
    }

    void spell_garrison_shipyard::spell_garrison_shipyard_SpellScript::Register()
    {
        OnCheckCast += SpellCheckCastFn(spell_garrison_shipyard_SpellScript::HandleCheckCast);
        OnEffectHit += SpellEffectFn(spell_garrison_shipyard_SpellScript::HandleDummy, EFFECT_2, SPELL_EFFECT_DUMMY);
    }

    /// Should return a fully valid SpellScript pointer.
    SpellScript * spell_garrison_shipyard::GetSpellScript() const
    {
        return new spell_garrison_shipyard_SpellScript();
    }

    /// Stables Lassos - 173686
    class spell_garrison_stables_lasso : public SpellScriptLoader
    {
        public:
            uint32 g_LassoBreakSpellID;
            spell_garrison_stables_lasso() : SpellScriptLoader("spell_garrison_stables_lasso") { g_LassoBreakSpellID = 173702; }

            class spell_garrison_stables_lasso_AuraScript : public AuraScript
            {
                PrepareAuraScript(spell_garrison_stables_lasso_AuraScript);


                void OnTick(constAuraEffectPtr p_AurEff)
                {
                    Unit* l_Caster = GetCaster();
                    WorldObject* l_Target = GetOwner();

                    if (l_Caster == nullptr || l_Target == nullptr)
                        return;

                    if (l_Caster->GetDistance2d(l_Target) >= 15.0f)
                    {
                        if (Creature* l_Creature = l_Target->ToCreature())
                            l_Creature->CastSpell(l_Creature, 173702, true);
                    }
                }

                void OnRemove(constAuraEffectPtr p_AurEff, AuraEffectHandleModes p_Mode)
                {
                    WorldObject* l_Target = GetOwner();

                    if (l_Target == nullptr)
                        return;

                    if (Creature* l_Creature = l_Target->ToCreature())
                        l_Creature->DespawnOrUnsummon();
                }

                void Register() override
                {
                    OnEffectPeriodic += AuraEffectPeriodicFn(spell_garrison_stables_lasso_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
                    OnEffectRemove += AuraEffectRemoveFn(spell_garrison_stables_lasso_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
                }
            };

            AuraScript* GetAuraScript() const
            {
                return new spell_garrison_stables_lasso_AuraScript();
            }
    };

}   ///< namespace Garrison
}   ///< namespace MS

void AddSC_Garrison()
{
    new MS::Garrison::spell_garrison_stables_lasso();
    new MS::Garrison::spell_garrison_hearthstone();
    new MS::Garrison::spell_garrison_portal();
    new MS::Garrison::spell_garrison_shipyard();
}