////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef BATTLEGROUND_PACKET_FACTORY_HPP
# define BATTLEGROUND_PACKET_FACTORY_HPP

# include "BattlegroundAV.h"
# include "BattlegroundAB.h"
# include "BattlegroundDG.h"
# include "BattlegroundEY.h"
# include "BattlegroundWS.h"
# include "BattlegroundNA.h"
# include "BattlegroundBE.h"
# include "BattlegroundAA.h"
# include "BattlegroundRL.h"
# include "BattlegroundSA.h"
# include "BattlegroundDS.h"
# include "BattlegroundRV.h"
# include "BattlegroundIC.h"
# include "BattlegroundRB.h"
# include "BattlegroundRBG.h"
# include "BattlegroundTP.h"
# include "BattlegroundBFG.h"
# include "BattlegroundKT.h"
# include "BattlegroundSM.h"
# include "BattlegroundTV.h"
# include "BattlegroundTTP.h"

# include "LFG.h"

namespace MS
{
    namespace Battlegrounds
    {
        class PacketFactory
        {
        public:
            static void Status(WorldPacket* p_Data, Battleground* p_BG, Player* p_Player, uint8 p_QueueSlot, uint8 p_StatusID, uint32 p_Time1, uint32 p_Time2, uint8 /*p_Arenatype*/, bool p_IsSkirmish)
            {
                /// we can be in 2 queues in same time...
                if (!p_BG)
                    p_StatusID = STATUS_NONE;

                uint64 l_PlayerGuid = p_Player->GetGUID();
                uint64 l_BGQueueID = p_BG ? p_BG->GetGUID() : 0;

                bool l_RegisteredMatch = p_IsSkirmish;
                if (!p_IsSkirmish && p_BG != nullptr)
                    l_RegisteredMatch = p_BG->isArena() ? p_BG->IsSkirmish() : p_BG->IsRatedBG();

                // We haven't default battleground template for arena skirmish, we must add the queueType by hand.
                if (p_IsSkirmish)
                    l_BGQueueID |= (uint8(BattlegroundQueueType::ArenaSkirmish) & 0xF) << 16;

                if (p_BG && p_BG->IsWargame())
                    l_BGQueueID |= (uint8(BattlegroundQueueType::WarGame) & 0xF) << 16;

                switch (p_StatusID)
                {
                    case STATUS_NONE:
                    {
                        p_Data->Initialize(SMSG_BATTLEFIELD_STATUS_NONE);

                        p_Data->appendPackGUID(l_PlayerGuid);                                           ///< Requester Guid
                        *p_Data << uint32(p_QueueSlot);                                                 ///< Instance ID

                        *p_Data << uint32(1);                                                           ///< Type

                        *p_Data << uint32(p_Time1);                                                     ///< Time

                        break;
                    }
                    case STATUS_WAIT_QUEUE:
                    {
                        p_Data->Initialize(SMSG_BATTLEFIELD_STATUS_QUEUED);

                        p_Data->appendPackGUID(l_PlayerGuid);                                     ///< Requester Guid
                        *p_Data << uint32(p_QueueSlot);                                           ///< Instance ID
                        *p_Data << uint32(1);                                                     ///< Type
                        *p_Data << uint32(p_Time2);                                               ///< Time

                        *p_Data << uint64(l_BGQueueID);                                           ///< QueueID
                        *p_Data << uint8(p_BG->GetMinLevel());                                    ///< RangeMin
                        *p_Data << uint8(p_BG->GetMaxLevel());                                    ///< RangeMax
                        *p_Data << uint8(p_BG->isArena() ? p_BG->GetMaxPlayersPerTeam() : 1);     ///< Team Size
                        *p_Data << uint32(p_BG->GetClientInstanceID());                           ///< Id
                        p_Data->WriteBit(l_RegisteredMatch);                                      ///< Registered Match
                        p_Data->WriteBit(p_BG->UseTournamentRules());                             ///< Tournament Rules
                        p_Data->FlushBits();

                        *p_Data << uint32(p_Time1);                                               ///< Average Wait Time
                        *p_Data << uint32(GetMSTimeDiffToNow(p_Time2));                           ///< Wait Time

                        p_Data->WriteBit(false);                                                  ///< @TODO As Group
                        p_Data->WriteBit(1);                                                      ///< Eligible For Matchmaking
                        p_Data->WriteBit(false);                                                  ///< @TODO Suspended Queue
                        p_Data->FlushBits();

                        break;
                    }
                    case STATUS_WAIT_JOIN:
                    {
                        p_Data->Initialize(SMSG_BATTLEFIELD_STATUS_NEED_CONFIRMATION, 44);
                        uint8 l_Role = p_Player->GetBattleGroundRoles();

                        p_Data->appendPackGUID(l_PlayerGuid);                                     ///< Requester Guid
                        *p_Data << uint32(p_QueueSlot);                                           ///< Instance ID
                        *p_Data << uint32(1);                                                     ///< Type
                        *p_Data << uint32(p_Time2);                                               ///< Time

                        *p_Data << uint64(l_BGQueueID);                                           ///< QueueID
                        *p_Data << uint8(p_BG->GetMinLevel());                                    ///< RangeMin
                        *p_Data << uint8(p_BG->GetMaxLevel());                                    ///< RangeMax
                        *p_Data << uint8(p_BG->isArena() ? p_BG->GetMaxPlayersPerTeam() : 1);     ///< Team Size
                        *p_Data << uint32(p_BG->GetClientInstanceID());                           ///< Id
                        p_Data->WriteBit(l_RegisteredMatch);                                      ///< Registered Match
                        p_Data->WriteBit(p_BG->UseTournamentRules());                             ///< Tournament Rules
                        p_Data->FlushBits();

                        *p_Data << uint32(p_BG->GetMapId());                                      ///< Map ID
                        *p_Data << uint32(p_Time1);                                               ///< Time until closed

                        if (l_Role & LFG_ROLEMASK_TANK)
                            *p_Data << uint8(ROLE_TANK);
                        else if (l_Role & LFG_ROLEMASK_HEALER)
                            *p_Data << uint8(ROLE_HEALER);
                        else if (l_Role & LFG_ROLEMASK_DAMAGE)
                            *p_Data << uint8(ROLE_DAMAGE);
                        else
                            *p_Data << uint8(ROLE_DAMAGE);

                        break;
                    }
                    case STATUS_IN_PROGRESS:
                    {
                        p_Data->Initialize(SMSG_BATTLEFIELD_STATUS_ACTIVE, 49);

                        p_Data->appendPackGUID(l_PlayerGuid);                                     ///< Requester Guid
                        *p_Data << uint32(p_QueueSlot);                                           ///< Instance ID
                        *p_Data << uint32(1);                                                     ///< Type
                        *p_Data << uint32(p_Time2);                                               ///< Time

                        *p_Data << uint64(l_BGQueueID);                                           ///< QueueID
                        *p_Data << uint8(p_BG->GetMinLevel());                                    ///< RangeMin
                        *p_Data << uint8(p_BG->GetMaxLevel());                                    ///< RangeMax
                        *p_Data << uint8(p_BG->isArena() ? p_BG->GetMaxPlayersPerTeam() : 1);     ///< Team Size
                        *p_Data << uint32(p_BG->GetClientInstanceID());                           ///< Id
                        p_Data->WriteBit(l_RegisteredMatch);                                      ///< Registered Match
                        p_Data->WriteBit(p_BG->UseTournamentRules());                             ///< Tournament Rules
                        p_Data->FlushBits();

                        *p_Data << uint32(p_BG->GetMapId());                                      ///< Map Id
                        *p_Data << uint32(0);                                                     ///< Time to Close
                        *p_Data << uint32(p_BG->GetElapsedTime());                                ///< Elapsed Time
                        p_Data->WriteBit(p_Player->GetBGTeam() == HORDE ? 0 : 1);                 ///< Battlefield Faction ( 0 horde, 1 alliance )
                        p_Data->WriteBit(false);                                                  ///< @TODO Left early
                        p_Data->FlushBits();

                        break;
                    }
                    case STATUS_WAIT_LEAVE:
                    {
                        p_Data->Initialize(SMSG_BATTLEFIELD_STATUS_WAIT_FOR_GROUPS, 48);

                        p_Data->appendPackGUID(l_PlayerGuid);                                     ///< Requester Guid
                        *p_Data << uint32(p_QueueSlot);                                           ///< Instance ID
                        *p_Data << uint32(1);                                                     ///< Type
                        *p_Data << uint32(p_Time2);                                               ///< Time

                        *p_Data << uint64(l_BGQueueID);                                           ///< QueueID
                        *p_Data << uint8(p_BG->GetMinLevel());                                    ///< RangeMin
                        *p_Data << uint8(p_BG->GetMaxLevel());                                    ///< RangeMax
                        *p_Data << uint8(p_BG->isArena() ? p_BG->GetMaxPlayersPerTeam() : 1);     ///< Team Size
                        *p_Data << uint32(p_BG->GetClientInstanceID());                           ///< Id
                        p_Data->WriteBit(l_RegisteredMatch);                                      ///< Registered Match
                        p_Data->WriteBit(p_BG->UseTournamentRules());                             ///< Tournament Rules
                        p_Data->FlushBits();

                        *p_Data << uint32(p_BG->GetMapId());                                      ///< Map Id
                        *p_Data << uint32(p_Time1);                                               ///< Time to Close

                        for (uint32 l_I = 0; l_I < TeamsCount::Value; l_I++)
                        {
                            *p_Data << uint8(0);                                                  ///< @TODO TotalPlayers
                            *p_Data << uint8(0);                                                  ///< @TODO AwaitingPlayers
                        }

                        break;
                    }
                }
            }

            static void PvpLogData(WorldPacket * p_Data, Battleground * p_BG)
            {
                int32 l_PlayersCount = 0;

                uint8 l_AlliancePlayerCount = 0;
                uint8 l_HordePlayerCount = 0;
                uint8 l_IsSkirmish = (p_BG->IsSkirmish() ? 1 : 0);           ///< type (normal=0/skirmish(=1) -- ATM arena or bg, RBG NYI
                uint8 l_IsArena = (p_BG->isArena() ? 1 : 0);               ///< Arena names
                uint8 l_IsRatedBg = (p_BG->IsRatedBG() ? 1 : 0);

                p_Data->Initialize(SMSG_PVPLOG_DATA, 5 * 1024);

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

                bool l_HasHonor = !l_IsArena;
                bool l_HasRatings = (l_IsArena && !l_IsSkirmish) || l_IsRatedBg;
                bool l_HasRatingChange = (((l_IsArena && !l_IsSkirmish) || l_IsRatedBg) && p_BG->GetStatus() == STATUS_WAIT_LEAVE);
                bool l_HasPreMatchRating = false;
                bool l_HasPreMatchMMR = false;
                bool l_HasMMRChange = false;

                p_Data->WriteBit(l_HasRatings);                             ///< HaveArenaData
                p_Data->WriteBit(p_BG->GetStatus() == STATUS_WAIT_LEAVE);   ///< If Ended
                p_Data->FlushBits();

                *p_Data << uint32(l_PlayersCount);

                *p_Data << uint8(l_HordePlayerCount);
                *p_Data << uint8(l_AlliancePlayerCount);

                if (l_HasRatings)
                {
                    for (int8 l_I = 0; l_I < TeamsCount::Value; ++l_I)
                    {
                        int32 l_RatingChange = p_BG->GetArenaTeamRatingChangeByIndex(l_I);
                        int32 l_Prematch = l_RatingChange < 0 ? -l_RatingChange : 0;

                        *p_Data << int32(l_Prematch);                      ///< Rating Lost
                    }

                    for (int8 l_I = 0; l_I < TeamsCount::Value; ++l_I)
                    {
                        int32 l_RatingChange = p_BG->GetArenaTeamRatingChangeByIndex(l_I);
                        int32 l_Postmatch = l_RatingChange > 0 ? l_RatingChange : 0;

                        *p_Data << int32(l_Postmatch);                    ///< Rating Lost
                    }

                    for (int8 l_I = 0; l_I < TeamsCount::Value; ++l_I)
                    {
                        int32 l_PrematchMMR = p_BG->GetArenaMatchmakerRatingByIndex(l_I);

                        *p_Data << int32(l_PrematchMMR);                   ///< Matchmaking Value
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
                            l_Buffer << uint32(((BattleGroundKTScore*)l_ScoreBeginIT->second)->Score);
                            break;
                        case BATTLEGROUND_DG:
                            l_Buffer << uint32(((BattlegroundDGScore*)l_ScoreBeginIT->second)->m_AssaultedMines);
                            l_Buffer << uint32(((BattlegroundDGScore*)l_ScoreBeginIT->second)->m_DefendedMines);
                            l_Buffer << uint32(((BattlegroundDGScore*)l_ScoreBeginIT->second)->m_CapturedCart);
                            l_Buffer << uint32(((BattlegroundDGScore*)l_ScoreBeginIT->second)->m_ReturnedCart);
                            break;
                        case BATTLEGROUND_SM:
                            l_Buffer << uint32(((BattlegroundSMScore*)l_ScoreBeginIT->second)->MineCartCaptures);    // mine carts captured
                            break;
                        default:
                            break;
                    }

                    p_Data->appendPackGUID(l_Player->GetGUID());                                    ///< Player GUID

                    *p_Data << uint32(l_ScoreBeginIT->second->KillingBlows);
                    *p_Data << uint32(l_ScoreBeginIT->second->DamageDone);
                    *p_Data << uint32(l_ScoreBeginIT->second->HealingDone);
                    *p_Data << uint32(l_Buffer.size() / 4);                                         ///< Stats count
                    *p_Data << int32(l_Player->GetSpecializationId());                              ///< PrimaryTalentTree
                    *p_Data << int32(l_Player->getGender());                                        ///< Gender
                    *p_Data << int32(l_Player->getRace());                                          ///< Race

                    p_Data->append(l_Buffer);                                                       ///< Stats

                    if ((l_IsArena && !l_IsSkirmish) || l_IsRatedBg)
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

            static void StatusFailed(WorldPacket* p_Data, Battleground* p_BG, Player* p_Player, uint32 p_QueueSlot, GroupJoinBattlegroundResult p_Result)
            {
                uint64 l_ClientID = 0;

                p_Data->Initialize(SMSG_BATTLEFIELD_STATUS_FAILED);
                p_Data->appendPackGUID(p_Player->GetGUID());
                *p_Data << uint32(p_QueueSlot);                                                 ///< ID
                *p_Data << uint32(p_BG->isArena() ? p_BG->GetMaxPlayersPerTeam() : 1);          ///< Type
                *p_Data << uint32(p_Player->GetBattlegroundQueueJoinTime(GetSchedulerType(p_BG->GetTypeID())));   ///< Time

                *p_Data << uint64(p_BG->GetGUID());
                *p_Data << uint32(p_Result);
                p_Data->appendPackGUID(l_ClientID);
            }

            static void UpdateWorldState(WorldPacket* p_Data, uint32 field, uint32 value)
            {
                p_Data->Initialize(SMSG_UPDATE_WORLD_STATE, 4 + 4 + 1);
                *p_Data << uint32(field);
                *p_Data << uint32(value);
                p_Data->WriteBit(0);
                p_Data->FlushBits();
            }

            static void PlayerLeftBattleground(WorldPacket* p_Data, uint64 p_Guid)
            {
                p_Data->Initialize(SMSG_BATTLEGROUND_PLAYER_LEFT, 16 + 2);
                p_Data->appendPackGUID(p_Guid);
            }

            static void PlayerJoinedBattleground(WorldPacket* p_Data, uint64 p_Guid)
            {
                p_Data->Initialize(SMSG_BATTLEGROUND_PLAYER_JOINED, 16 + 2);
                p_Data->appendPackGUID(p_Guid);
            }

            static void List(WorldPacket* p_Data, uint64 p_GUID, Player* p_Player, BattlegroundTypeId p_BGTypeID, uint32 p_BattlemasterListID, bool p_PvpAnywhere, bool p_IsRandomBG, bool p_HasHolidayWinToday, bool p_HasRandomWinToday)
            {
                if (!p_Player)
                    return;

                std::vector<uint32> l_BattleFields;
                Bracket const* l_Bracket = nullptr;
                if (p_BGTypeID != BATTLEGROUND_AA)
                {
                    if (Battleground* l_BGTemplate = sBattlegroundMgr->GetBattlegroundTemplate(GetTypeFromId(p_BGTypeID, 0, false))) ///< l_BGTemplate is unused
                    {
                        /// Expected bracket entry
                        if ((l_Bracket = Brackets::FindForLevel(p_Player->getLevel())))
                        {
                            auto l_ClientBattlegroundIds = sBattlegroundMgr->GetClientBattlegroundIds(GetSchedulerType(p_BGTypeID), l_Bracket->m_Id);
                            for (std::set<uint32>::iterator l_It = l_ClientBattlegroundIds.begin(); l_It != l_ClientBattlegroundIds.end(); ++l_It)
                                l_BattleFields.push_back(*l_It);
                        }
                    }
                }
                
                p_Data->Initialize(SMSG_BATTLEFIELD_LIST);
                p_Data->appendPackGUID(p_GUID);
                *p_Data << uint32(p_BattlemasterListID);
                *p_Data << uint8(l_Bracket ? l_Bracket->m_MinLevel : 90); ///< MinLevel
                *p_Data << uint8(l_Bracket ? l_Bracket->m_MaxLevel : 90); ///< MaxLevel
                *p_Data << uint32(l_BattleFields.size());

                for (uint32 l_BattleField : l_BattleFields)
                    *p_Data << uint32(l_BattleField);

                p_Data->WriteBit(p_PvpAnywhere);
                p_Data->WriteBit(p_HasHolidayWinToday);
                p_Data->WriteBit(p_HasRandomWinToday);
                p_Data->WriteBit(p_IsRandomBG);
                p_Data->FlushBits();
            }

            static void AreaSpiritHealerQuery(Player * p_Player, Battleground * p_Battleground, uint64 p_Guid)
            {
                uint32 l_Time = 30000 - p_Battleground->GetLastResurrectTime();      ///< resurrect every 30 seconds
                if (l_Time == uint32(-1))
                    l_Time = 0;

                WorldPacket l_Data(SMSG_AREA_SPIRIT_HEALER_TIME, 16 + 2 + 4);
                l_Data.appendPackGUID(p_Guid);
                l_Data << uint32(l_Time);

                p_Player->GetSession()->SendPacket(&l_Data);
            }

            static void CheckWargameEntry(Player* p_Requester, Player* p_Target, uint64 p_QueueID, bool p_TournamentRules)
            {
                uint32 l_Timeout         = 60;

                WorldPacket l_Data(SMSG_CHECK_WARGAME_ENTRY);
                l_Data.appendPackGUID(p_Requester->GetGUID());
                l_Data << uint32(g_RealmID);
                l_Data << uint16(0);                ///< unk
                l_Data << uint8(0);                 ///< ServerSpec
                l_Data.appendPackGUID(0);           ///< BnetGuid, bypass bnet friend list check
                l_Data << uint64(p_QueueID);
                l_Data << uint32(l_Timeout);
                l_Data.WriteBit(p_TournamentRules);

                p_Target->GetSession()->SendPacket(&l_Data);
            }
        };
    }
}

#endif /// !BATTLEGROUND_PACKET_FACTORY_HPP
