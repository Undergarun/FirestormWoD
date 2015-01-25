////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "ScriptedCosmeticAI.hpp"

namespace MS { namespace AI 
{
    /// Constructor
    CosmeticAI::CosmeticAI(Creature * p_Creature)
        : CreatureAI(p_Creature)
    {

    }
    /// Destructor
    CosmeticAI::~CosmeticAI()
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Add timed delayed operation
    /// @p_Timeout  : Delay time
    /// @p_Function : Callback function
    void CosmeticAI::AddTimedDelayedOperation(uint32 p_Timeout, std::function<void()> && p_Function)
    {
        m_TimedDelayedOperations.push_back(std::pair<uint32, std::function<void()>>(p_Timeout, p_Function));
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Set AI obstacle manager enabled
    /// @p_Enabled : Is enabled
    void CosmeticAI::SetAIObstacleManagerEnabled(bool p_Enabled)
    {
        if (p_Enabled)
            me->SetFlag(UNIT_FIELD_NPC_FLAGS + 1, UNIT_NPC_FLAG2_AI_OBSTACLE);
        else
            me->RemoveFlag(UNIT_FIELD_NPC_FLAGS + 1, UNIT_NPC_FLAG2_AI_OBSTACLE);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On AI Update
    /// @p_Diff : Time since last update
    void CosmeticAI::UpdateAI(const uint32 p_Diff)
    {
        while (!m_DelayedOperations.empty())
        {
            m_DelayedOperations.front()();
            m_DelayedOperations.pop();
        }

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
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called at waypoint reached or point movement finished
    /// @p_Type : Movement Type
    /// @p_ID   : Misc ID
    void CosmeticAI::MovementInform(uint32 p_Type, uint32 p_ID)
    {
        if (p_Type == POINT_MOTION_TYPE)
        {
            if (m_OnPointReached.find(p_ID) != m_OnPointReached.end())
            {
                m_DelayedOperations.push([this, p_ID]() -> void
                {
                    if (m_OnPointReached.find(p_ID) != m_OnPointReached.end())
                        m_OnPointReached[p_ID]();
                });
            }
        }
    }

}   ///< namespace AI
}   ///< namespace MS