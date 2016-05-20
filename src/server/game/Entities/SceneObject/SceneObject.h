////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "Object.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Map.h"

class SceneObject : public WorldObject
{
    protected:
        SceneObject();

    public:
        virtual ~SceneObject();

        static SceneObject* CreateSceneObject(uint32 sceneId, WorldObject* spectator);
};

#endif
