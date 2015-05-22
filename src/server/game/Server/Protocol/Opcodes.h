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
        SMSG_AUTH_CHALLENGE                         = 0x1102, ///< 6.2.0 20033
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Dispatch
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamDispatch
        /// Base opcodes
        SMSG_AUTH_RESPONSE                          = 0x0403, ///< 6.2.0 20033
        SMSG_CACHE_VERSION                          = 0x02C3, ///< 6.2.0 20033
        SMSG_ADDON_INFO                             = 0x0AB6, ///< 6.2.0 20033
        SMSG_FEATURE_SYSTEM_STATUS                  = 0x0090, ///< 6.2.0 20033
        SMSG_SET_TIME_ZONE_INFORMATION              = 0x068F, ///< 6.2.0 20033
        SMSG_LOGIN_VERIFY_WORLD                     = 0x0CD3, ///< 6.2.0 20033
        SMSG_NEW_WORLD                              = 0x0026, ///< 6.2.0 20033
        SMSG_LOGIN_SET_TIME_SPEED                   = 0x02A0, ///< 6.2.0 20033
        SMSG_HOTFIX_NOTIFY_BLOB                     = 0x0095, ///< 6.2.0 20033
        SMSG_HOTFIX_NOTIFY                          = 0x0AA1, ///< 6.2.0 20033
        SMSG_WORLD_SERVER_INFO                      = 0x0626, ///< 6.2.0 20033
        SMSG_INITIAL_SETUP                          = 0x02CC, ///< 6.2.0 20033
        SMSG_PONG                                   = 0x0000, ///< Autoparser error - Opcode not found. Old Opcode: 0x005E
        SMSG_UITIME                                 = 0x0A52, ///< 6.2.0 20033

        /// Query
        SMSG_DB_REPLY                               = 0x0C5F, ///< 6.2.0 20033
        SMSG_QUERY_CREATURE_RESPONSE                = 0x0331, ///< 6.2.0 20033
        SMSG_NPC_TEXT_UPDATE                        = 0x00A2, ///< 6.2.0 20033
        SMSG_NAME_QUERY_RESPONSE                    = 0x0A5C, ///< 6.2.0 20033
        SMSG_REALM_QUERY_RESPONSE                   = 0x0222, ///< 6.2.0 20033
        SMSG_GAMEOBJECT_QUERY_RESPONSE              = 0x0022, ///< 6.2.0 20033
        SMSG_QUERY_TIME_RESPONSE                    = 0x0232, ///< 6.2.0 20033

        /// Misc
        SMSG_UPDATE_ACTION_BUTTONS                  = 0x0A0B, ///< 6.2.0 20033
        SMSG_SET_PROFICIENCY                        = 0x06C7, ///< 6.2.0 20033
        SMSG_INIT_WORLD_STATES                      = 0x0C88, ///< 6.2.0 20033
        SMSG_UPDATE_WORLD_STATE                     = 0x0897, ///< 6.2.0 20033
        SMSG_EMOTE                                  = 0x0A8F, ///< 6.2.0 20033
        SMSG_EXPLORATION_EXPERIENCE                 = 0x01A5, ///< 6.2.0 20033
        SMSG_LOG_XP_GAIN                            = 0x0A91, ///< 6.2.0 20033
        SMSG_LEVELUP_INFO                           = 0x0ECB, ///< 6.2.0 20033
        SMSG_ITEM_PUSH_RESULT                       = 0x0690, ///< 6.2.0 20033
        SMSG_INIT_CURRENCY                          = 0x0B45, ///< 6.2.0 20033
        SMSG_UPDATE_CURRENCY                        = 0x0214, ///< 6.2.0 20033
        SMSG_UPDATE_CURRENCY_WEEK_LIMIT             = 0x0853, ///< 6.2.0 20033
        SMSG_WEEKLY_RESET_CURRENCY                  = 0x0D26, ///< 6.2.0 20033
        SMSG_EQUIPMENT_SET_LIST                     = 0x0C4C, ///< 6.2.0 20033
        SMSG_BINDER_CONFIRM                         = 0x0A16, ///< 6.2.0 20033
        SMSG_BIND_POINT_UPDATE                      = 0x00C4, ///< 6.2.0 20033
        SMSG_UPDATE_TALENT_DATA                     = 0x0057, ///< 6.2.0 20033
        SMSG_TRANSFER_ABORTED                       = 0x061F, ///< 6.2.0 20033
        SMSG_TRANSFER_PENDING                       = 0x05B5, ///< 6.2.0 20033
        SMSG_INVENTORY_CHANGE_FAILURE               = 0x0F32, ///< 6.2.0 20033
        SMSG_SET_DUNGEON_DIFFICULTY                 = 0x07A1, ///< 6.2.0 20033
        SMSG_SET_RAID_DIFFICULTY                    = 0x09A2, ///< 6.2.0 20033
        SMSG_LOAD_CUF_PROFILES                      = 0x0848, ///< 6.2.0 20033
        SMSG_STANDSTATE_UPDATE                      = 0x08D3, ///< 6.2.0 20033
        SMSG_START_TIMER                            = 0x0F26, ///< 6.2.0 20033
        SMSG_START_ELAPSED_TIMER                    = 0x0498, ///< 6.2.0 20033
        SMSG_START_ELAPSED_TIMERS                   = 0x048F, ///< 6.2.0 20033
        SMSG_STOP_ELAPSED_TIMER                     = 0x0A8C, ///< 6.2.0 20033
        SMSG_DEATH_RELEASE_LOC                      = 0x0A0C, ///< 6.2.0 20033
        SMSG_CORPSE_RECLAIM_DELAY                   = 0x029F, ///< 6.2.0 20033
        SMSG_CORPSE_LOCATION                        = 0x0A44, ///< 6.2.0 20033
        SMSG_CORPSE_TRANSPORT_QUERY                 = 0x00B1, ///< 6.2.0 20033
        SMSG_DURABILITY_DAMAGE_DEATH                = 0x04B6, ///< 6.2.0 20033
        SMSG_PLAY_MUSIC                             = 0x0A42, ///< 6.2.0 20033
        SMSG_PLAY_OBJECT_SOUND                      = 0x0C97, ///< 6.2.0 20033
        SMSG_PLAY_SOUND                             = 0x08CC, ///< 6.2.0 20033
        SMSG_DISPLAY_TOAST                          = 0x0B25, ///< 6.2.0 20033
        SMSG_ITEM_ENCHANT_TIME_UPDATE               = 0x00A0, ///< 6.2.0 20033
        SMSG_ENCHANTMENT_LOG                        = 0x0A4B, ///< 6.2.0 20033
        SMSG_PARTY_KILL_LOG                         = 0x0945, ///< 6.2.0 20033
        SMSG_PROCRESIST                             = 0x0008, ///< 6.2.0 20033
        SMSG_CLEAR_TARGET                           = 0x0C35, ///< 6.2.0 20033
        SMSG_WEATHER                                = 0x0C58, ///< 6.2.0 20033
        SMSG_SET_PHASE_SHIFT                        = 0x0C36, ///< 6.2.0 20033
        SMSG_TRIGGER_CINEMATIC                      = 0x02A5, ///< 6.2.0 20033
        SMSG_TOTEM_CREATED                          = 0x0CA0, ///< 6.2.0 20033
        SMSG_RESPEC_WIPE_CONFIRM                    = 0x0E21, ///< 6.2.0 20033
        SMSG_USE_EQUIPMENT_SET_RESULT               = 0x06DF, ///< 6.2.0 20033
        SMSG_TRADE_STATUS                           = 0x0A9C, ///< 6.2.0 20033
        SMSG_TRADE_UPDATED                          = 0x0EA0, ///< 6.2.0 20033
        SMSG_SHOW_NEUTRAL_PLAYER_FACTION_SELECT_UI  = 0x068C, ///< 6.2.0 20033
        SMSG_NEUTRAL_PLAYER_FACTION_SELECT_RESULT   = 0x0000, ///< (unused)
        SMSG_INSPECT_TALENT                         = 0x01B2, ///< 6.2.0 20033
        SMSG_INSPECT_HONOR_STATS                    = 0x0AC7, ///< 6.2.0 20033
        SMSG_INSPECT_RATED_BG_STATS                 = 0x0C43, ///< 6.2.0 20033
        SMSG_TIME_SYNC_REQUEST                      = 0x1097, ///< 6.2.0 20033
        SMSG_RESUME_TOKEN                           = 0x0E08, ///< 6.2.0 20033
        SMSG_BARBER_SHOP_RESULT                     = 0x00D7, ///< 6.2.0 20033
        SMSG_ENABLE_BARBER_SHOP                     = 0x08D6, ///< 6.2.0 20033
        SMSG_PAGE_TEXT_QUERY_RESPONSE               = 0x089F, ///< 6.2.0 20033
        SMSG_PAGE_TEXT                              = 0x06A2, ///< 6.2.0 20033
        SMSG_READ_ITEM_FAILED                       = 0x02DC, ///< 6.2.0 20033
        SMSG_READ_ITEM_OK                           = 0x0296, ///< 6.2.0 20033
        SMSG_TEXT_EMOTE                             = 0x04D3, ///< 6.2.0 20033
        SMSG_TITLE_EARNED                           = 0x0C9C, ///< 6.2.0 20033
        SMSG_TITLE_LOST                             = 0x07B2, ///< 6.2.0 20033
        SMSG_WHOIS                                  = 0x0D31, ///< 6.2.0 20033
        SMSG_TRIGGER_MOVIE                          = 0x0087, ///< 6.2.0 20033
        SMSG_UPDATE_LAST_INSTANCE                   = 0x02D4, ///< 6.2.0 20033
        SMSG_UPDATE_INSTANCE_OWNERSHIP              = 0x0891, ///< 6.2.0 20033
        SMSG_SUMMON_REQUEST                         = 0x041C, ///< 6.2.0 20033
        SMSG_STOP_MIRROR_TIMER                      = 0x0E90, ///< 6.2.0 20033
        SMSG_START_MIRROR_TIMER                     = 0x0B52, ///< 6.2.0 20033
        SMSG_SOCKET_GEMS                            = 0x0E93, ///< 6.2.0 20033
        SMSG_RESURRECT_REQUEST                      = 0x08A6, ///< 6.2.0 20033
        SMSG_RESPOND_INSPECT_ACHIEVEMENTS           = 0x0B32, ///< 6.2.0 20033
        SMSG_RESET_FAILED_NOTIFY                    = 0x0ED7, ///< 6.2.0 20033
        SMSG_SETUP_RESEARCH_HISTORY                 = 0x0325, ///< 6.2.0 20033
        SMSG_RESEARCH_COMPLETE                      = 0x0297, ///< 6.2.0 20033
        SMSG_REQUEST_CEMETERY_LIST_RESPONSE         = 0x00A1, ///< 6.2.0 20033
        SMSG_PLAYED_TIME                            = 0x0660, ///< 6.2.0 20033
        SMSG_BREAK_TARGET                           = 0x01B5, ///< 6.2.0 20033
        SMSG_CANCEL_AUTO_REPEAT                     = 0x0BA5, ///< 6.2.0 20033
        SMSG_CANCEL_COMBAT                          = 0x0B55, ///< 6.2.0 20033
        SMSG_COMPLAIN_RESULT                        = 0x00B5, ///< 6.2.0 20033
        SMSG_XPGAIN_ABORTED                         = 0x01A6, ///< 6.2.0 20033
        SMSG_SUPERCEDED_SPELL                       = 0x023A, ///< 6.2.0 20033
        SMSG_PVP_CREDIT                             = 0x0C84, ///< 6.2.0 20033
        SMSG_PRE_RESURRECT                          = 0x0E8F, ///< 6.2.0 20033
        SMSG_PLAY_ONE_SHOT_ANIM_KIT                 = 0x02A1, ///< 6.2.0 20033
        SMSG_SET_AI_ANIM_KIT                        = 0x084C, ///< 6.2.0 20033
        SMSG_PLAYER_BOUND                           = 0x08A0, ///< 6.2.0 20033
        SMSG_OVERRIDE_LIGHT                         = 0x0C44, ///< 6.2.0 20033
        SMSG_PRINT_NOTIFICATION                     = 0x0E20, ///< 6.2.0 20033
        SMSG_SPECIAL_MOUNT_ANIM                     = 0x0050, ///< 6.2.0 20033
        SMSG_ITEM_TIME_UPDATE                       = 0x0E48, ///< 6.2.0 20033
        SMSG_QUERY_ITEM_TEXT_RESPONSE               = 0x0A25, ///< 6.2.0 20033
        SMSG_ITEM_PURCHASE_REFUND_RESULT            = 0x0047, ///< 6.2.0 20033
        SMSG_GAMEOBJECT_DESPAWN                     = 0x0E88, ///< 6.2.0 20033
        SMSG_GAMEOBJECT_CUSTOM_ANIM                 = 0x0FB2, ///< 6.2.0 20033
        SMSG_GAME_OBJECT_ACTIVATE_ANIM_KIT          = 0x0335, ///< 6.2.0 20033
        SMSG_FISH_NOT_HOOKED                        = 0x004F, ///< 6.2.0 20033
        SMSG_FISH_ESCAPED                           = 0x049B, ///< 6.2.0 20033
        SMSG_DESTRUCTIBLE_BUILDING_DAMAGE           = 0x0AD3, ///< 6.2.0 20033
        SMSG_CROSSED_INEBRIATION_THRESHOLD          = 0x0604, ///< 6.2.0 20033
        SMSG_CLIENT_CONTROL_UPDATE                  = 0x004B, ///< 6.2.0 20033
        SMSG_AREA_TRIGGER_NO_CORPSE                 = 0x07A6, ///< 6.2.0 20033
        SMSG_BAG_SORT_RESULT                        = 0x0A88, ///< 6.2.0 20033
        SMSG_VIGNETTE_UPDATE                        = 0x06D0, ///< 6.2.0 20033
        SMSG_ACCOUNT_MOUNT_UPDATE                   = 0x085C, ///< 6.2.0 20033
        SMSG_PLAY_SCENE                             = 0x0BD6, ///< 6.2.0 20033
        SMSG_CANCEL_SCENE                           = 0x0A15, ///< 6.2.0 20033
        SMSG_TALENTS_INVOLUNTARILY_RESET            = 0x0000, ///< Autoparser error - Opcode not found. Old Opcode: 0x01E3
        SMSG_SET_ITEM_PURCHASE_DATA                 = 0x1085, ///< 6.2.0 20033
        SMSG_SHOW_TRADE_SKILL_RESPONSE              = 0x07A2, ///< 6.2.0 20033

        /// Control Alert
        SMSG_LOSS_OF_CONTROL_AURA_UPDATE            = 0x0CB6, ///< 6.2.0 20033
        SMSG_ADD_LOSS_OF_CONTROL                    = 0x0097, ///< 6.2.0 20033
        SMSG_CLEAR_LOSS_OF_CONTROL                  = 0x0D32, ///< 6.2.0 20033
        SMSG_REMOVE_LOSS_OF_CONTROL                 = 0x0883, ///< 6.2.0 20033

        /// Reputations
        SMSG_INITIALIZE_FACTIONS                    = 0x0017, ///< 6.2.0 20033
        SMSG_FACTION_BONUS_INFO                     = 0x0688, ///< 6.2.0 20033
        SMSG_SET_FACTION_VISIBLE                    = 0x06A0, ///< 6.2.0 20033
        SMSG_SET_FACTION_NOT_VISIBLE                = 0x021C, ///< 6.2.0 20033
        SMSG_SET_FACTION_STANDING                   = 0x0926, ///< 6.2.0 20033
        SMSG_SET_FACTION_ATWAR                      = 0x025C, ///< 6.2.0 20033
        SMSG_SET_FORCED_REACTIONS                   = 0x088F, ///< 6.2.0 20033

        /// Interaction
        SMSG_LOGOUT_RESPONSE                        = 0x0618, ///< 6.2.0 20033
        SMSG_LOGOUT_CANCEL_ACK                      = 0x0E31, ///< 6.2.0 20033
        SMSG_LOGOUT_COMPLETE                        = 0x0817, ///< 6.2.0 20033
        SMSG_GOSSIP_POI                             = 0x0643, ///< 6.2.0 20033
        SMSG_ARCHAEOLOGY_SURVERY_CAST               = 0x0825, ///< 6.2.0 20033

        /// World Object management
        SMSG_UPDATE_OBJECT                          = 0x0A06, ///< 6.2.0 20033
        SMSG_POWER_UPDATE                           = 0x080C, ///< 6.2.0 20033

        /// Character list
        SMSG_ENUM_CHARACTERS_RESULT                 = 0x0290, ///< 6.2.0 20033
        SMSG_CREATE_CHAR                            = 0x0488, ///< 6.2.0 20033
        SMSG_CHAR_DELETE                            = 0x0856, ///< 6.2.0 20033
        SMSG_SET_PLAYER_DECLINED_NAMES_RESULT       = 0x0B36, ///< 6.2.0 20033
        SMSG_CHAR_FACTION_CHANGE                    = 0x0094, ///< 6.2.0 20033
        SMSG_CHAR_RENAME                            = 0x0912, ///< 6.2.0 20033
        SMSG_CHAR_CUSTOMIZE_RESULT                  = 0x0EE0, ///< 6.2.0 20033
        SMSG_CHAR_CUSTOMIZE                         = 0x0093, ///< 6.2.0 20033

        /// Account data
        SMSG_ACCOUNT_DATA_TIMES                     = 0x0E32, ///< 6.2.0 20033
        SMSG_UPDATE_ACCOUNT_DATA                    = 0x048B, ///< 6.2.0 20033
        SMSG_TUTORIAL_FLAGS                         = 0x005C, ///< 6.2.0 20033

        /// Combat
        SMSG_ATTACKER_STATE_UPDATE                  = 0x08D4, ///< 6.2.0 20033
        SMSG_ATTACK_START                           = 0x0C57, ///< 6.2.0 20033
        SMSG_ATTACK_STOP                            = 0x0A46, ///< 6.2.0 20033
        SMSG_AI_REACTION                            = 0x0CDC, ///< 6.2.0 20033
        SMSG_ATTACK_SWING_ERROR                     = 0x04A5, ///< 6.2.0 20033

        /// Duel
        SMSG_DUEL_REQUESTED                         = 0x08B6, ///< 6.2.0 20033
        SMSG_DUEL_COUNTDOWN                         = 0x03D2, ///< 6.2.0 20033
        SMSG_DUEL_COMPLETE                          = 0x0B15, ///< 6.2.0 20033
        SMSG_DUEL_INBOUNDS                          = 0x0450, ///< 6.2.0 20033
        SMSG_DUEL_OUT_OF_BOUNDS                     = 0x069F, ///< 6.2.0 20033
        SMSG_DUEL_WINNER                            = 0x0098, ///< 6.2.0 20033
        SMSG_CAN_DUEL_RESULT                        = 0x09B2, ///< 6.2.0 20033

        /// Vendor
        SMSG_LIST_INVENTORY                         = 0x0AD4, ///< 6.2.0 20033
        SMSG_BUY_FAILED                             = 0x1086, ///< 6.2.0 20033
        SMSG_BUY_ITEM                               = 0x0EC7, ///< 6.2.0 20033
        SMSG_SELL_ITEM                              = 0x045C, ///< 6.2.0 20033

        /// Achievement
        SMSG_ALL_ACHIEVEMENT_DATA                   = 0x0457, ///< 6.2.0 20033
        SMSG_ACHIEVEMENT_DELETED                    = 0x0C5B, ///< 6.2.0 20033
        SMSG_ACHIEVEMENT_EARNED                     = 0x01A1, ///< 6.2.0 20033
        SMSG_ACCOUNT_CRITERIA_UPDATE                = 0x09C5, ///< 6.2.0 20033
        SMSG_ACCOUNT_CRITERIA_UPDATE_ALL            = 0x0887, ///< 6.2.0 20033
        SMSG_CRITERIA_DELETED                       = 0x0721, ///< 6.2.0 20033
        SMSG_CRITERIA_UPDATE                        = 0x081C, ///< 6.2.0 20033

        /// Friends
        SMSG_CONTACT_LIST                           = 0x0E98, ///< 6.2.0 20033
        SMSG_FRIEND_STATUS                          = 0x0E04, ///< 6.2.0 20033

        /// Taxi
        SMSG_TAXI_NODE_STATUS                       = 0x0A90, ///< 6.2.0 20033
        SMSG_NEW_TAXI_PATH                          = 0x0000, ///< Autoparser error - Opcode not found. Old Opcode: 0x080F
        SMSG_SHOW_TAXI_NODES                        = 0x04CC, ///< 6.2.0 20033
        SMSG_ACTIVATE_TAXI_REPLY                    = 0x02B5, ///< 6.2.0 20033

        /// Loot
        SMSG_LOOT_LIST                              = 0x0698, ///< 6.2.0 20033
        SMSG_LOOT_RESPONSE                          = 0x0DB1, ///< 6.2.0 20033
        SMSG_LOOT_MONEY_NOTIFY                      = 0x0E94, ///< 6.2.0 20033
        SMSG_LOOT_RELEASE                           = 0x0404, ///< 6.2.0 20033
        SMSG_LOOT_RELEASE_ALL                       = 0x06A5, ///< 6.2.0 20033
        SMSG_LOOT_REMOVED                           = 0x0FA6, ///< 6.2.0 20033
        SMSG_COIN_REMOVED                           = 0x0846, ///< 6.2.0 20033
        SMSG_LOOT_ALL_PASSED                        = 0x089B, ///< 6.2.0 20033
        SMSG_LOOT_CONTENTS                          = 0x0000, ///< (unused)
        SMSG_LOOT_ITEM_LIST                         = 0x0000, ///< (unused)
        SMSG_MASTER_LOOT_CANDIDATE_LIST             = 0x0EC8, ///< 6.2.0 20033
        SMSG_LOOT_ROLL                              = 0x008F, ///< 6.2.0 20033
        SMSG_LOOT_ROLLS_COMPLETE                    = 0x0000, ///< (unused)
        SMSG_LOOT_ROLL_WON                          = 0x0DB2, ///< 6.2.0 20033
        SMSG_LOOT_START_ROLL                        = 0x0B42, ///< 6.2.0 20033
        SMSG_AE_LOOT_TARGETS                        = 0x0000, ///< (unused)
        SMSG_AE_LOOT_TARGETS_ACK                    = 0x0000, ///< (unused)
        SMSG_ITEM_BONUS_DEBUG                       = 0x06A6, ///< 6.2.0 20033

        /// Bank
        SMSG_SHOW_BANK                              = 0x0E18, ///< 6.2.0 20033

        /// Rune
        SMSG_RESYNC_RUNES                           = 0x0932, ///< 6.2.0 20033
        SMSG_DEBUG_RUNE_REGEN                       = 0x0EB1, ///< 6.2.0 20033
        SMSG_ADD_RUNE_POWER                         = 0x08DC, ///< 6.2.0 20033
        SMSG_CONVERT_RUNE                           = 0x0E0F, ///< 6.2.0 20033

        /// Group
        SMSG_PARTY_UPDATE                           = 0x0736, ///< 6.2.0 20033
        SMSG_PARTY_MEMBER_STATE_FULL                = 0x0E58, ///< 6.2.0 20033
        SMSG_PARTY_MEMBER_STATE_PARTIAL             = 0x0A11, ///< 6.2.0 20033
        SMSG_PARTY_INVITE                           = 0x0208, ///< 6.2.0 20033
        SMSG_GROUP_DECLINE                          = 0x0291, ///< 6.2.0 20033
        SMSG_PARTY_COMMAND_RESULT                   = 0x0220, ///< 6.2.0 20033
        SMSG_GROUP_UNINVITE                         = 0x0A97, ///< 6.2.0 20033
        SMSG_GROUP_NEW_LEADER                       = 0x009F, ///< 6.2.0 20033
        SMSG_MINIMAP_PING                           = 0x06D4, ///< 6.2.0 20033
        SMSG_SET_LOOT_METHOD_FAILED                 = 0x081B, ///< 6.2.0 20033
        SMSG_RANDOM_ROLL                            = 0x0844, ///< 6.2.0 20033
        SMSG_SEND_RAID_TARGET_UPDATE_SINGLE         = 0x049C, ///< 6.2.0 20033
        SMSG_SEND_RAID_TARGET_UPDATE_ALL            = 0x001F, ///< 6.2.0 20033
        SMSG_READY_CHECK_COMPLETED                  = 0x0E84, ///< 6.2.0 20033
        SMSG_READY_CHECK_RESPONSE                   = 0x029B, ///< 6.2.0 20033
        SMSG_READY_CHECK_STARTED                    = 0x0010, ///< 6.2.0 20033
        SMSG_ROLL_POLL_INFORM                       = 0x0731, ///< 6.2.0 20033
        SMSG_ROLE_CHANGED_INFORM                    = 0x0E8C, ///< 6.2.0 20033
        SMSG_GROUP_DESTROYED                        = 0x0121, ///< 6.2.0 20033

        /// Battleground
        SMSG_BATTLEFIELD_STATUS_QUEUED                      = 0x0888, ///< 6.2.0 20033
        SMSG_BATTLEFIELD_STATUS_NONE                        = 0x0E22, ///< 6.2.0 20033
        SMSG_BATTLEFIELD_STATUS_NEED_CONFIRMATION           = 0x06C8, ///< 6.2.0 20033
        SMSG_BATTLEFIELD_STATUS_ACTIVE                      = 0x065F, ///< 6.2.0 20033
        SMSG_BATTLEFIELD_STATUS_FAILED                      = 0x0525, ///< 6.2.0 20033
        SMSG_BATTLEFIELD_STATUS_WAIT_FOR_GROUPS             = 0x0B26, ///< 6.2.0 20033
        SMSG_BATTLEFIELD_PORT_DENIED                        = 0x0000, ///< (unused)
        SMSG_RATED_BATTLEFIELD_INFO                         = 0x0621, ///< 6.2.0 20033
        SMSG_BATTLEGROUND_PLAYER_JOINED                     = 0x00C8, ///< 6.2.0 20033
        SMSG_BATTLEGROUND_PLAYER_LEFT                       = 0x0018, ///< 6.2.0 20033
        SMSG_BATTLEGROUND_PLAYER_POSITIONS                  = 0x0C98, ///< 6.2.0 20033
        SMSG_BATTLEFIELD_MGR_ENTERING                       = 0x028B, ///< 6.2.0 20033
        SMSG_BATTLEFIELD_MGR_QUEUE_REQUEST_RESPONSE         = 0x0250, ///< 6.2.0 20033
        SMSG_BATTLEFIELD_MGR_EJECT_PENDING                  = 0x0000, ///< (unused)
        SMSG_BATTLEFIELD_MGR_EJECTED                        = 0x0603, ///< 6.2.0 20033
        SMSG_BATTLEFIELD_MGR_QUEUE_INVITE                   = 0x0AD2, ///< 6.2.0 20033
        SMSG_BATTLEFIELD_MGR_STATE_CHANGED                  = 0x0000, ///< (unused)
        SMSG_BFMGR_ENTRY_INVITE                             = 0x0FA2, ///< 6.2.0 20033
        SMSG_BATTLEFIELD_MGR_EXIT_REQUEST                   = 0x0000,
        SMSG_REQUEST_PVP_REWARDS_RESPONSE                   = 0x0820, ///< 6.2.0 20033
        SMSG_PVP_OPTIONS_ENABLED                            = 0x0B56, ///< 6.2.0 20033
        SMSG_PVPLOG_DATA                                    = 0x01C1, ///< 6.2.0 20033
        SMSG_ARENA_OPPONENT_SPECIALIZATIONS                 = 0x0E50, ///< 6.2.0 20033
        SMSG_DESTROY_ARENA_UNIT                             = 0x0000, ///< (unused)
        SMSG_BATTLEGROUND_POINTS                            = 0x0000, ///< (unused)
        SMSG_BATTLEGROUND_INIT                              = 0x0000, ///< (unused)
        SMSG_MAP_OBJECTIVES_INIT                            = 0x0000, ///< (unused)
        SMSG_CONQUEST_FORMULA_CONSTANTS                     = 0x0A18, ///< 6.2.0 20033
        SMSG_PVP_SEASON                                     = 0x0CB2, ///< 6.2.0 20033
        SMSG_WARGAME_REQUEST_SUCCESSFULLY_SENT_TO_OPPENENT  = 0x0000, ///< (unused)
        SMSG_SPIRIT_HEALER_CONFIRM                          = 0x08D1, ///< 6.2.0 20033
        SMSG_AREA_SPIRIT_HEALER_TIME                        = 0x0EB5, ///< 6.2.0 20033
        SMSG_BATTLEFIELD_LIST                               = 0x0454, ///< 6.2.0 20033
        SMSG_CHECK_WARGAME_ENTRY                            = 0x0BB2, ///< 6.2.0 20033

        /// Pet
        SMSG_PET_NAME_QUERY_RESPONSE                        = 0x0CA2, ///< 6.2.0 20033
        SMSG_PET_NAME_INVALID                               = 0x03B2, ///< 6.2.0 20033
        SMSG_PET_DISMISS_SOUND                              = 0x0EB6, ///< 6.2.0 20033
        SMSG_PET_GUIDS                                      = 0x0A4C, ///< 6.2.0 20033
        SMSG_PET_MODE                                       = 0x0000, ///< (unused)
        SMSG_PET_SLOT_UPDATED                               = 0x0000, ///< (unused)
        SMSG_PET_STABLE_LIST                                = 0x0A48, ///< 6.2.0 20033
        SMSG_PET_ACTION_FEEDBACK                            = 0x07A5, ///< 6.2.0 20033
        SMSG_PET_ACTION_SOUND                               = 0x0BA2, ///< 6.2.0 20033
        SMSG_PET_ADDED                                      = 0x0000, ///< (unused)
        SMSG_SET_PET_SPECIALIZATION                         = 0x0054, ///< 6.2.0 20033
        SMSG_STABLE_RESULT                                  = 0x0054, ///< 6.2.0 20033

        /// Pet Battle opcodes
        SMSG_BATTLE_PET_JOURNAL                             = 0x060F, ///< 6.2.0 20033
        SMSG_QUERY_PET_NAME_RESPONSE                        = 0x0CA2, ///< 6.2.0 20033
        SMSG_PETBATTLE_REQUEST_FAILED                       = 0x0000,
        SMSG_PETBATTLE_FIRST_ROUND                          = 0x0000,
        SMSG_PETBATTLE_FULL_UPDATE                          = 0x0000,
        SMSG_PETBATTLE_FINALIZE_LOCATION                    = 0x0000,
        SMSG_PETBATTLE_PVP_CHALLENGE                        = 0x0000,
        SMSG_PETBATTLE_ROUND_RESULT                         = 0x0000,
        SMSG_PETBATTLE_REPLACEMENTS_MADE                    = 0x0000,
        SMSG_PETBATTLE_FINAL_ROUND                          = 0x0000,
        SMSG_PETBATTLE_FINISHED                             = 0x0000,
        SMSG_PET_BATTLE_SLOT_UPDATES                        = 0x0025, ///< 6.2.0 20033

        /// Instances
        SMSG_INSTANCE_ENCOUNTER_ENGAGE_UNIT                     = 0x06CC, ///< 6.2.0 20033
        SMSG_INSTANCE_ENCOUNTER_DISENGAGE_UNIT                  = 0x0B01, ///< 6.2.0 20033
        SMSG_INSTANCE_ENCOUNTER_CHANGE_PRIORITY                 = 0x0901, ///< 6.2.0 20033
        SMSG_INSTANCE_ENCOUNTER_TIMER_START                     = 0x0E07, ///< 6.2.0 20033
        SMSG_INSTANCE_ENCOUNTER_OBJECTIVE_START                 = 0x0ECC, ///< 6.2.0 20033
        SMSG_INSTANCE_ENCOUNTER_OBJECTIVE_COMPLETE              = 0x0447, ///< 6.2.0 20033
        SMSG_INSTANCE_ENCOUNTER_START                           = 0x0C5C, ///< 6.2.0 20033
        SMSG_INSTANCE_ENCOUNTER_OBJECTIVE_UPDATE                = 0x07B5, ///< 6.2.0 20033
        SMSG_INSTANCE_ENCOUNTER_END                             = 0x00D3, ///< 6.2.0 20033
        SMSG_INSTANCE_ENCOUNTER_IN_COMBAT_RESURRECTION          = 0x0893, ///< 6.2.0 20033
        SMSG_INSTANCE_ENCOUNTER_PHASE_SHIFT_CHANGED             = 0x04D0, ///< 6.2.0 20033
        SMSG_INSTANCE_ENCOUNTER_GAIN_COMBAT_RESURRECTION_CHARGE = 0x0CCF, ///< 6.2.0 20033
        SMSG_INSTANCE_RESET                                     = 0x03D6, ///< 6.2.0 20033
        SMSG_INSTANCE_RESET_FAILED                              = 0x0C0F, ///< 6.2.0 20033
        SMSG_RAID_INSTANCE_INFO                                 = 0x025F, ///< 6.2.0 20033
        SMSG_RAID_GROUP_ONLY                                    = 0x084B, ///< 6.2.0 20033
        SMSG_RAID_MARKERS_CHANGED                               = 0x0E5F, ///< 6.2.0 20033
        SMSG_INSTANCE_SAVE_CREATED                              = 0x0F21, ///< 6.2.0 20033
        SMSG_INSTANCE_GROUP_SIZE_CHANGED                        = 0x0E1B, ///< 6.2.0 20033
        SMSG_ENCOUNTER_END                                      = 0x0608, ///< 6.2.0 20033
        SMSG_ENCOUNTER_START                                    = 0x0E14, ///< 6.2.0 20033
        SMSG_BOSS_KILL_CREDIT                                   = 0x0F25, ///< 6.2.0 20033

        /// Scenarios
        SMSG_SCENARIO_POI                                       = 0x00CC, ///< 6.2.0 20033
        SMSG_SCENARIO_PROGRESS_UPDATE                           = 0x0AA5, ///< 6.2.0 20033
        SMSG_SCENARIO_STATE                                     = 0x0E35, ///< 6.2.0 20033

        /// Auction House
        SMSG_AUCTION_HELLO_RESPONSE                             = 0x0C18, ///< 6.2.0 20033
        SMSG_AUCTION_BIDDER_LIST_RESULT                         = 0x020F, ///< 6.2.0 20033
        SMSG_AUCTION_BIDDER_NOTIFICATION                        = 0x04A0, ///< 6.2.0 20033
        SMSG_AUCTION_BUYOUT_NOTIFICATION                        = 0x0287, ///< 6.2.0 20033
        SMSG_AUCTION_COMMAND_RESULT                             = 0x0847, ///< 6.2.0 20033
        SMSG_AUCTION_CLOSED_NOTIFICATION                        = 0x0EC3, ///< 6.2.0 20033
        SMSG_AUCTION_LIST_PENDING_SALES                         = 0x02C8, ///< 6.2.0 20033
        SMSG_AUCTION_LIST_RESULT                                = 0x0952, ///< 6.2.0 20033
        SMSG_AUCTION_OWNER_LIST_RESULT                          = 0x0802, ///< 6.2.0 20033
        SMSG_AUCTION_OWNER_BID_NOTIFICATION                     = 0x0CCC, ///< 6.2.0 20033

        /// Mail
        SMSG_SEND_MAIL_RESULT                                   = 0x04B1, ///< 6.2.0 20033
        SMSG_MAIL_LIST_RESULT                                   = 0x0951, ///< 6.2.0 20033
        SMSG_RECEIVED_MAIL                                      = 0x06B5, ///< 6.2.0 20033
        SMSG_MAIL_QUERY_NEXT_TIME_RESULT                        = 0x00D6, ///< 6.2.0 20033

        /// Trainers
        SMSG_TRAINER_LIST                                       = 0x0A1B, ///< 6.2.0 20033
        SMSG_TRAINER_SERVICE                                    = 0x0BA2, ///< 6.2.0 20033

        /// Void Storage
        SMSG_VOID_ITEM_SWAP_RESPONSE                            = 0x0004, ///< 6.2.0 20033
        SMSG_VOID_STORAGE_CONTENTS                              = 0x08D2, ///< 6.2.0 20033
        SMSG_VOID_STORAGE_FAILED                                = 0x008B, ///< 6.2.0 20033
        SMSG_VOID_STORAGE_TRANSFER_CHANGES                      = 0x00D5, ///< 6.2.0 20033
        SMSG_VOID_TRANSFER_RESULT                               = 0x0D35, ///< 6.2.0 20033

        /// Petition
        SMSG_PETITION_ALREADY_SIGNED                            = 0x0C60, ///< 6.2.0 20033
        SMSG_PETITION_SIGN_RESULTS                              = 0x00C8, ///< 6.2.0 20033
        SMSG_QUERY_PETITION_RESPONSE                            = 0x06B2, ///< 6.2.0 20033
        SMSG_PETITION_SHOW_LIST                                 = 0x0D25, ///< 6.2.0 20033
        SMSG_PETITION_SHOW_SIGNATURES                           = 0x08DB, ///< 6.2.0 20033
        SMSG_TURN_IN_PETITION_RESULTS                           = 0x0236, ///< 6.2.0 20033
        SMSG_PETITION_DECLINED                                  = 0x0000, ///< fake opcode

        /// Threat
        SMSG_THREAT_CLEAR                                       = 0x03C6, ///< 6.2.0 20033
        SMSG_THREAT_REMOVE                                      = 0x0C8F, ///< 6.2.0 20033
        SMSG_THREAT_UPDATE                                      = 0x0814, ///< 6.2.0 20033
        SMSG_HIGHEST_THREAT_UPDATE                              = 0x0816, ///< 6.2.0 20033

        /// Tickets
        SMSG_GM_TICKET_GET_TICKET_RESPONSE                      = 0x0EDF, ///< 6.2.0 20033
        SMSG_GM_TICKET_RESPONSE                                 = 0x0131, ///< 6.2.0 20033
        SMSG_GM_TICKET_RESPONSE_ERROR                           = 0x0841, ///< 6.2.0 20033
        SMSG_GM_TICKET_RESOLVE_RESPONSE                         = 0x0408, ///< 6.2.0 20033
        SMSG_GM_TICKET_SYSTEM_STATUS                            = 0x03D1, ///< 6.2.0 20033
        SMSG_GM_TICKET_UPDATE                                   = 0x0A94, ///< 6.2.0 20033
        SMSG_GM_TICKET_STATUS_UPDATE                            = 0x005F, ///< 6.2.0 20033
        SMSG_GM_TICKET_CASE_STATUS                              = 0x0A03, ///< 6.2.0 20033

        /// Calendar
        SMSG_CALENDAR_CLEAR_PENDING_ACTION                      = 0x0C53, ///< 6.2.0 20033
        SMSG_CALENDAR_COMMAND_RESULT                            = 0x0243, ///< 6.2.0 20033
        SMSG_CALENDAR_EVENT_INITIAL_INVITE                      = 0x060D, ///< 6.2.0 20033
        SMSG_CALENDAR_EVENT_INVITE                              = 0x04B5, ///< 6.2.0 20033
        SMSG_CALENDAR_EVENT_INVITE_ALERT                        = 0x045F, ///< 6.2.0 20033
        SMSG_CALENDAR_EVENT_INVITE_NOTES                        = 0x088C, ///< 6.2.0 20033
        SMSG_CALENDAR_EVENT_INVITE_NOTES_ALERT                  = 0x0735, ///< 6.2.0 20033
        SMSG_CALENDAR_EVENT_INVITE_REMOVED                      = 0x0C48, ///< 6.2.0 20033
        SMSG_CALENDAR_EVENT_INVITE_REMOVED_ALERT                = 0x0284, ///< 6.2.0 20033
        SMSG_CALENDAR_EVENT_INVITE_STATUS                       = 0x0925, ///< 6.2.0 20033
        SMSG_CALENDAR_EVENT_INVITE_STATUS_ALERT                 = 0x12D7, ///< 6.2.0 20033
        SMSG_CALENDAR_EVENT_MODERATOR_STATUS                    = 0x05B2, ///< 6.2.0 20033
        SMSG_CALENDAR_EVENT_REMOVED_ALERT                       = 0x0C93, ///< 6.2.0 20033
        SMSG_CALENDAR_EVENT_UPDATED_ALERT                       = 0x0235, ///< 6.2.0 20033
        SMSG_CALENDAR_RAID_LOCKOUT_ADDED                        = 0x0484, ///< 6.2.0 20033
        SMSG_CALENDAR_RAID_LOCKOUT_REMOVED                      = 0x00D8, ///< 6.2.0 20033
        SMSG_CALENDAR_RAID_LOCKOUT_UPDATED                      = 0x09B6, ///< 6.2.0 20033
        SMSG_CALENDAR_SEND_CALENDAR                             = 0x01D2, ///< 6.2.0 20033
        SMSG_CALENDAR_SEND_EVENT                                = 0x090A, ///< 6.2.0 20033
        SMSG_CALENDAR_SEND_NUM_PENDING                          = 0x0A87, ///< 6.2.0 20033

        /// Warden
        SMSG_WARDEN_DATA                                        = 0x0000, ///< Autoparser error - Opcode not found. Old Opcode: 0x0BEC

        /// Challenges
        SMSG_CHALLENGE_MODE_REQUEST_LEADERS_RESULT              = 0x0453, ///< 6.2.0 20033
        SMSG_CHALLENGE_MODE_MAP_STATS_UPDATE                    = 0x0810, ///< 6.2.0 20033
        SMSG_CHALLENGE_MODE_NEW_PLAYER_RECORD                   = 0x0E83, ///< 6.2.0 20033
        SMSG_CHALLENGE_MODE_ALL_MAP_STATS                       = 0x0A22, ///< 6.2.0 20033
        SMSG_CHALLENGE_MODE_START                               = 0x0000, ///< This opcode is wrong and not existing
        SMSG_CHALLENGE_MODE_COMPLETE                            = 0x068B, ///< 6.2.0 20033
        SMSG_CHALLENGE_MOD_REWARDS                              = 0x0000, ///< (NYI)

        /// BlackMarket
        SMSG_BLACK_MARKET_OPEN_RESULT                           = 0x0A58, ///< 6.2.0 20033
        SMSG_BLACK_MARKET_OUTBID                                = 0x0226, ///< 6.2.0 20033
        SMSG_BLACK_MARKET_REQUEST_ITEMS_RESULT                  = 0x0A14, ///< 6.2.0 20033
        SMSG_BLACK_MARKET_BID_ON_ITEM_RESULT                    = 0x0921, ///< 6.2.0 20033
        SMSG_BLACK_MARKET_WON                                   = 0x01C5, ///< 6.2.0 20033

        /// Garrison
        SMSG_OPEN_SHIPMENT_NPCFROM_GOSSIP                       = 0x089C, ///< 6.2.0 20033
        SMSG_GET_SHIPMENT_INFO_RESPONSE                         = 0x0826, ///< 6.2.0 20033
        SMSG_GET_SHIPMENTS                                      = 0x0C22, ///< 6.2.0 20033
        SMSG_CREATE_SHIPMENT_RESPONSE                           = 0x00B6, ///< 6.2.0 20033

        /// Twitter
        SMSG_REQUEST_TWITTER_STATUS_RESPONSE                    = 0x0000,
        SMSG_OAUTH_SAVED_DATA                                   = 0x04DA, ///< 6.2.0 20033

        /// Toys
        SMSG_ACCOUNT_TOYS_UPDATE                                = 0x0ACF, ///< 6.2.0 20033
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Guild
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamGuild
        /// Petition
        SMSG_PETITION_RENAME                        = 0x08AB, ///< 6.2.0 20033

        /// Guild Finder
        SMSG_LFGUILD_BROWSE                         = 0x08FC, ///< 6.2.0 20033
        SMSG_LFGUILD_APPLICATIONS                   = 0x04EB, ///< 6.2.0 20033
        SMSG_LFGUILD_APPLICATIONS_LIST_CHANGED      = 0x00FC, ///< 6.2.0 20033
        SMSG_LFGUILD_COMMAND_RESULT                 = 0x0000, ///< (unused)
        SMSG_LFGUILD_RECRUITS                       = 0x08E4, ///< 6.2.0 20033
        SMSG_LFGUILD_POST                           = 0x02B4, ///< 6.2.0 20033
        SMSG_LFGUILD_APPLICANT_LIST_CHANGED         = 0x00FC, ///< 6.2.0 20033

        /// Bank
        SMSG_GUILD_RANKS                            = 0x06FB, ///< 6.2.0 20033
        SMSG_GUILD_BANK_LOG_QUERY_RESULT            = 0x06EB, ///< 6.2.0 20033
        SMSG_GUILD_BANK_REMAINING_WITHDRAW_MONEY    = 0x0CA4, ///< 6.2.0 20033
        SMSG_GUILD_BANK_QUERY_TEXT_RESULT           = 0x02E4, ///< 6.2.0 20033
        SMSG_GUILD_BANK_QUERY_RESULTS               = 0x06FC, ///< 6.2.0 20033
        SMSG_GUILD_PERMISSIONS_QUERY_RESULTS        = 0x00EC, ///< 6.2.0 20033

        /// Achievement
        SMSG_GUILD_ACHIEVEMENT_DELETED              = 0x06AC, ///< 6.2.0 20033
        SMSG_GUILD_ACHIEVEMENT_MEMBERS              = 0x0000, ///< (unused)
        SMSG_GUILD_ACHIEVEMENT_EARNED               = 0x01A1, ///< 6.2.0 20033
        SMSG_GUILD_CRITERIA_UPDATE                  = 0x0000, ///< (unused)
        SMSG_GUILD_CRITERIA_DELETED                 = 0x00AB, ///< 6.2.0 20033
        SMSG_ALL_GUILD_ACHIEVEMENTS                 = 0x08E3, ///< 6.2.0 20033

        /// Info
        SMSG_GUILD_ROSTER                           = 0x04B3, ///< 6.2.0 20033
        SMSG_GUILD_ROSTER_UPDATE                    = 0x0000, ///< (unused)
        SMSG_QUERY_GUILD_INFO_RESPONSE              = 0x04A4, ///< 6.2.0 20033
        SMSG_GUILD_MEMBER_UPDATE_NOTE               = 0x0000, ///< (unused)
        SMSG_GUILD_REWARDS_LIST                     = 0x0AF3, ///< 6.2.0 20033
        SMSG_GUILD_SEND_RANK_CHANGE                 = 0x00B4, ///< 6.2.0 20033
        SMSG_GUILD_CHALLENGE_UPDATED                = 0x02AB, ///< 6.2.0 20033
        SMSG_GUILD_MEMBER_RECIPES                   = 0x0000, ///< (unused)
        SMSG_GUILD_INVITE_EXPIRED                   = 0x0000, ///< (unused)
        SMSG_GUILD_COMMAND_RESULT                   = 0x06EC, ///< 6.2.0 20033
        SMSG_GUILD_CHALLENGE_COMPLETED              = 0x00BC, ///< 6.2.0 20033
        SMSG_GUILD_REPUTATION_REACTION_CHANGED      = 0x00E3, ///< 6.2.0 20033
        SMSG_GUILD_KNOWN_RECIPES                    = 0x04BB, ///< 6.2.0 20033
        SMSG_GUILD_MEMBER_DAILY_RESET               = 0x0AFB, ///< 6.2.0 20033
        SMSG_GUILD_FLAGGED_FOR_RENAME               = 0x0000, ///< (unused)
        SMSG_GUILD_NAME_CHANGED                     = 0x0000, ///< (unused)
        SMSG_GUILD_RESET                            = 0x0000, ///< (unused)
        SMSG_GUILD_CHANGE_NAME_RESULT               = 0x0000, ///< (unused)
        SMSG_GUILD_INVITE                           = 0x08BB, ///< 6.2.0 20033
        SMSG_GUILD_INVITE_DECLINED                  = 0x0000, ///< (unused)
        SMSG_GUILD_PARTY_STATE                      = 0x0ABC, ///< 6.2.0 20033
        SMSG_GUILD_MEMBERS_WITH_RECIPE              = 0x0000, ///< (unused)
        SMSG_GUILD_MOVE_STARTING                    = 0x0000, ///< (unused)
        SMSG_GUILD_MOVED                            = 0x0000, ///< (unused)
        SMSG_GUILD_NEWS                             = 0x00A3, ///< 6.2.0 20033
        SMSG_GUILD_NEWS_DELETED                     = 0x0000, ///< (unused)
        SMSG_PLAYER_TABAR_VENDOR_SHOW               = 0x0C90, ///< 6.2.0 20033
        SMSG_PLAYER_SAVE_GUILD_EMBLEM               = 0x02EB, ///< 6.2.0 20033

        /// Event system
        SMSG_GUILD_EVENT_NEW_LEADER                 = 0x02F3, ///< 6.2.0 20033
        SMSG_GUILD_EVENT_PRESENCE_CHANGE            = 0x06F4, ///< 6.2.0 20033
        SMSG_GUILD_EVENT_PLAYER_JOINED              = 0x06E4, ///< 6.2.0 20033
        SMSG_GUILD_EVENT_PLAYER_LEFT                = 0x06A4, ///< 6.2.0 20033
        SMSG_GUILD_EVENT_RANKS_UPDATED              = 0x00F4, ///< 6.2.0 20033
        SMSG_GUILD_EVENT_RANK_CHANGED               = 0x08A3, ///< 6.2.0 20033
        SMSG_GUILD_EVENT_LOG_QUERY_RESULTS          = 0x08B4, ///< 6.2.0 20033
        SMSG_GUILD_EVENT_MOTD                       = 0x02A3, ///< 6.2.0 20033
        SMSG_GUILD_EVENT_DISBANDED                  = 0x06F3, ///< 6.2.0 20033
        SMSG_GUILD_EVENT_TAB_ADDED                  = 0x0AA4, ///< 6.2.0 20033
        SMSG_GUILD_EVENT_TAB_MODIFIED               = 0x04A3, ///< 6.2.0 20033
        SMSG_GUILD_EVENT_TAB_DELETED                = 0x04B4, ///< 6.2.0 20033
        SMSG_GUILD_EVENT_TAB_TEXT_CHANGED           = 0x08F4, ///< 6.2.0 20033
        SMSG_GUILD_EVENT_BANK_CONTENTS_CHANGED      = 0x00F3, ///< 6.2.0 20033
        SMSG_GUILD_EVENT_BANK_MONEY_CHANGED         = 0x0AF4, ///< 6.2.0 20033
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
        SMSG_SERVER_FIRST_ACHIEVEMENT               = 0x154A, ///< 6.2.0 20033
        SMSG_RAID_INSTANCE_MESSAGE                  = 0x171A, ///< 6.2.0 20033
        SMSG_MOTD                                   = 0x1649, ///< 6.2.0 20033
        SMSG_EXPECTED_SPAM_RECORDS                  = 0x0000, ///< (unused)
        SMSG_DEFENSE_MESSAGE                        = 0x0000, ///< (unused)
        SMSG_ZONE_UNDER_ATTACK                      = 0x1419, ///< 6.2.0 20033
        SMSG_WHO                                    = 0x16CA, ///< 6.2.0 20033

        /// Chat
        SMSG_CHAT_SERVER_MESSAGE                    = 0x1D49, ///< 6.2.0 20033
        SMSG_CHAT_RESTRICTED                        = 0x16DA, ///< 6.2.0 20033
        SMSG_CHAT_RECONNECT                         = 0x0000, ///< (unused)
        SMSG_CHAT_PLAYER_NOTFOUND                   = 0x17D9, ///< 6.2.0 20033
        SMSG_CHAT_PLAYER_AMBIGUOUS                  = 0x1459, ///< 6.2.0 20033
        SMSG_CHAT_NOT_IN_PARTY                      = 0x0000, ///< (unused)
        SMSG_CHAT_IS_DOWN                           = 0x0000, ///< (unused)
        SMSG_CHAT_IGNORED_ACCOUNT_MUTED             = 0x0000, ///< (unused)
        SMSG_CHAT_DOWN                              = 0x0000, ///< (unused)
        SMSG_CHAT_AUTO_RESPONDED                    = 0x0000, ///< (unused)
        SMSG_CHAT                                   = 0x144A, ///< 6.2.0 20033

        /// Channel
        SMSG_CHANNEL_NOTIFY_LEFT                    = 0x145A, ///< 6.2.0 20033
        SMSG_CHANNEL_NOTIFY_JOINED                  = 0x1519, ///< 6.2.0 20033
        SMSG_CHANNEL_NOTIFY                         = 0x1699, ///< 6.2.0 20033
        SMSG_CHANNEL_LIST                           = 0x14DA, ///< 6.2.0 20033
        SMSG_USERLIST_ADD                           = 0x1C59, ///< 6.2.0 20033
        SMSG_USERLIST_REMOVE                        = 0x1D5A, ///< 6.2.0 20033
        SMSG_USERLIST_UPDATE                        = 0x1489, ///< 6.2.0 20033
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Move
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamMove
        /// Move speeds
        SMSG_MOVE_SET_RUN_SPEED                     = 0x1C04, ///< 6.2.0 20033
        SMSG_MOVE_SET_RUN_BACK_SPEED                = 0x1CC8, ///< 6.2.0 20033
        SMSG_MOVE_SET_SWIM_SPEED                    = 0x10D3, ///< 6.2.0 20033
        SMSG_MOVE_SET_SWIM_BACK_SPEED               = 0x1227, ///< 6.2.0 20033
        SMSG_MOVE_SET_FLIGHT_SPEED                  = 0x1C43, ///< 6.2.0 20033
        SMSG_MOVE_SET_FLIGHT_BACK_SPEED             = 0x0ED6, ///< 6.2.0 20033
        SMSG_MOVE_SET_WALK_SPEED                    = 0x1954, ///< 6.2.0 20033
        SMSG_MOVE_SET_TURN_RATE                     = 0x032C, ///< 6.2.0 20033
        SMSG_MOVE_SET_PITCH_RATE                    = 0x0F30, ///< 6.2.0 20033
        SMSG_MOVE_KNOCK_BACK                        = 0x19D3, ///< 6.2.0 20033
        SMSG_MOVE_UPDATE_RUN_SPEED                  = 0x0630, ///< 6.2.0 20033
        SMSG_MOVE_UPDATE_RUN_BACK_SPEED             = 0x0623, ///< 6.2.0 20033
        SMSG_MOVE_UPDATE_SWIM_SPEED                 = 0x0C64, ///< 6.2.0 20033
        SMSG_MOVE_UPDATE_SWIM_BACK_SPEED            = 0x0E92, ///< 6.2.0 20033
        SMSG_MOVE_UPDATE_FLIGHT_SPEED               = 0x1104, ///< 6.2.0 20033
        SMSG_MOVE_UPDATE_FLIGHT_BACK_SPEED          = 0x0C51, ///< 6.2.0 20033
        SMSG_MOVE_UPDATE_WALK_SPEED                 = 0x1188, ///< 6.2.0 20033
        SMSG_MOVE_UPDATE_TURN_RATE                  = 0x0196, ///< 6.2.0 20033
        SMSG_MOVE_UPDATE_PITCH_RATE                 = 0x0423, ///< 6.2.0 20033
        SMSG_MOVE_UPDATE_KNOCK_BACK                 = 0x1D18, ///< 6.2.0 20033

        /// Player
        SMSG_MOVE_UPDATE                                    = 0x1325, ///< 6.2.0 20033
        SMSG_MOVE_TELEPORT                                  = 0x1157, ///< 6.2.0 20033
        SMSG_MOVE_UPDATE_TELEPORT                           = 0x1958, ///< 6.2.0 20033
        SMSG_MOVE_ROOT                                      = 0x0AC6, ///< 6.2.0 20033
        SMSG_MOVE_UNROOT                                    = 0x1271, ///< 6.2.0 20033
        SMSG_MOVE_WATER_WALK                                = 0x1508, ///< 6.2.0 20033
        SMSG_MOVE_LAND_WALK                                 = 0x0C82, ///< 6.2.0 20033
        SMSG_MOVE_FEATHER_FALL                              = 0x1D04, ///< 6.2.0 20033
        SMSG_MOVE_NORMAL_FALL                               = 0x1513, ///< 6.2.0 20033
        SMSG_MOVE_SET_ACTIVE_MOVER                          = 0x0ED1, ///< 6.2.0 20033
        SMSG_MOVE_SET_CAN_FLY                               = 0x0824, ///< 6.2.0 20033
        SMSG_MOVE_UNSET_CAN_FLY                             = 0x0A81, ///< 6.2.0 20033
        SMSG_MOVE_SET_HOVER                                 = 0x1054, ///< 6.2.0 20033
        SMSG_MOVE_UNSET_HOVER                               = 0x0082, ///< 6.2.0 20033
        SMSG_MOVE_SET_CAN_TURN_WHILE_FALLING                = 0x1488, ///< 6.2.0 20033
        SMSG_MOVE_UNSET_CAN_TURN_WHILE_FALLING              = 0x0B2F, ///< 6.2.0 20033
        SMSG_MOVE_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY   = 0x1143, ///< 6.2.0 20033
        SMSG_MOVE_UNSET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY = 0x1D84, ///< 6.2.0 20033
        SMSG_MOVE_SET_VEHICLE_REC_ID                        = 0x1276, ///< 6.2.0 20033
        SMSG_MOVE_SET_COLLISION_HEIGHT                      = 0x1918, ///< 6.2.0 20033
        SMSG_APPLY_MOVEMENT_FORCE                           = 0x1C53, ///< 6.2.0 20033
        SMSG_UNAPPLY_MOVEMENT_FORCE                         = 0x19C3, ///< 6.2.0 20033
        SMSG_MOVE_SKIP_TIME                                 = 0x0000, ///< (unused)
        SMSG_MOVE_ENABLE_GRAVITY                            = 0x0000, ///< (unused)
        SMSG_MOVE_DISABLE_GRAVITY                           = 0x0000, ///< (unused)
        SMSG_MOVE_ENABLE_COLLISION                          = 0x0000, ///< (unused)
        SMSG_MOVE_DISABLE_COLLISION                         = 0x0000, ///< (unused)

        /// Creature
        SMSG_MONSTER_MOVE                           = 0x0C28, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_SET_RUN_SPEED              = 0x12E1, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_SET_RUN_BACK_SPEED         = 0x1058, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_SET_SWIM_SPEED             = 0x1588, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_SET_SWIM_BACK_SPEED        = 0x1D17, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_SET_FLIGHT_SPEED           = 0x18C7, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_SET_FLIGHT_BACK_SPEED      = 0x1264, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_SET_WALK_SPEED             = 0x1954, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_SET_TURN_RATE              = 0x1557, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_SET_PITCH_RATE             = 0x1908, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_ROOT                       = 0x12E3, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_UNROOT                     = 0x0141, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_GRAVITY_DISABLE            = 0x0827, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_GRAVITY_ENABLE             = 0x12F5, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_COLLISION_DISABLE          = 0x0000, ///< (unused)
        SMSG_SPLINE_MOVE_COLLISION_ENABLE           = 0x0000, ///< (unused)
        SMSG_SPLINE_MOVE_SET_FEATHER_FALL           = 0x1335, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_SET_NORMAL_FALL            = 0x0281, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_SET_HOVER                  = 0x0E91, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_UNSET_HOVER                = 0x0627, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_SET_WATER_WALK             = 0x0C45, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_SET_LAND_WALK              = 0x0B24, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_START_SWIM                 = 0x0000, ///< (unused)
        SMSG_SPLINE_MOVE_STOP_SWIM                  = 0x0000, ///< (unused)
        SMSG_SPLINE_MOVE_SET_RUN_MODE               = 0x0C91, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_SET_WALK_MODE              = 0x12F1, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_SET_FLYING                 = 0x1048, ///< 6.2.0 20033
        SMSG_SPLINE_MOVE_UNSET_FLYING               = 0x1483, ///< 6.2.0 20033
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Spell
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamSpell
        /// Aura
        SMSG_AURA_POINTS_DEPLETED                   = 0x0000, ///< (unused)
        SMSG_AURA_UPDATE                            = 0x0ABD, ///< 6.2.0 20033

        /// Misc
        SMSG_CHEAT_IGNORE_DIMISHING_RETURNS         = 0x0000, ///< (unused)
        SMSG_DISPEL_FAILED                          = 0x057E, ///< 6.2.0 20033
        SMSG_INCREASE_CAST_TIME_FOR_SPELL           = 0x0000, ///< (unused)
        SMSG_MIRROR_IMAGE_COMPONENTED_DATA          = 0x092D, ///< 6.2.0 20033
        SMSG_MIRROR_IMAGE_CREATURE_DATA             = 0x0000, ///< (unused)
        SMSG_PET_CLEAR_SPELLS                       = 0x0000, ///< (unused)
        SMSG_REFRESH_SPELL_HISTORY                  = 0x0000, ///< (unused)
        SMSG_RESUME_CAST_BAR                        = 0x0000, ///< (unused)
        SMSG_CAST_FAILED                            = 0x066E, ///< 6.2.0 20033
        SMSG_SPELL_FAILURE                          = 0x00EA, ///< 6.2.0 20033
        SMSG_SPELL_FAILED_OTHER                     = 0x0BAA, ///< 6.2.0 20033
        SMSG_PET_CAST_FAILED                        = 0x07FE, ///< 6.2.0 20033
        SMSG_ITEM_COOLDOWN                          = 0x0A47, ///< 6.2.0 20033
        SMSG_MODIFY_COOLDOWN                        = 0x0831, ///< 6.2.0 20033
        SMSG_COOLDOWN_CHEAT                         = 0x0000, ///< (unused)
        SMSG_PET_TAME_FAILURE                       = 0x0822, ///< 6.2.0 20033
        SMSG_FEIGN_DEATH_RESISTED                   = 0x0091, ///< 6.2.0 20033
        SMSG_NOTIFY_DEST_LOC_SPELL_CAST             = 0x0000, ///< (unused)
        SMSG_ON_CANCEL_EXPECTED_RIDE_VEHICLE_AURA   = 0x060B, ///< 6.2.0 20033
        SMSG_SET_VEHICLE_REC_ID                     = 0x0526, ///< 6.2.0 20033
        SMSG_COOLDOWN_EVENT                         = 0x0E44, ///< 6.2.0 20033
        SMSG_DISMOUNT                               = 0x080B, ///< 6.2.0 20033
        SMSG_DISMOUNTRESULT                         = 0x0000, ///< (unused)
        SMSG_MOUNT_RESULT                           = 0x0AB1, ///< 6.2.0 20033

        /// Spell Book / Bar
        SMSG_UPDATE_WEEKLY_SPELL_USAGE              = 0x0000, ///< (unused)
        SMSG_WEEKLY_SPELL_USAGE                     = 0x0000, ///< (unused)
        SMSG_SEND_KNOWN_SPELLS                      = 0x096E, ///< 6.2.0 20033
        SMSG_SEND_SPELL_CHARGES                     = 0x0BF9, ///< 6.2.0 20033
        SMSG_SEND_SPELL_HISTORY                     = 0x0000, ///< (unused)
        SMSG_SEND_UNLEARN_SPELLS                    = 0x0000, ///< (unused)
        SMSG_CLEAR_ALL_SPELL_CHARGES                = 0x016D, ///< 6.2.0 20033
        SMSG_CLEAR_COOLDOWN                         = 0x0C08, ///< 6.2.0 20033
        SMSG_CLEAR_COOLDOWNS                        = 0x05BE, ///< 6.2.0 20033
        SMSG_CATEGORY_COOLDOWN                      = 0x0C7E, ///< 6.2.0 20033
        SMSG_CLEAR_SPELL_CHARGES                    = 0x08A9, ///< 6.2.0 20033
        SMSG_SET_FLAT_SPELL_MODIFIER                = 0x0AE9, ///< 6.2.0 20033
        SMSG_SET_PCT_SPELL_MODIFIER                 = 0x01BA, ///< 6.2.0 20033
        SMSG_SET_SPELL_CHARGES                      = 0x002D, ///< 6.2.0 20033
        SMSG_LEARNED_SPELL                          = 0x092A, ///< 6.2.0 20033
        SMSG_UNLEARNED_SPELLS                       = 0x04B9, ///< 6.2.0 20033

        /// Casting
        SMSG_SPELL_CHANNEL_START                    = 0x07BD, ///< 6.2.0 20033
        SMSG_SPELL_COOLDOWN                         = 0x0569, ///< 6.2.0 20033
        SMSG_SPELL_CHANNEL_UPDATE                   = 0x067E, ///< 6.2.0 20033
        SMSG_SPELL_DAMAGE_SHIELD                    = 0x07B9, ///< 6.2.0 20033
        SMSG_SPELL_DELAYED                          = 0x012E, ///< 6.2.0 20033
        SMSG_SPELL_GO                               = 0x046E, ///< 6.2.0 20033
        SMSG_SPELL_MULTISTRIKE_EFFECT               = 0x0000, ///< (unused)
        SMSG_SPELL_OR_DAMAGE_IMMUNE                 = 0x00BE, ///< 6.2.0 20033
        SMSG_SPELL_START                            = 0x007D, ///< 6.2.0 20033
        SMSG_SPELL_UPDATE_CHAIN_TARGETS             = 0x0000, ///< (unused)

        /// Logging
        SMSG_SPELL_DISPELL_LOG                      = 0x04AA, ///< 6.2.0 20033
        SMSG_SPELL_ENERGIZE_LOG                     = 0x05EE, ///< 6.2.0 20033
        SMSG_SPELL_EXECUTE_LOG                      = 0x0C39, ///< 6.2.0 20033
        SMSG_SPELL_NON_MELEE_DAMAGE_LOG             = 0x062E, ///< 6.2.0 20033
        SMSG_SPELL_HEAL_LOG                         = 0x02A9, ///< 6.2.0 20033
        SMSG_SPELL_INSTAKILL_LOG                    = 0x02FA, ///< 6.2.0 20033
        SMSG_SPELL_INTERRUPT_LOG                    = 0x05A9, ///< 6.2.0 20033
        SMSG_SPELL_MISS_LOG                         = 0x07BE, ///< 6.2.0 20033
        SMSG_ENVIRONMENTAL_DAMAGE_LOG               = 0x093D, ///< 6.2.0 20033
        SMSG_SPELL_PERIODIC_AURA_LOG                = 0x06FA, ///< 6.2.0 20033
        SMSG_SPELL_PROC_SCRIPT_LOG                  = 0x0000, ///< Debug opcode (disabled)
        SMSG_SPELL_PROCS_PER_MINUTE_LOG             = 0x0000, ///< Debug opcode (disabled)
        SMSG_SPELL_CHANCE_PROC_LOG                  = 0x0000, ///< Debug opcode (disabled)
        SMSG_RESIST_LOG                             = 0x0000, ///< Debug opcode (disabled)
        SMSG_AURA_CAST_LOG                          = 0x0000, ///< Debug opcode (disabled)

        /// Pet
        SMSG_PET_SPELLS_MESSAGE                     = 0x016A, ///< 6.2.0 20033
        SMSG_PET_UNLEARNED_SPELLS                   = 0x06BE, ///< 6.2.0 20033
        SMSG_PET_LEARNED_SPELLS                     = 0x08B9, ///< 6.2.0 20033

        /// Visuals
        SMSG_PLAY_ORPHAN_SPELL_VISUAL               = 0x04FE, ///< 6.2.0 20033
        SMSG_CANCEL_ORPHAN_SPELL_VISUAL             = 0x01BD, ///< 6.2.0 20033
        SMSG_PLAY_SPELL_VISUAL                      = 0x002E, ///< 6.2.0 20033
        SMSG_CANCEL_SPELL_VISUAL                    = 0x006D, ///< 6.2.0 20033
        SMSG_PLAY_SPELL_VISUAL_KIT                  = 0x0B3E, ///< 6.2.0 20033
        SMSG_CANCEL_SPELL_VISUAL_KIT                = 0x0BFA, ///< 6.2.0 20033
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Quest
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamQuest
        /// Quest log
        SMSG_QUERY_QUEST_INFO_RESPONSE              = 0x0C09, ///< 6.2.0 20033
        SMSG_QUEST_UPDATE_FAILED_TIMER              = 0x0919, ///< 6.2.0 20033
        SMSG_QUEST_UPDATE_FAILED                    = 0x044A, ///< 6.2.0 20033
        SMSG_QUEST_UPDATE_COMPLETE                  = 0x1344, ///< 6.2.0 20033
        SMSG_QUEST_UPDATE_ADD_PVP_CREDIT            = 0x1203, ///< 6.2.0 20033
        SMSG_QUEST_UPDATE_ADD_CREDIT_SIMPLE         = 0x1218, ///< 6.2.0 20033
        SMSG_QUEST_UPDATE_ADD_CREDIT                = 0x005E, ///< 6.2.0 20033
        SMSG_QUEST_PUSH_RESULT                      = 0x035A, ///< 6.2.0 20033
        SMSG_QUEST_POIQUERY_RESPONSE                = 0x0409, ///< 6.2.0 20033
        SMSG_QUEST_LOG_FULL                         = 0x06CE, ///< 6.2.0 20033
        SMSG_IS_QUEST_COMPLETE_RESPONSE             = 0x0D0A, ///< 6.2.0 20033
        SMSG_QUEST_FORCE_REMOVED                    = 0x085A, ///< 6.2.0 20033
        SMSG_QUEST_CONFIRM_ACCEPT                   = 0x0589, ///< 6.2.0 20033
        SMSG_QUEST_COMPLETION_NPCRESPONSE           = 0x0E8A, ///< 6.2.0 20033
        SMSG_DAILY_QUESTS_RESET                     = 0x0C1B, ///< 6.2.0 20033

        /// Quest giver
        SMSG_QUEST_GIVER_STATUS_MULTIPLE            = 0x000D, ///< 6.2.0 20033
        SMSG_QUEST_GIVER_STATUS                     = 0x084D, ///< 6.2.0 20033
        SMSG_QUEST_GIVER_REQUEST_ITEMS              = 0x06CA, ///< 6.2.0 20033
        SMSG_QUEST_GIVER_QUEST_LIST_MESSAGE         = 0x05C9, ///< 6.2.0 20033
        SMSG_QUEST_GIVER_QUEST_FAILED               = 0x040E, ///< 6.2.0 20033
        SMSG_QUEST_GIVER_QUEST_DETAILS              = 0x015D, ///< 6.2.0 20033
        SMSG_QUEST_GIVER_QUEST_COMPLETE             = 0x021A, ///< 6.2.0 20033
        SMSG_QUEST_GIVER_OFFER_REWARD_MESSAGE       = 0x0949, ///< 6.2.0 20033
        SMSG_QUEST_GIVER_INVALID_QUEST              = 0x0689, ///< 6.2.0 20033

        /// Gossip
        SMSG_GOSSIP_MESSAGE                         = 0x0E0A, ///< 6.2.0 20033
        SMSG_GOSSIP_COMPLETE                        = 0x0D89, ///< 6.2.0 20033
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Lfg
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamLFG
        /// LFG
        SMSG_LFG_BOOT_PLAYER                              = 0x0611, ///< 6.2.0 20033
        SMSG_LFG_DISABLED                                 = 0x0C6C, ///< 6.2.0 20033
        SMSG_LFG_JOIN_RESULT                              = 0x0363, ///< 6.2.0 20033
        SMSG_LFG_OFFER_CONTINUE                           = 0x046B, ///< 6.2.0 20033
        SMSG_LFG_OPEN_FROM_GOSSIP                         = 0x0000, ///< (unused)
        SMSG_LFG_PARTY_INFO                               = 0x0256, ///< 6.2.0 20033
        SMSG_LFG_PLAYER_INFO                              = 0x0356, ///< 6.2.0 20033
        SMSG_LFG_PLAYER_REWARD                            = 0x066B, ///< 6.2.0 20033
        SMSG_LFG_PROPOSAL_UPDATE                          = 0x0D6B, ///< 6.2.0 20033
        SMSG_LFG_QUEUE_STATUS                             = 0x0255, ///< 6.2.0 20033
        SMSG_LFG_ROLE_CHECK_UPDATE                        = 0x0368, ///< 6.2.0 20033
        SMSG_LFG_ROLE_CHOSEN                              = 0x066F, ///< 6.2.0 20033
        SMSG_LFG_SEARCH_RESULT                            = 0x0000, ///< (unused)
        SMSG_LFG_SLOT_INVALID                             = 0x0000, ///< (unused)
        SMSG_LFG_TELEPORT_DENIED                          = 0x0364, ///< 6.2.0 20033
        SMSG_LFG_UPDATE_LIST                              = 0x0000, ///<
        SMSG_LFG_UPDATE_PARTY                             = 0x0000,
        SMSG_LFG_UPDATE_PLAYER                            = 0x0000,
        SMSG_LFG_UPDATE_SEARCH                            = 0x0000,
        SMSG_LFG_UPDATE_STATUS                            = 0x076B, ///< 6.2.0 20033
        SMSG_LFG_UPDATE_STATUS_NONE                       = 0x0000,
        SMSG_UPDATE_DUNGEON_ENCOUNTER_FOR_LOOT            = 0x0000,

        /// LFG List
        SMSG_LFG_LIST_JOIN_RESULT                         = 0x0252, ///< 6.2.0 20033
        SMSG_LFG_LIST_UPDATE_BLACKLIST                    = 0x086C, ///< 6.2.0 20033
        SMSG_LFG_LIST_UPDATE_STATUS                       = 0x0241, ///< 6.2.0 20033
        SMSG_LFG_LIST_SEARCH_RESULT                       = 0x052C, ///< 6.2.0 20033
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Garrison
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamGarrison
        SMSG_GET_GARRISON_INFO_RESULT                                   = 0x0B75, ///< 6.2.0 20033
        SMSG_GARRISON_PLOT_PLACED                                       = 0x0172, ///< 6.2.0 20033
        SMSG_GARRISON_PLOT_REMOVED                                      = 0x0866, ///< 6.2.0 20033
        SMSG_GARRISON_PLACE_BUILDING_RESULT                             = 0x08F1, ///< 6.2.0 20033
        SMSG_GARRISON_BUILDING_REMOVED                                  = 0x04F5, ///< 6.2.0 20033
        SMSG_GARRISON_LEARN_SPECIALIZATION_RESULT                       = 0x0000,
        SMSG_GARRISON_BUILDING_SET_ACTIVE_SPECIALIZATION_RESULT         = 0x0000,
        SMSG_GARRISON_LEARN_BLUEPRINT_RESULT                            = 0x0176, ///< 6.2.0 20033
        SMSG_GARRISON_BLUEPRINT_AND_SPECIALIZATION_DATA                 = 0x05F5, ///< 6.2.0 20033
        SMSG_GARRISON_GET_BUILDINGS_DATA                                = 0x0275, ///< 6.2.0 20033
        SMSG_GARRISON_REMOTE_INFO                                       = 0x01F5, ///< 6.2.0 20033
        SMSG_GARRISON_BUILDING_ACTIVATED                                = 0x0662, ///< 6.2.0 20033
        SMSG_GARRISON_UPGRADE_RESULT                                    = 0x0000,
        SMSG_GARRISON_ADD_FOLLOWER_RESULT                               = 0x05E2, ///< 6.2.0 20033
        SMSG_GARRISON_REMOVE_FOLLOWER_RESULT                            = 0x0000,
        SMSG_GARRISON_LIST_FOLLOWERS_CHEAT_RESULT                       = 0x0000,
        SMSG_GARRISON_UPDATE_FOLLOWER_ACTIVATION_COUNT                  = 0x0775, ///< 6.2.0 20033
        SMSG_GARRISON_UPDATE_FOLLOWER                                   = 0x0AE2, ///< 6.2.0 20033
        SMSG_GARRISON_FOLLOWER_CHANGED_XP                               = 0x04E1, ///< 6.2.0 20033
        SMSG_GARRISON_FOLLOWER_CHANGED_ITEM_LEVEL                       = 0x0000,
        SMSG_GARRISON_ADD_MISSION_RESULT                                = 0x0366, ///< 6.2.0 20033
        SMSG_GARRISON_START_MISSION_RESULT                              = 0x07E6, ///< 6.2.0 20033
        SMSG_GARRISON_COMPLETE_MISSION_RESULT                           = 0x01E6, ///< 6.2.0 20033
        SMSG_GARRISON_MISSION_BONUS_ROLL_RESULT                         = 0x01F2, ///< 6.2.0 20033
        SMSG_GARRISON_ASSIGN_FOLLOWER_TO_BUILDING_RESULT                = 0x0000,
        SMSG_GARRISON_REMOVE_FOLLOWER_FROM_BUILDING_RESULT              = 0x0000,
        SMSG_GARRISON_DELETE_RESULT                                     = 0x0000,
        SMSG_GARRISON_OPEN_ARCHITECT                                    = 0x0000,
        SMSG_GARRISON_OPEN_MISSION_NPC                                  = 0x09F5, ///< 6.2.0 20033
        SMSG_GARRISON_REQUEST_UPGRADEABLE_RESULT                        = 0x0566, ///< 6.2.0 20033
        SMSG_GARRISON_OPEN_TRADESKILL_NPC                               = 0x00E2, ///< 6.2.0 20033
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
    CMSG_GARRISON_CANCEL_CONSTRUCTION                       = 0x0000, ///<
    CMSG_GARRISON_LEARN_BUILDING_SPECIALIZATION_CHEAT       = 0x0000,
    CMSG_GARRISON_SET_ACTIVE_BUILDING_SPECIALIZATION        = 0x0000,
    CMSG_GARRISON_SET_ACTIVE_BUILDING_SPECIALIZATION_CHEAT  = 0x0000,
    CMSG_GET_GARRISON_INFO                                  = 0x0000, ///<
    CMSG_GARRISON_LEARN_BLUEPRINT_CHEAT                     = 0x0000,
    CMSG_GARRISON_REQUEST_BUILDINGS                         = 0x0000, ///<
    CMSG_GARRISON_PURCHASE_BUILDING                         = 0x0000, ///<
    CMSG_GARRISON_SET_BUILDING_ACTIVE                       = 0x0000,
    CMSG_GARRISON_FORCE_BUILDING_ACTIVE                     = 0x0000,
    CMSG_GARRISON_PORT_CHEAT                                = 0x0000,
    CMSG_GARRISON_ADD_FOLLOWER_XPCHEAT                      = 0x0000,
    CMSG_GARRISON_SET_FOLLOWER_ITEM_LEVEL_CHEAT             = 0x0000,
    CMSG_GARRISON_ASSIGN_FOLLOWER_TO_BUILDING               = 0x0000,
    CMSG_GARRISON_REMOVE_FOLLOWER_FROM_BUILDING             = 0x0000,
    CMSG_GARRISON_CHANGE_FOLLOWER_ACTIVATION_STATE          = 0x0000, ///<
    CMSG_SHOW_GARRISON_TRANSFER                             = 0x0000,
    CMSG_GARRISON_ADD_UNIQUE_FOLLOWER_CHEAT                 = 0x0000,
    CMSG_GARRISON_REMOVE_FOLLOWER_CHEAT                     = 0x0000,
    CMSG_GARRISON_LIST_FOLLOWERS_CHEAT                      = 0x0000,
    CMSG_GARRISON_ADD_MISSION_CHEAT                         = 0x0000,
    CMSG_GARRISON_START_MISSION                             = 0x0000, ///<
    CMSG_GARRISON_COMPLETE_MISSION                          = 0x0000, ///<
    CMSG_GARRISON_MISSION_BONUS_ROLL                        = 0x0000, ///<
    CMSG_GARRISON_REQUEST_UPGRADEABLE                       = 0x0000, ///<
    CMSG_UPGRADE_GARRISON                                   = 0x0000, ///<
    CMSG_GARRISON_REQUEST_LANDING_PAGE_SHIPMENT_INFO        = 0x0000,
    CMSG_GARRISON_MISSION_NPC_HELLO                         = 0x0000, ///<
    CMSG_GET_SHIPMENT_INFO                                  = 0x0000, ///<
    CMSG_CREATE_SHIPMENT                                    = 0x0000, ///<
    CMSG_GET_SHIPMENTS                                      = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// User Router
    //////////////////////////////////////////////////////////////////////////
    CMSG_LOG_STREAMING_ERROR                    = 0x0DD9, ///< 6.2.0 20033
    CMSG_QUEUED_MESSAGES_END                    = 0x069A, ///< 6.2.0 20033
    CMSG_LOG_DISCONNECT                         = 0x045D, ///< 6.2.0 20033
    CMSG_PING                                   = 0x0659, ///< 6.2.0 20033
    CMSG_AUTH_CONTINUED_SESSION                 = 0x06DA, ///< 6.2.0 20033
    CMSG_SUSPEND_TOKEN_RESPONSE                 = 0x0C9E, ///< 6.2.0 20033
    CMSG_AUTH_SESSION                           = 0x045A, ///< 6.2.0 20033
    CMSG_ENABLE_NAGLE                           = 0x065D, ///< 6.2.0 20033
    CMSG_SUSPEND_COMMS_ACK                      = 0x0C99, ///< 6.2.0 20033
    CMSG_KEEP_ALIVE                             = 0x0EA6, ///< 6.2.0 20033
    CMSG_OBJECT_UPDATE_FAILED                   = 0x0B2D, ///< 6.2.0 20033
    CMSG_OBJECT_UPDATE_RESCUED                  = 0x1988, ///< 6.2.0 20033

    //////////////////////////////////////////////////////////////////////////
    /// Character
    //////////////////////////////////////////////////////////////////////////
    CMSG_ENUM_CHARACTERS                        = 0x03C6, ///< 6.2.0 20033
    CMSG_CREATE_CHARACTER                       = 0x0F35, ///< 6.2.0 20033
    CMSG_CHAR_DELETE                            = 0x0BC5, ///< 6.2.0 20033
    CMSG_REORDER_CHARACTERS                     = 0x0A45, ///< 6.2.0 20033
    CMSG_PLAYER_LOGIN                           = 0x0921, ///< 6.2.0 20033
    CMSG_VIOLENCE_LEVEL                         = 0x1118, ///< 6.2.0 20033
    CMSG_LOAD_SCREEN                            = 0x0735, ///< 6.2.0 20033
    CMSG_REQUEST_ACCOUNT_DATA                   = 0x0CB6, ///< 6.2.0 20033
    CMSG_UPDATE_ACCOUNT_DATA                    = 0x0DB5, ///< 6.2.0 20033
    CMSG_SET_DUNGEON_DIFFICULTY                 = 0x0E16, ///< 6.2.0 20033
    CMSG_SET_RAID_DIFFICULTY                    = 0x0397, ///< 6.2.0 20033
    CMSG_AUTO_DECLINE_GUILD_INVITES             = 0x09B1, ///< 6.2.0 20033
    CMSG_SHOWING_CLOAK                          = 0x0843, ///< 6.2.0 20033
    CMSG_SHOWING_HELM                           = 0x0A5B, ///< 6.2.0 20033
    CMSG_SET_ACTIVE_MOVER                       = 0x12C3, ///< 6.2.0 20033
    CMSG_LEARN_TALENTS                          = 0x0AAA, ///< 6.2.0 20033
    CMSG_AUTOEQUIP_ITEM                         = 0x0A64, ///< 6.2.0 20033
    CMSG_SWAP_INV_ITEM                          = 0x0651, ///< 6.2.0 20033
    CMSG_SWAP_ITEM                              = 0x066C, ///< 6.2.0 20033
    CMSG_AUTOSTORE_BAG_ITEM                     = 0x0245, ///< 6.2.0 20033
    CMSG_REQUEST_PET_INFO                       = 0x0000, ///<
    CMSG_STAND_STATE_CHANGE                     = 0x1913, ///< 6.2.0 20033
    CMSG_BINDER_ACTIVATE                        = 0x0604, ///< 6.2.0 20033
    CMSG_REQUEST_FORCED_REACTIONS               = 0x12E8, ///< 6.2.0 20033
    CMSG_DESTROY_ITEM                           = 0x1331, ///< 6.2.0 20033
    CMSG_OPEN_ITEM                              = 0x1048, ///< 6.2.0 20033
    CMSG_SET_TITLE                              = 0x1676, ///< 6.2.0 20033
    CMSG_PLAYED_TIME                            = 0x18D8, ///< 6.2.0 20033
    CMSG_SAVE_EQUIPMENT_SET                     = 0x04CF, ///< 6.2.0 20033
    CMSG_USE_EQUIPMENT_SET                      = 0x0668, ///< 6.2.0 20033
    CMSG_DELETE_EQUIPMENT_SET                   = 0x0620, ///< 6.2.0 20033
    CMSG_WHO                                    = 0x0535, ///< 6.2.0 20033
    CMSG_SOCKET_GEMS                            = 0x0260, ///< 6.2.0 20033
    CMSG_RESURRECT_RESPONSE                     = 0x0521, ///< 6.2.0 20033
    CMSG_QUERY_INSPECT_ACHIEVEMENTS             = 0x0A4B, ///< 6.2.0 20033
    CMSG_SPLIT_ITEM                             = 0x0370, ///< 6.2.0 20033
    CMSG_SET_PLAYER_DECLINED_NAMES              = 0x0DB2, ///< 6.2.0 20033
    CMSG_MOUNT_SET_FAVORITE                     = 0x02D6, ///< 6.2.0 20033
    CMSG_SORT_BAGS                              = 0x1268, ///< 6.2.0 20033
    CMSG_CHAR_RENAME                            = 0x0C31, ///< 6.2.0 20033
    CMSG_CHAR_CUSTOMIZE                         = 0x0326, ///< 6.2.0 20033
    CMSG_CHAR_RACE_OR_FACTION_CHANGE            = 0x0EB2, ///< 6.2.0 20033

    //////////////////////////////////////////////////////////////////////////
    /// Bank
    //////////////////////////////////////////////////////////////////////////
    CMSG_BANKER_ACTIVATE                        = 0x0000, ///<
    CMSG_AUTOBANK_ITEM                          = 0x0000, ///<
    CMSG_AUTOSTORE_BANK_ITEM                    = 0x0000, ///<
    CMSG_BUY_BANK_SLOT                          = 0x0000, ///<
    CMSG_BUY_REAGENT_BANK                       = 0x0000, ///<
    CMSG_SORT_BANK_BAGS                         = 0x0000, ///<
    CMSG_SORT_REAGENT_BANK_BAGS                 = 0x0000, ///<
    CMSG_DEPOSIT_ALL_REAGENTS                   = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Auction House
    //////////////////////////////////////////////////////////////////////////
    CMSG_AUCTION_LIST_BIDDER_ITEMS              = 0x0000, ///<
    CMSG_AUCTION_LIST_ITEMS                     = 0x0000, ///<
    CMSG_AUCTION_LIST_OWNER_ITEMS               = 0x0000, ///<
    CMSG_AUCTION_LIST_PENDING_SALES             = 0x0000, ///<
    CMSG_AUCTION_PLACE_BID                      = 0x0000, ///<
    CMSG_AUCTION_REMOVE_ITEM                    = 0x0000, ///<
    CMSG_AUCTION_SELL_ITEM                      = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Loot
    //////////////////////////////////////////////////////////////////////////
    CMSG_LOOT                                   = 0x0000, ///<
    CMSG_LOOT_MONEY                             = 0x0000, ///<
    CMSG_LOOT_ITEM                              = 0x0000, ///<
    CMSG_LOOT_RELEASE                           = 0x0000, ///<
    CMSG_LOOT_ROLL                              = 0x0000, ///<
    CMSG_MASTER_LOOT_ITEM                       = 0x0000, ///<
    CMSG_DO_MASTER_LOOT_ROLL                    = 0x0000, ///<
    CMSG_SET_LOOT_SPECIALIZATION                = 0x0000, ///<
    CMSG_SET_LOOT_METHOD                        = 0x0000, ///<
    CMSG_OPT_OUT_OF_LOOT                        = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Combat
    //////////////////////////////////////////////////////////////////////////
    CMSG_ATTACKSTOP                             = 0x04D0, ///< 6.2.0 20033
    CMSG_ATTACKSWING                            = 0x1D58, ///< 6.2.0 20033

    //////////////////////////////////////////////////////////////////////////
    /// Duel
    //////////////////////////////////////////////////////////////////////////
    CMSG_DUEL_PROPOSED                          = 0x0235, ///< 6.2.0 20033
    CMSG_DUEL_RESPONSE                          = 0x0248, ///< 6.2.0 20033

    //////////////////////////////////////////////////////////////////////////
    /// Spell
    //////////////////////////////////////////////////////////////////////////
    CMSG_CAST_SPELL                             = 0x1632, ///< 6.2.0 20033
    CMSG_CANCEL_CAST                            = 0x1458, ///< 6.2.0 20033
    CMSG_USE_ITEM                               = 0x1144, ///< 6.2.0 20033
    CMSG_CANCEL_AURA                            = 0x14D3, ///< 6.2.0 20033
    CMSG_CANCEL_AUTO_REPEAT_SPELL               = 0x04D0, ///< 6.2.0 20033
    CMSG_CANCEL_CHANNELLING                     = 0x17F1, ///< 6.2.0 20033
    CMSG_CANCEL_GROWTH_AURA                     = 0x1C04, ///< 6.2.0 20033
    CMSG_CANCEL_MOUNT_AURA                      = 0x1518, ///< 6.2.0 20033
    CMSG_CANCEL_QUEUED_SPELL                    = 0x1225, ///< 6.2.0 20033

    //////////////////////////////////////////////////////////////////////////
    /// Cache
    //////////////////////////////////////////////////////////////////////////
    CMSG_DB_QUERY_BULK                          = 0x0A42, ///< 6.2.0 20033
    CMSG_QUERY_CREATURE                         = 0x1408, ///< 6.2.0 20033
    CMSG_NPC_TEXT_QUERY                         = 0x1E24, ///< 6.2.0 20033
    CMSG_NAME_QUERY                             = 0x0932, ///< 6.2.0 20033
    CMSG_QUEST_QUERY                            = 0x0FA9, ///< 6.2.0 20033
    CMSG_QUEST_POI_QUERY                        = 0x01A6, ///< 6.2.0 20033
    CMSG_REALM_NAME_QUERY                       = 0x0B21, ///< 6.2.0 20033
    CMSG_GAMEOBJECT_QUERY                       = 0x17F8, ///< 6.2.0 20033
    CMSG_PETITION_QUERY                         = 0x0FAC, ///< 6.2.0 20033
    CMSG_QUERY_GUILD_INFO                       = 0x0892, ///< 6.2.0 20033
    CMSG_PAGE_TEXT_QUERY                        = 0x1AA2, ///< 6.2.0 20033
    CMSG_ITEM_TEXT_QUERY                        = 0x1583, ///< 6.2.0 20033

    //////////////////////////////////////////////////////////////////////////
    /// Interaction
    //////////////////////////////////////////////////////////////////////////
    CMSG_LOGOUT_REQUEST                         = 0x085C, ///< 6.2.0 20033
    CMSG_LOGOUT_CANCEL                          = 0x00C8, ///< 6.2.0 20033
    CMSG_SET_ACTION_BUTTON                      = 0x0000, ///<
    CMSG_SET_ACTIONBAR_TOGGLES                  = 0x0000, ///<
    CMSG_SET_SELECTION                          = 0x0610, ///< 6.2.0 20033
    CMSG_SETSHEATHED                            = 0x0000, ///<
    CMSG_GOSSIP_HELLO                           = 0x0000, ///<
    CMSG_GOSSIP_SELECT_OPTION                   = 0x0000, ///<
    CMSG_TOGGLE_PVP                             = 0x0000, ///<
    CMSG_TUTORIAL                               = 0x0000, ///<
    CMSG_SET_SPECIALIZATION                     = 0x0000, ///<
    CMSG_SET_WATCHED_FACTION                    = 0x0000, ///<
    CMSG_SET_FACTION_INACTIVE                   = 0x0000, ///<
    CMSG_AREATRIGGER                            = 0x0000, ///<
    CMSG_GAMEOBJECT_REPORT_USE                  = 0x0000, ///<
    CMSG_GAMEOBJECT_USE                         = 0x0000, ///<
    CMSG_SAVE_CUF_PROFILES                      = 0x0000, ///<
    CMSG_SPELLCLICK                             = 0x0000, ///<
    CMSG_REPOP_REQUEST                          = 0x0000, ///<
    CMSG_RECLAIM_CORPSE                         = 0x0000, ///<
    CMSG_QUERY_CORPSE_LOCATION_FROM_CLIENT      = 0x0000, ///<
    CMSG_QUERY_CORPSE_TRANSPORT                 = 0x0000, ///<
    CMSG_RETURN_TO_GRAVEYARD                    = 0x0000, ///<
    CMSG_CLOSE_INTERACTION                      = 0x0000, ///<
    CMSG_ITEM_REFUND_INFO                       = 0x0000, ///<
    CMSG_FAR_SIGHT                              = 0x0000, ///<
    CMSG_MOUNTSPECIAL_ANIM                      = 0x0000, ///<
    CMSG_OPENING_CINEMATIC                      = 0x0000, ///<
    CMSG_NEXT_CINEMATIC_CAMERA                  = 0x0000, ///<
    CMSG_COMPLETE_CINEMATIC                     = 0x0000, ///<
    CMSG_REQUEST_CEMETERY_LIST                  = 0x0000, ///<
    CMSG_TOTEM_DESTROYED                        = 0x0000, ///<
    CMSG_CONFIRM_RESPEC_WIPE                    = 0x0000, ///<
    CMSG_CANCEL_TRADE                           = 0x0000, ///<
    CMSG_SET_TRADE_CURRENCY                     = 0x0000, ///<
    CMSG_SET_TRADE_GOLD                         = 0x0000, ///<
    CMSG_SET_TRADE_ITEM                         = 0x0000, ///<
    CMSG_CLEAR_TRADE_ITEM                       = 0x0000, ///<
    CMSG_ACCEPT_TRADE                           = 0x0000, ///<
    CMSG_BUSY_TRADE                             = 0x0000, ///<
    CMSG_BEGIN_TRADE                            = 0x0000, ///<
    CMSG_IGNORE_TRADE                           = 0x0000, ///<
    CMSG_INITIATE_TRADE                         = 0x0000, ///<
    CMSG_UNACCEPT_TRADE                         = 0x0000, ///<
    CMSG_NEUTRAL_PLAYER_SELECT_FACTION          = 0x0000, ///<
    CMSG_INSPECT                                = 0x0000, ///<
    CMSG_INSPECT_HONOR_STATS                    = 0x0000, ///<
    CMSG_REQUEST_INSPECT_RATED_BG_STATS         = 0x0000, ///<
    CMSG_TIME_SYNC_RESP                         = 0x1A94, ///< 6.2.0 20033
    CMSG_UNLEARN_SKILL                          = 0x0000, ///<
    CMSG_EMOTE                                  = 0x0000, ///<
    CMSG_SEND_TEXT_EMOTE                        = 0x0000, ///<
    CMSG_ALTER_APPEARANCE                       = 0x0000, ///<
    CMSG_SELF_RES                               = 0x0000, ///<
    CMSG_READ_ITEM                              = 0x0000, ///<
    CMSG_COMPLETE_MOVIE                         = 0x0000, ///<
    CMSG_SCENE_TRIGGER_EVENT                    = 0x0000, ///<
    CMSG_GET_MIRRORIMAGE_DATA                   = 0x0000, ///<
    CMSG_SHOW_TRADE_SKILL                       = 0x0000, ///<
    CMSG_SCENE_PLAYBACK_CANCELED                = 0x0000, ///<
    CMSG_REQUEST_RESEARCH_HISTORY               = 0x0000, ///<
    CMSG_SUMMON_RESPONSE                        = 0x0000, ///<
    CMSG_SET_FACTION_ATWAR                      = 0x0000, ///<
    CMSG_UNSET_FACTION_ATWAR                    = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Vehicles
    //////////////////////////////////////////////////////////////////////////
    CMSG_MOVE_SET_VEHICLE_REC_ID_ACK            = 0x0000, ///<
    CMSG_PLAYER_VEHICLE_ENTER                   = 0x0000, ///<
    CMSG_REQUEST_VEHICLE_EXIT                   = 0x0000, ///<
    CMSG_REQUEST_VEHICLE_NEXT_SEAT              = 0x0000, ///<
    CMSG_REQUEST_VEHICLE_PREV_SEAT              = 0x0000, ///<
    CMSG_REQUEST_VEHICLE_SWITCH_SEAT            = 0x0000, ///<
    CMSG_SET_VEHICLE_REC_ID_ACK                 = 0x0000, ///<
    CMSG_CHANGE_SEATS_ON_CONTROLLED_VEHICLE     = 0x0000, ///<
    CMSG_EJECT_PASSENGER                        = 0x0000, ///<
    CMSG_DISMISS_CONTROLLED_VEHICLE             = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Vendors
    //////////////////////////////////////////////////////////////////////////
    CMSG_LIST_INVENTORY                         = 0x0000, ///<
    CMSG_REPAIR_ITEM                            = 0x0000, ///<
    CMSG_BUYBACK_ITEM                           = 0x0000, ///<
    CMSG_BUY_ITEM                               = 0x0000, ///<
    CMSG_SELL_ITEM                              = 0x0000, ///<
    CMSG_ITEM_REFUND                            = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Taxi
    //////////////////////////////////////////////////////////////////////////
    CMSG_SET_TAXI_BENCHMARK_MODE                = 0x0000, ///<
    CMSG_ENABLE_TAXI_NODE                       = 0x0000, ///<
    CMSG_TAXI_HELLO                             = 0x0000, ///<
    CMSG_ACTIVATE_TAXI                          = 0x0000, ///<
    CMSG_TAXI_NODE_STATUS_QUERY                 = 0x0000, ///<
    CMSG_TAXI_REQUEST_EARLY_LANDING             = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Quest
    //////////////////////////////////////////////////////////////////////////
    CMSG_QUESTGIVER_HELLO                       = 0x0000, ///<
    CMSG_QUEST_GIVER_STATUS_QUERY               = 0x0000, ///<
    CMSG_QUESTGIVER_STATUS_MULTIPLE_QUERY       = 0x0000, ///<
    CMSG_QUESTGIVER_QUERY_QUEST                 = 0x0000, ///<
    CMSG_QUESTGIVER_COMPLETE_QUEST              = 0x0000, ///<
    CMSG_QUESTGIVER_CHOOSE_REWARD               = 0x0000, ///<
    CMSG_QUESTGIVER_ACCEPT_QUEST                = 0x0000, ///<
    CMSG_QUESTGIVER_REQUEST_REWARD              = 0x0000, ///<
    CMSG_QUEST_CONFIRM_ACCEPT                   = 0x0000, ///<
    CMSG_QUESTLOG_REMOVE_QUEST                  = 0x0000, ///<
    CMSG_PUSH_QUEST_TO_PARTY                    = 0x0000, ///<
    CMSG_QUEST_PUSH_RESULT                      = 0x0000, ///<
    CMSG_QUERY_QUEST_COMPLETION_NPCS            = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Account data
    //////////////////////////////////////////////////////////////////////////
    CMSG_GET_UNDELETE_CHARACTER_COOLDOWN_STATUS = 0x0946, ///< 6.2.0 20033

    //////////////////////////////////////////////////////////////////////////
    /// Chat
    //////////////////////////////////////////////////////////////////////////
    /// Addon chat
    CMSG_CHAT_ADDON_MESSAGE_INSTANCE_CHAT       = 0x0000, ///<
    CMSG_CHAT_ADDON_MESSAGE_WHISPER             = 0x0000, ///<
    CMSG_CHAT_ADDON_MESSAGE_GUILD               = 0x0000, ///<
    CMSG_CHAT_ADDON_MESSAGE_OFFICER             = 0x0000, ///<
    CMSG_CHAT_ADDON_MESSAGE_RAID                = 0x0000, ///<
    CMSG_CHAT_ADDON_MESSAGE_PARTY               = 0x0000, ///<
    CMSG_ADDON_REGISTERED_PREFIXES              = 0x0000, ///<

    /// Chat
    CMSG_CHAT_MESSAGE_RAID_WARNING              = 0x0000, ///<
    CMSG_CHAT_MESSAGE_PARTY                     = 0x0000, ///<
    CMSG_CHAT_MESSAGE_YELL                      = 0x0000, ///<
    CMSG_CHAT_MESSAGE_SAY                       = 0x0AE9, ///< 6.2.0 20033
    CMSG_CHAT_MESSAGE_OFFICER                   = 0x0000, ///<
    CMSG_CHAT_MESSAGE_EMOTE                     = 0x0000, ///<
    CMSG_CHAT_MESSAGE_AFK                       = 0x0000, ///<
    CMSG_CHAT_MESSAGE_DND                       = 0x0000, ///<
    CMSG_CHAT_MESSAGE_GUILD                     = 0x0000, ///<
    CMSG_CHAT_MESSAGE_RAID                      = 0x0000, ///<
    CMSG_CHAT_MESSAGE_WHISPER                   = 0x0000, ///<
    CMSG_CHAT_MESSAGE_CHANNEL                   = 0x0000, ///<

    /// Channel
    CMSG_JOIN_CHANNEL                           = 0x0000, ///<
    CMSG_CHANNEL_ANNOUNCEMENTS                  = 0x0000, ///<
    CMSG_CHANNEL_BAN                            = 0x0000, ///<
    CMSG_CHANNEL_DISPLAY_LIST                   = 0x0000, ///<
    CMSG_CHANNEL_INVITE                         = 0x0000, ///<
    CMSG_CHANNEL_KICK                           = 0x0000, ///<
    CMSG_CHANNEL_LIST                           = 0x0000, ///<
    CMSG_CHANNEL_MODERATOR                      = 0x0000, ///<
    CMSG_CHANNEL_MUTE                           = 0x0000, ///<
    CMSG_CHANNEL_OWNER                          = 0x0000, ///<
    CMSG_CHANNEL_PASSWORD                       = 0x0000, ///<
    CMSG_CHANNEL_SET_OWNER                      = 0x0000, ///<
    CMSG_CHANNEL_SILENCE_ALL                    = 0x0000, ///<
    CMSG_CHANNEL_SILENCE_VOICE                  = 0x0000, ///<
    CMSG_CHANNEL_UNBAN                          = 0x0000, ///<
    CMSG_CHANNEL_UNMODERATOR                    = 0x0000, ///<
    CMSG_CHANNEL_UNMUTE                         = 0x0000, ///<
    CMSG_CHANNEL_UNSILENCE_ALL                  = 0x0000, ///<
    CMSG_CHANNEL_UNSILENCE_VOICE                = 0x0000, ///<
    CMSG_CHANNEL_VOICE_OFF                      = 0x0000, ///<
    CMSG_CHANNEL_VOICE_ON                       = 0x0000, ///<
    CMSG_DECLINE_CHANNEL_INVITE                 = 0x0000, ///<
    CMSG_LEAVE_CHANNEL                          = 0x0000, ///<
    CMSG_SET_ACTIVE_VOICE_CHANNEL               = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Movement
    //////////////////////////////////////////////////////////////////////////
    /// Start
    CMSG_MOVE_START_FORWARD                     = 0x1B97, ///< 6.2.0 20033
    CMSG_MOVE_START_TURN_LEFT                   = 0x1298, ///< 6.2.0 20033
    CMSG_MOVE_START_TURN_RIGHT                  = 0x1A03, ///< 6.2.0 20033
    CMSG_MOVE_START_BACKWARD                    = 0x1294, ///< 6.2.0 20033
    CMSG_MOVE_START_STRAFE_LEFT                 = 0x1A53, ///< 6.2.0 20033
    CMSG_MOVE_START_STRAFE_RIGHT                = 0x1657, ///< 6.2.0 20033
    CMSG_MOVE_START_ASCEND                      = 0x17C4, ///< 6.2.0 20033
    CMSG_MOVE_START_DESCEND                     = 0x1B43, ///< 6.2.0 20033
    CMSG_MOVE_START_PITCH_UP                    = 0x12D7, ///< 6.2.0 20033
    CMSG_MOVE_START_PITCH_DOWN                  = 0x1704, ///< 6.2.0 20033
    CMSG_MOVE_START_SWIM                        = 0x13C8, ///< 6.2.0 20033

    /// Misc
    CMSG_MOVE_JUMP                              = 0x1A18, ///< 6.2.0 20033
    CMSG_MOVE_FALL_LAND                         = 0x12C8, ///< 6.2.0 20033
    CMSG_WORLD_PORT_RESPONSE                    = 0x04B6, ///< 6.2.0 20033
    CMSG_MOVE_TIME_SKIPPED                      = 0x1317, ///< 6.2.0 20033
    CMSG_MOVE_SPLINE_DONE                       = 0x16C8, ///< 6.2.0 20033

    /// Update
    CMSG_MOVE_HEARTBEAT                         = 0x1B18, ///< 6.2.0 20033
    CMSG_MOVE_SET_FACING                        = 0x1A13, ///< 6.2.0 20033
    CMSG_MOVE_SET_PITCH                         = 0x1797, ///< 6.2.0 20033
    CMSG_MOVE_SET_RUN_MODE                      = 0x1287, ///< 6.2.0 20033
    CMSG_MOVE_SET_WALK_MODE                     = 0x16C4, ///< 6.2.0 20033
    CMSG_MOVE_TELEPORT_ACK                      = 0x17C8, ///< 6.2.0 20033
    CMSG_MOVE_CHNG_TRANSPORT                    = 0x1B13, ///< 6.2.0 20033

    /// Stop
    CMSG_MOVE_STOP                              = 0x1A08, ///< 6.2.0 20033
    CMSG_MOVE_STOP_TURN                         = 0x16D7, ///< 6.2.0 20033
    CMSG_MOVE_STOP_STRAFE                       = 0x1218, ///< 6.2.0 20033
    CMSG_MOVE_STOP_SWIM                         = 0x1247, ///< 6.2.0 20033
    CMSG_MOVE_STOP_ASCEND                       = 0x1303, ///< 6.2.0 20033
    CMSG_MOVE_STOP_PITCH                        = 0x1A98, ///< 6.2.0 20033

    /// ACK
    CMSG_MOVE_SET_RUN_SPEED_CHEAT               = 0x1383, ///< 6.2.0 20033
    CMSG_MOVE_SET_RUN_BACK_SPEED_CHEAT          = 0x1A04, ///< 6.2.0 20033
    CMSG_MOVE_SET_WALK_SPEED_CHEAT              = 0x1E18, ///< 6.2.0 20033
    CMSG_MOVE_SET_SWIM_SPEED_CHEAT              = 0x1314, ///< 6.2.0 20033
    CMSG_MOVE_SET_SWIM_BACK_SPEED_CHEAT         = 0x1B83, ///< 6.2.0 20033
    CMSG_MOVE_SET_FLIGHT_SPEED_CHEAT            = 0x1A44, ///< 6.2.0 20033
    CMSG_MOVE_SET_FLIGHT_BACK_SPEED_CHEAT       = 0x1397, ///< 6.2.0 20033
    CMSG_MOVE_SET_TURN_SPEED_CHEAT              = 0x1683, ///< 6.2.0 20033
    CMSG_MOVE_SET_PITCH_SPEED_CHEAT             = 0x12C4, ///< 6.2.0 20033
    CMSG_MOVE_KNOCK_BACK_ACK                    = 0x1244, ///< 6.2.0 20033
    CMSG_MOVE_SET_CAN_FLY_ACK                   = 0x1798, ///< 6.2.0 20033
    CMSG_MOVE_FEATHER_FALL_ACK                  = 0x1707, ///< 6.2.0 20033
    CMSG_MOVE_WATER_WALK_ACK                    = 0x1754, ///< 6.2.0 20033
    CMSG_MOVE_HOVER_ACK                         = 0x1284, ///< 6.2.0 20033

    //////////////////////////////////////////////////////////////////////////
    /// Group
    //////////////////////////////////////////////////////////////////////////
    CMSG_PARTY_INVITE                           = 0x0000, ///<
    CMSG_PARTY_INVITE_RESPONSE                  = 0x0000, ///<
    CMSG_LEAVE_GROUP                            = 0x0000, ///<
    CMSG_PARTY_UNINVITE                         = 0x0000, ///<
    CMSG_SET_PARTY_LEADER                       = 0x0000, ///<
    CMSG_SET_ROLE                               = 0x0000, ///<
    CMSG_MINIMAP_PING                           = 0x0000, ///<
    CMSG_RANDOM_ROLL                            = 0x0000, ///<
    CMSG_UPDATE_RAID_TARGET                     = 0x0000, ///<
    CMSG_CONVERT_RAID                           = 0x0000, ///<
    CMSG_DO_READY_CHECK                         = 0x0000, ///<
    CMSG_READY_CHECK_RESPONSE                   = 0x0000, ///<
    CMSG_INITIATE_ROLE_POLL                     = 0x0000, ///<
    CMSG_REQUEST_RAID_INFO                      = 0x0000, ///<
    CMSG_SET_EVERYONE_IS_ASSISTANT              = 0x0000, ///<
    CMSG_CLEAR_RAID_MARKER                      = 0x0000, ///<
    CMSG_GROUP_ASSISTANT_LEADER                 = 0x0000, ///<
    CMSG_GROUP_ASSIGNMENT                       = 0x0000, ///<
    CMSG_GROUP_CHANGE_SUB_GROUP                 = 0x0000, ///<
    CMSG_GROUP_REQUEST_JOIN_UPDATES             = 0x0000, ///<
    CMSG_GROUP_SWAP_SUB_GROUP                   = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Friend
    //////////////////////////////////////////////////////////////////////////
    CMSG_SEND_CONTACT_LIST                      = 0x0000, ///<
    CMSG_ADD_FRIEND                             = 0x0000, ///<
    CMSG_ADD_IGNORE                             = 0x0000, ///<
    CMSG_SET_CONTACT_NOTES                      = 0x0000, ///<
    CMSG_DEL_FRIEND                             = 0x0000, ///<
    CMSG_DEL_IGNORE                             = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Petition
    //////////////////////////////////////////////////////////////////////////
    CMSG_OFFER_PETITION                         = 0x0000, ///<
    CMSG_PETITION_BUY                           = 0x0000, ///<
    CMSG_PETITION_DECLINE                       = 0x0000, ///<
    CMSG_PETITION_RENAME                        = 0x0000, ///<
    CMSG_PETITION_SHOWLIST                      = 0x0000, ///<
    CMSG_PETITION_SHOW_SIGNATURES               = 0x0000, ///<
    CMSG_PETITION_SIGN                          = 0x0000, ///<
    CMSG_TURN_IN_PETITION                       = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Pet
    //////////////////////////////////////////////////////////////////////////
    CMSG_PET_NAME_QUERY                         = 0x0000, ///<
    CMSG_PET_RENAME                             = 0x0000, ///<
    CMSG_PET_ABANDON                            = 0x0000, ///<
    CMSG_PET_ACTION                             = 0x0000, ///<
    CMSG_PET_CANCEL_AURA                        = 0x0000, ///<
    CMSG_PET_CAST_SPELL                         = 0x0000, ///<
    CMSG_PET_SET_ACTION                         = 0x0000, ///<
    CMSG_PET_STOP_ATTACK                        = 0x0000, ///<
    CMSG_LEARN_PET_SPECIALIZATION_GROUP         = 0x0000, ///<
    CMSG_LIST_STABLE_PETS                       = 0x0000, ///<
    CMSG_SET_PET_SLOT                           = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Battlegrounds
    //////////////////////////////////////////////////////////////////////////
    CMSG_BATTLEMASTER_JOIN                      = 0x0000, ///<
    CMSG_BATTLEMASTER_JOIN_ARENA                = 0x0000, ///<
    CMSG_BATTLEMASTER_JOIN_ARENA_SKIRMISH       = 0x0000, ///<
    CMSG_JOIN_RATED_BATTLEGROUND                = 0x0000, ///<
    CMSG_BATTLEFIELD_PORT                       = 0x0000, ///<
    CMSG_REQUEST_BATTLEFIELD_STATUS             = 0x0000, ///<
    CMSG_BATTLEFIELD_REQUEST_SCORE_DATA         = 0x0000, ///<
    CMSG_REQUEST_PVP_REWARDS                    = 0x0000, ///<
    CMSG_REQUEST_PVP_OPTIONS_ENABLED            = 0x0000, ///<
    CMSG_QUERY_COUNTDOWN_TIMER                  = 0x06F1, ///< (unused)
    CMSG_REQUEST_CONQUEST_FORMULA_CONSTANTS     = 0x0000, ///<
    CMSG_LEAVE_BATTLEFIELD                      = 0x0000, ///<
    CMSG_SPIRIT_HEALER_ACTIVATE                 = 0x0000, ///<
    CMSG_AREA_SPIRIT_HEALER_QUERY               = 0x0000, ///<
    CMSG_AREA_SPIRIT_HEALER_QUEUE               = 0x0000, ///<
    CMSG_BATTLEFIELD_MGR_QUEUE_REQUEST          = 0x0000, ///<
    CMSG_BATTLEFIELD_MGR_QUEUE_INVITE_RESPONSE  = 0x0000, ///<
    CMSG_BATTLEFIELD_MGR_QUEUE_EXIT_REQUEST     = 0x0000, ///<
    CMSG_BATTLEFIELD_MGR_ENTRY_INVITE_RESPONSE  = 0x0000, ///<
    CMSG_BATTLEFIELD_LIST                       = 0x0000, ///<
    CMSG_BATTLEFIELD_LEAVE                      = 0x0000,
    CMSG_REQUEST_RATED_BATTLEFIELD_INFO         = 0x0000, ///<
    CMSG_START_WAR_GAME                         = 0x0000, ///<
    CMSG_ACCEPT_WARGAME_INVITE                  = 0x0000, ///<
    CMSG_REPORT_PVP_AFK                         = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Guild
    //////////////////////////////////////////////////////////////////////////
    CMSG_GUILD_INVITE_BY_NAME                      = 0x0000, ///<
    CMSG_ACCEPT_GUILD_INVITE                       = 0x0000, ///<
    CMSG_GUILD_DECLINE_INVITATION                  = 0x0000, ///<
    CMSG_GUILD_OFFICER_REMOVE_MEMBER               = 0x0000, ///<
    CMSG_GUILD_SET_GUILD_MASTER                    = 0x0000, ///<
    CMSG_GUILD_LEAVE                               = 0x0000, ///<
    CMSG_GUILD_DELETE                              = 0x0000, ///<
    CMSG_GUILD_UPDATE_MOTD_TEXT                    = 0x0000, ///<
    CMSG_GUILD_UPDATE_INFO_TEXT                    = 0x0000, ///<
    CMSG_GUILD_GET_RANKS                           = 0x0000, ///<
    CMSG_GUILD_ADD_RANK                            = 0x0000, ///<
    CMSG_GUILD_DELETE_RANK                         = 0x0000, ///<
    CMSG_GUILD_SET_RANK_PERMISSIONS                = 0x0000, ///<
    CMSG_GUILD_SHIFT_RANK                          = 0x0000, ///<
    CMSG_GUILD_ASSIGN_MEMBER_RANK                  = 0x0000, ///<
    CMSG_GUILD_GET_ROSTER                          = 0x0000, ///<
    CMSG_GUILD_BANK_ACTIVATE                       = 0x0000, ///<
    CMSG_GUILD_BANK_BUY_TAB                        = 0x0000, ///<
    CMSG_GUILD_BANK_DEPOSIT_MONEY                  = 0x0000, ///<
    CMSG_GUILD_BANK_LOG_QUERY                      = 0x0000, ///<
    CMSG_GUILD_BANK_REMAINING_WITHDRAW_MONEY_QUERY = 0x0000, ///<
    CMSG_GUILD_BANK_QUERY_TAB                      = 0x0000, ///<
    CMSG_GUILD_BANK_TEXT_QUERY                     = 0x0000, ///<
    CMSG_GUILD_BANK_SWAP_ITEMS                     = 0x0000, ///<
    CMSG_GUILD_BANK_UPDATE_TAB                     = 0x0000, ///<
    CMSG_GUILD_BANK_WITHDRAW_MONEY                 = 0x0000, ///<
    CMSG_GUILD_BANK_SET_TAB_TEXT                   = 0x0000, ///<
    CMSG_GUILD_GET_ACHIEVEMENT_MEMBERS             = 0x0000, ///< (unused)
    CMSG_GUILD_SET_FOCUSED_ACHIEVEMENT             = 0x0000, ///<
    CMSG_GUILD_SET_ACHIEVEMENT_TRACKING            = 0x0000, ///< (unused)
    CMSG_GUILD_SET_MEMBER_NOTE                     = 0x0000, ///<
    CMSG_GUILD_CHALLENGE_UPDATE_REQUEST            = 0x0000, ///<
    CMSG_REQUEST_GUILD_PARTY_STATE                 = 0x0000, ///<
    CMSG_REQUEST_GUILD_REWARDS_LIST                = 0x0000, ///<
    CMSG_GUILD_REPLACE_GUILD_MASTER                = 0x0000, ///<
    CMSG_GUILD_CHANGE_NAME_REQUEST                 = 0x0000, ///<  (unused)
    CMSG_GUILD_PERMISSIONS_QUERY                   = 0x0000, ///<
    CMSG_GUILD_EVENT_LOG_QUERY                     = 0x0000, ///<
    CMSG_GUILD_NEWS_UPDATE_STICKY                  = 0x0000, ///<
    CMSG_GUILD_QUERY_NEWS                          = 0x0000, ///<
    CMSG_QUERY_GUILD_MEMBERS_FOR_RECIPE            = 0x0000, ///< (unused)
    CMSG_QUERY_GUILD_MEMBER_RECIPES                = 0x0000, ///< (unused)
    CMSG_QUERY_GUILD_RECIPES                       = 0x0000, ///<
    CMSG_PLAYER_SAVE_GUILD_EMBLEM                  = 0x0000, ///<

    /// Guild finding
    CMSG_LF_GUILD_ADD_RECRUIT                      = 0x0000, ///<
    CMSG_LF_GUILD_BROWSE                           = 0x0000, ///<
    CMSG_LF_GUILD_DECLINE_RECRUIT                  = 0x0000, ///<
    CMSG_LF_GUILD_GET_APPLICATIONS                 = 0x0000, ///<
    CMSG_LF_GUILD_GET_RECRUITS                     = 0x0000, ///<
    CMSG_LF_GUILD_POST_REQUEST                     = 0x0000, ///<
    CMSG_LF_GUILD_REMOVE_RECRUIT                   = 0x0000, ///<
    CMSG_LF_GUILD_SET_GUILD_POST                   = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Battle pet opcodes
    //////////////////////////////////////////////////////////////////////////
    CMSG_SUMMON_COMPANION                          = 0x0000, ///<
    CMSG_PETBATTLE_QUERY_JOURNAL                   = 0x0000, ///<
    CMSG_PETBATTLE_SET_ABILITY                     = 0x0000, ///<
    CMSG_PETBATTLE_RENAME                          = 0x0000, ///<
    CMSG_PETBATTLE_CAGE_PET                        = 0x0000,
    CMSG_PETBATTLE_QUERY_NAME                      = 0x0000, ///<
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
    CMSG_BATTLE_PAY_GET_PURCHASE_LIST              = 0x0621, ///< (unused) 6.2.0 20033

    //////////////////////////////////////////////////////////////////////////
    /// LFG
    //////////////////////////////////////////////////////////////////////////
    CMSG_DFGET_SYSTEM_INFO                         = 0x0000, ///<
    CMSG_LFG_GET_PLAYER_INFO                       = 0x0000, 
    CMSG_LFG_GET_STATUS                            = 0x0000, ///<
    CMSG_LFG_JOIN                                  = 0x0000, ///<
    CMSG_LFG_LEAVE                                 = 0x0000, ///<
    CMSG_LFG_PROPOSAL_RESULT                       = 0x0000, ///<
    CMSG_LFG_SET_BOOT_VOTE                         = 0x0000, ///<
    CMSG_LFG_SET_COMMENT                           = 0x0000, ///< (unused)
    CMSG_DFSET_ROLES                               = 0x0000, ///<
    CMSG_LFG_TELEPORT                              = 0x0000, ///<
    CMSG_SEARCH_LFG_JOIN                           = 0x0000, ///< (unused)
    CMSG_SEARCH_LFG_LEAVE                          = 0x0000, ///< (unused)
    CMSG_RESET_INSTANCES                           = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// LFG LIST
    //////////////////////////////////////////////////////////////////////////
    CMSG_LFG_LIST_JOIN                              = 0x0000, ///<
    CMSG_LFG_LIST_UPDATE_REQUEST                    = 0x0000, ///<
    CMSG_LFG_LIST_LEAVE                             = 0x0000, ///<
    CMSG_LFG_LIST_SEARCH                            = 0x0000, ///<
    CMSG_REQUEST_LFG_LIST_BLACKLIST                 = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Auction House
    //////////////////////////////////////////////////////////////////////////
    CMSG_AUCTION_HELLO                             = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Trainers
    //////////////////////////////////////////////////////////////////////////
    CMSG_TRAINER_LIST                              = 0x0000, ///<
    CMSG_TRAINER_BUY_SPELL                         = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Void storage
    //////////////////////////////////////////////////////////////////////////
    CMSG_VOID_STORAGE_QUERY                        = 0x0000, ///<
    CMSG_VOID_STORAGE_TRANSFER                     = 0x0000, ///<
    CMSG_VOID_STORAGE_UNLOCK                       = 0x0000, ///<
    CMSG_VOID_SWAP_ITEM                            = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Transmogrification
    //////////////////////////////////////////////////////////////////////////
    CMSG_TRANSMOGRIFY_ITEMS                        = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Mail
    //////////////////////////////////////////////////////////////////////////
    CMSG_GET_MAIL_LIST                             = 0x0000, ///<
    CMSG_MAIL_CREATE_TEXT_ITEM                     = 0x0000, ///<
    CMSG_MAIL_DELETE                               = 0x0000, ///<
    CMSG_MAIL_MARK_AS_READ                         = 0x0000, ///<
    CMSG_MAIL_RETURN_TO_SENDER                     = 0x0000, ///<
    CMSG_MAIL_TAKE_ITEM                            = 0x0000, ///<
    CMSG_MAIL_TAKE_MONEY                           = 0x0000, ///<
    CMSG_SEND_MAIL                                 = 0x0000, ///<
    CMSG_QUERY_NEXT_MAIL_TIME                      = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Time
    //////////////////////////////////////////////////////////////////////////
    CMSG_UITIME_REQUEST                            = 0x0F32, ///< 6.2.0 20033

    //////////////////////////////////////////////////////////////////////////
    /// GameMasters
    //////////////////////////////////////////////////////////////////////////
    CMSG_GMRESPONSE_RESOLVE                         = 0x0000, ///<
    CMSG_GMSURVEY_SUBMIT                            = 0x0000, ///<
    CMSG_GMTICKET_CREATE                            = 0x0000, ///<
    CMSG_GMTICKET_DELETETICKET                      = 0x0000, ///<
    CMSG_GMTICKET_GETTICKET                         = 0x0000, ///<
    CMSG_GMTICKET_GET_CASE_STATUS                   = 0x0000, ///<
    CMSG_GMTICKET_SYSTEMSTATUS                      = 0x0000, ///<
    CMSG_GMTICKET_UPDATETEXT                        = 0x0000, ///<
    CMSG_GM_REPORT_LAG                              = 0x0000, ///<
    CMSG_REPORT_BUG                                 = 0x0000, ///<
    CMSG_REPORT_SUGGESTION                          = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// ToyBox
    //////////////////////////////////////////////////////////////////////////
    CMSG_ADD_NEW_TOY_TO_BOX                         = 0x0000, ///<
    CMSG_SET_FAVORITE_TOY                           = 0x0000, ///<
    CMSG_USE_TOY                                    = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Calendar
    //////////////////////////////////////////////////////////////////////////
    CMSG_CALENDAR_ADD_EVENT                         = 0x0000, ///<
    CMSG_CALENDAR_COMPLAIN                          = 0x0000, ///<
    CMSG_CALENDAR_COPY_EVENT                        = 0x0000, ///<
    CMSG_CALENDAR_EVENT_INVITE                      = 0x0000, ///<
    CMSG_CALENDAR_EVENT_MODERATOR_STATUS            = 0x0000, ///<
    CMSG_CALENDAR_EVENT_REMOVE_INVITE               = 0x0000, ///<
    CMSG_CALENDAR_EVENT_RSVP                        = 0x0000, ///<
    CMSG_CALENDAR_EVENT_SIGNUP                      = 0x0000, ///<
    CMSG_CALENDAR_EVENT_STATUS                      = 0x0000, ///<
    CMSG_CALENDAR_GET_CALENDAR                      = 0x0000, ///<
    CMSG_CALENDAR_GET_EVENT                         = 0x0000, ///<
    CMSG_CALENDAR_GET_NUM_PENDING                   = 0x0000, ///<
    CMSG_CALENDAR_GUILD_FILTER                      = 0x0000, ///<
    CMSG_CALENDAR_REMOVE_EVENT                      = 0x0000, ///<
    CMSG_CALENDAR_UPDATE_EVENT                      = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Challenges
    //////////////////////////////////////////////////////////////////////////
    CMSG_CHALLENGE_MODE_REQUEST_LEADERS             = 0x0000, ///<
    CMSG_GET_CHALLENGE_MODE_REWARDS                 = 0x0000, ///<
    CMSG_CHALLENGE_MODE_REQUEST_MAP_STATS           = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// BlackMarket
    //////////////////////////////////////////////////////////////////////////
    CMSG_BLACK_MARKET_OPEN                         = 0x0000, ///<
    CMSG_BLACK_MARKET_REQUEST_ITEMS                = 0x0000, ///<
    CMSG_BLACK_MARKET_PLACE_BID                    = 0x0000, ///<

    //////////////////////////////////////////////////////////////////////////
    /// Twitter
    //////////////////////////////////////////////////////////////////////////
    CMSG_REQUEST_TWITTER_STATUS                     = 0x0000, ///<

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
    CMSG_SUSPEND_TOKEN                                  = 0x0FA5, ///< 6.2.0 20033
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
    CMSG_WHOIS                                          = 0x17BF,
    CMSG_WORLD_TELEPORT                                 = 0x0000,
    CMSG_WRAP_ITEM                                      = 0x0000,
    CMSG_ARENA_2v2_STATS_REQUEST                        = 0x0000,
    CMSG_ARENA_3v3_STATS_REQUEST                        = 0x0000,

    // SMSG
    SMSG_ACCOUNT_PROFILE                                = 0x0000,
    SMSG_ADJUST_SPLINE_DURATION                         = 0x0000,
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
    SMSG_SET_MELEE_ANIM_KIT                             = 0x0000,
    SMSG_SET_MOVEMENT_ANIM_KIT                          = 0x0000,
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
    SMSG_FLIGHT_SPLINE_SYNC                             = 0x0000,
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
    SMSG_NOTIFY_MONEY                                   = 0x01D1, ///< 6.2.0 20033
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
    SMSG_SET_PLAY_HOVER_ANIM                            = 0x0000,
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
