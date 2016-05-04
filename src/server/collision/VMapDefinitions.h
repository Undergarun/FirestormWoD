////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _VMAPDEFINITIONS_H
#define _VMAPDEFINITIONS_H
#include <cstring>
#include <cstdio>

#define LIQUID_TILE_SIZE (533.333f / 128.0f)

namespace VMAP
{
    const char VMAP_MAGIC[] = "VMAP_4.3";
    const char RAW_VMAP_MAGIC[] = "VMAP043";                // used in extracted vmap files with raw data
    const char GAMEOBJECT_MODELS[] = "GameObjectModels.dtree";

    // defined in TileAssembler.cpp currently...
    bool readChunk(FILE* rf, char *dest, const char *compare, uint32 len);
}

// Set of helper macros for extractors (VMAP and MMAP)
#ifndef NO_CORE_FUNCS
#define VMAP_ERROR_LOG(FILTER, ...) TC_LOG_ERROR(FILTER, __VA_ARGS__)
#define VMAP_DEBUG_LOG(FILTER, ...) TC_LOG_DEBUG(FILTER, __VA_ARGS__)
#define VMAP_INFO_LOG(FILTER, ...) TC_LOG_INFO(FILTER, __VA_ARGS__)
#else
#define VMAP_ERROR_LOG(FILTER, ...) (void)sizeof(FILTER)
#define VMAP_DEBUG_LOG(FILTER, ...) (void)sizeof(FILTER)
#define VMAP_INFO_LOG(FILTER, ...)  (void)sizeof(FILTER)
#endif

#endif
