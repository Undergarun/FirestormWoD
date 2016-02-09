#ifndef _PATHMANAGER_H
#define _PATHMANAGER_H

#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"

//  move map related classes
namespace MMAP
{
    typedef UNORDERED_MAP<uint32, dtTileRef> MapTileSet;
    typedef UNORDERED_MAP<uint32, dtNavMeshQuery*> NavMeshQuerySet;

    // dummy struct to hold maps data
    struct MapData
    {
        MapData(dtNavMesh* mesh) : navMesh(mesh) {}
        ~MapData()
        {
            for (NavMeshQuerySet::iterator i = navMeshQueries.begin(); i != navMeshQueries.end(); ++i)
                dtFreeNavMeshQuery(i->second);

            if (navMesh)
                dtFreeNavMesh(navMesh);
        }

        dtNavMesh* navMesh;

        // we have to use single dtNavMeshQuery for every instance, since those are not thread safe
        NavMeshQuerySet navMeshQueries;     // instanceId to query
        MapTileSet mapLoadedTiles;        // maps [map grid coords] to [dtTile]
    };

    typedef UNORDERED_MAP<uint32, MapData*> MapDataSet;

    // singelton class
    // holds all all access to mmap loading unloading and meshes
    class PathManager
    {
        public:
            PathManager() : loadedTiles(0) {}
            ~PathManager();

            bool loadMap(uint32 mapId, int32 x, int32 y);
            bool unloadMap(uint32 mapId, int32 x, int32 y);
            bool unloadMap(uint32 mapId);
            bool unloadMapInstance(uint32 mapId, uint32 instanceId);

            // the returned [dtNavMeshQuery const*] is NOT threadsafe
            dtNavMeshQuery const* GetNavMeshQuery(uint32 mapId, uint32 instanceId);
            dtNavMesh const* GetNavMesh(uint32 mapId);

            uint32 getLoadedTilesCount() const { return loadedTiles; }
            uint32 getLoadedMapsCount() const { return loadedMaps.size(); }
        private:
            bool loadMapData(uint32 mapId);
            uint32 packTileID(int32 x, int32 y);

            MapDataSet loadedMaps;
            uint32 loadedTiles;
    };
}

#endif