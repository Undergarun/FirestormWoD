////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "GuildMgr.h"

GuildMgr::GuildMgr()
{
#ifndef CROSS
    NextGuildId = 1;
#endif /* not CROSS */
}

GuildMgr::~GuildMgr()
{
#ifndef CROSS
    for (GuildContainer::iterator itr = GuildStore.begin(); itr != GuildStore.end(); ++itr)
        delete itr->second;
#endif /* not CROSS */
}

#ifndef CROSS
void GuildMgr::AddGuild(Guild* guild)
#else /* CROSS */
InterRealmGuildEmblem::InterRealmGuildEmblem(uint32 style, uint32 color, uint32 borderStyle, uint32 borderColor, uint32 backgroundColor) :
m_Style(style), m_Color(color), m_BorderStyle(borderStyle), m_BorderColor(borderColor), m_BackgroundColor(backgroundColor)
#endif /* CROSS */
{
#ifndef CROSS
    GuildStore[guild->GetId()] = guild;
#else /* CROSS */

#endif /* CROSS */
}

#ifndef CROSS
void GuildMgr::RemoveGuild(uint32 guildId)
#else /* CROSS */
InterRealmGuildRank::InterRealmGuildRank(uint32 rankId, std::string& rankName)
#endif /* CROSS */
{
#ifndef CROSS
    GuildStore.erase(guildId);
#else /* CROSS */
    m_RankId = rankId;
    m_RankName = rankName;
#endif /* CROSS */
}

#ifndef CROSS
void GuildMgr::SaveGuilds()
#else /* CROSS */
InterRealmGuild::InterRealmGuild(uint64 guildGuid, std::string& guildName, uint32 membersCount, uint32 level, uint64 experience, uint32 achievementPoints, InterRealmGuildEmblem* emblem)
#endif /* CROSS */
{
#ifndef CROSS
    for (GuildContainer::iterator itr = GuildStore.begin(); itr != GuildStore.end(); ++itr)
        itr->second->SaveToDB();
#else /* CROSS */
    m_GuildGUID = guildGuid;
    m_GuildName = guildName;
    m_MembersCount = membersCount;
    m_GuildLevel = level;
    m_Experience = experience;
    m_AchievementPoints = achievementPoints;

    m_Emblem = emblem;
#endif /* CROSS */
}

#ifndef CROSS
uint32 GuildMgr::GenerateGuildId()
#else /* CROSS */
InterRealmGuild::~InterRealmGuild()
#endif /* CROSS */
{
#ifndef CROSS
    if (NextGuildId >= 0xFFFFFFFE)
#else /* CROSS */
    if (m_Emblem != NULL)
#endif /* CROSS */
    {
#ifndef CROSS
        sLog->outError(LOG_FILTER_GUILD, "Guild ids overflow!! Can't continue, shutting down server. ");
        World::StopNow(ERROR_EXIT_CODE);
#else /* CROSS */
        delete m_Emblem;
        m_Emblem = NULL;
#endif /* CROSS */
    }
#ifndef CROSS
    return NextGuildId++;
}
#endif /* not CROSS */

#ifndef CROSS
// Guild collection
Guild* GuildMgr::GetGuildById(uint32 guildId) const
{
    GuildContainer::const_iterator itr = GuildStore.find(guildId);
    if (itr != GuildStore.end())
        return itr->second;
#else /* CROSS */
    for (InterRealmGuildRanks::iterator itr = m_Ranks.begin(); itr != m_Ranks.end(); ++itr)
        delete (*itr);
#endif /* CROSS */

#ifndef CROSS
    return NULL;
#else /* CROSS */
    m_Ranks.clear();
#endif /* CROSS */
}

#ifndef CROSS
Guild* GuildMgr::GetGuildByGuid(uint64 guid) const
#else /* CROSS */
void InterRealmGuild::AddRank(InterRealmGuildRank* rank)
#endif /* CROSS */
{
#ifndef CROSS
    // Full guids are only used when receiving/sending data to client
    // everywhere else guild id is used
    if (IS_GUILD(guid))
        if (uint32 guildId = GUID_LOPART(guid))
            return GetGuildById(guildId);

    return NULL;
#else /* CROSS */
    m_Ranks.push_back(rank);
#endif /* CROSS */
}

#ifndef CROSS
Guild* GuildMgr::GetGuildByName(const std::string& guildName) const
#else /* CROSS */
InterRealmGuildRanks const* InterRealmGuild::GetRanks()
#endif /* CROSS */
{
#ifndef CROSS
    std::string search = guildName;
    std::transform(search.begin(), search.end(), search.begin(), ::toupper);
    for (GuildContainer::const_iterator itr = GuildStore.begin(); itr != GuildStore.end(); ++itr)
    {
        std::string gname = itr->second->GetName();
        std::transform(gname.begin(), gname.end(), gname.begin(), ::toupper);
        if (search == gname)
            return itr->second;
    }
    return NULL;
#else /* CROSS */
    return &m_Ranks;
#endif /* CROSS */
}

#ifndef CROSS
std::string GuildMgr::GetGuildNameById(uint32 guildId) const
#else /* CROSS */
InterRealmGuildEmblem const* InterRealmGuild::GetEmblem()
#endif /* CROSS */
{
#ifndef CROSS
    if (Guild* guild = GetGuildById(guildId))
        return guild->GetName();

    return "";
#else /* CROSS */
    return m_Emblem;
#endif /* CROSS */
}

#ifndef CROSS
Guild* GuildMgr::GetGuildByLeader(uint64 guid) const
#else /* CROSS */
void GuildMgr::AddInterRealmGuild(uint32 realmId, InterRealmGuild* guild)
#endif /* CROSS */
{
#ifndef CROSS
    for (GuildContainer::const_iterator itr = GuildStore.begin(); itr != GuildStore.end(); ++itr)
        if (itr->second->GetLeaderGUID() == guid)
            return itr->second;

    return NULL;
}
#else /* CROSS */
    InterRealmGuilds& guilds = m_InterRealmGuilds[realmId];
#endif /* CROSS */

#ifndef CROSS
void GuildMgr::ResetReputationCaps()
{
    /// @TODO: Implement
#else /* CROSS */
    guilds[guild->GetGUID()] = guild;
#endif /* CROSS */
}

#ifndef CROSS

void GuildMgr::LoadGuilds()
#else /* CROSS */
void GuildMgr::ClearInterRealmGuilds()
#endif /* CROSS */
{
#ifndef CROSS
    // 1. Load all guilds
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading guilds definitions...");
    {
        uint32 oldMSTime = getMSTime();

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUILD);
        PreparedQueryResult result = CharacterDatabase.Query(stmt);

        if (!result)
        {
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 guild definitions. DB table `guild` is empty.");
            return;
        }
        else
        {
            uint32 count = 0;
            do
            {
                Field* fields = result->Fetch();
                Guild* guild = new Guild();

                if (!guild->LoadFromDB(fields))
                {
                    delete guild;
                    continue;
                }
                AddGuild(guild);

                ++count;
            }
            while (result->NextRow());

            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u guild definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        }
    }

    // 2. Load all guild ranks
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading guild ranks...");
    {
        uint32 oldMSTime = getMSTime();

        // Delete orphaned guild rank entries before loading the valid ones
        CharacterDatabase.DirectExecute("DELETE gr FROM guild_rank gr LEFT JOIN guild g ON gr.guildId = g.guildId WHERE g.guildId IS NULL");

        //                                                         0    1      2       3                4
        QueryResult result = CharacterDatabase.Query("SELECT guildid, rid, rname, rights, BankMoneyPerDay FROM guild_rank ORDER BY guildid ASC, rid ASC");

        if (!result)
        {
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 guild ranks. DB table `guild_rank` is empty.");
        }
        else
        {
            uint32 count = 0;
            do
            {
                Field* fields = result->Fetch();
                uint32 guildId = fields[0].GetUInt32();

                if (Guild* guild = GetGuildById(guildId))
                    guild->LoadRankFromDB(fields);

                ++count;
            }
            while (result->NextRow());

            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u guild ranks in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        }
    }

    // 3. Load all guild members
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading guild members...");
#else /* CROSS */
    for (InterRealmGuildsContainer::iterator itr = m_InterRealmGuilds.begin(); itr != m_InterRealmGuilds.end(); ++itr)
#endif /* CROSS */
    {
#ifndef CROSS
        uint32 oldMSTime = getMSTime();

        // Delete orphaned guild member entries before loading the valid ones
        CharacterDatabase.DirectExecute("DELETE gm FROM guild_member gm LEFT JOIN guild g ON gm.guildId = g.guildId WHERE g.guildId IS NULL");

                                                     //          0        1        2     3      4        5                   6
        QueryResult result = CharacterDatabase.Query("SELECT gm.guildid, gm.guid, rank, pnote, offnote, BankResetTimeMoney, BankRemMoney, "
                                                     //   7                  8                 9                  10                11                 12
                                                     "BankResetTimeTab0, BankRemSlotsTab0, BankResetTimeTab1, BankRemSlotsTab1, BankResetTimeTab2, BankRemSlotsTab2, "
                                                     //   13                 14                15                 16                17                 18
                                                     "BankResetTimeTab3, BankRemSlotsTab3, BankResetTimeTab4, BankRemSlotsTab4, BankResetTimeTab5, BankRemSlotsTab5, "
                                                     //   19                 20                21                 22
                                                     "BankResetTimeTab6, BankRemSlotsTab6, BankResetTimeTab7, BankRemSlotsTab7, "
                                                     //   23      24       25       26      27         28
                                                     "c.name, c.level, c.class, c.zone, c.account, c.logout_time "
                                                     "FROM guild_member gm LEFT JOIN characters c ON c.guid = gm.guid ORDER BY guildid ASC");

        if (!result)
#else /* CROSS */
        for (InterRealmGuilds::iterator itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
#endif /* CROSS */
        {
#ifndef CROSS
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 guild members. DB table `guild_member` is empty.");
        }
        else
        {
            uint32 count = 0;

            do
            {
                Field* fields = result->Fetch();
                uint32 guildId = fields[0].GetUInt32();

                if (Guild* guild = GetGuildById(guildId))
                    guild->LoadMemberFromDB(fields);

                ++count;
            }
            while (result->NextRow());

            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u guild members int %u ms", count, GetMSTimeDiffToNow(oldMSTime));
#else /* CROSS */
            delete itr2->second;
#endif /* CROSS */
        }
    }

#ifndef CROSS
    // 4. Load all guild bank tab rights
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading bank tab rights...");
    {
        uint32 oldMSTime = getMSTime();

        // Delete orphaned guild bank right entries before loading the valid ones
        CharacterDatabase.DirectExecute("DELETE gbr FROM guild_bank_right gbr LEFT JOIN guild g ON gbr.guildId = g.guildId WHERE g.guildId IS NULL");

                                                     //       0        1      2    3        4
        QueryResult result = CharacterDatabase.Query("SELECT guildid, TabId, rid, gbright, SlotPerDay FROM guild_bank_right ORDER BY guildid ASC, TabId ASC");
#else /* CROSS */
    m_InterRealmGuilds.clear();
}
#endif /* CROSS */

#ifndef CROSS
        if (!result)
        {
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 guild bank tab rights. DB table `guild_bank_right` is empty.");
        }
        else
        {
            uint32 count = 0;
            do
            {
                Field* fields = result->Fetch();
                uint32 guildId = fields[0].GetUInt32();

                if (Guild* guild = GetGuildById(guildId))
                    guild->LoadBankRightFromDB(fields);

                ++count;
            }
            while (result->NextRow());
#else /* CROSS */
InterRealmGuildsContainer const* GuildMgr::GetInterRealmGuilds()
{
    return &m_InterRealmGuilds;
}
#endif /* CROSS */

#ifndef CROSS
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u bank tab rights in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        }
    }
#else /* CROSS */
InterRealmGuilds const* GuildMgr::GetInterRealmGuildsByRealm(uint32 realmId)
{
    InterRealmGuildsContainer::const_iterator itr = m_InterRealmGuilds.find(realmId);
    if (itr == m_InterRealmGuilds.end())
        return NULL;
#endif /* CROSS */

#ifndef CROSS
    // 5. Load all event logs
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading guild event logs...");
    {
        uint32 oldMSTime = getMSTime();
#else /* CROSS */
    return &(itr->second);
}
#endif /* CROSS */

#ifndef CROSS
        CharacterDatabase.DirectPExecute("DELETE FROM guild_eventlog WHERE LogGuid > %u", sWorld->getIntConfig(CONFIG_GUILD_EVENT_LOG_COUNT));
#else /* CROSS */
InterRealmGuild* GuildMgr::GetInterRealmGuild(uint32 realmId, uint64 guildGuid)
{
    InterRealmGuildsContainer::const_iterator itr = m_InterRealmGuilds.find(realmId);
    if (itr == m_InterRealmGuilds.end())
        return NULL;
#endif /* CROSS */

#ifndef CROSS
                                                     //          0        1        2          3            4            5        6
        QueryResult result = CharacterDatabase.Query("SELECT guildid, LogGuid, EventType, PlayerGuid1, PlayerGuid2, NewRank, TimeStamp FROM guild_eventlog ORDER BY TimeStamp DESC, LogGuid DESC");
#else /* CROSS */
    InterRealmGuilds::const_iterator itr2 = itr->second.find(guildGuid);
    if (itr2 == itr->second.end())
        return NULL;
#endif /* CROSS */

#ifndef CROSS
        if (!result)
        {
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 guild event logs. DB table `guild_eventlog` is empty.");
        }
        else
        {
            uint32 count = 0;
            do
            {
                Field* fields = result->Fetch();
                uint32 guildId = fields[0].GetUInt32();

                if (Guild* guild = GetGuildById(guildId))
                    guild->LoadEventLogFromDB(fields);

                ++count;
            }
            while (result->NextRow());
#else /* CROSS */
    return itr2->second;
}
#endif /* CROSS */

#ifndef CROSS
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u guild event logs in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        }
    }
#else /* CROSS */
bool GuildMgr::HasInterRealmGuild(uint32 realmId, uint64 guildGuid) const
{
    InterRealmGuildsContainer::const_iterator itr = m_InterRealmGuilds.find(realmId);
    if (itr == m_InterRealmGuilds.end())
        return NULL;
#endif /* CROSS */

#ifndef CROSS
    // 6. Load guild achievements
    {
        PreparedQueryResult achievementResult;
        PreparedQueryResult criteriaResult;
        for (GuildContainer::const_iterator itr = GuildStore.begin(); itr != GuildStore.end(); ++itr)
        {
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUILD_ACHIEVEMENT);
            stmt->setUInt32(0, itr->first);
            achievementResult = CharacterDatabase.Query(stmt);
            stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUILD_ACHIEVEMENT_CRITERIA);
            stmt->setUInt32(0, itr->first);
            criteriaResult = CharacterDatabase.Query(stmt);
#else /* CROSS */
    return itr->second.find(guildGuid) != itr->second.end();
}
#endif /* CROSS */

#ifndef CROSS
            itr->second->GetAchievementMgr().LoadFromDB(NULL, itr->second, achievementResult, criteriaResult);
        }
    }
#else /* CROSS */
bool GuildMgr::HandleInterRealmGuildQuery(Player* p_Player, uint32 p_RealmId, uint64 p_GuildGUID)
{
    if (!IS_GUILD_GUID(p_GuildGUID))
        return false;
#endif /* CROSS */

#ifndef CROSS
    // 7. Deleting old Guild News (older than one week)
    sLog->outInfo(LOG_FILTER_GENERAL, "Deleting old Guild News");
    {
        CharacterDatabase.PQuery("DELETE FROM guild_news_log WHERE date < %u;", uint32(time(NULL) - DAY * 7));
    }
#else /* CROSS */
    InterRealmGuild* l_Guild = GetInterRealmGuild(p_RealmId, p_GuildGUID);
    if (!l_Guild)
        return false;
#endif /* CROSS */

#ifndef CROSS
    // 8. Loading Guild news
    sLog->outInfo(LOG_FILTER_GENERAL, "Loading Guild News");
    {
        for (GuildContainer::const_iterator itr = GuildStore.begin(); itr != GuildStore.end(); ++itr)
        {
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_LOAD_GUILD_NEWS);
            stmt->setInt32(0, itr->first);
            itr->second->GetNewsLog().LoadFromDB(CharacterDatabase.Query(stmt));
        }
    }
#else /* CROSS */
    WorldPacket l_Data(SMSG_QUERY_GUILD_INFO_RESPONSE, 500);
#endif /* CROSS */

#ifndef CROSS
    // 9. Validate loaded guild data
    sLog->outInfo(LOG_FILTER_GENERAL, "Validating data of loaded guilds...");
    {
        uint32 oldMSTime = getMSTime();
#else /* CROSS */
    l_Data.appendPackGUID(p_GuildGUID);
    l_Data.WriteBit(1);                 ///< hasData
    l_Data.FlushBits();
#endif /* CROSS */

#ifndef CROSS
        for (GuildContainer::iterator itr = GuildStore.begin(); itr != GuildStore.end();)
        {
            Guild* guild = itr->second;
            ++itr;
            if (guild && !guild->Validate())
                delete guild;
        }
#else /* CROSS */
    l_Data.appendPackGUID(p_GuildGUID);
    l_Data << uint32(p_RealmId);
    l_Data << uint32(l_Guild->GetRanks()->size());
    l_Data << uint32(l_Guild->GetEmblem()->GetStyle());
    l_Data << uint32(l_Guild->GetEmblem()->GetColor());
    l_Data << uint32(l_Guild->GetEmblem()->GetBorderStyle());
    l_Data << uint32(l_Guild->GetEmblem()->GetBorderColor());
    l_Data << uint32(l_Guild->GetEmblem()->GetBackgroundColor());
#endif /* CROSS */

#ifndef CROSS
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Validated data of loaded guilds in %u ms", GetMSTimeDiffToNow(oldMSTime));
    }
#else /* CROSS */
    uint32 l_I = 0;
#endif /* CROSS */

#ifndef CROSS
    // 10. Validate loaded guild challenges
    sLog->outInfo(LOG_FILTER_GENERAL, "Validating guild Challenges...");
#else /* CROSS */
    for (InterRealmGuildRanks::const_iterator itr = l_Guild->GetRanks()->begin(); itr != l_Guild->GetRanks()->end(); ++itr)
#endif /* CROSS */
    {
#ifndef CROSS
        uint32 l_OldMSTime = getMSTime();
        PreparedQueryResult l_Result = CharacterDatabase.Query(CharacterDatabase.GetPreparedStatement(CHAR_LOAD_GUILD_CHALLENGES));
#else /* CROSS */
        l_Data << uint32((*itr)->GetRankId());
        l_Data << uint32(l_I);
#endif /* CROSS */

#ifndef CROSS
        if (l_Result)
        {
            uint32 l_Count = 0;
            do
            {
                Field* l_Fields = l_Result->Fetch();
                uint32 l_GuildId = l_Fields[0].GetInt32();

                if (Guild* l_Guild = GetGuildById(l_GuildId))
                {
                    l_Guild->LoadGuildChallengesFromDB(l_Fields);
                }
                ++l_Count;
#else /* CROSS */
        l_Data.WriteBits((*itr)->GetRankName().size(), 7);
        l_Data.FlushBits();
#endif /* CROSS */

#ifndef CROSS
            } while (l_Result->NextRow());
#else /* CROSS */
        l_Data.WriteString((*itr)->GetRankName());
#endif /* CROSS */

#ifndef CROSS
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u guild challenges in %u ms", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
        }
#else /* CROSS */
        l_I++;
#endif /* CROSS */
    }

#ifndef CROSS
    /// Delete orphaned guild bank tab
    CharacterDatabase.DirectExecute("DELETE gbt FROM guild_bank_tab gbt LEFT JOIN guild g ON gbt.guildId = g.guildId WHERE g.guildId IS NULL");

    /// Delete orphan guild bank items
    CharacterDatabase.DirectExecute("DELETE gbi FROM guild_bank_item gbi LEFT JOIN guild g ON gbi.guildId = g.guildId WHERE g.guildId IS NULL");

    /// Remove log entries that exceed the number of allowed entries per guild
    CharacterDatabase.DirectPExecute("DELETE FROM guild_bank_eventlog WHERE LogGuid > %u", sWorld->getIntConfig(CONFIG_GUILD_BANK_EVENT_LOG_COUNT));
}

void GuildMgr::LoadGuildRewards()
{
    GuildRewards.clear();
    uint32 oldMSTime = getMSTime();

    //                                                  0      1         2        3        4
    QueryResult result = WorldDatabase.Query("SELECT entry, standing, racemask, price, achievement FROM guild_rewards");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 guild reward definitions. DB table `guild_rewards` is empty.");
        return;
    }
#else /* CROSS */
    l_Data.WriteBits(l_Guild->GetName().size(), 7);
    l_Data.FlushBits();
#endif /* CROSS */

#ifndef CROSS
    uint32 count = 0;
#else /* CROSS */
    l_Data.WriteString(l_Guild->GetName());
#endif /* CROSS */

#ifndef CROSS
    do
    {
        GuildReward reward;
        Field* fields = result->Fetch();
        reward.Entry = fields[0].GetUInt32();
        reward.Standing = fields[1].GetUInt8();
        reward.Racemask = fields[2].GetInt32();
        reward.Price = fields[3].GetUInt64();
        reward.AchievementId = fields[4].GetUInt32();

        if (!sObjectMgr->GetItemTemplate(reward.Entry))
        {
            sLog->outError(LOG_FILTER_SERVER_LOADING, "Guild rewards contains not existing item entry %u", reward.Entry);
            continue;
        }

        if (reward.AchievementId != 0 && (!sAchievementStore.LookupEntry(reward.AchievementId)))
        {
            sLog->outError(LOG_FILTER_SERVER_LOADING, "Guild rewards contains not existing achievement entry %u", reward.AchievementId);
            continue;
        }

        if (reward.Standing >= MAX_REPUTATION_RANK)
        {
            sLog->outError(LOG_FILTER_SERVER_LOADING, "Guild rewards contains wrong reputation standing %u, max is %u", uint32(reward.Standing), MAX_REPUTATION_RANK - 1);
            continue;
        }

        GuildRewards.push_back(reward);
        ++count;
    }
    while (result->NextRow());
#else /* CROSS */
    p_Player->GetSession()->SendPacket(&l_Data);
#endif /* CROSS */

#ifndef CROSS
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u guild reward definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
#else /* CROSS */
    return true;
#endif /* CROSS */
}
