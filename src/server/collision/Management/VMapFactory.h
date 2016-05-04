////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _VMAPFACTORY_H
#define _VMAPFACTORY_H

#include "IVMapManager.h"

/**
This is the access point to the VMapManager.
*/

namespace VMAP
{
    //===========================================================

    class VMapFactory
    {
        public:
            static IVMapManager* createOrGetVMapManager();
            static void clear();
    };

}
#endif
