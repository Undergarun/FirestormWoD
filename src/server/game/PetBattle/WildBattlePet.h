#pragma once
#ifndef _WILDBATTLEPET_HEADER_
#define _WILDBATTLEPET_HEADER_

#include "Common.h"
#include <ace/Singleton.h>
#include "Timer.h"
#include "PetBattle.h"

#define WILDBATTLEPETMGR_UPDATE_INTERVAL 2000
#define WILDBATTLEPET_RESPAWN_WHEN_NOT_DEFEATED 10

class Creature;

#define BREED_SIZE 10

struct WildBattlePetPoolTemplate
{
    uint32 Species;
    uint32 Entry;
    uint32 Replace;
    uint32 Max;
    uint32 RespawnTime;
    uint32 MinLevel;
    uint32 MaxLevel;
    uint32 Breeds[BREED_SIZE];

    std::list<Creature*>            ToBeReplaced;
    std::list<Creature*>            Replaced;

    ///   tobereplaced replaced
    std::map<Creature*, Creature*>          ReplacedRelation;
    std::map<Creature*, BattlePetInstance>  ReplacedBattlePetInstances;
};

class WildBattlePetZonePools
{
    public:
        void LoadPoolTemplate(Field* fields);

        void Populate();
        void Depopulate();

        void OnAddToMap(Creature * p_Creature);
        void OnRemoveToMap(Creature * p_Creature);

        void EnterInBattle(Creature * p_Creature);
        void LeaveBattle(Creature * p_Creature, bool p_Defeated);

        void ReplaceCreature(Creature * p_Creature, WildBattlePetPoolTemplate * p_Template);
        void UnreplaceCreature(Creature * p_Creature, WildBattlePetPoolTemplate * p_Template);

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

        void OnAddToMap(Creature * p_Creature);
        void OnRemoveToMap(Creature * p_Creature);

        bool IsWildPet(Creature * p_Creature);
        BattlePetInstance * GetWildBattlePet(Creature * p_Creature);
        
        void EnterInBattle(Creature * p_Creature);
        void LeaveBattle(Creature * p_Creature, bool p_Defeated);

        void Update(uint32 p_TimeDiff);

    private:
        ///        map              zone          pools
        std::map<uint32, std::map<uint32, WildBattlePetZonePools> > m_PoolsByMap;
        IntervalTimer                                               m_UpdateTime;
};

#define sWildBattlePetMgr ACE_Singleton<WildBattlePetMgr, ACE_Null_Mutex>::instance()

#endif
