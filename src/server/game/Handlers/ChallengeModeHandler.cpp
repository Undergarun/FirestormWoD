////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "World.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Log.h"
#include "Opcodes.h"
#include "Player.h"

#include <WowTime.hpp>

void WorldSession::HandleGetChallengeModeRewards(WorldPacket& /*p_RecvData*/)
{
    WorldPacket l_Data(SMSG_CHALLENGE_MOD_REWARDS);
    ChallengeRewardsMap l_ChallengeRewards = sObjectMgr->GetChallengeRewards();

    l_Data << uint32(l_ChallengeRewards.size());
    l_Data << uint32(0);   ///< TierRewards

    ///< RewardsCount
    {
        for (auto l_Reward : l_ChallengeRewards)
        {
            uint32 l_RewardsPerMedalCount = 4;

            l_Data << int32(l_Reward.first);
            l_Data << uint32(l_RewardsPerMedalCount);

            ChallengeReward l_ChallengeReward = l_Reward.second;
            for (uint32 l_J = 0; l_J < l_RewardsPerMedalCount; ++l_J)
            {
                uint32 l_ItemRewards = 0;
                uint32 l_CurrencyRewards = 0;

                l_Data << uint32(l_ItemRewards);
                l_Data << uint32(l_CurrencyRewards);
                l_Data << uint32(l_ChallengeReward.MoneyReward[l_J]);
            }
        }
    }

    ///< Anything else is needed to be sent
    SendPacket(&l_Data);
}

void WorldSession::HandleChallengeModeRequestLeaders(WorldPacket& p_RecvData)
{
    int32 l_MapID = p_RecvData.read<int32>();
    p_RecvData.read_skip<int32>();
    p_RecvData.read_skip<int32>();

    RealmCompletedChallenge* l_GroupChallenge = sObjectMgr->GetGroupCompletedChallengeForMap(l_MapID);
    RealmCompletedChallenge* l_GuildChallenge = sObjectMgr->GetGuildCompletedChallengeForMap(l_MapID);

    WorldPacket l_Data(SMSG_CHALLENGE_MODE_REQUEST_LEADERS_RESULT, 300);

    l_Data << int32(l_MapID);
    l_Data << int32(time(NULL));
    l_Data << int32(time(NULL));

    uint32 l_RealmLeaders = l_GroupChallenge != nullptr;
    uint32 l_GuildLeaders = 0;

    if (l_GuildChallenge != nullptr && l_GuildChallenge->m_GuildID == m_Player->GetGuildId())
        l_GuildLeaders = 1;

    l_Data << uint32(l_RealmLeaders);
    l_Data << uint32(l_GuildLeaders);

    ///< RealmLeaders
    if (l_RealmLeaders > 0)
    {
        l_Data << uint32(g_RealmID);
        l_Data << uint32(l_GroupChallenge->m_AttemptID);
        l_Data << int32(l_GroupChallenge->m_CompletionTime);
        l_Data << uint32(MS::Utilities::WowTime::Encode(l_GroupChallenge->m_CompletionDate));
        l_Data << int32(l_GroupChallenge->m_MedalEarned);

        uint32 l_Members = l_GroupChallenge->m_MembersCount;
        l_Data << uint32(l_Members);

        for (uint32 l_J = 0; l_J < l_Members; ++l_J)
        {
            l_Data << uint32(g_RealmID);
            l_Data << uint32(g_RealmID);
            l_Data.appendPackGUID(l_GroupChallenge->m_Members[l_J].m_Guid);
            l_Data << int32(l_GroupChallenge->m_Members[l_J].m_SpecID);
        }
    }

    ///< GuildLeaders
    if (l_GuildLeaders > 0)
    {
        l_Data << uint32(g_RealmID);
        l_Data << uint32(l_GuildChallenge->m_AttemptID);
        l_Data << int32(l_GuildChallenge->m_CompletionTime);
        l_Data << uint32(MS::Utilities::WowTime::Encode(l_GuildChallenge->m_CompletionDate));
        l_Data << int32(l_GuildChallenge->m_MedalEarned);

        uint32 l_Members = l_GuildChallenge->m_MembersCount;
        l_Data << uint32(l_Members);

        for (uint32 l_J = 0; l_J < l_Members; ++l_J)
        {
            l_Data << uint32(g_RealmID);
            l_Data << uint32(g_RealmID);
            l_Data.appendPackGUID(l_GuildChallenge->m_Members[l_J].m_Guid);
            l_Data << int32(l_GuildChallenge->m_Members[l_J].m_SpecID);
        }
    }

    SendPacket(&l_Data);
}

void WorldSession::HandleChallengeModeRequestMapStats(WorldPacket& /*p_RecvData*/)
{
    uint32 l_MapsCount = m_Player->m_CompletedChallenges.size();
    WorldPacket l_Data(SMSG_CHALLENGE_MODE_ALL_MAP_STATS);

    l_Data << uint32(l_MapsCount);

    ///< MapData
    if (l_MapsCount > 0)
    {
        for (auto l_ChallengeData : m_Player->m_CompletedChallenges)
        {
            CompletedChallenge l_CompletedChallenge = l_ChallengeData.second;

            l_Data << int32(l_ChallengeData.first);
            l_Data << int32(l_CompletedChallenge.m_BestTime);
            l_Data << int32(l_CompletedChallenge.m_LastTime);
            l_Data << int32(l_CompletedChallenge.m_BestMedal);
            l_Data << uint32(MS::Utilities::WowTime::Encode(l_CompletedChallenge.m_BestMedalDate));

            uint32 l_SpecCount = 0;
            l_Data << uint32(l_SpecCount);

            /// BestSpecs - Useless part ?
            {
                for (uint32 l_J = 0; l_J < l_SpecCount; ++l_J)
                    l_Data << uint16(0);    ///< SpecID
            }
        }
    }

    SendPacket(&l_Data);
}

void WorldSession::SendChallengeModeMapStatsUpdate(uint32 p_MapID)
{
    WorldPacket l_Data(Opcodes::SMSG_CHALLENGE_MODE_MAP_STATS_UPDATE, 6 * 4);

    for (auto l_ChallengeData : m_Player->m_CompletedChallenges)
    {
        if (l_ChallengeData.first == p_MapID)
        {
            CompletedChallenge l_CompletedChallenge = l_ChallengeData.second;

            l_Data << int32(l_ChallengeData.first);
            l_Data << int32(l_CompletedChallenge.m_BestTime);
            l_Data << int32(l_CompletedChallenge.m_LastTime);
            l_Data << int32(l_CompletedChallenge.m_BestMedal);
            l_Data << uint32(MS::Utilities::WowTime::Encode(l_CompletedChallenge.m_BestMedalDate));

            uint32 l_SpecCount = 0;
            l_Data << uint32(l_SpecCount);

            /// BestSpecs - Useless part ?
            {
                for (uint32 l_J = 0; l_J < l_SpecCount; ++l_J)
                    l_Data << uint16(0);    ///< SpecID
            }
        }
    }

    SendPacket(&l_Data);
}
