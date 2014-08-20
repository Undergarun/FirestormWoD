#ifndef _GARRISON_H_INCLUDED_
#define _GARRISON_H_INCLUDED_

#include "DB2Stores.h"

enum GarrisonFactionIndex
{
    GARRISON_FACTION_HORDE      = 0,
    GARRISON_FACTION_ALLIANCE   = 1
};

enum GarrisonMissionState
{
    GARRISON_MISSION_AVAILABLE      = 0,
    GARRISON_MISSION_IN_PROGRESS    = 1
};


enum GarrisonPlotType
{
    GARRISON_PLOT_TYPE_SMALL    = 0,
    GARRISON_PLOT_TYPE_MEDIUM   = 1,
    GARRISON_PLOT_TYPE_LARGE    = 2,
    GARRISON_PLOT_TYPE_MAX      = 3,
};

extern uint32 gGarrisonEmptyPlotGameObject[GARRISON_PLOT_TYPE_MAX];

#define GARRISON_PLOT_INSTANCE_COUNT 40

struct GarrisonPlotInstanceInfoLocation
{
    uint32 SiteLevelID;
    uint32 PlotInstanceID;
    float X, Y, Z;
    float O;
};

extern GarrisonPlotInstanceInfoLocation gGarrisonPlotInstanceInfoLocation[GARRISON_PLOT_INSTANCE_COUNT];

struct GarrisonMission
{
    uint64 DB_ID;
    uint32 MissionID;
    uint32 OfferTime;
    uint32 OfferMaxDuration;
    uint32 StartTime;
    GarrisonMissionState State;
};

class Player;

class Garrison
{
    public:
        /// Constructor
        Garrison(Player * p_Owner);

        /// Save this garrison to DB
        void Save();

        /// Get GarrSiteLevelEntry for current garrison
        const GarrSiteLevelEntry * GetGarrisonSiteLevelEntry();
        /// Get Garrison Faction Index
        GarrisonFactionIndex GetGarrisonFactionIndex();
        /// Get plots for level
        std::vector<GarrisonPlotInstanceInfoLocation> GetPlots();
        /// Get plot instance plot type
        uint32 GetPlotType(uint32 p_PlotInstanceID);

    public:
        /// Open mission npc at client side
        void SendGarrisonOpenMissionNpc(uint64 p_CreatureGUID);

    private:
        /// Init data for level
        void InitDataForLevel();

    private:
        Player *    m_Owner;            ///< Garrison owner
        uint32      m_GarrisonLevel;    ///< Garrison level
        uint32      m_GarrisonLevelID;  ///< Garrison level ID in 
        uint32      m_GarrisonSiteID;   ///< Garrison site ID

        std::vector<GarrisonPlotInstanceInfoLocation>   m_Plots;
        std::vector<GarrisonMission>                    m_Missions;
};


#endif