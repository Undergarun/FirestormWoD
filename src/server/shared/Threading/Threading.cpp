////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Threading.h"
#include "Errors.h"
#include <ace/OS_NS_unistd.h>
#include <ace/Sched_Params.h>
#include "Common.h"

using namespace ACE_Based;

ThreadPriority::ThreadPriority()
{
    for (int i = Idle; i < MAXPRIORITYNUM; ++i)
        m_priority[i] = ACE_THR_PRI_OTHER_DEF;

    m_priority[Idle] = ACE_Sched_Params::priority_min(ACE_SCHED_OTHER);
    m_priority[Realtime] = ACE_Sched_Params::priority_max(ACE_SCHED_OTHER);

    std::vector<int> _tmp;

    ACE_Sched_Params::Policy _policy = ACE_SCHED_OTHER;
    ACE_Sched_Priority_Iterator pr_iter(_policy);

    while (pr_iter.more())
    {
        _tmp.push_back(pr_iter.priority());
        pr_iter.next();
    }

    ASSERT (!_tmp.empty());

    if (_tmp.size() >= MAXPRIORITYNUM)
    {
        const size_t max_pos = _tmp.size();
        size_t min_pos = 1;
        size_t norm_pos = 0;
        for (size_t i = 0; i < max_pos; ++i)
        {
            if (_tmp[i] == ACE_THR_PRI_OTHER_DEF)
            {
                norm_pos = i + 1;
                break;
            }
        }

        //since we have only 7(seven) values in enum Priority
        //and 3 we know already (Idle, Normal, Realtime) so
        //we need to split each list [Idle...Normal] and [Normal...Realtime]
        //into 4 piesces
        const size_t _divider = 4;
        size_t _div = (norm_pos - min_pos) / _divider;
        if (_div == 0)
            _div = 1;

        min_pos = (norm_pos - 1);

        m_priority[Low] = _tmp[min_pos -= _div];
        m_priority[Lowest] = _tmp[min_pos -= _div ];

        _div = (max_pos - norm_pos) / _divider;
        if (_div == 0)
            _div = 1;

        min_pos = norm_pos - 1;

        m_priority[High] = _tmp[min_pos += _div];
        m_priority[Highest] = _tmp[min_pos += _div];
    }
}

int ThreadPriority::getPriority(Priority p) const
{
    if (p < Idle)
        p = Idle;

    if (p > Realtime)
        p = Realtime;

    return m_priority[p];
}

#define THREADFLAG (THR_NEW_LWP | THR_SCHED_DEFAULT| THR_JOINABLE)

Thread::Thread(const std::string & p_Name) : m_iThreadId(0), m_hThreadHandle(0), m_task(0), m_Name(p_Name)
{

}

Thread::Thread(Runnable* instance, const std::string & p_Name) : m_iThreadId(0), m_hThreadHandle(0), m_task(instance), m_Name(p_Name)
{
    // register reference to m_task to prevent it deeltion until destructor
    if (m_task)
        m_task->incReference();

    bool _start = start();
    ASSERT (_start);
}

Thread::~Thread()
{
    //Wait();

    // deleted runnable object (if no other references)
    if (m_task)
        m_task->decReference();
}

//initialize Thread's class static member
Thread::ThreadStorage Thread::m_ThreadStorage;
ThreadPriority Thread::m_TpEnum;

bool Thread::start()
{
    if (m_task == 0 || m_iThreadId != 0)
        return false;

    bool res = (ACE_Thread::spawn(&Thread::ThreadTask, (void*)new std::pair<Runnable*, std::string>(m_task, m_Name), THREADFLAG, &m_iThreadId, &m_hThreadHandle, 0) == 0);

    if (res)
        m_task->incReference();

    return res;
}

bool Thread::wait()
{
    if (!m_hThreadHandle || !m_task)
        return false;

    ACE_THR_FUNC_RETURN _value = ACE_THR_FUNC_RETURN(-1);
    int _res = ACE_Thread::join(m_hThreadHandle, &_value);

    m_iThreadId = 0;
    m_hThreadHandle = 0;

    return (_res == 0);
}

void Thread::destroy()
{
    if (!m_iThreadId || !m_task)
        return;

    if (ACE_Thread::kill(m_iThreadId, -1) != 0)
        return;

    m_iThreadId = 0;
    m_hThreadHandle = 0;

    // reference set at ACE_Thread::spawn
    m_task->decReference();
}

void Thread::suspend()
{
    ACE_Thread::suspend(m_hThreadHandle);
}

void Thread::resume()
{
    ACE_Thread::resume(m_hThreadHandle);
}

ACE_THR_FUNC_RETURN Thread::ThreadTask(void * param)
{
    std::pair<Runnable*, std::string> * l_Params = reinterpret_cast<std::pair<Runnable*, std::string>*>(param);
    Thread::current()->setName((char*)l_Params->second.c_str());
    Runnable* _task = l_Params->first;

    delete l_Params;

    _task->run();

    // task execution complete, free referecne added at
    _task->decReference();

    return (ACE_THR_FUNC_RETURN)0;
}

ACE_thread_t Thread::currentId()
{
    return ACE_Thread::self();
}

ACE_hthread_t Thread::currentHandle()
{
    ACE_hthread_t _handle;
    ACE_Thread::self(_handle);

    return _handle;
}

Thread * Thread::current()
{
    Thread * _thread = m_ThreadStorage.ts_object();
    if (!_thread)
    {
        _thread = new Thread();
        _thread->m_iThreadId = Thread::currentId();
        _thread->m_hThreadHandle = Thread::currentHandle();

        Thread * _oldValue = m_ThreadStorage.ts_object(_thread);
        if (_oldValue)
            delete _oldValue;
    }

    return _thread;
}

void Thread::setPriority(Priority type)
{
    int _priority = m_TpEnum.getPriority(type);
    int _ok = ACE_Thread::setprio(m_hThreadHandle, _priority);
    //remove this ASSERT in case you don't want to know is thread priority change was successful or not
    ASSERT (_ok == 0);
}
void Thread::setName(const char* p_Name)
{
#ifdef _MSC_VER
    struct THREADNAME_INFO
    {
        DWORD dwType; // must be 0x1000
        LPCSTR szName; // pointer to name (in user addr space)
        DWORD dwThreadID; // thread ID (-1=caller thread)
        DWORD dwFlags; // reserved for future use, must be zero
    };

    __try
    {
        THREADNAME_INFO l_Info;
        {
            l_Info.dwType = 0x1000;
            l_Info.szName = p_Name;
            l_Info.dwThreadID = GetCurrentThreadId();
            l_Info.dwFlags = 0;
        }

        RaiseException(0x406D1388, 0, sizeof(l_Info) / sizeof(DWORD), (ULONG_PTR*) &l_Info);
    }
    __except (EXCEPTION_CONTINUE_EXECUTION)
    {
    }
#elif PLATFORM == PLATFORM_UNIX
    pthread_setname_np(pthread_self(), p_Name);
#elif PLATFORM == PLATFORM_APPLE
    pthread_setname_np(p_Name);
#endif // _DEBUG

}

void Thread::Sleep(unsigned long msecs)
{
    ACE_OS::sleep(ACE_Time_Value(0, 1000 * msecs));
}
