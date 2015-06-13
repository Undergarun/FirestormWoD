/*
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

#ifndef DEF_AUCHINDON_H
#define DEF_AUCHINDON_H
enum eDataBosses
{
    DATA_KATHAAR = 403,
    DATA_BOSS_NYAMI = 404,
    DATA_AZZAKAEL = 405,
    DATA_TERONOGOR = 406,
};
enum eDataCreatures
{
    DATA_NYAMI = 500,
    DATA_TUULANI = 501,
    DATA_WARDEN = 502,
    DATA_GULKOSH = 503,
    DATA_DURAG = 504,
    DATA_GROMTASH = 505,
};
enum eDataTriggers
{
    DATA_TRIGGER_MIDDLE_NYAMI_FIGHT_BUBBLE = 600,
    DATA_TRIGGER_AZZAKEL_CONTROLLER = 601,
};
enum eDataObjects
{
    DATA_LEFT_CRYSTAL = 1000,
    DATA_RIGHT_CRYSTAL = 1001,
    DATA_HOLY_BARRIER = 1002,
    DATA_AUCHINDON_WINDOW = 1003,
    DATA_FEL_BARRIER = 1004,
    DATA_FEL_PORTAL = 1005,

    // Soul Transports
    DATA_SOUL_TRANSPORT_START = 1006,
    DATA_SOUL_TRANSPORT_1 = 1007,
    DATA_SOUL_TRANSPORT_2 = 1008,
    DATA_SOUL_TRANSPORT_3 = 1009,
    DATA_SOUL_TRANSPORT_4 = 1010,
};
enum eBosses
{
    BOSS_KAATHAR = 86217,
    BOSS_NYAMI = 76177,
    BOSS_AZAAKEL = 75927,
    BOSS_TERONOGOR = 77734,
};
enum eCreature
{
    // Creatures, RP
    CREATURE_SOUL_BINDER_NYAMI = 77810,
    CREATURE_SOUL_BINDER_TUULANI = 79248,

    // Friendly
	CREATURE_AUCHENI_DEFENDER = 77693,
    CREATURE_AUCHENI_MAGUS_2 = 77694,
	CREATURE_AUCHENI_CLERIC = 77947,
	CREATURE_AUCHENI_SOUL_PRIEST = 77696,
    CREATURE_AUCHENI_ARBITER = 77695,
    CREATURE_AUCHENI_VIGILIANT = 77819,
    CREATURE_AUCHENI_SOULTENDER = 77131,
    CREATURE_AUCHENI_HOPLITE = 77948,
    CREATURE_AUCHENI_RITUALIST = 77949,
    CREATURE_AUCHENI_WARDEN = 77704,
    CREATURE_AUCHENI_MAGUS = 87075,
    CREATURE_AUCHENI_ZEALOT = 77950,

    // Nyami
    CREATURE_SARGEI_HOPLITE = 77133,
    CREATURE_SARGEREI_MAGUS = 76263,
    CREATURE_SARGERI_RITUALIST = 77130,
    CREATURE_SARGERI_MAGUS = 77694,
    CREATURE_SARGERI_ZEALOT = 77950,
    CREATURE_SARGERI_WARDEN = 77935,
    CREATURE_SARGEREI_DEFENDER = 77042,
    CREATURE_SARGERI_SOUL_PRIEST = 76595,
    CREATURE_SARGERI_SOUL_BINDER = 77812,
    CREATURE_SPITEFUL_ARBITRER_TRASH = 76284,

 
    // Azzakel
    CREATURE_FELGUARD = 76259,
    CREATURE_CACKLING_PYROMANIAC = 76260,
    CREATURE_FELBORNE_ABYSSAL = 77905,
    CREATURE_BLAZING_TRICKSTER = 79511,
    CREATURE_ZIPTEQ = 78734,
    CREATURE_DURAG_THE_DOMINATOR = 77890,
    CREATURE_SHAADUM = 78728,
    CREATURE_GULKOSH = 78437,
    CREATURE_GROMTASH_THE_DESTRUCTOR = 77889,
    CREATURE_ZASHOO = 78735,
    CREATURE_IRUUN = 77762,
    CREATURE_DUREM = 77743,
    CREATURE_JORAA = 77756,

    CREATURE_SHIELD_STALKER = 82681, 

    // azzakel event
    CREATURE_WARDEN_AZZAKAEL = 76572,
};
enum eObjects
{
    GAMEOBJECT_GHOST_OBJECT = 180322,
    GAMEOBJECT_HOLY_BARRIER = 230399,
    GAMEOBJECT_HOLY_WALL = 231680,
    GAMEOBJECT_FEL_BARRIER = 230400,
    GAMEOBJECT_TALADOR_PORTAL = 236689,
    GAMEOBJECT_CHEST_AUCHENI = 231241,
    GAMEOBJECT_NIGHTMARE_BELL = 237479,
    GAMEOBJECT_DEMONIC_PORTAL = 543242, //233102,
   // GAMEOBJECT_BLADE = 237464,
    GAMEOBJECT_PUDDLE = 237542,
    GAMEOBJECT_AUCHINDON_WINDOW = 233009,

    // Soul Transports
    GAMEOBJECT_SOUL_TRANSPORT_START = 231736,
    GAMEOBJECT_SOUL_TRANSPORT_1 = 345366,
    GAMEOBJECT_SOUL_TRANSPORT_2 = 345367,
    GAMEOBJECT_SOUL_TRANSPORT_3 = 345368,
};
enum eTriggers
{
	TRIGGER_LIGHT_WALL = 953642,
    TRIGGER_LIGHT_WALL_TARGETS = 79417,
    TRIGGER_SHADOW_BEAM = 100055,
    SHIELD_TRIGGER_VISUAL = 82681,
    TRIGGER_ARCANE_BOMB = 324156,
    TRIGGER_CORPSES_NYAMI_FIGHT = 456234,
    TRIGGER_SHIELD_SPOT = 464574,
    TRIGGER_ARCANE_BOMB_EXPLOSION_NPC = 341634,
    TRIGGER_WARDENS_HAMMER = 76655,
    TRIGGER_DEMONS_SUMMONER = 76216,
};
enum GeneralActions
{
    ACTION_COUNT_PRE_1ST_BOSS_KILL = 54342,
    ACTION_COUNT_PRE_3ST_BOSS_KILL = 54343,
    AUCTION_DEACTIVATE_FIXATION = 54344,
    ACTION_COUNT_PORTALS = 54367,

    // soul move
    ACTION_SOUL_MOVE_1 = 6000,
    ACTION_SOUL_MOVE_2 = 6001,
    ACTION_SOUL_MOVE_3 = 6002,
    ACTION_SOUL_MOVE_4 = 6003,
};
enum eTalk
{
    TUULANI_TALK_1 = 0, // Good! We must have arrived before Teron'gor and Gul'den! (44644)
    TUULANI_TALK_2 = 1, // Nyami is working to bolster the defenses against their assault - follow me,we will report to her. (44654)
    TUULANI_TALK_3 = 2,	// Beautiful, isn't it? Just think - you're one of the first outsiders to see inside the Auchindoum. (44655) 
    AUCHENAI_DEFENDER_TALK_1 = 3, // Nyami awaits you in the western ritual chamber, champions.()
    TUULANI_TALK_4 = 4, // Strange... why the door is sealed? No matter... (44656)
    TUULANI_TALK_0002 = 18, // Let us proceed (44657)
    TUULANI_TALK_5 = 5, // The Vindicators and Exrachs of the past. Heroes all - their souls still remain with us here. (44658)
    TUULANI_TALK_6 = 6, // Nyami! We are here - (44659)
    TUULANI_TALK_7 = 7, // What is this? Oh.. no.. no!
    TUULANI_TALK_8 = 8, // You have to stop them! (44661)
    TUULANI_TALK_9 = 9, // The protector still answers her call, you'll have to defeat it! (44645)
    TUULANI_TALK_10 = 10, // 44646
    TUULANI_TALK_11 = 11, // She got away.. the defences! (44647)
    TUULANI_TALK_12 = 12, // She had help.. so many of our order were corrupt! (44648)
    TUULANI_TALK_13 = 13, // I will gather any who remain loyal here, but you.. please.. go stop this madness (44649)
    TUULANI_TALK_14 = 14, // You.. you did it! (44650)
    TUULANI_TALK_15 = 15, // Here.. this is not enough to thank you for your actions, but it is what we have to give. (44651)
    TUULANI_TALK_16 = 16, // Now we can rebuild, we must rebuild. We'll never allow our souls to fall to the legion. (44652)
    TUULANI_TALK_17 = 17, // You have done more then you know, our people can never thank you enough. (44653)

    // 1st boss RP

    // Nyami
    NYAMI_TALK_1 = 21, // Yes Tuulani, now you see. (43638)
    NYAMI_TALK_2 = 22, // Your efforts were valiant, I did not think the Encorai could powered the defences with only one of the Crystals. (43639)
    NYAMI_TALK_3 = 23, // But nothing will stop my plan, nor that of my master's.. and the only thing that's on our way.. (43630)
    NYAMI_TALK_4 = 24, // Are YOU Tuulani, and your outsider friends! (43641)
    NYAMI_TALK_5 = 25, // Try as you might, this place is doomed.. it is only a matter of time. (43642)
    NYAMI_TALK_6 = 26, // Yes... yes! the Crystal harms, just a bit of focus and.. (43643)
    NYAMI_TALK_7 = 27, // It is done! as for you.. I think it is fitting that one of my tools dispatches the others! (43644)
    NYAMI_TALK_8 = 28, // Protector! destroy these intruders! (43645)
};
enum eSpellsVisuals
{
    SPELL_APPLAUD = 106301,
    EMOTE_HOVER = 533,
    EMOTE_READ = 483,
    SPELL_GUARD = 156893,
    SPELL_SHADOW_BEAM = 156862,// 174950,
    SPELL_KNEEL = 142479,
    SPELL_TUULANI_UNLOCK = 160415,
    SPELL_BARRIER_VISUAL = 160993,
    SPELL_LEVITATE_NYAMI = 136383,
    SPELL_NYAMI_ESCAPE = 153992,
    SPELL_NYAMI_EXPLODE_CRYSTAL = 153991,
    SPELL_DARK_FIRE = 156955,
    SPELL_VOID_FORM_TRIGGER_BUFF = 163715,
    SPELL_DISPERSION_VISUAL_NYAMI = 156868,
    SPELL_HOLY_BEAM = 158565,
    SPELL_TUULANI_CAPTURED_VOID_PRISON = 154840,
    SPELL_CRYSTAL_EARTHQUAKE = 150209,
    SPELL_VOID_CHANNEL = 170677,
    EMOTE_TALK = 1,
    SPELL_ARCANE_CHANNELING = 77855,
    SPELL_PRISON_AURA = 167081,
    SPELL_STRANGULATE = 78037,
    SPELL_DRAIN_SOUL_VISUAL = 156862,
    SPELL_TERONOGOR_SHIELD = 157017,
    SPELL_ABYSSAL_VISUAL = 159610,
};
enum eEvents
{
    EVENT_MIND_SPIKE = 1000,
    EVENT_SEVER_TENDOM = 10,
    EVENT_VOID_MENDING = 20,
    EVENT_HEROIC_VOID_MENDING = 30,
    EVENT_VOID_SHIFT = 40,
    EVENT_SHIELD_BASH = 50,
    EVENT_VOID_STRIKES = 60,
    EVENT_VOID_SHELL = 61,
    EVENT_MIND_SHEAR = 62,
    EVENT_BEND_WILL = 63,
    EVENT_ARCANE_BOMB = 71,
    EVENT_CRUSADER_STIRKE = 73,
    EVENT_FELBLAST = 75,
    EVENT_FEL_STOMP = 76,
    EVENT_AVENGERS_SHIELD = 98,
    EVENT_FIXATE = 79,
    EVENT_RANDOM_MOVE = 80,   
    EVENT_WARDEN_HAMMER = 82,
    EVENT_WARDEN_CHAIN = 83,
    EVENT_CONFLIGRATE = 84,
};
enum eSpells
{
    SPELL_MIND_SPIKE = 157043, // Blasts the target for 22800 to 25200 Shadowfrost damage.
    SPELL_SEVER_TENDON_DUMMY = 157167,
    SPELL_SEVER_TENDON_AURA = 157165,
    SPELL_VOID_MENDING_DUMMY = 154623, // Cast Only
    SPELL_VOID_MENDING_AURA = 165031, // Heal Buff
    SPELL_VOID_SHIFT_DUMMY = 155524,
    SPELL_VOID_SHIFT_DAMAGE = 157037,
    SPELL_SHIELD_BASH = 157159, // 	Bashes a nearby target with a shield, interrupting their current cast.
    SPELL_VOID_STRIKES = 166749, // The Hoplite's attacks inflict Shadow damage to enemies in a frontal cone.
    SPELL_VOID_SHELL = 160312,
    SPELL_MIND_SHEAR = 157170,
    SPELL_BEND_WILL = 154527,
    SPELL_ARCANE_BOMB_AREA_TRIGGER = 154174,
    SPELL_ARCANE_BOMB_DMG = 154232,
    SPELL_ARCANE_BOMB_DUMMY = 157652,
    SPELL_AVENGERS_SHIELD = 165715,

    SPELL_WARDEN_HAMMER_DUMMY = 154729,
    SPELL_WARDEN_THROW_HAMMER = 154730,
    SPELL_WARDEN_HAMMER_LIGHTNING = 154740,
    SPELL_WARDEN_HAMMER_DAMAGE = 154773,
    SPELL_WARDEN_HAMMER_LIGHTNING_TEMPO = 154775,

    SPELL_FELBLAST = 167092,
    SPELL_FEL_STOMP = 157173,  // Stomps the ground with fel-infused might.
    SPELL_CRUSADER_STIRKE = 176931,
    SPELL_FIXATE = 157168,
    SPELL_CONFLIGIRATE = 154981,
};


#endif

