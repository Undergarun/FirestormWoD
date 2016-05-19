////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef ARCHAEOLOGY_MGR_HPP_INCLUDED
# define ARCHAEOLOGY_MGR_HPP_INCLUDED

# include "Containers.h"

class Player;

namespace MS { namespace Skill { namespace Archaeology
{
    namespace Constants
    {
        enum
        {
            ResearchSitePerContinent = 4,                                                   ///< Player can have at max 4 research site per continent
            ResearchContinentCount   = 6,                                                   ///< Kalimdor, EasternKingdoms, Outland, Northrend, Pandaria, Draenor
            MaxResearchProject       = 20,                                                  ///< Research project max count
            MaxResearchSites         = ResearchSitePerContinent * ResearchContinentCount,   ///< Total max research site per player
            DigSiteFarDist           = 40,                                                  ///< Distance between player and the loot far threshold
            DigSiteMedDist           = 20,                                                  ///< Distance between player and the loot medium threshold
            DigSiteCloseDist         = 8                                                    ///< Distance between player and the loot close threshold
        };
    }

    namespace Continents
    {
        enum
        {
            EasternKingdoms = 0,
            Kalimdor        = 1,
            Outland         = 530,
            Northrend       = 571,
            Pandaria        = 870,
            Draenor         = 1116
        };
    }

    namespace Zones
    {
        enum
        {
            TwilightHightlands  = 4922,
            Hyjal               = 616,
            Uldum               = 5034
        };
    }

    namespace GameObjects
    {
        enum
        {
            DigSite_Far_SurveyBot   = 206590,
            DigSite_Med_SurveyBot   = 206589,
            DigSite_Close_SurveyBot = 204272,
        };
    }

    enum ResearchWithLevelResult
    {
        RS_RESULT_FAIL = 0,
        RS_RESULT_OK = 1,
        RS_RESULT_HIDE = 2,
    };

    /// Player research site
    struct DigitSite
    {
        uint16 SiteID;              ///< Site ID
        uint8 SiteLootCount;        ///< Site loot progression
        uint8 SiteMaxLootCount;     ///< Site loot max
        uint32 LootGameObjectID;    ///< Loot GameObject entry
        float LootGameObjectX;      ///< Loot Pos X
        float LootGameObjectY;      ///< Loot Pos Y
        float LootGameObjectZ;      ///< Loot Pos Z

        /// Reset player research site
        void Reset()
        {
            SiteID = 0;
            LootGameObjectID = 0;
            LootGameObjectX = 0.0f;
            LootGameObjectY = 0.0f;
            LootGameObjectZ = 0.0f;
            SiteLootCount = 0;
            SiteMaxLootCount = 0;
        }
    };

    /// Cost entry for a project
    struct ProjectCost
    {
        /// Constructor
        /// @p_Entry      : ItemID or CurrencyID
        /// @p_Count      : Needed entry count
        /// @p_IsCurrency : p_Entry is a CurrencyID ?
        ProjectCost(uint32 p_Entry, uint32 p_Count, bool p_IsCurrency)
        {
            Entry = p_Entry;
            Count = p_Count;
            currency = p_IsCurrency;
        }

        uint32 Entry;   ///< ItemID or CurrencyID
        uint32 Count;   ///< Needed entry count
        bool currency;  ///< Entry is a CurrencyID ?
    };

    /// Completed project structure
    struct CompletedProject
    {
        uint32 ProjectID;           ///< Project ID
        uint32 CompletionCount;     ///< Completion count
        uint32 FirstCompletedDate;  ///< Date of first completion
    };

    /// Shortcuts
    using SiteSet               = std::set<uint32>;
    using ProjectSet            = std::set<uint32>;
    using ProjectsSetMap        = std::map<uint32, ProjectSet>;
    using ProjectCosts          = std::vector<ProjectCost>;
    using ResearchSiteSet       = std::set<uint32>;
    using ResearchSitesMap      = std::map<uint32, ResearchSiteSet>;
    using ResearchProjectSet    = std::set<uint32>;
    using CompletedProjectMap   = std::map<uint32, CompletedProject>;

    /// Conditions for a research place
    struct ResearchConditions
    {
        uint32 ContinentID;         ///< Continent ID
        uint32 ZoneID;              ///< Zone ID
        uint32 RequiredLevel;       ///< val > 0 : Required level, val == 0 ResearchZoneEntry level used
        uint32 LevelAdditional;     ///< CurrentLevel + LevelAdditional used for checks
        uint32 RequiredSkill;       ///< Required skill value
    };

    static ResearchConditions gResearchConditions[] =
    {
        /// ContinentIDOrZoneID        ZoneID               RequiredLevel LevelAdditional RequiredSkill
        { Continents::EasternKingdoms, 0,                         0,          29,               0 },
        { Continents::EasternKingdoms, Zones::TwilightHightlands, 0,          29,             450 },
        { Continents::Kalimdor,        0,                         0,          29,               0 },
        { Continents::Kalimdor,        Zones::Hyjal,              0,          29,             450 },
        { Continents::Kalimdor,        Zones::Uldum,              0,          29,             450 },
        { Continents::Outland,         0,                        58,           0,             275 },
        { Continents::Northrend,       0,                        68,           0,             350 },
        { Continents::Pandaria,        0,                        85,           0,             500 },
        { Continents::Draenor,         0,                        90,           0,               1 },
    };

    class Manager
    {
        public:
            /// Constructor
            Manager(Player* p_Player);
            /// Destructor
            ~Manager();

            /// Load player Archaeology from database
            /// @p_Result         : Result for table character_archaeology
            /// @p_ResultProjects : Result for table character_archaeology_projects
            /// @p_ResultSites    : Result for table character_archaeology_sites
            void LoadArchaeology(PreparedQueryResult p_Result, PreparedQueryResult p_ResultProjects, PreparedQueryResult p_ResultSites);
            /// Save player Archaeology
            /// @p_Transaction : Destination transaction
            void SaveArchaeology(SQLTransaction& p_Transaction);

            /// Generate research project list for the player
            void GenerateResearchProjects();
            /// Generate research site list for the player
            void GenerateResearchSites();
            /// Generate a research site in a continent
            /// @p_MapID : Continent ID
            void GenerateResearchSiteInContinent(uint32 p_MapID);
            /// Generate research site list in a continent
            /// @p_MapID     : Continent ID
            /// @p_SiteCount : Desired site count
            void GenerateResearchSitesForContinent(uint32 p_MapId, uint32 p_SitesCount);

            /// Propagate research projects into the player dynamic update fields
            void PropagateResearchProjects();
            /// Propagate research sites into the player dynamic update fields
            void PropagateResearchSites();

            /// Get an active site in continent
            /// @p_MapID : Continent ID
            uint16 GetRandomActiveSiteInContinent(uint32 p_MapID);

            /// Get survey bot entry (GoB spawned when the player use his skill)
            /// @p_Orientation : Destination GoB spawn orientation
            uint32 GetSurveyBotEntry(float & p_Orientation);
            /// Is the last found artifact game object
            /// @p_GameObjectEntry : GameObject entry
            bool IsLastArtifactGameObject(uint32 p_GameObjectEntry);
            /// Reset last found artifact game object
            void ResetLastArtifactGameObject();

            /// Add a cost to the current project
            /// @p_Entry      : ItemID or CurrencyID
            /// @p_Count      : Needed entry count
            /// @p_IsCurrency : p_Entry is a CurrencyID ?
            void AddProjectCost(uint32 p_Entry, uint32 p_Count, bool p_IsCurrency);
            /// Clear all cost the current project
            void ClearProjectCost();
            /// Solve a research project
            /// @p_ProjectID : Project ID
            bool SolveResearchProject(uint32 p_ProjectID);

            /// Get the list of all completed projects
            CompletedProjectMap & GetCompletedProjects();

            /// Build & send SMSG_ARCHAEOLOGY_SURVERY_CAST
            void SendArchaeologySurveryCast(bool p_Finished, uint8 p_Count, uint8 p_MaxCount, uint16 p_SiteID);
            /// Build & send SMSG_RESEARCH_COMPLETE
            void SendResearchComplete(uint32 p_ProjectID, uint32 p_FirstCompleted, uint32 p_CompletionCount);

        private:
            /// Check and fix errors for research sites
            void ValidateResearchSites();
            /// Check and fix errors for projects
            void ValidateProjects();
            /// Check that the player have all requirement to complete a project
            bool ValidateCostData();

            /// Check if the player can research in a site id
            /// @p_SiteID : Subject
            ResearchWithLevelResult CanResearchWithLevel(uint32 p_SiteID);
            /// Called when a player complete a research site
            /// @p_SiteID : Completed site ID
            void UseResearchSite(uint32 p_SiteID);
            /// Find current research site with player position
            uint16 GetResearchSiteID();
            /// Check if the player have the specified research site
            /// @p_SiteID      : Subject
            /// @p_ContinentID : Location
            bool HasResearchSite(uint32 p_SiteID, uint32 p_ContinentID);

            /// Check if the player have the specified project
            /// @p_ProjectID : Subject
            bool HasResearchProject(uint32 p_ProjectID) const;

            /// Get random loot for a research site
            /// @p_SiteID : Site ID
            /// @p_Site   : [Out]Current site descriptor
            bool GenerateDigitLoot(uint16 p_SiteID, DigitSite &p_Site);

        private:
            Player*             m_Player;                                               ///< Player instance
            DigitSite           m_DigSites[Archaeology::Constants::MaxResearchSites];   ///< Research sites per continents
            ProjectCosts        m_ProjectCostData;                                      ///< Costs for current project
            ResearchSitesMap    m_ResearchSites;                                        ///< Research sites
            ResearchProjectSet  m_ResearchProjects;                                     ///< Research projects
            CompletedProjectMap m_CompletedProjects;                                    ///< List of completed projects
            uint32              m_LastArtifactGameObjectEntry;                          ///< Last found artifact gameobject entry

            bool _archaeologyChanged;

    };

}   ///< namespace Archaeology
}   ///< namespace Skill
}   ///< namespace MS

#endif  ///< ARCHAEOLOGY_MGR_HPP_INCLUDED
