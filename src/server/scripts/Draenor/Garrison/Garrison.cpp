#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "Garrison.h"

enum
{
    SPELL_GARRISON_HEARTHSTONE = 171253
};

class PlayerScript_Garrison: public MS::Game::Scripting::Interfaces::PlayerScript
{
    public:
        PlayerScript_Garrison()
            : PlayerScript("PlayerScript_Garrison")
        {

        }

        /// Called when player rewards some quest
        virtual void OnQuestReward(Player* p_Player, const Quest* p_Quest) override
        {
            if (p_Player && p_Player->IsInGarrison() && p_Quest)
                p_Player->GetGarrison()->OnQuestReward(p_Quest);
        }

};

/// Garrison hearthstone teleport coords
float gGarrisonHearthStoneTeleportCoord[][4] =
{
    /// Horde
    { 5561.15f,  4508.785f, 132.70f,    3.406f   }, ///< Horde Level 1
    {    0.f,       0.f,      0.f,      0.000f   }, ///< Horde Level 2
    { 5563.70f,  4599.920f, 141.71100f, 0.000f   }, ///< Horde Level 3
    /// Alliance
    { 1850.71f,   254.430f,  78.08300f, 1.76f    }, ///< Alliance Level 1
    { 1947.08f,   324.883f,  90.28076f, 0.00f    }, ///< Alliance Level 2
    {    0.f,       0.f,      0.f,      0.00f    }, ///< Alliance Level 3
};

class spell_garrison_hearthstone: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        /// Constructor
        spell_garrison_hearthstone() 
            : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_garrison_hearthstone")
        { 

        }

        /// Spell script
        class spell_garrison_hearthstone_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_garrison_hearthstone_SpellScript);

            /// Function called on server startup, if returns false script won't be used in core
            /// use for: dbc/template data presence/correctness checks
            bool Validate(SpellInfo const* p_SpellEntry) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_GARRISON_HEARTHSTONE))
                    return false;

                return true;
            }

            /// On spell check cast
            SpellCastResult HandleCheckCast()
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

            /// On spell EFFECT_0 dummy proc
            void HandleDummy(SpellEffIndex p_EffectIndex)
            {
                Player  * l_Player = GetCaster()->ToPlayer();

                if (l_Player && l_Player->GetGarrison())
                {
                    Garrison * l_Garrison = l_Player->GetGarrison();

                    uint32 l_Index = (l_Garrison->GetGarrisonFactionIndex() * GARRISON_MAX_LEVEL) + (l_Garrison->GetGarrisonSiteLevelEntry()->Level - 1);

                    float l_X = gGarrisonHearthStoneTeleportCoord[l_Index][0];
                    float l_Y = gGarrisonHearthStoneTeleportCoord[l_Index][1];
                    float l_Z = gGarrisonHearthStoneTeleportCoord[l_Index][2];
                    float l_O = gGarrisonHearthStoneTeleportCoord[l_Index][3];

                    l_Player->TeleportTo(l_Garrison->GetGarrisonSiteLevelEntry()->MapID, l_X, l_Y, l_Z, l_O);
                }
            }

            /// SpellScript/AuraScript interface base
            /// these functions are safe to override, see notes below for usage instructions
            ///
            /// Function in which handler functions are registered, must be implemented in script
            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_garrison_hearthstone_SpellScript::HandleCheckCast);
                OnEffectHit += SpellEffectFn(spell_garrison_hearthstone_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        /// Should return a fully valid SpellScript pointer.
        SpellScript * GetSpellScript() const
        {
            return new spell_garrison_hearthstone_SpellScript();
        }

};


void AddSC_Garrison()
{
    new PlayerScript_Garrison();
    new spell_garrison_hearthstone();
}