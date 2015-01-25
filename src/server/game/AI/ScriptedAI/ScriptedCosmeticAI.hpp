////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef SCRIPTED_COMESTIC_AI_HPP_SCRIPTED_AI
#define SCRIPTED_COMESTIC_AI_HPP_SCRIPTED_AI

#include "Creature.h"
#include "CreatureAI.h"

#include <map>
#include <vector>

namespace MS { namespace AI 
{

    struct CosmeticAI : public CreatureAI
    {
        public:
            /// Constructor
            explicit CosmeticAI(Creature * p_Creature);
            /// Destructor
            virtual ~CosmeticAI();

            /// Add timed delayed operation
            /// @p_Timeout  : Delay time
            /// @p_Function : Callback function
            void AddTimedDelayedOperation(uint32 p_Timeout, std::function<void()> && p_Function);

            /// Set AI obstacle manager enabled
            /// @p_Enabled : Is enabled
            void SetAIObstacleManagerEnabled(bool p_Enabled);

        public:
            /// On AI Update
            /// @p_Diff : Time since last update
            virtual void UpdateAI(const uint32 p_Diff) override;

            /// Called at waypoint reached or point movement finished
            /// @p_Type : Movement Type
            /// @p_ID   : Misc ID
            virtual void MovementInform(uint32 p_Type, uint32 p_ID) override;

        protected:
            std::map<uint32, std::function<void()>>                 m_OnPointReached;           ///< Delayed operations
            std::queue<std::function<void()>>                       m_DelayedOperations;        ///< Delayed operations
            std::vector<std::pair<int32, std::function<void()>>>    m_TimedDelayedOperations;   ///< Delayed operations

    };

}   ///< namespace AI
}   ///< namespace MS

#endif  ///< SCRIPTED_COMESTIC_AI_HPP_SCRIPTED_AI