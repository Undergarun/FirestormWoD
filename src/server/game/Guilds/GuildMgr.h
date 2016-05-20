////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _GUILDMGR_H
#define _GUILDMGR_H

#include "Guild.h"

class GuildMgr
{
    friend class ACE_Singleton<GuildMgr, ACE_Null_Mutex>;
private:
    GuildMgr();
    ~GuildMgr();

public:
    typedef std::unordered_map<uint32, Guild*> GuildContainer;

    Guild* GetGuildByLeader(uint64 guid) const;
    Guild* GetGuildById(uint32 guildId) const;
    Guild* GetGuildByGuid(uint64 guid) const;
    Guild* GetGuildByName(const std::string& guildName) const;
    std::string GetGuildNameById(uint32 guildId) const;

    void LoadGuildRewards();

    void LoadGuilds();
    void AddGuild(Guild* guild);
    void RemoveGuild(uint32 guildId);

    void SaveGuilds();

    void ResetReputationCaps();

    uint32 GenerateGuildId();
    void SetNextGuildId(uint32 Id) { NextGuildId = Id; }

    std::vector<GuildReward> const& GetGuildRewards() const { return GuildRewards; }

protected:
    uint32 NextGuildId;
    GuildContainer GuildStore;
    std::vector<GuildReward> GuildRewards;

};

#define sGuildMgr ACE_Singleton<GuildMgr, ACE_Null_Mutex>::instance()

#endif
