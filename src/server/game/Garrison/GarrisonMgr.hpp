////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_MGR_HPP_GARRISON
#define GARRISON_MGR_HPP_GARRISON

#include "GarrisonMgrStructures.hpp"
#include "GarrisonMgrConstants.hpp"
#include "GarrisonFollower.hpp"
#include "GarrisonMission.hpp"
#include "GarrisonBuilding.hpp"
#include "GarrisonWorkOrder.hpp"
#include "GarrisonShipmentManager.hpp"
#include "GarrisonBuildingManager.hpp"

#include "Interfaces/Interface_GarrisonSite.hpp"

/// This macro enabled compilation of debug code in Dev env
#ifdef _MSC_VER
    #define GARRISON_CHEST_FORMULA_DEBUG 1
#endif

namespace MS { namespace Garrison 
{
    /// Garrison manager class
    class Manager
    {
        public:
            /// Constructor
            Manager(Player * p_Owner);

            /// Create the garrison
            void Create();
            /// Load
            bool Load(PreparedQueryResult p_GarrisonResult, PreparedQueryResult p_BuildingsResult, PreparedQueryResult p_FollowersResult, PreparedQueryResult p_MissionsResult, PreparedQueryResult p_WorkOrderResult);
            /// Save this garrison to DB
            void Save();
            /// Delete garrison
            static void DeleteFromDB(uint64 p_PlayerGUID, SQLTransaction p_Transation);

            /// Update the garrison
            void Update();

            /// Reward garrison cache content
            void RewardGarrisonCache();
            /// Get garrison cache token count
            uint32 GetGarrisonCacheTokenCount();

            /// Get terrain swaps
            void GetTerrainSwaps(std::set<uint32> & p_TerrainSwaps);

            /// Get garrison script
            Interfaces::GarrisonSite * GetGarrisonScript();

            /// When the garrison owner enter in the garrisson (@See Player::UpdateArea)
            void OnPlayerEnter();
            /// When the garrison owner leave the garrisson (@See Player::UpdateArea)
            void OnPlayerLeave();
            /// When the garrison owner started a quest
            void OnQuestStarted(const Quest * p_Quest);
            /// When the garrison owner reward a quest
            void OnQuestReward(const Quest * p_Quest);
            /// When the garrison owner abandon a quest
            void OnQuestAbandon(const Quest * p_Quest);

            /// set last used activation game object
            void SetLastUsedActivationGameObject(uint64 p_Guid);

            /// Get GarrSiteLevelEntry for current garrison
            const GarrSiteLevelEntry * GetGarrisonSiteLevelEntry();
            /// Get Garrison Faction Index
            Factions::Type GetGarrisonFactionIndex();

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
            /// Send mission start failed packet
            void StartMissionFailed(uint32 p_MissionRecID, std::vector<uint64> p_Followers);
            /// Complete a mission
            void CompleteMission(uint32 p_MissionRecID);
            /// Do mission bonus roll
            void DoMissionBonusRoll(uint32 p_MissionRecID);
            /// Set mission has complete
            void SetAllInProgressMissionAsComplete();
            /// Get followers on a mission
            std::vector<GarrisonFollower*> GetMissionFollowers(uint32 p_MissionRecID);
            /// Get mission followers abilities effect
            std::vector<uint32> GetMissionFollowersAbilitiesEffects(uint32 p_MissionRecID);
            /// Get mission followers abilities effect
            std::vector<uint32> GetMissionFollowersAbilitiesEffects(uint32 p_MissionRecID, AbilityEffectTypes::Type p_Type, uint32 p_TargetMask);
            /// Get the mission travel time
            uint32 GetMissionTravelDuration(uint32 p_MissionRecID);
            /// Get the mission duration
            uint32 GetMissionDuration(uint32 p_MissionRecID);
            /// Get mission chest chance
            uint32 GetMissionSuccessChance(uint32 p_MissionRecID);
            /// Get missions
            std::vector<GarrisonMission> GetMissions();
            /// Get all completed missions
            std::vector<GarrisonMission> GetCompletedMissions();

            /// Add follower
            bool AddFollower(uint32 p_FollowerID);
            /// Change follower activation state
            void ChangeFollowerActivationState(uint64 p_FollowerDBID, bool p_Active);
            /// Get followers
            std::vector<GarrisonFollower> GetFollowers();
            /// Get follower
            GarrisonFollower GetFollower(uint32 p_FollowerID);
            /// Get activated followers count
            uint32 GetActivatedFollowerCount();
            /// Get num follower activation remaining
            uint32 GetNumFollowerActivationsRemaining();

            /// Can build building X at slot instance Y
            bool IsBuildingPlotInstanceValid(uint32 p_BuildingRecID, uint32 p_PlotInstanceID);
            /// Player fill all condition
            PurchaseBuildingResults::Type CanPurchaseBuilding(uint32 p_BuildingRecID);
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
            /// Has active building
            bool HasActiveBuilding(uint32 p_BuildingID);
            /// Has building type
            bool HasBuildingType(BuildingType::Type p_BuildingType);
            /// Get building max work order
            uint32 GetBuildingMaxWorkOrder(uint32 p_PlotInstanceID);
            /// Get in progress work order count
            uint32 GetWorkOrderCount(uint32 p_PlotInstanceID);
            /// Start new work order
            uint64 StartWorkOrder(uint32 p_PlotInstanceID, uint32 p_ShipmentID);
            /// Get creature plot instance ID
            uint32 GetCreaturePlotInstanceID(uint64 p_GUID);

            /// Get known blueprints
            std::vector<int32> GetKnownBlueprints();
            /// Learn blue print
            bool LearnBlueprint(uint32 p_BuildingRecID);
            /// Known blue print
            bool KnownBlueprint(uint32 p_BuildingRecID);

            /// Get known specializations
            std::vector<int32> GetKnownSpecializations();

            /// Get work orders
            std::vector<GarrisonWorkOrder> GetWorkOrders();

        public:
            /// Replace garrison script
            void _SetGarrisonScript(Interfaces::GarrisonSite * p_Script)
            {
                m_GarrisonScript = p_Script;
            }

        private:
            /// Init
            void Init();
            /// Init data for level
            void InitDataForLevel();

            /// Init Game objects
            void InitPlots();
            /// Uninit plots
            void UninitPlots();

            /// Update plot gameobject
            void UpdatePlot(uint32 p_PlotInstanceID);

            /// Update garrison stats
            void UpdateStats();

            /// Update buildings
            void UpdateBuildings();
            /// Update followers
            void UpdateFollowers();
            /// Update cache
            void UpdateCache();
            /// Update mission distribution
            void UpdateMissionDistribution();
            /// Update garrison ability
            void UpdateGarrisonAbility();
            /// Update work order
            void UpdateWorkOrders();

        private:
            Player *    m_Owner;            ///< Garrison owner
            uint32      m_ID;               ///< Garrison DB ID
            uint32      m_GarrisonLevel;    ///< Garrison level
            uint32      m_GarrisonLevelID;  ///< Garrison level ID in 
            uint32      m_GarrisonSiteID;   ///< Garrison site ID
            uint32      m_NumFollowerActivation;
            uint32      m_NumFollowerActivationRegenTimestamp;
            uint32      m_CacheLastUsage;
            uint32      m_MissionDistributionLastUpdate;

            uint64      m_LastUsedActivationGameObject;
            uint64      m_CacheGameObjectGUID;

            uint32      m_CacheLastTokenAmount;

            std::vector<GarrisonPlotInstanceInfoLocation>   m_Plots;
            std::vector<GarrisonMission>                    m_Missions;
            std::vector<GarrisonFollower>                   m_Followers;
            std::vector<GarrisonBuilding>                   m_Buildings;
            std::vector<GarrisonWorkOrder>                  m_WorkOrders;
            std::vector<int32>                              m_KnownBlueprints;
            std::vector<int32>                              m_KnownSpecializations;

            std::map<uint32, uint64>                m_PlotsGob;
            std::map<uint32, uint64>                m_PlotsActivateGob;
            std::map<uint32, uint64>                m_PlotsWorkOrderGob;
            std::map<uint32, std::vector<uint64>>   m_PlotsGameObjects;
            std::map<uint32, std::vector<uint64>>   m_PlotsCreatures;

            uint32 m_Stat_MaxActiveFollower;

            Interfaces::GarrisonSite * m_GarrisonScript;

            GarrisonMissionReward m_PendingMissionReward;

    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_MGR_HPP_GARRISON