////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TRINITY_MAP_INSTANCED_H
#define TRINITY_MAP_INSTANCED_H

#include "Map.h"
#include "InstanceSaveMgr.h"
#include "DBCEnums.h"

class MapInstanced : public Map
{
    friend class MapManager;
    public:
        typedef std::unordered_map< uint32, Map*> InstancedMaps;

        MapInstanced(uint32 id, time_t expiry);
        ~MapInstanced() {}

        // functions overwrite Map versions
        void Update(const uint32);
        void DelayedUpdate(const uint32 diff);
        //void RelocationNotify();
        void UnloadAll();
        bool CanEnter(Player* player);

        Map* CreateInstanceForPlayer(const uint32 mapId, Player* player);
        Map* FindInstanceMap(uint32 instanceId) const
        {
            InstancedMaps::const_iterator i = m_InstancedMaps.find(instanceId);
            return(i == m_InstancedMaps.end() ? NULL : i->second);
        }
        bool DestroyInstance(InstancedMaps::iterator &itr);

        void AddGridMapReference(const GridCoord &p)
        {
            ++GridMapReference[p.x_coord][p.y_coord];
            SetUnloadReferenceLock(GridCoord(63-p.x_coord, 63-p.y_coord), true);
        }

        void RemoveGridMapReference(GridCoord const& p)
        {
            --GridMapReference[p.x_coord][p.y_coord];
            if (!GridMapReference[p.x_coord][p.y_coord])
                SetUnloadReferenceLock(GridCoord(63-p.x_coord, 63-p.y_coord), false);
        }

        InstancedMaps &GetInstancedMaps() { return m_InstancedMaps; }
        virtual void InitVisibilityDistance();

    private:
        InstanceMap* CreateInstance(uint32 InstanceId, InstanceSave* save, Difficulty difficulty);
        BattlegroundMap* CreateBattleground(uint32 InstanceId, Battleground* bg);

        InstancedMaps m_InstancedMaps;

        uint16 GridMapReference[MAX_NUMBER_OF_GRIDS][MAX_NUMBER_OF_GRIDS];
};
#endif
