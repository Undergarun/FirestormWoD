////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _FIELD_H
#define _FIELD_H

#include "Common.h"
#include "Log.h"

#include <mysql.h>

/// |   MySQL type           |  method to use                         |
/// |------------------------|----------------------------------------|
/// | TINYINT                | GetBool, GetInt8, GetUInt8             |
/// | SMALLINT               | GetInt16, GetUInt16                    |
/// | MEDIUMINT, INT         | GetInt32, GetUInt32                    |
/// | BIGINT                 | GetInt64, GetUInt64                    |
/// | FLOAT                  | GetFloat                               |
/// | DOUBLE, DECIMAL        | GetDouble                              |
/// | CHAR, VARCHAR,         | GetCString, GetString                  |
/// | TINYTEXT, MEDIUMTEXT,  | GetCString, GetString                  |
/// | TEXT, LONGTEXT         | GetCString, GetString                  |
/// | TINYBLOB, MEDIUMBLOB,  | GetBinary, GetString                   |
/// | BLOB, LONGBLOB         | GetBinary, GetString                   |
/// | BINARY, VARBINARY      | GetBinary                              |

class Field
{
    friend class ResultSet;
    friend class PreparedResultSet;

    public:
        Field();
        ~Field();

        bool GetBool() const // Wrapper, actually gets integer
        {
            return GetUInt8() == 1 ? true : false;
        }

        uint8 GetUInt8() const
        {
            if (!data.value)
                return 0;

            #ifdef TRINITY_DEBUG
            if (!IsType(MYSQL_TYPE_TINY))
            {
                ACE_Stack_Trace l_Trace;
                printf("%s\n", l_Trace.c_str());
                sLog->outFatal(LOG_FILTER_SQL, "FATAL: GetUInt8() on non-tinyint field %s.%s (%s.%s) at index %u. Using type: %s.",
                              meta.TableAlias, meta.Alias, meta.TableName, meta.Name, meta.Index, meta.Type);
                return 0;
            }
            #endif

            if (data.raw)
                return *reinterpret_cast<uint8*>(data.value);
            return static_cast<uint8>(atol((char*)data.value));
        }

        int8 GetInt8() const
        {
            if (!data.value)
                return 0;

            #ifdef TRINITY_DEBUG
            if (!IsType(MYSQL_TYPE_TINY))
            {
                sLog->outFatal(LOG_FILTER_SQL, "FATAL: GetInt8() on non-tinyint field %s.%s (%s.%s) at index %u. Using type: %s.",
                              meta.TableAlias, meta.Alias, meta.TableName, meta.Name, meta.Index, meta.Type);
                return 0;
            }
            #endif

            if (data.raw)
                return *reinterpret_cast<int8*>(data.value);
            return static_cast<int8>(atol((char*)data.value));
        }

        uint16 GetUInt16() const
        {
            if (!data.value)
                return 0;

            #ifdef TRINITY_DEBUG
            if (!IsType(MYSQL_TYPE_SHORT) && !IsType(MYSQL_TYPE_YEAR))
            {
                sLog->outFatal(LOG_FILTER_SQL, "FATAL: GetUInt16() on non-smallint field %s.%s (%s.%s) at index %u. Using type: %s.",
                              meta.TableAlias, meta.Alias, meta.TableName, meta.Name, meta.Index, meta.Type);
                return 0;
            }
            #endif

            if (data.raw)
                return *reinterpret_cast<uint16*>(data.value);
            return static_cast<uint16>(atol((char*)data.value));
        }

        int16 GetInt16() const
        {
            if (!data.value)
                return 0;

            #ifdef TRINITY_DEBUG
            if (!IsType(MYSQL_TYPE_SHORT) && !IsType(MYSQL_TYPE_YEAR))
            {
                sLog->outFatal(LOG_FILTER_SQL, "FATAL: GetInt16() on non-smallint field %s.%s (%s.%s) at index %u. Using type: %s.",
                              meta.TableAlias, meta.Alias, meta.TableName, meta.Name, meta.Index, meta.Type);
                return 0;
            }
            #endif

            if (data.raw)
                return *reinterpret_cast<int16*>(data.value);
            return static_cast<int16>(atol((char*)data.value));
        }

        uint32 GetUInt32() const
        {
            if (!data.value)
                return 0;

            #ifdef TRINITY_DEBUG
            if (!IsType(MYSQL_TYPE_INT24) && !IsType(MYSQL_TYPE_LONG))
            {
                sLog->outFatal(LOG_FILTER_SQL, "FATAL: GetUInt32() on non-(medium)int field %s.%s (%s.%s) at index %u. Using type: %s.",
                              meta.TableAlias, meta.Alias, meta.TableName, meta.Name, meta.Index, meta.Type);
                return 0;
            }
            #endif

            if (data.raw)
                return *reinterpret_cast<uint32*>(data.value);
            return static_cast<uint32>(atol((char*)data.value));
        }

        int32 GetInt32() const
        {
            if (!data.value)
                return 0;

            #ifdef TRINITY_DEBUG
            if (!IsType(MYSQL_TYPE_INT24) && !IsType(MYSQL_TYPE_LONG))
            {
                sLog->outFatal(LOG_FILTER_SQL, "FATAL: GetInt32() on non-(medium)int field %s.%s (%s.%s) at index %u. Using type: %s.",
                              meta.TableAlias, meta.Alias, meta.TableName, meta.Name, meta.Index, meta.Type);
                return 0;
            }
            #endif

            if (data.raw)
                return *reinterpret_cast<int32*>(data.value);
            return static_cast<int32>(atol((char*)data.value));
        }

        uint64 GetUInt64() const
        {
            if (!data.value)
                return 0;

            #ifdef TRINITY_DEBUG
            if (!IsType(MYSQL_TYPE_LONGLONG) && !IsType(MYSQL_TYPE_BIT))
            {
                sLog->outFatal(LOG_FILTER_SQL, "FATAL: GetUInt64() on non-bigint field %s.%s (%s.%s) at index %u. Using type: %s.",
                              meta.TableAlias, meta.Alias, meta.TableName, meta.Name, meta.Index, meta.Type);
                return 0;
            }
            #endif

            if (data.raw)
                return *reinterpret_cast<uint64*>(data.value);
            return static_cast<uint64>(atol((char*)data.value));
        }

        int64 GetInt64() const
        {
            if (!data.value)
                return 0;

            #ifdef TRINITY_DEBUG
            if (!IsType(MYSQL_TYPE_LONGLONG) && !IsType(MYSQL_TYPE_BIT))
            {
                sLog->outFatal(LOG_FILTER_SQL, "FATAL: GetInt64() on non-bigint field %s.%s (%s.%s) at index %u. Using type: %s.",
                              meta.TableAlias, meta.Alias, meta.TableName, meta.Name, meta.Index, meta.Type);
                return 0;
            }
            #endif

            if (data.raw)
                return *reinterpret_cast<int64*>(data.value);
            return static_cast<int64>(strtol((char*)data.value, NULL, 10));
        }

        float GetFloat() const
        {
            if (!data.value)
                return 0.0f;

            #ifdef TRINITY_DEBUG
            if (!IsType(MYSQL_TYPE_FLOAT))
            {
                sLog->outFatal(LOG_FILTER_SQL, "FATAL: GetFloat() on non-float field %s.%s (%s.%s) at index %u. Using type: %s.",
                              meta.TableAlias, meta.Alias, meta.TableName, meta.Name, meta.Index, meta.Type);
                return 0.0f;
            }
            #endif

            if (data.raw)
                return *reinterpret_cast<float*>(data.value);
            return static_cast<float>(atof((char*)data.value));
        }

        double GetDouble() const
        {
            if (!data.value)
                return 0.0f;

            #ifdef TRINITY_DEBUG
            if (!IsType(MYSQL_TYPE_DOUBLE))
            {
                sLog->outFatal(LOG_FILTER_SQL, "FATAL: GetDouble() on non-double field %s.%s (%s.%s) at index %u. Using type: %s.",
                              meta.TableAlias, meta.Alias, meta.TableName, meta.Name, meta.Index, meta.Type);
                return 0.0f;
            }
            #endif

            if (data.raw)
                return *reinterpret_cast<double*>(data.value);
            return static_cast<double>(atof((char*)data.value));
        }

        char const* GetCString() const
        {
            if (!data.value)
                return NULL;

            #ifdef TRINITY_DEBUG
            if (IsNumeric())
            {
                sLog->outFatal(LOG_FILTER_SQL, "FATAL: GetCString() on numeric field %s.%s (%s.%s) at index %u. Using type: %s.",
                              meta.TableAlias, meta.Alias, meta.TableName, meta.Name, meta.Index, meta.Type);
                return NULL;
            }
            #endif
            return static_cast<char const*>(data.value);

        }

        std::string GetString() const
        {
            if (!data.value)
                return "";

            char const* string = GetCString();
            if (!string)
                return "";

            return std::string(string, data.length);
        }

        uint32 GetStringLength() const
        {
            return data.length;
        }

        struct Metadata
        {
            char const* TableName;
            char const* TableAlias;
            char const* Name;
            char const* Alias;
            char const* Type;
            uint32 Index;
        };

    protected:
        #if defined(__GNUC__)
        #pragma pack(1)
        #else
        #pragma pack(push, 1)
        #endif
        struct
        {
            uint32 length;          // Length (prepared strings only)
            void* value;            // Actual data in memory
            enum_field_types type;  // Field type
            bool raw;               // Raw bytes? (Prepared statement or ad hoc)
         } data;
        #if defined(__GNUC__)
        #pragma pack()
        #else
        #pragma pack(pop)
        #endif

        void SetByteValue(void* newValue, enum_field_types newType, uint32 length);
        void SetStructuredValue(char* newValue, enum_field_types newType);

        void CleanUp()
        {
            // Field does not own the data if fetched with prepared statement
            if (!data.raw)
                delete[] ((char*)data.value);
            data.value = NULL;
        }

        static size_t SizeForType(MYSQL_FIELD* field)
        {
            switch (field->type)
            {
                case MYSQL_TYPE_NULL:
                    return 0;
                case MYSQL_TYPE_TINY:
                    return 1;
                case MYSQL_TYPE_YEAR:
                case MYSQL_TYPE_SHORT:
                    return 2;
                case MYSQL_TYPE_INT24:
                case MYSQL_TYPE_LONG:
                case MYSQL_TYPE_FLOAT:
                case MYSQL_TYPE_ENUM:
                    return 4;
                case MYSQL_TYPE_DOUBLE:
                case MYSQL_TYPE_LONGLONG:
                case MYSQL_TYPE_BIT:
                    return 8;

                case MYSQL_TYPE_TIMESTAMP:
                case MYSQL_TYPE_DATE:
                case MYSQL_TYPE_TIME:
                case MYSQL_TYPE_DATETIME:
                    return sizeof(MYSQL_TIME);

                case MYSQL_TYPE_TINY_BLOB:
                case MYSQL_TYPE_MEDIUM_BLOB:
                case MYSQL_TYPE_LONG_BLOB:
                case MYSQL_TYPE_BLOB:
                case MYSQL_TYPE_STRING:
                case MYSQL_TYPE_VAR_STRING:
                    return field->max_length + 1;

                case MYSQL_TYPE_DECIMAL:
                case MYSQL_TYPE_NEWDECIMAL:
                    return 64;

                case MYSQL_TYPE_GEOMETRY:
                /*
                Following types are not sent over the wire:
                MYSQL_TYPE_SET:
                */
                default:
                    sLog->outWarn(LOG_FILTER_SQL, "SQL::SizeForType(): invalid field type %u", uint32(field->type));
                    return 0;
            }
        }

        bool IsType(enum_field_types type) const
        {
            return data.type == type;
        }

        bool IsNumeric() const
        {
            return (data.type == MYSQL_TYPE_TINY ||
                    data.type == MYSQL_TYPE_SHORT ||
                    data.type == MYSQL_TYPE_INT24 ||
                    data.type == MYSQL_TYPE_LONG ||
                    data.type == MYSQL_TYPE_FLOAT ||
                    data.type == MYSQL_TYPE_DOUBLE ||
                    data.type == MYSQL_TYPE_LONGLONG );
        }

    private:
        #ifdef TRINITY_DEBUG
        static char const* FieldTypeToString(enum_field_types type)
        {
            switch (type)
            {
                case MYSQL_TYPE_BIT:         return "BIT";
                case MYSQL_TYPE_BLOB:        return "BLOB";
                case MYSQL_TYPE_DATE:        return "DATE";
                case MYSQL_TYPE_DATETIME:    return "DATETIME";
                case MYSQL_TYPE_NEWDECIMAL:  return "NEWDECIMAL";
                case MYSQL_TYPE_DECIMAL:     return "DECIMAL";
                case MYSQL_TYPE_DOUBLE:      return "DOUBLE";
                case MYSQL_TYPE_ENUM:        return "ENUM";
                case MYSQL_TYPE_FLOAT:       return "FLOAT";
                case MYSQL_TYPE_GEOMETRY:    return "GEOMETRY";
                case MYSQL_TYPE_INT24:       return "INT24";
                case MYSQL_TYPE_LONG:        return "LONG";
                case MYSQL_TYPE_LONGLONG:    return "LONGLONG";
                case MYSQL_TYPE_LONG_BLOB:   return "LONG_BLOB";
                case MYSQL_TYPE_MEDIUM_BLOB: return "MEDIUM_BLOB";
                case MYSQL_TYPE_NEWDATE:     return "NEWDATE";
                case MYSQL_TYPE_NULL:        return "NULL";
                case MYSQL_TYPE_SET:         return "SET";
                case MYSQL_TYPE_SHORT:       return "SHORT";
                case MYSQL_TYPE_STRING:      return "STRING";
                case MYSQL_TYPE_TIME:        return "TIME";
                case MYSQL_TYPE_TIMESTAMP:   return "TIMESTAMP";
                case MYSQL_TYPE_TINY:        return "TINY";
                case MYSQL_TYPE_TINY_BLOB:   return "TINY_BLOB";
                case MYSQL_TYPE_VAR_STRING:  return "VAR_STRING";
                case MYSQL_TYPE_YEAR:        return "YEAR";
                default:                     return "-Unknown-";
            }
        }

        void SetMetadata(MYSQL_FIELD* field, uint32 fieldIndex)
        {
            meta.TableName = field->org_table;
            meta.TableAlias = field->table;
            meta.Name = field->org_name;
            meta.Alias = field->name;
            meta.Type = FieldTypeToString(field->type);
            meta.Index = fieldIndex;
        }

        Metadata meta;

        #endif
};

#endif

