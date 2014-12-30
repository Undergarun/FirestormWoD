#ifndef SCOPED_MUTEX_HPP
# define SCOPED_MUTEX_HPP

# include <mutex>

namespace MS
{
    namespace Threading
    {
        class ScopedMutex
        {
        public:
            // The basic idea here is to take an existing mutex and to lock it from the construction to the destruction of the object (RAII idiom).
            ScopedMutex(std::mutex& p_Mutex)
                : m_Mutex(p_Mutex)
            {
                m_Mutex.lock();
            }

            ~ScopedMutex()
            {
                m_Mutex.unlock();
            }
        private:
            std::mutex& m_Mutex;
        };
    }
}

#endif /* !SCOPED_MUTEX */