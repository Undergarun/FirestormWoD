////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef __TRINITY_CHANNELMGR_H
#define __TRINITY_CHANNELMGR_H

#include "Common.h"
#include "Channel.h"
#include "World.h"

class ChannelMgr
{
    public:
        uint32 team;
        typedef ACE_Based::LockedMap<std::wstring, Channel*> ChannelMap;
        ChannelMgr() {team = 0;}
        ~ChannelMgr();

        Channel* GetJoinChannel(std::string name, uint32 channel_id);
        Channel* GetChannel(std::string name, Player* p, bool pkt = true);
        void LeftChannel(std::string const& name);
    private:
        ChannelMap channels;
        void MakeNotOnPacket(WorldPacket* data, std::string name);
};

class AllianceChannelMgr : public ChannelMgr {};
class HordeChannelMgr    : public ChannelMgr {};

ChannelMgr* channelMgr(uint32 team);

#endif
