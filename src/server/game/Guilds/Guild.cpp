////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "DatabaseEnv.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "GuildFinderMgr.h"
#include "ScriptMgr.h"
#include "Chat.h"
#include "Config.h"
#include "SocialMgr.h"
#include "Log.h"
#include "AccountMgr.h"
#include "CalendarMgr.h"
#include "WowTime.hpp"

#define MAX_GUILD_BANK_TAB_TEXT_LEN 500
#define EMBLEM_PRICE 10 * GOLD

inline uint32 _GetGuildBankTabPrice(uint8 tabId)
{
    switch (tabId)
    {
        case 0: return 100;
        case 1: return 250;
        case 2: return 500;
        case 3: return 1000;
        case 4: return 2500;
        case 5: return 5000;
        default: return 0;
    }
}

void Guild::SendCommandResult(WorldSession* p_Session, GuildCommandType p_Command, GuildCommandError p_Result, const std::string& p_Param)
{
    WorldPacket l_Data(SMSG_GUILD_COMMAND_RESULT);
    l_Data << uint32(p_Result);
    l_Data << uint32(p_Command);
    l_Data.WriteBits(p_Param.size(), 8);
    l_Data.FlushBits();
    l_Data.WriteString(p_Param);
    p_Session->SendPacket(&l_Data);
}

void Guild::SendSaveEmblemResult(WorldSession* p_Session, GuildEmblemError p_Error)
{
    WorldPacket data(SMSG_PLAYER_SAVE_GUILD_EMBLEM, 4);
    data << uint32(p_Error);
    p_Session->SendPacket(&data);
}

// LogHolder
Guild::LogHolder::~LogHolder()
{
    // Cleanup
    for (GuildLog::iterator itr = m_log.begin(); itr != m_log.end(); ++itr)
        delete (*itr);
}

// Adds event loaded from database to collection
inline void Guild::LogHolder::LoadEvent(LogEntry* entry)
{
    if (m_nextGUID == uint32(GUILD_EVENT_LOG_GUID_UNDEFINED))
        m_nextGUID = entry->GetGUID();
    m_log.push_front(entry);
}

// Adds new event happened in game.
// If maximum number of events is reached, oldest event is removed from collection.
inline void Guild::LogHolder::AddEvent(SQLTransaction& trans, LogEntry* entry)
{
    // Check max records limit
    if (m_log.size() >= m_maxRecords)
    {
        LogEntry* oldEntry = m_log.front();
        delete oldEntry;
        m_log.pop_front();
    }
    // Add event to list
    m_log.push_back(entry);
    // Save to DB
    entry->SaveToDB(trans);
}

// Writes information about all events into packet.
inline void Guild::LogHolder::WritePacket(WorldPacket& p_Data) const
{
    p_Data << uint32(m_log.size());

    for (GuildLog::const_iterator itr = m_log.begin(); itr != m_log.end(); ++itr)
        (*itr)->WritePacket(p_Data);
}

inline uint32 Guild::LogHolder::GetNextGUID()
{
    // Next guid was not initialized. It means there are no records for this holder in DB yet.
    // Start from the beginning.
    if (m_nextGUID == uint32(GUILD_EVENT_LOG_GUID_UNDEFINED))
        m_nextGUID = 0;
    else
        m_nextGUID = (m_nextGUID + 1) % m_maxRecords;
    return m_nextGUID;
}

///////////////////////////////////////////////////////////////////////////////
// EventLogEntry
void Guild::EventLogEntry::SaveToDB(SQLTransaction& trans) const
{
    PreparedStatement* stmt = NULL;

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_EVENTLOG);
    stmt->setUInt32(0, m_guildId);
    stmt->setUInt32(1, m_guid);
    CharacterDatabase.ExecuteOrAppend(trans, stmt);

    uint8 index = 0;
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GUILD_EVENTLOG);
    stmt->setUInt32(  index, m_guildId);
    stmt->setUInt32(++index, m_guid);
    stmt->setUInt8 (++index, uint8(m_eventType));
    stmt->setUInt32(++index, m_playerGuid1);
    stmt->setUInt32(++index, m_playerGuid2);
    stmt->setUInt8 (++index, m_newRank);
    stmt->setUInt64(++index, m_timestamp);
    CharacterDatabase.ExecuteOrAppend(trans, stmt);
}

void Guild::EventLogEntry::WritePacket(WorldPacket& p_Data) const
{
    uint64 l_PlayerGUID = MAKE_NEW_GUID(m_playerGuid1, 0, HIGHGUID_PLAYER);
    uint64 l_OtherGUID = MAKE_NEW_GUID(m_playerGuid2, 0, HIGHGUID_PLAYER);

    p_Data.appendPackGUID(l_PlayerGUID);
    p_Data.appendPackGUID(l_OtherGUID);
    p_Data << uint8(m_eventType);
    p_Data << uint8(m_newRank);
    p_Data << uint32(::time(NULL) - m_timestamp);
}

///////////////////////////////////////////////////////////////////////////////
// BankEventLogEntry
void Guild::BankEventLogEntry::SaveToDB(SQLTransaction& trans) const
{
    PreparedStatement* stmt = NULL;
    uint8 index = 0;

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_BANK_EVENTLOG);
    stmt->setUInt32(  index, m_guildId);
    stmt->setUInt32(++index, m_guid);
    stmt->setUInt8 (++index, m_bankTabId);
    CharacterDatabase.ExecuteOrAppend(trans, stmt);

    index = 0;
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GUILD_BANK_EVENTLOG);
    stmt->setUInt32(  index, m_guildId);
    stmt->setUInt32(++index, m_guid);
    stmt->setUInt8 (++index, m_bankTabId);
    stmt->setUInt8 (++index, uint8(m_eventType));
    stmt->setUInt32(++index, m_playerGuid);
    stmt->setUInt32(++index, m_itemOrMoney);
    stmt->setUInt16(++index, m_itemStackCount);
    stmt->setUInt8 (++index, m_destTabId);
    stmt->setUInt64(++index, m_timestamp);
    CharacterDatabase.ExecuteOrAppend(trans, stmt);
}

void Guild::BankEventLogEntry::WritePacket(WorldPacket & p_Data) const
{
    uint64 l_PlayerGUID = MAKE_NEW_GUID(m_playerGuid, 0, HIGHGUID_PLAYER);

    bool l_HasItemID = m_eventType == GUILD_BANK_LOG_DEPOSIT_ITEM || m_eventType == GUILD_BANK_LOG_WITHDRAW_ITEM ||
                   m_eventType == GUILD_BANK_LOG_MOVE_ITEM || m_eventType == GUILD_BANK_LOG_MOVE_ITEM2;

    bool l_HasOtherTab = (m_eventType == GUILD_BANK_LOG_MOVE_ITEM || m_eventType == GUILD_BANK_LOG_MOVE_ITEM2);

    bool l_HasCount = (l_HasItemID && m_itemStackCount > 1);

    p_Data.appendPackGUID(l_PlayerGUID);                    ///< Player GUID
    p_Data << uint32(time(NULL) - m_timestamp);             ///< Time Offset
    p_Data << uint8(m_eventType);                           ///< Entry Type

    p_Data.WriteBit(IsMoneyEvent());                        ///< Has Money
    p_Data.WriteBit(l_HasItemID);                           ///< Has Item ID
    p_Data.WriteBit(l_HasCount);                            ///< Has Count
    p_Data.WriteBit(l_HasOtherTab);                         ///< Has Other Tab
    p_Data.FlushBits();

    if (IsMoneyEvent())
        p_Data << uint64(m_itemOrMoney);                    ///< Money

    if (l_HasItemID)
        p_Data << uint32(m_itemOrMoney);                    ///< Item ID

    if (l_HasCount)
        p_Data << uint32(m_itemStackCount);                 ///< Count

    if (l_HasOtherTab)
        p_Data << uint8(m_destTabId);                       ///< Other Tab
}

///////////////////////////////////////////////////////////////////////////////
// RankInfo
void Guild::RankInfo::LoadFromDB(Field* fields)
{
    m_rankId            = fields[1].GetUInt8();
    m_name              = fields[2].GetString();
    m_rights            = fields[3].GetUInt32();
    m_bankMoneyPerDay   = fields[4].GetUInt32();
    if (m_rankId == GR_GUILDMASTER)                     // Prevent loss of leader rights
        m_rights |= GR_RIGHT_ALL;
}

void Guild::RankInfo::SaveToDB(SQLTransaction& trans) const
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GUILD_RANK);
    stmt->setUInt32(0, m_guildId);
    stmt->setUInt8 (1, m_rankId);
    stmt->setString(2, m_name);
    stmt->setUInt32(3, m_rights);
    CharacterDatabase.ExecuteOrAppend(trans, stmt);
}

void Guild::RankInfo::UpdateId(uint32 newId)
{
    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GUILD_MEMBERS_RANK);
    stmt->setUInt8 (0, newId);
    stmt->setUInt32(1, m_guildId);
    stmt->setUInt8 (2, m_rankId);
    CharacterDatabase.ExecuteOrAppend(trans, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GUILD_RANK_ID);
    stmt->setUInt8 (0, newId);
    stmt->setUInt32(1, m_guildId);
    stmt->setUInt8 (2, m_rankId);
    CharacterDatabase.ExecuteOrAppend(trans, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GUILD_BANK_RIGHTS_ID);
    stmt->setUInt8 (0, newId);
    stmt->setUInt32(1, m_guildId);
    stmt->setUInt8 (2, m_rankId);
    CharacterDatabase.ExecuteOrAppend(trans, stmt);

    CharacterDatabase.CommitTransaction(trans);

    SetId(newId);
}

void Guild::RankInfo::SetName(const std::string& name)
{
    if (m_name == name)
        return;

    m_name = name;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GUILD_RANK_NAME);
    stmt->setString(0, m_name);
    stmt->setUInt8 (1, m_rankId);
    stmt->setUInt32(2, m_guildId);
    CharacterDatabase.Execute(stmt);
}

void Guild::RankInfo::SetRights(uint32 rights)
{
    if (m_rankId == GR_GUILDMASTER)                     // Prevent loss of leader rights
        rights = GR_RIGHT_ALL;

    if (m_rights == rights)
        return;

    m_rights = rights;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GUILD_RANK_RIGHTS);
    stmt->setUInt32(0, m_rights);
    stmt->setUInt8 (1, m_rankId);
    stmt->setUInt32(2, m_guildId);
    CharacterDatabase.Execute(stmt);
}

void Guild::RankInfo::SetBankMoneyPerDay(uint32 money)
{
    if (m_rankId == GR_GUILDMASTER)                     // Prevent loss of leader rights
        money = uint32(GUILD_WITHDRAW_MONEY_UNLIMITED);

    if (m_bankMoneyPerDay == money)
        return;

    m_bankMoneyPerDay = money;

    PreparedStatement* stmt = NULL;
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GUILD_RANK_BANK_MONEY);
    stmt->setUInt32(0, money);
    stmt->setUInt8 (1, m_rankId);
    stmt->setUInt32(2, m_guildId);
    CharacterDatabase.Execute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GUILD_RANK_BANK_RESET_TIME);
    stmt->setUInt32(0, m_guildId);
    stmt->setUInt8 (1, m_rankId);
    CharacterDatabase.Execute(stmt);
}

void Guild::RankInfo::SetBankTabSlotsAndRights(uint8 tabId, GuildBankRightsAndSlots rightsAndSlots, bool saveToDB)
{
    if (m_rankId == GR_GUILDMASTER)                     // Prevent loss of leader rights
        rightsAndSlots.SetGuildMasterValues();

    if (m_bankTabRightsAndSlots[tabId].IsEqual(rightsAndSlots))
        return;

    m_bankTabRightsAndSlots[tabId] = rightsAndSlots;

    if (saveToDB)
    {
        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        PreparedStatement* stmt = NULL;

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_BANK_RIGHT);
        stmt->setUInt32(0, m_guildId);
        stmt->setUInt8 (1, tabId);
        stmt->setUInt8 (2, m_rankId);
        trans->Append(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GUILD_BANK_RIGHT);
        stmt->setUInt32(0, m_guildId);
        stmt->setUInt8 (1, tabId);
        stmt->setUInt8 (2, m_rankId);
        stmt->setUInt8 (3, m_bankTabRightsAndSlots[tabId].rights);
        stmt->setUInt32(4, m_bankTabRightsAndSlots[tabId].slots);
        trans->Append(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CharacterDatabaseStatements(CHAR_UPD_GUILD_RANK_BANK_TIME0 + tabId));
        stmt->setUInt32(0, m_guildId);
        stmt->setUInt8 (1, m_rankId);
        trans->Append(stmt);

        CharacterDatabase.CommitTransaction(trans);
    }
}

///////////////////////////////////////////////////////////////////////////////
// BankTab
bool Guild::BankTab::LoadFromDB(Field* fields)
{
    m_name = fields[2].GetString();
    m_icon = fields[3].GetString();
    m_text = fields[4].GetString();
    return true;
}

bool Guild::BankTab::LoadItemFromDB(Field* fields)
{
    uint8 slotId = fields[18].GetUInt8();
    uint32 itemGuid = fields[19].GetUInt32();
    uint32 itemEntry = fields[20].GetUInt32();
    if (slotId >= GUILD_BANK_MAX_SLOTS)
    {
        sLog->outError(LOG_FILTER_GUILD, "Invalid slot for item (GUID: %u, id: %u) in guild bank, skipped.", itemGuid, itemEntry);
        return false;
    }

    ItemTemplate const* proto = sObjectMgr->GetItemTemplate(itemEntry);
    if (!proto)
    {
        sLog->outError(LOG_FILTER_GUILD, "Unknown item (GUID: %u, id: %u) in guild bank, skipped.", itemGuid, itemEntry);
        return false;
    }

    Item* pItem = NewItemOrBag(proto);
    if (!pItem->LoadFromDB(itemGuid, 0, fields, itemEntry))
    {
        sLog->outError(LOG_FILTER_GUILD, "Item (GUID %u, id: %u) not found in item_instance, deleting from guild bank!", itemGuid, itemEntry);

        PreparedStatement *stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_NONEXISTENT_GUILD_BANK_ITEM);
        stmt->setUInt32(0, m_guildId);
        stmt->setUInt8 (1, m_tabId);
        stmt->setUInt8 (2, slotId);
        CharacterDatabase.Execute(stmt);

        delete pItem;
        return false;
    }

    pItem->AddToWorld();
    m_items[slotId] = pItem;
    return true;
}

// Deletes contents of the tab from the world (and from DB if necessary)
void Guild::BankTab::Delete(SQLTransaction& trans, bool removeItemsFromDB)
{
    for (uint8 slotId = 0; slotId < GUILD_BANK_MAX_SLOTS; ++slotId)
        if (Item* pItem = m_items[slotId])
        {
            pItem->RemoveFromWorld();
            if (removeItemsFromDB)
                pItem->DeleteFromDB(trans);
            delete pItem;
            pItem = NULL;
        }
}

void Guild::BankTab::SetInfo(const std::string& name, const std::string& icon)
{
    if (m_name == name && m_icon == icon)
        return;

    m_name = name;
    m_icon = icon;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GUILD_BANK_TAB_INFO);
    stmt->setString(0, m_name);
    stmt->setString(1, m_icon);
    stmt->setUInt32(2, m_guildId);
    stmt->setUInt8 (3, m_tabId);
    CharacterDatabase.Execute(stmt);
}

void Guild::BankTab::SetText(const std::string& text)
{
    if (m_text == text)
        return;

    m_text = text;
    utf8truncate(m_text, MAX_GUILD_BANK_TAB_TEXT_LEN);          // DB and client size limitation

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GUILD_BANK_TAB_TEXT);
    stmt->setString(0, m_text);
    stmt->setUInt32(1, m_guildId);
    stmt->setUInt8 (2, m_tabId);
    CharacterDatabase.Execute(stmt);
}

// Sets/removes contents of specified slot.
// If pItem == NULL contents are removed.
bool Guild::BankTab::SetItem(SQLTransaction& trans, uint8 slotId, Item* item)
{
    if (slotId >= GUILD_BANK_MAX_SLOTS)
        return false;

    m_items[slotId] = item;

    PreparedStatement* stmt = NULL;

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_BANK_ITEM);
    stmt->setUInt32(0, m_guildId);
    stmt->setUInt8 (1, m_tabId);
    stmt->setUInt8 (2, slotId);
    CharacterDatabase.ExecuteOrAppend(trans, stmt);

    if (item)
    {
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GUILD_BANK_ITEM);
        stmt->setUInt32(0, m_guildId);
        stmt->setUInt8 (1, m_tabId);
        stmt->setUInt8 (2, slotId);
        stmt->setUInt32(3, item->GetGUIDLow());
        CharacterDatabase.ExecuteOrAppend(trans, stmt);

        item->SetGuidValue(ITEM_FIELD_CONTAINED_IN, 0);
        item->SetGuidValue(ITEM_FIELD_OWNER, 0);
        item->FSetState(ITEM_NEW);
        item->SaveToDB(trans);                                 // Not in inventory and can be saved standalone
    }
    return true;
}

void Guild::BankTab::SendText(const Guild * p_Guild, WorldSession* p_Session) const
{
    WorldPacket l_Data(SMSG_GUILD_BANK_QUERY_TEXT_RESULT, 500);

    l_Data << uint32(m_tabId);
    l_Data.WriteBits(m_text.size(), 14);
    l_Data.FlushBits();
    l_Data.WriteString(m_text);

    if (p_Session)
        p_Session->SendPacket(&l_Data);
    else
        p_Guild->BroadcastPacket(&l_Data);
}

///////////////////////////////////////////////////////////////////////////////
// Member
void Guild::Member::SetStats(Player* player)
{
    m_name      = player->GetName();
    m_level     = player->getLevel();
    m_class     = player->getClass();
    m_zoneId    = player->GetZoneId();
    m_accountId = player->GetSession()->GetAccountId();
}

void Guild::Member::SetStats(const std::string& name, uint8 level, uint8 _class, uint32 zoneId, uint32 accountId)
{
    m_name      = name;
    m_level     = level;
    m_class     = _class;
    m_zoneId    = zoneId;
    m_accountId = accountId;
}

void Guild::Member::SetPublicNote(const std::string& publicNote)
{
    if (m_publicNote == publicNote)
        return;

    m_publicNote = publicNote;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GUILD_MEMBER_PNOTE);
    stmt->setString(0, publicNote);
    stmt->setUInt32(1, GUID_LOPART(m_guid));
    CharacterDatabase.Execute(stmt);
}

void Guild::Member::SetOfficerNote(const std::string& officerNote)
{
    if (m_officerNote == officerNote)
        return;

    m_officerNote = officerNote;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GUILD_MEMBER_OFFNOTE);
    stmt->setString(0, officerNote);
    stmt->setUInt32(1, GUID_LOPART(m_guid));
    CharacterDatabase.Execute(stmt);
}

void Guild::Member::ChangeRank(uint8 newRank)
{
    m_rankId = newRank;

    // Update rank information in player's field, if he is online.
    if (Player* player = FindPlayer())
        player->SetRank(newRank);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GUILD_MEMBER_RANK);
    stmt->setUInt8 (0, newRank);
    stmt->setUInt32(1, GUID_LOPART(m_guid));
    CharacterDatabase.Execute(stmt);
}

void Guild::Member::SaveToDB(SQLTransaction& trans) const
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GUILD_MEMBER);
    stmt->setUInt32(0, m_guildId);
    stmt->setUInt32(1, GUID_LOPART(m_guid));
    stmt->setUInt8 (2, m_rankId);
    stmt->setString(3, m_publicNote);
    stmt->setString(4, m_officerNote);
    CharacterDatabase.ExecuteOrAppend(trans, stmt);
}

// Loads member's data from database.
// If member has broken fields (level, class) returns false.
// In this case member has to be removed from guild.
bool Guild::Member::LoadFromDB(Field* fields)
{
    m_publicNote    = fields[3].GetString();
    m_officerNote   = fields[4].GetString();
    m_WithdrawMoneyReset = fields[5].GetUInt32();
    m_WithdrawMoneyValue = fields[6].GetUInt32();
    for (uint8 i = 0; i < GUILD_BANK_MAX_TABS; ++i)
    {
        m_bankRemaining[i].resetTime                = fields[7 + i * 2].GetUInt32();
        m_bankRemaining[i].value                    = fields[8 + i * 2].GetUInt32();
    }

    SetStats(fields[23].GetString(),
             fields[24].GetUInt8(),                         // characters.level
             fields[25].GetUInt8(),                         // characters.class
             fields[26].GetUInt16(),                        // characters.zone
             fields[27].GetUInt32());                       // characters.account
    m_logoutTime    = fields[28].GetUInt32();               // characters.logout_time

    if (!CheckStats())
        return false;

    if (!m_zoneId)
    {
        sLog->outError(LOG_FILTER_GUILD, "Player (GUID: %u) has broken zone-data", GUID_LOPART(m_guid));
        m_zoneId = Player::GetZoneIdFromDB(m_guid);
    }
    return true;
}

// Validate player fields. Returns false if corrupted fields are found.
bool Guild::Member::CheckStats() const
{
    if (m_level < 1)
    {
        sLog->outError(LOG_FILTER_GUILD, "Player (GUID: %u) has a broken data in field `characters`.`level`, deleting him from guild!", GUID_LOPART(m_guid));
        return false;
    }
    if (m_class < CLASS_WARRIOR || m_class >= MAX_CLASSES)
    {
        sLog->outError(LOG_FILTER_GUILD, "Player (GUID: %u) has a broken data in field `characters`.`class`, deleting him from guild!", GUID_LOPART(m_guid));
        return false;
    }
    return true;
}

// Decrease remaining items amount for specified tab.
void Guild::Member::DecreaseBankRemainingValue(SQLTransaction& trans, uint8 tabId, uint32 amount)
{
    m_bankRemaining[tabId].value -= amount;

    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CharacterDatabaseStatements(CHAR_UPD_GUILD_MEMBER_BANK_REM_SLOTS0 + tabId));
    l_Statement->setUInt32(0, m_bankRemaining[tabId].value);
    l_Statement->setUInt32(1, m_guildId);
    l_Statement->setUInt32(2, GUID_LOPART(m_guid));
    CharacterDatabase.ExecuteOrAppend(trans, l_Statement);
}

void Guild::Member::DecreaseRemainingMoneyValue(SQLTransaction& p_Transaction, uint64 p_Amount)
{
    m_WithdrawMoneyValue -= p_Amount;

    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GUILD_MEMBER_BANK_REM_MONEY);
    l_Statement->setUInt32(0, m_WithdrawMoneyValue);
    l_Statement->setUInt32(1, m_guildId);
    l_Statement->setUInt32(2, GUID_LOPART(m_guid));
    CharacterDatabase.ExecuteOrAppend(p_Transaction, l_Statement);
}

// Return remaining items amount for specified tab.
// If reset time was more than 24 hours ago, renew reset time and reset amount to maximum value.
uint32 Guild::Member::GetBankRemainingValue(uint8 tabId, const Guild* guild) const
{
    // Guild master has unlimited amount.
    if (IsRank(GR_GUILDMASTER))
        return GUILD_WITHDRAW_SLOT_UNLIMITED;

    // Check rights for non-money tab.
    if (tabId != GUILD_BANK_MAX_TABS)
        if ((guild->_GetRankBankTabRights(m_rankId, tabId) & GUILD_BANK_RIGHT_VIEW_TAB) != GUILD_BANK_RIGHT_VIEW_TAB)
            return 0;

    uint32 curTime = uint32(::time(NULL) / MINUTE); // minutes
    if (curTime > m_bankRemaining[tabId].resetTime + 24 * HOUR / MINUTE)
    {
        RemainingValue& rv = const_cast <RemainingValue&> (m_bankRemaining[tabId]);
        rv.resetTime = curTime;
        rv.value = guild->_GetRankBankTabSlotsPerDay(m_rankId, tabId);

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CharacterDatabaseStatements(CHAR_UPD_GUILD_MEMBER_BANK_TIME_REM_SLOTS0 + tabId));
        stmt->setUInt32(0, m_bankRemaining[tabId].resetTime);
        stmt->setUInt32(1, m_bankRemaining[tabId].value);
        stmt->setUInt32(2, m_guildId);
        stmt->setUInt32(3, GUID_LOPART(m_guid));
        CharacterDatabase.Execute(stmt);
    }
    return m_bankRemaining[tabId].value;
}

inline void Guild::Member::ResetTabTimes()
{
    for (uint8 tabId = 0; tabId < GUILD_BANK_MAX_TABS; ++tabId)
        m_bankRemaining[tabId].resetTime = 0;
}

inline void Guild::Member::ResetMoneyTime()
{
    m_WithdrawMoneyReset = 0;
}

///////////////////////////////////////////////////////////////////////////////
// EmblemInfo
void EmblemInfo::LoadFromDB(Field* fields)
{
    m_style             = fields[3].GetUInt8();
    m_color             = fields[4].GetUInt8();
    m_borderStyle       = fields[5].GetUInt8();
    m_borderColor       = fields[6].GetUInt8();
    m_backgroundColor   = fields[7].GetUInt8();
}

void EmblemInfo::WritePacket(WorldPacket& data) const
{
    data << uint32(m_style);
    data << uint32(m_color);
    data << uint32(m_borderStyle);
    data << uint32(m_borderColor);
    data << uint32(m_backgroundColor);
}

void EmblemInfo::SaveToDB(uint32 guildId) const
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GUILD_EMBLEM_INFO);
    stmt->setUInt32(0, m_style);
    stmt->setUInt32(1, m_color);
    stmt->setUInt32(2, m_borderStyle);
    stmt->setUInt32(3, m_borderColor);
    stmt->setUInt32(4, m_backgroundColor);
    stmt->setUInt32(5, guildId);
    CharacterDatabase.Execute(stmt);
}

///////////////////////////////////////////////////////////////////////////////
// MoveItemData
bool Guild::MoveItemData::CheckItem(uint32& splitedAmount)
{
    ASSERT(m_pItem);
    if (splitedAmount > m_pItem->GetCount())
        return false;
    if (splitedAmount == m_pItem->GetCount())
        splitedAmount = 0;
    return true;
}

bool Guild::MoveItemData::CanStore(Item* pItem, bool swap, bool sendError)
{
    m_vec.clear();
    InventoryResult msg = CanStore(pItem, swap);
    if (sendError && msg != EQUIP_ERR_OK)
        m_pPlayer->SendEquipError(msg, pItem);
    return (msg == EQUIP_ERR_OK);
}

bool Guild::MoveItemData::CloneItem(uint32 count)
{
    ASSERT(m_pItem);
    m_pClonedItem = m_pItem->CloneItem(count);
    if (!m_pClonedItem)
    {
        m_pPlayer->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, m_pItem);
        return false;
    }
    return true;
}

void Guild::MoveItemData::LogAction(MoveItemData* pFrom) const
{
    ASSERT(pFrom->GetItem());

    sScriptMgr->OnGuildItemMove(m_pGuild, m_pPlayer, pFrom->GetItem(),
        pFrom->IsBank(), pFrom->GetContainer(), pFrom->GetSlotId(),
        IsBank(), GetContainer(), GetSlotId());
}

inline void Guild::MoveItemData::CopySlots(SlotIds& ids) const
{
    for (ItemPosCountVec::const_iterator itr = m_vec.begin(); itr != m_vec.end(); ++itr)
        ids.insert(uint8(itr->pos));
}

///////////////////////////////////////////////////////////////////////////////
// PlayerMoveItemData
bool Guild::PlayerMoveItemData::InitItem()
{
    m_pItem = m_pPlayer->GetItemByPos(m_container, m_slotId);
    if (m_pItem)
    {
        // Anti-WPE protection. Do not move non-empty bags to bank.
        if (m_pItem->IsNotEmptyBag())
        {
            m_pPlayer->SendEquipError(EQUIP_ERR_DESTROY_NONEMPTY_BAG, m_pItem);
            m_pItem = NULL;
        }
        // Bound items cannot be put into bank.
        else if (!m_pItem->CanBeTraded())
        {
            m_pPlayer->SendEquipError(EQUIP_ERR_CANT_SWAP, m_pItem);
            m_pItem = NULL;
        }
    }
    return (m_pItem != NULL);
}

void Guild::PlayerMoveItemData::RemoveItem(SQLTransaction& trans, MoveItemData* /*pOther*/, uint32 splitedAmount)
{
    if (splitedAmount)
    {
        m_pItem->SetCount(m_pItem->GetCount() - splitedAmount);
        m_pItem->SetState(ITEM_CHANGED, m_pPlayer);
        m_pPlayer->SaveInventoryAndGoldToDB(trans);
    }
    else
    {
        m_pPlayer->MoveItemFromInventory(m_container, m_slotId, true);
        m_pItem->DeleteFromInventoryDB(trans);
        m_pItem = NULL;
    }
}

Item* Guild::PlayerMoveItemData::StoreItem(SQLTransaction& trans, Item* pItem)
{
    ASSERT(pItem);
    m_pPlayer->MoveItemToInventory(m_vec, pItem, true);
    m_pPlayer->SaveInventoryAndGoldToDB(trans);
    return pItem;
}

void Guild::PlayerMoveItemData::LogBankEvent(SQLTransaction& trans, MoveItemData* pFrom, uint32 count) const
{
    ASSERT(pFrom);
    // Bank -> Char
    m_pGuild->_LogBankEvent(trans, GUILD_BANK_LOG_WITHDRAW_ITEM, pFrom->GetContainer(), m_pPlayer->GetGUIDLow(),
        pFrom->GetItem()->GetEntry(), count);
}

inline InventoryResult Guild::PlayerMoveItemData::CanStore(Item* pItem, bool swap)
{
    return m_pPlayer->CanStoreItem(m_container, m_slotId, m_vec, pItem, swap);
}

///////////////////////////////////////////////////////////////////////////////
// BankMoveItemData
bool Guild::BankMoveItemData::InitItem()
{
    m_pItem = m_pGuild->_GetItem(m_container, m_slotId);
    return (m_pItem != NULL);
}

bool Guild::BankMoveItemData::HasStoreRights(MoveItemData* pOther) const
{
    ASSERT(pOther);
    // Do not check rights if item is being swapped within the same bank tab
    if (pOther->IsBank() && pOther->GetContainer() == m_container)
        return true;
    return m_pGuild->_MemberHasTabRights(m_pPlayer->GetGUID(), m_container, GUILD_BANK_RIGHT_DEPOSIT_ITEM);
}

bool Guild::BankMoveItemData::HasWithdrawRights(MoveItemData* pOther) const
{
    ASSERT(pOther);
    // Do not check rights if item is being swapped within the same bank tab
    if (pOther->IsBank() && pOther->GetContainer() == m_container)
        return true;
    return (m_pGuild->_GetMemberRemainingSlots(m_pPlayer->GetGUID(), m_container) != 0);
}

void Guild::BankMoveItemData::RemoveItem(SQLTransaction& trans, MoveItemData* pOther, uint32 splitedAmount)
{
    ASSERT(m_pItem);
    if (splitedAmount)
    {
        m_pItem->SetCount(m_pItem->GetCount() - splitedAmount);
        m_pItem->FSetState(ITEM_CHANGED);
        m_pItem->SaveToDB(trans);
    }
    else
    {
        m_pGuild->_RemoveItem(trans, m_container, m_slotId);
        m_pItem = NULL;
    }
    // Decrease amount of player's remaining items (if item is moved to different tab or to player)
    if (!pOther->IsBank() || pOther->GetContainer() != m_container)
        m_pGuild->_DecreaseMemberRemainingSlots(trans, m_pPlayer->GetGUID(), m_container);
}

Item* Guild::BankMoveItemData::StoreItem(SQLTransaction& trans, Item* pItem)
{
    if (!pItem)
        return NULL;

    BankTab* pTab = m_pGuild->GetBankTab(m_container);
    if (!pTab)
        return NULL;

    Item* pLastItem = pItem;
    for (ItemPosCountVec::const_iterator itr = m_vec.begin(); itr != m_vec.end(); )
    {
        ItemPosCount pos(*itr);
        ++itr;

        sLog->outDebug(LOG_FILTER_GUILD, "GUILD STORAGE: StoreItem tab = %u, slot = %u, item = %u, count = %u",
            m_container, m_slotId, pItem->GetEntry(), pItem->GetCount());
        pLastItem = _StoreItem(trans, pTab, pItem, pos, itr != m_vec.end());
    }
    return pLastItem;
}

void Guild::BankMoveItemData::LogBankEvent(SQLTransaction& trans, MoveItemData* pFrom, uint32 count) const
{
    ASSERT(pFrom->GetItem());
    if (pFrom->IsBank())
        // Bank -> Bank
        m_pGuild->_LogBankEvent(trans, GUILD_BANK_LOG_MOVE_ITEM, pFrom->GetContainer(), m_pPlayer->GetGUIDLow(),
            pFrom->GetItem()->GetEntry(), count, m_container);
    else
        // Char -> Bank
        m_pGuild->_LogBankEvent(trans, GUILD_BANK_LOG_DEPOSIT_ITEM, m_container, m_pPlayer->GetGUIDLow(),
            pFrom->GetItem()->GetEntry(), count);
}

void Guild::BankMoveItemData::LogAction(MoveItemData* pFrom) const
{
    MoveItemData::LogAction(pFrom);
    if (!pFrom->IsBank() && sWorld->getBoolConfig(CONFIG_GM_LOG_TRADE) && !AccountMgr::IsPlayerAccount(m_pPlayer->GetSession()->GetSecurity()))       // TODO: move to scripts
        sLog->outCommand(m_pPlayer->GetSession()->GetAccountId(), "", m_pPlayer->GetGUIDLow(), m_pPlayer->GetName(), 0, "", 0, "",
                        "GM %s (Account: %u) deposit item: %s (Entry: %d Count: %u) to guild bank (Guild ID: %u)",
                        m_pPlayer->GetName(), m_pPlayer->GetSession()->GetAccountId(), pFrom->GetItem()->GetTemplate()->Name1->Get(sWorld->GetDefaultDbcLocale()), pFrom->GetItem()->GetEntry(), pFrom->GetItem()->GetCount(), m_pGuild->GetId());
}

Item* Guild::BankMoveItemData::_StoreItem(SQLTransaction& trans, BankTab* pTab, Item* pItem, ItemPosCount& pos, bool clone) const
{
    uint8 slotId = uint8(pos.pos);
    uint32 count = pos.count;
    if (Item* pItemDest = pTab->GetItem(slotId))
    {
        pItemDest->SetCount(pItemDest->GetCount() + count);
        pItemDest->FSetState(ITEM_CHANGED);
        pItemDest->SaveToDB(trans);
        if (!clone)
        {
            pItem->RemoveFromWorld();
            pItem->DeleteFromDB(trans);
            delete pItem;
        }
        return pItemDest;
    }

    if (clone)
        pItem = pItem->CloneItem(count);
    else
        pItem->SetCount(count);

    if (pItem && pTab->SetItem(trans, slotId, pItem))
        return pItem;

    return NULL;
}

// Tries to reserve space for source item.
// If item in destination slot exists it must be the item of the same entry
// and stack must have enough space to take at least one item.
// Returns false if destination item specified and it cannot be used to reserve space.
bool Guild::BankMoveItemData::_ReserveSpace(uint8 slotId, Item* pItem, Item* pItemDest, uint32& count)
{
    uint32 requiredSpace = pItem->GetMaxStackCount();
    if (pItemDest)
    {
        // Make sure source and destination items match and destination item has space for more stacks.
        if (pItemDest->GetEntry() != pItem->GetEntry() || pItemDest->GetCount() >= pItem->GetMaxStackCount())
            return false;
        requiredSpace -= pItemDest->GetCount();
    }
    // Let's not be greedy, reserve only required space
    requiredSpace = std::min(requiredSpace, count);

    // Reserve space
    ItemPosCount pos(slotId, requiredSpace);
    if (!pos.isContainedIn(m_vec))
    {
        m_vec.push_back(pos);
        count -= requiredSpace;
    }
    return true;
}

void Guild::BankMoveItemData::CanStoreItemInTab(Item* pItem, uint8 skipSlotId, bool merge, uint32& count)
{
    for (uint8 slotId = 0; (slotId < GUILD_BANK_MAX_SLOTS) && (count > 0); ++slotId)
    {
        // Skip slot already processed in CanStore (when destination slot was specified)
        if (slotId == skipSlotId)
            continue;

        Item* pItemDest = m_pGuild->_GetItem(m_container, slotId);
        if (pItemDest == pItem)
            pItemDest = NULL;

        // If merge skip empty, if not merge skip non-empty
        if ((pItemDest != NULL) != merge)
            continue;

        _ReserveSpace(slotId, pItem, pItemDest, count);
    }
}

InventoryResult Guild::BankMoveItemData::CanStore(Item* pItem, bool swap)
{
    sLog->outDebug(LOG_FILTER_GUILD, "GUILD STORAGE: CanStore() tab = %u, slot = %u, item = %u, count = %u",
        m_container, m_slotId, pItem->GetEntry(), pItem->GetCount());

    uint32 count = pItem->GetCount();
    // Soulbound items cannot be moved
    if (pItem->IsSoulBound())
        return EQUIP_ERR_DROP_BOUND_ITEM;

    // Make sure destination bank tab exists
    if (m_container >= m_pGuild->GetPurchasedTabsSize())
        return EQUIP_ERR_WRONG_BAG_TYPE;

    // Slot explicitely specified. Check it.
    if (m_slotId != NULL_SLOT)
    {
        Item* pItemDest = m_pGuild->_GetItem(m_container, m_slotId);
        // Ignore swapped item (this slot will be empty after move)
        if ((pItemDest == pItem) || swap)
            pItemDest = NULL;

        if (!_ReserveSpace(m_slotId, pItem, pItemDest, count))
            return EQUIP_ERR_CANT_STACK;

        if (count == 0)
            return EQUIP_ERR_OK;
    }

    // Slot was not specified or it has not enough space for all the items in stack
    // Search for stacks to merge with
    if (pItem->GetMaxStackCount() > 1)
    {
        CanStoreItemInTab(pItem, m_slotId, true, count);
        if (count == 0)
            return EQUIP_ERR_OK;
    }

    // Search free slot for item
    CanStoreItemInTab(pItem, m_slotId, false, count);
    if (count == 0)
        return EQUIP_ERR_OK;

    return EQUIP_ERR_BANK_FULL;
}

///////////////////////////////////////////////////////////////////////////////
// Guild
Guild::Guild() : m_id(0), m_leaderGuid(0), m_createdDate(0), m_accountsNumber(0), m_bankMoney(0), m_eventLog(NULL),
    m_achievementMgr(this), _newsLog(this), m_BankLoaded(false)
{
    for (uint8 l_Type = 0; l_Type < ChallengeMax; l_Type++)
        m_ChallengeCount[l_Type] = 0;

    memset(&m_bankEventLog, 0, (GUILD_BANK_MAX_TABS + 1) * sizeof(LogHolder*));
}

Guild::~Guild()
{
    SQLTransaction temp(NULL);
    _DeleteBankItems(temp);

    // Cleanup
    if (m_eventLog)
        delete m_eventLog;
    for (uint8 tabId = 0; tabId <= GUILD_BANK_MAX_TABS; ++tabId)
        if (m_bankEventLog[tabId])
            delete m_bankEventLog[tabId];
    for (Members::iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
        delete itr->second;
}

/// Creates new guild with default data and saves it to database.
bool Guild::Create(Player * p_Leader, const std::string & p_Name)
{
    /// Check if guild with such name already exists
    if (sGuildMgr->GetGuildByName(p_Name))
        return false;

    WorldSession* l_LeaderSession = p_Leader->GetSession();

    if (!l_LeaderSession)
        return false;

    m_id            = sGuildMgr->GenerateGuildId();
    m_leaderGuid    = p_Leader->GetGUID();
    m_name          = p_Name;
    m_info          = "";
    m_motd          = "No message set.";
    m_bankMoney     = 0;
    m_createdDate   = ::time(NULL);

    _CreateLogHolders();

    sLog->outDebug(LOG_FILTER_GUILD, "GUILD: creating guild [%s] for leader %s (%u)", p_Name.c_str(), p_Leader->GetName(), GUID_LOPART(m_leaderGuid));

    PreparedStatement* l_Stmt = NULL;
    SQLTransaction l_Transaction = CharacterDatabase.BeginTransaction();

    l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_MEMBERS);
    l_Stmt->setUInt32(0, m_id);
    l_Transaction->Append(l_Stmt);

    uint8 index = 0;
    l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GUILD);
    l_Stmt->setUInt32(  index, m_id);
    l_Stmt->setString(++index, p_Name);
    l_Stmt->setUInt32(++index, GUID_LOPART(m_leaderGuid));
    l_Stmt->setString(++index, m_info);
    l_Stmt->setString(++index, m_motd);
    l_Stmt->setUInt64(++index, uint32(m_createdDate));
    l_Stmt->setUInt32(++index, m_emblemInfo.GetStyle());
    l_Stmt->setUInt32(++index, m_emblemInfo.GetColor());
    l_Stmt->setUInt32(++index, m_emblemInfo.GetBorderStyle());
    l_Stmt->setUInt32(++index, m_emblemInfo.GetBorderColor());
    l_Stmt->setUInt32(++index, m_emblemInfo.GetBackgroundColor());
    l_Stmt->setUInt64(++index, m_bankMoney);
    l_Transaction->Append(l_Stmt);

    CharacterDatabase.CommitTransaction(l_Transaction);

    /// Create default ranks
    _CreateDefaultGuildRanks(l_LeaderSession->GetSessionDbLocaleIndex());

    /// Add guildmaster
    bool l_Result = AddMember(m_leaderGuid, GR_GUILDMASTER);

    /// Call scripts on successful create
    if (l_Result)
        sScriptMgr->OnGuildCreate(this, p_Leader, p_Name);

    for (int8 l_Itr = 1; l_Itr < ChallengeMax; l_Itr++)
    {
        PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_INIT_GUILD_CHALLENGES);
        l_Statement->setInt32(0, GetId());
        l_Statement->setInt32(1, l_Itr);
        CharacterDatabase.Execute(l_Statement);
    }

    WorldPacket l_Data(SMSG_GUILD_EVENT_MOTD, 1 + 1 + m_motd.size());

    l_Data.WriteBits(m_motd.size(), 10);
    l_Data.FlushBits();

    l_Data.WriteString(m_motd);

    BroadcastPacket(&l_Data);

    return l_Result;
}

// Disbands guild and deletes all related data from database
void Guild::Disband()
{
    // Call scripts before guild data removed from database
    sScriptMgr->OnGuildDisband(this);

    WorldPacket l_Data(SMSG_GUILD_EVENT_DISBANDED);
    BroadcastPacket(&l_Data);

    // Remove all members
    while (!m_members.empty())
    {
        Members::const_iterator itr = m_members.begin();
        DeleteMember(itr->second->GetGUID(), true);
    }

    PreparedStatement* stmt = NULL;
    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD);
    stmt->setUInt32(0, m_id);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_RANKS);
    stmt->setUInt32(0, m_id);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_BANK_TABS);
    stmt->setUInt32(0, m_id);
    trans->Append(stmt);

    // Free bank tab used memory and delete items stored in them
    _DeleteBankItems(trans, true);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_BANK_ITEMS);
    stmt->setUInt32(0, m_id);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_BANK_RIGHTS);
    stmt->setUInt32(0, m_id);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_BANK_EVENTLOGS);
    stmt->setUInt32(0, m_id);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_EVENTLOGS);
    stmt->setUInt32(0, m_id);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_REMOVE_GUILD_CHALLENGES);
    stmt->setUInt32(0, m_id);
    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);

    sGuildFinderMgr->DeleteGuild(m_id);

    sGuildMgr->RemoveGuild(m_id);
}

void Guild::SaveToDB()
{
    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    m_achievementMgr.SaveToDB(trans);

    CharacterDatabase.CommitTransaction(trans);
}

///////////////////////////////////////////////////////////////////////////////
// HANDLE CLIENT COMMANDS
bool Guild::SetName(std::string const& name)
{
    if (m_name == name || name.empty() || name.length() > 24 || sObjectMgr->IsReservedName(name) || !ObjectMgr::IsValidCharterName(name))
        return false;

    m_name = name;
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GUILD_NAME);
    stmt->setString(0, m_name);
    stmt->setUInt32(1, GetId());
    CharacterDatabase.Execute(stmt);
    return true;
}

void Guild::HandleRoster(WorldSession* p_Session /*= NULL*/)
{
    ByteBuffer memberData;
    uint32 weeklyRepCap = uint32(sWorld->getIntConfig(CONFIG_GUILD_WEEKLY_REP_CAP)); ///< weekyRepCap is never read 01/18/16

    WorldPacket l_Data(SMSG_GUILD_ROSTER, 500 * 1024);

    l_Data << uint32(m_accountsNumber);
    l_Data << uint32(MS::Utilities::WowTime::Encode(m_createdDate));
    l_Data << uint32(0);                                      ///< Guild Flags
    l_Data << uint32(m_members.size());

    for (Members::const_iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
    {
        Member* l_Member = itr->second;
        Player* l_Player = l_Member->FindPlayer();

        if (!l_Player)
            l_Player = ObjectAccessor::FindPlayerInOrOutOfWorld(l_Member->GetGUID());

        bool l_InInterRealm = false;
        if (l_Player && l_Player->GetSession()->GetInterRealmBG())
            l_InInterRealm = true;


        uint8 l_Flags = GUILDMEMBER_STATUS_NONE;
        if (l_Player)
        {
            l_Flags |= GUILDMEMBER_STATUS_ONLINE;

            if (l_Player->isAFK())
                l_Flags |= GUILDMEMBER_STATUS_AFK;

            if (l_Player->isDND())
                l_Flags |= GUILDMEMBER_STATUS_DND;
        }

        l_Data.appendPackGUID(l_Member->GetGUID());

        l_Data << uint32(l_Member->GetRankId());

        uint32 l_ZoneId = l_Member->GetZone();
        if (l_InInterRealm)
            l_ZoneId = l_Player->GetSession()->GetInterRealmBG();
        else if (l_Player)
            l_ZoneId = l_Player->GetZoneId();


        l_Data << uint32(l_ZoneId);
        l_Data << uint32(l_Player ? l_Player->GetAchievementMgr().GetAchievementPoints() : 0);
        l_Data << uint32(l_Player ? l_Player->GetReputation(REP_GUILD) : 0);

        l_Data << float(l_Player ? 0.0f : float(::time(NULL) - l_Member->GetLogoutTime()) / DAY);       ///< Last Save

        /// For (2 professions)
        for (int l_I = 0; l_I < 2; ++l_I)
        {
            uint32 l_ProfessionID = l_Player ? l_Player->GetUInt32Value(PLAYER_FIELD_PROFESSION_SKILL_LINE + l_I) : 0;

            if (l_ProfessionID)
            {
                l_Data << uint32(l_ProfessionID);                                                       ///< Db ID
                l_Data << uint32(l_Player->GetSkillValue(l_ProfessionID));                              ///< Rank
                l_Data << uint32(l_Player->GetSkillStep(l_ProfessionID));                               ///< Step
            }
            else
            {
                l_Data << uint32(0);                                                                    ///< Db ID
                l_Data << uint32(0);                                                                    ///< Rank
                l_Data << uint32(0);                                                                    ///< Step
            }
        }

        l_Data << uint32(g_RealmID);                                                                    ///< Virtual Realm Address
        l_Data << uint8(l_Flags);                                                                       ///< Status
        l_Data << uint8(l_Member->GetLevel());                                                          ///< Level
        l_Data << uint8(l_Member->GetClass());                                                          ///< Class ID
        l_Data << uint8(l_Player ? l_Player->getGender() : 0);                                          ///< Gender

        l_Data.WriteBits(l_Member->GetName().length(), 6);                                              ///< Name
        l_Data.WriteBits(l_Member->GetPublicNote().length(), 8);                                        ///< Note
        l_Data.WriteBits(l_Member->GetOfficerNote().length(), 8);                                       ///< Officer Note
        l_Data.WriteBit(false);                                                                         ///< @TODO Has Authenticator
        l_Data.WriteBit(false);                                                                         ///< @TODO Can Scroll of Ressurect
        l_Data.FlushBits();

        l_Data.WriteString(l_Member->GetName());                                                        ///< Name
        l_Data.WriteString(l_Member->GetPublicNote());                                                  ///< Note
        l_Data.WriteString(l_Member->GetOfficerNote());                                                 ///< Officer Note
    }

    l_Data.WriteBits(m_motd.length(), 10);                                                              ///< Welcome Text
    l_Data.WriteBits(m_info.length(), 11);                                                              ///< Info Text
    l_Data.FlushBits();

    l_Data.WriteString(m_motd);                                                                         ///< Welcome Text
    l_Data.WriteString(m_info);                                                                         ///< Info Text

    if (p_Session)
        p_Session->SendPacket(&l_Data);
    else
        BroadcastPacket(&l_Data);

    sLog->outDebug(LOG_FILTER_GUILD, "WORLD: Sent (SMSG_GUILD_ROSTER)");
}

void Guild::HandleQuery(WorldSession* session)
{
    WorldPacket l_Data(SMSG_QUERY_GUILD_INFO_RESPONSE, 500);

    l_Data.appendPackGUID(GetGUID());
    l_Data.WriteBit(1);                 ///< hasData
    l_Data.FlushBits();

    l_Data.appendPackGUID(GetGUID());
    l_Data << uint32(g_RealmID);
    l_Data << uint32(_GetRanksSize());
    l_Data << uint32(m_emblemInfo.GetStyle());
    l_Data << uint32(m_emblemInfo.GetColor());
    l_Data << uint32(m_emblemInfo.GetBorderStyle());
    l_Data << uint32(m_emblemInfo.GetBorderColor());
    l_Data << uint32(m_emblemInfo.GetBackgroundColor());

    for (uint8 i = 0; i < _GetRanksSize(); ++i)
    {
        l_Data << uint32(m_ranks[i].GetId());
        l_Data << uint32(i);

        l_Data.WriteBits(m_ranks[i].GetName().size(), 7);
        l_Data.FlushBits();

        l_Data.WriteString(m_ranks[i].GetName());
    }

    l_Data.WriteBits(m_name.size(), 7);
    l_Data.FlushBits();

    l_Data.WriteString(m_name);

    session->SendPacket(&l_Data);

    sLog->outDebug(LOG_FILTER_GUILD, "WORLD: Sent (SMSG_GUILD_QUERY_RESPONSE)");
}

void Guild::HandleGuildRanks(WorldSession* p_Session) const
{
    WorldPacket l_Data(SMSG_GUILD_RANKS, 2 * 1024);

    l_Data << uint32(_GetRanksSize());

    for (uint8 l_I = 0; l_I < _GetRanksSize(); l_I++)
    {
        RankInfo const* l_RankInfo = GetRankInfo(l_I);

        if (!l_RankInfo)
            continue;

        l_Data << uint32(l_RankInfo->GetId());
        l_Data << uint32(l_I);
        l_Data << uint32(l_RankInfo->GetRights());
        l_Data << uint32(l_RankInfo->GetBankMoneyPerDay());

        for (uint8 j = 0; j < GUILD_BANK_MAX_TABS; ++j)
        {
            l_Data << uint32(l_RankInfo->GetBankTabRights(j));
            l_Data << uint32(l_RankInfo->GetBankTabSlotsPerDay(j));
        }

        l_Data.WriteBits(l_RankInfo->GetName().length(), 7);
        l_Data.FlushBits();

        l_Data.WriteString(l_RankInfo->GetName());
    }

    p_Session->SendPacket(&l_Data);
}

void Guild::HandleSetMOTD(WorldSession* session, const std::string& motd)
{
    if (m_motd == motd)
        return;

    // Player must have rights to set MOTD
    if (!_HasRankRight(session->GetPlayer(), GR_RIGHT_SETMOTD))
        SendCommandResult(session, GUILD_INVITE_S, ERR_GUILD_PERMISSIONS);
    else
    {
        m_motd = motd;

        sScriptMgr->OnGuildMOTDChanged(this, motd);

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GUILD_MOTD);
        stmt->setString(0, motd);
        stmt->setUInt32(1, m_id);
        CharacterDatabase.Execute(stmt);

        WorldPacket l_Data(SMSG_GUILD_EVENT_MOTD, 1 + 1 + m_motd.size());

        l_Data.WriteBits(m_motd.size(), 10);
        l_Data.FlushBits();

        l_Data.WriteString(m_motd);

        BroadcastPacket(&l_Data);
    }
}

void Guild::HandleSetInfo(WorldSession* session, const std::string& info)
{
    if (m_info == info)
        return;

    // Player must have rights to set guild's info
    if (!_HasRankRight(session->GetPlayer(), GR_RIGHT_MODIFY_GUILD_INFO))
        SendCommandResult(session, GUILD_CREATE_S, ERR_GUILD_PERMISSIONS);
    else
    {
        m_info = info;

        sScriptMgr->OnGuildInfoChanged(this, info);

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GUILD_INFO);
        stmt->setString(0, info);
        stmt->setUInt32(1, m_id);
        CharacterDatabase.Execute(stmt);
    }
}

void Guild::HandleSetEmblem(WorldSession* session, const EmblemInfo& emblemInfo)
{
    Player* player = session->GetPlayer();
    if (!_IsLeader(player))
        // "Only guild leaders can create emblems."
        SendSaveEmblemResult(session, ERR_GUILDEMBLEM_NOTGUILDMASTER);
    else if (!player->HasEnoughMoney(uint64(EMBLEM_PRICE)))
        // "You can't afford to do that."
        SendSaveEmblemResult(session, ERR_GUILDEMBLEM_NOTENOUGHMONEY);
    else
    {
        player->ModifyMoney(-int64(EMBLEM_PRICE));

        m_emblemInfo = emblemInfo;
        m_emblemInfo.SaveToDB(m_id);

        // "Guild Emblem saved."
        SendSaveEmblemResult(session, ERR_GUILDEMBLEM_SUCCESS);

        HandleQuery(session);

        m_achievementMgr.UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BUY_GUILD_TABARD, 1, 0, 0, NULL, player);
    }
}

void Guild::HandleSetLeader(WorldSession * p_Session, const std::string & p_Name)
{
    Player* player = p_Session->GetPlayer();

    /// Only leader can assign new leader
    if (!_IsLeader(player))
        SendCommandResult(p_Session, GUILD_INVITE_S, ERR_GUILD_PERMISSIONS);

    /// Old leader must be a member of guild
    else if (Member * l_OldLeader = GetMember(player->GetGUID()))
    {
        /// New leader must be a member of guild
        if (Member * l_NewLeader = GetMember(p_Session, p_Name))
        {
            _SetLeaderGUID(l_NewLeader);
            l_OldLeader->ChangeRank(GR_OFFICER);

            WorldPacket l_Data(SMSG_GUILD_EVENT_NEW_LEADER, 80);
            l_Data.WriteBit(false);                                 ///< Self Promoted
            l_Data.WriteBits(l_OldLeader->GetName().size(), 6);     ///< Old Leader Name
            l_Data.WriteBits(l_NewLeader->GetName().size(), 6);     ///< New Leader Name
            l_Data.FlushBits();

            l_Data.appendPackGUID(l_OldLeader->GetGUID());          ///< Old Leader GUID
            l_Data << uint32(g_RealmID);                            ///< Old Leader Virtual Realm Address
            l_Data.appendPackGUID(l_NewLeader->GetGUID());          ///< New Leader GUID
            l_Data << uint32(g_RealmID);                            ///< New Leader Virtual Realm Address

            l_Data.WriteString(l_OldLeader->GetName());             ///< Old Leader Name
            l_Data.WriteString(l_NewLeader->GetName());             ///< New Leader Name

            BroadcastPacket(&l_Data);
        }
    }
    else
        SendCommandResult(p_Session, GUILD_INVITE_S, ERR_GUILD_PERMISSIONS);
}

bool Guild::SwitchGuildLeader(uint64 newLeaderGuid)
{
    if (Member* l_OldLeader = GetMember(GetLeaderGUID()))
    {
        /// New leader must be a member of guild
        if (Member* l_NewLeader = GetMember(newLeaderGuid))
        {
            _SetLeaderGUID(l_NewLeader);
            l_OldLeader->ChangeRank(GR_OFFICER);

            WorldPacket l_Data(SMSG_GUILD_EVENT_NEW_LEADER, 80);
            l_Data.WriteBit(false);                                 ///< Self Promoted
            l_Data.WriteBits(l_OldLeader->GetName().size(), 6);     ///< Old Leader Name
            l_Data.WriteBits(l_NewLeader->GetName().size(), 6);     ///< New Leader Name
            l_Data.FlushBits();

            l_Data.appendPackGUID(l_OldLeader->GetGUID());          ///< Old Leader GUID
            l_Data << uint32(g_RealmID);                            ///< Old Leader Virtual Realm Address
            l_Data.appendPackGUID(l_NewLeader->GetGUID());          ///< New Leader GUID
            l_Data << uint32(g_RealmID);                            ///< New Leader Virtual Realm Address

            l_Data.WriteString(l_OldLeader->GetName());             ///< Old Leader Name
            l_Data.WriteString(l_NewLeader->GetName());             ///< New Leader Name

            BroadcastPacket(&l_Data);

            return true;
        }
    }

    return false;
}

void Guild::HandleSetBankTabInfo(WorldSession* /*p_Session*/, uint8 p_TabID, const std::string& p_Name, const std::string& p_Icon)
{
    if (BankTab* pTab = GetBankTab(p_TabID))
    {
        pTab->SetInfo(p_Name, p_Icon);

        WorldPacket l_Data(SMSG_GUILD_EVENT_TAB_MODIFIED, 80);
        l_Data << uint32(p_TabID);

        l_Data.WriteBits(p_Name.size(), 7);
        l_Data.WriteBits(p_Icon.size(), 9);
        l_Data.FlushBits();

        l_Data.WriteString(p_Name);
        l_Data.WriteString(p_Icon);

        BroadcastPacket(&l_Data);
    }
}

void Guild::HandleSetMemberNote(WorldSession* session, std::string const& note, uint64 guid, bool isPublic)
{
    // Player must have rights to set public/officer note
    if (!_HasRankRight(session->GetPlayer(), isPublic ? GR_RIGHT_EPNOTE : GR_RIGHT_EOFFNOTE))
        SendCommandResult(session, GUILD_INVITE_S, ERR_GUILD_PERMISSIONS);

    else if (Member* member = GetMember(guid))
    {
        if (isPublic)
            member->SetPublicNote(note);
        else
            member->SetOfficerNote(note);

        HandleRoster(session);
    }
}

void Guild::HandleSetRankInfo(WorldSession * p_Session, uint32 p_RankID, const std::string& p_Name, uint32 p_Rights, uint32 p_MoneyPerDay, GuildBankRightsAndSlotsVec p_RightsAndSlots)
{
    /// Only leader can modify ranks
    if (!_IsLeader(p_Session->GetPlayer()))
        SendCommandResult(p_Session, GUILD_INVITE_S, ERR_GUILD_PERMISSIONS);

    else if (RankInfo * l_RankInfo = GetRankInfo(p_RankID))
    {
        sLog->outDebug(LOG_FILTER_GUILD, "WORLD: Changed RankName to '%s', rights to 0x%08X", p_Name.c_str(), p_Rights);

        l_RankInfo->SetName(p_Name);
        l_RankInfo->SetRights(p_Rights);

        _SetRankBankMoneyPerDay(p_RankID, p_MoneyPerDay);

        uint8 l_TabID = 0;

        for (GuildBankRightsAndSlotsVec::const_iterator l_It = p_RightsAndSlots.begin(); l_It != p_RightsAndSlots.end(); ++l_It)
        {
            _SetRankBankTabRightsAndSlots(p_RankID, l_TabID++, *l_It);
        }

        WorldPacket l_Data(SMSG_GUILD_EVENT_RANK_CHANGED);
        l_Data << uint32(p_RankID);
        BroadcastPacket(&l_Data);
    }
}

void Guild::HandleBuyBankTab(WorldSession* p_Session, uint8 p_TabID)
{
    if (p_TabID != GetPurchasedTabsSize())
        return;

    uint32 p_TabCost = _GetGuildBankTabPrice(p_TabID) * GOLD;

    if (!p_TabCost && p_TabID < 6) // For SPELL_EFFECT_UNLOCK_GUILD_VAULT_TAB
        return;

    Player* p_Player = p_Session->GetPlayer();

    if (!p_Player->HasEnoughMoney(uint64(p_TabCost)))                   // Should not happen, this is checked by client
        return;

    if (!_CreateNewBankTab())
        return;

    p_Player->ModifyMoney(-int64(p_TabCost));

    HandleSpellEffectBuyBankTab(p_Session, p_TabID);
}

void Guild::HandleSpellEffectBuyBankTab(WorldSession* p_Session, uint8 p_TabID)
{
    Player* p_Player = p_Session->GetPlayer();
    _SetRankBankMoneyPerDay(p_Player->GetRank(), uint32(GUILD_WITHDRAW_MONEY_UNLIMITED));
    _SetRankBankTabRightsAndSlots(p_Player->GetRank(), p_TabID, GuildBankRightsAndSlots(GUILD_BANK_RIGHT_FULL, uint32(GUILD_WITHDRAW_SLOT_UNLIMITED)));

    WorldPacket l_Data(SMSG_GUILD_EVENT_RANKS_UPDATED);
    BroadcastPacket(&l_Data);

    l_Data.Initialize(SMSG_GUILD_EVENT_TAB_ADDED);
    BroadcastPacket(&l_Data);

    SendBankList(p_Session, p_TabID, false, true);

    GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BUY_GUILD_BANK_SLOTS, 1, 0, 0, NULL, p_Player);
}

void Guild::HandleInviteMember(WorldSession* p_Session, const std::string& p_Name)
{
    Player* p_Invitee = sObjectAccessor->FindPlayerByName(p_Name.c_str());

    if (!p_Invitee)
    {
        SendCommandResult(p_Session, GUILD_INVITE_S, ERR_GUILD_PLAYER_NOT_FOUND_S, p_Name);
        return;
    }

    Player* l_Player = p_Session->GetPlayer();

    /// Do not show invitations from ignored players
    if (p_Invitee->GetSocial()->HasIgnore(l_Player->GetGUIDLow()))
        return;

    if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GUILD) && p_Invitee->GetTeam() != l_Player->GetTeam())
    {
        SendCommandResult(p_Session, GUILD_INVITE_S, ERR_GUILD_NOT_ALLIED, p_Name);
        return;
    }
    
    /// Invited player cannot be invited
    if (p_Invitee->GetGuildIdInvited())
    {
        SendCommandResult(p_Session, GUILD_INVITE_S, ERR_ALREADY_INVITED_TO_GUILD_S, p_Name);
        return;
    }
    
    /// Player already in guild cannot be invited
    if (p_Invitee->GetGuildId())
    {
        SendCommandResult(p_Session, GUILD_INVITE_S, ERR_ALREADY_IN_GUILD_S, p_Name);
        return;
    }

    /// Inviting player must have rights to invite
    if (!_HasRankRight(l_Player, GR_RIGHT_INVITE))
    {
        SendCommandResult(p_Session, GUILD_INVITE_S, ERR_GUILD_PERMISSIONS);
        return;
    }

    sLog->outDebug(LOG_FILTER_GUILD, "Player %s invited %s to join his Guild", l_Player->GetName(), p_Name.c_str());

    p_Invitee->SetGuildIdInvited(m_id);

    _LogEvent(GUILD_EVENT_LOG_INVITE_PLAYER, l_Player->GetGUIDLow(), p_Invitee->GetGUIDLow());

    WorldPacket l_Data(SMSG_GUILD_INVITE);
    uint64 l_OldGuildGUID = MAKE_NEW_GUID(p_Invitee->GetGuildId(), 0, p_Invitee->GetGuildId() ? uint32(HIGHGUID_GUILD) : 0);
    uint64 l_NewGuildGUID = GetGUID();

    l_Data.WriteBits(strlen(l_Player->GetName()), 6);
    l_Data.WriteBits(m_name.length(), 7);
    l_Data.WriteBits(p_Invitee->GetGuildName().size(), 7);
    l_Data << uint32(g_RealmID);
    l_Data << uint32(g_RealmID);
    l_Data.appendPackGUID(l_NewGuildGUID);
    l_Data << uint32(g_RealmID);
    l_Data.appendPackGUID(l_OldGuildGUID);
    l_Data << uint32(m_emblemInfo.GetStyle());
    l_Data << uint32(m_emblemInfo.GetColor());
    l_Data << uint32(m_emblemInfo.GetBorderStyle());
    l_Data << uint32(m_emblemInfo.GetBorderColor());
    l_Data << uint32(m_emblemInfo.GetBackgroundColor());
    l_Data << uint32(GetAchievementMgr().GetAchievementPoints());

    if (strlen(l_Player->GetName()) > 0)
        l_Data.WriteString(l_Player->GetName());

    l_Data.WriteString(m_name);
    l_Data.WriteString(p_Invitee->GetGuildName());

    p_Invitee->GetSession()->SendPacket(&l_Data);

    sLog->outDebug(LOG_FILTER_GUILD, "WORLD: Sent (SMSG_GUILD_INVITE)");
}

void Guild::HandleAcceptMember(WorldSession* p_Session)
{
    Player* p_Player = p_Session->GetPlayer();

    if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GUILD) && p_Player->GetTeam() != sObjectMgr->GetPlayerTeamByGUID(GetLeaderGUID()))
        return;

    if (AddMember(p_Player->GetGUID()))
    {
        _LogEvent(GUILD_EVENT_LOG_JOIN_GUILD, p_Player->GetGUIDLow());

        WorldPacket l_Data(SMSG_GUILD_EVENT_PLAYER_JOINED, 60);
        l_Data.appendPackGUID(p_Player->GetGUID());
        l_Data << uint32(g_RealmID);

        l_Data.WriteBits(strlen(p_Player->GetName()), 6);
        l_Data.FlushBits();

        if (p_Player->GetName())
            l_Data.WriteString(p_Player->GetName());

        BroadcastPacket(&l_Data);

        l_Data.Initialize(SMSG_GUILD_EVENT_MOTD, 1 + 1 + m_motd.size());
        l_Data.WriteBits(m_motd.size(), 10);
        l_Data.FlushBits();

        l_Data.WriteString(m_motd);

        p_Session->SendPacket(&l_Data);

        l_Data.Initialize(SMSG_GUILD_EVENT_PRESENCE_CHANGE);
        l_Data.appendPackGUID(p_Session->GetPlayer()->GetGUID());           ///< Guid
        l_Data << uint32(g_RealmID);                                        ///< Virtual Realm Address
        l_Data.WriteBits(strlen(p_Session->GetPlayer()->GetName()), 6);     ///< Name
        l_Data.WriteBit(false);                                             ///< Mobile
        l_Data.WriteBit(true);                                              ///< Logged On
        l_Data.FlushBits();

        if (strlen(p_Session->GetPlayer()->GetName()) > 0)
            l_Data.append(p_Session->GetPlayer()->GetName(), strlen(p_Session->GetPlayer()->GetName()));

        BroadcastPacket(&l_Data);

        sGuildFinderMgr->RemoveMembershipRequest(p_Player->GetGUIDLow(), GUID_LOPART(this->GetGUID()));
    }
}

void Guild::HandleLeaveMember(WorldSession* session)
{
    Player* player = session->GetPlayer();
    bool disband = false;

    // If leader is leaving
    if (_IsLeader(player))
    {
        if (m_members.size() > 1)
            // Leader cannot leave if he is not the last member
            SendCommandResult(session, GUILD_QUIT_S, ERR_GUILD_LEADER_LEAVE);
        else
        {
            // Guild is disbanded if leader leaves.
            Disband();
            disband = true;
        }
    }
    else
    {
        SendMemberLeave(session, ObjectGuid(player->GetGUID()), false);
        DeleteMember(player->GetGUID(), false, false);
        _LogEvent(GUILD_EVENT_LOG_LEAVE_GUILD, player->GetGUIDLow());
        SendCommandResult(session, GUILD_QUIT_S, ERR_PLAYER_NO_MORE_IN_GUILD, m_name);
    }

    uint32 l_GuildID = GetId();

    if (disband)
        delete this;
    else
        HandleRoster(session);

    sCalendarMgr->RemovePlayerGuildEventsAndSignups(player->GetGUID(), l_GuildID);
}

void Guild::HandleRemoveMember(WorldSession* session, uint64 guid)
{
    Player* player = session->GetPlayer();
    Player* removedPlayer = ObjectAccessor::FindPlayer(guid);
    Member* member = GetMember(guid);

    // Player must have rights to remove members
    if (!_HasRankRight(player, GR_RIGHT_REMOVE))
        SendCommandResult(session, GUILD_INVITE_S, ERR_GUILD_PERMISSIONS);
    // Removed player must be a member of the guild
    else if (member)
    {
        // Guild masters cannot be removed
        if (member->IsRank(GR_GUILDMASTER))
            SendCommandResult(session, GUILD_QUIT_S, ERR_GUILD_LEADER_LEAVE);
        // Do not allow to remove player with the same rank or higher
        else if (member->IsRankNotLower(player->GetRank()))
            SendCommandResult(session, GUILD_QUIT_S, ERR_GUILD_RANK_TOO_HIGH_S, member->GetName());
        else
        {
            // After call to DeleteMember pointer to member becomes invalid
            _LogEvent(GUILD_EVENT_LOG_UNINVITE_PLAYER, player->GetGUIDLow(), GUID_LOPART(guid));
            SendMemberLeave(session, ObjectGuid(member->GetGUID()), true);
            DeleteMember(guid, false, true);
        }
    }
    else if (removedPlayer)
        SendCommandResult(session, GUILD_QUIT_S, ERR_PLAYER_NO_MORE_IN_GUILD, removedPlayer->GetName());

    HandleRoster(session);
}

void Guild::HandleSetMemberRank(WorldSession* session, uint64 targetGuid, uint64 setterGuid, uint32 rank)
{
    Player* player = session->GetPlayer();

    // Promoted player must be a member of guild
    if (Member* member = GetMember(targetGuid))
    {
        if (!_HasRankRight(player, rank > member->GetRankId() ? GR_RIGHT_DEMOTE : GR_RIGHT_PROMOTE))
        {
            SendCommandResult(session, GUILD_INVITE_S, ERR_GUILD_PERMISSIONS);
            return;
        }

        // Player cannot promote himself
        if (member->IsSamePlayer(player->GetGUID()))
        {
            SendCommandResult(session, GUILD_INVITE_S, ERR_GUILD_NAME_INVALID);
            return;
        }

        SendGuildRanksUpdate(setterGuid, targetGuid, rank);
    }
}

void Guild::HandleSwapRanks(WorldSession* /*p_Session*/, uint32 p_RankID, bool p_Up) ///< p_Session is unused
{
    RankInfo* l_FirstRank = NULL;
    RankInfo* l_SecondRank = NULL;

    uint32 l_RankSecondID = p_RankID - (-1 + 2 * uint8(p_Up));

    for (uint32 l_I = 0; l_I < m_ranks.size(); ++l_I)
    {
        if (m_ranks[l_I].GetId() == p_RankID)
            l_FirstRank = &m_ranks[l_I];

        if (m_ranks[l_I].GetId() == l_RankSecondID)
            l_SecondRank = &m_ranks[l_I];
    }
    if (!l_FirstRank || !l_SecondRank)
        return;

    RankInfo l_Tmp = *l_SecondRank;
    l_SecondRank->SetName(l_FirstRank->GetName());
    l_SecondRank->SetRights(l_FirstRank->GetRights());

    l_FirstRank->SetName(l_Tmp.GetName());
    l_FirstRank->SetRights(l_Tmp.GetRights());

    WorldPacket l_Data(SMSG_GUILD_EVENT_RANK_CHANGED);
    l_Data << uint32(p_RankID);
    BroadcastPacket(&l_Data);

    l_Data.Initialize(SMSG_GUILD_EVENT_RANK_CHANGED);
    l_Data << uint32(l_RankSecondID);
    BroadcastPacket(&l_Data);
}

void Guild::HandleAddNewRank(WorldSession* session, std::string const& name) //, uint32 rankId)
{
    if (_GetRanksSize() >= GUILD_RANKS_MAX_COUNT)
        return;

    // Only leader can add new rank
    if (!_IsLeader(session->GetPlayer()))
        SendCommandResult(session, GUILD_INVITE_S, ERR_GUILD_PERMISSIONS);
    else
    {
        _CreateRank(name, GR_RIGHT_GCHATLISTEN | GR_RIGHT_GCHATSPEAK);
    }
}

void Guild::HandleRemoveRank(WorldSession* session, uint32 rankId)
{
    // Cannot remove rank if total count is minimum allowed by the client
    if (_GetRanksSize() <= GUILD_RANKS_MIN_COUNT)
        return;

    // Only leader can delete ranks
    if (!_IsLeader(session->GetPlayer()))
        SendCommandResult(session, GUILD_INVITE_S, ERR_GUILD_PERMISSIONS);
    else
    {
        SQLTransaction trans = CharacterDatabase.BeginTransaction();

        // Delete bank rights for rank
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_BANK_RIGHTS_FOR_RANK);
        stmt->setUInt32(0, m_id);
        stmt->setUInt8(1, rankId);
        CharacterDatabase.ExecuteOrAppend(trans, stmt);
        // Delete rank
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_RANK);
        stmt->setUInt32(0, m_id);
        stmt->setUInt8(1, rankId);
        CharacterDatabase.ExecuteOrAppend(trans, stmt);

        CharacterDatabase.CommitTransaction(trans);

        m_ranks.erase(m_ranks.begin() + rankId);

        // Restruct m_ranks
        for (uint8 i = 0; i < m_ranks.size(); ++i)
            if (m_ranks[i].GetId() != i)
                m_ranks[i].UpdateId(i);

        WorldPacket l_Data(SMSG_GUILD_EVENT_RANKS_UPDATED);
        BroadcastPacket(&l_Data);
    }
}

void Guild::HandleMemberDepositMoney(WorldSession* session, uint64 amount, bool cashFlow /*=false*/)
{
    Player* player = session->GetPlayer();

    // Call script after validation and before money transfer.
    sScriptMgr->OnGuildMemberDepositMoney(this, player, amount);

    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    // Add money to bank
    _ModifyBankMoney(trans, amount, true);
    // Remove money from player
    player->ModifyMoney(-int64(amount));
    player->SaveGoldToDB(trans);
    // Log GM action (@TODO: move to scripts)
    if (!AccountMgr::IsPlayerAccount(player->GetSession()->GetSecurity()) && sWorld->getBoolConfig(CONFIG_GM_LOG_TRADE))
    {
        sLog->outCommand(player->GetSession()->GetAccountId(), "", player->GetGUIDLow(), player->GetName(), 0, "", 0, "",
                        "GM %s (Account: %u) deposit money (Amount: %lu) to guild bank (Guild ID %u)",
                        player->GetName(), player->GetSession()->GetAccountId(), amount, m_id);
    }
    // Log guild bank event
    _LogBankEvent(trans, cashFlow ? GUILD_BANK_LOG_CASH_FLOW_DEPOSIT : GUILD_BANK_LOG_DEPOSIT_MONEY, uint8(0), player->GetGUIDLow(), amount);

    CharacterDatabase.CommitTransaction(trans);
}

void Guild::DepositMoney(uint64 amount)
{
    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    _ModifyBankMoney(trans, amount, true);
    CharacterDatabase.CommitTransaction(trans);
}

bool Guild::HandleMemberWithdrawMoney(WorldSession* session, uint64 amount, bool repair)
{
    // clamp amount to MAX_MONEY_AMOUNT, Players can't hold more than that anyway
    amount = std::min(amount, uint64(MAX_MONEY_AMOUNT));

    if (m_bankMoney < amount)                               // Not enough money in bank
        return false;

    Player* player = session->GetPlayer();
    if (!_HasRankRight(player, repair ? GR_RIGHT_WITHDRAW_REPAIR : GR_RIGHT_WITHDRAW_GOLD))
        return false;

    uint64 remainingMoney = _GetMemberRemainingMoney(player->GetGUID());
    if (!remainingMoney)
        return false;

    if (remainingMoney < amount)
        return false;

    // Call script after validation and before money transfer.
    sScriptMgr->OnGuildMemberWitdrawMoney(this, player, amount, repair);

    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    // Update remaining money amount
    if (remainingMoney < uint64(GUILD_WITHDRAW_MONEY_UNLIMITED))
    {
        if (Member* member = GetMember(player->GetGUID()))
            member->DecreaseRemainingMoneyValue(trans, amount);
    }

    // Remove money from bank
    _ModifyBankMoney(trans, amount, false);
    // Add money to player (if required)
    if (!repair)
    {
        player->ModifyMoney(amount);
        player->SaveGoldToDB(trans);
    }
    // Log guild bank event
    _LogBankEvent(trans, repair ? GUILD_BANK_LOG_REPAIR_MONEY : GUILD_BANK_LOG_WITHDRAW_MONEY, uint8(0), player->GetGUIDLow(), amount);
    CharacterDatabase.CommitTransaction(trans);

    SendMoneyInfo(session);
    if (!repair)
        SendBankList(session, 0, false, false);
    return true;
}

void Guild::HandleMemberLogout(WorldSession * p_Session)
{
    Player* l_Player = p_Session->GetPlayer();

    if (Member * l_Member = GetMember(l_Player->GetGUID()))
    {
        l_Member->SetStats(l_Player);
        l_Member->UpdateLogoutTime();
    }

    WorldPacket l_Data(SMSG_GUILD_EVENT_PRESENCE_CHANGE);
    l_Data.appendPackGUID(p_Session->GetPlayer()->GetGUID());           ///< Guid
    l_Data << uint32(g_RealmID);                                        ///< Virtual Realm Address
    l_Data.WriteBits(strlen(p_Session->GetPlayer()->GetName()), 6);     ///< Name
    l_Data.WriteBit(false);                                             ///< Mobile
    l_Data.WriteBit(false);                                             ///< Logged On
    l_Data.FlushBits();

    if (strlen(p_Session->GetPlayer()->GetName()) > 0)
        l_Data.append(p_Session->GetPlayer()->GetName(), strlen(p_Session->GetPlayer()->GetName()));

    BroadcastPacket(&l_Data);

    SaveToDB();
}

void Guild::HandleDisband(WorldSession* session)
{
    // Only leader can disband guild
    if (!_IsLeader(session->GetPlayer()))
        Guild::SendCommandResult(session, GUILD_INVITE_S, ERR_GUILD_PERMISSIONS);
    else
    {
        Disband();
        sLog->outDebug(LOG_FILTER_GUILD, "WORLD: Guild Successfully Disbanded");
        delete this;
    }
}

void Guild::HandleGuildPartyRequest(WorldSession * p_Session)
{
    Player * l_Player = p_Session->GetPlayer();
    Group  * l_Group  = l_Player->GetGroup();

    /// Make sure player is a member of the guild and that he is in a group.
    if (!IsMember(l_Player->GetGUID()) || !l_Group)
        return;

    WorldPacket l_Data(SMSG_GUILD_PARTY_STATE, 13);

    l_Data.WriteBit(l_Player->GetMap()->GetOwnerGuildId(l_Player->GetTeam()) == GetId());       ///< Is guild group
    l_Data.FlushBits();

    l_Data << uint32(0);                                                                        ///< Current guild members
    l_Data << uint32(0);                                                                        ///< Needed guild members
    l_Data << float(0.0f);                                                                       ///< Guild XP multiplier

    p_Session->SendPacket(&l_Data);
}

///////////////////////////////////////////////////////////////////////////////
// Send data to client
void Guild::SendEventLog(WorldSession* p_Session) const
{
    WorldPacket l_Data(SMSG_GUILD_EVENT_LOG_QUERY_RESULTS, 2 * 1024);
    m_eventLog->WritePacket(l_Data);
    p_Session->SendPacket(&l_Data);
    sLog->outDebug(LOG_FILTER_GUILD, "WORLD: Sent (SMSG_GUILD_EVENT_LOG_QUERY_RESULTS)");
}

void Guild::SendBankLog(WorldSession * p_Session, uint8 p_TabID) const
{
    // GUILD_BANK_MAX_TABS send by client for money log
    if (p_TabID < GetPurchasedTabsSize() || p_TabID == GUILD_BANK_MAX_TABS)
    {
        const LogHolder * l_Log = m_bankEventLog[p_TabID];
        WorldPacket l_Data(SMSG_GUILD_BANK_LOG_QUERY_RESULT, 5 * 1024);

        l_Data << uint32(p_TabID);
        l_Log->WritePacket(l_Data);

        p_Session->SendPacket(&l_Data);
    }
}

void Guild::LoadBank()
{
    if (m_BankLoaded)
        return;

    //                                                      0        1      2        3          4           5            6               7          8
    QueryResult result = CharacterDatabase.PQuery("SELECT guildid, TabId, LogGuid, EventType, PlayerGuid, ItemOrMoney, ItemStackCount, DestTabId, TimeStamp FROM guild_bank_eventlog WHERE guildid = %u ORDER BY TimeStamp DESC, LogGuid DESC", m_id);

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            LoadBankEventLogFromDB(fields);
        }
        while (result->NextRow());
    }

    //                                                     0        1      2        3        4
    result = CharacterDatabase.PQuery("SELECT guildid, TabId, TabName, TabIcon, TabText FROM guild_bank_tab WHERE guildid = %u ORDER BY TabId ASC", m_id);
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            LoadBankTabFromDB(fields);
        }
        while (result->NextRow());
    }

    //                                                      0            1                2      3         4        5      6                  7                 8           9      10
    result = CharacterDatabase.PQuery("SELECT creatorGuid, giftCreatorGuid, count, duration, charges, flags, enchantments, randomPropertyId, transmogrifyId, bonuses, upgradeId, "
                                                    //   11       12           13      14          15                   16      17       18        19    20
                                                    "durability, playedTime, text, custom_flags, enchantIllusionId, guildid, TabId, SlotId, item_guid, itemEntry FROM guild_bank_item gbi INNER JOIN item_instance ii ON gbi.guildid = %u AND gbi.item_guid = ii.guid", m_id);
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            LoadBankItemFromDB(fields);
        }
        while (result->NextRow());
    }

    m_BankLoaded = true;
}

void Guild::SendBankList(WorldSession* p_Session, uint8 p_TabID, bool p_WithContent, bool p_WithTabInfo)
{
    if (!m_BankLoaded)
        LoadBank();

    /// Don't send packet for non purchased tab
    BankTab const* l_CurrTab = GetBankTab(p_TabID);
    if (!l_CurrTab && p_TabID > 0)
        return;

    WorldPacket l_Data(Opcodes::SMSG_GUILD_BANK_QUERY_RESULTS, 6 * 1024);

    uint32 l_ItemCount = 0;

    if (p_WithContent && _MemberHasTabRights(p_Session->GetPlayer()->GetGUID(), p_TabID, GuildBankRights::GUILD_BANK_RIGHT_VIEW_TAB))
    {
        if (BankTab const* l_BankTab = GetBankTab(p_TabID))
        {
            for (uint8 l_SlotID = 0; l_SlotID < GuildMisc::GUILD_BANK_MAX_SLOTS; ++l_SlotID)
            {
                if (Item * l_TabItem = l_BankTab->GetItem(l_SlotID)) ///< l_TabItem is unused
                    ++l_ItemCount;
            }
        }
    }

    l_Data << uint64(m_bankMoney);                                                              ///< Money
    l_Data << uint32(p_TabID);                                                                  ///< Tab
    l_Data << uint32(_GetMemberRemainingSlots(p_Session->GetPlayer()->GetGUID(), p_TabID));     ///< Withdrawals Remaining
    l_Data << uint32(p_WithTabInfo ? GetPurchasedTabsSize() : 0);                               ///< Tab Info Count
    l_Data << uint32(p_WithContent ? l_ItemCount : 0);                                          ///< Item Info Count

    if (p_WithTabInfo)
    {
        for (uint8 l_TabID = 0; l_TabID < GetPurchasedTabsSize(); ++l_TabID)
        {
            l_Data << uint32(l_TabID);                                                          ///< Tab Index
            l_Data.WriteBits(m_bankTabs[l_TabID]->GetName().length(), 7);                       ///< Name
            l_Data.WriteBits(m_bankTabs[l_TabID]->GetIcon().length(), 9);                       ///< Icon
            l_Data.FlushBits();

            l_Data.WriteString(m_bankTabs[l_TabID]->GetName());                                 ///< Name
            l_Data.WriteString(m_bankTabs[l_TabID]->GetIcon());                                 ///< Icon
        }
    }

    if (p_WithContent && _MemberHasTabRights(p_Session->GetPlayer()->GetGUID(), p_TabID, GuildBankRights::GUILD_BANK_RIGHT_VIEW_TAB))
    {
        if (BankTab const* l_Tab = GetBankTab(p_TabID))
        {
            for (uint8 l_SlotID = 0; l_SlotID < GuildMisc::GUILD_BANK_MAX_SLOTS; ++l_SlotID)
            {
                if (Item* l_TabItem = l_Tab->GetItem(l_SlotID))
                {
                    uint32 l_EnchantsCount = 0;

                    for (uint32 l_EnchantmentSlot = 0; l_EnchantmentSlot < EnchantmentSlot::MAX_ENCHANTMENT_SLOT; ++l_EnchantmentSlot)
                    {
                        if (uint32 l_EnchantId = l_TabItem->GetEnchantmentId(EnchantmentSlot(l_EnchantmentSlot))) ///< l_EnchantId is unused
                            ++l_EnchantsCount;
                    }

                    l_Data << uint32(l_SlotID);                             ///< Slot

                    Item::BuildDynamicItemDatas(l_Data, l_TabItem);

                    l_Data << uint32(l_TabItem->GetCount());                ///< Count
                    l_Data << uint32(0);                                    ///< Enchantment ID
                    l_Data << uint32(abs(l_TabItem->GetSpellCharges()));    ///< Charges
                    l_Data << uint32(0);                                    ///< OnUse Enchantment ID
                    l_Data << uint32(0);                                    ///< Enchant count
                    l_Data << uint32(0);                                    ///< Flags

                    /* Socket info only ????
                    for (uint32 l_EnchantmentSlot = 0; l_EnchantmentSlot < EnchantmentSlot::MAX_ENCHANTMENT_SLOT; ++l_EnchantmentSlot)
                    {
                        if (uint32 l_EnchantId = l_TabItem->GetEnchantmentId(EnchantmentSlot(l_EnchantmentSlot)))
                        {
                            l_Data << uint32(l_EnchantmentSlot);            ///< Socket Index
                            l_Data << uint32(l_EnchantId);                  ///< Socket Enchant ID
                        }
                    }
                    */

                    l_Data.WriteBit(0);                                     ///< Is Locked
                    l_Data.FlushBits();
                }
            }
        }
    }

    l_Data.WriteBit(p_WithContent && p_WithTabInfo);                        ///< Is Full Update
    l_Data.FlushBits();

    p_Session->SendPacket(&l_Data);
}

void Guild::SendBankTabText(WorldSession* session, uint8 tabId) const
{
     if (BankTab const* tab = GetBankTab(tabId))
        tab->SendText(this, session);
}

void Guild::SendPermissions(WorldSession* p_Session) const
{
    uint64 l_Guid = p_Session->GetPlayer()->GetGUID();
    uint32 l_RankID = p_Session->GetPlayer()->GetRank();

    WorldPacket l_Data(SMSG_GUILD_PERMISSIONS_QUERY_RESULTS);

    l_Data << uint32(l_RankID);
    l_Data << uint32(_GetRankRights(l_RankID));
    l_Data << uint32(_GetMemberRemainingMoney(l_Guid));
    l_Data << uint32(GetPurchasedTabsSize());
    l_Data << uint32(GUILD_BANK_MAX_TABS);

    for (uint8 l_TabID = 0; l_TabID < GUILD_BANK_MAX_TABS; ++l_TabID)
    {
        l_Data << uint32(_GetRankBankTabRights(l_RankID, l_TabID));
        l_Data << uint32(_GetMemberRemainingSlots(l_Guid, l_TabID));
    }

    p_Session->SendPacket(&l_Data);

    sLog->outDebug(LOG_FILTER_GUILD, "WORLD: Sent (SMSG_GUILD_PERMISSIONS_QUERY_RESULTS)");
}

void Guild::SendMoneyInfo(WorldSession * p_Session) const
{
    WorldPacket l_Data(SMSG_GUILD_BANK_REMAINING_WITHDRAW_MONEY, 8);
    l_Data << uint64(_GetMemberRemainingMoney(p_Session->GetPlayer()->GetGUID()));
    p_Session->SendPacket(&l_Data);
}

void Guild::SendLoginInfo(WorldSession * p_Session)
{
    /*
        Login sequence:
          SMSG_GUILD_EVENT_MOTD
          SMSG_GUILD_RANK
          -- learn perks
          SMSG_GUILD_ACHIEVEMENT_DATA
          SMSG_GUILD_MEMBER_DAILY_RESET // bank withdrawal reset
          SMSG_GUILD_SEND_PLAYER_LOGIN_STATUS
          */

    WorldPacket l_Data(SMSG_GUILD_EVENT_MOTD, 1 + 1 + m_motd.size());

    l_Data.WriteBits(m_motd.size(), 10);
    l_Data.FlushBits();

    l_Data.WriteString(m_motd);

    p_Session->SendPacket(&l_Data);

    HandleGuildRanks(p_Session);

    l_Data.Initialize(SMSG_GUILD_MEMBER_DAILY_RESET, 0);  // tells the client to request bank withdrawal limit
    p_Session->SendPacket(&l_Data);

    if (!sWorld->getBoolConfig(CONFIG_GUILD_LEVELING_ENABLED))
        return;

    for (uint32 i = 0; i < sGuildPerkSpellsStore.GetNumRows(); ++i)
        if (GuildPerkSpellsEntry const* entry = sGuildPerkSpellsStore.LookupEntry(i))
            p_Session->GetPlayer()->learnSpell(entry->SpellId, true);

    GetAchievementMgr().SendAllAchievementData(p_Session->GetPlayer());

    l_Data.Initialize(SMSG_GUILD_EVENT_PRESENCE_CHANGE);
    l_Data.appendPackGUID(p_Session->GetPlayer()->GetGUID());           ///< Guid
    l_Data << uint32(g_RealmID);                                          ///< Virtual Realm Address
    l_Data.WriteBits(strlen(p_Session->GetPlayer()->GetName()), 6);     ///< Name
    l_Data.WriteBit(false);                                             ///< Mobile
    l_Data.WriteBit(true);                                              ///< Logged On
    l_Data.FlushBits();

    if (strlen(p_Session->GetPlayer()->GetName()) > 0)
        l_Data.append(p_Session->GetPlayer()->GetName(), strlen(p_Session->GetPlayer()->GetName()));

    BroadcastPacket(&l_Data);
}

void Guild::SendGuildRecipes(WorldSession * p_Session) const
{
    WorldPacket l_Data(SMSG_GUILD_KNOWN_RECIPES);

    l_Data << uint32(0);

    /*var count = packet.ReadBits("Count", 15);

    for (int i = 0; i < count; ++i)
    {
        packet.ReadUInt32("Skill Id", i);        // iterate all SkillLineAbility.dbc rows:
        for (int j = 0; j < 300; ++j)            // if (entry->skillId != "Skill Id") continue;
            packet.ReadByte("Bit Index", i, j);  // if (mask[entry->col13 / 8] & (entry->col13 & 0x7)) recipe_spell_id: entry->spellId
    }*/

    p_Session->SendPacket(&l_Data);
}

void Guild::SendMemberLeave(WorldSession * p_Session, uint64 p_PlayerGuid, bool p_Kicked)
{
    if (!p_Session)
        return;

    Player* l_KickerPlayer = p_Session->GetPlayer();

    if (!l_KickerPlayer)
        return;

    Member * l_Member = GetMember(p_PlayerGuid);

    if (!l_Member)
        return;

    if (l_Member)
    {
        WorldPacket l_Data(SMSG_GUILD_EVENT_PLAYER_LEFT, 20);

        l_Data.WriteBit(p_Kicked);                                  ///< Removed
        l_Data.WriteBits(l_Member->GetName().size(), 6);            ///< Leaver Name

        if (p_Kicked)
        {
            l_Data.WriteBits(strlen(l_KickerPlayer->GetName()), 6); ///< RemoverName
            l_Data.FlushBits();

            l_Data.appendPackGUID(l_KickerPlayer->GetGUID());       ///< Remover GUID
            l_Data << uint32(g_RealmID);                            ///< Remover Virtual Realm Address

            if (l_KickerPlayer->GetName())
                l_Data.WriteString(l_KickerPlayer->GetName());      ///< RemoverName
        }
        else
        {
            l_Data.FlushBits();
        }

        l_Data.appendPackGUID(p_PlayerGuid);                        ///< Leaver GUID
        l_Data << uint32(g_RealmID);                                ///< Leaver Virtual Realm Address

        l_Data.WriteString(l_Member->GetName());                    ///< Leaver Name

        BroadcastPacket(&l_Data);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Loading methods
bool Guild::LoadFromDB(Field * p_Fields)
{
    m_id            = p_Fields[0].GetUInt32();
    m_name          = p_Fields[1].GetString();
    m_leaderGuid    = MAKE_NEW_GUID(p_Fields[2].GetUInt32(), 0, HIGHGUID_PLAYER);
    m_emblemInfo.LoadFromDB(p_Fields);
    m_info          = p_Fields[8].GetString();
    m_motd          = p_Fields[9].GetString();
    m_createdDate   = time_t(p_Fields[10].GetUInt32());
    m_bankMoney     = p_Fields[11].GetUInt64();

    uint8 l_PurchasedTabs = uint8(p_Fields[12].GetUInt64());

    if (l_PurchasedTabs > GUILD_BANK_MAX_TABS)
        l_PurchasedTabs = GUILD_BANK_MAX_TABS;

    m_bankTabs.resize(l_PurchasedTabs);

    for (uint8 i = 0; i < l_PurchasedTabs; ++i)
        m_bankTabs[i] = new BankTab(m_id, i);

    _CreateLogHolders();
    return true;
}

void Guild::LoadRankFromDB(Field* fields)
{
    RankInfo rankInfo(m_id);

    rankInfo.LoadFromDB(fields);

    m_ranks.push_back(rankInfo);
}

bool Guild::LoadMemberFromDB(Field* fields)
{
    uint32 lowguid = fields[1].GetUInt32();
    Member *member = new Member(m_id, MAKE_NEW_GUID(lowguid, 0, HIGHGUID_PLAYER), fields[2].GetUInt8());
    if (!member->LoadFromDB(fields))
    {
        _DeleteMemberFromDB(lowguid);
        delete member;
        return false;
    }
    m_members[lowguid] = member;
    return true;
}

void Guild::LoadBankRightFromDB(Field* fields)
{
                                           // rights             slots
    GuildBankRightsAndSlots rightsAndSlots(fields[3].GetUInt8(), fields[4].GetUInt32());
                                  // rankId             tabId
    _SetRankBankTabRightsAndSlots(fields[2].GetUInt8(), fields[1].GetUInt8(), rightsAndSlots, false);
}

bool Guild::LoadEventLogFromDB(Field* fields)
{
    if (m_eventLog->CanInsert())
    {
        m_eventLog->LoadEvent(new EventLogEntry(
            m_id,                                       // guild id
            fields[1].GetUInt32(),                      // guid
            time_t(fields[6].GetUInt32()),              // timestamp
            GuildEventLogTypes(fields[2].GetUInt8()),   // event type
            fields[3].GetUInt32(),                      // player guid 1
            fields[4].GetUInt32(),                      // player guid 2
            fields[5].GetUInt8()));                     // rank
        return true;
    }
    return false;
}

bool Guild::LoadBankEventLogFromDB(Field* fields)
{
    uint8 dbTabId = fields[1].GetUInt8();
    bool isMoneyTab = (dbTabId == GUILD_BANK_MONEY_LOGS_TAB);
    if (dbTabId < GetPurchasedTabsSize() || isMoneyTab)
    {
        uint8 tabId = isMoneyTab ? uint8(GUILD_BANK_MAX_TABS) : dbTabId;
        LogHolder* pLog = m_bankEventLog[tabId];
        if (pLog->CanInsert())
        {
            uint32 guid = fields[2].GetUInt32();
            GuildBankEventLogTypes eventType = GuildBankEventLogTypes(fields[3].GetUInt8());
            if (BankEventLogEntry::IsMoneyEvent(eventType))
            {
                if (!isMoneyTab)
                {
                    sLog->outError(LOG_FILTER_GUILD, "GuildBankEventLog ERROR: MoneyEvent(LogGuid: %u, Guild: %u) does not belong to money tab (%u), ignoring...", guid, m_id, dbTabId);
                    return false;
                }
            }
            else if (isMoneyTab)
            {
                sLog->outError(LOG_FILTER_GUILD, "GuildBankEventLog ERROR: non-money event (LogGuid: %u, Guild: %u) belongs to money tab, ignoring...", guid, m_id);
                return false;
            }
            pLog->LoadEvent(new BankEventLogEntry(
                m_id,                                   // guild id
                guid,                                   // guid
                time_t(fields[8].GetUInt32()),          // timestamp
                dbTabId,                                // tab id
                eventType,                              // event type
                fields[4].GetUInt32(),                  // player guid
                fields[5].GetUInt32(),                  // item or money
                fields[6].GetUInt16(),                  // itam stack count
                fields[7].GetUInt8()));                 // dest tab id
        }
    }
    return true;
}

bool Guild::LoadBankTabFromDB(Field* fields)
{
    uint8 tabId = fields[1].GetUInt8();
    if (tabId >= GetPurchasedTabsSize())
    {
        sLog->outError(LOG_FILTER_GUILD, "Invalid tab (tabId: %u) in guild bank, skipped.", tabId);
        return false;
    }
    return m_bankTabs[tabId]->LoadFromDB(fields);
}

bool Guild::LoadBankItemFromDB(Field* fields)
{
    uint8 tabId = fields[17].GetUInt8();
    if (tabId >= GetPurchasedTabsSize())
    {
        sLog->outError(LOG_FILTER_GUILD, "Invalid tab for item (GUID: %u, id: #%u) in guild bank, skipped.",
            fields[19].GetUInt32(), fields[20].GetUInt32());
        return false;
    }
    return m_bankTabs[tabId]->LoadItemFromDB(fields);
}

bool Guild::LoadGuildChallengesFromDB(Field* p_Fields)
{
    int32 l_ChallengeType = p_Fields[1].GetInt32();
    int32 l_ChallengeCount = p_Fields[2].GetInt32();

    if (l_ChallengeType >= ChallengeMax)
        return false;

    m_ChallengeCount[l_ChallengeType] = l_ChallengeCount;
    return true;
}

// Validates guild data loaded from database. Returns false if guild should be deleted.
bool Guild::Validate()
{
    // Validate ranks data
    // GUILD RANKS represent a sequence starting from 0 = GUILD_MASTER (ALL PRIVILEGES) to max 9 (lowest privileges).
    // The lower rank id is considered higher rank - so promotion does rank-- and demotion does rank++
    // Between ranks in sequence cannot be gaps - so 0, 1, 2, 4 is impossible
    // Min ranks count is 5 and max is 10.
    bool broken_ranks = false;
    if (_GetRanksSize() < GUILD_RANKS_MIN_COUNT || _GetRanksSize() > GUILD_RANKS_MAX_COUNT)
    {
        sLog->outError(LOG_FILTER_GUILD, "Guild %u has invalid number of ranks, creating new...", m_id);
        broken_ranks = true;
    }
    else
    {
        for (uint8 rankId = 0; rankId < _GetRanksSize(); ++rankId)
        {
            RankInfo* rankInfo = GetRankInfo(rankId);
            if (rankInfo->GetId() != rankId)
            {
                sLog->outError(LOG_FILTER_GUILD, "Guild %u has broken rank id %u, creating default set of ranks...", m_id, rankId);
                broken_ranks = true;
            }
        }
    }

    if (broken_ranks)
    {
        m_ranks.clear();
        _CreateDefaultGuildRanks(DEFAULT_LOCALE);
    }

    // Validate members' data
    for (Members::iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
        if (itr->second->GetRankId() > _GetRanksSize())
            itr->second->ChangeRank(_GetLowestRankId());

    // Repair the structure of the guild.
    // If the guildmaster doesn't exist or isn't member of the guild
    // attempt to promote another member.
    Member* pLeader = GetMember(m_leaderGuid);
    if (!pLeader)
    {
        DeleteMember(m_leaderGuid);
        // If no more members left, disband guild
        if (m_members.empty())
        {
            //Disband();
            return false;
        }
    }
    else if (!pLeader->IsRank(GR_GUILDMASTER))
        _SetLeaderGUID(pLeader);

    // Check config if multiple guildmasters are allowed
    if (!ConfigMgr::GetBoolDefault("Guild.AllowMultipleGuildMaster", 0))
        for (Members::iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
            if (itr->second->GetRankId() == GR_GUILDMASTER && !itr->second->IsSamePlayer(m_leaderGuid))
                itr->second->ChangeRank(GR_OFFICER);

    _UpdateAccountsNumber();
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Broadcasts
void Guild::BroadcastToGuild(WorldSession* session, bool officerOnly, const std::string& msg, uint32 language) const
{
    if (session && session->GetPlayer() && _HasRankRight(session->GetPlayer(), officerOnly ? GR_RIGHT_OFFCHATSPEAK : GR_RIGHT_GCHATSPEAK))
    {
        WorldPacket data;
        ChatHandler::FillMessageData(&data, session, officerOnly ? CHAT_MSG_OFFICER : CHAT_MSG_GUILD, language, NULL, 0, msg.c_str(), NULL);
        for (Members::const_iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
        {
            if (Player* player = itr->second->FindPlayer())
            {
                if (player->GetSession() && _HasRankRight(player, officerOnly ? GR_RIGHT_OFFCHATLISTEN : GR_RIGHT_GCHATLISTEN) &&
                    !player->GetSocial()->HasIgnore(session->GetPlayer()->GetGUIDLow()))
                    player->GetSession()->SendPacket(&data);
            }
           else if (Player* player = ObjectAccessor::FindPlayerInOrOutOfWorld(itr->second->GetGUID()))
           {
               if (player->GetSession() && _HasRankRight(player, officerOnly ? GR_RIGHT_OFFCHATLISTEN : GR_RIGHT_GCHATLISTEN) &&
                   !player->GetSocial()->HasIgnore(session->GetPlayer()->GetGUIDLow()) &&
                   player->GetSession()->GetInterRealmBG())
               {
                   player->GetSession()->SendPacket(&data, true);
               }
           }
        }
    }
}

void Guild::BroadcastAddonToGuild(WorldSession* session, bool officerOnly, const std::string& msg, const std::string& prefix) const
{
    if (session && session->GetPlayer() && _HasRankRight(session->GetPlayer(), officerOnly ? GR_RIGHT_OFFCHATSPEAK : GR_RIGHT_GCHATSPEAK))
    {
        WorldPacket data;
        ChatHandler::FillMessageData(&data, session, officerOnly ? CHAT_MSG_OFFICER : CHAT_MSG_GUILD, CHAT_MSG_ADDON, NULL, 0, msg.c_str(), NULL, prefix.c_str());
        for (Members::const_iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
            if (Player* player = itr->second->FindPlayer())
                if (player->GetSession() && _HasRankRight(player, officerOnly ? GR_RIGHT_OFFCHATLISTEN : GR_RIGHT_GCHATLISTEN) &&
                    !player->GetSocial()->HasIgnore(session->GetPlayer()->GetGUIDLow()) &&
                    player->GetSession()->IsAddonRegistered(prefix))
                        player->GetSession()->SendPacket(&data);
    }
}

void Guild::BroadcastPacketToRank(WorldPacket* packet, uint8 rankId) const
{
    for (Members::const_iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
        if (itr->second->IsRank(rankId))
            if (Player* player = itr->second->FindPlayer())
                player->GetSession()->SendPacket(packet);
}

void Guild::BroadcastPacket(WorldPacket* packet) const
{
    for (Members::const_iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
        if (Player* player = itr->second->FindPlayer())
            player->GetSession()->SendPacket(packet);
}

void Guild::MassInviteToEvent(WorldSession* /*p_Session*/, uint32 /*p_MinLevel*/, uint32 /*p_MaxLevel*/, uint32 /*p_MinRank*/)
{
    // Finish me. Thank still not done in 2016 !
    /*uint32 count = 0;

    WorldPacket data(SMSG_CALENDAR_FILTER_GUILD);
    data << uint32(count); // count placeholder

    for (Members::const_iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
    {
        // not sure if needed, maybe client checks it as well
        if (count >= CALENDAR_MAX_INVITES)
        {
            if (Player* player = p_Session->GetPlayer())
                sCalendarMgr->SendCalendarCommandResult(player->GetGUID(), CALENDAR_ERROR_INVITES_EXCEEDED);
            return;
        }

        Member* member = itr->second;
        uint32 level = Player::GetLevelFromDB(member->GetGUID());

        if (member->GetGUID() != p_Session->GetPlayer()->GetGUID() && level >= p_MinLevel && level <= p_MaxLevel && member->IsRankNotLower(p_MinRank))
        {
            data.appendPackGUID(member->GetGUID());
            data << uint8(0); // unk
            ++count;
        }
    }

    data.put<uint32>(0, count);

    p_Session->SendPacket(&data);*/
}

///////////////////////////////////////////////////////////////////////////////
// Members handling
bool Guild::AddMember(uint64 p_Guid, uint8 p_RankID)
{
    Player* l_Player = ObjectAccessor::FindPlayer(p_Guid);

    /// Player cannot be in guild
    if (l_Player)
    {
        if (l_Player->GetGuildId() != 0)
            return false;
    }
    else if (Player::GetGuildIdFromDB(p_Guid) != 0)
        return false;

    /// Remove all player signs from another petitions
    /// This will be prevent attempt to join many guilds and corrupt guild data integrity
    Player::RemovePetitionsAndSigns(p_Guid, GUILD_CHARTER_TYPE);

    uint32 l_LowGuid = GUID_LOPART(p_Guid);

    /// If rank was not passed, assign lowest possible rank
    if (p_RankID == GUILD_RANK_NONE)
        p_RankID = _GetLowestRankId();

    Member* l_Member = new Member(m_id, p_Guid, p_RankID);

    if (l_Player)
        l_Member->SetStats(l_Player);
    else
    {
        bool l_Ok = false;

        /// Player must exist
        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_DATA_FOR_GUILD);

        l_Stmt->setUInt32(0, l_LowGuid);

        if (PreparedQueryResult l_Result = CharacterDatabase.Query(l_Stmt))
        {
            Field * l_Fields = l_Result->Fetch();

            l_Member->SetStats(
                l_Fields[0].GetString(),
                l_Fields[1].GetUInt8(),
                l_Fields[2].GetUInt8(),
                l_Fields[3].GetUInt16(),
                l_Fields[4].GetUInt32());

            l_Ok = l_Member->CheckStats();
        }

        if (!l_Ok)
        {
            delete l_Member;
            return false;
        }
    }

    m_members[l_LowGuid] = l_Member;

    SQLTransaction l_Transaction(NULL);
    l_Member->SaveToDB(l_Transaction);

    /// If player not in game data in will be loaded from guild tables, so no need to update it!
    if (l_Player)
    {
        l_Player->SetInGuild(m_id);
        l_Player->SetRank(p_RankID);
        l_Player->SetGuildLevel(GetAchievementMgr().GetAchievementPoints());
        l_Player->SetGuildIdInvited(0);

        for (uint32 l_I = 0; l_I < sGuildPerkSpellsStore.GetNumRows(); ++l_I)
        {
            if (const GuildPerkSpellsEntry * l_Entry = sGuildPerkSpellsStore.LookupEntry(l_I))
                l_Player->learnSpell(l_Entry->SpellId, true);
        }

        if (FactionEntry const* l_FactionEntry = sFactionStore.LookupEntry(REP_GUILD))
        {
            ReputationRank l_Rank = l_Player->GetReputationMgr().GetRank(l_FactionEntry);

            if (l_Rank > REP_NEUTRAL)
            {
                l_Rank = (ReputationRank)(l_Rank - 1);
                uint32 l_ReputationPts = 0;

                for (int32 l_I = REP_NEUTRAL; l_I < l_Rank; ++l_I)
                    l_ReputationPts += ReputationMgr::PointsInRank[l_I];

                l_Player->GetReputationMgr().SetReputation(l_FactionEntry, l_ReputationPts);
            }
            else
                l_Player->GetReputationMgr().SetReputation(l_FactionEntry, 0);
        }
    }

    _UpdateAccountsNumber();

    /// Call scripts if member was successfully added (and stored to database)
    sScriptMgr->OnGuildAddMember(this, l_Player, p_RankID);

    return true;
}

void Guild::DeleteMember(uint64 p_Guid, bool p_IsDisbanding, bool p_IsKicked, bool p_CanDeleteGuild)
{
    uint32 l_LowGuid = GUID_LOPART(p_Guid);
    Player * l_Player = ObjectAccessor::FindPlayer(p_Guid);

    /// Guild master can be deleted when loading guild and guid doesn't exist in characters table
    /// or when he is removed from guild by gm command
    if (m_leaderGuid == p_Guid && !p_IsDisbanding)
    {
        Member * l_OldLeader = NULL;
        Member * l_NewLeader = NULL;

        for (Guild::Members::iterator l_I = m_members.begin(); l_I != m_members.end(); ++l_I)
        {
            if (l_I->first == l_LowGuid)
                l_OldLeader = l_I->second;

            else if (!l_NewLeader || l_NewLeader->GetRankId() > l_I->second->GetRankId())
                l_NewLeader = l_I->second;
        }
        if (!l_NewLeader)
        {
            Disband();

            if (p_CanDeleteGuild)
                delete this;

            return;
        }

        _SetLeaderGUID(l_NewLeader);

        Player * l_NewLeaderPlayer = l_NewLeader->FindPlayer();

        /// If player not online data in data field will be loaded from guild tabs no need to update it !!
        if (l_NewLeaderPlayer)
            l_NewLeaderPlayer->SetRank(GR_GUILDMASTER);

        /// If leader does not exist (at guild loading with deleted leader) do not send broadcasts
        if (l_OldLeader)
        {
            WorldPacket l_Data(SMSG_GUILD_EVENT_NEW_LEADER, 80);
            l_Data.WriteBit(false);                                 ///< Self Promoted
            l_Data.WriteBits(l_OldLeader->GetName().size(), 6);     ///< Old Leader Name
            l_Data.WriteBits(l_NewLeader->GetName().size(), 6);     ///< New Leader Name
            l_Data.FlushBits();

            l_Data.appendPackGUID(l_OldLeader->GetGUID());          ///< Old Leader GUID
            l_Data << uint32(g_RealmID);                            ///< Old Leader Virtual Realm Address
            l_Data.appendPackGUID(l_NewLeader->GetGUID());          ///< New Leader GUID
            l_Data << uint32(g_RealmID);                            ///< New Leader Virtual Realm Address

            l_Data.WriteString(l_OldLeader->GetName());             ///< Old Leader Name
            l_Data.WriteString(l_NewLeader->GetName());             ///< New Leader Name

            BroadcastPacket(&l_Data);

            SendMemberLeave(l_NewLeaderPlayer ? l_NewLeaderPlayer->GetSession() : 0, l_OldLeader->GetGUID(), true);
        }
    }

    /// Call script on remove before member is actually removed from guild (and database)
    sScriptMgr->OnGuildRemoveMember(this, l_Player, p_IsDisbanding, p_IsKicked);

    if (Member* member = GetMember(p_Guid))
        delete member;

    m_members.erase(l_LowGuid);

    /// If player not online data in data field will be loaded from guild tabs no need to update it !!
    if (l_Player)
    {
        l_Player->SetInGuild(0);
        l_Player->SetRank(0);
        l_Player->SetGuildLevel(0);

        for (uint32 l_I = 0; l_I < sGuildPerkSpellsStore.GetNumRows(); ++l_I)
        {
            if (GuildPerkSpellsEntry const* entry = sGuildPerkSpellsStore.LookupEntry(l_I))
                l_Player->removeSpell(entry->SpellId, false, false);
        }
    }

    _DeleteMemberFromDB(l_LowGuid);

    if (!p_IsDisbanding)
        _UpdateAccountsNumber();
}

bool Guild::ChangeMemberRank(uint64 p_GUID, uint8 p_NewRank)
{
    if (p_NewRank <= _GetLowestRankId())                    // Validate rank (allow only existing ranks)
    {
        if (Member* l_Member = GetMember(p_GUID))
        {
            l_Member->ChangeRank(p_NewRank);
            return true;
        }
    }

    return false;
}

bool Guild::IsMember(uint64 p_Guid)
{
    Members::const_iterator l_It = m_members.find(GUID_LOPART(p_Guid));
    return l_It != m_members.end();
}

///////////////////////////////////////////////////////////////////////////////
// Bank (items move)
void Guild::SwapItems(Player * p_Player, uint8 p_TabID, uint8 p_SlotID, uint8 p_DestTabID, uint8 p_DestSlotID, uint32 splitedAmount)
{
    if (p_TabID >= GetPurchasedTabsSize() || p_SlotID >= GUILD_BANK_MAX_SLOTS ||
        p_DestTabID >= GetPurchasedTabsSize() || p_DestSlotID >= GUILD_BANK_MAX_SLOTS)
        return;

    if (p_TabID == p_DestTabID && p_SlotID == p_DestSlotID)
        return;

    BankMoveItemData l_From(this, p_Player, p_TabID, p_SlotID);
    BankMoveItemData l_To(this, p_Player, p_DestTabID, p_DestSlotID);

    _MoveItems(&l_From, &l_To, splitedAmount);
}

void Guild::SwapItemsWithInventory(Player * p_Player, bool p_ToChar, uint8 p_TabID, uint8 p_SlotID, uint8 p_PlayerBag, uint8 p_PlayerSlotID, uint32 p_SplitedAmount)
{
    if ((p_SlotID >= GUILD_BANK_MAX_SLOTS && p_SlotID != NULL_SLOT) || p_TabID >= GetPurchasedTabsSize())
        return;

    BankMoveItemData l_BankData(this, p_Player, p_TabID, p_SlotID);
    PlayerMoveItemData l_CharData(this, p_Player, p_PlayerBag, p_PlayerSlotID);

    if (p_ToChar)
        _MoveItems(&l_BankData, &l_CharData, p_SplitedAmount);
    else
        _MoveItems(&l_CharData, &l_BankData, p_SplitedAmount);
}

void Guild::AutoStoreItemInInventory(Player * p_Player, uint8 p_TabID, uint8 p_SlotID, uint32 p_Amount)
{
    if ((p_SlotID >= GUILD_BANK_MAX_SLOTS && p_SlotID != NULL_SLOT) || p_TabID >= GetPurchasedTabsSize())
        return;

    Item * l_Item = _GetItem(p_TabID, p_SlotID);

    if (!l_Item)
    {
        p_Player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL);
        return;
    }

    ItemPosCountVec l_Dest;
    InventoryResult l_Result = p_Player->CanStoreItem(NULL_BAG, NULL_SLOT, l_Dest, l_Item, false);

    if (l_Result != EQUIP_ERR_OK)
    {
        p_Player->SendEquipError(l_Result, l_Item, NULL);
        return;
    }

    if (_GetMemberRemainingSlots(p_Player->GetGUID(), p_TabID) == 0)
        return;

    SQLTransaction l_Transaction = CharacterDatabase.BeginTransaction();

    if (BankTab * l_Tab = GetBankTab(p_TabID))
        l_Tab->SetItem(l_Transaction, p_SlotID, NULL);

    _DecreaseMemberRemainingSlots(l_Transaction, p_Player->GetGUID(), p_TabID);
    _LogBankEvent(l_Transaction, GUILD_BANK_LOG_MOVE_ITEM, p_TabID, p_Player->GetGUIDLow(), l_Item->GetEntry(), p_Amount, 0);

    CharacterDatabase.CommitTransaction(l_Transaction);

    p_Player->StoreItem(l_Dest, l_Item, true);
    SendBankList(p_Player->GetSession(), p_TabID, true, true);
}

///////////////////////////////////////////////////////////////////////////////
// Bank tabs
void Guild::SetBankTabText(uint8 p_TabID, const std::string& p_Text)
{
    if (BankTab * l_Tab = GetBankTab(p_TabID))
    {
        l_Tab->SetText(p_Text);

        WorldPacket l_Data(SMSG_GUILD_EVENT_TAB_TEXT_CHANGED, 4);
        l_Data << uint32(p_TabID);

        BroadcastPacket(&l_Data);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Private methods
void Guild::_CreateLogHolders()
{
    m_eventLog = new LogHolder(m_id, sWorld->getIntConfig(CONFIG_GUILD_EVENT_LOG_COUNT));
    for (uint8 tabId = 0; tabId <= GUILD_BANK_MAX_TABS; ++tabId)
        m_bankEventLog[tabId] = new LogHolder(m_id, sWorld->getIntConfig(CONFIG_GUILD_BANK_EVENT_LOG_COUNT));
}

bool Guild::_CreateNewBankTab()
{
    if (GetPurchasedTabsSize() >= GUILD_BANK_MAX_TABS)
        return false;

    uint8 tabId = GetPurchasedTabsSize();                      // Next free id
    m_bankTabs.push_back(new BankTab(m_id, tabId));

    PreparedStatement * l_Stmt = NULL;
    SQLTransaction l_Transaction = CharacterDatabase.BeginTransaction();

    l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_BANK_TAB);
    l_Stmt->setUInt32(0, m_id);
    l_Stmt->setUInt8 (1, tabId);
    l_Transaction->Append(l_Stmt);

    l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GUILD_BANK_TAB);
    l_Stmt->setUInt32(0, m_id);
    l_Stmt->setUInt8 (1, tabId);
    l_Transaction->Append(l_Stmt);

    CharacterDatabase.CommitTransaction(l_Transaction);
    return true;
}

void Guild::_CreateDefaultGuildRanks(LocaleConstant loc)
{
    PreparedStatement* stmt = NULL;

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_RANKS);
    stmt->setUInt32(0, m_id);
    CharacterDatabase.Execute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_BANK_RIGHTS);
    stmt->setUInt32(0, m_id);
    CharacterDatabase.Execute(stmt);

    _CreateRank(sObjectMgr->GetTrinityString(LANG_GUILD_MASTER,   loc), GR_RIGHT_ALL);
    _CreateRank(sObjectMgr->GetTrinityString(LANG_GUILD_OFFICER,  loc), GR_RIGHT_ALL);
    _CreateRank(sObjectMgr->GetTrinityString(LANG_GUILD_VETERAN,  loc), GR_RIGHT_GCHATLISTEN | GR_RIGHT_GCHATSPEAK);
    _CreateRank(sObjectMgr->GetTrinityString(LANG_GUILD_MEMBER,   loc), GR_RIGHT_GCHATLISTEN | GR_RIGHT_GCHATSPEAK);
    _CreateRank(sObjectMgr->GetTrinityString(LANG_GUILD_INITIATE, loc), GR_RIGHT_GCHATLISTEN | GR_RIGHT_GCHATSPEAK);
}

void Guild::_CreateRank(const std::string& p_Name, uint32 p_Rights)
{
    if (_GetRanksSize() >= GUILD_RANKS_MAX_COUNT)
        return;

    // Ranks represent sequence 0, 1, 2, ... where 0 means guildmaster
    uint32 p_NewRankId = _GetRanksSize();

    RankInfo p_Info(m_id, p_NewRankId, p_Name, p_Rights, 0);
    m_ranks.push_back(p_Info);

    SQLTransaction l_Transaction = CharacterDatabase.BeginTransaction();
    for (uint8 l_I = 0; l_I < GetPurchasedTabsSize(); ++l_I)
    {
        /// Create bank rights with default values
        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GUILD_BANK_RIGHT_DEFAULT);
        l_Stmt->setUInt32(0, m_id);
        l_Stmt->setUInt8 (1, l_I);
        l_Stmt->setUInt8 (2, p_NewRankId);

        l_Transaction->Append(l_Stmt);
    }

    p_Info.SaveToDB(l_Transaction);
    CharacterDatabase.CommitTransaction(l_Transaction);

    WorldPacket l_Data(SMSG_GUILD_EVENT_RANK_CHANGED);
    l_Data << uint32(m_id);
    BroadcastPacket(&l_Data);
}

// Updates the number of accounts that are in the guild
// Player may have many characters in the guild, but with the same account
void Guild::_UpdateAccountsNumber()
{
    // We use a set to be sure each element will be unique
    std::set<uint32> accountsIdSet;
    for (Members::const_iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
        accountsIdSet.insert(itr->second->GetAccountId());

    m_accountsNumber = accountsIdSet.size();
}

// Detects if player is the guild master.
// Check both leader guid and player's rank (otherwise multiple feature with
// multiple guild masters won't work)
bool Guild::_IsLeader(Player* player) const
{
    if (player->GetGUID() == m_leaderGuid)
        return true;
    if (const Member* member = GetMember(player->GetGUID()))
        return member->IsRank(GR_GUILDMASTER);
    return false;
}

void Guild::_DeleteBankItems(SQLTransaction& trans, bool removeItemsFromDB)
{
    for (uint8 tabId = 0; tabId < GetPurchasedTabsSize(); ++tabId)
    {
        m_bankTabs[tabId]->Delete(trans, removeItemsFromDB);
        delete m_bankTabs[tabId];
        m_bankTabs[tabId] = NULL;
    }
    m_bankTabs.clear();
}

bool Guild::_ModifyBankMoney(SQLTransaction& trans, uint64 amount, bool add)
{
    if (add)
        m_bankMoney += amount;
    else
    {
        // Check if there is enough money in bank.
        if (m_bankMoney < amount)
            return false;
        m_bankMoney -= amount;
    }

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GUILD_BANK_MONEY);
    stmt->setUInt64(0, m_bankMoney);
    stmt->setUInt32(1, m_id);
    trans->Append(stmt);

    WorldPacket l_Data(SMSG_GUILD_EVENT_BANK_MONEY_CHANGED, 8);
    l_Data << uint64(m_bankMoney);

    BroadcastPacket(&l_Data);

    return true;
}

void Guild::_SetLeaderGUID(Member* pLeader)
{
    if (!pLeader)
        return;

    m_leaderGuid = pLeader->GetGUID();
    pLeader->ChangeRank(GR_GUILDMASTER);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GUILD_LEADER);
    stmt->setUInt32(0, GUID_LOPART(m_leaderGuid));
    stmt->setUInt32(1, m_id);
    CharacterDatabase.Execute(stmt);
}

void Guild::_SetRankBankMoneyPerDay(uint32 rankId, uint32 moneyPerDay)
{
    if (RankInfo* rankInfo = GetRankInfo(rankId))
    {
        for (Members::iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
            if (itr->second->IsRank(rankId))
                itr->second->ResetMoneyTime();

        rankInfo->SetBankMoneyPerDay(moneyPerDay);
    }
}

void Guild::_SetRankBankTabRightsAndSlots(uint32 rankId, uint8 tabId, GuildBankRightsAndSlots rightsAndSlots, bool saveToDB)
{
    if (tabId >= GetPurchasedTabsSize())
        return;

    if (RankInfo* rankInfo = GetRankInfo(rankId))
    {
        for (Members::iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
            if (itr->second->IsRank(rankId))
                itr->second->ResetTabTimes();

        rankInfo->SetBankTabSlotsAndRights(tabId, rightsAndSlots, saveToDB);
    }
}

inline std::string Guild::_GetRankName(uint32 rankId) const
{
    if (const RankInfo* rankInfo = GetRankInfo(rankId))
        return rankInfo->GetName();
    return "<unknown>";
}

inline uint32 Guild::_GetRankRights(uint32 rankId) const
{
    if (const RankInfo* rankInfo = GetRankInfo(rankId))
        return rankInfo->GetRights();
    return 0;
}

inline uint32 Guild::_GetRankBankMoneyPerDay(uint32 rankId) const
{
    if (const RankInfo* rankInfo = GetRankInfo(rankId))
        return rankInfo->GetBankMoneyPerDay();
    return 0;
}

inline uint32 Guild::_GetRankBankTabSlotsPerDay(uint32 rankId, uint8 tabId) const
{
    if (tabId < GetPurchasedTabsSize())
        if (const RankInfo* rankInfo = GetRankInfo(rankId))
            return rankInfo->GetBankTabSlotsPerDay(tabId);
    return 0;
}

inline uint32 Guild::_GetRankBankTabRights(uint32 rankId, uint8 tabId) const
{
    if (const RankInfo* rankInfo = GetRankInfo(rankId))
        return rankInfo->GetBankTabRights(tabId);
    return 0;
}

inline uint32 Guild::_GetMemberRemainingSlots(uint64 guid, uint8 tabId) const
{
    if (const Member* member = GetMember(guid))
        return member->GetBankRemainingValue(tabId, this);
    return -1;
}

inline uint64 Guild::_GetMemberRemainingMoney(uint64 guid) const
{
    if (const Member* member = GetMember(guid))
    {
        if (member->IsRank(GR_GUILDMASTER))
            return GUILD_WITHDRAW_SLOT_UNLIMITED;

        return member->GetRemainingWithdrawMoney();
    }
    return 0;
}

inline void Guild::_DecreaseMemberRemainingSlots(SQLTransaction& trans, uint64 guid, uint8 tabId)
{
    // Remaining slots must be more then 0
    if (uint32 remainingSlots = _GetMemberRemainingSlots(guid, tabId))
    {
        // Ignore guild master
        if (remainingSlots < uint32(GUILD_WITHDRAW_SLOT_UNLIMITED))
        {
            if (Member* member = GetMember(guid))
                member->DecreaseBankRemainingValue(trans, tabId, 1);
        }
    }
}

inline bool Guild::_MemberHasTabRights(uint64 guid, uint8 tabId, uint32 rights) const
{
    if (const Member* member = GetMember(guid))
    {
        // Leader always has full rights
        if (member->IsRank(GR_GUILDMASTER) || m_leaderGuid == guid)
            return true;
        return (_GetRankBankTabRights(member->GetRankId(), tabId) & rights) == rights;
    }
    return false;
}

// Add new event log record
inline void Guild::_LogEvent(GuildEventLogTypes eventType, uint32 playerGuid1, uint32 playerGuid2, uint8 newRank)
{
    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    m_eventLog->AddEvent(trans, new EventLogEntry(m_id, m_eventLog->GetNextGUID(), eventType, playerGuid1, playerGuid2, newRank));
    CharacterDatabase.CommitTransaction(trans);

    sScriptMgr->OnGuildEvent(this, uint8(eventType), playerGuid1, playerGuid2, newRank);
}

// Add new bank event log record
void Guild::_LogBankEvent(SQLTransaction& trans, GuildBankEventLogTypes eventType, uint8 tabId, uint32 lowguid, uint64 itemOrMoney, uint16 itemStackCount, uint8 destTabId)
{
    if (tabId > GUILD_BANK_MAX_TABS)
        return;

     // not logging moves within the same tab
    if (eventType == GUILD_BANK_LOG_MOVE_ITEM && tabId == destTabId)
        return;

    uint8 dbTabId = tabId;
    if (BankEventLogEntry::IsMoneyEvent(eventType))
    {
        tabId = GUILD_BANK_MAX_TABS;
        dbTabId = GUILD_BANK_MONEY_LOGS_TAB;
    }
    LogHolder* pLog = m_bankEventLog[tabId];
    pLog->AddEvent(trans, new BankEventLogEntry(m_id, pLog->GetNextGUID(), eventType, dbTabId, lowguid, itemOrMoney, itemStackCount, destTabId));

    sScriptMgr->OnGuildBankEvent(this, uint8(eventType), tabId, lowguid, itemOrMoney, itemStackCount, destTabId);
}

inline Item* Guild::_GetItem(uint8 tabId, uint8 slotId) const
{
    if (const BankTab* tab = GetBankTab(tabId))
        return tab->GetItem(slotId);
    return NULL;
}

inline void Guild::_RemoveItem(SQLTransaction& trans, uint8 tabId, uint8 slotId)
{
    if (BankTab* pTab = GetBankTab(tabId))
        pTab->SetItem(trans, slotId, NULL);
}

void Guild::_MoveItems(MoveItemData* pSrc, MoveItemData* pDest, uint32 splitedAmount)
{
    // 1. Initialize source item
    if (!pSrc->InitItem())
        return; // No source item

    // 2. Check source item
    if (!pSrc->CheckItem(splitedAmount))
        return; // Source item or splited amount is invalid
    /*
    if (pItemSrc->GetCount() == 0)
    {
        sLog->outFatal(LOG_FILTER_GENERAL, "Guild::SwapItems: Player %s(GUIDLow: %u) tried to move item %u from tab %u slot %u to tab %u slot %u, but item %u has a stack of zero!",
            player->GetName(), player->GetGUIDLow(), pItemSrc->GetEntry(), tabId, slotId, destTabId, destSlotId, pItemSrc->GetEntry());
        //return; // Commented out for now, uncomment when it's verified that this causes a crash!!
    }
    // */

    // 3. Check destination rights
    if (!pDest->HasStoreRights(pSrc))
        return; // Player has no rights to store item in destination

    // 4. Check source withdraw rights
    if (!pSrc->HasWithdrawRights(pDest))
        return; // Player has no rights to withdraw items from source

    // 5. Check split
    if (splitedAmount)
    {
        // 5.1. Clone source item
        if (!pSrc->CloneItem(splitedAmount))
            return; // Item could not be cloned

        // 5.2. Move splited item to destination
        _DoItemsMove(pSrc, pDest, true, splitedAmount);
    }
    else // 6. No split
    {
        // 6.1. Try to merge items in destination (pDest->GetItem() == NULL)
        if (!_DoItemsMove(pSrc, pDest, false)) // Item could not be merged
        {
            // 6.2. Try to swap items
            // 6.2.1. Initialize destination item
            if (!pDest->InitItem())
                return;

            // 6.2.2. Check rights to store item in source (opposite direction)
            if (!pSrc->HasStoreRights(pDest))
                return; // Player has no rights to store item in source (opposite direction)

            if (!pDest->HasWithdrawRights(pSrc))
                return; // Player has no rights to withdraw item from destination (opposite direction)

            // 6.2.3. Swap items (pDest->GetItem() != NULL)
            _DoItemsMove(pSrc, pDest, true);
        }
    }

    // 7. Send changes
    WorldPacket l_Data(SMSG_GUILD_EVENT_BANK_CONTENTS_CHANGED);
    BroadcastPacket(&l_Data);
}

bool Guild::_DoItemsMove(MoveItemData* pSrc, MoveItemData* pDest, bool sendError, uint32 splitedAmount)
{
    Item* pDestItem = pDest->GetItem();
    bool swap = (pDestItem != NULL);

    Item* pSrcItem = pSrc->GetItem(splitedAmount);
    // 1. Can store source item in destination
    if (!pDest->CanStore(pSrcItem, swap, sendError))
        return false;

    // 2. Can store destination item in source
    if (swap)
        if (!pSrc->CanStore(pDestItem, true, true))
            return false;

    // GM LOG (TODO: move to scripts)
    pDest->LogAction(pSrc);
    if (swap)
        pSrc->LogAction(pDest);

    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    // 3. Log bank events
    pDest->LogBankEvent(trans, pSrc, pSrcItem->GetCount());
    if (swap)
        pSrc->LogBankEvent(trans, pDest, pDestItem->GetCount());

    // 4. Remove item from source
    pSrc->RemoveItem(trans, pDest, splitedAmount);

    // 5. Remove item from destination
    if (swap)
        pDest->RemoveItem(trans, pSrc);

    // 6. Store item in destination
    pDest->StoreItem(trans, pSrcItem);

    // 7. Store item in source
    if (swap)
        pSrc->StoreItem(trans, pDestItem);

    CharacterDatabase.DirectCommitTransaction(trans);
    return true;
}

void Guild::_SendBankContentUpdate(MoveItemData* pSrc, MoveItemData* pDest) const
{
    ASSERT(pSrc->IsBank() || pDest->IsBank());

    uint8 tabId = 0;
    SlotIds slots;
    if (pSrc->IsBank()) // B ->
    {
        tabId = pSrc->GetContainer();
        slots.insert(pSrc->GetSlotId());
        if (pDest->IsBank()) // B -> B
        {
            // Same tab - add destination slots to collection
            if (pDest->GetContainer() == pSrc->GetContainer())
                pDest->CopySlots(slots);
            else // Different tabs - send second message
            {
                SlotIds destSlots;
                pDest->CopySlots(destSlots);
                _SendBankContentUpdate(pDest->GetContainer(), destSlots);
            }
        }
    }
    else if (pDest->IsBank()) // C -> B
    {
        tabId = pDest->GetContainer();
        pDest->CopySlots(slots);
    }

    _SendBankContentUpdate(tabId, slots);
}

void Guild::_SendBankContentUpdate(uint8 tabId, SlotIds slots) const
{
    if (BankTab const* tab = GetBankTab(tabId))
    {
        ByteBuffer tabData;
        WorldPacket data(SMSG_GUILD_BANK_QUERY_RESULTS);
        data.WriteBits(slots.size(), 18);                                           // Item count
        data.WriteBits(0, 21);                                                      // Tab count

        for (SlotIds::const_iterator itr = slots.begin(); itr != slots.end(); ++itr)
        {
            Item const* tabItem = tab->GetItem(*itr);
            uint32 enchantCount = 0;

            if (tabItem)
                for (uint32 enchSlot = 0; enchSlot < MAX_ENCHANTMENT_SLOT; ++enchSlot)
                    if (uint32 enchantId = tabItem->GetEnchantmentId(EnchantmentSlot(enchSlot)))
                        ++enchantCount;

            data.WriteBits(enchantCount, 21);                                       // enchantment count
            data.WriteBit(0);

            tabData << uint32(0);
            tabData << uint32(*itr);
            tabData << uint32(tabItem ? abs(tabItem->GetSpellCharges()) : 0);       // Spell charges
            tabData << uint32(0);
            tabData << uint32(tabItem ? tabItem->GetItemSuffixFactor() : 0);        // SuffixFactor
            tabData << uint32(tabItem ? tabItem->GetEntry() : 0);
            tabData << uint32(0);

            if (tabItem)
            {
                for (uint32 enchSlot = 0; enchSlot < MAX_ENCHANTMENT_SLOT; ++enchSlot)
                {
                    if (uint32 enchantId = tabItem->GetEnchantmentId(EnchantmentSlot(enchSlot)))
                    {
                        tabData << uint32(enchSlot);
                        tabData << uint32(enchantId);
                    }
                }
            }

            tabData << uint32(tabItem ? tabItem->GetItemRandomPropertyId() : 0);
            tabData << uint32(0);
            tabData << uint32(tabItem ? tabItem->GetCount() : 0);                   // ITEM_FIELD_STACK_COUNT
        }

        data.WriteBit(0);
        data.FlushBits();

        if (!tabData.empty())
            data.append(tabData);

        data << uint32(tabId);
        data << uint64(m_bankMoney);

        size_t rempos = data.wpos(); ///< rempos is never read 01/18/16
        data << uint32(-1);                                      // Item withdraw amount, will be filled later

        /*for (Members::const_iterator itr = m_members.begin(); itr != m_members.end(); ++itr)
            if (_MemberHasTabRights(itr->second->GetGUID(), tabId, GUILD_BANK_RIGHT_VIEW_TAB))
                if (Player* player = itr->second->FindPlayer())
                {
                    data.put<uint32>(rempos, uint32(_GetMemberRemainingSlots(player->GetGUID(), tabId)));
                    player->GetSession()->SendPacket(&data);
                }
                */
        sLog->outDebug(LOG_FILTER_GUILD, "WORLD: Sent (SMSG_GUILD_BANK_LIST)");
    }
}

void Guild::_BroadcastEvent(GuildEvents guildEvent, uint64 guid, const char* param1, const char* param2, const char* param3) const
{
    uint8 count = !param3 ? (!param2 ? (!param1 ? 0 : 1) : 2) : 3;

    WorldPacket data;// (SMSG_GUILD_EVENT, 1 + 1 + count + (guid ? 8 : 0));
    data << uint8(guildEvent);
    data << uint8(count);

    if (param3)
        data << param1 << param2 << param3;
    else if (param2)
        data << param1 << param2;
    else if (param1)
        data << param1;

    if (guid)
        data << uint64(guid);

    BroadcastPacket(&data);

    sLog->outDebug(LOG_FILTER_GUILD, "WORLD: Sent SMSG_GUILD_EVENT");
}

void Guild::SendGuildRanksUpdate(uint64 p_OfficierGUID, uint64 p_OtherGUID, uint32 p_RankID)
{
    Member* l_Member = GetMember(p_OtherGUID);
    ASSERT(l_Member);

    WorldPacket l_Data(SMSG_GUILD_SEND_RANK_CHANGE, 100);
    l_Data.appendPackGUID(p_OfficierGUID);
    l_Data.appendPackGUID(p_OtherGUID);
    l_Data << uint32(p_RankID);

    l_Data.WriteBit(p_RankID < l_Member->GetRankId()); // 1 == higher, 0 = lower?
    l_Data.FlushBits();

    BroadcastPacket(&l_Data);

    l_Member->ChangeRank(p_RankID);

    _LogEvent((p_RankID < l_Member->GetRankId()) ? GUILD_EVENT_LOG_DEMOTE_PLAYER : GUILD_EVENT_LOG_PROMOTE_PLAYER, GUID_LOPART(p_OfficierGUID), GUID_LOPART(p_OtherGUID), p_RankID);
}

void Guild::CompleteGuildChallenge(int32 p_ChallengeType)
{
    if (p_ChallengeType >= ChallengeMax)
        return;

    GuildChallengeRewardData const& l_RewardDatas = sObjectMgr->GetGuildChallengeRewardData();

    int32 l_MaxCount = l_RewardDatas[p_ChallengeType].ChallengeCount;
    int32 l_GoldReward = l_RewardDatas[p_ChallengeType].Gold;

    if (m_ChallengeCount[p_ChallengeType] >= l_MaxCount)
        return;

    m_ChallengeCount[p_ChallengeType]++;

    /// SaveToDB

    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_COMPLETE_GUILD_CHALLENGE);
    l_Stmt->setInt32(0, m_ChallengeCount[p_ChallengeType]);
    l_Stmt->setInt32(1, GetId());
    l_Stmt->setInt32(2, p_ChallengeType);
    CharacterDatabase.Execute(l_Stmt);

    /// Reward gold

    DepositMoney(l_GoldReward * GOLD);

    WorldPacket l_Data(SMSG_GUILD_CHALLENGE_COMPLETED, 4 * 4);
    l_Data << int32(p_ChallengeType);
    l_Data << int32(m_ChallengeCount[p_ChallengeType]);
    l_Data << int32(l_MaxCount);
    l_Data << int32(l_GoldReward);
    BroadcastPacket(&l_Data);
}

void Guild::GuildNewsLog::AddNewEvent(GuildNews eventType, time_t date, uint64 playerGuid, uint32 flags, uint32 data)
{
    uint32 id = _newsLog.size();
    GuildNewsEntry& log = _newsLog[id];
    log.EventType = eventType;
    log.PlayerGuid = playerGuid;
    log.Data = data;
    log.Flags = flags;
    log.Date = date;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SAVE_GUILD_NEWS);
    stmt->setUInt32(0, GetGuild()->GetId());
    stmt->setUInt32(1, id);
    stmt->setUInt32(2, log.EventType);
    stmt->setUInt64(3, log.PlayerGuid);
    stmt->setUInt32(4, log.Data);
    stmt->setUInt32(5, log.Flags);
    stmt->setUInt32(6, uint32(log.Date));
    CharacterDatabase.Execute(stmt);

    WorldPacket packet;
    BuildNewsData(id, log, packet);
    GetGuild()->BroadcastPacket(&packet);
}

void Guild::GuildNewsLog::LoadFromDB(PreparedQueryResult result)
{
    if (!result)
        return;
    do
    {
        Field* fields = result->Fetch();
        uint32 id = fields[0].GetInt32();
        GuildNewsEntry& log = _newsLog[id];
        log.EventType = GuildNews(fields[1].GetInt32());
        log.PlayerGuid = fields[2].GetInt64();
        log.Data = fields[3].GetInt32();
        log.Flags = fields[4].GetInt32();
        log.Date = time_t(fields[5].GetInt32());
    }
    while (result->NextRow());
}

void Guild::GuildNewsLog::BuildNewsData(uint32 p_ID, GuildNewsEntry & p_GuildNews, WorldPacket & p_Data)
{
    p_Data.Initialize(SMSG_GUILD_NEWS);
    p_Data << uint32(1);

    p_Data << uint32(p_ID);
    p_Data << uint32(MS::Utilities::WowTime::Encode(p_GuildNews.Date));
    p_Data << uint32(p_GuildNews.EventType);
    p_Data << uint32(p_GuildNews.Flags);                      ///< 1 sticky
    p_Data << uint32(p_GuildNews.Data);                       ///< Data 1
    p_Data << uint32(0);                                      ///< Data 2

    p_Data.appendPackGUID(p_GuildNews.PlayerGuid);
    p_Data << uint32(0);                                      ///< Member List Count

    p_Data.WriteBit(false);                                   ///< Has item loot data
    p_Data.FlushBits();
}

void Guild::GuildNewsLog::BuildNewsData(WorldPacket& p_Data)
{
    p_Data.Initialize(SMSG_GUILD_NEWS);
    p_Data << uint32(_newsLog.size());

    for (GuildNewsLogMap::const_iterator l_It = _newsLog.begin(); l_It != _newsLog.end(); l_It++)
    {
        p_Data << uint32(l_It->first);
        p_Data << uint32(MS::Utilities::WowTime::Encode(l_It->second.Date));
        p_Data << uint32(l_It->second.EventType);
        p_Data << uint32(l_It->second.Flags);                     ///< 1 sticky
        p_Data << uint32(l_It->second.Data);                      ///< Data 1
        p_Data << uint32(0);                                      ///< Data 2

        p_Data.appendPackGUID(l_It->second.PlayerGuid);
        p_Data << uint32(0);                                      ///< Member List Count

        p_Data.WriteBit(false);                                   ///< Has item loot data
        p_Data.FlushBits();
    }
}
