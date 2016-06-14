////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "stormstout_brewery.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"

enum eHabaneroBeer
{
    NPC_BARREL              = 56731,

    SPELL_PROC_EXPLOSION    = 106787
};

class spell_stormstout_brewery_habanero_beer: public SpellScriptLoader
{
    public:
        spell_stormstout_brewery_habanero_beer() : SpellScriptLoader("spell_stormstout_brewery_habanero_beer") { }

        class spell_stormstout_brewery_habanero_beer_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_stormstout_brewery_habanero_beer_SpellScript);

            void HandleInstaKill(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster())
                    return;

                std::list<Creature*> creatureList;

                GetCreatureListWithEntryInGrid(creatureList, GetCaster(), NPC_BARREL, 10.0f);

                GetCaster()->RemoveAurasDueToSpell(SPELL_PROC_EXPLOSION);

                for (auto barrel : creatureList)
                {
                    if (barrel->HasAura(SPELL_PROC_EXPLOSION))
                    {
                        barrel->RemoveAurasDueToSpell(SPELL_PROC_EXPLOSION);
                        barrel->CastSpell(barrel, GetSpellInfo()->Id, true);
                    }
                }
            }

            void HandleAfterCast()
            {
                if (Unit* caster = GetCaster())
                    if (caster->ToCreature())
                        caster->ToCreature()->ForcedDespawn(1000);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_stormstout_brewery_habanero_beer_SpellScript::HandleInstaKill, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
                AfterCast += SpellCastFn(spell_stormstout_brewery_habanero_beer_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_stormstout_brewery_habanero_beer_SpellScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_stormstout_brewery()
{
    new spell_stormstout_brewery_habanero_beer();
}
#endif
