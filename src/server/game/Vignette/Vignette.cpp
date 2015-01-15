/*
* Copyright (C) 2012-2015 Ashran <http://www.ashran.com>
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

#include "Vignette.hpp"
#include "ObjectMgr.h"

namespace Vignette
{
    Entity::Entity(VignetteEntry const* p_VignetteEntry, uint32 const p_MapId) 
        : m_Map(p_MapId), m_VignetteEntry(p_VignetteEntry)
    {
        m_Guid             = 0;
        m_NeedClientUpdate = false;
    }

    Entity::~Entity()
    {
    }

    void Entity::Create(Type p_Type, G3D::Vector3 p_Position, uint64 p_SourceGuid)
    {
        m_Guid       = sObjectMgr->GenerateNewVignetteGUID();
        m_Type       = p_Type;
        m_Position   = p_Position;
        m_SourceGuid = p_SourceGuid;
    }

    void Entity::UpdatePosition(G3D::Vector3 p_NewPosition)
    {
        m_Position         = p_NewPosition;
        m_NeedClientUpdate = true;
    }
}