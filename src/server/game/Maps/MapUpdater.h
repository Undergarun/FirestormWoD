#ifndef _MAP_UPDATER_H_INCLUDED
#define _MAP_UPDATER_H_INCLUDED

#include <ace/Thread_Mutex.h>
#include <ace/Condition_Thread_Mutex.h>

#include "DelayExecutor.h"

class Map;

class MapUpdater
{
    public:

        MapUpdater();
        virtual ~MapUpdater();

        int schedule_update(Map& map, ACE_UINT32 diff);
        int schedule_specific(ACE_Method_Request* p_Request);

        int wait();

        int activate(size_t num_threads);

        int deactivate();

        bool activated();

        void _update_finished();

    private:

        DelayExecutor m_executor;
        ACE_Thread_Mutex m_mutex;
        ACE_Condition_Thread_Mutex m_condition;
        size_t pending_requests;
};

#endif //_MAP_UPDATER_H_INCLUDED
