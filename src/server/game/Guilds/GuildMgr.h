////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _GUILDMGR_H
#define _GUILDMGR_H

#ifndef CROSS
#include "Guild.h"
#else /* CROSS */
struct InterRealmGuildEmblem
{
    public:

        InterRealmGuildEmblem(uint32 style, uint32 color, uint32 borderStyle, uint32 borderColor, uint32 backgroundColor);

        uint32 GetStyle() const { return m_Style; }
        uint32 GetColor() const { return m_Color; }
        uint32 GetBorderStyle() const { return m_BorderStyle; }
        uint32 GetBorderColor() const { return m_BorderColor; }
        uint32 GetBackgroundColor() const { return m_BackgroundColor; }

    private:

        uint32 m_Style;
        uint32 m_Color;
        uint32 m_BorderStyle;
        uint32 m_BorderColor;
        uint32 m_BackgroundColor;
};

struct InterRealmGuildRank
{
    public:

        InterRealmGuildRank(uint32 rankId, std::string& rankName);

        uint32 GetRankId() const { return m_RankId; }
        std::string GetRankName() const { return m_RankName; }

    private:
    
        uint32 m_RankId;
        std::string m_RankName;
};

typedef std::list<InterRealmGuildRank*> InterRealmGuildRanks;

class InterRealmGuild
{
    public:

        InterRealmGuild(uint64 guildGuid, std::string& guildName, uint32 membersCount, uint32 level, uint64 experience, uint32 achievementPoints, InterRealmGuildEmblem* emblem);
        ~InterRealmGuild();

        uint64 GetGUID() const { return m_GuildGUID; }
        std::string GetName() const { return m_GuildName; }
        uint32 GetMembersCount() const { return m_MembersCount; }
        uint32 GetLevel() const { return m_GuildLevel; }
        uint64 GetExperience() const { return m_Experience; }
        uint32 GetAchievementPoints() const { return m_AchievementPoints; }

        void AddRank(InterRealmGuildRank* rank);
        InterRealmGuildRanks const* GetRanks();

        InterRealmGuildEmblem const* GetEmblem();

    private:

        uint64 m_GuildGUID;
        std::string m_GuildName;
        uint32 m_MembersCount;
        uint32 m_GuildLevel;
        uint64 m_Experience;
        uint32 m_AchievementPoints;

        InterRealmGuildRanks m_Ranks;
        InterRealmGuildEmblem* m_Emblem;
};

typedef std::unordered_map<uint32, InterRealmGuild*> InterRealmGuilds;
typedef std::unordered_map<uint64, InterRealmGuilds> InterRealmGuildsContainer;
#endif /* CROSS */

class GuildMgr
{
    friend class ACE_Singleton<GuildMgr, ACE_Null_Mutex>;
private:
    GuildMgr();
    ~GuildMgr();

public:
    typedef std::unordered_map<uint32, Guild*> GuildContainer;

#ifndef CROSS
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
#else /* CROSS */
    void AddInterRealmGuild(uint32 realmId, InterRealmGuild* guild);
    void ClearInterRealmGuilds();
    InterRealmGuildsContainer const* GetInterRealmGuilds();
    InterRealmGuilds const* GetInterRealmGuildsByRealm(uint32 realmId);
    InterRealmGuild* GetInterRealmGuild(uint32 realmId, uint64 guildGuid);
    bool HasInterRealmGuild(uint32 realmId, uint64 guildGuid) const;
    bool HandleInterRealmGuildQuery(Player* player, uint32 realmId, uint64 guildGuid);
#endif /* CROSS */

protected:
#ifndef CROSS
    uint32 NextGuildId;
    GuildContainer GuildStore;
    std::vector<GuildReward> GuildRewards;
#endif /* not CROSS */

#ifdef CROSS
    InterRealmGuildsContainer m_InterRealmGuilds;
#endif /* CROSS */
};

#define sGuildMgr ACE_Singleton<GuildMgr, ACE_Null_Mutex>::instance()

#endif
