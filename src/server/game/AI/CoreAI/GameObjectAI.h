////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TRINITY_GAMEOBJECTAI_H
#define TRINITY_GAMEOBJECTAI_H

#include "Define.h"
#include "Common.h"
#include "Object.h"
#include "GameObject.h"
#include "CreatureAI.h"

class GameObjectAI
{
    protected:
        GameObject* const go;
    public:
        explicit GameObjectAI(GameObject* g) : go(g) {}
        virtual ~GameObjectAI() {}

        virtual void UpdateAI(uint32 /*diff*/) {}

        virtual void InitializeAI() { Reset(); }

        virtual void Reset() {};

        // Pass parameters between AI
        virtual void DoAction(const int32 /*param = 0 */) {}
        virtual void SetGUID(const uint64& /*guid*/, int32 /*id = 0 */) {}
        virtual uint64 GetGUID(int32 /*id = 0 */) { return 0; }

        static int Permissible(GameObject const* go);

        virtual bool GossipHello(Player* /*player*/) { return false; }
        virtual bool GossipSelect(Player* /*player*/, uint32 /*sender*/, uint32 /*action*/) { return false; }
        virtual bool GossipSelectCode(Player* /*player*/, uint32 /*sender*/, uint32 /*action*/, char const* /*code*/) { return false; }
        virtual bool QuestAccept(Player* /*player*/, Quest const* /*quest*/) { return false; }
        virtual bool QuestReward(Player* /*player*/, Quest const* /*quest*/, uint32 /*opt*/) { return false; }
        virtual uint32 GetDialogStatus(Player* /*player*/) { return 100; }
        virtual void Destroyed(Player* /*player*/, uint32 /*eventId*/) { }
        virtual uint32 GetData(uint32 /*id*/) { return 0; }
        virtual void SetData64(uint32 /*id*/, uint64 /*value*/) { }
        virtual uint64 GetData64(uint32 /*id*/) { return 0; }
        virtual void SetData(uint32 /*id*/, uint32 /*value*/) { }
        virtual void OnGameEvent(bool /*start*/, uint16 /*eventId*/) { }
        virtual void OnLootStateChanged(uint32 /*state*/, Unit* /*unit*/) { }
        virtual void OnStateChanged(uint32 /*p_State*/) { }
        virtual void EventInform(uint32 /*eventId*/) { }

        /// Add timed delayed operation
        /// @p_Timeout  : Delay time
        /// @p_Function : Callback function
        void AddTimedDelayedOperation(uint32 p_Timeout, std::function<void()> && p_Function)
        {
            m_EmptyWarned = false;
            m_TimedDelayedOperations.push_back(std::pair<uint32, std::function<void()>>(p_Timeout, p_Function));
        }

        /// Called after last delayed operation was deleted
        /// Do whatever you want
        virtual void LastOperationCalled() { }

        void UpdateOperations(uint32 const p_Diff);

        void ClearDelayedOperations()
        {
            m_TimedDelayedOperations.clear();
            m_EmptyWarned = false;
        }

        std::vector<std::pair<int32, std::function<void()>>>    m_TimedDelayedOperations;   ///< Delayed operations
        bool                                                    m_EmptyWarned;              ///< Warning when there are no more delayed operations
};

class NullGameObjectAI : public GameObjectAI
{
    public:
        explicit NullGameObjectAI(GameObject* g);

        void UpdateAI(uint32 /*diff*/) {}

        static int Permissible(GameObject const* /*go*/) { return PERMIT_BASE_IDLE; }
};
#endif
