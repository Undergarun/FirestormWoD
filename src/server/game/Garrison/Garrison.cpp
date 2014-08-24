#include "Garrison.h"
#include "Player.h"
#include "DatabaseEnv.h"

uint32 gGarrisonEmptyPlotGameObject[GARRISON_PLOT_TYPE_MAX * GARRISON_FACTION_COUNT] =
{
    /// Horde
    233083,     ///< GARRISON_PLOT_TYPE_SMALL
    0,          ///< GARRISON_PLOT_TYPE_MEDIUM
    233081,     ///< GARRISON_PLOT_TYPE_LARGE
    0,          ///< GARRISON_PLOT_TYPE_FARM
    0,          ///< GARRISON_PLOT_TYPE_MINE
    0,          ///< GARRISON_PLOT_TYPE_FISHING_HUT
    0,          ///< GARRISON_PLOT_TYPE_PET_MENAGERIE
    /// Alliance
    229501,     ///< GARRISON_PLOT_TYPE_SMALL
    232283,     ///< GARRISON_PLOT_TYPE_MEDIUM
    232143,     ///< GARRISON_PLOT_TYPE_LARGE
    232286,     ///< GARRISON_PLOT_TYPE_FARM
    233485,     ///< GARRISON_PLOT_TYPE_MINE
    237223,     ///< GARRISON_PLOT_TYPE_FISHING_HUT
    0,          ///< GARRISON_PLOT_TYPE_PET_MENAGERIE
};

uint32 gGarrisonBuildingPlotGameObject[GARRISON_PLOT_TYPE_MAX * GARRISON_FACTION_COUNT] =
{
    /// Horde
    0,          ///< GARRISON_PLOT_TYPE_SMALL
    0,          ///< GARRISON_PLOT_TYPE_MEDIUM
    232410,     ///< GARRISON_PLOT_TYPE_LARGE
    0,          ///< GARRISON_PLOT_TYPE_FARM same as GARRISON_PLOT_TYPE_MEDIUM
    0,          ///< GARRISON_PLOT_TYPE_MINE same as GARRISON_PLOT_TYPE_MEDIUM
    0,          ///< GARRISON_PLOT_TYPE_FISHING_HUT same as GARRISON_PLOT_TYPE_SMALL
    0,          ///< GARRISON_PLOT_TYPE_PET_MENAGERIE same as GARRISON_PLOT_TYPE_SMALL
    /// Alliance
    233957,     ///< GARRISON_PLOT_TYPE_SMALL
    232409,     ///< GARRISON_PLOT_TYPE_MEDIUM
    232411,     ///< GARRISON_PLOT_TYPE_LARGE
    232409,     ///< GARRISON_PLOT_TYPE_FARM same as GARRISON_PLOT_TYPE_MEDIUM
    232409,     ///< GARRISON_PLOT_TYPE_MINE same as GARRISON_PLOT_TYPE_MEDIUM
    233957,     ///< GARRISON_PLOT_TYPE_FISHING_HUT same as GARRISON_PLOT_TYPE_SMALL
    233957,     ///< GARRISON_PLOT_TYPE_PET_MENAGERIE same as GARRISON_PLOT_TYPE_SMALL
};

GarrisonPlotInstanceInfoLocation gGarrisonPlotInstanceInfoLocation[GARRISON_PLOT_INSTANCE_COUNT] = {
    /// SiteLevelID PlotInstanceID      X            Y            Z           O
    /// Alliance Level 1                                                    
    {       5,          19,         1829.896f,    197.5504f,    72.00920f,   1.8849560f  },
    {       5,          23,         1911.550f,    232.9792f,    76.65489f,   2.7952700f  },
    /// Alliance Level 2                                                    
    {     444,          18,         1819.583f,    231.2813f,    72.17403f,  -1.2915440f  },
    {     444,          19,         1829.896f,    197.5504f,    71.98585f,   1.8849560f  },
    {     444,          22,         1864.955f,    320.2083f,    81.66048f,  -1.4835300f  },
    {     444,          23,         1918.637f,    228.7674f,    76.63956f,   2.7750740f  },
    {     444,          59,         1845.083f,    146.2743f,    53.43811f,   0.3490658f  },
    {     444,          63,         1847.615f,    134.7257f,    78.10705f,   2.7052600f  },
    {     444,          67,         2031.594f,    174.4410f,    84.59409f,   2.8361600f  },
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
    {     258,          18,         5645.124f,   4508.9600f,   119.27010f,   2.0423500f  },
    {     258,          23,         5575.461f,   4459.3380f,   130.36810f,   0.9599311f  },
    /// Horde level 2                                                       
    {     445,          18,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     445,          19,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     445,          22,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     445,          23,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     445,          59,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     445,          63,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     445,          67,            0.0f,        0.0f,        0.0f,       0.0f        },
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
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Create the garrison
void Garrison::Create()
{
    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GARRISON);

    uint32 l_Index = 0;
    l_Stmt->setUInt32(l_Index++, m_Owner->GetGUIDLow());
    l_Stmt->setUInt32(l_Index++, m_GarrisonLevel);

    CharacterDatabase.Query(l_Stmt);

    l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GARRISON_DB_ID);

    l_Index = 0;
    l_Stmt->setUInt32(l_Index++, m_Owner->GetGUIDLow());

    PreparedQueryResult l_Result = CharacterDatabase.Query(l_Stmt);

    if (!l_Result)
        assert(false && "Garrison::Create() failed to retrieve created garrison ID");

    Field * l_Fields = l_Result->Fetch();
    
    m_ID = l_Fields[0].GetUInt32();

    Init();
}
/// Load
bool Garrison::Load()
{
    PreparedStatement * l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GARRISON);

    uint32 l_Index = 0;
    l_Stmt->setUInt32(0, m_Owner->GetGUIDLow());

    PreparedQueryResult l_Result = CharacterDatabase.Query(l_Stmt);

    if (l_Result)
    {
        Field * l_Fields = l_Result->Fetch();

        m_ID            = l_Fields[0].GetUInt32();
        m_GarrisonLevel = l_Fields[1].GetUInt32();

        if (!l_Fields[2].GetString().empty())
        {
            Tokenizer l_BluePrints(l_Fields[2].GetString(), ' ');

            for (Tokenizer::const_iterator l_It = l_BluePrints.begin(); l_It != l_BluePrints.end(); ++l_It)
                m_KnownBlueprints.push_back(atol(*l_It));
        }

        if (!l_Fields[3].GetString().empty())
        {
            Tokenizer l_Specializations(l_Fields[3].GetString(), ' ');

            for (Tokenizer::const_iterator l_It = l_Specializations.begin(); l_It != l_Specializations.end(); ++l_It)
                m_KnownSpecializations.push_back(atol(*l_It));
        }

        l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GARRISON_BUILDING);
        l_Stmt->setUInt32(0, m_ID);

        l_Result = CharacterDatabase.Query(l_Stmt);

        if (l_Result)
        {
            do
            {
                l_Fields = l_Result->Fetch();

                GarrisonBuilding l_Building;
                l_Building.DB_ID            = l_Fields[0].GetUInt32();
                l_Building.PlotInstanceID   = l_Fields[1].GetUInt32();
                l_Building.BuildingID       = l_Fields[2].GetUInt32();
                l_Building.SpecID           = l_Fields[3].GetUInt32();
                l_Building.TimeBuiltStart   = l_Fields[4].GetUInt32();
                l_Building.TimeBuiltEnd     = l_Fields[5].GetUInt32();
                l_Building.Active           = l_Fields[6].GetBool();

                m_Buildings.push_back(l_Building);
            } while (l_Result->NextRow());
        }

        l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GARRISON_MISSION);
        l_Stmt->setUInt32(0, m_ID);

        l_Result = CharacterDatabase.Query(l_Stmt);

        if (l_Result)
        {
            do
            {
                l_Fields = l_Result->Fetch();

                GarrisonMission l_Mission;
                l_Mission.DB_ID             = l_Fields[0].GetUInt32();
                l_Mission.MissionID         = l_Fields[1].GetUInt32();
                l_Mission.OfferTime         = l_Fields[2].GetUInt32();
                l_Mission.OfferMaxDuration  = l_Fields[3].GetUInt32();
                l_Mission.StartTime         = l_Fields[4].GetUInt32();
                l_Mission.State             = (GarrisonMissionState)l_Fields[5].GetUInt32();

                m_Missions.push_back(l_Mission);
            } while (l_Result->NextRow());
        }

        Init();

        return true;
    }

    return false;
}
/// Save this garrison to DB
void Garrison::Save()
{
    std::ostringstream l_KnownBluePrintsStr;

    for (uint32 l_I = 0; l_I < m_KnownBlueprints.size(); ++l_I)
        l_KnownBluePrintsStr << m_KnownBlueprints[l_I] << ' ';

    std::ostringstream l_KnownSpecializationsStr;

    for (uint32 l_I = 0; l_I < m_KnownSpecializations.size(); ++l_I)
        l_KnownSpecializationsStr << m_KnownSpecializations[l_I] << ' ';

    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GARRISON);

    uint32 l_Index = 0;
    l_Stmt->setUInt32(l_Index++, m_GarrisonLevel);
    l_Stmt->setString(l_Index++, l_KnownBluePrintsStr.str());
    l_Stmt->setString(l_Index++, l_KnownSpecializationsStr.str());
    l_Stmt->setUInt32(l_Index++, m_ID);

    CharacterDatabase.AsyncQuery(l_Stmt);

    for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
    {
        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GARRISON_BUILDING);

        uint32 l_Index = 0;
        l_Stmt->setUInt32(l_Index++, m_Buildings[l_I].PlotInstanceID);
        l_Stmt->setUInt32(l_Index++, m_Buildings[l_I].BuildingID);
        l_Stmt->setUInt32(l_Index++, m_Buildings[l_I].SpecID);
        l_Stmt->setUInt32(l_Index++, m_Buildings[l_I].TimeBuiltStart);
        l_Stmt->setUInt32(l_Index++, m_Buildings[l_I].TimeBuiltEnd);
        l_Stmt->setBool(l_Index++, m_Buildings[l_I].Active);

        l_Stmt->setUInt32(l_Index++, m_Buildings[l_I].DB_ID);
        l_Stmt->setUInt32(l_Index++, m_ID);

        CharacterDatabase.AsyncQuery(l_Stmt);
    }

    for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
    {
        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GARRISON_MISSION);

        uint32 l_Index = 0;
        l_Stmt->setUInt32(l_Index++, m_Missions[l_I].MissionID);
        l_Stmt->setUInt32(l_Index++, m_Missions[l_I].OfferTime);
        l_Stmt->setUInt32(l_Index++, m_Missions[l_I].OfferMaxDuration);
        l_Stmt->setUInt32(l_Index++, m_Missions[l_I].StartTime);
        l_Stmt->setUInt32(l_Index++, m_Missions[l_I].State);

        l_Stmt->setUInt32(l_Index++, m_Missions[l_I].DB_ID);
        l_Stmt->setUInt32(l_Index++, m_ID);

        CharacterDatabase.AsyncQuery(l_Stmt);
    }
}
/// Delete garisson
void Garrison::Delete(uint64 p_PlayerGUID, SQLTransaction p_Transation)
{
    PreparedStatement* l_Stmt;

    l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_BUILDING);
    l_Stmt->setUInt32(0, p_PlayerGUID);
    p_Transation->Append(l_Stmt);

    l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_MISSION);
    l_Stmt->setUInt32(0, p_PlayerGUID);
    p_Transation->Append(l_Stmt);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Update the garrison
void Garrison::Update()
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

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

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

    return l_PlotEntry->PlotType;
}
/// Plot is free ?
bool Garrison::PlotIsFree(uint32 p_PlotInstanceID)
{
    for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        if (m_Buildings[l_I].PlotInstanceID == p_PlotInstanceID)
            return false;

    return true;
}
/// Has plot instance
bool Garrison::HasPlotInstance(uint32 p_PlotInstanceID)
{
    for (uint32 l_I = 0; l_I < m_Plots.size(); ++l_I)
        if (m_Plots[l_I].PlotInstanceID == p_PlotInstanceID)
            return true;

    return false;
}
/// Get plot location
GarrisonPlotInstanceInfoLocation Garrison::GetPlot(uint32 p_PlotInstanceID)
{
    for (uint32 l_I = 0; l_I < m_Plots.size(); ++l_I)
    {
        if (m_Plots[l_I].SiteLevelID == m_GarrisonLevelID && m_Plots[l_I].PlotInstanceID == p_PlotInstanceID)
            return m_Plots[l_I];
    }

    return GarrisonPlotInstanceInfoLocation();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Add mission
bool Garrison::AddMission(uint32 p_MissionRecID)
{
    const GarrMissionEntry * l_MissionEntry = sGarrMissionStore.LookupEntry(p_MissionRecID);

    if (!l_MissionEntry)
        return false;

    if (HaveMission(p_MissionRecID))
        return false;

    if (l_MissionEntry->RequiredLevel > m_Owner->getLevel())
        return false;

    if (l_MissionEntry->RequiredItemLevel > m_Owner->GetAverageItemLevel())
        return false;

    GarrisonMission l_Mission;

    l_Mission.MissionID         = p_MissionRecID;
    l_Mission.OfferTime         = time(0);
    l_Mission.OfferMaxDuration  = l_MissionEntry->OfferDuration;
    l_Mission.State             = GARRISON_MISSION_AVAILABLE;
    l_Mission.StartTime         = 0;

    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GARRISON_MISSION);

    uint32 l_Index = 0;
    l_Stmt->setUInt32(l_Index++, m_ID);
    l_Stmt->setUInt32(l_Index++, p_MissionRecID);

    CharacterDatabase.Query(l_Stmt);

    l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GARRISON_MISSION_DB_ID);

    l_Index = 0;
    l_Stmt->setUInt32(l_Index++, m_ID);
    l_Stmt->setUInt32(l_Index++, p_MissionRecID);

    PreparedQueryResult l_Result = CharacterDatabase.Query(l_Stmt);

    if (!l_Result)
        assert(false && "Garrison::AddMission() failed to retrieve created garrison mission ID");

    Field * l_Fields = l_Result->Fetch();

    l_Mission.DB_ID = l_Fields[0].GetUInt32();

    m_Missions.push_back(l_Mission);

    return true;
}
/// Player have mission
bool Garrison::HaveMission(uint32 p_MissionRecID)
{
    for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        if (m_Missions[l_I].MissionID == p_MissionRecID)
            return true;

    return false;
}
/// Get missions
std::vector<GarrisonMission> Garrison::GetMissions()
{
    std::vector<GarrisonMission> l_Result;

    for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        if (m_Missions[l_I].State != GARRISON_MISSION_COMPLETE)
            l_Result.push_back(m_Missions[l_I]);

    return l_Result;
}
/// Get all completed missions
std::vector<GarrisonMission> Garrison::GetCompletedMissions()
{
    std::vector<GarrisonMission> l_Result;

    for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        if (m_Missions[l_I].State == GARRISON_MISSION_COMPLETE)
            l_Result.push_back(m_Missions[l_I]);

    return l_Result;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Can build building X at slot instance Y
bool Garrison::IsBuildingPlotInstanceValid(uint32 p_BuildingRecID, uint32 p_PlotInstanceID)
{
    const GarrBuildingEntry * l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingRecID);

    if (!l_BuildingEntry)
        return false;

    const GarrPlotInstanceEntry * l_PlotInstanceEntry = sGarrPlotInstanceStore.LookupEntry(p_PlotInstanceID);

    if (!l_PlotInstanceEntry)
        return false;

    /// Search building plot ID 
    for (uint32 l_I = 0; l_I < sGarrPlotBuildingStore.GetNumRows(); ++l_I)
    {
        const GarrPlotBuildingEntry * l_PlotBuildingEntry = sGarrPlotBuildingStore.LookupEntry(l_I);

        if (l_PlotBuildingEntry && l_PlotBuildingEntry->PlotId == l_PlotInstanceEntry->PlotID && l_PlotBuildingEntry->BuildingID == p_BuildingRecID)
            return true;
    }

    return false;
}
/// Player fill all condition
GarrisonPurchaseBuildingResult Garrison::CanPurchaseBuilding(uint32 p_BuildingRecID)
{
    const GarrBuildingEntry * l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingRecID);

    if (!l_BuildingEntry)
        return GARRISON_PURCHASE_BUILDING_INVALID_BUILDING_ID;

    if (l_BuildingEntry->BuildCostCurrencyID != 0)
    {
        if (!m_Owner->HasCurrency(l_BuildingEntry->BuildCostCurrencyID, l_BuildingEntry->BuildCostCurrencyAmount))
            return GARRISON_PURCHASE_BUILDING_NOT_ENOUGH_CURRENCY;
    }

    return GARRISON_PURCHASE_BUILDING_OK;
}
/// PurchaseBuilding
GarrisonBuilding Garrison::PurchaseBuilding(uint32 p_BuildingRecID, uint32 p_PlotInstanceID, bool p_Triggered)
{
    const GarrBuildingEntry * l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingRecID);

    GarrisonBuilding l_Building;

    memset(&l_Building, 0, sizeof(l_Building));

    if (!l_BuildingEntry)
        return l_Building;

    if (l_BuildingEntry->BuildCostCurrencyID != 0 && !p_Triggered)
        m_Owner->ModifyCurrency(l_BuildingEntry->BuildCostCurrencyID, -(int32)l_BuildingEntry->BuildCostCurrencyAmount);

    if (!p_Triggered)
    {
        WorldPacket l_PlotRemoved(SMSG_GARRISON_PLOT_REMOVED, 4);
        l_PlotRemoved << uint32(p_PlotInstanceID);
        m_Owner->SendDirectMessage(&l_PlotRemoved);
    }

    l_Building.BuildingID       = p_BuildingRecID;
    l_Building.PlotInstanceID   = p_PlotInstanceID;
    l_Building.TimeBuiltStart   = time(0);
    l_Building.TimeBuiltEnd     = time(0) + l_BuildingEntry->BuildTime;           ///< 5/5/1905 18:45:05
    l_Building.SpecID           = 0;
    l_Building.Active           = false;

    if (p_Triggered)
        l_Building.TimeBuiltEnd = l_Building.TimeBuiltStart;

    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GARRISON_BUILDING);

    uint32 l_Index = 0;
    l_Stmt->setUInt32(l_Index++, m_ID);
    l_Stmt->setUInt32(l_Index++, p_PlotInstanceID);
    l_Stmt->setUInt32(l_Index++, p_BuildingRecID);

    CharacterDatabase.Query(l_Stmt);

    l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GARRISON_BUILDING_DB_ID);

    l_Index = 0;
    l_Stmt->setUInt32(l_Index++, m_ID);
    l_Stmt->setUInt32(l_Index++, p_PlotInstanceID);
    l_Stmt->setUInt32(l_Index++, p_BuildingRecID);

    PreparedQueryResult l_Result = CharacterDatabase.Query(l_Stmt);

    if (!l_Result)
        assert(false && "Garrison::PurchaseBuilding() failed to retrieve created garrison building ID");

    Field * l_Fields = l_Result->Fetch();

    l_Building.DB_ID = l_Fields[0].GetUInt32();

    m_Buildings.push_back(l_Building);

    UpdatePlotGameObject(p_PlotInstanceID);
    
    return l_Building;
}
/// Get building
GarrisonBuilding Garrison::GetBuilding(uint32 p_PlotInstanceID)
{
    for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        if (m_Buildings[l_I].PlotInstanceID == p_PlotInstanceID)
            return m_Buildings[l_I];

    return GarrisonBuilding();
}
/// Get buildings
std::vector<GarrisonBuilding> Garrison::GetBuildings()
{
    return m_Buildings;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Get known blueprints
std::vector<int32> Garrison::GetKnownBlueprints()
{
    return m_KnownBlueprints;
}
/// Learn blue print
bool Garrison::LearnBlueprint(uint32 p_BuildingRecID)
{
    GarrisonLearnBluePrintResult l_ResultCode;

    if (std::find(m_KnownBlueprints.begin(), m_KnownBlueprints.end(), p_BuildingRecID) == m_KnownBlueprints.end())
    {
        const GarrBuildingEntry * l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingRecID);

        if (l_BuildingEntry)
        {
            m_KnownBlueprints.push_back(p_BuildingRecID);
            l_ResultCode = GARRISON_LEARN_BLUEPRINT_LEARNED;
        }
        else
        {
            l_ResultCode = GARRISON_LEARN_BLUEPRINT_UNABLE_TO_LEARN;
        }
    }
    else
    {
        l_ResultCode = GARRISON_LEARN_BLUEPRINT_KNOWN;
    }

    WorldPacket l_Result(SMSG_GARRISON_LEARN_BLUEPRINT_RESULT, 8);
    l_Result << uint32(l_ResultCode);
    l_Result << uint32(p_BuildingRecID);

    m_Owner->SendDirectMessage(&l_Result);

    return true;
}
/// Known blue print
bool Garrison::KnownBlueprint(uint32 p_BuildingRecID)
{
    return std::find(m_KnownBlueprints.begin(), m_KnownBlueprints.end(), p_BuildingRecID) != m_KnownBlueprints.end();
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Get known specializations
std::vector<int32> Garrison::GetKnownSpecializations()
{
    return m_KnownSpecializations;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Init
void Garrison::Init()
{
    InitDataForLevel();
    InitGameObjects();
}
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
/// Init Game objects
void Garrison::InitGameObjects()
{
    for (uint32 l_I = 0; l_I < m_Plots.size(); ++l_I)
        UpdatePlotGameObject(m_Plots[l_I].PlotInstanceID);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Update plot game object
void Garrison::UpdatePlotGameObject(uint32 p_PlotInstanceID)
{
    GarrisonPlotInstanceInfoLocation l_PlotInfo = GetPlot(p_PlotInstanceID);

    if (m_PlotsGob[p_PlotInstanceID] != 0)
    {
        GameObject * l_Gob = sObjectAccessor->GetGameObjects().at(m_PlotsGob[p_PlotInstanceID]);

        if (l_Gob)
        {
            l_Gob->CleanupsBeforeDelete();
            delete l_Gob;
        }
    }

    uint32 l_GobEntry = 0;

    if (PlotIsFree(p_PlotInstanceID))
    {
        l_GobEntry = gGarrisonEmptyPlotGameObject[GetPlotType(p_PlotInstanceID) + (GetGarrisonFactionIndex() * GARRISON_PLOT_TYPE_MAX)];
    }
    else
    {
        GarrisonBuilding l_Building = GetBuilding(p_PlotInstanceID);

        if (l_Building.TimeBuiltEnd > time(0))
        {
            l_GobEntry = gGarrisonBuildingPlotGameObject[GetPlotType(p_PlotInstanceID) + (GetGarrisonFactionIndex() * GARRISON_PLOT_TYPE_MAX)];
        }
        else
        {
            const GarrBuildingEntry * l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_Building.BuildingID);
            
            if (!l_BuildingEntry)
                return;

            l_GobEntry = l_BuildingEntry->GameObjects[GetGarrisonFactionIndex()];
        }
    }

    if (l_GobEntry != 0)
    {
        GameObject * l_Gob = m_Owner->SummonGameObject(l_GobEntry, l_PlotInfo.X, l_PlotInfo.Y, l_PlotInfo.Z, l_PlotInfo.O, 0, 0, 0, 0, 0);
        
        if (l_Gob)
            m_PlotsGob[p_PlotInstanceID] = l_Gob->GetGUID();
    }
}