#include "BattlegroundScheduler.hpp"
#include "BattlegroundMgr.h"
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
            static bool IsEligibleForBattleground(GroupQueueInfo const* p_Group, std::size_t p_BattlegroundMask, Bracket::Id p_BracketId)
            {
                if (p_Group->m_BracketId != p_BracketId)
                    return false;

                if (p_Group->m_IsSkirmish)
                {
                    if (p_Group->m_ArenaType == ArenaType::Arena2v2 && p_BattlegroundMask & BattlegroundMasks::ArenaSkirmish2v2)
                        return true;
                    if (p_Group->m_ArenaType == ArenaType::Arena3v3 && p_BattlegroundMask & BattlegroundMasks::ArenaSkirmish3v3)
                        return true;
                }

                if (p_Group->m_IsRatedBG)
                {
                    if (p_Group->m_ArenaType == ArenaType::Arena2v2 && p_BattlegroundMask & BattlegroundMasks::Arena2v2)
                        return true;
                    if (p_Group->m_ArenaType == ArenaType::Arena3v3 && p_BattlegroundMask & BattlegroundMasks::Arena3v3)
                        return true;
                    if (p_Group->m_ArenaType == ArenaType::Arena5v5 && p_BattlegroundMask & BattlegroundMasks::Arena5v5)
                        return true;
                }

                return (p_Group->m_WantedBGs & p_BattlegroundMask) != 0;
            }

            /// Utility function for calculation average.
            static float AddAndRecalculateAvg(float p_Avg, float p_Value, float p_Norm)
            {
                return ((p_Avg * p_Norm) + p_Value) / p_Norm;
            }

        }

        BattlegroundScheduler::BattlegroundScheduler()
            : m_Battlegrounds(),
            m_QueuedGroups(),
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

        void BattlegroundScheduler::AddToBG(GroupQueueInfo* p_Group, Battleground* p_BG)
        {
            /// Now that the group leave the queue, we can restrict to instanciable battlegrounds.
            for (auto const& l_Itr : p_Group->m_Players)
            {
                /// Get the player.
                Player* l_Player = ObjectAccessor::FindPlayer(l_Itr.first);

                /// If offline, skip him, this should not happen - player is removed from queue when he logs out.
                if (!l_Player)
                    continue;

                l_Player->SetBattlegroundQueueTypeId(l_Player->GetBattlegroundQueueIndex(p_Group->m_BgTypeId), GetSchedulerType(p_BG->GetTypeID()));
            }
            p_Group->m_BgTypeId = GetSchedulerType(p_BG->GetTypeID());

            /// Move group to invitations mgr and invite them to the battleground.
            sBattlegroundMgr->GetInvitationsMgr().InviteGroupToBG(p_Group, p_BG, 0);
        }

        GroupQueueInfo* BattlegroundScheduler::AddGroup(Player* p_Leader, Group* p_Group, BattlegroundType::Type p_BgTypeId, uint32 p_BlackWishes[2], MS::Battlegrounds::Bracket const*  p_BracketEntry, uint8 p_ArenaType, bool p_IsRatedBG, bool p_IsPremade, uint32 p_ArenaRating, uint32 p_MatchmakerRating, bool p_IsSkirmish)
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

            /// We specify the wishes of the group.
            switch (p_BgTypeId)
            {
            case BattlegroundType::RandomBattleground:
                l_GroupQueue->m_WantedBGs = BattlegroundMasks::All;
                break;
            case BattlegroundType::AllArenas:
                l_GroupQueue->m_WantedBGs = BattlegroundMasks::AllArenas;
                break;
            default:
                l_GroupQueue->m_WantedBGs = 1 << p_BgTypeId;
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

                    if (l_GroupQueue->m_WantedBGs & (1 << l_Type))
                        l_GroupQueue->m_WantedBGs &= ~(1 << l_Type);
                }
            }

            l_GroupQueue->m_Players.clear();

            /// Compute index (if group is premade or joined a rated match) to queues.
            uint32 l_Index = 0;
            if (!p_IsRatedBG && !p_IsPremade)
                l_Index += BG_TEAMS_COUNT;
            if (l_GroupQueue->m_Team == HORDE)
                l_Index++;

            uint32 l_LastOnlineTime = getMSTime();

            /// Add players from group to GroupQueueInfo.
            {
                if (p_Group)
                {
                    for (GroupReference* l_Iterator = p_Group->GetFirstMember(); l_Iterator != NULL; l_Iterator = l_Iterator->next())
                    {
                        Player* l_Member = l_Iterator->getSource();
                        if (!l_Member)
                            continue;

                        /// Create the PlayerQueueInfo.
                        PlayerQueueInfo& l_PlayerQueue = m_QueuedPlayers[l_Member->GetGUID()];
                        l_PlayerQueue.LastOnlineTime = l_LastOnlineTime;
                        l_PlayerQueue.GroupInfo = l_GroupQueue;

                        l_GroupQueue->m_Players[l_Member->GetGUID()] = &l_PlayerQueue;
                    }
                }
                else
                {
                    PlayerQueueInfo& l_PlayerQueue = m_QueuedPlayers[p_Leader->GetGUID()];
                    l_PlayerQueue.LastOnlineTime = l_LastOnlineTime;
                    l_PlayerQueue.GroupInfo = l_GroupQueue;

                    l_GroupQueue->m_Players[p_Leader->GetGUID()] = &l_PlayerQueue;
                }
            }

            /// Add the GroupQueueInfo in the groups to match.
            m_QueuedGroups[l_BracketId][l_GroupQueue->GetTeam()].emplace_back(l_GroupQueue);

            return l_GroupQueue;
        }

        void BattlegroundScheduler::RemovePlayer(uint64 p_Guid)
        {
            int32 l_BracketId = -1;                                     // signed for proper for-loop finish

            /// Remove player from map, if he's there.
            auto l_Itr = m_QueuedPlayers.find(p_Guid);
            if (l_Itr == m_QueuedPlayers.end())
                return;

            GroupQueueInfo* l_Group = l_Itr->second.GroupInfo;

            //player can't be in queue without group, but just in case
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
            m_QueuedPlayers.erase(l_Itr);

            // remove group queue info if needed
            if (l_Group->m_Players.empty())
            {
                m_QueuedGroups[l_BracketId][l_Group->GetTeam()].remove(l_Group);
                delete l_Group;
            }

            // if group wasn't empty, so it wasn't deleted, and player have left a rated
            // queue -> everyone from the group should leave too
            // don't remove recursively if already invited to bg!
            else if (!l_Group->m_IsInvitedToBGInstanceGUID && l_Group->m_IsRatedBG)
            {
                // remove next player, this is recursive
                // first send removal information
                if (Player* plr2 = ObjectAccessor::FindPlayer(l_Group->m_Players.begin()->first))
                {
                    Battleground* bg = sBattlegroundMgr->GetBattlegroundTemplate(l_Group->m_BgTypeId);
                    BattlegroundType::Type bgQueueTypeId = GetTypeFromId(GetIdFromType(l_Group->m_BgTypeId), l_Group->m_ArenaType);
                    uint32 queueSlot = plr2->GetBattlegroundQueueIndex(bgQueueTypeId);
                    plr2->RemoveBattlegroundQueueId(bgQueueTypeId); // must be called this way, because if you move this call to
                    // queue->removeplayer, it causes bugs
                    WorldPacket data;
                    PacketFactory::Status(&data, bg, plr2, queueSlot, STATUS_NONE, plr2->GetBattlegroundQueueJoinTime(l_Group->m_BgTypeId), 0, 0, false);
                    plr2->GetSession()->SendPacket(&data);
                }
                // then actually delete, this may delete the group as well!
                RemovePlayer(l_Group->m_Players.begin()->first);
            }
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

            /// Insert groups in their categories.
            for (GroupQueueInfo* l_Group : m_QueuedGroups[p_BracketId][p_Team])
            {
                /// We first sort the averages and keep track of the indexes.
                std::sort(std::begin(p_Avg), std::end(p_Avg), [](std::pair<float, std::size_t> const& p_A, std::pair<float, std::size_t> const& p_B) {
                    return p_A.first < p_B.first;
                });

                bool l_AreFull = true;
                for (std::size_t l_Fallback = 0; l_Fallback < BattlegroundType::Max; l_Fallback++)
                {
                    std::size_t l_BgType = p_Avg[l_Fallback].second;

                    /// We get the battleground template.
                    Battleground* l_Template = sBattlegroundMgr->GetBattlegroundTemplate(static_cast<BattlegroundType::Type>(l_BgType));
                    if (!l_Template)
                        continue;

                    /// We check if the group is eligible for the battleground.
                    if (!IsEligibleForBattleground(l_Group, 1 << l_BgType, p_BracketId))
                        continue;

                    /// We sort according to the lowest ratio of players for the specific team.
                    m_Battlegrounds[p_BracketId][l_BgType].sort([p_Team, l_BgType, l_Template](Battleground* p_A, Battleground* p_B) {
                        return p_A->GetPlayersCountByTeam(p_Team == TEAM_ALLIANCE ? ALLIANCE : HORDE) / l_Template->GetMaxPlayers()
                            - p_B->GetPlayersCountByTeam(p_Team == TEAM_ALLIANCE ? ALLIANCE : HORDE) / l_Template->GetMaxPlayers();
                    });

                    for (Battleground* l_Bg : m_Battlegrounds[p_BracketId][l_BgType])
                    {
                        /// We check if the number of invited/playing players and the size of this group doesn't exceed the max allowed.
                        if (l_Bg->CanGroupEnter(l_Group))
                        {
                            l_AreFull = true;
                            AddToBG(l_Group, l_Bg);
                            l_ToRemove.emplace_back(l_Group);
                            p_Avg[l_Fallback].first = AddAndRecalculateAvg(p_Avg[l_Fallback].first, l_Group->m_Players.size(), m_Battlegrounds[p_BracketId][l_BgType].size() * l_Template->GetMaxPlayers());
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
                        /// We check if the group is eligible for the arena.
                        if (!IsEligibleForBattleground(l_Group, 1 << (i / 2), p_BracketId))
                            continue;

                        if (BattlegroundType::IsCasualBattleground(static_cast<BattlegroundType::Type>(i / 2)))
                        {
                            /// We get the battleground template.
                            Battleground* l_Template = sBattlegroundMgr->GetBattlegroundTemplate(static_cast<BattlegroundType::Type>(i / 2));
                            if (!l_Template)
                                continue;

                            /// We check if the number of players doesn't exceed the max number of players in the battleground.
                            if (p_PotientialBGs[i + l_Team] + l_Group->m_Players.size() > l_Template->GetMaxPlayers())
                                continue;

                            p_PotientialBGs[i + l_Team] += l_Group->m_Players.size();
                            p_PotentialGroups[i + l_Team].emplace_back(l_Group);
                        }
                        else if (BattlegroundType::IsArena(static_cast<BattlegroundType::Type>(i / 2)))
                        {
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
                    /// We get the battleground template.
                    Battleground* l_Template = sBattlegroundMgr->GetBattlegroundTemplate(static_cast<BattlegroundType::Type>(i));
                    if (!l_Template)
                        continue;

                    // What is the average filling of each battleground type ?
                    for (Battleground* l_BG : m_Battlegrounds[l_BracketId][i])
                    {
                        l_AvgHorde[i].first += l_BG->GetPlayersCountByTeam(HORDE);
                        l_AvgAlliance[i].first += l_BG->GetPlayersCountByTeam(ALLIANCE);
                    }

                    if (!m_Battlegrounds[l_BracketId][i].empty())
                    {
                        l_AvgHorde[i].first /= m_Battlegrounds[l_BracketId][i].size() * l_Template->GetMaxPlayers();
                        l_AvgAlliance[i].first /= m_Battlegrounds[l_BracketId][i].size() * l_Template->GetMaxPlayers();
                    }
                }

                /// Sort averages and keep track of indexes.
                std::sort(std::begin(l_AvgAlliance), std::end(l_AvgAlliance), [](std::pair<float, std::size_t> const& p_A, std::pair<float, std::size_t> const& p_B) {
                    return p_A.first < p_B.first;
                });
                std::sort(std::begin(l_AvgHorde), std::end(l_AvgHorde), [](std::pair<float, std::size_t> const& p_A, std::pair<float, std::size_t> const& p_B) {
                    return p_A.first < p_B.first;
                });

                /// We insert groups in existing battlegrounds.
                AllocateGroupsInExistingBattlegrounds(static_cast<Bracket::Id>(l_BracketId), TEAM_ALLIANCE, l_AvgAlliance);
                AllocateGroupsInExistingBattlegrounds(static_cast<Bracket::Id>(l_BracketId), TEAM_HORDE, l_AvgHorde);

                /// Take a decision on what battleground can start from now.
                std::vector<float> l_NumPlayersByBGTypes(BattlegroundType::Max * 2);
                std::vector<std::list<GroupQueueInfo*>> l_PotentialGroups(BattlegroundType::Max * 2);

                /// We sort according to the join time because we want older groups to join the most rapidly.
                m_QueuedGroups[l_BracketId][TEAM_ALLIANCE].sort([](GroupQueueInfo const* p_G1, GroupQueueInfo const* p_G2) {
                    return p_G1->m_JoinTime < p_G2->m_JoinTime;
                });
                m_QueuedGroups[l_BracketId][TEAM_HORDE].sort([](GroupQueueInfo const* p_G1, GroupQueueInfo const* p_G2) {
                    return p_G1->m_JoinTime < p_G2->m_JoinTime;
                });

                int l_BestType = 0;
                while (l_BestType >= 0)
                {
                    /// Find the potential battlegrounds.
                    FindPotentialBGs(static_cast<Bracket::Id>(l_BracketId), l_NumPlayersByBGTypes, l_PotentialGroups);

                    /// We clone our array of occurrences.
                    std::pair<float, std::size_t> l_Occurences[BattlegroundType::Max];
                    for (std::size_t i = 0; i < BattlegroundType::Max; i++)
                        l_Occurences[i] = m_BattlegroundOccurences[l_BracketId][i];

                    /// We sort our battleground occurrences.
                    std::sort(std::begin(l_Occurences), std::end(l_Occurences), [](std::pair<float, std::size_t> const& p_A, std::pair<float, std::size_t> const& p_B) {
                        return p_A.first < p_B.first;
                    });

                    l_BestType = -1;

                    /// TODO: Make special cases for arenas and rated bg.
                    for (std::size_t i = 0; i < BattlegroundType::Max; i++)
                    {
                        BattlegroundType::Type l_BGType = static_cast<BattlegroundType::Type>(l_Occurences[i].second);

                        /// If testing flag is on, we take the first one.
                        if (sBattlegroundMgr->isTesting() && (!l_PotentialGroups[l_BGType * 2 + TEAM_ALLIANCE].empty() || !l_PotentialGroups[l_BGType * 2 + TEAM_HORDE].empty()))
                        {
                            l_BestType = l_BGType;
                            break;
                        }

                        if (BattlegroundType::IsCasualBattleground(l_BGType))
                        {
                            /// We get the battleground template.
                            Battleground* l_Template = sBattlegroundMgr->GetBattlegroundTemplate(l_BGType);
                            if (!l_Template)
                                continue;

                            /// We check if the battleground can start.
                            if (l_NumPlayersByBGTypes[l_BGType * 2 + TEAM_ALLIANCE] < l_Template->GetMinPlayers() || l_NumPlayersByBGTypes[l_BGType * 2 + TEAM_HORDE] < l_Template->GetMinPlayers())
                                continue;

                            /// If the actual ratio in the battleground is good enough and we are not making too much instances of this battleground, we choose it.
                            float l_Ratio = std::abs(1 - l_NumPlayersByBGTypes[l_BGType * 2 + TEAM_ALLIANCE] / l_NumPlayersByBGTypes[l_BGType * 2 + TEAM_HORDE]);
                            if (l_Ratio < 0.15f && l_Occurences[i].first < (1.0f / BattlegroundType::NumBattlegrounds) + 0.05f)
                            {
                                l_BestType = l_BGType;
                                break;
                            }
                        }
                        /// If we are not caring about factions so we start.
                        else if (BattlegroundType::IsArena(l_BGType))
                        {
                            /// We check if the number of players for each arena type is filled.
                            switch (BattlegroundType::GetArenaType(l_BGType))
                            {
                            case ArenaType::Arena2v2:
                                if (l_NumPlayersByBGTypes[l_BGType * 2 + TEAM_ALLIANCE] < 2)
                                    continue;
                            case ArenaType::Arena3v3:
                                if (l_NumPlayersByBGTypes[l_BGType * 2 + TEAM_ALLIANCE] < 3)
                                    continue;
                            case ArenaType::Arena5v5:
                                if (l_NumPlayersByBGTypes[l_BGType * 2 + TEAM_ALLIANCE] < 5)
                                    continue;
                                break;
                            }

                            l_BestType = l_BGType;
                            break;
                        }
                    }

                    if (l_BestType >= 0)
                    {
                        /// We check this because of the testing flag.
                        if (l_PotentialGroups[l_BestType * 2 + TEAM_ALLIANCE].empty() && l_PotentialGroups[l_BestType * 2 + TEAM_HORDE].empty())
                            continue;

                        BattlegroundType::Type l_DecidedBg = static_cast<BattlegroundType::Type>(l_BestType);

                        if (BattlegroundType::IsCasualBattleground(l_DecidedBg))
                        {
                            /// We get the battleground template.
                            Battleground* l_Template = sBattlegroundMgr->GetBattlegroundTemplate(l_DecidedBg);
                            if (!l_Template)
                                continue;

                            /// Create the new battleground.
                            Battleground* l_Bg = sBattlegroundMgr->CreateNewBattleground(l_DecidedBg, Brackets::RetreiveFromId(l_BracketId), 0, false);

                            auto& l_TotalOccurences = m_TotalOccurences[l_BracketId];

                            /// Update stats.
                            l_TotalOccurences++;
                            m_BattlegroundOccurences[l_BracketId][l_DecidedBg].first = (m_BattlegroundOccurences[l_BracketId][l_DecidedBg].first * (l_TotalOccurences - 1) + 1) / l_TotalOccurences;

                            for (std::size_t i = 0; i < BattlegroundType::Max; i++)
                            {
                                if (i == l_DecidedBg)
                                    continue;

                                m_BattlegroundOccurences[l_BracketId][i].first = (m_BattlegroundOccurences[l_BracketId][i].first * (l_TotalOccurences - 1)) / l_TotalOccurences;
                            }

                            /// Add groups to the battleground and remove them from waiting groups list.
                            for (std::size_t i = TEAM_ALLIANCE; i <= TEAM_HORDE; i++)
                            {
                                for (GroupQueueInfo* l_Group : l_PotentialGroups[l_DecidedBg * 2 + i])
                                {
                                    AddToBG(l_Group, l_Bg);
                                    RemoveGroupFromQueues(l_Group);
                                }
                            }
                        }
                        else if (BattlegroundType::IsArena(static_cast<BattlegroundType::Type>(l_BestType)))
                        {
                            /// Players can't decide on which kind of arenas there are playing, so it's a basic random.
                            BattlegroundType::Type l_ArenaBg = static_cast<BattlegroundType::Type>(urand(BattlegroundType::TigersPeaks, BattlegroundType::TheRingOfValor));

                            /// We get the battleground template.
                            Battleground* l_Template = sBattlegroundMgr->GetBattlegroundTemplate(l_ArenaBg);
                            if (!l_Template)
                                continue;

                            /// Create the new battleground.
                            Battleground* l_Bg = sBattlegroundMgr->CreateNewBattleground(l_ArenaBg, Brackets::RetreiveFromId(l_BracketId), BattlegroundType::GetArenaType(l_DecidedBg), BattlegroundType::IsSkirmish(l_DecidedBg));

                            /// Add groups to the battleground and remove them from waiting groups list.
                            /// There is only one kind of faction in Arena so we don't take care of the potential groups in horde side.
                            for (GroupQueueInfo* l_Group : l_PotentialGroups[l_DecidedBg * 2])
                            {
                                AddToBG(l_Group, l_Bg);
                                RemoveGroupFromQueues(l_Group);
                            }
                        }
                    }
                }
            }
#pragma endregion
        }
    }
}