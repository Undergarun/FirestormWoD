////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef MUTEXEDMAP_HPP_INCLUDED
# define MUTEXEDMAP_HPP_INCLUDED

#include "Common.h"

namespace MS { namespace Utilities
{
    /// Mutexed map base class
    template <typename Left, typename Right> class MutextedMap
    {
        public:
            using KeyType       = Left;
            using ElementType   = Right;
            using MapType       = std::map<Left, ElementType>;
            using LockType      = ACE_RW_Thread_Mutex;

        public:
            /// Constructor
            MutextedMap()
            {

            }

            /// Insert element
            /// @p_Key     : Element key
            /// @p_Element : Element to insert
            void Insert(KeyType& p_Key, ElementType& p_Element)
            {
                TRINITY_WRITE_GUARD(LockType, m_Lock);
                m_ObjectMap[p_Key] = p_Element;
            }
            /// Remove element
            /// @p_Key : Element key
            void Remove(KeyType& p_Key)
            {
                TRINITY_WRITE_GUARD(LockType, m_Lock);
                m_ObjectMap.erase(p_Key);
            }
            /// Find a element
            /// @p_Key : Element key
            ElementType Find(KeyType& p_Key)
            {
                TRINITY_READ_GUARD(LockType, m_Lock);
                typename MapType::iterator l_Itr = m_ObjectMap.find(p_Key);
                return (l_Itr != m_ObjectMap.end()) ? l_Itr->second : 0;
            }

            /// Find a element
            /// @p_Key : Element key
            ElementType Find(KeyType& p_Key) const
            {
                TRINITY_READ_GUARD(LockType, m_Lock);
                typename MapType::const_iterator l_Itr = m_ObjectMap.find(p_Key);
                return (l_Itr != m_ObjectMap.end()) ? l_Itr->second : 0;
            }

            /// Get container
            MapType& GetContainer()
            {
                return m_ObjectMap;
            }
            /// Get lock object
            LockType* GetLock()
            {
                return &m_Lock;
            }

        private:
            mutable LockType m_Lock; ///< Map lock
            MapType  m_ObjectMap;    ///< Map

    };

}   ///< namespace Utilities
}   ///< namespace MS

#endif  ///< MUTEXEDMAP_HPP_INCLUDED
