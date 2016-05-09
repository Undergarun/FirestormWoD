////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
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
            Manager(Player* p_Owner);

            /// Create the garrison
            void Create();
            /// Load
            bool Load(PreparedQueryResult p_GarrisonResult, PreparedQueryResult p_BuildingsResult, PreparedQueryResult p_FollowersResult, PreparedQueryResult p_MissionsResult, PreparedQueryResult p_WorkOrderResult);

            /// Save this garrison to DB
            void Save();

            /// Save Follower(s) to database
            void SaveFollowersToDB(uint32 p_Entry = 0);

            /// Delete garrison
            static void DeleteFromDB(uint64 p_PlayerGUID, SQLTransaction p_Transation);

            /// Update the garrison
            void Update();

            /// Set garrison level
            void SetLevel(uint32 p_Level);

            /// Reward garrison cache content
            void RewardGarrisonCache();
            /// Get garrison cache token count
            uint32 GetGarrisonCacheTokenCount() const;

            /// Get terrain swaps
            void GetTerrainSwaps(std::set<uint32>& p_TerrainSwaps) const;

            /// Get garrison script
            Interfaces::GarrisonSite* GetGarrisonScript() const;

            /// Can upgrade the garrison
            bool CanUpgrade() const;
            /// Upgrade the garrison
            void Upgrade();

            /// When the garrison owner enter in the garrisson (@See Player::UpdateArea)
            void OnPlayerEnter();
            /// When the garrison owner leave the garrisson (@See Player::UpdateArea)
            void OnPlayerLeave();
            /// When the garrison owner started a quest
            void OnQuestStarted(const Quest* p_Quest);
            /// When the garrison owner reward a quest
            void OnQuestReward(const Quest* p_Quest);
            /// When the garrison owner abandon a quest
            void OnQuestAbandon(const Quest* p_Quest);

            /// When the owner player change level
            /// @p_Level : New owner level
            void OnOwnerLevelChange(uint32 p_Level);

            /// set last used activation game object
            void SetLastUsedActivationGameObject(uint64 p_Guid);

            /// Get GarrSiteLevelEntry for current garrison
            const GarrSiteLevelEntry* GetGarrisonSiteLevelEntry() const;
            /// Get Garrison Faction Index
            FactionIndex::Type GetGarrisonFactionIndex() const;

        /// Plot section
        public:
            /// Get plots for level
            std::vector<GarrisonPlotInstanceInfoLocation> GetPlots();
            /// Get plot by position
            GarrisonPlotInstanceInfoLocation GetPlot(float p_X, float p_Y, float p_Z);
            /// Get plot instance plot type
            uint32 GetPlotType(uint32 p_PlotInstanceID) const;
            /// Plot is free ?
            bool PlotIsFree(uint32 p_PlotInstanceID) const;
            /// Has plot instance
            bool HasPlotInstance(uint32 p_PlotInstanceID) const;
            /// Get plot location
            GarrisonPlotInstanceInfoLocation GetPlot(uint32 p_PlotInstanceID) const;
            /// Get plot instance ID by activation game object
            uint32 GetPlotInstanceIDByActivationGameObject(uint64 p_Guid) const;
            /// Return Daily Tavern Datas
            std::vector<uint32>& GetGarrisonTavernDatas() { return m_GarrisonDailyTavernData; };

        /// Mission section
        public:
            /// Add mission
            bool AddMission(uint32 p_MissionRecID);
            /// Player have mission
            bool HasMission(uint32 p_MissionRecID) const;
            /// Start mission
            void StartMission(uint32 p_MissionRecID, std::vector<uint64> p_Followers);
            /// Send mission start failed packet
            void StartMissionFailed(uint32 p_MissionRecID, std::vector<uint64> p_Followers);
            /// Complete a mission
            void CompleteMission(uint32 p_MissionRecID);
            /// Do mission bonus roll
            void DoMissionBonusRoll(uint64 p_MissionTableGUID, uint32 p_MissionRecID);
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
            std::vector<GarrisonMission> GetMissions() const;
            /// Get all completed missions
            std::vector<GarrisonMission> GetCompletedMissions() const;
            /// Get all missions in progress
            std::vector<GarrisonMission> GetPendingMissions() const;
            /// Check if player has pending mission
            bool HasPendingMission(uint32 p_MissionID);
            /// Get mission with mission ID
            GarrisonMission* GetMissionWithID(uint32 p_MissionID);

        /// Follower section
        public:
            /// Add follower
            bool AddFollower(uint32 p_FollowerID);
            /// Assign a follower to a building
            void AssignFollowerToBuilding(uint64 p_FollowerDBID, uint32 p_PlotInstanceID);
            /// Change follower activation state
            void ChangeFollowerActivationState(uint64 p_FollowerDBID, bool p_Active);
            /// Get followers
            std::vector<GarrisonFollower> GetFollowers() const;
            /// Get follower
            GarrisonFollower* GetFollower(uint32 p_FollowerID);
            /// Get follower with Database ID
            GarrisonFollower* GetFollowerWithDatabaseID(uint32 p_FollowerDatabaseID);
            /// Get activated followers count
            uint32 GetActiveFollowerCount(uint32 p_FollowerType) const;
            /// Get num follower activation remaining
            uint32 GetNumFollowerActivationsRemaining() const;
            /// Returns error message of upgrade attempt
            SpellCastResult CanUpgradeItemLevelWith(uint32 p_FollowerID, SpellInfo const* p_SpellInfo) const;
            /// Upgrades follower with spell
            void UpgradeFollowerItemLevelWith(uint32 p_FollowerID, SpellInfo const* p_SpellInfo);
            /// Check if any followers has ability in parameter
            bool HasFollowerAbility(uint32 p_AbilityID) const;

        /// Blueprint section
        public:
            /// Get known blueprints
            std::vector<int32> GetKnownBlueprints() const;
            /// Learn blue print
            bool LearnBlueprint(uint32 p_BuildingRecID);
            /// Known blue print
            bool KnownBlueprint(uint32 p_BuildingRecID) const;

        /// Building section
        public:
            /// Can build building X at slot instance Y
            bool IsBuildingPlotInstanceValid(uint32 p_BuildingRecID, uint32 p_PlotInstanceID) const;
            /// Player fill all condition
            PurchaseBuildingResults::Type CanPurchaseBuilding(uint32 p_BuildingRecID) const;
            /// PurchaseBuilding
            GarrisonBuilding PurchaseBuilding(uint32 p_BuildingRecID, uint32 p_PlotInstanceID, bool p_Triggered = false);
            /// Get building
            GarrisonBuilding GetBuilding(uint32 p_PlotInstanceID) const;
            /// Get Building with ID
            GarrisonBuilding GetBuildingWithBuildingID(uint32 p_BuildingID) const;
            /// Get building object
            GarrisonBuilding* GetBuildingObject(uint32 p_PlotInstanceID);
            /// Get buildings
            std::vector<GarrisonBuilding> GetBuildings() const;
            /// Get building passive ability effects
            std::vector<uint32> GetBuildingsPassiveAbilityEffects() const;
            /// Activate building
            void ActivateBuilding(uint32 p_PlotInstanceID);
            /// Activate building
            void ActivateBuilding();
            /// Cancel construction
            void CancelConstruction(uint32 p_PlotInstanceID);
            /// Delete building
            void DeleteBuilding(uint32 p_PlotInstanceID, bool p_Canceled, bool p_RemoveForUpgrade);
            /// Has active building
            bool HasActiveBuilding(uint32 p_BuildingID) const;
            /// Has building type
            bool HasBuildingType(BuildingType::Type p_BuildingType, bool p_DontNeedActive = false) const;
            /// Get building with type
            GarrisonBuilding GetBuildingWithType(BuildingType::Type p_BuildingType, bool p_DontNeedActive = false) const;
            /// Get Level of the building
            uint32 GetBuildingLevel(GarrisonBuilding p_Building) const;
            /// Get building max work order
            uint32 GetBuildingMaxWorkOrder(uint32 p_PlotInstanceID) const;
            /// Get in progress work order count
            uint32 GetWorkOrderCount(uint32 p_PlotInstanceID) const;
            /// Start new work order
            uint64 StartWorkOrder(uint32 p_PlotInstanceID, uint32 p_ShipmentID);
            /// Delete work order
            void DeleteWorkOrder(uint64 p_DBID);
            /// Calculate the chance to double the work order of the building, depending of the follower
            uint8 CalculateAssignedFollowerShipmentBonus(uint32 p_PlotInstanceID);
            /// Generates random reward for Armory work order
            uint32 CalculateArmoryWorkOrder() const;
            /// Get follower assigned to building from plot instance ID
            GarrisonFollower* GetAssignedFollower(uint32 p_PlotInstanceID);
            /// Checks if the building has the required follower assigned to apply bonus
            bool HasRequiredFollowerAssignedAbility(uint32 p_PlotInstanceID);
            /// Add new creature in plot datas, that way any summoned creature can be detected as part of the building
            void InsertNewCreatureInPlotDatas(uint32 p_PlotInstanceID, uint64 p_Guid);
            /// Get creature plot instance ID
            uint32 GetCreaturePlotInstanceID(uint64 p_GUID) const;
            /// Get gameobject plot instance ID
            uint32 GetGameObjectPlotInstanceID(uint64 p_GUID) const;
            /// Get building gathering data
            /// @p_PlotInstanceID : Plot building location
            std::string GetBuildingGatheringData(uint32 p_PlotInstanceID);
            /// Set building gathering data
            /// @p_PlotInstanceID   : Plot building location
            /// @p_Data             : Gathering data
            void SetBuildingGatheringData(uint32 p_PlotInstanceID, std::string p_Data);
            /// Get list of creature in a specific building type
            /// @p_Type : Building type
            std::vector<uint64> GetBuildingCreaturesByBuildingType(BuildingType::Type p_Type);
            /// Get Garrison ID
            uint32 GetGarrisonID() { return m_ID; };
            /// Get Garrison Level
            uint32 GetGarrisonLevel() { return m_GarrisonLevel; };
            /// Check if the players has the right mount
            bool CheckGarrisonStablesQuestsConditions(uint32 p_QuestID, Player* p_Player);
            /// Checks training mounts auras
            bool IsTrainingMount();

        /// Tavern System
        public:
            void AddGarrisonTavernData(uint32 p_Data);
            void SetGarrisonTavernData(uint32 p_Data);
            void CleanGarrisonTavernData();

            /// Get known specializations
            std::vector<int32> GetKnownSpecializations() const;

            /// Get work orders
            std::vector<GarrisonWorkOrder>& GetWorkOrders();

            /// Update mission distribution
            void UpdateMissionDistribution(bool p_Force = false, uint32 p_ForcedCount = 0);

            /// Renames the specified follower
            bool RenameFollower(uint32 p_DatabaseID, std::string p_FollowerName);

            /// Removes the follower
            bool RemoveFollower(uint32 p_DatabaseID, bool p_Force = false);

            /// Gets the follower count of specified type
            uint32 GetTotalFollowerCount(uint32 p_Type);

            /// Sends a packet to owner
            void SendPacketToOwner(WorldPacket* p_Data);

            /// Checks if ship has crew
            bool HasCrewAbility(GarrisonFollower const& p_Follower) const;

            /// Generates follower ability (crew) for the specified shit
            uint32 GenerateCrewAbilityIdForShip(GarrisonFollower const& p_Follower);

            /// Generate random NPC Ability
            uint32 GenerateRandomAbility(GarrisonFollower* p_Follower);

            /// Generate random trait
            uint32 GenerateRandomTrait(uint32 p_Type, std::vector<uint32> const& p_KnownAbilities);

            /// Reroll Follower Abilities
            void GenerateFollowerAbilities(GarrisonFollower& p_Follower, bool p_Reset = true, bool p_Abilities = true, bool p_Traits = true, bool p_Update = false);

            /// Reroll Follower Abilities
            void GenerateFollowerAbilities(uint32 p_FollowerID, bool p_Reset = true, bool p_Abilities = true, bool p_Traits = true, bool p_Update = false);

            /// Generates the followers list
            std::list<std::string> GenerateFollowerTextList(uint32 l_Type);

            /// Levelup follower
            bool LevelUpFollower(uint32 p_DatabaseID);

            /// Returns all the  abilities with the same type
            uint32 CountFollowerAbilitiesByType(uint32 p_FollowerID, uint32 p_Type) const;

            /// Returns error message of the attempt
            SpellCastResult CanLearnTrait(uint32 p_FollowerID, uint32 p_Slot, SpellInfo const* p_SpellInfo, uint32 p_EffIndex) const;

            /// Learns the specific trait
            void LearnFollowerTrait(uint32 p_FollowerID, uint32 p_Slot, SpellInfo const* p_SpellInfo, uint32 p_EffIndex);

            /// Returns the cap of the specified follower type
            uint32 GetFollowerSoftCap(uint32 p_FollowerType) const;

            /// Returns if the mission with specified follower type can be started after soft cap
            bool CanMissionBeStartedAfterSoftCap(uint32 p_FollowerType) const;

            /// Returns if the specific follower type can be renamed
            bool CanRenameFollowerType(uint32 p_FollowerType) const;

            /// Returns max follower item level that can be achieved by the specific follower type
            uint32 GetMaxFollowerItemLevel(uint32 p_FollowerType) const;

            /// Returns if player has shipyard or not
            bool HasShipyard() const;

            /// Returns the shipyard level
            uint32 GetShipyardLevel() const;

            /// Returns mapId of shipyard
            uint32 GetShipyardMapId() const;

            /// Get shipyard terain swap
            void GetShipyardTerainSwaps(std::set<uint32>& p_TerrainSwaps) const;

            /// Create shipyard by spell
            bool CreateShipyardBySpell();

            /// Update plot gameobject
            void UpdatePlot(uint32 p_PlotInstanceID);

        public:
            /// Replace garrison script
            void _SetGarrisonScript(Interfaces::GarrisonSite* p_Script)
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

            /// Update garrison stats
            void UpdateStats();

            /// Update buildings
            void UpdateBuildings();
            /// Update followers
            void UpdateFollowers();
            /// Update cache
            void UpdateCache();
            /// Update garrison ability
            void UpdateGarrisonAbility();
            /// Update work order
            void UpdateWorkOrders();

        private:
            Player*     m_Owner;            ///< Garrison owner
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
            std::vector<uint32>                             m_GarrisonDailyTavernData;

            std::map<uint32, uint64>                m_PlotsGob;
            std::map<uint32, uint64>                m_PlotsActivateGob;
            std::map<uint32, uint64>                m_PlotsWorkOrderGob;
            std::map<uint32, std::vector<uint64>>   m_PlotsGameObjects;
            std::map<uint32, std::vector<uint64>>   m_PlotsCreatures;
            std::map<uint32, uint32>                m_LastPlotBuildingType; ///< <PlotID, BuildingType>

            Interfaces::GarrisonSite* m_GarrisonScript;

            GarrisonMissionReward m_PendingMissionReward;

    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_MGR_HPP_GARRISON