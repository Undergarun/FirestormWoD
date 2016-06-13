////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"

void WorldSession::HandleVoiceSessionEnableOpcode(WorldPacket & p_Packet)
{
    p_Packet.rfinish();
}

void WorldSession::HandleChannelVoiceOnOpcode(WorldPacket & p_Packet)
{
    p_Packet.rfinish();
}

void WorldSession::HandleSetActiveVoiceChannel(WorldPacket& p_Packet)
{
    p_Packet.rfinish();
}

