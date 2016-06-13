////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ObjectMgr.h"
#include "ArchaeologyMgr.hpp"
#include "Player.h"

const static int q_patt[2][2] = { { 0, 1 }, { 3, 2 } };

namespace JadeCore
{
    bool IsPointInZone(const ResearchPOIPoint &test, const ResearchPOIPoints &polygon)
    {
        if (polygon.size() < 3)
            return false;

        ResearchPOIPoints::const_iterator end = polygon.end();
        ResearchPOIPoint pred_pt = polygon.back();
        pred_pt.x -= test.x;
        pred_pt.y -= test.y;

        int pred_q = q_patt[pred_pt.y < 0][pred_pt.x<  0];

        int w = 0;

        for (std::vector<ResearchPOIPoint>::const_iterator iter = polygon.begin(); iter != end; ++iter)
        {
            ResearchPOIPoint cur_pt = *iter;

            cur_pt.x -= test.x;
            cur_pt.y -= test.y;

            int q = q_patt[cur_pt.y < 0][cur_pt.x < 0];

            switch (q - pred_q)
            {
                case -3:
                    ++w;
                    break;
                case 3:
                    --w;
                    break;
                case -2:
                    if (pred_pt.x * cur_pt.y >= pred_pt.y * cur_pt.x)
                        ++w;
                    break;
                case 2:
                    if (!(pred_pt.x * cur_pt.y >= pred_pt.y * cur_pt.x))
                        --w;
                    break;
            }

            pred_pt = cur_pt;
            pred_q = q;
        }
        return w != 0;
    }
}

namespace MS { namespace Skill { namespace Archaeology
{
    /// Constructor
    Manager::Manager(Player* p_Player)
        : m_Player(p_Player), m_LastArtifactGameObjectEntry(0)
    {
        for (uint8 l_I = 0; l_I < Archaeology::Constants::MaxResearchSites; ++l_I)
            m_DigSites[l_I].Reset();
    }

    /// Destructor
    Manager::~Manager()
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Load player Archaeology from database
    /// @p_Result         : Result for table character_archaeology
    /// @p_ResultProjects : Result for table character_archaeology_projects
    /// @p_ResultSites    : Result for table character_archaeology_sites
    void Manager::LoadArchaeology(PreparedQueryResult p_Result, PreparedQueryResult p_ResultProjects, PreparedQueryResult p_ResultSites)
    {
        if (!m_Player->GetSkillValue(SKILL_ARCHAEOLOGY))
            return;

        for (uint8 l_I = 0; l_I < Archaeology::Constants::MaxResearchSites; ++l_I)
            m_DigSites[l_I].Reset();

        if (!p_Result)
            return;

        Field * l_Fields = p_Result->Fetch();

        m_ResearchSites.clear();

        /// Loading current zones
        if (p_ResultSites)
        {
            do
            {
                Field* l_SiteFields = p_ResultSites->Fetch();

                uint32 l_MapID = l_SiteFields[0].GetUInt32();
                Tokenizer l_Tokens(l_SiteFields[1].GetCString(), ' ', Archaeology::Constants::ResearchSitePerContinent);

                if (l_Tokens.size() == Archaeology::Constants::ResearchSitePerContinent)
                {
                    for (uint8 l_I = 0; l_I < l_Tokens.size(); ++l_I)
                        m_ResearchSites[l_MapID].insert(uint32(atoi(l_Tokens[l_I])));
                }
            } while (p_ResultSites->NextRow());
        }

        ValidateResearchSites();

        /// Loading current zone info
        if (l_Fields[0].GetCString())
        {
            Tokenizer l_Tokens(l_Fields[0].GetCString(), ' ', Archaeology::Constants::MaxResearchSites);

            if (l_Tokens.size() == Archaeology::Constants::MaxResearchSites)
            {
                for (uint8 l_I = 0; l_I < Archaeology::Constants::MaxResearchSites; ++l_I)
                    m_DigSites[l_I].SiteLootCount = uint32(atoi(l_Tokens[l_I]));
            }
        }

        PropagateResearchSites();

        /// Loading current projects
        m_ResearchProjects.clear();
        if (l_Fields[1].GetCString())
        {
            Tokenizer l_Tokens(l_Fields[1].GetCString(), ' ');

            for (uint8 l_I = 0; l_I < l_Tokens.size() && l_I < Constants::MaxResearchProject; ++l_I)
                m_ResearchProjects.insert(uint32(atoi(l_Tokens[l_I])));
        }

        ValidateProjects();
        PropagateResearchProjects();

        if (!p_ResultProjects)
            return;

        m_CompletedProjects.clear();

        do
        {
            Field* l_ProjectsFields = p_ResultProjects->Fetch();

            CompletedProject l_CompletedProjectEntry;
            uint32 l_ProjectID = l_ProjectsFields[0].GetUInt32();

            l_CompletedProjectEntry.ProjectID           = l_ProjectID;
            l_CompletedProjectEntry.CompletionCount     = l_ProjectsFields[1].GetUInt32();
            l_CompletedProjectEntry.FirstCompletedDate  = l_ProjectsFields[2].GetUInt32();

            if (m_CompletedProjects.find(l_ProjectID) == m_CompletedProjects.end())
                m_CompletedProjects.insert(std::make_pair(l_ProjectID, l_CompletedProjectEntry));
        } while (p_ResultProjects->NextRow());
    }

    /// Save player Archaeology
    /// @p_Transaction : Destination transaction
    void Manager::SaveArchaeology(SQLTransaction& p_Transaction)
    {
        if (!m_Player->GetSkillValue(SKILL_ARCHAEOLOGY))
            return;

        if (!_archaeologyChanged)
            return;

        std::ostringstream l_StringStream;

        /// character_archaeology
        PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_ARCHAEOLOGY);
        l_Statement->setUInt32(0, m_Player->GetGUIDLow());
        p_Transaction->Append(l_Statement);

        l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_INS_PLAYER_ARCHAEOLOGY);
        l_Statement->setUInt32(0, m_Player->GetGUIDLow());

        for (uint8 l_J = 0; l_J < Archaeology::Constants::MaxResearchSites; ++l_J)
            l_StringStream << uint32(m_DigSites[l_J].SiteLootCount) << " ";

        l_Statement->setString(1, l_StringStream.str().c_str());
        l_StringStream.str("");

        for (ResearchProjectSet::const_iterator l_Pair = m_ResearchProjects.begin(); l_Pair != m_ResearchProjects.end(); ++l_Pair)
            l_StringStream << (*l_Pair) << " ";

        l_Statement->setString(2, l_StringStream.str().c_str());
        p_Transaction->Append(l_Statement);

        /// character_archaeology_sites
        l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_ARCHAEOLOGY_SITES);
        l_Statement->setUInt32(0, m_Player->GetGUIDLow());
        p_Transaction->Append(l_Statement);

        for (ResearchSitesMap::const_iterator l_Iterator = m_ResearchSites.begin(); l_Iterator != m_ResearchSites.end(); ++l_Iterator)
        {
            if (l_Iterator->second.empty())
                continue;

            l_StringStream.str("");

            l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_INS_PLAYER_ARCHAEOLOGY_SITES);
            l_Statement->setUInt32(0, m_Player->GetGUIDLow());
            l_Statement->setUInt32(1, l_Iterator->first);

            for (ResearchSiteSet::const_iterator l_Iterator2 = l_Iterator->second.begin(); l_Iterator2 != l_Iterator->second.end(); ++l_Iterator2)
                l_StringStream << (*l_Iterator2) << " ";

            l_Statement->setString(2, l_StringStream.str().c_str());

            p_Transaction->Append(l_Statement);
        }

        /// character_archaeology_projetcs
        l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_ARCHAEOLOGY_PROJECTS);
        l_Statement->setUInt32(0, m_Player->GetGUIDLow());
        p_Transaction->Append(l_Statement);

        for (auto l_Pair : m_CompletedProjects)
        {
            l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_INS_PLAYER_ARCHAEOLOGY_PROJECTS);
            l_Statement->setUInt32(0, m_Player->GetGUIDLow());
            l_Statement->setUInt32(1, l_Pair.first);
            l_Statement->setUInt32(2, l_Pair.second.CompletionCount);
            l_Statement->setUInt32(3, l_Pair.second.FirstCompletedDate);
            p_Transaction->Append(l_Statement);
        }

        _archaeologyChanged = false;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Generate research project list for the player
    void Manager::GenerateResearchProjects()
    {
        if (sResearchProjectSet.empty())
            return;

        if (m_ResearchProjects.size() > Constants::MaxResearchProject)
            return;

        uint16 l_CurrentSkillValue = m_Player->GetSkillValue(SKILL_ARCHAEOLOGY);
        if (!l_CurrentSkillValue)
            return;

        ProjectsSetMap l_CandidatesProjectListPerBranch;
        uint32 l_ProjectBaseChance = l_CurrentSkillValue / 10;

        for (std::set<ResearchProjectEntry const*>::const_iterator l_It = sResearchProjectSet.begin(); l_It != sResearchProjectSet.end(); ++l_It)
        {
            ResearchProjectEntry const* l_ProjectEntry = (*l_It);

            if (!l_ProjectEntry)
                continue;

            auto l_SecondIt = std::find_if(m_ResearchProjects.begin(), m_ResearchProjects.end(), [l_ProjectEntry](uint32 const& p_ProjectID) -> bool
            {
                ResearchProjectEntry const* l_Second = sResearchProjectStore.LookupEntry(p_ProjectID);

                if (!l_Second || l_Second->branchId != l_ProjectEntry->branchId)
                    return false;

                return true;
            });

            if (l_SecondIt != m_ResearchProjects.end())
                continue;

            if ((l_ProjectEntry->rare && !roll_chance_i(l_ProjectBaseChance)))
                continue;

            l_CandidatesProjectListPerBranch[l_ProjectEntry->branchId].insert(l_ProjectEntry->ID);
        }

        for (uint32 l_I = 0; l_I < sResearchBranchStore.GetNumRows(); ++l_I)
        {
            ResearchBranchEntry const* l_Entry = sResearchBranchStore.LookupEntry(l_I);

            if (!l_Entry)
                continue;

            if (l_CandidatesProjectListPerBranch.find(l_Entry->ID) == l_CandidatesProjectListPerBranch.end())
                continue;

            if (!m_Player->GetCurrency(l_Entry->CurrencyID, sCurrencyTypesStore.LookupEntry(l_Entry->CurrencyID)->HasPrecision()))
                continue;

            ProjectSet& l_BranchPorjects = l_CandidatesProjectListPerBranch[l_Entry->ID];
            if (l_BranchPorjects.empty())
                continue;

            ProjectSet::iterator l_It = l_BranchPorjects.begin();
            std::advance(l_It, rand() % l_BranchPorjects.size());

            if (m_ResearchProjects.size() < Constants::MaxResearchProject)
                m_ResearchProjects.emplace(*l_It);
        }

        _archaeologyChanged = true;

        PropagateResearchProjects();
    }

    /// Generate research site list for the player
    void Manager::GenerateResearchSites()
    {
        if (sResearchSiteSet.empty())
            return;

        for (ResearchSitesMap::iterator l_Iterator = m_ResearchSites.begin(); l_Iterator != m_ResearchSites.end(); ++l_Iterator)
            l_Iterator->second.clear();

        for (std::set<ResearchSiteEntry const*>::const_iterator l_Iterator = sResearchSiteSet.begin(); l_Iterator != sResearchSiteSet.end(); ++l_Iterator)
        {
            if (CanResearchWithLevel((*l_Iterator)->ID))
                m_ResearchSites[(*l_Iterator)->mapId].insert((*l_Iterator)->ID);
        }

        for (ResearchSitesMap::iterator l_Iterator = m_ResearchSites.begin(); l_Iterator != m_ResearchSites.end(); ++l_Iterator)
            JadeCore::Containers::RandomResizeSet(l_Iterator->second, Archaeology::Constants::ResearchSitePerContinent);

        _archaeologyChanged = true;
        PropagateResearchSites();
    }

    /// Generate a research site in a continent
    /// @p_MapID : Continent ID
    void Manager::GenerateResearchSiteInContinent(uint32 p_MapID)
    {
        if (sResearchSiteSet.empty())
            return;

        SiteSet l_TempSites;
        for (std::set<ResearchSiteEntry const*>::const_iterator l_It = sResearchSiteSet.begin(); l_It != sResearchSiteSet.end(); ++l_It)
        {
            ResearchSiteEntry const* l_Entry = (*l_It);

            if (!HasResearchSite(l_Entry->ID, p_MapID) && l_Entry->mapId == p_MapID && CanResearchWithLevel(l_Entry->ID))
                l_TempSites.insert(l_Entry->ID);
        }

        if (l_TempSites.empty())
            return;

        m_ResearchSites[p_MapID].insert(JadeCore::Containers::SelectRandomContainerElement(l_TempSites));
        _archaeologyChanged = true;

        PropagateResearchSites();
    }

    /// Generate research site list in a continent
    /// @p_MapID     : Continent ID
    /// @p_SiteCount : Desired site count
    void Manager::GenerateResearchSitesForContinent(uint32 p_MapId, uint32 p_SitesCount)
    {
        if (sResearchSiteSet.empty())
            return;

        m_ResearchSites[p_MapId].clear();

        for (std::set<ResearchSiteEntry const*>::const_iterator l_Iterator = sResearchSiteSet.begin(); l_Iterator != sResearchSiteSet.end(); ++l_Iterator)
        {
            if ((*l_Iterator)->mapId != p_MapId)
                continue;

            if (CanResearchWithLevel((*l_Iterator)->ID))
                m_ResearchSites[p_MapId].insert((*l_Iterator)->ID);
        }

        JadeCore::Containers::RandomResizeSet(m_ResearchSites[p_MapId], p_SitesCount);

        _archaeologyChanged = true;
        PropagateResearchSites();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Propagate research projects into the player dynamic update fields
    void Manager::PropagateResearchSites()
    {
        if (!m_Player->GetSkillValue(SKILL_ARCHAEOLOGY))
            return;

        uint8 l_Count = 0;

        ResearchSiteSet l_TempSet;
        for (ResearchSitesMap::const_iterator l_Iterator = m_ResearchSites.begin(); l_Iterator != m_ResearchSites.end(); ++l_Iterator)
        {
            for (ResearchSiteSet::const_iterator l_Iterator2 = l_Iterator->second.begin(); l_Iterator2 != l_Iterator->second.end(); ++l_Iterator2)
                l_TempSet.insert((*l_Iterator2));
        }

        for (ResearchSiteSet::const_iterator l_It = l_TempSet.begin(); l_It != l_TempSet.end(); ++l_It)
        {
            uint32 l_ID = (*l_It);

            if (CanResearchWithLevel(l_ID) == RS_RESULT_HIDE)
                l_ID = 0;

            m_Player->SetDynamicValue(PLAYER_DYNAMIC_FIELD_RESEARCH_SITES, l_Count, l_ID);

            for (uint8 i = 0; i < Archaeology::Constants::MaxResearchSites; ++i)
            {
                if (m_DigSites[i].SiteID == l_ID)
                {
                    m_Player->SetDynamicValue(PLAYER_DYNAMIC_FIELD_RESEARCH_SITES + 1, l_Count, m_DigSites[i].SiteLootCount);
                    break;
                }
                else
                    m_Player->SetDynamicValue(PLAYER_DYNAMIC_FIELD_RESEARCH_SITES + 1, l_Count, 0);
            }

            ++l_Count;
        }
    }

    /// Propagate research sites into the player dynamic update fields
    void Manager::PropagateResearchProjects()
    {
        /// Reset research project
        for (uint32 l_I = 0; l_I < (PLAYER_FIELD_PROFESSION_SKILL_LINE - PLAYER_FIELD_RESEARCHING); ++l_I)
            m_Player->SetUInt32Value(PLAYER_FIELD_RESEARCHING + l_I, 0);

        if (!m_Player->GetSkillValue(SKILL_ARCHAEOLOGY))
            return;

        uint8 l_Count = 0;
        for (ResearchProjectSet::const_iterator l_It = m_ResearchProjects.begin(); l_It != m_ResearchProjects.end() && l_Count < 20; ++l_It)
        {
            m_Player->SetUInt16Value(PLAYER_FIELD_RESEARCHING + (l_Count / 2), l_Count % 2, (*l_It));
            ++l_Count;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get an active site in continent
    /// @p_MapID : Continent ID
    uint16 Manager::GetRandomActiveSiteInContinent(uint32 mapId)
    {
        std::list<uint16> l_SitesID;

        for (uint16 l_SiteID : m_ResearchSites[mapId])
            l_SitesID.push_back(l_SiteID);

        if (l_SitesID.empty())
            return 0;

        JadeCore::Containers::RandomResizeList(l_SitesID, 1);
        return l_SitesID.front();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get survey bot entry (GoB spawned when the player use his skill)
    /// @p_Orientation : Destination GoB spawn orientation
    uint32 Manager::GetSurveyBotEntry(float & p_Orientation)
    {
        uint16 l_SkillValue = m_Player->GetSkillValue(SKILL_ARCHAEOLOGY);
        if (!l_SkillValue)
            return 0;

        uint16 l_ResearchSiteID = GetResearchSiteID();
        if (!l_ResearchSiteID)
            return 0;

        uint32 l_AtPos = 0xFFFFF;

        for (uint8 l_I = 0; l_I < Archaeology::Constants::MaxResearchSites; ++l_I)
        {
            //Replace by GetUInt16Value
            uint32 l_FirstCurrentSite  = (m_Player->GetDynamicValue(PLAYER_DYNAMIC_FIELD_RESEARCH_SITES, l_I) >>  0) & 0xFFFFF;
            uint32 l_SecondCurrentSite = (m_Player->GetDynamicValue(PLAYER_DYNAMIC_FIELD_RESEARCH_SITES, l_I) >> 20) & 0xFFFFF;

            if (l_ResearchSiteID == l_FirstCurrentSite)
            {
                l_AtPos = l_I;
                break;
            }
            if (l_ResearchSiteID == l_SecondCurrentSite)
            {
                l_AtPos = l_I + 1;
                break;
            }
        }

        if (l_AtPos == 0xFFFFF)
        {
            sLog->outAshran("ArcheologyMgr::GetSurveyBotEntry, l_AtPos (%u) = 0xFFFFF for site %u !", l_AtPos, l_ResearchSiteID);
            return 0;
        }

        if (l_AtPos >= Archaeology::Constants::MaxResearchSites)
        {
            sLog->outAshran("ArcheologyMgr::GetSurveyBotEntry, l_AtPos (%u) >= 20 for site %u !", l_AtPos, l_ResearchSiteID);
            return 0;
        }

        DigitSite &l_DigSite = m_DigSites[l_AtPos];
        if (l_DigSite.SiteID != l_ResearchSiteID)
        {
            if (!GenerateDigitLoot(l_ResearchSiteID, l_DigSite))
                return 0;

            l_DigSite.SiteID = l_ResearchSiteID;
        }

        p_Orientation = m_Player->GetAngle(l_DigSite.LootGameObjectX, l_DigSite.LootGameObjectY);
        float l_CurrentDistance = m_Player->GetDistance2d(l_DigSite.LootGameObjectX, l_DigSite.LootGameObjectY);

        if (l_CurrentDistance >= Archaeology::Constants::DigSiteFarDist)
        {
            SendArchaeologySurveryCast(false, l_DigSite.SiteLootCount, l_DigSite.SiteMaxLootCount, l_DigSite.SiteID);
            return Archaeology::GameObjects::DigSite_Far_SurveyBot;
        }
        if (l_CurrentDistance >= Archaeology::Constants::DigSiteMedDist)
        {
            SendArchaeologySurveryCast(false, l_DigSite.SiteLootCount, l_DigSite.SiteMaxLootCount, l_DigSite.SiteID);
            return Archaeology::GameObjects::DigSite_Med_SurveyBot;
        }
        if (l_CurrentDistance >= Archaeology::Constants::DigSiteCloseDist)
        {
            SendArchaeologySurveryCast(false, l_DigSite.SiteLootCount, l_DigSite.SiteMaxLootCount, l_DigSite.SiteID);
            return Archaeology::GameObjects::DigSite_Close_SurveyBot;
        }

        m_Player->SummonGameObject(l_DigSite.LootGameObjectID, l_DigSite.LootGameObjectX, l_DigSite.LootGameObjectY, l_DigSite.LootGameObjectZ, 0, 0, 0, 0, 0, 30000);
        m_LastArtifactGameObjectEntry = l_DigSite.LootGameObjectID;

        if (l_DigSite.SiteLootCount < l_DigSite.SiteMaxLootCount)
        {
            SendArchaeologySurveryCast(true, l_DigSite.SiteLootCount, l_DigSite.SiteMaxLootCount, l_DigSite.SiteID);
            l_DigSite.SiteLootCount++;

            if (!GenerateDigitLoot(l_ResearchSiteID, l_DigSite))
                return 0;

            PropagateResearchSites();
        }
        else
        {
            SendArchaeologySurveryCast(true, (l_DigSite.SiteLootCount + 1), l_DigSite.SiteMaxLootCount, l_DigSite.SiteID);
            l_DigSite.Reset();
            UseResearchSite(l_ResearchSiteID);
        }

        _archaeologyChanged = true;

        return 0;
    }

    /// Is the last found artifact game object
    /// @p_GameObjectEntry : GameObject entry
    bool Manager::IsLastArtifactGameObject(uint32 p_GameObjectEntry)
    {
        return m_LastArtifactGameObjectEntry == p_GameObjectEntry;
    }

    /// Reset last found artifact game object
    void Manager::ResetLastArtifactGameObject()
    {
        m_LastArtifactGameObjectEntry = 0;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Add a cost to the current project
    /// @p_Entry      : ItemID or CurrencyID
    /// @p_Count      : Needed entry count
    /// @p_IsCurrency : p_Entry is a CurrencyID ?
    void Manager::AddProjectCost(uint32 p_Entry, uint32 p_Count, bool p_IsCurrency)
    {
        m_ProjectCostData.push_back(ProjectCost(p_Entry, p_Count, p_IsCurrency));
    }

    /// Clear all cost the current project
    void Manager::ClearProjectCost()
    {
        m_ProjectCostData.clear();
    }

    /// Solve a research project
    /// @p_ProjectID : Project ID
    bool Manager::SolveResearchProject(uint32 p_ProjectID)
    {
        if (!p_ProjectID)
            return false;

        /// Check for skill
        uint16 l_SkillValue = m_Player->GetSkillValue(SKILL_ARCHAEOLOGY);
        if (!l_SkillValue)
            return false;

        /// Check for correct cost data
        if (!ValidateCostData())
            return false;

        /// Check for project id
        ResearchProjectEntry const* l_ResearchProjectEntry = NULL;
        for (std::set<ResearchProjectEntry const*>::const_iterator l_It = sResearchProjectSet.begin(); l_It != sResearchProjectSet.end(); ++l_It)
        {
            if ((*l_It)->ID == p_ProjectID)
            {
                l_ResearchProjectEntry = (*l_It);
                break;
            }
        }

        if (!l_ResearchProjectEntry)
            return false;

        /// Check for existing project
        if (!HasResearchProject(p_ProjectID))
            return false;

        /// All done
        for (std::vector<ProjectCost>::const_iterator l_It = m_ProjectCostData.begin(); l_It != m_ProjectCostData.end(); ++l_It)
        {
            if ((*l_It).currency)
                m_Player->ModifyCurrency((*l_It).Entry, -int32((*l_It).Count));
            else
                m_Player->DestroyItemCount((*l_It).Entry, (*l_It).Count, true);
        }

        ClearProjectCost();

        m_ResearchProjects.erase(p_ProjectID);

        m_Player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ARCHAEOLOGY_PROJECTS, p_ProjectID, 1);

        uint32 l_FirstCompleted = 0;
        uint32 l_CompletionCount = 0;

        /// Already Completed
        if (m_CompletedProjects.find(p_ProjectID) != m_CompletedProjects.end())
        {
            ++m_CompletedProjects[p_ProjectID].CompletionCount;

            l_FirstCompleted  = m_CompletedProjects[p_ProjectID].FirstCompletedDate;
            l_CompletionCount = m_CompletedProjects[p_ProjectID].CompletionCount;
        }
        else
        {
            CompletedProject l_NewCompletedProjectEntry;
            l_NewCompletedProjectEntry.CompletionCount = 1;
            l_NewCompletedProjectEntry.FirstCompletedDate = time(NULL);

            m_CompletedProjects.insert(std::make_pair(p_ProjectID, l_NewCompletedProjectEntry));

            l_FirstCompleted  = l_NewCompletedProjectEntry.FirstCompletedDate;
            l_CompletionCount = l_NewCompletedProjectEntry.CompletionCount;
        }

        SendResearchComplete(p_ProjectID, l_FirstCompleted, l_CompletionCount);

        /// Source : http://www.wow-professions.com/wowguides/wow-archaeology-guide.html
        m_Player->UpdateSkill(SKILL_ARCHAEOLOGY, l_ResearchProjectEntry->rare ? 15 : 5);

        // Add new project
        ProjectSet l_CandidateProjectsListPerBranch;
        uint32 l_Chance = l_SkillValue / 10;

        for (std::set<ResearchProjectEntry const*>::const_iterator l_It = sResearchProjectSet.begin(); l_It != sResearchProjectSet.end(); ++l_It)
        {
            if ((*l_It)->branchId == l_ResearchProjectEntry->branchId)
            {
                if (((*l_It)->rare && !roll_chance_i(l_Chance)))
                    continue;

                l_CandidateProjectsListPerBranch.insert((*l_It)->ID);
            }
        }

        m_ResearchProjects.insert(JadeCore::Containers::SelectRandomContainerElement(l_CandidateProjectsListPerBranch));
        _archaeologyChanged = true;

        PropagateResearchProjects();
        return true;
    }

    /// Get the list of all completed projects
    CompletedProjectMap & Manager::GetCompletedProjects()
    {
        return m_CompletedProjects;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Build & send SMSG_ARCHAEOLOGY_SURVERY_CAST
    void Manager::SendArchaeologySurveryCast(bool p_Finished, uint8 p_Count, uint8 p_MaxCount, uint16 p_SiteID)
    {
        uint32 l_ResearchBranchID = 0;

        ResearchLootVector const& l_Loot = sObjectMgr->GetResearchLoot();
        if (!l_Loot.empty())
        {
            for (ResearchLootVector::const_iterator l_Iter = l_Loot.begin(); l_Iter != l_Loot.end(); ++l_Iter)
            {
                ResearchLootEntry l_Entry = (*l_Iter);
                if (l_Entry.id != p_SiteID)
                    continue;

                l_ResearchBranchID = uint32(l_Entry.ResearchBranchID);
                break;
            }
        }

        WorldPacket l_Data(SMSG_ARCHAEOLOGY_SURVERY_CAST, 13);
        l_Data << uint32(p_Count);
        l_Data << uint32(p_MaxCount);
        l_Data << uint32(l_ResearchBranchID);
        l_Data.WriteBit(p_Finished);
        l_Data.FlushBits();

        if (m_Player->GetSession())
            m_Player->GetSession()->SendPacket(&l_Data);
    }

    /// Build & send SMSG_RESEARCH_COMPLETE
    void Manager::SendResearchComplete(uint32 p_ProjectID, uint32 p_FirstCompleted, uint32 p_CompletionCount)
    {
        WorldPacket l_Packet(SMSG_RESEARCH_COMPLETE, 12);
        l_Packet << uint32(p_ProjectID);
        l_Packet << uint32(p_FirstCompleted);
        l_Packet << uint32(p_CompletionCount);

        m_Player->SendDirectMessage(&l_Packet);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Check and fix errors for research sites
    void Manager::ValidateResearchSites()
    {
        bool l_Error = false;

        if (m_ResearchSites.empty())
            l_Error = true;

        for (ResearchSitesMap::const_iterator l_Iterator = m_ResearchSites.begin(); l_Iterator != m_ResearchSites.end(); ++l_Iterator)
        {
            if (l_Iterator->second.empty())
            {
                l_Error = true;
                break;
            }

            for (ResearchSiteSet::const_iterator l_Iterator2 = l_Iterator->second.begin(); l_Iterator2 != l_Iterator->second.end(); ++l_Iterator2)
            {
                if (!sResearchSiteStore.LookupEntry((*l_Iterator2)))
                {
                    l_Error = true;
                    break;
                }
            }
        }

        if (l_Error)
            GenerateResearchSites();
    }

    /// Check and fix errors for projects
    void Manager::ValidateProjects()
    {
        bool l_Error = false;
        if (m_ResearchProjects.empty())
            l_Error = true;

        for (ResearchProjectSet::const_iterator itr = m_ResearchProjects.begin(); itr != m_ResearchProjects.end(); ++itr)
        {
            if (!sResearchProjectStore.LookupEntry((*itr)))
            {
                l_Error = true;
                break;
            }
        }

        if (l_Error)
            GenerateResearchProjects();
    }

    /// Check that the player have all requirement to complete a project
    bool Manager::ValidateCostData()
    {
        if (m_ProjectCostData.empty())
            return false;

        for (std::vector<ProjectCost>::const_iterator l_It = m_ProjectCostData.begin(); l_It != m_ProjectCostData.end(); ++l_It)
        {
            if ((*l_It).currency)
            {
                if (!m_Player->HasCurrency((*l_It).Entry, (*l_It).Count))
                    return false;
            }
            else
            {
                if (!m_Player->HasItemCount((*l_It).Entry, (*l_It).Count))
                    return false;
            }
        }
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Is the player allowed to research for this site
    /// @p_SiteID : Subject
    ResearchWithLevelResult Manager::CanResearchWithLevel(uint32 p_SiteID)
    {
        uint32 l_SkillValue = m_Player->GetSkillValue(SKILL_ARCHAEOLOGY);
        if (!l_SkillValue)
            return RS_RESULT_FAIL;

#define MakeVector(T, a) std::vector<T>(a, a + (sizeof(a) / sizeof(a[0])))
        std::vector<ResearchConditions> l_Conditions = MakeVector(ResearchConditions, gResearchConditions);
#undef MakeVector

        ResearchZoneMap const& l_ZoneMap = sObjectMgr->GetResearchZoneMap();

        for (std::map<uint32, ResearchZoneEntry>::const_iterator l_It = l_ZoneMap.begin(); l_It != l_ZoneMap.end(); ++l_It)
        {
            if ((*l_It).first == p_SiteID)
            {
                uint32 l_ContinentID = (*l_It).second.map;
                uint32 l_ZoneID      = (*l_It).second.zone;

                /// Search zone specific conditions
                std::vector<ResearchConditions>::iterator l_ResearchConditionsIt = std::find_if(l_Conditions.begin(), l_Conditions.end(), [l_ContinentID, l_ZoneID](ResearchConditions & p_Elem) -> bool
                {
                    if (p_Elem.ContinentID == l_ContinentID && p_Elem.ZoneID == l_ZoneID)
                        return true;

                    return false;
                });

                /// Search continent conditions
                if (l_ResearchConditionsIt == l_Conditions.end())
                {
                    l_ResearchConditionsIt = std::find_if(l_Conditions.begin(), l_Conditions.end(), [l_ContinentID](ResearchConditions & p_Elem) -> bool
                    {
                        if (p_Elem.ContinentID == l_ContinentID && p_Elem.ZoneID == 0)
                            return true;

                        return false;
                    });
                }

                if (l_ResearchConditionsIt != l_Conditions.end())
                {
                    uint8 l_RequiredLevel = 0;
                    uint8 l_CurrentLevel  = m_Player->getLevel();

                    ResearchConditions const* l_Conditions = &(*l_ResearchConditionsIt);

                    l_RequiredLevel = l_Conditions->RequiredLevel;
                    if (!l_RequiredLevel)
                        l_RequiredLevel = (*l_It).second.level;

                    if ((l_CurrentLevel + l_Conditions->LevelAdditional) < l_RequiredLevel)
                        return RS_RESULT_FAIL;

                    if (l_SkillValue < l_Conditions->RequiredSkill)
                        return RS_RESULT_FAIL;

                    return RS_RESULT_OK;
                }

            }
        }
        // If get there, so POIid is wrong
        return RS_RESULT_FAIL;
    }

    /// Called when a player complete a research site
    /// @p_SiteID : Completed site ID
    void Manager::UseResearchSite(uint32 p_SiteID)
    {
        uint32 l_ContinentID = m_Player->GetMapId();
        m_ResearchSites[l_ContinentID].erase(p_SiteID);

        GenerateResearchSiteInContinent(l_ContinentID);
    }

    /// Find current research site with player position
    uint16 Manager::GetResearchSiteID()
    {
        ResearchZoneMap const& l_ZoneMap = sObjectMgr->GetResearchZoneMap();

        for (std::map<uint32, ResearchZoneEntry>::const_iterator l_It = l_ZoneMap.begin(); l_It != l_ZoneMap.end(); ++l_It)
        {
            if ((*l_It).second.map == m_Player->GetMapId() && (*l_It).second.zone == m_Player->GetZoneId())
            {
                if (JadeCore::IsPointInZone(ResearchPOIPoint(m_Player->GetPositionX(), m_Player->GetPositionY()), l_It->second.coords))
                    return (*l_It).first;
            }
        }

        return 0;
    }

    /// Check if the player have the specified research site
    /// @p_SiteID      : Subject
    /// @p_ContinentID : Location
    bool Manager::HasResearchSite(uint32 p_SiteID, uint32 p_ContinentID)
    {
        if (m_ResearchSites.find(p_ContinentID) == m_ResearchSites.end())
            return false;

        return m_ResearchSites[p_ContinentID].find(p_SiteID) != m_ResearchSites[p_ContinentID].end();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Check if the player have the specified project
    /// @p_ProjectID : Subject
    bool Manager::HasResearchProject(uint32 p_ProjectID) const
    {
        return m_ResearchProjects.find(p_ProjectID) != m_ResearchProjects.end();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get random loot for a research site
    /// @p_SiteID : Site ID
    /// @p_Site   : [Out]Current site descriptor
    bool Manager::GenerateDigitLoot(uint16 p_SiteID, DigitSite &p_Site)
    {
        ResearchLootVector const& l_Loot = sObjectMgr->GetResearchLoot();
        if (l_Loot.empty())
            return false;

        p_Site.LootGameObjectID = 0;

        ResearchLootVector l_LootList;
        for (ResearchLootVector::const_iterator l_It = l_Loot.begin(); l_It != l_Loot.end(); ++l_It)
        {
            ResearchLootEntry l_Entry = (*l_It);
            if (l_Entry.id != p_SiteID)
                continue;

            if (p_Site.LootGameObjectID == 0)
            {
                switch (l_Entry.ResearchBranchID)
                {
                    case 1: p_Site.LootGameObjectID = 204282; break;
                    case 2: p_Site.LootGameObjectID = 207188; break;
                    case 3: p_Site.LootGameObjectID = 206836; break;
                    case 4: p_Site.LootGameObjectID = 203071; break;
                    case 5: p_Site.LootGameObjectID = 203078; break;
                    case 6: p_Site.LootGameObjectID = 207187; break;
                    case 7: p_Site.LootGameObjectID = 207190; break;
                    case 8: p_Site.LootGameObjectID = 202655; break;
                    case 27: p_Site.LootGameObjectID = 207189; break;
                    case 29: p_Site.LootGameObjectID = 218950; break;
                    case 229: p_Site.LootGameObjectID = 211163; break;
                    case 231: p_Site.LootGameObjectID = 211174; break;
                    case 315: p_Site.LootGameObjectID = 234105; break;
                    case 350: p_Site.LootGameObjectID = 226521; break;
                    case 382: p_Site.LootGameObjectID = 234106; break;
                    default: p_Site.LootGameObjectID = 0; break;
                }
            }

            l_LootList.push_back(l_Entry);
        }

        if (l_LootList.empty())
            return false;

        ResearchLootVector::const_iterator l_It = l_LootList.begin();
        std::advance(l_It, urand(0, l_LootList.size() - 1));

        p_Site.LootGameObjectX = l_It->x;
        p_Site.LootGameObjectY = l_It->y;
        p_Site.LootGameObjectZ = l_It->z;
        p_Site.SiteMaxLootCount = l_LootList.size();

        return true;
    }

}   ///< namespace Archaeology
}   ///< namespace Skill
}   ///< namespace MS
