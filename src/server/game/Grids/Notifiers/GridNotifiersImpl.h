/*
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

#ifndef TRINITY_GRIDNOTIFIERSIMPL_H
#define TRINITY_GRIDNOTIFIERSIMPL_H

#include "GridNotifiers.h"
#include "WorldPacket.h"
#include "Corpse.h"
#include "Player.h"
#include "UpdateData.h"
#include "CreatureAI.h"
#include "SpellAuras.h"

template<class T>
inline void JadeCore::VisibleNotifier::Visit(GridVector<T*> &m)
{
    for (typename GridVector<T*>::iterator iter = m.begin(); iter != m.end(); ++iter)
    {
        vis_guids.erase((*iter)->GetGUID());
        i_player.UpdateVisibilityOf(*iter, i_data, i_visibleNow);
    }
}

inline void JadeCore::ObjectUpdater::Visit(CreatureMapType &m)
{
    m.m_Iterate = true;

    for (m.m_Idx = 0; m.m_Idx < m.size(); m.m_Idx++)
    {
        if (m[m.m_Idx]->IsInWorld())
            m[m.m_Idx]->Update(i_timeDiff);
    }

    m.m_Iterate = false;
    m.m_Idx     = 0;
}

inline void JadeCore::ObjectUpdater::Visit(GameObjectMapType &m)
{
    m.m_Iterate = true;
    uint32 l_Size = m.size();

    for (m.m_Idx = 0; m.m_Idx < l_Size; m.m_Idx++)
    {
        if (m[m.m_Idx]->IsInWorld() && !m[m.m_Idx]->IsTransport())
            m[m.m_Idx]->Update(i_timeDiff);
    }

    m.m_Iterate = false;
    m.m_Idx     = 0;
}

// SEARCHERS & LIST SEARCHERS & WORKERS

// WorldObject searchers & workers

template<class Check>
void JadeCore::WorldObjectSearcher<Check>::Visit(GameObjectMapType &m)
{
    if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_GAMEOBJECT))
        return;

    // already found
    if (i_object)
        return;

    for (GameObjectMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
        {
            i_object = (*itr);
            return;
        }
    }
}

template<class Check>
void JadeCore::WorldObjectSearcher<Check>::Visit(PlayerMapType &m)
{
    if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_PLAYER))
        return;

    // already found
    if (i_object)
        return;

    for (PlayerMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
        {
            i_object = (*itr);
            return;
        }
    }
}

template<class Check>
void JadeCore::WorldObjectSearcher<Check>::Visit(CreatureMapType &m)
{
    if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_CREATURE))
        return;

    // already found
    if (i_object)
        return;

    for (CreatureMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
        {
            i_object = (*itr);
            return;
        }
    }
}

template<class Check>
void JadeCore::WorldObjectSearcher<Check>::Visit(CorpseMapType &m)
{
    if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_CORPSE))
        return;

    // already found
    if (i_object)
        return;

    for (CorpseMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
        {
            i_object = (*itr);
            return;
        }
    }
}

template<class Check>
void JadeCore::WorldObjectSearcher<Check>::Visit(DynamicObjectMapType &m)
{
    if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_DYNAMICOBJECT))
        return;

    // already found
    if (i_object)
        return;

    for (DynamicObjectMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
        {
            i_object = (*itr);
            return;
        }
    }
}

template<class Check>
void JadeCore::WorldObjectSearcher<Check>::Visit(AreaTriggerMapType &m)
{
    if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_AREATRIGGER))
        return;

    // already found
    if (i_object)
        return;

    for (AreaTriggerMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
        {
            i_object = (*itr);
            return;
        }
    }
}

template<class Check>
void JadeCore::WorldObjectLastSearcher<Check>::Visit(GameObjectMapType &m)
{
    if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_GAMEOBJECT))
        return;

    for (GameObjectMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
            i_object = (*itr);
    }
}

template<class Check>
void JadeCore::WorldObjectLastSearcher<Check>::Visit(PlayerMapType &m)
{
    if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_PLAYER))
        return;

    for (PlayerMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
            i_object = (*itr);
    }
}

template<class Check>
void JadeCore::WorldObjectLastSearcher<Check>::Visit(CreatureMapType &m)
{
    if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_CREATURE))
        return;

    for (CreatureMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
            i_object = (*itr);
    }
}

template<class Check>
void JadeCore::WorldObjectLastSearcher<Check>::Visit(CorpseMapType &m)
{
    if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_CORPSE))
        return;

    for (CorpseMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
            i_object = (*itr);
    }
}

template<class Check>
void JadeCore::WorldObjectLastSearcher<Check>::Visit(DynamicObjectMapType &m)
{
    if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_DYNAMICOBJECT))
        return;

    for (DynamicObjectMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
            i_object = (*itr);
    }
}

template<class Check>
void JadeCore::WorldObjectLastSearcher<Check>::Visit(AreaTriggerMapType  &m)
{
    if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_AREATRIGGER))
        return;

    for (AreaTriggerMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
            i_object = (*itr);
    }
}

template<class Check>
void JadeCore::WorldObjectListSearcher<Check>::Visit(PlayerMapType &m)
{
    if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_PLAYER))
        return;

    for (PlayerMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
        if (i_check((*itr)))
            i_objects.push_back((*itr));
}

template<class Check>
void JadeCore::WorldObjectListSearcher<Check>::Visit(CreatureMapType &m)
{
    if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_CREATURE))
        return;

    for (CreatureMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
        if (i_check((*itr)))
            i_objects.push_back((*itr));
}

template<class Check>
void JadeCore::WorldObjectListSearcher<Check>::Visit(CorpseMapType &m)
{
    if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_CORPSE))
        return;

    for (CorpseMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
        if (i_check((*itr)))
            i_objects.push_back((*itr));
}

template<class Check>
void JadeCore::WorldObjectListSearcher<Check>::Visit(GameObjectMapType &m)
{
    if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_GAMEOBJECT))
        return;

    for (GameObjectMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
        if (i_check((*itr)))
            i_objects.push_back((*itr));
}

template<class Check>
void JadeCore::WorldObjectListSearcher<Check>::Visit(DynamicObjectMapType &m)
{
    if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_DYNAMICOBJECT))
        return;

    for (DynamicObjectMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
        if (i_check((*itr)))
            i_objects.push_back((*itr));
}

template<class Check>
void JadeCore::WorldObjectListSearcher<Check>::Visit(AreaTriggerMapType &m)
{
    if (!(i_mapTypeMask & GRID_MAP_TYPE_MASK_AREATRIGGER))
        return;

    for (AreaTriggerMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
        if (i_check((*itr)))
            i_objects.push_back((*itr));
}

/// AreaTrigger searchers

template<class Check>
void JadeCore::AreaTriggerListSearcher<Check>::Visit(AreaTriggerMapType& p_AreaTriggerMap)
{
    for (AreaTriggerMapType::iterator l_Iterator = p_AreaTriggerMap.begin(); l_Iterator != p_AreaTriggerMap.end(); ++l_Iterator)
    {
        if ((*l_Iterator)->InSamePhase(m_PhaseMask))
        {
            if (m_Check(*l_Iterator))
                m_AreaTriggers.push_back(*l_Iterator);
        }
    }
}

template<class Check>
void JadeCore::AreaTriggerSearcher<Check>::Visit(AreaTriggerMapType& p_AreatriggerMap)
{
    // already found
    if (i_object)
        return;

    for (AreaTriggerMapType::iterator itr=p_AreatriggerMap.begin(); itr != p_AreatriggerMap.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
        {
            i_object = (*itr);
            return;
        }
    }

}
/// Gameobject searchers

template<class Check>
void JadeCore::GameObjectSearcher<Check>::Visit(GameObjectMapType &m)
{
    // already found
    if (i_object)
        return;

    for (GameObjectMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
        {
            i_object = (*itr);
            return;
        }
    }
}

template<class Check>
void JadeCore::GameObjectLastSearcher<Check>::Visit(GameObjectMapType &m)
{
    for (GameObjectMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
            i_object = (*itr);
    }
}

template<class Check>
void JadeCore::GameObjectListSearcher<Check>::Visit(GameObjectMapType &m)
{
    for (GameObjectMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
        if ((*itr)->InSamePhase(i_phaseMask))
            if (i_check((*itr)))
                i_objects.push_back((*itr));
}

// Unit searchers

template<class Check>
void JadeCore::UnitSearcher<Check>::Visit(CreatureMapType &m)
{
    // already found
    if (i_object)
        return;

    for (CreatureMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
        {
            i_object = (*itr);
            return;
        }
    }
}

template<class Check>
void JadeCore::UnitSearcher<Check>::Visit(PlayerMapType &m)
{
    // already found
    if (i_object)
        return;

    for (PlayerMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
        {
            i_object = (*itr);
            return;
        }
    }
}

template<class Check>
void JadeCore::UnitLastSearcher<Check>::Visit(CreatureMapType &m)
{
    for (CreatureMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
            i_object = (*itr);
    }
}

template<class Check>
void JadeCore::UnitLastSearcher<Check>::Visit(PlayerMapType &m)
{
    for (PlayerMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
            i_object = (*itr);
    }
}

template<class Check>
void JadeCore::UnitListSearcher<Check>::Visit(PlayerMapType &m)
{
    for (PlayerMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
        if ((*itr)->InSamePhase(i_phaseMask))
            if (i_check((*itr)))
                i_objects.push_back((*itr));
}

template<class Check>
void JadeCore::UnitListSearcher<Check>::Visit(CreatureMapType &m)
{
    for (CreatureMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
        if ((*itr)->InSamePhase(i_phaseMask))
            if (i_check((*itr)))
                i_objects.push_back((*itr));
}

// Creature searchers
template<class Check>
void JadeCore::CreatureSearcher<Check>::Visit(CreatureMapType &m)
{
    // already found
    if (i_object)
        return;

    for (CreatureMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
        {
            i_object = (*itr);
            return;
        }
    }
}

template<class Check>
void JadeCore::CreatureLastSearcher<Check>::Visit(CreatureMapType &m)
{
    for (CreatureMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
            i_object = (*itr);
    }
}

template<class Check>
void JadeCore::CreatureListSearcher<Check>::Visit(CreatureMapType &m)
{
    for (CreatureMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
        if ((*itr)->InSamePhase(i_phaseMask))
            if (i_check((*itr)))
                i_objects.push_back((*itr));
}

template<class Check>
void JadeCore::PlayerListSearcher<Check>::Visit(PlayerMapType &m)
{
    for (PlayerMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr))
            continue;

        if ((*itr)->InSamePhase(i_phaseMask))
            if (i_check((*itr)))
                i_objects.push_back((*itr));
    }
}

template<class Check>
void JadeCore::PlayerSearcher<Check>::Visit(PlayerMapType &m)
{
    // already found
    if (i_object)
        return;

    for (PlayerMapType::iterator itr=m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
        {
            i_object = (*itr);
            return;
        }
    }
}

template<class Check>
void JadeCore::PlayerLastSearcher<Check>::Visit(PlayerMapType& m)
{
    for (PlayerMapType::iterator itr = m.begin(); itr != m.end(); ++itr)
    {
        if (!(*itr)->InSamePhase(i_phaseMask))
            continue;

        if (i_check((*itr)))
            i_object = (*itr);
    }
}

template<class Builder>
void JadeCore::LocalizedPacketDo<Builder>::operator()(Player* p)
{
    LocaleConstant loc_idx = p->GetSession()->GetSessionDbLocaleIndex();
    uint32 cache_idx = loc_idx+1;
    WorldPacket* data;

    // create if not cached yet
    if (i_data_cache.size() < cache_idx+1 || !i_data_cache[cache_idx])
    {
        if (i_data_cache.size() < cache_idx+1)
            i_data_cache.resize(cache_idx+1);

        data = new WorldPacket(SMSG_CHAT, 800);

        i_builder(*data, loc_idx);

        i_data_cache[cache_idx] = data;
    }
    else
        data = i_data_cache[cache_idx];

    p->SendDirectMessage(data);
}

template<class Builder>
void JadeCore::LocalizedPacketListDo<Builder>::operator()(Player* p)
{
    LocaleConstant loc_idx = p->GetSession()->GetSessionDbLocaleIndex();
    uint32 cache_idx = loc_idx+1;
    WorldPacketList* data_list;

    // create if not cached yet
    if (i_data_cache.size() < cache_idx+1 || i_data_cache[cache_idx].empty())
    {
        if (i_data_cache.size() < cache_idx+1)
            i_data_cache.resize(cache_idx+1);

        data_list = &i_data_cache[cache_idx];

        i_builder(*data_list, loc_idx);
    }
    else
        data_list = &i_data_cache[cache_idx];

    for (size_t i = 0; i < data_list->size(); ++i)
        p->SendDirectMessage((*data_list)[i]);
}

#endif                                                      // TRINITY_GRIDNOTIFIERSIMPL_H
