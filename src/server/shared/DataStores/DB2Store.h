////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DB2STORE_H
#define DB2STORE_H

#include "DB2FileLoader.h"
#include "DB2fmt.h"
#include "Log.h"
#include "Field.h"
#include "DatabaseWorkerPool.h"
#include "Implementation/HotfixDatabase.h"
#include "DatabaseEnv.h"
#include "Common.h"

struct SqlDb2
{
    std::string m_FormatString;
    std::string m_IndexName;
    std::string m_SQLTableName;
    int32 m_IndexPosition;
    int32 m_SQLIndexPosition;

    SqlDb2(const std::string p_FileName, const std::string p_Format, const std::string p_IndexName, const char * p_DB2Fmt)
        : m_FormatString(p_Format), m_IndexName(p_IndexName), m_SQLIndexPosition(0)
    {
        /// Convert dbc file name to sql table name
        m_SQLTableName = p_FileName;
        for (uint32 l_I = 0; l_I < m_SQLTableName.size(); ++l_I)
        {
            if (isalpha(m_SQLTableName[l_I]))
                m_SQLTableName[l_I] = char(tolower(m_SQLTableName[l_I]));
            else if (m_SQLTableName[l_I] == '.')
                m_SQLTableName[l_I] = '_';
        }

        /// Get sql index position
        DB2FileLoader::GetFormatRecordSize(p_DB2Fmt, &m_IndexPosition);
        if (m_IndexPosition >= 0)
        {
            uint32 uindexPos = uint32(m_IndexPosition);
            for (uint32 l_X = 0; l_X < m_FormatString.size(); ++l_X)
            {
                // Count only fields present in sql
                if (m_FormatString[l_X] == FT_SQL_PRESENT)
                {
                    if (l_X == uindexPos)
                        break;

                    ++m_SQLIndexPosition;
                }
            }
        }
    }
};

class DB2StorageBase
{
    public:
        /// Constructor
        /// @p_Format :  DB2 format
        DB2StorageBase(const char * p_Format)
            : m_TableHash(0), m_MaxID(0), m_FieldCount(0), m_Format(p_Format)
        {

        }

        /// Get DB2 hash
        uint32 GetHash() const
        {
            return m_TableHash;
        }
        /// Get num rows
        uint32 GetNumRows() const
        {
            return m_MaxID;
        }
        /// Get format descriptor
        char const* GetFormat() const
        {
            return m_Format;
        }
        /// Get field count
        uint32 GetFieldCount() const
        {
            return m_FieldCount;
        }

        /// Get raw entry by index
        /// @p_ID : Entry index
        virtual char * LookupEntryRaw(uint32 p_ID) const = 0;

        /// Write record into a ByteBuffer
        /// @p_ID     : Record ID
        /// @p_Buffer : Destination buffer
        virtual bool WriteRecord(uint32 p_ID, ByteBuffer & p_Buffer, uint32 p_Locale)
        {
            char const* l_Raw = LookupEntryRaw(p_ID);

            if (!l_Raw)
                return false;

            std::size_t l_FieldCount = strlen(m_Format);
            for (uint32 l_I = 0; l_I < l_FieldCount; ++l_I)
            {
                switch (m_Format[l_I])
                {
                    case FT_INDEX:
                    case FT_INT:
                        p_Buffer << *(uint32*)l_Raw;
                        l_Raw += 4;
                        break;
                    case FT_FLOAT:
                        p_Buffer << *(float*)l_Raw;
                        l_Raw += 4;
                        break;
                    case FT_BYTE:
                        p_Buffer << *(uint8*)l_Raw;
                        l_Raw += 1;
                        break;
                    case FT_STRING:
                    {
                        LocalizedString * l_LocalizedString = *((LocalizedString**)(l_Raw));

                        char const* locStr = l_LocalizedString->Get(p_Locale);
                        l_Raw += sizeof(char*);

                        std::string l_String(locStr);

                        std::size_t l_Lenght = l_String.length();
                        p_Buffer << uint16(l_Lenght ? l_Lenght + 1 : 0);

                        if (l_Lenght)
                        {
                            p_Buffer.WriteString(l_String);
                            p_Buffer << uint8(0);
                        }
                        break;
                    }
                }
            }

            return true;
        }

        /// Reload the data store
        virtual bool Reload(std::string& p_OutMessage) = 0;

    protected:
        uint32 m_TableHash;
        uint32 m_MaxID;
        uint32 m_FieldCount;
        char const* m_Format;

};

template<class T> class DB2Storage : public DB2StorageBase
{
    using StringPoolList = std::list<char*>;
    using DataTableEx = std::vector<T*>;

    public:
        /// Constructor
        /// @p_Format :  DB2 format
        explicit DB2Storage(char const* p_Format)
            : DB2StorageBase(p_Format), m_IndexTable(NULL), m_DataTable(NULL), m_SQL(nullptr)
        {

        }
        /// Destructor
        ~DB2Storage()
        {
            Clear();
        }

        /// Get entry by index
        /// @p_ID : Entry index
        T const* LookupEntry(uint32 p_ID) const
        {
            return (p_ID >= m_MaxID) ? nullptr: m_IndexTable[p_ID];
        }
        /// Get raw entry by index
        /// @p_ID : Entry index
        virtual char * LookupEntryRaw(uint32 p_ID) const override
        {
            return (char*)LookupEntry(p_ID);
        }

        /// Copies the provided entry and stores it.
        /// @p_ID       : Entry index
        /// @p_Entry    : The entry
        /// @p_DontCopy : Do not copy the entry
        void AddEntry(uint32 p_ID, T const* p_Entry, bool p_DontCopy = false)
        {
            if (LookupEntry(p_ID))
                return;

            if (p_ID >= m_MaxID)
            {
                // reallocate index table
                char** l_TempIndexTable = new char*[p_ID+1];
                memset(l_TempIndexTable, 0, (p_ID+1) * sizeof(char*));
                memcpy(l_TempIndexTable, (char*)m_IndexTable, m_MaxID * sizeof(char*));
                delete[] ((char*)m_IndexTable);

                m_MaxID      = p_ID + 1;
                m_IndexTable = (T**)l_TempIndexTable;
            }

            if (!p_DontCopy)
            {
                T* l_EntryCopy = new T;
                memcpy((char*)l_EntryCopy, (char*)p_Entry, sizeof(T));
                m_DataTableEx.push_back(l_EntryCopy);
                m_IndexTable[p_ID] = l_EntryCopy;
            }
            else
            {
                m_DataTableEx.push_back(const_cast<T*>(p_Entry));
                m_IndexTable[p_ID] = const_cast<T*>(p_Entry);
            }
        }
        /// Erase an entry
        /// @p_ID       : Entry index
        void EraseEntry(uint32 p_ID)
        {
            m_IndexTable[p_ID] = nullptr;
        }

        /// Load the DB2 from a file
        /// @p_FileName : DB2 file path
        /// @p_SQL      : SQL options
        bool Load(char const* p_FileName, SqlDb2 * p_SQL, uint32 p_Locale)
        {
            DB2FileLoader l_DB2Reader;
            m_SQL = p_SQL;
            /// Check if load was successful, only then continue
            if (!l_DB2Reader.Load(p_FileName, m_Format))
                return false;

            uint32 l_SQLlRecordCount = 0;
            uint32 l_SQLHighestIndex = 0;
            Field* l_SQLFields = nullptr;

            QueryResult l_SQLQueryResult = QueryResult(nullptr);

            /// Load data from SQL
            if (p_SQL)
            {
                std::string l_SQLQueryStr = "SELECT * FROM " + p_SQL->m_SQLTableName;

                if (p_SQL->m_IndexPosition >= 0)
                    l_SQLQueryStr += " ORDER BY " + p_SQL->m_IndexName + " DESC";
                l_SQLQueryStr += ';';

                l_SQLQueryResult = HotfixDatabase.Query(l_SQLQueryStr.c_str());
                if (l_SQLQueryResult)
                {
                    l_SQLlRecordCount = uint32(l_SQLQueryResult->GetRowCount());
                    if (p_SQL->m_IndexPosition >= 0)
                    {
                        l_SQLFields = l_SQLQueryResult->Fetch();
                        l_SQLHighestIndex = l_SQLFields[p_SQL->m_SQLIndexPosition].GetUInt32();
                    }
                    /// Check if SQL index pos is valid
                    if (int32(l_SQLQueryResult->GetFieldCount() - 1) < p_SQL->m_SQLIndexPosition)
                    {
                        sLog->outError(LOG_FILTER_GENERAL, "Invalid index pos for db2:'%s'", p_SQL->m_SQLTableName.c_str());
                        return false;
                    }
                }
            }

            m_FieldCount    = l_DB2Reader.GetCols();
            m_TableHash     = l_DB2Reader.GetHash();

            m_DataTable = (T*)l_DB2Reader.AutoProduceData(m_Format, m_MaxID, (char**&)m_IndexTable, m_LocalizedString);         ///< Load raw non-string data
            m_StringPoolList.push_back(l_DB2Reader.AutoProduceStringsArrayHolders(m_Format, (char*)m_DataTable, p_Locale));     ///< Create string holders for loaded string fields
            m_StringPoolList.push_back(l_DB2Reader.AutoProduceStrings(m_Format, (char*)m_DataTable, p_Locale));                 ///< Load strings from dbc data

            /// Insert SQL data into arrays
            if (l_SQLQueryResult)
            {
                if (m_IndexTable)
                {
                    do
                    {
                        l_SQLFields = l_SQLQueryResult->Fetch();

                        uint32 l_Entry = l_SQLFields[p_SQL->m_IndexPosition].GetUInt32();
                        T * l_NewEntry = (T*)LookupEntry(l_Entry);

                        if (!l_NewEntry)
                            l_NewEntry = new T();

                        char * l_WritePtr = (char*)l_NewEntry;

                        uint32 l_ColumnNumber       = 0;
                        uint32 l_SQLColumnNumber    = 0;
                        uint32 l_WritePosition      = 0;

                        for (; l_ColumnNumber < p_SQL->m_FormatString.size(); ++l_ColumnNumber)
                        {
                            if ((p_SQL->m_FormatString)[l_ColumnNumber] == FT_SQL_SUP)
                            {
                                break;
                            }
                            else if ((p_SQL->m_FormatString)[l_ColumnNumber] == FT_SQL_ABSENT)
                            {
                                switch (m_Format[l_ColumnNumber])
                                {
                                    case FT_FLOAT:
                                        *((float*)(&l_WritePtr[l_WritePosition])) = 0.0f;
                                        l_WritePosition += 4;
                                        break;
                                    case FT_INDEX:
                                    case FT_INT:
                                        *((uint32*)(&l_WritePtr[l_WritePosition])) = uint32(0);
                                        l_WritePosition += 4;
                                        break;
                                    case FT_BYTE:
                                        *((uint8*)(&l_WritePtr[l_WritePosition])) = uint8(0);
                                        l_WritePosition += 1;
                                        break;
                                    case FT_STRING:
                                        if (!LookupEntry(l_Entry))
                                        {
                                            *((LocalizedString**)(&l_WritePtr[l_WritePosition])) = new LocalizedString();
                                            m_LocalizedString.emplace(*((LocalizedString**)(&l_WritePtr[l_WritePosition])));
                                        }

                                        LocalizedString * l_LocalizedString = *((LocalizedString**)(&l_WritePtr[l_WritePosition]));

                                        // Beginning of the pool - empty string
                                        l_LocalizedString->Str[LOCALE_enUS] = m_StringPoolList.back();
                                        l_WritePosition += sizeof(LocalizedString*);
                                        break;
                                }
                            }
                            else if ((p_SQL->m_FormatString)[l_ColumnNumber] == FT_SQL_PRESENT)
                            {
                                bool l_IsValidSqlColumn = true;
                                switch (m_Format[l_ColumnNumber])
                                {
                                    case FT_FLOAT:
                                        *((float*)(&l_WritePtr[l_WritePosition])) = l_SQLFields[l_SQLColumnNumber].GetFloat();
                                        l_WritePosition += 4;
                                        break;
                                    case FT_INDEX:
                                    case FT_INT:
                                        *((uint32*)(&l_WritePtr[l_WritePosition])) = l_SQLFields[l_SQLColumnNumber].GetUInt32();
                                        l_WritePosition += 4;
                                        break;
                                    case FT_BYTE:
                                        *((uint8*)(&l_WritePtr[l_WritePosition])) = l_SQLFields[l_SQLColumnNumber].GetUInt8();
                                        l_WritePosition += 1;
                                        break;
                                    case FT_STRING:
                                    {
                                        if (!LookupEntry(l_Entry))
                                        {
                                            *((LocalizedString**)(&l_WritePtr[l_WritePosition])) = new LocalizedString();
                                            m_LocalizedString.emplace(*((LocalizedString**)(&l_WritePtr[l_WritePosition])));
                                        }

                                        m_CustomStrings.push_back(l_SQLFields[l_SQLColumnNumber].GetString());

                                        char * l_NewString = const_cast<char*>(m_CustomStrings.back().c_str());

                                        LocalizedString * l_LocalizedString = *((LocalizedString**)(&l_WritePtr[l_WritePosition]));
                                        l_LocalizedString->Str[LOCALE_enUS] = l_NewString;

                                        l_WritePosition += sizeof(LocalizedString*);
                                        break;
                                    }
                                    case FT_SORT:
                                        break;
                                    default:
                                        l_IsValidSqlColumn = false;
                                }
                                if (l_IsValidSqlColumn && (l_ColumnNumber != (p_SQL->m_FormatString.size() - 1)))
                                    l_SQLColumnNumber++;
                            }
                            else
                            {
                                sLog->outError(LOG_FILTER_GENERAL, "Incorrect SQL format string '%s' at char %d", p_SQL->m_SQLTableName.c_str(), l_ColumnNumber);
                                return false;
                            }
                        }
                        if (l_SQLColumnNumber != (l_SQLQueryResult->GetFieldCount() - 1))
                        {
                            sLog->outError(LOG_FILTER_GENERAL, "SQL and DB2 format strings are not matching for table: '%s'", p_SQL->m_SQLTableName.c_str());
                            return false;
                        }

                        if (!LookupEntry(l_Entry))
                            AddEntry(l_Entry, l_NewEntry, true);
                    } while (l_SQLQueryResult->NextRow());
                }
            }

            uint32 l_StringCount = 0;

            for (uint32 l_I = 0; l_I < strlen(m_Format); ++l_I)
            {
                if (m_Format[l_I] == FT_STRING)
                    l_StringCount++;
            }

            if (m_SQL && l_StringCount)
            {
                std::string l_SQLQueryStr = "SELECT * FROM " + m_SQL->m_SQLTableName + "_locale";

                if (m_SQL->m_IndexPosition >= 0)
                    l_SQLQueryStr += " ORDER BY " + m_SQL->m_IndexName + " DESC";
                l_SQLQueryStr += ';';

                l_SQLQueryResult = HotfixDatabase.Query(l_SQLQueryStr.c_str());
                if (l_SQLQueryResult)
                {
                    /// All locales (but not us), Index & BuildVerified
                    if (l_SQLQueryResult->GetFieldCount() != ((l_StringCount * (MAX_LOCALES - 1)) + 2))
                    {
                        sLog->outError(LOG_FILTER_GENERAL, "SQL format invalid for table : '%s_locale'", p_SQL->m_SQLTableName.c_str());
                        return false;
                    }

                    do
                    {
                        Field* l_SQLFields          = l_SQLQueryResult->Fetch();
                        uint32 l_SQLColumnNumber    = 0;
                        uint32 l_EntryID            = l_SQLFields[l_SQLColumnNumber++].GetUInt32();

                        T * l_Entry = (T*)LookupEntry(l_EntryID);

                        if (!l_Entry)
                            continue;

                        char * l_WritePtr = (char*)l_Entry;
                        uint32 l_WritePosition = 0;

                        for (uint32 l_I = 0; l_I < strlen(m_Format); ++l_I)
                        {
                            switch (m_Format[l_I])
                            {
                                case FT_FLOAT:
                                case FT_INDEX:
                                case FT_INT:
                                    l_WritePosition += 4;
                                    break;
                                case FT_BYTE:
                                    l_WritePosition += 1;
                                    break;
                                case FT_STRING:
                                {
                                    LocalizedString * l_LocalizedString = *((LocalizedString**)(&l_WritePtr[l_WritePosition]));

                                    for (uint32 l_Y = 1; l_Y < MAX_LOCALES; l_Y++)
                                    {
                                        std::string l_Str = l_SQLFields[l_SQLColumnNumber++].GetString();

                                        if (l_Str.empty())
                                            continue;

                                        m_CustomStrings.push_back(l_Str);
                                        char * l_NewString = const_cast<char*>(m_CustomStrings.back().c_str());

                                        l_LocalizedString->Str[l_Y] = l_NewString;
                                    }
                                    l_WritePosition += sizeof(LocalizedString*);
                                    break;
                                }
                                case FT_SORT:
                                    break;
                            }
                        }
                    } while (l_SQLQueryResult->NextRow());
                }
            }

            /// Error in DB2 file at loading if NULL
            return m_IndexTable != nullptr;
        }

        /// Reload the data store
        bool Reload(std::string& p_OutMessage) override
        {
            if (!m_SQL)
            {
                p_OutMessage = "No sql settings found";
                return false;
            }

            std::string l_SQLQueryStr = "SELECT * FROM " + m_SQL->m_SQLTableName;

            if (m_SQL->m_IndexPosition >= 0)
                l_SQLQueryStr += " ORDER BY " + m_SQL->m_IndexName + " DESC";
            l_SQLQueryStr += ';';

            QueryResult l_SQLQueryResult = HotfixDatabase.Query(l_SQLQueryStr.c_str());
            if (l_SQLQueryResult)
            {
                uint32 l_SQLlRecordCount = 0;
                uint32 l_SQLHighestIndex = 0;
                Field* l_SQLFields = nullptr;

                l_SQLlRecordCount = uint32(l_SQLQueryResult->GetRowCount());
                if (m_SQL->m_IndexPosition >= 0)
                {
                    l_SQLFields = l_SQLQueryResult->Fetch();
                    l_SQLHighestIndex = l_SQLFields[m_SQL->m_SQLIndexPosition].GetUInt32();
                }
                /// Check if SQL index pos is valid
                if (int32(l_SQLQueryResult->GetFieldCount() - 1) < m_SQL->m_SQLIndexPosition)
                {
                    p_OutMessage = "Invalid index pos for db2:'" + m_SQL->m_SQLTableName + "'";
                    return false;
                }

                do
                {
                    l_SQLFields = l_SQLQueryResult->Fetch();

                    uint32 l_Entry = l_SQLFields[m_SQL->m_IndexPosition].GetUInt32();
                    T * l_NewEntry = (T*)LookupEntry(l_Entry);

                    if (!l_NewEntry)
                        l_NewEntry = new T();

                    char * l_WritePtr = (char*)l_NewEntry;

                    uint32 l_ColumnNumber = 0;
                    uint32 l_SQLColumnNumber = 0;
                    uint32 l_WritePosition = 0;

                    for (; l_ColumnNumber < m_SQL->m_FormatString.size(); ++l_ColumnNumber)
                    {
                        if ((m_SQL->m_FormatString)[l_ColumnNumber] == FT_SQL_SUP)
                        {
                            break;
                        }
                        else if ((m_SQL->m_FormatString)[l_ColumnNumber] == FT_SQL_ABSENT)
                        {
                            switch (m_Format[l_ColumnNumber])
                            {
                                case FT_FLOAT:
                                    *((float*)(&l_WritePtr[l_WritePosition])) = 0.0f;
                                    l_WritePosition += 4;
                                    break;
                                case FT_INDEX:
                                case FT_INT:
                                    *((uint32*)(&l_WritePtr[l_WritePosition])) = uint32(0);
                                    l_WritePosition += 4;
                                    break;
                                case FT_BYTE:
                                    *((uint8*)(&l_WritePtr[l_WritePosition])) = uint8(0);
                                    l_WritePosition += 1;
                                    break;
                                case FT_STRING:
                                    if (!LookupEntry(l_Entry))
                                    {
                                        *((LocalizedString**)(&l_WritePtr[l_WritePosition])) = new LocalizedString();
                                        m_LocalizedString.emplace(*((LocalizedString**)(&l_WritePtr[l_WritePosition])));
                                    }

                                    LocalizedString * l_LocalizedString = *((LocalizedString**)(&l_WritePtr[l_WritePosition]));

                                    // Beginning of the pool - empty string
                                    l_LocalizedString->Str[LOCALE_enUS] = m_StringPoolList.back();
                                    l_WritePosition += sizeof(LocalizedString*);
                                    break;
                            }
                        }
                        else if ((m_SQL->m_FormatString)[l_ColumnNumber] == FT_SQL_PRESENT)
                        {
                            bool l_IsValidSqlColumn = true;
                            switch (m_Format[l_ColumnNumber])
                            {
                                case FT_FLOAT:
                                    *((float*)(&l_WritePtr[l_WritePosition])) = l_SQLFields[l_SQLColumnNumber].GetFloat();
                                    l_WritePosition += 4;
                                    break;
                                case FT_INDEX:
                                case FT_INT:
                                    *((uint32*)(&l_WritePtr[l_WritePosition])) = l_SQLFields[l_SQLColumnNumber].GetUInt32();
                                    l_WritePosition += 4;
                                    break;
                                case FT_BYTE:
                                    *((uint8*)(&l_WritePtr[l_WritePosition])) = l_SQLFields[l_SQLColumnNumber].GetUInt8();
                                    l_WritePosition += 1;
                                    break;
                                case FT_STRING:
                                {
                                    if (!LookupEntry(l_Entry))
                                    {
                                        *((LocalizedString**)(&l_WritePtr[l_WritePosition])) = new LocalizedString();
                                        m_LocalizedString.emplace(*((LocalizedString**)(&l_WritePtr[l_WritePosition])));
                                    }

                                    m_CustomStrings.push_back(l_SQLFields[l_SQLColumnNumber].GetString());

                                    char * l_NewString = const_cast<char*>(m_CustomStrings.back().c_str());

                                    LocalizedString * l_LocalizedString = *((LocalizedString**)(&l_WritePtr[l_WritePosition]));
                                    l_LocalizedString->Str[LOCALE_enUS] = l_NewString;

                                    l_WritePosition += sizeof(LocalizedString*);
                                    break;
                                }
                                case FT_SORT:
                                    break;
                                default:
                                    l_IsValidSqlColumn = false;
                            }
                            if (l_IsValidSqlColumn && (l_ColumnNumber != (m_SQL->m_FormatString.size() - 1)))
                                l_SQLColumnNumber++;
                        }
                        else
                        {
                            p_OutMessage = "Incorrect SQL format string '" + m_SQL->m_SQLTableName + " at char " + std::to_string(l_ColumnNumber);
                            return false;
                        }
                    }
                    if (l_SQLColumnNumber != (l_SQLQueryResult->GetFieldCount() - 1))
                    {
                        p_OutMessage = "SQL and DB2 format strings are not matching for table: '" + m_SQL->m_SQLTableName + "'";
                        return false;
                    }

                    if (!LookupEntry(l_Entry))
                        AddEntry(l_Entry, l_NewEntry, true);
                } while (l_SQLQueryResult->NextRow());
            }

            uint32 l_StringCount = 0;

            for (uint32 l_I = 0; l_I < strlen(m_Format); ++l_I)
            {
                if (m_Format[l_I] == FT_STRING)
                    l_StringCount++;
            }

            if (l_StringCount)
            {
                std::string l_SQLQueryStr = "SELECT * FROM " + m_SQL->m_SQLTableName + "_locale";

                if (m_SQL->m_IndexPosition >= 0)
                    l_SQLQueryStr += " ORDER BY " + m_SQL->m_IndexName + " DESC";
                l_SQLQueryStr += ';';

                l_SQLQueryResult = HotfixDatabase.Query(l_SQLQueryStr.c_str());
                if (l_SQLQueryResult)
                {
                    /// All locales (but not us), Index & BuildVerified
                    if (l_SQLQueryResult->GetFieldCount() != ((l_StringCount * (MAX_LOCALES - 1)) + 2))
                    {
                        p_OutMessage = "SQL format invalid for table : '" + m_SQL->m_SQLTableName + "_locale'";
                        return false;
                    }

                    do
                    {
                        Field* l_SQLFields          = l_SQLQueryResult->Fetch();
                        uint32 l_SQLColumnNumber    = 0;
                        uint32 l_EntryID            = l_SQLFields[l_SQLColumnNumber++].GetUInt32();

                        T * l_Entry = (T*)LookupEntry(l_EntryID);

                        if (!l_Entry)
                            continue;

                        char * l_WritePtr = (char*)l_Entry;
                        uint32 l_WritePosition = 0;

                        for (uint32 l_I = 0; l_I < strlen(m_Format); ++l_I)
                        {
                            switch (m_Format[l_I])
                            {
                                case FT_FLOAT:
                                case FT_INDEX:
                                case FT_INT:
                                    l_WritePosition += 4;
                                    break;
                                case FT_BYTE:
                                    l_WritePosition += 1;
                                    break;
                                case FT_STRING:
                                {
                                    LocalizedString * l_LocalizedString = *((LocalizedString**)(&l_WritePtr[l_WritePosition]));

                                    for (uint32 l_Y = 1; l_Y < MAX_LOCALES; l_Y++)
                                    {
                                        std::string l_Str = l_SQLFields[l_SQLColumnNumber++].GetString();

                                        if (l_Str.empty())
                                            continue;

                                        m_CustomStrings.push_back(l_Str);
                                        char * l_NewString = const_cast<char*>(m_CustomStrings.back().c_str());

                                        l_LocalizedString->Str[l_Y] = l_NewString;
                                    }
                                    l_WritePosition += sizeof(LocalizedString*);
                                    break;
                                }
                                case FT_SORT:
                                    break;
                            }
                        }
                    } while (l_SQLQueryResult->NextRow());
                }
            }

            return true;
        }

        /// Load string from an other DB2
        /// @p_FileName : DB2 file path
        bool LoadStringsFrom(char const* p_FileName)
        {
            /// DBC must be already loaded using Load
            if (!m_IndexTable)
                return false;

            DB2FileLoader l_DB2Reader;

            /// Check if load was successful, only then continue
            if (!l_DB2Reader.Load(p_FileName, m_Format))
                return false;

            m_DB2FileName = p_FileName;

            /// load strings from another locale dbc data
            m_StringPoolList.push_back(l_DB2Reader.AutoProduceStrings(m_Format, (char*)m_DataTable));

            return true;
        }

        /// Clear the DB2
        void Clear()
        {
            if (!m_IndexTable)
                return;

            delete[]((char*)m_IndexTable);
            delete[]((char*)m_DataTable);

            m_IndexTable = nullptr;
            m_DataTable  = nullptr;

            for (typename DataTableEx::const_iterator l_It = m_DataTableEx.begin(); l_It != m_DataTableEx.end(); ++l_It)
                delete *l_It;

            for (auto l_Elem : m_LocalizedString)
                delete l_Elem;

            m_LocalizedString.clear();
            m_DataTableEx.clear();

            while (!m_StringPoolList.empty())
            {
                delete[] m_StringPoolList.front();
                m_StringPoolList.pop_front();
            }

            m_MaxID = 0;

            if (m_SQL)
                delete m_SQL;
        }

        std::string GetDB2FileName() const
        {
            return m_DB2FileName;
        }

    private:
        T** m_IndexTable;
        T* m_DataTable;
        DataTableEx m_DataTableEx;
        StringPoolList m_StringPoolList;
        std::list<std::string> m_CustomStrings;
        std::set<LocalizedString*> m_LocalizedString;
        SqlDb2 * m_SQL;
        std::string m_DB2FileName;
};

#endif
