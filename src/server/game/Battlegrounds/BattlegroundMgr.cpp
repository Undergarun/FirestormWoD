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
#include "BattlegroundTP.h"
#include "BattlegroundBFG.h"
#include "BattlegroundKT.h"
#include "BattlegroundSSM.h"
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

void BattlegroundMgr::BuildBattlegroundStatusPacket(WorldPacket* data, Battleground* bg, Player * pPlayer, uint8 QueueSlot, uint8 StatusID, uint32 Time1, uint32 Time2, uint8 arenatype, uint8 uiFrame)
{
    // we can be in 2 queues in same time...
    if (!bg)
        StatusID = STATUS_NONE;

    ObjectGuid player_guid = pPlayer->GetGUID();
    ObjectGuid bg_guid = bg ? bg->GetGUID() : 0;

    switch (StatusID)
    {
        case STATUS_NONE:
        {
            data->Initialize(SMSG_BATTLEFIELD_STATUS);

            if (bg)
            	*data << uint32(bg->isArena() ? bg->GetMaxPlayersPerTeam() : 1);                         // unk, always 1
            else
            	*data << uint32(1);
            *data << uint32(QueueSlot);                 // Queue slot
            *data << uint32(Time1);                     // Join Time

            uint8 bitOrder[8] = {1, 2, 4, 6, 5, 7, 0, 3};
            data->WriteBitInOrder(player_guid, bitOrder);
            data->FlushBits();

            data->WriteByteSeq(player_guid[7]);
            data->WriteByteSeq(player_guid[6]);
            data->WriteByteSeq(player_guid[4]);
            data->WriteByteSeq(player_guid[3]);
            data->WriteByteSeq(player_guid[0]);
            data->WriteByteSeq(player_guid[1]);
            data->WriteByteSeq(player_guid[2]);
            data->WriteByteSeq(player_guid[5]);
            break;
        }
        case STATUS_WAIT_QUEUE:
        {
            data->Initialize(SMSG_BATTLEFIELD_STATUS_QUEUED);

            data->WriteBit(bg_guid[7]);
            data->WriteBit(player_guid[6]);
            data->WriteBit(1);   // Eligible In Queue
            data->WriteBit(bg_guid[0]);
            data->WriteBit(bg->isRated());   // byte38
            data->WriteBit(0);   // byte4C
            data->WriteBit(player_guid[5]);
            data->WriteBit(player_guid[1]);
            data->WriteBit(player_guid[3]);
            data->WriteBit(bg_guid[6]);
            data->WriteBit(player_guid[4]);
            data->WriteBit(player_guid[0]);
            data->WriteBit(bg_guid[4]);
            data->WriteBit(0);   // byte44
            data->WriteBit(bg_guid[5]);
            data->WriteBit(player_guid[7]);
            data->WriteBit(bg_guid[1]);
            data->WriteBit(bg_guid[2]);
            data->WriteBit(player_guid[2]);
            data->WriteBit(bg_guid[3]);
            data->FlushBits();

            data->WriteByteSeq(bg_guid[1]);
            *data << uint32(Time1);                     // Estimated Wait Time
            *data << uint8(0); // byte32
            *data << uint32(Time2);                     //Time of the join
            *data << uint32(GetMSTimeDiffToNow(Time2));
            data->WriteByteSeq(player_guid[2]);
            data->WriteByteSeq(player_guid[3]);
            data->WriteByteSeq(bg_guid[5]);
            data->WriteByteSeq(bg_guid[3]);
            data->WriteByteSeq(player_guid[5]);
            *data << uint8(bg->GetMinLevel()); //BG Min level
            *data << uint8(0); // byte30
            data->WriteByteSeq(bg_guid[4]);
            *data << uint32(QueueSlot);
            *data << uint32(bg->isArena() ? bg->GetMaxPlayersPerTeam() : 1);
            data->WriteByteSeq(bg_guid[2]);
            *data << uint32(bg->GetClientInstanceID()); // Client Instance ID            
            data->WriteByteSeq(player_guid[1]);
            data->WriteByteSeq(player_guid[0]);
            data->WriteByteSeq(player_guid[4]);
            data->WriteByteSeq(bg_guid[0]);
            data->WriteByteSeq(bg_guid[6]);
            data->WriteByteSeq(bg_guid[7]);
            data->WriteByteSeq(player_guid[6]);
            data->WriteByteSeq(player_guid[7]);
            break;
        }
        case STATUS_WAIT_JOIN:
        {
            data->Initialize(SMSG_BATTLEFIELD_STATUS_NEED_CONFIRMATION, 44);

            uint8 role = pPlayer->GetBattleGroundRoles();

            data->WriteBit(bg_guid[4]);
            data->WriteBit(bg_guid[6]);
            data->WriteBit(bg_guid[1]);
            data->WriteBit(bg_guid[3]);
            data->WriteBit(player_guid[2]);
            data->WriteBit(player_guid[3]);
            data->WriteBit(bg_guid[2]);
            data->WriteBit(player_guid[5]);
            data->WriteBit(role == ROLE_DAMAGE);
            data->WriteBit(bg_guid[0]);
            data->WriteBit(bg_guid[7]);
            data->WriteBit(bg_guid[5]);
            data->WriteBit(player_guid[7]);
            data->WriteBit(player_guid[0]);
            data->WriteBit(player_guid[6]);
            data->WriteBit(bg->isRated()); // byte38
            data->WriteBit(player_guid[1]);
            data->WriteBit(player_guid[4]);

            data->FlushBits();

            if (role != ROLE_DAMAGE)
            {
                // Client use sent value like this
                // Role = 1 << (val + 1)
                if (role == ROLE_TANK)
                    *data << uint8(0);
                else
                    *data << uint8(1);
            }

            *data << uint32(bg->GetClientInstanceID());
            data->WriteByteSeq(bg_guid[5]);
            *data << uint8(0); // byte32
            *data << uint32(bg->isArena() ? bg->GetMaxPlayersPerTeam() : 1);
            data->WriteByteSeq(player_guid[0]);
            data->WriteByteSeq(player_guid[6]);
            *data << uint32(Time2);                     // Time until closed
            *data << uint8(bg->GetMinLevel());
            data->WriteByteSeq(player_guid[1]);
            data->WriteByteSeq(player_guid[5]);
            data->WriteByteSeq(player_guid[2]);
            data->WriteByteSeq(bg_guid[4]);
            data->WriteByteSeq(player_guid[7]);
            data->WriteByteSeq(bg_guid[1]);
            data->WriteByteSeq(bg_guid[7]);
            data->WriteByteSeq(bg_guid[0]);
             *data << uint32(Time1);
            data->WriteByteSeq(bg_guid[6]);
            data->WriteByteSeq(bg_guid[2]);
            *data << uint8(0); // byte30
            data->WriteByteSeq(player_guid[3]);
            data->WriteByteSeq(player_guid[4]);
            *data << uint32(bg->GetMapId());
            *data << uint32(QueueSlot);
            data->WriteByteSeq(bg_guid[3]);
            break;
        }
        case STATUS_IN_PROGRESS:
        {
            data->Initialize(SMSG_BATTLEFIELD_STATUS_ACTIVE, 49);

            data->WriteBit(pPlayer->GetBGTeam() == HORDE ? 0 : 1);     // Battlefield Faction ( 0 horde, 1 alliance )
            data->WriteBit(bg_guid[1]);
            data->WriteBit(bg_guid[7]);
            data->WriteBit(player_guid[0]);
            data->WriteBit(player_guid[5]);
            data->WriteBit(player_guid[3]);
            data->WriteBit(bg_guid[0]);
            data->WriteBit(player_guid[2]);
            data->WriteBit(player_guid[7]);
            data->WriteBit(bg_guid[4]);
            data->WriteBit(bg->isRated()); // byte38
            data->WriteBit(player_guid[6]);
            data->WriteBit(bg_guid[2]);
            data->WriteBit(bg_guid[5]);
            data->WriteBit(bg_guid[6]);
            data->WriteBit(bg_guid[3]);
            data->WriteBit(player_guid[4]);
            data->WriteBit(player_guid[1]);
            data->FlushBits();

            *data << uint8(0); // byte30
            *data << uint8(bg->GetMinLevel());          // Min Level
            *data << uint8(0); // byte32
            data->WriteByteSeq(player_guid[7]);
            data->WriteByteSeq(player_guid[0]);
            data->WriteByteSeq(player_guid[1]);
            data->WriteByteSeq(player_guid[2]);
            data->WriteByteSeq(bg_guid[7]);
            *data << uint32(Time2); // Elapsed Time
            *data << uint32(bg->GetClientInstanceID()); // Client Instance ID            
            *data << uint32(bg->GetMapId());            // Map Id
            data->WriteByteSeq(bg_guid[4]);
            data->WriteByteSeq(bg_guid[3]);
            data->WriteByteSeq(bg_guid[1]);
            *data << uint32(bg->isArena() ? bg->GetMaxPlayersPerTeam() : 1);                 // Queue slot
            data->WriteByteSeq(player_guid[3]);
            data->WriteByteSeq(player_guid[4]);
            *data << uint32(Time1); // Time to Close
            data->WriteByteSeq(bg_guid[2]);
            data->WriteByteSeq(player_guid[5]);
            data->WriteByteSeq(bg_guid[6]);
            *data << uint32(GetMSTimeDiffToNow(Time2));
            data->WriteByteSeq(bg_guid[5]);
            *data << uint32(QueueSlot);
            data->WriteByteSeq(bg_guid[0]);
            data->WriteByteSeq(player_guid[6]);
            break;
        }
        case STATUS_WAIT_LEAVE:
        {
          /*  data->Initialize(SMSG_BATTLEFIELD_STATUS_WAIT_FOR_GROUPS, 48);

            *data << uint8(0);                          // unk
            *data << uint32(bg->GetStatus());           // Status
            *data << uint32(QueueSlot);                 // Queue slot
            *data << uint32(Time1);                     // Time until closed
            *data << uint32(0);                         // unk
            *data << uint8(0);                          // unk
            *data << uint8(0);                          // unk
            *data << uint8(bg->GetMinLevel());          // Min Level
            *data << uint8(0);                          // unk
            *data << uint8(0);                          // unk
            *data << uint32(bg->GetMapId());            // Map Id
            *data << uint32(Time2);                     // Time
            *data << uint8(0);                          // unk

            data->WriteBit(bg_guid[0]);
            data->WriteBit(bg_guid[1]);
            data->WriteBit(bg_guid[7]);
            data->WriteBit(player_guid[7]);
            data->WriteBit(player_guid[0]);
            data->WriteBit(bg_guid[4]);
            data->WriteBit(player_guid[6]);
            data->WriteBit(player_guid[2]);
            data->WriteBit(player_guid[3]);
            data->WriteBit(bg_guid[3]);
            data->WriteBit(player_guid[4]);
            data->WriteBit(bg_guid[5]);
            data->WriteBit(player_guid[5]);
            data->WriteBit(bg_guid[2]);
            data->WriteBit(bg->isRated());              // Is Rated
            data->WriteBit(player_guid[1]);
            data->WriteBit(bg_guid[6]);

            data->FlushBits();

            data->WriteByteSeq(player_guid[0]);
            data->WriteByteSeq(bg_guid[4]);
            data->WriteByteSeq(player_guid[3]);
            data->WriteByteSeq(bg_guid[1]);
            data->WriteByteSeq(bg_guid[0]);
            data->WriteByteSeq(bg_guid[2]);
            data->WriteByteSeq(player_guid[2]);
            data->WriteByteSeq(bg_guid[7]);
            data->WriteByteSeq(player_guid[1]);
            data->WriteByteSeq(player_guid[6]);
            data->WriteByteSeq(bg_guid[6]);
            data->WriteByteSeq(bg_guid[5]);
            data->WriteByteSeq(player_guid[5]);
            data->WriteByteSeq(player_guid[4]);
            data->WriteByteSeq(player_guid[7]);
            data->WriteByteSeq(bg_guid[3]);*/
            break;
        }
    }
}

void BattlegroundMgr::BuildPvpLogDataPacket(WorldPacket* data, Battleground* bg)
{
    uint8 isRated = (bg->isRated() ? 1 : 0);               // type (normal=0/rated=1) -- ATM arena or bg, RBG NYI
    uint8 isArena = (bg->isArena() ? 1 : 0);               // Arena names
    int32 count = 0;
    uint8 counta2 = 0;
    uint8 counth2 = 0;

    data->Initialize(SMSG_PVP_LOG_DATA);

    Battleground::BattlegroundScoreMap::const_iterator itr2 = bg->GetPlayerScoresBegin();
    for (Battleground::BattlegroundScoreMap::const_iterator itr = itr2; itr != bg->GetPlayerScoresEnd();)
    {
        itr2 = itr++;
        if (!bg->IsPlayerInBattleground(itr2->first))
        {
            sLog->outError(LOG_FILTER_BATTLEGROUND, "Player " UI64FMTD " has scoreboard entry for battleground %u but is not in battleground!", itr->first, bg->GetTypeID(true));
            continue;
        }

        Player* player = ObjectAccessor::FindPlayer(itr2->first);

        if (!player)
            continue;

        count++;

        if (player->GetBGTeam() == ALLIANCE)
            ++counta2;
        else
            ++counth2;
    }

    *data << uint8(counta2);
    *data << uint8(counth2);
    data->WriteBit(bg->GetStatus() == STATUS_WAIT_LEAVE);    // If Ended
    data->WriteBit(isRated);                                 // HaveArenaData
    data->WriteBits(count, 19);

    itr2 = bg->GetPlayerScoresBegin();
    for (Battleground::BattlegroundScoreMap::const_iterator itr = itr2; itr != bg->GetPlayerScoresEnd();)
    {
        itr2 = itr++;
        if (!bg->IsPlayerInBattleground(itr2->first))
        {
            sLog->outError(LOG_FILTER_BATTLEGROUND, "Player " UI64FMTD " has scoreboard entry for battleground %u but is not in battleground!", itr->first, bg->GetTypeID(true));
            continue;
        }

        ObjectGuid guid = itr2->first;
        Player* player = ObjectAccessor::FindPlayer(itr2->first);

        if (!player)
            continue;

        data->WriteBit(guid[6]);
        data->WriteBit(guid[0]);
        data->WriteBit(0); // unkbit1
        data->WriteBit(guid[1]);
        data->WriteBit(guid[7]);
        data->WriteBit(1); // unkbit2

        if (isArena)
            data->WriteBit(bg->GetPlayerTeam(guid) == ALLIANCE);
        else
            data->WriteBit(player->GetTeam() == ALLIANCE);

        data->WriteBit(guid[2]);
        data->WriteBit(0); // unkbit4
        data->WriteBit(guid[4]);

        switch (bg->GetTypeID(true))                             // Custom values
        {
            case BATTLEGROUND_RB:
                switch (bg->GetMapId())
                {
                    case 489:
                    case 529:
                    case 607:
                    case 628:
                    case 726:
                        data->WriteBits(0x00000002, 22);
                        break;
                    case 761:
                        data->WriteBits(0x00000002, 22);
                        break;
                    case 30:
                        data->WriteBits(0x00000005, 22);
                        break;
                    case 566:
                        data->WriteBits(0x00000001, 22);
                        break;
                    default:
                        data->WriteBits(0, 22);
                        break;
                }
                break;
            case BATTLEGROUND_AV:
                data->WriteBits(0x00000005, 22);
                break;
            case BATTLEGROUND_EY:
                data->WriteBits(0x00000001, 22);
                break;
            case BATTLEGROUND_WS:
            case BATTLEGROUND_AB:
            case BATTLEGROUND_SA:
            case BATTLEGROUND_IC:
            case BATTLEGROUND_TP:
            case BATTLEGROUND_BFG:
            case BATTLEGROUND_KT:
                data->WriteBits(0x00000002, 22);
                break;
            default:
                data->WriteBits(0, 22);
                break;
        }

        data->WriteBit(guid[5]);
        data->WriteBit(guid[3]);
        data->WriteBit(!isArena); // HaveBonusData
        data->WriteBit(0); // unkbit6
        data->WriteBit(isArena);
    }

    data->WriteBit(false);                                 // HaveArenaData2

    /*if (isArena)
    {
        ArenaTeam* at1 = sArenaTeamMgr->GetArenaTeamById(bg->GetArenaTeamIdByIndex(0));
        ArenaTeam* at2 = sArenaTeamMgr->GetArenaTeamById(bg->GetArenaTeamIdByIndex(1));
        ObjectGuid TeamGuid1 = 0;
        ObjectGuid TeamGuid2 = 0;

        if (at1)
            data->WriteBits(at1->GetName().length(), 7);
        else
            data->WriteBits(0, 7);
        data->WriteBit(TeamGuid1[2]);
        data->WriteBit(TeamGuid2[0]);
        data->WriteBit(TeamGuid2[7]);
        data->WriteBit(TeamGuid2[2]);
        data->WriteBit(TeamGuid1[5]);
        data->WriteBit(TeamGuid1[6]);
        data->WriteBit(TeamGuid1[4]);
        data->WriteBit(TeamGuid1[3]);
        data->WriteBit(TeamGuid2[6]);
        data->WriteBit(TeamGuid1[1]);
        data->WriteBit(TeamGuid1[0]);
        data->WriteBit(TeamGuid2[1]);
        data->WriteBit(TeamGuid2[5]);
        if (at1)
            data->WriteBits(at2->GetName().length(), 7);
        else
            data->WriteBits(0, 7);
        data->WriteBit(TeamGuid2[4]);
        data->WriteBit(TeamGuid2[3]);
        data->WriteBit(TeamGuid1[7]);

        data->WriteByteSeq(TeamGuid2[4]);
        data->WriteByteSeq(TeamGuid2[0]);
        data->WriteByteSeq(TeamGuid1[6]);
        data->WriteByteSeq(TeamGuid2[3]);
        data->WriteByteSeq(TeamGuid2[6]);

        if (at2)
            data->WriteString(at2->GetName());

        data->WriteByteSeq(TeamGuid1[3]);

        if (at1)
            data->WriteString(at1->GetName());

        data->WriteByteSeq(TeamGuid2[7]);
        data->WriteByteSeq(TeamGuid1[1]);
        data->WriteByteSeq(TeamGuid1[5]);
        data->WriteByteSeq(TeamGuid1[7]);
        data->WriteByteSeq(TeamGuid2[5]);
        data->WriteByteSeq(TeamGuid1[4]);
        data->WriteByteSeq(TeamGuid2[1]);
        data->WriteByteSeq(TeamGuid1[2]);
        data->WriteByteSeq(TeamGuid1[0]);
        data->WriteByteSeq(TeamGuid2[2]);
    }*/

    itr2 = bg->GetPlayerScoresBegin();
    for (Battleground::BattlegroundScoreMap::const_iterator itr = itr2; itr != bg->GetPlayerScoresEnd();)
    {
        itr2 = itr++;
        if (!bg->IsPlayerInBattleground(itr2->first))
        {
            sLog->outError(LOG_FILTER_BATTLEGROUND, "Player " UI64FMTD " has scoreboard entry for battleground %u but is not in battleground!", itr->first, bg->GetTypeID(true));
            continue;
        }

        ObjectGuid guid = itr2->first;
        Player* player = ObjectAccessor::FindPlayer(itr2->first);

        if (!player)
            continue;

        *data << uint32(itr2->second->KillingBlows);
        data->WriteByteSeq(guid[4]);
        data->WriteByteSeq(guid[0]);

        if (!isArena) // HaveBonusData
        {
            *data << uint32(itr2->second->BonusHonor / 100);
            *data << uint32(itr2->second->Deaths);
            *data << uint32(itr2->second->HonorableKills);
        }

        *data << uint32(itr2->second->HealingDone);             // healing done

        //if (unkbit6)
        //    *data << uint32(dword15);

        data->WriteByteSeq(guid[1]);
        *data << uint32(player->GetPrimaryTalentTree(player->GetActiveSpec()));
        data->WriteByteSeq(guid[7]);

        //if (unkbit4)
        //    *data << uint32(dword13);
        //if (unkbit1)
        //    *data << uint32(dword9);

        data->WriteByteSeq(guid[6]);
        data->WriteByteSeq(guid[5]);
        data->WriteByteSeq(guid[3]);

        switch (bg->GetTypeID(true))                             // Custom values
        {
            case BATTLEGROUND_RB:
                switch (bg->GetMapId())
                {
                    case 489:
                        *data << uint32(((BattlegroundWGScore*)itr2->second)->FlagCaptures);        // flag captures
                        *data << uint32(((BattlegroundWGScore*)itr2->second)->FlagReturns);         // flag returns
                        break;
                    case 566:
                        *data << uint32(((BattlegroundEYScore*)itr2->second)->FlagCaptures);        // flag captures
                        break;
                    case 529:
                        *data << uint32(((BattlegroundABScore*)itr2->second)->BasesAssaulted);      // bases asssulted
                        *data << uint32(((BattlegroundABScore*)itr2->second)->BasesDefended);       // bases defended
                        break;
                    case 30:
                        *data << uint32(((BattlegroundAVScore*)itr2->second)->GraveyardsAssaulted); // GraveyardsAssaulted
                        *data << uint32(((BattlegroundAVScore*)itr2->second)->GraveyardsDefended);  // GraveyardsDefended
                        *data << uint32(((BattlegroundAVScore*)itr2->second)->TowersAssaulted);     // TowersAssaulted
                        *data << uint32(((BattlegroundAVScore*)itr2->second)->TowersDefended);      // TowersDefended
                        *data << uint32(((BattlegroundAVScore*)itr2->second)->MinesCaptured);       // MinesCaptured
                        break;
                    case 607:
                        *data << uint32(((BattlegroundSAScore*)itr2->second)->demolishers_destroyed);
                        *data << uint32(((BattlegroundSAScore*)itr2->second)->gates_destroyed);
                        break;
                    case 628:                                   // IC
                        *data << uint32(((BattlegroundICScore*)itr2->second)->BasesAssaulted);       // bases asssulted
                        *data << uint32(((BattlegroundICScore*)itr2->second)->BasesDefended);        // bases defended
                        break;
                    case 726:
                        *data << uint32(((BattlegroundTPScore*)itr2->second)->FlagCaptures);         // flag captures
                        *data << uint32(((BattlegroundTPScore*)itr2->second)->FlagReturns);          // flag returns
                        break;
                    case 761:
                        *data << uint32(((BattlegroundBFGScore*)itr2->second)->BasesAssaulted);      // bases asssulted
                        *data << uint32(((BattlegroundBFGScore*)itr2->second)->BasesDefended);       // bases defended
                        break;
                }
                break;
            case BATTLEGROUND_AV:
                *data << uint32(((BattlegroundAVScore*)itr2->second)->GraveyardsAssaulted); // GraveyardsAssaulted
                *data << uint32(((BattlegroundAVScore*)itr2->second)->GraveyardsDefended);  // GraveyardsDefended
                *data << uint32(((BattlegroundAVScore*)itr2->second)->TowersAssaulted);     // TowersAssaulted
                *data << uint32(((BattlegroundAVScore*)itr2->second)->TowersDefended);      // TowersDefended
                *data << uint32(((BattlegroundAVScore*)itr2->second)->MinesCaptured);       // MinesCaptured
                break;
            case BATTLEGROUND_WS:
                *data << uint32(((BattlegroundWGScore*)itr2->second)->FlagCaptures);        // flag captures
                *data << uint32(((BattlegroundWGScore*)itr2->second)->FlagReturns);         // flag returns
                break;
            case BATTLEGROUND_AB:
                *data << uint32(((BattlegroundABScore*)itr2->second)->BasesAssaulted);      // bases asssulted
                *data << uint32(((BattlegroundABScore*)itr2->second)->BasesDefended);       // bases defended
                break;
            case BATTLEGROUND_EY:
                *data << uint32(((BattlegroundEYScore*)itr2->second)->FlagCaptures);        // flag captures
                break;
            case BATTLEGROUND_SA:
                *data << uint32(((BattlegroundSAScore*)itr2->second)->demolishers_destroyed);
                *data << uint32(((BattlegroundSAScore*)itr2->second)->gates_destroyed);
                break;
            case BATTLEGROUND_IC:
                *data << uint32(((BattlegroundICScore*)itr2->second)->BasesAssaulted);       // bases asssulted
                *data << uint32(((BattlegroundICScore*)itr2->second)->BasesDefended);        // bases defended
                break;
            case BATTLEGROUND_TP:
                *data << uint32(((BattlegroundTPScore*)itr2->second)->FlagCaptures);         // flag captures
                *data << uint32(((BattlegroundTPScore*)itr2->second)->FlagReturns);          // flag returns
                break;
            case BATTLEGROUND_BFG:
                *data << uint32(((BattlegroundBFGScore*)itr2->second)->BasesAssaulted);      // bases asssulted
                *data << uint32(((BattlegroundBFGScore*)itr2->second)->BasesDefended);       // bases defended
                break;
            case BATTLEGROUND_KT:
                *data << uint32(((BattleGroundKTScore*)itr2->second)->OrbHandles);
                *data << uint32(((BattleGroundKTScore*)itr2->second)->Score * 10);
                break;
        }

        *data << uint32(itr2->second->DamageDone);              // damage done
        data->WriteByteSeq(guid[2]);
        if (isArena)
            *data << int32(itr2->second->RatingChange);
    }

    if (isRated)                                             // arena TODO : Fix Order on Rated Implementation
    {
        // it seems this must be according to BG_WINNER_A/H and _NOT_ BG_TEAM_A/H
        for (int8 i = BG_TEAMS_COUNT - 1; i >= 0; --i)
        {
            int32 rating_change = bg->GetArenaTeamRatingChangeByIndex(i);

            uint32 pointsLost = rating_change < 0 ? -rating_change : 0;
            uint32 pointsGained = rating_change > 0 ? rating_change : 0;
            uint32 MatchmakerRating = bg->GetArenaMatchmakerRatingByIndex(i);

            if (i == 1)
            {
                *data << uint32(pointsLost);                    // Rating Lost
                *data << uint32(MatchmakerRating);              // Matchmaking Value
                *data << uint32(pointsGained);                  // Rating gained
            }
            else
            {
                *data << uint32(pointsLost);                    // Rating Lost
                *data << uint32(MatchmakerRating);              // Matchmaking Value
                *data << uint32(pointsGained);                  // Rating gained
            }

            sLog->outDebug(LOG_FILTER_BATTLEGROUND, "rating change: %d", rating_change);
        }
    }

    if (bg->GetStatus() == STATUS_WAIT_LEAVE)
        *data << uint8(bg->GetWinner());                    // who win
}

void BattlegroundMgr::BuildStatusFailedPacket(WorldPacket* data, Battleground* bg, Player* player, uint8 QueueSlot, GroupJoinBattlegroundResult result)
{
    ObjectGuid player_guid = player->GetGUID(); // player who caused the error
    ObjectGuid bg_guid = bg->GetGUID();
    ObjectGuid unkguid = 0;

    data->Initialize(SMSG_BATTLEFIELD_STATUS_FAILED);

    data->WriteBit(player_guid[3]);
    data->WriteBit(bg_guid[3]);
    data->WriteBit(player_guid[5]);
    data->WriteBit(player_guid[1]);
    data->WriteBit(player_guid[4]);
    data->WriteBit(bg_guid[2]);
    data->WriteBit(player_guid[0]);
    data->WriteBit(bg_guid[6]);
    data->WriteBit(player_guid[6]);
    data->WriteBit(unkguid[0]);
    data->WriteBit(player_guid[2]);
    data->WriteBit(unkguid[4]);
    data->WriteBit(unkguid[3]);
    data->WriteBit(unkguid[5]);
    data->WriteBit(unkguid[1]);
    data->WriteBit(player_guid[7]);
    data->WriteBit(bg_guid[1]);
    data->WriteBit(unkguid[2]);
    data->WriteBit(bg_guid[7]);
    data->WriteBit(unkguid[7]);
    data->WriteBit(unkguid[6]);
    data->WriteBit(bg_guid[5]);
    data->WriteBit(bg_guid[4]);
    data->WriteBit(bg_guid[0]);
    data->FlushBits();

    data->WriteByteSeq(player_guid[7]);
    data->WriteByteSeq(unkguid[5]);
    data->WriteByteSeq(player_guid[6]);
    data->WriteByteSeq(unkguid[3]);
    data->WriteByteSeq(player_guid[5]);
    data->WriteByteSeq(unkguid[2]);
    data->WriteByteSeq(bg_guid[6]);
    data->WriteByteSeq(unkguid[6]);
    data->WriteByteSeq(player_guid[3]);
    data->WriteByteSeq(bg_guid[1]);
    data->WriteByteSeq(bg_guid[7]);

    *data << uint32(QueueSlot);                 // Queue slot

    data->WriteByteSeq(unkguid[4]);
    data->WriteByteSeq(bg_guid[4]);
    data->WriteByteSeq(unkguid[0]);
    data->WriteByteSeq(player_guid[4]);
    data->WriteByteSeq(player_guid[2]);

    *data << uint32(bg->isArena() ? bg->GetMaxPlayersPerTeam() : 1);                         // Unk, always 1 

    data->WriteByteSeq(bg_guid[0]);
    data->WriteByteSeq(unkguid[1]);
    data->WriteByteSeq(bg_guid[5]);

    *data << uint32(result);

    data->WriteByteSeq(player_guid[0]);
    data->WriteByteSeq(unkguid[7]);
    data->WriteByteSeq(bg_guid[2]);
    data->WriteByteSeq(player_guid[1]);

    *data << uint32(player->GetBattlegroundQueueJoinTime(bg->GetTypeID())); // Join Time RANDOM

    data->WriteByteSeq(bg_guid[3]);
}

void BattlegroundMgr::BuildUpdateWorldStatePacket(WorldPacket* data, uint32 field, uint32 value)
{
    data->Initialize(SMSG_UPDATE_WORLD_STATE, 4+4);
    *data << uint32(field);
    *data << uint32(value);
    data->WriteBit(0);
}

void BattlegroundMgr::BuildPlayerLeftBattlegroundPacket(WorldPacket* data, uint64 guid)
{
    ObjectGuid guidBytes = guid;

    data->Initialize(SMSG_BATTLEGROUND_PLAYER_LEFT, 8);
    
    uint8 bitOrder[8] = { 5, 6, 1, 7, 0, 2, 4, 3 };
    data->WriteBitInOrder(guidBytes, bitOrder);
    
    uint8 byteOrder[8] = { 6, 1, 5, 7, 4, 0, 3, 2 };
    data->WriteBytesSeq(guidBytes, byteOrder);
}

void BattlegroundMgr::BuildPlayerJoinedBattlegroundPacket(WorldPacket* data, uint64 guid)
{
    data->Initialize(SMSG_BATTLEGROUND_PLAYER_JOINED, 8);
    ObjectGuid playerGuid = guid;

    uint8 bits[8] = { 0, 1, 7, 2, 4, 3, 5, 6 };
    data->WriteBitInOrder(playerGuid, bits);

    uint8 bytes[8] = { 2, 3, 1, 0, 5, 4, 7, 6 };
    data->WriteBytesSeq(playerGuid, bytes);
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

    if (bg_template->isArena())
        selectionWeights = &m_ArenaSelectionWeights;
    else if (bgTypeId == BATTLEGROUND_RB)
    {
        selectionWeights = &m_BGSelectionWeights;
        isRandom = true;
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
        case BATTLEGROUND_BE:
            bg = new BattlegroundBE(*(BattlegroundBE*)bg_template);
            break;
        case BATTLEGROUND_AA:
            bg = new BattlegroundAA(*(BattlegroundAA*)bg_template);
            break;
        case BATTLEGROUND_EY:
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
        default:
            //error, but it is handled few lines above
            return 0;
    }

    // set battelground difficulty before initialization
    bg->SetBracket(bracketEntry);

    // generate a new instance id
    bg->SetInstanceID(sMapMgr->GenerateInstanceId()); // set instance id
    bg->SetClientInstanceID(CreateClientVisibleInstanceId(isRandom ? BATTLEGROUND_RB : bgTypeId, bracketEntry->GetBracketId()));

    // reset the new bg (set status to status_wait_queue from status_none)
    bg->Reset();

    // start the joining of the bg
    bg->SetStatus(STATUS_WAIT_JOIN);
    bg->SetArenaType(arenaType);
    bg->SetRated(isRated);
    bg->SetRandom(isRandom);
    bg->SetTypeID(isRandom ? BATTLEGROUND_RB : bgTypeId);
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
        case BATTLEGROUND_BE: bg = new BattlegroundBE; break;
        case BATTLEGROUND_AA: bg = new BattlegroundAA; break;
        case BATTLEGROUND_EY: bg = new BattlegroundEY; break;
        case BATTLEGROUND_RL: bg = new BattlegroundRL; break;
        case BATTLEGROUND_SA: bg = new BattlegroundSA; break;
        case BATTLEGROUND_DS: bg = new BattlegroundDS; break;
        case BATTLEGROUND_RV: bg = new BattlegroundRV; break;
        case BATTLEGROUND_IC: bg = new BattlegroundIC; break;
        case BATTLEGROUND_TP: bg = new BattlegroundTP; break;
        case BATTLEGROUND_BFG: bg = new BattlegroundBFG; break;
        case BATTLEGROUND_RB: bg = new BattlegroundRB; break;
        case BATTLEGROUND_KT: bg = new BattlegroundKT; break;
        default:
            bg = new Battleground;
            break;
    }

    bg->SetMapId(data.MapID);
    bg->SetTypeID(data.bgTypeId);
    bg->InitGUID();
    bg->SetInstanceID(0);
    bg->SetArenaorBGType(data.IsArena);
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
        else if (data.bgTypeId == BATTLEGROUND_AA || data.bgTypeId == BATTLEGROUND_RB)
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
        else if (data.bgTypeId == BATTLEGROUND_AA || data.bgTypeId == BATTLEGROUND_RB)
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
        else if (data.bgTypeId != BATTLEGROUND_RB)
            m_BGSelectionWeights[data.bgTypeId] = selectionWeight;
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
        case BATTLEGROUND_TV:
            return BATTLEGROUND_QUEUE_TV;
        case BATTLEGROUND_AA:
        case BATTLEGROUND_NA:
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
        case BATTLEGROUND_QUEUE_TV:
            return BATTLEGROUND_TV;
        case BATTLEGROUND_QUEUE_2v2:
        case BATTLEGROUND_QUEUE_3v3:
        case BATTLEGROUND_QUEUE_5v5:
            return BATTLEGROUND_AA;
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
