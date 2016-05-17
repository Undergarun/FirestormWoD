////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef SCRIPTED_COMESTIC_AI_HPP_SCRIPTED_AI
# define SCRIPTED_COMESTIC_AI_HPP_SCRIPTED_AI

#include "Creature.h"
#include "CreatureAI.h"
#include "Common.h"

namespace MS { namespace AI 
{

    struct CosmeticAI : public CreatureAI
    {
        public:
            /// Constructor
            explicit CosmeticAI(Creature * p_Creature) : CreatureAI(p_Creature)
            {
                m_EmptyWarned = true;
            }

            /// Destructor
            virtual ~CosmeticAI()
            {

            }

            /// Add timed delayed operation
            /// @p_Timeout  : Delay time
            /// @p_Function : Callback function
            void AddTimedDelayedOperation(uint32 p_Timeout, std::function<void()> && p_Function)
            {
                m_EmptyWarned = false;
                m_TimedDelayedOperations.push_back(std::pair<uint32, std::function<void()>>(p_Timeout, p_Function));
            }

            /// Set AI obstacle manager enabled
            /// @p_Enabled : Is enabled
            void SetAIObstacleManagerEnabled(bool p_Enabled)
            {
                if (p_Enabled)
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS + 1, UNIT_NPC_FLAG2_AI_OBSTACLE);
                else
                    me->RemoveFlag(UNIT_FIELD_NPC_FLAGS + 1, UNIT_NPC_FLAG2_AI_OBSTACLE);
            }

            /// Called after last delayed operation was deleted
            /// Do whatever you want
            virtual void LastOperationCalled() { }

        public:
            /// On AI Update
            /// @p_Diff : Time since last update
            virtual void UpdateAI(const uint32 p_Diff) override
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

                uint32 l_TimedDelayedOperationCountToRemove = (uint32)std::count_if(std::begin(m_TimedDelayedOperations), std::end(m_TimedDelayedOperations), [](std::pair<int32, std::function<void()>> const& p_Pair) -> bool
                {
                    return p_Pair.second == nullptr;
                });

                for (uint32 l_I = 0; l_I < l_TimedDelayedOperationCountToRemove; l_I++)
                {
                    auto l_It = std::find_if(std::begin(m_TimedDelayedOperations), std::end(m_TimedDelayedOperations), [](std::pair<int32, std::function<void()>> const& p_Pair) -> bool
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

            /// Called at waypoint reached or point movement finished
            /// @p_Type : Movement Type
            /// @p_ID   : Misc ID
            virtual void MovementInform(uint32 p_Type, uint32 p_ID) override
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

        public:
            std::map<uint32, std::function<void()>>                 m_OnPointReached;           ///< Delayed operations
            std::queue<std::function<void()>>                       m_DelayedOperations;        ///< Delayed operations
            std::vector<std::pair<int32, std::function<void()>>>    m_TimedDelayedOperations;   ///< Delayed operations
            bool                                                    m_EmptyWarned;              ///< Warning when there are no more delayed operations

    };

}   ///< namespace AI
}   ///< namespace MS

#endif  ///< SCRIPTED_COMESTIC_AI_HPP_SCRIPTED_AI
