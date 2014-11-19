#ifndef SKYREACH_INSTANCE_H
# define SKYREACH_INSTANCE_H

# include "ScriptMgr.h"

namespace MS
{
    enum class RandomSpells : uint32
    {
        INSTANCE_BOOTSTRAPPER = 171344,
        DRAENOR_SCALING_AURA = 156832,
        FORGETFUL = 152828,
        SABOTEUR = 152983,
        STEALTH_AND_INVISIBILITY_DETECTION = 141048,
        MOD_SCALE_70_130 = 151051,
        DORMANT = 160641,
        SUMMON_INTRO_DREAD_RAVEN = 163831,
        RIDE_VEHICLE_HARDCODED = 46598,
        CLOACK = 165848,
        INTRO_NARRATOR = 163922,
        TWISTER_DNT = 178617,
        CONJURE_SUN_ORB_DNT = 178618,
        WIELD_CHAKRAMS = 173168,
        WIELD_CHAKRAMS_2 = 170378,
        ENERGIZE_GLOWY_ORBS_COVER_DNT = 178324, // Visual to do when closed to sun orbs.
        ENERGIZE_GLOWY_ORBS_DNT_1 = 178321,
        ENERGIZE_GLOWY_ORBS_DNT_2 = 178330,
        EJECT_ALL_PASSENGERS = 50630,
        SERENE = 153716,
        OVERSEER_1 = 153195,
        OVERSEER_2 = 154368,
        EJECT_PASSENGER_1 = 60603,
        JUMP_TO_JUMP_POINT = 163828,
        // Skyreach Taln mobs.
        FIXATED = 152838,
        // Skyreach raven whisperer
        EXCITE = 153923,
    };

    namespace InstanceSkyreach
    {
        static Unit* SelectNearestFriendExcluededMe(Unit* p_Me, float p_Range = 0.0f, bool p_CheckLoS = true)
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

        static Player* SelectRandomPlayerExcludedTank(Creature* p_me, float p_range = 0.0f, bool p_checkLoS = true)
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
                if ((p_me->IsWithinLOSInMap(i->getSource()) || !p_checkLoS) && p_me->getVictim() != i->getSource() &&
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


        static Player* SelectFarEnoughPlayerIncludedTank(Creature* p_me, float p_range = 0.0f, bool p_checkLoS = true)
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
                if ((p_me->IsWithinLOSInMap(i->getSource()) || !p_checkLoS)
                    && !p_me->IsWithinDistInMap(i->getSource(), p_range)
                    && i->getSource()->isAlive())
                    return i->getSource();
            }

            return nullptr;
        }

        static Player* SelectRandomPlayerIncludedTank(Creature* p_me, float p_range = 0.0f, bool p_checkLoS = true)
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
                if ((p_me->IsWithinLOSInMap(i->getSource()) || !p_checkLoS)
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

#endif // !SKYREACH_INSTANCE_H