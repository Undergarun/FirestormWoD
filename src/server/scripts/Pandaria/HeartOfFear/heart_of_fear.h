/*
 * Copyright (C) 2008-20XX JadeCore <http://www.pandashan.com>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#ifndef HEART_OF_FEAR_H_
#define HEART_OF_FEAR_H_

#include "SpellScript.h"
#include "Map.h"
#include "Creature.h"
#include "CreatureAIImpl.h"

enum eData
{
    DATA_ZORLOK         = 0,
    DATA_TAYAK          = 1,
    DATA_GARALON        = 2,
    DATA_MELJARAK       = 3,
    DATA_UNSOK          = 4,
    DATA_SHEKZEER       = 5,
    DATA_MAX_BOSS_DATA  = 6
};

enum eSpells
{
    // Imperial Vizier Zor'lok
    SPELL_ATTENUATION           = 122440,
    SPELL_CONVERT               = 122740,
    SPELL_INHALE                = 122852,
    SPELL_EXHALE                = 122761,
    SPELL_EXHALE_DMG            = 122760,
    SPELL_PHEROMONES_OF_ZEAL    = 123812,
    SPELL_PHEROMONES_CLOUD      = 123811,
    SPELL_FORCE_AND_VERVE       = 122713,
    SPELL_NOISE_CANCELLING      = 122706,
    SPELL_MISSILE_NOISE_CANC    = 122707,
    SPELL_SONG_OF_THE_EMPRESS   = 123791,
    SPELL_SONIC_RING_VISUAL     = 122334,
    SPELL_SONIC_RING_AURA       = 122336,
    SPELL_INHALE_PHEROMONES     = 124018,
    SPELL_REINFORCE             = 123833,
    SPELL_BERSERK               = 120207,
    SPELL_SPRING_RABBIT_JUMP    = 61724,

    // --- Blade Lord Ta'yak ---
    SPELL_TEMPEST_SLASH         = 122842,
    SPELL_TEMPEST_TARGET        = 122838,
    SPELL_TEMPEST_GESTURE       = 122850,
    SPELL_WIND_STEP_2           = 123175,
    SPELL_OVERWHELMING_ASSAULT  = 123474,
    SPELL_BLADE_TEMPEST         = 125310,
    SPELL_STORM_UNLEASHED       = 123815,

    // Trashes
    SPELL_ARTERIAL_SPIRIT       = 123422,
    SPELL_GOUGE_THROAT          = 123434,
    SPELL_SLOW                  = 31589,
    SPELL_VITAL_STRIKES         = 123421,
    SPELL_UNDYING_DEVOTION      = 123502,
    SPELL_BRAIN_FREEZE          = 57761,
    SPELL_DISMANTLED_ARMOR      = 123417,
    SPELL_MASS_SPELL_REFLEXION  = 124817,
    SPELL_SHIELD_SLAM           = 124815,
    SPELL_STUNNING_STRIKE       = 123420,
    SPELL_BLURRING_SLASH        = 123437,
    SPELL_RIPOSTE               = 123436,
    SPELL_ICE_TRAP              = 135382,
    SPELL_MASH_AND_GNASH        = 124809,
    SPELL_JAWBONE_SLAM          = 123490,
    SPELL_WIND_SLASH            = 125716,
    SPELL_GALE_FORCE_WINDS      = 123497,
    SPELL_WINDBURST             = 123499,
    SPELL_SUMMON_ZEPHYR         = 124085,
    SPELL_SWIFT_STEP            = 125742,
    SPELL_WIND_STEP             = 123180,
    SPELL_ZEALOUS_RUSH          = 123622,
    SPELL_UNDERWHELMING_ASSAULT = 125728,
    SPELL_GUST                  = 124072,
    SPELL_BURST                 = 125054,
};

enum eActions
{
    // --- Zorlok ---
    // Zor'lok
    ACTION_SUMMON_SONIC_RINGS   = 1,
    ACTION_SONIC_CARD           = 2,
    ACTION_SONIC_QUAD1          = 3,
    ACTION_SONIC_QUAD2          = 4,
    ACTION_INHALE_PHEROMONES    = 5,
    ACTION_WIPE                 = 6,

    // Ta'yak
    ACTION_TAYAK_TALK_TRASH     = 7,
};

enum eCreatures
{
    // Generic / Trash
    NPC_SETTHIK_TEMPEST             = 64358,
    NPC_KORTHIL_SLICER              = 63048,
    NPC_KAZRIK                      = 64826,
    NPC_SPEARED_KLAXXI              = 65521,
    NPC_MANTID_SPEAR                = 65519,
    NPC_SPEARED_KLAXXI_2            = 65524,
    NPC_SETTHIK_SWIFTBLADE          = 63049,
    NPC_SRATHIK_SHIELD_MASTER       = 63032,
    NPC_ZARTHIK_SUPPLICANT          = 63853,
    NPC_SETTHIK_ZEPHYRIAN           = 63593,
    NPC_BURNING_BRAZIER             = 65597,
    NPC_WIND_LORD_MELJARAK          = 65501,
    NPC_ZARTHIK_BATTLEMENDER        = 65498,
    NPC_JADE_SERPENT_STATUE         = 60849,
    NPC_KORTHIK_SILENTWING          = 64355,
    NPC_ZARTHIK_SUPPLICANT_2        = 67177,
    NPC_SETTHIK_GALESLICER          = 64353,
    NPC_XUEN                        = 63508,
    NPC_ENSLAVED_BONESMASHER        = 63030,
    NPC_ZARTHIK_SUPPLICANT_3        = 66181,
    NPC_SETTHIK_FANATIC             = 63031,
    NPC_INSTRUCTOR_MALTIK           = 64340,
    NPC_INSTRUCTOR_KLITHAK          = 64338,
    NPC_INSTRUCTOR_ZARIK            = 64341,
    NPC_INSTRUCTOR_TAKTHOK          = 64339,
    NPC_ZARTHIK_ZEALOT              = 63035,
    NPC_KORTHIK_SWARMER             = 64357,
    NPC_SETTHIK_GUSTWING            = 63592,
    NPC_COAGULATED_AMBER            = 63597,

    // Zor'lok
    NPC_ZORLOK                      = 62980,
    NPC_STORM_SPIRIT                = 69680,
    // Sonic Rings
    NPC_SONIC_RING_FORWARD          = 62689,
    NPC_SONIC_RING_30LEFT           = 62694,
    NPC_SONIC_RING_30RIGHT          = 62696,
    NPC_SONIC_RING_60LEFT           = 62716,
    NPC_SONIC_RING_60RIGHT          = 62717,
    NPC_SONIC_RING_120LEFT          = 62718,
    NPC_SONIC_RING_120RIGHT         = 62719,
    NPC_SONIC_RING_90LEFT           = 62726,
    NPC_SONIC_RING_90RIGHT          = 62727,
    NPC_SONIC_RING_150LEFT          = 62743,
    NPC_SONIC_RING_150RIGHT         = 62744,
    NPC_SONIC_RING_BACK             = 62746,

    // Heroic Adds
    NPC_ECHO_OF_ATTENUATION         = 65173,
    NPC_ECHO_OF_FORCE_AND_VERVE     = 65174,

    // Ta'yak
    NPC_TAYAK                       = 62543,
    NPC_TEMPEST_TARGET              = 62897,
    NPC_TEMPEST_STALKER             = 64373, //62908,
    NPC_GALE_WINDS_STALKER          = 63292,
    NPC_STORM_UNLEASHED_EAST1       = 63301,
    NPC_STORM_UNLEASHED_EAST2       = 63302,
    NPC_STORM_UNLEASHED_EAST3       = 63303,
    NPC_STORM_UNLEASHED_WEST1       = 63278,
    NPC_STORM_UNLEASHED_WEST2       = 63299,
    NPC_STORM_UNLEASHED_WEST3       = 63300,
    NPC_STORM_PLAYER_VEHICLE        = 63567,

    // Garalon
    NPC_GARALON                     = 62164,
    NPC_GARALON_LEG                 = 63053,
    NPC_PHEROMONE_TRAIL             = 63021,

    // Mel'jarak
    NPC_MELJARAK                    = 62397,
    NPC_SRATHIK_AMBER_TRAPPER       = 65499,
    NPC_ZARTHIK_BATTLE_MENDER       = 65498,
    NPC_KORTHIK_ELITE_BLADEMASTER   = 65500,

    // Amber-Shaper Un'Sok
    NPC_UNSOK                       = 62511,
    NPC_LIVING_AMBER                = 62691,
    NPC_MUTATED_CONSTRUCT           = 62701,
    NPC_VOLATILE_AMBER_GLOBULE      = 73332,

    // Grand Empress Shek'zeer
    NPC_SHEKZEER                    = 62837,
    NPC_SETTHIK_WINDBLADE           = 64453,
    NPC_KORTHIK_REAVER              = 63591,
};

enum eGameObjects
{
    GOB_ANTECHAMBER_DOOR_ENTRANCE   = 214483,
    GOB_ANTECHAMBER_DOOR_EXIT       = 214484,
    GOB_ORATIUM_DOOR_ENTRANCE       = 214485,
    GOB_QUARTERS_DOOR_ENTRANCE      = 214486,
    GOB_QUARTERS_DOOR_EXIT          = 214487,
    GOB_STAIRWAYS_DOOR_EXIT         = 214488,
    GOB_BALCONY_DOOR_EXIT           = 214489,
    GOB_ATRIUM_DOOR_ENTRANCE        = 214490,
    GOB_ATRIUM_DOOR_EXIT            = 214491,
    GOB_SANCTUM_DOOR_ENTRANCE       = 214492,
    GOB_HEARTOFFEAR_DOOR_ENTRANCE   = 214493,

    // Zor'lok Barriers
    GOB_ARENA_WALLS                 = 212916,
    GOB_FINAL_PHASE_WALLS           = 212943,
};

enum ePhases
{
    PHASE_ZORLOK1                   = 1,
    PHASE_ZORLOK2                   = 4, // value '4' needed, DON'T CHANGE IT !!!

    PHASE_TAYAK1                    = 2,
    PHASE_TAYAK2                    = 3,
};

enum eWeapons
{
    EQUIP_ZORLOK                    = 85947,
    EQUIP_TAYAK                     = 85401,
};

enum eEvents
{
    // --- Zorlok ---
    // Zor'lok
    EVENT_INHALE                    = 1,
    EVENT_EXHALE                    = 2,
    EVENT_BERSERK                   = 3,
    EVENT_ATTENUATION               = 4,
    EVENT_SUMMON_RINGS              = 5,
    EVENT_SUMMON_RINGS1             = 6,
    EVENT_SUMMON_RINGS2             = 7,
    EVENT_PHEROMONES_CLOUD          = 8,
    EVENT_FORCE_AND_VERVE           = 9,
    EVENT_CAST_FANDV                = 10,
    EVENT_CONVERT                   = 11,
    EVENT_PULL_RAID                 = 12,

    // Ta'yak
    // EVENT_TEMPEST_SLASH             = 15,
    // EVENT_TAYAK_WIND_STEP           = 16,
    // EVENT_UNSEEN_STRIKE             = 17,
    EVENT_INTENSIFY                 = 18,
    EVENT_TAYAK_OVERW_ASSAULT       = 19,
    // EVENT_BLADE_TEMPEST             = 20,
    // EVENT_STORM_UNLEASHED           = 21,
    EVENT_TEMPEST_MOVE              = 22,

    // Sonic Ring
    EVENT_SONIC_CHECK_POSITION      = 50,
    EVENT_SONIC_MOVE                = 51,

    // Trashes
    EVENT_ARTERIAL_SPIRIT,
    EVENT_GOUGE_THROAT,
    EVENT_SLOW,
    EVENT_VITAL_STRIKES,
    EVENT_UNDYING_DEVOTION,
    EVENT_BRAIN_FREEZE,
    EVENT_DISMANTLED_ARMOR,
    EVENT_MASS_SPELL_REFLEXION,
    EVENT_SHIELD_SLAM,
    EVENT_STUNNING_STRIKE,
    EVENT_BLURRING_SLASH,
    EVENT_RIPOSTE,
    EVENT_ICE_TRAP,
    EVENT_MASH_AND_GNASH,
    EVENT_JAWBONE_SLAM,
    EVENT_WIND_SLASH,
    EVENT_GALE_FORCE_WINDS,
    EVENT_WINDBURST,
    EVENT_SUMMON_ZEPHYR,
    EVENT_SWIFT_STEP,
    EVENT_WIND_STEP,
    EVENT_WIND_STEP_2,
    EVENT_OVERWHELMING_ASSAULT,
    EVENT_ZEALOUS_RUSH,
    EVENT_UNDERWHELMING_ASSAULT,
    EVENT_GUST,
    EVENT_BURST,
};

enum eTypes
{
    // Zor'lok
    TYPE_EXHALE_TARGET  = 1,
    // Tayak
    TYPE_STORM_POINT    = 2,
    TYPE_PHASE_TAYAK    = 3,
    TYPE_STORM_PHASE    = 4,
};

#define DISPLAYID_INVISIBLE 11686

#endif // HEART_OF_FEAR_H_