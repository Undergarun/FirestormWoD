////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/// \addtogroup u2w
/// @{
/// \file

#ifndef _OPCODES_H
#define _OPCODES_H

#include "Common.h"

enum OpcodeTransferDirection : uint8
{
    WOW_SERVER_TO_CLIENT = 0,
    WOW_CLIENT_TO_SERVER = 1,

    TRANSFER_DIRECTION_MAX = 2
};

/// List of Opcodes
enum Opcodes
{
    MAX_OPCODE                                        = (0x7FFF+1),
    NUM_OPCODE_HANDLERS                               = MAX_OPCODE + 1,
    UNKNOWN_OPCODE                                    = (0xFFFF+1),
    NULL_OPCODE                                       = 0,
    COMPRESSED_OPCODE_MASK                            = 0x8000,

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Protocol
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamProtocol
        /// Opcodes
        SMSG_AUTH_CHALLENGE                         = 0x0307, ///< 6.2.3 20726
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Dispatch
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamDispatch
        /// Base opcodes
        SMSG_AUTH_RESPONSE                          = 0x1357, ///< 6.2.3 20726
        SMSG_CACHE_VERSION                          = 0x1747, ///< 6.2.3 20726
        SMSG_ADDON_INFO                             = 0x026F, ///< 6.2.3 20726
        SMSG_FEATURE_SYSTEM_STATUS                  = 0x1AC4, ///< 6.2.3 20726
        SMSG_SET_TIME_ZONE_INFORMATION              = 0x17E0, ///< 6.2.3 20726
        SMSG_LOGIN_VERIFY_WORLD                     = 0x12C8, ///< 6.2.3 20726
        SMSG_NEW_WORLD                              = 0x0AEB, ///< 6.2.3 20726
        SMSG_LOGIN_SET_TIME_SPEED                   = 0x1BEC, ///< 6.2.3 20726
        SMSG_HOTFIX_NOTIFY_BLOB                     = 0x0748, ///< 6.2.3 20726
        SMSG_HOTFIX_NOTIFY                          = 0x0BEC, ///< 6.2.3 20726
        SMSG_WORLD_SERVER_INFO                      = 0x1B78, ///< 6.2.3 20726
        SMSG_INITIAL_SETUP                          = 0x07F0, ///< 6.2.3 20726
        SMSG_PONG                                   = 0x0288, ///< 6.2.3 20726
        SMSG_UITIME                                 = 0x1B4C, ///< 6.2.3 20726

        /// Query
        SMSG_DB_REPLY                               = 0x0E7B, ///< 6.2.3 20726
        SMSG_QUERY_CREATURE_RESPONSE                = 0x134F, ///< 6.2.3 20726
        SMSG_NPC_TEXT_UPDATE                        = 0x02F7, ///< 6.2.3 20726
        SMSG_NAME_QUERY_RESPONSE                    = 0x17D0, ///< 6.2.3 20726
        SMSG_REALM_QUERY_RESPONSE                   = 0x0ED8, ///< 6.2.3 20726
        SMSG_GAMEOBJECT_QUERY_RESPONSE              = 0x03EB, ///< 6.2.3 20726
        SMSG_QUERY_TIME_RESPONSE                    = 0x1B7C, ///< 6.2.3 20726

        /// Misc
        SMSG_DISPLAY_GAME_ERROR                     = 0x13EF, ///< 6.2.3 20726
        SMSG_UPDATE_ACTION_BUTTONS                  = 0x17F8, ///< 6.2.3 20726
        SMSG_SET_PROFICIENCY                        = 0x0668, ///< 6.2.3 20726
        SMSG_INIT_WORLD_STATES                      = 0x0770, ///< 6.2.3 20726
        SMSG_UPDATE_WORLD_STATE                     = 0x0A67, ///< 6.2.3 20726
        SMSG_EMOTE                                  = 0x06C4, ///< 6.2.3 20726
        SMSG_EXPLORATION_EXPERIENCE                 = 0x0780, ///< 6.2.3 20726
        SMSG_LOG_XP_GAIN                            = 0x12F4, ///< 6.2.3 20726
        SMSG_LEVELUP_INFO                           = 0x1643, ///< 6.2.3 20726
        SMSG_ITEM_PUSH_RESULT                       = 0x1370, ///< 6.2.3 20726
        SMSG_INIT_CURRENCY                          = 0x0F48, ///< 6.2.3 20726
        SMSG_UPDATE_CURRENCY                        = 0x0BFC, ///< 6.2.3 20726
        SMSG_UPDATE_CURRENCY_WEEK_LIMIT             = 0x0F63, ///< 6.2.3 20726
        SMSG_WEEKLY_RESET_CURRENCY                  = 0x0EF0, ///< 6.2.3 20726
        SMSG_EQUIPMENT_SET_LIST                     = 0x0360, ///< 6.2.3 20726
        SMSG_BINDER_CONFIRM                         = 0x0A58, ///< 6.2.3 20726
        SMSG_BIND_POINT_UPDATE                      = 0x075B, ///< 6.2.3 20726
        SMSG_UPDATE_TALENT_DATA                     = 0x0277, ///< 6.2.3 20726
        SMSG_TRANSFER_ABORTED                       = 0x12DB, ///< 6.2.3 20726
        SMSG_TRANSFER_PENDING                       = 0x0348, ///< 6.2.3 20726
        SMSG_INVENTORY_CHANGE_FAILURE               = 0x1268, ///< 6.2.3 20726
        SMSG_SET_DUNGEON_DIFFICULTY                 = 0x0647, ///< 6.2.3 20726
        SMSG_SET_RAID_DIFFICULTY                    = 0x1368, ///< 6.2.3 20726
        SMSG_LOAD_CUF_PROFILES                      = 0x17C4, ///< 6.2.3 20726
        SMSG_STANDSTATE_UPDATE                      = 0x12E0, ///< 6.2.3 20726
        SMSG_START_TIMER                            = 0x1A4B, ///< 6.2.3 20726
        SMSG_START_ELAPSED_TIMER                    = 0x166F, ///< 6.2.3 20726
        SMSG_START_ELAPSED_TIMERS                   = 0x02CC, ///< 6.2.3 20726
        SMSG_STOP_ELAPSED_TIMER                     = 0x0ED0, ///< 6.2.3 20726
        SMSG_DEATH_RELEASE_LOC                      = 0x12FF, ///< 6.2.3 20726
        SMSG_CORPSE_RECLAIM_DELAY                   = 0x1BC7, ///< 6.2.3 20726
        SMSG_CORPSE_LOCATION                        = 0x12C3, ///< 6.2.3 20726
        SMSG_CORPSE_TRANSPORT_QUERY                 = 0x03EC, ///< 6.2.3 20726
        SMSG_DURABILITY_DAMAGE_DEATH                = 0x1AD8, ///< 6.2.3 20726
        SMSG_PLAY_MUSIC                             = 0x1B57, ///< 6.2.3 20726
        SMSG_PLAY_OBJECT_SOUND                      = 0x0A4C, ///< 6.2.3 20726
        SMSG_PLAY_SOUND                             = 0x0F6F, ///< 6.2.3 20726
        SMSG_DISPLAY_TOAST                          = 0x13C4, ///< 6.2.3 20726
        SMSG_ITEM_ENCHANT_TIME_UPDATE               = 0x1A50, ///< 6.2.3 20726
        SMSG_ENCHANTMENT_LOG                        = 0x16CF, ///< 6.2.3 20726
        SMSG_PARTY_KILL_LOG                         = 0x03EF, ///< 6.2.3 20726
        SMSG_PROCRESIST                             = 0x1677, ///< 6.2.3 20726
        SMSG_CLEAR_TARGET                           = 0x0E44, ///< 6.2.3 20726
        SMSG_WEATHER                                = 0x12FC, ///< 6.2.3 20726
        SMSG_SET_PHASE_SHIFT                        = 0x137C, ///< 6.2.3 20726
        SMSG_TRIGGER_CINEMATIC                      = 0x16FF, ///< 6.2.3 20726
        SMSG_TOTEM_CREATED                          = 0x0BC7, ///< 6.2.3 20726
        SMSG_RESPEC_WIPE_CONFIRM                    = 0x1BE8, ///< 6.2.3 20726
        SMSG_USE_EQUIPMENT_SET_RESULT               = 0x0B57, ///< 6.2.3 20726
        SMSG_TRADE_STATUS                           = 0x1253, ///< 6.2.3 20726
        SMSG_TRADE_UPDATED                          = 0x0AF3, ///< 6.2.3 20726
        SMSG_SHOW_NEUTRAL_PLAYER_FACTION_SELECT_UI  = 0x12FB, ///< 6.2.3 20726
        SMSG_NEUTRAL_PLAYER_FACTION_SELECT_RESULT   = 0x0000, ///< (unused)
        SMSG_INSPECT_TALENT                         = 0x0F7B, ///< 6.2.3 20726
        SMSG_INSPECT_HONOR_STATS                    = 0x1B77, ///< 6.2.3 20726
        SMSG_INSPECT_RATED_BG_STATS                 = 0x065F, ///< 6.2.3 20726
        SMSG_TIME_SYNC_REQUEST                      = 0x0101, ///< 6.2.3 20726
        SMSG_RESUME_TOKEN                           = 0x1373, ///< 6.2.3 20726
        SMSG_BARBER_SHOP_RESULT                     = 0x1348, ///< 6.2.3 20726
        SMSG_ENABLE_BARBER_SHOP                     = 0x1774, ///< 6.2.3 20726
        SMSG_PAGE_TEXT_QUERY_RESPONSE               = 0x1B58, ///< 6.2.3 20726
        SMSG_PAGE_TEXT                              = 0x03FB, ///< 6.2.3 20726
        SMSG_READ_ITEM_FAILED                       = 0x0ED3, ///< 6.2.3 20726
        SMSG_READ_ITEM_OK                           = 0x0AF7, ///< 6.2.3 20726
        SMSG_TEXT_EMOTE                             = 0x13E4, ///< 6.2.3 20726
        SMSG_TITLE_EARNED                           = 0x125C, ///< 6.2.3 20726
        SMSG_TITLE_LOST                             = 0x06D8, ///< 6.2.3 20726
        SMSG_WHOIS                                  = 0x0FD4, ///< 6.2.3 20726
        SMSG_TRIGGER_MOVIE                          = 0x16F3, ///< 6.2.3 20726
        SMSG_UPDATE_LAST_INSTANCE                   = 0x0B7C, ///< 6.2.3 20726
        SMSG_UPDATE_INSTANCE_OWNERSHIP              = 0x175B, ///< 6.2.3 20726
        SMSG_SUMMON_REQUEST                         = 0x065B, ///< 6.2.3 20726
        SMSG_STOP_MIRROR_TIMER                      = 0x13EB, ///< 6.2.3 20726
        SMSG_START_MIRROR_TIMER                     = 0x166C, ///< 6.2.3 20726
        SMSG_SOCKET_GEMS                            = 0x025C, ///< 6.2.3 20726
        SMSG_RESURRECT_REQUEST                      = 0x06FC, ///< 6.2.3 20726
        SMSG_RESPOND_INSPECT_ACHIEVEMENTS           = 0x0F4C, ///< 6.2.3 20726
        SMSG_RESET_FAILED_NOTIFY                    = 0x0270, ///< 6.2.3 20726
        SMSG_SETUP_RESEARCH_HISTORY                 = 0x126F, ///< 6.2.3 20726
        SMSG_RESEARCH_COMPLETE                      = 0x1AEC, ///< 6.2.3 20726
        SMSG_REQUEST_CEMETERY_LIST_RESPONSE         = 0x1344, ///< 6.2.3 20726
        SMSG_PLAYED_TIME                            = 0x0EDF, ///< 6.2.3 20726
        SMSG_BREAK_TARGET                           = 0x1B00, ///< 6.2.3 20726
        SMSG_CANCEL_AUTO_REPEAT                     = 0x1800, ///< 6.2.3 20726
        SMSG_CANCEL_COMBAT                          = 0x02DB, ///< 6.2.3 20726
        SMSG_COMPLAIN_RESULT                        = 0x0F80, ///< 6.2.3 20726
        SMSG_XPGAIN_ABORTED                         = 0x1300, ///< 6.2.3 20726
        SMSG_SUPERCEDED_SPELL                       = 0x04F4, ///< 6.2.3 20726
        SMSG_PVP_CREDIT                             = 0x1673, ///< 6.2.3 20726
        SMSG_PRE_RESURRECT                          = 0x0C00, ///< 6.2.3 20726
        SMSG_PLAY_ONE_SHOT_ANIM_KIT                 = 0x0344, ///< 6.2.3 20726
        SMSG_SET_AI_ANIM_KIT                        = 0x13C3, ///< 6.2.3 20726
        SMSG_SET_MELEE_ANIM_KIT                     = 0x17CF, ///< 6.2.3 20726
        SMSG_SET_ANIM_TIER                          = 0x03E4, ///< 6.2.3 20726
        SMSG_SET_PLAY_HOVER_ANIM                    = 0x12E4, ///< 6.2.3 20726
        SMSG_SET_MOVEMENT_ANIM_KIT                  = 0x1A5B, ///< 6.2.3 20726
        SMSG_PLAYER_BOUND                           = 0x12CB, ///< 6.2.3 20726
        SMSG_OVERRIDE_LIGHT                         = 0x13CC, ///< 6.2.3 20726
        SMSG_PRINT_NOTIFICATION                     = 0x03FC, ///< 6.2.3 20726
        SMSG_SPECIAL_MOUNT_ANIM                     = 0x0274, ///< 6.2.3 20726
        SMSG_ITEM_TIME_UPDATE                       = 0x13EC, ///< 6.2.3 20726
        SMSG_QUERY_ITEM_TEXT_RESPONSE               = 0x037F, ///< 6.2.3 20726
        SMSG_ITEM_PURCHASE_REFUND_RESULT            = 0x126C, ///< 6.2.3 20726
        SMSG_GAMEOBJECT_DESPAWN                     = 0x137F, ///< 6.2.3 20726
        SMSG_GAMEOBJECT_CUSTOM_ANIM                 = 0x1ACC, ///< 6.2.3 20726
        SMSG_GAME_OBJECT_ACTIVATE_ANIM_KIT          = 0x0EDC, ///< 6.2.3 20726
        SMSG_FISH_NOT_HOOKED                        = 0x13FF, ///< 6.2.3 20726
        SMSG_FISH_ESCAPED                           = 0x0AF0, ///< 6.2.3 20726
        SMSG_DESTRUCTIBLE_BUILDING_DAMAGE           = 0x03CC, ///< 6.2.3 20726
        SMSG_CROSSED_INEBRIATION_THRESHOLD          = 0x12EF, ///< 6.2.3 20726
        SMSG_CLIENT_CONTROL_UPDATE                  = 0x07D0, ///< 6.2.3 20726
        SMSG_AREA_TRIGGER_NO_CORPSE                 = 0x1B43, ///< 6.2.3 20726
        SMSG_BAG_SORT_RESULT                        = 0x0BF3, ///< 6.2.3 20726
        SMSG_VIGNETTE_UPDATE                        = 0x0EF7, ///< 6.2.3 20726
        SMSG_ACCOUNT_MOUNT_UPDATE                   = 0x1350, ///< 6.2.3 20726
        SMSG_PLAY_SCENE                             = 0x1778, ///< 6.2.3 20726
        SMSG_CANCEL_SCENE                           = 0x0BE3, ///< 6.2.3 20726
        SMSG_TALENTS_INVOLUNTARILY_RESET            = 0x035F, ///< 6.2.3 20726
        SMSG_SET_ITEM_PURCHASE_DATA                 = 0x165B, ///< 6.2.3 20726
        SMSG_SHOW_TRADE_SKILL_RESPONSE              = 0x0B77, ///< 6.2.3 20726
        SMSG_FORCED_DEATH_UPDATE                    = 0x074C, ///< 6.2.3 20779

        /// Control Alert
        SMSG_LOSS_OF_CONTROL_AURA_UPDATE            = 0x0254, ///< 6.2.3 20726
        SMSG_ADD_LOSS_OF_CONTROL                    = 0x1754, ///< 6.2.3 20726
        SMSG_CLEAR_LOSS_OF_CONTROL                  = 0x07EB, ///< 6.2.3 20726
        SMSG_REMOVE_LOSS_OF_CONTROL                 = 0x0364, ///< 6.2.3 20726

        /// Reputations
        SMSG_INITIALIZE_FACTIONS                    = 0x0EC4, ///< 6.2.3 20726
        SMSG_FACTION_BONUS_INFO                     = 0x1248, ///< 6.2.3 20726
        SMSG_SET_FACTION_VISIBLE                    = 0x0AD4, ///< 6.2.3 20726
        SMSG_SET_FACTION_NOT_VISIBLE                = 0x0A80, ///< 6.2.3 20726
        SMSG_SET_FACTION_STANDING                   = 0x13C8, ///< 6.2.3 20726
        SMSG_SET_FACTION_ATWAR                      = 0x02F4, ///< 6.2.3 20726
        SMSG_SET_FORCED_REACTIONS                   = 0x0A6C, ///< 6.2.3 20726

        /// Interaction
        SMSG_LOGOUT_RESPONSE                        = 0x0664, ///< 6.2.3 20726
        SMSG_LOGOUT_CANCEL_ACK                      = 0x17E4, ///< 6.2.3 20726
        SMSG_LOGOUT_COMPLETE                        = 0x16EF, ///< 6.2.3 20726
        SMSG_GOSSIP_POI                             = 0x174C, ///< 6.2.3 20726
        SMSG_ARCHAEOLOGY_SURVERY_CAST               = 0x1BC4, ///< 6.2.3 20726

        /// World Object management
        SMSG_UPDATE_OBJECT                          = 0x026B, ///< 6.2.3 20726
        SMSG_POWER_UPDATE                           = 0x1BDB, ///< 6.2.3 20726
        SMSG_ADJUST_SPLINE_DURATION                 = 0x0F57, ///< 6.2.3 20726

        /// Character list
        SMSG_ENUM_CHARACTERS_RESULT                 = 0x17D8, ///< 6.2.3 20726
        SMSG_CREATE_CHAR                            = 0x0B5B, ///< 6.2.3 20726
        SMSG_CHAR_DELETE                            = 0x13DF, ///< 6.2.3 20726
        SMSG_SET_PLAYER_DECLINED_NAMES_RESULT       = 0x1650, ///< 6.2.3 20726
        SMSG_CHAR_FACTION_CHANGE                    = 0x0EF8, ///< 6.2.3 20726
        SMSG_CHAR_RENAME                            = 0x0264, ///< 6.2.3 20726
        SMSG_CHAR_CUSTOMIZE_RESULT                  = 0x1B6B, ///< 6.2.3 20726
        SMSG_CHAR_CUSTOMIZE                         = 0x1263, ///< 6.2.3 20726
        SMSG_GENERATE_RANDOM_CHARACTER_NAME_RESULT  = 0x070E, ///< 6.2.3 20726

        /// Account data
        SMSG_ACCOUNT_DATA_TIMES                     = 0x1773, ///< 6.2.3 20726
        SMSG_UPDATE_ACCOUNT_DATA                    = 0x0B47, ///< 6.2.3 20726
        SMSG_TUTORIAL_FLAGS                         = 0x1B7B, ///< 6.2.3 20726

        /// Combat
        SMSG_ATTACKER_STATE_UPDATE                  = 0x0ACF, ///< 6.2.3 20726
        SMSG_ATTACK_START                           = 0x12CC, ///< 6.2.3 20726
        SMSG_ATTACK_STOP                            = 0x0F6B, ///< 6.2.3 20726
        SMSG_AI_REACTION                            = 0x135C, ///< 6.2.3 20726
        SMSG_ATTACK_SWING_ERROR                     = 0x1A78, ///< 6.2.3 20726

        /// Duel
        SMSG_DUEL_REQUESTED                         = 0x07FB, ///< 6.2.3 20726
        SMSG_DUEL_COUNTDOWN                         = 0x03E8, ///< 6.2.3 20726
        SMSG_DUEL_COMPLETE                          = 0x0B6F, ///< 6.2.3 20726
        SMSG_DUEL_INBOUNDS                          = 0x0AF4, ///< 6.2.3 20726
        SMSG_DUEL_OUT_OF_BOUNDS                     = 0x1254, ///< 6.2.3 20726
        SMSG_DUEL_WINNER                            = 0x0B78, ///< 6.2.3 20726
        SMSG_CAN_DUEL_RESULT                        = 0x16FB, ///< 6.2.3 20726

        /// Vendor
        SMSG_LIST_INVENTORY                         = 0x0E5B, ///< 6.2.3 20726
        SMSG_BUY_FAILED                             = 0x1258, ///< 6.2.3 20726
        SMSG_BUY_ITEM                               = 0x0357, ///< 6.2.3 20726
        SMSG_SELL_ITEM                              = 0x0EE0, ///< 6.2.3 20726

        /// Achievement
        SMSG_ALL_ACHIEVEMENT_DATA                   = 0x1B50, ///< 6.2.3 20726
        SMSG_ACHIEVEMENT_DELETED                    = 0x1B63, ///< 6.2.3 20726
        SMSG_ACHIEVEMENT_EARNED                     = 0x0BE4, ///< 6.2.3 20726
        SMSG_ACCOUNT_CRITERIA_UPDATE                = 0x1A7F, ///< 6.2.3 20726
        SMSG_ACCOUNT_CRITERIA_UPDATE_ALL            = 0x0A60, ///< 6.2.3 20726
        SMSG_CRITERIA_DELETED                       = 0x0E53, ///< 6.2.3 20726
        SMSG_CRITERIA_UPDATE                        = 0x0667, ///< 6.2.3 20726

        /// Friends
        SMSG_CONTACT_LIST                           = 0x1A6F, ///< 6.2.3 20726
        SMSG_FRIEND_STATUS                          = 0x17EB, ///< 6.2.3 20726

        /// Taxi
        SMSG_TAXI_NODE_STATUS                       = 0x12F3, ///< 6.2.3 20726
        SMSG_NEW_TAXI_PATH                          = 0x0BFF, ///< 6.2.3 20726
        SMSG_SHOW_TAXI_NODES                        = 0x066F, ///< 6.2.3 20726
        SMSG_ACTIVATE_TAXI_REPLY                    = 0x1A54, ///< 6.2.3 20726

        /// Loot
        SMSG_LOOT_LIST                              = 0x16E4, ///< 6.2.3 20726
        SMSG_LOOT_RESPONSE                          = 0x167B, ///< 6.2.3 20726
        SMSG_LOOT_MONEY_NOTIFY                      = 0x167F, ///< 6.2.3 20726
        SMSG_LOOT_RELEASE                           = 0x12E7, ///< 6.2.3 20726
        SMSG_LOOT_RELEASE_ALL                       = 0x0FC4, ///< 6.2.3 20726
        SMSG_LOOT_REMOVED                           = 0x03F4, ///< 6.2.3 20726
        SMSG_COIN_REMOVED                           = 0x02E4, ///< 6.2.3 20726
        SMSG_LOOT_ALL_PASSED                        = 0x076B, ///< 6.2.3 20726
        SMSG_LOOT_CONTENTS                          = 0x0000, ///< (unused)
        SMSG_LOOT_ITEM_LIST                         = 0x0000, ///< (unused)
        SMSG_MASTER_LOOT_CANDIDATE_LIST             = 0x1A5C, ///< 6.2.3 20726
        SMSG_LOOT_ROLL                              = 0x16C7, ///< 6.2.3 20726
        SMSG_LOOT_ROLLS_COMPLETE                    = 0x0000, ///< (unused)
        SMSG_LOOT_ROLL_WON                          = 0x06EC, ///< 6.2.3 20726
        SMSG_LOOT_START_ROLL                        = 0x02CB, ///< 6.2.3 20726
        SMSG_AE_LOOT_TARGETS                        = 0x0000, ///< (unused)
        SMSG_AE_LOOT_TARGETS_ACK                    = 0x0000, ///< (unused)
        SMSG_ITEM_BONUS_DEBUG                       = 0x17F3, ///< 6.2.3 20726

        /// Bank
        SMSG_SHOW_BANK                              = 0x0B48, ///< 6.2.3 20726

        /// Rune
        SMSG_RESYNC_RUNES                           = 0x13FC, ///< 6.2.3 20726
        SMSG_DEBUG_RUNE_REGEN                       = 0x0FD3, ///< 6.2.3 20726
        SMSG_ADD_RUNE_POWER                         = 0x0367, ///< 6.2.3 20726
        SMSG_CONVERT_RUNE                           = 0x0368, ///< 6.2.3 20726

        /// Group
        SMSG_PARTY_UPDATE                           = 0x027B, ///< 6.2.3 20726
        SMSG_PARTY_MEMBER_STATE_FULL                = 0x0EF3, ///< 6.2.3 20726
        SMSG_PARTY_MEMBER_STATE_PARTIAL             = 0x06EF, ///< 6.2.3 20726
        SMSG_PARTY_INVITE                           = 0x07E8, ///< 6.2.3 20726
        SMSG_GROUP_DECLINE                          = 0x1667, ///< 6.2.3 20726
        SMSG_PARTY_COMMAND_RESULT                   = 0x0744, ///< 6.2.3 20726
        SMSG_GROUP_UNINVITE                         = 0x176C, ///< 6.2.3 20726
        SMSG_GROUP_NEW_LEADER                       = 0x1AF3, ///< 6.2.3 20726
        SMSG_MINIMAP_PING                           = 0x1BFC, ///< 6.2.3 20726
        SMSG_SET_LOOT_METHOD_FAILED                 = 0x0F47, ///< 6.2.3 20726
        SMSG_RANDOM_ROLL                            = 0x07DB, ///< 6.2.3 20726
        SMSG_SEND_RAID_TARGET_UPDATE_SINGLE         = 0x0FCF, ///< 6.2.3 20726
        SMSG_SEND_RAID_TARGET_UPDATE_ALL            = 0x1BF4, ///< 6.2.3 20726
        SMSG_READY_CHECK_COMPLETED                  = 0x0BD0, ///< 6.2.3 20726
        SMSG_READY_CHECK_RESPONSE                   = 0x1BDF, ///< 6.2.3 20726
        SMSG_READY_CHECK_STARTED                    = 0x02C3, ///< 6.2.3 20726
        SMSG_ROLL_POLL_INFORM                       = 0x1663, ///< 6.2.3 20726
        SMSG_ROLE_CHANGED_INFORM                    = 0x16D7, ///< 6.2.3 20726
        SMSG_GROUP_DESTROYED                        = 0x0374, ///< 6.2.3 20726

        /// Battleground
        SMSG_BATTLEFIELD_STATUS_QUEUED                      = 0x0ADB, ///< 6.2.3 20726
        SMSG_BATTLEFIELD_STATUS_NONE                        = 0x165C, ///< 6.2.3 20726
        SMSG_BATTLEFIELD_STATUS_NEED_CONFIRMATION           = 0x0370, ///< 6.2.3 20726
        SMSG_BATTLEFIELD_STATUS_ACTIVE                      = 0x1AE4, ///< 6.2.3 20726
        SMSG_BATTLEFIELD_STATUS_FAILED                      = 0x0643, ///< 6.2.3 20726
        SMSG_BATTLEFIELD_STATUS_WAIT_FOR_GROUPS             = 0x175C, ///< 6.2.3 20726
        SMSG_BATTLEFIELD_PORT_DENIED                        = 0x0000, ///< (unused)
        SMSG_RATED_BATTLEFIELD_INFO                         = 0x1BD8, ///< 6.2.3 20726
        SMSG_BATTLEGROUND_PLAYER_JOINED                     = 0x0E50, ///< 6.2.3 20726
        SMSG_BATTLEGROUND_PLAYER_LEFT                       = 0x1377, ///< 6.2.3 20726
        SMSG_BATTLEGROUND_PLAYER_POSITIONS                  = 0x16D8, ///< 6.2.3 20726
        SMSG_BATTLEFIELD_MGR_ENTERING                       = 0x1AD0, ///< 6.2.3 20726
        SMSG_BATTLEFIELD_MGR_QUEUE_REQUEST_RESPONSE         = 0x0B4F, ///< 6.2.3 20726
        SMSG_BATTLEFIELD_MGR_EJECT_PENDING                  = 0x0000, ///< (unused)
        SMSG_BATTLEFIELD_MGR_EJECTED                        = 0x0747, ///< 6.2.3 20726
        SMSG_BATTLEFIELD_MGR_QUEUE_INVITE                   = 0x1E43, ///< 6.2.3 20726
        SMSG_BATTLEFIELD_MGR_STATE_CHANGED                  = 0x0000, ///< (unused)
        SMSG_BFMGR_ENTRY_INVITE                             = 0x02FB, ///< 6.2.3 20726
        SMSG_BATTLEFIELD_MGR_EXIT_REQUEST                   = 0x0000,
        SMSG_REQUEST_PVP_REWARDS_RESPONSE                   = 0x1AEB, ///< 6.2.3 20726
        SMSG_PVP_OPTIONS_ENABLED                            = 0x1BFB, ///< 6.2.3 20726
        SMSG_PVPLOG_DATA                                    = 0x0EEF, ///< 6.2.3 20726
        SMSG_ARENA_OPPONENT_SPECIALIZATIONS                 = 0x0F78, ///< 6.2.3 20726
        SMSG_DESTROY_ARENA_UNIT                             = 0x0000, ///< (unused)
        SMSG_BATTLEGROUND_POINTS                            = 0x0000, ///< (unused)
        SMSG_BATTLEGROUND_INIT                              = 0x0000, ///< (unused)
        SMSG_MAP_OBJECTIVES_INIT                            = 0x0000, ///< (unused)
        SMSG_CONQUEST_FORMULA_CONSTANTS                     = 0x026C, ///< 6.2.3 20726
        SMSG_PVP_SEASON                                     = 0x1BF0, ///< 6.2.3 20726
        SMSG_WARGAME_REQUEST_SUCCESSFULLY_SENT_TO_OPPENENT  = 0x0000, ///< (unused)
        SMSG_SPIRIT_HEALER_CONFIRM                          = 0x164B, ///< 6.2.3 20726
        SMSG_AREA_SPIRIT_HEALER_TIME                        = 0x024C, ///< 6.2.3 20726
        SMSG_BATTLEFIELD_LIST                               = 0x0FEF, ///< 6.2.3 20726
        SMSG_CHECK_WARGAME_ENTRY                            = 0x16F4, ///< 6.2.3 20726

        /// Pet
        SMSG_PET_NAME_QUERY_RESPONSE                        = 0x0E5F, ///< 6.2.3 20726
        SMSG_PET_NAME_INVALID                               = 0x03F0, ///< 6.2.3 20726
        SMSG_PET_DISMISS_SOUND                              = 0x07F3, ///< 6.2.3 20726
        SMSG_PET_GUIDS                                      = 0x1BF7, ///< 6.2.3 20726
        SMSG_PET_MODE                                       = 0x0000, ///< (unused)
        SMSG_PET_SLOT_UPDATED                               = 0x0000, ///< (unused)
        SMSG_PET_STABLE_LIST                                = 0x13D0, ///< 6.2.3 20726
        SMSG_PET_ACTION_FEEDBACK                            = 0x07F4, ///< 6.2.3 20726
        SMSG_PET_ACTION_SOUND                               = 0x0267, ///< 6.2.3 20726
        SMSG_PET_ADDED                                      = 0x0000, ///< (unused)
        SMSG_SET_PET_SPECIALIZATION                         = 0x1B7F, ///< 6.2.3 20726
        SMSG_STABLE_RESULT                                  = 0x03C3, ///< 6.2.3 20726

        /// Battle pet opcodes
        SMSG_BATTLE_PET_UPDATES                             = 0x02FF,   ///< 6.2.3 20726 (unsued)
        SMSG_BATTLE_PET_TRAP_LEVEL                          = 0x13FB,   ///< 6.2.3 20726
        SMSG_BATTLE_PET_JOURNAL_LOCK_ACQUIRED               = 0x1BCB,   ///< 6.2.3 20726
        SMSG_BATTLE_PET_JOURNAL_LOCK_DENIED                 = 0x13D7,   ///< 6.2.3 20726
        SMSG_BATTLE_PET_JOURNAL                             = 0x13E7,   ///< 6.2.3 20726
        SMSG_BATTLE_PET_DELETED                             = 0x17EC,   ///< 6.2.3 20726 (unsued)
        SMSG_BATTLE_PET_REVOKED                             = 0x0E47,   ///< 6.2.3 20726 (unsued)
        SMSG_BATTLE_PET_RESTORED                            = 0x12DF,   ///< 6.2.3 20726 (unsued)
        SMSG_BATTLE_PETS_HEALED                             = 0x1B5F,   ///< 6.2.3 20726 (unsued)
        SMSG_BATTLE_PET_LICENSE_CHANGED                     = 0x0247,   ///< 6.2.3 20726 (unsued)
        SMSG_BATTLE_PET_ERROR                               = 0x1A7C,   ///< 6.2.3 20726 (unsued)
        SMSG_BATTLE_PET_CAGE_DATE_ERROR                     = 0x1A74,   ///< 6.2.3 20726 (unsued)
        SMSG_QUERY_BATTLE_PET_NAME_RESPONSE                 = 0x0800,   ///< 6.2.3 20726

        /// Pet Battle opcodes
        SMSG_PET_BATTLE_SLOT_UPDATES                        = 0x0FCB,   ///< 6.2.3 20726
        SMSG_PET_BATTLE_REQUEST_FAILED                      = 0x12F0,   ///< 6.2.3 20726
        SMSG_PET_BATTLE_PVPCHALLENGE                        = 0x0A63,   ///< 6.2.3 20726
        SMSG_PET_BATTLE_FINALIZE_LOCATION                   = 0x165F,   ///< 6.2.3 20726
        SMSG_PET_BATTLE_INITIAL_UPDATE                      = 0x0B80,   ///< 6.2.3 20726
        SMSG_PET_BATTLE_FIRST_ROUND                         = 0x03E3,   ///< 6.2.3 20726
        SMSG_PET_BATTLE_ROUND_RESULT                        = 0x0EC8,   ///< 6.2.3 20726
        SMSG_PET_BATTLE_REPLACEMENTS_MADE                   = 0x1360,   ///< 6.2.3 20726
        SMSG_PET_BATTLE_FINAL_ROUND                         = 0x02CF,   ///< 6.2.3 20726
        SMSG_PET_BATTLE_FINISHED                            = 0x02F8,   ///< 6.2.3 20726
        SMSG_PET_BATTLE_CHAT_RESTRICTED                     = 0x1B48,   ///< 6.2.3 20726
        SMSG_PET_BATTLE_MAX_GAME_LENGTH_WARNING             = 0x1AC8,   ///< 6.2.3 20726
        SMSG_PET_BATTLE_QUEUE_PROPOSE_MATCH                 = 0x03D3,   ///< 6.2.3 20726
        SMSG_PET_BATTLE_QUEUE_STATUS                        = 0x1BD3,   ///< 6.2.3 20726
        SMSG_PET_BATTLE_DEBUG_QUEUE_DUMP_RESPONSE           = 0x0377,   ///< 6.2.3 20726

        /// Instances
        SMSG_INSTANCE_ENCOUNTER_ENGAGE_UNIT                     = 0x06D4, ///< 6.2.3 20726
        SMSG_INSTANCE_ENCOUNTER_DISENGAGE_UNIT                  = 0x12D4, ///< 6.2.3 20726
        SMSG_INSTANCE_ENCOUNTER_CHANGE_PRIORITY                 = 0x06E7, ///< 6.2.3 20726
        SMSG_INSTANCE_ENCOUNTER_TIMER_START                     = 0x0350, ///< 6.2.3 20726
        SMSG_INSTANCE_ENCOUNTER_OBJECTIVE_START                 = 0x0400, ///< 6.2.3 20726
        SMSG_INSTANCE_ENCOUNTER_OBJECTIVE_COMPLETE              = 0x06F8, ///< 6.2.3 20726
        SMSG_INSTANCE_ENCOUNTER_START                           = 0x0B53, ///< 6.2.3 20726
        SMSG_INSTANCE_ENCOUNTER_OBJECTIVE_UPDATE                = 0x0EEB, ///< 6.2.3 20726
        SMSG_INSTANCE_ENCOUNTER_END                             = 0x02C4, ///< 6.2.3 20726
        SMSG_INSTANCE_ENCOUNTER_IN_COMBAT_RESURRECTION          = 0x0253, ///< 6.2.3 20726
        SMSG_INSTANCE_ENCOUNTER_PHASE_SHIFT_CHANGED             = 0x1374, ///< 6.2.3 20726
        SMSG_INSTANCE_ENCOUNTER_GAIN_COMBAT_RESURRECTION_CHARGE = 0x0B54, ///< 6.2.3 20726
        SMSG_INSTANCE_RESET                                     = 0x07E3, ///< 6.2.3 20726
        SMSG_INSTANCE_RESET_FAILED                              = 0x17D4, ///< 6.2.3 20726
        SMSG_RAID_INSTANCE_INFO                                 = 0x0AE8, ///< 6.2.3 20726
        SMSG_RAID_GROUP_ONLY                                    = 0x1B6C, ///< 6.2.3 20726
        SMSG_RAID_MARKERS_CHANGED                               = 0x17E7, ///< 6.2.3 20726
        SMSG_INSTANCE_SAVE_CREATED                              = 0x0ED4, ///< 6.2.3 20726
        SMSG_INSTANCE_GROUP_SIZE_CHANGED                        = 0x0E77, ///< 6.2.3 20726
        SMSG_ENCOUNTER_END                                      = 0x1AE8, ///< 6.2.3 20726
        SMSG_ENCOUNTER_START                                    = 0x0F5B, ///< 6.2.3 20726
        SMSG_BOSS_KILL_CREDIT                                   = 0x1743, ///< 6.2.3 20726

        /// Scenarios
        SMSG_SCENARIO_POI                                       = 0x1BE3, ///< 6.2.3 20726
        SMSG_SCENARIO_PROGRESS_UPDATE                           = 0x0ED7, ///< 6.2.3 20726
        SMSG_SCENARIO_STATE                                     = 0x1364, ///< 6.2.3 20726

        /// Auction House
        SMSG_AUCTION_HELLO_RESPONSE                             = 0x0E57, ///< 6.2.3 20726
        SMSG_AUCTION_BIDDER_LIST_RESULT                         = 0x027C, ///< 6.2.3 20726
        SMSG_AUCTION_BIDDER_NOTIFICATION                        = 0x1ACB, ///< 6.2.3 20726
        SMSG_AUCTION_BUYOUT_NOTIFICATION                        = 0x0A5C, ///< 6.2.3 20726
        SMSG_AUCTION_COMMAND_RESULT                             = 0x12F7, ///< 6.2.3 20726
        SMSG_AUCTION_CLOSED_NOTIFICATION                        = 0x1BE0, ///< 6.2.3 20726
        SMSG_AUCTION_LIST_PENDING_SALES                         = 0x1BEF, ///< 6.2.3 20726
        SMSG_AUCTION_LIST_RESULT                                = 0x1243, ///< 6.2.3 20726
        SMSG_AUCTION_OWNER_LIST_RESULT                          = 0x0BCC, ///< 6.2.3 20726
        SMSG_AUCTION_OWNER_BID_NOTIFICATION                     = 0x02D7, ///< 6.2.3 20726

        /// Mail
        SMSG_SEND_MAIL_RESULT                                   = 0x06D0, ///< 6.2.3 20726
        SMSG_MAIL_LIST_RESULT                                   = 0x16EC, ///< 6.2.3 20726
        SMSG_RECEIVED_MAIL                                      = 0x0A73, ///< 6.2.3 20726
        SMSG_MAIL_QUERY_NEXT_TIME_RESULT                        = 0x136C, ///< 6.2.3 20726

        /// Trainers
        SMSG_TRAINER_LIST                                       = 0x12DC, ///< 6.2.3 20726
        SMSG_TRAINER_SERVICE                                    = 0x025B, ///< 6.2.3 20726

        /// Void Storage
        SMSG_VOID_ITEM_SWAP_RESPONSE                            = 0x0F64, ///< 6.2.3 20726
        SMSG_VOID_STORAGE_CONTENTS                              = 0x1ADC, ///< 6.2.3 20726
        SMSG_VOID_STORAGE_FAILED                                = 0x03CF, ///< 6.2.3 20726
        SMSG_VOID_STORAGE_TRANSFER_CHANGES                      = 0x1A6B, ///< 6.2.3 20726
        SMSG_VOID_TRANSFER_RESULT                               = 0x0E68, ///< 6.2.3 20726

        /// Petition
        SMSG_PETITION_ALREADY_SIGNED                            = 0x13D8, ///< 6.2.3 20726
        SMSG_PETITION_SIGN_RESULTS                              = 0x0FE3, ///< 6.2.3 20726
        SMSG_QUERY_PETITION_RESPONSE                            = 0x1668, ///< 6.2.3 20726
        SMSG_PETITION_SHOW_LIST                                 = 0x136B, ///< 6.2.3 20726
        SMSG_PETITION_SHOW_SIGNATURES                           = 0x0663, ///< 6.2.3 20726
        SMSG_TURN_IN_PETITION_RESULTS                           = 0x16C8, ///< 6.2.3 20726
        SMSG_PETITION_DECLINED                                  = 0x0000, ///< fake opcode

        /// Threat
        SMSG_THREAT_CLEAR                                       = 0x07C3, ///< 6.2.3 20726
        SMSG_THREAT_REMOVE                                      = 0x0BDF, ///< 6.2.3 20726
        SMSG_THREAT_UPDATE                                      = 0x035B, ///< 6.2.3 20726
        SMSG_HIGHEST_THREAT_UPDATE                              = 0x0F53, ///< 6.2.3 20726

        /// Tickets
        SMSG_GM_TICKET_SYSTEM_STATUS                            = 0x1680, ///< 6.2.3 20726
        SMSG_GM_TICKET_CASE_STATUS                              = 0x17C7, ///< 6.2.3 20726

        /// Calendar
        SMSG_CALENDAR_CLEAR_PENDING_ACTION                      = 0x02DC, ///< 6.2.3 20726
        SMSG_CALENDAR_COMMAND_RESULT                            = 0x0258, ///< 6.2.3 20726
        SMSG_CALENDAR_EVENT_INITIAL_INVITE                      = 0x0777, ///< 6.2.3 20726
        SMSG_CALENDAR_EVENT_INVITE                              = 0x02D0, ///< 6.2.3 20726
        SMSG_CALENDAR_EVENT_INVITE_ALERT                        = 0x16C4, ///< 6.2.3 20726
        SMSG_CALENDAR_EVENT_INVITE_NOTES                        = 0x0E70, ///< 6.2.3 20726
        SMSG_CALENDAR_EVENT_INVITE_NOTES_ALERT                  = 0x1748, ///< 6.2.3 20726
        SMSG_CALENDAR_EVENT_INVITE_REMOVED                      = 0x0AFB, ///< 6.2.3 20726
        SMSG_CALENDAR_EVENT_INVITE_REMOVED_ALERT                = 0x06E3, ///< 6.2.3 20726
        SMSG_CALENDAR_EVENT_INVITE_STATUS                       = 0x1B68, ///< 6.2.3 20726
        SMSG_CALENDAR_EVENT_INVITE_STATUS_ALERT                 = 0x0280, ///< 6.2.3 20726
        SMSG_CALENDAR_EVENT_MODERATOR_STATUS                    = 0x0244, ///< 6.2.3 20726
        SMSG_CALENDAR_EVENT_REMOVED_ALERT                       = 0x0BD7, ///< 6.2.3 20726
        SMSG_CALENDAR_EVENT_UPDATED_ALERT                       = 0x02D8, ///< 6.2.3 20726
        SMSG_CALENDAR_RAID_LOCKOUT_ADDED                        = 0x0AD0, ///< 6.2.3 20726
        SMSG_CALENDAR_RAID_LOCKOUT_REMOVED                      = 0x0AC4, ///< 6.2.3 20726
        SMSG_CALENDAR_RAID_LOCKOUT_UPDATED                      = 0x1AC3, ///< 6.2.3 20726
        SMSG_CALENDAR_SEND_CALENDAR                             = 0x0F00, ///< 6.2.3 20726
        SMSG_CALENDAR_SEND_EVENT                                = 0x12F8, ///< 6.2.3 20726
        SMSG_CALENDAR_SEND_NUM_PENDING                          = 0x03C8, ///< 6.2.3 20726

        /// Warden
        SMSG_WARDEN_DATA                                        = 0x0A68, ///< 6.2.3 20726

        /// Challenges
        SMSG_CHALLENGE_MODE_REQUEST_LEADERS_RESULT              = 0x0FE7, ///< 6.2.3 20726
        SMSG_CHALLENGE_MODE_MAP_STATS_UPDATE                    = 0x1B80, ///< 6.2.3 20726
        SMSG_CHALLENGE_MODE_NEW_PLAYER_RECORD                   = 0x07DC, ///< 6.2.3 20726
        SMSG_CHALLENGE_MODE_ALL_MAP_STATS                       = 0x1A58, ///< 6.2.3 20726
        SMSG_CHALLENGE_MODE_START                               = 0x0000, ///< This opcode is wrong and not existing
        SMSG_CHALLENGE_MODE_COMPLETE                            = 0x07CF, ///< 6.2.3 20726
        SMSG_CHALLENGE_MOD_REWARDS                              = 0x0000, ///< (NYI)

        /// Battlepay
        SMSG_BATTLE_PAY_GET_PRODUCT_LIST_RESPONSE               = 0x1674, ///< 6.2.3 20726
        SMSG_BATTLE_PAY_GET_PURCHASE_LIST_RESPONSE              = 0x0A6B, ///< 6.2.3 20726
        SMSG_BATTLE_PAY_GET_DISTRIBUTION_LIST_RESPONSE          = 0x17F0, ///< 6.2.3 20726
        SMSG_BATTLE_PAY_START_PURCHASE_RESPONSE                 = 0x0BC3, ///< 6.2.3 20726
        SMSG_BATTLE_PAY_PURCHASE_UPDATE                         = 0x0A5B, ///< 6.2.3 20726
        SMSG_BATTLE_PAY_CONFIRM_PURCHASE                        = 0x16DC, ///< 6.2.3 20726

        /// Black Market
        SMSG_BLACK_MARKET_OPEN_RESULT                           = 0x1760, ///< 6.2.3 20726
        SMSG_BLACK_MARKET_OUTBID                                = 0x164F, ///< 6.2.3 20726
        SMSG_BLACK_MARKET_REQUEST_ITEMS_RESULT                  = 0x0B68, ///< 6.2.3 20726
        SMSG_BLACK_MARKET_BID_ON_ITEM_RESULT                    = 0x07E0, ///< 6.2.3 20726
        SMSG_BLACK_MARKET_WON                                   = 0x1780, ///< 6.2.3 20726

        /// Garrison
        SMSG_OPEN_SHIPMENT_NPCFROM_GOSSIP                       = 0x1657, ///< 6.2.3 20726
        SMSG_GET_SHIPMENT_INFO_RESPONSE                         = 0x0BF7, ///< 6.2.3 20726
        SMSG_GARRISON_LANDING_PAGE_SHIPMENT_INFO                = 0x1AF7, ///< 6.2.3 20726
        SMSG_CREATE_SHIPMENT_RESPONSE                           = 0x0EFF, ///< 6.2.3 20726

        /// Twitter
        SMSG_REQUEST_TWITTER_STATUS_RESPONSE                    = 0x0000,
        SMSG_OAUTH_SAVED_DATA                                   = 0x0060, ///< 6.2.3 20726

        /// Toys
        SMSG_ACCOUNT_TOYS_UPDATE                                = 0x1B44, ///< 6.2.3 20726

        /// AreaTriggers
        SMSG_AREA_TRIGGER_RE_SHAPE                              = 0x127B, ///< 6.2.3 20726
        SMSG_AREA_TRIGGER_RE_PATH                               = 0x065C, ///< 6.2.3 20726
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Guild
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamGuild
        /// Petition
        SMSG_PETITION_RENAME                        = 0x141F, ///< 6.2.3 20726

        /// Guild Finder
        SMSG_LFGUILD_BROWSE                         = 0x154C, ///< 6.2.3 20726
        SMSG_LFGUILD_APPLICATIONS                   = 0x1460, ///< 6.2.3 20726
        SMSG_LFGUILD_APPLICATIONS_LIST_CHANGED      = 0x1503, ///< 6.2.3 20726
        SMSG_LFGUILD_COMMAND_RESULT                 = 0x0000, ///< (unused)
        SMSG_LFGUILD_RECRUITS                       = 0x1453, ///< 6.2.3 20726
        SMSG_LFGUILD_POST                           = 0x1450, ///< 6.2.3 20726
        SMSG_LFGUILD_APPLICANT_LIST_CHANGED         = 0x1587, ///< 6.2.3 20726

        /// Bank
        SMSG_GUILD_RANKS                            = 0x150F, ///< 6.2.3 20726
        SMSG_GUILD_BANK_LOG_QUERY_RESULT            = 0x1413, ///< 6.2.3 20726
        SMSG_GUILD_BANK_REMAINING_WITHDRAW_MONEY    = 0x1483, ///< 6.2.3 20726
        SMSG_GUILD_BANK_QUERY_TEXT_RESULT           = 0x155C, ///< 6.2.3 20726
        SMSG_GUILD_BANK_QUERY_RESULTS               = 0x140B, ///< 6.2.3 20726
        SMSG_GUILD_PERMISSIONS_QUERY_RESULTS        = 0x1544, ///< 6.2.3 20726

        /// Achievement
        SMSG_GUILD_ACHIEVEMENT_DELETED              = 0x1448, ///< 6.2.3 20726
        SMSG_GUILD_ACHIEVEMENT_MEMBERS              = 0x0000, ///< (unused)
        SMSG_GUILD_ACHIEVEMENT_EARNED               = 0x1543, ///< 6.2.3 20726
        SMSG_GUILD_CRITERIA_UPDATE                  = 0x0000, ///< (unused)
        SMSG_GUILD_CRITERIA_DELETED                 = 0x1508, ///< 6.2.3 20726
        SMSG_ALL_GUILD_ACHIEVEMENTS                 = 0x154F, ///< 6.2.3 20726

        /// Info
        SMSG_GUILD_ROSTER                           = 0x1408, ///< 6.2.3 20726
        SMSG_GUILD_ROSTER_UPDATE                    = 0x0000, ///< (unused)
        SMSG_QUERY_GUILD_INFO_RESPONSE              = 0x1507, ///< 6.2.3 20726
        SMSG_GUILD_MEMBER_UPDATE_NOTE               = 0x0000, ///< (unused)
        SMSG_GUILD_REWARDS_LIST                     = 0x1410, ///< 6.2.3 20726
        SMSG_GUILD_SEND_RANK_CHANGE                 = 0x149F, ///< 6.2.3 20726
        SMSG_GUILD_CHALLENGE_UPDATED                = 0x140F, ///< 6.2.3 20726
        SMSG_GUILD_MEMBER_RECIPES                   = 0x0000, ///< (unused)
        SMSG_GUILD_INVITE_EXPIRED                   = 0x0000, ///< (unused)
        SMSG_GUILD_COMMAND_RESULT                   = 0x150B, ///< 6.2.3 20726
        SMSG_GUILD_CHALLENGE_COMPLETED              = 0x1497, ///< 6.2.3 20726
        SMSG_GUILD_REPUTATION_REACTION_CHANGED      = 0x141B, ///< 6.2.3 20726
        SMSG_GUILD_KNOWN_RECIPES                    = 0x1407, ///< 6.2.3 20726
        SMSG_GUILD_MEMBER_DAILY_RESET               = 0x150C, ///< 6.2.3 20726
        SMSG_GUILD_FLAGGED_FOR_RENAME               = 0x0000, ///< (unused)
        SMSG_GUILD_NAME_CHANGED                     = 0x0000, ///< (unused)
        SMSG_GUILD_RESET                            = 0x0000, ///< (unused)
        SMSG_GUILD_CHANGE_NAME_RESULT               = 0x0000, ///< (unused)
        SMSG_GUILD_INVITE                           = 0x155F, ///< 6.2.3 20726
        SMSG_GUILD_INVITE_DECLINED                  = 0x0000, ///< (unused)
        SMSG_GUILD_PARTY_STATE                      = 0x151C, ///< 6.2.3 20726
        SMSG_GUILD_MEMBERS_WITH_RECIPE              = 0x0000, ///< (unused)
        SMSG_GUILD_MOVE_STARTING                    = 0x0000, ///< (unused)
        SMSG_GUILD_MOVED                            = 0x0000, ///< (unused)
        SMSG_GUILD_NEWS                             = 0x1557, ///< 6.2.3 20726
        SMSG_GUILD_NEWS_DELETED                     = 0x0000, ///< (unused)
        SMSG_PLAYER_TABAR_VENDOR_SHOW               = 0x16C3, ///< 6.2.3 20726
        SMSG_PLAYER_SAVE_GUILD_EMBLEM               = 0x14C8, ///< 6.2.3 20726

        /// Event system
        SMSG_GUILD_EVENT_NEW_LEADER                 = 0x1417, ///< 6.2.3 20726
        SMSG_GUILD_EVENT_PRESENCE_CHANGE            = 0x151F, ///< 6.2.3 20726
        SMSG_GUILD_EVENT_PLAYER_JOINED              = 0x1514, ///< 6.2.3 20726
        SMSG_GUILD_EVENT_PLAYER_LEFT                = 0x14D3, ///< 6.2.3 20726
        SMSG_GUILD_EVENT_RANKS_UPDATED              = 0x1560, ///< 6.2.3 20726
        SMSG_GUILD_EVENT_RANK_CHANGED               = 0x14C4, ///< 6.2.3 20726
        SMSG_GUILD_EVENT_LOG_QUERY_RESULTS          = 0x1457, ///< 6.2.3 20726
        SMSG_GUILD_EVENT_MOTD                       = 0x14CF, ///< 6.2.3 20726
        SMSG_GUILD_EVENT_DISBANDED                  = 0x1490, ///< 6.2.3 20726
        SMSG_GUILD_EVENT_TAB_ADDED                  = 0x1513, ///< 6.2.3 20726
        SMSG_GUILD_EVENT_TAB_MODIFIED               = 0x149C, ///< 6.2.3 20726
        SMSG_GUILD_EVENT_TAB_DELETED                = 0x14E0, ///< 6.2.3 20726
        SMSG_GUILD_EVENT_TAB_TEXT_CHANGED           = 0x1558, ///< 6.2.3 20726
        SMSG_GUILD_EVENT_BANK_CONTENTS_CHANGED      = 0x1554, ///< 6.2.3 20726
        SMSG_GUILD_EVENT_BANK_MONEY_CHANGED         = 0x1418, ///< 6.2.3 20726
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Chat
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamChat
        /// Voice chat
        SMSG_VOICE_SESSION_ROSTER_UPDATE            = 0x0000, ///< (unused)
        SMSG_VOICE_SESSION_LEAVE                    = 0x0000, ///< (unused)
        SMSG_VOICE_PARENTAL_CONTROLS                = 0x0000, ///< (unused)
        SMSG_VOICE_CHAT_STATUS                      = 0x0000, ///< (unused)
        SMSG_AVAILABLE_VOICE_CHANNEL                = 0x0000, ///< (unused)

        /// Misc
        SMSG_SERVER_FIRST_ACHIEVEMENT               = 0x030E, ///< 6.2.3 20726
        SMSG_RAID_INSTANCE_MESSAGE                  = 0x0306, ///< 6.2.3 20726
        SMSG_MOTD                                   = 0x0206, ///< 6.2.3 20726
        SMSG_EXPECTED_SPAM_RECORDS                  = 0x0000, ///< (unused)
        SMSG_DEFENSE_MESSAGE                        = 0x0000, ///< (unused)
        SMSG_ZONE_UNDER_ATTACK                      = 0x0A06, ///< 6.2.3 20726
        SMSG_WHO                                    = 0x0A09, ///< 6.2.3 20726

        /// Chat
        SMSG_CHAT_SERVER_MESSAGE                    = 0x0A01, ///< 6.2.3 20726
        SMSG_CHAT_RESTRICTED                        = 0x0B01, ///< 6.2.3 20726
        SMSG_CHAT_RECONNECT                         = 0x0000, ///< (unused)
        SMSG_CHAT_PLAYER_NOTFOUND                   = 0x0A0D, ///< 6.2.3 20726
        SMSG_CHAT_PLAYER_AMBIGUOUS                  = 0x0A85, ///< 6.2.3 20726
        SMSG_CHAT_NOT_IN_PARTY                      = 0x0000, ///< (unused)
        SMSG_CHAT_IS_DOWN                           = 0x0000, ///< (unused)
        SMSG_CHAT_IGNORED_ACCOUNT_MUTED             = 0x0000, ///< (unused)
        SMSG_CHAT_DOWN                              = 0x0000, ///< (unused)
        SMSG_CHAT_AUTO_RESPONDED                    = 0x0000, ///< (unused)
        SMSG_CHAT                                   = 0x068D, ///< 6.2.3 20726

        /// Channel
        SMSG_CHANNEL_NOTIFY_LEFT                    = 0x0685, ///< 6.2.3 20726
        SMSG_CHANNEL_NOTIFY_JOINED                  = 0x020D, ///< 6.2.3 20726
        SMSG_CHANNEL_NOTIFY                         = 0x038E, ///< 6.2.3 20726
        SMSG_CHANNEL_LIST                           = 0x0E05, ///< 6.2.3 20726
        SMSG_USERLIST_ADD                           = 0x0B8D, ///< 6.2.3 20726
        SMSG_USERLIST_REMOVE                        = 0x0A8A, ///< 6.2.3 20726
        SMSG_USERLIST_UPDATE                        = 0x0289, ///< 6.2.3 20726
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Move
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamMove
        /// Move speeds
        SMSG_MOVE_SET_RUN_SPEED                     = 0x04FD, ///< 6.2.3 20726
        SMSG_MOVE_SET_RUN_BACK_SPEED                = 0x0875, ///< 6.2.3 20726
        SMSG_MOVE_SET_SWIM_SPEED                    = 0x0961, ///< 6.2.3 20726
        SMSG_MOVE_SET_SWIM_BACK_SPEED               = 0x0D72, ///< 6.2.3 20726
        SMSG_MOVE_SET_FLIGHT_SPEED                  = 0x08BD, ///< 6.2.3 20726
        SMSG_MOVE_SET_FLIGHT_BACK_SPEED             = 0x0D0D, ///< 6.2.3 20726
        SMSG_MOVE_SET_WALK_SPEED                    = 0x0872, ///< 6.2.3 20726
        SMSG_MOVE_SET_TURN_RATE                     = 0x0DB6, ///< 6.2.3 20726
        SMSG_MOVE_SET_PITCH_RATE                    = 0x042D, ///< 6.2.3 20726
        SMSG_MOVE_KNOCK_BACK                        = 0x0026, ///< 6.2.3 20726
        SMSG_MOVE_UPDATE_RUN_SPEED                  = 0x0CEA, ///< 6.2.3 20726
        SMSG_MOVE_UPDATE_RUN_BACK_SPEED             = 0x100A, ///< 6.2.3 20726
        SMSG_MOVE_UPDATE_SWIM_SPEED                 = 0x0D75, ///< 6.2.3 20726
        SMSG_MOVE_UPDATE_SWIM_BACK_SPEED            = 0x0902, ///< 6.2.3 20726
        SMSG_MOVE_UPDATE_FLIGHT_SPEED               = 0x0CA9, ///< 6.2.3 20726
        SMSG_MOVE_UPDATE_FLIGHT_BACK_SPEED          = 0x097D, ///< 6.2.3 20726
        SMSG_MOVE_UPDATE_WALK_SPEED                 = 0x0DA5, ///< 6.2.3 20726
        SMSG_MOVE_UPDATE_TURN_RATE                  = 0x05A2, ///< 6.2.3 20726
        SMSG_MOVE_UPDATE_PITCH_RATE                 = 0x04FE, ///< 6.2.3 20726
        SMSG_MOVE_UPDATE_KNOCK_BACK                 = 0x0CE2, ///< 6.2.3 20726

        /// Player
        SMSG_MOVE_UPDATE                                    = 0x0439, ///< 6.2.3 20726
        SMSG_MOVE_TELEPORT                                  = 0x0D2D, ///< 6.2.3 20726
        SMSG_MOVE_UPDATE_TELEPORT                           = 0x0D09, ///< 6.2.3 20726
        SMSG_MOVE_ROOT                                      = 0x0471, ///< 6.2.3 20726
        SMSG_MOVE_UNROOT                                    = 0x0876, ///< 6.2.3 20726
        SMSG_MOVE_WATER_WALK                                = 0x00F1, ///< 6.2.3 20726
        SMSG_MOVE_LAND_WALK                                 = 0x09A1, ///< 6.2.3 20726
        SMSG_MOVE_FEATHER_FALL                              = 0x0425, ///< 6.2.3 20726
        SMSG_MOVE_NORMAL_FALL                               = 0x00F6, ///< 6.2.3 20726
        SMSG_MOVE_SET_ACTIVE_MOVER                          = 0x0175, ///< 6.2.3 20726
        SMSG_MOVE_SET_CAN_FLY                               = 0x05E9, ///< 6.2.3 20726
        SMSG_MOVE_UNSET_CAN_FLY                             = 0x1006, ///< 6.2.3 20726
        SMSG_MOVE_SET_HOVER                                 = 0x04EA, ///< 6.2.3 20726
        SMSG_MOVE_UNSET_HOVER                               = 0x0535, ///< 6.2.3 20726
        SMSG_MOVE_SET_CAN_TURN_WHILE_FALLING                = 0x00FA, ///< 6.2.3 20726
        SMSG_MOVE_UNSET_CAN_TURN_WHILE_FALLING              = 0x0DF9, ///< 6.2.3 20726
        SMSG_MOVE_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY   = 0x0906, ///< 6.2.3 20726
        SMSG_MOVE_UNSET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY = 0x0D3A, ///< 6.2.3 20726
        SMSG_MOVE_SET_VEHICLE_REC_ID                        = 0x08E6, ///< 6.2.3 20726
        SMSG_MOVE_SET_COLLISION_HEIGHT                      = 0x086E, ///< 6.2.3 20726
        SMSG_APPLY_MOVEMENT_FORCE                           = 0x0C62, ///< 6.2.3 20726
        SMSG_UNAPPLY_MOVEMENT_FORCE                         = 0x01E2, ///< 6.2.3 20726
        SMSG_MOVE_SKIP_TIME                                 = 0x0000, ///< (unused)
        SMSG_MOVE_ENABLE_GRAVITY                            = 0x0000, ///< (unused)
        SMSG_MOVE_DISABLE_GRAVITY                           = 0x0000, ///< (unused)
        SMSG_MOVE_ENABLE_COLLISION                          = 0x0000, ///< (unused)
        SMSG_MOVE_DISABLE_COLLISION                         = 0x0000, ///< (unused)

        /// Creature
        SMSG_MONSTER_MOVE                           = 0x043A, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_SET_RUN_SPEED              = 0x0DE1, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_SET_RUN_BACK_SPEED         = 0x053A, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_SET_SWIM_SPEED             = 0x05F2, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_SET_SWIM_BACK_SPEED        = 0x0031, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_SET_FLIGHT_SPEED           = 0x0D01, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_SET_FLIGHT_BACK_SPEED      = 0x05F1, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_SET_WALK_SPEED             = 0x0401, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_SET_TURN_RATE              = 0x002A, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_SET_PITCH_RATE             = 0x0D79, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_ROOT                       = 0x05E2, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_UNROOT                     = 0x0032, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_GRAVITY_DISABLE            = 0x0465, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_GRAVITY_ENABLE             = 0x0171, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_COLLISION_DISABLE          = 0x0000, ///< (unused)
        SMSG_SPLINE_MOVE_COLLISION_ENABLE           = 0x0000, ///< (unused)
        SMSG_SPLINE_MOVE_SET_FEATHER_FALL           = 0x08A6, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_SET_NORMAL_FALL            = 0x0C79, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_SET_HOVER                  = 0x00ED, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_UNSET_HOVER                = 0x0D4D, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_SET_WATER_WALK             = 0x0865, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_SET_LAND_WALK              = 0x04A6, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_START_SWIM                 = 0x0000, ///< (unused)
        SMSG_SPLINE_MOVE_STOP_SWIM                  = 0x0000, ///< (unused)
        SMSG_SPLINE_MOVE_SET_RUN_MODE               = 0x040E, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_SET_WALK_MODE              = 0x09AE, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_SET_FLYING                 = 0x0475, ///< 6.2.3 20726
        SMSG_SPLINE_MOVE_UNSET_FLYING               = 0x057E, ///< 6.2.3 20726
        SMSG_FLIGHT_SPLINE_SYNC                     = 0x09E1, ///< 6.2.3 20726
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Spell
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamSpell
        /// Aura
        SMSG_AURA_POINTS_DEPLETED                   = 0x0000, ///< (unused)
        SMSG_AURA_UPDATE                            = 0x10AF, ///< 6.2.3 20726

        /// Misc
        SMSG_CHEAT_IGNORE_DIMISHING_RETURNS         = 0x0000, ///< (unused)
        SMSG_DISPEL_FAILED                          = 0x0073, ///< 6.2.3 20726
        SMSG_INCREASE_CAST_TIME_FOR_SPELL           = 0x0000, ///< (unused)
        SMSG_MIRROR_IMAGE_COMPONENTED_DATA          = 0x01BF, ///< 6.2.3 20726
        SMSG_MIRROR_IMAGE_CREATURE_DATA             = 0x0000, ///< (unused)
        SMSG_PET_CLEAR_SPELLS                       = 0x0000, ///< (unused)
        SMSG_REFRESH_SPELL_HISTORY                  = 0x0000, ///< (unused)
        SMSG_RESUME_CAST_BAR                        = 0x0000, ///< (unused)
        SMSG_CAST_FAILED                            = 0x1138, ///< 6.2.3 20726
        SMSG_SPELL_FAILURE                          = 0x0424, ///< 6.2.3 20726
        SMSG_SPELL_FAILED_OTHER                     = 0x11EC, ///< 6.2.3 20726
        SMSG_PET_CAST_FAILED                        = 0x11A7, ///< 6.2.3 20726
        SMSG_ITEM_COOLDOWN                          = 0x0E5C, ///< 6.2.3 20726
        SMSG_MODIFY_COOLDOWN                        = 0x13C7, ///< 6.2.3 20726
        SMSG_COOLDOWN_CHEAT                         = 0x0000, ///< (unused)
        SMSG_PET_TAME_FAILURE                       = 0x16E7, ///< 6.2.3 20726
        SMSG_FEIGN_DEATH_RESISTED                   = 0x174B, ///< 6.2.3 20726
        SMSG_NOTIFY_DEST_LOC_SPELL_CAST             = 0x0000, ///< (unused)
        SMSG_ON_CANCEL_EXPECTED_RIDE_VEHICLE_AURA   = 0x1B54, ///< 6.2.3 20726
        SMSG_SET_VEHICLE_REC_ID                     = 0x0B4B, ///< 6.2.3 20726
        SMSG_COOLDOWN_EVENT                         = 0x0BDB, ///< 6.2.3 20726
        SMSG_DISMOUNT                               = 0x0AE0, ///< 6.2.3 20726
        SMSG_DISMOUNTRESULT                         = 0x0000, ///< (unused)
        SMSG_MOUNT_RESULT                           = 0x16FC, ///< 6.2.3 20726

        /// Spell Book / Bar
        SMSG_UPDATE_WEEKLY_SPELL_USAGE              = 0x0000, ///< (unused)
        SMSG_WEEKLY_SPELL_USAGE                     = 0x0000, ///< (unused)
        SMSG_SEND_KNOWN_SPELLS                      = 0x00EF, ///< 6.2.3 20726
        SMSG_SEND_SPELL_CHARGES                     = 0x05FC, ///< 6.2.3 20726
        SMSG_SEND_SPELL_HISTORY                     = 0x01F0, ///< 6.2.3 20726
        SMSG_SEND_UNLEARN_SPELLS                    = 0x0000, ///< (unused)
        SMSG_CLEAR_ALL_SPELL_CHARGES                = 0x10B3, ///< 6.2.3 20726
        SMSG_CLEAR_COOLDOWN                         = 0x0AC3, ///< 6.2.3 20726
        SMSG_CLEAR_COOLDOWNS                        = 0x0580, ///< 6.2.3 20726
        SMSG_CATEGORY_COOLDOWN                      = 0x00FB, ///< 6.2.3 20726
        SMSG_CLEAR_SPELL_CHARGES                    = 0x10F3, ///< 6.2.3 20726
        SMSG_SET_FLAT_SPELL_MODIFIER                = 0x00C0, ///< 6.2.3 20726
        SMSG_SET_PCT_SPELL_MODIFIER                 = 0x11AB, ///< 6.2.3 20726
        SMSG_SET_SPELL_CHARGES                      = 0x1027, ///< 6.2.3 20726
        SMSG_LEARNED_SPELL                          = 0x05A7, ///< 6.2.3 20726
        SMSG_UNLEARNED_SPELLS                       = 0x0470, ///< 6.2.3 20726

        /// Casting
        SMSG_SPELL_CHANNEL_START                    = 0x1428, ///< 6.2.3 20726
        SMSG_SPELL_COOLDOWN                         = 0x0124, ///< 6.2.3 20726
        SMSG_SPELL_CHANNEL_UPDATE                   = 0x04F7, ///< 6.2.3 20726
        SMSG_SPELL_DAMAGE_SHIELD                    = 0x047B, ///< 6.2.3 20726
        SMSG_SPELL_DELAYED                          = 0x116F, ///< 6.2.3 20726
        SMSG_SPELL_GO                               = 0x00BF, ///< 6.2.3 20726
        SMSG_SPELL_MULTISTRIKE_EFFECT               = 0x0000, ///< (unused)
        SMSG_SPELL_OR_DAMAGE_IMMUNE                 = 0x04C0, ///< 6.2.3 20726
        SMSG_SPELL_START                            = 0x1077, ///< 6.2.3 20726
        SMSG_SPELL_UPDATE_CHAIN_TARGETS             = 0x0000, ///< (unused)

        /// Logging
        SMSG_SPELL_DISPELL_LOG                      = 0x116B, ///< 6.2.3 20726
        SMSG_SPELL_ENERGIZE_LOG                     = 0x0100, ///< 6.2.3 20726
        SMSG_SPELL_EXECUTE_LOG                      = 0x11FC, ///< 6.2.3 20726
        SMSG_SPELL_NON_MELEE_DAMAGE_LOG             = 0x10F0, ///< 6.2.3 20726
        SMSG_SPELL_HEAL_LOG                         = 0x10B7, ///< 6.2.3 20726
        SMSG_SPELL_INSTAKILL_LOG                    = 0x0064, ///< 6.2.3 20726
        SMSG_SPELL_INTERRUPT_LOG                    = 0x01C0, ///< 6.2.3 20726
        SMSG_SPELL_MISS_LOG                         = 0x057B, ///< 6.2.3 20726
        SMSG_ENVIRONMENTAL_DAMAGE_LOG               = 0x10EC, ///< 6.2.3 20726
        SMSG_SPELL_PERIODIC_AURA_LOG                = 0x10FC, ///< 6.2.3 20726
        SMSG_SPELL_PROC_SCRIPT_LOG                  = 0x0000, ///< Debug opcode (disabled)
        SMSG_SPELL_PROCS_PER_MINUTE_LOG             = 0x0000, ///< Debug opcode (disabled)
        SMSG_SPELL_CHANCE_PROC_LOG                  = 0x0000, ///< Debug opcode (disabled)
        SMSG_RESIST_LOG                             = 0x0000, ///< Debug opcode (disabled)
        SMSG_AURA_CAST_LOG                          = 0x0000, ///< Debug opcode (disabled)

        /// Pet
        SMSG_PET_SPELLS_MESSAGE                     = 0x10E4, ///< 6.2.3 20726
        SMSG_PET_UNLEARNED_SPELLS                   = 0x053F, ///< 6.2.3 20726
        SMSG_PET_LEARNED_SPELLS                     = 0x046F, ///< 6.2.3 20726

        /// Visuals
        SMSG_PLAY_ORPHAN_SPELL_VISUAL               = 0x10A3, ///< 6.2.3 20726
        SMSG_CANCEL_ORPHAN_SPELL_VISUAL             = 0x11F8, ///< 6.2.3 20726
        SMSG_PLAY_SPELL_VISUAL                      = 0x11B0, ///< 6.2.3 20726
        SMSG_CANCEL_SPELL_VISUAL                    = 0x1440, ///< 6.2.3 20726
        SMSG_PLAY_SPELL_VISUAL_KIT                  = 0x00F3, ///< 6.2.3 20726
        SMSG_CANCEL_SPELL_VISUAL_KIT                = 0x0167, ///< 6.2.3 20726
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Quest
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamQuest
        /// Quest log
        SMSG_QUERY_QUEST_INFO_RESPONSE              = 0x15AD, ///< 6.2.3 20726
        SMSG_QUEST_UPDATE_FAILED_TIMER              = 0x1989, ///< 6.2.3 20726
        SMSG_QUEST_UPDATE_FAILED                    = 0x023E, ///< 6.2.3 20726
        SMSG_QUEST_UPDATE_COMPLETE                  = 0x1981, ///< 6.2.3 20726
        SMSG_QUEST_UPDATE_ADD_PVP_CREDIT            = 0x1501, ///< 6.2.3 20726
        SMSG_QUEST_UPDATE_ADD_CREDIT_SIMPLE         = 0x0616, ///< 6.2.3 20726
        SMSG_QUEST_UPDATE_ADD_CREDIT                = 0x1D8A, ///< 6.2.3 20726
        SMSG_QUEST_PUSH_RESULT                      = 0x1D21, ///< 6.2.3 20726
        SMSG_QUEST_POIQUERY_RESPONSE                = 0x1D82, ///< 6.2.3 20726
        SMSG_QUEST_LOG_FULL                         = 0x031D, ///< 6.2.3 20726
        SMSG_IS_QUEST_COMPLETE_RESPONSE             = 0x1D26, ///< 6.2.3 20726
        SMSG_QUEST_FORCE_REMOVED                    = 0x1525, ///< 6.2.3 20726
        SMSG_QUEST_CONFIRM_ACCEPT                   = 0x0756, ///< 6.2.3 20726
        SMSG_QUEST_COMPLETION_NPCRESPONSE           = 0x0442, ///< 6.2.3 20726
        SMSG_DAILY_QUESTS_RESET                     = 0x112D, ///< 6.2.3 20726

        /// Quest giver
        SMSG_QUEST_GIVER_STATUS_MULTIPLE            = 0x0735, ///< 6.2.3 20726
        SMSG_QUEST_GIVER_STATUS                     = 0x1D02, ///< 6.2.3 20726
        SMSG_QUEST_GIVER_REQUEST_ITEMS              = 0x1DAA, ///< 6.2.3 20726
        SMSG_QUEST_GIVER_QUEST_LIST_MESSAGE         = 0x0259, ///< 6.2.3 20726
        SMSG_QUEST_GIVER_QUEST_FAILED               = 0x152D, ///< 6.2.3 20726
        SMSG_QUEST_GIVER_QUEST_DETAILS              = 0x1921, ///< 6.2.3 20726
        SMSG_QUEST_GIVER_QUEST_COMPLETE             = 0x1D05, ///< 6.2.3 20726
        SMSG_QUEST_GIVER_OFFER_REWARD_MESSAGE       = 0x1125, ///< 6.2.3 20726
        SMSG_QUEST_GIVER_INVALID_QUEST              = 0x11A1, ///< 6.2.3 20726

        /// Gossip
        SMSG_GOSSIP_MESSAGE                         = 0x0351, ///< 6.2.3 20726
        SMSG_GOSSIP_COMPLETE                        = 0x15A2, ///< 6.2.3 20726
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Lfg
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamLFG
        /// LFG
        SMSG_LFG_BOOT_PLAYER                              = 0x100F, ///< 6.2.3 20726
        SMSG_LFG_DISABLED                                 = 0x108B, ///< 6.2.3 20726
        SMSG_LFG_JOIN_RESULT                              = 0x114F, ///< 6.2.3 20726
        SMSG_LFG_OFFER_CONTINUE                           = 0x1060, ///< 6.2.3 20726
        SMSG_LFG_OPEN_FROM_GOSSIP                         = 0x0000, ///< (unused)
        SMSG_LFG_PARTY_INFO                               = 0x1097, ///< 6.2.3 20726
        SMSG_LFG_PLAYER_INFO                              = 0x1187, ///< 6.2.3 20726
        SMSG_LFG_PLAYER_REWARD                            = 0x1103, ///< 6.2.3 20726
        SMSG_LFG_PROPOSAL_UPDATE                          = 0x110F, ///< 6.2.3 20726
        SMSG_LFG_QUEUE_STATUS                             = 0x1117, ///< 6.2.3 20726
        SMSG_LFG_ROLE_CHECK_UPDATE                        = 0x1058, ///< 6.2.3 20726
        SMSG_LFG_ROLE_CHOSEN                              = 0x111B, ///< 6.2.3 20726
        SMSG_LFG_SEARCH_RESULT                            = 0x0000, ///< (unused)
        SMSG_LFG_SLOT_INVALID                             = 0x0000, ///< (unused)
        SMSG_LFG_TELEPORT_DENIED                          = 0x1053, ///< 6.2.3 20726
        SMSG_LFG_UPDATE_LIST                              = 0x0000, ///<
        SMSG_LFG_UPDATE_PARTY                             = 0x0000,
        SMSG_LFG_UPDATE_PLAYER                            = 0x0000,
        SMSG_LFG_UPDATE_SEARCH                            = 0x0000,
        SMSG_LFG_UPDATE_STATUS                            = 0x1010, ///< 6.2.3 20726
        SMSG_LFG_UPDATE_STATUS_NONE                       = 0x0000,
        SMSG_UPDATE_DUNGEON_ENCOUNTER_FOR_LOOT            = 0x0000,

        /// LFG List
        SMSG_LFG_LIST_JOIN_RESULT                         = 0x109F, ///< 6.2.3 20726
        SMSG_LFG_LIST_UPDATE_BLACKLIST                    = 0x1048, ///< 6.2.3 20726
        SMSG_LFG_LIST_UPDATE_STATUS                       = 0x1157, ///< 6.2.3 20726
        SMSG_LFG_LIST_SEARCH_RESULT                       = 0x110B, ///< 6.2.3 20726
        SMSG_LFG_LIST_APPLY_FOR_GROUP_RESULT              = 0x1143, ///< 6.2.3 20726
        SMSG_LFG_LIST_APPLICANT_LIST_UPDATE               = 0x1108, ///< 6.2.3 20726
        SMSG_LFG_LIST_APPLICANT_GROUP_INVITE              = 0x105F, ///< 6.2.3 20726
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Garrison
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamGarrison
        SMSG_GET_GARRISON_INFO_RESULT                                   = 0x1196, ///< 6.2.3 20779
        SMSG_GARRISON_PLOT_PLACED                                       = 0x1191, ///< 6.2.3 20779
        SMSG_GARRISON_PLOT_REMOVED                                      = 0x1C16, ///< 6.2.3 20779
        SMSG_GARRISON_PLACE_BUILDING_RESULT                             = 0x1992, ///< 6.2.3 20779
        SMSG_GARRISON_BUILDING_REMOVED                                  = 0x113D, ///< 6.2.3 20779
        SMSG_GARRISON_LEARN_SPECIALIZATION_RESULT                       = 0x1095, ///< 6.2.3 20779
        SMSG_GARRISON_BUILDING_SET_ACTIVE_SPECIALIZATION_RESULT         = 0x199E, ///< 6.2.3 20779
        SMSG_GARRISON_LEARN_BLUEPRINT_RESULT                            = 0x1819, ///< 6.2.3 20779
        SMSG_GARRISON_BLUEPRINT_AND_SPECIALIZATION_DATA                 = 0x1491, ///< 6.2.3 20779
        SMSG_GARRISON_GET_BUILDINGS_DATA                                = 0x14B6, ///< 6.2.3 20779
        SMSG_GARRISON_REMOTE_INFO                                       = 0x1811, ///< 6.2.3 20779
        SMSG_GARRISON_BUILDING_ACTIVATED                                = 0x151A, ///< 6.2.3 20779
        SMSG_GARRISON_UPGRADE_RESULT                                    = 0x181E, ///< 6.2.3 20779
        SMSG_GARRISON_ADD_FOLLOWER_RESULT                               = 0x113A, ///< 6.2.3 20779
        SMSG_GARRISON_REMOVE_FOLLOWER_RESULT                            = 0x18B5, ///< 6.2.3 20779
        SMSG_GARRISON_LIST_FOLLOWERS_CHEAT_RESULT                       = 0x0000,
        SMSG_GARRISON_UPDATE_FOLLOWER_ACTIVATION_COUNT                  = 0x1832, ///< 6.2.3 20779
        SMSG_GARRISON_UPDATE_FOLLOWER                                   = 0x1939, ///< 6.2.3 20779
        SMSG_GARRISON_FOLLOWER_CHANGED_XP                               = 0x1599, ///< 6.2.3 20779
        SMSG_GARRISON_FOLLOWER_CHANGED_ITEM_LEVEL                       = 0x1111, ///< 6.2.3 20779
        SMSG_GARRISON_ADD_MISSION_RESULT                                = 0x1135, ///< 6.2.3 20779
        SMSG_GARRISON_START_MISSION_RESULT                              = 0x1C1A, ///< 6.2.3 20779
        SMSG_GARRISON_COMPLETE_MISSION_RESULT                           = 0x149D, ///< 6.2.3 20779
        SMSG_GARRISON_MISSION_BONUS_ROLL_RESULT                         = 0x14B2, ///< 6.2.3 20779
        SMSG_GARRISON_ASSIGN_FOLLOWER_TO_BUILDING_RESULT                = 0x1931, ///< 6.2.3 20779
        SMSG_GARRISON_REMOVE_FOLLOWER_FROM_BUILDING_RESULT              = 0x1036, ///< 6.2.3 20779
        SMSG_GARRISON_DELETE_RESULT                                     = 0x1836, ///< 6.2.3 20779
        SMSG_GARRISON_OPEN_ARCHITECT                                    = 0x1195, ///< 6.2.3 20779
        SMSG_GARRISON_OPEN_RECRUITMENT_NPC                              = 0x1C12, ///< 6.2.3 20779
        SMSG_GARRISON_RECRUITMENT_FOLLOWERS_GENERATED                   = 0x159E, ///< 6.2.3 20779
        SMSG_GARRISON_RECRUIT_FOLLOWER_RESULT                           = 0x18B5, ///< 6.2.3 20779
        SMSG_GARRISON_OPEN_MISSION_NPC                                  = 0x119E, ///< 6.2.3 20779
        SMSG_GARRISON_REQUEST_UPGRADEABLE_RESULT                        = 0x10BA, ///< 6.2.3 20779
        SMSG_GARRISON_OPEN_TRADESKILL_NPC                               = 0x141E, ///< 6.2.3 20779
        SMSG_GARRISON_SET_MISSION_NPC                                   = 0x159A, ///< 6.2.3 20779
        SMSG_GARRISON_FOLLOWER_DELETE_NOTIFY                            = 0x18B5, ///< 6.2.3 20779
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    /// Low protocol
    //////////////////////////////////////////////////////////////////////////
    CMSG_HANDSHAKE                              = 0x4F57,
    SMSG_HANDSHAKE                              = 0x4F57,

    //////////////////////////////////////////////////////////////////////////
    /// Garrison
    //////////////////////////////////////////////////////////////////////////
    CMSG_GARRISON_CREATE_CHEAT                              = 0x0000,
    CMSG_GARRISON_DELETE_CHEAT                              = 0x0000,
    CMSG_GARRISON_SET_LEVEL_CHEAT                           = 0x0000,
    CMSG_GARRISON_PLACE_BUILDING_CHEAT                      = 0x0000,
    CMSG_GARRISON_REMOVE_BUILDING_CHEAT                     = 0x0000,
    CMSG_GARRISON_UPGRADE_BUILDING_CHEAT                    = 0x0000,
    CMSG_GARRISON_CANCEL_CONSTRUCTION                       = 0x01AE, ///< 6.2.3 20726
    CMSG_GARRISON_LEARN_BUILDING_SPECIALIZATION_CHEAT       = 0x0000,
    CMSG_GARRISON_SET_ACTIVE_BUILDING_SPECIALIZATION        = 0x0000,
    CMSG_GARRISON_SET_ACTIVE_BUILDING_SPECIALIZATION_CHEAT  = 0x0000,
    CMSG_GET_GARRISON_INFO                                  = 0x0414, ///< 6.2.3 20726
    CMSG_GARRISON_LEARN_BLUEPRINT_CHEAT                     = 0x0000,
    CMSG_GARRISON_REQUEST_BUILDINGS                         = 0x0175, ///< 6.2.3 20779
    CMSG_GARRISON_PURCHASE_BUILDING                         = 0x047E, ///< 6.2.3 20779
    CMSG_GARRISON_SET_BUILDING_ACTIVE                       = 0x0417, ///< 6.2.3 20779
    CMSG_GARRISON_FORCE_BUILDING_ACTIVE                     = 0x0000,
    CMSG_GARRISON_PORT_CHEAT                                = 0x0000,
    CMSG_GARRISON_ADD_FOLLOWER_XPCHEAT                      = 0x0000,
    CMSG_GARRISON_SET_FOLLOWER_ITEM_LEVEL_CHEAT             = 0x0000,
    CMSG_GARRISON_ASSIGN_FOLLOWER_TO_BUILDING               = 0x0583, ///< 6.2.3 20779
    CMSG_GARRISON_REMOVE_FOLLOWER_FROM_BUILDING             = 0x100A, ///< 6.2.3 20779
    CMSG_GARRISON_CHANGE_FOLLOWER_ACTIVATION_STATE          = 0x08B0, ///< 6.2.3 20779
    CMSG_SHOW_GARRISON_TRANSFER                             = 0x0000,
    CMSG_GARRISON_ADD_UNIQUE_FOLLOWER_CHEAT                 = 0x0000,
    CMSG_GARRISON_REMOVE_FOLLOWER_CHEAT                     = 0x0000,
    CMSG_GARRISON_LIST_FOLLOWERS_CHEAT                      = 0x0000,
    CMSG_GARRISON_ADD_MISSION_CHEAT                         = 0x0000,
    CMSG_GARRISON_START_MISSION                             = 0x087E, ///< 6.2.3 20779
    CMSG_GARRISON_COMPLETE_MISSION                          = 0x0D4D, ///< 6.2.3 20779
    CMSG_GARRISON_MISSION_BONUS_ROLL                        = 0x0125, ///< 6.2.3 20779
    CMSG_GARRISON_GENERATE_RECRUITS                         = 0x016E, ///< 6.2.3 20779
    CMSG_GARRISON_SET_RECRUITMENT_PREFERENCES               = 0x013E, ///< 6.2.3 20779
    CMSG_GARRISON_RECRUIT_FOLLOWER                          = 0x0588, ///< 6.2.3 20779
    CMSG_GARRISON_REQUEST_UPGRADEABLE                       = 0x08ED, ///< 6.2.3 20779
    CMSG_UPGRADE_GARRISON                                   = 0x0466, ///< 6.2.3 20779
    CMSG_GARRISON_REQUEST_LANDING_PAGE_SHIPMENT_INFO        = 0x18AA, ///< 6.2.3 20779
    CMSG_GARRISON_MISSION_NPC_HELLO                         = 0x08B1, ///< 6.2.3 20779
    CMSG_GET_SHIPMENT_INFO                                  = 0x00EA, ///< 6.2.3 20779
    CMSG_CREATE_SHIPMENT                                    = 0x0575, ///< 6.2.3 20779
    CMSG_REQUEST_SET_MISSION_NPC                            = 0x092D, ///< 6.2.3 20779

    /// Shipyard
    CMSG_GARRISON_FOLLOWER_RENAME                           = 0x0472, ///< 6.2.3 20726
    CMSG_GARRISON_DECOMISSION_SHIP                          = 0x0136, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// User Router
    //////////////////////////////////////////////////////////////////////////
    CMSG_LOG_STREAMING_ERROR                    = 0x0838, ///< 6.2.3 20726
    CMSG_QUEUED_MESSAGES_END                    = 0x0878, ///< 6.2.3 20726
    CMSG_LOG_DISCONNECT                         = 0x0C67, ///< 6.2.3 20726
    CMSG_PING                                   = 0x0828, ///< 6.2.3 20726
    CMSG_AUTH_CONTINUED_SESSION                 = 0x0C33, ///< 6.2.3 20726
    CMSG_SUSPEND_TOKEN_RESPONSE                 = 0x08E8, ///< 6.2.3 20726
    CMSG_AUTH_SESSION                           = 0x0977, ///< 6.2.3 20726
    CMSG_ENABLE_NAGLE                           = 0x0827, ///< 6.2.3 20726
    CMSG_SUSPEND_COMMS_ACK                      = 0x0CB7, ///< 6.2.3 20726
    CMSG_KEEP_ALIVE                             = 0x1434, ///< 6.2.3 20726
    CMSG_OBJECT_UPDATE_FAILED                   = 0x01A9, ///< 6.2.3 20726
    CMSG_OBJECT_UPDATE_RESCUED                  = 0x059F, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Character
    //////////////////////////////////////////////////////////////////////////
    CMSG_ENUM_CHARACTERS                        = 0x1163, ///< 6.2.3 20726
    CMSG_CREATE_CHARACTER                       = 0x143B, ///< 6.2.3 20726
    CMSG_GENERATE_RANDOM_CHARACTER_NAME         = 0x0580, ///< 6.2.3 20726

    CMSG_CHAR_DELETE                            = 0x0428, ///< 6.2.3 20726
    CMSG_REORDER_CHARACTERS                     = 0x10B3, ///< 6.2.3 20726
    CMSG_PLAYER_LOGIN                           = 0x10F3, ///< 6.2.3 20726
    CMSG_VIOLENCE_LEVEL                         = 0x0CBA, ///< 6.2.3 20726
    CMSG_LOAD_SCREEN                            = 0x1428, ///< 6.2.3 20726
    CMSG_REQUEST_ACCOUNT_DATA                   = 0x01B4, ///< 6.2.3 20726
    CMSG_UPDATE_ACCOUNT_DATA                    = 0x102F, ///< 6.2.3 20726
    CMSG_SET_DUNGEON_DIFFICULTY                 = 0x00AF, ///< 6.2.3 20726
    CMSG_SET_RAID_DIFFICULTY                    = 0x103F, ///< 6.2.3 20726
    CMSG_AUTO_DECLINE_GUILD_INVITES             = 0x1D0A, ///< 6.2.3 20726
    CMSG_SHOWING_CLOAK                          = 0x118E, ///< 6.2.3 20726
    CMSG_SHOWING_HELM                           = 0x073E, ///< 6.2.3 20726
    CMSG_SET_ACTIVE_MOVER                       = 0x1936, ///< 6.2.3 20726
    CMSG_LEARN_TALENTS                          = 0x0497, ///< 6.2.3 20726
    CMSG_AUTOEQUIP_ITEM                         = 0x1375, ///< 6.2.3 20726
    CMSG_SWAP_INV_ITEM                          = 0x1379, ///< 6.2.3 20726
    CMSG_SWAP_ITEM                              = 0x1619, ///< 6.2.3 20726
    CMSG_AUTOSTORE_BAG_ITEM                     = 0x161E, ///< 6.2.3 20726
    CMSG_REQUEST_PET_INFO                       = 0x0000, ///<
    CMSG_STAND_STATE_CHANGE                     = 0x05EE, ///< 6.2.3 20726
    CMSG_BINDER_ACTIVATE                        = 0x073D, ///< 6.2.3 20726
    CMSG_REQUEST_FORCED_REACTIONS               = 0x086A, ///< 6.2.3 20726
    CMSG_DESTROY_ITEM                           = 0x00F2, ///< 6.2.3 20726
    CMSG_OPEN_ITEM                              = 0x042D, ///< 6.2.3 20726
    CMSG_SET_TITLE                              = 0x0C72, ///< 6.2.3 20726
    CMSG_PLAYED_TIME                            = 0x003D, ///< 6.2.3 20726
    CMSG_SAVE_EQUIPMENT_SET                     = 0x041E, ///< 6.2.3 20726
    CMSG_USE_EQUIPMENT_SET                      = 0x1236, ///< 6.2.3 20726
    CMSG_DELETE_EQUIPMENT_SET                   = 0x027A, ///< 6.2.3 20726
    CMSG_WHO                                    = 0x00A4, ///< 6.2.3 20726
    CMSG_SOCKET_GEMS                            = 0x021A, ///< 6.2.3 20726
    CMSG_RESURRECT_RESPONSE                     = 0x003C, ///< 6.2.3 20726
    CMSG_QUERY_INSPECT_ACHIEVEMENTS             = 0x08CE, ///< 6.2.3 20726
    CMSG_SPLIT_ITEM                             = 0x1219, ///< 6.2.3 20726
    CMSG_SET_PLAYER_DECLINED_NAMES              = 0x04B3, ///< 6.2.3 20726
    CMSG_MOUNT_SET_FAVORITE                     = 0x142C, ///< 6.2.3 20726
    CMSG_SORT_BAGS                              = 0x01ED, ///< 6.2.3 20726
    CMSG_CHAR_RENAME                            = 0x05F0, ///< 6.2.3 20726
    CMSG_CHAR_CUSTOMIZE                         = 0x0433, ///< 6.2.3 20726
    CMSG_CHAR_RACE_OR_FACTION_CHANGE            = 0x0528, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Bank
    //////////////////////////////////////////////////////////////////////////
    CMSG_BANKER_ACTIVATE                        = 0x1526, ///< 6.2.3 20726
    CMSG_AUTOBANK_ITEM                          = 0x121E, ///< 6.2.3 20726
    CMSG_AUTOSTORE_BANK_ITEM                    = 0x1312, ///< 6.2.3 20726
    CMSG_BUY_BANK_SLOT                          = 0x0016, ///< 6.2.3 20726
    CMSG_BUY_REAGENT_BANK                       = 0x0636, ///< 6.2.3 20726
    CMSG_SORT_BANK_BAGS                         = 0x09F2, ///< 6.2.3 20726
    CMSG_SORT_REAGENT_BANK_BAGS                 = 0x0CA2, ///< 6.2.3 20726
    CMSG_DEPOSIT_ALL_REAGENTS                   = 0x0C69, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Auction House
    //////////////////////////////////////////////////////////////////////////
    CMSG_AUCTION_LIST_BIDDER_ITEMS              = 0x150D, ///< 6.2.3 20726
    CMSG_AUCTION_LIST_ITEMS                     = 0x118D, ///< 6.2.3 20726
    CMSG_AUCTION_LIST_OWNER_ITEMS               = 0x15AE, ///< 6.2.3 20726
    CMSG_AUCTION_LIST_PENDING_SALES             = 0x0256, ///< 6.2.3 20726
    CMSG_AUCTION_PLACE_BID                      = 0x0811, ///< 6.2.3 20726
    CMSG_AUCTION_REMOVE_ITEM                    = 0x0659, ///< 6.2.3 20726
    CMSG_AUCTION_SELL_ITEM                      = 0x005D, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Loot
    //////////////////////////////////////////////////////////////////////////
    CMSG_LOOT                                   = 0x096E, ///< 6.2.3 20726
    CMSG_LOOT_MONEY                             = 0x0025, ///< 6.2.3 20726
    CMSG_LOOT_ITEM                              = 0x0936, ///< 6.2.3 20726
    CMSG_LOOT_RELEASE                           = 0x0922, ///< 6.2.3 20726
    CMSG_LOOT_ROLL                              = 0x08AD, ///< 6.2.3 20726
    CMSG_MASTER_LOOT_ITEM                       = 0x04FA, ///< 6.2.3 20726
    CMSG_DO_MASTER_LOOT_ROLL                    = 0x0036, ///< 6.2.3 20726
    CMSG_SET_LOOT_SPECIALIZATION                = 0x0A51, ///< 6.2.3 20726
    CMSG_SET_LOOT_METHOD                        = 0x016B, ///< 6.2.3 20726
    CMSG_OPT_OUT_OF_LOOT                        = 0x065A, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Combat
    //////////////////////////////////////////////////////////////////////////
    CMSG_ATTACKSTOP                             = 0x0DA6, ///< 6.2.3 20726
    CMSG_ATTACKSWING                            = 0x0C2A, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Duel
    //////////////////////////////////////////////////////////////////////////
    CMSG_DUEL_PROPOSED                          = 0x10EB, ///< 6.2.3 20726
    CMSG_DUEL_RESPONSE                          = 0x19A6, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Spell
    //////////////////////////////////////////////////////////////////////////
    CMSG_CAST_SPELL                             = 0x00F5, ///< 6.2.3 20726
    CMSG_CANCEL_CAST                            = 0x0525, ///< 6.2.3 20726
    CMSG_USE_ITEM                               = 0x0562, ///< 6.2.3 20726
    CMSG_CANCEL_AURA                            = 0x0CB6, ///< 6.2.3 20726
    CMSG_CANCEL_AUTO_REPEAT_SPELL               = 0x1185, ///< 6.2.3 20726
    CMSG_CANCEL_CHANNELLING                     = 0x01F1, ///< 6.2.3 20726
    CMSG_CANCEL_GROWTH_AURA                     = 0x002E, ///< 6.2.3 20726
    CMSG_CANCEL_MOUNT_AURA                      = 0x0425, ///< 6.2.3 20726
    CMSG_CANCEL_QUEUED_SPELL                    = 0x0935, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Cache
    //////////////////////////////////////////////////////////////////////////
    CMSG_DB_QUERY_BULK                          = 0x10E4, ///< 6.2.3 20726
    CMSG_QUERY_CREATURE                         = 0x1002, ///< 6.2.3 20726
    CMSG_NPC_TEXT_QUERY                         = 0x04E2, ///< 6.2.3 20726
    CMSG_NAME_QUERY                             = 0x01A4, ///< 6.2.3 20726
    CMSG_QUEST_QUERY                            = 0x08BD, ///< 6.2.3 20726
    CMSG_QUEST_POI_QUERY                        = 0x04AF, ///< 6.2.3 20726
    CMSG_REALM_NAME_QUERY                       = 0x016C, ///< 6.2.3 20726
    CMSG_GAMEOBJECT_QUERY                       = 0x182D, ///< 6.2.3 20726
    CMSG_PETITION_QUERY                         = 0x050A, ///< 6.2.3 20726
    CMSG_QUERY_GUILD_INFO                       = 0x00BC, ///< 6.2.3 20726
    CMSG_PAGE_TEXT_QUERY                        = 0x0C21, ///< 6.2.3 20726
    CMSG_ITEM_TEXT_QUERY                        = 0x08BA, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Interaction
    //////////////////////////////////////////////////////////////////////////
    CMSG_LOGOUT_REQUEST                         = 0x1126, ///< 6.2.3 20726
    CMSG_LOGOUT_CANCEL                          = 0x0632, ///< 6.2.3 20726
    CMSG_SET_ACTION_BUTTON                      = 0x1037, ///< 6.2.3 20726
    CMSG_SET_ACTIONBAR_TOGGLES                  = 0x1D29, ///< 6.2.3 20726
    CMSG_SET_SELECTION                          = 0x1986, ///< 6.2.3 20726
    CMSG_SETSHEATHED                            = 0x0C5A, ///< 6.2.3 20726
    CMSG_GOSSIP_HELLO                           = 0x0A19, ///< 6.2.3 20726
    CMSG_GOSSIP_SELECT_OPTION                   = 0x0A59, ///< 6.2.3 20726
    CMSG_TOGGLE_PVP                             = 0x08AC, ///< 6.2.3 20726
    CMSG_TUTORIAL                               = 0x0164, ///< 6.2.3 20726
    CMSG_SET_SPECIALIZATION                     = 0x1C82, ///< 6.2.3 20726
    CMSG_SET_WATCHED_FACTION                    = 0x1905, ///< 6.2.3 20726
    CMSG_SET_FACTION_INACTIVE                   = 0x1DAE, ///< 6.2.3 20726
    CMSG_AREATRIGGER                            = 0x043D, ///< 6.2.3 20726
    CMSG_GAMEOBJECT_REPORT_USE                  = 0x1101, ///< 6.2.3 20726
    CMSG_GAMEOBJECT_USE                         = 0x0359, ///< 6.2.3 20726
    CMSG_SAVE_CUF_PROFILES                      = 0x000A, ///< 6.2.3 20726
    CMSG_SPELLCLICK                             = 0x150A, ///< 6.2.3 20726
    CMSG_REPOP_REQUEST                          = 0x15A9, ///< 6.2.3 20726
    CMSG_RECLAIM_CORPSE                         = 0x0275, ///< 6.2.3 20726
    CMSG_QUERY_CORPSE_LOCATION_FROM_CLIENT      = 0x0074, ///< 6.2.3 20726
    CMSG_QUERY_CORPSE_TRANSPORT                 = 0x1124, ///< 6.2.3 20726
    CMSG_RETURN_TO_GRAVEYARD                    = 0x1509, ///< 6.2.3 20726
    CMSG_CLOSE_INTERACTION                      = 0x001D, ///< 6.2.3 20726
    CMSG_ITEM_REFUND_INFO                       = 0x1DA1, ///< 6.2.3 20726
    CMSG_FAR_SIGHT                              = 0x025A, ///< 6.2.3 20726
    CMSG_MOUNTSPECIAL_ANIM                      = 0x04F5, ///< 6.2.3 20726
    CMSG_OPENING_CINEMATIC                      = 0x0655, ///< 6.2.3 20726
    CMSG_NEXT_CINEMATIC_CAMERA                  = 0x0C51, ///< 6.2.3 20726
    CMSG_COMPLETE_CINEMATIC                     = 0x023A, ///< 6.2.3 20726
    CMSG_REQUEST_CEMETERY_LIST                  = 0x0DBA, ///< 6.2.3 20726
    CMSG_TOTEM_DESTROYED                        = 0x0056, ///< 6.2.3 20726
    CMSG_CONFIRM_RESPEC_WIPE                    = 0x04A2, ///< 6.2.3 20726
    CMSG_CANCEL_TRADE                           = 0x18AE, ///< 6.2.3 20726
    CMSG_SET_TRADE_CURRENCY                     = 0x140A, ///< 6.2.3 20726
    CMSG_SET_TRADE_GOLD                         = 0x0CE5, ///< 6.2.3 20726
    CMSG_SET_TRADE_ITEM                         = 0x0105, ///< 6.2.3 20726
    CMSG_CLEAR_TRADE_ITEM                       = 0x087A, ///< 6.2.3 20726
    CMSG_ACCEPT_TRADE                           = 0x00FD, ///< 6.2.3 20726
    CMSG_BUSY_TRADE                             = 0x0590, ///< 6.2.3 20726
    CMSG_BEGIN_TRADE                            = 0x1022, ///< 6.2.3 20726
    CMSG_IGNORE_TRADE                           = 0x083C, ///< 6.2.3 20726
    CMSG_INITIATE_TRADE                         = 0x041F, ///< 6.2.3 20726
    CMSG_UNACCEPT_TRADE                         = 0x1081, ///< 6.2.3 20726
    CMSG_NEUTRAL_PLAYER_SELECT_FACTION          = 0x092E, ///< 6.2.3 20726
    CMSG_INSPECT                                = 0x0371, ///< 6.2.3 20726
    CMSG_INSPECT_HONOR_STATS                    = 0x1C86, ///< 6.2.3 20726
    CMSG_REQUEST_INSPECT_RATED_BG_STATS         = 0x1024, ///< 6.2.3 20726
    CMSG_TIME_SYNC_RESP                         = 0x119D, ///< 6.2.3 20726
    CMSG_UNLEARN_SKILL                          = 0x0816, ///< 6.2.3 20726
    CMSG_EMOTE                                  = 0x077E, ///< 6.2.3 20726
    CMSG_SEND_TEXT_EMOTE                        = 0x0412, ///< 6.2.3 20726
    CMSG_ALTER_APPEARANCE                       = 0x152D, ///< 6.2.3 20726
    CMSG_SELF_RES                               = 0x1989, ///< 6.2.3 20726
    CMSG_READ_ITEM                              = 0x0071, ///< 6.2.3 20726
    CMSG_COMPLETE_MOVIE                         = 0x0612, ///< 6.2.3 20726
    CMSG_SCENE_TRIGGER_EVENT                    = 0x084D, ///< 6.2.3 20726
    CMSG_GET_MIRRORIMAGE_DATA                   = 0x0461, ///< 6.2.3 20726
    CMSG_SHOW_TRADE_SKILL                       = 0x11C0, ///< 6.2.3 20726
    CMSG_SCENE_PLAYBACK_CANCELED                = 0x0C65, ///< 6.2.3 20726
    CMSG_REQUEST_RESEARCH_HISTORY               = 0x017A, ///< 6.2.3 20726
    CMSG_SUMMON_RESPONSE                        = 0x0577, ///< 6.2.3 20726
    CMSG_SET_FACTION_ATWAR                      = 0x1582, ///< 6.2.3 20726
    CMSG_UNSET_FACTION_ATWAR                    = 0x077D, ///< 6.2.3 20726
    CMSG_USE_CRITTER_ITEM                       = 0x0408, ///< 6.2.3 20779

    //////////////////////////////////////////////////////////////////////////
    /// Vehicles
    //////////////////////////////////////////////////////////////////////////
    CMSG_MOVE_SET_VEHICLE_REC_ID_ACK            = 0x1836, ///< 6.2.3 20726
    CMSG_PLAYER_VEHICLE_ENTER                   = 0x1CA9, ///< 6.2.3 20726
    CMSG_REQUEST_VEHICLE_EXIT                   = 0x0DF5, ///< 6.2.3 20726
    CMSG_REQUEST_VEHICLE_NEXT_SEAT              = 0x08BB, ///< 6.2.3 20726
    CMSG_REQUEST_VEHICLE_PREV_SEAT              = 0x10A6, ///< 6.2.3 20726
    CMSG_REQUEST_VEHICLE_SWITCH_SEAT            = 0x0C6A, ///< 6.2.3 20726
    CMSG_CHANGE_SEATS_ON_CONTROLLED_VEHICLE     = 0x1032, ///< 6.2.3 20726
    CMSG_EJECT_PASSENGER                        = 0x0D65, ///< 6.2.3 20726
    CMSG_DISMISS_CONTROLLED_VEHICLE             = 0x111E, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Vendors
    //////////////////////////////////////////////////////////////////////////
    CMSG_LIST_INVENTORY                         = 0x0A55, ///< 6.2.3 20726
    CMSG_REPAIR_ITEM                            = 0x0736, ///< 6.2.3 20726
    CMSG_BUYBACK_ITEM                           = 0x1906, ///< 6.2.3 20726
    CMSG_BUY_ITEM                               = 0x158A, ///< 6.2.3 20726
    CMSG_SELL_ITEM                              = 0x1922, ///< 6.2.3 20726
    CMSG_ITEM_REFUND                            = 0x0C1A, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Taxi
    //////////////////////////////////////////////////////////////////////////
    CMSG_SET_TAXI_BENCHMARK_MODE                = 0x0455, ///< 6.2.3 20726
    CMSG_ENABLE_TAXI_NODE                       = 0x190D, ///< 6.2.3 20726
    CMSG_TAXI_QUERY_AVAILABLE_NODES             = 0x0336, ///< 6.2.3 20726
    CMSG_ACTIVATE_TAXI                          = 0x037A, ///< 6.2.3 20726
    CMSG_TAXI_NODE_STATUS_QUERY                 = 0x1DAD, ///< 6.2.3 20726
    CMSG_TAXI_REQUEST_EARLY_LANDING             = 0x0755, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Quest
    //////////////////////////////////////////////////////////////////////////
    CMSG_QUESTGIVER_HELLO                       = 0x035A, ///< 6.2.3 20726
    CMSG_QUEST_GIVER_STATUS_QUERY               = 0x001A, ///< 6.2.3 20726
    CMSG_QUESTGIVER_STATUS_MULTIPLE_QUERY       = 0x061E, ///< 6.2.3 20726
    CMSG_QUESTGIVER_QUERY_QUEST                 = 0x198A, ///< 6.2.3 20726
    CMSG_QUESTGIVER_COMPLETE_QUEST              = 0x0775, ///< 6.2.3 20726
    CMSG_QUESTGIVER_CHOOSE_REWARD               = 0x037E, ///< 6.2.3 20726
    CMSG_QUESTGIVER_ACCEPT_QUEST                = 0x005E, ///< 6.2.3 20726
    CMSG_QUESTGIVER_REQUEST_REWARD              = 0x118A, ///< 6.2.3 20726
    CMSG_QUEST_CONFIRM_ACCEPT                   = 0x0C16, ///< 6.2.3 20726
    CMSG_QUESTLOG_REMOVE_QUEST                  = 0x033E, ///< 6.2.3 20726
    CMSG_PUSH_QUEST_TO_PARTY                    = 0x110A, ///< 6.2.3 20726
    CMSG_QUEST_PUSH_RESULT                      = 0x0415, ///< 6.2.3 20726
    CMSG_QUERY_QUEST_COMPLETION_NPCS            = 0x04E9, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Account data
    //////////////////////////////////////////////////////////////////////////
    CMSG_GET_UNDELETE_CHARACTER_COOLDOWN_STATUS = 0x017C, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Chat
    //////////////////////////////////////////////////////////////////////////
    /// Addon chat
    CMSG_CHAT_ADDON_MESSAGE_INSTANCE_CHAT       = 0x17FA, ///< 6.2.3 20779
    CMSG_CHAT_ADDON_MESSAGE_WHISPER             = 0x16D9, ///< 6.2.3 20779
    CMSG_CHAT_ADDON_MESSAGE_GUILD               = 0x03FE, ///< 6.2.3 20779
    CMSG_CHAT_ADDON_MESSAGE_OFFICER             = 0x16FD, ///< 6.2.3 20779
    CMSG_CHAT_ADDON_MESSAGE_RAID                = 0x07B1, ///< 6.2.3 20779
    CMSG_CHAT_ADDON_MESSAGE_PARTY               = 0x02BE, ///< 6.2.3 20779
    CMSG_CHAT_ADDON_MESSAGE_CHANNEL             = 0x03D6, ///< 6.2.3 20779
    CMSG_CHAT_REGISTER_ADDON_PREFIXES           = 0x0692, ///< 6.2.3 20779
    CMSG_CHAT_UNREGISTER_ALL_ADDON_PREFIXES     = 0x03F9, ///< 6.2.3 20779

    /// Chat
    CMSG_CHAT_MESSAGE_RAID_WARNING              = 0x0EF9, ///< 6.2.3 20726
    CMSG_CHAT_MESSAGE_PARTY                     = 0x12F5, ///< 6.2.3 20726
    CMSG_CHAT_MESSAGE_YELL                      = 0x03BD, ///< 6.2.3 20726
    CMSG_CHAT_MESSAGE_SAY                       = 0x03BE, ///< 6.2.3 20726
    CMSG_CHAT_MESSAGE_OFFICER                   = 0x13B5, ///< 6.2.3 20726
    CMSG_CHAT_MESSAGE_EMOTE                     = 0x1799, ///< 6.2.3 20726
    CMSG_CHAT_MESSAGE_AFK                       = 0x07BE, ///< 6.2.3 20726
    CMSG_CHAT_MESSAGE_DND                       = 0x13DA, ///< 6.2.3 20726
    CMSG_CHAT_MESSAGE_GUILD                     = 0x13FA, ///< 6.2.3 20726
    CMSG_CHAT_MESSAGE_RAID                      = 0x07D2, ///< 6.2.3 20726
    CMSG_CHAT_MESSAGE_WHISPER                   = 0x13BD, ///< 6.2.3 20726
    CMSG_CHAT_MESSAGE_CHANNEL                   = 0x0BB2, ///< 6.2.3 20726

    /// Channel
    CMSG_JOIN_CHANNEL                           = 0x0BF6, ///< 6.2.3 20726
    CMSG_CHANNEL_ANNOUNCEMENTS                  = 0x0BBE, ///< 6.2.3 20726
    CMSG_CHANNEL_BAN                            = 0x0FFA, ///< 6.2.3 20726
    CMSG_CHANNEL_DISPLAY_LIST                   = 0x039E, ///< 6.2.0 19173
    CMSG_CHANNEL_INVITE                         = 0x07DE, ///< 6.2.3 20726
    CMSG_CHANNEL_KICK                           = 0x12B6, ///< 6.2.3 20726
    CMSG_CHANNEL_LIST                           = 0x139D, ///< 6.2.3 20726
    CMSG_CHANNEL_MODERATOR                      = 0x13B1, ///< 6.2.3 20726
    CMSG_CHANNEL_MUTE                           = 0x1295, ///< 6.2.3 20726
    CMSG_CHANNEL_OWNER                          = 0x0EFE, ///< 6.2.3 20726
    CMSG_CHANNEL_PASSWORD                       = 0x16B2, ///< 6.2.3 20726
    CMSG_CHANNEL_SET_OWNER                      = 0x13D2, ///< 6.2.3 20726
    CMSG_CHANNEL_SILENCE_ALL                    = 0x13DE, ///< 6.2.3 20726
    CMSG_CHANNEL_SILENCE_VOICE                  = 0x02F1, ///< 6.2.3 20726
    CMSG_CHANNEL_UNBAN                          = 0x0FB1, ///< 6.2.3 20726
    CMSG_CHANNEL_UNMODERATOR                    = 0x13F1, ///< 6.2.3 20726
    CMSG_CHANNEL_UNMUTE                         = 0x03DD, ///< 6.2.3 20726
    CMSG_CHANNEL_UNSILENCE_ALL                  = 0x02D2, ///< 6.2.3 20726
    CMSG_CHANNEL_UNSILENCE_VOICE                = 0x0AF9, ///< 6.2.3 20726
    CMSG_CHANNEL_VOICE_OFF                      = 0x0EDE, ///< 6.2.3 20726
    CMSG_CHANNEL_VOICE_ON                       = 0x1A96, ///< 6.2.3 20726
    CMSG_DECLINE_CHANNEL_INVITE                 = 0x0BF5, ///< 6.2.3 20726
    CMSG_LEAVE_CHANNEL                          = 0x0BF1, ///< 6.2.3 20726
    CMSG_SET_ACTIVE_VOICE_CHANNEL               = 0x00FF, ///< 6.2.3 20726


    //////////////////////////////////////////////////////////////////////////
    /// Movement
    //////////////////////////////////////////////////////////////////////////
    /// Start
    CMSG_MOVE_START_FORWARD                     = 0x1196, ///< 6.2.3 20726
    CMSG_MOVE_START_TURN_LEFT                   = 0x1819, ///< 6.2.3 20726
    CMSG_MOVE_START_TURN_RIGHT                  = 0x141D, ///< 6.2.3 20726
    CMSG_MOVE_START_BACKWARD                    = 0x1191, ///< 6.2.3 20726
    CMSG_MOVE_START_STRAFE_LEFT                 = 0x1992, ///< 6.2.3 20726
    CMSG_MOVE_START_STRAFE_RIGHT                = 0x113D, ///< 6.2.3 20726
    CMSG_MOVE_START_ASCEND                      = 0x1515, ///< 6.2.3 20726
    CMSG_MOVE_START_DESCEND                     = 0x1419, ///< 6.2.3 20726
    CMSG_MOVE_START_PITCH_UP                    = 0x1811, ///< 6.2.3 20726
    CMSG_MOVE_START_PITCH_DOWN                  = 0x151A, ///< 6.2.3 20726
    CMSG_MOVE_START_SWIM                        = 0x189E, ///< 6.2.3 20726

    /// Misc
    CMSG_MOVE_JUMP                              = 0x199E, ///< 6.2.3 20726
    CMSG_MOVE_FALL_LAND                         = 0x149D, ///< 6.2.3 20726
    CMSG_WORLD_PORT_RESPONSE                    = 0x04F7, ///< 6.2.3 20726
    CMSG_MOVE_TIME_SKIPPED                      = 0x18B9, ///< 6.2.3 20726
    CMSG_MOVE_SPLINE_DONE                       = 0x1991, ///< 6.2.3 20726

    /// Update
    CMSG_MOVE_HEARTBEAT                         = 0x1512, ///< 6.2.3 20726
    CMSG_MOVE_SET_FACING                        = 0x1119, ///< 6.2.3 20726
    CMSG_MOVE_SET_PITCH                         = 0x1532, ///< 6.2.3 20726
    CMSG_MOVE_SET_RUN_MODE                      = 0x181E, ///< 6.2.3 20726
    CMSG_MOVE_SET_WALK_MODE                     = 0x113A, ///< 6.2.3 20726
    CMSG_MOVE_TELEPORT_ACK                      = 0x1C1A, ///< 6.2.3 20726
    CMSG_MOVE_CHNG_TRANSPORT                    = 0x1412, ///< 6.2.3 20726

    /// Stop
    CMSG_MOVE_STOP                              = 0x1C16, ///< 6.2.3 20726
    CMSG_MOVE_STOP_TURN                         = 0x1491, ///< 6.2.3 20726
    CMSG_MOVE_STOP_STRAFE                       = 0x1095, ///< 6.2.3 20726
    CMSG_MOVE_STOP_SWIM                         = 0x14B5, ///< 6.2.3 20726
    CMSG_MOVE_STOP_ASCEND                       = 0x1912, ///< 6.2.3 20726
    CMSG_MOVE_STOP_PITCH                        = 0x1519, ///< 6.2.3 20726

    /// ACK
    CMSG_MOVE_FORCE_RUN_SPEED_CHANGE_ACK         = 0x1436, ///< 6.2.3 20726
    CMSG_MOVE_FORCE_RUN_BACK_SPEED_CHANGE_ACK    = 0x1832, ///< 6.2.3 20726
    CMSG_MOVE_FORCE_WALK_SPEED_CHANGE_ACK        = 0x15BE, ///< 6.2.3 20726
    CMSG_MOVE_FORCE_SWIM_SPEED_CHANGE_ACK        = 0x1931, ///< 6.2.3 20726
    CMSG_MOVE_FORCE_SWIM_BACK_SPEED_CHANGE_ACK   = 0x14B6, ///< 6.2.3 20726
    CMSG_MOVE_FORCE_FLIGHT_SPEED_CHANGE_ACK      = 0x1431, ///< 6.2.3 20726
    CMSG_MOVE_FORCE_FLIGHT_BACK_SPEED_CHANGE_ACK = 0x103D, ///< 6.2.3 20726
    CMSG_MOVE_FORCE_TURN_RATE_CHANGE_ACK         = 0x1112, ///< 6.2.3 20726
    CMSG_MOVE_FORCE_PITCH_RATE_CHANGE_ACK        = 0x10B2, ///< 6.2.3 20726
    CMSG_MOVE_KNOCK_BACK_ACK                     = 0x159E, ///< 6.2.3 20726
    CMSG_MOVE_SET_CAN_FLY_ACK                    = 0x1411, ///< 6.2.3 20726
    CMSG_MOVE_FEATHER_FALL_ACK                   = 0x11BE, ///< 6.2.3 20726
    CMSG_MOVE_WATER_WALK_ACK                     = 0x18B2, ///< 6.2.3 20726
    CMSG_MOVE_HOVER_ACK                          = 0x111D, ///< 6.2.3 20726
    CMSG_MOVE_APPLY_MOVEMENT_FORCE_ACK           = 0x1195, ///< 6.2.3 20779
    CMSG_MOVE_REMOVE_MOVEMENT_FORCE_ACK          = 0x141E, ///< 6.2.3 20779

    //////////////////////////////////////////////////////////////////////////
    /// Group
    //////////////////////////////////////////////////////////////////////////
    CMSG_PARTY_INVITE                           = 0x11FC, ///< 6.2.3 20726
    CMSG_PARTY_INVITE_RESPONSE                  = 0x057B, ///< 6.2.3 20726
    CMSG_LEAVE_GROUP                            = 0x01A7, ///< 6.2.3 20726
    CMSG_PARTY_UNINVITE                         = 0x05FF, ///< 6.2.3 20726
    CMSG_SET_PARTY_LEADER                       = 0x11E7, ///< 6.2.3 20726
    CMSG_SET_ROLE                               = 0x01BF, ///< 6.2.3 20726
    CMSG_MINIMAP_PING                           = 0x1173, ///< 6.2.3 20726
    CMSG_RANDOM_ROLL                            = 0x10BC, ///< 6.2.3 20726
    CMSG_UPDATE_RAID_TARGET                     = 0x01B8, ///< 6.2.3 20726
    CMSG_CONVERT_RAID                           = 0x11FF, ///< 6.2.3 20726
    CMSG_DO_READY_CHECK                         = 0x012B, ///< 6.2.3 20726
    CMSG_READY_CHECK_RESPONSE                   = 0x1078, ///< 6.2.3 20726
    CMSG_INITIATE_ROLE_POLL                     = 0x0124, ///< 6.2.3 20726
    CMSG_REQUEST_RAID_INFO                      = 0x0533, ///< 6.2.3 20726
    CMSG_SET_EVERYONE_IS_ASSISTANT              = 0x0437, ///< 6.2.3 20726
    CMSG_CLEAR_RAID_MARKER                      = 0x09EE, ///< 6.2.3 20726
    CMSG_GROUP_ASSISTANT_LEADER                 = 0x04BB, ///< 6.2.3 20726
    CMSG_GROUP_ASSIGNMENT                       = 0x1074, ///< 6.2.3 20726
    CMSG_GROUP_CHANGE_SUB_GROUP                 = 0x11B8, ///< 6.2.3 20726
    CMSG_GROUP_REQUEST_JOIN_UPDATES             = 0x0570, ///< 6.2.3 20726
    CMSG_GROUP_SWAP_SUB_GROUP                   = 0x05EC, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Friend
    //////////////////////////////////////////////////////////////////////////
    CMSG_SEND_CONTACT_LIST                      = 0x0077, ///< 6.2.3 20726
    CMSG_ADD_FRIEND                             = 0x046B, ///< 6.2.3 20726
    CMSG_ADD_IGNORE                             = 0x04A7, ///< 6.2.3 20726
    CMSG_SET_CONTACT_NOTES                      = 0x1030, ///< 6.2.3 20726
    CMSG_DEL_FRIEND                             = 0x117C, ///< 6.2.3 20726
    CMSG_DEL_IGNORE                             = 0x11EB, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Petition
    //////////////////////////////////////////////////////////////////////////
    CMSG_OFFER_PETITION                         = 0x053B, ///< 6.2.3 20726
    CMSG_PETITION_BUY                           = 0x150E, ///< 6.2.3 20726
    CMSG_PETITION_DECLINE                       = 0x1D86, ///< 6.2.3 20726
    CMSG_PETITION_RENAME                        = 0x1133, ///< 6.2.3 20726
    CMSG_PETITION_SHOWLIST                      = 0x0779, ///< 6.2.3 20726
    CMSG_PETITION_SHOW_SIGNATURES               = 0x1521, ///< 6.2.3 20726
    CMSG_PETITION_SIGN                          = 0x0679, ///< 6.2.3 20726
    CMSG_TURN_IN_PETITION                       = 0x0A3A, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Pet
    //////////////////////////////////////////////////////////////////////////
    CMSG_PET_NAME_QUERY                         = 0x0569, ///< 6.2.3 20726
    CMSG_PET_RENAME                             = 0x05F7, ///< 6.2.3 20726
    CMSG_PET_ABANDON                            = 0x1129, ///< 6.2.3 20726
    CMSG_PET_ACTION                             = 0x0312, ///< 6.2.3 20726
    CMSG_PET_CANCEL_AURA                        = 0x0019, ///< 6.2.3 20726
    CMSG_PET_CAST_SPELL                         = 0x1089, ///< 6.2.3 20726
    CMSG_PET_SET_ACTION                         = 0x0355, ///< 6.2.3 20726
    CMSG_PET_STOP_ATTACK                        = 0x0271, ///< 6.2.3 20726
    CMSG_LEARN_PET_SPECIALIZATION_GROUP         = 0x04B1, ///< 6.2.3 20726
    CMSG_LIST_STABLE_PETS                       = 0x0456, ///< 6.2.3 20726
    CMSG_SET_PET_SLOT                           = 0x0C45, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Battlegrounds
    //////////////////////////////////////////////////////////////////////////
    CMSG_BATTLEMASTER_JOIN                      = 0x0A71, ///< 6.2.3 20726
    CMSG_BATTLEMASTER_JOIN_ARENA                = 0x0A16, ///< 6.2.3 20726
    CMSG_BATTLEMASTER_JOIN_ARENA_SKIRMISH       = 0x1DA9, ///< 6.2.3 20726
    CMSG_JOIN_RATED_BATTLEGROUND                = 0x053A, ///< 6.2.3 20726
    CMSG_BATTLEFIELD_PORT                       = 0x0619, ///< 6.2.3 20726
    CMSG_REQUEST_BATTLEFIELD_STATUS             = 0x116B, ///< 6.2.3 20726
    CMSG_BATTLEFIELD_REQUEST_SCORE_DATA         = 0x0075, ///< 6.2.3 20726
    CMSG_REQUEST_PVP_REWARDS                    = 0x0435, ///< 6.2.3 20726
    CMSG_REQUEST_PVP_OPTIONS_ENABLED            = 0x1038, ///< 6.2.3 20726
    CMSG_QUERY_COUNTDOWN_TIMER                  = 0x08F2, ///< 6.2.3 20726
    CMSG_REQUEST_CONQUEST_FORMULA_CONSTANTS     = 0x0875, ///< 6.2.3 20726
    CMSG_LEAVE_BATTLEFIELD                      = 0x0429, ///< 6.2.3 20726
    CMSG_SPIRIT_HEALER_ACTIVATE                 = 0x0A36, ///< 6.2.3 20726
    CMSG_AREA_SPIRIT_HEALER_QUERY               = 0x1DAA, ///< 6.2.3 20726
    CMSG_AREA_SPIRIT_HEALER_QUEUE               = 0x0675, ///< 6.2.3 20726
    CMSG_BATTLEFIELD_MGR_QUEUE_REQUEST          = 0x0846, ///< 6.2.3 20726
    CMSG_BATTLEFIELD_MGR_QUEUE_INVITE_RESPONSE  = 0x00E8, ///< 6.2.3 20726
    CMSG_BATTLEFIELD_MGR_QUEUE_EXIT_REQUEST     = 0x10BF, ///< 6.2.3 20726
    CMSG_BATTLEFIELD_MGR_ENTRY_INVITE_RESPONSE  = 0x04B4, ///< 6.2.3 20726
    CMSG_BATTLEFIELD_LIST                       = 0x08F9, ///< 6.2.3 20726
    CMSG_REQUEST_RATED_BATTLEFIELD_INFO         = 0x00B0, ///< 6.2.3 20726
    CMSG_START_WAR_GAME                         = 0x0100, ///< 6.2.3 20726
    CMSG_ACCEPT_WARGAME_INVITE                  = 0x117F, ///< 6.2.3 20726
    CMSG_REPORT_PVP_AFK                         = 0x1D09, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Guild
    //////////////////////////////////////////////////////////////////////////
    CMSG_GUILD_INVITE_BY_NAME                      = 0x1440, ///< 6.2.3 20726
    CMSG_ACCEPT_GUILD_INVITE                       = 0x00BF, ///< 6.2.3 20726
    CMSG_GUILD_DECLINE_INVITATION                  = 0x019D, ///< 6.2.3 20726
    CMSG_GUILD_OFFICER_REMOVE_MEMBER               = 0x0889, ///< 6.2.3 20726
    CMSG_GUILD_SET_GUILD_MASTER                    = 0x0034, ///< 6.2.3 20726
    CMSG_GUILD_LEAVE                               = 0x09CE, ///< 6.2.3 20726
    CMSG_GUILD_DELETE                              = 0x0589, ///< 6.2.3 20726
    CMSG_GUILD_UPDATE_MOTD_TEXT                    = 0x01CE, ///< 6.2.3 20726
    CMSG_GUILD_UPDATE_INFO_TEXT                    = 0x0882, ///< 6.2.3 20726
    CMSG_GUILD_GET_RANKS                           = 0x0195, ///< 6.2.3 20726
    CMSG_GUILD_ADD_RANK                            = 0x048D, ///< 6.2.3 20726
    CMSG_GUILD_DELETE_RANK                         = 0x04C1, ///< 6.2.3 20726
    CMSG_GUILD_SET_RANK_PERMISSIONS                = 0x058A, ///< 6.2.3 20726
    CMSG_GUILD_SHIFT_RANK                          = 0x0881, ///< 6.2.3 20726
    CMSG_GUILD_ASSIGN_MEMBER_RANK                  = 0x09C2, ///< 6.2.3 20726
    CMSG_GUILD_GET_ROSTER                          = 0x04D2, ///< 6.2.3 20726
    CMSG_GUILD_BANK_ACTIVATE                       = 0x0311, ///< 6.2.3 20726
    CMSG_GUILD_BANK_BUY_TAB                        = 0x0C11, ///< 6.2.3 20726
    CMSG_GUILD_BANK_DEPOSIT_MONEY                  = 0x0051, ///< 6.2.3 20726
    CMSG_GUILD_BANK_LOG_QUERY                      = 0x0C82, ///< 6.2.3 20726
    CMSG_GUILD_BANK_REMAINING_WITHDRAW_MONEY_QUERY = 0x05CE, ///< 6.2.3 20726
    CMSG_GUILD_BANK_QUERY_TAB                      = 0x0252, ///< 6.2.3 20726
    CMSG_GUILD_BANK_TEXT_QUERY                     = 0x048E, ///< 6.2.3 20726
    CMSG_GUILD_BANK_SWAP_ITEMS                     = 0x1522, ///< 6.2.3 20726
    CMSG_GUILD_BANK_UPDATE_TAB                     = 0x112E, ///< 6.2.3 20726
    CMSG_GUILD_BANK_WITHDRAW_MONEY                 = 0x0855, ///< 6.2.3 20726
    CMSG_GUILD_BANK_SET_TAB_TEXT                   = 0x01C5, ///< 6.2.3 20726
    CMSG_GUILD_GET_ACHIEVEMENT_MEMBERS             = 0x0000, ///< (unused)
    CMSG_GUILD_SET_FOCUSED_ACHIEVEMENT             = 0x1921, ///< 6.2.3 20726
    CMSG_GUILD_SET_ACHIEVEMENT_TRACKING            = 0x0000, ///< (unused)
    CMSG_GUILD_SET_MEMBER_NOTE                     = 0x0586, ///< 6.2.3 20726
    CMSG_GUILD_CHALLENGE_UPDATE_REQUEST            = 0x0592, ///< 6.2.3 20726
    CMSG_REQUEST_GUILD_PARTY_STATE                 = 0x08A6, ///< 6.2.3 20726
    CMSG_REQUEST_GUILD_REWARDS_LIST                = 0x00B1, ///< 6.2.3 20726
    CMSG_GUILD_REPLACE_GUILD_MASTER                = 0x08D5, ///< 6.2.3 20726
    CMSG_GUILD_CHANGE_NAME_REQUEST                 = 0x0000, ///<  (unused)
    CMSG_GUILD_PERMISSIONS_QUERY                   = 0x018D, ///< 6.2.3 20726
    CMSG_GUILD_EVENT_LOG_QUERY                     = 0x0896, ///< 6.2.3 20726
    CMSG_GUILD_NEWS_UPDATE_STICKY                  = 0x0C8A, ///< 6.2.3 20726
    CMSG_GUILD_QUERY_NEWS                          = 0x09C6, ///< 6.2.3 20726
    CMSG_QUERY_GUILD_MEMBERS_FOR_RECIPE            = 0x0000, ///< (unused)
    CMSG_QUERY_GUILD_MEMBER_RECIPES                = 0x0000, ///< (unused)
    CMSG_QUERY_GUILD_RECIPES                       = 0x019A, ///< 6.2.3 20726
    CMSG_PLAYER_SAVE_GUILD_EMBLEM                  = 0x1402, ///< 6.2.3 20726

    /// Guild finding
    CMSG_LF_GUILD_ADD_RECRUIT                      = 0x11F7, ///< 6.2.3 20726
    CMSG_LF_GUILD_BROWSE                           = 0x11EF, ///< 6.2.3 20726
    CMSG_LF_GUILD_DECLINE_RECRUIT                  = 0x05C9, ///< 6.2.3 20726
    CMSG_LF_GUILD_GET_APPLICATIONS                 = 0x0181, ///< 6.2.3 20726
    CMSG_LF_GUILD_GET_RECRUITS                     = 0x08DE, ///< 6.2.3 20726
    CMSG_LF_GUILD_POST_REQUEST                     = 0x049D, ///< 6.2.3 20726
    CMSG_LF_GUILD_REMOVE_RECRUIT                   = 0x0189, ///< 6.2.3 20726
    CMSG_LF_GUILD_SET_GUILD_POST                   = 0x11B4, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Battle pet opcodes
    //////////////////////////////////////////////////////////////////////////
    CMSG_QUERY_BATTLE_PET_NAME                     = 0x0CE1,   ///< 6.2.3 20726

    CMSG_BATTLE_PETS_RECONVERT                     = 0x0000,   ///< Internal
    CMSG_BATTLE_PET_UPDATE_NOTIFY                  = 0x1085,   ///< 6.2.3 20726
    CMSG_BATTLE_PET_REQUEST_JOURNAL_LOCK           = 0x116C,   ///< 6.2.3 20726
    CMSG_BATTLE_PET_REQUEST_JOURNAL                = 0x0123,   ///< 6.2.3 20726
    CMSG_BATTLE_PET_DELETE_PET                     = 0x0168,   ///< 6.2.3 20726
    CMSG_BATTLE_PET_DELETE_PET_CHEAT               = 0x0600,   ///< 6.2.3 20726
    CMSG_BATTLE_PET_DELETE_JOURNAL                 = 0x0000,   ///< Internal
    CMSG_BATTLE_PET_MODIFY_NAME                    = 0x00E3,   ///< 6.2.3 20726
    CMSG_BATTLE_PET_SUMMON                         = 0x007B,   ///< 6.2.3 20726
    CMSG_BATTLE_PET_SET_LEVEL                      = 0x0000,   ///< Internal
    CMSG_BATTLE_PET_SET_BATTLE_SLOT                = 0x106C,   ///< 6.2.3 20726
    CMSG_BATTLE_PET_SET_COLLAR                     = 0x0000,   ///< Internal
    CMSG_BATTLE_PET_SET_FLAGS                      = 0x0538,   ///< 6.2.3 20726
    CMSG_RESTORE_BATTLE_PETS_HEALTH                = 0x0000,   ///< Internal
    CMSG_ADD_BATTLE_PET                            = 0x0000,   ///< Internal
    CMSG_CHEAT_BATTLE_PET_SET_QUALITY              = 0x0000,   ///< Internal
    CMSG_CAGE_BATTLE_PET                           = 0x10A2,   ///< 6.2.3 20726

    CMSG_JOIN_PET_BATTLE_QUEUE                     = 0x140D,   ///< 6.2.3 20726
    CMSG_LEAVE_PET_BATTLE_QUEUE                    = 0x04F6,   ///< 6.2.3 20726
    CMSG_PET_BATTLE_REQUEST_WILD                   = 0x05AE,   ///< 6.2.3 20726
    CMSG_PET_BATTLE_WILD_LOCATION_FAIL             = 0x0000,
    CMSG_PET_BATTLE_REQUEST_PVP                    = 0x1422,   ///< 6.2.3 20726
    CMSG_PET_BATTLE_REQUEST_UPDATE                 = 0x0CBE,   ///< 6.2.3 20726
    CMSG_PET_BATTLE_QUIT_NOTIFY                    = 0x1029,   ///< 6.2.3 20726
    CMSG_PET_BATTLE_FINAL_NOTIFY                   = 0x1025,   ///< 6.2.3 20726
    CMSG_PET_BATTLE_SCRIPT_ERROR_NOTIFY            = 0x0000,   ///< Internal
    CMSG_PET_BATTLE_QUEUE_PROPOSE_MATCH_RESULT     = 0x1C02,   ///< 6.2.3 20726
    CMSG_PET_BATTLE_INPUT_FIRST_PET                = 0x0000,   ///< Internal
    CMSG_PET_BATTLE_INPUT                          = 0x01FC,   ///< 6.2.3 20726
    CMSG_PET_BATTLE_REPLACE_FRONT_PET              = 0x1137,   ///< 6.2.3 20726
    CMSG_PET_BATTLE_DEBUG_QUEUE_DUMP               = 0x0000,   ///< Internal

    //////////////////////////////////////////////////////////////////////////
    /// Battle pay
    //////////////////////////////////////////////////////////////////////////
    CMSG_BATTLE_PAY_GET_PURCHASE_LIST              = 0x05E8, ///< 6.2.3 20726
    CMSG_BATTLE_PAY_GET_PRODUCT_LIST_QUERY         = 0x00F0, ///< 6.2.3 20726
    CMSG_BATTLE_PAY_START_PURCHASE                 = 0x0430, ///< 6.2.3 20726
    CMSG_BATTLE_PAY_CONFIRM_PURCHASE_RESPONSE      = 0x1424, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// LFG
    //////////////////////////////////////////////////////////////////////////
    CMSG_DFGET_SYSTEM_INFO                         = 0x1138, ///< 6.2.3 20726
    CMSG_LFG_GET_PLAYER_INFO                       = 0x0000,
    CMSG_LFG_GET_STATUS                            = 0x11A7, ///< 6.2.3 20726
    CMSG_LFG_JOIN                                  = 0x11F8, ///< 6.2.3 20726
    CMSG_LFG_LEAVE                                 = 0x0424, ///< 6.2.3 20726
    CMSG_LFG_PROPOSAL_RESULT                       = 0x11B0, ///< 6.2.3 20726
    CMSG_LFG_SET_BOOT_VOTE                         = 0x05AC, ///< 6.2.3 20726
    CMSG_LFG_SET_COMMENT                           = 0x0000, ///< (unused)
    CMSG_DFSET_ROLES                               = 0x003B, ///< 6.2.3 20726
    CMSG_LFG_TELEPORT                              = 0x00A8, ///< 6.2.3 20726
    CMSG_SEARCH_LFG_JOIN                           = 0x0000, ///< (unused)
    CMSG_SEARCH_LFG_LEAVE                          = 0x0000, ///< (unused)
    CMSG_RESET_INSTANCES                           = 0x04FF, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// LFG LIST
    //////////////////////////////////////////////////////////////////////////
    CMSG_REQUEST_LFG_LIST_BLACKLIST                 = 0x0D7A, ///< 6.2.3 20726
    CMSG_LFG_LIST_JOIN                              = 0x10A3, ///< 6.2.3 20726
    CMSG_LFG_LIST_UPDATE_REQUEST                    = 0x0167, ///< 6.2.3 20726
    CMSG_LFG_LIST_LEAVE                             = 0x00F3, ///< 6.2.3 20726
    CMSG_LFG_LIST_SEARCH                            = 0x0473, ///< 6.2.3 20726
    CMSG_LFG_LIST_APPLY_FOR_GROUP                   = 0x04F4, ///< 6.2.3 20726
    CMSG_LFG_LIST_INVITE_APPLICANT                  = 0x046F, ///< 6.2.3 20726
    CMSG_LFG_LIST_REMOVE_APPLICANT                  = 0x0470, ///< 6.2.3 20726
    CMSG_LFG_LIST_CANCEL_APPLICATION                = 0x05A7, ///< 6.2.3 20726
    CMSG_LFG_LIST_INVITATION_ANSWER                 = 0x053F, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Auction House
    //////////////////////////////////////////////////////////////////////////
    CMSG_AUCTION_HELLO                             = 0x1D0E, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Trainers
    //////////////////////////////////////////////////////////////////////////
    CMSG_TRAINER_LIST                              = 0x0735, ///< 6.2.3 20726
    CMSG_TRAINER_BUY_SPELL                         = 0x1D26, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Void storage
    //////////////////////////////////////////////////////////////////////////
    CMSG_VOID_STORAGE_QUERY                        = 0x08E5, ///< 6.2.3 20726
    CMSG_VOID_STORAGE_TRANSFER                     = 0x14A2, ///< 6.2.3 20726
    CMSG_VOID_STORAGE_UNLOCK                       = 0x0436, ///< 6.2.3 20726
    CMSG_VOID_SWAP_ITEM                            = 0x096A, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Transmogrification
    //////////////////////////////////////////////////////////////////////////
    CMSG_TRANSMOGRIFY_ITEMS                        = 0x09A1, ///< 6.2.0 19173

    //////////////////////////////////////////////////////////////////////////
    /// Mail
    //////////////////////////////////////////////////////////////////////////
    CMSG_GET_MAIL_LIST                             = 0x0859, ///< 6.2.3 20726
    CMSG_MAIL_CREATE_TEXT_ITEM                     = 0x19A9, ///< 6.2.3 20726
    CMSG_MAIL_DELETE                               = 0x0876, ///< 6.2.3 20726
    CMSG_MAIL_MARK_AS_READ                         = 0x0A1A, ///< 6.2.3 20726
    CMSG_MAIL_RETURN_TO_SENDER                     = 0x10C0, ///< 6.2.3 20726
    CMSG_MAIL_TAKE_ITEM                            = 0x0715, ///< 6.2.3 20726
    CMSG_MAIL_TAKE_MONEY                           = 0x0379, ///< 6.2.3 20726
    CMSG_SEND_MAIL                                 = 0x00C0, ///< 6.2.3 20726
    CMSG_QUERY_NEXT_MAIL_TIME                      = 0x1189, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Time
    //////////////////////////////////////////////////////////////////////////
    CMSG_UITIME_REQUEST                            = 0x0567, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// GameMasters
    //////////////////////////////////////////////////////////////////////////
    CMSG_REPORT_BUG                                 = 0x10E8, ///< 6.2.3 20726
    CMSG_REPORT_SUGGESTION                          = 0x11BC, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// ToyBox
    //////////////////////////////////////////////////////////////////////////
    CMSG_ADD_NEW_TOY_TO_BOX                         = 0x05E1, ///< 6.2.3 20726
    CMSG_SET_FAVORITE_TOY                           = 0x1068, ///< 6.2.3 20726
    CMSG_USE_TOY                                    = 0x057D, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Calendarr
    //////////////////////////////////////////////////////////////////////////
    CMSG_CALENDAR_ADD_EVENT                         = 0x00F8, ///< 6.2.3 20726
    CMSG_CALENDAR_COMPLAIN                          = 0x0180, ///< 6.2.3 20726
    CMSG_CALENDAR_COPY_EVENT                        = 0x01B7, ///< 6.2.3 20726
    CMSG_CALENDAR_EVENT_INVITE                      = 0x0438, ///< 6.2.3 20726
    CMSG_CALENDAR_EVENT_MODERATOR_STATUS            = 0x0130, ///< 6.2.3 20726
    CMSG_CALENDAR_EVENT_REMOVE_INVITE               = 0x1067, ///< 6.2.3 20726
    CMSG_CALENDAR_EVENT_RSVP                        = 0x1040, ///< 6.2.3 20726
    CMSG_CALENDAR_EVENT_SIGNUP                      = 0x01A8, ///< 6.2.3 20726
    CMSG_CALENDAR_EVENT_STATUS                      = 0x013C, ///< 6.2.3 20726
    CMSG_CALENDAR_GET_CALENDAR                      = 0x0477, ///< 6.2.3 20726
    CMSG_CALENDAR_GET_EVENT                         = 0x01BB, ///< 6.2.3 20726
    CMSG_CALENDAR_GET_NUM_PENDING                   = 0x00A7, ///< 6.2.3 20726
    CMSG_CALENDAR_GUILD_FILTER                      = 0x04A3, ///< 6.2.3 20726
    CMSG_CALENDAR_REMOVE_EVENT                      = 0x00E7, ///< 6.2.3 20726
    CMSG_CALENDAR_UPDATE_EVENT                      = 0x042C, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Challenges
    //////////////////////////////////////////////////////////////////////////
    CMSG_CHALLENGE_MODE_REQUEST_LEADERS             = 0x089E, ///< 6.2.3 20726
    CMSG_GET_CHALLENGE_MODE_REWARDS                 = 0x04D6, ///< 6.2.3 20726
    CMSG_CHALLENGE_MODE_REQUEST_MAP_STATS           = 0x00DA, ///< 6.2.3 20726
    CMSG_RESET_CHALLENGE_MODE                       = 0x0065, ///< 6.2.3 20779

    //////////////////////////////////////////////////////////////////////////
    /// BlackMarket
    //////////////////////////////////////////////////////////////////////////
    CMSG_BLACK_MARKET_OPEN                         = 0x021E, ///< 6.2.3 20726
    CMSG_BLACK_MARKET_REQUEST_ITEMS                = 0x0C59, ///< 6.2.3 20726
    CMSG_BLACK_MARKET_PLACE_BID                    = 0x031A, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    /// Twitter
    //////////////////////////////////////////////////////////////////////////
    CMSG_REQUEST_TWITTER_STATUS                     = 0x0916, ///< 6.2.3 20726

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    // CMSG
    CMSG_ACCEPT_LEVEL_GRANT                             = 0x0000,
    CMSG_ADD_VOICE_IGNORE                               = 0x0000,
    CMSG_ARENA_TEAM_ACCEPT                              = 0x0000,
    CMSG_ARENA_TEAM_CREATE                              = 0x0000,
    CMSG_ARENA_TEAM_DECLINE                             = 0x0000,
    CMSG_ARENA_TEAM_DISBAND                             = 0x0000,
    CMSG_ARENA_TEAM_INVITE                              = 0x0000,
    CMSG_ARENA_TEAM_LEADER                              = 0x0000,
    CMSG_ARENA_TEAM_LEAVE                               = 0x0000,
    CMSG_ARENA_TEAM_QUERY                               = 0x0000,
    CMSG_ARENA_TEAM_REMOVE                              = 0x0000,
    CMSG_ARENA_TEAM_ROSTER                              = 0x0000,
    CMSG_AUTOEQUIP_GROUND_ITEM                          = 0x0000,
    CMSG_AUTOEQUIP_ITEM_SLOT                            = 0x0000,
    CMSG_AUTOSTORE_GROUND_ITEM                          = 0x0000,
    CMSG_BOT_DETECTED2                                  = 0x0000,
    CMSG_CANCEL_TEMP_ENCHANTMENT                        = 0x0000,
    CMSG_CHANGEPLAYER_DIFFICULTY                        = 0x0000,
    CMSG_CHAT_FILTERED                                  = 0x0000,
    CMSG_CHAT_IGNORED                                   = 0x0000,
    CMSG_COMMENTATOR_ENABLE                             = 0x0000,
    CMSG_COMMENTATOR_ENTER_INSTANCE                     = 0x0000,
    CMSG_COMMENTATOR_EXIT_INSTANCE                      = 0x0000,
    CMSG_COMMENTATOR_GET_MAP_INFO                       = 0x0000,
    CMSG_COMMENTATOR_GET_PARTY_INFO                     = 0x0000,
    CMSG_COMMENTATOR_GET_PLAYER_INFO                    = 0x0000,
    CMSG_COMMENTATOR_INSTANCE_COMMAND                   = 0x0000,
    CMSG_COMMENTATOR_SKIRMISH_QUEUE_COMMAND             = 0x0000,
    CMSG_COMMENTATOR_START_WARGAME                      = 0x0000,
    CMSG_COMPLAIN                                       = 0x0133,
    CMSG_CONNECT_TO_FAILED                              = 0x0000,
    CMSG_DANCE_QUERY                                    = 0x0000,
    CMSG_DEL_VOICE_IGNORE                               = 0x0000,
    CMSG_DISMISS_CRITTER                                = 0x0000,
    CMSG_FORCE_MOVE_ROOT_ACK                            = 0x0000,
    CMSG_FORCE_MOVE_UNROOT_ACK                          = 0x0000,
    CMSG_GAMESPEED_SET                                  = 0x0000,
    CMSG_GAMETIME_SET                                   = 0x0000,
    CMSG_GETDEATHBINDZONE                               = 0x0000,
    CMSG_GHOST                                          = 0x0000,
    CMSG_GRANT_LEVEL                                    = 0x0000,
    CMSG_HEARTH_AND_RESURRECT                           = 0x0000,
    CMSG_INSTANCE_LOCK_WARNING_RESPONSE                 = 0x0000,
    CMSG_MANEUVER_START                                 = 0x0000,
    CMSG_MEETINGSTONE_INFO                              = 0x0000,
    CMSG_MINIGAME_MOVE                                  = 0x0000,
    CMSG_MOVE_CHARM_TELEPORT_CHEAT                      = 0x0000,
    CMSG_MOVE_ENABLE_SWIM_TO_FLY_TRANS_ACK              = 0x0000,
    CMSG_MOVE_NOT_ACTIVE_MOVER                          = 0x0000,
    CMSG_MOVE_SET_RELATIVE_POSITION                     = 0x0000,
    CMSG_MOVE_TOGGLE_COLLISION_ACK                      = 0x0000,
    CMSG_NEW_SPELL_SLOT                                 = 0x0000,
    CMSG_PLAYER_DIFFICULTY_CHANGE                       = 0x0000,
    CMSG_PLAYER_LOGOUT                                  = 0x0000,
    CMSG_PLAY_DANCE                                     = 0x0000,
    CMSG_PVP_LOG_DATA                                   = 0x0000,
    CMSG_QUERY_BATTLEFIELD_STATE                        = 0x0000,
    CMSG_QUERY_TIME                                     = 0x0000,
    CMSG_QUEST_NPC_QUERY                                = 0x0000,
    CMSG_REALM_SPLIT                                    = 0x0000,
    CMSG_REDIRECTION_AUTH_PROOF                         = 0x0000,
    CMSG_REQUEST_BATTLEPET_JOURNAL                      = 0x0000,
    CMSG_REQUEST_CATEGORY_COOLDOWNS                     = 0x0000,
    CMSG_REQUEST_PARTY_MEMBER_STATS                     = 0x0000,
    CMSG_REQUEST_RATED_BG_INFO                          = 0x0000,
    CMSG_REQUEST_RATED_BG_STATS                         = 0x0000,
    CMSG_RESET_FACTION_CHEAT                            = 0x0000,
    CMSG_SAVE_PLAYER                                    = 0x0000,
    CMSG_SEND_SOR_REQUEST_VIA_ADDRESS                   = 0x0000,
    CMSG_SEND_SOR_REQUEST_VIA_BNET_ACCOUNT_ID           = 0x0000,
    CMSG_SERVERTIME                                     = 0x0000,
    CMSG_SETDEATHBINDPOINT                              = 0x0000,
    CMSG_SET_ALLOW_LOW_LEVEL_RAID1                      = 0x0000,
    CMSG_SET_ALLOW_LOW_LEVEL_RAID2                      = 0x0000,
    CMSG_SET_CURRENCY_FLAGS                             = 0x0000,
    CMSG_SET_FACTION_CHEAT                              = 0x0000,
    CMSG_SET_LFG_COMMENT                                = 0x0000,
    CMSG_SET_PREFERED_CEMETERY                          = 0x0000,
    CMSG_SET_RELATIVE_POSITION                          = 0x0000,
    CMSG_SET_SAVED_INSTANCE_EXTEND                      = 0x0000,
    CMSG_SET_SKILL_CHEAT                                = 0x0000,
    CMSG_SHOW_ACCOUNT_ACHIEVEMENT                       = 0x0000,
    CMSG_START_QUEST                                    = 0x0000,
    CMSG_STOP_DANCE                                     = 0x0000,
    CMSG_SUBMIT_BUG                                     = 0x0000,
    CMSG_SUBMIT_COMPLAIN                                = 0x0000,
    CMSG_SUGGESTION_SUBMIT                              = 0x0000,
    CMSG_SUMMON_BATTLEPET_COMPANION                     = 0x0000,
    CMSG_SUSPEND_TOKEN                                  = 0x0000, ///<
    CMSG_SYNC_DANCE                                     = 0x0000,
    CMSG_TELEPORT_TO_UNIT                               = 0x0000,
    CMSG_TIME_ADJUSTMENT_RESPONSE                       = 0x0000, ///<
    CMSG_TIME_SYNC_RESP_FAILED                          = 0x0000, ///<
    CMSG_UNLEARN_SPECIALIZATION                         = 0x0000,
    CMSG_UNREGISTER_ALL_ADDON_PREFIXES                  = 0x0000,
    CMSG_UPDATE_MISSILE_TRAJECTORY                      = 0x0000,
    CMSG_UPGRADE_ITEM                                   = 0x0000,
    CMSG_USED_FOLLOW                                    = 0x0000,
    CMSG_VOICE_SESSION_ENABLE                           = 0x0000,
    CMSG_WARDEN_DATA                                    = 0x0000,
    CMSG_WARGAME_ACCEPT                                 = 0x0000,
    CMSG_WARGAME_START                                  = 0x0000,
    CMSG_WHOIS                                          = 0x0000,
    CMSG_WORLD_TELEPORT                                 = 0x0000,
    CMSG_WRAP_ITEM                                      = 0x0000,
    CMSG_ARENA_2v2_STATS_REQUEST                        = 0x0000,
    CMSG_ARENA_3v3_STATS_REQUEST                        = 0x0000,

    CMSG_GMSURVEY_SUBMIT                            = 0x0000, ///< RIP ???
    CMSG_GMTICKET_GET_CASE_STATUS                   = 0x0000, ///< RIP ????
    CMSG_GMTICKET_SYSTEMSTATUS                      = 0x0000, ///< RIP ????
    CMSG_GM_REPORT_LAG                              = 0x0000, ///< RIP ????

    // SMSG
    SMSG_ACCOUNT_PROFILE                                = 0x0000,
    SMSG_ADD_BATTLENET_FRIEND_RESPONSE                  = 0x0000,
    SMSG_AREA_SHARE_MAPPINGS_RESPONSE                   = 0x0000,
    SMSG_AREA_SHARE_INFO_RESPONSE                       = 0x0000,
    SMSG_BEASTMASTER_FAILED                             = 0x0000,
    SMSG_BONUS_ROLL_EMPTY                               = 0x0000,
    SMSG_CHALLENGE_MODE_DELETE_LEADER_RESULT            = 0x0000,
    SMSG_CLEAR_ITEM_CHALLENGE_MODE_DATA                 = 0x0000,
    SMSG_CONSOLE_WRITE                                  = 0x0000,
    SMSG_DISPLAY_PROMOTION                              = 0x0000,
    SMSG_DISPLAY_PLAYER_CHOICE                          = 0x0000,
    SMSG_FORCE_OBJECT_RELINK                            = 0x0000,
    SMSG_GHOST                                          = 0x0000,
    SMSG_GM_CHARACTER_SAVE_FAILURE                      = 0x0000,
    SMSG_GM_FREEZE_FAILURE                              = 0x0000,
    SMSG_GM_NAMED_POINTS                                = 0x0000,
    SMSG_GM_RESURRECT_FAILURE                           = 0x0000,
    SMSG_GM_REVEAL_TO                                   = 0x0000,
    SMSG_GM_SILENCE                                     = 0x0000,
    SMSG_IMMIGRANT_HOST_SEARCH_LOG                      = 0x0000,
    SMSG_LIST_TARGETS                                   = 0x0000,
    SMSG_MOVE_CHARACTER_CHEAT_FAILURE                   = 0x0000,
    SMSG_MOVE_CHARACTER_CHEAT_SUCCESS                   = 0x0000,
    SMSG_PENDING_RAID_LOCK                              = 0x0000, //
    SMSG_PLAYER_NOT_FOUND_FAILURE                       = 0x0000,
    SMSG_REFRESH_COMPONENT                              = 0x0000,
    SMSG_RESET_WEEKLY_CURRENCY                          = 0x0000,
    SMSG_SCENE_OBJECT_EVENT                             = 0x0000,
    SMSG_SCENE_OBJECT_PET_BATTLE_FINISHED               = 0x0000,
    SMSG_SCENE_OBJECT_PET_BATTLE_FIRST_ROUND            = 0x0000,
    SMSG_SCENE_OBJECT_PET_BATTLE_FINAL_ROUND            = 0x0000,
    SMSG_SCENE_OBJECT_PET_BATTLE_ROUND_RESULT           = 0x0000,
    SMSG_SCENE_OBJECT_PET_BATTLE_REMPLACEMENTS_MADE     = 0x0000,
    SMSG_SERVER_INFO_QUERY_RESPONSE                     = 0x0000,
    SMSG_PROFILE_DATA_RESPONSE                          = 0x0000,
    SMSG_SET_ITEM_CHALLENGE_MODE_DATA                   = 0x0000,
    SMSG_SET_PROMOTION_RESPONSE                         = 0x0000,
    SMSG_SET_SERVER_WOW_TIME                            = 0x0000,
    SMSG_SHOW_ZONES_CHEAT_RESULT                        = 0x0000,
    SMSG_UPDATE_EXPANSION_LEVEL                         = 0x0000,
    SMSG_ACCOUNT_INFO_RESPONSE                          = 0x0000,
    SMSG_AREA_TRIGGER_DENIED                            = 0x0000,
    SMSG_AREA_TRIGGER_MOVEMENT_UPDATE                   = 0x0000,
    SMSG_ARENA_ERROR                                    = 0x0000,
    SMSG_AVERAGE_ITEM_LEVEL_INFORM                      = 0x0000,
    SMSG_BATTLEGROUND_INFO_THROTTLED                    = 0x0000,
    SMSG_CAMERA_SHAKE                                   = 0x0000,
    SMSG_CHARACTER_LOGIN_FAILED                         = 0x0000,
    SMSG_CLEAR_BOSS_EMOTES                              = 0x0000,
    SMSG_COMBAT_EVENT_FAILED                            = 0x0000,
    SMSG_COMMENTATOR_MAP_INFO                           = 0x0000,
    SMSG_COMMENTATOR_PLAYER_INFO                        = 0x0000,
    SMSG_COMMENTATOR_STATE_CHANGED                      = 0x0000,
    SMSG_CUSTOM_LOAD_SCREEN                             = 0x0000,
    SMSG_DAMAGE_CALC_LOG                                = 0x0000,
    SMSG_DIFFERENT_INSTANCE_FROM_PARTY                  = 0x0000,
    SMSG_DISENCHANT_CREDIT                              = 0x0000,
    SMSG_DONT_AUTO_PUSH_SPELLS_TO_ACTION_BAR            = 0x0000,
    SMSG_DUMP_RIDE_TICKETS_RESPONSE                     = 0x0000,
    SMSG_FAILED_PLAYER_CONDITION                        = 0x0000,
    SMSG_GAMEOBJECT_RESET_STATE                         = 0x0000,
    SMSG_GAME_SPEED_SET                                 = 0x0000,
    SMSG_GAME_TIME_SET                                  = 0x0000,
    SMSG_GAME_TIME_UPDATE                               = 0x0000,
    SMSG_GAME_EVENT_DEBUG_LOG                           = 0x0000,
    SMSG_GM_PLAYER_INFO                                 = 0x0000,
    SMSG_GOD_MODE                                       = 0x0000,
    SMSG_GROUP_ACTION_THROTTLED                         = 0x0000,
    SMSG_HEALTH_UPDATE                                  = 0x0000,
    SMSG_INVALIDATE_PLAYER                              = 0x0000,
    SMSG_INVALID_PROMOTION_CODE                         = 0x0000,
    SMSG_ITEM_ADD_PASSIVE                               = 0x0000,
    SMSG_REMOVE_ITEM_PASSIVE                            = 0x0000,
    SMSG_SEND_ITEM_PASSIVES                             = 0x0000,
    SMSG_MAP_OBJ_EVENTS                                 = 0x0000,
    SMSG_MESSAGE_BOX                                    = 0x0000,
    SMSG_MISSILE_CANCEL                                 = 0x0000,
    SMSG_NOTIFY_MONEY                                   = 0x1BCC, ///< 6.2.3 20726
    SMSG_MOVE_SET_COMPOUND_STATE                        = 0x0000,
    SMSG_MOVE_UPDATE_COLLISION_HEIGHT                   = 0x0000,
    SMSG_ABORT_NEW_WORLD                                = 0x0000,
    SMSG_OPEN_CONTAINER                                 = 0x0000,
    SMSG_OPEN_LFG_DUNGEON_FINDER                        = 0x0000,
    SMSG_PAUSE_MIRROR_TIMER                             = 0x0000,
    SMSG_CHANGE_PLAYER_DIFFICULTY_RESULT                = 0x0000,
    SMSG_PLAYER_SKINNED                                 = 0x0000,
    SMSG_PLAY_TIME_WARNING                              = 0x0000,
    SMSG_PROPOSE_LEVEL_GRANT                            = 0x0000,
    SMSG_SUMMON_RAID_MEMBER_VALIDATE_FAILED             = 0x0000,
    SMSG_REALM_SPLIT                                    = 0x0000,
    SMSG_REFER_A_FRIEND_EXPIRED                         = 0x0000,
    SMSG_REFER_A_FRIEND_FAILURE                         = 0x0000,
    SMSG_REPORT_PVP_PLAYER_AFK_RESULT                   = 0x0000,
    SMSG_SERVER_TIME                                    = 0x0000,
    SMSG_SERVER_INFO_RESPONSE                           = 0x0000,
    SMSG_SERVER_PERF                                    = 0x0000,
    SMSG_SET_DF_FAST_LAUNCH_RESULT                      = 0x0000,
    SMSG_SHOW_RATINGS                                   = 0x0000,
    SMSG_SOR_START_EXPERIENCE_INCOMPLETE                = 0x0000,
    SMSG_STREAMING_MOVIES                               = 0x0000,
    SMSG_SUMMON_CANCEL                                  = 0x0000,
    SMSG_SUPPRESS_NPC_GREETINGS                         = 0x0000,
    SMSG_TIME_ADJUSTMENT                                = 0x0000,
    SMSG_UPDATE_SERVER_PLAYER_POSITION                  = 0x0000,
    SMSG_WAIT_QUEUE_FINISH                              = 0x0000,
    SMSG_WAIT_QUEUE_UPDATE                              = 0x0000,
    SMSG_WARGAME_REQUEST_SUCCESSFULLY_SENT_TO_OPPONENT  = 0x0000,
    SMSG_CHEAT_PLAYER_LOOKUP                            = 0x0000,
    SMSG_DBLOOKUP_RESULTS                               = 0x0000,
    SMSG_DEBUG_AISTATE                                  = 0x0000,
    SMSG_DYNAMIC_DROP_ROLL_RESULT                       = 0x0000,
    SMSG_FORCE_ACTION_SHOW_RESPONSE                     = 0x0000,
    SMSG_FORCE_ANIM                                     = 0x0000,
    SMSG_GHOSTEE_GONE                                   = 0x0000,
    SMSG_KICK_REASON                                    = 0x0000,
    SMSG_RESET_RANGED_COMBAT_TIMER                      = 0x0000,
    SMSG_SCRIPT_MESSAGE                                 = 0x0000,
    SMSG_SERVER_BUCK_DATA                               = 0x0000,
    SMSG_SERVER_BUCK_DATA_START                         = 0x0000,
    SMSG_SHOW_MAILBOX                                   = 0x0000,
};

/// Player state
enum SessionStatus
{
    STATUS_AUTHED = 0,                                          // Player authenticated (_player == NULL, m_playerRecentlyLogout = false or will be reset before handler call, m_GUID have garbage)
    STATUS_LOGGEDIN,                                            // Player in game (_player != NULL, m_GUID == _player->GetGUID(), inWorld())
    STATUS_TRANSFER,                                            // Player transferring to another map (_player != NULL, m_GUID == _player->GetGUID(), !inWorld())
    STATUS_LOGGEDIN_OR_RECENTLY_LOGGOUT,                        // _player != NULL or _player == NULL && m_playerRecentlyLogout && m_playerLogout, m_GUID store last _player guid)
    STATUS_NEVER,                                               // Opcode not accepted from client (deprecated or server side only)
    STATUS_UNHANDLED                                            // Opcode not handled yet
};

enum PacketProcessing
{
    PROCESS_INPLACE = 0,                                        // process packet whenever we receive it - mostly for non-handled or non-implemented packets
    PROCESS_THREADUNSAFE,                                       // packet is not thread-safe - process it in World::UpdateSessions()
    PROCESS_THREADSAFE                                          // packet is thread-safe - process it in Map::Update()
};

enum IRPacketProcessing
 {
     PROCESS_LOCAL           = 0,                            // Never send to interrealm
     PROCESS_DISTANT_IF_NEED = 1,                            // Send to interrealm if needed (player is on bg)
 };

class WorldPacket;
class WorldSession;

typedef void(WorldSession::*g_OpcodeHandlerType)(WorldPacket& recvPacket);

struct OpcodeHandler
{
    OpcodeHandler() {}
    OpcodeHandler(char const* _name, SessionStatus _status, PacketProcessing _processing, g_OpcodeHandlerType _handler, IRPacketProcessing _forwardToIR)
        : name(_name), status(_status), packetProcessing(_processing), handler(_handler), forwardToIR(_forwardToIR) {}

    char const* name;
    SessionStatus status;
    PacketProcessing packetProcessing;
    g_OpcodeHandlerType handler;
	IRPacketProcessing forwardToIR;
};

extern OpcodeHandler* g_OpcodeTable[TRANSFER_DIRECTION_MAX][NUM_OPCODE_HANDLERS];
void InitOpcodes();

// Lookup opcode name for human understandable logging
inline std::string GetOpcodeNameForLogging(uint16 id, int p_Direction)
{
    uint32 opcode = uint32(id);
    std::ostringstream ss;
    ss << '[';

    if (id < UNKNOWN_OPCODE)
    {
        OpcodeHandler* handler = g_OpcodeTable[p_Direction][uint32(id) & 0x7FFF];

        if (!handler)
            ss << "UNKNOWN OPCODE";
        else
            ss << handler->name;
    }
    else
        ss << "INVALID OPCODE";

    ss << " 0x" << std::hex << std::uppercase << opcode << std::nouppercase << " (" << std::dec << opcode << ")]";
    return ss.str();
}

inline bool CanBeSentDuringInterRealm(uint16 id)
{
    switch (id)
    {
        case SMSG_CHAT:
        case SMSG_CONTACT_LIST:
        case SMSG_GUILD_BANK_QUERY_RESULTS:
        case SMSG_GUILD_COMMAND_RESULT:
        case SMSG_GUILD_INVITE:
        case SMSG_QUERY_GUILD_INFO_RESPONSE:
        case SMSG_GUILD_ROSTER:
            return true;
    }

    return false;
}

#endif
/// @}
