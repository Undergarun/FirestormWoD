////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Vignette.hpp"
#include "ObjectMgr.h"

namespace Vignette
{
    Entity::Entity(VignetteEntry const* p_VignetteEntry, uint32 const p_MapId)
        : m_Map(p_MapId), m_VignetteEntry(p_VignetteEntry)
    {

        ASSERT(p_VignetteEntry != nullptr);

        m_Guid             = 0;
        m_NeedClientUpdate = false;
    }

    Entity::~Entity()
    {
    }

    void Entity::Create(Type p_Type, G3D::Vector3 p_Position, uint64 p_SourceGuid)
    {
        m_Guid       = MAKE_NEW_GUID(sObjectMgr->GenerateNewVignetteGUID(), m_VignetteEntry->Id, HIGHGUID_VIGNETTE);
        m_Type       = p_Type;
        m_Position   = p_Position;
        m_SourceGuid = p_SourceGuid;
    }

    void Entity::UpdatePosition(G3D::Vector3 p_NewPosition)
    {
        if ((int32)m_Position.x == (int32)p_NewPosition.x &&
            (int32)m_Position.y == (int32)p_NewPosition.y)
            return;

        m_Position         = p_NewPosition;
        m_NeedClientUpdate = true;
    }
}
