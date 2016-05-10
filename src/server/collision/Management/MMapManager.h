////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MMAP_MANAGER_H
#define _MMAP_MANAGER_H

#include "Define.h"
#include "DetourAlloc.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"
#include "MapDefines.h"
#include "Common.h"

//  move map related classes
namespace MMAP
{
    typedef std::unordered_map<uint32, dtTileRef> MMapTileSet;
    typedef std::unordered_map<uint32, dtNavMeshQuery*> NavMeshQuerySet;


    typedef std::set<uint32> TerrainSet;

    struct NavMeshHolder
    {
        // Pre-built navMesh
        dtNavMesh* navMesh;

        // List of terrain swap map ids used to build the navMesh
        TerrainSet terrainIds;

        MMapTileSet loadedTileRefs;
    };

    struct PhasedTile
    {
        unsigned char* data;
        MmapTileHeader fileHeader;
        int32 dataSize;
    };

    typedef std::unordered_map<uint32, PhasedTile*> PhaseTileContainer;
    typedef std::unordered_map<uint32, PhaseTileContainer> PhaseTileMap;


    typedef std::unordered_map<uint32, TerrainSet> TerrainSetMap;

    class MMapData
    {
    public:
        MMapData(dtNavMesh* mesh, uint32 mapId);
        ~MMapData();

        dtNavMesh* GetNavMesh(TerrainSet swaps);

        // we have to use single dtNavMeshQuery for every instance, since those are not thread safe
        NavMeshQuerySet navMeshQueries;     // instanceId to query

        dtNavMesh* navMesh;
        MMapTileSet loadedTileRefs;
        TerrainSetMap loadedPhasedTiles;

    private:
        uint32 _mapId;
        PhaseTileContainer _baseTiles;
        std::set<uint32> _activeSwaps;
        void RemoveSwap(PhasedTile* ptile, uint32 swap, uint32 packedXY);
        void AddSwap(PhasedTile* tile, uint32 swap, uint32 packedXY);
    };


    typedef std::unordered_map<uint32, MMapData*> MMapDataSet;

    // singleton class
    // holds all all access to mmap loading unloading and meshes
    class MMapManager
    {
        public:
            MMapManager() : loadedTiles(0), thread_safe_environment(true) {}
            ~MMapManager();

            void InitializeThreadUnsafe(std::unordered_map<uint32, std::vector<uint32>> const& mapData);
            bool loadMap(const std::string& basePath, uint32 mapId, int32 x, int32 y);
            bool unloadMap(uint32 mapId, int32 x, int32 y);
            bool unloadMap(uint32 mapId);
            bool unloadMapInstance(uint32 mapId, uint32 instanceId);

            // the returned [dtNavMeshQuery const*] is NOT threadsafe
            dtNavMeshQuery const* GetNavMeshQuery(uint32 mapId, uint32 instanceId, TerrainSet swaps);
            dtNavMesh const* GetNavMesh(uint32 mapId, TerrainSet swaps);

            uint32 getLoadedTilesCount() const { return loadedTiles; }
            uint32 getLoadedMapsCount() const { return loadedMMaps.size(); }

            typedef std::unordered_map<uint32, std::vector<uint32>> PhaseChildMapContainer;
            void LoadPhaseTiles(PhaseChildMapContainer::const_iterator phasedMapData, int32 x, int32 y);
            void UnloadPhaseTile(PhaseChildMapContainer::const_iterator phasedMapData, int32 x, int32 y);
            PhaseTileContainer const* GetPhaseTileContainer(uint32 mapId) const
            {
                auto itr = _phaseTiles.find(mapId);
                if (itr != _phaseTiles.end())
                    return &itr->second;
                return nullptr;
            }

        private:
            bool loadMapData(uint32 mapId);
            uint32 packTileID(int32 x, int32 y);

            MMapDataSet::const_iterator GetMMapData(uint32 mapId) const;
            MMapDataSet loadedMMaps;
            PhaseChildMapContainer phaseMapData;
            uint32 loadedTiles;
            bool thread_safe_environment;

            PhasedTile* LoadTile(uint32 mapId, int32 x, int32 y);
            PhaseTileMap _phaseTiles;
    };
}

#endif
