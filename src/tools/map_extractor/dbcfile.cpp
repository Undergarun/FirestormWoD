#define _CRT_SECURE_NO_DEPRECATE

#include "dbcfile.h"
#include <inttypes.h>

DBCFile::DBCFile(std::string p_FileName) :
    m_FileName(p_FileName), _data(NULL), _stringTable(NULL)
{
}

bool DBCFile::open()
{
    uint32_t header;

    FILE* f = fopen(m_FileName.c_str(), "rb");

    if (!f)
        return false;

    if (fread(&header, 4, 1, f) != 1)                        // Number of records
    {
        fclose(f);
        return false;
    }

    if (header != 0x43424457)                                //'WDBC'
    {
        fclose(f);
        return false;
    }

    if (fread(&_recordCount, 4, 1, f) != 1)                   // Number of records
    {
        fclose(f);
        return false;
    }

    if (fread(&_fieldCount, 4, 1, f) != 1)                    // Number of fields
    {
        fclose(f);
        return false;
    }

    if (fread(&_recordSize, 4, 1, f) != 1)                    // Size of a record
    {
        fclose(f);
        return false;
    }

    if (fread(&_stringSize, 4, 1, f) != 1)                    // String size
    {
        fclose(f);
        return false;
    }

    if (_fieldCount * 4 != _recordSize)
        return false;

    _data = new unsigned char[_recordSize * _recordCount + _stringSize];
    _stringTable = _data + _recordSize*_recordCount;

    if (fread(_data, _recordSize * _recordCount + _stringSize, 1, f) != 1)
    {
        fclose(f);
        return false;
    }

    fclose(f);

    return true;
}

DBCFile::~DBCFile()
{
    delete [] _data;
}

DBCFile::Record DBCFile::GetRecord(size_t id)
{
    assert(_data);
    return Record(*this, _data + id*_recordSize);
}

size_t DBCFile::GetMaxId()
{
    assert(_data);

    size_t maxId = 0;
    for(size_t i = 0; i < GetRecordCount(); ++i)
        if (maxId < GetRecord(i).GetUInt(0))
            maxId = GetRecord(i).GetUInt(0);

    return maxId;
}

DBCFile::Iterator DBCFile::begin()
{
    assert(_data);
    return Iterator(*this, _data);
}

DBCFile::Iterator DBCFile::end()
{
    assert(_data);
    return Iterator(*this, _stringTable);
}

