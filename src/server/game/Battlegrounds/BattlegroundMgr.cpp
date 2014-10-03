/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "Common.h"
#include "ObjectMgr.h"
#include "World.h"
#include "WorldPacket.h"

#include "Arena.h"
#include "BattlegroundMgr.h"
#include "BattlegroundAV.h"
#include "BattlegroundAB.h"
#include "BattlegroundEY.h"
#include "BattlegroundWS.h"
#include "BattlegroundNA.h"
#include "BattlegroundBE.h"
#include "BattlegroundAA.h"
#include "BattlegroundRL.h"
#include "BattlegroundSA.h"
#include "BattlegroundDS.h"
#include "BattlegroundRV.h"
#include "BattlegroundIC.h"
#include "BattlegroundRB.h"
#include "BattlegroundRBG.h"
#include "BattlegroundTP.h"
#include "BattlegroundBFG.h"
#include "BattlegroundKT.h"
#include "BattlegroundSSM.h"
#include "BattlegroundTV.h"
#include "BattlegroundTTP.h"
#include "Chat.h"
#include "Map.h"
#include "MapInstanced.h"
#include "MapManager.h"
#include "Player.h"
#include "GameEventMgr.h"
#include "SharedDefines.h"
#include "Formulas.h"
#include "DisableMgr.h"
#include "LFG.h"

/*********************************************************/
/***            BATTLEGROUND MANAGER                   ***/
/*********************************************************/

BattlegroundMgr::BattlegroundMgr() : m_ArenaTesting(false)
{
    for (uint32 i = BATTLEGROUND_TYPE_NONE; i < MAX_BATTLEGROUND_TYPE_ID; i++)
        m_Battlegrounds[i].clear();
    m_NextRatedArenaUpdate = sWorld->getIntConfig(CONFIG_ARENA_RATED_UPDATE_TIMER);
    m_Testing=false;
}

BattlegroundMgr::~BattlegroundMgr()
{
    DeleteAllBattlegrounds();
}

void BattlegroundMgr::DeleteAllBattlegrounds()
{
    for (uint32 i = BATTLEGROUND_TYPE_NONE; i < MAX_BATTLEGROUND_TYPE_ID; ++i)
    {
        for (BattlegroundSet::iterator itr = m_Battlegrounds[i].begin(); itr != m_Battlegrounds[i].end();)
        {
            Battleground* bg = itr->second;
            m_Battlegrounds[i].erase(itr++);
            if (!m_ClientBattlegroundIds[i][bg->GetBracketId()].empty())
                m_ClientBattlegroundIds[i][bg->GetBracketId()].erase(bg->GetClientInstanceID());
            delete bg;
        }
    }

    // destroy template battlegrounds that listed only in queues (other already terminated)
    for (uint32 bgTypeId = 0; bgTypeId < MAX_BATTLEGROUND_TYPE_ID; ++bgTypeId)
    {
        // ~Battleground call unregistring BG from queue
        while (!BGFreeSlotQueue[bgTypeId].empty())
            delete BGFreeSlotQueue[bgTypeId].front();
    }
}

// used to update running battlegrounds, and delete finished ones
void BattlegroundMgr::Update(uint32 diff)
{
    BattlegroundSet::iterator itr, next;
    for (uint32 i = BATTLEGROUND_TYPE_NONE; i < MAX_BATTLEGROUND_TYPE_ID; ++i)
    {
        itr = m_Battlegrounds[i].begin();
        // skip updating battleground template
        if (itr != m_Battlegrounds[i].end())
            ++itr;
        for (; itr != m_Battlegrounds[i].end(); itr = next)
        {
            next = itr;
            ++next;
            itr->second->Update(diff);
            // use the SetDeleteThis variable
            // direct deletion caused crashes
            if (itr->second->ToBeDeleted())
            {
                Battleground* bg = itr->second;
                m_Battlegrounds[i].erase(itr);
                if (!m_ClientBattlegroundIds[i][bg->GetBracketId()].empty())
                    m_ClientBattlegroundIds[i][bg->GetBracketId()].erase(bg->GetClientInstanceID());

                delete bg;
            }
        }
    }

    // update events timer
    for (int qtype = BATTLEGROUND_QUEUE_NONE; qtype < MAX_BATTLEGROUND_QUEUE_TYPES; ++qtype)
        m_BattlegroundQueues[qtype].UpdateEvents(diff);

    // update scheduled queues
    if (!m_QueueUpdateScheduler.empty())
    {
        std::vector<QueueSchedulerItem*> scheduled;
        {
            //copy vector and clear the other
            scheduled = std::vector<QueueSchedulerItem*>(m_QueueUpdateScheduler);
            m_QueueUpdateScheduler.clear();
            //release lock
        }

        for (uint8 i = 0; i < scheduled.size(); i++)
        {
            uint32 arenaMMRating = scheduled[i]->_arenaMMRating;
            uint8 arenaType = scheduled[i]->_arenaType;
            BattlegroundQueueTypeId bgQueueTypeId = scheduled[i]->_bgQueueTypeId;
            BattlegroundTypeId bgTypeId = scheduled[i]->_bgTypeId;
            BattlegroundBracketId bracket_id = scheduled[i]->_bracket_id;
            m_BattlegroundQueues[bgQueueTypeId].BattlegroundQueueUpdate(diff, bgTypeId, bracket_id, arenaType, arenaMMRating > 0, arenaMMRating);
        }

        /*for (std::vector<QueueSchedulerItem*>::iterator itr = scheduled.begin();
            itr != scheduled.end(); ++itr)
            delete *itr;*/
    }

    // if rating difference counts, maybe force-update queues
    if (sWorld->getIntConfig(CONFIG_ARENA_MAX_RATING_DIFFERENCE) && sWorld->getIntConfig(CONFIG_ARENA_RATED_UPDATE_TIMER))
    {
        // it's time to force update
        if (m_NextRatedArenaUpdate < diff)
        {
            // forced update for rated arenas (scan all, but skipped non rated)
            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BattlegroundMgr: UPDATING ARENA QUEUES");
            for (int qtype = BATTLEGROUND_QUEUE_2v2; qtype <= BATTLEGROUND_QUEUE_5v5; ++qtype)
                for (int bracket = BG_BRACKET_ID_FIRST; bracket < MAX_BATTLEGROUND_BRACKETS; ++bracket)
                {
                    m_BattlegroundQueues[qtype].BattlegroundQueueUpdate(diff,
                        BATTLEGROUND_AA, BattlegroundBracketId(bracket),
                        BattlegroundMgr::BGArenaType(BattlegroundQueueTypeId(qtype)), false, 0);

                }

            m_NextRatedArenaUpdate = sWorld->getIntConfig(CONFIG_ARENA_RATED_UPDATE_TIMER);
        }
        else
            m_NextRatedArenaUpdate -= diff;
    }
}

void BattlegroundMgr::BuildBattlegroundStatusPacket(WorldPacket* p_Data, Battleground* l_BG, Player * pPlayer, uint8 p_QueueSlot, uint8 StatusID, uint32 Time1, uint32 Time2, uint8 arenatype, uint8 uiFrame)
{
    /// we can be in 2 queues in same time...
    if (!l_BG)
        StatusID = STATUS_NONE;

    uint64 l_PlayerGuid = pPlayer->GetGUID();
    uint64 l_BGQueueID = l_BG ? l_BG->GetGUID() : 0;

    switch (StatusID)
    {
        case STATUS_NONE:
        {
            p_Data->Initialize(SMSG_BATTLEFIELD_STATUS_NONE);

            p_Data->appendPackGUID(l_PlayerGuid);                                     ///< Requester Guid
            *p_Data << uint32(p_QueueSlot);                                           ///< Instance ID
            *p_Data << uint32(l_BG->isArena() ? l_BG->GetMaxPlayersPerTeam() : 1);    ///< Type
            *p_Data << uint32(Time1);                                                 ///< Time

            break;
        }
        case STATUS_WAIT_QUEUE:
        {
            p_Data->Initialize(SMSG_BATTLEFIELD_STATUS_QUEUED);

            p_Data->appendPackGUID(l_PlayerGuid);                                     ///< Requester Guid
            *p_Data << uint32(p_QueueSlot);                                           ///< Instance ID
            *p_Data << uint32(l_BG->isArena() ? l_BG->GetMaxPlayersPerTeam() : 1);    ///< Type
            *p_Data << uint32(Time2);                                                 ///< Time

            *p_Data << uint64(l_BGQueueID);                                           ///< QueueID
            *p_Data << uint8(l_BG->GetMinLevel());                                    ///< RangeMin
            *p_Data << uint8(l_BG->GetMaxLevel());                                    ///< RangeMax
            *p_Data << uint8(l_BG->isArena() ? l_BG->GetMaxPlayersPerTeam() : 1);     ///< Team Size
            *p_Data << uint32(l_BG->GetClientInstanceID());                           ///< Id
            p_Data->WriteBit(l_BG->isRated());                                        ///< Registered Match
            p_Data->WriteBit(false);                                                  ///< Tournament Rules
            p_Data->FlushBits();

            *p_Data << uint32(Time1);                                                 ///< Average Wait Time
            *p_Data << uint32(GetMSTimeDiffToNow(Time2));                             ///< Wait Time

            p_Data->WriteBit(false);                                                  ///< @TODO As Group
            p_Data->WriteBit(1);                                                      ///< Eligible For Matchmaking
            p_Data->WriteBit(false);                                                  ///< @TODO Suspended Queue
            p_Data->FlushBits();

            break;
        }
        case STATUS_WAIT_JOIN:
        {
            p_Data->Initialize(SMSG_BATTLEFIELD_STATUS_NEED_CONFIRMATION, 44);
            uint8 l_Role = pPlayer->GetBattleGroundRoles();

            p_Data->appendPackGUID(l_PlayerGuid);                                     ///< Requester Guid
            *p_Data << uint32(p_QueueSlot);                                           ///< Instance ID
            *p_Data << uint32(l_BG->isArena() ? l_BG->GetMaxPlayersPerTeam() : 1);    ///< Type
            *p_Data << uint32(Time2);                                                 ///< Time

            *p_Data << uint64(l_BGQueueID);                                           ///< QueueID
            *p_Data << uint8(l_BG->GetMinLevel());                                    ///< RangeMin
            *p_Data << uint8(l_BG->GetMaxLevel());                                    ///< RangeMax
            *p_Data << uint8(l_BG->isArena() ? l_BG->GetMaxPlayersPerTeam() : 1);     ///< Team Size
            *p_Data << uint32(l_BG->GetClientInstanceID());                           ///< Id
            p_Data->WriteBit(l_BG->isRated());                                        ///< Registered Match
            p_Data->WriteBit(false);                                                  ///< Tournament Rules
            p_Data->FlushBits();

            *p_Data << uint32(l_BG->GetMapId());                                      ///< Map ID
            *p_Data << uint32(Time1);                                                 ///< Time until closed
            
            if (l_Role == ROLE_TANK)
                *p_Data << uint8(0);
            else if (l_Role == ROLE_HEALER)
                *p_Data << uint8(1);
            else if (l_Role == ROLE_DAMAGE)
                *p_Data << uint8(2);
            else
                *p_Data << uint8(2);

            break;
        }
        case STATUS_IN_PROGRESS:
        {
            p_Data->Initialize(SMSG_BATTLEFIELD_STATUS_ACTIVE, 49);

            p_Data->appendPackGUID(l_PlayerGuid);                                     ///< Requester Guid
            *p_Data << uint32(p_QueueSlot);                                           ///< Instance ID
            *p_Data << uint32(l_BG->isArena() ? l_BG->GetMaxPlayersPerTeam() : 1);    ///< Type
            *p_Data << uint32(Time2);                                                 ///< Time

            *p_Data << uint64(l_BGQueueID);                                           ///< QueueID
            *p_Data << uint8(l_BG->GetMinLevel());                                    ///< RangeMin
            *p_Data << uint8(l_BG->GetMaxLevel());                                    ///< RangeMax
            *p_Data << uint8(l_BG->isArena() ? l_BG->GetMaxPlayersPerTeam() : 1);     ///< Team Size
            *p_Data << uint32(l_BG->GetClientInstanceID());                           ///< Id
            p_Data->WriteBit(l_BG->isRated());                                        ///< Registered Match
            p_Data->WriteBit(false);                                                  ///< Tournament Rules
            p_Data->FlushBits();

            *p_Data << uint32(l_BG->GetMapId());                                      ///< Map Id
            *p_Data << uint32(Time1);                                                 ///< Time to Close
            *p_Data << uint32(GetMSTimeDiffToNow(Time2));                             ///< Elapsed Time
            p_Data->WriteBit(pPlayer->GetBGTeam() == HORDE ? 0 : 1);                  ///< Battlefield Faction ( 0 horde, 1 alliance )
            p_Data->WriteBit(false);                                                  ///< @TODO Left early
            p_Data->FlushBits();

            break;
        }
        case STATUS_WAIT_LEAVE:
        {
            p_Data->Initialize(SMSG_BATTLEFIELD_STATUS_WAIT_FOR_GROUPS, 48);

            p_Data->appendPackGUID(l_PlayerGuid);                                     ///< Requester Guid
            *p_Data << uint32(p_QueueSlot);                                           ///< Instance ID
            *p_Data << uint32(l_BG->isArena() ? l_BG->GetMaxPlayersPerTeam() : 1);    ///< Type
            *p_Data << uint32(Time2);                                                 ///< Time

            *p_Data << uint64(l_BGQueueID);                                           ///< QueueID
            *p_Data << uint8(l_BG->GetMinLevel());                                    ///< RangeMin
            *p_Data << uint8(l_BG->GetMaxLevel());                                    ///< RangeMax
            *p_Data << uint8(l_BG->isArena() ? l_BG->GetMaxPlayersPerTeam() : 1);     ///< Team Size
            *p_Data << uint32(l_BG->GetClientInstanceID());                           ///< Id
            p_Data->WriteBit(l_BG->isRated());                                        ///< Registered Match
            p_Data->WriteBit(false);                                                  ///< Tournament Rules
            p_Data->FlushBits();

            *p_Data << uint32(l_BG->GetMapId());                                      ///< Map Id
            *p_Data << uint32(Time1);                                                 ///< Time to Close

            for (uint32 l_I = 0; l_I < BG_TEAMS_COUNT; l_I++)
            {
                *p_Data << uint8(0);                                                  ///< @TODO TotalPlayers
                *p_Data << uint8(0);                                                  ///< @TODO AwaitingPlayers
            }

            break;
        }
    }
}

void BattlegroundMgr::BuildPvpLogDataPacket(WorldPacket * p_Data, Battleground * p_BG)
{
    int32 l_PlayersCount = 0;

    uint8 l_AlliancePlayerCount = 0;
    uint8 l_HordePlayerCount    = 0;
    uint8 l_IsRated             = (p_BG->isRated() ? 1 : 0);               ///< type (normal=0/rated=1) -- ATM arena or bg, RBG NYI
    uint8 l_IsArena             = (p_BG->isArena() ? 1 : 0);               ///< Arena names
    uint8 l_IsRatedBg           = (p_BG->IsRatedBG() ? 1 : 0);

    p_Data->Initialize(SMSG_PVP_LOG_DATA);

    Battleground::BattlegroundScoreMap::const_iterator l_ScoreBeginIT = p_BG->GetPlayerScoresBegin();

    for (Battleground::BattlegroundScoreMap::const_iterator l_It = l_ScoreBeginIT; l_It != p_BG->GetPlayerScoresEnd();)
    {
        l_ScoreBeginIT = l_It++;

        if (!p_BG->IsPlayerInBattleground(l_ScoreBeginIT->first))
        {
            sLog->outError(LOG_FILTER_BATTLEGROUND, "Player " UI64FMTD " has scoreboard entry for battleground %u but is not in battleground!", l_It->first, p_BG->GetTypeID(true));
            continue;
        }

        Player* l_Player = ObjectAccessor::FindPlayer(l_ScoreBeginIT->first);

        if (!l_Player)
            continue;

        l_PlayersCount++;

        if (l_Player->GetBGTeam() == ALLIANCE)
            ++l_AlliancePlayerCount;
        else
            ++l_HordePlayerCount;
    }

    bool l_HasHonor             = !l_IsArena;
    bool l_HasRatings           = l_IsArena || l_IsRatedBg;
    bool l_HasRatingChange      = ((l_IsArena || l_IsRatedBg) && p_BG->GetStatus() == STATUS_WAIT_LEAVE);
    bool l_HasPreMatchRating    = false;
    bool l_HasPreMatchMMR       = false;
    bool l_HasMMRChange         = false;

    p_Data->WriteBit(l_HasRatings);                             ///< HaveArenaData
    p_Data->WriteBit(p_BG->GetStatus() == STATUS_WAIT_LEAVE);   ///< If Ended
    p_Data->FlushBits();

    *p_Data << uint32(l_PlayersCount);

    *p_Data << uint8(l_AlliancePlayerCount);
    *p_Data << uint8(l_HordePlayerCount);

    if (l_HasRatings)
    {
        for (int8 l_I = 0; l_I < BG_TEAMS_COUNT; --l_I)
        {
            int32 l_RatingChange = p_BG->GetArenaTeamRatingChangeByIndex(l_I);
            uint32 l_Prematch = l_RatingChange < 0 ? -l_RatingChange : 0;

            *p_Data << uint32(l_Prematch);                      ///< Rating Lost
        }

        for (int8 l_I = 0; l_I < BG_TEAMS_COUNT; --l_I)
        {
            int32 l_RatingChange = p_BG->GetArenaTeamRatingChangeByIndex(l_I);
            uint32 l_Postmatch = l_RatingChange > 0 ? l_RatingChange : 0;

            *p_Data << uint32(l_Postmatch);                    ///< Rating Lost
        }

        for (int8 l_I = 0; l_I < BG_TEAMS_COUNT; --l_I)
        {
            uint32 l_PrematchMMR = p_BG->GetArenaMatchmakerRatingByIndex(l_I);

            *p_Data << uint32(l_PrematchMMR);                   ///< Matchmaking Value
        }
    }

    if (p_BG->GetStatus() == STATUS_WAIT_LEAVE)
        *p_Data << uint8(p_BG->GetWinner());                    ///< who win

    l_ScoreBeginIT = p_BG->GetPlayerScoresBegin();

    for (Battleground::BattlegroundScoreMap::const_iterator l_It = l_ScoreBeginIT; l_It != p_BG->GetPlayerScoresEnd();)
    {
        l_ScoreBeginIT = l_It++;

        if (!p_BG->IsPlayerInBattleground(l_ScoreBeginIT->first))
        {
            sLog->outError(LOG_FILTER_BATTLEGROUND, "Player " UI64FMTD " has scoreboard entry for battleground %u but is not in battleground!", l_It->first, p_BG->GetTypeID(true));
            continue;
        }

        Player* l_Player = ObjectAccessor::FindPlayer(l_ScoreBeginIT->first);

        if (!l_Player)
            continue;

        /// Stats
        ByteBuffer l_Buffer;
        switch (p_BG->GetTypeID(true))
        {
            case BATTLEGROUND_RB:
                switch (p_BG->GetMapId())
                {
                    case 489:
                        l_Buffer << uint32(((BattlegroundWGScore*)l_ScoreBeginIT->second)->FlagCaptures);        // flag captures
                        l_Buffer << uint32(((BattlegroundWGScore*)l_ScoreBeginIT->second)->FlagReturns);         // flag returns
                        break;

                    case 566:
                        l_Buffer << uint32(((BattlegroundEYScore*)l_ScoreBeginIT->second)->FlagCaptures);        // flag captures
                        break;

                    case 529:
                        l_Buffer << uint32(((BattlegroundABScore*)l_ScoreBeginIT->second)->BasesAssaulted);      // bases asssulted
                        l_Buffer << uint32(((BattlegroundABScore*)l_ScoreBeginIT->second)->BasesDefended);       // bases defended
                        break;

                    case 30:
                        l_Buffer << uint32(((BattlegroundAVScore*)l_ScoreBeginIT->second)->GraveyardsAssaulted); // GraveyardsAssaulted
                        l_Buffer << uint32(((BattlegroundAVScore*)l_ScoreBeginIT->second)->GraveyardsDefended);  // GraveyardsDefended
                        l_Buffer << uint32(((BattlegroundAVScore*)l_ScoreBeginIT->second)->TowersAssaulted);     // TowersAssaulted
                        l_Buffer << uint32(((BattlegroundAVScore*)l_ScoreBeginIT->second)->TowersDefended);      // TowersDefended
                        l_Buffer << uint32(((BattlegroundAVScore*)l_ScoreBeginIT->second)->MinesCaptured);       // MinesCaptured
                        break;

                    case 607:
                        l_Buffer << uint32(((BattlegroundSAScore*)l_ScoreBeginIT->second)->demolishers_destroyed);
                        l_Buffer << uint32(((BattlegroundSAScore*)l_ScoreBeginIT->second)->gates_destroyed);
                        break;

                    case 628:                                   // IC
                        l_Buffer << uint32(((BattlegroundICScore*)l_ScoreBeginIT->second)->BasesAssaulted);       // bases asssulted
                        l_Buffer << uint32(((BattlegroundICScore*)l_ScoreBeginIT->second)->BasesDefended);        // bases defended
                        break;

                    case 726:
                        l_Buffer << uint32(((BattlegroundTPScore*)l_ScoreBeginIT->second)->FlagCaptures);         // flag captures
                        l_Buffer << uint32(((BattlegroundTPScore*)l_ScoreBeginIT->second)->FlagReturns);          // flag returns
                        break;

                    case 761:
                        l_Buffer << uint32(((BattlegroundBFGScore*)l_ScoreBeginIT->second)->BasesAssaulted);      // bases asssulted
                        l_Buffer << uint32(((BattlegroundBFGScore*)l_ScoreBeginIT->second)->BasesDefended);       // bases defended
                        break;

                }
                break;

            case BATTLEGROUND_AV:
                l_Buffer << uint32(((BattlegroundAVScore*)l_ScoreBeginIT->second)->GraveyardsAssaulted); // GraveyardsAssaulted
                l_Buffer << uint32(((BattlegroundAVScore*)l_ScoreBeginIT->second)->GraveyardsDefended);  // GraveyardsDefended
                l_Buffer << uint32(((BattlegroundAVScore*)l_ScoreBeginIT->second)->TowersAssaulted);     // TowersAssaulted
                l_Buffer << uint32(((BattlegroundAVScore*)l_ScoreBeginIT->second)->TowersDefended);      // TowersDefended
                l_Buffer << uint32(((BattlegroundAVScore*)l_ScoreBeginIT->second)->MinesCaptured);       // MinesCaptured
                break;

            case BATTLEGROUND_WS:
                l_Buffer << uint32(((BattlegroundWGScore*)l_ScoreBeginIT->second)->FlagCaptures);        // flag captures
                l_Buffer << uint32(((BattlegroundWGScore*)l_ScoreBeginIT->second)->FlagReturns);         // flag returns
                break;

            case BATTLEGROUND_AB:
                l_Buffer << uint32(((BattlegroundABScore*)l_ScoreBeginIT->second)->BasesAssaulted);      // bases asssulted
                l_Buffer << uint32(((BattlegroundABScore*)l_ScoreBeginIT->second)->BasesDefended);       // bases defended
                break;

            case BATTLEGROUND_EY:
            case BATTLEGROUND_EYR:
                l_Buffer << uint32(((BattlegroundEYScore*)l_ScoreBeginIT->second)->FlagCaptures);        // flag captures
                break;

            case BATTLEGROUND_SA:
                l_Buffer << uint32(((BattlegroundSAScore*)l_ScoreBeginIT->second)->demolishers_destroyed);
                l_Buffer << uint32(((BattlegroundSAScore*)l_ScoreBeginIT->second)->gates_destroyed);
                break;

            case BATTLEGROUND_IC:
                l_Buffer << uint32(((BattlegroundICScore*)l_ScoreBeginIT->second)->BasesAssaulted);       // bases asssulted
                l_Buffer << uint32(((BattlegroundICScore*)l_ScoreBeginIT->second)->BasesDefended);        // bases defended
                break;

            case BATTLEGROUND_TP:
                l_Buffer << uint32(((BattlegroundTPScore*)l_ScoreBeginIT->second)->FlagCaptures);         // flag captures
                l_Buffer << uint32(((BattlegroundTPScore*)l_ScoreBeginIT->second)->FlagReturns);          // flag returns
                break;

            case BATTLEGROUND_BFG:
                l_Buffer << uint32(((BattlegroundBFGScore*)l_ScoreBeginIT->second)->BasesAssaulted);      // bases asssulted
                l_Buffer << uint32(((BattlegroundBFGScore*)l_ScoreBeginIT->second)->BasesDefended);       // bases defended
                break;

            case BATTLEGROUND_KT:
                l_Buffer << uint32(((BattleGroundKTScore*)l_ScoreBeginIT->second)->OrbHandles);
                l_Buffer << uint32(((BattleGroundKTScore*)l_ScoreBeginIT->second)->Score * 10);
                break;

        }

        p_Data->appendPackGUID(l_Player->GetGUID());                                    ///< Player GUID

        *p_Data << uint32(l_ScoreBeginIT->second->KillingBlows);
        *p_Data << uint32(l_ScoreBeginIT->second->DamageDone);
        *p_Data << uint32(l_ScoreBeginIT->second->HealingDone);
        *p_Data << uint32(l_Buffer.size() / 4);                                         ///< Stats count
        *p_Data << int32(0);                                                            ///< Unk can be l_Player->GetPrimaryTalentTree(player->GetActiveSpec()) ?
        *p_Data << int32(0);                                                            ///< Unk can be l_Player->GetPrimaryTalentTree(player->GetActiveSpec()) ?

        p_Data->append(l_Buffer);                                                       ///< Stats

        if (l_IsArena || l_IsRatedBg)
            p_Data->WriteBit(p_BG->GetPlayerTeam(l_Player->GetGUID()) == ALLIANCE);     ///< Faction
        else
            p_Data->WriteBit(l_Player->GetTeam() == ALLIANCE);                          ///< Faction
        p_Data->WriteBit(true);                                                         ///< Is In world
        p_Data->WriteBit(l_HasHonor);                                                   ///< Has Honor
        p_Data->WriteBit(l_HasPreMatchRating);                                          ///< Pre Match Rating
        p_Data->WriteBit(l_HasRatingChange);                                            ///< Rating Change
        p_Data->WriteBit(l_HasPreMatchMMR);                                             ///< Pre Match MMR
        p_Data->WriteBit(l_HasMMRChange);                                               ///< MMR Change
        p_Data->FlushBits();

        if (l_HasHonor)
        {
            *p_Data << uint32(l_ScoreBeginIT->second->HonorableKills);                  ///< Honor Kills
            *p_Data << uint32(l_ScoreBeginIT->second->Deaths);                          ///< Deaths
            *p_Data << uint32(l_ScoreBeginIT->second->BonusHonor / 100);                ///< Contribution Points
        }

        if (l_HasPreMatchRating)
            *p_Data << uint32(0);                                                       ///< @TODO

        if (l_HasRatingChange)
            *p_Data << int32(l_ScoreBeginIT->second->RatingChange);

        if (l_HasPreMatchMMR)
            *p_Data << uint32(0);                                                       ///< @TODO

        if (l_HasMMRChange)
            *p_Data << uint32(0);                                                       ///< @TODO
    }
}

void BattlegroundMgr::BuildStatusFailedPacket(WorldPacket* p_Data, Battleground * p_BG, Player* p_Player, uint32 p_QueueSlot, GroupJoinBattlegroundResult p_Result)
{
    uint64 l_ClientID = 0;

    p_Data->Initialize(SMSG_BATTLEFIELD_STATUS_FAILED);
    p_Data->appendPackGUID(p_Player->GetGUID());
    *p_Data << uint32(p_QueueSlot);                                                 ///< ID
    *p_Data << uint32(p_BG->isArena() ? p_BG->GetMaxPlayersPerTeam() : 1);          ///< Type
    *p_Data << uint32(p_Player->GetBattlegroundQueueJoinTime(p_BG->GetTypeID()));   ///< Time

    *p_Data << uint64(p_BG->GetGUID());
    *p_Data << uint32(p_Result);
    p_Data->appendPackGUID(l_ClientID);
}

void BattlegroundMgr::BuildUpdateWorldStatePacket(WorldPacket* p_Data, uint32 field, uint32 value)
{
    p_Data->Initialize(SMSG_UPDATE_WORLD_STATE, 4+4);
    *p_Data << uint32(field);
    *p_Data << uint32(value);
    p_Data->WriteBit(0);
    p_Data->FlushBits();
}

void BattlegroundMgr::BuildPlayerLeftBattlegroundPacket(WorldPacket* p_Data, uint64 p_Guid)
{
    p_Data->Initialize(SMSG_BATTLEGROUND_PLAYER_LEFT, 8);
    p_Data->appendPackGUID(p_Guid);
}

void BattlegroundMgr::BuildPlayerJoinedBattlegroundPacket(WorldPacket* p_Data, uint64 p_Guid)
{
    p_Data->Initialize(SMSG_BATTLEGROUND_PLAYER_JOINED, 8);
    p_Data->appendPackGUID(p_Guid);
}

Battleground* BattlegroundMgr::GetBattlegroundThroughClientInstance(uint32 instanceId, BattlegroundTypeId bgTypeId)
{
    //cause at HandleBattlegroundJoinOpcode the clients sends the instanceid he gets from
    //SMSG_BATTLEFIELD_LIST we need to find the battleground with this clientinstance-id
    Battleground* bg = GetBattlegroundTemplate(bgTypeId);
    if (!bg)
        return NULL;

    if (bg->isArena())
        return GetBattleground(instanceId, bgTypeId);

    for (BattlegroundSet::iterator itr = m_Battlegrounds[bgTypeId].begin(); itr != m_Battlegrounds[bgTypeId].end(); ++itr)
    {
        if (itr->second->GetClientInstanceID() == instanceId)
            return itr->second;
    }
    return NULL;
}

Battleground* BattlegroundMgr::GetBattleground(uint32 InstanceID, BattlegroundTypeId bgTypeId)
{
    if (!InstanceID)
        return NULL;
    //search if needed
    BattlegroundSet::iterator itr;
    if (bgTypeId == BATTLEGROUND_TYPE_NONE)
    {
        for (uint32 i = BATTLEGROUND_AV; i < MAX_BATTLEGROUND_TYPE_ID; i++)
        {
            itr = m_Battlegrounds[i].find(InstanceID);
            if (itr != m_Battlegrounds[i].end())
                return itr->second;
        }
        return NULL;
    }
    itr = m_Battlegrounds[bgTypeId].find(InstanceID);
    return ((itr != m_Battlegrounds[bgTypeId].end()) ? itr->second : NULL);
}

Battleground* BattlegroundMgr::GetBattlegroundTemplate(BattlegroundTypeId bgTypeId)
{
    //map is sorted and we can be sure that lowest instance id has only BG template
    return m_Battlegrounds[bgTypeId].empty() ? NULL : m_Battlegrounds[bgTypeId].begin()->second;
}

uint32 BattlegroundMgr::CreateClientVisibleInstanceId(BattlegroundTypeId bgTypeId, BattlegroundBracketId bracket_id)
{
    if (IsArenaType(bgTypeId))
        return 0;                                           //arenas don't have client-instanceids

    // we create here an instanceid, which is just for
    // displaying this to the client and without any other use..
    // the client-instanceIds are unique for each battleground-type
    // the instance-id just needs to be as low as possible, beginning with 1
    // the following works, because std::set is default ordered with "<"
    // the optimalization would be to use as bitmask std::vector<uint32> - but that would only make code unreadable
    uint32 lastId = 0;
    for (std::set<uint32>::iterator itr = m_ClientBattlegroundIds[bgTypeId][bracket_id].begin(); itr != m_ClientBattlegroundIds[bgTypeId][bracket_id].end();)
    {
        if ((++lastId) != *itr)                             //if there is a gap between the ids, we will break..
            break;
        lastId = *itr;
    }
    m_ClientBattlegroundIds[bgTypeId][bracket_id].insert(lastId + 1);
    return lastId + 1;
}

// create a new battleground that will really be used to play
Battleground* BattlegroundMgr::CreateNewBattleground(BattlegroundTypeId bgTypeId, PvPDifficultyEntry const* bracketEntry, uint8 arenaType, bool isRated)
{
    // get the template BG
    Battleground* bg_template = GetBattlegroundTemplate(bgTypeId);
    BattlegroundSelectionWeightMap* selectionWeights = NULL;

    if (!bg_template)
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground: CreateNewBattleground - bg template not found for %u", bgTypeId);
        return NULL;
    }
    bool isRandom = false;
    bool isRatedBg = false;

    if (bg_template->isArena())
        selectionWeights = &m_ArenaSelectionWeights;
    else if (bgTypeId == BATTLEGROUND_RB)
    {
        selectionWeights = &m_BGSelectionWeights;
        isRandom = true;
    }
    else if (bgTypeId == BATTLEGROUND_RATED_10_VS_10)
    {
        selectionWeights = &m_RatedBGSelectionWeights;
        isRatedBg;
    }

    if (selectionWeights)
    {
        if (selectionWeights->empty())
           return NULL;
        uint32 Weight = 0;
        uint32 selectedWeight = 0;
        bgTypeId = BATTLEGROUND_TYPE_NONE;
        // Get sum of all weights
        for (BattlegroundSelectionWeightMap::const_iterator it = selectionWeights->begin(); it != selectionWeights->end(); ++it)
            Weight += it->second;
        if (!Weight)
            return NULL;
        // Select a random value
        selectedWeight = urand(0, Weight-1);

        // Select the correct bg (if we have in DB A(10), B(20), C(10), D(15) --> [0---A---9|10---B---29|30---C---39|40---D---54])
        Weight = 0;
        for (BattlegroundSelectionWeightMap::const_iterator it = selectionWeights->begin(); it != selectionWeights->end(); ++it)
        {
            Weight += it->second;
            if (selectedWeight < Weight)
            {
                bgTypeId = it->first;
                break;
            }
        }
        bg_template = GetBattlegroundTemplate(bgTypeId);
        if (!bg_template)
        {
            sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground: CreateNewBattleground - bg template not found for %u", bgTypeId);
            return NULL;
        }
    }

    Battleground* bg = NULL;
    // create a copy of the BG template
    switch (bgTypeId)
    {
        case BATTLEGROUND_AV:
            bg = new BattlegroundAV(*(BattlegroundAV*)bg_template);
            break;
        case BATTLEGROUND_WS:
            bg = new BattlegroundWS(*(BattlegroundWS*)bg_template);
            break;
        case BATTLEGROUND_AB:
            bg = new BattlegroundAB(*(BattlegroundAB*)bg_template);
            break;
        case BATTLEGROUND_NA:
            bg = new BattlegroundNA(*(BattlegroundNA*)bg_template);
            break;
        case BATTLEGROUND_TV:
            bg = new BattlegroundTV(*(BattlegroundTV*)bg_template);
            break;
        case BATTLEGROUND_TTP:
            bg = new BattlegroundTTP(*(BattlegroundTTP*)bg_template);
            break;
        case BATTLEGROUND_BE:
            bg = new BattlegroundBE(*(BattlegroundBE*)bg_template);
            break;
        case BATTLEGROUND_AA:
            bg = new BattlegroundAA(*(BattlegroundAA*)bg_template);
            break;
        case BATTLEGROUND_EY:
        case BATTLEGROUND_EYR:
            bg = new BattlegroundEY(*(BattlegroundEY*)bg_template);
            break;
        case BATTLEGROUND_RL:
            bg = new BattlegroundRL(*(BattlegroundRL*)bg_template);
            break;
        case BATTLEGROUND_SA:
            bg = new BattlegroundSA(*(BattlegroundSA*)bg_template);
            break;
        case BATTLEGROUND_DS:
            bg = new BattlegroundDS(*(BattlegroundDS*)bg_template);
            break;
        case BATTLEGROUND_RV:
            bg = new BattlegroundRV(*(BattlegroundRV*)bg_template);
            break;
        case BATTLEGROUND_IC:
            bg = new BattlegroundIC(*(BattlegroundIC*)bg_template);
            break;
        case BATTLEGROUND_TP:
            bg = new BattlegroundTP(*(BattlegroundTP*)bg_template);
            break;
        case BATTLEGROUND_BFG:
            bg = new BattlegroundBFG(*(BattlegroundBFG*)bg_template);
            break;
        case BATTLEGROUND_RB:
            bg = new BattlegroundRB(*(BattlegroundRB*)bg_template);
            break;
        case BATTLEGROUND_KT:
            bg = new BattlegroundKT(*(BattlegroundKT*)bg_template);
            break;
        case BATTLEGROUND_SSM:
            bg = new BattlegroundSSM(*(BattlegroundSSM*)bg_template);
            break;
        case BATTLEGROUND_RATED_10_VS_10:
            bg = new BattlegroundRBG(*(BattlegroundRBG*)bg_template);
            break;
        default:
            //error, but it is handled few lines above
            return 0;
    }

    // set battelground difficulty before initialization
    bg->SetBracket(bracketEntry);

    // generate a new instance id
    bg->SetInstanceID(sMapMgr->GenerateInstanceId()); // set instance id
    bg->SetClientInstanceID(CreateClientVisibleInstanceId(isRandom ? BATTLEGROUND_RB : (isRatedBg ? BATTLEGROUND_RATED_10_VS_10 : bgTypeId), bracketEntry->GetBracketId()));

    // reset the new bg (set status to status_wait_queue from status_none)
    bg->Reset();

    // start the joining of the bg
    bg->SetStatus(STATUS_WAIT_JOIN);
    bg->SetArenaType(arenaType);
    bg->SetRated(isRated);
    bg->SetRatedBG(isRatedBg);
    bg->SetRandom(isRandom);
    bg->SetTypeID(isRandom ? BATTLEGROUND_RB : (isRatedBg ? BATTLEGROUND_RATED_10_VS_10 : bgTypeId));
    bg->SetRandomTypeID(bgTypeId);
    bg->InitGUID();

    return bg;
}

// used to create the BG templates
uint32 BattlegroundMgr::CreateBattleground(CreateBattlegroundData& data)
{
    // Create the BG
    Battleground* bg = NULL;
    switch (data.bgTypeId)
    {
        case BATTLEGROUND_AV: bg = new BattlegroundAV; break;
        case BATTLEGROUND_WS: bg = new BattlegroundWS; break;
        case BATTLEGROUND_AB: bg = new BattlegroundAB; break;
        case BATTLEGROUND_NA: bg = new BattlegroundNA; break;
        case BATTLEGROUND_TV: bg = new BattlegroundTV; break;
        case BATTLEGROUND_TTP: bg = new BattlegroundTTP; break;
        case BATTLEGROUND_BE: bg = new BattlegroundBE; break;
        case BATTLEGROUND_AA: bg = new BattlegroundAA; break;
        case BATTLEGROUND_EY:
        case BATTLEGROUND_EYR:
            bg = new BattlegroundEY;
            break;
        case BATTLEGROUND_RL: bg = new BattlegroundRL; break;
        case BATTLEGROUND_SA: bg = new BattlegroundSA; break;
        case BATTLEGROUND_DS: bg = new BattlegroundDS; break;
        case BATTLEGROUND_RV: bg = new BattlegroundRV; break;
        case BATTLEGROUND_IC: bg = new BattlegroundIC; break;
        case BATTLEGROUND_TP: bg = new BattlegroundTP; break;
        case BATTLEGROUND_BFG: bg = new BattlegroundBFG; break;
        case BATTLEGROUND_RB: bg = new BattlegroundRB; break;
        case BATTLEGROUND_KT: bg = new BattlegroundKT; break;
        case BATTLEGROUND_RATED_10_VS_10: bg = new BattlegroundRBG; break;
        default:
            bg = new Battleground;
            break;
    }

    bg->SetMapId(data.MapID);
    bg->SetTypeID(data.bgTypeId);
    bg->InitGUID();
    bg->SetInstanceID(0);
    bg->SetArenaorBGType(data.IsArena);
    bg->SetRatedBG(data.bgTypeId == BATTLEGROUND_RATED_10_VS_10);
    bg->SetMinPlayersPerTeam(data.MinPlayersPerTeam);
    bg->SetMaxPlayersPerTeam(data.MaxPlayersPerTeam);
    bg->SetMinPlayers(data.MinPlayersPerTeam* 2);
    bg->SetMaxPlayers(data.MaxPlayersPerTeam* 2);
    bg->SetName(data.BattlegroundName);
    bg->SetTeamStartLoc(ALLIANCE, data.Team1StartLocX, data.Team1StartLocY, data.Team1StartLocZ, data.Team1StartLocO);
    bg->SetTeamStartLoc(HORDE,    data.Team2StartLocX, data.Team2StartLocY, data.Team2StartLocZ, data.Team2StartLocO);
    bg->SetStartMaxDist(data.StartMaxDist);
    bg->SetLevelRange(data.LevelMin, data.LevelMax);
    bg->SetHolidayId(data.holiday);
    bg->SetScriptId(data.scriptId);

    // add bg to update list
    AddBattleground(bg->GetInstanceID(), bg->GetTypeID(), bg);

    // return some not-null value, bgTypeId is good enough for me
    return data.bgTypeId;
}

void BattlegroundMgr::CreateInitialBattlegrounds()
{
    uint32 oldMSTime = getMSTime();

    uint8 selectionWeight;
    BattlemasterListEntry const* bl;
    BattlemasterListEntry const* rated_bl = sBattlemasterListStore.LookupEntry(BATTLEGROUND_RATED_10_VS_10);

    //                                               0   1                  2                  3       4       5                 6               7              8            9             10      11       12
    QueryResult result = WorldDatabase.Query("SELECT id, MinPlayersPerTeam, MaxPlayersPerTeam, MinLvl, MaxLvl, AllianceStartLoc, AllianceStartO, HordeStartLoc, HordeStartO, StartMaxDist, Weight, holiday, ScriptName FROM battleground_template");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 battlegrounds. DB table `battleground_template` is empty.");
        return;
    }

    uint32 count = 0, startId;

    do
    {
        Field* fields = result->Fetch();

        uint32 bgTypeID_ = fields[0].GetUInt32();
        if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, bgTypeID_, NULL))
            continue;

        // can be overwrite by values from DB
        bl = sBattlemasterListStore.LookupEntry(bgTypeID_);
        if (!bl)
        {
            sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground ID %u not found in BattlemasterList.dbc. Battleground not created.", bgTypeID_);
            continue;
        }

        CreateBattlegroundData data;
        data.bgTypeId = BattlegroundTypeId(bgTypeID_);
        data.IsArena = (bl->type == TYPE_ARENA);
        data.MinPlayersPerTeam = fields[1].GetUInt16();
        data.MaxPlayersPerTeam = fields[2].GetUInt16();
        data.LevelMin = fields[3].GetUInt8();
        data.LevelMax = fields[4].GetUInt8();

        // check values from DB
        if (data.MaxPlayersPerTeam == 0 || data.MinPlayersPerTeam > data.MaxPlayersPerTeam)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `battleground_template` for id %u has bad values for MinPlayersPerTeam (%u) and MaxPlayersPerTeam(%u)",
                data.bgTypeId, data.MinPlayersPerTeam, data.MaxPlayersPerTeam);
            continue;
        }

        if (data.LevelMin == 0 || data.LevelMax == 0 || data.LevelMin > data.LevelMax)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `battleground_template` for id %u has bad values for LevelMin (%u) and LevelMax(%u)",
                data.bgTypeId, data.LevelMin, data.LevelMax);
            continue;
        }

        startId = fields[5].GetUInt32();
        if (WorldSafeLocsEntry const* start = sWorldSafeLocsStore.LookupEntry(startId))
        {
            data.Team1StartLocX = start->x;
            data.Team1StartLocY = start->y;
            data.Team1StartLocZ = start->z;
            data.Team1StartLocO = fields[6].GetFloat();
        }
        else if (data.bgTypeId == BATTLEGROUND_AA || data.bgTypeId == BATTLEGROUND_RB || data.bgTypeId == BATTLEGROUND_RATED_10_VS_10)
        {
            data.Team1StartLocX = 0;
            data.Team1StartLocY = 0;
            data.Team1StartLocZ = 0;
            data.Team1StartLocO = fields[6].GetFloat();
        }
        else
        {
            sLog->outError(LOG_FILTER_SQL, "Table `battleground_template` for id %u have non-existed WorldSafeLocs.dbc id %u in field `AllianceStartLoc`. BG not created.", data.bgTypeId, startId);
            continue;
        }

        startId = fields[7].GetUInt32();
        if (WorldSafeLocsEntry const* start = sWorldSafeLocsStore.LookupEntry(startId))
        {
            data.Team2StartLocX = start->x;
            data.Team2StartLocY = start->y;
            data.Team2StartLocZ = start->z;
            data.Team2StartLocO = fields[8].GetFloat();
        }
        else if (data.bgTypeId == BATTLEGROUND_AA || data.bgTypeId == BATTLEGROUND_RB || data.bgTypeId == BATTLEGROUND_RATED_10_VS_10)
        {
            data.Team2StartLocX = 0;
            data.Team2StartLocY = 0;
            data.Team2StartLocZ = 0;
            data.Team2StartLocO = fields[8].GetFloat();
        }
        else
        {
            sLog->outError(LOG_FILTER_SQL, "Table `battleground_template` for id %u have non-existed WorldSafeLocs.dbc id %u in field `HordeStartLoc`. BG not created.", data.bgTypeId, startId);
            continue;
        }

        data.StartMaxDist = fields[9].GetFloat();

        selectionWeight = fields[10].GetUInt8();
        data.holiday = fields[11].GetUInt32();
        data.scriptId = sObjectMgr->GetScriptId(fields[12].GetCString());

        //data.BattlegroundName = bl->name[sWorld->GetDefaultDbcLocale()];
        data.MapID = bl->mapid[0];

        if (!CreateBattleground(data))
            continue;

        if (data.IsArena)
        {
            if (data.bgTypeId != BATTLEGROUND_AA)
                m_ArenaSelectionWeights[data.bgTypeId] = selectionWeight;
        }
        else if (data.bgTypeId != BATTLEGROUND_RB && data.bgTypeId != BATTLEGROUND_RATED_10_VS_10
            && data.bgTypeId != BATTLEGROUND_RATED_15_VS_15 && data.bgTypeId != BATTLEGROUND_RATED_25_VS_25)
            m_BGSelectionWeights[data.bgTypeId] = selectionWeight;

        for (int i = 0; i < 11; ++i)
            if (rated_bl->mapid[i] == bl->mapid[0] && bl->mapid[1] == -1)
                m_RatedBGSelectionWeights[data.bgTypeId] = selectionWeight;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u battlegrounds in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void BattlegroundMgr::BuildBattlegroundListPacket(WorldPacket* data, ObjectGuid guid, Player* player, BattlegroundTypeId bgTypeId)
{
    if (!player)
        return;

    uint32 winner_conquest = (player->GetRandomWinner() ? BG_REWARD_WINNER_CONQUEST_FIRST : BG_REWARD_WINNER_CONQUEST_LAST) / 100;
    uint32 winner_honor = (player->GetRandomWinner() ? BG_REWARD_WINNER_HONOR_FIRST : BG_REWARD_WINNER_HONOR_LAST) / 100;

    ByteBuffer dataBuffer;

    data->Initialize(SMSG_BATTLEFIELD_LIST);
    data->WriteBit(0); // unk1
    data->WriteBit(guid[4]);
    data->WriteBit(1); // byte2C
    data->WriteBit(guid[2]);
    data->WriteBit(guid[6]);

    if (bgTypeId == BATTLEGROUND_AA)                         // arena
    {
        data->WriteBits(0, 22);                                 // unk (count?)
    }
    else                                                    // battleground
    {
        uint32 count = 0;
        if (Battleground* bgTemplate = sBattlegroundMgr->GetBattlegroundTemplate(bgTypeId))
        {
            // expected bracket entry
            if (PvPDifficultyEntry const* bracketEntry = GetBattlegroundBracketByLevel(bgTemplate->GetMapId(), player->getLevel()))
            {
                BattlegroundBracketId bracketId = bracketEntry->GetBracketId();
                for (std::set<uint32>::iterator itr = m_ClientBattlegroundIds[bgTypeId][bracketId].begin(); itr != m_ClientBattlegroundIds[bgTypeId][bracketId].end();++itr)
                {
                    dataBuffer << uint32(*itr);
                    ++count;
                }
            }
        }
        data->WriteBits(count, 22);
    }

    data->WriteBit(0); // byte40
    data->WriteBit(guid[7]);
    data->WriteBit(1); // byte50
    data->WriteBit(guid[1]);
    data->WriteBit(guid[0]);
    data->WriteBit(guid[3]);
    data->WriteBit(guid[5]);

    *data << uint32(winner_conquest); // dword28
    data->WriteByteSeq(guid[4]);
    *data << uint32(45); // dword20
    data->WriteByteSeq(guid[2]);
    data->WriteByteSeq(guid[7]);
    *data << uint32(32); // dword24
    *data << uint8(90); // byte41
    data->WriteByteSeq(guid[1]);
    data->WriteByteSeq(guid[5]);
    data->WriteByteSeq(guid[0]);
    *data << uint32(45); // dword18
    *data << uint8(90); // byte48
    data->WriteByteSeq(guid[3]);
    *data << uint32(winner_honor); // dword4C
    *data << uint32(winner_conquest); // dword1C
    data->append(dataBuffer);
    data->WriteByteSeq(guid[6]);
    *data << uint32(winner_honor); // dword44
}

void BattlegroundMgr::SendToBattleground(Player* player, uint32 instanceId, BattlegroundTypeId bgTypeId)
{
    Battleground* bg = GetBattleground(instanceId, bgTypeId);
    if (bg)
    {
        uint32 mapid = bg->GetMapId();
        float x, y, z, O;
        uint32 team = player->GetBGTeam();
        if (team == 0)
            team = player->GetTeam();
        bg->GetTeamStartLoc(team, x, y, z, O);

        sLog->outInfo(LOG_FILTER_BATTLEGROUND, "BATTLEGROUND: Sending %s to map %u, X %f, Y %f, Z %f, O %f", player->GetName(), mapid, x, y, z, O);
        player->TeleportTo(mapid, x, y, z, O);
    }
    else
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "player %u is trying to port to non-existent bg instance %u", player->GetGUIDLow(), instanceId);
    }
}

void BattlegroundMgr::SendAreaSpiritHealerQueryOpcode(Player* player, Battleground* bg, uint64 guid)
{
    WorldPacket data(SMSG_AREA_SPIRIT_HEALER_TIME, 12);
    ObjectGuid npcGuid = guid;

    uint32 time_ = 30000 - bg->GetLastResurrectTime();      // resurrect every 30 seconds
    if (time_ == uint32(-1))
        time_ = 0;

    uint8 bitsOrder[8] = { 0, 4, 2, 6, 3, 5, 1, 7 };
    data.WriteBitInOrder(npcGuid, bitsOrder);

    data.WriteByteSeq(npcGuid[4]);
    data.WriteByteSeq(npcGuid[5]);
    data.WriteByteSeq(npcGuid[3]);
    data.WriteByteSeq(npcGuid[2]);
    data.WriteByteSeq(npcGuid[6]);
    data.WriteByteSeq(npcGuid[7]);
    data << uint32(time_);
    data.WriteByteSeq(npcGuid[0]);
    data.WriteByteSeq(npcGuid[1]);

    player->GetSession()->SendPacket(&data);
}

bool BattlegroundMgr::IsArenaType(BattlegroundTypeId bgTypeId)
{
    return (bgTypeId == BATTLEGROUND_AA ||
        bgTypeId == BATTLEGROUND_BE ||
        bgTypeId == BATTLEGROUND_NA ||
        bgTypeId == BATTLEGROUND_TV ||
        bgTypeId == BATTLEGROUND_TTP ||
        bgTypeId == BATTLEGROUND_DS ||
        bgTypeId == BATTLEGROUND_RV ||
        bgTypeId == BATTLEGROUND_RL);
}

BattlegroundQueueTypeId BattlegroundMgr::BGQueueTypeId(BattlegroundTypeId bgTypeId, uint8 arenaType)
{
    switch (bgTypeId)
    {
        case BATTLEGROUND_WS:
            return BATTLEGROUND_QUEUE_WS;
        case BATTLEGROUND_AB:
            return BATTLEGROUND_QUEUE_AB;
        case BATTLEGROUND_AV:
            return BATTLEGROUND_QUEUE_AV;
        case BATTLEGROUND_EY:
            return BATTLEGROUND_QUEUE_EY;
        case BATTLEGROUND_SA:
            return BATTLEGROUND_QUEUE_SA;
        case BATTLEGROUND_IC:
            return BATTLEGROUND_QUEUE_IC;
        case BATTLEGROUND_TP:
            return BATTLEGROUND_QUEUE_TP;
        case BATTLEGROUND_BFG:
            return BATTLEGROUND_QUEUE_BFG;
        case BATTLEGROUND_RB:
            return BATTLEGROUND_QUEUE_RB;
        case BATTLEGROUND_KT:
            return BATTLEGROUND_QUEUE_KT;
        case BATTLEGROUND_CTF3:
            return BATTLEGROUND_QUEUE_CTF3;
        case BATTLEGROUND_SSM:
            return BATTLEGROUND_QUEUE_SSM;
        case BATTLEGROUND_AA:
        case BATTLEGROUND_NA:
        case BATTLEGROUND_TV:
        case BATTLEGROUND_TTP:
        case BATTLEGROUND_RL:
        case BATTLEGROUND_BE:
        case BATTLEGROUND_DS:
        case BATTLEGROUND_RV:
            switch (arenaType)
            {
                case ARENA_TYPE_2v2:
                    return BATTLEGROUND_QUEUE_2v2;
                case ARENA_TYPE_3v3:
                    return BATTLEGROUND_QUEUE_3v3;
                case ARENA_TYPE_5v5:
                    return BATTLEGROUND_QUEUE_5v5;
                default:
                    return BATTLEGROUND_QUEUE_NONE;
            }
        case BATTLEGROUND_RATED_10_VS_10:
            return BATTLEGROUND_QUEUE_RATED_10_VS_10;
        case BATTLEGROUND_RATED_15_VS_15:
            return BATTLEGROUND_QUEUE_RATED_15_VS_15;
        case BATTLEGROUND_RATED_25_VS_25:
            return BATTLEGROUND_QUEUE_RATED_25_VS_25;
        default:
            return BATTLEGROUND_QUEUE_NONE;
    }
}

BattlegroundTypeId BattlegroundMgr::BGTemplateId(BattlegroundQueueTypeId bgQueueTypeId)
{
    switch (bgQueueTypeId)
    {
        case BATTLEGROUND_QUEUE_WS:
            return BATTLEGROUND_WS;
        case BATTLEGROUND_QUEUE_AB:
            return BATTLEGROUND_AB;
        case BATTLEGROUND_QUEUE_AV:
            return BATTLEGROUND_AV;
        case BATTLEGROUND_QUEUE_EY:
            return BATTLEGROUND_EY;
        case BATTLEGROUND_QUEUE_SA:
            return BATTLEGROUND_SA;
        case BATTLEGROUND_QUEUE_IC:
            return BATTLEGROUND_IC;
        case BATTLEGROUND_QUEUE_TP:
            return BATTLEGROUND_TP;
        case BATTLEGROUND_QUEUE_BFG:
            return BATTLEGROUND_BFG;
        case BATTLEGROUND_QUEUE_RB:
            return BATTLEGROUND_RB;
        case BATTLEGROUND_QUEUE_KT:
            return BATTLEGROUND_KT;
        case BATTLEGROUND_QUEUE_CTF3:
            return BATTLEGROUND_CTF3;
        case BATTLEGROUND_QUEUE_SSM:
            return BATTLEGROUND_SSM;
        case BATTLEGROUND_QUEUE_2v2:
        case BATTLEGROUND_QUEUE_3v3:
        case BATTLEGROUND_QUEUE_5v5:
            return BATTLEGROUND_AA;
        case BATTLEGROUND_QUEUE_RATED_10_VS_10:
            return BATTLEGROUND_RATED_10_VS_10;
        case BATTLEGROUND_QUEUE_RATED_15_VS_15:
            return BATTLEGROUND_RATED_15_VS_15;
        case BATTLEGROUND_QUEUE_RATED_25_VS_25:
            return BATTLEGROUND_RATED_25_VS_25;
        default:
            return BattlegroundTypeId(0);                   // used for unknown template (it existed and do nothing)
    }
}

uint8 BattlegroundMgr::BGArenaType(BattlegroundQueueTypeId bgQueueTypeId)
{
    switch (bgQueueTypeId)
    {
        case BATTLEGROUND_QUEUE_2v2:
            return ARENA_TYPE_2v2;
        case BATTLEGROUND_QUEUE_3v3:
            return ARENA_TYPE_3v3;
        case BATTLEGROUND_QUEUE_5v5:
            return ARENA_TYPE_5v5;
        default:
            return 0;
    }
}

void BattlegroundMgr::ToggleTesting()
{
    m_Testing = !m_Testing;
    if (m_Testing)
        sWorld->SendWorldText(LANG_DEBUG_BG_ON);
    else
        sWorld->SendWorldText(LANG_DEBUG_BG_OFF);
}

void BattlegroundMgr::ToggleArenaTesting()
{
    m_ArenaTesting = !m_ArenaTesting;
    if (m_ArenaTesting)
        sWorld->SendWorldText(LANG_DEBUG_ARENA_ON);
    else
        sWorld->SendWorldText(LANG_DEBUG_ARENA_OFF);
}

void BattlegroundMgr::SetHolidayWeekends(std::list<uint32> activeHolidayId)
{
    for (uint32 bgtype = 1; bgtype < MAX_BATTLEGROUND_TYPE_ID; ++bgtype)
    {
        if (Battleground* bg = GetBattlegroundTemplate(BattlegroundTypeId(bgtype)))
        {
            bool holidayActivate = false;

            if (uint32 holidayId = bg->GetHolidayId())
                for (auto activeId: activeHolidayId)
                    if (holidayId == activeId)
                        holidayActivate = true;

            bg->SetHoliday(holidayActivate);
        }
    }
}

void BattlegroundMgr::ScheduleQueueUpdate(uint32 arenaMatchmakerRating, uint8 arenaType, BattlegroundQueueTypeId bgQueueTypeId, BattlegroundTypeId bgTypeId, BattlegroundBracketId bracket_id)
{
    //This method must be atomic, TODO add mutex
    //we will use only 1 number created of bgTypeId and bracket_id
    QueueSchedulerItem* schedule_id = new QueueSchedulerItem(arenaMatchmakerRating, arenaType, bgQueueTypeId, bgTypeId, bracket_id);
    bool found = false;
    for (uint8 i = 0; i < m_QueueUpdateScheduler.size(); i++)
    {
        if (m_QueueUpdateScheduler[i]->_arenaMMRating == arenaMatchmakerRating
            && m_QueueUpdateScheduler[i]->_arenaType == arenaType
            && m_QueueUpdateScheduler[i]->_bgQueueTypeId == bgQueueTypeId
            && m_QueueUpdateScheduler[i]->_bgTypeId == bgTypeId
            && m_QueueUpdateScheduler[i]->_bracket_id == bracket_id)
        {
            found = true;
            break;
        }
    }
    if (!found)
        m_QueueUpdateScheduler.push_back(schedule_id);
}

uint32 BattlegroundMgr::GetMaxRatingDifference() const
{
    // this is for stupid people who can't use brain and set max rating difference to 0
    uint32 diff = sWorld->getIntConfig(CONFIG_ARENA_MAX_RATING_DIFFERENCE);
    if (diff == 0)
        diff = 5000;
    return diff;
}

uint32 BattlegroundMgr::GetRatingDiscardTimer() const
{
    return sWorld->getIntConfig(CONFIG_ARENA_RATING_DISCARD_TIMER);
}

uint32 BattlegroundMgr::GetPrematureFinishTime() const
{
    return sWorld->getIntConfig(CONFIG_BATTLEGROUND_PREMATURE_FINISH_TIMER);
}

void BattlegroundMgr::LoadBattleMastersEntry()
{
    uint32 oldMSTime = getMSTime();

    mBattleMastersMap.clear();                                  // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, bg_template FROM battlemaster_entry");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 battlemaster entries. DB table `battlemaster_entry` is empty!");
        return;
    }

    uint32 count = 0;

    do
    {
        ++count;

        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();
        uint32 bgTypeId  = fields[1].GetUInt32();
        if (!sBattlemasterListStore.LookupEntry(bgTypeId))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `battlemaster_entry` contain entry %u for not existed battleground type %u, ignored.", entry, bgTypeId);
            continue;
        }

        mBattleMastersMap[entry] = BattlegroundTypeId(bgTypeId);
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u battlemaster entries in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

HolidayIds BattlegroundMgr::BGTypeToWeekendHolidayId(BattlegroundTypeId bgTypeId)
{
    switch (bgTypeId)
    {
        case BATTLEGROUND_AV: return HOLIDAY_CALL_TO_ARMS_AV;
        case BATTLEGROUND_EY: return HOLIDAY_CALL_TO_ARMS_EY;
        case BATTLEGROUND_WS: return HOLIDAY_CALL_TO_ARMS_WS;
        case BATTLEGROUND_SA: return HOLIDAY_CALL_TO_ARMS_SA;
        case BATTLEGROUND_AB: return HOLIDAY_CALL_TO_ARMS_AB;
        case BATTLEGROUND_IC: return HOLIDAY_CALL_TO_ARMS_IC;
        case BATTLEGROUND_TP: return HOLIDAY_CALL_TO_ARMS_TP;
        case BATTLEGROUND_BFG: return HOLIDAY_CALL_TO_ARMS_BFG;
        default: return HOLIDAY_NONE;
    }
}

BattlegroundTypeId BattlegroundMgr::WeekendHolidayIdToBGType(HolidayIds holiday)
{
    switch (holiday)
    {
        case HOLIDAY_CALL_TO_ARMS_AV: return BATTLEGROUND_AV;
        case HOLIDAY_CALL_TO_ARMS_EY: return BATTLEGROUND_EY;
        case HOLIDAY_CALL_TO_ARMS_WS: return BATTLEGROUND_WS;
        case HOLIDAY_CALL_TO_ARMS_SA: return BATTLEGROUND_SA;
        case HOLIDAY_CALL_TO_ARMS_AB: return BATTLEGROUND_AB;
        case HOLIDAY_CALL_TO_ARMS_IC: return BATTLEGROUND_IC;
        case HOLIDAY_CALL_TO_ARMS_TP: return BATTLEGROUND_TP;
        case HOLIDAY_CALL_TO_ARMS_BFG: return BATTLEGROUND_BFG;
        default: return BATTLEGROUND_TYPE_NONE;
    }
}

bool BattlegroundMgr::IsBGWeekend(BattlegroundTypeId bgTypeId)
{
    return IsHolidayActive(BGTypeToWeekendHolidayId(bgTypeId));
}
