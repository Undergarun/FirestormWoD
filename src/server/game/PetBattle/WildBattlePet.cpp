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

#include "WildBattlePet.h"
#include "DatabaseEnv.h"
#include "Creature.h"
#include "PetBattle.h"
#include "Map.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "DB2Stores.h"
#include <stdexcept>
#include <algorithm>

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void WildBattlePetZonePools::LoadPoolTemplate(Field* l_Fields)
{
    WildBattlePetPoolTemplate l_PoolTemplate;
    l_PoolTemplate.Species      = l_Fields[1].GetUInt32();
    l_PoolTemplate.Replace      = l_Fields[2].GetUInt32();
    l_PoolTemplate.Max          = l_Fields[3].GetUInt32();
    l_PoolTemplate.RespawnTime  = l_Fields[4].GetUInt32();
    l_PoolTemplate.MinLevel     = l_Fields[5].GetUInt32();
    l_PoolTemplate.MaxLevel     = l_Fields[6].GetUInt32();

    for (size_t l_I = 0; l_I < 10; ++l_I)
        l_PoolTemplate.Breeds[l_I] = l_Fields[7 + l_I].GetUInt32();

    l_PoolTemplate.Entry = 0;

    if (BattlePetSpeciesEntry const* l_SpeciesInfo = sBattlePetSpeciesStore.LookupEntry(l_PoolTemplate.Species))
        l_PoolTemplate.Entry = l_SpeciesInfo->entry;

    if (!l_PoolTemplate.Entry)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, "No npc entry for species %u", l_PoolTemplate.Species);
        return;
    }

    m_Templates.push_back(l_PoolTemplate);
}

//////////////////////////////////////////////////////////////////////////

void WildBattlePetZonePools::Populate()
{
    for (size_t l_I = 0; l_I < m_Templates.size(); l_I++)
    {
        WildBattlePetPoolTemplate* l_Template = &m_Templates[l_I];

        if (l_Template->Max <= l_Template->Replaced.size())
            continue;

        if (BattlePetSpeciesEntry const* l_Entry = sBattlePetSpeciesStore.LookupEntry(l_Template->Species))
        {
            if ((l_Entry->flags & BATTLEPET_SPECIES_FLAG_UNTAMEABLE) != 0)
                continue;
        }

        uint32 l_ToReplaceCount = l_Template->Max - l_Template->Replaced.size();

        std::vector<Creature*> l_AvailableForReplacement;

        for (std::list<uint64>::iterator l_It = l_Template->ToBeReplaced.begin(); l_It != l_Template->ToBeReplaced.end(); l_It++)
        {
            if (l_Template->ReplacedRelation.find((*l_It)) == l_Template->ReplacedRelation.end())
            {
                Unit * l_Unit = sObjectAccessor->FindUnit((*l_It));

                if (l_Unit && l_Unit->ToCreature())
                    l_AvailableForReplacement.push_back(l_Unit->ToCreature());
            }
        }
        
        if (!l_AvailableForReplacement.size())
            continue;

        std::random_shuffle(l_AvailableForReplacement.begin(), l_AvailableForReplacement.end());

        for (size_t l_Y = 0; l_Y < l_AvailableForReplacement.size() && l_Y < l_ToReplaceCount; l_Y++)
            ReplaceCreature(l_AvailableForReplacement[l_Y], l_Template);
    }
}
void WildBattlePetZonePools::Depopulate()
{
    for (size_t l_I = 0; l_I < m_Templates.size(); l_I++)
    {
        WildBattlePetPoolTemplate* l_Template = &m_Templates[l_I];

        std::map<uint64, uint64> l_Creatures = l_Template->ReplacedRelation;

        for (std::map<uint64, uint64>::iterator l_It = l_Creatures.begin(); l_It != l_Creatures.end(); l_It++)
        {
            Unit * l_Unit = sObjectAccessor->FindUnit((*l_It).first);

            if (l_Unit && l_Unit->ToCreature())
                UnreplaceCreature(l_Unit->ToCreature(), l_Template);
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void WildBattlePetZonePools::OnAddToMap(Creature* p_Creature)
{
    if (!p_Creature)
        return;

    for (size_t l_I = 0; l_I < m_Templates.size(); l_I++)
    {
        WildBattlePetPoolTemplate* l_Template = &m_Templates[l_I];

        if (l_Template->Replace == p_Creature->GetEntry() && std::find(l_Template->ToBeReplaced.begin(), l_Template->ToBeReplaced.end(), p_Creature->GetGUID()) == l_Template->ToBeReplaced.end()
            && l_Template->ReplacedBattlePetInstances.find(p_Creature->GetGUID()) == l_Template->ReplacedBattlePetInstances.end())
        {
            l_Template->ToBeReplaced.push_back(p_Creature->GetGUID());
        }
    }
}
void WildBattlePetZonePools::OnRemoveToMap(Creature* p_Creature)
{
    if (!p_Creature)
        return;

    for (size_t l_I = 0; l_I < m_Templates.size(); l_I++)
    {
        WildBattlePetPoolTemplate* l_Template = &m_Templates[l_I];

        if (l_Template->Replace == p_Creature->GetEntry())
            l_Template->ToBeReplaced.remove(p_Creature->GetGUID());
    }
}

//////////////////////////////////////////////////////////////////////////

void WildBattlePetZonePools::ReplaceCreature(Creature* p_Creature, WildBattlePetPoolTemplate* p_Template)
{
    if (!p_Creature->FindMap())
        return;

    BattlePetSpeciesEntry const* l_SpeciesInfo = sBattlePetSpeciesStore.LookupEntry(p_Template->Species);
    if (!l_SpeciesInfo)
        return;

    Creature* l_ReplacementCreature = new Creature();
    l_ReplacementCreature->m_isTempWorldObject = true;

    if (!l_ReplacementCreature->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_UNIT), p_Creature->GetMap(), p_Creature->GetPhaseMask(), l_SpeciesInfo->entry, 0, 0,
        p_Creature->m_positionX, p_Creature->m_positionY, p_Creature->m_positionZ, p_Creature->m_orientation))
    {
        delete l_ReplacementCreature;
        return;
    }

    l_ReplacementCreature->SetHomePosition(*l_ReplacementCreature);

    // BattlePet fill data
    p_Template->ReplacedBattlePetInstances[l_ReplacementCreature->GetGUID()] = std::shared_ptr<BattlePetInstance>(new BattlePetInstance());
    std::shared_ptr<BattlePetInstance> l_BattlePetInstance = p_Template->ReplacedBattlePetInstances[l_ReplacementCreature->GetGUID()];

    l_BattlePetInstance->JournalID      = 0;
    l_BattlePetInstance->Slot           = 0;
    l_BattlePetInstance->NameTimeStamp  = 0;
    l_BattlePetInstance->Species        = l_SpeciesInfo->id;
    l_BattlePetInstance->DisplayModelID = l_ReplacementCreature->GetDisplayId();
    l_BattlePetInstance->XP             = 0;
    l_BattlePetInstance->Flags          = 0;
    l_BattlePetInstance->Health         = 20000;

    // Select level
    l_BattlePetInstance->Level = std::max(urand(p_Template->MinLevel, p_Template->MaxLevel), (uint32)1);

    // Select breed
    static const uint32 breedQualityWeights[4] = { 100, 50, 20, 8 };
    static const uint32 breedQuality[23] =
    { 0, 0, 0, 0, 3, 3, 3, 2, 2, 2, 1, 1,
      1, 0, 3, 3, 3, 2, 2, 2, 1, 1, 1 };

    uint32 totalBreedWeight = 0;
    for (size_t i = 0; i < 10; ++i)
        if (p_Template->Breeds[i] < 23)
            totalBreedWeight += breedQualityWeights[breedQuality[p_Template->Breeds[i]]];

    l_BattlePetInstance->Breed = 3;
    uint32 minBreedChances = l_BattlePetInstance->Level * totalBreedWeight * 2 / 75;
    uint32 breedChances = urand(1, totalBreedWeight);
    for (size_t i = 0; i < 10; ++i)
    {
        if (p_Template->Breeds[i] >= 23)
            continue;

        int32 weight = breedQualityWeights[breedQuality[p_Template->Breeds[i]]];
        if ((int32)breedChances <= weight)
        {
            l_BattlePetInstance->Breed = p_Template->Breeds[i];
            break;
        }

        breedChances -= weight;
    }

    l_BattlePetInstance->Quality = breedQuality[l_BattlePetInstance->Breed];

    // Select abilities
    uint32 l_AbilityLevels[3];
    memset(l_AbilityLevels, 0, sizeof(l_AbilityLevels));
    memset(l_BattlePetInstance->Abilities, 0, sizeof(l_BattlePetInstance->Abilities));

    for (uint32 l_SpeciesXAbilityId = 0; l_SpeciesXAbilityId < sBattlePetSpeciesXAbilityStore.GetNumRows(); ++l_SpeciesXAbilityId)
    {
        BattlePetSpeciesXAbilityEntry const* l_SpeciesXAbilityInfo = sBattlePetSpeciesXAbilityStore.LookupEntry(l_SpeciesXAbilityId);
        if (!l_SpeciesXAbilityInfo || l_SpeciesXAbilityInfo->speciesId != l_BattlePetInstance->Species || l_SpeciesXAbilityInfo->level > l_BattlePetInstance->Level)
            continue;

        if (l_AbilityLevels[l_SpeciesXAbilityInfo->tier])
        {
            int l_Chance = 80;

            if (l_AbilityLevels[l_SpeciesXAbilityInfo->tier] < l_SpeciesXAbilityInfo->level)
                l_Chance = 100 - l_Chance;

            if (rand() % 100 > l_Chance)
                continue;
        }

        l_BattlePetInstance->Abilities[l_SpeciesXAbilityInfo->tier] = l_SpeciesXAbilityInfo->abilityId;
        l_AbilityLevels[l_SpeciesXAbilityInfo->tier]                = l_SpeciesXAbilityInfo->level;
    }

    // Set creature flags
    l_ReplacementCreature->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_PETBATTLE);
    l_ReplacementCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
    l_ReplacementCreature->SetUInt32Value(UNIT_FIELD_WILD_BATTLE_PET_LEVEL, l_BattlePetInstance->Level);
    l_ReplacementCreature->SetRespawnRadius(3.5f);
    l_ReplacementCreature->SetDefaultMovementType(RANDOM_MOTION_TYPE);

    if (!p_Creature->GetMap()->AddToMap(l_ReplacementCreature))
    {
        delete l_ReplacementCreature;
        return;
    }

    // Despawn replaced creature
    p_Creature->ForcedDespawn();
    p_Creature->SetRespawnTime(MONTH);
    p_Creature->RemoveCorpse(false);

    p_Template->ReplacedRelation[p_Creature->GetGUID()] = l_ReplacementCreature->GetGUID();
    p_Template->Replaced.push_back(l_ReplacementCreature->GetGUID());
}
void WildBattlePetZonePools::UnreplaceCreature(Creature* p_Creature, WildBattlePetPoolTemplate* p_Template)
{
    if (!p_Creature || p_Template->ReplacedRelation.find(p_Creature->GetGUID()) == p_Template->ReplacedRelation.end())
        return;

    Unit * l_Unit = sObjectAccessor->FindUnit(p_Template->ReplacedRelation[p_Creature->GetGUID()]);

    if (!l_Unit || !l_Unit->ToCreature())
        return;

    Creature* l_ReplacementCreature = l_Unit->ToCreature();

    if (p_Template->ReplacedBattlePetInstances.find(l_ReplacementCreature->GetGUID()) != p_Template->ReplacedBattlePetInstances.end())
        p_Template->ReplacedBattlePetInstances.erase(p_Template->ReplacedBattlePetInstances.find(l_ReplacementCreature->GetGUID()));

    p_Template->Replaced.remove(l_ReplacementCreature->GetGUID());

    l_ReplacementCreature->RemoveFromWorld();
    l_ReplacementCreature->AddObjectToRemoveList();

    p_Creature->SetRespawnTime(p_Creature->GetCreatureData() ? p_Creature->GetCreatureData()->spawntimesecs : 15);
    p_Creature->Respawn();

    p_Template->ReplacedRelation.erase(p_Template->ReplacedRelation.find(p_Creature->GetGUID()));
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

WildBattlePetMgr::WildBattlePetMgr()
{
    m_UpdateTime.SetInterval(WILDBATTLEPETMGR_UPDATE_INTERVAL);
}

//////////////////////////////////////////////////////////////////////////

void WildBattlePetMgr::Load()
{
    m_PoolsByMap.clear();

    QueryResult l_Result = WorldDatabase.Query("SELECT Zone, Species, `Replace`, `Max`, RespawnTime, MinLevel, MaxLevel, Breed0, Breed1, Breed2, Breed3, Breed4, Breed5, Breed6, Breed7, Breed8, Breed9 FROM wild_battlepet_zone_pool");
    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 species definitions. DB table `WildBattlePetZoneSpecies` is empty");
        return;
    }

    uint32 l_Count = 0;
    do
    {
        Field* l_Fields = l_Result->Fetch();

        uint32 l_ZoneID = l_Fields[0].GetUInt32();
        int32 l_MapID  = 0;

        for (uint32 l_I = 0; l_I < sAreaStore.GetNumRows(); l_I++)
        {
            AreaTableEntry const* l_AreaInfo = sAreaStore.LookupEntry(l_I);
            if (l_AreaInfo && l_AreaInfo->ID == l_ZoneID)
            {
                l_MapID = l_AreaInfo->ContinentID;
                break;
            }
        }

        if (l_MapID == -1)
        {
            sLog->outError(LOG_FILTER_SERVER_LOADING, "WildBattlePetMgr::Load() no map id found for zone %u", l_ZoneID);
            continue;
        }

        bool l_Error = false;
        for (size_t l_I = 0; l_I < m_PoolsByMap[l_MapID][l_ZoneID].m_Templates.size(); l_I++)
        {
            if (m_PoolsByMap[l_MapID][l_ZoneID].m_Templates[l_I].Replace == l_Fields[2].GetUInt32())
            {
                l_Error = true;
                sLog->outError(LOG_FILTER_SERVER_LOADING, "WildBattlePetMgr::Load() zone %u already contains a replacement for creature entry %u", l_ZoneID, l_Fields[2].GetUInt32());

                break;
            }
        }

        if (l_Error)
            continue;

        m_PoolsByMap[l_MapID][l_ZoneID].LoadPoolTemplate(l_Fields);
        m_PoolsByMap[l_MapID][l_ZoneID].MapID   = l_MapID;
        m_PoolsByMap[l_MapID][l_ZoneID].ZoneID  = l_ZoneID;

        ++l_Count;
    }
    while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u species definitions.", l_Count);
}

//////////////////////////////////////////////////////////////////////////

void WildBattlePetMgr::PopulateAll()
{
    for (std::map<uint32, std::map<uint32, WildBattlePetZonePools> >::iterator l_It = m_PoolsByMap.begin(); l_It != m_PoolsByMap.end(); l_It++)
        for (std::map<uint32, WildBattlePetZonePools>::iterator l_It2 = l_It->second.begin(); l_It2 != l_It->second.end(); l_It2++)
            (*l_It2).second.Populate();
}
void WildBattlePetMgr::PopulateMap(uint32 p_MapID)
{
    if (m_PoolsByMap.find(p_MapID) == m_PoolsByMap.end())
        return;

    for (std::map<uint32, WildBattlePetZonePools>::iterator l_It = m_PoolsByMap[p_MapID].begin(); l_It != m_PoolsByMap[p_MapID].end(); l_It++)
        (*l_It).second.Populate();
}
void WildBattlePetMgr::DepopulateMap(uint32 p_MapID)
{
    if (m_PoolsByMap.find(p_MapID) == m_PoolsByMap.end())
        return;

    for (std::map<uint32, WildBattlePetZonePools>::iterator l_It = m_PoolsByMap[p_MapID].begin(); l_It != m_PoolsByMap[p_MapID].end(); l_It++)
        (*l_It).second.Depopulate();
}

//////////////////////////////////////////////////////////////////////////

void WildBattlePetMgr::OnAddToMap(Creature* p_Creature)
{
    if (!p_Creature)
        return;

    uint32 l_ZoneID = p_Creature->GetZoneId();
    uint32 l_MapID  = p_Creature->GetMapId();

    if (m_PoolsByMap.find(l_MapID) == m_PoolsByMap.end())
        return;

    if (m_PoolsByMap[l_MapID].find(l_ZoneID) == m_PoolsByMap[l_MapID].end())
        return;

    m_PoolsByMap[l_MapID][l_ZoneID].OnAddToMap(p_Creature);
}
void WildBattlePetMgr::OnRemoveToMap(Creature* p_Creature)
{
    if (!p_Creature)
        return;

    uint32 l_ZoneID = p_Creature->GetZoneId();
    uint32 l_MapID  = p_Creature->GetMapId();

    if (m_PoolsByMap.find(l_MapID) == m_PoolsByMap.end())
        return;

    if (m_PoolsByMap[l_MapID].find(l_ZoneID) == m_PoolsByMap[l_MapID].end())
        return;

    m_PoolsByMap[l_MapID][l_ZoneID].OnRemoveToMap(p_Creature);
}

//////////////////////////////////////////////////////////////////////////

bool WildBattlePetMgr::IsWildPet(Creature* p_Creature)
{
    if (!p_Creature)
        return false;

    uint32 l_ZoneID = p_Creature->GetZoneId();
    uint32 l_MapID  = p_Creature->GetMapId();

    if (m_PoolsByMap.find(l_MapID) == m_PoolsByMap.end())
        return false;

    if (m_PoolsByMap[l_MapID].find(l_ZoneID) == m_PoolsByMap[l_MapID].end())
        return false;

    WildBattlePetZonePools* l_Pools = &m_PoolsByMap[l_MapID][l_ZoneID];

    for (size_t l_I = 0; l_I < l_Pools->m_Templates.size(); l_I++)
    {
        if (l_Pools->m_Templates[l_I].ReplacedBattlePetInstances.find(p_Creature->GetGUID()) != l_Pools->m_Templates[l_I].ReplacedBattlePetInstances.end())
            return true;
    }

    return false;
}
std::shared_ptr<BattlePetInstance> WildBattlePetMgr::GetWildBattlePet(Creature* p_Creature)
{
    if (!IsWildPet(p_Creature) || !p_Creature)
        return NULL;

    uint32 l_ZoneID = p_Creature->GetZoneId();
    uint32 l_MapID  = p_Creature->GetMapId();

    WildBattlePetZonePools* l_Pools = &m_PoolsByMap[l_MapID][l_ZoneID];

    for (size_t l_I = 0; l_I < l_Pools->m_Templates.size(); l_I++)
    {
        if (l_Pools->m_Templates[l_I].ReplacedBattlePetInstances.find(p_Creature->GetGUID()) != l_Pools->m_Templates[l_I].ReplacedBattlePetInstances.end())
            return BattlePetInstance::CloneForBattle(l_Pools->m_Templates[l_I].ReplacedBattlePetInstances[p_Creature->GetGUID()]);
    }

    return NULL;
}

//////////////////////////////////////////////////////////////////////////

void WildBattlePetMgr::EnterInBattle(Creature* p_Creature)
{
    if (!IsWildPet(p_Creature))
        return;

    p_Creature->ForcedDespawn();
    p_Creature->SetRespawnTime(MONTH);
    p_Creature->RemoveCorpse(false);
}
void WildBattlePetMgr::LeaveBattle(Creature* p_Creature, bool p_Defeated)
{
    if (!IsWildPet(p_Creature))
        return;

    uint32 l_ZoneID = p_Creature->GetZoneId();
    uint32 l_MapID  = p_Creature->GetMapId();

    WildBattlePetZonePools* l_Pools = &m_PoolsByMap[l_MapID][l_ZoneID];

    Creature* l_ReplacedCreature = NULL;

    for (size_t l_I = 0; l_I < l_Pools->m_Templates.size(); l_I++)
    {
        WildBattlePetPoolTemplate* l_Template = &l_Pools->m_Templates[l_I];

        for (std::map<uint64, uint64>::iterator l_It = l_Template->ReplacedRelation.begin(); l_It != l_Template->ReplacedRelation.end(); ++l_It)
        {
            if (l_It->second == p_Creature->GetGUID())
            {
                Unit * l_Unit = sObjectAccessor->FindUnit(l_It->first);

                if (!l_Unit || !l_Unit->ToCreature())
                    continue;

                l_Pools->UnreplaceCreature(l_Unit->ToCreature(), l_Template);

                return;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void WildBattlePetMgr::Update(uint32 p_TimeDiff)
{
    m_UpdateTime.Update(p_TimeDiff);

    if (m_UpdateTime.Passed())
    {
        m_UpdateTime.Reset();

        PopulateAll();
    }
}
