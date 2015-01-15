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

#include "VignetteMgr.hpp"
#include "ObjectAccessor.h"

namespace Vignette
{
    Manager::Manager(Player const* p_Player)
    {
        m_Owner = p_Player;
    }

    Manager::~Manager()
    {
        m_Owner = nullptr;

        for (auto l_Iterator : m_Vignettes)
            delete l_Iterator.second;
    }

    Vignette::Entity* Manager::CreateAndAddVignette(VignetteEntry const* p_VignetteEntry, uint32 const p_MapId, Vignette::Type const p_VignetteType, G3D::Vector3 const p_Position, uint64 const p_SourceGuid)
    {
        Vignette::Entity* l_Vignette = new Vignette::Entity(p_VignetteEntry, p_MapId);
        l_Vignette->Create(p_VignetteType, p_Position, p_SourceGuid);

        m_Vignettes.insert(std::make_pair(l_Vignette->GetGuid(), l_Vignette));
        m_AddedVignette.insert(l_Vignette->GetGuid());

        return l_Vignette;
    }

    void Manager::DestroyAndRemoveVignettes(std::function<bool(Vignette::Entity* const)> p_Lamba)
    {
        for (auto l_Iterator = m_Vignettes.begin(); l_Iterator != m_Vignettes.end();)
        {
            if (p_Lamba(l_Iterator->second))
            {
                delete l_Iterator->second;
                m_RemovedVignette.insert(l_Iterator->first);
                l_Iterator = m_Vignettes.erase(l_Iterator);
                continue;
            }

            ++l_Iterator;
        }
    }

    void Manager::SendVignetteUpdateToClient()
    {
        WorldPacket l_Data(SMSG_VIGNETTE_UPDATE);
        l_Data.WriteBit(false);                                 ///< ForceUpdate

        l_Data << uint32(m_RemovedVignette.size());             ///< RemovedCount
        for (auto l_VignetteGuid : m_RemovedVignette)
            l_Data.appendPackGUID(l_VignetteGuid);

        m_RemovedVignette.clear();

        l_Data << uint32(m_AddedVignette.size());
        for (auto l_VignetteGuid : m_AddedVignette)
            l_Data.appendPackGUID(l_VignetteGuid);

        uint32 l_AddedVignetteCount    = 0;
        size_t l_AddedVignetteCountPos = l_Data.wpos();

        l_Data << uint32(l_AddedVignetteCount);
        for (auto l_VignetteGuid : m_AddedVignette)
        {
            auto l_FindResult = m_Vignettes.find(l_VignetteGuid);
            if (l_FindResult == m_Vignettes.end())
                continue;

            l_AddedVignetteCount++;

            auto l_Vignette = l_FindResult->second;

            l_Data << float(l_Vignette->GetPosition().x);
            l_Data << float(l_Vignette->GetPosition().y);
            l_Data << float(l_Vignette->GetPosition().z);
            l_Data.appendPackGUID(l_Vignette->GetGuid());
            l_Data << uint32(l_Vignette->GetVignetteEntry()->Id);
            l_Data << uint32(0);                                    ///< Zone restricted (Vignette with flag 0x40)
        }

        l_Data.put<uint32>(l_AddedVignetteCountPos, l_AddedVignetteCount);
        m_AddedVignette.clear();

        l_Data << uint32(m_UpdatedVignette.size());
        for (auto l_VignetteGuid : m_UpdatedVignette)
            l_Data.appendPackGUID(l_VignetteGuid);

        uint32 l_UpdatedVignetteCount    = 0;
        size_t l_UpdatedVignetteCountPos = l_Data.wpos();

        l_Data << uint32(l_UpdatedVignetteCount);
        for (auto l_VignetteGuid : m_UpdatedVignette)
        {
            auto l_FindResult = m_Vignettes.find(l_VignetteGuid);
            if (l_FindResult == m_Vignettes.end())
                continue;

            l_UpdatedVignetteCount++;

            auto l_Vignette = l_FindResult->second;

            l_Data << float(l_Vignette->GetPosition().x);
            l_Data << float(l_Vignette->GetPosition().y);
            l_Data << float(l_Vignette->GetPosition().z);
            l_Data.appendPackGUID(l_Vignette->GetGuid());
            l_Data << uint32(l_Vignette->GetVignetteEntry()->Id);
            l_Data << uint32(0);                                    ///< Zone restricted (Vignette with flag 0x40)
        }

        l_Data.put<uint32>(l_UpdatedVignetteCountPos, l_UpdatedVignetteCount);
        m_UpdatedVignette.clear();

        m_Owner->GetSession()->SendPacket(&l_Data);
    }

    void Manager::Update()
    {
        for (auto l_Iterator : m_Vignettes)
        {
            auto l_Vignette = l_Iterator.second;

            /// - Update the position of the vignette if vignette is linked to a creature
            if (IS_UNIT_GUID(l_Vignette->GeSourceGuid()))
            {
                Creature* l_SourceCreature = sObjectAccessor->FindCreature(l_Vignette->GeSourceGuid());
                if (l_SourceCreature != nullptr)
                    l_Vignette->UpdatePosition(G3D::Vector3(l_SourceCreature->GetPositionX(), l_SourceCreature->GetPositionY(), l_SourceCreature->GetPositionZ()));
            }

            if (l_Vignette->NeedClientUpdate())
            {
                m_UpdatedVignette.insert(l_Vignette->GetGuid());
                l_Vignette->ResetNeedClientUpdate();
            }
        }

        /// Send update to client if needed
        if (!m_AddedVignette.empty() || !m_UpdatedVignette.empty() || !m_RemovedVignette.empty())
            SendVignetteUpdateToClient();
    }
}