////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014 Millenium-studio SARL
//  All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////// 

#include "Common.h"
#include "ObjectMgr.h"
#include "World.h"
#include "WorldPacket.h"

#include "Arena.h"
#include "BattlegroundMgr.hpp"
#include "BattlegroundAV.h"
#include "BattlegroundAB.h"
#include "BattlegroundDG.h"
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
#include "BattlegroundSM.h"
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

namespace MS
{
    namespace Battlegrounds
    {
        //////////////////////////////////////////////////////////////////////////
        /// Battleground Manager
        //////////////////////////////////////////////////////////////////////////

        BattlegroundMgr::BattlegroundMgr()
            : m_BattleMastersMap(),
            m_Battlegrounds(),
            m_ArenaSelectionWeights(),
            m_BGSelectionWeights(),
            m_RatedBGSelectionWeights(),
            m_ClientBattlegroundIds(),
            m_ArenaTesting(false),
            m_Testing(false),
            m_Scheduler(),
            m_InvitationsMgr()
        {
            std::fill(std::begin(m_BattlegroundTemplates), std::end(m_BattlegroundTemplates), nullptr);
        }

        BattlegroundMgr::~BattlegroundMgr()
        {
            DeleteAllBattlegrounds();
        }

        void BattlegroundMgr::DeleteAllBattlegrounds()
        {
             for (std::size_t i = BattlegroundType::Begin; i < BattlegroundType::End; i++)
             {
                 /// Delete template object.
                 if (m_BattlegroundTemplates[i])
                     delete m_BattlegroundTemplates[i];
 
                 /// Delete battleground objects for each bracket id.
                 for (std::size_t l_BracketId = 0; l_BracketId < Brackets::Count; l_BracketId++)
                 {
                     auto& l_Battlegrounds = m_Battlegrounds[l_BracketId][i];
                     for (auto itr : l_Battlegrounds)
                     {
                         Battleground* l_Bg = itr.second;
                         if (!m_ClientBattlegroundIds[i][l_Bg->GetBracketId()].empty())
                             m_ClientBattlegroundIds[i][l_Bg->GetBracketId()].erase(l_Bg->GetClientInstanceID());
                         delete l_Bg;
                     }
 
                     m_Battlegrounds[l_BracketId][i].clear();
                 }
             }
        }

        Battleground* BattlegroundMgr::GetBattlegroundTemplate(BattlegroundType::Type p_BgType) const
        {
            /// We check if the type is in the range of instanciable battlegrounds.
            if (p_BgType < BattlegroundType::Total && !BattlegroundType::IsArena(p_BgType))
                return m_BattlegroundTemplates[p_BgType];
            else if (BattlegroundType::IsArena(p_BgType))
                return m_BattlegroundTemplates[BattlegroundType::AllArenas];
            return nullptr;
        }

        void BattlegroundMgr::Update(uint32 p_Diff)
        {
            /// Update every battleground and delete the ones needed.
            for (std::size_t l_BracketId = 0; l_BracketId < Brackets::Count; l_BracketId++)
            {
                for (std::size_t i = BattlegroundType::Begin; i < BattlegroundType::End; i++)
                {
                    auto& l_Battlegrounds = m_Battlegrounds[l_BracketId][i];
                    auto& l_Clients = m_ClientBattlegroundIds[i];
                    auto& l_I2B = m_InstanceId2Brackets;

                    /// Update every battleground.
                    for (auto const& l_Pair : l_Battlegrounds)
                        l_Pair.second->Update(p_Diff);

                    /// Delete every battleground which asked to be deleted.
                    l_Battlegrounds.remove_if([&l_Clients, i, &l_I2B](std::pair<uint32, Battleground*> const& p_Pair)
                    {
                        Battleground* l_Bg = p_Pair.second;
                        if (l_Bg->ToBeDeleted())
                        {
                            l_I2B.erase(l_Bg->GetInstanceID());
                            l_Clients[l_Bg->GetBracketId()].erase(l_Bg->GetClientInstanceID());
                            delete l_Bg;

                            return true;
                        }

                        return false;
                    });
                }
            }

            /// Update InvitationsMgr.
            m_InvitationsMgr.UpdateEvents(p_Diff);

            /// Update Scheduler.
            m_Scheduler.FindMatches();
        }

        Battleground* BattlegroundMgr::GetBattleground(uint32 p_InstanceId, BattlegroundType::Type p_BgType) const
        {
            if (!p_InstanceId)
                return nullptr;

            Battleground* l_TemplateBg = GetBattlegroundTemplate(p_BgType);
            if (!l_TemplateBg)
                return nullptr;

            /// We get the bracket id of this instance id.
            auto l_InstanceId2Bracket = m_InstanceId2Brackets.find(p_InstanceId);
            if (l_InstanceId2Bracket == std::end(m_InstanceId2Brackets))
                return nullptr;

            /// We look for the battleground with this instance id.
            auto& l_Battlegrounds = m_Battlegrounds[l_InstanceId2Bracket->second][p_BgType];
            auto l_Results = std::find_if(std::begin(l_Battlegrounds), std::end(l_Battlegrounds), [p_InstanceId](std::pair<uint32, Battleground*> const& p_Bg)
            {
                return p_Bg.first == p_InstanceId;
            });

            if (l_Results != std::end(l_Battlegrounds))
                return l_Results->second;

            return nullptr;
        }

        uint32 BattlegroundMgr::CreateClientVisibleInstanceId(BattlegroundType::Type p_BgType, Bracket::Id p_BracketId)
        {
            /// Arenas don't have client instance id.
            if (IsArenaType(p_BgType))
                return 0;

            /// We create here an instance id, which is just for
            /// displaying this to the client and without any other use..
            /// The client-instanceIds are unique for each battleground-type.
            /// The instance-id just needs to be as low as possible, beginning with 1.
            /// The following works, because std::set is default ordered.
            /// The optimalization would be to use as bitmask std::vector<uint32> - but that would only make code unreadable.
            uint32 l_LastId = 0;
            for (std::set<uint32>::iterator l_Itr = m_ClientBattlegroundIds[p_BgType][p_BracketId].begin(); l_Itr != m_ClientBattlegroundIds[p_BgType][p_BracketId].end();)
            {
                /// We are looking for gaps in order to fill them.
                if ((++l_LastId) != *l_Itr)
                    break;
                l_LastId = *l_Itr;
            }

            m_ClientBattlegroundIds[p_BgType][p_BracketId].insert(l_LastId + 1);
            return l_LastId + 1;
        }

        Battleground* BattlegroundMgr::CreateNewBattleground(BattlegroundType::Type p_BgType, Bracket const* p_BracketEntry, uint8 p_ArenaType, bool p_IsSkirmish, bool p_IsWargame, bool p_UseTournamentRules, bool p_RatedBg)
        {
            /// Get the template battleground.
            Battleground* l_BattlegroundTemplate = GetBattlegroundTemplate(p_BgType);
            if (!l_BattlegroundTemplate)
            {
                sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground: CreateNewBattleground - bg template not found for %u", p_BgType);
                return nullptr;
            }

            Battleground* l_Battleground = nullptr;

            /// We create a copy of the template.
            switch (p_BgType)
            {
                case BattlegroundType::AlteracValley:
                    l_Battleground = new BattlegroundAV(*(BattlegroundAV*)l_BattlegroundTemplate);
                    break;
                case BattlegroundType::Warsong:
                    l_Battleground = new BattlegroundWS(*(BattlegroundWS*)l_BattlegroundTemplate);
                    break;
                case BattlegroundType::ArathiBassin:
                    l_Battleground = new BattlegroundAB(*(BattlegroundAB*)l_BattlegroundTemplate);
                    break;
                case BattlegroundType::NagrandArena:
                    l_Battleground = new BattlegroundNA(*(BattlegroundNA*)l_BattlegroundTemplate);
                    break;
                case BattlegroundType::TolvironArena:
                    l_Battleground = new BattlegroundTV(*(BattlegroundTV*)l_BattlegroundTemplate);
                    break;
                case BattlegroundType::TigersPeaks:
                    l_Battleground = new BattlegroundTTP(*(BattlegroundTTP*)l_BattlegroundTemplate);
                    break;
                case BattlegroundType::BladeEdgeArena:
                    l_Battleground = new BattlegroundBE(*(BattlegroundBE*)l_BattlegroundTemplate);
                    break;
                case BattlegroundType::EyeOfTheStorm:
                    l_Battleground = new BattlegroundEY(*(BattlegroundEY*)l_BattlegroundTemplate);
                    break;
                case BattlegroundType::RuinsOfLordaeron:
                    l_Battleground = new BattlegroundRL(*(BattlegroundRL*)l_BattlegroundTemplate);
                    break;
                case BattlegroundType::StrandOfTheAncients:
                    l_Battleground = new BattlegroundSA(*(BattlegroundSA*)l_BattlegroundTemplate);
                    break;
                case BattlegroundType::DalaranArena:
                    l_Battleground = new BattlegroundDS(*(BattlegroundDS*)l_BattlegroundTemplate);
                    break;
                case BattlegroundType::TheRingOfValor:
                    l_Battleground = new BattlegroundRV(*(BattlegroundRV*)l_BattlegroundTemplate);
                    break;
                case BattlegroundType::IsleOfConquest:
                    l_Battleground = new BattlegroundIC(*(BattlegroundIC*)l_BattlegroundTemplate);
                    break;
                case BattlegroundType::TwinPeaks:
                    l_Battleground = new BattlegroundTP(*(BattlegroundTP*)l_BattlegroundTemplate);
                    break;
                case BattlegroundType::BattleForGilneas:
                    l_Battleground = new BattlegroundBFG(*(BattlegroundBFG*)l_BattlegroundTemplate);
                    break;
                case BattlegroundType::KotmoguTemple:
                    l_Battleground = new BattlegroundKT(*(BattlegroundKT*)l_BattlegroundTemplate);
                    break;
                case BattlegroundType::SilvershardMines:
                    l_Battleground = new BattlegroundSM(*(BattlegroundSM*)l_BattlegroundTemplate);
                    break;
                case BattlegroundType::DeepwindGorge:
                    l_Battleground = new BattlegroundDG(*(BattlegroundDG*)l_BattlegroundTemplate);
                    break;
                default:
                    /// Error, we got a non valid type.
                    return nullptr;
            }

            /// Set battleground difficulty before initialization.
            l_Battleground->SetBracket(p_BracketEntry);

            /// Generate a new instance id.
            l_Battleground->SetInstanceID(sMapMgr->GenerateInstanceId());
            m_InstanceId2Brackets[l_Battleground->GetInstanceID()] = p_BracketEntry->m_Id;
            l_Battleground->SetClientInstanceID(CreateClientVisibleInstanceId(p_RatedBg ? BattlegroundType::RatedBg10v10 : p_BgType, p_BracketEntry->m_Id));

            /// Reset the new battleground.
            l_Battleground->Reset();

            /// Start the joining of the battleground.
            l_Battleground->SetStatus(STATUS_WAIT_JOIN);
            l_Battleground->SetArenaType(p_ArenaType);
            l_Battleground->SetRatedBG(p_RatedBg);
            l_Battleground->SetSkirmish(p_IsSkirmish);
            l_Battleground->SetRandom(true);
            l_Battleground->SetWargame(p_IsWargame);
            l_Battleground->SetTypeID(GetIdFromType(p_RatedBg ? BattlegroundType::RatedBg10v10 : p_BgType));
            l_Battleground->SetRandomTypeID(GetIdFromType(p_BgType));
            l_Battleground->InitGUID();

            if (p_UseTournamentRules)
                l_Battleground->EnableTournamentRules();

            /// We store the battleground.
            m_Battlegrounds[p_BracketEntry->m_Id][p_RatedBg ? BattlegroundType::RatedBg10v10 : p_BgType].emplace_back(std::make_pair(l_Battleground->GetInstanceID(), l_Battleground));

            return l_Battleground;
        }

        uint32 BattlegroundMgr::CreateBattlegroundTemplate(CreateBattlegroundData& data)
        {
            /// Create the battleground template.
            Battleground* l_Bg = nullptr;
            switch (data.bgTypeId)
            {
            case BATTLEGROUND_AV:
                l_Bg = new BattlegroundAV;
                break;
            case BATTLEGROUND_WS:
                l_Bg = new BattlegroundWS;
                break;
            case BATTLEGROUND_AB:
                l_Bg = new BattlegroundAB;
                break;
            case BATTLEGROUND_NA:
                l_Bg = new BattlegroundNA;
                break;
            case BATTLEGROUND_TV:
                l_Bg = new BattlegroundTV;
                break;
            case BATTLEGROUND_TTP:
                l_Bg = new BattlegroundTTP;
                break;
            case BATTLEGROUND_BE:
                l_Bg = new BattlegroundBE;
                break;
            case BATTLEGROUND_AA:
                l_Bg = new BattlegroundAA;
                break;
            case BATTLEGROUND_EY:
            case BATTLEGROUND_EYR:
                l_Bg = new BattlegroundEY;
                break;
            case BATTLEGROUND_DG:
                l_Bg = new BattlegroundDG;
                break;
            case BATTLEGROUND_SM:
                l_Bg = new BattlegroundSM;
                break;
            case BATTLEGROUND_RL:
                l_Bg = new BattlegroundRL;
                break;
            case BATTLEGROUND_SA:
                l_Bg = new BattlegroundSA;
                break;
            case BATTLEGROUND_DS:
                l_Bg = new BattlegroundDS;
                break;
            case BATTLEGROUND_RV:
                l_Bg = new BattlegroundRV;
                break;
            case BATTLEGROUND_IC:
                l_Bg = new BattlegroundIC;
                break;
            case BATTLEGROUND_TP:
                l_Bg = new BattlegroundTP;
                break;
            case BATTLEGROUND_BFG:
                l_Bg = new BattlegroundBFG;
                break;
            case BATTLEGROUND_RB:
                l_Bg = new BattlegroundRB;
                break;
            case BATTLEGROUND_KT:
                l_Bg = new BattlegroundKT;
                break;
            case BATTLEGROUND_RATED_10_VS_10:
                l_Bg = new BattlegroundRBG;
                break;
            default:
                l_Bg = new Battleground;
                break;
            }

            l_Bg->SetMapId(data.MapID);
            l_Bg->SetTypeID(data.bgTypeId);
            l_Bg->InitGUID();
            l_Bg->SetInstanceID(0);
            l_Bg->SetArenaorBGType(data.IsArena);
            l_Bg->SetRatedBG(data.bgTypeId == BATTLEGROUND_RATED_10_VS_10);
            l_Bg->SetMinPlayersPerTeam(data.MinPlayersPerTeam);
            l_Bg->SetMaxPlayersPerTeam(data.MaxPlayersPerTeam);
            l_Bg->SetMinPlayers(data.MinPlayersPerTeam * 2);
            l_Bg->SetMaxPlayers(data.MaxPlayersPerTeam * 2);
            l_Bg->SetName(data.BattlegroundName);
            l_Bg->SetTeamStartLoc(ALLIANCE, data.Team1StartLocX, data.Team1StartLocY, data.Team1StartLocZ, data.Team1StartLocO);
            l_Bg->SetTeamStartLoc(HORDE, data.Team2StartLocX, data.Team2StartLocY, data.Team2StartLocZ, data.Team2StartLocO);
            l_Bg->SetStartMaxDist(data.StartMaxDist);
            l_Bg->SetLevelRange(data.LevelMin, data.LevelMax);
            l_Bg->SetHolidayId(data.holiday);
            l_Bg->SetScriptId(data.scriptId);

            /// Add template.
            m_BattlegroundTemplates[GetSchedulerType(l_Bg->GetTypeID())] = l_Bg;

            /// Return some not-null value, bgTypeId is good enough for me.
            return data.bgTypeId;
        }

        void BattlegroundMgr::CreateInitialBattlegrounds()
        {
            uint32 l_OldMSTime = getMSTime();

            uint8 l_SelectionWeight;
            BattlemasterListEntry const* l_Bl = nullptr;
            BattlemasterListEntry const* l_RatedBl = sBattlemasterListStore.LookupEntry(BATTLEGROUND_RATED_10_VS_10);

            //                                               0   1                  2                  3       4       5                 6               7              8      9        10
            QueryResult l_Result = WorldDatabase.Query("SELECT id, MinPlayersPerTeam, MaxPlayersPerTeam, MinLvl, MaxLvl, AllianceStartLoc, HordeStartLoc, StartMaxDist, Weight, holiday, ScriptName FROM battleground_template");

            if (!l_Result)
            {
                sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 battlegrounds. DB table `battleground_template` is empty.");
                return;
            }

            uint32 l_Count = 0;
            uint32 l_StartId = 0;

            do
            {
                Field* l_Fields = l_Result->Fetch();

                uint32 l_BgTypeId = l_Fields[0].GetUInt32();
                if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, l_BgTypeId, nullptr))
                    continue;

                // can be overwrite by values from DB
                l_Bl = sBattlemasterListStore.LookupEntry(l_BgTypeId);
                if (!l_Bl)
                {
                    sLog->outError(LOG_FILTER_BATTLEGROUND, "Battleground ID %u not found in BattlemasterList.dbc. Battleground not created.", l_BgTypeId);
                    continue;
                }

                CreateBattlegroundData l_Data;
                l_Data.bgTypeId = BattlegroundTypeId(l_BgTypeId);
                l_Data.IsArena = (l_Bl->InstanceType == TYPE_ARENA);
                l_Data.MinPlayersPerTeam = l_Fields[1].GetUInt16();
                l_Data.MaxPlayersPerTeam = l_Fields[2].GetUInt16();
                l_Data.LevelMin = l_Fields[3].GetUInt8();
                l_Data.LevelMax = l_Fields[4].GetUInt8();

                if (l_Data.MinPlayersPerTeam == 0)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `battleground_template` for id %u has bad values for MinPlayersPerTeam (%u)",
                        l_Data.bgTypeId, l_Data.MinPlayersPerTeam, l_Data.MaxPlayersPerTeam); ///< Data argument not used by format string
                    assert(false);
                }

                // check values from DB
                if (l_Data.MaxPlayersPerTeam == 0 || l_Data.MinPlayersPerTeam > l_Data.MaxPlayersPerTeam)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `battleground_template` for id %u has bad values for MinPlayersPerTeam (%u) and MaxPlayersPerTeam(%u)",
                        l_Data.bgTypeId, l_Data.MinPlayersPerTeam, l_Data.MaxPlayersPerTeam);
                    continue;
                }

                if (l_Data.LevelMin == 0 || l_Data.LevelMax == 0 || l_Data.LevelMin > l_Data.LevelMax)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `battleground_template` for id %u has bad values for LevelMin (%u) and LevelMax(%u)",
                        l_Data.bgTypeId, l_Data.LevelMin, l_Data.LevelMax);
                    continue;
                }

                l_StartId = l_Fields[5].GetUInt32();
                if (WorldSafeLocsEntry const* l_Start = sWorldSafeLocsStore.LookupEntry(l_StartId))
                {
                    l_Data.Team1StartLocX = l_Start->x;
                    l_Data.Team1StartLocY = l_Start->y;
                    l_Data.Team1StartLocZ = l_Start->z;
                    l_Data.Team1StartLocO = l_Start->o;
                }
                else if (l_Data.bgTypeId == BATTLEGROUND_AA || l_Data.bgTypeId == BATTLEGROUND_RB || l_Data.bgTypeId == BATTLEGROUND_RATED_10_VS_10)
                {
                    l_Data.Team1StartLocX = 0;
                    l_Data.Team1StartLocY = 0;
                    l_Data.Team1StartLocZ = 0;
                    l_Data.Team1StartLocO = 0;
                }
                else
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `battleground_template` for id %u have non-existed WorldSafeLocs.dbc id %u in field `AllianceStartLoc`. BG not created.", l_Data.bgTypeId, l_StartId);
                    continue;
                }

                l_StartId = l_Fields[6].GetUInt32();
                if (WorldSafeLocsEntry const* l_Start = sWorldSafeLocsStore.LookupEntry(l_StartId))
                {
                    l_Data.Team2StartLocX = l_Start->x;
                    l_Data.Team2StartLocY = l_Start->y;
                    l_Data.Team2StartLocZ = l_Start->z;
                    l_Data.Team2StartLocO = l_Start->o;
                }
                else if (l_Data.bgTypeId == BATTLEGROUND_AA || l_Data.bgTypeId == BATTLEGROUND_RB || l_Data.bgTypeId == BATTLEGROUND_RATED_10_VS_10)
                {
                    l_Data.Team2StartLocX = 0;
                    l_Data.Team2StartLocY = 0;
                    l_Data.Team2StartLocZ = 0;
                    l_Data.Team2StartLocO = 0;
                }
                else
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `battleground_template` for id %u have non-existed WorldSafeLocs.dbc id %u in field `HordeStartLoc`. BG not created.", l_Data.bgTypeId, l_StartId);
                    continue;
                }

                l_Data.StartMaxDist = l_Fields[7].GetFloat();

                l_SelectionWeight = l_Fields[8].GetUInt8();
                l_Data.holiday = l_Fields[9].GetUInt32();
                l_Data.scriptId = sObjectMgr->GetScriptId(l_Fields[10].GetCString());
                l_Data.MapID = l_Bl->MapID[0];

                if (!CreateBattlegroundTemplate(l_Data))
                    continue;

                if (l_Data.IsArena)
                {
                    if (l_Data.bgTypeId != BATTLEGROUND_AA)
                        m_ArenaSelectionWeights[l_Data.bgTypeId] = l_SelectionWeight;
                }
                else if (l_Data.bgTypeId != BATTLEGROUND_RB && l_Data.bgTypeId != BATTLEGROUND_RATED_10_VS_10
                    && l_Data.bgTypeId != BATTLEGROUND_RATED_15_VS_15 && l_Data.bgTypeId != BATTLEGROUND_RATED_25_VS_25)
                    m_BGSelectionWeights[l_Data.bgTypeId] = l_SelectionWeight;

                for (int i = 0; i < 16; ++i)
                    if (l_RatedBl->MapID[i] == l_Bl->MapID[0] && l_Bl->MapID[1] == -1) ///< Comparison of integers of different signs: 'const uint32' (aka 'const unsigned int') and 'int'
                        m_RatedBGSelectionWeights[l_Data.bgTypeId] = l_SelectionWeight;

                ++l_Count;
            } while (l_Result->NextRow());

            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u battlegrounds in %u ms", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
        }

        void BattlegroundMgr::TeleportToBattleground(Player* p_Player, uint32 p_InstanceId, BattlegroundType::Type p_BgType)
        {
            Battleground* l_Bg = GetBattleground(p_InstanceId, p_BgType);
            if (l_Bg)
            {
                uint32 l_MapId = l_Bg->GetMapId();
                float l_X, l_Y, l_Z, l_O;
                uint32 l_Team = p_Player->GetBGTeam();
                if (l_Team == 0)
                    l_Team = p_Player->GetTeam();
                l_Bg->GetTeamStartLoc(l_Team, l_X, l_Y, l_Z, l_O);

                sLog->outInfo(LOG_FILTER_BATTLEGROUND, "BATTLEGROUND: Sending %s to map %u, X %f, Y %f, Z %f, O %f", p_Player->GetName(), l_MapId, l_X, l_Y, l_Z, l_O);
                p_Player->TeleportTo(l_MapId, l_X, l_Y, l_Z, l_O);
            }
            else
            {
                sLog->outError(LOG_FILTER_BATTLEGROUND, "player %u is trying to port to non-existent bg instance %u", p_Player->GetGUIDLow(), p_InstanceId);
            }
        }

        //////////////////////////////////////////////////////////////////////////
        /// Config stuff
        //////////////////////////////////////////////////////////////////////////

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

        uint32 BattlegroundMgr::GetMaxRatingDifference() const
        {
            // this is for stupid people who can't use brain and set max rating difference to 0
            uint32 l_Diff = sWorld->getIntConfig(CONFIG_ARENA_MAX_RATING_DIFFERENCE);
            if (l_Diff == 0)
                l_Diff = 5000;
            return l_Diff;
        }

        uint32 BattlegroundMgr::GetRatingDiscardTimer() const
        {
            return sWorld->getIntConfig(CONFIG_ARENA_RATING_DISCARD_TIMER);
        }

        uint32 BattlegroundMgr::GetPrematureFinishTime() const
        {
            return sWorld->getIntConfig(CONFIG_BATTLEGROUND_PREMATURE_FINISH_TIMER);
        }

        //////////////////////////////////////////////////////////////////////////
        /// Loading stuff
        //////////////////////////////////////////////////////////////////////////

        void BattlegroundMgr::LoadBattleMastersEntry()
        {
            uint32 l_OldMSTime = getMSTime();

            m_BattleMastersMap.clear();                                  // need for reload case

            QueryResult l_Result = WorldDatabase.Query("SELECT entry, bg_template FROM battlemaster_entry");

            if (!l_Result)
            {
                sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 battlemaster entries. DB table `battlemaster_entry` is empty!");
                return;
            }

            uint32 l_Count = 0;
            do
            {
                ++l_Count;

                Field* l_Fields = l_Result->Fetch();

                uint32 l_Entry = l_Fields[0].GetUInt32();
                uint32 l_BgTypeId = l_Fields[1].GetUInt32();
                if (!sBattlemasterListStore.LookupEntry(l_BgTypeId))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `battlemaster_entry` contain entry %u for not existed battleground type %u, ignored.", l_Entry, l_BgTypeId);
                    continue;
                }

                m_BattleMastersMap[l_Entry] = BattlegroundTypeId(l_BgTypeId);
            } while (l_Result->NextRow());

            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u battlemaster entries in %u ms", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
        }

        //////////////////////////////////////////////////////////////////////////
        /// Holiday stuff
        //////////////////////////////////////////////////////////////////////////

        void BattlegroundMgr::SetHolidayWeekends(std::list<uint32> const& activeHolidayId)
        {
            for (std::size_t l_BgType = BattlegroundType::Begin; l_BgType < BattlegroundType::End; l_BgType++)
            {
                if (Battleground* bg = GetBattlegroundTemplate(static_cast<BattlegroundType::Type>(l_BgType)))
                {
                    bool holidayActivate = false;

                    if (uint32 holidayId = bg->GetHolidayId())
                        for (auto activeId : activeHolidayId)
                            if (holidayId == activeId)
                                holidayActivate = true;

                    bg->SetHoliday(holidayActivate);
                }
            }
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
    } ///< namespace Battlegrounds.
} ///< namespace MS.