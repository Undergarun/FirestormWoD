/*
* Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com/>
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#ifndef _WILDBATTLEPET_HEADER_
#define _WILDBATTLEPET_HEADER_

#include "Common.h"
#include <ace/Singleton.h>
#include "Timer.h"
#include "PetBattle.h"

#define WILDBATTLEPETMGR_UPDATE_INTERVAL 6000
#define WILDBATTLEPET_RESPAWN_WHEN_NOT_DEFEATED 10

class Creature;

struct WildBattlePetPoolTemplate
{
    uint32 Species;
    uint32 Entry;
    uint32 Replace;
    uint32 Max;
    uint32 RespawnTime;
    uint32 MinLevel;
    uint32 MaxLevel;
    uint32 Breeds[10];

    std::list<uint64>            ToBeReplaced;
    std::list<uint64>            Replaced;

    /// @TODO: replace this
    std::map<uint64, uint64>                                ReplacedRelation;
    std::map<uint64, std::shared_ptr<BattlePetInstance>>    ReplacedBattlePetInstances;
};

class WildBattlePetZonePools
{
    public:
        void LoadPoolTemplate(Field* fields);

        void Populate();
        void Depopulate();

        void OnAddToMap(Creature* p_Creature);
        void OnRemoveToMap(Creature* p_Creature);

        void EnterInBattle(Creature* p_Creature);
        void LeaveBattle(Creature* p_Creature, bool p_Defeated);

        void ReplaceCreature(Creature* p_Creature, WildBattlePetPoolTemplate* p_Template);
        void UnreplaceCreature(Creature* p_Creature, WildBattlePetPoolTemplate* p_Template);

    public:
        uint32 ZoneID;
        uint32 MapID;

        std::vector<WildBattlePetPoolTemplate> m_Templates;
};

class WildBattlePetMgr
{
    public:
        WildBattlePetMgr();

        void Load();

        void PopulateAll();
        void PopulateMap(uint32 p_MapID);
        void DepopulateMap(uint32 p_MapID);

        void OnAddToMap(Creature* p_Creature);
        void OnRemoveToMap(Creature* p_Creature);

        bool IsWildPet(Creature* p_Creature);
        std::shared_ptr<BattlePetInstance> GetWildBattlePet(Creature* p_Creature);
        
        void EnterInBattle(Creature* p_Creature);
        void LeaveBattle(Creature* p_Creature, bool p_Defeated);

        void Update(uint32 p_TimeDiff);

    private:
        ///        map              zone          pools
        std::map<uint32, std::map<uint32, WildBattlePetZonePools>>  m_PoolsByMap;
        IntervalTimer                                               m_UpdateTime;
};

#define sWildBattlePetMgr ACE_Singleton<WildBattlePetMgr, ACE_Null_Mutex>::instance()

#endif
