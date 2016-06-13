////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "GarrisonMgr.hpp"
#include "../../../scripts/Draenor/Garrison/GarrisonScriptData.hpp"
#include "Player.h"
#include "DatabaseEnv.h"
#include "ObjectMgr.h"
#include "ObjectAccessor.h"
#include "CreatureAI.h"
#include "DisableMgr.h"

namespace MS { namespace Garrison
{
    /// Constructor
    Manager::Manager(Player* p_Owner)
        : m_Owner(p_Owner)
    {
        m_GarrisonLevel     = 1;
        m_GarrisonLevelID   = 0;
        m_GarrisonSiteID    = 0;

        m_NumFollowerActivation                = 1;
        m_NumFollowerActivationRegenTimestamp  = time(0);
        m_CacheLastUsage                       = time(0);
        m_MissionDistributionLastUpdate        = time(0);

        m_CacheGameObjectGUID = 0;

        m_CacheLastTokenAmount = 0;

        m_GarrisonScript = nullptr;
        m_CanRecruitFollower = p_Owner->GetCharacterWorldStateValue(CharacterWorldStates::GarrisonTavernBoolCanRecruitFollower) == 0;

        /// Select Garrison site ID
        switch (GetGarrisonFactionIndex())
        {
            case FactionIndex::Alliance:
                m_GarrisonSiteID = 2;
                break;

            case FactionIndex::Horde:
                m_GarrisonSiteID = 71;
                break;

            case FactionIndex::Max:
            default:
                ASSERT(false);
                break;

        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Create the garrison
    void Manager::Create()
    {
        /// At creation, the garrison cache contains 50 token
        m_CacheLastUsage = time(0) - (200 * Globals::CacheTokenGenerateTime);

        std::ostringstream l_KnownBluePrintsStr;

        for (uint32 l_I = 0; l_I < m_KnownBlueprints.size(); ++l_I)
            l_KnownBluePrintsStr << m_KnownBlueprints[l_I] << ' ';

        std::ostringstream l_KnownSpecializationsStr;

        for (uint32 l_I = 0; l_I < m_KnownSpecializations.size(); ++l_I)
            l_KnownSpecializationsStr << m_KnownSpecializations[l_I] << ' ';

        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GARRISON);

        m_ID = sObjectMgr->GetNewGarrisonID();

        uint32 l_Index = 0;
        l_Stmt->setUInt32(l_Index++, m_ID);
        l_Stmt->setUInt32(l_Index++, m_Owner->GetGUIDLow());
        l_Stmt->setUInt32(l_Index++, m_GarrisonLevel);
        l_Stmt->setString(l_Index++, l_KnownBluePrintsStr.str());
        l_Stmt->setString(l_Index++, l_KnownSpecializationsStr.str());
        l_Stmt->setUInt32(l_Index++, m_NumFollowerActivation);
        l_Stmt->setUInt32(l_Index++, m_NumFollowerActivationRegenTimestamp);
        l_Stmt->setUInt32(l_Index++, m_CacheLastUsage);

        CharacterDatabase.AsyncQuery(l_Stmt);

        Init();

        /// Force mission distribution update
        m_MissionDistributionLastUpdate = 0;
    
        std::vector<uint32> l_FollowerQuests = sObjectMgr->FollowerQuests;

        /// Quest non rewarded followers
        for (uint32 l_QuestID : l_FollowerQuests)
        {
            if (m_Owner->GetQuestStatus(l_QuestID) == QUEST_STATUS_REWARDED)
            {
                const Quest         * l_QuestTemplate   = sObjectMgr->GetQuestTemplate(l_QuestID);
                const SpellInfo     * l_SpellInfo       = sSpellMgr->GetSpellInfo(l_QuestTemplate->GetRewSpellCast());

                if (GetFollower(l_SpellInfo->Effects[EFFECT_0].MiscValue) == nullptr)
                    AddFollower(l_SpellInfo->Effects[EFFECT_0].MiscValue);
            }
        }

        /// Garrison ability
        if (!m_Owner->HasSpell(GARRISON_SPELL_GARR_ABILITY))
            m_Owner->learnSpell(GARRISON_SPELL_GARR_ABILITY, false);

        uint32 l_QuestEntry = 0;

        /// Select Garrison site ID
        switch (GetGarrisonFactionIndex())
        {
            case FactionIndex::Alliance:
                l_QuestEntry = BaseQuests::FoundedGarrisonA;
                break;

            case FactionIndex::Horde:
                l_QuestEntry = BaseQuests::FoundedGarrisonH;
                break;

            case FactionIndex::Max:
            default:
                ASSERT(false);
                break;

        }

        /// Minimap icons are completed quest, reason => BLIZZARD !!!!!!!!!!!!!!
        if (m_Owner->GetQuestStatus(l_QuestEntry) != QUEST_STATUS_REWARDED)
        {
            if (const Quest* l_Quest = sObjectMgr->GetQuestTemplate(l_QuestEntry))
            {
                m_Owner->AddQuest(l_Quest, m_Owner);
                m_Owner->CompleteQuest(l_QuestEntry);
            }
        }

        sGarrisonBuildingManager->LearnAllowedBuildings(m_Owner, this);

        /// Learn garrison taxinodes
        if (m_Owner->getFaction() == HORDE)
            m_Owner->m_taxi.SetTaximaskNode(TaxiNodes::Horde);
        else
            m_Owner->m_taxi.SetTaximaskNode(TaxiNodes::Alliance);
    }

    /// Load
    bool Manager::Load(PreparedQueryResult p_GarrisonResult, PreparedQueryResult p_BuildingsResult, PreparedQueryResult p_FollowersResult, PreparedQueryResult p_MissionsResult, PreparedQueryResult p_WorkOrderResult)
    {
        if (p_GarrisonResult)
        {
            Field* l_Fields = p_GarrisonResult->Fetch();

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

            m_NumFollowerActivation                = l_Fields[4].GetUInt32();
            m_NumFollowerActivationRegenTimestamp  = l_Fields[5].GetUInt32();
            m_CacheLastUsage                       = l_Fields[6].GetUInt32();

            if (p_BuildingsResult)
            {
                do
                {
                    l_Fields = p_BuildingsResult->Fetch();

                    GarrisonBuilding l_Building;
                    l_Building.DatabaseID       = l_Fields[0].GetUInt32();
                    l_Building.PlotInstanceID   = l_Fields[1].GetUInt32();
                    l_Building.BuildingID       = l_Fields[2].GetUInt32();
                    l_Building.SpecID           = l_Fields[3].GetUInt32();
                    l_Building.TimeBuiltStart   = l_Fields[4].GetUInt32();
                    l_Building.TimeBuiltEnd     = l_Fields[5].GetUInt32();
                    l_Building.FollowerAssigned = l_Fields[6].GetUInt32();
                    l_Building.Active           = l_Fields[7].GetBool();
                    l_Building.GatheringData    = l_Fields[8].GetString();

                    if (!l_Building.Active && time(0) > l_Building.TimeBuiltEnd)
                        l_Building.BuiltNotified = true;    ///< Auto notify by info packet

                    m_Buildings.push_back(l_Building);
                } while (p_BuildingsResult->NextRow());
            }

            if (p_MissionsResult)
            {
                do
                {
                    l_Fields = p_MissionsResult->Fetch();

                    GarrisonMission l_Mission;
                    l_Mission.DatabaseID        = l_Fields[0].GetUInt32();
                    l_Mission.MissionID         = l_Fields[1].GetUInt32();
                    l_Mission.OfferTime         = l_Fields[2].GetUInt32();
                    l_Mission.OfferMaxDuration  = l_Fields[3].GetUInt32();
                    l_Mission.StartTime         = l_Fields[4].GetUInt32();
                    l_Mission.State             = (MissionStates::Type)l_Fields[5].GetUInt32();

                    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_GARRISON_MISSION, l_Mission.MissionID, m_Owner))
                        continue;

                    if ((l_Mission.OfferTime + l_Mission.OfferMaxDuration) > time(0) || l_Mission.State == MissionStates::InProgress)
                        m_Missions.push_back(l_Mission);
                    else
                    {
                        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_MISSION);
                        l_Stmt->setUInt32(0, l_Mission.DatabaseID);
                        CharacterDatabase.AsyncQuery(l_Stmt);
                    }

                } while (p_MissionsResult->NextRow());
            }

            if (p_FollowersResult)
            {
                do
                {
                    l_Fields = p_FollowersResult->Fetch();

                    GarrisonFollower l_Follower;
                    l_Follower.DatabaseID        = l_Fields[0].GetUInt32();
                    l_Follower.FollowerID        = l_Fields[1].GetUInt32();
                    l_Follower.Level             = l_Fields[2].GetUInt32();
                    l_Follower.XP                = l_Fields[3].GetUInt32();
                    l_Follower.Quality           = l_Fields[4].GetUInt32();
                    l_Follower.ItemLevelArmor    = l_Fields[5].GetUInt32();
                    l_Follower.ItemLevelWeapon   = l_Fields[6].GetUInt32();
                    l_Follower.CurrentMissionID  = l_Fields[7].GetUInt32();
                    l_Follower.CurrentBuildingID = l_Fields[8].GetUInt32();

                    if (!l_Fields[9].GetString().empty())
                    {
                        Tokenizer l_Abilities(l_Fields[9].GetString(), ' ');

                        for (Tokenizer::const_iterator l_It = l_Abilities.begin(); l_It != l_Abilities.end(); ++l_It)
                            l_Follower.Abilities.push_back(atol(*l_It));
                    }

                    l_Follower.Flags = l_Fields[10].GetUInt32();
                    l_Follower.ShipName = l_Fields[11].GetString();

                    m_Followers.push_back(l_Follower);
                    GenerateFollowerAbilities(l_Follower.DatabaseID, false);

                } while (p_FollowersResult->NextRow());
            }

            if (p_WorkOrderResult)
            {
                do
                {
                    l_Fields = p_WorkOrderResult->Fetch();

                    GarrisonWorkOrder l_Order;
                    l_Order.DatabaseID        = l_Fields[0].GetUInt32();
                    l_Order.PlotInstanceID    = l_Fields[1].GetUInt32();
                    l_Order.ShipmentID        = l_Fields[2].GetUInt32();
                    l_Order.CreationTime      = l_Fields[3].GetUInt32();
                    l_Order.CompleteTime      = l_Fields[4].GetUInt32();

                    m_WorkOrders.push_back(l_Order);

                } while (p_WorkOrderResult->NextRow());
            }

            Init();

            if (!GetGarrisonSiteLevelEntry())
                return false;

            /// Remove doubloon mission
            std::map<uint32, uint32> l_MissionToRemoveCount;

            for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
            {
                GarrisonMission & l_Mission = m_Missions[l_I];

                uint32 l_Count = (uint32)std::count_if(m_Missions.begin(), m_Missions.end(), [l_Mission](const GarrisonMission& p_Mission)
                {
                    return p_Mission.MissionID == l_Mission.MissionID;
                });

                l_MissionToRemoveCount[l_Mission.MissionID] = l_Count - 1;
            }

            std::vector<uint64> l_MissionToRemove;
            for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
            {
                GarrisonMission& l_Mission = m_Missions[l_I];

                if (l_MissionToRemoveCount[l_Mission.MissionID] > 0)
                {
                    l_MissionToRemove.push_back(l_Mission.DatabaseID);
                    l_MissionToRemoveCount[l_Mission.MissionID]--;
                }
            }

            for (uint64 l_MissionBD_ID : l_MissionToRemove)
            {
                auto l_It = std::find_if(m_Missions.begin(), m_Missions.end(), [l_MissionBD_ID](const GarrisonMission& p_Mission)
                {
                    return p_Mission.DatabaseID == l_MissionBD_ID;
                });

                if (l_It != m_Missions.end())
                {
                    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_MISSION);
                    l_Stmt->setUInt32(0, l_MissionBD_ID);

                    CharacterDatabase.AsyncQuery(l_Stmt);

                    m_Missions.erase(l_It);
                }
            }

            l_MissionToRemove.clear();

            for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
            {
                GarrisonMission& l_Mission = m_Missions[l_I];

                if (l_Mission.State != MissionStates::InProgress)
                    continue;

                uint32 l_FollowerCount = (uint32)std::count_if(m_Followers.begin(), m_Followers.end(), [l_Mission](const GarrisonFollower & p_Follower) -> bool
                {
                    if (p_Follower.CurrentMissionID == l_Mission.MissionID)
                        return true;

                    return false;
                });

                if (l_FollowerCount == 0)
                {
                    l_MissionToRemove.push_back(l_Mission.DatabaseID);
                    continue;
                }

                GarrMissionEntry const* l_MissionTemplate = sGarrMissionStore.LookupEntry(l_Mission.MissionID);

                if (!l_MissionTemplate || l_MissionTemplate->RequiredFollowersCount != l_FollowerCount)
                    l_MissionToRemove.push_back(l_Mission.DatabaseID);
            }

            for (uint64 l_MissionBD_ID : l_MissionToRemove)
            {
                auto l_It = std::find_if(m_Missions.begin(), m_Missions.end(), [l_MissionBD_ID](const GarrisonMission & p_Mission)
                {
                    return p_Mission.DatabaseID == l_MissionBD_ID;
                });

                if (l_It != m_Missions.end())
                {
                    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_MISSION);
                    l_Stmt->setUInt32(0, l_MissionBD_ID);

                    CharacterDatabase.AsyncQuery(l_Stmt);

                    m_Missions.erase(l_It);
                }
            }

            std::vector<uint32> l_FollowerQuests = sObjectMgr->FollowerQuests;

            /// Quest non rewarded followers
            for (uint32 l_QuestID : l_FollowerQuests)
            {
                if (m_Owner->GetQuestStatus(l_QuestID) == QUEST_STATUS_REWARDED)
                {
                    const Quest* l_QuestTemplate = sObjectMgr->GetQuestTemplate(l_QuestID);
                    const SpellInfo* l_SpellInfo = sSpellMgr->GetSpellInfo(l_QuestTemplate->GetRewSpellCast());

                    if (GetFollower(l_SpellInfo->Effects[EFFECT_0].MiscValue) == nullptr)
                        AddFollower(l_SpellInfo->Effects[EFFECT_0].MiscValue);
                }
            }

            /// Force mission distribution update
            m_MissionDistributionLastUpdate = 0;

            /// Fix bug in mission distribution TEMP CODE
            uint32 l_MaxMissionCount            = ceil(m_Followers.size() * GARRISON_MISSION_DISTRIB_FOLLOWER_COEFF);
            uint32 l_CurrentAvailableMission    = 0;

            std::for_each(m_Missions.begin(), m_Missions.end(), [&l_CurrentAvailableMission](const GarrisonMission& p_Mission) -> void
            {
                if (p_Mission.State == MissionStates::Available && (p_Mission.OfferTime + p_Mission.OfferMaxDuration) > time(0))
                    l_CurrentAvailableMission++;
            });

            if (l_CurrentAvailableMission > l_MaxMissionCount)
            {
                m_Missions.erase(std::remove_if(m_Missions.begin(), m_Missions.end(), [l_CurrentAvailableMission, l_MaxMissionCount](const GarrisonMission& p_Mission) -> bool
                {
                    if (p_Mission.State == MissionStates::Available && (p_Mission.OfferTime + p_Mission.OfferMaxDuration) > time(0) && l_CurrentAvailableMission > l_MaxMissionCount)
                        return true;

                    return false;
                }), m_Missions.end());
            }

            /// Unstuck follower
            std::for_each(m_Followers.begin(), m_Followers.end(), [this](GarrisonFollower& p_Follower)
            {
                if (p_Follower.CurrentMissionID != 0)
                {
                    auto l_It = std::find_if(m_Missions.begin(), m_Missions.end(), [p_Follower](const GarrisonMission& p_Mission) -> bool
                    {
                        if (p_Mission.MissionID == p_Follower.CurrentMissionID)
                            return true;

                        return false;
                    });

                    if (l_It == m_Missions.end() || (*l_It).State == MissionStates::Available)
                        p_Follower.CurrentMissionID = 0;
                }

                /// Corrupted DB
                if (p_Follower.Level == 0)
                {
                    GarrFollowerEntry const* l_Entry = sGarrFollowerStore.LookupEntry(p_Follower.FollowerID);

                    if (l_Entry)
                    {
                        p_Follower.Level            = l_Entry->Level;
                        p_Follower.XP               = 0;
                        p_Follower.Quality          = l_Entry->Quality;
                        p_Follower.ItemLevelArmor   = l_Entry->ItemLevelArmor;
                        p_Follower.ItemLevelWeapon  = l_Entry->ItemLevelWeapon;

                        p_Follower.Abilities.clear();

                        for (uint32 l_I = 0; l_I < sGarrFollowerXAbilityStore.GetNumRows(); ++l_I)
                        {
                            GarrFollowerXAbilityEntry const* l_Entry = sGarrFollowerXAbilityStore.LookupEntry(l_I);

                            if (l_Entry && l_Entry->FollowerID == p_Follower.FollowerID && sGarrAbilityStore.LookupEntry(l_Entry->AbilityID) && l_Entry->FactionIndex == GetGarrisonFactionIndex())
                                p_Follower.Abilities.push_back(l_Entry->AbilityID);
                        }
                    }
                }
            });

            /// Garrison ability
            if (!m_Owner->HasSpell(GARRISON_SPELL_GARR_ABILITY))
                m_Owner->learnSpell(GARRISON_SPELL_GARR_ABILITY, false);

            uint32 l_QuestEntry = 0;
            /// Select Garrison site ID
            switch (GetGarrisonFactionIndex())
            {
                case FactionIndex::Alliance:
                    l_QuestEntry = BaseQuests::FoundedGarrisonA;
                    break;

                case FactionIndex::Horde:
                    l_QuestEntry = BaseQuests::FoundedGarrisonH;
                    break;

                case FactionIndex::Max:
                default:
                    ASSERT(false);
                    break;

            }

            /// Minimap icons are completed quest, reason => BLIZZARD !!!!!!!!!!!!!!
            if (m_Owner->GetQuestStatus(l_QuestEntry) != QUEST_STATUS_REWARDED)
            {
                if (const Quest* l_Quest = sObjectMgr->GetQuestTemplate(l_QuestEntry))
                {
                    m_Owner->AddQuest(l_Quest, m_Owner);
                    m_Owner->CompleteQuest(l_QuestEntry);
                }
            }

            sGarrisonBuildingManager->LearnAllowedBuildings(m_Owner, this);

            return true;
        }

        return false;
    }

    /// Save this garrison to DB
    void Manager::Save()
    {
        SQLTransaction l_GarrisonTransaction = CharacterDatabase.BeginTransaction();

        std::ostringstream l_KnownBluePrintsStr;

        for (uint32 l_I = 0; l_I < m_KnownBlueprints.size(); ++l_I)
            l_KnownBluePrintsStr << m_KnownBlueprints[l_I] << ' ';

        std::ostringstream l_KnownSpecializationsStr;

        for (uint32 l_I = 0; l_I < m_KnownSpecializations.size(); ++l_I)
            l_KnownSpecializationsStr << m_KnownSpecializations[l_I] << ' ';

        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GARRISON);

        std::string l_KnownBluePrints = l_KnownBluePrintsStr.str();
        if (l_KnownBluePrints.empty())
            l_KnownBluePrints = " ";

        std::string l_KnowSpecializations = l_KnownSpecializationsStr.str();
        if (l_KnowSpecializations.empty())
            l_KnowSpecializations = " ";

        uint32 l_Index = 0;
        l_Stmt->setUInt32(l_Index++, m_GarrisonLevel);
        l_Stmt->setString(l_Index++, l_KnownBluePrints);
        l_Stmt->setString(l_Index++, l_KnowSpecializations);
        l_Stmt->setUInt32(l_Index++, m_NumFollowerActivation);
        l_Stmt->setUInt32(l_Index++, m_NumFollowerActivationRegenTimestamp);
        l_Stmt->setUInt32(l_Index++, m_CacheLastUsage);
        l_Stmt->setUInt32(l_Index++, m_ID);
        
        l_GarrisonTransaction->Append(l_Stmt);

        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        {
            PreparedStatement* l_BuildingStatement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GARRISON_BUILDING);

            l_Index = 0;
            l_BuildingStatement->setUInt32(l_Index++, m_Buildings[l_I].PlotInstanceID);
            l_BuildingStatement->setUInt32(l_Index++, m_Buildings[l_I].BuildingID);
            l_BuildingStatement->setUInt32(l_Index++, m_Buildings[l_I].SpecID);
            l_BuildingStatement->setUInt32(l_Index++, m_Buildings[l_I].TimeBuiltStart);
            l_BuildingStatement->setUInt32(l_Index++, m_Buildings[l_I].TimeBuiltEnd);
            l_BuildingStatement->setUInt32(l_Index++, m_Buildings[l_I].FollowerAssigned);
            l_BuildingStatement->setBool  (l_Index++, m_Buildings[l_I].Active);
            l_BuildingStatement->setString(l_Index++, m_Buildings[l_I].GatheringData);
            l_BuildingStatement->setUInt32(l_Index++, m_Buildings[l_I].DatabaseID);
            l_BuildingStatement->setUInt32(l_Index++, m_ID);

            l_GarrisonTransaction->Append(l_BuildingStatement);
        }

        for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        {
            if ((m_Missions[l_I].OfferTime + m_Missions[l_I].OfferMaxDuration) > time(0) || m_Missions[l_I].State == MissionStates::InProgress || m_Missions[l_I].State == MissionStates::CompleteSuccess)
            {
                PreparedStatement* l_MissionStmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GARRISON_MISSION);

                l_Index = 0;
                l_MissionStmt->setUInt32(l_Index++, m_Missions[l_I].MissionID);
                l_MissionStmt->setUInt32(l_Index++, m_Missions[l_I].OfferTime);
                l_MissionStmt->setUInt32(l_Index++, m_Missions[l_I].OfferMaxDuration);
                l_MissionStmt->setUInt32(l_Index++, m_Missions[l_I].StartTime);
                l_MissionStmt->setUInt32(l_Index++, m_Missions[l_I].State);

                l_MissionStmt->setUInt32(l_Index++, m_Missions[l_I].DatabaseID);
                l_MissionStmt->setUInt32(l_Index++, m_ID);

                l_GarrisonTransaction->Append(l_MissionStmt);
            }
            else
            {
                PreparedStatement * l_MissionStmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_MISSION);
                l_MissionStmt->setUInt32(0, m_Missions[l_I].DatabaseID);
                l_GarrisonTransaction->Append(l_MissionStmt);
            }
        }

        for (uint32 l_I = 0; l_I < m_Followers.size(); ++l_I)
        {
            std::ostringstream l_Abilities;

            for (uint32 l_Y = 0; l_Y < m_Followers[l_I].Abilities.size(); ++l_Y)
                l_Abilities << m_Followers[l_I].Abilities[l_Y] << ' ';

            PreparedStatement* l_FollowerStmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GARRISON_FOLLOWER);

            l_Index = 0;
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].FollowerID);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].Level);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].XP);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].Quality);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].ItemLevelArmor);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].ItemLevelWeapon);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].CurrentMissionID);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].CurrentBuildingID);
            l_FollowerStmt->setString(l_Index++, l_Abilities.str());
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].Flags);
            l_FollowerStmt->setString(l_Index++, m_Followers[l_I].ShipName);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].DatabaseID);
            l_FollowerStmt->setUInt32(l_Index++, m_ID);

            l_GarrisonTransaction->Append(l_FollowerStmt);
        }

        CharacterDatabase.CommitTransaction(l_GarrisonTransaction);
    }

    /// Update Follower in database
    void Manager::SaveFollowersToDB(uint32 p_Entry /*= 0*/)
    {
        SQLTransaction l_GarrisonTransaction = CharacterDatabase.BeginTransaction();

        for (uint32 l_I = 0; l_I < m_Followers.size(); ++l_I)
        {
            /// Save only one if specified
            if (p_Entry && p_Entry != m_Followers[l_I].FollowerID)
                continue;

            std::ostringstream l_Abilities;

            for (uint32 l_Y = 0; l_Y < m_Followers[l_I].Abilities.size(); ++l_Y)
                l_Abilities << m_Followers[l_I].Abilities[l_Y] << ' ';

            PreparedStatement* l_FollowerStmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GARRISON_FOLLOWER);

            uint8 l_Index = 0;
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].FollowerID);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].Level);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].XP);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].Quality);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].ItemLevelArmor);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].ItemLevelWeapon);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].CurrentMissionID);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].CurrentBuildingID);
            l_FollowerStmt->setString(l_Index++, l_Abilities.str());
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].Flags);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].DatabaseID);
            l_FollowerStmt->setUInt32(l_Index++, m_ID);

            l_GarrisonTransaction->Append(l_FollowerStmt);
        }

        CharacterDatabase.CommitTransaction(l_GarrisonTransaction);
    }

    /// Delete garrison
    void Manager::DeleteFromDB(uint64 p_PlayerGUID, SQLTransaction p_Transation)
    {
        PreparedStatement* l_Stmt;

        l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_BUILDINGS);
        l_Stmt->setUInt32(0, p_PlayerGUID);
        p_Transation->Append(l_Stmt);

        l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_MISSIONS);
        l_Stmt->setUInt32(0, p_PlayerGUID);
        p_Transation->Append(l_Stmt);

        l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_FOLLOWERS);
        l_Stmt->setUInt32(0, p_PlayerGUID);
        p_Transation->Append(l_Stmt);

        l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_WORKORDERS);
        l_Stmt->setUInt32(0, p_PlayerGUID);
        p_Transation->Append(l_Stmt);

        l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON);
        l_Stmt->setUInt32(0, p_PlayerGUID);
        p_Transation->Append(l_Stmt);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Update the garrison
    void Manager::Update()
    {
        /// Update buildings
        UpdateBuildings();
        /// Update followers
        UpdateFollowers();
        /// Update cache
        UpdateCache();
        /// Update mission distribution
        UpdateMissionDistribution();
        /// Update garrison ability
        UpdateGarrisonAbility();
        /// Update work order
        UpdateWorkOrders();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Set garrison level
    void Manager::SetLevel(uint32 p_Level)
    {
        m_GarrisonLevel = p_Level;

        UninitPlots();
        Init();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get garrison cache token count
    uint32 Manager::GetGarrisonCacheTokenCount() const
    {
        return m_CacheLastTokenAmount;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get terrain swaps
    void Manager::GetTerrainSwaps(std::set<uint32>& p_TerrainSwaps) const
    {
        if (!GetGarrisonSiteLevelEntry())
            return;

        if (GetGarrisonFactionIndex() == FactionIndex::Horde)
        {
            switch (GetGarrisonSiteLevelEntry()->Level)
            {
                case 1:
                    p_TerrainSwaps.emplace(TERRAIN_SWAP_GARRISON_FF_HORDE_TIER_1);
                    break;

                case 2:
                    p_TerrainSwaps.emplace(TERRAIN_SWAP_GARRISON_FF_HORDE_TIER_2);
                    break;

                case 3:
                    p_TerrainSwaps.emplace(TERRAIN_SWAP_GARRISON_FF_HORDE_TIER_3);
                    break;
            }
        }
        else
        {
            switch (GetGarrisonSiteLevelEntry()->Level)
            {
                case 1:
                    p_TerrainSwaps.emplace(TERRAIN_SWAP_GARRISON_SMV_ALLIANCE_TIER_1);
                    break;

                case 2:
                    p_TerrainSwaps.emplace(TERRAIN_SWAP_GARRISON_SMV_ALLIANCE_TIER_2);
                    break;

                case 3:
                    p_TerrainSwaps.emplace(TERRAIN_SWAP_GARRISON_SMV_ALLIANCE_TIER_3);
                    break;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get garrison script
    Interfaces::GarrisonSite* Manager::GetGarrisonScript() const
    {
        return m_GarrisonScript;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Can upgrade the garrison
    bool Manager::CanUpgrade() const
    {
        if (m_Owner->isGameMaster() && m_GarrisonLevel < 3)
            return true;

        if (!m_Owner->HasCurrency(Globals::CurrencyID, GetGarrisonSiteLevelEntry()->UpgradeCost))
            return false;

        if (!m_Owner->HasEnoughMoney((int64)GetGarrisonSiteLevelEntry()->UpgradeMoneyCost))
            return false;

        Interfaces::GarrisonSite* l_GarrisonScript = GetGarrisonScript();

        if (l_GarrisonScript)
            return l_GarrisonScript->CanUpgrade(m_Owner, m_GarrisonLevel);

        return false;
    }

    /// Upgrade the garrison
    void Manager::Upgrade()
    {
        if (!m_Owner->isGameMaster())
        {
            m_Owner->ModifyCurrency(Globals::CurrencyID, -((int32)GetGarrisonSiteLevelEntry()->UpgradeCost));
            m_Owner->ModifyMoney(-((int64)GetGarrisonSiteLevelEntry()->UpgradeMoneyCost));
        }

        SetLevel(m_GarrisonLevel + 1);

        Interfaces::GarrisonSite* l_GarrisonScript = GetGarrisonScript();

        if (l_GarrisonScript)
            l_GarrisonScript->OnUpgrade(m_Owner);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Reward garrison cache content
    void Manager::RewardGarrisonCache()
    {
        m_Owner->SendDisplayToast(Globals::CurrencyID, m_CacheLastTokenAmount, DISPLAY_TOAST_METHOD_GARRISON_CACHE, TOAST_TYPE_NEW_CURRENCY, false, false);
        m_Owner->ModifyCurrency(Globals::CurrencyID, m_CacheLastTokenAmount, false);

        m_CacheLastTokenAmount  = 0;
        m_CacheLastUsage        = time(0);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the garrison owner enter in the garrisson (@See Player::UpdateArea)
    void Manager::OnPlayerEnter()
    {
        InitPlots();    ///< AKA update plots

        /// Enable AI Client collision manager
        m_Owner->SetFlag(UNIT_FIELD_NPC_FLAGS + 1, UNIT_NPC_FLAG2_AI_OBSTACLE);

        Interfaces::GarrisonSite* l_GarrisonScript = GetGarrisonScript();

        if (l_GarrisonScript)
            m_Owner->SetPhaseMask(l_GarrisonScript->GetPhaseMask(m_Owner), true);

        for (std::map<uint32, uint64>::iterator l_It = m_PlotsActivateGob.begin(); l_It != m_PlotsActivateGob.end(); ++l_It)
        {
            if (GameObject* l_Gob = HashMapHolder<GameObject>::Find(l_It->second))
                l_Gob->SendGameObjectActivateAnimKit(1696);
        }
    }

    /// When the garrison owner leave the garrisson (@See Player::UpdateArea)
    void Manager::OnPlayerLeave()
    {
        UninitPlots();

        if (m_CacheGameObjectGUID)
        {
            GameObject* l_Cache = HashMapHolder<GameObject>::Find(m_CacheGameObjectGUID);

            if (l_Cache)
            {
                l_Cache->DestroyForNearbyPlayers();
                l_Cache->AddObjectToRemoveList();
            }

            m_CacheGameObjectGUID = 0;
        }

        m_Owner->SetPhaseMask(1, true);

        /// Disable AI Client collision manager
        m_Owner->RemoveFlag(UNIT_FIELD_NPC_FLAGS + 1, UNIT_NPC_FLAG2_AI_OBSTACLE);
    }

    /// When the garrison owner started a quest
    void Manager::OnQuestStarted(const Quest* p_Quest)
    {
        Interfaces::GarrisonSite* l_GarrisonScript = GetGarrisonScript();

        if (l_GarrisonScript)
        {
            /// Broadcast event
            l_GarrisonScript->OnQuestStarted(m_Owner, p_Quest);
            /// Update phasing
            m_Owner->SetPhaseMask(l_GarrisonScript->GetPhaseMask(m_Owner), true);
        }
    }

    /// When the garrison owner reward a quest
    void Manager::OnQuestReward(const Quest* p_Quest)
    {
        Interfaces::GarrisonSite* l_GarrisonScript = GetGarrisonScript();

        if (l_GarrisonScript)
        {
            /// Broadcast event
            l_GarrisonScript->OnQuestReward(m_Owner, p_Quest);
            /// Update phasing
            m_Owner->SetPhaseMask(l_GarrisonScript->GetPhaseMask(m_Owner), true);
        }
    }

    /// When the garrison owner abandon a quest
    void Manager::OnQuestAbandon(const Quest* p_Quest)
    {
        Interfaces::GarrisonSite* l_GarrisonScript = GetGarrisonScript();

        if (l_GarrisonScript)
        {
            /// Broadcast event
            l_GarrisonScript->OnQuestAbandon(m_Owner, p_Quest);
            /// Update phasing
            m_Owner->SetPhaseMask(l_GarrisonScript->GetPhaseMask(m_Owner), true);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the owner player change level
    /// @p_Level : New owner level
    void Manager::OnOwnerLevelChange(uint32 p_Level)
    {
        Interfaces::GarrisonSite* l_GarrisonScript = GetGarrisonScript();

        if (l_GarrisonScript)
        {
            /// Broadcast event
            l_GarrisonScript->OnOwnerLevelChange(p_Level);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// set last used activation game  object
    void Manager::SetLastUsedActivationGameObject(uint64 p_Guid)
    {
        m_LastUsedActivationGameObject = p_Guid;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get GarrSiteLevelEntry for current garrison
    const GarrSiteLevelEntry* Manager::GetGarrisonSiteLevelEntry() const
    {
        return sGarrSiteLevelStore.LookupEntry(m_GarrisonLevelID);
    }

    /// Get Garrison Faction Index
    FactionIndex::Type Manager::GetGarrisonFactionIndex() const
    {
        assert(m_Owner);

        switch (m_Owner->GetTeam())
        {
            case ALLIANCE:
                return FactionIndex::Alliance;

            case HORDE:
                return FactionIndex::Horde;
        }

        return FactionIndex::Horde;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get slots for level
    std::vector<GarrisonPlotInstanceInfoLocation> Manager::GetPlots()
    {
        return m_Plots;
    }

    /// Get plot by position
    GarrisonPlotInstanceInfoLocation Manager::GetPlot(float p_X, float p_Y, float p_Z)
    {
        GarrisonPlotInstanceInfoLocation l_Plot = { 0, 0, 0.0f, 0.0f, 0.0f, 0.0f} ;

        if (m_Owner && !m_Owner->IsInGarrison())
            return l_Plot;

        Position                            l_Position;

        memset(&l_Plot, 0, sizeof(GarrisonPlotInstanceInfoLocation));

        l_Position.m_positionX = p_X;
        l_Position.m_positionY = p_Y;
        l_Position.m_positionZ = p_Z;

        for (uint32 l_I = 0; l_I < m_Plots.size(); ++l_I)
        {
            if (l_Position.GetExactDist2d(m_Plots[l_I].X, m_Plots[l_I].Y) < l_Position.GetExactDist2d(l_Plot.X, l_Plot.Y))
            {
                /// Specific check for mine, plot surface is way bigger than other plots
                if (GetBuilding(m_Plots[l_I].PlotInstanceID).BuildingID == 61 || l_Position.GetExactDist2d(m_Plots[l_I].X, m_Plots[l_I].Y) < 35.0f)
                    l_Plot = m_Plots[l_I];
            }
        }

        return l_Plot;
    }

    /// Get plot instance plot type
    uint32 Manager::GetPlotType(uint32 p_PlotInstanceID) const
    {
        GarrPlotInstanceEntry const* l_PlotInstanceEntry = sGarrPlotInstanceStore.LookupEntry(p_PlotInstanceID);

        if (!l_PlotInstanceEntry)
            return 0;

        GarrPlotEntry const* l_PlotEntry = sGarrPlotStore.LookupEntry(l_PlotInstanceEntry->PlotID);

        if (!l_PlotEntry)
            return 0;

        return l_PlotEntry->PlotType;
    }

    /// Plot is free ?
    bool Manager::PlotIsFree(uint32 p_PlotInstanceID) const
    {
        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
            if (m_Buildings[l_I].PlotInstanceID == p_PlotInstanceID)
                return false;

        return true;
    }

    /// Has plot instance
    bool Manager::HasPlotInstance(uint32 p_PlotInstanceID) const
    {
        for (uint32 l_I = 0; l_I < m_Plots.size(); ++l_I)
            if (m_Plots[l_I].PlotInstanceID == p_PlotInstanceID)
                return true;

        return false;
    }

    /// Get plot location
    GarrisonPlotInstanceInfoLocation Manager::GetPlot(uint32 p_PlotInstanceID) const
    {
        for (uint32 l_I = 0; l_I < m_Plots.size(); ++l_I)
        {
            if (m_Plots[l_I].SiteLevelID == m_GarrisonLevelID && m_Plots[l_I].PlotInstanceID == p_PlotInstanceID)
                return m_Plots[l_I];
        }

        return GarrisonPlotInstanceInfoLocation();
    }

    /// Get plot instance ID by activation game object
    uint32 Manager::GetPlotInstanceIDByActivationGameObject(uint64 p_Guid) const
    {
        for (std::map<uint32, uint64>::const_iterator l_It = m_PlotsActivateGob.begin(); l_It != m_PlotsActivateGob.end(); ++l_It)
            if (l_It->second == p_Guid)
                return l_It->first;

        return 0;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Add mission
    bool Manager::AddMission(uint32 p_MissionRecID)
    {
        GarrMissionEntry const* l_MissionEntry = sGarrMissionStore.LookupEntry(p_MissionRecID);

        if (!l_MissionEntry || DisableMgr::IsDisabledFor(DISABLE_TYPE_GARRISON_MISSION, p_MissionRecID, m_Owner))
            return false;

        uint32 l_Count = (uint32)std::count_if(m_Missions.begin(), m_Missions.end(), [p_MissionRecID](const GarrisonMission& p_Mission)
        {
            return p_Mission.MissionID == p_MissionRecID;
        });

        if (l_Count)
            return false;

        std::vector<GarrisonFollower> l_Followers = GetFollowers();
        uint32 l_FollowermaxLevel = 0;
        uint32 l_MaxIlvl = 0;

        for (auto l_Follower : l_Followers)
        {
            if (l_Follower.Level < 100)
            {
                l_FollowermaxLevel = l_Follower.Level;
                continue;
            }

            uint32 l_MaxFollowerIlvl = std::round((l_Follower.ItemLevelArmor + l_Follower.ItemLevelWeapon) / 2);

            if (l_MaxFollowerIlvl > l_MaxIlvl)
                l_MaxIlvl = l_MaxFollowerIlvl;
        }

        if (!l_MaxIlvl && (uint32)l_MissionEntry->RequiredLevel > l_FollowermaxLevel)
            return false;
        else if (l_MaxIlvl && (uint32)l_MissionEntry->RequiredItemLevel > l_MaxIlvl)
            return false;

        GarrisonMission l_Mission;
        l_Mission.DatabaseID        = sObjectMgr->GetNewGarrisonMissionID();
        l_Mission.MissionID         = p_MissionRecID;
        l_Mission.OfferTime         = time(0);
        l_Mission.OfferMaxDuration  = l_MissionEntry->OfferTime;
        l_Mission.State             = MissionStates::Available;
        l_Mission.StartTime         = 0;

        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GARRISON_MISSION);

        uint32 l_Index = 0;
        l_Stmt->setUInt32(l_Index++, l_Mission.DatabaseID);
        l_Stmt->setUInt32(l_Index++, m_ID);
        l_Stmt->setUInt32(l_Index++, l_Mission.MissionID);
        l_Stmt->setUInt32(l_Index++, l_Mission.OfferTime);
        l_Stmt->setUInt32(l_Index++, l_Mission.OfferMaxDuration);
        l_Stmt->setUInt32(l_Index++, l_Mission.StartTime);
        l_Stmt->setUInt32(l_Index++, l_Mission.State);

        CharacterDatabase.AsyncQuery(l_Stmt);

        m_Missions.push_back(l_Mission);

        WorldPacket l_AddMissionResult(SMSG_GARRISON_ADD_MISSION_RESULT, 40);
        l_AddMissionResult << uint32(PurchaseBuildingResults::Ok);
        l_AddMissionResult << uint8(l_Mission.State);

        l_AddMissionResult << uint64(l_Mission.DatabaseID);
        l_AddMissionResult << uint32(l_Mission.MissionID);
        l_AddMissionResult << uint32(l_Mission.OfferTime);
        l_AddMissionResult << uint32(l_Mission.OfferMaxDuration);
        l_AddMissionResult << uint32(l_Mission.StartTime);
        l_AddMissionResult << uint32(0);   ///< Travel duration
        l_AddMissionResult << uint32(0);   ///< Mission duration
        l_AddMissionResult << uint32(l_Mission.State);

        l_AddMissionResult.WriteBit(false);
        l_AddMissionResult.FlushBits();

        m_Owner->SendDirectMessage(&l_AddMissionResult);

        return true;
    }

    /// Player have mission
    bool Manager::HasMission(uint32 p_MissionRecID)  const
    {
        for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        {
            if (m_Missions[l_I].MissionID == p_MissionRecID)
            {
                if (m_Missions[l_I].State < MissionStates::CompleteSuccess
                    && !((m_Missions[l_I].OfferTime + m_Missions[l_I].OfferMaxDuration) < time(0) && m_Missions[l_I].State == MissionStates::Available))
                    return true;
            }
        }

        return false;
    }

    /// Start mission
    void Manager::StartMission(uint32 p_MissionRecID, std::vector<uint64> p_Followers)
    {
        if (!HasMission(p_MissionRecID))
        {
            StartMissionFailed(p_MissionRecID, p_Followers);
            return;
        }

        GarrMissionEntry const* l_MissionTemplate = sGarrMissionStore.LookupEntry(p_MissionRecID);

        if (CanMissionBeStartedAfterSoftCap(l_MissionTemplate->FollowerType) || (GetActiveFollowerCount(l_MissionTemplate->FollowerType) > GetFollowerSoftCap(l_MissionTemplate->FollowerType)))
            return;

        if (l_MissionTemplate->CurrencyCost && !m_Owner->HasCurrency(Globals::CurrencyID, l_MissionTemplate->CurrencyCost))
        {
            StartMissionFailed(p_MissionRecID, p_Followers);
            return;
        }

        if (p_Followers.size() < l_MissionTemplate->RequiredFollowersCount)
        {
            StartMissionFailed(p_MissionRecID, p_Followers);
            return;
        }

        for (uint32 l_I = 0; l_I < p_Followers.size(); ++l_I)
        {
            std::vector<GarrisonFollower>::iterator l_It = std::find_if(m_Followers.begin(), m_Followers.end(), [this, p_Followers, l_I](const GarrisonFollower p_Follower) -> bool
            {
                if (p_Follower.DatabaseID == uint32(p_Followers[l_I]))
                    return true;

                return false;
            });

            if (l_It == m_Followers.end())
            {
                StartMissionFailed(p_MissionRecID, p_Followers);
                return;
            }

            if (l_It->CurrentBuildingID != 0 || l_It->CurrentMissionID != 0)
            {
                StartMissionFailed(p_MissionRecID, p_Followers);
                return;
            }

            if (l_It->GetEntry()->Type != l_MissionTemplate->FollowerType)
            {
                StartMissionFailed(p_MissionRecID, p_Followers);
                return;
            }

            /// Should not happen
            if (l_It->Flags & GARRISON_FOLLOWER_FLAG_INACTIVE)
                return;
        }

        if (l_MissionTemplate->CurrencyCost)
            m_Owner->ModifyCurrency(Globals::CurrencyID, -(int32)l_MissionTemplate->CurrencyCost);

        std::vector<uint32> l_FollowersIDs;
        for (uint32 l_I = 0; l_I < p_Followers.size(); ++l_I)
        {
            std::vector<GarrisonFollower>::iterator l_It = std::find_if(m_Followers.begin(), m_Followers.end(), [this, p_Followers, l_I](const GarrisonFollower p_Follower) -> bool
            {
                if (p_Follower.DatabaseID == p_Followers[l_I])
                    return true;

                return false;
            });

            l_It->CurrentMissionID = p_MissionRecID;
            l_FollowersIDs.push_back(l_It->FollowerID);
        }

        GarrisonMission * l_Mission = nullptr;

        for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        {
            if (m_Missions[l_I].MissionID == p_MissionRecID)
            {
                l_Mission = &m_Missions[l_I];
                break;
            }
        }

        if (l_Mission)
        {
            l_Mission->State        = MissionStates::InProgress;
            l_Mission->StartTime    = time(0);
            l_Mission->OfferTime    = time(0);

            uint32 l_TravelDuration  = GetMissionTravelDuration(p_MissionRecID);
            uint32 l_MissionDuration = GetMissionDuration(p_MissionRecID);

            WorldPacket l_Result(SMSG_GARRISON_START_MISSION_RESULT, 200);
            l_Result << uint32(0);    ///< Result (0 = OK, 1 = failed)

            l_Result << uint64(l_Mission->DatabaseID);
            l_Result << uint32(l_Mission->MissionID);
            l_Result << uint32(l_Mission->OfferTime);
            l_Result << uint32(l_Mission->OfferMaxDuration);
            l_Result << uint32(l_Mission->StartTime);
            l_Result << uint32(l_TravelDuration);
            l_Result << uint32(l_MissionDuration);
            l_Result << uint32(l_Mission->State);

            l_Result << uint32(p_Followers.size());

            for (uint32 l_I = 0; l_I < p_Followers.size(); ++l_I)
                l_Result << uint64(p_Followers[l_I]);

            m_Owner->SendDirectMessage(&l_Result);

            if (GetGarrisonScript())
                GetGarrisonScript()->OnMissionStart(m_Owner, p_MissionRecID, l_FollowersIDs);
        }
    }

    /// Send mission start failed packet
    void Manager::StartMissionFailed(uint32 p_MissionRecID, std::vector<uint64> p_Followers)
    {
        WorldPacket l_Data(SMSG_GARRISON_START_MISSION_RESULT, 200);
        l_Data << uint32(1);    ///< Result (0 = OK, 1 = failed)

        GarrMissionEntry const* l_MissionTemplate = sGarrMissionStore.LookupEntry(p_MissionRecID);
        GarrisonMission * l_Mission = nullptr;

        for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        {
            if (m_Missions[l_I].MissionID == p_MissionRecID)
            {
                l_Mission = &m_Missions[l_I];
                break;
            }
        }

        if (l_Mission)
        {
            l_Data << uint64(l_Mission->DatabaseID);
            l_Data << uint32(l_Mission->MissionID);
            l_Data << uint32(l_Mission->OfferTime);
            l_Data << uint32(l_Mission->OfferMaxDuration);
            l_Data << uint32(l_Mission->StartTime);
            l_Data << uint32(0);                            ///< TravelDuration
            l_Data << uint32(l_MissionTemplate->Duration);
            l_Data << uint32(l_Mission->State);

            l_Data << uint32(p_Followers.size());
        }
        else
        {
            /// Mission details
            l_Data << uint64(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
        }

        for (uint32 l_I = 0; l_I < p_Followers.size(); ++l_I)
            l_Data << uint64(p_Followers[l_I]);

        m_Owner->SendDirectMessage(&l_Data);
    }

    /// Complete a mission
    void Manager::CompleteMission(uint32 p_MissionRecID)
    {
        if (!HasMission(p_MissionRecID))
            return;

        GarrMissionEntry const* l_MissionTemplate = sGarrMissionStore.LookupEntry(p_MissionRecID);

        GarrisonMission* l_Mission = nullptr;

        for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        {
            if (m_Missions[l_I].MissionID == p_MissionRecID)
            {
                l_Mission = &m_Missions[l_I];
                break;
            }
        }

        if (l_Mission == nullptr)
            return;

        uint32 l_TravelDuration  = GetMissionTravelDuration(p_MissionRecID);
        uint32 l_MissionDuration = GetMissionDuration(p_MissionRecID);

        std::vector<GarrisonFollower*> l_MissionFollowers = GetMissionFollowers(p_MissionRecID);

        /// Hack attempt
        if ((l_Mission->StartTime + (l_TravelDuration + l_MissionDuration)) > time(0))
            return;

        uint32 l_ChestChance = GetMissionSuccessChance(p_MissionRecID);

        bool l_Succeeded   = roll_chance_i(l_ChestChance);  ///< Seems to be MissionChance
        bool l_CanComplete = true;

        l_Mission->State = l_Succeeded ? MissionStates::CompleteSuccess : MissionStates::CompleteFailed;

        WorldPacket l_Result(SMSG_GARRISON_COMPLETE_MISSION_RESULT, 100);

        l_Result << uint32(!l_CanComplete);

        l_Result << uint64(l_Mission->DatabaseID);
        l_Result << uint32(l_Mission->MissionID);
        l_Result << uint32(l_Mission->OfferTime);
        l_Result << uint32(l_Mission->OfferMaxDuration);
        l_Result << uint32(l_Mission->StartTime);
        l_Result << uint32(l_TravelDuration);
        l_Result << uint32(l_MissionDuration);
        l_Result << uint32(l_Mission->State);

        l_Result << uint32(l_Mission->MissionID);
        l_Result << uint32(0);  ///< Unk Loop
        l_Result.WriteBit(l_Succeeded);
        l_Result.FlushBits();

        m_Owner->SendDirectMessage(&l_Result);

        std::vector<uint32> l_PartyXPModifiersEffect = GetMissionFollowersAbilitiesEffects(p_MissionRecID, AbilityEffectTypes::ModXpGain, AbilityEffectTargetMasks::Unk | AbilityEffectTargetMasks::Party);
        std::vector<uint32> l_PassiveEffects         = GetBuildingsPassiveAbilityEffects();

        /// Global XP Bonus modifier
        float l_XPModifier = 1.0f;
        for (uint32 l_I = 0; l_I < l_PartyXPModifiersEffect.size(); ++l_I)
        {
            GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_PartyXPModifiersEffect[l_I]);

            if (!l_AbilityEffectEntry)
                continue;

            l_XPModifier = (l_AbilityEffectEntry->Amount - 1.0) + l_XPModifier;
        }

        for (uint32 l_Y = 0; l_Y < l_PassiveEffects.size(); ++l_Y)
        {
            GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_PassiveEffects[l_Y]);

            if (!l_AbilityEffectEntry)
                continue;

            if (l_AbilityEffectEntry->EffectType == AbilityEffectTypes::ModXpGain && (l_AbilityEffectEntry->TargetMask == AbilityEffectTargetMasks::Party || l_AbilityEffectEntry->TargetMask == AbilityEffectTargetMasks::Unk))
                l_XPModifier = (l_AbilityEffectEntry->Amount - 1.0) + l_XPModifier;
        }
        /// ------------------------------------------

        float l_BonusXP = (l_XPModifier - 1.0f) * l_MissionTemplate->RewardFollowerExperience;

        for (uint32 l_FollowerIt = 0; l_FollowerIt < l_MissionFollowers.size(); ++l_FollowerIt)
        {
            if (!l_MissionFollowers[l_FollowerIt]->CanXP())
                continue;

            uint32 l_FollowerLevel = l_MissionFollowers[l_FollowerIt]->Level;
            float l_SecondXPModifier = 1.0f;

            /// Personal XP Bonus
            for (uint32 l_AbilityIt = 0; l_AbilityIt < l_MissionFollowers[l_FollowerIt]->Abilities.size(); l_AbilityIt++)
            {
                uint32 l_CurrentAbilityID = l_MissionFollowers[l_FollowerIt]->Abilities[l_AbilityIt];

                for (uint32 l_EffectIt = 0; l_EffectIt < sGarrAbilityEffectStore.GetNumRows(); l_EffectIt++)
                {
                    GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectIt);

                    if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
                        continue;

                    if (l_AbilityEffectEntry->EffectType == AbilityEffectTypes::ModXpGain && l_AbilityEffectEntry->TargetMask == AbilityEffectTargetMasks::Self)
                        l_SecondXPModifier = (l_AbilityEffectEntry->Amount - 1.0) + l_SecondXPModifier;
                }
            }

            uint32 l_AddedXP = (l_BonusXP + l_MissionTemplate->RewardFollowerExperience) * l_SecondXPModifier;
            l_AddedXP = l_MissionFollowers[l_FollowerIt]->EarnXP(l_AddedXP, m_Owner); ///< l_addedXP is never read 01/18/16

            if (l_FollowerLevel != l_MissionFollowers[l_FollowerIt]->Level && l_MissionFollowers[l_FollowerIt]->Level == 100) ///< Comparison of integers of different signs: 'const uint32' (aka 'const unsigned int') and 'const int32' (aka 'const int')
                m_Owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LEVELUP_FOLLOWERS);
        }

        m_PendingMissionReward.RewardFollowerXPBonus.clear();
        m_PendingMissionReward.MissionFollowers.clear();
        m_PendingMissionReward.RewardCurrencies.clear();
        m_PendingMissionReward.RewardItems.clear();
        m_PendingMissionReward.RewardGold       = 0;
        m_PendingMissionReward.RewardFollowerXP = 0;
        m_PendingMissionReward.Rewarded         = false;
        m_PendingMissionReward.MissionID        = p_MissionRecID;

        if (l_Succeeded)
        {
            for (uint32 l_I = 0; l_I < l_MissionFollowers.size(); ++l_I)
            {
                m_PendingMissionReward.MissionFollowers.push_back(l_MissionFollowers[l_I]->DatabaseID);
            }

            for (uint32 l_I = 0; l_I < sGarrMissionRewardStore.GetNumRows(); ++l_I)
            {
                GarrMissionRewardEntry const* l_RewardEntry = sGarrMissionRewardStore.LookupEntry(l_I);

                if (!l_RewardEntry || l_RewardEntry->MissionID != p_MissionRecID)
                    continue;

                if (l_RewardEntry->ItemID)
                    m_PendingMissionReward.RewardItems.push_back(std::make_pair(l_RewardEntry->ItemID, l_RewardEntry->ItemQuantity));

                if (l_RewardEntry->RewardCurrencyID == 0)
                    m_PendingMissionReward.RewardGold += l_RewardEntry->RewardCurrencyAmount;

                if (l_RewardEntry->RewardCurrencyID)
                {
                    uint32 l_Amount = l_RewardEntry->RewardCurrencyAmount;

                    if (l_RewardEntry->RewardCurrencyID == Globals::CurrencyID)
                    {
                        std::vector<uint32> l_PartyCurrencyModifiersEffect = GetMissionFollowersAbilitiesEffects(p_MissionRecID, AbilityEffectTypes::ModGarrCurrencyDrop, AbilityEffectTargetMasks::Unk | AbilityEffectTargetMasks::Party);

                        /// Global currency Bonus modifier
                        float l_Modifier = 1.0f;
                        for (uint32 l_I = 0; l_I < l_PartyCurrencyModifiersEffect.size(); ++l_I)
                        {
                            GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_PartyCurrencyModifiersEffect[l_I]);

                            if (!l_AbilityEffectEntry)
                                continue;

                            l_Modifier = (l_AbilityEffectEntry->Amount - 1.0) + l_Modifier;
                        }

                        l_Amount += (l_Modifier - 1.0f) * l_Amount;
                    }

                    m_PendingMissionReward.RewardCurrencies.push_back(std::make_pair(l_RewardEntry->RewardCurrencyID, l_Amount));
                }

                if (l_RewardEntry->PlayerRewardXP)
                    m_PendingMissionReward.RewardFollowerXP += l_RewardEntry->PlayerRewardXP;
            }

            switch (l_MissionTemplate->RequiredLevel)
            {
                case 90:
                case 91:
                case 92:
                case 93:
                case 94:
                {
                    if (HasActiveBuilding(Buildings::SalvageYard_SalvageYard_Level1))
                    {
                        if (roll_chance_i(30))
                            m_PendingMissionReward.RewardItems.push_back(std::make_pair(Items::ItemBagOfSalvagedGoods, 1));
                    }
                    else if (HasActiveBuilding(Buildings::SalvageYard_SalvageYard_Level2))
                    {
                        if (roll_chance_i(50))
                            m_PendingMissionReward.RewardItems.push_back(std::make_pair(Items::ItemBagOfSalvagedGoods, 1));
                    }
                    else if (HasActiveBuilding(Buildings::SalvageYard_SalvageYard_Level3))
                    {
                        if (roll_chance_i(75))
                            m_PendingMissionReward.RewardItems.push_back(std::make_pair(Items::ItemBagOfSalvagedGoods, 1));
                    }
                    break;
                }
                case 95:
                case 96:
                case 97:
                case 98:
                case 99:
                {
                    if (HasActiveBuilding(Buildings::SalvageYard_SalvageYard_Level1))
                    {
                        if (roll_chance_i(50))
                            m_PendingMissionReward.RewardItems.push_back(std::make_pair(Items::ItemBagOfSalvagedGoods, 1));
                    }
                    else if (HasActiveBuilding(Buildings::SalvageYard_SalvageYard_Level2))
                    {
                        if (roll_chance_i(30))
                            m_PendingMissionReward.RewardItems.push_back(std::make_pair(Items::ItemCrateOfSalvage, 1));
                    }
                    else if (HasActiveBuilding(Buildings::SalvageYard_SalvageYard_Level3))
                    {
                        if (roll_chance_i(50))
                            m_PendingMissionReward.RewardItems.push_back(std::make_pair(Items::ItemCrateOfSalvage, 1));
                    }
                    break;
                }
                case 100:
                {
                    if (HasActiveBuilding(Buildings::SalvageYard_SalvageYard_Level1))
                    {
                        if (roll_chance_i(75))
                            m_PendingMissionReward.RewardItems.push_back(std::make_pair(Items::ItemBagOfSalvagedGoods, 1));
                    }
                    else if (HasActiveBuilding(Buildings::SalvageYard_SalvageYard_Level2))
                    {
                        if (roll_chance_i(50))
                            m_PendingMissionReward.RewardItems.push_back(std::make_pair(Items::ItemCrateOfSalvage, 1));
                    }
                    else if (HasActiveBuilding(Buildings::SalvageYard_SalvageYard_Level3))
                    {
                        if (roll_chance_i(50))
                            m_PendingMissionReward.RewardItems.push_back(std::make_pair(Items::ItemBigCrateOfSalvage, 1));
                    }
                    break;
                }
                default:
                    break;
            }

            /// @TODO fix this
            ///l_BonusXP = (l_XPModifier - 1.0f) * m_PendingMissionReward.RewardFollowerXP;
            ///m_PendingMissionReward.RewardFollowerXP += l_BonusXP;
            ///
            ///for (uint32 l_FollowerIt = 0; l_FollowerIt < l_MissionFollowers.size(); ++l_FollowerIt)
            ///{
            ///    float l_SecondXPModifier = 1.0f;
            ///
            ///    /// Personnal XP Bonus
            ///    for (uint32 l_AbilityIt = 0; l_AbilityIt < l_MissionFollowers[l_FollowerIt]->Abilities.size(); l_AbilityIt++)
            ///    {
            ///        uint32 l_CurrentAbilityID = l_MissionFollowers[l_FollowerIt]->Abilities[l_AbilityIt];
            ///
            ///        for (uint32 l_EffectIt = 0; l_EffectIt < sGarrAbilityEffectStore.GetNumRows(); l_EffectIt++)
            ///        {
            ///            const GarrAbilityEffectEntry * l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectIt);
            ///
            ///            if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
            ///                continue;
            ///
            ///            if (l_AbilityEffectEntry->EffectType == GARRISION_ABILITY_EFFECT_MOD_XP_GAIN && l_AbilityEffectEntry->TargetMask == GARRISON_ABILITY_EFFECT_TARGET_MASK_SELF)
            ///                l_SecondXPModifier = (l_AbilityEffectEntry->Amount - 1.0) + l_SecondXPModifier;
            ///        }
            ///    }
            ///
            ///    m_PendingMissionReward.RewardFollowerXPBonus.push_back(std::make_pair(l_MissionFollowers[l_FollowerIt]->DB_ID, (l_BonusXP + m_PendingMissionReward.RewardFollowerXP) * l_SecondXPModifier));
            ///}
        }

        /// Unassign follower to the mission
        for (uint32 l_I = 0; l_I < m_Followers.size(); ++l_I)
        {
            if (m_Followers[l_I].CurrentMissionID == p_MissionRecID)
            {
                m_Followers[l_I].CurrentMissionID = 0;
                m_Followers[l_I].SendFollowerUpdate(m_Owner);
            }
        }
    }

    /// Do mission bonus roll
    void Manager::DoMissionBonusRoll(uint64 p_MissionTableGUID, uint32 p_MissionRecID)
    {
        auto l_It = std::find_if(m_Missions.begin(), m_Missions.end(), [p_MissionRecID](const GarrisonMission& p_Mission) -> bool
        {
            if (p_Mission.State == MissionStates::CompleteFailed || p_Mission.State == MissionStates::CompleteSuccess)
            {
                if (p_Mission.MissionID == p_MissionRecID)
                    return true;
            }

            return false;
        });

        GarrisonMission* l_Mission = nullptr;

        if (l_It != m_Missions.end())
            l_Mission = reinterpret_cast<GarrisonMission*>(&(*l_It));

        if (m_PendingMissionReward.MissionID != p_MissionRecID || m_PendingMissionReward.Rewarded == true || !l_Mission || (l_Mission && l_Mission->State == MissionStates::CompleteFailed))
        {
            m_PendingMissionReward.Rewarded = true;

            WorldPacket l_Packet(SMSG_GARRISON_MISSION_BONUS_ROLL_RESULT, 100);

            if (l_Mission)
            {
                l_Packet << uint64(l_Mission->DatabaseID);
                l_Packet << uint32(l_Mission->MissionID);
                l_Packet << uint32(l_Mission->OfferTime);
                l_Packet << uint32(l_Mission->OfferMaxDuration);
                l_Packet << uint32(l_Mission->StartTime);
                l_Packet << uint32(0);
                l_Packet << uint32(0);
                l_Packet << uint32(l_Mission->State);
            }
            else
            {
                l_Packet << uint64(0);
                l_Packet << uint32(0);
                l_Packet << uint32(0);
                l_Packet << uint32(0);
                l_Packet << uint32(0);
                l_Packet << uint32(0);
                l_Packet << uint32(0);
                l_Packet << uint32(0);
            }

            l_Packet << uint32(p_MissionRecID);
            l_Packet << uint32(MissionBonusRollResults::Error);

            m_Owner->SendDirectMessage(&l_Packet);

            return;
        }

        WorldPacket l_Packet(SMSG_GARRISON_MISSION_BONUS_ROLL_RESULT, 100);

        l_Packet << uint64(l_Mission->DatabaseID);
        l_Packet << uint32(l_Mission->MissionID);
        l_Packet << uint32(l_Mission->OfferTime);
        l_Packet << uint32(l_Mission->OfferMaxDuration);
        l_Packet << uint32(l_Mission->StartTime);
        l_Packet << uint32(0);
        l_Packet << uint32(0);
        l_Packet << uint32(l_Mission->State);

        l_Packet << uint32(p_MissionRecID);
        l_Packet << uint32(MissionBonusRollResults::Ok);

        m_Owner->SendDirectMessage(&l_Packet);

        m_PendingMissionReward.Rewarded = true;

        m_Owner->ModifyMoney(m_PendingMissionReward.RewardGold);

        for (auto l_Currency : m_PendingMissionReward.RewardCurrencies)
        {
            CurrencyTypesEntry const* l_CurrencyEntry = sCurrencyTypesStore.LookupEntry(l_Currency.first);

            if (l_CurrencyEntry)
                m_Owner->ModifyCurrency(l_Currency.first, l_Currency.second, false);
        }

        for (auto l_Item : m_PendingMissionReward.RewardItems)
        {
            const ItemTemplate* l_ItemTemplate = sObjectMgr->GetItemTemplate(l_Item.first);

            if (!l_ItemTemplate)
                continue;

            /// Check if the rewarded item is an follower contract, because make server crash on Player::SendNewItem,
            /// the spell is auto casted by the player, and the spell delete the caster item itself.
            bool l_IsContractItem = false;
            if (l_ItemTemplate->Spells[0].SpellId)
            {
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(l_ItemTemplate->Spells[0].SpellId);

                if (l_SpellInfo && l_SpellInfo->Effects[0].Effect == SPELL_EFFECT_ADD_GARRISON_FOLLOWER)
                {
                    l_IsContractItem = true;
                    m_Owner->CastSpell(m_Owner, l_SpellInfo, TRIGGERED_FULL_MASK);
                }
            }

            if (!l_IsContractItem)
            {
                uint32 l_NoSpaceForCount = 0;
                uint32 l_Count = l_Item.second;
                ItemPosCountVec l_Dest;

                if (m_Owner->CanStoreNewItem(NULL_BAG, NULL_SLOT, l_Dest, l_Item.first, l_Count, &l_NoSpaceForCount) != EQUIP_ERR_OK)
                    l_Count -= l_NoSpaceForCount;

                if (l_Count == 0 || l_Dest.empty())
                {
                    Item* l_NewItem = l_Item.first ? Item::CreateItem(l_Item.first, l_Item.second, m_Owner) : nullptr;

                    int l_LocIDX = m_Owner->GetSession()->GetSessionDbLocaleIndex(); ///< l_LocIDX is never read 01/18/16

                    MailDraft l_Draft("Garrison mission reward", "");

                    SQLTransaction l_Transaction = CharacterDatabase.BeginTransaction();
                    if (l_NewItem)
                    {
                        // /Save new item before send
                        l_NewItem->SaveToDB(l_Transaction);                               /// Save for prevent lost at next mail load, if send fail then item will deleted

                        /// Item
                        l_Draft.AddItem(l_NewItem);
                    }

                    l_Draft.SendMailTo(l_Transaction, m_Owner, MailSender(MAIL_CREATURE, GUID_ENPART(p_MissionTableGUID)));
                    CharacterDatabase.CommitTransaction(l_Transaction);
                }
                else
                    m_Owner->AddItem(l_Item.first, l_Item.second);
            }
        }

        std::vector<GarrisonFollower*> l_MissionFollowers;

        std::for_each(m_Followers.begin(), m_Followers.end(), [this, &l_MissionFollowers](const GarrisonFollower& p_Follower) -> void
        {
            if (std::find(m_PendingMissionReward.MissionFollowers.begin(), m_PendingMissionReward.MissionFollowers.end(), p_Follower.DatabaseID) != m_PendingMissionReward.MissionFollowers.end())
                l_MissionFollowers.push_back(const_cast<GarrisonFollower*>(&p_Follower));
        });

        std::for_each(l_MissionFollowers.begin(), l_MissionFollowers.end(), [this](const GarrisonFollower* p_Follower) -> void
        {
            uint32 l_FollowerLevel = p_Follower->Level;
            uint32 l_AddedXP = m_PendingMissionReward.RewardFollowerXP;

            std::for_each(m_PendingMissionReward.RewardFollowerXPBonus.begin(), m_PendingMissionReward.RewardFollowerXPBonus.end(), [p_Follower, &l_AddedXP](const std::pair<uint64, uint32>& p_Values)
            {
                if (p_Values.first == p_Follower->DatabaseID)
                    l_AddedXP += p_Values.second;
            });

            l_AddedXP = const_cast<GarrisonFollower*>(p_Follower)->EarnXP(l_AddedXP, m_Owner);

            if (l_FollowerLevel != p_Follower->Level && p_Follower->Level == 100) ///< Comparison of integers of different signs: 'const uint32' (aka 'const unsigned int') and 'const int32' (aka 'const int')
                m_Owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LEVELUP_FOLLOWERS);
        });
    }

    /// Set mission has complete
    void Manager::SetAllInProgressMissionAsComplete()
    {
        for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        {
            if (m_Missions[l_I].State == MissionStates::InProgress)
                m_Missions[l_I].StartTime = time(0) - (GetMissionTravelDuration(m_Missions[l_I].MissionID) + GetMissionDuration(m_Missions[l_I].MissionID));
        }

        WorldPacket l_PlaceHolder;
        m_Owner->GetSession()->HandleGetGarrisonInfoOpcode(l_PlaceHolder);
    }

    /// Get followers on a mission
    std::vector<GarrisonFollower*> Manager::GetMissionFollowers(uint32 p_MissionRecID)
    {
        std::vector<GarrisonFollower*> l_MissionFollowers;

        for (uint32 l_I = 0; l_I < m_Followers.size(); ++l_I)
        {
            if (m_Followers[l_I].CurrentMissionID == p_MissionRecID)
                l_MissionFollowers.push_back(&m_Followers[l_I]);
        }

        return l_MissionFollowers;
    }

    /// Get mission followers abilities effect
    std::vector<uint32> Manager::GetMissionFollowersAbilitiesEffects(uint32 p_MissionRecID)
    {
        std::vector<GarrisonFollower*> l_MissionFollowers = GetMissionFollowers(p_MissionRecID);
        std::vector<uint32>            l_AbilitiesEffects;

        for (uint32 l_FollowerIt = 0; l_FollowerIt < l_MissionFollowers.size(); ++l_FollowerIt)
        {
            for (uint32 l_AbilityIt = 0; l_AbilityIt < l_MissionFollowers[l_FollowerIt]->Abilities.size(); l_AbilityIt++)
            {
                uint32 l_CurrentAbilityID = l_MissionFollowers[l_FollowerIt]->Abilities[l_AbilityIt];

                for (uint32 l_EffectIt = 0; l_EffectIt < sGarrAbilityEffectStore.GetNumRows(); l_EffectIt++)
                {
                    GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectIt);

                    if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
                        continue;

                    l_AbilitiesEffects.push_back(l_AbilityEffectEntry->EffectID);
                }
            }
        }

        return l_AbilitiesEffects;
    }

    /// Get mission followers abilities effect
    std::vector<uint32> Manager::GetMissionFollowersAbilitiesEffects(uint32 p_MissionRecID, AbilityEffectTypes::Type p_Type, uint32 p_TargetMask)
    {
        std::vector<GarrisonFollower*> l_MissionFollowers = GetMissionFollowers(p_MissionRecID);
        std::vector<uint32>            l_AbilitiesEffects;

        for (uint32 l_FollowerIt = 0; l_FollowerIt < l_MissionFollowers.size(); ++l_FollowerIt)
        {
            for (uint32 l_AbilityIt = 0; l_AbilityIt < l_MissionFollowers[l_FollowerIt]->Abilities.size(); l_AbilityIt++)
            {
                uint32 l_CurrentAbilityID = l_MissionFollowers[l_FollowerIt]->Abilities[l_AbilityIt];

                for (uint32 l_EffectIt = 0; l_EffectIt < sGarrAbilityEffectStore.GetNumRows(); l_EffectIt++)
                {
                    GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectIt);

                    if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
                        continue;

                    if (l_AbilityEffectEntry->EffectType != p_Type || (l_AbilityEffectEntry->TargetMask != 0 && (l_AbilityEffectEntry->TargetMask & p_TargetMask) == 0))
                        continue;

                    l_AbilitiesEffects.push_back(l_AbilityEffectEntry->EffectID);
                }
            }
        }

        return l_AbilitiesEffects;
    }

    /// Get the mission travel time
    uint32 Manager::GetMissionTravelDuration(uint32 p_MissionRecID)
    {
        GarrMissionEntry const*     l_MissionTemplate = sGarrMissionStore.LookupEntry(p_MissionRecID);
        std::vector<uint32>         l_AbilitiesEffects  = GetMissionFollowersAbilitiesEffects(p_MissionRecID);
        std::vector<uint32>         l_PassiveEffects    = GetBuildingsPassiveAbilityEffects();
        float                       l_MissionTravelTime = l_MissionTemplate->TravelTime;

        for (uint32 l_EffectI = 0; l_EffectI < l_AbilitiesEffects.size(); l_EffectI++)
        {
            GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_AbilitiesEffects[l_EffectI]);

            if (!l_AbilityEffectEntry)
                continue;

            if (l_AbilityEffectEntry->EffectType == AbilityEffectTypes::ModTravelTime)
                l_MissionTravelTime = l_MissionTravelTime * l_AbilityEffectEntry->Amount;
        }

        for (uint32 l_Y = 0; l_Y < l_PassiveEffects.size(); ++l_Y)
        {
            GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_PassiveEffects[l_Y]);

            if (!l_AbilityEffectEntry)
                continue;

            if (l_AbilityEffectEntry->EffectType == AbilityEffectTypes::ModTravelTime)
                l_MissionTravelTime = l_MissionTravelTime * l_AbilityEffectEntry->Amount;
        }

        return floorf(l_MissionTravelTime);
    }

    /// Get the mission duration
    uint32 Manager::GetMissionDuration(uint32 p_MissionRecID)
    {
        GarrMissionEntry const*     l_MissionTemplate   = sGarrMissionStore.LookupEntry(p_MissionRecID);
        std::vector<uint32>         l_AbilitiesEffects  = GetMissionFollowersAbilitiesEffects(p_MissionRecID);
        float                       l_MissionDuration   = l_MissionTemplate->Duration;

        for (uint32 l_EffectI = 0; l_EffectI < l_AbilitiesEffects.size(); l_EffectI++)
        {
            GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_AbilitiesEffects[l_EffectI]);

            if (!l_AbilityEffectEntry)
                continue;

            if (l_AbilityEffectEntry->EffectType == AbilityEffectTypes::ModMissionDuration)
                l_MissionDuration = l_MissionDuration * l_AbilityEffectEntry->Amount;
        }

        return floorf(l_MissionDuration);
    }

    /// Get mission chest chance
    uint32 Manager::GetMissionSuccessChance(uint32 p_MissionRecID)
    {
        GarrMissionEntry const* l_MissionTemplate = sGarrMissionStore.LookupEntry(p_MissionRecID);

        std::vector<uint32>                     l_Encounters;
        std::vector<std::pair<uint32, uint32>>  l_EncoutersMechanics;
        std::vector<GarrisonFollower*>          l_MissionFollowers = GetMissionFollowers(p_MissionRecID);
        std::vector<uint32>                     l_PassiveEffects = GetBuildingsPassiveAbilityEffects();
        std::map<uint64, double>                l_FollowersBiasMap;
        std::vector<uint32>                     l_CounterAbilityUsed;

        for (uint32 l_I = 0; l_I < sGarrMissionXEncouterStore.GetNumRows(); ++l_I)
        {
            GarrMissionXEncouterEntry const* l_Encounter = sGarrMissionXEncouterStore.LookupEntry(l_I);

            if (l_Encounter && l_Encounter->MissionID == p_MissionRecID)
                l_Encounters.push_back(l_Encounter->EncounterID);
        }

        for (uint32 l_I = 0; l_I < l_Encounters.size(); l_I++)
        {
            for (uint32 l_Y = 0; l_Y < sGarrEncouterXMechanicStore.GetNumRows(); ++l_Y)
            {
                GarrEncouterXMechanicEntry const* l_EncounterMechanic = sGarrEncouterXMechanicStore.LookupEntry(l_Y);

                if (l_EncounterMechanic && l_EncounterMechanic->EncounterID == l_Encounters[l_I])
                    l_EncoutersMechanics.push_back(std::make_pair(l_Encounters[l_I], l_EncounterMechanic->MechanicID));
            }
        }

        for (uint32 l_Y = 0; l_Y < l_MissionFollowers.size(); ++l_Y)
        {
            double l_FollowerBias = (l_MissionFollowers[l_Y]->Level - l_MissionTemplate->RequiredLevel) * 0.33333334;

            if (l_MissionTemplate->RequiredLevel == Globals::MaxFollowerLevel)
            {
                if (l_MissionTemplate->RequiredItemLevel > 0)
                {
                    uint32 l_TotalFollowerItemLevel = l_MissionFollowers[l_Y]->ItemLevelArmor + l_MissionFollowers[l_Y]->ItemLevelWeapon;
                    l_FollowerBias = (((l_TotalFollowerItemLevel >> 1) - l_MissionTemplate->RequiredItemLevel) * 0.06666667) + l_FollowerBias;
                }
            }

            if (l_FollowerBias < -1.0)
                l_FollowerBias = -1.0;
            else if (l_FollowerBias > 1.0)
                l_FollowerBias = 1.0;

            l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] = l_FollowerBias;

            #ifdef GARRISON_CHEST_FORMULA_DEBUG
                printf("Follower %u bias %Lf\n", l_MissionFollowers[l_Y]->FollowerID, l_FollowerBias);
            #endif // GARRISON_CHEST_FORMULA_DEBUG
        }

        double l_Float8 = 100.0;
        double l_FloatC = 150.0;

        double l_V8 = l_MissionTemplate->RequiredFollowersCount * l_Float8;
        double l_V60 = l_MissionTemplate->RequiredFollowersCount * l_Float8;

        for (uint32 l_I = 0; l_I < l_EncoutersMechanics.size(); ++l_I)
        {
            GarrMechanicEntry     const* l_MechanicEntry     = sGarrMechanicStore.LookupEntry(l_EncoutersMechanics[l_I].second);
            GarrMechanicTypeEntry const* l_MechanicTypeEntry = sGarrMechanicTypeStore.LookupEntry(l_MechanicEntry->MechanicTypeID);

            if (l_MechanicTypeEntry && l_MechanicTypeEntry->Type != MechanicTypes::Ability)
            {
                l_V8 = l_V60;
            }
            else
            {
                l_V8  = l_MechanicEntry->Unk2 + l_V60;
                l_V60 = l_MechanicEntry->Unk2 + l_V60;
            }
        }

        double l_CurrentAdditionalWinChance = 0;

        double l_V11 = 100.0 / l_V8;
        double l_V62 = 100.0 / l_V8;

        /// OK 100%
        #pragma region Followers Bias
        for (uint32 l_Y = 0; l_Y < l_MissionFollowers.size(); ++l_Y)
        {
            double l_Seil = 0;

            if (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] >= 0.0)
                l_Seil = (l_FloatC - l_Float8) * l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] + l_Float8;
            else
                l_Seil = (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] + 1.0) * l_Float8;

            l_V8 = (l_Seil * l_V11) + l_CurrentAdditionalWinChance; ///< l_V8 is never read 01/18/16
            l_CurrentAdditionalWinChance = (l_Seil * l_V11) + l_CurrentAdditionalWinChance;

            #ifdef GARRISON_CHEST_FORMULA_DEBUG
                printf("Added %.2f to success due to follower %u bias.\n", (l_Seil * l_V11), l_MissionFollowers[l_Y]->FollowerID);
            #endif // GARRISON_CHEST_FORMULA_DEBUG
        }
        #pragma endregion

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        /// OK 100%
        #pragma region Counter mechanic
        for (uint32 l_I = 0; l_I < l_EncoutersMechanics.size(); ++l_I)
        {
            GarrMechanicEntry     const* l_MechanicEntry     = sGarrMechanicStore.LookupEntry(l_EncoutersMechanics[l_I].second);
            GarrMechanicTypeEntry const* l_MechanicTypeEntry = sGarrMechanicTypeStore.LookupEntry(l_MechanicEntry->MechanicTypeID);

            if (l_MechanicTypeEntry->Type == MechanicTypes::Ability)
            {
                double l_Unk1 = l_MechanicEntry->Unk2; ///< l_Unk1 is never read 01/18/16
                double l_Unk2 = l_MechanicEntry->Unk2;

                if (l_MissionFollowers.size() > 0)
                {
                    for (uint32 l_Y = 0; l_Y < l_MissionFollowers.size(); ++l_Y)
                    {
                        for (uint32 l_Z = 0; l_Z < l_MissionFollowers[l_Y]->Abilities.size(); l_Z++)
                        {
                            uint32 l_CurrentAbilityID = l_MissionFollowers[l_Y]->Abilities[l_Z];

                            for (uint32 l_EffectI = 0; l_EffectI < sGarrAbilityEffectStore.GetNumRows(); l_EffectI++)
                            {
                                GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectI);

                                if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
                                    continue;

                                if (l_AbilityEffectEntry->CounterMechanicTypeID == l_MechanicTypeEntry->ID && !(l_AbilityEffectEntry->Unk3 & 1))
                                {
                                    l_Unk1 = l_Unk2; ///< l_Unk1 is never read 01/18/16
                                    if (l_Unk2 != 0.0)
                                    {
                                        float l_Seil = 0;

                                        if (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] >= 0.0)
                                            l_Seil = (l_AbilityEffectEntry->ModMax - l_AbilityEffectEntry->ModMin) * l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] + l_AbilityEffectEntry->ModMin;
                                        else
                                            l_Seil = (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] + 1.0) * l_AbilityEffectEntry->ModMin;

                                        l_Unk1 = l_MechanicEntry->Unk2;

                                        if (l_Seil <= l_Unk1)
                                            l_Unk1 = l_Seil;

                                        l_Unk2 = l_Unk2 - l_Unk1;
                                    }
                                }
                            }
                        }
                    }
                }

                if (l_Unk2 < 0.0)
                    l_Unk2 = 0.0;

                l_Unk1 = l_MechanicEntry->Unk2;
                l_Unk1 = (l_Unk1 - l_Unk2) * l_V62;
                l_CurrentAdditionalWinChance = l_Unk1 + l_CurrentAdditionalWinChance;

                #ifdef GARRISON_CHEST_FORMULA_DEBUG
                    printf("Added %.2f to success due to followers countering boss mechanic %u.\n", l_Unk1, l_EncoutersMechanics[l_I].second);
                #endif // GARRISON_CHEST_FORMULA_DEBUG
            }
        }
        #pragma endregion

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        /// UNTESTED
        #pragma region Race Ability Counter
        for (uint32 l_I = 0; l_I < l_EncoutersMechanics.size(); ++l_I)
        {
            GarrMechanicEntry     const* l_MechanicEntry     = sGarrMechanicStore.LookupEntry(l_EncoutersMechanics[l_I].second);
            GarrMechanicTypeEntry const* l_MechanicTypeEntry = sGarrMechanicTypeStore.LookupEntry(l_MechanicEntry->MechanicTypeID);

            if (l_MechanicTypeEntry->Type == MechanicTypes::Racial)
            {
                for (uint32 l_Y = 0; l_Y < l_MissionFollowers.size(); ++l_Y)
                {
                    for (uint32 l_Z = 0; l_Z < l_MissionFollowers[l_Y]->Abilities.size(); l_Z++)
                    {
                        uint32 l_CurrentAbilityID = l_MissionFollowers[l_Y]->Abilities[l_Z];

                        for (uint32 l_EffectI = 0; l_EffectI < sGarrAbilityEffectStore.GetNumRows(); l_EffectI++)
                        {
                            GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectI);

                            if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
                                continue;

                            if (l_AbilityEffectEntry->CounterMechanicTypeID == l_MissionTemplate->GarrMechanicTypeRecID)
                            {
                                double l_Seil = 0;

                                if (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] >= 0.0)
                                    l_Seil = (l_AbilityEffectEntry->ModMax - l_AbilityEffectEntry->ModMin) * l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] + l_AbilityEffectEntry->ModMin;
                                else
                                    l_Seil = (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] + 1.0) * l_AbilityEffectEntry->ModMin;

                                l_CurrentAdditionalWinChance = (l_Seil * l_V62) + l_CurrentAdditionalWinChance;

                                #ifdef GARRISON_CHEST_FORMULA_DEBUG
                                    printf("Added %.2f to success due to follower %u enemy race ability %d.\n", (l_Seil * l_V62), 0, l_CurrentAbilityID);
                                #endif // GARRISON_CHEST_FORMULA_DEBUG
                            }
                        }
                    }
                }
            }
        }
        #pragma endregion

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        /// OK 100%
        #pragma region Environment Ability
        for (uint32 l_Y = 0; l_Y < l_MissionFollowers.size(); ++l_Y)
        {
            for (uint32 l_Z = 0; l_Z < l_MissionFollowers[l_Y]->Abilities.size(); l_Z++)
            {
                uint32 l_CurrentAbilityID = l_MissionFollowers[l_Y]->Abilities[l_Z];

                for (uint32 l_EffectI = 0; l_EffectI < sGarrAbilityEffectStore.GetNumRows(); l_EffectI++)
                {
                    GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectI);

                    if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
                        continue;

                    if (l_AbilityEffectEntry->CounterMechanicTypeID == l_MissionTemplate->GarrMechanicTypeRecID)
                    {
                        double l_Seil = 0;

                        if (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] >= 0.0)
                            l_Seil = (l_AbilityEffectEntry->ModMax - l_AbilityEffectEntry->ModMin) * l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] + l_AbilityEffectEntry->ModMin;
                        else
                            l_Seil = (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] + 1.0) * l_AbilityEffectEntry->ModMin;

                        l_CurrentAdditionalWinChance = (l_Seil * l_V62) + l_CurrentAdditionalWinChance;

                        #ifdef GARRISON_CHEST_FORMULA_DEBUG
                            printf("Added %.2f to success due to follower %u environment ability %u.\n", (l_Seil * l_V62), l_MissionFollowers[l_Y]->FollowerID, l_CurrentAbilityID);
                        #endif // GARRISON_CHEST_FORMULA_DEBUG
                    }
                }
            }
        }
        #pragma endregion

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        /// OK 100%
        #pragma region Follower Trait
        double l_MissionDuration = GetMissionDuration(p_MissionRecID);
        double l_MissionTravelTime = GetMissionTravelDuration(p_MissionRecID);

        for (uint32 l_Y = 0; l_Y < l_MissionFollowers.size(); ++l_Y)
        {
            for (uint32 l_Z = 0; l_Z < l_MissionFollowers[l_Y]->Abilities.size(); l_Z++)
            {
                uint32 l_CurrentAbilityID = l_MissionFollowers[l_Y]->Abilities[l_Z];

                for (uint32 l_EffectI = 0; l_EffectI < sGarrAbilityEffectStore.GetNumRows(); l_EffectI++)
                {
                    GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectI);

                    if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
                        continue;

                    bool l_Proc = false;

                    switch (l_AbilityEffectEntry->EffectType)
                    {
                        /// Proc if MissionFollowerCount == 1
                        case AbilityEffectTypes::ModWinRateSolo:
                            l_Proc = l_MissionFollowers.size() == 1;
                            break;

                        /// Proc every time, no condition
                        case AbilityEffectTypes::ModWinRate:
                            l_Proc = true;
                            break;

                        /// Proc if Find(MissionFollowers[Class], MiscValueA) != NULL
                        case AbilityEffectTypes::ModWinRateClass:
                            for (uint32 l_W = 0; l_W < l_MissionFollowers.size(); ++l_W)
                            {
                                if (l_W != l_Y)
                                {
                                    GarrFollowerEntry const* l_FollowerTemplate = sGarrFollowerStore.LookupEntry(l_MissionFollowers[l_W]->FollowerID);

                                    if (l_FollowerTemplate && l_FollowerTemplate->CreatureID[GetGarrisonFactionIndex()] == l_AbilityEffectEntry->MiscValueA)
                                    {
                                        l_Proc = true;
                                        break;
                                    }
                                }
                            }
                            break;

                        /// Proc if Duration > (3600 * MiscValueB)
                        case AbilityEffectTypes::ModWinRateDurationMore:
                            l_Proc = l_MissionDuration > (3600 * l_AbilityEffectEntry->MiscValueB);
                            break;

                        /// Proc if Duration < (3600 * MiscValueB)
                        case AbilityEffectTypes::ModWinRateDurationLess:
                            l_Proc = l_MissionDuration < (3600 * l_AbilityEffectEntry->MiscValueB);
                            break;

                        /// Proc if TravelDuration > (3600 * MiscValueB)
                        case AbilityEffectTypes::ModWinRateTravelDurationMore:
                            l_Proc = l_MissionTravelTime > (3600 * l_AbilityEffectEntry->MiscValueB);
                            break;

                        /// Proc if TravelDuration < (3600 * MiscValueB)
                        case AbilityEffectTypes::ModWinRateTravelDurationLess:
                            l_Proc = l_MissionTravelTime < (3600 * l_AbilityEffectEntry->MiscValueB);
                            break;

                        case AbilityEffectTypes::ModUnk0:
                        case AbilityEffectTypes::ModTravelTime:
                        case AbilityEffectTypes::ModXpGain:
                        case AbilityEffectTypes::ModGarrCurrencyDrop:
                        case AbilityEffectTypes::ModUnk11:
                        case AbilityEffectTypes::ModDummyProduction:
                        case AbilityEffectTypes::ModBronzeTreasureDrop:
                        case AbilityEffectTypes::ModSilverTreasureDrop:
                        case AbilityEffectTypes::ModGoldTreasureDrop:
                        case AbilityEffectTypes::ModChestDropRate:
                        case AbilityEffectTypes::ModMissionDuration:
                            break;

                        default:
                            break;

                    }

                    if (!l_Proc)
                        break;

                    double l_Seil = 0.0;

                    if (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] >= 0.0)
                        l_Seil = (l_AbilityEffectEntry->ModMax - l_AbilityEffectEntry->ModMin) * l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] + l_AbilityEffectEntry->ModMin;
                    else
                        l_Seil = (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] + 1.0) * l_AbilityEffectEntry->ModMin;

                    l_CurrentAdditionalWinChance = (l_Seil * l_V62) + l_CurrentAdditionalWinChance;

                    #ifdef GARRISON_CHEST_FORMULA_DEBUG
                        printf("Added %.2f to success due to follower %u trait %u.\n", (l_Seil * l_V62), l_MissionFollowers[l_Y]->FollowerID, l_AbilityEffectEntry->EffectType);
                    #endif // GARRISON_CHEST_FORMULA_DEBUG
                }
            }
        }
        #pragma endregion

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        /// UNTESTED
        #pragma region Passive Effect
        for (uint32 l_Y = 0; l_Y < l_PassiveEffects.size(); ++l_Y)
        {
            GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_PassiveEffects[l_Y]);

            if (!l_AbilityEffectEntry)
                continue;

            if (l_AbilityEffectEntry->EffectType == AbilityEffectTypes::ModWinRate)
            {
                l_CurrentAdditionalWinChance = (l_AbilityEffectEntry->ModMin * l_V62) + l_CurrentAdditionalWinChance;

                #ifdef GARRISON_CHEST_FORMULA_DEBUG
                    printf("Added %.2f to success due to passive effect %u.\n", l_AbilityEffectEntry->ModMin * l_V62, l_AbilityEffectEntry->AbilityID);
                #endif // GARRISON_CHEST_FORMULA_DEBUG
            }
        }
        #pragma endregion

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        l_CurrentAdditionalWinChance = (((100.0 - l_MissionTemplate->BaseBonusChance) * l_CurrentAdditionalWinChance) * 0.0099999998) + l_MissionTemplate->BaseBonusChance;

        if (l_CurrentAdditionalWinChance > 100.0)
            l_CurrentAdditionalWinChance = 100.0;

        #ifdef GARRISON_CHEST_FORMULA_DEBUG
            printf("Total success chance: %.2f.\n", l_CurrentAdditionalWinChance);
        #endif // GARRISON_CHEST_FORMULA_DEBUG

        return l_CurrentAdditionalWinChance;
    }

    /// Get missions
    std::vector<GarrisonMission> Manager::GetMissions() const
    {
        std::vector<GarrisonMission> l_Result;

        for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        {
            if (m_Missions[l_I].State < MissionStates::CompleteSuccess
                && !((m_Missions[l_I].OfferTime + m_Missions[l_I].OfferMaxDuration) < time(0) && m_Missions[l_I].State == MissionStates::Available))
                l_Result.push_back(m_Missions[l_I]);
        }

        return l_Result;
    }

    /// Get all completed missions
    std::vector<GarrisonMission> Manager::GetCompletedMissions() const
    {
        std::vector<GarrisonMission> l_Result;

        for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        {
            if (m_Missions[l_I].State >= MissionStates::CompleteSuccess)
                l_Result.push_back(m_Missions[l_I]);
        }

        return l_Result;
    }

    GarrisonMission* Manager::GetMissionWithID(uint32 p_MissionID)
    {
        for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        {
            if (m_Missions[l_I].MissionID == p_MissionID)
                return &m_Missions[l_I];
        }

        return nullptr;
    }

    /// Get all completed missions
    std::vector<GarrisonMission> Manager::GetPendingMissions() const
    {
        std::vector<GarrisonMission> l_Result;

        for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        {
            if (m_Missions[l_I].State >= MissionStates::InProgress)
                l_Result.push_back(m_Missions[l_I]);
        }

        return l_Result;
    }

    bool Manager::HasPendingMission(uint32 p_MissionID)
    {
        for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        {
            if (m_Missions[l_I].State == MissionStates::InProgress && m_Missions[l_I].MissionID == p_MissionID)
                return true;
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Add follower
    bool Manager::AddFollower(uint32 p_FollowerID)
    {
        auto l_TempFollower = GetFollower(p_FollowerID);
        if (l_TempFollower && l_TempFollower->FollowerID != 0)
            return false;

        GarrFollowerEntry const* l_Entry = sGarrFollowerStore.LookupEntry(p_FollowerID);

        if (!l_Entry)
            return false;

        GarrisonFollower l_Follower;
        l_Follower.DatabaseID           = sObjectMgr->GetNewGarrisonFollowerID();
        l_Follower.FollowerID           = p_FollowerID;
        l_Follower.Level                = l_Entry->Level;
        l_Follower.XP                   = 0;
        l_Follower.Quality              = l_Entry->Quality;
        l_Follower.ItemLevelArmor       = l_Entry->ItemLevelArmor;
        l_Follower.ItemLevelWeapon      = l_Entry->ItemLevelWeapon;
        l_Follower.CurrentBuildingID    = 0;
        l_Follower.CurrentMissionID     = 0;
        l_Follower.Flags                = 0;

        GenerateFollowerAbilities(l_Follower);

        std::ostringstream l_Abilities;

        for (uint32 l_Y = 0; l_Y < l_Follower.Abilities.size(); ++l_Y)
            l_Abilities << l_Follower.Abilities[l_Y] << ' ';

        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GARRISON_FOLLOWER);

        uint32 l_Index = 0;
        l_Stmt->setUInt32(l_Index++, l_Follower.DatabaseID);
        l_Stmt->setUInt32(l_Index++, m_ID);
        l_Stmt->setUInt32(l_Index++, l_Follower.FollowerID);
        l_Stmt->setUInt32(l_Index++, l_Follower.Level);
        l_Stmt->setUInt32(l_Index++, l_Follower.XP);
        l_Stmt->setUInt32(l_Index++, l_Follower.Quality);
        l_Stmt->setUInt32(l_Index++, l_Follower.ItemLevelArmor);
        l_Stmt->setUInt32(l_Index++, l_Follower.ItemLevelWeapon);
        l_Stmt->setUInt32(l_Index++, l_Follower.CurrentMissionID);
        l_Stmt->setUInt32(l_Index++, l_Follower.CurrentBuildingID);
        l_Stmt->setString(l_Index++, l_Abilities.str());
        l_Stmt->setUInt32(l_Index++, l_Follower.Flags);
        l_Stmt->setString(l_Index++, l_Follower.ShipName);

        CharacterDatabase.AsyncQuery(l_Stmt);

        m_Followers.push_back(l_Follower);

        WorldPacket l_AddFollowerResult(SMSG_GARRISON_ADD_FOLLOWER_RESULT, 64);
        l_AddFollowerResult << uint32(PurchaseBuildingResults::Ok);
        l_Follower.Write(l_AddFollowerResult);

        m_Owner->SendDirectMessage(&l_AddFollowerResult);

        uint32 l_AchievementEntry = 0;

        switch (l_Follower.Quality)
        {
            case ItemQualities::ITEM_QUALITY_RARE:
                l_AchievementEntry = 9130;  ///< A Rare Friend
                break;
            case ItemQualities::ITEM_QUALITY_EPIC:
                l_AchievementEntry = 9131;  ///< An Epic Buddy
                break;

            default:
                break;
        }

        if (l_AchievementEntry && !m_Owner->GetAchievementMgr().HasAchieved(l_AchievementEntry))
            m_Owner->GetAchievementMgr().CompletedAchievement(sAchievementStore.LookupEntry(l_AchievementEntry), nullptr);

        m_Owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_RECRUIT_FOLLOWER_IN_OWN_GARRISON);

        return true;
    }

    bool Manager::AddFollower(GarrisonFollower p_Follower)
    {
        GarrFollowerEntry const* l_Entry = sGarrFollowerStore.LookupEntry(p_Follower.FollowerID);

        if (!l_Entry)
            return false;

        std::ostringstream l_Abilities;

        for (uint32 l_Itr = 0; l_Itr < p_Follower.Abilities.size(); ++l_Itr)
            l_Abilities << p_Follower.Abilities[l_Itr] << ' ';

        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GARRISON_FOLLOWER);

        uint32 l_Index = 0;
        l_Stmt->setUInt32(l_Index++, p_Follower.DatabaseID);
        l_Stmt->setUInt32(l_Index++, m_ID);
        l_Stmt->setUInt32(l_Index++, p_Follower.FollowerID);
        l_Stmt->setUInt32(l_Index++, p_Follower.Level);
        l_Stmt->setUInt32(l_Index++, p_Follower.XP);
        l_Stmt->setUInt32(l_Index++, p_Follower.Quality);
        l_Stmt->setUInt32(l_Index++, p_Follower.ItemLevelArmor);
        l_Stmt->setUInt32(l_Index++, p_Follower.ItemLevelWeapon);
        l_Stmt->setUInt32(l_Index++, p_Follower.CurrentMissionID);
        l_Stmt->setUInt32(l_Index++, p_Follower.CurrentBuildingID);
        l_Stmt->setString(l_Index++, l_Abilities.str());
        l_Stmt->setUInt32(l_Index++, p_Follower.Flags);
        l_Stmt->setString(l_Index++, p_Follower.ShipName);

        CharacterDatabase.AsyncQuery(l_Stmt);

        m_Followers.push_back(p_Follower);

        WorldPacket l_AddFollowerResult(SMSG_GARRISON_ADD_FOLLOWER_RESULT, 64);
        l_AddFollowerResult << uint32(PurchaseBuildingResults::Ok);
        p_Follower.Write(l_AddFollowerResult);

        m_Owner->SendDirectMessage(&l_AddFollowerResult);

        if (p_Follower.Quality == ItemQualities::ITEM_QUALITY_RARE)
        {
            if (!m_Owner->GetAchievementMgr().HasAchieved(9130))
                m_Owner->GetAchievementMgr().CompletedAchievement(sAchievementStore.LookupEntry(9130), nullptr);

            m_Owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_RECRUIT_FOLLOWER_IN_OWN_GARRISON);
        }

        return true;
    }

    /// Assign a follower to a building
    void Manager::AssignFollowerToBuilding(uint64 p_FollowerDBID, uint32 p_PlotInstanceID)
    {
        GarrisonFollower* l_Follower = GetFollowerWithDatabaseID(p_FollowerDBID);

        if (l_Follower == nullptr)
            return;

        MS::Garrison::GarrisonBuilding* l_Building = GetBuildingObject(p_PlotInstanceID);

        if (l_Building == nullptr)
        {
            bool l_NeedSave = false;
            if (p_PlotInstanceID == 0)
            {
                for (GarrisonBuilding& l_Building : m_Buildings)
                {
                    if (l_Building.FollowerAssigned == p_FollowerDBID)
                    {
                        l_Building.FollowerAssigned = 0;
                        break;
                    }
                }

                l_NeedSave = true;
            }
            if (l_Follower->CurrentBuildingID != 0)
            {
                l_Follower->CurrentBuildingID = 0;
                l_NeedSave = true;
            }

            if (l_NeedSave)
                Save();

            UpdatePlot(p_PlotInstanceID);

            return;
        }

        l_Building->FollowerAssigned  = p_FollowerDBID;
        l_Follower->CurrentBuildingID = l_Building->BuildingID;

        Save();
        UpdatePlot(p_PlotInstanceID);
    }

    bool Manager::CanRecruitFollower() const
    {
        return m_CanRecruitFollower;
    }

    void Manager::SetCanRecruitFollower(bool p_Apply)
    {
        m_CanRecruitFollower = p_Apply;
    }

    /// Change follower activation state
    void Manager::ChangeFollowerActivationState(uint64 p_FollowerDBID, bool p_Active)
    {
        GarrisonFollower* l_Follower = nullptr;

        if (p_Active)
        {
            if (!m_Owner->HasEnoughMoney((uint64)Globals::FollowerActivationCost))
                return;

            if (GetNumFollowerActivationsRemaining() < 1)
                return;

            auto l_It = std::find_if(m_Followers.begin(), m_Followers.end(), [p_FollowerDBID](const GarrisonFollower& p_Follower) { return p_Follower.DatabaseID == p_FollowerDBID; });

            if (l_It != m_Followers.end())
            {
                if (!l_It->IsNPC())
                    return;

                m_Owner->ModifyMoney(-Globals::FollowerActivationCost);

                m_NumFollowerActivation--;
                m_NumFollowerActivationRegenTimestamp = time(0);

                l_It->Flags = l_It->Flags & ~GARRISON_FOLLOWER_FLAG_INACTIVE;
                l_Follower = &(*l_It);

                WorldPacket l_Data(SMSG_GARRISON_UPDATE_FOLLOWER_ACTIVATION_COUNT, 4);
                l_Data << uint32(GetNumFollowerActivationsRemaining());

                m_Owner->SendDirectMessage(&l_Data);
            }
        }
        else
        {
            if (!m_Owner->HasEnoughMoney((uint64)Globals::FollowerActivationCost))
                return;

            auto l_It = std::find_if(m_Followers.begin(), m_Followers.end(), [p_FollowerDBID](const GarrisonFollower& p_Follower) { return p_Follower.DatabaseID == p_FollowerDBID; });

            if (l_It != m_Followers.end())
            {
                m_Owner->ModifyMoney(-Globals::FollowerActivationCost);

                l_It->Flags |= GARRISON_FOLLOWER_FLAG_INACTIVE;
                l_Follower = &(*l_It); ///< l_follower is never read 01/18/16
            }

            l_Follower = &*l_It;
        }

        if (!l_Follower)
            return;

        l_Follower->SendFollowerUpdate(m_Owner);
    }

    /// Get followers
    std::vector<GarrisonFollower> Manager::GetFollowers() const
    {
        return m_Followers;
    }

    /// Get followers dbc list with abilities
    std::list<GarrisonFollower> Manager::GetFollowersWithAbility(uint32 p_AbilityID, bool p_IsTrait)
    {
        std::list<GarrisonFollower> l_FinalFollowers;
        std::vector<uint32> l_FollowerIDs;
        std::vector<uint32> l_PotentialFollowers;

        for (uint32 l_I = 0; l_I < sGarrFollowerXAbilityStore.GetNumRows(); ++l_I)
        {
            GarrFollowerEntry const* l_FollowerEntry = sGarrFollowerStore.LookupEntry(l_I);

            if (l_FollowerEntry != nullptr)
                l_FollowerIDs.push_back(l_FollowerEntry->ID);
        }

        l_FollowerIDs.erase(std::remove_if(l_FollowerIDs.begin(), l_FollowerIDs.end(), [this, p_AbilityID, &l_PotentialFollowers](uint32 p_FollowerID) -> bool
        {
            std::vector<GarrisonFollower> l_PossessedFollowers = GetFollowers();
            
            auto l_Found = std::find_if(l_PossessedFollowers.begin(), l_PossessedFollowers.end(), [this, p_FollowerID](GarrisonFollower p_Follower) -> bool
            {
                if (p_Follower.FollowerID == p_FollowerID)
                    return true;

                return false;
            });

            if (l_Found != l_PossessedFollowers.end())
                return true;

            GarrFollowerEntry const* l_Entry = sGarrFollowerStore.LookupEntry(p_FollowerID);

            if (l_Entry == nullptr || l_Entry->Quality >= ItemQualities::ITEM_QUALITY_EPIC || l_Entry->Level > 90 || l_Entry->HordeSourceText->Str[LOCALE_enUS] == "")
                return true;

            l_PotentialFollowers.push_back(p_FollowerID);

            for (uint32 l_I = 0; l_I < sGarrFollowerXAbilityStore.GetNumRows(); ++l_I)
            {
                GarrFollowerXAbilityEntry const* l_AbilityEntry = sGarrFollowerXAbilityStore.LookupEntry(l_I);

                /// TEAMID server side is inverted with DBCs 0/1 values, so real condition for l_AbilityEntry->FactionIndex != m_Owner->GetTeamId() is ==
                if (l_AbilityEntry != nullptr && l_AbilityEntry->FollowerID == p_FollowerID && l_AbilityEntry->AbilityID == p_AbilityID && l_AbilityEntry->FactionIndex != m_Owner->GetTeamId())
                    return false;
            }

            return true;
        }), l_FollowerIDs.end());

        for (uint32 l_FollowerID : l_FollowerIDs)
        {
            l_FinalFollowers.push_back(GenerateNewFollowerEntity(l_FollowerID));
            l_PotentialFollowers.erase(std::remove(l_PotentialFollowers.begin(), l_PotentialFollowers.end(), l_FollowerID), l_PotentialFollowers.end());
        }

        if (l_PotentialFollowers.empty())
            return l_FinalFollowers;

        while (l_FinalFollowers.size() < 3)
        {
            GarrisonFollower l_Follower = GenerateNewFollowerEntity(l_PotentialFollowers[urand(0, l_PotentialFollowers.size() - 1)]);
            uint32 l_AbilityGenerated = 0;

            for (auto l_Itr = l_Follower.Abilities.begin(); l_Itr != l_Follower.Abilities.end(); ++l_Itr)
            {
                using namespace MS::Garrison::GarrisonAbilities;

                if (!p_IsTrait)
                {
                    std::vector<uint32> l_PotentialAbilities;
                    for (uint32 l_EffectID = 0; l_EffectID < sGarrAbilityEffectStore.GetNumRows(); ++l_EffectID)
                    {
                        GarrAbilityEffectEntry const* l_Effect = sGarrAbilityEffectStore.LookupEntry(l_EffectID);

                        if (!l_Effect)
                            continue;

                        if (l_Effect->CounterMechanicTypeID == p_AbilityID)
                            l_PotentialAbilities.push_back(l_Effect->AbilityID);
                    }

                    if (!l_PotentialAbilities.empty())
                        l_AbilityGenerated = l_PotentialAbilities[urand(0, l_PotentialAbilities.size() - 1)];

                    l_Itr = l_Follower.Abilities.erase(l_Itr);
                    break;
                }

                if (p_IsTrait && std::find(g_FollowerTraits.begin(), g_FollowerTraits.end(), *l_Itr) != g_FollowerTraits.end())
                {
                    l_Itr = l_Follower.Abilities.erase(l_Itr);
                    break;
                }
            }

            if (l_AbilityGenerated)
                l_Follower.Abilities.push_back(l_AbilityGenerated);
            else
                l_Follower.Abilities.push_back(p_AbilityID);

            l_FinalFollowers.push_back(l_Follower);
        }

        return l_FinalFollowers;
    }

    /// Get follower
    GarrisonFollower* Manager::GetFollower(uint32 p_FollowerID)
    {
        for (uint32 l_I = 0; l_I < m_Followers.size(); l_I++)
        {
            if (m_Followers[l_I].FollowerID == p_FollowerID)
                return &m_Followers[l_I];
        }

        return nullptr;
    }

    /// Get follower
    GarrisonFollower* Manager::GetFollowerWithDatabaseID(uint32 p_FollowerDatabaseID)
    {
        for (uint32 l_I = 0; l_I < m_Followers.size(); l_I++)
        {
            if (m_Followers[l_I].DatabaseID == p_FollowerDatabaseID)
                return &m_Followers[l_I];
        }

        return nullptr;
    }

    /// Get activated followers count
    uint32 Manager::GetActiveFollowerCount(uint32 p_FollowerType) const
    {
        return (uint32)std::count_if(m_Followers.begin(), m_Followers.end(), [p_FollowerType](GarrisonFollower p_Folloser) -> bool
        {
            auto l_Entry = p_Folloser.GetEntry();
            return l_Entry ? (l_Entry->Type == p_FollowerType && (p_FollowerType == FollowerType::Ship || !(p_Folloser.Flags & GARRISON_FOLLOWER_FLAG_INACTIVE))) : -1;
        });
    }

    /// Get num follower activation remaining
    uint32 Manager::GetNumFollowerActivationsRemaining() const
    {
        return m_NumFollowerActivation;
    }

    SpellCastResult Manager::CanUpgradeItemLevelWith(uint32 p_FollowerID, SpellInfo const* p_SpellInfo) const
    {
        auto l_It = std::find_if(m_Followers.begin(), m_Followers.end(), [p_FollowerID](GarrisonFollower const& p_Follower) { return p_Follower.FollowerID == p_FollowerID; });

        if (l_It == m_Followers.end())
            return SPELL_FAILED_BAD_TARGETS;

        GarrisonFollower const* l_Follower = &(*l_It);
        SpellEffectInfo const* l_SpellEffect = p_SpellInfo->GetEffectByType(SPELL_EFFECT_INCREASE_FOLLOWER_ITEM_LEVEL);

        if (!l_SpellEffect)
            return SPELL_FAILED_ERROR;

        SpellEffectInfo const* l_Dummy = p_SpellInfo->GetEffectByType(SPELL_EFFECT_DUMMY);
        int32 l_Cap = l_Dummy ? l_Dummy->BasePoints : GetMaxFollowerItemLevel(l_Follower->GetEntry()->Type);
        int32 l_Ilvl = l_SpellEffect->MiscValue == 1 || l_SpellEffect->MiscValue == 3 ? l_Follower->ItemLevelArmor : l_Follower->ItemLevelWeapon;

        if (l_Ilvl >= l_Cap)
            return SPELL_FAILED_GARRISON_FOLLOWER_MAX_ITEM_LEVEL;

        return SPELL_CAST_OK;
    }

    void Manager::UpgradeFollowerItemLevelWith(uint32 p_FollowerID, SpellInfo const* p_SpellInfo)
    {
        if (CanUpgradeItemLevelWith(p_FollowerID, p_SpellInfo) != SPELL_CAST_OK)
            return;

        auto l_It = std::find_if(m_Followers.begin(), m_Followers.end(), [p_FollowerID](GarrisonFollower const& p_Follower) { return p_Follower.FollowerID == p_FollowerID; });
        GarrisonFollower* l_Follower = const_cast<GarrisonFollower*>(&(*l_It));

        SpellEffectInfo const* l_SpellEffect = p_SpellInfo->GetEffectByType(SPELL_EFFECT_INCREASE_FOLLOWER_ITEM_LEVEL);
        SpellEffectInfo const* l_Dummy = p_SpellInfo->GetEffectByType(SPELL_EFFECT_DUMMY);
        int32 l_Cap = l_Dummy ? l_Dummy->BasePoints : GetMaxFollowerItemLevel(l_Follower->GetEntry()->Type);
        int32 &l_Ilvl = l_SpellEffect->MiscValue == 1 || l_SpellEffect->MiscValue == 3 ? l_Follower->ItemLevelArmor : l_Follower->ItemLevelWeapon;

        l_Ilvl = std::min(l_Cap, l_Ilvl + l_SpellEffect->BasePoints);

        WorldPacket l_Data(SMSG_GARRISON_FOLLOWER_CHANGED_ITEM_LEVEL, 4 + 4 + 4 + 1);
        l_Follower->Write(l_Data);
        m_Owner->SendDirectMessage(&l_Data);
        
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Can build building X at slot instance Y
    bool Manager::IsBuildingPlotInstanceValid(uint32 p_BuildingRecID, uint32 p_PlotInstanceID) const
    {
        GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingRecID);

        if (!l_BuildingEntry)
            return false;

        GarrPlotInstanceEntry const* l_PlotInstanceEntry = sGarrPlotInstanceStore.LookupEntry(p_PlotInstanceID);

        if (!l_PlotInstanceEntry)
            return false;

        /// Search building plot ID
        for (uint32 l_I = 0; l_I < sGarrPlotBuildingStore.GetNumRows(); ++l_I)
        {
            GarrPlotBuildingEntry const* l_PlotBuildingEntry = sGarrPlotBuildingStore.LookupEntry(l_I);

            if (l_PlotBuildingEntry && l_PlotBuildingEntry->PlotID == l_PlotInstanceEntry->PlotID && l_PlotBuildingEntry->BuildingID == p_BuildingRecID)
                return true;
        }

        return false;
    }

    /// Player fill all condition
    PurchaseBuildingResults::Type Manager::CanPurchaseBuilding(uint32 p_BuildingRecID) const
    {
        GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingRecID);

        if (!l_BuildingEntry)
            return PurchaseBuildingResults::InvalidBuildingID;

        if (l_BuildingEntry->CostCurrencyID != 0)
        {
            if (!m_Owner->HasCurrency(l_BuildingEntry->CostCurrencyID, l_BuildingEntry->CostCurrencyAmount))
                return PurchaseBuildingResults::NotEnoughCurrency;

            if (!m_Owner->HasEnoughMoney((uint64)l_BuildingEntry->CostMoney * GOLD))
                return PurchaseBuildingResults::NotEnoughGold;
        }

        return PurchaseBuildingResults::Ok;
    }

    /// PurchaseBuilding
    GarrisonBuilding Manager::PurchaseBuilding(uint32 p_BuildingRecID, uint32 p_PlotInstanceID, bool p_Triggered)
    {
        GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingRecID);

        GarrisonBuilding l_Building;
        Interfaces::GarrisonSite* l_GarrisonScript = GetGarrisonScript();

        if (!l_BuildingEntry)
            return l_Building;

        if (!PlotIsFree(p_PlotInstanceID))
        {
            GarrPlotInstanceEntry const* l_PlotInstanceEntry = sGarrPlotInstanceStore.LookupEntry(p_PlotInstanceID);
            GarrisonBuilding l_Building = GetBuilding(p_PlotInstanceID);
            GarrBuildingEntry const* l_OldBuildingEntry = sGarrBuildingStore.LookupEntry(l_Building.BuildingID);
            bool l_ForUpgrade = false;

            if (l_OldBuildingEntry && l_PlotInstanceEntry && l_OldBuildingEntry->Type == l_BuildingEntry->Type && l_OldBuildingEntry->Level < l_BuildingEntry->Level && l_GarrisonScript)
            {
                m_LastPlotBuildingType.insert(std::make_pair(l_PlotInstanceEntry->PlotID, l_OldBuildingEntry->Type));
                l_ForUpgrade = true;
            }

            DeleteBuilding(p_PlotInstanceID, false, l_ForUpgrade);
        }

        if (l_BuildingEntry->CostCurrencyID != 0 && !p_Triggered)
            m_Owner->ModifyCurrency(l_BuildingEntry->CostCurrencyID, -(int32)l_BuildingEntry->CostCurrencyAmount);

        if (l_BuildingEntry->CostMoney != 0 && !p_Triggered)
            m_Owner->ModifyMoney(-(int64)(l_BuildingEntry->CostMoney * GOLD));

        if (!p_Triggered)
        {
            WorldPacket l_PlotRemoved(SMSG_GARRISON_PLOT_REMOVED, 4);
            l_PlotRemoved << uint32(p_PlotInstanceID);
            m_Owner->SendDirectMessage(&l_PlotRemoved);
        }

        uint32 l_BuildingTime = l_BuildingEntry->BuildDuration;

        if (l_GarrisonScript)
            l_BuildingTime = l_GarrisonScript->OnPrePurchaseBuilding(m_Owner, p_BuildingRecID, l_BuildingTime);

        l_Building.DatabaseID       = sObjectMgr->GetNewGarrisonBuildingID();
        l_Building.BuildingID       = p_BuildingRecID;
        l_Building.PlotInstanceID   = p_PlotInstanceID;
        l_Building.TimeBuiltStart   = time(nullptr);
        l_Building.TimeBuiltEnd     = time(nullptr) + l_BuildingTime;           ///< 5/5/1905 18:45:05
        l_Building.SpecID           = 0;
        l_Building.Active           = false;
        l_Building.BuiltNotified    = false;

        if (l_BuildingEntry->BuildingCategory == BuildingCategory::Prebuilt)
        {
            l_Building.TimeBuiltStart   = time(nullptr) - l_BuildingTime;
            l_Building.TimeBuiltEnd     = time(nullptr);
            l_Building.Active           = false;
            l_Building.BuiltNotified    = true;
        }
        else if (p_Triggered)
            l_Building.TimeBuiltEnd = l_Building.TimeBuiltStart;

        if (l_BuildingEntry->Type == Globals::ShipyardBuildingType)
        {
            l_Building.Active = true;
            l_Building.BuiltNotified = true;
        }

        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GARRISON_BUILDING);

        uint32 l_Index = 0;
        l_Stmt->setUInt32(l_Index++, l_Building.DatabaseID);
        l_Stmt->setUInt32(l_Index++, m_ID);
        l_Stmt->setUInt32(l_Index++, l_Building.PlotInstanceID);
        l_Stmt->setUInt32(l_Index++, l_Building.BuildingID);
        l_Stmt->setUInt32(l_Index++, l_Building.SpecID);
        l_Stmt->setUInt32(l_Index++, l_Building.TimeBuiltStart);
        l_Stmt->setUInt32(l_Index++, l_Building.TimeBuiltEnd);
        l_Stmt->setUInt32(l_Index++, l_Building.FollowerAssigned);
        l_Stmt->setBool(l_Index++, l_Building.Active);
        l_Stmt->setString(l_Index++, l_Building.GatheringData);

        CharacterDatabase.AsyncQuery(l_Stmt);

        m_Buildings.push_back(l_Building);

        UpdatePlot(p_PlotInstanceID);

        if (l_GarrisonScript)
            l_GarrisonScript->OnPurchaseBuilding(m_Owner, p_BuildingRecID);

        return l_Building;
    }

    /// Get building
    GarrisonBuilding Manager::GetBuilding(uint32 p_PlotInstanceID) const
    {
        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
            if (m_Buildings[l_I].PlotInstanceID == p_PlotInstanceID)
                return m_Buildings[l_I];

        return GarrisonBuilding();
    }

    /// Get building
    GarrisonBuilding Manager::GetBuildingWithBuildingID(uint32 p_BuildingID) const
    {
        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        if (m_Buildings[l_I].BuildingID == p_BuildingID)
            return m_Buildings[l_I];

        return GarrisonBuilding();
    }

    /// Get buildings
    std::vector<GarrisonBuilding> Manager::GetBuildings() const
    {
        std::vector<GarrisonBuilding> l_Buildings;

        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        {
            /// Don't include the building if the owner doesn't have access to it
            if (!sGarrisonBuildingManager->MatchsConditionsForBuilding(m_Buildings[l_I].BuildingID, m_Owner))
                continue;

            l_Buildings.push_back(m_Buildings[l_I]);
        }

        return l_Buildings;
    }

    /// Get Building object
    GarrisonBuilding* Manager::GetBuildingObject(uint32 p_PlotInstanceID)
    {
        for (uint32 l_I = 0; l_I < m_Buildings.size(); l_I++)
        {
            if (m_Buildings[l_I].PlotInstanceID == p_PlotInstanceID)
                return &m_Buildings[l_I];
        }

        return nullptr;
    }

    /// Get building passive ability effects
    std::vector<uint32> Manager::GetBuildingsPassiveAbilityEffects() const
    {
        std::vector<uint32> l_PassiveEffects;

        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        {
            /// Don't include prebuilt building passives if the owner doesn't have access to it
            if (!sGarrisonBuildingManager->MatchsConditionsForBuilding(m_Buildings[l_I].BuildingID, m_Owner))
                continue;

            GarrBuildingEntry const* l_BuildingTemplate = sGarrBuildingStore.LookupEntry(m_Buildings[l_I].BuildingID);

            if (l_BuildingTemplate && l_BuildingTemplate->FollowerGarrAbilityEffectID && sGarrAbilityEffectStore.LookupEntry(l_BuildingTemplate->FollowerGarrAbilityEffectID) != nullptr)
                l_PassiveEffects.push_back(l_BuildingTemplate->FollowerGarrAbilityEffectID);
        }

        return l_PassiveEffects;
    }

    /// Activate building
    void Manager::ActivateBuilding(uint32 p_PlotInstanceID)
    {
        GarrisonBuilding* l_Building = nullptr;

        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        {
            if (m_Buildings[l_I].PlotInstanceID == p_PlotInstanceID)
            {
                l_Building = &m_Buildings[l_I];
                break;
            }
        }

        if (!l_Building)
            return;

        GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_Building->BuildingID);

        if (!l_BuildingEntry)
            return;

        l_Building->Active = true;

        UpdatePlot(p_PlotInstanceID);

        WorldPacket l_Packet(SMSG_GARRISON_BUILDING_ACTIVATED, 4);
        l_Packet << uint32(p_PlotInstanceID);
        m_Owner->SendDirectMessage(&l_Packet);

        UpdateStats();

        switch (l_BuildingEntry->Type)
        {
            case BuildingType::Fishing:
            case BuildingType::Mine:
            case BuildingType::Farm:
            {
                WorldPacket l_NullPacket;
                m_Owner->GetSession()->HandleGetGarrisonInfoOpcode(l_NullPacket);
                break;
            }
        }

        if (GetGarrisonScript())
        {
            GarrPlotInstanceEntry const* l_PlotInstanceEntry = sGarrPlotInstanceStore.LookupEntry(p_PlotInstanceID);

            if (l_PlotInstanceEntry && !m_LastPlotBuildingType.empty() && m_LastPlotBuildingType.find(l_PlotInstanceEntry->PlotID) != m_LastPlotBuildingType.end() && m_LastPlotBuildingType[l_PlotInstanceEntry->PlotID] == l_BuildingEntry->Type)
            {
                GetGarrisonScript()->OnUpgradeBuilding(m_Owner, l_BuildingEntry->ID);
                m_LastPlotBuildingType.erase(l_PlotInstanceEntry->PlotID);
            }
            else
                GetGarrisonScript()->OnBuildingActivated(m_Owner, l_Building->BuildingID);
        }
    }

    /// Activate building
    void Manager::ActivateBuilding()
    {
        uint32 l_PlotInstance = GetPlotInstanceIDByActivationGameObject(m_LastUsedActivationGameObject);

        if (l_PlotInstance)
            ActivateBuilding(l_PlotInstance);
    }

    /// Cancel construction
    void Manager::CancelConstruction(uint32 p_PlotInstanceID)
    {
        if (!HasPlotInstance(p_PlotInstanceID))
            return;

        uint32 l_BuildingID = GetBuilding(p_PlotInstanceID).BuildingID;

        if (!l_BuildingID)
            return;

        GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_BuildingID);

        if (!l_BuildingEntry)
            return;

        DeleteBuilding(p_PlotInstanceID, true, false);

        if (l_BuildingEntry->CostCurrencyID != 0)
            m_Owner->ModifyCurrency(l_BuildingEntry->CostCurrencyID, (int32)l_BuildingEntry->CostCurrencyAmount);

        if (l_BuildingEntry->CostMoney != 0)
            m_Owner->ModifyMoney(l_BuildingEntry->CostMoney);
    }

    /// Delete building
    void Manager::DeleteBuilding(uint32 p_PlotInstanceID, bool p_Canceled, bool p_RemoveForUpgrade)
    {
        if (!HasPlotInstance(p_PlotInstanceID))
            return;

        uint32 l_BuildingID = GetBuilding(p_PlotInstanceID).BuildingID;

        if (!l_BuildingID)
            return;

        GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_BuildingID);

        if (!l_BuildingEntry)
            return;

        if (GetGarrisonScript())
        {
            GetGarrisonScript()->OnDeleteBuilding(m_Owner, l_BuildingID, l_BuildingEntry->Type, p_RemoveForUpgrade);
        }

        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_BUILDING);
        l_Stmt->setUInt32(0, GetBuilding(p_PlotInstanceID).DatabaseID);
        CharacterDatabase.AsyncQuery(l_Stmt);

        for (std::vector<GarrisonBuilding>::iterator l_It = m_Buildings.begin(); l_It != m_Buildings.end(); ++l_It)
        {
            if (l_It->BuildingID == l_BuildingID)
            {
                m_Buildings.erase(l_It);
                break;
            }
        }

        if (p_Canceled)
        {
            GarrPlotInstanceEntry const* l_PlotInstanceEntry = sGarrPlotInstanceStore.LookupEntry(p_PlotInstanceID);

            if (l_PlotInstanceEntry && !m_LastPlotBuildingType.empty() && m_LastPlotBuildingType.find(l_PlotInstanceEntry->PlotID) != m_LastPlotBuildingType.end())
                m_LastPlotBuildingType.erase(l_PlotInstanceEntry->PlotID);
        }

        AssignFollowerToBuilding(GetBuilding(p_PlotInstanceID).FollowerAssigned, 0);

        UpdatePlot(p_PlotInstanceID);

        GarrisonPlotInstanceInfoLocation l_PlotLocation = GetPlot(p_PlotInstanceID);

        WorldPacket l_PlotPlacedPacket(SMSG_GARRISON_PLOT_PLACED, 24);
        l_PlotPlacedPacket << uint32(p_PlotInstanceID);
        l_PlotPlacedPacket << float(l_PlotLocation.X);
        l_PlotPlacedPacket << float(l_PlotLocation.Y);
        l_PlotPlacedPacket << float(l_PlotLocation.Z);
        l_PlotPlacedPacket << float(l_PlotLocation.O);
        l_PlotPlacedPacket << uint32(GetPlotType(p_PlotInstanceID));

        m_Owner->SendDirectMessage(&l_PlotPlacedPacket);

        WorldPacket l_BuildingRemovedPacket(SMSG_GARRISON_BUILDING_REMOVED, 12);
        l_BuildingRemovedPacket << uint32(PurchaseBuildingResults::Ok);
        l_BuildingRemovedPacket << uint32(p_PlotInstanceID);
        l_BuildingRemovedPacket << uint32(l_BuildingID);

        m_Owner->SendDirectMessage(&l_BuildingRemovedPacket);
    }

    /// Has active building
    bool Manager::HasActiveBuilding(uint32 p_BuildingID) const
    {
        for (std::vector<GarrisonBuilding>::const_iterator l_It = m_Buildings.begin(); l_It != m_Buildings.end(); ++l_It)
        {
            if (l_It->BuildingID == p_BuildingID && l_It->Active)
                return true;
        }

        return false;
    }

    /// Has building type
    bool Manager::HasBuildingType(BuildingType::Type p_BuildingType, bool p_DontNeedActive) const
    {
        for (std::vector<GarrisonBuilding>::const_iterator l_It = m_Buildings.begin(); l_It != m_Buildings.end(); ++l_It)
        {
            GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_It->BuildingID);

            if (!l_BuildingEntry)
                continue;

            if (l_BuildingEntry->Type == p_BuildingType && (p_DontNeedActive ? true : (*l_It).Active == true))
                return true;
        }

        return false;
    }

    /// Get building with type
    GarrisonBuilding Manager::GetBuildingWithType(BuildingType::Type p_BuildingType, bool p_DontNeedActive) const
    {
        for (GarrisonBuilding l_Building : m_Buildings)
        {
            GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_Building.BuildingID);

            if (!l_BuildingEntry)
                continue;

            if (l_BuildingEntry->Type == p_BuildingType && (p_DontNeedActive ? true : l_Building.Active == true))
                return l_Building;
        }

        return GarrisonBuilding();
    }

    uint32 Manager::GetBuildingLevel(GarrisonBuilding p_Building) const
    {
        if (GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_Building.BuildingID))
            return l_BuildingEntry->Level;

        return 0;
    }

    /// Get building max work order
    uint32 Manager::GetBuildingMaxWorkOrder(uint32 p_PlotInstanceID) const
    {
        if (!HasPlotInstance(p_PlotInstanceID))
            return 0;

        uint32 l_BuildingID = GetBuilding(p_PlotInstanceID).BuildingID;

        if (!l_BuildingID)
            return 0;

        GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_BuildingID);

        if (!l_BuildingEntry)
            return 0;

        uint32 l_MaxWorkOrder = 0;

        for (uint32 l_CurrentValue : gGarrisonBuildingMaxWorkOrderPerBuildingLevel)
        {
            if (l_CurrentValue == l_BuildingEntry->MaxShipments)
            {
                l_MaxWorkOrder += l_BuildingEntry->MaxShipments;
                break;
            }
        }

        if (!l_MaxWorkOrder)
            return 0;

        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        {
            GarrBuildingEntry const* l_Building = sGarrBuildingStore.LookupEntry(m_Buildings[l_I].BuildingID);

            if (!l_Building)
                continue;

            if (l_Building->Type != BuildingType::StoreHouse)
                continue;

            l_MaxWorkOrder += l_Building->MaxShipments;
        }

        return l_MaxWorkOrder;
    }

    /// Get in progress work order count
    uint32 Manager::GetWorkOrderCount(uint32 p_PlotInstanceID) const
    {
        return (uint32)std::count_if(m_WorkOrders.begin(), m_WorkOrders.end(), [p_PlotInstanceID](const GarrisonWorkOrder& p_Order) -> bool
        {
            return p_Order.PlotInstanceID == p_PlotInstanceID;
        });
    }

    std::vector<GarrisonWorkOrder> Manager::GetBuildingWorkOrders(uint32 p_PlotInstanceID) const
    {
        std::vector<GarrisonWorkOrder> l_Orders;

        for (GarrisonWorkOrder l_Order : m_WorkOrders)
        {
            if (p_PlotInstanceID == l_Order.PlotInstanceID)
                l_Orders.push_back(l_Order);
        }

        return l_Orders;
    }

    /// Start new work order
    uint64 Manager::StartWorkOrder(uint32 p_PlotInstanceID, uint32 p_ShipmentID)
    {
        CharShipmentEntry const* l_ShipmentEntry = sCharShipmentStore.LookupEntry(p_ShipmentID);

        if (!l_ShipmentEntry)
            return 0;

        uint32 l_MaxCompleteTime = time(0);

        for (uint32 l_I = 0; l_I < m_WorkOrders.size(); ++l_I)
        {
            if (m_WorkOrders[l_I].PlotInstanceID == p_PlotInstanceID)
                l_MaxCompleteTime = std::max<uint32>(l_MaxCompleteTime, m_WorkOrders[l_I].CompleteTime);
        }

        GarrisonWorkOrder l_WorkOrder;
        l_WorkOrder.DatabaseID      = sObjectMgr->GetNewGarrisonWorkOrderID();
        l_WorkOrder.PlotInstanceID  = p_PlotInstanceID;
        l_WorkOrder.ShipmentID      = p_ShipmentID;
        l_WorkOrder.CreationTime    = l_MaxCompleteTime;
        l_WorkOrder.CompleteTime    = l_MaxCompleteTime + l_ShipmentEntry->Duration;

        PreparedStatement * l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GARRISON_WORKORDER);

        uint32 l_Index = 0;
        l_Stmt->setUInt32(l_Index++, l_WorkOrder.DatabaseID);
        l_Stmt->setUInt32(l_Index++, m_ID);
        l_Stmt->setUInt32(l_Index++, l_WorkOrder.PlotInstanceID);
        l_Stmt->setUInt32(l_Index++, l_WorkOrder.ShipmentID);
        l_Stmt->setUInt32(l_Index++, l_WorkOrder.CreationTime);
        l_Stmt->setUInt32(l_Index++, l_WorkOrder.CompleteTime);

        CharacterDatabase.AsyncQuery(l_Stmt);

        m_WorkOrders.push_back(l_WorkOrder);

        return l_WorkOrder.DatabaseID;
    }

    /// Delete work order
    void Manager::DeleteWorkOrder(uint64 p_DBID)
    {
        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_WORKORDER);
        l_Stmt->setUInt32(0, p_DBID);
        CharacterDatabase.AsyncQuery(l_Stmt);

        for (std::vector<GarrisonWorkOrder>::iterator l_It = m_WorkOrders.begin(); l_It != m_WorkOrders.end(); ++l_It)
        {
            if (l_It->DatabaseID == p_DBID)
            {
                m_WorkOrders.erase(l_It);
                break;
            }
        }
    }

    uint8 Manager::CalculateAssignedFollowerShipmentBonus(uint32 p_PlotInstanceID)
    {
        std::map<uint32, uint32> l_FollowerLevelBonus =
        {
            { 90, 50 },
            { 91, 55 },
            { 92, 60 },
            { 93, 65 },
            { 94, 70 },
            { 95, 75 },
            { 96, 80 },
            { 97, 85 },
            { 98, 90 },
            { 99, 95 },
            { 100, 100 }
        };

        GarrisonFollower* l_Follower = GetFollower(GetBuilding(p_PlotInstanceID).FollowerAssigned);

        if (l_Follower == nullptr)
            return 1;

        return HasRequiredFollowerAssignedAbility(p_PlotInstanceID) ? roll_chance_i(l_FollowerLevelBonus[l_Follower->Level]) + 1 : 1;
    }

    GarrisonFollower* Manager::GetAssignedFollower(uint32 p_PlotInstanceID)
    {
        return GetFollowerWithDatabaseID(GetBuilding(p_PlotInstanceID).FollowerAssigned);
    }

    bool Manager::HasRequiredFollowerAssignedAbility(uint32 p_PlotInstanceID)
    {
        GarrisonFollower* l_Follower = GetFollowerWithDatabaseID(GetBuilding(p_PlotInstanceID).FollowerAssigned);

        if (l_Follower == nullptr)
            return false;

        GarrBuildingEntry const* l_Building = sGarrBuildingStore.LookupEntry(GetBuilding(p_PlotInstanceID).BuildingID);

        if (l_Building == nullptr)
            return false;

        for (uint32 l_Ability : l_Follower->Abilities)
        {
            if (l_Ability == l_Building->FollowerRequiredGarrAbilityID)
                return true;
        }

        return false;
    }

    uint32 Manager::CalculateArmoryWorkOrder() const
    {
        std::vector<uint32> l_UncommonRewards = { 114745, 114807, 114128, 114616 };
        std::vector<uint32> l_RareRewards     = { 114808, 114806, 114129, 114081 };
        std::vector<uint32> l_EpicRewards     = { 114822, 114746, 114131, 114622 };

        uint32 l_Chance = urand(0, 100);

        if (l_Chance > 40)
            return l_UncommonRewards[urand(0, l_UncommonRewards.size() - 1)];
        else if (l_Chance > 10)
            return l_RareRewards[urand(0, l_RareRewards.size() - 1)];
        else
            return l_EpicRewards[urand(0, l_EpicRewards.size() - 1)];

        return 0;
    }

    bool Manager::FillSanctumWorkOrderRewards(std::map<uint32, uint32>& l_RewardItems, std::map<CurrencyTypes, uint32>& l_RewardCurrencies)
    {
        uint8 l_Chance = urand(1, 100);
        /// 50%
        if (l_Chance >= 50)
        {
            uint32 l_Count = urand(100, 150) * CURRENCY_PRECISION;
            auto l_Itr = l_RewardCurrencies.find(CurrencyTypes::CURRENCY_TYPE_HONOR_POINTS);

            if (l_Itr != l_RewardCurrencies.end())
                l_Itr->second += l_Count;
            else
                l_RewardCurrencies.insert({CurrencyTypes::CURRENCY_TYPE_HONOR_POINTS, l_Count});

            return true;
        }
        /// 30%
        else if (l_Chance >= 20)
        {
            uint32 l_CurrencyCount = 0;

            if (m_Owner->GetCurrencyWeekCap(CurrencyTypes::CURRENCY_TYPE_CONQUEST_META_ARENA_BG, false) > m_Owner->GetCurrencyOnWeek(CURRENCY_TYPE_CONQUEST_META_ARENA_BG, false))
            {

                l_CurrencyCount = urand(75, 150) * CURRENCY_PRECISION;

                auto l_Itr = l_RewardCurrencies.find(CurrencyTypes::CURRENCY_TYPE_CONQUEST_META_ARENA_BG);
                if (l_Itr != l_RewardCurrencies.end())
                    l_Itr->second += l_CurrencyCount;
                else
                    l_RewardCurrencies.insert({ CurrencyTypes::CURRENCY_TYPE_CONQUEST_META_ARENA_BG, l_CurrencyCount });

                return true;
            }

            l_CurrencyCount = urand(100, 150) * CURRENCY_PRECISION;

            auto l_Itr = l_RewardCurrencies.find(CurrencyTypes::CURRENCY_TYPE_HONOR_POINTS);
            if (l_Itr != l_RewardCurrencies.end())
                l_Itr->second += l_CurrencyCount;
            else
                l_RewardCurrencies.insert({ CurrencyTypes::CURRENCY_TYPE_HONOR_POINTS, l_CurrencyCount });

            return true;
        }
        /// 20%
        else
        {
            LootTemplate const* l_LootTemplate = LootTemplates_Item.GetLootFor(m_Owner->GetTeam() == ALLIANCE ? 126906 : 126901); //< Golden Strongbox Loot

            std::list<ItemTemplate const*> l_LootTable;
            std::vector<uint32> l_Items;
            uint32 l_SpecID = m_Owner->GetLootSpecId() ? m_Owner->GetLootSpecId() : m_Owner->GetSpecializationId(m_Owner->GetActiveSpec());

            l_LootTemplate->FillAutoAssignationLoot(l_LootTable);

            if (!l_SpecID)
                l_SpecID = m_Owner->GetDefaultSpecId();

            for (ItemTemplate const* l_Template : l_LootTable)
            {
                if ((l_Template->AllowableClass && !(l_Template->AllowableClass & m_Owner->getClassMask())) ||
                    (l_Template->AllowableRace && !(l_Template->AllowableRace & m_Owner->getRaceMask())))
                    continue;

                for (SpecIndex l_ItemSpecID : l_Template->specs[1])
                {
                    if (l_ItemSpecID == l_SpecID)
                        l_Items.push_back(l_Template->ItemId);
                }
            }

            if ((uint32)l_Items.size())
                l_RewardItems.insert({ l_Items[urand(0, l_Items.size())], 1 });
        }

        return false;
    }

    void Manager::InsertNewCreatureInPlotDatas(uint32 p_PlotInstanceID, uint64 p_Guid)
    {
        m_PlotsCreatures[p_PlotInstanceID].push_back(p_Guid);
    }

    /// Get creature plot instance ID
    uint32 Manager::GetCreaturePlotInstanceID(uint64 p_GUID) const
    {
        for (auto & l_Pair : m_PlotsCreatures)
        {
            auto l_It = std::find(l_Pair.second.begin(), l_Pair.second.end(), p_GUID);

            if (l_It != l_Pair.second.end())
                return l_Pair.first;
        }

        return 0;
    }

    /// Get gameobject plot instance ID
    uint32 Manager::GetGameObjectPlotInstanceID(uint64 p_GUID) const
    {
        for (auto & l_Pair : m_PlotsGameObjects)
        {
            auto l_It = std::find(l_Pair.second.begin(), l_Pair.second.end(), p_GUID);

            if (l_It != l_Pair.second.end())
                return l_Pair.first;
        }

        return 0;
    }

    /// Get building gathering data
    /// @p_PlotInstanceID : Plot building location
    std::string Manager::GetBuildingGatheringData(uint32 p_PlotInstanceID)
    {
        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        {
            if (m_Buildings[l_I].PlotInstanceID == p_PlotInstanceID)
                return m_Buildings[l_I].GatheringData;
        }

        return "";
    }

    /// Set building gathering data
    /// @p_PlotInstanceID   : Plot building location
    /// @p_Data             : Gathering data
    void Manager::SetBuildingGatheringData(uint32 p_PlotInstanceID, std::string p_Data)
    {
        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        {
            if (m_Buildings[l_I].PlotInstanceID == p_PlotInstanceID)
            {
                m_Buildings[l_I].GatheringData = p_Data;
                return;
            }
        }
    }

    /// Get list of creature in a specific building type
    /// @p_Type : Building type
    std::vector<uint64> Manager::GetBuildingCreaturesByBuildingType(BuildingType::Type p_Type)
    {
        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        {
            GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(m_Buildings[l_I].BuildingID);

            if (!l_BuildingEntry)
                continue;

            if (l_BuildingEntry->Type == p_Type && m_Buildings[l_I].Active == true)
                return m_PlotsCreatures[m_Buildings[l_I].PlotInstanceID];
        }

        return std::vector<uint64>();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get known blueprints
    std::vector<int32> Manager::GetKnownBlueprints() const
    {
        return m_KnownBlueprints;
    }

    /// Learn blue print
    bool Manager::LearnBlueprint(uint32 p_BuildingRecID)
    {
        LearnBluePrintResults::Type l_ResultCode;

        if (std::find(m_KnownBlueprints.begin(), m_KnownBlueprints.end(), p_BuildingRecID) == m_KnownBlueprints.end())
        {
            GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingRecID);

            if (l_BuildingEntry)
            {
                m_KnownBlueprints.push_back(p_BuildingRecID);
                l_ResultCode = LearnBluePrintResults::Learned;

                m_Owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LEARN_GARRISON_BLUEPRINTS);
            }
            else
            {
                l_ResultCode = LearnBluePrintResults::UnableToLearn;
            }
        }
        else
        {
            l_ResultCode = LearnBluePrintResults::Known;
        }

        WorldPacket l_Result(SMSG_GARRISON_LEARN_BLUEPRINT_RESULT, 8);
        l_Result << uint32(l_ResultCode);
        l_Result << uint32(p_BuildingRecID);

        m_Owner->SendDirectMessage(&l_Result);

        return true;
    }

    /// Known blue print
    bool Manager::KnownBlueprint(uint32 p_BuildingRecID) const
    {
        return std::find(m_KnownBlueprints.begin(), m_KnownBlueprints.end(), p_BuildingRecID) != m_KnownBlueprints.end();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get known specializations
    std::vector<int32> Manager::GetKnownSpecializations() const
    {
        return m_KnownSpecializations;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get work orders
    std::vector<GarrisonWorkOrder>& Manager::GetWorkOrders()
    {
        return m_WorkOrders;
    }

    /// Check if any followers has ability in parameter
    bool Manager::HasFollowerAbility(uint32 p_AbilityID) const
    {
        std::vector<GarrisonFollower> l_Followers = GetFollowers();

        for (GarrisonFollower l_Follower : l_Followers)
        {
            for (auto l_Ability : l_Follower.Abilities)
            {
                if (l_Ability == p_AbilityID)
                    return true;
            }
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Init
    void Manager::Init()
    {
        InitDataForLevel();
        InitPlots();
        UpdateStats();
    }

    /// Init data for level
    void Manager::InitDataForLevel()
    {
        GarrSiteLevelEntry const* l_SiteEntry = nullptr;

        /// Search garrison site entry by SideID & Level
        for (uint32 l_I = 0; l_I < sGarrSiteLevelStore.GetNumRows(); ++l_I)
        {
            GarrSiteLevelEntry const* l_CurrentSiteEntry = sGarrSiteLevelStore.LookupEntry(l_I);

            if (l_CurrentSiteEntry && l_CurrentSiteEntry->Level == m_GarrisonLevel && l_CurrentSiteEntry->SiteID == m_GarrisonSiteID)
            {
                l_SiteEntry = l_CurrentSiteEntry;
                break;
            }
        }

        if (l_SiteEntry == nullptr)
        {
            sLog->outAshran("Garrison::InitDataForLevel() not data found");
            assert(false && "Garrison::InitDataForLevel() not data found");
            return;
        }

        m_GarrisonLevelID = l_SiteEntry->SiteLevelID;

        /// Find all plots for garrison level
        m_Plots.clear();

        for (uint32 l_I = 0; l_I < Globals::PlotInstanceCount; ++l_I)
        {
            if (gGarrisonPlotInstanceInfoLocation[l_I].SiteLevelID == m_GarrisonLevelID)
                m_Plots.push_back(gGarrisonPlotInstanceInfoLocation[l_I]);
        }

        /// Add prebuilt buildings
        for (uint32 l_I = 0; l_I < sGarrBuildingStore.GetNumRows(); ++l_I)
        {
            GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_I);

            if (!l_BuildingEntry || l_BuildingEntry->BuildingCategory != BuildingCategory::Prebuilt)
                continue;

            if (HasBuildingType((BuildingType::Type)l_BuildingEntry->Type, true))
                continue;

            uint32 l_PlotID = 0;

            /// Search building plot ID
            for (uint32 l_I = 0; l_I < sGarrPlotBuildingStore.GetNumRows(); ++l_I)
            {
                GarrPlotBuildingEntry const* l_PlotBuildingEntry = sGarrPlotBuildingStore.LookupEntry(l_I);

                if (l_PlotBuildingEntry && l_PlotBuildingEntry->BuildingID == l_BuildingEntry->ID)
                {
                    l_PlotID = l_PlotBuildingEntry->PlotID;
                    break;
                }
            }

            if (!l_PlotID)
                continue;

            uint32 l_PlotInstanceID = 0;

            /// Search building plot instance ID
            for (uint32 l_I = 0; l_I < sGarrBuildingStore.GetNumRows(); ++l_I)
            {
                GarrPlotInstanceEntry const* l_PlotInstanceEntry = sGarrPlotInstanceStore.LookupEntry(l_I);

                if (l_PlotInstanceEntry && l_PlotInstanceEntry->PlotID == l_PlotID)
                {
                    l_PlotInstanceID = l_PlotInstanceEntry->InstanceID;
                    break;
                }
            }

            if (!l_PlotInstanceID || !HasPlotInstance(l_PlotInstanceID))
                continue;

            PurchaseBuilding(l_BuildingEntry->ID, l_PlotInstanceID, true);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Init Game objects
    void Manager::InitPlots()
    {
        if (!m_Owner->IsInGarrison())
            return;

        for (uint32 l_I = 0; l_I < m_Plots.size(); ++l_I)
            UpdatePlot(m_Plots[l_I].PlotInstanceID);
    }

    /// Uninit plots
    void Manager::UninitPlots()
    {
        for (std::map<uint32, uint64>::iterator l_It = m_PlotsActivateGob.begin(); l_It != m_PlotsActivateGob.end(); ++l_It)
        {
            GameObject* l_Gob = HashMapHolder<GameObject>::Find(l_It->second);

            if (l_Gob)
            {
                l_Gob->DestroyForNearbyPlayers();
                l_Gob->AddObjectToRemoveList();
            }
        }

        m_PlotsActivateGob.clear();

        for (std::map<uint32, uint64>::iterator l_It = m_PlotsGob.begin(); l_It != m_PlotsGob.end(); ++l_It)
        {
            GameObject* l_Gob = HashMapHolder<GameObject>::Find(l_It->second);

            if (l_Gob)
            {
                l_Gob->DestroyForNearbyPlayers();
                l_Gob->AddObjectToRemoveList();
            }
        }

        m_PlotsGob.clear();

        for (std::map<uint32, std::vector<uint64>>::iterator l_It = m_PlotsGameObjects.begin(); l_It != m_PlotsGameObjects.end(); ++l_It)
        {
            for (uint32 l_Y = 0; l_Y < l_It->second.size(); ++l_Y)
            {
                GameObject* l_Gob = HashMapHolder<GameObject>::Find(l_It->second[l_Y]);

                if (l_Gob)
                {
                    l_Gob->DestroyForNearbyPlayers();
                    l_Gob->AddObjectToRemoveList();
                }
            }
        }

        m_PlotsGameObjects.clear();

        for (std::map<uint32, std::vector<uint64>>::iterator l_It = m_PlotsCreatures.begin(); l_It != m_PlotsCreatures.end(); ++l_It)
        {
            for (uint32 l_Y = 0; l_Y < l_It->second.size(); ++l_Y)
            {
                Creature* l_Crea = HashMapHolder<Creature>::Find(l_It->second[l_Y]);

                if (l_Crea)
                {
                    l_Crea->DestroyForNearbyPlayers();
                    l_Crea->AddObjectToRemoveList();

                    if (l_Crea->AI())
                        l_Crea->AI()->SetData(CreatureAIDataIDs::DespawnData, 0);
                }
            }
        }

        m_PlotsCreatures.clear();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Update plot game object
    void Manager::UpdatePlot(uint32 p_PlotInstanceID)
    {
        if (!m_Owner->IsInGarrison())
            return;

        GarrisonPlotInstanceInfoLocation    l_PlotInfo = GetPlot(p_PlotInstanceID);
        GarrisonBuilding                    l_Building = GetBuilding(p_PlotInstanceID);
    
        if (m_PlotsGob[p_PlotInstanceID] != 0)
        {
            GameObject* l_Gob = HashMapHolder<GameObject>::Find(m_PlotsGob[p_PlotInstanceID]);

            if (l_Gob)
            {
                m_Owner->SendObjectDeSpawnAnim(m_PlotsGob[p_PlotInstanceID]);
                l_Gob->DestroyForNearbyPlayers();
                l_Gob->AddObjectToRemoveList();
            }

            m_PlotsGob[p_PlotInstanceID] = 0;
        }
    
        m_PlotsWorkOrderGob[p_PlotInstanceID] = 0;

        uint32 l_GobEntry = 0;
        bool l_SpanwActivateGob = false;
        bool l_IsPlotBuilding = false;

        if (PlotIsFree(p_PlotInstanceID))
        {
            l_GobEntry = gGarrisonEmptyPlotGameObject[GetPlotType(p_PlotInstanceID) + (GetGarrisonFactionIndex() * PlotTypes::Max)];
        }
        else
        {
            GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_Building.BuildingID);

            if (!l_BuildingEntry)
                return;

            if (!l_Building.Active &&
                (l_BuildingEntry->Type != BuildingType::Mine &&
                 l_BuildingEntry->Type != BuildingType::Farm &&
                 l_BuildingEntry->Type != BuildingType::Fishing &&
                 l_BuildingEntry->Type != BuildingType::PetMenagerie))
            {
                l_GobEntry = gGarrisonBuildingPlotGameObject[GetPlotType(p_PlotInstanceID) + (GetGarrisonFactionIndex() * PlotTypes::Max)];

                if (time(0) > l_Building.TimeBuiltEnd)
                    l_SpanwActivateGob  = true;

                l_IsPlotBuilding = true;
            }
            else
            {
                l_GobEntry = l_BuildingEntry->GameObjects[GetGarrisonFactionIndex()];

                if (!l_Building.Active && l_BuildingEntry->Level > 1 &&
                    (l_BuildingEntry->Type == BuildingType::Mine ||
                     l_BuildingEntry->Type == BuildingType::Farm ||
                     l_BuildingEntry->Type == BuildingType::Fishing ||
                     l_BuildingEntry->Type == BuildingType::PetMenagerie))
                {
                    uint32 l_TargetLevel = l_BuildingEntry->Level - 1;

                    for (uint32 l_I = 0; l_I < sGarrBuildingStore.GetNumRows(); ++l_I)
                    {
                        GarrBuildingEntry const* l_CurrentEntry = sGarrBuildingStore.LookupEntry(l_I);

                        if (!l_CurrentEntry || l_CurrentEntry->Type != l_BuildingEntry->Type || l_CurrentEntry->Level != l_TargetLevel)
                            continue;

                        l_GobEntry = l_CurrentEntry->GameObjects[GetGarrisonFactionIndex()];

                        if (time(0) > l_Building.TimeBuiltEnd)
                            l_SpanwActivateGob  = true;

                        break;
                    }
                }
            }
        }

        if (l_GobEntry != 0)
        {
            GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_Building.BuildingID);
            GameObject* l_Gob = m_Owner->SummonGameObject(l_GobEntry, l_PlotInfo.X, l_PlotInfo.Y, l_PlotInfo.Z, l_PlotInfo.O, 0, 0, 0, 0, 0, 0, 0, 255, 0, true);
        
            if (l_Gob)
            {
                m_PlotsGob[p_PlotInstanceID] = l_Gob->GetGUID();

                if (m_PlotsGameObjects[p_PlotInstanceID].size() != 0 || m_PlotsCreatures[p_PlotInstanceID].size() != 0)
                {
                    for (uint32 l_I = 0; l_I < m_PlotsCreatures[p_PlotInstanceID].size(); ++l_I)
                    {
                        Creature* l_Crea = HashMapHolder<Creature>::Find(m_PlotsCreatures[p_PlotInstanceID][l_I]);

                        if (l_Crea)
                        {
                            l_Crea->SetFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_DISABLE_CLIENT_SIDE);
                            l_Crea->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_UNK1);

                            if (l_Crea->AI())
                                l_Crea->AI()->SetData(CreatureAIDataIDs::DespawnData, 0);

                            UpdateData l_UpdateData(m_Owner->GetMapId());
                            WorldPacket l_UpdatePacket;

                            l_Crea->BuildValuesUpdateBlockForPlayer(&l_UpdateData, m_Owner);
                                
                            if (l_UpdateData.HasData())
                            {
                                if (l_UpdateData.BuildPacket(&l_UpdatePacket))
                                    m_Owner->SendDirectMessage(&l_UpdatePacket);
                            }

                            l_Crea->DestroyForNearbyPlayers();
                            l_Crea->AddObjectToRemoveList();
                        }
                    }

                    m_PlotsCreatures[p_PlotInstanceID].clear();

                    for (uint32 l_I = 0; l_I < m_PlotsGameObjects[p_PlotInstanceID].size(); ++l_I)
                    {
                        GameObject* l_Gob = HashMapHolder<GameObject>::Find(m_PlotsGameObjects[p_PlotInstanceID][l_I]);

                        if (l_Gob)
                        {
                            m_Owner->SendObjectDeSpawnAnim(m_PlotsGameObjects[p_PlotInstanceID][l_I]);
                            l_Gob->DestroyForNearbyPlayers();
                            l_Gob->AddObjectToRemoveList();
                        }
                    }

                    m_PlotsGameObjects[p_PlotInstanceID].clear();
                }

                G3D::Vector3 l_NonRotatedPosition;
                {
                    G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
                    l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), -l_PlotInfo.O);

                    /// transform plot coord
                    l_NonRotatedPosition = l_Mat * G3D::Vector3(l_PlotInfo.X, l_PlotInfo.Y, l_PlotInfo.Z);
                }

                std::vector<GarrisonPlotBuildingContent> l_Contents;

                if (l_IsPlotBuilding)
                    l_Contents = sObjectMgr->GetGarrisonPlotBuildingContent(GetPlotType(p_PlotInstanceID), GetGarrisonFactionIndex());
                else if ((l_Building.Active ||
                         (l_BuildingEntry && (l_BuildingEntry->Type == BuildingType::Mine
                                           || l_BuildingEntry->Type == BuildingType::PetMenagerie
                                           || l_BuildingEntry->Type == BuildingType::Fishing
                                           || l_BuildingEntry->Type == BuildingType::Farm))
                         )
                        && l_Building.BuildingID)
                {
                    uint32 l_BuildingID = l_Building.BuildingID;

                    if (!l_Building.Active && l_BuildingEntry->Level > 1 &&
                        (l_BuildingEntry->Type == BuildingType::Mine ||
                         l_BuildingEntry->Type == BuildingType::Farm ||
                         l_BuildingEntry->Type == BuildingType::Fishing ||
                         l_BuildingEntry->Type == BuildingType::PetMenagerie))
                    {
                        uint32 l_TargetLevel = l_BuildingEntry->Level - 1;

                        for (uint32 l_I = 0; l_I < sGarrBuildingStore.GetNumRows(); ++l_I)
                        {
                            GarrBuildingEntry const* l_CurrentEntry = sGarrBuildingStore.LookupEntry(l_I);

                            if (!l_CurrentEntry || l_CurrentEntry->Type != l_BuildingEntry->Type || l_CurrentEntry->Level != l_TargetLevel)
                                continue;

                            l_BuildingID = l_CurrentEntry->ID;
                            break;
                        }
                    }

                    l_Contents = sObjectMgr->GetGarrisonPlotBuildingContent(-(int32)l_BuildingID, GetGarrisonFactionIndex());
                }

                for (uint32 l_I = 0; l_I < l_Contents.size(); ++l_I)
                {
                    if (l_IsPlotBuilding && l_Contents[l_I].PlotTypeOrBuilding < 0)
                        continue;

                    if (!l_IsPlotBuilding)
                    {
                        if (!l_Building.BuildingID)
                            continue;

                        int32 l_NegPlotTypeOrBuilding = -l_Contents[l_I].PlotTypeOrBuilding;

                        if (l_Building.Active && l_NegPlotTypeOrBuilding != l_Building.BuildingID) ///< Comparison of integers of different signs: 'const uint32' (aka 'const unsigned int') and 'const int32' (aka 'const int')
                            continue;
                    }

                    G3D::Vector3 l_Position = G3D::Vector3(l_Contents[l_I].X, l_Contents[l_I].Y, 0);

                    G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
                    l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), l_PlotInfo.O);

                    l_Position.x += l_NonRotatedPosition.x;
                    l_Position.y += l_NonRotatedPosition.y;

                    l_Position = l_Mat * l_Position;

                    l_Position.z = l_Contents[l_I].Z + l_PlotInfo.Z;

                    if (l_Contents[l_I].CreatureOrGob > 0)
                    {
                        Creature* l_Creature = m_Owner->SummonCreature(l_Contents[l_I].CreatureOrGob, l_Position.x, l_Position.y, l_Position.z, l_Contents[l_I].O + l_PlotInfo.O, TEMPSUMMON_MANUAL_DESPAWN);

                        if (l_Creature)
                        {
                            m_PlotsCreatures[p_PlotInstanceID].push_back(l_Creature->GetGUID());

                            if (l_Creature->AI())
                            {
                                if (l_IsPlotBuilding)
                                    l_Creature->AI()->SetData(CreatureAIDataIDs::Builder, 1);
                                else
                                {
                                    l_Creature->AI()->SetGUID(m_Owner->GetGUID(), CreatureAIDataIDs::OwnerGuid); ///< Value first, data index next
                                    l_Creature->AI()->SetData(CreatureAIDataIDs::BuildingID,    -l_Contents[l_I].PlotTypeOrBuilding);
                                    l_Creature->AI()->SetData(CreatureAIDataIDs::PlotInstanceID, p_PlotInstanceID | (GetGarrisonSiteLevelEntry()->SiteLevelID << 16));
                                }
                            }
                        }
                    }
                    else
                    {
                        GameObject* l_Cosmetic = m_Owner->SummonGameObject(-l_Contents[l_I].CreatureOrGob, l_Position.x, l_Position.y, l_Position.z, l_Contents[l_I].O + l_PlotInfo.O, 0, 0, 0, 0, 0);

                        if (l_Cosmetic)
                        {
                            m_PlotsGameObjects[p_PlotInstanceID].push_back(l_Cosmetic->GetGUID());

                            if (l_Cosmetic->GetGoType() == GAMEOBJECT_TYPE_GARRISON_SHIPMENT)
                                m_PlotsWorkOrderGob[p_PlotInstanceID] = l_Cosmetic->GetGUID();
                        }
                    }
                }

                if (m_PlotsActivateGob[p_PlotInstanceID] != 0)
                {
                    GameObject* l_Gob = HashMapHolder<GameObject>::Find(m_PlotsActivateGob[p_PlotInstanceID]);

                    if (l_Gob)
                    {
                        m_Owner->SendObjectDeSpawnAnim(m_PlotsActivateGob[p_PlotInstanceID]);
                        l_Gob->DestroyForNearbyPlayers();
                        l_Gob->AddObjectToRemoveList();
                    }
                }

                if (l_SpanwActivateGob)
                {
                    G3D::Vector3 l_FinalPosition;
                    float l_FinalOrientation = l_PlotInfo.O;

                    if (   l_BuildingEntry->Type == BuildingType::Farm
                        || l_BuildingEntry->Type == BuildingType::Mine
                        || l_BuildingEntry->Type == BuildingType::PetMenagerie
                        || l_BuildingEntry->Type == BuildingType::Fishing)
                    {
                        uint32 l_Lvl = GetGarrisonSiteLevelEntry()->Level;

                        if (GetGarrisonFactionIndex() == FactionIndex::Alliance)
                        {
                            switch (l_BuildingEntry->Type)
                            {
                                case BuildingType::Farm:
                                    l_FinalPosition     = (l_Lvl == 2 ? G3D::Vector3(1859.0985f, 155.4274f, 79.0399f) : G3D::Vector3(1855.8151f, 151.5068f, 78.4132f));
                                    l_FinalOrientation  = (l_Lvl == 2 ? 0.956567f : 0.857591f);
                                    break;
                                case BuildingType::Mine:
                                    l_FinalPosition     = (l_Lvl == 2 ? G3D::Vector3(1898.2411f,  89.8438f, 83.5268f) : G3D::Vector3(1898.6614f,  88.5848f, 83.5269f));
                                    l_FinalOrientation  = (l_Lvl == 2 ? 0.673824f : 0.555211f);
                                    break;
                                case BuildingType::Fishing:
                                    l_FinalPosition     = (l_Lvl == 2 ? G3D::Vector3(2010.6321f, 166.1842f, 83.5260f) : G3D::Vector3(2013.2568f, 166.8641f, 83.7605f));
                                    l_FinalOrientation  = (l_Lvl == 2 ? 3.756512f : 3.879798f);
                                    break;
                                case BuildingType::PetMenagerie:    ///< Only level 3 garrison
                                    l_FinalPosition     = G3D::Vector3(1909.9861f, 328.0322f, 88.9653f);
                                    l_FinalOrientation  = 5.132511f;
                                    break;
                            }
                        }
                        else        ///< FactionIndex::Horde
                        {
                            switch (l_BuildingEntry->Type)
                            {
                                case BuildingType::Farm:
                                    l_FinalPosition     = (l_Lvl == 2 ? G3D::Vector3(5433.7769f, 4574.0503f, 136.0184f) : G3D::Vector3(5431.2168f, 4573.4658f, 136.1743f));
                                    l_FinalOrientation  = (l_Lvl == 2 ? 5.840217f : 5.998074f);
                                    break;
                                case BuildingType::Mine:
                                    l_FinalPosition     = (l_Lvl == 2 ? G3D::Vector3(5476.3716f, 4446.7773f, 144.4951f) : G3D::Vector3(5474.9707f, 4443.2588f, 144.6435f));
                                    l_FinalOrientation  = (l_Lvl == 2 ? 0.955031f : 0.99013f);
                                    break;
                                case BuildingType::Fishing:
                                    l_FinalPosition     = (l_Lvl == 2 ? G3D::Vector3(5476.4160f, 4613.2881f, 134.4511f) : G3D::Vector3(5478.0010f, 4614.4854f, 134.4501f));
                                    l_FinalOrientation  = (l_Lvl == 2 ? 5.023405f : 5.306918f);
                                    break;
                                case BuildingType::PetMenagerie:    ///< Only level 3 garrison
                                    l_FinalPosition     = G3D::Vector3(5620.6782f, 4649.7178f, 142.2780f);
                                    l_FinalOrientation  = 4.230919f;
                                    break;
                            }
                        }
                    }
                    else
                    {
                        /// For this part we use an matrix to transform plot coord in, order to get the position without the rotation
                        /// Once we have the "non rotated" position, we compute activation game object position in a 2 dimensional system
                        /// And after we reapply the rotation on coords to transform and get the correct final position
                        G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
                        l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), -l_PlotInfo.O);

                        /// transform plot coord
                        G3D::Vector3 l_NonRotatedPosition = l_Mat * G3D::Vector3(l_PlotInfo.X, l_PlotInfo.Y, l_PlotInfo.Z);

                        GameObjectDisplayInfoEntry const* l_GobDispInfo = sGameObjectDisplayInfoStore.LookupEntry(l_Gob->GetDisplayId());

                        /// Work with plot AABB
                        if (l_GobDispInfo)
                        {
                            /// Get AABB on X axis
                            float l_XAxisSize = fabs(l_GobDispInfo->maxX - l_GobDispInfo->minX) * l_Gob->GetFloatValue(OBJECT_FIELD_SCALE);

                            /// We use a "diminish return" on box size for big plots
                            l_NonRotatedPosition.x += l_XAxisSize * (gGarrisonBuildingPlotAABBDiminishReturnFactor[GetPlotType(p_PlotInstanceID) + (GetGarrisonFactionIndex() * PlotTypes::Max)] / l_XAxisSize);
                        }

                        l_Mat = G3D::Matrix3::identity();
                        l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), l_PlotInfo.O);

                        /// Reapply the rotation on coords
                        l_FinalPosition = l_Mat * l_NonRotatedPosition;
                    }

                    uint32 l_AnimProgress   = 0;
                    uint32 l_Health         = 255;

                    GameObject* l_ActivationGob = m_Owner->SummonGameObject(gGarrisonBuildingActivationGameObject[GetGarrisonFactionIndex()], l_FinalPosition.x, l_FinalPosition.y, l_FinalPosition.z, l_FinalOrientation, 0, 0, 0, 0, 0, 0, 0, l_AnimProgress, l_Health);

                    if (l_ActivationGob)
                    {
                        m_PlotsActivateGob[p_PlotInstanceID] = l_ActivationGob->GetGUID();

                        l_ActivationGob->SendGameObjectActivateAnimKit(1696);
                    }
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Update garrison stats
    void Manager::UpdateStats()
    {
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Update building
    void Manager::UpdateBuildings()
    {
        /// Update building in construction
        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        {
            GarrisonBuilding* l_Building = &m_Buildings[l_I];

            if (!l_Building->Active && !l_Building->BuiltNotified && time(0) > l_Building->TimeBuiltEnd)
            {
                l_Building->BuiltNotified = true;

                /// Nothing more needed, client auto deduce notification
                UpdatePlot(l_Building->PlotInstanceID);
            }
        }
    }

    /// Update followers
    void Manager::UpdateFollowers()
    {
        /// Update follower activation count
        if (m_NumFollowerActivation < Globals::FollowerActivationMaxStack && (time(0) - m_NumFollowerActivationRegenTimestamp) > DAY)
        {
            m_NumFollowerActivation++;
            m_NumFollowerActivationRegenTimestamp = time(0);

            WorldPacket l_Data(SMSG_GARRISON_UPDATE_FOLLOWER_ACTIVATION_COUNT, 4);
            l_Data << uint32(GetNumFollowerActivationsRemaining());

            m_Owner->SendDirectMessage(&l_Data);
        }
    }

    /// Update cache
    void Manager::UpdateCache()
    {
        if (!m_Owner->IsInGarrison())
            return;

        Interfaces::GarrisonSite* l_GarrisonScript = GetGarrisonScript();

        /// Update garrison cache
        if (m_CacheGameObjectGUID && HashMapHolder<GameObject>::Find(m_CacheGameObjectGUID) == nullptr)
            m_CacheGameObjectGUID = 0;

        if (!l_GarrisonScript || !l_GarrisonScript->CanUseGarrisonCache(m_Owner))
        {
            if (m_CacheGameObjectGUID != 0)
            {
                GameObject* l_Cache = HashMapHolder<GameObject>::Find(m_CacheGameObjectGUID);

                if (l_Cache)
                {
                    l_Cache->DestroyForNearbyPlayers();
                    l_Cache->AddObjectToRemoveList();
                }

                m_CacheGameObjectGUID = 0;
            }

            return;
        }

        uint32 l_NumRessourceGenerated = std::min((uint32)((time(0) - m_CacheLastUsage) / Globals::CacheTokenGenerateTime), (uint32)Globals::CacheMaxToken);

        if (!m_CacheGameObjectGUID)
        {
            m_CacheLastTokenAmount = l_NumRessourceGenerated;
            m_Owner->SendUpdateWorldState(WorldStates::CacheNumToken, l_NumRessourceGenerated);

            if (l_NumRessourceGenerated >= Globals::CacheMinToken)
            {
                /// Get display ID
                uint32 l_DisplayIDOffset    = l_NumRessourceGenerated == Globals::CacheMaxToken ? 2 : ((l_NumRessourceGenerated > Globals::CacheHeftyToken) ? 1 : 0);
                const uint32 & l_DisplayID  = gGarrisonCacheGameObjectID[(GetGarrisonFactionIndex() * 3) + l_DisplayIDOffset];

                /// Destroy old cache if exist
                GameObject * l_Cache = HashMapHolder<GameObject>::Find(m_CacheGameObjectGUID);

                if (l_Cache)
                {
                    l_Cache->DestroyForNearbyPlayers();
                    l_Cache->AddObjectToRemoveList();
                }

                m_CacheGameObjectGUID = 0;

                /// Create new one
                if (m_Owner->IsInGarrison())
                {
                    /// Extract new location
                    const GarrisonCacheInfoLocation& l_Location = gGarrisonCacheInfoLocation[(GetGarrisonFactionIndex() * Globals::MaxLevel) + (m_GarrisonLevel - 1)];
                    l_Cache = m_Owner->SummonGameObject(l_DisplayID, l_Location.X, l_Location.Y, l_Location.Z, l_Location.O, 0, 0, 0, 0, 0);

                    if (l_Cache)
                        m_CacheGameObjectGUID = l_Cache->GetGUID();
                }

            }
        }
    
        if (m_CacheGameObjectGUID && l_NumRessourceGenerated < Globals::CacheMinToken)
        {
            GameObject* l_Cache = HashMapHolder<GameObject>::Find(m_CacheGameObjectGUID);

            if (l_Cache)
            {
                l_Cache->DestroyForNearbyPlayers();
                l_Cache->AddObjectToRemoveList();
            }

            m_CacheGameObjectGUID = 0;
        }
    }

    /// Update mission distribution
    void Manager::UpdateMissionDistribution(bool p_Force /* = false */, uint32 p_ForcedCount /* = 0 */)
    {
        /// Do ramdom mission distribution
        if (p_Force || ((time(nullptr) - m_MissionDistributionLastUpdate) > Globals::MissionDistributionInterval))
        {
            /// Random, no detail about how blizzard do
            uint32 l_MaxMissionCount         = p_ForcedCount ? p_ForcedCount : ceil(GetTotalFollowerCount(FollowerType::NPC) * GARRISON_MISSION_DISTRIB_FOLLOWER_COEFF);
            uint32 l_CurrentAvailableMission = 0;

            std::for_each(m_Missions.begin(), m_Missions.end(), [&l_CurrentAvailableMission](const GarrisonMission& p_Mission) -> void
            {
                if (p_Mission.State == MissionStates::Available && (p_Mission.OfferTime + p_Mission.OfferMaxDuration) > time(0))
                    l_CurrentAvailableMission++;
            });

            /// Temp cancel changes due to 79f3871dfffa4181fdcc739285da84c7fffbefa4
            /*if (l_CurrentAvailableMission < l_MaxMissionCount)
            {
                uint32 l_AVGFollowerLevel = 0;
                uint32 l_MaxFollowerItemLevel = 600;
                uint32 l_FollowerNpcCount = 0;

                std::for_each(m_Followers.begin(), m_Followers.end(), [&l_AVGFollowerLevel, &l_MaxFollowerItemLevel, &l_FollowerNpcCount](const GarrisonFollower & p_Follower) -> void
                {
                    if (!p_Follower.IsNPC())
                        return;

                    l_AVGFollowerLevel      += p_Follower.Level;
                    l_MaxFollowerItemLevel  = std::max(l_MaxFollowerItemLevel, (uint32)((p_Follower.ItemLevelArmor + p_Follower.ItemLevelWeapon) / 2));

                    l_FollowerNpcCount++;
                });

                if (l_FollowerNpcCount == 0)
                    l_FollowerNpcCount = 1;

                l_AVGFollowerLevel /= l_FollowerNpcCount;

                std::vector<const GarrMissionEntry*> l_Candidates;

                for (uint32 l_I = 0; l_I < sGarrMissionStore.GetNumRows(); ++l_I)
                {
                    GarrMissionEntry const* l_Entry = sGarrMissionStore.LookupEntry(l_I);

                    if (!l_Entry)
                        continue;

                    uint32 l_Count = std::count_if(m_Missions.begin(), m_Missions.end(), [l_Entry](const GarrisonMission & p_Mission)
                    {
                        return p_Mission.MissionID == l_Entry->MissionRecID;
                    });

                    if (l_Count)
                        continue;

                    if (l_Entry->RequiredFollowersCount > m_Followers.size())
                        continue;

                    if (l_Entry->Duration <= 10)
                        continue;

                    if (l_Entry->RequiredFollowersCount > Globals::MaxFollowerPerMission)
                        continue;

                    /// If player has Tavern lvl 3, he'll have 50% chance to get a treasure hunter type mission
                    if (l_Entry->MissionType == MissionType::Treasure)
                    {
                        if (!GetBuildingLevel(GetBuildingWithType(BuildingType::Inn)) == 3)
                            continue;
                        else if (urand(0, 1))
                            continue;
                    }

                    /// Faction speific mission (No generic way to handle it)
                    switch (GetGarrisonFactionIndex())
                    {
                        case FactionIndex::Alliance:
                            switch (l_Entry->MissionRecID)
                            {
                                case 2: ///< Gronnlings Abound
                                case 7: ///< Stonefury Rescue
                                    continue;
                            }
                            break;

                        case FactionIndex::Horde:
                            switch (l_Entry->MissionRecID)
                            {
                                case 66: ///< Killing the Corrupted
                                case 91: ///< Rangari Rescue
                                    continue;
                            }
                            break;
                    }

                    uint32 l_RewardCount = 0;
                    for (uint32 l_RewardIT = 0; l_RewardIT < sGarrMissionRewardStore.GetNumRows(); ++l_RewardIT)
                    {
                        GarrMissionRewardEntry const* l_RewardEntry = sGarrMissionRewardStore.LookupEntry(l_RewardIT);

                        if (!l_RewardEntry)
                            continue;

                        if (l_RewardEntry->MissionID != l_Entry->MissionRecID)
                            continue;

                        /// Elemental Rune & Abrogator Stone - Legendary Questline  NYI
                        if (l_RewardEntry->ItemID == 115510 || l_RewardEntry->ItemID == 115280)
                        {
                            l_RewardCount = 0;
                            break;
                        }

                        /// Special case for XP item, if the owner is already at max level he doesn't need this item anymore
                        if (m_Owner->getLevel() == MAX_LEVEL && l_RewardEntry->ItemID == 120205)
                        {
                            l_RewardCount = 0;
                            break;
                        }

                        /// Follower case
                        if (l_RewardEntry->ItemID)
                        {
                            ItemTemplate const* l_Template = sObjectMgr->GetItemTemplate(l_RewardEntry->ItemID);

                            if (l_Template && l_Template->Spells[0].SpellId != 0)
                            {
                                SpellInfo const* l_Spell = sSpellMgr->GetSpellInfo(l_Template->Spells[0].SpellId);

                                if (l_Spell && l_Spell->Effects[0].Effect == SPELL_EFFECT_ADD_GARRISON_FOLLOWER)
                                {
                                    if (GetFollower(l_Spell->Effects[0].MiscValue) != nullptr)
                                    {
                                        l_RewardCount = 0;
                                        break;
                                    }
                                }
                            }
                        }

                        ++l_RewardCount;
                    }

                    /// All missions should have a reward
                    if (!l_RewardCount)
                        continue;

                    /// Max Level cap : 2
                    if (l_Entry->RequiredLevel > (int32)(l_AVGFollowerLevel + 2))
                        continue;

                    if (l_Entry->RequiredItemLevel > (int32)l_MaxFollowerItemLevel)
                        continue;

                    /// Ships NYI
                    if (l_Entry->FollowerType != MS::Garrison::FollowerType::NPC)
                        continue;

                    /// We are getting too many rare missions compared to retail
                    if (l_Entry->Flags & MS::Garrison::MissionFlags::Rare)
                    {
                        if (urand(0, 100) >= 15)
                            continue;
                    }

                    l_Candidates.push_back(l_Entry);
                }

                uint32 l_ShuffleCount = std::rand() % 4;

                for (uint32 l_I = 0; l_I < l_ShuffleCount; ++l_I)
                    std::random_shuffle(l_Candidates.begin(), l_Candidates.end());

                int32 l_MissionToAddCount = (int32)l_MaxMissionCount - (int32)l_CurrentAvailableMission;

                if (l_MissionToAddCount > 0)
                {
                    l_MissionToAddCount = std::min(l_MissionToAddCount, (int32)l_Candidates.size());

                    for (int32 l_I = 0; l_I < l_MissionToAddCount; ++l_I)
                        AddMission(l_Candidates[l_I]->MissionRecID);
                }
            }
            */

            if (l_CurrentAvailableMission < l_MaxMissionCount)
            {
                uint32 l_MaxFollowerLevel = 90;
                uint32 l_MaxFollowerItemLevel = 600;

                std::for_each(m_Followers.begin(), m_Followers.end(), [&l_MaxFollowerLevel, &l_MaxFollowerItemLevel](const GarrisonFollower & p_Follower) -> void
                {
                    if (!p_Follower.IsNPC())
                        return;

                    l_MaxFollowerLevel      = std::max(l_MaxFollowerLevel, (uint32)p_Follower.Level);
                    l_MaxFollowerItemLevel  = std::max(l_MaxFollowerItemLevel, (uint32)((p_Follower.ItemLevelArmor + p_Follower.ItemLevelWeapon) / 2));
                });

                std::vector<const GarrMissionEntry*> l_Candidates;

                for (uint32 l_I = 0; l_I < sGarrMissionStore.GetNumRows(); ++l_I)
                {
                    GarrMissionEntry const* l_Entry = sGarrMissionStore.LookupEntry(l_I);

                    if (!l_Entry)
                        continue;

                    uint32 l_Count = (uint32)std::count_if(m_Missions.begin(), m_Missions.end(), [l_Entry](const GarrisonMission & p_Mission)
                    {
                        return p_Mission.MissionID == l_Entry->MissionRecID;
                    });

                    if (l_Count)
                        continue;

                    if (l_Entry->RequiredFollowersCount > m_Followers.size())
                        continue;

                    if (l_Entry->Duration <= 10)
                        continue;

                    if (l_Entry->RequiredFollowersCount > Globals::MaxFollowerPerMission)
                        continue;

                    /// If player has Tavern lvl 3, he'll have 50% chance to get a treasure hunter type mission
                    if (l_Entry->MissionType == MissionType::Treasure)
                    {
                        if (GetBuildingLevel(GetBuildingWithType(BuildingType::Inn)) != 3)
                            continue;
                        else if (urand(0, 1))
                            continue;
                    }

                    uint32 l_RewardCount = 0;
                    for (uint32 l_RewardIT = 0; l_RewardIT < sGarrMissionRewardStore.GetNumRows(); ++l_RewardIT)
                    {
                        GarrMissionRewardEntry const* l_RewardEntry = sGarrMissionRewardStore.LookupEntry(l_RewardIT);

                        if (!l_RewardEntry)
                            continue;

                        if (l_RewardEntry->MissionID != l_Entry->MissionRecID)
                            continue;

                        /// Elemental Rune & Abrogator Stone - Legendary Questline  NYI
                        if (l_RewardEntry->ItemID == 115510 || l_RewardEntry->ItemID == 115280)
                        {
                            l_RewardCount = 0;
                            break;
                        }

                        ++ l_RewardCount;
                    }

                    /// All missions should have a reward
                    if (!l_RewardCount)
                        continue;

                    /// Max Level cap : 2
                    if (l_Entry->RequiredLevel > (int32)(l_MaxFollowerLevel + 2))
                        continue;

                    if (l_Entry->RequiredItemLevel > (int32)l_MaxFollowerItemLevel)
                        continue;

                    /// Ships NYI
                    if (l_Entry->FollowerType != MS::Garrison::FollowerType::NPC)
                        continue;

                    /// We are getting too many rare missions compared to retail
                    if (l_Entry->Flags & MS::Garrison::MissionFlags::Rare)
                    {
                        if (urand(0, 100) >= 15)
                            continue;
                    }

                    l_Candidates.push_back(l_Entry);
                }

                uint32 l_ShuffleCount = std::rand() % 4;

                for (uint32 l_I = 0; l_I < l_ShuffleCount; ++l_I)
                    std::random_shuffle(l_Candidates.begin(), l_Candidates.end());

                int32 l_MissionToAddCount = (int32)l_MaxMissionCount - (int32)l_CurrentAvailableMission;

                if (l_MissionToAddCount > 0)
                {
                    l_MissionToAddCount = std::min(l_MissionToAddCount, (int32)l_Candidates.size());

                    for (int32 l_I = 0; l_I < l_MissionToAddCount; ++l_I)
                        AddMission(l_Candidates[l_I]->MissionRecID);
                }
            }
            m_MissionDistributionLastUpdate = time(0);
        }
    }

    /// Update garrison ability
    void Manager::UpdateGarrisonAbility()
    {
        if (!m_Owner->IsInWorld())
            return;

        uint32 l_AbilityOverrideSpellID = 0;

        switch (GetGarrisonFactionIndex())
        {
            case FactionIndex::Alliance:
                l_AbilityOverrideSpellID = GARRISON_SPELL_GARR_ABILITY_ALLIANCE_BASE;
                break;

            case FactionIndex::Horde:
                l_AbilityOverrideSpellID = GARRISON_SPELL_GARR_ABILITY_HORDE_BASE;
                break;

            case FactionIndex::Max:
            default:
                ASSERT(false);
                break;

        }

        bool l_Cond1 = m_Owner->IsInGarrison() || m_Owner->GetMapId() == Globals::BaseMap;
        bool l_Cond2 = GetGarrisonSiteLevelEntry()->Level == 1 ? (m_Owner->IsQuestRewarded(Quests::Alliance_BuildYourBarracks) || m_Owner->IsQuestRewarded(Quests::Horde_BuildYourBarracks)) : true;

        if (l_Cond1 && l_Cond2)
        {
            if (!m_Owner->HasAura(l_AbilityOverrideSpellID))
                m_Owner->AddAura(l_AbilityOverrideSpellID, m_Owner);
        }
        else
        {
            if (m_Owner->HasAura(l_AbilityOverrideSpellID))
                m_Owner->RemoveAura(l_AbilityOverrideSpellID);
        }
    }

    /// Update work order
    void Manager::UpdateWorkOrders()
    {
        if (!m_Owner->IsInGarrison())
            return;

        for (uint32 l_PlotI = 0; l_PlotI < m_Plots.size(); ++l_PlotI)
        {
            uint32 l_PlotInstanceID = m_Plots[l_PlotI].PlotInstanceID;

            if (m_PlotsWorkOrderGob[l_PlotInstanceID] == 0)
                continue;

            GameObject* l_WorkOrderGameObject = HashMapHolder<GameObject>::Find(m_PlotsWorkOrderGob[l_PlotInstanceID]);

            if (!l_WorkOrderGameObject)
                continue;

            std::vector<GarrisonWorkOrder*> l_PlotWorkOrder;

            for (uint32 l_OrderI = 0; l_OrderI < m_WorkOrders.size(); ++l_OrderI)
            {
                if (m_WorkOrders[l_OrderI].PlotInstanceID == l_PlotInstanceID)
                    l_PlotWorkOrder.push_back(&m_WorkOrders[l_OrderI]);
            }

            if (l_PlotWorkOrder.size() > 0)
            {
                CharShipmentEntry const* l_ShipmentEntry = sCharShipmentStore.LookupEntry(l_PlotWorkOrder[0]->ShipmentID);

                if (l_ShipmentEntry == nullptr)
                    continue;

                CharShipmentContainerEntry const* l_ShipmentContainerEntry = sCharShipmentContainerStore.LookupEntry(l_ShipmentEntry->ShipmentContainerID);

                if (l_ShipmentContainerEntry == nullptr)
                    continue;

                bool l_Complete = false;
                uint32 l_GobDisplayID = l_WorkOrderGameObject->GetGOInfo() ? l_WorkOrderGameObject->GetGOInfo()->displayId : 0;
                uint32 l_CurrentTimeStamp = time(0);
                uint32 l_ShipmentsSize = (uint32)l_PlotWorkOrder.size();

                for (uint32 l_OrderI = 0; l_OrderI < l_ShipmentsSize; ++l_OrderI)
                {
                    if (l_PlotWorkOrder[l_OrderI]->CompleteTime <= l_CurrentTimeStamp)
                        l_Complete = true;
                }

                if (l_ShipmentsSize < l_ShipmentContainerEntry->ShipmentAmountNeeded[0])
                    l_GobDisplayID = l_ShipmentContainerEntry->OverrideDisplayID1;
                else
                {
                    for (int8 l_Itr = 1; l_Itr >= 0; --l_Itr)
                    {
                        if (l_ShipmentsSize >= l_ShipmentContainerEntry->ShipmentAmountNeeded[l_Itr])
                            l_GobDisplayID = l_ShipmentContainerEntry->OverrideIfAmountMet[l_Itr];
                    }
                }

                if (l_Complete)
                {
                    l_WorkOrderGameObject->SetDisplayId(l_GobDisplayID);
                    l_WorkOrderGameObject->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_ACTIVATED);
                }
                else
                {
                    if (l_ShipmentContainerEntry->OverrideDisplayIfNotNull) ///< Shipyards related
                        l_WorkOrderGameObject->SetDisplayId(l_ShipmentContainerEntry->OverrideDisplayIfNotNull);
                    else
                    {
                        if (GameObjectTemplate const* l_GobTemplate = l_WorkOrderGameObject->GetGOInfo())
                        {
                            if (!l_GobTemplate->garrisonShipment.LargeAOI) ///< Shipyard ?
                                l_WorkOrderGameObject->SetDisplayId(l_GobTemplate->displayId);
                        }
                    }

                    l_WorkOrderGameObject->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_ACTIVATED);
                }
            }
            else
            {
                if (GameObjectTemplate const* l_GobTemplate = l_WorkOrderGameObject->GetGOInfo())
                {
                    if (!l_GobTemplate->garrisonShipment.LargeAOI) ///< Shipyard ?
                        l_WorkOrderGameObject->SetDisplayId(l_GobTemplate->displayId);
                }

                l_WorkOrderGameObject->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_ACTIVATED);
            }
        }
    }

    bool Manager::RenameFollower(uint32 p_DatabaseID, std::string p_FollowerName)
    {
        auto l_Itr = std::find_if(m_Followers.begin(), m_Followers.end(), [p_DatabaseID](GarrisonFollower l_Follower) -> bool
        {
            return l_Follower.DatabaseID == p_DatabaseID;
        });

        if (l_Itr == m_Followers.end())
            return false;

        if (!CanRenameFollowerType(l_Itr->GetEntry()->Type))
            return false;

        l_Itr->ShipName = p_FollowerName;
        l_Itr->SendFollowerUpdate(m_Owner);
        return true;
    }

    bool Manager::RemoveFollower(uint32 p_DatabaseID, bool p_Force /* = false */)
    {
        auto l_Itr = std::find_if(m_Followers.begin(), m_Followers.end(), [p_DatabaseID](GarrisonFollower l_Follower) -> bool
        {
            return l_Follower.DatabaseID == p_DatabaseID;
        });

        if (l_Itr == m_Followers.end())
            return false;

        /// Can never remove a follower on a mission
        if (l_Itr->CurrentMissionID)
            return false;

        if (!p_Force && (l_Itr->IsNPC() || (l_Itr->IsShip() && GetTotalFollowerCount(FollowerType::Ship) < GetFollowerSoftCap(FollowerType::Ship))))
            return false;

        WorldPacket l_Data(SMSG_GARRISON_FOLLOWER_DELETE_NOTIFY, 8 + 4 + 4);
        l_Data << uint64(p_DatabaseID);
        l_Data << uint32(PurchaseBuildingResults::Ok);  ///< Guessed
        l_Data << uint32(PurchaseBuildingResults::Ok);  ///< Guessed only checked if NPC is a ship -- GARRISON_SHIP_DECOMMISSION_SUCCEEDED
        SendPacketToOwner(&l_Data);

        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_FOLLOWER);
        l_Stmt->setUInt32(0, p_DatabaseID);
        CharacterDatabase.AsyncQuery(l_Stmt);

        m_Followers.erase(l_Itr);

        return true;
    }

    uint32 Manager::GetTotalFollowerCount(uint32 p_Type)
    {
        return (uint32)std::count_if(m_Followers.begin(), m_Followers.end(), [p_Type](GarrisonFollower l_Follower) -> bool
        {
            GarrFollowerEntry const* l_Entry = l_Follower.GetEntry();
            return l_Entry && l_Entry->Type == p_Type;
        });
    }

    void Manager::SendPacketToOwner(WorldPacket* p_Data)
    {
        m_Owner->SendDirectMessage(p_Data);
    }

    bool Manager::HasCrewAbility(GarrisonFollower const& p_Follower) const
    {
        for (auto l_Ability : p_Follower.Abilities)
        {
            GarrAbilityEntry const* l_GarrAbility = sGarrAbilityStore.LookupEntry(l_Ability);
            if (!l_GarrAbility)
                continue;

            if (l_GarrAbility->AbilityType == 1 || l_GarrAbility->AbilityType == 9 || l_GarrAbility->AbilityType == 3 || l_GarrAbility->AbilityType == 5)
                return true;
        }
        return false;
    }

    uint32 Manager::GenerateCrewAbilityIdForShip(GarrisonFollower const& p_Follower)
    {
        if (!p_Follower.IsShip())
            return 0;

        if (HasCrewAbility(p_Follower))
            return 0;

        uint32 l_Type = GetGarrisonFactionIndex() == FactionIndex::Alliance ? 9 : 5;

        std::vector<uint32> l_PossibleEntiers;

        for (uint32 l_ID = 0; l_ID < sGarrAbilityStore.GetNumRows(); ++l_ID)
        {
            GarrAbilityEntry const* l_Entry = sGarrAbilityStore.LookupEntry(l_ID);

            if (!l_Entry)
                continue;

            /// Neutral == 1
            if ((l_Entry->AbilityType == l_Type || l_Entry->AbilityType == 1) && l_Entry->FollowerType == FollowerType::Ship)
                l_PossibleEntiers.push_back(l_ID);
        }

        if (!l_PossibleEntiers.size())
            return 0;

        return l_PossibleEntiers[urand(0, l_PossibleEntiers.size() - 1)];
    }

    /// TODO: Only class specific - not fully random
    uint32 Manager::GenerateRandomAbility()
    {
        std::vector<uint32> l_PossibleEntiers;

        for (uint32 l_ID = 0; l_ID < sGarrAbilityStore.GetNumRows(); ++l_ID)
        {
            GarrAbilityEntry const* l_Entry = sGarrAbilityStore.LookupEntry(l_ID);

            if (!l_Entry)
                continue;

            if (l_Entry->FollowerType != FollowerType::NPC)
                continue;

            if (l_Entry->AbilityType != 0)
                continue;

            l_PossibleEntiers.push_back(l_Entry->ID);
        }

        if (!l_PossibleEntiers.size())
            return 0;

        return l_PossibleEntiers[urand(0, l_PossibleEntiers.size() - 1)];
    }

    /*uint32 Manager::GenerateRandomAbility(GarrisonFollower* p_Follower)
    {
        std::vector<uint32> l_PossibleEntiers;

        for (uint32 l_ID = 0; l_ID < sGarrFollowerXAbilityStore.GetNumRows(); ++l_ID)
        {
            GarrFollowerXAbilityEntry const* l_FollowerXAbilityEntry = sGarrFollowerXAbilityStore.LookupEntry(l_ID);

            if (!l_FollowerXAbilityEntry || l_FollowerXAbilityEntry->FactionIndex != GetGarrisonFactionIndex() ||l_FollowerXAbilityEntry->FollowerID != p_Follower->FollowerID)
                continue;

            GarrAbilityEntry const* l_AbilityEntry = sGarrAbilityStore.LookupEntry(l_FollowerXAbilityEntry->AbilityID);

            if (!l_AbilityEntry)
                continue;

            if (l_AbilityEntry->FollowerType != FollowerType::NPC)
                continue;

            if (l_AbilityEntry->AbilityType != 0)
                continue;

            if (std::find(p_Follower->Abilities.begin(), p_Follower->Abilities.end(), l_AbilityEntry->ID) != p_Follower->Abilities.end())
                continue;

            l_PossibleEntiers.push_back(l_AbilityEntry->ID);
        }

        if (!l_PossibleEntiers.size())
            return 0;

        return l_PossibleEntiers[urand(0, l_PossibleEntiers.size() - 1)];
    }*/

    uint32 Manager::GenerateRandomTrait(uint32 p_Type, std::vector<uint32> const& p_KnownAbilities)
{
        std::vector<uint32> l_PossibleEntiers;
        std::vector<uint32> l_KnownTraits;

        uint32 l_MyFactionType = GetGarrisonFactionIndex() == FactionIndex::Alliance ? 5 : 9;
        uint32 l_OtherFactionType = GetGarrisonFactionIndex() == FactionIndex::Alliance ? 9 : 5;

        if (p_Type != FollowerType::Ship)
        {
            for (auto l_Ability : p_KnownAbilities)
            {
                if (GarrAbilityEntry const* l_Entry = sGarrAbilityStore.LookupEntry(l_Ability))
                {
                    l_KnownTraits.push_back(l_Entry->AbilityType);
                }
            }

            l_KnownTraits.push_back(0);                 ///< Abilities
            l_KnownTraits.push_back(2);                 ///< Abilities
        }
        else if (p_Type == FollowerType::Ship)
        {
            /// Ships do not gain crew
            l_KnownTraits.push_back(l_MyFactionType);
            l_KnownTraits.push_back(1);                 ///< Neutral Crews
            l_KnownTraits.push_back(3);                 ///< Ship Specific crews
            l_KnownTraits.push_back(51);                ///< Ship Abilities
        }

        l_KnownTraits.push_back(l_OtherFactionType);


        for (uint32 l_ID = 0; l_ID < sGarrAbilityStore.GetNumRows(); ++l_ID)
        {
            GarrAbilityEntry const* l_Entry = sGarrAbilityStore.LookupEntry(l_ID);

            if (!l_Entry)
                continue;

            if (l_Entry->FollowerType != p_Type)
                continue;

            if (std::find(l_KnownTraits.begin(), l_KnownTraits.end(), l_Entry->AbilityType) != l_KnownTraits.end())
                continue;

            if (std::find(p_KnownAbilities.begin(), p_KnownAbilities.end(), l_ID) != p_KnownAbilities.end())
                continue;

            /// Test Entiers
            if (l_ID == 300 || l_ID == 299 || l_ID == 257)
                continue;

            l_PossibleEntiers.push_back(l_Entry->ID);
        }

        if (!l_PossibleEntiers.size())
            return 0;

        return l_PossibleEntiers[urand(0, l_PossibleEntiers.size() - 1)];
    }

    void Manager::GenerateFollowerAbilities(GarrisonFollower& p_Follower, bool p_Reset /* = true */, bool /*p_Abilities*/ /* = true */, bool /*p_Traits*/ /* = true */, bool p_Update /* = false */)
    {
        if (p_Reset)
            p_Follower.Abilities.clear();

        for (uint32 l_I = 0; l_I < sGarrFollowerXAbilityStore.GetNumRows(); ++l_I)
        {
            GarrFollowerXAbilityEntry const* l_Entry = sGarrFollowerXAbilityStore.LookupEntry(l_I);

            if (l_Entry && l_Entry->FollowerID == p_Follower.FollowerID && sGarrAbilityStore.LookupEntry(l_Entry->AbilityID) && l_Entry->FactionIndex == GetGarrisonFactionIndex() && std::find(p_Follower.Abilities.begin(), p_Follower.Abilities.end(), l_Entry->AbilityID) == p_Follower.Abilities.end())
                p_Follower.Abilities.push_back(l_Entry->AbilityID);
        }

        if (p_Follower.IsShip())
        {
            if (uint32 l_AbillityId = GenerateCrewAbilityIdForShip(p_Follower))
                p_Follower.Abilities.push_back(l_AbillityId);

            uint32 l_AbilityCount = p_Follower.Quality - ITEM_QUALITY_UNCOMMON; ///< rare 1, epic 2

            while ((1 /* ship type */ + 1 /* crew */ + l_AbilityCount) > p_Follower.Abilities.size())
            {
                if (uint32 l_NewAbility = GenerateRandomTrait(FollowerType::Ship, p_Follower.Abilities))
                    p_Follower.Abilities.push_back(l_NewAbility);
            }
        }
        else if (p_Follower.IsNPC())
        {
            if (p_Follower.Quality >= ITEM_QUALITY_EPIC)
            {
                if ((CountFollowerAbilitiesByType(p_Follower.DatabaseID, 0) + CountFollowerAbilitiesByType(p_Follower.DatabaseID, 2)) < 2)
                {
                    if (uint32 l_NewAbility = GenerateRandomAbility())
                        p_Follower.Abilities.push_back(l_NewAbility);
                }
            }

            uint32 l_TraitCount = p_Follower.Abilities.size() - CountFollowerAbilitiesByType(p_Follower.DatabaseID, 0) - CountFollowerAbilitiesByType(p_Follower.DatabaseID, 2);

            while (l_TraitCount < (p_Follower.Quality - ITEM_QUALITY_NORMAL))
            {
                if (uint32 l_NewAbility = GenerateRandomTrait(FollowerType::NPC, p_Follower.Abilities))
                {
                    p_Follower.Abilities.push_back(l_NewAbility);
                }

                l_TraitCount = p_Follower.Abilities.size() - CountFollowerAbilitiesByType(p_Follower.DatabaseID, 0) - CountFollowerAbilitiesByType(p_Follower.DatabaseID, 2);
            }
        }

        if (p_Update)
            p_Follower.SendFollowerUpdate(m_Owner);
    }

    void Manager::GenerateFollowerAbilities(uint32 p_FollowerID, bool p_Reset /* = true */, bool p_Abilities /* = true */, bool p_Traits /* = true */, bool p_Update /* = false */)
    {
        auto l_Itr = std::find_if(m_Followers.begin(), m_Followers.end(), [p_FollowerID](GarrisonFollower p_FollowerEntry) -> bool
        {
            return p_FollowerEntry.DatabaseID == p_FollowerID;
        });

        if (l_Itr == m_Followers.end())
            return;

        return GenerateFollowerAbilities(*l_Itr, p_Reset, p_Abilities, p_Traits, p_Update);
    }

    std::list<std::string> Manager::GenerateFollowerTextList(uint32 l_Type)
    {
        std::list<std::string> l_List;

        for_each(m_Followers.begin(), m_Followers.end(), [this, l_Type, &l_List](GarrisonFollower p_Follower) -> void
        {
            auto l_Entry = p_Follower.GetEntry();

            if (!l_Entry)
                return;

            if (l_Type)
            {
                if (l_Entry->Type != l_Type)
                    return;
            }

            std::stringstream l_String;
            l_String << p_Follower.DatabaseID << " " << p_Follower.FollowerID << " " << p_Follower.GetRealName(GetGarrisonFactionIndex());

            l_List.push_back(l_String.str());
        });

        return l_List;
    }

    bool Manager::LevelUpFollower(uint32 p_DatabaseID)
    {
        auto l_Iter = std::find_if(m_Followers.begin(), m_Followers.end(), [p_DatabaseID](GarrisonFollower p_Follower) -> bool
        {
            return p_Follower.DatabaseID == p_DatabaseID;
        });

        if (l_Iter == m_Followers.end())
            return false;

        GarrFollowerEntry const* l_Entry = l_Iter->GetEntry();
        if (!l_Entry)
            return false;

        if (!l_Iter->CanXP())
            return false;

        return l_Iter->EarnXP(l_Iter->GetRequiredLevelUpXP() - l_Iter->XP, m_Owner);
    }

    uint32 Manager::CountFollowerAbilitiesByType(uint32 p_FollowerID, uint32 p_Type) const
    {
        auto l_Iter = std::find_if(m_Followers.begin(), m_Followers.end(), [p_FollowerID](GarrisonFollower P_RefFollower) -> bool
        {
            return P_RefFollower.DatabaseID == p_FollowerID;
        });

        if (l_Iter == m_Followers.end())
            return 0;

        return (uint32)std::count_if(l_Iter->Abilities.begin(), l_Iter->Abilities.end(), [p_Type](uint32 p_Ability) -> bool
        {
            auto l_Ability = sGarrAbilityStore.LookupEntry(p_Ability);
            return l_Ability ? l_Ability->AbilityType == p_Type : false;
        });
    }

    SpellCastResult Manager::CanLearnTrait(uint32 p_FollowerID, uint32 p_Slot, SpellInfo const* p_SpellInfo, uint32 p_EffIndex) const
    {
        auto l_Iter = std::find_if(m_Followers.begin(), m_Followers.end(), [p_FollowerID](GarrisonFollower P_RefFollower) -> bool
        {
            return P_RefFollower.FollowerID == p_FollowerID;
        });

        if (l_Iter == m_Followers.end())
            return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

        for (int l_Index = 0; l_Index < (int)l_Iter->Abilities.size(); ++l_Index)
        {
            if (l_Iter->Abilities[l_Index] == p_Slot)
                break;

            if (l_Index == (l_Iter->Abilities.size() - 1)) ///< Comparison of integers of different signs: 'int' and 'unsigned long'
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
        }

        if (p_SpellInfo->Effects[p_EffIndex].MiscValueB == FollowerType::Ship && !l_Iter->IsShip())
            return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

        if ((p_SpellInfo->Effects[p_EffIndex].MiscValueB == FollowerType::NPC || p_SpellInfo->Effects[p_EffIndex].MiscValueB == 0)  && !l_Iter->IsNPC())
            return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

        GarrAbilityEntry const* l_Ability = sGarrAbilityStore.LookupEntry(p_Slot);

        if (!l_Ability)
            return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

        /// Cannot remove ships or ship types
        if (l_Iter->IsShip() && (l_Ability->AbilityType == 51 || l_Ability->AbilityType == 9 || l_Ability->AbilityType == 5 || l_Ability->AbilityType == 1 || l_Ability->AbilityType == 3))
            return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

        /// Cannot cast on abilities
        if (l_Iter->IsNPC() && (l_Ability->AbilityType == 0 || l_Ability->AbilityType == 2))
            return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

        if (std::find(l_Iter->Abilities.begin(), l_Iter->Abilities.end(), p_SpellInfo->Effects[p_EffIndex].MiscValue) != l_Iter->Abilities.end())
            return SpellCastResult::SPELL_FAILED_BAD_TARGETS;

        return SpellCastResult::SPELL_CAST_OK;
    }

    void Manager::LearnFollowerTrait(uint32 p_FollowerID, uint32 p_Slot, SpellInfo const* p_SpellInfo, uint32 p_EffIndex)
    {
        auto l_Iter = std::find_if(m_Followers.begin(), m_Followers.end(), [p_FollowerID](GarrisonFollower P_RefFollower) -> bool
        {
            return P_RefFollower.FollowerID == p_FollowerID;
        });

        if (l_Iter == m_Followers.end())
            return;

        int l_Index = 0;
        for (; l_Index < (int)l_Iter->Abilities.size(); ++l_Index)
        {
            if (l_Iter->Abilities[l_Index] == p_Slot)
                break;

            if (l_Index == (l_Iter->Abilities.size() - 1)) ///< Comparison of integers of different signs: 'int' and 'unsigned long'
                return;
        }

        l_Iter->Abilities[l_Index] = p_SpellInfo->Effects[p_EffIndex].MiscValue;
        l_Iter->SendFollowerUpdate(m_Owner);
    }

    GarrisonFollower Manager::GenerateNewFollowerEntity(uint32 p_FollowerID)
    {
        GarrisonFollower l_Follower;

        if (GetFollower(p_FollowerID) != nullptr)
            return *GetFollower(p_FollowerID);

        GarrFollowerEntry const* l_FollowerEntry = sGarrFollowerStore.LookupEntry(p_FollowerID);

        if (l_FollowerEntry == nullptr)
            return l_Follower;

        l_Follower.DatabaseID        = 0;
        l_Follower.FollowerID        = l_FollowerEntry->ID;
        l_Follower.Level             = l_FollowerEntry->Level;
        l_Follower.XP                = 0;
        l_Follower.Quality           = l_FollowerEntry->Quality;
        l_Follower.ItemLevelArmor    = l_FollowerEntry->ItemLevelArmor;
        l_Follower.ItemLevelWeapon   = l_FollowerEntry->ItemLevelWeapon;
        l_Follower.CurrentBuildingID = 0;
        l_Follower.CurrentMissionID  = 0;
        l_Follower.Flags             = 0;

        GenerateFollowerAbilities(l_Follower, true, true, true, true);

        return l_Follower;
    }

    uint32 Manager::GetFollowerSoftCap(uint32 p_FollowerType) const
    {
        GarrFollowerTypeEntry const* l_TypeEntry = sGarrFollowerTypeStore.LookupEntry(p_FollowerType);

        if (!l_TypeEntry)
            return 0;

        uint32 l_Cap = l_TypeEntry->SoftCap;
        GarrBuildingEntry const* l_GarrBuildingEntry = nullptr;

        auto l_Building = std::find_if(m_Buildings.begin(), m_Buildings.end(), [&l_GarrBuildingEntry, l_TypeEntry](GarrisonBuilding l_Building) -> bool
        {
            l_GarrBuildingEntry = sGarrBuildingStore.LookupEntry(l_Building.BuildingID);
            return l_GarrBuildingEntry ? l_GarrBuildingEntry->Type == l_TypeEntry->SoftCapBuildingIncreaseID : false;
        });

        if (l_Building != m_Buildings.end())
            l_Cap += l_GarrBuildingEntry->BonusAmount;

        return l_Cap;
    }

    bool Manager::CanMissionBeStartedAfterSoftCap(uint32 p_FollowerType) const
    {
        GarrFollowerTypeEntry const* l_Entry = sGarrFollowerTypeStore.LookupEntry(p_FollowerType);
        return l_Entry ? l_Entry->Flags & FollowerTypeFlags::CanStartMissionPastSoftCap : false;
    }

    bool Manager::CanRenameFollowerType(uint32 p_FollowerType) const
    {
        GarrFollowerTypeEntry const* l_Entry = sGarrFollowerTypeStore.LookupEntry(p_FollowerType);
        return l_Entry ? l_Entry->Flags & FollowerTypeFlags::CanRenameFollower : false;
    }

    uint32 Manager::GetMaxFollowerItemLevel(uint32 p_FollowerType) const
    {
        GarrFollowerTypeEntry const* l_Entry = sGarrFollowerTypeStore.LookupEntry(p_FollowerType);
        return l_Entry ? (l_Entry->MaxItemLevel ? l_Entry->MaxItemLevel : Globals::DefaultFollowerItemLevel) : Globals::DefaultFollowerItemLevel;
    }

    bool Manager::HasShipyard() const
    {
        return GetShipyardLevel() != 0;
    }

    uint32 Manager::GetShipyardLevel() const
    {
        uint32 l_Level = 0;

        auto l_Building = std::find_if(m_Buildings.begin(), m_Buildings.end(), [&l_Level](GarrisonBuilding l_Building) -> bool ///< l_Building is unused
        {
            auto l_GarrBuildingEntry = sGarrBuildingStore.LookupEntry(l_Building.BuildingID);

            if (l_GarrBuildingEntry)
            {
                if (l_GarrBuildingEntry->Type == Globals::ShipyardBuildingType)
                {
                    l_Level = l_GarrBuildingEntry->Level;
                    return true;
                }
                return false;
            }
            return false;
        });

        return l_Level;
    }

    uint32 Manager::GetShipyardMapId() const
    {
        return !HasShipyard() || !m_Owner ? -1 : GetGarrisonFactionIndex() == FactionIndex::Alliance ? ShipyardMapId::Alliance : ShipyardMapId::Horde;
    }

    void Manager::GetShipyardTerainSwaps(std::set<uint32>& p_TerrainSwaps) const
    {
        switch (GetGarrisonFactionIndex())
        {
            case FactionIndex::Alliance:
            {
                p_TerrainSwaps.emplace(ShipyardTerainSwaps::Part1);
                p_TerrainSwaps.emplace(ShipyardTerainSwaps::Part2);
                p_TerrainSwaps.emplace(ShipyardTerainSwaps::Part3);
                break;
            }
            case FactionIndex::Horde:
            default:
                break;
        }
    }

    bool Manager::CreateShipyardBySpell()
    {
        if (HasShipyard())
            return false;

        if (!GetGarrisonSiteLevelEntry() || GetGarrisonSiteLevelEntry()->Level != 3)
            return false;

        WorldPacket l_Data(SMSG_GARRISON_PLACE_BUILDING_RESULT, 26);
        l_Data << uint32(PurchaseBuildingResults::Ok);

        MS::Garrison::GarrisonBuilding l_Building = PurchaseBuilding(Globals::ShipyardBuildingID, Globals::ShipyardPlotID);

        l_Data << uint32(l_Building.PlotInstanceID);
        l_Data << uint32(l_Building.BuildingID);
        l_Data << uint32(l_Building.TimeBuiltStart);
        l_Data << uint32(l_Building.SpecID);
        l_Data << uint32(l_Building.TimeBuiltEnd);
        l_Data.WriteBit(l_Building.Active);
        l_Data.FlushBits();

        l_Data.WriteBit(true);                      ///< Unk bit
        l_Data.FlushBits();

        SendPacketToOwner(&l_Data);

        if (m_Owner->GetAreaId() == gGarrisonShipyardAreaID[GetGarrisonFactionIndex()])
            m_Owner->_SetInShipyard();

        return true;
    }

    bool Manager::CheckGarrisonStablesQuestsConditions(uint32 p_QuestID, Player* p_Player)
    {
        using namespace MS::Garrison::StablesData::Alliance;
        using namespace MS::Garrison::StablesData::Horde;

        if (std::find(FannyQuestGiver::g_BoarQuests.begin(), FannyQuestGiver::g_BoarQuests.end(), p_QuestID) != FannyQuestGiver::g_BoarQuests.end() ||
            std::find(TormakQuestGiver::g_BoarQuests.begin(), TormakQuestGiver::g_BoarQuests.end(), p_QuestID) != TormakQuestGiver::g_BoarQuests.end())
        {
            if (!p_Player->HasAura(MS::Garrison::StablesData::TrainingMountsAuras::RockstuckTrainingMountAura))
                return false;
        }
        else if (std::find(FannyQuestGiver::g_ClefthoofQuests.begin(), FannyQuestGiver::g_ClefthoofQuests.end(), p_QuestID) != FannyQuestGiver::g_ClefthoofQuests.end() ||
                 std::find(TormakQuestGiver::g_ClefthoofQuests.begin(), TormakQuestGiver::g_ClefthoofQuests.end(), p_QuestID) != TormakQuestGiver::g_ClefthoofQuests.end())
        {
            if (!p_Player->HasAura(MS::Garrison::StablesData::TrainingMountsAuras::IcehoofTrainingMountAura))
                return false;
        }
        else if (std::find(FannyQuestGiver::g_ElekkQuests.begin(), FannyQuestGiver::g_ElekkQuests.end(), p_QuestID) != FannyQuestGiver::g_ElekkQuests.end() ||
                 std::find(TormakQuestGiver::g_ElekkQuests.begin(), TormakQuestGiver::g_ElekkQuests.end(), p_QuestID) != TormakQuestGiver::g_ElekkQuests.end())
        {
            if (!p_Player->HasAura(MS::Garrison::StablesData::TrainingMountsAuras::MeadowstomperTrainingMountAura))
                return false;
        }
        else if (std::find(KeeganQuestGiver::g_RiverbeastQuests.begin(), KeeganQuestGiver::g_RiverbeastQuests.end(), p_QuestID) != KeeganQuestGiver::g_RiverbeastQuests.end() ||
                 std::find(SagePalunaQuestGiver::g_RiverbeastQuests.begin(), SagePalunaQuestGiver::g_RiverbeastQuests.end(), p_QuestID) != SagePalunaQuestGiver::g_RiverbeastQuests.end())
        {
            if (!p_Player->HasAura(MS::Garrison::StablesData::TrainingMountsAuras::RiverwallowTrainingMountAura))
                return false;
        }
        else if (std::find(KeeganQuestGiver::g_TalbukQuests.begin(), KeeganQuestGiver::g_TalbukQuests.end(), p_QuestID) != KeeganQuestGiver::g_TalbukQuests.end() ||
                 std::find(SagePalunaQuestGiver::g_TalbukQuests.begin(), SagePalunaQuestGiver::g_TalbukQuests.end(), p_QuestID) != SagePalunaQuestGiver::g_TalbukQuests.end())
        {
            if (!p_Player->HasAura(MS::Garrison::StablesData::TrainingMountsAuras::SilverpeltTrainingMountAura))
                return false;
        }
        else if (std::find(KeeganQuestGiver::g_WolfQuests.begin(), KeeganQuestGiver::g_WolfQuests.end(), p_QuestID) != KeeganQuestGiver::g_WolfQuests.end() ||
                 std::find(SagePalunaQuestGiver::g_WolfQuests.begin(), SagePalunaQuestGiver::g_WolfQuests.end(), p_QuestID) != SagePalunaQuestGiver::g_WolfQuests.end())
        {
            if (!p_Player->HasAura(MS::Garrison::StablesData::TrainingMountsAuras::SnarlerTrainingMountAura))
                return false;
        }

        return true;
    }

    bool Manager::IsTrainingMount()
    {
        return m_Owner->HasAura(StablesData::TrainingMountsAuras::RockstuckTrainingMountAura)
            || m_Owner->HasAura(StablesData::TrainingMountsAuras::IcehoofTrainingMountAura)
            || m_Owner->HasAura(StablesData::TrainingMountsAuras::MeadowstomperTrainingMountAura)
            || m_Owner->HasAura(StablesData::TrainingMountsAuras::RiverwallowTrainingMountAura)
            || m_Owner->HasAura(StablesData::TrainingMountsAuras::SilverpeltTrainingMountAura)
            || m_Owner->HasAura(StablesData::TrainingMountsAuras::SnarlerTrainingMountAura);
    }

    void Manager::HandleStablesAuraBonuses(bool p_Remove /*= false*/)
    {
        if (m_Owner->GetMapId() != 1116) ///< MapID Draenor
            return;

        using namespace StablesData;

        if (!p_Remove)
        {
            switch (GetBuildingLevel(GetBuildingWithType(BuildingType::Stable)))
            {
                case 1:
                    m_Owner->AddAura(BonusAuras::StablesAuraLevel1, m_Owner);
                    break;
                case 2:
                    m_Owner->AddAura(BonusAuras::StablesAuraLevel2, m_Owner);
                    m_Owner->SetFlag(UNIT_FIELD_FLAGS_3, UNIT_FLAG3_CAN_FIGHT_WITHOUT_DISMOUNT);
                    break;
                case 3:
                    m_Owner->AddAura(BonusAuras::StablesAuraLevel3, m_Owner);
                    m_Owner->SetFlag(UNIT_FIELD_FLAGS_3, UNIT_FLAG3_CAN_FIGHT_WITHOUT_DISMOUNT);
                    break;
            }
        }
        else
        {
            switch (GetBuildingLevel(GetBuildingWithType(BuildingType::Stable)))
            {
                case 1:
                    m_Owner->RemoveAura(BonusAuras::StablesAuraLevel1);
                    break;
                case 2:
                    m_Owner->RemoveAura(BonusAuras::StablesAuraLevel2);
                    m_Owner->RemoveFlag(UNIT_FIELD_FLAGS_3, UNIT_FLAG3_CAN_FIGHT_WITHOUT_DISMOUNT);
                    break;
                case 3:
                    m_Owner->RemoveAura(BonusAuras::StablesAuraLevel3);
                    m_Owner->RemoveFlag(UNIT_FIELD_FLAGS_3, UNIT_FLAG3_CAN_FIGHT_WITHOUT_DISMOUNT);
                    break;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////
    //////////////////////// DAILY TAVERN DATA ///////////////////////////
    //////////////////////////////////////////////////////////////////////

    void Manager::AddGarrisonDailyTavernData(uint32 p_Data)
    {
        SetGarrisonDailyTavernData(p_Data);
        m_Owner->SaveToDB(false);
    }

    void Manager::SetGarrisonDailyTavernData(uint32 p_Data)
    {
        GetGarrisonDailyTavernDatas().push_back(p_Data);
    }

    void Manager::CleanGarrisonDailyTavernData()
    {
        GetGarrisonDailyTavernDatas().clear();
    }

    void Manager::ResetGarrisonDailyTavernData()
    {
        if (roll_chance_i(50))
        {
            uint32 l_Entry = TavernDatas::g_QuestGiverEntries[urand(0, TavernDatas::g_QuestGiverEntries.size() - 1)];

            CleanGarrisonDailyTavernData();
            AddGarrisonDailyTavernData(l_Entry);
        }
        else
        {
            uint32 l_FirstEntry = TavernDatas::g_QuestGiverEntries[urand(0, TavernDatas::g_QuestGiverEntries.size() - 1)];
            uint32 l_SecondEntry = 0;

            do
                l_SecondEntry = TavernDatas::g_QuestGiverEntries[urand(0, TavernDatas::g_QuestGiverEntries.size() - 1)];
            while (l_SecondEntry == l_FirstEntry);

            CleanGarrisonDailyTavernData();
            AddGarrisonDailyTavernData(l_FirstEntry);
            AddGarrisonDailyTavernData(l_SecondEntry);
        }
    }

    //////////////////////////////////////////////////////////////////////


    //////////////////////////////////////////////////////////////////////
    //////////////////////// WEEKLY TAVERN DATA //////////////////////////
    //////////////////////////////////////////////////////////////////////

    void Manager::AddGarrisonWeeklyTavernData(WeeklyTavernData p_Data)
    {
        SetGarrisonWeeklyTavernData(p_Data);
        m_Owner->SaveToDB(false);
    }

    void Manager::SetGarrisonWeeklyTavernData(WeeklyTavernData p_Data)
    {
        GetGarrisonWeeklyTavernDatas().push_back(p_Data);
    }

    void Manager::CleanGarrisonWeeklyTavernData()
    {
        GetGarrisonWeeklyTavernDatas().clear();
    }

    void Manager::ResetGarrisonWeeklyTavernData()
    {
        CleanGarrisonWeeklyTavernData();
    }

    //////////////////////////////////////////////////////////////////////

    void Manager::ResetGarrisonWorkshopData(Player* p_Player)
    {
        using namespace WorkshopGearworks;

        if (p_Player->GetTeamId() == TEAM_ALLIANCE)
        {
            if (!p_Player->IsQuestRewarded(Quests::Alliance_UnconventionalInventions))
                return;
        }
        else if (p_Player->GetTeamId() == TEAM_HORDE)
        {
            if (!p_Player->IsQuestRewarded(Quests::Horde_UnconventionalInventions))
                return;
        }

        uint32 l_Worldstate = p_Player->GetCharacterWorldStateValue(CharacterWorldStates::GarrisonWorkshopGearworksInvention);
        std::vector<uint32> l_Inventions;

        for (uint32 l_Value : g_FirstLevelInventions)
            l_Inventions.push_back(l_Value);

        if (GetBuildingLevel(GetBuildingWithType(BuildingType::Workshop)) > 1)
        {
            for (uint32 l_Value : g_SecondLevelInventions)
                l_Inventions.push_back(l_Value);
        }

        for (std::vector<uint32>::iterator l_Itr = l_Inventions.begin(); l_Itr != l_Inventions.end();)
        {
            if (std::find(l_Inventions.begin(), l_Inventions.end(), *l_Itr) != l_Inventions.end())
            {
                ++l_Itr;
                continue;
            }

            if (*l_Itr == l_Worldstate)
                l_Itr = l_Inventions.erase(l_Itr);

            if (p_Player->GetTeamId() == TEAM_ALLIANCE && *l_Itr == GobPrototypeMechanoHog)
                l_Itr = l_Inventions.erase(l_Itr);

            if (p_Player->GetTeamId() == TEAM_HORDE && *l_Itr == GobPrototypeMekgineersChopper)
                l_Itr = l_Inventions.erase(l_Itr);
        }

        uint32 l_Entry = l_Inventions[urand(0, l_Inventions.size() - 1)];

        ItemTemplate const* l_ItemProto = sObjectMgr->GetItemTemplate(g_GobItemRelations[l_Entry]);

        if (l_ItemProto == nullptr)
            return;

        p_Player->SetCharacterWorldState(CharacterWorldStates::GarrisonWorkshopGearworksInvention, l_Entry);
        p_Player->SetCharacterWorldState(CharacterWorldStates::GarrisonWorkshopGearworksInventionCharges, l_ItemProto->Spells[0].SpellCharges);
    }

    void Manager::ResetGarrisonTradingPostData(Player* p_Player)
    {
        std::vector<uint32> l_HordeTradersEntries = { 86778, 86777, 86779, 86776, 86683 };
        std::vector<uint32> l_AllianceTradersEntries = { 87203, 87202, 87200, 87201, 87204 };
        std::vector<uint32> l_TradingPostShipments = { 138, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 196 };
        uint32 l_Entry = p_Player->GetTeamId() == TEAM_ALLIANCE ? l_AllianceTradersEntries[urand(0, l_AllianceTradersEntries.size() - 1)] : l_HordeTradersEntries[urand(0, l_HordeTradersEntries.size() - 1)];

        p_Player->SetCharacterWorldState(CharacterWorldStates::GarrisonTradingPostDailyRandomShipment, l_TradingPostShipments[urand(0, l_TradingPostShipments.size() - 1)]);
        p_Player->SetCharacterWorldState(CharacterWorldStates::GarrisonTradingPostDailyRandomTrader, l_Entry);
    }

    std::vector<GarrisonFollower> Manager::GetWeeklyFollowerRecruits(Player* p_Player)
    {
        std::vector<GarrisonFollower> l_Followers;
        
        std::vector<WeeklyTavernData> l_Data = GetGarrisonWeeklyTavernDatas();

        for (WeeklyTavernData l_Datum : l_Data)
        {
            GarrisonFollower l_Follower = GenerateNewFollowerEntity(l_Datum.FollowerID);

            l_Follower.Abilities.clear();

            for (uint32 l_Ability : l_Datum.Abilities)
                l_Follower.Abilities.push_back(l_Ability);

            l_Followers.push_back(l_Follower);
        }

        return l_Followers;
    }
}   ///< namespace Garrison
}   ///< namespace MS
