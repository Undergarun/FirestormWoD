////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/// \addtogroup world The World
/// @{
/// \file

#ifndef __WORLD_H
#define __WORLD_H

#include "Common.h"
#include "Timer.h"
#include <ace/Atomic_Op.h>
#include "SharedDefines.h"
#include "QueryResult.h"
#include "Callback.h"
#include "TimeDiffMgr.h"
#include "DatabaseWorkerPool.h"
#include "InterRealmSession.h"

#include <atomic>

class Object;
class WorldPacket;
class WorldSession;
class Player;
class WorldSocket;
class SystemMgr;
class LexicsCutter;

// ServerMessages.dbc
enum ServerMessageType
{
    SERVER_MSG_SHUTDOWN_TIME          = 1,
    SERVER_MSG_RESTART_TIME           = 2,
    SERVER_MSG_STRING                 = 3,
    SERVER_MSG_SHUTDOWN_CANCELLED     = 4,
    SERVER_MSG_RESTART_CANCELLED      = 5,
    SERVER_MSG_BG_SHUTDOWN_TIME       = 6,
    SERVER_MSG_BG_RESTART_TIME        = 7,
    SERVER_MSG_INSTANCE_SHUTDOWN_TIME = 8,
    SERVER_MSG_INSTANCE_RESTART_TIME  = 9,
    SERVER_MSG_CONTENT_READY          = 10,
    SERVER_MSG_TICKET_SERVICED_SOON   = 11,
    SERVER_MSG_WAIT_TIME_UNAVAILABLE  = 12,
    SERVER_MSG_TICKET_WAIT_TIME       = 13,
};

enum ShutdownMask
{
    SHUTDOWN_MASK_RESTART = 1,
    SHUTDOWN_MASK_IDLE    = 2,
};

enum ShutdownExitCode
{
    SHUTDOWN_EXIT_CODE = 0,
    ERROR_EXIT_CODE    = 1,
    RESTART_EXIT_CODE  = 2,
};

/// Timers for different object refresh rates
enum WorldTimers
{
    WUPDATE_AUCTIONS,
    WUPDATE_BLACKMARKET,
    WUPDATE_WEATHERS,
    WUPDATE_UPTIME,
    WUPDATE_CORPSES,
    WUPDATE_EVENTS,
    WUPDATE_CLEANDB,
    WUPDATE_AUTOBROADCAST,
    WUPDATE_MAILBOXQUEUE,
    WUPDATE_DELETECHARS,
    WUPDATE_PINGDB,
    WUPDATE_GUILDSAVE,
    WUPDATE_REALM_STATS,
    WUPDATE_TRANSFER,
    WUPDATE_TRANSFER_EXP,
    WUPDATE_COUNT
};

/// Configuration elements
enum WorldBoolConfigs
{
    CONFIG_DURABILITY_LOSS_IN_PVP = 0,
    CONFIG_ADDON_CHANNEL,
    CONFIG_ALLOW_PLAYER_COMMANDS,
    CONFIG_CLEAN_CHARACTER_DB,
    CONFIG_GRID_UNLOAD,
    CONFIG_STATS_SAVE_ONLY_ON_LOGOUT,
    CONFIG_ALLOW_TWO_SIDE_ACCOUNTS,
    CONFIG_ALLOW_TWO_SIDE_INTERACTION_CALENDAR,
    CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHAT,
    CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHANNEL,
    CONFIG_ALLOW_TWO_SIDE_INTERACTION_GROUP,
    CONFIG_ALLOW_TWO_SIDE_INTERACTION_GUILD,
    CONFIG_ALLOW_TWO_SIDE_INTERACTION_AUCTION,
    CONFIG_ALLOW_TWO_SIDE_INTERACTION_MAIL,
    CONFIG_ALLOW_TWO_SIDE_INTERACTION_MOUNT,
    CONFIG_ALLOW_TWO_SIDE_INTERACTION_MOUNT_CAPITALS,
    CONFIG_ALLOW_TWO_SIDE_WHO_LIST,
    CONFIG_ALLOW_TWO_SIDE_ADD_FRIEND,
    CONFIG_ALLOW_TWO_SIDE_TRADE,
    CONFIG_ALL_TAXI_PATHS,
    CONFIG_INSTANT_TAXI,
    CONFIG_INSTANCE_IGNORE_LEVEL,
    CONFIG_INSTANCE_IGNORE_RAID,
    CONFIG_CAST_UNSTUCK,
    CONFIG_GM_LOG_TRADE,
    CONFIG_ALLOW_GM_GROUP,
    CONFIG_ALLOW_GM_FRIEND,
    CONFIG_GM_LOWER_SECURITY,
    CONFIG_SKILL_PROSPECTING,
    CONFIG_SKILL_MILLING,
    CONFIG_SAVE_RESPAWN_TIME_IMMEDIATELY,
    CONFIG_WEATHER,
    CONFIG_QUEST_IGNORE_RAID,
    CONFIG_DETECT_POS_COLLISION,
    CONFIG_RESTRICTED_LFG_CHANNEL,
    CONFIG_SILENTLY_GM_JOIN_TO_CHANNEL,
    CONFIG_TALENTS_INSPECTING,
    CONFIG_CHAT_FAKE_MESSAGE_PREVENTING,
    CONFIG_DEATH_CORPSE_RECLAIM_DELAY_PVP,
    CONFIG_DEATH_CORPSE_RECLAIM_DELAY_PVE,
    CONFIG_DEATH_BONES_WORLD,
    CONFIG_DEATH_BONES_BG_OR_ARENA,
    CONFIG_DIE_COMMAND_MODE,
    CONFIG_DECLINED_NAMES_USED,
    CONFIG_BATTLEGROUND_CAST_DESERTER,
    CONFIG_BATTLEGROUND_QUEUE_ANNOUNCER_ENABLE,
    CONFIG_BATTLEGROUND_QUEUE_ANNOUNCER_PLAYERONLY,
    CONFIG_BG_XP_FOR_KILL,
    CONFIG_ARENA_AUTO_DISTRIBUTE_POINTS,
    CONFIG_ARENA_QUEUE_ANNOUNCER_ENABLE,
    CONFIG_ARENA_QUEUE_ANNOUNCER_PLAYERONLY,
    CONFIG_ARENA_SEASON_IN_PROGRESS,
    CONFIG_ARENA_LOG_EXTENDED_INFO,
    CONFIG_OFFHAND_CHECK_AT_SPELL_UNLEARN,
    CONFIG_VMAP_INDOOR_CHECK,
    CONFIG_START_ALL_SPELLS,
    CONFIG_START_ALL_EXPLORED,
    CONFIG_START_ALL_REP,
    CONFIG_ALWAYS_MAXSKILL,
    CONFIG_PVP_TOKEN_ENABLE,
    CONFIG_NO_RESET_TALENT_COST,
    CONFIG_SHOW_KICK_IN_WORLD,
    CONFIG_CHATLOG_CHANNEL,
    CONFIG_CHATLOG_WHISPER,
    CONFIG_CHATLOG_SYSCHAN,
    CONFIG_CHATLOG_PARTY,
    CONFIG_CHATLOG_RAID,
    CONFIG_CHATLOG_GUILD,
    CONFIG_CHATLOG_PUBLIC,
    CONFIG_CHATLOG_ADDON,
    CONFIG_CHATLOG_BGROUND,
    CONFIG_DUNGEON_FINDER_ENABLE,
    CONFIG_AUTOBROADCAST,
    CONFIG_ALLOW_TICKETS,
    CONFIG_DBC_ENFORCE_ITEM_ATTRIBUTES,
    CONFIG_PRESERVE_CUSTOM_CHANNELS,
    CONFIG_ANTICHEAT_ENABLE,
    CONFIG_PDUMP_NO_PATHS,
    CONFIG_PDUMP_NO_OVERWRITE,
    CONFIG_QUEST_IGNORE_AUTO_ACCEPT,
    CONFIG_QUEST_IGNORE_AUTO_COMPLETE,
    CONFIG_WARDEN_ENABLED,
    CONFIG_WINTERGRASP_ENABLE,
    CONFIG_GUILD_LEVELING_ENABLED,
    CONFIG_LIMIT_WHO_ONLINE,
    CONFIG_ANNOUNCE_BAN,
    CONFIG_ANNOUNCE_MUTE,
    CONFIG_SPELL_FORBIDDEN,
    CONFIG_STATS_LIMITS_ENABLE,
    CONFIG_DUEL_RESET_COOLDOWN_ON_START,
    CONFIG_DUEL_RESET_COOLDOWN_ON_FINISH,
    CONFIG_DUEL_RESET_COOLDOWN_ONLY_IN_ELWYNN_AND_DUROTAR,
    CONFIG_DUEL_RESET_COOLDOWN_RESET_ENERGY_ON_START,
    CONFIG_DUEL_RESET_COOLDOWN_MAX_ENERGY_ON_START,
    CONFIG_VIP_DEBUFF_COMMAND,
    CONFIG_VIP_BANK_COMMAND,
    CONFIG_VIP_REPAIR_COMMAND,
    CONFIG_VIP_RESET_TALENTS_COMMAND,
    CONFIG_VIP_TAXI_COMMAND,
    CONFIG_VIP_HOME_COMMAND,
    CONFIG_VIP_CAPITAL_COMMAND,
    CONFIG_VIP_EXCHANGE_ARENA_COMMAND,
    CONFIG_VIP_EXCHANGE_FROST_COMMAND,
    CONFIG_ANTISPAM_ENABLED,
    CONFIG_DISABLE_RESTART,
    CONFIG_TEMPLATES_ENABLED,
    CONFIG_AOE_LOOT_ENABLED,
    CONFIG_LEXICS_CUTTER_ENABLE,
    CONFIG_ACHIEVEMENT_DISABLE,
    CONFIG_MOP_TRANSFER_ENABLE,
    CONFIG_FUN_ENABLE,
    CONFIG_WEB_DATABASE_ENABLE,
    CONFIG_LOG_PACKETS,
    CONFIG_BATTLEPAY_ENABLE,
    CONFIG_DISABLE_SPELL_SPECIALIZATION_CHECK,
    CONFIG_INTERREALM_ENABLE,
    CONFIG_IGNORE_RESEARCH_SITE,
    CONFIG_ENABLE_MMAPS,
    CONFIG_ENABLE_QUEST,
    CONFIG_ENABLE_LOOTS,
    CONFIG_ENABLE_LOCALES,
    CONFIG_ENABLE_GAMEOBJECTS,
    CONFIG_ENABLE_ONLY_SPECIFIC_MAP,
    CONFIG_ENABLE_RESEARCH_SITE_LOAD,
    CONFIG_ENABLE_ITEM_SPEC_LOAD,
    BOOL_CONFIG_VALUE_COUNT
};

enum WorldFloatConfigs
{
    CONFIG_GROUP_XP_DISTANCE = 0,
    CONFIG_INSTANCE_GROUP_XP_DISTANCE,
    CONFIG_MAX_RECRUIT_A_FRIEND_DISTANCE,
    CONFIG_SIGHT_MONSTER,
    CONFIG_SIGHT_GUARDER,
    CONFIG_LISTEN_RANGE_SAY,
    CONFIG_LISTEN_RANGE_TEXTEMOTE,
    CONFIG_LISTEN_RANGE_YELL,
    CONFIG_CREATURE_FAMILY_FLEE_ASSISTANCE_RADIUS,
    CONFIG_CREATURE_FAMILY_ASSISTANCE_RADIUS,
    CONFIG_THREAT_RADIUS,
    CONFIG_CHANCE_OF_GM_SURVEY,
    CONFIG_STATS_LIMITS_DODGE,
    CONFIG_STATS_LIMITS_PARRY,
    CONFIG_STATS_LIMITS_BLOCK,
    CONFIG_STATS_LIMITS_CRIT,
    CONFIG_LFR_DROP_CHANCE,
    FLOAT_CONFIG_VALUE_COUNT
};

enum WorldIntConfigs
{
    CONFIG_COMPRESSION = 0,
    CONFIG_INTERVAL_SAVE,
    CONFIG_INTERVAL_GRIDCLEAN,
    CONFIG_INTERVAL_MAPUPDATE,
    CONFIG_INTERVAL_CHANGEWEATHER,
    CONFIG_INTERVAL_DISCONNECT_TOLERANCE,
    CONFIG_PORT_WORLD,
    CONFIG_SOCKET_TIMEOUTTIME,
    CONFIG_SESSION_ADD_DELAY,
    CONFIG_GAME_TYPE,
    CONFIG_REALM_ZONE,
    CONFIG_STRICT_PLAYER_NAMES,
    CONFIG_STRICT_CHARTER_NAMES,
    CONFIG_STRICT_PET_NAMES,
    CONFIG_MIN_PLAYER_NAME,
    CONFIG_MIN_CHARTER_NAME,
    CONFIG_MIN_PET_NAME,
    CONFIG_CHARACTER_CREATING_DISABLED,
    CONFIG_CHARACTER_CREATING_DISABLED_RACEMASK,
    CONFIG_CHARACTER_CREATING_DISABLED_CLASSMASK,
    CONFIG_CHARACTERS_PER_ACCOUNT,
    CONFIG_CHARACTERS_PER_REALM,
    CONFIG_CHARACTER_CREATING_MIN_LEVEL_FOR_HEROIC_CHARACTER,
    CONFIG_SKIP_CINEMATICS,
    CONFIG_MAX_PLAYER_LEVEL,
    CONFIG_MIN_DUALSPEC_LEVEL,
    CONFIG_START_PLAYER_LEVEL,
    CONFIG_START_HEROIC_PLAYER_LEVEL,
    CONFIG_START_PLAYER_MONEY,
    CONFIG_CURRENCY_START_JUSTICE_POINTS,
    CONFIG_CURRENCY_MAX_JUSTICE_POINTS,
    CONFIG_CURRENCY_START_HONOR_POINTS,
    CONFIG_CURRENCY_MAX_HONOR_POINTS,
    CONFIG_CURRENCY_START_CONQUEST_POINTS,
    CONFIG_CURRENCY_CONQUEST_POINTS_WEEK_CAP,
    CONFIG_CURRENCY_VIP_CONQUEST_POINTS_WEEK_CAP,
    CONFIG_CURRENCY_ASHRAN_CONQUEST_POINTS_WEEK_CAP,
    CONFIG_CURRENCY_CONQUEST_POINTS_ARENA_REWARD,
    CONFIG_CURRENCY_CONQUEST_POINTS_RATED_BG_REWARD,
    CONFIG_CURRENCY_RESET_HOUR,
    CONFIG_CURRENCY_RESET_DAY,
    CONFIG_CURRENCY_RESET_INTERVAL,
    CONFIG_MAX_RECRUIT_A_FRIEND_BONUS_PLAYER_LEVEL,
    CONFIG_MAX_RECRUIT_A_FRIEND_BONUS_PLAYER_LEVEL_DIFFERENCE,
    CONFIG_INSTANCE_RESET_TIME_HOUR,
    CONFIG_INSTANCE_UNLOAD_DELAY,
    CONFIG_MAX_PRIMARY_TRADE_SKILL,
    CONFIG_MIN_PETITION_SIGNS,
    CONFIG_GM_LOGIN_STATE,
    CONFIG_GM_VISIBLE_STATE,
    CONFIG_GM_ACCEPT_TICKETS,
    CONFIG_GM_CHAT,
    CONFIG_GM_WHISPERING_TO,
    CONFIG_GM_LEVEL_IN_GM_LIST,
    CONFIG_GM_LEVEL_IN_WHO_LIST,
    CONFIG_START_GM_LEVEL,
    CONFIG_GROUP_VISIBILITY,
    CONFIG_MAIL_DELIVERY_DELAY,
    CONFIG_UPTIME_UPDATE,
    CONFIG_SKILL_CHANCE_ORANGE,
    CONFIG_SKILL_CHANCE_YELLOW,
    CONFIG_SKILL_CHANCE_GREEN,
    CONFIG_SKILL_CHANCE_GREY,
    CONFIG_SKILL_CHANCE_MINING_STEPS,
    CONFIG_SKILL_CHANCE_SKINNING_STEPS,
    CONFIG_SKILL_GAIN_CRAFTING,
    CONFIG_SKILL_GAIN_GATHERING,
    CONFIG_MAX_OVERSPEED_PINGS,
    CONFIG_EXPANSION,
    CONFIG_CHATFLOOD_MESSAGE_COUNT,
    CONFIG_CHATFLOOD_MESSAGE_DELAY,
    CONFIG_CHATFLOOD_MUTE_TIME,
    CONFIG_CHATFLOOD_PRIVATE_MESSAGE_COUNT,
    CONFIG_CHATFLOOD_PRIVATE_MESSAGE_DELAY,
    CONFIG_EVENT_ANNOUNCE,
    CONFIG_CREATURE_FAMILY_ASSISTANCE_DELAY,
    CONFIG_CREATURE_FAMILY_FLEE_DELAY,
    CONFIG_WORLD_BOSS_LEVEL_DIFF,
    CONFIG_QUEST_LOW_LEVEL_HIDE_DIFF,
    CONFIG_QUEST_HIGH_LEVEL_HIDE_DIFF,
    CONFIG_CHAT_STRICT_LINK_CHECKING_SEVERITY,
    CONFIG_CHAT_STRICT_LINK_CHECKING_KICK,
    CONFIG_CHAT_CHANNEL_LEVEL_REQ,
    CONFIG_CHAT_WHISPER_LEVEL_REQ,
    CONFIG_CHAT_SAY_LEVEL_REQ,
    CONFIG_TRADE_LEVEL_REQ,
    CONFIG_TICKET_LEVEL_REQ,
    CONFIG_AUCTION_LEVEL_REQ,
    CONFIG_MAIL_LEVEL_REQ,
    CONFIG_CORPSE_DECAY_NORMAL,
    CONFIG_CORPSE_DECAY_RARE,
    CONFIG_CORPSE_DECAY_ELITE,
    CONFIG_CORPSE_DECAY_RAREELITE,
    CONFIG_CORPSE_DECAY_WORLDBOSS,
    CONFIG_DEATH_SICKNESS_LEVEL,
    CONFIG_INSTANT_LOGOUT,
    CONFIG_DISABLE_BREATHING,
    CONFIG_BATTLEGROUND_INVITATION_TYPE,
    CONFIG_BATTLEGROUND_PREMATURE_FINISH_TIMER,
    CONFIG_BATTLEGROUND_PREMADE_GROUP_WAIT_FOR_MATCH,
    CONFIG_ARENA_MAX_RATING_DIFFERENCE,
    CONFIG_ARENA_RATING_DISCARD_TIMER,
    CONFIG_ARENA_RATED_UPDATE_TIMER,
    CONFIG_ARENA_AUTO_DISTRIBUTE_INTERVAL_DAYS,
    CONFIG_ARENA_SEASON_ID,
    CONFIG_ARENA_START_RATING,
    CONFIG_ARENA_START_PERSONAL_RATING,
    CONFIG_ARENA_START_MATCHMAKER_RATING,
    CONFIG_MAX_WHO,
    CONFIG_HONOR_AFTER_DUEL,
    CONFIG_PVP_TOKEN_MAP_TYPE,
    CONFIG_PVP_TOKEN_ID,
    CONFIG_PVP_TOKEN_COUNT,
    CONFIG_INTERVAL_LOG_UPDATE,
    CONFIG_MIN_LOG_UPDATE,
    CONFIG_ENABLE_SINFO_LOGIN,
    CONFIG_PLAYER_ALLOW_COMMANDS,
    CONFIG_NUMTHREADS,
    CONFIG_LOGDB_CLEARINTERVAL,
    CONFIG_LOGDB_CLEARTIME,
    CONFIG_CLIENTCACHE_VERSION,
    CONFIG_GUILD_EVENT_LOG_COUNT,
    CONFIG_GUILD_BANK_EVENT_LOG_COUNT,
    CONFIG_MIN_LEVEL_STAT_SAVE,
    CONFIG_RANDOM_BG_RESET_HOUR,
    CONFIG_CHARDELETE_KEEP_DAYS,
    CONFIG_CHARDELETE_METHOD,
    CONFIG_CHARDELETE_MIN_LEVEL,
    CONFIG_AUTOBROADCAST_CENTER,
    CONFIG_AUTOBROADCAST_INTERVAL,
    CONFIG_MAX_RESULTS_LOOKUP_COMMANDS,
    CONFIG_DB_PING_INTERVAL,
    CONFIG_PRESERVE_CUSTOM_CHANNEL_DURATION,
    CONFIG_PERSISTENT_CHARACTER_CLEAN_FLAGS,
    CONFIG_MAX_INSTANCES_PER_HOUR,
    CONFIG_WARDEN_CLIENT_RESPONSE_DELAY,
    CONFIG_WARDEN_CLIENT_CHECK_HOLDOFF,
    CONFIG_WARDEN_CLIENT_FAIL_ACTION,
    CONFIG_WARDEN_CLIENT_BAN_DURATION,
    CONFIG_WARDEN_NUM_MEM_CHECKS,
    CONFIG_WARDEN_NUM_OTHER_CHECKS,
    CONFIG_ANTICHEAT_REPORTS_INGAME_NOTIFICATION,
    CONFIG_ANTICHEAT_MAX_REPORTS_FOR_DAILY_REPORT,
    CONFIG_ANTICHEAT_MAX_REPORTS_BEFORE_BAN,
    CONFIG_ANTICHEAT_BAN_CHECK_TIME_RANGE,
    CONFIG_ANTICHEAT_DETECTIONS_ENABLED,
    CONFIG_WINTERGRASP_PLR_MAX,
    CONFIG_WINTERGRASP_PLR_MIN,
    CONFIG_WINTERGRASP_PLR_MIN_LVL,
    CONFIG_WINTERGRASP_BATTLETIME,
    CONFIG_WINTERGRASP_NOBATTLETIME,
    CONFIG_WINTERGRASP_RESTART_AFTER_CRASH,
    CONFIG_GUILD_SAVE_INTERVAL,
    CONFIG_GUILD_MAX_LEVEL,
    CONFIG_GUILD_UNDELETABLE_LEVEL,
    CONFIG_GUILD_DAILY_XP_CAP,
    CONFIG_GUILD_WEEKLY_REP_CAP,
    CONFIG_BLACKMARKET_MAX_AUCTIONS,
    CONFIG_BLACKMARKET_AUCTION_DELAY,
    CONFIG_BLACKMARKET_AUCTION_DELAY_MOD,
    CONFIG_VIP_RATE_EXHANGE_HONOR_IN_ARENA,
    CONFIG_VIP_RATE_EXHANGE_TRIUMPH_IN_FROST,
    CONFIG_ANTISPAM_MAIL_TIMER,
    CONFIG_ANTISPAM_MAIL_COUNT,
    CONFIG_AUTO_SERVER_RESTART_HOUR,
    CONFIG_PVP_ITEM_LEVEL_CUTOFF,
    CONFIG_PVP_ITEM_LEVEL_MIN,
    CONFIG_PVP_ITEM_LEVEL_MAX,
    CONFIG_CHALLENGE_MODE_ITEM_LEVEL_MAX,
    CONFIG_LAST_CLIENT_BUILD,
    CONFIG_FIRST_PREMADE_MONEY,
    CONFIG_BATTLEPAY_MIN_SECURITY,
    CONFIG_SPELLOG_FLAGS,
    CONFIG_ACCOUNT_BIND_GROUP_MASK,
    CONFIG_ACCOUNT_BIND_SHOP_GROUP_MASK,
    CONFIG_ACCOUNT_BIND_ALLOWED_GROUP_MASK,
    CONFIG_ONLY_MAP,
    INT_CONFIG_VALUE_COUNT
};

/// Server rates
enum Rates
{
    RATE_HEALTH = 0,
    RATE_POWER_MANA,
    RATE_POWER_RAGE_INCOME,
    RATE_POWER_RAGE_LOSS,
    RATE_POWER_RUNICPOWER_INCOME,
    RATE_POWER_RUNICPOWER_LOSS,
    RATE_POWER_FOCUS,
    RATE_POWER_ENERGY,
    RATE_SKILL_DISCOVERY,
    RATE_DROP_ITEM_POOR,
    RATE_DROP_ITEM_NORMAL,
    RATE_DROP_ITEM_UNCOMMON,
    RATE_DROP_ITEM_RARE,
    RATE_DROP_ITEM_EPIC,
    RATE_DROP_ITEM_LEGENDARY,
    RATE_DROP_ITEM_ARTIFACT,
    RATE_DROP_ITEM_REFERENCED,
    RATE_DROP_ITEM_REFERENCED_AMOUNT,
    RATE_DROP_MONEY,
    RATE_XP_KILL,
    RATE_XP_QUEST,
    RATE_XP_GUILD_MODIFIER,
    RATE_XP_EXPLORE,
    RATE_XP_GATHERING,
    RATE_REPAIRCOST,
    RATE_REPUTATION_GAIN,
    RATE_REPUTATION_LOWLEVEL_KILL,
    RATE_REPUTATION_LOWLEVEL_QUEST,
    RATE_REPUTATION_RECRUIT_A_FRIEND_BONUS,
    RATE_CREATURE_NORMAL_HP,
    RATE_CREATURE_ELITE_ELITE_HP,
    RATE_CREATURE_ELITE_RAREELITE_HP,
    RATE_CREATURE_ELITE_WORLDBOSS_HP,
    RATE_CREATURE_ELITE_RARE_HP,
    RATE_CREATURE_NORMAL_DAMAGE,
    RATE_CREATURE_ELITE_ELITE_DAMAGE,
    RATE_CREATURE_ELITE_RAREELITE_DAMAGE,
    RATE_CREATURE_ELITE_WORLDBOSS_DAMAGE,
    RATE_CREATURE_ELITE_RARE_DAMAGE,
    RATE_CREATURE_NORMAL_SPELLDAMAGE,
    RATE_CREATURE_ELITE_ELITE_SPELLDAMAGE,
    RATE_CREATURE_ELITE_RAREELITE_SPELLDAMAGE,
    RATE_CREATURE_ELITE_WORLDBOSS_SPELLDAMAGE,
    RATE_CREATURE_ELITE_RARE_SPELLDAMAGE,
    RATE_CREATURE_AGGRO,
    RATE_REST_INGAME,
    RATE_REST_OFFLINE_IN_TAVERN_OR_CITY,
    RATE_REST_OFFLINE_IN_WILDERNESS,
    RATE_DAMAGE_FALL,
    RATE_AUCTION_TIME,
    RATE_AUCTION_DEPOSIT,
    RATE_AUCTION_CUT,
    RATE_HONOR,
    RATE_MINING_AMOUNT,
    RATE_MINING_NEXT,
    RATE_TALENT,
    RATE_CORPSE_DECAY_LOOTED,
    RATE_INSTANCE_RESET_TIME,
    RATE_TARGET_POS_RECALCULATION_RANGE,
    RATE_DURABILITY_LOSS_ON_DEATH,
    RATE_DURABILITY_LOSS_DAMAGE,
    RATE_DURABILITY_LOSS_PARRY,
    RATE_DURABILITY_LOSS_ABSORB,
    RATE_DURABILITY_LOSS_BLOCK,
    RATE_MOVESPEED,
    RATE_ONLINE,
    RATE_XP_KILL_PREMIUM,
    RATE_XP_QUEST_PREMIUM,
    RATE_XP_EXPLORE_PREMIUM,
    RATE_REPUTATION_GAIN_PREMIUM,
    RATE_HONOR_PREMIUM,
    MAX_RATES
};

/// Can be used in SMSG_AUTH_RESPONSE packet
enum BillingPlanFlags
{
    SESSION_NONE            = 0x00,
    SESSION_UNUSED          = 0x01,
    SESSION_RECURRING_BILL  = 0x02,
    SESSION_FREE_TRIAL      = 0x04,
    SESSION_IGR             = 0x08,
    SESSION_USAGE           = 0x10,
    SESSION_TIME_MIXTURE    = 0x20,
    SESSION_RESTRICTED      = 0x40,
    SESSION_ENABLE_CAIS     = 0x80,
};

/// Type of server, this is values from second column of Cfg_Configs.dbc
enum RealmType
{
    REALM_TYPE_NORMAL = 0,
    REALM_TYPE_PVP = 1,
    REALM_TYPE_NORMAL2 = 4,
    REALM_TYPE_RP = 6,
    REALM_TYPE_RPPVP = 8,
    REALM_TYPE_FFA_PVP = 16                                 // custom, free for all pvp mode like arena PvP in all zones except rest activated places and sanctuaries
                                                            // replaced by REALM_PVP in realm list
};

enum RealmZone
{
    REALM_ZONE_UNKNOWN       = 0,                           // any language
    REALM_ZONE_DEVELOPMENT   = 1,                           // any language
    REALM_ZONE_UNITED_STATES = 2,                           // extended-Latin
    REALM_ZONE_OCEANIC       = 3,                           // extended-Latin
    REALM_ZONE_LATIN_AMERICA = 4,                           // extended-Latin
    REALM_ZONE_TOURNAMENT_5  = 5,                           // basic-Latin at create, any at login
    REALM_ZONE_KOREA         = 6,                           // East-Asian
    REALM_ZONE_TOURNAMENT_7  = 7,                           // basic-Latin at create, any at login
    REALM_ZONE_ENGLISH       = 8,                           // extended-Latin
    REALM_ZONE_GERMAN        = 9,                           // extended-Latin
    REALM_ZONE_FRENCH        = 10,                          // extended-Latin
    REALM_ZONE_SPANISH       = 11,                          // extended-Latin
    REALM_ZONE_RUSSIAN       = 12,                          // Cyrillic
    REALM_ZONE_TOURNAMENT_13 = 13,                          // basic-Latin at create, any at login
    REALM_ZONE_TAIWAN        = 14,                          // East-Asian
    REALM_ZONE_TOURNAMENT_15 = 15,                          // basic-Latin at create, any at login
    REALM_ZONE_CHINA         = 16,                          // East-Asian
    REALM_ZONE_CN1           = 17,                          // basic-Latin at create, any at login
    REALM_ZONE_CN2           = 18,                          // basic-Latin at create, any at login
    REALM_ZONE_CN3           = 19,                          // basic-Latin at create, any at login
    REALM_ZONE_CN4           = 20,                          // basic-Latin at create, any at login
    REALM_ZONE_CN5           = 21,                          // basic-Latin at create, any at login
    REALM_ZONE_CN6           = 22,                          // basic-Latin at create, any at login
    REALM_ZONE_CN7           = 23,                          // basic-Latin at create, any at login
    REALM_ZONE_CN8           = 24,                          // basic-Latin at create, any at login
    REALM_ZONE_TOURNAMENT_25 = 25,                          // basic-Latin at create, any at login
    REALM_ZONE_TEST_SERVER   = 26,                          // any language
    REALM_ZONE_TOURNAMENT_27 = 27,                          // basic-Latin at create, any at login
    REALM_ZONE_QA_SERVER     = 28,                          // any language
    REALM_ZONE_CN9           = 29,                          // basic-Latin at create, any at login
    REALM_ZONE_TEST_SERVER_2 = 30,                          // any language
    REALM_ZONE_CN10          = 31,                          // basic-Latin at create, any at login
    REALM_ZONE_CTC           = 32,
    REALM_ZONE_CNC           = 33,
    REALM_ZONE_CN1_4         = 34,                          // basic-Latin at create, any at login
    REALM_ZONE_CN2_6_9       = 35,                          // basic-Latin at create, any at login
    REALM_ZONE_CN3_7         = 36,                          // basic-Latin at create, any at login
    REALM_ZONE_CN5_8         = 37                           // basic-Latin at create, any at login
};

enum WorldStates
{
    WS_WEEKLY_QUEST_RESET_TIME            = 20002,                     ///< Next weekly reset time
    WS_BG_DAILY_RESET_TIME                = 20003,                     ///< Next daily BG reset time
    WS_MONTHLY_QUEST_RESET_TIME           = 20004,                     ///< Next monthly reset time
    WS_AUTO_SERVER_RESTART_TIME           = 20005,                     ///< Next server restart time
    WS_DAILY_LOOT_RESET_TIME              = 20006,                     ///< Next daily loot reset time
    WS_WEEKLY_GUILD_CHALLENGES_RESET_TIME = 20007,                     ///< Next weekly guild challenges reset time
    WS_WEEKLY_BOSS_LOOTED_RESET_TIME      = 20008                      ///< Next weekly boss looted reset time
};

// DB scripting commands
enum ScriptCommands
{
    SCRIPT_COMMAND_TALK                  = 0,                // source/target = Creature, target = any, datalong = talk type (0=say, 1=whisper, 2=yell, 3=emote text, 4=boss emote text), datalong2 & 1 = player talk (instead of creature), dataint = string_id
    SCRIPT_COMMAND_EMOTE                 = 1,                // source/target = Creature, datalong = emote id, datalong2 = 0: set emote state; > 0: play emote state
    SCRIPT_COMMAND_FIELD_SET             = 2,                // source/target = Creature, datalong = field id, datalog2 = value
    SCRIPT_COMMAND_MOVE_TO               = 3,                // source/target = Creature, datalong2 = time to reach, x/y/z = destination
    SCRIPT_COMMAND_FLAG_SET              = 4,                // source/target = Creature, datalong = field id, datalog2 = bitmask
    SCRIPT_COMMAND_FLAG_REMOVE           = 5,                // source/target = Creature, datalong = field id, datalog2 = bitmask
    SCRIPT_COMMAND_TELEPORT_TO           = 6,                // source/target = Creature/Player (see datalong2), datalong = map_id, datalong2 = 0: Player; 1: Creature, x/y/z = destination, o = orientation
    SCRIPT_COMMAND_QUEST_EXPLORED        = 7,                // target/source = Player, target/source = GO/Creature, datalong = quest id, datalong2 = distance or 0
    SCRIPT_COMMAND_KILL_CREDIT           = 8,                // target/source = Player, datalong = creature entry, datalong2 = 0: personal credit, 1: group credit
    SCRIPT_COMMAND_RESPAWN_GAMEOBJECT    = 9,                // source = WorldObject (summoner), datalong = GO guid, datalong2 = despawn delay
    SCRIPT_COMMAND_TEMP_SUMMON_CREATURE  = 10,               // source = WorldObject (summoner), datalong = creature entry, datalong2 = despawn delay, x/y/z = summon position, o = orientation
    SCRIPT_COMMAND_OPEN_DOOR             = 11,               // source = Unit, datalong = GO guid, datalong2 = reset delay (min 15)
    SCRIPT_COMMAND_CLOSE_DOOR            = 12,               // source = Unit, datalong = GO guid, datalong2 = reset delay (min 15)
    SCRIPT_COMMAND_ACTIVATE_OBJECT       = 13,               // source = Unit, target = GO
    SCRIPT_COMMAND_REMOVE_AURA           = 14,               // source (datalong2 != 0) or target (datalong2 == 0) = Unit, datalong = spell id
    SCRIPT_COMMAND_CAST_SPELL            = 15,               // source and/or target = Unit, datalong2 = cast direction (0: s->t 1: s->s 2: t->t 3: t->s 4: s->creature with dataint entry), dataint & 1 = triggered flag
    SCRIPT_COMMAND_PLAY_SOUND            = 16,               // source = WorldObject, target = none/Player, datalong = sound id, datalong2 (bitmask: 0/1=anyone/player, 0/2=without/with distance dependency, so 1|2 = 3 is target with distance dependency)
    SCRIPT_COMMAND_CREATE_ITEM           = 17,               // target/source = Player, datalong = item entry, datalong2 = amount
    SCRIPT_COMMAND_DESPAWN_SELF          = 18,               // target/source = Creature, datalong = despawn delay

    SCRIPT_COMMAND_LOAD_PATH             = 20,               // source = Unit, datalong = path id, datalong2 = is repeatable
    SCRIPT_COMMAND_CALLSCRIPT_TO_UNIT    = 21,               // source = WorldObject (if present used as a search center), datalong = script id, datalong2 = unit lowguid, dataint = script table to use (see ScriptsType)
    SCRIPT_COMMAND_KILL                  = 22,               // source/target = Creature, dataint = remove corpse attribute

    // TrinityCore only
    SCRIPT_COMMAND_ORIENTATION           = 30,               // source = Unit, target (datalong > 0) = Unit, datalong = > 0 turn source to face target, o = orientation
    SCRIPT_COMMAND_EQUIP                 = 31,               // soucre = Creature, datalong = equipment id
    SCRIPT_COMMAND_MODEL                 = 32,               // source = Creature, datalong = model id
    SCRIPT_COMMAND_CLOSE_GOSSIP          = 33,               // source = Player
    SCRIPT_COMMAND_PLAYMOVIE             = 34                // source = Player, datalong = movie id
};

/// Storage class for commands issued for delayed execution
struct CliCommandHolder
{
    typedef void Print(void*, const char*);
    typedef void CommandFinished(void*, bool success);

    void* m_callbackArg;
    char *m_command;
    Print* m_print;

    CommandFinished* m_commandFinished;

    CliCommandHolder(void* callbackArg, const char *command, Print* zprint, CommandFinished* commandFinished)
        : m_callbackArg(callbackArg), m_print(zprint), m_commandFinished(commandFinished)
    {
        size_t len = strlen(command)+1;
        m_command = new char[len];
        memcpy(m_command, command, len);
    }

    ~CliCommandHolder() { delete[] m_command; }
};

typedef std::unordered_map<uint32, WorldSession*> SessionMap;

struct CharacterInfo
{
    std::string Name;
    uint32 AccountId;
    uint8 Class;
    uint8 Race;
    uint8 Sex;
    uint8 Level;
};

enum RecordDiffType
{
    RECORD_DIFF_MAP,
    RECORD_DIFF_BATTLEGROUND,
    RECORD_DIFF_SESSION,
    RECORD_DIFF_BATTLEFIELD,
    RECORD_DIFF_OUTDOORPVP,
    RECORD_DIFF_LFG,
    RECORD_DIFF_CALLBACK,
    RECORD_DIFF_MAX
};

struct QueryHolderCallback
{
    QueryHolderCallback(QueryResultHolderFuture p_QueryResultHolderFuture, std::function<void(SQLQueryHolder*)> p_Callback)
    {
        m_QueryResultHolderFuture = p_QueryResultHolderFuture;
        m_Callback = p_Callback;
    }

    QueryResultHolderFuture m_QueryResultHolderFuture;
    std::function<void(SQLQueryHolder*)>   m_Callback;
};


struct MotdText
{
    std::string Text;
    std::string TextFR;
    std::string TextES;
    std::string TextRU;
};

/// The World
class World
{
    public:
        static std::atomic<unsigned int> m_worldLoopCounter;

        World();
        ~World();

        WorldSession* FindSession(uint32 id) const;
        void AddSession(WorldSession* s);
        void SendAutoBroadcast();
        bool RemoveSession(uint32 id);
        /// Get the number of current active sessions
        void UpdateMaxSessionCounters();
        const SessionMap& GetAllSessions() const { return m_sessions; }
        uint32 GetActiveAndQueuedSessionCount() const { return uint32(m_sessions.size() * getRate(RATE_ONLINE)); }
        uint32 GetActiveSessionCount() const { return uint32((m_sessions.size() - m_QueuedPlayer.size()) * getRate(RATE_ONLINE)); }
        uint32 GetQueuedSessionCount() const { return m_QueuedPlayer.size(); }
        /// Get the maximum number of parallel sessions on the server since last reboot
        uint32 GetMaxQueuedSessionCount() const { return m_maxQueuedSessionCount; }
        uint32 GetMaxActiveSessionCount() const { return uint32(m_maxActiveSessionCount * getRate(RATE_ONLINE)); }
        /// Get number of players
        inline uint32 GetPlayerCount() const { return m_PlayerCount; }
        inline uint32 GetMaxPlayerCount() const { return m_MaxPlayerCount; }
        /// Increase/Decrease number of players
        inline void IncreasePlayerCount()
        {
            m_PlayerCount++;
            m_MaxPlayerCount = std::max(m_MaxPlayerCount, m_PlayerCount);
        }
        inline void DecreasePlayerCount() { m_PlayerCount--; }

        Player* FindPlayerInZone(uint32 zone);

        /// Deny clients?
        bool IsClosed() const;

        /// Close world
        void SetClosed(bool val);

        /// Security level limitations
        AccountTypes GetPlayerSecurityLimit() const { return m_allowedSecurityLevel; }
        void SetPlayerSecurityLimit(AccountTypes sec);
        void LoadDBAllowedSecurityLevel();

        /// Active session server limit
        void SetPlayerAmountLimit(uint32 limit) { m_playerLimit = limit; }
        uint32 GetPlayerAmountLimit() const { return m_playerLimit; }

        //player Queue
        typedef std::list<WorldSession*> Queue;
        void AddQueuedPlayer(WorldSession*);
        bool RemoveQueuedPlayer(WorldSession* session);
        int32 GetQueuePos(WorldSession*);
        bool HasRecentlyDisconnected(WorldSession*);

        /// \todo Actions on m_allowMovement still to be implemented
        /// Is movement allowed?
        bool getAllowMovement() const { return m_allowMovement; }
        /// Allow/Disallow object movements
        void SetAllowMovement(bool allow) { m_allowMovement = allow; }

        void LoadDBMotd();
        void SetDBMotd(MotdText p_MotdText);

        /// Get the current Message of the Day
        MotdText const& GetMotd() const;

        /// Set the string for new characters (first login)
        void SetNewCharString(std::string str) { m_newCharString = str; }
        /// Get the string for new characters (first login)
        const std::string& GetNewCharString() const { return m_newCharString; }

        LocaleConstant GetDefaultDbcLocale() const { return m_defaultDbcLocale; }

        /// Get the path where data (dbc, maps) are stored on disk
        std::string GetDataPath() const { return m_dataPath; }

        /// When server started?
        time_t const& GetStartTime() const { return m_startTime; }
        /// What time is it?
        time_t const& GetGameTime() const { return m_gameTime; }
        /// Get server region ID (used in wow time calculation)
        uint32 GetServerRegionID() const { return 1135753200; }
        /// Get server raid origin (used in wow time calculation)
        uint32 GetServerRaidOrigin() const { return 0; }
        /// Uptime (in secs)
        uint32 GetUptime() const { return uint32(m_gameTime - m_startTime); }
        /// Update time
        uint32 GetUpdateTime() const { return m_updateTime; }
        void SetRecordDiffInterval(int32 t) { if (t >= 0) m_int_configs[CONFIG_INTERVAL_LOG_UPDATE] = (uint32)t; }

        /// Next daily quests and random bg reset time
        time_t GetNextDailyQuestsResetTime() const { return m_NextDailyQuestReset; }
        time_t GetNextWeeklyQuestsResetTime() const { return m_NextWeeklyQuestReset; }
        time_t GetNextRandomBGResetTime() const { return m_NextRandomBGReset; }

        std::vector<uint32> GetMapsToLoad() const { return m_MapsToLoad; }

        /// Get the maximum skill level a player can reach
        uint16 GetConfigMaxSkillValue() const
        {
            uint8 lvl = uint8(getIntConfig(CONFIG_MAX_PLAYER_LEVEL));

            if (lvl >= 1 && lvl < 10)
                return 75;
            else if (lvl >= 10 && lvl < 20)
                return 150;
            else if (lvl >= 20 && lvl < 35)
                return 225;
            else if (lvl >= 35 && lvl < 50)
                return 300;
            else if (lvl >= 50 && lvl < 65)
                return 375;
            else if (lvl >= 65 && lvl < 75)
                return 450;
            else if (lvl >= 75 && lvl < 81)
                return 525;
            else if (lvl >= 81 && lvl < 90)
                return 600;
            else if (lvl >= 90)
                return 700;

            return 0;
        }

        void SetInitialWorldSettings();
        void LoadConfigSettings(bool reload = false);

        void SendWorldText(int32 string_id, ...);
        void SendGlobalText(const char* text, WorldSession* self);
        void SendGMText(int32 string_id, ...);
        void SendGlobalMessage(WorldPacket* packet, WorldSession* self = 0, uint32 team = 0);
        void SendGlobalGMMessage(WorldPacket* packet, WorldSession* self = 0, uint32 team = 0);
        void SendZoneMessage(uint32 zone, WorldPacket* packet, WorldSession* self = 0, uint32 team = 0);
        void SendZoneText(uint32 zone, const char *text, WorldSession* self = 0, uint32 team = 0);
        void SendServerMessage(ServerMessageType type, const char *text = "", Player* player = NULL);

        /// Are we in the middle of a shutdown?
        bool IsShuttingDown() const { return m_ShutdownTimer > 0; }
        uint32 GetShutDownTimeLeft() const { return m_ShutdownTimer; }
        void ShutdownServ(uint32 time, uint32 options, uint8 exitcode, const std::string& reason = std::string());
        void ShutdownCancel();
        void ShutdownMsg(bool show = false, Player* player = NULL, const std::string& reason = std::string());
        static uint8 GetExitCode() { return m_ExitCode; }
        static void StopNow(uint8 exitcode) { m_stopEvent = true; m_ExitCode = exitcode; }
        static bool IsStopped() { return m_stopEvent; }

        void Update(uint32 diff);

        void UpdateSessions(uint32 diff);
        /// Set a server rate (see #Rates)
        void setRate(Rates rate, float value) { rate_values[rate]=value; }
        /// Get a server rate (see #Rates)
        float getRate(Rates rate) const { return rate_values[rate]; }

        /// Set a server configuration element (see #WorldConfigs)
        void setBoolConfig(WorldBoolConfigs index, bool value)
        {
            if (index < BOOL_CONFIG_VALUE_COUNT)
                m_bool_configs[index] = value;
        }

        /// Get a server configuration element (see #WorldConfigs)
        bool getBoolConfig(WorldBoolConfigs index) const
        {
            return index < BOOL_CONFIG_VALUE_COUNT ? m_bool_configs[index] : 0;
        }

        /// Set a server configuration element (see #WorldConfigs)
        void setFloatConfig(WorldFloatConfigs index, float value)
        {
            if (index < FLOAT_CONFIG_VALUE_COUNT)
                m_float_configs[index] = value;
        }

        /// Get a server configuration element (see #WorldConfigs)
        float getFloatConfig(WorldFloatConfigs index) const
        {
            return index < FLOAT_CONFIG_VALUE_COUNT ? m_float_configs[index] : 0;
        }

        /// Set a server configuration element (see #WorldConfigs)
        void setIntConfig(WorldIntConfigs index, uint32 value)
        {
            if (index < INT_CONFIG_VALUE_COUNT)
                m_int_configs[index] = value;
        }

        /// Get a server configuration element (see #WorldConfigs)
        uint32 getIntConfig(WorldIntConfigs index) const
        {
            return index < INT_CONFIG_VALUE_COUNT ? m_int_configs[index] : 0;
        }

        bool CanBeSaveInLoginDatabase() const;
        void setWorldState(uint32 index, uint64 value);
        uint64 getWorldState(uint32 index) const;
        void LoadWorldStates();
        void FillMapsToLoad();

        /// Are we on a "Player versus Player" server?
        bool IsPvPRealm() const { return (getIntConfig(CONFIG_GAME_TYPE) == REALM_TYPE_PVP || getIntConfig(CONFIG_GAME_TYPE) == REALM_TYPE_RPPVP || getIntConfig(CONFIG_GAME_TYPE) == REALM_TYPE_FFA_PVP); }
        bool IsFFAPvPRealm() const { return getIntConfig(CONFIG_GAME_TYPE) == REALM_TYPE_FFA_PVP; }

        void KickAll();
        void KickAllLess(AccountTypes sec);
        BanReturn BanAccount(BanMode mode, std::string nameOrIP, std::string duration, std::string reason, std::string author);
        bool RemoveBanAccount(BanMode mode, std::string nameOrIP);
        BanReturn BanCharacter(std::string name, std::string duration, std::string reason, std::string author);
        bool RemoveBanCharacter(std::string name);

        // for max speed access
        static float GetMaxVisibleDistanceOnContinents()    { return m_MaxVisibleDistanceOnContinents; }
        static float GetMaxVisibleDistanceInInstances()     { return m_MaxVisibleDistanceInInstances;  }
        static float GetMaxVisibleDistanceInBG()            { return m_MaxVisibleDistanceInBG;         }
        static float GetMaxVisibleDistanceInArenas()        { return m_MaxVisibleDistanceInArenas;     }

        static int32 GetVisibilityNotifyPeriodOnContinents(){ return m_visibility_notify_periodOnContinents; }
        static int32 GetVisibilityNotifyPeriodInInstances() { return m_visibility_notify_periodInInstances;  }
        static int32 GetVisibilityNotifyPeriodInBGArenas()  { return m_visibility_notify_periodInBGArenas;   }

        static float Visibility_RelocationLowerLimit;
        static uint32 Visibility_AINotifyDelay;

        void ProcessCliCommands();
        void QueueCliCommand(CliCommandHolder* commandHolder) { cliCmdQueue.add(commandHolder); }

        void ForceGameEventUpdate();

        void UpdateRealmCharCount(uint32 accid);

        LocaleConstant GetAvailableDbcLocale(LocaleConstant locale) const { if (m_availableDbcLocaleMask & (1 << locale)) return locale; else return m_defaultDbcLocale; }

        // used World DB version
        void LoadDBVersion();
        char const* GetDBVersion() const { return m_DBVersion.c_str(); }

        void RecordTimeDiff(const char * text, ...);

        void LoadAutobroadcasts();

        void UpdateAreaDependentAuras();

        void ProcessStartEvent();
        void ProcessStopEvent();
        bool GetEventKill() const { return isEventKillStart; }

        bool isEventKillStart;

        CharacterInfo const* GetCharacterInfo(uint32 guid) const;
        void AddCharacterInfo(uint32 guid, std::string const& name, uint32 accountId, uint8 gender, uint8 race, uint8 playerClass, uint8 level);
        void UpdateCharacterInfo(uint32 guid, std::string const& name, uint8 gender = GENDER_NONE, uint8 race = RACE_NONE);
        void UpdateCharacterInfoLevel(uint32 guid, uint8 level);
        void DeleteCharacterInfo(uint32 guid) { _characterInfoStore.erase(guid); }
        bool HasCharacterInfo(uint32 guid) { return _characterInfoStore.find(guid) != _characterInfoStore.end(); }
        uint64 GetCharacterGuidByName(std::string const& p_Name);

        void SetInterRealmSession(InterRealmSession* irt) { m_InterRealmSession = irt; }
        InterRealmSession* GetInterRealmSession() { return m_InterRealmSession; }

        uint32 GetCleaningFlags() const { return m_CleaningFlags; }
        void   SetCleaningFlags(uint32 flags) { m_CleaningFlags = flags; }
        void   ResetEventSeasonalQuests(uint16 event_id);
        std::string GetRealmName() { return m_realmName; }
        std::string GetNormalizedRealmName() const;

        void UpdatePhaseDefinitions();

        void SetRecordDiff(RecordDiffType recordDiff, uint32 diff) { m_recordDiff[recordDiff] = diff; }
        uint32 GetRecordDiff(RecordDiffType recordDiff) { return m_recordDiff[recordDiff]; }

        void ResetCurrencyWeekCap();
        void ResetDailyLoots();
        void ResetGuildChallenges();
        void ResetBossLooted();

        bool ModerateMessage(std::string l_Text);

        //////////////////////////////////////////////////////////////////////////
        /// New callback system
        //////////////////////////////////////////////////////////////////////////
        void AddTransactionCallback(std::shared_ptr<MS::Utilities::Callback> p_Callback)
        {
            m_TransactionCallbackLock.lock();
            m_TransactionCallbacksBuffer->push_front(p_Callback);
            m_TransactionCallbackLock.unlock();
        }

        void AddPrepareStatementCallback(std::pair<std::function<void(PreparedQueryResult)>, PreparedQueryResultFuture> p_Callback)
        {
            m_PreparedStatementCallbackLock.lock();
            m_PreparedStatementCallbacksBuffer->push_front(p_Callback);
            m_PreparedStatementCallbackLock.unlock();
        }

        void AddQueryHolderCallback(QueryHolderCallback p_QueryHolderCallback)
        {
            m_QueryHolderCallbackLock.lock();
            m_QueryHolderCallbacksBuffer->push_front(p_QueryHolderCallback);
            m_QueryHolderCallbackLock.unlock();
        }

    protected:
        void _UpdateGameTime();
        // callback for UpdateRealmCharacters
        void _UpdateRealmCharCount(PreparedQueryResult resultCharCount);
        void _updateTransfers();

        void InitDailyQuestResetTime();
        void InitWeeklyQuestResetTime();
        void InitMonthlyQuestResetTime();
        void InitRandomBGResetTime();
        //void InitServerAutoRestartTime();
        void InitCurrencyResetTime();
        void InitDailyLootResetTime();
        void InitGuildChallengesResetTime();
        void InitBossLootedResetTime();
        void ResetDailyQuests();
        void ResetDailyGarrisonDatas();
        void ResetWeeklyQuests();
        void ResetWeeklyGarrisonDatas();
        void ResetMonthlyQuests();
        void ResetRandomBG();
        //void AutoRestartServer();
    private:
        InterRealmSession* m_InterRealmSession;
        static std::atomic<bool> m_stopEvent;
        static uint8 m_ExitCode;
        uint32 m_ShutdownTimer;
        uint32 m_ShutdownMask;

        uint32 m_CleaningFlags;

        bool m_isClosed;

        time_t m_startTime;
        time_t m_gameTime;
        IntervalTimer m_timers[WUPDATE_COUNT];
        time_t mail_timer;
        time_t mail_timer_expires;
        uint32 m_updateTime, m_updateTimeSum;
        uint32 m_updateTimeCount;
        uint32 m_currentTime;

        uint32 m_serverDelayTimer;
        uint32 m_serverDelaySum;
        uint32 m_serverUpdateCount;

        SessionMap m_sessions;
        typedef std::unordered_map<uint32, time_t> DisconnectMap;
        DisconnectMap m_disconnects;
        uint32 m_maxActiveSessionCount;
        uint32 m_maxQueuedSessionCount;
        uint32 m_PlayerCount;
        uint32 m_MaxPlayerCount;

        std::string m_newCharString;
        std::string m_realmName;

        float rate_values[MAX_RATES];
        uint32 m_int_configs[INT_CONFIG_VALUE_COUNT];
        bool m_bool_configs[BOOL_CONFIG_VALUE_COUNT];
        float m_float_configs[FLOAT_CONFIG_VALUE_COUNT];
        typedef std::map<uint32, uint64> WorldStatesMap;
        WorldStatesMap m_worldstates;
        std::vector<uint32> m_MapsToLoad;
        uint32 m_playerLimit;
        AccountTypes m_allowedSecurityLevel;
        LocaleConstant m_defaultDbcLocale;                     // from config for one from loaded DBC locales
        uint32 m_availableDbcLocaleMask;                       // by loaded DBC
        void DetectDBCLang();
        bool m_allowMovement;
        std::string m_dataPath;
        MotdText m_Motd;

        // for max speed access
        static float m_MaxVisibleDistanceOnContinents;
        static float m_MaxVisibleDistanceInInstances;
        static float m_MaxVisibleDistanceInBG;
        static float m_MaxVisibleDistanceInArenas;

        static int32 m_visibility_notify_periodOnContinents;
        static int32 m_visibility_notify_periodInInstances;
        static int32 m_visibility_notify_periodInBGArenas;

        // CLI command holder to be thread safe
        ACE_Based::LockedQueue<CliCommandHolder*, ACE_Thread_Mutex> cliCmdQueue;

        // next daily quests and random bg reset time
        time_t m_NextDailyQuestReset;
        time_t m_NextWeeklyQuestReset;
        time_t m_NextMonthlyQuestReset;
        time_t m_NextRandomBGReset;
        time_t m_NextCurrencyReset;
        time_t m_NextDailyLootReset;
        time_t m_NextGuildChallengesReset;
        time_t m_NextBossLootedReset;
        time_t m_NextServerRestart;

        //Player Queue
        Queue m_QueuedPlayer;

        // sessions that are added async
        void AddSession_(WorldSession* s);
        ACE_Based::LockedQueue<WorldSession*, ACE_Thread_Mutex> addSessQueue;

        // used versions
        std::string m_DBVersion;

        struct AutoBroadcastText
        {
            std::string Text;
            std::string TextFR;
            std::string TextRU;
            std::string TextES;
        };

        std::list<AutoBroadcastText> m_Autobroadcasts;

        typedef std::unordered_map<uint32, CharacterInfo> CharacterInfoContainer;
        CharacterInfoContainer _characterInfoStore;
        void LoadCharacterInfoStore();

        void ProcessQueryCallbacks();
        ACE_Future_Set<PreparedQueryResult> m_realmCharCallbacks;
        PreparedQueryResultFuture m_transfersDumpCallbacks;
        PreparedQueryResultFuture m_transfersLoadCallbacks;
        PreparedQueryResultFuture m_transfersExpLoadCallback;
        uint32 m_recordDiff[RECORD_DIFF_MAX];
        LexicsCutter *m_lexicsCutter;

        //////////////////////////////////////////////////////////////////////////
        /// New query holder callback system
        //////////////////////////////////////////////////////////////////////////
        using QueryHolderCallbacks = std::forward_list<QueryHolderCallback>;
        std::unique_ptr<QueryHolderCallbacks> m_QueryHolderCallbacks;
        std::unique_ptr<QueryHolderCallbacks> m_QueryHolderCallbacksBuffer;
        std::mutex m_QueryHolderCallbackLock;

        //////////////////////////////////////////////////////////////////////////
        /// New transaction query callback system
        //////////////////////////////////////////////////////////////////////////
        using TransactionCallbacks = std::forward_list<std::shared_ptr<MS::Utilities::Callback>>;
        std::unique_ptr<TransactionCallbacks> m_TransactionCallbacks;
        std::unique_ptr<TransactionCallbacks> m_TransactionCallbacksBuffer;
        std::mutex m_TransactionCallbackLock;

        //////////////////////////////////////////////////////////////////////////
        /// New prepare statement query callback system
        //////////////////////////////////////////////////////////////////////////
        using PrepareStatementCallback = std::pair<std::function<void(PreparedQueryResult)>, PreparedQueryResultFuture>;
        using PreparedStatementCallbacks = std::forward_list<PrepareStatementCallback>;
        std::unique_ptr<PreparedStatementCallbacks> m_PreparedStatementCallbacks;
        std::unique_ptr<PreparedStatementCallbacks> m_PreparedStatementCallbacksBuffer;
        std::mutex m_PreparedStatementCallbackLock;
};

extern uint32 g_RealmID;

#define sWorld ACE_Singleton<World, ACE_Null_Mutex>::instance()
#endif
/// @}
