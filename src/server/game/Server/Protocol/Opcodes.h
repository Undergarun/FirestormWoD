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
 * You should have received a copy of the GNU General Public License alonge
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
    NUM_OPCODE_HANDLERS                               = (0x7FFF+1),
    UNKNOWN_OPCODE                                    = (0xFFFF+1),
    NULL_OPCODE                                       = 0,
    COMPRESSED_OPCODE_MASK                            = 0x8000,

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Protocol
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamProtocol
        /// Opcodes
        SMSG_AUTH_CHALLENGE                         = 0x007E, ///< 6.1.2 19783
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Dispatch
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamDispatch
        /// Base opcodes
        SMSG_AUTH_RESPONSE                          = 0x18F6, ///< 6.1.2 19783
        SMSG_CACHE_VERSION                          = 0x0E09, ///< 6.1.2 19783
        SMSG_ADDON_INFO                             = 0x1715, ///< 6.1.2 19783
        SMSG_FEATURE_SYSTEM_STATUS                  = 0x0B3E, ///< 6.1.2 19783
        SMSG_SET_TIME_ZONE_INFORMATION              = 0x073A, ///< 6.1.2 19783
        SMSG_LOGIN_VERIFY_WORLD                     = 0x0A98, ///< 6.1.2 19783
        SMSG_NEW_WORLD                              = 0x0A15, ///< 6.1.2 19783
        SMSG_LOGIN_SET_TIME_SPEED                   = 0x0D65, ///< 6.1.2 19783
        SMSG_HOTFIX_NOTIFY_BLOB                     = 0x1D71, ///< 6.1.2 19783
        SMSG_HOTFIX_NOTIFY                          = 0x1736, ///< 6.1.2 19783
        SMSG_WORLD_SERVER_INFO                      = 0x0FB5, ///< 6.1.2 19783
        SMSG_INITIAL_SETUP                          = 0x0238, ///< 6.1.2 19783
        SMSG_PONG                                   = 0x005E, ///< 6.1.2 19783
        SMSG_UITIME                                 = 0x0DA1, ///< 6.1.2 19783

        /// Query
        SMSG_DB_REPLY                               = 0x09A5, ///< 6.1.2 19783
        SMSG_QUERY_CREATURE_RESPONSE                = 0x1A15, ///< 6.1.2 19783
        SMSG_NPC_TEXT_UPDATE                        = 0x071E, ///< 6.1.2 19783
        SMSG_NAME_QUERY_RESPONSE                    = 0x0C71, ///< 6.1.2 19783
        SMSG_REALM_QUERY_RESPONSE                   = 0x0DF2, ///< 6.1.2 19783
        SMSG_GAMEOBJECT_QUERY_RESPONSE              = 0x1345, ///< 6.1.2 19783
        SMSG_QUERY_TIME_RESPONSE                    = 0x0CF1, ///< 6.1.2 19783

        /// Misc
        SMSG_UPDATE_ACTION_BUTTONS                  = 0x03C0, ///< 6.1.2 19783
        SMSG_SET_PROFICIENCY                        = 0x0D75, ///< 6.1.2 19783
        SMSG_INIT_WORLD_STATES                      = 0x09E1, ///< 6.1.2 19783
        SMSG_UPDATE_WORLD_STATE                     = 0x1DF1, ///< 6.1.2 19783
        SMSG_EMOTE                                  = 0x0FC0, ///< 6.1.2 19783
        SMSG_EXPLORATION_EXPERIENCE                 = 0x0692, ///< 6.1.2 19783
        SMSG_LOG_XP_GAIN                            = 0x0E3E, ///< 6.1.2 19783
        SMSG_LEVELUP_INFO                           = 0x0B36, ///< 6.1.2 19783
        SMSG_ITEM_PUSH_RESULT                       = 0x0B15, ///< 6.1.2 19783
        SMSG_INIT_CURRENCY                          = 0x1CE2, ///< 6.1.2 19783
        SMSG_UPDATE_CURRENCY                        = 0x0336, ///< 6.1.2 19783
        SMSG_UPDATE_CURRENCY_WEEK_LIMIT             = 0x0D31, ///< 6.1.2 19783
        SMSG_WEEKLY_RESET_CURRENCY                  = 0x18F2, ///< 6.1.2 19783
        SMSG_EQUIPMENT_SET_LIST                     = 0x079E, ///< 6.1.2 19783
        SMSG_BINDER_CONFIRM                         = 0x19E5, ///< 6.1.2 19783
        SMSG_BIND_POINT_UPDATE                      = 0x0399, ///< 6.1.2 19783
        SMSG_UPDATE_TALENT_DATA                     = 0x1862, ///< 6.1.2 19783
        SMSG_TRANSFER_ABORTED                       = 0x1D31, ///< 6.1.2 19783
        SMSG_TRANSFER_PENDING                       = 0x03B8, ///< 6.1.2 19783
        SMSG_INVENTORY_CHANGE_FAILURE               = 0x07B5, ///< 6.1.2 19783
        SMSG_SET_DUNGEON_DIFFICULTY                 = 0x19F2, ///< 6.1.2 19783
        SMSG_SET_RAID_DIFFICULTY                    = 0x0E8B, ///< 6.1.2 19783
        SMSG_LOAD_CUF_PROFILES                      = 0x139A, ///< 6.1.2 19783
        SMSG_STANDSTATE_UPDATE                      = 0x0B37, ///< 6.1.2 19783
        SMSG_START_TIMER                            = 0x1A2D, ///< 6.1.2 19783
        SMSG_START_ELAPSED_TIMER                    = 0x0E98, ///< 6.1.2 19783
        SMSG_START_ELAPSED_TIMERS                   = 0x063D, ///< (unused)
        SMSG_STOP_ELAPSED_TIMER                     = 0x0795, ///< 6.1.2 19783
        SMSG_DEATH_RELEASE_LOC                      = 0x0A17, ///< 6.1.2 19783
        SMSG_CORPSE_RECLAIM_DELAY                   = 0x02BA, ///< 6.1.2 19783
        SMSG_CORPSE_LOCATION                        = 0x0ABF, ///< 6.1.2 19783
        SMSG_CORPSE_TRANSPORT_QUERY                 = 0x1E2E, ///< 6.1.2 19783
        SMSG_DURABILITY_DAMAGE_DEATH                = 0x1936, ///< 6.1.2 19783
        SMSG_PLAY_MUSIC                             = 0x09F1, ///< 6.1.2 19783
        SMSG_PLAY_OBJECT_SOUND                      = 0x16BF, ///< 6.1.2 19783
        SMSG_PLAY_SOUND                             = 0x1298, ///< 6.1.2 19783
        SMSG_DISPLAY_TOAST                          = 0x1CF1, ///< 6.1.2 19783
        SMSG_ITEM_ENCHANT_TIME_UPDATE               = 0x1E30, ///< 6.1.2 19783
        SMSG_ENCHANTMENT_LOG                        = 0x0DA6, ///< 6.1.2 19783
        SMSG_PARTY_KILL_LOG                         = 0x120F, ///< 6.1.2 19783
        SMSG_PROCRESIST                             = 0x0611, ///< 6.1.2 19783
        SMSG_CLEAR_TARGET                           = 0x1DF5, ///< 6.1.2 19783
        SMSG_WEATHER                                = 0x0397, ///< 6.1.2 19783
        SMSG_SET_PHASE_SHIFT                        = 0x18A6, ///< 6.1.2 19783
        SMSG_TRIGGER_CINEMATIC                      = 0x1CF6, ///< 6.1.2 19783
        SMSG_TOTEM_CREATED                          = 0x0BB7, ///< 6.1.2 19783
        SMSG_RESPEC_WIPE_CONFIRM                    = 0x1630, ///< 6.1.2 19783
        SMSG_USE_EQUIPMENT_SET_RESULT               = 0x07B2, ///< 6.1.2 19783
        SMSG_TRADE_STATUS                           = 0x071A, ///< 6.1.2 19783
        SMSG_TRADE_UPDATED                          = 0x123F, ///< 6.1.2 19783
        SMSG_SHOW_NEUTRAL_PLAYER_FACTION_SELECT_UI  = 0x0331, ///< 6.1.2 19783
        SMSG_NEUTRAL_PLAYER_FACTION_SELECT_RESULT   = 0x0000, ///< (unused)
        SMSG_INSPECT_TALENT                         = 0x1D22, ///< 6.1.2 19783
        SMSG_INSPECT_HONOR_STATS                    = 0x1740, ///< 6.1.2 19783
        SMSG_INSPECT_RATED_BG_STATS                 = 0x12B9, ///< 6.1.2 19783
        SMSG_TIME_SYNC_REQUEST                      = 0x1E23, ///< 6.1.2 19783
        SMSG_RESUME_TOKEN                           = 0x07BF, ///< 6.1.2 19783
        SMSG_BARBER_SHOP_RESULT                     = 0x1866, ///< 6.1.2 19783
        SMSG_ENABLE_BARBER_SHOP                     = 0x17B1, ///< 6.1.2 19783
        SMSG_PAGE_TEXT_QUERY_RESPONSE               = 0x048E, ///< 6.1.2 19783
        SMSG_PAGE_TEXT                              = 0x0E03, ///< 6.1.2 19783
        SMSG_READ_ITEM_FAILED                       = 0x1617, ///< 6.1.2 19783
        SMSG_READ_ITEM_OK                           = 0x131E, ///< 6.1.2 19783
        SMSG_TEXT_EMOTE                             = 0x0696, ///< 6.1.2 19783
        SMSG_TITLE_EARNED                           = 0x1E03, ///< 6.1.2 19783
        SMSG_TITLE_LOST                             = 0x03B2, ///< 6.1.2 19783
        SMSG_WHOIS                                  = 0x0F20, ///< 6.1.2 19783
        SMSG_TRIGGER_MOVIE                          = 0x163A, ///< 6.1.2 19783
        SMSG_UPDATE_LAST_INSTANCE                   = 0x0971, ///< 6.1.2 19783
        SMSG_UPDATE_INSTANCE_OWNERSHIP              = 0x12BF, ///< 6.1.2 19783
        SMSG_SUMMON_REQUEST                         = 0x1CE1, ///< 6.1.2 19783
        SMSG_STOP_MIRROR_TIMER                      = 0x0BB6, ///< 6.1.2 19783
        SMSG_START_MIRROR_TIMER                     = 0x0861, ///< 6.1.2 19783
        SMSG_SOCKET_GEMS                            = 0x1DF6, ///< 6.1.2 19783
        SMSG_RESURRECT_REQUEST                      = 0x0CF2, ///< 6.1.2 19783
        SMSG_RESPOND_INSPECT_ACHIEVEMENTS           = 0x1A2E, ///< 6.1.2 19783
        SMSG_RESET_FAILED_NOTIFY                    = 0x040D, ///< 6.1.2 19783
        SMSG_SETUP_RESEARCH_HISTORY                 = 0x071F, ///< 6.1.2 19783
        SMSG_RESEARCH_COMPLETE                      = 0x1335, ///< 6.1.2 19783
        SMSG_REQUEST_CEMETERY_LIST_RESPONSE         = 0x1398, ///< 6.1.2 19783
        SMSG_PLAYED_TIME                            = 0x1875, ///< 6.1.2 19783
        SMSG_BREAK_TARGET                           = 0x0E02, ///< 6.1.2 19783
        SMSG_CANCEL_AUTO_REPEAT                     = 0x1931, ///< 6.1.2 19783
        SMSG_CANCEL_COMBAT                          = 0x0220, ///< 6.1.2 19783
        SMSG_COMPLAIN_RESULT                        = 0x1C71, ///< 6.1.2 19783
        SMSG_XPGAIN_ABORTED                         = 0x19F5, ///< (unused)
        SMSG_SUPERCEDED_SPELL                       = 0x15EA, ///< 6.1.2 19783
        SMSG_PVP_CREDIT                             = 0x0B18, ///< 6.1.2 19783
        SMSG_PRE_RESURRECT                          = 0x058D, ///< 6.1.2 19783
        SMSG_PLAY_ONE_SHOT_ANIM_KIT                 = 0x0FB6, ///< 6.1.2 19783
        SMSG_SET_AI_ANIM_KIT                        = 0x1295, ///< 6.1.2 19783
        SMSG_SET_MELEE_ANIM_KIT                     = 0x0BB5, ///< 6.1.2 19783
        SMSG_SET_ANIM_TIER                          = 0x0317, ///< 6.1.2 19865
        SMSG_SET_PLAY_HOVER_ANIM                    = 0x0F9E, ///< 6.1.2 19865
        SMSG_SET_MOVEMENT_ANIM_KIT                  = 0x0D25, ///< 6.1.2 19783
        SMSG_PLAYER_BOUND                           = 0x1935, ///< 6.1.2 19783
        SMSG_OVERRIDE_LIGHT                         = 0x0936, ///< 6.1.2 19783
        SMSG_PRINT_NOTIFICATION                     = 0x0F3D, ///< 6.1.2 19783
        SMSG_SPECIAL_MOUNT_ANIM                     = 0x1319, ///< 6.1.2 19783
        SMSG_ITEM_TIME_UPDATE                       = 0x1DB5, ///< 6.1.2 19783
        SMSG_QUERY_ITEM_TEXT_RESPONSE               = 0x1D36, ///< 6.1.2 19783
        SMSG_ITEM_PURCHASE_REFUND_RESULT            = 0x1620, ///< 6.1.2 19783
        SMSG_GAMEOBJECT_DESPAWN                     = 0x0E0A, ///< 6.1.2 19783
        SMSG_GAMEOBJECT_CUSTOM_ANIM                 = 0x0797, ///< 6.1.2 19783
        SMSG_GAME_OBJECT_ACTIVATE_ANIM_KIT          = 0x0DA5, ///< 6.1.2 19783
        SMSG_FISH_NOT_HOOKED                        = 0x0231, ///< 6.1.2 19783
        SMSG_FISH_ESCAPED                           = 0x0A3E, ///< 6.1.2 19783
        SMSG_DESTRUCTIBLE_BUILDING_DAMAGE           = 0x08B5, ///< 6.1.2 19783
        SMSG_CROSSED_INEBRIATION_THRESHOLD          = 0x133A, ///< 6.1.2 19783
        SMSG_CLIENT_CONTROL_UPDATE                  = 0x0D32, ///< 6.1.2 19783
        SMSG_AREA_TRIGGER_NO_CORPSE                 = 0x0297, ///< 6.1.2 19783
        SMSG_BAG_SORT_RESULT                        = 0x050E, ///< 6.1.2 19783
        SMSG_VIGNETTE_UPDATE                        = 0x1691, ///< 6.1.2 19783
        SMSG_ACCOUNT_MOUNT_UPDATE                   = 0x079D, ///< 6.1.2 19783
        SMSG_PLAY_SCENE                             = 0x09B1, ///< 6.1.2 19783
        SMSG_CANCEL_SCENE                           = 0x0AB8, ///< 6.1.2 19783
        SMSG_TALENTS_INVOLUNTARILY_RESET            = 0x01E3, ///< 6.1.2 19783
        SMSG_SET_ITEM_PURCHASE_DATA                 = 0x179E, ///< 6.1.2 19783
        SMSG_SHOW_TRADE_SKILL_RESPONSE              = 0x1E0D, ///< 6.1.2 19783

        /// Control Alert
        SMSG_LOSS_OF_CONTROL_AURA_UPDATE            = 0x0737, ///< 6.1.2 19783
        SMSG_ADD_LOSS_OF_CONTROL                    = 0x19E2, ///< (not seen on retail)
        SMSG_CLEAR_LOSS_OF_CONTROL                  = 0x0636, ///< (not seen on retail)
        SMSG_REMOVE_LOSS_OF_CONTROL                 = 0x0821, ///< (not seen on retail)

        /// Reputations
        SMSG_INITIALIZE_FACTIONS                    = 0x1C32, ///< 6.1.2 19783
        SMSG_FACTION_BONUS_INFO                     = 0x0E89, ///< 6.1.2 19783
        SMSG_SET_FACTION_VISIBLE                    = 0x18A5, ///< 6.1.2 19783
        SMSG_SET_FACTION_NOT_VISIBLE                = 0x1217, ///< 6.1.2 19783
        SMSG_SET_FACTION_STANDING                   = 0x1210, ///< 6.1.2 19783
        SMSG_SET_FACTION_ATWAR                      = 0x0F9D, ///< 6.1.2 19783
        SMSG_SET_FORCED_REACTIONS                   = 0x07BA, ///< 6.1.2 19783

        /// Interaction
        SMSG_LOGOUT_RESPONSE                        = 0x0731, ///< 6.1.2 19783
        SMSG_LOGOUT_CANCEL_ACK                      = 0x08B2, ///< 6.1.2 19783
        SMSG_LOGOUT_COMPLETE                        = 0x0E95, ///< 6.1.2 19783
        SMSG_GOSSIP_POI                             = 0x1CA1, ///< 6.1.2 19783
        SMSG_ARCHAEOLOGY_SURVERY_CAST               = 0x160F, ///< 6.1.2 19783

        /// World Object management
        SMSG_UPDATE_OBJECT                          = 0x1CB2, ///< 6.1.2 19783
        SMSG_POWER_UPDATE                           = 0x0F96, ///< 6.1.2 19783
        SMSG_ADJUST_SPLINE_DURATION                 = 0x0E97, ///< 6.1.2 19865

        /// Character list
        SMSG_ENUM_CHARACTERS_RESULT                 = 0x18F1, ///< 6.1.2 19783
        SMSG_CREATE_CHAR                            = 0x16BA, ///< 6.1.2 19783
        SMSG_CHAR_DELETE                            = 0x06B8, ///< 6.1.2 19783
        SMSG_SET_PLAYER_DECLINED_NAMES_RESULT       = 0x0CA2, ///< 6.1.2 19783
        SMSG_CHAR_FACTION_CHANGE                    = 0x0F8A, ///< 6.1.2 19783
        SMSG_CHAR_RENAME                            = 0x1F81, ///< 6.1.2 19783
        SMSG_CHAR_CUSTOMIZE_RESULT                  = 0x121A, ///< 6.1.2 19783
        SMSG_CHAR_CUSTOMIZE                         = 0x03B9, ///< 6.1.2 19783

        /// Account data
        SMSG_ACCOUNT_DATA_TIMES                     = 0x16B8, ///< 6.1.2 19783
        SMSG_UPDATE_ACCOUNT_DATA                    = 0x1698, ///< 6.1.2 19783
        SMSG_TUTORIAL_FLAGS                         = 0x0E82, ///< 6.1.2 19783

        /// Combat
        SMSG_ATTACKER_STATE_UPDATE                  = 0x0EBD, ///< 6.1.2 19783
        SMSG_ATTACK_START                           = 0x1971, ///< 6.1.2 19783
        SMSG_ATTACK_STOP                            = 0x17C0, ///< 6.1.2 19783
        SMSG_AI_REACTION                            = 0x1739, ///< 6.1.2 19783
        SMSG_ATTACK_SWING_ERROR                     = 0x1D66, ///< 6.1.2 19783

        /// Duel
        SMSG_DUEL_REQUESTED                         = 0x0BBF, ///< 6.1.2 19783
        SMSG_DUEL_COUNTDOWN                         = 0x0318, ///< 6.1.2 19783
        SMSG_DUEL_COMPLETE                          = 0x1E83, ///< 6.1.2 19783
        SMSG_DUEL_INBOUNDS                          = 0x0796, ///< 6.1.2 19783
        SMSG_DUEL_OUT_OF_BOUNDS                     = 0x07B1, ///< 6.1.2 19783
        SMSG_DUEL_WINNER                            = 0x1299, ///< 6.1.2 19783
        SMSG_CAN_DUEL_RESULT                        = 0x1831, ///< (unused)

        /// Vendor
        SMSG_LIST_INVENTORY                         = 0x0E40, ///< 6.1.2 19783
        SMSG_BUY_FAILED                             = 0x1337, ///< 6.1.2 19783
        SMSG_BUY_ITEM                               = 0x1320, ///< 6.1.2 19783
        SMSG_SELL_ITEM                              = 0x133E, ///< 6.1.2 19783

        /// Achievement
        SMSG_ALL_ACHIEVEMENT_DATA                   = 0x163D, ///< 6.1.2 19783
        SMSG_ACHIEVEMENT_DELETED                    = 0x1CF2, ///< 6.1.2 19783
        SMSG_ACHIEVEMENT_EARNED                     = 0x06C0, ///< 6.1.2 19783
        SMSG_ACCOUNT_CRITERIA_UPDATE                = 0x1635, ///< 6.1.2 19783
        SMSG_ACCOUNT_CRITERIA_UPDATE_ALL            = 0x0392, ///< 6.1.2 19783
        SMSG_CRITERIA_DELETED                       = 0x1E2F, ///< 6.1.2 19783
        SMSG_CRITERIA_UPDATE                        = 0x0716, ///< 6.1.2 19783

        /// Friends
        SMSG_CONTACT_LIST                           = 0x039F, ///< 6.1.2 19783
        SMSG_FRIEND_STATUS                          = 0x0F03, ///< 6.1.2 19783

        /// Taxi
        SMSG_TAXI_NODE_STATUS                       = 0x0EBF, ///< 6.1.2 19783
        SMSG_NEW_TAXI_PATH                          = 0x080F, ///< 6.1.2 19783
        SMSG_SHOW_TAXI_NODES                        = 0x0FB8, ///< 6.1.2 19783
        SMSG_ACTIVATE_TAXI_REPLY                    = 0x0C61, ///< 6.1.2 19783

        /// Loot
        SMSG_LOOT_LIST                              = 0x08E2, ///< 6.1.2 19783
        SMSG_LOOT_RESPONSE                          = 0x122D, ///< 6.1.2 19783
        SMSG_LOOT_MONEY_NOTIFY                      = 0x17A0, ///< 6.1.2 19783
        SMSG_LOOT_RELEASE                           = 0x06B7, ///< 6.1.2 19783
        SMSG_LOOT_RELEASE_ALL                       = 0x1C61, ///< 6.1.2 19783
        SMSG_LOOT_REMOVED                           = 0x0F15, ///< 6.1.2 19783
        SMSG_COIN_REMOVED                           = 0x069D, ///< 6.1.2 19783
        SMSG_LOOT_ALL_PASSED                        = 0x0C26, ///< 6.1.2 19783
        SMSG_LOOT_CONTENTS                          = 0x0000, ///< (unused)
        SMSG_LOOT_ITEM_LIST                         = 0x0000, ///< (unused)
        SMSG_MASTER_LOOT_CANDIDATE_LIST             = 0x1821, ///< 6.1.2 19783
        SMSG_LOOT_ROLL                              = 0x09B2, ///< 6.1.2 19783
        SMSG_LOOT_ROLLS_COMPLETE                    = 0x0000, ///< (unused)
        SMSG_LOOT_ROLL_WON                          = 0x1296, ///< 6.1.2 19783
        SMSG_LOOT_START_ROLL                        = 0x0831, ///< 6.1.2 19783
        SMSG_AE_LOOT_TARGETS                        = 0x0000, ///< (unused)
        SMSG_AE_LOOT_TARGETS_ACK                    = 0x0000, ///< (unused)
        SMSG_ITEM_BONUS_DEBUG                       = 0x1CB6, ///< 6.1.2 19783

        /// Bank
        SMSG_SHOW_BANK                              = 0x179F, ///< 6.1.2 19783

        /// Rune
        SMSG_RESYNC_RUNES                           = 0x06BD, ///< 6.1.2 19783
        SMSG_DEBUG_RUNE_REGEN                       = 0x0732, ///< 6.1.2 19783
        SMSG_ADD_RUNE_POWER                         = 0x1718, ///< 6.1.2 19783
        SMSG_CONVERT_RUNE                           = 0x052E, ///< 6.1.2 19783

        /// Group
        SMSG_PARTY_UPDATE                           = 0x0F02, ///< 6.1.2 19783
        SMSG_PARTY_MEMBER_STATE_FULL                = 0x0335, ///< 6.1.2 19783
        SMSG_PARTY_MEMBER_STATE_PARTIAL             = 0x0A16, ///< 6.1.2 19783
        SMSG_PARTY_INVITE                           = 0x0E83, ///< 6.1.2 19783
        SMSG_GROUP_DECLINE                          = 0x139F, ///< 6.1.2 19783
        SMSG_PARTY_COMMAND_RESULT                   = 0x13B5, ///< 6.1.2 19783
        SMSG_GROUP_UNINVITE                         = 0x1318, ///< 6.1.2 19783
        SMSG_GROUP_NEW_LEADER                       = 0x0C32, ///< 6.1.2 19783
        SMSG_MINIMAP_PING                           = 0x1697, ///< 6.1.2 19783
        SMSG_SET_LOOT_METHOD_FAILED                 = 0x09B6, ///< 6.1.2 19783
        SMSG_RANDOM_ROLL                            = 0x1240, ///< 6.1.2 19783
        SMSG_SEND_RAID_TARGET_UPDATE_SINGLE         = 0x1F0C, ///< 6.1.2 19783
        SMSG_SEND_RAID_TARGET_UPDATE_ALL            = 0x1F84, ///< 6.1.2 19783
        SMSG_READY_CHECK_COMPLETED                  = 0x0B35, ///< 6.1.2 19783
        SMSG_READY_CHECK_RESPONSE                   = 0x0DF6, ///< 6.1.2 19783
        SMSG_READY_CHECK_STARTED                    = 0x1618, ///< 6.1.2 19783
        SMSG_ROLL_POLL_INFORM                       = 0x0EC0, ///< 6.1.2 19783
        SMSG_ROLE_CHANGED_INFORM                    = 0x0F09, ///< 6.1.2 19783
        SMSG_GROUP_DESTROYED                        = 0x1D75, ///< 6.1.2 19783

        /// Battleground
        SMSG_BATTLEFIELD_STATUS_QUEUED                      = 0x163F, ///< 6.1.2 19783
        SMSG_BATTLEFIELD_STATUS_NONE                        = 0x07B7, ///< 6.1.2 19783
        SMSG_BATTLEFIELD_STATUS_NEED_CONFIRMATION           = 0x1E8A, ///< 6.1.2 19783
        SMSG_BATTLEFIELD_STATUS_ACTIVE                      = 0x0B38, ///< 6.1.2 19783
        SMSG_BATTLEFIELD_STATUS_FAILED                      = 0x0715, ///< 6.1.2 19783
        SMSG_BATTLEFIELD_STATUS_WAIT_FOR_GROUPS             = 0x0791, ///< 6.1.2 19783
        SMSG_BATTLEFIELD_PORT_DENIED                        = 0x0000, ///< (unused)
        SMSG_RATED_BATTLEFIELD_INFO                         = 0x0311, ///< 6.1.2 19783
        SMSG_BATTLEGROUND_PLAYER_JOINED                     = 0x1C26, ///< 6.1.2 19783
        SMSG_BATTLEGROUND_PLAYER_LEFT                       = 0x0AA0, ///< 6.1.2 19783
        SMSG_BATTLEGROUND_PLAYER_POSITIONS                  = 0x0976, ///< 6.1.2 19783
        SMSG_BATTLEFIELD_MGR_ENTERING                       = 0x0299, ///< 6.1.2 19783
        SMSG_BATTLEFIELD_MGR_QUEUE_REQUEST_RESPONSE         = 0x1631, ///< 6.1.2 19783
        SMSG_BATTLEFIELD_MGR_EJECT_PENDING                  = 0x0000, ///< (unused)
        SMSG_BATTLEFIELD_MGR_EJECTED                        = 0x0F95, ///< 6.1.2 19783
        SMSG_BATTLEFIELD_MGR_QUEUE_INVITE                   = 0x121E, ///< 6.1.2 19783
        SMSG_BATTLEFIELD_MGR_STATE_CHANGED                  = 0x0000, ///< (unused)
        SMSG_BFMGR_ENTRY_INVITE                             = 0x18B1, ///< 6.1.2 19783
        SMSG_BATTLEFIELD_MGR_EXIT_REQUEST                   = 0x0000,
        SMSG_REQUEST_PVP_REWARDS_RESPONSE                   = 0x0218, ///< 6.1.2 19783
        SMSG_PVP_OPTIONS_ENABLED                            = 0x1E0F, ///< 6.1.2 19783
        SMSG_PVPLOG_DATA                                    = 0x063E, ///< 6.1.2 19783
        SMSG_ARENA_OPPONENT_SPECIALIZATIONS                 = 0x1876, ///< 6.1.2 19783
        SMSG_DESTROY_ARENA_UNIT                             = 0x0000, ///< (unused)
        SMSG_BATTLEGROUND_POINTS                            = 0x0000, ///< (unused)
        SMSG_BATTLEGROUND_INIT                              = 0x0000, ///< (unused)
        SMSG_MAP_OBJECTIVES_INIT                            = 0x0000, ///< (unused)
        SMSG_CONQUEST_FORMULA_CONSTANTS                     = 0x1C75, ///< 6.1.2 19783
        SMSG_PVP_SEASON                                     = 0x1D61, ///< 6.1.2 19783
        SMSG_WARGAME_REQUEST_SUCCESSFULLY_SENT_TO_OPPENENT  = 0x0000, ///< (unused)
        SMSG_SPIRIT_HEALER_CONFIRM                          = 0x1737, ///< 6.1.2 19783
        SMSG_AREA_SPIRIT_HEALER_TIME                        = 0x02B7, ///< 6.1.2 19783
        SMSG_BATTLEFIELD_LIST                               = 0x0338, ///< 6.1.2 19783
        SMSG_CHECK_WARGAME_ENTRY                            = 0x129F, ///< 6.1.2 19783

        /// Pet
        SMSG_PET_NAME_QUERY_RESPONSE                        = 0x023D, ///< 6.1.2 19783
        SMSG_PET_NAME_INVALID                               = 0x0975, ///< 6.1.2 19783
        SMSG_PET_DISMISS_SOUND                              = 0x0237, ///< 6.1.2 19783
        SMSG_PET_GUIDS                                      = 0x0245, ///< 6.1.2 19783
        SMSG_PET_MODE                                       = 0x0000, ///< (unused)
        SMSG_PET_SLOT_UPDATED                               = 0x0000, ///< (unused)
        SMSG_PET_STABLE_LIST                                = 0x0D36, ///< 6.1.2 19783
        SMSG_PET_ACTION_FEEDBACK                            = 0x1692, ///< 6.1.2 19783
        SMSG_PET_ACTION_SOUND                               = 0x0875, ///< 6.1.2 19783
        SMSG_PET_ADDED                                      = 0x0000, ///< (unused)
        SMSG_SET_PET_SPECIALIZATION                         = 0x0E0C, ///< 6.1.2 19783
        SMSG_STABLE_RESULT                                  = 0x0E0C, ///< 6.1.2 19783

        /// Pet Battle opcodes
        SMSG_BATTLE_PET_JOURNAL                             = 0x1C35, ///< 6.1.2 19783
        SMSG_QUERY_PET_NAME_RESPONSE                        = 0x023D, ///< 6.1.2 19783
        SMSG_PETBATTLE_REQUEST_FAILED                       = 0x0000,
        SMSG_PETBATTLE_FIRST_ROUND                          = 0x0000,
        SMSG_PETBATTLE_FULL_UPDATE                          = 0x0000,
        SMSG_PETBATTLE_FINALIZE_LOCATION                    = 0x0000,
        SMSG_PETBATTLE_PVP_CHALLENGE                        = 0x0000,
        SMSG_PETBATTLE_ROUND_RESULT                         = 0x0000,
        SMSG_PETBATTLE_REPLACEMENTS_MADE                    = 0x0000,
        SMSG_PETBATTLE_FINAL_ROUND                          = 0x0000,
        SMSG_PETBATTLE_FINISHED                             = 0x0000,
        SMSG_PET_BATTLE_SLOT_UPDATES                        = 0x1D21, ///< 6.1.2 19783

        /// Instances
        SMSG_INSTANCE_ENCOUNTER_ENGAGE_UNIT                     = 0x08E6, ///< 6.1.2 19783
        SMSG_INSTANCE_ENCOUNTER_DISENGAGE_UNIT                  = 0x1DB6, ///< 6.1.2 19783
        SMSG_INSTANCE_ENCOUNTER_CHANGE_PRIORITY                 = 0x0C72, ///< 6.1.2 19783
        SMSG_INSTANCE_ENCOUNTER_TIMER_START                     = 0x0619, ///< 6.1.2 19783
        SMSG_INSTANCE_ENCOUNTER_OBJECTIVE_START                 = 0x0B3D, ///< 6.1.2 19783
        SMSG_INSTANCE_ENCOUNTER_OBJECTIVE_COMPLETE              = 0x0735, ///< 6.1.2 19783
        SMSG_INSTANCE_ENCOUNTER_START                           = 0x06B9, ///< 6.1.2 19783
        SMSG_INSTANCE_ENCOUNTER_OBJECTIVE_UPDATE                = 0x1219, ///< 6.1.2 19783
        SMSG_INSTANCE_ENCOUNTER_END                             = 0x16B5, ///< 6.1.2 19783
        SMSG_INSTANCE_ENCOUNTER_IN_COMBAT_RESURRECTION          = 0x1717, ///< 6.1.2 19783
        SMSG_INSTANCE_ENCOUNTER_PHASE_SHIFT_CHANGED             = 0x0F89, ///< 6.1.2 19783
        SMSG_INSTANCE_ENCOUNTER_GAIN_COMBAT_RESURRECTION_CHARGE = 0x0866, ///< 6.1.2 19783
        SMSG_INSTANCE_RESET                                     = 0x0825, ///< 6.1.2 19783
        SMSG_INSTANCE_RESET_FAILED                              = 0x0F40, ///< 6.1.2 19783
        SMSG_RAID_INSTANCE_INFO                                 = 0x12B5, ///< 6.1.2 19783
        SMSG_RAID_GROUP_ONLY                                    = 0x1832, ///< 6.1.2 19783
        SMSG_RAID_MARKERS_CHANGED                               = 0x18F5, ///< 6.1.2 19783
        SMSG_INSTANCE_SAVE_CREATED                              = 0x0DA2, ///< 6.1.2 19783
        SMSG_INSTANCE_GROUP_SIZE_CHANGED                        = 0x1C22, ///< 6.1.2 19783
        SMSG_ENCOUNTER_END                                      = 0x0862, ///< 6.1.2 19783
        SMSG_ENCOUNTER_START                                    = 0x171A, ///< 6.1.2 19783
        SMSG_BOSS_KILL_CREDIT                                   = 0x0395, ///< 6.1.2 19783

        /// Scenarios
        SMSG_SCENARIO_POI                                       = 0x0CB2, ///< 6.1.2 19783
        SMSG_SCENARIO_PROGRESS_UPDATE                           = 0x1F89, ///< 6.1.2 19783
        SMSG_SCENARIO_STATE                                     = 0x0E16, ///< 6.1.2 19783

        /// Auction House
        SMSG_AUCTION_HELLO_RESPONSE                             = 0x1338, ///< 6.1.2 19783
        SMSG_AUCTION_BIDDER_LIST_RESULT                         = 0x02B9, ///< 6.1.2 19783
        SMSG_AUCTION_BIDDER_NOTIFICATION                        = 0x073E, ///< 6.1.2 19783
        SMSG_AUCTION_BUYOUT_NOTIFICATION                        = 0x161A, ///< 6.1.2 19783
        SMSG_AUCTION_COMMAND_RESULT                             = 0x13B6, ///< 6.1.2 19783
        SMSG_AUCTION_CLOSED_NOTIFICATION                        = 0x0EA0, ///< 6.1.2 19783
        SMSG_AUCTION_LIST_PENDING_SALES                         = 0x0E1F, ///< 6.1.2 19783
        SMSG_AUCTION_LIST_RESULT                                = 0x0E01, ///< 6.1.2 19783
        SMSG_AUCTION_OWNER_LIST_RESULT                          = 0x0FA0, ///< 6.1.2 19783
        SMSG_AUCTION_OWNER_BID_NOTIFICATION                     = 0x0612, ///< 6.1.2 19783

        /// Mail
        SMSG_SEND_MAIL_RESULT                                   = 0x17BF, ///< 6.1.2 19783
        SMSG_MAIL_LIST_RESULT                                   = 0x0871, ///< 6.1.2 19783
        SMSG_RECEIVED_MAIL                                      = 0x18E2, ///< 6.1.2 19783
        SMSG_MAIL_QUERY_NEXT_TIME_RESULT                        = 0x0F8C, ///< 6.1.2 19783

        /// Trainers
        SMSG_TRAINER_LIST                                       = 0x17B2, ///< 6.1.2 19783
        SMSG_TRAINER_SERVICE                                    = 0x0875, ///< 6.1.2 19783

        /// Void Storage
        SMSG_VOID_ITEM_SWAP_RESPONSE                            = 0x08A5, ///< 6.1.2 19783
        SMSG_VOID_STORAGE_CONTENTS                              = 0x0F82, ///< 6.1.2 19783
        SMSG_VOID_STORAGE_FAILED                                = 0x1962, ///< 6.1.2 19783
        SMSG_VOID_STORAGE_TRANSFER_CHANGES                      = 0x162F, ///< 6.1.2 19783
        SMSG_VOID_TRANSFER_RESULT                               = 0x0D66, ///< 6.1.2 19783

        /// Petition
        SMSG_PETITION_ALREADY_SIGNED                            = 0x0F18, ///< 6.1.2 19783
        SMSG_PETITION_SIGN_RESULTS                              = 0x1C26, ///< 6.1.2 19783
        SMSG_QUERY_PETITION_RESPONSE                            = 0x1872, ///< 6.1.2 19783
        SMSG_PETITION_SHOW_LIST                                 = 0x0B1F, ///< 6.1.2 19783
        SMSG_PETITION_SHOW_SIGNATURES                           = 0x1CA2, ///< 6.1.2 19783
        SMSG_TURN_IN_PETITION_RESULTS                           = 0x0A45, ///< 6.1.2 19783
        SMSG_PETITION_DECLINED                                  = 0x0000, ///< fake opcode

        /// Threat
        SMSG_THREAT_CLEAR                                       = 0x0F1D, ///< 6.1.2 19783
        SMSG_THREAT_REMOVE                                      = 0x0BBE, ///< 6.1.2 19783
        SMSG_THREAT_UPDATE                                      = 0x0AC0, ///< 6.1.2 19783
        SMSG_HIGHEST_THREAT_UPDATE                              = 0x0F35, ///< 6.1.2 19783

        /// Tickets
        SMSG_GM_TICKET_GET_TICKET_RESPONSE                      = 0x0B95, ///< 6.1.2 19783
        SMSG_GM_TICKET_RESPONSE                                 = 0x07B6, ///< 6.1.2 19783
        SMSG_GM_TICKET_RESPONSE_ERROR                           = 0x08A2, ///< 6.1.2 19783
        SMSG_GM_TICKET_RESOLVE_RESPONSE                         = 0x0A37, ///< 6.1.2 19783
        SMSG_GM_TICKET_SYSTEM_STATUS                            = 0x0B16, ///< 6.1.2 19783
        SMSG_GM_TICKET_UPDATE                                   = 0x0925, ///< 6.1.2 19783
        SMSG_GM_TICKET_STATUS_UPDATE                            = 0x13B7, ///< 6.1.2 19783
        SMSG_GM_TICKET_CASE_STATUS                              = 0x17B7, ///< 6.1.2 19783

        /// Calendar
        SMSG_CALENDAR_CLEAR_PENDING_ACTION                      = 0x0932, ///< 6.1.2 19783
        SMSG_CALENDAR_COMMAND_RESULT                            = 0x0CE5, ///< 6.1.2 19783
        SMSG_CALENDAR_EVENT_INITIAL_INVITE                      = 0x1F82, ///< 6.1.2 19783
        SMSG_CALENDAR_EVENT_INVITE                              = 0x0298, ///< 6.1.2 19783
        SMSG_CALENDAR_EVENT_INVITE_ALERT                        = 0x1797, ///< 6.1.2 19783
        SMSG_CALENDAR_EVENT_INVITE_NOTES                        = 0x0F01, ///< 6.1.2 19783
        SMSG_CALENDAR_EVENT_INVITE_NOTES_ALERT                  = 0x1922, ///< 6.1.2 19783
        SMSG_CALENDAR_EVENT_INVITE_REMOVED                      = 0x09A6, ///< 6.1.2 19783
        SMSG_CALENDAR_EVENT_INVITE_REMOVED_ALERT                = 0x0A20, ///< 6.1.2 19783
        SMSG_CALENDAR_EVENT_INVITE_STATUS                       = 0x1637, ///< 6.1.2 19783
        SMSG_CALENDAR_EVENT_INVITE_STATUS_ALERT                 = 0x0738, ///< 6.1.2 19783
        SMSG_CALENDAR_EVENT_MODERATOR_STATUS                    = 0x0835, ///< 6.1.2 19783
        SMSG_CALENDAR_EVENT_REMOVED_ALERT                       = 0x061D, ///< 6.1.2 19783
        SMSG_CALENDAR_EVENT_UPDATED_ALERT                       = 0x12B7, ///< 6.1.2 19783
        SMSG_CALENDAR_RAID_LOCKOUT_ADDED                        = 0x05AE, ///< 6.1.2 19783
        SMSG_CALENDAR_RAID_LOCKOUT_REMOVED                      = 0x07B9, ///< 6.1.2 19783
        SMSG_CALENDAR_RAID_LOCKOUT_UPDATED                      = 0x0CF5, ///< 6.1.2 19783
        SMSG_CALENDAR_SEND_CALENDAR                             = 0x0A1F, ///< 6.1.2 19783
        SMSG_CALENDAR_SEND_EVENT                                = 0x17BE, ///< 6.1.2 19783
        SMSG_CALENDAR_SEND_NUM_PENDING                          = 0x129E, ///< 6.1.2 19783

        /// Warden
        SMSG_WARDEN_DATA                                        = 0x0BEC, ///< 6.1.2 19783

        /// Challenges
        SMSG_CHALLENGE_MODE_REQUEST_LEADERS_RESULT              = 0x1976, ///< 6.1.2 19783
        SMSG_CHALLENGE_MODE_MAP_STATS_UPDATE                    = 0x07C0, ///< 6.1.2 19783
        SMSG_CHALLENGE_MODE_NEW_PLAYER_RECORD                   = 0x1232, ///< 6.1.2 19783
        SMSG_CHALLENGE_MODE_ALL_MAP_STATS                       = 0x0D22, ///< 6.1.2 19783
        SMSG_CHALLENGE_MODE_START                               = 0x0000, ///< This opcode is wrong and not existing
        SMSG_CHALLENGE_MODE_COMPLETE                            = 0x03B1, ///< 6.1.2 19783
        SMSG_CHALLENGE_MOD_REWARDS                              = 0x0000, ///< (NYI)

        /// Battlepay
        SMSG_BATTLE_PAY_GET_PRODUCT_LIST_RESPONSE               = 0x0CE1, ///< 6.1.2 19783
        SMSG_BATTLE_PAY_GET_PURCHASE_LIST_RESPONSE              = 0x02C0, ///< 6.1.2 19783
        SMSG_BATTLE_PAY_GET_DISTRIBUTION_LIST_RESPONSE          = 0x120E, ///< 6.1.2 19783
        SMSG_BATTLE_PAY_START_PURCHASE_RESPONSE                 = 0x0F04, ///< 6.1.2 19783
        SMSG_BATTLE_PAY_PURCHASE_UPDATE                         = 0x1639, ///< 6.1.2 19783
        SMSG_BATTLE_PAY_CONFIRM_PURCHASE                        = 0x1716, ///< 6.1.2 19783

        SMSG_BLACK_MARKET_OPEN_RESULT                           = 0x18B5, ///< 6.1.2 19783
        SMSG_BLACK_MARKET_OUTBID                                = 0x18E6, ///< 6.1.2 19783
        SMSG_BLACK_MARKET_REQUEST_ITEMS_RESULT                  = 0x06B6, ///< 6.1.2 19783
        SMSG_BLACK_MARKET_BID_ON_ITEM_RESULT                    = 0x0DB2, ///< 6.1.2 19783
        SMSG_BLACK_MARKET_WON                                   = 0x1A45, ///< 6.1.2 19783

        /// Garrison
        SMSG_OPEN_SHIPMENT_NPCFROM_GOSSIP                       = 0x1712, ///< 6.1.2 19783
        SMSG_GET_SHIPMENT_INFO_RESPONSE                         = 0x1D35, ///< 6.1.2 19783
        SMSG_GET_SHIPMENTS                                      = 0x1CA5, ///< 6.1.2 19783
        SMSG_CREATE_SHIPMENT_RESPONSE                           = 0x0D61, ///< 6.1.2 19783

        /// Twitter
        SMSG_REQUEST_TWITTER_STATUS_RESPONSE                    = 0x0000,
        SMSG_OAUTH_SAVED_DATA                                   = 0x0CBB, ///< 6.1.2 19783

        /// Toys
        SMSG_ACCOUNT_TOYS_UPDATE                                = 0x0E1D, ///< 6.1.2 19783

        /// AreaTriggers
        SMSG_AREA_TRIGGER_RE_SHAPE                              = 0x1A0E, ///< 6.1.2 19865
        SMSG_AREA_TRIGGER_RE_PATH                               = 0x1F09, ///< 6.1.2 19865
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Guild
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamGuild
        /// Petition
        SMSG_PETITION_RENAME                        = 0x1484, ///< 6.1.2 19783

        /// Guild Finder
        SMSG_LFGUILD_BROWSE                         = 0x1197, ///< 6.1.2 19783
        SMSG_LFGUILD_APPLICATIONS                   = 0x11BF, ///< 6.1.2 19783
        SMSG_LFGUILD_APPLICATIONS_LIST_CHANGED      = 0x118C, ///< 6.1.2 19783
        SMSG_LFGUILD_COMMAND_RESULT                 = 0x0000, ///< (unused)
        SMSG_LFGUILD_RECRUITS                       = 0x11BB, ///< 6.1.2 19783
        SMSG_LFGUILD_POST                           = 0x10B0, ///< 6.1.2 19783
        SMSG_LFGUILD_APPLICANT_LIST_CHANGED         = 0x118C, ///< 6.1.2 19783

        /// Bank
        SMSG_GUILD_RANKS                            = 0x14B0, ///< 6.1.2 19783
        SMSG_GUILD_BANK_LOG_QUERY_RESULT            = 0x10AB, ///< 6.1.2 19783
        SMSG_GUILD_BANK_REMAINING_WITHDRAW_MONEY    = 0x1588, ///< 6.1.2 19783
        SMSG_GUILD_BANK_QUERY_TEXT_RESULT           = 0x10B8, ///< 6.1.2 19783
        SMSG_GUILD_BANK_QUERY_RESULTS               = 0x1083, ///< 6.1.2 19783
        SMSG_GUILD_PERMISSIONS_QUERY_RESULTS        = 0x1097, ///< 6.1.2 19783

        /// Achievement
        SMSG_GUILD_ACHIEVEMENT_DELETED              = 0x11AB, ///< 6.1.2 19783
        SMSG_GUILD_ACHIEVEMENT_MEMBERS              = 0x0000, ///< (unused)
        SMSG_GUILD_ACHIEVEMENT_EARNED               = 0x06C0, ///< 6.1.2 19783
        SMSG_GUILD_CRITERIA_UPDATE                  = 0x0000, ///< (unused)
        SMSG_GUILD_CRITERIA_DELETED                 = 0x118F, ///< 6.1.2 19783
        SMSG_ALL_GUILD_ACHIEVEMENTS                 = 0x14AB, ///< 6.1.2 19783

        /// Info
        SMSG_GUILD_ROSTER                           = 0x1498, ///< 6.1.2 19783
        SMSG_GUILD_ROSTER_UPDATE                    = 0x0000, ///< (unused)
        SMSG_QUERY_GUILD_INFO_RESPONSE              = 0x1194, ///< 6.1.2 19783
        SMSG_GUILD_MEMBER_UPDATE_NOTE               = 0x0000, ///< (unused)
        SMSG_GUILD_REWARDS_LIST                     = 0x1494, ///< 6.1.2 19783
        SMSG_GUILD_SEND_RANK_CHANGE                 = 0x10B7, ///< 6.1.2 19783
        SMSG_GUILD_CHALLENGE_UPDATED                = 0x1183, ///< 6.1.2 19783
        SMSG_GUILD_MEMBER_RECIPES                   = 0x0000, ///< (unused)
        SMSG_GUILD_INVITE_EXPIRED                   = 0x0000, ///< (unused)
        SMSG_GUILD_COMMAND_RESULT                   = 0x11A0, ///< 6.1.2 19783
        SMSG_GUILD_CHALLENGE_COMPLETED              = 0x14AF, ///< 6.1.2 19802
        SMSG_GUILD_REPUTATION_REACTION_CHANGED      = 0x149F, ///< 6.1.2 19802
        SMSG_GUILD_KNOWN_RECIPES                    = 0x11A3, ///< 6.1.2 19783
        SMSG_GUILD_MEMBER_DAILY_RESET               = 0x148C, ///< 6.1.2 19783
        SMSG_GUILD_FLAGGED_FOR_RENAME               = 0x0000, ///< (unused)
        SMSG_GUILD_NAME_CHANGED                     = 0x0000, ///< (unused)
        SMSG_GUILD_RESET                            = 0x0000, ///< (unused)
        SMSG_GUILD_CHANGE_NAME_RESULT               = 0x0000, ///< (unused)
        SMSG_GUILD_INVITE                           = 0x11C0, ///< 6.1.2 19783
        SMSG_GUILD_INVITE_DECLINED                  = 0x0000, ///< (unused)
        SMSG_GUILD_PARTY_STATE                      = 0x1094, ///< 6.1.2 19783
        SMSG_GUILD_MEMBERS_WITH_RECIPE              = 0x0000, ///< (unused)
        SMSG_GUILD_MOVE_STARTING                    = 0x0000, ///< (unused)
        SMSG_GUILD_MOVED                            = 0x0000, ///< (unused)
        SMSG_GUILD_NEWS                             = 0x10A4, ///< 6.1.2 19783
        SMSG_GUILD_NEWS_DELETED                     = 0x0000, ///< (unused)
        SMSG_PLAYER_TABAR_VENDOR_SHOW               = 0x139E, ///< 6.1.2 19783
        SMSG_PLAYER_SAVE_GUILD_EMBLEM               = 0x148B, ///< 6.1.2 19783

        /// Event system
        SMSG_GUILD_EVENT_NEW_LEADER                 = 0x1587, ///< 6.1.2 19783
        SMSG_GUILD_EVENT_PRESENCE_CHANGE            = 0x10AF, ///< 6.1.2 19783
        SMSG_GUILD_EVENT_PLAYER_JOINED              = 0x1198, ///< 6.1.2 19783
        SMSG_GUILD_EVENT_PLAYER_LEFT                = 0x109B, ///< 6.1.2 19783
        SMSG_GUILD_EVENT_RANKS_UPDATED              = 0x11BC, ///< 6.1.2 19783
        SMSG_GUILD_EVENT_RANK_CHANGED               = 0x10BC, ///< 6.1.2 19783
        SMSG_GUILD_EVENT_LOG_QUERY_RESULTS          = 0x119B, ///< 6.1.2 19783
        SMSG_GUILD_EVENT_MOTD                       = 0x10BF, ///< 6.1.2 19783
        SMSG_GUILD_EVENT_DISBANDED                  = 0x14A3, ///< 6.1.2 19783
        SMSG_GUILD_EVENT_TAB_ADDED                  = 0x1084, ///< 6.1.2 19783
        SMSG_GUILD_EVENT_TAB_MODIFIED               = 0x14A8, ///< 6.1.2 19783
        SMSG_GUILD_EVENT_TAB_DELETED                = 0x1190, ///< 6.1.2 19783
        SMSG_GUILD_EVENT_TAB_TEXT_CHANGED           = 0x1497, ///< 6.1.2 19783
        SMSG_GUILD_EVENT_BANK_CONTENTS_CHANGED      = 0x11AF, ///< 6.1.2 19783
        SMSG_GUILD_EVENT_BANK_MONEY_CHANGED         = 0x10C0, ///< 6.1.2 19783
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
        SMSG_SERVER_FIRST_ACHIEVEMENT               = 0x11D0, ///< 6.1.2 19783
        SMSG_RAID_INSTANCE_MESSAGE                  = 0x15CB, ///< 6.1.2 19783
        SMSG_MOTD                                   = 0x18E8, ///< 6.1.2 19783
        SMSG_EXPECTED_SPAM_RECORDS                  = 0x0000, ///< (unused)
        SMSG_DEFENSE_MESSAGE                        = 0x0000, ///< (unused)
        SMSG_ZONE_UNDER_ATTACK                      = 0x14CF, ///< 6.1.2 19783
        SMSG_WHO                                    = 0x11CC, ///< 6.1.2 19783

        /// Chat
        SMSG_CHAT_SERVER_MESSAGE                    = 0x14E3, ///< 6.1.2 19783
        SMSG_CHAT_RESTRICTED                        = 0x19E8, ///< 6.1.2 19783
        SMSG_CHAT_RECONNECT                         = 0x0000, ///< (unused)
        SMSG_CHAT_PLAYER_NOTFOUND                   = 0x19D0, ///< 6.1.2 19783
        SMSG_CHAT_PLAYER_AMBIGUOUS                  = 0x18E3, ///< 6.1.2 19783
        SMSG_CHAT_NOT_IN_PARTY                      = 0x0000, ///< (unused)
        SMSG_CHAT_IS_DOWN                           = 0x0000, ///< (unused)
        SMSG_CHAT_IGNORED_ACCOUNT_MUTED             = 0x0000, ///< (unused)
        SMSG_CHAT_DOWN                              = 0x0000, ///< (unused)
        SMSG_CHAT_AUTO_RESPONDED                    = 0x0000, ///< (unused)
        SMSG_CHAT                                   = 0x11E7, ///< 6.1.2 19783

        /// Channel
        SMSG_CHANNEL_NOTIFY_LEFT                    = 0x19CF, ///< 6.1.2 19783
        SMSG_CHANNEL_NOTIFY_JOINED                  = 0x14C3, ///< 6.1.2 19783
        SMSG_CHANNEL_NOTIFY                         = 0x15EF, ///< 6.1.2 19783
        SMSG_CHANNEL_LIST                           = 0x14CC, ///< 6.1.2 19783
        SMSG_USERLIST_ADD                           = 0x15F0, ///< 6.1.2 19783
        SMSG_USERLIST_REMOVE                        = 0x10E4, ///< 6.1.2 19783
        SMSG_USERLIST_UPDATE                        = 0x18EF, ///< 6.1.2 19783
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Move
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamMove
        /// Move speeds
        SMSG_MOVE_SET_RUN_SPEED                     = 0x1EA9, ///< 6.1.2 19783
        SMSG_MOVE_SET_RUN_BACK_SPEED                = 0x03DA, ///< 6.1.2 19783
        SMSG_MOVE_SET_SWIM_SPEED                    = 0x1628, ///< 6.1.2 19783
        SMSG_MOVE_SET_SWIM_BACK_SPEED               = 0x0605, ///< 6.1.2 19783
        SMSG_MOVE_SET_FLIGHT_SPEED                  = 0x046D, ///< 6.1.2 19783
        SMSG_MOVE_SET_FLIGHT_BACK_SPEED             = 0x030D, ///< 6.1.2 19783
        SMSG_MOVE_SET_WALK_SPEED                    = 0x0F28, ///< 6.1.2 19783
        SMSG_MOVE_SET_TURN_RATE                     = 0x1E24, ///< 6.1.2 19783
        SMSG_MOVE_SET_PITCH_RATE                    = 0x0B2C, ///< 6.1.2 19783
        SMSG_MOVE_KNOCK_BACK                        = 0x1BA3, ///< 6.1.2 19783
        SMSG_MOVE_UPDATE_RUN_SPEED                  = 0x06DA, ///< 6.1.2 19783
        SMSG_MOVE_UPDATE_RUN_BACK_SPEED             = 0x1B82, ///< 6.1.2 19783
        SMSG_MOVE_UPDATE_SWIM_SPEED                 = 0x1A83, ///< 6.1.2 19783
        SMSG_MOVE_UPDATE_SWIM_BACK_SPEED            = 0x0B06, ///< 6.1.2 19783
        SMSG_MOVE_UPDATE_FLIGHT_SPEED               = 0x0628, ///< 6.1.2 19783
        SMSG_MOVE_UPDATE_FLIGHT_BACK_SPEED          = 0x032E, ///< 6.1.2 19783
        SMSG_MOVE_UPDATE_WALK_SPEED                 = 0x1F29, ///< 6.1.2 19783
        SMSG_MOVE_UPDATE_TURN_RATE                  = 0x000A, ///< 6.1.2 19783
        SMSG_MOVE_UPDATE_PITCH_RATE                 = 0x1AAC, ///< 6.1.2 19783
        SMSG_MOVE_UPDATE_KNOCK_BACK                 = 0x0273, ///< 6.1.2 19783

        /// Player
        SMSG_MOVE_UPDATE                                    = 0x0F2C, ///< 6.1.2 19783
        SMSG_MOVE_TELEPORT                                  = 0x1206, ///< 6.1.2 19783
        SMSG_MOVE_UPDATE_TELEPORT                           = 0x1F21, ///< 6.1.2 19783
        SMSG_MOVE_ROOT                                      = 0x018A, ///< 6.1.2 19783
        SMSG_MOVE_UNROOT                                    = 0x046E, ///< 6.1.2 19783
        SMSG_MOVE_WATER_WALK                                = 0x035C, ///< 6.1.2 19783
        SMSG_MOVE_LAND_WALK                                 = 0x0AC7, ///< 6.1.2 19783
        SMSG_MOVE_FEATHER_FALL                              = 0x0674, ///< 6.1.2 19783
        SMSG_MOVE_NORMAL_FALL                               = 0x04EE, ///< 6.1.2 19783
        SMSG_MOVE_SET_ACTIVE_MOVER                          = 0x025B, ///< 6.1.2 19783
        SMSG_MOVE_SET_CAN_FLY                               = 0x012A, ///< 6.1.2 19783
        SMSG_MOVE_UNSET_CAN_FLY                             = 0x03DC, ///< 6.1.2 19783
        SMSG_MOVE_SET_HOVER                                 = 0x0B21, ///< 6.1.2 19783
        SMSG_MOVE_UNSET_HOVER                               = 0x0651, ///< 6.1.2 19783
        SMSG_MOVE_SET_CAN_TURN_WHILE_FALLING                = 0x0EA1, ///< 6.1.2 19783
        SMSG_MOVE_UNSET_CAN_TURN_WHILE_FALLING              = 0x124F, ///< 6.1.2 19783
        SMSG_MOVE_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY   = 0x0BCF, ///< 6.1.2 19783
        SMSG_MOVE_UNSET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY = 0x1EAC, ///< 6.1.2 19783
        SMSG_MOVE_SET_VEHICLE_REC_ID                        = 0x1A29, ///< 6.1.2 19783
        SMSG_MOVE_SET_COLLISION_HEIGHT                      = 0x1BA4, ///< 6.1.2 19783
        SMSG_APPLY_MOVEMENT_FORCE                           = 0x1F50, ///< 6.1.2 19783
        SMSG_UNAPPLY_MOVEMENT_FORCE                         = 0x000E, ///< 6.1.2 19783
        SMSG_MOVE_SKIP_TIME                                 = 0x0000, ///< (unused)
        SMSG_MOVE_ENABLE_GRAVITY                            = 0x0000, ///< (unused)
        SMSG_MOVE_DISABLE_GRAVITY                           = 0x0000, ///< (unused)
        SMSG_MOVE_ENABLE_COLLISION                          = 0x0000, ///< (unused)
        SMSG_MOVE_DISABLE_COLLISION                         = 0x0000, ///< (unused)

        /// Creature
        SMSG_MONSTER_MOVE                           = 0x0EA9, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_SET_RUN_SPEED              = 0x164F, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_SET_RUN_BACK_SPEED         = 0x0305, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_SET_SWIM_SPEED             = 0x0E22, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_SET_SWIM_BACK_SPEED        = 0x0A47, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_SET_FLIGHT_SPEED           = 0x1208, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_SET_FLIGHT_BACK_SPEED      = 0x1F22, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_SET_WALK_SPEED             = 0x0F28, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_SET_TURN_RATE              = 0x035B, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_SET_PITCH_RATE             = 0x02C8, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_ROOT                       = 0x02D1, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_UNROOT                     = 0x1BA2, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_GRAVITY_DISABLE            = 0x1A21, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_GRAVITY_ENABLE             = 0x0252, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_COLLISION_DISABLE          = 0x0000, ///< (unused)
        SMSG_SPLINE_MOVE_COLLISION_ENABLE           = 0x0000, ///< (unused)
        SMSG_SPLINE_MOVE_SET_FEATHER_FALL           = 0x0F30, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_SET_NORMAL_FALL            = 0x037A, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_SET_HOVER                  = 0x1B21, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_UNSET_HOVER                = 0x0E47, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_SET_WATER_WALK             = 0x0EAB, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_SET_LAND_WALK              = 0x1A28, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_START_SWIM                 = 0x0000, ///< (unused)
        SMSG_SPLINE_MOVE_STOP_SWIM                  = 0x0000, ///< (unused)
        SMSG_SPLINE_MOVE_SET_RUN_MODE               = 0x1A8C, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_SET_WALK_MODE              = 0x0B09, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_SET_FLYING                 = 0x0B2A, ///< 6.1.2 19783
        SMSG_SPLINE_MOVE_UNSET_FLYING               = 0x1A2C, ///< 6.1.2 19783
        SMSG_FLIGHT_SPLINE_SYNC                     = 0x1647, ///< 6.1.2 19865
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Spell
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamSpell
        /// Aura
        SMSG_AURA_POINTS_DEPLETED                   = 0x0000, ///< (unused)
        SMSG_AURA_UPDATE                            = 0x1999, ///< 6.1.2 19783

        /// Misc
        SMSG_CHEAT_IGNORE_DIMISHING_RETURNS         = 0x0000, ///< (unused)
        SMSG_DISPEL_FAILED                          = 0x148E, ///< 6.1.2 19783
        SMSG_INCREASE_CAST_TIME_FOR_SPELL           = 0x0000, ///< (unused)
        SMSG_MIRROR_IMAGE_COMPONENTED_DATA          = 0x159E, ///< 6.1.2 19783
        SMSG_MIRROR_IMAGE_CREATURE_DATA             = 0x0000, ///< (unused)
        SMSG_PET_CLEAR_SPELLS                       = 0x0000, ///< (unused)
        SMSG_REFRESH_SPELL_HISTORY                  = 0x0000, ///< (unused)
        SMSG_RESUME_CAST_BAR                        = 0x0000, ///< (unused)
        SMSG_CAST_FAILED                            = 0x1409, ///< 6.1.2 19783
        SMSG_SPELL_FAILURE                          = 0x1CAD, ///< 6.1.2 19783
        SMSG_SPELL_FAILED_OTHER                     = 0x10CE, ///< 6.1.2 19783
        SMSG_PET_CAST_FAILED                        = 0x108D, ///< 6.1.2 19783
        SMSG_ITEM_COOLDOWN                          = 0x0D35, ///< 6.1.2 19783
        SMSG_MODIFY_COOLDOWN                        = 0x0832, ///< 6.1.2 19783
        SMSG_COOLDOWN_CHEAT                         = 0x0000, ///< (unused)
        SMSG_PET_TAME_FAILURE                       = 0x0616, ///< 6.1.2 19783
        SMSG_FEIGN_DEATH_RESISTED                   = 0x1CE5, ///< 6.1.2 19783
        SMSG_NOTIFY_DEST_LOC_SPELL_CAST             = 0x0000, ///< (unused)
        SMSG_ON_CANCEL_EXPECTED_RIDE_VEHICLE_AURA   = 0x0C66, ///< 6.1.2 19783
        SMSG_SET_VEHICLE_REC_ID                     = 0x0F1F, ///< 6.1.2 19783
        SMSG_COOLDOWN_EVENT                         = 0x0922, ///< 6.1.2 19783
        SMSG_DISMOUNT                               = 0x03BF, ///< 6.1.2 19783
        SMSG_DISMOUNTRESULT                         = 0x0000, ///< (unused)
        SMSG_MOUNT_RESULT                           = 0x06BE, ///< 6.1.2 19783

        /// Spell Book / Bar
        SMSG_UPDATE_WEEKLY_SPELL_USAGE              = 0x0000, ///< (unused)
        SMSG_WEEKLY_SPELL_USAGE                     = 0x0000, ///< (unused)
        SMSG_SEND_KNOWN_SPELLS                      = 0x109A, ///< 6.1.2 19783
        SMSG_SEND_SPELL_CHARGES                     = 0x187E, ///< 6.1.2 19783
        SMSG_SEND_SPELL_HISTORY                     = 0x142D, ///< 6.1.2 19783
        SMSG_SEND_UNLEARN_SPELLS                    = 0x0000, ///< (unused)
        SMSG_CLEAR_ALL_SPELL_CHARGES                = 0x152D, ///< 6.1.2 19783
        SMSG_CLEAR_COOLDOWN                         = 0x0A38, ///< 6.1.2 19783
        SMSG_CLEAR_COOLDOWNS                        = 0x15ED, ///< 6.1.2 19783
        SMSG_CATEGORY_COOLDOWN                      = 0x15FA, ///< 6.1.2 19783
        SMSG_CLEAR_SPELL_CHARGES                    = 0x10AE, ///< 6.1.2 19783
        SMSG_SET_FLAT_SPELL_MODIFIER                = 0x192A, ///< 6.1.2 19783
        SMSG_SET_PCT_SPELL_MODIFIER                 = 0x1DAA, ///< 6.1.2 19783
        SMSG_SET_SPELL_CHARGES                      = 0x1909, ///< 6.1.2 19783
        SMSG_LEARNED_SPELL                          = 0x183D, ///< 6.1.2 19783
        SMSG_UNLEARNED_SPELLS                       = 0x151E, ///< 6.1.2 19783

        /// Casting
        SMSG_SPELL_CHANNEL_START                    = 0x103E, ///< 6.1.2 19783
        SMSG_SPELL_COOLDOWN                         = 0x1D2A, ///< 6.1.2 19783
        SMSG_SPELL_CHANNEL_UPDATE                   = 0x10D9, ///< 6.1.2 19783
        SMSG_SPELL_DAMAGE_SHIELD                    = 0x10FE, ///< 6.1.2 19783
        SMSG_SPELL_DELAYED                          = 0x14FD, ///< 6.1.2 19783
        SMSG_SPELL_GO                               = 0x1CB9, ///< 6.1.2 19783
        SMSG_SPELL_MULTISTRIKE_EFFECT               = 0x0000, ///< (unused)
        SMSG_SPELL_OR_DAMAGE_IMMUNE                 = 0x11B9, ///< 6.1.2 19783
        SMSG_SPELL_START                            = 0x14BA, ///< 6.1.2 19783
        SMSG_SPELL_UPDATE_CHAIN_TARGETS             = 0x0000, ///< (unused)

        /// Logging
        SMSG_SPELL_DISPELL_LOG                      = 0x10FD, ///< 6.1.2 19783
        SMSG_SPELL_ENERGIZE_LOG                     = 0x1C3D, ///< 6.1.2 19783
        SMSG_SPELL_EXECUTE_LOG                      = 0x1D9A, ///< 6.1.2 19783
        SMSG_SPELL_NON_MELEE_DAMAGE_LOG             = 0x141E, ///< 6.1.2 19783
        SMSG_SPELL_HEAL_LOG                         = 0x155A, ///< 6.1.2 19783
        SMSG_SPELL_INSTAKILL_LOG                    = 0x11FE, ///< 6.1.2 19783
        SMSG_SPELL_INTERRUPT_LOG                    = 0x185E, ///< 6.1.2 19783
        SMSG_SPELL_MISS_LOG                         = 0x147A, ///< 6.1.2 19783
        SMSG_ENVIRONMENTAL_DAMAGE_LOG               = 0x183E, ///< 6.1.2 19783
        SMSG_SPELL_PERIODIC_AURA_LOG                = 0x14E9, ///< 6.1.2 19783
        SMSG_SPELL_PROC_SCRIPT_LOG                  = 0x0000, ///< Debug opcode (disabled)
        SMSG_SPELL_PROCS_PER_MINUTE_LOG             = 0x0000, ///< Debug opcode (disabled)
        SMSG_SPELL_CHANCE_PROC_LOG                  = 0x0000, ///< Debug opcode (disabled)
        SMSG_RESIST_LOG                             = 0x0000, ///< Debug opcode (disabled)
        SMSG_AURA_CAST_LOG                          = 0x0000, ///< Debug opcode (disabled)

        /// Pet
        SMSG_PET_SPELLS_MESSAGE                     = 0x14CA, ///< 6.1.2 19783
        SMSG_PET_UNLEARNED_SPELLS                   = 0x15CD, ///< 6.1.2 19783
        SMSG_PET_LEARNED_SPELLS                     = 0x1099, ///< 6.1.2 19783

        /// Visuals
        SMSG_PLAY_ORPHAN_SPELL_VISUAL               = 0x191E, ///< 6.1.2 19783
        SMSG_CANCEL_ORPHAN_SPELL_VISUAL             = 0x10ED, ///< 6.1.2 19783
        SMSG_PLAY_SPELL_VISUAL                      = 0x11EA, ///< 6.1.2 19783
        SMSG_CANCEL_SPELL_VISUAL                    = 0x106D, ///< 6.1.2 19783
        SMSG_PLAY_SPELL_VISUAL_KIT                  = 0x1859, ///< 6.1.2 19783
        SMSG_CANCEL_SPELL_VISUAL_KIT                = 0x112E, ///< 6.1.2 19783
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Quest
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamQuest
        /// Quest log
        SMSG_QUERY_QUEST_INFO_RESPONSE              = 0x0817, ///< 6.1.2 19783
        SMSG_QUEST_UPDATE_FAILED_TIMER              = 0x0108, ///< 6.1.2 19783
        SMSG_QUEST_UPDATE_FAILED                    = 0x0573, ///< 6.1.2 19783
        SMSG_QUEST_UPDATE_COMPLETE                  = 0x0480, ///< 6.1.2 19783
        SMSG_QUEST_UPDATE_ADD_PVP_CREDIT            = 0x005B, ///< 6.1.2 19783
        SMSG_QUEST_UPDATE_ADD_CREDIT_SIMPLE         = 0x0070, ///< 6.1.2 19783
        SMSG_QUEST_UPDATE_ADD_CREDIT                = 0x006C, ///< 6.1.2 19783
        SMSG_QUEST_PUSH_RESULT                      = 0x007B, ///< 6.1.2 19783
        SMSG_QUEST_POIQUERY_RESPONSE                = 0x051F, ///< 6.1.2 19783
        SMSG_QUEST_LOG_FULL                         = 0x0508, ///< 6.1.2 19783
        SMSG_IS_QUEST_COMPLETE_RESPONSE             = 0x0063, ///< 6.1.2 19783
        SMSG_QUEST_FORCE_REMOVED                    = 0x0504, ///< 6.1.2 19783
        SMSG_QUEST_CONFIRM_ACCEPT                   = 0x054C, ///< 6.1.2 19783
        SMSG_QUEST_COMPLETION_NPCRESPONSE           = 0x0540, ///< 6.1.2 19783
        SMSG_DAILY_QUESTS_RESET                     = 0x055B, ///< 6.1.2 19783

        /// Quest giver
        SMSG_QUEST_GIVER_STATUS_MULTIPLE            = 0x0814, ///< 6.1.2 19783
        SMSG_QUEST_GIVER_STATUS                     = 0x052F, ///< 6.1.2 19783
        SMSG_QUEST_GIVER_REQUEST_ITEMS              = 0x0528, ///< 6.1.2 19783
        SMSG_QUEST_GIVER_QUEST_LIST_MESSAGE         = 0x0843, ///< 6.1.2 19783
        SMSG_QUEST_GIVER_QUEST_FAILED               = 0x007F, ///< 6.1.2 19783
        SMSG_QUEST_GIVER_QUEST_DETAILS              = 0x0534, ///< 6.1.2 19783
        SMSG_QUEST_GIVER_QUEST_COMPLETE             = 0x004C, ///< 6.1.2 19783
        SMSG_QUEST_GIVER_OFFER_REWARD_MESSAGE       = 0x0547, ///< 6.1.2 19783
        SMSG_QUEST_GIVER_INVALID_QUEST              = 0x0543, ///< 6.1.2 19783

        /// Gossip
        SMSG_GOSSIP_MESSAGE                         = 0x0077, ///< 6.1.2 19783
        SMSG_GOSSIP_COMPLETE                        = 0x0010, ///< 6.1.2 19783
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Lfg
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamLFG
        /// LFG
        SMSG_LFG_BOOT_PLAYER                              = 0x0E11, ///< 6.1.2 19783
        SMSG_LFG_DISABLED                                 = 0x0EBA, ///< 6.1.2 19783
        SMSG_LFG_JOIN_RESULT                              = 0x1B31, ///< 6.1.2 19783
        SMSG_LFG_OFFER_CONTINUE                           = 0x0FB9, ///< 6.1.2 19783
        SMSG_LFG_OPEN_FROM_GOSSIP                         = 0x0000, ///< (unused)
        SMSG_LFG_PARTY_INFO                               = 0x1B39, ///< 6.1.2 19783
        SMSG_LFG_PLAYER_INFO                              = 0x0E32, ///< 6.1.2 19783
        SMSG_LFG_PLAYER_REWARD                            = 0x0E91, ///< 6.1.2 19783
        SMSG_LFG_PROPOSAL_UPDATE                          = 0x1B3A, ///< 6.1.2 19783
        SMSG_LFG_QUEUE_STATUS                             = 0x1A91, ///< 6.1.2 19783
        SMSG_LFG_ROLE_CHECK_UPDATE                        = 0x1A11, ///< 6.1.2 19783
        SMSG_LFG_ROLE_CHOSEN                              = 0x0A9A, ///< 6.1.2 19783
        SMSG_LFG_SEARCH_RESULT                            = 0x0000, ///< (unused)
        SMSG_LFG_SLOT_INVALID                             = 0x0000, ///< (unused)
        SMSG_LFG_TELEPORT_DENIED                          = 0x0FB1, ///< 6.1.2 19783
        SMSG_LFG_UPDATE_LIST                              = 0x0000, ///<
        SMSG_LFG_UPDATE_PARTY                             = 0x0000,
        SMSG_LFG_UPDATE_PLAYER                            = 0x0000,
        SMSG_LFG_UPDATE_SEARCH                            = 0x0000,
        SMSG_LFG_UPDATE_STATUS                            = 0x1A92, ///< 6.1.2 19783
        SMSG_LFG_UPDATE_STATUS_NONE                       = 0x0000,
        SMSG_UPDATE_DUNGEON_ENCOUNTER_FOR_LOOT            = 0x0000,

        /// LFG List
        SMSG_LFG_LIST_JOIN_RESULT                         = 0x0B99, ///< 6.1.2 19783
        SMSG_LFG_LIST_UPDATE_BLACKLIST                    = 0x0F31, ///< 6.1.2 19783
        SMSG_LFG_LIST_UPDATE_STATUS                       = 0x0B12, ///< 6.1.2 19783
        SMSG_LFG_LIST_SEARCH_RESULT                       = 0x0B32, ///< 6.1.2 19783
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Garrison
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamGarrison
        SMSG_GET_GARRISON_INFO_RESULT                                   = 0x0084, ///< 6.1.2 19783
        SMSG_GARRISON_PLOT_PLACED                                       = 0x00E7, ///< 6.1.2 19783
        SMSG_GARRISON_PLOT_REMOVED                                      = 0x01AB, ///< 6.1.2 19783
        SMSG_GARRISON_PLACE_BUILDING_RESULT                             = 0x08A4, ///< 6.1.2 19783
        SMSG_GARRISON_BUILDING_REMOVED                                  = 0x08F7, ///< 6.1.2 19783
        SMSG_GARRISON_LEARN_SPECIALIZATION_RESULT                       = 0x0000,
        SMSG_GARRISON_BUILDING_SET_ACTIVE_SPECIALIZATION_RESULT         = 0x0000,
        SMSG_GARRISON_LEARN_BLUEPRINT_RESULT                            = 0x08D8, ///< 6.1.2 19783
        SMSG_GARRISON_BLUEPRINT_AND_SPECIALIZATION_DATA                 = 0x08B4, ///< 6.1.2 19783
        SMSG_GARRISON_GET_BUILDINGS_DATA                                = 0x0987, ///< 6.1.2 19783
        SMSG_GARRISON_REMOTE_INFO                                       = 0x01B0, ///< 6.1.2 19783
        SMSG_GARRISON_BUILDING_ACTIVATED                                = 0x00AB, ///< 6.1.2 19783
        SMSG_GARRISON_UPGRADE_RESULT                                    = 0x0000,
        SMSG_GARRISON_ADD_FOLLOWER_RESULT                               = 0x08A3, ///< 6.1.2 19783
        SMSG_GARRISON_REMOVE_FOLLOWER_RESULT                            = 0x0000,
        SMSG_GARRISON_LIST_FOLLOWERS_CHEAT_RESULT                       = 0x0000,
        SMSG_GARRISON_UPDATE_FOLLOWER_ACTIVATION_COUNT                  = 0x088F, ///< 6.1.2 19783
        SMSG_GARRISON_UPDATE_FOLLOWER                                   = 0x0093, ///< 6.1.2 19783
        SMSG_GARRISON_FOLLOWER_CHANGED_XP                               = 0x00AC, ///< 6.1.2 19783
        SMSG_GARRISON_FOLLOWER_CHANGED_ITEM_LEVEL                       = 0x0000,
        SMSG_GARRISON_ADD_MISSION_RESULT                                = 0x01DB, ///< 6.1.2 19783
        SMSG_GARRISON_START_MISSION_RESULT                              = 0x01E0, ///< 6.1.2 19783
        SMSG_GARRISON_COMPLETE_MISSION_RESULT                           = 0x08FF, ///< 6.1.2 19783
        SMSG_GARRISON_MISSION_BONUS_ROLL_RESULT                         = 0x019C, ///< 6.1.2 19783
        SMSG_GARRISON_ASSIGN_FOLLOWER_TO_BUILDING_RESULT                = 0x0000,
        SMSG_GARRISON_REMOVE_FOLLOWER_FROM_BUILDING_RESULT              = 0x0000,
        SMSG_GARRISON_DELETE_RESULT                                     = 0x0000,
        SMSG_GARRISON_OPEN_ARCHITECT                                    = 0x0000,
        SMSG_GARRISON_OPEN_MISSION_NPC                                  = 0x0988, ///< 6.1.2 19783
        SMSG_GARRISON_REQUEST_UPGRADEABLE_RESULT                        = 0x01A8, ///< 6.1.2 19783
        SMSG_GARRISON_OPEN_TRADESKILL_NPC                               = 0x018F, ///< 6.1.2 19783
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
    CMSG_GARRISON_CANCEL_CONSTRUCTION                       = 0x1A02,   ///< 6.1.2 19783
    CMSG_GARRISON_LEARN_BUILDING_SPECIALIZATION_CHEAT       = 0x0000,
    CMSG_GARRISON_SET_ACTIVE_BUILDING_SPECIALIZATION        = 0x0000,
    CMSG_GARRISON_SET_ACTIVE_BUILDING_SPECIALIZATION_CHEAT  = 0x0000,
    CMSG_GET_GARRISON_INFO                                  = 0x072D,   ///< 6.1.2 19783
    CMSG_GARRISON_LEARN_BLUEPRINT_CHEAT                     = 0x0000,
    CMSG_GARRISON_REQUEST_BUILDINGS                         = 0x0EFC,   ///< 6.1.2 19783
    CMSG_GARRISON_PURCHASE_BUILDING                         = 0x1208,   ///< 6.1.2 19783
    CMSG_GARRISON_SET_BUILDING_ACTIVE                       = 0x0000,
    CMSG_GARRISON_FORCE_BUILDING_ACTIVE                     = 0x0000,
    CMSG_GARRISON_PORT_CHEAT                                = 0x0000,
    CMSG_GARRISON_ADD_FOLLOWER_XPCHEAT                      = 0x0000,
    CMSG_GARRISON_SET_FOLLOWER_ITEM_LEVEL_CHEAT             = 0x0000,
    CMSG_GARRISON_ASSIGN_FOLLOWER_TO_BUILDING               = 0x0000,
    CMSG_GARRISON_REMOVE_FOLLOWER_FROM_BUILDING             = 0x0000,
    CMSG_GARRISON_CHANGE_FOLLOWER_ACTIVATION_STATE          = 0x0AC8,   ///< 6.1.2 19783
    CMSG_SHOW_GARRISON_TRANSFER                             = 0x0000,
    CMSG_GARRISON_ADD_UNIQUE_FOLLOWER_CHEAT                 = 0x0000,
    CMSG_GARRISON_REMOVE_FOLLOWER_CHEAT                     = 0x0000,
    CMSG_GARRISON_LIST_FOLLOWERS_CHEAT                      = 0x0000,
    CMSG_GARRISON_ADD_MISSION_CHEAT                         = 0x0000,
    CMSG_GARRISON_START_MISSION                             = 0x0E0E,   ///< 6.1.2 19783
    CMSG_GARRISON_COMPLETE_MISSION                          = 0x0BA2,   ///< 6.1.2 19783
    CMSG_GARRISON_MISSION_BONUS_ROLL                        = 0x07D2,   ///< 6.1.2 19783
    CMSG_GARRISON_REQUEST_UPGRADEABLE                       = 0x12FA,   ///< 6.1.2 19783
    CMSG_UPGRADE_GARRISON                                   = 0x1B82,   ///< 6.1.2 19783
    CMSG_GARRISON_REQUEST_LANDING_PAGE_SHIPMENT_INFO        = 0x0000,
    CMSG_GARRISON_MISSION_NPC_HELLO                         = 0x0BA9,   ///< 6.1.2 19783
    CMSG_GET_SHIPMENT_INFO                                  = 0x03FA,   ///< 6.1.2 19783
    CMSG_CREATE_SHIPMENT                                    = 0x010E,   ///< 6.1.2 19783
    CMSG_GET_SHIPMENTS                                      = 0x06DA,   ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// User Router
    //////////////////////////////////////////////////////////////////////////
    CMSG_LOG_STREAMING_ERROR                    = 0x12D6, ///< 6.1.2 19783
    CMSG_QUEUED_MESSAGES_END                    = 0x0000, ///<
    CMSG_LOG_DISCONNECT                         = 0x12D5, ///< 6.1.2 19783
    CMSG_PING                                   = 0x12DE, ///< 6.1.2 19783
    CMSG_AUTH_CONTINUED_SESSION                 = 0x0000, ///<
    CMSG_SUSPEND_TOKEN_RESPONSE                 = 0x0000, ///<
    CMSG_AUTH_SESSION                           = 0x03DD, ///< 6.1.2 19783
    CMSG_ENABLE_NAGLE                           = 0x0B55, ///< 6.1.2 19783
    CMSG_SUSPEND_COMMS_ACK                      = 0x0000, ///<
    CMSG_KEEP_ALIVE                             = 0x0000, ///<
    CMSG_OBJECT_UPDATE_FAILED                   = 0x0000, ///<
    CMSG_OBJECT_UPDATE_RESCUED                  = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Character
    //////////////////////////////////////////////////////////////////////////
    CMSG_ENUM_CHARACTERS                        = 0x1696,   ///< 6.1.2 19783
    CMSG_CREATE_CHARACTER                       = 0x1636,   ///< 6.1.2 19783
    CMSG_CHAR_DELETE                            = 0x12B8,   ///< 6.1.2 19783
    CMSG_REORDER_CHARACTERS                     = 0x17B7,   ///< 6.1.2 19783
    CMSG_PLAYER_LOGIN                           = 0x0E98,   ///< 6.1.2 19783
    CMSG_VIOLENCE_LEVEL                         = 0x0F48,   ///< 6.1.2 19783
    CMSG_LOAD_SCREEN                            = 0x13C0,   ///< 6.1.2 19783
    CMSG_REQUEST_ACCOUNT_DATA                   = 0x0798,   ///< 6.1.2 19783
    CMSG_UPDATE_ACCOUNT_DATA                    = 0x1637,   ///< 6.1.2 19783
    CMSG_SET_DUNGEON_DIFFICULTY                 = 0x0E16,   ///< 6.1.2 19783
    CMSG_SET_RAID_DIFFICULTY                    = 0x0397,   ///< 6.1.2 19783
    CMSG_AUTO_DECLINE_GUILD_INVITES             = 0x09B1,   ///< 6.1.2 19783
    CMSG_SHOWING_CLOAK                          = 0x0F04,   ///< 6.1.2 19783
    CMSG_SHOWING_HELM                           = 0x0C36,   ///< 6.1.2 19783
    CMSG_SET_ACTIVE_MOVER                       = 0x0108,   ///< 6.1.2 19783
    CMSG_LEARN_TALENTS                          = 0x0AAA,   ///< 6.1.2 19783
    CMSG_AUTOEQUIP_ITEM                         = 0x0423,   ///< 6.1.2 19783
    CMSG_SWAP_INV_ITEM                          = 0x003C,   ///< 6.1.2 19783
    CMSG_SWAP_ITEM                              = 0x0438,   ///< 6.1.2 19783
    CMSG_AUTOSTORE_BAG_ITEM                     = 0x0424,   ///< 6.1.2 19783
    CMSG_REQUEST_PET_INFO                       = 0x0000,   ///<
    CMSG_STAND_STATE_CHANGE                     = 0x01AD,   ///< 6.1.2 19783
    CMSG_BINDER_ACTIVATE                        = 0x1C71,   ///< 6.1.2 19783
    CMSG_REQUEST_FORCED_REACTIONS               = 0x1AAC,   ///< 6.1.2 19783
    CMSG_DESTROY_ITEM                           = 0x0A72,   ///< 6.1.2 19783
    CMSG_OPEN_ITEM                              = 0x0354,   ///< 6.1.2 19783
    CMSG_SET_TITLE                              = 0x1650,   ///< 6.1.2 19783
    CMSG_PLAYED_TIME                            = 0x0750,   ///< 6.1.2 19783
    CMSG_SAVE_EQUIPMENT_SET                     = 0x09E2,   ///< 6.1.2 19783
    CMSG_USE_EQUIPMENT_SET                      = 0x083C,   ///< 6.1.2 19783
    CMSG_DELETE_EQUIPMENT_SET                   = 0x014A,   ///< 6.1.2 19783
    CMSG_WHO                                    = 0x079E,   ///< 6.1.2 19783
    CMSG_SOCKET_GEMS                            = 0x0F8B,   ///< 6.1.2 19783
    CMSG_RESURRECT_RESPONSE                     = 0x033E,   ///< 6.1.2 19783
    CMSG_QUERY_INSPECT_ACHIEVEMENTS             = 0x014E,   ///< 6.1.2 19783
    CMSG_SPLIT_ITEM                             = 0x052B,   ///< 6.1.2 19783
    CMSG_SET_PLAYER_DECLINED_NAMES              = 0x1617,   ///< 6.1.2 19783
    CMSG_MOUNT_SET_FAVORITE                     = 0x061E,   ///< 6.1.2 19783
    CMSG_SORT_BAGS                              = 0x0AF1,   ///< 6.1.2 19783
    CMSG_CHAR_RENAME                            = 0x0616,   ///< 6.1.2 19783
    CMSG_CHAR_CUSTOMIZE                         = 0x0A3D,   ///< 6.1.2 19783
    CMSG_CHAR_RACE_OR_FACTION_CHANGE            = 0x0F38,   ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Bank
    //////////////////////////////////////////////////////////////////////////
    CMSG_BANKER_ACTIVATE                        = 0x0CA5, ///< 6.1.2 19783
    CMSG_AUTOBANK_ITEM                          = 0x043F, ///< 6.1.2 19783
    CMSG_AUTOSTORE_BANK_ITEM                    = 0x0027, ///< 6.1.2 19783
    CMSG_BUY_BANK_SLOT                          = 0x1DE2, ///< 6.1.2 19783
    CMSG_BUY_REAGENT_BANK                       = 0x1D75, ///< 6.1.2 19783
    CMSG_SORT_BANK_BAGS                         = 0x0659, ///< 6.1.2 19783
    CMSG_SORT_REAGENT_BANK_BAGS                 = 0x06D2, ///< 6.1.2 19783
    CMSG_DEPOSIT_ALL_REAGENTS                   = 0x002A, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Auction House
    //////////////////////////////////////////////////////////////////////////
    CMSG_AUCTION_LIST_BIDDER_ITEMS              = 0x1821, ///< 6.1.2 19783
    CMSG_AUCTION_LIST_ITEMS                     = 0x0CB6, ///< 6.1.2 19783
    CMSG_AUCTION_LIST_OWNER_ITEMS               = 0x08A5, ///< 6.1.2 19783
    CMSG_AUCTION_LIST_PENDING_SALES             = 0x0935, ///< 6.1.2 19783
    CMSG_AUCTION_PLACE_BID                      = 0x18F6, ///< 6.1.2 19783
    CMSG_AUCTION_REMOVE_ITEM                    = 0x1C32, ///< 6.1.2 19783
    CMSG_AUCTION_SELL_ITEM                      = 0x0D25, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Loot
    //////////////////////////////////////////////////////////////////////////
    CMSG_LOOT                                   = 0x0BF1, ///< 6.1.2 19783
    CMSG_LOOT_MONEY                             = 0x050A, ///< 6.1.2 19783
    CMSG_LOOT_ITEM                              = 0x1BAC, ///< 6.1.2 19783
    CMSG_LOOT_RELEASE                           = 0x1A25, ///< 6.1.2 19783
    CMSG_LOOT_ROLL                              = 0x1F23, ///< 6.1.2 19783
    CMSG_MASTER_LOOT_ITEM                       = 0x06D9, ///< 6.1.2 19783
    CMSG_DO_MASTER_LOOT_ROLL                    = 0x0409, ///< 6.1.2 19783
    CMSG_SET_LOOT_SPECIALIZATION                = 0x0D72, ///< 6.1.2 19783
    CMSG_SET_LOOT_METHOD                        = 0x0E3E, ///< 6.1.2 19783
    CMSG_OPT_OUT_OF_LOOT                        = 0x1F89, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Combat
    //////////////////////////////////////////////////////////////////////////
    CMSG_ATTACKSTOP                             = 0x0A01, ///< 6.1.2 19783
    CMSG_ATTACKSWING                            = 0x0BF4, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Duel
    //////////////////////////////////////////////////////////////////////////
    CMSG_DUEL_PROPOSED                          = 0x0A38, ///< 6.1.2 19783
    CMSG_DUEL_RESPONSE                          = 0x0C62, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Spell
    //////////////////////////////////////////////////////////////////////////
    CMSG_CAST_SPELL                             = 0x1274, ///< 6.1.2 19783
    CMSG_CANCEL_CAST                            = 0x058A, ///< 6.1.2 19783
    CMSG_USE_ITEM                               = 0x06D0, ///< 6.1.2 19783
    CMSG_CANCEL_AURA                            = 0x12FB, ///< 6.1.2 19783
    CMSG_CANCEL_AUTO_REPEAT_SPELL               = 0x0821, ///< 6.1.2 19783
    CMSG_CANCEL_CHANNELLING                     = 0x0F71, ///< 6.1.2 19783
    CMSG_CANCEL_GROWTH_AURA                     = 0x0AA3, ///< 6.1.2 19783
    CMSG_CANCEL_MOUNT_AURA                      = 0x012D, ///< 6.1.2 19783
    CMSG_CANCEL_QUEUED_SPELL                    = 0x0EA2, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Cache
    //////////////////////////////////////////////////////////////////////////
    CMSG_DB_QUERY_BULK                          = 0x039F, ///< 6.1.2 19783
    CMSG_QUERY_CREATURE                         = 0x0FD3, ///< 6.1.2 19783
    CMSG_NPC_TEXT_QUERY                         = 0x1E24, ///< 6.1.2 19783
    CMSG_NAME_QUERY                             = 0x0BBD, ///< 6.1.2 19783
    CMSG_QUEST_QUERY                            = 0x0FA9, ///< 6.1.2 19783
    CMSG_QUEST_POI_QUERY                        = 0x1240, ///< 6.1.2 19783
    CMSG_REALM_NAME_QUERY                       = 0x0000, ///<
    CMSG_GAMEOBJECT_QUERY                       = 0x06C8, ///< 6.1.2 19783
    CMSG_PETITION_QUERY                         = 0x0FAC, ///< 6.1.2 19783
    CMSG_QUERY_GUILD_INFO                       = 0x12BE, ///< 6.1.2 19783
    CMSG_PAGE_TEXT_QUERY                        = 0x1AA2, ///< 6.1.2 19783
    CMSG_ITEM_TEXT_QUERY                        = 0x037B, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Interaction
    //////////////////////////////////////////////////////////////////////////
    CMSG_LOGOUT_REQUEST                         = 0x0CA6, ///< 6.1.2 19783
    CMSG_LOGOUT_CANCEL                          = 0x0F8C, ///< 6.1.2 19783
    CMSG_SET_ACTION_BUTTON                      = 0x133F, ///< 6.1.2 19783
    CMSG_SET_ACTIONBAR_TOGGLES                  = 0x0F81, ///< 6.1.2 19783
    CMSG_SET_SELECTION                          = 0x0E8C, ///< 6.1.2 19783
    CMSG_SETSHEATHED                            = 0x1CB1, ///< 6.1.2 19783
    CMSG_GOSSIP_HELLO                           = 0x1C22, ///< 6.1.2 19783
    CMSG_GOSSIP_SELECT_OPTION                   = 0x1E0C, ///< 6.1.2 19783
    CMSG_TOGGLE_PVP                             = 0x1BC7, ///< 6.1.2 19783
    CMSG_TUTORIAL                               = 0x0E9F, ///< 6.1.2 19783
    CMSG_SET_SPECIALIZATION                     = 0x0759, ///< 6.1.2 19783
    CMSG_SET_WATCHED_FACTION                    = 0x1E82, ///< 6.1.2 19783
    CMSG_SET_FACTION_INACTIVE                   = 0x1862, ///< 6.1.2 19783
    CMSG_AREATRIGGER                            = 0x0BDB, ///< 6.1.2 19783
    CMSG_GAMEOBJECT_REPORT_USE                  = 0x18B2, ///< 6.1.2 19783
    CMSG_GAMEOBJECT_USE                         = 0x08B1, ///< 6.1.2 19783
    CMSG_SAVE_CUF_PROFILES                      = 0x0EC7, ///< 6.1.2 19783
    CMSG_SPELLCLICK                             = 0x1DB2, ///< 6.1.2 19783
    CMSG_REPOP_REQUEST                          = 0x0D36, ///< 6.1.2 19783
    CMSG_RECLAIM_CORPSE                         = 0x0DB1, ///< 6.1.2 19783
    CMSG_QUERY_CORPSE_LOCATION_FROM_CLIENT      = 0x03B7, ///< 6.1.2 19783
    CMSG_QUERY_CORPSE_TRANSPORT                 = 0x1215, ///< 6.1.2 19783
    CMSG_RETURN_TO_GRAVEYARD                    = 0x0C65, ///< 6.1.2 19783
    CMSG_CLOSE_INTERACTION                      = 0x1C36, ///< 6.1.2 19783
    CMSG_ITEM_REFUND_INFO                       = 0x016E, ///< 6.1.2 19783
    CMSG_FAR_SIGHT                              = 0x00EE, ///< 6.1.2 19783
    CMSG_MOUNTSPECIAL_ANIM                      = 0x02D1, ///< 6.1.2 19783
    CMSG_OPENING_CINEMATIC                      = 0x0E0A, ///< 6.1.2 19783
    CMSG_NEXT_CINEMATIC_CAMERA                  = 0x0CE1, ///< 6.1.2 19783
    CMSG_COMPLETE_CINEMATIC                     = 0x0CF2, ///< 6.1.2 19783
    CMSG_REQUEST_CEMETERY_LIST                  = 0x0FD0, ///< 6.1.2 19783
    CMSG_TOTEM_DESTROYED                        = 0x19B5, ///< 6.1.2 19783
    CMSG_CONFIRM_RESPEC_WIPE                    = 0x1226, ///< 6.1.2 19783
    CMSG_CANCEL_TRADE                           = 0x0E0F, ///< 6.1.2 19783
    CMSG_SET_TRADE_CURRENCY                     = 0x06F2, ///< 6.1.2 19783
    CMSG_SET_TRADE_GOLD                         = 0x0E5A, ///< 6.1.2 19783
    CMSG_SET_TRADE_ITEM                         = 0x0626, ///< 6.1.2 19783
    CMSG_CLEAR_TRADE_ITEM                       = 0x0F7B, ///< 6.1.2 19783
    CMSG_ACCEPT_TRADE                           = 0x0F07, ///< 6.1.2 19783
    CMSG_BUSY_TRADE                             = 0x0E2B, ///< 6.1.2 19783
    CMSG_BEGIN_TRADE                            = 0x0E53, ///< 6.1.2 19783
    CMSG_IGNORE_TRADE                           = 0x1A08, ///< 6.1.2 19783
    CMSG_INITIATE_TRADE                         = 0x1A8C, ///< 6.1.2 19783
    CMSG_UNACCEPT_TRADE                         = 0x0AD1, ///< 6.1.2 19783
    CMSG_NEUTRAL_PLAYER_SELECT_FACTION          = 0x0EC8, ///< 6.1.2 19783
    CMSG_INSPECT                                = 0x1C21, ///< 6.1.2 19783
    CMSG_INSPECT_HONOR_STATS                    = 0x1207, ///< 6.1.2 19783
    CMSG_REQUEST_INSPECT_RATED_BG_STATS         = 0x0A9E, ///< 6.1.2 19783
    CMSG_TIME_SYNC_RESP                         = 0x0550, ///< 6.1.2 19783
    CMSG_UNLEARN_SKILL                          = 0x0931, ///< 6.1.2 19783
    CMSG_EMOTE                                  = 0x0E03, ///< 6.1.2 19783
    CMSG_SEND_TEXT_EMOTE                        = 0x01EE, ///< 6.1.2 19783
    CMSG_ALTER_APPEARANCE                       = 0x0F82, ///< 6.1.2 19783
    CMSG_SELF_RES                               = 0x1E0B, ///< 6.1.2 19783
    CMSG_READ_ITEM                              = 0x042A, ///< 6.1.2 19783
    CMSG_COMPLETE_MOVIE                         = 0x1E0A, ///< 6.1.2 19783
    CMSG_SCENE_TRIGGER_EVENT                    = 0x0589, ///< 6.1.2 19783
    CMSG_GET_MIRRORIMAGE_DATA                   = 0x0E74, ///< 6.1.2 19783
    CMSG_SHOW_TRADE_SKILL                       = 0x1735, ///< 6.1.2 19783
    CMSG_SCENE_PLAYBACK_CANCELED                = 0x0A8C, ///< 6.1.2 19783
    CMSG_REQUEST_RESEARCH_HISTORY               = 0x0EA9, ///< 6.1.2 19783
    CMSG_SUMMON_RESPONSE                        = 0x0740, ///< 6.1.2 19783
    CMSG_SET_FACTION_ATWAR                      = 0x1C66, ///< 6.1.2 19783
    CMSG_UNSET_FACTION_ATWAR                    = 0x0DF2, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Vehicles
    //////////////////////////////////////////////////////////////////////////
    CMSG_MOVE_SET_VEHICLE_REC_ID_ACK            = 0x0000, ///<
    CMSG_PLAYER_VEHICLE_ENTER                   = 0x1ED0, ///< 6.1.2 19783
    CMSG_REQUEST_VEHICLE_EXIT                   = 0x054D, ///< 6.1.2 19783
    CMSG_REQUEST_VEHICLE_NEXT_SEAT              = 0x0FA3, ///< 6.1.2 19783
    CMSG_REQUEST_VEHICLE_PREV_SEAT              = 0x1E2A, ///< 6.1.2 19783
    CMSG_REQUEST_VEHICLE_SWITCH_SEAT            = 0x0E7C, ///< 6.1.2 19783
    CMSG_SET_VEHICLE_REC_ID_ACK                 = 0x0000, ///<
    CMSG_CHANGE_SEATS_ON_CONTROLLED_VEHICLE     = 0x0404, ///< 6.1.2 19783
    CMSG_EJECT_PASSENGER                        = 0x1AD0, ///< 6.1.2 19783
    CMSG_DISMISS_CONTROLLED_VEHICLE             = 0x085F, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Vendors
    //////////////////////////////////////////////////////////////////////////
    CMSG_LIST_INVENTORY                         = 0x1922, ///< 6.1.2 19783
    CMSG_REPAIR_ITEM                            = 0x19A2, ///< 6.1.2 19783
    CMSG_BUYBACK_ITEM                           = 0x1E84, ///< 6.1.2 19783
    CMSG_BUY_ITEM                               = 0x1CE5, ///< 6.1.2 19783
    CMSG_SELL_ITEM                              = 0x1931, ///< 6.1.2 19783
    CMSG_ITEM_REFUND                            = 0x08B2, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Taxi
    //////////////////////////////////////////////////////////////////////////
    CMSG_SET_TAXI_BENCHMARK_MODE                = 0x0CF6, ///< 6.1.2 19783
    CMSG_ENABLE_TAXI_NODE                       = 0x0926, ///< 6.1.2 19783
    CMSG_TAXI_HELLO                             = 0x0E02, ///< 6.1.2 19783
    CMSG_ACTIVATE_TAXI                          = 0x1E83, ///< 6.1.2 19783
    CMSG_TAXI_NODE_STATUS_QUERY                 = 0x0CF1, ///< 6.1.2 19783
    CMSG_TAXI_REQUEST_EARLY_LANDING             = 0x0DE6, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Quest
    //////////////////////////////////////////////////////////////////////////
    CMSG_QUESTGIVER_HELLO                       = 0x01CE, ///< 6.1.2 19783
    CMSG_QUEST_GIVER_STATUS_QUERY               = 0x0836, ///< 6.1.2 19783
    CMSG_QUESTGIVER_STATUS_MULTIPLE_QUERY       = 0x0DA5, ///< 6.1.2 19783
    CMSG_QUESTGIVER_QUERY_QUEST                 = 0x0DA6, ///< 6.1.2 19783
    CMSG_QUESTGIVER_COMPLETE_QUEST              = 0x0CE6, ///< 6.1.2 19783
    CMSG_QUESTGIVER_CHOOSE_REWARD               = 0x0DA2, ///< 6.1.2 19783
    CMSG_QUESTGIVER_ACCEPT_QUEST                = 0x1865, ///< 6.1.2 19783
    CMSG_QUESTGIVER_REQUEST_REWARD              = 0x19F6, ///< 6.1.2 19783
    CMSG_QUEST_CONFIRM_ACCEPT                   = 0x08A1, ///< 6.1.2 19783
    CMSG_QUESTLOG_REMOVE_QUEST                  = 0x1DE1, ///< 6.1.2 19783
    CMSG_PUSH_QUEST_TO_PARTY                    = 0x0DE1, ///< 6.1.2 19783
    CMSG_QUEST_PUSH_RESULT                      = 0x1F81, ///< 6.1.2 19783
    CMSG_QUERY_QUEST_COMPLETION_NPCS            = 0x0489, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Account data
    //////////////////////////////////////////////////////////////////////////
    CMSG_GET_UNDELETE_CHARACTER_COOLDOWN_STATUS = 0x063D, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Chat
    //////////////////////////////////////////////////////////////////////////
    /// Addon chat
    CMSG_CHAT_ADDON_MESSAGE_INSTANCE_CHAT       = 0x1D9A, ///< 6.1.2 19783
    CMSG_CHAT_ADDON_MESSAGE_WHISPER             = 0x10FD, ///< 6.1.2 19783
    CMSG_CHAT_ADDON_MESSAGE_GUILD               = 0x1C3D, ///< 6.1.2 19783
    CMSG_CHAT_ADDON_MESSAGE_OFFICER             = 0x1C8D, ///< 6.1.2 19783
    CMSG_CHAT_ADDON_MESSAGE_RAID                = 0x10F9, ///< 6.1.2 19783
    CMSG_CHAT_ADDON_MESSAGE_PARTY               = 0x1D89, ///< 6.1.2 19783
    CMSG_ADDON_REGISTERED_PREFIXES              = 0x1D2A, ///< 6.1.2 19783

    /// Chat
    CMSG_CHAT_MESSAGE_RAID_WARNING              = 0x147A, ///< 6.1.2 19783
    CMSG_CHAT_MESSAGE_PARTY                     = 0x14BA, ///< 6.1.2 19783
    CMSG_CHAT_MESSAGE_YELL                      = 0x1CB9, ///< 6.1.2 19783
    CMSG_CHAT_MESSAGE_SAY                       = 0x192A, ///< 6.1.2 19783
    CMSG_CHAT_MESSAGE_OFFICER                   = 0x155A, ///< 6.1.2 19783
    CMSG_CHAT_MESSAGE_EMOTE                     = 0x1DAA, ///< 6.1.2 19783
    CMSG_CHAT_MESSAGE_AFK                       = 0x185E, ///< 6.1.2 19783
    CMSG_CHAT_MESSAGE_DND                       = 0x183E, ///< 6.1.2 19783
    CMSG_CHAT_MESSAGE_GUILD                     = 0x14E9, ///< 6.1.2 19783
    CMSG_CHAT_MESSAGE_RAID                      = 0x1CAA, ///< 6.1.2 19783
    CMSG_CHAT_MESSAGE_WHISPER                   = 0x103A, ///< 6.1.2 19783
    CMSG_CHAT_MESSAGE_CHANNEL                   = 0x1D8A, ///< 6.1.2 19783

    /// Channel
    CMSG_JOIN_CHANNEL                           = 0x152A, ///< 6.1.2 19783
    CMSG_CHANNEL_ANNOUNCEMENTS                  = 0x11B9, ///< 6.1.2 19783
    CMSG_CHANNEL_BAN                            = 0x187E, ///< 6.1.2 19783
    CMSG_CHANNEL_DISPLAY_LIST                   = 0x0000, ///< 6.1.2 19783
    CMSG_CHANNEL_INVITE                         = 0x142D, ///< 6.1.2 19783
    CMSG_CHANNEL_KICK                           = 0x153D, ///< 6.1.2 19783
    CMSG_CHANNEL_LIST                           = 0x1999, ///< 6.1.2 19783
    CMSG_CHANNEL_MODERATOR                      = 0x152D, ///< 6.1.2 19783
    CMSG_CHANNEL_MUTE                           = 0x1909, ///< 6.1.2 19783
    CMSG_CHANNEL_OWNER                          = 0x15ED, ///< 6.1.2 19783
    CMSG_CHANNEL_PASSWORD                       = 0x193A, ///< 6.1.2 19783
    CMSG_CHANNEL_SET_OWNER                      = 0x14CA, ///< 6.1.2 19783
    CMSG_CHANNEL_SILENCE_ALL                    = 0x141E, ///< 6.1.2 19783
    CMSG_CHANNEL_SILENCE_VOICE                  = 0x148E, ///< 6.1.2 19783
    CMSG_CHANNEL_UNBAN                          = 0x155D, ///< 6.1.2 19783
    CMSG_CHANNEL_UNMODERATOR                    = 0x10AE, ///< 6.1.2 19783
    CMSG_CHANNEL_UNMUTE                         = 0x109A, ///< 6.1.2 19783
    CMSG_CHANNEL_UNSILENCE_ALL                  = 0x11FE, ///< 6.1.2 19783
    CMSG_CHANNEL_UNSILENCE_VOICE                = 0x10FE, ///< 6.1.2 19783
    CMSG_CHANNEL_VOICE_OFF                      = 0x103E, ///< 6.1.2 19783
    CMSG_CHANNEL_VOICE_ON                       = 0x1419, ///< 6.1.2 19783
    CMSG_DECLINE_CHANNEL_INVITE                 = 0x10D9, ///< 6.1.2 19783
    CMSG_LEAVE_CHANNEL                          = 0x113D, ///< 6.1.2 19783
    CMSG_SET_ACTIVE_VOICE_CHANNEL               = 0x0298, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Movement
    //////////////////////////////////////////////////////////////////////////
    /// Start
    CMSG_MOVE_START_FORWARD                     = 0x0004, ///< 6.1.2 19783
    CMSG_MOVE_START_TURN_LEFT                   = 0x0918, ///< 6.1.2 19783
    CMSG_MOVE_START_TURN_RIGHT                  = 0x094B, ///< 6.1.2 19783
    CMSG_MOVE_START_BACKWARD                    = 0x0147, ///< 6.1.2 19783
    CMSG_MOVE_START_STRAFE_LEFT                 = 0x0844, ///< 6.1.2 19783
    CMSG_MOVE_START_STRAFE_RIGHT                = 0x0957, ///< 6.1.2 19783
    CMSG_MOVE_START_ASCEND                      = 0x0510, ///< 6.1.2 19783
    CMSG_MOVE_START_DESCEND                     = 0x0117, ///< 6.1.2 19783
    CMSG_MOVE_START_PITCH_UP                    = 0x0450, ///< 6.1.2 19783
    CMSG_MOVE_START_PITCH_DOWN                  = 0x004B, ///< 6.1.2 19783
    CMSG_MOVE_START_SWIM                        = 0x0157, ///< 6.1.2 19783

    /// Misc
    CMSG_MOVE_JUMP                              = 0x0158, ///< 6.1.2 19783
    CMSG_MOVE_FALL_LAND                         = 0x095F, ///< 6.1.2 19783
    CMSG_WORLD_PORT_RESPONSE                    = 0x061D, ///< 6.1.2 19783
    CMSG_MOVE_TIME_SKIPPED                      = 0x0903, ///< 6.1.2 19783
    CMSG_MOVE_SPLINE_DONE                       = 0x0514, ///< 6.1.2 19783

    /// Update
    CMSG_MOVE_HEARTBEAT                         = 0x055C, ///< 6.1.2 19783
    CMSG_MOVE_SET_FACING                        = 0x0803, ///< 6.1.2 19783
    CMSG_MOVE_SET_PITCH                         = 0x080F, ///< 6.1.2 19783
    CMSG_MOVE_SET_RUN_MODE                      = 0x0053, ///< 6.1.2 19783
    CMSG_MOVE_SET_WALK_MODE                     = 0x0843, ///< 6.1.2 19783
    CMSG_MOVE_TELEPORT_ACK                      = 0x0520, ///< 6.1.2 19783
    CMSG_MOVE_CHNG_TRANSPORT                    = 0x044F, ///< 6.1.2 19783

    /// Stop
    CMSG_MOVE_STOP                              = 0x044B, ///< 6.1.2 19783
    CMSG_MOVE_STOP_TURN                         = 0x0854, ///< 6.1.2 19783
    CMSG_MOVE_STOP_STRAFE                       = 0x084B, ///< 6.1.2 19783
    CMSG_MOVE_STOP_SWIM                         = 0x081B, ///< 6.1.2 19783
    CMSG_MOVE_STOP_ASCEND                       = 0x011C, ///< 6.1.2 19783
    CMSG_MOVE_STOP_PITCH                        = 0x045B, ///< 6.1.2 19783

    /// ACK
    CMSG_MOVE_SET_RUN_SPEED_CHEAT               = 0x0407, ///< 6.1.2 19783
    CMSG_MOVE_SET_RUN_BACK_SPEED_CHEAT          = 0x044C, ///< 6.1.2 19783
    CMSG_MOVE_SET_WALK_SPEED_CHEAT              = 0x0813, ///< 6.1.2 19783
    CMSG_MOVE_SET_SWIM_SPEED_CHEAT              = 0x0517, ///< 6.1.2 19783
    CMSG_MOVE_SET_SWIM_BACK_SPEED_CHEAT         = 0x0110, ///< 6.1.2 19783
    CMSG_MOVE_SET_FLIGHT_SPEED_CHEAT            = 0x004F, ///< 6.1.2 19783
    CMSG_MOVE_SET_FLIGHT_BACK_SPEED_CHEAT       = 0x0148, ///< 6.1.2 19783
    CMSG_MOVE_SET_TURN_SPEED_CHEAT              = 0x014C, ///< 6.1.2 19783
    CMSG_MOVE_SET_PITCH_SPEED_CHEAT             = 0x0410, ///< 6.1.2 19783
    CMSG_MOVE_KNOCK_BACK_ACK                    = 0x040F, ///< 6.1.2 19783
    CMSG_MOVE_SET_CAN_FLY_ACK                   = 0x0C03, ///< 6.1.2 19783
    CMSG_MOVE_FEATHER_FALL_ACK                  = 0x0120, ///< 6.1.2 19783
    CMSG_MOVE_WATER_WALK_ACK                    = 0x0C07, ///< 6.1.2 19783
    CMSG_MOVE_HOVER_ACK                         = 0x0860, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Group
    //////////////////////////////////////////////////////////////////////////
    CMSG_PARTY_INVITE                           = 0x12BD, ///< 6.1.2 19783
    CMSG_PARTY_INVITE_RESPONSE                  = 0x16BF, ///< 6.1.2 19783
    CMSG_LEAVE_GROUP                            = 0x179E, ///< 6.1.2 19783
    CMSG_PARTY_UNINVITE                         = 0x02B6, ///< 6.1.2 19783
    CMSG_SET_PARTY_LEADER                       = 0x131D, ///< 6.1.2 19783
    CMSG_SET_ROLE                               = 0x0398, ///< 6.1.2 19783
    CMSG_MINIMAP_PING                           = 0x1218, ///< 6.1.2 19783
    CMSG_RANDOM_ROLL                            = 0x179F, ///< 6.1.2 19783
    CMSG_UPDATE_RAID_TARGET                     = 0x17A0, ///< 6.1.2 19783
    CMSG_CONVERT_RAID                           = 0x0A98, ///< 6.1.2 19783
    CMSG_DO_READY_CHECK                         = 0x139E, ///< 6.1.2 19783
    CMSG_READY_CHECK_RESPONSE                   = 0x07B5, ///< 6.1.2 19783
    CMSG_INITIATE_ROLE_POLL                     = 0x071E, ///< 6.1.2 19783
    CMSG_REQUEST_RAID_INFO                      = 0x0A96, ///< 6.1.2 19783
    CMSG_SET_EVERYONE_IS_ASSISTANT              = 0x1716, ///< 6.1.2 19783
    CMSG_CLEAR_RAID_MARKER                      = 0x0FDB, ///< 6.1.2 19783
    CMSG_GROUP_ASSISTANT_LEADER                 = 0x0395, ///< 6.1.2 19783
    CMSG_GROUP_ASSIGNMENT                       = 0x0AB8, ///< 6.1.2 19783
    CMSG_GROUP_CHANGE_SUB_GROUP                 = 0x0AB7, ///< 6.1.2 19783
    CMSG_GROUP_REQUEST_JOIN_UPDATES             = 0x0620, ///< 6.1.2 19783
    CMSG_GROUP_SWAP_SUB_GROUP                   = 0x0F98, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Friend
    //////////////////////////////////////////////////////////////////////////
    CMSG_SEND_CONTACT_LIST                      = 0x0EC0, ///< 6.1.2 19783
    CMSG_ADD_FRIEND                             = 0x039E, ///< 6.1.2 19783
    CMSG_ADD_IGNORE                             = 0x16A0, ///< 6.1.2 19783
    CMSG_SET_CONTACT_NOTES                      = 0x0B3D, ///< 6.1.2 19783
    CMSG_DEL_FRIEND                             = 0x163F, ///< 6.1.2 19783
    CMSG_DEL_IGNORE                             = 0x03C0, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Petition
    //////////////////////////////////////////////////////////////////////////
    CMSG_OFFER_PETITION                         = 0x1A17, ///< 6.1.2 19783
    CMSG_PETITION_BUY                           = 0x1872, ///< 6.1.2 19783
    CMSG_PETITION_DECLINE                       = 0x1D66, ///< 6.1.2 19783
    CMSG_PETITION_RENAME                        = 0x0E37, ///< 6.1.2 19783
    CMSG_PETITION_SHOWLIST                      = 0x0CF5, ///< 6.1.2 19783
    CMSG_PETITION_SHOW_SIGNATURES               = 0x09B6, ///< 6.1.2 19783
    CMSG_PETITION_SIGN                          = 0x18E5, ///< 6.1.2 19783
    CMSG_TURN_IN_PETITION                       = 0x0DF5, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Pet
    //////////////////////////////////////////////////////////////////////////
    CMSG_PET_NAME_QUERY                         = 0x0E30, ///< 6.1.2 19783
    CMSG_PET_RENAME                             = 0x1618, ///< 6.1.2 19783
    CMSG_PET_ABANDON                            = 0x09E1, ///< 6.1.2 19783
    CMSG_PET_ACTION                             = 0x09F5, ///< 6.1.2 19783
    CMSG_PET_CANCEL_AURA                        = 0x12FB, ///< 6.1.2 19783
    CMSG_PET_CAST_SPELL                         = 0x1B0A, ///< 6.1.2 19783
    CMSG_PET_SET_ACTION                         = 0x0C75, ///< 6.1.2 19783
    CMSG_PET_STOP_ATTACK                        = 0x09A6, ///< 6.1.2 19783
    CMSG_LEARN_PET_SPECIALIZATION_GROUP         = 0x06FA, ///< 6.1.2 19783
    CMSG_LIST_STABLE_PETS                       = 0x01CA, ///< 6.1.2 19783
    CMSG_SET_PET_SLOT                           = 0x1B02, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Battlegrounds
    //////////////////////////////////////////////////////////////////////////
    CMSG_BATTLEMASTER_JOIN                      = 0x1D36, ///< 6.1.2 19783
    CMSG_BATTLEMASTER_JOIN_ARENA                = 0x0865, ///< 6.1.2 19783
    CMSG_BATTLEMASTER_JOIN_ARENA_SKIRMISH       = 0x1E01, ///< 6.1.2 19783
    CMSG_JOIN_RATED_BATTLEGROUND                = 0x01AA, ///< 6.1.2 19783
    CMSG_BATTLEFIELD_PORT                       = 0x1D32, ///< 6.1.2 19783
    CMSG_REQUEST_BATTLEFIELD_STATUS             = 0x039D, ///< 6.1.2 19783
    CMSG_BATTLEFIELD_REQUEST_SCORE_DATA         = 0x0E08, ///< 6.1.2 19783
    CMSG_REQUEST_PVP_REWARDS                    = 0x06DC, ///< 6.1.2 19783
    CMSG_REQUEST_PVP_OPTIONS_ENABLED            = 0x029E, ///< 6.1.2 19783
    CMSG_QUERY_COUNTDOWN_TIMER                  = 0x06F1, ///< (unused)
    CMSG_REQUEST_CONQUEST_FORMULA_CONSTANTS     = 0x1B8A, ///< 6.1.2 19783
    CMSG_LEAVE_BATTLEFIELD                      = 0x0272, ///< 6.1.2 19783
    CMSG_SPIRIT_HEALER_ACTIVATE                 = 0x1E8A, ///< 6.1.2 19783
    CMSG_AREA_SPIRIT_HEALER_QUERY               = 0x1825, ///< 6.1.2 19783
    CMSG_AREA_SPIRIT_HEALER_QUEUE               = 0x0E81, ///< 6.1.2 19783
    CMSG_BATTLEFIELD_MGR_QUEUE_REQUEST          = 0x1748, ///< 6.1.2 19783
    CMSG_BATTLEFIELD_MGR_QUEUE_INVITE_RESPONSE  = 0x17BE, ///< 6.1.2 19783
    CMSG_BATTLEFIELD_MGR_QUEUE_EXIT_REQUEST     = 0x0F96, ///< 6.1.2 19783
    CMSG_BATTLEFIELD_MGR_ENTRY_INVITE_RESPONSE  = 0x0F36, ///< 6.1.2 19783
    CMSG_BATTLEFIELD_LIST                       = 0x0250, ///< 6.1.2 19783
    CMSG_BATTLEFIELD_LEAVE                      = 0x0000,
    CMSG_REQUEST_RATED_BATTLEFIELD_INFO         = 0x0A40, ///< 6.1.2 19783
    CMSG_START_WAR_GAME                         = 0x12BF, ///< 6.1.2 19783
    CMSG_ACCEPT_WARGAME_INVITE                  = 0x0E3F, ///< 6.1.2 19783
    CMSG_REPORT_PVP_AFK                         = 0x0F0C, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Guild
    //////////////////////////////////////////////////////////////////////////
    CMSG_GUILD_INVITE_BY_NAME                      = 0x0716, ///< 6.1.2 19783
    CMSG_ACCEPT_GUILD_INVITE                       = 0x0B36, ///< 6.1.2 19783
    CMSG_GUILD_DECLINE_INVITATION                  = 0x1967, ///< 6.1.2 19783
    CMSG_GUILD_OFFICER_REMOVE_MEMBER               = 0x1928, ///< 6.1.2 19783
    CMSG_GUILD_SET_GUILD_MASTER                    = 0x1316, ///< 6.1.2 19783
    CMSG_GUILD_LEAVE                               = 0x1168, ///< 6.1.2 19783
    CMSG_GUILD_DELETE                              = 0x1473, ///< 6.1.2 19783
    CMSG_GUILD_UPDATE_MOTD_TEXT                    = 0x1C08, ///< 6.1.2 19783
    CMSG_GUILD_UPDATE_INFO_TEXT                    = 0x1507, ///< 6.1.2 19783
    CMSG_GUILD_GET_RANKS                           = 0x1533, ///< 6.1.2 19783
    CMSG_GUILD_ADD_RANK                            = 0x1953, ///< 6.1.2 19783
    CMSG_GUILD_DELETE_RANK                         = 0x1864, ///< 6.1.2 19783
    CMSG_GUILD_SET_RANK_PERMISSIONS                = 0x1053, ///< 6.1.2 19783
    CMSG_GUILD_SHIFT_RANK                          = 0x1458, ///< 6.1.2 19783
    CMSG_GUILD_ASSIGN_MEMBER_RANK                  = 0x1844, ///< 6.1.2 19783
    CMSG_GUILD_GET_ROSTER                          = 0x1434, ///< 6.1.2 19783
    CMSG_GUILD_BANK_ACTIVATE                       = 0x0DE2, ///< 6.1.2 19783
    CMSG_GUILD_BANK_BUY_TAB                        = 0x0F09, ///< 6.1.2 19783
    CMSG_GUILD_BANK_DEPOSIT_MONEY                  = 0x0832, ///< 6.1.2 19783
    CMSG_GUILD_BANK_LOG_QUERY                      = 0x1973, ///< 6.1.2 19783
    CMSG_GUILD_BANK_REMAINING_WITHDRAW_MONEY_QUERY = 0x1417, ///< 6.1.2 19783
    CMSG_GUILD_BANK_QUERY_TAB                      = 0x1DE5, ///< 6.1.2 19783
    CMSG_GUILD_BANK_TEXT_QUERY                     = 0x1914, ///< 6.1.2 19783
    CMSG_GUILD_BANK_SWAP_ITEMS                     = 0x1C75, ///< 6.1.2 19783
    CMSG_GUILD_BANK_UPDATE_TAB                     = 0x1F09, ///< 6.1.2 19783
    CMSG_GUILD_BANK_WITHDRAW_MONEY                 = 0x1D61, ///< 6.1.2 19783
    CMSG_GUILD_BANK_SET_TAB_TEXT                   = 0x1917, ///< 6.1.2 19783
    CMSG_GUILD_GET_ACHIEVEMENT_MEMBERS             = 0x0000, ///< (unused)
    CMSG_GUILD_SET_FOCUSED_ACHIEVEMENT             = 0x1167, ///< 6.1.2 19783
    CMSG_GUILD_SET_ACHIEVEMENT_TRACKING            = 0x0000, ///< (unused)
    CMSG_GUILD_SET_MEMBER_NOTE                     = 0x1427, ///< 6.1.2 19783
    CMSG_GUILD_CHALLENGE_UPDATE_REQUEST            = 0x1817, ///< 6.1.2 19783
    CMSG_REQUEST_GUILD_PARTY_STATE                 = 0x0B5B, ///< 6.1.2 19783
    CMSG_REQUEST_GUILD_REWARDS_LIST                = 0x0F2F, ///< 6.1.2 19783
    CMSG_GUILD_REPLACE_GUILD_MASTER                = 0x1923, ///< 6.1.2 19783
    CMSG_GUILD_CHANGE_NAME_REQUEST                 = 0x0000, ///<  (unused)
    CMSG_GUILD_PERMISSIONS_QUERY                   = 0x1878, ///< 6.1.2 19783
    CMSG_GUILD_EVENT_LOG_QUERY                     = 0x1933, ///< 6.1.2 19783
    CMSG_GUILD_NEWS_UPDATE_STICKY                  = 0x1538, ///< 6.1.2 19783
    CMSG_GUILD_QUERY_NEWS                          = 0x1573, ///< 6.1.2 19783
    CMSG_QUERY_GUILD_MEMBERS_FOR_RECIPE            = 0x0000, ///< (unused)
    CMSG_QUERY_GUILD_MEMBER_RECIPES                = 0x0000, ///< (unused)
    CMSG_QUERY_GUILD_RECIPES                       = 0x1C13, ///< 6.1.2 19783
    CMSG_PLAYER_SAVE_GUILD_EMBLEM                  = 0x0B0C, ///< 6.1.2 19783

    /// Guild finding
    CMSG_LF_GUILD_ADD_RECRUIT                      = 0x0B9F, ///< 6.1.2 19783
    CMSG_LF_GUILD_BROWSE                           = 0x1A37, ///< 6.1.2 19783
    CMSG_LF_GUILD_DECLINE_RECRUIT                  = 0x17B6, ///< 6.1.2 19783
    CMSG_LF_GUILD_GET_APPLICATIONS                 = 0x1548, ///< 6.1.2 19783
    CMSG_LF_GUILD_GET_RECRUITS                     = 0x1464, ///< 6.1.2 19783
    CMSG_LF_GUILD_POST_REQUEST                     = 0x0000, ///<
    CMSG_LF_GUILD_REMOVE_RECRUIT                   = 0x1803, ///< 6.1.2 19783
    CMSG_LF_GUILD_SET_GUILD_POST                   = 0x121F, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Battle pet opcodes
    //////////////////////////////////////////////////////////////////////////
    CMSG_SUMMON_COMPANION                          = 0x0A9D,   ///< 6.1.2 19783
    CMSG_PETBATTLE_QUERY_JOURNAL                   = 0x0F37,   ///< 6.1.2 19783
    CMSG_PETBATTLE_SET_ABILITY                     = 0x16BD,   ///< 6.1.2 19783
    CMSG_PETBATTLE_RENAME                          = 0x0B37,   ///< 6.1.2 19783
    CMSG_PETBATTLE_CAGE_PET                        = 0x0000,
    CMSG_PETBATTLE_QUERY_NAME                      = 0x0AFC,   ///< 6.1.2 19783
    CMSG_PETBATTLE_REQUEST_WILD                    = 0x0000,
    CMSG_PETBATTLE_REQUEST_PVP                     = 0x0000,
    CMSG_PETBATTLE_JOIN_QUEUE                      = 0x0000,
    CMSG_PETBATTLE_REQUEST_UPDATE                  = 0x0000,
    CMSG_PETBATTLE_CANCEL_REQUEST_PVP_MATCHMAKING  = 0x0000,
    CMSG_PETBATTLE_INPUT                           = 0x0000,
    CMSG_PETBATTLE_INPUT_NEW_FRONT_PET             = 0x0000,
    CMSG_BATTLEPET_SET_BATTLESLOT                  = 0x120B,

    //////////////////////////////////////////////////////////////////////////
    /// Battle pay
    //////////////////////////////////////////////////////////////////////////
    CMSG_BATTLE_PAY_GET_PURCHASE_LIST              = 0x1616,    ///< 6.1.2 19783
    CMSG_BATTLE_PAY_GET_PRODUCT_LIST_QUERY         = 0x0A1D,    ///< 6.1.2 19783
    CMSG_BATTLE_PAY_START_PURCHASE                 = 0x173F,    ///< 6.1.2 19783
    CMSG_BATTLE_PAY_CONFIRM_PURCHASE_RESPONSE      = 0x1A1F,    ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// LFG
    //////////////////////////////////////////////////////////////////////////
    CMSG_DFGET_SYSTEM_INFO                         = 0x1298, ///< 6.1.2 19783
    CMSG_LFG_GET_PLAYER_INFO                       = 0x0000, 
    CMSG_LFG_GET_STATUS                            = 0x1236, ///< 6.1.2 19783
    CMSG_LFG_JOIN                                  = 0x1237, ///< 6.1.2 19783
    CMSG_LFG_LEAVE                                 = 0x029D, ///< 6.1.2 19783
    CMSG_LFG_PROPOSAL_RESULT                       = 0x0795, ///< 6.1.2 19783
    CMSG_LFG_SET_BOOT_VOTE                         = 0x0EBF, ///< 6.1.2 19783
    CMSG_LFG_SET_COMMENT                           = 0x0000, ///< (unused)
    CMSG_DFSET_ROLES                               = 0x0297, ///< 6.1.2 19783
    CMSG_LFG_TELEPORT                              = 0x0FB7, ///< 6.1.2 19783
    CMSG_SEARCH_LFG_JOIN                           = 0x0000, ///< (unused)
    CMSG_SEARCH_LFG_LEAVE                          = 0x0000, ///< (unused)
    CMSG_RESET_INSTANCES                           = 0x0A97, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// LFG LIST
    //////////////////////////////////////////////////////////////////////////
    CMSG_LFG_LIST_JOIN                              = 0x13B6, ///< 6.1.2 19783
    CMSG_LFG_LIST_UPDATE_REQUEST                    = 0x12B7, ///< 6.1.2 19783
    CMSG_LFG_LIST_LEAVE                             = 0x0B20, ///< 6.1.2 19783
    CMSG_LFG_LIST_SEARCH                            = 0x1A1D, ///< 6.1.2 19783
    CMSG_REQUEST_LFG_LIST_BLACKLIST                 = 0x0A02, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Auction House
    //////////////////////////////////////////////////////////////////////////
    CMSG_AUCTION_HELLO                             = 0x1F82, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Trainers
    //////////////////////////////////////////////////////////////////////////
    CMSG_TRAINER_LIST                              = 0x0D21, ///< 6.1.2 19783
    CMSG_TRAINER_BUY_SPELL                         = 0x0921, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Void storage
    //////////////////////////////////////////////////////////////////////////
    CMSG_VOID_STORAGE_QUERY                        = 0x03D3, ///< 6.1.2 19783
    CMSG_VOID_STORAGE_TRANSFER                     = 0x0E07, ///< 6.1.2 19783
    CMSG_VOID_STORAGE_UNLOCK                       = 0x0AA1, ///< 6.1.2 19783
    CMSG_VOID_SWAP_ITEM                            = 0x0B02, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Transmogrification
    //////////////////////////////////////////////////////////////////////////
    CMSG_TRANSMOGRIFY_ITEMS                        = 0x03F1, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Mail
    //////////////////////////////////////////////////////////////////////////
    CMSG_GET_MAIL_LIST                             = 0x1F04, ///< 6.1.2 19783
    CMSG_MAIL_CREATE_TEXT_ITEM                     = 0x0149, ///< 6.1.2 19783
    CMSG_MAIL_DELETE                               = 0x0A73, ///< 6.1.2 19783
    CMSG_MAIL_MARK_AS_READ                         = 0x1C31, ///< 6.1.2 19783
    CMSG_MAIL_RETURN_TO_SENDER                     = 0x02A0, ///< 6.1.2 19783
    CMSG_MAIL_TAKE_ITEM                            = 0x0975, ///< 6.1.2 19783
    CMSG_MAIL_TAKE_MONEY                           = 0x0871, ///< 6.1.2 19783
    CMSG_SEND_MAIL                                 = 0x0240, ///< 6.1.2 19783
    CMSG_QUERY_NEXT_MAIL_TIME                      = 0x08B6, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Time
    //////////////////////////////////////////////////////////////////////////
    CMSG_UITIME_REQUEST                            = 0x0316, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// GameMasters
    //////////////////////////////////////////////////////////////////////////
    CMSG_GMRESPONSE_RESOLVE                         = 0x0217, ///< 6.1.2 19783
    CMSG_GMSURVEY_SUBMIT                            = 0x0E15, ///< 6.1.2 19783
    CMSG_GMTICKET_CREATE                            = 0x0A1E, ///< 6.1.2 19783
    CMSG_GMTICKET_DELETETICKET                      = 0x129E, ///< 6.1.2 19783
    CMSG_GMTICKET_GETTICKET                         = 0x0717, ///< 6.1.2 19783
    CMSG_GMTICKET_GET_CASE_STATUS                   = 0x1A3F, ///< 6.1.2 19783
    CMSG_GMTICKET_SYSTEMSTATUS                      = 0x0A18, ///< 6.1.2 19783
    CMSG_GMTICKET_UPDATETEXT                        = 0x13A0, ///< 6.1.2 19783
    CMSG_GM_REPORT_LAG                              = 0x0B3F, ///< 6.1.2 19783
    CMSG_REPORT_BUG                                 = 0x0B96, ///< 6.1.2 19783
    CMSG_REPORT_SUGGESTION                          = 0x1A16, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// ToyBox
    //////////////////////////////////////////////////////////////////////////
    CMSG_ADD_NEW_TOY_TO_BOX                         = 0x1E48, ///< 6.1.2 19783
    CMSG_SET_FAVORITE_TOY                           = 0x02BE, ///< 6.1.2 19783
    CMSG_USE_TOY                                    = 0x0B48, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Calendar
    //////////////////////////////////////////////////////////////////////////
    CMSG_CALENDAR_ADD_EVENT                         = 0x0320, ///< 6.1.2 19783
    CMSG_CALENDAR_COMPLAIN                          = 0x1395, ///< 6.1.2 19783
    CMSG_CALENDAR_COPY_EVENT                        = 0x123F, ///< 6.1.2 19783
    CMSG_CALENDAR_EVENT_INVITE                      = 0x02B5, ///< 6.1.2 19783
    CMSG_CALENDAR_EVENT_MODERATOR_STATUS            = 0x03B8, ///< 6.1.2 19783
    CMSG_CALENDAR_EVENT_REMOVE_INVITE               = 0x16B8, ///< 6.1.2 19783
    CMSG_CALENDAR_EVENT_RSVP                        = 0x021E, ///< 6.1.2 19783
    CMSG_CALENDAR_EVENT_SIGNUP                      = 0x0F3F, ///< 6.1.2 19783
    CMSG_CALENDAR_EVENT_STATUS                      = 0x13B8, ///< 6.1.2 19783
    CMSG_CALENDAR_GET_CALENDAR                      = 0x1A38, ///< 6.1.2 19783
    CMSG_CALENDAR_GET_EVENT                         = 0x13BD, ///< 6.1.2 19783
    CMSG_CALENDAR_GET_NUM_PENDING                   = 0x0BB6, ///< 6.1.2 19783
    CMSG_CALENDAR_GUILD_FILTER                      = 0x0000, ///<
    CMSG_CALENDAR_REMOVE_EVENT                      = 0x16B8, ///< 6.1.2 19783
    CMSG_CALENDAR_UPDATE_EVENT                      = 0x0F1E, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Challenges
    //////////////////////////////////////////////////////////////////////////
    CMSG_CHALLENGE_MODE_REQUEST_LEADERS             = 0x1017, ///< 6.1.2 19783
    CMSG_GET_CHALLENGE_MODE_REWARDS                 = 0x1118, ///< 6.1.2 19783
    CMSG_CHALLENGE_MODE_REQUEST_MAP_STATS           = 0x1503, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// BlackMarket
    //////////////////////////////////////////////////////////////////////////
    CMSG_BLACK_MARKET_OPEN                         = 0x0F84, ///< 6.1.2 19783
    CMSG_BLACK_MARKET_REQUEST_ITEMS                = 0x016D, ///< 6.1.2 19783
    CMSG_BLACK_MARKET_PLACE_BID                    = 0x1822, ///< 6.1.2 19783

    //////////////////////////////////////////////////////////////////////////
    /// Twitter
    //////////////////////////////////////////////////////////////////////////
    CMSG_REQUEST_TWITTER_STATUS                     = 0x0CDE, ///< 6.1.2 19783

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
    CMSG_COMPLAIN                                       = 0x0000,
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
    CMSG_RANDOMIZE_CHAR_NAME                            = 0x0000,
    CMSG_REALM_SPLIT                                    = 0x0000,
    CMSG_REDIRECTION_AUTH_PROOF                         = 0x0000,
    CMSG_REQUEST_BATTLEPET_JOURNAL                      = 0x0000,
    CMSG_REQUEST_CATEGORY_COOLDOWNS                     = 0x0000,
    CMSG_REQUEST_GM_TICKET                              = 0x0000,
    CMSG_REQUEST_PARTY_MEMBER_STATS                     = 0x0000,
    CMSG_REQUEST_RATED_BG_INFO                          = 0x0000,
    CMSG_REQUEST_RATED_BG_STATS                         = 0x0000,
    CMSG_RESET_CHALLENGE_MODE                           = 0x0000,
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
    CMSG_SUSPEND_TOKEN                                  = 0x0000,
    CMSG_SYNC_DANCE                                     = 0x0000,
    CMSG_TELEPORT_TO_UNIT                               = 0x0000,
    CMSG_TIME_ADJUSTMENT_RESPONSE                       = 0x0554, ///< 6.1.2 19783
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
    CMSG_WHOIS                                          = 0x17BF,
    CMSG_WORLD_TELEPORT                                 = 0x0000,
    CMSG_WRAP_ITEM                                      = 0x0000,
    CMSG_ARENA_2v2_STATS_REQUEST                        = 0x0000,
    CMSG_ARENA_3v3_STATS_REQUEST                        = 0x0000,

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
    SMSG_PET_BATTLE_CHAT_RESTRICTED                     = 0x0000,
    SMSG_PET_BATTLE_DEBUG_QUEUE_DUMP_RESPONSE           = 0x0000,
    SMSG_PET_BATTLE_FINAL_ROUND                         = 0x0000,
    SMSG_PET_BATTLE_FINISHED                            = 0x0000,
    SMSG_PET_BATTLE_FIRST_ROUND                         = 0x0000,
    SMSG_PET_BATTLE_QUEUE_PROPOSE_MATCH                 = 0x0000,
    SMSG_PET_BATTLE_QUEUE_STATUS                        = 0x0000,
    SMSG_PET_BATTLE_ROUND_RESULT                        = 0x0000,
    SMSG_PET_BATTLE_REPLACEMENTS_MADE                   = 0x0000,
    SMSG_PET_BATTLE_REQUEST_FAILED                      = 0x0000,
    SMSG_PET_BATTLE_SLOT_UPDATE                         = 0x0000,
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
    SMSG_BATTLEPET_JOURNAL                              = 0x0000,
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
    SMSG_DISPLAY_GAME_ERROR                             = 0x0000,
    SMSG_DONT_AUTO_PUSH_SPELLS_TO_ACTION_BAR            = 0x0000,
    SMSG_DUMP_RIDE_TICKETS_RESPONSE                     = 0x0000,
    SMSG_FAILED_PLAYER_CONDITION                        = 0x0000,
    SMSG_FORCED_DEATH_UPDATE                            = 0x0000,
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
    SMSG_NOTIFY_MONEY                                   = 0x0B1E, // 6.1.2 19783
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
    SMSG_GENERATE_RANDOM_CHARACTER_NAME_RESULT          = 0x0000,
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

class WorldPacket;
class WorldSession;

typedef void(WorldSession::*g_OpcodeHandlerType)(WorldPacket& recvPacket);

struct OpcodeHandler
{
    OpcodeHandler() {}
    OpcodeHandler(char const* _name, SessionStatus _status, PacketProcessing _processing, g_OpcodeHandlerType _handler)
        : name(_name), status(_status), packetProcessing(_processing), handler(_handler) {}

    char const* name;
    SessionStatus status;
    PacketProcessing packetProcessing;
    g_OpcodeHandlerType handler;
};

extern OpcodeHandler* g_OpcodeTable[TRANSFER_DIRECTION_MAX][NUM_OPCODE_HANDLERS];
void InitOpcodes();

// Lookup opcode name for human understandable logging
inline std::string GetOpcodeNameForLogging(Opcodes id, int p_Direction)
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

#endif
/// @}
