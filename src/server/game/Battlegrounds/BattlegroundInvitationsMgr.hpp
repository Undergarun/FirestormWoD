////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef BATTLEGROUND_INVITATIONS_MGR_HPP
# define BATTLEGROUND_INVITATIONS_MGR_HPP

#include "Define.h"
#include "Common.h"
#include "Battleground.h"

namespace MS
{
    namespace Battlegrounds
    {
        class BattlegroundInvitationsMgr
        {
            using QueuedPlayersMap = ACE_Based::LockedMap<uint64, PlayerQueueInfo>;

        public:
            /// Constructor.
            BattlegroundInvitationsMgr();

            /// Invite the group to a battleground.
            /// @p_GroupInfo : The group to invite.
            /// @p_Bg : The battleground to invite to.
            /// @p_Side : The faction of the group.
            bool InviteGroupToBG(GroupQueueInfo* p_GroupInfo, Battleground* p_Bg, uint32 p_Side);

            /// Update the average time for the group.
            /// @p_GroupInfo : The group.
            /// @p_BracketId : The bracketId corresponding to the level of the group.
            void UpdateAverageWaitTimeForGroup(GroupQueueInfo* p_GroupInfo, Bracket::Id p_BracketId);

            /// Get the average wait time in the current queue.
            /// @p_GroupInfo : The informations of the current group.
            /// @p_BracketId : BracketId corresponding to the level of the group.
            uint32 GetAverageQueueWaitTime(GroupQueueInfo* p_GroupInfo, Bracket::Id p_BracketId) const;

            /// Returns true when the player is in queue and is invited to the Battleground.
            /// @p_PlrGuid : The guid of the player.
            /// @p_BgInstanceId : The id of the battleground.
            /// @p_RemoveTime : ??????
            bool IsPlayerInvited(uint64 p_PlrGuid, uint32 p_BgInstanceId, uint32 p_RemoveTime) const;

            /// Returns true if the invitations manager is owning the player.
            /// @p_Guid : The guid of the player.
            bool IsOwningPlayer(uint64 p_Guid) const;

            /// Update the events registered.
            /// @p_Diff : The difference of time with the old update.
            void UpdateEvents(uint32 p_Diff);

            /// Remove the player from the invited list and from the GroupQueueInfo.
            /// If the GroupQueueInfo is then empty, we remove it too.
            /// @p_Guid : The player.
            /// @p_DecreaseInvitedCount : If set to true, decrease the invitation count in the battleground.
            /// @p_Type : The type of the battleground where the player wanted to go.
            void RemovePlayer(uint64 p_Guid, bool p_DecreaseInvitedCount, BattlegroundType::Type p_Type);

            /// Remove all invitation of one player from the invitation manager
            /// Used at player logout
            /// @p_Guid : The player
            void ClearPlayerInvitation(uint64 p_Guid);

            /// Retrieve the GroupQueueInfo structure from the invited list.
            /// @p_Guid : Guid of the player.
            /// @p_GroupInfo : The group structure to fill.
            /// @p_Type : The type that corresponds to the wishes of the group.
            bool GetPlayerGroupInfoData(uint64 p_Guid, GroupQueueInfo& p_GroupInfo, BattlegroundType::Type p_Type) const;
        
        private:
            EventProcessor m_Events;                                                                        ///< Events handler.
            uint32 m_WaitTimes[TeamsCount::Value][Brackets::Count][COUNT_OF_PLAYERS_TO_AVERAGE_WAIT_TIME];  ///< The average time to wait.
            uint32 m_WaitTimeLastPlayer[TeamsCount::Value][Brackets::Count];                                ///< ?????
            uint32 m_SumOfWaitTimes[TeamsCount::Value][Brackets::Count];                                    ///< The sum of the wait times.
            QueuedPlayersMap m_InvitedPlayers;                                                              ///< The players owned by the invitations manager that we are waiting for enter battleground.
        };

        /// This class is used to invite player to BG again, when minute lasts from his first invitation
        /// it is capable to solve all possibilities.
        class BGQueueInviteEvent : public BasicEvent
        {
            public:
                BGQueueInviteEvent(uint64 pl_guid, uint32 BgInstanceGUID, BattlegroundTypeId BgTypeId, uint8 arenaType, uint32 removeTime)
                    : BasicEvent(), m_PlayerGuid(pl_guid), m_BgInstanceGUID(BgInstanceGUID), m_BgTypeId(BgTypeId), m_ArenaType(arenaType), m_RemoveTime(removeTime) {}

                virtual ~BGQueueInviteEvent() {};

                virtual bool Execute(uint64 e_time, uint32 p_time);
                virtual void Abort(uint64 e_time);

            private:
                uint64 m_PlayerGuid;
                uint32 m_BgInstanceGUID;
                BattlegroundTypeId m_BgTypeId;
                uint8  m_ArenaType;
                uint32 m_RemoveTime;
        };

        /// This class is used to remove player from BG queue after 1 minute 20 seconds from first invitation
        /// We must store removeInvite time in case player left queue and joined and is invited again
        /// We must store bgQueueTypeId, because battleground can be deleted already, when player entered it
        class BGQueueRemoveEvent : public BasicEvent
        {
            public:
                BGQueueRemoveEvent(uint64 pl_guid, uint32 bgInstanceGUID, BattlegroundTypeId BgTypeId, BattlegroundType::Type p_BgType, uint32 removeTime)
                    : BasicEvent(), m_PlayerGuid(pl_guid), m_BgInstanceGUID(bgInstanceGUID), m_RemoveTime(removeTime), m_BgTypeId(BgTypeId), m_BgType(p_BgType) {}

                virtual ~BGQueueRemoveEvent() {}

                virtual bool Execute(uint64 e_time, uint32 p_time);
                virtual void Abort(uint64 e_time);

            private:
                uint64 m_PlayerGuid;
                uint32 m_BgInstanceGUID;
                uint32 m_RemoveTime;
                BattlegroundTypeId m_BgTypeId;
                BattlegroundType::Type m_BgType;
        };
    } ///< namespace Battlegrounds.
} ///< namespace MS.

#endif /// !BATTLEGROUND_INVITATIONS_MGR_HPP
