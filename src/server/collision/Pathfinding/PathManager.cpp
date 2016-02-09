#include "PathManager.h"
#include "Log.h"
#include "World.h"
#include "PathFactory.h"

namespace MMAP
{
    // ######################## PathManager ########################
    PathManager::~PathManager()
    {
        for (MapDataSet::iterator i = loadedMaps.begin(); i != loadedMaps.end(); ++i)
            delete i->second;

        // by now we should not have maps loaded
        // if we had, tiles in MapData->mapLoadedTiles, their actual data is lost!
    }

    bool PathManager::loadMapData(uint32 mapId)
    {
        // we already have this map loaded?
        if (loadedMaps.find(mapId) != loadedMaps.end())
            return true;

        // load and init dtNavMesh - read parameters from file
        uint32 pathLen = sWorld->GetDataPath().length() + strlen("mmaps/%03i.map")+1;
        char *fileName = new char[pathLen];
        snprintf(fileName, pathLen, (sWorld->GetDataPath()+"mmaps/%03i.map").c_str(), mapId);

        FILE* file = fopen(fileName, "rb");
        if (!file)
        {
            sLog->outDebug(LOG_FILTER_PATHFINDING, "MAP::loadMapData: Error: Could not open map file '%s'", fileName);
            delete [] fileName;
            return false;
        }

        dtNavMeshParams params;
        fread(&params, sizeof(dtNavMeshParams), 1, file);
        fclose(file);

        dtNavMesh* mesh = dtAllocNavMesh();
        ASSERT(mesh);
        if (DT_SUCCESS != mesh->init(&params))
        {
            dtFreeNavMesh(mesh);
            sLog->outError("MAP:loadMapData: Failed to initialize dtNavMesh for map %03u from file %s", mapId, fileName);
            delete [] fileName;
            return false;
        }

        delete [] fileName;

        sLog->outDetail("MAP:loadMapData: Loaded %03i.map", mapId);

        // store inside our map list
        MapData* map_data = new MapData(mesh);
        map_data->mapLoadedTiles.clear();

        loadedMaps.insert(std::pair<uint32, MapData*>(mapId, map_data));
        return true;
    }

    uint32 PathManager::packTileID(int32 x, int32 y)
    {
        return uint32(x << 16 | y);
    }

    bool PathManager::loadMap(uint32 mapId, int32 x, int32 y)
    {
        // make sure the mmap is loaded and ready to load tiles
        if(!loadMapData(mapId))
            return false;

        // get this mmap data
        MapData* map = loadedMaps[mapId];
        ASSERT(map->navMesh);

        // check if we already have this tile loaded
        uint32 packedGridPos = packTileID(x, y);
        if (map->mapLoadedTiles.find(packedGridPos) != map->mapLoadedTiles.end())
        {
            sLog->outError("MAP:loadMap: Asked to load already loaded navmesh tile. %03u%02i%02i.terrain", mapId, x, y);
            return false;
        }

        // load this tile :: mmaps/MMMXXYY.terrain
        uint32 pathLen = sWorld->GetDataPath().length() + strlen("mmaps/%03i%02i%02i.terrain")+1;
        char *fileName = new char[pathLen];
        snprintf(fileName, pathLen, (sWorld->GetDataPath()+"mmaps/%03i%02i%02i.terrain").c_str(), mapId, x, y);

        FILE *file = fopen(fileName, "rb");
        if (!file)
        {
            sLog->outDebug(LOG_FILTER_PATHFINDING, "MAP:loadMap: Could not open terrain file '%s'", fileName);
            delete [] fileName;
            return false;
        }
        delete [] fileName;

        // read header
        mapTileHeader fileHeader;
        fread(&fileHeader, sizeof(mapTileHeader), 1, file);

        if (fileHeader.mapMagic != MMAP_MAGIC)
        {
            sLog->outError("MAP: Bad header in map %03u%02i%02i.terrain", mapId, x, y);
            return false;
        }

        if (fileHeader.mapVersion != MMAP_VERSION)
        {
            sLog->outError("MAP: %03u%02i%02i.mmtile was built with generator v%i, expected v%i",
                                                mapId, x, y, fileHeader.mapVersion, MMAP_VERSION);
            return false;
        }

        unsigned char* data = (unsigned char*)dtAlloc(fileHeader.size, DT_ALLOC_PERM);
        ASSERT(data);

        size_t result = fread(data, fileHeader.size, 1, file);
        if (!result)
        {
            sLog->outError("MAP: Bad header or data in map %03u%02i%02i.terrain", mapId, x, y);
            fclose(file);
            return false;
        }

        fclose(file);

        dtMeshHeader* header = (dtMeshHeader*)data;
        dtTileRef tileRef = 0;

        // memory allocated for data is now managed by detour, and will be deallocated when the tile is removed
        if (DT_SUCCESS == map->navMesh->addTile(data, fileHeader.size, DT_TILE_FREE_DATA, 0, &tileRef))
        {
            map->mapLoadedTiles.insert(std::pair<uint32, dtTileRef>(packedGridPos, tileRef));
            ++loadedTiles;
            sLog->outDetail("MAP: Loaded terrain %03i[%02i,%02i] into %03i[%02i,%02i]", mapId, x, y, mapId, header->x, header->y);
            return true;
        }
        else
        {
            sLog->outError("MAP: Could not load %03u%02i%02i.terrain into navmesh", mapId, x, y);
            dtFree(data);
            return false;
        }
        return false;
    }

    bool PathManager::unloadMap(uint32 mapId, int32 x, int32 y)
    {
        // check if we have this map loaded
        if (loadedMaps.find(mapId) == loadedMaps.end())
        {
            // file may not exist, therefore not loaded
            sLog->outDebug(LOG_FILTER_PATHFINDING, "MAP: Asked to unload not loaded navmesh map. %03u%02i%02i.terrain", mapId, x, y);
            return false;
        }

        MapData* map = loadedMaps[mapId];

        // check if we have this tile loaded
        uint32 packedGridPos = packTileID(x, y);
        if (map->mapLoadedTiles.find(packedGridPos) == map->mapLoadedTiles.end())
        {
            // file may not exist, therefore not loaded
            sLog->outDebug(LOG_FILTER_PATHFINDING, "MAP: Asked to unload not loaded navmesh tile. %03u%02i%02i.terrain", mapId, x, y);
            return false;
        }

        dtTileRef tileRef = map->mapLoadedTiles[packedGridPos];

        // unload, and mark as non loaded
        if (DT_SUCCESS != map->navMesh->removeTile(tileRef, NULL, NULL))
        {
            // this is technically a memory leak
            // if the grid is later reloaded, dtNavMesh::addTile will return error but no extra memory is used
            // we cannot recover from this error - assert out
            sLog->outError("MAP: Could not unload %03u%02i%02i.terrain from navmesh", mapId, x, y);
            ASSERT(false);
        }
        else
        {
            map->mapLoadedTiles.erase(packedGridPos);
            --loadedTiles;
            sLog->outDetail("MAP: Unloaded terrain %03i[%02i,%02i] from %03i", mapId, x, y, mapId);
            return true;
        }

        return false;
    }

    bool PathManager::unloadMap(uint32 mapId)
    {
        if (loadedMaps.find(mapId) == loadedMaps.end())
        {
            // file may not exist, therefore not loaded
            sLog->outDebug(LOG_FILTER_PATHFINDING, "MAP: Asked to unload not loaded navmesh map %03u", mapId);
            return false;
        }

        // unload all tiles from given map
        MapData* map = loadedMaps[mapId];
        for (MapTileSet::iterator i = map->mapLoadedTiles.begin(); i != map->mapLoadedTiles.end(); ++i)
        {
            uint32 x = (i->first >> 16);
            uint32 y = (i->first & 0x0000FFFF);
            if(DT_SUCCESS != map->navMesh->removeTile(i->second, NULL, NULL))
                sLog->outError("MAP: Could not unload %03u%02i%02i.terrain from navmesh", mapId, x, y);
            else
            {
                --loadedTiles;
                sLog->outDetail("MAP: Unloaded terrain %03i[%02i,%02i] from %03i", mapId, x, y, mapId);
            }
        }

        delete map;
        loadedMaps.erase(mapId);
        sLog->outDetail("MAP:unloadMap: Unloaded %03i.map", mapId);

        return true;
    }

    bool PathManager::unloadMapInstance(uint32 mapId, uint32 instanceId)
    {
        // check if we have this map loaded
        if (loadedMaps.find(mapId) == loadedMaps.end())
        {
            // file may not exist, therefore not loaded
            sLog->outDebug(LOG_FILTER_PATHFINDING, "MAP:unloadMapInstance: Asked to unload not loaded navmesh map %03u", mapId);
            return false;
        }

        MapData* map = loadedMaps[mapId];
        if (map->navMeshQueries.find(instanceId) == map->navMeshQueries.end())
        {
            sLog->outDebug(LOG_FILTER_PATHFINDING, "MAP:unloadMapInstance: Asked to unload not loaded dtNavMeshQuery mapId %03u instanceId %u", mapId, instanceId);
            return false;
        }

        dtNavMeshQuery* query = map->navMeshQueries[instanceId];

        dtFreeNavMeshQuery(query);
        map->navMeshQueries.erase(instanceId);
        sLog->outDetail("MAP:unloadMapInstance: Unloaded mapId %03u instanceId %u", mapId, instanceId);

        return true;
    }

    dtNavMesh const* PathManager::GetNavMesh(uint32 mapId)
    {
        if (loadedMaps.find(mapId) == loadedMaps.end())
            return NULL;

        return loadedMaps[mapId]->navMesh;
    }

    dtNavMeshQuery const* PathManager::GetNavMeshQuery(uint32 mapId, uint32 instanceId)
    {
        if (loadedMaps.find(mapId) == loadedMaps.end())
            return NULL;
        MapData* map = loadedMaps[mapId];
        if (map->navMeshQueries.find(instanceId) == map->navMeshQueries.end())
        {
             // allocate mesh query
            dtNavMeshQuery* query = dtAllocNavMeshQuery();
            ASSERT(query);
            if(DT_SUCCESS != query->init(map->navMesh, 1024))
            {
                dtFreeNavMeshQuery(query);
                sLog->outError("MAP:GetNavMeshQuery: Failed to initialize dtNavMeshQuery for mapId %03u instanceId %u", mapId, instanceId);
                return NULL;
            }

            sLog->outDetail("MMAP:GetNavMeshQuery: created dtNavMeshQuery for mapId %03u instanceId %u", mapId, instanceId);
            map->navMeshQueries.insert(std::pair<uint32, dtNavMeshQuery*>(instanceId, query));
        }

        return map->navMeshQueries[instanceId];
    }
}