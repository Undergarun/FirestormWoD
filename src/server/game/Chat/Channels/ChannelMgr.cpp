////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ChannelMgr.h"

#include "World.h"

ChannelMgr* channelMgr(uint32 team)
{
    if (sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHANNEL))
        return ACE_Singleton<AllianceChannelMgr, ACE_Null_Mutex>::instance();        // cross-faction

    if (team == ALLIANCE)
        return ACE_Singleton<AllianceChannelMgr, ACE_Null_Mutex>::instance();
    if (team == HORDE)
        return ACE_Singleton<HordeChannelMgr, ACE_Null_Mutex>::instance();

    return NULL;
}

ChannelMgr::~ChannelMgr()
{
    for (ChannelMap::iterator itr = channels.begin(); itr != channels.end(); ++itr)
        delete itr->second;

    channels.clear();
}

Channel* ChannelMgr::GetJoinChannel(std::string name, uint32 channel_id)
{
    std::wstring wname;
    Utf8toWStr(name, wname);
    wstrToLower(wname);

    if (channels.find(wname) == channels.end())
    {
        Channel* nchan = new Channel(name, channel_id, team);
        channels[wname] = nchan;
        return nchan;
    }

    return channels[wname];
}

Channel* ChannelMgr::GetChannel(std::string name, Player* p, bool pkt)
{
    std::wstring wname;
    Utf8toWStr(name, wname);
    wstrToLower(wname);

    ChannelMap::const_iterator i = channels.find(wname);

    if (i == channels.end())
    {
        if (pkt)
        {
            WorldPacket data;
            MakeNotOnPacket(&data, name);
            p->GetSession()->SendPacket(&data);
        }

        return NULL;
    }
    else
        return i->second;
}

void ChannelMgr::LeftChannel(std::string const& name)
{
    std::wstring wname;
    Utf8toWStr(name, wname);
    wstrToLower(wname);

    ChannelMap::const_iterator i = channels.find(wname);

    if (i == channels.end())
        return;

    Channel* channel = i->second;

    if (channel->GetNumPlayers() == 0 && !channel->IsConstant())
    {
        channels.erase(wname);
        delete channel;
    }
}

void ChannelMgr::MakeNotOnPacket(WorldPacket* data, std::string name)
{
    data->Initialize(SMSG_CHANNEL_NOTIFY, (1+10));  // we guess size
    (*data) << (uint8)0x05 << name;
}
