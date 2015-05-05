/*
 * Copyright (C) 2011 TrintiyCore <http://www.trinitycore.org/>
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

#ifndef DB2STORE_H
#define DB2STORE_H

#include "DB2FileLoader.h"
#include "DB2fmt.h"
#include "Log.h"
#include "Field.h"
#include "DatabaseWorkerPool.h"
#include "Implementation/HotfixDatabase.h"
#include "DatabaseEnv.h"

#include <vector>

struct SqlDb2
{
    const std::string * m_FormatString;
    const std::string * m_IndexName;
    std::string m_SQLTableName;
    int32 m_IndexPosition;
    int32 m_SQLIndexPosition;

    SqlDb2(const std::string * p_FileName, const std::string * p_Format, const std::string * p_IndexName, const char * p_DB2Fmt)
        : m_FormatString(p_Format), m_IndexName(p_IndexName), m_SQLIndexPosition(0)
    {
        /// Convert dbc file name to sql table name
        m_SQLTableName = *p_FileName;
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
            for (uint32 l_X = 0; l_X < m_FormatString->size(); ++l_X)
            {
                // Count only fields present in sql
                if ((*m_FormatString)[l_X] == FT_SQL_PRESENT)
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
            : m_MaxID(0), m_FieldCount(0), m_Format(p_Format), m_TableHash(0)
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
        virtual bool WriteRecord(uint32 p_ID, ByteBuffer & p_Buffer)
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
                        char* locStr = *(char**)l_Raw;
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
            : DB2StorageBase(p_Format), m_IndexTable(NULL), m_DataTable(NULL)
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
        virtual char * LookupEntryRaw(uint32 p_ID) const
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
        bool Load(char const* p_FileName, SqlDb2 * p_SQL)
        {
            DB2FileLoader l_DB2Reader;

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
                    l_SQLQueryStr += " ORDER BY " + *p_SQL->m_IndexName + " DESC";
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

            m_DataTable = (T*)l_DB2Reader.AutoProduceData(m_Format, m_MaxID, (char**&)m_IndexTable);                    ///< Load raw non-string data
            m_StringPoolList.push_back(l_DB2Reader.AutoProduceStringsArrayHolders(m_Format, (char*)m_DataTable));       ///< Create string holders for loaded string fields
            m_StringPoolList.push_back(l_DB2Reader.AutoProduceStrings(m_Format, (char*)m_DataTable));                   ///< Load strings from dbc data

            /// Insert SQL data into arrays
            if (l_SQLQueryResult)
            {
                if (m_IndexTable)
                {
                    do
                    {
                        l_SQLFields = l_SQLQueryResult->Fetch();

                        uint32 l_Entry = l_SQLFields[p_SQL->m_IndexPosition].GetUInt32();
                        T * l_NewEntry = new T();
                        char * l_WritePtr = (char*)l_NewEntry;

                        uint32 l_ColumnNumber       = 0;
                        uint32 l_SQLColumnNumber    = 0;
                        uint32 l_WritePosition      = 0;

                        for (; l_ColumnNumber < p_SQL->m_FormatString->size(); ++l_ColumnNumber)
                        {
                            if ((*p_SQL->m_FormatString)[l_ColumnNumber] == FT_SQL_SUP)
                            {
                                break;
                            }
                            else if ((*p_SQL->m_FormatString)[l_ColumnNumber] == FT_SQL_ABSENT)
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
                                        // Beginning of the pool - empty string
                                        *((char**)(&l_NewEntry[l_WritePosition])) = m_StringPoolList.back();
                                        l_WritePosition += sizeof(char*);
                                        break;
                                }
                            }
                            else if ((*p_SQL->m_FormatString)[l_ColumnNumber] == FT_SQL_PRESENT)
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
                                        m_CustomStrings.push_back(l_SQLFields[l_SQLColumnNumber].GetString());
                                        char * l_NewString = const_cast<char*>(m_CustomStrings.back().c_str());
                                        char ** l_StringInStructPtr = ((char**)(&l_WritePtr[l_WritePosition]));

                                        *l_StringInStructPtr = l_NewString;

                                        l_WritePosition += sizeof(char*);
                                        break;
                                    }
                                    case FT_SORT:
                                        break;
                                    default:
                                        l_IsValidSqlColumn = false;
                                }
                                if (l_IsValidSqlColumn && (l_ColumnNumber != (p_SQL->m_FormatString->size() - 1)))
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

                        AddEntry(l_Entry, l_NewEntry, true);
                    } while (l_SQLQueryResult->NextRow());
                }
            }

            /// Error in DB2 file at loading if NULL
            return m_IndexTable != nullptr;
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

            m_DataTableEx.clear();

            while (!m_StringPoolList.empty())
            {
                delete[] m_StringPoolList.front();
                m_StringPoolList.pop_front();
            }

            m_MaxID = 0;
        }

    private:
        T** m_IndexTable;
        T* m_DataTable;
        DataTableEx m_DataTableEx;
        StringPoolList m_StringPoolList;
        std::list<std::string> m_CustomStrings;

};

#endif