////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "GridStates.h"
#include "GridNotifiers.h"
#include "Grid.h"
#include "Log.h"

void InvalidState::Update(Map &, NGridType &, GridInfo &, const uint32) const
{
}

void ActiveState::Update(Map &m, NGridType &grid, GridInfo & info, const uint32 t_diff) const
{
    // Only check grid activity every (grid_expiry/10) ms, because it's really useless to do it every cycle
    info.UpdateTimeTracker(t_diff);
    if (info.getTimeTracker().Passed())
    {
        if (!grid.GetWorldObjectCountInNGrid<Player>() && !m.ActiveObjectsNearGrid(grid))
        {
            ObjectGridStoper worker;
            TypeContainerVisitor<ObjectGridStoper, GridTypeMapContainer> visitor(worker);
            grid.VisitAllGrids(visitor);
            grid.SetGridState(GRID_STATE_IDLE);
            sLog->outDebug(LOG_FILTER_MAPS, "Grid[%u, %u] on map %u moved to IDLE state", grid.getX(), grid.getY(), m.GetId());
        }
        else
        {
            m.ResetGridExpiry(grid, 0.1f);
        }
    }
}

void IdleState::Update(Map &m, NGridType &grid, GridInfo &, const uint32) const
{
    m.ResetGridExpiry(grid);
    grid.SetGridState(GRID_STATE_REMOVAL);
    sLog->outDebug(LOG_FILTER_MAPS, "Grid[%u, %u] on map %u moved to REMOVAL state", grid.getX(), grid.getY(), m.GetId());
}

void RemovalState::Update(Map &m, NGridType &grid, GridInfo &info, const uint32 t_diff) const
{
    if (!info.getUnloadLock())
    {
        info.UpdateTimeTracker(t_diff);
        if (info.getTimeTracker().Passed())
        {
            if (!m.UnloadGrid(grid, false))
            {
                sLog->outDebug(LOG_FILTER_MAPS, "Grid[%u, %u] for map %u differed unloading due to players or active objects nearby", grid.getX(), grid.getY(), m.GetId());
                m.ResetGridExpiry(grid);
            }
        }
    }
}

