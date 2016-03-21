/*
 * Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com>
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

#ifndef SIEGE_OF_ORGRIMMAR_H_
#define SIEGE_OF_ORGRIMMAR_H_

#include "SpellScript.h"
#include "Map.h"
#include "Creature.h"
#include "CreatureAIImpl.h"

#define MAX_FALLEN_PROTECTORS 3
#define MAX_KORKRON_DARK_SHAMANS 2
#define MAX_PARAGONS_OF_THE_KLAXXI 9

enum eData
{
    // First part
    DATA_IMMERSEUS              = 0,
    DATA_FALLEN_PROTECTORS      = 1,
    DATA_NORUSHEN               = 2,
    DATA_SHA_OF_PRIDE           = 3,

    // Second part
    DATA_GALAKRAS               = 4,
    DATA_IRON_JUGGERNAUT        = 5,
    DATA_KORKRON_DARK_SHAMANS   = 6,
    DATA_GENERAL_NAZGRIM        = 7,

    // Third part
    DATA_MALKOROK               = 8,
    DATA_SPOILS_OF_PANDARIA     = 9,
    DATA_THOK_THE_BLOODTHIRSTY  = 10,

    // Last part
    DATA_SIEGECRAFTER_BLACKFUSE = 11,
    DATA_PARAGONS_OF_THE_KLAXXI = 12,
    DATA_GARROSH_HELLSCREAM     = 13,

    DATA_MAX_BOSS_DATA          = 14
};

enum eCreatures
{
    // Immerseus
    NPC_IMMERSEUS                       = 71543,
    NPC_SHA_PUDDLE                      = 71603,
    NPC_CONTAMINATED_PUDDLE             = 71604,

    // Fallen Protectors
    NPC_ROOK_STONETOE                   = 71475,
    NPC_HE_SOFTFOOT                     = 71479,
    NPC_SUN_THUNDERHEART                = 71480,

    // Norushen
    NPC_NORUSHEN                        = 71965,
    NPC_AMALGAM_OF_CORRUPTION           = 72276,
    NPC_ESSENCE_OF_CORRUPTION           = 71977,
    NPC_MANIFESTATION_OF_CORRUPTION     = 71976,

    // Sha of Pride
    NPC_SHA_OF_PRIDE                    = 71734,
    NPC_REFLECTION                      = 72172,

    // Galakras
    NPC_GALAKRAS                        = 72249,
    NPC_MASTER_CANNONEER_DAGRYN         = 72356,
    NPC_HIGH_ENFORCER_THRANOK           = 72355,
    NPC_LIEUTENANT_KRUGRUK              = 72357,
    NPC_KORGRA_THE_SNAKE                = 72456,
    NPC_DRAGONMAW_BONECRUSHER           = 72354,
    
    // Iron Juggernaut
    NPC_IRON_JUGGERNAUT                 = 71466,
    NPC_CRAWLER_MINE                    = 72050,

    // Kor'Kron Dark Shamans
    NPC_EARTHBREAKER_HAROMM             = 71859,
    NPC_WAVEBINDER_KARDIS               = 71858,
    NPC_DARKFANG                        = 71923,
    NPC_BLOODCLAW                       = 71923,
    NPC_FOULSTREAM_TOTEM                = 71916,
    NPC_POISONMIST_TOTEM                = 71917,
    NPC_ASHFLARE_TOTEM                  = 71917,
    NPC_FOUL_SLIM                       = 71825,

    // General Nazgrim
    NPC_GENERAL_NAZGRIM                 = 71515,
    NPC_ORGRIMMAR_FAITHFUL              = 71715,
    NPC_KORKRON_IRONBLADE               = 71770,
    NPC_KORKRON_ARCWEAVER               = 71771,
    NPC_KORKRON_ASSASSIN                = 71772,
    NPC_KORKRON_WARSHAMAN               = 71773,

    // Malkorok
    NPC_MALKOROK                        = 71454,

    // Spoils ofPandaria
    NPC_SPOILS_OF_PANDARIA              = 71889,
    NPC_MODIFIED_ANIMA_GOLEM            = 71395,
    NPC_MOGU_SHADOW_RITUALIST           = 71393,
    NPC_ZARTHIK_AMBER_PRIEST            = 71397,
    NPC_SETTHIK_WIELDER                 = 71405,
    NPC_ANIMATED_STONE_MOGU             = 71380,
    NPC_BURIAL_URN                      = 71382,
    NPC_QUILEN_GARDIANS                 = 72223,
    NPC_SRITHIK_BOMBARDIER              = 71385,
    NPC_KORTHIK_WARCALLER               = 62754,
    NPC_ANCIENT_BREWMASTER_SPIRIT       = 71427, // Unknown coordinates
    NPC_WISE_MISTEWEAVER_SPIRIT         = 71428, // Unknown coordinates
    NPC_NAMELESSE_WINDWALLKER_SPIRIT    = 71430, // Unknown coordinates

    // Thok the Bloothirsty
    NPC_THOK_THE_BLOODTHIRSTY           = 71529,
    NPC_KORKRON_JAILER                  = 71658,
    NPC_AKOLIK                          = 71742,
    NPC_WATERSPEAKER_GORAI              = 71749,
    NPC_MONTAK                          = 71763,

    // Siegecrafter Blackfuse
    NPC_SIEGECRAFTER_BLACKFUSE          = 71504,
    NPC_AUTOMATED_SHREDDER              = 71591,

    // Paragons of the Klaxxi
    NPC_KILRUK_THE_WIND_REAVER          = 71161,
    NPC_XARIL_THE_POISONED_MIND         = 71157,
    NPC_KAZTIK_THE_MANIPULATOR          = 71156,
    NPC_KORVEN_THE_PRIME                = 71155,
    NPC_IYYOKUK_THE_LUCID               = 71160,
    NPC_KAROZ_THE_LOCUST                = 71154,
    NPC_SKEER_THE_BLOODSEEKER           = 71152,
    NPC_RIKKAL_THE_DISSECTOR            = 71158,
    NPC_HISEK_THE_SWARMKEEPER           = 71153,

    // Garrosh Hellscream
    NPC_GARROSH_HELLSCREAM              = 71865,
    NPC_SIEGE_ENGINEER                  = 71984,
    NPC_WARBRINGER_KORKRON              = 71979,
    NPC_WOLF_RIDER_FARSEER              = 71983
};

enum eFallenProtectors
{
    ROOK_STONETOE,
    HE_SOFTFOOT,
    SUN_THUNDERHEART
};

enum eKorkronDarkShamans
{
    EARTHBREAKER_HAROMM,
    WAVEBINDER_KARDIS
};

enum eParagonsOfTheKlaaxi
{
    KILRUK_THE_WIND_REAVER,
    XARIL_THE_POISONED_MIND,
    KAZTIK_THE_MANIPULATOR,
    KORVEN_THE_PRIME,
    IYYOKUK_THE_LUCID,
    KAROZ_THE_LOCUST,
    SKEER_THE_BLOODSEEKER,
    RIKKAL_THE_DISSECTOR,
    HISEK_THE_SWARMKEEPER
};

enum eGameObjects
{
};

enum eAchievementData
{
};

enum sharedSpells
{
};

#endif // SIEGE_OF_ORGRIMMAR_H_
