////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef SCRIPT_UTILS_H
# define SCRIPT_UTILS_H
# include "ScriptPCH.h"
# include "ScriptMgr.h"

namespace MS
{
    namespace ScriptUtils
    {
        inline GameObject* SelectNearestGameObjectWithEntry(Unit* p_Me, uint32 p_Entry, float p_Range = 0.0f)
        {
            std::list<GameObject*> l_TargetList;

            JadeCore::NearestGameObjectEntryInObjectRangeCheck l_Check(*p_Me, p_Entry, p_Range);
            JadeCore::GameObjectListSearcher<JadeCore::NearestGameObjectEntryInObjectRangeCheck> l_Searcher(p_Me, l_TargetList, l_Check);
            p_Me->VisitNearbyObject(p_Range, l_Searcher);

            for (GameObject* l_Gob : l_TargetList)
                return l_Gob;

            return nullptr;
        }

        inline Creature* SelectNearestCreatureWithEntry(Unit* p_Me, uint32 p_Entry, float p_Range = 0.0f)
        {
            std::list<Unit*> l_TargetList;
            float l_Radius = p_Range;

            JadeCore::AnyUnitInObjectRangeCheck l_Check(p_Me, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyUnitInObjectRangeCheck> l_Searcher(p_Me, l_TargetList, l_Check);
            p_Me->VisitNearbyObject(l_Radius, l_Searcher);

            std::list<Unit*> l_Results;

            for (Unit* l_Unit : l_TargetList)
            {
                if (l_Unit->GetTypeId() == TYPEID_UNIT && l_Unit->GetEntry() == p_Entry)
                    return static_cast<Creature*>(l_Unit);
            }

            return nullptr;
        }

        inline Unit* SelectRandomCreatureWithEntry(Unit* p_Me, uint32 p_Entry, float p_Range = 0.0f)
        {
            std::list<Unit*> l_TargetList;
            float l_Radius = p_Range;

            JadeCore::AnyUnitInObjectRangeCheck l_Check(p_Me, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyUnitInObjectRangeCheck> l_Searcher(p_Me, l_TargetList, l_Check);
            p_Me->VisitNearbyObject(l_Radius, l_Searcher);

            std::list<Unit*> l_Results;
            uint32 l_Size = 0;
            for (Unit* l_Unit : l_TargetList)
            {
                if (l_Unit->GetEntry() == p_Entry)
                {
                    l_Results.emplace_back(l_Unit);
                    l_Size++;
                }
            }

            if (l_Results.empty())
                return nullptr;

            auto l_RandUnit = l_Results.begin();
            std::advance(l_RandUnit, urand(0, l_Size - 1));

            return *l_RandUnit;
        }

        inline std::list<Unit*> SelectNearestCreatureListWithEntry(Unit* p_Me, uint32 p_Entry, float p_Range = 0.0f)
        {
            std::list<Unit*> l_TargetList;
            float l_Radius = p_Range;

            JadeCore::AnyUnitInObjectRangeCheck l_Check(p_Me, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyUnitInObjectRangeCheck> l_Searcher(p_Me, l_TargetList, l_Check);
            p_Me->VisitNearbyObject(l_Radius, l_Searcher);

            std::list<Unit*> l_Results;

            for (Unit* l_Unit : l_TargetList)
            {
                if (l_Unit->GetEntry() == p_Entry)
                    l_Results.emplace_back(l_Unit);
            }

            return l_Results;
        }

        inline Unit* SelectRandomEnnemy(Unit* p_Me, float p_Range = 0.0f, bool p_CheckLoS = true)
        {
            std::list<Unit*> l_TargetList;
            float l_Radius = p_Range;

            JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(p_Me, p_Me, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(p_Me, l_TargetList, l_Check);
            p_Me->VisitNearbyObject(l_Radius, l_Searcher);

            l_TargetList.remove_if([p_Me, p_Range, p_CheckLoS](Unit* p_Unit) {
                return !(p_Unit && (p_Me->IsWithinLOSInMap(p_Unit) || !p_CheckLoS) &&
                    p_Me->IsWithinDistInMap(p_Unit, p_Range) && p_Unit->isAlive() && p_Unit->GetGUID() != p_Me->GetGUID());
            });
            if (l_TargetList.empty())
                return nullptr;

            auto l_Itr = std::begin(l_TargetList);
            std::advance(l_Itr, urand(0, l_TargetList.size() - 1));

            return *l_Itr;
        }

        inline Unit* SelectNearestFriendExcluededMe(Unit* p_Me, float p_Range = 0.0f, bool p_CheckLoS = true)
        {
            std::list<Unit*> l_TargetList;
            float l_Radius = p_Range;

            JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(p_Me, p_Me, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(p_Me, l_TargetList, l_Check);
            p_Me->VisitNearbyObject(l_Radius, l_Searcher);

            for (Unit* l_Unit : l_TargetList)
            {
                if (l_Unit && (p_Me->IsWithinLOSInMap(l_Unit) || !p_CheckLoS) &&
                    p_Me->IsWithinDistInMap(l_Unit, p_Range) && l_Unit->isAlive() && l_Unit->GetGUID() != p_Me->GetGUID())
                {
                    return l_Unit;
                }
            }

            return nullptr;
        }

        inline Player* SelectNearestPlayer(Unit* p_me, float p_range = 0.0f, bool p_checkLoS = true)
        {
            Map* map = p_me->GetMap();
            if (!map->IsDungeon())
                return nullptr;

            Map::PlayerList const &PlayerList = map->GetPlayers();
            if (PlayerList.isEmpty())
                return nullptr;

            std::list<Player*> temp;
            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (!i->getSource()->isGameMaster() && (p_me->IsWithinLOSInMap(i->getSource()) || !p_checkLoS) &&
                    p_me->GetExactDist2d(i->getSource()) < p_range && i->getSource()->isAlive())
                    temp.push_back(i->getSource());
            }

            if (!temp.empty())
                return temp.front();

            return nullptr;
        }

        inline void ApplyOnEveryPlayer(Unit* p_Me, std::function<void(Unit*, Player*)> p_Function)
        {
            Map* map = p_Me->GetMap();
            if (!map->IsDungeon())
                return;

            Map::PlayerList const &PlayerList = map->GetPlayers();
            if (PlayerList.isEmpty())
                return;

            std::list<Player*> temp;
            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                p_Function(p_Me, i->getSource());
        }

        inline Player* SelectRandomPlayerExcludedTank(Unit* p_me, float p_range = 0.0f, bool p_checkLoS = true)
        {
            Map* map = p_me->GetMap();
            if (!map->IsDungeon())
                return nullptr;

            Map::PlayerList const &PlayerList = map->GetPlayers();
            if (PlayerList.isEmpty())
                return nullptr;

            std::list<Player*> temp;
            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (!i->getSource()->isGameMaster() && (p_me->IsWithinLOSInMap(i->getSource()) || !p_checkLoS) && p_me->getVictim() != i->getSource() &&
                    p_me->IsWithinDistInMap(i->getSource(), p_range) && i->getSource()->isAlive())
                    temp.push_back(i->getSource());
            }

            if (!temp.empty())
            {
                std::list<Player*>::const_iterator j = temp.begin();
                advance(j, rand() % temp.size());
                return (*j);
            }
            return nullptr;
        }


        inline Player* SelectFarEnoughPlayerIncludedTank(Unit* p_me, float p_range = 0.0f, bool p_checkLoS = true)
        {
            Map* map = p_me->GetMap();
            if (!map->IsDungeon())
                return nullptr;

            Map::PlayerList const &PlayerList = map->GetPlayers();
            if (PlayerList.isEmpty())
                return nullptr;

            std::list<Player*> temp;
            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (!i->getSource()->isGameMaster() && (p_me->IsWithinLOSInMap(i->getSource()) || !p_checkLoS)
                    && !p_me->IsWithinDistInMap(i->getSource(), p_range)
                    && i->getSource()->isAlive())
                    return i->getSource();
            }

            return nullptr;
        }

        inline Player* SelectRandomPlayerIncludedTank(Unit* p_me, float p_range = 0.0f, bool p_checkLoS = true)
        {
            Map* map = p_me->GetMap();
            if (!map->IsDungeon())
                return nullptr;

            Map::PlayerList const &PlayerList = map->GetPlayers();
            if (PlayerList.isEmpty())
                return nullptr;

            std::list<Player*> temp;
            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (!i->getSource()->isGameMaster() && (p_me->IsWithinLOSInMap(i->getSource()) || !p_checkLoS)
                    && p_me->IsWithinDistInMap(i->getSource(), p_range)
                    && i->getSource()->isAlive())
                    temp.push_back(i->getSource());
            }

            if (!temp.empty())
            {
                std::list<Player*>::const_iterator j = temp.begin();
                advance(j, rand() % temp.size());
                return (*j);
            }
            return nullptr;
        }
    }
}

#endif /* !SCRIPT_UTILS */
