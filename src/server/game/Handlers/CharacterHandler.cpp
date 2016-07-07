////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "GuildMgr.h"
#include "GitRevision.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "DatabaseEnv.h"
#include "Arena.h"
#include "Chat.h"
#include "Group.h"
#include "Language.h"
#include "Log.h"
#include "Opcodes.h"
#include "Player.h"
#include "SharedDefines.h"
#include "SocialMgr.h"
#include "UpdateMask.h"
#include "Util.h"
#include "ScriptMgr.h"
#include "Battleground.h"
#include "AccountMgr.h"
#include "DBCStores.h"
#include "LFGMgr.h"

#ifndef CROSS
# include "Guild.h"
# include "GuildFinderMgr.h"
# include "PlayerDump.h"
#else
# include <time.h>
# include "InterRealmMgr.h"
#endif

bool LoginDBQueryHolder::Initialize()
{
    SetSize(MAX_PLAYER_LOGINDB_QUERY);

    bool l_Result = true;
    PreparedStatement* l_Statement = nullptr;

    l_Statement = LoginDatabase.GetPreparedStatement(LOGIN_SEL_CHARACTER_SPELL);
    l_Statement->setUInt32(0, m_AccountId);
    l_Result &= SetPreparedQuery(PLAYER_LOGINGB_SPELL, l_Statement);

    l_Statement = LoginDatabase.GetPreparedStatement(LOGIN_SEL_HEIRLOOM_COLLECTION);
    l_Statement->setUInt32(0, m_AccountId);
    l_Result &= SetPreparedQuery(PLAYER_LOGINDB_HEIRLOOM_COLLECTION, l_Statement);

    l_Statement = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_TOYS);
    l_Statement->setUInt32(0, m_AccountId);
    l_Result &= SetPreparedQuery(PLAYER_LOGINDB_TOYS, l_Statement);

    return l_Result;
}

bool LoginQueryHolder::Initialize()
{
    SetSize(MAX_PLAYER_LOGIN_QUERY);

    bool l_Result = true;
    uint32 l_LowGuid = GUID_LOPART(m_guid);
    PreparedStatement* l_Statement = nullptr;
    
#ifdef CROSS
    InterRealmDatabasePool l_RealmDatabase = *sInterRealmMgr->GetClientByRealmNumber(m_realmId)->GetDatabase();
#else
    auto l_RealmDatabase = CharacterDatabase;
#endif

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADFROM, l_Statement);

#ifndef CROSS
    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_GROUP_MEMBER);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADGROUP, l_Statement);
#endif

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_BOSS_LOOTED);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_BOSS_LOOTED, l_Statement);

#ifndef CROSS
    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_INSTANCE);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADBOUNDINSTANCES, l_Statement);
#endif /* CROSS */

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_AURAS);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADAURAS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_AURAS_EFFECTS);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADAURAS_EFFECTS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SPELL);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_CHAR_LOADSPELLS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_QUESTSTATUS);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADQUESTSTATUS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_QUEST_OBJECTIVE_STATUS);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_QUEST_OBJECTIVE_STATUS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_DAILYQUESTSTATUS);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADDAILYQUESTSTATUS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_WEEKLYQUESTSTATUS);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADWEEKLYQUESTSTATUS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_MONTHLYQUESTSTATUS);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_MONTHLY_QUEST_STATUS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SEASONALQUESTSTATUS);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADSEASONALQUESTSTATUS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_REPUTATION);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADREPUTATION, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_INVENTORY);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADINVENTORY, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHAR_VOID_STORAGE);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADVOIDSTORAGE, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_ACTIONS);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACTIONS, l_Statement);

#ifndef CROSS
    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_MAILCOUNT);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Statement->setUInt64(1, uint64(time(NULL)));
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADMAILCOUNT, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_MAIL);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADMAIL, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_MAILITEMS);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADMAIL_ITEMS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SOCIALLIST);
    l_Statement->setUInt32(0, m_accountId);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADSOCIALLIST, l_Statement);
#endif

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_HOMEBIND);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADHOMEBIND, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SPELLCOOLDOWNS);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADSPELLCOOLDOWNS, l_Statement);

    if (sWorld->getBoolConfig(CONFIG_DECLINED_NAMES_USED))
    {
        l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_DECLINEDNAMES);
        l_Statement->setUInt32(0, l_LowGuid);
        l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADDECLINEDNAMES, l_Statement);
    }

#ifndef CROSS
    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_GUILD_MEMBER);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADGUILD, l_Statement);
#endif

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_ACHIEVEMENTS);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACHIEVEMENTS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_ACHIEVEMENTS);
    l_Statement->setUInt32(0, m_accountId);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACCOUNTACHIEVEMENTS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_CRITERIAPROGRESS);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADCRITERIAPROGRESS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_CRITERIAPROGRESS);
    l_Statement->setUInt32(0, m_accountId);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACCOUNTCRITERIAPROGRESS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_EQUIPMENTSETS);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADEQUIPMENTSETS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_ARENA_DATA);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADARENADATA, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_BGDATA);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADBGDATA, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_GLYPHS);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADGLYPHS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_TALENTS);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADTALENTS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_PLAYER_ACCOUNT_DATA);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACCOUNTDATA, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SKILLS);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADSKILLS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_RANDOMBG);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADRANDOMBG, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_BANNED);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADBANNED, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_QUESTSTATUSREW);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADQUESTSTATUSREW, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_INSTANCELOCKTIMES);
    l_Statement->setUInt32(0, m_accountId);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADINSTANCELOCKTIMES, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_PLAYER_CURRENCY);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADCURRENCY, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHAR_ARCHAEOLOGY);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_ARCHAEOLOGY, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHAR_ARCHAEOLOGY_PROJECTS);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_ARCHAEOLOGY_PROJECTS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHAR_ARCHAEOLOGY_SITES);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_ARCHAEOLOGY_SITES, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CUF_PROFILE);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_CUF_PROFILES, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_CHARGES_COOLDOWN);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_CHARGES_COOLDOWNS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_COMPLETED_CHALLENGES);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_COMPLETED_CHALLENGES, l_Statement);

#ifndef CROSS
    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_GARRISON);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_GARRISON, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_GARRISON_FOLLOWER);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_GARRISON_FOLLOWERS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_GARRISON_MISSION);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_GARRISON_MISSIONS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_GARRISON_BUILDING);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_GARRISON_BUILDINGS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_GARRISON_WORKORDER);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_GARRISON_WORKORDERS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_GARRISON_DAILY_TAVERN_DATA_CHAR);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_GARRISON_MISSIONS_TAVERNDATA, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_GARRISON_WEEKLY_TAVERN_DATA_CHAR);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_GARRISON_WEEKLY_TAVERNDATA, l_Statement);
#endif

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_DAILY_LOOT_COOLDOWNS);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_DAILY_LOOT_COOLDOWNS, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_SEL_WORLD_STATES);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_WORLD_STATES, l_Statement);

#ifndef CROSS
    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_LOAD_BOUTIQUE_ITEM);
    l_Statement->setInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_BOUTIQUE_ITEM, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_LOAD_STORE_PROFESSION);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_STORE_PROFESSION, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_LOAD_BOUTIQUE_GOLD);
    l_Statement->setInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_BOUTIQUE_GOLD, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_LOAD_BOUTIQUE_TITLE);
    l_Statement->setInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_BOUTIQUE_TITLE, l_Statement);

    l_Statement = l_RealmDatabase.GetPreparedStatement(CHAR_LOAD_BOUTIQUE_LEVEL);
    l_Statement->setInt32(0, l_LowGuid);
    l_Result &= SetPreparedQuery(PLAYER_LOGIN_QUERY_BOUTIQUE_LEVEL, l_Statement);
#endif

    return l_Result;
}

#ifndef CROSS
void WorldSession::HandleCharEnum(PreparedQueryResult p_Result)
{
    uint32 l_CharacterCount             = 0;
    uint32 l_FactionChangeRestrictions  = 0;

    bool l_CanCreateCharacter = true;

    if (p_Result)
    {
        _allowedCharsToLogin.clear();
        l_CharacterCount = uint32(p_Result->GetRowCount());
    }

    WorldPacket l_Data(SMSG_ENUM_CHARACTERS_RESULT, 5 * 1024);

    l_Data.WriteBit(l_CanCreateCharacter);          ///< Allow char creation
    l_Data.WriteBit(0);                             ///< IsDeletedCharacters
    l_Data.FlushBits();

    l_Data << uint32(l_CharacterCount);             ///< Account character count
    l_Data << uint32(l_FactionChangeRestrictions);  ///< Faction change restrictions

    if (p_Result)
    {
        do
        {
            uint32 l_GuidLow = (*p_Result)[0].GetUInt32();

            Player::BuildEnumData(p_Result, &l_Data);

            /// This can happen if characters are inserted into the database manually. Core hasn't loaded name data yet.
            if (!sWorld->HasCharacterInfo(l_GuidLow))
                sWorld->AddCharacterInfo(l_GuidLow, (*p_Result)[1].GetString(), GetAccountId(), (*p_Result)[4].GetUInt8(), (*p_Result)[2].GetUInt8(), (*p_Result)[3].GetUInt8(), (*p_Result)[7].GetUInt8());

            _allowedCharsToLogin.insert(l_GuidLow);
        } while (p_Result->NextRow());
    }

    for (uint32 l_I = 0; l_I < l_FactionChangeRestrictions; l_I++)
    {
        l_Data << uint32(0);                        ///< Mask
        l_Data << uint8(0);                         ///< Race ID
    }

    SendPacket(&l_Data);

    /// Update realm character count
    SQLTransaction trans = LoginDatabase.BeginTransaction();

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_REALM_CHARACTERS);
    stmt->setUInt32(0, l_CharacterCount);
    stmt->setUInt32(1, GetAccountId());
    stmt->setUInt32(2, g_RealmID);
    trans->Append(stmt);

    LoginDatabase.CommitTransaction(trans);
}

void WorldSession::HandleCharEnumOpcode(WorldPacket& /*recvData*/)
{
    // remove expired bans
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_EXPIRED_BANS);
    CharacterDatabase.Execute(stmt);

    /// get all the data necessary for loading all characters (along with their pets) on the account

    if (sWorld->getBoolConfig(CONFIG_DECLINED_NAMES_USED))
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ENUM_DECLINED_NAME);
    else
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ENUM);

    stmt->setUInt8(0, PetSlot::PET_SLOT_ACTUAL_PET_SLOT);
    stmt->setUInt32(1, GetAccountId());

    m_CharEnumCallback = CharacterDatabase.AsyncQuery(stmt);
}

void WorldSession::HandleCharCreateOpcode(WorldPacket& p_RecvData)
{
    uint32 l_CharacterNameLenght    = 0;
    uint32 l_TemplateSetID          = 0;

    uint8 l_CharacterRace       = 0;
    uint8 l_CharacterClass      = 0;
    uint8 l_CharacterGender     = 0;
    uint8 l_CharacterSkin       = 0;
    uint8 l_CharacterFace       = 0;
    uint8 l_CharacterHairStyle  = 0;
    uint8 l_CharacterHairColor  = 0;
    uint8 l_CharacterFacialHair = 0;
    uint8 l_CharacterOutfitID   = 0;

    std::string l_CharacterName;

    bool l_HaveTemplateSetID = false;

    //////////////////////////////////////////////////////////////////////////

    l_CharacterNameLenght   = p_RecvData.ReadBits(6);
    l_HaveTemplateSetID     = p_RecvData.ReadBit();

    p_RecvData >> l_CharacterRace;                                          ///< uint8
    p_RecvData >> l_CharacterClass;                                         ///< uint8
    p_RecvData >> l_CharacterGender;                                        ///< uint8
    p_RecvData >> l_CharacterSkin;                                          ///< uint8
    p_RecvData >> l_CharacterFace;                                          ///< uint8
    p_RecvData >> l_CharacterHairStyle;                                     ///< uint8
    p_RecvData >> l_CharacterHairColor;                                     ///< uint8
    p_RecvData >> l_CharacterFacialHair;                                    ///< uint8
    p_RecvData >> l_CharacterOutfitID;                                      ///< uint8

    l_CharacterName = p_RecvData.ReadString(l_CharacterNameLenght);

    if (l_HaveTemplateSetID)
        p_RecvData >> l_TemplateSetID;

    //////////////////////////////////////////////////////////////////////////

    WorldPacket l_CreationResponse(SMSG_CREATE_CHAR, 1);                    ///< returned with diff.values in all cases

    if (l_TemplateSetID)
    {
        bool l_TemplateAvailable = sWorld->getBoolConfig(CONFIG_TEMPLATES_ENABLED);
        CharacterTemplate const* l_Template = sObjectMgr->GetCharacterTemplate(l_TemplateSetID);

        if (!l_TemplateAvailable || !l_Template || l_Template->m_PlayerClass != l_CharacterClass)
        {
            l_CreationResponse << (uint8)CHAR_CREATE_ERROR;
            SendPacket(&l_CreationResponse);
            return;
        }
    }

    if (l_TemplateSetID && l_CharacterRace == RACE_PANDAREN_NEUTRAL)
    {
        l_CreationResponse << (uint8)CHAR_CREATE_CHARACTER_CHOOSE_RACE;
        SendPacket(&l_CreationResponse);
        return;
    }

    if (!l_TemplateSetID && (l_CharacterRace == RACE_PANDAREN_HORDE || l_CharacterRace == RACE_PANDAREN_ALLI))
    {
        l_CreationResponse << (uint8)CHAR_CREATE_ERROR;
        SendPacket(&l_CreationResponse);
        return;
    }

    if (AccountMgr::IsPlayerAccount(GetSecurity()))
    {
        if (uint32 l_Mask = sWorld->getIntConfig(CONFIG_CHARACTER_CREATING_DISABLED))
        {
            bool l_IsDisabled = false;

            uint32 l_RaceTeam = Player::TeamForRace(l_CharacterRace);

            switch (l_RaceTeam)
            {
                case ALLIANCE:
                    l_IsDisabled = l_Mask & (1 << 0);
                    break;

                case HORDE:
                    l_IsDisabled = l_Mask & (1 << 1);
                    break;
            }

            if (l_IsDisabled)
            {
                l_CreationResponse << (uint8)CHAR_CREATE_DISABLED;
                SendPacket(&l_CreationResponse);

                return;
            }
        }
    }

    ChrClassesEntry const* l_ClassEntry = sChrClassesStore.LookupEntry(l_CharacterClass);
    if (!l_ClassEntry)
    {
        l_CreationResponse << (uint8)CHAR_CREATE_FAILED;
        SendPacket(&l_CreationResponse);

        sLog->outError(LOG_FILTER_NETWORKIO, "Class (%u) not found in DBC while creating new char for account (ID: %u): wrong DBC files or cheater?", l_CharacterClass, GetAccountId());

        return;
    }

    ChrRacesEntry const* l_RaceEntry = sChrRacesStore.LookupEntry(l_CharacterRace);
    if (!l_RaceEntry)
    {
        l_CreationResponse << (uint8)CHAR_CREATE_FAILED;
        SendPacket(&l_CreationResponse);

        sLog->outError(LOG_FILTER_NETWORKIO, "Race (%u) not found in DBC while creating new char for account (ID: %u): wrong DBC files or cheater?", l_CharacterRace, GetAccountId());

        return;
    }

    if (AccountMgr::IsPlayerAccount(GetSecurity()))
    {
        uint32 l_RaceMaskDisabled = sWorld->getIntConfig(CONFIG_CHARACTER_CREATING_DISABLED_RACEMASK);

        if ((1 << (l_CharacterRace - 1)) & l_RaceMaskDisabled)
        {
            l_CreationResponse << uint8(CHAR_CREATE_DISABLED);
            SendPacket(&l_CreationResponse);

            return;
        }

        uint32 classMaskDisabled = sWorld->getIntConfig(CONFIG_CHARACTER_CREATING_DISABLED_CLASSMASK);

        if ((1 << (l_CharacterClass - 1)) & classMaskDisabled)
        {
            l_CreationResponse << uint8(CHAR_CREATE_DISABLED);
            SendPacket(&l_CreationResponse);

            return;
        }
    }

    /// prevent character creating with invalid name
    if (!normalizePlayerName(l_CharacterName))
    {
        l_CreationResponse << (uint8)CHAR_NAME_NO_NAME;
        SendPacket(&l_CreationResponse);

        sLog->outError(LOG_FILTER_NETWORKIO, "Account:[%d] but tried to Create character with empty [name] ", GetAccountId());

        return;
    }

    /// check name limitations
    uint8 l_Result = ObjectMgr::CheckPlayerName(l_CharacterName, true);
    if (l_Result != CHAR_NAME_SUCCESS)
    {
        l_CreationResponse << uint8(l_Result);
        SendPacket(&l_CreationResponse);

        return;
    }

    if (AccountMgr::IsPlayerAccount(GetSecurity()) && sObjectMgr->IsReservedName(l_CharacterName))
    {
        l_CreationResponse << (uint8)CHAR_NAME_RESERVED;
        SendPacket(&l_CreationResponse);

        return;
    }

    /// speedup check for heroic class disabled case
    uint32 l_RequiredLevelForHeroic = sWorld->getIntConfig(CONFIG_CHARACTER_CREATING_MIN_LEVEL_FOR_HEROIC_CHARACTER);
    if (AccountMgr::IsPlayerAccount(GetSecurity()) && l_CharacterClass == CLASS_DEATH_KNIGHT && l_RequiredLevelForHeroic > sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL))
    {
        l_CreationResponse << (uint8)CHAR_CREATE_LEVEL_REQUIREMENT;
        SendPacket(&l_CreationResponse);
        return;
    }

    delete _charCreateCallback.GetParam();  // Delete existing if any, to make the callback chain reset to stage 0

    _charCreateCallback.SetParam(new CharacterCreateInfo(l_CharacterName, l_CharacterRace, l_CharacterClass, l_CharacterGender, l_CharacterSkin, l_CharacterFace, l_CharacterHairStyle, l_CharacterHairColor, l_CharacterFacialHair, 1, l_TemplateSetID, p_RecvData));

    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHECK_NAME);
    l_Stmt->setString(0, l_CharacterName);
    l_Stmt->setString(1, l_CharacterName);

    _charCreateCallback.SetFutureResult(CharacterDatabase.AsyncQuery(l_Stmt));
}

void WorldSession::HandleCharCreateCallback(PreparedQueryResult result, CharacterCreateInfo* createInfo)
{
    /** This is a series of callbacks executed consecutively as a result from the database becomes available.
        This is much more efficient than synchronous requests on packet handler, and much less DoS prone.
        It also prevents data syncrhonisation errors.
    */
    switch (_charCreateCallback.GetStage())
    {
        case 0:
        {
            if (result)
            {
                WorldPacket data(SMSG_CREATE_CHAR, 1);
                data << uint8(CHAR_CREATE_NAME_IN_USE);
                SendPacket(&data);
                delete createInfo;
                _charCreateCallback.Reset();
                return;
            }

            ASSERT(_charCreateCallback.GetParam() == createInfo);

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_SUM_CHARS);
            stmt->setUInt32(0, GetAccountId());

            _charCreateCallback.FreeResult();
            _charCreateCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
            _charCreateCallback.NextStage();
        }
        break;
        case 1:
        {
            if (result)
            {
                Field* fields = result->Fetch();
                createInfo->CharCount = uint8(fields[0].GetUInt64()); // SQL's COUNT() returns uint64 but it will always be less than uint8.Max

                if (createInfo->CharCount >= sWorld->getIntConfig(CONFIG_CHARACTERS_PER_REALM))
                {
                    WorldPacket data(SMSG_CREATE_CHAR, 1);
                    data << uint8(CHAR_CREATE_SERVER_LIMIT);
                    SendPacket(&data);
                    delete createInfo;
                    _charCreateCallback.Reset();
                    return;
                }
            }

            bool allowTwoSideAccounts = !sWorld->IsPvPRealm() || sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_ACCOUNTS) || !AccountMgr::IsPlayerAccount(GetSecurity());
            uint32 skipCinematics = sWorld->getIntConfig(CONFIG_SKIP_CINEMATICS);

            _charCreateCallback.FreeResult();

            if (!allowTwoSideAccounts || skipCinematics == 1 || createInfo->Class == CLASS_DEATH_KNIGHT)
            {
                PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_CREATE_INFO);
                stmt->setUInt32(0, GetAccountId());
                stmt->setUInt32(1, (skipCinematics == 1 || createInfo->Class == CLASS_DEATH_KNIGHT) ? 10 : 1);
                _charCreateCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
                _charCreateCallback.NextStage();
                return;
            }

            _charCreateCallback.NextStage();
            HandleCharCreateCallback(PreparedQueryResult(NULL), createInfo);   // Will jump to case 2
        }
        break;
        case 2:
        {
            bool haveSameRace = false;
            uint32 heroicReqLevel = sWorld->getIntConfig(CONFIG_CHARACTER_CREATING_MIN_LEVEL_FOR_HEROIC_CHARACTER);
            bool hasHeroicReqLevel = (heroicReqLevel == 0);
            bool allowTwoSideAccounts = !sWorld->IsPvPRealm() || sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_ACCOUNTS) || !AccountMgr::IsPlayerAccount(GetSecurity());
            uint32 skipCinematics = sWorld->getIntConfig(CONFIG_SKIP_CINEMATICS);

            if (result)
            {
                uint32 team = Player::TeamForRace(createInfo->Race);

                Field* field = result->Fetch();
                uint8 accRace  = field[1].GetUInt8();

                if (AccountMgr::IsPlayerAccount(GetSecurity()) && createInfo->Class == CLASS_DEATH_KNIGHT)
                {
                    if (!hasHeroicReqLevel)
                    {
                        uint8 accLevel = field[0].GetUInt8();
                        if (accLevel >= heroicReqLevel)
                            hasHeroicReqLevel = true;
                    }
                }

                // need to check team only for first character
                // TODO: what to if account already has characters of both races?
                if (!allowTwoSideAccounts)
                {
                    uint32 accTeam = 0;
                    if (accRace > 0)
                        accTeam = Player::TeamForRace(accRace);

                    if (accTeam != team)
                    {
                        WorldPacket data(SMSG_CREATE_CHAR, 1);
                        data << uint8(CHAR_CREATE_PVP_TEAMS_VIOLATION);
                        SendPacket(&data);
                        delete createInfo;
                        _charCreateCallback.Reset();
                        return;
                    }
                }

                // search same race for cinematic or same class if need
                // TODO: check if cinematic already shown? (already logged in?; cinematic field)
                while ((skipCinematics == 1 && !haveSameRace) || createInfo->Class == CLASS_DEATH_KNIGHT)
                {
                    if (!result->NextRow())
                        break;

                    field = result->Fetch();
                    accRace = field[1].GetUInt8();

                    if (!haveSameRace)
                        haveSameRace = createInfo->Race == accRace;

                    if (AccountMgr::IsPlayerAccount(GetSecurity()) && createInfo->Class == CLASS_DEATH_KNIGHT)
                    {
                        if (!hasHeroicReqLevel)
                        {
                            uint8 acc_level = field[0].GetUInt8();
                            if (acc_level >= heroicReqLevel)
                                hasHeroicReqLevel = true;
                        }
                    }
                }
            }

            if (AccountMgr::IsPlayerAccount(GetSecurity()) && createInfo->Class == CLASS_DEATH_KNIGHT && !hasHeroicReqLevel)
            {
                WorldPacket data(SMSG_CREATE_CHAR, 1);
                data << uint8(CHAR_CREATE_LEVEL_REQUIREMENT);
                SendPacket(&data);
                delete createInfo;
                _charCreateCallback.Reset();
                return;
            }

            if (createInfo->Data.rpos() < createInfo->Data.wpos())
            {
                uint8 unk;
                createInfo->Data >> unk;
                sLog->outDebug(LOG_FILTER_NETWORKIO, "Character creation %s (account %u) has unhandled tail data: [%u]", createInfo->Name.c_str(), GetAccountId(), unk);
            }

            Player newChar(this);
            newChar.GetMotionMaster()->Initialize();
            if (!newChar.Create(sObjectMgr->GenerateLowGuid(HIGHGUID_PLAYER), createInfo))
            {
                // Player not create (race/class/etc problem?)
                newChar.CleanupsBeforeDelete();

                WorldPacket data(SMSG_CREATE_CHAR, 1);
                data << uint8(CHAR_CREATE_ERROR);
                SendPacket(&data);
                delete createInfo;
                _charCreateCallback.Reset();
                return;
            }

            if ((haveSameRace && skipCinematics == 1) || skipCinematics == 2)
                newChar.setCinematic(1);                          // not show intro

            newChar.SetAtLoginFlag(AT_LOGIN_FIRST);               // First login


            // Player created, save it now
            uint32 l_AccountID = GetAccountId();

            newChar.SaveToDB(true, std::make_shared<MS::Utilities::Callback>([l_AccountID](bool p_Success) -> void
            {
                WorldSession* l_Session = sWorld->FindSession(l_AccountID);
                if (l_Session == nullptr)
                    return;

                WorldPacket l_Data(SMSG_CREATE_CHAR, 1);
                l_Data << uint8(p_Success ? CHAR_CREATE_SUCCESS : CHAR_CREATE_ERROR);
                l_Session->SendPacket(&l_Data);
            }));

            createInfo->CharCount++;

            SQLTransaction trans = LoginDatabase.BeginTransaction();

            PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_REALM_CHARACTERS);
            stmt->setUInt32(0, createInfo->CharCount);
            stmt->setUInt32(1, GetAccountId());
            stmt->setUInt32(2, g_RealmID);
            trans->Append(stmt);

            LoginDatabase.CommitTransaction(trans);

            std::string IP_str = GetRemoteAddress();
            sLog->outInfo(LOG_FILTER_CHARACTER, "Account: %d (IP: %s) Create Character:[%s] (GUID: %u)", GetAccountId(), IP_str.c_str(), createInfo->Name.c_str(), newChar.GetGUIDLow());
            sScriptMgr->OnPlayerCreate(&newChar);
            sWorld->AddCharacterInfo(newChar.GetGUIDLow(), std::string(newChar.GetName()), GetAccountId(), newChar.getGender(), newChar.getRace(), newChar.getClass(), newChar.getLevel());

            newChar.CleanupsBeforeDelete();
            delete createInfo;
            _charCreateCallback.Reset();
        }
        break;
    }
}

void WorldSession::HandleCharDeleteOpcode(WorldPacket& recvData)
{
    uint64 charGuid;

    recvData.readPackGUID(charGuid);

    // can't delete loaded character
    if (ObjectAccessor::FindPlayer(charGuid))
        return;

    uint32 accountId = 0;
    uint32 atLogin = 0;
    std::string name;

    // is guild leader
    if (sGuildMgr->GetGuildByLeader(charGuid))
    {
        WorldPacket data(SMSG_CHAR_DELETE, 1);
        data << uint8(CHAR_DELETE_FAILED_GUILD_LEADER);
        SendPacket(&data);
        return;
    }

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_NAME_BY_GUID);
    stmt->setUInt32(0, GUID_LOPART(charGuid));

    if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
    {
        Field* fields = result->Fetch();
        accountId     = fields[0].GetUInt32();
        name          = fields[1].GetString();
        atLogin       = fields[2].GetUInt16();
    }

    // prevent deleting other players' characters using cheating tools
    if (accountId != GetAccountId())
        return;

    if (atLogin & AT_LOGIN_LOCKED_FOR_TRANSFER)
    {
        WorldPacket data(SMSG_CHAR_DELETE, 1);
        data << uint8(CHAR_DELETE_FAILED_LOCKED_FOR_TRANSFER);
        SendPacket(&data);
        return;
    }

    std::string IP_str = GetRemoteAddress();
    sLog->outInfo(LOG_FILTER_CHARACTER, "Account: %d (IP: %s) Delete Character:[%s] (GUID: %u)", GetAccountId(), IP_str.c_str(), name.c_str(), GUID_LOPART(charGuid));
    sScriptMgr->OnPlayerDelete(charGuid);
    sWorld->DeleteCharacterInfo(GUID_LOPART(charGuid));

    sGuildFinderMgr->RemoveAllMembershipRequestsFromPlayer(charGuid);
    Player::DeleteFromDB(charGuid, GetAccountId());

    WorldPacket data(SMSG_CHAR_DELETE, 1);
    data << uint8(CHAR_DELETE_SUCCESS);
    SendPacket(&data);
}

void WorldSession::HandlePlayerLoginOpcode(WorldPacket& p_RecvData)
{
    // Prevent flood of CMSG_PLAYER_LOGIN
    m_PlayerLoginCounter++;

    if (m_PlayerLoginCounter > 5)
    {
        sLog->outAshran("Player kicked due to flood of CMSG_PLAYER_LOGIN");
        KickPlayer();
    }

    if (PlayerLoading() || GetPlayer() != NULL)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Player tries to login again, AccountId = %d", GetAccountId());
        return;
    }

    m_playerLoading = true;

    //////////////////////////////////////////////////////////////////////////

    uint64 l_PlayerGuid = 0;

    float l_FarClip = 0.0f;

    p_RecvData.readPackGUID(l_PlayerGuid);                                  ///< uint64
    p_RecvData >> l_FarClip;                                                ///< float

    //////////////////////////////////////////////////////////////////////////

    if (!CharCanLogin(GUID_LOPART(l_PlayerGuid)))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Account (%u) can't login with that character (%u).", GetAccountId(), GUID_LOPART(l_PlayerGuid));
        KickPlayer();

        return;
    }

    LoginPlayer(l_PlayerGuid);
}
#endif

void WorldSession::LoginPlayer(uint64 p_Guid)
{
    LoginQueryHolder* l_LoginQueryHolder = new LoginQueryHolder(GetAccountId(), p_Guid, g_RealmID, CharacterPortData());
    LoginDBQueryHolder* l_LoginDBQueryHolder = new LoginDBQueryHolder(GetAccountId());

    if (!l_LoginQueryHolder->Initialize() || !l_LoginDBQueryHolder->Initialize())
    {
        delete l_LoginQueryHolder;                                      // delete all unprocessed queries
        delete l_LoginDBQueryHolder;                                    // delete all unprocessed queries
        m_playerLoading = false;
        return;
    }

    m_CharacterLoginCallback = CharacterDatabase.DelayQueryHolder((SQLQueryHolder*)l_LoginQueryHolder);
    m_CharacterLoginDBCallback = LoginDatabase.DelayQueryHolder((SQLQueryHolder*)l_LoginDBQueryHolder);
}


void WorldSession::HandleLoadScreenOpcode(WorldPacket& recvPacket)
{
    sLog->outInfo(LOG_FILTER_GENERAL, "WORLD: Recvd CMSG_LOAD_SCREEN");
    uint32 mapID;

    recvPacket >> mapID;
    recvPacket.ReadBit();
}

#ifndef CROSS
void WorldSession::HandlePlayerLogin(LoginQueryHolder* l_CharacterHolder, LoginDBQueryHolder* l_LoginHolder)
{
    uint64 playerGuid = l_CharacterHolder->GetGuid();

    Player* pCurrChar = new Player(this);
     // for send server info and strings (config)
    ChatHandler chH = ChatHandler(pCurrChar);

    //uint32 time0 = getMSTime();

    // "GetAccountId() == db stored account id" checked in LoadFromDB (prevent login not own character using cheating tools)
    if (!pCurrChar->LoadFromDB(GUID_LOPART(playerGuid), l_CharacterHolder, l_LoginHolder))
    {
        SetPlayer(NULL);
        KickPlayer();                                       // disconnect client, player no set to session and it will not deleted or saved at kick
        delete pCurrChar;                                   // delete it manually
        delete l_CharacterHolder;                           // delete all unprocessed queries
        delete l_LoginHolder;                               // delete all unprocessed queries
        m_playerLoading = false;
        return;
    }

    //uint32 time1 = getMSTime() - time0;

    pCurrChar->GetMotionMaster()->Initialize();
    pCurrChar->SendDungeonDifficulty();

    WorldPacket l_Data(SMSG_RESUME_TOKEN, 5);
    l_Data << uint32(0);                                                    ///< Sequence
    l_Data << uint8(0x80);                                                  ///< Reason
    SendPacket(&l_Data);

    l_Data.Initialize(SMSG_LOGIN_VERIFY_WORLD, 24);
    l_Data << pCurrChar->GetMapId();                                        ///< uint32
    l_Data << pCurrChar->GetPositionX();                                    ///< float
    l_Data << pCurrChar->GetPositionY();                                    ///< float
    l_Data << pCurrChar->GetPositionZ();                                    ///< float
    l_Data << pCurrChar->GetOrientation();                                  ///< float
    l_Data << uint32(0);                                                    ///< uint32 => TransferSpellID
    SendPacket(&l_Data);

    // load player specific part before send times
    LoadAccountData(l_CharacterHolder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOADACCOUNTDATA), PER_CHARACTER_CACHE_MASK);

    if (!IsBackFromCross())
    {
        SendAccountDataTimes(pCurrChar->GetGUID());
        SendFeatureSystemStatus();
    }

    //uint32 time2 = getMSTime() - time1;

    /// Send MOTD
    if (!IsBackFromCross())
    {
        MotdText const l_MotdText = sWorld->GetMotd();
        std::string l_MotdStr = "";

        switch (GetSessionDbLocaleIndex())
        {
            case LocaleConstant::LOCALE_frFR:
                l_MotdStr = l_MotdText.TextFR;
                break;
            case LocaleConstant::LOCALE_esMX:
            case LocaleConstant::LOCALE_esES:
                l_MotdStr = l_MotdText.TextES;
                break;
            case LocaleConstant::LOCALE_ruRU:
                l_MotdStr = l_MotdText.TextRU;
                break;
            default:
                l_MotdStr = l_MotdText.Text;
                break;
        }

        std::string::size_type l_Position, l_NextPosition;
        std::vector<std::string> l_Lines;
        uint32 l_LineCount = 0;

        l_Data.Initialize(SMSG_MOTD, 100);                     // new in 2.0.1

        l_Position = 0;
        while ((l_NextPosition = l_MotdStr.find('@', l_Position)) != std::string::npos)
        {
            if (l_NextPosition != l_Position)
            {
                l_Lines.push_back(l_MotdStr.substr(l_Position, l_NextPosition - l_Position));
                ++l_LineCount;
            }

            l_Position = l_NextPosition + 1;
        }

        if (l_Position < l_MotdStr.length())
        {
            l_Lines.push_back(l_MotdStr.substr(l_Position));
            ++l_LineCount;
        }

        l_Data.WriteBits(l_Lines.size(), 4);
        l_Data.FlushBits();

        for (size_t l_I = 0; l_I < l_Lines.size(); l_I++)
        {
            l_Data.WriteBits(l_Lines[l_I].length(), 7);
            l_Data.FlushBits();
            l_Data.WriteString(l_Lines[l_I]);
        }

        SendPacket(&l_Data);

        /// Send server info
        if (sWorld->getIntConfig(CONFIG_ENABLE_SINFO_LOGIN) == 1)
            chH.PSendSysMessage(GitRevision::GetFullVersion());
    }

    SendTimeZoneInformations();

    if (sWorld->getBoolConfig(CONFIG_ARENA_SEASON_IN_PROGRESS))
    {
        l_Data.Initialize(SMSG_PVP_SEASON, 8);
        l_Data << uint32(sWorld->getIntConfig(CONFIG_ARENA_SEASON_ID));
        l_Data << uint32(sWorld->getIntConfig(CONFIG_ARENA_SEASON_ID) - 1);
        SendPacket(&l_Data);
    }

    //QueryResult* result = CharacterDatabase.PQuery("SELECT guildid, rank FROM guild_member WHERE guid = '%u'", pCurrChar->GetGUIDLow());
    if (PreparedQueryResult resultGuild = l_CharacterHolder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOADGUILD))
    {
        Field* fields = resultGuild->Fetch();
        pCurrChar->SetInGuild(fields[0].GetUInt32());
        pCurrChar->SetRank(fields[1].GetUInt8());

        if (Guild * guild = sGuildMgr->GetGuildById(pCurrChar->GetGuildId()))
            pCurrChar->SetGuildLevel(guild->GetAchievementMgr().GetAchievementPoints());
    }
    else if (pCurrChar->GetGuildId())                        // clear guild related fields in case wrong data about non existed membership
    {
        pCurrChar->SetInGuild(0);
        pCurrChar->SetRank(0);
        pCurrChar->SetGuildLevel(0);
    }

    HotfixData const& hotfix = sObjectMgr->GetHotfixData();
    l_Data.Initialize(SMSG_HOTFIX_NOTIFY_BLOB, 4 + hotfix.size() * 12);

    l_Data << uint32(hotfix.size());

    for (uint32 i = 0; i < hotfix.size(); ++i)
    {
        l_Data << uint32(hotfix[i].Type);
        l_Data << uint32(hotfix[i].Entry);
        l_Data << uint32(hotfix[i].Timestamp);
    }

    SendPacket(&l_Data);

    //uint32 time3 = getMSTime() - time2;

    // Send item extended costs hotfix
    std::set<uint32> extendedCostHotFix = sObjectMgr->GetOverwriteExtendedCosts();
    for (auto itr : extendedCostHotFix)
    {
        const ItemExtendedCostEntry* extendedCost = sItemExtendedCostStore.LookupEntry(itr);

        if (!extendedCost)
            continue;

        WorldPacket l_Data(SMSG_DB_REPLY, 1024);
        l_Data << uint32(sItemExtendedCostStore.GetHash());

        ByteBuffer l_ResponseData;
        if (sItemExtendedCostStore.WriteRecord(extendedCost->ID, l_ResponseData, GetSessionDbLocaleIndex()))
        {
            l_Data << uint32(extendedCost->ID);
            l_Data << uint32(sObjectMgr->GetHotfixDate(extendedCost->ID, sItemExtendedCostStore.GetHash()));
            l_Data.WriteBit(1);                                                         ///< Found ???
            l_Data << uint32(l_ResponseData.size());
            l_Data.append(l_ResponseData);
        }
        else
        {
            l_Data << uint32(-1);
            l_Data << uint32(time(NULL));
            l_Data.WriteBit(0);                                                         ///< Not Found ???
            l_Data << uint32(0);
        }

        SendPacket(&l_Data);
    }

    pCurrChar->SendInitialPacketsBeforeAddToMap();

    //Show cinematic at the first time that player login
    if (!pCurrChar->getCinematic())
    {
        pCurrChar->setCinematic(1);

        if (ChrClassesEntry const* cEntry = sChrClassesStore.LookupEntry(pCurrChar->getClass()))
        {
            if (cEntry->CinematicSequenceID)
                pCurrChar->SendCinematicStart(cEntry->CinematicSequenceID);
            else if (ChrRacesEntry const* rEntry = sChrRacesStore.LookupEntry(pCurrChar->getRace()))
                pCurrChar->SendCinematicStart(rEntry->CinematicSequence);

            // send new char string if not empty
            if (!sWorld->GetNewCharString().empty())
                chH.PSendSysMessage("%s", sWorld->GetNewCharString().c_str());
        }
    }

    if (Group* group = pCurrChar->GetGroup())
    {
        if (group->isLFGGroup())
        {
            LfgDungeonSet Dungeons;
            Dungeons.insert(sLFGMgr->GetDungeon(group->GetGUID()));
            sLFGMgr->SetSelectedDungeons(pCurrChar->GetGUID(), Dungeons);
            sLFGMgr->SetState(pCurrChar->GetGUID(), sLFGMgr->GetState(group->GetGUID()));
        }
    }

    //uint32 time4 = getMSTime() - time3;

    if (!pCurrChar->GetMap()->AddPlayerToMap(pCurrChar) || !pCurrChar->CheckInstanceLoginValid())
    {
        AreaTriggerStruct const* at = sObjectMgr->GetGoBackTrigger(pCurrChar->GetMapId());
        if (at)
            pCurrChar->TeleportTo(at->target_mapId, at->target_X, at->target_Y, at->target_Z, pCurrChar->GetOrientation());
        else
            pCurrChar->TeleportTo(pCurrChar->m_homebindMapId, pCurrChar->m_homebindX, pCurrChar->m_homebindY, pCurrChar->m_homebindZ, pCurrChar->GetOrientation());
    }

    sObjectAccessor->AddObject(pCurrChar);
    //sLog->outDebug(LOG_FILTER_GENERAL, "Player %s added to Map.", pCurrChar->GetName());

    if (pCurrChar->GetGuildId() != 0 && !IsBackFromCross())
    {
        if (Guild* guild = sGuildMgr->GetGuildById(pCurrChar->GetGuildId()))
            guild->SendLoginInfo(this);
        else
        {
            // remove wrong guild data
            sLog->outError(LOG_FILTER_GENERAL, "Player %s (GUID: %u) marked as member of not existing guild (id: %u), removing guild membership for player.", pCurrChar->GetName(), pCurrChar->GetGUIDLow(), pCurrChar->GetGuildId());
            pCurrChar->SetInGuild(0);
        }
    }

    //uint32 time5 = getMSTime() - time4;

    pCurrChar->SendInitialPacketsAfterAddToMap();

    CharacterDatabase.PExecute("UPDATE characters SET online = 1 WHERE guid = '%u'", pCurrChar->GetGUIDLow());
    LoginDatabase.PExecute("UPDATE account SET online = 1 WHERE id = '%u'", GetAccountId());
    pCurrChar->SetInGameTime(getMSTime());

    //uint32 time6 = getMSTime() - time5;

    // announce group about member online (must be after add to player list to receive announce to self)
    if (Group* group = pCurrChar->GetGroup())
    {
        //pCurrChar->groupInfo.group->SendInit(this); // useless
        group->SendUpdate();
        group->ResetMaxEnchantingLevel();
    }

    // friend status
    if (!IsBackFromCross())
        sSocialMgr->SendFriendStatus(pCurrChar, FRIEND_ONLINE, pCurrChar->GetGUIDLow(), true);

    // Place character in world (and load zone) before some object loading
    pCurrChar->LoadCorpse();

    // setting Ghost+speed if dead
    if (pCurrChar->m_deathState != ALIVE)
    {
        // not blizz like, we must correctly save and load player instead...
        if (pCurrChar->getRace() == RACE_NIGHTELF)
            pCurrChar->CastSpell(pCurrChar, 20584, true, 0);// auras SPELL_AURA_INCREASE_SPEED(+speed in wisp form), SPELL_AURA_INCREASE_SWIM_SPEED(+swim speed in wisp form), SPELL_AURA_TRANSFORM (to wisp form)
        pCurrChar->CastSpell(pCurrChar, 8326, true, 0);     // auras SPELL_AURA_GHOST, SPELL_AURA_INCREASE_SPEED(why?), SPELL_AURA_INCREASE_SWIM_SPEED(why?)

        pCurrChar->SetWaterWalking(true);
    }

    pCurrChar->ContinueTaxiFlight();
    //pCurrChar->LoadPet();
    //uint32 time7 = getMSTime() - time6;

    // Set FFA PvP for non GM in non-rest mode
    if (sWorld->IsFFAPvPRealm() && !pCurrChar->isGameMaster() && !pCurrChar->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_RESTING))
        pCurrChar->SetByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 1, UNIT_BYTE2_FLAG_FFA_PVP);

    if (pCurrChar->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_CONTESTED_PVP))
        pCurrChar->SetContestedPvP();

    // Apply at_login requests
    if (pCurrChar->HasAtLoginFlag(AT_LOGIN_RESET_SPELLS))
    {
        pCurrChar->resetSpells();
        SendNotification(LANG_RESET_SPELLS);
    }

    if (pCurrChar->HasAtLoginFlag(AT_LOGIN_RESET_TALENTS))
    {
        pCurrChar->ResetTalents(true);
        pCurrChar->SendTalentsInfoData(false);              // original talents send already in to SendInitialPacketsBeforeAddToMap, resend reset state
        pCurrChar->SendTalentsInvoluntarilyReset();
    }

    if (pCurrChar->HasAtLoginFlag(AT_LOGIN_RESET_SPECS))
    {
        pCurrChar->ResetAllSpecs();
        pCurrChar->RemoveAtLoginFlag(AT_LOGIN_RESET_SPECS);
    }

    if (pCurrChar->HasAtLoginFlag(AtLoginFlags::AT_LOGIN_DELETE_INVALID_SPELL))
    {
        pCurrChar->DeleteInvalidSpells();
        pCurrChar->RemoveAtLoginFlag(AtLoginFlags::AT_LOGIN_DELETE_INVALID_SPELL);
    }

    if (pCurrChar->HasAtLoginFlag(AT_LOGIN_FIRST))
    {
        pCurrChar->RemoveAtLoginFlag(AT_LOGIN_FIRST);

        PlayerInfo const* l_Info = sObjectMgr->GetPlayerInfo(pCurrChar->getRace(), pCurrChar->getClass());
        for (uint32 l_SpellId : l_Info->castSpells)
            pCurrChar->CastSpell(pCurrChar, l_SpellId, true);
    }

    // show time before shutdown if shutdown planned.
    if (sWorld->IsShuttingDown())
        sWorld->ShutdownMsg(true, pCurrChar);

    if (sWorld->getBoolConfig(CONFIG_ALL_TAXI_PATHS))
        pCurrChar->SetTaxiCheater(true);

    if (pCurrChar->isGameMaster())
        SendNotification(LANG_GM_ON);

    if (!IsBackFromCross())
    {
        pCurrChar->SendCUFProfiles();
        pCurrChar->SendToyBox();
    }

    //uint32 time8 = getMSTime() - time7;

    // Hackfix Remove Talent spell - Remove Glyph spell
    pCurrChar->learnSpell(111621, false); // Reset Glyph
    pCurrChar->learnSpell(113873, false); // Reset Talent

    std::string IP_str = GetRemoteAddress();
    sLog->outInfo(LOG_FILTER_CHARACTER, "Account: %d (IP: %s) Login Character:[%s] (GUID: %u) Level: %d",
        GetAccountId(), IP_str.c_str(), pCurrChar->GetName(), pCurrChar->GetGUIDLow(), pCurrChar->getLevel());

    if (!pCurrChar->IsStandState() && !pCurrChar->HasUnitState(UNIT_STATE_STUNNED))
        pCurrChar->SetStandState(UNIT_STAND_STATE_STAND);

    m_playerLoading = false;

    // fix exploit with Aura Bind Sight
    pCurrChar->StopCastingBindSight();
    pCurrChar->StopCastingCharm();
    pCurrChar->RemoveAurasByType(SPELL_AURA_BIND_SIGHT);

    /// - Vote bonus
    if (HaveVoteRemainingTime())
    {
        Aura* l_VoteAura = pCurrChar->HasAura(VOTE_BUFF) ? pCurrChar->GetAura(VOTE_BUFF) : pCurrChar->AddAura(VOTE_BUFF, pCurrChar);
        if (l_VoteAura)
            l_VoteAura->SetDuration(GetVoteRemainingTime() + 60 * IN_MILLISECONDS);
    }
    else
        pCurrChar->RemoveAurasDueToSpell(VOTE_BUFF);

    //uint32 time9 = getMSTime() - time8; ///> time9 is never read 01/18/16

    //uint32 totalTime = getMSTime() - time0; ///< totaltime is never read 01/18/16
    //if (totalTime > 50)
    //    sLog->outAshran("HandlePlayerLogin |****---> time1 : %u | time 2 : %u | time 3 : %u | time 4 : %u | time 5: %u | time 6 : %u | time 7 : %u | time 8 : %u | time 9 : %u | totaltime : %u", time1, time2, time3, time4, time5, time6, time7, time8, time9, totalTime);

    // Fix chat with transfert / rename
    sWorld->AddCharacterInfo(pCurrChar->GetGUIDLow(), pCurrChar->GetName(), GetAccountId(), pCurrChar->getGender(), pCurrChar->getRace(), pCurrChar->getClass(), pCurrChar->getLevel());

    /// Remove title due to exploit with first achievement
    pCurrChar->SetTitle(sCharTitlesStore.LookupEntry(139), true);
    pCurrChar->SetTitle(sCharTitlesStore.LookupEntry(120), true);
    pCurrChar->SetTitle(sCharTitlesStore.LookupEntry(122), true);
    pCurrChar->SetTitle(sCharTitlesStore.LookupEntry(159), true);

    if (!IsBackFromCross())
        sScriptMgr->OnPlayerLogin(pCurrChar);

    PreparedQueryResult l_ItemResult = l_CharacterHolder->GetPreparedResult(PLAYER_LOGIN_QUERY_BOUTIQUE_ITEM);
    PreparedQueryResult l_GoldResult = l_CharacterHolder->GetPreparedResult(PLAYER_LOGIN_QUERY_BOUTIQUE_GOLD);
    PreparedQueryResult l_TitleResult = l_CharacterHolder->GetPreparedResult(PLAYER_LOGIN_QUERY_BOUTIQUE_TITLE);
    PreparedQueryResult l_LevelResult = l_CharacterHolder->GetPreparedResult(PLAYER_LOGIN_QUERY_BOUTIQUE_LEVEL);
    PreparedQueryResult l_ProfessionResult = l_CharacterHolder->GetPreparedResult(PLAYER_LOGIN_QUERY_STORE_PROFESSION);

    pCurrChar->HandleStoreItemCallback(l_ItemResult);
    pCurrChar->HandleStoreGoldCallback(l_GoldResult);
    pCurrChar->HandleStoreTitleCallback(l_TitleResult);
    pCurrChar->HandleStoreLevelCallback(l_LevelResult);
    pCurrChar->HandleStoreProfessionCallback(l_ProfessionResult);

    if (l_ItemResult || l_GoldResult || l_TitleResult || l_LevelResult || l_ProfessionResult)
        pCurrChar->SaveToDB();

    if (IsBackFromCross())
        RestoreSpecialChannels();

    SetBackFromCross(false);

    delete l_CharacterHolder;
    delete l_LoginHolder;
}
#endif

void WorldSession::HandleSetFactionAtWar(WorldPacket& p_Packet)
{
    uint8 l_FactionIndex;
    p_Packet >> l_FactionIndex;

    GetPlayer()->GetReputationMgr().SetAtWar(l_FactionIndex, true);
}

void WorldSession::HandleUnSetFactionAtWar(WorldPacket& p_Packet)
{
    uint8 l_FactionIndex;
    p_Packet >> l_FactionIndex;

    GetPlayer()->GetReputationMgr().SetAtWar(l_FactionIndex, false);
}

//I think this function is never used :/ I dunno, but i guess this opcode not exists
void WorldSession::HandleSetFactionCheat(WorldPacket& /*recvData*/)
{
    sLog->outError(LOG_FILTER_NETWORKIO, "WORLD SESSION: HandleSetFactionCheat, not expected call, please report.");
    GetPlayer()->GetReputationMgr().SendStates();
}

enum TUTORIAL_ACTIONS
{
    TUTORIAL_ACTION_RESET   = 1,
    TUTORIAL_ACTION_CLEAR   = 2,
    TUTORIAL_ACTION_FLAG    = 3,
};

void WorldSession::HandleTutorial(WorldPacket& p_RecvPacket)
{
    uint32 l_Action;

    l_Action = p_RecvPacket.ReadBits(2);

    switch (l_Action)
    {
        case TUTORIAL_ACTION_FLAG:
            {
                uint32 l_TutorialBit;
                p_RecvPacket >> l_TutorialBit;

                uint8 l_Index = uint8(l_TutorialBit / 32);

                if (l_Index >= MAX_ACCOUNT_TUTORIAL_VALUES)
                    return;

                uint32 l_Value = (l_TutorialBit % 32);

                uint32 l_Flag = GetTutorialInt(l_Index);
                l_Flag |= (1 << l_Value);

                SetTutorialInt(l_Index, l_Flag);
            }
            break;

        case TUTORIAL_ACTION_CLEAR:
            for (uint8 l_I = 0; l_I < MAX_ACCOUNT_TUTORIAL_VALUES; ++l_I)
                SetTutorialInt(l_I, 0xFFFFFFFF);
            break;

        case TUTORIAL_ACTION_RESET:
            for (uint8 l_I = 0; l_I < MAX_ACCOUNT_TUTORIAL_VALUES; ++l_I)
                SetTutorialInt(l_I, 0x00000000);
            break;
    }
}

void WorldSession::HandleSetWatchedFactionOpcode(WorldPacket& recvData)
{
    uint32 fact;
    recvData >> fact;
    GetPlayer()->SetUInt32Value(PLAYER_FIELD_WATCHED_FACTION_INDEX, fact);
}

void WorldSession::HandleSetFactionInactiveOpcode(WorldPacket& recvData)
{
    uint32 replistid;
    bool inactive;

    recvData >> replistid;
    inactive = recvData.ReadBit();

    m_Player->GetReputationMgr().SetInactive(replistid, inactive);
}

void WorldSession::HandleShowAccountAchievement(WorldPacket& recvData)
{
    bool showing = recvData.ReadBit(); ///< showing is never read 01/18/16
}

void WorldSession::HandleShowingHelmOpcode(WorldPacket& recvData)
{
    recvData.read_skip<uint8>(); // unknown, bool?
    m_Player->ToggleFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HIDE_HELM);
}

void WorldSession::HandleShowingCloakOpcode(WorldPacket& recvData)
{
    recvData.read_skip<uint8>(); // unknown, bool?
    m_Player->ToggleFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HIDE_CLOAK);
}

#ifndef CROSS
void WorldSession::HandleCharRenameOpcode(WorldPacket& p_RecvData)
{
    uint64 l_Guid     = 0;
    uint32 nameLen  = 0;
    std::string newName;

    p_RecvData.readPackGUID(l_Guid);
    nameLen = p_RecvData.ReadBits(6);
    newName = p_RecvData.ReadString(nameLen);

    // prevent character rename to invalid name
    if (!normalizePlayerName(newName))
    {
        WorldPacket data(SMSG_CHAR_RENAME);
        BuildCharacterRename(&data, 0, CHAR_NAME_NO_NAME, "");
        SendPacket(&data);
        return;
    }

    uint8 res = ObjectMgr::CheckPlayerName(newName, true);
    if (res != CHAR_NAME_SUCCESS)
    {
        WorldPacket data(SMSG_CHAR_RENAME);
        BuildCharacterRename(&data, l_Guid, res, newName);
        SendPacket(&data);
        return;
    }

    // check name limitations
    if (AccountMgr::IsPlayerAccount(GetSecurity()) && sObjectMgr->IsReservedName(newName))
    {
        WorldPacket data(SMSG_CHAR_RENAME);
        BuildCharacterRename(&data, 0, CHAR_NAME_RESERVED, "");
        SendPacket(&data);
        return;
    }

    // Ensure that the character belongs to the current account, that rename at login is enabled
    // and that there is no character with the desired new name
    _charRenameCallback.SetParam(newName);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_FREE_NAME);

    stmt->setUInt32(0, GUID_LOPART(l_Guid));
    stmt->setUInt32(1, GetAccountId());
    stmt->setUInt16(2, AT_LOGIN_RENAME);
    stmt->setUInt16(3, AT_LOGIN_RENAME);
    stmt->setString(4, newName);
    stmt->setString(5, newName);

    _charRenameCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
}

void WorldSession::BuildCharacterRename(WorldPacket* p_Packet, ObjectGuid p_Guid, uint8 p_Result, std::string p_Name)
{
    *p_Packet << uint8(p_Result);
    p_Packet->WriteBit(p_Guid != 0);
    p_Packet->WriteBits(p_Name.size(), 6);
    p_Packet->FlushBits();

    if (p_Guid != 0)
        p_Packet->appendPackGUID(p_Guid);

    p_Packet->WriteString(p_Name);
}

void WorldSession::HandleChangePlayerNameOpcodeCallBack(PreparedQueryResult result, std::string newName)
{
    if (!result)
    {
        WorldPacket data(SMSG_CHAR_RENAME);
        BuildCharacterRename(&data, 0, CHAR_CREATE_ERROR, newName);
        SendPacket(&data);
        return;
    }

    Field* fields = result->Fetch();

    uint32 guidLow      = fields[0].GetUInt32();
    std::string oldName = fields[1].GetString();

    uint64 guid = MAKE_NEW_GUID(guidLow, 0, HIGHGUID_PLAYER);

    // Update name and at_login flag in the db
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_NAME);

    stmt->setString(0, newName);
    stmt->setUInt16(1, AT_LOGIN_RENAME);
    stmt->setUInt32(2, guidLow);

    CharacterDatabase.Execute(stmt);

    // Removed declined name from db
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_DECLINED_NAME);

    stmt->setUInt32(0, guidLow);

    CharacterDatabase.Execute(stmt);

    // Logging
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_NAME_LOG);

    stmt->setUInt32(0, guidLow);
    stmt->setString(1, oldName);
    stmt->setString(2, newName);

    CharacterDatabase.Execute(stmt);

    sLog->outInfo(LOG_FILTER_CHARACTER, "Account: %d (IP: %s) Character:[%s] (guid:%u) Changed name to: %s", GetAccountId(), GetRemoteAddress().c_str(), oldName.c_str(), guidLow, newName.c_str());

    WorldPacket data(SMSG_CHAR_RENAME);
    BuildCharacterRename(&data, guid, RESPONSE_SUCCESS, newName);
    SendPacket(&data);

    sWorld->UpdateCharacterInfo(guidLow, newName);
}
#endif

void WorldSession::HandleSetPlayerDeclinedNames(WorldPacket& p_RecvData)
{
#ifndef CROSS
    uint64       l_Player;
    uint8        l_DeclinedNameSize[5];
    DeclinedName l_DeclinedName;

    p_RecvData.readPackGUID(l_Player);

    for (int l_I = 0; l_I < MAX_DECLINED_NAME_CASES; l_I++)
        l_DeclinedNameSize[l_I] = p_RecvData.ReadBits(7);

    for (int l_I = 0; l_I < MAX_DECLINED_NAME_CASES; l_I++)
    {
        l_DeclinedName.name[l_I] = p_RecvData.ReadString(l_DeclinedNameSize[l_I]);
        if (!normalizePlayerName(l_DeclinedName.name[l_I]))
        {
            SendPlayerDeclinedNamesResult(l_Player, 1);
            return;
        }
    }

    // not accept declined names for unsupported languages
    std::string l_Name;
    if (!sObjectMgr->GetPlayerNameByGUID(l_Player, l_Name))
    {
        SendPlayerDeclinedNamesResult(l_Player, 1);
        return;
    }

    std::wstring l_WName;
    if (!Utf8toWStr(l_Name, l_WName))
    {
        SendPlayerDeclinedNamesResult(l_Player, 1);
        return;
    }

    if (!isCyrillicCharacter(l_WName[0]))                      // name already stored as only single alphabet using
    {
        SendPlayerDeclinedNamesResult(l_Player, 1);
        return;
    }

    if (!ObjectMgr::CheckDeclinedNames(l_WName, l_DeclinedName))
    {
        SendPlayerDeclinedNamesResult(l_Player, 1);
        return;
    }

    for (int l_I = 0; l_I < MAX_DECLINED_NAME_CASES; ++l_I)
        CharacterDatabase.EscapeString(l_DeclinedName.name[l_I]);

    SQLTransaction l_Transaction = CharacterDatabase.BeginTransaction();

    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_DECLINED_NAME);
    l_Statement->setUInt32(0, GUID_LOPART(l_Player));
    l_Transaction->Append(l_Statement);

    l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_DECLINED_NAME);
    l_Statement->setUInt32(0, GUID_LOPART(l_Player));

    for (uint8 l_I = 0; l_I < MAX_DECLINED_NAME_CASES; l_I++)
        l_Statement->setString(l_I + 1, l_DeclinedName.name[l_I]);

    l_Transaction->Append(l_Statement);
    
    CharacterDatabase.CommitTransaction(l_Transaction);

    SendPlayerDeclinedNamesResult(l_Player, 0);
#endif
}

void WorldSession::SendPlayerDeclinedNamesResult(uint64 p_Player, uint32 p_Result)
{
    WorldPacket data(SMSG_SET_PLAYER_DECLINED_NAMES_RESULT, 4 + 8);
    data << uint32(p_Result);
    data.appendPackGUID(p_Player);
    SendPacket(&data);
}

void WorldSession::HandleAlterAppearance(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_ALTER_APPEARANCE");

    uint32 Hair, Color, FacialHair, SkinColor, Face;
    recvData >> Hair >> Color >> FacialHair >> SkinColor >> Face;

    BarberShopStyleEntry const* bs_hair = sBarberShopStyleStore.LookupEntry(Hair);
    if (!bs_hair || bs_hair->Type != 0 || bs_hair->Race != m_Player->getRace() || bs_hair->Sex != m_Player->getGender())
        return;

    BarberShopStyleEntry const* bs_facialHair = sBarberShopStyleStore.LookupEntry(FacialHair);
    if (!bs_facialHair || bs_facialHair->Type != 2 || bs_facialHair->Race != m_Player->getRace() || bs_facialHair->Sex != m_Player->getGender())
        return;

    BarberShopStyleEntry const* bs_skinColor = sBarberShopStyleStore.LookupEntry(SkinColor);
    if (bs_skinColor && (bs_skinColor->Type != 3 || bs_skinColor->Race != m_Player->getRace() || bs_skinColor->Sex != m_Player->getGender()))
        return;

    BarberShopStyleEntry const* l_FaceEntry = sBarberShopStyleStore.LookupEntry(Face);
    if (l_FaceEntry && (l_FaceEntry->Type != 4 || l_FaceEntry->Race != m_Player->getRace() || l_FaceEntry->Sex != m_Player->getGender()))
        return;

    GameObject* go = m_Player->FindNearestGameObjectOfType(GAMEOBJECT_TYPE_BARBER_CHAIR, 5.0f);
    if (!go)
    {
        WorldPacket data(SMSG_BARBER_SHOP_RESULT, 4);
        data << uint32(2);
        SendPacket(&data);
        return;
    }

    if (m_Player->getStandState() != UNIT_STAND_STATE_SIT_LOW_CHAIR + go->GetGOInfo()->barberChair.chairheight)
    {
        WorldPacket data(SMSG_BARBER_SHOP_RESULT, 4);
        data << uint32(2);
        SendPacket(&data);
        return;
    }

    uint32 cost = m_Player->GetBarberShopCost(bs_hair->Data, Color, bs_facialHair->Data, bs_skinColor, l_FaceEntry);

    // 0 - ok
    // 1, 3 - not enough money
    // 2 - you have to sit on barber chair
    if (!m_Player->HasEnoughMoney((uint64)cost))
    {
        WorldPacket data(SMSG_BARBER_SHOP_RESULT, 4);
        data << uint32(1);                                  // no money
        SendPacket(&data);
        return;
    }
    else
    {
        WorldPacket data(SMSG_BARBER_SHOP_RESULT, 4);
        data << uint32(0);                                  // ok
        SendPacket(&data);
    }

    m_Player->ModifyMoney(-int64(cost));                     // it isn't free
    m_Player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_AT_BARBER, cost);

    m_Player->SetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, PLAYER_BYTES_OFFSET_HAIR_STYLE_ID, uint8(bs_hair->Data));
    m_Player->SetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, PLAYER_BYTES_OFFSET_HAIR_COLOR_ID, uint8(Color));
    m_Player->SetByteValue(PLAYER_FIELD_REST_STATE, PLAYER_BYTES_2_OFFSET_FACIAL_STYLE, uint8(bs_facialHair->Data));

    if (bs_skinColor)
        m_Player->SetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, PLAYER_BYTES_OFFSET_SKIN_ID, uint8(bs_skinColor->Data));

    if (l_FaceEntry)
        m_Player->SetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, PLAYER_BYTES_OFFSET_FACE_ID, l_FaceEntry->Data);

    m_Player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP, 1);

    m_Player->SetStandState(0);                              // stand up
}

void WorldSession::HandleRemoveGlyph(WorldPacket& recvData)
{
    uint32 slot;
    recvData >> slot;

    if (slot >= MAX_GLYPH_SLOT_INDEX)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "Client sent wrong glyph slot number in opcode CMSG_REMOVE_GLYPH %u", slot);
        return;
    }

    if (uint32 glyph = m_Player->GetGlyph(m_Player->GetActiveSpec(), slot))
    {
        if (GlyphPropertiesEntry const* gp = sGlyphPropertiesStore.LookupEntry(glyph))
        {
            m_Player->RemoveAurasDueToSpell(gp->SpellId);
            m_Player->SetGlyph(slot, 0);
            m_Player->SendTalentsInfoData(false);
        }
    }
}

#ifndef CROSS
void WorldSession::HandleCharCustomize(WorldPacket& p_RecvData)
{
    uint64 l_PlayerGuid = 0;
    uint8 l_CharacterGender     = 0;
    uint8 l_CharacterSkin       = 0;
    uint8 l_CharacterFace       = 0;
    uint8 l_CharacterHairStyle  = 0;
    uint8 l_CharacterHairColor  = 0;
    uint8 l_CharacterFacialHair = 0;

    std::string l_NewName;
    uint32 l_NameLen;

    p_RecvData.readPackGUID(l_PlayerGuid);

    p_RecvData >> l_CharacterGender;                                        ///< uint8
    p_RecvData >> l_CharacterSkin;                                          ///< uint8
    p_RecvData >> l_CharacterHairColor;                                     ///< uint8
    p_RecvData >> l_CharacterHairStyle;                                     ///< uint8
    p_RecvData >> l_CharacterFacialHair;                                    ///< uint8
    p_RecvData >> l_CharacterFace;                                          ///< uint8

    l_NameLen = p_RecvData.ReadBits(6);
    l_NewName = p_RecvData.ReadString(l_NameLen);

    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_AT_LOGIN);
    l_Statement->setUInt32(0, GUID_LOPART(l_PlayerGuid));
    PreparedQueryResult l_Result = CharacterDatabase.Query(l_Statement);

    if (!l_Result)
    {
        WorldPacket l_Data(SMSG_CHAR_CUSTOMIZE_RESULT, 1);
        l_Data << uint8(CHAR_CREATE_ERROR);
        l_Data.appendPackGUID(l_PlayerGuid);

        SendPacket(&l_Data);
        return;
    }

    Field* l_Fields = l_Result->Fetch();
    uint32 l_AtLoginFlag = l_Fields[0].GetUInt16();

    if (!(l_AtLoginFlag & AT_LOGIN_CUSTOMIZE))
    {
        WorldPacket l_Data(SMSG_CHAR_CUSTOMIZE_RESULT, 1);
        l_Data << uint8(CHAR_CREATE_ERROR);
        l_Data.appendPackGUID(l_PlayerGuid);

        SendPacket(&l_Data);
        return;
    }

    // prevent character rename to invalid name
    if (!normalizePlayerName(l_NewName))
    {
        WorldPacket l_Data(SMSG_CHAR_CUSTOMIZE_RESULT, 1);
        l_Data << uint8(CHAR_NAME_NO_NAME);
        l_Data.appendPackGUID(l_PlayerGuid);

        SendPacket(&l_Data);
        return;
    }

    uint8 l_Res = ObjectMgr::CheckPlayerName(l_NewName, true);
    if (l_Res != CHAR_NAME_SUCCESS)
    {
        WorldPacket l_Data(SMSG_CHAR_CUSTOMIZE_RESULT, 1);
        l_Data << uint8(l_Res);
        l_Data.appendPackGUID(l_PlayerGuid);

        SendPacket(&l_Data);
        return;
    }

    // check name limitations
    if (AccountMgr::IsPlayerAccount(GetSecurity()) && sObjectMgr->IsReservedName(l_NewName))
    {
        WorldPacket l_Data(SMSG_CHAR_CUSTOMIZE_RESULT, 1);
        l_Data << uint8(CHAR_NAME_RESERVED);
        l_Data.appendPackGUID(l_PlayerGuid);

        SendPacket(&l_Data);
        return;
    }

    // character with this name already exist
    if (uint64 l_NewGuid = sWorld->GetCharacterGuidByName(l_NewName))
    {
        if (l_NewGuid != l_PlayerGuid)
        {
            WorldPacket l_Data(SMSG_CHAR_CUSTOMIZE_RESULT, 1);
            l_Data << uint8(CHAR_CREATE_NAME_IN_USE);
            l_Data.appendPackGUID(l_PlayerGuid);

            SendPacket(&l_Data);
            return;
        }
    }

    l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_NAME);
    l_Statement->setUInt32(0, GUID_LOPART(l_PlayerGuid));
    l_Result = CharacterDatabase.Query(l_Statement);

    if (l_Result)
    {
        std::string oldname = l_Result->Fetch()[0].GetString();
        sLog->outInfo(LOG_FILTER_CHARACTER, "Account: %d (IP: %s), Character[%s] (guid:%u) Customized to: %s", GetAccountId(), GetRemoteAddress().c_str(), oldname.c_str(), GUID_LOPART(l_PlayerGuid), l_NewName.c_str());
    }

    Player::Customize(l_PlayerGuid, l_CharacterGender, l_CharacterSkin, l_CharacterFace, l_CharacterHairStyle, l_CharacterHairColor, l_CharacterFacialHair);

    l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_NAME_AT_LOGIN);

    l_Statement->setString(0, l_NewName);
    l_Statement->setUInt16(1, uint16(AT_LOGIN_CUSTOMIZE));
    l_Statement->setUInt32(2, GUID_LOPART(l_PlayerGuid));

    CharacterDatabase.Execute(l_Statement);

    l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_DECLINED_NAME);

    l_Statement->setUInt32(0, GUID_LOPART(l_PlayerGuid));

    CharacterDatabase.Execute(l_Statement);

    sWorld->UpdateCharacterInfo(GUID_LOPART(l_PlayerGuid), l_NewName, l_CharacterGender);

    WorldPacket l_Data(SMSG_CHAR_CUSTOMIZE);
    l_Data.appendPackGUID(l_PlayerGuid);
    l_Data.WriteBits(l_NewName.size(), 6);
    l_Data << uint8(l_CharacterGender);
    l_Data << uint8(l_CharacterSkin);
    l_Data << uint8(l_CharacterHairColor);
    l_Data << uint8(l_CharacterHairStyle);
    l_Data << uint8(l_CharacterFacialHair);
    l_Data << uint8(l_CharacterFace);
    l_Data.WriteString(l_NewName);

    SendPacket(&l_Data);
}
#endif

void WorldSession::HandleEquipmentSetSave(WorldPacket& p_RecvData)
{
    uint64 l_SetGuid = 0;
    uint32 l_SetID;
    EquipmentSet l_EquipmentSet;
    uint8 l_IconNameLen, l_SetNameLen;

    p_RecvData >> l_SetGuid;
    p_RecvData >> l_SetID;
    uint32 l_Unk = p_RecvData.read<uint32>();

    std::vector<uint64> l_ItemsGuids(EQUIPMENT_SLOT_END, 0);
    for (uint32 l_Iter = 0; l_Iter < EQUIPMENT_SLOT_END; ++l_Iter)
        p_RecvData.readPackGUID(l_ItemsGuids[l_Iter]);

    l_SetNameLen = p_RecvData.ReadBits(8);
    l_IconNameLen = 2 * p_RecvData.ReadBits(8);
    if (p_RecvData.ReadBit())
        ++l_IconNameLen;

    p_RecvData.FlushBits();

    std::string l_Name, l_IconName;
    l_Name = p_RecvData.ReadString(l_SetNameLen);
    l_IconName = p_RecvData.ReadString(l_IconNameLen);

    if (l_SetID >= MAX_EQUIPMENT_SET_INDEX)                    // client set slots amount
        return;

    for (uint32 l_Iter = 0; l_Iter < EQUIPMENT_SLOT_END; ++l_Iter)
    {
        // equipment manager sends "1" (as raw GUID) for slots set to "ignore" (don't touch slot at equip set)
        if (l_ItemsGuids[l_Iter] == 1)
        {
            // ignored slots saved as bit mask because we have no free special values for Items[i]
            l_EquipmentSet.IgnoreMask |= 1 << l_Iter;
            continue;
        }

        Item* l_Item = m_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, l_Iter);
        if (!l_Item && l_ItemsGuids[l_Iter])                               // cheating check 1
            return;

        if (l_Item && l_Item->GetGUID() != l_ItemsGuids[l_Iter])             // cheating check 2
            return;

        l_EquipmentSet.Items[l_Iter] = GUID_LOPART(l_ItemsGuids[l_Iter]);
    }

    l_EquipmentSet.Guid      = l_SetGuid;
    l_EquipmentSet.Name      = l_Name;
    l_EquipmentSet.IconName  = l_IconName;
    l_EquipmentSet.state     = EQUIPMENT_SET_NEW;

    m_Player->SetEquipmentSet(l_SetID, l_EquipmentSet);
}

void WorldSession::HandleEquipmentSetDelete(WorldPacket& p_RecvData)
{
    uint64 l_SetGuid = p_RecvData.read<uint64>();
    m_Player->DeleteEquipmentSet(l_SetGuid);
}

void WorldSession::HandleEquipmentSetUse(WorldPacket& p_RecvData)
{
    uint8 l_ItemCount = p_RecvData.ReadBits(2);

    std::vector<uint8> l_ItemBags(l_ItemCount, 0);
    std::vector<uint8> l_ItemSlots(l_ItemCount, 0);
    for (uint8 l_Iter = 0; l_Iter < l_ItemCount; ++l_Iter)
    {
        p_RecvData >> l_ItemBags[l_Iter];
        p_RecvData >> l_ItemSlots[l_Iter];
    }

    std::vector<uint64> l_ItemGuids(EQUIPMENT_SLOT_END, 0);
    std::vector<uint8>  l_ItemBag(EQUIPMENT_SLOT_END, 0);
    std::vector<uint8>  l_ItemSlot(EQUIPMENT_SLOT_END, 0);
    for (uint8 l_Iter = 0; l_Iter < EQUIPMENT_SLOT_END; ++l_Iter)
    {
        p_RecvData.readPackGUID(l_ItemGuids[l_Iter]);
        p_RecvData >> l_ItemSlot[l_Iter];
        p_RecvData >> l_ItemBag[l_Iter];
    }

    EquipmentSlots l_StartSlot = m_Player->isInCombat() ? EQUIPMENT_SLOT_MAINHAND : EQUIPMENT_SLOT_START;

    for (uint32 l_Iter = 0; l_Iter < EQUIPMENT_SLOT_END; ++l_Iter)
    {
        if (l_Iter == EQUIPMENT_SLOT_RANGED)
            continue;

        if (l_Iter < uint32(l_StartSlot))
            continue;

        // check if item slot is set to "ignored" (raw value == 1), must not be unequipped then
        if (l_ItemGuids[l_Iter] == 1)
            continue;

        Item* l_Item = m_Player->GetItemByGuid(l_ItemGuids[l_Iter]);

        uint16 l_DestPos = l_Iter | (INVENTORY_SLOT_BAG_0 << 8);

        if (!l_Item)
        {
            Item* l_OtherItem = m_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, l_Iter);
            if (!l_OtherItem)
                continue;

            ItemPosCountVec l_Dest;
            InventoryResult l_Result = m_Player->CanStoreItem(NULL_BAG, NULL_SLOT, l_Dest, l_OtherItem, false);
            if (l_Result == EQUIP_ERR_OK)
            {
                m_Player->RemoveItem(INVENTORY_SLOT_BAG_0, l_Iter, true);
                m_Player->StoreItem(l_Dest, l_OtherItem, true);
            }
            else
                m_Player->SendEquipError(l_Result, l_OtherItem, NULL);

            continue;
        }

        if (l_Item->GetPos() == l_DestPos)
            continue;

        m_Player->SwapItem(l_Item->GetPos(), l_DestPos);
    }

    WorldPacket l_Data(SMSG_USE_EQUIPMENT_SET_RESULT);
    l_Data << uint8(0);   ///< Reason 4 - equipment swap failed - inventory is full
    SendPacket(&l_Data);
}

#ifndef CROSS
void WorldSession::HandleCharRaceOrFactionChange(WorldPacket& p_Packet)
{
    bool l_FactionChange;
    bool l_HasSkinID;
    bool l_HasHairColor;
    bool l_HasHairStyleID;
    bool l_HasFacialHairStyleID;
    bool l_HasFaceID;

    uint8  l_SexID;
    uint8  l_RaceID;
    uint8  l_SkinID;
    uint8  l_HairColor;
    uint8  l_HairStyleID;
    uint8  l_FacialHairStyleID;
    uint8  l_FaceID;

    uint64      l_Guid;
    std::string l_Name;
    size_t      l_NameSize;

    l_FactionChange        = p_Packet.ReadBit(); ///< l_FactionChange is never read 01/18/16
    l_NameSize             = p_Packet.ReadBits(6);
    l_HasSkinID            = p_Packet.ReadBit();
    l_HasHairColor         = p_Packet.ReadBit();
    l_HasHairStyleID       = p_Packet.ReadBit();
    l_HasFacialHairStyleID = p_Packet.ReadBit();
    l_HasFaceID            = p_Packet.ReadBit();

    p_Packet.readPackGUID(l_Guid);

    p_Packet >> l_SexID;
    p_Packet >> l_RaceID;

    l_Name = p_Packet.ReadString(l_NameSize);

    if (l_HasSkinID)
        p_Packet >> l_SkinID;

    if (l_HasHairColor)
        p_Packet >> l_HairColor;

    if (l_HasHairStyleID)
        p_Packet >> l_HairStyleID;

    if (l_HasFacialHairStyleID)
        p_Packet >> l_FacialHairStyleID;

    if (l_HasFaceID)
        p_Packet >> l_FaceID;

    uint32 l_LowGuid = GUID_LOPART(l_Guid);

    // get the players old (at this moment current) race
    CharacterInfo const* l_NameData = sWorld->GetCharacterInfo(l_LowGuid);
    if (!l_NameData)
    {
        WorldPacket l_Data(SMSG_CHAR_FACTION_CHANGE, 1);
        l_Data.appendPackGUID(l_Guid);
        l_Data << uint8(CHAR_CREATE_ERROR);
        SendPacket(&l_Data);
        return;
    }

    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_CLASS_LVL_AT_LOGIN);
    l_Statement->setUInt32(0, l_LowGuid);
    PreparedQueryResult l_QueryResult = CharacterDatabase.Query(l_Statement);

    if (!l_QueryResult)
    {
        WorldPacket l_Data(SMSG_CHAR_FACTION_CHANGE, 1);
        l_Data.appendPackGUID(l_Guid);
        l_Data << uint8(CHAR_CREATE_ERROR);
        SendPacket(&l_Data);
        return;
    }

    Field* l_Fields         = l_QueryResult->Fetch();

    uint8  l_OldRace        = l_Fields[0].GetUInt8();
    uint32 l_PlayerClass    = uint32(l_Fields[1].GetUInt8());
    uint32 l_Level          = uint32(l_Fields[2].GetUInt8());
    uint32 l_AtLoginFlag    = l_Fields[3].GetUInt16();
    auto   l_KnownTitlesStr = l_Fields[4].GetCString();
    uint32 l_PlayerBytes    = l_Fields[5].GetUInt32();
    uint32 l_PlayerBytes2   = l_Fields[6].GetUInt32(); ///< L_playerbyte2 is never read 01/18/16

    /// - If client doesn't send value, get the old from database & use it
    {
        if (!l_HasSkinID)
            l_SkinID = l_PlayerBytes & 0xFF;

        if (!l_HasFaceID)
            l_FaceID = (l_PlayerBytes >> 8) & 0xFF;

        if (!l_HasHairStyleID)
            l_HairStyleID = (l_PlayerBytes >> 16) & 0xFF;

        if (!l_HasHairColor)
            l_HairColor = (l_PlayerBytes >> 24) & 0xFF;

        if (!l_HasFacialHairStyleID)
            l_FacialHairStyleID = l_PlayerBytes & 0xFF;
    }

    // Search each faction is targeted
    BattlegroundTeamId l_OldTeam = BG_TEAM_ALLIANCE;
    switch (l_OldRace)
    {
        case RACE_ORC:
        case RACE_GOBLIN:
        case RACE_TAUREN:
        case RACE_UNDEAD_PLAYER:
        case RACE_TROLL:
        case RACE_BLOODELF:
        case RACE_PANDAREN_HORDE:
            l_OldTeam = BG_TEAM_HORDE;
            break;
        default:
            break;
    }

    // Search each faction is targeted
    BattlegroundTeamId l_Team = BG_TEAM_ALLIANCE;
    switch (l_RaceID)
    {
        case RACE_ORC:
        case RACE_GOBLIN:
        case RACE_TAUREN:
        case RACE_UNDEAD_PLAYER:
        case RACE_TROLL:
        case RACE_BLOODELF:
        case RACE_PANDAREN_HORDE:
            l_Team = BG_TEAM_HORDE;
            break;
        default:
            break;
    }

    uint32 l_UsedLoginFlag = l_AtLoginFlag;

    // We need to correct race when it's pandaren
    // Because client always send neutral ID
    if (l_RaceID == RACE_PANDAREN_NEUTRAL)
    {
        if (l_AtLoginFlag & AT_LOGIN_CHANGE_RACE)
            l_Team = l_OldTeam;
        else
            l_Team = l_OldTeam == BG_TEAM_ALLIANCE ? BG_TEAM_HORDE : BG_TEAM_ALLIANCE;
    }

    if (l_RaceID == RACE_PANDAREN_NEUTRAL)
    {
        if (l_OldTeam == l_Team)
            l_RaceID = l_OldTeam == BG_TEAM_ALLIANCE ? RACE_PANDAREN_ALLI : RACE_PANDAREN_HORDE;
        else
            l_RaceID = l_OldTeam == BG_TEAM_ALLIANCE ? RACE_PANDAREN_HORDE : RACE_PANDAREN_ALLI;
    }

    // Not really necessary because changing race does not include faction change
    // But faction change can include race change
    if (l_OldTeam != l_Team)
        l_UsedLoginFlag = AT_LOGIN_CHANGE_FACTION;

    if (!sObjectMgr->GetPlayerInfo(l_RaceID, l_PlayerClass))
    {
        WorldPacket l_Data(SMSG_CHAR_FACTION_CHANGE, 1);
        l_Data.appendPackGUID(l_Guid);
        l_Data << uint8(CHAR_CREATE_ERROR);
        SendPacket(&l_Data);
        return;
    }

    if (!(l_AtLoginFlag & l_UsedLoginFlag))
    {
        WorldPacket l_Data(SMSG_CHAR_FACTION_CHANGE, 1);
        l_Data.appendPackGUID(l_Guid);
        l_Data << uint8(CHAR_CREATE_ERROR);
        SendPacket(&l_Data);
        return;
    }

    if (AccountMgr::IsPlayerAccount(GetSecurity()))
    {
        uint32 l_RaceMaskDisabled = sWorld->getIntConfig(CONFIG_CHARACTER_CREATING_DISABLED_RACEMASK);
        if ((1 << (l_RaceID - 1)) & l_RaceMaskDisabled)
        {
            WorldPacket l_Data(SMSG_CHAR_FACTION_CHANGE, 1);
            l_Data.appendPackGUID(l_Guid);
            l_Data << uint8(CHAR_CREATE_ERROR);
            SendPacket(&l_Data);
            return;
        }
    }

    // prevent character rename to invalid name
    if (!normalizePlayerName(l_Name))
    {
        WorldPacket l_Data(SMSG_CHAR_FACTION_CHANGE, 1);
        l_Data.appendPackGUID(l_Guid);
        l_Data << uint8(CHAR_NAME_NO_NAME);
        SendPacket(&l_Data);
        return;
    }

    uint8 l_Result = ObjectMgr::CheckPlayerName(l_Name, true);
    if (l_Result != CHAR_NAME_SUCCESS)
    {
        WorldPacket l_Data(SMSG_CHAR_FACTION_CHANGE, 1);
        l_Data.appendPackGUID(l_Guid);
        l_Data << uint8(l_Result);
        SendPacket(&l_Data);
        return;
    }

    // check name limitations
    if (AccountMgr::IsPlayerAccount(GetSecurity()) && sObjectMgr->IsReservedName(l_Name))
    {
        WorldPacket l_Data(SMSG_CHAR_FACTION_CHANGE, 1);
        l_Data.appendPackGUID(l_Guid);
        l_Data << uint8(CHAR_NAME_RESERVED);
        SendPacket(&l_Data);
        return;
    }

    // character with this name already exist
    if (uint64 l_NewGuid = sWorld->GetCharacterGuidByName(l_Name))
    {
        if (l_NewGuid != l_Guid)
        {
            WorldPacket l_Data(SMSG_CHAR_FACTION_CHANGE, 1);
            l_Data.appendPackGUID(l_Guid);
            l_Data << uint8(CHAR_CREATE_NAME_IN_USE);
            SendPacket(&l_Data);
            return;
        }
    }

    CharacterDatabase.EscapeString(l_Name);
    Player::Customize(l_Guid, l_SexID, l_SkinID, l_FaceID, l_HairStyleID, l_HairColor, l_FacialHairStyleID);
    SQLTransaction l_Transaction = CharacterDatabase.BeginTransaction();

    l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_FACTION_OR_RACE);
    l_Statement->setString(0, l_Name);
    l_Statement->setUInt8 (1, l_RaceID);
    l_Statement->setUInt16(2, l_UsedLoginFlag);
    l_Statement->setUInt32(3, l_LowGuid);
    l_Transaction->Append(l_Statement);

    l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_FACTION_OR_RACE_LOG);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Statement->setUInt32(1, GetAccountId());
    l_Statement->setUInt8 (2, l_OldRace);
    l_Statement->setUInt8 (3, l_RaceID);
    l_Transaction->Append(l_Statement);

    l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_DECLINED_NAME);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Transaction->Append(l_Statement);

    // CHECK PTR
    if (l_NameData)
    {
        l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_NAME_LOG);
        l_Statement->setUInt32(0, l_LowGuid);
        l_Statement->setString(1, l_NameData->Name);
        l_Statement->setString(2, l_Name);
        l_Transaction->Append(l_Statement);
    }

    sWorld->UpdateCharacterInfo(GUID_LOPART(l_Guid), l_Name, l_SexID, l_RaceID);

    // Switch Languages
    // delete all languages first
    l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SKILL_LANGUAGES);
    l_Statement->setUInt32(0, l_LowGuid);
    l_Transaction->Append(l_Statement);

    // Now add them back
    l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_SKILL_LANGUAGE);
    l_Statement->setUInt32(0, l_LowGuid);

    // Faction specific languages
    if (l_Team == BG_TEAM_HORDE)
        l_Statement->setUInt16(1, 109);
    else
        l_Statement->setUInt16(1, 98);

    l_Transaction->Append(l_Statement);

    // Race specific languages
    if (l_RaceID != RACE_ORC && l_RaceID != RACE_HUMAN)
    {
        l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_SKILL_LANGUAGE);
        l_Statement->setUInt32(0, l_LowGuid);

        switch (l_RaceID)
        {
            case RACE_DWARF:
                l_Statement->setUInt16(1, 111);
                break;
            case RACE_DRAENEI:
                l_Statement->setUInt16(1, 759);
                break;
            case RACE_GNOME:
                l_Statement->setUInt16(1, 313);
                break;
            case RACE_NIGHTELF:
                l_Statement->setUInt16(1, 113);
                break;
            case RACE_WORGEN:
                l_Statement->setUInt16(1, 791);
                break;
            case RACE_UNDEAD_PLAYER:
                l_Statement->setUInt16(1, 673);
                break;
            case RACE_TAUREN:
                l_Statement->setUInt16(1, 115);
                break;
            case RACE_TROLL:
                l_Statement->setUInt16(1, 315);
                break;
            case RACE_BLOODELF:
                l_Statement->setUInt16(1, 137);
                break;
            case RACE_GOBLIN:
                l_Statement->setUInt16(1, 792);
                break;
            case RACE_PANDAREN_ALLI:
                l_Statement->setUInt16(1, 906);
                break;
            case RACE_PANDAREN_HORDE:
                l_Statement->setUInt16(1, 907);
                break;
            case RACE_PANDAREN_NEUTRAL:
                l_Statement->setUInt16(1, 905);
                break;
        }

        l_Transaction->Append(l_Statement);
    }

    if (l_OldTeam != l_Team)
    {
        // Delete all Flypaths
        PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_TAXI_PATH);
        l_Statement->setUInt32(0, l_LowGuid);
        l_Transaction->Append(l_Statement);

        if (l_Level > 7)
        {
            // Update Taxi path
            // this doesn't seem to be 100% blizzlike... but it can't really be helped.
            std::ostringstream taximaskstream;
            TaxiMask const& factionMask = l_Team == TEAM_HORDE ? sHordeTaxiNodesMask : sAllianceTaxiNodesMask;
            for (uint8 i = 0; i < TaxiMaskSize; ++i)
            {
                // i = (315 - 1) / 8 = 39
                // m = 1 << ((315 - 1) % 8) = 4
                uint8 deathKnightExtraNode = l_PlayerClass != CLASS_DEATH_KNIGHT || i != 39 ? 0 : 4;
                taximaskstream << uint32(factionMask[i] | deathKnightExtraNode) << ' ';
            }

            l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_TAXIMASK);
            l_Statement->setString(0, taximaskstream.str());
            l_Statement->setUInt64(1, l_LowGuid);
            l_Transaction->Append(l_Statement);
        }

        // Delete all current quests
        l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_QUESTSTATUS);
        l_Statement->setUInt32(0, GUID_LOPART(l_Guid));
        l_Transaction->Append(l_Statement);

        // Delete record of the faction old completed quests
        {
            std::ostringstream l_Quests;
            ObjectMgr::QuestMap const& l_QuestTemplates = sObjectMgr->GetQuestTemplates();
            for (ObjectMgr::QuestMap::const_iterator iter = l_QuestTemplates.begin(); iter != l_QuestTemplates.end(); ++iter)
            {
                Quest *l_Quest = iter->second;
                uint32 l_RequiredRaces = l_Quest->GetRequiredRaces();
                if (l_Team == BG_TEAM_ALLIANCE)
                {
                    if (l_RequiredRaces & RACEMASK_ALLIANCE)
                    {
                        l_Quests << uint32(l_Quest->GetQuestId());
                        l_Quests << ',';
                    }
                }
                else // if (team == BG_TEAM_HORDE)
                {
                    if (l_RequiredRaces & RACEMASK_HORDE)
                    {
                        l_Quests << uint32(l_Quest->GetQuestId());
                        l_Quests << ',';
                    }
                }
            }

            std::string l_QuestsString = l_Quests.str();
            l_QuestsString = l_QuestsString.substr(0, l_QuestsString.length() - 1);

            if (!l_QuestsString.empty())
                l_Transaction->PAppend("DELETE FROM `character_queststatus_rewarded` WHERE guid='%u' AND quest IN (%s)", l_LowGuid, l_QuestsString.c_str());
        }

        if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GUILD))
        {
            // Reset guild
            PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUILD_MEMBER);

            l_Statement->setUInt32(0, l_LowGuid);

            PreparedQueryResult l_QueryResult = CharacterDatabase.Query(l_Statement);
            if (l_QueryResult)
                if (Guild* l_Guild = sGuildMgr->GetGuildById((l_QueryResult->Fetch()[0]).GetUInt32()))
                    l_Guild->DeleteMember(MAKE_NEW_GUID(l_LowGuid, 0, HIGHGUID_PLAYER), false, false, true);
        }

        if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_ADD_FRIEND))
        {
            // Delete Friend List
            l_Statement->setUInt32(0, GetAccountId());
            PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SOCIAL_BY_ACCOUNT_ID);
            l_Transaction->Append(l_Statement);

            l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SOCIAL_BY_FRIEND);
            l_Statement->setUInt32(0, l_LowGuid);
            l_Transaction->Append(l_Statement);
        }

        // Reset homebind and position
        l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_HOMEBIND);
        l_Statement->setUInt32(0, l_LowGuid);
        l_Transaction->Append(l_Statement);

        l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_INS_PLAYER_HOMEBIND);
        l_Statement->setUInt32(0, l_LowGuid);
        if (l_Team == BG_TEAM_ALLIANCE)
        {
            l_Statement->setUInt16(1, 0);
            l_Statement->setUInt16(2, 1519);
            l_Statement->setFloat (3, -8866.19f);
            l_Statement->setFloat (4, 671.16f);
            l_Statement->setFloat (5, 97.9034f);
            Player::SavePositionInDB(0, -8866.19f, 671.16f, 97.9034f, 0.0f, 1519, l_LowGuid);
        }
        else
        {
            l_Statement->setUInt16(1, 1);
            l_Statement->setUInt16(2, 1637);
            l_Statement->setFloat (3, 1633.33f);
            l_Statement->setFloat (4, -4439.11f);
            l_Statement->setFloat (5, 17.05f);
            Player::SavePositionInDB(1, 1633.33f, -4439.11f, 17.05f, 0.0f, 1637, l_LowGuid);
        }

        l_Transaction->Append(l_Statement);

        // Achievement conversion
        for (std::map<uint32, uint32>::const_iterator l_Iterator = sObjectMgr->FactionChange_Achievements.begin(); l_Iterator != sObjectMgr->FactionChange_Achievements.end(); ++l_Iterator)
        {
            uint32 l_AchievementAlliance = l_Iterator->first;
            uint32 l_AchievementHorde = l_Iterator->second;

            PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_ACHIEVEMENT_BY_ACHIEVEMENT);
            l_Statement->setUInt16(0, uint16(l_Team == BG_TEAM_ALLIANCE ? l_AchievementAlliance : l_AchievementHorde));
            l_Statement->setUInt32(1, l_LowGuid);
            l_Transaction->Append(l_Statement);

            l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_ACHIEVEMENT);
            l_Statement->setUInt16(0, uint16(l_Team == BG_TEAM_ALLIANCE ? l_AchievementAlliance : l_AchievementHorde));
            l_Statement->setUInt16(1, uint16(l_Team == BG_TEAM_ALLIANCE ? l_AchievementHorde : l_AchievementAlliance));
            l_Statement->setUInt32(2, l_LowGuid);
            l_Transaction->Append(l_Statement);
        }

        // Item conversion
        for (std::map<uint32, uint32>::const_iterator l_Iterator = sObjectMgr->FactionChange_Items.begin(); l_Iterator != sObjectMgr->FactionChange_Items.end(); ++l_Iterator)
        {
            uint32 l_ItemAlliance = l_Iterator->first;
            uint32 l_ItemHorde = l_Iterator->second;

            PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_INVENTORY_FACTION_CHANGE);
            l_Statement->setUInt32(0, (l_Team == BG_TEAM_ALLIANCE ? l_ItemAlliance : l_ItemHorde));
            l_Statement->setUInt32(1, (l_Team == BG_TEAM_ALLIANCE ? l_ItemHorde : l_ItemAlliance));
            l_Statement->setUInt32(2, l_LowGuid);
            l_Transaction->Append(l_Statement);
        }

        // Spell conversion
        for (std::map<uint32, uint32>::const_iterator l_Iterator = sObjectMgr->FactionChange_Spells.begin(); l_Iterator != sObjectMgr->FactionChange_Spells.end(); ++l_Iterator)
        {
            uint32 l_SpellAlliance = l_Iterator->first;
            uint32 l_SpellHorde = l_Iterator->second;

            PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SPELL_BY_SPELL);
            l_Statement->setUInt32(0, (l_Team == BG_TEAM_ALLIANCE ? l_SpellAlliance : l_SpellHorde));
            l_Statement->setUInt32(1, l_LowGuid);
            l_Transaction->Append(l_Statement);

            l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_SPELL_FACTION_CHANGE);
            l_Statement->setUInt32(0, (l_Team == BG_TEAM_ALLIANCE ? l_SpellAlliance : l_SpellHorde));
            l_Statement->setUInt32(1, (l_Team == BG_TEAM_ALLIANCE ? l_SpellHorde : l_SpellAlliance));
            l_Statement->setUInt32(2, l_LowGuid);
            l_Transaction->Append(l_Statement);
        }

        // Reputation conversion
        for (std::map<uint32, uint32>::const_iterator l_Iterator = sObjectMgr->FactionChange_Reputation.begin(); l_Iterator != sObjectMgr->FactionChange_Reputation.end(); ++l_Iterator)
        {
            uint32 reputation_alliance = l_Iterator->first;
            uint32 reputation_horde = l_Iterator->second;

            PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_REP_BY_FACTION);
            l_Statement->setUInt32(0, uint16(l_Team == BG_TEAM_ALLIANCE ? reputation_alliance : reputation_horde));
            l_Statement->setUInt32(1, l_LowGuid);
            l_Transaction->Append(l_Statement);

            l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_REP_FACTION_CHANGE);
            l_Statement->setUInt16(0, uint16(l_Team == BG_TEAM_ALLIANCE ? reputation_alliance : reputation_horde));
            l_Statement->setUInt16(1, uint16(l_Team == BG_TEAM_ALLIANCE ? reputation_horde : reputation_alliance));
            l_Statement->setUInt32(2, l_LowGuid);
            l_Transaction->Append(l_Statement);
        }

        // Title conversion
        if (l_KnownTitlesStr)
        {
            const uint32 l_KnowTitleCount = KNOWN_TITLES_SIZE;
            uint32 l_KnownTitles[l_KnowTitleCount];
            Tokenizer l_Tokens(l_KnownTitlesStr, ' ', l_KnowTitleCount);

            if (l_Tokens.size() != l_KnowTitleCount)
                return;

            for (uint32 l_Index = 0; l_Index < l_KnowTitleCount; ++l_Index)
                l_KnownTitles[l_Index] = atol(l_Tokens[l_Index]);

            for (std::map<uint32, uint32>::const_iterator l_Iterator = sObjectMgr->FactionChange_Titles.begin(); l_Iterator != sObjectMgr->FactionChange_Titles.end(); ++l_Iterator)
            {
                uint32 l_TitleAlliance = l_Iterator->first;
                uint32 l_TitleHorde    = l_Iterator->second;

                CharTitlesEntry const* l_AllianceTitle = sCharTitlesStore.LookupEntry(l_TitleAlliance);
                CharTitlesEntry const* l_HordeTitle    = sCharTitlesStore.LookupEntry(l_TitleHorde);

                // new team
                if (l_Team == BG_TEAM_ALLIANCE)
                {
                    uint32 l_BitIndex = l_HordeTitle->MaskID;
                    uint32 l_Index = l_BitIndex / 32;
                    uint32 l_OldFlag = 1 << (l_BitIndex % 32);
                    uint32 l_NewFlag = 1 << (l_AllianceTitle->MaskID % 32);

                    if (l_KnownTitles[l_Index] & l_OldFlag)
                    {
                        l_KnownTitles[l_Index] &= ~l_OldFlag;
                        // use index of the new title
                        l_KnownTitles[l_AllianceTitle->MaskID / 32] |= l_NewFlag;
                    }
                }
                else
                {
                    uint32 l_BitIndex = l_AllianceTitle->MaskID;
                    uint32 l_Index = l_BitIndex / 32;
                    uint32 l_OldFlag = 1 << (l_BitIndex % 32);
                    uint32 l_NewFlag = 1 << (l_HordeTitle->MaskID % 32);

                    if (l_KnownTitles[l_Index] & l_OldFlag)
                    {
                        l_KnownTitles[l_Index] &= ~l_OldFlag;
                        // use index of the new title
                        l_KnownTitles[l_HordeTitle->MaskID / 32] |= l_NewFlag;
                    }
                }

                std::ostringstream l_OstreamString;
                for (uint32 l_Index = 0; l_Index < l_KnowTitleCount; ++l_Index)
                    l_OstreamString << l_KnownTitles[l_Index] << ' ';

                PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_TITLES_FACTION_CHANGE);
                l_Statement->setString(0, l_OstreamString.str().c_str());
                l_Statement->setUInt32(1, l_LowGuid);
                l_Transaction->Append(l_Statement);

                // unset any currently chosen title
                l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_RES_CHAR_TITLES_FACTION_CHANGE);
                l_Statement->setUInt32(0, l_LowGuid);
                l_Transaction->Append(l_Statement);
            }
        }
    }

    CharacterDatabase.CommitTransaction(l_Transaction);

    std::string l_IPString = GetRemoteAddress();

    WorldPacket l_Data(SMSG_CHAR_FACTION_CHANGE, 1 + 8 + (l_Name.size() + 1) + 1 + 1 + 1 + 1 + 1 + 1 + 1);
    l_Data << uint8(RESPONSE_SUCCESS);      ///< Result
    l_Data.appendPackGUID(l_Guid);          ///< Guid
    l_Data.WriteBit(true);                  ///< l_HasDisplayInfo
    l_Data.FlushBits();

    /// - HasDisplayInfo block
    {
        l_Data.WriteBits(l_NameSize, 6);
        l_Data << uint8(l_SexID);
        l_Data << uint8(l_SkinID);
        l_Data << uint8(l_HairColor);
        l_Data << uint8(l_HairStyleID);
        l_Data << uint8(l_FacialHairStyleID);
        l_Data << uint8(l_FaceID);
        l_Data << uint8(l_RaceID);
        l_Data.WriteString(l_Name);
    }

    SendPacket(&l_Data);
}

void WorldSession::HandleRandomizeCharNameOpcode(WorldPacket& recvData)
{
    uint8 gender, race;

    recvData >> race;
    recvData >> gender;

    if (!Player::IsValidRace(race))
    {
        sLog->outError(LOG_FILTER_GENERAL, "Invalid race (%u) sent by accountId: %u", race, GetAccountId());
        return;
    }

    if (!Player::IsValidGender(gender))
    {
        sLog->outError(LOG_FILTER_GENERAL, "Invalid gender (%u) sent by accountId: %u", gender, GetAccountId());
        return;
    }

    std::string const* name = GetRandomCharacterName(race, gender);
    WorldPacket data(SMSG_GENERATE_RANDOM_CHARACTER_NAME_RESULT, 10);
    data.WriteBit(0);               ///< Succes
    data.WriteBits(name->size(), 6);
    data.WriteString(name->c_str());
    SendPacket(&data);
}

void WorldSession::HandleReorderCharacters(WorldPacket& p_Packet)
{
    uint32 l_CharactersCount = p_Packet.ReadBits(9);

    std::vector<uint64> l_Guids(l_CharactersCount);
    uint8 l_Position;

    p_Packet.FlushBits();

    SQLTransaction l_Transaction = CharacterDatabase.BeginTransaction();

    for (uint8 l_I = 0; l_I < l_CharactersCount; ++l_I)
    {
        p_Packet.readPackGUID(l_Guids[l_I]);
        p_Packet >> l_Position;

        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_LIST_SLOT);
        l_Stmt->setUInt8(0, l_Position);
        l_Stmt->setUInt32(1, GUID_LOPART(l_Guids[l_I]));
        l_Transaction->Append(l_Stmt);
    }

    CharacterDatabase.CommitTransaction(l_Transaction);
}
#endif

void WorldSession::HandleSuspendToken(WorldPacket& p_RecvData)
{
    uint32 l_Token = p_RecvData.read<uint32>();

    m_Player->m_tokenCounter = l_Token;
    GetPlayer()->SendResumeToken(l_Token);
}
