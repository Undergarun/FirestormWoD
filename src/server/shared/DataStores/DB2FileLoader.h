////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DB2_FILE_LOADER_H
#define DB2_FILE_LOADER_H

#include "Define.h"
#include "Utilities/ByteConverter.h"
#include <cassert>

class DB2FileLoader
{
    public:
        DB2FileLoader();
        ~DB2FileLoader();

    bool Load(const char *filename, const char *fmt);

    class Record
    {
    public:
        float getFloat(size_t field) const
        {
            assert(field < file.fieldCount);
            float val = *reinterpret_cast<float*>(offset+file.GetOffset(field));
            EndianConvert(val);
            return val;
        }
        uint32 getUInt(size_t field) const
        {
            assert(field < file.fieldCount);
            uint32 val = *reinterpret_cast<uint32*>(offset+file.GetOffset(field));
            EndianConvert(val);
            return val;
        }
        uint8 getUInt8(size_t field) const
        {
            assert(field < file.fieldCount);
            return *reinterpret_cast<uint8*>(offset+file.GetOffset(field));
        }

        const char *getString(size_t field) const
        {
            assert(field < file.fieldCount);
            size_t stringOffset = getUInt(field);
            assert(stringOffset < file.stringSize);
            return reinterpret_cast<char*>(file.stringTable + stringOffset);
        }

    private:
        Record(DB2FileLoader &file_, unsigned char *offset_): offset(offset_), file(file_) {}
        unsigned char *offset;
        DB2FileLoader &file;

        friend class DB2FileLoader;
    };

    // Get record by id
    Record getRecord(size_t id);
    /// Get begin iterator over records

    uint32 GetNumRows() const { return recordCount;}
    uint32 GetCols() const { return fieldCount; }
    uint32 GetOffset(size_t id) const { return (fieldsOffset != NULL && id < fieldCount) ? fieldsOffset[id] : 0; }
    uint32 GetHash() const { return tableHash; }
    bool IsLoaded() const { return (data != NULL); }
    char* AutoProduceData(const char* fmt, uint32& count, char**& indexTable, std::set<LocalizedString*> & p_LocalizedString);
    char* AutoProduceStringsArrayHolders(const char* fmt, char* dataTable, uint32 p_Locale);
    char* AutoProduceStrings(const char* fmt, char* dataTable, uint32 p_Locale);
    static uint32 GetFormatRecordSize(const char * format, int32 * index_pos = NULL);
    static uint32 GetFormatStringsFields(const char * format);
private:

    uint32 recordSize;
    uint32 recordCount;
    uint32 fieldCount;
    uint32 stringSize;
    uint32 *fieldsOffset;
    unsigned char *data;
    unsigned char *stringTable;

    // WDB2 / WCH2 fields
    uint32 tableHash;    // WDB2
    uint32 build;        // WDB2

    int unk1;            // WDB2 (Unix time in WCH2)
    int unk2;            // WDB2
    int maxIndex;        // WDB2 (index table)
    int locale;          // WDB2
    int unk5;            // WDB2
};

#endif
