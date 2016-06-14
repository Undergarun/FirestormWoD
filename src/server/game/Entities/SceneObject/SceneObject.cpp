////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "SceneObject.h"

SceneObject::SceneObject() : WorldObject(false)
{
    m_objectType    |= TYPEMASK_SCENEOBJECT;
    m_objectTypeId  = TYPEID_SCENEOBJECT;
    m_updateFlag    = 0;
    m_valuesCount   = SCENEOBJECT_END;
}

SceneObject::~SceneObject()
{
}

SceneObject* SceneObject::CreateSceneObject(uint32 sceneId, WorldObject* spectator)
{
    if (!spectator || !sceneId)
        return NULL;

    if (!sSceneScriptPackageStore.LookupEntry(sceneId))
        return NULL;

    SceneObject* sceneObject = new SceneObject();

    Position pos;
    pos.m_positionX = spectator->m_positionX;
    pos.m_positionY = spectator->m_positionY;
    pos.m_positionZ = spectator->m_positionZ;

    sceneObject->SetMap(spectator->GetMap());
    sceneObject->Relocate(pos);
    sceneObject->_Create(sceneId, HIGHGUID_SCENEOBJECT, spectator->GetPhaseMask());

    sceneObject->SetObjectScale(1);
    sceneObject->SetEntry(sceneId);
    sceneObject->SetUInt32Value(SCENEOBJECT_FIELD_SCRIPT_PACKAGE_ID, sceneId);
    sceneObject->SetUInt32Value(SCENEOBJECT_FIELD_RND_SEED_VAL, rand() % 0xBABABAB);
    sceneObject->SetGuidValue(SCENEOBJECT_FIELD_CREATED_BY, spectator->GetGUID());
    sceneObject->SetUInt32Value(SCENEOBJECT_FIELD_SCENE_TYPE, 0);

    return sceneObject;
}
