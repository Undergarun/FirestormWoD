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

// Updatefields auto generated for version 5.0.4.16016
enum EObjectFields
{
    OBJECT_FIELD_GUID                                = 0x0,
    OBJECT_FIELD_DATA                                = 0x2,
    OBJECT_FIELD_TYPE                                = 0x4,
    OBJECT_FIELD_ENTRY                               = 0x5,
    OBJECT_FIELD_DYNAMIC_FLAGS                       = 0x6,
    OBJECT_FIELD_SCALE_X                             = 0x7,
    OBJECT_END                                       = 0x8
};

enum EUnitFields
{
    UNIT_FIELD_CHARM                                 = OBJECT_END + 0, // size 2, flags MIRROR_ALL
    UNIT_FIELD_SUMMON                                = OBJECT_END + 2, // size 2, flags MIRROR_ALL
    UNIT_FIELD_CRITTER                               = OBJECT_END + 4, // size 2, flags MIRROR_SELF
    UNIT_FIELD_CHARMEDBY                             = OBJECT_END + 6, // size 2, flags MIRROR_ALL
    UNIT_FIELD_SUMMONEDBY                            = OBJECT_END + 8, // size 2, flags MIRROR_ALL
    UNIT_FIELD_CREATEDBY                             = OBJECT_END + 10, // size 2, flags MIRROR_ALL
    UNIT_FIELD_DEMON_CREATOR                         = OBJECT_END + 12, // size 2, flags MIRROR_ALL
    UNIT_FIELD_TARGET                                = OBJECT_END + 14, // size 2, flags MIRROR_ALL
    UNIT_FIELD_BATTLE_PET_COMPANION_GUID             = OBJECT_END + 16, // size 2, flags MIRROR_ALL
    UNIT_FIELD_CHANNEL_OBJECT                        = OBJECT_END + 18, // size 2, flags MIRROR_ALL, MIRROR_URGENT
    UNIT_CHANNEL_SPELL                               = OBJECT_END + 20, // size 1, flags MIRROR_ALL, MIRROR_URGENT
    UNIT_FIELD_SUMMONED_BY_HOME_REALM                = OBJECT_END + 21, // size 1, flags MIRROR_ALL
    UNIT_FIELD_BYTES_0                               = OBJECT_END + 22, // size 1, flags MIRROR_ALL
    UNIT_FIELD_DISPLAY_POWER                         = OBJECT_END + 23, // size 1, flags MIRROR_ALL
    UNIT_FIELD_OVERRIDE_DISPLAY_POWER                = OBJECT_END + 24, // size 1, flags MIRROR_ALL
    UNIT_FIELD_HEALTH                                = OBJECT_END + 25, // size 1, flags MIRROR_ALL
    UNIT_FIELD_POWER1                                = OBJECT_END + 26, // size 5, flags MIRROR_ALL
    UNIT_FIELD_MAXHEALTH                             = OBJECT_END + 31, // size 1, flags MIRROR_ALL
    UNIT_FIELD_MAXPOWER1                             = OBJECT_END + 32, // size 5, flags MIRROR_ALL
    UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER             = OBJECT_END + 37, // size 5, flags MIRROR_SELF, MIRROR_OWNER, MIRROR_UNIT_ALL
    UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER = OBJECT_END + 42, // size 5, flags MIRROR_SELF, MIRROR_OWNER, MIRROR_UNIT_ALL
    UNIT_FIELD_LEVEL                                 = OBJECT_END + 47, // size 1, flags MIRROR_ALL
    UNIT_FIELD_EFFECTIVE_LEVEL                       = OBJECT_END + 48, // size 1, flags MIRROR_ALL
    UNIT_FIELD_FACTIONTEMPLATE                       = OBJECT_END + 49, // size 1, flags MIRROR_ALL
    UNIT_VIRTUAL_ITEM_SLOT_ID                        = OBJECT_END + 50, // size 3, flags MIRROR_ALL
    UNIT_FIELD_FLAGS                                 = OBJECT_END + 53, // size 1, flags MIRROR_ALL
    UNIT_FIELD_FLAGS_2                               = OBJECT_END + 54, // size 1, flags MIRROR_ALL
    UNIT_FIELD_AURASTATE                             = OBJECT_END + 55, // size 1, flags MIRROR_ALL
    UNIT_FIELD_BASEATTACKTIME                        = OBJECT_END + 56, // size 2, flags MIRROR_ALL
    UNIT_FIELD_RANGEDATTACKTIME                      = OBJECT_END + 58, // size 1, flags MIRROR_SELF
    UNIT_FIELD_BOUNDINGRADIUS                        = OBJECT_END + 59, // size 1, flags MIRROR_ALL
    UNIT_FIELD_COMBATREACH                           = OBJECT_END + 60, // size 1, flags MIRROR_ALL
    UNIT_FIELD_DISPLAYID                             = OBJECT_END + 61, // size 1, flags MIRROR_VIEWER_DEPENDENT, MIRROR_URGENT
    UNIT_FIELD_NATIVEDISPLAYID                       = OBJECT_END + 62, // size 1, flags MIRROR_ALL, MIRROR_URGENT
    UNIT_FIELD_MOUNTDISPLAYID                        = OBJECT_END + 63, // size 1, flags MIRROR_ALL, MIRROR_URGENT
    UNIT_FIELD_MINDAMAGE                             = OBJECT_END + 64, // size 1, flags MIRROR_SELF, MIRROR_OWNER, MIRROR_EMPATH
    UNIT_FIELD_MAXDAMAGE                             = OBJECT_END + 65, // size 1, flags MIRROR_SELF, MIRROR_OWNER, MIRROR_EMPATH
    UNIT_FIELD_MINOFFHANDDAMAGE                      = OBJECT_END + 66, // size 1, flags MIRROR_SELF, MIRROR_OWNER, MIRROR_EMPATH
    UNIT_FIELD_MAXOFFHANDDAMAGE                      = OBJECT_END + 67, // size 1, flags MIRROR_SELF, MIRROR_OWNER, MIRROR_EMPATH
    UNIT_FIELD_BYTES_1                               = OBJECT_END + 68, // size 1, flags MIRROR_ALL
    UNIT_FIELD_PETNUMBER                             = OBJECT_END + 69, // size 1, flags MIRROR_ALL
    UNIT_FIELD_PET_NAME_TIMESTAMP                    = OBJECT_END + 70, // size 1, flags MIRROR_ALL
    UNIT_FIELD_PETEXPERIENCE                         = OBJECT_END + 71, // size 1, flags MIRROR_OWNER
    UNIT_FIELD_PETNEXTLEVELEXP                       = OBJECT_END + 72, // size 1, flags MIRROR_OWNER
    UNIT_MOD_CAST_SPEED                              = OBJECT_END + 73, // size 1, flags MIRROR_ALL
    UNIT_MOD_CAST_HASTE                              = OBJECT_END + 74, // size 1, flags MIRROR_ALL
    UNIT_MOD_HASTE                                   = OBJECT_END + 75, // size 1, flags MIRROR_ALL
    UNIT_FIELD_MOD_RANGED_HASTE                      = OBJECT_END + 76, // size 1, flags MIRROR_ALL
    UNIT_MOD_HASTE_REGEN                             = OBJECT_END + 77, // size 1, flags MIRROR_ALL
    UNIT_CREATED_BY_SPELL                            = OBJECT_END + 78, // size 1, flags MIRROR_ALL
    UNIT_NPC_FLAGS                                   = OBJECT_END + 79, // size 2, flags MIRROR_ALL, MIRROR_VIEWER_DEPENDENT
    UNIT_NPC_EMOTESTATE                              = OBJECT_END + 81, // size 1, flags MIRROR_ALL
    UNIT_FIELD_STAT0                                 = OBJECT_END + 82, // size 5, flags MIRROR_SELF, MIRROR_OWNER
    UNIT_FIELD_POSSTAT0                              = OBJECT_END + 87, // size 5, flags MIRROR_SELF, MIRROR_OWNER
    UNIT_FIELD_NEGSTAT0                              = OBJECT_END + 92, // size 5, flags MIRROR_SELF, MIRROR_OWNER
    UNIT_FIELD_RESISTANCES                           = OBJECT_END + 97, // size 7, flags MIRROR_SELF, MIRROR_OWNER, MIRROR_EMPATH
    UNIT_FIELD_RESISTANCEBUFFMODSPOSITIVE            = OBJECT_END + 104, // size 7, flags MIRROR_SELF, MIRROR_OWNER
    UNIT_FIELD_RESISTANCEBUFFMODSNEGATIVE            = OBJECT_END + 111, // size 7, flags MIRROR_SELF, MIRROR_OWNER
    UNIT_FIELD_BASE_MANA                             = OBJECT_END + 118, // size 1, flags MIRROR_ALL
    UNIT_FIELD_BASE_HEALTH                           = OBJECT_END + 119, // size 1, flags MIRROR_SELF, MIRROR_OWNER
    UNIT_FIELD_BYTES_2                               = OBJECT_END + 120, // size 1, flags MIRROR_ALL
    UNIT_FIELD_ATTACK_POWER                          = OBJECT_END + 121, // size 1, flags MIRROR_SELF, MIRROR_OWNER
    UNIT_FIELD_ATTACK_POWER_MOD_POS                  = OBJECT_END + 122, // size 1, flags MIRROR_SELF, MIRROR_OWNER
    UNIT_FIELD_ATTACK_POWER_MOD_NEG                  = OBJECT_END + 123, // size 1, flags MIRROR_SELF, MIRROR_OWNER
    UNIT_FIELD_ATTACK_POWER_MULTIPLIER               = OBJECT_END + 124, // size 1, flags MIRROR_SELF, MIRROR_OWNER
    UNIT_FIELD_RANGED_ATTACK_POWER                   = OBJECT_END + 125, // size 1, flags MIRROR_SELF, MIRROR_OWNER
    UNIT_FIELD_RANGED_ATTACK_POWER_MOD_POS           = OBJECT_END + 126, // size 1, flags MIRROR_SELF, MIRROR_OWNER
    UNIT_FIELD_RANGED_ATTACK_POWER_MOD_NEG           = OBJECT_END + 127, // size 1, flags MIRROR_SELF, MIRROR_OWNER
    UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER        = OBJECT_END + 128, // size 1, flags MIRROR_SELF, MIRROR_OWNER
    UNIT_FIELD_MINRANGEDDAMAGE                       = OBJECT_END + 129, // size 1, flags MIRROR_SELF, MIRROR_OWNER
    UNIT_FIELD_MAXRANGEDDAMAGE                       = OBJECT_END + 130, // size 1, flags MIRROR_SELF, MIRROR_OWNER
    UNIT_FIELD_POWER_COST_MODIFIER                   = OBJECT_END + 131, // size 7, flags MIRROR_SELF, MIRROR_OWNER
    UNIT_FIELD_POWER_COST_MULTIPLIER                 = OBJECT_END + 138, // size 7, flags MIRROR_SELF, MIRROR_OWNER
    UNIT_FIELD_MAX_HEALTH_MODIFIER                   = OBJECT_END + 145, // size 1, flags MIRROR_SELF, MIRROR_OWNER
    UNIT_FIELD_HOVERHEIGHT                           = OBJECT_END + 146, // size 1, flags MIRROR_ALL
    UNIT_FIELD_MIN_ITEM_LEVEL                        = OBJECT_END + 147, // size 1, flags MIRROR_ALL
    UNIT_FIELD_MAX_ITEM_LEVEL                        = OBJECT_END + 148, // size 1, flags MIRROR_ALL
    UNIT_FIELD_WILD_BATTLE_PET_LEVEL                 = OBJECT_END + 149, // size 1, flags MIRROR_ALL
    UNIT_FIELD_BATTLE_PET_COMPANION_NAME_TIMESTAMP   = OBJECT_END + 150, // size 1, flags MIRROR_ALL
    UNIT_FIELD_INTERACT_SPELL_ID                     = OBJECT_END + 151, // size 1, flags MIRROR_ALL
    UNIT_END                                         = OBJECT_END + 152
};

enum EItemFields
{
    ITEM_FIELD_OWNER                                 = OBJECT_END + 0x0,
    ITEM_FIELD_CONTAINED                             = OBJECT_END + 0x2,
    ITEM_FIELD_CREATOR                               = OBJECT_END + 0x4,
    ITEM_FIELD_GIFTCREATOR                           = OBJECT_END + 0x6,
    ITEM_FIELD_STACK_COUNT                           = OBJECT_END + 0x8,
    ITEM_FIELD_DURATION                              = OBJECT_END + 0x9,
    ITEM_FIELD_SPELL_CHARGES                         = OBJECT_END + 0xA,
    ITEM_FIELD_FLAGS                                 = OBJECT_END + 0xF,
    ITEM_FIELD_ENCHANTMENT_1_1                       = OBJECT_END + 0x10,
    ITEM_FIELD_ENCHANTMENT_1_3                       = OBJECT_END + 0x12,
    ITEM_FIELD_ENCHANTMENT_2_1                       = OBJECT_END + 0x13,
    ITEM_FIELD_ENCHANTMENT_2_3                       = OBJECT_END + 0x15,
    ITEM_FIELD_ENCHANTMENT_3_1                       = OBJECT_END + 0x16,
    ITEM_FIELD_ENCHANTMENT_3_3                       = OBJECT_END + 0x18,
    ITEM_FIELD_ENCHANTMENT_4_1                       = OBJECT_END + 0x19,
    ITEM_FIELD_ENCHANTMENT_4_3                       = OBJECT_END + 0x1B,
    ITEM_FIELD_ENCHANTMENT_5_1                       = OBJECT_END + 0x1C,
    ITEM_FIELD_ENCHANTMENT_5_3                       = OBJECT_END + 0x1E,
    ITEM_FIELD_ENCHANTMENT_6_1                       = OBJECT_END + 0x1F,
    ITEM_FIELD_ENCHANTMENT_6_3                       = OBJECT_END + 0x21,
    ITEM_FIELD_ENCHANTMENT_7_1                       = OBJECT_END + 0x22,
    ITEM_FIELD_ENCHANTMENT_7_3                       = OBJECT_END + 0x24,
    ITEM_FIELD_ENCHANTMENT_8_1                       = OBJECT_END + 0x25,
    ITEM_FIELD_ENCHANTMENT_8_3                       = OBJECT_END + 0x27,
    ITEM_FIELD_ENCHANTMENT_9_1                       = OBJECT_END + 0x28,
    ITEM_FIELD_ENCHANTMENT_9_3                       = OBJECT_END + 0x2A,
    ITEM_FIELD_ENCHANTMENT_10_1                      = OBJECT_END + 0x2B,
    ITEM_FIELD_ENCHANTMENT_10_3                      = OBJECT_END + 0x2D,
    ITEM_FIELD_ENCHANTMENT_11_1                      = OBJECT_END + 0x2E,
    ITEM_FIELD_ENCHANTMENT_11_3                      = OBJECT_END + 0x30,
    ITEM_FIELD_ENCHANTMENT_12_1                      = OBJECT_END + 0x31,
    ITEM_FIELD_ENCHANTMENT_12_3                      = OBJECT_END + 0x33,
    ITEM_FIELD_ENCHANTMENT_13_1                      = OBJECT_END + 0x34,
    ITEM_FIELD_ENCHANTMENT_13_3                      = OBJECT_END + 0x36,
    ITEM_FIELD_PROPERTY_SEED                         = OBJECT_END + 0x37,
    ITEM_FIELD_RANDOM_PROPERTIES_ID                  = OBJECT_END + 0x38,
    ITEM_FIELD_DURABILITY                            = OBJECT_END + 0x39,
    ITEM_FIELD_MAXDURABILITY                         = OBJECT_END + 0x3A,
    ITEM_FIELD_CREATE_PLAYED_TIME                    = OBJECT_END + 0x3B,
    ITEM_FIELD_MODIFIERS_MASK                        = OBJECT_END + 0x3C,
    ITEM_END                                         = OBJECT_END + 0x3D
};

enum EPlayerFields
{
    PLAYER_DUEL_ARBITER                              = UNIT_END + 0, // size 2, flags MIRROR_ALL
    PLAYER_FLAGS                                     = UNIT_END + 2, // size 1, flags MIRROR_ALL
    PLAYER_GUILDRANK                                 = UNIT_END + 3, // size 1, flags MIRROR_ALL
    PLAYER_FIELD_GUILD_DELETE_DATE                   = UNIT_END + 4, // size 1, flags MIRROR_ALL
    PLAYER_GUILDLEVEL                                = UNIT_END + 5, // size 1, flags MIRROR_ALL
    PLAYER_FIELD_BYTES                               = UNIT_END + 0x6,  // size 1, flags MIRROR_ALL
    PLAYER_BYTES_2                                   = UNIT_END + 0x7,// size 1, flags MIRROR_ALL
    PLAYER_BYTES_3                                   = UNIT_END + 0x8,// size 1, flags MIRROR_ALL
    PLAYER_DUEL_TEAM                                 = UNIT_END + 9, // size 1, flags MIRROR_ALL
    PLAYER_GUILD_TIMESTAMP                           = UNIT_END + 10, // size 1, flags MIRROR_ALL
    PLAYER_QUEST_LOG_1_1                             = UNIT_END + 11, // size 750, flags MIRROR_PARTY
    //PLAYER_FIELD_VISIBLE_ITEMS                     = UNIT_END + 761, // size 38, flags MIRROR_ALL
    PLAYER_VISIBLE_ITEM_1_ENTRYID                    = UNIT_END + 0x2F9,
    PLAYER_VISIBLE_ITEM_1_ENCHANTMENT                = UNIT_END + 0x2FA,
    PLAYER_VISIBLE_ITEM_2_ENTRYID                    = UNIT_END + 0x2FB,
    PLAYER_VISIBLE_ITEM_2_ENCHANTMENT                = UNIT_END + 0x2FC,
    PLAYER_VISIBLE_ITEM_3_ENTRYID                    = UNIT_END + 0x2FD,
    PLAYER_VISIBLE_ITEM_3_ENCHANTMENT                = UNIT_END + 0x2FE,
    PLAYER_VISIBLE_ITEM_4_ENTRYID                    = UNIT_END + 0x2FF,
    PLAYER_VISIBLE_ITEM_4_ENCHANTMENT                = UNIT_END + 0x300,
    PLAYER_VISIBLE_ITEM_5_ENTRYID                    = UNIT_END + 0x301,
    PLAYER_VISIBLE_ITEM_5_ENCHANTMENT                = UNIT_END + 0x302,
    PLAYER_VISIBLE_ITEM_6_ENTRYID                    = UNIT_END + 0x303,
    PLAYER_VISIBLE_ITEM_6_ENCHANTMENT                = UNIT_END + 0x304,
    PLAYER_VISIBLE_ITEM_7_ENTRYID                    = UNIT_END + 0x305,
    PLAYER_VISIBLE_ITEM_7_ENCHANTMENT                = UNIT_END + 0x306,
    PLAYER_VISIBLE_ITEM_8_ENTRYID                    = UNIT_END + 0x307,
    PLAYER_VISIBLE_ITEM_8_ENCHANTMENT                = UNIT_END + 0x308,
    PLAYER_VISIBLE_ITEM_9_ENTRYID                    = UNIT_END + 0x309,
    PLAYER_VISIBLE_ITEM_9_ENCHANTMENT                = UNIT_END + 0x30A,
    PLAYER_VISIBLE_ITEM_10_ENTRYID                   = UNIT_END + 0x30B,
    PLAYER_VISIBLE_ITEM_10_ENCHANTMENT               = UNIT_END + 0x30C,
    PLAYER_VISIBLE_ITEM_11_ENTRYID                   = UNIT_END + 0x30D,
    PLAYER_VISIBLE_ITEM_11_ENCHANTMENT               = UNIT_END + 0x30E,
    PLAYER_VISIBLE_ITEM_12_ENTRYID                   = UNIT_END + 0x30F,
    PLAYER_VISIBLE_ITEM_12_ENCHANTMENT               = UNIT_END + 0x310,
    PLAYER_VISIBLE_ITEM_13_ENTRYID                   = UNIT_END + 0x311,
    PLAYER_VISIBLE_ITEM_13_ENCHANTMENT               = UNIT_END + 0x312,
    PLAYER_VISIBLE_ITEM_14_ENTRYID                   = UNIT_END + 0x313,
    PLAYER_VISIBLE_ITEM_14_ENCHANTMENT               = UNIT_END + 0x314,
    PLAYER_VISIBLE_ITEM_15_ENTRYID                   = UNIT_END + 0x315,
    PLAYER_VISIBLE_ITEM_15_ENCHANTMENT               = UNIT_END + 0x316,
    PLAYER_VISIBLE_ITEM_16_ENTRYID                   = UNIT_END + 0x317,
    PLAYER_VISIBLE_ITEM_16_ENCHANTMENT               = UNIT_END + 0x318,
    PLAYER_VISIBLE_ITEM_17_ENTRYID                   = UNIT_END + 0x319,
    PLAYER_VISIBLE_ITEM_17_ENCHANTMENT               = UNIT_END + 0x31A,
    PLAYER_VISIBLE_ITEM_18_ENTRYID                   = UNIT_END + 0x31B,
    PLAYER_VISIBLE_ITEM_18_ENCHANTMENT               = UNIT_END + 0x31C,
    PLAYER_VISIBLE_ITEM_19_ENTRYID                   = UNIT_END + 0x31D,
    PLAYER_VISIBLE_ITEM_19_ENCHANTMENT               = UNIT_END + 0x31E,
    PLAYER_CHOSEN_TITLE                              = UNIT_END + 799, // size 1, flags MIRROR_ALL
    PLAYER_FAKE_INEBRIATION                          = UNIT_END + 800, // size 1, flags MIRROR_ALL
    PLAYER_FIELD_VIRTUAL_PLAYER_REALM                = UNIT_END + 801, // size 1, flags MIRROR_ALL
    PLAYER_CURRENT_SPEC_ID                           = UNIT_END + 802, // size 1, flags MIRROR_ALL
    PLAYER_FIELD_TAXI_MOUNT_ANIM_KIT_ID              = UNIT_END + 803, // size 1, flags MIRROR_ALL
    PLAYER_FIELD_CURRENT_BATTLE_PET_BREED_QUALITY    = UNIT_END + 804, // size 1, flags MIRROR_ALL
    PLAYER_FIELD_INV_SLOT_HEAD                       = UNIT_END + 805, // size 172, flags MIRROR_SELF
    PLAYER_FIELD_VENDORBUYBACK_SLOT_1                = UNIT_END + 953,
    PLAYER_FARSIGHT                                  = UNIT_END + 977, // size 2, flags MIRROR_SELF
    PLAYER_FIELD_KNOWN_TITLES                        = UNIT_END + 979, // size 10, flags MIRROR_SELF
    PLAYER_FIELD_COINAGE                             = UNIT_END + 989, // size 2, flags MIRROR_SELF
    PLAYER_XP                                        = UNIT_END + 991, // size 1, flags MIRROR_SELF
    PLAYER_NEXT_LEVEL_XP                             = UNIT_END + 992, // size 1, flags MIRROR_SELF
    PLAYER_SKILL_LINEID_0                            = UNIT_END + 993,
    PLAYER_SKILL_STEP_0                              = UNIT_END + 1057,
    PLAYER_SKILL_RANK_0                              = UNIT_END + 1121,
    PLAYER_SKILL_MODIFIER_0                          = UNIT_END + 1185,
    PLAYER_SKILL_MAX_RANK_0                          = UNIT_END + 1249,
    PLAYER_SKILL_TALENT_0                            = UNIT_END + 1313,
    PLAYER_SKILL_UNKNOWN                             = UNIT_END + 1377,
    PLAYER_CHARACTER_POINTS                          = UNIT_END + 1441, // size 1, flags MIRROR_SELF
    PLAYER_MAX_TALENT_TIERS                          = UNIT_END + 1442, // size 1, flags MIRROR_SELF
    PLAYER_TRACK_CREATURES                           = UNIT_END + 1443, // size 1, flags MIRROR_SELF
    PLAYER_TRACK_RESOURCES                           = UNIT_END + 1444, // size 1, flags MIRROR_SELF
    PLAYER_EXPERTISE                                 = UNIT_END + 1445, // size 1, flags MIRROR_SELF
    PLAYER_OFFHAND_EXPERTISE                         = UNIT_END + 1446, // size 1, flags MIRROR_SELF
    PLAYER_RANGED_EXPERTISE                          = UNIT_END + 1447, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_COMBAT_RATING_EXPERTISE             = UNIT_END + 1448, // size 1, flags MIRROR_SELF
    PLAYER_BLOCK_PERCENTAGE                          = UNIT_END + 1449, // size 1, flags MIRROR_SELF
    PLAYER_DODGE_PERCENTAGE                          = UNIT_END + 1450, // size 1, flags MIRROR_SELF
    PLAYER_PARRY_PERCENTAGE                          = UNIT_END + 1451, // size 1, flags MIRROR_SELF
    PLAYER_CRIT_PERCENTAGE                           = UNIT_END + 1452, // size 1, flags MIRROR_SELF
    PLAYER_RANGED_CRIT_PERCENTAGE                    = UNIT_END + 1453, // size 1, flags MIRROR_SELF
    PLAYER_OFFHAND_CRIT_PERCENTAGE                   = UNIT_END + 1454, // size 1, flags MIRROR_SELF
    PLAYER_SPELL_CRIT_PERCENTAGE1                    = UNIT_END + 1455, // size 7, flags MIRROR_SELF
    PLAYER_SHIELD_BLOCK                              = UNIT_END + 1462, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_SHIELD_BLOCK_CRIT_PERCENTAGE        = UNIT_END + 1463, // size 1, flags MIRROR_SELF
    PLAYER_MASTERY                                   = UNIT_END + 1464, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_PVP_POWER_DAMAGE                    = UNIT_END + 1465, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_PVP_POWER_HEALING                   = UNIT_END + 1466, // size 1, flags MIRROR_SELF
    PLAYER_EXPLORED_ZONES_1                          = UNIT_END + 1467, // size 200, flags MIRROR_SELF
    PLAYER_FIELD_REST_STATE_BONUS_POOL               = UNIT_END + 1667, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_MOD_DAMAGE_DONE_POS                 = UNIT_END + 1668, // size 7, flags MIRROR_SELF
    PLAYER_FIELD_MOD_DAMAGE_DONE_NEG                 = UNIT_END + 1675, // size 7, flags MIRROR_SELF
    PLAYER_FIELD_MOD_DAMAGE_DONE_PCT                 = UNIT_END + 1682, // size 7, flags MIRROR_SELF
    PLAYER_FIELD_MOD_HEALING_DONE_POS                = UNIT_END + 1689, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_MOD_HEALING_PERCENT                 = UNIT_END + 1690, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_MOD_HEALING_DONE_PCT                = UNIT_END + 1691, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_MOD_PERIODIC_HEALING_DONE_PERCENT   = UNIT_END + 1692, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_WEAPON_DMG_MULTIPLIERS              = UNIT_END + 1693, // size 3, flags MIRROR_SELF
    PLAYER_FIELD_MOD_SPELL_POWER_PCT                 = UNIT_END + 1696, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_MOD_RESILIENCE_PCT                  = UNIT_END + 1697, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_OVERRIDE_SPELL_POWER_BY_AP_PCT      = UNIT_END + 1698, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_OVERRIDE_AP_BY_SPELL_POWER_PCT      = UNIT_END + 1699, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_MOD_TARGET_RESISTANCE               = UNIT_END + 1700, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_MOD_TARGET_PHYSICAL_RESISTANCE      = UNIT_END + 1701, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_LIFETIME_MAX_RANK                   = UNIT_END + 1702, // size 1, flags MIRROR_SELF
    PLAYER_SELF_RES_SPELL                            = UNIT_END + 1703, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_PVP_MEDALS                          = UNIT_END + 1704, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_BUYBACK_PRICE_1                     = UNIT_END + 1705, // size 12, flags MIRROR_SELF
    PLAYER_FIELD_BUYBACK_TIMESTAMP_1                 = UNIT_END + 1717, // size 12, flags MIRROR_SELF
    PLAYER_FIELD_KILLS                               = UNIT_END + 1729, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_LIFETIME_HONORABLE_KILLS            = UNIT_END + 1730, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_WATCHED_FACTION_INDEX               = UNIT_END + 1731, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_COMBAT_RATING_1                     = UNIT_END + 1732, // size 27, flags MIRROR_SELF
    PLAYER_FIELD_PVP_INFO                            = UNIT_END + 1759, // size 24, flags MIRROR_SELF
    PLAYER_FIELD_MAX_LEVEL                           = UNIT_END + 1783, // size 1, flags MIRROR_SELF
    PLAYER_RUNE_REGEN_1                              = UNIT_END + 1784, // size 4, flags MIRROR_SELF
    PLAYER_NO_REAGENT_COST_1                         = UNIT_END + 1788, // size 4, flags MIRROR_SELF
    PLAYER_FIELD_GLYPH_SLOTS_1                       = UNIT_END + 1792, // size 6, flags MIRROR_SELF
    PLAYER_FIELD_GLYPHS_1                            = UNIT_END + 1798, // size 6, flags MIRROR_SELF
    PLAYER_GLYPHS_ENABLED                            = UNIT_END + 1804, // size 1, flags MIRROR_SELF
    PLAYER_PET_SPELL_POWER                           = UNIT_END + 1805, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_RESEARCHING                         = UNIT_END + 1806, // size 8, flags MIRROR_SELF
    PLAYER_PROFESSION_SKILL_LINE_1                   = UNIT_END + 1814, // size 2, flags MIRROR_SELF
    PLAYER_FIELD_UI_HIT_MODIFIER                     = UNIT_END + 1816, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_UI_SPELL_HIT_MODIFIER               = UNIT_END + 1817, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_HOME_REALM_TIME_OFFSET              = UNIT_END + 1818, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_MOD_PET_HASTE                       = UNIT_END + 1819, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_SUMMONED_BATTLE_PET_GUID            = UNIT_END + 1820, // size 2, flags MIRROR_SELF
    PLAYER_FIELD_OVERRIDE_SPELLS_ID                  = UNIT_END + 1822, // size 1, flags MIRROR_SELF, MIRROR_URGENT_SELF_ONLY
    PLAYER_FIELD_LFG_BONUS_FACTION_ID                = UNIT_END + 1823, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_LOOT_SPEC_ID                        = UNIT_END + 1824, // size 1, flags MIRROR_SELF
    PLAYER_FIELD_OVERRIDE_ZONE_PVP_TYPE              = UNIT_END + 1825, // size 1, flags MIRROR_SELF, MIRROR_URGENT_SELF_ONLY
    PLAYER_FIELD_ITEM_LEVEL_DELTA                    = UNIT_END + 1826, // size 1, flags MIRROR_SELF
    PLAYER_END                                       = UNIT_END + 1827
};

enum EPlayerDynamicFields
{
    PLAYER_DYNAMIC_SPELLVISUAL_CHANGE                = 0x0,
    PLAYER_DYNAMIC_ARCHEOLOGY_SITES                  = 0x2,
    PLAYER_DYNAMIC_DAILY_QUESTS_COMPLETED            = 0x4,
    PLAYER_DYNAMIC_END                               = 0x8
}; 

enum ItemDynamicFields
{
    ITEM_DYNAMIC_MODIFIERS                           = 0x0,
    ITEM_DYNAMIC_END                                 = 0x48
};

enum EContainerFields
{
    CONTAINER_FIELD_SLOT_1                           = ITEM_END + 0x0,
    CONTAINER_FIELD_NUM_SLOTS                        = ITEM_END + 0x48,
    CONTAINER_END                                    = ITEM_END + 0x49,
};

enum EGameObjectFields
{
    OBJECT_FIELD_CREATED_BY                          = OBJECT_END + 0x0,
    GAMEOBJECT_DISPLAYID                             = OBJECT_END + 0x2,
    GAMEOBJECT_FLAGS                                 = OBJECT_END + 0x3,
    GAMEOBJECT_PARENTROTATION                        = OBJECT_END + 0x4,
    GAMEOBJECT_FACTION                               = OBJECT_END + 0x8,
    GAMEOBJECT_LEVEL                                 = OBJECT_END + 0x9,
    GAMEOBJECT_BYTES_1                               = OBJECT_END + 0xA,
    GAMEOBJECT_FIELD_ANIM_PROGRESS                   = OBJECT_END + 0xB,
    GAMEOBJECT_END                                   = OBJECT_END + 0xC
};

enum EDynamicObjectFields
{
    DYNAMICOBJECT_CASTER                             = OBJECT_END + 0x0,
    DYNAMICOBJECT_FIELD_TYPE_AND_VISUAL_ID           = OBJECT_END + 0x2,
    DYNAMICOBJECT_SPELLID                            = OBJECT_END + 0x3,
    DYNAMICOBJECT_RADIUS                             = OBJECT_END + 0x4,
    DYNAMICOBJECT_CASTTIME                           = OBJECT_END + 0x5,
    DYNAMICOBJECT_END                                = OBJECT_END + 0x6
};

enum ECorpseFields
{
    CORPSE_FIELD_OWNER                               = OBJECT_END + 0x0,
    CORPSE_FIELD_PARTY                               = OBJECT_END + 0x2,
    CORPSE_FIELD_DISPLAY_ID                          = OBJECT_END + 0x4,
    CORPSE_FIELD_ITEM                                = OBJECT_END + 0x5,
    CORPSE_FIELD_BYTES_1                             = OBJECT_END + 0x18,
    CORPSE_FIELD_BYTES_2                             = OBJECT_END + 0x19,
    CORPSE_FIELD_FLAGS                               = OBJECT_END + 0x1A,
    CORPSE_FIELD_DYNAMIC_FLAGS                       = OBJECT_END + 0x1B,
    CORPSE_END                                       = OBJECT_END + 0x1C
};

enum EAreaTriggerFields
{
    AREATRIGGER_CASTER                               = OBJECT_END + 0x0,
    AREATRIGGER_DURATION                             = OBJECT_END + 0x2,
    AREATRIGGER_SPELLID                              = OBJECT_END + 0x3,
    AREATRIGGER_SPELLVISUALID                        = OBJECT_END + 0x4,
    AREATRIGGER_FIELD_EXPLICIT_SCALE                 = OBJECT_END + 0x5,
    AREATRIGGER_END                                  = OBJECT_END + 0x6
};

enum SceneObjectFields
{
    SCENEOBJECT_FIELD_SCRIPT_PACKAGE_ID             = 8,
    SCENEOBJECT_FIELD_RND_SEED_VAL                  = 9,
    SCENEOBJECT_FIELD_CREATED_BY                    = 10,
    SCENEOBJECT_FIELD_SCENE_TYPE                    = 12,
    SCENEOBJECT_END                                 = 13
};

#define PLAYER_END_NOT_SELF PLAYER_FAKE_INEBRIATION

#endif // _UPDATEFIELDS_H