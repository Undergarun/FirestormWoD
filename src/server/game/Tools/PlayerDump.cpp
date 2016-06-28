////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "gamePCH.h"
#include "Common.h"
#include "PlayerDump.h"
#include "DatabaseEnv.h"
#include "UpdateFields.h"
#include "ObjectMgr.h"
#include "AccountMgr.h"
#include "zlib.h"
#include "Config.h"


struct DumpTable
{
    char const* name;
    DumpTableType type;
    std::vector<std::string> columns;
};

static DumpTable g_DumpTables[] =
{
    { "account_achievement",              DTT_ACC_ACH    , {}},
    { "account_achievement_progress",     DTT_ACC_ACH_PRO, {}},
    { "account_spell",                    DTT_ACC_SPELL,   {}},
    { "characters",                       DTT_CHARACTER  , {}},
    { "character_achievement",            DTT_CHAR_TABLE , {}},
    { "character_achievement_progress",   DTT_CHAR_TABLE , {}},
    { "character_archaeology",            DTT_CHAR_TABLE , {}},
    { "character_archaeology_projects",   DTT_CHAR_TABLE , {}},
    { "character_archaeology_sites",      DTT_CHAR_TABLE , {}},
    { "character_action",                 DTT_CHAR_TABLE , {}},
    { "character_aura",                   DTT_CHAR_TABLE , {}},
    { "character_aura_effect",            DTT_CHAR_TABLE , {}},
    { "character_currency",               DTT_CHAR_TABLE , {}},
    { "character_declinedname",           DTT_CHAR_TABLE , {}},
    { "character_equipmentsets",          DTT_EQSET_TABLE, {}},
    { "character_glyphs",                 DTT_CHAR_TABLE , {}},
    { "character_homebind",               DTT_CHAR_TABLE , {}},
    { "character_inventory",              DTT_INVENTORY  , {}},
    { "character_pet",                    DTT_PET        , {}},
    { "character_pet_declinedname",       DTT_PET        , {}},
    { "character_queststatus",            DTT_CHAR_TABLE , {}},
    { "character_queststatus_rewarded",   DTT_CHAR_TABLE , {}},
    { "character_rates",                  DTT_CHAR_TABLE , {}},
    { "character_reputation",             DTT_CHAR_TABLE , {}},
    { "character_skills",                 DTT_CHAR_TABLE , {}},
    { "character_spell",                  DTT_CHAR_TABLE , {}},
    { "character_spell_cooldown",         DTT_CHAR_TABLE , {}},
    { "character_talent",                 DTT_CHAR_TABLE , {}},
    { "character_garrison",               DTT_GARR_MAIN  , {}},
    { "character_garrison_building",      DTT_GARR_B     , {}},
    { "character_garrison_follower",      DTT_GARR_F     , {}},
    { "character_garrison_mission",       DTT_GARR_M     , {}},
    { "character_garrison_work_order",    DTT_GARR_WO    , {}},
    { "character_void_storage",           DTT_VS_TABLE   , {}},
    { "item_instance",                    DTT_ITEM       , {}},
    { "mail",                             DTT_MAIL       , {}},
    { "mail_items",                       DTT_MAIL_ITEM  , {}},
    { "pet_aura",                         DTT_PET_TABLE  , {}},
    { "pet_aura_effect",                  DTT_PET_TABLE  , {}},
    { "pet_spell",                        DTT_PET_TABLE  , {}},
    { "pet_spell_cooldown",               DTT_PET_TABLE  , {}},
};

#define DUMP_TABLE_COUNT (sizeof(g_DumpTables) / sizeof(g_DumpTables[0]))

// Low level functions
static bool findtoknth(std::string &str, int n, std::string::size_type &s, std::string::size_type &e)
{
    int i;
    s = e = 0;
    std::string::size_type size = str.size();
    for (i = 1; s < size && i < n; s++)
        if (str[s] == ' ')
            ++i;
    if (i < n)
        return false;

    e = str.find(' ', s);

    return e != std::string::npos;
}

std::string gettoknth(std::string &str, int n)
{
    std::string::size_type s = 0, e = 0;
    if (!findtoknth(str, n, s, e))
        return "";

    return str.substr(s, e - s);
}

bool findnth(std::string &str, int n, std::string::size_type &s, std::string::size_type &e)
{
    s = str.find("VALUES ('") + 9;
    if (s == std::string::npos)
        return false;

    do
    {
        e = str.find("'", s);
        if (e == std::string::npos)
            return false;
    }
    while (str[e - 1] == '\\');

    for (int i = 1; i < n; ++i)
    {
        do
        {
            s = e + 4;
            e = str.find("'", s);
            if (e == std::string::npos)
                return false;
        }
        while (str[e - 1] == '\\');
    }

    return true;
}

std::string gettablename(std::string &str)
{
    std::string::size_type s = 13;
    std::string::size_type e = str.find(_TABLE_SIM_, s);
    if (e == std::string::npos)
        return "";

    return str.substr(s, e - s);
}

bool changenth(std::string &str, int n, const char *with, bool insert = false, bool nonzero = false)
{
    std::string::size_type s, e;
    if (!findnth(str, n, s, e))
        return false;

    if (nonzero && str.substr(s, e - s) == "0")
        return true; // not an error
    if (!insert)
        str.replace(s, e - s, with);
    else
        str.insert(s, with);

    return true;
}

std::string getnth(std::string &str, int n)
{
    std::string::size_type s, e;
    if (!findnth(str, n, s, e))
        return "";

    return str.substr(s, e - s);
}

bool changetoknth(std::string &str, int n, const char *with, bool insert = false, bool nonzero = false)
{
    std::string::size_type s = 0, e = 0;
    if (!findtoknth(str, n, s, e))
        return false;
    if (nonzero && str.substr(s, e - s) == "0")
        return true; // not an error
    if (!insert)
        str.replace(s, e - s, with);
    else
        str.insert(s, with);

    return true;
}

uint32 registerNewGuid(uint32 oldGuid, std::map<uint32, uint32> &guidMap, uint32 hiGuid)
{
    std::map<uint32, uint32>::const_iterator itr = guidMap.find(oldGuid);
    if (itr != guidMap.end())
        return itr->second;

    uint32 newguid = hiGuid;
    guidMap[oldGuid] = newguid;
    return newguid;
}

bool changeGuid(std::string &str, int n, std::map<uint32, uint32> &guidMap, uint32 hiGuid, bool nonzero = false)
{
    char chritem[20];
    uint32 oldGuid = atoll(getnth(str, n).c_str());
    if (nonzero && oldGuid == 0)
        return true; // not an error

    uint32 newGuid = registerNewGuid(oldGuid, guidMap, hiGuid);
    snprintf(chritem, 20, "%u", newGuid);

    return changenth(str, n, chritem, false, nonzero);
}

bool changetokGuid(std::string &str, int n, std::map<uint32, uint32> &guidMap, uint32 hiGuid, bool nonzero = false)
{
    char chritem[20];
    uint32 oldGuid = atoll(gettoknth(str, n).c_str());
    if (nonzero && oldGuid == 0)
        return true; // not an error

    uint32 newGuid = registerNewGuid(oldGuid, guidMap, hiGuid);
    snprintf(chritem, 20, "%u", newGuid);

    return changetoknth(str, n, chritem, false, nonzero);
}

std::string CreateDumpString(uint32 type, char const* tableName, QueryResult result, std::vector<std::string> const& columns, bool ashran)
{
    if (!tableName || !result)
        return "";

    if (ashran && (std::string(tableName) == "account_achievement"
        || std::string(tableName) == "account_achievement_progress"))
        return "";

    std::ostringstream ss;
    ss << "INSERT INTO " << _TABLE_SIM_ << tableName << _TABLE_SIM_;
    ss << " (";

    std::list<uint32> l_SkipIndex;

    for (int32 l_I = 0; l_I < (int32)columns.size(); l_I++)
    {
        if (ashran &&
            ((std::string(tableName) == "characters" && columns[l_I] == "instance_mode_mask")
            || (std::string(tableName) == "item_instance" && columns[l_I] == "reforgeId")))
        {
            l_SkipIndex.push_back(l_I);
            continue;
        }

        ss << columns[l_I];

        if (l_I != (columns.size() - 1))
            ss << ", ";
    }
    ss << ")";

    ss << " VALUES (";

    Field *fields = result->Fetch();
    for (uint32 i = 0; i < result->GetFieldCount(); ++i)
    {
        if (std::find(l_SkipIndex.begin(), l_SkipIndex.end(), i) != l_SkipIndex.end())
            continue;

        if (i == 0)
            ss << "'";
        else
            ss << ", '";

        std::string s = fields[i].GetString();
        CharacterDatabase.EscapeString(s);
        ss << s;

        ss << "'";
    }
    ss << ")";

    if (type == DTT_ACC_ACH)
    {
        std::string first_guid = fields[1].GetString();
        std::string date       = fields[3].GetString();

        ss << " ON DUPLICATE KEY UPDATE first_guid = CASE when `date` > " << date << " THEN " << first_guid << " ELSE first_guid END, `date` = LEAST(`date`, " << date << ")";
    }

    if (type == DTT_ACC_ACH_PRO)
    {
        std::string counter = fields[2].GetString();
        CharacterDatabase.EscapeString(counter);

        ss << " ON DUPLICATE KEY UPDATE counter = GREATEST(counter, " << counter << ")";
    }

    ss << ";";
    return ss.str();
}

std::string PlayerDumpWriter::GenerateWhereStr(char const* field, uint32 guid)
{
    std::ostringstream wherestr;
    wherestr << field << " = '" << guid << "'";
    return wherestr.str();
}

std::string PlayerDumpWriter::GenerateWhereStr(char const* field, GUIDs const& guids, GUIDs::const_iterator& itr)
{
    std::ostringstream wherestr;
    wherestr << field << " IN ('";
    for (; itr != guids.end(); ++itr)
    {
        wherestr << *itr;

        if (wherestr.str().size() > MAX_QUERY_LEN - 50) // near to max query
        {
            ++itr;
            break;
        }

        GUIDs::const_iterator itr2 = itr;
        if (++itr2 != guids.end())
            wherestr << "', '";
    }
    wherestr << "')";
    return wherestr.str();
}

void StoreGUID(QueryResult result, uint32 field, std::set<uint32>& guids)
{
    Field* fields = result->Fetch();
    uint32 guid = fields[field].GetUInt32();
    if (guid)
        guids.insert(guid);
}

void StoreGUID(QueryResult result, uint32 data, uint32 field, std::set<uint32>& guids)
{
    Field* fields = result->Fetch();
    std::string dataStr = fields[data].GetString();
    uint32 guid = atoll(gettoknth(dataStr, field).c_str());
    if (guid)
        guids.insert(guid);
}

uint32 GetFieldIndexFromColumn(std::string p_ColumnName, std::vector<std::string> const& p_Columns)
{
    uint32 l_Index = 0;
    for (auto l_Column : p_Columns)
    {
        if (l_Column == p_ColumnName)
            return l_Index;

        l_Index++;
    }

    return -1;
}

// Writing - High-level functions
bool PlayerDumpWriter::DumpTable(std::string& dump, uint32 guid, uint32 account, char const*tableFrom, char const*tableTo, DumpTableType type, std::vector<std::string> const& columns, bool ashran, uint32& p_GarrisonID)
{
    GUIDs const* guids = NULL;
    char const* fieldname = NULL;

    switch (type)
    {
        case DTT_ITEM:
            fieldname = "guid";
            guids = &items;
            break;
        case DTT_ITEM_GIFT:
            fieldname = "item_guid";
            guids = &items;
            break;
        case DTT_PET:
            fieldname = "owner";
            break;
        case DTT_PET_TABLE:
            fieldname = "guid";
            guids = &pets;
            break;
        case DTT_MAIL:
            fieldname = "receiver";
            break;
        case DTT_MAIL_ITEM:
            fieldname = "mail_id";
            guids = &mails;
            break;
        case DTT_VS_TABLE:
            fieldname = "playerGuid";
            break;
        case DTT_ACC_ACH:
        case DTT_ACC_ACH_PRO:
            fieldname = "account";
            break;
        case DTT_GARR_MAIN:
            fieldname = "character_guid";
            break;
        case DTT_GARR_B:
        case DTT_GARR_F:
        case DTT_GARR_M:
        case DTT_GARR_WO:
            fieldname = "garrison_id";
            break;
        default:
            fieldname = "guid";
            break;
    }

    if (type == DTT_ACC_SPELL)
        return true;

    // for guid set stop if set is empty
    if (guids && guids->empty())
        return true; // nothing to do

    // setup for guids case start position
    GUIDs::const_iterator guids_itr;
    if (guids)
        guids_itr = guids->begin();

    do
    {
        std::string wherestr;

        if (type == DTT_ACC_ACH_PRO || type == DTT_ACC_ACH) // account table (i know, it's 'account' but it's linked to realm :D)
            wherestr = GenerateWhereStr(fieldname, account);
        else if (guids) // set case, get next guids string
            wherestr = GenerateWhereStr(fieldname, *guids, guids_itr);
        else if (fieldname == "garrison_id")
            wherestr = GenerateWhereStr(fieldname, p_GarrisonID);
        else
            // not set case, get single guid string
            wherestr = GenerateWhereStr(fieldname, guid);

        QueryResult result = CharacterDatabase.PQuery("SELECT * FROM %s WHERE %s", tableFrom, wherestr.c_str());
        if (!result)
            return true;

        do
        {
            // collect guids
            switch (type)
            {
                case DTT_INVENTORY:
                {
                    uint32 l_Idx = GetFieldIndexFromColumn("item", columns);
                    if (l_Idx == -1)
                        return false;
                    StoreGUID(result, l_Idx, items);
                    break; // item guid collection (character_inventory.item)
                }
                case DTT_PET:
                {
                    uint32 l_Idx = GetFieldIndexFromColumn("id", columns);
                    if (l_Idx == -1)
                        return false;
                    StoreGUID(result, l_Idx, pets);
                    break; // pet petnumber collection (character_pet.id)
                }
                case DTT_MAIL:
                {
                    uint32 l_Idx = GetFieldIndexFromColumn("id", columns);
                    if (l_Idx == -1)
                        return false;
                    StoreGUID(result, l_Idx, mails); // mail id collection (mail.id)
                    break;
                }
                case DTT_MAIL_ITEM:
                {
                    uint32 l_Idx = GetFieldIndexFromColumn("item_guid", columns);
                    if (l_Idx == -1)
                        return false;
                    StoreGUID(result, l_Idx, items);
                    break; // item guid collection (mail_items.item_guid)
                }
                case DTT_CHARACTER:
                {
                    uint32 l_Idx = GetFieldIndexFromColumn("deleteInfos_Account", columns);
                    if (l_Idx == -1)
                        return false;

                    if (result->Fetch()[l_Idx].GetUInt32()) // characters.deleteInfos_Account - if filled error
                        return false;
                    break;
                }
                case DTT_GARR_MAIN:
                {
                    uint32 l_Idx = GetFieldIndexFromColumn("id", columns);
                    if (l_Idx == -1)
                        return false;

                    p_GarrisonID = result->Fetch()[l_Idx].GetUInt32();

                }
                default:
                    break;
            }

            dump += CreateDumpString(type, tableTo, result, columns, ashran);
            dump += "\n";
        }
        while (result->NextRow());
    }
    while (guids && guids_itr != guids->end()); // not set case iterate single time, set case iterate for all guids

    return true;
}

bool PlayerDumpWriter::GetDump(uint32 guid, uint32 account, std::string &dump, bool ashran)
{
    dump = "";

    dump +=
            "IMPORTANT NOTE: THIS DUMPFILE IS MADE FOR USE WITH THE 'PDUMP' COMMAND ONLY - EITHER THROUGH INGAME CHAT OR ON CONSOLE!\n";
    dump +=
            "IMPORTANT NOTE: DO NOT apply it directly - it will irreversibly DAMAGE and CORRUPT your database! You have been warned!\n\n";

    uint32 l_GarrisonID = 0xFFFFFFFF;
    for (int i = 0; i < DUMP_TABLE_COUNT; ++i)
    {
        if (!DumpTable(dump, guid, account, g_DumpTables[i].name, g_DumpTables[i].name, g_DumpTables[i].type, g_DumpTables[i].columns, ashran, l_GarrisonID))
            return false;
    }

    // TODO: Add instance/group..
    // TODO: Add a dump level option to skip some non-important tables

    return true;
}

DumpReturn PlayerDumpWriter::WriteDump(const std::string& file, uint32 guid, uint32 account, bool ashran)
{
    FILE* fout = fopen(file.c_str(), "w");
    if (!fout)
        return DUMP_FILE_OPEN_ERROR;

    DumpReturn ret = DUMP_SUCCESS;
    std::string dump;
    if (!GetDump(guid, account, dump, ashran))
        ret = DUMP_CHARACTER_DELETED;

    fprintf(fout, "%s\n", dump.c_str());
    fclose(fout);

    return ret;
}

// Reading - High-level functions
#define ROLLBACK(DR) { fclose(fin); return (DR); }

void fixNULLfields(std::string &line)
{
    std::string nullString("'NULL'");
    size_t pos = line.find(nullString);
    while (pos != std::string::npos)
    {
        line.replace(pos, nullString.length(), "NULL");
        pos = line.find(nullString);
    }
}

std::vector<std::string> GetColumnsFromLine(std::string p_Line)
{
    std::vector<std::string> l_Columns;
    auto l_StartPosition = p_Line.find("(");
    auto l_EndPosition = p_Line.find(")");

    if (l_StartPosition == std::string::npos)
        return{};

    std::string::size_type l_Position = l_StartPosition + 1;

    while (l_Position < l_EndPosition)
    {
        auto l_NewPosition = std::min(p_Line.find(", ", l_Position), l_EndPosition);
        std::string l_Column = p_Line.substr(l_Position, l_NewPosition - l_Position);
        l_Columns.push_back(l_Column);
        l_Position = l_NewPosition + 2;
    }

    return l_Columns;
}

DumpReturn PlayerDumpReader::LoadDump(const std::string& p_File, uint32 p_Account, std::string p_Name, uint32 p_Guid, bool p_OnlyBoundedItems, uint32 p_AtLogin)
{
    uint32 charcount = AccountMgr::GetCharactersCount(p_Account);
    if (charcount >= 11)
        return DUMP_TOO_MANY_CHARS;

    FILE* fin = fopen(p_File.c_str(), "r");
    if (!fin)
        return DUMP_FILE_OPEN_ERROR;

    QueryResult result = QueryResult(NULL);
    char newguid[20], chraccount[20], newpetid[20], currpetid[20], lastpetid[20], atLogin[20], l_NewGarrisonID[20];

    // make sure the same guid doesn't already exist and is safe to use
    bool incHighest = true;
    if (p_Guid != 0 && p_Guid < sObjectMgr->_hiCharGuid)
    {
        result = CharacterDatabase.PQuery("SELECT 1 FROM characters WHERE guid = '%u'", p_Guid);
        if (result)
            p_Guid = sObjectMgr->_hiCharGuid; // use first free if exists
        else
            incHighest = false;
    }
    else
        p_Guid = sObjectMgr->_hiCharGuid;

    // name encoded or empty

    snprintf(newguid, 20, "%u", p_Guid);
    snprintf(chraccount, 20, "%u", p_Account);
    snprintf(newpetid, 20, "%u", sObjectMgr->GeneratePetNumber());
    snprintf(lastpetid, 20, "%d", 0);
    snprintf(atLogin, 20, "%u", p_AtLogin);
    snprintf(l_NewGarrisonID, 20, "%u", sObjectMgr->GetNewGarrisonID());

    std::map<uint32, uint32> items;
    std::map<uint32, uint32> mails;
    char buf[32000] = "";

    typedef std::map<uint32, uint32> PetIds;// old->new petid relation
    typedef PetIds::value_type PetIdsPair;
    PetIds petids;

    SQLTransaction l_CharTransaction = CharacterDatabase.BeginTransaction();
    SQLTransaction l_AccTransaction  = LoginDatabase.BeginTransaction();
    std::stringstream stringstr;

    while (!feof(fin))
    {
        if (!fgets(buf, 32000, fin))
        {
            if (feof(fin)) 
                break;

            sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [1]");
            ROLLBACK(DUMP_FILE_BROKEN);
        }

        std::string l_Line;
        l_Line.assign(buf);

        // skip empty strings
        size_t nw_pos = l_Line.find_first_not_of(" \t\n\r\7");
        if (nw_pos == std::string::npos)
            continue;

        // skip logfile-side dump start notice, the important notes and dump end notices
        if ((l_Line.substr(nw_pos, 16) == "== START DUMP ==") ||
            (l_Line.substr(nw_pos, 15) == "IMPORTANT NOTE:") ||
            (l_Line.substr(nw_pos, 14) == "== END DUMP =="))
            continue;

        // determine table name and load type
        std::string l_TableName = gettablename(l_Line);
        if (l_TableName.empty())
        {
            sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [2]");
            ROLLBACK(DUMP_FILE_BROKEN);
        }

        DumpTableType l_TableType = DumpTableType(0);
        uint8 l_I;
        for (l_I = 0; l_I < DUMP_TABLE_COUNT; ++l_I)
        {
            if (l_TableName == g_DumpTables[l_I].name)
            {
                l_TableType = g_DumpTables[l_I].type;
                break;
            }
        }

        /// Extracts columns from the pdump
        std::vector<std::string> l_Columns = GetColumnsFromLine(l_Line);

        if (l_I == DUMP_TABLE_COUNT)
        {
            sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [3]");
            ROLLBACK(DUMP_FILE_BROKEN);
        }

        if (l_TableType == DTT_MAIL || l_TableType == DTT_MAIL_ITEM)
            continue;

        if (l_TableName == "character_aura"
            || l_TableName == "character_aura_effect")
            continue;

        bool l_AllowedAppend = true;

        // change the data to server values
        switch (l_TableType)
        {
            case DTT_CHARACTER:
            {
                uint32 l_Index = GetFieldIndexFromColumn("guid", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, newguid))                               ///< characters.guid update
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [4]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                l_Index = GetFieldIndexFromColumn("account", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, chraccount))                            ///< characters.account update
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [5]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                /// Avoid MySQL error (Data too long for column 'name' at row 1)
                if (p_Name.size() > 12)
                    p_Name = p_Name.substr(0, 11);

                l_Index = GetFieldIndexFromColumn("name", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, p_Name.c_str())) // characters.name
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [6]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                l_Index = GetFieldIndexFromColumn("at_login", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, atLogin))                           ///< characters.at_login set to "rename on login"
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [7]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                const char null[5] = "NULL";
                l_Index = GetFieldIndexFromColumn("deleteInfos_Account", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, null))                                  ///< characters.deleteInfos_Account
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [9]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                l_Index = GetFieldIndexFromColumn("deleteInfos_Name", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, null))                                  ///< characters.deleteInfos_Name
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [10]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                l_Index = GetFieldIndexFromColumn("deleteDate", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, null))                                  ///< characters.deleteDate
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [11]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                l_Index = GetFieldIndexFromColumn("petslotused", l_Columns) + 1;        ///< characters.petslotused
                if (getnth(l_Line, l_Index).c_str() == "")
                {
                    if (!changenth(l_Line, l_Index, "0"))
                    {
                        sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [characters.petslotused]");
                        ROLLBACK(DUMP_FILE_BROKEN);
                    }
                }

                break;
            }
            case DTT_CHAR_TABLE:
            {
                uint32 l_Index = GetFieldIndexFromColumn("guid", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, newguid))                               ///< character_*.guid update
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [12]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }
                break;
            }
            case DTT_GARR_MAIN:
            {
                uint32 l_Index = GetFieldIndexFromColumn("id", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, l_NewGarrisonID))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [DTT_GARR_MAIN]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                l_Index = GetFieldIndexFromColumn("character_guid", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, newguid))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [DTT_GARR_MAIN]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                break;
            }
            case DTT_GARR_B:
            {
                uint32 l_Index = GetFieldIndexFromColumn("id", l_Columns) + 1;
                auto l_StrValue = std::to_string(sObjectMgr->GetNewGarrisonBuildingID());
                if (!changenth(l_Line, l_Index, l_StrValue.c_str()))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [DTT_GARR_B]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                l_Index = GetFieldIndexFromColumn("garrison_id", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, l_NewGarrisonID))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [DTT_GARR_B]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }
                break;
            }
            case DTT_GARR_F:
            {
                uint32 l_Index = GetFieldIndexFromColumn("id", l_Columns) + 1;
                auto l_StrValue = std::to_string(sObjectMgr->GetNewGarrisonFollowerID());
                if (!changenth(l_Line, l_Index, l_StrValue.c_str()))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [DTT_GARR_F]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                l_Index = GetFieldIndexFromColumn("garrison_id", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, l_NewGarrisonID))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [DTT_GARR_F]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }
                break;
            }
            case DTT_GARR_M:
            {
                uint32 l_Index = GetFieldIndexFromColumn("id", l_Columns) + 1;
                auto l_StrValue = std::to_string(sObjectMgr->GetNewGarrisonMissionID());
                if (!changenth(l_Line, l_Index, l_StrValue.c_str()))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [DTT_GARR_M]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                l_Index = GetFieldIndexFromColumn("garrison_id", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, l_NewGarrisonID))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [DTT_GARR_M]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }
                break;
            }
            case DTT_GARR_WO:
            {
                uint32 l_Index = GetFieldIndexFromColumn("id", l_Columns) + 1;
                auto l_StrValue = std::to_string(sObjectMgr->GetNewGarrisonWorkOrderID());
                if (!changenth(l_Line, l_Index, l_StrValue.c_str()))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [DTT_GARR_WO]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                l_Index = GetFieldIndexFromColumn("garrison_id", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, l_NewGarrisonID))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [DTT_GARR_WO]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }
                break;
            }
            case DTT_EQSET_TABLE:
            {
                uint32 l_Index = GetFieldIndexFromColumn("guid", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, newguid))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [13]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                char newSetGuid[24];
                snprintf(newSetGuid, 24, UI64FMTD, sObjectMgr->GenerateEquipmentSetGuid());
                l_Index = GetFieldIndexFromColumn("setguid", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, newSetGuid))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [14]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }
                break;
            }
            case DTT_INVENTORY:
            {
                uint32 l_Index = GetFieldIndexFromColumn("guid", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, newguid))                               ///< character_inventory.guid update
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [15]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                l_Index = GetFieldIndexFromColumn("bag", l_Columns) + 1;
                if (!changeGuid(l_Line, l_Index, items, sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM), true))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [16]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                l_Index = GetFieldIndexFromColumn("item", l_Columns) + 1;
                if (!changeGuid(l_Line, l_Index, items, sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM)))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [17]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }
                break;
            }
            case DTT_VS_TABLE:
            {
                uint64 newItemIdNum = sObjectMgr->GenerateVoidStorageItemId();
                char newItemId[20];
                snprintf(newItemId, 20, UI64FMTD, newItemIdNum);

                uint32 l_Index = GetFieldIndexFromColumn("itemId", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, newItemId))                             ///< character_void_storage.itemId update
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [18]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                l_Index = GetFieldIndexFromColumn("playerGuid", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, newguid))                               ///< character_void_storage.playerGuid update
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [19]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }
                break;
            }
            case DTT_MAIL: // mail
            {
                uint32 l_Index = GetFieldIndexFromColumn("id", l_Columns) + 1;
                if (!changeGuid(l_Line, l_Index, mails, sObjectMgr->GenerateMailID()))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [20]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                l_Index = GetFieldIndexFromColumn("receiver", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, newguid))                               ///< mail.receiver update
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [21]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }
                break;
            }
            case DTT_MAIL_ITEM: // mail_items
            {
                uint32 l_Index = GetFieldIndexFromColumn("mail_id", l_Columns) + 1;
                if (!changeGuid(l_Line, l_Index, mails, sObjectMgr->GenerateMailID()))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [22]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }
                l_Index = GetFieldIndexFromColumn("item_guid", l_Columns) + 1;
                if (!changeGuid(l_Line, l_Index, items, sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM)))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [23]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }
                l_Index = GetFieldIndexFromColumn("receiver", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, newguid))                               ///< mail_items.receiver
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [24]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }
                break;
            }
            case DTT_ITEM:
            {
                /// item, owner, data field:item, owner guid
                uint32 l_Index = GetFieldIndexFromColumn("guid", l_Columns) + 1;
                if (!changeGuid(l_Line, l_Index, items, sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM)))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [25]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                l_Index = GetFieldIndexFromColumn("owner_guid", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, newguid))                               ///< item_instance.owner_guid update
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [26]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                l_Index = GetFieldIndexFromColumn("transmogrifyId", l_Columns) + 1;
                std::string oldTransmogrificationId = getnth(l_Line, l_Index).c_str();
                if (oldTransmogrificationId.size() == 0)
                {
                    if (!changenth(l_Line, l_Index, "0"))
                    {
                        sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [28]");
                        ROLLBACK(DUMP_FILE_BROKEN);
                    }
                }

                if (p_OnlyBoundedItems)
                {
                    std::string::size_type s, e;
                    l_Index = GetFieldIndexFromColumn("flags", l_Columns) + 1;
                    if (!findnth(l_Line, l_Index, s, e))
                    {
                        sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [29]");
                        ROLLBACK(DUMP_FILE_BROKEN);
                    }

                    l_Index = GetFieldIndexFromColumn("itemEntry", l_Columns) + 1;
                    uint32 l_ItemEntry = atoi(getnth(l_Line, l_Index).c_str());

                    // Delete Sapphire/Jade/Sunstone/Ruby Panther
                    if (l_ItemEntry == 83090 || l_ItemEntry == 83088 || l_ItemEntry == 83089 || l_ItemEntry == 83087)
                        l_AllowedAppend = false;

                    uint32 flags = atoi(l_Line.substr(s, e - s).c_str());

                    if (!(flags & 1))
                    {
                        /// Don't delete sac even if they aren't bounded, cause items can be inside
                        ItemTemplate const* l_Item = sObjectMgr->GetItemTemplate(l_ItemEntry);
                        if (l_Item && l_Item->InventoryType != INVTYPE_BAG)
                            l_AllowedAppend = false;
                    }
                }

                break;
            }
            case DTT_ITEM_GIFT:
            {
                uint32 l_Index = GetFieldIndexFromColumn("guid", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, newguid))                               ///< character_gifts.guid update
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [30]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }
                l_Index = GetFieldIndexFromColumn("item_guid", l_Columns) + 1;
                if (!changeGuid(l_Line, l_Index, items, sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM)))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [31]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }
                break;
            }
            case DTT_PET:
            {
                //store a map of old pet id to new inserted pet id for use by type 5 tables
                snprintf(currpetid, 20, "%s", getnth(l_Line, 1).c_str());

                if (*lastpetid == '\0')
                    snprintf(lastpetid, 20, "%s", currpetid);

                if (strcmp(lastpetid, currpetid) != 0)
                {
                    snprintf(newpetid, 20, "%u", sObjectMgr->GeneratePetNumber());
                    snprintf(lastpetid, 20, "%s", currpetid);
                }

                std::map<uint32, uint32> :: const_iterator petids_iter = petids.find(atoi(currpetid));

                if (petids_iter == petids.end())
                    petids.insert(PetIdsPair(atoi(currpetid), atoi(newpetid)));

                uint32 l_Index = GetFieldIndexFromColumn("id", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, newpetid))                              ///< character_pet.id update
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [32]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                l_Index = GetFieldIndexFromColumn("owner", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, newguid))                               ///< character_pet.owner update
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [33]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                break;
            }
            case DTT_PET_TABLE: // pet_aura, pet_spell, pet_spell_cooldown
            {
                snprintf(currpetid, 20, "%s", getnth(l_Line, 1).c_str());

                // lookup currpetid and match to new inserted pet id
                std::map<uint32, uint32> :: const_iterator petids_iter = petids.find(atoi(currpetid));
                if (petids_iter == petids.end())// couldn't find new inserted id
                {
                    l_AllowedAppend = false;
                    break;
                }

                snprintf(newpetid, 20, "%u", petids_iter->second);

                uint32 l_Index = GetFieldIndexFromColumn("guid", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, newpetid))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [34]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }

                break;
            }
            case DTT_ACC_ACH:
            {
                uint32 l_Index = GetFieldIndexFromColumn("first_guid", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, newguid))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [35]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }
                break;
            }
            case DTT_ACC_SPELL:
            {
                uint32 l_Index = GetFieldIndexFromColumn("accountId", l_Columns) + 1;
                if (!changenth(l_Line, l_Index, chraccount))
                {
                    sLog->outAshran("LoadDump: DUMP_FILE_BROKEN [can't change accountId]");
                    ROLLBACK(DUMP_FILE_BROKEN);
                }
                break;
            }
            case DTT_ACC_ACH_PRO:
                break; // nothing to change, it's account id only
            default:
                //sLog->outError("Unknown dump table type: %u", type);
                {
                    sLog->outSlack("#jarvis", "danger", true, "Transfer to realm [%u] on account [%u] failed. Reason: Unknow table", g_RealmID, p_Account);
                    ROLLBACK(DUMP_FILE_BROKEN);
                }
                break;
        }

        if (l_AllowedAppend)
        {
            fixNULLfields(l_Line);

            if (l_TableType == DTT_ACC_SPELL)
                l_AccTransaction->Append(l_Line.c_str());
            else
                l_CharTransaction->Append(l_Line.c_str());
        }
    }

    fclose(fin);

    if (!CharacterDatabase.DirectCommitTransaction(l_CharTransaction))
    {
        sLog->outSlack("#jarvis", "danger", true, "Transfer to realm [%u] on account [%u] failed. Reason: Character transaction fail", g_RealmID, p_Account);
        return DUMP_FILE_BROKEN;
    }

    LoginDatabase.DirectCommitTransaction(l_AccTransaction);

    if (incHighest)
        ++sObjectMgr->_hiCharGuid;

    return DUMP_SUCCESS;
}

void PlayerDump::LoadColumnsName()
{
    for (int l_I = 0; l_I < DUMP_TABLE_COUNT; ++l_I)
    {
        std::string l_TableName = g_DumpTables[l_I].name;

        if (g_DumpTables[l_I].type == DTT_ACC_SPELL)
        {
            MySQLConnectionInfo l_Info = MySQLConnectionInfo(ConfigMgr::GetStringDefault("LoginDatabaseInfo", ""));
            std::string l_DatabaseName = l_Info.database;

            QueryResult l_TableInfoResult = LoginDatabase.PQuery("SELECT `COLUMN_NAME`  FROM `INFORMATION_SCHEMA`.`COLUMNS` WHERE `TABLE_SCHEMA`='%s' AND `TABLE_NAME`='%s'", l_DatabaseName.c_str(), l_TableName.c_str());
            if (l_TableInfoResult)
            {
                do
                {
                    Field* l_Fields = l_TableInfoResult->Fetch();
                    std::string l_Column = l_Fields[0].GetString();

                    g_DumpTables[l_I].columns.push_back(l_Column);
                } while (l_TableInfoResult->NextRow());
            }
            continue;
        }

        MySQLConnectionInfo l_Info = MySQLConnectionInfo(ConfigMgr::GetStringDefault("CharacterDatabaseInfo", ""));
        std::string l_DatabaseName = l_Info.database;

        QueryResult l_TableInfoResult = CharacterDatabase.PQuery("SELECT `COLUMN_NAME`  FROM `INFORMATION_SCHEMA`.`COLUMNS` WHERE `TABLE_SCHEMA`='%s' AND `TABLE_NAME`='%s'", l_DatabaseName.c_str(), l_TableName.c_str());
        if (l_TableInfoResult)
        {
            do
            {
                Field* l_Fields = l_TableInfoResult->Fetch();
                std::string l_Column = l_Fields[0].GetString();

                g_DumpTables[l_I].columns.push_back(l_Column);
            } while (l_TableInfoResult->NextRow());
        }
    }
}
#endif