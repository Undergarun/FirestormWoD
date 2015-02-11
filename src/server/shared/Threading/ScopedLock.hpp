#ifndef SCOPED_LOCK_HPP
# define SCOPED_LOCK_HPP

namespace MS
{
    namespace Threading
    {
        /// A ScopedLock for owning an instance of a mutex (RAII idiom).
        /// It is usefull for controlling the state of a mutex (Lock and Unlock) at a scope level.
        template <typename MutexType>
        class ScopedLock
        {
            /// Delete all kind of copy/movable constructor/operator in order to not alter the mutex state.
            ScopedLock(ScopedLock const&) = delete;
            ScopedLock(ScopedLock const&&) = delete;
            ScopedLock& operator=(ScopedLock const&) = delete;
            ScopedLock& operator=(ScopedLock const&&) = delete;

        public:
            /// Lock the mutex when instantiating in order to get the property.
            /// @p_Mutex : The mutex we want to scope lock.
            ScopedLock(MutexType& p_Mutex)
                : m_Mutex(p_Mutex)
            {
                m_Mutex.lock();
            }

            /// Unlock the mutex so that to abandon the property.
            ~ScopedLock()
            {
                m_Mutex.unlock();
            }

            /// Get the property of the mutex.
            void Lock() const
            {
                m_Mutex.lock();
            }

            /// Abandon the property of the mutex.
            void Unlock() const
            {
                m_Mutex.unlock();
            }

            /// Try to get the property of the mutex.
            /// Return True if the object got the property.
            bool TryLock() const
            {
                return m_Mutex.try_lock();
            }

        private:
            /// The mutex. It's mutable because we may want to lock the mutex in a const-context method.
            MutexType& m_Mutex;
        };
    } ///< Namespace Threading
} ///< Namespace MS

#endif ///< SCOPED_LOCK_HPP
