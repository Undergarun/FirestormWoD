#ifndef _MMAPFACTORY_H
#define _MMAPFACTORY_H

#include "PathManager.h"

namespace MMAP
{
    // static class
    // holds all map global data
    // access point to PathManager singelton
    class MMapFactory
    {
        public:
            static PathManager* createOrGetMMapManager();
            static void clear();
            static void preventPathfindingOnMaps(const char* ignoreMapIds);
            static bool IsPathfindingEnabled(uint32 mapId);
    };
}

#endif