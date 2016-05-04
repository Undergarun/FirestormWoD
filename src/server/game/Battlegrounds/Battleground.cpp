////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Chat.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "World.h"
#include "WorldPacket.h"
#include "Battleground.h"
#include "BattlegroundMgr.hpp"
#include "Creature.h"
#include "Formulas.h"
#include "GridNotifiersImpl.h"
#include "Group.h"
#include "Language.h"
#include "MapManager.h"
#include "Object.h"
#include "SpellAuras.h"
#include "SpellAuraEffects.h"
#include "Util.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "GroupMgr.h"

#include "BattlegroundPacketFactory.hpp"

namespace JadeCore
{
    class BattlegroundChatBuilder
    {
        public:
            BattlegroundChatBuilder(ChatMsg msgtype, int32 textId, Player const* source, va_list* args = NULL)
                : _msgtype(msgtype), _textId(textId), _source(source), _args(args) { }

            void operator()(WorldPacket& data, LocaleConstant loc_idx)
            {
                char const* text = sObjectMgr->GetTrinityString(_textId, loc_idx);
                if (_args)
                {
                    // we need copy va_list before use or original va_list will corrupted
                    va_list ap;
                    va_copy(ap, *_args);

                    char str[2048];
                    vsnprintf(str, 2048, text, ap);
                    va_end(ap);

                    do_helper(data, &str[0]);
                }
                else
                    do_helper(data, text);
            }

        private:
            void do_helper(WorldPacket& data, char const* text)
            {
                ChatHandler::FillMessageData(&data, _source ? _source->GetSession() : NULL, _msgtype, LANG_UNIVERSAL, NULL, _source ? _source->GetGUID() : 0, text, NULL, NULL);
            }

            ChatMsg _msgtype;
            int32 _textId;
            Player const* _source;
            va_list* _args;
    };

    class Battleground2ChatBuilder
    {
        public:
            Battleground2ChatBuilder(ChatMsg msgtype, int32 textId, Player const* source, int32 arg1, int32 arg2)
                : _msgtype(msgtype), _textId(textId), _source(source), _arg1(arg1), _arg2(arg2) {}

            void operator()(WorldPacket& data, LocaleConstant loc_idx)
            {
                char const* text = sObjectMgr->GetTrinityString(_textId, loc_idx);
                char const* arg1str = _arg1 ? sObjectMgr->GetTrinityString(_arg1, loc_idx) : "";
                char const* arg2str = _arg2 ? sObjectMgr->GetTrinityString(_arg2, loc_idx) : "";

                char str[2048];
                snprintf(str, 2048, text, arg1str, arg2str);

                ChatHandler::FillMessageData(&data, _source ? _source->GetSession() : NULL, _msgtype, LANG_UNIVERSAL, NULL, _source ? _source->GetGUID() : 0, str, NULL, NULL);
            }

        private:
            ChatMsg _msgtype;
            int32 _textId;
            Player const* _source;
            int32 _arg1;
            int32 _arg2;
    };
}                                                           // namespace JadeCore

template<class Do>
void Battleground::BroadcastWorker(Do& _do)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = ObjectAccessor::FindPlayer(MAKE_NEW_GUID(itr->first, 0, HIGHGUID_PLAYER)))
            _do(player);
}

Battleground::Battleground()
{
    m_TypeID            = BATTLEGROUND_TYPE_NONE;
    m_RandomTypeID      = BATTLEGROUND_TYPE_NONE;
    m_Guid              = MAKE_NEW_GUID(m_TypeID, 0, HIGHGUID_TYPE_BATTLEGROUND);
    m_InstanceID        = 0;
    m_Status            = STATUS_NONE;
    m_ClientInstanceID  = 0;
    m_EndTime           = 0;
    m_LastResurrectTime = 0;
    m_BracketId         = 0;
    m_InvitedAlliance   = 0;
    m_InvitedHorde      = 0;
    m_ArenaType         = 0;
    m_IsArena           = false;
    m_IsSkirmish        = false;
    m_Winner            = 2;
    m_StartTime         = 0;
    m_CountdownTimer    = 0;
    m_ResetStatTimer    = 0;
    m_ValidStartPositionTimer = 0;
    m_Events            = 0;
    m_IsRatedBg         = false;
    m_BuffChange        = false;
    m_IsRandom          = false;
    m_Name              = "";
    m_LevelMin          = 0;
    m_LevelMax          = 0;
    m_InBGFreeSlotQueue = false;
    m_SetDeleteThis     = false;

    m_PlayersPositionsTimer = 500;

    m_MaxPlayersPerTeam = 0;
    m_MaxPlayers        = 0;
    m_MinPlayersPerTeam = 0;
    m_MinPlayers        = 0;

    m_MapId             = 0;
    m_Map               = NULL;

    m_TeamStartLocX[BG_TEAM_ALLIANCE]   = 0;
    m_TeamStartLocX[BG_TEAM_HORDE]      = 0;

    m_TeamStartLocY[BG_TEAM_ALLIANCE]   = 0;
    m_TeamStartLocY[BG_TEAM_HORDE]      = 0;

    m_TeamStartLocZ[BG_TEAM_ALLIANCE]   = 0;
    m_TeamStartLocZ[BG_TEAM_HORDE]      = 0;

    m_TeamStartLocO[BG_TEAM_ALLIANCE]   = 0;
    m_TeamStartLocO[BG_TEAM_HORDE]      = 0;

    m_StartMaxDist = 0.0f;
    m_holiday = 0;

    m_ArenaTeamRatingChanges[BG_TEAM_ALLIANCE]   = 0;
    m_ArenaTeamRatingChanges[BG_TEAM_HORDE]      = 0;

    m_ArenaTeamMMR[BG_TEAM_ALLIANCE]   = 0;
    m_ArenaTeamMMR[BG_TEAM_HORDE]      = 0;

    m_BgRaids[BG_TEAM_ALLIANCE]         = NULL;
    m_BgRaids[BG_TEAM_HORDE]            = NULL;

    m_PlayersCount[BG_TEAM_ALLIANCE]    = 0;
    m_PlayersCount[BG_TEAM_HORDE]       = 0;

    m_TeamScores[BG_TEAM_ALLIANCE]      = 0;
    m_TeamScores[BG_TEAM_HORDE]         = 0;

    m_PrematureCountDown = false;

    m_HonorMode = BG_NORMAL;

    StartDelayTimes[BG_STARTING_EVENT_FIRST]  = BG_START_DELAY_2M;
    StartDelayTimes[BG_STARTING_EVENT_SECOND] = BG_START_DELAY_1M;
    StartDelayTimes[BG_STARTING_EVENT_THIRD]  = BG_START_DELAY_30S;
    StartDelayTimes[BG_STARTING_EVENT_FOURTH] = BG_START_DELAY_NONE;
    //we must set to some default existing values
    StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_BG_WS_START_TWO_MINUTES;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_WS_START_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_BG_WS_START_HALF_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_WS_HAS_BEGUN;

    m_CrowdChosed = false;

    /// Wargame
    m_IsWargame          = false;
    m_UseTournamentRules = false;
}

Battleground::~Battleground()
{
    // remove objects and creatures
    // (this is done automatically in mapmanager update, when the instance is reset after the reset time)
    uint32 size = uint32(BgCreatures.size());
    for (uint32 i = 0; i < size; ++i)
        DelCreature(i);

    size = uint32(BgObjects.size());
    for (uint32 i = 0; i < size; ++i)
        DelObject(i);

    sBattlegroundMgr->RemoveBattleground(GetInstanceID(), MS::Battlegrounds::GetSchedulerType(GetTypeID()));
    // unload map
    if (m_Map)
    {
        m_Map->SetUnload();
        //unlink to prevent crash, always unlink all pointer reference before destruction
        m_Map->SetBG(NULL);
        m_Map = NULL;
    }

    for (BattlegroundScoreMap::const_iterator itr = PlayerScores.begin(); itr != PlayerScores.end(); ++itr)
        delete itr->second;
}

void Battleground::InitGUID()
{
    BattlegroundQueueType l_QueueType = BattlegroundQueueType::Battleground;

    if (isArena())
    {
        if (IsSkirmish())
            l_QueueType = BattlegroundQueueType::ArenaSkirmish;
        else
            l_QueueType = BattlegroundQueueType::Arena;
    }

    /// - Missing
    //if (isWorldPvP())
    //    l_QueueType = BattlegroundQueueType::WorldPvP;

    if (IsWargame())
        l_QueueType = BattlegroundQueueType::WarGame;

    if ((!isArena() && sBattlegroundMgr->isTesting())
        || (isArena() && sBattlegroundMgr->isArenaTesting()))
        l_QueueType = BattlegroundQueueType::Cheat;

    m_Guid = MAKE_NEW_GUID((m_TypeID & 0xFFFF) | (uint8(l_QueueType) & 0xF) << 16 , 0, 0x01F1);
}

void Battleground::Update(uint32 diff)
{
    if (!PreUpdateImpl(diff))
        return;

    if (!GetPlayersSize() && !m_InvitedHorde && !m_InvitedAlliance)
    {
        //BG is empty
        // if there are no players invited, delete BG
        // this will delete arena or bg object, where any player entered
        // [[   but if you use battleground object again (more battles possible to be played on 1 instance)
        //      then this condition should be removed and code:
        //      if (!GetInvitedCount(HORDE) && !GetInvitedCount(ALLIANCE))
        //          this->AddToFreeBGObjectsQueue(); // not yet implemented
        //      should be used instead of current
        // ]]
        // Battleground Template instance cannot be updated, because it would be deleted
        m_SetDeleteThis = true;
        return;
    }

    switch (GetStatus())
    {
        case STATUS_WAIT_JOIN:
            if (GetPlayersSize())
            {
                _ProcessJoin(diff);
                //_CheckSafePositions(diff);  disable, use check for every bg in PostUpdateImpl
            }
            break;
        case STATUS_IN_PROGRESS:
            _ProcessOfflineQueue();
            // after 20 minutes without one team losing, the arena closes with no winner and no rating change
            if (isArena())
            {
                ApplyDampeningIfNeeded();

                if (GetElapsedTime() >= 20 *  MINUTE * IN_MILLISECONDS)
                {
                    UpdateArenaWorldState();
                    CheckArenaAfterTimerConditions();
                    return;
                }
            }
            else
            {
                // Since 5.0.X, client doesn't send request to know players flags positions
                // We need to handle it server-side and send new position every ~ 500 ms
                if (m_PlayersPositionsTimer <= diff)
                {
                    SendFlagsPositions();
                    m_PlayersPositionsTimer = 500;
                }
                else
                    m_PlayersPositionsTimer -= diff;

                _ProcessRessurect(diff);
                if (sBattlegroundMgr->GetPrematureFinishTime() && (GetPlayersCountByTeam(ALLIANCE) < GetMinPlayersPerTeam() || GetPlayersCountByTeam(HORDE) < GetMinPlayersPerTeam()))
                    _ProcessProgress(diff);
                else if (m_PrematureCountDown)
                    m_PrematureCountDown = false;
            }
            break;
        case STATUS_WAIT_LEAVE:
            _ProcessLeave(diff);
            break;
        default:
            break;
    }

    // Update start time and reset stats timer
    m_StartTime += diff;
    m_ResetStatTimer += diff;

    PostUpdateImpl(diff);
}

inline void Battleground::_CheckSafePositions(uint32 diff)
{
    float maxDist = GetStartMaxDist();
    if (!maxDist)
        return;

    m_ValidStartPositionTimer += diff;
    if (m_ValidStartPositionTimer >= CHECK_PLAYER_POSITION_INVERVAL)
    {
        m_ValidStartPositionTimer = 0;

        Position pos;
        float x, y, z, o;
        for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            {
                player->GetPosition(&pos);
                GetTeamStartLoc(player->GetBGTeam(), x, y, z, o);
                if (pos.GetExactDistSq(x, y, z) > maxDist)
                {
                    sLog->outDebug(LOG_FILTER_BATTLEGROUND, "BATTLEGROUND: Sending %s back to start location (map: %u) (possible exploit)", player->GetName(), GetMapId());
                    player->TeleportTo(GetMapId(), x, y, z, o);
                }
            }
    }
}

inline void Battleground::_ProcessOfflineQueue()
{
    // remove offline players from bg after 5 minutes
    if (!m_OfflineQueue.empty())
    {
        BattlegroundPlayerMap::iterator itr = m_Players.find(*(m_OfflineQueue.begin()));
        if (itr != m_Players.end())
        {
            if (itr->second.OfflineRemoveTime <= sWorld->GetGameTime())
            {
                RemovePlayerAtLeave(itr->first, true, true);// remove player from BG
                m_OfflineQueue.pop_front();                 // remove from offline queue
                //do not use itr for anything, because it is erased in RemovePlayerAtLeave()
            }
        }
    }

}

inline void Battleground::_ProcessRessurect(uint32 diff)
{
    // *********************************************************
    // ***        BATTLEGROUND RESSURECTION SYSTEM           ***
    // *********************************************************
    // this should be handled by spell system
    m_LastResurrectTime += diff;
    if (m_LastResurrectTime >= RESURRECTION_INTERVAL)
    {
        if (GetReviveQueueSize())
        {
            for (std::map<uint64, std::vector<uint64> >::iterator itr = m_ReviveQueue.begin(); itr != m_ReviveQueue.end(); ++itr)
            {
                Creature* sh = NULL;
                for (std::vector<uint64>::const_iterator itr2 = (itr->second).begin(); itr2 != (itr->second).end(); ++itr2)
                {
                    Player* player = ObjectAccessor::FindPlayer(*itr2);
                    if (!player)
                        continue;

                    if (!sh && player->IsInWorld())
                    {
                        sh = player->GetMap()->GetCreature(itr->first);
                        // only for visual effect
                        if (sh)
                            // Spirit Heal, effect 117
                            sh->CastSpell(sh, SPELL_SPIRIT_HEAL, true);
                    }

                    // Resurrection visual
                    player->CastSpell(player, SPELL_RESURRECTION_VISUAL, true);
                    m_ResurrectQueue.push_back(*itr2);
                }
                (itr->second).clear();
            }

            m_ReviveQueue.clear();
            m_LastResurrectTime = 0;
        }
        else
            // queue is clear and time passed, just update last resurrection time
            m_LastResurrectTime = 0;
    }
    else if (m_LastResurrectTime > 500)    // Resurrect players only half a second later, to see spirit heal effect on NPC
    {
        for (std::vector<uint64>::const_iterator itr = m_ResurrectQueue.begin(); itr != m_ResurrectQueue.end(); ++itr)
        {
            Player* player = ObjectAccessor::FindPlayer(*itr);
            if (!player)
                continue;
            player->ResurrectPlayer(1.0f);
            player->CastSpell(player, 6962, true);
            player->CastSpell(player, SPELL_SPIRIT_HEAL_MANA, true);
            sObjectAccessor->ConvertCorpseForPlayer(*itr);
        }
        m_ResurrectQueue.clear();
    }
}

inline void Battleground::_ProcessProgress(uint32 diff)
{
    // *********************************************************
    // ***           BATTLEGROUND BALLANCE SYSTEM            ***
    // *********************************************************
    // if less then minimum players are in on one side, then start premature finish timer
    if (!m_PrematureCountDown)
    {
        m_PrematureCountDown = true;
        m_PrematureCountDownTimer = sBattlegroundMgr->GetPrematureFinishTime();
    }
    else if (m_PrematureCountDownTimer < diff)
    {
        // time's up!
        uint32 winner = 0;

        if (GetMaxScore())
        {
            if (GetTeamScore(ALLIANCE) != GetTeamScore(HORDE))
            {
                winner = GetTeamScore(ALLIANCE) > GetTeamScore(HORDE) ? ALLIANCE : HORDE;
                if (!IsScoreIncremental())
                    winner = winner == ALLIANCE ? HORDE : ALLIANCE;
            }
            else
                winner = 0;
        }
        else if (GetPlayersCountByTeam(ALLIANCE) >= GetMinPlayersPerTeam())
            winner = ALLIANCE;
        else if (GetPlayersCountByTeam(HORDE) >= GetMinPlayersPerTeam())
            winner = HORDE;

        EndBattleground(winner);
        m_PrematureCountDown = false;
    }
    else if (!sBattlegroundMgr->isTesting())
    {
        uint32 newtime = m_PrematureCountDownTimer - diff;
        // announce every minute
        if (newtime > (MINUTE * IN_MILLISECONDS))
        {
            if (newtime / (MINUTE * IN_MILLISECONDS) != m_PrematureCountDownTimer / (MINUTE * IN_MILLISECONDS))
                PSendMessageToAll(LANG_BATTLEGROUND_PREMATURE_FINISH_WARNING, CHAT_MSG_SYSTEM, NULL, (uint32)(m_PrematureCountDownTimer / (MINUTE * IN_MILLISECONDS)));
        }
        else
        {
            //announce every 15 seconds
            if (newtime / (15 * IN_MILLISECONDS) != m_PrematureCountDownTimer / (15 * IN_MILLISECONDS))
                PSendMessageToAll(LANG_BATTLEGROUND_PREMATURE_FINISH_WARNING_SECS, CHAT_MSG_SYSTEM, NULL, (uint32)(m_PrematureCountDownTimer / IN_MILLISECONDS));
        }
        m_PrematureCountDownTimer = newtime;
    }
}

inline void Battleground::_ProcessJoin(uint32 diff)
{
    // *********************************************************
    // ***           BATTLEGROUND STARTING SYSTEM            ***
    // *********************************************************
    ModifyStartDelayTime(diff);
    ModifyCountdownTimer(diff);

    // I know it's a too big but it's the value sent in packet, I get it from retail sniff.
    // I think it's link to the countdown when bgs start
    SetRemainingTime(300000);

    if (m_ResetStatTimer > 5000)
    {
        m_ResetStatTimer = 0;
        for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
            if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                player->ResetAllPowers();
    }

    if (!(m_Events & BG_STARTING_EVENT_1))
    {
        m_Events |= BG_STARTING_EVENT_1;

        if (!FindBgMap())
        {
            sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::_ProcessJoin: map (map id: %u, instance id: %u) is not created!", m_MapId, m_InstanceID);
            EndNow();
            return;
        }

        // Setup here, only when at least one player has ported to the map
        if (!SetupBattleground())
        {
            EndNow();
            return;
        }

        StartingEventCloseDoors();
        SetStartDelayTime(StartDelayTimes[BG_STARTING_EVENT_FIRST]);
        SetCountdownTimer(StartDelayTimes[BG_STARTING_EVENT_FIRST]);
        // First start warning - 2 or 1 minute
        SendMessageToAll(StartMessageIds[BG_STARTING_EVENT_FIRST], CHAT_MSG_BG_SYSTEM_NEUTRAL);
    }
    // After 1 minute or 30 seconds, warning is signaled
    else if (GetStartDelayTime() <= StartDelayTimes[BG_STARTING_EVENT_SECOND] && !(m_Events & BG_STARTING_EVENT_2))
    {
        m_Events |= BG_STARTING_EVENT_2;
        SendMessageToAll(StartMessageIds[BG_STARTING_EVENT_SECOND], CHAT_MSG_BG_SYSTEM_NEUTRAL);
        SendCountdownTimer();
    }
    // After 30 or 15 seconds, warning is signaled
    else if (GetStartDelayTime() <= StartDelayTimes[BG_STARTING_EVENT_THIRD] && !(m_Events & BG_STARTING_EVENT_3))
    {
        m_Events |= BG_STARTING_EVENT_3;
        SendMessageToAll(StartMessageIds[BG_STARTING_EVENT_THIRD], CHAT_MSG_BG_SYSTEM_NEUTRAL);
    }
    // Delay expired (after 2 or 1 minute)
    else if (GetStartDelayTime() <= 0 && !(m_Events & BG_STARTING_EVENT_4))
    {
        m_Events |= BG_STARTING_EVENT_4;

        StartingEventOpenDoors();

        SendWarningToAll(StartMessageIds[BG_STARTING_EVENT_FOURTH]);
        SetStatus(STATUS_IN_PROGRESS);
        SetStartDelayTime(StartDelayTimes[BG_STARTING_EVENT_FOURTH]);

        // Remove preparation
        if (isArena())
        {
            for (BattlegroundPlayerMap::const_iterator l_Iterator = GetPlayers().begin(); l_Iterator != GetPlayers().end(); ++l_Iterator)
            {
                if (Player* l_Player = ObjectAccessor::FindPlayer(l_Iterator->first))
                {
                    for (BattlegroundPlayerMap::const_iterator l_Iterator = GetPlayers().begin(); l_Iterator != GetPlayers().end(); ++l_Iterator)
                    {
                        if (Player* l_Player2 = ObjectAccessor::FindPlayer(l_Iterator->first))
                        {
                            l_Player2->ModifyAuraState(AURA_STATE_PVP_RAID_PREPARE, false);
                            l_Player2->ForceValuesUpdateAtIndex(UNIT_FIELD_AURA_STATE);

                            UpdateData l_UpdateData(GetMapId());
                            WorldPacket l_Packet;

                            l_Player2->BuildValuesUpdateBlockForPlayer(&l_UpdateData, l_Player);
                            if (l_UpdateData.BuildPacket(&l_Packet))
                                l_Player->SendDirectMessage(&l_Packet);
                        }
                    }
                }
            }

            // TODO : add arena sound PlaySoundToAll(SOUND_ARENA_START);
            for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
                if (Player* l_Player = ObjectAccessor::FindPlayer(itr->first))
                {
                    // BG Status packet
                    WorldPacket status;
                    MS::Battlegrounds::BattlegroundType::Type l_BgType = MS::Battlegrounds::GetTypeFromId(m_TypeID, GetArenaType(), IsSkirmish()); ///< l_BgType is never read 01/18/16
                    uint32 queueSlot = l_Player->GetBattlegroundQueueIndex(MS::Battlegrounds::GetSchedulerType(m_TypeID));
                    MS::Battlegrounds::PacketFactory::Status(&status, this, l_Player, queueSlot, STATUS_IN_PROGRESS, GetExpirationDate(), GetElapsedTime(), GetArenaType(), IsSkirmish());
                    l_Player->GetSession()->SendPacket(&status);

                    l_Player->SetByteValue(PLAYER_FIELD_ARENA_FACTION, PLAYER_BYTES_3_OFFSET_ARENA_FACTION, l_Player->GetBGTeam());
                    l_Player->RemoveAurasDueToSpell(SPELL_ARENA_PREPARATION);
                    l_Player->ResetAllPowers();

                    WorldPacket team_packet(SMSG_ARENA_OPPONENT_SPECIALIZATIONS);
                    BuildArenaOpponentSpecializations(&team_packet, GetOtherTeam(l_Player->GetBGTeam()));
                    l_Player->GetSession()->SendPacket(&team_packet);

                    for (BattlegroundPlayerMap::iterator l_Iterator = m_Players.begin(); l_Iterator != m_Players.end(); ++l_Iterator)
                    {
                        if (Player* l_Member = _GetPlayerForTeam(GetOtherTeam(l_Player->GetBGTeam()), l_Iterator, "BuildArenaOpponentSpecializations"))
                        {
                            WorldPacket l_Data;
                            l_Player->GetSession()->BuildPartyMemberStatsChangedPacket(l_Member, &l_Data, GROUP_UPDATE_FULL, true);
                            l_Player->GetSession()->SendPacket(&l_Data);
                        }
                    }

                    // remove auras with duration lower than 30s
                    Unit::AuraApplicationMap & auraMap = l_Player->GetAppliedAuras();
                    for (Unit::AuraApplicationMap::iterator iter = auraMap.begin(); iter != auraMap.end();)
                    {
                        AuraApplication * aurApp = iter->second;
                        Aura* aura = aurApp->GetBase();
                        if (!aura->IsPermanent()
                            && aura->GetDuration() <= 30*IN_MILLISECONDS
                            && aurApp->IsPositive()
                            && (!(aura->GetSpellInfo()->Attributes & SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY))
                            && (!aura->HasEffectType(SPELL_AURA_MOD_INVISIBILITY)))
                            l_Player->RemoveAura(iter);
                        else
                            ++iter;
                    }
                }

            CheckArenaWinConditions();
        }
        else
        {
            PlaySoundToAll(SOUND_BG_START);

            for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
            {
                if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                {
                    player->ModifyAuraState(AURA_STATE_PVP_RAID_PREPARE, false);
                    player->RemoveAurasDueToSpell(SPELL_PREPARATION);
                    player->ResetAllPowers();

                    if (IsRatedBG())
                    {
                        // remove auras with duration lower than 30s
                        Unit::AuraApplicationMap & auraMap = player->GetAppliedAuras();
                        for (Unit::AuraApplicationMap::iterator iter = auraMap.begin(); iter != auraMap.end();)
                        {
                            AuraApplication * aurApp = iter->second;
                            Aura* aura = aurApp->GetBase();
                            if (!aura->IsPermanent()
                                && aura->GetDuration() <= 30 * IN_MILLISECONDS
                                && aurApp->IsPositive()
                                && (!(aura->GetSpellInfo()->Attributes & SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY))
                                && (!aura->HasEffectType(SPELL_AURA_MOD_INVISIBILITY)))
                                player->RemoveAura(iter);
                            else
                                ++iter;
                        }
                    }
                }
            }

            // Announce BG starting
            if (sWorld->getBoolConfig(CONFIG_BATTLEGROUND_QUEUE_ANNOUNCER_ENABLE))
            {
                if (IsRatedBG())
                    sWorld->SendWorldText(LANG_RBG_STARTED_ANNOUNCE_WORLD, GetName(), GetMinLevel(), GetMaxLevel());
                else
                    sWorld->SendWorldText(LANG_BG_STARTED_ANNOUNCE_WORLD, GetName(), GetMinLevel(), GetMaxLevel());
            }
        }
    }

    if (m_EndTime > 0 && (m_EndTime -= diff) > 0)
        m_EndTime -= diff;
}

inline void Battleground::_ProcessLeave(uint32 diff)
{
    // *********************************************************
    // ***           BATTLEGROUND ENDING SYSTEM              ***
    // *********************************************************
    // remove all players from battleground after 2 minutes
    m_EndTime -= diff;
    if (m_EndTime <= 0)
    {
        m_EndTime = 0;
        BattlegroundPlayerMap::iterator itr, next;
        for (itr = m_Players.begin(); itr != m_Players.end(); itr = next)
        {
            next = itr;
            ++next;
            //itr is erased here!
            RemovePlayerAtLeave(itr->first, true, true);// remove player from BG
            // do not change any battleground's private variables
        }
    }
}

inline Player* Battleground::_GetPlayer(uint64 guid, bool offlineRemove, const char* /*context*/) const
{
    Player* player = NULL;
    if (!offlineRemove)
    {
        player = ObjectAccessor::FindPlayer(guid);
//         if (!player)
//             sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::%s: player (GUID: %u) not found for BG (map: %u, instance id: %u)!",
//                 context, GUID_LOPART(guid), m_MapId, m_InstanceID);
    }
    return player;
}

inline Player* Battleground::_GetPlayer(BattlegroundPlayerMap::iterator itr, const char* context)
{
    return _GetPlayer(itr->first, itr->second.OfflineRemoveTime, context);
}

inline Player* Battleground::_GetPlayer(BattlegroundPlayerMap::const_iterator itr, const char* context) const
{
    return _GetPlayer(itr->first, itr->second.OfflineRemoveTime, context);
}

inline Player* Battleground::_GetPlayerForTeam(uint32 teamId, BattlegroundPlayerMap::const_iterator itr, const char* context) const
{
    Player* player = _GetPlayer(itr, context);
    if (player)
    {
        uint32 team = itr->second.Team;
        if (!team)
            team = player->GetTeam();
        if (team != teamId)
            player = NULL;
    }
    return player;
}

void Battleground::SetTeamStartLoc(uint32 TeamID, float X, float Y, float Z, float O)
{
    BattlegroundTeamId idx = GetTeamIndexByTeamId(TeamID);
    m_TeamStartLocX[idx] = X;
    m_TeamStartLocY[idx] = Y;
    m_TeamStartLocZ[idx] = Z;
    m_TeamStartLocO[idx] = O;
}

void Battleground::SendPacketToAll(WorldPacket* packet)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = _GetPlayer(itr, "SendPacketToAll"))
            player->GetSession()->SendPacket(packet);
}

void Battleground::SendPacketToTeam(uint32 TeamID, WorldPacket* packet, Player* sender, bool self)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = _GetPlayerForTeam(TeamID, itr, "SendPacketToTeam"))
            if (self || sender != player)
                player->GetSession()->SendPacket(packet);
}

void Battleground::PlaySoundToAll(uint32 SoundID)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        if (Player* player = _GetPlayer(itr, "SendPacketToAll"))
            player->SendPlaySound(SoundID, true);
    }
}

void Battleground::PlaySoundToTeam(uint32 SoundID, uint32 TeamID)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = _GetPlayerForTeam(TeamID, itr, "PlaySoundToTeam"))
        {
            player->SendPlaySound(SoundID, true);
        }
}

void Battleground::CastSpellOnTeam(uint32 SpellID, uint32 TeamID)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = _GetPlayerForTeam(TeamID, itr, "CastSpellOnTeam"))
            player->CastSpell(player, SpellID, true);
}

void Battleground::RemoveAuraOnTeam(uint32 SpellID, uint32 TeamID)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = _GetPlayerForTeam(TeamID, itr, "RemoveAuraOnTeam"))
            player->RemoveAura(SpellID);
}

void Battleground::YellToAll(Creature* creature, const char* text, uint32 language)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = _GetPlayer(itr, "YellToAll"))
        {
            WorldPacket data;
            creature->BuildMonsterChat(&data, CHAT_MSG_MONSTER_YELL, text, language, creature->GetName(), itr->first);
            player->GetSession()->SendPacket(&data);
        }
}

void Battleground::RewardHonorToTeam(uint32 p_Honor, uint32 TeamID, MS::Battlegrounds::RewardCurrencyType::Type p_RewardCurrencyType /* = None */)
{
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        if (Player* l_Player = _GetPlayerForTeam(TeamID, itr, "RewardHonorToTeam"))
            UpdatePlayerScore(l_Player, NULL, SCORE_BONUS_HONOR, p_Honor, p_RewardCurrencyType);
    }
}

void Battleground::RewardReputationToTeam(uint32 faction_id, uint32 Reputation, uint32 TeamID)
{
    if (IsRatedBG())
        return;

    if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(faction_id))
        for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
            if (Player* player = _GetPlayerForTeam(TeamID, itr, "RewardReputationToTeam"))
                player->GetReputationMgr().ModifyReputation(factionEntry, Reputation);
}

void Battleground::UpdateWorldState(uint32 Field, uint32 Value)
{
    WorldPacket data;
    MS::Battlegrounds::PacketFactory::UpdateWorldState(&data, Field, Value);

    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        if (Player* player = _GetPlayer(itr, "SendPacketToAll"))
        {
            player->SetWorldState(Field, Value);
            player->GetSession()->SendPacket(&data);
        }
    }
}

void Battleground::UpdateWorldStateForPlayer(uint32 Field, uint32 Value, Player* Source)
{
    WorldPacket data;
    MS::Battlegrounds::PacketFactory::UpdateWorldState(&data, Field, Value);
    Source->SetWorldState(Field, Value);
    Source->GetSession()->SendPacket(&data);
}

void Battleground::EndBattleground(uint32 p_Winner)
{
    Group* winner_team = NULL;
    Group* loser_team = NULL;
    uint32 loser_team_rating = 0;
    uint32 loser_matchmaker_rating = 0;
    int32  loser_change = 0;
    int32  loser_matchmaker_change = 0;
    uint32 winner_team_rating = 0;
    uint32 winner_matchmaker_rating = 0;
    int32  winner_change = 0;
    int32  winner_matchmaker_change = 0;
    WorldPacket data;
    int32 winmsg_id = 0;

    AwardTeams(p_Winner);

    if (p_Winner == ALLIANCE)
    {
        winmsg_id = isBattleground() ? LANG_BG_A_WINS : LANG_ARENA_GOLD_WINS;

        PlaySoundToAll(SOUND_ALLIANCE_WINS);                // alliance wins sound

        SetWinner(WINNER_ALLIANCE);
    }
    else if (p_Winner == HORDE)
    {
        winmsg_id = isBattleground() ? LANG_BG_H_WINS : LANG_ARENA_GREEN_WINS;

        PlaySoundToAll(SOUND_HORDE_WINS);                   // horde wins sound

        SetWinner(WINNER_HORDE);
    }
    else
        SetWinner(3);

    SetStatus(STATUS_WAIT_LEAVE);
    //we must set it this way, because end time is sent in packet!
    SetRemainingTime(TIME_AUTOCLOSE_BATTLEGROUND);

    loser_team = GetBgRaid(GetOtherTeam(p_Winner));
    winner_team = GetBgRaid(p_Winner);

    if (IsRatedBG())
    {
        if (winner_team && loser_team && winner_team != loser_team && GetWinner() != 3)
        {
            loser_matchmaker_rating = GetArenaMatchmakerRating(GetOtherTeam(p_Winner), SLOT_RBG);
            winner_matchmaker_rating = GetArenaMatchmakerRating(p_Winner, SLOT_RBG);
        }

        /// ADD GUILD REWARDS

        if (!isArena())
        {
            for (BattlegroundPlayerMap::iterator l_Itr = m_Players.begin(); l_Itr != m_Players.end(); l_Itr++)
            {

                Player* l_Player = _GetPlayer(l_Itr, "EndBattleground");

                if (!l_Player)
                    continue;

                if (l_Player->GetGroup() && l_Player->GetGroup()->IsGuildGroup(0, true, true))
                {
                    if (Guild* l_Guild = l_Player->GetGuild())
                    {
                        l_Guild->CompleteGuildChallenge(ChallengeRatedBG);
                        break;
                    }
                }
            }
        }
    }

    // arena rating calculation
    if (isArena() && !IsSkirmish() && !IsWargame())
    {
        uint8 slot = Arena::GetSlotByType(GetArenaType());

        if (winner_team && loser_team && winner_team != loser_team && GetWinner() != 3)
        {
            loser_matchmaker_rating = GetArenaMatchmakerRating(GetOtherTeam(p_Winner), slot);
            winner_matchmaker_rating = GetArenaMatchmakerRating(p_Winner, slot);

            winner_team->WonAgainst(winner_matchmaker_rating, loser_matchmaker_rating, winner_change, slot);
            loser_team->LostAgainst(loser_matchmaker_rating, winner_matchmaker_rating, loser_change, slot);

            SetArenaMatchmakerRating(p_Winner, winner_matchmaker_rating + winner_matchmaker_change);
            SetArenaMatchmakerRating(GetOtherTeam(p_Winner), loser_matchmaker_rating + loser_matchmaker_change);
            SetArenaTeamRatingChangeForTeam(p_Winner, winner_change);
            SetArenaTeamRatingChangeForTeam(GetOtherTeam(p_Winner), loser_change);

        }
        // Deduct 16 points from each teams arena-rating if there are no winners after 45+2 minutes
        else if (GetWinner() == 3)
        {
            SetArenaTeamRatingChangeForTeam(ALLIANCE, ARENA_TIMELIMIT_POINTS_LOSS);
            SetArenaTeamRatingChangeForTeam(HORDE, ARENA_TIMELIMIT_POINTS_LOSS);
            if (winner_team)
                winner_team->FinishGame(ARENA_TIMELIMIT_POINTS_LOSS, slot);
            if (loser_team)
                loser_team->FinishGame(ARENA_TIMELIMIT_POINTS_LOSS, slot);
        }
    }

    bool guildAwarded = false;
    uint8 aliveWinners = GetAlivePlayersCountByTeam(p_Winner);
    for (BattlegroundPlayerMap::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        uint32 l_Team = itr->second.Team;

        if (itr->second.OfflineRemoveTime)
        {
            //if rated arena match - make member lost!
            if (isArena() && !IsSkirmish() && winner_team && loser_team && winner_team != loser_team && GetWinner() != 3)
            {
                if (l_Team == p_Winner)
                    winner_team->OfflineMemberLost(itr->first, loser_matchmaker_rating, Arena::GetSlotByType(GetArenaType()), winner_matchmaker_change);
                else
                    loser_team->OfflineMemberLost(itr->first, winner_matchmaker_rating, Arena::GetSlotByType(GetArenaType()), loser_matchmaker_change);
            }
            continue;
        }

        Player* l_Player = _GetPlayer(itr, "EndBattleground");
        if (!l_Player)
            continue;

        // should remove spirit of redemption
        if (l_Player->HasAuraType(SPELL_AURA_SPIRIT_OF_REDEMPTION))
            l_Player->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);

        // Last standing - Rated 5v5 arena & be solely alive player
        if (l_Team == p_Winner && isArena() && !IsSkirmish() && !IsWargame() && GetArenaType() == ArenaType::Arena5v5 && aliveWinners == 1 && l_Player->isAlive())
            l_Player->CastSpell(l_Player, SPELL_THE_LAST_STANDING, true);

        if (!l_Player->isAlive())
        {
            l_Player->ResurrectPlayer(1.0f);
            l_Player->SpawnCorpseBones();
        }
        else
        {
            //needed cause else in av some creatures will kill the players at the end
            l_Player->CombatStop();
            l_Player->getHostileRefManager().deleteReferences();
        }

        // per player calculation, achievements
        if (isArena() && !IsSkirmish() && !IsWargame() && winner_team && loser_team && winner_team != loser_team && GetWinner() != 3)
        {
            uint8 slot = Arena::GetSlotByType(GetArenaType());
            if (l_Team == p_Winner)
            {
                // update achievement BEFORE personal rating update
                uint32 rating = l_Player->GetArenaPersonalRating(slot);
                l_Player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA, rating ? rating : 1);
                l_Player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA, GetMapId());
                l_Player->ModifyCurrency(CURRENCY_TYPE_CONQUEST_META_ARENA_BG, sWorld->getIntConfig(CONFIG_CURRENCY_CONQUEST_POINTS_ARENA_REWARD));
            }
            else
            {
                // Member lost

                // Arena lost => reset the win_rated_arena having the "no_lose" condition
                l_Player->GetAchievementMgr().ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA, ACHIEVEMENT_CRITERIA_CONDITION_NO_LOSE);
            }
        }

        if (IsRatedBG() && winner_team && loser_team && winner_team != loser_team && GetWinner() != 3)
        {
            if (l_Player->GetArenaPersonalRating(SLOT_RBG) < l_Player->GetArenaMatchMakerRating(SLOT_RBG))
            {
                int32 l_RatingChange = Arena::GetRatingMod(l_Player->GetArenaPersonalRating(SLOT_RBG), l_Team == p_Winner ? loser_matchmaker_rating : winner_matchmaker_rating, l_Team == p_Winner, true);
                l_Player->SetArenaPersonalRating(SLOT_RBG, std::max(0, (int)l_Player->GetArenaPersonalRating(SLOT_RBG) + l_RatingChange));
            }

            if (l_Team == p_Winner)
            {
                uint32 l_BattleHardened[2] = { 38929, 38927 };

                if (Quest const* l_Quest = sObjectMgr->GetQuestTemplate(l_BattleHardened[l_Player->GetTeamId()]))
                {
                    if (!l_Player->HasQuest(l_Quest->GetQuestId()) && l_Player->CanTakeQuest(l_Quest, false))
                        l_Player->AddQuest(l_Quest, l_Player);

                    l_Player->KilledMonsterCredit(66623);
                }

                l_Player->ModifyCurrency(CURRENCY_TYPE_CONQUEST_META_ARENA_BG, sWorld->getIntConfig(CONFIG_CURRENCY_CONQUEST_POINTS_RATED_BG_REWARD));

                int32 MMRating_mod = Arena::GetMatchmakerRatingMod(winner_matchmaker_rating, loser_matchmaker_rating, true);
                l_Player->SetArenaMatchMakerRating(SLOT_RBG, l_Player->GetArenaMatchMakerRating(SLOT_RBG) + MMRating_mod);

                l_Player->IncrementWeekWins(SLOT_RBG);
                l_Player->IncrementSeasonWins(SLOT_RBG);
                l_Player->IncrementWeekGames(SLOT_RBG);
                l_Player->IncrementSeasonGames(SLOT_RBG);
            }
            else
            {
                int32 MMRating_mod = Arena::GetMatchmakerRatingMod(loser_matchmaker_rating, winner_matchmaker_rating, false);
                l_Player->SetArenaMatchMakerRating(SLOT_RBG, std::max(0, (int)l_Player->GetArenaMatchMakerRating(SLOT_RBG) + MMRating_mod));
                l_Player->IncrementWeekGames(SLOT_RBG);
                l_Player->IncrementSeasonGames(SLOT_RBG);
            }
        }

        uint32 winner_bonus = l_Player->GetRandomWinner() ? BG_REWARD_WINNER_HONOR_LAST : BG_REWARD_WINNER_HONOR_FIRST;
        uint32 loser_bonus = l_Player->GetRandomWinner() ? BG_REWARD_LOSER_HONOR_LAST : BG_REWARD_LOSER_HONOR_FIRST;

        // remove temporary currency bonus auras before rewarding player
        l_Player->RemoveAura(SPELL_HONORABLE_DEFENDER_25Y);
        l_Player->RemoveAura(SPELL_HONORABLE_DEFENDER_60Y);

        // Reward winner team
        if (l_Team == p_Winner)
        {
            if ((IsRandom() || MS::Battlegrounds::BattlegroundMgr::IsBGWeekend(GetTypeID())))
            {
                UpdatePlayerScore(l_Player, NULL, SCORE_BONUS_HONOR, winner_bonus, !IsWargame(), MS::Battlegrounds::RewardCurrencyType::Type::BattlegroundWin);
                if (!l_Player->GetRandomWinner() && !IsWargame())
                {
                    // 100cp awarded for the first rated battleground won each day
                    l_Player->ModifyCurrency(CURRENCY_TYPE_CONQUEST_META_ARENA_BG, BG_REWARD_WINNER_CONQUEST_FIRST, true, false, false, MS::Battlegrounds::RewardCurrencyType::Type::BattlegroundWin);

                    l_Player->SetRandomWinner(true);
                }
            }
            else if (!isArena() && !IsRatedBG()) // 50cp awarded for each non-rated battleground won
                l_Player->ModifyCurrency(CURRENCY_TYPE_CONQUEST_META_ARENA_BG, BG_REWARD_WINNER_CONQUEST_LAST, true, false, false, MS::Battlegrounds::RewardCurrencyType::Type::BattlegroundWin);

            if (IsSkirmish())
            {
                l_Player->ModifyCurrency(CURRENCY_TYPE_CONQUEST_META_ARENA_BG, ArenaSkirmishRewards::ConquestPointsWinner, true, false, false, MS::Battlegrounds::RewardCurrencyType::Type::ArenaSkyrmish);

                uint32 l_HonorReward = ArenaSkirmishRewards::HonorPointsWinnerBase;
                l_HonorReward += ArenaSkirmishRewards::HonorPointsWinnerBonusPerMinute * (GetElapsedTime() / (IN_MILLISECONDS * MINUTE));

                l_Player->ModifyCurrency(CURRENCY_TYPE_HONOR_POINTS, l_HonorReward, true, false, false, MS::Battlegrounds::RewardCurrencyType::Type::ArenaSkyrmish);
            }

            l_Player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_BG, 1);
            if (!guildAwarded)
            {
                guildAwarded = true;
                if (uint32 guildId = GetBgMap()->GetOwnerGuildId(l_Player->GetTeam()))
                    if (Guild* guild = sGuildMgr->GetGuildById(guildId))
                    {
                        guild->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_BG, 1, 0, 0, NULL, l_Player);
                        if (isArena() && !IsSkirmish() && winner_team && loser_team && winner_team != loser_team)
                            guild->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA, std::max<uint32>(winner_team->GetRating(Arena::GetSlotByType(GetArenaType())), 1), 0, 0, NULL, l_Player);
                    }
            }
        }
        else
        {
            if (IsRandom() || MS::Battlegrounds::BattlegroundMgr::IsBGWeekend(GetTypeID()))
            {
                UpdatePlayerScore(l_Player, NULL, SCORE_BONUS_HONOR, loser_bonus, !IsWargame());

                if (!IsWargame())
                    l_Player->ModifyCurrencyAndSendToast(CURRENCY_TYPE_HONOR_POINTS, loser_bonus);
            }

            if (IsSkirmish())
                l_Player->ModifyCurrency(CURRENCY_TYPE_HONOR_POINTS, ArenaSkirmishRewards::HonorPointLoser, true, false, false, MS::Battlegrounds::RewardCurrencyType::Type::ArenaSkyrmish);
        }

        l_Player->ResetAllPowers();
        l_Player->CombatStopWithPets(true);

        BlockMovement(l_Player);
        l_Player->SetBattlegroundQueueTypeId(l_Player->GetBattlegroundQueueIndex(MS::Battlegrounds::GetSchedulerType(m_TypeID)), MS::Battlegrounds::BattlegroundType::None);
        MS::Battlegrounds::PacketFactory::PvpLogData(&data, this);
        l_Player->GetSession()->SendPacket(&data);

        MS::Battlegrounds::BattlegroundType::Type bgQueueTypeId = MS::Battlegrounds::GetTypeFromId(GetTypeID(), GetArenaType(), IsSkirmish()); ///< bgQueueTypeId is never read 01/18/16
        MS::Battlegrounds::PacketFactory::Status(&data, this, l_Player, l_Player->GetBattlegroundQueueIndex(MS::Battlegrounds::GetSchedulerType(GetTypeID())), STATUS_IN_PROGRESS, GetExpirationDate(), GetElapsedTime(), GetArenaType(), IsSkirmish());
        l_Player->GetSession()->SendPacket(&data);

        if (!IsWargame())
            l_Player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND, 1);
    }

    if (winmsg_id)
        SendMessageToAll(winmsg_id, CHAT_MSG_BG_SYSTEM_NEUTRAL);
}

uint32 Battleground::GetBonusHonorFromKill(uint32 kills) const
{
    //variable kills means how many honorable kills you scored (so we need kills * honor_for_one_kill)
    uint32 maxLevel = std::min(GetMaxLevel(), 100U);
    return JadeCore::Honor::hk_honor_at_level(maxLevel, float(kills));
}

void Battleground::BlockMovement(Player* player)
{
    player->SetClientControl(player, 0);                          // movement disabled NOTE: the effect will be automatically removed by client when the player is teleported from the battleground, so no need to send with uint8(1) in RemovePlayerAtLeave()
}

void Battleground::RemovePlayerAtLeave(uint64 guid, bool Transport, bool SendPacket)
{
    uint32 team = GetPlayerTeam(guid);
    bool participant = false;
    // Remove from lists/maps
    BattlegroundPlayerMap::iterator itr = m_Players.find(guid);
    if (itr != m_Players.end())
    {
        UpdatePlayersCountByTeam(team, true);               // -1 player
        m_Players.erase(itr);
        // check if the player was a participant of the match, or only entered through gm command (goname)
        participant = true;
    }

    BattlegroundScoreMap::iterator itr2 = PlayerScores.find(guid);
    if (itr2 != PlayerScores.end())
    {
        delete itr2->second;                                // delete player's score
        PlayerScores.erase(itr2);
    }

    RemovePlayerFromResurrectQueue(guid);

    Player* player = ObjectAccessor::FindPlayer(guid);

    // should remove spirit of redemption
    if (player)
    {
        if (player->HasAuraType(SPELL_AURA_SPIRIT_OF_REDEMPTION))
            player->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);

        if (!player->isAlive())                              // resurrect on exit
        {
            player->ResurrectPlayer(1.0f);
            player->SpawnCorpseBones();
        }

        player->ActivateSpec(player->GetBGLastActiveSpec());
    }

    RemovePlayer(player, guid, team);                           // BG subclass specific code

    BattlegroundTypeId bgTypeId = GetTypeID();
    MS::Battlegrounds::BattlegroundType::Type bgQueueTypeId = MS::Battlegrounds::GetTypeFromId(GetTypeID(), GetArenaType(), IsSkirmish());

    if (participant) // if the player was a match participant, remove auras, calc rating, update queue
    {
        if (player)
        {
            player->ClearAfkReports();

            if (!team)
                team = player->GetTeam();

            // if arena, remove the specific arena auras
            if (isArena())
            {
                bgTypeId = BATTLEGROUND_AA;                   // set the bg type to all arenas (it will be used for queue refreshing)

                // unsummon current and summon old pet if there was one and there isn't a current pet
                if (Pet* pet = player->GetPet())
                    player->RemovePet(pet, PET_SLOT_ACTUAL_PET_SLOT, false, pet->m_Stampeded);
                else
                    player->RemovePet(NULL, PET_SLOT_ACTUAL_PET_SLOT, false, true);

                player->ResummonPetTemporaryUnSummonedIfAny();
                player->SummonLastSummonedBattlePet();

                if (!IsSkirmish() && GetStatus() == STATUS_IN_PROGRESS)
                {
                    //left a rated match while the encounter was in progress, consider as loser
                    Group* winner_group = GetBgRaid(GetOtherTeam(team));
                    Group* loser_group = GetBgRaid(team);

                    if (winner_group && loser_group && winner_group != loser_group)
                    {
                        uint8 slot = Arena::GetSlotByType(GetArenaType());

                        // Update personal rating
                        int32 mod = Arena::GetRatingMod(player->GetArenaPersonalRating(slot), GetArenaMatchmakerRating(GetOtherTeam(team), slot), false);
                        player->SetArenaPersonalRating(slot, std::max(0,  (int)player->GetArenaPersonalRating(slot) + mod));

                        // Update matchmaker rating
                        player->SetArenaMatchMakerRating(slot, std::max(0, (int)player->GetArenaMatchMakerRating(slot) -12));

                        // Update personal played stats
                        player->IncrementWeekGames(slot);
                        player->IncrementSeasonGames(slot);
                    }
                }
            }
            else if (IsRatedBG())
            {
                if (GetStatus() == STATUS_IN_PROGRESS)
                {
                    //left a rated match while the encounter was in progress, consider as loser
                    Group* winner_group = GetBgRaid(GetOtherTeam(team));
                    Group* loser_group = GetBgRaid(team);

                    if (winner_group && loser_group && winner_group != loser_group)
                    {
                        uint8 slot = SLOT_RBG;

                        // Update personal rating
                        int32 mod = Arena::GetRatingMod(player->GetArenaPersonalRating(slot), GetArenaMatchmakerRating(GetOtherTeam(team), slot), false);
                        player->SetArenaPersonalRating(slot, std::max(0, (int)player->GetArenaPersonalRating(slot) + mod));

                        // Update matchmaker rating
                        player->SetArenaMatchMakerRating(slot, std::max(0, (int)player->GetArenaMatchMakerRating(slot) - 12));

                        // Update personal played stats
                        player->IncrementWeekGames(slot);
                        player->IncrementSeasonGames(slot);
                    }
                }
            }

            if (SendPacket)
            {
                WorldPacket data;
                MS::Battlegrounds::PacketFactory::Status(&data, this, player, player->GetBattlegroundQueueIndex(bgQueueTypeId), STATUS_NONE, player->GetBattlegroundQueueJoinTime(bgQueueTypeId), 0, 0, IsSkirmish());
                player->GetSession()->SendPacket(&data);
            }

            // this call is important, because player, when joins to battleground, this method is not called, so it must be called when leaving bg
            player->RemoveBattlegroundQueueId(bgQueueTypeId);
        }
        else
        // removing offline participant
        {
            if (isArena() && !IsSkirmish() && GetStatus() == STATUS_IN_PROGRESS)
            {
                //left a rated match while the encounter was in progress, consider as loser
                Group* others_group = GetBgRaid(GetOtherTeam(team));
                Group* players_group = GetBgRaid(team);
                if (others_group && players_group)
                    players_group->OfflineMemberLost(guid, GetArenaMatchmakerRating(GetOtherTeam(team), Arena::GetSlotByType(GetArenaType())), Arena::GetSlotByType(GetArenaType()));
            }
        }

        // remove from raid group if player is member
        if (Group* group = GetBgRaid(team))
        {
            if (!group->RemoveMember(guid))                // group was disbanded
            {
                SetBgRaid(team, NULL);
            }
        }

        // Let others know
        WorldPacket data;
        MS::Battlegrounds::PacketFactory::PlayerLeftBattleground(&data, guid);
        SendPacketToTeam(team, &data, player, false);
    }

    if (player)
    {
        /// Do next only if found in battleground
        player->SetBattlegroundId(0, BATTLEGROUND_TYPE_NONE);  // We're not in BG.

        /// reset destination bg team
        player->SetBGTeam(0);

        /// Remove faction override aura
        if (IsRatedBG())
        {
            player->RemoveAura(81748);
            player->RemoveAura(81744);
        }

        player->SetByteValue(PLAYER_FIELD_ARENA_FACTION, 3, 0);
        player->RemoveBattlegroundQueueJoinTime(bgQueueTypeId);

        if (Transport)
            player->TeleportToBGEntryPoint();

        sLog->outInfo(LOG_FILTER_BATTLEGROUND, "BATTLEGROUND: Removed player %s from Battleground.", player->GetName());
    }

    if (player && IsRatedBG())
        player->setFactionForRace(player->getRace());

    //battleground object will be deleted next Battleground::Update() call
}

// this method is called when no players remains in battleground
void Battleground::Reset()
{
    SetWinner(WINNER_NONE);
    SetStatus(STATUS_WAIT_QUEUE);
    SetElapsedTime(0);
    SetRemainingTime(0);
    SetLastResurrectTime(0);
    SetArenaType(0);

    m_Events = 0;
    m_CreationTime = time(nullptr);

    if (m_InvitedAlliance > 0 || m_InvitedHorde > 0)
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::Reset: one of the counters is not 0 (alliance: %u, horde: %u) for BG (map: %u, instance id: %u)!",
            m_InvitedAlliance, m_InvitedHorde, m_MapId, m_InstanceID);

    m_InvitedAlliance = 0;
    m_InvitedHorde = 0;
    m_InBGFreeSlotQueue = false;

    m_Players.clear();

    for (BattlegroundScoreMap::const_iterator itr = PlayerScores.begin(); itr != PlayerScores.end(); ++itr)
        delete itr->second;
    PlayerScores.clear();

    ResetBGSubclass();
}

void Battleground::StartBattleground()
{
    SetElapsedTime(0);
    SetLastResurrectTime(0);
}

void Battleground::BuildArenaOpponentSpecializations(WorldPacket* p_Packet, uint32 p_Team)
{
    uint8 l_OpponentCount = 0;

    for (BattlegroundPlayerMap::const_iterator l_It = m_Players.begin(); l_It != m_Players.end(); ++l_It)
        if (_GetPlayerForTeam(p_Team, l_It, "BuildArenaOpponentSpecializations"))
            l_OpponentCount++;

    *p_Packet << uint32(l_OpponentCount);

    for (BattlegroundPlayerMap::iterator l_It = m_Players.begin(); l_It != m_Players.end(); ++l_It)
    {
        if (Player * l_Player = _GetPlayerForTeam(p_Team, l_It, "BuildArenaOpponentSpecializations"))
        {
            *p_Packet << uint32(l_Player->GetSpecializationId());
            *p_Packet << uint32(0);
            p_Packet->appendPackGUID(l_Player->GetGUID());
        }
    }
}

void Battleground::AddPlayer(Player* player)
{
    // remove afk from player
    if (player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_AFK))
        player->ToggleAFK();

    // score struct must be created in inherited class

    uint64 guid = player->GetGUID();
    uint32 team = player->GetBGTeam();

    BattlegroundPlayer bp;
    bp.OfflineRemoveTime = 0;
    bp.Team = team;

    // Add to list/maps
    m_Players[guid] = bp;

    UpdatePlayersCountByTeam(team, false);                  // +1 player

    WorldPacket data;
    MS::Battlegrounds::PacketFactory::PlayerJoinedBattleground(&data, guid);
    SendPacketToTeam(team, &data, player, false);

    // BG Status packet
    MS::Battlegrounds::BattlegroundType::Type bgQueueTypeId = MS::Battlegrounds::GetTypeFromId(m_TypeID, GetArenaType(), IsSkirmish());
    uint32 queueSlot = player->GetBattlegroundQueueIndex(MS::Battlegrounds::GetSchedulerType(m_TypeID));

    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        MS::Battlegrounds::PacketFactory::Status(&data, this, player, queueSlot, STATUS_IN_PROGRESS, GetExpirationDate(), GetElapsedTime(), GetArenaType(), IsSkirmish());
        player->GetSession()->SendPacket(&data);
    }

    player->Dismount();
    player->RemoveAurasByType(SPELL_AURA_MOUNTED);
    player->RemoveAurasByType(SPELL_AURA_FLY);
    player->ResetAllPowers();

    sScriptMgr->OnEnterBG(player, GetMapId());

    if (IsRatedBG())
        player->RemoveArenaSpellCooldowns(true);

    // add arena specific auras
    if (isArena())
    {
        player->ResummonPetTemporaryUnSummonedIfAny();

        // Removing pet's buffs and debuffs which are not permanent on Arena enter
        if (Pet* pet = player->GetPet())
        {
            if (!pet->isAlive())
                pet->setDeathState(ALIVE);

            // Set pet at full health
            pet->SetHealth(pet->GetMaxHealth());

            pet->RemoveAllAuras();
            pet->CastPetAuras(true);

            player->RemoveArenaSpellCooldowns(true);
        }

        player->RemoveArenaEnchantments(TEMP_ENCHANTMENT_SLOT);
        if (team == ALLIANCE)                                // gold
        {
            if (player->GetTeam() == HORDE)
                player->CastSpell(player, SPELL_HORDE_GOLD_FLAG, true);
            else
                player->CastSpell(player, SPELL_ALLIANCE_GOLD_FLAG, true);
        }
        else                                                // green
        {
            if (player->GetTeam() == HORDE)
                player->CastSpell(player, SPELL_HORDE_GREEN_FLAG, true);
            else
                player->CastSpell(player, SPELL_ALLIANCE_GREEN_FLAG, true);
        }

        player->DestroyConjuredItems(true);
        player->UnsummonPetTemporaryIfAny();
        player->UnsummonCurrentBattlePetIfAny(true);

        if (GetStatus() == STATUS_WAIT_JOIN)                 // not started yet
        {
            player->SaveBGLastSpecialization();
            player->CastSpell(player, SPELL_ARENA_PREPARATION, true);
            SendCountdownTimer();
        }

        // Set arena faction client-side to display arena unit frame
        player->SetByteValue(PLAYER_FIELD_ARENA_FACTION, 3, player->GetBGTeam() == HORDE ? 0 : 1);

        WorldPacket team_packet(SMSG_ARENA_OPPONENT_SPECIALIZATIONS);
        BuildArenaOpponentSpecializations(&team_packet, player->GetBGTeam());
        SendPacketToTeam(GetOtherTeam(player->GetBGTeam()), &team_packet, player, false);

        team_packet.Initialize(SMSG_ARENA_OPPONENT_SPECIALIZATIONS);
        BuildArenaOpponentSpecializations(&team_packet, GetOtherTeam(player->GetBGTeam()));
        player->GetSession()->SendPacket(&team_packet);

        ApplyDampeningIfNeeded();
    }
    else
    {
        if (GetStatus() == STATUS_WAIT_JOIN)                 // not started yet
        {
            player->SaveBGLastSpecialization();
            player->ModifyAuraState(AURA_STATE_PVP_RAID_PREPARE, true);
            player->CastSpell(player, SPELL_PREPARATION, true);   // reduces all mana cost of spells.
            SendCountdownTimer();
        }
    }

    player->GetAchievementMgr().ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP, GetMapId(), true);
    player->GetAchievementMgr().ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_BG, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP, GetMapId(), true);
    player->GetAchievementMgr().ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_DAMAGE_DONE, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP, GetMapId(), true);
    player->GetAchievementMgr().ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP, GetMapId(), true);
    player->GetAchievementMgr().ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP, GetMapId(), true);
    player->GetAchievementMgr().ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP, GetMapId(), true);
    player->GetAchievementMgr().ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP, GetMapId(), true);
    player->GetAchievementMgr().ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP, GetMapId(), true);
    player->GetAchievementMgr().ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP, GetMapId(), true);
    player->GetAchievementMgr().ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP, GetMapId(), true);
    player->GetAchievementMgr().ResetAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL, ACHIEVEMENT_CRITERIA_CONDITION_BG_MAP, GetMapId(), true);

    // setup BG group membership
    PlayerAddedToBGCheckIfBGIsRunning(player);
    AddOrSetPlayerToCorrectBgGroup(player, team);

    if (IsRatedBG())
    {
        if (player->GetTeam() != player->GetBGTeam())
            player->AddAura(player->GetBGTeam() == ALLIANCE ? 81748 : 81744, player);
    }

    // Log
    sLog->outInfo(LOG_FILTER_BATTLEGROUND, "BATTLEGROUND: Player %s joined the battle.", player->GetName());
}

// this method adds player to his team's bg group, or sets his correct group if player is already in bg group
void Battleground::AddOrSetPlayerToCorrectBgGroup(Player* player, uint32 team)
{
    uint64 playerGuid = player->GetGUID();
    Group* group = GetBgRaid(team);
    if (!group)                                      // first player joined
    {
        group = new Group;
        SetBgRaid(team, group);
        group->Create(player);
        group->SetLootMethod(LootMethod::FREE_FOR_ALL);

        sGroupMgr->AddGroup(group);
    }
    else                                            // raid already exist
    {
        if (group->IsMember(playerGuid))
        {
            uint8 subgroup = group->GetMemberGroup(playerGuid);
            player->SetBattlegroundOrBattlefieldRaid(group, subgroup);
        }
        else
        {
            group->AddMember(player);
            if (Group* originalGroup = player->GetOriginalGroup())
                if (originalGroup->IsLeader(playerGuid))
                {
                    group->ChangeLeader(playerGuid);
                    group->SendUpdate();
                }
        }
    }
}

// This method should be called when player logs into running battleground
void Battleground::EventPlayerLoggedIn(Player* player)
{
    if (IsRatedBG())
    {
        if (player->GetTeam() != player->GetBGTeam())
            player->AddAura(player->GetBGTeam() == ALLIANCE ? 81748 : 81744, player);
    }

    ApplyDampeningIfNeeded();

    uint64 guid = player->GetGUID();
    // player is correct pointer
    for (std::deque<uint64>::iterator itr = m_OfflineQueue.begin(); itr != m_OfflineQueue.end(); ++itr)
    {
        if (*itr == guid)
        {
            m_OfflineQueue.erase(itr);
            break;
        }
    }
    m_Players[guid].OfflineRemoveTime = 0;
    PlayerAddedToBGCheckIfBGIsRunning(player);
    // if battleground is starting, then add preparation aura
    // we don't have to do that, because preparation aura isn't removed when player logs out
}

// This method should be called when player logs out from running battleground
void Battleground::EventPlayerLoggedOut(Player* player)
{
    if (player == nullptr)
        return;

    uint64 guid = player->GetGUID();
    if (!IsPlayerInBattleground(guid))  // Check if this player really is in battleground (might be a GM who teleported inside)
        return;

    // player is correct pointer, it is checked in WorldSession::LogoutPlayer()
    m_OfflineQueue.push_back(player->GetGUID());
    m_Players[guid].OfflineRemoveTime = sWorld->GetGameTime() + MAX_OFFLINE_TIME;
    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        uint32 l_PlayerTeam = GetPlayerTeam(guid);
        uint32 l_OtherTeam = GetOtherTeam(l_PlayerTeam);

        // drop flag and handle other cleanups
        RemovePlayer(player, guid, l_PlayerTeam);

        if (player && IsRatedBG())
            player->setFactionForRace(player->getRace());

        // 1 player is logging out, if it is the last, then end arena!
        if (isArena())
            if (GetAlivePlayersCountByTeam(l_PlayerTeam) <= 1 && GetPlayersCountByTeam(l_OtherTeam))
                EndBattleground(l_OtherTeam);
    }
}

bool Battleground::HasFreeSlots() const
{
    return GetPlayersSize() < GetMaxPlayers();
}

void Battleground::UpdatePlayerScore(Player* Source, Player* victim, uint32 type, uint32 value, bool doAddHonor, MS::Battlegrounds::RewardCurrencyType::Type p_RewardCurrencyType)
{
    //this procedure is called from virtual function implemented in bg subclass
    BattlegroundScoreMap::const_iterator itr = PlayerScores.find(Source->GetGUID());
    if (itr == PlayerScores.end())                         // player not found...
        return;

    switch (type)
    {
        case SCORE_KILLING_BLOWS:                           // Killing blows
            itr->second->KillingBlows += value;
            break;
        case SCORE_DEATHS:                                  // Deaths
            itr->second->Deaths += value;
            break;
        case SCORE_HONORABLE_KILLS:                         // Honorable kills
            itr->second->HonorableKills += value;
            break;
        case SCORE_BONUS_HONOR:                             // Honor bonus
            // do not add honor in arenas (only skirmish)
            if (isBattleground() || IsSkirmish())
            {
                // reward honor instantly
                if (doAddHonor)
                {
                    /// RewardHonor calls UpdatePlayerScore with doAddHonor = false
                    if (isBattleground())
                        Source->RewardHonor(NULL, 1, value, false, p_RewardCurrencyType);
                    else
                        Source->RewardHonor(NULL, 1, value, false, MS::Battlegrounds::RewardCurrencyType::Type::ArenaSkyrmish);
                }
                else
                    itr->second->BonusHonor += value;
            }
            break;
            // used only in EY, but in MSG_PVP_LOG_DATA opcode
        case SCORE_DAMAGE_DONE:                             // Damage Done
            itr->second->DamageDone += value;
            if (victim)
            {
                if (victim->GetHealth() < m_minHealth)
                {
                    m_minHealth = victim->GetHealth();
                    m_teamDealMaxDamage = Source->GetBGTeam();
                }
            }
            break;
        case SCORE_HEALING_DONE:                            // Healing Done
            itr->second->HealingDone += value;
            break;
        default:
            sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::UpdatePlayerScore: unknown score type (%u) for BG (map: %u, instance id: %u)!",
                type, m_MapId, m_InstanceID);
            break;
    }
}

void Battleground::AddPlayerToResurrectQueue(uint64 npc_guid, uint64 player_guid)
{
    m_ReviveQueue[npc_guid].push_back(player_guid);

    Player* player = ObjectAccessor::FindPlayer(player_guid);
    if (!player)
        return;

    player->CastSpell(player, SPELL_WAITING_FOR_RESURRECT, true);
}

void Battleground::RemovePlayerFromResurrectQueue(uint64 player_guid)
{
    for (std::map<uint64, std::vector<uint64> >::iterator itr = m_ReviveQueue.begin(); itr != m_ReviveQueue.end(); ++itr)
    {
        for (std::vector<uint64>::iterator itr2 = (itr->second).begin(); itr2 != (itr->second).end(); ++itr2)
        {
            if (*itr2 == player_guid)
            {
                (itr->second).erase(itr2);
                if (Player* player = ObjectAccessor::FindPlayer(player_guid))
                    player->RemoveAurasDueToSpell(SPELL_WAITING_FOR_RESURRECT);
                return;
            }
        }
    }
}

bool Battleground::AddObject(uint32 type, uint32 entry, float x, float y, float z, float o, float rotation0, float rotation1, float rotation2, float rotation3, uint32 /*respawnTime*/)
{
    // If the assert is called, means that BgObjects must be resized!
    ASSERT(type < BgObjects.size());

    Map* map = FindBgMap();
    if (!map)
        return false;
    // Must be created this way, adding to godatamap would add it to the base map of the instance
    // and when loading it (in go::LoadFromDB()), a new guid would be assigned to the object, and a new object would be created
    // So we must create it specific for this instance
    GameObject* go = new GameObject;
    if (!go->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT), entry, GetBgMap(),
        PHASEMASK_NORMAL, x, y, z, o, rotation0, rotation1, rotation2, rotation3, 100, GO_STATE_READY))
    {
        sLog->outError(LOG_FILTER_SQL, "Battleground::AddObject: cannot create gameobject (entry: %u) for BG (map: %u, instance id: %u)!",
                entry, m_MapId, m_InstanceID);
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::AddObject: cannot create gameobject (entry: %u) for BG (map: %u, instance id: %u)!",
                entry, m_MapId, m_InstanceID);
        delete go;
        return false;
    }

    // Add to world, so it can be later looked up from HashMapHolder
    if (!map->AddToMap(go))
    {
        delete go;
        return false;
    }
    BgObjects[type] = go->GetGUID();
    return true;
}

// Some doors aren't despawned so we cannot handle their closing in gameobject::update()
// It would be nice to correctly implement GO_ACTIVATED state and open/close doors in gameobject code
void Battleground::DoorClose(uint32 type)
{
    if (GameObject* obj = GetBgMap()->GetGameObject(BgObjects[type]))
    {
        // If doors are open, close it
        if (obj->getLootState() == GO_ACTIVATED && obj->GetGoState() != GO_STATE_READY)
        {
            obj->SetLootState(GO_READY);
            obj->SetGoState(GO_STATE_READY);
        }
    }
    else
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::DoorClose: door gameobject (type: %u, GUID: %u) not found for BG (map: %u, instance id: %u)!",
            type, GUID_LOPART(BgObjects[type]), m_MapId, m_InstanceID);
}

void Battleground::DoorOpen(uint32 type)
{
    if (GameObject* obj = GetBgMap()->GetGameObject(BgObjects[type]))
    {
        obj->SetLootState(GO_ACTIVATED);
        obj->SetGoState(GO_STATE_ACTIVE);
    }
    else
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::DoorOpen: door gameobject (type: %u, GUID: %u) not found for BG (map: %u, instance id: %u)!",
            type, GUID_LOPART(BgObjects[type]), m_MapId, m_InstanceID);
}

GameObject* Battleground::GetBGObject(uint32 type)
{
    GameObject* obj = GetBgMap()->GetGameObject(BgObjects[type]);
//     if (!obj)
//         sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::GetBGObject: gameobject (type: %u, GUID: %u) not found for BG (map: %u, instance id: %u)!",
//             type, GUID_LOPART(BgObjects[type]), m_MapId, m_InstanceID);
    return obj;
}

Creature* Battleground::GetBGCreature(uint32 type)
{
    Creature* creature = GetBgMap()->GetCreature(BgCreatures[type]);
//     if (!creature)
//         sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::GetBGCreature: creature (type: %u, GUID: %u) not found for BG (map: %u, instance id: %u)!",
//             type, GUID_LOPART(BgCreatures[type]), m_MapId, m_InstanceID);
    return creature;
}

void Battleground::SpawnBGObject(uint32 type, uint32 respawntime)
{
    if (Map* map = FindBgMap())
        if (GameObject* obj = map->GetGameObject(BgObjects[type]))
        {
            if (respawntime)
                obj->SetLootState(GO_JUST_DEACTIVATED);
            else
                if (obj->getLootState() == GO_JUST_DEACTIVATED)
                    // Change state from GO_JUST_DEACTIVATED to GO_READY in case battleground is starting again
                    obj->SetLootState(GO_READY);
            obj->SetRespawnTime(respawntime);
            map->AddToMap(obj);
        }
}

Creature* Battleground::AddCreature(uint32 entry, uint32 type, uint32 teamval, float x, float y, float z, float o, uint32 respawntime)
{
    // If the assert is called, means that BgCreatures must be resized!
    ASSERT(type < BgCreatures.size());

    Map* map = FindBgMap();
    if (!map)
        return NULL;

    Creature* creature = new Creature;
    if (!creature->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_UNIT), map, PHASEMASK_NORMAL, entry, 0, teamval, x, y, z, o))
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::AddCreature: cannot create creature (entry: %u) for BG (map: %u, instance id: %u)!",
            entry, m_MapId, m_InstanceID);
        delete creature;
        return NULL;
    }

    creature->SetHomePosition(x, y, z, o);

    CreatureTemplate const* cinfo = sObjectMgr->GetCreatureTemplate(entry);
    if (!cinfo)
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::AddCreature: creature template (entry: %u) does not exist for BG (map: %u, instance id: %u)!",
            entry, m_MapId, m_InstanceID);
        delete creature;
        return NULL;
    }
    // Force using DB speeds
    creature->SetSpeed(MOVE_WALK,   cinfo->speed_walk);
    creature->SetSpeed(MOVE_RUN,    cinfo->speed_run);
    creature->SetSpeed(MOVE_FLIGHT, cinfo->speed_fly);

    if (!map->AddToMap(creature))
    {
        delete creature;
        return NULL;
    }

    BgCreatures[type] = creature->GetGUID();

    if (respawntime)
        creature->SetRespawnDelay(respawntime);

    return  creature;
}

bool Battleground::DelCreature(uint32 type)
{
    if (!BgCreatures[type])
        return true;

    if (Creature* creature = GetBgMap()->GetCreature(BgCreatures[type]))
    {
        creature->AddObjectToRemoveList();
        BgCreatures[type] = 0;
        return true;
    }

    sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::DelCreature: creature (type: %u, GUID: %u) not found for BG (map: %u, instance id: %u)!",
        type, GUID_LOPART(BgCreatures[type]), m_MapId, m_InstanceID);
    BgCreatures[type] = 0;
    return false;
}

bool Battleground::DelObject(uint32 type)
{
    if (!BgObjects[type])
        return true;

    if (GameObject* obj = GetBgMap()->GetGameObject(BgObjects[type]))
    {
        obj->SetRespawnTime(0);                                 // not save respawn time
        obj->Delete();
        BgObjects[type] = 0;
        return true;
    }
    sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::DelObject: gameobject (type: %u, GUID: %u) not found for BG (map: %u, instance id: %u)!",
        type, GUID_LOPART(BgObjects[type]), m_MapId, m_InstanceID);
    BgObjects[type] = 0;
    return false;
}

bool Battleground::AddSpiritGuide(uint32 type, float x, float y, float z, float o, uint32 team)
{
    uint32 entry = (team == ALLIANCE) ?
        BG_CREATURE_ENTRY_A_SPIRITGUIDE :
        BG_CREATURE_ENTRY_H_SPIRITGUIDE;

    if (Creature* creature = AddCreature(entry, type, team, x, y, z, o))
    {
        creature->setDeathState(DEAD);
        creature->SetGuidValue(UNIT_FIELD_CHANNEL_OBJECT, creature->GetGUID());
        // aura
        // casting visual effect
        creature->SetChannelSpellID(SPELL_SPIRIT_HEAL_CHANNEL);
        // correct cast speed
        creature->SetFloatValue(UNIT_FIELD_MOD_CASTING_SPEED, 1.0f);
        creature->SetFloatValue(UNIT_FIELD_MOD_SPELL_HASTE, 1.0f);
        return true;
    }
    sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::AddSpiritGuide: cannot create spirit guide (type: %u, entry: %u) for BG (map: %u, instance id: %u)!",
        type, entry, m_MapId, m_InstanceID);
    EndNow();
    return false;
}

void Battleground::SendMessageToAll(int32 entry, ChatMsg type, Player const* source)
{
    if (!entry)
        return;

    JadeCore::BattlegroundChatBuilder bg_builder(type, entry, source);
    JadeCore::LocalizedPacketDo<JadeCore::BattlegroundChatBuilder> bg_do(bg_builder);
    BroadcastWorker(bg_do);
}

void Battleground::PSendMessageToAll(int32 entry, ChatMsg type, Player const* source, ...)
{
    if (!entry)
        return;

    va_list ap;
    va_start(ap, source);

    JadeCore::BattlegroundChatBuilder bg_builder(type, entry, source, &ap);
    JadeCore::LocalizedPacketDo<JadeCore::BattlegroundChatBuilder> bg_do(bg_builder);
    BroadcastWorker(bg_do);

    va_end(ap);
}

void Battleground::SendWarningToAll(int32 entry, ...)
{
    if (!entry)
        return;

    const char *format = sObjectMgr->GetTrinityStringForDBCLocale(entry);

    char str[1024];
    va_list ap;
    va_start(ap, entry);
    vsnprintf(str, 1024, format, ap);
    va_end(ap);
    std::string msg(str);

    WorldPacket data;
    ChatHandler::FillMessageData(&data, NULL, CHAT_MSG_RAID_BOSS_EMOTE, LANG_UNIVERSAL, NULL, 0, msg.c_str(), NULL);
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        if (Player* player = ObjectAccessor::FindPlayer(MAKE_NEW_GUID(itr->first, 0, HIGHGUID_PLAYER)))
            if (player->GetSession())
                player->GetSession()->SendPacket(&data);
}

void Battleground::SendMessage2ToAll(int32 entry, ChatMsg type, Player const* source, int32 arg1, int32 arg2)
{
    JadeCore::Battleground2ChatBuilder bg_builder(type, entry, source, arg1, arg2);
    JadeCore::LocalizedPacketDo<JadeCore::Battleground2ChatBuilder> bg_do(bg_builder);
    BroadcastWorker(bg_do);
}

void Battleground::SendCountdownTimer()
{
    int countdownSec = (GetMaxCountdownTimer() - ceil(float(GetElapsedTime()) / 1000));

    for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
        if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            player->SendStartTimer(countdownSec, GetMaxCountdownTimer(), PVP_TIMER);
}

void Battleground::EndNow()
{
    SetStatus(STATUS_WAIT_LEAVE);
    SetRemainingTime(0);
}

// To be removed
const char* Battleground::GetTrinityString(int32 entry)
{
    // FIXME: now we have different DBC locales and need localized message for each target client
    return sObjectMgr->GetTrinityStringForDBCLocale(entry);
}

// IMPORTANT NOTICE:
// buffs aren't spawned/despawned when players captures anything
// buffs are in their positions when battleground starts
void Battleground::HandleTriggerBuff(uint64 go_guid)
{
    GameObject* obj = GetBgMap()->GetGameObject(go_guid);
    if (!obj || obj->GetGoType() != GAMEOBJECT_TYPE_TRAP || !obj->isSpawned())
        return;

    // Change buff type, when buff is used:
    int32 index = BgObjects.size() - 1;
    while (index >= 0 && BgObjects[index] != go_guid)
        index--;
    if (index < 0)
    {
        sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::HandleTriggerBuff: cannot find buff gameobject (GUID: %u, entry: %u, type: %u) in internal data for BG (map: %u, instance id: %u)!",
            GUID_LOPART(go_guid), obj->GetEntry(), obj->GetGoType(), m_MapId, m_InstanceID);
        return;
    }

    // Randomly select new buff
    uint8 buff = urand(0, 2);
    uint32 entry = obj->GetEntry();
    if (m_BuffChange && entry != Buff_Entries[buff])
    {
        // Despawn current buff
        SpawnBGObject(index, RESPAWN_ONE_DAY);
        // Set index for new one
        for (uint8 currBuffTypeIndex = 0; currBuffTypeIndex < 3; ++currBuffTypeIndex)
            if (entry == Buff_Entries[currBuffTypeIndex])
            {
                index -= currBuffTypeIndex;
                index += buff;
            }
    }

    SpawnBGObject(index, BUFF_RESPAWN_TIME);
}

void Battleground::HandleKillPlayer(Player* victim, Player* killer)
{
    // Keep in mind that for arena this will have to be changed a bit

    // Add +1 deaths
    UpdatePlayerScore(victim, NULL, SCORE_DEATHS, 1, !IsWargame());
    // Add +1 kills to group and +1 killing_blows to killer
    if (killer)
    {
        // Don't reward credit for killing ourselves, like fall damage of hellfire (warlock)
        if (killer == victim)
            return;

        UpdatePlayerScore(killer, NULL, SCORE_HONORABLE_KILLS, 1, !IsWargame());
        UpdatePlayerScore(killer, NULL, SCORE_KILLING_BLOWS, 1, !IsWargame());

        for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
        {
            Player* creditedPlayer = ObjectAccessor::FindPlayer(itr->first);
            if (!creditedPlayer || creditedPlayer == killer)
                continue;

            if (creditedPlayer->GetTeam() == killer->GetTeam() && creditedPlayer->IsAtGroupRewardDistance(victim))
                UpdatePlayerScore(creditedPlayer, NULL, SCORE_HONORABLE_KILLS, 1, !IsWargame());
        }
    }

    if (!isArena() && !IsWargame())
    {
        // To be able to remove insignia -- ONLY IN Battlegrounds
        victim->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);
        RewardXPAtKill(killer, victim);
    }
}

// Return the player's team based on battlegroundplayer info
// Used in same faction arena matches mainly
uint32 Battleground::GetPlayerTeam(uint64 guid) const
{
    BattlegroundPlayerMap::const_iterator itr = m_Players.find(guid);
    if (itr != m_Players.end())
        return itr->second.Team;
    return 0;
}

uint32 Battleground::GetOtherTeam(uint32 teamId) const
{
    return teamId ? ((teamId == ALLIANCE) ? HORDE : ALLIANCE) : 0;
}

bool Battleground::IsPlayerInBattleground(uint64 guid) const
{
    BattlegroundPlayerMap::const_iterator itr = m_Players.find(guid);
    if (itr != m_Players.end())
        return true;
    return false;
}

void Battleground::PlayerAddedToBGCheckIfBGIsRunning(Player* player)
{
    if (GetStatus() != STATUS_WAIT_LEAVE)
        return;

    WorldPacket data;
    MS::Battlegrounds::BattlegroundType::Type bgQueueTypeId = MS::Battlegrounds::GetTypeFromId(GetTypeID(), GetArenaType(), IsSkirmish());

    BlockMovement(player);

    MS::Battlegrounds::PacketFactory::PvpLogData(&data, this);
    player->GetSession()->SendPacket(&data);

    MS::Battlegrounds::PacketFactory::Status(&data, this, player, player->GetBattlegroundQueueIndex(bgQueueTypeId), STATUS_IN_PROGRESS, GetExpirationDate(), GetElapsedTime(), GetArenaType(), IsSkirmish());
    player->GetSession()->SendPacket(&data);
}

uint32 Battleground::GetAlivePlayersCountByTeam(uint32 Team) const
{
    int count = 0;
    for (BattlegroundPlayerMap::const_iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        if (itr->second.Team == Team)
        {
            Player* player = ObjectAccessor::FindPlayer(itr->first);
            if (player && player->isAlive() && !player->HasByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 3, FORM_SPIRITOFREDEMPTION))
                ++count;
        }
    }
    return count;
}

void Battleground::SetHoliday(bool is_holiday)
{
    m_HonorMode = is_holiday ? BG_HOLIDAY : BG_NORMAL;
}

int32 Battleground::GetObjectType(uint64 guid)
{
    for (uint32 i = 0; i < BgObjects.size(); ++i)
        if (BgObjects[i] == guid)
            return i;
    sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground::GetObjectType: player used gameobject (GUID: %u) which is not in internal data for BG (map: %u, instance id: %u), cheating?",
        GUID_LOPART(guid), m_MapId, m_InstanceID);
    return -1;
}

void Battleground::HandleKillUnit(Creature* /*creature*/, Player* /*killer*/)
{
}

void Battleground::CheckArenaAfterTimerConditions()
{
    if (GetAlivePlayersCountByTeam(ALLIANCE) == GetAlivePlayersCountByTeam(HORDE))
        EndBattleground(WINNER_NONE);
    else
        EndBattleground(GetAlivePlayersCountByTeam(ALLIANCE) > GetAlivePlayersCountByTeam(HORDE) ? ALLIANCE : HORDE);
}

void Battleground::CheckArenaWinConditions()
{
    if (!GetAlivePlayersCountByTeam(ALLIANCE) && GetPlayersCountByTeam(HORDE))
        EndBattleground(HORDE);
    else if (GetPlayersCountByTeam(ALLIANCE) && !GetAlivePlayersCountByTeam(HORDE))
        EndBattleground(ALLIANCE);
}

void Battleground::UpdateArenaWorldState()
{
    UpdateWorldState(0xe10, GetAlivePlayersCountByTeam(HORDE));
    UpdateWorldState(0xe11, GetAlivePlayersCountByTeam(ALLIANCE));
}

void Battleground::SetBgRaid(uint32 TeamID, Group* bg_raid)
{
    Group*& old_raid = TeamID == ALLIANCE ? m_BgRaids[BG_TEAM_ALLIANCE] : m_BgRaids[BG_TEAM_HORDE];
    if (old_raid)
        old_raid->SetBattlegroundGroup(NULL);
    if (bg_raid)
        bg_raid->SetBattlegroundGroup(this);
    old_raid = bg_raid;
}

WorldSafeLocsEntry const* Battleground::GetClosestGraveYard(Player* player)
{
    return sObjectMgr->GetClosestGraveYard(player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetMapId(), player->GetTeam());
}

bool Battleground::IsTeamScoreInRange(uint32 team, uint32 minScore, uint32 maxScore) const
{
    BattlegroundTeamId teamIndex = GetTeamIndexByTeamId(team);
    uint32 score = std::max(m_TeamScores[teamIndex], 0);
    return score >= minScore && score <= maxScore;
}

void Battleground::StartTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry)
{
    for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
        if (Player* player = ObjectAccessor::FindPlayer(itr->first))
            player->GetAchievementMgr().StartTimedAchievement(type, entry);
}

void Battleground::SetBracket(MS::Battlegrounds::Bracket const* bracketEntry)
{
    m_BracketId = bracketEntry->m_Id;
    SetLevelRange(bracketEntry->m_MinLevel, bracketEntry->m_MaxLevel);
}

void Battleground::RewardXPAtKill(Player* killer, Player* victim)
{
    if (IsWargame())
        return;

    if (sWorld->getBoolConfig(CONFIG_BG_XP_FOR_KILL) && killer && victim)
        killer->RewardPlayerAndGroupAtKill(victim, true);
}

void Battleground::SendFlagsPositions()
{
    std::list<Player*> l_Players;

    // Alliance
    std::set<uint64> l_AllianceFlagPickers = GetFlagPickersGUID(TEAM_ALLIANCE);
    std::for_each(l_AllianceFlagPickers.begin(), l_AllianceFlagPickers.end(), [&l_Players](uint64 p_PickerGuid)
    {
        Player* l_Player = ObjectAccessor::FindPlayer(p_PickerGuid);
        if (l_Player != nullptr)
            l_Players.push_back(l_Player);
    });

    std::set<uint64> l_HordeFlagPickers = GetFlagPickersGUID(TEAM_HORDE);
    std::for_each(l_HordeFlagPickers.begin(), l_HordeFlagPickers.end(), [&l_Players](uint64 p_PickerGuid)
    {
        Player* l_Player = ObjectAccessor::FindPlayer(p_PickerGuid);
        if (l_Player != nullptr)
            l_Players.push_back(l_Player);
    });

    WorldPacket l_Data(SMSG_BATTLEGROUND_PLAYER_POSITIONS);

    l_Data << uint32(l_Players.size());

    uint8 l_ArenaSlot = 1;  ///< Used to show UI frame of who is flag picker (1 - 5)

    for (auto l_Itr : l_Players)
    {
        l_Data.appendPackGUID(l_Itr->GetGUID());
        l_Data << float(l_Itr->GetPositionX());
        l_Data << float(l_Itr->GetPositionY());

        FlagIcon l_FlagIcon = FlagIcon::None;
        switch (GetTypeID())
        {
            /// - In Kotmogu Temple, each faction have theirs own flags, they didn't steal the other faction flag
            case BattlegroundTypeId::BATTLEGROUND_KT:
                l_FlagIcon = l_Itr->GetTeamId() == TeamId::TEAM_HORDE ? FlagIcon::Horde : FlagIcon::Alliance;
                break;
            /// - In all others bg with flags, each faction must steal the flag
            default:
                l_FlagIcon = l_Itr->GetTeamId() == TeamId::TEAM_HORDE ? FlagIcon::Alliance : FlagIcon::Horde;
                break;
        }

        l_Data << uint8(l_FlagIcon);
        l_Data << uint8(l_ArenaSlot++);
    }

    SendPacketToAll(&l_Data);
}

void Battleground::RelocateDeadPlayers(uint64 queueIndex)
{
    // Those who are waiting to resurrect at this node are taken to the closest own node's graveyard
    std::vector<uint64>& ghostList = m_ReviveQueue[queueIndex];
    if (!ghostList.empty())
    {
        WorldSafeLocsEntry const* closestGrave = NULL;
        for (std::vector<uint64>::const_iterator itr = ghostList.begin(); itr != ghostList.end(); ++itr)
        {
            Player* player = ObjectAccessor::FindPlayer(*itr);
            if (!player)
                continue;

            if (!closestGrave)
                closestGrave = GetClosestGraveYard(player);

            if (closestGrave)
                player->TeleportTo(GetMapId(), closestGrave->x, closestGrave->y, closestGrave->z, player->GetOrientation());
        }
        ghostList.clear();
    }
}

uint32 Battleground::GetArenaMatchmakerRating(uint32 Team, uint8 slot)
{
    uint32 MMR = 0;
    uint32 count = 0;

    if (Group* group = GetBgRaid(Team))
    {
        for (GroupReference* ref = group->GetFirstMember(); ref != NULL; ref = ref->next())
        {
            if (Player* groupMember = ref->getSource())
            {
                MMR += groupMember->GetArenaMatchMakerRating(slot);
                ++count;
            }
        }
    }

    if (!count)
        count = 1;

    MMR /= count;

    return MMR;
}

void Battleground::AwardTeams(uint32 p_Winner)
{
    if (IsSkirmish() && p_Winner)
    {
        AwardTeamsWithRewards(AWARD_NONE, AWARD_NONE, p_Winner);
        RewardHonorToTeam(40, p_Winner);
        return;
    }

    /// @todo: Implement rated bg rewards - random piece of CP gear 0-3 pieces per team for a win
    if (IsWargame() || IsRatedBG() || isArena())
        return;

    if (!GetMaxScore())
        return;

    BattlegroundAward l_LooserAward = AWARD_NONE;
    BattlegroundAward l_WinnerAward = p_Winner ? AWARD_GOLD : AWARD_NONE;

    if (!p_Winner)
        p_Winner = ALLIANCE; /// Doesnt matter anymore

    uint32 p_Looser = p_Winner ? GetOtherTeam(p_Winner) : 0;

    float l_LooserFactor = (IsScoreIncremental() ? (float)GetTeamScore(p_Looser) : (float)GetMaxScore() - (float)GetTeamScore(p_Looser)) / (float)GetMaxScore();

    if (l_LooserFactor >= 0.66f)
        l_LooserAward = AWARD_SILVER;
    else if (l_LooserFactor >= 0.33f)
        l_LooserAward = AWARD_BRONZE;

    if (l_WinnerAward == AWARD_NONE)
    {
        float l_WinnerFactor = (IsScoreIncremental() ? (float)GetTeamScore(p_Winner) : (float)GetMaxScore() - (float)GetTeamScore(p_Winner)) / (float)GetMaxScore();

        if (l_WinnerFactor >= 0.66f)
            l_WinnerAward = AWARD_SILVER;
        else if (l_WinnerFactor >= 0.33f)
            l_WinnerAward = AWARD_BRONZE;
    }

    AwardTeamsWithRewards(l_LooserAward, l_WinnerAward, p_Winner);
}

void Battleground::AwardTeamsWithRewards(BattlegroundAward p_LooserAward, BattlegroundAward p_WinnerAward, uint32 p_Winner)
{
    if (isBattleground())
    {
        CastSpellOnTeam(GetSpellIdForAward(p_WinnerAward), p_Winner);
        CastSpellOnTeam(GetSpellIdForAward(p_LooserAward), GetOtherTeam(p_Winner));
    }
    else if (IsSkirmish())
    {
        CastSpellOnTeam(PVP_AWARD_SPELL_SKIRMISH_WIN, p_Winner);
    }
}

uint32 Battleground::GetSpellIdForAward(BattlegroundAward p_Award)
{
    switch (p_Award)
    {
        case AWARD_GOLD:
            return PVP_AWARD_SPELL_GOLDEN_STRONBOX;
        case AWARD_SILVER:
            return PVP_AWARD_SPELL_SILVER_STRONGBOX;
        case AWARD_BRONZE:
            return PVP_AWARD_SPELL_BRONZE_STRONGBOX;
        default:
            return 0;
    }
}

void Battleground::ApplyDampeningIfNeeded()
{
    if (!isArena())
        return;

    if (GetStatus() != STATUS_IN_PROGRESS)
        return;

    uint32 l_Timer = 5;

    if (GetArenaType() == ArenaType::Arena2v2)
    {
        bool l_TeamHasHealer[TEAM_NEUTRAL] = { false, false };

        for (int l_Team = 0; l_Team < TEAM_NEUTRAL; l_Team++)
        {
            for (BattlegroundPlayerMap::iterator l_Iter = m_Players.begin(); l_Iter != m_Players.end(); ++l_Iter)
            {
                if (Player* l_Player = _GetPlayerForTeam(l_Team == TEAM_ALLIANCE ? ALLIANCE : HORDE, l_Iter, "ApplyDampeningIfNeeded"))
                {
                    if (l_Player->GetRoleForGroup() == ROLE_HEALER)
                    {
                        l_TeamHasHealer[l_Team] = true;
                        break;
                    }
                }
            }

            if (l_TeamHasHealer[TEAM_ALLIANCE] && l_TeamHasHealer[TEAM_HORDE])
                l_Timer = 0;
        }
    }

    if (GetElapsedTime() >= (l_Timer * MINUTE * IN_MILLISECONDS))
        for (BattlegroundPlayerMap::iterator l_Iter = m_Players.begin(); l_Iter != m_Players.end(); ++l_Iter)
            if (Player* l_Player = _GetPlayer(l_Iter, "ApplyDampeningIfNeeded"))
                if (!l_Player->HasAura(110310))
                    l_Player->AddAura(110310, l_Player);
}
