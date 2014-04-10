#ifndef KUN_LAI_SUMMIT_H
#define KUN_LAI_SUMMIT_H

enum eSpells
{
    SPELL_UNSTABLE_SERUM       = 127373,
    SPELL_RAIN_DANCE           = 124860,
    SPELL_TORRENT              = 124935,
    SPELL_WATER_BOLT           = 124854,
    SPELL_DRAIN_LIFE           =  84533,
    SPELL_SHADOW_BOLT          =   9613,
    SPELL_SHADOW_CRASH         = 129132,
    SPELL_DEVASTATING_ARC      = 124946,
    SPELL_SUMMON_QUILEN        = 124980,
    SPELL_TITANIC_STRENGH      = 124976,
    SPELL_BLADE_FURY           = 125370,
    SPELL_TORNADO              = 125398,
    SPELL_TORNADO_DMG          = 131693,
    SPELL_WINDSONG             = 125373,
    SPELL_VICIOUS_REND         = 125624,
    SPELL_GRAPPLING_HOOK       = 125623,
    SPELL_VANISH               = 125632,
    SPELL_SMOKED_BLADE         = 125633,
    SPELL_DEVASTATION          = 126631,
    SPELL_HATED_BLAST          = 126628,
    SPELL_SHA_CORRUPTION       = 126625,
    SPELL_UNTAMED_FURY         = 23719,
    SPELL_WHIRLWIND_OF_ANGER   = 126633
};

enum eEvents
{
    EVENT_RAIN_DANCE                 = 1,
    EVENT_TORRENT                    = 2,
    EVENT_WATER_BOLT                 = 3,
    EVENT_UNSTABLE_SERUM             = 4,
    EVENT_DRAIN_LIFE                 = 5,
    EVENT_SHADOW_BOLT                = 6,
    EVENT_SHADOW_CRASH               = 7,
    EVENT_DEVASTATING_ARC            = 8,
    EVENT_SUMMON_QUILEN              = 9,
    EVENT_TITANIC_STRENGH            = 10,
    EVENT_BLADE_FURY                 = 11,
    EVENT_TORNADO                    = 12,
    EVENT_WINDSONG                   = 13,
    EVENT_VICIOUS_REND               = 14,
    EVENT_GRAPPLING_HOOK             = 15,
    EVENT_VANISH                     = 16,
    EVENT_SMOKED_BLADE               = 17,
    EVENT_ZHI_TALK_1                 = 18,
    EVENT_ZHI_TALK_2                 = 19,
    EVENT_ANDRUIN_TALK_1             = 20,
    EVENT_ANDRUIN_TALK_2             = 21,
    EVENT_ANDRUIN_TALK_3             = 22,
    EVENT_ANDRUIN_TALK_4             = 23,
    EVENT_TARAN_ZHU_TALK_1           = 24,
    EVENT_TARAN_ZHU_TALK_2           = 25,
    EVENT_TARAN_ZHU_TALK_3           = 26,
    EVENT_TARAN_ZHU_TALK_4           = 27,
    EVENT_XUEN_TALK_1                = 28,
    EVENT_XUEN_TALK_2                = 29,
    EVENT_XUEN_TALK_3                = 30,
    EVENT_XUEN_TALK_4                = 31,
    EVENT_PLAYER_TELEPORT            = 32,
    EVENT_SPAWN_SPIRIT_OF_VIOLENCE   = 33,
    EVENT_DEVASTATION                = 34,
    EVENT_HATED_BLAST                = 35,
    EVENT_SHA_CORRUPTION             = 36,
    EVENT_SPAWN_SPIRIT_OF_ANGER      = 37,
    EVENT_SPAWN_SPIRIT_OF_HATRED     = 38,
    EVENT_UNTAMED_FURY               = 39,
    EVENT_WHIRLWIND_OF_ANGER         = 40,
    EVENT_FINAL_KILL_CREDIT          = 41
};

enum eQuests
{
    QUEST_A_CELESTIAL_EXPERIENCE_H = 31395,
    QUEST_A_CELESTIAL_EXPERIENCE_A = 31394
};

enum eActions
{
    ACTION_ZHI_TALK_1          = 1,
    ACTION_ZHI_TALK_2          = 2,
    ACTION_ANDRUIN_TALK_1      = 3,
    ACTION_ANDRUIN_TALK_2      = 4,
    ACTION_ANDRUIN_TALK_3      = 5,
    ACTION_ANDRUIN_TALK_4      = 6,
    ACTION_TARAN_ZHU_TALK_1    = 7,
    ACTION_TARAN_ZHU_TALK_2    = 8,
    ACTION_TARAN_ZHU_TALK_3    = 9,
    ACTION_TARAN_ZHU_TALK_4    = 10,
    ACTION_XUEN_TALK_1         = 11,
    ACTION_XUEN_TALK_2         = 12,
    ACTION_XUEN_TALK_3         = 13,
    ACTION_XUEN_TALK_4         = 14,
    ACTION_CONVERSATION        = 15,
    ACTION_SECOND_CONVERSATION = 16,
    ACTION_THIRD_CONVERSATION  = 17,
    ACTION_FINAL_KILL_CREDIT   = 18
};

enum eCreatures
{
    NPC_TARAN_ZHU                 = 64536,
    NPC_ANDRUIN_WRYNN             = 64540,
    NPC_ZHI_THE_HARMONIOUS        = 64537,
    MOB_SPIRIT_OF_ANGER           = 64684,
    MOB_SPIRIT_OF_VIOLENCE        = 64656,
    MOB_SPIRIT_OF_HATRED_1        = 64742,
    MOB_SPIRIT_OF_HATRED_2        = 64744,
    KILLED_CREDIT_FIRST_PHASE     = 64528,
    KILLED_CREDIT_SECOND_PHASE    = 64656,
    KILLED_CREDIT_THIRD_PHASE     = 64684,
    KILLED_CREDIT_FOURTH_PHASE_H  = 64742,
    KILLED_CREDIT_FOURTH_PHASE_A  = 64744,
};

enum eDatas
{
    XUEN_PHASE_1 = 0,
    XUEN_PHASE_2 = 1,
    XUEN_PHASE_3 = 2,
    XUEN_PHASE_4 = 3,
    XUEN_PHASE_MAX = 4
};

#endif
