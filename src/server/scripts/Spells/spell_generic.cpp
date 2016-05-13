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
 * Scripts for spells with SPELLFAMILY_GENERIC which cannot be included in AI script file
 * of creature using it or can't be bound to any player class.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_gen_"
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SkillDiscovery.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "InstanceScript.h"
#include "Group.h"
#include "LFGMgr.h"
#include "Player.h"
#include "GameEventMgr.h"
#include "DB2Stores.h"

class spell_gen_absorb0_hitlimit1: public SpellScriptLoader
{
    public:
        spell_gen_absorb0_hitlimit1() : SpellScriptLoader("spell_gen_absorb0_hitlimit1") { }

        class spell_gen_absorb0_hitlimit1_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_absorb0_hitlimit1_AuraScript);

            uint32 limit;

            bool Load()
            {
                // Max absorb stored in 1 dummy effect
                limit = GetSpellInfo()->Effects[EFFECT_1].CalcValue();
                return true;
            }

            void Absorb(AuraEffect* /*aurEff*/, DamageInfo& /*dmgInfo*/, uint32& absorbAmount)
            {
                absorbAmount = std::min(limit, absorbAmount);
            }

            void Register()
            {
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_gen_absorb0_hitlimit1_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_absorb0_hitlimit1_AuraScript();
        }
};

// 41337 Aura of Anger
class spell_gen_aura_of_anger: public SpellScriptLoader
{
    public:
        spell_gen_aura_of_anger() : SpellScriptLoader("spell_gen_aura_of_anger") { }

        class spell_gen_aura_of_anger_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_aura_of_anger_AuraScript);

            void HandleEffectPeriodicUpdate(AuraEffect* aurEff)
            {
                if (AuraEffect* aurEff1 = aurEff->GetBase()->GetEffect(EFFECT_1))
                    aurEff1->ChangeAmount(aurEff1->GetAmount() + 5);
                aurEff->SetAmount(100 * aurEff->GetTickNumber());
            }

            void Register()
            {
                OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_gen_aura_of_anger_AuraScript::HandleEffectPeriodicUpdate, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_aura_of_anger_AuraScript();
        }
};

class spell_gen_av_drekthar_presence: public SpellScriptLoader
{
    public:
        spell_gen_av_drekthar_presence() : SpellScriptLoader("spell_gen_av_drekthar_presence") { }

        class spell_gen_av_drekthar_presence_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_av_drekthar_presence_AuraScript);

            bool CheckAreaTarget(Unit* target)
            {
                switch (target->GetEntry())
                {
                    // alliance
                    case 14762: // Dun Baldar North Marshal
                    case 14763: // Dun Baldar South Marshal
                    case 14764: // Icewing Marshal
                    case 14765: // Stonehearth Marshal
                    case 11948: // Vandar Stormspike
                    // horde
                    case 14772: // East Frostwolf Warmaster
                    case 14776: // Tower Point Warmaster
                    case 14773: // Iceblood Warmaster
                    case 14777: // West Frostwolf Warmaster
                    case 11946: // Drek'thar
                        return true;
                    default:
                        return false;
                        break;
                }
            }
            void Register()
            {
                DoCheckAreaTarget += AuraCheckAreaTargetFn(spell_gen_av_drekthar_presence_AuraScript::CheckAreaTarget);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_av_drekthar_presence_AuraScript();
        }
};

// 46394 Brutallus Burn
class spell_gen_burn_brutallus: public SpellScriptLoader
{
    public:
        spell_gen_burn_brutallus() : SpellScriptLoader("spell_gen_burn_brutallus") { }

        class spell_gen_burn_brutallus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_burn_brutallus_AuraScript);

            void HandleEffectPeriodicUpdate(AuraEffect* aurEff)
            {
                if (aurEff->GetTickNumber() % 11 == 0)
                    aurEff->SetAmount(aurEff->GetAmount() * 2);
            }

            void Register()
            {
                OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_gen_burn_brutallus_AuraScript::HandleEffectPeriodicUpdate, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_burn_brutallus_AuraScript();
        }
};

enum CannibalizeSpells
{
    SPELL_CANNIBALIZE_TRIGGERED = 20578,
};

class spell_gen_cannibalize: public SpellScriptLoader
{
    public:
        spell_gen_cannibalize() : SpellScriptLoader("spell_gen_cannibalize") { }

        class spell_gen_cannibalize_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_cannibalize_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_CANNIBALIZE_TRIGGERED))
                    return false;
                return true;
            }

            SpellCastResult CheckIfCorpseNear()
            {
                Unit* caster = GetCaster();
                float max_range = GetSpellInfo()->GetMaxRange(false);
                WorldObject* result = NULL;
                // search for nearby enemy corpse in range
                JadeCore::AnyDeadUnitSpellTargetInRangeCheck check(caster, max_range, GetSpellInfo(), TARGET_CHECK_ENEMY);
                JadeCore::WorldObjectSearcher<JadeCore::AnyDeadUnitSpellTargetInRangeCheck> searcher(caster, result, check);
                caster->GetMap()->VisitFirstFound(caster->m_positionX, caster->m_positionY, max_range, searcher);
                if (!result)
                    return SPELL_FAILED_NO_EDIBLE_CORPSES;
                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                caster->CastSpell(caster, SPELL_CANNIBALIZE_TRIGGERED, false);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_gen_cannibalize_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnCheckCast += SpellCheckCastFn(spell_gen_cannibalize_SpellScript::CheckIfCorpseNear);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_cannibalize_SpellScript();
        }
};

// 45472 Parachute
enum ParachuteSpells
{
    SPELL_PARACHUTE         = 45472,
    SPELL_PARACHUTE_BUFF    = 44795,
};

class spell_gen_parachute: public SpellScriptLoader
{
    public:
        spell_gen_parachute() : SpellScriptLoader("spell_gen_parachute") { }

        class spell_gen_parachute_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_parachute_AuraScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PARACHUTE) || !sSpellMgr->GetSpellInfo(SPELL_PARACHUTE_BUFF))
                    return false;
                return true;
            }

            void HandleEffectPeriodic(AuraEffect const* /*aurEff*/)
            {
                if (Player* target = GetTarget()->ToPlayer())
                    if (target->IsFalling())
                    {
                        target->RemoveAurasDueToSpell(SPELL_PARACHUTE);
                        target->CastSpell(target, SPELL_PARACHUTE_BUFF, true);
                    }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_gen_parachute_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_parachute_AuraScript();
        }
};

enum NPCEntries
{
    NPC_DOOMGUARD   = 11859,
    NPC_INFERNAL    = 89,
    NPC_IMP         = 416,
};

class spell_gen_pet_summoned: public SpellScriptLoader
{
    public:
        spell_gen_pet_summoned() : SpellScriptLoader("spell_gen_pet_summoned") { }

        class spell_gen_pet_summoned_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_pet_summoned_SpellScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Player* player = GetCaster()->ToPlayer();
                if (player->GetLastPetNumber())
                {
                    PetType newPetType = (player->getClass() == CLASS_HUNTER) ? HUNTER_PET : SUMMON_PET;

                    if (Pet* l_NewPet = new Pet(player, newPetType))
                    {
                        PreparedStatement* l_PetStatement = PetQueryHolder::GenerateFirstLoadStatement(0, player->GetLastPetNumber(), player->GetGUIDLow(), true, PET_SLOT_UNK_SLOT);
                        uint64 l_PlayerGUID = player->GetGUID();
                        uint32 l_PetNumber  = player->GetLastPetNumber();

                        CharacterDatabase.AsyncQuery(l_PetStatement, [l_NewPet, l_PlayerGUID, l_PetNumber](PreparedQueryResult p_Result) -> void
                        {
                            if (!p_Result)
                            {
                                delete l_NewPet;
                                return;
                            }

                            PetQueryHolder* l_PetHolder = new PetQueryHolder(p_Result->Fetch()[0].GetUInt32(), p_Result);
                            l_PetHolder->Initialize();

                            auto l_QueryHolderResultFuture = CharacterDatabase.DelayQueryHolder(l_PetHolder);

                            sWorld->AddQueryHolderCallback(QueryHolderCallback(l_QueryHolderResultFuture, [l_NewPet, l_PlayerGUID, l_PetNumber](SQLQueryHolder* p_QueryHolder) -> void
                            {
                                Player* l_Player = sObjectAccessor->FindPlayer(l_PlayerGUID);
                                if (!l_Player || !p_QueryHolder)
                                {
                                    delete l_NewPet;
                                    return;
                                }

                                l_NewPet->LoadPetFromDB(l_Player, 0, l_PetNumber, true, PET_SLOT_UNK_SLOT, false, (PetQueryHolder*)p_QueryHolder, [](Pet* p_Pet, bool p_Result) -> void
                                {
                                    if (!p_Result)
                                    {
                                        delete p_Pet;
                                        return;
                                    }

                                    // revive the pet if it is dead
                                    if (p_Pet->getDeathState() == DEAD || p_Pet->getDeathState() == CORPSE)
                                        p_Pet->setDeathState(ALIVE);

                                    p_Pet->ClearUnitState(uint32(UNIT_STATE_ALL_STATE));
                                    p_Pet->SetFullHealth();
                                    p_Pet->SetPower(p_Pet->getPowerType(), p_Pet->GetMaxPower(p_Pet->getPowerType()));

                                    switch (p_Pet->GetEntry())
                                    {
                                        case NPC_DOOMGUARD:
                                        case NPC_INFERNAL:
                                            p_Pet->SetEntry(NPC_IMP);
                                            break;
                                        default:
                                            break;
                                    }
                                });
                            }));
                        });
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_pet_summoned_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_pet_summoned_SpellScript();
        }
};

class spell_gen_remove_flight_auras: public SpellScriptLoader
{
    public:
        spell_gen_remove_flight_auras() : SpellScriptLoader("spell_gen_remove_flight_auras") {}

        class spell_gen_remove_flight_auras_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_remove_flight_auras_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetHitUnit())
                {
                    target->RemoveAurasByType(SPELL_AURA_FLY);
                    target->RemoveAurasByType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_remove_flight_auras_SpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_remove_flight_auras_SpellScript();
        }
};

// 66118 Leeching Swarm
enum LeechingSwarmSpells
{
    SPELL_LEECHING_SWARM_DMG    = 66240,
    SPELL_LEECHING_SWARM_HEAL   = 66125,
};

class spell_gen_leeching_swarm: public SpellScriptLoader
{
    public:
        spell_gen_leeching_swarm() : SpellScriptLoader("spell_gen_leeching_swarm") { }

        class spell_gen_leeching_swarm_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_leeching_swarm_AuraScript);

            bool Validate(SpellInfo const* spellEntry)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_LEECHING_SWARM_DMG) || !sSpellMgr->GetSpellInfo(SPELL_LEECHING_SWARM_HEAL))
                    return false;
                return true;
            }

            void HandleEffectPeriodic(AuraEffect const* aurEff)
            {
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                bool l_IsValid = true;
                if (caster->GetMapId() != 649)
                {
                    caster->RemoveAura(GetSpellInfo()->Id);
                    caster->RemoveAurasDueToSpell(GetSpellInfo()->Id);
                    l_IsValid = false;
                    Remove();
                    SetDuration(0);
                }

                if (Unit* target = GetTarget())
                {
                    if (!l_IsValid)
                    {
                        target->RemoveAura(GetSpellInfo()->Id);
                        target->RemoveAurasDueToSpell(GetSpellInfo()->Id);
                        return;
                    }

                    int32 lifeLeeched = target->CountPctFromCurHealth(aurEff->GetAmount());
                    if (lifeLeeched < 250)
                        lifeLeeched = 250;
                    // Damage
                    caster->CastCustomSpell(target, SPELL_LEECHING_SWARM_DMG, &lifeLeeched, 0, 0, false);
                    // Heal
                    caster->CastCustomSpell(caster, SPELL_LEECHING_SWARM_HEAL, &lifeLeeched, 0, 0, false);
                }
            }

            void CheckSpell(AuraEffect const* aurEff, bool& isPeriodic, int32& amplitude)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Owner = GetUnitOwner();

                Unit* l_TabUnit[2] = { l_Caster, l_Owner };

                for (uint8 l_Idx = 0; l_Idx < 2; ++l_Idx)
                {
                    if (l_TabUnit[l_Idx])
                    {
                        if (l_TabUnit[l_Idx]->GetMapId() != 649)
                        {
                            isPeriodic = false;
                            Remove();
                            break;
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_gen_leeching_swarm_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
                DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_gen_leeching_swarm_AuraScript::CheckSpell, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_leeching_swarm_AuraScript();
        }

        class spell_gen_leeching_swarm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_leeching_swarm_SpellScript);

            SpellCastResult CheckMap()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->GetMapId() != 649)
                        return SPELL_FAILED_INCORRECT_AREA;
                    else
                        return SPELL_CAST_OK;
                }
                else
                    return SPELL_FAILED_CASTER_DEAD;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_gen_leeching_swarm_SpellScript::CheckMap);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_leeching_swarm_SpellScript();
        }
};

enum EluneCandle
{
    NPC_OMEN = 15467,

    SPELL_ELUNE_CANDLE_OMEN_HEAD   = 26622,
    SPELL_ELUNE_CANDLE_OMEN_CHEST  = 26624,
    SPELL_ELUNE_CANDLE_OMEN_HAND_R = 26625,
    SPELL_ELUNE_CANDLE_OMEN_HAND_L = 26649,
    SPELL_ELUNE_CANDLE_NORMAL      = 26636,
};

class spell_gen_elune_candle: public SpellScriptLoader
{
    public:
        spell_gen_elune_candle() : SpellScriptLoader("spell_gen_elune_candle") {}

        class spell_gen_elune_candle_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_elune_candle_SpellScript);
            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_ELUNE_CANDLE_OMEN_HEAD))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_ELUNE_CANDLE_OMEN_CHEST))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_ELUNE_CANDLE_OMEN_HAND_R))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_ELUNE_CANDLE_OMEN_HAND_L))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_ELUNE_CANDLE_NORMAL))
                    return false;
                return true;
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                uint32 spellId = 0;

                if (GetHitUnit()->GetEntry() == NPC_OMEN)
                {
                    switch (urand(0, 3))
                    {
                        case 0: spellId = SPELL_ELUNE_CANDLE_OMEN_HEAD; break;
                        case 1: spellId = SPELL_ELUNE_CANDLE_OMEN_CHEST; break;
                        case 2: spellId = SPELL_ELUNE_CANDLE_OMEN_HAND_R; break;
                        case 3: spellId = SPELL_ELUNE_CANDLE_OMEN_HAND_L; break;
                    }
                }
                else
                    spellId = SPELL_ELUNE_CANDLE_NORMAL;

                GetCaster()->CastSpell(GetHitUnit(), spellId, true, NULL);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_elune_candle_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_elune_candle_SpellScript();
        }
};

class spell_creature_permanent_feign_death: public SpellScriptLoader
{
    public:
        spell_creature_permanent_feign_death() : SpellScriptLoader("spell_creature_permanent_feign_death") { }

        class spell_creature_permanent_feign_death_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_creature_permanent_feign_death_AuraScript);

            void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                target->SetFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_DEAD);
                target->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_FEIGN_DEATH);

                if (target->GetTypeId() == TYPEID_UNIT)
                    target->ToCreature()->SetReactState(REACT_PASSIVE);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_creature_permanent_feign_death_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_creature_permanent_feign_death_AuraScript();
        }
};

enum PvPTrinketTriggeredSpells
{
    SPELL_WILL_OF_THE_FORSAKEN_COOLDOWN_TRIGGER         = 72752,
    SPELL_WILL_OF_THE_FORSAKEN_COOLDOWN_TRIGGER_WOTF    = 72757,
};

class spell_pvp_trinket_wotf_shared_cd: public SpellScriptLoader
{
    public:
        spell_pvp_trinket_wotf_shared_cd() : SpellScriptLoader("spell_pvp_trinket_wotf_shared_cd") {}

        class spell_pvp_trinket_wotf_shared_cd_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pvp_trinket_wotf_shared_cd_SpellScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_WILL_OF_THE_FORSAKEN_COOLDOWN_TRIGGER) || !sSpellMgr->GetSpellInfo(SPELL_WILL_OF_THE_FORSAKEN_COOLDOWN_TRIGGER_WOTF))
                    return false;
                return true;
            }

            void HandleScript()
            {
                // This is only needed because spells cast from spell_linked_spell are triggered by default
                // Spell::SendSpellCooldown() skips all spells with TRIGGERED_IGNORE_SPELL_AND_CATEGORY_CD
                GetCaster()->ToPlayer()->AddSpellAndCategoryCooldowns(GetSpellInfo(), GetCastItem() ? GetCastItem()->GetEntry() : 0, GetSpell());
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_pvp_trinket_wotf_shared_cd_SpellScript::HandleScript);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pvp_trinket_wotf_shared_cd_SpellScript();
        }
};

enum AnimalBloodPoolSpell
{
    SPELL_ANIMAL_BLOOD      = 46221,
    SPELL_SPAWN_BLOOD_POOL  = 63471,
};

class spell_gen_animal_blood: public SpellScriptLoader
{
    public:
        spell_gen_animal_blood() : SpellScriptLoader("spell_gen_animal_blood") { }

        class spell_gen_animal_blood_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_animal_blood_AuraScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SPAWN_BLOOD_POOL))
                    return false;
                return true;
            }

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                // Remove all auras with spell id 46221, except the one currently being applied
                while (Aura* aur = GetUnitOwner()->GetOwnedAura(SPELL_ANIMAL_BLOOD, 0, 0, 0, GetAura()))
                    GetUnitOwner()->RemoveOwnedAura(aur);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* owner = GetUnitOwner())
                    if (owner->IsInWater())
                        owner->CastSpell(owner, SPELL_SPAWN_BLOOD_POOL, true);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectRemoveFn(spell_gen_animal_blood_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_gen_animal_blood_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_animal_blood_AuraScript();
        }
};

enum DivineStormSpell
{
    SPELL_DIVINE_STORM  = 53385,
};

// 70769 Divine Storm!
class spell_gen_divine_storm_cd_reset: public SpellScriptLoader
{
    public:
        spell_gen_divine_storm_cd_reset() : SpellScriptLoader("spell_gen_divine_storm_cd_reset") {}

        class spell_gen_divine_storm_cd_reset_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_divine_storm_cd_reset_SpellScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_DIVINE_STORM))
                    return false;
                return true;
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Player* caster = GetCaster()->ToPlayer();
                if (caster->HasSpellCooldown(SPELL_DIVINE_STORM))
                    caster->RemoveSpellCooldown(SPELL_DIVINE_STORM, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_divine_storm_cd_reset_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_divine_storm_cd_reset_SpellScript();
        }
};

class spell_gen_gunship_portal: public SpellScriptLoader
{
    public:
        spell_gen_gunship_portal() : SpellScriptLoader("spell_gen_gunship_portal") { }

        class spell_gen_gunship_portal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_gunship_portal_SpellScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Player* caster = GetCaster()->ToPlayer();
                if (Battleground* bg = caster->GetBattleground())
                    if (bg->GetTypeID(true) == BATTLEGROUND_IC)
                        bg->DoAction(1, caster->GetGUID());
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_gunship_portal_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_gunship_portal_SpellScript();
        }
};

enum parachuteIC
{
    SPELL_PARACHUTE_IC = 66657,
};

class spell_gen_parachute_ic: public SpellScriptLoader
{
    public:
        spell_gen_parachute_ic() : SpellScriptLoader("spell_gen_parachute_ic") { }

        class spell_gen_parachute_ic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_parachute_ic_AuraScript)

            void HandleTriggerSpell(AuraEffect const* /*aurEff*/)
            {
                if (Player* target = GetTarget()->ToPlayer())
                    if (target->m_movementInfo.fallTime > 2000)
                        target->CastSpell(target, SPELL_PARACHUTE_IC, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_gen_parachute_ic_AuraScript::HandleTriggerSpell, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_parachute_ic_AuraScript();
        }
};

class spell_gen_dungeon_credit: public SpellScriptLoader
{
    public:
        spell_gen_dungeon_credit() : SpellScriptLoader("spell_gen_dungeon_credit") { }

        class spell_gen_dungeon_credit_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_dungeon_credit_SpellScript);

            bool Load()
            {
                _handled = false;
                return GetCaster()->GetTypeId() == TYPEID_UNIT;
            }

            void CreditEncounter()
            {
                // This hook is executed for every target, make sure we only credit instance once
                if (_handled)
                    return;

                _handled = true;
                Unit* caster = GetCaster();
                if (InstanceScript* instance = caster->GetInstanceScript())
                    instance->UpdateEncounterState(ENCOUNTER_CREDIT_CAST_SPELL, GetSpellInfo()->Id, caster);
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_gen_dungeon_credit_SpellScript::CreditEncounter);
            }

            bool _handled;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_dungeon_credit_SpellScript();
        }
};

class spell_gen_profession_research: public SpellScriptLoader
{
    public:
        spell_gen_profession_research() : SpellScriptLoader("spell_gen_profession_research") {}

        class spell_gen_profession_research_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_profession_research_SpellScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }

            SpellCastResult CheckRequirement()
            {
                if (HasDiscoveredAllSpells(GetSpellInfo()->Id, GetCaster()->ToPlayer()))
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_NOTHING_TO_DISCOVER);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_gen_profession_research_SpellScript::CheckRequirement);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_profession_research_SpellScript();
        }
};

class spell_generic_clone: public SpellScriptLoader
{
    public:
        spell_generic_clone() : SpellScriptLoader("spell_generic_clone") { }

        class spell_generic_clone_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_generic_clone_SpellScript);

            void HandleScriptEffect(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                Unit* l_Caster = GetCaster();

                if (Unit* l_Target = GetHitUnit())
                    l_Caster->CastSpell(l_Target,  uint32(GetEffectValue()), true);
            }

            void Register()
            {
                switch (m_scriptSpellId)
                {
                case 106935: /// Figment of Doubt
                case 136757:
                    OnEffectHitTarget += SpellEffectFn(spell_generic_clone_SpellScript::HandleScriptEffect, EFFECT_2, SPELL_EFFECT_SCRIPT_EFFECT);
                    break;
                case 126240:
                    OnEffectHitTarget += SpellEffectFn(spell_generic_clone_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
                    break;
                case 57528:
                    OnEffectHitTarget += SpellEffectFn(spell_generic_clone_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_DUMMY);
                    OnEffectHitTarget += SpellEffectFn(spell_generic_clone_SpellScript::HandleScriptEffect, EFFECT_2, SPELL_EFFECT_DUMMY);
                    break;
                default:
                    OnEffectHitTarget += SpellEffectFn(spell_generic_clone_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
                    OnEffectHitTarget += SpellEffectFn(spell_generic_clone_SpellScript::HandleScriptEffect, EFFECT_2, SPELL_EFFECT_SCRIPT_EFFECT);
                    break;
                }
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_generic_clone_SpellScript();
        }
};

enum CloneWeaponSpells
{
    SPELL_COPY_WEAPON_AURA       = 41055,
    SPELL_COPY_WEAPON_2_AURA     = 63416,
    SPELL_COPY_WEAPON_3_AURA     = 69891,

    SPELL_COPY_OFFHAND_AURA      = 45206,
    SPELL_COPY_OFFHAND_2_AURA    = 69892,

    SPELL_COPY_RANGED_AURA       = 57594
};

class spell_generic_clone_weapon: public SpellScriptLoader
{
    public:
        spell_generic_clone_weapon() : SpellScriptLoader("spell_generic_clone_weapon") { }

        class spell_generic_clone_weapon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_generic_clone_weapon_SpellScript);

            void HandleScriptEffect(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                Unit* l_Caster = GetCaster();

                if (Unit* l_Target = GetHitUnit())
                    l_Caster->CastSpell(l_Target,  uint32(GetEffectValue()), true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_generic_clone_weapon_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_generic_clone_weapon_SpellScript();
        }
};

class spell_gen_clone_weapon_aura: public SpellScriptLoader
{
    public:
        spell_gen_clone_weapon_aura() : SpellScriptLoader("spell_gen_clone_weapon_aura") { }

            class spell_gen_clone_weapon_auraScript : public AuraScript
            {
                PrepareAuraScript(spell_gen_clone_weapon_auraScript);

                uint32 prevItem;

                bool Validate(SpellInfo const* /*spellEntry*/)
                {
                    if (!sSpellMgr->GetSpellInfo(SPELL_COPY_WEAPON_AURA) ||
                        !sSpellMgr->GetSpellInfo(SPELL_COPY_WEAPON_2_AURA) ||
                        !sSpellMgr->GetSpellInfo(SPELL_COPY_WEAPON_3_AURA) ||
                        !sSpellMgr->GetSpellInfo(SPELL_COPY_OFFHAND_AURA) ||
                        !sSpellMgr->GetSpellInfo(SPELL_COPY_OFFHAND_2_AURA) ||
                        !sSpellMgr->GetSpellInfo(SPELL_COPY_RANGED_AURA))
                        return false;
                    return true;
                }

                void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
                {
                    Unit* caster = GetCaster();
                    Unit* target = GetTarget();

                    if (!caster)
                        return;

                    switch (GetSpellInfo()->Id)
                    {
                        case SPELL_COPY_WEAPON_AURA:
                        case SPELL_COPY_WEAPON_2_AURA:
                        case SPELL_COPY_WEAPON_3_AURA:
                        {
                            prevItem = target->GetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS);

                            if (Player* player = caster->ToPlayer())
                            {
                                if (Item* mainItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND))
                                    target->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS, mainItem->GetEntry());
                            }
                            else
                                target->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS, caster->GetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS));
                            break;
                        case SPELL_COPY_OFFHAND_AURA:
                        case SPELL_COPY_OFFHAND_2_AURA:
                        {
                            prevItem = target->GetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS + 2);

                            if (Player* player = caster->ToPlayer())
                            {
                                if (Item* offItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND))
                                    target->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS + 2, offItem->GetEntry());
                            }
                            else
                                target->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS + 2, caster->GetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS + 2));
                            break;
                        }
                        case SPELL_COPY_RANGED_AURA:
                        {
                            prevItem = target->GetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS + 4);

                            if (Player* player = caster->ToPlayer())
                            {
                                if (Item* rangedItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED))
                                    target->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS + 4, rangedItem->GetEntry());
                            }
                            else
                                target->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS + 4, caster->GetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS + 4));
                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();

                switch (GetSpellInfo()->Id)
                {
                    case SPELL_COPY_WEAPON_AURA:
                    case SPELL_COPY_WEAPON_2_AURA:
                    case SPELL_COPY_WEAPON_3_AURA:
                       target->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS, prevItem);
                       break;
                    case SPELL_COPY_OFFHAND_AURA:
                    case SPELL_COPY_OFFHAND_2_AURA:
                        target->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS + 2, prevItem);
                        break;
                    case SPELL_COPY_RANGED_AURA:
                        target->SetUInt32Value(UNIT_FIELD_VIRTUAL_ITEMS + 4, prevItem);
                        break;
                    default:
                        break;
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_gen_clone_weapon_auraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                OnEffectRemove += AuraEffectRemoveFn(spell_gen_clone_weapon_auraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }

        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_gen_clone_weapon_auraScript();
        }
};

enum FriendOrFowl
{
    SPELL_TURKEY_VENGEANCE  = 25285,
};

class spell_gen_turkey_marker: public SpellScriptLoader
{
    public:
        spell_gen_turkey_marker() : SpellScriptLoader("spell_gen_turkey_marker") { }

        class spell_gen_turkey_marker_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_turkey_marker_AuraScript);

            void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                // store stack apply times, so we can pop them while they expire
                _applyTimes.push_back(getMSTime());
                Unit* target = GetTarget();

                // on stack 15 cast the achievement crediting spell
                if (GetStackAmount() >= 15)
                    target->CastSpell(target, SPELL_TURKEY_VENGEANCE, true, NULL, aurEff, GetCasterGUID());
            }

            void OnPeriodic(AuraEffect const* /*aurEff*/)
            {
                if (_applyTimes.empty())
                    return;

                // pop stack if it expired for us
                if (_applyTimes.front() + GetMaxDuration() < getMSTime())
                    ModStackAmount(-1, AURA_REMOVE_BY_EXPIRE);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_gen_turkey_marker_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_gen_turkey_marker_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }

            std::list<uint32> _applyTimes;
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_turkey_marker_AuraScript();
        }
};

enum MagicRoosterSpells
{
    SPELL_MAGIC_ROOSTER_NORMAL          = 66122,
    SPELL_MAGIC_ROOSTER_DRAENEI_MALE    = 66123,
    SPELL_MAGIC_ROOSTER_TAUREN_MALE     = 66124,
};

class spell_gen_magic_rooster: public SpellScriptLoader
{
    public:
        spell_gen_magic_rooster() : SpellScriptLoader("spell_gen_magic_rooster") { }

        class spell_gen_magic_rooster_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_magic_rooster_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (Player* target = GetHitPlayer())
                {
                    // prevent client crashes from stacking mounts
                    target->RemoveAurasByType(SPELL_AURA_MOUNTED);

                    uint32 spellId = SPELL_MAGIC_ROOSTER_NORMAL;
                    switch (target->getRace())
                    {
                        case RACE_DRAENEI:
                            if (target->getGender() == GENDER_MALE)
                                spellId = SPELL_MAGIC_ROOSTER_DRAENEI_MALE;
                            break;
                        case RACE_TAUREN:
                            if (target->getGender() == GENDER_MALE)
                                spellId = SPELL_MAGIC_ROOSTER_TAUREN_MALE;
                            break;
                        default:
                            break;
                    }

                    target->CastSpell(target, spellId, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_magic_rooster_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_magic_rooster_SpellScript();
        }
};

class spell_gen_allow_cast_from_item_only: public SpellScriptLoader
{
    public:
        spell_gen_allow_cast_from_item_only() : SpellScriptLoader("spell_gen_allow_cast_from_item_only") { }

        class spell_gen_allow_cast_from_item_only_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_allow_cast_from_item_only_SpellScript);

            SpellCastResult CheckRequirement()
            {
                if (!GetCastItem())
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_gen_allow_cast_from_item_only_SpellScript::CheckRequirement);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_allow_cast_from_item_only_SpellScript();
        }
};

enum Launch
{
    SPELL_LAUNCH_NO_FALLING_DAMAGE = 66251
};

class spell_gen_launch: public SpellScriptLoader
{
    public:
        spell_gen_launch() : SpellScriptLoader("spell_gen_launch") {}

        class spell_gen_launch_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_launch_SpellScript);

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (Player* player = GetHitPlayer())
                    player->AddAura(SPELL_LAUNCH_NO_FALLING_DAMAGE, player); // prevents falling damage
            }

            void Launch()
            {
                WorldLocation const* const position = GetExplTargetDest();

                if (Player* player = GetHitPlayer())
                {
                    player->ExitVehicle();

                    // A better research is needed
                    // There is no spell for this, the following calculation was based on void Spell::CalculateJumpSpeeds

                    float speedZ = 10.0f;
                    float dist = position->GetExactDist2d(player->GetPositionX(), player->GetPositionY());
                    float speedXY = dist;

                    player->GetMotionMaster()->MoveJump(position->GetPositionX(), position->GetPositionY(), position->GetPositionZ(), speedXY, speedZ);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_launch_SpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_FORCE_CAST);
                AfterHit += SpellHitFn(spell_gen_launch_SpellScript::Launch);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_launch_SpellScript();
        }
};

enum VehicleScaling
{
    SPELL_GEAR_SCALING      = 66668,
};

class spell_gen_vehicle_scaling: public SpellScriptLoader
{
    public:
        spell_gen_vehicle_scaling() : SpellScriptLoader("spell_gen_vehicle_scaling") { }

        class spell_gen_vehicle_scaling_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_vehicle_scaling_SpellScript);

            SpellCastResult CheckCast()
            {
                if (Unit* target = GetExplTargetUnit())
                    if (target->IsPlayer())
                        return SPELL_FAILED_DONT_REPORT;

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_gen_vehicle_scaling_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_vehicle_scaling_SpellScript();
        }

        class spell_gen_vehicle_scaling_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_vehicle_scaling_AuraScript);

            bool Load()
            {
                return GetCaster() && GetCaster()->IsPlayer();
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                Unit* caster = GetCaster();
                float factor;
                uint16 baseItemLevel;

                // TODO: Reserach coeffs for different vehicles
                switch (GetId())
                {
                    case SPELL_GEAR_SCALING:
                        factor = 1.0f;
                        baseItemLevel = 405;
                        break;
                    default:
                        factor = 1.0f;
                        baseItemLevel = 405;
                        break;
                }

                uint16 avgILvl = caster->ToPlayer()->GetAverageItemLevelEquipped();
                if (avgILvl < baseItemLevel)
                    return;                     // TODO: Research possibility of scaling down

                amount = uint16((avgILvl - baseItemLevel) * factor);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_vehicle_scaling_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_HEALING_PCT);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_vehicle_scaling_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_vehicle_scaling_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_vehicle_scaling_AuraScript();
        }
};

class spell_gen_oracle_wolvar_reputation: public SpellScriptLoader
{
    public:
        spell_gen_oracle_wolvar_reputation() : SpellScriptLoader("spell_gen_oracle_wolvar_reputation") { }

        class spell_gen_oracle_wolvar_reputation_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_oracle_wolvar_reputation_SpellScript);

            bool Load()
            {
                return GetCaster()->IsPlayer();
            }

            void HandleDummy(SpellEffIndex effIndex)
            {
                Player* player = GetCaster()->ToPlayer();
                uint32 factionId = GetSpellInfo()->Effects[effIndex].CalcValue();
                int32  repChange =  GetSpellInfo()->Effects[EFFECT_1].CalcValue();

                FactionEntry const* factionEntry = sFactionStore.LookupEntry(factionId);

                if (!factionEntry)
                    return;

                // Set rep to baserep + basepoints (expecting spillover for oposite faction -> become hated)
                // Not when player already has equal or higher rep with this faction
                if (player->GetReputationMgr().GetBaseReputation(factionEntry) < repChange)
                    player->GetReputationMgr().SetReputation(factionEntry, repChange);

                // EFFECT_INDEX_2 most likely update at war state, we already handle this in SetReputation
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_gen_oracle_wolvar_reputation_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_oracle_wolvar_reputation_SpellScript();
        }
};

class spell_gen_luck_of_the_draw: public SpellScriptLoader
{
    public:
        spell_gen_luck_of_the_draw() : SpellScriptLoader("spell_gen_luck_of_the_draw") { }

        class spell_gen_luck_of_the_draw_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_luck_of_the_draw_AuraScript);

            bool Load()
            {
                return GetUnitOwner()->IsPlayer();
            }

            // cheap hax to make it have update calls
            void CalcPeriodic(AuraEffect const* /*effect*/, bool& isPeriodic, int32& amplitude)
            {
                isPeriodic = true;
                amplitude = 5 * IN_MILLISECONDS;
            }

            void Update(AuraEffect* /*effect*/)
            {
                if (Player* owner = GetUnitOwner()->ToPlayer())
                {
                    const LfgDungeonSet dungeons = sLFGMgr->GetSelectedDungeons(owner->GetGUID());
                    LfgDungeonSet::const_iterator itr = dungeons.begin();

                    if (itr == dungeons.end())
                    {
                        Remove(AURA_REMOVE_BY_DEFAULT);
                        return;
                    }


                    LFGDungeonEntry const* randomDungeon = sLFGDungeonStore.LookupEntry(*itr);
                    if (Group* group = owner->GetGroup())
                        if (Map const* map = owner->GetMap())
                            if (group->isLFGGroup())
                                if (uint32 dungeonId = sLFGMgr->GetDungeon(group->GetGUID(), true))
                                    if (LFGDungeonEntry const* dungeon = sLFGDungeonStore.LookupEntry(dungeonId))
                                        if (uint32(dungeon->map) == map->GetId() && dungeon->difficulty == uint32(map->GetDifficultyID()))
                                            if (randomDungeon && randomDungeon->type == TYPEID_RANDOM_DUNGEON)
                                                return; // in correct dungeon

                    Remove(AURA_REMOVE_BY_DEFAULT);
                }
            }

            void Register()
            {
                DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_gen_luck_of_the_draw_AuraScript::CalcPeriodic, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
                OnEffectUpdatePeriodic += AuraEffectUpdatePeriodicFn(spell_gen_luck_of_the_draw_AuraScript::Update, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_luck_of_the_draw_AuraScript();
        }
};

enum DummyTrigger
{
    SPELL_PERSISTANT_SHIELD_TRIGGERED       = 26470,
    SPELL_PERSISTANT_SHIELD                 = 26467,
};

class spell_gen_dummy_trigger: public SpellScriptLoader
{
    public:
        spell_gen_dummy_trigger() : SpellScriptLoader("spell_gen_dummy_trigger") { }

        class spell_gen_dummy_trigger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_dummy_trigger_SpellScript);

            bool Validate(SpellInfo const* /*SpellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PERSISTANT_SHIELD_TRIGGERED) || !sSpellMgr->GetSpellInfo(SPELL_PERSISTANT_SHIELD))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                int32 damage = GetEffectValue();
                Unit* caster = GetCaster();
                if (Unit* target = GetHitUnit())
                    if (SpellInfo const* triggeredByAuraSpell = GetTriggeringSpell())
                        if (triggeredByAuraSpell->Id == SPELL_PERSISTANT_SHIELD_TRIGGERED)
                            caster->CastCustomSpell(target, SPELL_PERSISTANT_SHIELD_TRIGGERED, &damage, NULL, NULL, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_dummy_trigger_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_dummy_trigger_SpellScript();
        }

};

class spell_gen_spirit_healer_res: public SpellScriptLoader
{
    public:
        spell_gen_spirit_healer_res(): SpellScriptLoader("spell_gen_spirit_healer_res") { }

        class spell_gen_spirit_healer_res_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_spirit_healer_res_SpellScript);

            bool Load()
            {
                return GetOriginalCaster() && GetOriginalCaster()->IsPlayer();
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Player* l_OriginalCaster = GetOriginalCaster()->ToPlayer();

                if (Unit * l_Target = GetHitUnit())
                {
                    WorldPacket l_Data(SMSG_SPIRIT_HEALER_CONFIRM, 16 + 2);
                    l_Data.appendPackGUID(l_Target->GetGUID());

                    l_OriginalCaster->GetSession()->SendPacket(&l_Data);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_spirit_healer_res_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_spirit_healer_res_SpellScript();
        }
};

enum TransporterBackfires
{
    SPELL_TRANSPORTER_MALFUNCTION_POLYMORPH     = 23444,
    SPELL_TRANSPORTER_EVIL_TWIN                 = 23445,
    SPELL_TRANSPORTER_MALFUNCTION_MISS          = 36902,
};

class spell_gen_gadgetzan_transporter_backfire: public SpellScriptLoader
{
    public:
        spell_gen_gadgetzan_transporter_backfire() : SpellScriptLoader("spell_gen_gadgetzan_transporter_backfire") { }

        class spell_gen_gadgetzan_transporter_backfire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_gadgetzan_transporter_backfire_SpellScript)

            bool Validate(SpellInfo const* /*SpellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_TRANSPORTER_MALFUNCTION_POLYMORPH) || !sSpellMgr->GetSpellInfo(SPELL_TRANSPORTER_EVIL_TWIN)
                    || !sSpellMgr->GetSpellInfo(SPELL_TRANSPORTER_MALFUNCTION_MISS))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Unit* caster = GetCaster();
                int32 r = irand(0, 119);
                if (r < 20)                           // Transporter Malfunction - 1/6 polymorph
                    caster->CastSpell(caster, SPELL_TRANSPORTER_MALFUNCTION_POLYMORPH, true);
                else if (r < 100)                     // Evil Twin               - 4/6 evil twin
                    caster->CastSpell(caster, SPELL_TRANSPORTER_EVIL_TWIN, true);
                else                                    // Transporter Malfunction - 1/6 miss the target
                    caster->CastSpell(caster, SPELL_TRANSPORTER_MALFUNCTION_MISS, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_gadgetzan_transporter_backfire_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_gadgetzan_transporter_backfire_SpellScript();
        }
};

enum GnomishTransporter
{
    SPELL_TRANSPORTER_SUCCESS                   = 23441,
    SPELL_TRANSPORTER_FAILURE                   = 23446,
};

class spell_gen_gnomish_transporter: public SpellScriptLoader
{
    public:
        spell_gen_gnomish_transporter() : SpellScriptLoader("spell_gen_gnomish_transporter") { }

        class spell_gen_gnomish_transporter_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_gnomish_transporter_SpellScript)

            bool Validate(SpellInfo const* /*SpellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_TRANSPORTER_SUCCESS) || !sSpellMgr->GetSpellInfo(SPELL_TRANSPORTER_FAILURE))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                Unit* caster = GetCaster();
                caster->CastSpell(caster, roll_chance_i(50) ? SPELL_TRANSPORTER_SUCCESS : SPELL_TRANSPORTER_FAILURE , true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_gnomish_transporter_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_gnomish_transporter_SpellScript();
        }
};

enum DalaranDisguiseSpells
{
    SPELL_SUNREAVER_DISGUISE_TRIGGER       = 69672,
    SPELL_SUNREAVER_DISGUISE_FEMALE        = 70973,
    SPELL_SUNREAVER_DISGUISE_MALE          = 70974,

    SPELL_SILVER_COVENANT_DISGUISE_TRIGGER = 69673,
    SPELL_SILVER_COVENANT_DISGUISE_FEMALE  = 70971,
    SPELL_SILVER_COVENANT_DISGUISE_MALE    = 70972,
};

class spell_gen_dalaran_disguise: public SpellScriptLoader
{
    public:
        spell_gen_dalaran_disguise(const char* name) : SpellScriptLoader(name) {}

        class spell_gen_dalaran_disguise_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_dalaran_disguise_SpellScript);
            bool Validate(SpellInfo const* spellEntry)
            {
                switch (spellEntry->Id)
                {
                    case SPELL_SUNREAVER_DISGUISE_TRIGGER:
                        if (!sSpellMgr->GetSpellInfo(SPELL_SUNREAVER_DISGUISE_FEMALE) || !sSpellMgr->GetSpellInfo(SPELL_SUNREAVER_DISGUISE_MALE))
                            return false;
                        break;
                    case SPELL_SILVER_COVENANT_DISGUISE_TRIGGER:
                        if (!sSpellMgr->GetSpellInfo(SPELL_SILVER_COVENANT_DISGUISE_FEMALE) || !sSpellMgr->GetSpellInfo(SPELL_SILVER_COVENANT_DISGUISE_MALE))
                            return false;
                        break;
                }
                return true;
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                if (Player* player = GetHitPlayer())
                {
                    uint8 gender = player->getGender();

                    uint32 spellId = GetSpellInfo()->Id;

                    switch (spellId)
                    {
                        case SPELL_SUNREAVER_DISGUISE_TRIGGER:
                            spellId = gender ? SPELL_SUNREAVER_DISGUISE_FEMALE : SPELL_SUNREAVER_DISGUISE_MALE;
                            break;
                        case SPELL_SILVER_COVENANT_DISGUISE_TRIGGER:
                            spellId = gender ? SPELL_SILVER_COVENANT_DISGUISE_FEMALE : SPELL_SILVER_COVENANT_DISGUISE_MALE;
                            break;
                        default:
                            break;
                    }
                    GetCaster()->CastSpell(player, spellId, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_dalaran_disguise_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_dalaran_disguise_SpellScript();
        }
};

/* DOCUMENTATION: Break-Shield spells
    Break-Shield spells can be classified in three groups:

        - Spells on vehicle bar used by players:
            + EFFECT_0: SCRIPT_EFFECT
            + EFFECT_1: NONE
            + EFFECT_2: NONE
        - Spells casted by players triggered by script:
            + EFFECT_0: SCHOOL_DAMAGE
            + EFFECT_1: SCRIPT_EFFECT
            + EFFECT_2: FORCE_CAST
        - Spells casted by NPCs on players:
            + EFFECT_0: SCHOOL_DAMAGE
            + EFFECT_1: SCRIPT_EFFECT
            + EFFECT_2: NONE

    In the following script we handle the SCRIPT_EFFECT for effIndex EFFECT_0 and EFFECT_1.
        - When handling EFFECT_0 we're in the "Spells on vehicle bar used by players" case
          and we'll trigger "Spells casted by players triggered by script"
        - When handling EFFECT_1 we're in the "Spells casted by players triggered by script"
          or "Spells casted by NPCs on players" so we'll search for the first defend layer and drop it.
*/

enum BreakShieldSpells
{
    SPELL_BREAK_SHIELD_DAMAGE_2K                 = 62626,
    SPELL_BREAK_SHIELD_DAMAGE_10K                = 64590,

    SPELL_BREAK_SHIELD_TRIGGER_FACTION_MOUNTS    = 62575, // Also on ToC5 mounts
    SPELL_BREAK_SHIELD_TRIGGER_CAMPAING_WARHORSE = 64595,
    SPELL_BREAK_SHIELD_TRIGGER_UNK               = 66480,
};

class spell_gen_break_shield: public SpellScriptLoader
{
    public:
        spell_gen_break_shield(const char* name) : SpellScriptLoader(name) {}

        class spell_gen_break_shield_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_break_shield_SpellScript)

            void HandleScriptEffect(SpellEffIndex effIndex)
            {
                Unit* target = GetHitUnit();

                switch (effIndex)
                {
                    case EFFECT_0: // On spells wich trigger the damaging spell (and also the visual)
                    {
                        uint32 spellId;

                        switch (GetSpellInfo()->Id)
                        {
                            case SPELL_BREAK_SHIELD_TRIGGER_UNK:
                            case SPELL_BREAK_SHIELD_TRIGGER_CAMPAING_WARHORSE:
                                spellId = SPELL_BREAK_SHIELD_DAMAGE_10K;
                                break;
                            case SPELL_BREAK_SHIELD_TRIGGER_FACTION_MOUNTS:
                                spellId = SPELL_BREAK_SHIELD_DAMAGE_2K;
                                break;
                            default:
                                return;
                        }

                        if (Unit* rider = GetCaster()->GetCharmer())
                            rider->CastSpell(target, spellId, false);
                        else
                            GetCaster()->CastSpell(target, spellId, false);
                        break;
                    }
                    case EFFECT_1: // On damaging spells, for removing a defend layer
                        {
                            Unit::AuraApplicationMap const& auras = target->GetAppliedAuras();
                            for (Unit::AuraApplicationMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
                            {
                                if (Aura* aura = itr->second->GetBase())
                                {
                                    SpellInfo const* auraInfo = aura->GetSpellInfo();
                                    if (auraInfo && auraInfo->SpellIconID == 2007 && aura->HasEffectType(SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN))
                                    {
                                        aura->ModStackAmount(-1, AURA_REMOVE_BY_ENEMY_SPELL);
                                        // Remove dummys from rider (Necessary for updating visual shields)
                                        if (Unit* rider = target->GetCharmer())
                                            if (Aura* defend = rider->GetAura(aura->GetId()))
                                                defend->ModStackAmount(-1, AURA_REMOVE_BY_ENEMY_SPELL);
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                    default:
                        break;
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_break_shield_SpellScript::HandleScriptEffect, EFFECT_FIRST_FOUND, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_break_shield_SpellScript();
        }
};

/* DOCUMENTATION: Charge spells
    Charge spells can be classified in four groups:

        - Spells on vehicle bar used by players:
            + EFFECT_0: SCRIPT_EFFECT
            + EFFECT_1: TRIGGER_SPELL
            + EFFECT_2: NONE
        - Spells casted by player's mounts triggered by script:
            + EFFECT_0: CHARGE
            + EFFECT_1: TRIGGER_SPELL
            + EFFECT_2: APPLY_AURA
        - Spells casted by players on the target triggered by script:
            + EFFECT_0: SCHOOL_DAMAGE
            + EFFECT_1: SCRIPT_EFFECT
            + EFFECT_2: NONE
        - Spells casted by NPCs on players:
            + EFFECT_0: SCHOOL_DAMAGE
            + EFFECT_1: CHARGE
            + EFFECT_2: SCRIPT_EFFECT

    In the following script we handle the SCRIPT_EFFECT and CHARGE
        - When handling SCRIPT_EFFECT:
            + EFFECT_0: Corresponds to "Spells on vehicle bar used by players" and we make player's mount cast
              the charge effect on the current target ("Spells casted by player's mounts triggered by script").
            + EFFECT_1 and EFFECT_2: Triggered when "Spells casted by player's mounts triggered by script" hits target,
              corresponding to "Spells casted by players on the target triggered by script" and "Spells casted by
              NPCs on players" and we check Defend layers and drop a charge of the first found.
        - When handling CHARGE:
            + Only launched for "Spells casted by player's mounts triggered by script", makes the player cast the
              damaging spell on target with a small chance of failing it.
*/

enum ChargeSpells
{
    SPELL_CHARGE_DAMAGE_8K5             = 62874,
    SPELL_CHARGE_DAMAGE_20K             = 68498,
    SPELL_CHARGE_DAMAGE_45K             = 64591,

    SPELL_CHARGE_CHARGING_EFFECT_8K5    = 63661,
    SPELL_CHARGE_CHARGING_EFFECT_20K_1  = 68284,
    SPELL_CHARGE_CHARGING_EFFECT_20K_2  = 68501,
    SPELL_CHARGE_CHARGING_EFFECT_45K_1  = 62563,
    SPELL_CHARGE_CHARGING_EFFECT_45K_2  = 66481,

    SPELL_CHARGE_TRIGGER_FACTION_MOUNTS = 62960,
    SPELL_CHARGE_TRIGGER_TRIAL_CHAMPION = 68282,

    SPELL_CHARGE_MISS_EFFECT            = 62977,
};

class spell_gen_mounted_charge: public SpellScriptLoader
{
    public:
        spell_gen_mounted_charge() : SpellScriptLoader("spell_gen_mounted_charge") { }

        class spell_gen_mounted_charge_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_mounted_charge_SpellScript)

            void HandleScriptEffect(SpellEffIndex effIndex)
            {
                Unit* target = GetHitUnit();

                switch (effIndex)
                {
                    case EFFECT_0: // On spells wich trigger the damaging spell (and also the visual)
                    {
                        uint32 spellId;

                        switch (GetSpellInfo()->Id)
                        {
                            case SPELL_CHARGE_TRIGGER_TRIAL_CHAMPION:
                                spellId = SPELL_CHARGE_CHARGING_EFFECT_20K_1;
                                break;
                            case SPELL_CHARGE_TRIGGER_FACTION_MOUNTS:
                                spellId = SPELL_CHARGE_CHARGING_EFFECT_8K5;
                                break;
                            default:
                                return;
                        }

                        // If target isn't a training dummy there's a chance of failing the charge
                        if (!target->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE) && roll_chance_f(12.5f))
                            spellId = SPELL_CHARGE_MISS_EFFECT;

                        if (Unit* vehicle = GetCaster()->GetVehicleBase())
                            vehicle->CastSpell(target, spellId, false);
                        else
                            GetCaster()->CastSpell(target, spellId, false);
                        break;
                    }
                    case EFFECT_1: // On damaging spells, for removing a defend layer
                    case EFFECT_2:
                        {
                            Unit::AuraApplicationMap const& auras = target->GetAppliedAuras();
                            for (Unit::AuraApplicationMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
                            {
                                if (Aura* aura = itr->second->GetBase())
                                {
                                    SpellInfo const* auraInfo = aura->GetSpellInfo();
                                    if (auraInfo && auraInfo->SpellIconID == 2007 && aura->HasEffectType(SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN))
                                    {
                                        aura->ModStackAmount(-1, AURA_REMOVE_BY_ENEMY_SPELL);
                                        // Remove dummys from rider (Necessary for updating visual shields)
                                        if (Unit* rider = target->GetCharmer())
                                            if (Aura* defend = rider->GetAura(aura->GetId()))
                                                defend->ModStackAmount(-1, AURA_REMOVE_BY_ENEMY_SPELL);
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                    default:
                        break;
                }
            }

            void HandleChargeEffect(SpellEffIndex /*effIndex*/)
            {
                uint32 spellId;

                switch (GetSpellInfo()->Id)
                {
                    case SPELL_CHARGE_CHARGING_EFFECT_8K5:
                        spellId = SPELL_CHARGE_DAMAGE_8K5;
                        break;
                    case SPELL_CHARGE_CHARGING_EFFECT_20K_1:
                    case SPELL_CHARGE_CHARGING_EFFECT_20K_2:
                        spellId = SPELL_CHARGE_DAMAGE_20K;
                        break;
                    case SPELL_CHARGE_CHARGING_EFFECT_45K_1:
                    case SPELL_CHARGE_CHARGING_EFFECT_45K_2:
                        spellId = SPELL_CHARGE_DAMAGE_45K;
                        break;
                    default:
                        return;
                }

                if (Unit* rider = GetCaster()->GetCharmer())
                    rider->CastSpell(GetHitUnit(), spellId, false);
                else
                    GetCaster()->CastSpell(GetHitUnit(), spellId, false);
            }

            void Register()
            {
                SpellInfo const* spell = sSpellMgr->GetSpellInfo(m_scriptSpellId);

                if (spell->HasEffect(SPELL_EFFECT_SCRIPT_EFFECT))
                    OnEffectHitTarget += SpellEffectFn(spell_gen_mounted_charge_SpellScript::HandleScriptEffect, EFFECT_FIRST_FOUND, SPELL_EFFECT_SCRIPT_EFFECT);

                if (spell->Effects[EFFECT_0].Effect == SPELL_EFFECT_CHARGE)
                    OnEffectHitTarget += SpellEffectFn(spell_gen_mounted_charge_SpellScript::HandleChargeEffect, EFFECT_0, SPELL_EFFECT_CHARGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_mounted_charge_SpellScript();
        }
};

enum DefendVisuals
{
    SPELL_VISUAL_SHIELD_1 = 63130,
    SPELL_VISUAL_SHIELD_2 = 63131,
    SPELL_VISUAL_SHIELD_3 = 63132,
};

class spell_gen_defend: public SpellScriptLoader
{
    public:
        spell_gen_defend() : SpellScriptLoader("spell_gen_defend") { }

        class spell_gen_defend_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_defend_AuraScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_VISUAL_SHIELD_1))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_VISUAL_SHIELD_2))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_VISUAL_SHIELD_3))
                    return false;
                return true;
            }

            void RefreshVisualShields(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (GetCaster())
                {
                    Unit* target = GetTarget();

                    for (uint8 i = 0; i < GetSpellInfo()->StackAmount; ++i)
                        target->RemoveAurasDueToSpell(SPELL_VISUAL_SHIELD_1 + i);

                    target->CastSpell(target, SPELL_VISUAL_SHIELD_1 + GetAura()->GetStackAmount() - 1, true, NULL, aurEff);
                }
                else
                    GetTarget()->RemoveAurasDueToSpell(GetId());
            }

            void RemoveVisualShields(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                for (uint8 i = 0; i < GetSpellInfo()->StackAmount; ++i)
                    GetTarget()->RemoveAurasDueToSpell(SPELL_VISUAL_SHIELD_1 + i);
            }

            void RemoveDummyFromDriver(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    if (TempSummon* vehicle = caster->ToTempSummon())
                        if (Unit* rider = vehicle->GetSummoner())
                            rider->RemoveAurasDueToSpell(GetId());
            }

            void Register()
            {
                SpellInfo const* spell = sSpellMgr->GetSpellInfo(m_scriptSpellId);

                // Defend spells casted by NPCs (add visuals)
                if (spell->Effects[EFFECT_0].ApplyAuraName == SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN)
                {
                    AfterEffectApply += AuraEffectApplyFn(spell_gen_defend_AuraScript::RefreshVisualShields, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                    OnEffectRemove += AuraEffectRemoveFn(spell_gen_defend_AuraScript::RemoveVisualShields, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
                }

                // Remove Defend spell from player when he dismounts
                if (spell->Effects[EFFECT_2].ApplyAuraName == SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN)
                    OnEffectRemove += AuraEffectRemoveFn(spell_gen_defend_AuraScript::RemoveDummyFromDriver, EFFECT_2, SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN, AURA_EFFECT_HANDLE_REAL);

                // Defend spells casted by players (add/remove visuals)
                if (spell->Effects[EFFECT_1].ApplyAuraName == SPELL_AURA_DUMMY)
                {
                    AfterEffectApply += AuraEffectApplyFn(spell_gen_defend_AuraScript::RefreshVisualShields, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                    OnEffectRemove += AuraEffectRemoveFn(spell_gen_defend_AuraScript::RemoveVisualShields, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_CHANGE_AMOUNT_MASK);
                }
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_defend_AuraScript();
        }
};

enum MountedDuelSpells
{
    SPELL_ON_TOURNAMENT_MOUNT = 63034,
    SPELL_MOUNTED_DUEL        = 62875,
};

class spell_gen_tournament_duel: public SpellScriptLoader
{
    public:
        spell_gen_tournament_duel() : SpellScriptLoader("spell_gen_tournament_duel") { }

        class spell_gen_tournament_duel_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_tournament_duel_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_ON_TOURNAMENT_MOUNT))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_MOUNTED_DUEL))
                    return false;
                return true;
            }

            void HandleScriptEffect(SpellEffIndex /*effIndex*/)
            {
                if (Unit* rider = GetCaster()->GetCharmer())
                {
                    if (Player* plrTarget = GetHitPlayer())
                    {
                        if (plrTarget->HasAura(SPELL_ON_TOURNAMENT_MOUNT) && plrTarget->GetVehicleBase())
                            rider->CastSpell(plrTarget, SPELL_MOUNTED_DUEL, true);
                    }
                    else if (Unit* unitTarget = GetHitUnit())
                    {
                        if (unitTarget->GetCharmer() && unitTarget->GetCharmer()->IsPlayer() && unitTarget->GetCharmer()->HasAura(SPELL_ON_TOURNAMENT_MOUNT))
                            rider->CastSpell(unitTarget->GetCharmer(), SPELL_MOUNTED_DUEL, true);
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_tournament_duel_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_tournament_duel_SpellScript();
        }
};

enum TournamentMountsSpells
{
    SPELL_LANCE_EQUIPPED = 62853,
};

class spell_gen_summon_tournament_mount: public SpellScriptLoader
{
    public:
        spell_gen_summon_tournament_mount() : SpellScriptLoader("spell_gen_summon_tournament_mount") { }

        class spell_gen_summon_tournament_mount_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_summon_tournament_mount_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_LANCE_EQUIPPED))
                    return false;
                return true;
            }

            SpellCastResult CheckIfLanceEquiped()
            {
                if (GetCaster()->IsInDisallowedMountForm())
                    GetCaster()->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);

                if (!GetCaster()->HasAura(SPELL_LANCE_EQUIPPED))
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_HAVE_LANCE_EQUIPPED);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_gen_summon_tournament_mount_SpellScript::CheckIfLanceEquiped);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_summon_tournament_mount_SpellScript();
        }
};

enum TournamentPennantSpells
{
    SPELL_PENNANT_STORMWIND_ASPIRANT      = 62595,
    SPELL_PENNANT_STORMWIND_VALIANT       = 62596,
    SPELL_PENNANT_STORMWIND_CHAMPION      = 62594,
    SPELL_PENNANT_GNOMEREGAN_ASPIRANT     = 63394,
    SPELL_PENNANT_GNOMEREGAN_VALIANT      = 63395,
    SPELL_PENNANT_GNOMEREGAN_CHAMPION     = 63396,
    SPELL_PENNANT_SEN_JIN_ASPIRANT        = 63397,
    SPELL_PENNANT_SEN_JIN_VALIANT         = 63398,
    SPELL_PENNANT_SEN_JIN_CHAMPION        = 63399,
    SPELL_PENNANT_SILVERMOON_ASPIRANT     = 63401,
    SPELL_PENNANT_SILVERMOON_VALIANT      = 63402,
    SPELL_PENNANT_SILVERMOON_CHAMPION     = 63403,
    SPELL_PENNANT_DARNASSUS_ASPIRANT      = 63404,
    SPELL_PENNANT_DARNASSUS_VALIANT       = 63405,
    SPELL_PENNANT_DARNASSUS_CHAMPION      = 63406,
    SPELL_PENNANT_EXODAR_ASPIRANT         = 63421,
    SPELL_PENNANT_EXODAR_VALIANT          = 63422,
    SPELL_PENNANT_EXODAR_CHAMPION         = 63423,
    SPELL_PENNANT_IRONFORGE_ASPIRANT      = 63425,
    SPELL_PENNANT_IRONFORGE_VALIANT       = 63426,
    SPELL_PENNANT_IRONFORGE_CHAMPION      = 63427,
    SPELL_PENNANT_UNDERCITY_ASPIRANT      = 63428,
    SPELL_PENNANT_UNDERCITY_VALIANT       = 63429,
    SPELL_PENNANT_UNDERCITY_CHAMPION      = 63430,
    SPELL_PENNANT_ORGRIMMAR_ASPIRANT      = 63431,
    SPELL_PENNANT_ORGRIMMAR_VALIANT       = 63432,
    SPELL_PENNANT_ORGRIMMAR_CHAMPION      = 63433,
    SPELL_PENNANT_THUNDER_BLUFF_ASPIRANT  = 63434,
    SPELL_PENNANT_THUNDER_BLUFF_VALIANT   = 63435,
    SPELL_PENNANT_THUNDER_BLUFF_CHAMPION  = 63436,
    SPELL_PENNANT_ARGENT_CRUSADE_ASPIRANT = 63606,
    SPELL_PENNANT_ARGENT_CRUSADE_VALIANT  = 63500,
    SPELL_PENNANT_ARGENT_CRUSADE_CHAMPION = 63501,
    SPELL_PENNANT_EBON_BLADE_ASPIRANT     = 63607,
    SPELL_PENNANT_EBON_BLADE_VALIANT      = 63608,
    SPELL_PENNANT_EBON_BLADE_CHAMPION     = 63609,
};

enum TournamentMounts
{
    NPC_STORMWIND_STEED             = 33217,
    NPC_IRONFORGE_RAM               = 33316,
    NPC_GNOMEREGAN_MECHANOSTRIDER   = 33317,
    NPC_EXODAR_ELEKK                = 33318,
    NPC_DARNASSIAN_NIGHTSABER       = 33319,
    NPC_ORGRIMMAR_WOLF              = 33320,
    NPC_DARK_SPEAR_RAPTOR           = 33321,
    NPC_THUNDER_BLUFF_KODO          = 33322,
    NPC_SILVERMOON_HAWKSTRIDER      = 33323,
    NPC_FORSAKEN_WARHORSE           = 33324,
    NPC_ARGENT_WARHORSE             = 33782,
    NPC_ARGENT_STEED_ASPIRANT       = 33845,
    NPC_ARGENT_HAWKSTRIDER_ASPIRANT = 33844,
};

enum TournamentQuestsAchievements
{
    ACHIEVEMENT_CHAMPION_STORMWIND     = 2781,
    ACHIEVEMENT_CHAMPION_DARNASSUS     = 2777,
    ACHIEVEMENT_CHAMPION_IRONFORGE     = 2780,
    ACHIEVEMENT_CHAMPION_GNOMEREGAN    = 2779,
    ACHIEVEMENT_CHAMPION_THE_EXODAR    = 2778,
    ACHIEVEMENT_CHAMPION_ORGRIMMAR     = 2783,
    ACHIEVEMENT_CHAMPION_SEN_JIN       = 2784,
    ACHIEVEMENT_CHAMPION_THUNDER_BLUFF = 2786,
    ACHIEVEMENT_CHAMPION_UNDERCITY     = 2787,
    ACHIEVEMENT_CHAMPION_SILVERMOON    = 2785,
    ACHIEVEMENT_ARGENT_VALOR           = 2758,
    ACHIEVEMENT_CHAMPION_ALLIANCE      = 2782,
    ACHIEVEMENT_CHAMPION_HORDE         = 2788,

    QUEST_VALIANT_OF_STORMWIND         = 13593,
    QUEST_A_VALIANT_OF_STORMWIND       = 13684,
    QUEST_VALIANT_OF_DARNASSUS         = 13706,
    QUEST_A_VALIANT_OF_DARNASSUS       = 13689,
    QUEST_VALIANT_OF_IRONFORGE         = 13703,
    QUEST_A_VALIANT_OF_IRONFORGE       = 13685,
    QUEST_VALIANT_OF_GNOMEREGAN        = 13704,
    QUEST_A_VALIANT_OF_GNOMEREGAN      = 13688,
    QUEST_VALIANT_OF_THE_EXODAR        = 13705,
    QUEST_A_VALIANT_OF_THE_EXODAR      = 13690,
    QUEST_VALIANT_OF_ORGRIMMAR         = 13707,
    QUEST_A_VALIANT_OF_ORGRIMMAR       = 13691,
    QUEST_VALIANT_OF_SEN_JIN           = 13708,
    QUEST_A_VALIANT_OF_SEN_JIN         = 13693,
    QUEST_VALIANT_OF_THUNDER_BLUFF     = 13709,
    QUEST_A_VALIANT_OF_THUNDER_BLUFF   = 13694,
    QUEST_VALIANT_OF_UNDERCITY         = 13710,
    QUEST_A_VALIANT_OF_UNDERCITY       = 13695,
    QUEST_VALIANT_OF_SILVERMOON        = 13711,
    QUEST_A_VALIANT_OF_SILVERMOON      = 13696,
};

class spell_gen_on_tournament_mount: public SpellScriptLoader
{
    public:
        spell_gen_on_tournament_mount() : SpellScriptLoader("spell_gen_on_tournament_mount") { }

        class spell_gen_on_tournament_mount_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_on_tournament_mount_AuraScript);

            uint32 _pennantSpellId;

            bool Load()
            {
                _pennantSpellId = 0;
                return GetCaster() && GetCaster()->IsPlayer();
            }

            void HandleApplyEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* vehicle = caster->GetVehicleBase())
                    {
                        _pennantSpellId = GetPennatSpellId(caster->ToPlayer(), vehicle);
                        caster->CastSpell(caster, _pennantSpellId, true);
                    }
                }
            }

            void HandleRemoveEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    caster->RemoveAurasDueToSpell(_pennantSpellId);
            }

            uint32 GetPennatSpellId(Player* player, Unit* mount)
            {
                switch (mount->GetEntry())
                {
                    case NPC_ARGENT_STEED_ASPIRANT:
                    case NPC_STORMWIND_STEED:
                    {
                        if (player->GetAchievementMgr().HasAchieved(ACHIEVEMENT_CHAMPION_STORMWIND))
                            return SPELL_PENNANT_STORMWIND_CHAMPION;
                        else if (player->GetQuestRewardStatus(QUEST_VALIANT_OF_STORMWIND) || player->GetQuestRewardStatus(QUEST_A_VALIANT_OF_STORMWIND))
                            return SPELL_PENNANT_STORMWIND_VALIANT;
                        else
                            return SPELL_PENNANT_STORMWIND_ASPIRANT;
                    }
                    case NPC_GNOMEREGAN_MECHANOSTRIDER:
                    {
                        if (player->GetAchievementMgr().HasAchieved(ACHIEVEMENT_CHAMPION_GNOMEREGAN))
                            return SPELL_PENNANT_GNOMEREGAN_CHAMPION;
                        else if (player->GetQuestRewardStatus(QUEST_VALIANT_OF_GNOMEREGAN) || player->GetQuestRewardStatus(QUEST_A_VALIANT_OF_GNOMEREGAN))
                            return SPELL_PENNANT_GNOMEREGAN_VALIANT;
                        else
                            return SPELL_PENNANT_GNOMEREGAN_ASPIRANT;
                    }
                    case NPC_DARK_SPEAR_RAPTOR:
                    {
                        if (player->GetAchievementMgr().HasAchieved(ACHIEVEMENT_CHAMPION_SEN_JIN))
                            return SPELL_PENNANT_SEN_JIN_CHAMPION;
                        else if (player->GetQuestRewardStatus(QUEST_VALIANT_OF_SEN_JIN) || player->GetQuestRewardStatus(QUEST_A_VALIANT_OF_SEN_JIN))
                            return SPELL_PENNANT_SEN_JIN_VALIANT;
                        else
                            return SPELL_PENNANT_SEN_JIN_ASPIRANT;
                    }
                    case NPC_ARGENT_HAWKSTRIDER_ASPIRANT:
                    case NPC_SILVERMOON_HAWKSTRIDER:
                    {
                        if (player->GetAchievementMgr().HasAchieved(ACHIEVEMENT_CHAMPION_SILVERMOON))
                            return SPELL_PENNANT_SILVERMOON_CHAMPION;
                        else if (player->GetQuestRewardStatus(QUEST_VALIANT_OF_SILVERMOON) || player->GetQuestRewardStatus(QUEST_A_VALIANT_OF_SILVERMOON))
                            return SPELL_PENNANT_SILVERMOON_VALIANT;
                        else
                            return SPELL_PENNANT_SILVERMOON_ASPIRANT;
                    }
                    case NPC_DARNASSIAN_NIGHTSABER:
                    {
                        if (player->GetAchievementMgr().HasAchieved(ACHIEVEMENT_CHAMPION_DARNASSUS))
                            return SPELL_PENNANT_DARNASSUS_CHAMPION;
                        else if (player->GetQuestRewardStatus(QUEST_VALIANT_OF_DARNASSUS) || player->GetQuestRewardStatus(QUEST_A_VALIANT_OF_DARNASSUS))
                            return SPELL_PENNANT_DARNASSUS_VALIANT;
                        else
                            return SPELL_PENNANT_DARNASSUS_ASPIRANT;
                    }
                    case NPC_EXODAR_ELEKK:
                    {
                        if (player->GetAchievementMgr().HasAchieved(ACHIEVEMENT_CHAMPION_THE_EXODAR))
                            return SPELL_PENNANT_EXODAR_CHAMPION;
                        else if (player->GetQuestRewardStatus(QUEST_VALIANT_OF_THE_EXODAR) || player->GetQuestRewardStatus(QUEST_A_VALIANT_OF_THE_EXODAR))
                            return SPELL_PENNANT_EXODAR_VALIANT;
                        else
                            return SPELL_PENNANT_EXODAR_ASPIRANT;
                    }
                    case NPC_IRONFORGE_RAM:
                    {
                        if (player->GetAchievementMgr().HasAchieved(ACHIEVEMENT_CHAMPION_IRONFORGE))
                            return SPELL_PENNANT_IRONFORGE_CHAMPION;
                        else if (player->GetQuestRewardStatus(QUEST_VALIANT_OF_IRONFORGE) || player->GetQuestRewardStatus(QUEST_A_VALIANT_OF_IRONFORGE))
                            return SPELL_PENNANT_IRONFORGE_VALIANT;
                        else
                            return SPELL_PENNANT_IRONFORGE_ASPIRANT;
                    }
                    case NPC_FORSAKEN_WARHORSE:
                    {
                        if (player->GetAchievementMgr().HasAchieved(ACHIEVEMENT_CHAMPION_UNDERCITY))
                            return SPELL_PENNANT_UNDERCITY_CHAMPION;
                        else if (player->GetQuestRewardStatus(QUEST_VALIANT_OF_UNDERCITY) || player->GetQuestRewardStatus(QUEST_A_VALIANT_OF_UNDERCITY))
                            return SPELL_PENNANT_UNDERCITY_VALIANT;
                        else
                            return SPELL_PENNANT_UNDERCITY_ASPIRANT;
                    }
                    case NPC_ORGRIMMAR_WOLF:
                    {
                        if (player->GetAchievementMgr().HasAchieved(ACHIEVEMENT_CHAMPION_ORGRIMMAR))
                            return SPELL_PENNANT_ORGRIMMAR_CHAMPION;
                        else if (player->GetQuestRewardStatus(QUEST_VALIANT_OF_ORGRIMMAR) || player->GetQuestRewardStatus(QUEST_A_VALIANT_OF_ORGRIMMAR))
                            return SPELL_PENNANT_ORGRIMMAR_VALIANT;
                        else
                            return SPELL_PENNANT_ORGRIMMAR_ASPIRANT;
                    }
                    case NPC_THUNDER_BLUFF_KODO:
                    {
                        if (player->GetAchievementMgr().HasAchieved(ACHIEVEMENT_CHAMPION_THUNDER_BLUFF))
                            return SPELL_PENNANT_THUNDER_BLUFF_CHAMPION;
                        else if (player->GetQuestRewardStatus(QUEST_VALIANT_OF_THUNDER_BLUFF) || player->GetQuestRewardStatus(QUEST_A_VALIANT_OF_THUNDER_BLUFF))
                            return SPELL_PENNANT_THUNDER_BLUFF_VALIANT;
                        else
                            return SPELL_PENNANT_THUNDER_BLUFF_ASPIRANT;
                    }
                    case NPC_ARGENT_WARHORSE:
                    {
                        if (player->GetAchievementMgr().HasAchieved(ACHIEVEMENT_CHAMPION_ALLIANCE) || player->GetAchievementMgr().HasAchieved(ACHIEVEMENT_CHAMPION_HORDE))
                            return player->getClass() == CLASS_DEATH_KNIGHT ? SPELL_PENNANT_EBON_BLADE_CHAMPION : SPELL_PENNANT_ARGENT_CRUSADE_CHAMPION;
                        else if (player->GetAchievementMgr().HasAchieved(ACHIEVEMENT_ARGENT_VALOR))
                            return player->getClass() == CLASS_DEATH_KNIGHT ? SPELL_PENNANT_EBON_BLADE_VALIANT : SPELL_PENNANT_ARGENT_CRUSADE_VALIANT;
                        else
                            return player->getClass() == CLASS_DEATH_KNIGHT ? SPELL_PENNANT_EBON_BLADE_ASPIRANT : SPELL_PENNANT_ARGENT_CRUSADE_ASPIRANT;
                    }
                    default:
                        return 0;
                }
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_gen_on_tournament_mount_AuraScript::HandleApplyEffect, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                OnEffectRemove += AuraEffectRemoveFn(spell_gen_on_tournament_mount_AuraScript::HandleRemoveEffect, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_on_tournament_mount_AuraScript();
        }
};

class spell_gen_tournament_pennant: public SpellScriptLoader
{
    public:
        spell_gen_tournament_pennant() : SpellScriptLoader("spell_gen_tournament_pennant") { }

        class spell_gen_tournament_pennant_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_tournament_pennant_AuraScript);

            bool Load()
            {
                return GetCaster() && GetCaster()->IsPlayer();
            }

            void HandleApplyEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    if (!caster->GetVehicleBase())
                        caster->RemoveAurasDueToSpell(GetId());
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_gen_tournament_pennant_AuraScript::HandleApplyEffect, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_tournament_pennant_AuraScript();
        }
};

enum ChaosBlast
{
    SPELL_CHAOS_BLAST   = 37675,
};

class spell_gen_chaos_blast: public SpellScriptLoader
{
    public:
        spell_gen_chaos_blast() : SpellScriptLoader("spell_gen_chaos_blast") { }

        class spell_gen_chaos_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_chaos_blast_SpellScript)

            bool Validate(SpellInfo const* /*SpellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_CHAOS_BLAST))
                    return false;
                return true;
            }
            void HandleDummy(SpellEffIndex /* effIndex */)
            {
                int32 basepoints0 = 100;
                Unit* caster = GetCaster();
                if (Unit* target = GetHitUnit())
                    caster->CastCustomSpell(target, SPELL_CHAOS_BLAST, &basepoints0, NULL, NULL, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_chaos_blast_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_chaos_blast_SpellScript();
        }

};

class spell_gen_wg_water: public SpellScriptLoader
{
    public:
        spell_gen_wg_water() : SpellScriptLoader("spell_gen_wg_water") {}

        class spell_gen_wg_water_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_wg_water_SpellScript);

            SpellCastResult CheckCast()
            {
                if (!GetSpellInfo()->CheckTargetCreatureType(GetCaster()))
                    return SPELL_FAILED_DONT_REPORT;
                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_gen_wg_water_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_wg_water_SpellScript();
        }
};

class spell_gen_count_pct_from_max_hp: public SpellScriptLoader
{
    public:
        spell_gen_count_pct_from_max_hp(char const* name, int32 damagePct = 0) : SpellScriptLoader(name), _damagePct(damagePct) { }

        class spell_gen_count_pct_from_max_hp_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_count_pct_from_max_hp_SpellScript)

        public:
            spell_gen_count_pct_from_max_hp_SpellScript(int32 damagePct) : SpellScript(), _damagePct(damagePct) { }

            void RecalculateDamage()
            {
                if (!_damagePct)
                    _damagePct = GetHitDamage();

                SetHitDamage(GetHitUnit()->CountPctFromMaxHealth(_damagePct));
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_gen_count_pct_from_max_hp_SpellScript::RecalculateDamage);
            }

        private:
            int32 _damagePct;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_count_pct_from_max_hp_SpellScript(_damagePct);
        }

    private:
        int32 _damagePct;
};

class spell_gen_despawn_self: public SpellScriptLoader
{
public:
    spell_gen_despawn_self() : SpellScriptLoader("spell_gen_despawn_self") { }

    class spell_gen_despawn_self_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_despawn_self_SpellScript);

        bool Load()
        {
            return GetCaster()->GetTypeId() == TYPEID_UNIT;
        }

        void HandleDummy(SpellEffIndex effIndex)
        {
            if (GetSpellInfo()->Effects[effIndex].Effect == SPELL_EFFECT_DUMMY || GetSpellInfo()->Effects[effIndex].Effect == SPELL_EFFECT_SCRIPT_EFFECT)
                GetCaster()->ToCreature()->DespawnOrUnsummon();
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_despawn_self_SpellScript::HandleDummy, EFFECT_ALL, SPELL_EFFECT_ANY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_gen_despawn_self_SpellScript();
    }
};

class spell_gen_touch_the_nightmare: public SpellScriptLoader
{
public:
    spell_gen_touch_the_nightmare() : SpellScriptLoader("spell_gen_touch_the_nightmare") { }

    class spell_gen_touch_the_nightmare_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_touch_the_nightmare_SpellScript);

        void HandleDamageCalc(SpellEffIndex /*effIndex*/)
        {
            uint32 bp = GetCaster()->GetMaxHealth() * 0.3f;
            SetHitDamage(bp);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_touch_the_nightmare_SpellScript::HandleDamageCalc, EFFECT_2, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_gen_touch_the_nightmare_SpellScript();
    }
};

class spell_gen_dream_funnel: public SpellScriptLoader
{
public:
    spell_gen_dream_funnel() : SpellScriptLoader("spell_gen_dream_funnel") { }

    class spell_gen_dream_funnel_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gen_dream_funnel_AuraScript);

        void HandleEffectCalcAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
        {
            if (GetCaster())
                amount = GetCaster()->GetMaxHealth() * 0.05f;

            canBeRecalculated = false;
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_dream_funnel_AuraScript::HandleEffectCalcAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_dream_funnel_AuraScript::HandleEffectCalcAmount, EFFECT_2, SPELL_AURA_PERIODIC_DAMAGE);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_gen_dream_funnel_AuraScript();
    }
};

enum GenericBandage
{
    SPELL_RECENTLY_BANDAGED = 11196,
};

class spell_gen_bandage: public SpellScriptLoader
{
    public:
        spell_gen_bandage() : SpellScriptLoader("spell_gen_bandage") { }

        class spell_gen_bandage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_bandage_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_RECENTLY_BANDAGED))
                    return false;
                return true;
            }

            SpellCastResult CheckCast()
            {
                if (Unit* target = GetExplTargetUnit())
                {
                    if (target->HasAura(SPELL_RECENTLY_BANDAGED))
                        return SPELL_FAILED_TARGET_AURASTATE;
                }
                return SPELL_CAST_OK;
            }

            void HandleScript()
            {
                if (Unit* target = GetHitUnit())
                    GetCaster()->CastSpell(target, SPELL_RECENTLY_BANDAGED, true);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_gen_bandage_SpellScript::CheckCast);
                AfterHit += SpellHitFn(spell_gen_bandage_SpellScript::HandleScript);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_bandage_SpellScript();
        }
};

enum GenericLifebloom
{
    SPELL_HEXLORD_MALACRASS_LIFEBLOOM_FINAL_HEAL        = 43422,
    SPELL_TUR_RAGEPAW_LIFEBLOOM_FINAL_HEAL              = 52552,
    SPELL_CENARION_SCOUT_LIFEBLOOM_FINAL_HEAL           = 53692,
    SPELL_TWISTED_VISAGE_LIFEBLOOM_FINAL_HEAL           = 57763,
    SPELL_FACTION_CHAMPIONS_DRU_LIFEBLOOM_FINAL_HEAL    = 66094,
};

class spell_gen_lifebloom: public SpellScriptLoader
{
    public:
        spell_gen_lifebloom(const char* name, uint32 spellId) : SpellScriptLoader(name), _spellId(spellId) { }

        class spell_gen_lifebloom_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_lifebloom_AuraScript);

        public:
            spell_gen_lifebloom_AuraScript(uint32 spellId) : AuraScript(), _spellId(spellId) { }

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(_spellId))
                    return false;
                return true;
            }

            void AfterRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                // Final heal only on duration end
                if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE && GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_ENEMY_SPELL)
                    return;

                // final heal
                GetTarget()->CastSpell(GetTarget(), _spellId, true, NULL, aurEff, GetCasterGUID());
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_gen_lifebloom_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
            }

        private:
            uint32 _spellId;
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_lifebloom_AuraScript(_spellId);
        }

    private:
        uint32 _spellId;
};

/// The following code for mounts might surely be deprecated
enum Mounts
{
    SPELL_COLD_WEATHER_FLYING           = 54197,

    // Magic Broom
    SPELL_MAGIC_BROOM_150               = 42667,
    SPELL_MAGIC_BROOM_280               = 42668,

    // Winged Steed of the Ebon Blade
    SPELL_WINGED_STEED_150              = 54726,
    SPELL_WINGED_STEED_280              = 54727,

    // Blazing Hippogryph
    SPELL_BLAZING_HIPPOGRYPH_150        = 74854,
    SPELL_BLAZING_HIPPOGRYPH_280        = 74855
};

class spell_gen_mount: public SpellScriptLoader
{
    public:
        spell_gen_mount(const char* name, uint32 mount0 = 0, uint32 mount60 = 0, uint32 mount100 = 0, uint32 mount150 = 0, uint32 mount280 = 0, uint32 mount310 = 0) : SpellScriptLoader(name),
            _mount0(mount0), _mount60(mount60), _mount100(mount100), _mount150(mount150), _mount280(mount280), _mount310(mount310) { }

        class spell_gen_mount_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_mount_SpellScript);

        public:
            spell_gen_mount_SpellScript(uint32 mount0, uint32 mount60, uint32 mount100, uint32 mount150, uint32 mount280, uint32 mount310) : SpellScript(),
                _mount0(mount0), _mount60(mount60), _mount100(mount100), _mount150(mount150), _mount280(mount280), _mount310(mount310) { }

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (_mount0 && !sSpellMgr->GetSpellInfo(_mount0))
                    return false;
                if (_mount60 && !sSpellMgr->GetSpellInfo(_mount60))
                    return false;
                if (_mount100 && !sSpellMgr->GetSpellInfo(_mount100))
                    return false;
                if (_mount150 && !sSpellMgr->GetSpellInfo(_mount150))
                    return false;
                if (_mount280 && !sSpellMgr->GetSpellInfo(_mount280))
                    return false;
                if (_mount310 && !sSpellMgr->GetSpellInfo(_mount310))
                    return false;
                return true;
            }

            void HandleMount(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);

                if (Player* target = GetHitPlayer())
                {
                    // Prevent stacking of mounts and client crashes upon dismounting
                    target->RemoveAurasByType(SPELL_AURA_MOUNTED, 0, GetHitAura());

                    // Triggered spell id dependent on riding skill and zone
                    bool canFly = false;
                    uint32 map = GetVirtualMapForMapAndZone(target->GetMapId(), target->GetZoneId());
                    if (map == 530 || (map == 571 && target->HasSpell(SPELL_COLD_WEATHER_FLYING)))
                        canFly = true;

                    float x, y, z;
                    target->GetPosition(x, y, z);
                    uint32 areaFlag = target->GetBaseMap()->GetAreaFlag(x, y, z);
                    AreaTableEntry const* area = sAreaStore.LookupEntry(areaFlag);
                    if (!area || (canFly && (area->Flags & AREA_FLAG_NO_FLY_ZONE)))
                        canFly = false;

                    uint32 mount = 0;
                    switch (target->GetBaseSkillValue(SKILL_RIDING))
                    {
                        case 0:
                            mount = _mount0;
                            break;
                        case 75:
                            mount = _mount60;
                            break;
                        case 150:
                            mount = _mount100;
                            break;
                        case 225:
                            if (canFly)
                                mount = _mount150;
                            else
                                mount = _mount100;
                            break;
                        case 300:
                            if (canFly)
                                mount = _mount280;
                            else
                                mount = _mount100;
                            break;
                        case 375:
                            if (canFly)
                                mount = _mount310;
                            else
                                mount = _mount100;
                            break;
                        default:
                            break;
                    }

                    if (mount)
                    {
                        PreventHitAura();
                        target->CastSpell(target, mount, true);
                    }
                }
            }

            void Register()
            {
                 OnEffectHitTarget += SpellEffectFn(spell_gen_mount_SpellScript::HandleMount, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }

        private:
            uint32 _mount0;
            uint32 _mount60;
            uint32 _mount100;
            uint32 _mount150;
            uint32 _mount280;
            uint32 _mount310;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_mount_SpellScript(_mount0, _mount60, _mount100, _mount150, _mount280, _mount310);
        }

    private:
        uint32 _mount0;
        uint32 _mount60;
        uint32 _mount100;
        uint32 _mount150;
        uint32 _mount280;
        uint32 _mount310;
};

enum FoamSword
{
    ITEM_FOAM_SWORD_GREEN = 45061,
    ITEM_FOAM_SWORD_PINK = 45176,
    ITEM_FOAM_SWORD_BLUE = 45177,
    ITEM_FOAM_SWORD_RED = 45178,
    ITEM_FOAM_SWORD_YELLOW = 45179,

    SPELL_BONKED = 62991,
    SPELL_FOAM_SWORD_DEFEAT = 62994,
    SPELL_ON_GUARD = 62972,
};

class spell_gen_upper_deck_create_foam_sword: public SpellScriptLoader
{
public:
    spell_gen_upper_deck_create_foam_sword() : SpellScriptLoader("spell_gen_upper_deck_create_foam_sword") { }

    class spell_gen_upper_deck_create_foam_sword_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_upper_deck_create_foam_sword_SpellScript);

        void HandleScript(SpellEffIndex effIndex)
        {
            if (Player* player = GetHitPlayer())
            {
                static uint32 const itemId[5] = { ITEM_FOAM_SWORD_GREEN, ITEM_FOAM_SWORD_PINK, ITEM_FOAM_SWORD_BLUE, ITEM_FOAM_SWORD_RED, ITEM_FOAM_SWORD_YELLOW };
                // player can only have one of these items
                for (uint8 i = 0; i < 5; ++i)
                {
                    if (player->HasItemCount(itemId[i], 1, true))
                        return;
                }

                CreateItem(effIndex, itemId[urand(0, 4)]);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_upper_deck_create_foam_sword_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_gen_upper_deck_create_foam_sword_SpellScript();
    }
};

class spell_gen_bonked: public SpellScriptLoader
{
public:
    spell_gen_bonked() : SpellScriptLoader("spell_gen_bonked") { }

    class spell_gen_bonked_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_bonked_SpellScript);

        void HandleScript(SpellEffIndex /*effIndex*/)
        {
            if (Player* target = GetHitPlayer())
            {
                Aura const* aura = GetHitAura();
                if (!(aura && aura->GetStackAmount() == 3))
                    return;

                target->CastSpell(target, SPELL_FOAM_SWORD_DEFEAT, true);
                target->RemoveAurasDueToSpell(SPELL_BONKED);

                if (Aura const* aura = target->GetAura(SPELL_ON_GUARD))
                {
                    if (Item* item = target->GetItemByGuid(aura->GetCastItemGUID()))
                        target->DestroyItemCount(item->GetEntry(), 1, true);
                }
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_bonked_SpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_gen_bonked_SpellScript();
    }
};

// Gift of the Naaru - 59548 or 59547 or 59545 or 59544 or 59543 or 59542 or 121093 or 28880
class spell_gen_gift_of_the_naaru: public SpellScriptLoader
{
    public:
        spell_gen_gift_of_the_naaru() : SpellScriptLoader("spell_gen_gift_of_the_naaru") { }

        class spell_gen_gift_of_the_naaru_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_gift_of_the_naaru_AuraScript);

            void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
            {
                if (!GetCaster())
                    return;

                amount = GetCaster()->CountPctFromMaxHealth(4);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_gift_of_the_naaru_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_gift_of_the_naaru_AuraScript();
        }
};

enum RunningWildMountIds
{
    RUNNING_WILD_MODEL_MALE     = 29422,
    RUNNING_WILD_MODEL_FEMALE   = 29423,
    SPELL_ALTERED_FORM          = 97709,
};

class spell_gen_running_wild: public SpellScriptLoader
{
    public:
        spell_gen_running_wild() : SpellScriptLoader("spell_gen_running_wild") { }

        class spell_gen_running_wild_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_running_wild_AuraScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sCreatureDisplayInfoStore.LookupEntry(RUNNING_WILD_MODEL_MALE))
                    return false;
                if (!sCreatureDisplayInfoStore.LookupEntry(RUNNING_WILD_MODEL_FEMALE))
                    return false;
                return true;
            }

            void HandleMount(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* target = GetTarget();
                PreventDefaultAction();

                target->Mount(target->getGender() == GENDER_FEMALE ? RUNNING_WILD_MODEL_FEMALE : RUNNING_WILD_MODEL_MALE, 0, 0);

                // cast speed aura
                if (MountCapabilityEntry const* mountCapability = sMountCapabilityStore.LookupEntry(aurEff->GetAmount()))
                    target->CastSpell(target, mountCapability->SpeedModSpell, TRIGGERED_FULL_MASK);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_gen_running_wild_AuraScript::HandleMount, EFFECT_1, SPELL_AURA_MOUNTED, AURA_EFFECT_HANDLE_REAL);
            }
        };

        class spell_gen_running_wild_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_running_wild_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_ALTERED_FORM))
                    return false;
                return true;
            }

            bool Load()
            {
                // Definitely not a good thing, but currently the only way to do something at cast start
                // Should be replaced as soon as possible with a new hook: BeforeCastStart
                GetCaster()->CastSpell(GetCaster(), SPELL_ALTERED_FORM, TRIGGERED_FULL_MASK);
                return false;
            }

            void Register()
            {
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_running_wild_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_running_wild_SpellScript();
        }
};

class spell_gen_two_forms: public SpellScriptLoader
{
    public:
        spell_gen_two_forms() : SpellScriptLoader("spell_gen_two_forms") { }

        class spell_gen_two_forms_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_two_forms_SpellScript);

            SpellCastResult CheckCast()
            {
                if (GetCaster()->isInCombat())
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_CANT_TRANSFORM);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }

                // Player cannot transform to human form if he is forced to be worgen for some reason (Darkflight)
                if (GetCaster()->GetAuraEffectsByType(SPELL_AURA_WORGEN_ALTERED_FORM).size() > 1)
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_CANT_TRANSFORM);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }

                return SPELL_CAST_OK;
            }

            void HandleTransform(SpellEffIndex effIndex)
            {
                Unit* target = GetHitUnit();
                PreventHitDefaultEffect(effIndex);
                if (target->HasAuraType(SPELL_AURA_WORGEN_ALTERED_FORM))
                    target->RemoveAurasByType(SPELL_AURA_WORGEN_ALTERED_FORM);
                else    // Basepoints 1 for this aura control whether to trigger transform transition animation or not.
                    target->CastCustomSpell(SPELL_ALTERED_FORM, SPELLVALUE_BASE_POINT0, 1, target, true);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_gen_two_forms_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_gen_two_forms_SpellScript::HandleTransform, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_two_forms_SpellScript();
        }
};

class spell_gen_darkflight: public SpellScriptLoader
{
    public:
        spell_gen_darkflight() : SpellScriptLoader("spell_gen_darkflight") { }

        class spell_gen_darkflight_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_darkflight_SpellScript);

            void TriggerTransform()
            {
                GetCaster()->CastSpell(GetCaster(), SPELL_ALTERED_FORM, TRIGGERED_FULL_MASK);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_gen_darkflight_SpellScript::TriggerTransform);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_darkflight_SpellScript();
        }
};

// Blood Fury - 20572 or Blood Fury - 33702
class spell_gen_blood_fury: public SpellScriptLoader
{
    public:
        spell_gen_blood_fury() : SpellScriptLoader("spell_gen_blood_fury") { }

        class spell_gen_blood_fury_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_blood_fury_SpellScript);

            void HandleOnHit()
            {
                if (GetCaster()->HasAura(GetSpellInfo()->Id) && GetCaster()->getClass() != CLASS_HUNTER)
                {
                    GetCaster()->RemoveAura(GetSpellInfo()->Id);
                    GetCaster()->CastSpell(GetCaster(), 33697, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_gen_blood_fury_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_blood_fury_SpellScript();
        }
};

#define GOBELING_GUMBO_BURP 42755

// Gobelin Gumbo - 42760
class spell_gen_gobelin_gumbo: public SpellScriptLoader
{
    public:
        spell_gen_gobelin_gumbo() : SpellScriptLoader("spell_gen_gobelin_gumbo") { }

        class spell_gen_gobelin_gumbo_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_gobelin_gumbo_AuraScript);

            uint32 burpTimer;

            bool Load()
            {
                burpTimer = 10000;
                return true;
            }

            void OnUpdate(uint32 diff, AuraEffect* aurEff)
            {
                if (GetCaster())
                {
                    if (burpTimer <= diff)
                    {
                        burpTimer = 10000;
                        if (roll_chance_i(30))
                            GetCaster()->CastSpell(GetCaster(), GOBELING_GUMBO_BURP, true);
                    }
                    else
                        burpTimer -= diff;
                }
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_gen_gobelin_gumbo_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_gobelin_gumbo_AuraScript();
        }
};

enum SilvermoonPolymorph
{
    NPC_AUROSALIA   = 18744,
};

enum magePolymorphForms
{
    SPELL_MAGE_SQUIRREL_FORM                     = 32813,
    SPELL_MAGE_GIRAFFE_FORM                      = 32816,
    SPELL_MAGE_SERPENT_FORM                      = 32817,
    SPELL_MAGE_DRAGONHAWK_FORM                   = 32818,
    SPELL_MAGE_WORGEN_FORM                       = 32819,
    SPELL_MAGE_SHEEP_FORM                        = 32820,
};

// TODO: move out of here and rename - not a mage spell
class spell_mage_polymorph_cast_visual: public SpellScriptLoader
{
    public:
        spell_mage_polymorph_cast_visual() : SpellScriptLoader("spell_mage_polymorph_visual") { }

        class spell_mage_polymorph_cast_visual_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_polymorph_cast_visual_SpellScript);

            static const uint32 PolymorhForms[6];

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                // check if spell ids exist in dbc
                for (uint32 i = 0; i < 6; i++)
                    if (!sSpellMgr->GetSpellInfo(PolymorhForms[i]))
                        return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* target = GetCaster()->FindNearestCreature(NPC_AUROSALIA, 30.0f))
                    if (target->GetTypeId() == TYPEID_UNIT)
                        target->CastSpell(target, PolymorhForms[urand(0, 5)], true);
            }

            void Register()
            {
                // add dummy effect spell handler to Polymorph visual
                OnEffectHitTarget += SpellEffectFn(spell_mage_polymorph_cast_visual_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mage_polymorph_cast_visual_SpellScript();
        }
};

const uint32 spell_mage_polymorph_cast_visual::spell_mage_polymorph_cast_visual_SpellScript::PolymorhForms[6] =
{
    SPELL_MAGE_SQUIRREL_FORM,
    SPELL_MAGE_GIRAFFE_FORM,
    SPELL_MAGE_SERPENT_FORM,
    SPELL_MAGE_DRAGONHAWK_FORM,
    SPELL_MAGE_WORGEN_FORM,
    SPELL_MAGE_SHEEP_FORM
};

// Hardened Shell - 129787
class spell_gen_hardened_shell: public SpellScriptLoader
{
    public:
        spell_gen_hardened_shell() : SpellScriptLoader("spell_gen_hardened_shell") { }

        class spell_gen_hardened_shell_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_hardened_shell_AuraScript);

            void HandleOnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetTarget())
                    {
                        if (target->IsMounted())
                        {
                            target->RemoveAurasByType(SPELL_AURA_MOUNTED);
                            target->Dismount();
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_gen_hardened_shell_AuraScript::HandleOnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_hardened_shell_AuraScript();
        }
};

class spell_gen_ds_flush_knockback: public SpellScriptLoader
{
    public:
        spell_gen_ds_flush_knockback() : SpellScriptLoader("spell_gen_ds_flush_knockback") {}

        class spell_gen_ds_flush_knockback_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_ds_flush_knockback_SpellScript);

            void HandleOnHit()
            {
                // Here the target is the water spout and determines the position where the player is knocked from
                if (Unit* caster = GetCaster())
                {
                    if (!GetHitUnit())
                        return;

                    if (Player* player = GetHitUnit()->ToPlayer())
                    {
                        float horizontalSpeed = 20.0f + (40.0f - GetCaster()->GetDistance(caster));
                        float verticalSpeed = 8.0f;
                        // This method relies on the Dalaran Sewer map disposition and Water Spout position
                        // What we do is knock the player from a position exactly behind him and at the end of the pipe
                        player->KnockbackFrom(caster->GetPositionX(), player->GetPositionY(), horizontalSpeed, verticalSpeed);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_gen_ds_flush_knockback_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_ds_flush_knockback_SpellScript();
        }
};

/// Last Update 6.2.3
/// Orb of Power - 121164 / 121175 / 121176 / 121177
class spell_gen_orb_of_power: public SpellScriptLoader
{
    public:
        spell_gen_orb_of_power() : SpellScriptLoader("spell_gen_orb_of_power") { }

        class spell_gen_orb_of_power_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_orb_of_power_AuraScript);

            void OnTick(AuraEffect const* /*aurEff*/)
            {
                Unit* l_Target = GetTarget();

                if (AuraEffect* l_DamageDone = l_Target->GetAuraEffect(GetSpellInfo()->Id, EFFECT_2))
                {
                    // Max +200% damage done
                    if (l_DamageDone->GetAmount() + 10 >= 100)
                        l_DamageDone->ChangeAmount(100);
                    else
                        l_DamageDone->ChangeAmount(l_DamageDone->GetAmount() + 10);
                }
                if (AuraEffect* l_Healing = l_Target->GetAuraEffect(GetSpellInfo()->Id, EFFECT_0))
                {
                    // Max -90% heal taken
                    if (l_Healing->GetAmount() - 5 <= -90)
                        l_Healing->ChangeAmount(-90);
                    else
                        l_Healing->ChangeAmount(l_Healing->GetAmount() - 5);
                }
                if (AuraEffect* l_DamageTaken = l_Target->GetAuraEffect(GetSpellInfo()->Id, EFFECT_1))
                {
                    // Max +500% damage taken
                    if (l_DamageTaken->GetAmount() + 30 >= 500)
                        l_DamageTaken->ChangeAmount(500);
                    else
                        l_DamageTaken->ChangeAmount(l_DamageTaken->GetAmount() + 30);
                }
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();

                l_Target->RemoveFlagsAuras();
            }


            void OnUpdate(uint32 /*diff*/, AuraEffect* /*aurEff*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                Player* l_Player = l_Caster->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (!l_Player->GetMap()->IsBattlegroundOrArena())
                    l_Player->RemoveAura(GetSpellInfo()->Id);
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_gen_orb_of_power_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_MOD_HEALING_PCT);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_gen_orb_of_power_AuraScript::OnTick, EFFECT_3, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectRemove += AuraEffectRemoveFn(spell_gen_orb_of_power_AuraScript::OnRemove, EFFECT_3, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_orb_of_power_AuraScript();
        }
};

// Whispers of Insanity - 176151, custom script, that buff is used to reward player vote
class spell_vote_buff: public SpellScriptLoader
{
    public:
        spell_vote_buff() : SpellScriptLoader("spell_vote_buff") { }

        class spell_vote_buff_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_vote_buff_AuraScript);

            void CalculateEffectAmount(AuraEffect const* p_AuraEffect, int32& p_Amount, bool& p_CanBeRecalculated)
            {
                if (!GetUnitOwner())
                    return;

                p_Amount = GetUnitOwner()->getLevel() * 2;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_vote_buff_AuraScript::CalculateEffectAmount, EFFECT_0, SPELL_AURA_MOD_STAT);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_vote_buff_AuraScript();
        }
};

// Touch of Elune - 45529
class PlayerScript_Touch_Of_Elune: public PlayerScript
{
    public:
        PlayerScript_Touch_Of_Elune() :PlayerScript("PlayerScript_Touch_Of_Elune") {}

        enum TouchOfELuneSpell
        {
            Day         = 154796,
            Night       = 154797,
            Racial      = 154748,
            EventNight  = 25 // Id on game_event
        };

        void OnUpdate(Player* p_Player, uint32 p_Diff)
        {
            if (p_Player->getRace() != Races::RACE_NIGHTELF || !p_Player->HasSpell(TouchOfELuneSpell::Racial))
                return;

            if (sGameEventMgr->IsActiveEvent(TouchOfELuneSpell::EventNight) && !p_Player->HasAura(TouchOfELuneSpell::Night))
            {
                p_Player->RemoveAura(TouchOfELuneSpell::Day);
                p_Player->AddAura(TouchOfELuneSpell::Night, p_Player);
            }
            else if (!p_Player->HasAura(TouchOfELuneSpell::Day) && !sGameEventMgr->IsActiveEvent(TouchOfELuneSpell::EventNight))
            {
                p_Player->RemoveAura(TouchOfELuneSpell::Night);
                p_Player->AddAura(TouchOfELuneSpell::Day, p_Player);
            }
        }
};

namespace Resolve
{
    enum
    {
        PassiveAura  = 158298,
        InCombatAura = 158300,
        CycleTime    = 10000,       ///< 10 sec (10000 ms)
        TickTimer    = 1000,        ///< 1 sec (1000 ms)
    };

    static int const k_CycleInSecs = 10;
    static float const k_Multiplier  = 2.5f; ///< Multiplier on damage historic used when school isn't physical or when damage come from DOT

    /// Resolve "basic equal-level creature damage" expectation values
    /// from http://us.battle.net/wow/en/forum/topic/14058407204?page=2#39
    /// using 1 as placeholder for unknown to avoid divide-by-zero errors
    static double const k_ResolveDpsByLevel[MAX_LEVEL] =
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       ///< 01-10
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       ///< 11-20
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       ///< 21-30
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       ///< 31-40
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       ///< 41-50
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       ///< 51-60
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       ///< 61-70
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       ///< 71-80
        1, 1, 1, 1, 1, 1, 1, 1, 1, 847.125, ///< 81-90
        1161, 1425.06, 1796.76, 2179.35, 2646, 3114.45, 3238.65, 3364.2, 3489.75, 3615.3 ///< 91-100
    };

    struct DamageHistoryEntry
    {
        DamageHistoryEntry(uint32 p_Damage, DamageEffectType p_EffectType, SpellSchoolMask p_SpellSchoolMask, time_t p_Timestamp)
        {
            Damage     = p_Damage;
            EffectType = p_EffectType;
            SchoolMask = p_SpellSchoolMask;
            Timestamp  = p_Timestamp;
        }

        uint32           Damage;
        DamageEffectType EffectType;
        SpellSchoolMask  SchoolMask;
        time_t           Timestamp;
    };

    typedef std::vector<DamageHistoryEntry> DamagesHistory;
    typedef std::map<uint64/*PlayerGUID*/, DamagesHistory> PlayerHistory;
    typedef std::map<uint64, uint32> PlayerTimers;

    class PlayerScript_Resolve : public PlayerScript
    {
        private:
            PlayerHistory m_HistoryDamagesPlayers;
            PlayerTimers  m_Timers;

        public:
            PlayerScript_Resolve() : PlayerScript("PlayerScript_Resolve") {}

            void OnLogout(Player* p_Player) override
            {
                if (m_HistoryDamagesPlayers.find(p_Player->GetGUID()) != m_HistoryDamagesPlayers.end())
                    m_HistoryDamagesPlayers.erase(p_Player->GetGUID());

                if (m_Timers.find(p_Player->GetGUID()) != m_Timers.end())
                    m_Timers.erase(p_Player->GetGUID());
            }

            void OnEnterInCombat(Player* p_Player) override
            {
                if (!ResolveIsAvailable(p_Player))
                    return;

                if (!p_Player->HasAura(Resolve::InCombatAura))
                    p_Player->CastSpell(p_Player, Resolve::InCombatAura, true);

                m_Timers[p_Player->GetGUID()] = Resolve::TickTimer;
            }

            void OnTakeDamage(Player* p_Player, DamageEffectType p_DamageEffectType, uint32 p_Damage, SpellSchoolMask p_SchoolMask, CleanDamage const* p_CleanDamage) override
            {
                if (!ResolveIsAvailable(p_Player) || p_CleanDamage == nullptr)
                    return;

                if (p_DamageEffectType == DamageEffectType::SELF_DAMAGE
                    || p_DamageEffectType == DamageEffectType::HEAL)
                    return;

                auto& l_DamagesHistory = m_HistoryDamagesPlayers[p_Player->GetGUID()];
                l_DamagesHistory.push_back(Resolve::DamageHistoryEntry(p_CleanDamage->absorbed_damage + p_CleanDamage->mitigated_damage + p_Damage, p_DamageEffectType, p_SchoolMask, time(nullptr)));

                Update(p_Player);
            }

            void OnUpdate(Player* p_Player, uint32 p_Diff) override
            {
                if (!ResolveIsAvailable(p_Player))
                    return;

                auto& l_DamagesHistory = m_HistoryDamagesPlayers[p_Player->GetGUID()];

                if (!p_Player->isInCombat() && l_DamagesHistory.empty())
                {
                    p_Player->RemoveAurasDueToSpell(Resolve::InCombatAura);
                    return;
                }

                auto& l_Timer = m_Timers[p_Player->GetGUID()];
                if (l_Timer <= p_Diff)
                {
                    Update(p_Player);
                    l_Timer = Resolve::TickTimer;
                }
                else
                    l_Timer -= p_Diff;
            }

        private:

            /// Check if the current player is avaiable to resolve aura
            /// @p_Player : player to check
            bool ResolveIsAvailable(Player* p_Player)
            {
                if (!p_Player->HasAura(Resolve::PassiveAura))
                    return false;

                if (p_Player->IsInPvPCombat() || (p_Player->GetMap() && p_Player->GetMap()->IsBattlegroundOrArena()))
                    return false;

                return true;
            }

            /// Update amount of resolve auras
            /// Call at each 1sec tick or at any damage taken
            /// @p_Player: player to update
            void Update(Player* p_Player)
            {
                /// - Remove old damage log (> 10 secs)
                auto l_Timestamp = time(nullptr);
                auto& l_History = m_HistoryDamagesPlayers[p_Player->GetGUID()];
                l_History.erase(std::remove_if(l_History.begin(), l_History.end(), [l_Timestamp](DamageHistoryEntry const& p_DamageLog)
                {
                    if (p_DamageLog.Timestamp + k_CycleInSecs < l_Timestamp)
                        return true;

                    return false;
                }), l_History.end());

                /// - Get the resolve multiplier from player level
                int    l_PlayerLevel       = std::max(static_cast<int>(p_Player->getLevel()), MAX_LEVEL);
                double l_BaseDamageAtLevel = k_ResolveDpsByLevel[l_PlayerLevel - 1];

                /// Not blizzlike, but blizz have given only 90-100 data
                if (l_BaseDamageAtLevel == 1)
                    l_BaseDamageAtLevel = sObjectMgr->GetCreatureBaseStats(l_PlayerLevel, CLASS_WARRIOR)->BaseDamage;

                double l_DamageModCoef = 1 / (10 * l_BaseDamageAtLevel);

                /// - Compute damage modifier amount from history
                double l_DamageMod   = 0.0f;

                for (auto l_DamageHistoryEntry : l_History)
                {
                    double l_Damage = l_DamageHistoryEntry.Damage;
                    double l_DeltaT = (l_Timestamp - l_DamageHistoryEntry.Timestamp);

                    if ((l_DamageHistoryEntry.SchoolMask & SpellSchoolMask::SPELL_SCHOOL_MASK_NORMAL) == 0
                        || l_DamageHistoryEntry.EffectType == DamageEffectType::DOT)
                        l_Damage *= k_Multiplier;

                    l_Damage *= 2.0 * (k_CycleInSecs - l_DeltaT) / k_CycleInSecs;
                    l_DamageMod += l_Damage;
                }

                auto l_TotalDamage = l_DamageMod;
                l_DamageMod *= l_DamageModCoef;

                /// - Compute the new resolve amount
                double l_ResolveAmount = 100 * std::max(0.0, 3.4 * (1 - std::exp(-0.045 * l_DamageMod)) - 1.0);

                /// - Update the amount of the tooltip aura
                auto l_ResolveAura = p_Player->GetAura(Resolve::InCombatAura);
                if (l_ResolveAura != nullptr)
                {
                    /// - Update visual percentage on the aura tooltip
                    auto l_VisualAuraEffect = l_ResolveAura->GetEffect(SpellEffIndex::EFFECT_0);
                    if (l_VisualAuraEffect != nullptr)
                        l_VisualAuraEffect->ChangeAmount(l_ResolveAmount);

                    /// - Update damage taken on the aura tooltip
                    auto l_DamageAuraEffect = l_ResolveAura->GetEffect(SpellEffIndex::EFFECT_2);
                    if (l_DamageAuraEffect != nullptr)
                        l_DamageAuraEffect->ChangeAmount(l_TotalDamage);
                }

                /// - Update the amount of the passive
                auto l_ResolvePassiveAura = p_Player->GetAura(Resolve::PassiveAura);
                if (l_ResolvePassiveAura != nullptr)
                {
                    /// - Update healing done modifier percentage
                    auto l_HealingModifierEffect = l_ResolvePassiveAura->GetEffect(SpellEffIndex::EFFECT_1);
                    if (l_HealingModifierEffect != nullptr)
                        l_HealingModifierEffect->ChangeAmount(l_ResolveAmount);

                    /// - Update absorption done modifier percentage
                    auto l_AbsorptionModifierEffect = l_ResolvePassiveAura->GetEffect(SpellEffIndex::EFFECT_2);
                    if (l_AbsorptionModifierEffect != nullptr)
                        l_AbsorptionModifierEffect->ChangeAmount(l_ResolveAmount);
                }
            }
    };

    class spell_resolve_passive : public SpellScriptLoader
    {
    public:
        spell_resolve_passive() : SpellScriptLoader("spell_resolve_passive") { }

        class spell_resolve_passive_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_resolve_passive_AuraScript);

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Target = GetTarget();
                if (l_Target == nullptr)
                    return;

                l_Target->RemoveAurasDueToSpell(Resolve::InCombatAura);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_resolve_passive_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_MOD_HEALING_DONE_PERCENT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_resolve_passive_AuraScript();
        }
    };
}

class spell_gen_dampening : public SpellScriptLoader
{
    public:
        spell_gen_dampening() : SpellScriptLoader("spell_gen_dampening") { }

        class spell_gen_dampening_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_dampening_AuraScript);

            void OnTick(AuraEffect const* p_AurEff)
            {
                if (AuraEffect* l_FirstEffect = p_AurEff->GetBase()->GetEffect(EFFECT_0))
                {
                    if (l_FirstEffect->GetAmount() < 100)
                        l_FirstEffect->SetAmount(l_FirstEffect->GetAmount() + 1);
                }
            }


            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_gen_dampening_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_dampening_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Drums of Fury - 1178207, Drums of Rage - 146555
class spell_gen_drums_of_fury : public SpellScriptLoader
{
    public:
        spell_gen_drums_of_fury() : SpellScriptLoader("spell_gen_drums_of_fury") { }

        class spell_gen_drums_of_fury_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_drums_of_fury_SpellScript);

            enum eSpells
            {
                Exhausted = 57723,
                Insanity = 95809,
                Sated = 57724,
                TemporalDisplacement = 80354,
                Fatigued = 160455
            };

            void HandleImmunity(SpellEffIndex p_EffIndex)
            {
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (l_Target->HasAura(eSpells::Exhausted) || l_Target->HasAura(eSpells::Insanity) ||
                    l_Target->HasAura(eSpells::Sated) || l_Target->HasAura(eSpells::TemporalDisplacement) ||
                    l_Target->HasAura(eSpells::Fatigued))
                    PreventHitAura();
            }

            void HandleAfterHit()
            {
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (!l_Target->HasAura(eSpells::Exhausted))
                    l_Target->CastSpell(l_Target, eSpells::Exhausted, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_gen_drums_of_fury_SpellScript::HandleImmunity, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
                OnEffectHitTarget += SpellEffectFn(spell_gen_drums_of_fury_SpellScript::HandleImmunity, EFFECT_1, SPELL_EFFECT_APPLY_AURA);
                AfterHit += SpellHitFn(spell_gen_drums_of_fury_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_drums_of_fury_SpellScript();
        }
};

class spell_gen_selfie_camera : public SpellScriptLoader
{
    public:
        spell_gen_selfie_camera() : SpellScriptLoader("spell_gen_selfie_camera") { }


        class spell_gen_selfie_camera_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_selfie_camera_AuraScript);

            uint64 m_PhotoBinberGUID = 0;

            enum eData
            {
                PhotoBomberNPC  = 91977,
                VisualKit       = 54168
            };

            void OnApply(AuraEffect const* p_AurEff, AuraEffectHandleModes /* p_Mode */)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (l_Caster->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                l_Caster->ToPlayer()->SendPlaySpellVisualKit(eData::VisualKit, 2, 0);

                std::list<Creature*> l_ListPhotoBomber;

                l_Caster->GetCreatureListWithEntryInGrid(l_ListPhotoBomber, eData::PhotoBomberNPC, 100.0f);

                /// Remove other players Master PhotoBomber
                for (std::list<Creature*>::iterator l_Itr = l_ListPhotoBomber.begin(); l_Itr != l_ListPhotoBomber.end(); l_Itr++)
                {
                    Unit* l_Owner = (*l_Itr)->GetOwner();

                    if (l_Owner != nullptr && l_Owner->GetGUID() == l_Caster->GetGUID() && (*l_Itr)->isSummon())
                    {
                        m_PhotoBinberGUID = (*l_Itr)->GetGUID();
                        break;
                    }
                }
            }

            void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /* p_Mode */)
            {
                Creature* l_PhotoBomber = ObjectAccessor::FindCreature(m_PhotoBinberGUID);

                if (l_PhotoBomber != nullptr)
                    l_PhotoBomber->DespawnOrUnsummon();

                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (l_Caster->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                l_Caster->ToPlayer()->CancelSpellVisualKit(eData::VisualKit);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_gen_selfie_camera_AuraScript::OnApply, EFFECT_0, SPELL_AURA_482, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_gen_selfie_camera_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_482, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_selfie_camera_AuraScript();
        }
};

/// 181883 - S.E.L.F.I.E. Lens Upgrade Kit
class spell_gen_selfie_lens_upgrade_kit : public SpellScriptLoader
{
    enum ItemIDs
    {
        SELFIECameraMkII = 122674
    };

    public:
        /// Constructor
        spell_gen_selfie_lens_upgrade_kit()
            : SpellScriptLoader("spell_gen_selfie_lens_upgrade_kit")
        {

        }

        /// Spell script
        class spell_gen_selfie_lens_upgrade_kit_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_selfie_lens_upgrade_kit_SpellScript);

            /// After the spell is caster
            void OnAfterCast()
            {
                if (GetCaster()->ToPlayer())
                    GetCaster()->ToPlayer()->AddItem(ItemIDs::SELFIECameraMkII, 1);
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_gen_selfie_lens_upgrade_kit_SpellScript::OnAfterCast);
            }
        };

        /// Get a fresh spell script instance
        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_selfie_lens_upgrade_kit_SpellScript();
        }

};

/// Carrying Seaforium - 52410
class spell_gen_carrying_seaforium : public SpellScriptLoader
{
    public:
        spell_gen_carrying_seaforium() : SpellScriptLoader("spell_gen_carrying_seaforium") { }

        class spell_gen_carrying_seaforium_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_carrying_seaforium_SpellScript);

            enum eSpells
            {
                CARRYING_SEAFORIUM_CAST = 52415
            };

            void HandleAfterCast()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                l_Caster->RemoveAura(eSpells::CARRYING_SEAFORIUM_CAST);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_gen_carrying_seaforium_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_carrying_seaforium_SpellScript();
        }
};

/// Inherit Master Threat List - 58838 - last update: 5.4.2 (17688)
class spell_inherit_master_threat_list : public SpellScriptLoader
{
    public:
        spell_inherit_master_threat_list() : SpellScriptLoader("spell_inherit_master_threat_list") {}

        class spell_inherit_master_threat_list_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_inherit_master_threat_list_SpellScript);

            void HandleCopy(SpellEffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (!GetHitUnit())
                     return;

                    if (Creature* l_Target = GetHitUnit()->ToCreature())
                    {
                        l_Target->getThreatManager().clearReferences();
                        std::list<HostileReference*>& l_PlayerThreatManager = l_Caster->getThreatManager().getThreatList();
                        for (HostileReference* l_Threat : l_PlayerThreatManager)
                        {
                            if (Unit* l_Obj = Unit::GetUnit(*l_Target, l_Threat->getUnitGuid()))
                                l_Target->getThreatManager().addThreat(l_Target, l_Threat->getThreat());
                        }
                    }
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_inherit_master_threat_list_SpellScript::HandleCopy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
            }
        };

    SpellScript* GetSpellScript() const
    {
        return new spell_inherit_master_threat_list_SpellScript;
    }
};

/// Taunt Flag Targeting - 51640 - last update: 6.1.2 19865
class spell_taunt_flag_targeting : public SpellScriptLoader
{
    public:
        spell_taunt_flag_targeting() : SpellScriptLoader("spell_taunt_flag_targeting") {}

        class spell_taunt_flag_targeting_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_taunt_flag_targeting_SpellScript);

            Position m_BestTargetPos;
            char const* m_BestTargetName = nullptr;
            std::size_t m_BestTargetNameLength = 0;

            SpellCastResult CheckCast()
            {
                /// Used to improve error message
                bool l_FoundCorpse = false;
                bool l_InFrontOfMe = false;
                if (Player* l_Caster = GetCaster()->ToPlayer())
                {
                    float l_SearchDist = GetSpellInfo()->Effects[SpellEffIndex::EFFECT_0].CalcRadius(l_Caster);

                    std::list<WorldObject*> l_Targets;
                    JadeCore::AllWorldObjectsInRange l_Check(l_Caster, l_SearchDist);
                    JadeCore::WorldObjectListSearcher<JadeCore::AllWorldObjectsInRange> l_Searcher(l_Caster, l_Targets, l_Check);
                    l_Caster->VisitNearbyObject(l_SearchDist, l_Searcher);

                    Position l_CasterPos;
                    l_Caster->GetPosition(&l_CasterPos);

                    for (auto l_It = l_Targets.begin(); l_It != l_Targets.end(); ++l_It)
                    {
                        /// Either we have a corpse, either we have a player
                        if (Corpse* l_Corpse = (*l_It)->ToCorpse())
                        {
                            if (Player* l_Owner = ObjectAccessor::FindPlayer(l_Corpse->GetOwnerGUID()))
                            {
                                if (l_Owner->GetGUID() == l_Caster->GetGUID())
                                    continue;

                                l_FoundCorpse = true;

                                if (!l_Caster->isInFront(l_Corpse)) ///< Only corpses in front of us
                                    continue;

                                l_InFrontOfMe = true;

                                if (m_BestTargetName && m_BestTargetPos.GetExactDistSq(&l_CasterPos) < l_Corpse->GetExactDistSq(&l_CasterPos))
                                    continue;

                                if (l_Caster->GetReactionTo(l_Owner) > REP_NEUTRAL || l_Owner->GetReactionTo(l_Caster) > REP_NEUTRAL) ///< Only enemies corpses
                                    continue;

                                l_Corpse->GetPosition(&m_BestTargetPos);

                                m_BestTargetName = l_Owner->GetName();
                                m_BestTargetNameLength = l_Owner->GetNameLength();
                            }
                        }
                        else if (Player* l_Player = (*l_It)->ToPlayer())
                        {
                            if (l_Player->GetGUID() == l_Caster->GetGUID())
                                continue;

                            if (l_Player->isAlive()) ///< Only corpses
                                continue;

                            l_FoundCorpse = true;

                            if (!l_Caster->isInFront(l_Player))
                                continue;

                            l_InFrontOfMe = true;

                            /// Skips ghosts
                            if (l_Player->HasAuraType(SPELL_AURA_GHOST))
                                continue;

                            /// Is this corpse closer?
                            if (m_BestTargetName && m_BestTargetPos.GetExactDistSq(&l_CasterPos) < l_Player->GetExactDistSq(&l_CasterPos))
                                continue;

                            if (l_Caster->GetReactionTo(l_Player) > REP_NEUTRAL || l_Player->GetReactionTo(l_Caster) > REP_NEUTRAL) ///< Only enemies corpses
                                continue;

                            l_Player->GetPosition(&m_BestTargetPos);

                            m_BestTargetName = l_Player->GetName();
                            m_BestTargetNameLength = l_Player->GetNameLength();
                        }
                    }
                }

                if (!m_BestTargetName)
                {
                    if (l_FoundCorpse)
                        return (l_InFrontOfMe) ? SpellCastResult::SPELL_FAILED_BAD_TARGETS : SpellCastResult::SPELL_FAILED_NOT_INFRONT;
                    else
                    {
                        SetCustomCastResultMessage(SpellCustomErrors::SPELL_CUSTOM_ERROR_NO_NEARBY_CORPSES);
                        return SpellCastResult::SPELL_FAILED_CUSTOM_ERROR;
                    }
                }

                return SpellCastResult::SPELL_CAST_OK;
            }

            void HandleCast(SpellEffIndex p_Index)
            {
                if (Player* l_Caster = GetCaster()->ToPlayer())
                {
                    if (m_BestTargetName)
                    {
                        l_Caster->CastSpell(m_BestTargetPos, 51657, true);
                        l_Caster->AddSpellCooldown(GetSpellInfo()->Id, 0, 60 * IN_MILLISECONDS, true);

                        if (WorldSession* l_Session = l_Caster->GetSession())
                        {
                            char const* l_TauntText = l_Session->GetTrinityString(LangTauntFlag);
                            std::size_t l_TauntTextLength = std::strlen(l_TauntText);

                            std::string l_Text;
                            l_Text.reserve(l_TauntTextLength + m_BestTargetNameLength);
                            l_Text.append(l_TauntText, l_TauntTextLength);
                            l_Text.append(m_BestTargetName);

                            WorldPacket l_Data;
                            /// No specific target needed
                            l_Caster->BuildPlayerChat(&l_Data, nullptr, CHAT_MSG_EMOTE, l_Text, LANG_UNIVERSAL);
                            l_Session->SendPacket(&l_Data);
                        }
                    }
                }
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_taunt_flag_targeting_SpellScript::CheckCast);
                OnEffectLaunch += SpellEffectFn(spell_taunt_flag_targeting_SpellScript::HandleCast, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_taunt_flag_targeting_SpellScript;
        }
};

/// Arcane Brillance - 1459, Dark Intent - 109773, Legacy of the White Tiger - 116781
class spell_gen_raid_buff_stack : public SpellScriptLoader
{
    public:
        spell_gen_raid_buff_stack() : SpellScriptLoader("spell_gen_raid_buff_stack") { }

        class spell_gen_raid_buff_stack_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_raid_buff_stack_SpellScript);

            enum eSpells
            {
                DarkIntent = 109773,
                ArcaneBrillance = 1459,
                LegacyoftheWhiteTiger = 116781,
            };

            static uint8 const l_TabAuraSize = 3;

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                uint32 l_TabAura[l_TabAuraSize] = { eSpells::DarkIntent, eSpells::ArcaneBrillance, eSpells::LegacyoftheWhiteTiger };

                for (uint8 l_Idx = 0; l_Idx < l_TabAuraSize; ++l_Idx)
                {
                    if (GetSpellInfo()->Id != l_TabAura[l_Idx])
                        p_Targets.remove_if(JadeCore::UnitAuraCheck(true, l_TabAura[l_Idx]));
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_gen_raid_buff_stack_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_ALLY_OR_RAID);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_gen_raid_buff_stack_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_ALLY_OR_RAID);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_raid_buff_stack_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Sword Technique - 177189
class spell_gen_sword_technique: public SpellScriptLoader
{
    public:
        spell_gen_sword_technique() : SpellScriptLoader("spell_gen_sword_technique") { }

        class spell_gen_sword_technique_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_sword_technique_AuraScript);

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*canBeRecalculated*/)
            {
                if (GetCaster() == nullptr)
                    return;

                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if ((l_Player->GetMap() && l_Player->GetMap()->IsBattlegroundOrArena()) || l_Player->IsInPvPCombat())
                    p_Amount -= CalculatePct(p_Amount, 40);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_sword_technique_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_STAT);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_sword_technique_AuraScript();
        }
};

/// Faction check for spells (seems to cause problems when applied to every spells)
class spell_gen_check_faction : public SpellScriptLoader
{
    public:
        spell_gen_check_faction() : SpellScriptLoader("spell_gen_check_faction") {}

        class spell_gen_check_faction_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_check_faction_SpellScript);

            SpellCastResult CheckFaction()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    uint32 l_RaceMask = l_Caster->getRaceMask();
                    uint32 l_AttributeEx7 = GetSpellInfo()->AttributesEx7;

                    if ((l_AttributeEx7 & SpellAttr7::SPELL_ATTR7_ALLIANCE_ONLY && ((l_RaceMask & RACEMASK_ALLIANCE) == 0)) ||
                        (l_AttributeEx7 & SpellAttr7::SPELL_ATTR7_HORDE_ONLY && ((l_RaceMask & RACEMASK_HORDE) == 0)))
                        return SpellCastResult::SPELL_FAILED_SPELL_UNAVAILABLE;
                }

                return SpellCastResult::SPELL_CAST_OK;
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_gen_check_faction_SpellScript::CheckFaction);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_check_faction_SpellScript();
        }
};

/// Stoneform - 20594
class spell_gen_stoneform_dwarf_racial : public SpellScriptLoader
{
    public:
        spell_gen_stoneform_dwarf_racial() : SpellScriptLoader("spell_gen_stoneform_dwarf_racial") {}

        class spell_gen_stoneform_dwarf_racial_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_stoneform_dwarf_racial_SpellScript);

            enum eSpell
            {
                PvPTrinket = 42292
            };

            void HandleAfterCast()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    /// Patch note 6.1 : "Stoneform when used, now triggers a 30-second shared cooldown with other PvP trinkets that breaks crowd-control effects."
                    l_Player->AddSpellCooldown(eSpell::PvPTrinket, 0, 30 * TimeConstants::IN_MILLISECONDS);

                    if (Item* l_FirstTrinket = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, EquipmentSlots::EQUIPMENT_SLOT_TRINKET1))
                    {
                        for (uint8 l_I = 0; l_I < MAX_ITEM_PROTO_SPELLS; ++l_I)
                        {
                            _Spell const& l_SpellData = l_FirstTrinket->GetTemplate()->Spells[l_I];

                            /// No spell or not which one we search
                            if (!l_SpellData.SpellId || l_SpellData.SpellId != eSpell::PvPTrinket)
                                continue;

                            l_Player->ApplyEquipCooldown(l_FirstTrinket);
                            break;
                        }
                    }

                    if (Item* l_SecondTrinket = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, EquipmentSlots::EQUIPMENT_SLOT_TRINKET2))
                    {
                        for (uint8 l_I = 0; l_I < MAX_ITEM_PROTO_SPELLS; ++l_I)
                        {
                            _Spell const& l_SpellData = l_SecondTrinket->GetTemplate()->Spells[l_I];

                            /// No spell or not which one we search
                            if (!l_SpellData.SpellId || l_SpellData.SpellId != eSpell::PvPTrinket)
                                continue;

                            l_Player->ApplyEquipCooldown(l_SecondTrinket);
                            break;
                        }
                    }
                }
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_gen_stoneform_dwarf_racial_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_gen_stoneform_dwarf_racial_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Touch of the Grave - 5227
class spell_dru_touch_of_the_grave : public SpellScriptLoader
{
    public:
        spell_dru_touch_of_the_grave() : SpellScriptLoader("spell_dru_touch_of_the_grave") { }

        class spell_dru_touch_of_the_grave_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_touch_of_the_grave_AuraScript);

            enum eSpells
            {
                TouchoftheGraveEffect = 127802
            };

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Attacker = p_EventInfo.GetDamageInfo()->GetAttacker();
                Unit* l_Victim = p_EventInfo.GetDamageInfo()->GetVictim();
                if (l_Attacker == nullptr || l_Victim == nullptr)
                    return;

                if (l_Attacker->GetGUID() == l_Victim->GetGUID() || !l_Attacker->ToPlayer())
                    return;

                if (l_Attacker->ToPlayer()->HasSpellCooldown(eSpells::TouchoftheGraveEffect))
                    return;

                /// Can proc just on damage spell, also check for absorbed damage, because all damage can be absorbed but it's still damage spell
                if (p_EventInfo.GetDamageInfo() && p_EventInfo.GetDamageInfo()->GetDamage() == 0 && p_EventInfo.GetDamageInfo()->GetAbsorb() == 0)
                    return;

                l_Attacker->CastSpell(l_Victim, eSpells::TouchoftheGraveEffect, true);
                l_Attacker->ToPlayer()->AddSpellCooldown(eSpells::TouchoftheGraveEffect, 0, 15 * IN_MILLISECONDS, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_dru_touch_of_the_grave_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_touch_of_the_grave_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Remove Rigor Mortis - 73523 on Fresh Out Of The Grave quest acceptation (24959)
class PlayerScript_gen_remove_rigor_mortis : public PlayerScript
{
    public:
        PlayerScript_gen_remove_rigor_mortis() :PlayerScript("PlayerScript_gen_remove_rigor_mortis") {}

        enum Constants
        {
            QUEST_FRESH_OUT_OF_THE_GRAVE = 24959,
            SPELL_RIGOR_MORTIS = 73523
        };

        void OnQuestAccept(Player* p_Player, Quest const* p_Quest) override
        {
            if (p_Quest->GetQuestId() == Constants::QUEST_FRESH_OUT_OF_THE_GRAVE)
                p_Player->RemoveAura(Constants::SPELL_RIGOR_MORTIS);
        }
};

/// last update : 6.1.2 19802
/// Savage Fortitude - 181706
class spell_gen_savage_fortitude : public SpellScriptLoader
{
    public:
        spell_gen_savage_fortitude() : SpellScriptLoader("spell_gen_savage_fortitude") { }

        class spell_gen_savage_fortitude_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_savage_fortitude_AuraScript);

            void CalculateAmount(AuraEffect const* /*p_AurEff*/, int32& p_Amount, bool& /*canBeRecalculated*/)
            {
                p_Amount *= 10;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_savage_fortitude_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_2);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_savage_fortitude_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Mark of Warsong - 159675
class spell_gen_mark_of_warsong : public SpellScriptLoader
{
    public:
        spell_gen_mark_of_warsong() : SpellScriptLoader("spell_gen_mark_of_warsong") { }

        class spell_gen_mark_of_warsong_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_mark_of_warsong_AuraScript);

            void OnApply(AuraEffect const* p_AurEff, AuraEffectHandleModes /*mode*/)
            {
                p_AurEff->GetBase()->ModStackAmount(9);
            }

            void OnPeriodic(AuraEffect const* p_AurEff)
            {
                if (p_AurEff->GetBase()->GetStackAmount() > 1)
                    p_AurEff->GetBase()->SetStackAmount(p_AurEff->GetBase()->GetStackAmount() - 1);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_gen_mark_of_warsong_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_RATING, AURA_EFFECT_HANDLE_REAL);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_gen_mark_of_warsong_AuraScript::OnPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_mark_of_warsong_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Elixir of Wandering Spirits - 147412
class spell_gen_elixir_of_wandering_spirits : public SpellScriptLoader
{
public:
    spell_gen_elixir_of_wandering_spirits() : SpellScriptLoader("spell_gen_elixir_of_wandering_spirits") { }

    class spell_gen_elixir_of_wandering_spirits_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_elixir_of_wandering_spirits_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {

            Unit* l_Caster = GetCaster();
            if (l_Caster == nullptr)
                return;

            const uint8 l_AmountOfModels = 8;
            /// Array of all spells that change model for this item
            static uint32 const WanderingSpiritsMorphs[l_AmountOfModels] = { 147402, 147403, 147405, 147406, 147407, 147409, 147410, 147411 };

            /// Remove previus auras if have, to prevent usebug with many auras
            for (uint8 l_I = 0; l_I < l_AmountOfModels; l_I++)
                l_Caster->RemoveAura(WanderingSpiritsMorphs[l_I]);

            l_Caster->CastSpell(l_Caster, WanderingSpiritsMorphs[urand(0, 7)], true);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_gen_elixir_of_wandering_spirits_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_gen_elixir_of_wandering_spirits_SpellScript();
    }
};

/// Last Update 6.2.3
/// Shadowmeld - 58984
class spell_gen_shadowmeld : public SpellScriptLoader
{
    public:
        spell_gen_shadowmeld() : SpellScriptLoader("spell_gen_shadowmeld") { }

        class spell_gen_shadowmeld_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_shadowmeld_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                /// 6.1 Hotfixes: March 23 - Should drop the character from pvp combat
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->IsInPvPCombat())
                    l_Player->SetInPvPCombat(false);

                /// Shadowmeld must remove combat
                l_Player->CombatStop();
                l_Player->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_IMMUNE_OR_LOST_SELECTION);
                l_Player->InterruptNonMeleeSpells(true);
                l_Player->getHostileRefManager().deleteReferences();
            }

            void Register()
            {
                AfterEffectApply += AuraEffectRemoveFn(spell_gen_shadowmeld_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_TOTAL_THREAT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_shadowmeld_AuraScript();
        }
};

enum DraenicPhilosophers
{
    ForceAura       = 60229,
    AgilityAura     = 60233,
    IntellectAura   = 60234
};

static uint32 const g_DraenicAuras[3] =
{
    DraenicPhilosophers::ForceAura,
    DraenicPhilosophers::AgilityAura,
    DraenicPhilosophers::IntellectAura
};

static Stats const g_DraenicStats[3] =
{
    STAT_STRENGTH,
    STAT_AGILITY,
    STAT_INTELLECT
};

/// last update : 6.1.2 19802
/// Draenic Philosopher's Stone - 157136
class spell_gen_draenic_philosophers : public SpellScriptLoader
{
    public:
        spell_gen_draenic_philosophers() : SpellScriptLoader("spell_gen_draenic_philosophers") { }

        class spell_gen_draenic_philosophers_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_draenic_philosophers_AuraScript);

            int32 m_Value = 1414;

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Player* l_Player = GetTarget()->ToPlayer();
                Stats l_MaxStatValue = STAT_STRENGTH;

                if (l_Player->HasSpellCooldown(GetSpellInfo()->Id))
                    return;

                for (int8 l_I = 0; l_I < 3; l_I++)
                {
                    if (l_Player->GetStat(g_DraenicStats[l_I]) >= l_Player->GetStat(g_DraenicStats[l_MaxStatValue]))
                        l_MaxStatValue = (Stats)l_I;
                }

                /// Can proc just on damage spell, also check for absorbed damage, because all damage can be absorbed but it's still damage spell
                if (p_EventInfo.GetDamageInfo() && p_EventInfo.GetDamageInfo()->GetDamage() == 0 && p_EventInfo.GetDamageInfo()->GetAbsorb() == 0)
                    return;

                l_Player->AddSpellCooldown(GetSpellInfo()->Id, 0, 55 * IN_MILLISECONDS, true);

                l_Player->CastSpell(l_Player, g_DraenicAuras[l_MaxStatValue], true);
                if (AuraEffect* l_DreanicAura = l_Player->GetAuraEffect(g_DraenicAuras[l_MaxStatValue], EFFECT_0))
                    l_DreanicAura->ChangeAmount(m_Value);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_gen_draenic_philosophers_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_draenic_philosophers_AuraScript();
        }
};

/// last update : 6.1.2
/// Jard's Peculiar Energy Source - 143743
class spell_gen_jards_peculiar_energy_source : public SpellScriptLoader
{
    public:
        spell_gen_jards_peculiar_energy_source() : SpellScriptLoader("spell_gen_jards_peculiar_energy_source") { }

        class spell_gen_jards_peculiar_energy_source_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_jards_peculiar_energy_source_SpellScript);

            enum eSpells 
            {
                SkyGolem                    = 139192,
                RascalBot                   = 143714,
                Pierre                      = 139196,
                AdvancedRefrigerationUnit   = 139197,
                JardPeculiarenergySource    = 139176
            };

            void HandleOnHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                l_Player->learnSpell(eSpells::SkyGolem, false);
                l_Player->learnSpell(eSpells::RascalBot, false);
                l_Player->learnSpell(eSpells::Pierre, false);
                l_Player->learnSpell(eSpells::AdvancedRefrigerationUnit, false);
                l_Player->learnSpell(eSpells::JardPeculiarenergySource, false);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_gen_jards_peculiar_energy_source_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_jards_peculiar_energy_source_SpellScript();
        }
};

/// last update : 6.1.2
/// Celestial Cloth and Its Uses - 143626
class spell_gen_celestial_cloth_and_its_uses : public SpellScriptLoader
{
public:
    spell_gen_celestial_cloth_and_its_uses() : SpellScriptLoader("spell_gen_celestial_cloth_and_its_uses") { }
    
    class spell_gen_celestial_cloth_and_its_uses_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_celestial_cloth_and_its_uses_SpellScript);
        
        enum eSpells
        {
            CelestialCloth              = 143011,
            AcceleratedCelestialCloth   = 146925,
            BeltOfTheNightSky           = 142964,
            LeggingsOfTheNightSky       = 142955,
            WhiteCloudBelt              = 142960,
            WhiteCloudLeggings          = 142951
        };
        
        void HandleOnHit()
        {
            Player* l_Player = GetCaster()->ToPlayer();
            
            if (l_Player == nullptr)
                return;
            
            l_Player->learnSpell(eSpells::CelestialCloth, false);
            l_Player->learnSpell(eSpells::AcceleratedCelestialCloth, false);
            l_Player->learnSpell(eSpells::BeltOfTheNightSky, false);
            l_Player->learnSpell(eSpells::LeggingsOfTheNightSky, false);
            l_Player->learnSpell(eSpells::WhiteCloudBelt, false);
            l_Player->learnSpell(eSpells::WhiteCloudLeggings, false);
        }
        
        void Register()
        {
            OnHit += SpellHitFn(spell_gen_celestial_cloth_and_its_uses_SpellScript::HandleOnHit);
        }
    };
    
    SpellScript* GetSpellScript() const
    {
        return new spell_gen_celestial_cloth_and_its_uses_SpellScript();
    }
};

/// last update : 6.1.2
/// Hardened Magnificent Hide and Its Uses - 143644
class spell_gen_hardened_magnificient_hide_and_its_uses : public SpellScriptLoader
{
public:
    spell_gen_hardened_magnificient_hide_and_its_uses() : SpellScriptLoader("spell_gen_hardened_magnificient_hide_and_its_uses") { }
    
    class spell_gen_hardened_magnificient_hide_and_its_uses_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gen_hardened_magnificient_hide_and_its_uses_SpellScript);
        
        enum eSpells
        {
            HardenMagnificentHide               = 142976,
            AcceleratedHardenMagnificentHide    = 146923,
            GorgeStalkerBelt                    = 142966,
            GorgeStalkerLegplates               = 142957,
            KrasariProwlerBelt                  = 142962,
            KrasariProwlerBritches              = 142953,
            PennyroyalBelt                      = 142961,
            PennyroyalLeggings                  = 142952,
            SnowLilyBelt                        = 142965,
            SnowLilyBritches                    = 142956
        };
        
        void HandleOnHit()
        {
            Player* l_Player = GetCaster()->ToPlayer();
            
            if (l_Player == nullptr)
                return;
            
            l_Player->learnSpell(eSpells::HardenMagnificentHide, false);
            l_Player->learnSpell(eSpells::AcceleratedHardenMagnificentHide, false);
            l_Player->learnSpell(eSpells::GorgeStalkerBelt, false);
            l_Player->learnSpell(eSpells::GorgeStalkerLegplates, false);
            l_Player->learnSpell(eSpells::KrasariProwlerBelt, false);
            l_Player->learnSpell(eSpells::KrasariProwlerBritches, false);
            l_Player->learnSpell(eSpells::PennyroyalBelt, false);
            l_Player->learnSpell(eSpells::PennyroyalLeggings, false);
            l_Player->learnSpell(eSpells::SnowLilyBelt, false);
            l_Player->learnSpell(eSpells::SnowLilyBritches, false);
        }
        
        void Register()
        {
            OnHit += SpellHitFn(spell_gen_hardened_magnificient_hide_and_its_uses_SpellScript::HandleOnHit);
        }
    };
    
    SpellScript* GetSpellScript() const
    {
        return new spell_gen_hardened_magnificient_hide_and_its_uses_SpellScript();
    }
};

/// Power handler
/// Reset timer to correctly start decreasing power at 10 sec
class spell_gen_power_handler : public PlayerScript
{
    public:
        spell_gen_power_handler() : PlayerScript("spell_gen_power_handler") {}

        void OnModifyPower(Player* p_Player, Powers p_Power, int32 p_OldValue, int32& p_NewValue, bool p_Regen)
        {
            if (p_Player->isInCombat())
                return;

            // Get the power earn (if > 0 ) or consum (if < 0)
            int32 l_DiffValue = p_NewValue - p_OldValue;
            if (l_DiffValue < 0)
                return;

            switch (p_Player->getClass())
            {
                case CLASS_PALADIN:
                    if (p_Power == POWER_HOLY_POWER)
                        p_Player->setHolyPowerRegenTimerCount(0);
                    break;
                case CLASS_MONK:
                    if (p_Power == POWER_CHI)
                        p_Player->setChiPowerRegenTimerCount(0);
                    break;
            }
        }

        void OnLeaveCombat(Player* p_Player)
        {
            switch (p_Player->getClass())
            {
                case CLASS_PALADIN:
                    if (p_Player->GetPower(POWER_HOLY_POWER))
                        p_Player->setHolyPowerRegenTimerCount(0);
                    break;
                case CLASS_MONK:
                    if (p_Player->GetPower(POWER_CHI))
                        p_Player->setChiPowerRegenTimerCount(0);
                    break;
            }
        }
};

/// Alchemist's Flask - 105617
enum alchemists_effects
{
    ALCHEMISTS_STRENGTH = 79638,
    ALCHEMISTS_AGILITY = 79639,
    ALCHEMISTS_INTELLECT = 79640
};

class spell_gen_alchemists_flask : public SpellScriptLoader
{
    public:
        spell_gen_alchemists_flask() : SpellScriptLoader("spell_gen_alchemists_flask") { }

        class spell_gen_alchemists_flask_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_alchemists_flask_SpellScript);

            void HandleOnHit()
            {
                if (!GetCaster())
                    return;

                if (!GetHitUnit())
                    return;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    int32 agility = _player->GetTotalStatValue(STAT_AGILITY);
                    int32 strength = _player->GetTotalStatValue(STAT_STRENGTH);
                    int32 intellect = _player->GetTotalStatValue(STAT_INTELLECT);

                    if (intellect > agility && intellect > strength)
                        _player->CastSpell(_player, ALCHEMISTS_INTELLECT, true);
                    else if (agility > intellect && agility > strength)
                        _player->CastSpell(_player, ALCHEMISTS_AGILITY, true);
                    else
                        _player->CastSpell(_player, ALCHEMISTS_STRENGTH, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_gen_alchemists_flask_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_alchemists_flask_SpellScript();
        }
};

/// Potion of Illusion - 80265
class spell_gen_potion_of_illusion : public SpellScriptLoader
{
    public:
        spell_gen_potion_of_illusion() : SpellScriptLoader("spell_gen_potion_of_illusion") { }

        class spell_gen_potion_of_illusion_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_potion_of_illusion_SpellScript);

            enum eSpells
            {
                Illusion = 80396
            };

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();

                l_Caster->CastSpell(l_Caster, eSpells::Illusion, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_gen_potion_of_illusion_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_potion_of_illusion_SpellScript();
        }
};

/// last update : 6.1.2
/// Hemet's Heartseeker - 173286, Megawatt Filament - 156059, Oglethorpe's Missile Splitter - 156052
class spell_gen_inge_trigger_enchant : public SpellScriptLoader
{
    public:
        spell_gen_inge_trigger_enchant() : SpellScriptLoader("spell_gen_inge_trigger_enchant") { }

        class spell_gen_inge_trigger_enchant_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_inge_trigger_enchant_AuraScript);

            enum eSpells
            {
                HemetsHeartseekerAura   = 173286,
                HemetsHeartseeker       = 173288,
                MegawattFilamentAura    = 156059,
                MegawattFilament        = 156060,
                OglethorpesMissileAura  = 156052,
                OglethorpesMissile      = 156055
            };

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();
                
                DamageInfo* l_DamageInfo = p_EventInfo.GetDamageInfo();

                if (l_DamageInfo == nullptr)
                    return;

                Unit* l_Target = l_DamageInfo->GetVictim();
                Unit* l_Caster = l_DamageInfo->GetAttacker();

                if (l_Target == nullptr || l_Caster == nullptr)
                    return;

                uint32 l_WeaponSpeed = l_Caster->GetAttackTime(WeaponAttackType::RangedAttack);
                float l_Chance = l_Caster->GetPPMProcChance(l_WeaponSpeed, 1.55f, GetSpellInfo());

                if (!roll_chance_f(l_Chance))
                    return;

                switch (GetSpellInfo()->Id)
                {
                case eSpells::HemetsHeartseekerAura:
                    l_Caster->CastSpell(l_Caster, eSpells::HemetsHeartseeker, true);
                    break;
                case eSpells::MegawattFilamentAura:
                    l_Caster->CastSpell(l_Caster, eSpells::MegawattFilament, true);
                    break;
                case eSpells::OglethorpesMissileAura:
                    l_Caster->CastSpell(l_Caster, eSpells::OglethorpesMissile, true);
                    break;
                default:
                    break;
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_gen_inge_trigger_enchant_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_inge_trigger_enchant_AuraScript();
        }
};

/// last update : 6.1.2
/// Mark of the Thunderlord - 159234
class spell_gen_mark_of_thunderlord : public SpellScriptLoader
{
    public:
        spell_gen_mark_of_thunderlord() : SpellScriptLoader("spell_gen_mark_of_thunderlord") { }

        class spell_gen_mark_of_thunderlord_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_mark_of_thunderlord_AuraScript);

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (!(p_EventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT))
                    return;

                if (!p_AurEff->GetBase()->GetDuration())
                    return;

                p_AurEff->GetBase()->SetDuration(p_AurEff->GetBase()->GetDuration() + 2 * IN_MILLISECONDS);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_gen_mark_of_thunderlord_AuraScript::OnProc, EFFECT_1, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_mark_of_thunderlord_AuraScript();
        }
};

/// Last Update 6.2.3
/// Iron Horde Pirate Costume - 173956
class spell_generic_iron_horde_pirate_costume : public SpellScriptLoader
{
    public:
        spell_generic_iron_horde_pirate_costume() : SpellScriptLoader("spell_generic_iron_horde_pirate_costume") { }

        class spell_generic_iron_horde_pirate_costume_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_generic_iron_horde_pirate_costume_SpellScript);

            void HandleScriptEffect(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->getGender() == GENDER_MALE)
                    l_Caster->CastSpell(l_Caster, 173958, true);
                else
                    l_Caster->CastSpell(l_Caster, 173959, true);

            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_generic_iron_horde_pirate_costume_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_generic_iron_horde_pirate_costume_SpellScript();
        }
};

/// last update : 6.2.3
/// Kilrogg's Dead Eye - 184762
class spell_gen_kilroggs_dead_eye : public SpellScriptLoader
{
    public:
        spell_gen_kilroggs_dead_eye() : SpellScriptLoader("spell_gen_kilroggs_dead_eye") { }

        class spell_gen_kilroggs_dead_eye_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_kilroggs_dead_eye_AuraScript);

            void OnAbsorb(AuraEffect* p_AurEff, DamageInfo& p_DmgInfo, uint32& p_AbsorbAmount)
            {
                Unit* l_Target = GetCaster();
                if (l_Target == nullptr)
                    return;

                float l_Multiplier = (p_AurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount() / 100) * ((100.0f - l_Target->GetHealthPct()) / 100.0f);
                p_AbsorbAmount = CalculatePct(p_DmgInfo.GetDamage(), l_Multiplier);
            }

            void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
            {
                amount = -1;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_gen_kilroggs_dead_eye_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_gen_kilroggs_dead_eye_AuraScript::OnAbsorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_kilroggs_dead_eye_AuraScript();
        }
};

/// Last Update 6.2.3
/// Blood Elf Illusion - 160331
class spell_gen_blood_elfe_illusion : public SpellScriptLoader
{
    public:
        spell_gen_blood_elfe_illusion() : SpellScriptLoader("spell_gen_blood_elfe_illusion") { }

        class spell_gen_blood_elfe_illusion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_blood_elfe_illusion_AuraScript);

            enum eSpells
            {
                BloodElfMale = 20578,
                BloodElfFemale = 20579,
            };

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                if (l_Caster->getGender() == GENDER_MALE)
                    l_Caster->SetDisplayId(eSpells::BloodElfMale);
                else if (l_Caster->getGender() == GENDER_FEMALE)
                    l_Caster->SetDisplayId(eSpells::BloodElfFemale);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectRemoveFn(spell_gen_blood_elfe_illusion_AuraScript::OnApply, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_blood_elfe_illusion_AuraScript();
        }
};

/// Last Update 6.2.3
/// Wyrmhunter Hooks - 88914
class spell_gen_wyrmhunter_hooks : public SpellScriptLoader
{
    public:
        spell_gen_wyrmhunter_hooks() : SpellScriptLoader("spell_gen_wyrmhunter_hooks") {}

        class spell_gen_wyrmhunter_hooks_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_wyrmhunter_hooks_SpellScript);

            enum eData
            {
                NpcObsidianPyrewing = 46141
            };

            SpellCastResult CheckTarget()
            {
                if (Unit* l_Target = GetExplTargetUnit())
                {
                    if (l_Target->IsPlayer())
                        return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
                    else if (l_Target->ToCreature() && l_Target->ToCreature()->GetEntry() != eData::NpcObsidianPyrewing)
                        return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
                }

                return SpellCastResult::SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_gen_wyrmhunter_hooks_SpellScript::CheckTarget);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_wyrmhunter_hooks_SpellScript();
        }
};

/// Last Update 6.2.3
/// Demon Hunter's Aspect - 113095
class spell_gen_demon_hunters_aspect : public SpellScriptLoader
{
    public:
        spell_gen_demon_hunters_aspect() : SpellScriptLoader("spell_gen_demon_hunters_aspect") { }

        class  spell_gen_demon_hunters_aspect_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_demon_hunters_aspect_AuraScript);

            enum eDatas
            {
                MorphMale = 35911
            };

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->getGender() == GENDER_MALE)
                    l_Player->SetDisplayId(eDatas::MorphMale);
                else ///< TODO : Fine display ID female
                    l_Player->SetDisplayId(eDatas::MorphMale);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                l_Player->SetDisplayId(l_Player->GetNativeDisplayId());
            }

            void Register()
            {
                AfterEffectApply += AuraEffectRemoveFn(spell_gen_demon_hunters_aspect_AuraScript::OnApply, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_gen_demon_hunters_aspect_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_demon_hunters_aspect_AuraScript();
        }
};

/// Last Update 6.2.3
/// Jewel of Hellfire - 187150
class spell_gen_jewel_of_hellfire_trigger : public SpellScriptLoader
{
    public:
        spell_gen_jewel_of_hellfire_trigger() : SpellScriptLoader("spell_gen_jewel_of_hellfire_trigger") { }

        class spell_gen_jewel_of_hellfire_trigger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_jewel_of_hellfire_trigger_SpellScript);

            enum eSpells
            {
                JewelAura = 187174
            };

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster = GetCaster();
                l_Caster->CastSpell(l_Caster, eSpells::JewelAura, false);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_gen_jewel_of_hellfire_trigger_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_jewel_of_hellfire_trigger_SpellScript();
        }
};

/// Last Update 6.2.3
/// Jewel of Hellfire - 187174
class spell_gen_jewel_of_hellfire : public SpellScriptLoader
{
public:
    spell_gen_jewel_of_hellfire() : SpellScriptLoader("spell_gen_jewel_of_hellfire") { }

    class  spell_gen_jewel_of_hellfire_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_gen_jewel_of_hellfire_AuraScript);

        enum eDatas
        {
            MorphMale = 63130,
            MorphFemale = 63138
        };

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* l_Player = GetTarget()->ToPlayer();

            if (l_Player == nullptr)
                return;

            if (l_Player->getGender() == GENDER_MALE)
                l_Player->SetDisplayId(eDatas::MorphMale);
            else
                l_Player->SetDisplayId(eDatas::MorphFemale);
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* l_Player = GetTarget()->ToPlayer();

            if (l_Player == nullptr)
                return;

            l_Player->SetDisplayId(l_Player->GetNativeDisplayId());
        }

        void Register()
        {
            AfterEffectApply += AuraEffectRemoveFn(spell_gen_jewel_of_hellfire_AuraScript::OnApply, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_gen_jewel_of_hellfire_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_gen_jewel_of_hellfire_AuraScript();
    }
};

/// Last Update 6.2.3
/// Wyrmhunter Hooks - 88914
class spell_reconfigured_remote_shock : public SpellScriptLoader
{
    public:
        spell_reconfigured_remote_shock() : SpellScriptLoader("spell_reconfigured_remote_shock") {}

        class spell_reconfigured_remote_shock_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_reconfigured_remote_shock_SpellScript);

            enum eData
            {
                NpcJungleShredder = 67285
            };

            SpellCastResult CheckTarget()
            {
                if (Unit* l_Target = GetExplTargetUnit())
                {
                    if (l_Target->IsPlayer())
                        return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
                    else if (l_Target->ToCreature() && l_Target->ToCreature()->GetEntry() != eData::NpcJungleShredder)
                        return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
                }

                return SpellCastResult::SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_reconfigured_remote_shock_SpellScript::CheckTarget);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_reconfigured_remote_shock_SpellScript();
        }
};

/// Last Update 6.2.3
/// Ironbeard's Hat - 188228
class spell_gen_ironbeards_hat : public SpellScriptLoader
{
    public:
        spell_gen_ironbeards_hat() : SpellScriptLoader("spell_gen_ironbeards_hat") { }

        class  spell_gen_ironbeards_hat_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_ironbeards_hat_AuraScript);

            enum eDatas
            {
                Morph = 63424
            };

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                l_Player->SetDisplayId(eDatas::Morph);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                l_Player->RestoreDisplayId();
            }

            void Register()
            {
                AfterEffectApply += AuraEffectRemoveFn(spell_gen_ironbeards_hat_AuraScript::OnApply, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_gen_ironbeards_hat_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_ironbeards_hat_AuraScript();
        }
};

/// Support for Pilfering Perfume quest(A:24656 H:24541)
enum ServiceUniform
{
    SPELL_SERVICE_UNIFORM       = 71450
};

class spell_gen_service_uniform : public SpellScriptLoader
{
    public:
        spell_gen_service_uniform() : SpellScriptLoader("spell_gen_service_uniform") { }
    
        class spell_gen_service_uniform_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_service_uniform_AuraScript);
            
            enum eDatas
            {
                MorphMale = 31002,
                MorphFemale = 31003
            };
        
            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SERVICE_UNIFORM))
                    return false;
                return true;
            }
        
            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Player = GetTarget()->ToPlayer();
                
                if (l_Player == nullptr)
                    return;
                
                if (l_Player->getGender() == GENDER_MALE)
                    l_Player->SetDisplayId(eDatas::MorphMale);
                else
                    l_Player->SetDisplayId(eDatas::MorphFemale);
            }
        
            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Player = GetTarget()->ToPlayer();
                
                if (l_Player == nullptr)
                    return;
                
                l_Player->RestoreDisplayId();
            }
            
            void Register()
            {
                AfterEffectApply += AuraEffectRemoveFn(spell_gen_service_uniform_AuraScript::OnApply, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_gen_service_uniform_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
            }
    };
    
    AuraScript* GetAuraScript() const override
    {
        return new spell_gen_service_uniform_AuraScript();
    }
};

/// Last Update 6.2.3
/// Coin of Many Faces - 192225
class spell_gen_coin_of_many_faces : public SpellScriptLoader
{
    public:
        spell_gen_coin_of_many_faces() : SpellScriptLoader("spell_gen_coin_of_many_faces") { }

        class  spell_gen_coin_of_many_faces_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_coin_of_many_faces_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                CreatureTemplate const* l_CreatureTemplate = sObjectMgr->GetRandomTemplate(CreatureType::CREATURE_TYPE_HUMANOID);

                if (l_CreatureTemplate != nullptr)
                    l_Player->SetDisplayId(l_CreatureTemplate->Modelid1);
           }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                l_Player->RestoreDisplayId();
            }

            void Register()
            {
                AfterEffectApply += AuraEffectRemoveFn(spell_gen_coin_of_many_faces_AuraScript::OnApply, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_gen_coin_of_many_faces_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_coin_of_many_faces_AuraScript();
        }
};

/// Last Update 6.2.3
/// Called by Spirit of Chi-Ji (146199), Essence of Yu'lon (146197), Endurance of Niuzao (146193), Flurry of Xuen (146195)
class spell_legendary_cloaks : public SpellScriptLoader
{
public:
    spell_legendary_cloaks() : SpellScriptLoader("spell_legendary_cloaks") { }

    class spell_legendary_cloaks_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_legendary_cloaks_AuraScript);

        void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
        {
            Unit* l_Attacker = p_EventInfo.GetDamageInfo()->GetAttacker();
            if (l_Attacker == nullptr)
                return;

            /// Doesn't work on 100lvl in WOD
            if (l_Attacker->getLevel() >= 100)
                PreventDefaultAction();
        }

        void Register()
        {
            OnEffectProc += AuraEffectProcFn(spell_legendary_cloaks_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_legendary_cloaks_AuraScript();
    }
};

/// Last Update 6.2.3
/// Nullification Barrier - 115817
class spell_nullification_barrier : public SpellScriptLoader
{
    public:
        spell_nullification_barrier() : SpellScriptLoader("spell_nullification_barrier") { }

        class spell_nullification_barrier_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_nullification_barrier_SpellScript);

            SpellCastResult CheckMap()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->GetMapId() != 1008)
                        return SPELL_FAILED_INCORRECT_AREA;
                    else
                        return SPELL_CAST_OK;
                }
                else
                    return SPELL_FAILED_CASTER_DEAD;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_nullification_barrier_SpellScript::CheckMap);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_nullification_barrier_SpellScript();
        }
};


/// Last Update 6.2.3
/// Transmorphose - 162313
class spell_gen_transmorphose : public SpellScriptLoader
{
    public:
        spell_gen_transmorphose() : SpellScriptLoader("spell_gen_transmorphose") { }

        class  spell_gen_transmorphose_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_transmorphose_AuraScript);

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
                OnEffectApply += AuraEffectApplyFn(spell_gen_transmorphose_AuraScript::OnApply, EFFECT_0, SPELL_AURA_TRANSFORM, AuraEffectHandleModes(AURA_EFFECT_HANDLE_REAL | AURA_EFFECT_HANDLE_SEND_FOR_CLIENT));
                AfterEffectRemove += AuraEffectRemoveFn(spell_gen_transmorphose_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_TRANSFORM, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_transmorphose_AuraScript();
        }
};

/// Last Update 6.2.3
/// Pvp Trinket - 42292
class spell_gen_pvp_trinket : public SpellScriptLoader
{
    public:
        spell_gen_pvp_trinket() : SpellScriptLoader("spell_gen_pvp_trinket") { }

        class spell_gen_pvp_trinket_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_pvp_trinket_SpellScript);

            enum eSpells 
            {
                AllianceTinketVisual    = 97403,
                HordeTinketVisual       = 97404
            };

            void TriggerAnimation()
            {
                Player* caster = GetCaster()->ToPlayer();

                switch (caster->GetTeam())
                {
                case ALLIANCE:
                    caster->CastSpell(caster, eSpells::AllianceTinketVisual, TRIGGERED_FULL_MASK);
                    break;
                case HORDE:
                    caster->CastSpell(caster, eSpells::HordeTinketVisual, TRIGGERED_FULL_MASK);
                    break;
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_gen_pvp_trinket_SpellScript::TriggerAnimation);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_pvp_trinket_SpellScript();
        }
};

void AddSC_generic_spell_scripts()
{
    new spell_gen_pvp_trinket();
    new spell_gen_ironbeards_hat();
    new spell_gen_coin_of_many_faces();
    new spell_gen_jewel_of_hellfire();
    new spell_gen_jewel_of_hellfire_trigger();
    new spell_reconfigured_remote_shock();
    new spell_gen_demon_hunters_aspect();
    new spell_gen_wyrmhunter_hooks();
    new spell_gen_blood_elfe_illusion();
    new spell_gen_kilroggs_dead_eye();
    new spell_generic_iron_horde_pirate_costume();
    new spell_gen_mark_of_thunderlord();
    new spell_gen_inge_trigger_enchant();
    new spell_gen_potion_of_illusion();
    new spell_gen_alchemists_flask();
    new spell_gen_jards_peculiar_energy_source();
    new spell_gen_celestial_cloth_and_its_uses();
    new spell_gen_hardened_magnificient_hide_and_its_uses();
    new spell_gen_draenic_philosophers();
    new spell_gen_shadowmeld();
    new spell_gen_mark_of_warsong();
    new spell_gen_savage_fortitude();
    new spell_dru_touch_of_the_grave();
    new spell_gen_drums_of_fury();
    new spell_gen_absorb0_hitlimit1();
    new spell_gen_aura_of_anger();
    new spell_gen_av_drekthar_presence();
    new spell_gen_burn_brutallus();
    new spell_gen_cannibalize();
    new spell_gen_leeching_swarm();
    new spell_gen_parachute();
    new spell_gen_pet_summoned();
    new spell_gen_remove_flight_auras();
    new spell_creature_permanent_feign_death();
    new spell_pvp_trinket_wotf_shared_cd();
    new spell_gen_animal_blood();
    new spell_gen_divine_storm_cd_reset();
    new spell_gen_parachute_ic();
    new spell_gen_gunship_portal();
    new spell_gen_dungeon_credit();
    new spell_gen_profession_research();
    new spell_generic_clone();
    new spell_generic_clone_weapon();
    new spell_gen_clone_weapon_aura();
    new spell_gen_turkey_marker();
    new spell_gen_magic_rooster();
    new spell_gen_allow_cast_from_item_only();
    new spell_gen_launch();
    new spell_gen_vehicle_scaling();
    new spell_gen_oracle_wolvar_reputation();
    new spell_gen_luck_of_the_draw();
    new spell_gen_dummy_trigger();
    new spell_gen_spirit_healer_res();
    new spell_gen_gadgetzan_transporter_backfire();
    new spell_gen_gnomish_transporter();
    new spell_gen_dalaran_disguise("spell_gen_sunreaver_disguise");
    new spell_gen_dalaran_disguise("spell_gen_silver_covenant_disguise");
    new spell_gen_elune_candle();
    new spell_gen_break_shield("spell_gen_break_shield");
    new spell_gen_break_shield("spell_gen_tournament_counterattack");
    new spell_gen_mounted_charge();
    new spell_gen_defend();
    new spell_gen_tournament_duel();
    new spell_gen_summon_tournament_mount();
    new spell_gen_on_tournament_mount();
    new spell_gen_tournament_pennant();
    new spell_gen_chaos_blast();
    new spell_gen_wg_water();
    new spell_gen_despawn_self();
    new spell_gen_touch_the_nightmare();
    new spell_gen_dream_funnel();
    new spell_gen_bandage();
    new spell_gen_lifebloom("spell_hexlord_lifebloom", SPELL_HEXLORD_MALACRASS_LIFEBLOOM_FINAL_HEAL);
    new spell_gen_lifebloom("spell_tur_ragepaw_lifebloom", SPELL_TUR_RAGEPAW_LIFEBLOOM_FINAL_HEAL);
    new spell_gen_lifebloom("spell_cenarion_scout_lifebloom", SPELL_CENARION_SCOUT_LIFEBLOOM_FINAL_HEAL);
    new spell_gen_lifebloom("spell_twisted_visage_lifebloom", SPELL_TWISTED_VISAGE_LIFEBLOOM_FINAL_HEAL);
    new spell_gen_lifebloom("spell_faction_champion_dru_lifebloom", SPELL_FACTION_CHAMPIONS_DRU_LIFEBLOOM_FINAL_HEAL);
    new spell_gen_mount("spell_magic_broom", 0, 0, 0, SPELL_MAGIC_BROOM_150, SPELL_MAGIC_BROOM_280);
    new spell_gen_mount("spell_winged_steed_of_the_ebon_blade", 0, 0, 0, SPELL_WINGED_STEED_150, SPELL_WINGED_STEED_280);
    new spell_gen_mount("spell_blazing_hippogryph", 0, 0, 0, SPELL_BLAZING_HIPPOGRYPH_150, SPELL_BLAZING_HIPPOGRYPH_280);
    new spell_gen_upper_deck_create_foam_sword();
    new spell_gen_bonked();
    new spell_gen_gift_of_the_naaru();
    new spell_gen_running_wild();
    new spell_gen_two_forms();
    new spell_gen_darkflight();
    new spell_gen_blood_fury();
    new spell_gen_gobelin_gumbo();
    new spell_mage_polymorph_cast_visual();
    new spell_gen_hardened_shell();
    new spell_gen_ds_flush_knockback();
    new spell_gen_orb_of_power();
    new spell_vote_buff();
    new Resolve::spell_resolve_passive();
    new spell_gen_dampening();
    new spell_gen_selfie_camera();
    new spell_gen_selfie_lens_upgrade_kit();
    new spell_gen_carrying_seaforium();
    new spell_inherit_master_threat_list();
    new spell_taunt_flag_targeting();
    new spell_gen_raid_buff_stack();
    new spell_gen_sword_technique();
    //new spell_gen_check_faction(); -- temp disable
    new spell_gen_stoneform_dwarf_racial();
    new spell_gen_elixir_of_wandering_spirits();
    new spell_gen_service_uniform();
    new spell_legendary_cloaks();
    new spell_nullification_barrier();

    /// PlayerScript
    new PlayerScript_Touch_Of_Elune();
    new PlayerScript_gen_remove_rigor_mortis();
    new Resolve::PlayerScript_Resolve();
    new spell_gen_power_handler();
}
