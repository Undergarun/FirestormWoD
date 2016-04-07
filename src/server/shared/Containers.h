////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CONTAINERS_H
#define CONTAINERS_H

#include "Common.h"

//! Because circular includes are bad
extern uint32 urand(uint32 min, uint32 max);

namespace JadeCore
{
    namespace Containers
    {
        template<class T>
        void RandomResizeSet(std::set<T> &t_set, uint32 size)
        {
            size_t set_size = t_set.size();

            while (set_size > size)
            {
                typename std::set<T>::iterator itr = t_set.begin();
                std::advance(itr, urand(0, set_size - 1));
                t_set.erase(itr);
                --set_size;
            }
        }

        template<class T>
        void RandomResizeList(std::list<T> &list, uint32 size)
        {
            size_t list_size = list.size();

            while (list_size > size)
            {
                typename std::list<T>::iterator itr = list.begin();
                std::advance(itr, urand(0, list_size - 1));
                list.erase(itr);
                --list_size;
            }
        }

        template<class T, class Predicate>
        void RandomResizeList(std::list<T> &list, Predicate& predicate, uint32 size)
        {
            //! First use predicate filter
            std::list<T> listCopy;
            for (typename std::list<T>::iterator itr = list.begin(); itr != list.end(); ++itr)
                if (predicate(*itr))
                    listCopy.push_back(*itr);

            if (size)
                RandomResizeList(listCopy, size);

            list = listCopy;
        }

        /* Select a random element from a container. Note: make sure you explicitly empty check the container */
        template <class C> typename C::value_type const& SelectRandomContainerElement(C const& container)
        {
            typename C::const_iterator it = container.begin();
            std::advance(it, urand(0, container.size() - 1));
            return *it;
        }
    }
    //! namespace Containers
}
//! namespace JadeCore

#endif //! #ifdef CONTAINERS_H