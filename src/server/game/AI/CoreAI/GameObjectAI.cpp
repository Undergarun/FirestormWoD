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

#include "GameObjectAI.h"

//GameObjectAI::GameObjectAI(GameObject* g) : go(g) {}
int GameObjectAI::Permissible(const GameObject* go)
{
    if (go->GetAIName() == "GameObjectAI")
        return PERMIT_BASE_SPECIAL;
    return PERMIT_BASE_NO;
}

void GameObjectAI::UpdateOperations(uint32 const p_Diff)
{
    for (auto l_It = m_TimedDelayedOperations.begin(); l_It != m_TimedDelayedOperations.end(); l_It++)
    {
        l_It->first -= p_Diff;

        if (l_It->first < 0)
        {
            l_It->second();
            l_It->second = nullptr;
        }
    }

    uint32 l_TimedDelayedOperationCountToRemove = std::count_if(std::begin(m_TimedDelayedOperations), std::end(m_TimedDelayedOperations), [](const std::pair<int32, std::function<void()>> & p_Pair) -> bool
    {
        return p_Pair.second == nullptr;
    });

    for (uint32 l_I = 0; l_I < l_TimedDelayedOperationCountToRemove; l_I++)
    {
        auto l_It = std::find_if(std::begin(m_TimedDelayedOperations), std::end(m_TimedDelayedOperations), [](const std::pair<int32, std::function<void()>> & p_Pair) -> bool
        {
            return p_Pair.second == nullptr;
        });

        if (l_It != std::end(m_TimedDelayedOperations))
            m_TimedDelayedOperations.erase(l_It);
    }

    if (m_TimedDelayedOperations.empty() && !m_EmptyWarned)
    {
        m_EmptyWarned = true;
        LastOperationCalled();
    }
}

NullGameObjectAI::NullGameObjectAI(GameObject* g) : GameObjectAI(g) {}
