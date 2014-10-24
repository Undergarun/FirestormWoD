#ifndef _GARRISON_H_INCLUDED_
#define _GARRISON_H_INCLUDED_

#include "DB2Stores.h"

enum GarrisonFactionIndex
{
    GARRISON_FACTION_HORDE      = 0,
    GARRISON_FACTION_ALLIANCE   = 1,
    GARRISON_FACTION_COUNT      = 2
};

enum GarrisonMissionState
{
    GARRISON_MISSION_AVAILABLE              = 0,
    GARRISON_MISSION_IN_PROGRESS            = 1,
    GARRISON_MISSION_COMPLETE_SUCCESS       = 2,
};

enum GarrisonMissionFlag
{
    GARRISON_MISSION_FLAG_RARE          = 0x01,
    GARRISON_MISSION_FLAG_UNK_2         = 0x02,
    GARRISON_MISSION_FLAG_EXHAUSTING    = 0x04,
};

enum GarrisonLearnBluePrintResult
{
    GARRISON_LEARN_BLUEPRINT_LEARNED            = 0,
    GARRISON_LEARN_BLUEPRINT_UNABLE_TO_LEARN    = 1,
    GARRISON_LEARN_BLUEPRINT_KNOWN              = 21
};

enum GarrisonPlotType
{
    GARRISON_PLOT_TYPE_SMALL            = 0,
    GARRISON_PLOT_TYPE_MEDIUM           = 1,
    GARRISON_PLOT_TYPE_LARGE            = 2,
    GARRISON_PLOT_TYPE_FARM             = 3,
    GARRISON_PLOT_TYPE_MINE             = 4,
    GARRISON_PLOT_TYPE_FISHING_HUT      = 5,
    GARRISON_PLOT_TYPE_PET_MENAGERIE    = 6,
    GARRISON_PLOT_TYPE_MAX              = 7,
};

enum GarrisonPurchaseBuildingResult
{
    GARRISON_PURCHASE_BUILDING_OK                       = 0,
    GARRISON_PURCHASE_BUILDING_BUILDING_EXIST           = 24,
    GARRISON_PURCHASE_BUILDING_INVALID_PLOT             = 10,
    GARRISON_PURCHASE_BUILDING_INVALID_BUILDING_ID      = 11,
    GARRISON_PURCHASE_BUILDING_INVALID_PLOT_BUILDING    = 16,
    GARRISON_PURCHASE_BUILDING_REQUIRE_BLUE_PRINT       = 22,
    GARRISON_PURCHASE_BUILDING_NOT_ENOUGH_CURRENCY      = 46,
    GARRISON_PURCHASE_BUILDING_NOT_ENOUGH_GOLD          = 47,
};

extern uint32 gGarrisonEmptyPlotGameObject[GARRISON_PLOT_TYPE_MAX * GARRISON_FACTION_COUNT];
extern uint32 gGarrisonBuildingPlotGameObject[GARRISON_PLOT_TYPE_MAX * GARRISON_FACTION_COUNT];
extern float gGarrisonBuildingPlotAABBDiminishReturnFactor[GARRISON_PLOT_TYPE_MAX * GARRISON_FACTION_COUNT];
extern uint32 gGarrisonBuildingActivationGameObject[GARRISON_FACTION_COUNT];

#define GARRISON_PLOT_INSTANCE_COUNT 40
#define GARRISON_CURRENCY_ID 824
#define GARRISON_MAX_FOLLOWER_LEVEL 100

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
    uint32 DB_ID;
    uint32 MissionID;
    uint32 OfferTime;
    uint32 OfferMaxDuration;
    uint32 StartTime;
    GarrisonMissionState State;
};

struct GarrisonFollower
{
    uint32 DB_ID;
    uint32 FollowerID;
    uint32 Quality;
    uint32 Level;
    uint32 ItemLevelWeapon;
    uint32 ItemLevelArmor;
    uint32 XP;
    uint32 CurrentBuildingID;
    uint32 CurrentMissionID;

    std::vector<uint32> Abilities;
};

struct GarrisonBuilding
{
    uint32 DB_ID;
    uint32 PlotInstanceID;
    uint32 BuildingID;
    uint32 SpecID;
    uint32 TimeBuiltStart;
    uint32 TimeBuiltEnd;

    bool Active;
    bool BuiltNotified;
};

class Player;

class Garrison
{
    public:
        /// Constructor
        Garrison(Player * p_Owner);

        /// Create the garrison
        void Create();
        /// Load
        bool Load();
        /// Save this garrison to DB
        void Save();
        /// Delete garisson
        static void Delete(uint64 p_PlayerGUID, SQLTransaction p_Transation);

        /// Update the garrison
        void Update();

        /// set last used activation gameobject
        void SetLastUsedActivationGameObject(uint64 p_Guid);

        /// Get GarrSiteLevelEntry for current garrison
        const GarrSiteLevelEntry * GetGarrisonSiteLevelEntry();
        /// Get Garrison Faction Index
        GarrisonFactionIndex GetGarrisonFactionIndex();

        /// Get plots for level
        std::vector<GarrisonPlotInstanceInfoLocation> GetPlots();
        /// Get plot by position
        GarrisonPlotInstanceInfoLocation GetPlot(float p_X, float p_Y, float p_Z);
        /// Get plot instance plot type
        uint32 GetPlotType(uint32 p_PlotInstanceID);
        /// Plot is free ?
        bool PlotIsFree(uint32 p_PlotInstanceID);
        /// Has plot instance
        bool HasPlotInstance(uint32 p_PlotInstanceID);
        /// Get plot location
        GarrisonPlotInstanceInfoLocation GetPlot(uint32 p_PlotInstanceID);
        /// Get plot instance ID by activation game object
        uint32 GetPlotInstanceIDByActivationGameObject(uint64 p_Guid);

        /// Add mission
        bool AddMission(uint32 p_MissionRecID);
        /// Player have mission
        bool HaveMission(uint32 p_MissionRecID);
        /// Start mission
        void StartMission(uint32 p_MissionRecID, std::vector<uint64> p_Followers);
        /// Complete a mission
        void CompleteMission(uint32 p_MissionRecID);
        /// Get mission chest chance
        void GetMissionChestChance(uint32 p_MissionRecID);
        /// Get missions
        std::vector<GarrisonMission> GetMissions();
        /// Get all completed missions
        std::vector<GarrisonMission> GetCompletedMissions();

        /// Add follower
        bool AddFollower(uint32 p_FollowerID);
        /// Get followers
        std::vector<GarrisonFollower> GetFollowers();
        /// Get follower
        GarrisonFollower GetFollower(uint32 p_FollowerID);

        /// Can build building X at slot instance Y
        bool IsBuildingPlotInstanceValid(uint32 p_BuildingRecID, uint32 p_PlotInstanceID);
        /// Player fill all condition
        GarrisonPurchaseBuildingResult CanPurchaseBuilding(uint32 p_BuildingRecID);
        /// PurchaseBuilding
        GarrisonBuilding PurchaseBuilding(uint32 p_BuildingRecID, uint32 p_PlotInstanceID, bool p_Triggered = false);
        /// Get building
        GarrisonBuilding GetBuilding(uint32 p_PlotInstanceID);
        /// Get buildings
        std::vector<GarrisonBuilding> GetBuildings();
        /// Activate building
        void ActivateBuilding(uint32 p_PlotInstanceID);
        /// Activate building
        void ActivateBuilding();
        /// Cancel construction
        void CancelConstruction(uint32 p_PlotInstanceID);
        /// Delete building
        void DeleteBuilding(uint32 p_PlotInstanceID);

        /// Get known blueprints
        std::vector<int32> GetKnownBlueprints();
        /// Learn blue print
        bool LearnBlueprint(uint32 p_BuildingRecID);
        /// Known blue print
        bool KnownBlueprint(uint32 p_BuildingRecID);

        /// Get known specializations
        std::vector<int32> GetKnownSpecializations();

    private:
        /// Init
        void Init();
        /// Init data for level
        void InitDataForLevel();
        /// Init Game objects
        void InitPlots();

        /// Update plot gameobject
        void UpdatePlot(uint32 p_PlotInstanceID);

    private:
        Player *    m_Owner;            ///< Garrison owner
        uint32      m_ID;               ///< Garrison DB ID
        uint32      m_GarrisonLevel;    ///< Garrison level
        uint32      m_GarrisonLevelID;  ///< Garrison level ID in 
        uint32      m_GarrisonSiteID;   ///< Garrison site ID

        uint64      m_LastUsedActivationGameObject;

        std::vector<GarrisonPlotInstanceInfoLocation>   m_Plots;
        std::vector<GarrisonMission>                    m_Missions;
        std::vector<GarrisonFollower>                   m_Followers;
        std::vector<GarrisonBuilding>                   m_Buildings;
        std::vector<int32>                              m_KnownBlueprints;
        std::vector<int32>                              m_KnownSpecializations;

        std::map<uint32, uint64> m_PlotsGob;
        std::map<uint32, uint64> m_PlotsActivateGob;
        std::map<uint32, std::vector<uint64>> m_PlotsBuildingCosmeticGobs;
};


#endif