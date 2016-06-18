////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __BATTLEGROUND_H
#define __BATTLEGROUND_H

#include "Common.h"
#include "SharedDefines.h"
#include "DBCEnums.h"
#include "ObjectDefines.h"
#include "ByteBuffer.h"
#include "Unit.h"
#include "AchievementMgr.h"
#include "Arena.h"

class Creature;
class GameObject;
class Group;
class Player;
class WorldPacket;
class BattlegroundMap;
class Unit;

struct WorldSafeLocsEntry;

#ifdef CROSS
enum BattlegroundAreaIds
{
    BATTLEGROUND_AB_ZONEID  = 3358,
    BATTLEGROUND_AV_ZONEID  = 2597,
    BATTLEGROUND_BE_ZONEID  = 3702,
    BATTLEGROUND_BFG_ZONEID = 5108,
    BATTLEGROUND_DS_ZONEID  = 4378,
    BATTLEGROUND_EY_ZONEID  = 5799,
    BATTLEGROUND_IC_ZONEID  = 4710,
    BATTLEGROUND_NA_ZONEID  = 3698,
    BATTLEGROUND_RL_ZONEID  = 3968,
    BATTLEGROUND_RV_ZONEID  = 4406,
    BATTLEGROUND_SA_ZONEID  = 4384,
    BATTLEGROUND_TP_ZONEID  = 5031,
    BATTLEGROUND_WS_ZONEID  = 3277,
    BATTLEGROUND_TV_ZONEID  = 6296,
    BATTLEGROUND_TTP_ZONEID = 6732,
    BATTLEGROUND_KT_ZONEID  = 6051,
    BATTLEGROUND_SSM_ZONEID = 6126,
    BATTLEGROUND_DG_ZONEID  = 6665,
};

#endif /* CROSS */
#define COUNT_OF_PLAYERS_TO_AVERAGE_WAIT_TIME 10

namespace MS
{
    namespace Battlegrounds
    {
        namespace TeamsCount
        {
            enum
            {
                Value = 2
            };
        }

        /// Correponds to the spells related to battlegrounds.
        namespace Spells
        {
            enum Type
            {
                DeserterBuff = 26013
            };
        }

        namespace RewardCurrencyType
        {
            enum Type
            {
                None                    = 0,
                Kill                    = 7,
                BattlegroundWin         = 8,
                BattlegroundRated       = 15,
                BattlegroundObjectif    = 16,
                Arena                   = 17
            };
        }

        /// This enum is for INTERNAL purpose, do not use it as a client battleground type id.
        namespace BattlegroundType
        {
            enum Type
            {
                /// Begin iterators.
                Begin                = 0,
                /// Battlegrounds.
                AlteracValley        = 0,
                Warsong              = 1,
                ArathiBassin         = 2,
                EyeOfTheStorm        = 3,
                TwinPeaks            = 4,
                BattleForGilneas     = 5,
                StrandOfTheAncients  = 6,
                IsleOfConquest       = 7,
                KotmoguTemple        = 8,
                SilvershardMines     = 9,
                DeepwindGorge        = 10,
                /// Arenas.
                Arena2v2             = 11,
                Arena3v3             = 12,
                Arena5v5             = 13,
                ArenaSkirmish2v2     = 14,
                ArenaSkirmish3v3     = 15,
                /// Rated battlegrounds.
                RatedBg10v10         = 16,
                /// Instanciable arenas.
                TigersPeaks          = 17,
                TolvironArena        = 18,
                BladeEdgeArena       = 19,
                DalaranArena         = 20,
                RuinsOfLordaeron     = 21,
                NagrandArena         = 22,
                /// End iterators.
                End                  = 23,
                RatedBg15v15,
                RatedBg25v25,
                RandomBattleground,
                AllArenas,
                Ctf3,
                /// Arena types.
                /// Non instanciable.
                Total,
                None = End,
                Max = End,
                EndArena = ArenaSkirmish3v3,
                BeginArena = Arena2v2,
                NumBattlegrounds = Arena2v2,
                TheRingOfValor, /// Deprecated.
            };

            /// Check if the BattlegroundType given is a casual battleground.
            /// @p_Type : The type of the battleground.
            inline bool IsCasualBattleground(Type p_Type) ///< IsCasualBattleground is unused
            {
                return p_Type < BattlegroundType::NumBattlegrounds || p_Type == BattlegroundType::RandomBattleground;
            }

            /// Check if the BattlegroundType given is an instance of an arena.
            /// @p_Type : The type of the battleground.
            inline bool IsArena(Type p_Type) ///< IsArena is unused
            {
                return p_Type >= BeginArena && p_Type <= EndArena;
            }

            /// Check if the BattlegroundType given is a rated battleground.
            /// @p_Type : The type of the battleground.
            inline bool IsRated(Type p_Type) ///< IsRated is unused 22/02/16
            {
                return (p_Type == RatedBg10v10 || p_Type == RatedBg15v15 || p_Type == RatedBg25v25) || (p_Type >= Arena2v2 && p_Type <= Arena5v5);
            }

            /// Return the ArenaType of the battleground.
            /// @p_Type : The type of the battleground.
            inline ArenaType GetArenaType(Type p_Type) ///< GetArenaType is unused
            {
                switch (p_Type)
                {
                    case BattlegroundType::Arena2v2:
                    case BattlegroundType::ArenaSkirmish2v2:
                        return ArenaType::Arena2v2;
                    case BattlegroundType::Arena3v3:
                    case BattlegroundType::ArenaSkirmish3v3:
                        return ArenaType::Arena3v3;
                    case BattlegroundType::Arena5v5:
                    default:
                        return ArenaType::Arena5v5;
                }
            }

            /// Return true if the arena type is skirmish.
            inline bool IsSkirmish(Type p_Type) ///< IsSkirmish is unused
            {
                switch (p_Type)
                {
                    case BattlegroundType::ArenaSkirmish2v2:
                    case BattlegroundType::ArenaSkirmish3v3:
                        return true;
                    default:
                        return false;
                }
            }
        }

        struct Bracket
        {
            using Id = uint32;
            Id m_Id;
            uint32 m_MinLevel;
            uint32 m_MaxLevel;
        };

        /// We don't want this variables to be accessible outside of the file.
        namespace
        {
            static std::map<uint32, BattlegroundType::Type> k_MapIdToBattlegroundType =
            {
                { 489, BattlegroundType::Warsong },
                { 529, BattlegroundType::ArathiBassin },
                { 566, BattlegroundType::EyeOfTheStorm },
                { 30, BattlegroundType::AlteracValley },
                { 761, BattlegroundType::BattleForGilneas },
                { 726, BattlegroundType::TwinPeaks },
                { 727, BattlegroundType::SilvershardMines },
                { 998, BattlegroundType::KotmoguTemple },
                { 1105, BattlegroundType::DeepwindGorge },
                { 607, BattlegroundType::StrandOfTheAncients },
                { 628, BattlegroundType::IsleOfConquest }
            };

            static const Bracket k_Brackets[] =
            {
                /// Normals.
                { 0, 0, 9 },
                { 1, 15, 19 },
                { 2, 10, 19 },
                { 3, 25, 29 },
                { 4, 20, 29 },
                { 5, 35, 39 },
                { 6, 30, 39 },
                { 7, 45, 49 },
                { 8, 40, 49 },
                { 9, 55, 59 },
                { 10, 50, 59 },
                { 11, 60, 69 },
                { 12, 70, 79 },
                { 13, 80, 89 },
                { 14, 90, 90 },
                { 15, 90, 99 },
                { 16, 100, 100 },
                { 17, 150, 150 }
            };
        }

        namespace Maps
        {
            inline BattlegroundType::Type FindAssociatedType(uint32 p_MapId) ///< FindAssociatedType is unused
            {
                auto l_Itr = k_MapIdToBattlegroundType.find(p_MapId);
                if (l_Itr != std::end(k_MapIdToBattlegroundType))
                    return l_Itr->second;

                return BattlegroundType::None;
            }
        }

        namespace Brackets
        {
            enum
            {
                Count = sizeof (k_Brackets) / sizeof (k_Brackets[0])
            };

            inline Bracket const* FindForLevel(std::size_t p_Level) ///< FindForLevel is unused
            {
                for (std::size_t i = 0; i < Count; i++)
                {
                    if (k_Brackets[i].m_MinLevel <= p_Level && p_Level <= k_Brackets[i].m_MaxLevel)
                        return &k_Brackets[i];
                }

                return nullptr;
            }

            inline Bracket const* RetreiveFromId(Bracket::Id p_Id) ///< RetreiveFromId is unused
            {
                return &k_Brackets[p_Id];
            }
        }

        inline BattlegroundTypeId GetIdFromType(BattlegroundType::Type p_Type) ///< GetIdFromType is unused
        {
            switch (p_Type)
            {
            case BattlegroundType::Warsong:
                return BATTLEGROUND_WS;
            case BattlegroundType::ArathiBassin:
                return BATTLEGROUND_AB;
            case BattlegroundType::AlteracValley:
                return BATTLEGROUND_AV;
            case BattlegroundType::EyeOfTheStorm:
                return BATTLEGROUND_EY;
            case BattlegroundType::StrandOfTheAncients:
                return BATTLEGROUND_SA;
            case BattlegroundType::IsleOfConquest:
                return BATTLEGROUND_IC;
            case BattlegroundType::TwinPeaks:
                return BATTLEGROUND_TP;
            case BattlegroundType::BattleForGilneas:
                return BATTLEGROUND_BFG;
            case BattlegroundType::RandomBattleground:
                return BATTLEGROUND_RB;
            case BattlegroundType::KotmoguTemple:
                return BATTLEGROUND_KT;
            case BattlegroundType::Ctf3:
                return BATTLEGROUND_CTF3;
            case BattlegroundType::SilvershardMines:
                return BATTLEGROUND_SM;
            case BattlegroundType::DeepwindGorge:
                return BATTLEGROUND_DG;
            case BattlegroundType::TigersPeaks:
                return BATTLEGROUND_TTP;
            case BattlegroundType::TolvironArena:
                return BATTLEGROUND_TV;
            case BattlegroundType::BladeEdgeArena:
                return BATTLEGROUND_BE;
            case BattlegroundType::DalaranArena:
                return BATTLEGROUND_DS;
            case BattlegroundType::RuinsOfLordaeron:
                return BATTLEGROUND_RL;
            case BattlegroundType::NagrandArena:
                return BATTLEGROUND_NA;
            case BattlegroundType::TheRingOfValor:
                return BATTLEGROUND_RV;
            case BattlegroundType::RatedBg10v10:
                return BATTLEGROUND_RATED_10_VS_10;
            case BattlegroundType::RatedBg15v15:
                return BATTLEGROUND_RATED_15_VS_15;
            case BattlegroundType::RatedBg25v25:
                return BATTLEGROUND_RATED_25_VS_25;
            case BattlegroundType::AllArenas:
                return BATTLEGROUND_AA;
            default:
                return BATTLEGROUND_TYPE_NONE;
            }
        }

        inline BattlegroundType::Type GetTypeFromId(BattlegroundTypeId p_BgTypeId, uint8 p_ArenaType, bool p_IsSkirmish = false) ///< GetTypeFromId is unused
        {
            switch (p_BgTypeId)
            {
            case BATTLEGROUND_WS:
                return BattlegroundType::Warsong;
            case BATTLEGROUND_AB:
                return BattlegroundType::ArathiBassin;
            case BATTLEGROUND_AV:
                return BattlegroundType::AlteracValley;
            case BATTLEGROUND_EY:
                return BattlegroundType::EyeOfTheStorm;
            case BATTLEGROUND_SA:
                return BattlegroundType::StrandOfTheAncients;
            case BATTLEGROUND_IC:
                return BattlegroundType::IsleOfConquest;
            case BATTLEGROUND_TP:
                return BattlegroundType::TwinPeaks;
            case BATTLEGROUND_BFG:
                return BattlegroundType::BattleForGilneas;
            case BATTLEGROUND_RB:
                return BattlegroundType::RandomBattleground;
            case BATTLEGROUND_KT:
                return BattlegroundType::KotmoguTemple;
            case BATTLEGROUND_CTF3:
                return BattlegroundType::Ctf3;
            case BATTLEGROUND_SM:
                return BattlegroundType::SilvershardMines;
            case BATTLEGROUND_DG:
                return BattlegroundType::DeepwindGorge;
            case BATTLEGROUND_RATED_10_VS_10:
                return BattlegroundType::RatedBg10v10;
            case BATTLEGROUND_RATED_15_VS_15:
                return BattlegroundType::RatedBg15v15;
            case BATTLEGROUND_RATED_25_VS_25:
                return BattlegroundType::RatedBg25v25;
            case BATTLEGROUND_AA:
            case BATTLEGROUND_NA:
            case BATTLEGROUND_TV:
            case BATTLEGROUND_TTP:
            case BATTLEGROUND_RL:
            case BATTLEGROUND_BE:
            case BATTLEGROUND_DS:
            case BATTLEGROUND_RV:
                switch (p_ArenaType)
                {
                case ArenaType::Arena2v2:
                    return p_IsSkirmish ? BattlegroundType::ArenaSkirmish2v2 : BattlegroundType::Arena2v2;
                case ArenaType::Arena3v3:
                    return p_IsSkirmish ? BattlegroundType::ArenaSkirmish3v3 : BattlegroundType::Arena3v3;
                case ArenaType::Arena5v5:
                    return BattlegroundType::Arena5v5;
                default:
                    return BattlegroundType::None;
                }
            default:
                return BattlegroundType::None;
            }
        }

        inline BattlegroundType::Type GetSchedulerType(BattlegroundTypeId p_BgTypeId) ///< GetSchedulerType is unused
        {
            switch (p_BgTypeId)
            {
            case BATTLEGROUND_WS:
                return BattlegroundType::Warsong;
            case BATTLEGROUND_AB:
                return BattlegroundType::ArathiBassin;
            case BATTLEGROUND_AV:
                return BattlegroundType::AlteracValley;
            case BATTLEGROUND_EY:
                return BattlegroundType::EyeOfTheStorm;
            case BATTLEGROUND_SA:
                return BattlegroundType::StrandOfTheAncients;
            case BATTLEGROUND_IC:
                return BattlegroundType::IsleOfConquest;
            case BATTLEGROUND_TP:
                return BattlegroundType::TwinPeaks;
            case BATTLEGROUND_BFG:
                return BattlegroundType::BattleForGilneas;
            case BATTLEGROUND_RB:
                return BattlegroundType::RandomBattleground;
            case BATTLEGROUND_KT:
                return BattlegroundType::KotmoguTemple;
            case BATTLEGROUND_CTF3:
                return BattlegroundType::Ctf3;
            case BATTLEGROUND_SM:
                return BattlegroundType::SilvershardMines;
            case BATTLEGROUND_DG:
                return BattlegroundType::DeepwindGorge;
            case BATTLEGROUND_RATED_10_VS_10:
                return BattlegroundType::RatedBg10v10;
            case BATTLEGROUND_RATED_15_VS_15:
                return BattlegroundType::RatedBg15v15;
            case BATTLEGROUND_RATED_25_VS_25:
                return BattlegroundType::RatedBg25v25;
            case BATTLEGROUND_AA:
                return BattlegroundType::AllArenas;
            case BATTLEGROUND_NA:
                return BattlegroundType::NagrandArena;
            case BATTLEGROUND_TV:
                return BattlegroundType::TolvironArena;
            case BATTLEGROUND_TTP:
                return BattlegroundType::TigersPeaks;
            case BATTLEGROUND_RL:
                return BattlegroundType::RuinsOfLordaeron;
            case BATTLEGROUND_BE:
                return BattlegroundType::BladeEdgeArena;
            case BATTLEGROUND_DS:
                return BattlegroundType::DalaranArena;
            case BATTLEGROUND_RV:
                return BattlegroundType::TheRingOfValor;
            default:
                return BattlegroundType::None;
            }
        }

        inline bool IsArenaType(BattlegroundType::Type p_BgType) ///< IsArenaType is unused
        {
            return (p_BgType == BattlegroundType::AllArenas ||
                p_BgType == BattlegroundType::BladeEdgeArena ||
                p_BgType == BattlegroundType::NagrandArena ||
                p_BgType == BattlegroundType::TigersPeaks ||
                p_BgType == BattlegroundType::TolvironArena ||
                p_BgType == BattlegroundType::DalaranArena ||
                p_BgType == BattlegroundType::TheRingOfValor ||
                p_BgType == BattlegroundType::RuinsOfLordaeron);
        }

        inline uint8 BGArenaType(BattlegroundType::Type p_BgType) ///< BGArenaType is unused
        {
            switch (p_BgType)
            {
            case BattlegroundType::Arena2v2:
            case BattlegroundType::ArenaSkirmish2v2:
                return ArenaType::Arena2v2;
            case BattlegroundType::Arena3v3:
            case BattlegroundType::ArenaSkirmish3v3:
                return ArenaType::Arena3v3;
            case BattlegroundType::Arena5v5:
                return ArenaType::Arena5v5;
            default:
                return 0;
            }
        }
    }
}
struct GroupQueueInfo;                                      // type predefinition
struct PlayerQueueInfo                                      // stores information for players in queue
{
    /// Internal structure.
    struct Pair
    {
        uint32 LastOnlineTime;      /// For tracking and removing offline players from queue after 5 minutes.
        GroupQueueInfo* GroupInfo;  /// Information of the group currently queued.
    };

    std::list<Pair> Infos;
};

struct GroupQueueInfo                                       // stores information about the group in queue (also used when joined as solo!)
{
    std::map<uint64, PlayerQueueInfo*> m_Players;             // player queue info map
    uint32  m_Team;                                           // Player team (ALLIANCE/HORDE)
    MS::Battlegrounds::BattlegroundType::Type m_BgTypeId;                            // battleground type id
    bool    m_IsRatedBG;                                      // rated battleground
    bool    m_IsSkirmish;                                     // skirmish arena
    bool    m_IsRandom;
    ArenaType   m_ArenaType;                                  // 2v2, 3v3, 5v5 or 0 when BG
    uint32  m_JoinTime;                                       // time when group was added
    uint32  m_RemoveInviteTime;                               // time when we will remove invite for players in group
    uint32  m_IsInvitedToBGInstanceGUID;                      // was invited to certain BG
    uint32  m_ArenaTeamRating;                                // if rated match, inited to the rating of the team
    uint32  m_ArenaMatchmakerRating;                          // if rated match, inited to the rating of the team
    uint32  m_OpponentsTeamRating;                            // for rated arena matches
    uint32  m_OpponentsMatchmakerRating;                      // for rated arena matches
    Group* m_Group;
    uint64 m_WantedBGs;
    uint32 m_BracketId;

    uint32 GetTeam() const
    {
        return m_Team == ALLIANCE ? TEAM_ALLIANCE : TEAM_HORDE;
    }
};

/// - See Script_GetBattlefieldStatus clientside (queueType)
/// - Last update 6.0.3 19116
enum class BattlegroundQueueType : uint8
{
    Battleground = 0,
    Arena = 1,
    WorldPvP = 2,
    WarGame = 3,
    Cheat = 4,
    ArenaSkirmish = 5
};

enum BattlegroundSounds
{
    SOUND_HORDE_WINS                = 8454,
    SOUND_ALLIANCE_WINS             = 8455,
    SOUND_BG_START                  = 3439,
    SOUND_BG_START_L70ETC           = 11803,
};

enum BattlegroundQuests
{
    SPELL_WS_QUEST_REWARD           = 43483,
    SPELL_AB_QUEST_REWARD           = 43484,
    SPELL_AV_QUEST_REWARD           = 43475,
    SPELL_AV_QUEST_KILLED_BOSS      = 23658,
    SPELL_EY_QUEST_REWARD           = 43477,
    SPELL_SA_QUEST_REWARD           = 61213,
    SPELL_AB_QUEST_REWARD_4_BASES   = 24061,
    SPELL_AB_QUEST_REWARD_5_BASES   = 24064
};

enum BattlegroundMarks
{
    SPELL_WS_MARK_LOSER             = 24950,
    SPELL_WS_MARK_WINNER            = 24951,
    SPELL_AB_MARK_LOSER             = 24952,
    SPELL_AB_MARK_WINNER            = 24953,
    SPELL_AV_MARK_LOSER             = 24954,
    SPELL_AV_MARK_WINNER            = 24955,
    SPELL_SA_MARK_WINNER            = 61160,
    SPELL_SA_MARK_LOSER             = 61159,
    ITEM_AV_MARK_OF_HONOR           = 20560,
    ITEM_WS_MARK_OF_HONOR           = 20558,
    ITEM_AB_MARK_OF_HONOR           = 20559,
    ITEM_EY_MARK_OF_HONOR           = 29024,
    ITEM_SA_MARK_OF_HONOR           = 42425
};

enum BattlegroundMarksCount
{
    ITEM_WINNER_COUNT               = 3,
    ITEM_LOSER_COUNT                = 1
};

enum BattlegroundCreatures
{
    BG_CREATURE_ENTRY_A_SPIRITGUIDE      = 13116,           // alliance
    BG_CREATURE_ENTRY_H_SPIRITGUIDE      = 13117,           // horde
};

enum BattlegroundSpells
{
    SPELL_WAITING_FOR_RESURRECT     = 2584,                 // Waiting to Resurrect
    SPELL_SPIRIT_HEAL_CHANNEL       = 22011,                // Spirit Heal Channel
    SPELL_SPIRIT_HEAL               = 22012,                // Spirit Heal
    SPELL_RESURRECTION_VISUAL       = 24171,                // Resurrection Impact Visual
    SPELL_ARENA_PREPARATION         = 32727,                // use this one, 32728 not correct
    SPELL_ALLIANCE_GOLD_FLAG        = 32724,
    SPELL_ALLIANCE_GREEN_FLAG       = 32725,
    SPELL_HORDE_GOLD_FLAG           = 35774,
    SPELL_HORDE_GREEN_FLAG          = 35775,
    SPELL_PREPARATION               = 44521,                // Preparation
    SPELL_SPIRIT_HEAL_MANA          = 44535,                // Spirit Heal
    SPELL_RECENTLY_DROPPED_FLAG     = 42792,                // Recently Dropped Flag
    SPELL_AURA_PLAYER_INACTIVE      = 43681,                // Inactive
    SPELL_HONORABLE_DEFENDER_25Y    = 68652,                // +50% honor when standing at a capture point that you control, 25yards radius (added in 3.2)
    SPELL_HONORABLE_DEFENDER_60Y    = 66157,                // +50% honor when standing at a capture point that you control, 60yards radius (added in 3.2), probably for 40+ player battlegrounds
    SPELL_THE_LAST_STANDING         = 26549,                // Arena achievement related
    SPELL_PET_SUMMONED              = 6962,
};

enum BattlegroundTimeIntervals
{
    CHECK_PLAYER_POSITION_INVERVAL  = 1000,                 // ms
    RESURRECTION_INTERVAL           = 30000,                // ms
    //REMIND_INTERVAL                 = 10000,                // ms
    INVITATION_REMIND_TIME          = 20000,                // ms
#ifndef CROSS
    INVITE_ACCEPT_WAIT_TIME         = 30000,                // ms
#else /* CROSS */
    INVITE_ACCEPT_WAIT_TIME         = 90000,                // ms
#endif /* CROSS */
    TIME_AUTOCLOSE_BATTLEGROUND     = 120000,               // ms
    MAX_OFFLINE_TIME                = 300,                  // secs
    RESPAWN_ONE_DAY                 = 86400,                // secs
    RESPAWN_IMMEDIATELY             = 0,                    // secs
    BUFF_RESPAWN_TIME               = 180,                  // secs
    BATTLEGROUND_COUNTDOWN_MAX      = 120000,               // ms
    ARENA_COUNTDOWN_MAX             = 60000                 // ms
};

enum BattlegroundStartTimeIntervals
{
    BG_START_DELAY_2M               = 120000,               // ms (2 minutes)
    BG_START_DELAY_1M               = 60000,                // ms (1 minute)
    BG_START_DELAY_30S              = 30000,                // ms (30 seconds)
    BG_START_DELAY_15S              = 15000,                // ms (15 seconds) Used only in arena
    BG_START_DELAY_NONE             = 0,                    // ms
};

enum BattlegroundBuffObjects
{
    BG_OBJECTID_SPEEDBUFF_ENTRY     = 179871,
    BG_OBJECTID_REGENBUFF_ENTRY     = 179904,
    BG_OBJECTID_BERSERKERBUFF_ENTRY = 179905
};

/// Battleground currency rewards. Should be with precision mod.
enum BattlegroundRandomRewards
{
    BG_REWARD_WINNER_HONOR_FIRST    = 30000,
    BG_REWARD_WINNER_HONOR_LAST     = 15000,
    BG_REWARD_WINNER_CONQUEST_FIRST = 15000,
    BG_REWARD_WINNER_CONQUEST_LAST  = 7500,
    BG_REWARD_LOSER_HONOR_FIRST     = 4500,
    BG_REWARD_LOSER_HONOR_LAST      = 3500
};

namespace ArenaSkirmishRewards
{
    enum
    {
        ConquestPointsWinner            = 2500,
        HonorPointsWinnerBase           = 3600,
        HonorPointsWinnerBonusPerMinute = 900,
        HonorPointLoser                 = 3500
    };
}

const uint32 Buff_Entries[3] = { BG_OBJECTID_SPEEDBUFF_ENTRY, BG_OBJECTID_REGENBUFF_ENTRY, BG_OBJECTID_BERSERKERBUFF_ENTRY };

enum BattlegroundStatus
{
    STATUS_NONE         = 0,                                // first status, should mean bg is not instance
    STATUS_WAIT_QUEUE   = 1,                                // means bg is empty and waiting for queue
    STATUS_WAIT_JOIN    = 2,                                // this means, that BG has already started and it is waiting for more players
    STATUS_IN_PROGRESS  = 3,                                // means bg is running
    STATUS_WAIT_LEAVE   = 4                                 // means some faction has won BG and it is ending
};

/// - See CGBattlefieldInfo::s_flagTokens
enum class FlagIcon : uint8
{
    None      = 0,              ///< ""
    Horde     = 1,              ///< "HordeFlag"
    Alliance  = 2               ///< "AllianceFlag"
};

enum BattlegroundAward
{
    AWARD_GOLD,
    AWARD_SILVER,
    AWARD_BRONZE,
    AWARD_NONE
};

enum PvPAwardSpells
{
    PVP_AWARD_SPELL_GOLDEN_STRONBOX     = 155306,
    PVP_AWARD_SPELL_SILVER_STRONGBOX    = 177453,
    PVP_AWARD_SPELL_BRONZE_STRONGBOX    = 155307,
    PVP_AWARD_SPELL_SKIRMISH_WIN        = 177346,
};

struct BattlegroundPlayer
{
    time_t  OfflineRemoveTime;                              // for tracking and removing offline players from queue after 5 minutes
    uint32  Team;                                           // Player's team
};

struct BattlegroundObjectInfo
{
    BattlegroundObjectInfo() : object(NULL), timer(0), spellid(0) {}

    GameObject  *object;
    int32       timer;
    uint32      spellid;
};


/// - See Blizzard_PVPUI.lua, ARENA_DATA array
/// - Last update : 6.2.3 20779
enum class SkirmishTypeId : uint8
{
    Skrimish2v2 = 4,
    Skrimish3v3 = 5
};

#define SKIRMISH_MAX 2

const SkirmishTypeId g_SkirmishTypes[SKIRMISH_MAX] = { SkirmishTypeId::Skrimish2v2, SkirmishTypeId::Skrimish3v3 };

enum ScoreType
{
    SCORE_KILLING_BLOWS         = 1,
    SCORE_DEATHS                = 2,
    SCORE_HONORABLE_KILLS       = 3,
    SCORE_BONUS_HONOR           = 4,
    // EY, but in MSG_PVP_LOG_DATA opcode!
    SCORE_DAMAGE_DONE           = 5,
    SCORE_HEALING_DONE          = 6,
    // WS
    SCORE_FLAG_CAPTURES         = 7,
    SCORE_FLAG_RETURNS          = 8,
    // AB and IC
    SCORE_BASES_ASSAULTED       = 9,
    SCORE_BASES_DEFENDED        = 10,
    // AV
    SCORE_GRAVEYARDS_ASSAULTED  = 11,
    SCORE_GRAVEYARDS_DEFENDED   = 12,
    SCORE_TOWERS_ASSAULTED      = 13,
    SCORE_TOWERS_DEFENDED       = 14,
    SCORE_MINES_CAPTURED        = 15,
    SCORE_LEADERS_KILLED        = 16,
    SCORE_SECONDARY_OBJECTIVES  = 17,
    // SOTA
    SCORE_DESTROYED_DEMOLISHER  = 18,
    SCORE_DESTROYED_WALL        = 19,
    // SM
    SCORE_CARTS_HELPED          = 20,
    // TK
    SCORE_ORB_HANDLES           = 21,
    SCORE_ORB_SCORE             = 22,
    // DG
    SCORE_CART_CAPTURES         = 21,
    SCORE_CART_RETURNS          = 22,
    // SM
    SCORE_CART_CONTROLLED       = 23
};

enum BattlegroundType
{
    TYPE_BATTLEGROUND     = 3,
    TYPE_ARENA            = 4
};

enum BattlegroundWinner
{
    WINNER_HORDE            = 0,
    WINNER_ALLIANCE         = 1,
    WINNER_NONE             = 2
};

enum BattlegroundTeamId
{
    BG_TEAM_ALLIANCE        = 0,
    BG_TEAM_HORDE           = 1
};

enum BattlegroundStartingEvents
{
    BG_STARTING_EVENT_NONE  = 0x00,
    BG_STARTING_EVENT_1     = 0x01,
    BG_STARTING_EVENT_2     = 0x02,
    BG_STARTING_EVENT_3     = 0x04,
    BG_STARTING_EVENT_4     = 0x08
};

enum BattlegroundStartingEventsIds
{
    BG_STARTING_EVENT_FIRST     = 0,
    BG_STARTING_EVENT_SECOND    = 1,
    BG_STARTING_EVENT_THIRD     = 2,
    BG_STARTING_EVENT_FOURTH    = 3
};
#define BG_STARTING_EVENT_COUNT 4

enum GroupJoinBattlegroundResult
{
    ERR_BATTLEGROUND_NONE                           = 0,
    ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS           = 2,        // You cannot join the battleground yet because you or one of your party members is flagged as a Deserter.
    ERR_ARENA_TEAM_PARTY_SIZE                       = 3,        // Incorrect party size for this arena.
    ERR_BATTLEGROUND_TOO_MANY_QUEUES                = 4,        // You can only be queued for 2 battles at once
    ERR_BATTLEGROUND_CANNOT_QUEUE_FOR_RATED         = 5,        // You cannot queue for a rated match while queued for other battles
    ERR_BATTLEDGROUND_QUEUED_FOR_RATED              = 6,        // You cannot queue for another battle while queued for a rated arena match
    ERR_BATTLEGROUND_TEAM_LEFT_QUEUE                = 7,        // Your team has left the arena queue
    ERR_BATTLEGROUND_NOT_IN_BATTLEGROUND            = 8,        // You can't do that in a battleground.
    ERR_BATTLEGROUND_JOIN_XP_GAIN                   = 9,        // wtf, doesn't exist in client...
    ERR_BATTLEGROUND_JOIN_RANGE_INDEX               = 10,       // Cannot join the queue unless all members of your party are in the same battleground level range.
    ERR_BATTLEGROUND_JOIN_TIMED_OUT                 = 11,       // %s was unavailable to join the queue. (uint64 guid exist in client cache)
    //ERR_BATTLEGROUND_JOIN_TIMED_OUT               = 12,       // same as 11
    //ERR_BATTLEGROUND_TEAM_LEFT_QUEUE              = 13,       // same as 7
    ERR_LFG_CANT_USE_BATTLEGROUND                   = 14,       // You cannot queue for a battleground or arena while using the dungeon system.
    ERR_IN_RANDOM_BG                                = 15,       // Can't do that while in a Random Battleground queue.
    ERR_IN_NON_RANDOM_BG                            = 16,       // Can't queue for Random Battleground while in another Battleground queue.
    ERR_BG_DEVELOPER_ONLY                           = 17,
    ERR_BATTLEGROUND_INVITATION_DECLINED            = 18,
    ERR_MEETING_STONE_NOT_FOUND                     = 19,
    ERR_WARGAME_REQUEST_FAILURE                     = 20,
    ERR_BATTLEFIELD_TEAM_PARTY_SIZE                 = 22,
    ERR_NOT_ON_TOURNAMENT_REALM                     = 23,
    ERR_BATTLEGROUND_PLAYERS_FROM_DIFFERENT_REALMS  = 24,
    ERR_REMOVE_FROM_PVP_QUEUE_GRANT_LEVEL           = 33,
    ERR_REMOVE_FROM_PVP_QUEUE_FACTION_CHANGE        = 34,
    ERR_BATTLEGROUND_JOIN_FAILED                    = 35,
    ERR_BATTLEGROUND_DUPE_QUEUE                     = 43
};

class BattlegroundScore
{
    public:
        BattlegroundScore() : KillingBlows(0), Deaths(0), HonorableKills(0),
            BonusHonor(0), DamageDone(0), HealingDone(0), RatingChange(0)
        {}
        virtual ~BattlegroundScore() {}                     //virtual destructor is used when deleting score from scores map

        uint32 KillingBlows;
        uint32 Deaths;
        uint32 HonorableKills;
        uint32 BonusHonor;
        uint32 DamageDone;
        uint32 HealingDone;
        int32 RatingChange;
};

enum BGHonorMode
{
    BG_NORMAL = 0,
    BG_HOLIDAY,
    BG_HONOR_MODE_NUM
};

#define BG_AWARD_ARENA_POINTS_MIN_LEVEL 71
#define ARENA_TIMELIMIT_POINTS_LOSS    -12

/*
This class is used to:
1. Add player to battleground
2. Remove player from battleground
3. some certain cases, same for all battlegrounds
4. It has properties same for all battlegrounds
*/
class Battleground
{
    public:
        Battleground();
        virtual ~Battleground();

        void Update(uint32 diff);

        virtual bool SetupBattleground()                    // must be implemented in BG subclass
        {
            return true;
        }
        virtual void Reset();                               // resets all common properties for battlegrounds, must be implemented and called in BG subclass
        virtual void StartingEventCloseDoors() {}
        virtual void StartingEventOpenDoors() {}
        virtual void ResetBGSubclass()                      // must be implemented in BG subclass
        {
        }

        virtual void DestroyGate(Player* /*player*/, GameObject* /*go*/) {}

        /* achievement req. */
        virtual bool IsAllNodesControlledByTeam(uint32 /*team*/) const { return false; }
        bool IsTeamScoreInRange(uint32 team, uint32 minScore, uint32 maxScore) const;
        void StartTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry);

        /* Battleground */
        // Get methods:
        char const* GetName() const         { return m_Name; }
        uint64 GetGUID() const { return m_Guid; }
        BattlegroundTypeId GetTypeID(bool GetRandom = false) const { return GetRandom ? m_RandomTypeID : m_TypeID; }
        MS::Battlegrounds::Bracket::Id GetBracketId() const { return m_BracketId; }
        uint32 GetInstanceID() const        { return m_InstanceID; }
        BattlegroundStatus GetStatus() const { return m_Status; }
        uint32 GetClientInstanceID() const  { return m_ClientInstanceID; }
        uint32 GetElapsedTime() const       { return m_StartTime; }
        uint32 GetRemainingTime() const     { return m_EndTime; }
        uint32 GetExpirationDate() const    { return 0; } ///< Handled differently.
        uint32 GetLastResurrectTime() const { return m_LastResurrectTime; }
        uint32 GetMaxPlayers() const        { return m_MaxPlayers; }
        uint32 GetMinPlayers() const        { return m_MinPlayers; }

        uint32 GetMinLevel() const          { return m_LevelMin; }
        uint32 GetMaxLevel() const          { return m_LevelMax; }

        uint32 GetMaxPlayersPerTeam() const { return m_MaxPlayersPerTeam; }
        uint32 GetMinPlayersPerTeam() const { return m_MinPlayersPerTeam; }

        int32 GetStartDelayTime() const     { return m_StartDelayTime; }
        uint8 GetArenaType() const          { return m_ArenaType; }
        uint8 GetWinner() const             { return m_Winner; }
        uint32 GetHolidayId() const         { return m_holiday; }
        uint32 GetScriptId() const          { return ScriptId; }
        bool IsRandom() const               { return m_IsRandom; }
        bool IsRatedBG() const              { return m_IsRatedBg; }
        bool IsSkirmish() const             { return m_IsSkirmish; }
        bool IsWargame() const              { return m_IsWargame; }
        bool UseTournamentRules() const     { return m_UseTournamentRules; }

        bool CanGroupEnter(GroupQueueInfo const* p_Group) const
        {
            return GetStatus() <= BattlegroundStatus::STATUS_IN_PROGRESS ///< BG isn't finished
                && static_cast<int>(GetMaxPlayersPerTeam() - GetInvitedCount(p_Group->m_Team) - m_PlayersCount[p_Group->GetTeam()] - p_Group->m_Players.size()) >= 0;
        }

        uint32 GetBonusHonorFromKill(uint32 kills) const;

        // Set methods:
        void SetName(char const* Name)      { m_Name = Name; }
        void SetTypeID(BattlegroundTypeId TypeID) { m_TypeID = TypeID; }
        void InitGUID();
        void SetRandomTypeID(BattlegroundTypeId TypeID) { m_RandomTypeID = TypeID; }
        //here we can count minlevel and maxlevel for players
        void SetBracket(MS::Battlegrounds::Bracket const* bracketEntry);
        void SetInstanceID(uint32 InstanceID) { m_InstanceID = InstanceID; }
        void SetStatus(BattlegroundStatus Status) { m_Status = Status; }
        void SetClientInstanceID(uint32 InstanceID) { m_ClientInstanceID = InstanceID; }
        void SetElapsedTime(uint32 Time)      { m_StartTime = Time; }
        void SetRemainingTime(uint32 Time)        { m_EndTime = Time; }
        void SetLastResurrectTime(uint32 Time) { m_LastResurrectTime = Time; }
        void SetMaxPlayers(uint32 MaxPlayers) { m_MaxPlayers = MaxPlayers; }
        void SetMinPlayers(uint32 MinPlayers) { m_MinPlayers = MinPlayers; }
        void SetLevelRange(uint32 min, uint32 max) { m_LevelMin = min; m_LevelMax = max; }
        void SetArenaType(uint8 type)       { m_ArenaType = type; }
        void SetArenaorBGType(bool _isArena) { m_IsArena = _isArena; }
        void SetWinner(uint8 winner)        { m_Winner = winner; }
        void SetHolidayId(uint8 holiday)        { m_holiday = holiday; }
        void SetScriptId(uint32 scriptId)   { ScriptId = scriptId; }

        void ResetCountdownTimer() { m_CountdownTimer = m_IsArena ? ARENA_COUNTDOWN_MAX : BATTLEGROUND_COUNTDOWN_MAX; }
        void SetCountdownTimer(int Time) { m_CountdownTimer = Time; }
        void ModifyCountdownTimer(int diff) { m_CountdownTimer -= diff; }
        int32 GetMaxCountdownTimer() { return m_IsArena ? ARENA_COUNTDOWN_MAX / 1000 : BATTLEGROUND_COUNTDOWN_MAX / 1000; }

        void ModifyStartDelayTime(int diff) { m_StartDelayTime -= diff; }
        void SetStartDelayTime(int Time)    { m_StartDelayTime = Time; }

        void SetMaxPlayersPerTeam(uint32 MaxPlayers) { m_MaxPlayersPerTeam = MaxPlayers; }
        void SetMinPlayersPerTeam(uint32 MinPlayers) { m_MinPlayersPerTeam = MinPlayers; }

        void DecreaseInvitedCount(uint32 team)      { (team == ALLIANCE) ? --m_InvitedAlliance : --m_InvitedHorde; }
        void IncreaseInvitedCount(uint32 team)      { (team == ALLIANCE) ? ++m_InvitedAlliance : ++m_InvitedHorde; }

        void SetRandom(bool isRandom)       { m_IsRandom = isRandom; }
        void SetRatedBG(bool isRatedBg)     { m_IsRatedBg = isRatedBg; }
        void SetSkirmish(bool p_IsSkirmish) { m_IsSkirmish = p_IsSkirmish; }
        void SetWargame(bool p_IsWargame)   { m_IsWargame = p_IsWargame; }
        void EnableTournamentRules()        { m_UseTournamentRules = true; }

        uint32 GetInvitedCount(uint32 team) const   { return (team == ALLIANCE) ? m_InvitedAlliance : m_InvitedHorde; }
        bool HasFreeSlots() const;

        bool isArena() const        { return m_IsArena; }
        bool isBattleground() const { return !m_IsArena; }

        typedef std::map<uint64, BattlegroundPlayer> BattlegroundPlayerMap;
        BattlegroundPlayerMap const& GetPlayers() const { return m_Players; }
        uint32 GetPlayersSize() const { return m_Players.size(); }

        typedef std::map<uint64, BattlegroundScore*> BattlegroundScoreMap;
        BattlegroundScoreMap::const_iterator GetPlayerScoresBegin() const { return PlayerScores.begin(); }
        BattlegroundScoreMap::const_iterator GetPlayerScoresEnd() const { return PlayerScores.end(); }
        uint32 GetPlayerScoresSize() const { return PlayerScores.size(); }

        uint32 GetReviveQueueSize() const { return m_ReviveQueue.size(); }

        void AddPlayerToResurrectQueue(uint64 npc_guid, uint64 player_guid);
        void RemovePlayerFromResurrectQueue(uint64 player_guid);

        /// Relocate all players in ReviveQueue to the closest graveyard
        void RelocateDeadPlayers(uint64 queueIndex);

        void StartBattleground();

        GameObject* GetBGObject(uint32 type);
        Creature* GetBGCreature(uint32 type);

        // Location
        void SetMapId(uint32 MapID) { m_MapId = MapID; }
        uint32 GetMapId() const { return m_MapId; }

        // Map pointers
        void SetBgMap(BattlegroundMap* map) { m_Map = map; }
        BattlegroundMap* GetBgMap() const { return m_Map; }
        BattlegroundMap* FindBgMap() const { return m_Map; }

        void SetTeamStartLoc(uint32 TeamID, float X, float Y, float Z, float O);
        virtual void GetTeamStartLoc(uint32 TeamID, float &X, float &Y, float &Z, float &O) const
        {
            BattlegroundTeamId idx = GetTeamIndexByTeamId(TeamID);
            X = m_TeamStartLocX[idx];
            Y = m_TeamStartLocY[idx];
            Z = m_TeamStartLocZ[idx];
            O = m_TeamStartLocO[idx];
        }
        void SetStartMaxDist(float startMaxDist) { m_StartMaxDist = startMaxDist; }
        float GetStartMaxDist() const { return m_StartMaxDist; }

        // Packet Transfer
        // method that should fill worldpacket with actual world states (not yet implemented for all battlegrounds!)
        virtual void FillInitialWorldStates(ByteBuffer& /*data*/) {}
        void SendPacketToTeam(uint32 TeamID, WorldPacket* packet, Player* sender = NULL, bool self = true);
        void SendPacketToAll(WorldPacket* packet);
        void YellToAll(Creature* creature, const char* text, uint32 language);

        template<class Do>
        void BroadcastWorker(Do& _do);

        void PlaySoundToTeam(uint32 SoundID, uint32 TeamID);
        void PlaySoundToAll(uint32 SoundID);
        void CastSpellOnTeam(uint32 SpellID, uint32 TeamID);
        void RemoveAuraOnTeam(uint32 SpellID, uint32 TeamID);
        void RewardHonorToTeam(uint32 Honor, uint32 TeamID, MS::Battlegrounds::RewardCurrencyType::Type p_RewardCurrencyType = MS::Battlegrounds::RewardCurrencyType::Type::None);
        void RewardReputationToTeam(uint32 faction_id, uint32 Reputation, uint32 TeamID);
        void UpdateWorldState(uint32 Field, uint32 Value);
        void UpdateWorldStateForPlayer(uint32 Field, uint32 Value, Player* Source);
        void EndBattleground(uint32 winner);
        void BlockMovement(Player* player);

        void SendWarningToAll(int32 entry, ...);
        void SendMessageToAll(int32 entry, ChatMsg type, Player const* source = NULL);
        void PSendMessageToAll(int32 entry, ChatMsg type, Player const* source, ...);
        void SendCountdownTimer();

        void SendFlagsPositions();

        // specialized version with 2 string id args
        void SendMessage2ToAll(int32 entry, ChatMsg type, Player const* source, int32 strId1 = 0, int32 strId2 = 0);

        // Raid Group
        Group* GetBgRaid(uint32 TeamID) const { return TeamID == ALLIANCE ? m_BgRaids[BG_TEAM_ALLIANCE] : m_BgRaids[BG_TEAM_HORDE]; }
        void SetBgRaid(uint32 TeamID, Group* bg_raid);

        virtual void UpdatePlayerScore(Player* p_Source, Player* p_Victim, uint32 p_Type, uint32 p_Value, bool p_DoAddHonor = true, MS::Battlegrounds::RewardCurrencyType::Type p_RewardType = MS::Battlegrounds::RewardCurrencyType::Type::None);

        static BattlegroundTeamId GetTeamIndexByTeamId(uint32 Team) { return Team == ALLIANCE ? BG_TEAM_ALLIANCE : BG_TEAM_HORDE; }
        uint32 GetPlayersCountByTeam(uint32 Team) const { return m_PlayersCount[GetTeamIndexByTeamId(Team)]; }
        uint32 GetAlivePlayersCountByTeam(uint32 Team) const;   // used in arenas to correctly handle death in spirit of redemption / last stand etc. (killer = killed) cases
        void UpdatePlayersCountByTeam(uint32 Team, bool remove)
        {
            if (remove)
                --m_PlayersCount[GetTeamIndexByTeamId(Team)];
            else
                ++m_PlayersCount[GetTeamIndexByTeamId(Team)];
        }

        // used for rated arena battles
        int32 GetArenaTeamRatingChangeByIndex(uint32 index) const
        {
            if (index >= MS::Battlegrounds::TeamsCount::Value)
                return 0;

            return m_ArenaTeamRatingChanges[index];
        }

        uint32 GetArenaMatchmakerRatingByIndex(uint32 index) const
        {
            if (index >= MS::Battlegrounds::TeamsCount::Value)
                return 0;

            return m_ArenaTeamMMR[index];
        }

        void SetArenaMatchmakerRating(uint32 Team, uint32 MMR){ m_ArenaTeamMMR[GetTeamIndexByTeamId(Team)] = MMR; }
        void SetArenaTeamRatingChangeForTeam(uint32 Team, int32 RatingChange) { m_ArenaTeamRatingChanges[GetTeamIndexByTeamId(Team)] = RatingChange; }

        uint32 GetArenaMatchmakerRating(uint32 Team, uint8 slot);
        void CheckArenaAfterTimerConditions();
        void CheckArenaWinConditions();
        void UpdateArenaWorldState();

        // Triggers handle
        // must be implemented in BG subclass
        virtual void HandleAreaTrigger(Player* /*Source*/, uint32 /*Trigger*/) {}
        // must be implemented in BG subclass if need AND call base class generic code
        virtual void HandleKillPlayer(Player* player, Player* killer);
        virtual void HandleKillUnit(Creature* /*unit*/, Player* /*killer*/);

        // Battleground events
        virtual void EventPlayerDroppedFlag(Player* /*player*/) {}
        virtual void EventPlayerClickedOnFlag(Player* /*player*/, GameObject* /*target_obj*/) {}
        virtual void EventPlayerClickedOnFlag(Player* /*player*/, Unit* /*target_unit*/) {}
        void EventPlayerLoggedIn(Player* player);
        void EventPlayerLoggedOut(Player* player);
        virtual void EventPlayerDamagedGO(Player* /*player*/, GameObject* /*go*/, uint32 /*eventType*/) {}
        virtual void EventPlayerUsedGO(Player* /*player*/, GameObject* /*go*/){}

        // this function can be used by spell to interact with the BG map
        virtual void DoAction(uint32 /*action*/, uint64 /*var*/) {}

        virtual void HandlePlayerResurrect(Player* /*player*/) {}

        // Death related
        virtual WorldSafeLocsEntry const* GetClosestGraveYard(Player* player);

        virtual void AddPlayer(Player* player);                // must be implemented in BG subclass

        void AddOrSetPlayerToCorrectBgGroup(Player* player, uint32 team);

#ifndef CROSS
        virtual void RemovePlayerAtLeave(uint64 guid, bool Transport, bool SendPacket);
#else /* CROSS */
        virtual uint32 GetZoneId() const { return 0; }

        virtual void RemovePlayerAtLeave(uint64 guid);
#endif /* CROSS */
                                                            // can be extended in in BG subclass

        void HandleTriggerBuff(uint64 go_guid);
        void SetHoliday(bool is_holiday);

        // TODO: make this protected:
        typedef std::vector<uint64> BGObjects;
        typedef std::vector<uint64> BGCreatures;
        BGObjects BgObjects;
        BGCreatures BgCreatures;
        void SpawnBGObject(uint32 type, uint32 respawntime);
        bool AddObject(uint32 type, uint32 entry, float x, float y, float z, float o, float rotation0, float rotation1, float rotation2, float rotation3, uint32 respawnTime = 0);
        Creature* AddCreature(uint32 entry, uint32 type, uint32 teamval, float x, float y, float z, float o, uint32 respawntime = 0);
        bool DelCreature(uint32 type);
        bool DelObject(uint32 type);
        bool AddSpiritGuide(uint32 type, float x, float y, float z, float o, uint32 team);
        int32 GetObjectType(uint64 guid);

        void DoorOpen(uint32 type);
        void DoorClose(uint32 type);
        //to be removed
        const char* GetTrinityString(int32 entry);

        virtual bool HandlePlayerUnderMap(Player* /*player*/) { return false; }

        // since arenas can be AvA or Hvh, we have to get the "temporary" team of a player
        uint32 GetPlayerTeam(uint64 guid) const;
        uint32 GetOtherTeam(uint32 teamId) const;
        bool IsPlayerInBattleground(uint64 guid) const;

        bool ToBeDeleted() const { return m_SetDeleteThis; }
        void SetDeleteThis() { m_SetDeleteThis = true; }

        // virtual score-array - get's used in bg-subclasses
        int32 m_TeamScores[MS::Battlegrounds::TeamsCount::Value];

        void RewardXPAtKill(Player* killer, Player* victim);
        bool CanAwardArenaPoints() const { return m_LevelMin >= BG_AWARD_ARENA_POINTS_MIN_LEVEL; }

        virtual std::set<uint64> const GetFlagPickersGUID(int32 /*team*/ = -1) const { return std::set<uint64>(); }

        /// - Debug only
        void FastStart() { m_StartDelayTime = 0; }
#ifdef CROSS

        void RemoveFromInterRealm(uint64 playerGuid);
#endif /* CROSS */

        void AwardTeams(uint32 p_Winner);
        void AwardTeamsWithRewards(BattlegroundAward p_LooserAward, BattlegroundAward p_WinnerAward, uint32 p_Winner);
        static uint32 GetSpellIdForAward(BattlegroundAward p_Award);
    protected:
        void BuildArenaOpponentSpecializations(WorldPacket* data, uint32 team);

        // this method is called, when BG cannot spawn its own spirit guide, or something is wrong, It correctly ends Battleground
        void EndNow();
        void PlayerAddedToBGCheckIfBGIsRunning(Player* player);

        Player* _GetPlayer(uint64 guid, bool offlineRemove, const char* context) const;
        Player* _GetPlayer(BattlegroundPlayerMap::iterator itr, const char* context);
        Player* _GetPlayer(BattlegroundPlayerMap::const_iterator itr, const char* context) const;
        Player* _GetPlayerForTeam(uint32 teamId, BattlegroundPlayerMap::const_iterator itr, const char* context) const;

        void _ProcessOfflineQueue();
        void _ProcessRessurect(uint32 diff);
        void _ProcessProgress(uint32 diff);
        void _ProcessLeave(uint32 diff);
        void _ProcessJoin(uint32 diff);
        void _CheckSafePositions(uint32 diff);

        // Scorekeeping
        BattlegroundScoreMap PlayerScores;                // Player scores
        // must be implemented in BG subclass
        virtual void RemovePlayer(Player* player, uint64 guid, uint32 team)
        {
            UNUSED(player);
            UNUSED(guid);
            UNUSED(team);
        }

        // Player lists, those need to be accessible by inherited classes
        BattlegroundPlayerMap  m_Players;
        // Spirit Guide guid + Player list GUIDS
        std::map<uint64, std::vector<uint64> >  m_ReviveQueue;

        // these are important variables used for starting messages
        uint8 m_Events;
        BattlegroundStartTimeIntervals  StartDelayTimes[BG_STARTING_EVENT_COUNT];
        // this must be filled in constructors!
        uint32 StartMessageIds[BG_STARTING_EVENT_COUNT];

        bool   m_BuffChange;
        bool   m_IsRandom;
        bool   m_IsRatedBg;
        bool   m_IsWargame;
        bool   m_UseTournamentRules;

        BGHonorMode m_HonorMode;

        virtual uint32 GetMaxScore() const { return 0; }
        virtual uint32 GetTeamScore(uint32 /*p_Team*/) const { return 0; }
        virtual bool IsScoreIncremental() const { return true; }

        void ApplyDampeningIfNeeded();
    private:
        // Battleground
        BattlegroundTypeId m_TypeID;
        BattlegroundTypeId m_RandomTypeID;
        uint32 m_InstanceID;                                // Battleground Instance's GUID!
        BattlegroundStatus m_Status;
        uint32 m_ClientInstanceID;                          // the instance-id which is sent to the client and without any other internal use
        uint32 m_StartTime;
        uint32 m_CountdownTimer;
        uint32 m_ResetStatTimer;
        uint32 m_ValidStartPositionTimer;
        int32 m_EndTime;                                    // it is set to 120000 when bg is ending and it decreases itself
        int32 m_CreationTime;
        uint32 m_LastResurrectTime;
        MS::Battlegrounds::Bracket::Id m_BracketId;
        uint8  m_ArenaType;                                 // 2=2v2, 3=3v3, 5=5v5
        bool   m_InBGFreeSlotQueue;                         // used to make sure that BG is only once inserted into the BattlegroundMgr.BGFreeSlotQueue[bgTypeId] deque
        bool   m_SetDeleteThis;                             // used for safe deletion of the bg after end / all players leave
        bool   m_IsArena;
        bool   m_IsSkirmish;
        uint8  m_Winner;                                    // 0=alliance, 1=horde, 2=none
        int32  m_StartDelayTime;
        bool   m_PrematureCountDown;
        uint32 m_PrematureCountDownTimer;
        char const* m_Name;
        uint64 m_Guid;
        uint32 m_PlayersPositionsTimer;

        bool m_CrowdChosed;
        uint32 m_minHealth;
        uint8 m_teamDealMaxDamage;

        /* Pre- and post-update hooks */

        /**
         * @brief Pre-update hook.
         *
         * Will be called before battleground update is started. Depending on
         * the result of this call actual update body may be skipped.
         *
         * @param diff a time difference between two worldserver update loops in
         * milliseconds.
         *
         * @return @c true if update must be performed, @c false otherwise.
         *
         * @see Update(), PostUpdateImpl().
         */
        virtual bool PreUpdateImpl(uint32 /* diff */) { return true; };

        /**
         * @brief Post-update hook.
         *
         * Will be called after battleground update has passed. May be used to
         * implement custom update effects in subclasses.
         *
         * @param diff a time difference between two worldserver update loops in
         * milliseconds.
         *
         * @see Update(), PreUpdateImpl().
         */
        virtual void PostUpdateImpl(uint32 /* diff */) { };

        // Player lists
        std::vector<uint64> m_ResurrectQueue;               // Player GUID
        std::deque<uint64> m_OfflineQueue;                  // Player GUID

        // Invited counters are useful for player invitation to BG - do not allow, if BG is started to one faction to have 2 more players than another faction
        // Invited counters will be changed only when removing already invited player from queue, removing player from battleground and inviting player to BG
        // Invited players counters
        uint32 m_InvitedAlliance;
        uint32 m_InvitedHorde;

        // Raid Group
        Group* m_BgRaids[MS::Battlegrounds::TeamsCount::Value];                   // 0 - alliance, 1 - horde

        // Players count by team
        uint32 m_PlayersCount[MS::Battlegrounds::TeamsCount::Value];

        // Arena team ids by team
        uint32 m_ArenaTeamIds[MS::Battlegrounds::TeamsCount::Value];

        int32 m_ArenaTeamRatingChanges[MS::Battlegrounds::TeamsCount::Value];
        uint32 m_ArenaTeamMMR[MS::Battlegrounds::TeamsCount::Value];

        // Limits
        uint32 m_LevelMin;
        uint32 m_LevelMax;
        uint32 m_MaxPlayersPerTeam;
        uint32 m_MaxPlayers;
        uint32 m_MinPlayersPerTeam;
        uint32 m_MinPlayers;

        // Start location
        uint32 m_MapId;
        BattlegroundMap* m_Map;
        float m_TeamStartLocX[MS::Battlegrounds::TeamsCount::Value];
        float m_TeamStartLocY[MS::Battlegrounds::TeamsCount::Value];
        float m_TeamStartLocZ[MS::Battlegrounds::TeamsCount::Value];
        float m_TeamStartLocO[MS::Battlegrounds::TeamsCount::Value];
        float m_StartMaxDist;
        uint32 m_holiday;
        uint32 ScriptId;
};

// helper functions for world state list fill
inline void FillInitialWorldState(ByteBuffer& data, uint32& count, uint32 state, uint32 value)
{
    data << uint32(state);
    data << uint32(value);
    ++count;
}

inline void FillInitialWorldState(ByteBuffer& data, uint32& count, uint32 state, int32 value)
{
    data << uint32(state);
    data << int32(value);
    ++count;
}

inline void FillInitialWorldState(ByteBuffer& data, uint32& count, uint32 state, bool value)
{
    data << uint32(state);
    data << uint32(value?1:0);
    ++count;
}

struct WorldStatePair
{
    uint32 state;
    uint32 value;
};

inline void FillInitialWorldState(ByteBuffer& data, uint32& count, WorldStatePair const* array)
{
    for (WorldStatePair const* itr = array; itr->state; ++itr)
    {
        data << uint32(itr->state);
        data << uint32(itr->value);
        ++count;
    }
}

#endif

