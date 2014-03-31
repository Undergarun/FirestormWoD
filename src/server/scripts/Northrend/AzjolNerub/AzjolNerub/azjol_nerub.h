/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#ifndef DEF_AZJOL_NERUB_H
#define DEF_AZJOL_NERUB_H

enum Data64
{
    DATA_KRIKTHIR_THE_GATEWATCHER,
    DATA_HADRONOX,
    DATA_ANUBARAK,
    DATA_WATCHER_GASHRA,
    DATA_WATCHER_SILTHIK,
    DATA_WATCHER_NARJIL
};

enum Data
{
    DATA_KRIKTHIR_THE_GATEWATCHER_EVENT,
    DATA_HADRONOX_EVENT,
    DATA_ANUBARAK_EVENT,
    DATA_ENGAGED,
    DATA_DENIED
};

enum CreatureIds
{
    NPC_RECLAIMER_AZAK      = 55564,
    NPC_AZJOL_ANAK_WARRIOR  = 55566,
    NPC_NERUBIAN_FLYER      = 55573,
};

const Position questPos[3] = 
{
    {551.382751f, 271.983917f, 223.45f, 4.73f}, // Reclaimer Azak
    {567.213135f, 265.146698f, 223.45f, 3.88f}, // Azjol-Anak Warrior
    {539.976807f, 267.492340f, 223.45f, 5.65f}  // Azjol-Anak Warrior
};

#endif
