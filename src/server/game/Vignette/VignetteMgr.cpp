////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

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
        /// Check for duplicated vignettes
        for (auto l_Vignette : m_Vignettes)
        {
            /// Return if same vignette has already been created
            if (l_Vignette.second->GetVignetteEntry()->Id == p_VignetteEntry->Id)
                return nullptr;
        }

        Vignette::Entity* l_Vignette = new Vignette::Entity(p_VignetteEntry, p_MapId);
        l_Vignette->Create(p_VignetteType, p_Position, p_SourceGuid);

        m_Vignettes.insert(std::make_pair(l_Vignette->GetGuid(), l_Vignette));
        m_AddedVignette.insert(l_Vignette->GetGuid());

        return l_Vignette;
    }

    void Manager::DestroyAndRemoveVignetteByEntry(VignetteEntry const* p_VignetteEntry)
    {
        if (p_VignetteEntry == nullptr)
            return;

        for (auto l_Iterator = m_Vignettes.begin(); l_Iterator != m_Vignettes.end();)
        {
            if (l_Iterator->second->GetVignetteEntry()->Id == p_VignetteEntry->Id)
            {
                delete l_Iterator->second;
                m_RemovedVignette.insert(l_Iterator->first);
                l_Iterator = m_Vignettes.erase(l_Iterator);
                continue;
            }

            ++l_Iterator;
        }
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
    
    template <class T>
    inline void Manager::OnWorldObjectAppear(T const* p_Target)
    {
        VignetteEntry const* l_VignetteEntry = GetVignetteEntryFromWorldObject(p_Target);
        if (l_VignetteEntry == nullptr)
            return;
        
        auto l_Type = GetDefaultVignetteTypeFromWorldObject(p_Target);
        auto l_TrackingQuest = GetTrackingQuestIdFromWorldObject(p_Target);
        
        if (l_TrackingQuest)
        {
            if (m_Owner->IsQuestBitFlaged(GetQuestUniqueBitFlag(l_TrackingQuest)))
                return;
            
            l_Type = GetTrackingVignetteTypeFromWorldObject(p_Target);
        }
        
        CreateAndAddVignette(l_VignetteEntry, p_Target->GetMapId(), l_Type, G3D::Vector3(p_Target->GetPositionX(), p_Target->GetPositionY(), p_Target->GetPositionZ()), p_Target->GetGUID());
    }
    
    template <class T>
    inline void Manager::OnWorldObjectDisappear(T const* p_Target)
    {
        auto l_VignetteEntry = GetVignetteEntryFromWorldObject(p_Target);
        if (l_VignetteEntry == nullptr)
            return;
        
        DestroyAndRemoveVignettes([p_Target](Vignette::Entity const* p_Vignette) -> bool
        {
            if (p_Vignette->GeSourceGuid() == p_Target->GetGUID()
                && p_Vignette->GetVignetteType() != Vignette::Type::SourceScript)
                return true;
            
            return false;
        });
    }

    template void Manager::OnWorldObjectDisappear(Corpse const*);
    template void Manager::OnWorldObjectDisappear(Creature const*);
    template void Manager::OnWorldObjectDisappear(GameObject const*);
    template void Manager::OnWorldObjectDisappear(DynamicObject const*);
    template void Manager::OnWorldObjectDisappear(AreaTrigger const*);
    template void Manager::OnWorldObjectDisappear(WorldObject const*);
    template void Manager::OnWorldObjectDisappear(Player const*);

    template void Manager::OnWorldObjectAppear(Corpse const*);
    template void Manager::OnWorldObjectAppear(Creature const*);
    template void Manager::OnWorldObjectAppear(GameObject const*);
    template void Manager::OnWorldObjectAppear(DynamicObject const*);
    template void Manager::OnWorldObjectAppear(AreaTrigger const*);
    template void Manager::OnWorldObjectAppear(WorldObject const*);
    template void Manager::OnWorldObjectAppear(Player const*);
}
