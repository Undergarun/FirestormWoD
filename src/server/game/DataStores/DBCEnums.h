////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DBCENUMS_H
#define DBCENUMS_H

enum LevelLimit
{
    // Client expected level limitation, like as used in DBC item max levels for "until max player level"
    // use as default max player level, must be fit max level for used client
    // also see MAX_LEVEL and STRONG_MAX_LEVEL define
#define DEFAULT_MAX_LEVEL 100

    // client supported max level for player/pets/etc. Avoid overflow or client stability affected.
    // also see GT_MAX_LEVEL define
#define MAX_LEVEL    100

    // Server side limitation. Base at used code requirements.
    // also see MAX_LEVEL and GT_MAX_LEVEL define
#define STRONG_MAX_LEVEL 255
};

/*
enum BattlegroundBracketId                                  // bracketId for level ranges
{
    BG_BRACKET_ID_FIRST          = 0,
    BG_BRACKET_ID_LAST           = 16
};*/

// must be max value in PvPDificulty slot+1
//#define MAX_BATTLEGROUND_BRACKETS  17

enum CurrencyFlags
{
    CURRENCY_FLAG_TRADEABLE             = 0x01,
    CURRENCY_FLAG_HIGH_PRECISION        = 0x08,
    CURRENCY_FLAG_ARCHAEOLOGY_FRAGMENT  = 0x20,
    CURRENCY_FLAG_HAS_SEASON_COUNT      = 0x80                  // guessed
};

enum AreaTeams
{
    AREATEAM_NONE  = 0,
    AREATEAM_ALLY  = 2,
    AREATEAM_HORDE = 4,
    AREATEAM_ANY   = 6
};

enum AreaFlags
{
    AREA_FLAG_SNOW             = 0x00000001,                // snow (only Dun Morogh, Naxxramas, Razorfen Downs and Winterspring)
    AREA_FLAG_UNK1             = 0x00000002,                // Razorfen Downs, Naxxramas and Acherus: The Ebon Hold (3.3.5a)
    AREA_FLAG_UNK2             = 0x00000004,                // Only used for areas on map 571 (development before)
    AREA_FLAG_SLAVE_CAPITAL    = 0x00000008,                // city and city subsones
    AREA_FLAG_UNK3             = 0x00000010,                // can't find common meaning
    AREA_FLAG_SLAVE_CAPITAL2   = 0x00000020,                // slave capital city flag?
    AREA_FLAG_ALLOW_DUELS      = 0x00000040,                // allow to duel here
    AREA_FLAG_ARENA            = 0x00000080,                // arena, both instanced and world arenas
    AREA_FLAG_CAPITAL          = 0x00000100,                // main capital city flag
    AREA_FLAG_CITY             = 0x00000200,                // only for one zone named "City" (where it located?)
    AREA_FLAG_OUTLAND          = 0x00000400,                // expansion zones? (only Eye of the Storm not have this flag, but have 0x00004000 flag)
    AREA_FLAG_SANCTUARY        = 0x00000800,                // sanctuary area (PvP disabled)
    AREA_FLAG_NEED_FLY         = 0x00001000,                // Respawn alive at the graveyard without corpse
    AREA_FLAG_UNUSED1          = 0x00002000,                // Unused in 3.3.5a
    AREA_FLAG_OUTLAND2         = 0x00004000,                // expansion zones? (only Circle of Blood Arena not have this flag, but have 0x00000400 flag)
    AREA_FLAG_OUTDOOR_PVP      = 0x00008000,                // pvp objective area? (Death's Door also has this flag although it's no pvp object area)
    AREA_FLAG_ARENA_INSTANCE   = 0x00010000,                // used by instanced arenas only
    AREA_FLAG_UNUSED2          = 0x00020000,                // Unused in 3.3.5a
    AREA_FLAG_CONTESTED_AREA   = 0x00040000,                // On PvP servers these areas are considered contested, even though the zone it is contained in is a Horde/Alliance territory.
    AREA_FLAG_UNK6             = 0x00080000,                // Valgarde and Acherus: The Ebon Hold
    AREA_FLAG_LOWLEVEL         = 0x00100000,                // used for some starting areas with area_level <= 15
    AREA_FLAG_TOWN             = 0x00200000,                // small towns with Inn
    AREA_FLAG_REST_ZONE_HORDE  = 0x00400000,                // Warsong Hold, Acherus: The Ebon Hold, New Agamand Inn, Vengeance Landing Inn, Sunreaver Pavilion (Something to do with team?)
    AREA_FLAG_REST_ZONE_ALLIANCE = 0x00800000,              // Valgarde, Acherus: The Ebon Hold, Westguard Inn, Silver Covenant Pavilion (Something to do with team?)
    AREA_FLAG_WINTERGRASP      = 0x01000000,                // Wintergrasp and it's subzones
    AREA_FLAG_INSIDE           = 0x02000000,                // used for determinating spell related inside/outside questions in Map::IsOutdoors
    AREA_FLAG_OUTSIDE          = 0x04000000,                // used for determinating spell related inside/outside questions in Map::IsOutdoors
    AREA_FLAG_WINTERGRASP_2    = 0x08000000,                // Can Hearth And Resurrect From Area
    AREA_FLAG_NO_FLY_ZONE      = 0x20000000,                // Marks zones where you cannot fly
    AREA_FLAG_UNK9             = 0x40000000
};

enum AreaFlags2
{
    AREA_FLAG2_UNK1             = 0x00000001,                //
    AREA_FLAG2_UNK2             = 0x00000002,                // PattymackLand only
    AREA_FLAG2_UNK3             = 0x00000004,                // donjons / raids
    AREA_FLAG2_UNK4             = 0x00000008,                // OrgrimmarRaid and DraenorAuchindoun
    AREA_FLAG2_UNK5             = 0x00000010,                //
    AREA_FLAG2_UNK6             = 0x00000020,                //
    AREA_FLAG2_UNK7             = 0x00000040,                // Garrison
    AREA_FLAG2_UNK8             = 0x00000080,                //
    AREA_FLAG2_UNK9             = 0x00000100,                //
    AREA_FLAG2_UNK10            = 0x00000200,                //
    AREA_FLAG2_UNK11            = 0x00000400,                //
    AREA_FLAG2_UNK12            = 0x00000800,                //
    AREA_FLAG2_UNK13            = 0x00001000,                //
    AREA_FLAG2_UNK14            = 0x00002000,                //
    AREA_FLAG2_UNK15            = 0x00004000,                //
    AREA_FLAG2_UNK16            = 0x00008000,                //
    AREA_FLAG2_UNK17            = 0x00010000,                //
    AREA_FLAG2_UNK18            = 0x00020000,                //
    AREA_FLAG2_UNK19            = 0x00040000,                //
    AREA_FLAG2_UNK20            = 0x00080000,                //
    AREA_FLAG2_UNK21            = 0x00100000,                //
    AREA_FLAG2_UNK22            = 0x00200000,                //
    AREA_FLAG2_UNK23            = 0x00400000,                //
    AREA_FLAG2_UNK24            = 0x00800000,                //
    AREA_FLAG2_UNK25            = 0x01000000,                //
    AREA_FLAG2_UNK26            = 0x02000000,                //
    AREA_FLAG2_UNK27            = 0x04000000,                //
    AREA_FLAG2_UNK28            = 0x08000000,                //
    AREA_FLAG2_UNK29            = 0x20000000,                //
    AREA_FLAG2_UNK30            = 0x40000000
};

enum Difficulty
{
    DifficultyNone          = 0,  ///< difficulty_entry_0
    DifficultyNormal        = 1,  ///< difficulty_entry_1
    DifficultyHeroic        = 2,  ///< difficulty_entry_2
    Difficulty10N           = 3,  ///< difficulty_entry_3
    Difficulty25N           = 4,  ///< difficulty_entry_4
    Difficulty10HC          = 5,  ///< difficulty_entry_5
    Difficulty25HC          = 6,  ///< difficulty_entry_6
    DifficultyRaidTool      = 7,  ///< difficulty_entry_7
    DifficultyChallenge     = 8,  ///< difficulty_entry_8
    Difficulty40            = 9,  ///< difficulty_entry_9

    DifficultyHCScenario    = 11, ///< difficulty_entry_11
    DifficultyNScenario     = 12, ///< difficulty_entry_12

    DifficultyRaidNormal    = 14, ///< difficulty_entry_14
    DifficultyRaidHeroic    = 15, ///< difficulty_entry_15
    DifficultyRaidMythic    = 16, ///< difficulty_entry_16
    DifficultyRaidLFR       = 17, ///< difficulty_entry_17
    DifficultyEventRaid     = 18, ///< difficulty_entry_18
    DifficultyEventDungeon  = 19, ///< difficulty_entry_19
    DifficultyEventScenario = 20, ///< difficulty_entry_20

    DifficultyMythic        = 23,
    DifficultyTimewalker    = 24,
    MaxDifficulties
};

enum DifficultyFlags
{
    DIFFICULTY_FLAG_HEROIC          = 0x01,
    DIFFICULTY_FLAG_DEFAULT         = 0x02,
    DIFFICULTY_FLAG_CAN_SELECT      = 0x04, ///< Player can select this difficulty in dropdown menu
    DIFFICULTY_FLAG_CHALLENGE_MODE  = 0x08,

    DIFFICULTY_FLAG_LEGACY          = 0x20,
    DIFFICULTY_FLAG_DISPLAY_HEROIC  = 0x40, ///< Controls icon displayed on minimap when inside the instance
    DIFFICULTY_FLAG_DISPLAY_MYTHIC  = 0x80  ///< Controls icon displayed on minimap when inside the instance
};

enum SpawnMask
{
    SpawnMaskContinent          = (1 << Difficulty::DifficultyNone),
    SpawnMaskDungeonNormal      = (1 << Difficulty::DifficultyNormal),
    SpawnMaskDungeonHeroic      = (1 << Difficulty::DifficultyHeroic),
    SpawnMaskRaid10Normal       = (1 << Difficulty::Difficulty10N ),
    SpawnMaskRaid25Normal       = (1 << Difficulty::Difficulty25N),
    SpawnMaskRaid10Heroic       = (1 << Difficulty::Difficulty10HC),
    SpawnMaskRaid25Heroic       = (1 << Difficulty::Difficulty25HC),
    SpawnMaskRaidTool           = (1 << Difficulty::DifficultyRaidTool),
    SpawnMaskChallengeMode      = (1 << Difficulty::DifficultyChallenge),
    SpawnMaskRaid40Normal       = (1 << Difficulty::Difficulty40),

    SpawnMaskScenarioHeroic     = (1 << Difficulty::DifficultyHCScenario),
    SpawnMaskScenarioNormal     = (1 << Difficulty::DifficultyNScenario),
    
    SpawnMaskRaidNormal         = (1 << Difficulty::DifficultyRaidNormal),
    SpawnMaskRaidHeroic         = (1 << Difficulty::DifficultyRaidHeroic),
    SpawnMaskRaidMythic         = (1 << Difficulty::DifficultyRaidMythic),
    SpawnMaskRaidLFR            = (1 << Difficulty::DifficultyRaidLFR),
    SpawnMaskEventRaid          = (1 << Difficulty::DifficultyEventRaid),
    SpawnMaskEventDungeon       = (1 << Difficulty::DifficultyEventDungeon),
    SpawnMaskEventScenario      = (1 << Difficulty::DifficultyEventScenario),

    SpawnMaskDungeonAll         = (SpawnMask::SpawnMaskDungeonNormal | SpawnMask::SpawnMaskDungeonHeroic | SpawnMask::SpawnMaskChallengeMode),
    SpawnMaskLegacyNormalAll    = (SpawnMask::SpawnMaskRaid10Normal | SpawnMask::SpawnMaskRaid25Normal | SpawnMask::SpawnMaskRaid40Normal),
    SpawnMaskLegacyHeroicAll    = (SpawnMask::SpawnMaskRaid10Heroic | SpawnMask::SpawnMaskRaid25Heroic),
    SpawnMaskLegacyRaidAll      = (SpawnMask::SpawnMaskLegacyNormalAll | SpawnMask::SpawnMaskLegacyHeroicAll | SpawnMask::SpawnMaskRaidTool),
    SpawnMaskRaidAll            = (SpawnMask::SpawnMaskRaidNormal | SpawnMask::SpawnMaskRaidHeroic | SpawnMask::SpawnMaskRaidMythic | SpawnMask::SpawnMaskRaidLFR)
};

enum MapFlags
{
    MAP_FLAG_UNK_1                  = 0x00000001,
    MAP_FLAG_DEV                    = 0x00000002,   ///< Client will reject loading of this map
    MAP_FLAG_UNK_3                  = 0x00000004,
    MAP_FLAG_UNK_4                  = 0x00000008,
    MAP_FLAG_UNK_5                  = 0x00000010,
    MAP_FLAG_UNK_6                  = 0x00000020,
    MAP_FLAG_UNK_7                  = 0x00000040,
    MAP_FLAG_UNK_8                  = 0x00000080,
    MAP_FLAG_CAN_TOGGLE_DIFFICULTY  = 0x00000100,   ///< Allow players to change difficulty
    MAP_FLAG_UNK_10                 = 0x00000200,
    MAP_FLAG_UNK_11                 = 0x00000400,
    MAP_FLAG_UNK_12                 = 0x00000800,
    MAP_FLAG_UNK_13                 = 0x00001000,
    MAP_FLAG_UNK_14                 = 0x00002000,
    MAP_FLAG_UNK_15                 = 0x00004000,
    MAP_FLAG_FLEX_LOCKING           = 0x00008000,   ///< All difficulties share completed encounters lock, not bound to a single instance id
                                                    ///< heroic difficulty flag overrides it and uses instance id bind
    MAP_FLAG_LIMIT_FAR_CLIP         = 0x00010000,   ///< Limit farclip to 727.0
    MAP_FLAG_UNK_18                 = 0x00020000,
    MAP_FLAG_UNK_19                 = 0x00040000,
    MAP_FLAG_UNK_20                 = 0x00080000,
    MAP_FLAG_UNK_21                 = 0x00100000,
    MAP_FLAG_UNK_22                 = 0x00200000,
    MAP_FLAG_UNK_23                 = 0x00400000,
    MAP_FLAG_UNK_24                 = 0x00800000,
    MAP_FLAG_UNK_25                 = 0x01000000,
    MAP_FLAG_UNK_26                 = 0x02000000,
    MAP_FLAG_GARRISON               = 0x04000000,
    MAP_FLAG_UNK_28                 = 0x08000000,
    MAP_FLAG_UNK_29                 = 0x10000000,
    MAP_FLAG_UNK_30                 = 0x20000000
};

enum FactionTemplateFlags
{
    FACTION_TEMPLATE_FLAG_PVP               = 0x00000800,   // flagged for PvP
    FACTION_TEMPLATE_FLAG_CONTESTED_GUARD   = 0x00001000,   // faction will attack players that were involved in PvP combats
    FACTION_TEMPLATE_FLAG_HOSTILE_BY_DEFAULT= 0x00002000
};

enum FactionMasks
{
    FACTION_MASK_PLAYER   = 1,                              // any player
    FACTION_MASK_ALLIANCE = 2,                              // player or creature from alliance team
    FACTION_MASK_HORDE    = 4,                              // player or creature from horde team
    FACTION_MASK_MONSTER  = 8                               // aggressive creature from monster team
    // if none flags set then non-aggressive creature
};

enum InstanceTypes                                          // m_InstanceTypes (Map.dbc)
{
    MAP_COMMON          = 0,                                // none
    MAP_INSTANCE        = 1,                                // party
    MAP_RAID            = 2,                                // raid
    MAP_BATTLEGROUND    = 3,                                // pvp
    MAP_ARENA           = 4,                                // arena
    MAP_SCENARIO        = 5                                 // Scenario
};

enum AbilytyLearnType
{
    ABILITY_LEARNED_ON_GET_PROFESSION_SKILL     = 1,
    ABILITY_LEARNED_ON_GET_RACE_OR_CLASS_SKILL  = 2
};

enum ItemEnchantmentType
{
    ITEM_ENCHANTMENT_TYPE_NONE             = 0,
    ITEM_ENCHANTMENT_TYPE_COMBAT_SPELL     = 1,
    ITEM_ENCHANTMENT_TYPE_DAMAGE           = 2,
    ITEM_ENCHANTMENT_TYPE_EQUIP_SPELL      = 3,
    ITEM_ENCHANTMENT_TYPE_RESISTANCE       = 4,
    ITEM_ENCHANTMENT_TYPE_STAT             = 5,
    ITEM_ENCHANTMENT_TYPE_TOTEM            = 6,
    ITEM_ENCHANTMENT_TYPE_USE_SPELL        = 7,
    ITEM_ENCHANTMENT_TYPE_PRISMATIC_SOCKET = 8
};


//TC 4.3.4 https://github.com/TrinityCore/TrinityCore/commit/21c42bf5a7e916bbb705b3195acaa890dc0126f8 implement it !
//enum ItemExtendedCostFlags
//{
//    ITEM_EXT_COST_FLAG_REQUIRE_GUILD = 0x01,
//    ITEM_EXT_COST_CURRENCY_REQ_IS_SEASON_EARNED_1 = 0x02,
//    ITEM_EXT_COST_CURRENCY_REQ_IS_SEASON_EARNED_2 = 0x04,
//    ITEM_EXT_COST_CURRENCY_REQ_IS_SEASON_EARNED_3 = 0x08,
//    ITEM_EXT_COST_CURRENCY_REQ_IS_SEASON_EARNED_4 = 0x10,
//    ITEM_EXT_COST_CURRENCY_REQ_IS_SEASON_EARNED_5 = 0x20,
//};

enum ItemLimitCategoryMode
{
    ITEM_LIMIT_CATEGORY_MODE_HAVE       = 0,                      // limit applied to amount items in inventory/bank
    ITEM_LIMIT_CATEGORY_MODE_EQUIP      = 1                       // limit applied to amount equipped items (including used gems)
};

enum class ItemContext : uint8
{
    None            = 0,
    DungeonNormal   = 1,
    DungeonHeroic   = 2,
    DungeonMythic   = 3,    ///< Guessed
    RaidHeroic      = 5,
    RaidMythic      = 6,
    RaidNormal      = 8,    ///< Guessed
    RaidLfr         = 9,    ///< Guessed
    TradeSkill      = 13,
    DungeonLevelUp1 = 17,
    DungeonLevelUp2 = 18,
    DungeonLevelUp3 = 19,
    DungeonLevelUp4 = 20
};

enum MountFlags
{
    MOUNT_FLAG_CAN_PITCH                = 0x4,                    // client checks MOVEMENTFLAG2_FULL_SPEED_PITCHING
    MOUNT_FLAG_CAN_SWIM                 = 0x8                     // client checks MOVEMENTFLAG_SWIMMING
};

enum TotemCategoryType
{
    TOTEM_CATEGORY_TYPE_KNIFE           = 1,
    TOTEM_CATEGORY_TYPE_TOTEM           = 2,
    TOTEM_CATEGORY_TYPE_ROD             = 3,
    TOTEM_CATEGORY_TYPE_PICK            = 21,
    TOTEM_CATEGORY_TYPE_STONE           = 22,
    TOTEM_CATEGORY_TYPE_HAMMER          = 23,
    TOTEM_CATEGORY_TYPE_SPANNER         = 24
};

// SummonProperties.dbc, col 1
enum SummonPropGroup
{
    SUMMON_PROP_GROUP_UNKNOWN1       = 0,                   // 1160 spells in 3.0.3
    SUMMON_PROP_GROUP_UNKNOWN2       = 1,                   // 861 spells in 3.0.3
    SUMMON_PROP_GROUP_PETS           = 2,                   // 52 spells in 3.0.3, pets mostly
    SUMMON_PROP_GROUP_CONTROLLABLE   = 3,                   // 13 spells in 3.0.3, mostly controllable
    SUMMON_PROP_GROUP_UNKNOWN3       = 4,                   // 86 spells in 3.0.3, taxi/mounts
    SUMMON_PROP_GROUP_UNKNOWN4       = 5                    // 86 spells in 3.0.3, taxi/mounts
};

// SummonProperties.dbc, col 3
enum SummonPropType
{
    SUMMON_PROP_TYPE_UNKNOWN         = 0,                   // different summons, 1330 spells in 3.0.3
    SUMMON_PROP_TYPE_SUMMON          = 1,                   // generic summons, 49 spells in 3.0.3
    SUMMON_PROP_TYPE_GUARDIAN        = 2,                   // summon guardian, 393 spells in 3.0.3
    SUMMON_PROP_TYPE_ARMY            = 3,                   // summon army, 5 spells in 3.0.3
    SUMMON_PROP_TYPE_TOTEM           = 4,                   // summon totem, 169 spells in 3.0.3
    SUMMON_PROP_TYPE_CRITTER         = 5,                   // critter/minipet, 195 spells in 3.0.3
    SUMMON_PROP_TYPE_DK              = 6,                   // summon DRW/Ghoul, 2 spells in 3.0.3
    SUMMON_PROP_TYPE_BOMB            = 7,                   // summon bot/bomb, 4 spells in 3.0.3
    SUMMON_PROP_TYPE_PHASING         = 8,                   // something todo with DK prequest line, 2 spells in 3.0.3
    SUMMON_PROP_TYPE_SIEGE_VEH       = 9,                   // summon different vehicles, 14 spells in 3.0.3
    SUMMON_PROP_TYPE_DRAKE_VEH       = 10,                  // summon drake (vehicle), 3 spells
    SUMMON_PROP_TYPE_LIGHTWELL       = 11,                  // summon lightwell, 6 spells in 3.0.3
    SUMMON_PROP_TYPE_JEEVES          = 12,                  // summon Jeeves, 1 spell in 3.3.5a
    SUMMON_PROP_TYPE_LASHTAIL        = 13,                  // Lashtail Hatchling, 1 spell in 4.2.2
    SUMMON_PROP_TYPE_UNKNOWN_1       = 14,                  // unk
    SUMMON_PROP_TYPE_UNKNOWN_2       = 15,                  // unk
    SUMMON_PROP_TYPE_UNKNOWN_3       = 16,                  // unk
    SUMMON_PROP_TYPE_UNKNOWN_4       = 17,                  // unk
    SUMMON_PROP_TYPE_UNKNOWN_5       = 18,                  // unk
    SUMMON_PROP_TYPE_UNKNOWN_6       = 19,                  // unk
    SUMMON_PROP_TYPE_UNKNOWN_7       = 20,                  // unk
    SUMMON_PROP_TYPE_UNKNOWN_8       = 21,                  // unk
    SUMMON_PROP_TYPE_UNKNOWN_9       = 24,                  // unk
    SUMMON_PROP_TYPE_UNKNOWN_10      = 25,                  // unk
    SUMMON_PROP_TYPE_UNKNOWN_11      = 26,                  // unk
};

// SummonProperties.dbc, col 5
enum SummonPropFlags
{
    SUMMON_PROP_FLAG_NONE            = 0x00000000,          // 1342 spells in 3.0.3
    SUMMON_PROP_FLAG_UNK1            = 0x00000001,          // 75 spells in 3.0.3, something unfriendly
    SUMMON_PROP_FLAG_UNK2            = 0x00000002,          // 616 spells in 3.0.3, something friendly
    SUMMON_PROP_FLAG_UNK3            = 0x00000004,          // 22 spells in 3.0.3, no idea...
    SUMMON_PROP_FLAG_UNK4            = 0x00000008,          // 49 spells in 3.0.3, some mounts
    SUMMON_PROP_FLAG_UNK5            = 0x00000010,          // 25 spells in 3.0.3, quest related?
    SUMMON_PROP_FLAG_UNK6            = 0x00000020,          // 0 spells in 3.3.5, unused
    SUMMON_PROP_FLAG_UNK7            = 0x00000040,          // 12 spells in 3.0.3, no idea
    SUMMON_PROP_FLAG_UNK8            = 0x00000080,          // 4 spells in 3.0.3, no idea
    SUMMON_PROP_FLAG_UNK9            = 0x00000100,          // 51 spells in 3.0.3, no idea, many quest related
    SUMMON_PROP_FLAG_UNK10           = 0x00000200,          // 51 spells in 3.0.3, something defensive
    SUMMON_PROP_FLAG_UNK11           = 0x00000400,          // 3 spells, requires something near?
    SUMMON_PROP_FLAG_UNK12           = 0x00000800,          // 30 spells in 3.0.3, no idea
    SUMMON_PROP_FLAG_UNK13           = 0x00001000,          // Lightwell, Jeeves, Gnomish Alarm-o-bot, Build vehicles(wintergrasp)
    SUMMON_PROP_FLAG_UNK14           = 0x00002000,          // Guides, player follows
    SUMMON_PROP_FLAG_UNK15           = 0x00004000,          // Force of Nature, Shadowfiend, Feral Spirit, Summon Water Elemental
    SUMMON_PROP_FLAG_UNK16           = 0x00008000,          // Light/Dark Bullet, Soul/Fiery Consumption, Twisted Visage, Twilight Whelp. Phase related?
    SUMMON_PROP_FLAG_UNK17           = 0x00010000,
    SUMMON_PROP_FLAG_UNK18           = 0x00020000,
    SUMMON_PROP_FLAG_UNK19           = 0x00040000,
    SUMMON_PROP_FLAG_UNK20           = 0x00080000,
    SUMMON_PROP_FLAG_UNK21           = 0x00100000           // Totems
};

enum VehicleSeatFlags
{
    VEHICLE_SEAT_FLAG_HAS_LOWER_ANIM_FOR_ENTER                         = 0x00000001,
    VEHICLE_SEAT_FLAG_HAS_LOWER_ANIM_FOR_RIDE                          = 0x00000002,
    VEHICLE_SEAT_FLAG_UNK3                                             = 0x00000004,
    VEHICLE_SEAT_FLAG_SHOULD_USE_VEH_SEAT_EXIT_ANIM_ON_VOLUNTARY_EXIT  = 0x00000008,
    VEHICLE_SEAT_FLAG_UNK5                                             = 0x00000010,
    VEHICLE_SEAT_FLAG_UNK6                                             = 0x00000020,
    VEHICLE_SEAT_FLAG_UNK7                                             = 0x00000040,
    VEHICLE_SEAT_FLAG_UNK8                                             = 0x00000080,
    VEHICLE_SEAT_FLAG_UNK9                                             = 0x00000100,
    VEHICLE_SEAT_FLAG_HIDE_PASSENGER                                   = 0x00000200, // Passenger is hidden
    VEHICLE_SEAT_FLAG_ALLOW_TURNING                                    = 0x00000400, // needed for CGCamera__SyncFreeLookFacing
    VEHICLE_SEAT_FLAG_CAN_CONTROL                                      = 0x00000800, // Lua_UnitInVehicleControlSeat
    VEHICLE_SEAT_FLAG_CAN_CAST_MOUNT_SPELL                             = 0x00001000, // Can cast spells with SPELL_AURA_MOUNTED from seat (possibly 4.x only, 0 seats on 3.3.5a)
    VEHICLE_SEAT_FLAG_UNCONTROLLED                                     = 0x00002000, // can override !& VEHICLE_SEAT_FLAG_CAN_ENTER_OR_EXIT
    VEHICLE_SEAT_FLAG_CAN_ATTACK                                       = 0x00004000, // Can attack, cast spells and use items from vehicle
    VEHICLE_SEAT_FLAG_SHOULD_USE_VEH_SEAT_EXIT_ANIM_ON_FORCED_EXIT     = 0x00008000,
    VEHICLE_SEAT_FLAG_UNK17                                            = 0x00010000,
    VEHICLE_SEAT_FLAG_UNK18                                            = 0x00020000, // Needs research and support (28 vehicles): Allow entering vehicles while keeping specific permanent(?) auras that impose visuals (states like beeing under freeze/stun mechanic, emote state animations).
    VEHICLE_SEAT_FLAG_HAS_VEH_EXIT_ANIM_VOLUNTARY_EXIT                 = 0x00040000,
    VEHICLE_SEAT_FLAG_HAS_VEH_EXIT_ANIM_FORCED_EXIT                    = 0x00080000,
    VEHICLE_SEAT_FLAG_PASSENGER_NOT_SELECTABLE                         = 0x00100000,
    VEHICLE_SEAT_FLAG_UNK22                                            = 0x00200000,
    VEHICLE_SEAT_FLAG_REC_HAS_VEHICLE_ENTER_ANIM                       = 0x00400000,
    VEHICLE_SEAT_FLAG_IS_USING_VEHICLE_CONTROLS                        = 0x00800000, // Lua_IsUsingVehicleControls
    VEHICLE_SEAT_FLAG_ENABLE_VEHICLE_ZOOM                              = 0x01000000,
    VEHICLE_SEAT_FLAG_CAN_ENTER_OR_EXIT                                = 0x02000000, // Lua_CanExitVehicle - can enter and exit at free will
    VEHICLE_SEAT_FLAG_CAN_SWITCH                                       = 0x04000000, // Lua_CanSwitchVehicleSeats
    VEHICLE_SEAT_FLAG_HAS_START_WARITING_FOR_VEH_TRANSITION_ANIM_ENTER = 0x08000000,
    VEHICLE_SEAT_FLAG_HAS_START_WARITING_FOR_VEH_TRANSITION_ANIM_EXIT  = 0x10000000,
    VEHICLE_SEAT_FLAG_CAN_CAST                                         = 0x20000000, // Lua_UnitHasVehicleUI
    VEHICLE_SEAT_FLAG_UNK2                                             = 0x40000000, // checked in conjunction with 0x800 in CastSpell2
    VEHICLE_SEAT_FLAG_ALLOWS_INTERACTION                               = 0x80000000
};

enum VehicleSeatFlagsB
{
    VEHICLE_SEAT_FLAG_B_NONE                     = 0x00000000,
    VEHICLE_SEAT_FLAG_B_USABLE_FORCED            = 0x00000002,
    VEHICLE_SEAT_FLAG_B_TARGETS_IN_RAIDUI        = 0x00000008,           // Lua_UnitTargetsVehicleInRaidUI
    VEHICLE_SEAT_FLAG_B_EJECTABLE                = 0x00000020,           // ejectable
    VEHICLE_SEAT_FLAG_B_USABLE_FORCED_2          = 0x00000040,
    VEHICLE_SEAT_FLAG_B_USABLE_FORCED_3          = 0x00000100,
    VEHICLE_SEAT_FLAG_B_USABLE_FORCED_4          = 0x02000000,
    VEHICLE_SEAT_FLAG_B_CAN_SWITCH               = 0x04000000,
    VEHICLE_SEAT_FLAG_B_VEHICLE_PLAYERFRAME_UI   = 0x80000000            // Lua_UnitHasVehiclePlayerFrameUI - actually checked for flagsb &~ 0x80000000
};

enum TaxiNodeFlags
{
    TAXI_NODE_FLAG_ALLIANCE = 0x1,
    TAXI_NODE_FLAG_HORDE    = 0x2
};

enum TaxiPathNodeFlags
{
    TAXI_PATH_NODE_FLAG_TELEPORT    = 0x1,
    TAXI_PATH_NODE_FLAG_STOP        = 0x2
};

// CurrencyTypes.dbc
enum CurrencyTypes
{
    CURRENCY_TYPE_DALARAN_JEWEL                    = 61,  // Jewelcrafting token WoTLK
    CURRENCY_TYPE_EPICUREAN                        = 81,  // Cook token WoTLK
    CURRENCY_TYPE_CHAMPION_SEAL                    = 241, // Argent tournament token
    ///< WotLK Currency
    CURRENCY_TYPE_ILLUSTROUS_JEWEL                 = 361, // Jewelcrafting token Cataclysm
    CURRENCY_TYPE_ARCHAEOLOGY_DWARF                = 384, // ARCHAEOLOGY Cataclysm
    CURRENCY_TYPE_ARCHAEOLOGY_TROLL                = 385, // ARCHAEOLOGY Cataclysm
    CURRENCY_TYPE_CONQUEST_POINTS                  = 390, // PvP
    CURRENCY_TYPE_TOL_BARAD                        = 391, // Battleground Cataclysm
    CURRENCY_TYPE_HONOR_POINTS                     = 392, // PvP
    CURRENCY_TYPE_ARCHAEOLOGY_FOSSIL               = 393, // ARCHAEOLOGY Cataclysm
    CURRENCY_TYPE_ARCHAEOLOGY_NIGHT_ELF            = 394, // ARCHAEOLOGY Cataclysm
    CURRENCY_TYPE_JUSTICE_POINTS                   = 395, // PvE
    CURRENCY_TYPE_VALOR_POINTS                     = 396, // PvE
    CURRENCY_TYPE_ARCHAEOLOGY_ORC                  = 397, // ARCHAEOLOGY Cataclysm
    CURRENCY_TYPE_ARCHAEOLOGY_DRAENEI              = 398, // ARCHAEOLOGY Cataclysm
    CURRENCY_TYPE_ARCHAEOLOGY_VRYKUL               = 399, // ARCHAEOLOGY Cataclysm
    CURRENCY_TYPE_ARCHAEOLOGY_NERUBIAN             = 400, // ARCHAEOLOGY Cataclysm
    CURRENCY_TYPE_ARCHAEOLOGY_TOLVIR               = 401, // ARCHAEOLOGY Cataclysm
    CURRENCY_TYPE_IRONPAW                          = 402, // Cook token MoP
    CURRENCY_TYPE_WORLD_TREE                       = 416, // 4.2 token Molten front
    CURRENCY_TYPE_CONQUEST_META_ARENA_BG           = 483, // PvP
    CURRENCY_TYPE_CONQUEST_META_RBG                = 484, // Deprecated since WoD (merge of arena/rbg weekcap)
    CURRENCY_TYPE_DARKMOON_TICKET                  = 515, // Darkmoon fair
    CURRENCY_TYPE_MOTE_OF_DARKNESS                 = 614, // 4.3.4 token Dragon soul
    CURRENCY_TYPE_CORRUPTED_ESSENCE                = 615, // 4.3.4 Deathwing token
    ///< MoP Currency
    CURRENCY_TYPE_ARCHAEOLOGY_PANDAREN             = 676, // ARCHAEOLOGY MoP
    CURRENCY_TYPE_ARCHAEOLOGY_MOGU                 = 677, // ARCHAEOLOGY MoP
    CURRENCY_TYPE_ELDER_CHARM_GOOD_FORTUNE         = 697, // LFR roll chance MoP
    CURRENCY_TYPE_ZEN_JEWEL                        = 698, // Jewelcrafting token MoP NYI
    CURRENCY_TYPE_LESSER_CHARM_GOOD_FORTUNE        = 738, // LFR roll chance MoP
    CURRENCY_TYPE_MOGU_RUNE_FATE                   = 752, // roll chance token for T15 boss
    CURRENCY_TYPE_ARCHAEOLOGY_MANTID               = 754, // ARCHAEOLOGY MoP
    CURRENCY_TYPE_WARFORGED_SEAL                   = 776, // roll chance token for T16 boss
    CURRENCY_TYPE_TIMELESS_COIN                    = 777, // timeless isle token
    CURRENCY_TYPE_BLOODY_COIN                      = 789, // timeless isle token
    ///< WoD Currency
    CURRENCY_TYPE_CONQUEST_META_ASHRAN             = 692, ///< Deprecated CURRENCY_TYPE_CONQUEST_META_RANDOM_BG, we use it for Ashran
    CURRENCY_TYPE_BLACK_IRON_FRAGEMENT             = 810, //
    CURRENCY_TYPE_DRAENOR_CLANS_ARCHAEOLOGY        = 821, // ARCHAEOLOGY WoD
    CURRENCY_TYPE_APEXIS_CRYSTAL                   = 823, // Set currency
    CURRENCY_TYPE_GARRISON_RESSOURCES              = 824, // Garrison currency
    CURRENCY_TYPE_OGRE_ARCHAEOLOGY_FRAGEMENT       = 828, // ARCHAEOLOGY WoD
    CURRENCY_TYPE_ARAKKOA_ARCHAEOLOGY              = 829, // ARCHAEOLOGY WoD
    CURRENCY_TYPE_UNUSED                           = 830, //
    CURRENCY_TYPE_UNUSED_2                         = 897, //
    CURRENCY_TYPE_SECRET_OF_DRAENOR_ALCHEMY        = 910, // Professions
    CURRENCY_TYPE_ARTIFACT_FRAGEMENT               = 944, //
    CURRENCY_TYPE_DINGY_IRON_COINS                 = 980, //
    CURRENCY_TYPE_SEAL_OF_TEMPERED_FATE            = 994, //
    CURRENCY_TYPE_SECRET_OF_DRAENOR_TAILORING      = 999, // Professions
    CURRENCY_TYPE_SECRET_OF_DRAENOR_JEWELCRAFTING  = 1008,// Professions
    CURRENCY_TYPE_SECRET_OF_DRAENOR_LEATHERWORKING = 1017,// Professions
    CURRENCY_TYPE_SECRET_OF_DRAENOR_BLACKSMITHING  = 1020,// Professions
    CURRENCY_TYPE_OIL                              = 1101 ///< Oil 6.1.X
};

/// Used for CRITERIA_CONDITION_LEGACY_RAID_TYPE
enum class CriteriaLegacyRaidType : uint8
{
    Normal10    = 0,
    Normal25    = 1,
    Heroic10    = 2,
    Heroic25    = 3,
    None        = 255     ///< Custom ID, internal to server
};

#endif
