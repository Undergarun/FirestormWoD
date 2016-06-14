////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __TRINITY_ACHIEVEMENTMGR_H
#define __TRINITY_ACHIEVEMENTMGR_H

#include "Common.h"
#include "DatabaseEnv.h"
#include "DBCEnums.h"
#include "DBCStores.h"
#include "MapUpdater.h"

typedef std::vector<CriteriaEntry const*>            AchievementCriteriaEntryList;
typedef std::vector<AchievementEntry const*>         AchievementEntryList;
typedef std::vector<CriteriaTreeEntry const*>        AchievementCriteriaTreeList;
typedef std::vector<ModifierTreeEntry const*>        ModifierTreeEntryList;

typedef std::unordered_map<uint32, AchievementEntryList>  AchievementListByReferencedId;
typedef std::vector<AchievementCriteriaTreeList>     AchievementCriteriaTreeByCriteriaId;
typedef std::vector<AchievementEntry const*>         AchievementEntryByCriteriaTree;
typedef std::vector<ModifierTreeEntryList>           ModifierTreeEntryByTreeId;
typedef std::vector<AchievementCriteriaTreeList>     SubCriteriaTreeListById;
typedef std::vector<AchievementEntry const*>         AchievementEntryByCriteriaTreeId;


struct CriteriaProgress
{
    uint32 counter;
    time_t date;                                            // latest update time.
    uint64 CompletedGUID;                                   // GUID of the player that completed this criteria (guild achievements)
    bool changed;
};

enum AchievementCriteriaDataType
{                                                           // value1         value2        comment
    ACHIEVEMENT_CRITERIA_DATA_TYPE_NONE                = 0, // 0              0
    ACHIEVEMENT_CRITERIA_DATA_TYPE_T_CREATURE          = 1, // creature_id    0
    ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_CLASS_RACE = 2, // class_id       race_id
    ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_LESS_HEALTH= 3, // health_percent 0
    ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA              = 5, // spell_id       effect_idx
    ACHIEVEMENT_CRITERIA_DATA_TYPE_T_AURA              = 7, // spell_id       effect_idx
    ACHIEVEMENT_CRITERIA_DATA_TYPE_VALUE               = 8, // minvalue                     value provided with achievement update must be not less that limit
    ACHIEVEMENT_CRITERIA_DATA_TYPE_T_LEVEL             = 9, // minlevel                     minlevel of target
    ACHIEVEMENT_CRITERIA_DATA_TYPE_T_GENDER            = 10, // gender                       0=male; 1=female
    ACHIEVEMENT_CRITERIA_DATA_TYPE_SCRIPT              = 11, // scripted requirement
    // REUSE
    ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_PLAYER_COUNT    = 13, // count                        "with less than %u people in the zone"
    ACHIEVEMENT_CRITERIA_DATA_TYPE_T_TEAM              = 14, // team                         HORDE(67), ALLIANCE(469)
    ACHIEVEMENT_CRITERIA_DATA_TYPE_S_DRUNK             = 15, // drunken_state  0             (enum DrunkenState) of player
    ACHIEVEMENT_CRITERIA_DATA_TYPE_HOLIDAY             = 16, // holiday_id     0             event in holiday time
    ACHIEVEMENT_CRITERIA_DATA_TYPE_BG_LOSS_TEAM_SCORE  = 17, // min_score      max_score     player's team win bg and opposition team have team score in range
    ACHIEVEMENT_CRITERIA_DATA_INSTANCE_SCRIPT          = 18, // 0              0             maker instance script call for check current criteria requirements fit
    ACHIEVEMENT_CRITERIA_DATA_TYPE_S_EQUIPED_ITEM      = 19, // item_level     item_quality  for equipped item in slot to check item level and quality
    ACHIEVEMENT_CRITERIA_DATA_TYPE_S_PLAYER_CLASS_RACE = 21  // class_id       race_id
};

enum AchievementFaction
{
    ACHIEVEMENT_FACTION_HORDE           = 0,
    ACHIEVEMENT_FACTION_ALLIANCE        = 1,
    ACHIEVEMENT_FACTION_ANY             = -1
};

enum AchievementFlags
{
    ACHIEVEMENT_FLAG_COUNTER                = 0x00000001,    // Just count statistic (never stop and complete)
    ACHIEVEMENT_FLAG_HIDDEN                 = 0x00000002,    // Not sent to client - internal use only
    ACHIEVEMENT_FLAG_PLAY_NO_VISUAL         = 0x00000004,    // Client does not play achievement earned visual
    ACHIEVEMENT_FLAG_SUMM                   = 0x00000008,    // Use sum criteria value from all requirements (and calculate max value)
    ACHIEVEMENT_FLAG_MAX_USED               = 0x00000010,    // Show max criteria (and calculate max value ??)
    ACHIEVEMENT_FLAG_REQ_COUNT              = 0x00000020,    // Use not zero req count (and calculate max value)
    ACHIEVEMENT_FLAG_AVERAGE                = 0x00000040,    // Show as average value (value / time_in_days) depend from other flag (by def use last criteria value)
    ACHIEVEMENT_FLAG_BAR                    = 0x00000080,    // Show as progress bar (value / max vale) depend from other flag (by def use last criteria value)
    ACHIEVEMENT_FLAG_REALM_FIRST_REACH      = 0x00000100,    //
    ACHIEVEMENT_FLAG_REALM_FIRST_KILL       = 0x00000200,    //
    ACHIEVEMENT_FLAG_UNK3                   = 0x00000400,    // ACHIEVEMENT_FLAG_HIDE_NAME_IN_TIE
    ACHIEVEMENT_FLAG_REALM_FIRST_GUILD      = 0x00000800,    // first guild on realm done something
    ACHIEVEMENT_FLAG_SHOW_IN_GUILD_NEWS     = 0x00001000,    // Shows in guild news
    ACHIEVEMENT_FLAG_SHOW_IN_GUILD_HEADER   = 0x00002000,    // Shows in guild news header
    ACHIEVEMENT_FLAG_GUILD                  = 0x00004000,    //
    ACHIEVEMENT_FLAG_SHOW_GUILD_MEMBERS     = 0x00008000,    //
    ACHIEVEMENT_FLAG_SHOW_CRITERIA_MEMBERS  = 0x00010000,    //
    ACHIEVEMENT_FLAG_ACCOUNT                = 0x00020000     // achievement linked to account
};

enum AchievementCriteriaLimits
{
    MAX_CRITERIA_REQUIREMENTS          = 2,
    MAX_ADDITIONAL_CRITERIA_CONDITIONS = 3
};

enum AchievementCriteriaCondition
{
    ACHIEVEMENT_CRITERIA_CONDITION_NONE            = 0,
    ACHIEVEMENT_CRITERIA_CONDITION_NO_DEATH        = 1,    // reset progress on death
    ACHIEVEMENT_CRITERIA_CONDITION_UNK1            = 2,    // only used in "Complete a daily quest every day for five consecutive days"
    ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP          = 3,    // requires you to be on specific map, reset at change
    ACHIEVEMENT_CRITERIA_CONDITION_NO_LOSE         = 4,    // only used in "Win 10 arenas without losing"
    ACHIEVEMENT_CRITERIA_CONDITION_NO_SPELL_HIT    = 9,    // requires the player not to be hit by specific spell
    ACHIEVEMENT_CRITERIA_CONDITION_NOT_IN_GROUP    = 10,   // requires the player not to be in group
    ACHIEVEMENT_CRITERIA_CONDITION_UNK13           = 13    // unk
};

enum AchievementCriteriaAdditionalCondition
{
    CRITERIA_CONDITION_SOURCE_DRUNK_VALUE                   = 1,    // 19116
    CRITERIA_CONDITION_UNK2                                 = 2,    // 19116 - NYI - See: http://www.wowhead.com/achievement=5869
    CRITERIA_CONDITION_ITEM_LEVEL                           = 3,    // 19116
    CRITERIA_CONDITION_TARGET_CREATURE_ENTRY                = 4,    // 19116
    CRITERIA_CONDITION_TARGET_MUST_BE_PLAYER                = 5,    // 19116
    CRITERIA_CONDITION_TARGET_MUST_BE_DEAD                  = 6,    // 19116
    CRITERIA_CONDITION_TARGET_MUST_BE_ENEMY                 = 7,    // 19116
    CRITERIA_CONDITION_SOURCE_HAS_AURA                      = 8,    // 19116
    CRITERIA_CONDITION_TARGET_HAS_AURA                      = 10,   // 19116
    CRITERIA_CONDITION_TARGET_HAS_AURA_TYPE                 = 11,   // 19116
    CRITERIA_CONDITION_UNK12                                = 12,   // 19116 - Required Value : 14
    CRITERIA_CONDITION_ITEM_QUALITY_MIN                     = 14,   // 19116
    CRITERIA_CONDITION_ITEM_QUALITY_EQUALS                  = 15,   // 19116
    CRITERIA_CONDITION_UNK16                                = 16,   // 19116 - NYI - See: http://www.wowhead.com/achievement=1260
    CRITERIA_CONDITION_SOURCE_AREA_OR_ZONE                  = 17,   // 19116
    CRITERIA_CONDITION_TARGET_AREA_OR_ZONE                  = 18,   // 19116
    CRITERIA_CONDITION_LEGACY_RAID_TYPE                     = 20,   // 19116
    CRITERIA_CONDITION_TARGET_CREATURE_YIELDS_XP            = 21,   // 19116
    CRITERIA_CONDITION_ARENA_TYPE                           = 24,   // 19116
    CRITERIA_CONDITION_SOURCE_RACE                          = 25,   // 19116
    CRITERIA_CONDITION_SOURCE_CLASS                         = 26,   // 19116
    CRITERIA_CONDITION_TARGET_RACE                          = 27,   // 19116
    CRITERIA_CONDITION_TARGET_CLASS                         = 28,   // 19116
    CRITERIA_CONDITION_MAX_GROUP_MEMBERS                    = 29,   // 19116
    CRITERIA_CONDITION_TARGET_CREATURE_TYPE                 = 30,   // 19116
    CRITERIA_CONDITION_SOURCE_MAP                           = 32,   // 19116
    CRITERIA_CONDITION_BUILD_VERSION                        = 33,   // 19116
    CRITERIA_CONDITION_BATTLEPET_TEAM_LEVEL                 = 34,   // 19116
    CRITERIA_CONDITION_COMPLETE_QUEST_NOT_IN_GROUP          = 35,   // 19116
    CRITERIA_CONDITION_MIN_PERSONAL_RATING                  = 37,   // 19116
    CRITERIA_CONDITION_TITLE_BIT_INDEX                      = 38,   // 19116
    CRITERIA_CONDITION_SOURCE_LEVEL                         = 39,   // 19116
    CRITERIA_CONDITION_TARGET_LEVEL                         = 40,   // 19116
    CRITERIA_CONDITION_TARGET_ZONE                          = 41,   // 19116
    CRITERIA_CONDITION_UNK43                                = 43,   // 19116 - Not used
    CRITERIA_CONDITION_TARGET_HEALTH_PERCENT_BELOW          = 46,   // 19116
    CRITERIA_CONDITION_UNK55                                = 55,   // 19116 - NYI - See: http://www.wowhead.com/achievement=2422
    CRITERIA_CONDITION_MIN_ACHIEVEMENT_POINTS               = 56,   // 19116
    CRITERIA_CONDITION_REQUIRES_LFG_GROUP                   = 58,   // 19116
    CRITERIA_CONDITION_BE_THE_LAST_SURVIVOR_5V5             = 60,   // 19116
    CRITERIA_CONDITION_REQUIRES_GUILD_GROUP                 = 61,   // 19116
    CRITERIA_CONDITION_GUILD_REPUTATION                     = 62,   // 19116
    CRITERIA_CONDITION_RATED_BATTLEGROUND                   = 63,   // 19116
    CRITERIA_CONDITION_PROJECT_RARITY                       = 65,   // 19116
    CRITERIA_CONDITION_PROJECT_RACE                         = 66,   // 19116
    CRITERIA_CONDITION_WORLD_STATE_EXPRESSION               = 67,   // 19116
    CRITERIA_CONDITION_DIFFICULTY                           = 68,   // 19116
    CRITERIA_CONDITION_UNK69                                = 69,   // 19116 - Not used
    CRITERIA_CONDITION_TARGET_MIN_LEVEL                     = 70,   // 19116
    CRITERIA_CONDITION_UNK73                                = 73,   // 19116 - NYI - See: http://www.wowhead.com/achievement=6683 - May need hard code
    CRITERIA_CONDITION_BATTLEPET_TYPE                       = 78,   // 19116
    CRITERIA_CONDITION_CAPTURE_BATTLEPET_ABOVE_HEALT_PCT    = 79,   // 19116
    CRITERIA_CONDITION_COUNT_OF_GUILD_MEMBER_IN_GROUP       = 80,   // 19116
    CRITERIA_CONDITION_TARGET_IS_BATTLEPET_MASTER           = 81,   // 19116 - NYI
    CRITERIA_CONDITION_UNK82                                = 82,   // 19116 - Not used
    CRITERIA_CONDITION_NEED_CHALLENGE_MEDAL                 = 83,   // 19116
    CRITERIA_CONDITION_UNK84                                = 84,   // 19116 - Not used
    CRITERIA_CONDITION_UNK86                                = 86,   // 19116 - Not used
    CRITERIA_CONDITION_UNK87                                = 87,   // 19116 - Not used
    CRITERIA_CONDITION_UNK88                                = 88,   // 19116 - Not used
    CRITERIA_CONDITION_BATTLEPET_MUST_BE_RARE               = 89,   // 19116
    CRITERIA_CONDITION_WIN_PVP_PETBATTLE                    = 90,   // 19116 - NYI
    CRITERIA_CONDITION_CAPTURE_PETBATTLE                    = 91,   // 19116
    CRITERIA_CONDITION_UNK92                                = 92,   // 19116 - Not used
    CRITERIA_CONDITION_UNK93                                = 93,   // 19116 - Not used
    CRITERIA_CONDITION_UNK94                                = 94,   // 19116 - Not used
    CRITERIA_CONDITION_UNK95                                = 95,   // 19116 - Not used
    CRITERIA_CONDITION_CRAFT_AMOUNT_OF_ITEM                 = 96,   // 19116
    CRITERIA_CONDITION_UNK97                                = 97,   // 19116 - Not used
    CRITERIA_CONDITION_UNK99                                = 99,   // 19116 - Not used
    CRITERIA_CONDITION_UNK112                               = 112,  // 19116 - Not used
    CRITERIA_CONDITION_EARN_CURRENCY_DURING_ARENA_SEASON    = 121,  // 19116
    CRITERIA_CONDITION_REQUIRE_DEATH_IN_DUNGEON_OR_RAID     = 122,  // 19116
    CRITERIA_CONDITION_REACH_ARENA_RATING_DURING_SEASON     = 125,  // 17399 - NYI
    CRITERIA_CONDITION_UNK126                               = 126,  // 19116 - NYI
    CRITERIA_CONDITION_UNK127                               = 127,  // 19116 - NYI
    CRITERIA_CONDITION_UNK128                               = 128,  // 19116 - NYI
    CRITERIA_CONDITION_UNK132                               = 132,  // 19116 - NYI
    CRITERIA_CONDITION_UNK134                               = 134,  // 19116 - NYI
    CRITERIA_CONDITION_UNK135                               = 135,  // 19116 - NYI
    CRITERIA_CONDITION_UNK138                               = 138,  // 19116 - NYI
    CRITERIA_CONDITION_UNK139                               = 139,  // 19116 - NYI
    CRITERIA_CONDITION_UNK140                               = 140,  // 19116 - NYI
    CRITERIA_CONDITION_UNK141                               = 141,  // 19116 - NYI
    CRITERIA_CONDITION_UNK142                               = 142,  // 19116 - NYI
    CRITERIA_CONDITION_FOLLOWER_QUALITY                     = 145,  // 19116 - NYI
    CRITERIA_CONDITION_FOLLOWER_LEVEL                       = 146,  // 19116 - NYI
    CRITERIA_CONDITION_UNK147                               = 147,  // 19116 - NYI
    CRITERIA_CONDITION_UNK148                               = 148,  // 19116 - NYI
    CRITERIA_CONDITION_BUILDING_LEVEL                       = 149,  // 19116 - NYI
    CRITERIA_CONDITION_UNK150                               = 150,  // 19116 - NYI
    CRITERIA_CONDITION_UNK151                               = 151,  // 19116 - NYI
    CRITERIA_CONDITION_UNK152                               = 152,  // 19116 - NYI
    CRITERIA_CONDITION_UNK153                               = 153,  // 19116 - NYI
    CRITERIA_CONDITION_UNK154                               = 154,  // 19116 - NYI
    CRITERIA_CONDITION_UNK155                               = 155,  // 19116 - NYI
    CRITERIA_CONDITION_UNK156                               = 156,  // 19116 - NYI
    CRITERIA_CONDITION_UNK157                               = 157,  // 19116 - NYI
    CRITERIA_CONDITION_UNK158                               = 158,  // 19116 - NYI
    CRITERIA_CONDITION_UNK159                               = 159,  // 19116 - NYI
    CRITERIA_CONDITION_UNK167                               = 167,  // 19116 - NYI
    CRITERIA_CONDITION_UNK168                               = 168,  // 19116 - NYI
    CRITERIA_CONDITION_FOLLOWER_ILEVEL                      = 169,  // 19116 - NYI
    CRITERIA_CONDITION_UNK170                               = 170,  // 19116 - NYI
    CRITERIA_CONDITION_UNK171                               = 171,  // 19116 - NYI
    CRITERIA_CONDITION_UNK172                               = 172,  // 19116 - NYI
    CRITERIA_CONDITION_UNK173                               = 173,  // 19116 - NYI
    CRITERIA_CONDITION_UNK174                               = 174,  // 19116 - NYI
    CRITERIA_CONDITION_UNK175                               = 175,  // 19116 - NYI
    CRITERIA_CONDITION_UNK176                               = 176,  // 19116 - NYI
    CRITERIA_CONDITION_UNK178                               = 178,  // 19116 - NYI
    CRITERIA_CONDITION_UNK179                               = 179,  // 19116 - NYI
    CRITERIA_CONDITION_UNK180                               = 180,  // 19116 - NYI
    CRITERIA_CONDITION_UNK182                               = 182   // 19116 - NYI
};

enum AchievementCriteriaFlags
{
    CRITERIA_FLAG_PROGRESS_BAR          = 0x00000001,   // Show progress as bar
    CRITERIA_FLAG_HIDDEN                = 0x00000002,   // Not show criteria in client
    CRITERIA_FLAG_FAIL_ACHIEVEMENT      = 0x00000004,   //
    CRITERIA_FLAG_RESET_ON_START        = 0x00000008,   //
    CRITERIA_FLAG_IS_DATE               = 0x00000010,   // not used
    CRITERIA_FLAG_IS_MONEY              = 0x00000020,   // Displays counter as money
    CRITERIA_FLAG_IS_ACHIEVEMENT_ID     = 0x00000040,   //
    CRITERIA_FLAG_QUANTITY_IS_CAPPED    = 0x00000080    //
};

enum AchievementCriteriaTimedTypes
{
    ACHIEVEMENT_TIMED_TYPE_SPELL_CASTER = 7,    // Timer is started by casting a spell with entry in timerStartEvent
    ACHIEVEMENT_TIMED_TYPE_SPELL_TARGET = 8,    // Timer is started by being target of spell with entry in timerStartEvent
    ACHIEVEMENT_TIMED_TYPE_QUEST        = 9,    // Timer is started by accepting quest with entry in timerStartEvent
    ACHIEVEMENT_TIMED_TYPE_CREATURE     = 10,   // Timer is started by killing creature with entry in timerStartEvent
    ACHIEVEMENT_TIMED_TYPE_ITEM         = 12,   // Timer is started by using item with entry in timerStartEvent
    ACHIEVEMENT_TIMED_TYPE_EVENT        = 13,   // Timer is started by internal event with id in timerStartEvent
    ACHIEVEMENT_TIMED_TYPE_UNK          = 14,   // Unknown

    ACHIEVEMENT_TIMED_TYPE_MAX
};

enum AchievementCriteriaTypes
{
    ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE                     = 0,    // 19116
    ACHIEVEMENT_CRITERIA_TYPE_WIN_BG                            = 1,    // 19116
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ARCHAEOLOGY_PROJECTS     = 3,    // 19116
    ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL                       = 5,    // 19116
    ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL                 = 7,    // 19116
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT              = 8,    // 19116
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT              = 9,    // 19116
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY        = 10,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE           = 11,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_CURRENCY                          = 12,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_DAMAGE_DONE                       = 13,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST              = 14,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND             = 15,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP                      = 16,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_DEATH                             = 17,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON                  = 18,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID                     = 19,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE                = 20,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_HIGH_SCORE_IN_ORDALIE             = 21,   // 19116 - Only three - Statistics
    ACHIEVEMENT_CRITERIA_TYPE_MOST_CHALLENGE_DUNGEON_WON        = 22,   // 19116 - NYI - Statistics
    ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_PLAYER                  = 23,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING                = 24,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM                       = 26,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST                    = 27,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET                   = 28,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL                        = 29,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE              = 30,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA            = 31,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA                         = 32,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA                        = 33,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL                       = 34,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL                    = 35,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM                          = 36,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA                   = 37,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING               = 38,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_PERSONAL_RATING           = 39,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL                 = 40,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM                          = 41,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM                         = 42,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA                      = 43,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK                          = 44,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT                     = 45,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION                   = 46,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION           = 47,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP                 = 48,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM                   = 49,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT                 = 50,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT                = 51,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS                          = 52,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_HK_RACE                           = 53,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE                          = 54,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE                      = 55,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS                 = 56,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM                        = 57,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_VENDORS                = 59,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TALENTS            = 60,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_TALENT_RESETS           = 61,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_QUEST_REWARD           = 62,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TRAVELLING         = 63,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_UNK64                             = 64,   // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_AT_BARBER              = 65,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_MAIL               = 66,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY                        = 67,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT                    = 68,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2                  = 69,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL                  = 70,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_WIN_CHALLENGE_DUNGEON             = 71,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT                = 72,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_EARNED_PVP_TITLE                  = 74,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS            = 75,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL                          = 76,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_LOSE_DUEL                         = 77,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE                = 78,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_COOK_SOME_MEALS                   = 79,   // 19116 - NYI
    ACHIEVEMENT_CRITERIA_TYPE_GOLD_EARNED_BY_AUCTIONS           = 80,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_ACHIEVEMENTS_IN_BATTLE_PET        = 81,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_CREATE_AUCTION                    = 82,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_BID               = 83,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_WON_AUCTIONS                      = 84,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_SOLD              = 85,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_GOLD_VALUE_OWNED          = 86,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_GAIN_REVERED_REPUTATION           = 87,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_GAIN_HONORED_REPUTATION           = 88,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_KNOWN_FACTIONS                    = 89,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_LOOT_EPIC_ITEM                    = 90,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_RECEIVE_EPIC_ITEM                 = 91,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_UNK92                             = 92,   // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED                         = 93,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED                        = 94,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_UNK95                             = 95,   // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLEPET                 = 96,   // 19116
    ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_DEALT                 = 101,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_RECEIVED              = 102,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_TOTAL_DAMAGE_RECEIVED             = 103,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEAL_CASTED               = 104,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_TOTAL_HEALING_RECEIVED            = 105,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEALING_RECEIVED          = 106,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_QUEST_ABANDONED                   = 107,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_FLIGHT_PATHS_TAKEN                = 108,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE                         = 109,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2                       = 110,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE                  = 112,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL               = 113,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_ACCEPTED_SUMMONINGS               = 114,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_EARN_ACHIEVEMENT_POINTS           = 115,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_UNK118                            = 118,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_USE_LFD_TO_GROUP_WITH_PLAYERS     = 119,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_UNK120                            = 120,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_UNK121                            = 121,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_UNK122                            = 122,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_UNK123                            = 123,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_SPENT_GOLD_GUILD_REPAIRS          = 124,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_REACH_GUILD_LEVEL                 = 125,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_CRAFT_ITEMS_GUILD                 = 126,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_CATCH_FROM_POOL                   = 127,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_BUY_GUILD_BANK_SLOTS              = 128,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_EARN_GUILD_ACHIEVEMENT_POINTS     = 129,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_BATTLEGROUND            = 130,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_REACH_BG_RATING                   = 132,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_BUY_GUILD_TABARD                  = 133,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_GUILD             = 134,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILLS_GUILD             = 135,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE_GUILD          = 136,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_UNK137                            = 137,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GUILD_CHALLENGE_TYPE     = 138,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GUILD_CHALLENGE          = 139,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_UNK140                            = 140,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_UNK141                            = 141,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_UNK142                            = 142,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_UNK143                            = 143,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_UNK144                            = 144,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_UNK145                            = 145,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_UNK146                            = 146,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_UNK147                            = 147,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_UNK148                            = 148,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_UNK149                            = 149,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_UNK150                            = 150,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_SCENARIOS_COMPLETED               = 151,  // 19116 - NYI
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_SCENARIO_ID              = 152,  // 19116 - NYI
    ACHIEVEMENT_CRITERIA_TYPE_UNK153                            = 153,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_SPECIFIC_BATTLEPET        = 155,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_EARN_BATTLEPET                    = 156,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLEPET_IN_COMBAT       = 157,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_WIN_PETBATTLE                     = 158,  // 19116 - NYI
    ACHIEVEMENT_CRITERIA_TYPE_UNK159                            = 159,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_LEVELUP_BATTLEPET                 = 160,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLEPET_IN_ZONE         = 161,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_UNK162                            = 162,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_PLACE_WORK_ORDER                  = 163,  // 19116 NYI
    ACHIEVEMENT_CRITERIA_TYPE_UNK164                            = 164,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_DEFEAT_ENCOUNTER                  = 165,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_UNK167                            = 167,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_UPDATE_BUILDING_LEVEL             = 168,  // 19116 NYI
    ACHIEVEMENT_CRITERIA_TYPE_BUILD_PRESET_BUILDING             = 169,  // 19116 NYI
    ACHIEVEMENT_CRITERIA_TYPE_UPDATE_GARRISON_LEVEL             = 170,  // 19116 NYI
    ACHIEVEMENT_CRITERIA_TYPE_UNK171                            = 171,  // 19116 - Not used
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GARRISON_MISSIONS        = 173,  // 19116 NYI
    ACHIEVEMENT_CRITERIA_TYPE_RECRUIT_FOLLOWER_IN_OWN_GARRISON  = 175,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_LEARN_GARRISON_BLUEPRINTS         = 178,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_WORK_ORDERS              = 182,  // 19116 NYI
    ACHIEVEMENT_CRITERIA_TYPE_RAISE_FOLLOWER_ILEVEL             = 183,  // 19116 NYI
    ACHIEVEMENT_CRITERIA_TYPE_LEVELUP_FOLLOWERS                 = 184,  // 19116
    ACHIEVEMENT_CRITERIA_TYPE_COLLECT_TOYS                      = 186,  // 19116 NYI
    ACHIEVEMENT_CRITERIA_TYPE_RECRUIT_FOLLOWER_OF_QUALITY       = 187,  // 19116 NYI
    // 188 unused
    ACHIEVEMENT_CRITERIA_TYPE_COLLECT_HEIRLOOMS                 = 189,
    // 0..162 => 163 criteria types total
    ACHIEVEMENT_CRITERIA_TYPE_TOTAL                             = 190
};

#define MAX_ACHIEVEMENT_CRITERIA_DATA_TYPE               22 // maximum value in AchievementCriteriaDataType enum

class Player;
class Guild;
class Unit;
class WorldPacket;

struct AchievementCriteriaData
{
    AchievementCriteriaDataType dataType;
    union
    {
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_NONE              = 0 (no data)
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_CREATURE        = 1
        struct
        {
            uint32 id;
        } creature;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_CLASS_RACE = 2
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_S_PLAYER_CLASS_RACE = 21
        struct
        {
            uint32 class_id;
            uint32 race_id;
        } classRace;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_LESS_HEALTH = 3
        struct
        {
            uint32 percent;
        } health;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA            = 5
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_AURA            = 7
        struct
        {
            uint32 spell_id;
            uint32 effect_idx;
        } aura;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_VALUE             = 8
        struct
        {
            uint32 minvalue;
        } value;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_LEVEL           = 9
        struct
        {
            uint32 minlevel;
        } level;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_GENDER          = 10
        struct
        {
            uint32 gender;
        } gender;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_SCRIPT            = 11 (no data)
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_PLAYER_COUNT  = 13
        struct
        {
            uint32 maxcount;
        } map_players;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_T_TEAM            = 14
        struct
        {
            uint32 team;
        } team;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_S_DRUNK           = 15
        struct
        {
            uint32 state;
        } drunk;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_HOLIDAY           = 16
        struct
        {
            uint32 id;
        } holiday;
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_BG_LOSS_TEAM_SCORE= 17
        struct
        {
            uint32 min_score;
            uint32 max_score;
        } bg_loss_team_score;
        // ACHIEVEMENT_CRITERIA_DATA_INSTANCE_SCRIPT        = 18 (no data)
        // ACHIEVEMENT_CRITERIA_DATA_TYPE_S_EQUIPED_ITEM    = 19
        struct
        {
            uint32 item_level;
            uint32 item_quality;
        } equipped_item;
        // raw
        struct
        {
            uint32 value1;
            uint32 value2;
        } raw;
    };
    uint32 ScriptId;

    AchievementCriteriaData() : dataType(ACHIEVEMENT_CRITERIA_DATA_TYPE_NONE)
    {
        raw.value1 = 0;
        raw.value2 = 0;
        ScriptId = 0;
    }

    AchievementCriteriaData(uint32 _dataType, uint32 _value1, uint32 _value2, uint32 _scriptId) : dataType(AchievementCriteriaDataType(_dataType))
    {
        raw.value1 = _value1;
        raw.value2 = _value2;
        ScriptId = _scriptId;
    }

    bool IsValid(CriteriaEntry const* criteria);
    bool Meets(uint32 criteria_id, Player const* source, Unit const* target, uint64 miscvalue1 = 0) const;
};

struct AchievementCriteriaDataSet
{
        AchievementCriteriaDataSet() : criteria_id(0) {}
        typedef std::vector<AchievementCriteriaData> Storage;
        void Add(AchievementCriteriaData const& data) { storage.push_back(data); }
        bool Meets(Player const* source, Unit const* target, uint64 miscvalue = 0) const;
        void SetCriteriaId(uint32 id) {criteria_id = id;}
    private:
        uint32 criteria_id;
        Storage storage;
};

typedef ACE_Based::LockedMap<uint32, AchievementCriteriaDataSet> AchievementCriteriaDataMap;

struct AchievementReward
{
    uint32 titleId[2];
    uint32 itemId;
    uint32 SpellID;
    uint32 sender;
    std::string subject;
    std::string text;
};

typedef ACE_Based::LockedMap<uint32, AchievementReward> AchievementRewards;

struct AchievementRewardLocale
{
    StringVector subject;
    StringVector text;
};

typedef ACE_Based::LockedMap<uint32, AchievementRewardLocale> AchievementRewardLocales;

struct CompletedAchievementData
{
    time_t date;
    std::set<uint64> guids;
    uint64 first_guid;
    bool completedByThisCharacter;
    bool changed;
};

typedef ACE_Based::LockedMap<uint32, CriteriaProgress> CriteriaProgressMap;
typedef ACE_Based::LockedMap<uint32, CompletedAchievementData> CompletedAchievementMap;

enum ProgressType
{
    PROGRESS_SET,
    PROGRESS_ACCUMULATE,
    PROGRESS_HIGHEST
};

template<class T>
class AchievementMgr
{
    public:
        AchievementMgr(T* owner);
        ~AchievementMgr();

        void Reset();
        static void DeleteFromDB(uint32 lowguid, uint32 accountId = 0);
        void LoadFromDB(Player* p_Player, Guild* p_Guild, PreparedQueryResult achievementResult, PreparedQueryResult criteriaResult, PreparedQueryResult achievementAccountResult = NULL, PreparedQueryResult criteriaAccountResult = NULL);
        void SaveToDB(SQLTransaction& trans);
        void ResetAchievementCriteria(AchievementCriteriaTypes type, uint64 miscValue1 = 0, uint64 miscValue2 = 0, bool evenIfCriteriaComplete = false);
        void UpdateAchievementCriteria(AchievementCriteriaTypes type, uint64 miscValue1 = 0, uint64 miscValue2 = 0, uint64 miscValue3 = 0, Unit const* unit = NULL, Player* referencePlayer = NULL, bool p_LoginCheck = false);
        bool IsCompletedAchievement(AchievementEntry const* entry);
        void CompletedAchievement(AchievementEntry const* entry, Player* referencePlayer, bool p_LoginCheck = false);
        void RewardAchievement(AchievementEntry const* p_Achievement);
        void CheckAllAchievementCriteria(Player* referencePlayer);
        void SendAllAchievementData(Player* receiver);
        void SendAchievementInfo(Player* receiver, uint32 achievementId = 0);
        bool HasAchieved(uint32 achievementId) const;
        bool HasAccountAchieved(uint32 achievementId) const;
        uint64 GetFirstAchievedCharacterOnAccount(uint32 achievementId) const;
        T* GetOwner() const { return _owner; }

        void UpdateTimedAchievements(uint32 timeDiff);
        void StartTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry, uint32 timeLost = 0);
        void RemoveTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry);   // used for quest and scripted timed achievements
        uint32 GetAchievementPoints() const { return _achievementPoints; }

        CompletedAchievementMap const& GetCompletedAchivements() const { return m_completedAchievements; }
        ACE_Thread_Mutex& GetCompletedAchievementLock() { return m_CompletedAchievementsLock; }

        void SetCriteriaProgress(CriteriaEntry const* entry, uint64 changeValue, Player* referencePlayer, ProgressType ptype = PROGRESS_SET);
        void SetCompletedAchievementsIfNeeded(CriteriaEntry const* p_Criteria, Player* p_RefPlayer, bool p_LoginCheck = false);

    private:
        void SendAchievementEarned(AchievementEntry const* achievement) const;
        void SendCriteriaUpdate(CriteriaEntry const* p_Entry, CriteriaProgress const* p_Progress, uint32 p_TimeElapsed, bool p_TimedCompleted, bool p_UpdateAccount) const;
        CriteriaProgressMap* GetCriteriaProgressMap();
        CriteriaProgress* GetCriteriaProgress(uint32 entry);
        CriteriaProgress* GetCriteriaProgress(CriteriaEntry const* entry);
        void RemoveCriteriaProgress(CriteriaEntry const* entry);
        void CompletedCriteriaFor(AchievementEntry const* achievement, Player* referencePlayer, bool p_LoginCheck = false);
        bool CanCompleteCriteria(CriteriaEntry const* achievementCriteria, AchievementEntry const* achievement);
        bool IsCompletedCriteria(CriteriaEntry const* criteria);
        bool IsCompletedCriteriaForAchievement(CriteriaEntry const* achievementCriteria, AchievementEntry const* achievement);
        bool CanUpdateCriteria(CriteriaEntry const* criteria, AchievementEntry const* achievement, uint64 miscValue1, uint64 miscValue2, uint64 miscValue3, Unit const* unit, Player* referencePlayer);
        void SendPacket(WorldPacket* data) const;

        bool ConditionsSatisfied(CriteriaEntry const *criteria, Player* referencePlayer) const;
        bool RequirementsSatisfied(CriteriaEntry const *criteria, uint64 miscValue1, uint64 miscValue2, uint64 miscValue3, Unit const* unit, Player* referencePlayer) const;
        bool AdditionalRequirementsSatisfied(CriteriaEntry const* criteria, uint64 miscValue1, uint64 miscValue2, Unit const* unit, Player* referencePlayer) const;
        bool RequiresScript(CriteriaEntry const* p_Criteria);

        T* _owner;
        CriteriaProgressMap m_criteriaProgress;
        CompletedAchievementMap m_completedAchievements;
        mutable ACE_Thread_Mutex m_CompletedAchievementsLock;
        typedef ACE_Based::LockedMap<uint32, uint32> TimedAchievementMap;
        TimedAchievementMap m_timedAchievements;      // Criteria id/time left in MS
        uint32 _achievementPoints;
        bool m_NeedDBSync;
};

struct AchievementCriteriaUpdateTask
{
    uint64 PlayerGUID;
    uint64 UnitGUID;
    std::function<void(uint64, uint64)> Task;
};

using LockedAchievementCriteriaTaskQueue   = ACE_Based::LockedQueue<AchievementCriteriaUpdateTask, ACE_Thread_Mutex>;
using LockedPlayersAchievementCriteriaTask = ACE_Based::LockedMap<uint64, LockedAchievementCriteriaTaskQueue>;

using AchievementCriteriaTaskQueue   = std::queue<AchievementCriteriaUpdateTask>;
using PlayersAchievementCriteriaTask = std::map<uint32, AchievementCriteriaTaskQueue>;

class AchievementGlobalMgr
{
        friend class ACE_Singleton<AchievementGlobalMgr, ACE_Null_Mutex>;
        AchievementGlobalMgr() {}
        ~AchievementGlobalMgr() {}

    public:
        static char const* GetCriteriaTypeString(uint32 type);

        AchievementCriteriaEntryList const& GetAchievementCriteriaByType(AchievementCriteriaTypes type) const
        {
            return m_AchievementCriteriasByType[type];
        }

        AchievementCriteriaEntryList const& GetTimedAchievementCriteriaByType(AchievementCriteriaTimedTypes type) const
        {
            return m_AchievementCriteriasByTimedType[type];
        }

        AchievementCriteriaTreeList const& GetAchievementCriteriaTreeList(CriteriaEntry const* criteria)
        {
            return m_AchievementCriteriaTreeByCriteriaId[criteria->ID];
        }

        AchievementEntryList const* GetAchievementByReferencedId(uint32 id) const
        {
            AchievementListByReferencedId::const_iterator itr = m_AchievementListByReferencedId.find(id);
            return itr != m_AchievementListByReferencedId.end() ? &itr->second : NULL;
        }

        ModifierTreeEntryList const& GetModifierTreeByModifierId(uint32 id) const
        {
            return m_ModifierTreeEntryByTreeId[id];
        }

        AchievementReward const* GetAchievementReward(AchievementEntry const* achievement) const
        {
            AchievementRewards::const_iterator iter = m_achievementRewards.find(achievement->ID);
            return iter != m_achievementRewards.end() ? &iter->second : NULL;
        }

        AchievementRewardLocale const* GetAchievementRewardLocale(AchievementEntry const* achievement) const
        {
            AchievementRewardLocales::const_iterator iter = m_achievementRewardLocales.find(achievement->ID);
            return iter != m_achievementRewardLocales.end() ? &iter->second : NULL;
        }

        AchievementCriteriaDataSet const* GetCriteriaDataSet(CriteriaEntry const* achievementCriteria) const
        {
            AchievementCriteriaDataMap::const_iterator iter = m_criteriaDataMap.find(achievementCriteria->ID);
            return iter != m_criteriaDataMap.end() ? &iter->second : NULL;
        }

        AchievementCriteriaTreeList const* GetSubCriteriaTreeById(uint32 p_ID) const
        {
            return &m_SubCriteriaTreeListById[p_ID];
        }

        AchievementEntry const* _GetAchievementEntryByCriteriaTree(CriteriaTreeEntry const* p_Criteria) const;

        AchievementEntry const* GetAchievementEntryByCriteriaTree(CriteriaTreeEntry const* p_Criteria) const
        {
            return m_AchievementEntryByCriteriaTreeId[p_Criteria->ID];
        }

        bool IsRealmCompleted(AchievementEntry const* achievement, uint32 instanceId) const
        {
            AllCompletedAchievements::const_iterator itr = m_allCompletedAchievements.find(achievement->ID);
            if (itr == m_allCompletedAchievements.end())
                return false;

            if (achievement->Flags & ACHIEVEMENT_FLAG_REALM_FIRST_KILL)
                return itr->second != instanceId;

            return true;
        }

        void SetRealmCompleted(AchievementEntry const* achievement, uint32 instanceId)
        {
            if (IsRealmCompleted(achievement, instanceId))
                return;

            m_allCompletedAchievements[achievement->ID] = instanceId;
        }

        bool IsGroupCriteriaType(AchievementCriteriaTypes type) const
        {
            switch (type)
            {
                case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE:
                case ACHIEVEMENT_CRITERIA_TYPE_WIN_BG:
                case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET:         // NYI
                case ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA:
                case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2:        // NYI
                case ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_BATTLEGROUND:  // NYI
                    return true;
                default:
                    break;
            }

            return false;
        }

        void LoadAchievementCriteriaList();
        void LoadAchievementCriteriaData();
        void LoadAchievementReferenceList();
        void LoadCompletedAchievements();
        void LoadRewards();
        void LoadRewardLocales();
        AchievementEntry const* GetAchievement(uint32 achievementId) const;
        CriteriaEntry const* GetAchievementCriteria(uint32 achievementId) const;

        void PrepareCriteriaUpdateTaskThread();

        void AddCriteriaUpdateTask(AchievementCriteriaUpdateTask const& p_Task)
        {
            m_LockedPlayersAchievementCriteriaTask[p_Task.PlayerGUID].add(p_Task);
        }

        PlayersAchievementCriteriaTask const& GetPlayersCriteriaTask() const
        {
            return m_PlayersAchievementCriteriaTask;
        }

        void ClearPlayersCriteriaTask()
        {
            m_PlayersAchievementCriteriaTask.clear();
        }

    private:
        AchievementCriteriaDataMap m_criteriaDataMap;

        // store achievement criterias by type to speed up lookup
        AchievementCriteriaEntryList m_AchievementCriteriasByType[ACHIEVEMENT_CRITERIA_TYPE_TOTAL];

        AchievementCriteriaEntryList m_AchievementCriteriasByTimedType[ACHIEVEMENT_TIMED_TYPE_MAX];

        // store achievements by referenced achievement id to speed up lookup
        AchievementListByReferencedId m_AchievementListByReferencedId;

        AchievementCriteriaTreeByCriteriaId m_AchievementCriteriaTreeByCriteriaId;
        AchievementEntryByCriteriaTree m_AchievementEntryByCriteriaTree;
        ModifierTreeEntryByTreeId m_ModifierTreeEntryByTreeId;
        typedef ACE_Based::LockedMap<uint32 /*achievementId*/, uint32 /*instanceId*/> AllCompletedAchievements;
        AllCompletedAchievements m_allCompletedAchievements;
        SubCriteriaTreeListById m_SubCriteriaTreeListById;

        AchievementEntryByCriteriaTreeId m_AchievementEntryByCriteriaTreeId;

        AchievementRewards m_achievementRewards;
        AchievementRewardLocales m_achievementRewardLocales;

        LockedPlayersAchievementCriteriaTask m_LockedPlayersAchievementCriteriaTask;  ///< All criteria update task are first storing here
        PlayersAchievementCriteriaTask       m_PlayersAchievementCriteriaTask;        ///< Before thread process, all task stored will be move here
};

#define sAchievementMgr ACE_Singleton<AchievementGlobalMgr, ACE_Null_Mutex>::instance()

class MapUpdater;
class AchievementCriteriaUpdateRequest : public MapUpdaterTask
{
    public:
        AchievementCriteriaUpdateRequest(MapUpdater* p_Updater, AchievementCriteriaTaskQueue p_TaskQueue);
        virtual void call() override;

    private:
        AchievementCriteriaTaskQueue m_CriteriaUpdateTasks;

};

#endif
