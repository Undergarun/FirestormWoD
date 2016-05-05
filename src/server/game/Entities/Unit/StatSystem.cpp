////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

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
    if (IsPlayer() && getClass() == CLASS_MONK)
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
    if (IsPlayer())
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
    if (getClass() == CLASS_ROGUE && GetSpecializationId() == SPEC_ROGUE_ASSASSINATION)
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
        if (p_Power == (*i)->GetMiscValue()) ///< Comparison of integers of different signs: 'Powers' and 'int32' (aka 'int')
            l_Value += float((*i)->GetAmount());

        l_Value = floor(l_Value + 0.5f);
    SetMaxPower(p_Power, uint32(l_Value));
}

void Player::UpdateItemLevel()
{
    SetFloatValue(PLAYER_FIELD_AVG_ITEM_LEVEL + PlayerAvgItemLevelOffsets::TotalAvgItemLevel, (float)GetAverageItemLevelTotal());
    SetFloatValue(PLAYER_FIELD_AVG_ITEM_LEVEL + PlayerAvgItemLevelOffsets::EquippedAvgItemLevel, (float)GetAverageItemLevelEquipped());
    SetFloatValue(PLAYER_FIELD_AVG_ITEM_LEVEL + PlayerAvgItemLevelOffsets::NonPvPAvgItemLevel, (float)GetAverageItemLevelTotalWithOrWithoutPvPBonus(false));
    SetFloatValue(PLAYER_FIELD_AVG_ITEM_LEVEL + PlayerAvgItemLevelOffsets::PvPAvgItemLevel, (float)GetAverageItemLevelTotalWithOrWithoutPvPBonus(true));
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
        index_mod_pos = UNIT_FIELD_RANGED_ATTACK_POWER_MOD_POS; ///< index_mod_pos is never read 01/18/16
        index_mod_neg = UNIT_FIELD_RANGED_ATTACK_POWER_MOD_NEG; ///< index_mod_neg is never read 01/18/16
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

    float attPowerMultiplier = 1.0f;
    if (ranged)
        attPowerMultiplier = GetTotalAuraMultiplier(SPELL_AURA_MOD_RANGED_ATTACK_POWER_PCT) - 1.0f;
    else
        attPowerMultiplier = GetTotalAuraMultiplier(SPELL_AURA_MOD_ATTACK_POWER_PCT) - 1.0f;

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
        int32 spellPower = GetBaseSpellPowerBonus(); // SpellPower from Weapon
        spellPower += std::max(0, int32(GetStat(STAT_INTELLECT)) - 10); // SpellPower from intellect

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

    float att_speed = (l_UsedWeapon ? l_UsedWeapon->GetTemplate()->Delay : BASE_ATTACK_TIME) / 1000.0f;
    float attackPower = GetTotalAttackPowerValue(attType);

    /// If player doesn't have weapons we should calculate damage with this values: min damage = 1 and max damage = 2
    float weapon_mindamage = GetWeaponDamageRange(attType, MINDAMAGE) == 0 ? 1.0f : GetWeaponDamageRange(attType, MINDAMAGE);
    float weapon_maxdamage = GetWeaponDamageRange(attType, MAXDAMAGE) == 0 ? 2.0f : GetWeaponDamageRange(attType, MAXDAMAGE);

    bool dualWield = mainItem && l_OffHandItem && !l_NoLongerDualWields;
    ///float dualWieldModifier = dualWield ? 0.81f : 1.0f; // Dual Wield Penalty: 19%
    float dualWieldModifier = 1.0f; ///< I don't know about reducing for 19%. We have checked it on PTR, and damage is the same with dual wield - like without it.
    if (dualWield && HasAuraType(SPELL_AURA_INCREASE_DUAL_WIELD_DAMAGE))
        dualWieldModifier += (float)GetTotalAuraModifier(SPELL_AURA_INCREASE_DUAL_WIELD_DAMAGE) / 100.0f;

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
    if (normalized && getClass() != CLASS_MONK && getClass() != CLASS_DRUID) ///< Monks and Druids have their own damage calculation
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

void Player::CalculateNormalizedWeaponDamage(WeaponAttackType /*attType*/, float& min_damage, float& max_damage, float attackPower, float weapon_mindamage, float weapon_maxdamage, Item* l_UsedWeapon)
{
    float l_NormalizedSpeedCoef = 1.0f;

    /// Speed coefficients from http://wowwiki.wikia.com/Normalization - tested on official server, information is correct
    if (l_UsedWeapon && l_UsedWeapon->GetTemplate())
    {
        if (l_UsedWeapon->GetTemplate()->IsRangedWeapon())
            l_NormalizedSpeedCoef = 2.8f;
        else if (l_UsedWeapon->GetTemplate()->IsOneHanded())
        {
            if (l_UsedWeapon->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_DAGGER)
                l_NormalizedSpeedCoef = 1.7f;
            else
                l_NormalizedSpeedCoef = 2.4f;
        }
        else if (l_UsedWeapon->GetTemplate()->IsTwoHandedWeapon())
            l_NormalizedSpeedCoef = 3.3f;
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

const int8 k_constatBaseRaceStr[MAX_RACES] =
{
    0, ///< None
    0, ///< Humain
    3, ///< Orc
    5, ///< Dwarf
    -4, ///< Night Elfe
    -1, ///< Undead
    5, ///< Tauren
    -5, ///< Gnome
    1, ///< Troll
    -3, ///< Goblin
    -3, ///< BloodElfe
    1, ///< Drenei
    1, ///< Worgen
    0, ///< Pandaren Neutral
    0, ///< Pandaren Alli
    0 ///< Pandaren Horde
};

const int8 k_constatBaseRaceAgi[MAX_RACES] =
{
    0, ///< None
    0, ///< Humain
    -3, ///< Orc
    -4, ///< Dwarf
    4, ///< Night Elfe
    -2, ///< Undead
    -4, ///< Tauren
    2, ///< Gnome
    2, ///< Troll
    -3, ///< Goblin
    2, ///< BloodElfe
    1, ///< Drenei
    3, ///< Worgen
    -2, ///< Pandaren Neutral
    -2, ///< Pandaren Alli
    -2 ///< Pandaren Horde
};

const float k_constant[MAX_CLASSES] =
{
    0.634f,  /// Warrior
    0.634f,  /// Paladin
    1.000f,  /// Hunter
    1.000f,  /// Rogue
    1.000f,  /// Priest
    0.634f,  /// DK
    1.000f,  /// Shaman
    1.000f,  /// Mage
    1.000f,  /// Warlock
    1.659f,  /// Monk
    1.000f   /// Druid
};

const float k_DodgeFactor[MAX_CLASSES] =
{
    1.659f,  /// Warrior
    2.259f,  /// Paladin
    1.000f,  /// Hunter
    1.000f,  /// Rogue
    1.000f,  /// Priest
    1.659f,  /// DK
    1.000f,  /// Shaman
    1.000f,  /// Mage
    1.000f,  /// Warlock
    0.300f,  /// Monk
    1.000f   /// Druid
};

static float k_constantVerticalStretch[MAX_CLASSES] =
{
    0.00665f, /// Warrior
    0.00665f, /// Paladin
    0.00687f, /// Hunter
    0.00687f, /// Rogue
    0.00665f, /// Priest
    0.00665f, /// DK
    0.00687f, /// Shaman
    0.00665f, /// Mage
    0.00665f, /// Warlock
    0.00665f, /// Monk
    0.00665f  /// Druid
};

static float k_constantVHorizontalShift[MAX_CLASSES] =
{
    0.956f, /// Warrior
    0.886f, /// Paladin
    0.988f, /// Hunter
    0.988f, /// Rogue
    0.983f, /// Priest
    0.956f, /// DK
    0.988f, /// Shaman
    0.983f, /// Mage
    0.983f, /// Warlock
    1.422f, /// Monk
    1.222f  /// Druid
};

void Player::UpdateParryPercentage()
{
    /// No parry
    float l_Total = 0.0f;
    uint32 pClass = getClass() - 1;

    if (CanParry())
    {
        /*http://www.sacredduty.net/2014/08/06/tc401-avoidance-diminishing-returns-in-wod/ */
        float l_BaseParry = 3.0f;

        if (getClass() == CLASS_PALADIN || getClass() == CLASS_DEATH_KNIGHT || getClass() == CLASS_WARRIOR)
            l_BaseParry += (k_constatBaseRaceStr[getRace()] + 0.0739f) * (1 / 176.3760684f);

        float l_BonusParry = (GetTotalStatValue(STAT_STRENGTH, false)  * (1 / 176.3760684f) + (GetRatingBonusValue(CR_PARRY) / 162));
        l_Total += l_BaseParry + l_BonusParry / (l_BonusParry * k_constant[pClass] * k_constantVerticalStretch[pClass] + k_constantVHorizontalShift[pClass]);

        l_Total += GetTotalAuraModifier(SPELL_AURA_MOD_PARRY_PERCENT);

        /// Apply parry from pct of critical strike from gear
        l_Total += CalculatePct(GetRatingBonusValue(CR_CRIT_MELEE), GetTotalAuraModifier(SPELL_AURA_CONVERT_CRIT_RATING_PCT_TO_PARRY_RATING));

        if (l_Total < 0.0f)
            l_Total = 0.0f;

        if (sWorld->getBoolConfig(CONFIG_STATS_LIMITS_ENABLE))
            l_Total = l_Total > sWorld->getFloatConfig(CONFIG_STATS_LIMITS_PARRY) ? sWorld->getFloatConfig(CONFIG_STATS_LIMITS_PARRY) : l_Total;
        /// Parry from strength, just for paladin/dk/warrior        
        /*1% parry before diminishing returns = 176.3760684 strength
        1 strength gives 1 / 176.3760684 = 0,0056697034301282*/
       /* if (getClass() == CLASS_PALADIN || getClass() == CLASS_DEATH_KNIGHT || getClass() == CLASS_WARRIOR)
            diminishing += (1.0f / 176.3760684f) / 100.0f) * GetTotalStatValue(STAT_STRENGTH, false); ///< Level 100 rating

        // apply diminishing formula to diminishing parry chance
        value = nondiminishing + diminishing * parryCap[pClass] / (diminishing + (parryCap[pClass] * k_constant[pClass]));

        /// Apply parry from pct of critical strike from gear
        value += CalculatePct(GetRatingBonusValue(CR_CRIT_MELEE), GetTotalAuraModifier(SPELL_AURA_CONVERT_CRIT_RATING_PCT_TO_PARRY_RATING));

        if (value < 0.0f)
            value = 0.0f;

        if (sWorld->getBoolConfig(CONFIG_STATS_LIMITS_ENABLE))
            value = value > sWorld->getFloatConfig(CONFIG_STATS_LIMITS_PARRY) ? sWorld->getFloatConfig(CONFIG_STATS_LIMITS_PARRY) : value;*/
    }

    SetStatFloatValue(PLAYER_FIELD_PARRY_PERCENTAGE, l_Total);
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
    uint32 pClass = getClass() - 1;

    // Dodge from rating
    float l_BaseDodge = 3.0f;
    float l_Total = 0.0f;

    // Dodge from SPELL_AURA_MOD_DODGE_PERCENT aura
    l_BaseDodge += GetTotalAuraModifier(SPELL_AURA_MOD_DODGE_PERCENT);

    if (getClass() == CLASS_MONK || getClass() == CLASS_DRUID || getClass() == CLASS_ROGUE || getClass() == CLASS_HUNTER || getClass() == CLASS_SHAMAN)
        l_BaseDodge += (k_constatBaseRaceAgi[getRace()] * (1 / 176.3760684f));

    float l_BonusDodge = (GetTotalStatValue(STAT_AGILITY, false)  * (1 / 176.3760684f) + (GetRatingBonusValue(CR_DODGE) / 162));
    l_Total += l_BaseDodge + l_BonusDodge / (l_BonusDodge * k_DodgeFactor[pClass] * k_constantVerticalStretch[pClass] + k_constantVHorizontalShift[pClass]);

    if (l_Total < 0.0f)
        l_Total = 0.0f;

    if (sWorld->getBoolConfig(CONFIG_STATS_LIMITS_ENABLE))
        l_Total = l_Total > sWorld->getFloatConfig(CONFIG_STATS_LIMITS_DODGE) ? sWorld->getFloatConfig(CONFIG_STATS_LIMITS_DODGE) : l_Total;

    SetStatFloatValue(PLAYER_FIELD_DODGE_PERCENTAGE, l_Total);
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
    /// No mastery
    float l_Value = 0.0f;
    if (CanMastery() && getLevel() >= 80)
    {
        /// Mastery from SPELL_AURA_MASTERY aura
        l_Value += GetTotalAuraModifier(SPELL_AURA_MASTERY);
        float l_Modifier = 0;

        /// Mastery from rating
        float l_RatingValue = GetRatingBonusValue(CombatRating::CR_MASTERY);

        ///< Add rating pct
        AuraEffectList const& l_ModRatingPCT = GetAuraEffectsByType(AuraType::SPELL_AURA_INCREASE_RATING_PCT);
        for (AuraEffectList::const_iterator l_Iter = l_ModRatingPCT.begin(); l_Iter != l_ModRatingPCT.end(); ++l_Iter)
        {
            if ((*l_Iter)->GetMiscValue() & (1 << CombatRating::CR_MASTERY))
                l_Modifier += float((*l_Iter)->GetAmount());
        }
        AddPct(l_RatingValue, l_Modifier);

        l_Value += l_RatingValue;
        l_Value = l_Value < 0.0f ? 0.0f : l_Value;
    }
    SetFloatValue(PLAYER_FIELD_MASTERY, l_Value);

    bool l_MasteryCache = false;

    /// Update some mastery spells
    AuraApplicationMap& l_AppliedAuras = GetAppliedAuras();
    for (auto l_Iter : l_AppliedAuras)
    {
        SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(l_Iter.first);
        if (l_SpellInfo != nullptr && l_SpellInfo->HasAttribute(SpellAttr8::SPELL_ATTR8_MASTERY_SPECIALIZATION))
        {
            Aura* l_Aura = l_Iter.second->GetBase();
            for (uint8 l_I = 0; l_I < l_Aura->GetEffectCount(); ++l_I)
            {
                if (AuraEffect* l_AurEff = l_Aura->GetEffect(l_I))
                {
                    l_AurEff->SetCanBeRecalculated(true);
                    if ((l_SpellInfo->Id == 77219 && !HasAura(103958) && l_I >= EFFECT_2) ///< EFFECT_2 and EFFECT_3 of Master Demonologist are only on Metamorphis Form
                        || (l_SpellInfo->Id == 76856)) ///< Mastery : Unshackled Fury
                        l_AurEff->ChangeAmount(0, true, true);
                    else
                    {
                        l_AurEff->ChangeAmount((int32)(l_Value * l_SpellInfo->Effects[l_I].BonusMultiplier), true, true);

                        if (!l_MasteryCache)
                        {
                            m_MasteryCache = l_Value * l_SpellInfo->Effects[l_I].BonusMultiplier;
                            l_MasteryCache = true;
                        }
                    }
                }
            }
        }
    }
}

void Player::UpdateAllSpellCritChances()
{
    for (int i = SPELL_SCHOOL_NORMAL; i < MAX_SPELL_SCHOOL; ++i)
        UpdateSpellCritChance(i);
}

void Player::UpdateMultistrikePercentage()
{
    float value = GetTotalAuraModifier(SPELL_AURA_MOD_MULTISTRIKE_PCT);
    float effect = 30.0f; // Default value
    value += GetRatingBonusValue(CR_MULTISTRIKE);
    AddPct(effect, GetTotalAuraModifier(SPELL_AURA_MOD_MULTISTRIKE_EFFECT_PCT));
    SetFloatValue(PLAYER_FIELD_MULTISTRIKE, value);
    SetFloatValue(PLAYER_FIELD_MULTISTRIKE_EFFECT, effect / 100.0f);
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
    float value = 0.0f;//GetTotalAuraModifier(SPELL_AURA);
    value += GetRatingBonusValue(CR_AVOIDANCE);
    SetFloatValue(PLAYER_FIELD_AVOIDANCE, value);
}

void Player::UpdateSpeedPercentage()
{
    float value = 0.0f;
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

    /// See http://us.battle.net/wow/en/forum/topic/6794873160 .

    uint8 l_PercentOfMana = 2;
    float l_SpiritRegenIncrease = OCTRegenMPPerSpirit();   ///< Mana regen increase from spirit - per-point calculation * player stat.

    float l_CombatRegenFromSpirit = 0;
    float l_CombatRegenFromAurPct = 0;
    float l_BaseRegenFromAurPct = 0;
    float l_RegenFromModPowerRegen = /*GetTotalAuraModifierByMiscValue(SPELL_AURA_MOD_POWER_REGEN, POWER_MANA) / 5.0f*/ 0.0f;
    {
        std::map<SpellGroup, int32> l_SameEffectSpellGroup;

        AuraEffectList const& l_List = GetAuraEffectsByType(SPELL_AURA_MOD_POWER_REGEN);
        for (auto const& l_Eff : l_List)
        {
            if (l_Eff->GetMiscValue() == POWER_MANA)
            {
                if (!sSpellMgr->AddSameEffectStackRuleSpellGroups(l_Eff->GetSpellInfo(), l_Eff->GetAmount(), l_SameEffectSpellGroup))
                {
                    auto l_Base = l_Eff->GetBase();
                    if (l_Base && l_Base->GetMaxDuration() == 3600000) ///< is one-hour aura
                        l_RegenFromModPowerRegen += l_Eff->GetAmount();
                    else
                        l_RegenFromModPowerRegen += l_Eff->GetAmount() / 5.0f;
                }
            }
        }

        for (auto const& l_Kvp :l_SameEffectSpellGroup)
        {
            l_RegenFromModPowerRegen += l_Kvp.second / 5.0f;
        }
    }

    /// Warlocks and Mages have 5% of maximum mana in base mana regen - blizzlike
    if (getClass() == CLASS_WARLOCK || getClass() == CLASS_MAGE)
        l_PercentOfMana = 5;

    /// 2% of base mana each 5 seconds.
    float l_Combat_regen = float(CalculatePct(GetMaxPower(POWER_MANA), l_PercentOfMana));
    float l_Base_regen = float(CalculatePct(GetMaxPower(POWER_MANA), l_PercentOfMana));

    if (HasAuraType(SPELL_AURA_MOD_MANA_REGEN_BY_HASTE))
    {
        float l_HastePct = 1.0f + GetUInt32Value(PLAYER_FIELD_COMBAT_RATINGS + CR_HASTE_MELEE) * GetRatingMultiplier(CR_HASTE_MELEE) / 100.0f;

        l_Combat_regen *= l_HastePct;
        l_Base_regen *= l_HastePct;
    }

    /// Try to get aura with spirit addition to combat mana regen.
    /// Meditation: Allows 50% of your mana regeneration from Spirit to continue while in combat.
    int32 l_PercentAllowCombatRegenBySpirit = 0;
    Unit::AuraEffectList const& ModPowerRegenPCTAuras = GetAuraEffectsByType(SPELL_AURA_MOD_MANA_REGEN_INTERRUPT);
    for (AuraEffectList::const_iterator i = ModPowerRegenPCTAuras.begin(); i != ModPowerRegenPCTAuras.end(); ++i)
        l_PercentAllowCombatRegenBySpirit += (*i)->GetAmount();

    if (HasAuraType(SPELL_AURA_MOD_MANA_REGEN_INTERRUPT) && l_PercentAllowCombatRegenBySpirit != 0)
        l_CombatRegenFromSpirit += (float(l_PercentAllowCombatRegenBySpirit) / 100) * l_SpiritRegenIncrease; ///< Allows you mana regeneration from Spirit to continue while in combat.

    /// Increase mana regen.
    int32 l_PercentIncreaseManaRegen = 0;
    int32 l_IncreaseManaRegen = l_Combat_regen;

    /// Increase mana from SPELL_AURA_MOD_POWER_REGEN_PERCENT
    Unit::AuraEffectList const& ModRegenPct = GetAuraEffectsByType(SPELL_AURA_MOD_POWER_REGEN_PERCENT);
    for (AuraEffectList::const_iterator i = ModRegenPct.begin(); i != ModRegenPct.end(); ++i)
        if (Powers((*i)->GetMiscValue()) == POWER_MANA)
            l_IncreaseManaRegen += l_IncreaseManaRegen * ((*i)->GetAmount() / 100.0f);

    /// Increase mana from SPELL_AURA_MODIFY_MANA_REGEN_FROM_MANA_PCT
    Unit::AuraEffectList const& ModRegenPctUnk = GetAuraEffectsByType(SPELL_AURA_MODIFY_MANA_REGEN_FROM_MANA_PCT);
    for (AuraEffectList::const_iterator i = ModRegenPctUnk.begin(); i != ModRegenPctUnk.end(); ++i)
        l_IncreaseManaRegen += l_IncreaseManaRegen * ((*i)->GetAmount() / 100.0f);

    /// If IncreaseManaRegen is bigger then combat_regen we have increased mana regen by auras, so we should add it
    if (HasAuraType(SPELL_AURA_MOD_POWER_REGEN_PERCENT) || HasAuraType(SPELL_AURA_MOD_MANA_REGEN_FROM_STAT) || (HasAuraType(SPELL_AURA_MODIFY_MANA_REGEN_FROM_MANA_PCT) && l_IncreaseManaRegen > l_Combat_regen))
    {
        l_IncreaseManaRegen -= l_Combat_regen;
        l_BaseRegenFromAurPct = l_IncreaseManaRegen;
        l_CombatRegenFromAurPct = l_IncreaseManaRegen;
    }

    /// Calculate for 1 second, the client multiplies the field values by 5.
    l_Base_regen = ((l_Base_regen + l_BaseRegenFromAurPct + l_RegenFromModPowerRegen) / 5.0f) + l_SpiritRegenIncrease;
    l_Combat_regen = ((l_Combat_regen + l_CombatRegenFromAurPct + l_RegenFromModPowerRegen) / 5.0f) + l_CombatRegenFromSpirit;

    /// Out of Combat
    SetStatFloatValue(UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER, l_Base_regen);
    /// In Combat
    SetStatFloatValue(UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER, l_Combat_regen);
}

void Player::UpdateEnergyRegen()
{
    if (getPowerType() != Powers::POWER_ENERGY)
        return;

    uint32 l_PowerIndex = GetPowerIndex(Powers::POWER_ENERGY, getClass());

    float l_RegenFlatMultiplier = 1.0f;
    Unit::AuraEffectList const& l_RegenAura = GetAuraEffectsByType(SPELL_AURA_MOD_POWER_REGEN_PERCENT);
    for (auto l_AuraEffect : l_RegenAura)
    {
        if (l_AuraEffect->GetMiscValue() != Powers::POWER_ENERGY)
            continue;

        l_RegenFlatMultiplier += l_AuraEffect->GetAmount() / 100.0f;
    }
    SetFloatValue(EUnitFields::UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER + l_PowerIndex, GetRegenForPower(Powers::POWER_ENERGY) - (GetRegenForPower(Powers::POWER_ENERGY) / l_RegenFlatMultiplier));
    SetFloatValue(EUnitFields::UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER + l_PowerIndex, GetRegenForPower(Powers::POWER_ENERGY) - (GetRegenForPower(Powers::POWER_ENERGY) / l_RegenFlatMultiplier));
}

void Player::UpdateFocusRegen()
{
    if (getPowerType() != Powers::POWER_FOCUS)
        return;

    SetFloatValue(EUnitFields::UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER, GetRegenForPower(Powers::POWER_FOCUS));
    SetFloatValue(EUnitFields::UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER, GetRegenForPower(Powers::POWER_FOCUS));
}

void Player::UpdateAllRunesRegen()
{
    if (getClass() != Classes::CLASS_DEATH_KNIGHT)
        return;

    for (uint8 l_I = 0; l_I < RuneType::NUM_RUNE_TYPES; ++l_I)
    {
        if (uint32 l_Cooldown = GetRuneTypeBaseCooldown(RuneType(l_I)))
        {
            float l_Regen = float(1 * TimeConstants::IN_MILLISECONDS) / float(l_Cooldown);

            if (l_Regen < 0.0099999998f)
                l_Regen = 0.01f;

            SetFloatValue(EPlayerFields::PLAYER_FIELD_RUNE_REGEN + l_I, l_Regen);
        }
    }

    SetFloatValue(EUnitFields::UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER, GetRegenForPower(Powers::POWER_RUNES));
}

float Player::GetRegenForPower(Powers p_Power)
{
    float l_BaseRegen = 0.0f;

    switch (p_Power)
    {
        /// Last Update: 6.1.2 19865
        /// Client calculate this value itself, i don't know how, it has base value 5, but it should be 4, so just -1.0f
        /// I've done some tests and now it's fine, please don't touch, just if server version is changed and client-part value is fixed
        case Powers::POWER_FOCUS:
            /// Steady Focus increase focus regen, i don't want to remove a hack with return -1 because it works fine
            if (HasAura(177668))
                return 1.0f;
            else
                return -1.0f;
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
            l_Pct += (float)(*l_Iter)->GetAmount() / 100.0f;
    }

    float l_HastePct = 1.0f;
    float l_Total = 1.0f;

    l_HastePct = GetFloatValue(EUnitFields::UNIT_FIELD_MOD_HASTE_REGEN);
    l_Total = l_BaseRegen * l_HastePct / l_Pct;

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

void Creature::UpdateDamagePhysical(WeaponAttackType p_AttType, bool /*l_NoLongerDualWields*/)
{
    float l_Variance = 1.0f;
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

    /// Since WoD, there is no need to add specific template for each difficulty (except for LFR and Mythic modes)
    /// Then for heroic mode we must increase the dmg_multiplier by 20% (guessed from creature_groupsizestats changes)
    if (GetCreatureTemplate()->expansion == Expansion::EXPANSION_WARLORDS_OF_DRAENOR && GetMap()->GetDifficultyID() == Difficulty::DifficultyRaidHeroic && isWorldBoss())
        l_DmgMultiplier *= 1.2f;

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

    Unit* l_Owner = GetSummoner();

    if (!l_Owner)
        return false;
    
    float l_Mod = 0.75f;

    switch (p_Stat)
    {
        case STAT_STAMINA:
        {
            l_Mod = 0.3f; ///< l_mod is never read 01/18/16

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
    Unit* l_Owner = GetSummoner();
    if (l_Owner == nullptr)
        return;

    UnitMods l_InitMod = UNIT_MOD_ARMOR;
    PetStatInfo const* l_PetStat = GetPetStat();

    /// - Since 6.x, armor of pet are always percentage of owner armor (default 1)
    float l_Value = l_Owner->GetArmor() * (l_PetStat != nullptr ? l_PetStat->m_ArmorCoef : 1.0f);

    /// - Apply mods
    {
        l_Value *= GetModifierValue(l_InitMod, BASE_PCT);
        l_Value *= GetModifierValue(l_InitMod, TOTAL_PCT);

        float l_Amount = 0;
        AuraEffectList const& l_ModPetStats = l_Owner->GetAuraEffectsByType(SPELL_AURA_MOD_PET_STATS);
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
    Unit* l_Owner = GetSummoner();
    if (l_Owner == nullptr)
        return;

    UnitMods l_UnitMod = UNIT_MOD_HEALTH;
    PetStatInfo const* l_PetStat = GetPetStat();

    float l_Value = (l_PetStat != nullptr) ? l_Owner->GetMaxHealth() * GetPetStat()->m_HealthCoef : GetModifierValue(l_UnitMod, BASE_VALUE) + GetCreateHealth();

    l_Value *= GetModifierValue(l_UnitMod, BASE_PCT);
    l_Value += GetModifierValue(l_UnitMod, TOTAL_VALUE);
    l_Value *= GetModifierValue(l_UnitMod, TOTAL_PCT);

    float l_Amount = 0;
    AuraEffectList const& l_ModPetStats = l_Owner->GetAuraEffectsByType(SPELL_AURA_MOD_PET_STATS);
    for (AuraEffectList::const_iterator l_Iterator = l_ModPetStats.begin(); l_Iterator != l_ModPetStats.end(); ++l_Iterator)
    {
        if ((*l_Iterator)->GetMiscValueB() && (int32)GetEntry() != (*l_Iterator)->GetMiscValueB())
            continue;
        if ((*l_Iterator)->GetMiscValue() == INCREASE_HEALTH_PERCENT)
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

    Unit* l_Owner = GetSummoner();
    if (l_Owner == nullptr)
        return;

    float l_AddValue = (l_PetStat != nullptr && l_PetStat->m_Power == p_Power) ? 0.0f : GetStat(STAT_INTELLECT) - GetCreateStat(STAT_INTELLECT);
    float l_Multiplicator = 15.0f;

    float l_Value = 0.0f;
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
                l_Value = float(l_Owner->GetMaxPower(l_Owner->getPowerType()) * l_PetStat->m_CreatePower);
        }
    }

    l_Value += GetModifierValue(l_UnitMod, BASE_VALUE);
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

    Unit* l_Owner = GetSummoner();
    if (l_Owner == nullptr)
        return;

    UnitMods l_UnitMod = UNIT_MOD_ATTACK_POWER;
    float l_BaseValue  = std::max(0.0f, 2 * GetStat(STAT_STRENGTH) - 20.0f);

    float l_BaseAttackPower       = l_BaseValue;
    float l_SpellPower            = l_BaseValue;
    float l_AttackPowerMultiplier = 1.0f;

    PetStatInfo const* l_PetStat = GetPetStat();
    if (l_PetStat != nullptr)
    {
        switch (l_PetStat->m_PowerStat)
        {
            case PetStatInfo::PowerStatBase::AttackPower:
                l_BaseValue       = l_Owner->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack);
                l_BaseAttackPower = l_BaseValue * l_PetStat->m_APSPCoef;
                l_SpellPower      = l_BaseValue * l_PetStat->m_SecondaryStatCoef;
                break;
            case PetStatInfo::PowerStatBase::SpellPower:
                l_BaseValue       = l_Owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SPELL);
                l_SpellPower      = l_BaseValue * l_PetStat->m_APSPCoef;
                l_BaseAttackPower = l_BaseValue * l_PetStat->m_SecondaryStatCoef;
                break;
            default:
                break;
        }
    }

    SetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_VALUE, l_BaseAttackPower);

    if (l_Owner->IsPlayer() && GetEntry() != ENTRY_FROZEN_ORB)
        l_Owner->SetUInt32Value(PLAYER_FIELD_PET_SPELL_POWER, l_SpellPower);

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

    /// Update off hand weapon damage for Shivarra and Wrathguard
    if (GetEntry() == ENTRY_SHIVARRA || GetEntry() == ENTRY_WRATHGUARD)
        UpdateDamagePhysical(WeaponAttackType::OffAttack);
}

/// WoD updated
void Guardian::UpdateDamagePhysical(WeaponAttackType p_AttType, bool /*l_NoLongerDualWields*/)
{
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

    float l_AttackSpeed = float(GetAttackTime(WeaponAttackType::BaseAttack)) / 1000.0f;

    float l_BaseValue  = GetModifierValue(l_UnitMod, BASE_VALUE);

    l_BaseValue += GetTotalAttackPowerValue(p_AttType) / 3.5f * l_AttackSpeed;

    PetStatInfo const* l_PetStat = GetPetStat();
    if (l_PetStat != nullptr)
        l_BaseValue *= l_PetStat->m_DamageCoef;

    float l_BasePct    = GetModifierValue(l_UnitMod, BASE_PCT);
    float l_TotalValue = GetModifierValue(l_UnitMod, TOTAL_VALUE);
    float l_TotalPct   = GetModifierValue(l_UnitMod, TOTAL_PCT);

    float l_WeaponMinDamage = GetWeaponDamageRange(WeaponAttackType::BaseAttack, MINDAMAGE);
    float l_WeaponMaxDamage = GetWeaponDamageRange(WeaponAttackType::BaseAttack, MAXDAMAGE);

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
            SetStatFloatValue(UNIT_FIELD_MIN_OFF_HAND_DAMAGE, l_MinDamage);
            SetStatFloatValue(UNIT_FIELD_MAX_OFF_HAND_DAMAGE, l_MaxDamage);
            break;
        case WeaponAttackType::RangedAttack:
            SetStatFloatValue(UNIT_FIELD_MIN_RANGED_DAMAGE, l_MinDamage);
            SetStatFloatValue(UNIT_FIELD_MAX_RANGED_DAMAGE, l_MaxDamage);
            break;
    }
}
