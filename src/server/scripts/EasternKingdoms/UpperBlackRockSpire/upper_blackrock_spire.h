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

#ifndef UPPER_BLACKROCK_SPIRE_H
#define UPPER_BLACKROCK_SPIRE_H

enum eDatas
{
    DATA_OREBENDER_GORASHAN,
    DATA_KYRAK_THE_CORRUPTOR,
    DATA_COMMANDER_THARBEK,
    DATA_RAGEWING_THE_UNTAMED,
    DATA_WARLORD_ZAELA,
    DATA_MAX_ENCOUNTERS,

    DATA_MAGNETS_ACHIEVEMENT,
    DATA_RUNES_DISABLED
};

enum eCreatures
{
    // Trashs mobs
    NPC_RUNE_GLOW               = 76396,
    NPC_BLACK_IRON_GRUNT        = 76179,
    NPC_BLACK_IRON_LEADBELCHER  = 76157,
    NPC_SENTRY_CANNON           = 76314,
    NPC_RAGEMAW_WORG            = 76181,
    NPC_BLACK_IRON_WARCASTER    = 76151,
    NPC_BLACK_IRON_ALCHEMIST    = 76100,
    NPC_BLACK_IRON_ENGINEER     = 76101,
    NPC_DRAKONID_MONSTROSITY    = 76018,

    // Orebender Gor'Ashan
    NPC_OREBENDER_GORASHAN      = 76413,
    NPC_BLACK_IRON_APPRENTICE   = 76773,
    NPC_RUNE_OF_POWER           = 76417,
    NPC_LIGHTNING_FIELD         = 76464,

    // Kyrak The Corruptor
    NPC_KYRAK_THE_CORRUPTOR     = 76021,
    NPC_DRAKONID_MONSTROSITY_2  = 82556
};

enum eGameObjects
{
    GOB_EMBERSEER_IN            = 175244,
    GOB_OREBENDER_ENTRANCE      = 175705,
    GOB_OREBENDER_EXIT          = 175153,

    GOB_RUNIC_CONDUIT           = 226704,

    GOB_KYRAK_EXIT_01           = 225945,
    GOB_KYRAK_EXIT_02           = 225944
};

enum eSharedSpells
{
};

enum eAchievements
{
    ACHIEV_MAGNETS_HOW_DO_THEY_WORK = 9045
};

#endif
