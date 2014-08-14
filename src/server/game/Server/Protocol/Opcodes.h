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
        SMSG_AUTH_CHALLENGE                         = 0x0114,   ///< 6.0.1 18556
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Dispatch
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamDispatch
        /// Base opcodes
        SMSG_AUTH_RESPONSE                          = 0x0028,   ///< 6.0.1 18716
        SMSG_CACHE_VERSION                          = 0x10CE,   ///< 6.0.1 18716
        SMSG_ADDON_INFO                             = 0x184A,   ///< 6.0.1 18716
        SMSG_FEATURE_SYSTEM_STATUS                  = 0x006D,   ///< 6.0.1 18716
        SMSG_SET_TIME_ZONE_INFORMATION              = 0x137D,   ///< 6.0.1 18716
        SMSG_LOGIN_VERIFY_WORLD                     = 0x16C5,   ///< 6.0.1 18716
        SMSG_NEW_WORLD                              = 0x0E09,   ///< 6.0.1 18716
        SMSG_LOGIN_SET_TIME_SPEED                   = 0x02FD,   ///< 6.0.1 18716
        SMSG_HOTFIX_NOTIFY_BLOB                     = 0x1A49,   ///< 6.0.1 18716
        SMSG_HOTFIX_NOTIFY                          = 0x1FC6,   ///< 6.0.1 18716
        SMSG_WORLD_SERVER_INFO                      = 0x1F61,   ///< 6.0.1 18716
        SMSG_INITIAL_SETUP                          = 0x07C1,   ///< 6.0.1 18716
        SMSG_PONG                                   = 0x011B,   ///< 6.0.1 18716

        /// Query
        SMSG_DB_REPLY                               = 0x116A,   ///< 6.0.1 18716
        SMSG_CREATURE_QUERY_RESPONSE                = 0x1566,   ///< 6.0.1 18716
        SMSG_NPC_TEXT_UPDATE                        = 0x1059,   ///< 6.0.1 18716
        SMSG_NAME_QUERY_RESPONSE                    = 0x1462,   ///< 6.0.1 18716
        SMSG_REALM_QUERY_RESPONSE                   = 0x0E58,   ///< 6.0.1 18716
        SMSG_GAMEOBJECT_QUERY_RESPONSE              = 0x127A,   ///< 6.0.1 18716

        /// Misc
        SMSG_UPDATE_ACTION_BUTTONS                  = 0x1E19,   ///< 6.0.1 18716
        SMSG_SET_PROFICIENCY                        = 0x1AFD,   ///< 6.0.1 18716
        SMSG_INIT_WORLD_STATES                      = 0x0B6D,   ///< 6.0.1 18716
        SMSG_UPDATE_WORLD_STATE                     = 0x12ED,   ///< 6.0.1 18716
        SMSG_EMOTE                                  = 0x086D,   ///< 6.0.1 18716
        SMSG_EXPLORATION_EXPERIENCE                 = 0x0288,   ///< 6.0.1 18716
        SMSG_LOG_XP_GAIN                            = 0x04E1,   ///< 6.0.1 18716
        SMSG_LEVELUP_INFO                           = 0x1CE2,   ///< 6.0.1 18716
        SMSG_ITEM_PUSH_RESULT                       = 0x0EBC,   ///< 6.0.1 18716
        SMSG_INIT_CURRENCY                          = 0x1653,   ///< 6.0.1 18716
        SMSG_EQUIPMENT_SET_LIST                     = 0x1654,   ///< 6.0.1 18716
        SMSG_INITIALIZE_FACTIONS                    = 0x02FA,   ///< 6.0.1 18716
        SMSG_BIND_POINT_UPDATE                      = 0x1D46,   ///< 6.0.1 18716
        SMSG_UPDATE_TALENT_DATA                     = 0x00CD,   ///< 6.0.1 18716
        SMSG_TRANSFER_ABORTED                       = 0x087E,   ///< 6.0.1 18716
        SMSG_TRANSFER_PENDING                       = 0x1249,   ///< 6.0.1 18716
        SMSG_INVENTORY_CHANGE_FAILURE               = 0x0A59,   ///< 6.0.1 18716

        /// Interaction
        SMSG_LOGOUT_RESPONSE                        = 0x0CD3,   ///< 6.0.1 18716
        SMSG_LOGOUT_CANCEL_ACK                      = 0x02CE,   ///< 6.0.1 18716
        SMSG_LOGOUT_COMPLETE                        = 0x0CAB,   ///< 6.0.1 18716
        SMSG_GOSSIP_POI                             = 0x1F45,   ///< 6.0.1 18716

        /// World Object management
        SMSG_UPDATE_OBJECT                          = 0x15C5,   ///< 6.0.1 18716
        SMSG_POWER_UPDATE                           = 0x0000,   ///<

        /// Character list
        SMSG_ENUM_CHARACTERS_RESULT                 = 0x1561,   ///< 6.0.1 18716
        SMSG_CREATE_CHAR                            = 0x0C2B,   ///< 6.0.1 18716
        SMSG_CHAR_DELETE                            = 0x0447,   ///< 6.0.1 18716

        /// Account data
        SMSG_ACCOUNT_DATA_TIMES                     = 0x137A,   ///< 6.0.1 18716
        SMSG_UPDATE_ACCOUNT_DATA                    = 0x07C6,   ///< 6.0.1 18716
        SMSG_TUTORIAL_FLAGS                         = 0x1546,   ///< 6.0.1 18716

        /// Combat
        SMSG_ATTACKER_STATE_UPDATE                  = 0x0654,   ///< 6.0.1 18716
        SMSG_ATTACK_START                           = 0x1A7D,   ///< 6.0.1 18716
        SMSG_ATTACK_STOP                            = 0x117A,   ///< 6.0.1 18716
        SMSG_AI_REACTION                            = 0x024A,   ///< 6.0.1 18716
        SMSG_ATTACK_SWING_ERROR                     = 0x0008,   ///< 6.0.1 18716

        /// Vendor
        SMSG_LIST_INVENTORY                         = 0x0C65,   ///< 6.0.1 18716
        SMSG_BUY_FAILED                             = 0x0ED4,   ///< 6.0.1 18716
        SMSG_BUY_ITEM                               = 0x1E42,   ///< 6.0.1 18716
        SMSG_SELL_ITEM                              = 0x007E,   ///< 6.0.1 18716

        /// Achievement
        SMSG_ALL_ACHIEVEMENT_DATA                   = 0x17E1,   ///< 6.0.1 18716

        /// Friends
        SMSG_CONTACT_LIST                           = 0x1C9B,   ///< 6.0.1 18716

        /// Taxi
        SMSG_TAXI_NODE_STATUS                       = 0x0049,   ///< 6.0.1 18716
        SMSG_NEW_TAXI_PATH                          = 0x1ED7,   ///< 6.0.1 18716
        SMSG_SHOW_TAXI_NODES                        = 0x0C2C,   ///< 6.0.1 18716
        SMSG_ACTIVATE_TAXI_REPLY                    = 0x004A,   ///< 6.0.1 18716

        /// Loot
        SMSG_LOOT_LIST                              = 0x1ED8,   ///< 6.0.1 18716
        SMSG_LOOT_RESPONSE                          = 0x02EE,   ///< 6.0.1 18716
        SMSG_LOOT_MONEY_NOTIFY                      = 0x1A59,   ///< 6.0.1 18716
        SMSG_LOOT_RELEASE                           = 0x07E6,   ///< 6.0.1 18716
        SMSG_LOOT_RELEASE_ALL                       = 0x0E8B,   ///< 6.0.1 18716
        SMSG_LOOT_REMOVED                           = 0x1C0C,   ///< 6.0.1 18716
        SMSG_COIN_REMOVED                           = 0x00ED,   ///< 6.0.1 18716

        /// Bank
        SMSG_SHOW_BANK                              = 0x1DE2,   ///< 6.0.1 18716

        /// Group
        SMSG_PARTY_UPDATE                           = 0x0CE6,   ///< 6.0.1 18716
        SMSG_PARTY_INVITE                           = 0x1FD8,   ///< 6.0.1 18716
        SMSG_GROUP_DECLINE                          = 0x0D65,   ///< 6.0.1 18716
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Guild
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamGuild
        /// Guild Finder
        SMSG_LFGUILD_BROWSE                         = 0x0000,
        SMSG_LFGUILD_APPLICATIONS                   = 0x0000,
        SMSG_LFGUILD_APPLICATIONS_LIST_CHANGED      = 0x0000,
        SMSG_LFGUILD_COMMAND_RESULT                 = 0x0000,
        SMSG_LFGUILD_RECRUITS                       = 0x0000,
        SMSG_LFGUILD_POST                           = 0x0000,
        SMSG_LFGUILD_APPLICANT_LIST_CHANGED         = 0x0000,

        /// Bank
        SMSG_GUILD_RANK                             = 0x0000,
        SMSG_GUILD_BANK_LOG_QUERY_RESULT            = 0x0000,
        SMSG_GUILD_BANK_REMAINING_WITHDRAW_MONEY    = 0x0000,
        SMSG_GUILD_BANK_QUERY_TEXT_RESULT           = 0x0000,
        SMSG_GUILD_BANK_QUERY_RESULTS               = 0x0000,
        SMSG_GUILD_PERMISSIONS_QUERY_RESULTS        = 0x0000,

        /// Achievement
        SMSG_GUILD_ACHIEVEMENT_DELETED              = 0x0000,
        SMSG_GUILD_ACHIEVEMENT_MEMBERS              = 0x0000,
        SMSG_GUILD_ACHIEVEMENT_EARNED               = 0x0000,
        SMSG_GUILD_CRITERIA_UPDATE                  = 0x0000,
        SMSG_GUILD_CRITERIA_DELETED                 = 0x0000,
        SMSG_ALL_GUILD_ACHIEVEMENTS                 = 0x0000,

        /// Info
        SMSG_GUILD_ROSTER                           = 0x0000,
        SMSG_GUILD_ROSTER_UPDATE                    = 0x0000,
        SMSG_QUERY_GUILD_INFO_RESPONSE              = 0x0000,
        SMSG_GUILD_SEND_MAX_DAILY_XP                = 0x0000,   ///< Debug opcode (disabled)
        SMSG_GUILD_XPEARNED                         = 0x0000,
        SMSG_GUILD_MEMBER_UPDATE_NOTE               = 0x0000,
        SMSG_GUILD_REWARDS_LIST                     = 0x0000,
        SMSG_GUILD_SEND_RANK_CHANGE                 = 0x0000,
        SMSG_GUILD_CHALLENGE_UPDATED                = 0x0000,
        SMSG_GUILD_REPUTATION_WEEKLY_CAP            = 0x0000,
        SMSG_GUILD_MEMBER_RECIPES                   = 0x0000,
        SMSG_GUILD_INVITE_EXPIRED                   = 0x0000,
        SMSG_GUILD_COMMAND_RESULT                   = 0x0000,
        SMSG_GUILD_CHALLENGE_COMPLETED              = 0x0000,
        SMSG_GUILD_SEND_GUILD_XP                    = 0x0000,
        SMSG_GUILD_REPUTATION_REACTION_CHANGED      = 0x0000,
        SMSG_GUILD_KNOWN_RECIPES                    = 0x0000,
        SMSG_GUILD_MEMBER_DAILY_RESET               = 0x0000,
        SMSG_GUILD_FLAGGED_FOR_RENAME               = 0x0000,
        SMSG_GUILD_NAME_CHANGED                     = 0x0000,
        SMSG_GUILD_RESET                            = 0x0000,
        SMSG_GUILD_CHANGE_NAME_RESULT               = 0x0000,
        SMSG_GUILD_INVITE                           = 0x0000,
        SMSG_GUILD_INVITE_DECLINED                  = 0x0000,
        SMSG_GUILD_PARTY_STATE                      = 0x0000,
        SMSG_GUILD_MEMBERS_WITH_RECIPE              = 0x0000,
        SMSG_GUILD_MOVE_STARTING                    = 0x0000,
        SMSG_GUILD_MOVED                            = 0x0000,
        SMSG_GUILD_NEWS                             = 0x0000,
        SMSG_GUILD_NEWS_DELETED                     = 0x0000,

        /// Event system
        SMSG_GUILD_EVENT_NEW_LEADER                 = 0x0000,
        SMSG_GUILD_EVENT_PRESENCE_CHANGE            = 0x0000,
        SMSG_GUILD_EVENT_PLAYER_JOINED              = 0x0000,
        SMSG_GUILD_EVENT_PLAYER_LEFT                = 0x0000,
        SMSG_GUILD_EVENT_RANKS_UPDATED              = 0x0000,
        SMSG_GUILD_EVENT_RANK_CHANGED               = 0x0000,
        SMSG_GUILD_EVENT_LOG_QUERY_RESULTS          = 0x0000,
        SMSG_GUILD_EVENT_MOTD                       = 0x0000,
        SMSG_GUILD_EVENT_DISBANDED                  = 0x0000,
        SMSG_GUILD_EVENT_TAB_MODIFIED               = 0x0000,
        SMSG_GUILD_EVENT_TAB_DELETED                = 0x0000,
        SMSG_GUILD_EVENT_TAB_TEXT_CHANGED           = 0x0000,
        SMSG_GUILD_EVENT_BANK_CONTENTS_CHANGED      = 0x0000,
        SMSG_GUILD_EVENT_BANK_MONEY_CHANGED         = 0x0000,

        /// Misc
        SMSG_GMGUILD_SAVE_RESPONSE                  = 0x0000,   ///< Debug opcode (disabled)
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Chat
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamChat
        /// Voice chat
        SMSG_VOICE_SESSION_ROSTER_UPDATE            = 0x0000,
        SMSG_VOICE_SESSION_LEAVE                    = 0x0000,
        SMSG_VOICE_PARENTAL_CONTROLS                = 0x0000,
        SMSG_VOICE_CHAT_STATUS                      = 0x0000,
        SMSG_AVAILABLE_VOICE_CHANNEL                = 0x0000,

        /// Friend list
        SMSG_USERLIST_ADD                           = 0x0000,
        SMSG_USERLIST_REMOVE                        = 0x0000,
        SMSG_USERLIST_UPDATE                        = 0x0000,

        /// Misc
        SMSG_SERVER_FIRST_ACHIEVEMENT               = 0x0000,
        SMSG_RAID_INSTANCE_MESSAGE                  = 0x0000,
        SMSG_MOTD                                   = 0x1053,   ///< 6.0.1 18716
        SMSG_EXPECTED_SPAM_RECORDS                  = 0x0000,
        SMSG_DEFENSE_MESSAGE                        = 0x0000,   ///<
        SMSG_ZONE_UNDER_ATTACK                      = 0x0855,   ///< 6.0.1 18716
        SMSG_WHO                                    = 0x0000,

        /// Chat
        SMSG_CHAT_SERVER_MESSAGE                    = 0x0000,
        SMSG_CHAT_RESTRICTED                        = 0x0000,
        SMSG_CHAT_RECONNECT                         = 0x0000,
        SMSG_CHAT_PLAYER_NOTFOUND                   = 0x0000,
        SMSG_CHAT_PLAYER_AMBIGUOUS                  = 0x0000,   ///<
        SMSG_CHAT_NOT_IN_PARTY                      = 0x0000,
        SMSG_CHAT_IS_DOWN                           = 0x0000,
        SMSG_CHAT_IGNORED_ACCOUNT_MUTED             = 0x0000,
        SMSG_CHAT_DOWN                              = 0x0000,
        SMSG_CHAT_AUTO_RESPONDED                    = 0x0000,
        SMSG_CHAT                                   = 0x00D2,   ///< 6.0.1 18716

        /// Channel
        SMSG_CHANNEL_NOTIFY_LEFT                    = 0x0073,   ///< 6.0.1 18716
        SMSG_CHANNEL_NOTIFY_JOINED                  = 0x0077,   ///< 6.0.1 18716
        SMSG_CHANNEL_NOTIFY                         = 0x0000,
        SMSG_CHANNEL_LIST                           = 0x0000,   ///<
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Move
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamMove
        /// Move speeds
        SMSG_MOVE_SET_RUN_SPEED                     = 0x000D,   ///< 6.0.1 18716
        SMSG_MOVE_SET_RUN_BACK_SPEED                = 0x0357,   ///< 6.0.1 18716
        SMSG_MOVE_SET_SWIM_SPEED                    = 0x008D,   ///< 6.0.1 18716
        SMSG_MOVE_SET_SWIM_BACK_SPEED               = 0x1352,   ///< 6.0.1 18716
        SMSG_MOVE_SET_FLIGHT_SPEED                  = 0x11E7,   ///< 6.0.1 18716
        SMSG_MOVE_SET_FLIGHT_BACK_SPEED             = 0x1364,   ///< 6.0.1 18716
        SMSG_MOVE_SET_WALK_SPEED                    = 0x13D8,   ///< 6.0.1 18716
        SMSG_MOVE_SET_TURN_RATE                     = 0x0B48,   ///< 6.0.1 18716
        SMSG_MOVE_SET_PITCH_RATE                    = 0x1365,   ///< 6.0.1 18716

        /// Player
        SMSG_MOVE_UPDATE                            = 0x01A5,   ///< 6.0.1 18716
        SMSG_MOVE_TELEPORT                          = 0x0176,   ///< 6.0.1 18716
        SMSG_MOVE_ROOT                              = 0x033E,   ///< 6.0.1 18716
        SMSG_MOVE_UNROOT                            = 0x0B45,   ///< 6.0.1 18716
        SMSG_MOVE_WATER_WALK                        = 0x0026,   ///< 6.0.1 18716
        SMSG_MOVE_LAND_WALK                         = 0x01D6,   ///< 6.0.1 18716
        SMSG_MOVE_FEATHER_FALL                      = 0x03E6,   ///< 6.0.1 18716
        SMSG_MOVE_NORMAL_FALL                       = 0x03D7,   ///< 6.0.1 18716
        SMSG_MOVE_SET_CAN_FLY                       = 0x012E,   ///< 6.0.1 18716
        SMSG_MOVE_UNSET_CAN_FLY                     = 0x0BF5,   ///< 6.0.1 18716
        SMSG_MOVE_SET_ACTIVE_MOVER                  = 0x0AA8,   ///< 6.0.1 18716

        /// Creature
        SMSG_MONSTER_MOVE                           = 0x03F5,   ///< 6.0.1 18716
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Spell
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamSpell
        /// Aura
        SMSG_AURA_POINTS_DEPLETED                   = 0x0000,
        SMSG_AURA_UPDATE                            = 0x08C2,   ///< 6.0.1 18716

        /// Misc
        SMSG_CHEAT_IGNORE_DIMISHING_RETURNS         = 0x0000,
        SMSG_DISPEL_FAILED                          = 0x0000,
        SMSG_INCREASE_CAST_TIME_FOR_SPELL           = 0x0000,
        SMSG_MIRROR_IMAGE_COMPONENTED_DATA          = 0x0000,
        SMSG_MIRROR_IMAGE_CREATURE_DATA             = 0x0000,
        SMSG_PET_CLEAR_SPELLS                       = 0x0000,
        SMSG_PET_SPELLS_MESSAGE                     = 0x0000,
        SMSG_REFRESH_SPELL_HISTORY                  = 0x0000,
        SMSG_RESUME_CAST_BAR                        = 0x0000,

        /// Spell Book / Bar
        SMSG_UPDATE_WEEKLY_SPELL_USAGE              = 0x0000,
        SMSG_WEEKLY_SPELL_USAGE                     = 0x0000,
        SMSG_SEND_KNOWN_SPELLS                      = 0x0A9C,   ///< 6.0.1 18716
        SMSG_SEND_SPELL_CHARGES                     = 0x0000,
        SMSG_SEND_SPELL_HISTORY                     = 0x0000,
        SMSG_SEND_UNLEARN_SPELLS                    = 0x0000,
        SMSG_CLEAR_ALL_SPELL_CHARGES                = 0x0000,
        SMSG_CLEAR_COOLDOWN                         = 0x0000,
        SMSG_CATEGORY_COOLDOWN                      = 0x0000,
        SMSG_CLEAR_SPELL_CHARGES                    = 0x0000,
        SMSG_SET_FLAT_SPELL_MODIFIER                = 0x0000,
        SMSG_SET_PCT_SPELL_MODIFIER                 = 0x0000,
        SMSG_SET_SPELL_CHARGES                      = 0x0000,
        SMSG_LEARNED_SPELL                          = 0x089C,   ///< 6.0.1 18716
        SMSG_UNLEARNED_SPELLS                       = 0x18E2,   ///< 6.0.1 18716

        /// Casting
        SMSG_SPELL_CHANNEL_START                    = 0x0000,
        SMSG_SPELL_CHANNEL_UPDATE                   = 0x0000,
        SMSG_SPELL_COOLDOWN                         = 0x0000,
        SMSG_SPELL_DAMAGE_SHIELD                    = 0x0000,
        SMSG_SPELL_DELAYED                          = 0x0000,
        SMSG_SPELL_GO                               = 0x17D6,   ///< 6.0.1 18716
        SMSG_SPELL_MULTISTRIKE_EFFECT               = 0x0000,
        SMSG_SPELL_OR_DAMAGE_IMMUNE                 = 0x0000,
        SMSG_SPELL_START                            = 0x05D1,   ///< 6.0.1 18716
        SMSG_SPELL_UPDATE_CHAIN_TARGETS             = 0x0000,

        /// Logging
        SMSG_SPELL_DISPELL_LOG                      = 0x0000,
        SMSG_SPELL_ENERGIZE_LOG                     = 0x0000,
        SMSG_SPELL_EXECUTE_LOG                      = 0x0000,
        SMSG_SPELL_NON_MELEE_DAMAGE_LOG             = 0x0E55,   ///< 6.0.1 18716
        SMSG_SPELL_HEAL_LOG                         = 0x0000,
        SMSG_SPELL_INSTAKILL_LOG                    = 0x0000,
        SMSG_SPELL_INTERRUPT_LOG                    = 0x0000,
        SMSG_SPELL_MISS_LOG                         = 0x0000,
        SMSG_ENVIRONMENTAL_DAMAGE_LOG               = 0x0000,
        SMSG_SPELL_PERIODIC_AURA_LOG                = 0x0000,
        SMSG_SPELL_PROC_SCRIPT_LOG                  = 0x0000,   ///< Debug opcode (disabled)
        SMSG_SPELL_PROCS_PER_MINUTE_LOG             = 0x0000,   ///< Debug opcode (disabled)
        SMSG_SPELL_CHANCE_PROC_LOG                  = 0x0000,   ///< Debug opcode (disabled)
        SMSG_RESIST_LOG                             = 0x0000,   ///< Debug opcode (disabled)
        SMSG_AURA_CAST_LOG                          = 0x0000,   ///< Debug opcode (disabled)
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Quest
    //////////////////////////////////////////////////////////////////////////
    #pragma region JamQuest
        /// Quest log
        SMSG_QUERY_QUEST_INFO_RESPONSE              = 0x1A4B,   ///< 6.0.1 18716
        SMSG_QUEST_UPDATE_FAILED_TIMER              = 0x1ADB,   ///< 6.0.1 18716
        SMSG_QUEST_UPDATE_FAILED                    = 0x19DC,   ///< 6.0.1 18716
        SMSG_QUEST_UPDATE_COMPLETE                  = 0x015B,   ///< 6.0.1 18716
        SMSG_QUEST_UPDATE_ADD_PVP_CREDIT            = 0x0A50,   ///< 6.0.1 18716
        SMSG_QUEST_UPDATE_ADD_CREDIT_SIMPLE         = 0x01DF,   ///< 6.0.1 18716
        SMSG_QUEST_UPDATE_ADD_CREDIT                = 0x08CC,   ///< 6.0.1 18716
        SMSG_QUEST_PUSH_RESULT                      = 0x12DF,   ///< 6.0.1 18716
        SMSG_QUEST_POIQUERY_RESPONSE                = 0x18DF,   ///< 6.0.1 18716
        SMSG_QUEST_LOG_FULL                         = 0x1BDF,   ///< 6.0.1 18716
        SMSG_SET_QUEST_COMPLETED_BIT                = 0x0000,
        SMSG_CLEAR_QUEST_COMPLETED_BITS             = 0x0000,
        SMSG_CLEAR_QUEST_COMPLETED_BIT              = 0x0000,
        SMSG_DAILY_QUESTS_RESET                     = 0x0000,
        SMSG_IS_QUEST_COMPLETE_RESPONSE             = 0x12CC,   ///< 6.0.1 18716
        SMSG_QUEST_FORCE_REMOVED                    = 0x00DC,   ///< 6.0.1 18716
        SMSG_QUEST_CONFIRM_ACCEPT                   = 0x084C,   ///< 6.0.1 18716
        SMSG_QUEST_COMPLETION_NPCRESPONSE           = 0x0000,

        /// Quest giver
        SMSG_QUEST_GIVER_STATUS_MULTIPLE            = 0x024C,   ///< 6.0.1 18716
        SMSG_QUEST_GIVER_STATUS                     = 0x1ADF,   ///< 6.0.1 18716
        SMSG_QUEST_GIVER_REQUEST_ITEMS              = 0x0B5C,   ///< 6.0.1 18716
        SMSG_QUEST_GIVER_QUEST_LIST_MESSAGE         = 0x18D0,   ///< 6.0.1 18716
        SMSG_QUEST_GIVER_QUEST_FAILED               = 0x13CF,   ///< 6.0.1 18716
        SMSG_QUEST_GIVER_QUEST_DETAILS              = 0x10DF,   ///< 6.0.1 18716
        SMSG_QUEST_GIVER_QUEST_COMPLETE             = 0x01E0,   ///< 6.0.1 18716
        SMSG_QUEST_GIVER_OFFER_REWARD_MESSAGE       = 0x104C,   ///< 6.0.1 18716
        SMSG_QUEST_GIVER_INVALID_QUEST              = 0x087F,   ///< 6.0.1 18716

        /// Gossip
        SMSG_GOSSIP_MESSAGE                         = 0x034F,   ///< 6.0.1 18716
        SMSG_GOSSIP_COMPLETE                        = 0x1060,   ///< 6.0.1 18716
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Lfg
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    /// Jam Client Garrison
    //////////////////////////////////////////////////////////////////////////

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
    /// User Router
    //////////////////////////////////////////////////////////////////////////
    CMSG_LOG_STREAMING_ERROR                    = 0x0000,   ///<
    CMSG_QUEUED_MESSAGES_END                    = 0x0000,   ///<
    CMSG_LOG_DISCONNECT                         = 0x032B,   ///< 6.0.1 18716
    CMSG_PING                                   = 0x1897,   ///< 6.0.1 18716
    CMSG_AUTH_CONTINUED_SESSION                 = 0x0000,   ///<
    CMSG_SUSPEND_TOKEN_RESPONSE                 = 0x0000,   ///<
    CMSG_AUTH_SESSION                           = 0x0204,   ///< 6.0.1 18716
    CMSG_ENABLE_NAGLE                           = 0x0A2C,   ///< 6.0.1 18716
    CMSG_SUSPEND_COMMS_ACK                      = 0x0000,   ///<

    //////////////////////////////////////////////////////////////////////////
    /// Character
    //////////////////////////////////////////////////////////////////////////
    CMSG_ENUM_CHARACTERS                        = 0x1446,   ///< 6.0.1 18716
    CMSG_CREATE_CHARACTER                       = 0x04D4,   ///< 6.0.1 18716
    CMSG_CHAR_DELETE                            = 0x06E2,   ///< 6.0.1 18716
    CMSG_PLAYER_LOGIN                           = 0x026A,   ///< 6.0.1 18716
    CMSG_VIOLENCE_LEVEL                         = 0x011D,   ///< 6.0.1 18716
    CMSG_LOAD_SCREEN                            = 0x025D,   ///< 6.0.1 18716
    CMSG_SET_DUNGEON_DIFFICULTY                 = 0x1D42,   ///< 6.0.1 18716
    CMSG_AUTO_DECLINE_GUILD_INVITES             = 0x18DA,   ///< 6.0.1 18716
    CMSG_SHOWING_CLOAK                          = 0x1B7A,   ///< 6.0.1 18716
    CMSG_SHOWING_HELM                           = 0x0ACE,   ///< 6.0.1 18716
    CMSG_SET_ACTIVE_MOVER                       = 0x125B,   ///< 6.0.1 18716
    CMSG_LEARN_TALENTS                          = 0x09D7,   ///< 6.0.1 18716
    CMSG_AUTOEQUIP_ITEM                         = 0x143D,   ///< 6.0.1 18716
    CMSG_SWAP_INV_ITEM                          = 0x0620,   ///< 6.0.1 18716
    CMSG_REQUEST_PET_INFO                       = 0x0E3A,   ///< 6.0.1 18716
    
    //////////////////////////////////////////////////////////////////////////
    /// Bank
    //////////////////////////////////////////////////////////////////////////
    CMSG_AUTOBANK_ITEM                          = 0x1440,   ///< 6.0.1 18716
    CMSG_AUTOSTORE_BANK_ITEM                    = 0x14BE,   ///< 6.0.1 18716
    CMSG_BUY_BANK_SLOT                          = 0x0C8B,   ///< 6.0.1 18716
    CMSG_BUY_REAGENT_BANK                       = 0x0747,   ///< 6.0.1 18716
    CMSG_SORT_REAGENT_BANK_BAGS                 = 0x0CB7,   ///< 6.0.1 18716

    //////////////////////////////////////////////////////////////////////////
    /// Loot
    //////////////////////////////////////////////////////////////////////////
    CMSG_LOOT                                   = 0x01B5,   ///< 6.0.1 18716
    CMSG_LOOT_MONEY                             = 0x1362,   ///< 6.0.1 18716
    CMSG_LOOT_ITEM                              = 0x0353,   ///< 6.0.1 18716
    CMSG_LOOT_RELEASE                           = 0x0B71,   ///< 6.0.1 18716

    //////////////////////////////////////////////////////////////////////////
    /// Combat
    //////////////////////////////////////////////////////////////////////////
    CMSG_ATTACKSTOP                             = 0x1347,   ///< 6.0.1 18716
    CMSG_ATTACKSWING                            = 0x020E,   ///< 6.0.1 18716

    //////////////////////////////////////////////////////////////////////////
    /// Spell
    //////////////////////////////////////////////////////////////////////////
    CMSG_CAST_SPELL                             = 0x0968,   ///< 6.0.1 18716
    CMSG_CANCEL_CAST                            = 0x1BF8,   ///< 6.0.1 18716
    CMSG_USE_ITEM                               = 0x0032,   ///< 6.0.1 18716

    //////////////////////////////////////////////////////////////////////////
    /// Cache
    //////////////////////////////////////////////////////////////////////////
    CMSG_REQUEST_HOTFIX                         = 0x006A,   ///< 6.0.1 18716
    CMSG_CREATURE_QUERY                         = 0x0367,   ///< 6.0.1 18716
    CMSG_NPC_TEXT_QUERY                         = 0x1B53,   ///< 6.0.1 18716
    CMSG_NAME_QUERY                             = 0x1E42,   ///< 6.0.1 18716
    CMSG_QUEST_QUERY                            = 0x0096,   ///< 6.0.1 18716
    CMSG_QUEST_POI_QUERY                        = 0x0E41,   ///< 6.0.1 18716
    CMSG_REALM_NAME_QUERY                       = 0x0CC1,   ///< 6.0.1 18716
    CMSG_GAMEOBJECT_QUERY                       = 0x0942,   ///< 6.0.1 18716

    //////////////////////////////////////////////////////////////////////////
    /// Interaction
    //////////////////////////////////////////////////////////////////////////
    CMSG_LOGOUT_REQUEST                         = 0x1294,   ///< 6.0.1 18716
    CMSG_LOGOUT_CANCEL                          = 0x08EA,   ///< 6.0.1 18716
    CMSG_SET_ACTION_BUTTON                      = 0x1646,   ///< 6.0.1 18716
    CMSG_SET_ACTIONBAR_TOGGLES                  = 0x0AFD,   ///< 6.0.1 18716
    CMSG_SET_SELECTION                          = 0x18EA,   ///< 6.0.1 18716
    CMSG_SETSHEATHED                            = 0x137E,   ///< 6.0.1 18716
    CMSG_GOSSIP_HELLO                           = 0x1194,   ///< 6.0.1 18716
    CMSG_GOSSIP_SELECT_OPTION                   = 0x12DA,   ///< 6.0.1 18716
    CMSG_TOGGLE_PVP                             = 0x1495,   ///< 6.0.1 18716
    CMSG_TUTORIAL                               = 0x0DE1,   ///< 6.0.1 18716
    CMSG_SET_SPECIALIZATION                     = 0x0482,   ///< 6.0.1 18716
    CMSG_SET_WATCHED_FACTION                    = 0x08EE,   ///< 6.0.1 18716
    CMSG_SET_FACTION_INACTIVE                   = 0x0AE9,   ///< 6.0.1 18716
    CMSG_AREATRIGGER                            = 0x02B3,   ///< 6.0.1 18716
    CMSG_GAMEOBJECT_REPORT_USE                  = 0x0369,   ///< 6.0.1 18716
    CMSG_GAMEOBJECT_USE                         = 0x02CE,   ///< 6.0.1 18716

    //////////////////////////////////////////////////////////////////////////
    /// Vendors
    //////////////////////////////////////////////////////////////////////////
    CMSG_LIST_INVENTORY                         = 0x02DE,   ///< 6.0.1 18716
    CMSG_REPAIR_ITEM                            = 0x0E39,   ///< 6.0.1 18716
    CMSG_BUYBACK_ITEM                           = 0x10DD,   ///< 6.0.1 18716
    CMSG_BUY_ITEM                               = 0x18DD,   ///< 6.0.1 18716
    CMSG_SELL_ITEM                              = 0x017D,   ///< 6.0.1 18716

    //////////////////////////////////////////////////////////////////////////
    /// Taxi
    //////////////////////////////////////////////////////////////////////////
    CMSG_SET_TAXI_BENCHMARK_MODE                = 0x0C9C,   ///< 6.0.1 18716
    CMSG_ENABLE_TAXI_NODE                       = 0x0ACD,   ///< 6.0.1 18716
    CMSG_ACTIVATE_TAXI                          = 0x0CB9,   ///< 6.0.1 18716
    CMSG_ACTIVATE_TAXI_EXPRESS                  = 0x02FA,   ///< 6.0.1 18716

    //////////////////////////////////////////////////////////////////////////
    /// Quest
    //////////////////////////////////////////////////////////////////////////
    CMSG_QUESTGIVER_HELLO                       = 0x1AE9,   ///< 6.0.1 18716
    CMSG_QUESTGIVER_STATUS_QUERY                = 0x1C8C,   ///< 6.0.1 18716
    CMSG_QUESTGIVER_STATUS_MULTIPLE_QUERY       = 0x0E8C,   ///< 6.0.1 18716
    CMSG_QUESTGIVER_QUERY_QUEST                 = 0x1B6D,   ///< 6.0.1 18716
    CMSG_QUESTGIVER_COMPLETE_QUEST              = 0x197D,   ///< 6.0.1 18716
    CMSG_QUESTGIVER_CHOOSE_REWARD               = 0x10DE,   ///< 6.0.1 18716
    CMSG_QUESTGIVER_ACCEPT_QUEST                = 0x00A8,   ///< 6.0.1 18716
    CMSG_QUESTGIVER_REQUEST_REWARD              = 0x1C9B,   ///< 6.0.1 18716
    CMSG_QUEST_CONFIRM_ACCEPT                   = 0x0B69,   ///< 6.0.1 18716
    CMSG_QUESTLOG_REMOVE_QUEST                  = 0x0E48,   ///< 6.0.1 18716
    CMSG_PUSHQUESTTOPARTY                       = 0x1297,   ///< 6.0.1 18716

    //////////////////////////////////////////////////////////////////////////
    /// Account data
    //////////////////////////////////////////////////////////////////////////
    CMSG_READY_FOR_ACCOUNT_DATA_TIMES           = 0x0000,   ///<
    CMSG_GET_UNDELETE_CHARACTER_COOLDOWN_STATUS = 0x0000,   ///<

    //////////////////////////////////////////////////////////////////////////
    /// Chat
    //////////////////////////////////////////////////////////////////////////
    /// Addon chat
    CMSG_CHAT_ADDON_MESSAGE_BATTLEGROUND        = 0x000D,   ///<
    CMSG_CHAT_ADDON_MESSAGE_WHISPER             = 0x000E,   ///<
    CMSG_CHAT_ADDON_MESSAGE_GUILD               = 0x000F,   ///<
    CMSG_CHAT_ADDON_MESSAGE_OFFICER             = 0x0010,   ///<
    CMSG_CHAT_ADDON_MESSAGE_RAID                = 0x0011,   ///<
    CMSG_CHAT_ADDON_MESSAGE_PARTY               = 0x0012,   ///<

    /// Chat
    CMSG_CHAT_MESSAGE_RAID_WARNING              = 0x0001,   ///<
    CMSG_CHAT_MESSAGE_PARTY                     = 0x0002,   ///<
    CMSG_CHAT_MESSAGE_YELL                      = 0x17D6,   ///< 6.0.1 18716
    CMSG_CHAT_MESSAGE_SAY                       = 0x16D2,   ///< 6.0.1 18716
    CMSG_CHAT_MESSAGE_OFFICER                   = 0x18C5,   ///< 6.0.1 18716
    CMSG_CHAT_MESSAGE_EMOTE                     = 0x05F6,   ///< 6.0.1 18716
    CMSG_CHAT_MESSAGE_AFK                       = 0x09A3,   ///< 6.0.1 18716
    CMSG_CHAT_MESSAGE_DND                       = 0x18E4,   ///< 6.0.1 18716
    CMSG_CHAT_MESSAGE_GUILD                     = 0x0FD2,   ///< 6.0.1 18716
    CMSG_CHAT_MESSAGE_RAID                      = 0x000A,   ///<
    CMSG_CHAT_MESSAGE_WHISPER                   = 0x098B,   ///< 6.0.1 18716
    CMSG_CHAT_MESSAGE_CHANNEL                   = 0x0FF1,   ///< 6.0.1 18716

    CMSG_JOIN_CHANNEL                           = 0x0066,   ///< 6.0.1 18716

    //////////////////////////////////////////////////////////////////////////
    /// Movement
    //////////////////////////////////////////////////////////////////////////
    /// Start
    CMSG_MOVE_START_FORWARD                     = 0x035C,   ///< 6.0.1 18716
    CMSG_MOVE_START_TURN_LEFT                   = 0x00DB,   ///< 6.0.1 18716
    CMSG_MOVE_START_TURN_RIGHT                  = 0x005F,   ///< 6.0.1 18716
    CMSG_MOVE_START_BACKWARD                    = 0x084B,   ///< 6.0.1 18716
    CMSG_MOVE_START_STRAFE_LEFT                 = 0x08CC,   ///< 6.0.1 18716
    CMSG_MOVE_START_STRAFE_RIGHT                = 0x03DC,   ///< 6.0.1 18716

    /// Misc
    CMSG_MOVE_JUMP                              = 0x03D0,   ///< 6.0.1 18716
    CMSG_WORLD_PORT_RESPONSE                    = 0x0E61,   ///< 6.0.1 18716

    /// Update
    CMSG_MOVE_HEARTBEAT                         = 0x12CB,   ///< 6.0.1 18716
    CMSG_MOVE_SET_FACING                        = 0x135F,   ///< 6.0.1 18716
    CMSG_MOVE_TELEPORT_ACK                      = 0x035F,   ///< 6.0.1 18716

    /// Stop
    CMSG_MOVE_STOP                              = 0x034B,   ///< 6.0.1 18716
    CMSG_MOVE_STOP_TURN                         = 0x0BCF,   ///< 6.0.1 18716
    CMSG_MOVE_STOP_STRAFE                       = 0x09CF,   ///< 6.0.1 18716

    /// ACK
    CMSG_MOVE_SET_SWIM_SPEED_CHEAT              = 0x104F,   ///< 6.0.1 18716
    CMSG_MOVE_SET_FLIGHT_SPEED_CHEAT            = 0x125C,   ///< 6.0.1 18716
    CMSG_MOVE_SET_RUN_BACK_SPEED_CHEAT          = 0x13CF,   ///< 6.0.1 18716
    CMSG_MOVE_SET_RUN_SPEED_CHEAT               = 0x02CF,   ///< 6.0.1 18716

    //////////////////////////////////////////////////////////////////////////
    /// Group
    //////////////////////////////////////////////////////////////////////////
    CMSG_PARTY_INVITE                           = 0x004A,   ///< 6.0.1 18716
    CMSG_PARTY_INVITE_RESPONSE                  = 0x0FC6,   ///< 6.0.1 18716


    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    // MSG
    MSG_INSPECT_ARENA_TEAMS = 0x0000,
    MSG_AUCTION_HELLO = 0x0000,
    MSG_BATTLEGROUND_PLAYER_POSITIONS = 0x0000,
    MSG_GM_BIND_OTHER = 0x0000,
    MSG_GM_SHOWLABEL = 0x0000,
    MSG_GM_SUMMON = 0x0000,
    MSG_MOVE_CHARM_TELEPORT_CHEAT = 0x0000,
    MSG_MOVE_FALL_LAND = 0x0000,
    MSG_MOVE_GRAVITY_CHNG = 0x0000,
    MSG_MOVE_HOVER = 0x0000,
    MSG_MOVE_JUMP = 0x0000,
    MSG_MOVE_SET_ALL_SPEED_CHEAT = 0x0000,
    MSG_MOVE_SET_COLLISION_HEIGHT = 0x0000,
    MSG_MOVE_SET_FACING = 0x0000,
    MSG_MOVE_SET_PITCH = 0x0000,
    MSG_MOVE_SET_RAW_POSITION_ACK = 0x0000,
    MSG_MOVE_SET_RUN_MODE = 0x0000,
    MSG_MOVE_SET_SWIM_BACK_SPEED_CHEAT = 0x0000,
    MSG_MOVE_SET_TURN_RATE_CHEAT = 0x0000,
    MSG_MOVE_SET_WALK_MODE = 0x0000,
    MSG_MOVE_SET_WALK_SPEED_CHEAT = 0x0000,
    MSG_MOVE_START_ASCEND = 0x0000,
    MSG_MOVE_START_DESCEND = 0x0000,
    MSG_MOVE_TELEPORT_CHEAT = 0x0000,
    MSG_MOVE_TIME_SKIPPED = 0x0000,
    MSG_MOVE_TOGGLE_COLLISION_CHEAT = 0x0000,
    MSG_MOVE_TOGGLE_FALL_LOGGING = 0x0000,
    MSG_MOVE_TOGGLE_LOGGING = 0x0000,
    MSG_MOVE_UPDATE_CAN_FLY = 0x0000,
    MSG_MOVE_UPDATE_FLIGHT_SPEED = 0x0000,
    MSG_MOVE_UPDATE_RUN_SPEED = 0x0000,
    MSG_MOVE_UPDATE_TELEPORT = 0x0000,
    MSG_MOVE_WATER_WALK = 0x0000,
    MSG_NOTIFY_PARTY_SQUELCH = 0x0000,
    MSG_QUERY_NEXT_MAIL_TIME = 0x0000,
    MSG_SET_RAID_DIFFICULTY = 0x0000,


    // CMSG
    CMSG_ACCEPT_LEVEL_GRANT                           = 0x0000,
    CMSG_ACCEPT_TRADE                                 = 0x0000,
    CMSG_ADDON_REGISTERED_PREFIXES                    = 0x0000,
    CMSG_ADD_FRIEND                                   = 0x0000,
    CMSG_ADD_IGNORE                                   = 0x0000,
    CMSG_ADD_VOICE_IGNORE                             = 0x0000,
    CMSG_ALTER_APPEARANCE                             = 0x0000,
    CMSG_AREA_SPIRIT_HEALER_QUERY                     = 0x0000,
    CMSG_AREA_SPIRIT_HEALER_QUEUE                     = 0x0000,
    CMSG_ARENA_TEAM_ACCEPT                            = 0x0000,
    CMSG_ARENA_TEAM_CREATE                            = 0x0000,
    CMSG_ARENA_TEAM_DECLINE                           = 0x0000,
    CMSG_ARENA_TEAM_DISBAND                           = 0x0000,
    CMSG_ARENA_TEAM_INVITE                            = 0x0000,
    CMSG_ARENA_TEAM_LEADER                            = 0x0000,
    CMSG_ARENA_TEAM_LEAVE                             = 0x0000,
    CMSG_ARENA_TEAM_QUERY                             = 0x0000,
    CMSG_ARENA_TEAM_REMOVE                            = 0x0000,
    CMSG_ARENA_TEAM_ROSTER                            = 0x0000,
    CMSG_AUCTION_HELLO                                = 0x0000,
    CMSG_AUCTION_LIST_BIDDER_ITEMS                    = 0x0000,
    CMSG_AUCTION_LIST_ITEMS                           = 0x0000,
    CMSG_AUCTION_LIST_OWNER_ITEMS                     = 0x0000,
    CMSG_AUCTION_LIST_PENDING_SALES                   = 0x0000,
    CMSG_AUCTION_PLACE_BID                            = 0x0000,
    CMSG_AUCTION_REMOVE_ITEM                          = 0x0000,
    CMSG_AUCTION_SELL_ITEM                            = 0x0000,
    CMSG_AUTOEQUIP_GROUND_ITEM                        = 0x0000,
    CMSG_AUTOEQUIP_ITEM_SLOT                          = 0x0000,
    CMSG_AUTOSTORE_BAG_ITEM                           = 0x0000,
    CMSG_AUTOSTORE_GROUND_ITEM                        = 0x0000,
    CMSG_BANKER_ACTIVATE                              = 0x0000,
    CMSG_BATTLEFIELD_JOIN                             = 0x0000,
    CMSG_BATTLEFIELD_LEAVE                            = 0x0000,
    CMSG_BATTLEFIELD_LIST                             = 0x0000,
    CMSG_BATTLEFIELD_MGR_ENTRY_INVITE_RESPONSE        = 0x0000,
    CMSG_BATTLEFIELD_MGR_EXIT_REQUEST                 = 0x0000,
    CMSG_BATTLEFIELD_MGR_QUEUE_INVITE_RESPONSE        = 0x0000,
    CMSG_BATTLEFIELD_MGR_QUEUE_REQUEST                = 0x0000,
    CMSG_BATTLEFIELD_PORT                             = 0x0000,
    CMSG_BATTLEFIELD_REQUEST_SCORE_DATA               = 0x0000,
    CMSG_BATTLEFIELD_STATUS                           = 0x0000,
    CMSG_BATTLEGROUND_PLAYER_POSITIONS                = 0x0000,
    CMSG_BATTLEMASTER_JOIN                            = 0x0000,
    CMSG_BATTLEMASTER_JOIN_ARENA                      = 0x0000,
    CMSG_BATTLEMASTER_JOIN_RATED                      = 0x0000,
    CMSG_BEGIN_TRADE                                  = 0x0000,
    CMSG_BINDER_ACTIVATE                              = 0x0000,
    CMSG_BLACK_MARKET_HELLO                           = 0x0000,
    CMSG_BLACK_MARKET_REQUEST_ITEMS                   = 0x0000,
    CMSG_BLACK_MARKET_PLACE_BID                       = 0x0000,
    CMSG_BOT_DETECTED2                                = 0x0000,
    CMSG_CALENDAR_ADD_EVENT                           = 0x0000,
    CMSG_CALENDAR_ARENA_TEAM                          = 0x0000,
    CMSG_CALENDAR_COMPLAIN                            = 0x0000,
    CMSG_CALENDAR_CONTEXT_EVENT_SIGNUP                = 0x0000,
    CMSG_CALENDAR_COPY_EVENT                          = 0x0000,
    CMSG_CALENDAR_EVENT_INVITE                        = 0x0000,
    CMSG_CALENDAR_EVENT_MODERATOR_STATUS              = 0x0000,
    CMSG_CALENDAR_EVENT_REMOVE_INVITE                 = 0x0000,
    CMSG_CALENDAR_EVENT_RSVP                          = 0x0000,
    CMSG_CALENDAR_EVENT_SIGNUP                        = 0x0000,
    CMSG_CALENDAR_EVENT_STATUS                        = 0x0000,
    CMSG_CALENDAR_GET_CALENDAR                        = 0x0000,
    CMSG_CALENDAR_GET_EVENT                           = 0x0000,
    CMSG_CALENDAR_GET_NUM_PENDING                     = 0x0000,
    CMSG_CALENDAR_GUILD_FILTER                        = 0x0000,
    CMSG_CALENDAR_REMOVE_EVENT                        = 0x0000,
    CMSG_CALENDAR_UPDATE_EVENT                        = 0x0000,
    CMSG_CANCEL_AURA                                  = 0x0000,
    CMSG_CANCEL_AUTO_REPEAT_SPELL                     = 0x0000,
    CMSG_CANCEL_CHANNELLING                           = 0x0000,
    CMSG_CANCEL_GROWTH_AURA                           = 0x0000,
    CMSG_CANCEL_MOUNT_AURA                            = 0x0000,
    CMSG_CANCEL_QUEUED_SPELL                          = 0x0000,
    CMSG_CANCEL_TEMP_ENCHANTMENT                      = 0x0000,
    CMSG_CANCEL_TRADE                                 = 0x0000,
    CMSG_CHANGEPLAYER_DIFFICULTY                      = 0x0000,
    CMSG_CHANGE_SEATS_ON_CONTROLLED_VEHICLE           = 0x0000,
    CMSG_CHANNEL_ANNOUNCEMENTS                        = 0x0000,
    CMSG_CHANNEL_BAN                                  = 0x0000,
    CMSG_CHANNEL_DISPLAY_LIST                         = 0x0000,
    CMSG_CHANNEL_INVITE                               = 0x0000,
    CMSG_CHANNEL_KICK                                 = 0x0000,
    CMSG_CHANNEL_LIST                                 = 0x0000,
    CMSG_CHANNEL_MODERATOR                            = 0x0000,
    CMSG_CHANNEL_MUTE                                 = 0x0000,
    CMSG_CHANNEL_OWNER                                = 0x0000,
    CMSG_CHANNEL_PASSWORD                             = 0x0000,
    CMSG_CHANNEL_SET_OWNER                            = 0x0000,
    CMSG_CHANNEL_SILENCE_ALL                          = 0x0000,
    CMSG_CHANNEL_SILENCE_VOICE                        = 0x0000,
    CMSG_CHANNEL_UNBAN                                = 0x0000,
    CMSG_CHANNEL_UNMODERATOR                          = 0x0000,
    CMSG_CHANNEL_UNMUTE                               = 0x0000,
    CMSG_CHANNEL_UNSILENCE_ALL                        = 0x0000,
    CMSG_CHANNEL_UNSILENCE_VOICE                      = 0x0000,
    CMSG_CHANNEL_VOICE_OFF                            = 0x0000,
    CMSG_CHANNEL_VOICE_ON                             = 0x0000,
    CMSG_CHAR_CUSTOMIZE                               = 0x0000,
    CMSG_CHAR_FACTION_OR_RACE_CHANGE                  = 0x0000,
    CMSG_CHAR_RENAME                                  = 0x0000,
    CMSG_CHAT_FILTERED                                = 0x0000,
    CMSG_CHAT_IGNORED                                 = 0x0000,
    CMSG_CLEAR_CHANNEL_WATCH                          = 0x0000,
    CMSG_CLEAR_RAID_MARKER                            = 0x0000,
    CMSG_CLEAR_TRADE_ITEM                             = 0x0000,
    CMSG_COMMENTATOR_ENABLE                           = 0x0000,
    CMSG_COMMENTATOR_ENTER_INSTANCE                   = 0x0000,
    CMSG_COMMENTATOR_EXIT_INSTANCE                    = 0x0000,
    CMSG_COMMENTATOR_GET_MAP_INFO                     = 0x0000,
    CMSG_COMMENTATOR_GET_PARTY_INFO                   = 0x0000,
    CMSG_COMMENTATOR_GET_PLAYER_INFO                  = 0x0000,
    CMSG_COMMENTATOR_INSTANCE_COMMAND                 = 0x0000,
    CMSG_COMMENTATOR_SKIRMISH_QUEUE_COMMAND           = 0x0000,
    CMSG_COMMENTATOR_START_WARGAME                    = 0x0000,
    CMSG_COMPLAIN                                     = 0x0000,
    CMSG_COMPLETE_CINEMATIC                           = 0x0000,
    CMSG_COMPLETE_MOVIE                               = 0x0000,
    CMSG_CONFIRM_RESPEC_WIPE                          = 0x0000,
    CMSG_CONNECT_TO_FAILED                            = 0x0000,
    CMSG_CONTACT_LIST                                 = 0x0000,
    CMSG_CORPSE_MAP_POSITION_QUERY                    = 0x0000,
    CMSG_DANCE_QUERY                                  = 0x0000,
    CMSG_DECLINE_CHANNEL_INVITE                       = 0x0000,
    CMSG_DEL_FRIEND                                   = 0x0000,
    CMSG_DEL_IGNORE                                   = 0x0000,
    CMSG_DEL_VOICE_IGNORE                             = 0x0000,
    CMSG_DESTROY_ITEM                                 = 0x0000,
    CMSG_DISMISS_CONTROLLED_VEHICLE                   = 0x0000,
    CMSG_DISMISS_CRITTER                              = 0x0000,
    CMSG_DUEL_RESPONSE                                = 0x0000,
    CMSG_DUEL_PROPOSED                                = 0x0000,
    CMSG_LFD_LOCK_INFO_REQUEST                        = 0x0000,
    CMSG_EJECT_PASSENGER                              = 0x0000,
    CMSG_EMOTE                                        = 0x0000,
    CMSG_EQUIPMENT_SET_SAVE                           = 0x0000,
    CMSG_EQUIPMENT_SET_USE                            = 0x0000,
    CMSG_EQUIPMENT_SET_DELETE                         = 0x0000,
    CMSG_FAR_SIGHT                                    = 0x0000,
    CMSG_FORCE_MOVE_ROOT_ACK                          = 0x0000,
    CMSG_FORCE_MOVE_UNROOT_ACK                        = 0x0000,
    CMSG_GAMESPEED_SET                                = 0x0000,
    CMSG_GAMETIME_SET                                 = 0x0000,
    CMSG_GETDEATHBINDZONE                             = 0x0000,
    CMSG_GET_MAIL_LIST                                = 0x0000,
    CMSG_GET_MIRRORIMAGE_DATA                         = 0x0000,
    CMSG_GHOST                                        = 0x0000,
    CMSG_GMRESPONSE_RESOLVE                           = 0x0000,
    CMSG_GMSURVEY_SUBMIT                              = 0x0000,
    CMSG_GMTICKET_CREATE                              = 0x0000,
    CMSG_GMTICKET_DELETETICKET                        = 0x0000,
    CMSG_GMTICKET_GETTICKET                           = 0x0000,
    CMSG_GMTICKET_GETWEBTICKET                        = 0x0000,
    CMSG_GMTICKET_SYSTEMSTATUS                        = 0x0000,
    CMSG_GMTICKET_UPDATETEXT                          = 0x0000,
    CMSG_GM_INVIS                                     = 0x0000,
    CMSG_GM_NUKE                                      = 0x0000,
    CMSG_GM_REPORT_LAG                                = 0x0000,
    CMSG_GM_SET_SECURITY_GROUP                        = 0x0000,
    CMSG_GRANT_LEVEL                                  = 0x0000,
    CMSG_GROUP_ASSISTANT_LEADER                       = 0x0000,
    CMSG_GROUP_ASSIGNMENT                             = 0x0000,
    CMSG_GROUP_CANCEL                                 = 0x0000,
    CMSG_GROUP_CHANGE_SUB_GROUP                       = 0x0000,
    CMSG_GROUP_DISBAND                                = 0x0000,
    CMSG_GROUP_RAID_CONVERT                           = 0x0000,
    CMSG_GROUP_REQUEST_JOIN_UPDATES                   = 0x0000,
    CMSG_GROUP_SET_LEADER                             = 0x0000,
    CMSG_GROUP_SET_ROLES                              = 0x0000,
    CMSG_GROUP_SWAP_SUB_GROUP                         = 0x0000,
    CMSG_GROUP_UNINVITE                               = 0x0000,
    CMSG_GROUP_UNINVITE_GUID                          = 0x0000,
    CMSG_GUILD_ACCEPT                                 = 0x0000,
    CMSG_GUILD_ACHIEVEMENT_MEMBERS                    = 0x0000,
    CMSG_GUILD_ACHIEVEMENT_PROGRESS_QUERY             = 0x0000,
    CMSG_GUILD_ADD_RANK                               = 0x0000,
    CMSG_GUILD_ASSIGN_MEMBER_RANK                     = 0x0000,
    CMSG_GUILD_BANKER_ACTIVATE                        = 0x0000,
    CMSG_GUILD_BANK_BUY_TAB                           = 0x0000,
    CMSG_GUILD_BANK_DEPOSIT_MONEY                     = 0x0000,
    CMSG_GUILD_BANK_LOG_QUERY                         = 0x0000,
    CMSG_GUILD_BANK_MONEY_WITHDRAWN_QUERY             = 0x0000,
    CMSG_GUILD_BANK_NOTE                              = 0x0000,
    CMSG_GUILD_BANK_QUERY_TAB                         = 0x0000,
    CMSG_GUILD_BANK_QUERY_TEXT                        = 0x0000,
    CMSG_GUILD_BANK_SWAP_ITEMS                        = 0x0000,
    CMSG_GUILD_BANK_UPDATE_TAB                        = 0x0000,
    CMSG_GUILD_BANK_WITHDRAW_MONEY                    = 0x0000,
    CMSG_GUILD_CHANGE_NAME_REQUEST                    = 0x0000,
    CMSG_GUILD_DECLINE                                = 0x0000,
    CMSG_GUILD_DEL_RANK                               = 0x0000,
    CMSG_GUILD_DISBAND                                = 0x0000,
    CMSG_GUILD_EVENT_LOG_QUERY                        = 0x0000,
    CMSG_GUILD_INFO                                   = 0x0000,
    CMSG_GUILD_INFO_TEXT                              = 0x0000,
    CMSG_GUILD_INVITE                                 = 0x0000,
    CMSG_GUILD_LEADER                                 = 0x0000,
    CMSG_GUILD_LEAVE                                  = 0x0000,
    CMSG_GUILD_MEMBER_SEND_SOR_REQUEST                = 0x0000,
    CMSG_GUILD_MOTD                                   = 0x0000,
    CMSG_GUILD_NEWS_UPDATE_STICKY                     = 0x0000,
    CMSG_GUILD_PERMISSIONS                            = 0x0000,
    CMSG_GUILD_QUERY                                  = 0x0000,
    CMSG_GUILD_QUERY_NEWS                             = 0x0000,
    CMSG_GUILD_QUERY_RANKS                            = 0x0000,
    CMSG_GUILD_REMOVE                                 = 0x0000,
    CMSG_GUILD_REPLACE_GUILD_MASTER                   = 0x0000,
    CMSG_GUILD_REQUEST_CHALLENGE_UPDATE               = 0x0000,
    CMSG_GUILD_REQUEST_MAX_DAILY_XP                   = 0x0000,
    CMSG_GUILD_REQUEST_PARTY_STATE                    = 0x0000,
    CMSG_GUILD_ROSTER                                 = 0x0000,
    CMSG_GUILD_SET_ACHIEVEMENT_TRACKING               = 0x0000,
    CMSG_GUILD_SET_GUILD_MASTER                       = 0x0000,
    CMSG_GUILD_SET_NOTE                               = 0x0000,
    CMSG_GUILD_SET_RANK_PERMISSIONS                   = 0x0000,
    CMSG_GUILD_SWITCH_RANK                            = 0x0000,
    CMSG_HEARTH_AND_RESURRECT                         = 0x0000,
    CMSG_IGNORE_TRADE                                 = 0x0000,
    CMSG_INITIATE_TRADE                               = 0x0000,
    CMSG_INSPECT                                      = 0x0000,
    CMSG_INSPECT_HONOR_STATS                          = 0x0000,
    CMSG_INSTANCE_LOCK_WARNING_RESPONSE               = 0x0000,
    CMSG_ITEM_REFUND                                  = 0x0000,
    CMSG_ITEM_REFUND_INFO                             = 0x0000,
    CMSG_ITEM_TEXT_QUERY                              = 0x0000,
    CMSG_KEEP_ALIVE                                   = 0x0000,
    CMSG_LEARN_PET_SPECIALIZATION_GROUP               = 0x0000,
    CMSG_LEAVE_BATTLEFIELD                            = 0x0000,
    CMSG_LEAVE_CHANNEL                                = 0x0000,
    CMSG_LFG_GET_PLAYER_INFO                          = 0x0000,
    CMSG_LFG_GET_STATUS                               = 0x0000,
    CMSG_LFG_JOIN                                     = 0x0000,
    CMSG_LFG_LEAVE                                    = 0x0000,
    CMSG_LFG_PARTY_LOCK_INFO_REQUEST                  = 0x0000,
    CMSG_LFG_PROPOSAL_RESULT                          = 0x0000,
    CMSG_LFG_SET_BOOT_VOTE                            = 0x0000,
    CMSG_LFG_SET_COMMENT                              = 0x0000,
    CMSG_LFG_SET_ROLES                                = 0x0000,
    CMSG_LFG_TELEPORT                                 = 0x0000,
    CMSG_LF_GUILD_ADD_RECRUIT                         = 0x0000,
    CMSG_LF_GUILD_BROWSE                              = 0x0000,
    CMSG_LF_GUILD_DECLINE_RECRUIT                     = 0x0000,
    CMSG_LF_GUILD_GET_APPLICATIONS                    = 0x0000,
    CMSG_LF_GUILD_GET_RECRUITS                        = 0x0000,
    CMSG_LF_GUILD_POST_REQUEST                        = 0x0000,
    CMSG_LF_GUILD_REMOVE_RECRUIT                      = 0x0000,
    CMSG_LF_GUILD_SET_GUILD_POST                      = 0x0000,
    CMSG_LIST_STABLE_PETS                             = 0x0000,
    CMSG_LOOT_CURRENCY                                = 0x0000,
    CMSG_LOOT_MASTER_GIVE                             = 0x0000,
    CMSG_LOOT_METHOD                                  = 0x0000,
    CMSG_LOOT_ROLL                                    = 0x0000,
    CMSG_LOOT_MASTER_ASK_FOR_ROLL                     = 0x0000,
    CMSG_MAIL_CREATE_TEXT_ITEM                        = 0x0000,
    CMSG_MAIL_DELETE                                  = 0x0000,
    CMSG_MAIL_MARK_AS_READ                            = 0x0000,
    CMSG_MAIL_RETURN_TO_SENDER                        = 0x0000,
    CMSG_MAIL_TAKE_ITEM                               = 0x0000,
    CMSG_MAIL_TAKE_MONEY                              = 0x0000,
    CMSG_MANEUVER_START                               = 0x0000,
    CMSG_MEETINGSTONE_INFO                            = 0x0000,
    CMSG_MINIGAME_MOVE                                = 0x0000,
    CMSG_MINIMAP_PING                                 = 0x0000,
    CMSG_MOUNTSPECIAL_ANIM                            = 0x0000,
    CMSG_MOVE_CHARM_TELEPORT_CHEAT                    = 0x0000,
    CMSG_MOVE_CHNG_TRANSPORT                          = 0x0000,
    CMSG_MOVE_ENABLE_SWIM_TO_FLY_TRANS_ACK            = 0x0000,
    CMSG_MOVE_NOT_ACTIVE_MOVER                        = 0x0000,
    CMSG_MOVE_SET_RELATIVE_POSITION                   = 0x0000,
    CMSG_MOVE_SET_VEHICLE_REC_ID_ACK                  = 0x0000,
    CMSG_MOVE_SPLINE_DONE                             = 0x0000,
    CMSG_MOVE_TIME_SKIPPED                            = 0x0000,
    CMSG_MOVE_TOGGLE_COLLISION_ACK                    = 0x0000,
    CMSG_NEW_SPELL_SLOT                               = 0x0000,
    CMSG_NEXT_CINEMATIC_CAMERA                        = 0x0000,
    CMSG_OBJECT_UPDATE_FAILED                         = 0x0000,
    CMSG_OBJECT_UPDATE_RESCUED                        = 0x0000,
    CMSG_OFFER_PETITION                               = 0x0000,
    CMSG_OPENING_CINEMATIC                            = 0x0000,
    CMSG_OPEN_ITEM                                    = 0x0000,
    CMSG_OPT_OUT_OF_LOOT                              = 0x0000,
    CMSG_PAGE_TEXT_QUERY                              = 0x0000,
    CMSG_PETITION_BUY                                 = 0x0000,
    CMSG_PETITION_DECLINE                             = 0x0000,
    CMSG_PETITION_QUERY                               = 0x0000,
    CMSG_PETITION_RENAME                              = 0x0000,
    CMSG_PETITION_SHOWLIST                            = 0x0000,
    CMSG_PETITION_SHOW_SIGNATURES                     = 0x0000,
    CMSG_PETITION_SIGN                                = 0x0000,
    CMSG_PET_ABANDON                                  = 0x0000,
    CMSG_PET_ACTION                                   = 0x0000,
    CMSG_PET_CANCEL_AURA                              = 0x0000,
    CMSG_PET_CAST_SPELL                               = 0x0000,
    CMSG_PET_NAME_QUERY                               = 0x0000,
    CMSG_PET_RENAME                                   = 0x0000,
    CMSG_PET_SET_ACTION                               = 0x0000,
    CMSG_PET_STOP_ATTACK                              = 0x0000,
    CMSG_PLAYED_TIME                                  = 0x0000,
    CMSG_PLAYER_DIFFICULTY_CHANGE                     = 0x0000,
    CMSG_PLAYER_LOGOUT                                = 0x0000,
    CMSG_PLAYER_VEHICLE_ENTER                         = 0x0000,
    CMSG_PLAY_DANCE                                   = 0x0000,
    CMSG_PVP_LOG_DATA                                 = 0x0000,
    CMSG_QUERY_BATTLEFIELD_STATE                      = 0x0000,
    CMSG_QUERY_GUILD_MEMBERS_FOR_RECIPE               = 0x0000,
    CMSG_QUERY_GUILD_MEMBER_RECIPES                   = 0x0000,
    CMSG_QUERY_GUILD_RECIPES                          = 0x0000,
    CMSG_QUERY_GUILD_REWARDS                          = 0x0000,
    CMSG_QUERY_GUILD_XP                               = 0x0000,
    CMSG_QUERY_INSPECT_ACHIEVEMENTS = 0x0000,
    CMSG_QUERY_QUESTS_COMPLETED = 0x0000,
    CMSG_QUERY_TIME                                   = 0x0000,
    CMSG_QUEST_NPC_QUERY                              = 0x0000,
    CMSG_RAID_LEADER_READY_CHECK                      = 0x0000,
    CMSG_RAID_CONFIRM_READY_CHECK                     = 0x0000,
    CMSG_RAID_TARGET_UPDATE                           = 0x0000,
    CMSG_RANDOM_ROLL                                  = 0x0000,
    CMSG_RANDOMIZE_CHAR_NAME                          = 0x0000,
    CMSG_READ_ITEM                                    = 0x0000,
    CMSG_REALM_SPLIT                                  = 0x0000,
    CMSG_RECLAIM_CORPSE                               = 0x0000,
    CMSG_REDIRECTION_AUTH_PROOF                       = 0x0000,
    CMSG_REFORGE_ITEM                                 = 0x0000,
    CMSG_REORDER_CHARACTERS                           = 0x0000,
    CMSG_REPOP_REQUEST                                = 0x0000,
    CMSG_REPORT_BUG                                   = 0x0000,
    CMSG_REPORT_PVP_AFK                               = 0x0000,
    CMSG_REPORT_SUGGESTION                            = 0x0000,
    CMSG_REQUEST_ACCOUNT_DATA                         = 0x0000,
    CMSG_REQUEST_BATTLEPET_JOURNAL                    = 0x0000,
    CMSG_REQUEST_CATEGORY_COOLDOWNS                   = 0x0000,
    CMSG_REQUEST_CEMETERY_LIST                        = 0x0000,
    CMSG_REQUEST_FORCED_REACTIONS                     = 0x0000,
    CMSG_REQUEST_GM_TICKET                            = 0x0000,
    CMSG_REQUEST_INSPECT_RATED_BG_STATS               = 0x0000,
    CMSG_REQUEST_PARTY_MEMBER_STATS                   = 0x0000,
    CMSG_REQUEST_PVP_OPTIONS_ENABLED                  = 0x0000,
    CMSG_REQUEST_PVP_REWARDS                          = 0x0000,
    CMSG_REQUEST_RAID_INFO                            = 0x0000,
    CMSG_REQUEST_RATED_BG_INFO                        = 0x0000,
    CMSG_REQUEST_RATED_BG_STATS                       = 0x0000,
    CMSG_REQUEST_RESEARCH_HISTORY                     = 0x0000,
    CMSG_REQUEST_VEHICLE_EXIT                         = 0x0000,
    CMSG_REQUEST_VEHICLE_NEXT_SEAT                    = 0x0000,
    CMSG_REQUEST_VEHICLE_PREV_SEAT                    = 0x0000,
    CMSG_REQUEST_VEHICLE_SWITCH_SEAT                  = 0x0000 + 1,
    CMSG_RESET_CHALLENGE_MODE                         = 0x0000,
    CMSG_RESET_FACTION_CHEAT                          = 0x0000,
    CMSG_RESET_INSTANCES                              = 0x0000,
    CMSG_RESURRECT_RESPONSE                           = 0x0000,
    CMSG_RETURN_TO_GRAVEYARD                          = 0x0000,
    CMSG_ROLE_POLL_BEGIN                              = 0x0000,
    CMSG_SAVE_CUF_PROFILES                            = 0x0000,
    CMSG_SAVE_PLAYER                                  = 0x0000,
    CMSG_SEARCH_LFG_JOIN                              = 0x0000,
    CMSG_SEARCH_LFG_LEAVE                             = 0x0000,
    CMSG_SELECT_FACTION                               = 0x0000,
    CMSG_SELF_RES                                     = 0x0000,
    CMSG_SEND_MAIL                                    = 0x0000,
    CMSG_SEND_SOR_REQUEST_VIA_ADDRESS                 = 0x0000,
    CMSG_SEND_SOR_REQUEST_VIA_BNET_ACCOUNT_ID         = 0x0000,
    CMSG_SERVERTIME                                   = 0x0000,
    CMSG_SETDEATHBINDPOINT                            = 0x0000,
    CMSG_SET_ACTIVE_VOICE_CHANNEL                     = 0x0000,
    CMSG_SET_ALLOW_LOW_LEVEL_RAID1                    = 0x0000,
    CMSG_SET_ALLOW_LOW_LEVEL_RAID2                    = 0x0000,
    CMSG_SET_CHANNEL_WATCH                            = 0x0000,
    CMSG_SET_CONTACT_NOTES                            = 0x0000,
    CMSG_SET_CURRENCY_FLAGS                           = 0x0000,
    CMSG_SET_EVERYONE_IS_ASSISTANT                    = 0x0000,
    CMSG_SET_FACTION_ATWAR                            = 0x0000,
    CMSG_UNSET_FACTION_ATWAR                          = 0x0000,
    CMSG_SET_FACTION_CHEAT                            = 0x0000,
    CMSG_SET_LOOT_SPECIALIZATION                      = 0x0000,
    CMSG_SET_GUILD_BANK_TEXT                          = 0x0000,
    CMSG_SET_LFG_COMMENT                              = 0x0000,
    CMSG_SET_PET_SLOT                                 = 0x0000,
    CMSG_SET_PLAYER_DECLINED_NAMES                    = 0x0000,
    CMSG_SET_PREFERED_CEMETERY                        = 0x0000,
    CMSG_SET_RELATIVE_POSITION                        = 0x0000,
    CMSG_SET_SAVED_INSTANCE_EXTEND                    = 0x0000,
    CMSG_SET_SKILL_CHEAT                              = 0x0000,
    CMSG_SET_TITLE                                    = 0x0000,
    CMSG_SET_TRADE_CURRENCY                           = 0x0000,
    CMSG_SET_TRADE_GOLD                               = 0x0000,
    CMSG_SET_TRADE_ITEM                               = 0x0000,
    CMSG_SET_VEHICLE_REC_ID_ACK                       = 0x0000,
    CMSG_SHOW_ACCOUNT_ACHIEVEMENT                     = 0x0000,
    CMSG_SPELLCLICK                                   = 0x0000,
    CMSG_SOCKET_GEMS                                  = 0x0000,
    CMSG_SPIRIT_HEALER_ACTIVATE                       = 0x0000,
    CMSG_SPLIT_ITEM                                   = 0x0000,
    CMSG_STANDSTATECHANGE                             = 0x0000,
    CMSG_START_QUEST                                  = 0x0000,
    CMSG_STOP_DANCE                                   = 0x0000,
    CMSG_STORE_LOOT_IN_SLOT                           = 0x0000,
    CMSG_SUBMIT_BUG                                   = 0x0000,
    CMSG_SUBMIT_COMPLAIN                              = 0x0000,
    CMSG_SUGGESTION_SUBMIT                            = 0x0000,
    CMSG_SUMMON_BATTLEPET_COMPANION                   = 0x0000,
    CMSG_SUMMON_RESPONSE                              = 0x0000,
    CMSG_SUSPEND_TOKEN                                = 0x0000,
    CMSG_SAVE_GUILD_EMBLEM                            = 0x0000,
    CMSG_SWAP_ITEM                                    = 0x0000,
    CMSG_SYNC_DANCE                                   = 0x0000,
    CMSG_TELEPORT_TO_UNIT                             = 0x0000,
    CMSG_TEXT_EMOTE                                   = 0x0000,
    CMSG_TIME_ADJUSTMENT_RESPONSE                     = 0x0000,
    CMSG_TIME_SYNC_RESP                               = 0x0000,
    CMSG_TIME_SYNC_RESP_FAILED                        = 0x0000,
    CMSG_TOTEM_DESTROYED                              = 0x0000,
    CMSG_TRADE_INFO                                   = 0x0000,
    CMSG_TRAINER_BUY_SPELL                            = 0x0000,
    CMSG_TRAINER_LIST                                 = 0x0000,
    CMSG_TRANSMOGRIFY_ITEMS                           = 0x0000,
    CMSG_TRIGGER_CINEMATIC_CHEAT                      = 0x0000,
    CMSG_TURN_IN_PETITION                             = 0x0000,
    CMSG_UNACCEPT_TRADE                               = 0x0000,
    CMSG_UNLEARN_SKILL                                = 0x0000,
    CMSG_UNLEARN_SPECIALIZATION                       = 0x0000,
    CMSG_UNREGISTER_ALL_ADDON_PREFIXES                = 0x0000,
    CMSG_UPDATE_ACCOUNT_DATA                          = 0x0000,
    CMSG_UPDATE_MISSILE_TRAJECTORY                    = 0x0000,
    CMSG_UPDATE_PROJECTILE_POSITION                   = 0x0000,
    CMSG_UPGRADE_ITEM                                 = 0x0000,
    CMSG_USED_FOLLOW                                  = 0x0000,
    CMSG_VOICE_SESSION_ENABLE                         = 0x0000,
    CMSG_VOID_STORAGE_QUERY                           = 0x0000,
    CMSG_VOID_STORAGE_TRANSFER                        = 0x0000,
    CMSG_VOID_STORAGE_UNLOCK                          = 0x0000,
    CMSG_VOID_SWAP_ITEM                               = 0x0000,
    CMSG_WARDEN_DATA                                  = 0x0000,
    CMSG_WARGAME_ACCEPT                               = 0x0000,
    CMSG_WARGAME_START                                = 0x0000,
    CMSG_WHO                                          = 0x0000,
    CMSG_WHOIS                                        = 0x0000,
    CMSG_WORLD_STATE_UI_TIMER_UPDATE                  = 0x0000,
    CMSG_WORLD_TELEPORT                               = 0x0000,
    CMSG_WRAP_ITEM                                    = 0x0000,
    CMSG_ZONEUPDATE                                   = 0x0000,
    CMSG_CORPSE_QUERY                                 = 0x0000,
    CMSG_CONQUEST_POINTS_STATS_REQUEST                = 0x0000,
    CMSG_ARENA_2v2_STATS_REQUEST                      = 0x0000,
    CMSG_ARENA_3v3_STATS_REQUEST                      = 0x0000,

    // SMSG
    SMSG_SUPERCEDED_SPELL                             = 0x0000,
    SMSG_INITIAL_SPELLS                               = 0x0000,
    SMSG_SPELLBREAKLOG                                = 0x0000,
    SMSG_SPELLSTEALLOG                                = 0x0000,
    SMSG_SPELL_FAILED_OTHER                           = 0x0000,
    SMSG_SPELL_FAILURE                                = 0x0000,
    SMSG_ACCOUNT_CRITERIA_UPDATE                      = 0x0000,
    SMSG_ACCOUNT_CRITERIA_UPDATE_ALL                  = 0x0000,
    SMSG_ACCOUNT_INFO_RESPONSE                        = 0x0000,
    SMSG_ACCOUNT_PROFILE                              = 0x0000,
    SMSG_ACCOUNT_RESTRICTED_WARNING                   = 0x0000,
    SMSG_ACHIEVEMENT_DELETED                          = 0x0000,
    SMSG_ACHIEVEMENT_EARNED                           = 0x0000,
    SMSG_ADJUST_SPLINE_DURATION                       = 0x0000,
    SMSG_ADD_BATTLENET_FRIEND_RESPONSE                = 0x0000,
    SMSG_ADD_LOSS_OF_CONTROL                          = 0x0000,
    SMSG_ADD_RUNE_POWER                               = 0x0000,
    SMSG_AE_LOOT_TARGETS                              = 0x0000,
    SMSG_AE_LOOT_TARGETS_ACK                          = 0x0000,
    SMSG_APPLY_MOVEMENT_FORCE                         = 0x0000,
    SMSG_AREA_SPIRIT_HEALER_TIME                      = 0x0000,
    SMSG_AREA_SHARE_MAPPINGS_RESPONSE                 = 0x0000,
    SMSG_AREA_SHARE_INFO_RESPONSE                     = 0x0000,
    SMSG_AREA_TRIGGER_DENIED                          = 0x0000,
    SMSG_AREA_TRIGGER_MESSAGE                         = 0x0000,
    SMSG_AREA_TRIGGER_MOVEMENT_UPDATE                 = 0x0000,
    SMSG_AREA_TRIGGER_NO_CORPSE                       = 0x0000,
    SMSG_AREA_TRIGGER_RESET                           = 0x0000,
    SMSG_ARENA_ERROR                                  = 0x0000,
    SMSG_ARENA_OPPONENT_UPDATE                        = 0x0000,
    SMSG_ARENA_OPPONENT_SPECIALIZATIONS               = 0x0000,
    SMSG_ARENA_TEAM_CHANGE_FAILED_QUEUED              = 0x0000,
    SMSG_ARENA_TEAM_COMMAND_RESULT                    = 0x0000,
    SMSG_ARENA_TEAM_EVENT                             = 0x0000,
    SMSG_ARENA_TEAM_INVITE                            = 0x0000,
    SMSG_ARENA_TEAM_QUERY_RESPONSE                    = 0x0000,
    SMSG_ARENA_TEAM_ROSTER                            = 0x0000,
    SMSG_ARENA_TEAM_STATS                             = 0x0000,
    SMSG_AUCTION_BIDDER_LIST_RESULT                   = 0x0000,
    SMSG_AUCTION_BIDDER_NOTIFICATION                  = 0x0000,
    SMSG_AUCTION_BUYOUT_NOTIFICATION                  = 0x0000,
    SMSG_AUCTION_COMMAND_RESULT                       = 0x0000,
    SMSG_AUCTION_CLOSED_NOTIFICATION                  = 0x0000,
    SMSG_AUCTION_HELLO_RESPONSE                       = 0x0000,
    SMSG_AUCTION_LIST_PENDING_SALES                   = 0x0000,
    SMSG_AUCTION_LIST_RESULT                          = 0x0000,
    SMSG_AUCTION_OWNER_LIST_RESULT                    = 0x0000,
    SMSG_AUCTION_OWNER_BID_NOTIFICATION               = 0x0000,
    SMSG_AUCTION_REMOVED_NOTIFICATION                 = 0x0000,
    SMSG_AVERAGE_ITEM_LEVEL_INFORM                    = 0x0000,
    SMSG_BARBER_SHOP_RESULT                           = 0x0000,
    SMSG_BATTLEFIELD_LIST                             = 0x0000,
    SMSG_BATTLEFIELD_MGR_EJECTED                      = 0x0000,
    SMSG_BATTLEFIELD_MGR_EJECT_PENDING                = 0x0000,
    SMSG_BATTLEFIELD_MGR_ENTERING                     = 0x0000,
    SMSG_BATTLEFIELD_MGR_ENTRY_INVITE                 = 0x0000,
    SMSG_BATTLEFIELD_MGR_EXIT_REQUEST                 = 0x0000,
    SMSG_BATTLEFIELD_MGR_QUEUE_INVITE                 = 0x0000,
    SMSG_BATTLEFIELD_MGR_QUEUE_REQUEST_RESPONSE       = 0x0000,
    SMSG_BATTLEFIELD_MGR_STATE_CHANGED                = 0x0000,
    SMSG_BATTLEFIELD_PORT_DENIED                      = 0x0000,
    SMSG_BATTLEFIELD_RATED_INFO                       = 0x0000,
    SMSG_BATTLEFIELD_STATUS                           = 0x0000,
    SMSG_BATTLEFIELD_STATUS_QUEUED                    = 0x0000,
    SMSG_BATTLEFIELD_STATUS_NEED_CONFIRMATION         = 0x0000,
    SMSG_BATTLEFIELD_STATUS_ACTIVE                    = 0x0000,
    SMSG_BATTLEFIELD_STATUS_WAIT_FOR_GROUPS           = 0x0000,
    SMSG_BATTLEFIELD_STATUS_FAILED                    = 0x0000,
    SMSG_BATTLEGROUND_INIT                            = 0x0000,
    SMSG_BATTLEGROUND_INFO_THROTTLED                  = 0x0000,
    SMSG_BATTLEGROUND_POINTS                          = 0x0000,
    SMSG_BATTLEGROUND_PLAYER_JOINED                   = 0x0000,
    SMSG_BATTLEGROUND_PLAYER_LEFT                     = 0x0000,
    SMSG_BATTLEGROUND_PLAYER_POSITIONS                = 0x0000,
    SMSG_BATTLEPET_CAGE_DATA_ERROR                    = 0x0000,
    SMSG_BATTLEPET_DELETE                             = 0x0000,
    SMSG_BATTLEPET_ERROR                              = 0x0000,
    SMSG_BATTLEPET_HEALED                             = 0x0000,
    SMSG_BATTLEPET_JOURNAL                            = 0x0000,
    SMSG_BATTLEPET_JOURNAL_LOCK_ACQUIRED              = 0x0000,
    SMSG_BATTLEPET_JOURNAL_LOCK_DENIED                = 0x0000,
    SMSG_BATTLEPET_LICENSE_CHANGED                    = 0x0000,
    SMSG_BATTLEPET_TRAP_LEVEL                         = 0x0000,
    SMSG_BATTLEPET_UPDATE                             = 0x0000,
    SMSG_BEASTMASTER_FAILED                           = 0x0000,
    SMSG_BINDER_CONFIRM                               = 0x0000,
    SMSG_BIND_ZONE_REPLY                              = 0x0000,
    SMSG_BLACK_MARKET_OPEN_RESULT                     = 0x0000,
    SMSG_BLACK_MARKET_OUT_BID                         = 0x0000,
    SMSG_BLACK_MARKET_ITEM_RESULT                     = 0x0000,
    SMSG_BLACK_MARKET_BID_ON_ITEM_RESULT              = 0x0000,
    SMSG_BLACK_MARKET_WON                             = 0x0000,
    SMSG_BONUS_ROLL_EMPTY                             = 0x0000,
    SMSG_BREAK_TARGET                                 = 0x0000,
    SMSG_CALENDAR_ACTION_PENDING                      = 0x0000,
    SMSG_CALENDAR_ARENA_TEAM                          = 0x0000,
    SMSG_CALENDAR_CLEAR_PENDING_ACTION                = 0x0000,
    SMSG_CALENDAR_COMMAND_RESULT                      = 0x0000,
    SMSG_CALENDAR_EVENT_INITIAL_INVITE                = 0x0000,
    SMSG_CALENDAR_EVENT_INVITE                        = 0x0000,
    SMSG_CALENDAR_EVENT_INVITE_ALERT                  = 0x0000,
    SMSG_CALENDAR_EVENT_INVITE_NOTES                  = 0x0000,
    SMSG_CALENDAR_EVENT_INVITE_NOTES_ALERT            = 0x0000,
    SMSG_CALENDAR_EVENT_INVITE_REMOVED                = 0x0000,
    SMSG_CALENDAR_EVENT_INVITE_REMOVED_ALERT          = 0x0000,
    SMSG_CALENDAR_EVENT_INVITE_STATUS                 = 0x0000,
    SMSG_CALENDAR_EVENT_INVITE_STATUS_ALERT           = 0x0000,
    SMSG_CALENDAR_EVENT_MODERATOR_STATUS              = 0x0000,
    SMSG_CALENDAR_EVENT_MODERATOR_STATUS_ALERT        = 0x0000,
    SMSG_CALENDAR_EVENT_REMOVED_ALERT                 = 0x0000,
    SMSG_CALENDAR_EVENT_STATUS                        = 0x0000,
    SMSG_CALENDAR_EVENT_UPDATED_ALERT                 = 0x0000,
    SMSG_CALENDAR_FILTER_GUILD                        = 0x0000,
    SMSG_CALENDAR_RAID_LOCKOUT_ADDED                  = 0x0000,
    SMSG_CALENDAR_RAID_LOCKOUT_REMOVED                = 0x0000,
    SMSG_CALENDAR_RAID_LOCKOUT_UPDATED                = 0x0000,
    SMSG_CALENDAR_SEND_CALENDAR                       = 0x0000,
    SMSG_CALENDAR_SEND_EVENT                          = 0x0000,
    SMSG_CALENDAR_SEND_NUM_PENDING                    = 0x0000,
    SMSG_CALENDAR_UPDATE_INVITE_LIST                  = 0x0000,
    SMSG_CAMERA_SHAKE                                 = 0x0000,
    SMSG_CANCEL_AUTO_REPEAT                           = 0x0000,
    SMSG_CANCEL_COMBAT                                = 0x0000,
    SMSG_CANCEL_SCENE                                 = 0x0000,
    SMSG_CANCEL_ORPHAN_SPELL_VISUAL                   = 0x0000,
    SMSG_CANCEL_SPELL_VISUAL                          = 0x0000,
    SMSG_CANCEL_SPELL_VISUAL_KIT                      = 0x0000,
    SMSG_CAST_FAILED                                  = 0x0000,
    SMSG_CAN_DUEL_RESULT                              = 0x0000,
    SMSG_CHALLENGE_MODE_MAP_STATS_UPDATE              = 0x0000,
    SMSG_CHALLENGE_MODE_DELETE_LEADER_RESULT          = 0x0000,
    SMSG_CHALLENGE_MODE_ALL_MAP_STATS                 = 0x0000,
    SMSG_CHALLENGE_MODE_REQUEST_LEADERS_RESULT        = 0x0000,
    SMSG_CHALLENGE_MODE_REWARDS                       = 0x0000,
    SMSG_CHALLENGE_MODE_NEW_PLAYER_RECORD             = 0x0000,
    SMSG_CHALLENGE_MODE_COMPLETE                      = 0x0000,
    SMSG_CHANGER_PLAYER_DIFFICULTY_RESULT             = 0x0000,
    SMSG_CHANNEL_MEMBER_COUNT                         = 0x0000,
    SMSG_CHARACTER_LOGIN_FAILED                       = 0x0000,
    SMSG_CHAR_CUSTOMIZE                               = 0x0000,
    SMSG_CHAR_FACTION_OR_RACE_CHANGE                  = 0x0000,
    SMSG_CHAR_RENAME                                  = 0x0000,
    SMSG_CHECK_FOR_BOTS                               = 0x0000,
    SMSG_CHEAT_IGNORE_REQUIEREMENTS_DISABLED          = 0x0000,
    SMSG_CHEAT_IGNORE_REQUIEREMENTS_ENABLED           = 0x0000,
    SMSG_CHEAT_PLAYER_LOOKUP                          = 0x0000,
    SMSG_CLEAR_BOSS_EMOTES                            = 0x0000,
    SMSG_CLEAR_FAR_SIGHT_IMMEDIATE                    = 0x0000,
    SMSG_CLEAR_LOSS_OF_CONTROL                        = 0x0000,
    SMSG_CLEAR_TARGET                                 = 0x0000,
    SMSG_CLEAR_ITEM_CHALLENGE_MODE_DATA               = 0x0000,
    SMSG_CLIENT_CONTROL_UPDATE                        = 0x0000,
    SMSG_COMBAT_EVENT_FAILED                          = 0x0000,
    SMSG_COMBAT_LOG_MULTIPLE                          = 0x0000,
    SMSG_COMMENTATOR_MAP_INFO                         = 0x0000,
    SMSG_COMMENTATOR_PARTY_INFO                       = 0x0000,
    SMSG_COMMENTATOR_PLAYER_INFO                      = 0x0000,
    SMSG_COMMENTATOR_SKIRMISH_QUEUE_RESULT1           = 0x0000,
    SMSG_COMMENTATOR_SKIRMISH_QUEUE_RESULT2           = 0x0000,
    SMSG_COMMENTATOR_STATE_CHANGED                    = 0x0000,
    SMSG_COMPLAIN_RESULT                              = 0x0000,
    SMSG_COMPRESSED_ACHIEVEMENT_DATA                  = 0x0000,
    SMSG_COMPRESSED_CHAR_ENUM                         = 0x0000,
    SMSG_COMPRESSED_GUILD_ROSTER                      = 0x0000,
    SMSG_COMPRESSED_MOVES                             = 0x0000,
    SMSG_COMPRESSED_UPDATE_OBJECT                     = 0x0000,
    SMSG_COMSAT_CONNECT_FAIL                          = 0x0000,
    SMSG_COMSAT_DISCONNECT                            = 0x0000,
    SMSG_COMSAT_RECONNECT_TRY                         = 0x0000,
    SMSG_CONSOLE_WRITE                                = 0x0000,
    SMSG_CONVERT_RUNE                                 = 0x0000,
    SMSG_COOLDOWN_CHEAT                               = 0x0000,
    SMSG_COOLDOWN_EVENT                               = 0x0000,
    SMSG_CORPSE_QUERY                                 = 0x0000,
    SMSG_CORPSE_MAP_POSITION_QUERY_RESPONSE           = 0x0000,
    SMSG_CORPSE_NOT_IN_INSTANCE                       = 0x0000,
    SMSG_CORPSE_RECLAIM_DELAY                         = 0x0000,
    SMSG_CORPSE_TRANSPORT_QUERY                       = 0x0000,
    SMSG_CRITERIA_DELETED                             = 0x0000,
    SMSG_CRITERIA_UPDATE                              = 0x0000,
    SMSG_CROSSED_INEBRIATION_THRESHOLD                = 0x0000,
    SMSG_CURRENCY_LOOT_REMOVED                        = 0x0000,
    SMSG_CURRENCY_LOOT_RESTORED                       = 0x0000,
    SMSG_CUSTOM_LOAD_SCREEN                           = 0x0000,
    SMSG_DAMAGE_CALC_LOG                              = 0x0000,
    SMSG_DAMAGE_DONE_OBSOLETE                         = 0x0000,
    SMSG_DANCE_QUERY_RESPONSE                         = 0x0000,
    SMSG_DB_LOOKUP_RESULT                             = 0x0000,
    SMSG_DEATH_RELEASE_LOC                            = 0x0000,
    SMSG_DEBUG_RUNE_REGEN                             = 0x0000,
    SMSG_DESTROY_ARENA_UNIT                           = 0x0000,
    SMSG_DESTRUCTIBLE_BUILDING_DAMAGE                 = 0x0000,
    SMSG_DIFFERENT_INSTANCE_FROM_PARTY                = 0x0000,
    SMSG_DISENCHANT_CREDIT                            = 0x0000,
    SMSG_DISMOUNT                                     = 0x0000,
    SMSG_DISMOUNTRESULT                               = 0x0000,
    SMSG_DISPLAY_GAME_ERROR                           = 0x0000,
    SMSG_DISPLAY_PROMOTION                            = 0x0000,
    SMSG_DISPLAY_PLAYER_CHOICE                        = 0x0000,
    SMSG_DONT_AUTO_PUSH_SPELLS_TO_ACTION_BAR          = 0x0000,
    SMSG_DROP_NEW_CONNECTION                          = 0x0000,
    SMSG_DUEL_COMPLETE                                = 0x0000,
    SMSG_DUEL_COUNTDOWN                               = 0x0000,
    SMSG_DUEL_INBOUNDS                                = 0x0000,
    SMSG_DUEL_OUT_OF_BOUNDS                           = 0x0000,
    SMSG_DUEL_REQUESTED                               = 0x0000,
    SMSG_DUEL_WINNER                                  = 0x0000,
    SMSG_DUMP_RIDE_TICKETS_RESPONSE                   = 0x0000,
    SMSG_DUMP_OBJECTS_DATA                            = 0x0000,
    SMSG_DURABILITY_DAMAGE_DEATH                      = 0x0000,
    SMSG_DYNAMIC_DROP_ROLL_RESULT                     = 0x0000,
    SMSG_ECHO_PARTY_SQUELCH                           = 0x0000,
    SMSG_ENABLE_BARBER_SHOP                           = 0x0000,
    SMSG_ENABLE_HONOR_BUDDY_DETECTION                 = 0x0000,
    SMSG_ENCHANTMENT_LOG                              = 0x0000,
    SMSG_EQUIPMENT_SET_SAVED                          = 0x0000,
    SMSG_FACTION_BONUS_INFO                           = 0x0000,
    SMSG_FAILED_PLAYER_CONDITION                      = 0x0000,
    SMSG_FEIGN_DEATH_RESISTED                         = 0x0000,
    SMSG_FISH_ESCAPED                                 = 0x0000,
    SMSG_FISH_NOT_HOOKED                              = 0x0000,
    SMSG_FLIGHT_SPLINE_SYNC                           = 0x0000,
    SMSG_FLOOD_DETECTED                               = 0x0000,
    SMSG_FORCE_ANIM                                   = 0x0000,
    SMSG_FORCE_OBJECT_RELINK                          = 0x0000,
    SMSG_FORCE_ACTION_SHOW                            = 0x0000,
    SMSG_FORCED_DEATH_UPDATE                          = 0x0000,
    SMSG_FORCE_DISPLAY_UPDATE                         = 0x0000,
    SMSG_FORCE_SEND_QUEUED_PACKETS                    = 0x0000,
    SMSG_FORCE_SET_VEHICLE_REC_ID                     = 0x0000,
    SMSG_FORGE_MASTER_SET                             = 0x0000,
    SMSG_FRIEND_STATUS                                = 0x0000,
    SMSG_GAMEOBJECT_CUSTOM_ANIM                       = 0x0000,
    SMSG_GAMEOBJECT_DESPAWN_ANIM                      = 0x0000,
    SMSG_GAMEOBJECT_PAGETEXT                          = 0x0000,
    SMSG_GAMEOBJECT_RESET_STATE                       = 0x0000,
    SMSG_GAMEOBJECT_PLAY_SPELL_VISUAL                 = 0x0000,
    SMSG_GAME_SPEED_SET                               = 0x0000,
    SMSG_GAME_TIME_SET                                = 0x0000,
    SMSG_GAME_TIME_UPDATE                             = 0x0000,
    SMSG_GAME_EVENT_DEBUG_LOG                         = 0x0000,
    SMSG_GAME_OBJECT_ACTIVATE_ANIM_KIT                = 0x0000,
    SMSG_GAME_OBJECT_DESPAWN                          = 0x0000,
    SMSG_GHOST                                        = 0x0000,
    SMSG_GHOSTEE_GONE                                 = 0x0000,
    SMSG_GM_ACCOUNT_ONLINE_RESPONSE                   = 0x0000,
    SMSG_GM_CASE_STATUS                               = 0x0000,
    SMSG_GM_CHANGE_ARENA_RATING                       = 0x0000,
    SMSG_GM_CHARACTER_RESTORE_RESPONSE                = 0x0000,
    SMSG_GM_CHARACTER_SAVE_FAILURE                    = 0x0000,
    SMSG_GM_CHARACTER_SAVE_SUCESS                     = 0x0000,
    SMSG_GM_DESTROY_CORPSE                            = 0x0000,
    SMSG_GM_FREEZE_FAILURE                            = 0x0000,
    SMSG_GM_FREEZE_SUCESS                             = 0x0000,
    SMSG_GM_GEAR_RATING_RESPONSE                      = 0x0000,
    SMSG_GM_MESSAGECHAT                               = 0x0000,
    SMSG_GM_NAMED_POINTS                              = 0x0000,
    SMSG_GM_NOTIFY_AREA_CHANGE                        = 0x0000,
    SMSG_GM_PLAYER_INFO                               = 0x0000,
    SMSG_GM_PHASE_DUMP                                = 0x0000,
    SMSG_GM_RESPONSE_DB_ERROR                         = 0x0000,
    SMSG_GM_RESPONSE_RECEIVED                         = 0x0000,
    SMSG_GM_RESPONSE_STATUS_UPDATE                    = 0x0000,
    SMSG_GM_RESURRECT_FAILURE                         = 0x0000,
    SMSG_GM_RESURRECT_SUCESS                          = 0x0000,
    SMSG_GM_REQUEST_PLAYER_INFO                       = 0x0000,
    SMSG_GM_REVEAL_TO                                 = 0x0000,
    SMSG_GM_SILENCE                                   = 0x0000,
    SMSG_GM_SHOW_COMPLAINTS                           = 0x0000,
    SMSG_GM_SUMMON                                    = 0x0000,
    SMSG_GM_TICKET_GET_TICKET_RESPONSE                = 0x0000,
    SMSG_GM_TICKET_RESPONSE_ERROR                     = 0x0000,
    SMSG_GM_TICKET_RESOLVE_EVAL                       = 0x0000,
    SMSG_GM_TICKET_SYSTEM_STATUS                      = 0x0000,
    SMSG_GM_TICKET_STATUS_UPDATE                      = 0x0000,
    SMSG_GM_UNSQUELCH                                 = 0x0000,
    SMSG_GOD_MODE                                     = 0x0000,
    SMSG_GROUP_ACTION_THROTTLED                       = 0x0000,
    SMSG_GROUP_CANCEL                                 = 0x0000,
    SMSG_GROUP_DESTROYED                              = 0x0000,
    SMSG_GROUP_JOINED_BATTLEGROUND                    = 0x0000,
    SMSG_GROUP_SET_LEADER                             = 0x0000,
    SMSG_GROUP_UNINVITE                               = 0x0000,
    SMSG_HEALTH_UPDATE                                = 0x0000,
    SMSG_HIGHEST_THREAT_UPDATE                        = 0x0000,
    SMSG_IMMIGRANT_HOST_SEARCH_LOG                    = 0x0000,
    SMSG_INSPECT                                      = 0x0000,
    SMSG_INSPECT_RATED_BG_STATS                       = 0x0000,
    SMSG_INSPECT_HONOR_STATS                          = 0x0000,
    SMSG_INSPECT_RESULTS_UPDATE                       = 0x0000,
    SMSG_INSPECT_TALENT                               = 0x0000,
    SMSG_INSTANCE_DIFFICULTY                          = 0x0000,
    SMSG_INSTANCE_LOCK_WARNING_QUERY                  = 0x0000,
    SMSG_INSTANCE_RESET                               = 0x0000,
    SMSG_INSTANCE_RESET_FAILED                        = 0x0000,
    SMSG_INSTANCE_SAVE_CREATED                        = 0x0000,
    SMSG_INVALIDATE_DANCE                             = 0x0000,
    SMSG_INVALIDATE_PLAYER                            = 0x0000,
    SMSG_INVALID_PROMOTION_CODE                       = 0x0000,
    SMSG_ITEM_ADD_PASSIVE                             = 0x0000,
    SMSG_ITEM_COOLDOWN                                = 0x0000,
    SMSG_ITEM_ENCHANT_TIME_UPDATE                     = 0x0000,
    SMSG_ITEM_REFUND_INFO_RESPONSE                    = 0x0000,
    SMSG_ITEM_REFUND_RESULT                           = 0x0000,
    SMSG_ITEM_REMOVE_PASSIVE                          = 0x0000,
    SMSG_ITEM_SEND_PASSIVE                            = 0x0000,
    SMSG_ITEM_TEXT_QUERY_RESPONSE                     = 0x0000,
    SMSG_ITEM_TIME_UPDATE                             = 0x0000,
    SMSG_ITEM_UPGRADE_RESULT                          = 0x0000,
    SMSG_JOINED_BATTLEGROUND_QUEUE                    = 0x0000,
    SMSG_KICK_REASON                                  = 0x0000,
    SMSG_LEARNED_DANCE_MOVES                          = 0x0000,
    SMSG_LFG_BOOT_PROPOSAL_UPDATE                     = 0x0000,
    SMSG_LFG_BOOT_PLAYER                              = 0x0000,
    SMSG_LFG_DISABLED                                 = 0x0000,
    SMSG_LFG_JOIN_RESULT                              = 0x0000,
    SMSG_LFG_OFFER_CONTINUE                           = 0x0000,
    SMSG_LFG_OPEN_FROM_GOSSIP                         = 0x0000,
    SMSG_LFG_PARTY_INFO                               = 0x0000,
    SMSG_LFG_PLAYER_INFO                              = 0x0000,
    SMSG_LFG_PLAYER_REWARD                            = 0x0000,
    SMSG_LFG_PROPOSAL_UPDATE                          = 0x0000,
    SMSG_LFG_QUEUE_STATUS                             = 0x0000,
    SMSG_LFG_ROLE_CHECK_UPDATE                        = 0x0000,
    SMSG_LFG_ROLE_CHOSEN                              = 0x0000,
    SMSG_LFG_SEARCH_RESULT                            = 0x0000,
    SMSG_LFG_SLOT_INVALID                             = 0x0000,
    SMSG_LFG_TELEPORT_DENIED                          = 0x0000,
    SMSG_LFG_UPDATE_LIST                              = 0x0000,
    SMSG_LFG_UPDATE_PARTY                             = 0x0000,
    SMSG_LFG_UPDATE_PLAYER                            = 0x0000,
    SMSG_LFG_UPDATE_SEARCH                            = 0x0000,
    SMSG_LFG_UPDATE_STATUS                            = 0x0000,
    SMSG_LFG_UPDATE_STATUS_NONE                       = 0x0000,
    SMSG_LIST_TARGETS                                 = 0x0000,
    SMSG_LOAD_CUF_PROFILES                            = 0x0000,
    SMSG_LOSS_OF_CONTROL_AURA_UPDATE                  = 0x0000,
    SMSG_LOOT_ALL_PASSED                              = 0x0000,
    SMSG_LOOT_CONTENTS                                = 0x0000,
    SMSG_LOOT_ITEM_LIST                               = 0x0000,
    SMSG_MASTER_LOOT_CANDIDATE_LIST                   = 0x0000,
    SMSG_LOOT_RELEASE_RESPONSE                        = 0x0000,
    SMSG_LOOT_ROLL                                    = 0x0000,
    SMSG_LOOT_ROLLS_COMPLETE                          = 0x0000,
    SMSG_LOOT_ROLL_WON                                = 0x0000,
    SMSG_LOOT_START_ROLL                              = 0x0000,
    SMSG_LOOT_UPDATED                                 = 0x0000,
    SMSG_MAIL_LIST_RESULT                             = 0x0000,
    SMSG_MAP_OBJ_EVENTS                               = 0x0000,
    SMSG_MEETINGSTONE_COMPLETE                        = 0x0000,
    SMSG_MEETINGSTONE_IN_PROGRESS                     = 0x0000,
    SMSG_MEETINGSTONE_MEMBER_ADDED                    = 0x0000,
    SMSG_MEETINGSTONE_SETQUEUE                        = 0x0000,
    SMSG_MESSAGE_BOX                                  = 0x0000,
    SMSG_MINIGAME_SETUP                               = 0x0000,
    SMSG_MINIGAME_STATE                               = 0x0000,
    SMSG_MINIMAP_PING                                 = 0x0000,
    SMSG_MISSILE_CANCEL                               = 0x0000,
    SMSG_MODIFY_COOLDOWN                              = 0x0000,
    SMSG_MONEY_NOTIFY                                 = 0x0000,
    SMSG_MOUNT_RESULT                                 = 0x0000,
    SMSG_MOUNT_SPECIAL_ANIM                           = 0x0000,
    SMSG_MOVE_COLLISION_DISABLE                       = 0x0000,
    SMSG_MOVE_COLLISION_ENABLE                        = 0x0000,
    SMSG_MOVE_CHARACTER_CHEAT_FAILURE                 = 0x0000,
    SMSG_MOVE_CHARACTER_CHEAT_SUCCESS                 = 0x0000,
    SMSG_MOVE_GRAVITY_DISABLE                         = 0x0000,
    SMSG_MOVE_GRAVITY_ENABLE                          = 0x0000,
    SMSG_MOVE_KNOCK_BACK                              = 0x0000,
    SMSG_MOVE_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY = 0x0000,
    SMSG_MOVE_SET_CAN_TURN_WHILE_FALLING              = 0x0000,
    SMSG_MOVE_SET_COLLISION_HEIGHT                    = 0x0000,
    SMSG_MOVE_SET_COMPOUND_STATE                      = 0x0000,
    SMSG_MOVE_SET_HOVER                               = 0x0000,
    SMSG_MOVE_SET_VEHICLE_REC_ID                      = 0x0000,
    SMSG_MOVE_UNSET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY = 0x0000,
    SMSG_MOVE_UNSET_CAN_TURN_WHILE_FALLING            = 0x0000,
    SMSG_MOVE_UNSET_HOVER                             = 0x0000,
    SMSG_MOVE_UPDATE_COLLISION_HEIGHT                 = 0x0000,
    SMSG_MOVE_UPDATE_FLIGHT_BACK_SPEED                = 0x0000,
    SMSG_MOVE_UPDATE_KNOCK_BACK                       = 0x0000,
    SMSG_MOVE_UPDATE_PITCH_RATE                       = 0x0000,
    SMSG_MOVE_UPDATE_RUN_BACK_SPEED                   = 0x0000,
    SMSG_MOVE_UPDATE_SWIM_BACK_SPEED                  = 0x0000,
    SMSG_MOVE_UPDATE_SWIM_SPEED                       = 0x0000,
    SMSG_MOVE_UPDATE_TELEPORT                         = 0x0000,
    SMSG_MOVE_UPDATE_TURN_RATE                        = 0x0000,
    SMSG_MOVE_UPDATE_WALK_SPEED                       = 0x0000,
    SMSG_MULTIPLE_PACKETS                             = 0x0000,
    SMSG_NEW_WORLD_ABORT                              = 0x0000,
    SMSG_NEUTRAL_PLAYER_FACTION_SELECT_RESULT         = 0x0000,
    SMSG_NOTIFICATION                                 = 0x0000,
    SMSG_NOTIFY_DANCE                                 = 0x0000,
    SMSG_NOTIFY_DEST_LOC_SPELL_CAST                   = 0x0000,
    SMSG_NPC_WONT_TALK                                = 0x0000,
    SMSG_ON_CANCEL_EXPECTED_RIDE_VEHICLE_AURA         = 0x0000,
    SMSG_OPEN_CONTAINER                               = 0x0000,
    SMSG_OPEN_LFG_DUNGEON_FINDER                      = 0x0000,
    SMSG_OVERRIDE_LIGHT                               = 0x0000,
    SMSG_PAGE_TEXT_QUERY_RESPONSE                     = 0x0000,
    SMSG_PAGE_TEXT                                    = 0x0000,
    SMSG_PARTY_COMMAND_RESULT                         = 0x0000,
    SMSG_PARTY_KILL_LOG                               = 0x0000,
    SMSG_PARTY_MEMBER_STATS                           = 0x0000,
    SMSG_PARTY_MEMBER_STATS_FULL                      = 0x0000,
    SMSG_PAUSE_MIRROR_TIMER                           = 0x0000,
    SMSG_PENDING_RAID_LOCK                            = 0x0000,
    SMSG_PETITION_ALREADY_SIGNED                      = 0x0000,
    SMSG_PETITION_DECLINED                            = 0x0000,
    SMSG_PETITION_QUERY_RESPONSE                      = 0x0000,
    SMSG_PETITION_RENAME                              = 0x0000,
    SMSG_PETITION_SHOW_LIST                           = 0x0000,
    SMSG_PETITION_SHOW_SIGNATURES                     = 0x0000,
    SMSG_PET_ACTION_FEEDBACK                          = 0x0000,
    SMSG_PET_ACTION_SOUND                             = 0x0000,
    SMSG_PET_ADDED                                    = 0x0000,
    SMSG_PET_BROKEN                                   = 0x0000,
    SMSG_PET_BATTLE_CHAT_RESTRICTED                   = 0x0000,
    SMSG_PET_BATTLE_DEBUG_QUEUE_DUMP_RESPONSE         = 0x0000,
    SMSG_PET_BATTLE_FULL_UPDATE                       = 0x0000,
    SMSG_PET_BATTLE_FINAL_ROUND                       = 0x0000,
    SMSG_PET_BATTLE_FINISHED                          = 0x0000,
    SMSG_PET_BATTLE_FIRST_ROUND                       = 0x0000,
    SMSG_PET_BATTLE_MAX_GAME_LENGHT_WARNING           = 0x0000,
    SMSG_PET_BATTLE_PVP_CHALLENGE                     = 0x0000,
    SMSG_PET_BATTLE_QUEUE_PROPOSE_MATCH               = 0x0000,
    SMSG_PET_BATTLE_QUEUE_STATUS                      = 0x0000,
    SMSG_PET_BATTLE_ROUND_RESULT                      = 0x0000,
    SMSG_PET_BATTLE_REPLACEMENTS_MADE                 = 0x0000,
    SMSG_PET_BATTLE_REQUEST_FAILED                    = 0x0000,
    SMSG_PET_BATTLE_SLOT_UPDATE                       = 0x0000,
    SMSG_PET_CAST_FAILED                              = 0x0000,
    SMSG_PET_DISMISS_SOUND                            = 0x0000,
    SMSG_PET_GOD_MODE                                 = 0x0000,
    SMSG_PET_GUIDS                                    = 0x0000,
    SMSG_PET_LEARNED_SPELL                            = 0x0000,
    SMSG_PET_MODE                                     = 0x0000,
    SMSG_PET_NAME_INVALID                             = 0x0000,
    SMSG_PET_NAME_QUERY_RESPONSE                      = 0x0000,
    SMSG_PET_REMOVED_SPELL                            = 0x0000,
    SMSG_PET_RENAMEABLE                               = 0x0000,
    SMSG_PET_SLOT_UPDATED                             = 0x0000,
    SMSG_PET_STABLE_LIST                              = 0x0000,
    SMSG_PET_TAME_FAILURE                             = 0x0000,
    SMSG_PLAYED_TIME                                  = 0x0000,
    SMSG_PLAYER_BIND_ERROR                            = 0x0000,
    SMSG_PLAYER_BOUND                                 = 0x0000,
    SMSG_PLAYER_DIFFICULTY_CHANGE                     = 0x0000,
    SMSG_PLAYER_NOT_FOUND_FAILURE                     = 0x0000,
    SMSG_PLAYER_SKINNED                               = 0x0000,
    SMSG_PLAYER_TABAR_VENDOR_ACTIVATE                 = 0x0000,
    SMSG_PLAYER_SAVE_GUILD_EMBLEM                     = 0x0000,
    SMSG_PLAYER_TABAR_VENDOR_SHOW                     = 0x0000,
    SMSG_PLAYER_UNK_DEAD_ALIVE                        = 0x0000,
    SMSG_PLAYER_VEHICLE_DATA                          = 0x0000,
    SMSG_PLAY_DANCE                                   = 0x0000,
    SMSG_PLAY_MUSIC                                   = 0x0000,
    SMSG_PLAY_OBJECT_SOUND                            = 0x0000,
    SMSG_PLAY_ONE_SHOT_ANIM_KIT                       = 0x0000,
    SMSG_PLAY_ORPHAN_SPELL_VISUAL                     = 0x0000,
    SMSG_PLAY_SCENE                                   = 0x0000,
    SMSG_PLAY_SOUND                                   = 0x0000,
    SMSG_PLAY_SPELL_VISUAL                            = 0x0000,
    SMSG_PLAY_SPELL_VISUAL_KIT                        = 0x0000,
    SMSG_PLAY_TIME_WARNING                            = 0x0000,
    SMSG_PRE_RESURRECT                                = 0x0000,
    SMSG_PRINT_NOTIFICATION                           = 0x0000,
    SMSG_PROCRESIST                                   = 0x0000,
    SMSG_PROPOSE_LEVEL_GRANT                          = 0x0000,
    SMSG_PROFILE_DATA_RESPONSE                        = 0x0000,
    SMSG_PUREMOUNT_CANCELLED_OBSOLETE                 = 0x0000,
    SMSG_PVP_CREDIT                                   = 0x0000,
    SMSG_PVP_LOG_DATA                                 = 0x0000,
    SMSG_PVP_OPTIONS_ENABLED                          = 0x0000,
    SMSG_QUERY_OBJECT_POSITION                        = 0x0000,
    SMSG_QUERY_OBJECT_ROTATION                        = 0x0000,
    SMSG_QUERY_TIME_RESPONSE                          = 0x0000,
    SMSG_QUERY_BATTLEPET_NAME_RESPONSE                = 0x0000,
    SMSG_QUERY_NPC_TEXT_RESPONSE                      = 0x0000,
    SMSG_RAID_GROUP_ONLY                              = 0x0000,
    SMSG_RAID_INSTANCE_INFO                           = 0x0000,
    SMSG_RAID_MARKERS_CHANGED                         = 0x0000,
    SMSG_RAID_READY_CHECK_COMPLETED                   = 0x0000,
    SMSG_RAID_READY_CHECK_RESPONSE                    = 0x0000,
    SMSG_RAID_READY_CHECK_STARTED                     = 0x0000,
    SMSG_RAID_READY_CHECK_THROTTLED_ERROR             = 0x0000,
    SMSG_RAID_SUMMON_FAILED                           = 0x0000,
    SMSG_RAID_TARGET_UPDATE_SINGLE                    = 0x0000,
    SMSG_RAID_TARGET_UPDATE_ALL                       = 0x0000,
    SMSG_RANDOMIZE_CHAR_NAME                          = 0x0000,
    SMSG_RANDOM_ROLL                                  = 0x0000,
    SMSG_RATED_BG_RATING                              = 0x0000,
    SMSG_RATED_BG_STATS                               = 0x0000,
    SMSG_READ_ITEM_FAILED                             = 0x0000,
    SMSG_READ_ITEM_OK                                 = 0x0000,
    SMSG_REALM_SPLIT                                  = 0x0000,
    SMSG_REAL_GROUP_UPDATE                            = 0x0000,
    SMSG_RECEIVED_MAIL                                = 0x0000,
    SMSG_REDIRECT_CLIENT                              = 0x0000,
    SMSG_REFER_A_FRIEND_EXPIRED                       = 0x0000,
    SMSG_REFER_A_FRIEND_FAILURE                       = 0x0000,
    SMSG_REFORGE_RESULT                               = 0x0000,
    SMSG_REFRESH_COMPONENT                            = 0x0000,
    SMSG_REMOVE_LOSS_OF_CONTROL                       = 0x0000,
    SMSG_REPORT_PVP_AFK_RESULT                        = 0x0000,
    SMSG_REQUEST_CEMETERY_LIST_RESPONSE               = 0x0000,
    SMSG_REQUEST_PVP_REWARDS_RESPONSE                 = 0x0000,
    SMSG_RESEARCH_COMPLETE                            = 0x0000,
    SMSG_RESEARCH_SETUP_HISTORY                       = 0x0000,
    SMSG_RESET_COMPRESSION_CONTEXT                    = 0x0000,
    SMSG_RESET_FAILED_NOTIFY                          = 0x0000,
    SMSG_RESET_RANGED_COMBAT_TIMER                    = 0x0000,
    SMSG_RESET_WEEKLY_CURRENCY                        = 0x0000,
    SMSG_RESET_DAILY_QUEST                            = 0x0000,
    SMSG_RESPEC_WIPE_CONFIRM                          = 0x0000,
    SMSG_RESPOND_INSPECT_ACHIEVEMENTS                 = 0x0000,
    SMSG_RESURRECT_REQUEST                            = 0x0000,
    SMSG_RESUME_TOKEN                                 = 0x0000,
    SMSG_RESYNC_RUNES                                 = 0x0000,
    SMSG_ROLE_CHANGED_INFORM                          = 0x0000,
    SMSG_ROLL_POLL_INFORM                             = 0x0000,
    SMSG_RWHOIS                                       = 0x0000,
    SMSG_SCENARIO_POI                                 = 0x0000,
    SMSG_SCENARIO_PROGRESS_UPDATE                     = 0x0000,
    SMSG_SCENARIO_STATE                               = 0x0000,
    SMSG_SCENE_OBJECT_EVENT                           = 0x0000,
    SMSG_SCENE_OBJECT_PET_BATTLE_FINISHED             = 0x0000,
    SMSG_SCENE_OBJECT_PET_BATTLE_FIRST_ROUND          = 0x0000,
    SMSG_SCENE_OBJECT_PET_BATTLE_FINAL_ROUND          = 0x0000,
    SMSG_SCENE_OBJECT_PET_BATTLE_FULL_UPDATE          = 0x0000,
    SMSG_SCENE_OBJECT_PET_BATTLE_ROUND_RESULT         = 0x0000,
    SMSG_SCENE_OBJECT_PET_BATTLE_REMPLACEMENTS_MADE   = 0x0000,
    SMSG_SCRIPT_MESSAGE                               = 0x0000,
    SMSG_SEND_ITEM_PASSIVE                            = 0x0000,
    SMSG_SEND_ITEM_DURABILITY                         = 0x0000,
    SMSG_SEND_MAIL_RESULT                             = 0x0000,
    SMSG_SERVER_BUCK_DATA                             = 0x0000,
    SMSG_SERVER_BUCK_DATA_START                       = 0x0000,
    SMSG_SERVER_TIME                                  = 0x0000,
    SMSG_SERVER_INFO_QUERY_RESPONSE                   = 0x0000,
    SMSG_SERVER_INFO_RESPONSE                         = 0x0000,
    SMSG_SERVER_PERF                                  = 0x0000,
    SMSG_SET_ARENA_SEASON                             = 0x0000,
    SMSG_SETUP_RESEARCH_HISTORY                       = 0x0000,
    SMSG_SET_AI_ANIM_KIT                              = 0x0000,
    SMSG_SET_ANIM_TIMER                               = 0x0000,
    SMSG_SET_DF_FAST_LAUNCH_RESULT                    = 0x0000,
    SMSG_SET_DUNGEON_DIFFICULTY                       = 0x0000,
    SMSG_SET_FACTION_ATWAR                            = 0x0000,
    SMSG_SET_FACTION_STANDING                         = 0x0000,
    SMSG_SET_FACTION_VISIBLE                          = 0x0000,
    SMSG_SET_FACTION_NOT_VISIBLE                      = 0x0000,
    SMSG_SET_FORCED_REACTIONS                         = 0x0000,
    SMSG_SET_ITEM_CHALLENGE_MODE_DATA                 = 0x0000,
    SMSG_SET_MELEE_ANIM_KIT                           = 0x0000,
    SMSG_SET_MOVEMENT_ANIM_KIT                        = 0x0000,
    SMSG_SET_PET_SPECIALIZATION                       = 0x0000,
    SMSG_SET_PHASE_SHIFT                              = 0x0000,
    SMSG_SET_PLAYER_DECLINED_NAMES_RESULT             = 0x0000,
    SMSG_SET_PLAY_HOVER_ANIM                          = 0x0000,
    SMSG_SET_PROJECTILE_POSITION                      = 0x0000,
    SMSG_SET_PROMOTION_RESPONSE                       = 0x0000,
    SMSG_SET_SERVER_WOW_TIME                          = 0x0000,
    SMSG_SHOW_NEUTRAL_PLAYER_FACTION_SELECT_UI        = 0x0000,
    SMSG_SHOW_RATINGS                                 = 0x0000,
    SMSG_SHOW_ZONES_CHEAT_RESULT                      = 0x0000,
    SMSG_SOCKET_GEMS                                  = 0x0000,
    SMSG_SOR_START_EXPERIENCE_INCOMPLETE              = 0x0000,
    SMSG_SPIRIT_HEALER_CONFIRM                        = 0x0000,
    SMSG_SPLINE_MOVE_COLLISION_DISABLE                = 0x0000,
    SMSG_SPLINE_MOVE_COLLISION_ENABLE                 = 0x0000,
    SMSG_SPLINE_MOVE_GRAVITY_DISABLE                  = 0x0000,
    SMSG_SPLINE_MOVE_GRAVITY_ENABLE                   = 0x0000,
    SMSG_SPLINE_MOVE_ROOT                             = 0x0000,
    SMSG_SPLINE_MOVE_SET_FEATHER_FALL                 = 0x0000,
    SMSG_SPLINE_MOVE_SET_FLIGHT_BACK_SPEED            = 0x0000,
    SMSG_SPLINE_MOVE_SET_FLIGHT_SPEED                 = 0x0000,
    SMSG_SPLINE_MOVE_SET_FLYING                       = 0x0000,
    SMSG_SPLINE_MOVE_SET_HOVER                        = 0x0000,
    SMSG_SPLINE_MOVE_SET_LAND_WALK                    = 0x0000,
    SMSG_SPLINE_MOVE_SET_NORMAL_FALL                  = 0x0000,
    SMSG_SPLINE_MOVE_SET_PITCH_RATE                   = 0x0000,
    SMSG_SPLINE_MOVE_SET_RUN_BACK_SPEED               = 0x0000,
    SMSG_SPLINE_MOVE_SET_RUN_MODE                     = 0x0000,
    SMSG_SPLINE_MOVE_SET_RUN_SPEED                    = 0x0000,
    SMSG_SPLINE_MOVE_SET_SWIM_BACK_SPEED              = 0x0000,
    SMSG_SPLINE_MOVE_SET_SWIM_SPEED                   = 0x0000,
    SMSG_SPLINE_MOVE_SET_TURN_RATE                    = 0x0000,
    SMSG_SPLINE_MOVE_SET_WALK_MODE                    = 0x0000,
    SMSG_SPLINE_MOVE_SET_WALK_SPEED                   = 0x0000,
    SMSG_SPLINE_MOVE_SET_WATER_WALK                   = 0x0000,
    SMSG_SPLINE_MOVE_START_SWIM                       = 0x0000,
    SMSG_SPLINE_MOVE_STOP_SWIM                        = 0x0000,
    SMSG_SPLINE_MOVE_UNROOT                           = 0x0000,
    SMSG_SPLINE_MOVE_UNSET_FLYING                     = 0x0000,
    SMSG_SPLINE_MOVE_UNSET_HOVER                      = 0x0000,
    SMSG_STABLE_RESULT                                = 0x0000,
    SMSG_STANDSTATE_UPDATE                            = 0x0000,
    SMSG_START_ELAPSED_TIMER                          = 0x0000,
    SMSG_START_ELAPSED_TIMERS                         = 0x0000,
    SMSG_START_MIRROR_TIMER                           = 0x0000,
    SMSG_START_TIMER                                  = 0x0000,
    SMSG_STOP_DANCE                                   = 0x0000,
    SMSG_STOP_MIRROR_TIMER                            = 0x0000,
    SMSG_STOP_ELAPSED_TIMER                           = 0x0000,
    SMSG_STREAMING_MOVIE                              = 0x0000,
    SMSG_SUMMON_CANCEL                                = 0x0000,
    SMSG_SUMMON_REQUEST                               = 0x0000,
    SMSG_SUPPRESS_NPC_GREETINGS                       = 0x0000,
    SMSG_SUSPEND_COMMS                                = 0x0000,
    SMSG_SUSPEND_TOKEN_RESPONSE                       = 0x0000,
    SMSG_TALENTS_INVOLUNTARILY_RESET                  = 0x0000,
    SMSG_TEST_DROP_RATE_RESULT                        = 0x0000,
    SMSG_TEXT_EMOTE                                   = 0x0000,
    SMSG_THREAT_CLEAR                                 = 0x0000,
    SMSG_THREAT_REMOVE                                = 0x0000,
    SMSG_THREAT_UPDATE                                = 0x0000,
    SMSG_TIME_ADJUSTMENT                              = 0x0000,
    SMSG_TIME_SYNC_REQUEST                            = 0x0000,
    SMSG_TITLE_EARNED                                 = 0x0000,
    SMSG_TITLE_LOST                                   = 0x0000,
    SMSG_TOGGLE_XP_GAIN                               = 0x0000,
    SMSG_TOTEM_CREATED                                = 0x0000,
    SMSG_TRADE_INFO                                   = 0x0000,
    SMSG_TRADE_STATUS                                 = 0x0000,
    SMSG_TRADE_UPDATED                                = 0x0000,
    SMSG_TRAINER_LIST                                 = 0x0000,
    SMSG_TRAINER_SERVICE                              = 0x0000,
    SMSG_TRIGGER_CINEMATIC                            = 0x0000,
    SMSG_TRIGGER_MOVIE                                = 0x0000,
    SMSG_TURN_IN_PETITION_RESULTS                     = 0x0000,
    SMSG_UI_TIME                                      = 0x0000,
    SMSG_UNAPPLY_MOVEMENT_FORCE                       = 0x0000,
    SMSG_UNIT_HEALTH_FREQUENT                         = 0x0000,
    SMSG_UNIT_SPELLCAST_START                         = 0x0000,
    SMSG_UPDATE_COMBO_POINTS                          = 0x0000,
    SMSG_UPDATE_CURRENCY                              = 0x0000,
    SMSG_UPDATE_CURRENCY_WEEK_LIMIT                   = 0x0000,
    SMSG_UPDATE_DUNGEON_ENCOUNTER_FOR_LOOT            = 0x0000,
    SMSG_UPDATE_EXPANSION_LEVEL                       = 0x0000,
    SMSG_UPDATE_INSTANCE_ENCOUNTER_UNIT               = 0x0000,
    SMSG_UPDATE_INSTANCE_OWNERSHIP                    = 0x0000,
    SMSG_UPDATE_ITEM_ENCHANTMENTS                     = 0x0000,
    SMSG_UPDATE_LAST_INSTANCE                         = 0x0000,
    SMSG_UPDATE_SERVER_PLAYER_POSITION                = 0x0000,
    SMSG_VOICE_SESSION_FULL                           = 0x0000,
    SMSG_VOICE_SET_TALKER_MUTED                       = 0x0000,
    SMSG_VOID_ITEM_SWAP_RESPONSE                      = 0x0000,
    SMSG_VOID_STORAGE_CONTENTS                        = 0x0000,
    SMSG_VOID_STORAGE_FAILED                          = 0x0000,
    SMSG_VOID_STORAGE_TRANSFER_CHANGES                = 0x0000,
    SMSG_VOID_TRANSFER_RESULT                         = 0x0000,
    SMSG_WAIT_QUEUE_FINISH                            = 0x0000,
    SMSG_WAIT_QUEUE_UPDATE                            = 0x0000,
    SMSG_WARDEN_DATA                                  = 0x0000,
    SMSG_WARGAME_CHECK_ENTRY                          = 0x0000,
    SMSG_WARGAME_REQUEST_SENT                         = 0x0000,
    SMSG_WARGAME_REQUEST_SUCCESSFULLY_SENT_TO_OPPENENT= 0x0000,
    SMSG_WEATHER                                      = 0x0000,
    SMSG_WEEKLY_LAST_RESET                            = 0x0000,
    SMSG_WEEKLY_RESET_CURRENCY                        = 0x0000,
    SMSG_WHOIS                                        = 0x0000,
    SMSG_WORLD_STATE_UI_TIMER_UPDATE                  = 0x0000,
    SMSG_XP_GAIN_ABORTED                              = 0x0000,
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
        bool foundet = false;

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
