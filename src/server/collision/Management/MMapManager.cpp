////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Config.h"
#include "MMapManager.h"
#include "Log.h"
#include "Config.h"
#include "MMapFactory.h"
#include "Errors.h"
#include <stdio.h>

namespace MMAP
{
    static char const* const MAP_FILE_NAME_FORMAT = "%s/mmaps/%04i.mmap";
    static char const* const TILE_FILE_NAME_FORMAT = "%s/mmaps/%04i%02i%02i.mmtile";

    // ######################## MMapManager ########################
    MMapManager::~MMapManager()
    {
        for (MMapDataSet::iterator i = loadedMMaps.begin(); i != loadedMMaps.end(); ++i)
            delete i->second;

        // by now we should not have maps loaded
        // if we had, tiles in MMapData->mmapLoadedTiles, their actual data is lost!
    }

    void MMapManager::InitializeThreadUnsafe(std::unordered_map<uint32, std::vector<uint32>> const& mapData)
    {
        // the caller must pass the list of all mapIds that will be used in the MMapManager lifetime
        for (auto const& p : mapData)
        {
            loadedMMaps.insert(MMapDataSet::value_type(p.first, nullptr));
            if (!p.second.empty())
            {
                phaseMapData[p.first] = p.second;
                for (uint32 phasedMapId : p.second)
                    _phaseTiles.insert(PhaseTileMap::value_type(phasedMapId, PhaseTileContainer()));
            }
        }

        thread_safe_environment = false;
    }

    MMapDataSet::const_iterator MMapManager::GetMMapData(uint32 mapId) const
    {
        // return the iterator if found or end() if not found/NULL
        MMapDataSet::const_iterator itr = loadedMMaps.find(mapId);
        if (itr != loadedMMaps.cend() && !itr->second)
            itr = loadedMMaps.cend();

        return itr;
    }

    bool MMapManager::loadMapData(uint32 mapId)
    {
        // we already have this map loaded?
        MMapDataSet::iterator itr = loadedMMaps.find(mapId);
        if (itr != loadedMMaps.end())
        {
            if (itr->second)
                return true;
        }
        else
        {
            if (thread_safe_environment)
                itr = loadedMMaps.insert(MMapDataSet::value_type(mapId, nullptr)).first;
            else
                WPError(false, "Invalid mapId passed to MMapManager after startup in thread unsafe environment");
        }

        // load and init dtNavMesh - read parameters from file
        char l_Buffer[4096];
        sprintf(l_Buffer, MAP_FILE_NAME_FORMAT, ConfigMgr::GetStringDefault("DataDir", ".").c_str(), mapId);

        std::string fileName = l_Buffer;
        FILE* file = fopen(fileName.c_str(), "rb");
        if (!file)
        {
            sLog->outDebug(LOG_FILTER_GENERAL, "maps", "MMAP:loadMapData: Error: Could not open mmap file '%s'", fileName.c_str());
            return false;
        }

        dtNavMeshParams params;
        uint32 count = uint32(fread(&params, sizeof(dtNavMeshParams), 1, file));
        fclose(file);
        if (count != 1)
        {
            sLog->outDebug(LOG_FILTER_GENERAL, "maps", "MMAP:loadMapData: Error: Could not read params from file '%s'", fileName.c_str());
            return false;
        }

        dtNavMesh* mesh = dtAllocNavMesh();
        ASSERT(mesh);
        if (dtStatusFailed(mesh->init(&params)))
        {
            dtFreeNavMesh(mesh);
            sLog->outError(LOG_FILTER_GENERAL, "maps", "MMAP:loadMapData: Failed to initialize dtNavMesh for mmap %04u from file %s", mapId, fileName.c_str());
            return false;
        }

        sLog->outDebug(LOG_FILTER_GENERAL, "maps", "MMAP:loadMapData: Loaded %04i.mmap", mapId);

        // store inside our map list
        MMapData* mmap_data = new MMapData(mesh, mapId);

        itr->second = mmap_data;
        return true;
    }

    uint32 MMapManager::packTileID(int32 x, int32 y)
    {
        return uint32(x << 16 | y);
    }

    bool MMapManager::loadMap(const std::string& /*basePath*/, uint32 mapId, int32 x, int32 y)
    {
        // make sure the mmap is loaded and ready to load tiles
        if (!loadMapData(mapId))
            return false;

        // get this mmap data
        MMapData* mmap = loadedMMaps[mapId];
        ASSERT(mmap->navMesh);

        // check if we already have this tile loaded
        uint32 packedGridPos = packTileID(x, y);
        if (mmap->loadedTileRefs.find(packedGridPos) != mmap->loadedTileRefs.end())
            return false;

        // load this tile :: mmaps/MMMMXXYY.mmtile
        char l_Buffer[4096];
        sprintf(l_Buffer, TILE_FILE_NAME_FORMAT, ConfigMgr::GetStringDefault("DataDir", ".").c_str(), mapId, x, y);
        std::string fileName = l_Buffer;
        FILE* file = fopen(fileName.c_str(), "rb");
        if (!file)
        {
            sLog->outDebug(LOG_FILTER_GENERAL, "maps", "MMAP:loadMap: Could not open mmtile file '%s'", fileName.c_str());
            return false;
        }

        // read header
        MmapTileHeader fileHeader;
        if (fread(&fileHeader, sizeof(MmapTileHeader), 1, file) != 1 || fileHeader.mmapMagic != MMAP_MAGIC)
        {
            sLog->outError(LOG_FILTER_GENERAL, "maps", "MMAP:loadMap: Bad header in mmap %04u%02i%02i.mmtile", mapId, x, y);
            fclose(file);
            return false;
        }

        if (fileHeader.mmapVersion != MMAP_VERSION)
        {
            sLog->outError(LOG_FILTER_GENERAL, "maps", "MMAP:loadMap: %04u%02i%02i.mmtile was built with generator v%i, expected v%i",
                mapId, x, y, fileHeader.mmapVersion, MMAP_VERSION);
            fclose(file);
            return false;
        }

        unsigned char* data = (unsigned char*)dtAlloc(fileHeader.size, DT_ALLOC_PERM);
        ASSERT(data);

        size_t result = fread(data, fileHeader.size, 1, file);
        if (!result)
        {
            sLog->outError(LOG_FILTER_GENERAL, "maps", "MMAP:loadMap: Bad header or data in mmap %04u%02i%02i.mmtile", mapId, x, y);
            fclose(file);
            return false;
        }

        fclose(file);

        dtMeshHeader* header = (dtMeshHeader*)data;
        dtTileRef tileRef = 0;

        // memory allocated for data is now managed by detour, and will be deallocated when the tile is removed
        if (dtStatusSucceed(mmap->navMesh->addTile(data, fileHeader.size, DT_TILE_FREE_DATA, 0, &tileRef)))
        {
            mmap->loadedTileRefs.insert(std::pair<uint32, dtTileRef>(packedGridPos, tileRef));
            ++loadedTiles;
            sLog->outDebug(LOG_FILTER_GENERAL, "maps", "MMAP:loadMap: Loaded mmtile %04i[%02i, %02i] into %04i[%02i, %02i]", mapId, x, y, mapId, header->x, header->y);

            PhaseChildMapContainer::const_iterator phasedMaps = phaseMapData.find(mapId);
            if (phasedMaps != phaseMapData.end())
                LoadPhaseTiles(phasedMaps, x, y);

            return true;
        }

        sLog->outError(LOG_FILTER_GENERAL, "maps", "MMAP:loadMap: Could not load %04u%02i%02i.mmtile into navmesh", mapId, x, y);
        dtFree(data);
        return false;
    }

    PhasedTile* MMapManager::LoadTile(uint32 mapId, int32 x, int32 y)
    {
        // load this tile :: mmaps/MMMXXYY.mmtile
        char l_Buffer[4096];
        sprintf(l_Buffer, TILE_FILE_NAME_FORMAT, ConfigMgr::GetStringDefault("DataDir", ".").c_str(), mapId, x, y);

        std::string fileName = l_Buffer;
        FILE* file = fopen(fileName.c_str(), "rb");
        if (!file)
        {
            // Not all tiles have phased versions, don't flood this msg
            //sLog->outDebug(LOG_FILTER_GENERAL, "phase", "MMAP:LoadTile: Could not open mmtile file '%s'", fileName);
            return NULL;
        }

        PhasedTile* pTile = new PhasedTile();

        // read header
        if (fread(&pTile->fileHeader, sizeof(MmapTileHeader), 1, file) != 1 || pTile->fileHeader.mmapMagic != MMAP_MAGIC)
        {
            sLog->outError(LOG_FILTER_GENERAL, "phase", "MMAP:LoadTile: Bad header in mmap %04u%02i%02i.mmtile", mapId, x, y);
            fclose(file);
            delete pTile;
            return nullptr;
        }

        if (pTile->fileHeader.mmapVersion != MMAP_VERSION)
        {
            sLog->outError(LOG_FILTER_GENERAL, "phase", "MMAP:LoadTile: %04u%02i%02i.mmtile was built with generator v%i, expected v%i",
                mapId, x, y, pTile->fileHeader.mmapVersion, MMAP_VERSION);
            fclose(file);
            delete pTile;
            return nullptr;
        }

        pTile->data = (unsigned char*)dtAlloc(pTile->fileHeader.size, DT_ALLOC_PERM);
        ASSERT(pTile->data);

        size_t result = fread(pTile->data, pTile->fileHeader.size, 1, file);
        if (!result)
        {
            sLog->outError(LOG_FILTER_GENERAL, "phase", "MMAP:LoadTile: Bad header or data in mmap %04u%02i%02i.mmtile", mapId, x, y);
            fclose(file);
            delete pTile;
            return nullptr;
        }

        fclose(file);

        return pTile;
    }

    void MMapManager::LoadPhaseTiles(PhaseChildMapContainer::const_iterator phasedMapData, int32 x, int32 y)
    {
        sLog->outDebug(LOG_FILTER_GENERAL, "phase", "MMAP:LoadPhaseTiles: Loading phased mmtiles for map %u, x: %i, y: %i", phasedMapData->first, x, y);

        uint32 packedGridPos = packTileID(x, y);

        for (uint32 phaseMapId : phasedMapData->second)
        {
            // only a few tiles have terrain swaps, do not write error for them
            if (PhasedTile* data = LoadTile(phaseMapId, x, y))
            {
                sLog->outDebug(LOG_FILTER_GENERAL, "phase", "MMAP:LoadPhaseTiles: Loaded phased %04u%02i%02i.mmtile for root phase map %u", phaseMapId, x, y, phasedMapData->first);
                _phaseTiles[phaseMapId][packedGridPos] = data;
            }
        }
    }

    void MMapManager::UnloadPhaseTile(PhaseChildMapContainer::const_iterator phasedMapData, int32 x, int32 y)
    {
        sLog->outDebug(LOG_FILTER_GENERAL, "phase", "MMAP:UnloadPhaseTile: Unloading phased mmtile for map %u, x: %i, y: %i", phasedMapData->first, x, y);

        uint32 packedGridPos = packTileID(x, y);

        for (uint32 phaseMapId : phasedMapData->second)
        {
            auto phasedTileItr = _phaseTiles.find(phaseMapId);
            if (phasedTileItr == _phaseTiles.end())
                continue;

            auto dataItr = phasedTileItr->second.find(packedGridPos);
            if (dataItr != phasedTileItr->second.end())
            {
                sLog->outDebug(LOG_FILTER_GENERAL, "phase", "MMAP:UnloadPhaseTile: Unloaded phased %04u%02i%02i.mmtile for root phase map %u", phaseMapId, x, y, phasedMapData->first);
                delete dataItr->second->data;
                delete dataItr->second;
                phasedTileItr->second.erase(dataItr);
            }
        }
    }

    bool MMapManager::unloadMap(uint32 mapId, int32 x, int32 y)
    {
        // check if we have this map loaded
        MMapDataSet::const_iterator itr = GetMMapData(mapId);
        if (itr == loadedMMaps.end())
        {
            // file may not exist, therefore not loaded
            sLog->outDebug(LOG_FILTER_GENERAL, "maps", "MMAP:unloadMap: Asked to unload not loaded navmesh map. %04u%02i%02i.mmtile", mapId, x, y);
            return false;
        }

        MMapData* mmap = itr->second;

        // check if we have this tile loaded
        uint32 packedGridPos = packTileID(x, y);
        if (mmap->loadedTileRefs.find(packedGridPos) == mmap->loadedTileRefs.end())
        {
            // file may not exist, therefore not loaded
            sLog->outDebug(LOG_FILTER_GENERAL, "maps", "MMAP:unloadMap: Asked to unload not loaded navmesh tile. %04u%02i%02i.mmtile", mapId, x, y);
            return false;
        }

        dtTileRef tileRef = mmap->loadedTileRefs[packedGridPos];

        // unload, and mark as non loaded
        if (dtStatusFailed(mmap->navMesh->removeTile(tileRef, NULL, NULL)))
        {
            // this is technically a memory leak
            // if the grid is later reloaded, dtNavMesh::addTile will return error but no extra memory is used
            // we cannot recover from this error - assert out
            sLog->outError(LOG_FILTER_GENERAL, "maps", "MMAP:unloadMap: Could not unload %04u%02i%02i.mmtile from navmesh", mapId, x, y);
            abort();
        }
        else
        {
            mmap->loadedTileRefs.erase(packedGridPos);
            --loadedTiles;
            sLog->outDebug(LOG_FILTER_GENERAL, "maps", "MMAP:unloadMap: Unloaded mmtile %03i[%02i, %02i] from %04i", mapId, x, y, mapId);

            PhaseChildMapContainer::const_iterator phasedMaps = phaseMapData.find(mapId);
            if (phasedMaps != phaseMapData.end())
                UnloadPhaseTile(phasedMaps, x, y);
            return true;
        }

        return false;
    }

    bool MMapManager::unloadMap(uint32 mapId)
    {
        MMapDataSet::iterator itr = loadedMMaps.find(mapId);
        if (itr == loadedMMaps.end() || !itr->second)
        {
            // file may not exist, therefore not loaded
            sLog->outDebug(LOG_FILTER_GENERAL, "maps", "MMAP:unloadMap: Asked to unload not loaded navmesh map %04u", mapId);
            return false;
        }

        // unload all tiles from given map
        MMapData* mmap = itr->second;
        for (MMapTileSet::iterator i = mmap->loadedTileRefs.begin(); i != mmap->loadedTileRefs.end(); ++i)
        {
            uint32 x = (i->first >> 16);
            uint32 y = (i->first & 0x0000FFFF);
            if (dtStatusFailed(mmap->navMesh->removeTile(i->second, NULL, NULL)))
                sLog->outError(LOG_FILTER_GENERAL, "maps", "MMAP:unloadMap: Could not unload %04u%02i%02i.mmtile from navmesh", mapId, x, y);
            else
            {
                PhaseChildMapContainer::const_iterator phasedMaps = phaseMapData.find(mapId);
                if (phasedMaps != phaseMapData.end())
                    UnloadPhaseTile(phasedMaps, x, y);
                --loadedTiles;
                sLog->outDebug(LOG_FILTER_GENERAL, "maps", "MMAP:unloadMap: Unloaded mmtile %04i[%02i, %02i] from %04i", mapId, x, y, mapId);
            }
        }

        delete mmap;
        itr->second = nullptr;
        sLog->outDebug(LOG_FILTER_GENERAL, "maps", "MMAP:unloadMap: Unloaded %04i.mmap", mapId);

        return true;
    }

    bool MMapManager::unloadMapInstance(uint32 mapId, uint32 instanceId)
    {
        // check if we have this map loaded
        MMapDataSet::const_iterator itr = GetMMapData(mapId);
        if (itr == loadedMMaps.end())
        {
            // file may not exist, therefore not loaded
            sLog->outDebug(LOG_FILTER_GENERAL, "maps", "MMAP:unloadMapInstance: Asked to unload not loaded navmesh map %04u", mapId);
            return false;
        }

        MMapData* mmap = itr->second;
        if (mmap->navMeshQueries.find(instanceId) == mmap->navMeshQueries.end())
        {
            sLog->outDebug(LOG_FILTER_GENERAL, "maps", "MMAP:unloadMapInstance: Asked to unload not loaded dtNavMeshQuery mapId %04u instanceId %u", mapId, instanceId);
            return false;
        }

        dtNavMeshQuery* query = mmap->navMeshQueries[instanceId];

        dtFreeNavMeshQuery(query);
        mmap->navMeshQueries.erase(instanceId);
        sLog->outDebug(LOG_FILTER_GENERAL, "maps", "MMAP:unloadMapInstance: Unloaded mapId %04u instanceId %u", mapId, instanceId);

        return true;
    }

    dtNavMesh const* MMapManager::GetNavMesh(uint32 mapId, TerrainSet swaps)
    {
        MMapDataSet::const_iterator itr = GetMMapData(mapId);
        if (itr == loadedMMaps.end())
            return NULL;

        return itr->second->GetNavMesh(swaps);
    }

    dtNavMeshQuery const* MMapManager::GetNavMeshQuery(uint32 mapId, uint32 instanceId, TerrainSet swaps)
    {
        MMapDataSet::const_iterator itr = GetMMapData(mapId);
        if (itr == loadedMMaps.end())
            return NULL;

        MMapData* mmap = itr->second;
        if (mmap->navMeshQueries.find(instanceId) == mmap->navMeshQueries.end())
        {
            // allocate mesh query
            dtNavMeshQuery* query = dtAllocNavMeshQuery();
            ASSERT(query);
            if (dtStatusFailed(query->init(mmap->GetNavMesh(swaps), 1024)))
            {
                dtFreeNavMeshQuery(query);
                sLog->outError(LOG_FILTER_GENERAL, "maps", "MMAP:GetNavMeshQuery: Failed to initialize dtNavMeshQuery for mapId %04u instanceId %u", mapId, instanceId);
                return NULL;
            }

            sLog->outDebug(LOG_FILTER_GENERAL, "maps", "MMAP:GetNavMeshQuery: created dtNavMeshQuery for mapId %04u instanceId %u", mapId, instanceId);
            mmap->navMeshQueries.insert(std::pair<uint32, dtNavMeshQuery*>(instanceId, query));
        }

        return mmap->navMeshQueries[instanceId];
    }

    MMapData::MMapData(dtNavMesh* mesh, uint32 mapId)
    {
        navMesh = mesh;
        _mapId = mapId;
    }

    MMapData::~MMapData()
    {
        for (NavMeshQuerySet::iterator i = navMeshQueries.begin(); i != navMeshQueries.end(); ++i)
            dtFreeNavMeshQuery(i->second);

        dtFreeNavMesh(navMesh);

        for (PhaseTileContainer::iterator i = _baseTiles.begin(); i != _baseTiles.end(); ++i)
        {
            delete (*i).second->data;
            delete (*i).second;
        }
    }

    void MMapData::RemoveSwap(PhasedTile* ptile, uint32 swap, uint32 packedXY)
    {
        uint32 x = (packedXY >> 16);
        uint32 y = (packedXY & 0x0000FFFF);

        if (loadedPhasedTiles[swap].find(packedXY) == loadedPhasedTiles[swap].end())
        {
            sLog->outDebug(LOG_FILTER_GENERAL, "phase", "MMapData::RemoveSwap: mmtile %04u[%02i, %02i] unload skipped, due to not loaded", swap, x, y);
            return;
        }
        dtMeshHeader* header = (dtMeshHeader*)ptile->data;

        // remove old tile
        if (dtStatusFailed(navMesh->removeTile(loadedTileRefs[packedXY], NULL, NULL)))
            sLog->outError(LOG_FILTER_GENERAL, "phase", "MMapData::RemoveSwap: Could not unload phased %04u%02i%02i.mmtile from navmesh", swap, x, y);
        else
        {
            sLog->outDebug(LOG_FILTER_GENERAL, "phase", "MMapData::RemoveSwap: Unloaded phased %04u%02i%02i.mmtile from navmesh", swap, x, y);

            // restore base tile
            if (dtStatusSucceed(navMesh->addTile(_baseTiles[packedXY]->data, _baseTiles[packedXY]->dataSize, 0, 0, &loadedTileRefs[packedXY])))
            {
                sLog->outDebug(LOG_FILTER_GENERAL, "phase", "MMapData::RemoveSwap: Loaded base mmtile %04u[%02i, %02i] into %04i[%02i, %02i]", _mapId, x, y, _mapId, header->x, header->y);
            }
            else
                sLog->outError(LOG_FILTER_GENERAL, "phase", "MMapData::RemoveSwap: Could not load base %04u%02i%02i.mmtile to navmesh", _mapId, x, y);
        }

        loadedPhasedTiles[swap].erase(packedXY);

        if (loadedPhasedTiles[swap].empty())
        {
            _activeSwaps.erase(swap);
            sLog->outDebug(LOG_FILTER_GENERAL, "phase", "MMapData::RemoveSwap: Fully removed swap %u from map %u", swap, _mapId);
        }
    }

    void MMapData::AddSwap(PhasedTile* ptile, uint32 swap, uint32 packedXY)
    {

        uint32 x = (packedXY >> 16);
        uint32 y = (packedXY & 0x0000FFFF);

        if (loadedTileRefs.find(packedXY) == loadedTileRefs.end())
        {
            sLog->outDebug(LOG_FILTER_GENERAL, "phase", "MMapData::AddSwap: phased mmtile %04u[%02i, %02i] load skipped, due to not loaded base tile on map %u", swap, x, y, _mapId);
            return;
        }
        if (loadedPhasedTiles[swap].find(packedXY) != loadedPhasedTiles[swap].end())
        {
            sLog->outDebug(LOG_FILTER_GENERAL, "phase", "MMapData::AddSwap: WARNING! phased mmtile %04u[%02i, %02i] load skipped, due to already loaded on map %u", swap, x, y, _mapId);
            return;
        }


        dtMeshHeader* header = (dtMeshHeader*)ptile->data;

        const dtMeshTile* oldTile = navMesh->getTileByRef(loadedTileRefs[packedXY]);

        if (!oldTile)
        {
            sLog->outDebug(LOG_FILTER_GENERAL, "phase", "MMapData::AddSwap: phased mmtile %04u[%02i, %02i] load skipped, due to not loaded base tile ref on map %u", swap, x, y, _mapId);
            return;
        }

        // header xy is based on the swap map's tile set, wich doesn't have all the same tiles as root map, so copy the xy from the orignal header
        header->x = oldTile->header->x;
        header->y = oldTile->header->y;

        // the removed tile's data
        PhasedTile* pt = new PhasedTile();
        // remove old tile
        if (dtStatusFailed(navMesh->removeTile(loadedTileRefs[packedXY], &pt->data, &pt->dataSize)))
        {
            sLog->outError(LOG_FILTER_GENERAL, "phase", "MMapData::AddSwap: Could not unload %04u%02i%02i.mmtile from navmesh", _mapId, x, y);
            delete pt;
        }
        else
        {
            sLog->outDebug(LOG_FILTER_GENERAL, "phase", "MMapData::AddSwap: Unloaded %04u%02i%02i.mmtile from navmesh", _mapId, x, y);

            // store the removed data first time, this is the origonal, non-phased tile
            if (_baseTiles.find(packedXY) == _baseTiles.end())
                _baseTiles[packedXY] = pt;

            _activeSwaps.insert(swap);
            loadedPhasedTiles[swap].insert(packedXY);

            // add new swapped tile
            if (dtStatusSucceed(navMesh->addTile(ptile->data, ptile->fileHeader.size, 0, 0, &loadedTileRefs[packedXY])))
            {
                sLog->outDebug(LOG_FILTER_GENERAL, "phase", "MMapData::AddSwap: Loaded phased mmtile %04u[%02i, %02i] into %04i[%02i, %02i]", swap, x, y, _mapId, header->x, header->y);
            }
            else
                sLog->outError(LOG_FILTER_GENERAL, "phase", "MMapData::AddSwap: Could not load %04u%02i%02i.mmtile to navmesh", swap, x, y);
        }
    }

    dtNavMesh* MMapData::GetNavMesh(TerrainSet swaps)
    {
        std::set<uint32> activeSwaps = _activeSwaps;    // _activeSwaps is modified inside RemoveSwap
        for (uint32 swap : activeSwaps)
        {
            if (!swaps.count(swap)) // swap not active
            {
                if (PhaseTileContainer const* ptc = MMAP::MMapFactory::createOrGetMMapManager()->GetPhaseTileContainer(swap))
                    for (PhaseTileContainer::const_iterator itr = ptc->begin(); itr != ptc->end(); ++itr)
                        RemoveSwap(itr->second, swap, itr->first); // remove swap
            }
        }

        // for each of the calling unit's terrain swaps
        for (uint32 swap : swaps)
        {
            if (!_activeSwaps.count(swap)) // swap not active
            {
                // for each of the terrain swap's xy tiles
                if (PhaseTileContainer const* ptc = MMAP::MMapFactory::createOrGetMMapManager()->GetPhaseTileContainer(swap))
                    for (PhaseTileContainer::const_iterator itr = ptc->begin(); itr != ptc->end(); ++itr)
                        AddSwap(itr->second, swap, itr->first); // add swap
            }
        }

        return navMesh;
    }
}
