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

#ifndef TERRACE_OF_ENDLESS_SPRING_H_
#define TERRACE_OF_ENDLESS_SPRING_H_

#include "SpellScript.h"
#include "Map.h"
#include "Creature.h"
#include "CreatureAIImpl.h"

enum eData
{
    DATA_PROTECTORS,
    DATA_TSULONG,
    DATA_LEI_SHI,
    DATA_SHA_OF_FEAR,
    DATA_MAX_BOSS_DATA
};

enum eSpells
{
    SPELL_RITUAL_OF_PURIFICATION    = 126848,
    SPELL_NIGHT_PHASE_EFFECT        = 122841, ///< Tsulong night effect
};

enum eActions
{
    ACTION_START_TSULONG_WAYPOINT   = 1,
    ACTION_INTRO_FINISHED           = 2,
    ACTION_SHOW_LEI_SHI             = 30,
    ACTION_ACTIVATE_SHA_OF_FEAR     = 31,
    ACTION_SHA_OF_FEAR_STOP_ATTACK  = 32
};

enum eCreatures
{
    // Protectors of the Endless
    NPC_ANCIENT_ASANI               = 60586,
    NPC_ANCIENT_REGAIL              = 60585,
    NPC_PROTECTOR_KAOLAN            = 60583,
    NPC_DEFILED_GROUND              = 60906,
    NPC_COALESCED_CORRUPTION        = 60886,
    NPC_MINION_OF_FEAR_CONTROLLER   = 60957,
    NPC_MINION_OF_FEAR              = 60885,

    // Tsulong
    NPC_TSULONG                     = 62442,

    // Lei Shi
    NPC_LEI_SHI                     = 62983,
    NPC_ANIMATED_PROTECTOR          = 62995,
    NPC_LEI_SHI_HIDDEN              = 63099,

    // Sha of Fear
    NPC_SHA_OF_FEAR                 = 60999,
    NPC_PURE_LIGHT_TERRACE          = 60788,
    NPC_TERROR_SPAWN                = 61034,
    NPC_YANG_GUOSHI                 = 61038,
    NPC_CHENG_KANG                  = 61042,
    NPC_JINLUN_KUN                  = 61046,
    NPC_RETURN_TO_THE_TERRACE       = 65736,
    NPC_SHA_GLOBE                   = 65691,
    NPC_WATERSPOUT                  = 63823,
    NPC_DREAD_SPAWN                 = 61003,
    NPC_TRAVEL_TO_DREAD_EXPANSE     = 61735,
    NPC_RETURN_FROM_DREAD_EXPANSE   = 61746,

    // Thrashs
    NPC_APPARITION_OF_FEAR          = 64368,
    NPC_APPARITION_OF_TERROR        = 66100,
    NPC_NIGHT_TERRORS               = 64390,
    NPC_NIGHT_TERROR_SUMMON         = 64393
};

enum eGameObjects
{
    GOB_WALL_OF_COUNCILS_VORTEX = 214854,
    GOB_COUNCILS_VORTEX         = 214853,

    CACHE_OF_TSULONG_10_NM      = 215355,
    CACHE_OF_TSULONG_25_NM      = 215356,
    CACHE_OF_TSULONG_10_HM      = 215357,
    CACHE_OF_TSULONG_25_HM      = 212922,

    GOB_WALL_OF_LEI_SHI         = 214852,
    GOB_LEI_SHIS_VORTEX         = 214851,
    GOB_LEI_SHI_CHEST_NORMAL_10 = 213076,
    GOB_LEI_SHI_CHEST_NORMAL_25 = 213074,
    GOB_LEI_SHI_CHEST_HEROIC_10 = 213075,
    GOB_LEI_SHI_CHEST_HEROIC_25 = 213073
};

enum ePhases
{
};

enum eWeapons
{
};

enum eEvents
{
};

enum eTypes
{
    INTRO_DONE
};

enum eTimers
{
    TIMER_TSULONG_SPAWN	= 10000,
};

#endif // TERRACE_OF_ENDLESS_SPRING_H_
