////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/*
 * Scripts for spells with SPELLFAMILY_GENERIC spells used by items.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_item_".
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SkillDiscovery.h"

// Generic script for handling item dummy effects which trigger another spell.
class spell_item_trigger_spell: public SpellScriptLoader
{
    private:
        uint32 _triggeredSpellId;

    public:
        spell_item_trigger_spell(const char* name, uint32 triggeredSpellId) : SpellScriptLoader(name), _triggeredSpellId(triggeredSpellId) { }

        class spell_item_trigger_spell_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_trigger_spell_SpellScript);
        private:
            uint32 _triggeredSpellId;

        public:
            spell_item_trigger_spell_SpellScript(uint32 triggeredSpellId) : SpellScript(), _triggeredSpellId(triggeredSpellId) { }

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(_triggeredSpellId))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Item* item = GetCastItem())
                    caster->CastSpell(caster, _triggeredSpellId, true, item);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_trigger_spell_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_trigger_spell_SpellScript(_triggeredSpellId);
        }
};

// http://www.wowhead.com/item=6522 Deviate Fish
// 8063 Deviate Fish
enum DeviateFishSpells
{
    SPELL_SLEEPY            = 8064,
    SPELL_INVIGORATE        = 8065,
    SPELL_SHRINK            = 8066,
    SPELL_PARTY_TIME        = 8067,
    SPELL_HEALTHY_SPIRIT    = 8068
};

class spell_item_deviate_fish: public SpellScriptLoader
{
    public:
        spell_item_deviate_fish() : SpellScriptLoader("spell_item_deviate_fish") { }

        class spell_item_deviate_fish_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_deviate_fish_SpellScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                for (uint32 spellId = SPELL_SLEEPY; spellId <= SPELL_HEALTHY_SPIRIT; ++spellId)
                    if (!sSpellMgr->GetSpellInfo(spellId))
                        return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                uint32 spellId = urand(SPELL_SLEEPY, SPELL_HEALTHY_SPIRIT);
                caster->CastSpell(caster, spellId, true, NULL);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_deviate_fish_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_deviate_fish_SpellScript();
        }
};

// http://www.wowhead.com/item=47499 Flask of the North
// 67019 Flask of the North
enum FlaskOfTheNorthSpells
{
    SPELL_FLASK_OF_THE_NORTH_SP = 67016,
    SPELL_FLASK_OF_THE_NORTH_AP = 67017,
    SPELL_FLASK_OF_THE_NORTH_STR = 67018
};

class spell_item_flask_of_the_north: public SpellScriptLoader
{
    public:
        spell_item_flask_of_the_north() : SpellScriptLoader("spell_item_flask_of_the_north") { }

        class spell_item_flask_of_the_north_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_flask_of_the_north_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_FLASK_OF_THE_NORTH_SP) || !sSpellMgr->GetSpellInfo(SPELL_FLASK_OF_THE_NORTH_AP) || !sSpellMgr->GetSpellInfo(SPELL_FLASK_OF_THE_NORTH_STR))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                std::vector<uint32> possibleSpells;
                switch (caster->getClass())
                {
                    case CLASS_WARLOCK:
                    case CLASS_MAGE:
                    case CLASS_PRIEST:
                        possibleSpells.push_back(SPELL_FLASK_OF_THE_NORTH_SP);
                        break;
                    case CLASS_DEATH_KNIGHT:
                    case CLASS_WARRIOR:
                        possibleSpells.push_back(SPELL_FLASK_OF_THE_NORTH_STR);
                        break;
                    case CLASS_ROGUE:
                    case CLASS_HUNTER:
                        possibleSpells.push_back(SPELL_FLASK_OF_THE_NORTH_AP);
                        break;
                    case CLASS_DRUID:
                    case CLASS_PALADIN:
                        possibleSpells.push_back(SPELL_FLASK_OF_THE_NORTH_SP);
                        possibleSpells.push_back(SPELL_FLASK_OF_THE_NORTH_STR);
                        break;
                    case CLASS_SHAMAN:
                        possibleSpells.push_back(SPELL_FLASK_OF_THE_NORTH_SP);
                        possibleSpells.push_back(SPELL_FLASK_OF_THE_NORTH_AP);
                        break;
                }

                caster->CastSpell(caster, possibleSpells[irand(0, (possibleSpells.size() - 1))], true, NULL);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_flask_of_the_north_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_flask_of_the_north_SpellScript();
        }
};

// http://www.wowhead.com/item=10645 Gnomish Death Ray
// 13280 Gnomish Death Ray
enum GnomishDeathRay
{
    SPELL_GNOMISH_DEATH_RAY_SELF = 13493,
    SPELL_GNOMISH_DEATH_RAY_TARGET = 13279
};

class spell_item_gnomish_death_ray: public SpellScriptLoader
{
    public:
        spell_item_gnomish_death_ray() : SpellScriptLoader("spell_item_gnomish_death_ray") { }

        class spell_item_gnomish_death_ray_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_gnomish_death_ray_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_GNOMISH_DEATH_RAY_SELF) || !sSpellMgr->GetSpellInfo(SPELL_GNOMISH_DEATH_RAY_TARGET))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Unit* target = GetHitUnit())
                {
                    if (urand(0, 99) < 15)
                        caster->CastSpell(caster, SPELL_GNOMISH_DEATH_RAY_SELF, true, NULL);    // failure
                    else
                        caster->CastSpell(target, SPELL_GNOMISH_DEATH_RAY_TARGET, true, NULL);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_gnomish_death_ray_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_gnomish_death_ray_SpellScript();
        }
};

// http://www.wowhead.com/item=27388 Mr. Pinchy
// 33060 Make a Wish
enum MakeAWish
{
    SPELL_MR_PINCHYS_BLESSING       = 33053,
    SPELL_SUMMON_MIGHTY_MR_PINCHY   = 33057,
    SPELL_SUMMON_FURIOUS_MR_PINCHY  = 33059,
    SPELL_TINY_MAGICAL_CRAWDAD      = 33062,
    SPELL_MR_PINCHYS_GIFT           = 33064
};

class spell_item_make_a_wish: public SpellScriptLoader
{
    public:
        spell_item_make_a_wish() : SpellScriptLoader("spell_item_make_a_wish") { }

        class spell_item_make_a_wish_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_make_a_wish_SpellScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MR_PINCHYS_BLESSING) || !sSpellMgr->GetSpellInfo(SPELL_SUMMON_MIGHTY_MR_PINCHY) || !sSpellMgr->GetSpellInfo(SPELL_SUMMON_FURIOUS_MR_PINCHY) || !sSpellMgr->GetSpellInfo(SPELL_TINY_MAGICAL_CRAWDAD) || !sSpellMgr->GetSpellInfo(SPELL_MR_PINCHYS_GIFT))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                uint32 spellId = SPELL_MR_PINCHYS_GIFT;
                switch (urand(1, 5))
                {
                    case 1: spellId = SPELL_MR_PINCHYS_BLESSING; break;
                    case 2: spellId = SPELL_SUMMON_MIGHTY_MR_PINCHY; break;
                    case 3: spellId = SPELL_SUMMON_FURIOUS_MR_PINCHY; break;
                    case 4: spellId = SPELL_TINY_MAGICAL_CRAWDAD; break;
                }
                caster->CastSpell(caster, spellId, true, NULL);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_make_a_wish_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_make_a_wish_SpellScript();
        }
};

// http://www.wowhead.com/item=32686 Mingo's Fortune Giblets
// 40802 Mingo's Fortune Generator
class spell_item_mingos_fortune_generator: public SpellScriptLoader
{
    public:
        spell_item_mingos_fortune_generator() : SpellScriptLoader("spell_item_mingos_fortune_generator") { }

        class spell_item_mingos_fortune_generator_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_mingos_fortune_generator_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                // Selecting one from Bloodstained Fortune item
                uint32 newitemid;
                switch (urand(1, 20))
                {
                    case 1:  newitemid = 32688; break;
                    case 2:  newitemid = 32689; break;
                    case 3:  newitemid = 32690; break;
                    case 4:  newitemid = 32691; break;
                    case 5:  newitemid = 32692; break;
                    case 6:  newitemid = 32693; break;
                    case 7:  newitemid = 32700; break;
                    case 8:  newitemid = 32701; break;
                    case 9:  newitemid = 32702; break;
                    case 10: newitemid = 32703; break;
                    case 11: newitemid = 32704; break;
                    case 12: newitemid = 32705; break;
                    case 13: newitemid = 32706; break;
                    case 14: newitemid = 32707; break;
                    case 15: newitemid = 32708; break;
                    case 16: newitemid = 32709; break;
                    case 17: newitemid = 32710; break;
                    case 18: newitemid = 32711; break;
                    case 19: newitemid = 32712; break;
                    case 20: newitemid = 32713; break;
                    default:
                        return;
                }

                CreateItem(effIndex, newitemid);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_mingos_fortune_generator_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_mingos_fortune_generator_SpellScript();
        }
};

// http://www.wowhead.com/item=10720 Gnomish Net-o-Matic Projector
// 13120 Net-o-Matic
enum NetOMaticSpells
{
    SPELL_NET_O_MATIC_TRIGGERED1 = 16566,
    SPELL_NET_O_MATIC_TRIGGERED2 = 13119,
    SPELL_NET_O_MATIC_TRIGGERED3 = 13099
};

class spell_item_net_o_matic: public SpellScriptLoader
{
    public:
        spell_item_net_o_matic() : SpellScriptLoader("spell_item_net_o_matic") { }

        class spell_item_net_o_matic_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_net_o_matic_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_NET_O_MATIC_TRIGGERED1) || !sSpellMgr->GetSpellInfo(SPELL_NET_O_MATIC_TRIGGERED2) || !sSpellMgr->GetSpellInfo(SPELL_NET_O_MATIC_TRIGGERED3))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                {
                    uint32 spellId = SPELL_NET_O_MATIC_TRIGGERED3;
                    uint32 roll = urand(0, 99);
                    if (roll < 2)                            // 2% for 30 sec self root (off-like chance unknown)
                        spellId = SPELL_NET_O_MATIC_TRIGGERED1;
                    else if (roll < 4)                       // 2% for 20 sec root, charge to target (off-like chance unknown)
                        spellId = SPELL_NET_O_MATIC_TRIGGERED2;

                    GetCaster()->CastSpell(target, spellId, true, NULL);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_net_o_matic_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_net_o_matic_SpellScript();
        }
};

// http://www.wowhead.com/item=8529 Noggenfogger Elixir
// 16589 Noggenfogger Elixir
enum NoggenfoggerElixirSpells
{
    SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED1 = 16595,
    SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED2 = 16593,
    SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED3 = 16591
};

class spell_item_noggenfogger_elixir: public SpellScriptLoader
{
    public:
        spell_item_noggenfogger_elixir() : SpellScriptLoader("spell_item_noggenfogger_elixir") { }

        class spell_item_noggenfogger_elixir_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_noggenfogger_elixir_SpellScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED1) || !sSpellMgr->GetSpellInfo(SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED2) || !sSpellMgr->GetSpellInfo(SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED3))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                uint32 spellId = SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED3;
                switch (urand(1, 3))
                {
                    case 1: spellId = SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED1; break;
                    case 2: spellId = SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED2; break;
                }

                caster->CastSpell(caster, spellId, true, NULL);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_noggenfogger_elixir_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_noggenfogger_elixir_SpellScript();
        }
};

// http://www.wowhead.com/item=6657 Savory Deviate Delight
// 8213 Savory Deviate Delight
enum SavoryDeviateDelight
{
    SPELL_FLIP_OUT_MALE     = 8219,
    SPELL_FLIP_OUT_FEMALE   = 8220,
    SPELL_YAAARRRR_MALE     = 8221,
    SPELL_YAAARRRR_FEMALE   = 8222
};

class spell_item_savory_deviate_delight: public SpellScriptLoader
{
    public:
        spell_item_savory_deviate_delight() : SpellScriptLoader("spell_item_savory_deviate_delight") { }

        class spell_item_savory_deviate_delight_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_savory_deviate_delight_SpellScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                for (uint32 spellId = SPELL_FLIP_OUT_MALE; spellId <= SPELL_YAAARRRR_FEMALE; ++spellId)
                    if (!sSpellMgr->GetSpellInfo(spellId))
                        return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                uint32 spellId = 0;
                switch (urand(1, 2))
                {
                    // Flip Out - ninja
                    case 1: spellId = (caster->getGender() == GENDER_MALE ? SPELL_FLIP_OUT_MALE : SPELL_FLIP_OUT_FEMALE); break;
                    // Yaaarrrr - pirate
                    case 2: spellId = (caster->getGender() == GENDER_MALE ? SPELL_YAAARRRR_MALE : SPELL_YAAARRRR_FEMALE); break;
                }
                caster->CastSpell(caster, spellId, true, NULL);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_savory_deviate_delight_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_savory_deviate_delight_SpellScript();
        }
};

// http://www.wowhead.com/item=7734 Six Demon Bag
// 14537 Six Demon Bag
enum SixDemonBagSpells
{
    SPELL_FROSTBOLT                 = 11538,
    SPELL_POLYMORPH                 = 14621,
    SPELL_SUMMON_FELHOUND_MINION    = 14642,
    SPELL_FIREBALL                  = 15662,
    SPELL_CHAIN_LIGHTNING           = 21179,
    SPELL_ENVELOPING_WINDS          = 25189
};

class spell_item_six_demon_bag: public SpellScriptLoader
{
    public:
        spell_item_six_demon_bag() : SpellScriptLoader("spell_item_six_demon_bag") { }

        class spell_item_six_demon_bag_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_six_demon_bag_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_FROSTBOLT) || !sSpellMgr->GetSpellInfo(SPELL_POLYMORPH) || !sSpellMgr->GetSpellInfo(SPELL_SUMMON_FELHOUND_MINION) || !sSpellMgr->GetSpellInfo(SPELL_FIREBALL) || !sSpellMgr->GetSpellInfo(SPELL_CHAIN_LIGHTNING) || !sSpellMgr->GetSpellInfo(SPELL_ENVELOPING_WINDS))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Unit* target = GetHitUnit())
                {
                    uint32 spellId = 0;
                    uint32 rand = urand(0, 99);
                    if (rand < 25)                      // Fireball (25% chance)
                        spellId = SPELL_FIREBALL;
                    else if (rand < 50)                 // Frostball (25% chance)
                        spellId = SPELL_FROSTBOLT;
                    else if (rand < 70)                 // Chain Lighting (20% chance)
                        spellId = SPELL_CHAIN_LIGHTNING;
                    else if (rand < 80)                 // Polymorph (10% chance)
                    {
                        spellId = SPELL_POLYMORPH;
                        if (urand(0, 100) <= 30)        // 30% chance to self-cast
                            target = caster;
                    }
                    else if (rand < 95)                 // Enveloping Winds (15% chance)
                        spellId = SPELL_ENVELOPING_WINDS;
                    else                                // Summon Felhund minion (5% chance)
                    {
                        spellId = SPELL_SUMMON_FELHOUND_MINION;
                        target = caster;
                    }

                    caster->CastSpell(target, spellId, true, GetCastItem());
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_six_demon_bag_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_six_demon_bag_SpellScript();
        }
};

// http://www.wowhead.com/item=44012 Underbelly Elixir
// 59640 Underbelly Elixir
enum UnderbellyElixirSpells
{
    SPELL_UNDERBELLY_ELIXIR_TRIGGERED1 = 59645,
    SPELL_UNDERBELLY_ELIXIR_TRIGGERED2 = 59831,
    SPELL_UNDERBELLY_ELIXIR_TRIGGERED3 = 59843
};

class spell_item_underbelly_elixir: public SpellScriptLoader
{
    public:
        spell_item_underbelly_elixir() : SpellScriptLoader("spell_item_underbelly_elixir") { }

        class spell_item_underbelly_elixir_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_underbelly_elixir_SpellScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }
            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_UNDERBELLY_ELIXIR_TRIGGERED1) || !sSpellMgr->GetSpellInfo(SPELL_UNDERBELLY_ELIXIR_TRIGGERED2) || !sSpellMgr->GetSpellInfo(SPELL_UNDERBELLY_ELIXIR_TRIGGERED3))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                uint32 spellId = SPELL_UNDERBELLY_ELIXIR_TRIGGERED3;
                switch (urand(1, 3))
                {
                    case 1: spellId = SPELL_UNDERBELLY_ELIXIR_TRIGGERED1; break;
                    case 2: spellId = SPELL_UNDERBELLY_ELIXIR_TRIGGERED2; break;
                }
                caster->CastSpell(caster, spellId, true, NULL);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_underbelly_elixir_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_underbelly_elixir_SpellScript();
        }
};

enum eShadowmourneVisuals
{
    SPELL_SHADOWMOURNE_VISUAL_LOW       = 72521,
    SPELL_SHADOWMOURNE_VISUAL_HIGH      = 72523,
    SPELL_SHADOWMOURNE_CHAOS_BANE_BUFF  = 73422
};

class spell_item_shadowmourne: public SpellScriptLoader
{
public:
    spell_item_shadowmourne() : SpellScriptLoader("spell_item_shadowmourne") { }

    class spell_item_shadowmourne_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_item_shadowmourne_AuraScript);

        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHADOWMOURNE_VISUAL_LOW) || !sSpellMgr->GetSpellInfo(SPELL_SHADOWMOURNE_VISUAL_HIGH) || !sSpellMgr->GetSpellInfo(SPELL_SHADOWMOURNE_CHAOS_BANE_BUFF))
                return false;
            return true;
        }

        void OnStackChange(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            switch (GetStackAmount())
            {
                case 1:
                    target->CastSpell(target, SPELL_SHADOWMOURNE_VISUAL_LOW, true);
                    break;
                case 6:
                    target->RemoveAurasDueToSpell(SPELL_SHADOWMOURNE_VISUAL_LOW);
                    target->CastSpell(target, SPELL_SHADOWMOURNE_VISUAL_HIGH, true);
                    break;
                case 10:
                    target->RemoveAurasDueToSpell(SPELL_SHADOWMOURNE_VISUAL_HIGH);
                    target->CastSpell(target, SPELL_SHADOWMOURNE_CHAOS_BANE_BUFF, true);
                    break;
                default:
                    break;
            }
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            target->RemoveAurasDueToSpell(SPELL_SHADOWMOURNE_VISUAL_LOW);
            target->RemoveAurasDueToSpell(SPELL_SHADOWMOURNE_VISUAL_HIGH);
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_item_shadowmourne_AuraScript::OnStackChange, EFFECT_0, SPELL_AURA_MOD_STAT, AuraEffectHandleModes(AURA_EFFECT_HANDLE_REAL | AURA_EFFECT_HANDLE_REAPPLY));
            AfterEffectRemove += AuraEffectRemoveFn(spell_item_shadowmourne_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_STAT, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_item_shadowmourne_AuraScript();
    }
};

enum AirRifleSpells
{
    SPELL_AIR_RIFLE_HOLD_VISUAL = 65582,
    SPELL_AIR_RIFLE_SHOOT       = 67532,
    SPELL_AIR_RIFLE_SHOOT_SELF  = 65577
};

class spell_item_red_rider_air_rifle: public SpellScriptLoader
{
    public:
        spell_item_red_rider_air_rifle() : SpellScriptLoader("spell_item_red_rider_air_rifle") { }

        class spell_item_red_rider_air_rifle_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_red_rider_air_rifle_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_AIR_RIFLE_HOLD_VISUAL) || !sSpellMgr->GetSpellInfo(SPELL_AIR_RIFLE_SHOOT) || !sSpellMgr->GetSpellInfo(SPELL_AIR_RIFLE_SHOOT_SELF))
                    return false;
                return true;
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                Unit* caster = GetCaster();
                if (Unit* target = GetHitUnit())
                {
                    caster->CastSpell(caster, SPELL_AIR_RIFLE_HOLD_VISUAL, true);
                    // needed because this spell shares GCD with its triggered spells (which must not be cast with triggered flag)
                    if (Player* player = caster->ToPlayer())
                        player->GetGlobalCooldownMgr().CancelGlobalCooldown(GetSpellInfo());
                    if (urand(0, 4))
                        caster->CastSpell(target, SPELL_AIR_RIFLE_SHOOT, false);
                    else
                        caster->CastSpell(caster, SPELL_AIR_RIFLE_SHOOT_SELF, false);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_red_rider_air_rifle_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_red_rider_air_rifle_SpellScript();
        }
};

enum GenericData
{
    SPELL_ARCANITE_DRAGONLING           = 19804,
    SPELL_BATTLE_CHICKEN                = 13166,
    SPELL_MECHANICAL_DRAGONLING         = 4073,
    SPELL_MITHRIL_MECHANICAL_DRAGONLING = 12749
};

enum CreateHeartCandy
{
    ITEM_HEART_CANDY_1 = 21818,
    ITEM_HEART_CANDY_2 = 21817,
    ITEM_HEART_CANDY_3 = 21821,
    ITEM_HEART_CANDY_4 = 21819,
    ITEM_HEART_CANDY_5 = 21816,
    ITEM_HEART_CANDY_6 = 21823,
    ITEM_HEART_CANDY_7 = 21822,
    ITEM_HEART_CANDY_8 = 21820
};

class spell_item_create_heart_candy: public SpellScriptLoader
{
    public:
        spell_item_create_heart_candy() : SpellScriptLoader("spell_item_create_heart_candy") { }

        class spell_item_create_heart_candy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_create_heart_candy_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (Player* target = GetHitPlayer())
                {
                    static const uint32 items[] = {ITEM_HEART_CANDY_1, ITEM_HEART_CANDY_2, ITEM_HEART_CANDY_3, ITEM_HEART_CANDY_4, ITEM_HEART_CANDY_5, ITEM_HEART_CANDY_6, ITEM_HEART_CANDY_7, ITEM_HEART_CANDY_8};
                    target->AddItem(items[urand(0, 7)], 1);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_create_heart_candy_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_create_heart_candy_SpellScript();
        }
};

// Item 45912 spell 64323
class spell_item_book_of_glyph_mastery: public SpellScriptLoader
{
    public:
        spell_item_book_of_glyph_mastery() : SpellScriptLoader("spell_item_book_of_glyph_mastery") {}

        class spell_item_book_of_glyph_mastery_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_book_of_glyph_mastery_SpellScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }

            SpellCastResult CheckRequirement()
            {
                if (HasDiscoveredAllSpells(GetSpellInfo()->Id, GetCaster()->ToPlayer()))
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_LEARNED_EVERYTHING);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_item_book_of_glyph_mastery_SpellScript::CheckRequirement);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_book_of_glyph_mastery_SpellScript();
        }
};

enum GiftOfTheHarvester
{
    NPC_GHOUL   = 28845,
    MAX_GHOULS  = 5
};

class spell_item_gift_of_the_harvester: public SpellScriptLoader
{
    public:
        spell_item_gift_of_the_harvester() : SpellScriptLoader("spell_item_gift_of_the_harvester") {}

        class spell_item_gift_of_the_harvester_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_gift_of_the_harvester_SpellScript);

            SpellCastResult CheckRequirement()
            {
                std::list<Creature*> ghouls;
                GetCaster()->GetAllMinionsByEntry(ghouls, NPC_GHOUL);
                if (ghouls.size() >= MAX_GHOULS)
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_TOO_MANY_GHOULS);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_item_gift_of_the_harvester_SpellScript::CheckRequirement);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_gift_of_the_harvester_SpellScript();
        }
};

enum Sinkholes
{
    NPC_SOUTH_SINKHOLE      = 25664,
    NPC_NORTHEAST_SINKHOLE  = 25665,
    NPC_NORTHWEST_SINKHOLE  = 25666
};

class spell_item_map_of_the_geyser_fields: public SpellScriptLoader
{
    public:
        spell_item_map_of_the_geyser_fields() : SpellScriptLoader("spell_item_map_of_the_geyser_fields") {}

        class spell_item_map_of_the_geyser_fields_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_map_of_the_geyser_fields_SpellScript);

            SpellCastResult CheckSinkholes()
            {
                Unit* caster = GetCaster();
                if (caster->FindNearestCreature(NPC_SOUTH_SINKHOLE, 30.0f, true) ||
                    caster->FindNearestCreature(NPC_NORTHEAST_SINKHOLE, 30.0f, true) ||
                    caster->FindNearestCreature(NPC_NORTHWEST_SINKHOLE, 30.0f, true))
                    return SPELL_CAST_OK;

                SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_BE_CLOSE_TO_SINKHOLE);
                return SPELL_FAILED_CUSTOM_ERROR;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_item_map_of_the_geyser_fields_SpellScript::CheckSinkholes);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_map_of_the_geyser_fields_SpellScript();
        }
};

enum VanquishedClutchesSpells
{
    SPELL_CRUSHER       = 64982,
    SPELL_CONSTRICTOR   = 64983,
    SPELL_CORRUPTOR     = 64984
};

class spell_item_vanquished_clutches: public SpellScriptLoader
{
    public:
        spell_item_vanquished_clutches() : SpellScriptLoader("spell_item_vanquished_clutches") { }

        class spell_item_vanquished_clutches_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_vanquished_clutches_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_CRUSHER) || !sSpellMgr->GetSpellInfo(SPELL_CONSTRICTOR) || !sSpellMgr->GetSpellInfo(SPELL_CORRUPTOR))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                uint32 spellId = RAND(SPELL_CRUSHER, SPELL_CONSTRICTOR, SPELL_CORRUPTOR);
                Unit* caster = GetCaster();
                caster->CastSpell(caster, spellId, true);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_vanquished_clutches_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_vanquished_clutches_SpellScript();
        }
};

enum MagicEater
{
    SPELL_WILD_MAGIC                             = 58891,
    SPELL_WELL_FED_1                             = 57288,
    SPELL_WELL_FED_2                             = 57139,
    SPELL_WELL_FED_3                             = 57111,
    SPELL_WELL_FED_4                             = 57286,
    SPELL_WELL_FED_5                             = 57291
};

class spell_magic_eater_food: public SpellScriptLoader
{
    public:
        spell_magic_eater_food() : SpellScriptLoader("spell_magic_eater_food") {}

        class spell_magic_eater_food_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_magic_eater_food_AuraScript);

            void HandleTriggerSpell(AuraEffect const* /*aurEff*/)
            {
                PreventDefaultAction();
                Unit* target = GetTarget();
                switch (urand(0, 5))
                {
                    case 0:
                        target->CastSpell(target, SPELL_WILD_MAGIC, true);
                        break;
                    case 1:
                        target->CastSpell(target, SPELL_WELL_FED_1, true);
                        break;
                    case 2:
                        target->CastSpell(target, SPELL_WELL_FED_2, true);
                        break;
                    case 3:
                        target->CastSpell(target, SPELL_WELL_FED_3, true);
                        break;
                    case 4:
                        target->CastSpell(target, SPELL_WELL_FED_4, true);
                        break;
                    case 5:
                        target->CastSpell(target, SPELL_WELL_FED_5, true);
                        break;
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_magic_eater_food_AuraScript::HandleTriggerSpell, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_magic_eater_food_AuraScript();
        }
};

class spell_item_shimmering_vessel: public SpellScriptLoader
{
    public:
        spell_item_shimmering_vessel() : SpellScriptLoader("spell_item_shimmering_vessel") { }

        class spell_item_shimmering_vessel_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_shimmering_vessel_SpellScript);

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                if (Creature* target = GetHitCreature())
                    target->setDeathState(JUST_RESPAWNED);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_shimmering_vessel_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_shimmering_vessel_SpellScript();
        }
};

enum PurifyHelboarMeat
{
    SPELL_SUMMON_PURIFIED_HELBOAR_MEAT      = 29277,
    SPELL_SUMMON_TOXIC_HELBOAR_MEAT         = 29278,
    ITEM_PURIFIED_HELBOAR_MEAT              = 23248
};

class spell_item_purify_helboar_meat: public SpellScriptLoader
{
    public:
        spell_item_purify_helboar_meat() : SpellScriptLoader("spell_item_purify_helboar_meat") { }

        class spell_item_purify_helboar_meat_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_purify_helboar_meat_SpellScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SUMMON_PURIFIED_HELBOAR_MEAT) ||  !sSpellMgr->GetSpellInfo(SPELL_SUMMON_TOXIC_HELBOAR_MEAT))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Unit* caster = GetCaster();
                if (roll_chance_i(50))
                    caster->CastSpell(caster, SPELL_SUMMON_TOXIC_HELBOAR_MEAT, true);
                else
                    if (caster->IsPlayer())
                        caster->ToPlayer()->AddItem(ITEM_PURIFIED_HELBOAR_MEAT, 1);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_purify_helboar_meat_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_purify_helboar_meat_SpellScript();
        }
};

enum CrystalPrison
{
    OBJECT_IMPRISONED_DOOMGUARD     = 179644
};

class spell_item_crystal_prison_dummy_dnd: public SpellScriptLoader
{
    public:
        spell_item_crystal_prison_dummy_dnd() : SpellScriptLoader("spell_item_crystal_prison_dummy_dnd") { }

        class spell_item_crystal_prison_dummy_dnd_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_crystal_prison_dummy_dnd_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sObjectMgr->GetGameObjectTemplate(OBJECT_IMPRISONED_DOOMGUARD))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                if (Creature* target = GetHitCreature())
                    if (target->isDead() && !target->isPet())
                    {
                        GetCaster()->SummonGameObject(OBJECT_IMPRISONED_DOOMGUARD, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 0, 0, 0, 0, uint32(target->GetRespawnTime()-time(NULL)));
                        target->DespawnOrUnsummon();
                    }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_crystal_prison_dummy_dnd_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_crystal_prison_dummy_dnd_SpellScript();
        }
};

enum ReindeerTransformation
{
    SPELL_FLYING_REINDEER_310                   = 44827,
    SPELL_FLYING_REINDEER_280                   = 44825,
    SPELL_FLYING_REINDEER_60                    = 44824,
    SPELL_REINDEER_100                          = 25859,
    SPELL_REINDEER_60                           = 25858
};

class spell_item_reindeer_transformation: public SpellScriptLoader
{
    public:
        spell_item_reindeer_transformation() : SpellScriptLoader("spell_item_reindeer_transformation") { }

        class spell_item_reindeer_transformation_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_reindeer_transformation_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_FLYING_REINDEER_310) || !sSpellMgr->GetSpellInfo(SPELL_FLYING_REINDEER_280)
                    || !sSpellMgr->GetSpellInfo(SPELL_FLYING_REINDEER_60) || !sSpellMgr->GetSpellInfo(SPELL_REINDEER_100)
                    || !sSpellMgr->GetSpellInfo(SPELL_REINDEER_60))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Unit* caster = GetCaster();
                if (caster->HasAuraType(SPELL_AURA_MOUNTED))
                {
                    float flyspeed = caster->GetSpeedRate(MOVE_FLIGHT);
                    float speed = caster->GetSpeedRate(MOVE_RUN);

                    caster->RemoveAurasByType(SPELL_AURA_MOUNTED);
                    //5 different spells used depending on mounted speed and if mount can fly or not

                    if (flyspeed >= 4.1f)
                        // Flying Reindeer
                        caster->CastSpell(caster, SPELL_FLYING_REINDEER_310, true); //310% flying Reindeer
                    else if (flyspeed >= 3.8f)
                        // Flying Reindeer
                        caster->CastSpell(caster, SPELL_FLYING_REINDEER_280, true); //280% flying Reindeer
                    else if (flyspeed >= 1.6f)
                        // Flying Reindeer
                        caster->CastSpell(caster, SPELL_FLYING_REINDEER_60, true); //60% flying Reindeer
                    else if (speed >= 2.0f)
                        // Reindeer
                        caster->CastSpell(caster, SPELL_REINDEER_100, true); //100% ground Reindeer
                    else
                        // Reindeer
                        caster->CastSpell(caster, SPELL_REINDEER_60, true); //60% ground Reindeer
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_reindeer_transformation_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_reindeer_transformation_SpellScript();
    }
};

enum NighInvulnerability
{
    SPELL_NIGH_INVULNERABILITY                  = 30456,
    SPELL_COMPLETE_VULNERABILITY                = 30457
};

class spell_item_nigh_invulnerability: public SpellScriptLoader
{
    public:
        spell_item_nigh_invulnerability() : SpellScriptLoader("spell_item_nigh_invulnerability") { }

        class spell_item_nigh_invulnerability_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_nigh_invulnerability_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_NIGH_INVULNERABILITY) || !sSpellMgr->GetSpellInfo(SPELL_COMPLETE_VULNERABILITY))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Unit* caster = GetCaster();
                if (Item* castItem = GetCastItem())
                {
                    if (roll_chance_i(86))                  // Nigh-Invulnerability   - success
                        caster->CastSpell(caster, SPELL_NIGH_INVULNERABILITY, true, castItem);
                    else                                    // Complete Vulnerability - backfire in 14% casts
                        caster->CastSpell(caster, SPELL_COMPLETE_VULNERABILITY, true, castItem);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_nigh_invulnerability_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_nigh_invulnerability_SpellScript();
        }
};

enum Poultryzer
{
    SPELL_POULTRYIZER_SUCCESS    = 30501,
    SPELL_POULTRYIZER_BACKFIRE   = 30504
};

class spell_item_poultryizer: public SpellScriptLoader
{
    public:
        spell_item_poultryizer() : SpellScriptLoader("spell_item_poultryizer") { }

        class spell_item_poultryizer_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_poultryizer_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_POULTRYIZER_SUCCESS) || !sSpellMgr->GetSpellInfo(SPELL_POULTRYIZER_BACKFIRE))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                if (GetCastItem() && GetHitUnit())
                    GetCaster()->CastSpell(GetHitUnit(), roll_chance_i(80) ? SPELL_POULTRYIZER_SUCCESS : SPELL_POULTRYIZER_BACKFIRE , true, GetCastItem());
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_poultryizer_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_poultryizer_SpellScript();
        }
};

enum SocretharsStone
{
    SPELL_SOCRETHAR_TO_SEAT     = 35743,
    SPELL_SOCRETHAR_FROM_SEAT   = 35744
};

class spell_item_socrethars_stone: public SpellScriptLoader
{
    public:
        spell_item_socrethars_stone() : SpellScriptLoader("spell_item_socrethars_stone") { }

        class spell_item_socrethars_stone_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_socrethars_stone_SpellScript);

            bool Load()
            {
                return (GetCaster()->GetAreaId() == 3900 || GetCaster()->GetAreaId() == 3742);
            }
            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SOCRETHAR_TO_SEAT) || !sSpellMgr->GetSpellInfo(SPELL_SOCRETHAR_FROM_SEAT))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Unit* caster = GetCaster();
                switch (caster->GetAreaId())
                {
                    case 3900:
                        caster->CastSpell(caster, SPELL_SOCRETHAR_TO_SEAT, true);
                        break;
                    case 3742:
                        caster->CastSpell(caster, SPELL_SOCRETHAR_FROM_SEAT, true);
                        break;
                    default:
                        return;
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_socrethars_stone_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_socrethars_stone_SpellScript();
        }
};

enum DemonBroiledSurprise
{
    QUEST_SUPER_HOT_STEW                    = 11379,
    SPELL_CREATE_DEMON_BROILED_SURPRISE     = 43753,
    NPC_ABYSSAL_FLAMEBRINGER                = 19973
};

class spell_item_demon_broiled_surprise: public SpellScriptLoader
{
    public:
        spell_item_demon_broiled_surprise() : SpellScriptLoader("spell_item_demon_broiled_surprise") { }

        class spell_item_demon_broiled_surprise_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_demon_broiled_surprise_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_CREATE_DEMON_BROILED_SURPRISE) || !sObjectMgr->GetCreatureTemplate(NPC_ABYSSAL_FLAMEBRINGER) || !sObjectMgr->GetQuestTemplate(QUEST_SUPER_HOT_STEW))
                    return false;
                return true;
            }

            bool Load()
            {
               return GetCaster()->IsPlayer();
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Unit* player = GetCaster();
                player->CastSpell(player, SPELL_CREATE_DEMON_BROILED_SURPRISE, false);
            }

            SpellCastResult CheckRequirement()
            {
                Player* player = GetCaster()->ToPlayer();
                if (player->GetQuestStatus(QUEST_SUPER_HOT_STEW) != QUEST_STATUS_INCOMPLETE)
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                if (Creature* creature = player->FindNearestCreature(NPC_ABYSSAL_FLAMEBRINGER, 10, false))
                    if (creature->isDead())
                        return SPELL_CAST_OK;
                return SPELL_FAILED_NOT_HERE;
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_demon_broiled_surprise_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
                OnCheckCast += SpellCheckCastFn(spell_item_demon_broiled_surprise_SpellScript::CheckRequirement);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_demon_broiled_surprise_SpellScript();
        }
};

enum CompleteRaptorCapture
{
    SPELL_RAPTOR_CAPTURE_CREDIT     = 42337
};

class spell_item_complete_raptor_capture: public SpellScriptLoader
{
    public:
        spell_item_complete_raptor_capture() : SpellScriptLoader("spell_item_complete_raptor_capture") { }

        class spell_item_complete_raptor_capture_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_complete_raptor_capture_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_RAPTOR_CAPTURE_CREDIT))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Unit* caster = GetCaster();
                if (GetHitCreature())
                {
                    GetHitCreature()->DespawnOrUnsummon();

                    //cast spell Raptor Capture Credit
                    caster->CastSpell(caster, SPELL_RAPTOR_CAPTURE_CREDIT, true, NULL);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_complete_raptor_capture_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_complete_raptor_capture_SpellScript();
        }
};

enum ImpaleLeviroth
{
    NPC_LEVIROTH                = 26452,
    SPELL_LEVIROTH_SELF_IMPALE  = 49882
};

class spell_item_impale_leviroth: public SpellScriptLoader
{
    public:
        spell_item_impale_leviroth() : SpellScriptLoader("spell_item_impale_leviroth") { }

        class spell_item_impale_leviroth_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_impale_leviroth_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sObjectMgr->GetCreatureTemplate(NPC_LEVIROTH))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                if (Unit* target = GetHitCreature())
                    if (target->GetEntry() == NPC_LEVIROTH && !target->HealthBelowPct(95))
                        target->CastSpell(target, SPELL_LEVIROTH_SELF_IMPALE, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_impale_leviroth_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_impale_leviroth_SpellScript();
        }
};

enum BrewfestMountTransformation
{
    SPELL_MOUNT_RAM_100                         = 43900,
    SPELL_MOUNT_RAM_60                          = 43899,
    SPELL_MOUNT_KODO_100                        = 49379,
    SPELL_MOUNT_KODO_60                         = 49378,
    SPELL_BREWFEST_MOUNT_TRANSFORM              = 49357,
    SPELL_BREWFEST_MOUNT_TRANSFORM_REVERSE      = 52845
};

class spell_item_brewfest_mount_transformation: public SpellScriptLoader
{
    public:
        spell_item_brewfest_mount_transformation() : SpellScriptLoader("spell_item_brewfest_mount_transformation") { }

        class spell_item_brewfest_mount_transformation_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_brewfest_mount_transformation_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MOUNT_RAM_100) || !sSpellMgr->GetSpellInfo(SPELL_MOUNT_RAM_60) || !sSpellMgr->GetSpellInfo(SPELL_MOUNT_KODO_100) || !sSpellMgr->GetSpellInfo(SPELL_MOUNT_KODO_60))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Player* caster = GetCaster()->ToPlayer();
                if (caster->HasAuraType(SPELL_AURA_MOUNTED))
                {
                    caster->RemoveAurasByType(SPELL_AURA_MOUNTED);
                    uint32 spell_id;

                    switch (GetSpellInfo()->Id)
                    {
                        case SPELL_BREWFEST_MOUNT_TRANSFORM:
                            if (caster->GetSpeedRate(MOVE_RUN) >= 2.0f)
                                spell_id = caster->GetTeam() == ALLIANCE ? SPELL_MOUNT_RAM_100 : SPELL_MOUNT_KODO_100;
                            else
                                spell_id = caster->GetTeam() == ALLIANCE ? SPELL_MOUNT_RAM_60 : SPELL_MOUNT_KODO_60;
                            break;
                        case SPELL_BREWFEST_MOUNT_TRANSFORM_REVERSE:
                            if (caster->GetSpeedRate(MOVE_RUN) >= 2.0f)
                                spell_id = caster->GetTeam() == HORDE ? SPELL_MOUNT_RAM_100 : SPELL_MOUNT_KODO_100;
                            else
                                spell_id = caster->GetTeam() == HORDE ? SPELL_MOUNT_RAM_60 : SPELL_MOUNT_KODO_60;
                            break;
                        default:
                            return;
                    }
                    caster->CastSpell(caster, spell_id, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_brewfest_mount_transformation_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_brewfest_mount_transformation_SpellScript();
        }
};

enum NitroBoots
{
    SPELL_NITRO_BOOTS_SUCCESS       = 54861,
    SPELL_NITRO_BOOTS_BACKFIRE      = 46014
};

class spell_item_nitro_boots: public SpellScriptLoader
{
    public:
        spell_item_nitro_boots() : SpellScriptLoader("spell_item_nitro_boots") { }

        class spell_item_nitro_boots_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_nitro_boots_SpellScript);

            bool Load()
            {
                if (!GetCastItem())
                    return false;
                return true;
            }

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_NITRO_BOOTS_SUCCESS) || !sSpellMgr->GetSpellInfo(SPELL_NITRO_BOOTS_BACKFIRE))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                Unit* caster = GetCaster();
                caster->CastSpell(caster, roll_chance_i(95) ? SPELL_NITRO_BOOTS_SUCCESS : SPELL_NITRO_BOOTS_BACKFIRE, true, GetCastItem());
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_item_nitro_boots_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_nitro_boots_SpellScript();
        }
};

enum TeachLanguage
{
    SPELL_LEARN_GNOMISH_BINARY      = 50242,
    SPELL_LEARN_GOBLIN_BINARY       = 50246
};

class spell_item_teach_language: public SpellScriptLoader
{
    public:
        spell_item_teach_language() : SpellScriptLoader("spell_item_teach_language") { }

        class spell_item_teach_language_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_teach_language_SpellScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_LEARN_GNOMISH_BINARY) || !sSpellMgr->GetSpellInfo(SPELL_LEARN_GOBLIN_BINARY))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Player* caster = GetCaster()->ToPlayer();

                if (roll_chance_i(34))
                    caster->CastSpell(caster,caster->GetTeam() == ALLIANCE ? SPELL_LEARN_GNOMISH_BINARY : SPELL_LEARN_GOBLIN_BINARY, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_teach_language_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_teach_language_SpellScript();
        }
};

enum RocketBoots
{
    SPELL_ROCKET_BOOTS_PROC      = 30452
};

class spell_item_rocket_boots: public SpellScriptLoader
{
    public:
        spell_item_rocket_boots() : SpellScriptLoader("spell_item_rocket_boots") { }

        class spell_item_rocket_boots_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_rocket_boots_SpellScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_ROCKET_BOOTS_PROC))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                if (!GetCaster() || !GetCaster()->ToPlayer())
                    return;

                Player* caster = GetCaster()->ToPlayer();
                if (Battleground* bg = caster->GetBattleground())
                    bg->EventPlayerDroppedFlag(caster);

                caster->RemoveSpellCooldown(SPELL_ROCKET_BOOTS_PROC);
                caster->CastSpell(caster, SPELL_ROCKET_BOOTS_PROC, true, NULL);
            }

            SpellCastResult CheckCast()
            {
                if (GetCaster()->IsInWater())
                    return SPELL_FAILED_ONLY_ABOVEWATER;
                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_item_rocket_boots_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_item_rocket_boots_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_rocket_boots_SpellScript();
        }
};

enum PygmyOil
{
    SPELL_PYGMY_OIL_PYGMY_AURA      = 53806,
    SPELL_PYGMY_OIL_SMALLER_AURA    = 53805
};

class spell_item_pygmy_oil: public SpellScriptLoader
{
    public:
        spell_item_pygmy_oil() : SpellScriptLoader("spell_item_pygmy_oil") { }

        class spell_item_pygmy_oil_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_pygmy_oil_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PYGMY_OIL_PYGMY_AURA) || !sSpellMgr->GetSpellInfo(SPELL_PYGMY_OIL_SMALLER_AURA))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Unit* caster = GetCaster();
                if (Aura* aura = caster->GetAura(SPELL_PYGMY_OIL_PYGMY_AURA))
                    aura->RefreshDuration();
                else
                {
                    aura = caster->GetAura(SPELL_PYGMY_OIL_SMALLER_AURA);
                    if (!aura || aura->GetStackAmount() < 5 || !roll_chance_i(50))
                         caster->CastSpell(caster, SPELL_PYGMY_OIL_SMALLER_AURA, true);
                    else
                    {
                        aura->Remove();
                        caster->CastSpell(caster, SPELL_PYGMY_OIL_PYGMY_AURA, true);
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_pygmy_oil_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_pygmy_oil_SpellScript();
        }
};

class spell_item_unusual_compass: public SpellScriptLoader
{
    public:
        spell_item_unusual_compass() : SpellScriptLoader("spell_item_unusual_compass") { }

        class spell_item_unusual_compass_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_unusual_compass_SpellScript);

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Unit* caster = GetCaster();
                caster->SetFacingTo(frand(0.0f, 62832.0f) / 10000.0f);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_unusual_compass_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_unusual_compass_SpellScript();
        }
};

enum ChickenCover
{
    SPELL_CHICKEN_NET               = 51959,
    SPELL_CAPTURE_CHICKEN_ESCAPE    = 51037,
    QUEST_CHICKEN_PARTY             = 12702,
    QUEST_FLOWN_THE_COOP            = 12532
};

class spell_item_chicken_cover: public SpellScriptLoader
{
    public:
        spell_item_chicken_cover() : SpellScriptLoader("spell_item_chicken_cover") { }

        class spell_item_chicken_cover_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_chicken_cover_SpellScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_CHICKEN_NET) || !sSpellMgr->GetSpellInfo(SPELL_CAPTURE_CHICKEN_ESCAPE) || !sObjectMgr->GetQuestTemplate(QUEST_CHICKEN_PARTY) || !sObjectMgr->GetQuestTemplate(QUEST_FLOWN_THE_COOP))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Player* caster = GetCaster()->ToPlayer();
                if (Unit* target = GetHitUnit())
                {
                    if (!target->HasAura(SPELL_CHICKEN_NET) && (caster->GetQuestStatus(QUEST_CHICKEN_PARTY) == QUEST_STATUS_INCOMPLETE || caster->GetQuestStatus(QUEST_FLOWN_THE_COOP) == QUEST_STATUS_INCOMPLETE))
                    {
                        caster->CastSpell(caster, SPELL_CAPTURE_CHICKEN_ESCAPE, true);
                        target->Kill(target);
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_chicken_cover_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_chicken_cover_SpellScript();
        }
};

enum Refocus
{
    SPELL_AIMED_SHOT    = 19434,
    SPELL_MULTISHOT     = 2643
};

class spell_item_refocus: public SpellScriptLoader
{
    public:
        spell_item_refocus() : SpellScriptLoader("spell_item_refocus") { }

        class spell_item_refocus_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_refocus_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Player* caster = GetCaster()->ToPlayer();

                if (!caster || caster->getClass() != CLASS_HUNTER)
                    return;

                if (caster->HasSpellCooldown(SPELL_AIMED_SHOT))
                    caster->RemoveSpellCooldown(SPELL_AIMED_SHOT, true);

                if (caster->HasSpellCooldown(SPELL_MULTISHOT))
                    caster->RemoveSpellCooldown(SPELL_MULTISHOT, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_refocus_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_NORMALIZED_WEAPON_DMG);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_refocus_SpellScript();
        }
};

class spell_item_muisek_vessel: public SpellScriptLoader
{
    public:
        spell_item_muisek_vessel() : SpellScriptLoader("spell_item_muisek_vessel") { }

        class spell_item_muisek_vessel_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_muisek_vessel_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Creature* target = GetHitCreature())
                    if (target->isDead())
                        target->DespawnOrUnsummon();
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_muisek_vessel_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_muisek_vessel_SpellScript();
        }
};

enum GreatmothersSoulcather
{
    SPELL_FORCE_CAST_SUMMON_GNOME_SOUL = 46486
};

class spell_item_greatmothers_soulcatcher: public SpellScriptLoader
{
public:
    spell_item_greatmothers_soulcatcher() : SpellScriptLoader("spell_item_greatmothers_soulcatcher") { }

    class spell_item_greatmothers_soulcatcher_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_greatmothers_soulcatcher_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (GetHitUnit())
                GetCaster()->CastSpell(GetCaster(),SPELL_FORCE_CAST_SUMMON_GNOME_SOUL);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_item_greatmothers_soulcatcher_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_greatmothers_soulcatcher_SpellScript();
    }
};

// Enohar Explosive Arrows - 78838
class spell_item_enohar_explosive_arrows: public SpellScriptLoader
{
    public:
        spell_item_enohar_explosive_arrows() : SpellScriptLoader("spell_item_enohar_explosive_arrows") { }

        class spell_item_enohar_explosive_arrows_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_enohar_explosive_arrows_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();

                if (caster && target)
                    caster->DealDamage(target, target->GetHealth());
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_enohar_explosive_arrows_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_enohar_explosive_arrows_SpellScript();
        }
};

enum HolyThurible
{
    NPC_WITHDRAWN_SOUL = 45166
};

class spell_item_holy_thurible: public SpellScriptLoader
{
    public:
        spell_item_holy_thurible() : SpellScriptLoader("spell_item_holy_thurible") { }

        class spell_item_holy_thurible_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_holy_thurible_SpellScript);

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Player* caster = GetCaster()->ToPlayer();
                // GetHitCreature don't work
                Creature* target  = caster->FindNearestCreature(NPC_WITHDRAWN_SOUL, 2.0f, true);
                if (target && caster)
                {

                    if (roll_chance_i(50))
                    {
                        caster->KilledMonsterCredit(NPC_WITHDRAWN_SOUL, target->GetGUID());
                        target->DespawnOrUnsummon(0);
                    }
                    else
                        target->setFaction(14);
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_holy_thurible_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_holy_thurible_SpellScript();
        }
};

enum bandage
{
    NPC_WOUNDED_DEFENDER = 38805
};

class spell_item_bandage_q24944: public SpellScriptLoader
{
    public:
        spell_item_bandage_q24944() : SpellScriptLoader("spell_item_bandage_q24944") { }

        class spell_item_bandage_q24944_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_bandage_q24944_SpellScript);

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Player* caster = GetCaster()->ToPlayer();
                Creature* target  = GetHitCreature();
                if (target && caster && target->GetEntry() == NPC_WOUNDED_DEFENDER)
                {
                    target->SetFullHealth();
                    target->MonsterSay("Thank you my friend", LANG_UNIVERSAL, 0);
                    target->ForcedDespawn();
                    caster->KilledMonsterCredit(NPC_WOUNDED_DEFENDER, 0);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_bandage_q24944_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_bandage_q24944_SpellScript();
        }
};

class spell_item_gen_alchemy_mop: public SpellScriptLoader
{
    public:
        spell_item_gen_alchemy_mop() : SpellScriptLoader("spell_item_gen_alchemy_mop") { }

        class spell_item_gen_alchemy_mop_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_gen_alchemy_mop_SpellScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }

            void HandleOnHit()
            {
                uint8 chance = urand(1,5); // not official, todo: find the rate
                Player* caster = GetCaster()->ToPlayer();
                if (caster && GetCaster()->IsPlayer() && !HasDiscoveredAllSpells(114751, GetCaster()->ToPlayer()) && chance == 1)
                {
                    if (uint32 discoveredSpellId = GetExplicitDiscoverySpell(114751, caster->ToPlayer()))
                        caster->learnSpell(discoveredSpellId, false);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_item_gen_alchemy_mop_SpellScript::HandleOnHit);
            }

        };
        SpellScript* GetSpellScript() const
        {
            return new spell_item_gen_alchemy_mop_SpellScript();
        }
};

class spell_alchemist_rejuvenation: public SpellScriptLoader
{
    public:
        spell_alchemist_rejuvenation() : SpellScriptLoader("spell_alchemist_rejuvenation") { }

        class spell_alchemist_rejuvenation_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_alchemist_rejuvenation_SpellScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->getPowerType() == POWER_MANA)
                    {
                        switch (caster->getLevel())
                        {
                             case 85:
                                caster->EnergizeBySpell(caster, 105704, urand(8484, 9376), POWER_MANA);
                                break;
                            case 86:
                                caster->EnergizeBySpell(caster, 105704, urand(13651, 15087), POWER_MANA);
                                break;
                            case 87:
                                caster->EnergizeBySpell(caster, 105704, urand(16451, 18181), POWER_MANA);
                                break;
                            case 88:
                                caster->EnergizeBySpell(caster, 105704, urand(19818, 21902), POWER_MANA);
                                break;
                            case 89:
                                caster->EnergizeBySpell(caster, 105704, urand(23884, 26398), POWER_MANA);
                                break;
                            case 90:
                                caster->EnergizeBySpell(caster, 105704, urand(28500, 31500), POWER_MANA);
                                break;
                            default:
                                break;
                        }
                    }
                    switch (caster->getLevel())
                    {
                        case 85:
                            caster->HealBySpell(caster, sSpellMgr->GetSpellInfo(105704), urand(33935, 37505), false);
                            break;
                        case 86:
                            caster->HealBySpell(caster, sSpellMgr->GetSpellInfo(105704), urand(54601, 60347), false);
                            break;
                        case 87:
                            caster->HealBySpell(caster, sSpellMgr->GetSpellInfo(105704), urand(65801, 72727), false);
                            break;
                        case 88:
                            caster->HealBySpell(caster, sSpellMgr->GetSpellInfo(105704), urand(79268, 87610), false);
                            break;
                        case 89:
                            caster->HealBySpell(caster, sSpellMgr->GetSpellInfo(105704), urand(95534, 105590), false);
                            break;
                        case 90:
                            caster->HealBySpell(caster, sSpellMgr->GetSpellInfo(105704), urand(114001, 126001), false);
                            break;
                        default:
                            break;
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_alchemist_rejuvenation_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_alchemist_rejuvenation_SpellScript();
        }
};

#define SPELL_AMBER_PRISON 127266

class spell_item_amber_prison: public SpellScriptLoader
{
    public:
        spell_item_amber_prison() : SpellScriptLoader("spell_item_amber_prison") { }

        class spell_item_amber_prison_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_amber_prison_SpellScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_AMBER_PRISON))
                    return false;
                return true;
            }

            SpellCastResult CheckCast()
            {
                if (Unit* target = GetExplTargetUnit())
                {
                    if (target->IsPlayer())
                        return SPELL_FAILED_TARGET_IS_PLAYER;
                    if (target->GetTypeId() == TYPEID_UNIT && target->getLevel() > 94)
                        return SPELL_FAILED_HIGHLEVEL;

                }
                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_item_amber_prison_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_amber_prison_SpellScript();
        }
};

class spell_item_first_aid: public SpellScriptLoader
{
    public:
        spell_item_first_aid() : SpellScriptLoader("spell_item_first_aid") { }

        class spell_item_first_aid_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_first_aid_SpellScript);

            void HandleAfterCast()
            {
                Unit* caster = GetCaster();
                Unit* target = GetExplTargetUnit();

                if (Player* player = caster->ToPlayer())
                {
                    if (player->GetQuestStatus(30148) == QUEST_STATUS_INCOMPLETE)
                    {
                        if (Creature* creature = target->ToCreature())
                        {
                            if (creature->GetEntry() == 58416)
                            {
                                player->KilledMonsterCredit(58417);
                                creature->DespawnOrUnsummon();
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_item_first_aid_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_first_aid_SpellScript();
        }
};

class spell_item_imputting_the_final_code: public SpellScriptLoader
{
    public:
        spell_item_imputting_the_final_code() : SpellScriptLoader("spell_item_imputting_the_final_code") { }

        class spell_item_imputting_the_final_code_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_imputting_the_final_code_SpellScript);

            void HandleAfterCast()
            {
                Unit* caster = GetCaster();

                if (Player* player = caster->ToPlayer())
                {
                    if (player->GetQuestStatus(10447) == QUEST_STATUS_INCOMPLETE)
                    {
                        player->KilledMonsterCredit(21039);
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_item_imputting_the_final_code_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_imputting_the_final_code_SpellScript();
        }
};

class spell_item_pot_of_fire: public SpellScriptLoader
{
    public:
        spell_item_pot_of_fire() : SpellScriptLoader("spell_item_pot_of_fire") { }

        class spell_item_pot_of_fire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_pot_of_fire_SpellScript);

            void CorrectTarget(std::list<WorldObject*>& targets)
            {
                std::list<WorldObject*> tempTargets = targets;
                for (auto itr : tempTargets)
                {
                    if (itr->GetEntry() != 61510)
                        targets.remove(itr);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_item_pot_of_fire_SpellScript::CorrectTarget, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_pot_of_fire_SpellScript();
        }

        class spell_item_pot_of_fire_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_pot_of_fire_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                Unit* caster = GetCaster();
                
                if (!target || !caster)
                    return;

                if (Creature* creature = target->ToCreature())
                {
                    if (Player* player = caster->ToPlayer())
                    {
                        player->KilledMonsterCredit(61510);
                        creature->DespawnOrUnsummon();
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_item_pot_of_fire_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_item_pot_of_fire_AuraScript();
        }
};

class spell_item_dit_da_jow: public SpellScriptLoader
{
    public:
        spell_item_dit_da_jow() : SpellScriptLoader("spell_item_dit_da_jow") { }

        class spell_item_dit_da_jow_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_dit_da_jow_SpellScript);

            void HandleBeforeCast()
            {
                Unit* caster = GetCaster();
                Unit* target = GetExplTargetUnit();

                if (Player* player = caster->ToPlayer())
                {
                    if (player->GetQuestStatus(30460) == QUEST_STATUS_INCOMPLETE)
                    {
                        if (Creature* creature = target->ToCreature())
                        {
                            if (creature->GetEntry() == 59143)
                            {
                                player->KilledMonsterCredit(59143);
                                creature->DespawnOrUnsummon();
                            }
                        }
                    }
                }
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_item_dit_da_jow_SpellScript::HandleBeforeCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_dit_da_jow_SpellScript();
        }
};

class spell_item_zuluhed_chains: public SpellScriptLoader
{
    public:
        spell_item_zuluhed_chains() : SpellScriptLoader("spell_item_zuluhed_chains") { }

        class spell_item_zuluhed_chains_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_zuluhed_chains_SpellScript);

            void HandleAfterCast()
            {
                Unit* caster = GetCaster();

                if (Player* player = caster->ToPlayer())
                {
                    if (player->GetQuestStatus(10866) == QUEST_STATUS_INCOMPLETE)
                    {
                        player->KilledMonsterCredit(22112);
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_item_zuluhed_chains_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_zuluhed_chains_SpellScript();
        }
};

class spell_item_yak_s_milk: public SpellScriptLoader
{
    public:
        spell_item_yak_s_milk() : SpellScriptLoader("spell_item_yak_s_milk") { }

        class spell_item_yak_s_milk_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_yak_s_milk_SpellScript);

            void HandleAfterCast()
            {
                Unit* caster = GetCaster();
                Unit* target = GetExplTargetUnit();

                if (Player* player = caster->ToPlayer())
                {
                    if (player->GetQuestStatus(30953) == QUEST_STATUS_INCOMPLETE)
                    {
                        if (Creature* creature = target->ToCreature())
                        {
                            if (creature->GetEntry() == 61570)
                            {
                                player->KilledMonsterCredit(61569);
                                creature->DespawnOrUnsummon();
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_item_yak_s_milk_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_yak_s_milk_SpellScript();
        }
};

class spell_item_throw_mantra: public SpellScriptLoader
{
    public:
        spell_item_throw_mantra() : SpellScriptLoader("spell_item_throw_mantra") { }

        class spell_item_throw_mantra_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_throw_mantra_SpellScript);

            void HandleAfterCast()
            {
                Unit* caster = GetCaster();
                Unit* target = GetExplTargetUnit();

                if (Player* player = caster->ToPlayer())
                {
                    if (player->GetQuestStatus(30066) == QUEST_STATUS_INCOMPLETE)
                    {
                        if (Creature* creature = target->ToCreature())
                        {
                            if (creature->GetEntry() == 58186 || creature->GetEntry() == 57400 || creature->GetEntry() == 57326)
                            {
                                player->KilledMonsterCredit(57705);
                                creature->DespawnOrUnsummon();
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_item_throw_mantra_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_throw_mantra_SpellScript();
        }
};

/// Ancient Knowledge - 127250
class spell_item_ancient_knowledge : public SpellScriptLoader
{
    public:
        spell_item_ancient_knowledge() : SpellScriptLoader("spell_item_ancient_knowledge") { }

        class spell_item_ancient_knowledge_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_ancient_knowledge_SpellScript);

            SpellCastResult CheckCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->getLevel() > 84)
                        return SpellCastResult::SPELL_FAILED_HIGHLEVEL;
                }

                return SpellCastResult::SPELL_CAST_OK;
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_item_ancient_knowledge_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_item_ancient_knowledge_SpellScript();
        }

        class spell_item_ancient_knowledge_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_ancient_knowledge_AuraScript);

            void OnUpdate(uint32 /*p_Diff*/)
            {
                if (Unit* l_Target = GetUnitOwner())
                {
                    if (l_Target->getLevel() >= 85)
                        Remove();
                }
            }

            void Register() override
            {
                OnAuraUpdate += AuraUpdateFn(spell_item_ancient_knowledge_AuraScript::OnUpdate);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_item_ancient_knowledge_AuraScript();
        }
};

/// Sky Golem - 134359
class spell_item_sky_golem : public SpellScriptLoader
{
    public:
        spell_item_sky_golem() : SpellScriptLoader("spell_item_sky_golem") { }

        class spell_item_sky_golem_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_sky_golem_AuraScript);

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (l_Target->IsPlayer() && !l_Target->GetMap()->IsBattlegroundOrArena())
                        l_Target->ToPlayer()->ApplyModFlag(EPlayerFields::PLAYER_FIELD_LOCAL_FLAGS, PlayerLocalFlags::PLAYER_LOCAL_FLAG_CAN_USE_OBJECTS_MOUNTED, true);
                }
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (l_Target->IsPlayer())
                        l_Target->ToPlayer()->ApplyModFlag(EPlayerFields::PLAYER_FIELD_LOCAL_FLAGS, PlayerLocalFlags::PLAYER_LOCAL_FLAG_CAN_USE_OBJECTS_MOUNTED, false);
                }
            }

            void Register() override
            {
                OnEffectApply += AuraEffectApplyFn(spell_item_sky_golem_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOUNTED, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_item_sky_golem_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOUNTED, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_item_sky_golem_AuraScript();
        }
};

enum eEngineeringScopesSpells
{
    SpellOglethorpesMissileSplitter = 156055,
    SpellMegawattFilament           = 156060,
    HemetsHeartseeker               = 173288
};

/// Called by Oglethorpe's Missile Splitter 156052, Megawatt Filament 156059, Hemet's Heartseeker 173286
class spell_item_engineering_scopes: public SpellScriptLoader
{
    public:
        spell_item_engineering_scopes(const char* p_Name, uint32 p_TriggeredSpellId) : SpellScriptLoader(p_Name), m_TriggeredSpellId(p_TriggeredSpellId) { }

        class spell_item_engineering_scopes_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_engineering_scopes_AuraScript);

            public:
                spell_item_engineering_scopes_AuraScript(uint32 p_TriggeredSpellId) : AuraScript(), m_TriggeredSpellId(p_TriggeredSpellId) { }

                bool Validate(SpellInfo const* /*p_SpellEntry*/)
                {
                    if (!sSpellMgr->GetSpellInfo(m_TriggeredSpellId))
                        return false;
                    return true;
                }

                void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& /*p_EventInfo*/)
                {
                    PreventDefaultAction();

                    Unit* l_Caster = GetCaster();
                    if (!l_Caster)
                        return;

                    l_Caster->CastSpell(l_Caster, m_TriggeredSpellId, true);
                }

                void Register()
                {
                    OnEffectProc += AuraEffectProcFn(spell_item_engineering_scopes_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
                }

            private:
                uint32 m_TriggeredSpellId;
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_item_engineering_scopes_AuraScript(m_TriggeredSpellId);
        }

    private:
        uint32 m_TriggeredSpellId;
};

/// Summon Chauffeur (Horde) - 179244
/// Summon Chauffeur (Alliance) - 179245
class spell_item_summon_chauffeur : public SpellScriptLoader
{
    public:
        spell_item_summon_chauffeur() : SpellScriptLoader("spell_item_summon_chauffeur") { }

        class spell_item_summon_chauffeur_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_summon_chauffeur_AuraScript);

            enum eCreatures
            {
                HordeChauffeur      = 89713,
                AllianceChauffeur   = 89715
            };

            void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Target = GetTarget())
                {
                    uint32 l_Entry = 0;
                    if (l_Target->GetTypeId() == TypeID::TYPEID_PLAYER)
                    {
                        if (l_Target->ToPlayer()->GetTeamId() == TeamId::TEAM_ALLIANCE)
                            l_Entry = eCreatures::AllianceChauffeur;
                        else
                            l_Entry = eCreatures::HordeChauffeur;
                    }

                    if (Creature* l_Chauffeur = l_Target->SummonCreature(l_Entry, *l_Target))
                        l_Target->SetPersonnalChauffeur(l_Chauffeur->GetGUID());
                }
            }

            void AfterApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (Creature* l_Chauffeur = sObjectAccessor->FindCreature(l_Target->GetPersonnalChauffeur()))
                        l_Chauffeur->EnterVehicle(l_Target);
                }
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (Creature* l_Chauffeur = sObjectAccessor->FindCreature(l_Target->GetPersonnalChauffeur()))
                        l_Chauffeur->DespawnOrUnsummon();
                }
            }

            void Register() override
            {
                OnEffectApply += AuraEffectApplyFn(spell_item_summon_chauffeur_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOUNTED, AURA_EFFECT_HANDLE_REAL);
                AfterEffectApply += AuraEffectApplyFn(spell_item_summon_chauffeur_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_MOUNTED, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_item_summon_chauffeur_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOUNTED, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_item_summon_chauffeur_AuraScript();
        }
};

/// Forgemaster's Vigor - 177096
class spell_item_forgemasters_vigor : public SpellScriptLoader
{
    public:
        spell_item_forgemasters_vigor() : SpellScriptLoader("spell_item_forgemasters_vigor") { }

        class spell_item_forgemasters_vigor_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_forgemasters_vigor_AuraScript);

            enum eSpell
            {
                HammerBlows = 177099
            };

            void OnTick(AuraEffect const* p_AurEff)
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (!l_Target->HasAura(eSpell::HammerBlows))
                    {
                        l_Target->CastSpell(l_Target, eSpell::HammerBlows, true);

                        if (AuraEffect* l_AuraEffect = l_Target->GetAuraEffect(eSpell::HammerBlows, EFFECT_0))
                            l_AuraEffect->ChangeAmount(p_AurEff->GetAmount());
                    }
                    else
                    {
                        if (Aura* l_Aura = l_Target->GetAura(eSpell::HammerBlows))
                            l_Aura->ModStackAmount(1);
                    }
                }
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->RemoveAura(eSpell::HammerBlows);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_item_forgemasters_vigor_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectRemove += AuraEffectRemoveFn(spell_item_forgemasters_vigor_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_item_forgemasters_vigor_AuraScript();
        }
};

/// Hammer Blows - 177099
class spell_item_hammer_blows : public SpellScriptLoader
{
    public:
        spell_item_hammer_blows() : SpellScriptLoader("spell_item_hammer_blows") { }

        class spell_item_hammer_blows_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_hammer_blows_AuraScript);

            enum eSpell
            {
                ForgemastersVigor = 177096
            };

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                if (Unit* l_Target = GetUnitOwner())
                {
                    if (AuraEffect* l_AurEff = l_Target->GetAuraEffect(eSpell::ForgemastersVigor, EFFECT_0))
                        p_Amount = l_AurEff->GetAmount();
                }
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_item_hammer_blows_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_RATING);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_item_hammer_blows_AuraScript();
        }
};

/// Detonation - 177067
class spell_item_detonation : public SpellScriptLoader
{
    public:
        spell_item_detonation() : SpellScriptLoader("spell_item_detonation") { }

        class spell_item_detonation_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_detonation_AuraScript);

            enum eSpell
            {
                Detonating = 177070
            };

            void OnTick(AuraEffect const* p_AurEff)
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (!l_Target->HasAura(eSpell::Detonating))
                    {
                        l_Target->CastSpell(l_Target, eSpell::Detonating, true);

                        if (AuraEffect* l_AuraEffect = l_Target->GetAuraEffect(eSpell::Detonating, EFFECT_0))
                            l_AuraEffect->ChangeAmount(p_AurEff->GetAmount());
                    }
                    else
                    {
                        if (Aura* l_Aura = l_Target->GetAura(eSpell::Detonating))
                            l_Aura->ModStackAmount(1);
                    }
                }
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->RemoveAura(eSpell::Detonating);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_item_detonation_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectRemove += AuraEffectRemoveFn(spell_item_detonation_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_item_detonation_AuraScript();
        }
};

/// Detonating - 177070
class spell_item_detonating : public SpellScriptLoader
{
    public:
        spell_item_detonating() : SpellScriptLoader("spell_item_detonating") { }

        class spell_item_detonating_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_detonating_AuraScript);

            enum eSpell
            {
                Detonation = 177067
            };

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                if (Unit* l_Target = GetUnitOwner())
                {
                    if (AuraEffect* l_AurEff = l_Target->GetAuraEffect(eSpell::Detonation, EFFECT_0))
                        p_Amount = l_AurEff->GetAmount();
                }
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_item_detonating_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_RATING);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_item_detonating_AuraScript();
        }
};

/// Battering - 177102
class spell_item_battering : public SpellScriptLoader
{
    public:
        spell_item_battering() : SpellScriptLoader("spell_item_battering") { }

        class spell_item_battering_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_battering_AuraScript);

            enum eSpell
            {
                Cracks = 177103
            };

            void OnTick(AuraEffect const* p_AurEff)
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (!l_Target->HasAura(eSpell::Cracks))
                    {
                        l_Target->CastSpell(l_Target, eSpell::Cracks, true);

                        if (AuraEffect* l_AuraEffect = l_Target->GetAuraEffect(eSpell::Cracks, EFFECT_0))
                            l_AuraEffect->ChangeAmount(p_AurEff->GetAmount());
                    }
                    else
                    {
                        if (Aura* l_Aura = l_Target->GetAura(eSpell::Cracks))
                            l_Aura->ModStackAmount(1);
                    }
                }
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->RemoveAura(eSpell::Cracks);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_item_battering_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectRemove += AuraEffectRemoveFn(spell_item_battering_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_item_battering_AuraScript();
        }
};

/// Cracks! - 177103
class spell_item_cracks : public SpellScriptLoader
{
    public:
        spell_item_cracks() : SpellScriptLoader("spell_item_cracks") { }

        class spell_item_cracks_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_cracks_AuraScript);

            enum eSpell
            {
                Battering = 177102
            };

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                if (Unit* l_Target = GetUnitOwner())
                {
                    if (AuraEffect* l_AurEff = l_Target->GetAuraEffect(eSpell::Battering, EFFECT_0))
                        p_Amount = l_AurEff->GetAmount();
                }
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_item_cracks_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_RATING);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_item_cracks_AuraScript();
        }
};

/// Sanitizing - 177086
class spell_item_sanatizing : public SpellScriptLoader
{
    public:
        spell_item_sanatizing() : SpellScriptLoader("spell_item_sanatizing") { }

        class spell_item_sanatizing_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_sanatizing_AuraScript);

            enum eSpell
            {
                CleansingSteam = 177087
            };

            void OnTick(AuraEffect const* p_AurEff)
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (!l_Target->HasAura(eSpell::CleansingSteam))
                    {
                        l_Target->CastSpell(l_Target, eSpell::CleansingSteam, true);

                        if (AuraEffect* l_AuraEffect = l_Target->GetAuraEffect(eSpell::CleansingSteam, EFFECT_0))
                            l_AuraEffect->ChangeAmount(p_AurEff->GetAmount());
                    }
                    else
                    {
                        if (Aura* l_Aura = l_Target->GetAura(eSpell::CleansingSteam))
                            l_Aura->ModStackAmount(1);
                    }
                }
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->RemoveAura(eSpell::CleansingSteam);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_item_sanatizing_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectRemove += AuraEffectRemoveFn(spell_item_sanatizing_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_item_sanatizing_AuraScript();
        }
};

/// Cleansing Steam - 177087
class spell_item_cleansing_steam : public SpellScriptLoader
{
    public:
        spell_item_cleansing_steam() : SpellScriptLoader("spell_item_cleansing_steam") { }

        class spell_item_cleansing_steam_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_cleansing_steam_AuraScript);

            enum eSpell
            {
                Sanitizing = 177086
            };

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                if (Unit* l_Target = GetUnitOwner())
                {
                    if (AuraEffect* l_AurEff = l_Target->GetAuraEffect(eSpell::Sanitizing, EFFECT_0))
                        p_Amount = l_AurEff->GetAmount();
                }
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_item_cleansing_steam_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_RATING);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_item_cleansing_steam_AuraScript();
        }
};

/// Molten Metal - 177081
class spell_item_molten_metal : public SpellScriptLoader
{
    public:
        spell_item_molten_metal() : SpellScriptLoader("spell_item_molten_metal") { }

        class spell_item_molten_metal_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_molten_metal_AuraScript);

            enum eSpell
            {
                PouringSlag = 177083
            };

            void OnTick(AuraEffect const* p_AurEff)
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (!l_Target->HasAura(eSpell::PouringSlag))
                    {
                        l_Target->CastSpell(l_Target, eSpell::PouringSlag, true);

                        if (AuraEffect* l_AuraEffect = l_Target->GetAuraEffect(eSpell::PouringSlag, EFFECT_0))
                            l_AuraEffect->ChangeAmount(p_AurEff->GetAmount());
                    }
                    else
                    {
                        if (Aura* l_Aura = l_Target->GetAura(eSpell::PouringSlag))
                            l_Aura->ModStackAmount(1);
                    }
                }
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->RemoveAura(eSpell::PouringSlag);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_item_molten_metal_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectRemove += AuraEffectRemoveFn(spell_item_molten_metal_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_item_molten_metal_AuraScript();
        }
};

/// Pouring Slag - 177083
class spell_item_pouring_slag : public SpellScriptLoader
{
    public:
        spell_item_pouring_slag() : SpellScriptLoader("spell_item_pouring_slag") { }

        class spell_item_pouring_slag_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_pouring_slag_AuraScript);

            enum eSpell
            {
                MoltenMetal = 177081
            };

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                if (Unit* l_Target = GetUnitOwner())
                {
                    if (AuraEffect* l_AurEff = l_Target->GetAuraEffect(eSpell::MoltenMetal, EFFECT_0))
                        p_Amount = l_AurEff->GetAmount();
                }
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_item_pouring_slag_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_RATING);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_item_pouring_slag_AuraScript();
        }
};

/// Legs of Iron (T17) - 178210
/// Legs of Iron (Normal - T17 - Quest) - 178218
/// Legs of Iron (Heroic - T17 - Quest) - 178221
/// Legs of Iron (Mythic - T17 - Quest) - 178228
class spell_item_legs_of_iron : public SpellScriptLoader
{
    public:
        spell_item_legs_of_iron() : SpellScriptLoader("spell_item_legs_of_iron") { }

        class spell_item_legs_of_iron_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_legs_of_iron_SpellScript);

            enum eItemIDs
            {
                LegplatesOfGudingLight      = 115569,
                ShadowCouncilsLeggings      = 115587,
                SoulPriestsLeggings         = 115564,
                ArcanoshatterLeggings       = 115554,
                LivingWoodLegguards         = 115543,
                OgreskullBoneplateGreaves   = 115535,
                PoisonersLegguards          = 115573,
                BlackhandsLegguards         = 115580,
                LegwrapsOfTheSomberGaze     = 115557,
                RylakstalkersLegguards      = 115546,
                WindspeakersLegwraps        = 115575
            };

            enum eBonusIDs
            {
                /// For Token
                TokenHeroic = 570,
                TokenMythic = 569,
                /// For T17
                Heroic      = 566,
                Mythic      = 567
            };

            enum eSpellIDs
            {
                NormalQuestToken    = 178218,
                HeroicQuestToken    = 178221,
                MythicQuestToken    = 178228
            };

            void HandleDummy(SpellEffIndex /*p_EffIndex*/)
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Item* l_Item = GetCastItem())
                    {
                        uint32 l_ItemID = 0;
                        std::vector<uint32> l_Bonuses;

                        switch (l_Player->getClass())
                        {
                            case Classes::CLASS_PALADIN:
                                l_ItemID = eItemIDs::LegplatesOfGudingLight;
                                break;
                            case Classes::CLASS_WARLOCK:
                                l_ItemID = eItemIDs::ShadowCouncilsLeggings;
                                break;
                            case Classes::CLASS_PRIEST:
                                l_ItemID = eItemIDs::SoulPriestsLeggings;
                                break;
                            case Classes::CLASS_MAGE:
                                l_ItemID = eItemIDs::ArcanoshatterLeggings;
                                break;
                            case Classes::CLASS_DRUID:
                                l_ItemID = eItemIDs::LivingWoodLegguards;
                                break;
                            case Classes::CLASS_DEATH_KNIGHT:
                                l_ItemID = eItemIDs::OgreskullBoneplateGreaves;
                                break;
                            case Classes::CLASS_ROGUE:
                                l_ItemID = eItemIDs::PoisonersLegguards;
                                break;
                            case Classes::CLASS_WARRIOR:
                                l_ItemID = eItemIDs::BlackhandsLegguards;
                                break;
                            case Classes::CLASS_MONK:
                                l_ItemID = eItemIDs::LegwrapsOfTheSomberGaze;
                                break;
                            case Classes::CLASS_HUNTER:
                                l_ItemID = eItemIDs::RylakstalkersLegguards;
                                break;
                            case Classes::CLASS_SHAMAN:
                                l_ItemID = eItemIDs::WindspeakersLegwraps;
                                break;
                            default:
                                break;
                        }

                        if (l_Item->HasItemBonus(eBonusIDs::TokenMythic))
                            l_Bonuses.push_back(eBonusIDs::Mythic);
                        else if (l_Item->HasItemBonus(eBonusIDs::TokenHeroic))
                            l_Bonuses.push_back(eBonusIDs::Heroic);

                        switch (GetSpellInfo()->Id)
                        {
                            case eSpellIDs::HeroicQuestToken:
                                l_Bonuses.push_back(eBonusIDs::Heroic);
                                break;
                            case eSpellIDs::MythicQuestToken:
                                l_Bonuses.push_back(eBonusIDs::Mythic);
                                break;
                            case eSpellIDs::NormalQuestToken:
                            default:
                                break;
                        }

                        if (l_ItemID)
                        {
                            /// Adding items
                            uint32 l_NoSpaceForCount = 0;
                            uint32 l_Count = 1;

                            /// check space and find places
                            ItemPosCountVec l_Dest;
                            InventoryResult l_MSG = l_Player->CanStoreNewItem(InventorySlot::NULL_BAG, InventorySlot::NULL_SLOT, l_Dest, l_ItemID, l_Count, &l_NoSpaceForCount);

                            /// Convert to possible store amount
                            if (l_MSG != InventoryResult::EQUIP_ERR_OK)
                                l_Count -= l_NoSpaceForCount;

                            /// Can't add any
                            if (l_Count == 0 || l_Dest.empty())
                                return;

                            if (Item* l_NewItem = l_Player->StoreNewItem(l_Dest, l_ItemID, true))
                            {
                                /// Context must be normal, difficulty bonus is already added
                                Item::GenerateItemBonus(l_ItemID, ItemContext::RaidNormal, l_Bonuses);

                                if (!l_Bonuses.empty())
                                    l_NewItem->AddItemBonuses(l_Bonuses);

                                l_Player->SendNewItem(l_NewItem, l_Count, false, true);
                            }
                        }
                    }
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_legs_of_iron_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_item_legs_of_iron_SpellScript();
        }
};

/// Chest of Iron (T17) - 178209
/// Chest of Iron (Normal - T17 - Quest) - 178217
/// Chest of Iron (Heroic - T17 - Quest) - 178225
/// Chest of Iron (Mythic - T17 - Quest) - 178227
class spell_item_chest_of_iron : public SpellScriptLoader
{
    public:
        spell_item_chest_of_iron() : SpellScriptLoader("spell_item_chest_of_iron") { }

        class spell_item_chest_of_iron_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_chest_of_iron_SpellScript);

            enum eItemIDs
            {
                BattleplateOfGudingLight        = 115566,
                ShadowCouncilsRobes             = 115588,
                SoulPriestsRaiment              = 115560,
                ArcanoshatterRobes              = 115550,
                LivingWoodRaiment               = 115540,
                OgreskullBoneplateBreastplate   = 115537,
                PoisonersTunic                  = 115570,
                BlackhandsChestguard            = 115582,
                VestOfTheSomberGaze             = 115558,
                RylakstalkersTunic              = 115548,
                WindspeakersTunic               = 115577
            };

            enum eBonusIDs
            {
                /// For Token
                TokenHeroic = 570,
                TokenMythic = 569,
                /// For T17
                Heroic      = 566,
                Mythic      = 567
            };

            enum eSpellIDs
            {
                NormalQuestToken    = 178217,
                HeroicQuestToken    = 178225,
                MythicQuestToken    = 178227
            };

            void HandleDummy(SpellEffIndex /*p_EffIndex*/)
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Item* l_Item = GetCastItem())
                    {
                        uint32 l_ItemID = 0;
                        std::vector<uint32> l_Bonuses;

                        switch (l_Player->getClass())
                        {
                            case Classes::CLASS_PALADIN:
                                l_ItemID = eItemIDs::BattleplateOfGudingLight;
                                break;
                            case Classes::CLASS_WARLOCK:
                                l_ItemID = eItemIDs::ShadowCouncilsRobes;
                                break;
                            case Classes::CLASS_PRIEST:
                                l_ItemID = eItemIDs::SoulPriestsRaiment;
                                break;
                            case Classes::CLASS_MAGE:
                                l_ItemID = eItemIDs::ArcanoshatterRobes;
                                break;
                            case Classes::CLASS_DRUID:
                                l_ItemID = eItemIDs::LivingWoodRaiment;
                                break;
                            case Classes::CLASS_DEATH_KNIGHT:
                                l_ItemID = eItemIDs::OgreskullBoneplateBreastplate;
                                break;
                            case Classes::CLASS_ROGUE:
                                l_ItemID = eItemIDs::PoisonersTunic;
                                break;
                            case Classes::CLASS_WARRIOR:
                                l_ItemID = eItemIDs::BlackhandsChestguard;
                                break;
                            case Classes::CLASS_MONK:
                                l_ItemID = eItemIDs::VestOfTheSomberGaze;
                                break;
                            case Classes::CLASS_HUNTER:
                                l_ItemID = eItemIDs::RylakstalkersTunic;
                                break;
                            case Classes::CLASS_SHAMAN:
                                l_ItemID = eItemIDs::WindspeakersTunic;
                                break;
                            default:
                                break;
                        }

                        if (l_Item->HasItemBonus(eBonusIDs::TokenMythic))
                            l_Bonuses.push_back(eBonusIDs::Mythic);
                        else if (l_Item->HasItemBonus(eBonusIDs::TokenHeroic))
                            l_Bonuses.push_back(eBonusIDs::Heroic);

                        switch (GetSpellInfo()->Id)
                        {
                            case eSpellIDs::HeroicQuestToken:
                                l_Bonuses.push_back(eBonusIDs::Heroic);
                                break;
                            case eSpellIDs::MythicQuestToken:
                                l_Bonuses.push_back(eBonusIDs::Mythic);
                                break;
                            case eSpellIDs::NormalQuestToken:
                            default:
                                break;
                        }

                        if (l_ItemID)
                        {
                            /// Adding items
                            uint32 l_NoSpaceForCount = 0;
                            uint32 l_Count = 1;

                            /// check space and find places
                            ItemPosCountVec l_Dest;
                            InventoryResult l_MSG = l_Player->CanStoreNewItem(InventorySlot::NULL_BAG, InventorySlot::NULL_SLOT, l_Dest, l_ItemID, l_Count, &l_NoSpaceForCount);

                            /// Convert to possible store amount
                            if (l_MSG != InventoryResult::EQUIP_ERR_OK)
                                l_Count -= l_NoSpaceForCount;

                            /// Can't add any
                            if (l_Count == 0 || l_Dest.empty())
                                return;

                            if (Item* l_NewItem = l_Player->StoreNewItem(l_Dest, l_ItemID, true))
                            {
                                /// Context must be normal, difficulty bonus is already added
                                Item::GenerateItemBonus(l_ItemID, ItemContext::RaidNormal, l_Bonuses);

                                if (!l_Bonuses.empty())
                                    l_NewItem->AddItemBonuses(l_Bonuses);

                                l_Player->SendNewItem(l_NewItem, l_Count, false, true);
                            }
                        }
                    }
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_chest_of_iron_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_item_chest_of_iron_SpellScript();
        }
};

/// Helm of Iron (T17) - 178212
/// Helm of Iron (Normal - T17 - Quest) - 178216
/// Helm of Iron (Heroic - T17 - Quest) - 178224
/// Helm of Iron (Mythic - T17 - Quest) - 178226
class spell_item_helm_of_iron : public SpellScriptLoader
{
    public:
        spell_item_helm_of_iron() : SpellScriptLoader("spell_item_helm_of_iron") { }

        class spell_item_helm_of_iron_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_helm_of_iron_SpellScript);

            enum eItemIDs
            {
                HelmetOfGudingLight         = 115568,
                ShadowCouncilsHood          = 115586,
                SoulPriestsHood             = 115563,
                ArcanoshatterHood           = 115553,
                LivingWoodHeadpiece         = 115542,
                OgreskullBoneplateGreathelm = 115539,
                PoisonersHelmet             = 115572,
                BlackhandsFaceguard         = 115584,
                HelmOfTheSomberGaze         = 115556,
                RylakstalkersHeadguard      = 115545,
                WindspeakersFaceguard       = 115579
            };

            enum eBonusIDs
            {
                /// For Token
                TokenHeroic = 570,
                TokenMythic = 569,
                /// For T17
                Heroic      = 566,
                Mythic      = 567
            };

            enum eSpellIDs
            {
                NormalQuestToken    = 178216,
                HeroicQuestToken    = 178224,
                MythicQuestToken    = 178226
            };

            void HandleDummy(SpellEffIndex /*p_EffIndex*/)
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Item* l_Item = GetCastItem())
                    {
                        uint32 l_ItemID = 0;
                        std::vector<uint32> l_Bonuses;

                        switch (l_Player->getClass())
                        {
                            case Classes::CLASS_PALADIN:
                                l_ItemID = eItemIDs::HelmetOfGudingLight;
                                break;
                            case Classes::CLASS_WARLOCK:
                                l_ItemID = eItemIDs::ShadowCouncilsHood;
                                break;
                            case Classes::CLASS_PRIEST:
                                l_ItemID = eItemIDs::SoulPriestsHood;
                                break;
                            case Classes::CLASS_MAGE:
                                l_ItemID = eItemIDs::ArcanoshatterHood;
                                break;
                            case Classes::CLASS_DRUID:
                                l_ItemID = eItemIDs::LivingWoodHeadpiece;
                                break;
                            case Classes::CLASS_DEATH_KNIGHT:
                                l_ItemID = eItemIDs::OgreskullBoneplateGreathelm;
                                break;
                            case Classes::CLASS_ROGUE:
                                l_ItemID = eItemIDs::PoisonersHelmet;
                                break;
                            case Classes::CLASS_WARRIOR:
                                l_ItemID = eItemIDs::BlackhandsFaceguard;
                                break;
                            case Classes::CLASS_MONK:
                                l_ItemID = eItemIDs::HelmOfTheSomberGaze;
                                break;
                            case Classes::CLASS_HUNTER:
                                l_ItemID = eItemIDs::RylakstalkersHeadguard;
                                break;
                            case Classes::CLASS_SHAMAN:
                                l_ItemID = eItemIDs::WindspeakersFaceguard;
                                break;
                            default:
                                break;
                        }

                        if (l_Item->HasItemBonus(eBonusIDs::TokenMythic))
                            l_Bonuses.push_back(eBonusIDs::Mythic);
                        else if (l_Item->HasItemBonus(eBonusIDs::TokenHeroic))
                            l_Bonuses.push_back(eBonusIDs::Heroic);

                        switch (GetSpellInfo()->Id)
                        {
                            case eSpellIDs::HeroicQuestToken:
                                l_Bonuses.push_back(eBonusIDs::Heroic);
                                break;
                            case eSpellIDs::MythicQuestToken:
                                l_Bonuses.push_back(eBonusIDs::Mythic);
                                break;
                            case eSpellIDs::NormalQuestToken:
                            default:
                                break;
                        }

                        if (l_ItemID)
                        {
                            /// Adding items
                            uint32 l_NoSpaceForCount = 0;
                            uint32 l_Count = 1;

                            /// check space and find places
                            ItemPosCountVec l_Dest;
                            InventoryResult l_MSG = l_Player->CanStoreNewItem(InventorySlot::NULL_BAG, InventorySlot::NULL_SLOT, l_Dest, l_ItemID, l_Count, &l_NoSpaceForCount);

                            /// Convert to possible store amount
                            if (l_MSG != InventoryResult::EQUIP_ERR_OK)
                                l_Count -= l_NoSpaceForCount;

                            /// Can't add any
                            if (l_Count == 0 || l_Dest.empty())
                                return;

                            if (Item* l_NewItem = l_Player->StoreNewItem(l_Dest, l_ItemID, true))
                            {
                                /// Context must be normal, difficulty bonus is already added
                                Item::GenerateItemBonus(l_ItemID, ItemContext::RaidNormal, l_Bonuses);

                                if (!l_Bonuses.empty())
                                    l_NewItem->AddItemBonuses(l_Bonuses);

                                l_Player->SendNewItem(l_NewItem, l_Count, false, true);
                            }
                        }
                    }
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_helm_of_iron_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_item_helm_of_iron_SpellScript();
        }
};

/// Shoulders of Iron (T17) - 178213
/// Shoulders of Iron (Normal - T17 - Quest) - 178220
/// Shoulders of Iron (Heroic - T17 - Quest) - 178223
/// Shoulders of Iron (Mythic - T17 - Quest) - 178230
class spell_item_shoulders_of_iron : public SpellScriptLoader
{
    public:
        spell_item_shoulders_of_iron() : SpellScriptLoader("spell_item_shoulders_of_iron") { }

        class spell_item_shoulders_of_iron_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_shoulders_of_iron_SpellScript);

            enum eItemIDs
            {
                PauldronsOfGudingLight      = 115565,
                ShadowCouncilsMantle        = 115589,
                SoulPriestsShoulderguards   = 115561,
                ArcanoshatterMantle         = 115551,
                LivingWoodSpaulders         = 115544,
                OgreskullBoneplatePauldrons = 115536,
                PoisonersSpaulders          = 115574,
                BlackhandsShoulderguards    = 115581,
                MantleOfTheSomberGaze       = 115559,
                RylakstalkersSpaulders      = 115547,
                WindspeakersMantle          = 115579
            };

            enum eBonusIDs
            {
                /// For Token
                TokenHeroic = 570,
                TokenMythic = 569,
                /// For T17
                Heroic      = 566,
                Mythic      = 567
            };

            enum eSpellIDs
            {
                NormalQuestToken    = 178220,
                HeroicQuestToken    = 178223,
                MythicQuestToken    = 178230
            };

            void HandleDummy(SpellEffIndex /*p_EffIndex*/)
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Item* l_Item = GetCastItem())
                    {
                        uint32 l_ItemID = 0;
                        std::vector<uint32> l_Bonuses;

                        switch (l_Player->getClass())
                        {
                            case Classes::CLASS_PALADIN:
                                l_ItemID = eItemIDs::PauldronsOfGudingLight;
                                break;
                            case Classes::CLASS_WARLOCK:
                                l_ItemID = eItemIDs::ShadowCouncilsMantle;
                                break;
                            case Classes::CLASS_PRIEST:
                                l_ItemID = eItemIDs::SoulPriestsShoulderguards;
                                break;
                            case Classes::CLASS_MAGE:
                                l_ItemID = eItemIDs::ArcanoshatterMantle;
                                break;
                            case Classes::CLASS_DRUID:
                                l_ItemID = eItemIDs::LivingWoodSpaulders;
                                break;
                            case Classes::CLASS_DEATH_KNIGHT:
                                l_ItemID = eItemIDs::OgreskullBoneplatePauldrons;
                                break;
                            case Classes::CLASS_ROGUE:
                                l_ItemID = eItemIDs::PoisonersSpaulders;
                                break;
                            case Classes::CLASS_WARRIOR:
                                l_ItemID = eItemIDs::BlackhandsShoulderguards;
                                break;
                            case Classes::CLASS_MONK:
                                l_ItemID = eItemIDs::MantleOfTheSomberGaze;
                                break;
                            case Classes::CLASS_HUNTER:
                                l_ItemID = eItemIDs::RylakstalkersSpaulders;
                                break;
                            case Classes::CLASS_SHAMAN:
                                l_ItemID = eItemIDs::WindspeakersMantle;
                                break;
                            default:
                                break;
                        }

                        if (l_Item->HasItemBonus(eBonusIDs::TokenMythic))
                            l_Bonuses.push_back(eBonusIDs::Mythic);
                        else if (l_Item->HasItemBonus(eBonusIDs::TokenHeroic))
                            l_Bonuses.push_back(eBonusIDs::Heroic);

                        switch (GetSpellInfo()->Id)
                        {
                            case eSpellIDs::HeroicQuestToken:
                                l_Bonuses.push_back(eBonusIDs::Heroic);
                                break;
                            case eSpellIDs::MythicQuestToken:
                                l_Bonuses.push_back(eBonusIDs::Mythic);
                                break;
                            case eSpellIDs::NormalQuestToken:
                            default:
                                break;
                        }

                        if (l_ItemID)
                        {
                            /// Adding items
                            uint32 l_NoSpaceForCount = 0;
                            uint32 l_Count = 1;

                            /// check space and find places
                            ItemPosCountVec l_Dest;
                            InventoryResult l_MSG = l_Player->CanStoreNewItem(InventorySlot::NULL_BAG, InventorySlot::NULL_SLOT, l_Dest, l_ItemID, l_Count, &l_NoSpaceForCount);

                            /// Convert to possible store amount
                            if (l_MSG != InventoryResult::EQUIP_ERR_OK)
                                l_Count -= l_NoSpaceForCount;

                            /// Can't add any
                            if (l_Count == 0 || l_Dest.empty())
                                return;

                            if (Item* l_NewItem = l_Player->StoreNewItem(l_Dest, l_ItemID, true))
                            {
                                /// Context must be normal, difficulty bonus is already added
                                Item::GenerateItemBonus(l_ItemID, ItemContext::RaidNormal, l_Bonuses);

                                if (!l_Bonuses.empty())
                                    l_NewItem->AddItemBonuses(l_Bonuses);

                                l_Player->SendNewItem(l_NewItem, l_Count, false, true);
                            }
                        }
                    }
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_shoulders_of_iron_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_item_shoulders_of_iron_SpellScript();
        }
};

/// Gauntlets of Iron (T17) - 178211
/// Gauntlets of Iron (Normal - T17 - Quest) - 178219
/// Gauntlets of Iron (Heroic - T17 - Quest) - 178222
/// Gauntlets of Iron (Mythic - T17 - Quest) - 178229
class spell_item_gauntlets_of_iron : public SpellScriptLoader
{
    public:
        spell_item_gauntlets_of_iron() : SpellScriptLoader("spell_item_gauntlets_of_iron") { }

        class spell_item_gauntlets_of_iron_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_gauntlets_of_iron_SpellScript);

            enum eItemIDs
            {
                GauntletsOfGudingLight      = 115567,
                ShadowCouncilsGloves        = 115585,
                SoulPriestsGloves           = 115562,
                ArcanoshatterGloves         = 115552,
                LivingWoodGrips             = 115541,
                OgreskullBoneplateGauntlets = 115538,
                PoisonersGloves             = 115571,
                BlackhandsHandguards        = 115583,
                HandwrapsOfTheSomberGaze    = 115555,
                RylakstalkersGloves         = 115549,
                WindspeakersHandwraps       = 115578
            };

            enum eBonusIDs
            {
                /// For Token
                TokenHeroic = 570,
                TokenMythic = 569,
                /// For T17
                Heroic      = 566,
                Mythic      = 567
            };

            enum eSpellIDs
            {
                NormalQuestToken    = 178219,
                HeroicQuestToken    = 178222,
                MythicQuestToken    = 178229
            };

            void HandleDummy(SpellEffIndex /*p_EffIndex*/)
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Item* l_Item = GetCastItem())
                    {
                        uint32 l_ItemID = 0;
                        std::vector<uint32> l_Bonuses;

                        switch (l_Player->getClass())
                        {
                            case Classes::CLASS_PALADIN:
                                l_ItemID = eItemIDs::GauntletsOfGudingLight;
                                break;
                            case Classes::CLASS_WARLOCK:
                                l_ItemID = eItemIDs::ShadowCouncilsGloves;
                                break;
                            case Classes::CLASS_PRIEST:
                                l_ItemID = eItemIDs::SoulPriestsGloves;
                                break;
                            case Classes::CLASS_MAGE:
                                l_ItemID = eItemIDs::ArcanoshatterGloves;
                                break;
                            case Classes::CLASS_DRUID:
                                l_ItemID = eItemIDs::LivingWoodGrips;
                                break;
                            case Classes::CLASS_DEATH_KNIGHT:
                                l_ItemID = eItemIDs::OgreskullBoneplateGauntlets;
                                break;
                            case Classes::CLASS_ROGUE:
                                l_ItemID = eItemIDs::PoisonersGloves;
                                break;
                            case Classes::CLASS_WARRIOR:
                                l_ItemID = eItemIDs::BlackhandsHandguards;
                                break;
                            case Classes::CLASS_MONK:
                                l_ItemID = eItemIDs::HandwrapsOfTheSomberGaze;
                                break;
                            case Classes::CLASS_HUNTER:
                                l_ItemID = eItemIDs::RylakstalkersGloves;
                                break;
                            case Classes::CLASS_SHAMAN:
                                l_ItemID = eItemIDs::WindspeakersHandwraps;
                                break;
                            default:
                                break;
                        }

                        if (l_Item->HasItemBonus(eBonusIDs::TokenMythic))
                            l_Bonuses.push_back(eBonusIDs::Mythic);
                        else if (l_Item->HasItemBonus(eBonusIDs::TokenHeroic))
                            l_Bonuses.push_back(eBonusIDs::Heroic);

                        switch (GetSpellInfo()->Id)
                        {
                            case eSpellIDs::HeroicQuestToken:
                                l_Bonuses.push_back(eBonusIDs::Heroic);
                                break;
                            case eSpellIDs::MythicQuestToken:
                                l_Bonuses.push_back(eBonusIDs::Mythic);
                                break;
                            case eSpellIDs::NormalQuestToken:
                            default:
                                break;
                        }

                        if (l_ItemID)
                        {
                            /// Adding items
                            uint32 l_NoSpaceForCount = 0;
                            uint32 l_Count = 1;

                            /// check space and find places
                            ItemPosCountVec l_Dest;
                            InventoryResult l_MSG = l_Player->CanStoreNewItem(InventorySlot::NULL_BAG, InventorySlot::NULL_SLOT, l_Dest, l_ItemID, l_Count, &l_NoSpaceForCount);

                            /// Convert to possible store amount
                            if (l_MSG != InventoryResult::EQUIP_ERR_OK)
                                l_Count -= l_NoSpaceForCount;

                            /// Can't add any
                            if (l_Count == 0 || l_Dest.empty())
                                return;

                            if (Item* l_NewItem = l_Player->StoreNewItem(l_Dest, l_ItemID, true))
                            {
                                /// Context must be normal, difficulty bonus is already added
                                Item::GenerateItemBonus(l_ItemID, ItemContext::RaidNormal, l_Bonuses);

                                if (!l_Bonuses.empty())
                                    l_NewItem->AddItemBonuses(l_Bonuses);

                                l_Player->SendNewItem(l_NewItem, l_Count, false, true);
                            }
                        }
                    }
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_gauntlets_of_iron_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_item_gauntlets_of_iron_SpellScript();
        }
};

/// Create Ring - 168103
/// Create Glove - 168115
/// Create Weapon - 168700
/// Create Trinket - 168701
/// Create Belt - 168705
/// Create Shoulders - 168706
/// Create Legs - 168707
/// Create Helm - 168708
/// Create Glove - 168709
/// Create Boot - 168710
/// Create Bracer - 168712
/// Create Weapon - 168713
/// Create Trinket - 168714
/// Create Neck - 168716
/// Create Shoulders - 168719
/// Create Boot - 168723
/// Create Chest - 168724
/// Create Bracer - 168725
/// Create Weapon - 168726
/// Create Trinket - 168727
/// Create Neck - 168729
/// Create Shoulders - 168732
/// Create Boot - 168736
/// Create Chest - 168737
/// Create Bracer - 168738
/// Create Weapon - 168739
/// Create Trinket - 168740
/// Create Neck - 168742
/// Create Shoulders - 168745
/// Create Boot - 168749
/// Create Chest - 168750
/// Create Bracer - 168751
/// Create Trinket - 168753
/// Create Trinket - 181732
class spell_create_reward_item : public SpellScriptLoader
{
    public:
        spell_create_reward_item() : SpellScriptLoader("spell_create_reward_item") { }

        class spell_create_reward_item_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_create_reward_item_SpellScript);

            enum eBonuses
            {
                Warforged       = 560,
                Avoidance       = 40,
                Indestructible  = 43,
                Leech           = 41,
                Speed           = 42,
                PrismaticSocket = 563
            };

            uint32 m_ItemID;

            bool Load() override
            {
                m_ItemID = 0;
                return true;
            }

            SpellCastResult CheckCast()
            {
                Player* l_Player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
                if (!l_Player)
                    return SpellCastResult::SPELL_FAILED_ERROR;

                LootStore& l_LootStore = LootTemplates_Spell;
                LootTemplate const* l_LootTemplate = l_LootStore.GetLootFor(GetSpellInfo()->Id);
                if (l_LootTemplate == nullptr)
                    return SpellCastResult::SPELL_FAILED_ERROR;

                std::list<ItemTemplate const*> l_LootTable;
                std::vector<uint32> l_Items;
                l_LootTemplate->FillAutoAssignationLoot(l_LootTable, l_Player, false);

                if (l_LootTable.empty())
                    return SpellCastResult::SPELL_FAILED_ERROR;

                uint32 l_SpecID = l_Player->GetLootSpecId() ? l_Player->GetLootSpecId() : l_Player->GetSpecializationId(l_Player->GetActiveSpec());

                for (ItemTemplate const* l_Template : l_LootTable)
                {
                    if (l_Template->HasSpec((SpecIndex)l_SpecID, l_Player->getLevel()))
                        l_Items.push_back(l_Template->ItemId);
                }

                if (l_Items.empty())
                    return SpellCastResult::SPELL_FAILED_ERROR;

                std::random_shuffle(l_Items.begin(), l_Items.end());

                m_ItemID = l_Items[0];

                ItemPosCountVec l_Destination;
                InventoryResult l_Message = l_Player->CanStoreNewItem(InventorySlot::NULL_BAG, InventorySlot::NULL_SLOT, l_Destination, m_ItemID, 1);

                if (l_Message != InventoryResult::EQUIP_ERR_OK)
                {
                    l_Player->SendEquipError(InventoryResult::EQUIP_ERR_INV_FULL, nullptr);
                    return SpellCastResult::SPELL_FAILED_DONT_REPORT;
                }

                return SpellCastResult::SPELL_CAST_OK;
            }

            void HandleCreateItem(SpellEffIndex p_EffIndex)
            {
                PreventHitDefaultEffect(p_EffIndex);

                Player* l_Player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
                if (!l_Player)
                    return;

                ItemPosCountVec l_Destination;
                InventoryResult l_Message = l_Player->CanStoreNewItem(InventorySlot::NULL_BAG, InventorySlot::NULL_SLOT, l_Destination, m_ItemID, 1);

                if (l_Message != InventoryResult::EQUIP_ERR_OK)
                    return;

                if (Item* l_Item = l_Player->StoreNewItem(l_Destination, m_ItemID, true, Item::GenerateItemRandomPropertyId(m_ItemID)))
                {
                    /// Some of created items have already secondary stats, but only with item level <= 519
                    if (l_Item->GetTemplate()->ItemLevel > 519)
                    {
                        std::vector<uint32> l_BonusList;

                        Item::GenerateItemBonus(m_ItemID, ItemContext::RaidNormal, l_BonusList);

                        std::vector<uint32> l_RandBonus = { eBonuses::Warforged, eBonuses::Avoidance, eBonuses::Indestructible, eBonuses::Leech, eBonuses::Speed, eBonuses::PrismaticSocket };

                        for (uint32 l_Bonus : l_RandBonus)
                        {
                            if (roll_chance_i(10))
                                l_BonusList.push_back(l_Bonus);
                        }

                        for (uint32 l_Bonus : l_BonusList)
                            l_Item->AddItemBonus(l_Bonus);
                    }

                    l_Player->SendNewItem(l_Item, 1, false, true);
                    l_Player->SendDisplayToast(m_ItemID, 1, DisplayToastMethod::DISPLAY_TOAST_METHOD_LOOT, ToastTypes::TOAST_TYPE_NEW_ITEM, false, false);
                }
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_create_reward_item_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_create_reward_item_SpellScript::HandleCreateItem, EFFECT_0, SPELL_EFFECT_CREATE_ITEM_2);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_create_reward_item_SpellScript();
        }
};

/// Ai-Li's Skymirror - 86589, Called by: 129803
class spell_item_skymirror_image : public SpellScriptLoader
{
public:
    spell_item_skymirror_image() : SpellScriptLoader("spell_item_skymirror_image") { }

    class spell_item_skymirror_image_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_skymirror_image_SpellScript);

        enum eSpells
        {
            SkymirrorImage = 127315,
        };

        void HandleDummy()
        {
            Unit* l_Caster = GetCaster();
            if (l_Caster == nullptr)
                return;

            Player* l_Player = l_Caster->ToPlayer();
            if (l_Player == nullptr)
                return;

            if (Unit* l_Target = l_Player->GetSelectedPlayer())
                l_Target->CastSpell(l_Player, eSpells::SkymirrorImage, true);
            else
                l_Player->CastSpell(l_Player, eSpells::SkymirrorImage);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_item_skymirror_image_SpellScript::HandleDummy);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_skymirror_image_SpellScript();
    }
};

/// Memory of Mr. Smite - 127207. Item - Mr. Smite's Brass Compass - 86568
class spell_item_memory_of_mr_smite : public SpellScriptLoader
{
    public:
        spell_item_memory_of_mr_smite() : SpellScriptLoader("spell_item_memory_of_mr_smite") { }

        class spell_item_memory_of_mr_smite_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_memory_of_mr_smite_AuraScript);

            enum eSpells
            {
                MrSmiteSword = 127196,
                MrSmiteHummer = 127206,
                MrSmiteBlade = 127205
            };

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                uint32 l_MrSmiteId;
                l_MrSmiteId = (urand(0, 2));
                switch (l_MrSmiteId)
                {
                    case 0:
                        l_MrSmiteId = eSpells::MrSmiteSword;
                        break;
                    case 1:
                        l_MrSmiteId = eSpells::MrSmiteHummer;
                     break;
                    case 2:
                        l_MrSmiteId = eSpells::MrSmiteBlade;
                        break;
                    default:
                        return;
                }

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                l_Caster->CastSpell(l_Caster, l_MrSmiteId, true);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                if (l_Caster->HasAura(eSpells::MrSmiteSword))
                    l_Caster->RemoveAura(eSpells::MrSmiteSword);
                else if (l_Caster->HasAura(eSpells::MrSmiteHummer))
                    l_Caster->RemoveAura(eSpells::MrSmiteHummer);
                else if (l_Caster->HasAura(eSpells::MrSmiteBlade))
                    l_Caster->RemoveAura(eSpells::MrSmiteBlade);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_item_memory_of_mr_smite_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_item_memory_of_mr_smite_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_item_memory_of_mr_smite_AuraScript();
        }
};

/// Celestial Defender's Medallion - 103685, Called by: 149228
class spell_item_celestial_defender : public SpellScriptLoader
{
    public:
        spell_item_celestial_defender() : SpellScriptLoader("spell_item_celestial_defender") { }

        class spell_item_celestial_defender_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_celestial_defender_SpellScript);

            enum eSpells
            {
                CelestialDefenderMale = 148369,
                CelestialDefenderFemale = 149229
            };

            void HandleDummy(SpellEffIndex /* index */)
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                if (l_Caster->getGender() == GENDER_MALE)
                    l_Caster->CastSpell(l_Caster, eSpells::CelestialDefenderMale, true);
                else if (l_Caster->getGender() == GENDER_FEMALE)
                    l_Caster->CastSpell(l_Caster, eSpells::CelestialDefenderFemale, true);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_celestial_defender_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_celestial_defender_SpellScript();
        }
};

/// Curious Bronze Timepiece - 104113, Called by: 147988
class spell_item_curious_bronze_timepiece_alliance : public SpellScriptLoader
{
    public:
        spell_item_curious_bronze_timepiece_alliance() : SpellScriptLoader("spell_item_curious_bronze_timepiece_alliance") { }

        class spell_item_curious_bronze_timepiece_alliance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_curious_bronze_timepiece_alliance_SpellScript);

            void HandleDummy()
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (l_Player == nullptr)
                    return;

                l_Player->TeleportTo(870, -906.980957f, -4643.734375f, 5.223629f, 2.934408f);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_item_curious_bronze_timepiece_alliance_SpellScript::HandleDummy);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_curious_bronze_timepiece_alliance_SpellScript();
        }
};

/// Curious Bronze Timepiece - 104110, Called by: 147985
class spell_item_curious_bronze_timepiece_horde : public SpellScriptLoader
{
    public:
        spell_item_curious_bronze_timepiece_horde() : SpellScriptLoader("spell_item_curious_bronze_timepiece_horde") { }

        class spell_item_curious_bronze_timepiece_horde_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_curious_bronze_timepiece_horde_SpellScript);

            void HandleDummy()
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (l_Player == nullptr)
                    return;

                l_Player->TeleportTo(870, -420.822906f, -4604.593262f, 3.901290f, 5.138229f);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_item_curious_bronze_timepiece_horde_SpellScript::HandleDummy);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_curious_bronze_timepiece_horde_SpellScript();
        }
};

/// Bladespire Relic - 118662, Called by: 175604
class spell_item_ascend_to_bladespire : public SpellScriptLoader
{
public:
    spell_item_ascend_to_bladespire() : SpellScriptLoader("spell_item_ascend_to_bladespire") { }

    class spell_item_ascend_to_bladespire_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_ascend_to_bladespire_SpellScript);

        void HandleDummy()
        {
            Unit* l_Caster = GetCaster();
            if (l_Caster == nullptr)
                return;

            Player* l_Player = l_Caster->ToPlayer();
            if (l_Player == nullptr)
                return;

            l_Player->TeleportTo(1116, 6802.729004f, 5861.151855f, 258.688385f, 5.067857f);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_item_ascend_to_bladespire_SpellScript::HandleDummy);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_ascend_to_bladespire_SpellScript();
    }
};

/// Faded Wizard Hat - 53057, Called by: 74589
class spell_item_faded_wizard_hat : public SpellScriptLoader
{
public:
    spell_item_faded_wizard_hat() : SpellScriptLoader("spell_item_faded_wizard_hat") { }

    class spell_item_faded_wizard_hat_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_item_faded_wizard_hat_AuraScript);

        enum eSpells
        {
            GoblinMaleOne = 30089,
            GoblinMaleTwo = 30096,
            GoblinFemaleOne = 30084,
            GoblinFemaleTwo = 29907,
            BloodElfMaleOne = 19840,
            BloodElfMaleTwo = 30085,
            BloodElfMaleThree = 30086,
            BloodElfFemale = 29909,
            MaleUndead = 30094,
            FemaleUndead = 30093,
            MaleOrc = 29908,
            FemaleTroll = 30088,
            MaleHuman = 7614,
            FemaleHuman = 30092,
            MaleGnome = 30095,
            FemaleNightElf = 11670,
        };

        void OnApply(AuraEffect const*, AuraEffectHandleModes /*mode*/)
        {
            Unit* l_Caster = GetCaster();
            if (l_Caster == nullptr)
                return;

            AuraEffect* l_AuraEffect = GetEffect(0);
            if (l_AuraEffect == nullptr)
                return;

            uint32 l_BaseDisplayId = l_AuraEffect->GetAmount();
            if (l_BaseDisplayId != 0)
                l_Caster->SetDisplayId(l_BaseDisplayId);
            else
            {
                uint32 l_DisplayId;
                l_DisplayId = (urand(0, 15));
                switch (l_DisplayId)
                {
                    case 0:  l_DisplayId = eSpells::GoblinMaleOne;
                        break;
                    case 1:  l_DisplayId = eSpells::GoblinMaleTwo;
                        break;
                    case 2:  l_DisplayId = eSpells::GoblinFemaleOne;
                        break;
                    case 3:  l_DisplayId = eSpells::GoblinFemaleTwo;
                        break;
                    case 4:  l_DisplayId = eSpells::BloodElfMaleOne;
                        break;
                    case 5:  l_DisplayId = eSpells::BloodElfMaleTwo;
                        break;
                    case 6:  l_DisplayId = eSpells::BloodElfMaleThree;
                        break;
                    case 7:  l_DisplayId = eSpells::BloodElfFemale;
                        break;
                    case 8:  l_DisplayId = eSpells::MaleUndead;
                        break;
                    case 9:  l_DisplayId = eSpells::FemaleUndead;
                        break;
                    case 10:  l_DisplayId = eSpells::MaleOrc;
                        break;
                    case 11:  l_DisplayId = eSpells::FemaleTroll;
                        break;
                    case 12:  l_DisplayId = eSpells::MaleHuman;
                        break;
                    case 13:  l_DisplayId = eSpells::FemaleHuman;
                        break;
                    case 14:  l_DisplayId = eSpells::MaleGnome;
                        break;
                    case 15:  l_DisplayId = eSpells::FemaleNightElf;
                        break;
                    default:
                        return;
                }

                if (!l_DisplayId)
                    return;

                l_AuraEffect->SetAmount(l_DisplayId);
                l_Caster->SetDisplayId(l_AuraEffect->GetAmount());
            }
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_item_faded_wizard_hat_AuraScript::OnApply, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_item_faded_wizard_hat_AuraScript();
    }
};

/// Reflecting Prism - 163219
class spell_item_reflecting_prism : public SpellScriptLoader
{
public:
    spell_item_reflecting_prism() : SpellScriptLoader("spell_item_reflecting_prism") { }

    class spell_item_reflecting_prism_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_reflecting_prism_SpellScript);

        enum eSpells
        {
            PrismaticReflection = 163267
        };

        void HandleDummy()
        {
            Unit* l_Caster = GetCaster();
            if (l_Caster == nullptr)
                return;

            Player* l_Player = l_Caster->ToPlayer();
            if (l_Player == nullptr)
                return;

            if (Unit* l_Target = l_Player->GetSelectedPlayer())
                l_Target->CastSpell(l_Player, eSpells::PrismaticReflection, true);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_item_reflecting_prism_SpellScript::HandleDummy);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_reflecting_prism_SpellScript();
    }
};

/// Relic of Karabor - 118663, Called by: 175608
class spell_item_ascend_to_karabor : public SpellScriptLoader
{
public:
    spell_item_ascend_to_karabor() : SpellScriptLoader("spell_item_ascend_to_karabor") { }

    class spell_item_ascend_to_karabor_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_ascend_to_karabor_SpellScript);

        void HandleDummy()
        {
            Unit* l_Caster = GetCaster();
            if (l_Caster == nullptr)
                return;

            Player* l_Player = l_Caster->ToPlayer();
            if (l_Player == nullptr)
                return;

            l_Player->TeleportTo(1191, 578.738037f, -2476.123779f, 96.248749f, 4.711049f);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_item_ascend_to_karabor_SpellScript::HandleDummy);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_ascend_to_karabor_SpellScript();
    }
};

/// Sargerei Disguise - 119134, Called by: 176567
class spell_item_sargerei_disguise : public SpellScriptLoader
{
public:
    spell_item_sargerei_disguise() : SpellScriptLoader("spell_item_sargerei_disguise") { }

    class spell_item_sargerei_disguise_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_sargerei_disguise_SpellScript);

        enum eSpells
        {
            SargereiDisguise = 176568
        };

        void AfterCast()
        {
            Unit* l_Caster = GetCaster();
            if (l_Caster == nullptr)
                return;

            l_Caster->CastSpell(l_Caster, eSpells::SargereiDisguise, true);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_item_sargerei_disguise_SpellScript::AfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_sargerei_disguise_SpellScript();
    }
};

/// Swapblaster - 111820, Called by: 161399
class spell_item_swapblaster : public SpellScriptLoader
{
public:
    spell_item_swapblaster() : SpellScriptLoader("spell_item_swapblaster") { }

    class spell_item_swapblaster_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_swapblaster_SpellScript);

        void HandleDummy()
        {
            Unit* l_Caster = GetCaster();
            if (l_Caster == nullptr)
                return;

            Player* l_Player = l_Caster->ToPlayer();
            if (l_Player == nullptr)
                return;

            Player* l_Target = l_Player->GetSelectedPlayer();
            if (l_Target == nullptr)
                return;

            l_Player->TeleportTo(l_Target->GetMapId(), l_Target->GetPositionX(), l_Target->GetPositionY(), l_Target->GetPositionZ(), l_Target->GetOrientation());
            l_Target->TeleportTo(l_Player->GetMapId(), l_Player->GetPositionX(), l_Player->GetPositionY(), l_Player->GetPositionZ(), l_Player->GetOrientation());

        }

        void Register()
        {
            OnHit += SpellHitFn(spell_item_swapblaster_SpellScript::HandleDummy);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_swapblaster_SpellScript();
    }
};


/// Transmorphic Tincture - 112090, Called by: 162313
class spell_item_transmorphic_tincture : public SpellScriptLoader
{
    public:
    spell_item_transmorphic_tincture() : SpellScriptLoader("spell_item_transmorphic_tincture") { }

    class  spell_item_transmorphic_tincture_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_item_transmorphic_tincture_AuraScript);

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* l_Player = GetTarget()->ToPlayer();
            if (l_Player == nullptr)
                return;

            AuraEffect* l_AuraEffect = GetEffect(0);
            if (l_AuraEffect == nullptr)
                return;

            Gender l_NewGender = (Gender)l_Player->getGender();

            /// This need to understand if player already have this buff (for example after logout), to prevent change of the gender one more time
            uint32 l_NativeGender = l_AuraEffect->GetAmount();
            if (l_NativeGender == 0)
                l_AuraEffect->SetAmount(l_NewGender + 1);

            uint32 l_OtherGender = l_AuraEffect->GetAmount() - 1;
            if (l_OtherGender == GENDER_MALE)            ///< MALE
                l_NewGender = GENDER_FEMALE;
            else if (l_OtherGender == GENDER_FEMALE)     ///< FEMALE
                l_NewGender = GENDER_MALE;
            else
                return;

            /// Set new gender
            l_Player->SetByteValue(UNIT_FIELD_SEX, 3, l_NewGender);
            l_Player->SetByteValue(PLAYER_FIELD_ARENA_FACTION, 0, l_NewGender);

            PlayerInfo const* l_Info = sObjectMgr->GetPlayerInfo(l_Player->getRace(), l_Player->getClass());
            if (!l_Info)
                return;
            uint16 l_NewDisplayId = 0;

            if (l_NewGender == GENDER_MALE)
                l_NewDisplayId = l_Info->displayId_m;
            else if (l_NewGender == GENDER_FEMALE)
                l_NewDisplayId = l_Info->displayId_f;

            /// Check if this is first time, our player already has this aura
            /// If aura has applied less then 3 seconds ago - just casted
            bool l_FirstTime = false;
            if (Aura* l_Transmorphed = l_Player->GetAura(GetSpellInfo()->Id))
                if (l_Transmorphed->GetDuration() > (l_Transmorphed->GetMaxDuration() - 500))
                    l_FirstTime = true;

            /// Check if at the moment player has the same model as his native
            bool l_CurrentModelEqualNative = l_Player->GetNativeDisplayId() == l_Player->GetDisplayId();

            /// If not in original form (for example cat,bear,metamorphosis), we don't need to change displayId now
            if ((!l_FirstTime && !l_CurrentModelEqualNative) || l_CurrentModelEqualNative)
                l_Player->InitDisplayIds();
            else
                l_Player->SetNativeDisplayId(l_NewDisplayId);
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* l_Player = GetTarget()->ToPlayer();
            if (l_Player == nullptr)
                return;

            AuraEffect* l_AuraEffect = GetEffect(0);
            if (l_AuraEffect == nullptr)
                return;

            uint32 l_NativeGender = l_AuraEffect->GetAmount() - 1;

            l_Player->SetByteValue(UNIT_FIELD_SEX, 3, l_NativeGender);
            l_Player->SetByteValue(PLAYER_FIELD_ARENA_FACTION, 0, l_NativeGender);


            PlayerInfo const* l_Info = sObjectMgr->GetPlayerInfo(l_Player->getRace(), l_Player->getClass());
            if (!l_Info)
                return;
            uint16 l_NativeDisplayId = 0;

            if (l_NativeGender == GENDER_MALE)
                l_NativeDisplayId = l_Info->displayId_m;
            else if (l_NativeGender == GENDER_FEMALE)
                l_NativeDisplayId = l_Info->displayId_f;

            /// If not in original form (for example cat,bear,metamorphosis), we don't need to change displayId now
            if (l_Player->GetNativeDisplayId() == l_Player->GetDisplayId())
                l_Player->InitDisplayIds();
            else
                l_Player->SetNativeDisplayId(l_NativeDisplayId);
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_item_transmorphic_tincture_AuraScript::OnApply, EFFECT_0, SPELL_AURA_TRANSFORM, AuraEffectHandleModes(AURA_EFFECT_HANDLE_REAL | AURA_EFFECT_HANDLE_SEND_FOR_CLIENT));
            AfterEffectRemove += AuraEffectRemoveFn(spell_item_transmorphic_tincture_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_item_transmorphic_tincture_AuraScript();
    }
};


/// Hypnosis Goggles - 113631, Called by: 167839
class spell_item_hypnotize_critter : public SpellScriptLoader
{
    public:
        spell_item_hypnotize_critter() : SpellScriptLoader("spell_item_hypnotize_critter") { }

        class spell_item_hypnotize_critter_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_hypnotize_critter_SpellScript);

            void HandleDummy()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                Unit* l_Critter = GetHitUnit();

                if (l_Player == nullptr || l_Critter == nullptr)
                    return;

                if (l_Critter->GetMotionMaster())
                    l_Critter->GetMotionMaster()->MoveFollow(l_Player, 0.0f, PET_FOLLOW_ANGLE, MOTION_SLOT_IDLE);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_item_hypnotize_critter_SpellScript::HandleDummy);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_hypnotize_critter_SpellScript();
        }
};

class spell_item_ghostly_backfire : public SpellScriptLoader
{
    public:
        spell_item_ghostly_backfire() : SpellScriptLoader("spell_item_ghostly_backfire") {}

        class spell_item_ghostly_backfire_SpellScript : public SpellScript
        {
            enum Creatures
            {
                NPC_DOZER = 35558
            };

            PrepareSpellScript(spell_item_ghostly_backfire_SpellScript);

            SpellCastResult CheckRequirement()
            {
                if (GetCaster()->FindNearestCreature(NPC_DOZER, 20.0f) != nullptr)
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_CANT_USE_THAT_ITEM);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_item_ghostly_backfire_SpellScript::CheckRequirement);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_ghostly_backfire_SpellScript();
        }
};

/// Blessing of A'dal 175442
class spell_item_blessing_of_adal : public SpellScriptLoader
{
    public:
        spell_item_blessing_of_adal() : SpellScriptLoader("spell_item_blessing_of_adal") { }

        class spell_item_blessing_of_adal_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_blessing_of_adal_AuraScript);

            void OnUpdate(uint32 /*p_Diff*/)
            {
                if (Unit* l_Target = GetUnitOwner())
                {
                    if (l_Target->GetMapId() != 1116)
                        Remove();
                }
            }

            void Register() override
            {
                OnAuraUpdate += AuraUpdateFn(spell_item_blessing_of_adal_AuraScript::OnUpdate);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_item_blessing_of_adal_AuraScript();
        }
};

/// Enduring Elixir of Wisdom - 146939 (item 103557)
class spell_item_enduring_elixir_of_wisdom : public SpellScriptLoader
{
    public:
        spell_item_enduring_elixir_of_wisdom() : SpellScriptLoader("spell_item_enduring_elixir_of_wisdom") { }

        class spell_item_enduring_elixir_of_wisdom_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_enduring_elixir_of_wisdom_SpellScript);

            enum eSpells
            {
                EnduringElixirOfWisdomBonus = 146929
            };

            void AfterCast()
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                l_Caster->CastSpell(l_Caster, eSpells::EnduringElixirOfWisdomBonus, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_item_enduring_elixir_of_wisdom_SpellScript::AfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_enduring_elixir_of_wisdom_SpellScript();
        }
};

/// Spike-Toed Booterang - 129295
/// Called by Spike-Toed Booterang - 193832
class spell_item_spike_toed_booterang : public SpellScriptLoader
{
    public:
        spell_item_spike_toed_booterang() : SpellScriptLoader("spell_item_spike_toed_booterang") { }

        class spell_item_spike_toed_booterang_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_spike_toed_booterang_SpellScript);

            SpellCastResult CheckRequirement()
            {
                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Player == nullptr)
                    return SPELL_FAILED_DONT_REPORT;

                if (Unit* l_Target = l_Player->GetSelectedUnit())
                {
                    if (l_Target->getLevel() > 100)
                        return SPELL_FAILED_BAD_TARGETS;
                }

                return SPELL_CAST_OK;
            }

            void HandleDummy()
            {
                Unit* l_Target = GetHitUnit();
                if (l_Target == nullptr)
                    return;

                if (l_Target->IsMounted())
                    l_Target->RemoveAurasByType(SPELL_AURA_MOUNTED);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_item_spike_toed_booterang_SpellScript::CheckRequirement);
                AfterHit += SpellHitFn(spell_item_spike_toed_booterang_SpellScript::HandleDummy);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_spike_toed_booterang_SpellScript();
        }
};

/// Magic Pet Mirror - 127696
/// Called by Mystic Image - 187356
class spell_item_mystic_image : public SpellScriptLoader
{
    public:
        spell_item_mystic_image() : SpellScriptLoader("spell_item_mystic_image") { }

        class spell_item_mystic_image_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_mystic_image_SpellScript);

            enum eSpells
            {
                MysticImage = 187358
            };

            void HandleDummy()
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (l_Player == nullptr)
                    return;

                Unit* l_Target = l_Player->GetSelectedUnit();   ///< Our minipet
                if (l_Target == nullptr)
                    return;

                l_Target->CastSpell(l_Player, eSpells::MysticImage, true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_item_mystic_image_SpellScript::HandleDummy);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_mystic_image_SpellScript();
        }
};

/// Sunreaver Beacon(For Horde) - 95568, Called by: 140300
class spell_item_sunreaver_beacon : public SpellScriptLoader
{
    public:
        spell_item_sunreaver_beacon() : SpellScriptLoader("spell_item_sunreaver_beacon") { }

        class spell_item_sunreaver_beacon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_sunreaver_beacon_SpellScript);

            void HandleDummy()
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (l_Player == nullptr)
                    return;

                l_Player->TeleportTo(1064, 7250.192871f, 6277.653320f, 19.352745f, 5.504922f);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_item_sunreaver_beacon_SpellScript::HandleDummy);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_sunreaver_beacon_SpellScript();
        }
};

/// Super Simian Sphere - 37254
/// Called by Going Ape - 48333
class spell_item_super_simian_sphere : public SpellScriptLoader
{
    public:
        spell_item_super_simian_sphere() : SpellScriptLoader("spell_item_super_simian_sphere") { }

        class  spell_item_super_simian_sphere_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_item_super_simian_sphere_AuraScript);

            enum eSpells
            {
                GoingApe = 48332
            };

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();
                if (l_Player == nullptr)
                    return;

                if (l_Player->HasAura(eSpells::GoingApe))
                    l_Player->RemoveAura(eSpells::GoingApe);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_item_super_simian_sphere_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_item_super_simian_sphere_AuraScript();
        }
};

/// The Perfect Blossom - 127766
/// Called by The Perfect Blossom - 187676
class spell_item_the_perfect_blossom : public SpellScriptLoader
{
    public:
        spell_item_the_perfect_blossom() : SpellScriptLoader("spell_item_the_perfect_blossom") { }

        class spell_item_the_perfect_blossom_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_the_perfect_blossom_SpellScript);

            enum eSpells
            {
                FelPetal = 127768,
                minEntry = 1,
                maxEntry = 5
            };

            void HandleCast()
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                Player* l_Player = l_Caster->ToPlayer();
                if (l_Player == nullptr)
                    return;

                uint32 l_Count = urand(eSpells::minEntry, eSpells::maxEntry);
                if (!l_Count)
                    return;

                l_Player->AddItem(eSpells::FelPetal, l_Count);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_item_the_perfect_blossom_SpellScript::HandleCast);

            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_the_perfect_blossom_SpellScript();
        }
};

/// Swapblaster - 161399
class spell_item_Swapblaster : public SpellScriptLoader
{
    public:
        spell_item_Swapblaster() : SpellScriptLoader("spell_item_Swapblaster") { }

        class spell_item_Swapblaster_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_Swapblaster_SpellScript);

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                float l_X = l_Target->GetPositionX();
                float l_Y = l_Target->GetPositionY();
                float l_Z = l_Target->GetPositionZ();
                float l_Orientation = l_Target->GetOrientation();

                l_Target->NearTeleportTo(l_Caster->GetPositionX(), l_Caster->GetPositionY(), l_Caster->GetPositionZ(), l_Caster->GetOrientation());
                l_Caster->NearTeleportTo(l_X, l_Y, l_Z, l_Orientation);
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_item_Swapblaster_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_item_Swapblaster_SpellScript();
        }
};


#ifndef __clang_analyzer__
void AddSC_item_spell_scripts()
{
    // 175442 Blessing of A'dal
    new spell_item_blessing_of_adal();
    // 67689 Ghostly Backfire
    new spell_item_ghostly_backfire();
    // 23074 Arcanite Dragonling
    new spell_item_trigger_spell("spell_item_arcanite_dragonling", SPELL_ARCANITE_DRAGONLING);
    // 23133 Gnomish Battle Chicken
    new spell_item_trigger_spell("spell_item_gnomish_battle_chicken", SPELL_BATTLE_CHICKEN);
    // 23076 Mechanical Dragonling
    new spell_item_trigger_spell("spell_item_mechanical_dragonling", SPELL_MECHANICAL_DRAGONLING);
    // 23075 Mithril Mechanical Dragonling
    new spell_item_trigger_spell("spell_item_mithril_mechanical_dragonling", SPELL_MITHRIL_MECHANICAL_DRAGONLING);
    new spell_item_deviate_fish();
    new spell_item_Swapblaster();
    new spell_item_super_simian_sphere();
    new spell_item_the_perfect_blossom();
    new spell_item_mystic_image();
    new spell_item_sunreaver_beacon();
    new spell_item_spike_toed_booterang();
    new spell_item_flask_of_the_north();
    new spell_item_gnomish_death_ray();
    new spell_item_make_a_wish();
    new spell_item_mingos_fortune_generator();
    new spell_item_net_o_matic();
    new spell_item_noggenfogger_elixir();
    new spell_item_savory_deviate_delight();
    new spell_item_six_demon_bag();
    new spell_item_underbelly_elixir();
    new spell_item_shadowmourne();
    new spell_item_red_rider_air_rifle();
    new spell_item_create_heart_candy();
    new spell_item_book_of_glyph_mastery();
    new spell_item_gift_of_the_harvester();
    new spell_item_map_of_the_geyser_fields();
    new spell_item_vanquished_clutches();
    new spell_magic_eater_food();
    new spell_item_refocus();
    new spell_item_shimmering_vessel();
    new spell_item_purify_helboar_meat();
    new spell_item_crystal_prison_dummy_dnd();
    new spell_item_reindeer_transformation();
    new spell_item_nigh_invulnerability();
    new spell_item_poultryizer();
    new spell_item_socrethars_stone();
    new spell_item_demon_broiled_surprise();
    new spell_item_complete_raptor_capture();
    new spell_item_impale_leviroth();
    new spell_item_brewfest_mount_transformation();
    new spell_item_nitro_boots();
    new spell_item_teach_language();
    new spell_item_rocket_boots();
    new spell_item_pygmy_oil();
    new spell_item_unusual_compass();
    new spell_item_chicken_cover();
    new spell_item_muisek_vessel();
    new spell_item_greatmothers_soulcatcher();
    new spell_item_enohar_explosive_arrows();
    new spell_item_holy_thurible();
    new spell_item_bandage_q24944();
    new spell_item_gen_alchemy_mop();
    new spell_alchemist_rejuvenation();
    new spell_item_amber_prison();
    new spell_item_first_aid();
    new spell_item_imputting_the_final_code();
    new spell_item_pot_of_fire();
    new spell_item_dit_da_jow();
    new spell_item_zuluhed_chains();
    new spell_item_yak_s_milk();
    new spell_item_throw_mantra();
    new spell_item_ancient_knowledge();
    new spell_item_sky_golem();
    new spell_item_engineering_scopes("spell_item_oglethorpe_s_missile_splitter", eEngineeringScopesSpells::SpellOglethorpesMissileSplitter);
    new spell_item_engineering_scopes("spell_item_megawatt_filament", eEngineeringScopesSpells::SpellMegawattFilament);
    new spell_item_engineering_scopes("spell_item_hemet_s_heartseeker", eEngineeringScopesSpells::HemetsHeartseeker);
    new spell_item_summon_chauffeur();
    new spell_item_forgemasters_vigor();
    new spell_item_hammer_blows();
    new spell_item_detonation();
    new spell_item_detonating();
    new spell_item_battering();
    new spell_item_cracks();
    new spell_item_sanatizing();
    new spell_item_cleansing_steam();
    new spell_item_molten_metal();
    new spell_item_pouring_slag();
    new spell_item_legs_of_iron();
    new spell_item_chest_of_iron();
    new spell_item_helm_of_iron();
    new spell_item_shoulders_of_iron();
    new spell_item_gauntlets_of_iron();
    new spell_create_reward_item();
    new spell_item_skymirror_image();
    new spell_item_memory_of_mr_smite();
    new spell_item_celestial_defender();
    new spell_item_curious_bronze_timepiece_alliance();
    new spell_item_curious_bronze_timepiece_horde();
    new spell_item_ascend_to_bladespire();
    new spell_item_faded_wizard_hat();
    new spell_item_reflecting_prism();
    new spell_item_ascend_to_karabor();
    new spell_item_sargerei_disguise();
    new spell_item_swapblaster();
    new spell_item_hypnotize_critter();
    new spell_item_enduring_elixir_of_wisdom();
}
#endif
