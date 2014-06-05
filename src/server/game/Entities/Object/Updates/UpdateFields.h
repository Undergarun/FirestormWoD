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

#ifndef _UPDATEFIELDS_H
#define _UPDATEFIELDS_H
struct CGObjectData
{
    uint32 m_guid[2];
    uint32 m_data[2];
    uint32 m_type;
    uint32 m_entryID;
    uint32 m_dynamicFlags;
    uint32 m_scale;
};

struct CGItemData
{
    uint32 m_owner[2];
    uint32 m_containedIn[2];
    uint32 m_creator[2];
    uint32 m_giftCreator[2];
    uint32 m_stackCount;
    uint32 m_expiration;
    uint32 m_spellCharges[5];
    uint32 m_dynamicFlags;
    uint32 m_enchantment[39];
    uint32 m_propertySeed;
    uint32 m_randomPropertiesID;
    uint32 m_durability;
    uint32 m_maxDurability;
    uint32 m_createPlayedTime;
    uint32 m_modifiersMask;
    uint32 m_context;
};

struct CGContainerData
{
    uint32 m_slots[72];
    uint32 m_numSlots;
};

struct CGUnitData
{
    uint32 charm[2];
    uint32 summon[2];
    uint32 critter[2];
    uint32 charmedBy[2];
    uint32 summonedBy[2];
    uint32 createdBy[2];
    uint32 demonCreator[2];
    uint32 target[2];
    uint32 battlePetCompanionGUID[2];
    uint32 channelObject[2];
    uint32 channelSpell;
    uint32 summonedByHomeRealm;
    uint32 sex;
    uint32 displayPower;
    uint32 overrideDisplayPowerID;
    uint32 health;
    uint32 power[6];
    uint32 maxHealth;
    uint32 maxPower[6];
    uint32 powerRegenFlatModifier[6];
    uint32 powerRegenInterruptedFlatModifier[6];
    uint32 level;
    uint32 effectiveLevel;
    uint32 factionTemplate;
    uint32 virtualItemID[3];
    uint32 flags;
    uint32 flags2;
    uint32 flags3;
    uint32 auraState;
    uint32 attackRoundBaseTime[2];
    uint32 rangedAttackRoundBaseTime;
    uint32 boundingRadius;
    uint32 combatReach;
    uint32 displayID;
    uint32 nativeDisplayID;
    uint32 mountDisplayID;
    uint32 minDamage;
    uint32 maxDamage;
    uint32 minOffHandDamage;
    uint32 maxOffHandDamage;
    uint32 animTier;
    uint32 petNumber;
    uint32 petNameTimestamp;
    uint32 petExperience;
    uint32 petNextLevelExperience;
    uint32 modCastingSpeed;
    uint32 modSpellHaste;
    uint32 modHaste;
    uint32 modRangedHaste;
    uint32 modHasteRegen;
    uint32 createdBySpell;
    uint32 npcFlags;
    uint32 emoteState;
    uint32 stats[5];
    uint32 statPosBuff[5];
    uint32 statNegBuff[5];
    uint32 resistances[7];
    uint32 resistanceBuffModsPositive[7];
    uint32 resistanceBuffModsNegative[7];
    uint32 baseMana;
    uint32 baseHealth;
    uint32 shapeshiftForm;
    uint32 attackPower;
    uint32 attackPowerModPos;
    uint32 attackPowerModNeg;
    uint32 attackPowerMultiplier;
    uint32 rangedAttackPower;
    uint32 rangedAttackPowerModPos;
    uint32 rangedAttackPowerModNeg;
    uint32 rangedAttackPowerMultiplier;
    uint32 minRangedDamage;
    uint32 maxRangedDamage;
    uint32 powerCostModifier[7];
    uint32 powerCostMultiplier[7];
    uint32 maxHealthModifier;
    uint32 hoverHeight;
    uint32 minItemLevel;
    uint32 maxItemLevel;
    uint32 wildBattlePetLevel;
    uint32 battlePetCompanionNameTimestamp;
    uint32 interactSpellID;
    uint32 stateSpellVisualID;
    uint32 stateAnimID;
    uint32 stateAnimKitID;
    uint32 stateWorldEffectID[4];
    uint32 garrisonFollowerGivenNameID;
    uint32 garrisonFollowerFamilyNameID;
    uint32 scaleDuration;
};

struct CGPlayerData
{
    uint32 duelArbiter[2];
    uint32 playerFlags;
    uint32 playerFlagsEx;
    uint32 guildRankID;
    uint32 guildDeleteDate;
    uint32 guildLevel;
    uint32 hairColorID;
    uint32 restState;
    uint32 arenaFaction;
    uint32 duelTeam;
    uint32 guildTimeStamp;
    uint32 questLog[750];
    uint32 visibleItems[57];
    uint32 playerTitle;
    uint32 fakeInebriation;
    uint32 virtualPlayerRealm;
    uint32 currentSpecID;
    uint32 taxiMountAnimKitID;
    uint32 avgItemLevelTotal;
    uint32 avgItemLevelEquipped;
    uint32 currentBattlePetBreedQuality;
    uint32 local_invSlots[368];
    uint32 local_farsightObject[2];
    uint32 local_knownTitles[10];
    uint32 local_coinage[2];
    uint32 local_XP;
    uint32 local_nextLevelXP;
    uint32 local_skill[448];
    uint32 local_characterPoints;
    uint32 local_maxTalentTiers;
    uint32 local_trackCreatureMask;
    uint32 local_trackResourceMask;
    uint32 local_mainhandExpertise;
    uint32 local_offhandExpertise;
    uint32 local_rangedExpertise;
    uint32 local_combatRatingExpertise;
    uint32 local_blockPercentage;
    uint32 local_dodgePercentage;
    uint32 local_parryPercentage;
    uint32 local_critPercentage;
    uint32 local_rangedCritPercentage;
    uint32 local_offhandCritPercentage;
    uint32 local_spellCritPercentage[7];
    uint32 local_shieldBlock;
    uint32 local_shieldBlockCritPercentage;
    uint32 local_mastery;
    uint32 local_amplify;
    uint32 local_multistrike;
    uint32 local_multistrikeEffect;
    uint32 local_readiness;
    uint32 local_speed;
    uint32 local_lifesteal;
    uint32 local_avoidance;
    uint32 local_sturdiness;
    uint32 local_cleave;
    uint32 local_pvpPowerDamage;
    uint32 local_pvpPowerHealing;
    uint32 local_exploredZones[200];
    uint32 local_restStateBonusPool;
    uint32 local_modDamageDonePos[7];
    uint32 local_modDamageDoneNeg[7];
    uint32 local_modDamageDonePercent[7];
    uint32 local_modHealingDonePos;
    uint32 local_modHealingPercent;
    uint32 local_modHealingDonePercent;
    uint32 local_modPeriodicHealingDonePercent;
    uint32 local_weaponDmgMultipliers[3];
    uint32 local_modSpellPowerPercent;
    uint32 local_modResiliencePercent;
    uint32 local_overrideSpellPowerByAPPercent;
    uint32 local_overrideAPBySpellPowerPercent;
    uint32 local_modTargetResistance;
    uint32 local_modTargetPhysicalResistance;
    uint32 local_localFlags;
    uint32 local_lifetimeMaxRank;
    uint32 local_selfResSpell;
    uint32 local_pvpMedals;
    uint32 local_buybackPrice[12];
    uint32 local_buybackTimestamp[12];
    uint32 local_yesterdayHonorableKills;
    uint32 local_lifetimeHonorableKills;
    uint32 local_watchedFactionIndex;
    uint32 local_combatRatings[32];
    uint32 local_pvpInfo[36];
    uint32 local_maxLevel;
    uint32 local_runeRegen[4];
    uint32 local_noReagentCostMask[4];
    uint32 local_glyphSlots[6];
    uint32 local_glyphs[6];
    uint32 local_glyphSlotsEnabled;
    uint32 local_petSpellPower;
    uint32 local_researching[10];
    uint32 local_professionSkillLine[2];
    uint32 local_uiHitModifier;
    uint32 local_uiSpellHitModifier;
    uint32 local_homeRealmTimeOffset;
    uint32 local_modPetHaste;
    uint32 local_summonedBattlePetGUID[2];
    uint32 local_overrideSpellsID;
    uint32 local_lfgBonusFactionID;
    uint32 local_lootSpecID;
    uint32 local_overrideZonePVPType;
    uint32 local_itemLevelDelta;
    uint32 local_bagSlotFlags[4];
    uint32 local_bankAutoSortDisabled;
    uint32 local_bankBagSlotAutoSortDisabled;
};

struct CGGameObjectData
{
    uint32 m_createdBy[2];
    uint32 m_displayID;
    uint32 m_flags;
    uint32 m_parentRotation[4];
    uint32 m_factionTemplate;
    uint32 m_level;
    uint32 m_percentHealth;
    uint32 m_spellVisualID;
    uint32 m_stateSpellVisualID;
    uint32 m_stateAnimID;
    uint32 m_stateAnimKitID;
    uint32 m_stateWorldEffectID[4];
};

struct CGDynamicObjectData
{
    uint32 m_caster[2];
    uint32 m_typeAndVisualID;
    uint32 m_spellID;
    uint32 m_radius;
    uint32 m_castTime;
};

struct CGCorpseData
{
    uint32 m_owner[2];
    uint32 m_partyGUID[2];
    uint32 m_displayID;
    uint32 m_items[19];
    uint32 m_skinID;
    uint32 m_facialHairStyleID;
    uint32 m_flags;
    uint32 m_dynamicFlags;
    uint32 m_factionTemplate;
};

struct CGAreaTriggerData
{
    uint32 m_caster[2];
    uint32 m_duration;
    uint32 m_spellID;
    uint32 m_spellVisualID;
    uint32 m_explicitScale;
};

struct CGSceneObjectData
{
    uint32 m_scriptPackageID;
    uint32 m_rndSeedVal;
    uint32 m_createdBy[2];
    uint32 m_sceneType;
};

struct CGConversationData
{
    uint32 dummy;
};

// > Object
enum EObjectFields
{
    OBJECT_FIELD_GUID = 0, // SIZE:2
    OBJECT_FIELD_DATA = 2, // SIZE:2
    OBJECT_FIELD_TYPE = 4, // SIZE:1
    OBJECT_FIELD_ENTRY_ID = 5, // SIZE:1
    OBJECT_FIELD_DYNAMIC_FLAGS = 6, // SIZE:1
    OBJECT_FIELD_SCALE = 7, // SIZE:1
    OBJECT_END = 8
};

// > Object > Item
enum EItemFields
{
    ITEM_FIELD_OWNER = 8, // SIZE:2
    ITEM_FIELD_CONTAINED_IN = 10, // SIZE:2
    ITEM_FIELD_CREATOR = 12, // SIZE:2
    ITEM_FIELD_GIFT_CREATOR = 14, // SIZE:2
    ITEM_FIELD_STACK_COUNT = 16, // SIZE:1
    ITEM_FIELD_EXPIRATION = 17, // SIZE:1
    ITEM_FIELD_SPELL_CHARGES = 18, // SIZE:5
    ITEM_FIELD_DYNAMIC_FLAGS = 23, // SIZE:1
    ITEM_FIELD_ENCHANTMENT = 24, // SIZE:39
    ITEM_FIELD_PROPERTY_SEED = 63, // SIZE:1
    ITEM_FIELD_RANDOM_PROPERTIES_ID = 64, // SIZE:1
    ITEM_FIELD_DURABILITY = 65, // SIZE:1
    ITEM_FIELD_MAX_DURABILITY = 66, // SIZE:1
    ITEM_FIELD_CREATE_PLAYED_TIME = 67, // SIZE:1
    ITEM_FIELD_MODIFIERS_MASK = 68, // SIZE:1
    ITEM_FIELD_CONTEXT = 69, // SIZE:1
    ITEM_END = 70
};

// > Object > Item > Container
enum EContainerFields
{
    CONTAINER_FIELD_SLOTS = 70, // SIZE:72
    CONTAINER_FIELD_NUM_SLOTS = 142, // SIZE:1
    CONTAINER_END = 143
};

// > Object > Unit
enum EUnitFields
{
    UNIT_FIELD_CHARM = 8, // SIZE:2
    UNIT_FIELD_SUMMON = 10, // SIZE:2
    UNIT_FIELD_CRITTER = 12, // SIZE:2
    UNIT_FIELD_CHARMED_BY = 14, // SIZE:2
    UNIT_FIELD_SUMMONED_BY = 16, // SIZE:2
    UNIT_FIELD_CREATED_BY = 18, // SIZE:2
    UNIT_FIELD_DEMON_CREATOR = 20, // SIZE:2
    UNIT_FIELD_TARGET = 22, // SIZE:2
    UNIT_FIELD_BATTLE_PET_COMPANION_GUID = 24, // SIZE:2
    UNIT_FIELD_CHANNEL_OBJECT = 26, // SIZE:2
    UNIT_FIELD_CHANNEL_SPELL = 28, // SIZE:1
    UNIT_FIELD_SUMMONED_BY_HOME_REALM = 29, // SIZE:1
    UNIT_FIELD_SEX = 30, // SIZE:1
    UNIT_FIELD_DISPLAY_POWER = 31, // SIZE:1
    UNIT_FIELD_OVERRIDE_DISPLAY_POWER_ID = 32, // SIZE:1
    UNIT_FIELD_HEALTH = 33, // SIZE:1
    UNIT_FIELD_POWER = 34, // SIZE:6
    UNIT_FIELD_MAX_HEALTH = 40, // SIZE:1
    UNIT_FIELD_MAX_POWER = 41, // SIZE:6
    UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER = 47, // SIZE:6
    UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER = 53, // SIZE:6
    UNIT_FIELD_LEVEL = 59, // SIZE:1
    UNIT_FIELD_EFFECTIVE_LEVEL = 60, // SIZE:1
    UNIT_FIELD_FACTION_TEMPLATE = 61, // SIZE:1
    UNIT_FIELD_VIRTUAL_ITEM_ID = 62, // SIZE:3
    UNIT_FIELD_FLAGS = 65, // SIZE:1
    UNIT_FIELD_FLAGS2 = 66, // SIZE:1
    UNIT_FIELD_FLAGS3 = 67, // SIZE:1
    UNIT_FIELD_AURA_STATE = 68, // SIZE:1
    UNIT_FIELD_ATTACK_ROUND_BASE_TIME = 69, // SIZE:2
    UNIT_FIELD_RANGED_ATTACK_ROUND_BASE_TIME = 71, // SIZE:1
    UNIT_FIELD_BOUNDING_RADIUS = 72, // SIZE:1
    UNIT_FIELD_COMBAT_REACH = 73, // SIZE:1
    UNIT_FIELD_DISPLAY_ID = 74, // SIZE:1
    UNIT_FIELD_NATIVE_DISPLAY_ID = 75, // SIZE:1
    UNIT_FIELD_MOUNT_DISPLAY_ID = 76, // SIZE:1
    UNIT_FIELD_MIN_DAMAGE = 77, // SIZE:1
    UNIT_FIELD_MAX_DAMAGE = 78, // SIZE:1
    UNIT_FIELD_MIN_OFF_HAND_DAMAGE = 79, // SIZE:1
    UNIT_FIELD_MAX_OFF_HAND_DAMAGE = 80, // SIZE:1
    UNIT_FIELD_ANIM_TIER = 81, // SIZE:1
    UNIT_FIELD_PET_NUMBER = 82, // SIZE:1
    UNIT_FIELD_PET_NAME_TIMESTAMP = 83, // SIZE:1
    UNIT_FIELD_PET_EXPERIENCE = 84, // SIZE:1
    UNIT_FIELD_PET_NEXT_LEVEL_EXPERIENCE = 85, // SIZE:1
    UNIT_FIELD_MOD_CASTING_SPEED = 86, // SIZE:1
    UNIT_FIELD_MOD_SPELL_HASTE = 87, // SIZE:1
    UNIT_FIELD_MOD_HASTE = 88, // SIZE:1
    UNIT_FIELD_MOD_RANGED_HASTE = 89, // SIZE:1
    UNIT_FIELD_MOD_HASTE_REGEN = 90, // SIZE:1
    UNIT_FIELD_CREATED_BY_SPELL = 91, // SIZE:1
    UNIT_FIELD_NPC_FLAGS = 92, // SIZE:1
    UNIT_FIELD_NPC_FLAGS2 = 93, // SIZE:2
    UNIT_FIELD_EMOTE_STATE = 94, // SIZE:1
    UNIT_FIELD_STATS = 95, // SIZE:5
    UNIT_FIELD_STAT_POS_BUFF = 100, // SIZE:5
    UNIT_FIELD_STAT_NEG_BUFF = 105, // SIZE:5
    UNIT_FIELD_RESISTANCES = 110, // SIZE:7
    UNIT_FIELD_RESISTANCE_BUFF_MODS_POSITIVE = 117, // SIZE:7
    UNIT_FIELD_RESISTANCE_BUFF_MODS_NEGATIVE = 124, // SIZE:7
    UNIT_FIELD_BASE_MANA = 131, // SIZE:1
    UNIT_FIELD_BASE_HEALTH = 132, // SIZE:1
    UNIT_FIELD_SHAPESHIFT_FORM = 133, // SIZE:1
    UNIT_FIELD_ATTACK_POWER = 134, // SIZE:1
    UNIT_FIELD_ATTACK_POWER_MOD_POS = 135, // SIZE:1
    UNIT_FIELD_ATTACK_POWER_MOD_NEG = 136, // SIZE:1
    UNIT_FIELD_ATTACK_POWER_MULTIPLIER = 137, // SIZE:1
    UNIT_FIELD_RANGED_ATTACK_POWER = 138, // SIZE:1
    UNIT_FIELD_RANGED_ATTACK_POWER_MOD_POS = 139, // SIZE:1
    UNIT_FIELD_RANGED_ATTACK_POWER_MOD_NEG = 140, // SIZE:1
    UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER = 141, // SIZE:1
    UNIT_FIELD_MIN_RANGED_DAMAGE = 142, // SIZE:1
    UNIT_FIELD_MAX_RANGED_DAMAGE = 143, // SIZE:1
    UNIT_FIELD_POWER_COST_MODIFIER = 144, // SIZE:7
    UNIT_FIELD_POWER_COST_MULTIPLIER = 151, // SIZE:7
    UNIT_FIELD_MAX_HEALTH_MODIFIER = 158, // SIZE:1
    UNIT_FIELD_HOVER_HEIGHT = 159, // SIZE:1
    UNIT_FIELD_MIN_ITEM_LEVEL = 160, // SIZE:1
    UNIT_FIELD_MAX_ITEM_LEVEL = 161, // SIZE:1
    UNIT_FIELD_WILD_BATTLE_PET_LEVEL = 162, // SIZE:1
    UNIT_FIELD_BATTLE_PET_COMPANION_NAME_TIMESTAMP = 163, // SIZE:1
    UNIT_FIELD_INTERACT_SPELL_ID = 164, // SIZE:1
    UNIT_FIELD_STATE_SPELL_VISUAL_ID = 165, // SIZE:1
    UNIT_FIELD_STATE_ANIM_ID = 166, // SIZE:1
    UNIT_FIELD_STATE_ANIM_KIT_ID = 167, // SIZE:1
    UNIT_FIELD_STATE_WORLD_EFFECT_ID = 168, // SIZE:4
    UNIT_FIELD_GARRISON_FOLLOWER_GIVEN_NAME_ID = 172, // SIZE:1
    UNIT_FIELD_GARRISON_FOLLOWER_FAMILY_NAME_ID = 173, // SIZE:1
    UNIT_FIELD_SCALE_DURATION = 174, // SIZE:1
    UNIT_END = 175
};

// > Object > Unit > Player
enum EPlayerFields
{
    PLAYER_FIELD_DUEL_ARBITER = 175, // SIZE:2
    PLAYER_FIELD_PLAYER_FLAGS = 177, // SIZE:1
    PLAYER_FIELD_PLAYER_FIELD_PLAYER_FLAGS_EX = 178, // SIZE:1
    PLAYER_FIELD_GUILD_RANK_ID = 179, // SIZE:1
    PLAYER_FIELD_GUILD_DELETE_DATE = 180, // SIZE:1
    PLAYER_FIELD_GUILD_LEVEL = 181, // SIZE:1
    PLAYER_FIELD_HAIR_COLOR_ID = 182, // SIZE:1
    PLAYER_FIELD_REST_STATE = 183, // SIZE:1
    PLAYER_FIELD_ARENA_FACTION = 184, // SIZE:1
    PLAYER_FIELD_DUEL_TEAM = 185, // SIZE:1
    PLAYER_FIELD_GUILD_TIME_STAMP = 186, // SIZE:1
    PLAYER_FIELD_QUEST_LOG = 187, // SIZE:750
    PLAYER_FIELD_VISIBLE_ITEMS = 937, // SIZE:57
    PLAYER_FIELD_PLAYER_TITLE = 994, // SIZE:1
    PLAYER_FIELD_FAKE_INEBRIATION = 995, // SIZE:1
    PLAYER_FIELD_VIRTUAL_PLAYER_REALM = 996, // SIZE:1
    PLAYER_FIELD_CURRENT_SPEC_ID = 997, // SIZE:1
    PLAYER_FIELD_TAXI_MOUNT_ANIM_KIT_ID = 998, // SIZE:1
    PLAYER_FIELD_AVG_ITEM_LEVEL_TOTAL = 999, // SIZE:1
    PLAYER_FIELD_AVG_ITEM_LEVEL_EQUIPPED = 1000, // SIZE:1
    PLAYER_FIELD_CURRENT_BATTLE_PET_BREED_QUALITY = 1001, // SIZE:1
    PLAYER_FIELD_INV_SLOTS = 1002, // SIZE:368
    PLAYER_FIELD_FARSIGHT_OBJECT = 1370, // SIZE:2
    PLAYER_FIELD_KNOWN_TITLES = 1372, // SIZE:10
    PLAYER_FIELD_COINAGE = 1382, // SIZE:2
    PLAYER_FIELD_XP = 1384, // SIZE:1
    PLAYER_FIELD_NEXT_LEVEL_XP = 1385, // SIZE:1
    PLAYER_FIELD_SKILL = 1386, // SIZE:448
    PLAYER_FIELD_CHARACTER_POINTS = 1834, // SIZE:1
    PLAYER_FIELD_MAX_TALENT_TIERS = 1835, // SIZE:1
    PLAYER_FIELD_TRACK_CREATURE_MASK = 1836, // SIZE:1
    PLAYER_FIELD_TRACK_RESOURCE_MASK = 1837, // SIZE:1
    PLAYER_FIELD_MAINHAND_EXPERTISE = 1838, // SIZE:1
    PLAYER_FIELD_OFFHAND_EXPERTISE = 1839, // SIZE:1
    PLAYER_FIELD_RANGED_EXPERTISE = 1840, // SIZE:1
    PLAYER_FIELD_COMBAT_RATING_EXPERTISE = 1841, // SIZE:1
    PLAYER_FIELD_BLOCK_PERCENTAGE = 1842, // SIZE:1
    PLAYER_FIELD_DODGE_PERCENTAGE = 1843, // SIZE:1
    PLAYER_FIELD_PARRY_PERCENTAGE = 1844, // SIZE:1
    PLAYER_FIELD_CRIT_PERCENTAGE = 1845, // SIZE:1
    PLAYER_FIELD_RANGED_CRIT_PERCENTAGE = 1846, // SIZE:1
    PLAYER_FIELD_OFFHAND_CRIT_PERCENTAGE = 1847, // SIZE:1
    PLAYER_FIELD_SPELL_CRIT_PERCENTAGE = 1848, // SIZE:7
    PLAYER_FIELD_SHIELD_BLOCK = 1855, // SIZE:1
    PLAYER_FIELD_SHIELD_BLOCK_CRIT_PERCENTAGE = 1856, // SIZE:1
    PLAYER_FIELD_MASTERY = 1857, // SIZE:1
    PLAYER_FIELD_AMPLIFY = 1858, // SIZE:1
    PLAYER_FIELD_MULTISTRIKE = 1859, // SIZE:1
    PLAYER_FIELD_MULTISTRIKE_EFFECT = 1860, // SIZE:1
    PLAYER_FIELD_READINESS = 1861, // SIZE:1
    PLAYER_FIELD_SPEED = 1862, // SIZE:1
    PLAYER_FIELD_LIFESTEAL = 1863, // SIZE:1
    PLAYER_FIELD_AVOIDANCE = 1864, // SIZE:1
    PLAYER_FIELD_STURDINESS = 1865, // SIZE:1
    PLAYER_FIELD_CLEAVE = 1866, // SIZE:1
    PLAYER_FIELD_PVP_POWER_DAMAGE = 1867, // SIZE:1
    PLAYER_FIELD_PVP_POWER_HEALING = 1868, // SIZE:1
    PLAYER_FIELD_EXPLORED_ZONES = 1869, // SIZE:200
    PLAYER_FIELD_REST_STATE_BONUS_POOL = 2069, // SIZE:1
    PLAYER_FIELD_MOD_DAMAGE_DONE_POS = 2070, // SIZE:7
    PLAYER_FIELD_MOD_DAMAGE_DONE_NEG = 2077, // SIZE:7
    PLAYER_FIELD_MOD_DAMAGE_DONE_PERCENT = 2084, // SIZE:7
    PLAYER_FIELD_MOD_HEALING_DONE_POS = 2091, // SIZE:1
    PLAYER_FIELD_MOD_HEALING_PERCENT = 2092, // SIZE:1
    PLAYER_FIELD_MOD_HEALING_DONE_PERCENT = 2093, // SIZE:1
    PLAYER_FIELD_MOD_PERIODIC_HEALING_DONE_PERCENT = 2094, // SIZE:1
    PLAYER_FIELD_WEAPON_DMG_MULTIPLIERS = 2095, // SIZE:3
    PLAYER_FIELD_MOD_SPELL_POWER_PERCENT = 2098, // SIZE:1
    PLAYER_FIELD_MOD_RESILIENCE_PERCENT = 2099, // SIZE:1
    PLAYER_FIELD_OVERRIDE_SPELL_POWER_BY_APPERCENT = 2100, // SIZE:1
    PLAYER_FIELD_OVERRIDE_APBY_SPELL_POWER_PERCENT = 2101, // SIZE:1
    PLAYER_FIELD_MOD_TARGET_RESISTANCE = 2102, // SIZE:1
    PLAYER_FIELD_MOD_TARGET_PHYSICAL_RESISTANCE = 2103, // SIZE:1
    PLAYER_FIELD_LOCAL_FLAGS = 2104, // SIZE:1
    PLAYER_FIELD_LIFETIME_MAX_RANK = 2105, // SIZE:1
    PLAYER_FIELD_SELF_RES_SPELL = 2106, // SIZE:1
    PLAYER_FIELD_PVP_MEDALS = 2107, // SIZE:1
    PLAYER_FIELD_BUYBACK_PRICE = 2108, // SIZE:12
    PLAYER_FIELD_BUYBACK_TIMESTAMP = 2120, // SIZE:12
    PLAYER_FIELD_YESTERDAY_HONORABLE_KILLS = 2132, // SIZE:1
    PLAYER_FIELD_LIFETIME_HONORABLE_KILLS = 2133, // SIZE:1
    PLAYER_FIELD_WATCHED_FACTION_INDEX = 2134, // SIZE:1
    PLAYER_FIELD_COMBAT_RATINGS = 2135, // SIZE:32
    PLAYER_FIELD_PVP_INFO = 2167, // SIZE:36
    PLAYER_FIELD_MAX_LEVEL = 2203, // SIZE:1
    PLAYER_FIELD_RUNE_REGEN = 2204, // SIZE:4
    PLAYER_FIELD_NO_REAGENT_COST_MASK = 2208, // SIZE:4
    PLAYER_FIELD_GLYPH_SLOTS = 2212, // SIZE:6
    PLAYER_FIELD_GLYPHS = 2218, // SIZE:6
    PLAYER_FIELD_GLYPH_SLOTS_ENABLED = 2224, // SIZE:1
    PLAYER_FIELD_PET_SPELL_POWER = 2225, // SIZE:1
    PLAYER_FIELD_RESEARCHING = 2226, // SIZE:10
    PLAYER_FIELD_PROFESSION_SKILL_LINE = 2236, // SIZE:2
    PLAYER_FIELD_UI_HIT_MODIFIER = 2238, // SIZE:1
    PLAYER_FIELD_UI_SPELL_HIT_MODIFIER = 2239, // SIZE:1
    PLAYER_FIELD_HOME_REALM_TIME_OFFSET = 2240, // SIZE:1
    PLAYER_FIELD_MOD_PET_HASTE = 2241, // SIZE:1
    PLAYER_FIELD_SUMMONED_BATTLE_PET_GUID = 2242, // SIZE:2
    PLAYER_FIELD_OVERRIDE_SPELLS_ID = 2244, // SIZE:1
    PLAYER_FIELD_LFG_BONUS_FACTION_ID = 2245, // SIZE:1
    PLAYER_FIELD_LOOT_SPEC_ID = 2246, // SIZE:1
    PLAYER_FIELD_OVERRIDE_ZONE_PVPTYPE = 2247, // SIZE:1
    PLAYER_FIELD_ITEM_LEVEL_DELTA = 2248, // SIZE:1
    PLAYER_FIELD_BAG_SLOT_FLAGS = 2249, // SIZE:4
    PLAYER_FIELD_BANK_AUTO_SORT_DISABLED = 2253, // SIZE:1
    PLAYER_FIELD_BANK_BAG_SLOT_AUTO_SORT_DISABLED = 2254, // SIZE:1
    PLAYER_END = 2255
};

// > Object > GameObject
enum EGameObjectFields
{
    GAMEOBJECT_FIELD_CREATED_BY = 8, // SIZE:2
    GAMEOBJECT_FIELD_DISPLAY_ID = 10, // SIZE:1
    GAMEOBJECT_FIELD_FLAGS = 11, // SIZE:1
    GAMEOBJECT_FIELD_PARENT_ROTATION = 12, // SIZE:4
    GAMEOBJECT_FIELD_FACTION_TEMPLATE = 16, // SIZE:1
    GAMEOBJECT_FIELD_LEVEL = 17, // SIZE:1
    GAMEOBJECT_FIELD_PERCENT_HEALTH = 18, // SIZE:1
    GAMEOBJECT_FIELD_SPELL_VISUAL_ID = 19, // SIZE:1
    GAMEOBJECT_FIELD_STATE_SPELL_VISUAL_ID = 20, // SIZE:1
    GAMEOBJECT_FIELD_STATE_ANIM_ID = 21, // SIZE:1
    GAMEOBJECT_FIELD_STATE_ANIM_KIT_ID = 22, // SIZE:1
    GAMEOBJECT_FIELD_STATE_WORLD_EFFECT_ID = 23, // SIZE:4
    GAMEOBJECT_END = 30
};

// > Object > DynamicObject
enum EDynamicObjectFields
{
    DYNAMICOBJECT_FIELD_CASTER = 8, // SIZE:2
    DYNAMICOBJECT_FIELD_TYPE_AND_VISUAL_ID = 10, // SIZE:1
    DYNAMICOBJECT_FIELD_SPELL_ID = 11, // SIZE:1
    DYNAMICOBJECT_FIELD_RADIUS = 12, // SIZE:1
    DYNAMICOBJECT_FIELD_CAST_TIME = 13, // SIZE:1
    DYNAMICOBJECT_END = 14
};

// > Object > Corpse
enum ECorpseFields
{
    CORPSE_FIELD_OWNER = 8, // SIZE:2
    CORPSE_FIELD_PARTY_GUID = 10, // SIZE:2
    CORPSE_FIELD_DISPLAY_ID = 12, // SIZE:1
    CORPSE_FIELD_ITEMS = 13, // SIZE:19
    CORPSE_FIELD_SKIN_ID = 32, // SIZE:1
    CORPSE_FIELD_FACIAL_HAIR_STYLE_ID = 33, // SIZE:1
    CORPSE_FIELD_FLAGS = 34, // SIZE:1
    CORPSE_FIELD_DYNAMIC_FLAGS = 35, // SIZE:1
    CORPSE_FIELD_FACTION_TEMPLATE = 36, // SIZE:1
    CORPSE_END = 37
};

// > Object > AreaTrigger
enum EAreaTriggerFields
{
    AREATRIGGER_FIELD_CASTER = 8, // SIZE:2
    AREATRIGGER_FIELD_DURATION = 10, // SIZE:1
    AREATRIGGER_FIELD_SPELL_ID = 11, // SIZE:1
    AREATRIGGER_FIELD_SPELL_VISUAL_ID = 12, // SIZE:1
    AREATRIGGER_FIELD_EXPLICIT_SCALE = 13, // SIZE:1
    AREATRIGGER_END = 14
};

// > Object > SceneObject
enum ESceneObjectFields
{
    SCENEOBJECT_FIELD_SCRIPT_PACKAGE_ID = 8, // SIZE:1
    SCENEOBJECT_FIELD_RND_SEED_VAL = 9, // SIZE:1
    SCENEOBJECT_FIELD_CREATED_BY = 10, // SIZE:2
    SCENEOBJECT_FIELD_SCENE_TYPE = 12, // SIZE:1
    SCENEOBJECT_END = 13
};

// > Object > Conversation
enum EConversationFields
{
    CONVERSATION_FIELD_DUMMY = 8, // SIZE:1
    CONVERSATION_END = 9
};

enum EPlayerDynamicFields
{
    PLAYER_DYNAMIC_SPELLVISUAL_CHANGE = 0x0,
    PLAYER_DYNAMIC_ARCHEOLOGY_SITES = 0x2,
    PLAYER_DYNAMIC_DAILY_QUESTS_COMPLETED = 0x4,
    PLAYER_DYNAMIC_END = 0x8
};

enum ItemDynamicFields
{
    ITEM_DYNAMIC_MODIFIERS = 0x0,
    ITEM_DYNAMIC_END = 0x48
};

#define PLAYER_END_NOT_SELF PLAYER_FIELD_CURRENT_BATTLE_PET_BREED_QUALITY

#endif // _UPDATEFIELDS_H