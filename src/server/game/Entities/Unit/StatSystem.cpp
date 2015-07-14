/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "Unit.h"
#include "Player.h"
#include "Pet.h"
#include "Creature.h"
#include "ObjectMgr.h"
#include "SharedDefines.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"
#include "SpellMgr.h"
#include "World.h"

inline bool _ModifyUInt32(bool apply, uint32& baseValue, int32& amount)
{
    // If amount is negative, change sign and value of apply.
    if (amount < 0)
    {
        apply = !apply;
        amount = -amount;
    }
    if (apply)
        baseValue += amount;
    else
    {
        // Make sure we do not get uint32 overflow.
        if (amount > int32(baseValue))
            amount = baseValue;
        baseValue -= amount;
    }
    return apply;
}

/*#######################################
########                         ########
########   PLAYERS STAT SYSTEM   ########
########                         ########
#######################################*/

bool Player::UpdateStats(Stats stat)
{
    if (stat > STAT_SPIRIT)
        return false;

    // value = ((base_value * base_pct) + total_value) * total_pct
    float value  = GetTotalStatValue(stat);

    SetStat(stat, int32(value));

    if (stat == STAT_STAMINA || stat == STAT_INTELLECT || stat == STAT_STRENGTH)
    {
        Pet* pet = GetPet();
        if (pet)
            pet->UpdateStats(stat);
    }

    switch (stat)
    {
        case STAT_AGILITY:
            UpdateDodgePercentage();
            UpdateAllCritPercentages();
            break;
        case STAT_STAMINA:
            UpdateMaxHealth();
            break;
        case STAT_INTELLECT:
            UpdateMaxPower(POWER_MANA);
            UpdateAllSpellCritChances();
            UpdateArmor();                                  //SPELL_AURA_MOD_RESISTANCE_OF_INTELLECT_PERCENT, only armor currently
            break;
        case STAT_SPIRIT:
            break;
        case STAT_STRENGTH:
            if (getLevel() == MAX_LEVEL && (getClass() == CLASS_DEATH_KNIGHT || getClass() == CLASS_WARRIOR || getClass() == CLASS_PALADIN))
                UpdateParryPercentage();
            break;
        default:
            break;
    }

    if (stat == STAT_STRENGTH)
        UpdateAttackPowerAndDamage(false);
    else if (stat == STAT_AGILITY)
    {
        UpdateAttackPowerAndDamage(false);
        UpdateAttackPowerAndDamage(true);
    }

    UpdateSpellDamageAndHealingBonus();
    UpdateManaRegen();

    // Update ratings in exist SPELL_AURA_MOD_RATING_FROM_STAT and only depends from stat
    uint32 mask = 0;
    AuraEffectList const& modRatingFromStat = GetAuraEffectsByType(SPELL_AURA_MOD_RATING_FROM_STAT);
    for (AuraEffectList::const_iterator i = modRatingFromStat.begin(); i != modRatingFromStat.end(); ++i)
        if (Stats((*i)->GetMiscValueB()) == stat)
            mask |= (*i)->GetMiscValue();
    if (mask)
    {
        for (uint32 rating = 0; rating < MAX_COMBAT_RATING; ++rating)
            if (mask & (1 << rating))
                ApplyRatingMod(CombatRating(rating), 0, true);
    }
    return true;
}

void Player::ApplySpellPowerBonus(int32 amount, bool apply)
{
    apply = _ModifyUInt32(apply, m_baseSpellPower, amount);

    // For speed just update for client
    ApplyModUInt32Value(PLAYER_FIELD_MOD_HEALING_DONE_POS, amount, apply);
    for (int i = SPELL_SCHOOL_HOLY; i < MAX_SPELL_SCHOOL; ++i)
        ApplyModUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + i, amount, apply);

    if (Pet* l_Pet = GetPet())
        l_Pet->UpdateAttackPowerAndDamage();
}

void Player::UpdateSpellDamageAndHealingBonus()
{
    // Magic damage modifiers implemented in Unit::SpellDamageBonusDone
    // This information for client side use only
    // Get healing bonus for all schools
    SetStatInt32Value(PLAYER_FIELD_MOD_HEALING_DONE_POS, SpellBaseHealingBonusDone(SPELL_SCHOOL_MASK_ALL));
    // Get damage bonus for all schools
    for (int i = SPELL_SCHOOL_HOLY; i < MAX_SPELL_SCHOOL; ++i)
        SetStatInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS+i, SpellBaseDamageBonusDone(SpellSchoolMask(1 << i)));
}

bool Player::UpdateAllStats()
{
    for (int8 i = STAT_STRENGTH; i < MAX_STATS; ++i)
    {
        float value = GetTotalStatValue(Stats(i));
        SetStat(Stats(i), int32(value));
    }

    UpdateArmor();
    // calls UpdateAttackPowerAndDamage() in UpdateArmor for SPELL_AURA_MOD_ATTACK_POWER_OF_ARMOR
    UpdateAttackPowerAndDamage(true);
    UpdateMaxHealth();

    for (uint8 i = POWER_MANA; i < MAX_POWERS; ++i)
        UpdateMaxPower(Powers(i));

    // Custom MoP script
    // Jab Override Driver
    if (GetTypeId() == TYPEID_PLAYER && getClass() == CLASS_MONK)
    {
        Item* mainItem = GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

        if (mainItem && mainItem->GetTemplate()->Class == ITEM_CLASS_WEAPON && !HasAura(125660))
        {
            RemoveAura(108561); // 2H Staff Override
            RemoveAura(115697); // 2H Polearm Override
            RemoveAura(115689); // D/W Axes
            RemoveAura(115694); // D/W Maces
            RemoveAura(115696); // D/W Swords

            switch (mainItem->GetTemplate()->SubClass)
            {
                case ITEM_SUBCLASS_WEAPON_STAFF:
                    CastSpell(this, 108561, true);
                    break;
                case ITEM_SUBCLASS_WEAPON_POLEARM:
                    CastSpell(this, 115697, true);
                    break;
                case ITEM_SUBCLASS_WEAPON_AXE:
                    CastSpell(this, 115689, true);
                    break;
                case ITEM_SUBCLASS_WEAPON_MACE:
                    CastSpell(this, 115694, true);
                    break;
                case ITEM_SUBCLASS_WEAPON_SWORD:
                    CastSpell(this, 115696, true);
                    break;
                default:
                    break;
            }
        }
        else if (HasAura(125660))
        {
            RemoveAura(108561); // 2H Staff Override
            RemoveAura(115697); // 2H Polearm Override
            RemoveAura(115689); // D/W Axes
            RemoveAura(115694); // D/W Maces
            RemoveAura(115696); // D/W Swords
        }
    }
    // Way of the Monk - 120277
    if (GetTypeId() == TYPEID_PLAYER)
    {
        if (getClass() == CLASS_MONK && HasAura(120277))
        {
            RemoveAurasDueToSpell(120275);
            RemoveAurasDueToSpell(108977);

            uint32 trigger = 0;
            if (IsTwoHandUsed())
            {
                trigger = 120275;
            }
            else
            {
                Item* mainItem = GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
                Item* offItem = GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
                if (mainItem && mainItem->GetTemplate()->Class == ITEM_CLASS_WEAPON && offItem && offItem->GetTemplate()->Class == ITEM_CLASS_WEAPON)
                    trigger = 108977;
            }

            if (trigger)
                CastSpell(this, trigger, true);
        }
    }
    // Assassin's Resolve - 84601
    if (GetTypeId() == TYPEID_PLAYER)
    {
        if (getClass() == CLASS_ROGUE && ToPlayer()->GetSpecializationId(ToPlayer()->GetActiveSpec()) == SPEC_ROGUE_ASSASSINATION)
        {
            Item* mainItem = GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
            Item* offItem = GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

            if (((mainItem && mainItem->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_DAGGER) || (offItem && offItem->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_DAGGER)))
            {
                if (HasAura(84601))
                    RemoveAura(84601);

                CastSpell(this, 84601, true);
            }
            else
                RemoveAura(84601);
        }
    }

    UpdateAllRatings();
    UpdateAllCritPercentages();
    UpdateAllSpellCritChances();
    UpdateBlockPercentage();
    UpdateParryPercentage();
    UpdateDodgePercentage();
    UpdateSpellDamageAndHealingBonus();
    UpdateManaRegen();
    UpdateItemLevel();
    for (int i = SPELL_SCHOOL_NORMAL; i < MAX_SPELL_SCHOOL; ++i)
        UpdateResistances(i);

    return true;
}

void Player::UpdateResistances(uint32 school)
{
    if (school > SPELL_SCHOOL_NORMAL)
    {
        float value  = GetTotalAuraModValue(UnitMods(UNIT_MOD_RESISTANCE_START + school));
        SetResistance(SpellSchools(school), int32(value));

        Pet* pet = GetPet();
        if (pet)
            pet->UpdateResistances(school);
    }
    else
        UpdateArmor();
}

void Player::UpdateArmor()
{
    float l_Armor = 0.0f;
    UnitMods l_UnitMod = UNIT_MOD_ARMOR;

    l_Armor  = GetModifierValue(l_UnitMod, BASE_VALUE);         // base armor (from items)
    l_Armor *= GetModifierValue(l_UnitMod, BASE_PCT);           // armor percent from items
    l_Armor += GetModifierValue(l_UnitMod, TOTAL_VALUE);
    l_Armor *= GetModifierValue(l_UnitMod, TOTAL_PCT);

    if (GetRoleForGroup() == ROLE_TANK)
    {
        uint32 l_BonusArmor  = GetModifierValue(UNIT_MOD_BONUS_ARMOR, BASE_VALUE);  ///< base armor (from items)
        l_BonusArmor *= GetModifierValue(UNIT_MOD_BONUS_ARMOR, BASE_PCT);           ///< armor percent from items
        l_BonusArmor += GetModifierValue(UNIT_MOD_BONUS_ARMOR, TOTAL_VALUE);
        l_BonusArmor *= GetModifierValue(UNIT_MOD_BONUS_ARMOR, TOTAL_PCT);
        SetUInt32Value(UNIT_FIELD_MOD_BONUS_ARMOR, l_BonusArmor);

        l_Armor += l_BonusArmor;
    }

    //add dynamic flat mods
    AuraEffectList const& mResbyIntellect = GetAuraEffectsByType(SPELL_AURA_MOD_RESISTANCE_OF_STAT_PERCENT);
    for (AuraEffectList::const_iterator i = mResbyIntellect.begin(); i != mResbyIntellect.end(); ++i)
    {
        if ((*i)->GetMiscValue() & SPELL_SCHOOL_MASK_NORMAL)
            l_Armor += CalculatePct(GetStat(Stats((*i)->GetMiscValueB())), (*i)->GetAmount());
    }

    SetArmor(int32(l_Armor));

    if (Pet* l_Pet = GetPet())
        l_Pet->UpdateArmor();

    UpdateAttackPowerAndDamage();                           // armor dependent auras update for SPELL_AURA_MOD_ATTACK_POWER_OF_ARMOR
}

float Player::GetHealthBonusFromStamina()
{
    float ratio = 14.0f;
    GtOCTHpPerStaminaEntry const* hpBase = sGtOCTHpPerStaminaStore.LookupEntry(getLevel() - 1);
    if (hpBase)
        ratio = hpBase->ratio;

    // Taken from PaperDollFrame.lua 6.0 build 18888
    return GetStat(STAT_STAMINA) * ratio;
}

void Player::UpdateMaxHealth()
{
    UnitMods unitMod = UNIT_MOD_HEALTH;

    float value = GetModifierValue(unitMod, BASE_VALUE) + GetCreateHealth();
    value *= GetModifierValue(unitMod, BASE_PCT);
    value += GetModifierValue(unitMod, TOTAL_VALUE) + GetHealthBonusFromStamina();
    value *= GetModifierValue(unitMod, TOTAL_PCT);

    SetMaxHealth((uint32)value);
}

void Player::UpdateMaxPower(Powers p_Power)
{
    UnitMods l_UnitMod = UnitMods(UNIT_MOD_POWER_START + p_Power);

    float l_Value       = GetModifierValue(l_UnitMod, BASE_VALUE);
    float l_CreatePower = GetCreatePowers(p_Power);

    float l_Mod = 1.0f;

    if (p_Power == Powers::POWER_MANA)
        AddPct(l_Mod, GetTotalAuraModifier(AuraType::SPELL_AURA_MODIFY_MANA_POOL_PCT));

    l_CreatePower *= l_Mod;
    l_Value       += l_CreatePower;

    l_Value *= GetModifierValue(l_UnitMod, BASE_PCT);
    l_Value += GetModifierValue(l_UnitMod, TOTAL_VALUE);
    l_Value *= GetModifierValue(l_UnitMod, TOTAL_PCT);

    AuraEffectList const& mModMaxPower = GetAuraEffectsByType(SPELL_AURA_MOD_MAX_POWER);
    for (AuraEffectList::const_iterator i = mModMaxPower.begin(); i != mModMaxPower.end(); ++i)
        if (p_Power == (*i)->GetMiscValue())
            l_Value += float((*i)->GetAmount());

        l_Value = floor(l_Value + 0.5f);
    SetMaxPower(p_Power, uint32(l_Value));
}

void Player::UpdateItemLevel()
{
    SetFloatValue(PLAYER_FIELD_AVG_ITEM_LEVEL_EQUIPPED, (float)GetAverageItemLevelEquipped());
    SetFloatValue(PLAYER_FIELD_AVG_ITEM_LEVEL_TOTAL, (float)GetAverageItemLevelTotal());
}

void Player::UpdateAttackPowerAndDamage(bool ranged)
{
    float val2 = 0.0f;

    ChrClassesEntry const* entry = sChrClassesStore.LookupEntry(getClass());
    UnitMods unitMod = ranged ? UNIT_MOD_ATTACK_POWER_RANGED : UNIT_MOD_ATTACK_POWER;

    uint16 index = UNIT_FIELD_ATTACK_POWER;
    uint16 index_mod_pos = UNIT_FIELD_ATTACK_POWER_MOD_POS;
    uint16 index_mod_neg = UNIT_FIELD_ATTACK_POWER_MOD_NEG;
    uint16 index_mult = UNIT_FIELD_ATTACK_POWER_MULTIPLIER;

    if (ranged)
    {
        index = UNIT_FIELD_RANGED_ATTACK_POWER;
        index_mod_pos = UNIT_FIELD_RANGED_ATTACK_POWER_MOD_POS;
        index_mod_neg = UNIT_FIELD_RANGED_ATTACK_POWER_MOD_NEG;
        index_mult = UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER;
        val2 = GetStat(STAT_AGILITY) *  entry->RangedAttackPowerPerAgility;
    }
    else
    {
        float strengthValue = GetStat(STAT_STRENGTH) * entry->AttackPowerPerStrength;
        float agilityValue = GetStat(STAT_AGILITY) * entry->AttackPowerPerAgility;

        // Double bonus ??? Why so - THis should be here
        // if (GetShapeshiftForm() == FORM_CAT || GetShapeshiftForm() == FORM_BEAR)
        //     agilityValue += GetStat(STAT_AGILITY);

        val2 = strengthValue + agilityValue;
    }

    SetModifierValue(unitMod, BASE_VALUE, val2);

    float base_attPower = GetModifierValue(unitMod, BASE_VALUE) * GetModifierValue(unitMod, BASE_PCT);
    float attPowerMod = GetModifierValue(unitMod, TOTAL_VALUE);
    float attPowerMultiplier = GetModifierValue(unitMod, TOTAL_PCT) - 1.0f;

    //add dynamic flat mods
    if (!ranged && HasAuraType(SPELL_AURA_MOD_ATTACK_POWER_OF_ARMOR))
    {
        AuraEffectList const& mAPbyArmor = GetAuraEffectsByType(SPELL_AURA_MOD_ATTACK_POWER_OF_ARMOR);
        for (AuraEffectList::const_iterator iter = mAPbyArmor.begin(); iter != mAPbyArmor.end(); ++iter)
        {
            // always: ((*i)->GetModifier()->m_miscvalue == 1 == SPELL_SCHOOL_MASK_NORMAL)
            int32 temp = int32(GetArmor() / (*iter)->GetAmount());
            if (temp > 0)
                attPowerMod += temp;
            else
                attPowerMod -= temp;
        }
    }

    attPowerMod += CalculatePct(GetUInt32Value(UNIT_FIELD_MOD_BONUS_ARMOR), GetTotalAuraModifier(SPELL_AURA_MOD_AP_FROM_BONUS_ARMOR_PCT));

    if (HasAuraType(SPELL_AURA_OVERRIDE_AP_BY_SPELL_POWER_PCT))
    {
        int32 ApBySpellPct = 0;
        int32 spellPower = ToPlayer()->GetBaseSpellPowerBonus(); // SpellPower from Weapon
        spellPower += std::max(0, int32(ToPlayer()->GetStat(STAT_INTELLECT)) - 10); // SpellPower from intellect

        AuraEffectList const& mAPFromSpellPowerPct = GetAuraEffectsByType(SPELL_AURA_OVERRIDE_AP_BY_SPELL_POWER_PCT);
        for (AuraEffectList::const_iterator i = mAPFromSpellPowerPct.begin(); i != mAPFromSpellPowerPct.end(); ++i)
            ApBySpellPct += CalculatePct(spellPower, (*i)->GetAmount());

        if (ApBySpellPct > 0)
        {
            SetInt32Value(index, uint32(ApBySpellPct));     //UNIT_FIELD_(RANGED)_ATTACK_POWER field
            SetFloatValue(index_mult, attPowerMultiplier);  //UNIT_FIELD_(RANGED)_ATTACK_POWER_MULTIPLIER field
        }
        else
        {
            SetInt32Value(index, uint32(base_attPower + attPowerMod));  //UNIT_FIELD_(RANGED)_ATTACK_POWER field
            SetFloatValue(index_mult, attPowerMultiplier);              //UNIT_FIELD_(RANGED)_ATTACK_POWER_MULTIPLIER field
        }
    }
    else
    {
        SetInt32Value(index, uint32(base_attPower + attPowerMod));  //UNIT_FIELD_(RANGED)_ATTACK_POWER field
        SetFloatValue(index_mult, attPowerMultiplier);              //UNIT_FIELD_(RANGED)_ATTACK_POWER_MULTIPLIER field
    }

    Pet* pet = GetPet();                                //update pet's AP
    //automatically update weapon damage after attack power modification
    if (ranged)
    {
        UpdateDamagePhysical(WeaponAttackType::RangedAttack);
        if (pet && pet->isHunterPet()) // At ranged attack change for hunter pet
            pet->UpdateAttackPowerAndDamage();
    }
    else
    {
        UpdateDamagePhysical(WeaponAttackType::BaseAttack);
        if (CanDualWield() && haveOffhandWeapon())           //allow update offhand damage only if player knows DualWield Spec and has equipped offhand weapon
            UpdateDamagePhysical(WeaponAttackType::OffAttack);
        if (getClass() == CLASS_SHAMAN || getClass() == CLASS_PALADIN)                      // mental quickness
            UpdateSpellDamageAndHealingBonus();

        if (pet && pet->IsPetGhoul()) // At ranged attack change for hunter pet
            pet->UpdateAttackPowerAndDamage();
    }
}

void Player::CalculateMinMaxDamage(WeaponAttackType attType, bool normalized, bool addTotalPct, float& min_damage, float& max_damage, bool l_NoLongerDualWields)
{
    Item* mainItem = GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
    Item* l_OffHandItem = GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
    Item* l_UsedWeapon = attType == WeaponAttackType::OffAttack ? l_OffHandItem : mainItem;

    UnitMods unitMod;

    switch (attType)
    {
        case WeaponAttackType::BaseAttack:
        default:
            unitMod = UNIT_MOD_DAMAGE_MAINHAND;
            break;
        case WeaponAttackType::OffAttack:
            unitMod = UNIT_MOD_DAMAGE_OFFHAND;
            break;
        case WeaponAttackType::RangedAttack:
            unitMod = UNIT_MOD_DAMAGE_RANGED;
            break;
    }

    float att_speed = (l_UsedWeapon ? l_UsedWeapon->GetTemplate()->Delay : BASE_ATTACK_TIME) / 1000.f;
    float attackPower = GetTotalAttackPowerValue(attType);

    /// If player doesn't have weapons we should calculate damage with this values: min damage = 1 and max damage = 2
    float weapon_mindamage = GetWeaponDamageRange(attType, MINDAMAGE) == 0 ? 1.0f : GetWeaponDamageRange(attType, MINDAMAGE);
    float weapon_maxdamage = GetWeaponDamageRange(attType, MAXDAMAGE) == 0 ? 2.0f : GetWeaponDamageRange(attType, MAXDAMAGE);

    bool dualWield = mainItem && l_OffHandItem && !l_NoLongerDualWields;
    ///float dualWieldModifier = dualWield ? 0.81f : 1.0f; // Dual Wield Penalty: 19%
    float dualWieldModifier = 1.0f; ///< I don't know about reducing for 19%. We have checked it on PTR, and damage is the same with dual wield - like without it.
    if (dualWield && HasAuraType(SPELL_AURA_INCREASE_DUAL_WIELD_DAMAGE))
        dualWieldModifier += (float)GetTotalAuraModifier(SPELL_AURA_INCREASE_DUAL_WIELD_DAMAGE) / 100.f;

    float weapon_normalized_min = weapon_mindamage + attackPower / 3.5f * att_speed * dualWieldModifier;
    float weapon_normalized_max = weapon_maxdamage + attackPower / 3.5f * att_speed * dualWieldModifier;

    if (IsInFeralForm())
    {
        float weaponSpeed = BASE_ATTACK_TIME / 1000.0f;
        if (mainItem && mainItem->GetTemplate()->Class == ITEM_CLASS_WEAPON)
            weaponSpeed = float(mainItem->GetTemplate()->Delay) / 1000.0f;

        if (GetShapeshiftForm() == FORM_CAT)
        {
            /// Special cases for Cat form
            if (weapon_mindamage == 1)
                weapon_mindamage = 2;
            if (weapon_maxdamage == 2)
                weapon_maxdamage = 3;

            weapon_normalized_min = ((weapon_mindamage / weaponSpeed) + (attackPower / 3.5f));
            weapon_normalized_max = ((weapon_maxdamage / weaponSpeed) + (attackPower / 3.5f));
        }
        else if (GetShapeshiftForm() == FORM_BEAR)
        {
            weapon_normalized_min = ((weapon_mindamage / weaponSpeed) + (attackPower / 3.5f)) * 2.5f;
            weapon_normalized_max = ((weapon_maxdamage / weaponSpeed) + (attackPower / 3.5f)) * 2.5f;
        }
    }

    float base_value = GetModifierValue(unitMod, BASE_VALUE);
    float base_pct = GetModifierValue(unitMod, BASE_PCT);
    float total_value = GetModifierValue(unitMod, TOTAL_VALUE);
    float total_pct = addTotalPct ? GetModifierValue(unitMod, TOTAL_PCT) : 1.0f;

    /// Normalized Weapon Damage
    if (normalized)
    {
        CalculateNormalizedWeaponDamage(attType, min_damage, max_damage, attackPower, weapon_mindamage, weapon_maxdamage, l_UsedWeapon);
        min_damage = (min_damage * base_pct + total_value) * total_pct;
        max_damage = (max_damage * base_pct + total_value) * total_pct;
    }
    /// Damage based on auto-attack
    else
    {
        min_damage = ((base_value + weapon_normalized_min) * base_pct + total_value) * total_pct;
        max_damage = ((base_value + weapon_normalized_max) * base_pct + total_value) * total_pct;

        uint32 autoAttacksPctBonus = GetTotalAuraModifier(SPELL_AURA_MOD_AUTOATTACK_DAMAGE);
        AddPct(min_damage, autoAttacksPctBonus);
        AddPct(max_damage, autoAttacksPctBonus);
    }
}

void Player::CalculateNormalizedWeaponDamage(WeaponAttackType attType, float& min_damage, float& max_damage, float attackPower, float weapon_mindamage, float weapon_maxdamage, Item* l_UsedWeapon)
{
    /// Monks and Druids have their own damage calculation, they don't have normalized weapon damage spells
    if (getClass() == CLASS_MONK || getClass() == CLASS_DRUID)
        return;

    float l_NormalizedSpeedCoef = 1.0f;

    /// Speed coefficients from http://wowwiki.wikia.com/Normalization - tested on official server, information is correct
    if (l_UsedWeapon && l_UsedWeapon->GetTemplate())
    {
        if (l_UsedWeapon->GetTemplate()->IsOneHanded())
        {
            if (l_UsedWeapon->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_DAGGER)
                l_NormalizedSpeedCoef = 1.7f;
            else
                l_NormalizedSpeedCoef = 2.4f;
        }
        else if (l_UsedWeapon->GetTemplate()->IsTwoHandedWeapon())
            l_NormalizedSpeedCoef = 3.3f;
        else if (l_UsedWeapon->GetTemplate()->IsRangedWeapon())
            l_NormalizedSpeedCoef = 2.8f;
    }

    min_damage = weapon_mindamage + (attackPower / 3.5f * l_NormalizedSpeedCoef);
    max_damage = weapon_maxdamage + (attackPower / 3.5f * l_NormalizedSpeedCoef);
}

void Player::UpdateDamagePhysical(WeaponAttackType attType, bool l_NoLongerDualWields)
{
    float mindamage;
    float maxdamage;

    CalculateMinMaxDamage(attType, false, true, mindamage, maxdamage, l_NoLongerDualWields);

    switch (attType)
    {
        case WeaponAttackType::BaseAttack:
        default:
            SetStatFloatValue(UNIT_FIELD_MIN_DAMAGE, mindamage);
            SetStatFloatValue(UNIT_FIELD_MAX_DAMAGE, maxdamage);
            break;
        case WeaponAttackType::OffAttack:
            SetStatFloatValue(UNIT_FIELD_MIN_OFF_HAND_DAMAGE, mindamage);
            SetStatFloatValue(UNIT_FIELD_MAX_OFF_HAND_DAMAGE, maxdamage);
            break;
        case WeaponAttackType::RangedAttack:
            SetStatFloatValue(UNIT_FIELD_MIN_RANGED_DAMAGE, mindamage);
            SetStatFloatValue(UNIT_FIELD_MAX_RANGED_DAMAGE, maxdamage);
            break;
    }
}

void Player::UpdateCritPercentage(WeaponAttackType attType)
{
    BaseModGroup modGroup;
    uint16 index;
    CombatRating cr;

    switch (attType)
    {
        case WeaponAttackType::OffAttack:
            modGroup = OFFHAND_CRIT_PERCENTAGE;
            index = PLAYER_FIELD_OFFHAND_CRIT_PERCENTAGE;
            cr = CR_CRIT_MELEE;
            break;
        case WeaponAttackType::RangedAttack:
            modGroup = RANGED_CRIT_PERCENTAGE;
            index = PLAYER_FIELD_RANGED_CRIT_PERCENTAGE;
            cr = CR_CRIT_RANGED;
            break;
        case WeaponAttackType::BaseAttack:
        default:
            modGroup = CRIT_PERCENTAGE;
            index = PLAYER_FIELD_CRIT_PERCENTAGE;
            cr = CR_CRIT_MELEE;
            break;
    }

    float value = GetTotalPercentageModValue(modGroup) + GetRatingBonusValue(cr);
    // Modify crit from weapon skill and maximized defense skill of same level victim difference
    value += (int32(GetMaxSkillValueForLevel()) - int32(GetMaxSkillValueForLevel())) * 0.04f;
    if (sWorld->getBoolConfig(CONFIG_STATS_LIMITS_ENABLE))
        value = value > sWorld->getFloatConfig(CONFIG_STATS_LIMITS_CRIT) ? sWorld->getFloatConfig(CONFIG_STATS_LIMITS_CRIT) : value;

    value = value < 0.0f ? 0.0f : value;
    SetStatFloatValue(index, value);

    if (HasAuraType(AuraType::SPELL_AURA_CONVERT_CRIT_RATING_PCT_TO_PARRY_RATING))
        UpdateParryPercentage();
}

void Player::UpdateAllCritPercentages()
{
    uint8 l_Level  = getLevel();
    uint32 l_Class = getClass();

    if (l_Level > GT_MAX_LEVEL)
        l_Level = GT_MAX_LEVEL;

    float l_Crit = 0.0f;
    GtChanceToMeleeCritBaseEntry const* l_CritBase = sGtChanceToMeleeCritBaseStore.LookupEntry((l_Class - 1)*GT_MAX_LEVEL + l_Level - 1);
    if (l_CritBase)
        l_Crit = l_CritBase->base * 100;

    SetBaseModValue(CRIT_PERCENTAGE, PCT_MOD, l_Crit);
    SetBaseModValue(OFFHAND_CRIT_PERCENTAGE, PCT_MOD, l_Crit);
    SetBaseModValue(RANGED_CRIT_PERCENTAGE, PCT_MOD, l_Crit);

    UpdateCritPercentage(WeaponAttackType::BaseAttack);
    UpdateCritPercentage(WeaponAttackType::OffAttack);
    UpdateCritPercentage(WeaponAttackType::RangedAttack);
}

const float k_constant[MAX_CLASSES] =
{
    0.9560f,  // Warrior
    0.8860f,  // Paladin
    0.9880f,  // Hunter
    0.9880f,  // Rogue
    0.9830f,  // Priest
    0.9560f,  // DK
    0.9880f,  // Shaman
    0.9830f,  // Mage
    0.9830f,  // Warlock
    1.4220f,  // Monk
    1.2220f   // Druid
};

void Player::UpdateParryPercentage()
{
    const float parryCap[MAX_CLASSES] =
    {
        237.1860f,    // Warrior
        237.1860f,    // Paladin
        145.5604f,    // Hunter
        145.5604f,    // Rogue
        150.3759f,    // Priest
        237.1860f,    // DK
        145.5604f,    // Shaman
        150.3759f,    // Mage
        150.3759f,    // Warlock
        90.6425f,     // Monk
        150.3759f     // Druid
    };

    // No parry
    float value = 0.0f;
    uint32 pClass = getClass() - 1;

    if (CanParry())
    {
        float diminishing = 0.0f;
        float nondiminishing = 3.0f;

        // Parry from SPELL_AURA_MOD_PARRY_PERCENT aura
        nondiminishing += GetTotalAuraModifier(SPELL_AURA_MOD_PARRY_PERCENT);

        // Parry from rating
        diminishing = GetRatingBonusValue(CR_PARRY);

        /// Parry from strength, just for paladin/dk/warrior
        /*http://www.sacredduty.net/2014/08/06/tc401-avoidance-diminishing-returns-in-wod/
        1% parry before diminishing returns = 176.3760684 strength
        1 strength gives 1 / 176.3760684 = 0,0056697034301282*/
        if (getClass() == CLASS_PALADIN || getClass() == CLASS_DEATH_KNIGHT || getClass() == CLASS_WARRIOR)
            diminishing += GetTotalStatValue(STAT_STRENGTH, false) * 0.00566970f;

        // apply diminishing formula to diminishing parry chance
        value = nondiminishing + diminishing * parryCap[pClass] / (diminishing + (parryCap[pClass] * k_constant[pClass]));

        /// Apply parry from pct of critical strike from gear
        value += CalculatePct(GetRatingBonusValue(CR_CRIT_MELEE), GetTotalAuraModifier(SPELL_AURA_CONVERT_CRIT_RATING_PCT_TO_PARRY_RATING));

        if (value < 0.0f)
            value = 0.0f;

        if (sWorld->getBoolConfig(CONFIG_STATS_LIMITS_ENABLE))
            value = value > sWorld->getFloatConfig(CONFIG_STATS_LIMITS_PARRY) ? sWorld->getFloatConfig(CONFIG_STATS_LIMITS_PARRY) : value;
    }

    SetStatFloatValue(PLAYER_FIELD_PARRY_PERCENTAGE, value);
}

void Player::UpdateDodgePercentage()
{
    const float dodgeCap[MAX_CLASSES] =
    {
        90.6425f,     // Warrior
        66.5675f,     // Paladin
        145.5604f,    // Hunter
        145.5604f,    // Rogue
        150.3759f,    // Priest
        90.6425f,     // DK
        145.5604f,    // Shaman
        150.3759f,    // Mage
        150.3759f,    // Warlock
        501.2531f,    // Monk
        150.3759f     // Druid
    };

    float diminishing = 0.0f;
    float nondiminishing = 3.0f;

    // Dodge from SPELL_AURA_MOD_DODGE_PERCENT aura
    nondiminishing += GetTotalAuraModifier(SPELL_AURA_MOD_DODGE_PERCENT);

    /*
        3.36 + 1.25 (3.36 + 1.25 from 221 agi) on offi -- enhancement shaman -- ask sovak if you dont understand

        4.61 = 3.36 + (x * 145.5604 / (x + 145.5604 * 0.9880))
        1.25 = (145.5604x / (x + 143.8136752)
        1.25x + 179.767094 = 145.5604x
        144.3104x = 179.767094
        x = 1.245764057198927
        221 agi = 1.245764057198927
        1 agi = 0.0056369414352893
    */

    // Dodge from rating
    diminishing += GetRatingBonusValue(CR_DODGE);
    diminishing += GetTotalStatValue(STAT_AGILITY, false) * 0.0056369414352893f;

    uint32 pClass = getClass() - 1;
    // apply diminishing formula to diminishing dodge chance
    float value = nondiminishing + (diminishing * dodgeCap[pClass] / (diminishing + (dodgeCap[pClass] * k_constant[pClass])));
    if (value < 0.0f)
        value = 0.0f;

    if (sWorld->getBoolConfig(CONFIG_STATS_LIMITS_ENABLE))
        value = value > sWorld->getFloatConfig(CONFIG_STATS_LIMITS_DODGE) ? sWorld->getFloatConfig(CONFIG_STATS_LIMITS_DODGE) : value;

    SetStatFloatValue(PLAYER_FIELD_DODGE_PERCENTAGE, value);
}

void Player::UpdateBlockPercentage()
{
    const float blockCap[MAX_CLASSES] =
    {
        90.6425f,     // Warrior
        66.5675f,     // Paladin
        145.5604f,    // Hunter
        145.5604f,    // Rogue
        150.3759f,    // Priest
        90.6425f,     // DK
        145.5604f,    // Shaman
        150.3759f,    // Mage
        150.3759f,    // Warlock
        501.2531f,    // Monk
        150.3759f     // Druid
    };

    // No block
    float value = 0.0f;
    uint32 pClass = getClass() - 1;

    if (CanBlock())
    {
        float diminishing = 0.0f;
        float nondiminishing = 3.0f;

        // Dodge from SPELL_AURA_MOD_BLOCK_PERCENT aura
        nondiminishing += GetTotalAuraModifier(SPELL_AURA_MOD_BLOCK_PERCENT);

        // Dodge from rating
        diminishing += GetRatingBonusValue(CR_BLOCK);

        // apply diminishing formula to diminishing dodge chance
        value = nondiminishing + (diminishing * blockCap[pClass] / (diminishing + blockCap[pClass] * k_constant[pClass]));

        if (value < 0.0f)
            value = 0.0f;

        if (sWorld->getBoolConfig(CONFIG_STATS_LIMITS_ENABLE))
            value = value > sWorld->getFloatConfig(CONFIG_STATS_LIMITS_BLOCK) ? sWorld->getFloatConfig(CONFIG_STATS_LIMITS_BLOCK) : value;
    }

    SetStatFloatValue(PLAYER_FIELD_BLOCK_PERCENTAGE, value);
}

void Player::UpdateSpellCritChance(uint32 school)
{
    // For normal school set zero crit chance
    if (school == SPELL_SCHOOL_NORMAL)
    {
        SetFloatValue(PLAYER_FIELD_SPELL_CRIT_PERCENTAGE, 0.0f);
        return;
    }
    // For others recalculate it from:
    float crit = 0.0f;
    // Increase crit from SPELL_AURA_MOD_SPELL_CRIT_CHANCE
    crit += GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
    // Increase crit from SPELL_AURA_MOD_CRIT_PCT
    crit += GetTotalAuraModifier(SPELL_AURA_MOD_CRIT_PCT);
    // Increase crit by school from SPELL_AURA_MOD_SPELL_CRIT_CHANCE_SCHOOL
    crit += GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_SPELL_CRIT_CHANCE_SCHOOL, 1<<school);
    // Increase crit from spell crit ratings
    crit += GetRatingBonusValue(CR_CRIT_SPELL);

    // Store crit value
    SetFloatValue(PLAYER_FIELD_SPELL_CRIT_PERCENTAGE + school, crit);

    if (Pet* pet = GetPet())
        pet->m_baseSpellCritChance = crit;
}

void Player::UpdateMasteryPercentage()
{
    // No mastery
    float value = 0.0f;
    if (CanMastery() && getLevel() >= 80)
    {
        // Mastery from SPELL_AURA_MASTERY aura
        value += GetTotalAuraModifier(SPELL_AURA_MASTERY);
        // Mastery from rating
        value += GetRatingBonusValue(CR_MASTERY);
        value = value < 0.0f ? 0.0f : value;
    }
    SetFloatValue(PLAYER_FIELD_MASTERY, value);

    /// Update some mastery spells
    AuraApplicationMap& l_AppliedAuras = GetAppliedAuras();
    for (auto l_Iter : l_AppliedAuras)
    {
        SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(l_Iter.first);
        if (l_SpellInfo != nullptr && l_SpellInfo->HasAttribute(SpellAttr8::SPELL_ATTR8_MASTERY_SPECIALIZATION))
        {
            AuraPtr l_Aura = l_Iter.second->GetBase();
            for (uint8 l_I = 0; l_I < MAX_SPELL_EFFECTS; ++l_I)
            {
                if (AuraEffectPtr l_AurEff = l_Aura->GetEffect(l_I))
                {
                    l_AurEff->SetCanBeRecalculated(true);
                    if ((l_SpellInfo->Id == 77219 && !HasAura(103958) && l_I >= EFFECT_2) ///< EFFECT_2 and EFFECT_3 of Master Demonologist are only on Metamorphis Form
                        || l_SpellInfo->Id == 76856 ///< Mastery : Unshackled Fury
                        || l_SpellInfo->Id == 77492) ///< Mastery : Total Eclipse
                        l_AurEff->ChangeAmount(0, true, true);
                    else
                    {
                        l_AurEff->ChangeAmount((int32)(value * l_SpellInfo->Effects[l_I].BonusMultiplier), true, true);
                    }
                }
            }
        }
    }
}

void Player::UpdatePvPPowerPercentage()
{
    float value = GetRatingBonusValue(CR_PVP_POWER);
    value = value < 0.0f ? 0.0f : value;

    float damage_value = value;
    float heal_value = value;

    switch (GetSpecializationId(GetActiveSpec()))
    {
        // All other specializations and classes (including tanking) receive a 40% bonus to healing from PvP Power.
        case SPEC_WARRIOR_ARMS:
        case SPEC_WARRIOR_FURY:
        case SPEC_WARRIOR_PROTECTION:
        case SPEC_HUNTER_BEASTMASTERY:
        case SPEC_HUNTER_MARKSMANSHIP:
        case SPEC_HUNTER_SURVIVAL:
        case SPEC_ROGUE_ASSASSINATION:
        case SPEC_ROGUE_COMBAT:
        case SPEC_ROGUE_SUBTLETY:
        case SPEC_DK_BLOOD:
        case SPEC_DK_FROST:
        case SPEC_DK_UNHOLY:
        case SPEC_MAGE_ARCANE:
        case SPEC_MAGE_FIRE:
        case SPEC_MAGE_FROST:
        case SPEC_WARLOCK_AFFLICTION:
        case SPEC_WARLOCK_DEMONOLOGY:
        case SPEC_WARLOCK_DESTRUCTION:
            heal_value *= 0.4f;
            break;
        // Healing specializations receive a 100% bonus to healing from PvP Power.
        case SPEC_PALADIN_HOLY:
        case SPEC_PRIEST_DISCIPLINE:
        case SPEC_PRIEST_HOLY:
        case SPEC_SHAMAN_RESTORATION:
        case SPEC_DRUID_RESTORATION:
        case SPEC_MONK_MISTWEAVER:
            damage_value = 0.0f;
            break;
        // Damage specializations for Druids, Monks, Paladins, Priests, and Shaman receive a 70% bonus to healing from PvP Power.
        default:
            heal_value *= 0.7f;
            break;
    }

    SetFloatValue(PLAYER_FIELD_PVP_POWER_DAMAGE, damage_value);
    SetFloatValue(PLAYER_FIELD_PVP_POWER_HEALING, heal_value);
}

void Player::UpdateAllSpellCritChances()
{
    for (int i = SPELL_SCHOOL_NORMAL; i < MAX_SPELL_SCHOOL; ++i)
        UpdateSpellCritChance(i);
}

void Player::UpdateMultistrikePercentage()
{
    float value = GetTotalAuraModifier(SPELL_AURA_MOD_MULTISTRIKE_PCT);
    float effect = 30.f; // Default value
    value += GetRatingBonusValue(CR_MULTISTRIKE);
    effect += GetTotalAuraModifier(SPELL_AURA_MOD_MULTISTRIKE_EFFECT_PCT);
    SetFloatValue(PLAYER_FIELD_MULTISTRIKE, value);
    SetFloatValue(PLAYER_FIELD_MULTISTRIKE_EFFECT, effect / 100.f);
}

void Player::UpdateLeechPercentage()
{
    float value = GetTotalAuraModifier(SPELL_AURA_MOD_LEECH_PCT);
    value += GetRatingBonusValue(CR_LIFESTEAL);
    SetFloatValue(PLAYER_FIELD_LIFESTEAL, value);
}

void Player::UpdateVersatilityPercentage()
{
    float valueBonus = GetTotalAuraModifier(SPELL_AURA_MOD_VERSATILITY_PCT);

    float valueDone = GetRatingBonusValue(CR_VERSATILITY_DAMAGE_DONE);

    SetFloatValue(PLAYER_FIELD_VERSATILITY, valueDone);
    SetFloatValue(PLAYER_FIELD_VERSATILITY_BONUS, valueBonus);

    /// Update damage after applying versatility rating
    if (getClass() == CLASS_HUNTER)
        UpdateDamagePhysical(WeaponAttackType::RangedAttack);
    else
        UpdateDamagePhysical(WeaponAttackType::BaseAttack);
}

void Player::UpdateAvoidancePercentage()
{
    float value = 0.f;//GetTotalAuraModifier(SPELL_AURA);
    value += GetRatingBonusValue(CR_AVOIDANCE);
    SetFloatValue(PLAYER_FIELD_AVOIDANCE, value);
}

void Player::UpdateSpeedPercentage()
{
    float value = 0.f;
    value += GetRatingBonusValue(CR_SPEED);
    SetFloatValue(PLAYER_FIELD_SPEED, value);
}

void Player::ApplyManaRegenBonus(int32 amount, bool apply)
{
    _ModifyUInt32(apply, m_baseManaRegen, amount);
    UpdateManaRegen();
}

void Player::ApplyHealthRegenBonus(int32 amount, bool apply)
{
    _ModifyUInt32(apply, m_baseHealthRegen, amount);
}

void Player::UpdateManaRegen()
{
    if (getPowerType() != POWER_MANA && !IsInFeralForm())
        return;

    uint32 hp;
    uint32 mana = 0;
    float spiritRegen = OCTRegenMPPerSpirit();
    float outOfCombatRegen;
    float combatRegen;

    sObjectMgr->GetPlayerClassLevelInfo(getClass(), getLevel(), hp, mana);
    mana *= GetTotalAuraMultiplier(SPELL_AURA_MODIFY_MANA_REGEN_FROM_MANA_PCT);

    if (GetRoleForGroup() != ROLE_HEALER)
    {
        combatRegen = 0.004f * mana + (GetTotalAuraModifierByMiscValue(SPELL_AURA_MOD_POWER_REGEN, POWER_MANA) / 5.0f);
        outOfCombatRegen = 0.004f * mana + spiritRegen + (GetTotalAuraModifierByMiscValue(SPELL_AURA_MOD_POWER_REGEN, POWER_MANA) / 5.0f);
    }
    else
    {
        combatRegen = spiritRegen + (GetTotalAuraModifierByMiscValue(SPELL_AURA_MOD_POWER_REGEN, POWER_MANA) / 5.0f);
        outOfCombatRegen = 0.004f * mana + spiritRegen + (GetTotalAuraModifierByMiscValue(SPELL_AURA_MOD_POWER_REGEN, POWER_MANA) / 5.0f);
    }

    /// Warlocks mana regen 5% of maximum mana - blizzlike
    if (getClass() == CLASS_WARLOCK)
    {
        combatRegen *= 2.5f;
        outOfCombatRegen *= 2.5f;
    }

    if (HasAuraType(SPELL_AURA_MOD_MANA_REGEN_INTERRUPT))
        combatRegen *= 1.5f; // Allows 50% of your mana regeneration from Spirit to continue while in combat.

    outOfCombatRegen *= GetTotalAuraMultiplierByMiscValue(SPELL_AURA_MOD_POWER_REGEN_PERCENT, POWER_MANA);
    combatRegen *= GetTotalAuraMultiplierByMiscValue(SPELL_AURA_MOD_POWER_REGEN_PERCENT, POWER_MANA);

    if (HasAuraType(AuraType::SPELL_AURA_MOD_MANA_REGEN_BY_HASTE))
    {
        float l_HastePct = 1.0f + GetUInt32Value(PLAYER_FIELD_COMBAT_RATINGS + CR_HASTE_MELEE) * GetRatingMultiplier(CR_HASTE_MELEE) / 100.0f;
        outOfCombatRegen *= l_HastePct;
        combatRegen *= l_HastePct;
    }

    SetStatFloatValue(UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER, combatRegen);
    SetStatFloatValue(UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER, outOfCombatRegen);
}

void Player::UpdateEnergyRegen()
{
    if (getPowerType() != Powers::POWER_ENERGY)
        return;

    uint32 l_PowerIndex = GetPowerIndexByClass(Powers::POWER_ENERGY, getClass());

    float l_RegenFlatMultiplier = 1.0f;
    Unit::AuraEffectList const& l_RegenAura = GetAuraEffectsByType(SPELL_AURA_MOD_POWER_REGEN_PERCENT);
    for (auto l_AuraEffect : l_RegenAura)
    {
        if (l_AuraEffect->GetMiscValue() != Powers::POWER_ENERGY)
            continue;

        l_RegenFlatMultiplier += l_AuraEffect->GetAmount() / 100.0f;
    }
    SetFloatValue(EUnitFields::UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER + l_PowerIndex, GetRegenForPower(Powers::POWER_ENERGY) - (GetRegenForPower(Powers::POWER_ENERGY) / l_RegenFlatMultiplier));
}

void Player::UpdateFocusRegen()
{
    if (getPowerType() != Powers::POWER_FOCUS)
        return;

    SetFloatValue(EUnitFields::UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER, GetRegenForPower(Powers::POWER_FOCUS));
}

void Player::UpdateRuneRegen(RuneType rune)
{
    if (rune > NUM_RUNE_TYPES)
        return;

    uint32 cooldown = 0;
    float HastePct = 2.0f - GetFloatValue(UNIT_FIELD_MOD_HASTE_REGEN);

    for (uint32 i = 0; i < MAX_RUNES; ++i)
        if (GetBaseRune(i) == rune)
        {
            cooldown = GetRuneBaseCooldown(i);
            break;
        }

    if (cooldown <= 0)
        return;

    float regen = float(1 * IN_MILLISECONDS) / float(cooldown);
    regen *= HastePct;
    SetFloatValue(PLAYER_FIELD_RUNE_REGEN + uint8(rune), regen);
}

void Player::UpdateAllRunesRegen()
{
    if (getClass() != Classes::CLASS_DEATH_KNIGHT)
        return;

    float HastePct = 2.0f - GetFloatValue(UNIT_FIELD_MOD_HASTE_REGEN);

    for (uint8 i = 0; i < NUM_RUNE_TYPES; ++i)
    {
        if (uint32 cooldown = GetRuneTypeBaseCooldown(RuneType(i)))
        {
            float regen = float(1 * IN_MILLISECONDS) / float(cooldown);

            if (regen < 0.0099999998f)
                regen = 0.01f;

            regen *= HastePct;
            SetFloatValue(PLAYER_FIELD_RUNE_REGEN + i, regen);
        }
    }

    SetFloatValue(EUnitFields::UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER, GetRegenForPower(Powers::POWER_RUNES));
}

float Player::GetRegenForPower(Powers p_Power)
{
    float l_BaseRegen = 0.0f;

    switch (p_Power)
    {
        case Powers::POWER_FOCUS:
            l_BaseRegen = 4.0f;
            break;
        case Powers::POWER_ENERGY:
        case Powers::POWER_RUNES:
            l_BaseRegen = 10.0f;
            break;
        default:
            return 0.0f;
    }

    float l_Pct = 1.0f;
    Unit::AuraEffectList const& l_ModPowerRegenPCT = GetAuraEffectsByType(AuraType::SPELL_AURA_MOD_POWER_REGEN_PERCENT);
    for (Unit::AuraEffectList::const_iterator l_Iter = l_ModPowerRegenPCT.begin(); l_Iter != l_ModPowerRegenPCT.end(); ++l_Iter)
    {
        if (Powers((*l_Iter)->GetMiscValue()) == p_Power)
            l_Pct += (*l_Iter)->GetAmount() / 100.0f;
    }

    float l_HastePct = 1.0f;
    float l_Total = 1.0f;

    /// I don't know how energy/runes regen should be calculated, but for focus it should be this way
    if (p_Power == Powers::POWER_FOCUS)
    {
        l_HastePct = 1.f / GetFloatValue(UNIT_FIELD_MOD_HASTE);
        /// TODO
        /// Not finished fix, i don't know how to fix it atm.
        /// UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER by default for hunter is 5 * haste pct. For example, if we have 10% haste: 5 * 1.1 = 5.5, but on WOD base regen for hunters is 4.
        /// So we should have 4 * 1.1 = 4.4 regen.
        /// If we just calculate correct regen here, we receive: 5 * haste pct + 4 * haste pct.
        /// I've tried to remove 5 * haste_pct from this field here, but it's not correct yet.
        /// If someone has some ideas how to make it exactly 4 * haste pct - help.
        l_Total = ((l_BaseRegen * l_HastePct) - (5.f * l_HastePct)) * l_Pct;
    }
    else
    {
        l_HastePct = 1.f / (1.f + (m_baseRatingValue[CR_HASTE_MELEE] * GetRatingMultiplier(CR_HASTE_MELEE)) / 100.f);
        l_Total = l_BaseRegen * l_HastePct * l_Pct;
    }

    return l_Total;
}

void Player::_ApplyAllStatBonuses()
{
    SetCanModifyStats(false);

    _ApplyAllAuraStatMods();
    _ApplyAllItemMods();

    SetCanModifyStats(true);

    UpdateAllStats();
}

void Player::_RemoveAllStatBonuses()
{
    SetCanModifyStats(false);

    _RemoveAllItemMods();
    _RemoveAllAuraStatMods();

    SetCanModifyStats(true);

    UpdateAllStats();
}

/*#######################################
########                         ########
########    MOBS STAT SYSTEM     ########
########                         ########
#######################################*/

bool Creature::UpdateStats(Stats /*stat*/)
{
    return true;
}

bool Creature::UpdateAllStats()
{
    UpdateMaxHealth();
    UpdateAttackPowerAndDamage();
    UpdateAttackPowerAndDamage(true);

    for (uint8 i = POWER_MANA; i < MAX_POWERS; ++i)
        UpdateMaxPower(Powers(i));

    for (int8 i = SPELL_SCHOOL_NORMAL; i < MAX_SPELL_SCHOOL; ++i)
        UpdateResistances(i);

    return true;
}

void Creature::UpdateResistances(uint32 school)
{
    if (school > SPELL_SCHOOL_NORMAL)
    {
        float value  = GetTotalAuraModValue(UnitMods(UNIT_MOD_RESISTANCE_START + school));
        SetResistance(SpellSchools(school), int32(value));
    }
    else
        UpdateArmor();
}

void Creature::UpdateArmor()
{
    float value = GetTotalAuraModValue(UNIT_MOD_ARMOR);
    SetArmor(int32(value));
}

void Creature::UpdateMaxHealth()
{
    float value = GetTotalAuraModValue(UNIT_MOD_HEALTH);
    SetMaxHealth((uint32)value);
}

void Creature::UpdateMaxPower(Powers power)
{
    UnitMods unitMod = UnitMods(UNIT_MOD_POWER_START + power);

    float value  = GetTotalAuraModValue(unitMod);
    SetMaxPower(power, uint32(value));
}

void Creature::UpdateAttackPowerAndDamage(bool ranged)
{
    UnitMods unitMod = ranged ? UNIT_MOD_ATTACK_POWER_RANGED : UNIT_MOD_ATTACK_POWER;

    uint16 index = UNIT_FIELD_ATTACK_POWER;
    uint16 index_mult = UNIT_FIELD_ATTACK_POWER_MULTIPLIER;

    if (ranged)
    {
        index = UNIT_FIELD_RANGED_ATTACK_POWER;
        index_mult = UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER;
    }

    float base_attPower  = GetModifierValue(unitMod, BASE_VALUE) * GetModifierValue(unitMod, BASE_PCT);
    float attPowerMultiplier = GetModifierValue(unitMod, TOTAL_PCT) - 1.0f;

    SetInt32Value(index, (uint32)base_attPower);            //UNIT_FIELD_(RANGED)_ATTACK_POWER field
    SetFloatValue(index_mult, attPowerMultiplier);          //UNIT_FIELD_(RANGED)_ATTACK_POWER_MULTIPLIER field

    //automatically update weapon damage after attack power modification
    if (ranged)
        UpdateDamagePhysical(WeaponAttackType::RangedAttack);
    else
    {
        UpdateDamagePhysical(WeaponAttackType::BaseAttack);
        UpdateDamagePhysical(WeaponAttackType::OffAttack);
    }
}

void Creature::UpdateDamagePhysical(WeaponAttackType p_AttType, bool l_NoLongerDualWields)
{
    float l_Variance = 1.f;
    UnitMods l_UnitMod;
    switch (p_AttType)
    {
        case WeaponAttackType::BaseAttack:
        default:
            l_Variance = GetCreatureTemplate()->baseVariance;
            l_UnitMod = UNIT_MOD_DAMAGE_MAINHAND;
            break;
        case WeaponAttackType::OffAttack:
            l_Variance = GetCreatureTemplate()->baseVariance;
            l_UnitMod = UNIT_MOD_DAMAGE_OFFHAND;
            break;
        case WeaponAttackType::RangedAttack:
            l_Variance = GetCreatureTemplate()->rangeVariance;
            l_UnitMod = UNIT_MOD_DAMAGE_RANGED;
            break;
    }

    float l_WeaponMinDamage = GetWeaponDamageRange(p_AttType, MINDAMAGE);
    float l_WeaponMaxDamage = GetWeaponDamageRange(p_AttType, MAXDAMAGE);

    /* difference in AP between current attack power and base value from DB */
    // creature's damage in battleground is calculated in Creature::SelectLevel
    // so don't change it with ap and dmg multipliers

    float l_AttackPower      = GetTotalAttackPowerValue(p_AttType);
    float l_AttackSpeedMulti = GetAPMultiplier(p_AttType, false);
    float l_BaseValue        = GetModifierValue(l_UnitMod, BASE_VALUE) + (l_AttackPower / 14.0f) * l_Variance;
    float l_BasePct          = GetModifierValue(l_UnitMod, BASE_PCT) * l_AttackSpeedMulti;
    float l_TotalValue       = GetModifierValue(l_UnitMod, TOTAL_VALUE);
    float l_TotalPct         = GetModifierValue(l_UnitMod, TOTAL_PCT);
    float l_DmgMultiplier    = GetCreatureTemplate()->dmg_multiplier;

    if (!CanUseAttackType(p_AttType))
    {
        l_WeaponMinDamage = 0;
        l_WeaponMaxDamage = 0;
    }

    float mindamage = ((l_BaseValue + l_WeaponMinDamage) * l_DmgMultiplier * l_BasePct + l_TotalValue) * l_TotalPct;
    float maxdamage = ((l_BaseValue + l_WeaponMaxDamage) * l_DmgMultiplier * l_BasePct + l_TotalValue) * l_TotalPct;

    switch (p_AttType)
    {
        case WeaponAttackType::BaseAttack:
        default:
            SetStatFloatValue(UNIT_FIELD_MIN_DAMAGE, mindamage);
            SetStatFloatValue(UNIT_FIELD_MAX_DAMAGE, maxdamage);
            break;
        case WeaponAttackType::OffAttack:
            SetStatFloatValue(UNIT_FIELD_MIN_OFF_HAND_DAMAGE, mindamage);
            SetStatFloatValue(UNIT_FIELD_MAX_OFF_HAND_DAMAGE, maxdamage);
            break;
        case WeaponAttackType::RangedAttack:
            SetStatFloatValue(UNIT_FIELD_MIN_RANGED_DAMAGE, mindamage);
            SetStatFloatValue(UNIT_FIELD_MAX_RANGED_DAMAGE, maxdamage);
            break;
    }
}

/*#######################################
########                         ########
########    PETS STAT SYSTEM     ########
########                         ########
#######################################*/

// @TODO: Update for WoD
bool Guardian::UpdateStats(Stats p_Stat)
{
    if (p_Stat >= MAX_STATS)
        return false;

    float l_Value  = GetTotalStatValue(p_Stat);
    ApplyStatBuffMod(p_Stat, m_statFromOwner[p_Stat], false);
    float l_OwnersBonus = 0.0f;

    Unit* l_Owner = GetOwner();

    float l_Mod = 0.75f;

    switch (p_Stat)
    {
        case STAT_STAMINA:
        {
            l_Mod = 0.3f;

            if (IsPetGhoul() || IsPetGargoyle())
                l_Mod = 0.45f;
            else if (l_Owner->getClass() == CLASS_WARLOCK && isPet())
                l_Mod = 0.75f;
            else if (l_Owner->getClass() == CLASS_MAGE && isPet())
                l_Mod = 0.75f;
            else
            {
                l_Mod = 0.45f;

                if (isPet())
                {
                    switch (ToPet()->GetSpecializationId())
                    {
                        case SPEC_PET_FEROCITY: l_Mod = 0.67f; break;
                        case SPEC_PET_TENACITY: l_Mod = 0.78f; break;
                        case SPEC_PET_CUNNING:  l_Mod = 0.725f; break;
                    }
                }
            }

            l_OwnersBonus = float(l_Owner->GetStat(p_Stat)) * l_Mod;
            l_OwnersBonus *= GetModifierValue(UNIT_MOD_STAT_STAMINA, TOTAL_PCT);

            float l_Modifier = 1.0f;
            AuraEffectList const& mModHealthFromOwner = l_Owner->GetAuraEffectsByType(SPELL_AURA_INCREASE_HEALTH_FROM_OWNER);
            for (AuraEffectList::const_iterator l_Iterator = mModHealthFromOwner.begin(); l_Iterator != mModHealthFromOwner.end(); ++l_Iterator)
                l_Modifier += float((*l_Iterator)->GetAmount() / 100.0f);

            l_OwnersBonus *= l_Modifier;
            l_Value += l_OwnersBonus;
            break;
        }
        case STAT_STRENGTH:
        {
            l_Mod = 0.7f;

            l_OwnersBonus = l_Owner->GetStat(p_Stat) * l_Mod;
            l_Value += l_OwnersBonus;
            break;
        }
        case STAT_INTELLECT:
        {
            if (l_Owner->getClass() == CLASS_WARLOCK || l_Owner->getClass() == CLASS_MAGE)
            {
                l_Mod = 0.3f;
                l_OwnersBonus = l_Owner->GetStat(p_Stat) * l_Mod;
            }
            else if (l_Owner->getClass() == CLASS_DEATH_KNIGHT && GetEntry() == 31216)
            {
                l_Mod = 0.3f;
                if (l_Owner->GetSimulacrumTarget())
                    l_OwnersBonus = l_Owner->GetSimulacrumTarget()->GetStat(p_Stat) * l_Mod;
                else
                    l_OwnersBonus = l_Owner->GetStat(p_Stat) * l_Mod;
            }

            l_Value += l_OwnersBonus;
            break;
        }
        default:
            break;
    }

    SetStat(p_Stat, int32(l_Value));
    m_statFromOwner[p_Stat] = l_OwnersBonus;
    ApplyStatBuffMod(p_Stat, m_statFromOwner[p_Stat], true);

    switch (p_Stat)
    {
        case STAT_STRENGTH:
            UpdateAttackPowerAndDamage();
            break;
        case STAT_AGILITY:
            UpdateArmor();
            break;
        case STAT_STAMINA:
            UpdateMaxHealth();
            break;
        case STAT_INTELLECT:
            UpdateMaxPower(POWER_MANA);
            if (l_Owner->getClass() == CLASS_MAGE || l_Owner->getClass() == CLASS_WARLOCK)
                UpdateAttackPowerAndDamage();
            break;
        case STAT_SPIRIT:
        default:
            break;
    }

    return true;
}

// Wod updated
bool Guardian::UpdateAllStats()
{
    for (uint8 l_I = STAT_STRENGTH; l_I < MAX_STATS; ++l_I)
        UpdateStats(Stats(l_I));

    for (uint8 l_I = POWER_MANA; l_I < MAX_POWERS; ++l_I)
        UpdateMaxPower(Powers(l_I));

    for (uint8 l_I = SPELL_SCHOOL_NORMAL; l_I < MAX_SPELL_SCHOOL; ++l_I)
        UpdateResistances(l_I);

    return true;
}

// WoD updated
void Guardian::UpdateResistances(uint32 p_School)
{
    if (p_School == SPELL_SCHOOL_NORMAL)
    {
        UpdateArmor();
        return;
    }

    float l_Value = GetTotalAuraModValue(UnitMods(UNIT_MOD_RESISTANCE_START + p_School));

    // @TODO: This is still true since WoD ? Look like resistance to specific magic school doesn't exist anymore anyway ...
    // hunter and warlock pets gain 40% of owner's resistance
    //if (isPet())
    //    l_Value += float(CalculatePct(m_owner->GetResistance(SpellSchools(p_School)), 40));

    SetResistance(SpellSchools(p_School), int32(l_Value));
}

// WoD updated
void Guardian::UpdateArmor()
{
    UnitMods l_InitMod = UNIT_MOD_ARMOR;
    PetStatInfo const* l_PetStat = GetPetStat();

    /// - Since 6.x, armor of pet are always percentage of owner armor (default 1)
    float l_Value = m_owner->GetArmor() * (l_PetStat != nullptr ? l_PetStat->m_ArmorCoef : 1.0f);

    /// - Apply mods
    {
        l_Value *= GetModifierValue(l_InitMod, BASE_PCT);
        l_Value *= GetModifierValue(l_InitMod, TOTAL_PCT);

        float l_Amount = 0;
        AuraEffectList const& l_ModPetStats = m_owner->GetAuraEffectsByType(SPELL_AURA_MOD_PET_STATS);
        for (AuraEffectList::const_iterator l_Iterator = l_ModPetStats.begin(); l_Iterator != l_ModPetStats.end(); ++l_Iterator)
        {
            if ((*l_Iterator)->GetMiscValue() == INCREASE_ARMOR_PERCENT && (*l_Iterator)->GetMiscValueB() && (int32)GetEntry() == (*l_Iterator)->GetMiscValueB())
                l_Amount += float((*l_Iterator)->GetAmount());
        }

        AddPct(l_Value, l_Amount);
    }

    SetArmor(int32(std::round(l_Value)));
}

// WoD updated
void Guardian::UpdateMaxHealth()
{
    UnitMods l_UnitMod = UNIT_MOD_HEALTH;
    PetStatInfo const* l_PetStat = GetPetStat();

    float l_Value = (l_PetStat != nullptr) ? m_owner->GetMaxHealth() * GetPetStat()->m_HealthCoef : GetModifierValue(l_UnitMod, BASE_VALUE) + GetCreateHealth();

    l_Value *= GetModifierValue(l_UnitMod, BASE_PCT);
    l_Value += GetModifierValue(l_UnitMod, TOTAL_VALUE);
    l_Value *= GetModifierValue(l_UnitMod, TOTAL_PCT);

    float l_Amount = 0;
    AuraEffectList const& l_ModPetStats = m_owner->GetAuraEffectsByType(SPELL_AURA_MOD_PET_STATS);
    for (AuraEffectList::const_iterator l_Iterator = l_ModPetStats.begin(); l_Iterator != l_ModPetStats.end(); ++l_Iterator)
    {
        if ((*l_Iterator)->GetMiscValue() == INCREASE_HEALTH_PERCENT && (*l_Iterator)->GetMiscValueB() && (int32)GetEntry() == (*l_Iterator)->GetMiscValueB())
            l_Amount += float((*l_Iterator)->GetAmount());
    }

    AddPct(l_Value, l_Amount);
    SetMaxHealth((uint32)l_Value);
}

// WoD updated
void Guardian::UpdateMaxPower(Powers p_Power)
{
    UnitMods l_UnitMod = UnitMods(UNIT_MOD_POWER_START + p_Power);
    PetStatInfo const* l_PetStat = GetPetStat();

    float l_AddValue = ((l_PetStat != nullptr) && p_Power == l_PetStat->m_Power) ? 0.0f : GetStat(STAT_INTELLECT) - GetCreateStat(STAT_INTELLECT);
    float l_Multiplicator = 15.0f;

    float l_Value = GetModifierValue(l_UnitMod, BASE_VALUE) + GetCreatePowers(p_Power);
    if (l_PetStat != nullptr)
    {
        if (l_PetStat->m_CreatePower == -1)
            l_Value = GetCreatePowers(l_PetStat->m_Power);
        else
        {
            // Negative number, it's fix value
            if (l_PetStat->m_CreatePower < 0.0f)
                l_Value = l_PetStat->m_CreatePower * -1;
            // Positive number, it's percentage of owner power
            else
                l_Value = float(m_owner->GetMaxPower(m_owner->getPowerType()) * l_PetStat->m_CreatePower);
        }
    }

    l_Value *= GetModifierValue(l_UnitMod, BASE_PCT);
    l_Value += GetModifierValue(l_UnitMod, TOTAL_VALUE) + l_AddValue * l_Multiplicator;
    l_Value *= GetModifierValue(l_UnitMod, TOTAL_PCT);

    SetMaxPower(p_Power, uint32(l_Value));
}

// WoD updated
void Guardian::UpdateAttackPowerAndDamage(bool p_Ranged)
{
    if (p_Ranged)
        return;

    UnitMods l_UnitMod = UNIT_MOD_ATTACK_POWER;
    float l_BaseValue  = std::max(0.0f, 2 * GetStat(STAT_STRENGTH) - 20.0f);

    float l_BaseAttackPower       = l_BaseValue;
    float l_SpellPower            = l_BaseValue;
    float l_AttackPowerMultiplier = 1.f;

    PetStatInfo const* l_PetStat = GetPetStat();
    if (l_PetStat != nullptr)
    {
        switch (l_PetStat->m_PowerStat)
        {
            case PetStatInfo::PowerStatBase::AttackPower:
                l_BaseValue       = m_owner->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack) * l_PetStat->m_APSPCoef;
                l_BaseAttackPower = l_BaseValue;
                l_SpellPower      = l_BaseValue * l_PetStat->m_SecondaryStatCoef;
                break;
            case PetStatInfo::PowerStatBase::SpellPower:
                l_BaseValue       = m_owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SPELL) * l_PetStat->m_APSPCoef;
                l_SpellPower      = l_BaseValue;
                l_BaseAttackPower = l_BaseValue * l_PetStat->m_SecondaryStatCoef;
                break;
            default:
                break;
        }
    }

    SetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_VALUE, l_BaseAttackPower);

    if (m_owner->GetTypeId() == TYPEID_PLAYER)
        m_owner->SetUInt32Value(PLAYER_FIELD_PET_SPELL_POWER, l_SpellPower);

    l_BaseAttackPower      *= GetModifierValue(l_UnitMod, BASE_PCT);
    l_AttackPowerMultiplier = GetModifierValue(l_UnitMod, TOTAL_PCT) - 1.0f;

    /// - UNIT_FIELD_(RANGED)_ATTACK_POWER field
    SetInt32Value(UNIT_FIELD_ATTACK_POWER,        (int32)std::round(l_BaseAttackPower));
    SetInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER, (int32)std::round(l_BaseAttackPower));

    /// - UNIT_FIELD_(RANGED)_ATTACK_POWER_MULTIPLIER field
    SetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER,        l_AttackPowerMultiplier);
    SetFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER, l_AttackPowerMultiplier);

    /// - Automatically update weapon damage after attack power modification
    UpdateDamagePhysical(WeaponAttackType::BaseAttack);
}

// WoD updated
void Guardian::UpdateDamagePhysical(WeaponAttackType p_AttType, bool l_NoLongerDualWields)
{
    if (p_AttType > WeaponAttackType::BaseAttack)
        return;

    UnitMods l_UnitMod;

    switch (p_AttType)
    {
        case WeaponAttackType::BaseAttack:
        default:
            l_UnitMod = UNIT_MOD_DAMAGE_MAINHAND;
            break;
        case WeaponAttackType::OffAttack:
            l_UnitMod = UNIT_MOD_DAMAGE_OFFHAND;
            break;
        case WeaponAttackType::RangedAttack:
            l_UnitMod = UNIT_MOD_DAMAGE_RANGED;
            break;
    }

    /// For hunter pets damage calculation we don't need take their attack speed time, it's always 2.0f
    float l_AttackSpeed = isHunterPet() ? 2.0f : float(GetAttackTime(WeaponAttackType::BaseAttack)) / 1000.0f;
    float l_BaseValue  = GetModifierValue(l_UnitMod, BASE_VALUE);

    PetStatInfo const* l_PetStat = GetPetStat();
    if (l_PetStat != nullptr)
        l_BaseValue += GetTotalAttackPowerValue(p_AttType) * l_PetStat->m_DamageCoef;
    else
        l_BaseValue += GetTotalAttackPowerValue(p_AttType) / 14.0f * l_AttackSpeed;

    /// Special calculation for hunter pets - WoD
    /// Last Update 6.1.2 19802
    if (isHunterPet())
        l_BaseValue = GetTotalAttackPowerValue(p_AttType) / 3.5f * l_AttackSpeed;

    float l_BasePct    = GetModifierValue(l_UnitMod, BASE_PCT);
    float l_TotalValue = GetModifierValue(l_UnitMod, TOTAL_VALUE);
    float l_TotalPct   = GetModifierValue(l_UnitMod, TOTAL_PCT);

    float l_WeaponMinDamage = 0.0f;
    float l_WeaponMaxDamage = 1.0f;

    float l_MinDamage = ((l_BaseValue + l_WeaponMinDamage) * l_BasePct + l_TotalValue) * l_TotalPct;
    float l_MaxDamage = ((l_BaseValue + l_WeaponMaxDamage) * l_BasePct + l_TotalValue) * l_TotalPct;

    switch (p_AttType)
    {
        case WeaponAttackType::BaseAttack:
        default:
            SetStatFloatValue(UNIT_FIELD_MIN_DAMAGE, l_MinDamage);
            SetStatFloatValue(UNIT_FIELD_MAX_DAMAGE, l_MaxDamage);
            break;
        case WeaponAttackType::OffAttack:
            SetStatFloatValue(UNIT_FIELD_MIN_OFF_HAND_DAMAGE, l_MinDamage / 2);
            SetStatFloatValue(UNIT_FIELD_MAX_OFF_HAND_DAMAGE, l_MaxDamage / 2);
            break;
        case WeaponAttackType::RangedAttack:
            SetStatFloatValue(UNIT_FIELD_MIN_RANGED_DAMAGE, l_MinDamage);
            SetStatFloatValue(UNIT_FIELD_MAX_RANGED_DAMAGE, l_MaxDamage);
            break;
    }
}