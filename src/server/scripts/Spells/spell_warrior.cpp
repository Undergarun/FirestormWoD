/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Scripts for spells with SPELLFAMILY_WARRIOR and SPELLFAMILY_GENERIC spells used by warrior players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_warr_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"

enum WarriorSpells
{
    WARRIOR_SPELL_LAST_STAND_TRIGGERED          = 12976,
    WARRIOR_SPELL_VICTORY_RUSH_DAMAGE           = 34428,
    WARRIOR_SPELL_VICTORY_RUSH_HEAL             = 118779,
    WARRIOR_SPELL_VICTORIOUS_STATE              = 32216,
    WARRIOR_SPELL_BLOODTHIRST                   = 23881,
    WARRIOR_SPELL_BLOODTHIRST_HEAL              = 117313,
    WARRIOR_SPELL_DEEP_WOUNDS                   = 115767,
    WARRIOR_SPELL_WEAKENED_BLOWS                = 115798,
    WARRIOR_SPELL_SHOCKWAVE_STUN                = 132168,
    WARRIOR_SPELL_HEROIC_LEAP_DAMAGE            = 52174,
    WARRIOR_SPELL_RALLYING_CRY                  = 97463,
    WARRIOR_SPELL_GLYPH_OF_MORTAL_STRIKE        = 58368,
    WARRIOR_SPELL_SWORD_AND_BOARD               = 50227,
    WARRIOR_SPELL_SHIELD_SLAM                   = 23922,
    WARRIOR_SPELL_ALLOW_RAGING_BLOW             = 131116,
    WARRIOR_SPELL_MOCKING_BANNER_TAUNT          = 114198,
    WARRIOR_NPC_MOCKING_BANNER                  = 59390,
    WARRIOR_SPELL_BERZERKER_RAGE_EFFECT         = 23691,
    WARRIOR_SPELL_ENRAGE                        = 12880,
    WARRIOR_SPELL_COLOSSUS_SMASH                = 167105,
    WARRIOR_SPELL_MORTAL_STRIKE_AURA            = 12294,
    WARRIOR_SPELL_TASTE_FOR_BLOOD               = 56636,
    WARRIOR_SPELL_ALLOW_OVERPOWER               = 60503,
    WARRIOR_SPELL_TASTE_FOR_BLOOD_DAMAGE_DONE   = 125831,
    WARRIOR_SPELL_SECOND_WIND_REGEN             = 16491,
    WARRIOR_SPELL_DRAGON_ROAR_KNOCK_BACK        = 118895,
    WARRIOR_SPELL_MEAT_CLEAVER_PROC             = 85739,
    WARRIOR_SPELL_PHYSICAL_VULNERABILITY        = 81326,
    WARRIOR_SPELL_STORM_BOLT_STUN               = 132169,
    WARRIOR_SPELL_SHIELD_BLOCK_TRIGGERED        = 132404,
    WARRIOR_SPELL_GLYPH_OF_HINDERING_STRIKES    = 58366,
    WARRIOR_SPELL_SLUGGISH                      = 129923,
    WARRIOR_SPELL_IMPENDING_VICTORY             = 103840,
    WARRIOR_SPELL_GLYPH_OF_RESONATING_POWER     = 58356,
    WARRIOR_SPELL_SWEEPING_STRIKES              = 12328,
    WARRIOR_SPELL_SLAM_AOE                      = 146361,
    WARRIOR_SPELL_BLOODSURGE                    = 46915,
    WARRIOR_SPELL_BLOODSURGE_PROC               = 46916,
    WARRIOR_SPELL_GLYPH_OF_COLOSSUS_SMASH       = 89003,
    WARRIOR_SPELL_SUNDER_ARMOR                  = 7386,
    WARRIOR_SPELL_SHIELD_OF_WALL_NOSHIELD       = 146128,
    WARRIOR_SPELL_SHIELD_OF_WALL_HORDE          = 146127,
    WARRIOR_SPELL_SHIELD_OF_WALL_ALLIANCE       = 147925,
    WARRIOR_SPELL_SPELL_REFLECTION_NOSHIELD     = 146120,
    WARRIOR_SPELL_SPELL_REFLECTION_HORDE        = 146122,
    WARRIOR_SPELL_SPELL_REFLECTION_ALLIANCE     = 147923,
    WARRIOR_SPELL_INTERVENE_TRIGGERED           = 34784,
    WARRIOR_SPELL_GAG_ORDER_SILENCE             = 18498,
    WARRIOR_SPELL_WILD_STRIKE                   = 100130,
    WARRIOR_SPELL_DOUBLE_TIME_MARKER            = 124184
};

/// Ravager - 76168
class npc_warrior_ravager : public CreatureScript
{
    public:
        npc_warrior_ravager() : CreatureScript("npc_warrior_ravager") { }

        struct npc_warrior_ravagerAI : public ScriptedAI
        {
            npc_warrior_ravagerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eSpells
            {
                RavagerAura = 153709
            };

            void IsSummonedBy(Unit* p_Summoner)
            {
                me->CastSpell(me, eSpells::RavagerAura, true);
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->AddUnitState(UnitState::UNIT_STATE_ROOT);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);

                if (p_Summoner == nullptr || p_Summoner->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                if (Player* l_Player = p_Summoner->ToPlayer())
                {
                    if (Item* l_Item = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, EquipmentSlots::EQUIPMENT_SLOT_MAINHAND))
                    {
                        /// Display Transmogrifications on player's clone
                        if (ItemTemplate const* l_Proto = sObjectMgr->GetItemTemplate(l_Item->GetDynamicValue(ItemDynamicFields::ITEM_DYNAMIC_FIELD_MODIFIERS, 0)))
                            me->SetUInt32Value(EUnitFields::UNIT_FIELD_VIRTUAL_ITEM_ID, l_Proto->ItemId);
                        else
                            me->SetUInt32Value(EUnitFields::UNIT_FIELD_VIRTUAL_ITEM_ID, l_Item->GetTemplate()->ItemId);
                    }

                    if (Item* l_Item = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, EquipmentSlots::EQUIPMENT_SLOT_OFFHAND))
                    {
                        /// Display Transmogrifications on player's clone
                        if (ItemTemplate const* l_Proto = sObjectMgr->GetItemTemplate(l_Item->GetDynamicValue(ItemDynamicFields::ITEM_DYNAMIC_FIELD_MODIFIERS, 0)))
                            me->SetUInt32Value(EUnitFields::UNIT_FIELD_VIRTUAL_ITEM_ID + 1, l_Proto->ItemId);
                        else
                            me->SetUInt32Value(EUnitFields::UNIT_FIELD_VIRTUAL_ITEM_ID + 1, l_Item->GetTemplate()->ItemId);
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff) { }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_warrior_ravagerAI(p_Creature);
        }
};

/// Ravager - 152277
class spell_warr_ravager : public SpellScriptLoader
{
    public:
        spell_warr_ravager() : SpellScriptLoader("spell_warr_ravager") { }

        class spell_warr_ravager_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_ravager_AuraScript);

            enum eDatas
            {
                RavagerNPC    = 76168,

                RavagerDamage = 156287
            };

            void CalculateParryPCT(constAuraEffectPtr p_AurEff, int32& p_Amount, bool& p_CanBeRecalculated)
            {
                if (GetCaster() == nullptr)
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) != SpecIndex::SPEC_WARRIOR_PROTECTION)
                        p_Amount = 0;
                }
            }

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    Creature* l_Ravager = nullptr;
                    for (auto l_Iter : l_Caster->m_Controlled)
                    {
                        if (l_Iter->GetEntry() == eDatas::RavagerNPC)
                            l_Ravager = l_Iter->ToCreature();
                    }

                    if (l_Ravager == nullptr)
                        return;

                    l_Caster->CastSpell(l_Ravager, eDatas::RavagerDamage, true);
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warr_ravager_AuraScript::CalculateParryPCT, EFFECT_0, SPELL_AURA_MOD_PARRY_PERCENT);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_warr_ravager_AuraScript::OnTick, EFFECT_2, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_ravager_AuraScript();
        }
};

/// Slam - 1464
class spell_warr_slam: public SpellScriptLoader
{
    public:
        spell_warr_slam() : SpellScriptLoader("spell_warr_slam") { }

        class spell_warr_slam_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_slam_SpellScript);

            void HandleBeforeHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->HasAura(WARRIOR_SPELL_COLOSSUS_SMASH, caster->GetGUID()))
                            SetHitDamage(int32(GetHitDamage() * 1.1f));
                    }
                }
            }

            void HandleAfterHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        int32 damage = CalculatePct(GetHitDamage(), 35);

                        if (caster->HasAura(WARRIOR_SPELL_SWEEPING_STRIKES))
                            caster->CastCustomSpell(target, WARRIOR_SPELL_SLAM_AOE, &damage, NULL, NULL, true);
                    }
                }
            }

            void Register()
            {
                BeforeHit += SpellHitFn(spell_warr_slam_SpellScript::HandleBeforeHit);
                AfterHit += SpellHitFn(spell_warr_slam_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_slam_SpellScript();
        }
};

/// Victorious State - 32216
class spell_warr_victorious_state: public SpellScriptLoader
{
    public:
        spell_warr_victorious_state() : SpellScriptLoader("spell_warr_victorious_state") { }

        class spell_warr_victorious_state_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_victorious_state_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasSpellCooldown(WARRIOR_SPELL_IMPENDING_VICTORY))
                        _player->RemoveSpellCooldown(WARRIOR_SPELL_IMPENDING_VICTORY, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_victorious_state_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_victorious_state_SpellScript();
        }
};

/// Called by Heroic Strike - 78 and Cleave - 845
/// Glyph of Hindering Strikes - 58366
class spell_warr_glyph_of_hindering_strikes: public SpellScriptLoader
{
    public:
        spell_warr_glyph_of_hindering_strikes() : SpellScriptLoader("spell_warr_glyph_of_hindering_strikes") { }

        class spell_warr_glyph_of_hindering_strikes_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_glyph_of_hindering_strikes_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (_player->HasAura(WARRIOR_SPELL_GLYPH_OF_HINDERING_STRIKES))
                            _player->CastSpell(target, WARRIOR_SPELL_SLUGGISH, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_glyph_of_hindering_strikes_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_glyph_of_hindering_strikes_SpellScript();
        }
};

/// Stampeding Shout - 122294
class spell_warr_stampeding_shout: public SpellScriptLoader
{
    public:
        spell_warr_stampeding_shout() : SpellScriptLoader("spell_warr_stampeding_shout") { }

        class spell_warr_stampeding_shout_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_stampeding_shout_SpellScript);

            void HandleOnHit()
            {
                if (Unit* target = GetHitUnit())
                    target->RemoveMovementImpairingAuras();
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_stampeding_shout_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_stampeding_shout_SpellScript();
        }
};

/// Shield Block - 2565
class spell_warr_shield_block: public SpellScriptLoader
{
    public:
        spell_warr_shield_block() : SpellScriptLoader("spell_warr_shield_block") { }

        class spell_warr_shield_block_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_shield_block_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                    caster->CastSpell(caster, WARRIOR_SPELL_SHIELD_BLOCK_TRIGGERED, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_shield_block_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_shield_block_SpellScript();
        }
};

/// Storm Bolt - 107570
class spell_warr_storm_bolt: public SpellScriptLoader
{
    public:
        spell_warr_storm_bolt() : SpellScriptLoader("spell_warr_storm_bolt") { }

        class spell_warr_storm_bolt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_storm_bolt_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* unitTarget = GetHitUnit())
                    {
                        if (unitTarget->IsImmunedToSpellEffect(sSpellMgr->GetSpellInfo(WARRIOR_SPELL_STORM_BOLT_STUN), EFFECT_0))
                            SetHitDamage(GetHitDamage() * 4); // Deals quadruple damage to targets permanently immune to stuns
                        else
                            caster->CastSpell(unitTarget, WARRIOR_SPELL_STORM_BOLT_STUN, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_storm_bolt_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_storm_bolt_SpellScript();
        }
};

enum ColossusSpells
{
    SPELL_WARRIOR_WEAPONS_MASTER = 76838,
};

/// Colossus Smash - 167105
class spell_warr_colossus_smash: public SpellScriptLoader
{
    public:
        spell_warr_colossus_smash() : SpellScriptLoader("spell_warr_colossus_smash") { }

        class spell_warr_colossus_smash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_colossus_smash_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        caster->CastSpell(target, WARRIOR_SPELL_PHYSICAL_VULNERABILITY, true);

                        if (caster->HasAura(WARRIOR_SPELL_GLYPH_OF_COLOSSUS_SMASH))
                            caster->CastSpell(target, WARRIOR_SPELL_SUNDER_ARMOR, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_colossus_smash_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_colossus_smash_SpellScript();
        }
};

/// Dragon Roar - 118000
class spell_warr_dragon_roar: public SpellScriptLoader
{
    public:
        spell_warr_dragon_roar() : SpellScriptLoader("spell_warr_dragon_roar") { }

        class spell_warr_dragon_roar_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_dragon_roar_SpellScript);

            void HandleAfterHit()
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetHitUnit())
                        caster->CastSpell(target, WARRIOR_SPELL_DRAGON_ROAR_KNOCK_BACK, true);
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_warr_dragon_roar_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_dragon_roar_SpellScript();
        }
};

/// Staggering Shout - 107566
class spell_warr_staggering_shout: public SpellScriptLoader
{
    public:
        spell_warr_staggering_shout() : SpellScriptLoader("spell_warr_staggering_shout") { }

        class spell_warr_staggering_shout_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_staggering_shout_SpellScript);

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(JadeCore::UnitAuraTypeCheck(false, SPELL_AURA_MOD_DECREASE_SPEED));
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warr_staggering_shout_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_staggering_shout_SpellScript();
        }
};

/// Second Wind - 29838
class spell_warr_second_wind: public SpellScriptLoader
{
    public:
        spell_warr_second_wind() : SpellScriptLoader("spell_warr_second_wind") { }

        class spell_warr_second_wind_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_second_wind_AuraScript);

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& l_ProcInfo)
            {
                PreventDefaultAction();

                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->GetHealthPct() <= 35.0f && !l_Caster->HasAura(WARRIOR_SPELL_SECOND_WIND_REGEN))
                        l_Caster->CastSpell(l_Caster, WARRIOR_SPELL_SECOND_WIND_REGEN, true);
                }
            }

            void OnRemove(constAuraEffectPtr p_AurEff, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(WARRIOR_SPELL_SECOND_WIND_REGEN))
                        l_Caster->RemoveAura(WARRIOR_SPELL_SECOND_WIND_REGEN);
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_warr_second_wind_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectProc += AuraEffectProcFn(spell_warr_second_wind_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_second_wind_AuraScript();
        }
};

/// Sudden Death - 52437
class spell_warr_sudden_death: public SpellScriptLoader
{
    public:
        spell_warr_sudden_death() : SpellScriptLoader("spell_warr_sudden_death") { }

        class spell_warr_sudden_death_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_sudden_death_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasSpellCooldown(WARRIOR_SPELL_COLOSSUS_SMASH))
                        _player->RemoveSpellCooldown(WARRIOR_SPELL_COLOSSUS_SMASH, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_sudden_death_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_sudden_death_SpellScript();
        }
};

/// Berzerker Rage - 18499
class spell_warr_berzerker_rage: public SpellScriptLoader
{
    public:
        spell_warr_berzerker_rage() : SpellScriptLoader("spell_warr_berzerker_rage") { }

        class spell_warr_berzerker_rage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_berzerker_rage_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    _player->CastSpell(_player, WARRIOR_SPELL_ENRAGE, true);

                    if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_WARRIOR_FURY && _player->getLevel() >= GetSpellInfo()->BaseLevel)
                        _player->AddAura(WARRIOR_SPELL_ALLOW_RAGING_BLOW, _player);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_berzerker_rage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_berzerker_rage_SpellScript();
        }
};

/// Enrage - 12880
class spell_warr_enrage: public SpellScriptLoader
{
    public:
        spell_warr_enrage() : SpellScriptLoader("spell_warr_enrage") { }

        class spell_warr_enrage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_enrage_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_WARRIOR_FURY && _player->getLevel() >= GetSpellInfo()->BaseLevel)
                        _player->AddAura(WARRIOR_SPELL_ALLOW_RAGING_BLOW, _player);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_enrage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_enrage_SpellScript();
        }
};

/// Mocking Banner - 114192
class spell_warr_mocking_banner: public SpellScriptLoader
{
    public:
        spell_warr_mocking_banner() : SpellScriptLoader("spell_warr_mocking_banner") { }

        class spell_warr_mocking_banner_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_mocking_banner_AuraScript);

            void OnTick(constAuraEffectPtr aurEff)
            {
                if (Unit* target = GetTarget())
                {
                    std::list<Creature*> bannerList;
                    std::list<Creature*> tempList;

                    target->GetCreatureListWithEntryInGrid(tempList, WARRIOR_NPC_MOCKING_BANNER, GetSpellInfo()->RangeEntry->maxRangeHostile);

                    bannerList = tempList;

                    // Remove other players banners
                    for (auto itr : tempList)
                    {
                        Unit* owner = itr->GetOwner();
                        if (owner && owner == target && itr->isSummon())
                            continue;

                        bannerList.remove(itr);
                    }

                    for (auto itr : bannerList)
                        target->CastSpell(itr, WARRIOR_SPELL_MOCKING_BANNER_TAUNT, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_warr_mocking_banner_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_mocking_banner_AuraScript();
        }
};

/// Raging Blow - 85288
class spell_warr_raging_blow: public SpellScriptLoader
{
    public:
        spell_warr_raging_blow() : SpellScriptLoader("spell_warr_raging_blow") { }

        class spell_warr_raging_blow_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_raging_blow_SpellScript);

            void HandleAfterCast()
            {
                if (Unit* caster = GetCaster())
                    if (AuraPtr ragingBlow = caster->GetAura(WARRIOR_SPELL_ALLOW_RAGING_BLOW))
                        ragingBlow->ModStackAmount(-1);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_warr_raging_blow_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_raging_blow_SpellScript();
        }
};

/// Called by Devastate - 20243
/// Sword and Board - 46953
class spell_warr_sword_and_board: public SpellScriptLoader
{
    public:
        spell_warr_sword_and_board() : SpellScriptLoader("spell_warr_sword_and_board") { }

        class spell_warr_sword_and_board_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_sword_and_board_SpellScript);

            void HandleOnHit()
            {
                if (!GetHitUnit())
                    return;

                // Fix Sword and Board
                if (Player* player = GetCaster()->ToPlayer())
                {
                    if (roll_chance_i(30))
                    {
                        player->CastSpell(player, WARRIOR_SPELL_SWORD_AND_BOARD, true);
                        player->RemoveSpellCooldown(WARRIOR_SPELL_SHIELD_SLAM, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_sword_and_board_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_sword_and_board_SpellScript();
        }
};

/// Mortal strike - 12294
class spell_warr_mortal_strike: public SpellScriptLoader
{
    public:
        spell_warr_mortal_strike() : SpellScriptLoader("spell_warr_mortal_strike") { }

        class spell_warr_mortal_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_mortal_strike_SpellScript);

            void HandleOnHit()
            {
                /// Fix Apply Mortal strike buff on player only if he has the correct glyph
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(WARRIOR_SPELL_MORTAL_STRIKE_AURA) && !l_Caster->HasAura(WARRIOR_SPELL_GLYPH_OF_MORTAL_STRIKE))
                        l_Caster->RemoveAura(WARRIOR_SPELL_MORTAL_STRIKE_AURA);

                    if (l_Caster->HasAura(WARRIOR_SPELL_TASTE_FOR_BLOOD))
                    {
                        l_Caster->AddComboPoints(1);
                        l_Caster->StartReactiveTimer(REACTIVE_OVERPOWER);
                        l_Caster->CastSpell(l_Caster, WARRIOR_SPELL_ALLOW_OVERPOWER, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_mortal_strike_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_mortal_strike_SpellScript();
        }
};

/// Rallying cry - 97462
class spell_warr_rallying_cry: public SpellScriptLoader
{
    public:
        spell_warr_rallying_cry() : SpellScriptLoader("spell_warr_rallying_cry") { }

        class spell_warr_rallying_cry_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_rallying_cry_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    int32 l_Bp0 = 0;
                    std::list<Unit*> l_MemberList;

                    l_Player->GetRaidMembers(l_MemberList);

                    for (auto l_Itr : l_MemberList)
                    {
                        if (l_Itr->IsWithinDistInMap(l_Player, GetSpellInfo()->Effects[EFFECT_0].RadiusEntry->radiusFriend))
                        {
                            l_Bp0 = CalculatePct(l_Itr->GetMaxHealth(), GetSpellInfo()->Effects[EFFECT_0].BasePoints);
                            l_Player->CastCustomSpell(l_Itr, WARRIOR_SPELL_RALLYING_CRY, &l_Bp0, NULL, NULL, true);
                        }
                    }

                    l_Bp0 = CalculatePct(l_Player->GetMaxHealth(), GetSpellInfo()->Effects[EFFECT_0].BasePoints);
                    l_Player->CastCustomSpell(l_Player, WARRIOR_SPELL_RALLYING_CRY, &l_Bp0, NULL, NULL, true);
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_warr_rallying_cry_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_rallying_cry_SpellScript();
        }
};

enum HeroicLeapSpells
{
    SPELL_WARR_HEROIC_LEAP_JUMP      = 94954,
    SPELL_WARR_ITEM_PVP_SET_4P_BONUS = 133277,
    SPELL_WARR_HEROIC_LEAP_SPEED     = 133278,
};

/// Heroic leap - 6544
class spell_warr_heroic_leap: public SpellScriptLoader
{
    public:
        spell_warr_heroic_leap() : SpellScriptLoader("spell_warr_heroic_leap") { }

        class spell_warr_heroic_leap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_heroic_leap_SpellScript);

            SpellCastResult CheckElevation()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                WorldLocation* l_SpellDest = const_cast<WorldLocation*>(GetExplTargetDest());

                if (!l_Player || !l_SpellDest)
                    return SPELL_FAILED_DONT_REPORT;

                if (l_SpellDest->GetPositionZ() > l_Player->GetPositionZ() + 5.0f)
                    return SPELL_FAILED_NOPATH;
                else if (l_Player->HasAuraType(SPELL_AURA_MOD_ROOT) || l_Player->HasAuraType(SPELL_AURA_MOD_ROOT_2))
                    return SPELL_FAILED_ROOTED;
                else if (l_Player->GetMap()->IsBattlegroundOrArena())
                {
                    if (Battleground* l_Bg = l_Player->GetBattleground())
                    {
                        if (l_Bg->GetStatus() != STATUS_IN_PROGRESS)
                            return SPELL_FAILED_NOT_READY;
                    }
                }

                return SPELL_CAST_OK;
            }

            void HandleOnCast()
            {
                WorldLocation* l_SpellDest = const_cast<WorldLocation*>(GetExplTargetDest());
                if (l_SpellDest)
                    GetCaster()->CastSpell(l_SpellDest->GetPositionX(), l_SpellDest->GetPositionY(), l_SpellDest->GetPositionZ(), SPELL_WARR_HEROIC_LEAP_JUMP, true);
            }

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(SPELL_WARR_ITEM_PVP_SET_4P_BONUS))
                    l_Caster->CastSpell(l_Caster, SPELL_WARR_HEROIC_LEAP_SPEED, true);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_warr_heroic_leap_SpellScript::CheckElevation);
                OnCast += SpellCastFn(spell_warr_heroic_leap_SpellScript::HandleOnCast);
                AfterCast += SpellCastFn(spell_warr_heroic_leap_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_heroic_leap_SpellScript();
        }
};

/// Heroic Leap (damage) - 52174
class spell_warr_heroic_leap_damage: public SpellScriptLoader
{
    public:
        spell_warr_heroic_leap_damage() : SpellScriptLoader("spell_warr_heroic_leap_damage") { }

        class spell_warr_heroic_leap_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_heroic_leap_damage_SpellScript);

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->GetTypeId() == TYPEID_PLAYER)
                    if (l_Caster->ToPlayer()->GetSpecializationId(l_Caster->ToPlayer()->GetActiveSpec()) == SPEC_WARRIOR_FURY)
                        SetHitDamage(int32(GetHitDamage() * 1.2f));
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_heroic_leap_damage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_heroic_leap_damage_SpellScript();
        }
};

enum class ShockwaveValues : uint32
{
    SpellId = 46968
};

/// Shockwave - 46968
class spell_warr_shockwave: public SpellScriptLoader
{
    public:
        spell_warr_shockwave() : SpellScriptLoader("spell_warr_shockwave") { }

        class spell_warr_shockwave_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_shockwave_SpellScript);

            void HandleDamage(SpellEffIndex effIndex)
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetHitUnit())
                        caster->CastSpell(target, WARRIOR_SPELL_SHOCKWAVE_STUN, true);
            }

            /// Cooldown reduced by 20 sec if it strikes at least 3 targets.
            void HandleAfterHit()
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (int32(GetSpell()->GetUnitTargetCount()) >= GetSpellInfo()->Effects[EFFECT_0].BasePoints)
                    GetCaster()->ToPlayer()->ReduceSpellCooldown((uint32)ShockwaveValues::SpellId, GetSpellInfo()->Effects[EFFECT_3].BasePoints * IN_MILLISECONDS);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_warr_shockwave_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
                AfterCast += SpellCastFn(spell_warr_shockwave_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_shockwave_SpellScript();
        }
};

/// Bloodthirst - 23881
class spell_warr_bloodthirst: public SpellScriptLoader
{
    public:
        spell_warr_bloodthirst() : SpellScriptLoader("spell_warr_bloodthirst") { }

        class spell_warr_bloodthirst_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_bloodthirst_SpellScript);

            bool Validate(SpellInfo const* /*SpellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(WARRIOR_SPELL_BLOODTHIRST) || !sSpellMgr->GetSpellInfo(WARRIOR_SPELL_BLOODTHIRST_HEAL))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (GetHitDamage())
                    {
                        l_Caster->CastSpell(l_Caster, WARRIOR_SPELL_BLOODTHIRST_HEAL, true);

                        const SpellInfo *l_SpellInfo = sSpellMgr->GetSpellInfo(WARRIOR_SPELL_BLOODSURGE);

                        if (l_SpellInfo != nullptr && l_Caster->HasAura(WARRIOR_SPELL_BLOODSURGE) && roll_chance_i(l_SpellInfo->Effects[EFFECT_0].BasePoints))
                            l_Caster->CastSpell(l_Caster, WARRIOR_SPELL_BLOODSURGE_PROC, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_bloodthirst_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_bloodthirst_SpellScript();
        }
};

/// Victory Rush - 34428
class spell_warr_victory_rush: public SpellScriptLoader
{
    public:
        spell_warr_victory_rush() : SpellScriptLoader("spell_warr_victory_rush") { }

        class spell_warr_victory_rush_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_victory_rush_SpellScript);

            bool Validate(SpellInfo const* /*SpellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(WARRIOR_SPELL_VICTORY_RUSH_DAMAGE))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                if (!GetHitUnit())
                    return;

                if (Unit* caster = GetCaster())
                {
                    caster->CastSpell(caster, WARRIOR_SPELL_VICTORY_RUSH_HEAL, true);

                    if (caster->HasAura(WARRIOR_SPELL_VICTORIOUS_STATE))
                        caster->RemoveAura(WARRIOR_SPELL_VICTORIOUS_STATE);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_victory_rush_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_victory_rush_SpellScript();
        }
};

/// Last Stand - 12975
class spell_warr_last_stand: public SpellScriptLoader
{
    public:
        spell_warr_last_stand() : SpellScriptLoader("spell_warr_last_stand") { }

        class spell_warr_last_stand_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_last_stand_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(WARRIOR_SPELL_LAST_STAND_TRIGGERED))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    int32 healthModSpellBasePoints0 = int32(caster->CountPctFromMaxHealth(GetSpellInfo()->Effects[EFFECT_0].BasePoints));
                    caster->CastCustomSpell(caster, WARRIOR_SPELL_LAST_STAND_TRIGGERED, &healthModSpellBasePoints0, NULL, NULL, true, NULL);
                }
            }

            void Register()
            {
                // add dummy effect spell handler to Last Stand
                OnEffectHit += SpellEffectFn(spell_warr_last_stand_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_last_stand_SpellScript();
        }
};

/// Called By Thunder Clap - 6343, Mortal Strike - 12294, Bloodthirst - 23881 and Devastate - 20243
/// Deep Wounds - 115767
class spell_warr_deep_wounds: public SpellScriptLoader
{
    public:
        spell_warr_deep_wounds() : SpellScriptLoader("spell_warr_deep_wounds") { }

        class spell_warr_deep_wounds_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_deep_wounds_SpellScript);

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();
                if (!l_Target)
                    return;

                if (l_Caster->GetTypeId() == TYPEID_PLAYER && l_Caster->ToPlayer()->GetSpecializationId(l_Caster->ToPlayer()->GetActiveSpec()) != SPEC_WARRIOR_PROTECTION)
                    return;

                if (l_Target->GetGUID() == l_Caster->GetGUID())
                    return;

                if (l_Caster->getLevel() >= GetSpellInfo()->BaseLevel)
                    l_Caster->CastSpell(l_Target, WARRIOR_SPELL_DEEP_WOUNDS, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_deep_wounds_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_deep_wounds_SpellScript();
        }
};

enum ChargeSpells
{
    SPELL_WARR_WARBRINGER_STUN    = 7922,
    SPELL_WARR_GLYPH_OF_BULL_RUSH = 94372,
    SPELL_WARR_DOUBLE_TIME        = 103827,
    SPELL_WARR_WARBRINGER         = 103828,
    SPELL_WARR_CHARGE_ROOT        = 105771,
    SPELL_WARR_DOUBLE_TIME_MARKER = 124184
};

/// Charge - 100
class spell_warr_charge: public SpellScriptLoader
{
    public:
        spell_warr_charge() : SpellScriptLoader("spell_warr_charge") { }

    class spell_warr_charge_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_charge_SpellScript)

        bool m_HasAuraDoubleTimeMarker = false;

        void HandleOnCast()
        {
            if (Unit* l_Caster = GetCaster())
                if (l_Caster->HasAura(WARRIOR_SPELL_DOUBLE_TIME_MARKER))
                    m_HasAuraDoubleTimeMarker = true;
        }

        void HandleCharge(SpellEffIndex /*effIndex*/)
        {
            Unit* l_Caster = GetCaster();
            Unit* l_Target = GetHitUnit();
            
            if (l_Target == nullptr || l_Caster == nullptr)
                return;

            l_Caster->CastSpell(l_Target, l_Caster->HasAura(SPELL_WARR_WARBRINGER) ? SPELL_WARR_WARBRINGER_STUN : SPELL_WARR_CHARGE_ROOT, true);
        }

        void HandleRageGain(SpellEffIndex /*effIndex*/)
        {
            Unit* l_Caster = GetCaster();

            if (l_Caster != nullptr && !m_HasAuraDoubleTimeMarker)
            {
                int32 l_RageGain = GetEffectValue() / l_Caster->GetPowerCoeff(POWER_RAGE);

                if (AuraEffectPtr l_BullRush = l_Caster->GetAuraEffect(SPELL_WARR_GLYPH_OF_BULL_RUSH, EFFECT_0))
                    l_RageGain += l_BullRush->GetAmount();

                l_Caster->EnergizeBySpell(l_Caster, GetSpellInfo()->Id, l_RageGain * l_Caster->GetPowerCoeff(POWER_RAGE), POWER_RAGE);
            }
        }

        void Register()
        {
            OnCast += SpellCastFn(spell_warr_charge_SpellScript::HandleOnCast);
            OnEffectHitTarget += SpellEffectFn(spell_warr_charge_SpellScript::HandleCharge, EFFECT_0, SPELL_EFFECT_CHARGE);
            OnEffectHitTarget += SpellEffectFn(spell_warr_charge_SpellScript::HandleRageGain, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warr_charge_SpellScript;
    }
};

/// Shield Wall - 871
class spell_warr_shield_wall: public SpellScriptLoader
{
    public:
        spell_warr_shield_wall() : SpellScriptLoader("spell_warr_shield_wall") { }

        class spell_warr_shield_wall_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_shield_wall_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster())
                    return;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->GetShield())
                        _player->AddAura(WARRIOR_SPELL_SHIELD_OF_WALL_NOSHIELD, _player);
                    else
                    {
                        if (_player->GetTeam() == HORDE)
                            _player->AddAura(WARRIOR_SPELL_SHIELD_OF_WALL_HORDE, _player);
                        else
                            _player->AddAura(WARRIOR_SPELL_SHIELD_OF_WALL_ALLIANCE, _player);
                    }
                }
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster())
                    return;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(WARRIOR_SPELL_SHIELD_OF_WALL_NOSHIELD))
                        _player->RemoveAura(WARRIOR_SPELL_SHIELD_OF_WALL_NOSHIELD);
                    else if (_player->HasAura(WARRIOR_SPELL_SHIELD_OF_WALL_HORDE))
                        _player->RemoveAura(WARRIOR_SPELL_SHIELD_OF_WALL_HORDE);
                    else
                        _player->RemoveAura(WARRIOR_SPELL_SHIELD_OF_WALL_ALLIANCE);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_warr_shield_wall_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_warr_shield_wall_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_shield_wall_AuraScript();
        }
};

/// Spell Reflection - 23920
class spell_warr_spell_reflection: public SpellScriptLoader
{
    public:
        spell_warr_spell_reflection() : SpellScriptLoader("spell_warr_spell_reflection") { }

        class spell_warr_spell_reflection_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_spell_reflection_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->GetShield())
                        _player->AddAura(WARRIOR_SPELL_SPELL_REFLECTION_NOSHIELD, _player);
                    else
                    {
                        if (_player->GetTeam() == HORDE)
                            _player->AddAura(WARRIOR_SPELL_SPELL_REFLECTION_HORDE, _player);
                        else
                            _player->AddAura(WARRIOR_SPELL_SPELL_REFLECTION_ALLIANCE, _player);
                    }
                }
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(WARRIOR_SPELL_SPELL_REFLECTION_NOSHIELD))
                        _player->RemoveAura(WARRIOR_SPELL_SPELL_REFLECTION_NOSHIELD);
                    else if (_player->HasAura(WARRIOR_SPELL_SPELL_REFLECTION_HORDE))
                        _player->RemoveAura(WARRIOR_SPELL_SPELL_REFLECTION_HORDE);
                    else
                        _player->RemoveAura(WARRIOR_SPELL_SPELL_REFLECTION_ALLIANCE);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_warr_spell_reflection_AuraScript::OnApply, EFFECT_0, SPELL_AURA_REFLECT_SPELLS, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_warr_spell_reflection_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_REFLECT_SPELLS, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_spell_reflection_AuraScript();
        }
};

/// Intervene - 3411
class spell_warr_intervene: public SpellScriptLoader
{
    public:
        spell_warr_intervene() : SpellScriptLoader("spell_warr_intervene") { }

        class spell_warr_intervene_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_intervene_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                        caster->CastSpell(target, WARRIOR_SPELL_INTERVENE_TRIGGERED, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_intervene_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_intervene_SpellScript();
        }
};

/// Called by Pummel - 6552 or Heroic Throw - 57755
class spell_warr_glyph_of_gag_order: public SpellScriptLoader
{
    public:
        spell_warr_glyph_of_gag_order() : SpellScriptLoader("spell_warr_glyph_of_gag_order") { }

        class spell_warr_glyph_of_gag_order_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_warr_glyph_of_gag_order_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (target->GetTypeId() != TYPEID_PLAYER)
                            caster->CastSpell(target, WARRIOR_SPELL_GAG_ORDER_SILENCE, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_warr_glyph_of_gag_order_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_warr_glyph_of_gag_order_SpellScript();
        }
};

/// Shield Barrier 174926 - Shield Barrier 112048
class spell_warr_shield_barrier: public SpellScriptLoader
{
    public:
        spell_warr_shield_barrier() : SpellScriptLoader("spell_warr_shield_barrier") { }

        class spell_warr_shield_barrier_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_shield_barrier_AuraScript);

            void CalculateAmount(constAuraEffectPtr /*aurEff*/, int32& p_Amount, bool& /*canBeRecalculated*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    int8 l_RagetoUse = l_Caster->GetPower(POWER_RAGE) / l_Caster->GetPowerCoeff(POWER_RAGE);

                    if (l_RagetoUse > 40)
                        l_RagetoUse = 40;

                    p_Amount = l_Caster->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack) * 1.125;
                    p_Amount += (p_Amount / 20) * l_RagetoUse;

                    l_Caster->ModifyPower(POWER_RAGE, -l_RagetoUse * l_Caster->GetPowerCoeff(POWER_RAGE));
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warr_shield_barrier_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_shield_barrier_AuraScript();
        }
};

enum AngerManagementSpells
{
    SPELL_WARR_ANGER_MANAGEMENT = 152278,
};

#define REDUCED_SPELLS_ID_MAX 13
uint32 g_ReducedSpellsId[REDUCED_SPELLS_ID_MAX] =
{
    107574, ///< Avatar
    12292,  ///< Bloodbath
    46924,  ///< Bladestorm
    107570, ///< Storm Bolt
    46968,  ///< Shockwave
    118000, ///< Dragon Roar
    114192, ///< Mocking Banner
    6544,   ///< Heroic Leap
    871,    ///< Shield Wall
    1160,   ///< Demoralizing Shout
    12975,  ///< Last Stand
    1719,   ///< Recklessness
    118038  ///< Die by the Sword
};

class spell_warr_anger_management: public PlayerScript
{
public:
    spell_warr_anger_management() : PlayerScript("spell_warr_anger_management") {}

    uint16 m_RageSpend = 0;

    void OnModifyPower(Player* p_Player, Powers p_Power, int32 p_OldValue, int32& p_NewValue, bool p_Regen)
    {
        if (!p_Player || p_Player->getClass() != CLASS_WARRIOR || p_Power != POWER_RAGE || p_Regen || !p_Player->HasAura(SPELL_WARR_ANGER_MANAGEMENT))
            return;

        // Get the power earn (if > 0 ) or consum (if < 0)
        int32 l_diffValue = p_NewValue - p_OldValue;

        // Only get spended rage
        if (l_diffValue > 0)
            return;

        m_RageSpend += -l_diffValue / p_Player->GetPowerCoeff(POWER_RAGE);
        if (m_RageSpend >= sSpellMgr->GetSpellInfo(SPELL_WARR_ANGER_MANAGEMENT)->Effects[EFFECT_0].BasePoints)
        {
            for (int l_I = 0; l_I < REDUCED_SPELLS_ID_MAX; l_I++)
            {
                if (p_Player->HasSpellCooldown(g_ReducedSpellsId[l_I]))
                    p_Player->ReduceSpellCooldown(g_ReducedSpellsId[l_I], 1 * IN_MILLISECONDS);
            }

            m_RageSpend = 0;
        }
    }
};

/// Execute - 163201
class spell_warr_execute: public SpellScriptLoader
{
public:
    spell_warr_execute() : SpellScriptLoader("spell_warr_execute") { }

    class spell_warr_execute_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_execute_SpellScript);

        void HandleOnHit()
        {
            int32 l_Damage = GetHitDamage();

            // converts each extra rage (up to 30 rage) into additional damage
            int32 l_RageConsumed = -GetCaster()->ModifyPower(POWER_RAGE, -(GetSpellInfo()->Effects[EFFECT_2].BasePoints * 10));
            // 30 rage = 320% more weapon damage
            AddPct(l_Damage, (l_RageConsumed / 1.5f));

            if (GetCaster()->HasAura(SPELL_WARRIOR_WEAPONS_MASTER))
            {
                float l_MasteryValue = GetCaster()->GetFloatValue(PLAYER_FIELD_MASTERY) * 3.5f;

                l_Damage += CalculatePct(l_Damage, l_MasteryValue);
            }

            SetHitDamage(l_Damage);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_warr_execute_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warr_execute_SpellScript();
    }
};

enum WhirlwindSpells
{
    SPELL_WARR_WHIRLWIND_OFFHAND = 44949
};

/// Whirlwind - 1680
class spell_warr_whirlwind: public SpellScriptLoader
{
public:
    spell_warr_whirlwind() : SpellScriptLoader("spell_warr_whirlwind") { }

    class spell_warr_whirlwind_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_whirlwind_SpellScript);

        void HandleOnHit()
        {
            Player* l_Player = GetCaster()->ToPlayer();
            Unit* l_Target = GetHitUnit();
            if (!l_Player || !l_Target)
                return;

            if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_WARRIOR_FURY)
                l_Player->CastSpell(l_Target, SPELL_WARR_WHIRLWIND_OFFHAND, true);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_warr_whirlwind_SpellScript::HandleOnHit);
        }
    };

    class spell_warr_whirlwind_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_warr_whirlwind_AuraScript);

        void CalculateAmount(constAuraEffectPtr /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
        {
            if (Unit* l_Caster = GetCaster())
                if (Player* l_Player = l_Caster->ToPlayer())
                    if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_WARRIOR_ARMS)
                        amount = 200;
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warr_whirlwind_AuraScript::CalculateAmount, EFFECT_1, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
        }

    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warr_whirlwind_SpellScript();
    }

    AuraScript* GetAuraScript() const
    {
        return new spell_warr_whirlwind_AuraScript();
    }
};

enum ShieldChargeSpells
{
    SPELL_WARR_SHIELD_CHARGE_MODIFIER = 169667,
    SPELL_WARR_SHIELD_CHARGE_CHARGE = 178768
};

/// Shield Charge - 156321
class spell_warr_shield_charge: public SpellScriptLoader
{
public:
    spell_warr_shield_charge() : SpellScriptLoader("spell_warr_shield_charge") { }

    class spell_warr_shield_charge_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_shield_charge_SpellScript);

        void HandleOnCast()
        {
            Unit* l_Caster = GetCaster();
            Unit* l_Target = GetExplTargetUnit();
            if (!l_Target)
                return;

            l_Caster->CastSpell(l_Target, SPELL_WARR_SHIELD_CHARGE_CHARGE, true);
            l_Caster->CastSpell(l_Caster, SPELL_WARR_SHIELD_CHARGE_MODIFIER, true);
        }

        void Register()
        {
            OnCast += SpellCastFn(spell_warr_shield_charge_SpellScript::HandleOnCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warr_shield_charge_SpellScript();
    }
};

enum ExecuteSpells
{
    SPELL_EXECUTE_OFFHAND = 163558
};

/// Execute - 5308 (Prot, Fury, Default)
class spell_warr_execute_default: public SpellScriptLoader
{
public:
    spell_warr_execute_default() : SpellScriptLoader("spell_warr_execute_default") { }

    class spell_warr_execute_default_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_warr_execute_default_SpellScript);

        void HandleOnCast()
        {
            if (Unit* l_Target = GetExplTargetUnit())
                if (Player* l_Caster = GetCaster()->ToPlayer())
                    if (l_Caster->GetSpecializationId(l_Caster->GetActiveSpec()) == SPEC_WARRIOR_FURY)
                        l_Caster->CastSpell(l_Target, SPELL_EXECUTE_OFFHAND, true);
        }

        void Register()
        {
            OnCast += SpellCastFn(spell_warr_execute_default_SpellScript::HandleOnCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_warr_execute_default_SpellScript();
    }
};

enum EnhancedRendSpells
{
    SPELL_WARR_ENHANCED_REND_DAMAGE = 174736,
    SPELL_WARR_REND_FINAL_BURST     = 94009,
    SPELL_WARR_REND                 = 772
};

/// Enhanced Rend - 174737
class spell_warr_enhanced_rend: public SpellScriptLoader
{
    public:
        spell_warr_enhanced_rend() : SpellScriptLoader("spell_warr_enhanced_rend") { }

        class spell_warr_enhanced_rend_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_enhanced_rend_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& l_ProcInfo)
            {
                PreventDefaultAction();

                if (Unit* l_Target = l_ProcInfo.GetActionTarget())
                {
                    if (Unit* l_Caster = GetCaster())
                    {
                        if (l_Target->HasAura(SPELL_WARR_REND, l_Caster->GetGUID()))
                            l_Caster->CastSpell(l_Target, SPELL_WARR_ENHANCED_REND_DAMAGE, true);
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_warr_enhanced_rend_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_enhanced_rend_AuraScript();
        }
};

/// Rend - 772
class spell_warr_rend : public SpellScriptLoader
{
    public:
        spell_warr_rend() : SpellScriptLoader("spell_warr_rend") { }

        class spell_warr_rend_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_warr_rend_AuraScript);

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Owner = GetUnitOwner();
                Unit* l_Target = GetTarget();

                if (l_Owner == nullptr || l_Target == nullptr)
                    return;

                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();

                if (l_RemoveMode != AURA_REMOVE_BY_EXPIRE)
                    return;

                l_Owner->CastSpell(l_Target, SPELL_WARR_REND_FINAL_BURST, true);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_warr_rend_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_rend_AuraScript();
        }
};

enum BloodBathSpells
{
    SPELL_BLOOD_BATH        = 12292,
    SPELL_BLOOD_BATH_SNARE  = 147531,
    SPELL_BLOOD_BATH_DAMAGE = 113344
};

/// Blood Bath - 12292
class spell_warr_blood_bath : public SpellScriptLoader
{
    public:
        spell_warr_blood_bath() : SpellScriptLoader("spell_warr_blood_bath") { }

        class spell_warr_blood_bath_Aurascript : public AuraScript
        {
            PrepareAuraScript(spell_warr_blood_bath_Aurascript);

            void HandleOnProc(constAuraEffectPtr aurEff, ProcEventInfo& l_ProcInfo)
            {
                PreventDefaultAction();

                if (!l_ProcInfo.GetDamageInfo() || !l_ProcInfo.GetDamageInfo()->GetDamage() || !l_ProcInfo.GetDamageInfo()->GetSpellInfo())
                    return;

                if (l_ProcInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_BLOOD_BATH_DAMAGE)
                    return;

                if (Unit* l_Target = l_ProcInfo.GetActionTarget())
                {
                    if (Unit* l_Caster = GetCaster())
                    {
                        // 30% additional damage as a bleed over 6 sec
                        int32 l_Damage = (l_ProcInfo.GetDamageInfo()->GetDamage() * sSpellMgr->GetSpellInfo(SPELL_BLOOD_BATH)->Effects[EFFECT_0].BasePoints / 100) / 6;

                        l_Caster->CastSpell(l_Target, SPELL_BLOOD_BATH_SNARE, true);
                        l_Caster->CastSpell(l_Target, SPELL_BLOOD_BATH_DAMAGE, true);

                        if (AuraEffectPtr l_BloodbathActual = l_Target->GetAuraEffect(SPELL_BLOOD_BATH_DAMAGE, EFFECT_0, l_Caster->GetGUID()))
                            l_BloodbathActual->SetAmount(l_Damage);
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_warr_blood_bath_Aurascript::HandleOnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_blood_bath_Aurascript();
        }
};

enum BloodCrazeSpells
{
    SPELL_WARR_BLOOD_CRAZE_HEAL = 159363
};

/// Blood Craze - 159362
class spell_warr_blood_craze : public SpellScriptLoader
{
    public:
        spell_warr_blood_craze() : SpellScriptLoader("spell_warr_blood_craze") { }

        class spell_warr_blood_craze_Aurascript : public AuraScript
        {
            PrepareAuraScript(spell_warr_blood_craze_Aurascript);

            void HandleOnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_ProcInfos)
            {
                PreventDefaultAction();

                if (!(p_ProcInfos.GetHitMask() & PROC_EX_INTERNAL_MULTISTRIKE))
                    return;

                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(SPELL_WARR_BLOOD_CRAZE_HEAL);
                if (l_SpellInfo == nullptr || !l_SpellInfo->GetDuration())
                    return;

                if (Unit* l_Caster = GetCaster())
                {
                    // 3% of your health over 3 sec.
                    int32 l_Health = CalculatePct(l_Caster->GetMaxHealth(), p_AurEff->GetAmount()) / (l_SpellInfo->GetDuration() / IN_MILLISECONDS);

                    l_Caster->CastCustomSpell(l_Caster, SPELL_WARR_BLOOD_CRAZE_HEAL, &l_Health, nullptr, nullptr, true);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_warr_blood_craze_Aurascript::HandleOnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_warr_blood_craze_Aurascript();
        }
};

void AddSC_warrior_spell_scripts()
{
    /// NPCs
    new npc_warrior_ravager();

    /// Spells
    new spell_warr_ravager();
    new spell_warr_rend();
    new spell_warr_slam();
    new spell_warr_victorious_state();
    new spell_warr_glyph_of_hindering_strikes();
    new spell_warr_stampeding_shout();
    new spell_warr_shield_block();
    new spell_warr_storm_bolt();
    new spell_warr_colossus_smash();
    new spell_warr_dragon_roar();
    new spell_warr_staggering_shout();
    new spell_warr_second_wind();
    new spell_warr_sudden_death();
    new spell_warr_berzerker_rage();
    new spell_warr_enrage();
    new spell_warr_mocking_banner();
    new spell_warr_raging_blow();
    new spell_warr_sword_and_board();
    new spell_warr_mortal_strike();
    new spell_warr_rallying_cry();
    new spell_warr_heroic_leap_damage();
    new spell_warr_heroic_leap();
    new spell_warr_shockwave();
    new spell_warr_bloodthirst();
    new spell_warr_victory_rush();
    new spell_warr_last_stand();
    new spell_warr_deep_wounds();
    new spell_warr_charge();
    new spell_warr_shield_wall();
    new spell_warr_spell_reflection();
    new spell_warr_intervene();
    new spell_warr_glyph_of_gag_order();
    new spell_warr_shield_barrier();
    new spell_warr_anger_management();
    new spell_warr_execute();
    new spell_warr_whirlwind();
    new spell_warr_shield_charge();
    new spell_warr_execute_default();
    new spell_warr_enhanced_rend();
    new spell_warr_blood_bath();
    new spell_warr_blood_craze();
}