////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_DEPRECATE

#include "loadlib.h"
#include <cstdio>

u_map_fcc MverMagic = { { 'R','E','V','M' } };

ChunkedFile::ChunkedFile()
{
    data = 0;
    data_size = 0;
}

ChunkedFile::~ChunkedFile()
{
    free();
}

bool ChunkedFile::loadFile(HANDLE mpq, char* filename, bool log)
{
    free();
    HANDLE file;
    if (!CascOpenFile(mpq, filename, CASC_LOCALE_ALL, 0, &file))
    {
        if (log)
            printf("No such file %s\n", filename);
        return false;
    }

    data_size = CascGetFileSize(file, NULL);
    data = new uint8[data_size];
    CascReadFile(file, data, data_size, NULL/*bytesRead*/);
    parseChunks();
    if (prepareLoadedData())
    {
        CascCloseFile(file);
        return true;
    }

    printf("Error loading %s\n", filename);
    CascCloseFile(file);
    free();

    return false;
}

bool ChunkedFile::prepareLoadedData()
{
    FileChunk* chunk = GetChunk("MVER");
    if (!chunk)
        return false;

    // Check version
    file_MVER* version = chunk->As<file_MVER>();
    if (version->fcc != MverMagic.fcc)
        return false;
    if (version->ver != FILE_FORMAT_VERSION)
        return false;
    return true;
}

void ChunkedFile::free()
{
    for (auto chunk : chunks)
        delete chunk.second;

    chunks.clear();

    delete[] data;
    data = 0;
    data_size = 0;
}

u_map_fcc InterestingChunks[] =
{
    { { 'R', 'E', 'V', 'M' } },
    { { 'N', 'I', 'A', 'M' } },
    { { 'O', '2', 'H', 'M' } },
    { { 'K', 'N', 'C', 'M' } },
    { { 'T', 'V', 'C', 'M' } },
    { { 'O', 'M', 'W', 'M' } },
    { { 'Q', 'L', 'C', 'M' } },
    { { 'O', 'B', 'F', 'M' } }
};

bool IsInterestingChunk(u_map_fcc const& fcc)
{
    for (u_map_fcc const& f : InterestingChunks)
        if (f.fcc == fcc.fcc)
            return true;

    return false;
}

void ChunkedFile::parseChunks()
{
    uint8* ptr = GetData();
    // Make sure there's enough data to read u_map_fcc struct and the uint32 size after it
    while (ptr <= GetData() + GetDataSize() - 8)
    {
        u_map_fcc header = *(u_map_fcc*)ptr;
        if (IsInterestingChunk(header))
        {
            uint32 size = *(uint32*)(ptr + 4);
            if (size <= data_size)
            {
                std::swap(header.fcc_txt[0], header.fcc_txt[3]);
                std::swap(header.fcc_txt[1], header.fcc_txt[2]);

                FileChunk* chunk = new FileChunk(ptr, size);
                chunk->parseSubChunks();
                chunks.insert({ std::string(header.fcc_txt, 4), chunk });
            }

            // move to next chunk
            ptr += size + 8;
        }
        else
            ++ptr;
    }
}

FileChunk* ChunkedFile::GetChunk(std::string const& name)
{
    auto range = chunks.equal_range(name);
    if (std::distance(range.first, range.second) == 1)
        return range.first->second;

    return NULL;
}

FileChunk::~FileChunk()
{
    for (auto subchunk : subchunks)
        delete subchunk.second;

    subchunks.clear();
}

void FileChunk::parseSubChunks()
{
    uint8* ptr = data + 8; // skip self
    while (ptr < data + size)
    {
        u_map_fcc header = *(u_map_fcc*)ptr;
        if (IsInterestingChunk(header))
        {
            uint32 subsize = *(uint32*)(ptr + 4);
            if (subsize < size)
            {
                std::swap(header.fcc_txt[0], header.fcc_txt[3]);
                std::swap(header.fcc_txt[1], header.fcc_txt[2]);

                FileChunk* chunk = new FileChunk(ptr, subsize);
                chunk->parseSubChunks();
                subchunks.insert({ std::string(header.fcc_txt, 4), chunk });
            }

            // move to next chunk
            ptr += subsize + 8;
        }
        else
            ++ptr;
    }
}

FileChunk* FileChunk::GetSubChunk(std::string const& name)
{
    auto range = subchunks.equal_range(name);
    if (std::distance(range.first, range.second) == 1)
        return range.first->second;

    return NULL;
}
