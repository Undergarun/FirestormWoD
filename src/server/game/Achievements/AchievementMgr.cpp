////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "DBCStructure.h"
#include "ObjectMgr.h"
#include "GuildMgr.h"
#include "World.h"
#include "WorldPacket.h"
#include "AchievementMgr.h"
#include "Arena.h"
#include "CellImpl.h"
#include "GameEventMgr.h"
#include "GridNotifiersImpl.h"
#include "Language.h"
#include "Player.h"
#include "SpellMgr.h"
#include "DisableMgr.h"
#include "ScriptMgr.h"
#include "MapManager.h"
#include "Battleground.h"
#include "BattlegroundAB.h"
#include "Map.h"
#include "InstanceScript.h"
#include "Group.h"
#include "Chat.h"
#include "WowTime.hpp"

#ifndef CROSS
# include "Guild.h"
#endif

namespace JadeCore
{
    class AchievementChatBuilder
    {
        public:
            AchievementChatBuilder(Player const& p_Player, ChatMsg p_MsgType, int32 p_TextID, uint32 p_AchievementID)
                : m_Player(p_Player), m_MsgType(p_MsgType), m_TextID(p_TextID), m_AchievementID(p_AchievementID) { }
            void operator()(WorldPacket& p_Data, LocaleConstant p_LocIDX)
            {
                char const* l_Text = sObjectMgr->GetTrinityString(m_TextID, p_LocIDX);

                ChatHandler::FillMessageData(&p_Data, m_Player.GetSession(), m_MsgType, LANG_UNIVERSAL, NULL, m_Player.GetGUID(), l_Text, NULL, NULL, m_AchievementID);
            }

        private:
            Player const& m_Player;
            ChatMsg m_MsgType;
            int32 m_TextID;
            uint32 m_AchievementID;
    };
}                                                           // namespace JadeCore

bool AchievementCriteriaData::IsValid(CriteriaEntry const* p_Criteria)
{
    if (dataType >= MAX_ACHIEVEMENT_CRITERIA_DATA_TYPE)
    {
        sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` for criteria (Entry: %u) has wrong data type (%u), ignored.", p_Criteria->ID, dataType);
        return false;
    }

    switch (p_Criteria->Type)
    {
        case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE:
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_BG:
        case ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING:
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:          // Only hard coded list
        case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL:
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA:
        case ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE:
        case ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL:
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL:
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE:
        case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2:
        case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET:
        case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2:
        case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM:
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT:
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT:
        case ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE:
        case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL:
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST:    // Only Children's Week achievements
        case ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM:                // Only Children's Week achievements
        case ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS:
        case ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL:
        case ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLEPET:
        case ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_SPECIFIC_BATTLEPET:
        case ACHIEVEMENT_CRITERIA_TYPE_EARN_BATTLEPET:
        case ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLEPET_IN_COMBAT:
        case ACHIEVEMENT_CRITERIA_TYPE_LEVELUP_BATTLEPET:
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_PETBATTLE:
            break;
        default:
            if (dataType != ACHIEVEMENT_CRITERIA_DATA_TYPE_SCRIPT)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` has data for non-supported criteria type (Entry: %u Type: %u), ignored.", p_Criteria->ID, p_Criteria->Type);
                return false;
            }
            break;
    }

    switch (dataType)
    {
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_NONE:
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_VALUE:
        case ACHIEVEMENT_CRITERIA_DATA_INSTANCE_SCRIPT:
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_CREATURE:
            if (!creature.id || !sObjectMgr->GetCreatureTemplate(creature.id))
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_CREATURE (%u) has non-existing creature id in value1 (%u), ignored.",
                    p_Criteria->ID, p_Criteria->Type, dataType, creature.id);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_CLASS_RACE:
            if (!classRace.class_id && !classRace.race_id)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_CLASS_RACE (%u) must not have 0 in either value field, ignored.",
                    p_Criteria->ID, p_Criteria->Type, dataType);
                return false;
            }
            if (classRace.class_id && ((1 << (classRace.class_id-1)) & CLASSMASK_ALL_PLAYABLE) == 0)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_CLASS_RACE (%u) has non-existing class in value1 (%u), ignored.",
                    p_Criteria->ID, p_Criteria->Type, dataType, classRace.class_id);
                return false;
            }
            if (classRace.race_id && ((1 << (classRace.race_id-1)) & RACEMASK_ALL_PLAYABLE) == 0)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_CLASS_RACE (%u) has non-existing race in value2 (%u), ignored.",
                    p_Criteria->ID, p_Criteria->Type, dataType, classRace.race_id);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_LESS_HEALTH:
            if (health.percent < 1 || health.percent > 100)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_PLAYER_LESS_HEALTH (%u) has wrong percent value in value1 (%u), ignored.",
                    p_Criteria->ID, p_Criteria->Type, dataType, health.percent);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA:
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_AURA:
        {
            SpellInfo const* spellEntry = sSpellMgr->GetSpellInfo(aura.spell_id);
            if (!spellEntry)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` (Entry: %u Type: %u) for data type %s (%u) has wrong spell id in value1 (%u), ignored.",
                    p_Criteria->ID, p_Criteria->Type, (dataType == ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA ? "ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA" : "ACHIEVEMENT_CRITERIA_DATA_TYPE_T_AURA"), dataType, aura.spell_id);
                return false;
            }
            if (aura.effect_idx >= 3)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` (Entry: %u Type: %u) for data type %s (%u) has wrong spell effect index in value2 (%u), ignored.",
                    p_Criteria->ID, p_Criteria->Type, (dataType == ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA ? "ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA" : "ACHIEVEMENT_CRITERIA_DATA_TYPE_T_AURA"), dataType, aura.effect_idx);
                return false;
            }
            if (!spellEntry->Effects[aura.effect_idx].ApplyAuraName)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` (Entry: %u Type: %u) for data type %s (%u) has non-aura spell effect (ID: %u Effect: %u), ignores.",
                    p_Criteria->ID, p_Criteria->Type, (dataType == ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA ? "ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA" : "ACHIEVEMENT_CRITERIA_DATA_TYPE_T_AURA"), dataType, aura.spell_id, aura.effect_idx);
                return false;
            }
            return true;
        }
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_LEVEL:
            if (level.minlevel > STRONG_MAX_LEVEL)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_T_LEVEL (%u) has wrong minlevel in value1 (%u), ignored.",
                    p_Criteria->ID, p_Criteria->Type, dataType, level.minlevel);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_GENDER:
            if (gender.gender > GENDER_NONE)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_T_GENDER (%u) has wrong gender in value1 (%u), ignored.",
                    p_Criteria->ID, p_Criteria->Type, dataType, gender.gender);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_SCRIPT:
            if (!ScriptId)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_SCRIPT (%u) does not have ScriptName set, ignored.",
                    p_Criteria->ID, p_Criteria->Type, dataType);
                return false;
            }
            return true;
        /*
        @Todo:
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_DIFFICULTY:
        */
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_PLAYER_COUNT:
            if (map_players.maxcount <= 0)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_PLAYER_COUNT (%u) has wrong max players count in value1 (%u), ignored.",
                    p_Criteria->ID, p_Criteria->Type, dataType, map_players.maxcount);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_TEAM:
            if (team.team != ALLIANCE && team.team != HORDE)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_T_TEAM (%u) has unknown team in value1 (%u), ignored.",
                    p_Criteria->ID, p_Criteria->Type, dataType, team.team);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_S_DRUNK:
            if (drunk.state >= MAX_DRUNKEN)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_S_DRUNK (%u) has unknown drunken state in value1 (%u), ignored.",
                    p_Criteria->ID, p_Criteria->Type, dataType, drunk.state);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_HOLIDAY:
            if (!sHolidaysStore.LookupEntry(holiday.id))
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_HOLIDAY (%u) has unknown holiday in value1 (%u), ignored.",
                    p_Criteria->ID, p_Criteria->Type, dataType, holiday.id);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_BG_LOSS_TEAM_SCORE:
            return true;                                    // Not check correctness node indexes
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_S_EQUIPED_ITEM:
            if (equipped_item.item_quality >= MAX_ITEM_QUALITY)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_requirement` (Entry: %u Type: %u) for requirement ACHIEVEMENT_CRITERIA_REQUIRE_S_EQUIPED_ITEM (%u) has unknown quality state in value1 (%u), ignored.",
                    p_Criteria->ID, p_Criteria->Type, dataType, equipped_item.item_quality);
                return false;
            }
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_S_PLAYER_CLASS_RACE:
            if (!classRace.class_id && !classRace.race_id)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_S_PLAYER_CLASS_RACE (%u) must not have 0 in either value field, ignored.",
                    p_Criteria->ID, p_Criteria->Type, dataType);
                return false;
            }
            if (classRace.class_id && ((1 << (classRace.class_id-1)) & CLASSMASK_ALL_PLAYABLE) == 0)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_S_PLAYER_CLASS_RACE (%u) has non-existing class in value1 (%u), ignored.",
                    p_Criteria->ID, p_Criteria->Type, dataType, classRace.class_id);
                return false;
            }
            if (classRace.race_id && ((1 << (classRace.race_id-1)) & RACEMASK_ALL_PLAYABLE) == 0)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` (Entry: %u Type: %u) for data type ACHIEVEMENT_CRITERIA_DATA_TYPE_S_PLAYER_CLASS_RACE (%u) has non-existing race in value2 (%u), ignored.",
                    p_Criteria->ID, p_Criteria->Type, dataType, classRace.race_id);
                return false;
            }
            return true;
        default:
            sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` (Entry: %u Type: %u) has data for non-supported data type (%u), ignored.", p_Criteria->ID, p_Criteria->Type, dataType);
            return false;
    }
}

bool AchievementCriteriaData::Meets(uint32 p_CriteriaID, Player const* p_Source, Unit const* p_Target, uint64 p_MiscValue1 /*= 0*/) const
{
    switch (dataType)
    {
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_NONE:
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_CREATURE:
            if (!p_Target || p_Target->GetTypeId() != TYPEID_UNIT)
                return false;
            return p_Target->GetEntry() == creature.id;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_CLASS_RACE:
            if (!p_Target || p_Target->GetTypeId() != TYPEID_PLAYER)
                return false;
            if (classRace.class_id && classRace.class_id != p_Target->ToPlayer()->getClass())
                return false;
            if (classRace.race_id && classRace.race_id != p_Target->ToPlayer()->getRace())
                return false;
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_S_PLAYER_CLASS_RACE:
            if (!p_Source || p_Source->GetTypeId() != TYPEID_PLAYER)
                return false;
            if (classRace.class_id && classRace.class_id != p_Source->ToPlayer()->getClass())
                return false;
            if (classRace.race_id && classRace.race_id != p_Source->ToPlayer()->getRace())
                return false;
            return true;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_PLAYER_LESS_HEALTH:
            if (!p_Target || p_Target->GetTypeId() != TYPEID_PLAYER)
                return false;
            return !p_Target->HealthAbovePct(health.percent);
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_S_AURA:
            return p_Source->HasAuraEffect(aura.spell_id, aura.effect_idx);
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_AURA:
            return p_Target && p_Target->HasAuraEffect(aura.spell_id, aura.effect_idx);
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_VALUE:
           return p_MiscValue1 >= value.minvalue;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_LEVEL:
            if (!p_Target)
                return false;
            return p_Target->getLevel() >= level.minlevel;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_GENDER:
            if (!p_Target)
                return false;
            return p_Target->getGender() == gender.gender;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_SCRIPT:
            return sScriptMgr->OnCriteriaCheck(ScriptId, const_cast<Player*>(p_Source), const_cast<Unit*>(p_Target));
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_MAP_PLAYER_COUNT:
            return p_Source->GetMap()->GetPlayersCountExceptGMs() <= map_players.maxcount;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_T_TEAM:
            if (!p_Target || p_Target->GetTypeId() != TYPEID_PLAYER)
                return false;
            return p_Target->ToPlayer()->GetTeam() == team.team;
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_S_DRUNK:
            return Player::GetDrunkenstateByValue(p_Source->GetDrunkValue()) >= DrunkenState(drunk.state);
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_HOLIDAY:
            return IsHolidayActive(HolidayIds(holiday.id));
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_BG_LOSS_TEAM_SCORE:
        {
            Battleground* bg = p_Source->GetBattleground();
            if (!bg)
                return false;

            uint16 winnerTeamScore = 0;
            switch(bg->GetTypeID(true))
            {
                case BATTLEGROUND_WS:
                    winnerTeamScore = 3;
                    break;
                case BATTLEGROUND_AB:
                case BATTLEGROUND_EY:
                    winnerTeamScore = 1600;
                    break;
                default:
                    break;
            }
            if (winnerTeamScore > 0 && !bg->IsTeamScoreInRange(p_Source->GetTeam(), winnerTeamScore, winnerTeamScore))
                return false;

            return bg->IsTeamScoreInRange(p_Source->GetTeam() == ALLIANCE ? HORDE : ALLIANCE, bg_loss_team_score.min_score, bg_loss_team_score.max_score);
        }
        case ACHIEVEMENT_CRITERIA_DATA_INSTANCE_SCRIPT:
        {
            if (!p_Source->IsInWorld())
                return false;
            Map* map = p_Source->GetMap();
            if (!map->IsDungeon())
            {
                sLog->outError(LOG_FILTER_ACHIEVEMENTSYS, "Achievement system call ACHIEVEMENT_CRITERIA_DATA_INSTANCE_SCRIPT (%u) for achievement criteria %u for non-dungeon/non-raid map %u",
                    ACHIEVEMENT_CRITERIA_DATA_INSTANCE_SCRIPT, p_CriteriaID, map->GetId());
                    return false;
            }
            InstanceScript* instance = ((InstanceMap*)map)->GetInstanceScript();
            if (!instance)
            {
                sLog->outError(LOG_FILTER_ACHIEVEMENTSYS, "Achievement system call ACHIEVEMENT_CRITERIA_DATA_INSTANCE_SCRIPT (%u) for achievement criteria %u for map %u but map does not have a instance script",
                    ACHIEVEMENT_CRITERIA_DATA_INSTANCE_SCRIPT, p_CriteriaID, map->GetId());
                return false;
            }
            return instance->CheckAchievementCriteriaMeet(p_CriteriaID, p_Source, p_Target, p_MiscValue1);
        }
        case ACHIEVEMENT_CRITERIA_DATA_TYPE_S_EQUIPED_ITEM:
        {
            ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(p_MiscValue1);
            if (!pProto)
                return false;
            return pProto->ItemLevel >= equipped_item.item_level && pProto->Quality >= equipped_item.item_quality;
        }
        default:
            break;
    }
    return false;
}

bool AchievementCriteriaDataSet::Meets(Player const* p_Source, Unit const* p_Target, uint64 p_MiscValue /*= 0*/) const
{
    for (Storage::const_iterator itr = storage.begin(); itr != storage.end(); ++itr)
        if (!itr->Meets(criteria_id, p_Source, p_Target, p_MiscValue))
            return false;

    return true;
}

template<class T>
AchievementMgr<T>::AchievementMgr(T* owner) : _owner(owner), _achievementPoints(0), m_NeedDBSync(false)
{
}

template<class T>
AchievementMgr<T>::~AchievementMgr()
{
}

template<class T>
void AchievementMgr<T>::SendPacket(WorldPacket* /*data*/) const
{
}

template<>
#ifndef CROSS
void AchievementMgr<Guild>::SendPacket(WorldPacket* data) const
{
    GetOwner()->BroadcastPacket(data);
}

template<>
#endif

void AchievementMgr<Player>::SendPacket(WorldPacket* data) const
{
    GetOwner()->GetSession()->SendPacket(data);
}

template<class T>
void AchievementMgr<T>::RemoveCriteriaProgress(CriteriaEntry const* p_Entry)
{
    CriteriaProgressMap* l_ProgressMap = GetCriteriaProgressMap();
    if (!l_ProgressMap)
        return;

    CriteriaProgressMap::iterator l_CriteriaProgress = l_ProgressMap->find(p_Entry->ID);
    if (l_CriteriaProgress == l_ProgressMap->end())
        return;

    WorldPacket l_Data(SMSG_CRITERIA_DELETED, 4);
    l_Data << uint32(p_Entry->ID);
    SendPacket(&l_Data);

    l_ProgressMap->erase(l_CriteriaProgress);
}

#ifndef CROSS
template<>
void AchievementMgr<Guild>::RemoveCriteriaProgress(CriteriaEntry const* p_Entry)
{
    CriteriaProgressMap::iterator l_CriteriaProgress = GetCriteriaProgressMap()->find(p_Entry->ID);

    if (l_CriteriaProgress == GetCriteriaProgressMap()->end())
        return;

    WorldPacket l_Data(SMSG_GUILD_CRITERIA_DELETED, 4 + 8);
    l_Data.appendPackGUID(GetOwner()->GetGUID());
    l_Data << uint32(p_Entry->ID);

    SendPacket(&l_Data);
    GetCriteriaProgressMap()->erase(l_CriteriaProgress);
    m_NeedDBSync = true;
}
#endif

template<class T>
void AchievementMgr<T>::ResetAchievementCriteria(AchievementCriteriaTypes p_Type, uint64 /*p_MiscValue1*/, uint64 /*p_MiscValue2*/, bool /*p_EvenIfCriteriaComplete*/)
{
    // Disable for game masters with GM-mode enabled
    if (GetOwner()->isGameMaster())
        return;

    AchievementCriteriaEntryList const& l_AchievementCriteriaList = sAchievementMgr->GetAchievementCriteriaByType(p_Type);
    for (AchievementCriteriaEntryList::const_iterator l_Iter = l_AchievementCriteriaList.begin(); l_Iter != l_AchievementCriteriaList.end(); ++l_Iter)
    {
        CriteriaEntry const* l_AchievementCriteria = (*l_Iter);

        // Don't update already completed criteria if not forced or achievement already complete
        if (!IsCompletedCriteria(l_AchievementCriteria))
            RemoveCriteriaProgress(l_AchievementCriteria);
    }
}

#ifndef CROSS
template<>
void AchievementMgr<Guild>::ResetAchievementCriteria(AchievementCriteriaTypes /*type*/, uint64 /*miscValue1*/, uint64 /*miscValue2*/, bool /*evenIfCriteriaComplete*/)
{
    // Not needed
}

template<class T>
void AchievementMgr<T>::DeleteFromDB(uint32 /*lowguid*/, uint32 /*accountId*/)
{
}

template<>
void AchievementMgr<Player>::DeleteFromDB(uint32 p_LowGUID, uint32 /*p_AccountID*/)
{
    SQLTransaction l_Trans = CharacterDatabase.BeginTransaction();

    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_ACHIEVEMENT);
    l_Stmt->setUInt32(0, p_LowGUID);
    l_Trans->Append(l_Stmt);

    CharacterDatabase.CommitTransaction(l_Trans);
}

template<>
void AchievementMgr<Guild>::DeleteFromDB(uint32 p_LowGUID, uint32 /*p_AccountID*/)
{
    SQLTransaction l_Trans = CharacterDatabase.BeginTransaction();

    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ALL_GUILD_ACHIEVEMENTS);
    l_Stmt->setUInt32(0, p_LowGUID);
    l_Trans->Append(l_Stmt);

    l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ALL_GUILD_ACHIEVEMENT_CRITERIA);
    l_Stmt->setUInt32(0, p_LowGUID);
    l_Trans->Append(l_Stmt);

    CharacterDatabase.CommitTransaction(l_Trans);
}
#endif

template<class T>
void AchievementMgr<T>::SaveToDB(SQLTransaction& /*trans*/)
{
}

template<>
void AchievementMgr<Player>::SaveToDB(SQLTransaction& trans)
{
    m_CompletedAchievementsLock.acquire();
    if (!m_completedAchievements.empty())
    {
        bool need_execute = false;

        std::ostringstream ssAccDel;
        std::ostringstream ssAccIns;

        std::ostringstream ssCharDel;
        std::ostringstream ssCharIns;
        std::ostringstream sscount;

        uint32 points = 0;

        for (CompletedAchievementMap::iterator itr =  m_completedAchievements.begin(); itr != m_completedAchievements.end(); ++itr)
            if (AchievementEntry const* pAchievement = sAchievementStore.LookupEntry(itr->first))
                points += pAchievement->Points;

        if (points)
        {
            sscount << "REPLACE INTO character_achievement_count (guid, count) VALUES (" << GetOwner()->GetRealGUIDLow() << "," << points << ");";
            trans->Append(sscount.str().c_str());
        }

        for (CompletedAchievementMap::iterator iter = m_completedAchievements.begin(); iter != m_completedAchievements.end(); ++iter)
        {
            if (!iter->second.changed)
                continue;

            bool mustSaveForCharacter = iter->second.completedByThisCharacter;

            // First new/changed record prefix
            if (!need_execute)
            {
                ssAccDel << "DELETE FROM account_achievement WHERE account = " << GetOwner()->GetSession()->GetAccountId() << " AND achievement IN (";
                ssAccIns << "INSERT INTO account_achievement (account, first_guid, achievement, date) VALUES ";

                if (mustSaveForCharacter)
                {
                    ssCharDel << "DELETE FROM character_achievement WHERE guid = " << GetOwner()->GetRealGUIDLow() << " AND achievement IN (";
                    ssCharIns << "INSERT INTO character_achievement (guid, achievement) VALUES ";
                }
                need_execute = true;
            }
            // Next new/changed record prefix
            else
            {
                ssAccDel << ',';
                ssAccIns << ',';

                if (mustSaveForCharacter)
                {
                    ssCharDel << ',';
                    ssCharIns << ',';
                }
            }

            // New/changed record data
            ssAccDel << iter->first;
            ssAccIns << '(' << GetOwner()->GetSession()->GetAccountId() << ',' << GUID_LOPART(iter->second.first_guid) << ',' << iter->first << ',' << iter->second.date << ')';

            if (mustSaveForCharacter)
            {
                ssCharDel << iter->first;
                ssCharIns << '(' << GetOwner()->GetRealGUIDLow() << ',' << iter->first << ')';
            }

            /// Mark as saved in db
            iter->second.changed = false;
        }

        if (need_execute)
        {
            ssAccDel  << ')';
            ssCharDel << ')';
            trans->Append(ssAccDel.str().c_str());
            trans->Append(ssAccIns.str().c_str());

            trans->Append(ssCharDel.str().c_str());
            trans->Append(ssCharIns.str().c_str());
        }
    }
    
    m_CompletedAchievementsLock.release();

    CriteriaProgressMap* progressMap = GetCriteriaProgressMap();
    if (!progressMap)
        return;

    if (!progressMap->empty())
    {
        // Prepare deleting and insert
        bool need_execute_del       = false;
        bool need_execute_ins       = false;
        bool need_execute_account   = false;

        bool isAccountAchievement   = false;

        bool alreadyOneCharDelLine  = false;
        bool alreadyOneAccDelLine   = false;
        bool alreadyOneCharInsLine  = false;
        bool alreadyOneAccInsLine   = false;

        std::ostringstream ssAccdel;
        std::ostringstream ssAccins;
        std::ostringstream ssChardel;
        std::ostringstream ssCharins;

        uint64 guid = GetOwner()->GetRealGUIDLow();

        uint32 accountId = GetOwner()->GetSession()->GetAccountId();

        for (CriteriaProgressMap::iterator iter = progressMap->begin(); iter != progressMap->end(); ++iter)
        {
            if (!iter->second.changed)
                continue;

            CriteriaEntry const* criteria = sAchievementMgr->GetAchievementCriteria(iter->first);
            if (!criteria)
                continue;

            AchievementEntry const* l_Achievement = nullptr;
            AchievementCriteriaTreeList const& criteriaTreeList = sAchievementMgr->GetAchievementCriteriaTreeList(criteria);
            for (AchievementCriteriaTreeList::const_iterator l_Iter = criteriaTreeList.begin(); l_Iter != criteriaTreeList.end(); ++l_Iter)
            {
                CriteriaTreeEntry const* l_CriteriaTree = *l_Iter;
                l_Achievement = sAchievementMgr->GetAchievementEntryByCriteriaTree(l_CriteriaTree);
            }

            if (l_Achievement == nullptr)
                continue;

            if (l_Achievement->Flags & ACHIEVEMENT_FLAG_ACCOUNT)
            {
                isAccountAchievement = true;
                need_execute_account = true;
            }
            else
                isAccountAchievement = false;

            // Deleted data (including 0 progress state)
            {
                // First new/changed record prefix (for any counter value)
                if (!need_execute_del)
                {
                    ssAccdel  << "DELETE FROM account_achievement_progress   WHERE account = " << accountId << " AND criteria IN (";
                    ssChardel << "DELETE FROM character_achievement_progress WHERE guid    = " << guid      << " AND criteria IN (";
                    need_execute_del = true;
                }
                // Next new/changed record prefix
                else
                {
                    if (isAccountAchievement)
                    {
                        if (alreadyOneAccDelLine)
                            ssAccdel  << ',';
                    }
                    else
                    {
                        if (alreadyOneCharDelLine)
                            ssChardel << ',';
                    }
                }

                // New/changed record data
                if (isAccountAchievement)
                {
                    ssAccdel << iter->first;
                    alreadyOneAccDelLine  = true;
                }
                else
                {
                    ssChardel << iter->first;
                    alreadyOneCharDelLine = true;
                }
            }

            // Store data only for real progress
            if (iter->second.counter != 0)
            {
                /// First new/changed record prefix
                if (!need_execute_ins)
                {
                    ssAccins  << "INSERT INTO account_achievement_progress   (account, criteria, counter, date) VALUES ";
                    ssCharins << "INSERT INTO character_achievement_progress (guid,    criteria, counter, date) VALUES ";
                    need_execute_ins = true;
                }
                // Next new/changed record prefix
                else
                {
                    if (isAccountAchievement)
                    {
                        if (alreadyOneAccInsLine)
                            ssAccins  << ',';
                    }
                    else
                    {
                        if (alreadyOneCharInsLine)
                            ssCharins << ',';
                    }
                }

                // New/changed record data
                if (isAccountAchievement)
                {
                    ssAccins  << '(' << accountId << ',' << iter->first << ',' << iter->second.counter << ',' << iter->second.date << ')';
                    alreadyOneAccInsLine  = true;
                }
                else
                {
                    ssCharins << '(' << guid      << ',' << iter->first << ',' << iter->second.counter << ',' << iter->second.date << ')';
                    alreadyOneCharInsLine = true;
                }
            }

            // Mark as updated in db
            iter->second.changed = false;
        }

        if (need_execute_del)                                // DELETE ... IN (.... _)_
        {
            ssAccdel  << ')';
            ssChardel << ')';
        }

        if (need_execute_del || need_execute_ins)
        {
            if (need_execute_del)
            {
                if (need_execute_account && alreadyOneAccDelLine)
                    trans->Append(ssAccdel.str().c_str());

                if (alreadyOneCharDelLine)
                    trans->Append(ssChardel.str().c_str());
            }

            if (need_execute_ins)
            {
                if (need_execute_account && alreadyOneAccInsLine)
                    trans->Append(ssAccins.str().c_str());

                if (alreadyOneCharInsLine)
                    trans->Append(ssCharins.str().c_str());
            }
        }
    }
}

#ifndef CROSS
template<>
void AchievementMgr<Guild>::SaveToDB(SQLTransaction& trans)
{
    if (!m_NeedDBSync)
        return;

    m_NeedDBSync = false;

    PreparedStatement* stmt;
    std::ostringstream guidstr;

    for (CompletedAchievementMap::iterator itr = m_completedAchievements.begin(); itr != m_completedAchievements.end(); ++itr)
    {
        if (!itr->second.changed)
            continue;

        itr->second.changed = false;

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_ACHIEVEMENT);
        stmt->setUInt32(0, GetOwner()->GetId());
        stmt->setUInt16(1, itr->first);
        trans->Append(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GUILD_ACHIEVEMENT);
        stmt->setUInt32(0, GetOwner()->GetId());
        stmt->setUInt16(1, itr->first);
        stmt->setUInt32(2, itr->second.date);
        for (std::set<uint64>::const_iterator gItr = itr->second.guids.begin(); gItr != itr->second.guids.end(); ++gItr)
            guidstr << GUID_LOPART(*gItr) << ',';

        stmt->setString(3, guidstr.str());
        trans->Append(stmt);

        guidstr.str("");
    }

    CriteriaProgressMap* progressMap = GetCriteriaProgressMap();
    if (!progressMap)
        return;

    for (CriteriaProgressMap::iterator itr = progressMap->begin(); itr != progressMap->end(); ++itr)
    {
        if (!itr->second.changed)
            continue;

        itr->second.changed = false;

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_ACHIEVEMENT_CRITERIA);
        stmt->setUInt32(0, GetOwner()->GetId());
        stmt->setUInt16(1, itr->first);
        trans->Append(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GUILD_ACHIEVEMENT_CRITERIA);
        stmt->setUInt32(0, GetOwner()->GetId());
        stmt->setUInt16(1, itr->first);
        stmt->setUInt32(2, itr->second.counter);
        stmt->setUInt32(3, itr->second.date);
        stmt->setUInt32(4, GUID_LOPART(itr->second.CompletedGUID));
        trans->Append(stmt);
    }
}
#endif

template<class T>
void AchievementMgr<T>::LoadFromDB(Player* /*p_Player*/, Guild* /*p_Guild*/, PreparedQueryResult /*achievementResult*/, PreparedQueryResult /*criteriaResult*/, PreparedQueryResult /*achievementAccountResult*/, PreparedQueryResult /*criteriaAccountResult*/)
{
}

template<>
void AchievementMgr<Player>::LoadFromDB(Player* /*p_Player*/, Guild* /*p_Guild*/, PreparedQueryResult achievementResult, PreparedQueryResult criteriaResult, PreparedQueryResult achievementAccountResult, PreparedQueryResult criteriaAccountResult)
{
    if (achievementAccountResult)
    {
        do
        {
            Field* fields = achievementAccountResult->Fetch();
            uint32 first_guid    = fields[0].GetUInt32();
            uint32 achievementid = fields[1].GetUInt16();

            // Must not happen: cleanup at server startup in sAchievementMgr->LoadCompletedAchievements()
            AchievementEntry const* achievement = sAchievementMgr->GetAchievement(achievementid);
            if (!achievement)
                continue;

            if (achievement->Flags & ACHIEVEMENT_FLAG_GUILD)
                continue;

            m_CompletedAchievementsLock.acquire();
            CompletedAchievementData& ca = m_completedAchievements[achievementid];
            ca.date = time_t(fields[2].GetUInt32());
            ca.changed = false;
            ca.first_guid = MAKE_NEW_GUID(first_guid, 0, HIGHGUID_PLAYER);
            ca.completedByThisCharacter = first_guid == GetOwner()->GetGUIDLow();
            m_CompletedAchievementsLock.release();

            _achievementPoints += achievement->Points;

            bool l_CanAddTitle = achievement->Flags & AchievementFlags::ACHIEVEMENT_FLAG_ACCOUNT || GetOwner()->GetGUIDLow() == first_guid;

            // Title achievement rewards are retroactive
            if (l_CanAddTitle)
            {
                if (AchievementReward const* reward = sAchievementMgr->GetAchievementReward(achievement))
                {
                    if (uint32 titleId = reward->titleId[Player::TeamForRace(GetOwner()->getRace()) == ALLIANCE ? 0 : 1])
                    {
                        if (CharTitlesEntry const* titleEntry = sCharTitlesStore.LookupEntry(titleId))
                            GetOwner()->SetTitle(titleEntry);
                    }
                }
            }

        }
        while (achievementAccountResult->NextRow());
    }

    if (criteriaResult)
    {
        time_t now = time(NULL);
        do
        {
            Field* fields = criteriaResult->Fetch();
            uint32 id      = fields[0].GetUInt16();
            uint32 counter = fields[1].GetUInt32();
            time_t date    = time_t(fields[2].GetUInt32());

            CriteriaEntry const* criteria = sAchievementMgr->GetAchievementCriteria(id);
            if (!criteria)
                continue;

            if (criteria->StartTimer && time_t(date + criteria->StartTimer) < now)
                continue;

            AchievementEntry const* l_Achievement = nullptr;
            AchievementCriteriaTreeList const& criteriaTreeList = sAchievementMgr->GetAchievementCriteriaTreeList(criteria);
            for (AchievementCriteriaTreeList::const_iterator l_Iter = criteriaTreeList.begin(); l_Iter != criteriaTreeList.end(); ++l_Iter)
            {
                CriteriaTreeEntry const* l_CriteriaTree = *l_Iter;
                l_Achievement = sAchievementMgr->GetAchievementEntryByCriteriaTree(l_CriteriaTree);
            }

            if (l_Achievement != nullptr && (l_Achievement->Flags & ACHIEVEMENT_FLAG_GUILD))
                continue;

            CriteriaProgressMap* progressMap = GetCriteriaProgressMap();
            if (!progressMap)
                continue;

            CriteriaProgress& progress = (*progressMap)[id];
            progress.counter = counter;
            progress.date    = date;
            progress.changed = false;
        }
        while (criteriaResult->NextRow());
    }

    if (achievementResult)
    {
        do
        {
            Field* fields = achievementResult->Fetch();
            uint32 achievementid = fields[0].GetUInt16();

            // Must not happen: cleanup at server startup in sAchievementMgr->LoadCompletedAchievements()
            AchievementEntry const* achievement = sAchievementMgr->GetAchievement(achievementid);
            if (!achievement)
                continue;
                
            m_CompletedAchievementsLock.acquire();

            // Achievement in character_achievement but not in account_achievement, there is a problem.
            if (m_completedAchievements.find(achievementid) == m_completedAchievements.end())
            {
                m_CompletedAchievementsLock.release();
                continue;
            }

            CompletedAchievementData& ca = m_completedAchievements[achievementid];
            ca.completedByThisCharacter = true;
            
            m_CompletedAchievementsLock.release();
            _achievementPoints += achievement->Points;

            if (AchievementReward const* reward = sAchievementMgr->GetAchievementReward(achievement))
            {
                if (uint32 titleId = reward->titleId[Player::TeamForRace(GetOwner()->getRace()) == ALLIANCE ? 0 : 1])
                {
                    if (CharTitlesEntry const* titleEntry = sCharTitlesStore.LookupEntry(titleId))
                        GetOwner()->SetTitle(titleEntry);
                }

                /// Spell
                if (reward->SpellID)
                    GetOwner()->learnSpell(reward->SpellID, false, false);
            }

        }
        while (achievementResult->NextRow());
    }

    if (criteriaAccountResult)
    {
        time_t now = time(NULL);
        do
        {
            Field* fields = criteriaAccountResult->Fetch();
            uint32 id      = fields[0].GetUInt16();
            uint32 counter = fields[1].GetUInt32();
            time_t date    = time_t(fields[2].GetUInt32());

            CriteriaEntry const* criteria = sAchievementMgr->GetAchievementCriteria(id);
            if (!criteria)
                continue;

            if (criteria->StartTimer && time_t(date + criteria->StartTimer) < now)
                continue;

            CriteriaProgressMap* progressMap = GetCriteriaProgressMap();

            if (!progressMap)
                continue;

            CriteriaProgress& progress = (*progressMap)[id];
            progress.counter = counter;
            progress.date    = date;
            progress.changed = false;
        }
        while (criteriaAccountResult->NextRow());
    }
}

#ifndef CROSS
template<>
void AchievementMgr<Guild>::LoadFromDB(Player* /*p_Player*/, Guild* /*p_Guild*/, PreparedQueryResult achievementResult, PreparedQueryResult criteriaResult, PreparedQueryResult /*achievementAccountResult*/, PreparedQueryResult /*criteriaAccountResult*/)
{
    if (achievementResult)
    {
        do
        {
            Field* fields = achievementResult->Fetch();
            uint32 achievementid = fields[0].GetUInt16();

            // Must not happen: cleanup at server startup in sAchievementMgr->LoadCompletedAchievements()
            AchievementEntry const* achievement = sAchievementStore.LookupEntry(achievementid);
            if (!achievement)
                continue;

            if (!(achievement->Flags & ACHIEVEMENT_FLAG_GUILD))
                continue;

            CompletedAchievementData& ca = m_completedAchievements[achievementid];
            ca.date = time_t(fields[1].GetUInt32());
            Tokenizer guids(fields[2].GetString(), ' ');
            for (uint32 i = 0; i < guids.size(); ++i)
                ca.guids.insert(MAKE_NEW_GUID(atol(guids[i]), 0, HIGHGUID_PLAYER));

            ca.changed = false;
            _achievementPoints += achievement->Points;

        }
        while (achievementResult->NextRow());
    }

    if (criteriaResult)
    {
        time_t now = time(NULL);
        do
        {
            Field* fields = criteriaResult->Fetch();
            uint32 id      = fields[0].GetUInt16();
            uint32 counter = fields[1].GetUInt32();
            time_t date    = time_t(fields[2].GetUInt32());
            uint64 guid    = fields[3].GetUInt32();

            CriteriaEntry const* criteria = sAchievementMgr->GetAchievementCriteria(id);
            if (!criteria)
                continue;

            if (criteria->StartTimer && time_t(date + criteria->StartTimer) < now)
                continue;

            AchievementEntry const* l_Achievement = nullptr;
            AchievementCriteriaTreeList const& criteriaTreeList = sAchievementMgr->GetAchievementCriteriaTreeList(criteria);
            for (AchievementCriteriaTreeList::const_iterator l_Iter = criteriaTreeList.begin(); l_Iter != criteriaTreeList.end(); ++l_Iter)
            {
                CriteriaTreeEntry const* l_CriteriaTree = *l_Iter;
                l_Achievement = sAchievementMgr->GetAchievementEntryByCriteriaTree(l_CriteriaTree);
            }

            if (l_Achievement != nullptr && !(l_Achievement->Flags & ACHIEVEMENT_FLAG_GUILD))
                continue;

            CriteriaProgressMap* progressMap = GetCriteriaProgressMap();
            if (!progressMap)
                continue;

            CriteriaProgress& progress = (*progressMap)[id];
            progress.counter = counter;
            progress.date    = date;
            progress.CompletedGUID = MAKE_NEW_GUID(guid, 0, HIGHGUID_PLAYER);
            progress.changed = false;
        }
        while (criteriaResult->NextRow());
    }
}

template<class T>
void AchievementMgr<T>::Reset()
{
}

template<>
void AchievementMgr<Player>::Reset()
{
    m_CompletedAchievementsLock.acquire();
    for (CompletedAchievementMap::const_iterator iter = m_completedAchievements.begin(); iter != m_completedAchievements.end(); ++iter)
    {
        WorldPacket data(SMSG_ACHIEVEMENT_DELETED, 4);
        data << uint32(iter->first);
        data << uint32(0);
        SendPacket(&data);
    }
    m_CompletedAchievementsLock.release();

    CriteriaProgressMap* criteriaProgress = GetCriteriaProgressMap();

    if (!criteriaProgress)
        return;

    for (CriteriaProgressMap::const_iterator iter = criteriaProgress->begin(); iter != criteriaProgress->end(); ++iter)
    {
        WorldPacket data(SMSG_CRITERIA_DELETED, 4);
        data << uint32(iter->first);
        SendPacket(&data);
    }

    m_CompletedAchievementsLock.acquire();
    m_completedAchievements.clear();
    m_CompletedAchievementsLock.release();

    _achievementPoints = 0;
    criteriaProgress->clear();
    DeleteFromDB(GetOwner()->GetGUIDLow());

    // Re-fill data
    CheckAllAchievementCriteria(GetOwner());
}

template<>
void AchievementMgr<Guild>::Reset()
{
    for (CompletedAchievementMap::const_iterator l_It = m_completedAchievements.begin(); l_It != m_completedAchievements.end(); ++l_It)
    {
        WorldPacket l_Data(SMSG_GUILD_ACHIEVEMENT_DELETED, 4);

        l_Data.appendPackGUID(GetOwner()->GetGUID());
        l_Data << uint32(l_It->first);
        l_Data << uint32(MS::Utilities::WowTime::Encode(l_It->second.date));

        SendPacket(&l_Data);
    }

    CriteriaProgressMap* criteriaProgressMap = GetCriteriaProgressMap();
    if (!criteriaProgressMap)
        return;

    while (!criteriaProgressMap->empty())
    {
        if (CriteriaEntry const* criteria = sAchievementMgr->GetAchievementCriteria(criteriaProgressMap->begin()->first))
            RemoveCriteriaProgress(criteria);
    }

    _achievementPoints = 0;
    m_completedAchievements.clear();
    DeleteFromDB(GetOwner()->GetId());
}

#endif /* not CROSS */
template<class T>
void AchievementMgr<T>::SendAchievementEarned(AchievementEntry const* achievement) const
{
    // Don't send for achievements with ACHIEVEMENT_FLAG_HIDDEN
    if (achievement->Flags & ACHIEVEMENT_FLAG_HIDDEN)
        return;

#ifndef CROSS
    if (Guild* guild = sGuildMgr->GetGuildById(GetOwner()->GetGuildId()))
    {
        JadeCore::AchievementChatBuilder say_builder(*GetOwner(), CHAT_MSG_GUILD_ACHIEVEMENT, LANG_ACHIEVEMENT_EARNED, achievement->ID);
        JadeCore::LocalizedPacketDo<JadeCore::AchievementChatBuilder> say_do(say_builder);
        guild->BroadcastWorker(say_do);
    }
#endif

    if (achievement->Flags & (ACHIEVEMENT_FLAG_REALM_FIRST_KILL | ACHIEVEMENT_FLAG_REALM_FIRST_REACH))
    {
        // Broadcast realm first reached
        std::string l_OwnerName = GetOwner()->GetName() ? GetOwner()->GetName() : "";

        WorldPacket data(SMSG_SERVER_FIRST_ACHIEVEMENT, strlen(GetOwner()->GetName()) + 1 + 8 + 4 + 4);
        data.WriteBits(l_OwnerName.size(), 7);
        data.WriteBit(false);                           ///< GuildAchievement
        data.FlushBits();

        data.appendPackGUID(GetOwner()->GetGUID());
        data << uint32(achievement->ID);
        data.WriteString(l_OwnerName);

        sWorld->SendGlobalMessage(&data);
    }
    // If player is in world he can tell his friends about new achievement
    else if (GetOwner()->IsInWorld())
    {
        CellCoord p = JadeCore::ComputeCellCoord(GetOwner()->GetPositionX(), GetOwner()->GetPositionY());

        Cell cell(p);
        cell.SetNoCreate();

        JadeCore::AchievementChatBuilder say_builder(*GetOwner(), CHAT_MSG_ACHIEVEMENT, LANG_ACHIEVEMENT_EARNED, achievement->ID);
        JadeCore::LocalizedPacketDo<JadeCore::AchievementChatBuilder> say_do(say_builder);
        JadeCore::PlayerDistWorker<JadeCore::LocalizedPacketDo<JadeCore::AchievementChatBuilder> > say_worker(GetOwner(), sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_SAY), say_do);
        TypeContainerVisitor<JadeCore::PlayerDistWorker<JadeCore::LocalizedPacketDo<JadeCore::AchievementChatBuilder> >, WorldTypeMapContainer > message(say_worker);
        cell.Visit(p, message, *GetOwner()->GetMap(), *GetOwner(), sWorld->getFloatConfig(CONFIG_LISTEN_RANGE_SAY));
    }


    ObjectGuid firstPlayerOnAccountGuid = GetOwner()->GetGUID();

    if (HasAccountAchieved(achievement->ID))
        firstPlayerOnAccountGuid = GetFirstAchievedCharacterOnAccount(achievement->ID);

    WorldPacket l_Data(SMSG_ACHIEVEMENT_EARNED);
    l_Data.appendPackGUID(GetOwner()->GetGUID());
    l_Data.appendPackGUID(firstPlayerOnAccountGuid);
    l_Data << uint32(achievement->ID);
    l_Data << uint32(MS::Utilities::WowTime::Encode(time(NULL)));
    l_Data << uint32(g_RealmID);
    l_Data << uint32(g_RealmID);

    l_Data.WriteBit(0); // DoNotShowAchievementBox
    l_Data.FlushBits();

    SendPacket(&l_Data);
}

#ifndef CROSS
template<>
void AchievementMgr<Guild>::SendAchievementEarned(AchievementEntry const* p_Achievement) const
{
    WorldPacket l_Data(SMSG_GUILD_ACHIEVEMENT_EARNED, 8 + 4 + 8);

    l_Data.appendPackGUID(GetOwner()->GetGUID());
    l_Data << uint32(p_Achievement->ID);
    l_Data << uint32(MS::Utilities::WowTime::Encode(time(NULL)));

    SendPacket(&l_Data);
}
#endif

template<class T>
void AchievementMgr<T>::SendCriteriaUpdate(CriteriaEntry const* /*entry*/, CriteriaProgress const* /*progress*/, uint32 /*timeElapsed*/, bool /*timedCompleted*/, bool/*updateAccount*/) const
{
}

template<>
void AchievementMgr<Player>::SendCriteriaUpdate(CriteriaEntry const* p_Entry, CriteriaProgress const* p_Progress, uint32 p_TimeElapsed, bool p_TimedCompleted, bool p_UpdateAccount) const
{
    WorldPacket l_Data(SMSG_CRITERIA_UPDATE, 4 + 8 + 16 + 2 + 4 + 4 + 4 + 4);

    l_Data << uint32(p_Entry->ID);                                                  ///< CriteriaID
    l_Data << uint64(p_Progress->counter);                                          ///< Quantity
    l_Data.appendPackGUID(GetOwner()->GetGUID());                                   ///< PlayerGUID

    // This are some flags, 1 is for keeping the counter at 0 in client
    if (!p_Entry->StartTimer)
        l_Data << uint32(0);                                                        ///< CreationTime
    else
        l_Data << uint32(p_TimedCompleted ? 0 : 1);

    l_Data << uint32(MS::Utilities::WowTime::Encode(p_Progress->date));
    l_Data << uint32(p_TimeElapsed);                                                ///< ElapsedTime
    l_Data << uint32(0);                                                            ///< CurrentTime @todo send current time

    SendPacket(&l_Data);

    if (p_UpdateAccount)
    {
        WorldPacket l_Packet(SMSG_ACCOUNT_CRITERIA_UPDATE, 4 + 8 + 16 + 2 + 4 + 4 + 4);
        l_Packet << uint32(p_Entry->ID);                                            ///< CriteriaID
        l_Packet << uint64(p_Progress->counter);                                    ///< Quantity
        l_Packet.appendPackGUID(GetOwner()->GetGUID());                             ///< PlayerGUID
        l_Packet << uint32(MS::Utilities::WowTime::Encode(p_Progress->date));       ///< CreationTime
        l_Packet << uint32(p_TimeElapsed);                                          ///< ElapsedTime
        l_Packet << uint32(0);                                                      ///< CurrentTime @todo send current time

        uint32 l_Flag = !p_Entry->StartTimer ? 0 : p_TimedCompleted ? 0 : 1;

        l_Packet.WriteBits(l_Flag, 4);                                              ///< Flags
        l_Packet.FlushBits();
        SendPacket(&l_Packet);
    }
}

#ifndef CROSS
template<>
void AchievementMgr<Guild>::SendCriteriaUpdate(CriteriaEntry const* p_Entry, CriteriaProgress const* p_Progress, uint32 /*timeElapsed*/, bool /*timedCompleted*/, bool /*accountIpdate*/) const
{
    // Will send response to criteria progress request
    WorldPacket l_Data(SMSG_GUILD_CRITERIA_UPDATE);

    l_Data << uint32(1);

    l_Data << uint32(p_Entry->ID);                                                  ///< CriteriaID
    l_Data << uint32(p_Progress->date);                                             ///< DateCreated
    l_Data << uint32(p_Progress->date);                                             ///< DateStarted
    l_Data << uint32(p_Progress->date);                                             ///< DateUpdated (not packed!)
    l_Data << uint64(p_Progress->counter);                                          ///< Quantity
    l_Data.appendPackGUID(p_Progress->CompletedGUID);                               ///< PlayerGUID
    l_Data << uint32(p_Progress->changed);                                          ///< Flags (@todo ?)

    SendPacket(&l_Data);
}
#endif

template<class T>
CriteriaProgressMap* AchievementMgr<T>::GetCriteriaProgressMap()
{
    return &m_criteriaProgress;
}

/**
 * Called at player login. The player might have fulfilled some achievements when the achievement system wasn't working yet
 */
template<class T>
void AchievementMgr<T>::CheckAllAchievementCriteria(Player* p_ReferencePlayer)
{
    if (sWorld->getBoolConfig(CONFIG_ACHIEVEMENT_DISABLE))
        return;

    for (uint32 l_AchievementCriteriaType = 0; l_AchievementCriteriaType < ACHIEVEMENT_CRITERIA_TYPE_TOTAL; ++l_AchievementCriteriaType)
    {
        AchievementCriteriaUpdateTask l_Task;
        l_Task.PlayerGUID = p_ReferencePlayer->GetGUID();
        l_Task.UnitGUID   = 0;
        l_Task.Task = [l_AchievementCriteriaType](uint64 const& p_PlayerGuid, uint64 const& /*p_UnitGUID*/) -> void
        {
            /// Task will be executed async
            /// We need to ensure the player still exist
            Player* l_Player = HashMapHolder<Player>::Find(p_PlayerGuid);
            if (l_Player == nullptr)
                return;

            l_Player->GetAchievementMgr().UpdateAchievementCriteria((AchievementCriteriaTypes)l_AchievementCriteriaType, 0, 0, 0, nullptr, l_Player, true);
        };

        sAchievementMgr->AddCriteriaUpdateTask(l_Task);
    }
}

// Helper function to avoid having to specialize template for a 800 line long function
template <typename T> static bool IsGuild() { return false; }
template<> bool IsGuild<Guild>() { return true; }

/**
 * This function will be called whenever the user might have done a criteria relevant action
 */
template<class T>
void AchievementMgr<T>::UpdateAchievementCriteria(AchievementCriteriaTypes p_Type, uint64 p_MiscValue1 /*= 0*/, uint64 p_MiscValue2 /*= 0*/, uint64 p_MiscValue3 /*= 0*/, Unit const* p_Unit /*= NULL*/, Player* p_ReferencePlayer /*= NULL*/, bool p_LoginCheck)
{
    if (sWorld->getBoolConfig(CONFIG_ACHIEVEMENT_DISABLE))
        return;

    if (p_Type >= ACHIEVEMENT_CRITERIA_TYPE_TOTAL)
    {
        sLog->outDebug(LOG_FILTER_ACHIEVEMENTSYS, "UpdateAchievementCriteria: Unknown criteria type [%u]", p_Type);
        return;
    }

    if (!p_ReferencePlayer)
        return;

    // disable for game masters with GM-mode enabled
    if (p_ReferencePlayer->isGameMaster())
        return;

     // Lua_GetGuildLevelEnabled() is checked in achievement UI to display guild tab
    if (IsGuild<T>() && !sWorld->getBoolConfig(CONFIG_GUILD_LEVELING_ENABLED))
        return;

    AchievementCriteriaEntryList const& l_AchievementCriteriaList = sAchievementMgr->GetAchievementCriteriaByType(p_Type);
    for (AchievementCriteriaEntryList::const_iterator i = l_AchievementCriteriaList.begin(); i != l_AchievementCriteriaList.end(); ++i)
    {
        CriteriaEntry const* l_AchievementCriteria = (*i);
        if (!CanUpdateCriteria(l_AchievementCriteria, NULL, p_MiscValue1, p_MiscValue2, p_MiscValue3, p_Unit, p_ReferencePlayer))
            continue;

        // Requirements not found in the dbcs
        if (AchievementCriteriaDataSet const* l_Data = sAchievementMgr->GetCriteriaDataSet(l_AchievementCriteria))
            if (!l_Data->Meets(p_ReferencePlayer, p_Unit, p_MiscValue1))
                continue;

        switch (p_Type)
        {
            // std. case: increment at 1
            case ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_TALENT_RESETS:
            case ACHIEVEMENT_CRITERIA_TYPE_LOSE_DUEL:
            case ACHIEVEMENT_CRITERIA_TYPE_CREATE_AUCTION:
            case ACHIEVEMENT_CRITERIA_TYPE_WON_AUCTIONS:    // @TODO : for online player only currently
            case ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED:
            case ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED:
            case ACHIEVEMENT_CRITERIA_TYPE_QUEST_ABANDONED:
            case ACHIEVEMENT_CRITERIA_TYPE_FLIGHT_PATHS_TAKEN:
            case ACHIEVEMENT_CRITERIA_TYPE_ACCEPTED_SUMMONINGS:
            case ACHIEVEMENT_CRITERIA_TYPE_LOOT_EPIC_ITEM:
            case ACHIEVEMENT_CRITERIA_TYPE_RECEIVE_EPIC_ITEM:
            case ACHIEVEMENT_CRITERIA_TYPE_DEATH:
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST:
            case ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP:
            case ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON:
            case ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE:
            case ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_PLAYER:
            case ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM:
            case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET:
            case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2:
            case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL:
            case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2:
            case ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA:
            case ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM:
            case ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT:
            case ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT:
            case ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE:
            case ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT:
            case ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT:
            case ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL:
            case ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS:
            case ACHIEVEMENT_CRITERIA_TYPE_HK_RACE:
            case ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE:
            case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL:
            case ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL:
            case ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS:
            case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA:
            case ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA:
            case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE:
            case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE_GUILD:
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ARCHAEOLOGY_PROJECTS:
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GUILD_CHALLENGE_TYPE:
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GUILD_CHALLENGE:
            case ACHIEVEMENT_CRITERIA_TYPE_COOK_SOME_MEALS:
            case ACHIEVEMENT_CRITERIA_TYPE_WIN_CHALLENGE_DUNGEON:
            case ACHIEVEMENT_CRITERIA_TYPE_RECRUIT_FOLLOWER_IN_OWN_GARRISON:
            case ACHIEVEMENT_CRITERIA_TYPE_LEARN_GARRISON_BLUEPRINTS:
            case ACHIEVEMENT_CRITERIA_TYPE_DEFEAT_ENCOUNTER:
                SetCriteriaProgress(l_AchievementCriteria, 1, p_ReferencePlayer, PROGRESS_ACCUMULATE);
                break;
            // std case: increment at miscValue1
            case ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_VENDORS:
            case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TALENTS:
            case ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_QUEST_REWARD:
            case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TRAVELLING:
            case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_AT_BARBER:
            case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_MAIL:
            case ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY:
            case ACHIEVEMENT_CRITERIA_TYPE_GOLD_EARNED_BY_AUCTIONS:     //@TODO : for on line player only currently
            case ACHIEVEMENT_CRITERIA_TYPE_TOTAL_DAMAGE_RECEIVED:
            case ACHIEVEMENT_CRITERIA_TYPE_TOTAL_HEALING_RECEIVED:
            case ACHIEVEMENT_CRITERIA_TYPE_USE_LFD_TO_GROUP_WITH_PLAYERS:
            case ACHIEVEMENT_CRITERIA_TYPE_WIN_BG:
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND:
            case ACHIEVEMENT_CRITERIA_TYPE_DAMAGE_DONE:
            case ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE:
            case ACHIEVEMENT_CRITERIA_TYPE_CATCH_FROM_POOL:
            case ACHIEVEMENT_CRITERIA_TYPE_BUY_GUILD_BANK_SLOTS:
            case ACHIEVEMENT_CRITERIA_TYPE_EARN_GUILD_ACHIEVEMENT_POINTS:
            case ACHIEVEMENT_CRITERIA_TYPE_BUY_GUILD_TABARD:
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_GUILD:
            case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILLS_GUILD:
            case ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLEPET:
            case ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_SPECIFIC_BATTLEPET:
            case ACHIEVEMENT_CRITERIA_TYPE_EARN_BATTLEPET:
            case ACHIEVEMENT_CRITERIA_TYPE_LEVELUP_BATTLEPET:
            case ACHIEVEMENT_CRITERIA_TYPE_WIN_PETBATTLE:
                SetCriteriaProgress(l_AchievementCriteria, p_MiscValue1, p_ReferencePlayer, PROGRESS_ACCUMULATE);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLEPET_IN_COMBAT:
                if (!p_MiscValue1)
                    break;
                SetCriteriaProgress(l_AchievementCriteria, p_MiscValue1, p_ReferencePlayer, PROGRESS_ACCUMULATE);
                break;
            // std case: increment at miscValue2
            case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE:
            case ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE:
            case ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM:
            case ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM:
            case ACHIEVEMENT_CRITERIA_TYPE_CURRENCY:
                SetCriteriaProgress(l_AchievementCriteria, p_MiscValue2, p_ReferencePlayer, PROGRESS_ACCUMULATE);
                break;
                // std case: high value at miscValue1
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_BID:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_SOLD:        //@TODO : for online player only currently
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_DEALT:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_RECEIVED:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEAL_CASTED:
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEALING_RECEIVED:
            case ACHIEVEMENT_CRITERIA_TYPE_COLLECT_HEIRLOOMS:
                SetCriteriaProgress(l_AchievementCriteria, p_MiscValue1, p_ReferencePlayer, PROGRESS_HIGHEST);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL:
                SetCriteriaProgress(l_AchievementCriteria, p_ReferencePlayer->getLevel(), p_ReferencePlayer);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL:
                if (uint32 skillvalue = p_ReferencePlayer->GetBaseSkillValue(l_AchievementCriteria->reach_skill_level.skillID))
                    SetCriteriaProgress(l_AchievementCriteria, skillvalue, p_ReferencePlayer);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL:
                if (uint32 maxSkillvalue = p_ReferencePlayer->GetPureMaxSkillValue(l_AchievementCriteria->learn_skill_level.skillID))
                    SetCriteriaProgress(l_AchievementCriteria, maxSkillvalue, p_ReferencePlayer);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT:
                SetCriteriaProgress(l_AchievementCriteria, p_ReferencePlayer->GetRewardedQuestCount(), p_ReferencePlayer);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY:
            {
                time_t nextDailyResetTime = sWorld->GetNextDailyQuestsResetTime();
                CriteriaProgress *progress = GetCriteriaProgress(l_AchievementCriteria);

                if (!p_MiscValue1) // Login case.
                {
                    // Reset if player missed one day.
                    if (progress && progress->date < (nextDailyResetTime - 2 * DAY))
                        SetCriteriaProgress(l_AchievementCriteria, 0, p_ReferencePlayer, PROGRESS_SET);
                    continue;
                }

                ProgressType progressType;
                if (!progress)
                    // 1st time. Start count.
                    progressType = PROGRESS_SET;
                else if (progress->date < (nextDailyResetTime - 2 * DAY))
                   // Last progress is older than 2 days. Player missed 1 day => Restart count.
                    progressType = PROGRESS_SET;
                else if (progress->date < (nextDailyResetTime - DAY))
                    // Last progress is between 1 and 2 days. => 1st time of the day.
                    progressType = PROGRESS_ACCUMULATE;
                else
                    // Last progress is within the day before the reset => Already counted today.
                    continue;

                SetCriteriaProgress(l_AchievementCriteria, 1, p_ReferencePlayer, progressType);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE:
            {
                uint32 counter = 0;

                const RewardedQuestSet &rewQuests = p_ReferencePlayer->getRewardedQuests();
                for (RewardedQuestSet::const_iterator itr = rewQuests.begin(); itr != rewQuests.end(); ++itr)
                {
                    Quest const* quest = sObjectMgr->GetQuestTemplate(*itr);
                    if (quest && quest->GetZoneOrSort() >= 0 && uint32(quest->GetZoneOrSort()) == l_AchievementCriteria->complete_quests_in_zone.zoneID)
                        ++counter;
                }
                SetCriteriaProgress(l_AchievementCriteria, counter, p_ReferencePlayer);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING:
                // miscValue1 is the ingame fallheight*100 as stored in dbc
                SetCriteriaProgress(l_AchievementCriteria, p_MiscValue1, p_ReferencePlayer);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:
            case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL:
            case ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA:
            case ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP:
            case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM:
            case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM:
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT:
                SetCriteriaProgress(l_AchievementCriteria, 1, p_ReferencePlayer);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT:
                SetCriteriaProgress(l_AchievementCriteria, p_ReferencePlayer->GetBankBagSlotCount(), p_ReferencePlayer);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION:
            {
                int32 reputation = p_ReferencePlayer->GetReputationMgr().GetReputation(l_AchievementCriteria->gain_reputation.factionID);
                if (reputation > 0)
                    SetCriteriaProgress(l_AchievementCriteria, reputation, p_ReferencePlayer);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION:
                SetCriteriaProgress(l_AchievementCriteria, p_ReferencePlayer->GetReputationMgr().GetExaltedFactionCount(), p_ReferencePlayer);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS:
            {
                uint32 spellCount = 0;
                for (PlayerSpellMap::const_iterator spellIter = p_ReferencePlayer->GetSpellMap().begin();
                    spellIter != p_ReferencePlayer->GetSpellMap().end();
                    ++spellIter)
                {
                    SkillLineAbilityMapBounds bounds = sSpellMgr->GetSkillLineAbilityMapBounds(spellIter->first);
                    for (SkillLineAbilityMap::const_iterator skillIter = bounds.first; skillIter != bounds.second; ++skillIter)
                    {
                        if (skillIter->second->skillId == l_AchievementCriteria->learn_skillline_spell.skillLine)
                            spellCount++;
                    }
                }
                SetCriteriaProgress(l_AchievementCriteria, spellCount, p_ReferencePlayer);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REVERED_REPUTATION:
                SetCriteriaProgress(l_AchievementCriteria, p_ReferencePlayer->GetReputationMgr().GetReveredFactionCount(), p_ReferencePlayer);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_GAIN_HONORED_REPUTATION:
                SetCriteriaProgress(l_AchievementCriteria, p_ReferencePlayer->GetReputationMgr().GetHonoredFactionCount(), p_ReferencePlayer);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_KNOWN_FACTIONS:
                SetCriteriaProgress(l_AchievementCriteria, p_ReferencePlayer->GetReputationMgr().GetVisibleFactionCount(), p_ReferencePlayer);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE:
            {
                uint32 spellCount = 0;
                for (PlayerSpellMap::const_iterator spellIter = p_ReferencePlayer->GetSpellMap().begin();
                    spellIter != p_ReferencePlayer->GetSpellMap().end();
                    ++spellIter)
                {
                    SkillLineAbilityMapBounds bounds = sSpellMgr->GetSkillLineAbilityMapBounds(spellIter->first);
                    for (SkillLineAbilityMap::const_iterator skillIter = bounds.first; skillIter != bounds.second; ++skillIter)
                        if (skillIter->second->skillId == l_AchievementCriteria->learn_skill_line.skillLine)
                            spellCount++;
                }
                SetCriteriaProgress(l_AchievementCriteria, spellCount, p_ReferencePlayer);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL:
                SetCriteriaProgress(l_AchievementCriteria, p_ReferencePlayer->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS), p_ReferencePlayer);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_GOLD_VALUE_OWNED:
                SetCriteriaProgress(l_AchievementCriteria, p_ReferencePlayer->GetMoney(), p_ReferencePlayer, PROGRESS_HIGHEST);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_ACHIEVEMENTS_IN_BATTLE_PET:
                SetCriteriaProgress(l_AchievementCriteria, p_MiscValue1, p_ReferencePlayer, PROGRESS_SET);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_EARN_ACHIEVEMENT_POINTS:
                if (!p_MiscValue1)
                    SetCriteriaProgress(l_AchievementCriteria, _achievementPoints, p_ReferencePlayer, PROGRESS_SET);
                else
                    SetCriteriaProgress(l_AchievementCriteria, p_MiscValue1, p_ReferencePlayer, PROGRESS_ACCUMULATE);
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING:
                {
                    uint32 reqTeamType = l_AchievementCriteria->highest_team_rating.teamtype;

                    if (p_MiscValue1)
                    {
                        if (p_MiscValue2 != reqTeamType)
                            continue;
                        SetCriteriaProgress(l_AchievementCriteria, p_MiscValue1, p_ReferencePlayer, PROGRESS_HIGHEST);
                    }
                    else // Login case
                    {
                        for (uint32 arena_slot = 0; arena_slot < MAX_ARENA_SLOT; ++arena_slot)
                        {
                            SetCriteriaProgress(l_AchievementCriteria, p_ReferencePlayer->GetArenaPersonalRating(arena_slot), p_ReferencePlayer, PROGRESS_HIGHEST);
                            break;
                        }
                    }
                }
                break;
            case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_PERSONAL_RATING:
            {
                uint32 reqTeamType = l_AchievementCriteria->highest_personal_rating.teamtype;

                if (p_MiscValue1)
                {
                    if (p_MiscValue2 != reqTeamType)
                        continue;

                    SetCriteriaProgress(l_AchievementCriteria, p_MiscValue1, p_ReferencePlayer, PROGRESS_HIGHEST);
                }

                else // Login case
                {
                    for (uint32 arena_slot = 0; arena_slot < MAX_ARENA_SLOT; ++arena_slot)
                        SetCriteriaProgress(l_AchievementCriteria, p_ReferencePlayer->GetArenaPersonalRating(arena_slot), p_ReferencePlayer, PROGRESS_HIGHEST);
                }

                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_SPENT_GOLD_GUILD_REPAIRS:
            {
                if (!p_MiscValue1)
                    continue;

                SetCriteriaProgress(l_AchievementCriteria, p_MiscValue1, p_ReferencePlayer, PROGRESS_ACCUMULATE);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_REACH_GUILD_LEVEL:
            {
                if (!p_MiscValue1)
                    continue;

                SetCriteriaProgress(l_AchievementCriteria, p_MiscValue1, p_ReferencePlayer);
                break;
            }
            case ACHIEVEMENT_CRITERIA_TYPE_CRAFT_ITEMS_GUILD:
            {
                if (!p_MiscValue1 || !p_MiscValue2)
                    continue;

                SetCriteriaProgress(l_AchievementCriteria, p_MiscValue2, p_ReferencePlayer, PROGRESS_ACCUMULATE);
                break;
            }
            // FIXME: not triggered in code as result, need to implement
            case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID:
            case ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA:
            case ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK:
            case ACHIEVEMENT_CRITERIA_TYPE_EARNED_PVP_TITLE:
            case ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_BATTLEGROUND:
                break;                                   // Not implemented yet :(
            default:
                break;
        }
        
        SetCompletedAchievementsIfNeeded(l_AchievementCriteria, p_ReferencePlayer, p_LoginCheck);
    }
}

template<class T>
bool AchievementMgr<T>::CanCompleteCriteria(CriteriaEntry const* /*achievementCriteria*/, AchievementEntry const* /*achievement*/)
{
    return true;
}

// Only player personal achievements require instance id to check realm firsts
// Guild restrictions are handled with additionalConditionType/additionalConditionValue
template<class T>
static uint32 GetInstanceId(T* /*object*/) { return 0xFFFFFFFF; }

template<>
uint32 GetInstanceId(Player* player) { return player->GetInstanceId(); }

template<>
bool AchievementMgr<Player>::CanCompleteCriteria(CriteriaEntry const* /*achievementCriteria*/, AchievementEntry const* achievement)
{
#ifdef CROSS
    /// Realm first achievement are disable on cross-realm
#endif /* CROSS */
    if (achievement->Flags & (ACHIEVEMENT_FLAG_REALM_FIRST_REACH | ACHIEVEMENT_FLAG_REALM_FIRST_KILL))
#ifndef CROSS
    {
        // Someone on this realm has already completed that achievement
        if (sAchievementMgr->IsRealmCompleted(achievement, GetInstanceId(GetOwner())))
            return false;

        if (GetOwner())
            if (GetOwner()->GetSession())
                if (GetOwner()->GetSession()->GetSecurity())
                    return false;
    }
#else /* CROSS */
        return false;
#endif /* CROSS */

    return true;
}

template<class T>
bool AchievementMgr<T>::IsCompletedCriteria(CriteriaEntry const* p_AchievementCriteria)
{
    AchievementCriteriaTreeList const& l_CriteriaTreeList = sAchievementMgr->GetAchievementCriteriaTreeList(p_AchievementCriteria);
    for (AchievementCriteriaTreeList::const_iterator l_Iter = l_CriteriaTreeList.begin(); l_Iter != l_CriteriaTreeList.end(); l_Iter++)
    {
        CriteriaTreeEntry const* l_CriteriaTree = *l_Iter;
        AchievementEntry const* l_Achievement = sAchievementMgr->GetAchievementEntryByCriteriaTree(l_CriteriaTree);
        if (!l_Achievement)
            return false;

        if (CriteriaEntry const* l_Criteria = sCriteriaStore.LookupEntry(l_CriteriaTree->CriteriaID))
            if (!IsCompletedCriteriaForAchievement(l_Criteria, l_Achievement))
                return false;
    }

    return true;
}

template<class T>
bool AchievementMgr<T>::IsCompletedCriteriaForAchievement(CriteriaEntry const* p_Criteria, AchievementEntry const* p_Achievement)
{
    if (!p_Criteria || !p_Achievement)
        return false;

    CriteriaTreeEntry const* l_CriteriaTree = NULL;
    AchievementCriteriaTreeList const* l_TreeList = sAchievementMgr->GetSubCriteriaTreeById(p_Achievement->CriteriaTree);
    if (l_TreeList)
    {
        for (AchievementCriteriaTreeList::const_iterator l_Iter = l_TreeList->begin(); l_Iter != l_TreeList->end(); l_Iter++)
        {
            if ((*l_Iter)->CriteriaID == p_Criteria->ID)
            {
                l_CriteriaTree = *l_Iter;
                break;
            }
        }
    }

    if (!l_CriteriaTree)
        return false;

    // counter can never complete
    if (p_Achievement->Flags & ACHIEVEMENT_FLAG_COUNTER)
        return false;

    if (p_Achievement->Flags & (ACHIEVEMENT_FLAG_REALM_FIRST_REACH | ACHIEVEMENT_FLAG_REALM_FIRST_KILL))
    {
        // someone on this realm has already completed that achievement
        if (sAchievementMgr->IsRealmCompleted(p_Achievement, GetInstanceId(GetOwner())))
            return false;
    }

    CriteriaProgress const* l_Progress = GetCriteriaProgress(p_Criteria);
    if (!l_Progress)
        return false;

    switch (AchievementCriteriaTypes(p_Criteria->Type))
    {
        case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE:
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_BG:
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ARCHAEOLOGY_PROJECTS:
        case ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL:
        case ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL:
        case ACHIEVEMENT_CRITERIA_TYPE_REACH_GUILD_LEVEL:
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT:
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY:
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE:
        case ACHIEVEMENT_CRITERIA_TYPE_DAMAGE_DONE:
        case ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE:
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST:
        case ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING:
        case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET:
        case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2:
        case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL:
        case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2:
        case ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE:
        case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA:
        case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL:
        case ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL:
        case ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM:
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_PERSONAL_RATING:
        case ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM:
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM:
        case ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT:
        case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION:
        case ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION:
        case ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP:
        case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM:
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT:
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT:
        case ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS:
        case ACHIEVEMENT_CRITERIA_TYPE_HK_RACE:
        case ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE:
        case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM:
        case ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_QUEST_REWARD:
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY:
        case ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT:
        case ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL:
        case ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT:
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS:
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL:
        case ACHIEVEMENT_CRITERIA_TYPE_ACHIEVEMENTS_IN_BATTLE_PET:
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE:
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE:
        case ACHIEVEMENT_CRITERIA_TYPE_USE_LFD_TO_GROUP_WITH_PLAYERS:
        case ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS:
        case ACHIEVEMENT_CRITERIA_TYPE_CURRENCY:
        case ACHIEVEMENT_CRITERIA_TYPE_COOK_SOME_MEALS:
        case ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLEPET:
        case ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_SPECIFIC_BATTLEPET:
        case ACHIEVEMENT_CRITERIA_TYPE_EARN_BATTLEPET:
        case ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLEPET_IN_COMBAT:
        case ACHIEVEMENT_CRITERIA_TYPE_LEVELUP_BATTLEPET:
        case ACHIEVEMENT_CRITERIA_TYPE_BUY_GUILD_BANK_SLOTS:
        case ACHIEVEMENT_CRITERIA_TYPE_SPENT_GOLD_GUILD_REPAIRS:
        case ACHIEVEMENT_CRITERIA_TYPE_CRAFT_ITEMS_GUILD:
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_PETBATTLE:
        case ACHIEVEMENT_CRITERIA_TYPE_CATCH_FROM_POOL:
        case ACHIEVEMENT_CRITERIA_TYPE_EARN_GUILD_ACHIEVEMENT_POINTS:
        case ACHIEVEMENT_CRITERIA_TYPE_BUY_GUILD_TABARD:
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_GUILD:
        case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILLS_GUILD:
        case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE_GUILD:
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GUILD_CHALLENGE_TYPE:
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GUILD_CHALLENGE:
        case ACHIEVEMENT_CRITERIA_TYPE_COLLECT_TOYS:
        case ACHIEVEMENT_CRITERIA_TYPE_COLLECT_HEIRLOOMS:
        case ACHIEVEMENT_CRITERIA_TYPE_DEFEAT_ENCOUNTER:
            return l_Progress->counter >= l_CriteriaTree->Amount;
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT:
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL:
        case ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA:
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_CHALLENGE_DUNGEON:
            return l_Progress->counter >= 1;
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL:
            return l_Progress->counter >= (l_CriteriaTree->Amount * 75);
        case ACHIEVEMENT_CRITERIA_TYPE_EARN_ACHIEVEMENT_POINTS:
            return l_Progress->counter >= 9000;
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA:
            return l_CriteriaTree->Amount && l_Progress->counter >= l_CriteriaTree->Amount;
            // handle all statistic-only criteria here
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND:
        case ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP:
        case ACHIEVEMENT_CRITERIA_TYPE_DEATH:
        case ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON:
        case ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGH_SCORE_IN_ORDALIE:
        case ACHIEVEMENT_CRITERIA_TYPE_MOST_CHALLENGE_DUNGEON_WON:
        case ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_PLAYER:
        case ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING:
        case ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_VENDORS:
        case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TALENTS:
        case ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_TALENT_RESETS:
        case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_AT_BARBER:
        case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_MAIL:
        case ACHIEVEMENT_CRITERIA_TYPE_LOSE_DUEL:
        case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE:
        case ACHIEVEMENT_CRITERIA_TYPE_GOLD_EARNED_BY_AUCTIONS:
        case ACHIEVEMENT_CRITERIA_TYPE_CREATE_AUCTION:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_BID:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_SOLD:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_GOLD_VALUE_OWNED:
        case ACHIEVEMENT_CRITERIA_TYPE_WON_AUCTIONS:
        case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REVERED_REPUTATION:
        case ACHIEVEMENT_CRITERIA_TYPE_GAIN_HONORED_REPUTATION:
        case ACHIEVEMENT_CRITERIA_TYPE_KNOWN_FACTIONS:
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_EPIC_ITEM:
        case ACHIEVEMENT_CRITERIA_TYPE_RECEIVE_EPIC_ITEM:
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED:
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED:
        case ACHIEVEMENT_CRITERIA_TYPE_QUEST_ABANDONED:
        case ACHIEVEMENT_CRITERIA_TYPE_FLIGHT_PATHS_TAKEN:
        case ACHIEVEMENT_CRITERIA_TYPE_ACCEPTED_SUMMONINGS:
        default:
            return false;
    }
    return false;
}

template<class T>
void AchievementMgr<T>::CompletedCriteriaFor(AchievementEntry const* p_Achievement, Player* p_ReferencePlayer, bool p_LoginCheck)
{
    // Counter can never complete
    if (p_Achievement->Flags & ACHIEVEMENT_FLAG_COUNTER)
        return;

    // Already completed and stored
    if (HasAchieved(p_Achievement->ID))
        return;

    if (IsCompletedAchievement(p_Achievement))
        CompletedAchievement(p_Achievement, p_ReferencePlayer, p_LoginCheck);
}

template<class T>
bool AchievementMgr<T>::IsCompletedAchievement(AchievementEntry const* p_Entry)
{
    // counter can never complete
    if (p_Entry->Flags & ACHIEVEMENT_FLAG_COUNTER)
        return false;

    // for achievement with referenced achievement criterias get from referenced and counter from self
    uint32 l_AchievementForTestID = p_Entry->SharesCriteria ? p_Entry->SharesCriteria : p_Entry->ID;
    uint32 l_AchievementForTestCount = p_Entry->MinimumCriteria;

    if (CriteriaTreeEntry const* l_CriteriaTree = sCriteriaTreeStore.LookupEntry(p_Entry->CriteriaTree))
        l_AchievementForTestCount = l_CriteriaTree->Amount;

    AchievementCriteriaEntryList l_CriteriaList;
    AchievementCriteriaTreeList const* l_List = sAchievementMgr->GetSubCriteriaTreeById(p_Entry->CriteriaTree);
    for (AchievementCriteriaTreeList::const_iterator l_Iter = l_List->begin(); l_Iter != l_List->end(); l_Iter++)
    {
        CriteriaTreeEntry const* l_CriteriaTree = *l_Iter;
        if (CriteriaEntry const* l_Criteria = sCriteriaStore.LookupEntry(l_CriteriaTree->CriteriaID))
            l_CriteriaList.push_back(l_Criteria);
    }

    if (!l_CriteriaList.size())
        return false;

    uint64 l_Counter = 0;

    // For SUMM achievements, we have to count the progress of each criteria of the achievement.
    // Oddly, the target count is NOT contained in the achievement, but in each individual criteria
    if (p_Entry->Flags & ACHIEVEMENT_FLAG_SUMM)
    {
        for (AchievementCriteriaEntryList::const_iterator itr = l_CriteriaList.begin(); itr != l_CriteriaList.end(); ++itr)
        {
            CriteriaEntry const* l_Criteria = *itr;

            CriteriaProgress const* l_Progress = GetCriteriaProgress(l_Criteria);
            if (!l_Progress)
                continue;

            l_Counter += l_Progress->counter;

            // for counters, field4 contains the main count requirement
            if (l_Counter >= sCriteriaTreeStore.LookupEntry(p_Entry->CriteriaTree)->Amount)
                return true;
        }
        return false;
    }

    // Default case - need complete all or
    bool l_IsAllCompleted = true;
    for (AchievementCriteriaEntryList::const_iterator l_Iter = l_CriteriaList.begin(); l_Iter != l_CriteriaList.end(); ++l_Iter)
    {
        CriteriaEntry const* l_Criteria = *l_Iter;

        bool l_IsCompleted = IsCompletedCriteriaForAchievement(l_Criteria, p_Entry);

        // found an uncompleted criteria, but DONT return false yet - there might be a completed criteria with ACHIEVEMENT_CRITERIA_COMPLETE_FLAG_ALL
        if (l_IsCompleted)
            ++l_Counter;
        else
            l_IsAllCompleted = false;

        // completed as have req. count of completed criterias
        if (l_AchievementForTestCount > 0 && l_AchievementForTestCount <= l_Counter)
            return true;
    }

    // all criterias completed requirement
    if (l_IsAllCompleted && l_AchievementForTestCount == 0)
        return true;

    return false;
}

template<class T>
CriteriaProgress* AchievementMgr<T>::GetCriteriaProgress(uint32 entry)
{
    CriteriaProgressMap* criteriaProgressMap = GetCriteriaProgressMap();

    if (!criteriaProgressMap)
        return NULL;

    CriteriaProgressMap::iterator iter = criteriaProgressMap->find(entry);

    if (iter == criteriaProgressMap->end())
        return NULL;

    return &(iter->second);
}

template<class T>
CriteriaProgress* AchievementMgr<T>::GetCriteriaProgress(CriteriaEntry const* entry)
{
   CriteriaProgressMap* criteriaProgressMap = GetCriteriaProgressMap();

    if (!criteriaProgressMap)
        return NULL;

    CriteriaProgressMap::iterator iter = criteriaProgressMap->find(entry->ID);

    if (iter == criteriaProgressMap->end())
        return NULL;

    return &(iter->second);
}

template<class T>
void AchievementMgr<T>::SetCriteriaProgress(CriteriaEntry const* p_Entry, uint64 p_ChangeValue, Player* p_ReferencePlayer, ProgressType p_Type)
{
    // Don't allow to cheat - doing timed achievements without timer active
    TimedAchievementMap::iterator l_TimedIter = m_timedAchievements.find(p_Entry->ID);
    if (p_Entry->StartTimer && l_TimedIter == m_timedAchievements.end())
        return;

    CriteriaProgress* l_Progress = GetCriteriaProgress(p_Entry);
    if (!l_Progress)
    {
        // not create record for 0 counter but allow it for timed achievements
        // we will need to send 0 progress to client to start the timer
        if (p_ChangeValue == 0 && !p_Entry->StartTimer)
            return;

        l_Progress = &m_criteriaProgress[p_Entry->ID];
        l_Progress->counter = p_ChangeValue;

        m_NeedDBSync = true;
    }
    else
    {
        uint64 l_NewValue = 0;
        switch (p_Type)
        {
            case PROGRESS_SET:
                l_NewValue = p_ChangeValue;
                break;
            case PROGRESS_ACCUMULATE:
            {
                // avoid overflow
                uint64 l_MaxValue = std::numeric_limits<uint64>::max();
                l_NewValue = l_MaxValue - l_Progress->counter > p_ChangeValue ? l_Progress->counter + p_ChangeValue : l_MaxValue;
                break;
            }
            case PROGRESS_HIGHEST:
                l_NewValue = l_Progress->counter < p_ChangeValue ? p_ChangeValue : l_Progress->counter;
                break;
            default:
                break;
        }

        // not update (not mark as changed) if counter will have same value
        if (l_Progress->counter == l_NewValue && !p_Entry->StartTimer)
            return;

        l_Progress->counter = l_NewValue;
        m_NeedDBSync = true;
    }

    l_Progress->changed = true;
    l_Progress->date = time(NULL); // set the date to the latest update.
    uint32 l_TimeElapsed = 0; // @todo : Fix me

    bool l_NeedAccountUpdate = false;

    AchievementCriteriaTreeList l_CriteriaList = sAchievementMgr->GetAchievementCriteriaTreeList(p_Entry);
    for (AchievementCriteriaTreeList::const_iterator l_Iter = l_CriteriaList.begin(); l_Iter != l_CriteriaList.end(); l_Iter++)
    {
        AchievementEntry const* l_Achievement = sAchievementMgr->GetAchievementEntryByCriteriaTree(*l_Iter);
        if (!l_Achievement)
            continue;

        if (l_Achievement->Flags & ACHIEVEMENT_FLAG_ACCOUNT)
            l_NeedAccountUpdate = true;

        bool l_IsCriteriaComplete = IsCompletedCriteriaForAchievement(p_Entry, l_Achievement);

        if (p_Entry->StartTimer)
        {
            // Client expects this in packet
            l_TimeElapsed = p_Entry->StartTimer - (l_TimedIter->second / IN_MILLISECONDS);

            // Remove the timer, we wont need it anymore
            if (l_IsCriteriaComplete)
                m_timedAchievements.erase(l_TimedIter);
        }

        if (l_IsCriteriaComplete && l_Achievement->Flags & ACHIEVEMENT_FLAG_SHOW_CRITERIA_MEMBERS && !l_Progress->CompletedGUID)
            l_Progress->CompletedGUID = p_ReferencePlayer->GetGUID();
    }

    SendCriteriaUpdate(p_Entry, l_Progress, l_TimeElapsed, false, l_NeedAccountUpdate);
}

template<class T>
void AchievementMgr<T>::SetCompletedAchievementsIfNeeded(CriteriaEntry const* p_Criteria, Player* p_RefPlayer, bool p_LoginCheck /*= false*/)
{
    AchievementCriteriaTreeList l_AchievementCriteriaTreeList = sAchievementMgr->GetAchievementCriteriaTreeList(p_Criteria);
    for (AchievementCriteriaTreeList::const_iterator l_Iter = l_AchievementCriteriaTreeList.begin(); l_Iter != l_AchievementCriteriaTreeList.end(); l_Iter++)
    {
        AchievementEntry const* l_Achievement = sAchievementMgr->GetAchievementEntryByCriteriaTree(*l_Iter);
        if (!l_Achievement)
            continue;

        if (IsCompletedCriteriaForAchievement(p_Criteria, l_Achievement))
            CompletedCriteriaFor(l_Achievement, p_RefPlayer, p_LoginCheck);

        // check again the completeness for SUMM and REQ COUNT achievements,
        // as they don't depend on the completed criteria but on the sum of the progress of each individual criteria
        if (l_Achievement->Flags & ACHIEVEMENT_FLAG_SUMM)
        {
            if (IsCompletedAchievement(l_Achievement))
                CompletedAchievement(l_Achievement, p_RefPlayer, p_LoginCheck);
        }

        if (AchievementEntryList const* l_AchRefList = sAchievementMgr->GetAchievementByReferencedId(l_Achievement->ID))
        {
            for (AchievementEntryList::const_iterator l_Itr = l_AchRefList->begin(); l_Itr != l_AchRefList->end(); ++l_Itr)
            {
                if (IsCompletedAchievement(*l_Itr))
                    CompletedAchievement(*l_Itr, p_RefPlayer, p_LoginCheck);
            }
        }
    }
}

template<class T>
void AchievementMgr<T>::UpdateTimedAchievements(uint32 timeDiff)
{
    if (!m_timedAchievements.empty())
    {
        for (TimedAchievementMap::iterator itr = m_timedAchievements.begin(); itr != m_timedAchievements.end();)
        {
            // Time is up, remove timer and reset progress
            if (itr->second <= timeDiff)
            {
                CriteriaEntry const* entry = sAchievementMgr->GetAchievementCriteria(itr->first);
                RemoveCriteriaProgress(entry);
                m_timedAchievements.erase(itr++);
            }
            else
            {
                itr->second -= timeDiff;
                ++itr;
            }
        }
    }
}

template<class T>
void AchievementMgr<T>::StartTimedAchievement(AchievementCriteriaTimedTypes /*type*/, uint32 /*entry*/, uint32 /*timeLost = 0*/)
{
}

template<>
void AchievementMgr<Player>::StartTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry, uint32 timeLost /* = 0 */)
{
    AchievementCriteriaEntryList const& achievementCriteriaList = sAchievementMgr->GetTimedAchievementCriteriaByType(type);
    for (AchievementCriteriaEntryList::const_iterator i = achievementCriteriaList.begin(); i != achievementCriteriaList.end(); ++i)
    {
        if ((*i)->StartAsset != entry)
            continue;

        AchievementEntry const* achievement = sAchievementMgr->GetAchievement(entry);
        if (m_timedAchievements.find((*i)->ID) == m_timedAchievements.end() && !IsCompletedCriteriaForAchievement(*i, achievement))
        {
            // Start the timer
            if ((*i)->StartTimer * IN_MILLISECONDS > timeLost)
            {
                m_timedAchievements[(*i)->ID] = (*i)->StartTimer * IN_MILLISECONDS - timeLost;

                // and at client too
                SetCriteriaProgress(*i, 0, GetOwner(), PROGRESS_SET);
            }
        }
    }
}

template<class T>
void AchievementMgr<T>::RemoveTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry)
{
    AchievementCriteriaEntryList const& achievementCriteriaList = sAchievementMgr->GetTimedAchievementCriteriaByType(type);
    for (AchievementCriteriaEntryList::const_iterator i = achievementCriteriaList.begin(); i != achievementCriteriaList.end(); ++i)
    {
        if ((*i)->StartAsset != entry)
            continue;

        TimedAchievementMap::iterator timedIter = m_timedAchievements.find((*i)->ID);
        // We don't have timer for this achievement
        if (timedIter == m_timedAchievements.end())
            continue;

        // Remove progress
        RemoveCriteriaProgress(*i);

        // Remove the timer
        m_timedAchievements.erase(timedIter);
    }
}

template<class T>
void AchievementMgr<T>::CompletedAchievement(AchievementEntry const* p_Achievement, Player* p_ReferencePlayer, bool p_LoginCheck)
{
    // Disable for game masters with GM-mode enabled
    if (GetOwner()->isGameMaster())
        return;

    if (p_Achievement->Flags & ACHIEVEMENT_FLAG_COUNTER || HasAchieved(p_Achievement->ID))
        return;

    if (p_Achievement->Flags & ACHIEVEMENT_FLAG_SHOW_IN_GUILD_NEWS)
    {
#ifndef CROSS
        if (Guild* l_Guild = sGuildMgr->GetGuildById(p_ReferencePlayer->GetGuildId()))
            l_Guild->GetNewsLog().AddNewEvent(GUILD_NEWS_PLAYER_ACHIEVEMENT, time(NULL), p_ReferencePlayer->GetGUID(), p_Achievement->Flags & ACHIEVEMENT_FLAG_SHOW_IN_GUILD_HEADER, p_Achievement->ID);
#else
        //if (Guild* l_Guild = sGuildMgr->GetGuildById(p_ReferencePlayer->GetGuildId()))
            //l_Guild->GetNewsLog().AddNewEvent(GUILD_NEWS_PLAYER_ACHIEVEMENT, time(NULL), p_ReferencePlayer->GetGUID(), p_Achievement->Flags & ACHIEVEMENT_FLAG_SHOW_IN_GUILD_HEADER, p_Achievement->ID);
#endif
    }

    switch (p_Achievement->ID)
    {
        case 7433:  ///< Newbie
        case 6566:  ///< Just a Pup
            GetOwner()->GetSession()->SendPetBattleSlotUpdates(true);
            GetOwner()->GetSession()->SendBattlePetLicenseChanged();
            break;

        case 6556:  ///< Going to Need More Traps
        case 6581:  ///< Pro Pet Crew
            GetOwner()->GetSession()->SendBattlePetTrapLevel();
            break;
    }

    if (!GetOwner()->GetSession()->PlayerLoading() && !p_LoginCheck)
        SendAchievementEarned(p_Achievement);

    if (HasAccountAchieved(p_Achievement->ID))
    {
        m_CompletedAchievementsLock.acquire();
        CompletedAchievementData& l_Data = m_completedAchievements[p_Achievement->ID];
        l_Data.completedByThisCharacter = true;
        l_Data.changed = true;
        m_CompletedAchievementsLock.release();

        /// Rewards must be handled too in this case
        RewardAchievement(p_Achievement);

        return;
    }

    m_CompletedAchievementsLock.acquire();
    CompletedAchievementData& l_Data = m_completedAchievements[p_Achievement->ID];
    l_Data.completedByThisCharacter = true;
    l_Data.date = time(NULL);
    l_Data.first_guid = MAKE_NEW_GUID(GetOwner()->GetGUIDLow(), 0, HIGHGUID_PLAYER);
    l_Data.changed = true;
    m_CompletedAchievementsLock.release();

    sAchievementMgr->SetRealmCompleted(p_Achievement, GetInstanceId(GetOwner()));

    _achievementPoints += p_Achievement->Points;

    // Category is BattlePet
    if (p_Achievement->Category >= 15117 && p_Achievement->Category <= 15120)
        UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_ACHIEVEMENTS_IN_BATTLE_PET, p_Achievement->Points, 0, 0, NULL, p_ReferencePlayer);

    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT, 0, 0, 0, NULL, p_ReferencePlayer);
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EARN_ACHIEVEMENT_POINTS, p_Achievement->Points, 0, 0, NULL, p_ReferencePlayer);

    RewardAchievement(p_Achievement);
}

#ifndef CROSS
template<>
void AchievementMgr<Guild>::CompletedAchievement(AchievementEntry const* achievement, Player* referencePlayer, bool p_LoginCheck)
{
    if (achievement->Flags & ACHIEVEMENT_FLAG_COUNTER || HasAchieved(achievement->ID)  || !(achievement->Flags & ACHIEVEMENT_FLAG_GUILD))
        return;

    /*if (achievement->flags & ACHIEVEMENT_FLAG_SHOW_IN_GUILD_NEWS)
        if (Guild* guild = sGuildMgr->GetGuildById(referencePlayer->GetGuildId()))
            guild->GetNewsLog().AddNewEvent(GUILD_NEWS_GUILD_ACHIEVEMENT, time(NULL), 0, achievement->flags & ACHIEVEMENT_FLAG_SHOW_IN_GUILD_HEADER, achievement->ID);*/

    SendAchievementEarned(achievement);
    CompletedAchievementData& ca = m_completedAchievements[achievement->ID];
    ca.date = time(NULL);
    ca.changed = true;

    if (achievement->Flags & ACHIEVEMENT_FLAG_SHOW_GUILD_MEMBERS)
    {
        if (referencePlayer->GetGuildId() == GetOwner()->GetId())
            ca.guids.insert(referencePlayer->GetGUID());

        if (Group const* group = referencePlayer->GetGroup())
            for (GroupReference const* ref = group->GetFirstMember(); ref != NULL; ref = ref->next())
                if (Player const* groupMember = ref->getSource())
                    if (groupMember->GetGuildId() == GetOwner()->GetId())
                        ca.guids.insert(groupMember->GetGUID());
    }

    sAchievementMgr->SetRealmCompleted(achievement, referencePlayer->GetInstanceId());

    _achievementPoints += achievement->Points;

    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT, 0, 0, 0, NULL, referencePlayer);
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EARN_ACHIEVEMENT_POINTS, achievement->Points, 0, 0, NULL, referencePlayer);
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EARN_GUILD_ACHIEVEMENT_POINTS, achievement->Points, 0, 0, NULL, referencePlayer);

    m_NeedDBSync = true;
}
#endif

template<class T>
void AchievementMgr<T>::RewardAchievement(AchievementEntry const* p_Achievement)
{
    /// Reward items and titles if any
    AchievementReward const* l_Reward = sAchievementMgr->GetAchievementReward(p_Achievement);

    /// No rewards
    if (!l_Reward)
        return;

    /// Titles
    /// Currently there's only one achievement that deals with gender-specific titles.
    /// Since no common attributes were found, (not even in titleRewardFlags field)
    /// we explicitly check by ID. Maybe in the future we could move the achievement_reward
    /// condition fields to the condition system.
    if (uint32 l_TitleID = l_Reward->titleId[p_Achievement->ID == 1793 ? GetOwner()->getGender() : (GetOwner()->GetTeam() == ALLIANCE ? 0 : 1)])
        if (CharTitlesEntry const* l_TitleEntry = sCharTitlesStore.LookupEntry(l_TitleID))
            GetOwner()->SetTitle(l_TitleEntry);

    /// Mail
    if (l_Reward->sender)
    {
#ifndef CROSS
        Item* l_Item = l_Reward->itemId ? Item::CreateItem(l_Reward->itemId, 1, GetOwner()) : NULL;

        int l_LocIDX = GetOwner()->GetSession()->GetSessionDbLocaleIndex();

        /// Subject and text
        std::string l_Subject = l_Reward->subject;
        std::string l_Text = l_Reward->text;
        if (l_LocIDX >= 0)
        {
            if (AchievementRewardLocale const* l_Locale = sAchievementMgr->GetAchievementRewardLocale(p_Achievement))
            {
                ObjectMgr::GetLocaleString(l_Locale->subject, l_LocIDX, l_Subject);
                ObjectMgr::GetLocaleString(l_Locale->text, l_LocIDX, l_Text);
            }
        }

        MailDraft l_Draft(l_Subject, l_Text);

        SQLTransaction l_Transaction = CharacterDatabase.BeginTransaction();
        if (l_Item)
        {
            /// Save new item before send
            l_Item->SaveToDB(l_Transaction);                               ///< Save for prevent lost at next mail load, if send fail then item will deleted

                                                                           ///< Item
            l_Draft.AddItem(l_Item);
        }

        l_Draft.SendMailTo(l_Transaction, GetOwner(), MailSender(MAIL_CREATURE, l_Reward->sender));
        CharacterDatabase.CommitTransaction(l_Transaction);
#else
        if (InterRealmClient* client = GetOwner()->GetSession()->GetInterRealmClient())
            client->SendAchievementReward(GetOwner()->GetRealGUID(), p_Achievement->ID);
#endif
    }

    /// Spell
    if (l_Reward->SpellID)
        GetOwner()->learnSpell(l_Reward->SpellID, false, false);
}

#ifndef CROSS
template<>
void AchievementMgr<Guild>::RewardAchievement(AchievementEntry const* p_Achievement)
{
}
#endif

struct VisibleAchievementPred
{
    bool operator()(CompletedAchievementMap::value_type const& val)
    {
        AchievementEntry const* achievement = sAchievementMgr->GetAchievement(val.first);
        return achievement && !(achievement->Flags & ACHIEVEMENT_FLAG_HIDDEN);
    }
};

template<class T>
void AchievementMgr<T>::SendAllAchievementData(Player* /*receiver*/)
{
    const CriteriaProgressMap* l_ProgressMap = GetCriteriaProgressMap();

    if (!l_ProgressMap)
        return;

    VisibleAchievementPred l_IsVisible;
    size_t l_CriteriaCount = l_ProgressMap->size();
    
    m_CompletedAchievementsLock.acquire();
    size_t l_AchievementCount = std::count_if(m_completedAchievements.begin(), m_completedAchievements.end(), l_IsVisible);

    WorldPacket data(SMSG_ALL_ACHIEVEMENT_DATA, 200 * 1024);

    data << uint32(l_AchievementCount);
    data << uint32(l_CriteriaCount);

    for (CompletedAchievementMap::const_iterator itr = m_completedAchievements.begin(); itr != m_completedAchievements.end(); ++itr)
    {
        if (!l_IsVisible(*itr))
            continue;

        data << uint32(itr->first);                                             ///< Id
        data << uint32(MS::Utilities::WowTime::Encode((*itr).second.date));     ///< Date
        data.appendPackGUID((*itr).second.first_guid);                          ///< Owner
        data << uint32(g_RealmID);                                              ///< Virtual Realm Address
        data << uint32(g_RealmID);                                              ///< Native Realm Address
    }
    m_CompletedAchievementsLock.release();

    for (CriteriaProgressMap::const_iterator itr = l_ProgressMap->begin(); itr != l_ProgressMap->end(); ++itr)
    {
        data << uint32(itr->first);                                             ///< Id
        data << uint64(itr->second.counter);                                    ///< Quantity
        data.appendPackGUID(GetOwner()->GetGUID());                             ///< PlayerGUID
        data << uint32(MS::Utilities::WowTime::Encode(time(0)));                ///< Date
        data << uint32(0);                                                      ///< TimeFromStart
        data << uint32(0);                                                      ///< TimeFromCreate
        data.WriteBits(0, 4);                                                   ///< Flags
        data.FlushBits();
    }

    SendPacket(&data);
}

#ifndef CROSS
template<>
void AchievementMgr<Guild>::SendAllAchievementData(Player* receiver)
{
    WorldPacket l_Data(SMSG_ALL_GUILD_ACHIEVEMENTS, m_completedAchievements.size() * (4 + 4 + 16 + 2 + 4 + 4) + 4);
    l_Data << uint32(m_completedAchievements.size());

    for (CompletedAchievementMap::const_iterator l_It = m_completedAchievements.begin(); l_It != m_completedAchievements.end(); ++l_It)
    {
        l_Data << uint32(l_It->first);
        l_Data << uint32(MS::Utilities::WowTime::Encode(l_It->second.date));
        l_Data.appendPackGUID(l_It->second.first_guid);
        l_Data << uint32(g_RealmID);
        l_Data << uint32(g_RealmID);
    }

    receiver->GetSession()->SendPacket(&l_Data);
}

#endif

template<class T>
void AchievementMgr<T>::SendAchievementInfo(Player* /*p_Receiver*/, uint32 /*p_AchievementID*/ /*= 0*/)
{
}

template<>
void AchievementMgr<Player>::SendAchievementInfo(Player* p_Receiver, uint32 /*p_AchievementID = 0*/)
{
    CriteriaProgressMap* l_ProgressMap = GetCriteriaProgressMap();
    if (!l_ProgressMap)
        return;

    ObjectGuid l_Guid = GetOwner()->GetGUID();

    VisibleAchievementPred l_IsVisible;
    size_t l_CriteriaCount = l_ProgressMap->size();
    m_CompletedAchievementsLock.acquire();
    size_t l_AchievementCount = std::count_if(m_completedAchievements.begin(), m_completedAchievements.end(), l_IsVisible);

    WorldPacket l_Data(SMSG_RESPOND_INSPECT_ACHIEVEMENTS, 80 * 1024);
    l_Data.appendPackGUID(l_Guid);

    l_Data << uint32(l_AchievementCount);
    l_Data << uint32(l_CriteriaCount);

    for (CompletedAchievementMap::const_iterator l_Iter = m_completedAchievements.begin(); l_Iter != m_completedAchievements.end(); ++l_Iter)
    {
        if (!l_IsVisible(*l_Iter))
            continue;

        l_Data << uint32(l_Iter->first);
        l_Data << uint32(MS::Utilities::WowTime::Encode((*l_Iter).second.date));
        l_Data.appendPackGUID((*l_Iter).second.first_guid);
        l_Data << uint32(g_RealmID);
        l_Data << uint32(g_RealmID);
    }
    m_CompletedAchievementsLock.release();

    for (CriteriaProgressMap::const_iterator l_Iter = l_ProgressMap->begin(); l_Iter != l_ProgressMap->end(); ++l_Iter)
    {
        l_Data << uint32(l_Iter->first);
        l_Data.appendPackGUID(l_Iter->second.counter);
        l_Data.appendPackGUID(l_Guid);
        l_Data << uint32(0);
        l_Data << uint32(MS::Utilities::WowTime::Encode((*l_Iter).second.date));
        l_Data << uint32(0);
        l_Data << uint32(0);
    }

    p_Receiver->GetSession()->SendPacket(&l_Data);
}

#ifndef CROSS
template<>
void AchievementMgr<Guild>::SendAchievementInfo(Player* p_Receiver, uint32 p_AchievementID /*= 0*/)
{
    // Will send response to criteria progress request
    CriteriaProgressMap* l_ProgressMap = GetCriteriaProgressMap();
    if (l_ProgressMap == nullptr)
        return;

    AchievementEntry const* l_Achievement = sAchievementStore.LookupEntry(p_AchievementID);
    if (l_Achievement == nullptr)
        return;

    AchievementCriteriaTreeList const* l_CriteriaTreeList = sAchievementMgr->GetSubCriteriaTreeById(l_Achievement->CriteriaTree);
    if (l_CriteriaTreeList == nullptr)
        return;

    for (AchievementCriteriaTreeList::const_iterator l_Iter = l_CriteriaTreeList->begin(); l_Iter != l_CriteriaTreeList->end(); ++l_Iter)
    {
        for (CriteriaProgressMap::const_iterator l_It = l_ProgressMap->begin(); l_It != l_ProgressMap->end(); ++l_It)
        {
            CriteriaEntry const* l_Criteria = sCriteriaStore.LookupEntry(l_It->first);
            if (l_Criteria == nullptr)
                continue;

            if (l_Criteria->ID == (*l_Iter)->CriteriaID)
            {
                WorldPacket l_Data(SMSG_GUILD_CRITERIA_UPDATE);

                l_Data << uint32(1);

                l_Data << uint32(l_Criteria->ID);
                l_Data << uint32(l_It->second.date);
                l_Data << uint32(l_It->second.date);
                l_Data << uint32(l_It->second.date);
                l_Data << uint64(l_It->second.counter);
                l_Data.appendPackGUID(l_It->second.CompletedGUID);
                l_Data << uint32(l_It->second.changed);

                p_Receiver->GetSession()->SendPacket(&l_Data);
                break;
            }
        }
    }
}
#endif

template<class T>
bool AchievementMgr<T>::HasAchieved(uint32 achievementId) const
{
    m_CompletedAchievementsLock.acquire();
    bool l_Result = m_completedAchievements.find(achievementId) != m_completedAchievements.end();
    m_CompletedAchievementsLock.release();
    
    return l_Result;
}

template<>
bool AchievementMgr<Player>::HasAchieved(uint32 achievementId) const
{
    m_CompletedAchievementsLock.acquire();
    CompletedAchievementMap::const_iterator itr = m_completedAchievements.find(achievementId);

    if (itr == m_completedAchievements.end())
    {
        m_CompletedAchievementsLock.release();
        return false;
    }
    m_CompletedAchievementsLock.release();

    return (*itr).second.completedByThisCharacter;
}

template<class T>
bool AchievementMgr<T>::HasAccountAchieved(uint32 achievementId) const
{
    m_CompletedAchievementsLock.acquire();
    bool l_Result = m_completedAchievements.find(achievementId) != m_completedAchievements.end();
    m_CompletedAchievementsLock.release();
    
    return l_Result;
}

template<class T>
uint64 AchievementMgr<T>::GetFirstAchievedCharacterOnAccount(uint32 achievementId) const
{
    m_CompletedAchievementsLock.acquire();
    CompletedAchievementMap::const_iterator itr = m_completedAchievements.find(achievementId);

    if (itr == m_completedAchievements.end())
    {
        m_CompletedAchievementsLock.release();
        return 0LL;
    }
        
    m_CompletedAchievementsLock.release();

    return (*itr).second.first_guid;
}

template<class T>
bool AchievementMgr<T>::CanUpdateCriteria(CriteriaEntry const* p_Criteria, AchievementEntry const* p_Achievement, uint64 p_MiscValue1, uint64 p_MiscValue2, uint64 p_MiscValue3, Unit const* p_Unit, Player* p_ReferencePlayer)
{
    if (DisableMgr::IsDisabledFor(DISABLE_TYPE_ACHIEVEMENT_CRITERIA, p_Criteria->ID, NULL))
    {
        sLog->outTrace(LOG_FILTER_ACHIEVEMENTSYS, "CanUpdateCriteria: (Id: %u Type %s) Disabled",
            p_Criteria->ID, AchievementGlobalMgr::GetCriteriaTypeString(p_Criteria->Type));
        return false;
    }

    if (p_Achievement && p_Achievement->InstanceId != -1 && p_ReferencePlayer->GetMapId() != uint32(p_Achievement->InstanceId))
    {
        sLog->outTrace(LOG_FILTER_ACHIEVEMENTSYS, "CanUpdateCriteria: (Id: %u Type %s) Wrong map",
            p_Criteria->ID, AchievementGlobalMgr::GetCriteriaTypeString(p_Criteria->Type));
        return false;
    }

    if (p_Achievement && ((p_Achievement->Faction == ACHIEVEMENT_FACTION_HORDE && p_ReferencePlayer->GetTeam() != HORDE) ||
        (p_Achievement->Faction == ACHIEVEMENT_FACTION_ALLIANCE && p_ReferencePlayer->GetTeam() != ALLIANCE)))
    {
        sLog->outTrace(LOG_FILTER_ACHIEVEMENTSYS, "CanUpdateCriteria: (Id: %u Type %s) Wrong faction",
            p_Criteria->ID, AchievementGlobalMgr::GetCriteriaTypeString(p_Criteria->Type));
        return false;
    }

    if (p_Achievement && ((!(p_Achievement->Flags & ACHIEVEMENT_FLAG_GUILD)) && IsGuild<T>()))
    {
        sLog->outTrace(LOG_FILTER_ACHIEVEMENTSYS, "CanUpdateCriteria: (Id: %u Type %s) Cannot update non-guild achcievement",
            p_Criteria->ID, AchievementGlobalMgr::GetCriteriaTypeString(p_Criteria->Type));
        return false;
    }

    if (IsCompletedCriteria(p_Criteria))
    {
        sLog->outTrace(LOG_FILTER_ACHIEVEMENTSYS, "CanUpdateCriteria: (Id: %u Type %s) Is Completed",
            p_Criteria->ID, AchievementGlobalMgr::GetCriteriaTypeString(p_Criteria->Type));
        return false;
    }

    if (!RequirementsSatisfied(p_Criteria, p_MiscValue1, p_MiscValue2, p_MiscValue3, p_Unit, p_ReferencePlayer))
    {
        sLog->outTrace(LOG_FILTER_ACHIEVEMENTSYS, "CanUpdateCriteria: (Id: %u Type %s) Requirements not satisfied",
            p_Criteria->ID, AchievementGlobalMgr::GetCriteriaTypeString(p_Criteria->Type));
        return false;
    }

    if (!AdditionalRequirementsSatisfied(p_Criteria, p_MiscValue1, p_MiscValue2, p_Unit, p_ReferencePlayer))
    {
        sLog->outTrace(LOG_FILTER_ACHIEVEMENTSYS, "CanUpdateCriteria: (Id: %u Type %s) Additional requirements not satisfied",
            p_Criteria->ID, AchievementGlobalMgr::GetCriteriaTypeString(p_Criteria->Type));
        return false;
    }

    if (!ConditionsSatisfied(p_Criteria, p_ReferencePlayer))
    {
        sLog->outTrace(LOG_FILTER_ACHIEVEMENTSYS, "CanUpdateCriteria: (Id: %u Type %s) Conditions not satisfied",
            p_Criteria->ID, AchievementGlobalMgr::GetCriteriaTypeString(p_Criteria->Type));
        return false;
    }

    if (RequiresScript(p_Criteria))
    {
        return false;
    }

    return true;
}

template<class T>
bool AchievementMgr<T>::ConditionsSatisfied(CriteriaEntry const* /*p_Criteria*/, Player* /*p_ReferencePlayer*/) const
{
    return true;
}

template<class T>
bool AchievementMgr<T>::RequirementsSatisfied(CriteriaEntry const* p_Criteria, uint64 p_MiscValue1, uint64 p_MiscValue2, uint64 p_MiscValue3, Unit const* p_Unit, Player* p_ReferencePlayer) const
{
    switch (AchievementCriteriaTypes(p_Criteria->Type))
    {
        case ACHIEVEMENT_CRITERIA_TYPE_ACCEPTED_SUMMONINGS:
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST:
        case ACHIEVEMENT_CRITERIA_TYPE_CREATE_AUCTION:
        case ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING:
        case ACHIEVEMENT_CRITERIA_TYPE_FLIGHT_PATHS_TAKEN:
        case ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS:
        case ACHIEVEMENT_CRITERIA_TYPE_GOLD_EARNED_BY_AUCTIONS:
        case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_AT_BARBER:
        case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_MAIL:
        case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TALENTS:
        case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TRAVELLING:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_BID:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_SOLD:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEALING_RECEIVED:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEAL_CASTED:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_DEALT:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_RECEIVED:
        case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL:
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY:
        case ACHIEVEMENT_CRITERIA_TYPE_LOSE_DUEL:
        case ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_QUEST_REWARD:
        case ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_VENDORS:
        case ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_TALENT_RESETS:
        case ACHIEVEMENT_CRITERIA_TYPE_QUEST_ABANDONED:
        case ACHIEVEMENT_CRITERIA_TYPE_REACH_GUILD_LEVEL:
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED:
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED:
        case ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL:
        case ACHIEVEMENT_CRITERIA_TYPE_TOTAL_DAMAGE_RECEIVED:
        case ACHIEVEMENT_CRITERIA_TYPE_TOTAL_HEALING_RECEIVED:
        case ACHIEVEMENT_CRITERIA_TYPE_USE_LFD_TO_GROUP_WITH_PLAYERS:
        case ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP:
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL:
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA:
        case ACHIEVEMENT_CRITERIA_TYPE_WON_AUCTIONS:
        case ACHIEVEMENT_CRITERIA_TYPE_COOK_SOME_MEALS:
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_CHALLENGE_DUNGEON:
            if (!p_MiscValue1 || p_MiscValue1 != p_Criteria->ChallengeDungeon.MapID)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT:
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY:
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT:
        case ACHIEVEMENT_CRITERIA_TYPE_EARNED_PVP_TITLE:
        case ACHIEVEMENT_CRITERIA_TYPE_EARN_ACHIEVEMENT_POINTS:
        case ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION:
        case ACHIEVEMENT_CRITERIA_TYPE_GAIN_HONORED_REPUTATION:
        case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REVERED_REPUTATION:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_GOLD_VALUE_OWNED:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_PERSONAL_RATING:
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING:
        case ACHIEVEMENT_CRITERIA_TYPE_KNOWN_FACTIONS:
        case ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL:
        case ACHIEVEMENT_CRITERIA_TYPE_COLLECT_TOYS:
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT:
            m_CompletedAchievementsLock.acquire();
            if (m_completedAchievements.find(p_Criteria->complete_achievement.linkedAchievement) == m_completedAchievements.end())
            {
                m_CompletedAchievementsLock.release();
                return false;
            }
            m_CompletedAchievementsLock.release();
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_BG:
            if (!p_MiscValue1 || !p_ReferencePlayer || p_Criteria->win_bg.MapID != p_ReferencePlayer->GetMapId())
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE:
            if (!p_MiscValue1 || p_Criteria->kill_creature.CreatureID != p_MiscValue1)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_LEVELUP_BATTLEPET:
            if (p_MiscValue1 && p_MiscValue1 != p_Criteria->BattlePetLevelUp.RequiredLevel)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL:
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL:
            // update at loading or specific skill update
            if (p_MiscValue1 && p_MiscValue1 != p_Criteria->reach_skill_level.skillID)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE:
            if (p_MiscValue1 && p_MiscValue1 != p_Criteria->complete_quests_in_zone.zoneID)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND:
            if (!p_MiscValue1 || !p_ReferencePlayer || p_ReferencePlayer->GetMapId() != p_Criteria->complete_battleground.mapID)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP:
            if (!p_MiscValue1 || !p_ReferencePlayer ||p_ReferencePlayer->GetMapId() != p_Criteria->death_at_map.mapID)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_DEATH:
        {
            if (!p_MiscValue1)
                return false;
            break;
        }
        case ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON:
        {
            if (!p_MiscValue1)
                return false;

            break;
        }
        case ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE:
            if (!p_MiscValue1 || p_MiscValue1 != p_Criteria->killed_by_creature.creatureEntry)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_PLAYER:
            if (!p_MiscValue1 || !p_Unit || p_Unit->GetTypeId() != TYPEID_PLAYER)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM:
            if (!p_MiscValue1 || p_MiscValue2 != p_Criteria->death_from.type)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:
        {
            // if miscValues != 0, it contains the questID.
            if (p_MiscValue1)
            {
                if (p_MiscValue1 != p_Criteria->complete_quest.questID)
                    return false;
            }
            else
            {
                // login case.
                if (!p_ReferencePlayer->GetQuestRewardStatus(p_Criteria->complete_quest.questID))
                    return false;
            }

            if (AchievementCriteriaDataSet const* l_Data = sAchievementMgr->GetCriteriaDataSet(p_Criteria))
            {
                if (p_Unit && !l_Data->Meets(p_ReferencePlayer, p_Unit))
                    return false;
            }

            break;
        }
        case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET:
        case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2:
            if (!p_MiscValue1 || p_MiscValue1 != p_Criteria->be_spell_target.spellID)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL:
        case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2:
            if (!p_MiscValue1 || p_MiscValue1 != p_Criteria->cast_spell.spellID)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL:
            if (p_MiscValue1 && p_MiscValue1 != p_Criteria->learn_spell.spellID)
                return false;

            if (!p_ReferencePlayer || !p_ReferencePlayer->HasSpell(p_Criteria->learn_spell.spellID))
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE:
            // miscValue1 = itemId - miscValue2 = count of item loot
            // miscValue3 = loot_type (note: 0 = LOOT_CORPSE and then it ignored)
            if (!p_MiscValue1 || !p_MiscValue2 || !p_MiscValue3 || p_MiscValue3 != p_Criteria->loot_type.lootType)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM:
            if (p_MiscValue1 && p_Criteria->own_item.itemID != p_MiscValue1)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM:
            if (!p_MiscValue1 || p_Criteria->use_item.itemID != p_MiscValue1)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM:
            if (!p_MiscValue1 || p_MiscValue1 != p_Criteria->own_item.itemID)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA:
        {
            if (!p_ReferencePlayer)
                return false;

            WorldMapOverlayEntry const* l_WorldMapOverlay = sWorldMapOverlayStore.LookupEntry(p_Criteria->explore_area.areaReference);
            if (!l_WorldMapOverlay)
                break;

            bool l_MatchFound = false;
            for (int l_Index = 0; l_Index < MAX_WORLD_MAP_OVERLAY_AREA_IDX; ++l_Index)
            {
                uint32 l_AreaID = l_WorldMapOverlay->areatableID[l_Index];
                if (!l_AreaID)                            // array have 0 only in empty tail
                    break;

                int32 l_ExploreFlags = GetAreaFlagByAreaID(l_AreaID);

                // Hack: Explore Southern Barrens
                if (p_Criteria->explore_area.areaReference == 3009)
                    l_ExploreFlags = 515;

                if (l_ExploreFlags < 0)
                    continue;

                uint32 l_PlrIndexOffset = uint32(l_ExploreFlags) / 32;
                uint32 l_Mask = 1 << (uint32(l_ExploreFlags) % 32);

                if (p_ReferencePlayer->GetUInt32Value(PLAYER_FIELD_EXPLORED_ZONES + l_PlrIndexOffset) & l_Mask)
                {
                    l_MatchFound = true;
                    break;
                }
            }

            if (!l_MatchFound)
                return false;
            break;
        }
        case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION:
            if (p_MiscValue1 && p_MiscValue1 != p_Criteria->gain_reputation.factionID)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM:
            // miscValue1 = itemid miscValue2 = itemSlot
            if (!p_MiscValue1 || p_MiscValue2 != p_Criteria->equip_epic_item.itemSlot)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT:
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT:
        {
            // miscValue1 = itemid miscValue2 = diced value
            if (!p_MiscValue1 || p_MiscValue2 != p_Criteria->roll_greed_on_loot.rollValue)
                return false;

            ItemTemplate const* l_ItemTemplate = sObjectMgr->GetItemTemplate(uint32(p_MiscValue1));
            if (!l_ItemTemplate)
                return false;
            break;
        }
        case ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE:
            if (!p_MiscValue1 || p_MiscValue1 != p_Criteria->do_emote.emoteID)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_DAMAGE_DONE:
        case ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE:
            if (!p_MiscValue1)
                return false;
            // map specific case (BG in fact) expected player targeted damage/heal
            if (!p_Unit || p_Unit->GetTypeId() != TYPEID_PLAYER)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM:
            // miscValue1 = item_id
            if (!p_MiscValue1 || p_MiscValue1 != p_Criteria->equip_item.itemID)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT:
            if (!p_MiscValue1 || p_MiscValue1 != p_Criteria->use_gameobject.goEntry)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT:
            if (!p_MiscValue1 || p_MiscValue1 != p_Criteria->fish_in_gameobject.goEntry)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS:
            if (p_MiscValue1 && p_MiscValue1 != p_Criteria->learn_skillline_spell.skillLine)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_EPIC_ITEM:
        case ACHIEVEMENT_CRITERIA_TYPE_RECEIVE_EPIC_ITEM:
        {
            if (!p_MiscValue1)
                return false;
            ItemTemplate const* l_ItemTemplate = sObjectMgr->GetItemTemplate(uint32(p_MiscValue1));
            if (!l_ItemTemplate)
                return false;
            break;
        }
        case ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLEPET:
            if (p_MiscValue1 && p_MiscValue1 != p_Criteria->CaptureBattlePet.BattlePetEntry)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE:
            if (p_MiscValue1 && p_MiscValue1 != p_Criteria->learn_skill_line.skillLine)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS:
            if (!p_MiscValue1 || p_MiscValue1 != p_Criteria->hk_class.classID)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_HK_RACE:
            if (!p_MiscValue1 || p_MiscValue1 != p_Criteria->hk_race.raceID)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE:
            if (!p_MiscValue1 || p_MiscValue1 != p_Criteria->bg_objective.objectiveId)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA:
            if (!p_MiscValue1 || p_MiscValue1 != p_Criteria->honorable_kill_at_area.areaID)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_CURRENCY:
            if (!p_MiscValue1 || !p_MiscValue2 || int64(p_MiscValue2) < 0
                || p_MiscValue1 != p_Criteria->currencyGain.currency)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA:
            if (p_MiscValue1 != p_Criteria->win_arena.mapID)
                return false;
            break;
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GUILD_CHALLENGE:
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GUILD_CHALLENGE_TYPE:
            return false;
        case ACHIEVEMENT_CRITERIA_TYPE_DEFEAT_ENCOUNTER:
            if (!p_MiscValue1 || p_MiscValue1 != p_Criteria->DefeatEncounter.EncounterID)
                return false;
        default:
            break;
    }
    return true;
}

template<class T>
bool AchievementMgr<T>::AdditionalRequirementsSatisfied(CriteriaEntry const* p_Criteria, uint64 p_MiscValue1, uint64 p_MiscValue2, Unit const* p_Unit, Player* p_ReferencePlayer) const
{
    ModifierTreeEntry const* l_Condition = sModifierTreeStore.LookupEntry(p_Criteria->ModifierTreeId);
    if (!l_Condition)
        return true;

    ModifierTreeEntryList const& l_List = sAchievementMgr->GetModifierTreeByModifierId(l_Condition->ID);
    if (l_List.empty())
        return true;

    for (ModifierTreeEntryList::const_iterator l_Iter = l_List.begin(); l_Iter != l_List.end(); l_Iter++)
    {
        ModifierTreeEntry const* l_ModifierTree = (*l_Iter);
        uint32 l_ReqType = l_ModifierTree->Type;
        uint32 l_ReqValue = l_ModifierTree->Asset;
        uint32 l_SecondValue = l_ModifierTree->SecondaryAsset;

        switch (AchievementCriteriaAdditionalCondition(l_ReqType))
        {
            case CRITERIA_CONDITION_SOURCE_DRUNK_VALUE:                 // 1
                if (!p_ReferencePlayer || p_ReferencePlayer->GetDrunkValue() < l_ReqValue)
                    return false;
                break;
            case CRITERIA_CONDITION_ITEM_LEVEL:                         // 2
            {
                ItemTemplate const* l_ItemTemplate = sObjectMgr->GetItemTemplate(p_MiscValue1);
                if (!l_ItemTemplate || l_ItemTemplate->ItemLevel < l_ReqValue)
                    return false;
                break;
            }
            case CRITERIA_CONDITION_TARGET_CREATURE_ENTRY:              // 4
                if (!p_Unit || p_Unit->GetEntry() != l_ReqValue)
                    return false;
                break;
            case CRITERIA_CONDITION_TARGET_MUST_BE_PLAYER:              // 5
                if (!p_Unit || p_Unit->GetTypeId() != TYPEID_PLAYER)
                    return false;
                break;
            case CRITERIA_CONDITION_TARGET_MUST_BE_DEAD:                // 6
                if (!p_Unit || p_Unit->isAlive())
                    return false;
                break;
            case CRITERIA_CONDITION_TARGET_MUST_BE_ENEMY:               // 7
                if (!p_Unit || !p_ReferencePlayer->IsHostileTo(p_Unit))
                    return false;
                break;
            case CRITERIA_CONDITION_SOURCE_HAS_AURA:                    // 8
                if (!p_ReferencePlayer->HasAura(l_ReqValue))
                    return false;
                break;
            case CRITERIA_CONDITION_TARGET_HAS_AURA:                    // 10
                if (!p_Unit || !p_Unit->HasAura(l_ReqValue))
                    return false;
                break;
            case CRITERIA_CONDITION_TARGET_HAS_AURA_TYPE:               // 11
                if (!p_Unit || !p_Unit->HasAuraType(AuraType(l_ReqValue)))
                    return false;
                break;
            case CRITERIA_CONDITION_ITEM_QUALITY_MIN:                   // 14
            {
                ItemTemplate const* l_ItemTemplate = sObjectMgr->GetItemTemplate(uint32(p_MiscValue1));
                if (!l_ItemTemplate || l_ItemTemplate->Quality < l_ReqValue)
                    return false;
                break;
            }
            case CRITERIA_CONDITION_ITEM_QUALITY_EQUALS:                // 15
            {
                ItemTemplate const* const l_ItemTemplate = sObjectMgr->GetItemTemplate(uint32(p_MiscValue1));
                if (!l_ItemTemplate || l_ItemTemplate->Quality != l_ReqValue)
                    return false;
                break;
            }
            case CRITERIA_CONDITION_SOURCE_AREA_OR_ZONE:                // 17
            {
                uint32 l_ZoneID, l_AreaID;
                p_ReferencePlayer->GetZoneAndAreaId(l_ZoneID, l_AreaID);
                if (l_ZoneID != l_ReqValue && l_AreaID != l_ReqValue)
                    return false;
                break;
            }
            case CRITERIA_CONDITION_TARGET_AREA_OR_ZONE:                // 18
            {
                if (!p_Unit)
                    return false;
                uint32 l_ZoneID, l_AreaID;
                p_Unit->GetZoneAndAreaId(l_ZoneID, l_AreaID);
                if (l_ZoneID != l_ReqValue && l_AreaID != l_ReqValue)
                    return false;
                break;
            }
            case CRITERIA_CONDITION_LEGACY_RAID_TYPE:                     // 20
            {
                if (!p_ReferencePlayer || uint32(p_ReferencePlayer->GetMap()->GetLegacyRaidType()) != (l_ReqValue))
                    return false;
                break;
            }
            case CRITERIA_CONDITION_TARGET_CREATURE_YIELDS_XP:          // 21
                if (!p_Unit || !p_ReferencePlayer || p_Unit->getLevel() + 5 <= p_ReferencePlayer->getLevel())
                    return false;
                break;
            case CRITERIA_CONDITION_ARENA_TYPE:                         // 24
            {
                if (!p_ReferencePlayer || !p_Unit || p_Unit->GetTypeId() != TYPEID_PLAYER)
                    return false;
                Battleground* l_PlrBattleground = p_ReferencePlayer->GetBattleground();
                Battleground* l_UnitBattleground = p_Unit->ToPlayer()->GetBattleground();
                if (!l_PlrBattleground || !l_PlrBattleground->isArena() || !l_UnitBattleground || !l_UnitBattleground->isArena())
                    return false;
                if ((!l_PlrBattleground->GetArenaType()) != l_ReqValue || !l_UnitBattleground->GetArenaType() != l_ReqValue)
                    return false;
                break;
            }
            case CRITERIA_CONDITION_SOURCE_RACE:                        // 25
                if (p_ReferencePlayer->getRace() != l_ReqValue)
                    return false;
                break;
            case CRITERIA_CONDITION_SOURCE_CLASS:                       // 26
                if (p_ReferencePlayer->getClass() != l_ReqValue)
                    return false;
                break;
            case CRITERIA_CONDITION_TARGET_RACE:                        // 27
                if (!p_Unit || p_Unit->GetTypeId() != TYPEID_PLAYER || p_Unit->getRace() != l_ReqValue)
                    return false;
                break;
            case CRITERIA_CONDITION_TARGET_CLASS:                       // 28
                if (!p_Unit || p_Unit->GetTypeId() != TYPEID_PLAYER || p_Unit->getClass() != l_ReqValue)
                    return false;
                break;
            case CRITERIA_CONDITION_MAX_GROUP_MEMBERS:                  // 29
                if (!p_ReferencePlayer->GetGroup() || p_ReferencePlayer->GetGroup()->GetMembersCount() >= l_ReqValue)
                    return false;
                break;
            case CRITERIA_CONDITION_TARGET_CREATURE_TYPE:               // 30
            {
                if (!p_Unit)
                    return false;

                // We must return true here, only one guild achievement and one player statistic which have this condition
                // This condition is CREATURE_TYPE_CRITER, but the other one is BattlePet type, which is not used yet
                Creature const* l_Creature = p_Unit->ToCreature();
                if (!l_Creature || l_Creature->GetCreatureType() != l_ReqValue)
                    return true;
                break;
            }
            case CRITERIA_CONDITION_SOURCE_MAP:                         // 32
                if (p_ReferencePlayer->GetMapId() != l_ReqValue)
                    return false;
                break;
            case CRITERIA_CONDITION_BUILD_VERSION:                      // 33
                if ((l_ReqValue - 33000) >= 19116)
                    return false;
                break;
            case CRITERIA_CONDITION_BATTLEPET_TEAM_LEVEL:               // 34
                if (!p_ReferencePlayer)
                    return false;
                break;
            case CRITERIA_CONDITION_COMPLETE_QUEST_NOT_IN_GROUP:        // 35
                if (!p_ReferencePlayer || p_ReferencePlayer->GetGroup())
                    return false;
                break;
            case CRITERIA_CONDITION_MIN_PERSONAL_RATING:                // 37
            {
                if (!p_ReferencePlayer)
                    return false;

                bool l_IsOkay = false;
                for (uint8 l_Iter = 0; l_Iter < MAX_ARENA_SLOT; ++l_Iter)
                {
                    if (p_ReferencePlayer->GetArenaPersonalRating(l_Iter) >= l_ReqValue)
                        l_IsOkay = true;
                }

                if (!l_IsOkay)
                    return false;

                break;
            }
            case CRITERIA_CONDITION_TITLE_BIT_INDEX:                    // 38
                if (p_MiscValue1 != l_ReqValue)
                    return false;
                break;
            case CRITERIA_CONDITION_SOURCE_LEVEL:                       // 39
                if (p_ReferencePlayer->getLevel() != l_ReqValue)
                    return false;
                break;
            case CRITERIA_CONDITION_TARGET_LEVEL:                       // 40
                if (!p_Unit || p_Unit->getLevel() != l_ReqValue)
                    return false;
                break;
            case CRITERIA_CONDITION_TARGET_ZONE:                        // 41
                if (!p_Unit || p_Unit->GetZoneId() != l_ReqValue)
                    return false;
            case CRITERIA_CONDITION_TARGET_HEALTH_PERCENT_BELOW:        // 46
                if (!p_Unit || p_Unit->GetHealthPct() >= l_ReqValue)
                    return false;
                break;
            case CRITERIA_CONDITION_MIN_ACHIEVEMENT_POINTS:             // 56
                if (!p_ReferencePlayer || (p_ReferencePlayer->GetAchievementMgr().GetAchievementPoints() < l_ReqValue))
                    return false;
                break;
            case CRITERIA_CONDITION_REQUIRES_LFG_GROUP:                 // 58
                if (!p_ReferencePlayer || !p_ReferencePlayer->GetGroup() || !p_ReferencePlayer->GetGroup()->isLFGGroup())
                    return false;
                break;
            case CRITERIA_CONDITION_BE_THE_LAST_SURVIVOR_5V5:           // 60
            {
                if (!p_ReferencePlayer || !p_ReferencePlayer->GetBattleground())
                    return false;

                Battleground* l_Battleground = p_ReferencePlayer->GetBattleground();
                if (!l_Battleground->isArena())
                    return false;

                bool l_LastOne = true;
                Battleground::BattlegroundPlayerMap const& l_PlayerList = l_Battleground->GetPlayers();
                if (l_PlayerList.empty())
                    return false;

                for (Battleground::BattlegroundPlayerMap::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
                {
                    if (Player* l_Player = Player::GetPlayer(*p_ReferencePlayer, l_Iter->first))
                    {
                        if (l_Player == p_ReferencePlayer)
                            continue;

                        if (l_Player->isAlive())
                            l_LastOne = false;
                    }
                }

                if (!l_LastOne)
                    return false;
                break;
            }
            case CRITERIA_CONDITION_REQUIRES_GUILD_GROUP:               // 61
            {
                if (!p_ReferencePlayer)
                    return false;

                Group* l_Group = p_ReferencePlayer->GetGroup();
#ifndef CROSS
                Guild* l_Guild = p_ReferencePlayer->GetGuild();
                if (!l_Group || !l_Guild)
#else
                auto l_GuildId = p_ReferencePlayer->GetGuildId();
                if (!l_Group || !l_GuildId)
#endif
                    return false;

#ifndef CROSS
                if (!l_Group->IsGuildGroup(l_Guild->GetId(), true, true))
#else
                if (!l_Group->IsGuildGroup(l_GuildId, true, true))
#endif
                    return false;

                break;
            }
            case CRITERIA_CONDITION_GUILD_REPUTATION:                   // 62
                if (!p_ReferencePlayer || !p_ReferencePlayer->GetGuild())
                    return false;
                if (p_ReferencePlayer->GetReputation(REP_GUILD) < l_ReqValue)
                    return false;
                break;
            case CRITERIA_CONDITION_RATED_BATTLEGROUND:                 // 63
                if (!p_ReferencePlayer || !p_ReferencePlayer->GetBattleground() || !p_ReferencePlayer->GetBattleground()->IsRatedBG())
                    return false;
                break;
            case CRITERIA_CONDITION_PROJECT_RARITY:                     // 65
            {
                if (!p_MiscValue1 || !p_ReferencePlayer)
                    return false;

                ResearchProjectEntry const* l_Project = sResearchProjectStore.LookupEntry(p_MiscValue1);
                if (!l_Project || l_Project->rare != l_ReqValue)
                    return false;
                break;
            }
            case CRITERIA_CONDITION_PROJECT_RACE:                       // 66
            {
                if (!p_MiscValue1 || !p_ReferencePlayer)
                    return false;

                ResearchProjectEntry const* l_Project = sResearchProjectStore.LookupEntry(p_MiscValue1);
                if (!l_Project || l_Project->branchId != l_ReqValue)
                    return false;
                break;
            }
            case CRITERIA_CONDITION_WORLD_STATE_EXPRESSION:
            {
                WorldStateExpressionEntry const* l_Expression = sWorldStateExpressionStore.LookupEntry(l_ReqValue);

                if (!l_Expression || !l_Expression->Eval(p_ReferencePlayer))
                    return false;

                break;
            }
            case CRITERIA_CONDITION_DIFFICULTY:                    // 68
                if (!p_ReferencePlayer || p_ReferencePlayer->GetMap()->GetDifficultyID() != Difficulty(l_ReqValue))
                    return false;
                break;
            case CRITERIA_CONDITION_TARGET_MIN_LEVEL:                   // 70
                if (!p_Unit || p_Unit->getLevel() < l_ReqValue)
                    return false;
                break;
            case CRITERIA_CONDITION_BATTLEPET_TYPE:
                if (!p_ReferencePlayer || !p_MiscValue2)
                    return false;
                if (p_MiscValue2 != l_ReqValue)
                    return false;
                break;
            case CRITERIA_CONDITION_CAPTURE_BATTLEPET_ABOVE_HEALT_PCT:  // 79
                return false;
            case CRITERIA_CONDITION_COUNT_OF_GUILD_MEMBER_IN_GROUP:     // 80
            {
                if (!p_ReferencePlayer)
                    return false;

#ifndef CROSS
                Group* l_Group = p_ReferencePlayer->GetGroup();
                Guild* l_Guild = p_ReferencePlayer->GetGuild();
                if (!l_Guild || !l_Group)
#else
                Group* l_Group   = p_ReferencePlayer->GetGroup();
                uint32 l_GuildId = p_ReferencePlayer->GetGuildId();
                if (!l_GuildId || !l_Group)
#endif
                    return false;

                uint32 l_Counter = 0;
                for (GroupReference* l_GroupRef = l_Group->GetFirstMember(); l_GroupRef != NULL; l_GroupRef = l_GroupRef->next())
                {
                    if (Player* l_Player = l_GroupRef->getSource())
                    {
#ifndef CROSS
                        if (l_Player->GetGuildId() == l_Guild->GetId())
#else
                        if (l_Player->GetGuildId() == l_GuildId)
#endif
                            ++l_Counter;
                    }
                }

                if (l_Counter < l_ReqValue)
                    return false;

                break;
            }
            case CRITERIA_CONDITION_TARGET_IS_BATTLEPET_MASTER:         // 81
                return false;
            case CRITERIA_CONDITION_NEED_CHALLENGE_MEDAL:               // 83
                if (p_MiscValue2 >= l_ReqValue)
                    return true;
                return false;
            case CRITERIA_CONDITION_BATTLEPET_MUST_BE_RARE:             // 89
                if (!p_MiscValue2 || (p_MiscValue2 < (l_ReqValue - 7)))
                    return false;
                break;
            case CRITERIA_CONDITION_WIN_PVP_PETBATTLE:                  // 90
                return false;
                /*
                l_ReqValue -> Need level 25 team
                */
            case CRITERIA_CONDITION_CAPTURE_PETBATTLE:
            {
                if (!p_MiscValue1)
                    return false;

                BattlePetSpeciesEntry const* l_SpeciesInfo = sBattlePetSpeciesStore.LookupEntry(p_MiscValue1);
                if (!l_SpeciesInfo)
                    return false;

                if (l_SpeciesInfo->id != l_ReqValue)
                    return false;
                break;
            }
            case CRITERIA_CONDITION_CRAFT_AMOUNT_OF_ITEM:               // 96
            {
                if (!p_ReferencePlayer || !p_ReferencePlayer->GetGuild())
                    return false;

                ItemTemplate const* l_ItemTemplate = sObjectMgr->GetItemTemplate(p_MiscValue1);
                if (!l_ItemTemplate)
                    return false;

                if (l_ItemTemplate->Class != l_ReqValue)
                    return false;
                break;
            }
            case CRITERIA_CONDITION_EARN_CURRENCY_DURING_ARENA_SEASON:  // 121
                if (!p_ReferencePlayer || p_ReferencePlayer->GetCurrency(l_ReqValue, true) < l_SecondValue)
                    return false;
                if (sWorld->getIntConfig(CONFIG_ARENA_SEASON_ID) != p_Criteria->EligibilityWorldStateValue)
                    return false;
                break;
            case CRITERIA_CONDITION_REQUIRE_DEATH_IN_DUNGEON_OR_RAID:   // 122
                if (!p_ReferencePlayer || p_ReferencePlayer->isAlive())
                    return false;
                break;
            default:
                break;
        }
    }
    return true;
}

char const* AchievementGlobalMgr::GetCriteriaTypeString(uint32 p_Type)
{
    switch (AchievementCriteriaTypes(p_Type))
    {
        case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE:
            return "KILL_CREATURE";
        case ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLEPET:
            return "CAPTURE_BATTLEPET";
        case ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_SPECIFIC_BATTLEPET:
            return "CAPTURE_SPECIFIC_BATTLEPET";
        case ACHIEVEMENT_CRITERIA_TYPE_EARN_BATTLEPET:
            return "EARN_BATTLEPET";
        case ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLEPET_IN_COMBAT:
            return "CAPTURE_BATTLEPET_IN_COMBAT";
        case ACHIEVEMENT_CRITERIA_TYPE_LEVELUP_BATTLEPET:
            return "LEVELUP_BATTLEPET";
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_PETBATTLE:
            return "WIN_PETBATTLE";
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_BG:
            return "TYPE_WIN_BG";
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ARCHAEOLOGY_PROJECTS:
            return "COMPLETE_ARCHAEOLOGY_PROJECT";
        case ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL:
            return "REACH_LEVEL";
        case ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL:
            return "REACH_SKILL_LEVEL";
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT:
            return "COMPLETE_ACHIEVEMENT";
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT:
            return "COMPLETE_QUEST_COUNT";
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY:
            return "COMPLETE_DAILY_QUEST_DAILY";
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE:
            return "COMPLETE_QUESTS_IN_ZONE";
        case ACHIEVEMENT_CRITERIA_TYPE_CURRENCY:
            return "CURRENCY";
        case ACHIEVEMENT_CRITERIA_TYPE_DAMAGE_DONE:
            return "DAMAGE_DONE";
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST:
            return "COMPLETE_DAILY_QUEST";
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND:
            return "COMPLETE_BATTLEGROUND";
        case ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP:
            return "DEATH_AT_MAP";
        case ACHIEVEMENT_CRITERIA_TYPE_DEATH:
            return "DEATH";
        case ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON:
            return "DEATH_IN_DUNGEON";
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID:
            return "COMPLETE_RAID";
        case ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE:
            return "KILLED_BY_CREATURE";
        case ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_PLAYER:
            return "KILLED_BY_PLAYER";
        case ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING:
            return "FALL_WITHOUT_DYING";
        case ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM:
            return "DEATHS_FROM";
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST:
            return "COMPLETE_QUEST";
        case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET:
            return "BE_SPELL_TARGET";
        case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL:
            return "CAST_SPELL";
        case ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE:
            return "BG_OBJECTIVE_CAPTURE";
        case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA:
            return "HONORABLE_KILL_AT_AREA";
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA:
            return "WIN_ARENA";
        case ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA:
            return "PLAY_ARENA";
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL:
            return "LEARN_SPELL";
        case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL:
            return "HONORABLE_KILL";
        case ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM:
            return "OWN_ITEM";
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA:
            return "WIN_RATED_ARENA";
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING:
            return "HIGHEST_TEAM_RATING";
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_PERSONAL_RATING:
            return "HIGHEST_PERSONAL_RATING";
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL:
            return "LEARN_SKILL_LEVEL";
        case ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM:
            return "USE_ITEM";
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM:
            return "LOOT_ITEM";
        case ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA:
            return "EXPLORE_AREA";
        case ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK:
            return "OWN_RANK";
        case ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT:
            return "BUY_BANK_SLOT";
        case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION:
            return "GAIN_REPUTATION";
        case ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION:
            return "GAIN_EXALTED_REPUTATION";
        case ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP:
            return "VISIT_BARBER_SHOP";
        case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM:
            return "EQUIP_EPIC_ITEM";
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT:
            return "ROLL_NEED_ON_LOOT";
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT:
            return "GREED_ON_LOOT";
        case ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS:
            return "HK_CLASS";
        case ACHIEVEMENT_CRITERIA_TYPE_HK_RACE:
            return "HK_RACE";
        case ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE:
            return "DO_EMOTE";
        case ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE:
            return "HEALING_DONE";
        case ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS:
            return "GET_KILLING_BLOWS";
        case ACHIEVEMENT_CRITERIA_TYPE_COOK_SOME_MEALS:
            return "COOK_SOME_MEALS";
        case ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM:
            return "EQUIP_ITEM";
        case ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_VENDORS:
            return "MONEY_FROM_VENDORS";
        case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TALENTS:
            return "GOLD_SPENT_FOR_TALENTS";
        case ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_TALENT_RESETS:
            return "NUMBER_OF_TALENT_RESETS";
        case ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_QUEST_REWARD:
            return "MONEY_FROM_QUEST_REWARD";
        case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TRAVELLING:
            return "GOLD_SPENT_FOR_TRAVELLING";
        case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_AT_BARBER:
            return "GOLD_SPENT_AT_BARBER";
        case ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_MAIL:
            return "GOLD_SPENT_FOR_MAIL";
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY:
            return "LOOT_MONEY";
        case ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT:
            return "USE_GAMEOBJECT";
        case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2:
            return "BE_SPELL_TARGET2";
        case ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL:
            return "SPECIAL_PVP_KILL";
        case ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT:
            return "FISH_IN_GAMEOBJECT";
        case ACHIEVEMENT_CRITERIA_TYPE_EARNED_PVP_TITLE:
            return "EARNED_PVP_TITLE";
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS:
            return "LEARN_SKILLLINE_SPELLS";
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL:
            return "WIN_DUEL";
        case ACHIEVEMENT_CRITERIA_TYPE_LOSE_DUEL:
            return "LOSE_DUEL";
        case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE:
            return "KILL_CREATURE_TYPE";
        case ACHIEVEMENT_CRITERIA_TYPE_GOLD_EARNED_BY_AUCTIONS:
            return "GOLD_EARNED_BY_AUCTIONS";
        case ACHIEVEMENT_CRITERIA_TYPE_ACHIEVEMENTS_IN_BATTLE_PET:
            return "ACHIEVEMENTS_IN_BATTLEPET";
        case ACHIEVEMENT_CRITERIA_TYPE_CREATE_AUCTION:
            return "CREATE_AUCTION";
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_BID:
            return "HIGHEST_AUCTION_BID";
        case ACHIEVEMENT_CRITERIA_TYPE_WON_AUCTIONS:
            return "WON_AUCTIONS";
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_SOLD:
            return "HIGHEST_AUCTION_SOLD";
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_GOLD_VALUE_OWNED:
            return "HIGHEST_GOLD_VALUE_OWNED";
        case ACHIEVEMENT_CRITERIA_TYPE_GAIN_REVERED_REPUTATION:
            return "GAIN_REVERED_REPUTATION";
        case ACHIEVEMENT_CRITERIA_TYPE_GAIN_HONORED_REPUTATION:
            return "GAIN_HONORED_REPUTATION";
        case ACHIEVEMENT_CRITERIA_TYPE_KNOWN_FACTIONS:
            return "KNOWN_FACTIONS";
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_EPIC_ITEM:
            return "LOOT_EPIC_ITEM";
        case ACHIEVEMENT_CRITERIA_TYPE_RECEIVE_EPIC_ITEM:
            return "RECEIVE_EPIC_ITEM";
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED:
            return "ROLL_NEED";
        case ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED:
            return "ROLL_GREED";
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_DEALT:
            return "HIT_DEALT";
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HIT_RECEIVED:
            return "HIT_RECEIVED";
        case ACHIEVEMENT_CRITERIA_TYPE_TOTAL_DAMAGE_RECEIVED:
            return "TOTAL_DAMAGE_RECEIVED";
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEAL_CASTED:
            return "HIGHEST_HEAL_CASTED";
        case ACHIEVEMENT_CRITERIA_TYPE_TOTAL_HEALING_RECEIVED:
            return "TOTAL_HEALING_RECEIVED";
        case ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_HEALING_RECEIVED:
            return "HIGHEST_HEALING_RECEIVED";
        case ACHIEVEMENT_CRITERIA_TYPE_QUEST_ABANDONED:
            return "QUEST_ABANDONED";
        case ACHIEVEMENT_CRITERIA_TYPE_FLIGHT_PATHS_TAKEN:
            return "FLIGHT_PATHS_TAKEN";
        case ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE:
            return "LOOT_TYPE";
        case ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2:
            return "CAST_SPELL2";
        case ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE:
            return "LEARN_SKILL_LINE";
        case ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL:
            return "EARN_HONORABLE_KILL";
        case ACHIEVEMENT_CRITERIA_TYPE_ACCEPTED_SUMMONINGS:
            return "ACCEPTED_SUMMONINGS";
        case ACHIEVEMENT_CRITERIA_TYPE_EARN_ACHIEVEMENT_POINTS:
            return "EARN_ACHIEVEMENT_POINTS";
        case ACHIEVEMENT_CRITERIA_TYPE_USE_LFD_TO_GROUP_WITH_PLAYERS:
            return "USE_LFD_TO_GROUP_WITH_PLAYERS";
        case ACHIEVEMENT_CRITERIA_TYPE_SPENT_GOLD_GUILD_REPAIRS:
            return "SPENT_GOLD_GUILD_REPAIRS";
        case ACHIEVEMENT_CRITERIA_TYPE_REACH_GUILD_LEVEL:
            return "REACH_GUILD_LEVEL";
        case ACHIEVEMENT_CRITERIA_TYPE_CRAFT_ITEMS_GUILD:
            return "CRAFT_ITEMS_GUILD";
        case ACHIEVEMENT_CRITERIA_TYPE_CATCH_FROM_POOL:
            return "CATCH_FROM_POOL";
        case ACHIEVEMENT_CRITERIA_TYPE_BUY_GUILD_BANK_SLOTS:
            return "BUY_GUILD_BANK_SLOTS";
        case ACHIEVEMENT_CRITERIA_TYPE_EARN_GUILD_ACHIEVEMENT_POINTS:
            return "EARN_GUILD_ACHIEVEMENT_POINTS";
        case ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_BATTLEGROUND:
            return "WIN_RATED_BATTLEGROUND";
        case ACHIEVEMENT_CRITERIA_TYPE_REACH_BG_RATING:
            return "REACH_BG_RATING";
        case ACHIEVEMENT_CRITERIA_TYPE_BUY_GUILD_TABARD:
            return "BUY_GUILD_TABARD";
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_GUILD:
            return "COMPLETE_QUESTS_GUILD";
        case ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILLS_GUILD:
            return "HONORABLE_KILLS_GUILD";
        case ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE_GUILD:
            return "KILL_CREATURE_TYPE_GUILD";
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GUILD_CHALLENGE_TYPE:
            return "GUILD_CHALLENGE_TYPE";
        case ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GUILD_CHALLENGE:
            return "GUILD_CHALLENGE";
        case ACHIEVEMENT_CRITERIA_TYPE_COLLECT_TOYS:
            return "COLLECT_TOYS";
        default:
            break;
    }
    return "MISSING_TYPE";
}

#ifndef CROSS
template class AchievementMgr<Guild>;
#endif
template class AchievementMgr<Player>;

//==========================================================
AchievementEntry const* AchievementGlobalMgr::_GetAchievementEntryByCriteriaTree(CriteriaTreeEntry const* p_Criteria) const
{
    while (true)
    {
        if (!p_Criteria->Parent || p_Criteria->Parent == p_Criteria->ID)
            return m_AchievementEntryByCriteriaTree[p_Criteria->ID];

        p_Criteria = sCriteriaTreeStore.LookupEntry(p_Criteria->Parent);
    }
    return NULL;
}

void AchievementGlobalMgr::LoadAchievementCriteriaList()
{
    uint32 l_OldMSTime = getMSTime();

    if (sCriteriaStore.GetNumRows() == 0)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 achievement criteria.");
        return;
    }

    uint32 l_MaxParentID = 0;
    for (uint32 l_EntryID = 0; l_EntryID < sCriteriaTreeStore.GetNumRows(); ++l_EntryID)
    {
        CriteriaTreeEntry const* l_CriteriaTree = sCriteriaTreeStore.LookupEntry(l_EntryID);
        if (!l_CriteriaTree)
            continue;

        if (CriteriaTreeEntry const* l_Parent = sCriteriaTreeStore.LookupEntry(l_CriteriaTree->Parent))
        {
            if (l_Parent->ID > l_MaxParentID)
                l_MaxParentID = l_Parent->ID;
        }
    }

    m_SubCriteriaTreeListById.resize(l_MaxParentID + 1);
    m_AchievementCriteriaTreeByCriteriaId.resize(sCriteriaStore.GetNumRows() + 1);

    for (uint32 l_EntryID = 0; l_EntryID < sCriteriaTreeStore.GetNumRows(); ++l_EntryID)
    {
        CriteriaTreeEntry const* l_CriteriaTree = sCriteriaTreeStore.LookupEntry(l_EntryID);
        if (!l_CriteriaTree)
            continue;

        if (CriteriaTreeEntry const* l_Parent = sCriteriaTreeStore.LookupEntry(l_CriteriaTree->Parent))
            m_SubCriteriaTreeListById[l_Parent->ID].push_back(l_CriteriaTree);

        CriteriaEntry const* l_CriteriaEntry = sCriteriaStore.LookupEntry(l_CriteriaTree->CriteriaID);
        if (!l_CriteriaEntry)
            continue;

        m_AchievementCriteriaTreeByCriteriaId[l_CriteriaEntry->ID].push_back(l_CriteriaTree);
    }

    uint32 l_MaxModifierTreeParent = 0;
    for (uint32 l_EntryID = 0; l_EntryID < sModifierTreeStore.GetNumRows(); ++l_EntryID)
    {
        ModifierTreeEntry const* l_ModifierTree = sModifierTreeStore.LookupEntry(l_EntryID);
        if (!l_ModifierTree)
            continue;

        if (l_ModifierTree->Parent > l_MaxModifierTreeParent)
            l_MaxModifierTreeParent = l_ModifierTree->Parent;
    }

    m_ModifierTreeEntryByTreeId.resize(l_MaxModifierTreeParent + 1);

    for (uint32 l_EntryID = 0; l_EntryID < sModifierTreeStore.GetNumRows(); ++l_EntryID)
    {
        ModifierTreeEntry const* l_ModifierTree = sModifierTreeStore.LookupEntry(l_EntryID);
        if (!l_ModifierTree)
            continue;

        if (l_ModifierTree->Parent)
            m_ModifierTreeEntryByTreeId[l_ModifierTree->Parent].push_back(l_ModifierTree);
    }

    uint32 l_CriteriaCount = 0;
    for (uint32 l_EntryID = 0; l_EntryID < sCriteriaStore.GetNumRows(); ++l_EntryID)
    {
        CriteriaEntry const* l_Criteria = sAchievementMgr->GetAchievementCriteria(l_EntryID);
        if (!l_Criteria)
            continue;

        m_AchievementCriteriasByType[l_Criteria->Type].push_back(l_Criteria);

        if (l_Criteria->StartTimer)
            m_AchievementCriteriasByTimedType[l_Criteria->StartEvent].push_back(l_Criteria);

        ++l_CriteriaCount;
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u achievement criteria in %u ms", l_CriteriaCount, GetMSTimeDiffToNow(l_OldMSTime));
}

void AchievementGlobalMgr::LoadAchievementReferenceList()
{
    uint32 l_OldMSTime = getMSTime();

    if (sAchievementStore.GetNumRows() == 0)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 achievement references.");
        return;
    }

    uint32 l_MaxCriteriaTree   = 0;
    uint32 l_MaxSharesCriteria = 0;

    for (uint32 l_EntryID = 0; l_EntryID < sAchievementStore.GetNumRows(); ++l_EntryID)
    {
        AchievementEntry const* l_Achievement = sAchievementMgr->GetAchievement(l_EntryID);
        if (!l_Achievement)
            continue;

        if (l_Achievement->CriteriaTree > l_MaxCriteriaTree)
            l_MaxCriteriaTree = l_Achievement->CriteriaTree;

        if (l_Achievement->SharesCriteria > l_MaxSharesCriteria)
            l_MaxSharesCriteria = l_Achievement->SharesCriteria;
    }

    m_AchievementEntryByCriteriaTree.resize(sCriteriaTreeStore.GetNumRows() + 1, nullptr);

    uint32 l_Counter = 0;

    for (uint32 l_EntryID = 0; l_EntryID < sAchievementStore.GetNumRows(); ++l_EntryID)
    {
        AchievementEntry const* l_Achievement = sAchievementMgr->GetAchievement(l_EntryID);
        if (!l_Achievement)
            continue;

        m_AchievementEntryByCriteriaTree[l_Achievement->CriteriaTree] = l_Achievement;

        if (!l_Achievement->SharesCriteria)
            continue;

        m_AchievementListByReferencedId[l_Achievement->SharesCriteria].push_back(l_Achievement);
        ++l_Counter;
    }

    m_AchievementEntryByCriteriaTreeId.resize(sCriteriaTreeStore.GetNumRows() + 1, nullptr);

    for (uint32 l_Entry = 0; l_Entry < sCriteriaTreeStore.GetNumRows(); l_Entry++)
    {
        CriteriaTreeEntry const* l_CriteriaTree = sCriteriaTreeStore.LookupEntry(l_Entry);
        if (l_CriteriaTree == nullptr)
            continue;

        m_AchievementEntryByCriteriaTreeId[l_CriteriaTree->ID] = _GetAchievementEntryByCriteriaTree(l_CriteriaTree);
    }

    // Once Bitten, Twice Shy (10 player) - Icecrown Citadel
    if (AchievementEntry const* l_Achievement = sAchievementMgr->GetAchievement(4539))
        const_cast<AchievementEntry*>(l_Achievement)->InstanceId = 631;    // Correct map requirement (currently has Ulduar)

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u achievement references in %u ms", l_Counter, GetMSTimeDiffToNow(l_OldMSTime));
}

void AchievementGlobalMgr::LoadAchievementCriteriaData()
{
    uint32 l_OldMSTime = getMSTime();

    m_criteriaDataMap.clear();                              // Need for reload case

    QueryResult l_Result = WorldDatabase.Query("SELECT criteria_id, type, value1, value2, ScriptName FROM achievement_criteria_data");

    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 additional achievement criteria data. DB table `achievement_criteria_data` is empty.");
        return;
    }

    uint32 l_Count = 0;

    do
    {
        Field* l_Fields = l_Result->Fetch();
        uint32 l_CriteriaID = l_Fields[0].GetUInt32();

        CriteriaEntry const* l_Criteria = sAchievementMgr->GetAchievementCriteria(l_CriteriaID);
        if (!l_Criteria)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` has data for non-existing criteria (Entry: %u), ignore.", l_CriteriaID);
            continue;
        }

        uint32 l_DataType = l_Fields[1].GetUInt8();
        const char* l_ScriptName = l_Fields[4].GetCString();
        uint32 l_ScriptID = 0;
        if (strcmp(l_ScriptName, "")) // Not empty
        {
            if (l_DataType != ACHIEVEMENT_CRITERIA_DATA_TYPE_SCRIPT)
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_criteria_data` has ScriptName set for non-scripted data type (Entry: %u, type %u), useless data.", l_CriteriaID, l_DataType);
            else
                l_ScriptID = sObjectMgr->GetScriptId(l_ScriptName);
        }

        AchievementCriteriaData l_Data(l_DataType, l_Fields[2].GetUInt32(), l_Fields[3].GetUInt32(), l_ScriptID);
        if (!l_Data.IsValid(l_Criteria))
            continue;

        // This will allocate empty data set storage
        AchievementCriteriaDataSet& l_DataSet = m_criteriaDataMap[l_CriteriaID];
        l_DataSet.SetCriteriaId(l_CriteriaID);

        // Add real data only for not NONE data types
        if (l_Data.dataType != ACHIEVEMENT_CRITERIA_DATA_TYPE_NONE)
            l_DataSet.Add(l_Data);

        // Counting data by and data types
        ++l_Count;
    }
    while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u additional achievement criteria data in %u ms", l_Count, GetMSTimeDiffToNow(l_OldMSTime));
}

void AchievementGlobalMgr::LoadCompletedAchievements()
{
#ifndef CROSS
    uint32 l_OldMSTime = getMSTime();

    QueryResult l_Result = CharacterDatabase.Query("SELECT achievement FROM character_achievement GROUP BY achievement");

    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 completed achievements. DB table `character_achievement` is empty.");
        return;
    }

    do
    {
        Field* l_Fields = l_Result->Fetch();

        uint16 l_AchievementID = l_Fields[0].GetUInt16();
        AchievementEntry const* l_Achievement = sAchievementMgr->GetAchievement(l_AchievementID);
        if (!l_Achievement)
        {
            // Remove non existent achievements from all characters
            sLog->outError(LOG_FILTER_ACHIEVEMENTSYS, "Non-existing achievement %u data removed from table `character_achievement`.", l_AchievementID);

            PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_INVALID_ACHIEVMENT);
            l_Stmt->setUInt16(0, uint16(l_AchievementID));
            CharacterDatabase.Execute(l_Stmt);
            continue;
        }
        else if (l_Achievement->Flags & (ACHIEVEMENT_FLAG_REALM_FIRST_REACH | ACHIEVEMENT_FLAG_REALM_FIRST_KILL))
            m_allCompletedAchievements[l_AchievementID] = uint32(0xFFFFFFFF);
    }
    while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %lu completed achievements in %u ms", (unsigned long)m_allCompletedAchievements.size(), GetMSTimeDiffToNow(l_OldMSTime));
#endif
}

void AchievementGlobalMgr::LoadRewards()
{
    uint32 l_OldMSTime = getMSTime();

    m_achievementRewards.clear();                           // Need for reload case

    //                                                   0      1        2        3     4       5        6        7
    QueryResult l_Result = WorldDatabase.Query("SELECT entry, title_A, title_H, item, SpellID, sender, subject, text FROM achievement_reward");

    if (!l_Result)
    {
        sLog->outError(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 achievement rewards. DB table `achievement_reward` is empty.");
        return;
    }

    uint32 l_Counter = 0;

    do
    {
        uint8 l_Index   = 0;
        Field* l_Fields = l_Result->Fetch();
        uint32 l_Entry  = l_Fields[l_Index++].GetUInt32();

        AchievementEntry const* l_Achievement = GetAchievement(l_Entry);
        if (!l_Achievement)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `achievement_reward` has wrong achievement (Entry: %u), ignored.", l_Entry);
            continue;
        }

        AchievementReward l_Reward;
        l_Reward.titleId[0] = l_Fields[l_Index++].GetUInt32();
        l_Reward.titleId[1] = l_Fields[l_Index++].GetUInt32();
        l_Reward.itemId     = l_Fields[l_Index++].GetUInt32();
        l_Reward.SpellID    = l_Fields[l_Index++].GetUInt32();
        l_Reward.sender     = l_Fields[l_Index++].GetUInt32();
        l_Reward.subject    = l_Fields[l_Index++].GetString();
        l_Reward.text       = l_Fields[l_Index++].GetString();

        // Must be title, spell or mail at least
        if (!l_Reward.titleId[0] && !l_Reward.titleId[1] && !l_Reward.sender && !l_Reward.SpellID)
        {
            sLog->outError(LOG_FILTER_SQL, "Table `achievement_reward` (Entry: %u) does not have title, spell or item reward data, ignored.", l_Entry);
            continue;
        }

        if (l_Achievement->Faction == ACHIEVEMENT_FACTION_ANY && ((l_Reward.titleId[0] == 0) != (l_Reward.titleId[1] == 0)))
            sLog->outError(LOG_FILTER_SQL, "Table `achievement_reward` (Entry: %u) has title (A: %u H: %u) for only one team.", l_Entry, l_Reward.titleId[0], l_Reward.titleId[1]);

        if (l_Reward.titleId[0])
        {
            CharTitlesEntry const* l_TitleEntry = sCharTitlesStore.LookupEntry(l_Reward.titleId[0]);
            if (!l_TitleEntry)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_reward` (Entry: %u) has invalid title id (%u) in `title_A`, set to 0", l_Entry, l_Reward.titleId[0]);
                l_Reward.titleId[0] = 0;
            }
        }

        if (l_Reward.titleId[1])
        {
            CharTitlesEntry const* l_TitleEntry = sCharTitlesStore.LookupEntry(l_Reward.titleId[1]);
            if (!l_TitleEntry)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_reward` (Entry: %u) has invalid title id (%u) in `title_H`, set to 0", l_Entry, l_Reward.titleId[1]);
                l_Reward.titleId[1] = 0;
            }
        }

        // Check mail data before item for report including wrong item case
        if (l_Reward.sender)
        {
            if (!sObjectMgr->GetCreatureTemplate(l_Reward.sender))
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_reward` (Entry: %u) has invalid creature entry %u as sender, mail reward skipped.", l_Entry, l_Reward.sender);
                l_Reward.sender = 0;
            }
        }
        else
        {
            if (l_Reward.itemId)
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_reward` (Entry: %u) does not have sender data but has item reward, item will not be rewarded.", l_Entry);

            if (!l_Reward.subject.empty())
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_reward` (Entry: %u) does not have sender data but has mail subject.", l_Entry);

            if (!l_Reward.text.empty())
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_reward` (Entry: %u) does not have sender data but has mail text.", l_Entry);
        }

        if (l_Reward.itemId)
        {
            ItemTemplate const* l_ItemTemplate = sObjectMgr->GetItemTemplate(l_Reward.itemId);

            if (l_ItemTemplate == nullptr)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_reward` (Entry: %u) has invalid item id %u, reward mail will not contain item.", l_Entry, l_Reward.itemId);
                l_Reward.itemId = 0;
            }

            const_cast<ItemTemplate*>(l_ItemTemplate)->FlagsCu |= ItemFlagsCustom::ITEM_FLAGS_CU_CANT_BE_SELL;
        }

        if (l_Reward.SpellID)
        {
            SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(l_Reward.SpellID);
            if (l_SpellInfo == nullptr)
            {
                sLog->outError(LOG_FILTER_SQL, "Table `achievement_reward` (Entry: %u) has invalid SpellID %u", l_Entry, l_Reward.SpellID);
                l_Reward.SpellID = 0;
            }
        }

        m_achievementRewards[l_Entry] = l_Reward;
        ++l_Counter;

    }
    while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u achievement rewards in %u ms", l_Counter, GetMSTimeDiffToNow(l_OldMSTime));
}

void AchievementGlobalMgr::LoadRewardLocales()
{
    uint32 l_OldMSTime = getMSTime();

    m_achievementRewardLocales.clear();                       // Need for reload case

    QueryResult l_Result = WorldDatabase.Query("SELECT entry, subject_loc1, text_loc1, subject_loc2, text_loc2, subject_loc3, text_loc3, subject_loc4, text_loc4, "
                                             "subject_loc5, text_loc5, subject_loc6, text_loc6, subject_loc7, text_loc7, subject_loc8, text_loc8, subject_loc9, text_loc9,"
                                             "subject_loc10, text_loc10 FROM locales_achievement_reward");

    if (!l_Result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 achievement reward locale strings.  DB table `locales_achievement_reward` is empty");
        return;
    }

    do
    {
        Field* l_Fields = l_Result->Fetch();

        uint32 l_Entry = l_Fields[0].GetUInt32();

        if (m_achievementRewards.find(l_Entry) == m_achievementRewards.end())
        {
            sLog->outError(LOG_FILTER_SQL, "Table `locales_achievement_reward` (Entry: %u) has locale strings for non-existing achievement reward.", l_Entry);
            continue;
        }

        AchievementRewardLocale& l_Data = m_achievementRewardLocales[l_Entry];

        for (int l_Iter = 1; l_Iter < TOTAL_LOCALES; ++l_Iter)
        {
            LocaleConstant l_Locale = (LocaleConstant) l_Iter;
            ObjectMgr::AddLocaleString(l_Fields[1 + 2 * (l_Iter - 1)].GetString(), l_Locale, l_Data.subject);
            ObjectMgr::AddLocaleString(l_Fields[1 + 2 * (l_Iter - 1) + 1].GetString(), l_Locale, l_Data.text);
        }
    }
    while (l_Result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %lu achievement reward locale strings in %u ms", (unsigned long)m_achievementRewardLocales.size(), GetMSTimeDiffToNow(l_OldMSTime));
}

AchievementEntry const* AchievementGlobalMgr::GetAchievement(uint32 p_AchievementID) const
{
    return sAchievementStore.LookupEntry(p_AchievementID);
}

CriteriaEntry const* AchievementGlobalMgr::GetAchievementCriteria(uint32 p_CriteriaID) const
{
    return sCriteriaStore.LookupEntry(p_CriteriaID);
}

// This function will be used temporarely for scripted achievements because we dont want them to be spread
// accross the whole core and untill criteria scripts are fixed.
template<class T>
bool AchievementMgr<T>::RequiresScript(CriteriaEntry const* p_Criteria)
{
    if (!p_Criteria)
        return true;

    switch (p_Criteria->ID)
    {
        case 24682: // A Gift of Earth and Fire
            return true;
        default:
            return false;
    }
}

void AchievementGlobalMgr::PrepareCriteriaUpdateTaskThread()
{
    AchievementCriteriaUpdateTask l_Task;
    for (auto l_Iterator = m_LockedPlayersAchievementCriteriaTask.begin(); l_Iterator != m_LockedPlayersAchievementCriteriaTask.end(); l_Iterator++)
    {
        while ((*l_Iterator).second.next(l_Task))
            m_PlayersAchievementCriteriaTask[(*l_Iterator).first].push(l_Task);
    }
}

AchievementCriteriaUpdateRequest::AchievementCriteriaUpdateRequest(MapUpdater* p_Updater, AchievementCriteriaTaskQueue p_TaskQueue)
: MapUpdaterTask(p_Updater), m_CriteriaUpdateTasks(p_TaskQueue)
{

}

void AchievementCriteriaUpdateRequest::call()
{
    AchievementCriteriaUpdateTask l_Task;
    while (!m_CriteriaUpdateTasks.empty())
    {
        l_Task = m_CriteriaUpdateTasks.front();
        l_Task.Task(l_Task.PlayerGUID, l_Task.UnitGUID);
        m_CriteriaUpdateTasks.pop();
    }

    UpdateFinished();
}
