#ifndef SKYREACH_INSTANCE_H
# define SKYREACH_INSTANCE_H

#include "ScriptPCH.h"
#include "ScriptMgr.h"

namespace MS
{
    namespace InstanceSkyreach
    {
        enum RandomSpells
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
            SUBMERGED = 154163,
            SUBMERGE = 154164,
            ENERGIZE = 154139, // During 12 seconds, restart after 3 seconds.
            ENERGIZE_HEAL = 154149,
            ENERGIZE_DMG = 154150,
            ENERGIZE_VISUAL_1 = 154179,
            ENERGIZE_VISUAL_2 = 154159,
        };

        enum GameObjectEntries
        {
            DOOR_RANJIT_ENTRANCE        = 234311,
            DOOR_RANJIT_EXIT            = 234310,
            DOOR_ARAKNATH_ENTRANCE_1    = 234314,
            DOOR_ARAKNATH_ENTRANCE_2    = 234315,
            DOOR_ARAKNATH_EXIT_1        = 234312,
            DOOR_ARAKNATH_EXIT_2        = 234313,
            CACHE_OF_ARAKKOAN_TREASURES = 234164,
            DOOR_RUKHRAN_EXIT           = 234316,
            DOOR_RUKHRAN_ENTRANCE       = 229038,
        };

        enum BossEntries
        {
            RANJIT = 86238,
            ARAKNATH = 76141,
            RUKHRAN = 76143,
        };

        enum MobEntries
        {
            SKYREACH_ARCANALOGIST = 76376,
            SKYREACH_SOLAR_CONSTRUCTOR = 76142,
            YOUNG_KALIRI = 76121,
            SKYREACH_RAVEN_WHISPERER = 76154,
            SOLAR_FLARE = 76227,
            PILE_OF_ASHES = 79505
        };

        enum Data
        {
            Ranjit,
            Araknath,
            Rukhran,
            SkyreachArcanologist,
            SkyreachArcanologistIsDead,
            SkyreachArcanologistReset,
            AraknathSolarConstructorActivation,
            SkyreachRavenWhispererIsDead,
            SolarFlareDying,
        };

        static GameObject* SelectNearestGameObjectWithEntry(Unit* p_Me, uint32 p_Entry, float p_Range = 0.0f)
        {
            std::list<GameObject*> l_TargetList;

            JadeCore::NearestGameObjectEntryInObjectRangeCheck l_Check(*p_Me, p_Entry, p_Range);
            JadeCore::GameObjectListSearcher<JadeCore::NearestGameObjectEntryInObjectRangeCheck> l_Searcher(p_Me, l_TargetList, l_Check);
            p_Me->VisitNearbyObject(p_Range, l_Searcher);

            for (GameObject* l_Gob : l_TargetList)
                return l_Gob;

            return nullptr;
        }

        static Creature* SelectNearestCreatureWithEntry(Unit* p_Me, uint32 p_Entry, float p_Range = 0.0f)
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

        static Unit* SelectRandomCreatureWithEntry(Unit* p_Me, uint32 p_Entry, float p_Range = 0.0f)
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

            auto l_RandUnit = l_Results.begin();
            std::advance(l_RandUnit, urand(0, l_Size - 1));

            return *l_RandUnit;
        }

        static std::list<Unit*> SelectNearestCreatureListWithEntry(Unit* p_Me, uint32 p_Entry, float p_Range = 0.0f)
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

        static Player* SelectNearestPlayer(Unit* p_me, float p_range = 0.0f, bool p_checkLoS = true)
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
                    p_me->GetExactDist2d(i->getSource()) < p_range && i->getSource()->isAlive())
                    temp.push_back(i->getSource());
            }

            if (!temp.empty())
                return temp.front();

            return nullptr;
        }

        static void ApplyOnEveryPlayer(Unit* p_Me, std::function<void(Unit*, Player*)> p_Function)
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

        static Player* SelectRandomPlayerExcludedTank(Unit* p_me, float p_range = 0.0f, bool p_checkLoS = true)
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


        static Player* SelectFarEnoughPlayerIncludedTank(Unit* p_me, float p_range = 0.0f, bool p_checkLoS = true)
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

        static Player* SelectRandomPlayerIncludedTank(Unit* p_me, float p_range = 0.0f, bool p_checkLoS = true)
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