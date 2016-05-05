////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014 Millenium-studio SARL
//  All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////// 

#include "BattlegroundScheduler.hpp"
#include "BattlegroundMgr.hpp"
#include "Group.h"
#include "BattlegroundPacketFactory.hpp"

namespace MS
{
    namespace Battlegrounds
    {
        /// Anonymous namespace for static function because we don't need them out of this unit translation.
        namespace
        {
            /// Return true if the battleground is wished by the group.
            /// @p_Group : The group.
            /// @p_Type : The type of the battleground.
            /// @p_BracketId : The bracket id of the battleground.
            static bool IsEligibleForBattleground(GroupQueueInfo const* p_Group, BattlegroundType::Type p_Type, Bracket::Id p_BracketId, Battleground const* p_Template)
            {
                uint64 l_BattlegroundMask = 1LL << p_Type;

                if (p_Group->m_BracketId != p_BracketId)
                    return false;

                /// We check if all the players have the good level for entering the battleground.
                for (auto l_Plrs : p_Group->m_Players)
                {
                    Player* l_Player = ObjectAccessor::FindPlayer(l_Plrs.first);
                    if (!l_Player)
                        continue;

                    if (l_Player->getLevel() < p_Template->GetMinLevel())
                        return false;
                }

                return (p_Group->m_WantedBGs & l_BattlegroundMask) != 0;
            }

            static bool AreMatching(GroupQueueInfo const* p_A, GroupQueueInfo const* p_B)
            {
                if (p_A == p_B)
                    return false;

                if (!p_A || !p_B)
                    return false;

                /// Setting rating ranges.
                uint32 l_GroupAMinRating = (p_A->m_ArenaMatchmakerRating <= sBattlegroundMgr->GetMaxRatingDifference()) ? 0 : p_A->m_ArenaMatchmakerRating - sBattlegroundMgr->GetMaxRatingDifference();
                uint32 l_GroupAMaxRating = p_A->m_ArenaMatchmakerRating + sBattlegroundMgr->GetMaxRatingDifference();

                uint32 l_GroupBMinRating = (p_B->m_ArenaMatchmakerRating <= sBattlegroundMgr->GetMaxRatingDifference()) ? 0 : p_B->m_ArenaMatchmakerRating - sBattlegroundMgr->GetMaxRatingDifference();
                uint32 l_GroupBMaxRating = p_B->m_ArenaMatchmakerRating + sBattlegroundMgr->GetMaxRatingDifference();

                float l_MmrASteps = floor(float((getMSTime() - p_A->m_JoinTime) / 60000));
                float l_MmrBSteps = floor(float((getMSTime() - p_B->m_JoinTime) / 60000));

                uint32 l_MmrAMaxDiff = l_MmrASteps * 150;
                uint32 l_MmrBMaxDiff = l_MmrBSteps * 150;

                uint32 l_DiscardTime = getMSTime() - sBattlegroundMgr->GetRatingDiscardTimer();

                if (l_MmrAMaxDiff > 0)
                {
                    l_GroupAMinRating = (l_MmrAMaxDiff < l_GroupAMinRating) ? l_GroupAMinRating - l_MmrAMaxDiff : 0;
                    l_GroupAMaxRating = l_MmrAMaxDiff + l_GroupAMaxRating;
                }

                if (l_MmrBMaxDiff > 0)
                {
                    l_GroupBMinRating = (l_MmrBMaxDiff < l_GroupBMinRating) ? l_GroupBMinRating - l_MmrBMaxDiff : 0;
                    l_GroupBMaxRating = l_MmrBMaxDiff + l_GroupBMaxRating;
                }

                if ((p_B->m_ArenaMatchmakerRating >= l_GroupAMinRating && p_B->m_ArenaMatchmakerRating <= l_GroupAMaxRating) || p_B->m_JoinTime < l_DiscardTime)
                    return true;

                if ((p_A->m_ArenaMatchmakerRating >= l_GroupBMinRating && p_A->m_ArenaMatchmakerRating <= l_GroupBMaxRating) || p_A->m_JoinTime < l_DiscardTime)
                    return true;

                return false;
            }

            /// Utility function for calculation average.
            static float AddAndRecalculateAvg(float p_Avg, float p_Value, float p_Norm)
            {
                return ((p_Avg * p_Norm) + p_Value) / p_Norm;
            }
        }

        BattlegroundScheduler::BattlegroundScheduler()
            : m_QueuedGroups(),
            m_NumInstances(0),
            m_BattlegroundOccurences()
        {
            /// Initialize BattlegroundOccurences.
            for (std::size_t l_BracketId = 0; l_BracketId < Brackets::Count; l_BracketId++)
            {
                m_TotalOccurences[l_BracketId] = 0.0f;
                for (std::size_t i = 0; i < BattlegroundType::Max; i++)
                    m_BattlegroundOccurences[l_BracketId][i] = std::make_pair(0.0f, i);
            }
        }

        void BattlegroundScheduler::AddToBG(GroupQueueInfo* p_Group, Battleground* p_BG, uint32 p_Team)
        {
            /// Now that the group leave the queue, we can restrict to instanciable battlegrounds.
            for (auto const& l_Itr : p_Group->m_Players)
            {
                /// Get the player.
                Player* l_Player = ObjectAccessor::FindPlayer(l_Itr.first);

                /// If offline, skip him, this should not happen - player is removed from queue when he logs out.
                if (!l_Player)
                    continue;

                if (GetSchedulerType(p_BG->GetTypeID()) != p_Group->m_BgTypeId)
                    l_Player->ChangeBattlegroundQueueJoinTimeKey(GetSchedulerType(p_BG->GetTypeID()), p_Group->m_BgTypeId);
                l_Player->SetBattlegroundQueueTypeId(l_Player->GetBattlegroundQueueIndex(p_Group->m_BgTypeId), GetSchedulerType(p_BG->GetTypeID()));
            }
            p_Group->m_BgTypeId = GetSchedulerType(p_BG->GetTypeID());

            /// Move group to invitations mgr and invite them to the battleground.
            sBattlegroundMgr->GetInvitationsMgr().InviteGroupToBG(p_Group, p_BG, p_Team);
        }

        GroupQueueInfo* BattlegroundScheduler::AddGroup(Player* p_Leader, Group* p_Group, BattlegroundType::Type p_BgTypeId, uint32 p_BlackWishes[2], MS::Battlegrounds::Bracket const*  p_BracketEntry, ArenaType p_ArenaType, bool p_IsRatedBG, uint32 p_ArenaRating, uint32 p_MatchmakerRating, bool p_IsSkirmish)
        {
            Bracket::Id l_BracketId = p_BracketEntry->m_Id;

            /// Create new GroupQueueInfo.
            GroupQueueInfo* l_GroupQueue = new GroupQueueInfo;
            l_GroupQueue->m_BgTypeId = p_BgTypeId;
            l_GroupQueue->m_ArenaType = p_ArenaType;
            l_GroupQueue->m_IsRatedBG = p_IsRatedBG;
            l_GroupQueue->m_IsSkirmish = p_IsSkirmish;
            l_GroupQueue->m_IsInvitedToBGInstanceGUID = 0;
            l_GroupQueue->m_JoinTime = getMSTime();
            l_GroupQueue->m_RemoveInviteTime = 0;
            l_GroupQueue->m_Team = p_Leader->GetTeam();
            l_GroupQueue->m_ArenaTeamRating = p_ArenaRating;
            l_GroupQueue->m_ArenaMatchmakerRating = p_MatchmakerRating;
            l_GroupQueue->m_OpponentsTeamRating = 0;
            l_GroupQueue->m_OpponentsMatchmakerRating = 0;
            l_GroupQueue->m_Group = p_Group;
            l_GroupQueue->m_BracketId = l_BracketId;
            l_GroupQueue->m_IsRandom = p_BgTypeId == BattlegroundType::RandomBattleground;

            /// We specify the wishes of the group.
            switch (p_BgTypeId)
            {
                case BattlegroundType::RandomBattleground:
                    l_GroupQueue->m_WantedBGs = BattlegroundMasks::AllBattlegrounds;
                    break;
                case BattlegroundType::AllArenas:
                    l_GroupQueue->m_WantedBGs = BattlegroundMasks::AllArenas;
                    break;
                default:
                    l_GroupQueue->m_WantedBGs = 1LL << p_BgTypeId;
                    break;
            }

            /// Handling black wishes.
            if (p_BlackWishes)
            {
                for (std::size_t i = 0; i < 2; i++)
                {
                    BattlegroundType::Type l_Type = Maps::FindAssociatedType(p_BlackWishes[i]);
                    if (l_Type == BattlegroundType::None)
                        continue;

                    if (l_GroupQueue->m_WantedBGs & (1LL << l_Type))
                        l_GroupQueue->m_WantedBGs &= ~(1LL << l_Type);
                }
            }

            l_GroupQueue->m_Players.clear();

            uint32 l_LastOnlineTime = getMSTime();

            /// Add players from group to GroupQueueInfo.
            if (p_Group)
            {
                for (GroupReference* l_Iterator = p_Group->GetFirstMember(); l_Iterator != nullptr; l_Iterator = l_Iterator->next())
                {
                    Player* l_Member = l_Iterator->getSource();
                    if (!l_Member)
                        continue;

                    /// Create the PlayerQueueInfo.
                    PlayerQueueInfo& l_PlayerQueue = m_QueuedPlayers[l_Member->GetGUID()];
                    l_PlayerQueue.Infos.emplace_back(PlayerQueueInfo::Pair{ l_LastOnlineTime, l_GroupQueue });

                    l_GroupQueue->m_Players[l_Member->GetGUID()] = &l_PlayerQueue;
                }
            }
            else
            {
                PlayerQueueInfo& l_PlayerQueue = m_QueuedPlayers[p_Leader->GetGUID()];
                l_PlayerQueue.Infos.emplace_back(PlayerQueueInfo::Pair{ l_LastOnlineTime, l_GroupQueue });

                l_GroupQueue->m_Players[p_Leader->GetGUID()] = &l_PlayerQueue;
            }

            /// Add the GroupQueueInfo in the groups to match.
            m_QueuedGroups[l_BracketId][l_GroupQueue->GetTeam()].emplace_back(l_GroupQueue);

            return l_GroupQueue;
        }

        void BattlegroundScheduler::RemovePlayer(uint64 p_Guid, BattlegroundType::Type p_Type)
        {
            /// Remove player from map, if he's there.
            auto l_Itr = m_QueuedPlayers.find(p_Guid);
            if (l_Itr == m_QueuedPlayers.end())
                return;

            GroupQueueInfo* l_Group = nullptr;

            auto l_Pair = std::begin(l_Itr->second.Infos);
            for (; l_Pair != std::end(l_Itr->second.Infos); l_Pair++)
            {
                GroupQueueInfo* l_GroupInfo = l_Pair->GroupInfo;
                if (l_GroupInfo->m_BgTypeId == p_Type)
                {
                    l_Group = l_GroupInfo;
                    break;
                }
            }

            /// Player can't be in queue without group, but just in case.
            if (!l_Group)
                return;

            int32 l_BracketId = l_Group->m_BracketId;
            if (l_BracketId == -1)
            {
                sLog->outError(LOG_FILTER_BATTLEGROUND, "BattlegroundQueue: ERROR Cannot find groupinfo for player GUID: %u", GUID_LOPART(p_Guid));
                return;
            }

            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BattlegroundQueue: Removing player GUID %u, from bracket_id %u", GUID_LOPART(p_Guid), (uint32)l_BracketId);

            // ALL variables are correctly set
            // We can ignore leveling up in queue - it should not cause crash
            // remove player from group
            // if only one player there, remove group

            /// Remove player queue info from group queue info.
            auto pitr = l_Group->m_Players.find(p_Guid);
            if (pitr != l_Group->m_Players.end())
                l_Group->m_Players.erase(pitr);

            /// Remove player queue info.
            if (l_Itr->second.Infos.size() == 1)
                m_QueuedPlayers.erase(l_Itr);
            else
                l_Itr->second.Infos.erase(l_Pair);

            // remove group queue info if needed
            if (l_Group->m_Players.empty())
            {
                auto& l_Queue = m_QueuedGroups[l_BracketId][l_Group->GetTeam()];
                auto l_Itr = std::find(std::begin(l_Queue), std::end(l_Queue), l_Group);

                if (std::end(l_Queue) != l_Itr)
                    l_Queue.erase(l_Itr);
                delete l_Group;
            }

            // if group wasn't empty, so it wasn't deleted, and player have left a rated
            // queue -> everyone from the group should leave too
            // don't remove recursively if already invited to bg!
            else if (!l_Group->m_IsInvitedToBGInstanceGUID && l_Group->m_IsRatedBG)
            {
                BattlegroundType::Type bgQueueTypeId = GetTypeFromId(GetIdFromType(l_Group->m_BgTypeId), l_Group->m_ArenaType);
                // remove next player, this is recursive
                // first send removal information
                if (Player* plr2 = ObjectAccessor::FindPlayer(l_Group->m_Players.begin()->first))
                {
                    Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(l_Group->m_BgTypeId);
                    uint32 queueSlot = plr2->GetBattlegroundQueueIndex(bgQueueTypeId);
                    plr2->RemoveBattlegroundQueueId(bgQueueTypeId); // must be called this way, because if you move this call to
                    // queue->removeplayer, it causes bugs
                    WorldPacket data;
                    PacketFactory::Status(&data, bg, plr2, queueSlot, STATUS_NONE, plr2->GetBattlegroundQueueJoinTime(l_Group->m_BgTypeId), 0, 0, false);
                    plr2->GetSession()->SendPacket(&data);
                }
                // then actually delete, this may delete the group as well!
                RemovePlayer(l_Group->m_Players.begin()->first, bgQueueTypeId);
            }
        }

        bool BattlegroundScheduler::GetPlayerGroupInfoData(uint64 p_Guid, GroupQueueInfo& p_GroupInfo, BattlegroundType::Type p_Type) const
        {
            auto l_Itr = m_QueuedPlayers.find(p_Guid);
            if (l_Itr == std::end(m_QueuedPlayers))
                return false;

            for (auto l_Pair = std::begin(l_Itr->second.Infos); l_Pair != std::end(l_Itr->second.Infos); l_Pair++)
            {
                GroupQueueInfo* l_Group = l_Pair->GroupInfo;
                if (l_Group->m_BgTypeId == p_Type)
                {
                    p_GroupInfo = *l_Group;
                    return true;
                }
            }

            return false;
        }

        //////////////////////////////////////////////////////////////////////////
        /// Internal system methods for handling the wishes of the groups.
        //////////////////////////////////////////////////////////////////////////

#pragma region Internal

        void BattlegroundScheduler::RemoveGroupFromQueues(GroupQueueInfo* p_Group)
        {
            m_QueuedGroups[p_Group->m_BracketId][p_Group->GetTeam()].remove(p_Group);

            for (auto const& l_Itr : p_Group->m_Players)
                m_QueuedPlayers.erase(l_Itr.first);
        }

        void BattlegroundScheduler::AllocateGroupsInExistingBattlegrounds(Bracket::Id p_BracketId, std::size_t p_Team, std::vector<std::pair<float, std::size_t>>& p_Avg)
        {
            std::list<GroupQueueInfo*> l_ToRemove;

            if (m_QueuedGroups[p_BracketId][p_Team].empty())
                return;

            /// Insert groups in their categories.
            for (GroupQueueInfo* l_Group : m_QueuedGroups[p_BracketId][p_Team])
            {
                /// We first sort the averages and keep track of the indexes.
                std::sort(std::begin(p_Avg), std::end(p_Avg), [](std::pair<float, std::size_t> const& p_A, std::pair<float, std::size_t> const& p_B)
                {
                    return p_A.first < p_B.first;
                });

                bool l_AreFull = true;
                for (std::size_t l_Fallback = 0; l_Fallback < BattlegroundType::Max; l_Fallback++)
                {
                    BattlegroundType::Type l_BgType = static_cast<BattlegroundType::Type>(p_Avg[l_Fallback].second);

                    /// We get the battleground template.
                    Battleground* l_Template = sBattlegroundMgr->GetBattlegroundTemplate(l_BgType);
                    if (!l_Template)
                        continue;

                    /// We check if the group is eligible for the battleground.
                    if (!IsEligibleForBattleground(l_Group, l_BgType, p_BracketId, l_Template))
                        continue;

                    auto& l_Battlegrounds = sBattlegroundMgr->GetBattlegroundList(p_BracketId, l_BgType);
                    /// We sort according to the lowest ratio of players for the specific team.
                    l_Battlegrounds.sort([p_Team, l_BgType, l_Template](std::pair<uint32, Battleground*> const& p_A, std::pair<uint32, Battleground*> const& p_B)
                    {
                        return p_A.second->GetPlayersCountByTeam(p_Team == TEAM_ALLIANCE ? ALLIANCE : HORDE) / l_Template->GetMaxPlayers()
                            - p_B.second->GetPlayersCountByTeam(p_Team == TEAM_ALLIANCE ? ALLIANCE : HORDE) / l_Template->GetMaxPlayers();
                    });

                    for (std::pair<uint32, Battleground*> const& l_Pair : l_Battlegrounds)
                    {
                        Battleground* l_Bg = l_Pair.second;

                        /// Players can't join already started rated battleground / arena match
                        if (l_Bg->IsRatedBG() || l_Bg->isArena())
                            continue;

                        /// We check if the number of invited/playing players and the size of this group doesn't exceed the max allowed.
                        if (l_Bg->CanGroupEnter(l_Group))
                        {
                            l_AreFull = false;
                            AddToBG(l_Group, l_Bg);
                            l_ToRemove.emplace_back(l_Group);
                            p_Avg[l_Fallback].first = AddAndRecalculateAvg(p_Avg[l_Fallback].first, l_Group->m_Players.size(), l_Battlegrounds.size() * l_Template->GetMaxPlayers());
                            break;
                        }
                    }

                    // We found a battleground so no need to check further.
                    if (!l_AreFull)
                        break;
                }
            }

            // We remove from the waiting groups the groups that entered a battleground.
            for (GroupQueueInfo* l_Group : l_ToRemove)
                RemoveGroupFromQueues(l_Group);
        }

        void BattlegroundScheduler::FindPotentialBGs(Bracket::Id p_BracketId, std::vector<float>& p_PotientialBGs, std::vector<std::list<GroupQueueInfo*>>& p_PotentialGroups)
        {
            /// We clean our variables.
            for (std::size_t i = 0; i < BattlegroundType::Max * 2; i++)
            {
                p_PotientialBGs[i] = 0;
                p_PotentialGroups[i].clear();
            }

            /// Accumulate group sizes.
            for (std::size_t l_Team = TEAM_ALLIANCE; l_Team <= TEAM_HORDE; l_Team++)
            {
                for (GroupQueueInfo* l_Group : m_QueuedGroups[p_BracketId][l_Team])
                {
                    /// Check if the group is of the right team.
                    if (l_Group->GetTeam() != l_Team)
                        continue;

                    for (std::size_t i = 0; i < BattlegroundType::Max * 2; i += 2)
                    {
                        BattlegroundType::Type l_BgType = static_cast<BattlegroundType::Type>(i / 2);

                        /// We get the battleground template.
                        Battleground* l_Template = sBattlegroundMgr->GetBattlegroundTemplate(l_BgType);
                        if (!l_Template)
                            continue;

                        /// We check if the group is eligible for the arena.
                        if (!IsEligibleForBattleground(l_Group, l_BgType, p_BracketId, l_Template))
                            continue;

                        /// In the case of the casual battleground, we check if the group can fit in the battleground according to the max players and the group size.
                        if (BattlegroundType::IsCasualBattleground(l_BgType))
                        {
                            /// We check if the number of players doesn't exceed the max number of players in the battleground.
                            if (p_PotientialBGs[i + l_Team] + l_Group->m_Players.size() > l_Template->GetMaxPlayersPerTeam())
                                continue;

                            p_PotientialBGs[i + l_Team] += l_Group->m_Players.size();
                            p_PotentialGroups[i + l_Team].emplace_back(l_Group);
                        }
                        /// If it's an arena not rated, we check according to the type of the arena, the size of the currents groups and the team of the groups.
                        else if (BattlegroundType::IsArena(l_BgType) && !BattlegroundType::IsRated(l_BgType))
                        {
                            uint32 l_MaxPlayerPerTeam = 0;

                            /// We get the maximum of players according to the arena type.
                            switch (BattlegroundType::GetArenaType(l_BgType))
                            {
                                case ArenaType::Arena2v2:
                                    l_MaxPlayerPerTeam = 2;
                                    break;
                                case ArenaType::Arena3v3:
                                    l_MaxPlayerPerTeam = 3;
                                    break;
                                case ArenaType::Arena5v5:
                                    l_MaxPlayerPerTeam = 5;
                                    break;
                                default:
                                    return;
                            }

                            /// Arena type couldn't be deduced.
                            if (l_MaxPlayerPerTeam == 0)
                                continue;

                            auto l_FirstGroup = std::begin(p_PotentialGroups[i + TEAM_HORDE]);
                            /// We first check if the factions are the same because we don't want mixed arena teams.
                            if (l_FirstGroup == std::end(p_PotentialGroups[i + TEAM_HORDE])
                                || (l_FirstGroup != std::end(p_PotentialGroups[i + TEAM_HORDE]) && (*l_FirstGroup)->m_Team == l_Group->m_Team))
                            {
                                /// We check if in the team there is still some place.
                                if (p_PotientialBGs[i + TEAM_HORDE] + l_Group->m_Players.size() <= l_MaxPlayerPerTeam)
                                {
                                    p_PotientialBGs[i + TEAM_HORDE] += l_Group->m_Players.size();
                                    p_PotentialGroups[i + TEAM_HORDE].emplace_back(l_Group);
                                    continue;
                                }
                            }

                            l_FirstGroup = std::begin(p_PotentialGroups[i + TEAM_ALLIANCE]);
                            /// We first check if the factions are the same because we don't want mixed arena teams.
                            if (l_FirstGroup == std::end(p_PotentialGroups[i + TEAM_ALLIANCE])
                                || (l_FirstGroup != std::end(p_PotentialGroups[i + TEAM_ALLIANCE]) && (*l_FirstGroup)->m_Team == l_Group->m_Team))
                            {
                                /// We check if in the team there is still some place.
                                if (p_PotientialBGs[i + TEAM_ALLIANCE] + l_Group->m_Players.size() <= l_MaxPlayerPerTeam)
                                {
                                    p_PotientialBGs[i + TEAM_ALLIANCE] += l_Group->m_Players.size();
                                    p_PotentialGroups[i + TEAM_ALLIANCE].emplace_back(l_Group);
                                    continue;
                                }
                            }
                        }
                        /// If the battleground is rated and is not an arena, we check according to the size and we merge the teams because we are sure there won't be any mix.
                        else if (BattlegroundType::IsRated(l_BgType))
                        {
                            uint32 l_NumberOfPlayersRequired = 0;

                            switch (l_BgType)
                            {
                                case BattlegroundType::RatedBg10v10:
                                    l_NumberOfPlayersRequired = 10;
                                    break;
                                case BattlegroundType::RatedBg15v15:
                                    l_NumberOfPlayersRequired = 15;
                                    break;
                                case BattlegroundType::RatedBg25v25:
                                    l_NumberOfPlayersRequired = 25;
                                    break;
                                case BattlegroundType::Arena2v2:
                                    l_NumberOfPlayersRequired = 2;
                                    break;
                                case BattlegroundType::Arena3v3:
                                    l_NumberOfPlayersRequired = 3;
                                    break;
                                case BattlegroundType::Arena5v5:
                                    l_NumberOfPlayersRequired = 5;
                                    break;
                            }

                            /// Battleground is not known as a rated one.
                            if (l_NumberOfPlayersRequired == 0)
                                continue;

                            /// If there is not enough players, we pass.
                            if (l_NumberOfPlayersRequired > l_Group->m_Players.size())
                                continue;

                            /// We don't make any differences between factions so we merge them.
                            p_PotientialBGs[i + TEAM_ALLIANCE] += l_Group->m_Players.size();
                            p_PotentialGroups[i + TEAM_ALLIANCE].emplace_back(l_Group);
                            p_PotientialBGs[i + TEAM_HORDE] += l_Group->m_Players.size();
                            p_PotentialGroups[i + TEAM_HORDE].emplace_back(l_Group);
                        }
                    }
                }
            }
        }

        void BattlegroundScheduler::FindMatches()
        {
            //////////////////////////////////////////////////////////////////////////////////
            // Policy: trying to have a ratio of battlegrounds instances equals for each type.
            //////////////////////////////////////////////////////////////////////////////////

            for (std::size_t l_BracketId = 0; l_BracketId < Brackets::Count; l_BracketId++)
            {
                /// We first calculate the average number of players by BGType in order to fill entirely the battlegrounds.
                std::vector<std::pair<float, std::size_t>> l_AvgHorde;
                std::vector<std::pair<float, std::size_t>> l_AvgAlliance;

                /// Initialize average arrays.
                l_AvgAlliance.resize(BattlegroundType::Max);
                l_AvgHorde.resize(BattlegroundType::Max);
                for (std::size_t i = 0; i < BattlegroundType::Max; i++)
                {
                    l_AvgHorde[i] = std::make_pair(0.0f, i);
                    l_AvgAlliance[i] = std::make_pair(0.0f, i);
                }

                /// Calculating averages.
                for (std::size_t i = 0; i < BattlegroundType::Max; i++)
                {
                    BattlegroundType::Type l_BgType = static_cast<BattlegroundType::Type>(i);

                    /// We get the battleground template.
                    Battleground* l_Template = sBattlegroundMgr->GetBattlegroundTemplate(l_BgType);
                    if (!l_Template)
                        continue;

                    auto& l_Battlegrounds = sBattlegroundMgr->GetBattlegroundList(l_BracketId, l_BgType);

                    // What is the average filling of each battleground type ?
                    std::size_t l_Size = 0;
                    for (std::pair<uint32, Battleground*> const& l_Pair : l_Battlegrounds)
                    {
                        Battleground* l_BG = l_Pair.second;

                        /// Don't count ended battleground
                        if (l_BG->GetStatus() == BattlegroundStatus::STATUS_WAIT_LEAVE)
                            continue;

                        l_Size++;

                        l_AvgHorde[i].first += l_BG->GetPlayersCountByTeam(HORDE);
                        l_AvgAlliance[i].first += l_BG->GetPlayersCountByTeam(ALLIANCE);
                    }

                    if (l_Size)
                    {
                        l_AvgHorde[i].first /= l_Size * l_Template->GetMaxPlayers();
                        l_AvgAlliance[i].first /= l_Size * l_Template->GetMaxPlayers();
                    }
                }

                /// Sort averages and keep track of indexes.
                std::sort(std::begin(l_AvgAlliance), std::end(l_AvgAlliance), [](std::pair<float, std::size_t> const& p_A, std::pair<float, std::size_t> const& p_B)
                {
                    return p_A.first < p_B.first;
                });

                std::sort(std::begin(l_AvgHorde), std::end(l_AvgHorde), [](std::pair<float, std::size_t> const& p_A, std::pair<float, std::size_t> const& p_B)
                {
                    return p_A.first < p_B.first;
                });

                /// We insert groups in existing battlegrounds.
                AllocateGroupsInExistingBattlegrounds(static_cast<Bracket::Id>(l_BracketId), TEAM_ALLIANCE, l_AvgAlliance);
                AllocateGroupsInExistingBattlegrounds(static_cast<Bracket::Id>(l_BracketId), TEAM_HORDE, l_AvgHorde);

                /// Take a decision on what battleground can start from now.
                std::vector<float> l_NumPlayersByBGTypes(BattlegroundType::Max * 2);
                std::vector<std::list<GroupQueueInfo*>> l_PotentialGroups(BattlegroundType::Max * 2);

                /// We sort according to the join time because we want older groups to join the most rapidly.
                m_QueuedGroups[l_BracketId][TEAM_ALLIANCE].sort([](GroupQueueInfo const* p_G1, GroupQueueInfo const* p_G2)
                {
                    return p_G1->m_JoinTime < p_G2->m_JoinTime;
                });
                m_QueuedGroups[l_BracketId][TEAM_HORDE].sort([](GroupQueueInfo const* p_G1, GroupQueueInfo const* p_G2)
                {
                    return p_G1->m_JoinTime < p_G2->m_JoinTime;
                });

                std::ostringstream l_BattlegroundDump;

                /// Find the potential battlegrounds.
                FindPotentialBGs(static_cast<Bracket::Id>(l_BracketId), l_NumPlayersByBGTypes, l_PotentialGroups);

                /// We clone our array of occurrences.
                std::array<std::pair<float, std::size_t>, BattlegroundType::Max> l_Occurences;
                for (std::size_t i = 0; i < BattlegroundType::Max; i++)
                    l_Occurences[i] = m_BattlegroundOccurences[l_BracketId][i];

                /// We sort our battleground occurrences.
                std::sort(std::begin(l_Occurences), std::end(l_Occurences), [](std::pair<float, std::size_t> const& p_A, std::pair<float, std::size_t> const& p_B)
                {
                    return p_A.first < p_B.first;
                });

                /// We iterate over the sorted occurrences to take a decision if possible.
                for (std::size_t i = 0; i < BattlegroundType::Max; i++)
                {
                    BattlegroundType::Type l_BGType = static_cast<BattlegroundType::Type>(l_Occurences[i].second);

                    /// If testing flag is on, we take the first one.
                    if (sBattlegroundMgr->isTesting() && (!l_PotentialGroups[l_BGType * 2 + TEAM_ALLIANCE].empty() || !l_PotentialGroups[l_BGType * 2 + TEAM_HORDE].empty()))
                    {
                        if (TryCreateBattleground(l_BGType, l_PotentialGroups, l_BracketId))
                            break;
                    }

                    /// If we are on a casual battleground, we want to check the occurrences and ratios.
                    if (BattlegroundType::IsCasualBattleground(l_BGType))
                    {
                        /// We get the battleground template.
                        Battleground* l_Template = sBattlegroundMgr->GetBattlegroundTemplate(l_BGType);
                        if (!l_Template)
                            continue;

                        /// We check if the battleground can start.
                        if (l_NumPlayersByBGTypes[l_BGType * 2 + TEAM_ALLIANCE] < l_Template->GetMinPlayersPerTeam() || l_NumPlayersByBGTypes[l_BGType * 2 + TEAM_HORDE] < l_Template->GetMinPlayersPerTeam())
                            continue;

                        /// If the actual ratio in the battleground is good enough and we are not making too much instances of this battleground, we choose it.
                        float l_Ratio = std::abs(1 - l_NumPlayersByBGTypes[l_BGType * 2 + TEAM_ALLIANCE] / l_NumPlayersByBGTypes[l_BGType * 2 + TEAM_HORDE]);
                        if (l_Ratio < 0.15f && l_Occurences[i].first < ((1.0f / BattlegroundType::NumBattlegrounds) + 0.05f))
                        {
                            if (TryCreateBattleground(l_BGType, l_PotentialGroups, l_BracketId))
                                break;
                        }
                    }
                    /// If we are not caring about factions so we start.
                    else if (BattlegroundType::IsArena(l_BGType))
                    {
                        /// We check if the number of players for each arena type is filled.
                        if (l_NumPlayersByBGTypes[l_BGType * 2 + TEAM_ALLIANCE] < BattlegroundType::GetArenaType(l_BGType))
                            continue;

                        if (l_NumPlayersByBGTypes[l_BGType * 2 + TEAM_HORDE] < BattlegroundType::GetArenaType(l_BGType))
                            continue;

                        if (TryCreateBattleground(l_BGType, l_PotentialGroups, l_BracketId))
                            break;
                    }
                    /// From here there should only be rated battlegrounds.
                    else if (BattlegroundType::IsRated(l_BGType))
                    {
                        switch (l_BGType)
                        {
                        case BattlegroundType::RatedBg10v10:
                            if (l_NumPlayersByBGTypes[l_BGType * 2 + TEAM_ALLIANCE] < 10 || l_NumPlayersByBGTypes[l_BGType * 2 + TEAM_HORDE] < 10)
                                continue;
                            break;
                        }

                        if (TryCreateBattleground(l_BGType, l_PotentialGroups, l_BracketId))
                            break;
                    }
                }
            }
        }

        bool BattlegroundScheduler::TryCreateBattleground(BattlegroundType::Type p_DecidedBg, std::vector<std::list<GroupQueueInfo*>>& p_PotentialGroups, std::size_t p_BracketId)
        {
            /// We check this because of the testing flag.
            if (p_PotentialGroups[p_DecidedBg * 2 + TEAM_ALLIANCE].empty() && p_PotentialGroups[p_DecidedBg * 2 + TEAM_HORDE].empty())
                return false;

            if (BattlegroundType::IsCasualBattleground(p_DecidedBg))
            {
                /// We get the battleground template.
                Battleground* l_Template = sBattlegroundMgr->GetBattlegroundTemplate(p_DecidedBg);
                if (!l_Template)
                    return false;

                /// Create the new battleground.
                Battleground* l_Bg = sBattlegroundMgr->CreateNewBattleground(p_DecidedBg, Brackets::RetreiveFromId(p_BracketId), 0, false);

                auto& l_TotalOccurences = m_TotalOccurences[p_BracketId];

                /// Update stats.
                l_TotalOccurences++;

                for (std::size_t i = 0; i < BattlegroundType::Max; i++)
                {
                    float l_OldFrequency = m_BattlegroundOccurences[p_BracketId][i].first; ///< l_OldFrequency is unused
                    float l_OldNumProc = m_BattlegroundOccurences[p_BracketId][i].first / (1.0f / (std::max((int32)(l_TotalOccurences - 1), (int32)1)));

                    if (i == p_DecidedBg)
                        l_OldNumProc += 1.0f;

                    m_BattlegroundOccurences[p_BracketId][i].first = l_OldNumProc / l_TotalOccurences;
                }

                /// Add groups to the battleground and remove them from waiting groups list.
                for (std::size_t i = TEAM_ALLIANCE; i <= TEAM_HORDE; i++)
                {
                    for (GroupQueueInfo* l_Group : p_PotentialGroups[p_DecidedBg * 2 + i])
                    {
                        RemoveGroupFromQueues(l_Group);
                        AddToBG(l_Group, l_Bg);
                    }
                }

                l_Bg->StartBattleground();
                return true;
            }
            else if (BattlegroundType::IsRated(p_DecidedBg))
            {
                // We only need to check one kind of faction list because they are merged when finding the potential bgs.
                auto& l_Groups = p_PotentialGroups[p_DecidedBg * 2];

                /// We sort them according to their MMR.
                l_Groups.sort([](GroupQueueInfo const* p_A, GroupQueueInfo const* p_B)
                {
                    return p_A->m_ArenaMatchmakerRating - p_B->m_ArenaMatchmakerRating;
                });

                GroupQueueInfo* l_Previous = nullptr;
                /// We iterate over pairs of groups and check if they match according to the MatchMaking Rating.
                for (GroupQueueInfo* l_Group : l_Groups)
                {
                    /// We check if their MMR are matching.
                    if (l_Previous && AreMatching(l_Previous, l_Group))
                    {
                        /// Players can't decide on which kind of battleground there are playing, so it's a basic random.
                        BattlegroundType::Type l_RatedBg = BattlegroundType::None;
                        if (BattlegroundType::IsArena(p_DecidedBg))
                            l_RatedBg = static_cast<BattlegroundType::Type>(urand(BattlegroundType::TigersPeaks, BattlegroundType::NagrandArena));
                        else
                            l_RatedBg = static_cast<BattlegroundType::Type>(urand(BattlegroundType::Warsong, BattlegroundType::BattleForGilneas));

                        bool l_IsRatedBattleground = !BattlegroundType::IsArena(p_DecidedBg);   ///< We talk about rated battleground here, not rated arena!

                        /// Create the new battleground.
                        Battleground* l_Bg = sBattlegroundMgr->CreateNewBattleground(l_RatedBg, Brackets::RetreiveFromId(p_BracketId), BattlegroundType::GetArenaType(p_DecidedBg), false, false, false, l_IsRatedBattleground);
                        if (l_Bg == nullptr)
                            return false;

                        /// Add groups to the battleground and remove them from waiting groups list.
                        RemoveGroupFromQueues(l_Group);
                        AddToBG(l_Group, l_Bg, ALLIANCE);
                        RemoveGroupFromQueues(l_Previous);
                        AddToBG(l_Previous, l_Bg, HORDE);

                        l_Bg->StartBattleground();

                        l_Previous = nullptr;
                        return true;
                    }

                    l_Previous = l_Group;
                }

                return false;
            }
            /// When this happened, there can't be any rated battlegrounds and it should only be skirmish arenas.
            else if (BattlegroundType::IsArena(p_DecidedBg))
            {
                BattlegroundType::Type l_Arena = static_cast<BattlegroundType::Type>(urand(BattlegroundType::TigersPeaks, BattlegroundType::NagrandArena));

                /// We get the battleground template.
                Battleground* l_Template = sBattlegroundMgr->GetBattlegroundTemplate(l_Arena);
                if (!l_Template)
                    return false;

                /// Create the new battleground.
                Battleground* l_Bg = sBattlegroundMgr->CreateNewBattleground(l_Arena, Brackets::RetreiveFromId(p_BracketId), BattlegroundType::GetArenaType(p_DecidedBg), true);

                /// Add groups to the battleground and remove them from waiting groups list.
                for (std::size_t i = TEAM_ALLIANCE; i <= TEAM_HORDE; i++)
                {
                    for (GroupQueueInfo* l_Group : p_PotentialGroups[p_DecidedBg * 2 + i])
                    {
                        RemoveGroupFromQueues(l_Group);
                        AddToBG(l_Group, l_Bg, i == TEAM_ALLIANCE ? ALLIANCE : HORDE);
                    }
                }

                l_Bg->StartBattleground();
                return true;
            }

            return false;
        }
#pragma endregion

    } ///< namespace Battlegrounds.
} ///< namespace MS.
