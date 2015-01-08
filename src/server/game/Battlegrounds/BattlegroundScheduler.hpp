#ifndef BATTLEGROUND_SCHEDULER_HPP
# define BATTLEGROUND_SCHEDULER_HPP

# include "Battleground.h"

namespace MS
{
    namespace Battlegrounds
    {
        namespace BattlegroundMasks
        {
            enum
            {
                AlteracValley = 0x1,
                Warsong = 0x2,
                ArathiBassin = 0x4,
                EyeOfTheStorm = 0x8,
                StrandOfTheAncients = 0x10,
                IsleOfConquest = 0x20,
                TwinPeaks = 0x40,
                BattleForGilneas = 0x80,
                KotmoguTemple = 0x100,
                SilvershardMines = 0x200,
                DeepwindGorge = 0x400,
                /// Arenas.
                Arena2v2 = 0x800,
                Arena3v3 = 0x1000,
                Arena5v5 = 0x2000,
                ArenaSkirmish2v2 = 0x4000,
                ArenaSkirmish3v3 = 0x8000,
                All = 0xFFFF
            };
        }

        class BattlegroundScheduler
        {
            using QueuedPlayersMap = ACE_Based::LockedMap<uint64, PlayerQueueInfo>;

        public:
            /// Constructor.
            BattlegroundScheduler();

            /// Methods.

            /// Feed the system with a new group.
            GroupQueueInfo* AddGroup(Player* p_Leader, Group* p_Group, BattlegroundType::Type p_BgTypeId, MS::Battlegrounds::Bracket const*  p_BracketEntry, uint8 p_ArenaType, bool p_IsRatedBG, bool p_IsPremade, uint32 p_ArenaRating, uint32 p_MatchmakerRating, bool p_IsSkirmish);

            /// Add to BG is called when group finally accepted the invitation.
            void AddToBG(GroupQueueInfo* p_Group, Battleground* p_BG);

            void RemovePlayer(uint64 p_Guid);

            bool GetPlayerGroupInfoData(uint64 p_Guid, GroupQueueInfo& p_GroupInfo) const;

            //////////////////////////////////////////////////////////////////////////
            /// Scheduling part.
            //////////////////////////////////////////////////////////////////////////
            /// Try to make matches with the queued groups.
            void FindMatches();

        private:
            /// Allocates the groups in the existing battlegrounds depending on different criteria and respecting eligibility.
            void AllocateGroupsInExistingBattlegrounds(Bracket::Id p_BracketId, std::size_t p_Team, std::vector<std::pair<float, std::size_t>>& p_Avg);

            /// Takes inventory of the wishes of the groups in order to make decisions.
            void FindPotentialBGs(Bracket::Id p_BracketId, std::vector<float>& p_PotientialBGs, std::vector<std::list<GroupQueueInfo*>>& p_PotentialGroups);

            /// Remove the group and its players from the queues.
            void RemoveGroupFromQueues(GroupQueueInfo* p_Group);

        private:
            std::list<Battleground*> m_Battlegrounds[Brackets::Count][BattlegroundType::Max];
            std::list<GroupQueueInfo*> m_QueuedGroups[Brackets::Count][2];
            std::size_t m_NumInstances;
            std::pair<float, std::size_t> m_BattlegroundOccurences[Brackets::Count][BattlegroundType::Max];
            std::size_t m_TotalOccurences[Brackets::Count];
            QueuedPlayersMap m_QueuedPlayers;
        };
    }
}

#endif /// !BATTLEGROUND_SCHEDUlER_HPP