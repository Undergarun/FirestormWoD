/*
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
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

#ifndef IRON_DOCKS_H
#define IRON_DOCKS_H

enum DataTypes
{
    DATA_NOKGAR = 0,
    DATA_MOUNT_WOLF = 600,
    DATA_OSHIR = 1,
    DATA_SKULLOC = 2,
    DATA_GRIMRAIL_MAKOGG = 19,
    DATA_GRIMRAIL_NOXX = 20,
    DATA_GRIMRAIL_DUGURU = 21,
    DATA_TURRET = 22,
    // mini bosses
    DATA_DARUNA = 4,
    DATA_GWARNOK = 5,
    DATA_OLUGAR = 6,
    // Off topic boss
    DATA_ZUGGOSH = 7,
    DATA_KORAMAR = 8,
    // DATA
    DATA_SECOND_EVENT = 9,
    DATA_THIRD_EVENT = 10,
};
enum CreatureIds
{
    // Bosses and Etc
    BOSS_FLESHRENDER_NOKGAR = 81305,// http://www.wowhead.com/npc=81305/fleshrender-nokgar
    BOSS_DREADFANG      = 81297, // http://www.wowhead.com/npc=81297/dreadfang
    BOSS_OSHIR          = 79852,// http://www.wowhead.com/npc=79852/oshir
    BOSS_SKULLOC = 83612,// http://www.wowhead.com/npc=83612/skulloc
    BOSS_GRIMRAIL = 80816,// http://www.wowhead.com/npc=80816/ahriok-dugru
    BOSS_TURRET = 84215,
    ACCESSORIES_SKULLOC_TURRET = 84030,
    NPC_ZOGGOSH       = 83616,// http://www.wowhead.com/npc=83616/zoggosh
    NPC_KORAMAR = 83613,//http://www.wowhead.com/npc=83613/koramar
    NPC_MAKOGG = 80805,// http://www.wowhead.com/npc=80805/makogg-emberblade
    NPC_NOX = 80808, //http://www.wowhead.com/npc=80808/neesa-nox
    NPC_DUGURU = 80816,

    // Boss Adds
    NPC_GROMKAR_FLAMESLINGER = 81279, // http://www.wowhead.com/npc=81279/gromkar-flameslinger
    // Mini bosses
    NPC_CHAMPION_DRUNA = 81603, // http://www.wowhead.com/npc=81603/champion-druna
    NPC_SIEGE_MASTER_OLUGAR = 83026, //http://www.wowhead.com/npc=83026/siegemaster-olugar
    NPC_PITWARDEN_GWARNOK = 84520, //http://www.wowhead.com/npc=84520/pitwarden-gwarnok
    // Trash
    NPC_GROMKAR_BATTLE_MASTER = 83025, // http://www.wowhead.com/npc=83025/gromkar-battlemaster
    NPC_GROMKAR_FOOT_SOLDIER = 85997, // http://www.wowhead.com/npc=85997/gromkar-footsoldier
    NPC_GROMKAR_INCINERATOR = 86809, // http://www.wowhead.com/npc=86809/gromkar-incinerator'
    NPC_GROMKAR_DEAD_EYE = 83028, // http://www.wowhead.com/npc=83028/gromkar-deadeye
    NPC_GROMKAR_TECHNICIAN = 83763, // http://www.wowhead.com/npc=83763/gromkar-technician
    NPC_GROMKAR_DECKHAND = 83762,// http://www.wowhead.com/npc=83762/gromkar-deckhand
    // Triggers
    TRIGGER_ARCHERY_TARGET = 79423,
    TRIGGER_GATECRASHER = 86534,
    TRIGGER_CANNON_BARRAGE_TARGET = 99657,
    TRIGGER_CANNON_BARRAGE_TARGET_2 = 100981,
    TRIGGER_CANNON_BARRAGE_TARGET_3 = 100982,
    TRIGGER_CANNON_BARRAGE_IN_BOSS_FIGHT = 354315,
    // Custom
    NPC_CHEERING_TRIGGER = 99655,
    NPC_CHEERING_TRIGGER_2 = 99656, // Used for the stunned ogor at the beginning, made it because you wanted everything to be handledi n source :)
    NPC_GROMKAR_FOOT_SOLDIER_2 = 859977, // http://www.wowhead.com/npc=85997/gromkar-footsoldier
    // MISV
    NPC_IRON_STAR = 81247, 
};
enum AchievementIds
{
    ACHIEVEMENT_MILITARSTIC_EXPANSIONIST = 9083, // http://www.wowhead.com/achievement=9083/militaristic-expansionist
};
enum GameObjectIds
{
};

#endif // IRON_DOCKS_H
