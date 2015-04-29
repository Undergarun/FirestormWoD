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
#include "Implementation/WorldDatabase.h"
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

template<class T>
class DB2Storage
{
    typedef std::list<char*> StringPoolList;
    typedef std::vector<T*> DataTableEx;
public:
    explicit DB2Storage(const char *f) : nCount(0), fieldCount(0), fmt(f), indexTable(NULL), m_dataTable(NULL) { }
    ~DB2Storage() { Clear(); }

    T const* LookupEntry(uint32 id) const { return (id>=nCount)?NULL:indexTable[id]; }
    uint32  GetNumRows() const { return nCount; }
    char const* GetFormat() const { return fmt; }
    uint32 GetFieldCount() const { return fieldCount; }

    /// Copies the provided entry and stores it.
    void AddEntry(uint32 id, const T* entry, bool p_DontCopy = false)
    {
        if (LookupEntry(id))
            return;

        if (id >= nCount)
        {
            // reallocate index table
            char** tmpIdxTable = new char*[id+1];
            memset(tmpIdxTable, 0, (id+1) * sizeof(char*));
            memcpy(tmpIdxTable, (char*)indexTable, nCount * sizeof(char*));
            delete[] ((char*)indexTable);
            nCount = id + 1;
            indexTable = (T**)tmpIdxTable;
        }

        if (!p_DontCopy)
        {
            T* entryDst = new T;
            memcpy((char*)entryDst, (char*)entry, sizeof(T));
            m_dataTableEx.push_back(entryDst);
            indexTable[id] = entryDst;
        }
        else
        {
            m_dataTableEx.push_back(const_cast<T*>(entry));
            indexTable[id] = const_cast<T*>(entry);
        }
    }

    bool Load(char const* fn, SqlDb2 * sql)
    {
        DB2FileLoader db2;
        // Check if load was sucessful, only then continue
        if (!db2.Load(fn, fmt))
            return false;

        uint32 sqlRecordCount = 0;
        uint32 sqlHighestIndex = 0;
        Field* fields = NULL;
        QueryResult result = QueryResult(NULL);
        // Load data from sql
        if (sql)
        {
            std::string query = "SELECT * FROM " + sql->m_SQLTableName;
            if (sql->m_IndexPosition >= 0)
                query += " ORDER BY " + *sql->m_IndexName + " DESC";
            query += ';';


            result = WorldDatabase.Query(query.c_str());
            if (result)
            {
                sqlRecordCount = uint32(result->GetRowCount());
                if (sql->m_IndexPosition >= 0)
                {
                    fields = result->Fetch();
                    sqlHighestIndex = fields[sql->m_SQLIndexPosition].GetUInt32();
                }
                // Check if sql index pos is valid
                if (int32(result->GetFieldCount() - 1) < sql->m_SQLIndexPosition)
                {
                    sLog->outError(LOG_FILTER_GENERAL, "Invalid index pos for db2:'%s'", sql->m_SQLTableName.c_str());
                    return false;
                }
            }
        }

        fieldCount = db2.GetCols();

        // load raw non-string data
        m_dataTable = (T*)db2.AutoProduceData(fmt, nCount, (char**&)indexTable);

        // create string holders for loaded string fields
        m_stringPoolList.push_back(db2.AutoProduceStringsArrayHolders(fmt, (char*)m_dataTable));

        // load strings from dbc data
        m_stringPoolList.push_back(db2.AutoProduceStrings(fmt, (char*)m_dataTable));

        // Insert sql data into arrays
        if (result)
        {
            if (indexTable)
            {
                uint32 offset = 0;
                uint32 rowIndex = db2.GetNumRows();
                do
                {
                    if (!fields)
                        fields = result->Fetch();

                    uint32 id = fields[sql->m_IndexPosition].GetUInt32();
                    T * l_NewEntry = new T();
                    char * l_WritePtr = (char*)l_NewEntry;

                    uint32 columnNumber = 0;
                    uint32 sqlColumnNumber = 0;

                    for (; columnNumber < sql->m_FormatString->size(); ++columnNumber)
                    {
                        if ((*sql->m_FormatString)[columnNumber] == FT_SQL_SUP)
                        {
                            break;
                        }
                        else if ((*sql->m_FormatString)[columnNumber] == FT_SQL_ABSENT)
                        {
                            switch (fmt[columnNumber])
                            {
                                case FT_FLOAT:
                                    *((float*)(&l_WritePtr[offset])) = 0.0f;
                                    offset += 4;
                                    break;
                                case FT_INDEX:
                                case FT_INT:
                                    *((uint32*)(&l_WritePtr[offset])) = uint32(0);
                                    offset += 4;
                                    break;
                                case FT_BYTE:
                                    *((uint8*)(&l_WritePtr[offset])) = uint8(0);
                                    offset += 1;
                                    break;
                                case FT_STRING:
                                    // Beginning of the pool - empty string
                                    *((char**)(&l_NewEntry[offset])) = m_stringPoolList.back();
                                    offset += sizeof(char*);
                                    break;
                            }
                        }
                        else if ((*sql->m_FormatString)[columnNumber] == FT_SQL_PRESENT)
                        {
                            bool validSqlColumn = true;
                            switch (fmt[columnNumber])
                            {
                                case FT_FLOAT:
                                    *((float*)(&l_WritePtr[offset])) = fields[sqlColumnNumber].GetFloat();
                                    offset += 4;
                                    break;
                                case FT_INDEX:
                                case FT_INT:
                                    *((uint32*)(&l_WritePtr[offset])) = fields[sqlColumnNumber].GetUInt32();
                                    offset += 4;
                                    break;
                                case FT_BYTE:
                                    *((uint8*)(&l_WritePtr[offset])) = fields[sqlColumnNumber].GetUInt8();
                                    offset += 1;
                                    break;
                                case FT_STRING:
                                    sLog->outError(LOG_FILTER_GENERAL, "Unsupported data type in table '%s' at char %d", sql->m_SQLTableName.c_str(), columnNumber);
                                    return false;
                                case FT_SORT:
                                    break;
                                default:
                                    validSqlColumn = false;
                            }
                            if (validSqlColumn && (columnNumber != (sql->m_FormatString->size() - 1)))
                                sqlColumnNumber++;
                        }
                        else
                        {
                            sLog->outError(LOG_FILTER_GENERAL, "Incorrect sql format string '%s' at char %d", sql->m_SQLTableName.c_str(), columnNumber);
                            return false;
                        }
                    }
                    if (sqlColumnNumber != (result->GetFieldCount() - 1))
                    {
                        sLog->outError(LOG_FILTER_GENERAL, "SQL and DB2 format strings are not matching for table: '%s'", sql->m_SQLTableName.c_str());
                        return false;
                    }

                    fields = NULL;
                    ++rowIndex;

                    AddEntry(id, l_NewEntry, true);
                } while (result->NextRow());
            }
        }

        // error in dbc file at loading if NULL
        return indexTable!=NULL;
    }

    bool LoadStringsFrom(char const* fn)
    {
        // DBC must be already loaded using Load
        if (!indexTable)
            return false;

        DB2FileLoader db2;
        // Check if load was successful, only then continue
        if (!db2.Load(fn, fmt))
            return false;

        // load strings from another locale dbc data
        m_stringPoolList.push_back(db2.AutoProduceStrings(fmt, (char*)m_dataTable));

        return true;
    }

    void Clear()
    {
        if (!indexTable)
            return;

        delete[] ((char*)indexTable);
        indexTable = NULL;
        delete[] ((char*)m_dataTable);
        m_dataTable = NULL;
            for (typename DataTableEx::const_iterator itr = m_dataTableEx.begin(); itr != m_dataTableEx.end(); ++itr)
                delete *itr;
            m_dataTableEx.clear();

        while (!m_stringPoolList.empty())
        {
            delete[] m_stringPoolList.front();
            m_stringPoolList.pop_front();
        }
        nCount = 0;
    }

    void EraseEntry(uint32 id) { indexTable[id] = NULL; }

private:
    uint32 nCount;
    uint32 fieldCount;
    uint32 recordSize;
    char const* fmt;
    T** indexTable;
    T* m_dataTable;
    DataTableEx m_dataTableEx;
    StringPoolList m_stringPoolList;
};

#endif