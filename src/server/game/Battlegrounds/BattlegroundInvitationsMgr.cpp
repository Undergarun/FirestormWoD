////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014 Millenium-studio SARL
//  All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////// 

#include "BattlegroundInvitationsMgr.hpp"
#include "BattlegroundMgr.hpp"
#include "BattlegroundPacketFactory.hpp"

namespace MS
{
    namespace Battlegrounds
    {
        BattlegroundInvitationsMgr::BattlegroundInvitationsMgr()
        {
            for (uint32 i = 0; i < TeamsCount::Value; ++i)
            {
                for (uint32 j = 0; j < Brackets::Count; ++j)
                {
                    m_SumOfWaitTimes[i][j] = 0;
                    m_WaitTimeLastPlayer[i][j] = 0;
                    for (uint32 k = 0; k < COUNT_OF_PLAYERS_TO_AVERAGE_WAIT_TIME; ++k)
                        m_WaitTimes[i][j][k] = 0;
                }
            }
        }

        bool BattlegroundInvitationsMgr::InviteGroupToBG(GroupQueueInfo* p_GroupInfo, Battleground* p_Bg, uint32 p_Side)
        {
            /// Set side if needed.
            if (p_Side)
                p_GroupInfo->m_Team = p_Side;

            if (!p_GroupInfo->m_IsInvitedToBGInstanceGUID)
            {
                /// Not yet invited, set invitation.
                p_GroupInfo->m_IsInvitedToBGInstanceGUID = p_Bg->GetInstanceID();
                BattlegroundTypeId l_BGTypeId = p_Bg->GetTypeID();
                BattlegroundType::Type l_BgQueueTypeId = GetTypeFromId(l_BGTypeId, p_Bg->GetArenaType(), p_GroupInfo->m_IsSkirmish);
                Bracket::Id l_BracketId = p_Bg->GetBracketId();

                p_GroupInfo->m_RemoveInviteTime = getMSTime() + INVITE_ACCEPT_WAIT_TIME;

                /// Loop through the players and send them the inviting packet.
                for (auto l_Itr = std::begin(p_GroupInfo->m_Players); l_Itr != std::end(p_GroupInfo->m_Players); ++l_Itr)
                {
                    /// Get the player.
                    Player* l_Player = ObjectAccessor::FindPlayer(l_Itr->first);

                    /// If offline, skip him, this should not happen - player is removed from queue when he logs out.
                    if (!l_Player)
                        continue;

                    /// Insert player in the invited map players.
                    PlayerQueueInfo& l_PlayerQueue = m_InvitedPlayers[l_Player->GetGUID()];
                    l_PlayerQueue.Infos.emplace_back(PlayerQueueInfo::Pair{ getMSTime(), p_GroupInfo });

                    /// Invite the player.
                    UpdateAverageWaitTimeForGroup(p_GroupInfo, l_BracketId);

                    /// Set invited player counters.
                    p_Bg->IncreaseInvitedCount(p_GroupInfo->m_Team);

                    l_Player->SetInviteForBattlegroundQueueType(p_GroupInfo->m_BgTypeId, p_GroupInfo->m_IsInvitedToBGInstanceGUID);

                    /// Create remind invite news.
                    //BGQueueInviteEvent* l_InviteEvent = new BGQueueInviteEvent(l_Player->GetGUID(), p_GroupInfo->m_IsInvitedToBGInstanceGUID, l_BGTypeId, p_GroupInfo->m_ArenaType, p_GroupInfo->m_RemoveInviteTime);
                    //m_Events.AddEvent(l_InviteEvent, m_Events.CalculateTime(INVITATION_REMIND_TIME));

                    /// Create automatic remove events.
                    BGQueueRemoveEvent* l_RemoveEvent = new BGQueueRemoveEvent(l_Player->GetGUID(), p_GroupInfo->m_IsInvitedToBGInstanceGUID, l_BGTypeId, l_BgQueueTypeId, p_GroupInfo->m_RemoveInviteTime);
                    l_Player->m_Events.AddEvent(l_RemoveEvent, l_Player->m_Events.CalculateTime(INVITE_ACCEPT_WAIT_TIME));

                    uint32 l_QueueSlot = l_Player->GetBattlegroundQueueIndex(p_GroupInfo->m_BgTypeId == BattlegroundType::RandomBattleground ? BattlegroundType::RandomBattleground : p_GroupInfo->m_BgTypeId);

                    sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: invited player %s (%u) to BG instance %u queueindex %u bgtype %u, I can't help it if they don't press the enter battle button.",
                        l_Player->GetName(), l_Player->GetGUIDLow(), p_Bg->GetInstanceID(), l_QueueSlot, p_Bg->GetTypeID());

                    /// Send status packet.
                    WorldPacket data;
                    PacketFactory::Status(&data, p_Bg, l_Player, l_QueueSlot, STATUS_WAIT_JOIN, p_Bg->GetExpirationDate(), l_Player->GetBattlegroundQueueJoinTime(p_GroupInfo->m_BgTypeId), p_GroupInfo->m_ArenaType, p_GroupInfo->m_IsSkirmish);
                    l_Player->GetSession()->SendPacket(&data);
                }

                return true;
            }

            return false;
        }

        void BattlegroundInvitationsMgr::UpdateAverageWaitTimeForGroup(GroupQueueInfo* p_GroupInfo, Bracket::Id p_BracketId)
        {
            uint32 l_TimeInQueue = getMSTimeDiff(p_GroupInfo->m_JoinTime, getMSTime());
            uint8 l_TeamIndex = BG_TEAM_ALLIANCE; /// Default set to BG_TEAM_ALLIANCE - or non rated arenas!

            if (!p_GroupInfo->m_ArenaType)
            {
                if (p_GroupInfo->m_Team == HORDE)
                    l_TeamIndex = BG_TEAM_HORDE;
            }
            else
            {
                if (p_GroupInfo->m_IsRatedBG)
                    l_TeamIndex = BG_TEAM_HORDE; /// For rated arenas use BG_TEAM_HORDE.
            }

            if (l_TeamIndex >= TeamsCount::Value || p_BracketId >= Brackets::Count)
                sLog->outAshran("BattlegroundInvitationsMgr: TeamIndex: %u, BracketId: %u", l_TeamIndex, p_BracketId);

            /// Store pointer to arrayindex of player that was added first.
            uint32* l_LastPlayerAddedPointer = &(m_WaitTimeLastPlayer[l_TeamIndex][p_BracketId]);

            /// Remove his sum of sum.
            m_SumOfWaitTimes[l_TeamIndex][p_BracketId] -= m_WaitTimes[l_TeamIndex][p_BracketId][(*l_LastPlayerAddedPointer)];

            /// Set average time to new.
            m_WaitTimes[l_TeamIndex][p_BracketId][(*l_LastPlayerAddedPointer)] = l_TimeInQueue;

            /// Add new time to sum.
            m_SumOfWaitTimes[l_TeamIndex][p_BracketId] += l_TimeInQueue;

            /// Set index of last player added to next one.
            (*l_LastPlayerAddedPointer)++;
            (*l_LastPlayerAddedPointer) %= COUNT_OF_PLAYERS_TO_AVERAGE_WAIT_TIME;
        }

        uint32 BattlegroundInvitationsMgr::GetAverageQueueWaitTime(GroupQueueInfo* p_GroupInfo, Bracket::Id p_BracketId) const
        {
            /// Default set to BG_TEAM_ALLIANCE - or non rated arenas!
            uint8 l_TeamIndex = BG_TEAM_ALLIANCE;

            if (!p_GroupInfo->m_ArenaType)
            {
                if (p_GroupInfo->m_Team == HORDE)
                    l_TeamIndex = BG_TEAM_HORDE;
            }
            else
            {
                if (p_GroupInfo->m_IsRatedBG)
                    l_TeamIndex = BG_TEAM_HORDE; /// For rated arenas use BG_TEAM_HORDE.
            }

            /// Check if there is enough values (we always add values > 0).
            if (m_WaitTimes[l_TeamIndex][p_BracketId][COUNT_OF_PLAYERS_TO_AVERAGE_WAIT_TIME - 1])
                return (m_SumOfWaitTimes[l_TeamIndex][p_BracketId] / COUNT_OF_PLAYERS_TO_AVERAGE_WAIT_TIME);

            /// If there aren't enough values return 0 - not available.
            return 0;
        }

        bool BattlegroundInvitationsMgr::IsOwningPlayer(uint64 p_Guid) const
        {
            return m_InvitedPlayers.find(p_Guid) != std::end(m_InvitedPlayers);
        }

        bool BattlegroundInvitationsMgr::IsPlayerInvited(uint64 p_PlrGuid, uint32 p_BgInstanceId, uint32 p_RemoveTime) const
        {
            QueuedPlayersMap::const_iterator l_Itr = m_InvitedPlayers.find(p_PlrGuid);

            if (l_Itr != std::end(m_InvitedPlayers))
            {
                for (auto l_Pair = l_Itr->second.Infos.begin(); l_Pair != l_Itr->second.Infos.end(); l_Pair++)
                {
                    GroupQueueInfo* l_Group = l_Pair->GroupInfo;
                    if (l_Group->m_IsInvitedToBGInstanceGUID == p_BgInstanceId && l_Group->m_RemoveInviteTime == p_RemoveTime)
                        return true;
                }
            }

            return false;
        }

        bool BattlegroundInvitationsMgr::GetPlayerGroupInfoData(uint64 p_Guid, GroupQueueInfo& p_GroupInfo, BattlegroundType::Type p_Type) const
        {
            auto l_Itr = m_InvitedPlayers.find(p_Guid);
            if (l_Itr == m_InvitedPlayers.end())
                return false;

            for (auto l_Pair = l_Itr->second.Infos.begin(); l_Pair != l_Itr->second.Infos.end(); l_Pair++)
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

        void BattlegroundInvitationsMgr::UpdateEvents(uint32 diff)
        {
            m_Events.Update(diff);
        }

        void BattlegroundInvitationsMgr::RemovePlayer(uint64 p_Guid, bool p_DecreaseInvitedCount, BattlegroundType::Type p_Type)
        {
            /// Remove player from map, if he's there.
            auto l_Itr = m_InvitedPlayers.find(p_Guid);
            if (l_Itr == m_InvitedPlayers.end())
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

            if (!l_Group)
                return;

            int32 l_BracketId = l_Group->m_BracketId;

            /// Player can't be in queue without group, but just in case.
            if (l_BracketId == -1)
            {
                sLog->outError(LOG_FILTER_BATTLEGROUND, "BattlegroundQueue: ERROR Cannot find groupinfo for player GUID: %u", GUID_LOPART(p_Guid));
                return;
            }
            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BattlegroundQueue: Removing player GUID %u, from bracket_id %u", GUID_LOPART(p_Guid), (uint32)l_BracketId);

            /// ALL variables are correctly set
            /// We can ignore leveling up in queue - it should not cause crash
            /// remove player from group
            /// if only one player there, remove group

            /// Remove player queue info from group queue info.
            auto pitr = l_Group->m_Players.find(p_Guid);
            if (pitr != l_Group->m_Players.end())
                l_Group->m_Players.erase(pitr);

            /// Remove player queue info.
            if (l_Itr->second.Infos.size() == 1)
                m_InvitedPlayers.erase(l_Itr);
            else
                l_Itr->second.Infos.erase(l_Pair);

            Battleground* l_BG = sBattlegroundMgr->GetBattleground(l_Group->m_IsInvitedToBGInstanceGUID, GetTypeFromId(GetIdFromType(l_Group->m_BgTypeId), l_Group->m_ArenaType, l_Group->m_IsSkirmish));
            if (l_BG)
                l_BG->DecreaseInvitedCount(l_Group->m_Team);

            /// If player leaves queue and he is invited to rated arena match, then he have to lose.
            if (l_Group->m_IsInvitedToBGInstanceGUID && l_Group->m_IsRatedBG && IsArena(l_Group->m_BgTypeId) && p_DecreaseInvitedCount)
            {
                if (Player* player = ObjectAccessor::FindPlayer(p_Guid))
                {
                    /// Only when the player refuse to enter
                    if (MS::Battlegrounds::GetTypeFromId(player->GetBattlegroundTypeId(), l_Group->m_ArenaType, l_Group->m_IsSkirmish) != p_Type)
                    {
                        /// Update personal rating.
                        uint8 slot = Arena::GetSlotByType(l_Group->m_ArenaType);
                        int32 mod = Arena::GetRatingMod(player->GetArenaPersonalRating(slot), l_Group->m_OpponentsMatchmakerRating, false);
                        player->SetArenaPersonalRating(slot, std::max(0, static_cast<int>(player->GetArenaPersonalRating(slot) + mod)));

                        /// Update matchmaker rating.
                        player->SetArenaMatchMakerRating(slot, std::max(0, static_cast<int>(player->GetArenaMatchMakerRating(slot) - 12)));

                        /// Update personal played stats.
                        player->IncrementWeekGames(slot);
                        player->IncrementSeasonGames(slot);
                    }
                }
            }

            /// Remove group queue info if needed.
            if (l_Group->m_Players.empty())
            {
                delete l_Group;
            }
            /// if group wasn't empty, so it wasn't deleted, and player have left a rated
            /// queue -> everyone from the group should leave too
            /// don't remove recursively if already invited to bg!
            else if (!l_Group->m_IsInvitedToBGInstanceGUID && l_Group->m_IsRatedBG)
            {
                BattlegroundType::Type bgQueueTypeId = GetTypeFromId(GetIdFromType(l_Group->m_BgTypeId), l_Group->m_ArenaType);
                /// Remove next player, this is recursive.
                /// First send removal information.
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

                /// Then actually delete, this may delete the group as well!
                RemovePlayer(l_Group->m_Players.begin()->first, p_DecreaseInvitedCount, bgQueueTypeId);
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// Battleground Queue Events
        //////////////////////////////////////////////////////////////////////////

        bool BGQueueInviteEvent::Execute(uint64 /*e_time*/, uint32 /*p_time*/)
        {
            Player* l_Player = ObjectAccessor::FindPlayer(m_PlayerGuid);
            /// Player logged off (we should do nothing, he is correctly removed from queue in another procedure).
            if (!l_Player)
                return true;

            Battleground* l_Bg = sBattlegroundMgr->GetBattleground(m_BgInstanceGUID, GetSchedulerType(m_BgTypeId));
            /// If battleground ended and its instance deleted - do nothing.
            if (!l_Bg)
                return true;

            BattlegroundType::Type bgQueueTypeId = GetTypeFromId(l_Bg->GetTypeID(), l_Bg->GetArenaType(), l_Bg->IsSkirmish());
            uint32 l_QueueSlot = l_Player->GetBattlegroundQueueIndex(GetSchedulerType(l_Bg->GetTypeID()));
            if (l_QueueSlot < PLAYER_MAX_BATTLEGROUND_QUEUES)         // player is in queue or in battleground
            {
                /// Check if the player is invited to this battleground.
                BattlegroundInvitationsMgr& l_InvitationsMgr = sBattlegroundMgr->GetInvitationsMgr();
                if (l_InvitationsMgr.IsPlayerInvited(m_PlayerGuid, m_BgInstanceGUID, m_RemoveTime))
                {
                    WorldPacket l_Data;

                    /// We must send remaining time in queue.
                    if (BattlegroundType::IsArena(bgQueueTypeId))
                        PacketFactory::Status(&l_Data, l_Bg, l_Player, l_QueueSlot, STATUS_WAIT_JOIN, l_Bg->GetExpirationDate(), l_Player->GetBattlegroundQueueJoinTime(bgQueueTypeId), m_ArenaType, l_Bg->IsSkirmish());
                    
                    l_Player->GetSession()->SendPacket(&l_Data);
                }
            }
            return true; /// Event will be deleted.
        }

        void BGQueueInviteEvent::Abort(uint64 /*p_Time*/)
        {
            /// Do nothing.
        }

        /// This event has many possibilities when it is executed:
        /// 1. player is in battleground (he clicked enter on invitation window)
        /// 2. player left battleground queue and he isn't there any more
        /// 3. player left battleground queue and he joined it again and IsInvitedToBGInstanceGUID = 0
        /// 4. player left queue and he joined again and he has been invited to same battleground again -> we should not remove him from queue yet
        /// 5. player is invited to bg and he didn't choose what to do and timer expired - only in this condition we should call queue::RemovePlayer
        /// we must remove player in the 5. case even if battleground object doesn't exist!
        bool BGQueueRemoveEvent::Execute(uint64 /*p_Time*/, uint32 /*p_Time*/)
        {
            sLog->outAshran("BGQueueRemoveEvent::Execute m_PlayerGuid: %u, m_BgInstanceGUID : %u, m_BgTypeId : %u, m_BgType : %u",
                m_PlayerGuid, m_BgInstanceGUID, m_BgTypeId, m_BgType);

            Player* l_Player = ObjectAccessor::FindPlayer(m_PlayerGuid);

            if (!l_Player) /// Player logged off (we should do nothing, he is correctly removed from queue in another procedure)
                return true;

            Battleground* l_Bg = sBattlegroundMgr->GetBattleground(m_BgInstanceGUID, GetSchedulerType(m_BgTypeId));
            /// Battleground can be deleted already when we are removing queue info
            /// Bg pointer can be NULL! so use it carefully!

            //sLog->outAshran("BGQueueRemoveEvent::Execute 1");

            uint32 l_QueueSlot = l_Player->GetBattlegroundQueueIndex(m_BgType);
            if (l_QueueSlot < PLAYER_MAX_BATTLEGROUND_QUEUES) /// Player is in queue, or in Battleground.
            {
                //sLog->outAshran("BGQueueRemoveEvent::Execute 2");

                /// Check if player is in queue for this BG and if we are removing his invite event.
                BattlegroundInvitationsMgr& l_InvitationsMgr = sBattlegroundMgr->GetInvitationsMgr();
                if (l_InvitationsMgr.IsPlayerInvited(m_PlayerGuid, m_BgInstanceGUID, m_RemoveTime))
                {
                    //sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Battleground: removing player %u from bg queue for instance %u because of not pressing enter battle in time.", l_Player->GetGUIDLow(), m_BgInstanceGUID);
                    //sLog->outAshran("BGQueueRemoveEvent::Execute 3");

                    l_Player->RemoveBattlegroundQueueId(m_BgType);
                    l_InvitationsMgr.RemovePlayer(m_PlayerGuid, true, Battlegrounds::GetSchedulerType(m_BgTypeId));
                    //sLog->outAshran("BGQueueRemoveEvent::Execute 4");


                    WorldPacket l_Data;
                    PacketFactory::Status(&l_Data, l_Bg, l_Player, l_QueueSlot, STATUS_NONE, l_Player->GetBattlegroundQueueJoinTime(GetSchedulerType(m_BgTypeId)), 0, 0, false);
                    l_Player->GetSession()->SendPacket(&l_Data);

                    //sLog->outAshran("BGQueueRemoveEvent::Execute 5");
                }
            }

            return true; /// Event will be deleted.
        }

        void BGQueueRemoveEvent::Abort(uint64 /*e_time*/)
        {
            /// Do nothing.
        }
    } ///< namespace Battlegrounds.
} ///< namespace MS
