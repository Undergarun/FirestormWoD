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

#ifndef THRONE_OF_THUNDER_H_
#define THRONE_OF_THUNDER_H_

#include "SpellScript.h"
#include "Map.h"
#include "Creature.h"
#include "CreatureAIImpl.h"

enum eData
{
    // Last Stand of the Zandalari
    DATA_JIN_ROKH_THE_BREAKER   = 0,
    DATA_HORRIDON               = 1,
    DATA_CONCIL_OF_ELDERS       = 2,

    // Forgotten Depths
    DATA_TORTOS                 = 3,
    DATA_MEGAERA                = 4,
    DATA_JI_KUN                 = 5,

    // Halls of Flesh-Shaping
    DATA_DURUMU_THE_FORGOTTEN   = 6,
    DATA_PRIMORDIUS             = 7,
    DATA_DARK_ANIMUS            = 8,

    // Pinnacle of Storms
    DATA_IRON_QON               = 9,
    DATA_TWIN_CONSORTS          = 10,
    DATA_LEI_SHEN               = 11,
    DATA_RA_DEN                 = 12,

    DATA_MAX_BOSS_DATA          = 13,

    DATA_STATUE_0               = 14,
    DATA_STATUE_1               = 15,
    DATA_STATUE_2               = 16,
    DATA_STATUE_3               = 17,

    DATA_ANCIENT_MOGU_BELL      = 18
};

enum eCreatures
{
    // Trash mobs
    NPC_ZANDALARI_WATER_BINDER      = 69455,
    NPC_ZANDALARI_BLADE_INITIATE    = 70230,
    NPC_ZANDALARI_SPEAR_SHAPER      = 69388,
    NPC_THROWN_SPEAR                = 69438,
    NPC_ZANDALARI_STORM_CALLER      = 69390,
    NPC_TRASH_FOCUSED_LIGHTNING     = 70174,
    NPC_ANCIENT_PYTHON              = 70448,
    NPC_DARK_WINDS_BUNNY            = 59394,
    NPC_DRAKKARI_FROST_WARDEN       = 69910,
    NPC_GLACIAL_FREEZE_TOTEM        = 70047,
    NPC_GURUBASHI_BERSERKER_1       = 69905,
    NPC_GURUBASHI_BERSERKER_2       = 69916,
    NPC_AMANI_SHI_FLAME_CHANTER     = 69909,
    NPC_FARRAKI_SAND_CONJURER       = 69899,
    NPC_SAND_ELEMENTAL              = 69944,
    NPC_ZANDALARI_HIGH_PRIEST       = 69906,
    NPC_ZANDALARI_PROPHET           = 70557,
    NPC_ZANDALARI_WARLORD           = 69911,
    NPC_ZANDALARI_PRELATE           = 69927,
    NPC_WATERSPOUT                  = 70147,
    NPC_VAMPIRIC_CAVE_BAT           = 69352,
    NPC_GREATER_CAVE_BAT            = 69351,
    NPC_MYSTERIOUS_MUSHROOM         = 70545,
    NPC_SHALE_STALKER               = 70587,
    NPC_FUNGAL_GROWTH               = 70153,
    NPC_MIST_LURKER                 = 70594,
    NPC_CAVERN_BURROWER             = 70589,
    NPC_ETERNAL_GUARDIAN            = 70586,
    NPC_SLG_GENERIC_MOP             = 63420,

    // Jin'Rokh the Breaker
    NPC_JIN_ROKH_THE_BREAKER        = 69465,
    NPC_FOCUSED_LIGHTNING           = 69593,
    NPC_LIGHTNING_FISSURE           = 69609,
    NPC_STATUE                      = 69467,
    NPC_CONDUCTIVE_WATER            = 69469,
    NPC_LIGHTNING_SPARK             = 69635,
    NPC_LIGHTNING_PILLER_STALKER    = 69813,

    // Horridon
    NPC_HORRIDON                    = 68476,
    NPC_WAR_GOD_JALAK               = 69374,
    NPC_DIREHORN_SPIRIT             = 70688,
    // Farraki tribe
    NPC_SUL_LITHUZ_STONEGAZER       = 69172,
    NPC_FARRAKI_SKIRMICHER          = 69173,
    NPC_FARRAKI_WASTEWALKER         = 69175,
    NPC_ZANDALARI_DINOMANCER        = 69221,
    NPC_SAND_TRAP                   = 69346,
    // Gurubashi tribe
    NPC_GURUBASHI_BLOODLORD         = 69167,
    NPC_GURUBASHI_VENOM_PRIEST      = 69164,
    NPC_VENOMOUS_EFFUSION           = 69314,
    // Drakkari tribe
    NPC_RISEN_DRAKKARI_WARRIOR      = 69184,
    NPC_RISEN_DRAKKARI_CHAMPION     = 69185,
    NPC_DRAKKARI_FROZEN_WARLORD     = 69178,
    NPC_FROZEN_ORB                  = 69268,
    // Amani tribe
    NPC_AMANI_SHI_FLAME_CASTER      = 69168,
    NPC_AMANI_SHI_PROTECTOR         = 69169,
    NPC_AMANI_SHI_BEAST_SHAMAN      = 69176,
    NPC_AMANI_WARBEAR               = 69177,
    NPC_LIGHTNING_NOVA_TOTEM        = 69215,

    // Council of Elders
    NPC_KAZRA_JIN                   = 69134,
    NPC_SUL_THE_SANDCRAWLER         = 69078,
    NPC_FROST_KING_MALAKK           = 69131,
    NPC_HIGH_PRIESTRESS_MAR_LI      = 69132,
    NPC_GARA_JAL_SOUL               = 69182,
    NPC_LIVING_SAND                 = 69153,
    NPC_BLESSED_LOA_SPIRIT          = 69480,
    NPC_FIRST_TWISTED_FATE          = 69740,
    NPC_SECOND_TWISTED_FATE         = 69746,
    NPC_RECKLESS_CHARGE             = 69453,
    NPC_GARA_JAL                    = 69135,

    // Tortos
    NPC_TORTOS                      = 67977,
    NPC_ROCKFALL                    = 68219,
    NPC_VAMPIRIC_CAVE_BAT_SUMMON    = 68497,
    NPC_WHIRL_TURTLE                = 67966,
    NPC_HUMMING_CRYSTAL             = 69639,

    // Megaera
    NPC_MEGAERA                     = 68065,
    NPC_FLAMING_HEAD                = 70212,
    NPC_BACK_FLAMING_HEAD           = 70229,
    NPC_FROZEN_HEAD                 = 70235,
    NPC_BACK_FROZEN_HEAD            = 70250,
    NPC_VENOMOUS_HEAD               = 70247,
    NPC_BACK_VENOMOUS_HEAD          = 70251,
    NPC_ARCANE_HEAD                 = 70248,
    NPC_BACK_ARCANE_HEAD            = 70252,
    NPC_FRONT_HEAD_SPAWNER          = 70175,
    NPC_BACK_HEAD_SPAWNER           = 68136,
    NPC_CINDERS                     = 70432,
    NPC_ACID_RAIN                   = 70435,
    NPC_ICY_GROUND                  = 70446,
    NPC_NETHER_WYRM                 = 70507,

    NPC_JI_KUN                      = 69712,

    NPC_DURUMU_THE_FORGOTTEN        = 68036,

    NPC_PRIMORDIUS                  = 69017,
    NPC_LIVING_FLUID                = 69069,
    NPC_VICIOUS_HORROR              = 69070,

    NPC_DARK_ANIMUS                 = 69427,

    NPC_IRON_QON                    = 68078,

    // Twin Consorts
    NPC_LU_LIN                      = 68905,
    NPC_SUEN                        = 68904,

    NPC_LEI_SHI                     = 68397,

    NPC_RA_DEN                      = 69473
};

enum eGameObjects
{
    GOB_MOGU_FOUNTAIN_NW            = 218678,
    GOB_MOGU_FOUNTAIN_SW            = 218676,
    GOB_MOGU_FOUNTAIN_NE            = 218677,
    GOB_MOGU_FOUNTAIN_SE            = 218675,

    GOB_HORRIDON_GATE               = 218674,
    GOB_FARRAKI_TRIBAL_DOOR         = 218672,
    GOB_FARRAKI_ORB_OF_CONTROL      = 218193,
    GOB_GURUBASHI_TRIBAL_DOOR       = 218670,
    GOB_GURUBASHI_ORB_OF_CONTROL    = 218374,
    GOB_DRAKKARI_TRIBAL_DOOR        = 218671,
    GOB_DRAKKARI_ORB_OF_CONTROL     = 218375,
    GOB_AMANI_TRIBAL_DOOR           = 218673,
    GOB_AMANI_ORB_OF_CONTROL        = 218376,

    GOB_JIN_ROKH_ENTRANCE_DOOR      = 218664,
    GOB_JIN_ROKH_EXIT_DOOR          = 218663,
    GOB_HORRIDON_ENTRANCE_DOOR      = 218667,
    GOB_HORRIDON_EXIT_DOOR          = 218666,
    GOB_COUNCIL_ENTRANCE_DOOR_LEFT  = 218655,
    GOB_COUNCIL_ENTRANCE_DOOR_RIGHT = 218656,
    GOB_COUNCIL_EXIT_DOOR           = 218657,

    GOB_TORTOS_EXIT_DOOR            = 218980,
    GOB_TORTOS_DEATH_COLLISION      = 218987,

    GOB_ANCIENT_MOGU_BELL           = 218723,

    GOB_MEGAERA_CHEST_10_NORMAL     = 218805,
    GOB_MEGAERA_CHEST_25_NORMAL     = 218806,
    GOB_MEGAERA_CHEST_10_HEROIC     = 218807,
    GOB_MEGAERA_CHEST_25_HEROIC     = 218808
};

enum eAchievementData
{
};

enum eSharedSpells
{
    SPELL_ENRAGE                    = 47008,
    SPELL_SELF_ROOT                 = 125467,
    SPELL_DARK_WINDS_FORCE_WEATHER  = 139485,

    // Vampiric Cave Bat
    SPELL_DRAIN_THE_WEAK            = 135103,
    SPELL_DRAIN_THE_WEAK_TRIGGERED  = 135101,
    SPELL_DRAIN_THE_WEAK_HEAL       = 135102
};

enum eSharedActions
{
    ACTION_MEGAERA_SPAWN
};

#endif // THRONE_OF_THUNDER_H_
