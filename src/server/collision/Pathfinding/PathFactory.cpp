#include "PathFactory.h"
#include "Log.h"
#include "World.h"
#include "Config.h"

namespace MMAP
{
    // ######################## MMapFactory ########################
    // our global singelton copy
    PathManager *g_MMapManager = NULL;

    // stores list of mapids which do not use pathfinding
    std::set<uint32>* g_mmapDisabledIds = NULL;

    PathManager* MMapFactory::createOrGetMMapManager()
    {
        if (g_MMapManager == NULL)
            g_MMapManager = new PathManager();

        return g_MMapManager;
    }

    void MMapFactory::preventPathfindingOnMaps(const char* ignoreMapIds)
    {
        if (!g_mmapDisabledIds)
            g_mmapDisabledIds = new std::set<uint32>();

        uint32 strLenght = strlen(ignoreMapIds)+1;
        char* mapList = new char[strLenght];
        memcpy(mapList, ignoreMapIds, sizeof(char)*strLenght);

        char* idstr = strtok(mapList, ",");
        while (idstr)
        {
            g_mmapDisabledIds->insert(uint32(atoi(idstr)));
            idstr = strtok(NULL, ",");
        }

        delete [] mapList;
    }

    bool MMapFactory::IsPathfindingEnabled(uint32 mapId)
    {
        return sWorld->getBoolConfig(CONFIG_ENABLE_PATHFINDING)
            && g_mmapDisabledIds->find(mapId) == g_mmapDisabledIds->end();
    }

    void MMapFactory::clear()
    {
        if (g_mmapDisabledIds)
        {
            delete g_mmapDisabledIds;
            g_mmapDisabledIds = NULL;
        }

        if (g_MMapManager)
        {
            delete g_MMapManager;
            g_MMapManager = NULL;
        }
    }
};