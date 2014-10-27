#ifndef _GARRISON_H_INCLUDED_
#define _GARRISON_H_INCLUDED_

#include "DB2Stores.h"

#ifdef _MSC_VER
    #define GARRISON_CHEST_FORMULA_DEBUG 1
#endif

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
    GARRISON_MISSION_COMPLETE_FAILED        = 3,
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

enum GarrisonAbilityEffectType
{
    GARRISION_ABILITY_EFFECT_UNK_0                                  = 0,    ///< @TODO
    GARRISION_ABILITY_EFFECT_MOD_WIN_RATE_SOLO                      = 1,    ///< Proc if MissionFollowerCount == 1
    GARRISION_ABILITY_EFFECT_MOD_WIN_RATE                           = 2,    ///< Proc every time
    GARRISION_ABILITY_EFFECT_MOD_TRAVEL_TIME                        = 3,    ///< Proc every time
    GARRISION_ABILITY_EFFECT_MOD_XP_GAIN                            = 4,    ///< Mod the XP earn (self, party)
    GARRISION_ABILITY_EFFECT_MOD_WIN_RATE_CLASS                     = 5,    ///< Proc if Find(MissionFollowers[Class], MiscValueA) != NULL 
    GARRISION_ABILITY_EFFECT_MOD_WIN_RATE_DURATION_MORE             = 6,    ///< Proc if Duration > (3600 * MiscValueB)
    GARRISION_ABILITY_EFFECT_MOD_WIN_RATE_DURATION_LESS             = 7,    ///< Proc if Duration < (3600 * MiscValueB)
    GARRISION_ABILITY_EFFECT_MOD_GARR_CURRENCY_DROP                 = 8,    ///< @TODO
    GARRISION_ABILITY_EFFECT_MOD_WIN_RATE_TRAVEL_DURATION_MORE      = 9,    ///< Proc if TravelDuration > (3600 * MiscValueB)
    GARRISION_ABILITY_EFFECT_MOD_WIN_RATE_TRAVEL_DURATION_LESS      = 10,   ///< Proc if TravelDuration < (3600 * MiscValueB)
    GARRISION_ABILITY_EFFECT_UNK_11                                 = 11,   ///< UNUSED
    GARRISION_ABILITY_EFFECT_MOD_DUMMY_PRODUCTION                   = 12,   ///< @TODO
    GARRISION_ABILITY_EFFECT_MOD_BRONZE_TREASURE_DROP               = 13,   ///< @TODO
    GARRISION_ABILITY_EFFECT_MOD_SILVER_TREASURE_DROP               = 14,   ///< @TODO
    GARRISION_ABILITY_EFFECT_MOD_GOLD_TREASURE_DROP                 = 15,   ///< @TODO
    GARRISION_ABILITY_EFFECT_MOD_CHEST_DROP_RATE                    = 16,   ///< @TODO
    GARRISION_ABILITY_EFFECT_MOD_MISSION_DURATION                   = 17,   ///< Proc every time
};

enum GarrisonAbilityEffectTargetMask
{
    GARRISON_ABILITY_EFFECT_TARGET_MASK_UNK   = 0 << 0,
    GARRISON_ABILITY_EFFECT_TARGET_MASK_SELF  = 1 << 0,
    GARRISON_ABILITY_EFFECT_TARGET_MASK_PARTY = 1 << 1,
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
        void StartMissionFailed();
        /// Complete a mission
        void CompleteMission(uint32 p_MissionRecID);
        /// Get followers on a mission
        std::vector<GarrisonFollower*> GetMissionFollowers(uint32 p_MissionRecID);
        /// Get mission followers abilities effect
        std::vector<uint32> GetMissionFollowersAbilitiesEffects(uint32 p_MissionRecID);
        /// Get mission followers abilities effect
        std::vector<uint32> GetMissionFollowersAbilitiesEffects(uint32 p_MissionRecID, GarrisonAbilityEffectType p_Type, uint32 p_TargetMask);
        /// Get the mission travel time
        uint32 GetMissionTravelDuration(uint32 p_MissionRecID);
        /// Get the mission duration
        uint32 GetMissionDuration(uint32 p_MissionRecID);
        /// Get mission chest chance
        uint32 GetMissionChestChance(uint32 p_MissionRecID);
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
        /// Get building passive ability effects
        std::vector<uint32> GetBuildingsPassiveAbilityEffects();
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

        std::map<uint32, uint64>                m_PlotsGob;
        std::map<uint32, uint64>                m_PlotsActivateGob;
        std::map<uint32, std::vector<uint64>>   m_PlotsBuildingCosmeticGobs;
};


#endif