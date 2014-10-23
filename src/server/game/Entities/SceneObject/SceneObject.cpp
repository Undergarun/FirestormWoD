/*
* Copyright (C) 2012-2014 JadeCore <http://pandashan.com>
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

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
    sceneObject->SetUInt32Value(SCENEOBJECT_FIELD_RND_SEED_VAL, rand() % 0xFFFFFFFF);
    sceneObject->SetGuidValue(SCENEOBJECT_FIELD_CREATED_BY, spectator->GetGUID());
    sceneObject->SetUInt32Value(SCENEOBJECT_FIELD_SCENE_TYPE, 0);

    return sceneObject;
}
