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
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "GuildMgr.h"
#include "SystemConfig.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "DatabaseEnv.h"
#include "Arena.h"
#include "Chat.h"
#include "Group.h"
#include "Guild.h"
#include "GuildFinderMgr.h"
#include "Language.h"
#include "Log.h"
#include "Opcodes.h"
#include "Player.h"
#include "PlayerDump.h"
#include "SharedDefines.h"
#include "SocialMgr.h"
#include "UpdateMask.h"
#include "Util.h"
#include "ScriptMgr.h"
#include "Battleground.h"
#include "AccountMgr.h"
#include "DBCStores.h"
#include "LFGMgr.h"

class LoginQueryHolder : public SQLQueryHolder
{
    private:
        uint32 m_accountId;
        uint64 m_guid;
    public:
        LoginQueryHolder(uint32 accountId, uint64 guid)
            : m_accountId(accountId), m_guid(guid) { }
        uint64 GetGuid() const { return m_guid; }
        uint32 GetAccountId() const { return m_accountId; }
        bool Initialize();
};

bool LoginQueryHolder::Initialize()
{
    SetSize(MAX_PLAYER_LOGIN_QUERY);

    bool res = true;
    uint32 lowGuid = GUID_LOPART(m_guid);
    PreparedStatement* stmt = NULL;

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADFROM, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GROUP_MEMBER);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADGROUP, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_INSTANCE);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADBOUNDINSTANCES, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_AURAS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADAURAS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_AURAS_EFFECTS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADAURAS_EFFECTS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SPELL);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_CHAR_LOADSPELLS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_QUESTSTATUS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADQUESTSTATUS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_DAILYQUESTSTATUS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADDAILYQUESTSTATUS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_WEEKLYQUESTSTATUS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADWEEKLYQUESTSTATUS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_MONTHLYQUESTSTATUS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_MONTHLY_QUEST_STATUS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SEASONALQUESTSTATUS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADSEASONALQUESTSTATUS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_REPUTATION);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADREPUTATION, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_INVENTORY);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADINVENTORY, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_VOID_STORAGE);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADVOIDSTORAGE, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_ACTIONS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACTIONS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_MAILCOUNT);
    stmt->setUInt32(0, lowGuid);
    stmt->setUInt64(1, uint64(time(NULL)));
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADMAILCOUNT, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_MAILDATE);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADMAILDATE, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SOCIALLIST);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADSOCIALLIST, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_HOMEBIND);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADHOMEBIND, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SPELLCOOLDOWNS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADSPELLCOOLDOWNS, stmt);

    if (sWorld->getBoolConfig(CONFIG_DECLINED_NAMES_USED))
    {
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_DECLINEDNAMES);
        stmt->setUInt32(0, lowGuid);
        res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADDECLINEDNAMES, stmt);
    }

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUILD_MEMBER);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADGUILD, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_ACHIEVEMENTS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACHIEVEMENTS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_ACHIEVEMENTS);
    stmt->setUInt32(0, m_accountId);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACCOUNTACHIEVEMENTS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_CRITERIAPROGRESS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADCRITERIAPROGRESS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_CRITERIAPROGRESS);
    stmt->setUInt32(0, m_accountId);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACCOUNTCRITERIAPROGRESS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_EQUIPMENTSETS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADEQUIPMENTSETS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_ARENA_DATA);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADARENADATA, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_BGDATA);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADBGDATA, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_GLYPHS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADGLYPHS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_TALENTS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADTALENTS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PLAYER_ACCOUNT_DATA);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADACCOUNTDATA, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_SKILLS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADSKILLS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_RANDOMBG);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADRANDOMBG, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_BANNED);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADBANNED, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_QUESTSTATUSREW);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADQUESTSTATUSREW, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_INSTANCELOCKTIMES);
    stmt->setUInt32(0, m_accountId);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADINSTANCELOCKTIMES, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PLAYER_CURRENCY);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOADCURRENCY, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_ARCHAEOLOGY);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_ARCHAEOLOGY, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_ARCHAEOLOGY_PROJECTS);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_ARCHAEOLOGY_PROJECTS, stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CUF_PROFILE);
    stmt->setUInt32(0, lowGuid);
    res &= SetPreparedQuery(PLAYER_LOGIN_QUERY_LOAD_CUF_PROFILES, stmt);

    return res;
}

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


    WorldPacket l_Data(SMSG_ENUM_CHARACTERS_RESULT);

    l_Data.WriteBit(l_CanCreateCharacter);          ///< Allow char creation
    l_Data.WriteBit(0);                             ///< unk
    l_Data.FlushBits();

    l_Data << l_CharacterCount;                     ///< Account character count
    l_Data << l_FactionChangeRestrictions;          ///< Faction change restrictions

    if (p_Result)
    {
        do
        {
            uint32 l_GuidLow = (*p_Result)[0].GetUInt32();

            Player::BuildEnumData(p_Result, &l_Data);

            /// This can happen if characters are inserted into the database manually. Core hasn't loaded name data yet.
            if (!sWorld->HasCharacterNameData(l_GuidLow))
                sWorld->AddCharacterNameData(l_GuidLow, (*p_Result)[1].GetString(), (*p_Result)[4].GetUInt8(), (*p_Result)[2].GetUInt8(), (*p_Result)[3].GetUInt8(), (*p_Result)[7].GetUInt8());

            _allowedCharsToLogin.insert(l_GuidLow);
        } while (p_Result->NextRow());
    }

    for (uint32 l_I = 0; l_I < l_FactionChangeRestrictions; l_I++)
    {
        l_Data << uint32(0);                        ///< Mask
        l_Data << uint8(0);                         ///< Race ID
    }

    SendPacket(&l_Data);
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

    stmt->setUInt8(0, PET_SLOT_ACTUAL_PET_SLOT);
    stmt->setUInt32(1, GetAccountId());

    _charEnumCallback = CharacterDatabase.AsyncQuery(stmt);
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
    uint32 l_HeroicFreeSlots = sWorld->getIntConfig(CONFIG_HEROIC_CHARACTERS_PER_REALM);
    if (l_HeroicFreeSlots == 0 && AccountMgr::IsPlayerAccount(GetSecurity()) && l_CharacterClass == CLASS_DEATH_KNIGHT)
    {
        l_CreationResponse << (uint8)CHAR_CREATE_UNIQUE_CLASS_LIMIT;
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

    _charCreateCallback.SetParam(new CharacterCreateInfo(l_CharacterName, l_CharacterRace, l_CharacterClass, l_CharacterGender, l_CharacterSkin, l_CharacterFace, l_CharacterHairStyle, l_CharacterHairColor, l_CharacterFacialHair, l_CharacterOutfitID, p_RecvData));

    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHECK_NAME);
    l_Stmt->setString(0, l_CharacterName);

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

            PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_SUM_REALM_CHARACTERS);
            stmt->setUInt32(0, GetAccountId());

            _charCreateCallback.FreeResult();
            _charCreateCallback.SetFutureResult(LoginDatabase.AsyncQuery(stmt));
            _charCreateCallback.NextStage();
        }
        break;
        case 1:
        {
            uint16 acctCharCount = 0;
            if (result)
            {
                Field* fields = result->Fetch();
                // SELECT SUM(x) is MYSQL_TYPE_NEWDECIMAL - needs to be read as string
                const char* ch = fields[0].GetCString();
                if (ch)
                {
                    // Try crashfix, atoi -> std::stoi with handling of exception
                    // We have log in Pandashan.log to make better fix
                    try
                    {
                        acctCharCount = std::stoi(ch);
                    }
                    catch(std::invalid_argument& /*e*/)
                    {
                        acctCharCount = 0;
                        sLog->OutPandashan("Exception (invalid argument) throw in HandleCharCreateCallback for account %u (ch : %s)", GetAccountId(), ch);
                        KickPlayer();
                        return;
                    }
                    catch(std::out_of_range)
                    {
                        acctCharCount = 0;
                        sLog->OutPandashan("Exception (out of range) throw in HandleCharCreateCallback for account %u (ch : %s)", GetAccountId(), ch);
                        KickPlayer();
                        return;
                    }
                }
            }

            if (acctCharCount >= sWorld->getIntConfig(CONFIG_CHARACTERS_PER_ACCOUNT))
            {
                WorldPacket data(SMSG_CREATE_CHAR, 1);
                data << uint8(CHAR_CREATE_ACCOUNT_LIMIT);
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
        case 2:
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
            HandleCharCreateCallback(PreparedQueryResult(NULL), createInfo);   // Will jump to case 3
        }
        break;
        case 3:
        {
            bool haveSameRace = false;
            uint32 heroicReqLevel = sWorld->getIntConfig(CONFIG_CHARACTER_CREATING_MIN_LEVEL_FOR_HEROIC_CHARACTER);
            bool hasHeroicReqLevel = (heroicReqLevel == 0);
            bool allowTwoSideAccounts = !sWorld->IsPvPRealm() || sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_ACCOUNTS) || !AccountMgr::IsPlayerAccount(GetSecurity());
            uint32 skipCinematics = sWorld->getIntConfig(CONFIG_SKIP_CINEMATICS);

            if (result)
            {
                uint32 team = Player::TeamForRace(createInfo->Race);
                uint32 freeHeroicSlots = sWorld->getIntConfig(CONFIG_HEROIC_CHARACTERS_PER_REALM);

                Field* field = result->Fetch();
                uint8 accRace  = field[1].GetUInt8();

                if (AccountMgr::IsPlayerAccount(GetSecurity()) && createInfo->Class == CLASS_DEATH_KNIGHT)
                {
                    uint8 accClass = field[2].GetUInt8();
                    if (accClass == CLASS_DEATH_KNIGHT)
                    {
                        if (freeHeroicSlots > 0)
                            --freeHeroicSlots;

                        if (freeHeroicSlots == 0)
                        {
                            WorldPacket data(SMSG_CREATE_CHAR, 1);
                            data << uint8(CHAR_CREATE_UNIQUE_CLASS_LIMIT);
                            SendPacket(&data);
                            delete createInfo;
                            _charCreateCallback.Reset();
                            return;
                        }
                    }

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
                        uint8 acc_class = field[2].GetUInt8();
                        if (acc_class == CLASS_DEATH_KNIGHT)
                        {
                            if (freeHeroicSlots > 0)
                                --freeHeroicSlots;

                            if (freeHeroicSlots == 0)
                            {
                                WorldPacket data(SMSG_CREATE_CHAR, 1);
                                data << uint8(CHAR_CREATE_UNIQUE_CLASS_LIMIT);
                                SendPacket(&data);
                                delete createInfo;
                                _charCreateCallback.Reset();
                                return;
                            }
                        }

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

            // Avoid exploit of create multiple characters with same name
            if (!sWorld->AddCharacterName(createInfo->Name))
            {
                WorldPacket data(SMSG_CREATE_CHAR, 1);
                data << uint8(CHAR_CREATE_NAME_IN_USE);
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
            newChar.SaveToDB(true);
            createInfo->CharCount += 1;

            SQLTransaction trans = LoginDatabase.BeginTransaction();

            PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_DEL_REALM_CHARACTERS_BY_REALM);
            stmt->setUInt32(0, GetAccountId());
            stmt->setUInt32(1, realmID);
            trans->Append(stmt);

            stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_REALM_CHARACTERS);
            stmt->setUInt32(0, createInfo->CharCount);
            stmt->setUInt32(1, GetAccountId());
            stmt->setUInt32(2, realmID);
            trans->Append(stmt);

            LoginDatabase.CommitTransaction(trans);

            QueryResult result2 = CharacterDatabase.PQuery("SELECT id FROM character_pet ORDER BY id DESC LIMIT 1");
            uint32 pet_id = 1;
            if (result2)
            {
                Field* fields = result2->Fetch();
                pet_id = fields[0].GetUInt32();
                pet_id += 1;
            }

            if (createInfo->Class == CLASS_HUNTER)
            {
                switch (createInfo->Race)
                {
                    case RACE_HUMAN:
                        CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 299, %u, 903, 13481, 1, 1, 0, 0, 'Wolf', 0, 0, 192, 0, 1295727347, '7 2 7 1 7 0 129 2649 129 17253 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, newChar.GetGUIDLow());
                        break;
                    case RACE_DWARF:
                        CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 42713, %u, 822, 13481, 1, 1, 0, 0, 'Bear', 0, 0, 212, 0, 1295727650, '7 2 7 1 7 0 129 2649 129 16827 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, newChar.GetGUIDLow());
                        break;
                    case RACE_ORC:
                        CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 42719, %u, 744, 13481, 1, 1, 0, 0, 'Boar', 0, 0, 212, 0, 1295727175, '7 2 7 1 7 0 129 2649 129 17253 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, newChar.GetGUIDLow());
                        break;
                    case RACE_NIGHTELF:
                        CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 42718, %u,  17090, 13481, 1, 1, 0, 0, 'Cat', 0, 0, 192, 0, 1295727501, '7 2 7 1 7 0 129 2649 129 16827 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, newChar.GetGUIDLow());
                        break;
                    case RACE_UNDEAD_PLAYER:
                        CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 51107, %u,  368, 13481, 1, 1, 0, 0, 'Spider', 0, 0, 202, 0, 1295727821, '7 2 7 1 7 0 129 2649 129 17253 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, newChar.GetGUIDLow());
                        break;
                    case RACE_TAUREN:
                        CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 42720, %u,  29057, 13481, 1, 1, 0, 0, 'Tallstrider', 0, 0, 192, 0, 1295727912, '7 2 7 1 7 0 129 2649 129 16827 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, newChar.GetGUIDLow());
                        break;
                    case RACE_TROLL:
                        CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 42721, %u,  23518, 13481, 1, 1, 0, 0, 'Raptor', 0, 0, 192, 0, 1295727987, '7 2 7 1 7 0 129 2649 129 50498 129 16827 1 0 6 2 6 1 6 0 ', 0)", pet_id, newChar.GetGUIDLow());
                        break;
                    case RACE_GOBLIN:
                        CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 42715, %u, 27692, 13481, 1, 1, 0, 0, 'Crab', 0, 0, 212, 0, 1295720595, '7 2 7 1 7 0 129 2649 129 16827 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, newChar.GetGUIDLow());
                        break;
                    case RACE_BLOODELF:
                        CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 42710, %u, 23515, 13481, 1, 1, 0, 0, 'Dragonhawk', 0, 0, 202, 0, 1295728068, '7 2 7 1 7 0 129 2649 129 17253 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, newChar.GetGUIDLow());
                        break;
                    case RACE_DRAENEI:
                        CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 42712, %u, 29056, 13481, 1, 1, 0, 0, 'Moth', 0, 0, 192, 0, 1295728128, '7 2 7 1 7 0 129 2649 129 49966 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, newChar.GetGUIDLow());
                        break;
                    case RACE_WORGEN:
                        CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 42722, %u, 30221, 13481, 1, 1, 0, 0, 'Dog', 0, 0, 192, 0, 1295728219, '7 2 7 1 7 0 129 2649 129 17253 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, newChar.GetGUIDLow());
                        break;
                    case RACE_PANDAREN_NEUTRAL:
                        CharacterDatabase.PExecute("REPLACE INTO character_pet (`id`, `entry`, `owner`, `modelid`, `CreatedBySpell`, `PetType`, `level`, `exp`, `Reactstate`, `name`, `renamed`, `slot`, `curhealth`, `curmana`, `savetime`, `abdata`, `specialization`) VALUES (%u, 57239, %u, 42656, 13481, 1, 1, 0, 0, 'Turtle', 0, 0, 192, 0, 1295728219, '7 2 7 1 7 0 129 2649 129 17253 1 0 1 0 6 2 6 1 6 0 ', 0)", pet_id, newChar.GetGUIDLow());
                        break;
                    default:
                        break;
                }

                CharacterDatabase.PExecute("UPDATE characters SET currentPetSlot = '0', petSlotUsed = '1' WHERE guid = %u", newChar.GetGUIDLow());
                newChar.SetTemporaryUnsummonedPetNumber(pet_id);
            }

            WorldPacket data(SMSG_CREATE_CHAR, 1);
            data << uint8(CHAR_CREATE_SUCCESS);
            SendPacket(&data);

            std::string IP_str = GetRemoteAddress();
            sLog->outInfo(LOG_FILTER_CHARACTER, "Account: %d (IP: %s) Create Character:[%s] (GUID: %u)", GetAccountId(), IP_str.c_str(), createInfo->Name.c_str(), newChar.GetGUIDLow());
            sScriptMgr->OnPlayerCreate(&newChar);
            sWorld->AddCharacterNameData(newChar.GetGUIDLow(), std::string(newChar.GetName()), newChar.getGender(), newChar.getRace(), newChar.getClass(), newChar.getLevel());

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
    }

    // prevent deleting other players' characters using cheating tools
    if (accountId != GetAccountId())
        return;

    std::string IP_str = GetRemoteAddress();
    sLog->outInfo(LOG_FILTER_CHARACTER, "Account: %d (IP: %s) Delete Character:[%s] (GUID: %u)", GetAccountId(), IP_str.c_str(), name.c_str(), GUID_LOPART(charGuid));
    sScriptMgr->OnPlayerDelete(charGuid);
    sWorld->DeleteCharacterNameData(GUID_LOPART(charGuid));

    if (sLog->ShouldLog(LOG_FILTER_PLAYER_DUMP, LOG_LEVEL_INFO)) // optimize GetPlayerDump call
    {
        std::string dump;
        if (PlayerDumpWriter().GetDump(GUID_LOPART(charGuid), dump))

            sLog->outCharDump(dump.c_str(), GetAccountId(), GUID_LOPART(charGuid), name.c_str());
    }

    sGuildFinderMgr->RemoveAllMembershipRequestsFromPlayer(charGuid);
    Player::DeleteFromDB(charGuid, GetAccountId());
    sWorld->DeleteCharName(name);

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
        sLog->OutPandashan("Player kicked due to flood of CMSG_PLAYER_LOGIN");
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

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd Player Logon Message");
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Character (Guid: %u) logging in", GUID_LOPART(l_PlayerGuid));

    if (!CharCanLogin(GUID_LOPART(l_PlayerGuid)))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Account (%u) can't login with that character (%u).", GetAccountId(), GUID_LOPART(l_PlayerGuid));
        KickPlayer();

        return;
    }

    LoginQueryHolder * l_LoginQueryHolder = new LoginQueryHolder(GetAccountId(), l_PlayerGuid);

    if (!l_LoginQueryHolder->Initialize())
    {
        delete l_LoginQueryHolder;                                      // delete all unprocessed queries
        m_playerLoading = false;
        return;
    }

    m_CharacterLoginCallback = CharacterDatabase.DelayQueryHolder((SQLQueryHolder*)l_LoginQueryHolder);

    PreparedStatement* l_Stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_CHARACTER_SPELL);
    l_Stmt->setUInt32(0, GetAccountId());

    m_AccountSpellCallback = LoginDatabase.AsyncQuery(l_Stmt);
}

void WorldSession::HandleLoadScreenOpcode(WorldPacket& recvPacket)
{
    sLog->outInfo(LOG_FILTER_GENERAL, "WORLD: Recvd CMSG_LOAD_SCREEN");
    uint32 mapID;

    recvPacket >> mapID;
    recvPacket.ReadBit();
}

void WorldSession::HandlePlayerLogin(LoginQueryHolder* holder, PreparedQueryResult accountResult)
{
    uint64 playerGuid = holder->GetGuid();

    Player* pCurrChar = new Player(this);
     // for send server info and strings (config)
    ChatHandler chH = ChatHandler(pCurrChar);

    uint32 time = getMSTime();

    // "GetAccountId() == db stored account id" checked in LoadFromDB (prevent login not own character using cheating tools)
    if (!pCurrChar->LoadFromDB(GUID_LOPART(playerGuid), holder, accountResult))
    {
        SetPlayer(NULL);
        KickPlayer();                                       // disconnect client, player no set to session and it will not deleted or saved at kick
        delete pCurrChar;                                   // delete it manually
        delete holder;                                      // delete all unprocessed queries
        m_playerLoading = false;
        return;
    }

    uint32 time1 = getMSTime() - time;

    pCurrChar->GetMotionMaster()->Initialize();
    pCurrChar->SendDungeonDifficulty(false);

    WorldPacket l_Data(SMSG_RESUME_TOKEN, 5);
    l_Data << uint32(0);
    l_Data << uint8(0x80);
    SendPacket(&l_Data);

    l_Data.Initialize(SMSG_LOGIN_VERIFY_WORLD, 20);
    l_Data << pCurrChar->GetMapId();                                        ///< uint32
    l_Data << pCurrChar->GetPositionX();                                    ///< float
    l_Data << pCurrChar->GetPositionY();                                    ///< float
    l_Data << pCurrChar->GetPositionZ();                                    ///< float
    l_Data << pCurrChar->GetOrientation();                                  ///< float
    l_Data << uint32(0);                                                    ///< uint32 => TransferSpellID
    SendPacket(&l_Data);

    // load player specific part before send times
    LoadAccountData(holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOADACCOUNTDATA), PER_CHARACTER_CACHE_MASK);
    SendAccountDataTimes(pCurrChar->GetGUID());

    bool l_EuropaTicketSystemEnabled            = true;
    bool l_PlayTimeAlert                        = false;
    bool l_ScrollOfResurrectionEnabled          = false;
    bool l_VoiceChatSystemEnabled               = false;
    bool l_ItemRestorationButtonEnbaled         = false;
    bool l_RecruitAFriendSystem                 = false;
    bool l_HasTravelPass                        = false;
    bool l_WebTicketSystemStatus                = false;
    bool l_StoreEnabled                         = false;
    bool l_StoreIsDisabledByParentalControls    = false;
    bool l_StoreIsAvailable                     = true;

    uint32 l_PlayTimeAlertDisplayAlertTime      = 0;
    uint32 l_PlayTimeAlertDisplayAlertDelay     = 0;
    uint32 l_PlayTimeAlertDisplayAlertPeriod    = 0;

    uint32 l_SORRemaining = 1;
    uint32 l_SORMaxPerDay = 1;

    uint32 l_ConfigRealmRecordID    = 640;
    uint32 l_ConfigRealmID          = realmID;

    uint32 l_ComplainSystemStatus = 2;                              ///< 0 - Disabled | 1 - Calendar & Mail | 2 - Calendar & Mail & Ignoring system

    l_Data.Initialize(SMSG_FEATURE_SYSTEM_STATUS, 7);

    l_Data << uint8(l_ComplainSystemStatus);                        ///< Complaints System Status
    l_Data << uint32(l_SORMaxPerDay);                               ///< Max SOR Per day
    l_Data << uint32(l_SORRemaining);                               ///< SOR remaining
    l_Data << uint32(l_ConfigRealmID);                              ///< Config Realm ID
    l_Data << uint32(l_ConfigRealmRecordID);                        ///< Config Realm Record ID (used for url dbc reading)

    l_Data.WriteBit(l_VoiceChatSystemEnabled);                      ///< voice Chat System Status
    l_Data.WriteBit(l_EuropaTicketSystemEnabled);                   ///< Europa Ticket System Enabled
    l_Data.WriteBit(l_ScrollOfResurrectionEnabled);                 ///< Scroll Of Resurrection Enabled
    l_Data.WriteBit(l_StoreEnabled);                                ///< Store system status
    l_Data.WriteBit(l_StoreIsAvailable);                            ///< Can purchase in store
    l_Data.WriteBit(l_StoreIsDisabledByParentalControls);           ///< Is store disabled by parental controls
    l_Data.WriteBit(l_ItemRestorationButtonEnbaled);                ///< Item Restoration Button Enabled
    l_Data.WriteBit(l_WebTicketSystemStatus);                       ///< Web ticket system enabled
    l_Data.WriteBit(l_PlayTimeAlert);                               ///< Session Alert Enabled
    l_Data.WriteBit(l_RecruitAFriendSystem);                        ///< Recruit A Friend System Status
    l_Data.WriteBit(l_HasTravelPass);                               ///< Has travel pass (can group with cross-realm Battle.net friends.)
    l_Data.FlushBits();

    if (l_EuropaTicketSystemEnabled)
    {
        l_Data << uint32(0);
        l_Data << uint32(60);
        l_Data << uint32(10);
        l_Data << uint32(1);
    }

    if (l_PlayTimeAlert)
    {
        l_Data << uint32(l_PlayTimeAlertDisplayAlertDelay);       ///< Alert delay
        l_Data << uint32(l_PlayTimeAlertDisplayAlertPeriod);      ///< Alert period
        l_Data << uint32(l_PlayTimeAlertDisplayAlertTime);        ///< Alert display time
    }

    SendPacket(&l_Data);

    uint32 time2 = getMSTime() - time1;

    // Send MOTD
    {
        std::string l_MotdStr = sWorld->GetMotd();
        std::string::size_type l_Position, l_NextPosition;
        std::vector<std::string> l_Lines;
        uint32 l_LineCount = 0;

        l_Data.Initialize(SMSG_MOTD, 50);                     // new in 2.0.1

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

        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent motd (SMSG_MOTD)");

        // send server info
        if (sWorld->getIntConfig(CONFIG_ENABLE_SINFO_LOGIN) == 1)
            chH.PSendSysMessage(_FULLVERSION);

        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent server info");
    }

    const static std::string l_TimeZoneName = "Europe/Paris";

    l_Data.Initialize(SMSG_SET_TIME_ZONE_INFORMATION, 26);
    l_Data.WriteBits(l_TimeZoneName.size(), 7);
    l_Data.WriteBits(l_TimeZoneName.size(), 7);
    l_Data.FlushBits();

    l_Data.WriteString(l_TimeZoneName);
    l_Data.WriteString(l_TimeZoneName);

    SendPacket(&l_Data);

    if (sWorld->getBoolConfig(CONFIG_ARENA_SEASON_IN_PROGRESS))
    {
        l_Data.Initialize(SMSG_SET_ARENA_SEASON, 8);
        l_Data << uint32(sWorld->getIntConfig(CONFIG_ARENA_SEASON_ID) - 1);
        l_Data << uint32(sWorld->getIntConfig(CONFIG_ARENA_SEASON_ID));
        SendPacket(&l_Data);
    }

    //QueryResult* result = CharacterDatabase.PQuery("SELECT guildid, rank FROM guild_member WHERE guid = '%u'", pCurrChar->GetGUIDLow());
    if (PreparedQueryResult resultGuild = holder->GetPreparedResult(PLAYER_LOGIN_QUERY_LOADGUILD))
    {
        Field* fields = resultGuild->Fetch();
        pCurrChar->SetInGuild(fields[0].GetUInt32());
        pCurrChar->SetRank(fields[1].GetUInt8());
        if (Guild* guild = sGuildMgr->GetGuildById(pCurrChar->GetGuildId()))
            pCurrChar->SetGuildLevel(guild->GetLevel());
    }
    else if (pCurrChar->GetGuildId())                        // clear guild related fields in case wrong data about non existed membership
    {
        pCurrChar->SetInGuild(0);
        pCurrChar->SetRank(0);
        pCurrChar->SetGuildLevel(0);
    }

    l_Data.Initialize(SMSG_LEARNED_DANCE_MOVES, 4+4);
    l_Data << uint64(0);
    SendPacket(&l_Data);

    l_Data.Initialize(SMSG_HOTFIX_INFO);
    HotfixData const& hotfix = sObjectMgr->GetHotfixData();
    l_Data.WriteBits(hotfix.size(), 20);
    l_Data.FlushBits();
    for (uint32 i = 0; i < hotfix.size(); ++i)
    {
        l_Data << uint32(hotfix[i].Entry);
        l_Data << uint32(hotfix[i].Timestamp);
        l_Data << uint32(hotfix[i].Type);
    }
    SendPacket(&l_Data);

    uint32 time3 = getMSTime() - time2;

    // Send item extended costs hotfix
    std::set<uint32> extendedCostHotFix = sObjectMgr->GetOverwriteExtendedCosts();
    for (auto itr : extendedCostHotFix)
    {
        const ItemExtendedCostEntry* extendedCost = sItemExtendedCostStore.LookupEntry(itr);

        if (!extendedCost)
            continue;

        WorldPacket data(SMSG_DB_REPLY);
        ByteBuffer buff;

        buff << uint32(extendedCost->ID);
        buff << uint32(0); // reqhonorpoints
        buff << uint32(0); // reqarenapoints
        buff << uint32(extendedCost->RequiredArenaSlot);

        for (uint32 i = 0; i < MAX_ITEM_EXT_COST_ITEMS; i++)
            buff << uint32(extendedCost->RequiredItem[i]);

        for (uint32 i = 0; i < MAX_ITEM_EXT_COST_ITEMS; i++)
            buff << uint32(extendedCost->RequiredItemCount[i]);

        buff << uint32(extendedCost->RequiredPersonalArenaRating);
        buff << uint32(0); // ItemPurchaseGroup

        for (uint32 i = 0; i < MAX_ITEM_EXT_COST_CURRENCIES; i++)
            buff << uint32(extendedCost->RequiredCurrency[i]);

        for (uint32 i = 0; i < MAX_ITEM_EXT_COST_CURRENCIES; i++)
            buff << uint32(extendedCost->RequiredCurrencyCount[i]);

        // Unk
        for (uint32 i = 0; i < MAX_ITEM_EXT_COST_CURRENCIES; i++)
            buff << uint32(0);

        data << uint32(buff.size());
        data.append(buff);

        data << uint32(DB2_REPLY_ITEM_EXTENDED_COST);
        data << uint32(sObjectMgr->GetHotfixDate(extendedCost->ID, DB2_REPLY_ITEM_EXTENDED_COST));
        data << uint32(extendedCost->ID);

        SendPacket(&data);
    }

    pCurrChar->SendInitialPacketsBeforeAddToMap();

    //Show cinematic at the first time that player login
    if (!pCurrChar->getCinematic())
    {
        pCurrChar->setCinematic(1);

        if (ChrClassesEntry const* cEntry = sChrClassesStore.LookupEntry(pCurrChar->getClass()))
        {
            if (cEntry->CinematicSequence)
                pCurrChar->SendCinematicStart(cEntry->CinematicSequence);
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

    uint32 time4 = getMSTime() - time3;

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

    if (pCurrChar->GetGuildId() != 0)
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

    uint32 time5 = getMSTime() - time4;

    pCurrChar->SendInitialPacketsAfterAddToMap();

    CharacterDatabase.PExecute("UPDATE characters SET online = 1 WHERE guid = '%u'", pCurrChar->GetGUIDLow());
    LoginDatabase.PExecute("UPDATE account SET online = 1 WHERE id = '%u'", GetAccountId());
    pCurrChar->SetInGameTime(getMSTime());

    uint32 time6 = getMSTime() - time5;

    // announce group about member online (must be after add to player list to receive announce to self)
    if (Group* group = pCurrChar->GetGroup())
    {
        //pCurrChar->groupInfo.group->SendInit(this); // useless
        group->SendUpdate();
        group->ResetMaxEnchantingLevel();
    }

    // friend status
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

        pCurrChar->SendMovementSetWaterWalking(true);
    }

    pCurrChar->ContinueTaxiFlight();
    //pCurrChar->LoadPet();
    uint32 time7 = getMSTime() - time6;

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
        SendNotification(LANG_RESET_TALENTS);
    }

    if (pCurrChar->HasAtLoginFlag(AT_LOGIN_FIRST))
        pCurrChar->RemoveAtLoginFlag(AT_LOGIN_FIRST);

    // show time before shutdown if shutdown planned.
    if (sWorld->IsShuttingDown())
        sWorld->ShutdownMsg(true, pCurrChar);

    if (sWorld->getBoolConfig(CONFIG_ALL_TAXI_PATHS))
        pCurrChar->SetTaxiCheater(true);

    if (pCurrChar->isGameMaster())
        SendNotification(LANG_GM_ON);

    pCurrChar->SendCUFProfiles();

    uint32 time8 = getMSTime() - time7;

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

    sScriptMgr->OnPlayerLogin(pCurrChar);

    uint32 time9 = getMSTime() - time8;

    uint32 totalTime = getMSTime() - time;
    if (totalTime > 50)
        sLog->OutPandashan("HandlePlayerLogin |****---> time1 : %u | time 2 : %u | time 3 : %u | time 4 : %u | time 5: %u | time 6 : %u | time 7 : %u | time 8 : %u | time 9 : %u | totaltime : %u", time1, time2, time3, time4, time5, time6, time7, time8, time9, totalTime);

    // Fix chat with transfert / rename
    sWorld->AddCharacterNameData(pCurrChar->GetGUIDLow(), pCurrChar->GetName(), pCurrChar->getGender(), pCurrChar->getRace(), pCurrChar->getClass(), pCurrChar->getLevel());

    delete holder;
}

void WorldSession::HandleSetFactionAtWar(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_SET_FACTION_ATWAR");

    uint8 repListID;

    recvData >> repListID;

    GetPlayer()->GetReputationMgr().SetAtWar(repListID, true);
}

void WorldSession::HandleUnSetFactionAtWar(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_UNSET_FACTION_ATWAR");

    uint8 repListID;

    recvData >> repListID;

    GetPlayer()->GetReputationMgr().SetAtWar(repListID, false);
}

//I think this function is never used :/ I dunno, but i guess this opcode not exists
void WorldSession::HandleSetFactionCheat(WorldPacket& /*recvData*/)
{
    sLog->outError(LOG_FILTER_NETWORKIO, "WORLD SESSION: HandleSetFactionCheat, not expected call, please report.");
    GetPlayer()->GetReputationMgr().SendStates();
}

void WorldSession::HandleTutorialFlag(WorldPacket& recvData)
{
    uint32 data;
    recvData >> data;

    uint8 index = uint8(data / 32);
    if (index >= MAX_ACCOUNT_TUTORIAL_VALUES)
        return;

    uint32 value = (data % 32);

    uint32 flag = GetTutorialInt(index);
    flag |= (1 << value);
    SetTutorialInt(index, flag);
}

void WorldSession::HandleTutorialClear(WorldPacket& /*recvData*/)
{
    for (uint8 i = 0; i < MAX_ACCOUNT_TUTORIAL_VALUES; ++i)
        SetTutorialInt(i, 0xFFFFFFFF);
}

void WorldSession::HandleTutorialReset(WorldPacket& /*recvData*/)
{
    for (uint8 i = 0; i < MAX_ACCOUNT_TUTORIAL_VALUES; ++i)
        SetTutorialInt(i, 0x00000000);
}

void WorldSession::HandleSetWatchedFactionOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_SET_WATCHED_FACTION");
    uint32 fact;
    recvData >> fact;
    GetPlayer()->SetUInt32Value(PLAYER_FIELD_WATCHED_FACTION_INDEX, fact);
}

void WorldSession::HandleSetFactionInactiveOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_SET_FACTION_INACTIVE");
    uint32 replistid;
    bool inactive;

    recvData >> replistid;
    inactive = recvData.ReadBit();

    m_Player->GetReputationMgr().SetInactive(replistid, inactive);
}

void WorldSession::HandleShowAccountAchievement(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_SHOW_ACCOUNT_ACHIEVEMENT for %s", m_Player->GetName());

    bool showing = recvData.ReadBit();
}

void WorldSession::HandleShowingHelmOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_SHOWING_HELM for %s", m_Player->GetName());
    recvData.read_skip<uint8>(); // unknown, bool?
    m_Player->ToggleFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HIDE_HELM);
}

void WorldSession::HandleShowingCloakOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_SHOWING_CLOAK for %s", m_Player->GetName());
    recvData.read_skip<uint8>(); // unknown, bool?
    m_Player->ToggleFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HIDE_CLOAK);
}

void WorldSession::HandleCharRenameOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;
    uint32 nameLen = 0;
    std::string newName;

    guid[3] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    nameLen = recvData.ReadBits(6);
    guid[4] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();

    recvData.FlushBits();

    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[1]);
    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[2]);
    newName = recvData.ReadString(nameLen);
    recvData.ReadByteSeq(guid[6]);

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
        BuildCharacterRename(&data, guid, res, newName);
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

    stmt->setUInt32(0, GUID_LOPART(guid));
    stmt->setUInt32(1, GetAccountId());
    stmt->setUInt16(2, AT_LOGIN_RENAME);
    stmt->setUInt16(3, AT_LOGIN_RENAME);
    stmt->setString(4, newName);

    _charRenameCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
}

void WorldSession::BuildCharacterRename(WorldPacket* pkt, ObjectGuid guid, uint8 result, std::string name)
{
    pkt->WriteBit(guid != 0);

    if (guid)
    {
        uint8 bitsOrder[8] = {3, 4, 7, 2, 6, 5, 1, 0};
        pkt->WriteBitInOrder(guid, bitsOrder);
    }

    pkt->WriteBit(name.empty());

    if (!name.empty())
        pkt->WriteBits(name.size(), 6);

    pkt->FlushBits();

    if (!name.empty())
        pkt->WriteString(name);

    if (guid)
    {
        uint8 bytesOrder[8] = {0, 1, 7, 3, 5, 6, 4, 2};
        pkt->WriteBytesSeq(guid, bytesOrder);
    }

    *pkt << uint8(result);
}

void WorldSession::HandleChangePlayerNameOpcodeCallBack(PreparedQueryResult result, std::string newName)
{
    if (!result)
    {
        WorldPacket data(SMSG_CHAR_RENAME, 1);
        data << uint8(CHAR_CREATE_ERROR);
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

    sWorld->UpdateCharacterNameData(guidLow, newName);
}

void WorldSession::HandleSetPlayerDeclinedNames(WorldPacket& recvData)
{
    uint64 guid;
    recvData >> guid;

    // not accept declined names for unsupported languages
    std::string name;
    if (!sObjectMgr->GetPlayerNameByGUID(guid, name))
    {
        SendPlayerDeclinedNamesResult(guid, 1);
        return;
    }

    std::wstring wname;
    if (!Utf8toWStr(name, wname))
    {
        SendPlayerDeclinedNamesResult(guid, 1);
        return;
    }

    if (!isCyrillicCharacter(wname[0]))                      // name already stored as only single alphabet using
    {
        SendPlayerDeclinedNamesResult(guid, 1);
        return;
    }

    std::string name2;
    DeclinedName declinedname;

    recvData >> name2;

    if (name2 != name)                                       // character have different name
    {
        SendPlayerDeclinedNamesResult(guid, 1);
        return;
    }

    for (int i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
    {
        recvData >> declinedname.name[i];
        if (!normalizePlayerName(declinedname.name[i]))
        {
            SendPlayerDeclinedNamesResult(guid, 1);
            return;
        }
    }

    if (!ObjectMgr::CheckDeclinedNames(wname, declinedname))
    {
        SendPlayerDeclinedNamesResult(guid, 1);
        return;
    }

    for (int i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
        CharacterDatabase.EscapeString(declinedname.name[i]);

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_DECLINED_NAME);
    stmt->setUInt32(0, GUID_LOPART(guid));
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_DECLINED_NAME);
    stmt->setUInt32(0, GUID_LOPART(guid));

    for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; i++)
        stmt->setString(i+1, declinedname.name[i]);

    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);

    SendPlayerDeclinedNamesResult(guid, 0);
}

void WorldSession::SendPlayerDeclinedNamesResult(uint64 guid, uint32 result)
{
    WorldPacket data(SMSG_SET_PLAYER_DECLINED_NAMES_RESULT, 4+8);
    data << uint32(result);
    data << uint64(guid);
    SendPacket(&data);
}

void WorldSession::HandleAlterAppearance(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_ALTER_APPEARANCE");

    uint32 Hair, Color, FacialHair, SkinColor;
    recvData >> Color >> FacialHair >> SkinColor >> Hair;

    BarberShopStyleEntry const* bs_hair = sBarberShopStyleStore.LookupEntry(Hair);
    if (!bs_hair || bs_hair->type != 0 || bs_hair->race != m_Player->getRace() || bs_hair->gender != m_Player->getGender())
        return;

    BarberShopStyleEntry const* bs_facialHair = sBarberShopStyleStore.LookupEntry(FacialHair);
    if (!bs_facialHair || bs_facialHair->type != 2 || bs_facialHair->race != m_Player->getRace() || bs_facialHair->gender != m_Player->getGender())
        return;

    BarberShopStyleEntry const* bs_skinColor = sBarberShopStyleStore.LookupEntry(SkinColor);
    if (bs_skinColor && (bs_skinColor->type != 3 || bs_skinColor->race != m_Player->getRace() || bs_skinColor->gender != m_Player->getGender()))
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

    uint32 cost = m_Player->GetBarberShopCost(bs_hair->hair_id, Color, bs_facialHair->hair_id, bs_skinColor);

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

    m_Player->SetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, 2, uint8(bs_hair->hair_id));
    m_Player->SetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, 3, uint8(Color));
    m_Player->SetByteValue(PLAYER_FIELD_REST_STATE, 0, uint8(bs_facialHair->hair_id));
    if (bs_skinColor)
        m_Player->SetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, 0, uint8(bs_skinColor->hair_id));

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

void WorldSession::HandleCharCustomize(WorldPacket& recvData)
{
    ObjectGuid playerGuid;
    uint8 gender, skin, face, hairStyle, hairColor, facialHair;
    std::string newName;
    uint32 nameLen;

    recvData >> gender >> hairColor >> facialHair >> skin >> face >> hairStyle;

    playerGuid[0] = recvData.ReadBit();
    playerGuid[3] = recvData.ReadBit();
    playerGuid[4] = recvData.ReadBit();
    playerGuid[5] = recvData.ReadBit();
    playerGuid[6] = recvData.ReadBit();
    nameLen = recvData.ReadBits(6);
    playerGuid[2] = recvData.ReadBit();
    playerGuid[7] = recvData.ReadBit();
    playerGuid[1] = recvData.ReadBit();
    recvData.FlushBits();
    newName = recvData.ReadString(nameLen);

    uint8 bytes[8] = { 6, 3, 1, 4, 7, 2, 5, 0 };
    recvData.ReadBytesSeq(playerGuid, bytes);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_AT_LOGIN);
    stmt->setUInt32(0, GUID_LOPART(playerGuid));
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
    {
        WorldPacket data(SMSG_CHAR_CUSTOMIZE, 1);

        uint8 bits[8] = { 0, 5, 2, 4, 6, 7, 3, 1 };
        data.WriteBitInOrder(playerGuid, bits);

        data.WriteByteSeq(playerGuid[7]);
        data.WriteByteSeq(playerGuid[1]);
        data.WriteByteSeq(playerGuid[0]);
        data.WriteByteSeq(playerGuid[5]);
        data.WriteByteSeq(playerGuid[2]);
        data << uint8(CHAR_CREATE_ERROR);
        data.WriteByteSeq(playerGuid[6]);
        data.WriteByteSeq(playerGuid[4]);
        data.WriteByteSeq(playerGuid[3]);

        SendPacket(&data);
        return;
    }

    Field* fields = result->Fetch();
    uint32 at_loginFlags = fields[0].GetUInt16();

    if (!(at_loginFlags & AT_LOGIN_CUSTOMIZE))
    {
        WorldPacket data(SMSG_CHAR_CUSTOMIZE, 1);

        uint8 bits[8] = { 0, 5, 2, 4, 6, 7, 3, 1 };
        data.WriteBitInOrder(playerGuid, bits);

        data.WriteByteSeq(playerGuid[7]);
        data.WriteByteSeq(playerGuid[1]);
        data.WriteByteSeq(playerGuid[0]);
        data.WriteByteSeq(playerGuid[5]);
        data.WriteByteSeq(playerGuid[2]);
        data << uint8(CHAR_CREATE_ERROR);
        data.WriteByteSeq(playerGuid[6]);
        data.WriteByteSeq(playerGuid[4]);
        data.WriteByteSeq(playerGuid[3]);

        SendPacket(&data);
        return;
    }

    // prevent character rename to invalid name
    if (!normalizePlayerName(newName))
    {
        WorldPacket data(SMSG_CHAR_CUSTOMIZE, 1);

        uint8 bits[8] = { 0, 5, 2, 4, 6, 7, 3, 1 };
        data.WriteBitInOrder(playerGuid, bits);

        data.WriteByteSeq(playerGuid[7]);
        data.WriteByteSeq(playerGuid[1]);
        data.WriteByteSeq(playerGuid[0]);
        data.WriteByteSeq(playerGuid[5]);
        data.WriteByteSeq(playerGuid[2]);
        data << uint8(CHAR_NAME_NO_NAME);
        data.WriteByteSeq(playerGuid[6]);
        data.WriteByteSeq(playerGuid[4]);
        data.WriteByteSeq(playerGuid[3]);

        SendPacket(&data);
        return;
    }

    uint8 res = ObjectMgr::CheckPlayerName(newName, true);
    if (res != CHAR_NAME_SUCCESS)
    {
        WorldPacket data(SMSG_CHAR_CUSTOMIZE, 1);

        uint8 bits[8] = { 0, 5, 2, 4, 6, 7, 3, 1 };
        data.WriteBitInOrder(playerGuid, bits);

        data.WriteByteSeq(playerGuid[7]);
        data.WriteByteSeq(playerGuid[1]);
        data.WriteByteSeq(playerGuid[0]);
        data.WriteByteSeq(playerGuid[5]);
        data.WriteByteSeq(playerGuid[2]);
        data << uint8(res);
        data.WriteByteSeq(playerGuid[6]);
        data.WriteByteSeq(playerGuid[4]);
        data.WriteByteSeq(playerGuid[3]);

        SendPacket(&data);
        return;
    }

    // check name limitations
    if (AccountMgr::IsPlayerAccount(GetSecurity()) && sObjectMgr->IsReservedName(newName))
    {
        WorldPacket data(SMSG_CHAR_CUSTOMIZE, 1);

        uint8 bits[8] = { 0, 5, 2, 4, 6, 7, 3, 1 };
        data.WriteBitInOrder(playerGuid, bits);

        data.WriteByteSeq(playerGuid[7]);
        data.WriteByteSeq(playerGuid[1]);
        data.WriteByteSeq(playerGuid[0]);
        data.WriteByteSeq(playerGuid[5]);
        data.WriteByteSeq(playerGuid[2]);
        data << uint8(CHAR_NAME_RESERVED);
        data.WriteByteSeq(playerGuid[6]);
        data.WriteByteSeq(playerGuid[4]);
        data.WriteByteSeq(playerGuid[3]);

        SendPacket(&data);
        return;
    }

    // character with this name already exist
    if (uint64 newguid = sObjectMgr->GetPlayerGUIDByName(newName))
    {
        if (newguid != playerGuid)
        {
            WorldPacket data(SMSG_CHAR_CUSTOMIZE, 1);

            uint8 bits[8] = { 0, 5, 2, 4, 6, 7, 3, 1 };
            data.WriteBitInOrder(playerGuid, bits);

            data.WriteByteSeq(playerGuid[7]);
            data.WriteByteSeq(playerGuid[1]);
            data.WriteByteSeq(playerGuid[0]);
            data.WriteByteSeq(playerGuid[5]);
            data.WriteByteSeq(playerGuid[2]);
            data << uint8(CHAR_CREATE_NAME_IN_USE);
            data.WriteByteSeq(playerGuid[6]);
            data.WriteByteSeq(playerGuid[4]);
            data.WriteByteSeq(playerGuid[3]);

            SendPacket(&data);
            return;
        }
    }

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_NAME);
    stmt->setUInt32(0, GUID_LOPART(playerGuid));
    result = CharacterDatabase.Query(stmt);

    if (result)
    {
        std::string oldname = result->Fetch()[0].GetString();
        sLog->outInfo(LOG_FILTER_CHARACTER, "Account: %d (IP: %s), Character[%s] (guid:%u) Customized to: %s", GetAccountId(), GetRemoteAddress().c_str(), oldname.c_str(), GUID_LOPART(playerGuid), newName.c_str());
    }

    Player::Customize(playerGuid, gender, skin, face, hairStyle, hairColor, facialHair);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_NAME_AT_LOGIN);

    stmt->setString(0, newName);
    stmt->setUInt16(1, uint16(AT_LOGIN_CUSTOMIZE));
    stmt->setUInt32(2, GUID_LOPART(playerGuid));

    CharacterDatabase.Execute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_DECLINED_NAME);

    stmt->setUInt32(0, GUID_LOPART(playerGuid));

    CharacterDatabase.Execute(stmt);

    sWorld->UpdateCharacterNameData(GUID_LOPART(playerGuid), newName, gender);

    WorldPacket data(SMSG_CHAR_CUSTOMIZE, 17 + newName.size());

    uint8 bits[8] = { 0, 5, 2, 4, 6, 7, 3, 1 };
    data.WriteBitInOrder(playerGuid, bits);

    data.WriteByteSeq(playerGuid[7]);
    data.WriteByteSeq(playerGuid[1]);
    data.WriteByteSeq(playerGuid[0]);
    data.WriteByteSeq(playerGuid[5]);
    data.WriteByteSeq(playerGuid[2]);
    data << uint8(RESPONSE_SUCCESS);
    data.WriteByteSeq(playerGuid[6]);
    data.WriteByteSeq(playerGuid[4]);
    data << uint8(skin);
    data << uint8(hairColor);
    data << uint8(facialHair);
    data << uint8(face);
    data << uint8(hairStyle);
    data << uint8(gender);
    data.WriteByteSeq(playerGuid[3]);
    data.WriteBits(newName.size(), 6);
    data.FlushBits();
    data.append(newName.c_str(), newName.size());

    SendPacket(&data);
}

void WorldSession::HandleEquipmentSetSave(WorldPacket& recvData)
{
    ObjectGuid setGuid;
    uint32 index;
    EquipmentSet eqSet;
    uint8 iconNameLen, setNameLen;

    recvData >> index;

    if (index >= MAX_EQUIPMENT_SET_INDEX)                    // client set slots amount
        return;

    setGuid[7] = recvData.ReadBit();

    ObjectGuid* itemGuid;
    itemGuid = new ObjectGuid[EQUIPMENT_SLOT_END];

    for (uint32 i = 0; i < EQUIPMENT_SLOT_END; ++i)
    {
        uint8 bitsOrder[8] = { 4, 6, 0, 1, 3, 5, 2, 7 };
        recvData.ReadBitInOrder(itemGuid[i], bitsOrder);
    }

    setGuid[4] = recvData.ReadBit();
    setGuid[1] = recvData.ReadBit();
    setGuid[5] = recvData.ReadBit();
    setGuid[0] = recvData.ReadBit();
    setGuid[3] = recvData.ReadBit();
    setGuid[2] = recvData.ReadBit();

    iconNameLen = 2 * recvData.ReadBits(8);
    bool pair = recvData.ReadBit();

    if (pair)
        iconNameLen++;

    setNameLen = recvData.ReadBits(8);
    setGuid[6] = recvData.ReadBit();
    recvData.FlushBits();

    for (uint32 i = 0; i < EQUIPMENT_SLOT_END; ++i)
    {
        uint8 bytesOrder[8] = { 6, 1, 4, 0, 3, 5, 7, 2 };
        recvData.ReadBytesSeq(itemGuid[i], bytesOrder);

        // equipment manager sends "1" (as raw GUID) for slots set to "ignore" (don't touch slot at equip set)
        if (itemGuid[i] == 1)
        {
            // ignored slots saved as bit mask because we have no free special values for Items[i]
            eqSet.IgnoreMask |= 1 << i;
            continue;
        }

        Item* item = m_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);

        if (!item && itemGuid[i])                               // cheating check 1
            return;

        if (item && item->GetGUID() != itemGuid[i])             // cheating check 2
            return;

        eqSet.Items[i] = GUID_LOPART(itemGuid[i]);
    }

    recvData.ReadByteSeq(setGuid[4]);
    recvData.ReadByteSeq(setGuid[3]);

    std::string name, iconName;

    iconName = recvData.ReadString(iconNameLen);
    name = recvData.ReadString(setNameLen);

    recvData.ReadByteSeq(setGuid[5]);
    recvData.ReadByteSeq(setGuid[0]);
    recvData.ReadByteSeq(setGuid[1]);
    recvData.ReadByteSeq(setGuid[7]);
    recvData.ReadByteSeq(setGuid[6]);
    recvData.ReadByteSeq(setGuid[2]);

    eqSet.Guid      = setGuid;
    eqSet.Name      = name;
    eqSet.IconName  = iconName;
    eqSet.state     = EQUIPMENT_SET_NEW;

    m_Player->SetEquipmentSet(index, eqSet);

    delete[] itemGuid;
    itemGuid = 0;
}

void WorldSession::HandleEquipmentSetDelete(WorldPacket& recvData)
{
    ObjectGuid setGuid;

    uint8 bitsOrder[8] = { 4, 1, 7, 0, 5, 6, 3, 2 };
    recvData.ReadBitInOrder(setGuid, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 5, 1, 3, 4, 2, 0, 7, 6 };
    recvData.ReadBytesSeq(setGuid, bytesOrder);

    m_Player->DeleteEquipmentSet(setGuid);
}

void WorldSession::HandleEquipmentSetUse(WorldPacket& recvData)
{
    uint8* srcbag = new uint8[EQUIPMENT_SLOT_END];;
    uint8* srcslot = new uint8[EQUIPMENT_SLOT_END];

    ObjectGuid* itemGuid = new ObjectGuid[EQUIPMENT_SLOT_END];

    EquipmentSlots startSlot = m_Player->isInCombat() ? EQUIPMENT_SLOT_MAINHAND : EQUIPMENT_SLOT_START;

    for (uint8 i = 0; i < EQUIPMENT_SLOT_END; ++i)
        recvData >> srcslot[i] >> srcbag[i];

    for (uint32 i = 0; i < EQUIPMENT_SLOT_END; ++i)
    {
        uint8 bitsOrder[8] = { 2, 5, 0, 1, 3, 6, 4, 7 };
        recvData.ReadBitInOrder(itemGuid[i], bitsOrder);
    }

    uint8 unkCounter = recvData.ReadBits(2);

    for (uint8 i = 0; i < unkCounter; i++)
    {
        recvData.ReadBit();
        recvData.ReadBit();
    }

    recvData.FlushBits();

    for (uint32 i = 0; i < EQUIPMENT_SLOT_END; ++i)
    {
        if (i == 17)
            continue;

        uint8 bytesOrder[8] = { 4, 1, 6, 5, 3, 2, 0, 7 };
        recvData.ReadBytesSeq(itemGuid[i], bytesOrder);

        if (i < uint32(startSlot))
            continue;

        // check if item slot is set to "ignored" (raw value == 1), must not be unequipped then
        if (itemGuid[i] == 1)
            continue;

        Item* item = m_Player->GetItemByGuid(itemGuid[i]);

        uint16 dstpos = i | (INVENTORY_SLOT_BAG_0 << 8);

        if (!item)
        {
            Item* uItem = m_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
            if (!uItem)
                continue;

            ItemPosCountVec sDest;
            InventoryResult msg = m_Player->CanStoreItem(NULL_BAG, NULL_SLOT, sDest, uItem, false);
            if (msg == EQUIP_ERR_OK)
            {
                m_Player->RemoveItem(INVENTORY_SLOT_BAG_0, i, true);
                m_Player->StoreItem(sDest, uItem, true);
            }
            else
                m_Player->SendEquipError(msg, uItem, NULL);

            continue;
        }

        if (item->GetPos() == dstpos)
            continue;

        m_Player->SwapItem(item->GetPos(), dstpos);
    }

    for (uint8 i = 0; i < unkCounter; i++)
    {
        recvData.read_skip<uint8>();
        recvData.read_skip<uint8>();
    }

    WorldPacket data(SMSG_DUMP_OBJECTS_DATA);
    data << uint8(0);   // 4 - equipment swap failed - inventory is full
    SendPacket(&data);

    delete[] srcbag;
    srcbag = 0;
    delete[] srcslot;
    srcslot = 0;
    delete[] itemGuid;
    itemGuid = 0;
}

void WorldSession::HandleCharFactionOrRaceChange(WorldPacket& recvData)
{
    // TODO: Move queries to prepared statements
    ObjectGuid guid;
    std::string newname;
    bool unk, hasSkin, hasFace, hasHairStyle, hasHairColor, hasFacialHair;
    uint8 gender, race;
    uint8 skin = 0;
    uint8 face = 0;
    uint8 hairStyle = 0;
    uint8 hairColor = 0;
    uint8 facialHair = 0;
    uint32 nameLen = 0;

    recvData >> gender;
    recvData >> race;
    unk = recvData.ReadBit();
    hasFace = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    nameLen = recvData.ReadBits(6);
    hasHairStyle = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    hasHairColor = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();
    hasFacialHair = recvData.ReadBit();
    guid[4] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    hasSkin = recvData.ReadBit();
    guid[6] = recvData.ReadBit();

    recvData.FlushBits();

    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[3]);
    recvData.ReadByteSeq(guid[7]);
    newname = recvData.ReadString(nameLen);
    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[4]);
    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[2]);
    recvData.ReadByteSeq(guid[1]);

    if (hasHairStyle)
        recvData >> hairStyle;
    if (hasHairColor)
        recvData >> hairColor;
    if (hasFace)
        recvData >> face;
    if (hasSkin)
        recvData >> skin;
    if (hasFacialHair)
        recvData >> facialHair;

    uint32 lowGuid = GUID_LOPART(guid);

    // get the players old (at this moment current) race
    CharacterNameData const* nameData = sWorld->GetCharacterNameData(lowGuid);
    if (!nameData)	
    {
        WorldPacket data(SMSG_CHAR_FACTION_OR_RACE_CHANGE, 1);
        data << uint8(CHAR_CREATE_ERROR);
        data << uint64(guid);
        SendPacket(&data);
        return;
    }

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHAR_CLASS_LVL_AT_LOGIN);
    stmt->setUInt32(0, lowGuid);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
    {
        WorldPacket data(SMSG_CHAR_FACTION_OR_RACE_CHANGE, 1);
        data << uint8(CHAR_CREATE_ERROR);
        data << uint64(guid);
        SendPacket(&data);
        return;
    }

    Field* fields = result->Fetch();
    uint8  oldRace          = fields[0].GetUInt8();
    uint32 playerClass      = uint32(fields[1].GetUInt8());
    uint32 level            = uint32(fields[2].GetUInt8());
    uint32 at_loginFlags    = fields[3].GetUInt16();
    char const* knownTitlesStr = fields[4].GetCString();

    // Search each faction is targeted
    BattlegroundTeamId oldTeam = BG_TEAM_ALLIANCE;
    switch (oldRace)
    {
        case RACE_ORC:
        case RACE_GOBLIN:
        case RACE_TAUREN:
        case RACE_UNDEAD_PLAYER:
        case RACE_TROLL:
        case RACE_BLOODELF:
        case RACE_PANDAREN_HORDE:
            oldTeam = BG_TEAM_HORDE;
            break;
        default:
            break;
    }

    // Search each faction is targeted
    BattlegroundTeamId team = BG_TEAM_ALLIANCE;
    switch (race)
    {
        case RACE_ORC:
        case RACE_GOBLIN:
        case RACE_TAUREN:
        case RACE_UNDEAD_PLAYER:
        case RACE_TROLL:
        case RACE_BLOODELF:
        case RACE_PANDAREN_HORDE:
            team = BG_TEAM_HORDE;
            break;
        default:
            break;
    }

    uint32 used_loginFlag = at_loginFlags;

    // We need to correct race when it's pandaren
    // Because client always send neutral ID
    if (race == RACE_PANDAREN_NEUTRAL)
    {
        if (at_loginFlags & AT_LOGIN_CHANGE_RACE)
            team = oldTeam;
        else
            team = oldTeam == BG_TEAM_ALLIANCE ? BG_TEAM_HORDE : BG_TEAM_ALLIANCE;
    }

    if (race == RACE_PANDAREN_NEUTRAL)
    {
        if (oldTeam == team)
            race = oldTeam == BG_TEAM_ALLIANCE ? RACE_PANDAREN_ALLI : RACE_PANDAREN_HORDE;
        else
            race = oldTeam == BG_TEAM_ALLIANCE ? RACE_PANDAREN_HORDE : RACE_PANDAREN_ALLI;
    }

    // Not really necessary because changing race does not include faction change
    // But faction change can include race change
    if (oldTeam != team)
        used_loginFlag = AT_LOGIN_CHANGE_FACTION;

    if (!sObjectMgr->GetPlayerInfo(race, playerClass))
    {
        WorldPacket data(SMSG_CHAR_FACTION_OR_RACE_CHANGE, 1);
        data << uint8(CHAR_CREATE_ERROR);
        data << uint64(guid);
        SendPacket(&data);
        return;
    }

    if (!(at_loginFlags & used_loginFlag))
    {
        WorldPacket data(SMSG_CHAR_FACTION_OR_RACE_CHANGE, 1);
        data << uint8(CHAR_CREATE_ERROR);
        data << uint64(guid);
        SendPacket(&data);
        return;
    }

    if (AccountMgr::IsPlayerAccount(GetSecurity()))
    {
        uint32 raceMaskDisabled = sWorld->getIntConfig(CONFIG_CHARACTER_CREATING_DISABLED_RACEMASK);
        if ((1 << (race - 1)) & raceMaskDisabled)
        {
            WorldPacket data(SMSG_CHAR_FACTION_OR_RACE_CHANGE, 1);
            data << uint8(CHAR_CREATE_ERROR);
            data << uint64(guid);
            SendPacket(&data);
            return;
        }
    }

    // prevent character rename to invalid name
    if (!normalizePlayerName(newname))
    {
        WorldPacket data(SMSG_CHAR_FACTION_OR_RACE_CHANGE, 1);
        data << uint8(CHAR_NAME_NO_NAME);
        data << uint64(guid);
        SendPacket(&data);
        return;
    }

    uint8 res = ObjectMgr::CheckPlayerName(newname, true);
    if (res != CHAR_NAME_SUCCESS)
    {
        WorldPacket data(SMSG_CHAR_FACTION_OR_RACE_CHANGE, 1);
        data << uint8(res);
        data << uint64(guid);
        SendPacket(&data);
        return;
    }

    // check name limitations
    if (AccountMgr::IsPlayerAccount(GetSecurity()) && sObjectMgr->IsReservedName(newname))
    {
        WorldPacket data(SMSG_CHAR_FACTION_OR_RACE_CHANGE, 1);
        data << uint8(CHAR_NAME_RESERVED);
        data << uint64(guid);
        SendPacket(&data);
        return;
    }

    // character with this name already exist
    if (uint64 newguid = sObjectMgr->GetPlayerGUIDByName(newname))
    {
        if (newguid != guid)
        {
            WorldPacket data(SMSG_CHAR_FACTION_OR_RACE_CHANGE, 1);
            data << uint8(CHAR_CREATE_NAME_IN_USE);
            data << uint64(guid);
            SendPacket(&data);
            return;
        }
    }

    CharacterDatabase.EscapeString(newname);
    Player::Customize(guid, gender, skin, face, hairStyle, hairColor, facialHair);
    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_FACTION_OR_RACE);
    stmt->setString(0, newname);
    stmt->setUInt8 (1, race);
    stmt->setUInt16(2, used_loginFlag);
    stmt->setUInt32(3, lowGuid);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_FACTION_OR_RACE_LOG);
    stmt->setUInt32(0, lowGuid);
    stmt->setUInt32(1, GetAccountId());
    stmt->setUInt8 (2, oldRace);
    stmt->setUInt8 (3, race);
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_DECLINED_NAME);
    stmt->setUInt32(0, lowGuid);
    trans->Append(stmt);

    // CHECK PTR
    if (nameData)
    {
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_NAME_LOG);
        stmt->setUInt32(0, lowGuid);
        stmt->setString(1, nameData->m_name);
        stmt->setString(2, newname);
        trans->Append(stmt);
    }


    sWorld->UpdateCharacterNameData(GUID_LOPART(guid), newname, gender, race);

    // Switch Languages
    // delete all languages first
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SKILL_LANGUAGES);
    stmt->setUInt32(0, lowGuid);
    trans->Append(stmt);

    // Now add them back
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_SKILL_LANGUAGE);
    stmt->setUInt32(0, lowGuid);

    // Faction specific languages
    if (team == BG_TEAM_HORDE)
        stmt->setUInt16(1, 109);
    else
        stmt->setUInt16(1, 98);

    trans->Append(stmt);

    // Race specific languages

    if (race != RACE_ORC && race != RACE_HUMAN)
    {
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CHAR_SKILL_LANGUAGE);
        stmt->setUInt32(0, lowGuid);

        switch (race)
        {
            case RACE_DWARF:
                stmt->setUInt16(1, 111);
                break;
            case RACE_DRAENEI:
                stmt->setUInt16(1, 759);
                break;
            case RACE_GNOME:
                stmt->setUInt16(1, 313);
                break;
            case RACE_NIGHTELF:
                stmt->setUInt16(1, 113);
                break;
            case RACE_WORGEN:
                stmt->setUInt16(1, 791);
                break;
            case RACE_UNDEAD_PLAYER:
                stmt->setUInt16(1, 673);
                break;
            case RACE_TAUREN:
                stmt->setUInt16(1, 115);
                break;
            case RACE_TROLL:
                stmt->setUInt16(1, 315);
                break;
            case RACE_BLOODELF:
                stmt->setUInt16(1, 137);
                break;
            case RACE_GOBLIN:
                stmt->setUInt16(1, 792);
                break;
            case RACE_PANDAREN_ALLI:
                stmt->setUInt16(1, 906);
                break;
            case RACE_PANDAREN_HORDE:
                stmt->setUInt16(1, 907);
                break;
            case RACE_PANDAREN_NEUTRAL:
                stmt->setUInt16(1, 905);
                break;
        }

        trans->Append(stmt);
    }

    if (oldTeam != team)
    {
        // Delete all Flypaths
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_TAXI_PATH);
        stmt->setUInt32(0, lowGuid);
        trans->Append(stmt);

        if (level > 7)
        {
            // Update Taxi path
            // this doesn't seem to be 100% blizzlike... but it can't really be helped.
            std::ostringstream taximaskstream;
            uint32 numFullTaximasks = level / 7;
            if (numFullTaximasks > 11)
                numFullTaximasks = 11;
            if (team == BG_TEAM_ALLIANCE)
            {
                if (playerClass != CLASS_DEATH_KNIGHT)
                {
                    for (uint8 i = 0; i < numFullTaximasks; ++i)
                        taximaskstream << uint32(sAllianceTaxiNodesMask[i]) << ' ';
                }
                else
                {
                    for (uint8 i = 0; i < numFullTaximasks; ++i)
                        taximaskstream << uint32(sAllianceTaxiNodesMask[i] | sDeathKnightTaxiNodesMask[i]) << ' ';
                }
            }
            else
            {
                if (playerClass != CLASS_DEATH_KNIGHT)
                {
                    for (uint8 i = 0; i < numFullTaximasks; ++i)
                        taximaskstream << uint32(sHordeTaxiNodesMask[i]) << ' ';
                }
                else
                {
                    for (uint8 i = 0; i < numFullTaximasks; ++i)
                        taximaskstream << uint32(sHordeTaxiNodesMask[i] | sDeathKnightTaxiNodesMask[i]) << ' ';
                }
            }

            uint32 numEmptyTaximasks = 11 - numFullTaximasks;
            for (uint8 i = 0; i < numEmptyTaximasks; ++i)
                taximaskstream << "0 ";
            taximaskstream << '0';
            std::string taximask = taximaskstream.str();

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_TAXIMASK);
            stmt->setString(0, taximask);
            stmt->setUInt32(1, lowGuid);
            trans->Append(stmt);
        }

        // Delete all current quests
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_QUESTSTATUS);
        stmt->setUInt32(0, GUID_LOPART(guid));
        trans->Append(stmt);

        // Delete record of the faction old completed quests
        {
            std::ostringstream quests;
            ObjectMgr::QuestMap const& qTemplates = sObjectMgr->GetQuestTemplates();
            for (ObjectMgr::QuestMap::const_iterator iter = qTemplates.begin(); iter != qTemplates.end(); ++iter)
            {
                Quest *qinfo = iter->second;
                uint32 requiredRaces = qinfo->GetRequiredRaces();
                if (team == BG_TEAM_ALLIANCE)
                {
                    if (requiredRaces & RACEMASK_ALLIANCE)
                    {
                        quests << uint32(qinfo->GetQuestId());
                        quests << ',';
                    }
                }
                else // if (team == BG_TEAM_HORDE)
                {
                    if (requiredRaces & RACEMASK_HORDE)
                    {
                        quests << uint32(qinfo->GetQuestId());
                        quests << ',';
                    }
                }
            }

            std::string questsStr = quests.str();
            questsStr = questsStr.substr(0, questsStr.length() - 1);

            if (!questsStr.empty())
                trans->PAppend("DELETE FROM `character_queststatus_rewarded` WHERE guid='%u' AND quest IN (%s)", lowGuid, questsStr.c_str());
        }

        if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GUILD))
        {
            // Reset guild
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUILD_MEMBER);

            stmt->setUInt32(0, lowGuid);

            PreparedQueryResult result = CharacterDatabase.Query(stmt);
            if (result)
                if (Guild* guild = sGuildMgr->GetGuildById((result->Fetch()[0]).GetUInt32()))
                    guild->DeleteMember(MAKE_NEW_GUID(lowGuid, 0, HIGHGUID_PLAYER), false, false, true);
        }

        if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_ADD_FRIEND))
        {
            // Delete Friend List
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SOCIAL_BY_GUID);
            stmt->setUInt32(0, lowGuid);
            trans->Append(stmt);

            stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SOCIAL_BY_FRIEND);
            stmt->setUInt32(0, lowGuid);
            trans->Append(stmt);
        }

        // Reset homebind and position
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PLAYER_HOMEBIND);
        stmt->setUInt32(0, lowGuid);
        trans->Append(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_PLAYER_HOMEBIND);
        stmt->setUInt32(0, lowGuid);
        if (team == BG_TEAM_ALLIANCE)
        {
            stmt->setUInt16(1, 0);
            stmt->setUInt16(2, 1519);
            stmt->setFloat (3, -8866.19f);
            stmt->setFloat (4, 671.16f);
            stmt->setFloat (5, 97.9034f);
            Player::SavePositionInDB(0, -8866.19f, 671.16f, 97.9034f, 0.0f, 1519, lowGuid);
        }
        else
        {
            stmt->setUInt16(1, 1);
            stmt->setUInt16(2, 1637);
            stmt->setFloat (3, 1633.33f);
            stmt->setFloat (4, -4439.11f);
            stmt->setFloat (5, 17.05f);
            Player::SavePositionInDB(1, 1633.33f, -4439.11f, 17.05f, 0.0f, 1637, lowGuid);
        }

        trans->Append(stmt);

        // Achievement conversion
        for (std::map<uint32, uint32>::const_iterator it = sObjectMgr->FactionChange_Achievements.begin(); it != sObjectMgr->FactionChange_Achievements.end(); ++it)
        {
            uint32 achiev_alliance = it->first;
            uint32 achiev_horde = it->second;

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_ACHIEVEMENT_BY_ACHIEVEMENT);
            stmt->setUInt16(0, uint16(team == BG_TEAM_ALLIANCE ? achiev_alliance : achiev_horde));
            stmt->setUInt32(1, lowGuid);
            trans->Append(stmt);

            stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_ACHIEVEMENT);
            stmt->setUInt16(0, uint16(team == BG_TEAM_ALLIANCE ? achiev_alliance : achiev_horde));
            stmt->setUInt16(1, uint16(team == BG_TEAM_ALLIANCE ? achiev_horde : achiev_alliance));
            stmt->setUInt32(2, lowGuid);
            trans->Append(stmt);
        }

        // Item conversion
        for (std::map<uint32, uint32>::const_iterator it = sObjectMgr->FactionChange_Items.begin(); it != sObjectMgr->FactionChange_Items.end(); ++it)
        {
            uint32 item_alliance = it->first;
            uint32 item_horde = it->second;

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_INVENTORY_FACTION_CHANGE);
            stmt->setUInt32(0, (team == BG_TEAM_ALLIANCE ? item_alliance : item_horde));
            stmt->setUInt32(1, (team == BG_TEAM_ALLIANCE ? item_horde : item_alliance));
            stmt->setUInt32(2, guid);
            trans->Append(stmt);
        }

        // Spell conversion
        for (std::map<uint32, uint32>::const_iterator it = sObjectMgr->FactionChange_Spells.begin(); it != sObjectMgr->FactionChange_Spells.end(); ++it)
        {
            uint32 spell_alliance = it->first;
            uint32 spell_horde = it->second;

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_SPELL_BY_SPELL);
            stmt->setUInt32(0, (team == BG_TEAM_ALLIANCE ? spell_alliance : spell_horde));
            stmt->setUInt32(1, lowGuid);
            trans->Append(stmt);

            stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_SPELL_FACTION_CHANGE);
            stmt->setUInt32(0, (team == BG_TEAM_ALLIANCE ? spell_alliance : spell_horde));
            stmt->setUInt32(1, (team == BG_TEAM_ALLIANCE ? spell_horde : spell_alliance));
            stmt->setUInt32(2, lowGuid);
            trans->Append(stmt);
        }

        // Reputation conversion
        for (std::map<uint32, uint32>::const_iterator it = sObjectMgr->FactionChange_Reputation.begin(); it != sObjectMgr->FactionChange_Reputation.end(); ++it)
        {
            uint32 reputation_alliance = it->first;
            uint32 reputation_horde = it->second;

            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_REP_BY_FACTION);
            stmt->setUInt32(0, uint16(team == BG_TEAM_ALLIANCE ? reputation_alliance : reputation_horde));
            stmt->setUInt32(1, lowGuid);
            trans->Append(stmt);

            stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_REP_FACTION_CHANGE);
            stmt->setUInt16(0, uint16(team == BG_TEAM_ALLIANCE ? reputation_alliance : reputation_horde));
            stmt->setUInt16(1, uint16(team == BG_TEAM_ALLIANCE ? reputation_horde : reputation_alliance));
            stmt->setUInt32(2, lowGuid);
            trans->Append(stmt);
        }

        // Title conversion
        if (knownTitlesStr)
        {
            const uint32 ktcount = KNOWN_TITLES_SIZE * 2;
            uint32 knownTitles[ktcount];
            Tokenizer tokens(knownTitlesStr, ' ', ktcount);

            if (tokens.size() != ktcount)
                return;

            for (uint32 index = 0; index < ktcount; ++index)
                knownTitles[index] = atol(tokens[index]);

            for (std::map<uint32, uint32>::const_iterator it = sObjectMgr->FactionChange_Titles.begin(); it != sObjectMgr->FactionChange_Titles.end(); ++it)
            {
                uint32 title_alliance = it->first;
                uint32 title_horde = it->second;

                CharTitlesEntry const* atitleInfo = sCharTitlesStore.LookupEntry(title_alliance);
                CharTitlesEntry const* htitleInfo = sCharTitlesStore.LookupEntry(title_horde);
                // new team
                if (team == BG_TEAM_ALLIANCE)
                {
                    uint32 bitIndex = htitleInfo->bit_index;
                    uint32 index = bitIndex / 32;
                    uint32 old_flag = 1 << (bitIndex % 32);
                    uint32 new_flag = 1 << (atitleInfo->bit_index % 32);
                    if (knownTitles[index] & old_flag)
                    {
                        knownTitles[index] &= ~old_flag;
                        // use index of the new title
                        knownTitles[atitleInfo->bit_index / 32] |= new_flag;
                    }
                }
                else
                {
                    uint32 bitIndex = atitleInfo->bit_index;
                    uint32 index = bitIndex / 32;
                    uint32 old_flag = 1 << (bitIndex % 32);
                    uint32 new_flag = 1 << (htitleInfo->bit_index % 32);
                    if (knownTitles[index] & old_flag)
                    {
                        knownTitles[index] &= ~old_flag;
                        // use index of the new title
                        knownTitles[htitleInfo->bit_index / 32] |= new_flag;
                    }
                }

                std::ostringstream ss;
                for (uint32 index = 0; index < ktcount; ++index)
                    ss << knownTitles[index] << ' ';

                PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_TITLES_FACTION_CHANGE);
                stmt->setString(0, ss.str().c_str());
                stmt->setUInt32(1, lowGuid);
                trans->Append(stmt);

                // unset any currently chosen title
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_RES_CHAR_TITLES_FACTION_CHANGE);
                stmt->setUInt32(0, lowGuid);
                trans->Append(stmt);
            }
        }
    }

    CharacterDatabase.CommitTransaction(trans);

    std::string IP_str = GetRemoteAddress();
    sLog->outDebug(LOG_FILTER_UNITS, "Account: %d (IP: %s), Character guid: %u Change Race/Faction to: %s", GetAccountId(), IP_str.c_str(), lowGuid, newname.c_str());

    WorldPacket data(SMSG_CHAR_FACTION_OR_RACE_CHANGE, 1 + 8 + (newname.size() + 1) + 1 + 1 + 1 + 1 + 1 + 1 + 1);
    data << uint8(RESPONSE_SUCCESS);
    data << uint64(guid);
    data << newname;
    data << uint8(gender);
    data << uint8(skin);
    data << uint8(hairColor);
    data << uint8(hairStyle);
    data << uint8(facialHair);
    data << uint8(face);
    data << uint8(race);
    SendPacket(&data);
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
    WorldPacket data(SMSG_RANDOMIZE_CHAR_NAME, 10);
    data.WriteBits(name->size(), 6);
    data.WriteBit(0); // unk
    data.WriteString(name->c_str());
    SendPacket(&data);
}

void WorldSession::HandleReorderCharacters(WorldPacket& recvData)
{
    uint32 charactersCount = recvData.ReadBits(9);

    std::vector<ObjectGuid> guids(charactersCount);
    uint8 position;

    for (uint8 i = 0; i < charactersCount; ++i)
    {
        uint8 bitOrder[8] = { 7, 1, 5, 6, 4, 3, 0, 2 };
        recvData.ReadBitInOrder(guids[i], bitOrder);
    }

    recvData.FlushBits();

    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    for (uint8 i = 0; i < charactersCount; ++i)
    {
        recvData.ReadByteSeq(guids[i][0]);

        recvData >> position;

        recvData.ReadByteSeq(guids[i][6]);
        recvData.ReadByteSeq(guids[i][2]);
        recvData.ReadByteSeq(guids[i][3]);
        recvData.ReadByteSeq(guids[i][1]);
        recvData.ReadByteSeq(guids[i][7]);
        recvData.ReadByteSeq(guids[i][4]);
        recvData.ReadByteSeq(guids[i][5]);

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_LIST_SLOT);
        stmt->setUInt8(0, position);
        stmt->setUInt32(1, GUID_LOPART(guids[i]));
        trans->Append(stmt);
    }

    CharacterDatabase.CommitTransaction(trans);
}

void WorldSession::HandleSuspendToken(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_SUSPEND_TOKEN");
    uint32 token = recvData.read<uint32>();

    GetPlayer()->SendResumeToken(token);
}
