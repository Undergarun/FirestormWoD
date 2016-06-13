////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

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
    for (uint32 l_I = 0; l_I < m_TimedDelayedOperations.size(); ++l_I)
    {
        auto& l_Pair = m_TimedDelayedOperations[l_I];

        l_Pair.first -= p_Diff;

        if (l_Pair.first < 0)
        {
            l_Pair.second();

            m_TimedDelayedOperations.erase(m_TimedDelayedOperations.begin() + l_I);
        }
    }

    if (m_TimedDelayedOperations.empty() && !m_EmptyWarned)
    {
        m_EmptyWarned = true;
        LastOperationCalled();
    }
}

NullGameObjectAI::NullGameObjectAI(GameObject* g) : GameObjectAI(g) {}
