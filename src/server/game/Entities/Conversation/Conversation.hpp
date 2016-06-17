////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CONVERSATION_HPP
# define CONVERSATION_HPP

# include "Object.h"
# include "Timer.h"

class Unit;

class Conversation : public WorldObject, public GridObject<Conversation>
{
    public:
        Conversation();
        ~Conversation();

        void AddToWorld();
        void RemoveFromWorld();

        bool StartConversation(Unit* p_Source, uint32 p_ConversationEntry, int32 p_Duration);

        void AddConversationActor(uint64 p_ActorGuid);

        void Update(uint32 p_Diff);

        void SetDuration(int32 p_Duration);
        int32 GetDuration() const;

        void SetUpdateTimerInterval(uint32 p_Timer);
        uint32 GetUpdateInterval() const;

    private:
        int32 m_Duration;

        IntervalTimer m_UpdateTimer;
};

#endif ///< CONVERSATION_HPP
