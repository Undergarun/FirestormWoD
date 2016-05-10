////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014 Millenium-studio SARL
//  All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////// 

#ifndef BATTLEGROUND_SCHEDULER_HPP
# define BATTLEGROUND_SCHEDULER_HPP

# include "Battleground.h"

namespace MS
{
    namespace Battlegrounds
    {
        namespace BattlegroundMasks
        {
            /// The orders should be the same as in BattlegroundType::Type enum.
            enum
            {
                AlteracValley = 0x1,
                Warsong = 0x2,
                ArathiBassin = 0x4,
                EyeOfTheStorm = 0x8,
                TwinPeaks = 0x10,
                BattleForGilneas = 0x20,
                StrandOfTheAncients = 0x40,
                IsleOfConquest = 0x80,
                KotmoguTemple = 0x100,
                SilvershardMines = 0x200,
                DeepwindGorge = 0x400,
                AllBattlegrounds = 0x7FF,
                /// Arenas.
                Arena2v2 = 0x800,
                Arena3v3 = 0x1000,
                Arena5v5 = 0x2000,
                AllArenas = Arena2v2 | Arena3v3 | Arena5v5,
                /// Skirmish Arenas.
                ArenaSkirmish2v2 = 0x4000,
                ArenaSkirmish3v3 = 0x8000,
                AllSkirmishArenas = ArenaSkirmish2v2 | ArenaSkirmish3v3,
                All = 0xFFFF
            };
        }

        class BattlegroundScheduler
        {
            using QueuedPlayersMap = ACE_Based::LockedMap<uint64, PlayerQueueInfo>;

        public:
            /// Constructor.
            BattlegroundScheduler();

            /// Constructs a GroupQueueInfo and queues it in the scheduler.
            /// @p_Leader           : The player who is leading the group.
            /// @p_Group            : The group of players.
            /// @p_BgTypeId         : The type of battlegrounds wished.
            /// @p_BlackWishes      : The battlegrounds the group doesn't want.
            /// @p_BracketEntry     : The level bracket of the group.
            /// @p_ArenaType        : The arena type wished.
            /// @p_IsRated          : True if the group wants rated battlegrounds.
            /// @p_ArenaRating      : The arena rating of the group.
            /// @p_MatchmakerRating : The MMR of the group.
            /// @p_IsSkirmish       : True if the group wants a skirmish arena.
            GroupQueueInfo* AddGroup(Player* p_Leader,
                Group* p_Group,
                BattlegroundType::Type p_BgTypeId,
                uint32 p_BlackWishes[2],
                Bracket const* p_BracketEntry,
                ArenaType p_ArenaType,
                bool p_IsRatedBG,
                uint32 p_ArenaRating,
                uint32 p_MatchmakerRating,
                bool p_IsSkirmish);

            /// Add to battleground is called when group finally accepted the invitation.
            /// @p_Group    : The group.
            /// @p_BG       : The battleground.
            /// @p_Team     : The team of the group (default: 0).
            void AddToBG(GroupQueueInfo* p_Group, Battleground* p_BG, uint32 p_Team = 0);

            /// Remove the player associated the GUID given.
            /// @p_Guid : The guid of the player.
            /// @p_Type : The type of wishes the player wanted
            void RemovePlayer(uint64 p_Guid, BattlegroundType::Type p_Type);

            /// Retreive the GroupQueueInfo structure from the queued list.
            /// @p_Guid : Guid of the player.
            /// @p_GroupInfo : The group structure to fill.
            /// @p_Type : The type that corresponds to the wishes of the group.
            bool GetPlayerGroupInfoData(uint64 p_Guid, GroupQueueInfo& p_GroupInfo, BattlegroundType::Type p_Type) const;

            /// Remove the group and its players from the queues.
            /// @p_Group : The group.
            void RemoveGroupFromQueues(GroupQueueInfo* p_Group);

            //////////////////////////////////////////////////////////////////////////
            /// Scheduling part.
            //////////////////////////////////////////////////////////////////////////

            /// Try to make matches with the queued groups.
            void FindMatches();

            /// Try to create a battleground
            bool TryCreateBattleground(BattlegroundType::Type p_DecidedBg, std::vector<std::list<GroupQueueInfo*>>& p_PotentialGroups, std::size_t p_BracketId);

        private:
            /// Allocates the groups in the existing battlegrounds depending on different criteria and respecting eligibility.
            /// @p_BracketId    : The bracket id.
            /// @p_Team         : The team to look after.
            /// @p_Avg          : The average vector fill.
            void AllocateGroupsInExistingBattlegrounds(Bracket::Id p_BracketId, std::size_t p_Team, std::vector<std::pair<float, std::size_t>>& p_Avg);

            /// Takes inventory of the wishes of the groups in order to make decisions.
            /// @p_BracketId        : The bracket id.
            /// @p_PotentialBGs     : The count of players inside the different battlegrounds.
            /// @p_PotentialGroups  : The vector of eligible groups.
            void FindPotentialBGs(Bracket::Id p_BracketId, std::vector<float>& p_PotientialBGs, std::vector<std::list<GroupQueueInfo*>>& p_PotentialGroups);

        private:
            std::list<GroupQueueInfo*> m_QueuedGroups[Brackets::Count][2];                                  ///< The queue of groups.
            std::pair<float, std::size_t> m_BattlegroundOccurences[Brackets::Count][BattlegroundType::Max]; ///< The occurrences of battlegrounds during runtime.
            std::size_t m_TotalOccurences[Brackets::Count];                                                 ///< The total number of occurences during runtime.
            QueuedPlayersMap m_QueuedPlayers;                                                               ///< The queue of players that are in the groups.
        };
    } ///< namespace Battlegrounds.
} ///< namespace MS.

#endif /// !BATTLEGROUND_SCHEDUlER_HPP
