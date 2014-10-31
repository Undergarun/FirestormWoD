/*
 * Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com/>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "OutdoorPvP.h"
#include "OutdoorPvPMgr.h"
#include "ScriptMgr.h"
#include "Player.h"
#include "WorldPacket.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Language.h"

#ifndef OUTDOOR_PVP_ASHRAN_H
#define OUTDOOR_PVP_ASHRAN_H

typedef std::set<uint64> GuidSet;
typedef std::map<uint64, uint32> PlayerTimerMap;

enum eAshranDatas
{
    ASHRAN_ZONE_ID              = 6941,
    ASHRAN_MAP_ID               = 1191,
    ASHRAN_NEUTRAL_MAP_ID       = 1116,
    ASHRAN_PRE_AREA_HORDE       = 7333,
    ASHRAN_PRE_AREA_ALLIANCE    = 7332,
    ASHRAN_WORLD_PVP_AREA_ID    = 24,
    BATTLEFIELD_TYPE_WORLD_PVP  = 0x20000,
    PLAYER_MIN_LEVEL            = 100,
    ASHRAN_TIME_FOR_INVITE      = 20
};

enum eAshranWorldStates
{
    WORLD_STATE_ENNEMIES_SLAIN_ALLIANCE         = 8933,
    WORLD_STATE_ENNEMIES_SLAIN_ALLIANCE_MAX     = 9801,
    WORLD_STATE_ENNEMIES_SLAIN_HORDE            = 8934,
    WORLD_STATE_ENNEMIES_SLAIN_HORDE_MAX        = 9802,
    WORLD_STATE_NEXT_BATTLE_TIMESTAMP           = 8945,
    WORLD_STATE_ACTIVE_STAGE                    = 8955,
    WORLD_STATE_RISEN_SPIRITS_CAPTURED_ALLIANCE = 9200,
    WORLD_STATE_RISEN_SPIRITS_CAPTURED_HORDE    = 9201,
    WORLD_STATE_ORE_COLLECTED_ALLIANCE          = 1581,
    WORLD_STATE_ORE_COLLECTED_HORDE             = 1582,
    WORLD_STATE_LAPS_ALLIANCE                   = 9291,
    WORLD_STATE_LAPS_HORDE                      = 9292,
    WORLD_STATE_TIME_REMAINING                  = 9316,
    WORLD_STATE_FIRES_CONTROLLED_ALLIANCE       = 9782,
    WORLD_STATE_FIRES_CONTROLLED_HORDE          = 9783,
    WORLD_STATE_FIRES_SCORING_ALLIANCE          = 9418,
    WORLD_STATE_FIRES_SCORING_HORDE             = 9419,
    WORLD_STATE_FIRES_SCORING_MAX               = 9784,
};

Position const g_HordeTeleportPos = { 5216.443359f, -3963.191406f, 5.553593f, 6.242684f };
Position const g_AllianceTeleportPos = { 3849.396240f, -4013.051025f, 26.282335f, 3.141932f };

class OutdoorPvPAshran : public OutdoorPvP
{
    public:
        OutdoorPvPAshran();

        bool SetupOutdoorPvP();

        void HandlePlayerEnterMap(Player* p_Player, uint32 p_MapID);
        void HandlePlayerLeaveMap(Player* p_Player, uint32 p_MapID);
        void HandlePlayerEnterArea(Player* p_Player, uint32 p_AreaID);
        void HandlePlayerLeaveArea(Player* p_Player, uint32 p_AreaID);

        bool Update(uint32 p_Diff);

        void FillInitialWorldStates(ByteBuffer& p_Data);
        void SendRemoveWorldStates(Player* p_Player);

        void HandleBFMGREntryInviteResponse(bool p_Accepted, Player* p_Player);

    private:
        uint64 m_Guid;
        uint32 m_WorldPvPAreaId;

        GuidSet m_PlayersInWar[BG_TEAMS_COUNT];
        PlayerTimerMap m_InvitedPlayers[BG_TEAMS_COUNT];
        PlayerTimerMap m_PlayersWillBeKick[BG_TEAMS_COUNT];
};

#endif
