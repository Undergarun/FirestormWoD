////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef THRONE_OF_THUNDER_H_
#define THRONE_OF_THUNDER_H_

#include "SpellScript.h"
#include "Map.h"
#include "Creature.h"
#include "CreatureAIImpl.h"
#include "Group.h"

enum eData
{
    // Last Stand of the Zandalari
    DATA_JIN_ROKH_THE_BREAKER   = 0,
    DATA_HORRIDON               = 1,
    DATA_COUNCIL_OF_ELDERS      = 2,

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

    DATA_ANCIENT_MOGU_BELL      = 18,
    DATA_THUNDER_CLAP           = 19
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
    NPC_BORE_WORM                   = 68221,
    NPC_BOW_FLY_SWARM               = 68222,
    NPC_BLOW_FLY                    = 70294,
    NPC_GASTROPOD                   = 68220,
    NPC_WEB                         = 68249,
    NPC_CORPSE_SPIDER               = 68248,
    NPC_QUIVERING_BLOB              = 69383,
    NPC_MALIGNANT_OOZE              = 69382,
    NPC_RITUAL_GUARD                = 70179,
    NPC_MUCKBAT                     = 70232,
    NPC_SKITTERING_SPIDERLING       = 70227,
    NPC_PUTRID_WASTE                = 70219,
    NPC_ROTTING_SCAVENGER           = 70224,
    NPC_SHANZE_CELESTIAL_SHAPER     = 70240,
    NPC_CELESTIAL_CONSTRUCT         = 70241,
    NPC_INVISIBLE_MAN               = 64188,
    NPC_THUNDER_LORD                = 69821,
    NPC_LIGHTNING_GUARDIAN          = 69834,

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
    NPC_SHADOWED_LOA_SPIRIT         = 69548,

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
    NPC_CINDERS                     = 70432,
    NPC_ACID_RAIN                   = 70435,
    NPC_ICY_GROUND                  = 70446,
    NPC_NETHER_WYRM                 = 70507,
    NPC_TORRENT_OF_ICE              = 70439,

    // Ji-Kun
    NPC_JI_KUN                      = 69712,
    NPC_FEED_POOL                   = 68188,
    NPC_FEED                        = 68178,
    NPC_JI_KUN_FLEDGLING_S_EGG      = 68202,
    NPC_HATCHLING                   = 68192,
    NPC_FLEDGLING                   = 68193, // Hatchling evolution or spawned by Fledgling egg
    NPC_YOUNG_EGG_OF_JI_KUN         = 68194, // Is spawned above hatchlings
    NPC_INCUBATER                   = 69626, // Middle of nest
    NPC_MATURE_EGG_OF_JI_KUN        = 69628, // Spawned by Fledgling
    NPC_JUVENILE                    = 69836, // Spawned by Mature Egg
    NPC_FLEGLING_JUVENILE           = 70095, // Juvenile spawned by Fledgling's egg
    NPC_FALL_CATCHER                = 69839, // Catch falling players and resend them on Ji-kun platform
    NPC_EXIT_CHAMBER                = 70734, // Spawns
    NPC_NEST_GUARDIAN               = 70134, // Heroic mode
    NPC_JI_KUN_HATCHLING            = 70144,

    // Durumu the Forgotten
    NPC_DURUMU_THE_FORGOTTEN        = 68036,
    NPC_APPARAISING_EYE             = 67858,
    NPC_HUNGRY_EYE                  = 67859,
    NPC_CROSS_EYE                   = 67857,
    NPC_MIND_S_EYE                  = 67875,
    NPC_YELLOW_EYE                  = 67856,
    NPC_RED_EYE                     = 67855,
    NPC_BLUE_EYE                    = 67854,
    NPC_EVIL_EYE                    = 67860,
    NPC_AZURE_FOG                   = 69052,
    NPC_AMBER_FOG                   = 69051,
    NPC_CRIMSON_FOG                 = 69050,
    NPC_EYEBEAM_TARGET              = 67882,
    NPC_CUSTOM_DAMAGE_CHECKER       = 200002,
    NPC_CUSTOM_EYEBEAM_TARGET       = 200001,
    NPC_CUSTOM_CROSS_EYE            = 200000,
    NPC_MONSTROSITY                 = 67791,

    // Primordius
    NPC_PRIMORDIUS                  = 69017,
    NPC_LIVING_FLUID                = 69069,
    NPC_VISCOUS_HORROR              = 69070,

    // Dark Animus
    NPC_DARK_ANIMUS                 = 69427,
    NPC_DARK_RITUALIST              = 69702,
    NPC_ANIMA_GOLEM                 = 69701,
    NPC_LARGE_ANIMA_GOLEM           = 69700,
    NPC_MASSIVE_ANIMA_GOLEM         = 69699,
    NPC_CRIMSON_WAKE                = 69951,
    NPC_ANIMA_ORB                   = 69756,

    // Iron Qon
    NPC_IRON_QON                    = 68078,
    NPC_RO_SHAK                     = 68079,
    NPC_QUET_ZAL                    = 68080,
    NPC_DAM_REN                     = 68081,
    NPC_MANCHU                      = 70202,
    NPC_WEISHENG                    = 70205,
    NPC_UNTRAINED_QUILEN            = 70206,
    NPC_UNTRAINED_QUILEN2           = 70209,

    // Twin Consorts
    NPC_LU_LIN                      = 68905,
    NPC_SUEN                        = 68904,
    NPC_WORLD_TRIGGER               = 59481,

    // Lei Shen
    NPC_THUNDER_TRAP                = 69825,
    NPC_LEI_SHEN                    = 68397,
    NPC_STATIC_SHOCK_CONDUIT        = 68398,
    NPC_DIFFUSION_CHAIN_CONDUIT     = 68696,
    NPC_OVERCHARGE_CONDUIT          = 68697,
    NPC_BOUNCING_BOLT_CONDUIT       = 68698,
    NPC_THUNDEROUS_THROW            = 68574,
    NPC_OVERWHELMING_POWER          = 69645,
    NPC_QUADRANT_STALKER            = 69024,
    NPC_UNHARNESSED_POWER           = 69133,

    NPC_RA_DEN                      = 69473
};

enum eGameObjects
{
    /// Jin'rokh
    GOB_JIN_ROKH_ENTRANCE_DOOR      = 218664,
    GOB_JIN_ROKH_EXIT_DOOR          = 218663,
    GOB_HORRIDON_ENTRANCE_DOOR      = 218667,
    GOB_HORRIDON_EXIT_DOOR          = 218666,
    GOB_MOGU_FOUNTAIN_NW            = 218678,
    GOB_MOGU_FOUNTAIN_SW            = 218676,
    GOB_MOGU_FOUNTAIN_NE            = 218677,
    GOB_MOGU_FOUNTAIN_SE            = 218675,

    /// Horridon
    GOB_HORRIDON_GATE               = 218674,
    GOB_FARRAKI_TRIBAL_DOOR         = 218672,
    GOB_FARRAKI_ORB_OF_CONTROL      = 218193,
    GOB_GURUBASHI_TRIBAL_DOOR       = 218670,
    GOB_GURUBASHI_ORB_OF_CONTROL    = 218374,
    GOB_DRAKKARI_TRIBAL_DOOR        = 218671,
    GOB_DRAKKARI_ORB_OF_CONTROL     = 218375,
    GOB_AMANI_TRIBAL_DOOR           = 218673,
    GOB_AMANI_ORB_OF_CONTROL        = 218376,

    /// Council of Elders
    GOB_COUNCIL_ENTRANCE_DOOR_LEFT  = 218655,
    GOB_COUNCIL_ENTRANCE_DOOR_RIGHT = 218656,
    GOB_COUNCIL_EXIT_DOOR           = 218657,

    /// Tortos
    GOB_TORTOS_EXIT_DOOR            = 218980,
    GOB_TORTOS_DEATH_COLLISION      = 218987,

    /// Megaera
    GOB_ANCIENT_MOGU_BELL           = 218723,
    GOB_MEGAERA_CHEST_10_NORMAL     = 218805,
    GOB_MEGAERA_CHEST_25_NORMAL     = 218806,
    GOB_MEGAERA_CHEST_10_HEROIC     = 218807,
    GOB_MEGAERA_CHEST_25_HEROIC     = 218808,
    GOB_MEGAERA_EXIT_DOOR           = 218746,

    /// Ji-Kun
    GOB_FEATHER_OF_JI_KUN           = 218543,
    GOB_JI_KUN_EXIT_DOOR            = 218888,

    /// Durumu
    GOB_DURUMU_ENTRANCE_GATE        = 218396,
    GOB_DURUMU_EXIT_GATE            = 218395,
    GOB_DURUMU_EXIT_DOOR            = 218390,

    /// Primordius
    GOB_FIRST_MOGU_BLOOD_VAT        = 218858,
    GOB_SECOND_MOGU_BLOOD_VAT       = 218859,
    GOB_THIRD_MOGU_BLOOD_VAT        = 218860,
    GOB_FOURTH_MOGU_BLOOD_VAT       = 218861,
    GOB_FIFTH_MOGU_BLOOD_VAT        = 218862,
    GOB_SIXTH_MOGU_BLOOD_VAT        = 218863,
    GOB_SEVENTH_MOGU_BLOOD_VAT      = 218864,
    GOB_EIGHTH_MOGU_BLOOD_VAT       = 218865,
    GOB_NINTH_MOGU_BLOOD_VAT        = 218866,
    GOB_TENTH_MOGU_BLOOD_VAT        = 218867,
    GOB_PRIMORDIUS_ENTRANCE         = 218584,
    GOB_PRIMORDIUS_EXIT             = 218585,

    /// Dark Animus
    GOB_DARK_ANIMUS_ENTRANCE        = 218392,
    GOB_DARK_ANIMUS_EXIT            = 218393,

    /// Iron Qon
    GOB_IRON_QON_SEWER_DOOR         = 218388,
    GOB_IRON_QON_EXIT_DOOR          = 218588,
    GOB_IRON_QON_EXIT_DOOR_2        = 218589,

    /// Twin Consorts
    GOB_TWIN_ENTRANCE               = 218781,
    GOB_TWIN_EXIT                   = 218394,
    GOB_CONCUBINES_BACK             = 218711,
    GOB_CONCUBINES_FRONT            = 218712,


    /// Lei Shen
    GOB_DISPLACEMENT_PAD            = 218417,   ///< Teleport Player to Lei Shen's room
    GOB_CHARGING_STATION            = 218397,
        /// Pillars
    GOB_NORTH_PILLAR                = 218398,
    GOB_EAST_PILLAR                 = 218399,
    GOB_SOUTH_PILLAR                = 218400,
    GOB_WEST_PILLAR                 = 218401,
        /// Electric floors
    GOB_FLOOR_NORTH                 = 218420,
    GOB_FLOOR_EAST                  = 218422,
    GOB_FLOOR_SOUTH                 = 218419,
    GOB_FLOOR_WEST                  = 218421,
        /// Windows
    GOB_SW_WINDOW                   = 218408,
    GOB_NW_WINDOW                   = 218409,
    GOB_SE_WINDOW                   = 218410,
    GOB_NE_WINDOW                   = 218411,
        /// Wind FX on floor
    GOB_WIND_NE_FX                  = 218412,
    GOB_WIND_SE_FX                  = 218413,
    GOB_WIND_SW_FX                  = 218414,
    GOB_WIND_NW_FX                  = 218415,

    // Ra-den
    GOB_RADEN_SECRET_ENTRY_IRIS     = 218553
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
    SPELL_DRAIN_THE_WEAK_HEAL       = 135102,

    /// Prevent Fall Damage from Ji-Kun's room (used by Fall Catchers to add it, and by Durumu to remove it)
    SPELL_PREVENT_FALL_DAMAGE       = 139265, ///< Reduce fall damages

    /// Arcing Energy, shared to be removed from instance script when player leaves, to avoid world spreading
    SPELL_ARCING_LIGHTNING          = 136193,
    SPELL_ARCING_PERIODIC_CHECK     = 138434,   // Triggers 138435
    SPELL_ARCING_PLAYER_CHECK       = 138435,

    /// Critically Damaged, called in InstanceScript to disabled Golems in 10 man raid
    SPELL_CRITICALLY_DAMAGED_1      = 138400,

    // 69365 - Star (used by Shanze Celestial Shaper's Cosmic Strike and Lu'lin's Cosmic Barrage)
    SPELL_LAUNCH_STAR               = 137139,   // Make Lu'lin move the stars
    SPELL_STAR_STATE                = 137138,   // Star visual
};

enum eSharedActions
{
    ACTION_MEGAERA_SPAWN,
    ACTION_SUMMON_GOLEMS      = 4
};

enum eSharedDisplayId
{
    DISPLAYID_INVISIBLE = 11686
};

enum eMiscData
{
    DATA_GOLEM_DAMAGED,
    DATA_ANIMA_LEVEL,
    DATA_WAITING_ANIMA,
    DATA_ANIMA_RECEIVE,
    DATA_COSMIC_TARGET
};

#endif // THRONE_OF_THUNDER_H_
