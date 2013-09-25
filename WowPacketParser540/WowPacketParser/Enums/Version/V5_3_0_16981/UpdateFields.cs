// 5.4 updatefield !!!

namespace WowPacketParser.Enums.Version.V5_3_0_16981
{
    // ReSharper disable InconsistentNaming
    // 5.3.0.16981
    public enum ObjectField
    {
        OBJECT_FIELD_GUID                                = 0x0000, // Size =   2, Type: Flags PUBLIC
        OBJECT_FIELD_DATA                                = 0x0002, // Size =   2, Type: Flags PUBLIC
        OBJECT_FIELD_TYPE                                = 0x0004, // Size =   1, Type: Flags PUBLIC
        OBJECT_FIELD_ENTRY                               = 0x0005, // Size =   1, Type: Flags UNUSED2
        OBJECT_FIELD_DYNAMIC_FLAGS                       = 0x0006, // Size =   1, Type: Flags UNUSED2 | DYNAMIC
        OBJECT_FIELD_SCALE_X                             = 0x0007, // Size =   1, Type: Flags PUBLIC
        OBJECT_END                                       = 0x0008,
    };

    public enum ItemField
    {
        ITEM_FIELD_OWNER                                 = ObjectField.OBJECT_END + 0x0000, // Size =   2, Type: Flags PUBLIC
        ITEM_FIELD_CONTAINED_IN                          = ObjectField.OBJECT_END + 0x0002, // Size =   2, Type: Flags PUBLIC
        ITEM_FIELD_CREATOR                               = ObjectField.OBJECT_END + 0x0004, // Size =   2, Type: Flags PUBLIC
        ITEM_FIELD_GIFT_CREATOR                          = ObjectField.OBJECT_END + 0x0006, // Size =   2, Type: Flags PUBLIC
        ITEM_FIELD_STACK_COUNT                           = ObjectField.OBJECT_END + 0x0008, // Size =   1, Type: Flags OWNER
        ITEM_FIELD_EXPIRATION                            = ObjectField.OBJECT_END + 0x0009, // Size =   1, Type: Flags OWNER
        ITEM_FIELD_SPELL_CHARGES                         = ObjectField.OBJECT_END + 0x000A, // Size =   5, Type: Flags OWNER
        ITEM_FIELD_DYNAMIC_FLAGS                         = ObjectField.OBJECT_END + 0x000F, // Size =   1, Type: Flags PUBLIC
        ITEM_FIELD_ENCHANTMENT                           = ObjectField.OBJECT_END + 0x0010, // Size =  39, Type: Flags PUBLIC
        ITEM_FIELD_PROPERTY_SEED                         = ObjectField.OBJECT_END + 0x0037, // Size =   1, Type: Flags PUBLIC
        ITEM_FIELD_RANDOM_PROPERTIES_ID                  = ObjectField.OBJECT_END + 0x0038, // Size =   1, Type: Flags PUBLIC
        ITEM_FIELD_DURABILITY                            = ObjectField.OBJECT_END + 0x0039, // Size =   1, Type: Flags OWNER
        ITEM_FIELD_MAX_DURABILITY                        = ObjectField.OBJECT_END + 0x003A, // Size =   1, Type: Flags OWNER
        ITEM_FIELD_CREATE_PLAYED_TIME                    = ObjectField.OBJECT_END + 0x003B, // Size =   1, Type: Flags PUBLIC
        ITEM_FIELD_MODIFIERS_MASK                        = ObjectField.OBJECT_END + 0x003C, // Size =   1, Type: Flags OWNER
        ITEM_END                                         = ObjectField.OBJECT_END + 0x003D,
    };


    public enum ItemDynamicField
    {
        ITEM_DYNAMIC_FIELD_MODIFIERS                     = ItemField.ITEM_END + 0x0,
        ITEM_DYNAMIC_END                                 = ItemField.ITEM_END + 0x48
    }

    public enum ContainerField
    {
        CONTAINER_FIELD_SLOTS                            = ItemField.ITEM_END + 0x0000, // Size =  72, Type: Flags PUBLIC
        CONTAINER_FIELD_NUM_SLOTS                        = ItemField.ITEM_END + 0x0048, // Size =   1, Type: Flags PUBLIC
        CONTAINER_END                                    = ItemField.ITEM_END + 0x0049,
    };


    public enum UnitField
    {
        UNIT_FIELD_CHARM = ObjectField.OBJECT_END + 0, // size 2, flags MIRROR_ALL
        UNIT_FIELD_SUMMON = ObjectField.OBJECT_END + 2, // size 2, flags MIRROR_ALL
        UNIT_FIELD_CRITTER = ObjectField.OBJECT_END + 4, // size 2, flags MIRROR_SELF
        UNIT_FIELD_CHARMED_BY = ObjectField.OBJECT_END + 6, // size 2, flags MIRROR_ALL
        UNIT_FIELD_SUMMONED_BY = ObjectField.OBJECT_END + 8, // size 2, flags MIRROR_ALL
        UNIT_FIELD_CREATED_BY = ObjectField.OBJECT_END + 10, // size 2, flags MIRROR_ALL
        UNIT_FIELD_DEMON_CREATOR = ObjectField.OBJECT_END + 12, // size 2, flags MIRROR_ALL
        UNIT_FIELD_TARGET = ObjectField.OBJECT_END + 14, // size 2, flags MIRROR_ALL
        UNIT_FIELD_BATTLE_PET_COMPANION_GUID = ObjectField.OBJECT_END + 16, // size 2, flags MIRROR_ALL
        UNIT_FIELD_CHANNEL_OBJECT = ObjectField.OBJECT_END + 18, // size 2, flags MIRROR_ALL, MIRROR_URGENT
        UNIT_FIELD_CHANNEL_SPELL = ObjectField.OBJECT_END + 20, // size 1, flags MIRROR_ALL, MIRROR_URGENT
        UNIT_FIELD_SUMMONED_BY_HOME_REALM = ObjectField.OBJECT_END + 21, // size 1, flags MIRROR_ALL
        UNIT_FIELD_SEX = ObjectField.OBJECT_END + 22, // size 1, flags MIRROR_ALL
        UNIT_FIELD_DISPLAY_POWER = ObjectField.OBJECT_END + 23, // size 1, flags MIRROR_ALL
        UNIT_FIELD_OVERRIDE_DISPLAY_POWER_I_D = ObjectField.OBJECT_END + 24, // size 1, flags MIRROR_ALL
        UNIT_FIELD_HEALTH = ObjectField.OBJECT_END + 25, // size 1, flags MIRROR_ALL
        UNIT_FIELD_POWER = ObjectField.OBJECT_END + 26, // size 5, flags MIRROR_ALL
        UNIT_FIELD_MAX_HEALTH = ObjectField.OBJECT_END + 31, // size 1, flags MIRROR_ALL
        UNIT_FIELD_MAX_POWER = ObjectField.OBJECT_END + 32, // size 5, flags MIRROR_ALL
        UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER = ObjectField.OBJECT_END + 37, // size 5, flags MIRROR_SELF, MIRROR_OWNER, MIRROR_UNIT_ALL
        UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER = ObjectField.OBJECT_END + 42, // size 5, flags MIRROR_SELF, MIRROR_OWNER, MIRROR_UNIT_ALL
        UNIT_FIELD_LEVEL = ObjectField.OBJECT_END + 47, // size 1, flags MIRROR_ALL
        UNIT_FIELD_EFFECTIVE_LEVEL = ObjectField.OBJECT_END + 48, // size 1, flags MIRROR_ALL
        UNIT_FIELD_FACTION_TEMPLATE = ObjectField.OBJECT_END + 49, // size 1, flags MIRROR_ALL
        UNIT_FIELD_VIRTUAL_ITEM_ID = ObjectField.OBJECT_END + 50, // size 3, flags MIRROR_ALL
        UNIT_FIELD_FLAGS = ObjectField.OBJECT_END + 53, // size 1, flags MIRROR_ALL
        UNIT_FIELD_FLAGS_2 = ObjectField.OBJECT_END + 54, // size 1, flags MIRROR_ALL
        UNIT_FIELD_AURA_STATE = ObjectField.OBJECT_END + 55, // size 1, flags MIRROR_ALL
        UNIT_FIELD_ATTACK_ROUND_BASE_TIME = ObjectField.OBJECT_END + 56, // size 2, flags MIRROR_ALL
        UNIT_FIELD_RANGED_ATTACK_ROUND_BASE_TIME = ObjectField.OBJECT_END + 58, // size 1, flags MIRROR_SELF
        UNIT_FIELD_BOUNDING_RADIUS = ObjectField.OBJECT_END + 59, // size 1, flags MIRROR_ALL
        UNIT_FIELD_COMBAT_REACH = ObjectField.OBJECT_END + 60, // size 1, flags MIRROR_ALL
        UNIT_FIELD_DISPLAY_ID = ObjectField.OBJECT_END + 61, // size 1, flags MIRROR_VIEWER_DEPENDENT, MIRROR_URGENT
        UNIT_FIELD_NATIVE_DISPLAY_ID = ObjectField.OBJECT_END + 62, // size 1, flags MIRROR_ALL, MIRROR_URGENT
        UNIT_FIELD_MOUNT_DISPLAY_ID = ObjectField.OBJECT_END + 63, // size 1, flags MIRROR_ALL, MIRROR_URGENT
        UNIT_FIELD_MIN_DAMAGE = ObjectField.OBJECT_END + 64, // size 1, flags MIRROR_SELF, MIRROR_OWNER, MIRROR_EMPATH
        UNIT_FIELD_MAX_DAMAGE = ObjectField.OBJECT_END + 65, // size 1, flags MIRROR_SELF, MIRROR_OWNER, MIRROR_EMPATH
        UNIT_FIELD_MIN_OFF_HAND_DAMAGE = ObjectField.OBJECT_END + 66, // size 1, flags MIRROR_SELF, MIRROR_OWNER, MIRROR_EMPATH
        UNIT_FIELD_MAX_OFF_HAND_DAMAGE = ObjectField.OBJECT_END + 67, // size 1, flags MIRROR_SELF, MIRROR_OWNER, MIRROR_EMPATH
        UNIT_FIELD_ANIM_TIER = ObjectField.OBJECT_END + 68, // size 1, flags MIRROR_ALL
        UNIT_FIELD_PET_NUMBER = ObjectField.OBJECT_END + 69, // size 1, flags MIRROR_ALL
        UNIT_FIELD_PET_NAME_TIMESTAMP = ObjectField.OBJECT_END + 70, // size 1, flags MIRROR_ALL
        UNIT_FIELD_PET_EXPERIENCE = ObjectField.OBJECT_END + 71, // size 1, flags MIRROR_OWNER
        UNIT_FIELD_PET_NEXT_LEVEL_EXPERIENCE = ObjectField.OBJECT_END + 72, // size 1, flags MIRROR_OWNER
        UNIT_FIELD_MOD_CASTING_SPEED = ObjectField.OBJECT_END + 73, // size 1, flags MIRROR_ALL
        UNIT_FIELD_MOD_SPELL_HASTE = ObjectField.OBJECT_END + 74, // size 1, flags MIRROR_ALL
        UNIT_FIELD_MOD_HASTE = ObjectField.OBJECT_END + 75, // size 1, flags MIRROR_ALL
        UNIT_FIELD_MOD_RANGED_HASTE = ObjectField.OBJECT_END + 76, // size 1, flags MIRROR_ALL
        UNIT_FIELD_MOD_HASTE_REGEN = ObjectField.OBJECT_END + 77, // size 1, flags MIRROR_ALL
        UNIT_CREATED_BY_SPELL = ObjectField.OBJECT_END + 78, // size 1, flags MIRROR_ALL
        UNIT_NPC_FLAGS = ObjectField.OBJECT_END + 79, // size 2, flags MIRROR_ALL, MIRROR_VIEWER_DEPENDENT
        UNIT_EMOTE_STATE = ObjectField.OBJECT_END + 81, // size 1, flags MIRROR_ALL
        UNIT_FIELD_STATS = ObjectField.OBJECT_END + 82, // size 5, flags MIRROR_SELF, MIRROR_OWNER
        UNIT_FIELD_STAT_POS_BUFF = ObjectField.OBJECT_END + 87, // size 5, flags MIRROR_SELF, MIRROR_OWNER
        UNIT_FIELD_STAT_NEG_BUFF = ObjectField.OBJECT_END + 92, // size 5, flags MIRROR_SELF, MIRROR_OWNER
        UNIT_FIELD_RESISTANCES = ObjectField.OBJECT_END + 97, // size 7, flags MIRROR_SELF, MIRROR_OWNER, MIRROR_EMPATH
        UNIT_FIELD_RESISTANCE_BUFF_MODS_POSITIVE = ObjectField.OBJECT_END + 104, // size 7, flags MIRROR_SELF, MIRROR_OWNER
        UNIT_FIELD_RESISTANCE_BUFF_MODS_NEGATIVE = ObjectField.OBJECT_END + 111, // size 7, flags MIRROR_SELF, MIRROR_OWNER
        UNIT_FIELD_BASE_MANA = ObjectField.OBJECT_END + 118, // size 1, flags MIRROR_ALL
        UNIT_FIELD_BASE_HEALTH = ObjectField.OBJECT_END + 119, // size 1, flags MIRROR_SELF, MIRROR_OWNER
        UNIT_FIELD_SHAPESHIFT_FORM = ObjectField.OBJECT_END + 120, // size 1, flags MIRROR_ALL
        UNIT_FIELD_ATTACK_POWER = ObjectField.OBJECT_END + 121, // size 1, flags MIRROR_SELF, MIRROR_OWNER
        UNIT_FIELD_ATTACK_POWER_MOD_POS = ObjectField.OBJECT_END + 122, // size 1, flags MIRROR_SELF, MIRROR_OWNER
        UNIT_FIELD_ATTACK_POWER_MOD_NEG = ObjectField.OBJECT_END + 123, // size 1, flags MIRROR_SELF, MIRROR_OWNER
        UNIT_FIELD_ATTACK_POWER_MULTIPLIER = ObjectField.OBJECT_END + 124, // size 1, flags MIRROR_SELF, MIRROR_OWNER
        UNIT_FIELD_RANGED_ATTACK_POWER = ObjectField.OBJECT_END + 125, // size 1, flags MIRROR_SELF, MIRROR_OWNER
        UNIT_FIELD_RANGED_ATTACK_POWER_MOD_POS = ObjectField.OBJECT_END + 126, // size 1, flags MIRROR_SELF, MIRROR_OWNER
        UNIT_FIELD_RANGED_ATTACK_POWER_MOD_NEG = ObjectField.OBJECT_END + 127, // size 1, flags MIRROR_SELF, MIRROR_OWNER
        UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER = ObjectField.OBJECT_END + 128, // size 1, flags MIRROR_SELF, MIRROR_OWNER
        UNIT_FIELD_MIN_RANGED_DAMAGE = ObjectField.OBJECT_END + 129, // size 1, flags MIRROR_SELF, MIRROR_OWNER
        UNIT_FIELD_MAX_RANGED_DAMAGE = ObjectField.OBJECT_END + 130, // size 1, flags MIRROR_SELF, MIRROR_OWNER
        UNIT_FIELD_POWER_COST_MODIFIER = ObjectField.OBJECT_END + 131, // size 7, flags MIRROR_SELF, MIRROR_OWNER
        UNIT_FIELD_POWER_COST_MULTIPLIER = ObjectField.OBJECT_END + 138, // size 7, flags MIRROR_SELF, MIRROR_OWNER
        UNIT_FIELD_MAX_HEALTH_MODIFIER = ObjectField.OBJECT_END + 145, // size 1, flags MIRROR_SELF, MIRROR_OWNER
        UNIT_FIELD_HOVER_HEIGHT = ObjectField.OBJECT_END + 146, // size 1, flags MIRROR_ALL
        UNIT_FIELD_MIN_ITEM_LEVEL = ObjectField.OBJECT_END + 147, // size 1, flags MIRROR_ALL
        UNIT_FIELD_MAX_ITEM_LEVEL = ObjectField.OBJECT_END + 148, // size 1, flags MIRROR_ALL
        UNIT_FIELD_WILD_BATTLE_PET_LEVEL = ObjectField.OBJECT_END + 149, // size 1, flags MIRROR_ALL
        UNIT_FIELD_BATTLE_PET_COMPANION_NAME_TIMESTAMP = ObjectField.OBJECT_END + 150, // size 1, flags MIRROR_ALL
        UNIT_FIELD_INTERACT_SPELL_ID = ObjectField.OBJECT_END + 151, // size 1, flags MIRROR_ALL
        UNIT_FIELD_END = ObjectField.OBJECT_END + 152
    };

    public enum UnitDynamicField
    {
        UNIT_DYNAMIC_FIELD_PASSIVE_SPELLS                = ObjectField.OBJECT_END + 0x0,
        UNIT_DYNAMIC_END                                 = ObjectField.OBJECT_END + 0x101
    }

    public enum PlayerField
    {
        PLAYER_FIELD_DUEL_ARBITER = UnitField.UNIT_FIELD_END + 0, // size 2, flags MIRROR_ALL
        PLAYER_FIELD_PLAYER_FLAGS = UnitField.UNIT_FIELD_END + 2, // size 1, flags MIRROR_ALL
        PLAYER_FIELD_GUILD_RANK_ID = UnitField.UNIT_FIELD_END + 3, // size 1, flags MIRROR_ALL
        PLAYER_FIELD_GUILD_DELETE_DATE = UnitField.UNIT_FIELD_END + 4, // size 1, flags MIRROR_ALL
        PLAYER_FIELD_GUILD_LEVEL = UnitField.UNIT_FIELD_END + 5, // size 1, flags MIRROR_ALL
        PLAYER_FIELD_HAIR_COLOR_ID = UnitField.UNIT_FIELD_END + 6, // size 1, flags MIRROR_ALL
        PLAYER_FIELD_REST_STATE = UnitField.UNIT_FIELD_END + 7, // size 1, flags MIRROR_ALL
        PLAYER_FIELD_ARENA_FACTION = UnitField.UNIT_FIELD_END + 8, // size 1, flags MIRROR_ALL
        PLAYER_FIELD_DUEL_TEAM = UnitField.UNIT_FIELD_END + 9, // size 1, flags MIRROR_ALL
        PLAYER_FIELD_GUILD_TIME_STAMP = UnitField.UNIT_FIELD_END + 10, // size 1, flags MIRROR_ALL
        PLAYER_FIELD_QUEST_LOG = UnitField.UNIT_FIELD_END + 11, // size 750, flags MIRROR_PARTY
        PLAYER_FIELD_VISIBLE_ITEMS = UnitField.UNIT_FIELD_END + 761, // size 38, flags MIRROR_ALL
        PLAYER_FIELD_PLAYER_TITLE = UnitField.UNIT_FIELD_END + 799, // size 1, flags MIRROR_ALL
        PLAYER_FIELD_FAKE_INEBRIATION = UnitField.UNIT_FIELD_END + 800, // size 1, flags MIRROR_ALL
        PLAYER_FIELD_VIRTUAL_PLAYER_REALM = UnitField.UNIT_FIELD_END + 801, // size 1, flags MIRROR_ALL
        PLAYER_FIELD_CURRENT_SPEC_ID = UnitField.UNIT_FIELD_END + 802, // size 1, flags MIRROR_ALL
        PLAYER_FIELD_TAXI_MOUNT_ANIM_KIT_ID = UnitField.UNIT_FIELD_END + 803, // size 1, flags MIRROR_ALL
        PLAYER_FIELD_CURRENT_BATTLE_PET_BREED_QUALITY = UnitField.UNIT_FIELD_END + 804, // size 1, flags MIRROR_ALL
        PLAYER_FIELD_INV_SLOTS = UnitField.UNIT_FIELD_END + 805, // size 172, flags MIRROR_SELF
        PLAYER_FIELD_FARSIGHT_OBJECT = UnitField.UNIT_FIELD_END + 977, // size 2, flags MIRROR_SELF
        PLAYER_FIELD_KNOWN_TITLES = UnitField.UNIT_FIELD_END + 979, // size 10, flags MIRROR_SELF
        PLAYER_FIELD_COINAGE = UnitField.UNIT_FIELD_END + 989, // size 2, flags MIRROR_SELF
        PLAYER_FIELD_XP = UnitField.UNIT_FIELD_END + 991, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_NEXT_LEVEL_XP = UnitField.UNIT_FIELD_END + 992, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_SKILL = UnitField.UNIT_FIELD_END + 993, // size 448, flags MIRROR_SELF
        PLAYER_FIELD_CHARACTER_POINTS = UnitField.UNIT_FIELD_END + 1441, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_MAX_TALENT_TIERS = UnitField.UNIT_FIELD_END + 1442, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_TRACK_CREATURE_MASK = UnitField.UNIT_FIELD_END + 1443, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_TRACK_RESOURCE_MASK = UnitField.UNIT_FIELD_END + 1444, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_MAINHAND_EXPERTISE = UnitField.UNIT_FIELD_END + 1445, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_OFFHAND_EXPERTISE = UnitField.UNIT_FIELD_END + 1446, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_RANGED_EXPERTISE = UnitField.UNIT_FIELD_END + 1447, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_COMBAT_RATING_EXPERTISE = UnitField.UNIT_FIELD_END + 1448, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_BLOCK_PERCENTAGE = UnitField.UNIT_FIELD_END + 1449, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_DODGE_PERCENTAGE = UnitField.UNIT_FIELD_END + 1450, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_PARRY_PERCENTAGE = UnitField.UNIT_FIELD_END + 1451, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_CRIT_PERCENTAGE = UnitField.UNIT_FIELD_END + 1452, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_RANGED_CRIT_PERCENTAGE = UnitField.UNIT_FIELD_END + 1453, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_OFFHAND_CRIT_PERCENTAGE = UnitField.UNIT_FIELD_END + 1454, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_SPELL_CRIT_PERCENTAGE = UnitField.UNIT_FIELD_END + 1455, // size 7, flags MIRROR_SELF
        PLAYER_FIELD_SHIELD_BLOCK = UnitField.UNIT_FIELD_END + 1462, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_SHIELD_BLOCK_CRIT_PERCENTAGE = UnitField.UNIT_FIELD_END + 1463, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_MASTERY = UnitField.UNIT_FIELD_END + 1464, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_PVP_POWER_DAMAGE = UnitField.UNIT_FIELD_END + 1465, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_PVP_POWER_HEALING = UnitField.UNIT_FIELD_END + 1466, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_EXPLORED_ZONES = UnitField.UNIT_FIELD_END + 1467, // size 200, flags MIRROR_SELF
        PLAYER_FIELD_REST_STATE_BONUS_POOL = UnitField.UNIT_FIELD_END + 1667, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_MOD_DAMAGE_DONE_POS = UnitField.UNIT_FIELD_END + 1668, // size 7, flags MIRROR_SELF
        PLAYER_FIELD_MOD_DAMAGE_DONE_NEG = UnitField.UNIT_FIELD_END + 1675, // size 7, flags MIRROR_SELF
        PLAYER_FIELD_MOD_DAMAGE_DONE_PERCENT = UnitField.UNIT_FIELD_END + 1682, // size 7, flags MIRROR_SELF
        PLAYER_FIELD_MOD_HEALING_DONE_POS = UnitField.UNIT_FIELD_END + 1689, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_MOD_HEALING_PERCENT = UnitField.UNIT_FIELD_END + 1690, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_MOD_HEALING_DONE_PERCENT = UnitField.UNIT_FIELD_END + 1691, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_MOD_PERIODIC_HEALING_DONE_PERCENT = UnitField.UNIT_FIELD_END + 1692, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_WEAPON_DMG_MULTIPLIERS = UnitField.UNIT_FIELD_END + 1693, // size 3, flags MIRROR_SELF
        PLAYER_FIELD_MOD_SPELL_POWER_PERCENT = UnitField.UNIT_FIELD_END + 1696, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_MOD_RESILIENCE_PERCENT = UnitField.UNIT_FIELD_END + 1697, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_OVERRIDE_SPELL_POWER_BY_AP_PERCENT = UnitField.UNIT_FIELD_END + 1698, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_OVERRIDE_AP_BY_SPELL_POWER_PERCENT = UnitField.UNIT_FIELD_END + 1699, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_MOD_TARGET_RESISTANCE = UnitField.UNIT_FIELD_END + 1700, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_MOD_TARGET_PHYSICAL_RESISTANCE = UnitField.UNIT_FIELD_END + 1701, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_LIFETIME_MAX_RANK = UnitField.UNIT_FIELD_END + 1702, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_SELF_RES_SPELL = UnitField.UNIT_FIELD_END + 1703, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_PVP_MEDALS = UnitField.UNIT_FIELD_END + 1704, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_BUYBACK_PRICE = UnitField.UNIT_FIELD_END + 1705, // size 12, flags MIRROR_SELF
        PLAYER_FIELD_BUYBACK_TIMESTAMP = UnitField.UNIT_FIELD_END + 1717, // size 12, flags MIRROR_SELF
        PLAYER_FIELD_YESTERDAY_HONORABLE_KILLS = UnitField.UNIT_FIELD_END + 1729, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_LIFETIME_HONORABLE_KILLS = UnitField.UNIT_FIELD_END + 1730, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_WATCHED_FACTION_INDEX = UnitField.UNIT_FIELD_END + 1731, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_COMBAT_RATINGS = UnitField.UNIT_FIELD_END + 1732, // size 27, flags MIRROR_SELF
        PLAYER_FIELD_PVP_INFO = UnitField.UNIT_FIELD_END + 1759, // size 24, flags MIRROR_SELF
        PLAYER_FIELD_MAX_LEVEL = UnitField.UNIT_FIELD_END + 1783, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_RUNE_REGEN = UnitField.UNIT_FIELD_END + 1784, // size 4, flags MIRROR_SELF
        PLAYER_FIELD_NO_REAGENT_COST_MASK = UnitField.UNIT_FIELD_END + 1788, // size 4, flags MIRROR_SELF
        PLAYER_FIELD_GLYPH_SLOTS = UnitField.UNIT_FIELD_END + 1792, // size 6, flags MIRROR_SELF
        PLAYER_FIELD_GLYPHS = UnitField.UNIT_FIELD_END + 1798, // size 6, flags MIRROR_SELF
        PLAYER_FIELD_GLYPH_SLOTS_ENABLED = UnitField.UNIT_FIELD_END + 1804, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_PET_SPELL_POWER = UnitField.UNIT_FIELD_END + 1805, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_RESEARCHING = UnitField.UNIT_FIELD_END + 1806, // size 8, flags MIRROR_SELF
        PLAYER_FIELD_PROFESSION_SKILL_LINE = UnitField.UNIT_FIELD_END + 1814, // size 2, flags MIRROR_SELF
        PLAYER_FIELD_UI_HIT_MODIFIER = UnitField.UNIT_FIELD_END + 1816, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_UI_SPELL_HIT_MODIFIER = UnitField.UNIT_FIELD_END + 1817, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_HOME_REALM_TIME_OFFSET = UnitField.UNIT_FIELD_END + 1818, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_MOD_PET_HASTE = UnitField.UNIT_FIELD_END + 1819, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_SUMMONED_BATTLE_PET_GUID = UnitField.UNIT_FIELD_END + 1820, // size 2, flags MIRROR_SELF
        PLAYER_FIELD_OVERRIDE_SPELLS_ID = UnitField.UNIT_FIELD_END + 1822, // size 1, flags MIRROR_SELF, MIRROR_URGENT_SELF_ONLY
        PLAYER_FIELD_LFG_BONUS_FACTION_ID = UnitField.UNIT_FIELD_END + 1823, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_LOOT_SPEC_ID = UnitField.UNIT_FIELD_END + 1824, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_OVERRIDE_ZONE_PVP_TYPE = UnitField.UNIT_FIELD_END + 1825, // size 1, flags MIRROR_SELF, MIRROR_URGENT_SELF_ONLY
        PLAYER_FIELD_ITEM_LEVEL_DELTA = UnitField.UNIT_FIELD_END + 1826, // size 1, flags MIRROR_SELF
        PLAYER_FIELD_END = UnitField.UNIT_FIELD_END + 1827
    };

    public enum PlayerDynamicField
    {
        PLAYER_DYNAMIC_FIELD_RESEARCH_SITES              = PlayerField.PLAYER_FIELD_END + 0x0,
        PLAYER_DYNAMIC_FIELD_DAILY_QUESTS_COMPLETED      = PlayerField.PLAYER_FIELD_END + 0x2,
        PLAYER_DYNAMIC_END                               = PlayerField.PLAYER_FIELD_END + 0x4
    }

    public enum GameObjectField
    {
        GAMEOBJECT_FIELD_CREATED_BY                      = ObjectField.OBJECT_END + 0x0000, // Size =   2, Type: Flags PUBLIC
        GAMEOBJECT_DISPLAYID                             = ObjectField.OBJECT_END + 0x0002, // Size =   1, Type: Flags PUBLIC
        GAMEOBJECT_FLAGS                                 = ObjectField.OBJECT_END + 0x0003, // Size =   1, Type: Flags PUBLIC | DYNAMIC
        GAMEOBJECT_PARENTROTATION                        = ObjectField.OBJECT_END + 0x0004, // Size =   4, Type: Flags PUBLIC
        GAMEOBJECT_FACTION                               = ObjectField.OBJECT_END + 0x0008, // Size =   1, Type: Flags PUBLIC
        GAMEOBJECT_LEVEL                                 = ObjectField.OBJECT_END + 0x0009, // Size =   1, Type: Flags PUBLIC
        GAMEOBJECT_BYTES_1                               = ObjectField.OBJECT_END + 0x000A, // Size =   1, Type: Flags PUBLIC | DYNAMIC
        GAMEOBJECT_FIELD_ANIM_PROGRESS                   = ObjectField.OBJECT_END + 0x000B, // Size =   1, Type: Flags PUBLIC | DYNAMIC
        GAMEOBJECT_FIELD_END                             = ObjectField.OBJECT_END + 0x000C
    };

    public enum DynamicObjectField
    {
        DYNAMICOBJECT_FIELD_CASTER                       = ObjectField.OBJECT_END + 0x0,
        DYNAMICOBJECT_FIELD_TYPE_AND_VISUAL_ID           = ObjectField.OBJECT_END + 0x2,
        DYNAMICOBJECT_FIELD_SPELLID                      = ObjectField.OBJECT_END + 0x3,
        DYNAMICOBJECT_FIELD_RADIUS                       = ObjectField.OBJECT_END + 0x4,
        DYNAMICOBJECT_FIELD_CASTTIME                     = ObjectField.OBJECT_END + 0x5,
        DYNAMICOBJECT_FIELD_END                          = ObjectField.OBJECT_END + 0x6
    };

    public enum CorpseField
    {
        CORPSE_FIELD_OWNER                               = ObjectField.OBJECT_END + 0x0,
        CORPSE_FIELD_PARTY_GUID                          = ObjectField.OBJECT_END + 0x2,
        CORPSE_FIELD_DISPLAYID                           = ObjectField.OBJECT_END + 0x4,
        CORPSE_FIELD_ITEMS                               = ObjectField.OBJECT_END + 0x5,
        CORPSE_FIELD_SKINID                              = ObjectField.OBJECT_END + 0x18,
        CORPSE_FIELD_FACIAL_HAIR_STYLE_ID                = ObjectField.OBJECT_END + 0x19,
        CORPSE_FIELD_FLAGS                               = ObjectField.OBJECT_END + 0x1A,
        CORPSE_FIELD_DYNAMIC_FLAGS                       = ObjectField.OBJECT_END + 0x1B,
        CORPSE_END                                       = ObjectField.OBJECT_END + 0x1C
    };

    public enum AreaTriggerField
    {
        AREATRIGGER_FIELD_CASTER                         = ObjectField.OBJECT_END + 0x0,
        AREATRIGGER_FIELD_DURATION                       = ObjectField.OBJECT_END + 0x2,
        AREATRIGGER_FIELD_SPELLID                        = ObjectField.OBJECT_END + 0x3,
        AREATRIGGER_FIELD_SPELL_VISUAL_ID                = ObjectField.OBJECT_END + 0x4,
        AREATRIGGER_FIELD_EXPLICIT_SCALE                 = ObjectField.OBJECT_END + 0x5,
        AREATRIGGER_END                                  = ObjectField.OBJECT_END + 0x6
    };

    public enum SceneObjectField
    {
        SCENEOBJECT_FIELD_SCRIPT_PACKAGE_ID              = ObjectField.OBJECT_END + 0x0000, // Size = 1, Type: Flags PUBLIC
        SCENEOBJECT_FIELD_RND_SEED_VAL                   = ObjectField.OBJECT_END + 0x0001, // Size = 1, Type: Flags PUBLIC
        SCENEOBJECT_FIELD_CREATEDBY                      = ObjectField.OBJECT_END + 0x0002, // Size = 2, Type: Flags PUBLIC
        SCENEOBJECT_FIELD_SCENE_TYPE                     = ObjectField.OBJECT_END + 0x0004, // Size = 1, Type: Flags PUBLIC
        SCENEOBJECT_FIELD_END                            = ObjectField.OBJECT_END + 0x0005
    };
    // ReSharper restore InconsistentNaming
}
