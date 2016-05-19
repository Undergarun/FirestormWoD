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
#include "DatabaseEnv.h"
#include "Log.h"
#include "MapManager.h"
#include "ObjectMgr.h"
#include "GuildMgr.h"
#include "GroupMgr.h"
#include "SpellMgr.h"
#include "UpdateMask.h"
#include "World.h"
#include "Arena.h"
#include "Transport.h"
#include "Language.h"
#include "GameEventMgr.h"
#include "Spell.h"
#include "Chat.h"
#include "AccountMgr.h"
#include "InstanceSaveMgr.h"
#include "SpellAuras.h"
#include "Util.h"
#include "WaypointManager.h"
#include "GossipDef.h"
#include "Vehicle.h"
#include "AchievementMgr.h"
#include "DisableMgr.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "PoolMgr.h"
#include "DB2Structure.h"
#include "DB2Stores.h"
#include "Configuration/Config.h"
#include "VMapFactory.h"
#include "GarrisonMgr.hpp"

ScriptMapMap sQuestEndScripts;
ScriptMapMap sQuestStartScripts;
ScriptMapMap sSpellScripts;
ScriptMapMap sGameObjectScripts;
ScriptMapMap sEventScripts;
ScriptMapMap sWaypointScripts;

std::string GetScriptsTableNameByType(ScriptsType type)
{
    std::string res = "";
    switch (type)
    {
        case SCRIPTS_QUEST_END:     res = "quest_end_scripts";  break;
        case SCRIPTS_QUEST_START:   res = "quest_start_scripts";break;
        case SCRIPTS_SPELL:         res = "spell_scripts";      break;
        case SCRIPTS_GAMEOBJECT:    res = "gameobject_scripts"; break;
        case SCRIPTS_EVENT:         res = "event_scripts";      break;
        case SCRIPTS_WAYPOINT:      res = "waypoint_scripts";   break;
        default: break;
    }
    return res;
}

ScriptMapMap* GetScriptsMapByType(ScriptsType type)
{
    ScriptMapMap* res = NULL;
    switch (type)
    {
        case SCRIPTS_QUEST_END:     res = &sQuestEndScripts;    break;
        case SCRIPTS_QUEST_START:   res = &sQuestStartScripts;  break;
        case SCRIPTS_SPELL:         res = &sSpellScripts;       break;
        case SCRIPTS_GAMEOBJECT:    res = &sGameObjectScripts;  break;
        case SCRIPTS_EVENT:         res = &sEventScripts;       break;
        case SCRIPTS_WAYPOINT:      res = &sWaypointScripts;    break;
        default: break;
    }
    return res;
}

std::string GetScriptCommandName(ScriptCommands command)
{
    std::string res = "";
    switch (command)
    {
        case SCRIPT_COMMAND_TALK: res = "SCRIPT_COMMAND_TALK"; break;
        case SCRIPT_COMMAND_EMOTE: res = "SCRIPT_COMMAND_EMOTE"; break;
        case SCRIPT_COMMAND_FIELD_SET: res = "SCRIPT_COMMAND_FIELD_SET"; break;
        case SCRIPT_COMMAND_MOVE_TO: res = "SCRIPT_COMMAND_MOVE_TO"; break;
        case SCRIPT_COMMAND_FLAG_SET: res = "SCRIPT_COMMAND_FLAG_SET"; break;
        case SCRIPT_COMMAND_FLAG_REMOVE: res = "SCRIPT_COMMAND_FLAG_REMOVE"; break;
        case SCRIPT_COMMAND_TELEPORT_TO: res = "SCRIPT_COMMAND_TELEPORT_TO"; break;
        case SCRIPT_COMMAND_QUEST_EXPLORED: res = "SCRIPT_COMMAND_QUEST_EXPLORED"; break;
        case SCRIPT_COMMAND_KILL_CREDIT: res = "SCRIPT_COMMAND_KILL_CREDIT"; break;
        case SCRIPT_COMMAND_RESPAWN_GAMEOBJECT: res = "SCRIPT_COMMAND_RESPAWN_GAMEOBJECT"; break;
        case SCRIPT_COMMAND_TEMP_SUMMON_CREATURE: res = "SCRIPT_COMMAND_TEMP_SUMMON_CREATURE"; break;
        case SCRIPT_COMMAND_OPEN_DOOR: res = "SCRIPT_COMMAND_OPEN_DOOR"; break;
        case SCRIPT_COMMAND_CLOSE_DOOR: res = "SCRIPT_COMMAND_CLOSE_DOOR"; break;
        case SCRIPT_COMMAND_ACTIVATE_OBJECT: res = "SCRIPT_COMMAND_ACTIVATE_OBJECT"; break;
        case SCRIPT_COMMAND_REMOVE_AURA: res = "SCRIPT_COMMAND_REMOVE_AURA"; break;
        case SCRIPT_COMMAND_CAST_SPELL: res = "SCRIPT_COMMAND_CAST_SPELL"; break;
        case SCRIPT_COMMAND_PLAY_SOUND: res = "SCRIPT_COMMAND_PLAY_SOUND"; break;
        case SCRIPT_COMMAND_CREATE_ITEM: res = "SCRIPT_COMMAND_CREATE_ITEM"; break;
        case SCRIPT_COMMAND_DESPAWN_SELF: res = "SCRIPT_COMMAND_DESPAWN_SELF"; break;
        case SCRIPT_COMMAND_LOAD_PATH: res = "SCRIPT_COMMAND_LOAD_PATH"; break;
        case SCRIPT_COMMAND_CALLSCRIPT_TO_UNIT: res = "SCRIPT_COMMAND_CALLSCRIPT_TO_UNIT"; break;
        case SCRIPT_COMMAND_KILL: res = "SCRIPT_COMMAND_KILL"; break;
        case SCRIPT_COMMAND_ORIENTATION: res = "SCRIPT_COMMAND_ORIENTATION"; break;
        case SCRIPT_COMMAND_EQUIP: res = "SCRIPT_COMMAND_EQUIP"; break;
        case SCRIPT_COMMAND_MODEL: res = "SCRIPT_COMMAND_MODEL"; break;
        case SCRIPT_COMMAND_CLOSE_GOSSIP: res = "SCRIPT_COMMAND_CLOSE_GOSSIP"; break;
        case SCRIPT_COMMAND_PLAYMOVIE: res = "SCRIPT_COMMAND_PLAYMOVIE"; break;
        default:
        {
            char sz[32];
            sprintf(sz, "Unknown command: %d", command);
            res = sz;
            break;
        }
    }
    return res;
}

std::string ScriptInfo::GetDebugInfo() const
{
    char sz[256];
    sprintf(sz, "%s ('%s' script id: %u)", GetScriptCommandName(command).c_str(), GetScriptsTableNameByType(type).c_str(), id);
    return std::string(sz);
}

bool normalizePlayerName(std::string& p_Name)
{
    if (p_Name.empty())
        return false;

    if (p_Name[0] == -61 && p_Name[1] == -97) // Interdiction d'utiliser ce caractere au debut, il fait planter l'affichage cote client
        return false;

    wchar_t wstr_buf[MAX_INTERNAL_PLAYER_NAME+1];
    size_t wstr_len = MAX_INTERNAL_PLAYER_NAME;

    if (!Utf8toWStr(p_Name, &wstr_buf[0], wstr_len))
        return false;

    wstr_buf[0] = wcharToUpper(wstr_buf[0]);
    for (size_t i = 1; i < wstr_len; ++i)
        wstr_buf[i] = wcharToLower(wstr_buf[i]);

    if (!WStrToUtf8(wstr_buf, wstr_len, p_Name))
        return false;

    std::string l_NewName = "";
    for (uint32 l_I = 0; l_I < p_Name.length(); l_I++)
    {
        if (p_Name[l_I] == '-')
            break;

        l_NewName += p_Name[l_I];
    }

    p_Name = l_NewName;
    return true;
}

bool checkMailText(std::string _text)
{
    std::string temp_str(_text);
    std::transform(temp_str.begin(), temp_str.end(), temp_str.begin(), tolower);

    // Check for special symbols
    bool bOk = (temp_str.find("|tinterface") == std::string::npos);

    return bOk;
}

LanguageDesc lang_description[LANGUAGES_COUNT] =
{
    { LANG_ADDON,           0, 0                       },
    { LANG_UNIVERSAL,       0, 0                       },
    { LANG_ORCISH,        669, SKILL_LANG_ORCISH       },
    { LANG_DARNASSIAN,    671, SKILL_LANG_DARNASSIAN   },
    { LANG_TAURAHE,       670, SKILL_LANG_TAURAHE      },
    { LANG_DWARVISH,      672, SKILL_LANG_DWARVEN      },
    { LANG_COMMON,        668, SKILL_LANG_COMMON       },
    { LANG_DEMONIC,       815, SKILL_LANG_DEMON_TONGUE },
    { LANG_TITAN,         816, SKILL_LANG_TITAN        },
    { LANG_THALASSIAN,    813, SKILL_LANG_THALASSIAN   },
    { LANG_DRACONIC,      814, SKILL_LANG_DRACONIC     },
    { LANG_KALIMAG,       817, SKILL_LANG_OLD_TONGUE   },
    { LANG_GNOMISH,      7340, SKILL_LANG_GNOMISH      },
    { LANG_TROLL,        7341, SKILL_LANG_TROLL        },
    { LANG_GUTTERSPEAK, 17737, SKILL_LANG_GUTTERSPEAK  },
    { LANG_DRAENEI,     29932, SKILL_LANG_DRAENEI      },
    { LANG_ZOMBIE,          0, 0                       },
    { LANG_GNOMISH_BINARY,  0, 0                       },
    { LANG_GOBLIN_BINARY,   0, 0                       },
    { LANG_WORGEN,      69270, SKILL_LANG_WORGEN       },
    { LANG_GOBLIN,      69269, SKILL_LANG_GOBLIN       },
    { LANG_PANDAREN_N,  108127,  SKILL_LANG_PANDAREN_N },
    { LANG_PANDAREN_H,  108130,  SKILL_LANG_PANDAREN_A },
    { LANG_PANDAREN_A,  108131,  SKILL_LANG_PANDAREN_H }

};

LanguageDesc const* GetLanguageDescByID(uint32 lang)
{
    for (uint8 i = 0; i < LANGUAGES_COUNT; ++i)
    {
        if (uint32(lang_description[i].lang_id) == lang)
            return &lang_description[i];
    }

    return NULL;
}

bool SpellClickInfo::IsFitToRequirements(Unit const* clicker, Unit const* clickee) const
{
    Player const* playerClicker = clicker->ToPlayer();
    if (!playerClicker)
        return true;

    Unit const* summoner = NULL;
    // Check summoners for party
    if (clickee->isSummon())
        summoner = clickee->ToTempSummon()->GetSummoner();
    if (!summoner)
        summoner = clickee;

    // This only applies to players
    switch (userType)
    {
        case SPELL_CLICK_USER_FRIEND:
            if (!playerClicker->IsFriendlyTo(summoner))
                return false;
            break;
        case SPELL_CLICK_USER_RAID:
            if (!playerClicker->IsInRaidWith(summoner))
                return false;
            break;
        case SPELL_CLICK_USER_PARTY:
            if (!playerClicker->IsInPartyWith(summoner))
                return false;
            break;
        default:
            break;
    }

    return true;
}

ObjectMgr::ObjectMgr(): _auctionId(1), _equipmentSetGuid(1),
    _itemTextId(1), _mailId(1), _hiPetNumber(1), _voidItemId(1), _hiCharGuid(1),
    _hiCreatureGuid(1), _hiPetGuid(1), _hiVehicleGuid(1),
    _hiGoGuid(1), _hiDoGuid(1), _hiCorpseGuid(1), _hiAreaTriggerGuid(1), _hiMoTransGuid(1), m_HiVignetteGuid(1), _skipUpdateCount(1)
{
    m_HighItemGuid = 1;

    m_CreatureTemplateStoreSize = 0;
    m_CreatureTemplateStore     = nullptr;
}

ObjectMgr::~ObjectMgr()
{
    for (QuestMap::iterator i = _questTemplates.begin(); i != _questTemplates.end(); ++i)
        delete i->second;

    for (int race = 0; race < MAX_RACES; ++race)
        for (int class_ = 0; class_ < MAX_CLASSES; ++class_)
            delete[] _playerInfo[race][class_].levelInfo;

    for (CacheVendorItemContainer::iterator itr = _cacheVendorItemStore.begin(); itr != _cacheVendorItemStore.end(); ++itr)
        itr->second.Clear();

    _cacheTrainerSpellStore.clear();

    for (DungeonEncounterContainer::iterator itr =_dungeonEncounterStore.begin(); itr != _dungeonEncounterStore.end(); ++itr)
        for (DungeonEncounterList::iterator encounterItr = itr->second.begin(); encounterItr != itr->second.end(); ++encounterItr)
            delete *encounterItr;

    for (CharacterTemplates::iterator l_Iter = m_CharacterTemplatesStore.begin(); l_Iter != m_CharacterTemplatesStore.end(); l_Iter++)
        delete l_Iter->second;
}

void ObjectMgr::AddLocaleString(std::string const& s, LocaleConstant locale, StringVector& data)
{
    if (!s.empty())
    {
        if (data.size() <= size_t(locale))
            data.resize(locale + 1);

        data[locale] = s;
    }
}

void ObjectMgr::LoadCreatureLocales()
{
    uint32 oldMSTime = getMSTime();

    _creatureLocaleStore.clear();                              // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, name_loc1, femaleName_loc1, subname_loc1, name_loc2, femaleName_loc2, subname_loc2, name_loc3, femaleName_loc3, subname_loc3, name_loc4, femaleName_loc4, subname_loc4, name_loc5, femaleName_loc5, subname_loc5, name_loc6, femaleName_loc6, subname_loc6, name_loc7, femaleName_loc7, subname_loc7, name_loc8, femaleName_loc8, subname_loc8, name_loc9, femaleName_loc9, subname_loc9, name_loc10, femaleName_loc10, subname_loc10 FROM locales_creature");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        CreatureLocale& data = _creatureLocaleStore[entry];

        for (uint8 i = 1; i < TOTAL_LOCALES; ++i)
        {
            LocaleConstant locale = (LocaleConstant) i;
            AddLocaleString(fields[1 + 3 * (i - 1)].GetString(), locale, data.Name);
            AddLocaleString(fields[1 + 3 * (i - 1) + 1].GetString(), locale, data.l_FemaleName);
            AddLocaleString(fields[1 + 3 * (i - 1) + 2].GetString(), locale, data.SubName);
        }
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %lu creature locale strings in %u ms", (unsigned long)_creatureLocaleStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadGossipMenuItemsLocales()
{
    uint32 oldMSTime = getMSTime();

    _gossipMenuItemsLocaleStore.clear();                              // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT menu_id, id, "
        "option_text_loc1, box_text_loc1, option_text_loc2, box_text_loc2, "
        "option_text_loc3, box_text_loc3, option_text_loc4, box_text_loc4, "
        "option_text_loc5, box_text_loc5, option_text_loc6, box_text_loc6, "
        "option_text_loc7, box_text_loc7, option_text_loc8, box_text_loc8, "
        "option_text_loc9, box_text_loc9, option_text_loc10, box_text_loc10 "
        "FROM locales_gossip_menu_option");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 menuId   = fields[0].GetUInt32();
        uint16 id       = fields[1].GetUInt16();

        GossipMenuItemsLocale& data = _gossipMenuItemsLocaleStore[MAKE_PAIR64(menuId, id)];

        for (uint8 i = 1; i < TOTAL_LOCALES; ++i)
        {
            LocaleConstant locale = (LocaleConstant) i;
            AddLocaleString(fields[2 + 2 * (i - 1)].GetString(), locale, data.OptionText);
            AddLocaleString(fields[2 + 2 * (i - 1) + 1].GetString(), locale, data.BoxText);
        }
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %lu gossip_menu_option locale strings in %u ms", (unsigned long)_gossipMenuItemsLocaleStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadPointOfInterestLocales()
{
    uint32 oldMSTime = getMSTime();

    _pointOfInterestLocaleStore.clear();                              // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, icon_name_loc1, icon_name_loc2, icon_name_loc3, icon_name_loc4, icon_name_loc5, icon_name_loc6, icon_name_loc7, icon_name_loc8, icon_name_loc9, icon_name_loc10 FROM locales_points_of_interest");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        PointOfInterestLocale& data = _pointOfInterestLocaleStore[entry];

        for (uint8 i = 1; i < TOTAL_LOCALES; ++i)
            AddLocaleString(fields[i].GetString(), LocaleConstant(i), data.IconName);
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %lu points_of_interest locale strings in %u ms", (unsigned long)_pointOfInterestLocaleStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadCreatureTemplates()
{
    uint32 oldMSTime = getMSTime();

    //                                                 0           1          2           3          4       5
    QueryResult result = WorldDatabase.Query("SELECT entry, KillCredit1, KillCredit2, modelid1, modelid2, modelid3, "
    //                                           6        7      8           9       10           11            12       13      14     15       16       17         18        19        20
                                             "modelid4, name, femaleName, subname, IconName, gossip_menu_id, minlevel, maxlevel, exp, exp_unk, faction, npcflag, npcflag2, speed_walk, speed_run, "
    //                                             21       22   23      24            25           26               27               28          29             30
                                             "speed_fly, scale, rank,  dmgschool, dmg_multiplier, baseattacktime, rangeattacktime, baseVariance, rangeVariance,  unit_class, "
    //                                             31         32           33          34            35              36          37            38          39            40           41
                                             "unit_flags, unit_flags2, unit_flags3, dynamicflags, WorldEffectID,   family, trainer_type, trainer_spell, trainer_class, trainer_race, type, "
    //                                            42          43           44          45          46         47         48            49         50            51           52
                                             "type_flags, type_flags2, lootid, pickpocketloot, skinloot, resistance1, resistance2, resistance3, resistance4, resistance5, resistance6, "
    //                                           53     54       55     56       57     58       59      60        61       62       63      64       65
                                             "spell1, spell2, spell3, spell4, spell5, spell6, spell7, spell8, VehicleId, mingold, maxgold, AIName, MovementType, "
    //                                           66             67          68         69           70           71         72            73           74          75         76          77
                                             "InhabitType, HoverHeight, Health_mod, Mana_mod, Mana_mod_extra, Armor_mod, RacialLeader, questItem1, questItem2, questItem3, questItem4, questItem5, "
    //                                            78           79         80          81               82               83              84            85
                                             "questItem6, movementId, VignetteID, TrackingQuestID,  RegenHealth, mechanic_immune_mask, flags_extra, ScriptName "
                                             "FROM creature_template;");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature template definitions. DB table `creature_template` is empty.");
        return;
    }

    QueryResult l_MaxResult = WorldDatabase.Query("SELECT MAX(entry) from creature_template");
    if (!l_MaxResult)
        return;

    m_CreatureTemplateStoreSize = l_MaxResult->Fetch()[0].GetUInt32() + 1;  ///< Start from 0
    m_CreatureTemplateStore     = new CreatureTemplate*[m_CreatureTemplateStoreSize];

    memset(m_CreatureTemplateStore, 0, m_CreatureTemplateStoreSize * sizeof(CreatureTemplate*));

    uint32 count = 0;
    do
    {
        uint8 index = 0;
        Field* fields = result->Fetch();

        uint32 l_Entry = fields[index++].GetUInt32();


        CreatureTemplate* l_CreatureTemplate = new CreatureTemplate();

        l_CreatureTemplate->Entry = l_Entry;

        for (uint8 i = 0; i < Difficulty::MaxDifficulties; ++i)
            l_CreatureTemplate->DifficultyEntry[i] = 0;

        for (uint8 i = 0; i < MAX_KILL_CREDIT; ++i)
            l_CreatureTemplate->KillCredit[i] = fields[index++].GetUInt32();

        l_CreatureTemplate->Modelid1          = fields[index++].GetUInt32();
        l_CreatureTemplate->Modelid2          = fields[index++].GetUInt32();
        l_CreatureTemplate->Modelid3          = fields[index++].GetUInt32();
        l_CreatureTemplate->Modelid4          = fields[index++].GetUInt32();
        l_CreatureTemplate->Name              = fields[index++].GetString();
        l_CreatureTemplate->FemaleName        = fields[index++].GetString();
        l_CreatureTemplate->SubName           = fields[index++].GetString();
        l_CreatureTemplate->IconName          = fields[index++].GetString();
        l_CreatureTemplate->GossipMenuId      = fields[index++].GetUInt32();
        l_CreatureTemplate->minlevel          = fields[index++].GetUInt8();
        l_CreatureTemplate->maxlevel          = fields[index++].GetUInt8();
        l_CreatureTemplate->expansion         = uint32(fields[index++].GetInt16());
        l_CreatureTemplate->expansionUnknown  = uint32(fields[index++].GetUInt16());
        l_CreatureTemplate->faction           = uint32(fields[index++].GetUInt16());
        l_CreatureTemplate->NpcFlags1         = fields[index++].GetUInt32();
        l_CreatureTemplate->NpcFlags2         = fields[index++].GetUInt32();
        l_CreatureTemplate->speed_walk        = fields[index++].GetFloat();
        l_CreatureTemplate->speed_run         = fields[index++].GetFloat();
        l_CreatureTemplate->speed_fly         = fields[index++].GetFloat();
        l_CreatureTemplate->scale             = fields[index++].GetFloat();
        l_CreatureTemplate->rank              = uint32(fields[index++].GetUInt8());
        l_CreatureTemplate->dmgschool         = uint32(fields[index++].GetInt8());
        l_CreatureTemplate->dmg_multiplier    = fields[index++].GetFloat();
        l_CreatureTemplate->baseattacktime    = fields[index++].GetUInt32();
        l_CreatureTemplate->rangeattacktime   = fields[index++].GetUInt32();
        l_CreatureTemplate->baseVariance      = fields[index++].GetFloat();
        l_CreatureTemplate->rangeVariance     = fields[index++].GetFloat();
        l_CreatureTemplate->unit_class        = uint32(fields[index++].GetUInt8());
        l_CreatureTemplate->UnitFlags1        = fields[index++].GetUInt32();
        l_CreatureTemplate->UnitFlags2        = fields[index++].GetUInt32();
        l_CreatureTemplate->UnitFlags3        = fields[index++].GetUInt32();
        l_CreatureTemplate->dynamicflags      = fields[index++].GetUInt32();
        l_CreatureTemplate->WorldEffectID     = fields[index++].GetUInt32();
        l_CreatureTemplate->family            = uint32(fields[index++].GetUInt32());
        l_CreatureTemplate->trainer_type      = uint32(fields[index++].GetUInt8());
        l_CreatureTemplate->trainer_spell     = fields[index++].GetUInt32();
        l_CreatureTemplate->trainer_class     = uint32(fields[index++].GetUInt8());
        l_CreatureTemplate->trainer_race      = uint32(fields[index++].GetUInt8());
        l_CreatureTemplate->type              = uint32(fields[index++].GetUInt8());
        l_CreatureTemplate->type_flags        = fields[index++].GetUInt32();
        l_CreatureTemplate->type_flags2       = fields[index++].GetUInt32();
        l_CreatureTemplate->lootid            = fields[index++].GetUInt32();
        l_CreatureTemplate->pickpocketLootId  = fields[index++].GetUInt32();
        l_CreatureTemplate->SkinLootId        = fields[index++].GetUInt32();

        for (uint8 i = SPELL_SCHOOL_HOLY; i < MAX_SPELL_SCHOOL; ++i)
            l_CreatureTemplate->resistance[i] = fields[index++].GetInt16();

        for (uint8 i = 0; i < CREATURE_MAX_SPELLS; ++i)
            l_CreatureTemplate->spells[i] = fields[index++].GetUInt32();

        l_CreatureTemplate->VehicleId      = fields[index++].GetUInt32();
        l_CreatureTemplate->mingold        = fields[index++].GetUInt32();
        l_CreatureTemplate->maxgold        = fields[index++].GetUInt32();
        l_CreatureTemplate->AIName         = fields[index++].GetString();
        l_CreatureTemplate->MovementType   = uint32(fields[index++].GetUInt8());
        l_CreatureTemplate->InhabitType    = uint32(fields[index++].GetUInt8());
        l_CreatureTemplate->HoverHeight    = fields[index++].GetFloat();
        l_CreatureTemplate->ModHealth      = fields[index++].GetFloat();
        l_CreatureTemplate->ModMana        = fields[index++].GetFloat();
        l_CreatureTemplate->ModManaExtra   = fields[index++].GetFloat();
        l_CreatureTemplate->ModArmor       = fields[index++].GetFloat();
        l_CreatureTemplate->RacialLeader   = fields[index++].GetBool();

        for (uint8 i = 0; i < MAX_CREATURE_QUEST_ITEMS; ++i)
            l_CreatureTemplate->questItems[i] = fields[index++].GetUInt32();

        l_CreatureTemplate->movementId         = fields[index++].GetUInt32();
        l_CreatureTemplate->VignetteID         = fields[index++].GetUInt32();
        l_CreatureTemplate->TrackingQuestID    = fields[index++].GetUInt32();
        l_CreatureTemplate->RegenHealth        = fields[index++].GetBool();
        l_CreatureTemplate->MechanicImmuneMask = fields[index++].GetUInt32();
        l_CreatureTemplate->flags_extra        = fields[index++].GetUInt32();
        l_CreatureTemplate->ScriptID           = GetScriptId(fields[index++].GetCString());

        m_CreatureTemplateStore[l_Entry] = l_CreatureTemplate;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadCreatureTemplatesDifficulties()
{
    uint32 l_OldMSTime = getMSTime();

    //                                                  0           1          2
    QueryResult l_Result = WorldDatabase.Query("SELECT entry, CONVERT(difficulty, UNSIGNED), difficulty_entry FROM creature_template_difficulty");

    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature template difficulties definitions. DB table `creature_template_difficulty` is empty.");
        return;
    }

    uint32 l_Count = 0;
    do
    {
        uint8 l_Index = 0;
        Field* l_Fields = l_Result->Fetch();

        uint32 l_Entry           = l_Fields[l_Index++].GetUInt32();
        uint32 l_DifficultyIndex = l_Fields[l_Index++].GetUInt32() - 2;
        uint32 l_DifficultyEntry = l_Fields[l_Index++].GetUInt32();

        if (l_DifficultyIndex >= Difficulty::MaxDifficulties)
            continue;

        CreatureTemplate* l_CreatureTemplate = m_CreatureTemplateStore[l_Entry];
        if (l_CreatureTemplate == nullptr)
            continue;

        l_CreatureTemplate->DifficultyEntry[l_DifficultyIndex] = l_DifficultyEntry;

        ++l_Count;
    }
    while (l_Result->NextRow());

    // Checking needs to be done after loading because of the difficulty self referencing
    for (uint32 l_Entry = 0; l_Entry < m_CreatureTemplateStoreSize; l_Entry++)
    {
        if (CreatureTemplate* l_Creature = m_CreatureTemplateStore[l_Entry])
            CheckCreatureTemplate(l_Creature);
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature difficulties definitions in %u ms", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

void ObjectMgr::LoadCreatureTemplateAddons()
{
    uint32 l_OldMSTime = getMSTime();

    ///                                                0       1       2      3       4       5      6       7
    QueryResult l_Result = WorldDatabase.Query("SELECT entry, path_id, mount, bytes1, bytes2, emote, auras, animkit FROM creature_template_addon");
    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature template addon definitions. DB table `creature_template_addon` is empty.");
        return;
    }

    uint32 l_Count = 0;
    do
    {
        Field* l_Fields = l_Result->Fetch();

        uint32 l_Entry = l_Fields[0].GetUInt32();

        if (!sObjectMgr->GetCreatureTemplate(l_Entry))
            continue;

        CreatureAddon& l_CreatureAddon = _creatureTemplateAddonStore[l_Entry];

        l_CreatureAddon.PathID  = l_Fields[1].GetUInt32();
        l_CreatureAddon.Mount   = l_Fields[2].GetUInt32();
        l_CreatureAddon.Bytes1  = l_Fields[3].GetUInt32();
        l_CreatureAddon.Bytes2  = l_Fields[4].GetUInt32();
        l_CreatureAddon.Emote   = l_Fields[5].GetUInt32();
        l_CreatureAddon.AnimKit = l_Fields[7].GetUInt32();

        Tokenizer l_Tokens(l_Fields[6].GetString(), ' ');
        uint8 l_Index = 0;
        l_CreatureAddon.Auras.resize(l_Tokens.size());
        for (Tokenizer::const_iterator l_Iter = l_Tokens.begin(); l_Iter != l_Tokens.end(); ++l_Iter)
        {
            SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(uint32(atol(*l_Iter)));
            if (!l_SpellInfo)
                continue;

            l_CreatureAddon.Auras[l_Index++] = uint32(atol(*l_Iter));
        }

        if (l_CreatureAddon.Mount)
        {
            if (!sCreatureDisplayInfoStore.LookupEntry(l_CreatureAddon.Mount))
                l_CreatureAddon.Mount = 0;
        }

        if (!sEmotesStore.LookupEntry(l_CreatureAddon.Emote))
            l_CreatureAddon.Emote = 0;

        ++l_Count;
    }
    while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature template addons in %u ms", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

void ObjectMgr::CheckCreatureTemplate(CreatureTemplate const* cInfo)
{
    if (!cInfo)
        return;

    bool ok = true;                                     // bool to allow continue outside this loop
    for (uint32 diff = 0; diff < Difficulty::MaxDifficulties - 1 && ok; ++diff)
    {
        if (!cInfo->DifficultyEntry[diff])
            continue;
        ok = false;                                     // will be set to true at the end of this loop again

        CreatureTemplate const* difficultyInfo = GetCreatureTemplate(cInfo->DifficultyEntry[diff]);
        if (!difficultyInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has `difficulty_entry_%u`=%u but creature entry %u does not exist.",
                cInfo->Entry, diff + 1, cInfo->DifficultyEntry[diff], cInfo->DifficultyEntry[diff]);
            continue;
        }

        bool ok2 = true;
        for (uint32 diff2 = 0; diff2 < Difficulty::MaxDifficulties - 1 && ok2; ++diff2)
        {
            ok2 = false;
            if (_difficultyEntries[diff2].find(cInfo->Entry) != _difficultyEntries[diff2].end())
            {
                sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) is listed as `difficulty_entry_%u` of another creature, but itself lists %u in `difficulty_entry_%u`.",
                    cInfo->Entry, diff2 + 1, cInfo->DifficultyEntry[diff], diff + 1);
                continue;
            }

            /*if (_difficultyEntries[diff2].find(cInfo->DifficultyEntry[diff]) != _difficultyEntries[diff2].end())
            {
                sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) already listed as `difficulty_entry_%u` for another entry.", cInfo->DifficultyEntry[diff], diff2 + 1);
                continue;
            }*/

            if (_hasDifficultyEntries[diff2].find(cInfo->DifficultyEntry[diff]) != _hasDifficultyEntries[diff2].end())
            {
                sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has `difficulty_entry_%u`=%u but creature entry %u has itself a value in `difficulty_entry_%u`.",
                    cInfo->Entry, diff + 1, cInfo->DifficultyEntry[diff], cInfo->DifficultyEntry[diff], diff2 + 1);
                continue;
            }
            ok2 = true;
        }
        if (!ok2)
            continue;

        if (cInfo->unit_class != difficultyInfo->unit_class)
        {
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u, class %u) has different `unit_class` in difficulty %u mode (Entry: %u, class %u).",
                cInfo->Entry, cInfo->unit_class, diff + 1, cInfo->DifficultyEntry[diff], difficultyInfo->unit_class);
            continue;
        }

        if (cInfo->NpcFlags1 != difficultyInfo->NpcFlags1)
        {
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has different `npcflag` in difficulty %u mode (Entry: %u).", cInfo->Entry, diff + 1, cInfo->DifficultyEntry[diff]);
            continue;
        }

        if (cInfo->NpcFlags2 != difficultyInfo->NpcFlags2)
        {
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has different `npcflag2` in difficulty %u mode (Entry: %u).", cInfo->Entry, diff + 1, cInfo->DifficultyEntry[diff]);
            continue;
        }

        if (cInfo->trainer_class != difficultyInfo->trainer_class)
        {
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has different `trainer_class` in difficulty %u mode (Entry: %u).", cInfo->Entry, diff + 1, cInfo->DifficultyEntry[diff]);
            continue;
        }

        if (cInfo->trainer_race != difficultyInfo->trainer_race)
        {
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has different `trainer_race` in difficulty %u mode (Entry: %u).", cInfo->Entry, diff + 1, cInfo->DifficultyEntry[diff]);
            continue;
        }

        if (cInfo->trainer_type != difficultyInfo->trainer_type)
        {
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has different `trainer_type` in difficulty %u mode (Entry: %u).", cInfo->Entry, diff + 1, cInfo->DifficultyEntry[diff]);
            continue;
        }

        if (cInfo->trainer_spell != difficultyInfo->trainer_spell)
        {
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has different `trainer_spell` in difficulty %u mode (Entry: %u).", cInfo->Entry, diff + 1, cInfo->DifficultyEntry[diff]);
            continue;
        }

        if (!difficultyInfo->AIName.empty())
        {
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) lists difficulty %u mode entry %u with `AIName` filled in. `AIName` of difficulty 0 mode creature is always used instead.",
                cInfo->Entry, diff + 1, cInfo->DifficultyEntry[diff]);
            continue;
        }

        if (difficultyInfo->ScriptID)
        {
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) lists difficulty %u mode entry %u with `ScriptName` filled in. `ScriptName` of difficulty 0 mode creature is always used instead.",
                cInfo->Entry, diff + 1, cInfo->DifficultyEntry[diff]);
            continue;
        }

        _hasDifficultyEntries[diff].insert(cInfo->Entry);
        _difficultyEntries[diff].insert(cInfo->DifficultyEntry[diff]);
        ok = true;
    }

    FactionTemplateEntry const* factionTemplate = sFactionTemplateStore.LookupEntry(cInfo->faction);
    if (!factionTemplate)
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has non-existing faction template (%u).", cInfo->Entry, cInfo->faction);

    // used later for scale
    CreatureDisplayInfoEntry const* displayScaleEntry = NULL;

    if (cInfo->Modelid1)
    {
        CreatureDisplayInfoEntry const* displayEntry = sCreatureDisplayInfoStore.LookupEntry(cInfo->Modelid1);
        if (!displayEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) lists non-existing Modelid1 id (%u), this can crash the client.", cInfo->Entry, cInfo->Modelid1);
            const_cast<CreatureTemplate*>(cInfo)->Modelid1 = 0;
        }
        else if (!displayScaleEntry)
            displayScaleEntry = displayEntry;

        CreatureModelInfo const* modelInfo = GetCreatureModelInfo(cInfo->Modelid1);
        if (!modelInfo)
            sLog->outError(LOG_FILTER_SQL, "No model data exist for `Modelid1` = %u listed by creature (Entry: %u).", cInfo->Modelid1, cInfo->Entry);
    }

    if (cInfo->Modelid2)
    {
        CreatureDisplayInfoEntry const* displayEntry = sCreatureDisplayInfoStore.LookupEntry(cInfo->Modelid2);
        if (!displayEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) lists non-existing Modelid2 id (%u), this can crash the client.", cInfo->Entry, cInfo->Modelid2);
            const_cast<CreatureTemplate*>(cInfo)->Modelid2 = 0;
        }
        else if (!displayScaleEntry)
            displayScaleEntry = displayEntry;

        CreatureModelInfo const* modelInfo = GetCreatureModelInfo(cInfo->Modelid2);
        if (!modelInfo)
            sLog->outError(LOG_FILTER_SQL, "No model data exist for `Modelid2` = %u listed by creature (Entry: %u).", cInfo->Modelid2, cInfo->Entry);
    }

    if (cInfo->Modelid3)
    {
        CreatureDisplayInfoEntry const* displayEntry = sCreatureDisplayInfoStore.LookupEntry(cInfo->Modelid3);
        if (!displayEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) lists non-existing Modelid3 id (%u), this can crash the client.", cInfo->Entry, cInfo->Modelid3);
            const_cast<CreatureTemplate*>(cInfo)->Modelid3 = 0;
        }
        else if (!displayScaleEntry)
            displayScaleEntry = displayEntry;

        CreatureModelInfo const* modelInfo = GetCreatureModelInfo(cInfo->Modelid3);
        if (!modelInfo)
            sLog->outError(LOG_FILTER_SQL, "No model data exist for `Modelid3` = %u listed by creature (Entry: %u).", cInfo->Modelid3, cInfo->Entry);
    }

    if (cInfo->Modelid4)
    {
        CreatureDisplayInfoEntry const* displayEntry = sCreatureDisplayInfoStore.LookupEntry(cInfo->Modelid4);
        if (!displayEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) lists non-existing Modelid4 id (%u), this can crash the client.", cInfo->Entry, cInfo->Modelid4);
            const_cast<CreatureTemplate*>(cInfo)->Modelid4 = 0;
        }
        else if (!displayScaleEntry)
            displayScaleEntry = displayEntry;

        CreatureModelInfo const* modelInfo = GetCreatureModelInfo(cInfo->Modelid4);
        if (!modelInfo)
            sLog->outError(LOG_FILTER_SQL, "No model data exist for `Modelid4` = %u listed by creature (Entry: %u).", cInfo->Modelid4, cInfo->Entry);
    }

    if (!displayScaleEntry)
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) does not have any existing display id in Modelid1/Modelid2/Modelid3/Modelid4.", cInfo->Entry);

    for (int k = 0; k < MAX_KILL_CREDIT; ++k)
    {
        if (cInfo->KillCredit[k])
        {
            if (!GetCreatureTemplate(cInfo->KillCredit[k]))
            {
                sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) lists non-existing creature entry %u in `KillCredit%d`.", cInfo->Entry, cInfo->KillCredit[k], k + 1);
                const_cast<CreatureTemplate*>(cInfo)->KillCredit[k] = 0;
            }
        }
    }

    if (!cInfo->unit_class || ((1 << (cInfo->unit_class-1)) & CLASSMASK_ALL_CREATURES) == 0)
    {
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has invalid unit_class (%u) in creature_template. Set to 1 (UNIT_CLASS_WARRIOR).", cInfo->Entry, cInfo->unit_class);
        const_cast<CreatureTemplate*>(cInfo)->unit_class = UNIT_CLASS_WARRIOR;
    }

    if (cInfo->dmgschool >= MAX_SPELL_SCHOOL)
    {
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has invalid spell school value (%u) in `dmgschool`.", cInfo->Entry, cInfo->dmgschool);
        const_cast<CreatureTemplate*>(cInfo)->dmgschool = SPELL_SCHOOL_NORMAL;
    }

    if (cInfo->baseattacktime == 0)
        const_cast<CreatureTemplate*>(cInfo)->baseattacktime  = BASE_ATTACK_TIME;

    if (cInfo->rangeattacktime == 0)
        const_cast<CreatureTemplate*>(cInfo)->rangeattacktime = BASE_ATTACK_TIME;

    if ((cInfo->NpcFlags1 & UNIT_NPC_FLAG_TRAINER) && cInfo->trainer_type >= MAX_TRAINER_TYPE)
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has wrong trainer type %u.", cInfo->Entry, cInfo->trainer_type);

    if (cInfo->type && !sCreatureTypeStore.LookupEntry(cInfo->type))
    {
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has invalid creature type (%u) in `type`.", cInfo->Entry, cInfo->type);
        const_cast<CreatureTemplate*>(cInfo)->type = CREATURE_TYPE_HUMANOID;
    }

    // must exist or used hidden but used in data horse case
    if (cInfo->family && !sCreatureFamilyStore.LookupEntry(cInfo->family) && cInfo->family != CREATURE_FAMILY_HORSE_CUSTOM)
    {
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has invalid creature family (%u) in `family`.", cInfo->Entry, cInfo->family);
        const_cast<CreatureTemplate*>(cInfo)->family = 0;
    }

    if (cInfo->InhabitType <= 0 || cInfo->InhabitType > INHABIT_ANYWHERE)
    {
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has wrong value (%u) in `InhabitType`, creature will not correctly walk/swim/fly.", cInfo->Entry, cInfo->InhabitType);
        const_cast<CreatureTemplate*>(cInfo)->InhabitType = INHABIT_ANYWHERE;
    }

    if (cInfo->HoverHeight < 0.0f)
    {
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has wrong value (%f) in `HoverHeight`", cInfo->Entry, cInfo->HoverHeight);
        const_cast<CreatureTemplate*>(cInfo)->HoverHeight = 1.0f;
    }

    if (cInfo->VehicleId)
    {
        VehicleEntry const* vehId = sVehicleStore.LookupEntry(cInfo->VehicleId);
        if (!vehId)
        {
             sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has a non-existing VehicleId (%u). This *WILL* cause the client to freeze!", cInfo->Entry, cInfo->VehicleId);
             const_cast<CreatureTemplate*>(cInfo)->VehicleId = 0;
        }
    }

    for (uint8 j = 0; j < CREATURE_MAX_SPELLS; ++j)
    {
        if (cInfo->spells[j] && !sSpellMgr->GetSpellInfo(cInfo->spells[j]))
        {
            WorldDatabase.PExecute("UPDATE creature_template SET spell%d = 0 WHERE entry = %u", j+1, cInfo->Entry);
            sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has non-existing Spell%d (%u), set to 0.", cInfo->Entry, j+1, cInfo->spells[j]);
            const_cast<CreatureTemplate*>(cInfo)->spells[j] = 0;
        }
    }

    if (cInfo->MovementType >= MAX_DB_MOTION_TYPE)
    {
        sLog->outError(LOG_FILTER_SQL, "Creature (Entry: %u) has wrong movement generator type (%u), ignored and set to IDLE.", cInfo->Entry, cInfo->MovementType);
        const_cast<CreatureTemplate*>(cInfo)->MovementType = IDLE_MOTION_TYPE;
    }

    /// if not set custom creature scale then load scale from CreatureDisplayInfo.dbc
    if (cInfo->scale <= 0.0f)
    {
        if (displayScaleEntry)
            const_cast<CreatureTemplate*>(cInfo)->scale = displayScaleEntry->scale;
        else
            const_cast<CreatureTemplate*>(cInfo)->scale = 1.0f;
    }

    if (cInfo->expansion > MAX_CREATURE_BASE_HP)
    {
        sLog->outError(LOG_FILTER_SQL, "Table `creature_template` lists creature (Entry: %u) with `exp` %u. Ignored and set to 0.", cInfo->Entry, cInfo->expansion);
        const_cast<CreatureTemplate*>(cInfo)->expansion = 0;
    }

    if (cInfo->expansionUnknown > MAX_CREATURE_BASE_HP)
    {
        sLog->outError(LOG_FILTER_SQL, "Table `creature_template` lists creature (Entry: %u) with `exp_unk` %u. Ignored and set to 0.", cInfo->Entry, cInfo->expansionUnknown);
        const_cast<CreatureTemplate*>(cInfo)->expansionUnknown = 0;
    }

    if (uint32 badFlags = (cInfo->flags_extra & ~CREATURE_FLAG_EXTRA_DB_ALLOWED))
    {
        sLog->outError(LOG_FILTER_SQL, "Table `creature_template` lists creature (Entry: %u) with disallowed `flags_extra` %u, removing incorrect flag.", cInfo->Entry, badFlags);
        const_cast<CreatureTemplate*>(cInfo)->flags_extra &= CREATURE_FLAG_EXTRA_DB_ALLOWED;
    }

    if (cInfo->VignetteID && !sVignetteStore.LookupEntry(cInfo->VignetteID))
    {
        sLog->outError(LOG_FILTER_SQL, "Creature(Entry: %u) has a non - existing vignette id (%u)", cInfo->Entry, cInfo->VignetteID);
        const_cast<CreatureTemplate*>(cInfo)->VignetteID = 0;
    }

    const_cast<CreatureTemplate*>(cInfo)->dmg_multiplier *= Creature::_GetDamageMod(cInfo->rank);
}

void ObjectMgr::LoadCreatureAddons()
{
    uint32 l_OldMSTime = getMSTime();

    ///                                                0      1       2      3       4       5      6       7
    QueryResult l_Result = WorldDatabase.Query("SELECT guid, path_id, mount, bytes1, bytes2, emote, auras, animkit FROM creature_addon");
    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature addon definitions. DB table `creature_addon` is empty.");
        return;
    }

    uint32 l_Count = 0;
    do
    {
        Field* l_Fields = l_Result->Fetch();

        uint32 l_Guid = l_Fields[0].GetUInt32();

        CreatureData const* l_CreatureData = GetCreatureData(l_Guid);
        if (!l_CreatureData)
            continue;

        CreatureAddon& l_CreatureAddon = _creatureAddonStore[l_Guid];

        l_CreatureAddon.PathID = l_Fields[1].GetUInt32();
        if (l_CreatureData->movementType == WAYPOINT_MOTION_TYPE && !l_CreatureAddon.PathID)
            const_cast<CreatureData*>(l_CreatureData)->movementType = IDLE_MOTION_TYPE;

        l_CreatureAddon.Mount   = l_Fields[2].GetUInt32();
        l_CreatureAddon.Bytes1  = l_Fields[3].GetUInt32();
        l_CreatureAddon.Bytes2  = l_Fields[4].GetUInt32();
        l_CreatureAddon.Emote   = l_Fields[5].GetUInt32();
        l_CreatureAddon.AnimKit = l_Fields[7].GetUInt32();

        Tokenizer l_Tokens(l_Fields[6].GetString(), ' ');
        uint8 l_Index = 0;
        l_CreatureAddon.Auras.resize(l_Tokens.size());
        for (Tokenizer::const_iterator l_Iter = l_Tokens.begin(); l_Iter != l_Tokens.end(); ++l_Iter)
        {
            SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(uint32(atol(*l_Iter)));
            if (!l_SpellInfo)
                continue;

            l_CreatureAddon.Auras[l_Index++] = uint32(atol(*l_Iter));
        }

        if (l_CreatureAddon.Mount)
        {
            if (!sCreatureDisplayInfoStore.LookupEntry(l_CreatureAddon.Mount))
                l_CreatureAddon.Mount = 0;
        }

        if (!sEmotesStore.LookupEntry(l_CreatureAddon.Emote))
            l_CreatureAddon.Emote = 0;

        ++l_Count;
    }
    while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature addons in %u ms", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

CreatureAddon const* ObjectMgr::GetCreatureAddon(uint32 lowguid)
{
    CreatureAddonContainer::const_iterator itr = _creatureAddonStore.find(lowguid);
    if (itr != _creatureAddonStore.end())
        return &(itr->second);

    return NULL;
}

CreatureAddon const* ObjectMgr::GetCreatureTemplateAddon(uint32 entry)
{
    CreatureAddonContainer::const_iterator itr = _creatureTemplateAddonStore.find(entry);
    if (itr != _creatureTemplateAddonStore.end())
        return &(itr->second);

    return NULL;
}

EquipmentInfo const* ObjectMgr::GetEquipmentInfo(uint32 p_Entry, int8& p_ID)
{
    EquipmentInfoContainer::const_iterator itr = _equipmentInfoStore.find(p_Entry);
    if (itr == _equipmentInfoStore.end())
        return NULL;

    if (itr->second.empty())
        return NULL;

    if (p_ID == -1) // Select a random element
    {
        EquipmentInfoContainerInternal::const_iterator l_Iter = itr->second.begin();
        std::advance(l_Iter, urand(0, itr->second.size() - 1));
        p_ID = std::distance(itr->second.begin(), l_Iter) + 1;
        return &l_Iter->second;
    }
    else
    {
        EquipmentInfoContainerInternal::const_iterator l_Iter = itr->second.find(p_ID);
        if (l_Iter != itr->second.end())
            return &l_Iter->second;
    }

    return NULL;
}

void ObjectMgr::LoadEquipmentTemplates()
{
    uint32 oldMSTime = getMSTime();

    //                                                 0     1       2           3           4
    QueryResult result = WorldDatabase.Query("SELECT entry, id, itemEntry1, itemEntry2, itemEntry3 FROM creature_equip_template");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature equipment templates. DB table `creature_equip_template` is empty!");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        uint8 l_Index = 0;

        uint32 entry = fields[l_Index++].GetUInt32();

        if (!sObjectMgr->GetCreatureTemplate(entry))
        {
            sLog->outError(LOG_FILTER_SQL, "Creature template (Entry: %u) does not exist but has a record in `creature_equip_template`", entry);
            continue;
        }

        uint8 id = fields[l_Index++].GetUInt8();
        if (!id)
        {
            sLog->outError(LOG_FILTER_SQL, "Creature equipment template with ID 0 found for creature %u, skipped.", entry);
            continue;
        }

        EquipmentInfo& equipmentInfo = _equipmentInfoStore[entry][id];

        equipmentInfo.ItemEntry[0] = fields[l_Index++].GetUInt32();
        equipmentInfo.ItemEntry[1] = fields[l_Index++].GetUInt32();
        equipmentInfo.ItemEntry[2] = fields[l_Index++].GetUInt32();

        for (uint8 i = 0; i < MAX_EQUIPMENT_ITEMS; ++i)
        {
            if (!equipmentInfo.ItemEntry[i])
                continue;

            ItemEntry const* dbcItem = sItemStore.LookupEntry(equipmentInfo.ItemEntry[i]);

            if (!dbcItem)
            {
                sLog->outError(LOG_FILTER_SQL, "Unknown item (entry=%u) in creature_equip_template.itemEntry%u for entry = %u and id=%u, forced to 0.",
                    equipmentInfo.ItemEntry[i], i + 1, entry, id);
                equipmentInfo.ItemEntry[i] = 0;
                continue;
            }

            if (dbcItem->InventoryType != INVTYPE_WEAPON &&
                dbcItem->InventoryType != INVTYPE_SHIELD &&
                dbcItem->InventoryType != INVTYPE_RANGED &&
                dbcItem->InventoryType != INVTYPE_2HWEAPON &&
                dbcItem->InventoryType != INVTYPE_WEAPONMAINHAND &&
                dbcItem->InventoryType != INVTYPE_WEAPONOFFHAND &&
                dbcItem->InventoryType != INVTYPE_HOLDABLE &&
                dbcItem->InventoryType != INVTYPE_THROWN &&
                dbcItem->InventoryType != INVTYPE_RANGEDRIGHT)
            {
                sLog->outError(LOG_FILTER_SQL, "Item (entry=%u) in creature_equip_template.itemEntry%u for entry = %u and id = %u is not equipable in a hand, forced to 0.",
                    equipmentInfo.ItemEntry[i], i + 1, entry, id);
                equipmentInfo.ItemEntry[i] = 0;
            }
        }

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u equipment templates in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

CreatureModelInfo const* ObjectMgr::GetCreatureModelInfo(uint32 modelId)
{
    CreatureModelContainer::const_iterator itr = _creatureModelStore.find(modelId);
    if (itr != _creatureModelStore.end())
        return &(itr->second);

    return NULL;
}

uint32 ObjectMgr::ChooseDisplayId(uint32 /*team*/, const CreatureTemplate* cinfo, const CreatureData* data /*= NULL*/)
{
    // Load creature model (display id)
    uint32 display_id = 0;

    if (!data || data->displayid == 0)
    {
        display_id = cinfo->GetRandomValidModelId();
    }
    else
        return data->displayid;

    return display_id;
}

void ObjectMgr::ChooseCreatureFlags(const CreatureTemplate* p_CreatureTemplate, uint32 & p_NpcFlags1, uint32 & p_NpcFlags2, uint32 & p_UnitFlags1, uint32 & p_UnitFlags2, uint32 & p_UnitFlags3, uint32 & p_Dynamicflags, const CreatureData* p_Data)
{
    p_NpcFlags1     = p_CreatureTemplate->NpcFlags1;
    p_NpcFlags2     = p_CreatureTemplate->NpcFlags2;
    p_UnitFlags1    = p_CreatureTemplate->UnitFlags1;
    p_UnitFlags2    = p_CreatureTemplate->UnitFlags2;
    p_UnitFlags3    = p_CreatureTemplate->UnitFlags3;
    p_Dynamicflags  = p_CreatureTemplate->dynamicflags;

    if (p_Data)
    {
        if (p_Data->NpcFlags1)
            p_NpcFlags1 = p_Data->NpcFlags1;

        if (p_Data->NpcFlags2)
            p_NpcFlags2 = p_Data->NpcFlags2;

        if (p_Data->UnitFlags1)
            p_UnitFlags1 = p_Data->UnitFlags1;

        if (p_Data->UnitFlags2)
            p_UnitFlags2 = p_Data->UnitFlags2;

        if (p_Data->UnitFlags3)
            p_UnitFlags3 = p_Data->UnitFlags3;

        if (p_Data->dynamicflags)
            p_Dynamicflags = p_Data->dynamicflags;
    }
}

CreatureModelInfo const* ObjectMgr::GetCreatureModelRandomGender(uint32* displayID)
{
    CreatureModelInfo const* modelInfo = GetCreatureModelInfo(*displayID);
    if (!modelInfo)
        return NULL;

    // If a model for another gender exists, 50% chance to use it
    if (modelInfo->modelid_other_gender != 0 && urand(0, 1) == 0)
    {
        CreatureModelInfo const* minfo_tmp = GetCreatureModelInfo(modelInfo->modelid_other_gender);
        if (!minfo_tmp)
            sLog->outError(LOG_FILTER_SQL, "Model (Entry: %u) has modelid_other_gender %u not found in table `creature_model_info`. ", *displayID, modelInfo->modelid_other_gender);
        else
        {
            // Model ID changed
            *displayID = modelInfo->modelid_other_gender;
            return minfo_tmp;
        }
    }

    return modelInfo;
}

void ObjectMgr::LoadCreatureModelInfo()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT modelid, bounding_radius, combat_reach, gender, modelid_other_gender FROM creature_model_info");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature model definitions. DB table `creature_model_info` is empty.");
        return;
    }

    //_creatureModelStore.rehash(result->GetRowCount());
    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 modelId = fields[0].GetUInt32();

        CreatureModelInfo& modelInfo = _creatureModelStore[modelId];

        modelInfo.bounding_radius      = fields[1].GetFloat();
        modelInfo.combat_reach         = fields[2].GetFloat();
        modelInfo.gender               = fields[3].GetUInt8();
        modelInfo.modelid_other_gender = fields[4].GetUInt32();

        // Checks

        if (!sCreatureDisplayInfoStore.LookupEntry(modelId))
            sLog->outError(LOG_FILTER_SQL, "Table `creature_model_info` has model for not existed display id (%u).", modelId);

        if (modelInfo.gender > GENDER_NONE)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature_model_info` has wrong gender (%u) for display id (%u).", uint32(modelInfo.gender), modelId);
            modelInfo.gender = GENDER_MALE;
        }

        if (modelInfo.modelid_other_gender && !sCreatureDisplayInfoStore.LookupEntry(modelInfo.modelid_other_gender))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature_model_info` has not existed alt.gender model (%u) for existed display id (%u).", modelInfo.modelid_other_gender, modelId);
            modelInfo.modelid_other_gender = 0;
        }

        if (modelInfo.combat_reach < 0.1f)
            modelInfo.combat_reach = DEFAULT_COMBAT_REACH;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature model based info in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadLinkedRespawn()
{
    uint32 oldMSTime = getMSTime();

    _linkedRespawnStore.clear();
    //                                                 0        1          2
    QueryResult result = WorldDatabase.Query("SELECT guid, linkedGuid, linkType FROM linked_respawn ORDER BY guid ASC");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 linked respawns. DB table `linked_respawn` is empty.");

        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 guidLow = fields[0].GetUInt32();
        uint32 linkedGuidLow = fields[1].GetUInt32();
        uint8  linkType = fields[2].GetUInt8();

        uint64 guid = 0, linkedGuid = 0;
        bool error = false;
        switch (linkType)
        {
            case CREATURE_TO_CREATURE:
            {
                const CreatureData* slave = GetCreatureData(guidLow);
                if (!slave)
                {
                    sLog->outError(LOG_FILTER_SQL, "Couldn't get creature data for GUIDLow %u", guidLow);
                    error = true;
                    break;
                }

                const CreatureData* master = GetCreatureData(linkedGuidLow);
                if (!master)
                {
                    sLog->outError(LOG_FILTER_SQL, "Couldn't get creature data for GUIDLow %u", linkedGuidLow);
                    error = true;
                    break;
                }

                const MapEntry* const map = sMapStore.LookupEntry(master->mapid);
                if (!map || !map->Instanceable() || (master->mapid != slave->mapid))
                {
                    sLog->outError(LOG_FILTER_SQL, "Creature '%u' linking to '%u' on an unpermitted map.", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                if (!(master->spawnMask & slave->spawnMask))  // they must have a possibility to meet (normal/heroic difficulty)
                {
                    sLog->outError(LOG_FILTER_SQL, "LinkedRespawn: Creature '%u' linking to '%u' with not corresponding spawnMask", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                guid = MAKE_NEW_GUID(guidLow, slave->id, HIGHGUID_UNIT);
                linkedGuid = MAKE_NEW_GUID(linkedGuidLow, master->id, HIGHGUID_UNIT);
                break;
            }
            case CREATURE_TO_GO:
            {
                const CreatureData* slave = GetCreatureData(guidLow);
                if (!slave)
                {
                    sLog->outError(LOG_FILTER_SQL, "Couldn't get creature data for GUIDLow %u", guidLow);
                    error = true;
                    break;
                }

                const GameObjectData* master = GetGOData(linkedGuidLow);
                if (!master)
                {
                    sLog->outError(LOG_FILTER_SQL, "Couldn't get gameobject data for GUIDLow %u", linkedGuidLow);
                    error = true;
                    break;
                }

                const MapEntry* const map = sMapStore.LookupEntry(master->mapid);
                if (!map || !map->Instanceable() || (master->mapid != slave->mapid))
                {
                    sLog->outError(LOG_FILTER_SQL, "Creature '%u' linking to '%u' on an unpermitted map.", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                if (!(master->spawnMask & slave->spawnMask))  // they must have a possibility to meet (normal/heroic difficulty)
                {
                    sLog->outError(LOG_FILTER_SQL, "LinkedRespawn: Creature '%u' linking to '%u' with not corresponding spawnMask", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                guid = MAKE_NEW_GUID(guidLow, slave->id, HIGHGUID_UNIT);
                linkedGuid = MAKE_NEW_GUID(linkedGuidLow, master->id, HIGHGUID_GAMEOBJECT);
                break;
            }
            case GO_TO_GO:
            {
                const GameObjectData* slave = GetGOData(guidLow);
                if (!slave)
                {
                    sLog->outError(LOG_FILTER_SQL, "Couldn't get gameobject data for GUIDLow %u", guidLow);
                    error = true;
                    break;
                }

                const GameObjectData* master = GetGOData(linkedGuidLow);
                if (!master)
                {
                    sLog->outError(LOG_FILTER_SQL, "Couldn't get gameobject data for GUIDLow %u", linkedGuidLow);
                    error = true;
                    break;
                }

                const MapEntry* const map = sMapStore.LookupEntry(master->mapid);
                if (!map || !map->Instanceable() || (master->mapid != slave->mapid))
                {
                    sLog->outError(LOG_FILTER_SQL, "Creature '%u' linking to '%u' on an unpermitted map.", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                if (!(master->spawnMask & slave->spawnMask))  // they must have a possibility to meet (normal/heroic difficulty)
                {
                    sLog->outError(LOG_FILTER_SQL, "LinkedRespawn: Creature '%u' linking to '%u' with not corresponding spawnMask", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                guid = MAKE_NEW_GUID(guidLow, slave->id, HIGHGUID_GAMEOBJECT);
                linkedGuid = MAKE_NEW_GUID(linkedGuidLow, master->id, HIGHGUID_GAMEOBJECT);
                break;
            }
            case GO_TO_CREATURE:
            {
                const GameObjectData* slave = GetGOData(guidLow);
                if (!slave)
                {
                    sLog->outError(LOG_FILTER_SQL, "Couldn't get gameobject data for GUIDLow %u", guidLow);
                    error = true;
                    break;
                }

                const CreatureData* master = GetCreatureData(linkedGuidLow);
                if (!master)
                {
                    sLog->outError(LOG_FILTER_SQL, "Couldn't get creature data for GUIDLow %u", linkedGuidLow);
                    error = true;
                    break;
                }

                const MapEntry* const map = sMapStore.LookupEntry(master->mapid);
                if (!map || !map->Instanceable() || (master->mapid != slave->mapid))
                {
                    sLog->outError(LOG_FILTER_SQL, "Creature '%u' linking to '%u' on an unpermitted map.", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                if (!(master->spawnMask & slave->spawnMask))  // they must have a possibility to meet (normal/heroic difficulty)
                {
                    sLog->outError(LOG_FILTER_SQL, "LinkedRespawn: Creature '%u' linking to '%u' with not corresponding spawnMask", guidLow, linkedGuidLow);
                    error = true;
                    break;
                }

                guid = MAKE_NEW_GUID(guidLow, slave->id, HIGHGUID_GAMEOBJECT);
                linkedGuid = MAKE_NEW_GUID(linkedGuidLow, master->id, HIGHGUID_UNIT);
                break;
            }
        }

        if (!error)
            _linkedRespawnStore[guid] = linkedGuid;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded " UI64FMTD " linked respawns in %u ms", uint64(_linkedRespawnStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

bool ObjectMgr::SetCreatureLinkedRespawn(uint32 guidLow, uint32 linkedGuidLow)
{
    if (!guidLow)
        return false;

    const CreatureData* master = GetCreatureData(guidLow);
    uint64 guid = MAKE_NEW_GUID(guidLow, master->id, HIGHGUID_UNIT);

    if (!linkedGuidLow) // we're removing the linking
    {
        _linkedRespawnStore.erase(guid);
        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_DEL_CRELINKED_RESPAWN);
        stmt->setUInt32(0, guidLow);
        WorldDatabase.Execute(stmt);
        return true;
    }

    const CreatureData* slave = GetCreatureData(linkedGuidLow);

    const MapEntry* const map = sMapStore.LookupEntry(master->mapid);
    if (!map || !map->Instanceable() || (master->mapid != slave->mapid))
    {
        sLog->outError(LOG_FILTER_SQL, "Creature '%u' linking to '%u' on an unpermitted map.", guidLow, linkedGuidLow);
        return false;
    }

    if (!(master->spawnMask & slave->spawnMask))  // they must have a possibility to meet (normal/heroic difficulty)
    {
        sLog->outError(LOG_FILTER_SQL, "LinkedRespawn: Creature '%u' linking to '%u' with not corresponding spawnMask", guidLow, linkedGuidLow);
        return false;
    }

    uint64 linkedGuid = MAKE_NEW_GUID(linkedGuidLow, slave->id, HIGHGUID_UNIT);

    _linkedRespawnStore[guid] = linkedGuid;
    PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_REP_CREATURE_LINKED_RESPAWN);
    stmt->setUInt32(0, guidLow);
    stmt->setUInt32(1, linkedGuidLow);
    WorldDatabase.Execute(stmt);
    return true;
}

void ObjectMgr::LoadTempSummons()
{
    uint32 oldMSTime = getMSTime();

    //                                               0           1             2        3      4           5           6           7            8           9
    QueryResult result = WorldDatabase.Query("SELECT summonerId, summonerType, groupId, entry, position_x, position_y, position_z, orientation, summonType, summonTime FROM creature_summon_groups");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 temp summons. DB table `creature_summon_groups` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 summonerId               = fields[0].GetUInt32();
        SummonerType summonerType       = SummonerType(fields[1].GetUInt8());
        uint8 group                     = fields[2].GetUInt8();

        switch (summonerType)
        {
            case SUMMONER_TYPE_CREATURE:
                if (!GetCreatureTemplate(summonerId))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `creature_summon_groups` has summoner with non existing entry %u for creature summoner type, skipped.", summonerId);
                    continue;
                }
                break;
            case SUMMONER_TYPE_GAMEOBJECT:
                if (!GetGameObjectTemplate(summonerId))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `creature_summon_groups` has summoner with non existing entry %u for gameobject summoner type, skipped.", summonerId);
                    continue;
                }
                break;
            case SUMMONER_TYPE_MAP:
                if (!sMapStore.LookupEntry(summonerId))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `creature_summon_groups` has summoner with non existing entry %u for map summoner type, skipped.", summonerId);
                    continue;
                }
                break;
            default:
                sLog->outError(LOG_FILTER_SQL, "Table `creature_summon_groups` has unhandled summoner type %u for summoner %u, skipped.", summonerType, summonerId);
                continue;
        }

        TempSummonData data;
        data.entry                      = fields[3].GetUInt32();

        if (!GetCreatureTemplate(data.entry))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature_summon_groups` has creature in group [Summoner ID: %u, Summoner Type: %u, Group ID: %u] with non existing creature entry %u, skipped.", summonerId, summonerType, group, data.entry);
            continue;
        }

        float posX                      = fields[4].GetFloat();
        float posY                      = fields[5].GetFloat();
        float posZ                      = fields[6].GetFloat();
        float orientation               = fields[7].GetFloat();

        data.pos.Relocate(posX, posY, posZ, orientation);

        data.type                       = TempSummonType(fields[8].GetUInt8());

        if (data.type > TEMPSUMMON_MANUAL_DESPAWN)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature_summon_groups` has unhandled temp summon type %u in group [Summoner ID: %u, Summoner Type: %u, Group ID: %u] for creature entry %u, skipped.", data.type, summonerId, summonerType, group, data.entry);
            continue;
        }

        data.time                       = fields[9].GetUInt32();

        TempSummonGroupKey key(summonerId, summonerType, group);
        _tempSummonDataStore[key].push_back(data);

        ++count;

    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u temp summons in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadCreatures()
{
    uint32 oldMSTime = getMSTime();

    //                                               0              1   2       3      4       5           6           7           8            9            10            11          12
    std::string l_Query = "SELECT creature.guid, id, map, zoneId, areaId, modelid, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, "
        //        13            14         15       16            17         18         19          20            21                22                      23                  24                     25                     26                      27                   28
        "currentwaypoint, curhealth, curmana, MovementType, spawnMask, phaseMask, eventEntry, pool_entry, creature.npcflag, creature.npcflag2, creature.unit_flags, creature.unit_flags2,  creature.unit_flags3, creature.dynamicflags, creature.WorldEffectID, creature.isActive "
        "FROM creature "
        "LEFT OUTER JOIN game_event_creature ON creature.guid = game_event_creature.guid "
        "LEFT OUTER JOIN pool_creature ON creature.guid = pool_creature.guid";


    if (sWorld->getBoolConfig(CONFIG_ENABLE_ONLY_SPECIFIC_MAP))
    {
        std::vector<uint32> l_MapsToLoad = sWorld->GetMapsToLoad();
        std::string l_TempQueryEnding = l_Query += " WHERE creature.map IN (";
        uint8 l_Counter = 0;

        for (uint32 l_MapID : l_MapsToLoad)
        {
            l_TempQueryEnding += std::to_string(l_MapID);
            ++l_Counter;

            if (l_Counter < l_MapsToLoad.size())
                l_TempQueryEnding += ", ";
            else
                l_TempQueryEnding += ");";
        }
    }

    QueryResult result = WorldDatabase.Query(l_Query.c_str());

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creatures. DB table `creature` is empty.");

        return;
    }

    // Build single time for check spawnmask
    std::map<uint32, uint32> spawnMasks;
    for (uint32 i = 0; i < sMapStore.GetNumRows(); ++i)
        if (sMapStore.LookupEntry(i))
            for (int k = 0; k < Difficulty::MaxDifficulties; ++k)
                if (GetMapDifficultyData(i, Difficulty(k)))
                    spawnMasks[i] |= (1 << k);

    //_creatureDataStore.rehash(result->GetRowCount());
    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint8 index = 0;

        uint32 guid         = fields[index++].GetUInt32();
        uint32 entry        = fields[index++].GetUInt32();

        CreatureTemplate const* cInfo = GetCreatureTemplate(entry);
        if (!cInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature` has creature (GUID: %u) with non existing creature entry %u, skipped.", guid, entry);
            continue;
        }

        CreatureData& data = _creatureDataStore[guid];
        data.id             = entry;
        data.mapid          = fields[index++].GetUInt16();
        data.zoneId         = fields[index++].GetUInt16();
        data.areaId         = fields[index++].GetUInt16();
        data.displayid      = fields[index++].GetUInt32();
        data.equipmentId    = fields[index++].GetInt8();
        data.posX           = fields[index++].GetFloat();
        data.posY           = fields[index++].GetFloat();
        data.posZ           = fields[index++].GetFloat();
        data.orientation    = fields[index++].GetFloat();
        data.spawntimesecs  = fields[index++].GetUInt32();
        data.spawndist      = fields[index++].GetFloat();
        data.currentwaypoint= fields[index++].GetUInt32();
        data.curhealth      = fields[index++].GetUInt32();
        data.curmana        = fields[index++].GetUInt32();
        data.movementType   = fields[index++].GetUInt8();
        data.spawnMask      = fields[index++].GetUInt32();
        data.phaseMask      = fields[index++].GetUInt32();
        int16 gameEvent     = fields[index++].GetInt8();
        uint32 PoolId       = fields[index++].GetUInt32();
        data.NpcFlags1      = fields[index++].GetUInt32();
        data.NpcFlags2      = fields[index++].GetUInt32();
        data.UnitFlags1     = fields[index++].GetUInt32();
        data.UnitFlags2     = fields[index++].GetUInt32();
        data.UnitFlags3     = fields[index++].GetUInt32();
        data.dynamicflags   = fields[index++].GetUInt32();
        data.WorldEffectID  = fields[index++].GetUInt32();
        data.isActive       = fields[index++].GetBool();

        MapEntry const* mapEntry = sMapStore.LookupEntry(data.mapid);
        if (!mapEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature` have creature (GUID: %u) that spawned at not existed map (Id: %u), skipped.", guid, data.mapid);
            continue;
        }

        if (data.spawnMask & ~spawnMasks[data.mapid])
            sLog->outError(LOG_FILTER_SQL, "Table `creature` have creature (GUID: %u) that have wrong spawn mask %u including not supported difficulty modes for map (Id: %u) spawnMasks[data.mapid]: %u.", guid, data.spawnMask, data.mapid, spawnMasks[data.mapid]);

        bool ok = true;
        for (uint32 diff = 0; diff < Difficulty::MaxDifficulties - 1 && ok; ++diff)
        {
            if (_difficultyEntries[diff].find(data.id) != _difficultyEntries[diff].end())
            {
                sLog->outError(LOG_FILTER_SQL, "Table `creature` have creature (GUID: %u) that listed as difficulty %u template (entry: %u) in `creature_template`, skipped.",
                    guid, diff + 1, data.id);
                ok = false;
            }
        }
        if (!ok)
            continue;

        // -1 random, 0 no equipment,
        if (data.equipmentId != 0)
        {
            if (!GetEquipmentInfo(data.id, data.equipmentId))
            {
                sLog->outError(LOG_FILTER_SQL, "Table `creature` have creature (Entry: %u) with equipment_id %u not found in table `creature_equip_template`, set to no equipment.", data.id, data.equipmentId);
                data.equipmentId = 0;
            }
        }

        if (cInfo->flags_extra & CREATURE_FLAG_EXTRA_INSTANCE_BIND)
        {
            if (!mapEntry || !mapEntry->IsDungeon())
                sLog->outError(LOG_FILTER_SQL, "Table `creature` have creature (GUID: %u Entry: %u) with `creature_template`.`flags_extra` including CREATURE_FLAG_EXTRA_INSTANCE_BIND but creature are not in instance.", guid, data.id);
        }

        if (data.spawndist < 0.0f)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature` have creature (GUID: %u Entry: %u) with `spawndist`< 0, set to 0.", guid, data.id);
            data.spawndist = 0.0f;
        }
        else if (data.movementType == RANDOM_MOTION_TYPE)
        {
            if (data.spawndist == 0.0f)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `creature` have creature (GUID: %u Entry: %u) with `MovementType`=1 (random movement) but with `spawndist`=0, replace by idle movement type (0).", guid, data.id);
                data.movementType = IDLE_MOTION_TYPE;
            }
        }
        else if (data.movementType == IDLE_MOTION_TYPE)
        {
            if (data.spawndist != 0.0f)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `creature` have creature (GUID: %u Entry: %u) with `MovementType`=0 (idle) have `spawndist`<>0, set to 0.", guid, data.id);
                data.spawndist = 0.0f;
            }
        }

        if (data.phaseMask == 0)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature` have creature (GUID: %u Entry: %u) with `phaseMask`=0 (not visible for anyone), set to 1.", guid, data.id);
            data.phaseMask = 1;
        }

        // Add to grid if not managed by the game event or pool system
        if (gameEvent == 0 && PoolId == 0)
            AddCreatureToGrid(guid, &data);

        if (!data.zoneId || !data.areaId)
        {
            uint32 zoneId = 0;
            uint32 areaId = 0;

            sMapMgr->GetZoneAndAreaId(zoneId, areaId, data.mapid, data.posX, data.posY, data.posZ);
            WorldDatabase.PExecute("UPDATE creature SET zoneId = %u, areaId = %u WHERE guid = %u", zoneId, areaId, guid);
        }
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creatures in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::AddCreatureToGrid(uint32 guid, CreatureData const* data)
{
    uint32 mask = data->spawnMask;
    for (uint32 i = 0; mask != 0; i++, mask >>= 1)
    {
        if (mask & 1)
        {
            CellCoord cellCoord = JadeCore::ComputeCellCoord(data->posX, data->posY);
            CellObjectGuids& cell_guids = _mapObjectGuidsStore[MAKE_PAIR32(data->mapid, i)][cellCoord.GetId()];
            cell_guids.creatures.insert(guid);
        }
    }
}

void ObjectMgr::RemoveCreatureFromGrid(uint32 guid, CreatureData const* data)
{
    uint32 mask = data->spawnMask;
    for (uint32 i = 0; mask != 0; i++, mask >>= 1)
    {
        if (mask & 1)
        {
            CellCoord cellCoord = JadeCore::ComputeCellCoord(data->posX, data->posY);
            CellObjectGuids& cell_guids = _mapObjectGuidsStore[MAKE_PAIR32(data->mapid, i)][cellCoord.GetId()];
            cell_guids.creatures.erase(guid);
        }
    }
}

bool ObjectMgr::AddGOData(uint32 p_LowGuid, uint32 entry, uint32 mapId, float x, float y, float z, float o, uint32 spawntimedelay, float rotation0, float rotation1, float rotation2, float rotation3)
{
    GameObjectTemplate const* goinfo = GetGameObjectTemplate(entry);
    if (!goinfo)
        return false;

    Map* map = sMapMgr->CreateBaseMap(mapId);
    if (!map)
        return false;

    GameObjectData& data = NewGOData(p_LowGuid);
    data.id             = entry;
    data.mapid          = mapId;
    data.posX           = x;
    data.posY           = y;
    data.posZ           = z;
    data.orientation    = o;
    data.rotation0      = rotation0;
    data.rotation1      = rotation1;
    data.rotation2      = rotation2;
    data.rotation3      = rotation3;
    data.spawntimesecs  = spawntimedelay;
    data.animprogress   = 100;
    data.spawnMask      = 1;
    data.go_state       = GO_STATE_READY;
    data.phaseMask      = PHASEMASK_NORMAL;
    data.artKit         = goinfo->type == GAMEOBJECT_TYPE_CONTROL_ZONE ? 21 : 0;
    data.dbData = false;

    AddGameobjectToGrid(p_LowGuid, &data);

    // Spawn if necessary (loaded grids only)
    // We use spawn coords to spawn
    if (!map->Instanceable() && map->IsGridLoaded(x, y))
    {
        GameObject* go = new GameObject;
        if (!go->LoadGameObjectFromDB(p_LowGuid, map))
        {
            sLog->outError(LOG_FILTER_GENERAL, "AddGOData: cannot add gameobject entry %u to map", entry);
            delete go;
            return false;
        }
    }

    sLog->outDebug(LOG_FILTER_MAPS, "AddGOData: dbguid %u entry %u map %u x %f y %f z %f o %f", p_LowGuid, entry, mapId, x, y, z, o);

    return true;
}

bool ObjectMgr::MoveCreData(uint32 guid, uint32 mapId, Position pos)
{
    CreatureData& data = NewOrExistCreatureData(guid);
    if (!data.id)
        return false;

    RemoveCreatureFromGrid(guid, &data);
    if (data.posX == pos.GetPositionX() && data.posY == pos.GetPositionY() && data.posZ == pos.GetPositionZ())
        return true;
    data.posX = pos.GetPositionX();
    data.posY = pos.GetPositionY();
    data.posZ = pos.GetPositionZ();
    data.orientation = pos.GetOrientation();
    AddCreatureToGrid(guid, &data);

    // Spawn if necessary (loaded grids only)
    if (Map* map = sMapMgr->CreateBaseMap(mapId))
    {
        // We use spawn coords to spawn
        if (!map->Instanceable() && map->IsGridLoaded(data.posX, data.posY))
        {
            Creature* creature = new Creature;
            if (!creature->LoadCreatureFromDB(guid, map))
            {
                sLog->outError(LOG_FILTER_GENERAL, "AddCreature: cannot add creature entry %u to map", guid);
                delete creature;
                return false;
            }
        }
    }
    return true;
}

uint32 ObjectMgr::AddCreData(uint32 entry, uint32 /*team*/, uint32 mapId, float x, float y, float z, float o, uint32 spawntimedelay)
{
    CreatureTemplate const* l_CreatureTemplate = GetCreatureTemplate(entry);
    if (!l_CreatureTemplate)
        return 0;

    uint32 level = l_CreatureTemplate->minlevel == l_CreatureTemplate->maxlevel ? l_CreatureTemplate->minlevel : urand(l_CreatureTemplate->minlevel, l_CreatureTemplate->maxlevel); // Only used for extracting creature base stats
    CreatureBaseStats const* stats = GetCreatureBaseStats(level, l_CreatureTemplate->unit_class);

    uint32 guid = GenerateLowGuid(HIGHGUID_UNIT);

    CreatureData& l_Data = NewOrExistCreatureData(guid);

    l_Data.id = entry;
    l_Data.mapid = mapId;
    l_Data.displayid = 0;

    if (_equipmentInfoStore.find(entry) != _equipmentInfoStore.end())
        l_Data.equipmentId = 1; // Assuming equipmentId is 1
    else
        l_Data.equipmentId = 0;

    l_Data.posX = x;
    l_Data.posY = y;
    l_Data.posZ = z;
    l_Data.orientation = o;
    l_Data.spawntimesecs = spawntimedelay;
    l_Data.spawndist = 0;
    l_Data.currentwaypoint = 0;
    l_Data.curhealth = stats->GenerateHealth(l_CreatureTemplate);
    l_Data.curmana = stats->GenerateMana(l_CreatureTemplate);
    l_Data.movementType = l_CreatureTemplate->MovementType;
    l_Data.spawnMask = 1;
    l_Data.phaseMask = PHASEMASK_NORMAL;
    l_Data.dbData = false;
    l_Data.NpcFlags1      = l_CreatureTemplate->NpcFlags1;
    l_Data.NpcFlags2      = l_CreatureTemplate->NpcFlags2;
    l_Data.UnitFlags1     = l_CreatureTemplate->UnitFlags1;
    l_Data.UnitFlags2     = l_CreatureTemplate->UnitFlags2;
    l_Data.UnitFlags3     = l_CreatureTemplate->UnitFlags3;
    l_Data.dynamicflags   = l_CreatureTemplate->dynamicflags;
    l_Data.WorldEffectID  = l_CreatureTemplate->WorldEffectID;

    AddCreatureToGrid(guid, &l_Data);

    // Spawn if necessary (loaded grids only)
    if (Map* map = sMapMgr->CreateBaseMap(mapId))
    {
        // We use spawn coords to spawn
        if (!map->Instanceable() && !map->IsRemovalGrid(x, y))
        {
            Creature* creature = new Creature;
            if (!creature->LoadCreatureFromDB(guid, map))
            {
                sLog->outError(LOG_FILTER_GENERAL, "AddCreature: cannot add creature entry %u to map", entry);
                delete creature;
                return 0;
            }
        }
    }

    return guid;
}

void ObjectMgr::LoadGameobjects()
{
    uint32 oldMSTime = getMSTime();

    uint32 count = 0;

    //                                                0                1   2    3         4           5           6        7           8
    std::string l_Query = "SELECT gameobject.guid, id, map, zoneId, areaId, position_x, position_y, position_z, orientation, "
        //      9          10         11          12         13          14             15      16         17         18        19          20
        "rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state, isActive, spawnMask, phaseMask, eventEntry, pool_entry "
        "FROM gameobject LEFT OUTER JOIN game_event_gameobject ON gameobject.guid = game_event_gameobject.guid "
        "LEFT OUTER JOIN pool_gameobject ON gameobject.guid = pool_gameobject.guid";

    if (sWorld->getBoolConfig(CONFIG_ENABLE_ONLY_SPECIFIC_MAP))
    {
        std::vector<uint32> l_MapsToLoad = sWorld->GetMapsToLoad();
        std::string l_TempQueryEnding = l_Query += " WHERE gameobject.map IN (";
        uint8 l_Counter = 0;

        for (uint32 l_MapID : l_MapsToLoad)
        {
            l_TempQueryEnding += std::to_string(l_MapID);
            ++l_Counter;

            if (l_Counter < l_MapsToLoad.size())
                l_TempQueryEnding += ", ";
            else
                l_TempQueryEnding += ");";
        }
    }

    QueryResult result = WorldDatabase.Query(l_Query.c_str());

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 gameobjects. DB table `gameobject` is empty.");
        return;
    }

    // build single time for check spawnmask
    std::map<uint32, uint32> spawnMasks;
    for (uint32 i = 0; i < sMapStore.GetNumRows(); ++i)
        if (sMapStore.LookupEntry(i))
            for (int k = 0; k < Difficulty::MaxDifficulties; ++k)
                if (GetMapDifficultyData(i, Difficulty(k)))
                    spawnMasks[i] |= (1 << k);

    //_gameObjectDataStore.rehash(result->GetRowCount());
    do
    {
        Field* fields = result->Fetch();

        uint32 guid         = fields[0].GetUInt32();
        uint32 entry        = fields[1].GetUInt32();

        GameObjectTemplate const* gInfo = GetGameObjectTemplate(entry);
        if (!gInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject` has gameobject (GUID: %u) with non existing gameobject entry %u, skipped.", guid, entry);
            continue;
        }

        if (!gInfo->displayId)
        {
            switch (gInfo->type)
            {
                case GAMEOBJECT_TYPE_TRAP:
                case GAMEOBJECT_TYPE_SPELL_FOCUS:
                    break;
                default:
                    sLog->outError(LOG_FILTER_SQL, "Gameobject (GUID: %u Entry %u GoType: %u) doesn't have a displayId (%u), not loaded.", guid, entry, gInfo->type, gInfo->displayId);
                    break;
            }
        }

        if (gInfo->displayId && !sGameObjectDisplayInfoStore.LookupEntry(gInfo->displayId))
        {
            sLog->outError(LOG_FILTER_SQL, "Gameobject (GUID: %u Entry %u GoType: %u) has an invalid displayId (%u), not loaded.", guid, entry, gInfo->type, gInfo->displayId);
            continue;
        }

        GameObjectData& data = _gameObjectDataStore[guid];

        data.id             = entry;
        data.mapid          = fields[2].GetUInt16();
        data.zoneId         = fields[3].GetUInt16();
        data.areaId         = fields[4].GetUInt16();
        data.posX           = fields[5].GetFloat();
        data.posY           = fields[6].GetFloat();
        data.posZ           = fields[7].GetFloat();
        data.orientation    = fields[8].GetFloat();
        data.rotation0      = fields[9].GetFloat();
        data.rotation1      = fields[10].GetFloat();
        data.rotation2      = fields[11].GetFloat();
        data.rotation3      = fields[12].GetFloat();
        data.spawntimesecs  = fields[13].GetInt32();

        MapEntry const* mapEntry = sMapStore.LookupEntry(data.mapid);
        if (!mapEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject` has gameobject (GUID: %u Entry: %u) spawned on a non-existed map (Id: %u), skip", guid, data.id, data.mapid);
            continue;
        }

        if (!data.zoneId || !data.areaId)
        {
            uint32 zoneId = 0;
            uint32 areaId = 0;

            sMapMgr->GetZoneAndAreaId(zoneId, areaId, data.mapid, data.posX, data.posY, data.posZ);
            WorldDatabase.PExecute("UPDATE gameobject SET zoneId = %u, areaId = %u WHERE guid = %u", zoneId, areaId, guid);
        }

        if (data.spawntimesecs == 0 && gInfo->IsDespawnAtAction())
        {
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject` has gameobject (GUID: %u Entry: %u) with `spawntimesecs` (0) value, but the gameobejct is marked as despawnable at action.", guid, data.id);
        }

        data.animprogress   = fields[14].GetUInt8();
        data.artKit         = 0;

        uint32 go_state     = fields[15].GetUInt8();
        if (go_state >= MAX_GO_STATE)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject` has gameobject (GUID: %u Entry: %u) with invalid `state` (%u) value, skip", guid, data.id, go_state);
            continue;
        }

        data.go_state       = GOState(go_state);

        data.isActive       = fields[16].GetBool();
        data.CustomFlags    = fields[17].GetUInt32();
        data.spawnMask      = fields[18].GetUInt32();

        if (data.spawnMask & ~spawnMasks[data.mapid])
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject` has gameobject (GUID: %u Entry: %u) that has wrong spawn mask %u including not supported difficulty modes for map (Id: %u), skip", guid, data.id, data.spawnMask, data.mapid);

        data.phaseMask      = fields[19].GetUInt32();
        int16 gameEvent     = fields[20].GetInt8();
        uint32 PoolId        = fields[21].GetUInt32();

        if (data.rotation2 < -1.0f || data.rotation2 > 1.0f)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject` has gameobject (GUID: %u Entry: %u) with invalid rotation2 (%f) value, skip", guid, data.id, data.rotation2);
            continue;
        }

        if (data.rotation3 < -1.0f || data.rotation3 > 1.0f)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject` has gameobject (GUID: %u Entry: %u) with invalid rotation3 (%f) value, skip", guid, data.id, data.rotation3);
            continue;
        }

        if (!MapManager::IsValidMapCoord(data.mapid, data.posX, data.posY, data.posZ, data.orientation))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject` has gameobject (GUID: %u Entry: %u) with invalid coordinates, skip", guid, data.id);
            continue;
        }

        if (data.phaseMask == 0)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject` has gameobject (GUID: %u Entry: %u) with `phaseMask`=0 (not visible for anyone), set to 1.", guid, data.id);
            data.phaseMask = 1;
        }

        if (gameEvent == 0 && PoolId == 0)                      // if not this is to be managed by GameEvent System or Pool system
            AddGameobjectToGrid(guid, &data);
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %lu gameobjects in %u ms", (unsigned long)_gameObjectDataStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::AddGameobjectToGrid(uint32 guid, GameObjectData const* data)
{
    uint32 mask = data->spawnMask;
    for (uint32 i = 0; mask != 0; i++, mask >>= 1)
    {
        if (mask & 1)
        {
            CellCoord cellCoord = JadeCore::ComputeCellCoord(data->posX, data->posY);
            CellObjectGuids& cell_guids = _mapObjectGuidsStore[MAKE_PAIR32(data->mapid, i)][cellCoord.GetId()];
            cell_guids.gameobjects.insert(guid);
        }
    }
}

void ObjectMgr::RemoveGameobjectFromGrid(uint32 guid, GameObjectData const* data)
{
    uint32 mask = data->spawnMask;
    for (uint32 i = 0; mask != 0; i++, mask >>= 1)
    {
        if (mask & 1)
        {
            CellCoord cellCoord = JadeCore::ComputeCellCoord(data->posX, data->posY);
            CellObjectGuids& cell_guids = _mapObjectGuidsStore[MAKE_PAIR32(data->mapid, i)][cellCoord.GetId()];
            cell_guids.gameobjects.erase(guid);
        }
    }
}

Player* ObjectMgr::GetPlayerByLowGUID(uint32 lowguid) const
{
    uint64 guid = MAKE_NEW_GUID(lowguid, 0, HIGHGUID_PLAYER);
    return ObjectAccessor::FindPlayer(guid);
}

// name must be checked to correctness (if received) before call this function
uint64 ObjectMgr::GetPlayerGUIDByName(std::string name) const
{
    uint64 guid = 0;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_BY_NAME);

    stmt->setString(0, name);

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
        guid = MAKE_NEW_GUID((*result)[0].GetUInt32(), 0, HIGHGUID_PLAYER);

    return guid;
}

bool ObjectMgr::GetPlayerNameByGUID(uint64 guid, std::string &name) const
{
    // prevent DB access for online player
    if (Player* player = ObjectAccessor::FindPlayer(guid))
    {
        name = player->GetName();
        return true;
    }

    if (CharacterInfo const* l_CharacterInfo = sWorld->GetCharacterInfo(guid))
    {
        name = l_CharacterInfo->Name;
        return true;
    }

    return false;
}

uint32 ObjectMgr::GetPlayerTeamByGUID(uint64 guid) const
{
    if (CharacterInfo const* l_CharacterInfo = sWorld->GetCharacterInfo(guid))
        return Player::TeamForRace(l_CharacterInfo->Race);

    return 0;
}

uint32 ObjectMgr::GetPlayerAccountIdByGUID(uint64 guid) const
{
    if (CharacterInfo const* l_CharacterInfo = sWorld->GetCharacterInfo(guid))
        return l_CharacterInfo->AccountId;

    return 0;
}

uint32 ObjectMgr::GetPlayerAccountIdByPlayerName(const std::string& name) const
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_BY_NAME);

    stmt->setString(0, name);

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
    {
        uint32 acc = (*result)[0].GetUInt32();
        return acc;
    }

    return 0;
}

void ObjectMgr::LoadRealmCompletedChallenges()
{
    uint32 l_OldMSTime = getMSTime();
    uint32 l_Count = 0;

    QueryResult l_Result = CharacterDatabase.Query("SELECT map_id, attempt_id, completion_time, completion_date, medal_earned, group_members, group_1_guid, group_1_spec, group_2_guid, group_2_spec, "
                                               "group_3_guid, group_3_spec, group_4_guid, group_4_spec, group_5_guid, group_5_spec FROM group_completed_challenges");
    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 group completed challenges. DB table `group_completed_challenges` is empty.");
        return;
    }

    do
    {
        uint32 l_Index = 0;
        Field* l_Fields = l_Result->Fetch();
        uint32 l_MapID = l_Fields[l_Index++].GetUInt32();

        RealmCompletedChallenge& l_RealmChallenge = m_GroupsCompletedChallenges[l_MapID];

        l_RealmChallenge.m_GuildID = 0;
        l_RealmChallenge.m_AttemptID = l_Fields[l_Index++].GetUInt32();
        l_RealmChallenge.m_CompletionTime = l_Fields[l_Index++].GetUInt32();
        l_RealmChallenge.m_CompletionDate = l_Fields[l_Index++].GetUInt32();
        l_RealmChallenge.m_MedalEarned = l_Fields[l_Index++].GetUInt8();
        l_RealmChallenge.m_MembersCount = l_Fields[l_Index++].GetUInt8();

        for (uint8 l_I = 0; l_I < 5; ++l_I)
        {
            l_RealmChallenge.m_Members[l_I].m_Guid = MAKE_NEW_GUID(l_Fields[l_Index++].GetUInt32(), 0, HIGHGUID_PLAYER);
            l_RealmChallenge.m_Members[l_I].m_SpecID = l_Fields[l_Index++].GetUInt32();
        }

        ++l_Count;
    }
    while (l_Result->NextRow());

    l_Result = CharacterDatabase.Query("SELECT map_id, guild_id, attempt_id, completion_time, completion_date, medal_earned, guild_members, guild_1_guid, guild_1_spec, guild_2_guid, guild_2_spec, "
                                   "guild_3_guid, guild_3_spec, guild_4_guid, guild_4_spec, guild_5_guid, guild_5_spec FROM guild_completed_challenges");
    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 guild completed challenges. DB table `guild_completed_challenges` is empty.");
        return;
    }

    do
    {
        uint32 l_Index = 0;
        Field* l_Fields = l_Result->Fetch();
        uint32 l_MapID = l_Fields[l_Index++].GetUInt32();

        RealmCompletedChallenge& l_RealmChallenge = m_GuildsCompletedChallenges[l_MapID];

        l_RealmChallenge.m_GuildID = l_Fields[l_Index++].GetUInt32();
        l_RealmChallenge.m_AttemptID = l_Fields[l_Index++].GetUInt32();
        l_RealmChallenge.m_CompletionTime = l_Fields[l_Index++].GetUInt32();
        l_RealmChallenge.m_CompletionDate = l_Fields[l_Index++].GetUInt32();
        l_RealmChallenge.m_MedalEarned = l_Fields[l_Index++].GetUInt8();
        l_RealmChallenge.m_MembersCount = l_Fields[l_Index++].GetUInt8();

        for (uint8 l_I = 0; l_I < 5; ++l_I)
        {
            l_RealmChallenge.m_Members[l_I].m_Guid = MAKE_NEW_GUID(l_Fields[l_Index++].GetUInt32(), 0, HIGHGUID_PLAYER);
            l_RealmChallenge.m_Members[l_I].m_SpecID = l_Fields[l_Index++].GetUInt32();
        }

        ++l_Count;
    }
    while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u realm completed challenges in %u ms", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

void ObjectMgr::LoadChallengeRewards()
{
    uint32 l_OldMSTime = getMSTime();
    uint32 l_Count = 0;

    QueryResult l_Result = WorldDatabase.Query("SELECT map_id, none_money, bronze_money, silver_money, gold_money, title, gold_achievement FROM challenge_mode_rewards");
    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 challenge rewards. DB table `challenge_mode_rewards` is empty.");
        return;
    }

    do
    {
        uint32 l_Index = 0;
        Field* l_Fields = l_Result->Fetch();
        uint32 l_MapID = l_Fields[l_Index++].GetUInt32();

        ChallengeReward& l_Rewards = m_ChallengeRewardsMap[l_MapID];

        l_Rewards.MapID = l_MapID;

        for (uint8 l_I = 0; l_I < 4; ++l_I)
            l_Rewards.MoneyReward[l_I] = l_Fields[l_Index++].GetUInt32();

        l_Rewards.TitleID       = l_Fields[l_Index++].GetUInt32();
        l_Rewards.AchievementID = l_Fields[l_Index++].GetUInt32();

        ++l_Count;
    }
    while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u challenge mode rewards in %u ms", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

void FillItemDamageFields(float* minDamage, float* maxDamage, float* dps, uint32 itemLevel, uint32 itemClass, uint32 itemSubClass, uint32 quality, uint32 delay, float statScalingFactor, uint32 inventoryType, uint32 flags2)
{
    *minDamage = *maxDamage = *dps = 0.0f;
    if (itemClass != ITEM_CLASS_WEAPON || quality > ITEM_QUALITY_ARTIFACT)
        return;

    DBCStorage<ItemDamageEntry>* store = NULL;
    // get the right store here
    if (inventoryType > 0xD + 13)
        return;

    switch (inventoryType)
    {
        case INVTYPE_AMMO:
            store = &sItemDamageAmmoStore;
            break;
        case INVTYPE_2HWEAPON:
            if (flags2 & ITEM_FLAG2_CASTER_WEAPON)
                store = &sItemDamageTwoHandCasterStore;
            else
                store = &sItemDamageTwoHandStore;
            break;
        case INVTYPE_RANGED:
        case INVTYPE_THROWN:
        case INVTYPE_RANGEDRIGHT:
            switch (itemSubClass)
            {
                case ITEM_SUBCLASS_WEAPON_WAND:
                    store = &sItemDamageWandStore;
                    break;
                case ITEM_SUBCLASS_WEAPON_THROWN:
                    store = &sItemDamageThrownStore;
                    break;
                case ITEM_SUBCLASS_WEAPON_BOW:
                case ITEM_SUBCLASS_WEAPON_GUN:
                case ITEM_SUBCLASS_WEAPON_CROSSBOW:
                    store = &sItemDamageRangedStore;
                    break;
                default:
                    return;
            }
            break;
        case INVTYPE_WEAPON:
        case INVTYPE_WEAPONMAINHAND:
        case INVTYPE_WEAPONOFFHAND:
            if (flags2 & ITEM_FLAG2_CASTER_WEAPON)
                store = &sItemDamageOneHandCasterStore;
            else
                store = &sItemDamageOneHandStore;
            break;
        default:
            return;
    }

    if (!store)
        return;

    ItemDamageEntry const* damageInfo = store->LookupEntry(itemLevel);
    if (!damageInfo)
        return;

    *dps = damageInfo->DPS[quality];
    float avgDamage = *dps * delay * 0.001f;
    *minDamage = (statScalingFactor * -0.5f + 1.0f) * avgDamage;
    *maxDamage = floor(float(avgDamage * (statScalingFactor * 0.5f + 1.0f) + 0.5f));
}

uint32 FillItemArmor(uint32 itemlevel, uint32 itemClass, uint32 itemSubclass, uint32 quality, uint32 inventoryType)
{
    if (quality > ITEM_QUALITY_ARTIFACT)
        return 0;

    if (itemClass != ITEM_CLASS_ARMOR)
        return 0;

    // all items but shields
    if (itemSubclass != ITEM_SUBCLASS_ARMOR_SHIELD)
    {
        ItemArmorQualityEntry const* armorQuality = sItemArmorQualityStore.LookupEntry(itemlevel);
        ItemArmorTotalEntry const* armorTotal = sItemArmorTotalStore.LookupEntry(itemlevel);
        if (!armorQuality || !armorTotal)
            return 0;

        if (inventoryType == INVTYPE_ROBE)
            inventoryType = INVTYPE_CHEST;

        ArmorLocationEntry const* location = sArmorLocationStore.LookupEntry(inventoryType);
        if (!location)
            return 0;

        if (itemSubclass < ITEM_SUBCLASS_ARMOR_CLOTH)
            return 0;

        return uint32(armorQuality->Value[quality] * armorTotal->Value[itemSubclass - 1] * location->Value[itemSubclass - 1] + 0.5f);
    }

    // shields
    ItemArmorShieldEntry const* shield = sItemArmorShieldStore.LookupEntry(itemlevel);
    if (!shield)
        return 0;

    return uint32(shield->Value[quality] + 0.5f);
}

uint32 FillMaxDurability(uint32 itemClass, uint32 itemSubClass, uint32 inventoryType, uint32 quality, uint32 itemLevel)
{
    if (itemClass != ITEM_CLASS_ARMOR && itemClass != ITEM_CLASS_WEAPON)
        return 0;

    static float const qualityMultipliers[MAX_ITEM_QUALITY] =
    {
        1.0f, 1.0f, 1.0f, 1.17f, 1.37f, 1.68f, 0.0f, 0.0f
    };

    static float const armorMultipliers[MAX_INVTYPE] =
    {
        0.00f, // INVTYPE_NON_EQUIP
        0.59f, // INVTYPE_HEAD
        0.00f, // INVTYPE_NECK
        0.59f, // INVTYPE_SHOULDERS
        0.00f, // INVTYPE_BODY
        1.00f, // INVTYPE_CHEST
        0.35f, // INVTYPE_WAIST
        0.75f, // INVTYPE_LEGS
        0.49f, // INVTYPE_FEET
        0.35f, // INVTYPE_WRISTS
        0.35f, // INVTYPE_HANDS
        0.00f, // INVTYPE_FINGER
        0.00f, // INVTYPE_TRINKET
        0.00f, // INVTYPE_WEAPON
        1.00f, // INVTYPE_SHIELD
        0.00f, // INVTYPE_RANGED
        0.00f, // INVTYPE_CLOAK
        0.00f, // INVTYPE_2HWEAPON
        0.00f, // INVTYPE_BAG
        0.00f, // INVTYPE_TABARD
        1.00f, // INVTYPE_ROBE
        0.00f, // INVTYPE_WEAPONMAINHAND
        0.00f, // INVTYPE_WEAPONOFFHAND
        0.00f, // INVTYPE_HOLDABLE
        0.00f, // INVTYPE_AMMO
        0.00f, // INVTYPE_THROWN
        0.00f, // INVTYPE_RANGEDRIGHT
        0.00f, // INVTYPE_QUIVER
        0.00f, // INVTYPE_RELIC
    };

    static float const weaponMultipliers[MAX_ITEM_SUBCLASS_WEAPON] =
    {
        0.89f, // ITEM_SUBCLASS_WEAPON_AXE
        1.03f, // ITEM_SUBCLASS_WEAPON_AXE2
        0.77f, // ITEM_SUBCLASS_WEAPON_BOW
        0.77f, // ITEM_SUBCLASS_WEAPON_GUN
        0.89f, // ITEM_SUBCLASS_WEAPON_MACE
        1.03f, // ITEM_SUBCLASS_WEAPON_MACE2
        1.03f, // ITEM_SUBCLASS_WEAPON_POLEARM
        0.89f, // ITEM_SUBCLASS_WEAPON_SWORD
        1.03f, // ITEM_SUBCLASS_WEAPON_SWORD2
        0.00f, // ITEM_SUBCLASS_WEAPON_Obsolete
        1.03f, // ITEM_SUBCLASS_WEAPON_STAFF
        0.00f, // ITEM_SUBCLASS_WEAPON_EXOTIC
        0.00f, // ITEM_SUBCLASS_WEAPON_EXOTIC2
        0.64f, // ITEM_SUBCLASS_WEAPON_FIST_WEAPON
        0.00f, // ITEM_SUBCLASS_WEAPON_MISCELLANEOUS
        0.64f, // ITEM_SUBCLASS_WEAPON_DAGGER
        0.64f, // ITEM_SUBCLASS_WEAPON_THROWN
        0.00f, // ITEM_SUBCLASS_WEAPON_SPEAR
        0.77f, // ITEM_SUBCLASS_WEAPON_CROSSBOW
        0.64f, // ITEM_SUBCLASS_WEAPON_WAND
        0.64f, // ITEM_SUBCLASS_WEAPON_FISHING_POLE
    };

    float levelPenalty = 1.0f;
    if (itemLevel <= 28)
        levelPenalty = 0.966f - float(28u - itemLevel) / 54.0f;

    if (itemClass == ITEM_CLASS_ARMOR)
    {
        if (inventoryType > INVTYPE_ROBE)
            return 0;

        return 5 * uint32(23.0f * qualityMultipliers[quality] * armorMultipliers[inventoryType] * levelPenalty + 0.5f);
    }

    return 5 * uint32(17.0f * qualityMultipliers[quality] * weaponMultipliers[itemSubClass] * levelPenalty + 0.5f);
};

void FillDisenchantFields(uint32* disenchantID, uint32* requiredDisenchantSkill, ItemTemplate const& itemTemplate)
{
    *disenchantID = 0;
    *(int32*)requiredDisenchantSkill = -1;
    if ((itemTemplate.Flags & (ITEM_FLAG_CONJURED | ITEM_FLAG_UNK6)) ||
        itemTemplate.Bonding == BIND_QUEST_ITEM || itemTemplate.Area || itemTemplate.Map ||
        itemTemplate.Stackable > 1 ||
        itemTemplate.Quality < ITEM_QUALITY_UNCOMMON || itemTemplate.Quality > ITEM_QUALITY_EPIC ||
        !(itemTemplate.Class == ITEM_CLASS_ARMOR || itemTemplate.Class == ITEM_CLASS_WEAPON) ||
        !(Item::GetSpecialPrice(&itemTemplate) || sItemCurrencyCostStore.LookupEntry(itemTemplate.ItemId)))
        return;

    for (uint32 i = 0; i < sItemDisenchantLootStore.GetNumRows(); ++i)
    {
        ItemDisenchantLootEntry const* disenchant = sItemDisenchantLootStore.LookupEntry(i);
        if (!disenchant)
            continue;

        if (disenchant->ItemClass == itemTemplate.Class &&
            disenchant->ItemQuality == itemTemplate.Quality &&
            disenchant->MinItemLevel <= itemTemplate.ItemLevel &&
            disenchant->MaxItemLevel >= itemTemplate.ItemLevel)
        {
            if (disenchant->Id == 60 || disenchant->Id == 61)   // epic item disenchant ilvl range 66-99 (classic)
            {
                if (itemTemplate.RequiredLevel > 60 || itemTemplate.RequiredSkillRank > 300)
                    continue;                                   // skip to epic item disenchant ilvl range 90-199 (TBC)
            }
            else if (disenchant->Id == 66 || disenchant->Id == 67)  // epic item disenchant ilvl range 90-199 (TBC)
            {
                if (itemTemplate.RequiredLevel <= 60 || (itemTemplate.RequiredSkill && itemTemplate.RequiredSkillRank <= 300))
                    continue;
            }

            *disenchantID = disenchant->Id;
            *requiredDisenchantSkill = disenchant->RequiredDisenchantSkill;
            return;
        }
    }
}

void ObjectMgr::LoadItemTemplates()
{
    uint32 oldMSTime = getMSTime();
    uint32 sparseCount = 0;

    for (uint32 itemId = 0; itemId < sItemSparseStore.GetNumRows(); ++itemId)
    {
        ItemSparseEntry const* sparse = sItemSparseStore.LookupEntry(itemId);
        ItemEntry const* db2Data = sItemStore.LookupEntry(itemId);
        if (!sparse || !db2Data)
            continue;

        ItemTemplate& itemTemplate = _itemTemplateStore[itemId];

        itemTemplate.ItemId = itemId;
        itemTemplate.Class = db2Data->Class;
        itemTemplate.SubClass = db2Data->SubClass;
        itemTemplate.SoundOverrideSubclass = db2Data->SoundOverrideSubclass;
        itemTemplate.Name1 = sparse->Name;
        itemTemplate.DisplayInfoID = db2Data->DisplayId;

        auto l_Itr = g_ItemTemplateDisplayIDs.find(itemId);
        if (l_Itr != g_ItemTemplateDisplayIDs.end())
            itemTemplate.DisplayInfoID = l_Itr->second;

        itemTemplate.Quality = sparse->Quality;
        itemTemplate.Flags = sparse->Flags;
        itemTemplate.Flags2 = sparse->Flags2;
        itemTemplate.Flags3 = sparse->Flags3;
        itemTemplate.Unk430_1 = sparse->Unk430_1;
        itemTemplate.Unk430_2 = sparse->Unk430_2;
        itemTemplate.BuyCount = std::max(sparse->BuyCount, 1u);
        itemTemplate.BuyPrice = sparse->BuyPrice;
        itemTemplate.SellPrice = sparse->SellPrice;
        itemTemplate.InventoryType = db2Data->InventoryType;
        itemTemplate.AllowableClass = sparse->AllowableClass;
        itemTemplate.AllowableRace = sparse->AllowableRace;
        itemTemplate.ItemLevel = sparse->ItemLevel;
        itemTemplate.RequiredLevel = sparse->RequiredLevel;
        itemTemplate.RequiredSkill = sparse->RequiredSkill;
        itemTemplate.RequiredSkillRank = sparse->RequiredSkillRank;
        itemTemplate.RequiredSpell = sparse->RequiredSpell;
        itemTemplate.RequiredHonorRank = sparse->RequiredHonorRank;
        itemTemplate.RequiredCityRank = sparse->RequiredCityRank;
        itemTemplate.RequiredReputationFaction = sparse->RequiredReputationFaction;
        itemTemplate.RequiredReputationRank = sparse->RequiredReputationRank;
        itemTemplate.MaxCount = sparse->MaxCount;
        itemTemplate.Stackable = sparse->Stackable;
        itemTemplate.ContainerSlots = sparse->ContainerSlots;

        for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
        {
            itemTemplate.ItemStat[i].ItemStatType = sparse->ItemStatType[i];
            itemTemplate.ItemStat[i].ItemStatValue = sparse->ItemStatValue[i];
            itemTemplate.ItemStat[i].ScalingValue = sparse->ScalingValue[i];
            itemTemplate.ItemStat[i].SocketCostRate = sparse->SocketCostRate[i];
        }

        itemTemplate.ScalingStatDistribution = sparse->ScalingStatDistribution;

        // cache item damage
        FillItemDamageFields(&itemTemplate.DamageMin, &itemTemplate.DamageMax, &itemTemplate.DPS, sparse->ItemLevel,
                             db2Data->Class, db2Data->SubClass, sparse->Quality, sparse->Delay, sparse->StatScalingFactor,
                             sparse->InventoryType, sparse->Flags2);

        itemTemplate.DamageType = sparse->DamageType;
        itemTemplate.Armor = FillItemArmor(sparse->ItemLevel, db2Data->Class, db2Data->SubClass, sparse->Quality, sparse->InventoryType);
        itemTemplate.Delay = sparse->Delay;
        itemTemplate.RangedModRange = sparse->RangedModRange;

        std::vector<uint32> & l_EffectsIndex = sItemEffectsByItemID[itemId];

        ASSERT(l_EffectsIndex.size() < MAX_ITEM_PROTO_SPELLS);

        memset(&itemTemplate.Spells[0], 0, sizeof(_Spell)* MAX_ITEM_PROTO_SPELLS);

        for (uint32 l_I = 0; l_I < l_EffectsIndex.size(); ++l_I)
        {
            const ItemEffectEntry* l_Entry = sItemEffectStore.LookupEntry(l_EffectsIndex[l_I]);
            if (!l_Entry)
                continue;

            itemTemplate.Spells[l_Entry->EffectIndex].SpellId               = l_Entry->SpellID;
            itemTemplate.Spells[l_Entry->EffectIndex].SpellTrigger          = l_Entry->SpellTrigger;
            itemTemplate.Spells[l_Entry->EffectIndex].SpellCharges          = l_Entry->SpellCharge;
            itemTemplate.Spells[l_Entry->EffectIndex].SpellCooldown         = l_Entry->SpellCooldown;
            itemTemplate.Spells[l_Entry->EffectIndex].SpellCategory         = l_Entry->SpellCategory;
            itemTemplate.Spells[l_Entry->EffectIndex].SpellCategoryCooldown = l_Entry->SpellCategoryCooldown;
        }

        itemTemplate.SpellPPMRate = 0.0f;
        itemTemplate.Bonding = sparse->Bonding;
        itemTemplate.Description = sparse->Description;
        itemTemplate.PageText = sparse->PageText;
        itemTemplate.LanguageID = sparse->LanguageID;
        itemTemplate.PageMaterial = sparse->PageMaterial;
        itemTemplate.StartQuest = sparse->StartQuest;
        itemTemplate.LockID = sparse->LockID;
        itemTemplate.Material = sparse->Material;
        itemTemplate.Sheath = sparse->Sheath;
        itemTemplate.RandomProperty = sparse->RandomProperty;
        itemTemplate.RandomSuffix = sparse->RandomSuffix;
        itemTemplate.ItemSet = sparse->ItemSet;
        itemTemplate.MaxDurability = FillMaxDurability(db2Data->Class, db2Data->SubClass, sparse->InventoryType, sparse->Quality, sparse->ItemLevel);
        itemTemplate.Area = sparse->Area;
        itemTemplate.Map = sparse->Map;
        itemTemplate.BagFamily = sparse->BagFamily;
        itemTemplate.TotemCategory = sparse->TotemCategory;
        for (uint32 i = 0; i < MAX_ITEM_PROTO_SOCKETS; ++i)
        {
            itemTemplate.Socket[i].Color = sparse->Color[i];
            itemTemplate.Socket[i].Content = 0;//sparse->Content[i];
        }

        itemTemplate.socketBonus = sparse->SocketBonus;
        itemTemplate.GemProperties = sparse->GemProperties;
        FillDisenchantFields(&itemTemplate.DisenchantID, &itemTemplate.RequiredDisenchantSkill, itemTemplate);

        itemTemplate.ArmorDamageModifier = sparse->ArmorDamageModifier;
        itemTemplate.Duration = sparse->Duration;
        itemTemplate.ItemLimitCategory = sparse->ItemLimitCategory;
        itemTemplate.HolidayId = sparse->HolidayId;
        itemTemplate.StatScalingFactor = sparse->StatScalingFactor;
        itemTemplate.CurrencySubstitutionId = sparse->CurrencySubstitutionId;
        itemTemplate.CurrencySubstitutionCount = sparse->CurrencySubstitutionCount;
        itemTemplate.ItemNameDescriptionID = sparse->ItemNameDescriptionID;
        itemTemplate.ScriptId = 0;
        itemTemplate.FoodType = 0;
        itemTemplate.MinMoneyLoot = 0;
        itemTemplate.MaxMoneyLoot = 0;
        itemTemplate.FlagsCu = 0;

        auto l_Iter = g_PvPItemStoreLevels.find(itemId);
        if (l_Iter != g_PvPItemStoreLevels.end())
            itemTemplate.PvPScalingLevel = l_Iter->second;
        else
            itemTemplate.PvPScalingLevel = 0;
        ++sparseCount;
    }

    // Check if item templates for DBC referenced character start outfit are present
    std::set<uint32> notFoundOutfit;
    for (uint32 i = 1; i < sCharStartOutfitStore.GetNumRows(); ++i)
    {
        CharStartOutfitEntry const* entry = sCharStartOutfitStore.LookupEntry(i);
        if (!entry)
            continue;

        for (int j = 0; j < MAX_OUTFIT_ITEMS; ++j)
        {
            if (entry->ItemId[j] <= 0)
                continue;

            uint32 item_id = entry->ItemId[j];

            if (!GetItemTemplate(item_id))
                notFoundOutfit.insert(item_id);
        }
    }

    for (std::set<uint32>::const_iterator itr = notFoundOutfit.begin(); itr != notFoundOutfit.end(); ++itr)
        sLog->outError(LOG_FILTER_SQL, "Item (Entry: %u) does not exist in `item_template` but is referenced in `CharStartOutfit.dbc`", *itr);

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u item templates from Item-sparse.db2 in %u ms", sparseCount, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadItemTemplateCorrections()
{
    for (ItemTemplateContainer::const_iterator l_Iter = _itemTemplateStore.begin(); l_Iter != _itemTemplateStore.end(); l_Iter++)
    {
        ItemTemplate& l_ItemTemplate = const_cast<ItemTemplate&>(l_Iter->second);

        switch (l_ItemTemplate.ItemId)
        {
            // Prevent people opening strongboxed they cant use & abuse some other stuff
            case 120354: ///< Gold Strongbox A
            case 120355: ///< Silver Strongbox A
            case 120356: ///< Bronze Strongbox A
            case 120353: ///< Steel Strongbox A
            case 118065: ///< Gleaming Ashmaul Strongbox (A)
            case 126905: ///< Steel Strongbox A - S2
            case 126908: ///< Bronze Strongbox A - S2
            case 126907: ///< Silver Strongbox A - S2
            case 126906: ///< Gold Strongbox A - S2
            case 126919: ///< Champion's Strongbox A (RBG)
                l_ItemTemplate.Flags2 |= ITEM_FLAG2_ALLIANCE_ONLY;
                l_ItemTemplate.RequiredLevel = 100;
                break;
            case 111598: ///< Gold Strongbox H
            case 111599: ///< Silver strongbox H
            case 111600: ///< Bronze Strongbox H
            case 119330: ///< Steel StrongBox H
            case 120151: ///< Gleaming Ashmaul Strongbox (H)
            case 126904: ///< Steel Strongbox H - S2
            case 126903: ///< Bronze Strongbox H - S2
            case 126902: ///< Silver Strongbox H - S2
            case 126901: ///< Gold Strongbox H - S2
            case 126920: ///< Champion's Strongbox H (RBG)
                l_ItemTemplate.Flags2 |= ITEM_FLAG2_HORDE_ONLY;
                l_ItemTemplate.RequiredLevel = 100;
                break;
        }
    }
}

void ObjectMgr::LoadItemTemplateAddon()
{
    uint32 oldMSTime = getMSTime();
    uint32 count = 0;

    QueryResult result = WorldDatabase.Query("SELECT Id, FlagsCu, FoodType, MinMoneyLoot, MaxMoneyLoot, SpellPPMChance FROM item_template_addon");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 itemId = fields[0].GetUInt32();
            if (!GetItemTemplate(itemId))
            {
                sLog->outError(LOG_FILTER_SQL, "Item %u specified in `item_template_addon` does not exist, skipped.", itemId);
                continue;
            }

            uint32 minMoneyLoot = fields[3].GetUInt32();
            uint32 maxMoneyLoot = fields[4].GetUInt32();
            if (minMoneyLoot > maxMoneyLoot)
            {
                sLog->outError(LOG_FILTER_SQL, "Minimum money loot specified in `item_template_addon` for item %u was greater than maximum amount, swapping.", itemId);
                std::swap(minMoneyLoot, maxMoneyLoot);
            }
            ItemTemplate& itemTemplate = _itemTemplateStore[itemId];
            itemTemplate.FlagsCu = fields[1].GetUInt32();
            itemTemplate.FoodType = fields[2].GetUInt8();
            itemTemplate.MinMoneyLoot = minMoneyLoot;
            itemTemplate.MaxMoneyLoot = maxMoneyLoot;
            itemTemplate.SpellPPMRate = fields[5].GetFloat();
            ++count;
        }
        while (result->NextRow());
    }

    for (uint32 l_Entry = 0; l_Entry < sItemSparseStore.GetNumRows(); l_Entry++)
    {
        auto l_Itr = _itemTemplateStore.find(l_Entry);
        if (l_Itr == _itemTemplateStore.end())
            continue;

        ItemTemplate& l_ItemTemplate = l_Itr->second;

        if (l_ItemTemplate.Quality == ItemQualities::ITEM_QUALITY_HEIRLOOM)
            l_ItemTemplate.FlagsCu |= ItemFlagsCustom::ITEM_FLAGS_CU_CANT_BE_SELL;
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u item addon templates in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadItemScriptNames()
{
    uint32 oldMSTime = getMSTime();
    uint32 count = 0;

    QueryResult result = WorldDatabase.Query("SELECT Id, ScriptName FROM item_script_names");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 itemId = fields[0].GetUInt32();
            if (!GetItemTemplate(itemId))
            {
                sLog->outError(LOG_FILTER_SQL, "Item %u specified in `item_script_names` does not exist, skipped.", itemId);
                continue;
            }

            _itemTemplateStore[itemId].ScriptId = GetScriptId(fields[1].GetCString());
            ++count;
        }
        while (result->NextRow());
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u item script names in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

struct ItemSpecStats
{
    uint32 ItemType;
    uint32 ItemSpecStatTypes[MAX_ITEM_PROTO_STATS];
    uint32 ItemSpecStatCount;

    ItemSpecStats(ItemEntry const* item, ItemSparseEntry const* sparse) : ItemType(0), ItemSpecStatCount(0)
    {
        memset(ItemSpecStatTypes, -1, sizeof(ItemSpecStatTypes));

        if (item == nullptr || sparse == nullptr)
            return;

        if (item->Class == ITEM_CLASS_WEAPON)
        {
            ItemType = 5;
            switch (item->SubClass)
            {
            case ITEM_SUBCLASS_WEAPON_AXE:
                AddStat(ITEM_SPEC_STAT_ONE_HANDED_AXE);
                break;
            case ITEM_SUBCLASS_WEAPON_AXE2:
                AddStat(ITEM_SPEC_STAT_TWO_HANDED_AXE);
                break;
            case ITEM_SUBCLASS_WEAPON_BOW:
                AddStat(ITEM_SPEC_STAT_BOW);
                break;
            case ITEM_SUBCLASS_WEAPON_GUN:
                AddStat(ITEM_SPEC_STAT_GUN);
                break;
            case ITEM_SUBCLASS_WEAPON_MACE:
                AddStat(ITEM_SPEC_STAT_ONE_HANDED_MACE);
                break;
            case ITEM_SUBCLASS_WEAPON_MACE2:
                AddStat(ITEM_SPEC_STAT_TWO_HANDED_MACE);
                break;
            case ITEM_SUBCLASS_WEAPON_POLEARM:
                AddStat(ITEM_SPEC_STAT_POLEARM);
                break;
            case ITEM_SUBCLASS_WEAPON_SWORD:
                AddStat(ITEM_SPEC_STAT_ONE_HANDED_SWORD);
                break;
            case ITEM_SUBCLASS_WEAPON_SWORD2:
                AddStat(ITEM_SPEC_STAT_TWO_HANDED_SWORD);
                break;
            case ITEM_SUBCLASS_WEAPON_STAFF:
                AddStat(ITEM_SPEC_STAT_STAFF);
                break;
            case ITEM_SUBCLASS_WEAPON_FIST_WEAPON:
                AddStat(ITEM_SPEC_STAT_FIST_WEAPON);
                break;
            case ITEM_SUBCLASS_WEAPON_DAGGER:
                AddStat(ITEM_SPEC_STAT_DAGGER);
                break;
            case ITEM_SUBCLASS_WEAPON_THROWN:
                AddStat(ITEM_SPEC_STAT_THROWN);
                break;
            case ITEM_SUBCLASS_WEAPON_CROSSBOW:
                AddStat(ITEM_SPEC_STAT_CROSSBOW);
                break;
            case ITEM_SUBCLASS_WEAPON_WAND:
                AddStat(ITEM_SPEC_STAT_WAND);
                break;
            default:
                break;
            }
        }
        else if (item->Class == ITEM_CLASS_ARMOR && item->SubClass > 5 && item->SubClass <= 11)
        {
            switch (item->SubClass)
            {
                case ITEM_SUBCLASS_ARMOR_CLOTH:
                    if (sparse->InventoryType != INVTYPE_CLOAK)
                    {
                        ItemType = 1;
                        break;
                    }

                    ItemType = 0;
                    AddStat(ITEM_SPEC_STAT_CLOAK);
                    break;
                case ITEM_SUBCLASS_ARMOR_LEATHER:
                    ItemType = 2;
                    break;
                case ITEM_SUBCLASS_ARMOR_MAIL:
                    ItemType = 3;
                    break;
                case ITEM_SUBCLASS_ARMOR_PLATE:
                    ItemType = 4;
                    break;
                default:
                    ItemType = 6;
                    if (item->SubClass == ITEM_SUBCLASS_ARMOR_SHIELD)
                        AddStat(ITEM_SPEC_STAT_SHIELD);
                    else if (item->SubClass > ITEM_SUBCLASS_ARMOR_SHIELD && item->SubClass <= ITEM_SUBCLASS_ARMOR_RELIC)
                        AddStat(ITEM_SPEC_STAT_RELIC);
                    break;
            }
        }
        else
            ItemType = 0;

        for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
            if (sparse->ItemStatType[i] != -1)
                AddModStat(sparse->ItemStatType[i]);
    }

    void AddStat(ItemSpecStat statType)
    {
        if (ItemSpecStatCount >= MAX_ITEM_PROTO_STATS)
            return;

        for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
            if (ItemSpecStatTypes[i] == uint32(statType))
                return;

        ItemSpecStatTypes[ItemSpecStatCount++] = statType;
    }

    void AddModStat(int32 itemStatType)
    {
        switch (itemStatType)
        {
        case ITEM_MOD_AGILITY:
            AddStat(ITEM_SPEC_STAT_AGILITY);
            break;
        case ITEM_MOD_STRENGTH:
            AddStat(ITEM_SPEC_STAT_STRENGTH);
            break;
        case ITEM_MOD_INTELLECT:
            AddStat(ITEM_SPEC_STAT_INTELLECT);
            break;
        case ITEM_MOD_SPIRIT:
            AddStat(ITEM_SPEC_STAT_SPIRIT);
            break;
        case ITEM_MOD_DODGE_RATING:
            AddStat(ITEM_SPEC_STAT_DODGE);
            break;
        case ITEM_MOD_PARRY_RATING:
            AddStat(ITEM_SPEC_STAT_PARRY);
            break;
        case ITEM_MOD_CRIT_MELEE_RATING:
        case ITEM_MOD_CRIT_RANGED_RATING:
        case ITEM_MOD_CRIT_SPELL_RATING:
        case ITEM_MOD_CRIT_RATING:
            AddStat(ITEM_SPEC_STAT_CRIT);
            break;
        case ITEM_MOD_HASTE_MELEE_RATING:
        case ITEM_MOD_HASTE_RANGED_RATING:
        case ITEM_MOD_HASTE_SPELL_RATING:
        case ITEM_MOD_HASTE_RATING:
            AddStat(ITEM_SPEC_STAT_HASTE);
            break;
        case ITEM_MOD_HIT_RATING:
            AddStat(ITEM_SPEC_STAT_HIT);
            break;
        case ITEM_MOD_EXTRA_ARMOR:
            AddStat(ITEM_SPEC_STAT_BONUS_ARMOR);
            break;
        case ITEM_MOD_DYNAMIC_STAT_AGI_STR_INT:
            AddStat(ITEM_SPEC_STAT_AGILITY);
            AddStat(ITEM_SPEC_STAT_STRENGTH);
            AddStat(ITEM_SPEC_STAT_INTELLECT);
            break;
        case ITEM_MOD_DYNAMIC_STAT_AGI_STR:
            AddStat(ITEM_SPEC_STAT_AGILITY);
            AddStat(ITEM_SPEC_STAT_STRENGTH);
            break;
        case ITEM_MOD_DYNAMIC_STAT_AGI_INT:
            AddStat(ITEM_SPEC_STAT_AGILITY);
            AddStat(ITEM_SPEC_STAT_INTELLECT);
            break;
        case ITEM_MOD_DYNAMIC_STAT_STR_INT:
            AddStat(ITEM_SPEC_STAT_STRENGTH);
            AddStat(ITEM_SPEC_STAT_INTELLECT);
            break;
        }
    }
};

void ObjectMgr::LoadItemSpecs()
{
    uint32 l_Count = 0;
    uint32 l_OldMSTime = getMSTime();

    for (ItemTemplateContainer::iterator l_Itr = _itemTemplateStore.begin(); l_Itr != _itemTemplateStore.end(); ++l_Itr)
    {
        ItemTemplate& l_ItemTemplate = l_Itr->second;
        if (l_ItemTemplate.HasSpec())
            continue;
        
        ItemSpecStats itemSpecStats(sItemStore.LookupEntry(l_ItemTemplate.ItemId), sItemSparseStore.LookupEntry(l_ItemTemplate.ItemId));
        if (itemSpecStats.ItemSpecStatCount)
        {
            for (uint32 l_SpecIndex = 0; l_SpecIndex < sItemSpecStore.GetNumRows(); l_SpecIndex++)
            {
                ItemSpecEntry const* itemSpec = sItemSpecStore.LookupEntry(l_SpecIndex);
                if (itemSpec == nullptr)
                    continue;
                
                if (itemSpecStats.ItemType != itemSpec->ItemType)
                    continue;

                bool hasPrimary = false;
                bool hasSecondary = itemSpec->SecondaryStat == ITEM_SPEC_STAT_NONE;

                for (uint32 i = 0; i < itemSpecStats.ItemSpecStatCount; ++i)
                {
                    if (itemSpecStats.ItemSpecStatTypes[i] == itemSpec->PrimaryStat)
                        hasPrimary = true;
                    if (itemSpecStats.ItemSpecStatTypes[i] == itemSpec->SecondaryStat)
                        hasSecondary = true;
                }

                if (!hasPrimary)
                    hasPrimary = hasSecondary && itemSpecStats.ItemSpecStatCount == 1 && !itemSpec->PrimaryStat; ///< If only 1 was found, it doesnt have any stats, therefore ignore primary stat check

                if (!hasPrimary || !hasSecondary)
                    continue;

                if (l_ItemTemplate.RequiredLevel > 40 && l_ItemTemplate.InventoryType != InventoryType::INVTYPE_CLOAK && l_ItemTemplate.Class == ItemClass::ITEM_CLASS_ARMOR)
                {
                    uint8 l_Class = GetClassBySpec(itemSpec->SpecializationID);
                    switch (l_ItemTemplate.SubClass)
                    {
                        case ITEM_SUBCLASS_ARMOR_CLOTH:
                            if (l_Class != Classes::CLASS_PRIEST && l_Class != Classes::CLASS_MAGE && l_Class != Classes::CLASS_WARLOCK)
                                continue;
                            break;
                        case ITEM_SUBCLASS_ARMOR_LEATHER:
                            if (l_Class != Classes::CLASS_DRUID && l_Class != Classes::CLASS_ROGUE && l_Class != Classes::CLASS_MONK)
                                continue;
                            break;
                        case ITEM_SUBCLASS_ARMOR_MAIL:
                            if (l_Class != Classes::CLASS_HUNTER && l_Class != Classes::CLASS_SHAMAN)
                                continue;
                            break;
                        case ITEM_SUBCLASS_ARMOR_PLATE:
                            if (l_Class != Classes::CLASS_DEATH_KNIGHT && l_Class != Classes::CLASS_WARRIOR && l_Class != Classes::CLASS_PALADIN)
                                continue;
                            break;
                    }
                }

                if (ChrSpecializationsEntry const* l_Specialization = sChrSpecializationsStore.LookupEntry(itemSpec->SpecializationID))
                {
                    if ((1 << (l_Specialization->ClassID - 1)) & l_ItemTemplate.AllowableClass && l_ItemTemplate.RequiredLevel <= itemSpec->MaxLevel)
                        l_ItemTemplate.AddSpec((SpecIndex)itemSpec->SpecializationID, itemSpec->MaxLevel);
                }
            }
        }
    }
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u item specs in %u ms", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

void ObjectMgr::LoadItemSpecsOverride()
{
    uint32 l_Count = 0;
    uint32 l_OldMSTime = getMSTime();

    for (uint32 l_Idx = 0; l_Idx < sItemSpecOverrideStore.GetNumRows(); l_Idx++)
    {
        ItemSpecOverrideEntry const* specInfo = sItemSpecOverrideStore.LookupEntry(l_Idx);
        if (!specInfo)
            continue;

        if (_itemTemplateStore.find(specInfo->itemEntry) == _itemTemplateStore.end())
            continue;

        ItemTemplate& itemTemplate = _itemTemplateStore[specInfo->itemEntry];
        itemTemplate.AddSpec((SpecIndex)specInfo->specID, 40);
        itemTemplate.AddSpec((SpecIndex)specInfo->specID, 100);
        l_Count++;
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u item specs override in %u ms", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

ItemTemplate const* ObjectMgr::GetItemTemplate(uint32 entry)
{
    ItemTemplateContainer::const_iterator itr = _itemTemplateStore.find(entry);
    if (itr != _itemTemplateStore.end())
        return &(itr->second);
    return NULL;
}

void ObjectMgr::LoadVehicleTemplateAccessories()
{
    uint32 oldMSTime = getMSTime();

    _vehicleTemplateAccessoryStore.clear();                           // needed for reload case

    uint32 count = 0;

    //                                                  0             1              2          3           4             5
    QueryResult result = WorldDatabase.Query("SELECT `entry`, `accessory_entry`, `seat_id`, `minion`, `summontype`, `summontimer` FROM `vehicle_template_accessory`");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 vehicle template accessories. DB table `vehicle_template_accessory` is empty.");

        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 uiEntry      = fields[0].GetUInt32();
        uint32 uiAccessory  = fields[1].GetUInt32();
        int8   uiSeat       = int8(fields[2].GetInt8());
        bool   bMinion      = fields[3].GetBool();
        uint8  uiSummonType = fields[4].GetUInt8();
        uint32 uiSummonTimer= fields[5].GetUInt32();

        if (!sObjectMgr->GetCreatureTemplate(uiEntry))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `vehicle_template_accessory`: creature template entry %u does not exist.", uiEntry);
            continue;
        }

        if (!sObjectMgr->GetCreatureTemplate(uiAccessory))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `vehicle_template_accessory`: Accessory %u does not exist.", uiAccessory);
            continue;
        }

        if (_spellClickInfoStore.find(uiEntry) == _spellClickInfoStore.end())
        {
            sLog->outError(LOG_FILTER_SQL, "Table `vehicle_template_accessory`: creature template entry %u has no data in npc_spellclick_spells", uiEntry);
            continue;
        }

        _vehicleTemplateAccessoryStore[uiEntry].push_back(VehicleAccessory(uiAccessory, uiSeat, bMinion, uiSummonType, uiSummonTimer));

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Vehicle Template Accessories in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadVehicleAccessories()
{
    uint32 oldMSTime = getMSTime();

    _vehicleAccessoryStore.clear();                           // needed for reload case

    uint32 count = 0;

    //                                                  0             1             2          3           4             5
    QueryResult result = WorldDatabase.Query("SELECT `guid`, `accessory_entry`, `seat_id`, `minion`, `summontype`, `summontimer` FROM `vehicle_accessory`");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 Vehicle Accessories in %u ms", GetMSTimeDiffToNow(oldMSTime));
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 uiGUID       = fields[0].GetUInt32();
        uint32 uiAccessory  = fields[1].GetUInt32();
        int8   uiSeat       = int8(fields[2].GetInt16());
        bool   bMinion      = fields[3].GetBool();
        uint8  uiSummonType = fields[4].GetUInt8();
        uint32 uiSummonTimer= fields[5].GetUInt32();

        if (!sObjectMgr->GetCreatureTemplate(uiAccessory))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `vehicle_accessory`: Accessory %u does not exist.", uiAccessory);
            continue;
        }

        _vehicleAccessoryStore[uiGUID].push_back(VehicleAccessory(uiAccessory, uiSeat, bMinion, uiSummonType, uiSummonTimer));

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Vehicle Accessories in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadAreaTriggerMoveTemplates()
{
    uint32 l_OldMSTime = getMSTime();

    m_AreaTriggerMoveTemplate.clear();                           // needed for reload case

    uint32 l_Count = 0;

    //                                                      0                1          2
    QueryResult l_Result = WorldDatabase.Query("SELECT `move_curve_id`, `path_size`, `duration` FROM `areatrigger_move_template`");
    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 Areatrigger Move Template in %u ms", GetMSTimeDiffToNow(l_OldMSTime));
        return;
    }

    do
    {
        Field* l_Fields = l_Result->Fetch();
        uint8 l_Index = 0;

        AreaTriggerMoveTemplate l_Template;
        l_Template.m_move_id = l_Fields[l_Index++].GetUInt32();
        l_Template.m_path_size = l_Fields[l_Index++].GetUInt32();
        l_Template.m_duration = l_Fields[l_Index++].GetUInt32();

        m_AreaTriggerMoveTemplate.insert(std::make_pair(l_Template.m_move_id, l_Template));

        ++l_Count;
    } while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Areatrigger Move Template in %u ms", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

void ObjectMgr::LoadAreaTriggerMoveSplines()
{
    uint32 l_OldMSTime = getMSTime();

    m_AreaTriggerMoveSplines.clear();                           // needed for reload case

    uint32 l_Count = 0;

    //                                                      0                1          2        3        4
    QueryResult l_Result = WorldDatabase.Query("SELECT `move_curve_id`, `path_id`, `path_x`, `path_y`, `path_z` FROM `areatrigger_move_splines`");
    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 Areatrigger Move Splines in %u ms", GetMSTimeDiffToNow(l_OldMSTime));
        return;
    }

    do
    {
        Field* l_Fields = l_Result->Fetch();
        uint8 l_Index = 0;

        AreaTriggerMoveSplines l_MoveSplines;
        l_MoveSplines.m_move_id = l_Fields[l_Index++].GetUInt32();
        l_MoveSplines.m_path_id = l_Fields[l_Index++].GetUInt32();
        l_MoveSplines.m_path_x = l_Fields[l_Index++].GetFloat();
        l_MoveSplines.m_path_y = l_Fields[l_Index++].GetFloat();
        l_MoveSplines.m_path_z = l_Fields[l_Index++].GetFloat();

        m_AreaTriggerMoveSplines.insert(std::make_pair(std::make_pair(l_MoveSplines.m_move_id, l_MoveSplines.m_path_id), l_MoveSplines));

        ++l_Count;
    } while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Areatrigger Move Splines in %u ms", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

void ObjectMgr::LoadAreaTriggerTemplates()
{
    uint32 l_OldMSTime = getMSTime();

    m_AreaTriggerTemplates.clear();                                // needed for reload case
    m_AreaTriggerTemplatesSpell.clear();                           // needed for reload case

    uint32 l_Count = 0;

    //                                                      0           1          2        3        4          5         6            7               8                   9
    QueryResult l_Result = WorldDatabase.Query("SELECT `spell_id`, `eff_index`, `entry`, `type`, `scale_x`, `scale_y`, `flags`, `move_curve_id`, `scale_curve_id`, `morph_curve_id`,"
    //                                                         10            11        12      13       14       15       16        17      18          19                20
                                                       "`facing_curve_id`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `ScriptName`, `creature_visual` FROM `areatrigger_template`");
    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 Areatrigger template in %u ms", GetMSTimeDiffToNow(l_OldMSTime));
        return;
    }

    do
    {
        Field* l_Fields = l_Result->Fetch();
        uint8 l_Index = 0;

        AreaTriggerTemplate l_Template;
        l_Template.m_SpellID       = l_Fields[l_Index++].GetUInt32();
        l_Template.m_EffIndex      = l_Fields[l_Index++].GetUInt32();
        l_Template.m_Entry         = l_Fields[l_Index++].GetUInt32();
        l_Template.m_Type          = l_Fields[l_Index++].GetUInt32();
        l_Template.m_ScaleX        = l_Fields[l_Index++].GetFloat();
        l_Template.m_ScaleY        = l_Fields[l_Index++].GetFloat();
        l_Template.m_Flags         = l_Fields[l_Index++].GetUInt32();
        l_Template.m_MoveCurveID   = l_Fields[l_Index++].GetUInt32();
        l_Template.m_ScaleCurveID  = l_Fields[l_Index++].GetUInt32();
        l_Template.m_MorphCurveID  = l_Fields[l_Index++].GetUInt32();
        l_Template.m_FacingCurveID = l_Fields[l_Index++].GetUInt32();

        switch (l_Template.m_Type)
        {
            case AREATRIGGER_TYPE_POLYGON:
                l_Template.m_PolygonDatas.m_Vertices[0]         = l_Fields[l_Index++].GetFloat();
                l_Template.m_PolygonDatas.m_Vertices[1]         = l_Fields[l_Index++].GetFloat();
                l_Template.m_PolygonDatas.m_HeightTarget        = l_Fields[l_Index++].GetFloat();
                l_Template.m_PolygonDatas.m_VerticesTarget[0]   = l_Fields[l_Index++].GetFloat();
                l_Template.m_PolygonDatas.m_VerticesTarget[1]   = l_Fields[l_Index++].GetFloat();
                l_Template.m_PolygonDatas.m_Height              = l_Fields[l_Index++].GetFloat();
                l_Template.m_PolygonDatas.m_VerticesCount       = l_Fields[l_Index++].GetUInt32();
                l_Template.m_PolygonDatas.m_VerticesTargetCount = l_Fields[l_Index++].GetUInt32();
                break;
            case AREATRIGGER_TYPE_BOX:
                l_Template.m_BoxDatas.m_ExtentTarget[0] = l_Fields[l_Index++].GetFloat();
                l_Template.m_BoxDatas.m_Extent[2]       = l_Fields[l_Index++].GetFloat();
                l_Template.m_BoxDatas.m_ExtentTarget[1] = l_Fields[l_Index++].GetFloat();
                l_Template.m_BoxDatas.m_Extent[1]       = l_Fields[l_Index++].GetFloat();
                l_Template.m_BoxDatas.m_ExtentTarget[2] = l_Fields[l_Index++].GetFloat();
                l_Template.m_BoxDatas.m_Extent[0]       = l_Fields[l_Index++].GetFloat();
                l_Index++;
                l_Index++;
                break;
            case AREATRIGGER_TYPE_SPHERE:
            case AREATRIGGER_TYPE_CYLINDER:
            case AREATRIGGER_TYPE_SPLINE:
            default:
                l_Index += 8;
                break;
        }

        l_Template.m_ScriptId = sObjectMgr->GetScriptId(l_Fields[l_Index++].GetCString());
        l_Template.m_CreatureVisualEntry = l_Fields[l_Index++].GetUInt32();

        m_AreaTriggerTemplates[l_Template.m_Entry].push_back(l_Template);
        m_AreaTriggerTemplatesSpell[l_Template.m_SpellID].push_back(l_Template);

        ++l_Count;
    }
    while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Areatrigger templates in %u ms", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

void ObjectMgr::LoadPetStatInfo()
{
    uint32 l_OldMSTime = getMSTime();

    //                                                   0     1       2             3           4          5            6            7            8           9            10
    QueryResult l_Result = WorldDatabase.Query("SELECT entry, speed, powerstatbase, armor_coef, apsp_coef, health_coef, damage_coef, attackspeed, powertype, createpower, secondarystat_coef FROM pet_stats");
    if (!l_Result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 level pet stats definitions. DB table `pet_levelstats` is empty.");
        return;
    }

    /// - Load databases stats

    uint32 l_Count = 0;

    do
    {
        Field* l_Fields = l_Result->Fetch();
        uint32 l_Index  = 0;
        uint32 l_Entry  = l_Fields[l_Index++].GetUInt32();

        PetStatInfo l_PetStat;
        l_PetStat.m_Speed             = l_Fields[l_Index++].GetFloat();
        l_PetStat.m_PowerStat         = PetStatInfo::PowerStatBase(l_Fields[l_Index++].GetUInt32());
        l_PetStat.m_ArmorCoef         = l_Fields[l_Index++].GetFloat();
        l_PetStat.m_APSPCoef          = l_Fields[l_Index++].GetFloat();
        l_PetStat.m_HealthCoef        = l_Fields[l_Index++].GetFloat();
        l_PetStat.m_DamageCoef        = l_Fields[l_Index++].GetFloat();
        l_PetStat.m_AttackSpeed       = l_Fields[l_Index++].GetFloat();
        l_PetStat.m_Power             = Powers(l_Fields[l_Index++].GetUInt32());
        l_PetStat.m_CreatePower       = l_Fields[l_Index++].GetFloat();
        l_PetStat.m_SecondaryStatCoef = l_Fields[l_Index++].GetFloat();

        m_PetInfoStore.insert(std::make_pair(std::move(l_Entry), std::move(l_PetStat)));

        ++l_Count;
    }
    while (l_Result->NextRow());

    /// - Create default stat for pet we don't have informations

    PetStatInfo l_DefaultPetStat;
    l_DefaultPetStat.m_Speed             = 1.14f;
    l_DefaultPetStat.m_PowerStat         = PetStatInfo::PowerStatBase::SpellPower;
    l_DefaultPetStat.m_ArmorCoef         = 1.0f;
    l_DefaultPetStat.m_APSPCoef          = 1.0f;
    l_DefaultPetStat.m_HealthCoef        = 0.7f;
    l_DefaultPetStat.m_DamageCoef        = 0.85f;
    l_DefaultPetStat.m_AttackSpeed       = 2.0f;
    l_DefaultPetStat.m_Power             = Powers::POWER_MANA;
    l_DefaultPetStat.m_CreatePower       = -1;
    l_DefaultPetStat.m_SecondaryStatCoef = 1.0f;

    m_PetInfoStore.insert(std::make_pair(0, std::move(l_DefaultPetStat)));

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u pet stats definitions in %u ms", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

void ObjectMgr::PlayerCreateInfoAddItemHelper(uint32 race_, uint32 class_, uint32 itemId, int32 count)
{
    if (count > 0)
        _playerInfo[race_][class_].item.push_back(PlayerCreateInfoItem(itemId, count));
    else
    {
        if (count < -1)
            sLog->outError(LOG_FILTER_SQL, "Invalid count %i specified on item %u be removed from original player create info (use -1)!", count, itemId);

        bool doneOne = false;
        for (uint32 i = 1; i < sCharStartOutfitStore.GetNumRows(); ++i)
        {
            if (CharStartOutfitEntry const* entry = sCharStartOutfitStore.LookupEntry(i))
            {
                if ((entry->RaceID == race_ && entry->ClassID == class_) || (entry->RaceID == race_ && entry->ClassID == class_ && entry->SexID == 1))
                {
                    bool found = false;
                    for (uint8 x = 0; x < MAX_OUTFIT_ITEMS; ++x)
                    {
                        if (entry->ItemId[x] > 0 && uint32(entry->ItemId[x]) == itemId)
                        {
                            found = true;
                            const_cast<CharStartOutfitEntry*>(entry)->ItemId[x] = 0;
                            break;
                        }
                    }

                    if (!found)
                        sLog->outError(LOG_FILTER_SQL, "Item %u specified to be removed from original create info not found in dbc!", itemId);

                    if (!doneOne)
                        doneOne = true;
                    else
                        break;
                }
            }
        }
    }
}

void ObjectMgr::LoadPlayerInfo()
{
    // Load playercreate
    {
        uint32 oldMSTime = getMSTime();
        //                                                0     1      2    3        4          5           6
        QueryResult result = WorldDatabase.Query("SELECT race, class, map, zone, position_x, position_y, position_z, orientation FROM playercreateinfo");

        if (!result)
        {

            sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 player create definitions. DB table `playercreateinfo` is empty.");
            exit(1);
        }
        else
        {
            uint32 count = 0;

            do
            {
                Field* fields = result->Fetch();

                uint32 current_race  = fields[0].GetUInt8();
                uint32 current_class = fields[1].GetUInt8();
                uint32 mapId         = fields[2].GetUInt16();
                uint32 areaId        = fields[3].GetUInt32(); // zone
                float  positionX     = fields[4].GetFloat();
                float  positionY     = fields[5].GetFloat();
                float  positionZ     = fields[6].GetFloat();
                float  orientation   = fields[7].GetFloat();

                if (current_race >= MAX_RACES)
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong race %u in `playercreateinfo` table, ignoring.", current_race);
                    continue;
                }

                ChrRacesEntry const* rEntry = sChrRacesStore.LookupEntry(current_race);
                if (!rEntry)
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong race %u in `playercreateinfo` table, ignoring.", current_race);
                    continue;
                }

                if (current_class >= MAX_CLASSES)
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong class %u in `playercreateinfo` table, ignoring.", current_class);
                    continue;
                }

                if (!sChrClassesStore.LookupEntry(current_class))
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong class %u in `playercreateinfo` table, ignoring.", current_class);
                    continue;
                }

                // accept DB data only for valid position (and non instanceable)
                if (!MapManager::IsValidMapCoord(mapId, positionX, positionY, positionZ, orientation))
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong home position for class %u race %u pair in `playercreateinfo` table, ignoring.", current_class, current_race);
                    continue;
                }

                if (sMapStore.LookupEntry(mapId)->Instanceable())
                {
                    sLog->outError(LOG_FILTER_SQL, "Home position in instanceable map for class %u race %u pair in `playercreateinfo` table, ignoring.", current_class, current_race);
                    continue;
                }

                PlayerInfo* pInfo = &_playerInfo[current_race][current_class];

                pInfo->mapId       = mapId;
                pInfo->areaId      = areaId;
                pInfo->positionX   = positionX;
                pInfo->positionY   = positionY;
                pInfo->positionZ   = positionZ;
                pInfo->orientation = orientation;

                pInfo->displayId_m = rEntry->model_m;
                pInfo->displayId_f = rEntry->model_f;

                ++count;
            }
            while (result->NextRow());

            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u player create definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        }
    }

    // Load playercreate items
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Player Create Items Data...");
    {
        uint32 oldMSTime = getMSTime();
        //                                                0     1      2       3
        QueryResult result = WorldDatabase.Query("SELECT race, class, itemid, amount FROM playercreateinfo_item");

        if (!result)
        {
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 custom player create items. DB table `playercreateinfo_item` is empty.");
        }
        else
        {
            uint32 count = 0;

            do
            {
                Field* fields = result->Fetch();

                uint32 current_race = fields[0].GetUInt8();
                if (current_race >= MAX_RACES)
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong race %u in `playercreateinfo_item` table, ignoring.", current_race);
                    continue;
                }

                uint32 current_class = fields[1].GetUInt8();
                if (current_class >= MAX_CLASSES)
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong class %u in `playercreateinfo_item` table, ignoring.", current_class);
                    continue;
                }

                uint32 item_id = fields[2].GetUInt32();

                if (!GetItemTemplate(item_id))
                {
                    sLog->outError(LOG_FILTER_SQL, "Item id %u (race %u class %u) in `playercreateinfo_item` table but not listed in `item_template`, ignoring.", item_id, current_race, current_class);
                    continue;
                }

                int32 amount   = fields[3].GetInt8();

                if (!amount)
                {
                    sLog->outError(LOG_FILTER_SQL, "Item id %u (class %u race %u) have amount == 0 in `playercreateinfo_item` table, ignoring.", item_id, current_race, current_class);
                    continue;
                }

                if (!current_race || !current_class)
                {
                    uint32 min_race = current_race ? current_race : 1;
                    uint32 max_race = current_race ? current_race + 1 : MAX_RACES;
                    uint32 min_class = current_class ? current_class : 1;
                    uint32 max_class = current_class ? current_class + 1 : MAX_CLASSES;
                    for (uint32 r = min_race; r < max_race; ++r)
                        for (uint32 c = min_class; c < max_class; ++c)
                            PlayerCreateInfoAddItemHelper(r, c, item_id, amount);
                }
                else
                    PlayerCreateInfoAddItemHelper(current_race, current_class, item_id, amount);

                ++count;
            }
            while (result->NextRow());

            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u custom player create items in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        }
    }

    // Load playercreate spells
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Player Create Spell Data...");
    {
        uint32 oldMSTime = getMSTime();

        std::string tableName = sWorld->getBoolConfig(CONFIG_START_ALL_SPELLS) ? "playercreateinfo_spell_custom" : "playercreateinfo_spell";
        QueryResult result = WorldDatabase.PQuery("SELECT race, class, Spell FROM %s", tableName.c_str());

        if (!result)
        {
            sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 player create spells. DB table `%s` is empty.", sWorld->getBoolConfig(CONFIG_START_ALL_SPELLS) ? "playercreateinfo_spell_custom" : "playercreateinfo_spell");

        }
        else
        {
            uint32 count = 0;

            do
            {
                Field* fields = result->Fetch();

                uint32 current_race = fields[0].GetUInt8();
                if (current_race >= MAX_RACES)
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong race %u in `playercreateinfo_spell` table, ignoring.", current_race);
                    continue;
                }

                uint32 current_class = fields[1].GetUInt8();
                if (current_class >= MAX_CLASSES)
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong class %u in `playercreateinfo_spell` table, ignoring.", current_class);
                    continue;
                }

                if (!current_race || !current_class)
                {
                    uint32 min_race = current_race ? current_race : 1;
                    uint32 max_race = current_race ? current_race + 1 : MAX_RACES;
                    uint32 min_class = current_class ? current_class : 1;
                    uint32 max_class = current_class ? current_class + 1 : MAX_CLASSES;
                    for (uint32 r = min_race; r < max_race; ++r)
                        for (uint32 c = min_class; c < max_class; ++c)
                            _playerInfo[r][c].customSpells.push_back(fields[2].GetUInt32());
                }
                else
                    _playerInfo[current_race][current_class].customSpells.push_back(fields[2].GetUInt32());

                ++count;
            }
            while (result->NextRow());

            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u player create spells in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        }
    }

    // Load playercreate cast spell
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Player Create Cast Spell Data...");
    {
        uint32 oldMSTime = getMSTime();

        QueryResult result = WorldDatabase.PQuery("SELECT raceMask, classMask, spell FROM playercreateinfo_cast_spell");

        if (!result)
            sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 player create cast spells. DB table `playercreateinfo_cast_spell` is empty.");
        else
        {
            uint32 count = 0;

            do
            {
                Field* fields       = result->Fetch();
                uint32 raceMask     = fields[0].GetUInt32();
                uint32 classMask    = fields[1].GetUInt32();
                uint32 spellId      = fields[2].GetUInt32();

                if (raceMask != 0 && !(raceMask & RACEMASK_ALL_PLAYABLE))
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong race mask %u in `playercreateinfo_cast_spell` table, ignoring.", raceMask);
                    continue;
                }

                if (classMask != 0 && !(classMask & CLASSMASK_ALL_PLAYABLE))
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong class mask %u in `playercreateinfo_cast_spell` table, ignoring.", classMask);
                    continue;
                }

                for (uint32 raceIndex = RACE_HUMAN; raceIndex < MAX_RACES; ++raceIndex)
                {
                    if (raceMask == 0 || ((1 << (raceIndex - 1)) & raceMask))
                    {
                        for (uint32 classIndex = CLASS_WARRIOR; classIndex < MAX_CLASSES; ++classIndex)
                        {
                            if (classMask == 0 || ((1 << (classIndex - 1)) & classMask))
                            {
                                if (PlayerInfo* l_Info = &_playerInfo[raceIndex][classIndex])
                                {
                                    l_Info->castSpells.push_back(spellId);
                                    ++count;
                                }
                            }
                        }
                    }
                }
            } while (result->NextRow());

            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u player create cast spells in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        }
    }

    // Load playercreate actions
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Player Create Action Data...");
    {
        uint32 oldMSTime = getMSTime();

        //                                                0     1      2       3       4
        QueryResult result = WorldDatabase.Query("SELECT race, class, button, action, type FROM playercreateinfo_action");

        if (!result)
        {
            sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 player create actions. DB table `playercreateinfo_action` is empty.");

        }
        else
        {
            uint32 count = 0;

            do
            {
                Field* fields = result->Fetch();

                uint32 current_race = fields[0].GetUInt8();
                if (current_race >= MAX_RACES)
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong race %u in `playercreateinfo_action` table, ignoring.", current_race);
                    continue;
                }

                uint32 current_class = fields[1].GetUInt8();
                if (current_class >= MAX_CLASSES)
                {
                    sLog->outError(LOG_FILTER_SQL, "Wrong class %u in `playercreateinfo_action` table, ignoring.", current_class);
                    continue;
                }

                PlayerInfo* pInfo = &_playerInfo[current_race][current_class];
                pInfo->action.push_back(PlayerCreateInfoAction(fields[2].GetUInt16(), fields[3].GetUInt32(), fields[4].GetUInt16()));

                ++count;
            }
            while (result->NextRow());

            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u player create actions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
        }
    }

    // Loading levels data (class/race dependent)
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Player Create Level Stats Data...");
    {
        uint32 oldMSTime = getMSTime();

        //                                                 0     1      2      3    4    5    6    7
        QueryResult result  = WorldDatabase.Query("SELECT race, class, level, str, agi, sta, inte, spi FROM player_levelstats");

        if (!result)
        {
            sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 level stats definitions. DB table `player_levelstats` is empty.");

            exit(1);
        }

        uint32 count = 0;

        do
        {
            Field* fields = result->Fetch();

            uint32 current_race = fields[0].GetUInt8();
            if (current_race >= MAX_RACES)
            {
                sLog->outError(LOG_FILTER_SQL, "Wrong race %u in `player_levelstats` table, ignoring.", current_race);
                continue;
            }

            uint32 current_class = fields[1].GetUInt8();
            if (current_class >= MAX_CLASSES)
            {
                sLog->outError(LOG_FILTER_SQL, "Wrong class %u in `player_levelstats` table, ignoring.", current_class);
                continue;
            }

            uint32 current_level = fields[2].GetUInt8();
            if (current_level > sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL))
            {
                if (current_level > STRONG_MAX_LEVEL)        // hardcoded level maximum
                    sLog->outError(LOG_FILTER_SQL, "Wrong (> %u) level %u in `player_levelstats` table, ignoring.", STRONG_MAX_LEVEL, current_level);
                else
                {
                    sLog->outInfo(LOG_FILTER_GENERAL, "Unused (> MaxPlayerLevel in worldserver.conf) level %u in `player_levelstats` table, ignoring.", current_level);
                    ++count;                                // make result loading percent "expected" correct in case disabled detail mode for example.
                }
                continue;
            }

            PlayerInfo* pInfo = &_playerInfo[current_race][current_class];

            if (!pInfo->levelInfo)
                pInfo->levelInfo = new PlayerLevelInfo[sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL)];

            PlayerLevelInfo* pLevelInfo = &pInfo->levelInfo[current_level-1];

            for (int i = 0; i < MAX_STATS; i++)
            {
                pLevelInfo->stats[i] = fields[i+3].GetUInt16();
            }

            ++count;
        }
        while (result->NextRow());

        // Fill gaps and check integrity
        for (int race = 0; race < MAX_RACES; ++race)
        {
            // skip non existed races
            if (!sChrRacesStore.LookupEntry(race))
                continue;

            for (int class_ = 0; class_ < MAX_CLASSES; ++class_)
            {
                // skip non existed classes
                if (!sChrClassesStore.LookupEntry(class_))
                    continue;

                PlayerInfo* pInfo = &_playerInfo[race][class_];

                // skip non loaded combinations
                if (!pInfo->displayId_m || !pInfo->displayId_f)
                    continue;

                // skip expansion races if not playing with expansion
                if (sWorld->getIntConfig(CONFIG_EXPANSION) < 1 && (race == RACE_BLOODELF || race == RACE_DRAENEI))
                    continue;

                // skip expansion classes if not playing with expansion
                if (sWorld->getIntConfig(CONFIG_EXPANSION) < 2 && class_ == CLASS_DEATH_KNIGHT)
                    continue;

                // skip expansion classes / races if not playing with expansion
                if (sWorld->getIntConfig(CONFIG_EXPANSION) < 3 && (race == RACE_GOBLIN || race == RACE_WORGEN))

                // skip expansion classes / races if not playing with expansion
                if (sWorld->getIntConfig(CONFIG_EXPANSION) < 4 && (class_ == CLASS_MONK || race == RACE_PANDAREN_NEUTRAL || race == RACE_PANDAREN_ALLI || race == RACE_PANDAREN_HORDE))

                // fatal error if no level 1 data
                if (!pInfo->levelInfo || pInfo->levelInfo[0].stats[0] == 0)
                {
                    sLog->outError(LOG_FILTER_SQL, "Race %i Class %i Level 1 does not have stats data!", race, class_);
                    exit(1);
                }

                // fill level gaps
                for (uint8 level = 1; level < sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL); ++level)
                {
                    if (pInfo->levelInfo[level].stats[0] == 0)
                    {
                        sLog->outError(LOG_FILTER_SQL, "Race %i Class %i Level %i does not have stats data. Using stats data of level %i.", race, class_, level+1, level);
                        pInfo->levelInfo[level] = pInfo->levelInfo[level-1];
                    }
                }
            }
        }

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u level stats definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    }

    // Loading xp per level data
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading Player Create XP Data...");
    {
        uint32 oldMSTime = getMSTime();

        _playerXPperLevel.resize(sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL));
        for (uint8 level = 0; level < sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL); ++level)
            _playerXPperLevel[level] = 0;

        uint32 l_Count = 0;

        for (uint8 l_Index = 0; l_Index < sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL); ++l_Index)
        {
            GtOCTLevelExperienceEntry const* l_LevelExperience = sGtOCTLevelExperienceStore.LookupEntry(l_Index);
            if (!l_LevelExperience)
                continue;

            l_Count++;
            _playerXPperLevel[l_LevelExperience->Index] = l_LevelExperience->Data;
        }

        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u xp for level definitions in %u ms", l_Count, GetMSTimeDiffToNow(oldMSTime));
    }
}

PetStatInfo const* ObjectMgr::GetPetStatInfo(uint32 p_Entry) const
{
    PetStatInfoContainer::const_iterator l_Iterator = m_PetInfoStore.find(p_Entry);
    if (l_Iterator != m_PetInfoStore.end())
        return &l_Iterator->second;

    return nullptr;
}

void ObjectMgr::GetPlayerClassLevelInfo(uint32 class_, uint8 level, uint32& baseHP, uint32& baseMana) const
{
    if (level < 1 || class_ >= MAX_CLASSES)
        return;

    if (level >= GT_MAX_LEVEL)
        level = GT_MAX_LEVEL - 1;
    else
        --level;

    GtOCTBaseHPByClassEntry const* hp = sGtOCTBaseHPByClassStore.LookupEntry((class_ - 1) * GT_MAX_LEVEL + level);
    GtOCTBaseMPByClassEntry const* mp = sGtOCTBaseMPByClassStore.LookupEntry((class_ - 1) * GT_MAX_LEVEL + level);

    if (!hp || !mp)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Tried to get non-existant Class-Level combination data for base hp/mp. Class %u Level %u", class_, level);
        return;
    }

    baseHP = uint32(hp->ratio);
    baseMana = uint32(mp->ratio);

}

void ObjectMgr::GetPlayerLevelInfo(uint32 race, uint32 class_, uint8 level, PlayerLevelInfo* info) const
{
    if (level < 1 || race >= MAX_RACES || class_ >= MAX_CLASSES)
        return;

    PlayerInfo const* pInfo = &_playerInfo[race][class_];
    if (pInfo->displayId_m == 0 || pInfo->displayId_f == 0)
        return;

    if (level <= sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL))
        *info = pInfo->levelInfo[level-1];
    else
        BuildPlayerLevelInfo(race, class_, level, info);
}

void ObjectMgr::BuildPlayerLevelInfo(uint8 race, uint8 _class, uint8 level, PlayerLevelInfo* info) const
{
    // base data (last known level)
    *info = _playerInfo[race][_class].levelInfo[sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL)-1];

    // if conversion from uint32 to uint8 causes unexpected behaviour, change lvl to uint32
    for (uint8 lvl = sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL)-1; lvl < level; ++lvl)
    {
        switch (_class)
        {
            case CLASS_WARRIOR:
                info->stats[STAT_STRENGTH]  += (lvl > 23 ? 2: (lvl > 1  ? 1: 0));
                info->stats[STAT_STAMINA]   += (lvl > 23 ? 2: (lvl > 1  ? 1: 0));
                info->stats[STAT_AGILITY]   += (lvl > 36 ? 1: (lvl > 6 && (lvl%2) ? 1: 0));
                info->stats[STAT_INTELLECT] += (lvl > 9 && !(lvl%2) ? 1: 0);
                info->stats[STAT_SPIRIT]    += (lvl > 9 && !(lvl%2) ? 1: 0);
                break;
            case CLASS_PALADIN:
                info->stats[STAT_STRENGTH]  += (lvl > 3  ? 1: 0);
                info->stats[STAT_STAMINA]   += (lvl > 33 ? 2: (lvl > 1 ? 1: 0));
                info->stats[STAT_AGILITY]   += (lvl > 38 ? 1: (lvl > 7 && !(lvl%2) ? 1: 0));
                info->stats[STAT_INTELLECT] += (lvl > 6 && (lvl%2) ? 1: 0);
                info->stats[STAT_SPIRIT]    += (lvl > 7 ? 1: 0);
                break;
            case CLASS_HUNTER:
                info->stats[STAT_STRENGTH]  += (lvl > 4  ? 1: 0);
                info->stats[STAT_STAMINA]   += (lvl > 4  ? 1: 0);
                info->stats[STAT_AGILITY]   += (lvl > 33 ? 2: (lvl > 1 ? 1: 0));
                info->stats[STAT_INTELLECT] += (lvl > 8 && (lvl%2) ? 1: 0);
                info->stats[STAT_SPIRIT]    += (lvl > 38 ? 1: (lvl > 9 && !(lvl%2) ? 1: 0));
                break;
            case CLASS_ROGUE:
                info->stats[STAT_STRENGTH]  += (lvl > 5  ? 1: 0);
                info->stats[STAT_STAMINA]   += (lvl > 4  ? 1: 0);
                info->stats[STAT_AGILITY]   += (lvl > 16 ? 2: (lvl > 1 ? 1: 0));
                info->stats[STAT_INTELLECT] += (lvl > 8 && !(lvl%2) ? 1: 0);
                info->stats[STAT_SPIRIT]    += (lvl > 38 ? 1: (lvl > 9 && !(lvl%2) ? 1: 0));
                break;
            case CLASS_PRIEST:
                info->stats[STAT_STRENGTH]  += (lvl > 9 && !(lvl%2) ? 1: 0);
                info->stats[STAT_STAMINA]   += (lvl > 5  ? 1: 0);
                info->stats[STAT_AGILITY]   += (lvl > 38 ? 1: (lvl > 8 && (lvl%2) ? 1: 0));
                info->stats[STAT_INTELLECT] += (lvl > 22 ? 2: (lvl > 1 ? 1: 0));
                info->stats[STAT_SPIRIT]    += (lvl > 3  ? 1: 0);
                break;
            case CLASS_SHAMAN:
                info->stats[STAT_STRENGTH]  += (lvl > 34 ? 1: (lvl > 6 && (lvl%2) ? 1: 0));
                info->stats[STAT_STAMINA]   += (lvl > 4 ? 1: 0);
                info->stats[STAT_AGILITY]   += (lvl > 7 && !(lvl%2) ? 1: 0);
                info->stats[STAT_INTELLECT] += (lvl > 5 ? 1: 0);
                info->stats[STAT_SPIRIT]    += (lvl > 4 ? 1: 0);
                break;
            case CLASS_MAGE:
                info->stats[STAT_STRENGTH]  += (lvl > 9 && !(lvl%2) ? 1: 0);
                info->stats[STAT_STAMINA]   += (lvl > 5  ? 1: 0);
                info->stats[STAT_AGILITY]   += (lvl > 9 && !(lvl%2) ? 1: 0);
                info->stats[STAT_INTELLECT] += (lvl > 24 ? 2: (lvl > 1 ? 1: 0));
                info->stats[STAT_SPIRIT]    += (lvl > 33 ? 2: (lvl > 2 ? 1: 0));
                break;
            case CLASS_WARLOCK:
                info->stats[STAT_STRENGTH]  += (lvl > 9 && !(lvl%2) ? 1: 0);
                info->stats[STAT_STAMINA]   += (lvl > 38 ? 2: (lvl > 3 ? 1: 0));
                info->stats[STAT_AGILITY]   += (lvl > 9 && !(lvl%2) ? 1: 0);
                info->stats[STAT_INTELLECT] += (lvl > 33 ? 2: (lvl > 2 ? 1: 0));
                info->stats[STAT_SPIRIT]    += (lvl > 38 ? 2: (lvl > 3 ? 1: 0));
                break;
            case CLASS_DRUID:
                info->stats[STAT_STRENGTH]  += (lvl > 38 ? 2: (lvl > 6 && (lvl%2) ? 1: 0));
                info->stats[STAT_STAMINA]   += (lvl > 32 ? 2: (lvl > 4 ? 1: 0));
                info->stats[STAT_AGILITY]   += (lvl > 38 ? 2: (lvl > 8 && (lvl%2) ? 1: 0));
                info->stats[STAT_INTELLECT] += (lvl > 38 ? 3: (lvl > 4 ? 1: 0));
                info->stats[STAT_SPIRIT]    += (lvl > 38 ? 3: (lvl > 5 ? 1: 0));
            case CLASS_MONK:
                info->stats[STAT_STRENGTH]  += (lvl > 38 ? 2 : (lvl > 6 && (lvl % 2) ? 1 : 0));
                info->stats[STAT_STAMINA]   += (lvl > 32 ? 2 : (lvl > 4 ? 1 : 0));
                info->stats[STAT_AGILITY]   += (lvl > 38 ? 2 : (lvl > 8 && (lvl % 2) ? 1 : 0));
                info->stats[STAT_INTELLECT] += (lvl > 38 ? 3 : (lvl > 4 ? 1 : 0));
                info->stats[STAT_SPIRIT]    += (lvl > 38 ? 3 : (lvl > 5 ? 1 : 0));
                break;
        }
    }
}

/* Need to add new fields
        RequiredFactionKills        after RequiredFactionValue2
        RequiredFactionKillsCount   after RequiredFactionKills
        RequiredPetBattleWith       after RequiredFactionKillsCount
        RewardXPId                  after RewardXPId
        move EndText                after RequestItemsText
        Add RequiredNpcOrGo 5 to 10 after RequiredNpcOrGo4
        RequiredNpcOrGoCount 5 to 10 after RequiredNpcOrGoCount4
        RequiredItemId 7 to 10      after RequiredItemId6
        RequiredItemCount 7 to 10   after RequiredItemCount6
        RequiredSpellCast 5 to 6    after RequiredSpellCast4
        ObjectiveText 5 to 10       after ObjectiveText4
        remove QuestGiverTargetName
*/
void ObjectMgr::LoadQuests()
{
    uint32 oldMSTime = getMSTime();

    // For reload case
    for (QuestMap::const_iterator itr=_questTemplates.begin(); itr != _questTemplates.end(); ++itr)
        delete itr->second;
    _questTemplates.clear();

    mExclusiveQuestGroups.clear();

    QueryResult result = WorldDatabase.Query("SELECT "
        "Id, Method, Level, MinLevel, MaxLevel, PackageID, ZoneOrSort, Type, SuggestedPlayers, LimitTime, RequiredTeam, RequiredClasses, RequiredRaces, RequiredSkillId, RequiredSkillPoints, "
        "RequiredMinRepFaction, RequiredMaxRepFaction, RequiredMinRepValue, RequiredMaxRepValue, "
        "PrevQuestId, NextQuestId, ExclusiveGroup, NextQuestIdChain, RewardXPId, RewardMoney, RewardMoneyMaxLevel, RewardSpell, RewardSpellCast, RewardHonor, RewardHonorMultiplier, "
        "RewardMailTemplateId, RewardMailDelay, SourceItemId, SourceSpellId, Flags, Flags2, SpecialFlags, MinimapTargetMark, RewardTitleId, RewardTalents, RewardArenaPoints, "
        "RewardSkillId, RewardSkillPoints, RewardReputationMask, QuestGiverPortrait, QuestTurnInPortrait, RewardItemId1, RewardItemId2, RewardItemId3, RewardItemId4, RewardItemCount1, RewardItemCount2, RewardItemCount3, RewardItemCount4, "
        "RewardChoiceItemId1, RewardChoiceItemId2, RewardChoiceItemId3, RewardChoiceItemId4, RewardChoiceItemId5, RewardChoiceItemId6, RewardChoiceItemCount1, RewardChoiceItemCount2, RewardChoiceItemCount3, RewardChoiceItemCount4, RewardChoiceItemCount5, RewardChoiceItemCount6, "
        "RewardFactionId1, RewardFactionId2, RewardFactionId3, RewardFactionId4, RewardFactionId5, RewardFactionValueId1, RewardFactionValueId2, RewardFactionValueId3, RewardFactionValueId4, RewardFactionValueId5, "
        "RewardFactionValueIdOverride1, RewardFactionValueIdOverride2, RewardFactionValueIdOverride3, RewardFactionValueIdOverride4, RewardFactionValueIdOverride5, "
        "PointMapId, PointX, PointY, PointOption, Title, Objectives, Details, EndText, CompletedText, OfferRewardText, RequestItemsText, "
        "RequiredSourceItemId1, RequiredSourceItemId2, RequiredSourceItemId3, RequiredSourceItemId4, RequiredSourceItemCount1, RequiredSourceItemCount2, RequiredSourceItemCount3, RequiredSourceItemCount4, "
        "RewardCurrencyId1, RewardCurrencyId2, RewardCurrencyId3, RewardCurrencyId4, RewardCurrencyCount1, RewardCurrencyCount2, RewardCurrencyCount3, RewardCurrencyCount4, "
        "QuestGiverTextWindow, QuestGiverTargetName, QuestTurnTextWindow, QuestTurnTargetName, SoundAccept, SoundTurnIn, "
        "DetailsEmote1, DetailsEmote2, DetailsEmote3, DetailsEmote4, DetailsEmoteDelay1, DetailsEmoteDelay2, DetailsEmoteDelay3, DetailsEmoteDelay4, EmoteOnIncomplete, EmoteOnComplete, "
        "OfferRewardEmote1, OfferRewardEmote2, OfferRewardEmote3, OfferRewardEmote4, OfferRewardEmoteDelay1, OfferRewardEmoteDelay2, OfferRewardEmoteDelay3, OfferRewardEmoteDelay4, "
        "StartScript, CompleteScript, BuildVerified"
        " FROM quest_template");
    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 quests definitions. DB table `quest_template` is empty.");

        return;
    }

    // Create multimap previous quest for each existed quest
    // some quests can have many previous maps set by NextQuestId in previous quest
    // for example set of race quests can lead to single not race specific quest
    do
    {
        Field* fields = result->Fetch();

        Quest* newQuest = new Quest(fields);
        _questTemplates[newQuest->GetQuestId()] = newQuest;
    }
    while (result->NextRow());

    std::map<uint32, uint32> usedMailTemplates;

    std::list<uint32> l_QuestToRemove;

    // Post processing
    for (QuestMap::iterator iter = _questTemplates.begin(); iter != _questTemplates.end(); ++iter)
    {
        // Skip post-loading checks for disabled quests
        if (DisableMgr::IsDisabledFor(DISABLE_TYPE_QUEST, iter->first, NULL))
            continue;

        Quest* qinfo = iter->second;

        // Additional quest integrity checks (GO, creature_template and item_template must be loaded already)
        if (qinfo->GetQuestMethod() >= 3)
            sLog->outError(LOG_FILTER_SQL, "Quest %u has `Method` = %u, expected values are 0, 1 or 2.", qinfo->GetQuestId(), qinfo->GetQuestMethod());

        if (qinfo->IsAutoComplete() && qinfo->IsRepeatable())
        {
            sLog->outError(LOG_FILTER_SQL, "Quest %u is auto-complete and is repeatable", qinfo->GetQuestId());
            l_QuestToRemove.push_back(iter->first);
            continue;
        }

        if (qinfo->SpecialFlags & ~QUEST_SPECIAL_FLAGS_DB_ALLOWED)
        {
            sLog->outError(LOG_FILTER_SQL, "Quest %u has `SpecialFlags` = %u > max allowed value. Correct `SpecialFlags` to value <= %u",
                qinfo->GetQuestId(), qinfo->SpecialFlags, QUEST_SPECIAL_FLAGS_DB_ALLOWED);
            qinfo->SpecialFlags &= QUEST_SPECIAL_FLAGS_DB_ALLOWED;
        }

        if (qinfo->Flags & QUEST_FLAGS_DAILY && qinfo->Flags & QUEST_FLAGS_WEEKLY)
        {
            sLog->outError(LOG_FILTER_SQL, "Weekly Quest %u is marked as daily quest in `Flags`, removed daily flag.", qinfo->GetQuestId());
            qinfo->Flags &= ~QUEST_FLAGS_DAILY;
        }

        if (qinfo->Flags & QUEST_FLAGS_DAILY)
        {
            if (!(qinfo->SpecialFlags & QUEST_SPECIAL_FLAGS_REPEATABLE))
            {
                sLog->outError(LOG_FILTER_SQL, "Daily Quest %u not marked as repeatable in `SpecialFlags`, added.", qinfo->GetQuestId());
                qinfo->SpecialFlags |= QUEST_SPECIAL_FLAGS_REPEATABLE;
            }
        }

        if (qinfo->Flags & QUEST_FLAGS_WEEKLY)
        {
            if (!(qinfo->SpecialFlags & QUEST_SPECIAL_FLAGS_REPEATABLE))
            {
                sLog->outError(LOG_FILTER_SQL, "Weekly Quest %u not marked as repeatable in `SpecialFlags`, added.", qinfo->GetQuestId());
                qinfo->SpecialFlags |= QUEST_SPECIAL_FLAGS_REPEATABLE;
            }
        }

        if (qinfo->Flags & QUEST_SPECIAL_FLAGS_MONTHLY)
        {
            if (!(qinfo->Flags & QUEST_SPECIAL_FLAGS_REPEATABLE))
            {
                sLog->outError(LOG_FILTER_SQL, "Monthly quest %u not marked as repeatable in `SpecialFlags`, added.", qinfo->GetQuestId());
                qinfo->Flags |= QUEST_SPECIAL_FLAGS_REPEATABLE;
            }
        }

        if (qinfo->Flags & QUEST_FLAGS_AUTO_REWARDED)
        {
            // At auto-reward can be rewarded only RewardChoiceItemId[0]
            for (int j = 1; j < QUEST_REWARD_CHOICES_COUNT; ++j)
            {
                if (uint32 id = qinfo->RewardChoiceItemId[j])
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardChoiceItemId%d` = %u but item from `RewardChoiceItemId%d` can't be rewarded with quest flag QUEST_FLAGS_AUTO_REWARDED.",
                        qinfo->GetQuestId(), j + 1, id, j + 1);
                    // No changes, quest ignore this data
                }
            }
        }

        if (qinfo->MinLevel == uint32(-1) || qinfo->MinLevel > DEFAULT_MAX_LEVEL)
        {
            /// We dont need this log, blizzard send this on data.
            ///sLog->outError(LOG_FILTER_SQL, "Quest %u should be disabled because `MinLevel` = %i", qinfo->GetQuestId(), int32(qinfo->MinLevel));
            // No changes needed, sending -1 in SMSG_QUEST_QUERY_RESPONSE is valid
        }

        // Client quest log visual (area case)
        if (qinfo->ZoneOrSort > 0)
        {
            if (!GetAreaEntryByAreaID(qinfo->ZoneOrSort))
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `ZoneOrSort` = %u (zone case) but zone with this id does not exist.",
                    qinfo->GetQuestId(), qinfo->ZoneOrSort);
                // No changes, quest not dependent from this value but can have problems at client
            }
        }
        // Client quest log visual (sort case)
        if (qinfo->ZoneOrSort < 0)
        {
            QuestSortEntry const* qSort = sQuestSortStore.LookupEntry(-int32(qinfo->ZoneOrSort));
            if (!qSort)
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `ZoneOrSort` = %i (sort case) but quest sort with this id does not exist.",
                    qinfo->GetQuestId(), qinfo->ZoneOrSort);
                // No changes, quest not dependent from this value but can have problems at client (note some may be 0, we must allow this so no check)
            }
            // Check for proper RequiredSkillId value (skill case)
            if (uint32 skill_id = SkillByQuestSort(-int32(qinfo->ZoneOrSort)))
            {
                if (qinfo->RequiredSkillId != skill_id)
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has `ZoneOrSort` = %i but `RequiredSkillId` does not have a corresponding value (%d).",
                        qinfo->GetQuestId(), qinfo->ZoneOrSort, skill_id);
                    // Override, and force proper value here?
                }
            }
        }

        // RequiredClasses, can be 0/CLASSMASK_ALL_PLAYABLE to allow any class
        if (qinfo->RequiredClasses)
        {
            uint32 RequiredClassCheck = qinfo->RequiredClasses > 0 ? qinfo->RequiredClasses : -(qinfo->RequiredClasses); ///< RequiredclassCheck is never read 01/18/16

            if (!(qinfo->RequiredClasses & CLASSMASK_ALL_PLAYABLE))
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u does not contain any playable classes in `RequiredClasses` (%u), value set to 0 (all classes).", qinfo->GetQuestId(), qinfo->RequiredClasses);
                qinfo->RequiredClasses = 0;
            }
        }
        // RequiredRaces, can be 0/RACEMASK_ALL_PLAYABLE to allow any race
        if (qinfo->RequiredRaces)
        {
            uint32 RequiredRacesCheck = qinfo->RequiredRaces > 0 ? qinfo->RequiredRaces : -(qinfo->RequiredRaces); ///< RequiredRacesCheck is never read 01/18/16

            if (!(qinfo->RequiredRaces & RACEMASK_ALL_PLAYABLE))
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u does not contain any playable races in `RequiredRaces` (%u), value set to 0 (all races).", qinfo->GetQuestId(), qinfo->RequiredRaces);
                qinfo->RequiredRaces = 0;
            }
        }
        // RequiredSkillId, can be 0
        if (qinfo->RequiredSkillId)
        {
            if (!sSkillLineStore.LookupEntry(qinfo->RequiredSkillId))
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RequiredSkillId` = %u but this skill does not exist",
                    qinfo->GetQuestId(), qinfo->RequiredSkillId);
            }
        }

        if (qinfo->RequiredSkillPoints)
        {
            if (qinfo->RequiredSkillPoints > sWorld->GetConfigMaxSkillValue())
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RequiredSkillPoints` = %u but max possible skill is %u, quest can't be done.",
                    qinfo->GetQuestId(), qinfo->RequiredSkillPoints, sWorld->GetConfigMaxSkillValue());
                // No changes, quest can't be done for this requirement
            }
        }
        // Else Skill quests can have 0 skill level, this is ok

        if (qinfo->RequiredMinRepFaction && !sFactionStore.LookupEntry(qinfo->RequiredMinRepFaction))
        {
            sLog->outError(LOG_FILTER_SQL, "Quest %u has `RequiredMinRepFaction` = %u but faction template %u does not exist, quest can't be done.",
                qinfo->GetQuestId(), qinfo->RequiredMinRepFaction, qinfo->RequiredMinRepFaction);
            // No changes, quest can't be done for this requirement
        }

        if (qinfo->RequiredMaxRepFaction && !sFactionStore.LookupEntry(qinfo->RequiredMaxRepFaction))
        {
            sLog->outError(LOG_FILTER_SQL, "Quest %u has `RequiredMaxRepFaction` = %u but faction template %u does not exist, quest can't be done.",
                qinfo->GetQuestId(), qinfo->RequiredMaxRepFaction, qinfo->RequiredMaxRepFaction);
            // No changes, quest can't be done for this requirement
        }

        if (qinfo->RequiredMinRepValue && qinfo->RequiredMinRepValue > ReputationMgr::Reputation_Cap)
        {
            sLog->outError(LOG_FILTER_SQL, "Quest %u has `RequiredMinRepValue` = %d but max reputation is %u, quest can't be done.",
                qinfo->GetQuestId(), qinfo->RequiredMinRepValue, ReputationMgr::Reputation_Cap);
            // No changes, quest can't be done for this requirement
        }

        if (qinfo->RequiredMinRepValue && qinfo->RequiredMaxRepValue && qinfo->RequiredMaxRepValue <= qinfo->RequiredMinRepValue)
        {
            sLog->outError(LOG_FILTER_SQL, "Quest %u has `RequiredMaxRepValue` = %d and `RequiredMinRepValue` = %d, quest can't be done.",
                qinfo->GetQuestId(), qinfo->RequiredMaxRepValue, qinfo->RequiredMinRepValue);
            // No changes, quest can't be done for this requirement
        }

        if (!qinfo->RequiredMinRepFaction && qinfo->RequiredMinRepValue != 0)
        {
            sLog->outError(LOG_FILTER_SQL, "Quest %u has `RequiredMinRepValue` = %d but `RequiredMinRepFaction` is 0, value has no effect",
                qinfo->GetQuestId(), qinfo->RequiredMinRepValue);
            // Warning
        }

        if (!qinfo->RequiredMaxRepFaction && qinfo->RequiredMaxRepValue != 0)
        {
            sLog->outError(LOG_FILTER_SQL, "Quest %u has `RequiredMaxRepValue` = %d but `RequiredMaxRepFaction` is 0, value has no effect",
                qinfo->GetQuestId(), qinfo->RequiredMaxRepValue);
            // Warning
        }

        if (qinfo->RewardTitleId && !sCharTitlesStore.LookupEntry(qinfo->RewardTitleId))
        {
            sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardTitleId` = %u but CharTitle Id %u does not exist, quest can't be rewarded with title.",
                qinfo->GetQuestId(), qinfo->GetCharTitleId(), qinfo->GetCharTitleId());
            qinfo->RewardTitleId = 0;
            // Quest can't reward this title
        }

        if (qinfo->SourceItemId)
        {
            if (!sObjectMgr->GetItemTemplate(qinfo->SourceItemId))
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `SourceItemId` = %u but item with entry %u does not exist, quest can't be done.",
                    qinfo->GetQuestId(), qinfo->SourceItemId, qinfo->SourceItemId);
                qinfo->SourceItemId = 0;                       // Qquest can't be done for this requirement
            }
        }

        if (qinfo->SourceSpellid)
        {
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(qinfo->SourceSpellid);
            if (!spellInfo)
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `SourceSpellid` = %u but spell %u doesn't exist, quest can't be done.",
                    qinfo->GetQuestId(), qinfo->SourceSpellid, qinfo->SourceSpellid);
                qinfo->SourceSpellid = 0;                      // Quest can't be done for this requirement
            }
            else if (!SpellMgr::IsSpellValid(spellInfo))
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `SourceSpellid` = %u but spell %u is broken, quest can't be done.",
                    qinfo->GetQuestId(), qinfo->SourceSpellid, qinfo->SourceSpellid);
                qinfo->SourceSpellid = 0;                      // Quest can't be done for this requirement
            }
        }

        for (uint8 j = 0; j < QUEST_SOURCE_ITEM_IDS_COUNT; ++j)
        {
            uint32 id = qinfo->RequiredSourceItemId[j];
            if (id)
            {
                if (!sObjectMgr->GetItemTemplate(id))
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has `RequiredSourceItemId%d` = %u but item with entry %u does not exist, quest can't be done.",
                        qinfo->GetQuestId(), j+1, id, id);
                    // No changes, quest can't be done for this requirement
                }
            }
            else
            {
                if (qinfo->RequiredSourceItemCount[j]>0)
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has `RequiredSourceItemId%d` = 0 but `RequiredSourceItemCount%d` = %u.",
                        qinfo->GetQuestId(), j+1, j+1, qinfo->RequiredSourceItemCount[j]);
                    // No changes, quest ignore this data
                }
            }
        }

        for (uint8 j = 0; j < QUEST_REWARD_CHOICES_COUNT; ++j)
        {
            uint32 id = qinfo->RewardChoiceItemId[j];
            if (id)
            {
                if (!sObjectMgr->GetItemTemplate(id))
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardChoiceItemId%d` = %u but item with entry %u does not exist, quest will not reward this item.",
                        qinfo->GetQuestId(), j+1, id, id);
                    qinfo->RewardChoiceItemId[j] = 0;          // No changes, quest will not reward this
                }

                if (!qinfo->RewardChoiceItemCount[j])
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardChoiceItemId%d` = %u but `RewardChoiceItemCount%d` = 0, quest can't be done.",
                        qinfo->GetQuestId(), j+1, id, j+1);
                    // No changes, quest can't be done
                }
            }
            else if (qinfo->RewardChoiceItemCount[j]>0)
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardChoiceItemId%d` = 0 but `RewardChoiceItemCount%d` = %u.",
                    qinfo->GetQuestId(), j+1, j+1, qinfo->RewardChoiceItemCount[j]);
                // No changes, quest ignore this data
            }
        }

        for (uint8 j = 0; j < QUEST_REWARDS_COUNT; ++j)
        {
            uint32 id = qinfo->RewardItemId[j];
            if (id)
            {
                if (!sObjectMgr->GetItemTemplate(id))
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardItemId%d` = %u but item with entry %u does not exist, quest will not reward this item.",
                        qinfo->GetQuestId(), j+1, id, id);
                    qinfo->RewardItemId[j] = 0;                // No changes, quest will not reward this item
                }

                if (!qinfo->RewardItemIdCount[j])
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardItemId%d` = %u but `RewardItemCount%d` = 0, quest will not reward this item.",
                        qinfo->GetQuestId(), j+1, id, j+1);
                    // No changes
                }
            }
            else if (qinfo->RewardItemIdCount[j]>0)
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardItemId%d` = 0 but `RewardItemCount%d` = %u.",
                    qinfo->GetQuestId(), j+1, j+1, qinfo->RewardItemIdCount[j]);
                // No changes, quest ignore this data
            }
        }

        for (uint8 j = 0; j < QUEST_REPUTATIONS_COUNT; ++j)
        {
            if (qinfo->RewardFactionId[j])
            {
                if (abs(qinfo->RewardFactionValueId[j]) > 9)
                {
               sLog->outError(LOG_FILTER_SQL, "Quest %u has RewardFactionValueId%d = %i. That is outside the range of valid values (-9 to 9).", qinfo->GetQuestId(), j+1, qinfo->RewardFactionValueId[j]);
                }
                if (!sFactionStore.LookupEntry(qinfo->RewardFactionId[j]))
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardFactionId%d` = %u but raw faction (faction.dbc) %u does not exist, quest will not reward reputation for this faction.", qinfo->GetQuestId(), j+1, qinfo->RewardFactionId[j], qinfo->RewardFactionId[j]);
                    qinfo->RewardFactionId[j] = 0;             // Quest will not reward this
                }
            }

            else if (qinfo->RewardFactionValueIdOverride[j] != 0)
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardFactionId%d` = 0 but `RewardFactionValueIdOverride%d` = %i.",
                    qinfo->GetQuestId(), j+1, j+1, qinfo->RewardFactionValueIdOverride[j]);
                // No changes, quest ignore this data
            }
        }

        if (qinfo->RewardSpell)
        {
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(qinfo->RewardSpell);

            if (!spellInfo)
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardSpell` = %u but spell %u does not exist, spell removed as display reward.",
                    qinfo->GetQuestId(), qinfo->RewardSpell, qinfo->RewardSpell);
                qinfo->RewardSpell = 0;                        // No spell reward will display for this quest
            }

            else if (!SpellMgr::IsSpellValid(spellInfo))
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardSpell` = %u but spell %u is broken, quest will not have a spell reward.",
                    qinfo->GetQuestId(), qinfo->RewardSpell, qinfo->RewardSpell);
                qinfo->RewardSpell = 0;                        // No spell reward will display for this quest
            }

            /*else if (GetTalentSpellCost(qinfo->RewardSpell))
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardSpell` = %u but spell %u is talent, quest will not have a spell reward.",
                    qinfo->GetQuestId(), qinfo->RewardSpell, qinfo->RewardSpell);
                qinfo->RewardSpell = 0;                        // No spell reward will display for this quest
            }*/
        }

        if (qinfo->RewardSpellCast > 0)
        {
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(qinfo->RewardSpellCast);

            if (!spellInfo)
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardSpellCast` = %u but spell %u does not exist, quest will not have a spell reward.",
                    qinfo->GetQuestId(), qinfo->RewardSpellCast, qinfo->RewardSpellCast);
                qinfo->RewardSpellCast = 0;                    // No spell will be casted on player
            }

            else if (!SpellMgr::IsSpellValid(spellInfo))
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardSpellCast` = %u but spell %u is broken, quest will not have a spell reward.",
                    qinfo->GetQuestId(), qinfo->RewardSpellCast, qinfo->RewardSpellCast);
                qinfo->RewardSpellCast = 0;                    // No spell will be casted on player
            }

            /*else if (GetTalentSpellCost(qinfo->RewardSpellCast))
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardSpell` = %u but spell %u is talent, quest will not have a spell reward.",
                    qinfo->GetQuestId(), qinfo->RewardSpellCast, qinfo->RewardSpellCast);
                qinfo->RewardSpellCast = 0;                    // No spell will be casted on player
            }*/
        }

        if (qinfo->RewardMailTemplateId)
        {
            if (!sMailTemplateStore.LookupEntry(qinfo->RewardMailTemplateId))
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardMailTemplateId` = %u but mail template  %u does not exist, quest will not have a mail reward.",
                    qinfo->GetQuestId(), qinfo->RewardMailTemplateId, qinfo->RewardMailTemplateId);
                qinfo->RewardMailTemplateId = 0;               // No mail will send to player
                qinfo->RewardMailDelay      = 0;               // No mail will send to player
            }
            else if (usedMailTemplates.find(qinfo->RewardMailTemplateId) != usedMailTemplates.end())
            {
                std::map<uint32, uint32>::const_iterator used_mt_itr = usedMailTemplates.find(qinfo->RewardMailTemplateId);
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardMailTemplateId` = %u but mail template  %u already used for quest %u, quest will not have a mail reward.",
                    qinfo->GetQuestId(), qinfo->RewardMailTemplateId, qinfo->RewardMailTemplateId, used_mt_itr->second);
                qinfo->RewardMailTemplateId = 0;               // No mail will send to player
                qinfo->RewardMailDelay      = 0;               // No mail will send to player
            }
            else
                usedMailTemplates[qinfo->RewardMailTemplateId] = qinfo->GetQuestId();
        }

        if (qinfo->NextQuestIdChain)
        {
            QuestMap::iterator qNextItr = _questTemplates.find(qinfo->NextQuestIdChain);
            if (qNextItr == _questTemplates.end())
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `NextQuestIdChain` = %u but quest %u does not exist, quest chain will not work.",
                    qinfo->GetQuestId(), qinfo->NextQuestIdChain, qinfo->NextQuestIdChain);
                qinfo->NextQuestIdChain = 0;
            }
            else
                qNextItr->second->prevChainQuests.push_back(qinfo->GetQuestId());
        }

        for (uint8 j = 0; j < QUEST_REWARD_CURRENCY_COUNT; ++j)
        {
            if (qinfo->RewardCurrencyId[j])
            {
                if (qinfo->RewardCurrencyCount[j] == 0)
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardCurrencyId%d` = %u but `RewardCurrencyCount%d` = 0, quest can't be done.",
                        qinfo->GetQuestId(), j + 1, qinfo->RewardCurrencyId[j], j + 1);
                    // No changes, quest can't be done for this requirement
                }

                if (!sCurrencyTypesStore.LookupEntry(qinfo->RewardCurrencyId[j]))
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardCurrencyId%d` = %u but currency with entry %u does not exist, quest can't be done.",
                        qinfo->GetQuestId(), j + 1, qinfo->RewardCurrencyId[j], qinfo->RewardCurrencyId[j]);
                    qinfo->RewardCurrencyCount[j] = 0;         // Prevent incorrect work of quest
                }
            }
            else if (qinfo->RewardCurrencyCount[j] > 0)
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardCurrencyId%d` = 0 but `RewardCurrencyCount%d` = %u, quest can't be done.",
                    qinfo->GetQuestId(), j + 1, j + 1, qinfo->RewardCurrencyCount[j]);
                qinfo->RewardCurrencyCount[j] = 0;             // Prevent incorrect work of quest
            }
        }

        if (qinfo->SoundAccept)
        {
            if (!sSoundEntriesStore.LookupEntry(qinfo->SoundAccept))
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `SoundAccept` = %u but sound %u does not exist, set to 0.",
                    qinfo->GetQuestId(), qinfo->SoundAccept, qinfo->SoundAccept);
                qinfo->SoundAccept = 0;                        // No sound will be played
            }
        }

        if (qinfo->SoundTurnIn)
        {
            if (!sSoundEntriesStore.LookupEntry(qinfo->SoundTurnIn))
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `SoundTurnIn` = %u but sound %u does not exist, set to 0.",
                    qinfo->GetQuestId(), qinfo->SoundTurnIn, qinfo->SoundTurnIn);
                qinfo->SoundTurnIn = 0;                        // No sound will be played
            }
        }

        if (qinfo->RewardSkillId)
        {
            if (!sSkillLineStore.LookupEntry(qinfo->RewardSkillId))
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardSkillId` = %u but this skill does not exist",
                    qinfo->GetQuestId(), qinfo->RewardSkillId);
            }
            if (!qinfo->RewardSkillPoints)
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardSkillId` = %u but `RewardSkillPoints` is 0",
                    qinfo->GetQuestId(), qinfo->RewardSkillId);
            }
        }

        if (qinfo->RewardSkillPoints)
        {
            if (qinfo->RewardSkillPoints > sWorld->GetConfigMaxSkillValue())
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardSkillPoints` = %u but max possible skill is %u, quest can't be done.",
                    qinfo->GetQuestId(), qinfo->RewardSkillPoints, sWorld->GetConfigMaxSkillValue());
                // No changes, quest can't be done for this requirement
            }
            if (!qinfo->RewardSkillId)
            {
                sLog->outError(LOG_FILTER_SQL, "Quest %u has `RewardSkillPoints` = %u but `RewardSkillId` is 0",
                    qinfo->GetQuestId(), qinfo->RewardSkillPoints);
            }
        }

        // Fill additional data stores
        if (qinfo->PrevQuestId)
        {
            if (_questTemplates.find(abs(qinfo->GetPrevQuestId())) == _questTemplates.end())
                sLog->outError(LOG_FILTER_SQL, "Quest %d has PrevQuestId %i, but no such quest", qinfo->GetQuestId(), qinfo->GetPrevQuestId());
            else
                qinfo->prevQuests.push_back(qinfo->PrevQuestId);
        }

        if (qinfo->NextQuestId)
        {
            QuestMap::iterator qNextItr = _questTemplates.find(abs(qinfo->GetNextQuestId()));
            if (qNextItr == _questTemplates.end())
                sLog->outError(LOG_FILTER_SQL, "Quest %d has NextQuestId %i, but no such quest", qinfo->GetQuestId(), qinfo->GetNextQuestId());
            else
            {
                int32 signedQuestId = qinfo->NextQuestId < 0 ? -int32(qinfo->GetQuestId()) : int32(qinfo->GetQuestId());
                qNextItr->second->prevQuests.push_back(signedQuestId);
            }
        }

        if (qinfo->ExclusiveGroup)
            mExclusiveQuestGroups.insert(std::pair<int32, uint32>(qinfo->ExclusiveGroup, qinfo->GetQuestId()));
        if (qinfo->LimitTime)
            qinfo->SetSpecialFlag(QUEST_SPECIAL_FLAGS_TIMED);
    }

    for (uint32 l_QuestID : l_QuestToRemove)
    {
        if (_questTemplates.find(l_QuestID) != _questTemplates.end())
            _questTemplates.erase(l_QuestID);   ///< Disable auto complete quests which are repeatable
    }

    // Check QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT for spell with SPELL_EFFECT_QUEST_COMPLETE
    for (uint32 i = 0; i < sSpellMgr->GetSpellInfoStoreSize(); ++i)
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(i);
        if (!spellInfo)
            continue;

        for (uint8 j = 0; j < spellInfo->EffectCount; ++j)
        {
            if (spellInfo->Effects[j].Effect != SPELL_EFFECT_QUEST_COMPLETE)
                continue;

            uint32 quest_id = spellInfo->Effects[j].MiscValue;

            Quest const* quest = GetQuestTemplate(quest_id);

            // Some quest referenced in spells not exist (outdated spells)
            if (!quest)
                continue;

            if (!quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT))
            {
                sLog->outError(LOG_FILTER_SQL, "Spell (id: %u) have SPELL_EFFECT_QUEST_COMPLETE for quest %u, but quest not have flag QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT. Quest flags must be fixed, quest modified to enable objective.", spellInfo->Id, quest_id);

                // This will prevent quest completing without objective
                const_cast<Quest*>(quest)->SetSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT);
            }
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %lu quests definitions in %u ms", (unsigned long)_questTemplates.size(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadQuestLocales()
{
    uint32 oldMSTime = getMSTime();

    _questLocaleStore.clear();                                 // Need for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, "
        "Title_loc1, Details_loc1, Objectives_loc1, OfferRewardText_loc1, RequestItemsText_loc1, EndText_loc1, CompletedText_loc1, QuestGiverTextWindow_loc1, QuestGiverTargetName_loc1, QuestTurnTextWindow_loc1, QuestTurnTargetName_loc1,"
        "Title_loc2, Details_loc2, Objectives_loc2, OfferRewardText_loc2, RequestItemsText_loc2, EndText_loc2, CompletedText_loc2, QuestGiverTextWindow_loc2, QuestGiverTargetName_loc2, QuestTurnTextWindow_loc2, QuestTurnTargetName_loc2,"
        "Title_loc3, Details_loc3, Objectives_loc3, OfferRewardText_loc3, RequestItemsText_loc3, EndText_loc3, CompletedText_loc3, QuestGiverTextWindow_loc3, QuestGiverTargetName_loc3, QuestTurnTextWindow_loc3, QuestTurnTargetName_loc3,"
        "Title_loc4, Details_loc4, Objectives_loc4, OfferRewardText_loc4, RequestItemsText_loc4, EndText_loc4, CompletedText_loc4, QuestGiverTextWindow_loc4, QuestGiverTargetName_loc4, QuestTurnTextWindow_loc4, QuestTurnTargetName_loc4,"
        "Title_loc5, Details_loc5, Objectives_loc5, OfferRewardText_loc5, RequestItemsText_loc5, EndText_loc5, CompletedText_loc5, QuestGiverTextWindow_loc5, QuestGiverTargetName_loc5, QuestTurnTextWindow_loc5, QuestTurnTargetName_loc5,"
        "Title_loc6, Details_loc6, Objectives_loc6, OfferRewardText_loc6, RequestItemsText_loc6, EndText_loc6, CompletedText_loc6, QuestGiverTextWindow_loc6, QuestGiverTargetName_loc6, QuestTurnTextWindow_loc6, QuestTurnTargetName_loc6,"
        "Title_loc7, Details_loc7, Objectives_loc7, OfferRewardText_loc7, RequestItemsText_loc7, EndText_loc7, CompletedText_loc7, QuestGiverTextWindow_loc7, QuestGiverTargetName_loc7, QuestTurnTextWindow_loc7, QuestTurnTargetName_loc7,"
        "Title_loc8, Details_loc8, Objectives_loc8, OfferRewardText_loc8, RequestItemsText_loc8, EndText_loc8, CompletedText_loc8, QuestGiverTextWindow_loc8, QuestGiverTargetName_loc8, QuestTurnTextWindow_loc8, QuestTurnTargetName_loc8,"
        "Title_loc9, Details_loc9, Objectives_loc9, OfferRewardText_loc9, RequestItemsText_loc9, EndText_loc9, CompletedText_loc9, QuestGiverTextWindow_loc9, QuestGiverTargetName_loc9, QuestTurnTextWindow_loc9, QuestTurnTargetName_loc9,"
        "Title_loc10, Details_loc10, Objectives_loc10, OfferRewardText_loc10, RequestItemsText_loc10, EndText_loc10, CompletedText_loc10, QuestGiverTextWindow_loc10, QuestGiverTargetName_loc10, QuestTurnTextWindow_loc10, QuestTurnTargetName_loc10"
        " FROM locales_quest");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        QuestLocale& data = _questLocaleStore[entry];

        for (uint8 i = 1; i < TOTAL_LOCALES; ++i)
        {
            LocaleConstant locale = (LocaleConstant) i;

            AddLocaleString(fields[1 + 11 * (i - 1)].GetString(), locale, data.Title);
            AddLocaleString(fields[1 + 11 * (i - 1) + 1].GetString(), locale, data.Details);
            AddLocaleString(fields[1 + 11 * (i - 1) + 2].GetString(), locale, data.Objectives);
            AddLocaleString(fields[1 + 11 * (i - 1) + 3].GetString(), locale, data.OfferRewardText);
            AddLocaleString(fields[1 + 11 * (i - 1) + 4].GetString(), locale, data.RequestItemsText);
            AddLocaleString(fields[1 + 11 * (i - 1) + 5].GetString(), locale, data.EndText);
            AddLocaleString(fields[1 + 11 * (i - 1) + 6].GetString(), locale, data.CompletedText);
            AddLocaleString(fields[1 + 11 * (i - 1) + 7].GetString(), locale, data.QuestGiverTextWindow);
            AddLocaleString(fields[1 + 11 * (i - 1) + 8].GetString(), locale, data.QuestGiverTargetName);
            AddLocaleString(fields[1 + 11 * (i - 1) + 9].GetString(), locale, data.QuestTurnTextWindow);
            AddLocaleString(fields[1 + 11 * (i - 1) + 10].GetString(), locale, data.QuestTurnTargetName);
        }
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %lu Quest locale strings in %u ms", (unsigned long)_questLocaleStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadScripts(ScriptsType type)
{
    uint32 oldMSTime = getMSTime();

    ScriptMapMap* scripts = GetScriptsMapByType(type);
    if (!scripts)
        return;

    std::string tableName = GetScriptsTableNameByType(type);
    if (tableName.empty())
        return;

    if (sScriptMgr->IsScriptScheduled())                    // function cannot be called when scripts are in use.
        return;

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Loading %s...", tableName.c_str());

    scripts->clear();                                       // need for reload support

    bool isSpellScriptTable = (type == SCRIPTS_SPELL);
    //                                                 0    1       2         3         4          5    6  7  8  9
    QueryResult result = WorldDatabase.PQuery("SELECT id, delay, command, datalong, datalong2, dataint, x, y, z, o%s FROM %s", isSpellScriptTable ? ", effIndex" : "", tableName.c_str());

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 script definitions. DB table `%s` is empty!", tableName.c_str());
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        ScriptInfo tmp;
        tmp.type      = type;
        tmp.id           = fields[0].GetUInt32();
        if (isSpellScriptTable)
            tmp.id      |= fields[10].GetUInt8() << 24;
        tmp.delay        = fields[1].GetUInt32();
        tmp.command      = ScriptCommands(fields[2].GetUInt32());
        tmp.Raw.nData[0] = fields[3].GetUInt32();
        tmp.Raw.nData[1] = fields[4].GetUInt32();
        tmp.Raw.nData[2] = fields[5].GetInt32();
        tmp.Raw.fData[0] = fields[6].GetFloat();
        tmp.Raw.fData[1] = fields[7].GetFloat();
        tmp.Raw.fData[2] = fields[8].GetFloat();
        tmp.Raw.fData[3] = fields[9].GetFloat();

        // generic command args check
        switch (tmp.command)
        {
            case SCRIPT_COMMAND_TALK:
            {
                if (tmp.Talk.ChatType > CHAT_TYPE_WHISPER && tmp.Talk.ChatType != CHAT_MSG_RAID_BOSS_WHISPER)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid talk type (datalong = %u) in SCRIPT_COMMAND_TALK for script id %u",
                        tableName.c_str(), tmp.Talk.ChatType, tmp.id);
                    continue;
                }
                if (!tmp.Talk.TextID)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid talk text id (dataint = %i) in SCRIPT_COMMAND_TALK for script id %u",
                        tableName.c_str(), tmp.Talk.TextID, tmp.id);
                    continue;
                }
                if (tmp.Talk.TextID < MIN_DB_SCRIPT_STRING_ID || tmp.Talk.TextID >= MAX_DB_SCRIPT_STRING_ID)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has out of range text id (dataint = %i expected %u-%u) in SCRIPT_COMMAND_TALK for script id %u",
                        tableName.c_str(), tmp.Talk.TextID, MIN_DB_SCRIPT_STRING_ID, MAX_DB_SCRIPT_STRING_ID, tmp.id);
                    continue;
                }

                break;
            }

            case SCRIPT_COMMAND_EMOTE:
            {
                if (!sEmotesStore.LookupEntry(tmp.Emote.EmoteID))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid emote id (datalong = %u) in SCRIPT_COMMAND_EMOTE for script id %u",
                        tableName.c_str(), tmp.Emote.EmoteID, tmp.id);
                    continue;
                }
                break;
            }

            case SCRIPT_COMMAND_TELEPORT_TO:
            {
                if (!sMapStore.LookupEntry(tmp.TeleportTo.MapID))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid map (Id: %u) in SCRIPT_COMMAND_TELEPORT_TO for script id %u",
                        tableName.c_str(), tmp.TeleportTo.MapID, tmp.id);
                    continue;
                }

                if (!JadeCore::IsValidMapCoord(tmp.TeleportTo.DestX, tmp.TeleportTo.DestY, tmp.TeleportTo.DestZ, tmp.TeleportTo.Orientation))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid coordinates (X: %f Y: %f Z: %f O: %f) in SCRIPT_COMMAND_TELEPORT_TO for script id %u",
                        tableName.c_str(), tmp.TeleportTo.DestX, tmp.TeleportTo.DestY, tmp.TeleportTo.DestZ, tmp.TeleportTo.Orientation, tmp.id);
                    continue;
                }
                break;
            }

            case SCRIPT_COMMAND_QUEST_EXPLORED:
            {
                Quest const* quest = GetQuestTemplate(tmp.QuestExplored.QuestID);
                if (!quest)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid quest (ID: %u) in SCRIPT_COMMAND_QUEST_EXPLORED in `datalong` for script id %u",
                        tableName.c_str(), tmp.QuestExplored.QuestID, tmp.id);
                    continue;
                }

                if (!quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has quest (ID: %u) in SCRIPT_COMMAND_QUEST_EXPLORED in `datalong` for script id %u, but quest not have flag QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT in quest flags. Script command or quest flags wrong. Quest modified to require objective.",
                        tableName.c_str(), tmp.QuestExplored.QuestID, tmp.id);

                    // this will prevent quest completing without objective
                    const_cast<Quest*>(quest)->SetSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT);

                    // continue; - quest objective requirement set and command can be allowed
                }

                if (float(tmp.QuestExplored.Distance) > DEFAULT_VISIBILITY_DISTANCE)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has too large distance (%u) for exploring objective complete in `datalong2` in SCRIPT_COMMAND_QUEST_EXPLORED in `datalong` for script id %u",
                        tableName.c_str(), tmp.QuestExplored.Distance, tmp.id);
                    continue;
                }

                if (tmp.QuestExplored.Distance && float(tmp.QuestExplored.Distance) > DEFAULT_VISIBILITY_DISTANCE)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has too large distance (%u) for exploring objective complete in `datalong2` in SCRIPT_COMMAND_QUEST_EXPLORED in `datalong` for script id %u, max distance is %f or 0 for disable distance check",
                        tableName.c_str(), tmp.QuestExplored.Distance, tmp.id, DEFAULT_VISIBILITY_DISTANCE);
                    continue;
                }

                if (tmp.QuestExplored.Distance && float(tmp.QuestExplored.Distance) < INTERACTION_DISTANCE)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has too small distance (%u) for exploring objective complete in `datalong2` in SCRIPT_COMMAND_QUEST_EXPLORED in `datalong` for script id %u, min distance is %f or 0 for disable distance check",
                        tableName.c_str(), tmp.QuestExplored.Distance, tmp.id, INTERACTION_DISTANCE);
                    continue;
                }

                break;
            }

            case SCRIPT_COMMAND_KILL_CREDIT:
            {
                if (!GetCreatureTemplate(tmp.KillCredit.CreatureEntry))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid creature (Entry: %u) in SCRIPT_COMMAND_KILL_CREDIT for script id %u",
                        tableName.c_str(), tmp.KillCredit.CreatureEntry, tmp.id);
                    continue;
                }
                break;
            }

            case SCRIPT_COMMAND_RESPAWN_GAMEOBJECT:
            {
                GameObjectData const* data = GetGOData(tmp.RespawnGameobject.GOGuid);
                if (!data)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid gameobject (GUID: %u) in SCRIPT_COMMAND_RESPAWN_GAMEOBJECT for script id %u",
                        tableName.c_str(), tmp.RespawnGameobject.GOGuid, tmp.id);
                    continue;
                }

                GameObjectTemplate const* info = GetGameObjectTemplate(data->id);
                if (!info)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has gameobject with invalid entry (GUID: %u Entry: %u) in SCRIPT_COMMAND_RESPAWN_GAMEOBJECT for script id %u",
                        tableName.c_str(), tmp.RespawnGameobject.GOGuid, data->id, tmp.id);
                    continue;
                }

                if (info->type == GAMEOBJECT_TYPE_FISHINGNODE ||
                    info->type == GAMEOBJECT_TYPE_FISHINGHOLE ||
                    info->type == GAMEOBJECT_TYPE_DOOR        ||
                    info->type == GAMEOBJECT_TYPE_BUTTON      ||
                    info->type == GAMEOBJECT_TYPE_TRAP)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` have gameobject type (%u) unsupported by command SCRIPT_COMMAND_RESPAWN_GAMEOBJECT for script id %u",
                        tableName.c_str(), info->entry, tmp.id);
                    continue;
                }
                break;
            }

            case SCRIPT_COMMAND_TEMP_SUMMON_CREATURE:
            {
                if (!JadeCore::IsValidMapCoord(tmp.TempSummonCreature.PosX, tmp.TempSummonCreature.PosY, tmp.TempSummonCreature.PosZ, tmp.TempSummonCreature.Orientation))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid coordinates (X: %f Y: %f Z: %f O: %f) in SCRIPT_COMMAND_TEMP_SUMMON_CREATURE for script id %u",
                        tableName.c_str(), tmp.TempSummonCreature.PosX, tmp.TempSummonCreature.PosY, tmp.TempSummonCreature.PosZ, tmp.TempSummonCreature.Orientation, tmp.id);
                    continue;
                }

                if (!GetCreatureTemplate(tmp.TempSummonCreature.CreatureEntry))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid creature (Entry: %u) in SCRIPT_COMMAND_TEMP_SUMMON_CREATURE for script id %u",
                        tableName.c_str(), tmp.TempSummonCreature.CreatureEntry, tmp.id);
                    continue;
                }
                break;
            }

            case SCRIPT_COMMAND_OPEN_DOOR:
            case SCRIPT_COMMAND_CLOSE_DOOR:
            {
                GameObjectData const* data = GetGOData(tmp.ToggleDoor.GOGuid);
                if (!data)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has invalid gameobject (GUID: %u) in %s for script id %u",
                        tableName.c_str(), tmp.ToggleDoor.GOGuid, GetScriptCommandName(tmp.command).c_str(), tmp.id);
                    continue;
                }

                GameObjectTemplate const* info = GetGameObjectTemplate(data->id);
                if (!info)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has gameobject with invalid entry (GUID: %u Entry: %u) in %s for script id %u",
                        tableName.c_str(), tmp.ToggleDoor.GOGuid, data->id, GetScriptCommandName(tmp.command).c_str(), tmp.id);
                    continue;
                }

                if (info->type != GAMEOBJECT_TYPE_DOOR)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has gameobject type (%u) non supported by command %s for script id %u",
                        tableName.c_str(), info->entry, GetScriptCommandName(tmp.command).c_str(), tmp.id);
                    continue;
                }

                break;
            }

            case SCRIPT_COMMAND_REMOVE_AURA:
            {
                if (!sSpellMgr->GetSpellInfo(tmp.RemoveAura.SpellID))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` using non-existent spell (id: %u) in SCRIPT_COMMAND_REMOVE_AURA for script id %u",
                        tableName.c_str(), tmp.RemoveAura.SpellID, tmp.id);
                    continue;
                }
                if (tmp.RemoveAura.Flags & ~0x1)                    // 1 bits (0, 1)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` using unknown flags in datalong2 (%u) in SCRIPT_COMMAND_REMOVE_AURA for script id %u",
                        tableName.c_str(), tmp.RemoveAura.Flags, tmp.id);
                    continue;
                }
                break;
            }

            case SCRIPT_COMMAND_CAST_SPELL:
            {
                if (!sSpellMgr->GetSpellInfo(tmp.CastSpell.SpellID))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` using non-existent spell (id: %u) in SCRIPT_COMMAND_CAST_SPELL for script id %u",
                        tableName.c_str(), tmp.CastSpell.SpellID, tmp.id);
                    continue;
                }
                if (tmp.CastSpell.Flags > 4)                      // targeting type
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` using unknown target in datalong2 (%u) in SCRIPT_COMMAND_CAST_SPELL for script id %u",
                        tableName.c_str(), tmp.CastSpell.Flags, tmp.id);
                    continue;
                }
                if (tmp.CastSpell.Flags != 4 && tmp.CastSpell.CreatureEntry & ~0x1)                      // 1 bit (0, 1)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` using unknown flags in dataint (%u) in SCRIPT_COMMAND_CAST_SPELL for script id %u",
                        tableName.c_str(), tmp.CastSpell.CreatureEntry, tmp.id);
                    continue;
                }
                else if (tmp.CastSpell.Flags == 4 && !GetCreatureTemplate(tmp.CastSpell.CreatureEntry))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` using invalid creature entry in dataint (%u) in SCRIPT_COMMAND_CAST_SPELL for script id %u",
                        tableName.c_str(), tmp.CastSpell.CreatureEntry, tmp.id);
                    continue;
                }
                break;
            }

            case SCRIPT_COMMAND_CREATE_ITEM:
            {
                if (!GetItemTemplate(tmp.CreateItem.ItemEntry))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` has nonexistent item (entry: %u) in SCRIPT_COMMAND_CREATE_ITEM for script id %u",
                        tableName.c_str(), tmp.CreateItem.ItemEntry, tmp.id);
                    continue;
                }
                if (!tmp.CreateItem.Amount)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `%s` SCRIPT_COMMAND_CREATE_ITEM but amount is %u for script id %u",
                        tableName.c_str(), tmp.CreateItem.Amount, tmp.id);
                    continue;
                }
                break;
            }
            default:
                break;
        }

        if (scripts->find(tmp.id) == scripts->end())
        {
            ScriptMap emptyMap;
            (*scripts)[tmp.id] = emptyMap;
        }
        (*scripts)[tmp.id].insert(std::pair<uint32, ScriptInfo>(tmp.delay, tmp));

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u script definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadGameObjectScripts()
{
    LoadScripts(SCRIPTS_GAMEOBJECT);

    // check ids
    for (ScriptMapMap::const_iterator itr = sGameObjectScripts.begin(); itr != sGameObjectScripts.end(); ++itr)
    {
        if (!GetGOData(itr->first))
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject_scripts` has not existing gameobject (GUID: %u) as script id", itr->first);
    }
}

void ObjectMgr::LoadQuestEndScripts()
{
    LoadScripts(SCRIPTS_QUEST_END);

    // check ids
    for (ScriptMapMap::const_iterator itr = sQuestEndScripts.begin(); itr != sQuestEndScripts.end(); ++itr)
    {
        if (!GetQuestTemplate(itr->first))
            sLog->outError(LOG_FILTER_SQL, "Table `quest_end_scripts` has not existing quest (Id: %u) as script id", itr->first);
    }
}

void ObjectMgr::LoadQuestStartScripts()
{
    LoadScripts(SCRIPTS_QUEST_START);

    // check ids
    for (ScriptMapMap::const_iterator itr = sQuestStartScripts.begin(); itr != sQuestStartScripts.end(); ++itr)
    {
        if (!GetQuestTemplate(itr->first))
            sLog->outError(LOG_FILTER_SQL, "Table `quest_start_scripts` has not existing quest (Id: %u) as script id", itr->first);
    }
}

void ObjectMgr::LoadSpellScripts()
{
    LoadScripts(SCRIPTS_SPELL);

    // check ids
    for (ScriptMapMap::const_iterator itr = sSpellScripts.begin(); itr != sSpellScripts.end(); ++itr)
    {
        uint32 spellId = uint32(itr->first) & 0x00FFFFFF;
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);

        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `spell_scripts` has not existing spell (Id: %u) as script id", spellId);
            continue;
        }

        uint8 i = (uint8)((uint32(itr->first) >> 24) & 0x000000FF);
        //check for correct spellEffect
        if (!spellInfo->Effects[i].Effect || (spellInfo->Effects[i].Effect != SPELL_EFFECT_SCRIPT_EFFECT && spellInfo->Effects[i].Effect != SPELL_EFFECT_DUMMY))
            sLog->outError(LOG_FILTER_SQL, "Table `spell_scripts` - spell %u effect %u is not SPELL_EFFECT_SCRIPT_EFFECT or SPELL_EFFECT_DUMMY", spellId, i);
    }
}

void ObjectMgr::LoadEventScripts()
{
    LoadScripts(SCRIPTS_EVENT);

    std::set<uint32> evt_scripts;
    // Load all possible script entries from gameobjects
    GameObjectTemplateContainer const* gotc = sObjectMgr->GetGameObjectTemplates();
    for (GameObjectTemplateContainer::const_iterator itr = gotc->begin(); itr != gotc->end(); ++itr)
        if (uint32 eventId = itr->second.GetEventScriptId())
            evt_scripts.insert(eventId);

    // Load all possible script entries from spells
    for (uint32 i = 1; i < sSpellMgr->GetSpellInfoStoreSize(); ++i)
        if (SpellInfo const* spell = sSpellMgr->GetSpellInfo(i))
            for (uint8 j = 0; j < spell->EffectCount; ++j)
                if (spell->Effects[j].Effect == SPELL_EFFECT_SEND_EVENT)
                    if (spell->Effects[j].MiscValue)
                        evt_scripts.insert(spell->Effects[j].MiscValue);

    for (size_t path_idx = 0; path_idx < sTaxiPathNodesByPath.size(); ++path_idx)
    {
        for (size_t node_idx = 0; node_idx < sTaxiPathNodesByPath[path_idx].size(); ++node_idx)
        {
            TaxiPathNodeEntry const* node = sTaxiPathNodesByPath[path_idx][node_idx];

            if (node->ArrivalEventID)
                evt_scripts.insert(node->ArrivalEventID);

            if (node->DepartureEventID)
                evt_scripts.insert(node->DepartureEventID);
        }
    }

    // Then check if all scripts are in above list of possible script entries
    for (ScriptMapMap::const_iterator itr = sEventScripts.begin(); itr != sEventScripts.end(); ++itr)
    {
        std::set<uint32>::const_iterator itr2 = evt_scripts.find(itr->first);
        if (itr2 == evt_scripts.end())
            sLog->outError(LOG_FILTER_SQL, "Table `event_scripts` has script (Id: %u) not referring to any gameobject_template type 10 data2 field, type 3 data6 field, type 13 data 2 field or any spell effect %u",
                itr->first, SPELL_EFFECT_SEND_EVENT);
    }
}

//Load WP Scripts
void ObjectMgr::LoadWaypointScripts()
{
    LoadScripts(SCRIPTS_WAYPOINT);

    std::set<uint32> actionSet;

    for (ScriptMapMap::const_iterator itr = sWaypointScripts.begin(); itr != sWaypointScripts.end(); ++itr)
        actionSet.insert(itr->first);

    PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_WAYPOINT_DATA_ACTION);
    PreparedQueryResult result = WorldDatabase.Query(stmt);

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 action = fields[0].GetUInt32();

            actionSet.erase(action);
        }
        while (result->NextRow());
    }

    for (std::set<uint32>::iterator itr = actionSet.begin(); itr != actionSet.end(); ++itr)
        sLog->outError(LOG_FILTER_SQL, "There is no waypoint which links to the waypoint script %u", *itr);
}

void ObjectMgr::LoadSpellScriptNames()
{
    uint32 oldMSTime = getMSTime();

    _spellScriptsStore.clear();                            // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT spell_id, ScriptName FROM spell_script_names");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell script names. DB table `spell_script_names` is empty!");
        return;
    }

    uint32 count = 0;

    do
    {

        Field* fields = result->Fetch();

        int32 spellId          = fields[0].GetInt32();
        const char* scriptName = fields[1].GetCString();

        bool allRanks = false;
        if (spellId <= 0)
        {
            allRanks = true;
            spellId = -spellId;
        }

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Scriptname:`%s` spell (spell_id:%d) does not exist in `Spell.dbc`.", scriptName, fields[0].GetInt32());
            continue;
        }

        if (allRanks)
        {
            if (sSpellMgr->GetFirstSpellInChain(spellId) != uint32(spellId))
            {
                sLog->outError(LOG_FILTER_SQL, "Scriptname:`%s` spell (spell_id:%d) is not first rank of spell.", scriptName, fields[0].GetInt32());
                continue;
            }
            while (spellInfo)
            {
                _spellScriptsStore.insert(SpellScriptsContainer::value_type(spellInfo->Id, GetScriptId(scriptName)));
                spellInfo = sSpellMgr->GetSpellInfo(spellInfo->Id)->GetNextRankSpell();
            }
        }
        else
            _spellScriptsStore.insert(SpellScriptsContainer::value_type(spellInfo->Id, GetScriptId(scriptName)));
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell script names in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::ValidateSpellScripts()
{
    uint32 oldMSTime = getMSTime();

    if (_spellScriptsStore.empty())
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Validated 0 scripts.");
        return;
    }

    uint32 count = 0;

    for (SpellScriptsContainer::iterator itr = _spellScriptsStore.begin(); itr != _spellScriptsStore.end();)
    {
        SpellInfo const* spellEntry = sSpellMgr->GetSpellInfo(itr->first);
        std::vector<std::pair<SpellScriptLoader*, SpellScriptsContainer::iterator> > SpellScriptLoaders;
        sScriptMgr->CreateSpellScriptLoaders(itr->first, SpellScriptLoaders);
        itr = _spellScriptsStore.upper_bound(itr->first);

        for (std::vector<std::pair<SpellScriptLoader*, SpellScriptsContainer::iterator> >::iterator sitr = SpellScriptLoaders.begin(); sitr != SpellScriptLoaders.end(); ++sitr)
        {
            SpellScript* spellScript = sitr->first->GetSpellScript();
            AuraScript* auraScript = sitr->first->GetAuraScript();
            bool valid = true;
            if (!spellScript && !auraScript)
            {
                sLog->outError(LOG_FILTER_TSCR, "Functions GetSpellScript() and GetAuraScript() of script `%s` do not return objects - script skipped",  GetScriptName(sitr->second->second));
                valid = false;
            }
            if (spellScript)
            {
                spellScript->_Init(&sitr->first->GetName(), spellEntry->Id);
                spellScript->_Register();
                if (!spellScript->_Validate(spellEntry))
                    valid = false;
                delete spellScript;
            }
            if (auraScript)
            {
                auraScript->_Init(&sitr->first->GetName(), spellEntry->Id);
                auraScript->_Register();
                if (!auraScript->_Validate(spellEntry))
                    valid = false;
                delete auraScript;
            }
            if (!valid)
            {
                _spellScriptsStore.erase(sitr->second);
            }
        }
        ++count;
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Validated %u scripts in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadPageTexts()
{
    uint32 oldMSTime = getMSTime();

    //                                                 0      1       2
    QueryResult result = WorldDatabase.Query("SELECT entry, text, next_page FROM page_text");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 page texts. DB table `page_text` is empty!");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        PageText& pageText = _pageTextStore[fields[0].GetUInt32()];

        pageText.Text     = fields[1].GetString();
        pageText.NextPage = fields[2].GetUInt32();

        ++count;
    }
    while (result->NextRow());

    for (PageTextContainer::const_iterator itr = _pageTextStore.begin(); itr != _pageTextStore.end(); ++itr)
    {
        if (itr->second.NextPage)
        {
            PageTextContainer::const_iterator itr2 = _pageTextStore.find(itr->second.NextPage);
            if (itr2 == _pageTextStore.end())
                sLog->outError(LOG_FILTER_SQL, "Page text (Id: %u) has not existing next page (Id: %u)", itr->first, itr->second.NextPage);

        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u page texts in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

PageText const* ObjectMgr::GetPageText(uint32 pageEntry)
{
    PageTextContainer::const_iterator itr = _pageTextStore.find(pageEntry);
    if (itr != _pageTextStore.end())
        return &(itr->second);

    return NULL;
}

void ObjectMgr::LoadPageTextLocales()
{
    uint32 oldMSTime = getMSTime();

    _pageTextLocaleStore.clear();                             // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, text_loc1, text_loc2, text_loc3, text_loc4, text_loc5, text_loc6, text_loc7, text_loc8, text_loc9, text_loc10 FROM locales_page_text");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        PageTextLocale& data = _pageTextLocaleStore[entry];

        for (uint8 i = 1; i < TOTAL_LOCALES; ++i)
            AddLocaleString(fields[i].GetString(), LocaleConstant(i), data.Text);
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %lu PageText locale strings in %u ms", (unsigned long)_pageTextLocaleStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadInstanceTemplate()
{
    uint32 oldMSTime = getMSTime();

    //                                                0     1       2        4
    QueryResult result = WorldDatabase.Query("SELECT map, parent, script, allowMount FROM instance_template");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 instance templates. DB table `page_text` is empty!");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint16 mapID = fields[0].GetUInt16();

        if (!MapManager::IsValidMAP(mapID, true))
        {
            sLog->outError(LOG_FILTER_SQL, "ObjectMgr::LoadInstanceTemplate: bad mapid %d for template!", mapID);
            continue;
        }

        InstanceTemplate instanceTemplate;

        instanceTemplate.AllowMount = fields[3].GetBool();
        instanceTemplate.Parent     = uint32(fields[1].GetUInt16());
        instanceTemplate.ScriptId   = sObjectMgr->GetScriptId(fields[2].GetCString());

        _instanceTemplateStore[mapID] = instanceTemplate;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u instance templates in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

InstanceTemplate const* ObjectMgr::GetInstanceTemplate(uint32 mapID)
{
    InstanceTemplateContainer::const_iterator itr = _instanceTemplateStore.find(uint16(mapID));
    if (itr != _instanceTemplateStore.end())
        return &(itr->second);

    return NULL;
}

void ObjectMgr::LoadInstanceEncounters()
{
    uint32 l_OldMSTime = getMSTime();
    uint32 l_Counter = 0;

    for (uint32 l_Index = 0; l_Index <= sDungeonEncounterStore.GetLastEntry(); ++l_Index)
    {
        if (DungeonEncounterEntry const* l_Encounter = sDungeonEncounterStore.LookupEntry(l_Index))
        {
            DungeonEncounterList& l_Encounters = _dungeonEncounterStore[MAKE_PAIR32(l_Encounter->MapID, l_Encounter->DifficultyID)];
            l_Encounters.push_back(new DungeonEncounter(l_Encounter, EncounterCreditType(0), 0, 0));
            ++l_Counter;
        }
    }

    ///                                                 0         1            2                3
    QueryResult l_Result = WorldDatabase.Query("SELECT entry, creditType, creditEntry, lastEncounterDungeon FROM instance_encounters");

    if (!l_Result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 instance encounters, table is empty!");

        return;
    }

    std::map<uint32, DungeonEncounterEntry const*> l_DungeonLastBosses;
    do
    {
        Field* l_Fields                                 = l_Result->Fetch();
        uint32 l_EncounterID                            = l_Fields[0].GetUInt32();
        uint8 l_CreditType                              = l_Fields[1].GetUInt8();
        uint32 l_CreditEntry                            = l_Fields[2].GetUInt32();
        uint16 l_LastEncounterDungeon                   = l_Fields[3].GetUInt16();
        DungeonEncounterEntry const* l_DungeonEncounter = sDungeonEncounterStore.LookupEntry(l_EncounterID);

        if (!l_DungeonEncounter)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `instance_encounters` has an invalid encounter id %u, skipped!", l_EncounterID);
            continue;
        }

        if (l_LastEncounterDungeon && !sLFGDungeonStore.LookupEntry(l_LastEncounterDungeon))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `instance_encounters` has an encounter %u (%s) marked as final for invalid dungeon id %u, skipped!", l_EncounterID, l_DungeonEncounter->NameLang, l_LastEncounterDungeon);
            continue;
        }

        std::map<uint32, DungeonEncounterEntry const*>::const_iterator l_Itr = l_DungeonLastBosses.find(l_LastEncounterDungeon);
        if (l_LastEncounterDungeon)
        {
            if (l_Itr != l_DungeonLastBosses.end())
            {
                sLog->outError(LOG_FILTER_SQL, "Table `instance_encounters` specified encounter %u (%s) as last encounter but %u (%s) is already marked as one, skipped!", l_EncounterID, l_DungeonEncounter->NameLang, l_Itr->second->ID, l_Itr->second->NameLang);
                continue;
            }

            l_DungeonLastBosses[l_LastEncounterDungeon] = l_DungeonEncounter;
        }

        switch (l_CreditType)
        {
            case ENCOUNTER_CREDIT_KILL_CREATURE:
            {
                CreatureTemplate const* l_CreatureInfo = GetCreatureTemplate(l_CreditEntry);
                if (!l_CreatureInfo)
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `instance_encounters` has an invalid creature (entry %u) linked to the encounter %u (%s), skipped!", l_CreditEntry, l_EncounterID, l_DungeonEncounter->NameLang);
                    continue;
                }
                const_cast<CreatureTemplate*>(l_CreatureInfo)->flags_extra |= CREATURE_FLAG_EXTRA_DUNGEON_BOSS;
                break;
            }
            case ENCOUNTER_CREDIT_CAST_SPELL:
                if (!sSpellMgr->GetSpellInfo(l_CreditEntry))
                {
                    sLog->outError(LOG_FILTER_SQL, "Table `instance_encounters` has an invalid spell (entry %u) linked to the encounter %u (%s), skipped!", l_CreditEntry, l_EncounterID, l_DungeonEncounter->NameLang);
                    continue;
                }
                break;
            default:
                sLog->outError(LOG_FILTER_SQL, "Table `instance_encounters` has an invalid credit type (%u) for encounter %u (%s), skipped!", l_CreditType, l_EncounterID, l_DungeonEncounter->NameLang);
                continue;
        }

        uint32 l_DungeonStoreIndex = MAKE_PAIR32(l_DungeonEncounter->MapID, l_DungeonEncounter->DifficultyID);

        if (_dungeonEncounterStore.find(l_DungeonStoreIndex) != _dungeonEncounterStore.end())
        {
            DungeonEncounterList& l_Encounters = _dungeonEncounterStore[l_DungeonStoreIndex];

            for (auto& l_Encounter : l_Encounters)
            {
                if (l_Encounter->dbcEntry && l_Encounter->dbcEntry->ID == l_EncounterID)
                {
                    delete l_Encounter;
                    l_Encounter = new DungeonEncounter(l_DungeonEncounter, EncounterCreditType(l_CreditType), l_CreditEntry, l_LastEncounterDungeon);
                    break;
                }
            }
        }
    }
    while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u instance encounters in %u ms", l_Counter, GetMSTimeDiffToNow(l_OldMSTime));
}

GossipText const* ObjectMgr::GetGossipText(uint32 Text_ID) const
{
    GossipTextContainer::const_iterator itr = _gossipTextStore.find(Text_ID);
    if (itr != _gossipTextStore.end())
        return &itr->second;
    return NULL;
}

void ObjectMgr::LoadGossipText()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT * FROM npc_text");

    int count = 0;
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u npc texts", count);
        return;
    }
    //_gossipTextStore.rehash(result->GetRowCount());

    int cic;

    do
    {
        ++count;
        cic = 0;

        Field* fields = result->Fetch();

        uint32 Text_ID    = fields[cic++].GetUInt32();
        if (!Text_ID)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `npc_text` has record wit reserved id 0, ignore.");
            continue;
        }

        GossipText& gText = _gossipTextStore[Text_ID];

        for (int i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; i++)
        {
            gText.Options[i].Text_0           = fields[cic++].GetString();
            gText.Options[i].Text_1           = fields[cic++].GetString();

            gText.Options[i].Language         = fields[cic++].GetUInt8();
            gText.Options[i].Probability      = fields[cic++].GetFloat();

            for (uint8 j=0; j < MAX_GOSSIP_TEXT_EMOTES; ++j)
            {
                gText.Options[i].Emotes[j]._Delay  = fields[cic++].GetUInt16();
                gText.Options[i].Emotes[j]._Emote  = fields[cic++].GetUInt16();
            }
        }
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u npc texts in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadNpcTextLocales()
{
    uint32 oldMSTime = getMSTime();

    _npcTextLocaleStore.clear();                              // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, "
        "Text0_0_loc1,  Text0_1_loc1,  Text1_0_loc1,  Text1_1_loc1,  Text2_0_loc1,  Text2_1_loc1,  Text3_0_loc1,  Text3_1_loc1,  Text4_0_loc1,  Text4_1_loc1,  Text5_0_loc1,  Text5_1_loc1,  Text6_0_loc1,  Text6_1_loc1,  Text7_0_loc1,  Text7_1_loc1,  Text8_0_loc1,   Text8_1_loc1,   Text9_0_loc1,   Text9_1_loc1, "
        "Text0_0_loc2,  Text0_1_loc2,  Text1_0_loc2,  Text1_1_loc2,  Text2_0_loc2,  Text2_1_loc2,  Text3_0_loc2,  Text3_1_loc1,  Text4_0_loc2,  Text4_1_loc2,  Text5_0_loc2,  Text5_1_loc2,  Text6_0_loc2,  Text6_1_loc2,  Text7_0_loc2,  Text7_1_loc2,  Text8_0_loc2,   Text8_1_loc2,   Text9_0_loc2,   Text9_1_loc2, "
        "Text0_0_loc3,  Text0_1_loc3,  Text1_0_loc3,  Text1_1_loc3,  Text2_0_loc3,  Text2_1_loc3,  Text3_0_loc3,  Text3_1_loc1,  Text4_0_loc3,  Text4_1_loc3,  Text5_0_loc3,  Text5_1_loc3,  Text6_0_loc3,  Text6_1_loc3,  Text7_0_loc3,  Text7_1_loc3,  Text8_0_loc3,   Text8_1_loc3,   Text9_0_loc3,   Text9_1_loc3, "
        "Text0_0_loc4,  Text0_1_loc4,  Text1_0_loc4,  Text1_1_loc4,  Text2_0_loc4,  Text2_1_loc4,  Text3_0_loc4,  Text3_1_loc1,  Text4_0_loc4,  Text4_1_loc4,  Text5_0_loc4,  Text5_1_loc4,  Text6_0_loc4,  Text6_1_loc4,  Text7_0_loc4,  Text7_1_loc4,  Text8_0_loc4,   Text8_1_loc4,   Text9_0_loc4,   Text9_1_loc4, "
        "Text0_0_loc5,  Text0_1_loc5,  Text1_0_loc5,  Text1_1_loc5,  Text2_0_loc5,  Text2_1_loc5,  Text3_0_loc5,  Text3_1_loc1,  Text4_0_loc5,  Text4_1_loc5,  Text5_0_loc5,  Text5_1_loc5,  Text6_0_loc5,  Text6_1_loc5,  Text7_0_loc5,  Text7_1_loc5,  Text8_0_loc5,   Text8_1_loc5,   Text9_0_loc5,   Text9_1_loc5, "
        "Text0_0_loc6,  Text0_1_loc6,  Text1_0_loc6,  Text1_1_loc6,  Text2_0_loc6,  Text2_1_loc6,  Text3_0_loc6,  Text3_1_loc1,  Text4_0_loc6,  Text4_1_loc6,  Text5_0_loc6,  Text5_1_loc6,  Text6_0_loc6,  Text6_1_loc6,  Text7_0_loc6,  Text7_1_loc6,  Text8_0_loc6,   Text8_1_loc6,   Text9_0_loc6,   Text9_1_loc6, "
        "Text0_0_loc7,  Text0_1_loc7,  Text1_0_loc7,  Text1_1_loc7,  Text2_0_loc7,  Text2_1_loc7,  Text3_0_loc7,  Text3_1_loc1,  Text4_0_loc7,  Text4_1_loc7,  Text5_0_loc7,  Text5_1_loc7,  Text6_0_loc7,  Text6_1_loc7,  Text7_0_loc7,  Text7_1_loc7,  Text8_0_loc7,   Text8_1_loc7,   Text9_0_loc7,   Text9_1_loc7, "
        "Text0_0_loc8,  Text0_1_loc8,  Text1_0_loc8,  Text1_1_loc8,  Text2_0_loc8,  Text2_1_loc8,  Text3_0_loc8,  Text3_1_loc1,  Text4_0_loc8,  Text4_1_loc8,  Text5_0_loc8,  Text5_1_loc8,  Text6_0_loc8,  Text6_1_loc8,  Text7_0_loc8,  Text7_1_loc8,  Text8_0_loc8,   Text8_1_loc8,   Text9_0_loc8,   Text9_1_loc8, "
        "Text0_0_loc9,  Text0_1_loc9,  Text1_0_loc9,  Text1_1_loc9,  Text2_0_loc9,  Text2_1_loc9,  Text3_0_loc9,  Text3_1_loc9,  Text4_0_loc9,  Text4_1_loc9,  Text5_0_loc9,  Text5_1_loc9,  Text6_0_loc9,  Text6_1_loc9,  Text7_0_loc9,  Text7_1_loc9,  Text8_0_loc9,   Text8_1_loc9,   Text9_0_loc9,   Text9_1_loc9, "
        "Text0_0_loc10, Text0_1_loc10, Text1_0_loc10, Text1_1_loc10, Text2_0_loc10, Text2_1_loc10, Text3_0_loc10, Text3_1_loc10, Text4_0_loc10, Text4_1_loc10, Text5_0_loc10, Text5_1_loc10, Text6_0_loc10, Text6_1_loc10, Text7_0_loc10, Text7_1_loc10, Text8_0_loc10,  Text8_1_loc10,  Text9_0_loc10,  Text9_1_loc10 "
        " FROM locales_npc_text");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        NpcTextLocale& data = _npcTextLocaleStore[entry];

        for (uint8 i = TOTAL_LOCALES - 1; i > 0; --i)
        {
            LocaleConstant locale = (LocaleConstant) i;
            for (uint8 j = 0; j < MAX_GOSSIP_TEXT_OPTIONS; ++j)
            {
                AddLocaleString(fields[1 + (TOTAL_LOCALES - 1) * 2 * (i - 1) + 2 * j].GetString(), locale, data.Text_0[j]);
                AddLocaleString(fields[1 + (TOTAL_LOCALES - 1) * 2 * (i - 1) + 2 * j + 1].GetString(), locale, data.Text_1[j]);
            }
        }
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %lu NpcText locale strings in %u ms", (unsigned long)_npcTextLocaleStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

//not very fast function but it is called only once a day, or on starting-up
void ObjectMgr::ReturnOrDeleteOldMails(bool serverUp)
{
    uint32 oldMSTime = getMSTime();

    time_t curTime = time(NULL);
    tm* lt = localtime(&curTime);
    uint64 basetime(curTime);
    sLog->outInfo(LOG_FILTER_GENERAL, "Returning mails current time: hour: %d, minute: %d, second: %d ", lt->tm_hour, lt->tm_min, lt->tm_sec);

    // Delete all old mails without item and without body immediately, if starting server
    if (!serverUp)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_EMPTY_EXPIRED_MAIL);
        stmt->setUInt64(0, basetime);
        CharacterDatabase.Execute(stmt);
    }
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_EXPIRED_MAIL);
    stmt->setUInt64(0, basetime);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> No expired mails found.");
        return;                                             // any mails need to be returned or deleted
    }

    std::map<uint32 /*messageId*/, MailItemInfoVec> itemsCache;
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_EXPIRED_MAIL_ITEMS);
    stmt->setUInt32(0, (uint32)basetime);
    if (PreparedQueryResult items = CharacterDatabase.Query(stmt))
    {
        MailItemInfo item;
        do
        {
            Field* fields = items->Fetch();
            item.item_guid = fields[0].GetUInt32();
            item.item_template = fields[1].GetUInt32();
            uint32 mailId = fields[2].GetUInt32();
            itemsCache[mailId].push_back(item);
        }
        while (items->NextRow());
    }

    uint32 deletedCount = 0;
    uint32 returnedCount = 0;
    do
    {
        Field* fields = result->Fetch();
        Mail* m = new Mail;
        m->messageID      = fields[0].GetUInt32();
        m->messageType    = fields[1].GetUInt8();
        m->sender         = fields[2].GetUInt32();
        m->receiver       = fields[3].GetUInt32();
        bool has_items    = fields[4].GetBool();
        m->expire_time    = time_t(fields[5].GetUInt32());
        m->deliver_time   = 0;
        m->COD            = fields[6].GetUInt64();
        m->checked        = fields[7].GetUInt8();
        m->mailTemplateId = fields[8].GetInt16();

        Player* player = NULL;
        if (serverUp)
            player = ObjectAccessor::FindPlayer((uint64)m->receiver);

        if (player)
        {                                                   // this code will run very improbably (the time is between 4 and 5 am, in game is online a player, who has old mail
            // his in mailbox and he has already listed his mails)
            delete m;
            continue;
        }

        // Delete or return mail
        if (has_items)
        {
            // read items from cache
            m->items.swap(itemsCache[m->messageID]);

            // if it is mail from non-player, or if it's already return mail, it shouldn't be returned, but deleted
            if (m->messageType != MAIL_NORMAL || (m->checked & (MAIL_CHECK_MASK_COD_PAYMENT | MAIL_CHECK_MASK_RETURNED)))
            {
                // mail open and then not returned
                for (MailItemInfoVec::iterator itr2 = m->items.begin(); itr2 != m->items.end(); ++itr2)
                {
                    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_INSTANCE);
                    stmt->setUInt32(0, itr2->item_guid);
                    CharacterDatabase.Execute(stmt);
                }
            }
            else
            {
                // Mail will be returned
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_MAIL_RETURNED);
                stmt->setUInt32(0, m->receiver);
                stmt->setUInt32(1, m->sender);
                stmt->setUInt32(2, basetime + 30 * DAY);
                stmt->setUInt32(3, basetime);
                stmt->setUInt8 (4, uint8(MAIL_CHECK_MASK_RETURNED));
                stmt->setUInt32(5, m->messageID);
                CharacterDatabase.Execute(stmt);
                for (MailItemInfoVec::iterator itr2 = m->items.begin(); itr2 != m->items.end(); ++itr2)
                {
                    // Update receiver in mail items for its proper delivery, and in instance_item for avoid lost item at sender delete
                    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_MAIL_ITEM_RECEIVER);
                    stmt->setUInt32(0, m->sender);
                    stmt->setUInt32(1, itr2->item_guid);
                    CharacterDatabase.Execute(stmt);

                    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ITEM_OWNER);
                    stmt->setUInt32(0, m->sender);
                    stmt->setUInt32(1, itr2->item_guid);
                    CharacterDatabase.Execute(stmt);
                }
                delete m;
                ++returnedCount;
                continue;
            }
        }

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_MAIL_BY_ID);
        stmt->setUInt32(0, m->messageID);
        CharacterDatabase.Execute(stmt);
        delete m;
        ++deletedCount;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Processed %u expired mails: %u deleted and %u returned in %u ms", deletedCount + returnedCount, deletedCount, returnedCount, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadQuestAreaTriggers()
{
    uint32 oldMSTime = getMSTime();

    _questAreaTriggerStore.clear();                           // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT id, quest FROM areatrigger_involvedrelation");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 quest trigger points. DB table `areatrigger_involvedrelation` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        ++count;

        Field* fields = result->Fetch();

        uint32 trigger_ID = fields[0].GetUInt32();
        uint32 quest_ID   = fields[1].GetUInt32();

        AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(trigger_ID);
        if (!atEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Area trigger (ID:%u) does not exist in `AreaTrigger.dbc`.", trigger_ID);
            continue;
        }

        Quest const* quest = GetQuestTemplate(quest_ID);

        if (!quest)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `areatrigger_involvedrelation` has record (id: %u) for not existing quest %u", trigger_ID, quest_ID);
            continue;
        }

        if (!quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `areatrigger_involvedrelation` has record (id: %u) for not quest %u, but quest not have flag QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT. Trigger or quest flags must be fixed, quest modified to require objective.", trigger_ID, quest_ID);

            // this will prevent quest completing without objective
            const_cast<Quest*>(quest)->SetSpecialFlag(QUEST_SPECIAL_FLAGS_EXPLORATION_OR_EVENT);

            // continue; - quest modified to required objective and trigger can be allowed.
        }

        _questAreaTriggerStore[trigger_ID] = quest_ID;

    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u quest trigger points in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadTavernAreaTriggers()
{
    uint32 oldMSTime = getMSTime();

    _tavernAreaTriggerStore.clear();                          // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT id FROM areatrigger_tavern");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 tavern triggers. DB table `areatrigger_tavern` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        ++count;

        Field* fields = result->Fetch();

        uint32 Trigger_ID      = fields[0].GetUInt32();

        AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(Trigger_ID);
        if (!atEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Area trigger (ID:%u) does not exist in `AreaTrigger.dbc`.", Trigger_ID);
            continue;
        }

        _tavernAreaTriggerStore.insert(Trigger_ID);
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u tavern triggers in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadAreaTriggerScripts()
{
    uint32 oldMSTime = getMSTime();

    _areaTriggerScriptStore.clear();                            // need for reload case
    QueryResult result = WorldDatabase.Query("SELECT entry, ScriptName FROM areatrigger_scripts");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 areatrigger scripts. DB table `areatrigger_scripts` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        ++count;

        Field* fields = result->Fetch();

        uint32 Trigger_ID      = fields[0].GetUInt32();
        const char* scriptName = fields[1].GetCString();

        AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(Trigger_ID);
        if (!atEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Area trigger (ID:%u) does not exist in `AreaTrigger.dbc`.", Trigger_ID);
            continue;
        }
        _areaTriggerScriptStore[Trigger_ID] = GetScriptId(scriptName);
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u areatrigger scripts in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

uint32 ObjectMgr::GetNearestTaxiNode(float x, float y, float z, uint32 mapid, uint32 team)
{
    bool found = false;
    float dist = 10000;
    uint32 id = 0;

    std::map<uint32, uint32> l_MapOverrides;

    /// Special case for taxi in garrison phased map
    for (uint32 l_I = 0; l_I < sGarrSiteLevelStore.GetNumRows(); ++l_I)
    {
        const GarrSiteLevelEntry* l_Entry = sGarrSiteLevelStore.LookupEntry(l_I);

        if (l_Entry)
        {
            l_MapOverrides[l_Entry->MapID] = MS::Garrison::Globals::BaseMap;

            if (l_Entry->MapID == mapid)
                mapid = MS::Garrison::Globals::BaseMap;
        }
    }


    uint32 requireFlag = (team == ALLIANCE) ? TAXI_NODE_FLAG_ALLIANCE : TAXI_NODE_FLAG_HORDE;
    for (uint32 i = 0; i < sTaxiNodesStore.GetNumRows(); ++i)
    {
        TaxiNodesEntry const* node = sTaxiNodesStore.LookupEntry(i);

        if (!node || !(node->Flags & requireFlag))
            continue;

        if (node->MapID != mapid)
        {
            if (l_MapOverrides.find(node->MapID) != l_MapOverrides.end() && l_MapOverrides[node->MapID] != mapid)
                continue;
        }

        uint8  field   = (uint8)((node->ID - 1) / 8);
        uint32 submask = 1 << ((node->ID - 1) % 8);

        // skip not taxi network nodes
        if ((sTaxiNodesMask[field] & submask) == 0)
            continue;

        float dist2 = (node->x - x)*(node->x - x) + (node->y - y)*(node->y - y) + (node->z - z)*(node->z - z);
        if (found)
        {
            if (dist2 < dist)
            {
                dist = dist2;
                id = node->ID;
            }
        }
        else
        {
            found = true;
            dist = dist2;
            id = node->ID;
        }
    }

    return id;
}


void ObjectMgr::GetTaxiPath(uint32 source, uint32 destination, uint32 &path, uint32 &cost)
{
    TaxiPathSetBySource::iterator src_i = sTaxiPathSetBySource.find(source);
    if (src_i == sTaxiPathSetBySource.end())
    {
        path = 0;
        cost = 0;
        return;
    }

    TaxiPathSetForSource& pathSet = src_i->second;

    TaxiPathSetForSource::iterator dest_i = pathSet.find(destination);
    if (dest_i == pathSet.end())
    {
        path = 0;
        cost = 0;
        return;
    }

    cost = dest_i->second.price;
    path = dest_i->second.ID;
}

uint32 ObjectMgr::GetTaxiMountDisplayId(uint32 id, uint32 team, bool allowed_alt_team /* = false */)
{
    uint32 mount_id = 0;

    // select mount creature id
    TaxiNodesEntry const* node = sTaxiNodesStore.LookupEntry(id);
    if (node)
    {
        uint32 mount_entry = 0;
        if (team == ALLIANCE)
            mount_entry = node->MountCreatureID[1];
        else
            mount_entry = node->MountCreatureID[0];

        // Fix for Alliance not being able to use Acherus taxi
        // only one mount type for both sides
        if (mount_entry == 0 && allowed_alt_team)
        {
            // Simply reverse the selection. At least one team in theory should have a valid mount ID to choose.
            mount_entry = team == ALLIANCE ? node->MountCreatureID[0] : node->MountCreatureID[1];
        }

        CreatureTemplate const* mount_info = GetCreatureTemplate(mount_entry);
        if (mount_info)
        {
            mount_id = mount_info->GetRandomValidModelId();
            if (!mount_id)
            {
                sLog->outError(LOG_FILTER_SQL, "No displayid found for the taxi mount with the entry %u! Can't load it!", mount_entry);
                return false;
            }
        }
    }

    // minfo is not actually used but the mount_id was updated
    GetCreatureModelRandomGender(&mount_id);

    return mount_id;
}

void ObjectMgr::LoadGraveyardZones()
{
    uint32 oldMSTime = getMSTime();

    GraveYardStore.clear();                                  // need for reload case

    //                                                0       1         2
    QueryResult result = WorldDatabase.Query("SELECT id, ghost_zone, faction FROM game_graveyard_zone");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 graveyard-zone links. DB table `game_graveyard_zone` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        ++count;

        Field* fields = result->Fetch();

        uint32 safeLocId = fields[0].GetUInt32();
        uint32 zoneId = fields[1].GetUInt32();
        uint32 team   = fields[2].GetUInt16();

        WorldSafeLocsEntry const* entry = sWorldSafeLocsStore.LookupEntry(safeLocId);
        if (!entry)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `game_graveyard_zone` has a record for not existing graveyard (WorldSafeLocs.dbc id) %u, skipped.", safeLocId);
            continue;
        }

        AreaTableEntry const* areaEntry = GetAreaEntryByAreaID(zoneId);
        if (!areaEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `game_graveyard_zone` has a record for not existing zone id (%u), skipped.", zoneId);
            continue;
        }

        if (areaEntry->ParentAreaID != 0 && zoneId != 33 && zoneId != 5287 && zoneId != 6170 && zoneId != 6176 && zoneId != 6450 && zoneId != 6451
            && zoneId != 6452 && zoneId != 6453 && zoneId != 6454 && zoneId != 6455 && zoneId != 6456 && zoneId != 6450 && zoneId != 7004 && zoneId != 7078)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `game_graveyard_zone` has a record for subzone id (%u) instead of zone, skipped.", zoneId);
            continue;
        }

        if (team != 0 && team != HORDE && team != ALLIANCE)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `game_graveyard_zone` has a record for non player faction (%u), skipped.", team);
            continue;
        }

        if (!AddGraveYardLink(safeLocId, zoneId, team, false))
            sLog->outError(LOG_FILTER_SQL, "Table `game_graveyard_zone` has a duplicate record for Graveyard (ID: %u) and Zone (ID: %u), skipped.", safeLocId, zoneId);
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u graveyard-zone links in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

WorldSafeLocsEntry const* ObjectMgr::GetDefaultGraveYard(uint32 team)
{
    enum DefaultGraveyard
    {
        HORDE_GRAVEYARD = 10, // Crossroads
        ALLIANCE_GRAVEYARD = 4, // Westfall
    };

    if (team == HORDE)
        return sWorldSafeLocsStore.LookupEntry(HORDE_GRAVEYARD);
    else if (team == ALLIANCE)
        return sWorldSafeLocsStore.LookupEntry(ALLIANCE_GRAVEYARD);
    else return NULL;
}

WorldSafeLocsEntry const* ObjectMgr::GetClosestGraveYard(float x, float y, float z, uint32 MapId, uint32 team)
{
    // search for zone associated closest graveyard
    uint32 zoneId = sMapMgr->GetZoneId(MapId, x, y, z);

    if (!zoneId)
    {
        if (z > -500)
        {
            sLog->outError(LOG_FILTER_GENERAL, "ZoneId not found for map %u coords (%f, %f, %f)", MapId, x, y, z);
            return GetDefaultGraveYard(team);
        }
    }

    // Simulate std. algorithm:
    //   found some graveyard associated to (ghost_zone, ghost_map)
    //
    //   if mapId == graveyard.mapId (ghost in plain zone or city or battleground) and search graveyard at same map
    //     then check faction
    //   if mapId != graveyard.mapId (ghost in instance) and search any graveyard associated
    //     then check faction
    GraveYardContainer::const_iterator graveLow  = GraveYardStore.lower_bound(zoneId);
    GraveYardContainer::const_iterator graveUp   = GraveYardStore.upper_bound(zoneId);
    MapEntry const* map = sMapStore.LookupEntry(MapId);
    // not need to check validity of map object; MapId _MUST_ be valid here

    if (graveLow == graveUp && !map->IsBattleArena())
    {
        sLog->outError(LOG_FILTER_SQL, "Table `game_graveyard_zone` incomplete: Zone %u Team %u does not have a linked graveyard.", zoneId, team);
        return GetDefaultGraveYard(team);
    }

    // at corpse map
    bool foundNear = false;
    float distNear = 10000;
    WorldSafeLocsEntry const* entryNear = NULL;

    // at entrance map for corpse map
    bool foundEntr = false;
    float distEntr = 10000;
    WorldSafeLocsEntry const* entryEntr = NULL;

    // some where other
    WorldSafeLocsEntry const* entryFar = NULL;

    MapEntry const* mapEntry = sMapStore.LookupEntry(MapId);

    for (GraveYardContainer::const_iterator itr = graveLow; itr != graveUp; ++itr)
    {
        GraveYardData const& data = itr->second;

        WorldSafeLocsEntry const* entry = sWorldSafeLocsStore.LookupEntry(data.safeLocId);
        if (!entry)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `game_graveyard_zone` has record for not existing graveyard (WorldSafeLocs.dbc id) %u, skipped.", data.safeLocId);
            continue;
        }

        // skip enemy faction graveyard
        // team == 0 case can be at call from .neargrave
        if (data.team != 0 && team != 0 && data.team != team)
            continue;

        // find now nearest graveyard at other map
        if (MapId != entry->map_id)
        {
            // if find graveyard at different map from where entrance placed (or no entrance data), use any first
            if (!mapEntry
                || mapEntry->CorpseMapID < 0
                || uint32(mapEntry->CorpseMapID) != entry->map_id
                || (mapEntry->CorpseX == 0 && mapEntry->CorpseY == 0))
            {
                // not have any corrdinates for check distance anyway
                entryFar = entry;
                continue;
            }

            // at entrance map calculate distance (2D);
            float dist2 = (entry->x - mapEntry->CorpseX)*(entry->x - mapEntry->CorpseX)
                + (entry->y - mapEntry->CorpseY)*(entry->y - mapEntry->CorpseY);

            if (foundEntr)
            {
                if (dist2 < distEntr)
                {
                    distEntr = dist2;
                    entryEntr = entry;
                }
            }
            else
            {
                foundEntr = true;
                distEntr = dist2;
                entryEntr = entry;
            }
        }
        // find now nearest graveyard at same map
        else
        {
            float dist2 = (entry->x - x)*(entry->x - x)+(entry->y - y)*(entry->y - y)+(entry->z - z)*(entry->z - z);
            if (foundNear)
            {
                if (dist2 < distNear)
                {
                    distNear = dist2;
                    entryNear = entry;
                }
            }
            else
            {
                foundNear = true;
                distNear = dist2;
                entryNear = entry;
            }
        }
    }

    if (entryNear)
        return entryNear;

    if (entryEntr)
        return entryEntr;

    return entryFar;
}

GraveYardData const* ObjectMgr::FindGraveYardData(uint32 id, uint32 zoneId)
{
    GraveYardContainer::const_iterator graveLow  = GraveYardStore.lower_bound(zoneId);
    GraveYardContainer::const_iterator graveUp   = GraveYardStore.upper_bound(zoneId);

    for (GraveYardContainer::const_iterator itr = graveLow; itr != graveUp; ++itr)
    {
        if (itr->second.safeLocId == id)
            return &itr->second;
    }

    return NULL;
}

bool ObjectMgr::AddGraveYardLink(uint32 id, uint32 zoneId, uint32 team, bool persist /*= true*/)
{
    if (FindGraveYardData(id, zoneId))
        return false;

    // add link to loaded data
    GraveYardData data;
    data.safeLocId = id;
    data.team = team;

    GraveYardStore.insert(GraveYardContainer::value_type(zoneId, data));

    // add link to DB
    if (persist)
    {
        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_INS_GRAVEYARD_ZONE);

        stmt->setUInt32(0, id);
        stmt->setUInt32(1, zoneId);
        stmt->setUInt16(2, uint16(team));

        WorldDatabase.Execute(stmt);
    }

    return true;
}

void ObjectMgr::RemoveGraveYardLink(uint32 id, uint32 zoneId, uint32 team, bool persist /*= false*/)
{
    GraveYardContainer::iterator graveLow  = GraveYardStore.lower_bound(zoneId);
    GraveYardContainer::iterator graveUp   = GraveYardStore.upper_bound(zoneId);
    if (graveLow == graveUp)
    {
        //sLog->outError(LOG_FILTER_SQL, "Table `game_graveyard_zone` incomplete: Zone %u Team %u does not have a linked graveyard.", zoneId, team);
        return;
    }

    bool found = false;

    GraveYardContainer::iterator itr;

    for (itr = graveLow; itr != graveUp; ++itr)
    {
        GraveYardData & data = itr->second;

        // skip not matching safezone id
        if (data.safeLocId != id)
            continue;

        // skip enemy faction graveyard at same map (normal area, city, or battleground)
        // team == 0 case can be at call from .neargrave
        if (data.team != 0 && team != 0 && data.team != team)
            continue;

        found = true;
        break;
    }

    // no match, return
    if (!found)
        return;

    // remove from links
    GraveYardStore.erase(itr);

    // remove link from DB
    if (persist)
    {
        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_DEL_GRAVEYARD_ZONE);

        stmt->setUInt32(0, id);
        stmt->setUInt32(1, zoneId);
        stmt->setUInt16(2, uint16(team));

        WorldDatabase.Execute(stmt);
    }
}

void ObjectMgr::LoadAreaTriggerTeleports()
{
    uint32 oldMSTime = getMSTime();

    _areaTriggerStore.clear();                                  // need for reload case

    //                                                        0            1                  2                  3                  4                   5
    QueryResult result = WorldDatabase.Query("SELECT id,  target_map, target_position_x, target_position_y, target_position_z, target_orientation FROM areatrigger_teleport");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 area trigger teleport definitions. DB table `areatrigger_teleport` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        ++count;

        uint32 Trigger_ID = fields[0].GetUInt32();

        AreaTriggerStruct at;

        at.target_mapId             = fields[1].GetUInt16();
        at.target_X                 = fields[2].GetFloat();
        at.target_Y                 = fields[3].GetFloat();
        at.target_Z                 = fields[4].GetFloat();
        at.target_Orientation       = fields[5].GetFloat();

        AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(Trigger_ID);
        if (!atEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Area trigger (ID:%u) does not exist in `AreaTrigger.dbc`.", Trigger_ID);
            continue;
        }

        MapEntry const* mapEntry = sMapStore.LookupEntry(at.target_mapId);
        if (!mapEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Area trigger (ID:%u) target map (ID: %u) does not exist in `Map.dbc`.", Trigger_ID, at.target_mapId);
            continue;
        }

        if (at.target_X == 0 && at.target_Y == 0 && at.target_Z == 0)
        {
            sLog->outError(LOG_FILTER_SQL, "Area trigger (ID:%u) target coordinates not provided.", Trigger_ID);
            continue;
        }

        _areaTriggerStore[Trigger_ID] = at;

    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u area trigger teleport definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadAccessRequirements()
{
    uint32 oldMSTime = getMSTime();

    _accessRequirementStore.clear();                                  // need for reload case

    //                                               0      1           2          3          4     5      6             7             8                      9                     10             11               12
    QueryResult result = WorldDatabase.Query("SELECT mapid, difficulty, level_min, level_max, item, item2, quest_done_A, quest_done_H, completed_achievement, leader_achievement, itemlevel_min, itemlevel_max, quest_failed_text FROM access_requirement");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 access requirement definitions. DB table `access_requirement` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        ++count;

        uint32 mapid = fields[0].GetUInt32();
        uint8 difficulty = fields[1].GetUInt8();
        uint32 requirement_ID = MAKE_PAIR32(mapid, difficulty);

        AccessRequirement ar;

        ar.levelMin                 = fields[2].GetUInt8();
        ar.levelMax                 = fields[3].GetUInt8();
        ar.item                     = fields[4].GetUInt32();
        ar.item2                    = fields[5].GetUInt32();
        ar.quest_A                  = fields[6].GetUInt32();
        ar.quest_H                  = fields[7].GetUInt32();
        ar.achievement              = fields[8].GetUInt32();
        ar.leader_achievement       = fields[9].GetUInt32();
        ar.itemlevelMin             = fields[10].GetUInt32();
        ar.itemlevelMax             = fields[11].GetUInt32();
        ar.questFailedText          = fields[12].GetString();

        if (ar.item)
        {
            ItemTemplate const* pProto = GetItemTemplate(ar.item);
            if (!pProto)
            {
                sLog->outError(LOG_FILTER_GENERAL, "Key item %u does not exist for map %u difficulty %u, removing key requirement.", ar.item, mapid, difficulty);
                ar.item = 0;
            }
        }

        if (ar.item2)
        {
            ItemTemplate const* pProto = GetItemTemplate(ar.item2);
            if (!pProto)
            {
                sLog->outError(LOG_FILTER_GENERAL, "Second item %u does not exist for map %u difficulty %u, removing key requirement.", ar.item2, mapid, difficulty);
                ar.item2 = 0;
            }
        }

        if (ar.quest_A)
        {
            if (!GetQuestTemplate(ar.quest_A))
            {
                sLog->outError(LOG_FILTER_SQL, "Required Alliance Quest %u not exist for map %u difficulty %u, remove quest done requirement.", ar.quest_A, mapid, difficulty);
                ar.quest_A = 0;
            }
        }

        if (ar.quest_H)
        {
            if (!GetQuestTemplate(ar.quest_H))
            {
                sLog->outError(LOG_FILTER_SQL, "Required Horde Quest %u not exist for map %u difficulty %u, remove quest done requirement.", ar.quest_H, mapid, difficulty);
                ar.quest_H = 0;
            }
        }

        if (ar.achievement)
        {
            if (!sAchievementStore.LookupEntry(ar.achievement))
            {
                sLog->outError(LOG_FILTER_SQL, "Required Achievement %u not exist for map %u difficulty %u, remove quest done requirement.", ar.achievement, mapid, difficulty);
                ar.achievement = 0;
            }
        }

        if (ar.leader_achievement)
        {
            if (!sAchievementMgr->GetAchievement(ar.leader_achievement))
            {
                sLog->outError(LOG_FILTER_SQL, "Required Leader achievement %u not exist for map %u difficulty %u, remove quest done requirement.", ar.leader_achievement, mapid, difficulty);
                ar.leader_achievement = 0;
            }
        }

        _accessRequirementStore[requirement_ID] = ar;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u access requirement definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadLFRAccessRequirements()
{
    uint32 l_OldMSTime = 0;

    /// Needed for reload case
    m_LFRAccessRequirements.clear();

    ///                                                    0            1         2       3      4      5         6          7              8               9         10           11
    QueryResult l_Result = WorldDatabase.Query("SELECT dungeon_id, level_min, level_max, item, item2, quest_A, quest_H, achievement, leader_achievement, ilvl_min, ilvl_max, quest_failed_text FROM lfr_access_requirement");
    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 LFR access requirement definitions. DB table `lfr_access_requirement` is empty.");
        return;
    }

    uint32 l_Count = 0;

    do
    {
        LFRAccessRequirement l_LFRAccessReq;

        Field* l_Fields                     = l_Result->Fetch();
        uint32 l_Index                      = 0;
        uint32 l_DungeonID                  = l_Fields[l_Index++].GetUInt32();

        l_LFRAccessReq.LevelMin             = l_Fields[l_Index++].GetUInt8();
        l_LFRAccessReq.LevelMax             = l_Fields[l_Index++].GetUInt8();
        l_LFRAccessReq.Item                 = l_Fields[l_Index++].GetUInt32();
        l_LFRAccessReq.Item2                = l_Fields[l_Index++].GetUInt32();
        l_LFRAccessReq.QuestA               = l_Fields[l_Index++].GetUInt32();
        l_LFRAccessReq.QuestH               = l_Fields[l_Index++].GetUInt32();
        l_LFRAccessReq.Achievement          = l_Fields[l_Index++].GetUInt32();
        l_LFRAccessReq.LeaderAchievement    = l_Fields[l_Index++].GetUInt32();
        l_LFRAccessReq.ItemLevelMin         = l_Fields[l_Index++].GetUInt32();
        l_LFRAccessReq.ItemLevelMax         = l_Fields[l_Index++].GetUInt32();
        l_LFRAccessReq.QuestFailedText      = l_Fields[l_Index++].GetString();

        if (l_LFRAccessReq.Item)
        {
            ItemTemplate const* l_ItemTemplate = GetItemTemplate(l_LFRAccessReq.Item);
            if (!l_ItemTemplate)
            {
                sLog->outError(LOG_FILTER_GENERAL, "Key item %u does not exist for dungeon ID %u, removing key requirement.", l_LFRAccessReq.Item, l_DungeonID);
                l_LFRAccessReq.Item = 0;
            }
        }

        if (l_LFRAccessReq.Item2)
        {
            ItemTemplate const* l_ItemTemplate = GetItemTemplate(l_LFRAccessReq.Item2);
            if (!l_ItemTemplate)
            {
                sLog->outError(LOG_FILTER_GENERAL, "Second item %u does not exist for dungeon ID %u, removing key requirement.", l_LFRAccessReq.Item2, l_DungeonID);
                l_LFRAccessReq.Item2 = 0;
            }
        }

        if (l_LFRAccessReq.QuestA)
        {
            if (!GetQuestTemplate(l_LFRAccessReq.QuestA))
            {
                sLog->outError(LOG_FILTER_SQL, "Required Alliance Quest %u not exist for dungeon ID %u, remove quest done requirement.", l_LFRAccessReq.QuestA, l_DungeonID);
                l_LFRAccessReq.QuestA = 0;
            }
        }

        if (l_LFRAccessReq.QuestH)
        {
            if (!GetQuestTemplate(l_LFRAccessReq.QuestH))
            {
                sLog->outError(LOG_FILTER_SQL, "Required Horde Quest %u not exist for dungeon ID %u, remove quest done requirement.", l_LFRAccessReq.QuestH, l_DungeonID);
                l_LFRAccessReq.QuestH = 0;
            }
        }

        if (l_LFRAccessReq.Achievement)
        {
            if (!sAchievementStore.LookupEntry(l_LFRAccessReq.Achievement))
            {
                sLog->outError(LOG_FILTER_SQL, "Required Achievement %u not exist for dungeon ID %u, remove quest done requirement.", l_LFRAccessReq.Achievement, l_DungeonID);
                l_LFRAccessReq.Achievement = 0;
            }
        }

        if (l_LFRAccessReq.LeaderAchievement)
        {
            if (!sAchievementMgr->GetAchievement(l_LFRAccessReq.LeaderAchievement))
            {
                sLog->outError(LOG_FILTER_SQL, "Required Leader achievement %u not exist for dungeon ID %u, remove quest done requirement.", l_LFRAccessReq.LeaderAchievement, l_DungeonID);
                l_LFRAccessReq.LeaderAchievement = 0;
            }
        }

        m_LFRAccessRequirements[l_DungeonID] = l_LFRAccessReq;
        ++l_Count;
    }
    while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u lfr access requirement definitions in %u ms", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

/*
 * Searches for the areatrigger which teleports players out of the given map with instance_template.parent field support
 */
AreaTriggerStruct const* ObjectMgr::GetGoBackTrigger(uint32 Map) const
{
    bool useParentDbValue = false;
    uint32 parentId = 0;
    const MapEntry* mapEntry = sMapStore.LookupEntry(Map);
    if (!mapEntry || mapEntry->CorpseMapID < 0)
        return NULL;

    if (mapEntry->IsDungeon())
    {
        const InstanceTemplate* iTemplate = sObjectMgr->GetInstanceTemplate(Map);

        if (!iTemplate)
            return NULL;

        parentId = iTemplate->Parent;
        useParentDbValue = true;
    }

    uint32 entrance_map = uint32(mapEntry->CorpseMapID);
    for (AreaTriggerContainer::const_iterator itr = _areaTriggerStore.begin(); itr != _areaTriggerStore.end(); ++itr)
        if ((!useParentDbValue && itr->second.target_mapId == entrance_map) || (useParentDbValue && itr->second.target_mapId == parentId))
        {
            AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(itr->first);
            if (atEntry && atEntry->ContinentID == Map)
                return &itr->second;
        }
    return NULL;
}

/**
 * Searches for the areatrigger which teleports players to the given map
 */
AreaTriggerStruct const* ObjectMgr::GetMapEntranceTrigger(uint32 Map) const
{
    for (AreaTriggerContainer::const_iterator itr = _areaTriggerStore.begin(); itr != _areaTriggerStore.end(); ++itr)
    {
        if (itr->second.target_mapId == Map)
        {
            AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(itr->first);
            if (atEntry)
                return &itr->second;
        }
    }
    return NULL;
}

void ObjectMgr::SetHighestGuids()
{
    QueryResult result = CharacterDatabase.Query("SELECT MAX(guid) FROM characters");
    if (result)
        _hiCharGuid = (*result)[0].GetUInt32()+1;

    result = WorldDatabase.Query("SELECT MAX(guid) FROM creature");
    if (result)
        _hiCreatureGuid = (*result)[0].GetUInt32()+1;

    _hiCreatureGuid += 10000;

    result = CharacterDatabase.Query("SELECT MAX(guid) FROM item_instance");
    if (result)
        m_HighItemGuid = (*result)[0].GetUInt32()+1;

    // Cleanup other tables from not existed guids ( >= _hiItemGuid)
    CharacterDatabase.PExecute("DELETE FROM character_inventory WHERE item >= '%u'", (uint32)m_HighItemGuid);      // One-time query
    CharacterDatabase.PExecute("DELETE FROM mail_items WHERE item_guid >= '%u'", (uint32)m_HighItemGuid);          // One-time query
    CharacterDatabase.PExecute("DELETE FROM auctionhouse WHERE itemguid >= '%u'", (uint32)m_HighItemGuid);         // One-time query
    CharacterDatabase.PExecute("DELETE FROM guild_bank_item WHERE item_guid >= '%u'", (uint32)m_HighItemGuid);     // One-time query

    result = WorldDatabase.Query("SELECT MAX(guid) FROM gameobject");
    if (result)
        _hiGoGuid = (*result)[0].GetUInt32()+1;

    result = WorldDatabase.Query("SELECT MAX(guid) FROM transports");
    if (result)
        _hiMoTransGuid = (*result)[0].GetUInt32()+1;

    result = CharacterDatabase.Query("SELECT MAX(id) FROM auctionhouse");
    if (result)
        _auctionId = (*result)[0].GetUInt32()+1;

    result = CharacterDatabase.Query("SELECT MAX(id) FROM mail");
    if (result)
        _mailId = (*result)[0].GetUInt32()+1;

    result = CharacterDatabase.Query("SELECT MAX(corpseGuid) FROM corpse");
    if (result)
        _hiCorpseGuid = (*result)[0].GetUInt32()+1;

    result = CharacterDatabase.Query("SELECT MAX(setguid) FROM character_equipmentsets");
    if (result)
        _equipmentSetGuid = (*result)[0].GetUInt64()+1;

    result = CharacterDatabase.Query("SELECT MAX(guildId) FROM guild");
    if (result)
        sGuildMgr->SetNextGuildId((*result)[0].GetUInt32()+1);

    result = CharacterDatabase.Query("SELECT MAX(guid) FROM groups");
    if (result)
        sGroupMgr->SetGroupDbStoreSize((*result)[0].GetUInt32()+1);

    result = CharacterDatabase.Query("SELECT MAX(itemId) from character_void_storage");
    if (result)
        _voidItemId = (*result)[0].GetUInt64()+1;

    result = CharacterDatabase.Query("SELECT MAX(id) from character_garrison");
    if (result)
        m_GarrisonID = (*result)[0].GetUInt32() + 1;

    result = CharacterDatabase.Query("SELECT MAX(id) from character_garrison_building");
    if (result)
        m_GarrisonBuildingID = (*result)[0].GetUInt32() + 1;

    result = CharacterDatabase.Query("SELECT MAX(id) from character_garrison_follower");
    if (result)
        m_GarrisonFollowerID = (*result)[0].GetUInt32() + 1;

    result = CharacterDatabase.Query("SELECT MAX(id) from character_garrison_mission");
    if (result)
        m_GarrisonMissionID = (*result)[0].GetUInt32() + 1;

    result = CharacterDatabase.Query("SELECT MAX(id) from character_garrison_work_order");
    if (result)
        m_GarrisonWorkOrderID = (*result)[0].GetUInt32() + 1;

    m_StandaloneSceneInstanceID = 1;
}

uint32 ObjectMgr::GenerateAuctionID()
{
    if (_auctionId >= 0xFFFFFFFE)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Auctions ids overflow!! Can't continue, shutting down server. ");
        World::StopNow(ERROR_EXIT_CODE);
    }
    return _auctionId++;
}

uint64 ObjectMgr::GenerateEquipmentSetGuid()
{
    if (_equipmentSetGuid >= uint64(0xFFFFFFFFFFFFFFFELL))
    {
        sLog->outError(LOG_FILTER_GENERAL, "EquipmentSet guid overflow!! Can't continue, shutting down server. ");
        World::StopNow(ERROR_EXIT_CODE);
    }
    return _equipmentSetGuid++;
}

uint32 ObjectMgr::GenerateMailID()
{
    if (_mailId >= 0xFFFFFFFE)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Mail ids overflow!! Can't continue, shutting down server. ");
        World::StopNow(ERROR_EXIT_CODE);
    }
    return _mailId++;
}

uint32 ObjectMgr::GenerateLowGuid(HighGuid guidhigh)
{
    switch (guidhigh)
    {
        case HIGHGUID_ITEM:
        {
            ASSERT(m_HighItemGuid < 0xFFFFFFFE && "Item guid overflow!");
            return m_HighItemGuid.fetch_add(1);
        }
        case HIGHGUID_UNIT:
        {
            ASSERT(_hiCreatureGuid < 0xFFFFFFFE && "Creature guid overflow!");
            return _hiCreatureGuid++;
        }
        case HIGHGUID_PET:
        {
            ASSERT(_hiPetGuid < 0xFFFFFFFE && "Pet guid overflow!");
            return _hiPetGuid++;
        }
        case HIGHGUID_VEHICLE:
        {
            ASSERT(_hiVehicleGuid < 0xFFFFFFFE && "Vehicle guid overflow!");
            return _hiVehicleGuid++;
        }
        case HIGHGUID_PLAYER:
        {
            ASSERT(_hiCharGuid < 0xFFFFFFFE && "Player guid overflow!");
            return _hiCharGuid++;
        }
        case HIGHGUID_GAMEOBJECT:
        {
            ASSERT(_hiGoGuid < 0xFFFFFFFE && "Gameobject guid overflow!");
            return _hiGoGuid++;
        }
        case HIGHGUID_CORPSE:
        {
            ASSERT(_hiCorpseGuid < 0xFFFFFFFE && "Corpse guid overflow!");
            return _hiCorpseGuid++;
        }
        case HIGHGUID_DYNAMICOBJECT:
        {
            ASSERT(_hiDoGuid < 0xFFFFFFFE && "DynamicObject guid overflow!");
            return _hiDoGuid++;
        }
        case HIGHGUID_AREATRIGGER:
        {
            ASSERT(_hiAreaTriggerGuid < 0xFFFFFFFE && "AreaTrigger guid overflow!");
            return _hiAreaTriggerGuid++;
        }
        case HIGHGUID_MO_TRANSPORT:
        {
            ASSERT(_hiMoTransGuid < 0xFFFFFFFE && "MO Transport guid overflow!");
            return _hiMoTransGuid++;
        }
        default:
            ASSERT(false && "ObjectMgr::GenerateLowGuid - Unknown HIGHGUID type");
            return 0;
    }
}

void ObjectMgr::LoadGameObjectLocales()
{
    uint32 oldMSTime = getMSTime();

    _gameObjectLocaleStore.clear();                           // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, "
        "name_loc1, name_loc2, name_loc3, name_loc4, name_loc5, name_loc6, name_loc7, name_loc8, name_loc9, name_loc10, "
        "castbarcaption_loc1, castbarcaption_loc2, castbarcaption_loc3, castbarcaption_loc4, "
        "castbarcaption_loc5, castbarcaption_loc6, castbarcaption_loc7, castbarcaption_loc8, castbarcaption_loc9, castbarcaption_loc10 FROM locales_gameobject");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        GameObjectLocale& data = _gameObjectLocaleStore[entry];

        for (uint8 i = 1; i < TOTAL_LOCALES; ++i)
            AddLocaleString(fields[i].GetString(), LocaleConstant(i), data.Name);

        for (uint8 i = 1; i < TOTAL_LOCALES; ++i)
            AddLocaleString(fields[i + (TOTAL_LOCALES - 1)].GetString(), LocaleConstant(i), data.CastBarCaption);
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %lu gameobject locale strings in %u ms", (unsigned long)_gameObjectLocaleStore.size(), GetMSTimeDiffToNow(oldMSTime));
}

inline void CheckGOLockId(GameObjectTemplate const* goInfo, uint32 dataN, uint32 N)
{
    if (sLockStore.LookupEntry(dataN))
        return;

    sLog->outError(LOG_FILTER_SQL, "Gameobject (Entry: %u GoType: %u) have data%d=%u but lock (Id: %u) not found.",
        goInfo->entry, goInfo->type, N, goInfo->door.open, goInfo->door.open);
}

inline void CheckGOLinkedTrapId(GameObjectTemplate const* goInfo, uint32 dataN, uint32 N)
{
    if (GameObjectTemplate const* trapInfo = sObjectMgr->GetGameObjectTemplate(dataN))
    {
        if (trapInfo->type != GAMEOBJECT_TYPE_TRAP)
            sLog->outError(LOG_FILTER_SQL, "Gameobject (Entry: %u GoType: %u) have data%d=%u but GO (Entry %u) have not GAMEOBJECT_TYPE_TRAP (%u) type.",
            goInfo->entry, goInfo->type, N, dataN, dataN, GAMEOBJECT_TYPE_TRAP);
    }
}

inline void CheckGOSpellId(GameObjectTemplate const* goInfo, uint32 dataN, uint32 N)
{
    if (sSpellMgr->GetSpellInfo(dataN))
        return;

    sLog->outError(LOG_FILTER_SQL, "Gameobject (Entry: %u GoType: %u) have data%d=%u but Spell (Entry %u) not exist.",
        goInfo->entry, goInfo->type, N, dataN, dataN);
}

inline void CheckAndFixGOChairHeightId(GameObjectTemplate const* goInfo, uint32 const& dataN, uint32 N)
{
    if (dataN <= (UNIT_STAND_STATE_SIT_HIGH_CHAIR-UNIT_STAND_STATE_SIT_LOW_CHAIR))
        return;

    sLog->outError(LOG_FILTER_SQL, "Gameobject (Entry: %u GoType: %u) have data%d=%u but correct chair height in range 0..%i.",
        goInfo->entry, goInfo->type, N, dataN, UNIT_STAND_STATE_SIT_HIGH_CHAIR-UNIT_STAND_STATE_SIT_LOW_CHAIR);

    // prevent client and server unexpected work
    const_cast<uint32&>(dataN) = 0;
}

inline void CheckGONoDamageImmuneId(GameObjectTemplate* goTemplate, uint32 dataN, uint32 N)
{
    // 0/1 correct values
    if (dataN <= 1)
        return;

    sLog->outError(LOG_FILTER_SQL, "Gameobject (Entry: %u GoType: %u) have data%d=%u but expected boolean (0/1) noDamageImmune field value.", goTemplate->entry, goTemplate->type, N, dataN);
}

inline void CheckGOConsumable(GameObjectTemplate const* goInfo, uint32 dataN, uint32 N)
{
    // 0/1 correct values
    if (dataN <= 1)
        return;

    sLog->outError(LOG_FILTER_SQL, "Gameobject (Entry: %u GoType: %u) have data%d=%u but expected boolean (0/1) consumable field value.",
        goInfo->entry, goInfo->type, N, dataN);
}

void ObjectMgr::LoadGameObjectTemplate()
{
    uint32 oldMSTime = getMSTime();

    //                                                 0      1      2        3       4             5          6      7       8     9        10         11          12
    QueryResult result = WorldDatabase.Query("SELECT entry, type, displayId, name, IconName, castBarCaption, unk1, faction, flags, size, questItem1, questItem2, questItem3, "
    //                                            13          14          15       16     17     18     19     20     21     22     23     24     25      26      27      28
                                             "questItem4, questItem5, questItem6, data0, data1, data2, data3, data4, data5, data6, data7, data8, data9, data10, data11, data12, "
    //                                          29      30      31      32      33      34      35      36      37      38      39      40      41      42      43      44
                                             "data13, data14, data15, data16, data17, data18, data19, data20, data21, data22, data23, data24, data25, data26, data27, data28, "
    //                                          45      46      47       48       49        50      51
                                             "data29, data30, data31,  data32, unkInt32, AIName, ScriptName "
                                             "FROM gameobject_template");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 gameobject definitions. DB table `gameobject_template` is empty.");
        return;
    }

    //_gameObjectTemplateStore.rehash(result->GetRowCount());
    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        GameObjectTemplate& got = _gameObjectTemplateStore[entry];

        got.entry          = entry;
        got.type           = uint32(fields[1].GetUInt8());
        got.displayId      = fields[2].GetUInt32();
        got.name           = fields[3].GetString();
        got.IconName       = fields[4].GetString();
        got.castBarCaption = fields[5].GetString();
        got.unk1           = fields[6].GetString();
        got.faction        = uint32(fields[7].GetUInt16());
        got.flags          = fields[8].GetUInt32();
        got.size           = fields[9].GetFloat();

        for (uint8 i = 0; i < MAX_GAMEOBJECT_QUEST_ITEMS; ++i)
            got.questItems[i] = fields[10 + i].GetUInt32();

        for (uint8 i = 0; i < MAX_GAMEOBJECT_DATA; ++i)
            got.raw.data[i] = fields[16 + i].GetUInt32();

        got.unkInt32 = fields[49].GetInt32();
        got.AIName = fields[50].GetString();
        got.ScriptId = GetScriptId(fields[51].GetCString());

        // Checks

        switch (got.type)
        {
            case GAMEOBJECT_TYPE_DOOR:                      //0
            {
                if (got.door.open)
                    CheckGOLockId(&got, got.door.open, 1);
                CheckGONoDamageImmuneId(&got, got.door.noDamageImmune, 3);
                break;
            }
            case GAMEOBJECT_TYPE_BUTTON:                    //1
            {
                if (got.button.open)
                    CheckGOLockId(&got, got.button.open, 1);
                CheckGONoDamageImmuneId(&got, got.button.noDamageImmune, 4);
                break;
            }
            case GAMEOBJECT_TYPE_QUESTGIVER:                //2
            {
                if (got.questgiver.open)
                    CheckGOLockId(&got, got.questgiver.open, 0);
                CheckGONoDamageImmuneId(&got, got.questgiver.noDamageImmune, 5);
                break;
            }
            case GAMEOBJECT_TYPE_CHEST:                     //3
            {
                if (got.chest.open)
                    CheckGOLockId(&got, got.chest.open, 0);

                CheckGOConsumable(&got, got.chest.consumable, 3);

                if (got.chest.linkedTrap)              // linked trap
                    CheckGOLinkedTrapId(&got, got.chest.linkedTrap, 7);
                break;
            }
            case GAMEOBJECT_TYPE_TRAP:                      //6
            {
                if (got.trap.open)
                    CheckGOLockId(&got, got.trap.open, 0);
                break;
            }
            case GAMEOBJECT_TYPE_CHAIR:                     //7
                CheckAndFixGOChairHeightId(&got, got.chair.chairheight, 1);
                break;
            case GAMEOBJECT_TYPE_SPELL_FOCUS:               //8
            {
                if (got.spellFocus.spellFocusType)
                {
                    if (!sSpellFocusObjectStore.LookupEntry(got.spellFocus.spellFocusType))
                        sLog->outError(LOG_FILTER_SQL, "GameObject (Entry: %u GoType: %u) have data0=%u but SpellFocus (Id: %u) not exist.",
                        entry, got.type, got.spellFocus.spellFocusType, got.spellFocus.spellFocusType);
                }

                if (got.spellFocus.linkedTrap)        // linked trap
                    CheckGOLinkedTrapId(&got, got.spellFocus.linkedTrap, 2);
                break;
            }
            case GAMEOBJECT_TYPE_GOOBER:                    //10
            {
                if (got.goober.open)
                    CheckGOLockId(&got, got.goober.open, 0);

                CheckGOConsumable(&got, got.goober.consumable, 3);

                if (got.goober.pageID)                  // pageId
                {
                    if (!GetPageText(got.goober.pageID))
                        sLog->outError(LOG_FILTER_SQL, "GameObject (Entry: %u GoType: %u) have data7=%u but PageText (Entry %u) not exist.",
                        entry, got.type, got.goober.pageID, got.goober.pageID);
                }
                CheckGONoDamageImmuneId(&got, got.goober.noDamageImmune, 11);
                if (got.goober.linkedTrap)            // linked trap
                    CheckGOLinkedTrapId(&got, got.goober.linkedTrap, 12);
                break;
            }
            case GAMEOBJECT_TYPE_AREADAMAGE:                //12
            {
                if (got.areadamage.open)
                    CheckGOLockId(&got, got.areadamage.open, 0);
                break;
            }
            case GAMEOBJECT_TYPE_CAMERA:                    //13
            {
                if (got.camera.open)
                    CheckGOLockId(&got, got.camera.open, 0);
                break;
            }
            case GAMEOBJECT_TYPE_MAP_OBJ_TRANSPORT:              //15
            {
                if (got.moTransport.taxiPathID)
                {
                    if (got.moTransport.taxiPathID >= sTaxiPathNodesByPath.size() || sTaxiPathNodesByPath[got.moTransport.taxiPathID].empty())
                        sLog->outError(LOG_FILTER_SQL, "GameObject (Entry: %u GoType: %u) have data0=%u but TaxiPath (Id: %u) not exist.",
                        entry, got.type, got.moTransport.taxiPathID, got.moTransport.taxiPathID);
                }
                break;
            }
            case GAMEOBJECT_TYPE_RITUAL:          //18
                break;
            case GAMEOBJECT_TYPE_SPELLCASTER:               //22
            {
                // always must have spell
                CheckGOSpellId(&got, got.spellcaster.spell, 0);
                break;
            }
            case GAMEOBJECT_TYPE_FLAGSTAND:                 //24
            {
                if (got.flagstand.open)
                    CheckGOLockId(&got, got.flagstand.open, 0);
                CheckGONoDamageImmuneId(&got, got.flagstand.noDamageImmune, 5);
                break;
            }
            case GAMEOBJECT_TYPE_FISHINGHOLE:               //25
            {
                if (got.fishinghole.open)
                    CheckGOLockId(&got, got.fishinghole.open, 4);
                break;
            }
            case GAMEOBJECT_TYPE_FLAGDROP:                  //26
            {
                if (got.flagdrop.open)
                    CheckGOLockId(&got, got.flagdrop.open, 0);
                CheckGONoDamageImmuneId(&got, got.flagdrop.noDamageImmune, 3);
                break;
            }
            case GAMEOBJECT_TYPE_BARBER_CHAIR:              //32
                CheckAndFixGOChairHeightId(&got, got.barberChair.chairheight, 0);
                break;
        }

       ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u game object templates in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadGarrisonPlotBuildingContent()
{
    uint32 l_StartTime = getMSTime();

    QueryResult l_Result = WorldDatabase.Query("SELECT id, plot_type_or_building, faction_index, creature_or_gob, x, y, z, o FROM garrison_plot_content");

    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 garrison plot building content. DB table `garrison_plot_building_content` is empty.");
        return;
    }

    uint32 l_Count = 0;

    do
    {
        Field* l_Fields = l_Result->Fetch();

        GarrisonPlotBuildingContent l_Content;
        l_Content.DB_ID                 = l_Fields[0].GetUInt32();
        l_Content.PlotTypeOrBuilding    = l_Fields[1].GetInt32();
        l_Content.FactionIndex          = l_Fields[2].GetUInt32();
        l_Content.CreatureOrGob         = l_Fields[3].GetInt32();
        l_Content.X                     = l_Fields[4].GetFloat();
        l_Content.Y                     = l_Fields[5].GetFloat();
        l_Content.Z                     = l_Fields[6].GetFloat();
        l_Content.O                     = l_Fields[7].GetFloat();

        m_GarrisonPlotBuildingContents.push_back(l_Content);

        ++l_Count;
    } while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u garrison plot building content in %u ms", l_Count, GetMSTimeDiffToNow(l_StartTime));
}

void ObjectMgr::AddGarrisonPlotBuildingContent(GarrisonPlotBuildingContent & p_Data)
{
    WorldDatabase.PQuery("INSERT INTO garrison_plot_content(plot_type_or_building, faction_index, creature_or_gob, x, y, z, o) VALUES "
        "(%d, %u, %d, %f, %f, %f, %f) ", p_Data.PlotTypeOrBuilding, p_Data.FactionIndex, p_Data.CreatureOrGob, p_Data.X, p_Data.Y, p_Data.Z, p_Data.O);

    QueryResult l_Result = WorldDatabase.PQuery("SELECT id FROM garrison_plot_content WHERE plot_type_or_building=%d AND faction_index=%u AND creature_or_gob=%d AND x BETWEEN %f AND %f AND y BETWEEN %f AND %f AND z BETWEEN %f AND %f",
                                                p_Data.PlotTypeOrBuilding, p_Data.FactionIndex, p_Data.CreatureOrGob, p_Data.X - 0.5f, p_Data.X + 0.5f, p_Data.Y - 0.5f, p_Data.Y + 0.5f, p_Data.Z - 0.5f, p_Data.Z + 0.5f);

    if (!l_Result)
        return;

    Field* l_Fields = l_Result->Fetch();

    p_Data.DB_ID = l_Fields[0].GetUInt32();

    m_GarrisonPlotBuildingContents.push_back(p_Data);
}

void ObjectMgr::DeleteGarrisonPlotBuildingContent(GarrisonPlotBuildingContent & p_Data)
{
    auto l_It = std::find_if(m_GarrisonPlotBuildingContents.begin(), m_GarrisonPlotBuildingContents.end(), [p_Data](const GarrisonPlotBuildingContent & p_Elem) -> bool
    {
        return p_Elem.DB_ID == p_Data.DB_ID;
    });

    if (l_It != m_GarrisonPlotBuildingContents.end())
    {
        WorldDatabase.PQuery("DELETE FROM garrison_plot_content WHERE id=%u", p_Data.DB_ID);
        m_GarrisonPlotBuildingContents.erase(l_It);
    }
}

std::vector<GarrisonPlotBuildingContent> ObjectMgr::GetGarrisonPlotBuildingContent(int32 p_PlotTypeOrBuilding, uint32 p_FactionIndex)
{
    std::vector<GarrisonPlotBuildingContent> l_Data;

    for (uint32 l_I = 0; l_I < m_GarrisonPlotBuildingContents.size(); ++l_I)
    {
        if (m_GarrisonPlotBuildingContents[l_I].PlotTypeOrBuilding == p_PlotTypeOrBuilding && m_GarrisonPlotBuildingContents[l_I].FactionIndex == p_FactionIndex)
            l_Data.push_back(m_GarrisonPlotBuildingContents[l_I]);
    }

    return l_Data;
}

void ObjectMgr::LoadNpcRecipesConditions()
{
    uint32 l_StartTime = getMSTime();

    QueryResult l_Result = WorldDatabase.Query("SELECT NpcEntry, RecipeID, PlayerConditionID FROM npc_recipe_condition");

    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 npc recipe condition. DB table `npc_recipe_condition` is empty.");
        return;
    }

    uint32 l_Count = 0;

    do
    {
        Field* l_Fields = l_Result->Fetch();
        RecipesConditions l_Conditions;

        uint32 l_NpcID                 = l_Fields[0].GetUInt32();
        l_Conditions.RecipeID          = l_Fields[1].GetUInt32();
        l_Conditions.PlayerConditionID = l_Fields[2].GetUInt32();

        _NpcRecipesConditions[l_NpcID].push_back(l_Conditions);

        ++l_Count;
    } while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u npc recipe condition in %u ms", l_Count, GetMSTimeDiffToNow(l_StartTime));
}

void ObjectMgr::LoadExplorationBaseXP()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT level, basexp FROM exploration_basexp");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 BaseXP definitions. DB table `exploration_basexp` is empty.");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        uint8 level  = fields[0].GetUInt8();
        uint32 basexp = fields[1].GetInt32();
        _baseXPTable[level] = basexp;
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u BaseXP definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

uint32 ObjectMgr::GetBaseXP(uint8 level)
{
    return _baseXPTable[level] ? _baseXPTable[level] : 0;
}

uint32 ObjectMgr::GetXPForLevel(uint8 level) const
{
    if (level < _playerXPperLevel.size())
        return _playerXPperLevel[level];
    return 0;
}

void ObjectMgr::LoadPetNames()
{
    uint32 oldMSTime = getMSTime();
    //                                                0     1      2
    QueryResult result = WorldDatabase.Query("SELECT word, entry, half FROM pet_name_generation");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 pet name parts. DB table `pet_name_generation` is empty!");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        std::string word = fields[0].GetString();
        uint32 entry     = fields[1].GetUInt32();
        bool   half      = fields[2].GetBool();
        if (half)
            _petHalfName1[entry].push_back(word);
        else
            _petHalfName0[entry].push_back(word);
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u pet name parts in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadPetNumber()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = CharacterDatabase.Query("SELECT MAX(id) FROM character_pet");
    if (result)
    {
        Field* fields = result->Fetch();
        _hiPetNumber = fields[0].GetUInt32()+1;
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded the max pet number: %d in %u ms", _hiPetNumber-1, GetMSTimeDiffToNow(oldMSTime));
}

std::string ObjectMgr::GeneratePetName(uint32 entry)
{
    StringVector & list0 = _petHalfName0[entry];
    StringVector & list1 = _petHalfName1[entry];

    if (list0.empty() || list1.empty())
    {
        CreatureTemplate const* cinfo = GetCreatureTemplate(entry);
        const char* petname = GetPetName(cinfo->family, sWorld->GetDefaultDbcLocale());
        if (!petname)
            return cinfo->Name;

        return std::string(petname);
    }

    return *(list0.begin()+urand(0, list0.size()-1)) + *(list1.begin()+urand(0, list1.size()-1));
}

uint32 ObjectMgr::GeneratePetNumber()
{
    return ++_hiPetNumber;
}

uint64 ObjectMgr::GenerateVoidStorageItemId()
{
    return ++_voidItemId;
}

void ObjectMgr::LoadCorpses()
{
    //        0     1     2     3            4      5          6          7       8       9      10        11    12          13          14          15         16
    // SELECT posX, posY, posZ, orientation, mapId, displayId, itemCache, bytes1, bytes2, flags, dynFlags, time, corpseType, instanceId, phaseMask, corpseGuid, guid FROM corpse WHERE corpseType <> 0

    uint32 oldMSTime = getMSTime();

    PreparedQueryResult result = CharacterDatabase.Query(CharacterDatabase.GetPreparedStatement(CHAR_SEL_CORPSES));
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 corpses. DB table `corpse` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        uint32 guid = fields[16].GetUInt32();
        CorpseType type = CorpseType(fields[12].GetUInt8());
        if (type >= MAX_CORPSE_TYPE)
        {
            sLog->outError(LOG_FILTER_GENERAL, "Corpse (guid: %u) have wrong corpse type (%u), not loading.", guid, type);
            continue;
        }

        Corpse* corpse = new Corpse(type);
        if (!corpse->LoadCorpseFromDB(guid, fields))
        {
            delete corpse;
            continue;
        }

        sObjectAccessor->AddCorpse(corpse);
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u corpses in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadReputationRewardRate()
{
    uint32 oldMSTime = getMSTime();

    _repRewardRateStore.clear();                             // for reload case

    uint32 count = 0; //                                0          1            2             3
    QueryResult result = WorldDatabase.Query("SELECT faction, quest_rate, creature_rate, spell_rate FROM reputation_reward_rate");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SQL, ">> Loaded `reputation_reward_rate`, table is empty!");

        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 factionId        = fields[0].GetUInt32();

        RepRewardRate repRate;

        repRate.quest_rate      = fields[1].GetFloat();
        repRate.creature_rate   = fields[2].GetFloat();
        repRate.spell_rate      = fields[3].GetFloat();

        FactionEntry const* factionEntry = sFactionStore.LookupEntry(factionId);
        if (!factionEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Faction (faction.dbc) %u does not exist but is used in `reputation_reward_rate`", factionId);
            continue;
        }

        if (repRate.quest_rate < 0.0f)
        {
            sLog->outError(LOG_FILTER_SQL, "Table reputation_reward_rate has quest_rate with invalid rate %f, skipping data for faction %u", repRate.quest_rate, factionId);
            continue;
        }

        if (repRate.creature_rate < 0.0f)
        {
            sLog->outError(LOG_FILTER_SQL, "Table reputation_reward_rate has creature_rate with invalid rate %f, skipping data for faction %u", repRate.creature_rate, factionId);
            continue;
        }

        if (repRate.spell_rate < 0.0f)
        {
            sLog->outError(LOG_FILTER_SQL, "Table reputation_reward_rate has spell_rate with invalid rate %f, skipping data for faction %u", repRate.spell_rate, factionId);
            continue;
        }

        _repRewardRateStore[factionId] = repRate;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u reputation_reward_rate in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadCurrencyOnKill()
{
    uint32 oldMSTime = getMSTime();

    _curOnKillStore.clear();

    uint32 count = 0;

    QueryResult result = WorldDatabase.Query("SELECT `creature_id`, `CurrencyId1`,  `CurrencyId2`,  `CurrencyId3`, `CurrencyCount1`, `CurrencyCount2`, `CurrencyCount3` FROM `creature_loot_currency`");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature currency definitions. DB table `creature_currency` is empty.");
        return;
    }

    do
    {
        Field*l_Fields = result->Fetch();

        uint32 l_Creature_id = l_Fields[0].GetUInt32();

        CurrencyOnKillEntry l_CurrOnKill;
        l_CurrOnKill[l_Fields[1].GetUInt16()] = l_Fields[4].GetInt32();
        l_CurrOnKill[l_Fields[2].GetUInt16()] = l_Fields[5].GetInt32();
        l_CurrOnKill[l_Fields[3].GetUInt16()] = l_Fields[6].GetInt32();

        if (!GetCreatureTemplate(l_Creature_id))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature_creature` have data for not existed creature entry (%u), skipped", l_Creature_id);
            continue;
        }

        for (CurrencyOnKillEntry::const_iterator i = l_CurrOnKill.begin(); i != l_CurrOnKill.end(); ++i)
        {
            if (!i->first)
                continue;

            if (!sCurrencyTypesStore.LookupEntry(i->first))
            {
                sLog->outError(LOG_FILTER_SQL, "CurrencyType (CurrencyTypes.dbc) %u does not exist but is used in `creature_currency`", i->first);
                continue;
            }
        }

        _curOnKillStore[l_Creature_id] = l_CurrOnKill;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature currency definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadPersonnalCurrencyOnKill()
{
    uint32 l_OldMSTime = getMSTime();

    m_PersonnalCurrOnKillStore.clear();

    uint32 l_Count = 0;

    QueryResult l_Result = WorldDatabase.Query("SELECT `creature_id`, `CurrencyId1`,  `CurrencyId2`,  `CurrencyId3`, `CurrencyCount1`, `CurrencyCount2`, `CurrencyCount3` FROM `creature_loot_currency_personnal`");
    if (!l_Result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 personnal creature currency definitions. DB table `creature_loot_currency_personnal` is empty.");
        return;
    }

    do
    {
        Field* l_Fields = l_Result->Fetch();

        uint32 l_Entry = l_Fields[0].GetUInt32();

        CurrencyOnKillEntry l_CurrOnKill;
        l_CurrOnKill[l_Fields[1].GetUInt16()] = l_Fields[4].GetInt32();
        l_CurrOnKill[l_Fields[2].GetUInt16()] = l_Fields[5].GetInt32();
        l_CurrOnKill[l_Fields[3].GetUInt16()] = l_Fields[6].GetInt32();

        if (!GetCreatureTemplate(l_Entry))
            continue;

        for (CurrencyOnKillEntry::const_iterator i = l_CurrOnKill.begin(); i != l_CurrOnKill.end(); ++i)
        {
            if (!sCurrencyTypesStore.LookupEntry(i->first))
                continue;
        }

        m_PersonnalCurrOnKillStore[l_Entry] = l_CurrOnKill;
        ++l_Count;
    }
    while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u personnal creature currency definitions in %u ms", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

void ObjectMgr::LoadReputationOnKill()
{
    uint32 oldMSTime = getMSTime();

    // For reload case
    _repOnKillStore.clear();

    uint32 count = 0;

    //                                                0            1                     2
    QueryResult result = WorldDatabase.Query("SELECT creature_id, RewOnKillRepFaction1, RewOnKillRepFaction2, "
    //   3             4             5                   6             7             8                   9
        "IsTeamAward1, MaxStanding1, RewOnKillRepValue1, IsTeamAward2, MaxStanding2, RewOnKillRepValue2, TeamDependent "
        "FROM creature_onkill_reputation");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature award reputation definitions. DB table `creature_onkill_reputation` is empty.");

        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 creature_id = fields[0].GetUInt32();

        ReputationOnKillEntry repOnKill;
        repOnKill.RepFaction1          = fields[1].GetInt16();
        repOnKill.RepFaction2          = fields[2].GetInt16();
        repOnKill.IsTeamAward1        = fields[3].GetBool();
        repOnKill.ReputationMaxCap1  = fields[4].GetUInt8();
        repOnKill.RepValue1            = fields[5].GetInt32();
        repOnKill.IsTeamAward2        = fields[6].GetBool();
        repOnKill.ReputationMaxCap2  = fields[7].GetUInt8();
        repOnKill.RepValue2            = fields[8].GetInt32();
        repOnKill.TeamDependent       = fields[9].GetUInt8();

        if (!GetCreatureTemplate(creature_id))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `creature_onkill_reputation` have data for not existed creature entry (%u), skipped", creature_id);
            continue;
        }

        if (repOnKill.RepFaction1)
        {
            FactionEntry const* factionEntry1 = sFactionStore.LookupEntry(repOnKill.RepFaction1);
            if (!factionEntry1)
            {
                sLog->outError(LOG_FILTER_SQL, "Faction (faction.dbc) %u does not exist but is used in `creature_onkill_reputation`", repOnKill.RepFaction1);
                continue;
            }
        }

        if (repOnKill.RepFaction2)
        {
            FactionEntry const* factionEntry2 = sFactionStore.LookupEntry(repOnKill.RepFaction2);
            if (!factionEntry2)
            {
                sLog->outError(LOG_FILTER_SQL, "Faction (faction.dbc) %u does not exist but is used in `creature_onkill_reputation`", repOnKill.RepFaction2);
                continue;
            }
        }

        _repOnKillStore[creature_id] = repOnKill;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature award reputation definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadReputationSpilloverTemplate()
{
    uint32 oldMSTime = getMSTime();

    _repSpilloverTemplateStore.clear();                      // for reload case

    uint32 count = 0; //                                0         1        2       3        4       5       6         7        8      9        10       11     12        13       14     15
    QueryResult result = WorldDatabase.Query("SELECT faction, faction1, rate_1, rank_1, faction2, rate_2, rank_2, faction3, rate_3, rank_3, faction4, rate_4, rank_4, faction5, rate_5, rank_5 FROM reputation_spillover_template");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded `reputation_spillover_template`, table is empty.");
        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 factionId                = fields[0].GetUInt16();

        RepSpilloverTemplate repTemplate;

        repTemplate.faction[0]          = fields[1].GetUInt16();
        repTemplate.faction_rate[0]     = fields[2].GetFloat();
        repTemplate.faction_rank[0]     = fields[3].GetUInt8();
        repTemplate.faction[1]          = fields[4].GetUInt16();
        repTemplate.faction_rate[1]     = fields[5].GetFloat();
        repTemplate.faction_rank[1]     = fields[6].GetUInt8();
        repTemplate.faction[2]          = fields[7].GetUInt16();
        repTemplate.faction_rate[2]     = fields[8].GetFloat();
        repTemplate.faction_rank[2]     = fields[9].GetUInt8();
        repTemplate.faction[3]          = fields[10].GetUInt16();
        repTemplate.faction_rate[3]     = fields[11].GetFloat();
        repTemplate.faction_rank[3]     = fields[12].GetUInt8();
        repTemplate.faction[4]          = fields[13].GetUInt16();
        repTemplate.faction_rate[4]     = fields[14].GetFloat();
        repTemplate.faction_rank[4]     = fields[15].GetUInt8();

        FactionEntry const* factionEntry = sFactionStore.LookupEntry(factionId);

        if (!factionEntry)
        {
            sLog->outError(LOG_FILTER_SQL, "Faction (faction.dbc) %u does not exist but is used in `reputation_spillover_template`", factionId);
            continue;
        }

        if (factionEntry->ParentFactionID == 0)
        {
            sLog->outError(LOG_FILTER_SQL, "Faction (faction.dbc) %u in `reputation_spillover_template` does not belong to any team, skipping", factionId);
            continue;
        }

        for (uint32 i = 0; i < MAX_SPILLOVER_FACTIONS; ++i)
        {
            if (repTemplate.faction[i])
            {
                FactionEntry const* factionSpillover = sFactionStore.LookupEntry(repTemplate.faction[i]);

                if (!factionSpillover)
                {
                    sLog->outError(LOG_FILTER_SQL, "Spillover faction (faction.dbc) %u does not exist but is used in `reputation_spillover_template` for faction %u, skipping", repTemplate.faction[i], factionId);
                    continue;
                }

                if (factionSpillover->ReputationIndex < 0)
                {
                    sLog->outError(LOG_FILTER_SQL, "Spillover faction (faction.dbc) %u for faction %u in `reputation_spillover_template` can not be listed for client, and then useless, skipping", repTemplate.faction[i], factionId);
                    continue;
                }

                if (repTemplate.faction_rank[i] >= MAX_REPUTATION_RANK)
                {
                    sLog->outError(LOG_FILTER_SQL, "Rank %u used in `reputation_spillover_template` for spillover faction %u is not valid, skipping", repTemplate.faction_rank[i], repTemplate.faction[i]);
                    continue;
                }
            }
        }

        FactionEntry const* factionEntry0 = sFactionStore.LookupEntry(repTemplate.faction[0]);
        if (repTemplate.faction[0] && !factionEntry0)
        {
            sLog->outError(LOG_FILTER_SQL, "Faction (faction.dbc) %u does not exist but is used in `reputation_spillover_template`", repTemplate.faction[0]);
            continue;
        }
        FactionEntry const* factionEntry1 = sFactionStore.LookupEntry(repTemplate.faction[1]);
        if (repTemplate.faction[1] && !factionEntry1)
        {
            sLog->outError(LOG_FILTER_SQL, "Faction (faction.dbc) %u does not exist but is used in `reputation_spillover_template`", repTemplate.faction[1]);
            continue;
        }
        FactionEntry const* factionEntry2 = sFactionStore.LookupEntry(repTemplate.faction[2]);
        if (repTemplate.faction[2] && !factionEntry2)
        {
            sLog->outError(LOG_FILTER_SQL, "Faction (faction.dbc) %u does not exist but is used in `reputation_spillover_template`", repTemplate.faction[2]);
            continue;
        }
        FactionEntry const* factionEntry3 = sFactionStore.LookupEntry(repTemplate.faction[3]);
        if (repTemplate.faction[3] && !factionEntry3)
        {
            sLog->outError(LOG_FILTER_SQL, "Faction (faction.dbc) %u does not exist but is used in `reputation_spillover_template`", repTemplate.faction[3]);
            continue;
        }
        FactionEntry const* factionEntry4 = sFactionStore.LookupEntry(repTemplate.faction[4]);
        if (repTemplate.faction[4] && !factionEntry4)
        {
            sLog->outError(LOG_FILTER_SQL, "Faction (faction.dbc) %u does not exist but is used in `reputation_spillover_template`", repTemplate.faction[4]);
            continue;
        }

        _repSpilloverTemplateStore[factionId] = repTemplate;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u reputation_spillover_template in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadPointsOfInterest()
{
    uint32 oldMSTime = getMSTime();

    _pointsOfInterestStore.clear();                              // need for reload case

    uint32 count = 0;

    //                                                  0   1  2   3      4     5       6
    QueryResult result = WorldDatabase.Query("SELECT entry, x, y, icon, flags, data, icon_name FROM points_of_interest");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 Points of Interest definitions. DB table `points_of_interest` is empty.");

        return;
    }

    do
    {
        Field* fields = result->Fetch();

        uint32 point_id = fields[0].GetUInt32();

        PointOfInterest POI;
        POI.x = fields[1].GetFloat();
        POI.y = fields[2].GetFloat();
        POI.icon = fields[3].GetUInt32();
        POI.flags = fields[4].GetUInt32();
        POI.data = fields[5].GetUInt32();
        POI.icon_name = fields[6].GetString();

        if (!JadeCore::IsValidMapCoord(POI.x, POI.y))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `points_of_interest` (Entry: %u) have invalid coordinates (X: %f Y: %f), ignored.", point_id, POI.x, POI.y);
            continue;
        }

        _pointsOfInterestStore[point_id] = POI;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Points of Interest definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadQuestPOI()
{
    uint32 oldMSTime = getMSTime();

    _questPOIStore.clear();                              // need for reload case

    uint32 count = 0;

    //                                               0        1          2               3      4               5      6         7      8              9                  10
    QueryResult result = WorldDatabase.Query("SELECT QuestID, BlobIndex, ObjectiveIndex, MapID, WorldMapAreaId, Floor, Priority, Flags, WorldEffectID, PlayerConditionID, Unk FROM quest_poi order by QuestID");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 quest POI definitions. DB table `quest_poi` is empty.");

        return;
    }

    //                                                0       1   2  3
    QueryResult points = WorldDatabase.Query("SELECT questId, id, x, y FROM quest_poi_points ORDER BY questId DESC, idx");

    std::vector<std::vector<std::vector<QuestPOIPoint> > > POIs;

    if (points)
    {
        // The first result should have the highest questId
        Field* fields = points->Fetch();
        uint32 questIdMax = fields[0].GetUInt32();
        POIs.resize(questIdMax + 1);

        do
        {
            fields = points->Fetch();

            uint32 questId            = fields[0].GetUInt32();
            uint32 id                 = fields[1].GetUInt32();
            int32  x                  = fields[2].GetInt32();
            int32  y                  = fields[3].GetInt32();

            if (POIs[questId].size() <= id + 1)
                POIs[questId].resize(id + 10);

            QuestPOIPoint point(x, y);
            POIs[questId][id].push_back(point);
        }
        while (points->NextRow());
    }

    do
    {
        Field* l_Fields = result->Fetch();

        uint32 l_QuestId            = l_Fields[0].GetUInt32();
        uint32 l_BlobIndex          = l_Fields[1].GetUInt32();
        int32 l_ObjectiveIndex      = l_Fields[2].GetInt32();
        uint32 l_MapID              = l_Fields[3].GetUInt32();
        uint32 l_WorldMapAreaId     = l_Fields[4].GetUInt32();
        uint32 l_Floor              = l_Fields[5].GetUInt32();
        uint32 l_Priority           = l_Fields[6].GetUInt32();
        uint32 l_Flags              = l_Fields[7].GetUInt32();
        uint32 l_WorldEffectID      = l_Fields[8].GetUInt32();
        uint32 l_PlayerConditionID  = l_Fields[9].GetUInt32();
        uint32 l_Unk                = l_Fields[10].GetUInt32();
        
        
        if (l_QuestId < int32(POIs.size()) && l_BlobIndex < int32(POIs[l_QuestId].size())) ///< Comparison of integers of different signs: 'uint32' (aka 'unsigned int') and 'int32' (aka 'int')
        {
            QuestPOI l_POI(l_BlobIndex, l_ObjectiveIndex, l_MapID, l_WorldMapAreaId, l_Floor, l_Priority, l_Flags, l_WorldEffectID, l_PlayerConditionID, l_Unk);
            l_POI.Points = POIs[l_QuestId][l_BlobIndex];

            _questPOIStore[l_QuestId].push_back(l_POI);
        }

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u quest POI definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadNPCSpellClickSpells()
{
    uint32 oldMSTime = getMSTime();

    _spellClickInfoStore.clear();
    //                                                0          1         2            3
    QueryResult result = WorldDatabase.Query("SELECT npc_entry, spell_id, cast_flags, user_type FROM npc_spellclick_spells");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spellclick spells. DB table `npc_spellclick_spells` is empty.");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 npc_entry = fields[0].GetUInt32();
        CreatureTemplate const* cInfo = GetCreatureTemplate(npc_entry);
        if (!cInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Table npc_spellclick_spells references unknown creature_template %u. Skipping entry.", npc_entry);
            continue;
        }

        uint32 spellid = fields[1].GetUInt32();
        SpellInfo const* spellinfo = sSpellMgr->GetSpellInfo(spellid);
        if (!spellinfo)
        {
            sLog->outError(LOG_FILTER_SQL, "Table npc_spellclick_spells references unknown spellid %u. Skipping entry.", spellid);
            continue;
        }

        uint8 userType = fields[3].GetUInt16();
        if (userType >= SPELL_CLICK_USER_MAX)
            sLog->outError(LOG_FILTER_SQL, "Table npc_spellclick_spells references unknown user type %u. Skipping entry.", uint32(userType));

        uint8 castFlags = fields[2].GetUInt8();
        SpellClickInfo info;
        info.spellId = spellid;
        info.castFlags = castFlags;
        info.userType = SpellClickUserTypes(userType);
        _spellClickInfoStore.insert(SpellClickInfoContainer::value_type(npc_entry, info));

        ++count;
    }
    while (result->NextRow());

    // all spellclick data loaded, now we check if there are creatures with NPC_FLAG_SPELLCLICK but with no data
    // NOTE: It *CAN* be the other way around: no spellclick flag but with spellclick data, in case of creature-only vehicle accessories
    CreatureTemplate** l_CreatureTemplates = sObjectMgr->GetCreatureTemplates();
    uint32 l_LastEntry = sObjectMgr->GetCreatureTemplateStoreSize();

    for (uint32 l_Entry = 0; l_Entry < l_LastEntry; l_Entry++)
    {
        CreatureTemplate const* l_CreatureTemplate = l_CreatureTemplates[l_Entry];
        if (l_CreatureTemplate == nullptr)
            continue;

        if ((l_CreatureTemplate->NpcFlags1 & UNIT_NPC_FLAG_SPELLCLICK) && _spellClickInfoStore.find(l_CreatureTemplate->Entry) == _spellClickInfoStore.end())
        {
            sLog->outError(LOG_FILTER_SQL, "npc_spellclick_spells: Creature template %u has UNIT_NPC_FLAG_SPELLCLICK but no data in spellclick table! Removing flag", l_CreatureTemplate->Entry);
            const_cast<CreatureTemplate*>(l_CreatureTemplate)->NpcFlags1 &= ~UNIT_NPC_FLAG_SPELLCLICK;
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spellclick definitions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::DeleteCreatureData(uint32 guid)
{
    // remove mapid*cellid -> guid_set map
    CreatureData const* data = GetCreatureData(guid);
    if (data)
        RemoveCreatureFromGrid(guid, data);

    _creatureDataStore.erase(guid);
}

void ObjectMgr::DeleteGOData(uint32 guid)
{
    // remove mapid*cellid -> guid_set map
    GameObjectData const* data = GetGOData(guid);
    if (data)
        RemoveGameobjectFromGrid(guid, data);

    _gameObjectDataStore.erase(guid);
}

void ObjectMgr::AddCorpseCellData(uint32 mapid, uint32 cellid, uint32 player_guid, uint32 instance)
{
    // corpses are always added to spawn mode 0 and they are spawned by their instance id
    CellObjectGuids& cell_guids = _mapObjectGuidsStore[MAKE_PAIR32(mapid, 0)][cellid];
    cell_guids.corpses[player_guid] = instance;
}

void ObjectMgr::DeleteCorpseCellData(uint32 mapid, uint32 cellid, uint32 player_guid)
{
    // corpses are always added to spawn mode 0 and they are spawned by their instance id
    CellObjectGuids& cell_guids = _mapObjectGuidsStore[MAKE_PAIR32(mapid, 0)][cellid];
    cell_guids.corpses.erase(player_guid);
}

void ObjectMgr::LoadQuestRelationsHelper(QuestRelations& map, std::string table, bool starter, bool go)
{
    uint32 oldMSTime = getMSTime();

    map.clear();                                            // need for reload case

    uint32 count = 0;

    QueryResult result = WorldDatabase.PQuery("SELECT id, quest, pool_entry FROM %s qr LEFT JOIN pool_quest pq ON qr.quest = pq.entry", table.c_str());

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 quest relations from `%s`, table is empty.", table.c_str());

        return;
    }

    PooledQuestRelation* poolRelationMap = go ? &sPoolMgr->mQuestGORelation : &sPoolMgr->mQuestCreatureRelation;
    if (starter)
        poolRelationMap->clear();

    do
    {
        uint32 id     = result->Fetch()[0].GetUInt32();
        uint32 quest  = result->Fetch()[1].GetUInt32();
        uint32 poolId = result->Fetch()[2].GetUInt32();

        if (_questTemplates.find(quest) == _questTemplates.end())
        {
            sLog->outError(LOG_FILTER_SQL, "Table `%s`: Quest %u listed for entry %u does not exist.", table.c_str(), quest, id);
            continue;
        }

        if (!poolId || !starter)
            map.insert(QuestRelations::value_type(id, quest));
        else if (starter)
            poolRelationMap->insert(PooledQuestRelation::value_type(quest, id));

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u quest relations from %s in %u ms", count, table.c_str(), GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadGameobjectQuestStarters()
{
    LoadQuestRelationsHelper(_goQuestRelations, "gameobject_queststarter", true, true);

    for (QuestRelations::iterator itr = _goQuestRelations.begin(); itr != _goQuestRelations.end(); ++itr)
    {
        GameObjectTemplate const* goInfo = GetGameObjectTemplate(itr->first);
        if (!goInfo)
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject_queststarter` have data for not existed gameobject entry (%u) and existed quest %u", itr->first, itr->second);
        else if (goInfo->type != GAMEOBJECT_TYPE_QUESTGIVER)
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject_queststarter` have data gameobject entry (%u) for quest %u, but GO is not GAMEOBJECT_TYPE_QUESTGIVER", itr->first, itr->second);
    }
}

void ObjectMgr::LoadGameobjectQuestEnders()
{
    LoadQuestRelationsHelper(_goQuestInvolvedRelations, "gameobject_questender", false, true);

    for (QuestRelations::iterator itr = _goQuestInvolvedRelations.begin(); itr != _goQuestInvolvedRelations.end(); ++itr)
    {
        GameObjectTemplate const* goInfo = GetGameObjectTemplate(itr->first);
        if (!goInfo)
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject_questender` have data for not existed gameobject entry (%u) and existed quest %u", itr->first, itr->second);
        else if (goInfo->type != GAMEOBJECT_TYPE_QUESTGIVER)
            sLog->outError(LOG_FILTER_SQL, "Table `gameobject_questender` have data gameobject entry (%u) for quest %u, but GO is not GAMEOBJECT_TYPE_QUESTGIVER", itr->first, itr->second);
    }
}

void ObjectMgr::LoadCreatureQuestStarters()
{
    LoadQuestRelationsHelper(_creatureQuestRelations, "creature_queststarter", true, false);

    for (QuestRelations::const_iterator l_Iterator = _creatureQuestRelations.begin(); l_Iterator != _creatureQuestRelations.end(); ++l_Iterator)
    {
        CreatureTemplate const* cInfo = GetCreatureTemplate(l_Iterator->first);
        if (!cInfo)
            sLog->outError(LOG_FILTER_SQL, "Table `creature_queststarter` have data for not existed creature entry (%u) and existed quest %u", l_Iterator->first, l_Iterator->second);
        else if (!(cInfo->NpcFlags1 & UNIT_NPC_FLAG_QUESTGIVER))
            sLog->outError(LOG_FILTER_SQL, "Table `creature_queststarter` has creature entry (%u) for quest %u, but npcflag does not include UNIT_NPC_FLAG_QUESTGIVER", l_Iterator->first, l_Iterator->second);

        Quest* l_Quest = const_cast<Quest*>(GetQuestTemplate(l_Iterator->second));
        if (l_Quest != nullptr)
            l_Quest->completionsNpcs.push_back(l_Iterator->first);
    }
}

void ObjectMgr::LoadCreatureQuestEnders()
{
    LoadQuestRelationsHelper(_creatureQuestInvolvedRelations, "creature_questender", false, false);

    for (QuestRelations::iterator itr = _creatureQuestInvolvedRelations.begin(); itr != _creatureQuestInvolvedRelations.end(); ++itr)
    {
        CreatureTemplate const* cInfo = GetCreatureTemplate(itr->first);
        if (!cInfo)
            sLog->outError(LOG_FILTER_SQL, "Table `creature_questender` have data for not existed creature entry (%u) and existed quest %u", itr->first, itr->second);
        else if (!(cInfo->NpcFlags1 & UNIT_NPC_FLAG_QUESTGIVER))
            sLog->outError(LOG_FILTER_SQL, "Table `creature_questender` has creature entry (%u) for quest %u, but npcflag does not include UNIT_NPC_FLAG_QUESTGIVER", itr->first, itr->second);
    }
}

void ObjectMgr::LoadReservedPlayersNames()
{
    uint32 oldMSTime = getMSTime();

    _reservedNamesStore.clear();                                // need for reload case

    QueryResult result = CharacterDatabase.Query("SELECT name FROM reserved_name");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 reserved player names. DB table `reserved_name` is empty!");
        return;
    }

    uint32 count = 0;

    Field* fields;
    do
    {
        fields = result->Fetch();
        std::string name= fields[0].GetString();

        std::wstring wstr;
        if (!Utf8toWStr (name, wstr))
        {
            sLog->outError(LOG_FILTER_GENERAL, "Table `reserved_name` have invalid name: %s", name.c_str());
            continue;
        }

        wstrToLower(wstr);

        _reservedNamesStore.insert(wstr);
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u reserved player names in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

bool ObjectMgr::IsReservedName(const std::string& name) const
{
    std::wstring wstr;
    if (!Utf8toWStr (name, wstr))
        return false;

    wstrToLower(wstr);

    return _reservedNamesStore.find(wstr) != _reservedNamesStore.end();
}

enum LanguageType
{
    LT_BASIC_LATIN    = 0x0000,
    LT_EXTENDEN_LATIN = 0x0001,
    LT_CYRILLIC       = 0x0002,
    LT_EAST_ASIA      = 0x0004,
    LT_ANY            = 0xFFFF
};

static LanguageType GetRealmLanguageType(bool create)
{
    switch (sWorld->getIntConfig(CONFIG_REALM_ZONE))
    {
        case REALM_ZONE_UNKNOWN:                            // any language
        case REALM_ZONE_DEVELOPMENT:
        case REALM_ZONE_TEST_SERVER:
        case REALM_ZONE_QA_SERVER:
            return LT_ANY;
        case REALM_ZONE_UNITED_STATES:                      // extended-Latin
        case REALM_ZONE_OCEANIC:
        case REALM_ZONE_LATIN_AMERICA:
        case REALM_ZONE_ENGLISH:
        case REALM_ZONE_GERMAN:
        case REALM_ZONE_FRENCH:
        case REALM_ZONE_SPANISH:
            return LT_EXTENDEN_LATIN;
        case REALM_ZONE_KOREA:                              // East-Asian
        case REALM_ZONE_TAIWAN:
        case REALM_ZONE_CHINA:
            return LT_EAST_ASIA;
        case REALM_ZONE_RUSSIAN:                            // Cyrillic
            return LT_CYRILLIC;
        default:
            return create ? LT_BASIC_LATIN : LT_ANY;        // basic-Latin at create, any at login
    }
}

bool isValidString(std::wstring& wstr, uint32 strictMask, bool numericOrSpace, bool create = false)
{
    if (strictMask == 0)                                       // any language, ignore realm
    {
        if (isExtendedLatinString(wstr, numericOrSpace))
            return true;
        if (isCyrillicString(wstr, numericOrSpace))
            return true;
        if (isEastAsianString(wstr, numericOrSpace))
            return true;
        return false;
    }

    if (strictMask & 0x2)                                    // realm zone specific
    {
        LanguageType lt = GetRealmLanguageType(create);
        if (lt & LT_EXTENDEN_LATIN)
            if (isExtendedLatinString(wstr, numericOrSpace))
                return true;
        if (lt & LT_CYRILLIC)
            if (isCyrillicString(wstr, numericOrSpace))
                return true;
        if (lt & LT_EAST_ASIA)
            if (isEastAsianString(wstr, numericOrSpace))
                return true;
    }

    if (strictMask & 0x1)                                    // basic Latin
    {
        if (isBasicLatinString(wstr, numericOrSpace))
            return true;
    }

    return false;
}

uint8 ObjectMgr::CheckPlayerName(const std::string& name, bool create)
{
    std::wstring wname;
    if (!Utf8toWStr(name, wname))
        return CHAR_NAME_INVALID_CHARACTER;

    if (wname.size() > MAX_PLAYER_NAME)
        return CHAR_NAME_TOO_LONG;

    uint32 minName = sWorld->getIntConfig(CONFIG_MIN_PLAYER_NAME);
    if (wname.size() < minName)
        return CHAR_NAME_TOO_SHORT;

    uint32 strictMask = sWorld->getIntConfig(CONFIG_STRICT_PLAYER_NAMES);
    if (!isValidString(wname, strictMask, false, create))
        return CHAR_NAME_MIXED_LANGUAGES;

    wstrToLower(wname);
    for (size_t i = 2; i < wname.size(); ++i)
        if (wname[i] == wname[i-1] && wname[i] == wname[i-2])
            return CHAR_NAME_THREE_CONSECUTIVE;

    return CHAR_NAME_SUCCESS;
}

bool ObjectMgr::IsValidCharterName(const std::string& name)
{
    std::wstring wname;
    if (!Utf8toWStr(name, wname))
        return false;

    if (wname.size() > MAX_CHARTER_NAME)
        return false;

    uint32 minName = sWorld->getIntConfig(CONFIG_MIN_CHARTER_NAME);
    if (wname.size() < minName)
        return false;

    uint32 strictMask = sWorld->getIntConfig(CONFIG_STRICT_CHARTER_NAMES);

    return isValidString(wname, strictMask, true);
}

PetNameInvalidReason ObjectMgr::CheckPetName(const std::string& name)
{
    std::wstring wname;
    if (!Utf8toWStr(name, wname))
        return PET_NAME_INVALID;

    if (wname.size() > MAX_PET_NAME)
        return PET_NAME_TOO_LONG;

    uint32 minName = sWorld->getIntConfig(CONFIG_MIN_PET_NAME);
    if (wname.size() < minName)
        return PET_NAME_TOO_SHORT;

    uint32 strictMask = sWorld->getIntConfig(CONFIG_STRICT_PET_NAMES);
    if (!isValidString(wname, strictMask, false))
        return PET_NAME_MIXED_LANGUAGES;

    return PET_NAME_SUCCESS;
}

void ObjectMgr::LoadGameObjectForQuests()
{
    uint32 oldMSTime = getMSTime();

    _gameObjectForQuestStore.clear();                         // need for reload case

    if (sObjectMgr->GetGameObjectTemplates()->empty())
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 GameObjects for quests");
        return;
    }

    uint32 count = 0;

    // collect GO entries for GO that must activated
    GameObjectTemplateContainer const* gotc = sObjectMgr->GetGameObjectTemplates();
    for (GameObjectTemplateContainer::const_iterator itr = gotc->begin(); itr != gotc->end(); ++itr)
    {
        switch (itr->second.type)
        {
            // scan GO chest with loot including quest items
            case GAMEOBJECT_TYPE_CHEST:
            {
                uint32 loot_id = (itr->second.GetLootId());

                // find quest loot for GO
                if (itr->second.chest.questID || LootTemplates_Gameobject.HaveQuestLootFor(loot_id))
                {
                    _gameObjectForQuestStore.insert(itr->second.entry);
                    ++count;
                }
                break;
            }
            case GAMEOBJECT_TYPE_GENERIC:
            {
                if (itr->second._generic.questID > 0)            //quests objects
                {
                    _gameObjectForQuestStore.insert(itr->second.entry);
                    count++;
                }
                break;
            }
            case GAMEOBJECT_TYPE_GOOBER:
            {
                if (itr->second.goober.questID > 0)              //quests objects
                {
                    _gameObjectForQuestStore.insert(itr->second.entry);
                    count++;
                }
                break;
            }
            default:
                break;
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u GameObjects for quests in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

bool ObjectMgr::LoadTrinityStrings(const char* table, int32 min_value, int32 max_value)
{
    uint32 oldMSTime = getMSTime();

    int32 start_value = min_value;
    int32 end_value   = max_value;
    // some string can have negative indexes range
    if (start_value < 0)
    {
        if (end_value >= start_value)
        {
            sLog->outError(LOG_FILTER_SQL, "Table '%s' attempt loaded with invalid range (%d - %d), strings not loaded.", table, min_value, max_value);
            return false;
        }

        // real range (max+1, min+1) exaple: (-10, -1000) -> -999...-10+1
        std::swap(start_value, end_value);
        ++start_value;
        ++end_value;
    }
    else
    {
        if (start_value >= end_value)
        {
            sLog->outError(LOG_FILTER_SQL, "Table '%s' attempt loaded with invalid range (%d - %d), strings not loaded.", table, min_value, max_value);
            return false;
        }
    }

    // cleanup affected map part for reloading case
    for (TrinityStringLocaleContainer::iterator itr = _trinityStringLocaleStore.begin(); itr != _trinityStringLocaleStore.end();)
    {
        if (itr->first >= start_value && itr->first < end_value)
            _trinityStringLocaleStore.erase(itr++);
        else
            ++itr;
    }

    QueryResult result = WorldDatabase.PQuery("SELECT entry, content_default, content_loc1, content_loc2, content_loc3, content_loc4, content_loc5, content_loc6, content_loc7, content_loc8, content_loc9, content_loc10 FROM %s", table);

    if (!result)
    {
        if (min_value == MIN_TRINITY_STRING_ID)              // error only in case internal strings
            sLog->outError(LOG_FILTER_SERVER_LOADING, ">>  Loaded 0 trinity strings. DB table `%s` is empty. Cannot continue.", table);
        else
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 string templates. DB table `%s` is empty.", table);

        return false;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        int32 entry = fields[0].GetInt32();

        if (entry == 0)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `%s` contain reserved entry 0, ignored.", table);
            continue;
        }
        else if (entry < start_value || entry >= end_value)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `%s` contain entry %i out of allowed range (%d - %d), ignored.", table, entry, min_value, max_value);
            continue;
        }

        TrinityStringLocale& data = _trinityStringLocaleStore[entry];

        if (!data.Content.empty())
        {
            sLog->outError(LOG_FILTER_SQL, "Table `%s` contain data for already loaded entry  %i (from another table?), ignored.", table, entry);
            continue;
        }

        data.Content.resize(1);
        ++count;

        for (uint8 i = 0; i < TOTAL_LOCALES; ++i)
            AddLocaleString(fields[i + 1].GetString(), LocaleConstant(i), data.Content);
    }
    while (result->NextRow());

    if (min_value == MIN_TRINITY_STRING_ID)
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Trinity strings from table %s in %u ms", count, table, GetMSTimeDiffToNow(oldMSTime));
    else
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u string templates from %s in %u ms", count, table, GetMSTimeDiffToNow(oldMSTime));

    return true;
}

const char* ObjectMgr::GetTrinityString(int32 entry, LocaleConstant locale_idx) const
{
    if (TrinityStringLocale const* msl = GetTrinityStringLocale(entry))
    {
        if (msl->Content.size() > size_t(locale_idx) && !msl->Content[locale_idx].empty())
            return msl->Content[locale_idx].c_str();

        return msl->Content[DEFAULT_LOCALE].c_str();
    }

    if (entry > 0)
        sLog->outError(LOG_FILTER_SQL, "Entry %i not found in `trinity_string` table.", entry);
    else
        sLog->outError(LOG_FILTER_SQL, "Trinity string entry %i not found in DB.", entry);
    return "<error>";
}

void ObjectMgr::LoadFishingBaseSkillLevel()
{
    uint32 oldMSTime = getMSTime();

    _fishingBaseForAreaStore.clear();                            // for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, skill FROM skill_fishing_base_level");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 areas for fishing base skill level. DB table `skill_fishing_base_level` is empty.");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        uint32 entry  = fields[0].GetUInt32();
        int32 skill   = fields[1].GetInt16();

        AreaTableEntry const* fArea = GetAreaEntryByAreaID(entry);
        if (!fArea)
        {
            sLog->outError(LOG_FILTER_SQL, "AreaId %u defined in `skill_fishing_base_level` does not exist", entry);
            continue;
        }

        _fishingBaseForAreaStore[entry] = skill;
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u areas for fishing base skill level in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

bool ObjectMgr::CheckDeclinedNames(std::wstring w_ownname, DeclinedName const& names)
{
    // get main part of the name
    std::wstring mainpart = GetMainPartOfName(w_ownname, 0);
    // prepare flags
    bool x = true;
    bool y = true;

    // check declined names
    for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
    {
        std::wstring wname;
        if (!Utf8toWStr(names.name[i], wname))
            return false;

        if (mainpart != GetMainPartOfName(wname, i+1))
            x = false;

        if (w_ownname != wname)
            y = false;
    }
    return (x || y);
}

uint32 ObjectMgr::GetAreaTriggerScriptId(uint32 trigger_id)
{
    AreaTriggerScriptContainer::const_iterator i = _areaTriggerScriptStore.find(trigger_id);
    if (i!= _areaTriggerScriptStore.end())
        return i->second;
    return 0;
}

SpellScriptsBounds ObjectMgr::GetSpellScriptsBounds(uint32 spell_id)
{
    return SpellScriptsBounds(_spellScriptsStore.lower_bound(spell_id), _spellScriptsStore.upper_bound(spell_id));
}

SkillRangeType GetSkillRangeType(SkillLineEntry const* pSkill, bool racial)
{
    switch (pSkill->categoryId)
    {
        case SKILL_CATEGORY_LANGUAGES:
            return SKILL_RANGE_LANGUAGE;
        case SKILL_CATEGORY_WEAPON:
            return SKILL_RANGE_LEVEL;
        case SKILL_CATEGORY_ARMOR:
        case SKILL_CATEGORY_CLASS:
            if (pSkill->id != SKILL_LOCKPICKING)
                return SKILL_RANGE_MONO;
            else
                return SKILL_RANGE_LEVEL;
        case SKILL_CATEGORY_SECONDARY:
        case SKILL_CATEGORY_PROFESSION:
            // not set skills for professions and racial abilities
            if (IsProfessionSkill(pSkill->id))
                return SKILL_RANGE_RANK;
            else if (racial)
                return SKILL_RANGE_NONE;
            else
                return SKILL_RANGE_MONO;
        default:
        case SKILL_CATEGORY_ATTRIBUTES:                     //not found in dbc
        case SKILL_CATEGORY_GENERIC:                        //only GENERIC(DND)
            return SKILL_RANGE_NONE;
    }
}

void ObjectMgr::LoadGameTele()
{
    uint32 oldMSTime = getMSTime();

    _gameTeleStore.clear();                                  // for reload case

    //                                                0       1           2           3           4        5     6
    QueryResult result = WorldDatabase.Query("SELECT id, position_x, position_y, position_z, orientation, map, name FROM game_tele");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">>  Loaded 0 GameTeleports. DB table `game_tele` is empty!");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 id         = fields[0].GetUInt32();

        GameTele gt;

        gt.position_x     = fields[1].GetFloat();
        gt.position_y     = fields[2].GetFloat();
        gt.position_z     = fields[3].GetFloat();
        gt.orientation    = fields[4].GetFloat();
        gt.mapId          = fields[5].GetUInt16();
        gt.name           = fields[6].GetString();

        if (!MapManager::IsValidMapCoord(gt.mapId, gt.position_x, gt.position_y, gt.position_z, gt.orientation))
        {
            sLog->outError(LOG_FILTER_SQL, "Wrong position for id %u (name: %s) in `game_tele` table, ignoring.", id, gt.name.c_str());
            continue;
        }

        if (!Utf8toWStr(gt.name, gt.wnameLow))
        {
            sLog->outError(LOG_FILTER_SQL, "Wrong UTF8 name for id %u in `game_tele` table, ignoring.", id);
            continue;
        }

        wstrToLower(gt.wnameLow);

        _gameTeleStore[id] = gt;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u GameTeleports in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

GameTele const* ObjectMgr::GetGameTele(const std::string& name) const
{
    // explicit name case
    std::wstring wname;
    if (!Utf8toWStr(name, wname))
        return NULL;

    // converting string that we try to find to lower case
    wstrToLower(wname);

    // Alternative first GameTele what contains wnameLow as substring in case no GameTele location found
    const GameTele* alt = NULL;
    for (GameTeleContainer::const_iterator itr = _gameTeleStore.begin(); itr != _gameTeleStore.end(); ++itr)
    {
        if (itr->second.wnameLow == wname)
            return &itr->second;
        else if (alt == NULL && itr->second.wnameLow.find(wname) != std::wstring::npos)
            alt = &itr->second;
    }

    return alt;
}

bool ObjectMgr::AddGameTele(GameTele& tele)
{
    // find max id
    uint32 new_id = 0;
    for (GameTeleContainer::const_iterator itr = _gameTeleStore.begin(); itr != _gameTeleStore.end(); ++itr)
        if (itr->first > new_id)
            new_id = itr->first;

    // use next
    ++new_id;

    if (!Utf8toWStr(tele.name, tele.wnameLow))
        return false;

    wstrToLower(tele.wnameLow);

    _gameTeleStore[new_id] = tele;

    PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_INS_GAME_TELE);

    stmt->setUInt32(0, new_id);
    stmt->setFloat(1, tele.position_x);
    stmt->setFloat(2, tele.position_y);
    stmt->setFloat(3, tele.position_z);
    stmt->setFloat(4, tele.orientation);
    stmt->setUInt16(5, uint16(tele.mapId));
    stmt->setString(6, tele.name);

    WorldDatabase.Execute(stmt);

    return true;
}

bool ObjectMgr::DeleteGameTele(const std::string& name)
{
    // explicit name case
    std::wstring wname;
    if (!Utf8toWStr(name, wname))
        return false;

    // converting string that we try to find to lower case
    wstrToLower(wname);

    for (GameTeleContainer::iterator itr = _gameTeleStore.begin(); itr != _gameTeleStore.end(); ++itr)
    {
        if (itr->second.wnameLow == wname)
        {
            PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_DEL_GAME_TELE);

            stmt->setString(0, itr->second.name);

            WorldDatabase.Execute(stmt);

            _gameTeleStore.erase(itr);
            return true;
        }
    }

    return false;
}

void ObjectMgr::LoadMailLevelRewards()
{
    uint32 oldMSTime = getMSTime();

    _mailLevelRewardStore.clear();                           // for reload case

    //                                                 0        1             2            3
    QueryResult result = WorldDatabase.Query("SELECT level, raceMask, mailTemplateId, senderEntry FROM mail_level_reward");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">>  Loaded 0 level dependent mail rewards. DB table `mail_level_reward` is empty.");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint8 level           = fields[0].GetUInt8();
        uint32 raceMask       = fields[1].GetUInt32();
        uint32 mailTemplateId = fields[2].GetUInt32();
        uint32 senderEntry    = fields[3].GetUInt32();

        if (level > MAX_LEVEL)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `mail_level_reward` have data for level %u that more supported by client (%u), ignoring.", level, MAX_LEVEL);
            continue;
        }

        if (!(raceMask & RACEMASK_ALL_PLAYABLE))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `mail_level_reward` have raceMask (%u) for level %u that not include any player races, ignoring.", raceMask, level);
            continue;
        }

        if (!sMailTemplateStore.LookupEntry(mailTemplateId))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `mail_level_reward` have invalid mailTemplateId (%u) for level %u that invalid not include any player races, ignoring.", mailTemplateId, level);
            continue;
        }

        if (!GetCreatureTemplate(senderEntry))
        {
            sLog->outError(LOG_FILTER_SQL, "Table `mail_level_reward` have not existed sender creature entry (%u) for level %u that invalid not include any player races, ignoring.", senderEntry, level);
            continue;
        }

        _mailLevelRewardStore[level].push_back(MailLevelReward(raceMask, mailTemplateId, senderEntry));

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u level dependent mail rewards in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::AddSpellToTrainer(uint32 entry, uint32 spell, uint32 spellCost, uint32 reqSkill, uint32 reqSkillValue, uint32 reqLevel)
{
    if (entry >= TRINITY_TRAINER_START_REF)
        return;

    CreatureTemplate const* cInfo = GetCreatureTemplate(entry);
    if (!cInfo)
    {
        sLog->outError(LOG_FILTER_SQL, "Table `npc_trainer` contains an entry for a non-existing creature template (Entry: %u), ignoring", entry);
        return;
    }

    if (!(cInfo->NpcFlags1 & UNIT_NPC_FLAG_TRAINER))
    {
        sLog->outError(LOG_FILTER_SQL, "Table `npc_trainer` contains an entry for a creature template (Entry: %u) without trainer flag, ignoring", entry);
        return;
    }

    SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(spell);
    if (!l_SpellInfo)
    {
        sLog->outError(LOG_FILTER_SQL, "Table `npc_trainer` contains an entry (Entry: %u) for a non-existing spell (Spell: %u), ignoring", entry, spell);
        return;
    }

    if (!SpellMgr::IsSpellValid(l_SpellInfo))
    {
        sLog->outError(LOG_FILTER_SQL, "Table `npc_trainer` contains an entry (Entry: %u) for a broken spell (Spell: %u), ignoring", entry, spell);
        return;
    }

/*    if (GetTalentSpellCost(spell))
    {
        sLog->outError(LOG_FILTER_SQL, "Table `npc_trainer` contains an entry (Entry: %u) for a non-existing spell (Spell: %u) which is a talent, ignoring", entry, spell);
        return;
    }*/

    TrainerSpellData& data = _cacheTrainerSpellStore[entry];

    TrainerSpell& trainerSpell = data.spellList[spell];
    trainerSpell.spell         = spell;
    trainerSpell.spellCost     = spellCost;
    trainerSpell.reqSkill      = reqSkill;
    trainerSpell.reqSkillValue = reqSkillValue;
    trainerSpell.reqLevel      = reqLevel;

    if (!trainerSpell.reqLevel)
        trainerSpell.reqLevel = l_SpellInfo->SpellLevel;

    // calculate learned spell for profession case when stored cast-spell
    trainerSpell.learnedSpell[0] = spell;
    for (uint8 i = 0; i < l_SpellInfo->EffectCount; ++i)
    {
        if (l_SpellInfo->Effects[i].Effect != SPELL_EFFECT_LEARN_SPELL)
            continue;
        if (trainerSpell.learnedSpell[0] == spell)
            trainerSpell.learnedSpell[0] = 0;
        // player must be able to cast spell on himself
        if (l_SpellInfo->Effects[i].TargetA.GetTarget() != 0 && l_SpellInfo->Effects[i].TargetA.GetTarget() != TARGET_UNIT_TARGET_ALLY
            && l_SpellInfo->Effects[i].TargetA.GetTarget() != TARGET_UNIT_TARGET_ANY && l_SpellInfo->Effects[i].TargetA.GetTarget() != TARGET_UNIT_CASTER)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `npc_trainer` has spell %u for trainer entry %u with learn effect which has incorrect target type, ignoring learn effect!", spell, entry);
            continue;
        }

        trainerSpell.learnedSpell[i] = l_SpellInfo->Effects[i].TriggerSpell;

        if (trainerSpell.learnedSpell[i])
        {
            SpellInfo const* learnedSpellInfo = sSpellMgr->GetSpellInfo(trainerSpell.learnedSpell[i]);
            if (learnedSpellInfo && learnedSpellInfo->IsProfession())
                data.trainerType = 2;
        }
    }

    return;
}

void ObjectMgr::LoadTrainerSpell()
{
    uint32 oldMSTime = getMSTime();

    // For reload case
    _cacheTrainerSpellStore.clear();

    QueryResult result = WorldDatabase.Query("SELECT b.entry, a.spell, a.spellcost, a.reqskill, a.reqskillvalue, a.reqlevel FROM npc_trainer AS a "
                                             "INNER JOIN npc_trainer AS b ON a.entry = -(b.spell) "
                                             "UNION SELECT * FROM npc_trainer WHERE spell > 0");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">>  Loaded 0 Trainers. DB table `npc_trainer` is empty!");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 entry         = fields[0].GetUInt32();
        uint32 spell         = fields[1].GetUInt32();
        uint32 spellCost     = fields[2].GetUInt32();
        uint32 reqSkill      = fields[3].GetUInt16();
        uint32 reqSkillValue = fields[4].GetUInt16();
        uint32 reqLevel      = fields[5].GetUInt8();

        AddSpellToTrainer(entry, spell, spellCost, reqSkill, reqSkillValue, reqLevel);

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %d Trainers in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

int ObjectMgr::LoadReferenceVendor(int32 vendor, int32 item, uint8 type, std::set<uint32>* skip_vendors)
{
    // find all items from the reference vendor
    PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_SEL_NPC_VENDOR_REF);
    stmt->setUInt32(0, uint32(item));
    stmt->setUInt8(1, type);
    PreparedQueryResult result = WorldDatabase.Query(stmt);

    if (!result)
        return 0;

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        int32 item_id = fields[0].GetInt32();

        // if item is a negative, its a reference
        if (item_id < 0)
            count += LoadReferenceVendor(vendor, -item_id, type, skip_vendors);
        else
        {
            int32  maxcount             = fields[1].GetUInt32();
            uint32 incrtime             = fields[2].GetUInt32();
            uint32 ExtendedCost         = fields[3].GetUInt32();
            uint8  type                 = fields[4].GetUInt8();
            uint32 l_PlayerConditionID  = fields[5].GetUInt32();

            if (!IsVendorItemValid(vendor, item_id, maxcount, incrtime, ExtendedCost, type, NULL, skip_vendors))
                continue;

            VendorItemData& vList = _cacheVendorItemStore[vendor];

            vList.AddItem(item_id, maxcount, incrtime, ExtendedCost, type, l_PlayerConditionID);
            ++count;
        }
    }
    while (result->NextRow());

    return count;
}

void ObjectMgr::LoadVendors()
{
    uint32 oldMSTime = getMSTime();

    // For reload case
    for (CacheVendorItemContainer::iterator itr = _cacheVendorItemStore.begin(); itr != _cacheVendorItemStore.end(); ++itr)
        itr->second.Clear();
    _cacheVendorItemStore.clear();

    std::set<uint32> skip_vendors;

    QueryResult result = WorldDatabase.Query("SELECT entry, item, maxcount, incrtime, ExtendedCost, type, PlayerConditionID FROM npc_vendor ORDER BY entry, slot ASC");
    if (!result)
    {

        sLog->outError(LOG_FILTER_SERVER_LOADING, ">>  Loaded 0 Vendors. DB table `npc_vendor` is empty!");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields           = result->Fetch();

        uint32 entry            = fields[0].GetUInt32();
        int32 item_id           = fields[1].GetInt32();

        // if item is a negative, its a reference
        if (item_id < 0)
            count += LoadReferenceVendor(entry, -item_id, 0, &skip_vendors);
        else
        {
            uint32 maxcount     = fields[2].GetUInt32();
            uint32 incrtime     = fields[3].GetUInt32();
            uint32 ExtendedCost = fields[4].GetUInt32();
            uint8  type         = fields[5].GetUInt8();
            uint32 l_PlayerConditionID = fields[6].GetUInt32();

            if (!IsVendorItemValid(entry, item_id, maxcount, incrtime, ExtendedCost, type, NULL, &skip_vendors))
                continue;

            VendorItemData& vList = _cacheVendorItemStore[entry];

            vList.AddItem(item_id, maxcount, incrtime, ExtendedCost, type, l_PlayerConditionID);
            ++count;
        }
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %d Vendors in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadGossipMenu()
{
    uint32 oldMSTime = getMSTime();

    _gossipMenusStore.clear();

    QueryResult result = WorldDatabase.Query("SELECT entry, text_id FROM gossip_menu");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0  gossip_menu entries. DB table `gossip_menu` is empty!");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        GossipMenus gMenu;

        gMenu.entry             = fields[0].GetUInt32();
        gMenu.text_id           = fields[1].GetUInt32();

        if (!GetGossipText(gMenu.text_id))
        {
            sLog->outError(LOG_FILTER_SQL, "Table gossip_menu entry %u are using non-existing text_id %u", gMenu.entry, gMenu.text_id);
            continue;
        }

        _gossipMenusStore.insert(GossipMenusContainer::value_type(gMenu.entry, gMenu));

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u gossip_menu entries in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadGossipMenuItems()
{
    uint32 oldMSTime = getMSTime();

    _gossipMenuItemsStore.clear();

    QueryResult result = WorldDatabase.Query(
        //          0              1            2           3              4
        "SELECT menu_id, id, option_icon, option_text, option_id, npc_option_npcflag, "
        //       5              6           7          8         9
        "action_menu_id, action_poi_id, box_coded, box_money, box_text "
        "FROM gossip_menu_option ORDER BY menu_id, id");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 gossip_menu_option entries. DB table `gossip_menu_option` is empty!");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        GossipMenuItems gMenuItem;

        gMenuItem.MenuId                = fields[0].GetUInt32();
        gMenuItem.OptionIndex           = fields[1].GetUInt16();
        gMenuItem.OptionIcon            = fields[2].GetUInt32();
        gMenuItem.OptionText            = fields[3].GetString();
        gMenuItem.OptionType            = fields[4].GetUInt8();
        gMenuItem.OptionNpcflag         = fields[5].GetUInt32();
        gMenuItem.ActionMenuId          = fields[6].GetUInt32();
        gMenuItem.ActionPoiId           = fields[7].GetUInt32();
        gMenuItem.BoxCoded              = fields[8].GetBool();
        gMenuItem.BoxMoney              = fields[9].GetUInt32();
        gMenuItem.BoxText               = fields[10].GetString();

        if (gMenuItem.OptionIcon >= GOSSIP_ICON_MAX)
        {
            sLog->outError(LOG_FILTER_SQL, "Table gossip_menu_option for menu %u, id %u has unknown icon id %u. Replacing with GOSSIP_ICON_CHAT", gMenuItem.MenuId, gMenuItem.OptionIndex, gMenuItem.OptionIcon);
            gMenuItem.OptionIcon = GOSSIP_ICON_CHAT;
        }

        if (gMenuItem.OptionType >= GOSSIP_OPTION_MAX)
            sLog->outError(LOG_FILTER_SQL, "Table gossip_menu_option for menu %u, id %u has unknown option id %u. Option will not be used", gMenuItem.MenuId, gMenuItem.OptionIndex, gMenuItem.OptionType);

        if (gMenuItem.ActionPoiId && !GetPointOfInterest(gMenuItem.ActionPoiId))
        {
            sLog->outError(LOG_FILTER_SQL, "Table gossip_menu_option for menu %u, id %u use non-existing action_poi_id %u, ignoring", gMenuItem.MenuId, gMenuItem.OptionIndex, gMenuItem.ActionPoiId);
            gMenuItem.ActionPoiId = 0;
        }

        _gossipMenuItemsStore.insert(GossipMenuItemsContainer::value_type(gMenuItem.MenuId, gMenuItem));
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u gossip_menu_option entries in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::AddVendorItem(uint32 entry, uint32 item, int32 maxcount, uint32 incrtime, uint32 extendedCost, uint8 type, bool persist /*= true*/, uint32 p_PlayerConditionID /* = 0*/)
{
    VendorItemData& vList = _cacheVendorItemStore[entry];
    vList.AddItem(item, maxcount, incrtime, extendedCost, type, p_PlayerConditionID);

    if (persist)
    {
        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_INS_NPC_VENDOR);

        stmt->setUInt32(0, entry);
        stmt->setUInt32(1, item);
        stmt->setUInt8(2, maxcount);
        stmt->setUInt32(3, incrtime);
        stmt->setUInt32(4, extendedCost);
        stmt->setUInt8(5, type);
        stmt->setUInt32(6, p_PlayerConditionID);

        WorldDatabase.Execute(stmt);
    }
}

bool ObjectMgr::RemoveVendorItem(uint32 entry, uint32 item, uint8 type, bool persist /*= true*/)
{
    CacheVendorItemContainer::iterator  iter = _cacheVendorItemStore.find(entry);
    if (iter == _cacheVendorItemStore.end())
        return false;

    if (!iter->second.RemoveItem(item, type))
        return false;

    if (persist)
    {
        PreparedStatement* stmt = WorldDatabase.GetPreparedStatement(WORLD_DEL_NPC_VENDOR);

        stmt->setUInt32(0, entry);
        stmt->setUInt32(1, item);
        stmt->setUInt8(2, type);

        WorldDatabase.Execute(stmt);
    }

    return true;
}

bool ObjectMgr::IsVendorItemValid(uint32 vendor_entry, uint32 id, int32 maxcount, uint32 incrtime, uint32 ExtendedCost, uint8 type, Player* player, std::set<uint32>* skip_vendors, uint32 ORnpcflag) const
{
    CreatureTemplate const* cInfo = sObjectMgr->GetCreatureTemplate(vendor_entry);
    if (!cInfo)
    {
        if (player)
            ChatHandler(player).SendSysMessage(LANG_COMMAND_VENDORSELECTION);
        else
            sLog->outError(LOG_FILTER_SQL, "Table `(game_event_)npc_vendor` have data for not existed creature template (Entry: %u), ignore", vendor_entry);
        return false;
    }

    if (!((cInfo->NpcFlags1 | ORnpcflag) & UNIT_NPC_FLAG_VENDOR))
    {
        if (!skip_vendors || skip_vendors->count(vendor_entry) == 0)
        {
            if (player)
                ChatHandler(player).SendSysMessage(LANG_COMMAND_VENDORSELECTION);
            else
                sLog->outError(LOG_FILTER_SQL, "Table `(game_event_)npc_vendor` have data for not creature template (Entry: %u) without vendor flag, ignore", vendor_entry);

            if (skip_vendors)
                skip_vendors->insert(vendor_entry);
        }
        return false;
    }

    if ((type == ITEM_VENDOR_TYPE_ITEM && !sObjectMgr->GetItemTemplate(id)) ||
        (type == ITEM_VENDOR_TYPE_CURRENCY && !sCurrencyTypesStore.LookupEntry(id)))
    {
        if (player)
            ChatHandler(player).PSendSysMessage(LANG_ITEM_NOT_FOUND, id, type);
        else
            sLog->outError(LOG_FILTER_SQL, "Table `(game_event_)npc_vendor` for Vendor (Entry: %u) have in item list non-existed item (%u, type %u), ignore", vendor_entry, id, type);
        return false;
    }

    if (ExtendedCost && !sItemExtendedCostStore.LookupEntry(ExtendedCost))
    {
        if (player)
            ChatHandler(player).PSendSysMessage(LANG_EXTENDED_COST_NOT_EXIST, ExtendedCost);
        else
            sLog->outError(LOG_FILTER_SQL, "Table `(game_event_)npc_vendor` have Item (Entry: %u) with wrong ExtendedCost (%u) for vendor (%u), ignore", id, ExtendedCost, vendor_entry);
        return false;
    }

    if (type == ITEM_VENDOR_TYPE_ITEM) // not applicable to currencies
    {
        if (maxcount > 0 && incrtime == 0)
        {
            if (player)
                ChatHandler(player).PSendSysMessage("MaxCount != 0 (%u) but IncrTime == 0", maxcount);
            else
                sLog->outError(LOG_FILTER_SQL, "Table `(game_event_)npc_vendor` has `maxcount` (%u) for item %u of vendor (Entry: %u) but `incrtime`=0, ignore", maxcount, id, vendor_entry);
            return false;
        }
        else if (maxcount == 0 && incrtime > 0)
        {
            if (player)
                ChatHandler(player).PSendSysMessage("MaxCount == 0 but IncrTime<>= 0");
            else
                sLog->outError(LOG_FILTER_SQL, "Table `(game_event_)npc_vendor` has `maxcount`=0 for item %u of vendor (Entry: %u) but `incrtime`<>0, ignore", id, vendor_entry);
            return false;
        }
    }

    VendorItemData const* vItems = GetNpcVendorItemList(vendor_entry);
    if (!vItems)
        return true;                                        // later checks for non-empty lists

    if (vItems->FindItemCostPair(id, ExtendedCost, type))
    {
        if (player)
            ChatHandler(player).PSendSysMessage(LANG_ITEM_ALREADY_IN_LIST, id, ExtendedCost, type);
        else
            sLog->outError(LOG_FILTER_SQL, "Table `npc_vendor` has duplicate items %u (with extended cost %u, type %u) for vendor (Entry: %u), ignoring", id, ExtendedCost, type, vendor_entry);
        return false;
    }

    if (type == ITEM_VENDOR_TYPE_CURRENCY && maxcount == 0)
    {
        sLog->outError(LOG_FILTER_SQL, "Table `(game_event_)npc_vendor` have Item (Entry: %u, type: %u) with missing maxcount for vendor (%u), ignore", id, type, ExtendedCost, vendor_entry); ///<  Data argument not used by format string
        return false;
    }

    return true;
}

void ObjectMgr::LoadScriptNames()
{
    uint32 oldMSTime = getMSTime();

    _scriptNamesStore.push_back("");
    QueryResult result = WorldDatabase.Query(
      "SELECT DISTINCT(ScriptName) FROM achievement_criteria_data WHERE ScriptName <> '' AND type = 11 "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM battleground_template WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM creature_template WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM gameobject_template WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM item_script_names WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM areatrigger_scripts WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM spell_script_names WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM transports WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM game_weather WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM conditions WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM outdoorpvp_template WHERE ScriptName <> '' "
      "UNION "
      "SELECT DISTINCT(script) FROM instance_template WHERE script <> ''"
      "UNION "
      "SELECT DISTINCT(ScriptName) FROM areatrigger_template WHERE ScriptName <> ''");

    if (!result)
    {

        sLog->outError(LOG_FILTER_SQL, ">> Loaded empty set of Script Names!");
        return;
    }

    uint32 count = 1;

    do
    {
        _scriptNamesStore.push_back((*result)[0].GetString());
        ++count;
    }
    while (result->NextRow());

    std::sort(_scriptNamesStore.begin(), _scriptNamesStore.end());
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %d Script Names in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

uint32 ObjectMgr::GetScriptId(const char* name)
{
    // use binary search to find the script name in the sorted vector
    // assume "" is the first element
    if (!name)
        return 0;

    ScriptNameContainer::const_iterator itr = std::lower_bound(_scriptNamesStore.begin(), _scriptNamesStore.end(), name);
    if (itr == _scriptNamesStore.end() || *itr != name)
        return 0;

    return uint32(itr - _scriptNamesStore.begin());
}

void ObjectMgr::CheckScripts(ScriptsType type, std::set<int32>& ids)
{
    ScriptMapMap* scripts = GetScriptsMapByType(type);
    if (!scripts)
        return;

    for (ScriptMapMap::const_iterator itrMM = scripts->begin(); itrMM != scripts->end(); ++itrMM)
    {
        for (ScriptMap::const_iterator itrM = itrMM->second.begin(); itrM != itrMM->second.end(); ++itrM)
        {
            switch (itrM->second.command)
            {
                case SCRIPT_COMMAND_TALK:
                {
                    if (!GetTrinityStringLocale (itrM->second.Talk.TextID))
                        sLog->outError(LOG_FILTER_SQL, "Table `%s` references invalid text id %u from `db_script_string`, script id: %u.", GetScriptsTableNameByType(type).c_str(), itrM->second.Talk.TextID, itrMM->first);

                    if (ids.find(itrM->second.Talk.TextID) != ids.end())
                        ids.erase(itrM->second.Talk.TextID);
                }
                default:
                    break;
            }
        }
    }
}

void ObjectMgr::LoadDbScriptStrings()
{
    LoadTrinityStrings("db_script_string", MIN_DB_SCRIPT_STRING_ID, MAX_DB_SCRIPT_STRING_ID);

    std::set<int32> ids;

    for (int32 i = MIN_DB_SCRIPT_STRING_ID; i < MAX_DB_SCRIPT_STRING_ID; ++i)
        if (GetTrinityStringLocale(i))
            ids.insert(i);

    for (int type = SCRIPTS_FIRST; type < SCRIPTS_LAST; ++type)
        CheckScripts(ScriptsType(type), ids);

    for (std::set<int32>::const_iterator itr = ids.begin(); itr != ids.end(); ++itr)
        sLog->outError(LOG_FILTER_SQL, "Table `db_script_string` has unused string id  %u", *itr);
}

bool LoadTrinityStrings(const char* table, int32 start_value, int32 end_value)
{
    // MAX_DB_SCRIPT_STRING_ID is max allowed negative value for scripts (scrpts can use only more deep negative values
    // start/end reversed for negative values
    if (start_value > MAX_DB_SCRIPT_STRING_ID || end_value >= start_value)
    {
        sLog->outError(LOG_FILTER_SQL, "Table '%s' load attempted with range (%d - %d) reserved by Trinity, strings not loaded.", table, start_value, end_value+1);
        return false;
    }

    return sObjectMgr->LoadTrinityStrings(table, start_value, end_value);
}

CreatureBaseStats const* ObjectMgr::GetCreatureBaseStats(uint8 level, uint8 unitClass)
{
    CreatureBaseStatsContainer::const_iterator it = _creatureBaseStatsStore.find(MAKE_PAIR16(level, unitClass));

    if (it != _creatureBaseStatsStore.end())
        return &(it->second);

    struct DefaultCreatureBaseStats : public CreatureBaseStats
    {
        DefaultCreatureBaseStats()
        {
            BaseArmor = 1;
            for (uint8 j = 0; j < MAX_CREATURE_BASE_HP; ++j)
                BaseHealth[j] = 1;
            BaseMana = 0;
        }
    };
    static const DefaultCreatureBaseStats def_stats;
    return &def_stats;
}

void ObjectMgr::LoadCreatureClassLevelStats()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT level, class, basehp0, basehp1, basehp2, basehp3, basehp4, basehp5, basemana, basearmor, attackpower, rangedattackpower, damage_base FROM creature_classlevelstats");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature base stats. DB table `creature_classlevelstats` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint8 index = 0;

        uint8 Level = fields[index++].GetInt8();
        uint8 Class = fields[index++].GetInt8();

        CreatureBaseStats stats;

        for (uint8 i = 0; i < MAX_CREATURE_BASE_HP; ++i)
            stats.BaseHealth[i] = fields[index++].GetUInt32();

        stats.BaseMana = fields[index++].GetUInt32();
        stats.BaseArmor = fields[index++].GetUInt32();

        stats.AttackPower = fields[index++].GetInt16();
        stats.RangedAttackPower = fields[index++].GetInt16();

        stats.BaseDamage = fields[index++].GetFloat();

        if (!Class || ((1 << (Class - 1)) & CLASSMASK_ALL_CREATURES) == 0)
            sLog->outError(LOG_FILTER_SQL, "Creature base stats for level %u has invalid class %u", Level, Class);

        for (uint8 i = 0; i < MAX_CREATURE_BASE_HP; ++i)
        {
            if (stats.BaseHealth[i] < 1)
            {
                sLog->outError(LOG_FILTER_SQL, "Creature base stats for class %u, level %u has invalid zero base HP[%u] - set to 1", Class, Level, i);
                stats.BaseHealth[i] = 1;
            }
        }

        _creatureBaseStatsStore[MAKE_PAIR16(Level, Class)] = stats;

        ++count;
    }
    while (result->NextRow());

    CreatureTemplate** l_CreatureTemplates = sObjectMgr->GetCreatureTemplates();
    uint32 l_LastEntry = sObjectMgr->GetCreatureTemplateStoreSize();

    for (uint32 l_Entry = 0; l_Entry < l_LastEntry; l_Entry++)
    {
        CreatureTemplate const* l_CreatureTemplate = l_CreatureTemplates[l_Entry];
        if (l_CreatureTemplate == nullptr)
            continue;

        for (uint16 lvl = l_CreatureTemplate->minlevel; lvl <= l_CreatureTemplate->maxlevel; ++lvl)
        {
            if (_creatureBaseStatsStore.find(MAKE_PAIR16(lvl, l_CreatureTemplate->unit_class)) == _creatureBaseStatsStore.end())
                sLog->outError(LOG_FILTER_SQL, "Missing base stats for creature class %u level %u", l_CreatureTemplate->unit_class, lvl);
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature base stats in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadCreatureGroupSizeStats()
{
    uint32 l_OldMSTime = getMSTime();

    QueryResult l_Result = WorldDatabase.Query("SELECT entry, difficulty, groupSize, health FROM creature_groupsizestats");
    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 creature group size stats. DB table `creature_groupsizestats` is empty.");
        return;
    }

    uint32 l_Count = 0;

    do
    {
        Field* l_Fields = l_Result->Fetch();

        uint32 l_CreatureEntry = l_Fields[0].GetUInt32();
        uint32 l_Difficulty    = l_Fields[1].GetUInt32();
        uint32 l_GroupSize     = l_Fields[2].GetUInt32();
        uint32 l_Health        = l_Fields[3].GetUInt32();

        if (!sObjectMgr->GetCreatureTemplate(l_CreatureEntry))
        {
            sLog->outError(LOG_FILTER_SQL, "Creature template entry (entry: %u) used in `creature_groupsizestats` does not exist.", l_CreatureEntry);
            continue;
        }

        if (l_Difficulty >= MaxDifficulties)
        {
            sLog->outError(LOG_FILTER_SQL, "Difficulty %u (entry %u) used in `creature_groupsizestats` is invalid.", l_Difficulty, l_CreatureEntry);
            continue;
        }

        if (l_GroupSize >= MAX_GROUP_SCALING)
        {
            sLog->outError(LOG_FILTER_SQL, "Group size %u (entry %u) used in `creature_groupsizestats` is invalid.", l_GroupSize, l_CreatureEntry);
            continue;
        }

        CreatureGroupSizeStat& l_CreatureGroupSizeStat = m_CreatureGroupSizeStore[l_CreatureEntry][l_Difficulty];
        l_CreatureGroupSizeStat.Healths[l_GroupSize] = l_Health;
        ++l_Count;
    }
    while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u creature group size stats in %u ms", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

void ObjectMgr::LoadFactionChangeAchievements()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT alliance_id, horde_id FROM player_factionchange_achievement");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 faction change achievement pairs. DB table `player_factionchange_achievement` is empty.");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 alliance = fields[0].GetUInt32();
        uint32 horde = fields[1].GetUInt32();

        if (!sAchievementStore.LookupEntry(alliance))
            sLog->outError(LOG_FILTER_SQL, "Achievement %u referenced in `player_factionchange_achievement` does not exist, pair skipped!", alliance);
        else if (!sAchievementStore.LookupEntry(horde))
            sLog->outError(LOG_FILTER_SQL, "Achievement %u referenced in `player_factionchange_achievement` does not exist, pair skipped!", horde);
        else
            FactionChange_Achievements[alliance] = horde;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u faction change achievement pairs in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadFactionChangeItems()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT alliance_id, horde_id FROM player_factionchange_items");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 faction change item pairs. DB table `player_factionchange_items` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 alliance = fields[0].GetUInt32();
        uint32 horde = fields[1].GetUInt32();

        if (!GetItemTemplate(alliance))
            sLog->outError(LOG_FILTER_SQL, "Item %u referenced in `player_factionchange_items` does not exist, pair skipped!", alliance);
        else if (!GetItemTemplate(horde))
            sLog->outError(LOG_FILTER_SQL, "Item %u referenced in `player_factionchange_items` does not exist, pair skipped!", horde);
        else
            FactionChange_Items[alliance] = horde;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u faction change item pairs in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadFactionChangeSpells()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT alliance_id, horde_id FROM player_factionchange_spells");

    if (!result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 faction change spell pairs. DB table `player_factionchange_spells` is empty.");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 alliance = fields[0].GetUInt32();
        uint32 horde = fields[1].GetUInt32();

        if (!sSpellMgr->GetSpellInfo(alliance))
            sLog->outError(LOG_FILTER_SQL, "Spell %u referenced in `player_factionchange_spells` does not exist, pair skipped!", alliance);
        else if (!sSpellMgr->GetSpellInfo(horde))
            sLog->outError(LOG_FILTER_SQL, "Spell %u referenced in `player_factionchange_spells` does not exist, pair skipped!", horde);
        else
            FactionChange_Spells[alliance] = horde;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u faction change spell pairs in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadFactionChangeReputations()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT alliance_id, horde_id FROM player_factionchange_reputations");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 faction change reputation pairs. DB table `player_factionchange_reputations` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 alliance = fields[0].GetUInt32();
        uint32 horde = fields[1].GetUInt32();

        if (!sFactionStore.LookupEntry(alliance))
            sLog->outError(LOG_FILTER_SQL, "Reputation %u referenced in `player_factionchange_reputations` does not exist, pair skipped!", alliance);
        else if (!sFactionStore.LookupEntry(horde))
            sLog->outError(LOG_FILTER_SQL, "Reputation %u referenced in `player_factionchange_reputations` does not exist, pair skipped!", horde);
        else
            FactionChange_Reputation[alliance] = horde;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u faction change reputation pairs in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadFactionChangeTitles()
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = WorldDatabase.Query("SELECT alliance_id, horde_id FROM player_factionchange_titles");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 faction change title pairs. DB table `player_factionchange_title` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 alliance = fields[0].GetUInt32();
        uint32 horde = fields[1].GetUInt32();

        if (!sCharTitlesStore.LookupEntry(alliance))
            sLog->outError(LOG_FILTER_SQL, "Title %u referenced in `player_factionchange_title` does not exist, pair skipped!", alliance);
        else if (!sCharTitlesStore.LookupEntry(horde))
            sLog->outError(LOG_FILTER_SQL, "Title %u referenced in `player_factionchange_title` does not exist, pair skipped!", horde);
        else
            FactionChange_Titles[alliance] = horde;

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u faction change title pairs in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadHotfixData(bool p_Reload)
{
    uint32 oldMSTime = getMSTime();

    auto l_ProcessHotifx = [this](HotfixInfo p_Info, bool p_Reloaded) -> void
    {
        if (p_Reloaded)
        {
            auto l_It = std::find_if(_hotfixData.begin(), _hotfixData.end(), [p_Info](HotfixInfo const& p_Elem) -> bool
            {
                return p_Elem.Type == p_Info.Type && p_Elem.Entry == p_Info.Entry;
            });

            if (l_It == _hotfixData.end())
                _hotfixData.push_back(p_Info);

            WorldPacket l_Notify(SMSG_HOTFIX_NOTIFY, 100);
            l_Notify << uint32(p_Info.Type);
            l_Notify << uint32(p_Info.Entry);
            l_Notify << uint32(p_Info.Timestamp);

            sWorld->SendGlobalMessage(&l_Notify);
        }
        else
            _hotfixData.push_back(p_Info);
    };

    QueryResult result = HotfixDatabase.Query("SELECT Entry, Hash, Date FROM _hotfixs");

    uint32 l_Count = 0;

    if (result)
    {
        if (!p_Reload)
        {
            _hotfixData.clear();
            _hotfixData.reserve(result->GetRowCount());
        }

        do
        {
            Field* l_Fields = result->Fetch();

            HotfixInfo l_Infos;
            l_Infos.Entry       = l_Fields[0].GetUInt32();
            l_Infos.Type        = l_Fields[1].GetUInt32();
            l_Infos.Timestamp   = l_Fields[2].GetUInt32();

            l_ProcessHotifx(l_Infos, p_Reload);

            ++l_Count;
        }
        while (result->NextRow());
    }

    result = HotfixDatabase.Query("SELECT ID FROM _custom_items");

    if (result)
    {
        do
        {
            Field* l_Fields = result->Fetch();
            uint32 l_ItemID = l_Fields[0].GetUInt32();

            if (sItemStore.LookupEntry(l_ItemID))
            {
                HotfixInfo l_Infos;
                l_Infos.Type         = sItemStore.GetHash();
                l_Infos.Timestamp    = time(nullptr);
                l_Infos.Entry        = l_ItemID;
                l_ProcessHotifx(l_Infos, p_Reload);
            }

            if (sItemSparseStore.LookupEntry(l_ItemID))
            {
                HotfixInfo l_Infos;
                l_Infos.Type        = sItemSparseStore.GetHash();
                l_Infos.Timestamp   = time(nullptr);
                l_Infos.Entry       = l_ItemID;
                l_ProcessHotifx(l_Infos, p_Reload);
            }

            for (uint32 l_I = 0; l_I < sPvpItemStore.GetNumRows(); ++l_I)
            {
                PvpItemEntry const* l_Entry = sPvpItemStore.LookupEntry(l_I);
                if (l_Entry == nullptr || l_Entry->itemId != l_ItemID)
                    continue;

                HotfixInfo l_Infos;
                l_Infos.Type        = sPvpItemStore.GetHash();
                l_Infos.Timestamp   = time(nullptr);
                l_Infos.Entry       = l_I;
                l_ProcessHotifx(l_Infos, p_Reload);
            }

            for (uint32 l_I = 0; l_I < sItemEffectStore.GetNumRows(); ++l_I)
            {
                ItemEffectEntry const* l_Entry = sItemEffectStore.LookupEntry(l_I);

                if (!l_Entry || l_Entry->ItemID != l_ItemID)
                    continue;

                HotfixInfo l_Infos;
                l_Infos.Type        = sItemEffectStore.GetHash();
                l_Infos.Timestamp   = time(nullptr);
                l_Infos.Entry       = l_I;
                l_ProcessHotifx(l_Infos, p_Reload);
            }

            for (uint32 l_I = 0; l_I < sItemModifiedAppearanceStore.GetNumRows(); ++l_I)
            {
                ItemModifiedAppearanceEntry const* l_Entry = sItemModifiedAppearanceStore.LookupEntry(l_I);

                if (!l_Entry || l_Entry->ItemID != l_ItemID)
                    continue;

                HotfixInfo l_Infos;
                l_Infos.Type        = sItemModifiedAppearanceStore.GetHash();
                l_Infos.Timestamp   = time(nullptr);
                l_Infos.Entry       = l_I;
                l_ProcessHotifx(l_Infos, p_Reload);

                if (sItemAppearanceStore.LookupEntry(l_Entry->AppearanceID))
                {
                    l_Infos.Type        = sItemAppearanceStore.GetHash();
                    l_Infos.Timestamp   = time(nullptr);
                    l_Infos.Entry       = l_Entry->AppearanceID;
                    l_ProcessHotifx(l_Infos, p_Reload);
                }
            }
            ++l_Count;
        } while (result->NextRow());
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u hotfix info entries in %u ms", l_Count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadHotfixTableHashs()
{
    QueryResult l_Result = HotfixDatabase.Query("SELECT ID, Name FROM _hashs");

    if (!l_Result)
        return;

    HotfixTableID.clear();

    do
    {
        Field* l_Fields = l_Result->Fetch();

        HotfixTableID[l_Fields[1].GetString()] = l_Fields[0].GetUInt32();
    } while (l_Result->NextRow());
}

void ObjectMgr::LoadPhaseDefinitions()
{
    _PhaseDefinitionStore.clear();

    uint32 oldMSTime = getMSTime();

    //                                                 0       1       2         3            4           5
    QueryResult result = WorldDatabase.Query("SELECT zoneId, entry, phasemask, phaseId, terrainswapmap, flags FROM `phase_definitions` ORDER BY `entry` ASC");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 phasing definitions. DB table `phase_definitions` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        PhaseDefinition pd;

        pd.zoneId                = fields[0].GetUInt32();
        pd.entry                 = fields[1].GetUInt32();
        pd.phasemask             = fields[2].GetUInt32();
        pd.phaseId               = fields[3].GetUInt32();
        pd.terrainswapmap        = fields[4].GetUInt32();
        pd.flags                 = fields[5].GetUInt8();

        // Checks
        if ((pd.flags & PHASE_FLAG_OVERWRITE_EXISTING) && (pd.flags & PHASE_FLAG_NEGATE_PHASE))
        {
            sLog->outError(LOG_FILTER_SQL, "Flags defined in phase_definitions in zoneId %d and entry %u does contain PHASE_FLAG_OVERWRITE_EXISTING and PHASE_FLAG_NEGATE_PHASE. Setting flags to PHASE_FLAG_OVERWRITE_EXISTING", pd.zoneId, pd.entry);
            pd.flags &= ~PHASE_FLAG_NEGATE_PHASE;
        }

        _PhaseDefinitionStore[pd.zoneId].push_back(pd);

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u phasing definitions in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadSpellPhaseInfo()
{
    _SpellPhaseStore.clear();

    uint32 oldMSTime = getMSTime();

    //                                               0       1            2
    QueryResult result = WorldDatabase.Query("SELECT id, phasemask, terrainswapmap FROM `spell_phase`");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 spell dbc infos. DB table `spell_phase` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        SpellPhaseInfo spellPhaseInfo;
        spellPhaseInfo.spellId                = fields[0].GetUInt32();

        SpellInfo const* spell = sSpellMgr->GetSpellInfo(spellPhaseInfo.spellId);
        if (!spell)
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u defined in `spell_phase` does not exists, skipped.", spellPhaseInfo.spellId);
            continue;
        }

        if (!spell->HasAura(SPELL_AURA_PHASE))
        {
            sLog->outError(LOG_FILTER_SQL, "Spell %u defined in `spell_phase` does not have aura effect type SPELL_AURA_PHASE, useless value.", spellPhaseInfo.spellId);
            continue;
        }

        spellPhaseInfo.phasemask              = fields[1].GetUInt32();
        spellPhaseInfo.terrainswapmap         = fields[2].GetUInt32();

        _SpellPhaseStore[spellPhaseInfo.spellId] = spellPhaseInfo;

        ++count;
    }
    while (result->NextRow());
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u spell dbc infos in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadBattlePetTemplate()
{
    uint32 oldMSTime = getMSTime();

    _battlePetTemplateStore.clear();

    QueryResult result = WorldDatabase.Query("SELECT species, breed, quality, level FROM battlepet_template");

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 battlepet template. DB table `battlepet_template` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();
        BattlePetTemplate temp;
        temp.Species = fields[0].GetUInt32();
        temp.Breed = fields[1].GetUInt32();
        temp.Quality = fields[2].GetUInt32();
        temp.Level = fields[3].GetUInt32();
        _battlePetTemplateStore[temp.Species] = temp;
        count += 1;
    } while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u battlepet template in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}

void ObjectMgr::LoadBattlePetNpcTeamMember()
{
    uint32 l_OldMSTime = getMSTime();

    m_BattlePetNpcTeamMembers.clear();

    QueryResult l_Result = WorldDatabase.Query("SELECT NpcID, Specie, Level, Ability1, Ability2, Ability3 FROM battlepet_npc_team_member");

    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 battlepet npc team member. DB table `battlepet_npc_team_member` is empty.");
        return;
    }

    uint32 l_Count = 0;
    do
    {
        Field* l_Fields = l_Result->Fetch();

        BattlePetNpcTeamMember l_Current;
        l_Current.Specie        = l_Fields[1].GetUInt32();
        l_Current.Level         = l_Fields[2].GetUInt32();
        l_Current.Ability[0]    = l_Fields[2].GetUInt32();
        l_Current.Ability[1]    = l_Fields[3].GetUInt32();
        l_Current.Ability[2]    = l_Fields[4].GetUInt32();

        m_BattlePetNpcTeamMembers[l_Fields[0].GetUInt32()].push_back(l_Current);
        l_Count += 1;

    } while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u battlepet npc team member in %u ms.", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

#include <fstream>
#include <iostream>

/// Compute battle pet spawns
void ObjectMgr::ComputeBattlePetSpawns()
{
    uint32 l_OldMSTime = getMSTime();
    QueryResult l_Result = WorldDatabase.Query("SELECT CritterEntry, BattlePetEntry FROM temp_battlepet_spawn_relation a");

    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> ComputeBattlePetSpawns No battlepet relation");
        return;
    }

    std::map<uint32, uint32> l_BattlePetToCritter;
    do
    {
        Field* l_Fields = l_Result->Fetch();
        l_BattlePetToCritter[l_Fields[1].GetUInt32()] = l_Fields[0].GetUInt32();
    } while (l_Result->NextRow());

    l_Result = WorldDatabase.Query("SELECT MapID, a.Zone, BattlePetNPCID, XPos, YPos, MinLevel, MaxLevel FROM temp_battlepet_tocompute a");

    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> ComputeBattlePetSpawns No data");
        return;
    }

    struct PoolInfo
    {
        uint32 ZoneID;
        std::map<uint32, uint32> CountPerBattlePetTemplateEntry;
        uint32 MinLevel;
        uint32 MaxLevel;
    };

    std::map<uint32, PoolInfo> l_PoolInfosPerZoneID;
    std::map<uint32, uint32> l_MissingCorelations;

    std::ofstream l_OutSpawns;
    l_OutSpawns.open("BattlePetSpawns.sql");

    do
    {
        Field* l_Fields = l_Result->Fetch();
        uint32 l_MapID = l_Fields[0].GetUInt32();
        uint32 l_ZoneID = l_Fields[1].GetUInt32();
        uint32 l_BattlePetNpcID = l_Fields[2].GetUInt32();
        double l_XPos = l_Fields[3].GetDouble();
        double l_YPos = l_Fields[4].GetDouble();
        uint32 l_MinLevel = l_Fields[5].GetUInt32();
        uint32 l_MaxLevel = l_Fields[6].GetUInt32();

        if (l_BattlePetToCritter.find(l_BattlePetNpcID) == l_BattlePetToCritter.end())
        {
            l_MissingCorelations[l_BattlePetNpcID] = 1;
            continue;
        }

        if (!MapManager::IsValidMapCoord(l_MapID, l_XPos, l_YPos))
        {
            printf("Map %u Zone %u Npc %u X %f Y %F invalid map coord\n", l_MapID, l_ZoneID, l_BattlePetNpcID, l_XPos, l_YPos);
            continue;
        }

        Map const* l_Map = sMapMgr->CreateBaseMap(l_MapID);
        float l_ZPos = l_Map->GetHeight(l_XPos, l_YPos, MAX_HEIGHT) + 0.5f;

        std::string l_Query = "INSERT INTO creature(id, map, zoneID, spawnMask, phaseMask, position_x, position_y, position_z, spawntimesecs) VALUES (";
        l_Query += std::to_string(l_BattlePetToCritter[l_BattlePetNpcID]) + ", " + std::to_string(l_MapID) + ", " + std::to_string(l_ZoneID) + ", 1, 1, " + std::to_string(l_XPos) + ", " + std::to_string(l_YPos) + ", " + std::to_string(l_ZPos) + ", 120);\n";

        l_OutSpawns << l_Query << std::flush;

        l_PoolInfosPerZoneID[l_ZoneID].ZoneID = l_ZoneID;
        l_PoolInfosPerZoneID[l_ZoneID].MinLevel = l_MinLevel;
        l_PoolInfosPerZoneID[l_ZoneID].MaxLevel = l_MaxLevel;

        if (l_PoolInfosPerZoneID[l_ZoneID].CountPerBattlePetTemplateEntry.find(l_BattlePetNpcID) == l_PoolInfosPerZoneID[l_ZoneID].CountPerBattlePetTemplateEntry.end())
            l_PoolInfosPerZoneID[l_ZoneID].CountPerBattlePetTemplateEntry[l_BattlePetNpcID] = 1;
        else
            l_PoolInfosPerZoneID[l_ZoneID].CountPerBattlePetTemplateEntry[l_BattlePetNpcID] = 1 + l_PoolInfosPerZoneID[l_ZoneID].CountPerBattlePetTemplateEntry[l_BattlePetNpcID];
    } while (l_Result->NextRow());

    for (std::map<uint32, uint32>::iterator l_Current = l_MissingCorelations.begin(); l_Current != l_MissingCorelations.end(); l_Current++)
        printf("Npc %u no critter npc found\n", l_Current->first);

    l_OutSpawns.close();

    std::ofstream l_OutPools;
    l_OutPools.open("BattlePetPools.sql");
    for (std::map<uint32, PoolInfo>::iterator l_Current = l_PoolInfosPerZoneID.begin(); l_Current != l_PoolInfosPerZoneID.end(); l_Current++)
    {
        PoolInfo& l_PoolInfo = l_Current->second;

        for (std::map<uint32, uint32>::iterator l_CurrentTemplate = l_PoolInfo.CountPerBattlePetTemplateEntry.begin(); l_CurrentTemplate != l_PoolInfo.CountPerBattlePetTemplateEntry.end(); l_CurrentTemplate++)
        {
            uint32 l_RespawnTime = 60;
            uint32 l_Replace = l_BattlePetToCritter[l_CurrentTemplate->first];
            uint32 l_Max = float(l_CurrentTemplate->second) > 1 ? (float(l_CurrentTemplate->second) * 0.95f) : 1;

            if (l_BattlePetToCritter[l_CurrentTemplate->first] == l_CurrentTemplate->first)
                l_Max = l_CurrentTemplate->second;

            uint32 l_Species = 0;

            for (std::size_t l_I = 0; l_I < sBattlePetSpeciesStore.GetNumRows(); ++l_I)
            {
                BattlePetSpeciesEntry const* l_Entry = sBattlePetSpeciesStore.LookupEntry(l_I);

                if (!l_Entry || l_Entry->entry != l_CurrentTemplate->first)
                    continue;

                l_Species = l_Entry->id;
                break;
            }

            if (l_Species == 0 || l_Replace == 0)
            {
                printf("No species or replacement for npc %u found\n", l_CurrentTemplate->first);
                continue;
            }

            std::string l_Query = "INSERT INTO `wild_battlepet_zone_pool` (`Zone`, `Species`, `Replace`, `Max`, `RespawnTime`, `MinLevel`, `MaxLevel`, `Breed0`, `Breed1`, `Breed2`, `Breed3`, `Breed4`, `Breed5`, `Breed6`, `Breed7`, `Breed8`, `Breed9`) VALUES (";
            l_Query += std::to_string(l_PoolInfo.ZoneID) + ", " + std::to_string(l_Species) + ", " + std::to_string(l_Replace) + ", " + std::to_string(l_Max) + ", " + std::to_string(l_RespawnTime) + ", " + std::to_string(l_PoolInfo.MinLevel) + ", " + std::to_string(l_PoolInfo.MaxLevel) + ", '3', '3', '3', '3', '3', '3', '3', '3', '3', '3');\n";

            l_OutPools << l_Query << std::flush;
        }
    }

    l_OutPools.close();

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> ComputeBattlePetSpawns %u ms.", GetMSTimeDiffToNow(l_OldMSTime));

#ifdef _WIN32
    system("pause");
#endif
}

GameObjectTemplate const* ObjectMgr::GetGameObjectTemplate(uint32 entry)
{
    GameObjectTemplateContainer::const_iterator itr = _gameObjectTemplateStore.find(entry);
    if (itr != _gameObjectTemplateStore.end())
        return &(itr->second);

    return NULL;
}

CreatureTemplate const* ObjectMgr::GetCreatureTemplate(uint32 p_Entry)
{
    if (p_Entry >= m_CreatureTemplateStoreSize)
        return nullptr;

    return m_CreatureTemplateStore[p_Entry];
}

CreatureTemplate const* ObjectMgr::GetRandomTemplate(CreatureType p_Type)
{
    CreatureTemplate const* l_Template = nullptr;

    for (uint32 i = 0; i < m_CreatureTemplateStoreSize; i++)
    {
        if (l_Template != nullptr && l_Template->type == p_Type)
            return l_Template;
        else
            l_Template = GetCreatureTemplate(urand(0, m_CreatureTemplateStoreSize));
    }

    return l_Template;
}

VehicleAccessoryList const* ObjectMgr::GetVehicleAccessoryList(Vehicle* veh) const
{
    if (Creature* cre = veh->GetBase()->ToCreature())
    {
        // Give preference to GUID-based accessories
        VehicleAccessoryContainer::const_iterator itr = _vehicleAccessoryStore.find(cre->GetDBTableGUIDLow());
        if (itr != _vehicleAccessoryStore.end())
            return &itr->second;
    }

    // Otherwise return entry-based
    VehicleAccessoryContainer::const_iterator itr = _vehicleTemplateAccessoryStore.find(veh->GetCreatureEntry());
    if (itr != _vehicleTemplateAccessoryStore.end())
        return &itr->second;
    return NULL;
}

 void ObjectMgr::LoadResearchSiteZones()
{
    uint32 l_OldMSTime = getMSTime();

    for (auto itr : sResearchSiteSet)
    {
        for (uint32 i = 0; i < sQuestPOIPointStore.GetNumRows(); ++i)
        {
            if (QuestPOIPointEntry const* POI = sQuestPOIPointStore.LookupEntry(i))
            {
                if (POI->ID != itr->POIid)
                    continue;

                ResearchZoneEntry &ptr = _researchZoneMap[itr->ID];
                ptr.coords.push_back(ResearchPOIPoint(POI->x, POI->y));
                ptr.map = itr->mapId;
                ptr.zone = sMapMgr->GetZoneId(ptr.map, POI->x, POI->y, 0.0f);
                ptr.level = 0;

                for (uint32 i = 0; i < sAreaStore.GetNumRows(); ++i)
                {
                    AreaTableEntry const* area = sAreaStore.LookupEntry(i);
                    if (!area)
                        continue;

                    if (area->ContinentID == ptr.map && area->ParentAreaID == ptr.zone)
                    {
                        ptr.level = area->ExplorationLevel;
                        break;
                    }
                }
            }
        }
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %lu Archeology research site zones in %u ms.", (unsigned long)sResearchSiteSet.size(), GetMSTimeDiffToNow(l_OldMSTime));
}

void ObjectMgr::LoadResearchSiteLoot()
{
    QueryResult result = WorldDatabase.Query("SELECT site_id, x, y, z, race FROM research_loot");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 Archeology site loot. DB table `research_loot` is empty.");
        return;
    }

    uint32 l_OldMSTime = getMSTime();

    do
    {
        ResearchLootEntry dg;
        {
            Field* fields = result->Fetch();

            dg.id = uint16(fields[0].GetUInt32());
            dg.x = fields[1].GetFloat();
            dg.y = fields[2].GetFloat();
            dg.z = fields[3].GetFloat();
            dg.ResearchBranchID = fields[4].GetUInt32();
        }

        _researchLoot.push_back(dg);
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %lu Archeology research site loot in %u ms.", (unsigned long)_researchLoot.size(), GetMSTimeDiffToNow(l_OldMSTime));
}

void ObjectMgr::LoadSkipUpdateZone()
{
    skipData.clear();

    _skipUpdateCount = ConfigMgr::GetIntDefault("ZoneSkipUpdate.count", 1);

    QueryResult result = WorldDatabase.PQuery("SELECT zone FROM zone_skip_update");
    if (!result)
        return;

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();
        uint32 zoneId = fields[0].GetUInt32();
        skipData[zoneId] = true;
        count++;
    }
    while (result->NextRow());

     sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u zone skip update.", count);
}

void ObjectMgr::RestructCreatureGUID(uint32 nbLigneToRestruct)
{
    QueryResult result = WorldDatabase.PQuery("SELECT guid FROM creature ORDER BY guid DESC LIMIT %u;", nbLigneToRestruct);

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Soucis lors du select de la fonction 'restructCreatureGUID' (nombre de lignes : %u)", nbLigneToRestruct);
        return;
    }

    std::vector<uint32> guidList;

    do
    {
        Field* fields = result->Fetch();
        guidList.push_back(fields[0].GetUInt32());
    }
    while (result->NextRow());

    uint32 upperGUID = 0;
    uint32 lowerGUID = 0;

    std::map<uint32, uint32> newGUIDList;

    for (int32 i = guidList.size() - 2; i >= 0; --i)
    {
        upperGUID = guidList[i];
        lowerGUID = guidList[i + 1];

        if (upperGUID != lowerGUID + 1)
        {
            newGUIDList[upperGUID] = lowerGUID + 1;
            guidList[i] = lowerGUID + 1;
        }
    }

    uint32 oldGUID   = 0;
    uint32 newGUID   = 0;

    SQLTransaction worldTrans = WorldDatabase.BeginTransaction();

    for (std::map<uint32, uint32>::iterator Itr = newGUIDList.begin(); Itr != newGUIDList.end(); ++Itr)
    {
        oldGUID = Itr->first;
        newGUID = Itr->second;

        // World Database
        std::ostringstream creature_ss;
        creature_ss << "UPDATE creature SET guid = "                        << newGUID << " WHERE guid = "          << oldGUID << "; ";
        worldTrans->Append(creature_ss.str().c_str());

        std::ostringstream addon_ss;
        addon_ss << "UPDATE creature_addon SET guid = "                     << newGUID << " WHERE guid = "          << oldGUID << "; ";
        worldTrans->Append(addon_ss.str().c_str());

        std::ostringstream formation1_ss;
        formation1_ss << "UPDATE creature_formations SET leaderGUID = "     << newGUID << " WHERE leaderGUID = "    << oldGUID << "; ";
        worldTrans->Append(formation1_ss.str().c_str());

        std::ostringstream formation2_ss;
        formation2_ss << "UPDATE creature_formations SET memberGUID = "     << newGUID << " WHERE memberGUID = "    << oldGUID << "; ";
        worldTrans->Append(formation2_ss.str().c_str());

        std::ostringstream transport_ss;
        transport_ss << "UPDATE creature_transport SET guid = "             << newGUID << " WHERE guid = "          << oldGUID << "; ";
        worldTrans->Append(transport_ss.str().c_str());

        std::ostringstream game_event_ss;
        game_event_ss << "UPDATE game_event_creature SET guid = "           << newGUID << " WHERE guid = "          << oldGUID << "; ";
        worldTrans->Append(game_event_ss.str().c_str());

        std::ostringstream pool_ss;
        pool_ss << "UPDATE pool_creature SET guid = "                       << newGUID << " WHERE guid = "          << oldGUID << "; ";
        worldTrans->Append(pool_ss.str().c_str());
    }

    std::ostringstream increment_ss;
    // Le dernier newGUID est le plus haut
    increment_ss << "ALTER TABLE creature AUTO_INCREMENT = "                << newGUID << ";";
    worldTrans->Append(increment_ss.str().c_str());

    WorldDatabase.DirectCommitTransaction(worldTrans);

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "%u lignes ont ete restructuree.", nbLigneToRestruct);
}

void ObjectMgr::RestructGameObjectGUID(uint32 nbLigneToRestruct)
{
    QueryResult result = WorldDatabase.PQuery("SELECT guid FROM gameobject ORDER BY guid DESC LIMIT %u;", nbLigneToRestruct);

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, "Soucis lors du select de la fonction 'RestructGameObjectGUID' (nombre de lignes : %u)", nbLigneToRestruct);
        return;
    }

    std::vector<uint32> guidList;

    do
    {
        Field* fields = result->Fetch();
        guidList.push_back(fields[0].GetUInt32());
    }
    while (result->NextRow());

    uint32 upperGUID = 0;
    uint32 lowerGUID = 0;

    std::map<uint32, uint32> newGUIDList;

    for (int32 i = guidList.size() - 2; i >= 0; --i)
    {
        upperGUID = guidList[i];
        lowerGUID = guidList[i + 1];

        if (upperGUID != lowerGUID + 1)
        {
            newGUIDList[upperGUID] = lowerGUID + 1;
            guidList[i] = lowerGUID + 1;
        }
    }

    uint32 oldGUID   = 0;
    uint32 newGUID   = 0;

    SQLTransaction worldTrans = WorldDatabase.BeginTransaction();

    for (std::map<uint32, uint32>::iterator Itr = newGUIDList.begin(); Itr != newGUIDList.end(); ++Itr)
    {
        oldGUID = Itr->first;
        newGUID = Itr->second;

        // World Database
        std::ostringstream gameobject_ss;
        gameobject_ss << "UPDATE gameobject SET guid = "            << newGUID << " WHERE guid = " << oldGUID << "; ";
        worldTrans->Append(gameobject_ss.str().c_str());

        std::ostringstream game_event_ss;
        game_event_ss << "UPDATE game_event_gameobject SET guid = " << newGUID << " WHERE guid = " << oldGUID << "; ";
        worldTrans->Append(game_event_ss.str().c_str());

        std::ostringstream pool_ss;
        pool_ss << "UPDATE pool_gameobject SET guid = "             << newGUID << " WHERE guid = " << oldGUID << "; ";
        worldTrans->Append(pool_ss.str().c_str());
    }

    std::ostringstream increment_ss;
    // Le dernier newGUID est le plus haut
    increment_ss << "ALTER TABLE creature AUTO_INCREMENT = "        << newGUID << ";";
    worldTrans->Append(increment_ss.str().c_str());

    WorldDatabase.DirectCommitTransaction(worldTrans);

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, "%u lignes ont ete restructuree.", nbLigneToRestruct);
}

void ObjectMgr::LoadGuildChallengeRewardInfo()
{
    uint32 oldMSTime = getMSTime();
    QueryResult result = WorldDatabase.Query("SELECT Type, Experience, Gold, Gold2, Count FROM guild_challenge_reward");
    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 guild challenge reward data.");
        return;
    }

    _challengeRewardData.reserve(result->GetRowCount());

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 type = fields[0].GetUInt32();
        if (type >= ChallengeMax)
        {
            sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> guild_challenge_reward has unknown challenge type %u, skip.", type);
            continue;
        }

        GuildChallengeReward reward;
        {
            reward.Experience = fields[1].GetUInt32();
            reward.Gold = fields[2].GetUInt32();
            reward.Gold2 = fields[3].GetUInt32();
            reward.ChallengeCount = fields[4].GetUInt32();
        }

        _challengeRewardData.push_back(reward);
        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u guild challenge reward data in %u ms.", count, GetMSTimeDiffToNow(oldMSTime));
}


void ObjectMgr::LoadCharacterTemplateData()
{
    uint32 l_OldMSTime = getMSTime();
    QueryResult l_Result = WorldDatabase.Query("SELECT id, class, name, description, level, money, alianceX, alianceY, alianceZ, alianceO, alianceMap, hordeX, hordeY, hordeZ, hordeO, hordeMap, hordeDefaultRace, allianceDefaultRace FROM character_template WHERE disabled = 0");
    uint32 l_Count = 0;

    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 character templates.");
        return;
    }

    do
    {
        Field* l_Fields = l_Result->Fetch();
        uint32 l_ID = l_Fields[0].GetUInt32();

        CharacterTemplate* l_CharacterTemplate = new CharacterTemplate();
        {
            l_CharacterTemplate->m_ID = l_ID;
            l_CharacterTemplate->m_PlayerClass = l_Fields[1].GetUInt8();
            l_CharacterTemplate->m_Name = l_Fields[2].GetString();
            l_CharacterTemplate->m_Description = l_Fields[3].GetString();
            l_CharacterTemplate->m_Level = l_Fields[4].GetUInt8();
            l_CharacterTemplate->m_Money = l_Fields[5].GetUInt64();
            l_CharacterTemplate->m_AliancePos.Relocate(l_Fields[6].GetFloat(), l_Fields[7].GetFloat(), l_Fields[8].GetFloat(), l_Fields[9].GetFloat());
            l_CharacterTemplate->m_AlianceMapID = l_Fields[10].GetInt16();
            l_CharacterTemplate->m_HordePos.Relocate(l_Fields[11].GetFloat(), l_Fields[12].GetFloat(), l_Fields[13].GetFloat(), l_Fields[14].GetFloat());
            l_CharacterTemplate->m_HordeMapID = l_Fields[15].GetInt16();
            l_CharacterTemplate->m_HordeDefaultRace = l_Fields[16].GetUInt8();
            l_CharacterTemplate->m_AllianceDefaultRace = l_Fields[17].GetUInt8();
        }

        if (!sChrClassesStore.LookupEntry(l_CharacterTemplate->m_PlayerClass))
        {
            sLog->outError(LOG_FILTER_SQL, "Template %u defined in `character_template` with class %u does not exists, skipped.", l_ID, l_CharacterTemplate->m_PlayerClass);
            delete l_CharacterTemplate;
            continue;
        }

        if (!l_ID)
        {
            sLog->outError(LOG_FILTER_SQL, "Template %u defined in `character_template` cannot have null index, skipped.", l_ID);
            delete l_CharacterTemplate;
            continue;
        }

        if (!l_CharacterTemplate->m_Level)
            l_CharacterTemplate->m_Level = 1;

        QueryResult l_ItemResult = WorldDatabase.PQuery("SELECT itemID, faction, count, type FROM character_template_item WHERE id = %i OR id = %i", l_ID, MAX_CLASSES);
        if (l_ItemResult)
        {
            do
            {
                Field* l_ItemFields = l_ItemResult->Fetch();

                CharacterTemplate::TemplateItem l_TemplateItem;
                {
                    l_TemplateItem.m_ItemID = l_ItemFields[0].GetUInt32();
                    l_TemplateItem.m_Faction = l_ItemFields[1].GetUInt32();
                    l_TemplateItem.m_Count = l_ItemFields[2].GetUInt32();
                    l_TemplateItem.m_Type = l_ItemFields[3].GetUInt8();
                }

                if (!GetItemTemplate(l_TemplateItem.m_ItemID))
                {
                    sLog->outError(LOG_FILTER_SQL, "ItemID %u defined in `character_template_item` does not exist, ignoring.", l_TemplateItem.m_ItemID);
                    continue;
                }

                l_CharacterTemplate->m_TemplateItems.push_back(l_TemplateItem);
            }
            while (l_ItemResult->NextRow());
        }

        QueryResult l_SpellsResult = WorldDatabase.PQuery("SELECT spellId FROM character_template_spell WHERE id = %i OR id = %i", l_ID, MAX_CLASSES);
        if (l_SpellsResult)
        {
            do
            {
                Field* l_SpellFields = l_SpellsResult->Fetch();
                uint32 l_SpellID = l_SpellFields[0].GetUInt32();

                if (!sSpellMgr->GetSpellInfo(l_SpellID))
                {
                    sLog->outError(LOG_FILTER_SQL, "SpellId %u defined in `character_template_spell` does not exist, ignoring.", l_SpellID);
                    continue;
                }

                l_CharacterTemplate->m_SpellIDs.push_back(l_SpellID);
            }
            while (l_SpellsResult->NextRow());
        }

        // NYI - will fix later
        QueryResult l_ReputationResult = WorldDatabase.PQuery("SELECT factionID, reputation FROM character_template_reputation WHERE id = %i OR id = %i", l_ID, MAX_CLASSES);
        if (l_ReputationResult)
        {
            do
            {
                Field* l_ReputationFields = l_ReputationResult->Fetch();
                uint32 l_FactionID = l_ReputationFields[0].GetUInt32();

                if (!sFactionStore.LookupEntry(l_FactionID))
                {
                    sLog->outError(LOG_FILTER_SQL, "FactionID %u defined in `character_template_reputation` does not exist, ignoring.", l_FactionID);
                    continue;
                }

                CharacterTemplate::TemplateFaction l_TemplateReputation;
                l_TemplateReputation.m_FactionID = l_FactionID;
                l_TemplateReputation.m_Reputaion = l_ReputationFields[1].GetUInt32();
                l_CharacterTemplate->m_TemplateFactions.push_back(l_TemplateReputation);
            }
            while (l_ReputationResult->NextRow());
        }

        m_CharacterTemplatesStore[l_ID] = l_CharacterTemplate;
        l_Count++;
    }

    while (l_Result->NextRow());
    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u character templates %u ms.", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

CharacterTemplate const* ObjectMgr::GetCharacterTemplate(uint32 p_ID) const
{
    CharacterTemplates::const_iterator l_Iter = m_CharacterTemplatesStore.find(p_ID);
    return l_Iter != m_CharacterTemplatesStore.end() ? l_Iter->second : nullptr;
}

void ObjectMgr::LoadQuestObjectives()
{
    uint32 l_OldMSTime = getMSTime();

    m_questObjectiveLookup.clear();
    for (auto& l_Quest : _questTemplates)
    {
        l_Quest.second->QuestObjectives.clear();

        for (int l_I = 0; l_I < QUEST_OBJECTIVE_TYPE_END; ++l_I)
            l_Quest.second->QuestObjecitveTypeCount[l_I] = 0;
    }

    QueryResult l_Result = WorldDatabase.Query("SELECT `ID`,`QuestID`,`Type`,`Index`,`ObjectID`,`Amount`,`Flags`,`UnkFloat`,`Description`,`VisualEffects` FROM quest_template_objective ORDER BY QuestID ASC");
    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 Quest Objectives. DB table `quest_objective` is empty.");
        return;
    }

    uint32 l_Count = 0;
    do
    {
        Field* l_Fields = l_Result->Fetch();

        uint32  l_ObjectiveID               = l_Fields[0].GetUInt32();
        uint32  l_ObjectiveQuestId          = l_Fields[1].GetUInt32();
        uint8   l_ObjectiveType             = l_Fields[2].GetUInt8();
        uint8   l_ObjectiveIndex            = l_Fields[3].GetUInt8();
        uint32  l_ObjectiveObjectID         = l_Fields[4].GetUInt32();
        int32   l_ObjectiveAmount           = l_Fields[5].GetInt32();
        uint32  l_ObjectiveFlags            = l_Fields[6].GetUInt32();
        float   l_ObjectiveUnkFloat         = l_Fields[7].GetFloat();
        std::string l_ObjectiveDescription  = l_Fields[8].GetString();
        std::string l_ObjectiveVisualEffects= l_Fields[9].GetString();

        if (_questTemplates.find(l_ObjectiveQuestId) == _questTemplates.end())
        {
            sLog->outError(LOG_FILTER_SQL, "Quest Objective %u has non existant Quest Id %u! Skipping.", l_ObjectiveID, l_ObjectiveQuestId);
            m_IgnoredQuestObjectives.push_back(l_ObjectiveID);
            continue;
        }

        if (DisableMgr::IsDisabledFor(DISABLE_TYPE_QUEST, l_ObjectiveQuestId, NULL))
        {
            m_IgnoredQuestObjectives.push_back(l_ObjectiveID);
            continue;
        }

        if (l_ObjectiveType >= QUEST_OBJECTIVE_TYPE_END)
        {
            sLog->outError(LOG_FILTER_SQL, "Quest Objective %u has invalid type %u! Skipping.", l_ObjectiveID, l_ObjectiveType);
            m_IgnoredQuestObjectives.push_back(l_ObjectiveID);
            continue;
        }

        Quest* l_Quest = _questTemplates.find(l_ObjectiveQuestId)->second;

        switch (l_ObjectiveType)
        {
            case QUEST_OBJECTIVE_TYPE_NPC:
            case QUEST_OBJECTIVE_TYPE_NPC_INTERACT:
            case QUEST_OBJECTIVE_TYPE_PET_BATTLE_TAMER:
            {
                if (!GetCreatureTemplate(l_ObjectiveObjectID))
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest Objective %u has non existant Creature Id %u! Skipping.", l_ObjectiveID, l_ObjectiveObjectID);
                    continue;
                }

                if (l_ObjectiveAmount <= 0)
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest Objective %u has valid Creature Id %u but amount %u is invalid! Skipping.", l_ObjectiveID, l_ObjectiveObjectID, l_ObjectiveAmount);
                    m_IgnoredQuestObjectives.push_back(l_ObjectiveID);
                    continue;
                }

                break;
            }
            case QUEST_OBJECTIVE_TYPE_ITEM:
            {
                if (!GetItemTemplate(l_ObjectiveObjectID))
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest Objective %u has non existant Item Id %u! Skipping.", l_ObjectiveID, l_ObjectiveObjectID);
                    m_IgnoredQuestObjectives.push_back(l_ObjectiveID);
                    continue;
                }

                if (l_ObjectiveAmount <= 0)
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest Objective %u has valid Item Id %u but amount %u is invalid! Skipping.", l_ObjectiveID, l_ObjectiveObjectID, l_ObjectiveAmount);
                    m_IgnoredQuestObjectives.push_back(l_ObjectiveID);
                    continue;
                }

                break;
            }
            case QUEST_OBJECTIVE_TYPE_GO:
            {
                if (!GetGameObjectTemplate(l_ObjectiveObjectID))
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest Objective %u has non existant GameObject Id %u! Skipping.", l_ObjectiveID, l_ObjectiveObjectID);
                    m_IgnoredQuestObjectives.push_back(l_ObjectiveID);
                    continue;
                }

                if (l_ObjectiveAmount <= 0)
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest Objective %u has valid GameObject Id %u but amount %u is invalid! Skipping.", l_ObjectiveID, l_ObjectiveObjectID, l_ObjectiveAmount);
                    m_IgnoredQuestObjectives.push_back(l_ObjectiveID);
                    continue;
                }

                break;
            }
            case QUEST_OBJECTIVE_TYPE_CURRENCY:
            {
                if (!sCurrencyTypesStore.LookupEntry(l_ObjectiveObjectID))
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest Objective %u has non existant Currency Id %u! Skipping.", l_ObjectiveID, l_ObjectiveObjectID);
                    m_IgnoredQuestObjectives.push_back(l_ObjectiveID);
                    continue;
                }

                break;
            }
            case QUEST_OBJECTIVE_TYPE_SPELL:
            {
                const SpellInfo* l_Spell = sSpellMgr->GetSpellInfo(l_ObjectiveObjectID);

                if (!l_Spell)
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest Objective %u has non existant Spell Id %u! Skipping.", l_ObjectiveID, l_ObjectiveObjectID);
                    continue;
                }

                else if (!SpellMgr::IsSpellValid(l_Spell))
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest Objective %u has invalid Spell Id %u! Skipping.", l_ObjectiveID, l_ObjectiveObjectID);
                    m_IgnoredQuestObjectives.push_back(l_ObjectiveID);
                    continue;
                }

                if (l_ObjectiveAmount <= 0)
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest Objective %u has valid Spell Id %u but amount %u is invalid! Skipping.", l_ObjectiveID, l_ObjectiveObjectID, l_ObjectiveAmount);
                    m_IgnoredQuestObjectives.push_back(l_ObjectiveID);
                    continue;
                }

                break;
            }
            case QUEST_OBJECTIVE_TYPE_FACTION_REP:
            case QUEST_OBJECTIVE_TYPE_FACTION_REP2:
            {
                if (!sFactionStore.LookupEntry(l_ObjectiveObjectID))
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest Objective %u has non existant Faction Id %u! Skipping.", l_ObjectiveID, l_ObjectiveObjectID);
                    m_IgnoredQuestObjectives.push_back(l_ObjectiveID);
                    continue;
                }

                break;
            }
            case QUEST_OBJECTIVE_TYPE_MONEY:
            {
                if (l_Quest->GetQuestObjectiveCountType(l_ObjectiveType) >= 1)
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest Objective %u is invalid, Quest %u already has the max amount of Quest Objective type %u! Skipping.", l_ObjectiveID, l_ObjectiveQuestId, l_ObjectiveType);
                    m_IgnoredQuestObjectives.push_back(l_ObjectiveID);
                    continue;
                }

                break;
            }
            case QUEST_OBJECTIVE_TYPE_PET_BATTLE_ELITE:
            {
                if (!sBattlePetSpeciesStore.LookupEntry(l_ObjectiveObjectID))
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest Objective %u has non existant Battle Pet Species %u! Skipping.", l_ObjectiveID, l_ObjectiveObjectID);
                    m_IgnoredQuestObjectives.push_back(l_ObjectiveID);
                    continue;
                }

                ///if (!BattlePetSpeciesHasFlag(objectId, BATTLE_PET_FLAG_ELITE))
                ///{
                ///    sLog->outError(LOG_FILTER_SQL, "Quest Objective %u has invalid Battle Pet Species %u, it doesn't have BATTLE_PET_FLAG_ELITE flag! Skipping.", id, objectId);
                ///    m_IgnoredQuestObjectives.push_back(l_ObjectiveID);
                ///    continue;
                ///}

                if (l_ObjectiveAmount <= 0)
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest Objective %u has valid Item Id %u but amount %u is invalid! Skipping.", l_ObjectiveID, l_ObjectiveObjectID, l_ObjectiveAmount);
                    m_IgnoredQuestObjectives.push_back(l_ObjectiveID);
                    continue;
                }

                break;
            }
            case QUEST_OBJECTIVE_TYPE_PET_BATTLE_PVP:
            {
                if (l_Quest->GetQuestObjectiveCountType(l_ObjectiveType) >= 1)
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest Objective %u is invalid, Quest %u already has the max amount of Quest Objective type %u! Skipping.", l_ObjectiveID, l_ObjectiveQuestId, l_ObjectiveType);
                    m_IgnoredQuestObjectives.push_back(l_ObjectiveID);
                    continue;
                }

                if (l_ObjectiveAmount <= 0)
                {
                    sLog->outError(LOG_FILTER_SQL, "Quest Objective %u has invalid Pet Battle PvP win amount %u! Skipping.", l_ObjectiveID, l_ObjectiveObjectID, l_ObjectiveAmount); ///< Data argument not used by format string
                    m_IgnoredQuestObjectives.push_back(l_ObjectiveID);
                    continue;
                }

                break;
            }
            default:
                break;
        }

        if (m_questObjectiveLookup.find(l_ObjectiveID) == m_questObjectiveLookup.end())
            m_questObjectiveLookup.insert(std::make_pair(l_ObjectiveID, l_ObjectiveQuestId));

        QuestObjective l_Objective;
        l_Objective.ID          = l_ObjectiveID;
        l_Objective.QuestID     = l_ObjectiveQuestId;
        l_Objective.Type        = l_ObjectiveType;
        l_Objective.Index       = l_ObjectiveIndex;
        l_Objective.ObjectID    = l_ObjectiveObjectID;
        l_Objective.Amount      = l_ObjectiveAmount;
        l_Objective.Flags       = l_ObjectiveFlags;
        l_Objective.UnkFloat    = l_ObjectiveUnkFloat;
        l_Objective.Description = l_ObjectiveDescription;

        if (!l_ObjectiveVisualEffects.empty())
        {
            Tokenizer l_Specializations(l_ObjectiveVisualEffects, ' ');

            for (Tokenizer::const_iterator l_It = l_Specializations.begin(); l_It != l_Specializations.end(); ++l_It)
                l_Objective.VisualEffects.push_back(atol(*l_It));
        }

        l_Quest->QuestObjectives.push_back(l_Objective);
        m_QuestObjectiveByType[l_ObjectiveType].push_back(l_Objective);
        l_Quest->QuestObjecitveTypeCount[l_ObjectiveType]++;

        l_Count++;
    } while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Quest Objectives in %u ms.", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

void ObjectMgr::LoadQuestObjectiveLocales()
{
    uint32 l_OldMSTime = getMSTime();

    QueryResult l_Result = WorldDatabase.Query("SELECT `ID`, `Locale`, `Description` FROM `locales_quest_template_objective` ORDER BY `id` ASC");

    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 Quest Objective locale descriptions. DB table `locales_quest_objective` is empty.");
        return;
    }

    uint32 l_Count = 0;
    do
    {
        Field* l_Fields = l_Result->Fetch();

        uint32 l_ObjectiveID = l_Fields[0].GetUInt32();
        uint8 l_Locale = l_Fields[1].GetUInt8();
        std::string l_Description = l_Fields[2].GetString();

        if (std::find(m_IgnoredQuestObjectives.begin(), m_IgnoredQuestObjectives.end(), l_ObjectiveID) != m_IgnoredQuestObjectives.end())
            continue;

        if (!QuestObjectiveExists(l_ObjectiveID))
        {
            sLog->outError(LOG_FILTER_SQL, "Quest Objective locale Id %u has invalid Quest Objective %u! Skipping.", l_Locale, l_ObjectiveID);
            continue;
        }

        if (l_Locale >= TOTAL_LOCALES)
        {
            sLog->outError(LOG_FILTER_SQL, "Locale Id %u for Quest Objective %u is invalid! Skipping.", l_Locale, l_ObjectiveID);
            continue;
        }

        AddLocaleString(l_Description, (LocaleConstant)l_Locale, m_questObjectiveLocaleStore[l_ObjectiveID].Description);
    } while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Quest Objective visual effects in %u ms.", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

void ObjectMgr::LoadFollowerQuests()
{
    const ObjectMgr::QuestMap & l_QuestTemplates = GetQuestTemplates();
    for (ObjectMgr::QuestMap::const_iterator l_It = l_QuestTemplates.begin(); l_It != l_QuestTemplates.end(); ++l_It)
    {
        Quest* l_Quest = l_It->second;

        uint32 l_SpellID = l_Quest->RewardSpellCast;

        if (!l_SpellID)
            continue;

        const SpellInfo* l_Info = sSpellMgr->GetSpellInfo(l_SpellID);

        if (!l_Info)
            continue;

        if (l_Info->Effects[EFFECT_0].Effect != SPELL_EFFECT_ADD_GARRISON_FOLLOWER)
            continue;

        FollowerQuests.push_back(l_Quest->Id);
    }
}

void ObjectMgr::LoadQuestForItem()
{
    const ObjectMgr::QuestMap & l_QuestTemplates = GetQuestTemplates();
    for (ObjectMgr::QuestMap::const_iterator l_It = l_QuestTemplates.begin(); l_It != l_QuestTemplates.end(); ++l_It)
    {
        Quest* l_Quest = l_It->second;

        for (auto l_Objective : l_Quest->QuestObjectives)
        {
            if (l_Objective.Type != QUEST_OBJECTIVE_TYPE_ITEM)
                continue;

            QuestForItem[l_Objective.ObjectID].push_back(std::pair<uint32, uint8>(l_Quest->Id, l_Objective.Index));
        }
    }
}

QuestObjectiveLocale const* ObjectMgr::GetQuestObjectiveLocale(uint32 objectiveId) const
{
    QuestObjectiveLocaleContainer::const_iterator l_It = m_questObjectiveLocaleStore.find(objectiveId);

    if (l_It == m_questObjectiveLocaleStore.end())
        return NULL;

    return &l_It->second;
}

bool ObjectMgr::QuestObjectiveExists(uint32 objectiveId) const
{
    if (m_questObjectiveLookup.find(objectiveId) == m_questObjectiveLookup.end())
        return false;

    return true;
}

uint32 ObjectMgr::GetQuestObjectiveQuestId(uint32 objectiveId) const
{
    QuestObjectiveLookupMap::const_iterator l_It = m_questObjectiveLookup.find(objectiveId);

    if (l_It == m_questObjectiveLookup.end())
        return 0;

    return l_It->second;
}

CreatureGroupSizeStat const* ObjectMgr::GetCreatureGroupSizeStat(uint32 p_Entry, uint32 p_Difficulty) const
{
    auto l_DifficultyIterator = m_CreatureGroupSizeStore.find(p_Entry);
    if (l_DifficultyIterator == m_CreatureGroupSizeStore.end())
        return nullptr;

    auto l_StatsIterator = l_DifficultyIterator->second.find(p_Difficulty);
    if (l_StatsIterator == l_DifficultyIterator->second.end())
        return nullptr;

    return &l_StatsIterator->second;
}

void ObjectMgr::LoadItemBonusGroup()
{
    uint32 l_OldMSTime = getMSTime();

    QueryResult l_Result = WorldDatabase.Query("SELECT id, bonus FROM item_bonus_group");

    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 Item Bonus Group. DB table `item_bonus_group` is empty.");
        return;
    }

    uint32 l_Count = 0;
    do
    {
        Field* l_Fields  = l_Result->Fetch();
        uint32 l_GroupID = l_Fields[0].GetUInt32();

        ItemBonus::GroupContainer l_ItemBonusGroup;
        Tokenizer l_Tokens(l_Fields[1].GetCString(), ',');

        for (uint32 l_I = 0; l_I < l_Tokens.size(); ++l_I)
            l_ItemBonusGroup.push_back((uint32)atoi(l_Tokens[l_I]));

        m_ItemBonusGroupStore.insert(std::make_pair(l_GroupID, l_ItemBonusGroup));

        l_Count++;
    } 
    while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u item bonus group in %u ms.", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

void ObjectMgr::LoadItemBonusGroupLinked()
{
    uint32 l_OldMSTime = getMSTime();

    QueryResult l_Result = WorldDatabase.Query("SELECT itemEntry, itemBonusGroup FROM item_bonus_group_linked");

    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 Item Bonus Group Linked. DB table `item_bonus_group_linked` is empty.");
        return;
    }

    uint32 l_Count = 0;
    do
    {
        Field* l_Fields = l_Result->Fetch();
        uint32 l_ItemEntry      = l_Fields[0].GetUInt32();
        uint32 l_ItemBonusGroup = l_Fields[1].GetUInt32();

        if (_itemTemplateStore.find(l_ItemEntry) == _itemTemplateStore.end())
            continue;

        ItemTemplate& l_ItemTemplate = _itemTemplateStore[l_ItemEntry];
        l_ItemTemplate.m_ItemBonusGroups.push_back(l_ItemBonusGroup);

        l_Count++;
    }
    while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u item bonus group linked in %u ms.", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

void ObjectMgr::LoadSpellInvalid()
{
    uint32 l_OldMSTime = getMSTime();
    m_SpellInvalid.clear();

    QueryResult l_Result = WorldDatabase.Query("SELECT spellid FROM spell_invalid");

    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 Spell Invalid. DB table `spell_invalid` is empty.");
        return;
    }

    uint32 l_Count = 0;
    do
    {
        Field* l_Fields    = l_Result->Fetch();
        uint32 l_SpellID   = l_Fields[0].GetUInt32();

        m_SpellInvalid.push_back(l_SpellID);

        l_Count++;
    }
    while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Spell Invalid in %u ms.", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

void ObjectMgr::LoadSpellStolen()
{
    uint32 l_OldMSTime = getMSTime();
    m_SpellStolen.clear();

    QueryResult l_Result = WorldDatabase.Query("SELECT spell_id FROM spell_stolen");

    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 Spell Stolen. DB table `spell_stolen` is empty.");
        return;
    }

    uint32 l_Count = 0;
    do
    {
        Field* l_Fields = l_Result->Fetch();
        uint32 l_SpellID = l_Fields[0].GetUInt32();

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(l_SpellID);
        if (!spellInfo)
        {
            sLog->outError(LOG_FILTER_SQL, "SpellStolen: spell (spell_id:%d) does not exist in `Spell.dbc`.", l_SpellID);
            continue;
        }

        m_SpellStolen.push_back(l_SpellID);

        l_Count++;
    } while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Spell Stolen in %u ms.", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

void ObjectMgr::LoadDisabledEncounters()
{
    uint32 l_OldMSTime = getMSTime();

    m_DisabledEncounters.clear();

    QueryResult l_Result = WorldDatabase.Query("SELECT EncounterID, DifficultyID FROM instance_disabled_rankings");

    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 disabled ranking. DB table `instance_disabled_rankings` is empty.");
        return;
    }

    uint32 l_Count = 0;
    do
    {
        Field* l_Fields         = l_Result->Fetch();
        uint32 l_EncounterID    = l_Fields[0].GetUInt32();
        uint32 l_DifficultyID   = l_Fields[1].GetUInt32();

        m_DisabledEncounters.insert(std::make_pair(l_EncounterID, l_DifficultyID));

        l_Count++;
    }
    while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u disabled ranking in %u ms.", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}
