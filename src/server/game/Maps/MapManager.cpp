////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "MapManager.h"
#include "InstanceSaveMgr.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "Transport.h"
#include "GridDefines.h"
#include "MapInstanced.h"
#include "InstanceScript.h"
#include "Config.h"
#include "World.h"
#include "CellImpl.h"
#include "Corpse.h"
#include "ObjectMgr.h"
#include "Language.h"
#include "WorldPacket.h"
#include "Group.h"
#include "Common.h"

extern GridState* si_GridStates[];                          // debugging code, should be deleted some day

MapManager::MapManager()
{
    i_gridCleanUpDelay = sWorld->getIntConfig(CONFIG_INTERVAL_GRIDCLEAN);
    i_timer.SetInterval(sWorld->getIntConfig(CONFIG_INTERVAL_MAPUPDATE));
}

MapManager::~MapManager()
{
}

void MapManager::Initialize()
{
    Map::InitStateMachine();

    // debugging code, should be deleted some day
    {
        for (uint8 i = 0; i < MAX_GRID_STATE; ++i)
             i_GridStates[i] = si_GridStates[i];

        i_GridStateErrorCount = 0;
    }
    int num_threads(sWorld->getIntConfig(CONFIG_NUMTHREADS));
    // Start mtmaps if needed.
    if (num_threads > 0)
        m_updater.activate(num_threads);
}

void MapManager::InitializeVisibilityDistanceInfo()
{
    for (MapMapType::iterator iter=i_maps.begin(); iter != i_maps.end(); ++iter)
        (*iter).second->InitVisibilityDistance();
}

// debugging code, should be deleted some day
void MapManager::checkAndCorrectGridStatesArray()
{
    bool ok = true;
    for (int i=0; i<MAX_GRID_STATE; i++)
    {
        if (i_GridStates[i] != si_GridStates[i])
        {
            sLog->outError(LOG_FILTER_MAPS, "MapManager::checkGridStates(), GridState: si_GridStates is currupt !!!");
            ok = false;
            si_GridStates[i] = i_GridStates[i];
        }
        #ifdef TRINITY_DEBUG
        // inner class checking only when compiled with debug
        if (!si_GridStates[i]->checkMagic())
        {
            ok = false;
            si_GridStates[i]->setMagic();
        }
        #endif
    }
    if (!ok)
        ++i_GridStateErrorCount;
}

Map* MapManager::CreateBaseMap(uint32 id)
{
    Map* map = FindBaseMap(id);

    if (map == NULL)
    {
        TRINITY_GUARD(ACE_Thread_Mutex, Lock);

        MapEntry const* entry = sMapStore.LookupEntry(id);
        ASSERT(entry);

        if (entry->Instanceable())
            map = new MapInstanced(id, i_gridCleanUpDelay);
        else
        {
            map = new Map(id, i_gridCleanUpDelay, 0, DifficultyNone);
            map->LoadRespawnTimes();
        }

        i_maps[id] = map;
    }

    ASSERT(map);
    return map;
}

Map* MapManager::FindBaseNonInstanceMap(uint32 mapId) const
{
    Map* map = FindBaseMap(mapId);
    if (map && map->Instanceable())
        return NULL;
    return map;
}

Map* MapManager::CreateMap(uint32 id, Player* player)
{
    Map* m = CreateBaseMap(id);

    if (m && m->Instanceable())
        m = ((MapInstanced*)m)->CreateInstanceForPlayer(id, player);

    return m;
}

Map* MapManager::FindMap(uint32 mapid, uint32 instanceId) const
{
    Map* map = FindBaseMap(mapid);
    if (!map)
        return NULL;

    if (!map->Instanceable())
        return instanceId == 0 ? map : NULL;

    return ((MapInstanced*)map)->FindInstanceMap(instanceId);
}

bool MapManager::CanPlayerEnter(uint32 mapid, Player* player, bool loginCheck)
{
    MapEntry const* entry = sMapStore.LookupEntry(mapid);
    if (!entry)
       return false;

    // Molten Front
    // That's not an instance, so check it here
    if (entry->MapID == 861)
        if (player->getLevel() < 85)
            return false;

    if (!entry->IsDungeon())
        return true;

    InstanceTemplate const* instance = sObjectMgr->GetInstanceTemplate(mapid);
    if (!instance)
        return false;

    Difficulty targetDifficulty = player->GetDifficultyID(entry);

#ifndef CROSS
    if (entry->MapID == player->GetGarrisonMapID() || entry->MapID == player->GetShipyardMapID())
        targetDifficulty = Difficulty::DifficultyNormal;
#endif

    //The player has a heroic mode and tries to enter into instance which has no a heroic mode
    MapDifficulty const* mapDiff = GetMapDifficultyData(entry->MapID, targetDifficulty);
    if (!mapDiff)
    {
        // Send aborted message for dungeons
        if (entry->IsNonRaidDungeon())
        {
            player->SendTransferAborted(mapid, TRANSFER_ABORT_DIFFICULTY, player->GetDungeonDifficultyID());
            return false;
        }
        else    // attempt to downscale
            mapDiff = GetDownscaledMapDifficultyData(entry->MapID, targetDifficulty); ///< mapDiff is never read 01/18/16
    }

    //Bypass checks for GMs
    if (player->isGameMaster())
        return true;

    char const* mapName = entry->MapNameLang;

    if (!player->isAlive())
    {
        if (Corpse* corpse = player->GetCorpse())
        {
            // let enter in ghost mode in instance that connected to inner instance with corpse
            uint32 corpseMap = corpse->GetMapId();
            do
            {
                if (corpseMap == mapid)
                    break;

                InstanceTemplate const* corpseInstance = sObjectMgr->GetInstanceTemplate(corpseMap);
                corpseMap = corpseInstance ? corpseInstance->Parent : 0;
            }
            while (corpseMap);

            if (!corpseMap)
            {
                WorldPacket l_Data(SMSG_AREA_TRIGGER_NO_CORPSE);
                player->GetSession()->SendPacket(&l_Data);
                sLog->outDebug(LOG_FILTER_MAPS, "MAP: Player '%s' does not have a corpse in instance '%s' and cannot enter.", player->GetName(), mapName);
                return false;
            }

            sLog->outDebug(LOG_FILTER_MAPS, "MAP: Player '%s' has corpse in instance '%s' and can enter.", player->GetName(), mapName);
            player->ResurrectPlayer(0.5f, false);
            player->SpawnCorpseBones();
        }
        else
            sLog->outDebug(LOG_FILTER_MAPS, "Map::CanPlayerEnter - player '%s' is dead but does not have a corpse!", player->GetName());
    }

    Group* group = player->GetGroup();
    if (entry->IsRaid() && entry->Expansion() >= 4)
    {
        // can only enter in a raid group except for raids before Mists of Pandaria
        if ((!group || !group->isRaidGroup()) && !sWorld->getBoolConfig(CONFIG_INSTANCE_IGNORE_RAID))
        {
            sLog->outDebug(LOG_FILTER_MAPS, "MAP: Player '%s' must be in a raid group to enter instance '%s'", player->GetName(), mapName);
            return false;
        }
    }

    //Get instance where player's group is bound & its map
    if (group)
    {
        InstanceGroupBind* boundInstance = group->GetBoundInstance(entry);
        if (boundInstance && boundInstance->save)
            if (Map* boundMap = sMapMgr->FindMap(mapid, boundInstance->save->GetInstanceId()))
                if (!loginCheck && !boundMap->CanEnter(player))
                    return false;
    }

    // players are only allowed to enter 5 instances per hour
    if (entry->IsDungeon() && (!player->GetGroup() || (player->GetGroup() && !player->GetGroup()->isLFGGroup())))
    {
        uint32 instaceIdToCheck = 0;
        if (InstanceSave* save = player->GetInstanceSave(mapid))
            instaceIdToCheck = save->GetInstanceId();

        // instanceId can never be 0 - will not be found
        if (!player->CheckInstanceCount(instaceIdToCheck) && !player->isDead())
        {
            player->SendTransferAborted(mapid, TRANSFER_ABORT_TOO_MANY_INSTANCES);
            return false;
        }
    }

    //Other requirements
    return player->Satisfy(sObjectMgr->GetAccessRequirement(mapid, targetDifficulty), mapid, true);
}

void MapManager::Update(uint32 diff)
{
    i_timer.Update(diff);
    if (!i_timer.Passed())
        return;

    m_MapsDelay.clear();

    /// - Start Achievement criteria update processing thread
    sAchievementMgr->PrepareCriteriaUpdateTaskThread();

    for (auto l_PlayerTask : sAchievementMgr->GetPlayersCriteriaTask())
    {
        if (m_updater.activated())
            m_updater.schedule_specific(new AchievementCriteriaUpdateRequest(&m_updater, l_PlayerTask.second));
        else
        {
            /// Process all task in synchrone way
            auto l_Task = new AchievementCriteriaUpdateRequest(nullptr, l_PlayerTask.second);
            l_Task->call();
            delete l_Task;
        }
    }

    /// - Start map updater threads
    MapMapType::iterator iter = i_maps.begin();
    for (; iter != i_maps.end(); ++iter)
    {
        if (m_updater.activated())
            m_updater.schedule_update(*iter->second, uint32(i_timer.GetCurrent()));
        else
            iter->second->Update(uint32(i_timer.GetCurrent()));
    }

    if (m_updater.activated())
        m_updater.wait();

    sAchievementMgr->ClearPlayersCriteriaTask();

    for (iter = i_maps.begin(); iter != i_maps.end(); ++iter)
        iter->second->DelayedUpdate(uint32(i_timer.GetCurrent()));

    sObjectAccessor->Update(uint32(i_timer.GetCurrent()));

    std::queue<std::function<bool()>> l_Operations;
    m_CriticalOperationLock.acquire();

    l_Operations = m_CriticalOperation;
    
    while (!m_CriticalOperation.empty())
        m_CriticalOperation.pop();

    m_CriticalOperationLock.release();

    std::queue<std::function<bool()>> l_CriticalOperationFallBack;
    while (!l_Operations.empty())
    {
        if (l_Operations.front())
        {
            if (!(l_Operations.front()()))
                l_CriticalOperationFallBack.push(l_Operations.front());
        }

        l_Operations.pop();
    }

    if (!l_CriticalOperationFallBack.empty())
    {
        m_CriticalOperationLock.acquire();
        while (!l_CriticalOperationFallBack.empty())
        {
            m_CriticalOperation.push(l_CriticalOperationFallBack.front());
            l_CriticalOperationFallBack.pop();
        }
        m_CriticalOperationLock.release();
    }

    i_timer.SetCurrent(0);
}

void MapManager::DoDelayedMovesAndRemoves()
{
}

bool MapManager::ExistMapAndVMap(uint32 mapid, float x, float y)
{
    GridCoord p = JadeCore::ComputeGridCoord(x, y);

    int gx=63-p.x_coord;
    int gy=63-p.y_coord;

    return Map::ExistMap(mapid, gx, gy) && Map::ExistVMap(mapid, gx, gy);
}

bool MapManager::IsValidMAP(uint32 mapid, bool startUp)
{
    MapEntry const* mEntry = sMapStore.LookupEntry(mapid);

    if (startUp)
        return mEntry ? true : false;
    else
        return mEntry && (!mEntry->IsDungeon() || sObjectMgr->GetInstanceTemplate(mapid));

    // TODO: add check for battleground template
}

void MapManager::UnloadAll()
{
    for (MapMapType::iterator iter = i_maps.begin(); iter != i_maps.end();)
    {
        iter->second->UnloadAll();
        delete iter->second;
        i_maps.erase(iter++);
    }

    if (m_updater.activated())
        m_updater.deactivate();

    Map::DeleteStateMachine();
}

uint32 MapManager::GetNumInstances()
{
    TRINITY_GUARD(ACE_Thread_Mutex, Lock);

    uint32 ret = 0;
    for (MapMapType::iterator itr = i_maps.begin(); itr != i_maps.end(); ++itr)
    {
        Map* map = itr->second;
        if (!map->Instanceable())
            continue;
        MapInstanced::InstancedMaps &maps = ((MapInstanced*)map)->GetInstancedMaps();
        for (MapInstanced::InstancedMaps::iterator mitr = maps.begin(); mitr != maps.end(); ++mitr)
            if (mitr->second->IsDungeon()) ret++;
    }
    return ret;
}

uint32 MapManager::GetNumPlayersInInstances()
{
    TRINITY_GUARD(ACE_Thread_Mutex, Lock);

    uint32 ret = 0;
    for (MapMapType::iterator itr = i_maps.begin(); itr != i_maps.end(); ++itr)
    {
        Map* map = itr->second;
        if (!map->Instanceable())
            continue;
        MapInstanced::InstancedMaps &maps = ((MapInstanced*)map)->GetInstancedMaps();
        for (MapInstanced::InstancedMaps::iterator mitr = maps.begin(); mitr != maps.end(); ++mitr)
            if (mitr->second->IsDungeon())
                ret += ((InstanceMap*)mitr->second)->GetPlayers().getSize();
    }
    return ret;
}

void MapManager::InitInstanceIds()
{
    m_NextInstanceID = 1;
}

void MapManager::RegisterInstanceId(uint32 p_InstanceID)
{
    /// Allocation and sizing was done in InitInstanceIds()
    m_InstanceIDs.insert(p_InstanceID);
}

uint32 MapManager::GenerateInstanceId()
{
    uint32 l_NewInstanceID = m_NextInstanceID;

    /// Find the lowest available id starting from the current NextInstanceId (which should be the lowest according to the logic in FreeInstanceId()
    for (uint32 l_I = ++m_NextInstanceID; l_I < 0xFFFFFFFF; ++l_I)
    {
        if (m_InstanceIDs.find(l_I) != m_InstanceIDs.end())
            continue;

        m_NextInstanceID = l_I;
        break;
    }

    if (l_NewInstanceID == m_NextInstanceID)
    {
        sLog->outError(LogFilterType::LOG_FILTER_MAPS, "Instance ID overflow!! Can't continue, shutting down server. ");
        World::StopNow(ShutdownExitCode::ERROR_EXIT_CODE);
    }

    m_InstanceIDs.insert(l_NewInstanceID);

    return l_NewInstanceID;
}

void MapManager::FreeInstanceId(uint32 p_InstanceID)
{
    /// If freed instance id is lower than the next id available for new instances, use the freed one instead
    if (p_InstanceID < m_NextInstanceID)
        SetNextInstanceId(p_InstanceID);

    m_InstanceIDs.erase(p_InstanceID);
}
