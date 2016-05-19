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

#ifndef _OBJECTMGR_H
#define _OBJECTMGR_H

#include "Log.h"
#include "Object.h"
#include "Bag.h"
#include "Creature.h"
#include "Player.h"
#include "DynamicObject.h"
#include "GameObject.h"
#include "TemporarySummon.h"
#include "Corpse.h"
#include "QuestDef.h"
#include "ItemPrototype.h"
#include "NPCHandler.h"
#include "DatabaseEnv.h"
#include "Mail.h"
#include "Map.h"
#include "ObjectAccessor.h"
#include "ObjectDefines.h"
#include <ace/Singleton.h>
#include "VehicleDefines.h"
#include <string>
#include <map>
#include <limits>
#include "ConditionMgr.h"
#include <functional>
#include "PhaseMgr.h"
#include <LockedMap.h>
#include <ace/Thread_Mutex.h>

class Item;
class PhaseMgr;

// GCC have alternative #pragma pack(N) syntax and old gcc version not support pack(push, N), also any gcc version not support it at some platform
#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

struct PageText
{
    std::string Text;
    uint16 NextPage;
};

// GCC have alternative #pragma pack() syntax and old gcc version not support pack(pop), also any gcc version not support it at some platform
#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

// Benchmarked: Faster than UNORDERED_MAP (insert/find)
typedef std::map<uint32, PageText> PageTextContainer;

// Benchmarked: Faster than std::map (insert/find)
typedef UNORDERED_MAP<uint16, InstanceTemplate> InstanceTemplateContainer;

struct GameTele
{
    float  position_x;
    float  position_y;
    float  position_z;
    float  orientation;
    uint32 mapId;
    std::string name;
    std::wstring wnameLow;
};

typedef UNORDERED_MAP<uint32, GameTele > GameTeleContainer;

enum ScriptsType
{
    SCRIPTS_FIRST = 1,

    SCRIPTS_QUEST_END = SCRIPTS_FIRST,
    SCRIPTS_QUEST_START,
    SCRIPTS_SPELL,
    SCRIPTS_GAMEOBJECT,
    SCRIPTS_EVENT,
    SCRIPTS_WAYPOINT,

    SCRIPTS_LAST
};

enum eScriptFlags
{
    // Talk Flags
    SF_TALK_USE_PLAYER          = 0x1,

    // Emote flags
    SF_EMOTE_USE_STATE          = 0x1,

    // TeleportTo flags
    SF_TELEPORT_USE_CREATURE    = 0x1,

    // KillCredit flags
    SF_KILLCREDIT_REWARD_GROUP  = 0x1,

    // RemoveAura flags
    SF_REMOVEAURA_REVERSE       = 0x1,

    // CastSpell flags
    SF_CASTSPELL_SOURCE_TO_TARGET = 0,
    SF_CASTSPELL_SOURCE_TO_SOURCE = 1,
    SF_CASTSPELL_TARGET_TO_TARGET = 2,
    SF_CASTSPELL_TARGET_TO_SOURCE = 3,
    SF_CASTSPELL_SEARCH_CREATURE  = 4,
    SF_CASTSPELL_TRIGGERED      = 0x1,

    // PlaySound flags
    SF_PLAYSOUND_TARGET_PLAYER  = 0x1,
    SF_PLAYSOUND_DISTANCE_SOUND = 0x2,

    // Orientation flags
    SF_ORIENTATION_FACE_TARGET  = 0x1,
};

struct ScriptInfo
{
    ScriptsType type;
    uint32 id;
    uint32 delay;
    ScriptCommands command;

    union
    {
        struct
        {
            uint32 nData[3];
            float  fData[4];
        } Raw;

        struct                      // SCRIPT_COMMAND_TALK (0)
        {
            uint32 ChatType;        // datalong
            uint32 Flags;           // datalong2
            int32  TextID;          // dataint
        } Talk;

        struct                      // SCRIPT_COMMAND_EMOTE (1)
        {
            uint32 EmoteID;         // datalong
            uint32 Flags;           // datalong2
        } Emote;

        struct                      // SCRIPT_COMMAND_FIELD_SET (2)
        {
            uint32 FieldID;         // datalong
            uint32 FieldValue;      // datalong2
        } FieldSet;

        struct                      // SCRIPT_COMMAND_MOVE_TO (3)
        {
            uint32 Unused1;         // datalong
            uint32 TravelTime;      // datalong2
            int32  Unused2;         // dataint

            float DestX;
            float DestY;
            float DestZ;
        } MoveTo;

        struct                      // SCRIPT_COMMAND_FLAG_SET (4)
                                    // SCRIPT_COMMAND_FLAG_REMOVE (5)
        {
            uint32 FieldID;         // datalong
            uint32 FieldValue;      // datalong2
        } FlagToggle;

        struct                      // SCRIPT_COMMAND_TELEPORT_TO (6)
        {
            uint32 MapID;           // datalong
            uint32 Flags;           // datalong2
            int32  Unused1;         // dataint

            float DestX;
            float DestY;
            float DestZ;
            float Orientation;
        } TeleportTo;

        struct                      // SCRIPT_COMMAND_QUEST_EXPLORED (7)
        {
            uint32 QuestID;         // datalong
            uint32 Distance;        // datalong2
        } QuestExplored;

        struct                      // SCRIPT_COMMAND_KILL_CREDIT (8)
        {
            uint32 CreatureEntry;   // datalong
            uint32 Flags;           // datalong2
        } KillCredit;

        struct                      // SCRIPT_COMMAND_RESPAWN_GAMEOBJECT (9)
        {
            uint32 GOGuid;          // datalong
            uint32 DespawnDelay;    // datalong2
        } RespawnGameobject;

        struct                      // SCRIPT_COMMAND_TEMP_SUMMON_CREATURE (10)
        {
            uint32 CreatureEntry;   // datalong
            uint32 DespawnDelay;    // datalong2
            int32  Unused1;         // dataint

            float PosX;
            float PosY;
            float PosZ;
            float Orientation;
        } TempSummonCreature;

        struct                      // SCRIPT_COMMAND_CLOSE_DOOR (12)
                                    // SCRIPT_COMMAND_OPEN_DOOR (11)
        {
            uint32 GOGuid;          // datalong
            uint32 ResetDelay;      // datalong2
        } ToggleDoor;

                                    // SCRIPT_COMMAND_ACTIVATE_OBJECT (13)

        struct                      // SCRIPT_COMMAND_REMOVE_AURA (14)
        {
            uint32 SpellID;         // datalong
            uint32 Flags;           // datalong2
        } RemoveAura;

        struct                      // SCRIPT_COMMAND_CAST_SPELL (15)
        {
            uint32 SpellID;         // datalong
            uint32 Flags;           // datalong2
            int32  CreatureEntry;   // dataint

            float SearchRadius;
        } CastSpell;

        struct                      // SCRIPT_COMMAND_PLAY_SOUND (16)
        {
            uint32 SoundID;         // datalong
            uint32 Flags;           // datalong2
        } PlaySound;

        struct                      // SCRIPT_COMMAND_CREATE_ITEM (17)
        {
            uint32 ItemEntry;       // datalong
            uint32 Amount;          // datalong2
        } CreateItem;

        struct                      // SCRIPT_COMMAND_DESPAWN_SELF (18)
        {
            uint32 DespawnDelay;    // datalong
        } DespawnSelf;

        struct                      // SCRIPT_COMMAND_LOAD_PATH (20)
        {
            uint32 PathID;          // datalong
            uint32 IsRepeatable;    // datalong2
        } LoadPath;

        struct                      // SCRIPT_COMMAND_CALLSCRIPT_TO_UNIT (21)
        {
            uint32 CreatureEntry;   // datalong
            uint32 ScriptID;        // datalong2
            uint32 ScriptType;      // dataint
        } CallScript;

        struct                      // SCRIPT_COMMAND_KILL (22)
        {
            uint32 Unused1;         // datalong
            uint32 Unused2;         // datalong2
            int32  RemoveCorpse;    // dataint
        } Kill;

        struct                      // SCRIPT_COMMAND_ORIENTATION (30)
        {
            uint32 Flags;           // datalong
            uint32 Unused1;         // datalong2
            int32  Unused2;         // dataint

            float Unused3;
            float Unused4;
            float Unused5;
            float Orientation;
        } Orientation;

        struct                      // SCRIPT_COMMAND_EQUIP (31)
        {
            uint32 EquipmentID;     // datalong
        } Equip;

        struct                      // SCRIPT_COMMAND_MODEL (32)
        {
            uint32 ModelID;         // datalong
        } Model;

                                    // SCRIPT_COMMAND_CLOSE_GOSSIP (33)

        struct                      // SCRIPT_COMMAND_PLAYMOVIE (34)
        {
            uint32 MovieID;         // datalong
        } PlayMovie;
    };

    ScriptInfo()
    {
        memset(this, 0, sizeof(ScriptInfo));
    }

    std::string GetDebugInfo() const;
};


struct CharacterTemplate
{
    struct TemplateItem
    {
        int32 m_ItemID;
        int32 m_Faction;
        int32 m_Count;
        int8  m_Type;
    };

    struct TemplateFaction
    {
        uint32 m_FactionID;
        uint32 m_Reputaion;
    };

    uint32 m_ID;
    uint8 m_PlayerClass;
    std::string m_Name;
    std::string m_Description;

    Position m_AliancePos;
    Position m_HordePos;
    int16 m_AlianceMapID;
    int16 m_HordeMapID;
    uint8 m_Level;
    uint64 m_Money;
    uint8 m_HordeDefaultRace;
    uint8 m_AllianceDefaultRace;

    std::list<TemplateItem> m_TemplateItems;
    std::list<TemplateFaction> m_TemplateFactions;
    std::list<uint32> m_SpellIDs;
};

typedef std::unordered_map<uint32, CharacterTemplate*> CharacterTemplates;

typedef std::multimap<uint32, ScriptInfo> ScriptMap;
typedef std::map<uint32, ScriptMap > ScriptMapMap;
typedef std::multimap<uint32, uint32> SpellScriptsContainer;
typedef std::pair<SpellScriptsContainer::iterator, SpellScriptsContainer::iterator> SpellScriptsBounds;
extern ScriptMapMap sQuestEndScripts;
extern ScriptMapMap sQuestStartScripts;
extern ScriptMapMap sSpellScripts;
extern ScriptMapMap sGameObjectScripts;
extern ScriptMapMap sEventScripts;
extern ScriptMapMap sWaypointScripts;

std::string GetScriptsTableNameByType(ScriptsType type);
ScriptMapMap* GetScriptsMapByType(ScriptsType type);
std::string GetScriptCommandName(ScriptCommands command);

struct SpellClickInfo
{
    uint32 spellId;
    uint8 castFlags;
    SpellClickUserTypes userType;

    // helpers
    bool IsFitToRequirements(Unit const* clicker, Unit const* clickee) const;
};

typedef std::multimap<uint32, SpellClickInfo> SpellClickInfoContainer;
typedef std::pair<SpellClickInfoContainer::const_iterator, SpellClickInfoContainer::const_iterator> SpellClickInfoMapBounds;

struct AreaTriggerStruct
{
    uint32 target_mapId;
    float  target_X;
    float  target_Y;
    float  target_Z;
    float  target_Orientation;
};

typedef std::set<uint32> CellGuidSet;
typedef std::map<uint32/*player guid*/, uint32/*instance*/> CellCorpseSet;
struct CellObjectGuids
{
    CellGuidSet creatures;
    CellGuidSet gameobjects;
    CellCorpseSet corpses;
};
typedef ACE_Based::LockedMap<uint32/*cell_id*/, CellObjectGuids> CellObjectGuidsMap;
typedef ACE_Based::LockedMap<uint32/*(mapid, spawnMode) pair*/, CellObjectGuidsMap> MapObjectGuids;

// Trinity string ranges
#define MIN_TRINITY_STRING_ID           1                    // 'trinity_string'
#define MAX_TRINITY_STRING_ID           2000000000
#define MIN_DB_SCRIPT_STRING_ID        MAX_TRINITY_STRING_ID // 'db_script_string'
#define MAX_DB_SCRIPT_STRING_ID        2000010000
#define MIN_CREATURE_AI_TEXT_STRING_ID (-1)                 // 'creature_ai_texts'
#define MAX_CREATURE_AI_TEXT_STRING_ID (-1000000)

// Trinity Trainer Reference start range
#define TRINITY_TRAINER_START_REF      200000

struct TrinityStringLocale
{
    StringVector Content;
};

/// Key for storing temp summon data in TempSummonDataContainer
struct TempSummonGroupKey
{
    TempSummonGroupKey(uint32 summonerEntry, SummonerType summonerType, uint8 group)
        : _summonerEntry(summonerEntry), _summonerType(summonerType), _summonGroup(group)
    {
    }

    bool operator<(TempSummonGroupKey const& rhs) const
    {
        return memcmp(this, &rhs, sizeof(TempSummonGroupKey)) < 0;
    }

private:
    uint32 _summonerEntry;      ///< Summoner's entry
    SummonerType _summonerType; ///< Summoner's type, see SummonerType for available types
    uint8 _summonGroup;         ///< Summon's group id
};

typedef std::map<uint64, uint64> LinkedRespawnContainer;
typedef ACE_Based::LockedMap<uint32, CreatureData> CreatureDataContainer;
typedef ACE_Based::LockedMap<uint32, GameObjectData> GameObjectDataContainer;
typedef ACE_Based::LockedMap<TempSummonGroupKey, std::vector<TempSummonData>> TempSummonDataContainer;
typedef ACE_Based::LockedMap<uint32, CreatureLocale> CreatureLocaleContainer;
typedef ACE_Based::LockedMap<uint32, GameObjectLocale> GameObjectLocaleContainer;
typedef ACE_Based::LockedMap<uint32, ItemLocale> ItemLocaleContainer;
typedef ACE_Based::LockedMap<uint32, QuestLocale> QuestLocaleContainer;
typedef ACE_Based::LockedMap<uint32, NpcTextLocale> NpcTextLocaleContainer;
typedef ACE_Based::LockedMap<uint32, PageTextLocale> PageTextLocaleContainer;
typedef ACE_Based::LockedMap<int32, TrinityStringLocale> TrinityStringLocaleContainer;
typedef ACE_Based::LockedMap<uint64, GossipMenuItemsLocale> GossipMenuItemsLocaleContainer;
typedef ACE_Based::LockedMap<uint32, PointOfInterestLocale> PointOfInterestLocaleContainer;
typedef ACE_Based::LockedMap<uint32, QuestObjectiveLocale> QuestObjectiveLocaleContainer;

typedef std::multimap<uint32, uint32> QuestRelations;
typedef std::pair<QuestRelations::const_iterator, QuestRelations::const_iterator> QuestRelationBounds;

struct PetStatInfo
{
    enum PowerStatBase
    {
        AttackPower = 0,
        SpellPower  = 1
    };

    PetStatInfo()
    {
        memset(this, 0, sizeof(PetStatInfo));
    }

    float m_Speed;
    float m_ArmorCoef;
    float m_APSPCoef;
    float m_HealthCoef;
    float m_DamageCoef;
    float m_AttackSpeed;
    float m_CreatePower;
    float m_SecondaryStatCoef;

    Powers        m_Power;
    PowerStatBase m_PowerStat;
};

struct MailLevelReward
{
    MailLevelReward() : raceMask(0), mailTemplateId(0), senderEntry(0) {}
    MailLevelReward(uint32 _raceMask, uint32 _mailTemplateId, uint32 _senderEntry) : raceMask(_raceMask), mailTemplateId(_mailTemplateId), senderEntry(_senderEntry) {}

    uint32 raceMask;
    uint32 mailTemplateId;
    uint32 senderEntry;
};

typedef std::list<MailLevelReward> MailLevelRewardList;
typedef UNORDERED_MAP<uint8, MailLevelRewardList> MailLevelRewardContainer;

// We assume the rate is in general the same for all three types below, but chose to keep three for scalability and customization
struct RepRewardRate
{
    float quest_rate;                                       // We allow rate = 0.0 in database. For this case, it means that
    float creature_rate;                                    // no reputation are given at all for this faction/rate type.
    float spell_rate;
};

struct ReputationOnKillEntry
{
    uint32 RepFaction1;
    uint32 RepFaction2;
    uint32 ReputationMaxCap1;
    int32 RepValue1;
    uint32 ReputationMaxCap2;
    int32 RepValue2;
    bool IsTeamAward1;
    bool IsTeamAward2;
    bool TeamDependent;
};

struct RepSpilloverTemplate
{
    uint32 faction[MAX_SPILLOVER_FACTIONS];
    float faction_rate[MAX_SPILLOVER_FACTIONS];
    uint32 faction_rank[MAX_SPILLOVER_FACTIONS];
};

struct PointOfInterest
{
    uint32 entry;
    float x;
    float y;
    uint32 icon;
    uint32 flags;
    uint32 data;
    std::string icon_name;
};

struct GossipMenuItems
{
    uint32             MenuId;
    uint32             OptionIndex;
    uint8              OptionIcon;
    std::string        OptionText;
    uint32             OptionType;
    uint32             OptionNpcflag;
    uint32             ActionMenuId;
    uint32             ActionPoiId;
    bool               BoxCoded;
    uint32             BoxMoney;
    std::string        BoxText;
    ConditionContainer Conditions;
};

struct GossipMenus
{
    uint32             entry;
    uint32             text_id;
    ConditionContainer conditions;
};

typedef std::multimap<uint32, GossipMenus> GossipMenusContainer;
typedef std::pair<GossipMenusContainer::const_iterator, GossipMenusContainer::const_iterator> GossipMenusMapBounds;
typedef std::pair<GossipMenusContainer::iterator, GossipMenusContainer::iterator> GossipMenusMapBoundsNonConst;
typedef std::multimap<uint32, GossipMenuItems> GossipMenuItemsContainer;
typedef std::pair<GossipMenuItemsContainer::const_iterator, GossipMenuItemsContainer::const_iterator> GossipMenuItemsMapBounds;
typedef std::pair<GossipMenuItemsContainer::iterator, GossipMenuItemsContainer::iterator> GossipMenuItemsMapBoundsNonConst;
typedef std::map<uint32, int32> CurrencyOnKillEntry;

struct QuestPOIPoint
{
    int32 x;
    int32 y;

    QuestPOIPoint() : x(0), y(0) {}
    QuestPOIPoint(int32 _x, int32 _y) : x(_x), y(_y) {}
};

struct QuestPOI
{
    uint32 Id;
    int32 ObjectiveIndex;
    uint32 MapID;
    uint32 WorldMapAreaID;
    uint32 Floor;
    uint32 Priority;
    uint32 Flags;
    uint32 WorldEffectID;
    uint32 PlayerConditionID;
    uint32 Unk;
    std::vector<QuestPOIPoint> Points;

    QuestPOI()
        : Id(0), ObjectiveIndex(0), MapID(0), WorldMapAreaID(0), Floor(0), Priority(0), Flags(0), WorldEffectID(0), PlayerConditionID(0), Unk(0)
    {

    }
    QuestPOI(uint32 id, int32 objIndex, uint32 p_MapID, uint32 p_WorldMapAreaID, uint32 p_Floor, uint32 p_Priority, uint32 p_Flags, uint32 p_WorldEffectID, uint32 p_PlayerConditionID, uint32 p_Unk)
        : Id(id), ObjectiveIndex(objIndex), MapID(p_MapID), WorldMapAreaID(p_WorldMapAreaID), Floor(p_Floor), Priority(p_Priority), Flags(p_Flags), WorldEffectID(p_WorldEffectID), PlayerConditionID(p_PlayerConditionID), Unk(p_Unk)
    {

    }
};

typedef std::vector<QuestPOI> QuestPOIVector;
typedef UNORDERED_MAP<uint32, QuestPOIVector> QuestPOIContainer;

struct GraveYardData
{
    uint32 safeLocId;
    uint32 team;
};

typedef std::multimap<uint32, GraveYardData> GraveYardContainer;

typedef UNORDERED_MAP<uint32, VendorItemData> CacheVendorItemContainer;
typedef UNORDERED_MAP<uint32, TrainerSpellData> CacheTrainerSpellContainer;

enum SkillRangeType
{
    SKILL_RANGE_LANGUAGE,                                   // 300..300
    SKILL_RANGE_LEVEL,                                      // 1..max skill for level
    SKILL_RANGE_MONO,                                       // 1..1, grey monolite bar
    SKILL_RANGE_RANK,                                       // 1..skill for known rank
    SKILL_RANGE_NONE,                                       // 0..0 always
};

SkillRangeType GetSkillRangeType(SkillLineEntry const* pSkill, bool racial);

#define MAX_PLAYER_NAME          12                         // max allowed by client name length
#define MAX_INTERNAL_PLAYER_NAME 15                         // max server internal player name length (> MAX_PLAYER_NAME for support declined names)
#define MAX_PET_NAME             12                         // max allowed by client name length
#define MAX_CHARTER_NAME         24                         // max allowed by client name length

bool normalizePlayerName(std::string& name);
bool checkMailText(std::string _text);

struct LanguageDesc
{
    Language lang_id;
    uint32   spell_id;
    uint32   skill_id;
};

extern LanguageDesc lang_description[LANGUAGES_COUNT];
LanguageDesc const* GetLanguageDescByID(uint32 lang);

enum EncounterCreditType
{
    ENCOUNTER_CREDIT_KILL_CREATURE  = 0,
    ENCOUNTER_CREDIT_CAST_SPELL     = 1,
};

struct DungeonEncounter
{
    DungeonEncounter(DungeonEncounterEntry const* _dbcEntry, EncounterCreditType _creditType, uint32 _creditEntry, uint32 _lastEncounterDungeon)
        : dbcEntry(_dbcEntry), creditType(_creditType), creditEntry(_creditEntry), lastEncounterDungeon(_lastEncounterDungeon) { }

    DungeonEncounterEntry const* dbcEntry;
    EncounterCreditType creditType;
    uint32 creditEntry;
    uint32 lastEncounterDungeon;
};

typedef std::list<DungeonEncounter const*> DungeonEncounterList;
typedef UNORDERED_MAP<uint32, DungeonEncounterList> DungeonEncounterContainer;

struct GuildChallengeReward
{
    uint32 Experience;
    uint32 Gold;
    uint32 ChallengeCount;
    uint32 Gold2;
};

struct RealmCompletedChallenge
{
    struct ChallengeMember
    {
        ChallengeMember()
        {
            m_Guid = 0;
            m_SpecID = 0;
        }

        uint64 m_Guid;
        uint32 m_SpecID;
    };

    RealmCompletedChallenge()
    {
        memset(this, 0, sizeof (RealmCompletedChallenge));
    }

    uint32 m_GuildID;
    uint32 m_AttemptID;
    uint32 m_CompletionTime;
    uint32 m_CompletionDate;
    uint8 m_MedalEarned;
    uint8 m_MembersCount;

    ChallengeMember m_Members[5];
};

struct ChallengeReward
{
    ChallengeReward()
    {
        memset(this, 0, sizeof (ChallengeReward));
    }

    uint32 MapID;
    uint32 MoneyReward[4];
    uint32 TitleID;
    uint32 AchievementID;
};

struct MapChallengeModeHotfix
{
    MapChallengeModeHotfix()
    {
        memset(this, 0, sizeof (MapChallengeModeHotfix));
    }

    uint32 m_ID;
    uint32 m_MapID;
    uint32 m_Field2;
    uint32 m_Field3;
    uint32 m_Field4;
    uint32 m_BronzeTime;
    uint32 m_SilverTime;
    uint32 m_GoldTime;
    uint32 m_Field8;
    uint32 m_Field9;
};

struct HotfixInfo
{
    uint32 Type;
    uint32 Timestamp;
    uint32 Entry;
};

struct BattlePetTemplate
{
    uint32 Species;
    uint32 Breed;
    uint32 Quality;
    uint32 Level;
};
typedef std::map<uint32, BattlePetTemplate> BattlePetTemplateContainer;

struct BattlePetNpcTeamMember
{
    uint32 Specie;
    uint32 Level;
    uint32 Ability[3];
};
typedef std::map<uint32, std::vector<BattlePetNpcTeamMember>> BattlePetNpcTeamMembers;

struct ResearchPOIPoint
{
    ResearchPOIPoint(int32 _x, int32 _y) { x = _x; y = _y; }
    int32 x;
    int32 y;
};

struct ResearchZoneEntry
{
    uint32 POIid;
    std::vector<ResearchPOIPoint> coords;
    uint16 map;
    uint16 zone;
    uint8 level;
};

struct ResearchLootEntry
{
    uint16 id;
    float x;
    float y;
    float z;
    uint32 ResearchBranchID;
};

struct GarrisonPlotBuildingContent
{
    GarrisonPlotBuildingContent() {}
    GarrisonPlotBuildingContent(const GarrisonPlotBuildingContent& p_Other)
    {
        DB_ID               = p_Other.DB_ID;
        PlotTypeOrBuilding  = p_Other.PlotTypeOrBuilding;
        FactionIndex        = p_Other.FactionIndex;
        CreatureOrGob       = p_Other.CreatureOrGob;
        X                   = p_Other.X;
        Y                   = p_Other.Y;
        Z                   = p_Other.Z;
        O                   = p_Other.O;
    }

    uint32 DB_ID;
    int32 PlotTypeOrBuilding;
    uint32 FactionIndex;
    int32 CreatureOrGob;
    float X, Y, Z, O;
};

/// Tradeskill NPC Recipes
struct RecipesConditions
{
    uint32 RecipeID;
    uint32 PlayerConditionID;
};

namespace ItemBonus
{
    using GroupContainer = std::vector<uint32>;
    using Group = std::map<uint32, GroupContainer>;
}

typedef std::vector<HotfixInfo> HotfixData;
typedef std::map<uint32, uint32> QuestObjectiveLookupMap;
typedef std::vector<GuildChallengeReward> GuildChallengeRewardData;
typedef std::map<uint32, RealmCompletedChallenge> GroupsCompletedChallengesMap;
typedef std::map<uint32, RealmCompletedChallenge> GuildsCompletedChallengesMap;
typedef std::map<uint32, ChallengeReward> ChallengeRewardsMap;
typedef std::map<uint32, bool> UpdateSkipData;

typedef std::vector<ResearchLootEntry> ResearchLootVector;
typedef std::vector<ResearchPOIPoint> ResearchPOIPoints;
typedef std::map<uint32 /*site id*/, ResearchZoneEntry> ResearchZoneMap;
typedef std::map<uint32, std::vector<RecipesConditions>> NpcRecipesContainer;

class PlayerDumpReader;

class ObjectMgr
{
    friend class PlayerDumpReader;
    friend class ACE_Singleton<ObjectMgr, ACE_Null_Mutex>;

    private:
        ObjectMgr();
        ~ObjectMgr();

    public:
        typedef UNORDERED_MAP<uint32, Item*> ItemMap;

        typedef UNORDERED_MAP<uint32, Quest*> QuestMap;

        typedef UNORDERED_MAP<uint32, AreaTriggerStruct> AreaTriggerContainer;

        typedef UNORDERED_MAP<uint32, uint32> AreaTriggerScriptContainer;

        typedef UNORDERED_MAP<uint32, AccessRequirement> AccessRequirementContainer;
        typedef UNORDERED_MAP<uint32, LFRAccessRequirement> LFRAccessRequirements;
        typedef UNORDERED_MAP<uint32, RepRewardRate > RepRewardRateContainer;
        typedef UNORDERED_MAP<uint32, ReputationOnKillEntry> RepOnKillContainer;
        typedef UNORDERED_MAP<uint32, RepSpilloverTemplate> RepSpilloverTemplateContainer;
        typedef UNORDERED_MAP<uint32, CurrencyOnKillEntry> CurOnKillContainer;

        typedef UNORDERED_MAP<uint32, PointOfInterest> PointOfInterestContainer;

        typedef std::vector<std::string> ScriptNameContainer;

        typedef std::map<uint32, uint32> CharacterConversionMap;

        Player* GetPlayerByLowGUID(uint32 lowguid) const;

        GameObjectTemplate const* GetGameObjectTemplate(uint32 entry);
        GameObjectTemplateContainer const* GetGameObjectTemplates() const { return &_gameObjectTemplateStore; }
        int LoadReferenceVendor(int32 vendor, int32 item, uint8 type, std::set<uint32>* skip_vendors);

        void LoadGameObjectTemplate();

        void LoadGarrisonPlotBuildingContent();
        void AddGarrisonPlotBuildingContent(GarrisonPlotBuildingContent& p_Data);
        void DeleteGarrisonPlotBuildingContent(GarrisonPlotBuildingContent& p_Data);
        std::vector<GarrisonPlotBuildingContent> GetGarrisonPlotBuildingContent(int32 p_PlotTypeOrBuilding, uint32 p_FactionIndex);

        void LoadNpcRecipesConditions();
        std::vector<RecipesConditions> GetNpcRecipesConditions(uint32 p_NpcID) { return _NpcRecipesConditions[p_NpcID]; }

        CreatureTemplate const* GetCreatureTemplate(uint32 entry);
        CreatureTemplate const* GetRandomTemplate(CreatureType p_Type);
        CreatureTemplate** GetCreatureTemplates() const { return m_CreatureTemplateStore; }
        uint32 GetCreatureTemplateStoreSize() const { return m_CreatureTemplateStoreSize; }

        CreatureModelInfo const* GetCreatureModelInfo(uint32 modelId);
        CreatureModelInfo const* GetCreatureModelRandomGender(uint32* displayID);
        static uint32 ChooseDisplayId(uint32 team, const CreatureTemplate* cinfo, const CreatureData* data = NULL);
        static void ChooseCreatureFlags(const CreatureTemplate* p_CreatureTemplate, uint32& p_NpcFlags1, uint32& p_NpcFlags2, uint32& p_UnitFlags1, uint32& p_UnitFlags2, uint32& p_UnitFlags3, uint32& p_Dynamicflags, const CreatureData* p_Data = nullptr);
        EquipmentInfo const* GetEquipmentInfo(uint32 p_Entry, int8& p_ID);
        CreatureAddon const* GetCreatureAddon(uint32 lowguid);
        CreatureAddon const* GetCreatureTemplateAddon(uint32 entry);
        ItemTemplate const* GetItemTemplate(uint32 entry);
        ItemTemplateContainer const* GetItemTemplateStore() const { return &_itemTemplateStore; }

        InstanceTemplate const* GetInstanceTemplate(uint32 mapId);

        PetStatInfo const* GetPetStatInfo(uint32 p_Entry) const;

        void GetPlayerClassLevelInfo(uint32 class_, uint8 level, uint32& baseHP, uint32& baseMana) const;

        PlayerInfo const* GetPlayerInfo(uint32 race, uint32 class_) const
        {
            if (race >= MAX_RACES)
                return NULL;
            if (class_ >= MAX_CLASSES)
                return NULL;
            PlayerInfo const* info = &_playerInfo[race][class_];
            if (info->displayId_m == 0 || info->displayId_f == 0)
                return NULL;
            return info;
        }

        void GetPlayerLevelInfo(uint32 race, uint32 class_, uint8 level, PlayerLevelInfo* info) const;

        uint64 GetPlayerGUIDByName(std::string name) const;
        bool GetPlayerNameByGUID(uint64 guid, std::string& name) const;
        uint32 GetPlayerTeamByGUID(uint64 guid) const;
        uint32 GetPlayerAccountIdByGUID(uint64 guid) const;
        uint32 GetPlayerAccountIdByPlayerName(const std::string& name) const;

        uint32 GetNearestTaxiNode(float x, float y, float z, uint32 mapid, uint32 team);
        void GetTaxiPath(uint32 source, uint32 destination, uint32& path, uint32& cost);
        uint32 GetTaxiMountDisplayId(uint32 id, uint32 team, bool allowed_alt_team = false);

        Quest const* GetQuestTemplate(uint32 quest_id) const
        {
            QuestMap::const_iterator itr = _questTemplates.find(quest_id);
            return itr != _questTemplates.end() ? itr->second : NULL;
        }

        QuestMap const& GetQuestTemplates() const { return _questTemplates; }

        uint32 GetQuestForAreaTrigger(uint32 Trigger_ID) const
        {
            QuestAreaTriggerContainer::const_iterator itr = _questAreaTriggerStore.find(Trigger_ID);
            if (itr != _questAreaTriggerStore.end())
                return itr->second;
            return 0;
        }

        bool IsTavernAreaTrigger(uint32 Trigger_ID) const
        {
            return _tavernAreaTriggerStore.find(Trigger_ID) != _tavernAreaTriggerStore.end();
        }

        bool IsGameObjectForQuests(uint32 entry) const
        {
            return _gameObjectForQuestStore.find(entry) != _gameObjectForQuestStore.end();
        }

        GossipText const* GetGossipText(uint32 Text_ID) const;

        WorldSafeLocsEntry const* GetDefaultGraveYard(uint32 team);
        WorldSafeLocsEntry const* GetClosestGraveYard(float x, float y, float z, uint32 MapId, uint32 team);
        bool AddGraveYardLink(uint32 id, uint32 zoneId, uint32 team, bool persist = true);
        void RemoveGraveYardLink(uint32 id, uint32 zoneId, uint32 team, bool persist = false);
        void LoadGraveyardZones();
        GraveYardData const* FindGraveYardData(uint32 id, uint32 zone);

        AreaTriggerStruct const* GetAreaTrigger(uint32 trigger) const
        {
            AreaTriggerContainer::const_iterator itr = _areaTriggerStore.find(trigger);
            if (itr != _areaTriggerStore.end())
                return &itr->second;
            return NULL;
        }

        AccessRequirement const* GetAccessRequirement(uint32 mapid, Difficulty difficulty) const
        {
            AccessRequirementContainer::const_iterator itr = _accessRequirementStore.find(MAKE_PAIR32(mapid, difficulty));
            if (itr != _accessRequirementStore.end())
                return &itr->second;
            return NULL;
        }

        LFRAccessRequirement const* GetLFRAccessRequirement(uint32 p_DungeonID) const
        {
            LFRAccessRequirements::const_iterator l_Iter = m_LFRAccessRequirements.find(p_DungeonID);
            if (l_Iter != m_LFRAccessRequirements.end())
                return &l_Iter->second;

            return nullptr;
        }

        AreaTriggerStruct const* GetGoBackTrigger(uint32 Map) const;
        AreaTriggerStruct const* GetMapEntranceTrigger(uint32 Map) const;

        uint32 GetAreaTriggerScriptId(uint32 trigger_id);
        SpellScriptsBounds GetSpellScriptsBounds(uint32 spell_id);

        RepRewardRate const* GetRepRewardRate(uint32 factionId) const
        {
            RepRewardRateContainer::const_iterator itr = _repRewardRateStore.find(factionId);
            if (itr != _repRewardRateStore.end())
                return &itr->second;

            return NULL;
        }

        ReputationOnKillEntry const* GetReputationOnKilEntry(uint32 id) const
        {
            RepOnKillContainer::const_iterator itr = _repOnKillStore.find(id);
            if (itr != _repOnKillStore.end())
                return &itr->second;
            return NULL;
        }

        RepSpilloverTemplate const* GetRepSpilloverTemplate(uint32 factionId) const
        {
            RepSpilloverTemplateContainer::const_iterator itr = _repSpilloverTemplateStore.find(factionId);
            if (itr != _repSpilloverTemplateStore.end())
                return &itr->second;

            return NULL;
        }

        CurrencyOnKillEntry const* GetCurrencyOnKillEntry(uint32 id) const
        {
            CurOnKillContainer::const_iterator itr = _curOnKillStore.find(id);
            if (itr != _curOnKillStore.end())
                return &itr->second;
            return NULL;
        }

        CurrencyOnKillEntry const* GetPersonnalCurrencyOnKillEntry(uint32 p_ID) const
        {
            CurOnKillContainer::const_iterator l_Iter = m_PersonnalCurrOnKillStore.find(p_ID);
            if (l_Iter != m_PersonnalCurrOnKillStore.end())
                return &l_Iter->second;
            return NULL;
        }

        PointOfInterest const* GetPointOfInterest(uint32 id) const
        {
            PointOfInterestContainer::const_iterator itr = _pointsOfInterestStore.find(id);
            if (itr != _pointsOfInterestStore.end())
                return &itr->second;
            return NULL;
        }

        QuestPOIVector const* GetQuestPOIVector(uint32 questId)
        {
            QuestPOIContainer::const_iterator itr = _questPOIStore.find(questId);
            if (itr != _questPOIStore.end())
                return &itr->second;
            return NULL;
        }

        VehicleAccessoryList const* GetVehicleAccessoryList(Vehicle* veh) const;

        DungeonEncounterList const* GetDungeonEncounterList(uint32 mapId, Difficulty difficulty)
        {
            UNORDERED_MAP<uint32, DungeonEncounterList>::const_iterator itr = _dungeonEncounterStore.find(MAKE_PAIR32(mapId, difficulty));

            if (itr != _dungeonEncounterStore.end())
                return &itr->second;
            else
            {
                itr = _dungeonEncounterStore.find(MAKE_PAIR32(mapId, DifficultyNone));
                if (itr != _dungeonEncounterStore.end())
                    return &itr->second;
            }

            return NULL;
        }

        void LoadQuests();
        void LoadQuestObjectives();
        void LoadQuestObjectiveLocales();

        void LoadQuestRelations()
        {
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading GO Start Quest Data...");
            LoadGameobjectQuestStarters();
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading GO End Quest Data...");
            LoadGameobjectQuestEnders();
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Creature Start Quest Data...");
            LoadCreatureQuestStarters();
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Creature End Quest Data...");
            LoadCreatureQuestEnders();
        }
        void LoadFollowerQuests();
        std::vector<uint32> FollowerQuests;

        void LoadQuestForItem();
        std::map<uint32, std::vector<std::pair<uint32, uint8>>> QuestForItem;    ///< <ItemID, [<QuestID, ObjectiveIndex>]>

        void LoadGameobjectQuestStarters();
        void LoadGameobjectQuestEnders();
        void LoadCreatureQuestStarters();
        void LoadCreatureQuestEnders();

        QuestRelations* GetGOQuestRelationMap()
        {
            return &_goQuestRelations;
        }

        QuestRelationBounds GetGOQuestRelationBounds(uint32 go_entry)
        {
            return _goQuestRelations.equal_range(go_entry);
        }

        QuestRelationBounds GetGOQuestInvolvedRelationBounds(uint32 go_entry)
        {
            return _goQuestInvolvedRelations.equal_range(go_entry);
        }

        QuestRelations* GetCreatureQuestRelationMap()
        {
            return &_creatureQuestRelations;
        }

        QuestRelationBounds GetCreatureQuestRelationBounds(uint32 creature_entry)
        {
            return _creatureQuestRelations.equal_range(creature_entry);
        }

        void AddCreatureQuestRelationBounds(uint32 p_Creature_entry, uint32 p_QuestID)
        {
            if (_creatureQuestRelations.find(p_Creature_entry) == _creatureQuestRelations.end())
                _creatureQuestRelations.insert(std::make_pair(p_Creature_entry, p_QuestID));
        }

        QuestRelationBounds GetCreatureQuestInvolvedRelationBounds(uint32 creature_entry)
        {
            return _creatureQuestInvolvedRelations.equal_range(creature_entry);
        }

        void AddCreatureQuestInvolvedRelationBounds(uint32 p_Creature_entry, uint32 p_QuestID)
        {
            if (_creatureQuestInvolvedRelations.find(p_Creature_entry) == _creatureQuestInvolvedRelations.end())
                _creatureQuestInvolvedRelations.insert(std::make_pair(p_Creature_entry, p_QuestID));
        }

        void LoadGameObjectScripts();
        void LoadQuestEndScripts();
        void LoadQuestStartScripts();
        void LoadEventScripts();
        void LoadSpellScripts();
        void LoadWaypointScripts();

        void LoadSpellScriptNames();
        void ValidateSpellScripts();

        bool LoadTrinityStrings(char const* table, int32 min_value, int32 max_value);
        bool LoadTrinityStrings() { return LoadTrinityStrings("trinity_string", MIN_TRINITY_STRING_ID, MAX_TRINITY_STRING_ID); }
        void LoadDbScriptStrings();
        void LoadCreatureClassLevelStats();
        void LoadCreatureGroupSizeStats();
        void LoadCreatureLocales();
        void LoadCreatureTemplates();
        void LoadCreatureTemplatesDifficulties();
        void LoadCreatureTemplateAddons();
        void CheckCreatureTemplate(CreatureTemplate const* cInfo);
        void RestructCreatureGUID(uint32 nbLigneToRestruct);
        void RestructGameObjectGUID(uint32 nbLigneToRestruct);
        void LoadTempSummons();
        void LoadCreatures();
        void LoadLinkedRespawn();
        bool SetCreatureLinkedRespawn(uint32 guid, uint32 linkedGuid);
        void LoadCreatureAddons();
        void LoadCreatureModelInfo();
        void LoadEquipmentTemplates();
        void LoadGameObjectLocales();
        void LoadGameobjects();
        void LoadItemTemplates();
        void LoadItemTemplateCorrections();
        void LoadItemTemplateAddon();
        void LoadItemScriptNames();
        void LoadItemSpecs();
        void LoadItemSpecsOverride();
        void LoadItemBonusGroup();
        void LoadItemBonusGroupLinked();
        void LoadQuestLocales();
        void LoadNpcTextLocales();
        void LoadPageTextLocales();
        void LoadGossipMenuItemsLocales();
        void LoadPointOfInterestLocales();
        void LoadInstanceTemplate();
        void LoadInstanceEncounters();
        void LoadMailLevelRewards();
        void LoadVehicleTemplateAccessories();
        void LoadVehicleAccessories();
        void LoadAreaTriggerTemplates();
        void LoadAreaTriggerMoveSplines();
        void LoadAreaTriggerMoveTemplates();

        void LoadGossipText();

        void LoadAreaTriggerTeleports();
        void LoadAccessRequirements();
        void LoadLFRAccessRequirements();
        void LoadQuestAreaTriggers();
        void LoadAreaTriggerScripts();
        void LoadTavernAreaTriggers();
        void LoadGameObjectForQuests();

        void LoadPageTexts();
        PageText const* GetPageText(uint32 pageEntry);

        void LoadPlayerInfo();
        void LoadPetStatInfo();
        void LoadExplorationBaseXP();
        void LoadPetNames();
        void LoadPetNumber();
        void LoadCorpses();
        void LoadFishingBaseSkillLevel();

        void LoadReputationRewardRate();
        void LoadReputationOnKill();
        void LoadReputationSpilloverTemplate();
        void LoadCurrencyOnKill();
        void LoadPersonnalCurrencyOnKill();

        void LoadPointsOfInterest();
        void LoadQuestPOI();

        void LoadNPCSpellClickSpells();

        void LoadGameTele();

        void LoadGossipMenu();
        void LoadGossipMenuItems();

        void LoadVendors();
        void LoadTrainerSpell();
        void AddSpellToTrainer(uint32 entry, uint32 spell, uint32 spellCost, uint32 reqSkill, uint32 reqSkillValue, uint32 reqLevel);

        void LoadPhaseDefinitions();
        void LoadSpellPhaseInfo();
        void LoadSpellInvalid();
        void LoadSpellStolen();
        void LoadDisabledEncounters();
        void LoadBattlePetTemplate();
        void LoadBattlePetNpcTeamMember();
        void ComputeBattlePetSpawns();

        void LoadGuildChallengeRewardInfo();

        void LoadResearchSiteZones();
        void LoadResearchSiteLoot();

        void LoadCharacterTemplateData();
        void LoadRealmCompletedChallenges();
        void LoadChallengeRewards();

        std::vector<BattlePetNpcTeamMember> GetPetBattleTrainerTeam(uint32 p_NpcID)
        {
            return m_BattlePetNpcTeamMembers[p_NpcID];
        }

        RealmCompletedChallenge* GetGroupCompletedChallengeForMap(uint32 p_MapID)
        {
            if (m_GroupsCompletedChallenges.find(p_MapID) == m_GroupsCompletedChallenges.end())
                return nullptr;

            return &m_GroupsCompletedChallenges[p_MapID];
        }

        RealmCompletedChallenge* GetGuildCompletedChallengeForMap(uint32 p_MapID)
        {
            if (m_GuildsCompletedChallenges.find(p_MapID) == m_GuildsCompletedChallenges.end())
                return nullptr;

            return &m_GuildsCompletedChallenges[p_MapID];
        }

        ChallengeRewardsMap GetChallengeRewards() const
        {
            return m_ChallengeRewardsMap;
        }

        ChallengeReward* GetChallengeRewardsForMap(uint32 p_MapID)
        {
            if (m_ChallengeRewardsMap.find(p_MapID) == m_ChallengeRewardsMap.end())
                return nullptr;

            return &m_ChallengeRewardsMap[p_MapID];
        }

        BattlePetTemplate const* GetBattlePetTemplate(uint32 species) const
        {
            BattlePetTemplateContainer::const_iterator itr = _battlePetTemplateStore.find(species);
            return itr != _battlePetTemplateStore.end() ? &itr->second : NULL;
        }

        ResearchZoneMap const& GetResearchZoneMap() const { return _researchZoneMap; }
        ResearchLootVector const& GetResearchLoot() const { return _researchLoot; }

        PhaseDefinitionStore const* GetPhaseDefinitionStore() { return &_PhaseDefinitionStore; }
        SpellPhaseStore const* GetSpellPhaseStore() { return &_SpellPhaseStore; }

        std::string GeneratePetName(uint32 entry);
        uint32 GetBaseXP(uint8 level);
        uint32 GetXPForLevel(uint8 level) const;

        int32 GetFishingBaseSkillLevel(uint32 entry) const
        {
            FishingBaseSkillContainer::const_iterator itr = _fishingBaseForAreaStore.find(entry);
            return itr != _fishingBaseForAreaStore.end() ? itr->second : 0;
        }

        void ReturnOrDeleteOldMails(bool serverUp);

        CreatureBaseStats const* GetCreatureBaseStats(uint8 level, uint8 unitClass);
        CreatureGroupSizeStat const* GetCreatureGroupSizeStat(uint32 p_Entry, uint32 p_Difficulty) const;

        void SetHighestGuids();
        uint32 GenerateLowGuid(HighGuid guidhigh);
        uint32 GenerateAuctionID();
        uint64 GenerateEquipmentSetGuid();
        uint32 GenerateMailID();
        uint32 GeneratePetNumber();
        uint64 GenerateVoidStorageItemId();

        typedef std::multimap<int32, uint32> ExclusiveQuestGroups;
        ExclusiveQuestGroups mExclusiveQuestGroups;

        MailLevelReward const* GetMailLevelReward(uint32 level, uint32 raceMask)
        {
            MailLevelRewardContainer::const_iterator map_itr = _mailLevelRewardStore.find(level);
            if (map_itr == _mailLevelRewardStore.end())
                return NULL;

            for (MailLevelRewardList::const_iterator set_itr = map_itr->second.begin(); set_itr != map_itr->second.end(); ++set_itr)
                if (set_itr->raceMask & raceMask)
                    return &*set_itr;

            return NULL;
        }

        CellObjectGuids const& GetCellObjectGuids(uint16 mapid, uint8 spawnMode, uint32 cell_id)
        {
            return _mapObjectGuidsStore[MAKE_PAIR32(mapid, spawnMode)][cell_id];
        }

        CellObjectGuidsMap const& GetMapObjectGuids(uint16 mapid, uint8 spawnMode)
        {
            return _mapObjectGuidsStore[MAKE_PAIR32(mapid, spawnMode)];
        }

       /**
        * Gets temp summon data for all creatures of specified group.
        *
        * @param summonerId   Summoner's entry.
        * @param summonerType Summoner's type, see SummonerType for available types.
        * @param group        Id of required group.
        *
        * @return null if group was not found, otherwise reference to the creature group data
        */
        std::vector<TempSummonData> const* GetSummonGroup(uint32 summonerId, SummonerType summonerType, uint8 group) const
        {
            TempSummonDataContainer::const_iterator itr = _tempSummonDataStore.find(TempSummonGroupKey(summonerId, summonerType, group));
            if (itr != _tempSummonDataStore.end())
                return &itr->second;

            return NULL;
        }

        CreatureData const* GetCreatureData(uint32 guid) const
        {
            CreatureDataContainer::const_iterator itr = _creatureDataStore.find(guid);
            if (itr == _creatureDataStore.end()) return NULL;
            return &itr->second;
        }
        CreatureData& NewOrExistCreatureData(uint32 guid) { return _creatureDataStore[guid]; }
        void DeleteCreatureData(uint32 guid);
        uint64 GetLinkedRespawnGuid(uint64 guid) const
        {
            LinkedRespawnContainer::const_iterator itr = _linkedRespawnStore.find(guid);
            if (itr == _linkedRespawnStore.end()) return 0;
            return itr->second;
        }
        CreatureLocale const* GetCreatureLocale(uint32 entry) const
        {
            CreatureLocaleContainer::const_iterator itr = _creatureLocaleStore.find(entry);
            if (itr == _creatureLocaleStore.end()) return NULL;
            return &itr->second;
        }
        GameObjectLocale const* GetGameObjectLocale(uint32 entry) const
        {
            GameObjectLocaleContainer::const_iterator itr = _gameObjectLocaleStore.find(entry);
            if (itr == _gameObjectLocaleStore.end()) return NULL;
            return &itr->second;
        }
        QuestLocale const* GetQuestLocale(uint32 entry) const
        {
            QuestLocaleContainer::const_iterator itr = _questLocaleStore.find(entry);
            if (itr == _questLocaleStore.end()) return NULL;
            return &itr->second;
        }
        NpcTextLocale const* GetNpcTextLocale(uint32 entry) const
        {
            NpcTextLocaleContainer::const_iterator itr = _npcTextLocaleStore.find(entry);
            if (itr == _npcTextLocaleStore.end()) return NULL;
            return &itr->second;
        }
        PageTextLocale const* GetPageTextLocale(uint32 entry) const
        {
            PageTextLocaleContainer::const_iterator itr = _pageTextLocaleStore.find(entry);
            if (itr == _pageTextLocaleStore.end()) return NULL;
            return &itr->second;
        }
        GossipMenuItemsLocale const* GetGossipMenuItemsLocale(uint64 entry) const
        {
            GossipMenuItemsLocaleContainer::const_iterator itr = _gossipMenuItemsLocaleStore.find(entry);
            if (itr == _gossipMenuItemsLocaleStore.end()) return NULL;
            return &itr->second;
        }
        PointOfInterestLocale const* GetPointOfInterestLocale(uint32 poi_id) const
        {
            PointOfInterestLocaleContainer::const_iterator itr = _pointOfInterestLocaleStore.find(poi_id);
            if (itr == _pointOfInterestLocaleStore.end()) return NULL;
            return &itr->second;
        }
        QuestObjectiveLocale const* GetQuestObjectiveLocale(uint32 objectiveId) const;

        GameObjectData const* GetGOData(uint32 guid) const
        {
            GameObjectDataContainer::const_iterator itr = _gameObjectDataStore.find(guid);
            if (itr == _gameObjectDataStore.end()) return NULL;
            return &itr->second;
        }
        GameObjectData& NewGOData(uint32 guid) { return _gameObjectDataStore[guid]; }
        void DeleteGOData(uint32 guid);

        TrinityStringLocale const* GetTrinityStringLocale(int32 entry) const
        {
            TrinityStringLocaleContainer::const_iterator itr = _trinityStringLocaleStore.find(entry);
            if (itr == _trinityStringLocaleStore.end()) return NULL;
            return &itr->second;
        }
        const char* GetTrinityString(int32 entry, LocaleConstant locale_idx) const;
        const char* GetTrinityStringForDBCLocale(int32 entry) const { return GetTrinityString(entry, DBCLocaleIndex); }
        LocaleConstant GetDBCLocaleIndex() const { return DBCLocaleIndex; }
        void SetDBCLocaleIndex(LocaleConstant locale) { DBCLocaleIndex = locale; }

        void AddCorpseCellData(uint32 mapid, uint32 cellid, uint32 player_guid, uint32 instance);
        void DeleteCorpseCellData(uint32 mapid, uint32 cellid, uint32 player_guid);

        // grid objects
        void AddCreatureToGrid(uint32 guid, CreatureData const* data);
        void RemoveCreatureFromGrid(uint32 guid, CreatureData const* data);
        void AddGameobjectToGrid(uint32 guid, GameObjectData const* data);
        void RemoveGameobjectFromGrid(uint32 guid, GameObjectData const* data);
        bool AddGOData(uint32 p_LowGuid, uint32 entry, uint32 map, float x, float y, float z, float o, uint32 spawntimedelay = 0, float rotation0 = 0, float rotation1 = 0, float rotation2 = 0, float rotation3 = 0);
        uint32 AddCreData(uint32 entry, uint32 team, uint32 map, float x, float y, float z, float o, uint32 spawntimedelay = 0);
        bool MoveCreData(uint32 guid, uint32 map, Position pos);

        // reserved names
        void LoadReservedPlayersNames();
        bool IsReservedName(const std::string& name) const;

        // name with valid structure and symbols
        static uint8 CheckPlayerName(const std::string& name, bool create = false);
        static PetNameInvalidReason CheckPetName(const std::string& name);
        static bool IsValidCharterName(const std::string& name);

        static bool CheckDeclinedNames(std::wstring w_ownname, DeclinedName const& names);

        GameTele const* GetGameTele(uint32 id) const
        {
            GameTeleContainer::const_iterator itr = _gameTeleStore.find(id);
            if (itr == _gameTeleStore.end()) return NULL;
            return &itr->second;
        }
        GameTele const* GetGameTele(const std::string& name) const;
        GameTeleContainer const& GetGameTeleMap() const { return _gameTeleStore; }
        bool AddGameTele(GameTele& data);
        bool DeleteGameTele(const std::string& name);

        TrainerSpellData const* GetNpcTrainerSpells(uint32 entry) const
        {
            CacheTrainerSpellContainer::const_iterator  iter = _cacheTrainerSpellStore.find(entry);
            if (iter == _cacheTrainerSpellStore.end())
                return NULL;

            return &iter->second;
        }

        VendorItemData const* GetNpcVendorItemList(uint32 entry) const
        {
            CacheVendorItemContainer::const_iterator iter = _cacheVendorItemStore.find(entry);
            if (iter == _cacheVendorItemStore.end())
                return NULL;

            return &iter->second;
        }
        void AddVendorItem(uint32 entry, uint32 item, int32 maxcount, uint32 incrtime, uint32 extendedCost, uint8 type, bool persist = true, uint32 p_PlayerConditionID = 0); // for event
        bool RemoveVendorItem(uint32 entry, uint32 item, uint8 type, bool persist = true); // for event
        bool IsVendorItemValid(uint32 vendor_entry, uint32 id, int32 maxcount, uint32 ptime, uint32 ExtendedCost, uint8 type, Player* player = NULL, std::set<uint32>* skip_vendors = NULL, uint32 ORnpcflag = 0) const;

        void LoadScriptNames();
        ScriptNameContainer& GetScriptNames() { return _scriptNamesStore; }
        const char* GetScriptName(uint32 id) const { return id < _scriptNamesStore.size() ? _scriptNamesStore[id].c_str() : ""; }
        uint32 GetScriptId(const char* name);

        SpellClickInfoMapBounds GetSpellClickInfoMapBounds(uint32 creature_id) const
        {
            return SpellClickInfoMapBounds(_spellClickInfoStore.lower_bound(creature_id), _spellClickInfoStore.upper_bound(creature_id));
        }

        GossipMenusMapBounds GetGossipMenusMapBounds(uint32 uiMenuId) const
        {
            return GossipMenusMapBounds(_gossipMenusStore.lower_bound(uiMenuId), _gossipMenusStore.upper_bound(uiMenuId));
        }

        GossipMenusMapBoundsNonConst GetGossipMenusMapBoundsNonConst(uint32 uiMenuId)
        {
            return GossipMenusMapBoundsNonConst(_gossipMenusStore.lower_bound(uiMenuId), _gossipMenusStore.upper_bound(uiMenuId));
        }

        GossipMenuItemsMapBounds GetGossipMenuItemsMapBounds(uint32 uiMenuId) const
        {
            return GossipMenuItemsMapBounds(_gossipMenuItemsStore.lower_bound(uiMenuId), _gossipMenuItemsStore.upper_bound(uiMenuId));
        }
        GossipMenuItemsMapBoundsNonConst GetGossipMenuItemsMapBoundsNonConst(uint32 uiMenuId)
        {
            return GossipMenuItemsMapBoundsNonConst(_gossipMenuItemsStore.lower_bound(uiMenuId), _gossipMenuItemsStore.upper_bound(uiMenuId));
        }

        // for wintergrasp only
        GraveYardContainer GraveYardStore;

        static void AddLocaleString(const std::string& s, LocaleConstant locale, StringVector& data);
        static inline void GetLocaleString(const StringVector& data, int loc_idx, std::string& value)
        {
            if (data.size() > size_t(loc_idx) && !data[loc_idx].empty())
                value = data[loc_idx];
        }

        CharacterConversionMap FactionChange_Achievements;
        CharacterConversionMap FactionChange_Items;
        CharacterConversionMap FactionChange_Spells;
        CharacterConversionMap FactionChange_Reputation;
        CharacterConversionMap FactionChange_Titles;

        void LoadFactionChangeAchievements();
        void LoadFactionChangeItems();
        void LoadFactionChangeSpells();
        void LoadFactionChangeReputations();
        void LoadFactionChangeTitles();

        void LoadHotfixData(bool p_Reload = false);
        void LoadHotfixTableHashs();
        std::map<std::string, uint32> HotfixTableID;

        HotfixData const& GetHotfixData() const { return _hotfixData; }
        time_t GetHotfixDate(uint32 entry, uint32 type) const
        {
            time_t ret = 0;
            for (HotfixData::const_iterator itr = _hotfixData.begin(); itr != _hotfixData.end(); ++itr)
                if (itr->Entry == entry && itr->Type == type)
                    if (itr->Timestamp > ret)
                        ret = itr->Timestamp;

            return ret ? ret : time(NULL);
        }

        GuildChallengeRewardData const& GetGuildChallengeRewardData() const { return _challengeRewardData; }

        void LoadSkipUpdateZone();

        bool IsSkipZone(uint32 zone)
        {
            if (skipData.find(zone) != skipData.end())
                return skipData[zone];

            return false;
        }

        bool IsSkipZoneEnabled()
        {
            return !skipData.empty();
        }

        uint32 GetSkipUpdateCount()
        {
            return _skipUpdateCount;
        }

        UpdateSkipData skipData;

        ///Temporaire pour la création des Z, a remettre en private après
        GameObjectDataContainer _gameObjectDataStore;

        std::set<uint32> const& GetOverwriteExtendedCosts() const
        {
            return _overwriteExtendedCosts;
        }

        uint64 GetCreatureGUIDByLootViewGUID(uint64 lootview)
        {
            return _lootViewGUID.find(lootview) != _lootViewGUID.end() ? _lootViewGUID[lootview] : 0;
        }

        void setLootViewGUID(uint64 lootview, uint64 creature)
        {
            _lootViewGUID[lootview] = creature;
        }

        const AreaTriggerTemplateList* GetAreaTriggerTemplatesForEntry(uint32 p_Entry)
        {
            if (m_AreaTriggerTemplates.find(p_Entry) != m_AreaTriggerTemplates.end())
                return &m_AreaTriggerTemplates[p_Entry];

            return nullptr;
        }

        const AreaTriggerTemplateList* GetAreaTriggerTemplatesForSpell(uint32 p_SpellID)
        {
            if (m_AreaTriggerTemplatesSpell.find(p_SpellID) != m_AreaTriggerTemplatesSpell.end())
                return &m_AreaTriggerTemplatesSpell[p_SpellID];

            return nullptr;
        }

        AreaTriggerMoveTemplate GetAreaTriggerMoveTemplate(uint32 p_move_curve_id)
        {
            auto l_itr = m_AreaTriggerMoveTemplate.find(p_move_curve_id);
            if (l_itr != m_AreaTriggerMoveTemplate.end())
                return l_itr->second;

            return AreaTriggerMoveTemplate();
        }

        AreaTriggerMoveSplines GetAreaTriggerMoveSplines(uint32 p_move_curve_id, uint32 p_path_id)
        {
            auto l_itr = m_AreaTriggerMoveSplines.find(std::make_pair(p_move_curve_id, p_path_id));
            if (l_itr != m_AreaTriggerMoveSplines.end())
                return l_itr->second;

            return AreaTriggerMoveSplines();
        }

        CharacterTemplates const& GetCharacterTemplates() const { return m_CharacterTemplatesStore; }
        CharacterTemplate const* GetCharacterTemplate(uint32 p_ID) const;

        bool QuestObjectiveExists(uint32 objectiveId) const;
        uint32 GetQuestObjectiveQuestId(uint32 objectiveId) const;
        std::vector<QuestObjective> GetQuestObjectivesByType(uint8 p_Type)
        {
            return m_QuestObjectiveByType[p_Type];
        }

        uint32 GetNewGarrisonID()
        {
            return m_GarrisonID++;
        }

        uint32 GetNewGarrisonBuildingID()
        {
            return m_GarrisonBuildingID++;
        }

        uint32 GetNewGarrisonFollowerID()
        {
            return m_GarrisonFollowerID++;
        }

        uint32 GetNewGarrisonMissionID()
        {
            return m_GarrisonMissionID++;
        }

        uint32 GenerateNewVignetteGUID()
        {
            return m_HiVignetteGuid++;
        }

        uint32 GetNewGarrisonWorkOrderID()
        {
            return m_GarrisonWorkOrderID++;
        }

        uint32 GetNewStandaloneSceneInstanceID()
        {
            return m_StandaloneSceneInstanceID++;
        }

        ItemBonus::GroupContainer const* GetItemBonusGroup(uint32 p_GroupID) const
        {
            auto l_Find = m_ItemBonusGroupStore.find(p_GroupID);
            if (l_Find == m_ItemBonusGroupStore.end())
                return nullptr;

            return &(*l_Find).second;
        }

        bool IsInvalidSpell(uint32 p_SpellId)
        {
            if (std::find(m_SpellInvalid.begin(), m_SpellInvalid.end(), p_SpellId) != m_SpellInvalid.end())
                return true;

            return false;
        }

        bool IsStolenSpell(uint32 p_SpellId)
        {
            if (std::find(m_SpellStolen.begin(), m_SpellStolen.end(), p_SpellId) != m_SpellStolen.end())
                return true;

            return false;
        }

        bool IsDisabledEncounter(uint32 p_EncounterID, uint32 p_DifficultyID) const
        {
            auto l_Iter = m_DisabledEncounters.find(std::make_pair(p_EncounterID, p_DifficultyID));
            if (l_Iter == m_DisabledEncounters.end())
                return false;

            return true;
        }

    private:
        // first free id for selected id type
        std::atomic<unsigned int>  _auctionId;
        std::atomic<unsigned long> _equipmentSetGuid;
        std::atomic<unsigned int>  _itemTextId;
        std::atomic<unsigned int>  _mailId;
        std::atomic<unsigned int>  _hiPetNumber;
        std::atomic<unsigned long> _voidItemId;

        // first free low guid for selected guid type
        std::atomic<unsigned int> _hiCharGuid;
        std::atomic<unsigned int> _hiCreatureGuid;
        std::atomic<unsigned int> _hiPetGuid;
        std::atomic<unsigned int> _hiVehicleGuid;
        std::atomic<unsigned int> _hiGoGuid;
        std::atomic<unsigned int> _hiDoGuid;
        std::atomic<unsigned int> _hiCorpseGuid;
        std::atomic<unsigned int> _hiAreaTriggerGuid;
        std::atomic<unsigned int> _hiMoTransGuid;
        std::atomic<unsigned int> m_GarrisonID;
        std::atomic<unsigned int> m_GarrisonBuildingID;
        std::atomic<unsigned int> m_GarrisonFollowerID;
        std::atomic<unsigned int> m_GarrisonMissionID;
        std::atomic<unsigned int> m_GarrisonWorkOrderID;
        std::atomic<unsigned int> m_HiVignetteGuid;
        std::atomic<unsigned int> m_StandaloneSceneInstanceID;

        std::atomic<unsigned int> m_HighItemGuid;

        QuestMap _questTemplates;
        QuestObjectiveLookupMap m_questObjectiveLookup;
        std::vector<uint32> m_IgnoredQuestObjectives;

        typedef UNORDERED_MAP<uint32, GossipText> GossipTextContainer;
        typedef UNORDERED_MAP<uint32, uint32> QuestAreaTriggerContainer;
        typedef std::set<uint32> TavernAreaTriggerContainer;
        typedef std::set<uint32> GameObjectForQuestContainer;

        QuestAreaTriggerContainer _questAreaTriggerStore;
        TavernAreaTriggerContainer _tavernAreaTriggerStore;
        GameObjectForQuestContainer _gameObjectForQuestStore;
        GossipTextContainer _gossipTextStore;
        AreaTriggerContainer _areaTriggerStore;
        AreaTriggerScriptContainer _areaTriggerScriptStore;
        AccessRequirementContainer _accessRequirementStore;
        LFRAccessRequirements m_LFRAccessRequirements;
        DungeonEncounterContainer _dungeonEncounterStore;

        RepRewardRateContainer _repRewardRateStore;
        RepOnKillContainer _repOnKillStore;
        RepSpilloverTemplateContainer _repSpilloverTemplateStore;
        CurOnKillContainer _curOnKillStore;
        CurOnKillContainer m_PersonnalCurrOnKillStore;

        GossipMenusContainer _gossipMenusStore;
        GossipMenuItemsContainer _gossipMenuItemsStore;
        PointOfInterestContainer _pointsOfInterestStore;

        QuestPOIContainer _questPOIStore;

        QuestRelations _goQuestRelations;
        QuestRelations _goQuestInvolvedRelations;
        QuestRelations _creatureQuestRelations;
        QuestRelations _creatureQuestInvolvedRelations;

        //character reserved names
        typedef std::set<std::wstring> ReservedNamesContainer;
        ReservedNamesContainer _reservedNamesStore;

        GameTeleContainer _gameTeleStore;

        ScriptNameContainer _scriptNamesStore;

        SpellClickInfoContainer _spellClickInfoStore;

        SpellScriptsContainer _spellScriptsStore;

        VehicleAccessoryContainer _vehicleTemplateAccessoryStore;
        VehicleAccessoryContainer _vehicleAccessoryStore;

        LocaleConstant DBCLocaleIndex;

        PageTextContainer _pageTextStore;
        InstanceTemplateContainer _instanceTemplateStore;

        PhaseDefinitionStore _PhaseDefinitionStore;
        SpellPhaseStore _SpellPhaseStore;

        uint32 _skipUpdateCount;
        ACE_Based::LockedMap<uint64, uint64> _lootViewGUID;

        AreaTriggerTemplateContainer m_AreaTriggerTemplates;
        AreaTriggerTemplateContainer m_AreaTriggerTemplatesSpell;
        AreaTriggerMoveSplinesContainer m_AreaTriggerMoveSplines;
        AreaTriggerMoveTemplateContainer m_AreaTriggerMoveTemplate;

        ResearchZoneMap _researchZoneMap;
        ResearchLootVector _researchLoot;

        std::vector<GarrisonPlotBuildingContent> m_GarrisonPlotBuildingContents;
        NpcRecipesContainer _NpcRecipesConditions;

        ItemBonus::Group m_ItemBonusGroupStore;

    private:
        CharacterTemplates m_CharacterTemplatesStore;
        void LoadScripts(ScriptsType type);
        void CheckScripts(ScriptsType type, std::set<int32>& ids);
        void LoadQuestRelationsHelper(QuestRelations& map, std::string table, bool starter, bool go);
        void PlayerCreateInfoAddItemHelper(uint32 race_, uint32 class_, uint32 itemId, int32 count);

        MailLevelRewardContainer _mailLevelRewardStore;

        CreatureBaseStatsContainer _creatureBaseStatsStore;
        CreatureGroupSizeStatsContainer m_CreatureGroupSizeStore;

        typedef std::map<uint32, PetStatInfo> PetStatInfoContainer;
        PetStatInfoContainer m_PetInfoStore;

        void BuildPlayerLevelInfo(uint8 race, uint8 class_, uint8 level, PlayerLevelInfo* plinfo) const;

        PlayerInfo _playerInfo[MAX_RACES][MAX_CLASSES];

        typedef std::vector<uint32> PlayerXPperLevel;       // [level]
        PlayerXPperLevel _playerXPperLevel;

        typedef std::map<uint32, uint32> BaseXPContainer;          // [area level][base xp]
        BaseXPContainer _baseXPTable;

        typedef std::map<uint32, int32> FishingBaseSkillContainer; // [areaId][base skill level]
        FishingBaseSkillContainer _fishingBaseForAreaStore;

        typedef std::map<uint32, StringVector> HalfNameContainer;
        HalfNameContainer _petHalfName0;
        HalfNameContainer _petHalfName1;

        MapObjectGuids _mapObjectGuidsStore;
        CreatureDataContainer _creatureDataStore;

        CreatureTemplate** m_CreatureTemplateStore;
        uint32  m_CreatureTemplateStoreSize;

        CreatureModelContainer _creatureModelStore;
        CreatureAddonContainer _creatureAddonStore;
        CreatureAddonContainer _creatureTemplateAddonStore;
        EquipmentInfoContainer _equipmentInfoStore;
        LinkedRespawnContainer _linkedRespawnStore;
        CreatureLocaleContainer _creatureLocaleStore;
        //GameObjectDataContainer _gameObjectDataStore;
        GameObjectLocaleContainer _gameObjectLocaleStore;
        GameObjectTemplateContainer _gameObjectTemplateStore;
        /// Stores temp summon data grouped by summoner's entry, summoner's type and group id
        TempSummonDataContainer _tempSummonDataStore;

        ItemTemplateContainer _itemTemplateStore;
        QuestLocaleContainer _questLocaleStore;
        NpcTextLocaleContainer _npcTextLocaleStore;
        PageTextLocaleContainer _pageTextLocaleStore;
        TrinityStringLocaleContainer _trinityStringLocaleStore;
        GossipMenuItemsLocaleContainer _gossipMenuItemsLocaleStore;
        PointOfInterestLocaleContainer _pointOfInterestLocaleStore;
        BattlePetTemplateContainer _battlePetTemplateStore;
        BattlePetNpcTeamMembers m_BattlePetNpcTeamMembers;
        QuestObjectiveLocaleContainer m_questObjectiveLocaleStore;

        std::map<uint8, std::vector<QuestObjective>> m_QuestObjectiveByType;

        CacheVendorItemContainer _cacheVendorItemStore;
        CacheTrainerSpellContainer _cacheTrainerSpellStore;

        std::set<uint32> _difficultyEntries[Difficulty::MaxDifficulties - 1]; // already loaded difficulty 1 value in creatures, used in CheckCreatureTemplate
        std::set<uint32> _hasDifficultyEntries[Difficulty::MaxDifficulties - 1]; // already loaded creatures with difficulty 1 values, used in CheckCreatureTemplate

        std::list<uint32> m_SpellInvalid;
        std::list<uint32> m_SpellStolen;

        std::set<uint32> _overwriteExtendedCosts;

        enum CreatureLinkedRespawnType
        {
            CREATURE_TO_CREATURE,
            CREATURE_TO_GO,         // Creature is dependant on GO
            GO_TO_GO,
            GO_TO_CREATURE,         // GO is dependant on creature
        };
        HotfixData _hotfixData;
        GuildChallengeRewardData _challengeRewardData;

        GroupsCompletedChallengesMap m_GroupsCompletedChallenges;
        GuildsCompletedChallengesMap m_GuildsCompletedChallenges;
        ChallengeRewardsMap m_ChallengeRewardsMap;

        std::set<std::pair<uint32, uint32>> m_DisabledEncounters;
};

#define sObjectMgr ACE_Singleton<ObjectMgr, ACE_Null_Mutex>::instance()

// scripting access functions
bool LoadTrinityStrings(char const* table, int32 start_value = MAX_CREATURE_AI_TEXT_STRING_ID, int32 end_value = std::numeric_limits<int32>::min());

#endif
