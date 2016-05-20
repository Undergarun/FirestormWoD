////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAP_UPDATER_H_INCLUDED
#define _MAP_UPDATER_H_INCLUDED

#include "Define.h"
#include "Common.h"
#include <condition_variable>
#include "ProducerConsumerQueue.h"

class MapUpdater;

class MapUpdaterTask
{
    public:
        /// Constructor
        MapUpdaterTask(MapUpdater* p_Updater);

        virtual void call() = 0;

        /// Notify that the task is done
        void UpdateFinished();

    private:
        MapUpdater* m_updater;

};

class Map;

class MapUpdater
{
    public:

        MapUpdater() : _cancelationToken(false), pending_requests(0) {}
        ~MapUpdater() { };

        friend class MapUpdaterTask;

        void schedule_update(Map& map, uint32 diff);
        void schedule_specific(MapUpdaterTask* p_Request);

        void wait();

        void activate(size_t num_threads);

        void deactivate();

        bool activated();

    private:

        ProducerConsumerQueue<MapUpdaterTask*> _queue;

        std::vector<std::thread> _workerThreads;
        std::atomic<bool> _cancelationToken;

        std::mutex _lock;
        std::condition_variable _condition;
        size_t pending_requests;

        void update_finished();

        void WorkerThread();
};

#endif //_MAP_UPDATER_H_INCLUDED
