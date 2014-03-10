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
    SPELL_RITUAL_OF_PURIFICATION = 126848
};

enum eActions
{
    ACTION_START_TSULONG_WAYPOINT
};

enum eCreatures
{
    // Protectors of the Endless
    NPC_ANCIENT_ASANI           = 60586,
    NPC_ANCIENT_REGAIL          = 60585,
    NPC_PROTECTOR_KAOLAN        = 60583,
    NPC_DEFILED_GROUND          = 60906,
    NPC_COALESCED_CORRUPTION    = 60886,

    // Tsulong
    NPC_TSULONG                 = 62442,

    // Lei Shi
    NPC_LEI_SHI                 = 62983,

    // Sha of Fear
    NPC_SHA_OF_FEAR             = 60999
};

enum eGameObjects
{
    GOB_JINYU_COUNCIL_DOOR  = 214854,
    GOB_LEI_SHI_DOOR        = 214851,
    GOB_SHA_OF_FEAR_DOOR    = 214849
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
};

enum eTimers
{
	TIMER_TSULONG_SPAWN	= 10000,
};

#endif // TERRACE_OF_ENDLESS_SPRING_H_
