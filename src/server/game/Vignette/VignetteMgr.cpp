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
    }

    void Manager::AddVignette(Vignette::Entity* p_Vignette)
    {
        m_Vignettes.insert(std::make_pair(p_Vignette->GetGuid(), p_Vignette));
        m_AddedVignette.insert(p_Vignette->GetGuid());
    }

    void Manager::DestroyAndRemoveVignettes(std::function<bool(Vignette::Entity* const)> p_Lamba)
    {
        std::set<uint64> l_MatchVignette;
        for (auto l_Iterator : m_Vignettes)
        {
            if (p_Lamba(l_Iterator.second))
                l_MatchVignette.insert(l_Iterator.first);
        }

        for (auto l_Iterator : l_MatchVignette)
        {
            auto l_Vignette = m_Vignettes[l_Iterator];
            m_Vignettes.erase(l_Iterator);
            m_RemovedVignette.insert(l_Iterator);
            delete l_Vignette;
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

        uint32 l_AddedVignetteSize    = 0;
        size_t l_AddedVignetteSizePos = l_Data.wpos();

        l_Data << uint32(l_AddedVignetteSize);
        for (auto l_VignetteGuid : m_AddedVignette)
        {
            auto l_FindResult = m_Vignettes.find(l_VignetteGuid);
            if (l_FindResult == m_Vignettes.end())
                continue;

            l_AddedVignetteSize++;

            auto l_Vignette = l_FindResult->second;

            l_Data << float(l_Vignette->GetPosition().x);
            l_Data << float(l_Vignette->GetPosition().y);
            l_Data << float(l_Vignette->GetPosition().z);
            l_Data.appendPackGUID(l_Vignette->GetGuid());
            l_Data << uint32(0);                            ///< Unk
        }

        l_Data.put<uint32>(l_AddedVignetteSizePos, l_AddedVignetteSize);
        m_AddedVignette.clear();

        l_Data << uint32(m_UpdatedVignette.size());
        for (auto l_VignetteGuid : m_UpdatedVignette)
            l_Data.appendPackGUID(l_VignetteGuid);

        uint32 l_UpdatedVignetteSize    = 0;
        size_t l_UpdatedVignetteSizePos = l_Data.wpos();

        l_Data << uint32(l_UpdatedVignetteSize);
        for (auto l_VignetteGuid : m_UpdatedVignette)
        {
            auto l_FindResult = m_Vignettes.find(l_VignetteGuid);
            if (l_FindResult == m_Vignettes.end())
                continue;

            l_AddedVignetteSize++;

            auto l_Vignette = l_FindResult->second;

            l_Data << float(l_Vignette->GetPosition().x);
            l_Data << float(l_Vignette->GetPosition().y);
            l_Data << float(l_Vignette->GetPosition().z);
            l_Data.appendPackGUID(l_Vignette->GetGuid());
            l_Data << uint32(0);                            ///< Unk
        }

        l_Data.put<uint32>(l_UpdatedVignetteSizePos, l_UpdatedVignetteSize);
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


    /*template void Manager::OnWorldObjectAppear(Creature*);
    template void Manager::OnWorldObjectAppear(GameObject*);
    template void Manager::OnWorldObjectDisappear(Creature*);
    template void Manager::OnWorldObjectDisappear(GameObject*);*/
}