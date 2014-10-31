/*
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

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"

void WorldSession::HandleVoiceSessionEnableOpcode(WorldPacket & p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_VOICE_SESSION_ENABLE");
    p_Packet.rfinish();
}

void WorldSession::HandleChannelVoiceOnOpcode(WorldPacket & p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_CHANNEL_VOICE_ON");
    p_Packet.rfinish();
}

void WorldSession::HandleSetActiveVoiceChannel(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_SET_ACTIVE_VOICE_CHANNEL");
    p_Packet.rfinish();
}

