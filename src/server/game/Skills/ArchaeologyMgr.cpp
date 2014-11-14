/*
 * Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com/>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ArchaeologyMgr.h"
#include "Common.h"
#include "Containers.h"
#include "ObjectMgr.h"

const static uint8 _races[12] = { 1, 2, 3, 4, 5, 6, 7, 8, 27, 29, 229, 231 };

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

bool ArchaeologyMgr::GenerateDigitLoot(uint16 zoneid, DigitSite &site)
{
    ResearchLootVector const& loot = sObjectMgr->GetResearchLoot();
    if (loot.empty())
        return false;

    site.loot_id = 0;

    ResearchLootVector lootList;
    for (ResearchLootVector::const_iterator itr = loot.begin(); itr != loot.end(); ++itr)
    {
        ResearchLootEntry entry = (*itr);
        if (entry.id != zoneid)
            continue;

        if (site.loot_id == 0)
        {
            switch (entry.race)
            {
                case 1: site.loot_id = 204282; break;
                case 2: site.loot_id = 207188; break;
                case 3: site.loot_id = 206836; break;
                case 4: site.loot_id = 203071; break;
                case 5: site.loot_id = 203078; break;
                case 6: site.loot_id = 207187; break;
                case 7: site.loot_id = 207190; break;
                case 8: site.loot_id = 202655; break;
                case 27: site.loot_id = 207189; break;
                case 29: site.loot_id = 218950; break;
                case 229: site.loot_id = 211163; break;
                case 231: site.loot_id = 211174; break;
                default: site.loot_id = 0; break;
            }
        }

        lootList.push_back(entry);
    }

    if (lootList.empty())
        return false;

    ResearchLootVector::const_iterator entry = lootList.begin();
    std::advance(entry, urand(0, lootList.size() - 1));

    site.loot_x = entry->x;
    site.loot_y = entry->y;
    site.loot_z = entry->z;

    return true;
}

uint32 ArchaeologyMgr::GetSurveyBotEntry(float &orientation)
{
    uint16 skill_now = _player->GetSkillValue(SKILL_ARCHAEOLOGY);
    if (!skill_now)
        return 0;

    uint16 zoneid = GetResearchSiteID();
    if (!zoneid)
        return 0;

    uint32 at_pos = 0xFFFFF;

    for (uint8 i = 0; i < MAX_RESEARCH_SITES; ++i)
    {
        //Replace by GetUInt16Value
        uint32 site_now_1 = _player->GetDynamicUInt32Value(PLAYER_DYNAMIC_ARCHEOLOGY_SITES, i) & 0xFFFFF;
        uint32 site_now_2 = _player->GetDynamicUInt32Value(PLAYER_DYNAMIC_ARCHEOLOGY_SITES, i) >> 20;

        if (zoneid == site_now_1)
        {
            at_pos = i;
            break;
        }
        if (zoneid == site_now_2)
        {
            at_pos = i + 1;
            break;
        }
    }

    if (at_pos == 0xFFFFF)
    {
        sLog->outAshran("ArcheologyMgr::GetSurveyBotEntry, at_pos (%u) = 0xFFFFF for site %u !", at_pos, zoneid);
        return 0;
    }

    if (at_pos >= 20)
    {
        sLog->outAshran("ArcheologyMgr::GetSurveyBotEntry, at_pos (%u) >= 20 for site %u !", at_pos, zoneid);
        return 0;
    }

    DigitSite &site = _digSites[at_pos];
    if (site.site_id != zoneid)
    {
        if (!GenerateDigitLoot(zoneid, site))
            return 0;

        site.site_id = zoneid;
    }

    orientation = _player->GetAngle(site.loot_x, site.loot_y);
    float dist_now = _player->GetDistance2d(site.loot_x, site.loot_y);

    if (dist_now >= ARCHAEOLOGY_DIG_SITE_FAR_DIST)
    {
        SendSearchComplete(false, site.count, site.site_id);
        return ARCHAEOLOGY_DIG_SITE_FAR_SURVEYBOT;
    }
    if (dist_now >= ARCHAEOLOGY_DIG_SITE_MED_DIST)
    {
        SendSearchComplete(false, site.count, site.site_id);
        return ARCHAEOLOGY_DIG_SITE_MEDIUM_SURVEYBOT;
    }
    if (dist_now >= ARCHAEOLOGY_DIG_SITE_CLOSE_DIST)
    {
        SendSearchComplete(false, site.count, site.site_id);
        return ARCHAEOLOGY_DIG_SITE_CLOSE_SURVEYBOT;
    }

    if (skill_now < 50)
        _player->UpdateSkill(SKILL_ARCHAEOLOGY, 1);

    _player->SummonGameObject(site.loot_id, site.loot_x, site.loot_y, site.loot_z, 0, 0, 0, 0, 0, 30000);

    if (site.count < 5)
    {
        site.count++;
        if (!GenerateDigitLoot(zoneid, site))
            return 0;

        SendSearchComplete(true, site.count, site.site_id);
        ShowResearchSites();
    }
    else
    {
        SendSearchComplete(true, (site.count + 1), site.site_id);
        site.clear();
        UseResearchSite(zoneid);
    }

    _archaeologyChanged = true;

    return 0;
}

uint16 ArchaeologyMgr::GetResearchSiteID()
{
    ResearchZoneMap const& zoneMap = sObjectMgr->GetResearchZoneMap();

    for (std::map<uint32, ResearchZoneEntry>::const_iterator itr = zoneMap.begin(); itr != zoneMap.end(); ++itr)
    {
        if ((*itr).second.map == _player->GetMapId() && (*itr).second.zone == _player->GetZoneId())
        {
            if (JadeCore::IsPointInZone(ResearchPOIPoint(_player->GetPositionX(), _player->GetPositionY()), itr->second.coords))
                return (*itr).first;
        }
    }

    return 0;
}

void ArchaeologyMgr::UseResearchSite(uint32 id)
{
    uint32 map = _player->GetMapId();
    _researchSites[map].erase(id);

    GenerateResearchSiteInMap(_player->GetMapId());
}

void ArchaeologyMgr::ShowResearchSites()
{
    if (!_player->GetSkillValue(SKILL_ARCHAEOLOGY))
        return;

    uint8 count = 0;
    uint32 newvalue = 0;

    ResearchSiteSet l_TempSet;
    for (ResearchSitesMap::const_iterator l_Iterator = _researchSites.begin(); l_Iterator != _researchSites.end(); ++l_Iterator)
    {
        for (ResearchSiteSet::const_iterator l_Iterator2 = l_Iterator->second.begin(); l_Iterator2 != l_Iterator->second.end(); ++l_Iterator2)
            l_TempSet.insert((*l_Iterator2));
    }

    for (ResearchSiteSet::const_iterator itr = l_TempSet.begin(); itr != l_TempSet.end(); ++itr)
    {
        uint32 id = (*itr);
        if (CanResearchWithLevel(id) == RS_RESULT_HIDE)
            id = 0;

        _player->SetDynamicUInt32Value(PLAYER_DYNAMIC_ARCHEOLOGY_SITES, count, id);

        for (uint8 i = 0; i < MAX_RESEARCH_SITES; ++i)
        {
            if (_digSites[i].site_id == id)
            {
                _player->SetDynamicUInt32Value(PLAYER_DYNAMIC_ARCHEOLOGY_SITES + 1, count, _digSites[i].count);
                break;
            }
            else
                _player->SetDynamicUInt32Value(PLAYER_DYNAMIC_ARCHEOLOGY_SITES + 1, count, 0);
        }

        ++count;
    }
}

void ArchaeologyMgr::ShowResearchProjects()
{
    if (!_player->GetSkillValue(SKILL_ARCHAEOLOGY))
        return;

    uint8 count = 0;
    uint32 newvalue;

    for (ResearchProjectSet::const_iterator itr = _researchProjects.begin(); itr != _researchProjects.end(); ++itr)
    {
        if (count % 2 == 1)
        {
            newvalue |= (*itr);
            _player->SetUInt32Value(PLAYER_FIELD_RESEARCHING + count / 2, newvalue);

            if (count >= MAX_RESEARCH_PROJECTS)
                break;
        }
        else
            newvalue = (*itr) << 16;

        ++count;
    }
}

ResearchWithLevelResult ArchaeologyMgr::CanResearchWithLevel(uint32 siteId)
{
    uint32 skill_now = _player->GetSkillValue(SKILL_ARCHAEOLOGY);
    if (!skill_now)
        return RS_RESULT_FAIL;

    ResearchZoneMap const& zoneMap = sObjectMgr->GetResearchZoneMap();

    for (std::map<uint32, ResearchZoneEntry>::const_iterator itr = zoneMap.begin(); itr != zoneMap.end(); ++itr)
    {
        if ((*itr).first == siteId)
        {
            uint16 skill_cap = 0;
            uint8 level_cap = 0;
            uint8 cur_level = _player->getLevel();
            switch ((*itr).second.map)
            {
                case 0:
                    if ((*itr).second.zone == 4922) // Twilight Hightlands
                        skill_cap = 450;
                    level_cap = (*itr).second.level;
                    if (skill_now < skill_cap || (cur_level + 29 < level_cap))
                        return RS_RESULT_FAIL;
                    break;
                case 1:
                    if ((*itr).second.zone == 616) // Hyjal
                        skill_cap = 450;
                    else if ((*itr).second.zone == 5034) // Uldum
                        skill_cap = 450;
                    level_cap = (*itr).second.level;
                    if (skill_now < skill_cap || (cur_level + 29 < level_cap))
                        return RS_RESULT_FAIL;
                    break;
                case 530: // Outland
                    skill_cap = 275;
                    level_cap = 58;
                    if (skill_now < skill_cap || (cur_level < level_cap))
                        return RS_RESULT_HIDE;
                    break;
                case 571: // Northrend
                    skill_cap = 350;
                    level_cap = 68;
                    if (skill_now < skill_cap || (cur_level < level_cap))
                        return RS_RESULT_HIDE;
                    break;
                case 870: // Pandaria
                    skill_cap = 525;
                    level_cap = 85;
                    if (skill_now < skill_cap || (cur_level < level_cap))
                        return RS_RESULT_HIDE;
                    break;
                default:
                    return RS_RESULT_FAIL;
            }

            return RS_RESULT_OK;
        }
    }
    // If get there, so POIid is wrong
    return RS_RESULT_FAIL;
}

void ArchaeologyMgr::GenerateResearchSiteInMap(uint32 p_MapId)
{
    if (sResearchSiteSet.empty())
        return;

    SiteSet l_TempSites;
    for (std::set<ResearchSiteEntry const*>::const_iterator itr = sResearchSiteSet.begin(); itr != sResearchSiteSet.end(); ++itr)
        if (!HasResearchSite((*itr)->ID, p_MapId) && (*itr)->mapId == p_MapId && CanResearchWithLevel((*itr)->ID))
            l_TempSites.insert((*itr)->ID);

    if (l_TempSites.empty())
        return;

    _researchSites[p_MapId].insert(JadeCore::Containers::SelectRandomContainerElement(l_TempSites));
    _archaeologyChanged = true;

    ShowResearchSites();
}

void ArchaeologyMgr::GenerateResearchSites()
{
    if (sResearchSiteSet.empty())
        return;

    for (ResearchSitesMap::iterator l_Iterator = _researchSites.begin(); l_Iterator != _researchSites.end(); ++l_Iterator)
        l_Iterator->second.clear();

    for (std::set<ResearchSiteEntry const*>::const_iterator l_Iterator = sResearchSiteSet.begin(); l_Iterator != sResearchSiteSet.end(); ++l_Iterator)
    {
        if (CanResearchWithLevel((*l_Iterator)->ID))
            _researchSites[(*l_Iterator)->mapId].insert((*l_Iterator)->ID);
    }

    for (ResearchSitesMap::iterator l_Iterator = _researchSites.begin(); l_Iterator != _researchSites.end(); ++l_Iterator)
        JadeCore::Containers::RandomResizeSet(l_Iterator->second, RESEARCH_SITES_PER_MAP);

    _archaeologyChanged = true;
    ShowResearchSites();
}

void ArchaeologyMgr::GenerateResearchSitesForMap(uint32 p_MapId, uint32 p_SitesCount)
{
    if (sResearchSiteSet.empty())
        return;

    _researchSites[p_MapId].clear();

    for (std::set<ResearchSiteEntry const*>::const_iterator l_Iterator = sResearchSiteSet.begin(); l_Iterator != sResearchSiteSet.end(); ++l_Iterator)
    {
        if ((*l_Iterator)->mapId != p_MapId)
            continue;

        if (CanResearchWithLevel((*l_Iterator)->ID))
            _researchSites[p_MapId].insert((*l_Iterator)->ID);
    }

    JadeCore::Containers::RandomResizeSet(_researchSites[p_MapId], p_SitesCount);

    _archaeologyChanged = true;
    ShowResearchSites();
}


void ArchaeologyMgr::GenerateResearchProjects()
{
    if (sResearchProjectSet.empty())
        return;

    uint16 skill_now = _player->GetSkillValue(SKILL_ARCHAEOLOGY);
    if (!skill_now)
        return;

    _researchProjects.clear();

    Projects tempProjects;
    uint32 chance = skill_now / 10;

    for (std::set<ResearchProjectEntry const*>::const_iterator itr = sResearchProjectSet.begin(); itr != sResearchProjectSet.end(); ++itr)
    {
        ResearchProjectEntry const* entry = (*itr);
        if (!entry)
            continue;

        if ((entry->rare && !roll_chance_i(chance)))
            continue;

        tempProjects[entry->branchId].insert(entry->ID);
    }

    for (int i = 0; i < 12; i++)
    {
        uint32 raceIndex = _races[i];
        if (tempProjects.find(raceIndex) == tempProjects.end())
            continue;

        ProjectSet& proj = tempProjects[raceIndex];
        if (proj.empty())
            continue;

        ProjectSet::iterator itr = proj.begin();
        std::advance(itr, rand() % proj.size());
        _researchProjects.insert(*itr);
    }

    _archaeologyChanged = true;

    ShowResearchProjects();
}

bool ArchaeologyMgr::SolveResearchProject(uint32 projectId)
{
    if (!projectId)
        return false;

    // Check for skill
    uint16 skill_now = _player->GetSkillValue(SKILL_ARCHAEOLOGY);
    if (!skill_now)
        return false;

    // Check for correct cost data
    if (!ValidateCostData())
        return false;

    // Check for project id
    ResearchProjectEntry const* entry = NULL;
    for (std::set<ResearchProjectEntry const*>::const_iterator itr = sResearchProjectSet.begin(); itr != sResearchProjectSet.end(); ++itr)
    {
        if ((*itr)->ID == projectId)
        {
            entry = (*itr);
            break;
        }
    }

    if (!entry)
        return false;

    // Check for existing project
    if (!HasResearchProject(projectId))
        return false;

    // All done
    for (std::vector<ProjectCost>::const_iterator itr = costData.begin(); itr != costData.end(); ++itr)
    {
        if ((*itr).currency)
            _player->ModifyCurrency((*itr).itemId, -int32((*itr).count));
        else
            _player->DestroyItemCount((*itr).itemId, (*itr).count, true);
    }

    ClearProjectCost();

    _researchProjects.erase(projectId);

    _player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ARCHAEOLOGY_PROJECTS, projectId, 1);

     // Already Completed
    if (_completedProjects.find(projectId) != _completedProjects.end())
        ++_completedProjects[projectId].count;
    else
    {
        CompletedProject project;
        project.count = 1;
        project.first_date = time(NULL);

        _completedProjects.insert(std::make_pair(projectId, project));
    }

    // Add new project
    ProjectSet tempProjects;
    uint32 chance = skill_now / 10;

    for (std::set<ResearchProjectEntry const*>::const_iterator itr = sResearchProjectSet.begin(); itr != sResearchProjectSet.end(); ++itr)
    {
        if ((*itr)->branchId == entry->branchId)
        {
            if (((*itr)->rare && !roll_chance_i(chance)))
                continue;

            tempProjects.insert((*itr)->ID);
        }
    }

    _researchProjects.insert(JadeCore::Containers::SelectRandomContainerElement(tempProjects));
    _archaeologyChanged = true;

    ShowResearchProjects();
    return true;
}

void ArchaeologyMgr::SaveArchaeology(SQLTransaction& p_Transaction)
{
    if (!_player->GetSkillValue(SKILL_ARCHAEOLOGY))
        return;

    if (!_archaeologyChanged)
        return;

    std::ostringstream l_StringStream;

    /// character_archaeology
    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_ARCHAEOLOGY);
    l_Statement->setUInt32(0, _player->GetGUIDLow());
    p_Transaction->Append(l_Statement);

    l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_INS_PLAYER_ARCHAEOLOGY);
    l_Statement->setUInt32(0, _player->GetGUIDLow());

    for (uint8 j = 0; j < MAX_RESEARCH_SITES; ++j)
        l_StringStream << uint32(_digSites[j].count) << " ";

    l_Statement->setString(1, l_StringStream.str().c_str());
    l_StringStream.str("");

    for (ResearchProjectSet::const_iterator itr = _researchProjects.begin(); itr != _researchProjects.end(); ++itr)
        l_StringStream << (*itr) << " ";

    l_Statement->setString(2, l_StringStream.str().c_str());
    p_Transaction->Append(l_Statement);

    /// character_archaeology_sites
    l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_ARCHAEOLOGY_SITES);
    l_Statement->setUInt32(0, _player->GetGUIDLow());
    p_Transaction->Append(l_Statement);

    for (ResearchSitesMap::const_iterator l_Iterator = _researchSites.begin(); l_Iterator != _researchSites.end(); ++l_Iterator)
    {
        if (l_Iterator->second.empty())
            continue;

        l_StringStream.str("");

        l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_INS_PLAYER_ARCHAEOLOGY_SITES);
        l_Statement->setUInt32(0, _player->GetGUIDLow());
        l_Statement->setUInt32(1, l_Iterator->first);

        for (ResearchSiteSet::const_iterator l_Iterator2 = l_Iterator->second.begin(); l_Iterator2 != l_Iterator->second.end(); ++l_Iterator2)
            l_StringStream << (*l_Iterator2) << " ";

        l_Statement->setString(2, l_StringStream.str().c_str());

        p_Transaction->Append(l_Statement);
    }

    /// character_archaeology_projetcs
    l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_ARCHAEOLOGY_PROJECTS);
    l_Statement->setUInt32(0, _player->GetGUIDLow());
    p_Transaction->Append(l_Statement);

    for (auto itr : _completedProjects)
    {
        l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_INS_PLAYER_ARCHAEOLOGY_PROJECTS);
        l_Statement->setUInt32(0, _player->GetGUIDLow());
        l_Statement->setUInt32(1, itr.first);
        l_Statement->setUInt32(2, itr.second.count);
        l_Statement->setUInt32(3, itr.second.first_date);
        p_Transaction->Append(l_Statement);
    }

    _archaeologyChanged = false;
}

void ArchaeologyMgr::LoadArchaeology(PreparedQueryResult p_Result, PreparedQueryResult p_ResultProjects, PreparedQueryResult p_ResultSites)
{
    if (!_player->GetSkillValue(SKILL_ARCHAEOLOGY))
        return;

    for (uint8 i = 0; i < MAX_RESEARCH_SITES; ++i)
        _digSites[i].clear();

    if (!p_Result)
        return;

    Field* l_Fields = p_Result->Fetch();

    _researchSites.clear();

    /// Loading current zones
    if (p_ResultSites)
    {
        do
        {
            Field* l_Fields2 = p_ResultSites->Fetch();

            uint32 l_MapID = l_Fields2[0].GetUInt32();
            Tokenizer l_Tokens(l_Fields2[1].GetCString(), ' ', RESEARCH_SITES_PER_MAP);

            if (l_Tokens.size() == RESEARCH_SITES_PER_MAP)
            {
                for (uint8 l_I = 0; l_I < l_Tokens.size(); ++l_I)
                    _researchSites[l_MapID].insert(uint32(atoi(l_Tokens[l_I])));
            }
        }
        while (p_ResultProjects->NextRow());
    }

    ValidateSites();

    // Loading current zone info
    if (l_Fields[0].GetCString())
    {
        Tokenizer tokens(l_Fields[0].GetCString(), ' ', MAX_RESEARCH_SITES);
        if (tokens.size() == MAX_RESEARCH_SITES)
        {
            for (uint8 i = 0; i < MAX_RESEARCH_SITES; ++i)
                _digSites[i].count = uint32(atoi(tokens[i]));
        }
    }

    /// Loading current projects
    _researchProjects.clear();
    if (l_Fields[1].GetCString())
    {
        Tokenizer tokens(l_Fields[0].GetCString(), ' ', MAX_RESEARCH_PROJECTS);
        if (tokens.size() == MAX_RESEARCH_PROJECTS)
            for (uint8 i = 0; i < MAX_RESEARCH_PROJECTS; ++i)
                _researchProjects.insert(uint32(atoi(tokens[i])));
    }

    ValidateProjects();

    if (!p_ResultProjects)
        return;

    _completedProjects.clear();

    do
    {
        Field* l_Fields3 = p_ResultProjects->Fetch();

        CompletedProject project;
        uint32 projectId = l_Fields3[0].GetUInt32();
        project.count = l_Fields3[1].GetUInt32();
        project.first_date = l_Fields3[2].GetUInt32();

        if (_completedProjects.find(projectId) == _completedProjects.end())
            _completedProjects.insert(std::make_pair(projectId, project));
    }
    while (p_ResultProjects->NextRow());
}

void ArchaeologyMgr::ValidateSites()
{
    bool l_Error = false;

    for (ResearchSitesMap::const_iterator l_Iterator = _researchSites.begin(); l_Iterator != _researchSites.end(); ++l_Iterator)
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

void ArchaeologyMgr::ValidateProjects()
{
    bool err = false;
    if (_researchProjects.empty())
        err = true;

    for (ResearchProjectSet::const_iterator itr = _researchProjects.begin(); itr != _researchProjects.end(); ++itr)
    {
        if (!sResearchProjectStore.LookupEntry((*itr)))
        {
            err = true;
            break;
        }
    }

    if (err)
        GenerateResearchProjects();
}

bool ArchaeologyMgr::ValidateCostData()
{
    if (costData.empty())
        return false;

    for (std::vector<ProjectCost>::const_iterator itr = costData.begin(); itr != costData.end(); ++itr)
    {
        if ((*itr).currency)
        {
            if (!_player->HasCurrency((*itr).itemId, (*itr).count))
                return false;
        }
        else
        {
            if (!_player->HasItemCount((*itr).itemId, (*itr).count))
                return false;
        }
    }
    return true;
}

uint16 ArchaeologyMgr::GetRandomActiveSiteInMap(uint32 mapId)
{
    std::list<uint16> sitesId;

    for (uint16 itr : _researchSites[mapId])
        sitesId.push_back(itr);

    if (sitesId.empty())
        return 0;

    JadeCore::Containers::RandomResizeList(sitesId, 1);
    return sitesId.front();
}

void ArchaeologyMgr::SendSearchComplete(bool finished, uint8 count, uint16 siteId)
{
    uint32 race = 0;
    ResearchLootVector const& loot = sObjectMgr->GetResearchLoot();
    if (!loot.empty())
    {
        for (ResearchLootVector::const_iterator itr = loot.begin(); itr != loot.end(); ++itr)
        {
            ResearchLootEntry entry = (*itr);
            if (entry.id != siteId)
                continue;

            race = uint32(entry.race);
            break;
        }
    }

    WorldPacket data(SMSG_RESEARCH_COMPLETE, 13);

    data << uint32(count);
    data << uint32(6);
    data << race;

    data.WriteBit(finished);
    data.FlushBits();

    _player->GetSession()->SendPacket(&data);

    if (count == 6)
        SendSearchSiteComplete(siteId);
}

void ArchaeologyMgr::SendSearchSiteComplete(uint16 siteId)
{
//     WorldPacket data(SMSG_RESEARCH_SITE_COMPLETE, 12);
//     data << uint32(siteId);
//     data << uint32(time(NULL));
//     data << uint32(1);
//     _player->GetSession()->SendPacket(&data);
}
