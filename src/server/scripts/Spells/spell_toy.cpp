////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"
#include "ScriptedCreature.h"

/// Stonebound - 129755
class spell_toy_stonebound : public SpellScriptLoader
{
    public:
        spell_toy_stonebound() : SpellScriptLoader("spell_toy_stonebound") { }

        class spell_toy_stonebound_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_toy_stonebound_AuraScript);

            uint32 m_Update;

            bool Load()
            {
                m_Update = 500;
                return true;
            }

            void OnUpdate(uint32 p_Diff)
            {
                if (m_Update <= p_Diff)
                {
                    m_Update = 500;

                    if (Unit* l_Target = GetUnitOwner())
                    {
                        bool l_Remove = false;
                        if (l_Target->isAlive())
                            l_Remove = true;

                        if (l_Target->GetTypeId() == TypeID::TYPEID_PLAYER)
                        {
                            if (l_Target->ToPlayer()->HasFlag(EPlayerFields::PLAYER_FIELD_PLAYER_FLAGS, PlayerFlags::PLAYER_FLAGS_GHOST))
                                l_Remove = true;
                        }

                        if (l_Remove)
                            Remove();
                    }
                }
                else
                    m_Update -= p_Diff;
            }

            void Register() override
            {
                OnAuraUpdate += AuraUpdateFn(spell_toy_stonebound_AuraScript::OnUpdate);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_toy_stonebound_AuraScript();
        }
};

void AddSC_spell_toy()
{
    new spell_toy_stonebound();
}