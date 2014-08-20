#include "Garrison.h"
#include "Player.h"

uint32 gGarrisonEmptyPlotGameObject[GARRISON_PLOT_TYPE_MAX] = 
{
    229501,     ///< GARRISON_PLOT_TYPE_SMALL
    0,          ///< GARRISON_PLOT_TYPE_MEDIUM
    232143      ///< GARRISON_PLOT_TYPE_LARGE
};

GarrisonPlotInstanceInfoLocation gGarrisonPlotInstanceInfoLocation[GARRISON_PLOT_INSTANCE_COUNT] = {
    /// SiteLevelID PlotInstanceID      X            Y            Z           O
    /// Alliance Level 1                                                    
    {       5,          19,         1829.896f,    197.5504f,    72.00920f,   1.884956f   },
    {       5,          23,         1911.550f,    232.9792f,    76.65489f,   2.795270f   },
    /// Alliance Level 2                                                    
    {     444,          18,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     444,          19,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     444,          22,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     444,          23,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     444,          59,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     444,          63,            0.0f,        0.0f,        0.0f,       0.0f        },
    /// Alliance Level 3                                                  
    {       6,          18,            0.0f,        0.0f,        0.0f,       0.0f        },
    {       6,          19,            0.0f,        0.0f,        0.0f,       0.0f        },
    {       6,          20,            0.0f,        0.0f,        0.0f,       0.0f        },
    {       6,          22,            0.0f,        0.0f,        0.0f,       0.0f        },
    {       6,          23,            0.0f,        0.0f,        0.0f,       0.0f        },
    {       6,          24,            0.0f,        0.0f,        0.0f,       0.0f        },
    {       6,          25,            0.0f,        0.0f,        0.0f,       0.0f        },
    {       6,          59,            0.0f,        0.0f,        0.0f,       0.0f        },
    {       6,          63,            0.0f,        0.0f,        0.0f,       0.0f        },
    {       6,          67,            0.0f,        0.0f,        0.0f,       0.0f        },
    {       6,          81,            0.0f,        0.0f,        0.0f,       0.0f        },
    /// Horde Level 1                                                       
    {     258,          18,         5645.124f,   4508.960f,    119.2701f,    2.0423500f  },
    {     258,          23,         5575.461f,   4459.338f,    130.3681f,    0.9599311f  },
    /// Horde level 2                                                       
    {     445,          18,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     445,          19,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     445,          22,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     445,          23,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     445,          59,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     445,          63,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     445,          67,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     444,          67,            0.0f,        0.0f,        0.0f,       0.0f        },
    /// Horde level 3                                                      
    {     259,          18,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     259,          19,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     259,          20,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     259,          22,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     259,          23,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     259,          24,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     259,          25,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     259,          59,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     259,          63,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     259,          67,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     259,          81,            0.0f,        0.0f,        0.0f,       0.0f        }
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Constructor
Garrison::Garrison(Player * p_Owner)
    : m_Owner(p_Owner)
{
    m_GarrisonLevel     = 1;
    m_GarrisonLevelID   = 0;
    m_GarrisonSiteID    = 0;

    /// Select Garrison site ID
    switch (GetGarrisonFactionIndex())
    {
        case GARRISON_FACTION_ALLIANCE:
            m_GarrisonSiteID = 2;
            break;

        case GARRISON_FACTION_HORDE:
            m_GarrisonSiteID = 71;
            break;
    }

    InitDataForLevel();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Save this garrison to DB
void Garrison::Save()
{

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Get GarrSiteLevelEntry for current garrison
const GarrSiteLevelEntry * Garrison::GetGarrisonSiteLevelEntry()
{
    return sGarrSiteLevelStore.LookupEntry(m_GarrisonLevelID);
}
/// Get Garrison Faction Index
GarrisonFactionIndex Garrison::GetGarrisonFactionIndex()
{
    assert(m_Owner);
    
    switch (m_Owner->GetTeam())
    {
        case ALLIANCE:
            return GARRISON_FACTION_ALLIANCE;

        case HORDE:
            return GARRISON_FACTION_HORDE;
    }

    return GARRISON_FACTION_HORDE;
}
/// Get slots for level
std::vector<GarrisonPlotInstanceInfoLocation> Garrison::GetPlots()
{
    return m_Plots;
}
/// Get plot instance plot type
uint32 Garrison::GetPlotType(uint32 p_PlotInstanceID)
{
    const GarrPlotInstanceEntry * l_PlotInstanceEntry = sGarrPlotInstanceStore.LookupEntry(p_PlotInstanceID);

    if (!l_PlotInstanceEntry)
        return 0;

    const GarrPlotEntry * l_PlotEntry = sGarrPlotStore.LookupEntry(l_PlotInstanceEntry->PlotID);

    if (!l_PlotEntry)
        return 0;

    const GarrPlotUICategoryEntry * l_PlotUICategoryEntry = sGarrPlotUICategoryStore.LookupEntry(l_PlotEntry->PlotUiCategoryID);

    if (!l_PlotUICategoryEntry)
        return 0;

    return l_PlotUICategoryEntry->Type;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Open mission npc at client side
void Garrison::SendGarrisonOpenMissionNpc(uint64 p_CreatureGUID)
{
    uint32 l_MissionToUpdateCount = 0;

    WorldPacket l_Data(SMSG_GARRISON_OPEN_ARCHITECT, 4);

    l_Data.appendPackGUID(p_CreatureGUID);
    l_Data << uint32(l_MissionToUpdateCount);

    if (l_MissionToUpdateCount)
    {
        /// TODO
    }
    else
    {
        l_Data.WriteBit(false);    ///< Hide UI
        l_Data.FlushBits();
    }

    m_Owner->SendDirectMessage(&l_Data);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Init data for level
void Garrison::InitDataForLevel()
{
    const GarrSiteLevelEntry * l_SiteEntry = nullptr;

    /// Search garrison site entry by SideID & Level
    for (uint32 l_I = 0; l_I < sGarrSiteLevelStore.GetNumRows(); ++l_I)
    {
        const GarrSiteLevelEntry * l_CurrentSiteEntry = sGarrSiteLevelStore.LookupEntry(l_I);

        if (l_CurrentSiteEntry && l_CurrentSiteEntry->Level == m_GarrisonLevel && l_CurrentSiteEntry->SiteID == m_GarrisonSiteID)
        {
            l_SiteEntry = l_CurrentSiteEntry;
            break;
        }
    }

    if (!l_SiteEntry)
        assert(false && "Garrison::InitDataForLevel() not data found");

    m_GarrisonLevelID = l_SiteEntry->SiteLevelID;

    /// Find all plots for garrison level
    m_Plots.clear();

    for (uint32 l_I = 0; l_I < GARRISON_PLOT_INSTANCE_COUNT; ++l_I)
    {
        if (gGarrisonPlotInstanceInfoLocation[l_I].SiteLevelID == m_GarrisonLevelID)
            m_Plots.push_back(gGarrisonPlotInstanceInfoLocation[l_I]);
    }


}