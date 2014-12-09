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
#include "Language.h"
#include "DatabaseEnv.h"
#include "WorldPacket.h"
#include "Opcodes.h"
#include "Log.h"
#include "CUFProfiles.h"
#include "Player.h"
#include "GossipDef.h"
#include "World.h"
#include "ObjectMgr.h"
#include "GuildMgr.h"
#include "WorldSession.h"
#include "BigNumber.h"
#include "SHA1.h"
#include "UpdateData.h"
#include "LootMgr.h"
#include "Chat.h"
#include "zlib.h"
#include "ObjectAccessor.h"
#include "Object.h"
#include "Battleground.h"
#include "OutdoorPvP.h"
#include "Pet.h"
#include "SocialMgr.h"
#include "CellImpl.h"
#include "AccountMgr.h"
#include "Vehicle.h"
#include "CreatureAI.h"
#include "DBCEnums.h"
#include "ScriptMgr.h"
#include "MapManager.h"
#include "InstanceScript.h"
#include "GameObjectAI.h"
#include "Group.h"
#include "AccountMgr.h"
#include "Spell.h"
#include "BattlegroundMgr.h"
#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "TicketMgr.h"

void WorldSession::HandleRepopRequestOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_REPOP_REQUEST Message");

    bool l_CheckInstance = recvData.ReadBit();

    if (GetPlayer()->isAlive() || GetPlayer()->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
        return;

    if (GetPlayer()->HasAuraType(SPELL_AURA_PREVENT_RESURRECTION))
        return; // silently return, client should display the error by itself

    // the world update order is sessions, players, creatures
    // the netcode runs in parallel with all of these
    // creatures can kill players
    // so if the server is lagging enough the player can
    // release spirit after he's killed but before he is updated
    if (GetPlayer()->getDeathState() == JUST_DIED)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleRepopRequestOpcode: got request after player %s(%d) was killed and before he was updated", GetPlayer()->GetName(), GetPlayer()->GetGUIDLow());
        GetPlayer()->KillPlayer();
    }

    //this is spirit release confirm?
    GetPlayer()->RemovePet(NULL, PET_SLOT_OTHER_PET, true, true);
    GetPlayer()->BuildPlayerRepop();
    GetPlayer()->RepopAtGraveyard();
}

void WorldSession::HandleGossipSelectOptionOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_GOSSIP_SELECT_OPTION");

    uint32 gossipListId;
    uint32 textId;
    uint64 guid;
    uint32 codeLen = 0;
    std::string code = "";

    recvData.readPackGUID(guid);
    recvData >> textId >> gossipListId;
    codeLen = recvData.ReadBits(8);
    code = recvData.ReadString(codeLen);

    Creature* unit = NULL;
    GameObject* go = NULL;
    if (IS_CRE_OR_VEH_GUID(guid))
    {
        unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_NONE);
        if (!unit)
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGossipSelectOptionOpcode - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(guid)));
            return;
        }
    }
    else if (IS_GAMEOBJECT_GUID(guid))
    {
        go = m_Player->GetMap()->GetGameObject(guid);
        if (!go)
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGossipSelectOptionOpcode - GameObject (GUID: %u) not found.", uint32(GUID_LOPART(guid)));
            return;
        }
    }
    else
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGossipSelectOptionOpcode - unsupported GUID type for highguid %u. lowpart %u.", uint32(GUID_HIPART(guid)), uint32(GUID_LOPART(guid)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    if ((unit && unit->GetCreatureTemplate()->ScriptID != unit->LastUsedScriptID) || (go && go->GetGOInfo()->ScriptId != go->LastUsedScriptID))
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGossipSelectOptionOpcode - Script reloaded while in use, ignoring and set new scipt id");
        if (unit)
            unit->LastUsedScriptID = unit->GetCreatureTemplate()->ScriptID;
        if (go)
            go->LastUsedScriptID = go->GetGOInfo()->ScriptId;
        m_Player->PlayerTalkClass->SendCloseGossip();
        return;
    }

    uint32 menuId = m_Player->PlayerTalkClass->GetGossipMenu().GetMenuId();

    if (!code.empty())
    {
        if (unit)
        {
            unit->AI()->sGossipSelectCode(m_Player, menuId, gossipListId, code.c_str());
            if (!sScriptMgr->OnGossipSelectCode(m_Player, unit, m_Player->PlayerTalkClass->GetGossipOptionSender(gossipListId), m_Player->PlayerTalkClass->GetGossipOptionAction(gossipListId), code.c_str()))
                m_Player->OnGossipSelect(unit, gossipListId, menuId);
        }
        else
        {
            go->AI()->GossipSelectCode(m_Player, menuId, gossipListId, code.c_str());
            sScriptMgr->OnGossipSelectCode(m_Player, go, m_Player->PlayerTalkClass->GetGossipOptionSender(gossipListId), m_Player->PlayerTalkClass->GetGossipOptionAction(gossipListId), code.c_str());
        }
    }
    else
    {
        if (unit)
        {
            unit->AI()->sGossipSelect(m_Player, menuId, gossipListId);
            if (!sScriptMgr->OnGossipSelect(m_Player, unit, m_Player->PlayerTalkClass->GetGossipOptionSender(gossipListId), m_Player->PlayerTalkClass->GetGossipOptionAction(gossipListId)))
                m_Player->OnGossipSelect(unit, gossipListId, menuId);
        }
        else
        {
            go->AI()->GossipSelect(m_Player, menuId, gossipListId);
            if (!sScriptMgr->OnGossipSelect(m_Player, go, m_Player->PlayerTalkClass->GetGossipOptionSender(gossipListId), m_Player->PlayerTalkClass->GetGossipOptionAction(gossipListId)))
                m_Player->OnGossipSelect(go, gossipListId, menuId);
        }
    }
}

void WorldSession::HandleWhoOpcode(WorldPacket& p_RecvData)
{
    time_t now = time(NULL);
    if (now - timeLastWhoCommand < 5)
        return;
    else
        timeLastWhoCommand = now;

    uint32 l_MatchCount = 0;

    uint32 l_LevelMin, l_LevelMax, l_ZonesCount, l_StringCount;
    int32 l_RaceMask, l_ClassMask;
    uint32 l_ZoneIDs[10];                                   // 10 is client limit

    bool l_Unk1, l_Unk2, l_Bit725, l_Bit740;
    uint8 l_PlayerLen = 0, l_GuildLen = 0;
    uint8 l_UnkLen2, l_UnkLen3;
    std::string l_PlayerName, l_GuildName;

    l_ZonesCount = p_RecvData.ReadBits(4);                  // zones count, client limit = 10 (2.0.10)
    if (l_ZonesCount > 10)                                  // can't be received from real client or broken packet
        return;

    p_RecvData.FlushBits();

    p_RecvData >> l_LevelMin;                               // maximal player level, default 0
    p_RecvData >> l_LevelMax;                               // minimal player level, default 123 (MAX_LEVEL)
    p_RecvData >> l_RaceMask;                               // race mask, default -1
    p_RecvData >> l_ClassMask;                              // class mask, default -1

    l_PlayerLen = p_RecvData.ReadBits(6);
    l_UnkLen2 = p_RecvData.ReadBits(8) << 1;
    l_UnkLen2 += p_RecvData.ReadBit();
    l_GuildLen = p_RecvData.ReadBits(7);
    l_UnkLen3 = p_RecvData.ReadBits(8) << 1;
    l_UnkLen3 += p_RecvData.ReadBit();

    l_StringCount = p_RecvData.ReadBits(3);
    if (l_StringCount > 4)                                  // can't be received from real client or broken packet
        return;

    l_Unk1 = p_RecvData.ReadBit();
    l_Unk2 = p_RecvData.ReadBit();
    l_Bit725 = p_RecvData.ReadBit();
    l_Bit740 = p_RecvData.ReadBit();

    p_RecvData.FlushBits();

    if (l_PlayerLen > 0)
        l_PlayerName = p_RecvData.ReadString(l_PlayerLen);  // player name, case sensitive...

    if (l_UnkLen2 > 0)
        std::string l_UnkStr2 = p_RecvData.ReadString(l_UnkLen2);

    if (l_GuildLen > 0)
        l_GuildName = p_RecvData.ReadString(l_GuildLen);    // guild name, case sensitive ...

    if (l_UnkLen3 > 0)
        std::string l_UnkStr3 = p_RecvData.ReadString(l_UnkLen3);

    std::vector<uint8> l_UnkLens(l_StringCount, 0);
    std::wstring l_UnkStrings[4];                           // 4 is client limit
    for (uint8 l_Iter = 0; l_Iter < l_StringCount; ++l_Iter)
    {
        l_UnkLens[l_Iter] = p_RecvData.ReadBits(7);
        p_RecvData.FlushBits();

        if (l_UnkLens[l_Iter] > 0)
        {
            std::string l_Temp = p_RecvData.ReadString(l_UnkLens[l_Iter]); // user entered string, it used as universal search pattern(guild+player name)?
            if (!Utf8toWStr(l_Temp, l_UnkStrings[l_Iter]))
                continue;

            wstrToLower(l_UnkStrings[l_Iter]);
            sLog->outDebug(LOG_FILTER_NETWORKIO, "String %u: %s", l_Iter, l_Temp.c_str());
        }
    }

    if (l_Bit740)
    {
        uint32 l_Unks[3];
        p_RecvData >> l_Unks[0] >> l_Unks[1] >> l_Unks[2];
    }

    for (uint8 l_Iter = 0; l_Iter < l_ZonesCount; ++l_Iter)
        p_RecvData >> l_ZoneIDs[l_Iter];

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Minlvl %u, maxlvl %u, name %s, guild %s, racemask %u, classmask %u, zones %u, strings %u", l_LevelMin, l_LevelMax, l_PlayerName.c_str(), l_GuildName.c_str(), l_RaceMask, l_ClassMask, l_ZonesCount, l_StringCount);

    std::wstring l_WQueryerPlayerName;
    std::wstring l_WQueryerPlayerGuildName;

    if (!(Utf8toWStr(l_PlayerName, l_WQueryerPlayerName) && Utf8toWStr(l_GuildName, l_WQueryerPlayerGuildName)))
        return;

    wstrToLower(l_WQueryerPlayerName);
    wstrToLower(l_WQueryerPlayerGuildName);

    /// Client send in case not set max level value 100 but Trinity supports 255 max level,
    /// update it to show GMs with characters after 100 level
    if (l_LevelMax >= MAX_LEVEL)
        l_LevelMax = STRONG_MAX_LEVEL;

    uint32 l_QueryerPlayerTeam = m_Player->GetTeam();
    uint32 l_Security = GetSecurity();
    bool l_AllowTwoSideWhoList = sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_WHO_LIST);
    uint32 l_GMLevelInWhoList  = sWorld->getIntConfig(CONFIG_GM_LEVEL_IN_WHO_LIST);
    uint8 l_MemberCount = 0;

    WorldPacket l_Data(SMSG_WHO);
    ByteBuffer l_Buffer;

    TRINITY_READ_GUARD(HashMapHolder<Player>::LockType, *HashMapHolder<Player>::GetLock());
    HashMapHolder<Player>::MapType const& l_PlayersMap = sObjectAccessor->GetPlayers();

    for (HashMapHolder<Player>::MapType::const_iterator l_It = l_PlayersMap.begin(); l_It != l_PlayersMap.end(); ++l_It)
    {
        if (AccountMgr::IsPlayerAccount(l_Security))
        {
            /// Player can see member of other team only if CONFIG_ALLOW_TWO_SIDE_WHO_LIST
            if (l_It->second->GetTeam() != l_QueryerPlayerTeam && !l_AllowTwoSideWhoList)
                continue;

            /// Player can see MODERATOR, GAME MASTER, ADMINISTRATOR only if CONFIG_GM_IN_WHO_LIST
            if ((l_It->second->GetSession()->GetSecurity() > AccountTypes(l_GMLevelInWhoList)))
                continue;
        }

        /// Do not process players which are not in world
        if (!(l_It->second->IsInWorld()))
            continue;

        /// check if target is globally visible for player
        if (!(l_It->second->IsVisibleGloballyFor(m_Player)))
            continue;

        uint32  l_PlayerClass   = l_It->second->getClass();
        uint32  l_PlayerRace    = l_It->second->getRace();
        uint32  l_AreaID        = l_It->second->GetZoneId();
        uint8   l_PlayerLevel   = l_It->second->getLevel();
        uint8   l_PlayerSex     = l_It->second->getGender();

        /// Check if target's level is in level range
        if (l_PlayerLevel < l_LevelMin || l_PlayerLevel > l_LevelMax)
            continue;

        /// Check if class matches classmask
        if (!(l_ClassMask & (1 << l_PlayerClass)))
            continue;

        // check if race matches racemask
        if (!(l_RaceMask & (1 << l_PlayerRace)))
            continue;

        bool l_ZoneShow = true;
        for (uint32 i = 0; i < l_ZonesCount; ++i)
        {
            if (l_ZoneIDs[i] == l_AreaID)
            {
                l_ZoneShow = true;
                break;
            }

            l_ZoneShow = false;
        }

        if (!l_ZoneShow)
            continue;

        std::string  l_PlayerName = l_It->second->GetName();
        std::wstring l_WPlayerName;

        if (!Utf8toWStr(l_PlayerName, l_WPlayerName))
            continue;

        wstrToLower(l_WPlayerName);

        if (!(l_WQueryerPlayerName.empty() || l_WPlayerName.find(l_WQueryerPlayerName) != std::wstring::npos))
            continue;

        std::string  l_GuildName = sGuildMgr->GetGuildNameById(l_It->second->GetGuildId());
        std::wstring l_WGuildName;

        if (!Utf8toWStr(l_GuildName, l_WGuildName))
            continue;

        wstrToLower(l_WGuildName);

        if (!(l_WQueryerPlayerGuildName.empty() || l_WGuildName.find(l_WQueryerPlayerGuildName) != std::wstring::npos))
            continue;

        std::string aname;
        if (AreaTableEntry const* areaEntry = GetAreaEntryByAreaID(l_It->second->GetZoneId()))
            aname = areaEntry->AreaNameLang[GetSessionDbcLocale()];

        bool s_show = true;
        for (uint32 i = 0; i < l_StringCount; ++i)
        {
            if (!l_UnkStrings[i].empty())
            {
                if (l_WGuildName.find(l_UnkStrings[i]) != std::wstring::npos ||
                    l_WPlayerName.find(l_UnkStrings[i]) != std::wstring::npos ||
                    Utf8FitTo(aname, l_UnkStrings[i]))
                {
                    s_show = true;
                    break;
                }
                s_show = false;
            }
        }
        if (!s_show)
            continue;

        /// 49 is maximum player count sent to client - can be overridden
        /// through config, but is unstable
        if ((l_MatchCount++) >= sWorld->getIntConfig(CONFIG_MAX_WHO))
        {
            if (sWorld->getBoolConfig(CONFIG_LIMIT_WHO_ONLINE))
                break;
            else
                continue;

            break;
        }

        uint64 l_GuildGUID = l_It->second->GetGuild() ? l_It->second->GetGuild()->GetGUID() : 0;
        
        l_Buffer.WriteBit(false);                                                                       ///< Is Deleted
        l_Buffer.WriteBits(l_PlayerName.size(), 6);                                                     ///< Name length

        if (DeclinedName const* l_DeclinedNames = l_It->second->GetDeclinedNames())
        {
            for (uint8 l_I = 0; l_I < MAX_DECLINED_NAME_CASES; ++l_I)
                l_Buffer.WriteBits(l_DeclinedNames->name[l_I].size(), 7);                               ///< DeclinedName[l_I] length
        }
        else
        {
            for (uint8 l_I = 0; l_I < MAX_DECLINED_NAME_CASES; ++l_I)                                   ///< DeclinedName[l_I] length
                l_Buffer.WriteBits(0, 7);
        }
        l_Buffer.FlushBits();

        if (DeclinedName const* l_DeclinedNames = l_It->second->GetDeclinedNames())
        {
            for (uint8 l_I = 0; l_I < MAX_DECLINED_NAME_CASES; ++l_I)
                l_Buffer.WriteString(l_DeclinedNames->name[l_I]);                                       ///< DeclinedName[l_I] 
        }

        l_Buffer.appendPackGUID(l_It->second ? l_It->second->GetSession()->GetWoWAccountGUID() : 0);    ///< WoW account GUID
        l_Buffer.appendPackGUID(l_It->second ? l_It->second->GetSession()->GetBNetAccountGUID() : 0);   ///< BNet account GUID
        l_Buffer.appendPackGUID(l_It->second->GetGUID());                                               ///< Player GUID
        l_Buffer << uint32(g_RealmID);                                                                  ///< Virtual Realm Address
        l_Buffer << uint8(l_PlayerRace);                                                                ///< Race
        l_Buffer << uint8(l_PlayerSex);                                                                 ///< Sex
        l_Buffer << uint8(l_PlayerClass);                                                               ///< Class ID
        l_Buffer << uint8(l_PlayerLevel);                                                               ///< Level

        l_Buffer.WriteString(l_PlayerName);                                                             ///< Name

        l_Buffer.appendPackGUID(l_GuildGUID);                                                           ///< Guild GUID
        l_Buffer << uint32(g_RealmID);                                                                  ///< Guild Virtual Realm Address
        l_Buffer << uint32(l_AreaID);                                                                   ///< Area ID

        l_Buffer.WriteBits(l_GuildName.size(), 7);                                                      ///< Guild Name length
        l_Buffer.WriteBit(l_It->second->isGameMaster());                                                ///< Is Game Master
        l_Buffer.FlushBits();

        l_Buffer.WriteString(l_GuildName);                                                              ///< Guild Name

        ++l_MemberCount;
    }

    l_Data.WriteBits(l_MemberCount, 6);
    l_Data.FlushBits();
    l_Data.append(l_Buffer);

    SendPacket(&l_Data);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Send SMSG_WHO Message");
}

void WorldSession::HandleLogoutRequestOpcode(WorldPacket& /*recvData*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_LOGOUT_REQUEST Message, security - %u", GetSecurity());

    if (uint64 lguid = GetPlayer()->GetLootGUID())
        DoLootRelease(lguid);

    uint32 reason = 0;

    if (GetPlayer()->isInCombat())
        reason = 1;
    else if (GetPlayer()->m_movementInfo.HasMovementFlag(MOVEMENTFLAG_FALLING | MOVEMENTFLAG_FALLING_FAR))
        reason = 3;                                         // is jumping or falling
    else if (GetPlayer()->m_Duel || GetPlayer()->HasAura(9454)) // is dueling or frozen by GM via freeze command
        reason = 2;                                         // FIXME - Need the correct value

    if (reason)
    {
        WorldPacket data(SMSG_LOGOUT_RESPONSE, 1+4);
        data << uint32(reason);
        data.WriteBit(0);
        data.FlushBits();
        SendPacket(&data);
        LogoutRequest(0);
        return;
    }

    // Instant logout in taverns/cities or on taxi or for admins, gm's, mod's if its enabled in worldserver.conf
    if (GetPlayer()->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_RESTING) || GetPlayer()->isInFlight() ||
        GetSecurity() >= AccountTypes(sWorld->getIntConfig(CONFIG_INSTANT_LOGOUT)))
    {
        WorldPacket data(SMSG_LOGOUT_RESPONSE, 1+4);
        data << uint32(reason);
        data.WriteBit(1);           // instant logout
        data.FlushBits();
        SendPacket(&data);
        LogoutPlayer(true);
        return;
    }

    // not set flags if player can't free move to prevent lost state at logout cancel
    if (GetPlayer()->CanFreeMove())
    {
        GetPlayer()->SetStandState(UNIT_STAND_STATE_SIT);
        GetPlayer()->SetRooted(true);
        GetPlayer()->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
    }

    WorldPacket data(SMSG_LOGOUT_RESPONSE, 1+4);
    data << uint32(0);
    data.WriteBit(0);
    data.FlushBits();
    SendPacket(&data);
    LogoutRequest(time(NULL));
}

void WorldSession::HandlePlayerLogoutOpcode(WorldPacket& recvData)
{
    bool unkBit = !recvData.ReadBit();

    recvData.FlushBits();

    uint32 unk = 0;
    if (unkBit)
        unk = recvData.read<uint32>();

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_PLAYER_LOGOUT Message");
}

void WorldSession::HandleLogoutCancelOpcode(WorldPacket& /*recvData*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_LOGOUT_CANCEL Message");

    // Player have already logged out serverside, too late to cancel
    if (!GetPlayer())
        return;

    LogoutRequest(0);

    WorldPacket data(SMSG_LOGOUT_CANCEL_ACK, 0);
    SendPacket(&data);

    // not remove flags if can't free move - its not set in Logout request code.
    if (GetPlayer()->CanFreeMove())
    {
        //!we can move again
        GetPlayer()->SetRooted(false);

        //! Stand Up
        GetPlayer()->SetStandState(UNIT_STAND_STATE_STAND);

        //! DISABLE_ROTATE
        GetPlayer()->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
    }

    GetPlayer()->PetSpellInitialize();

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_LOGOUT_CANCEL_ACK Message");
}

void WorldSession::HandleTogglePvP(WorldPacket& recvData)
{
    // this opcode can be used in two ways: Either set explicit new status or toggle old status
    if (recvData.size() == 1)
    {
        bool newPvPStatus = recvData.ReadBit();

        GetPlayer()->ApplyModFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP, newPvPStatus);
        GetPlayer()->ApplyModFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_PVP_TIMER, !newPvPStatus);
    }
    else
    {
        GetPlayer()->ToggleFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP);
        GetPlayer()->ToggleFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_PVP_TIMER);
    }

    if (GetPlayer()->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP))
    {
        if (!GetPlayer()->IsPvP() || GetPlayer()->pvpInfo.endTimer != 0)
            GetPlayer()->UpdatePvP(true, true);
    }
    else
    {
        if (!GetPlayer()->pvpInfo.inHostileArea && GetPlayer()->IsPvP())
            GetPlayer()->pvpInfo.endTimer = time(NULL);     // start toggle-off
    }

    //if (OutdoorPvP* pvp = _player->GetOutdoorPvP())
    //    pvp->HandlePlayerActivityChanged(_player);
}

void WorldSession::HandleZoneUpdateOpcode(WorldPacket& recvData)
{
    uint32 newZone;
    recvData >> newZone;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd ZONE_UPDATE: %u", newZone);

    // use server size data
    uint32 newzone, newarea;
    GetPlayer()->GetZoneAndAreaId(newzone, newarea);
    GetPlayer()->UpdateZone(newzone, newarea);
    //GetPlayer()->SendInitWorldStates(true, newZone);
}

void WorldSession::HandleReturnToGraveyard(WorldPacket& /*recvPacket*/)
{
    if (GetPlayer()->isAlive() || !GetPlayer()->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
        return;
    //TODO: unk32, unk32
    GetPlayer()->RepopAtGraveyard();
}

void WorldSession::HandleSetSelectionOpcode(WorldPacket& p_RecvData)
{
    uint64 l_NewTargetGuid;

    p_RecvData.readPackGUID(l_NewTargetGuid);

    m_Player->SetSelection(l_NewTargetGuid);
}

void WorldSession::HandleStandStateChangeOpcode(WorldPacket& recvData)
{
    uint32 l_StandState;
    recvData >> l_StandState;

    m_Player->SetStandState(l_StandState);
}

void WorldSession::HandleContactListOpcode(WorldPacket& p_RecvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_SEND_CONTACT_LIST");

    uint32 l_Flags;

    p_RecvData >> l_Flags;

    if (l_Flags & 1)
        m_Player->GetSocial()->SendSocialList(m_Player);
}

void WorldSession::HandleAddFriendOpcode(WorldPacket& p_RecvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_ADD_FRIEND");

    std::string l_FriendName = GetTrinityString(LANG_FRIEND_IGNORE_UNKNOWN);
    std::string l_FriendNote;

    uint32 l_NameLen = p_RecvData.ReadBits(9);
    uint32 l_NoteLen = p_RecvData.ReadBits(10);

    l_FriendName = p_RecvData.ReadString(l_NameLen);
    l_FriendNote = p_RecvData.ReadString(l_NoteLen);

    if (!normalizePlayerName(l_FriendName))
        return;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: %s asked to add friend : '%s'", GetPlayer()->GetName(), l_FriendName.c_str());

    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_RACE_ACC_BY_NAME);

    l_Stmt->setString(0, l_FriendName);

    _addFriendCallback.SetParam(l_FriendNote);
    _addFriendCallback.SetFutureResult(CharacterDatabase.AsyncQuery(l_Stmt));
}

void WorldSession::HandleAddFriendOpcodeCallBack(PreparedQueryResult result, std::string friendNote)
{
    if (!GetPlayer())
        return;

    uint64 friendGuid;
    uint32 friendAccountId;
    uint32 team;
    FriendsResult friendResult;

    friendResult = FRIEND_NOT_FOUND;
    friendGuid = 0;

    if (result)
    {
        Field* fields = result->Fetch();

        friendGuid = MAKE_NEW_GUID(fields[0].GetUInt32(), 0, HIGHGUID_PLAYER);
        team = Player::TeamForRace(fields[1].GetUInt8());
        friendAccountId = fields[2].GetUInt32();

        if (!AccountMgr::IsPlayerAccount(GetSecurity()) || sWorld->getBoolConfig(CONFIG_ALLOW_GM_FRIEND) || AccountMgr::IsPlayerAccount(AccountMgr::GetSecurity(friendAccountId, g_RealmID)))
        {
            if (friendGuid)
            {
                if (friendGuid == GetPlayer()->GetGUID())
                    friendResult = FRIEND_SELF;
                else if (GetPlayer()->GetTeam() != team && !sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_ADD_FRIEND) && AccountMgr::IsPlayerAccount(GetSecurity()))
                    friendResult = FRIEND_ENEMY;
                else if (GetPlayer()->GetSocial()->HasFriend(GUID_LOPART(friendGuid)))
                    friendResult = FRIEND_ALREADY;
                else
                {
                    Player* pFriend = ObjectAccessor::FindPlayer(friendGuid);
                    if (pFriend && pFriend->IsInWorld() && pFriend->IsVisibleGloballyFor(GetPlayer()))
                        friendResult = FRIEND_ADDED_ONLINE;
                    else
                        friendResult = FRIEND_ADDED_OFFLINE;
                    if (!GetPlayer()->GetSocial()->AddToSocialList(GUID_LOPART(friendGuid), false))
                    {
                        friendResult = FRIEND_LIST_FULL;
                        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: %s's friend list is full.", GetPlayer()->GetName());
                    }
                }
                GetPlayer()->GetSocial()->SetFriendNote(GUID_LOPART(friendGuid), friendNote);
            }
        }
    }

    sSocialMgr->SendFriendStatus(GetPlayer(), friendResult, GUID_LOPART(friendGuid), false);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent (SMSG_FRIEND_STATUS)");
}

void WorldSession::HandleDelFriendOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_DEL_FRIEND");

    uint64 l_Guid;

    uint32 l_VirtualRealmAddress;

    recvData >> l_VirtualRealmAddress;
    recvData.readPackGUID(l_Guid);

    m_Player->GetSocial()->RemoveFromSocialList(GUID_LOPART(l_Guid), false);

    sSocialMgr->SendFriendStatus(GetPlayer(), FRIEND_REMOVED, GUID_LOPART(l_Guid), false);
}

void WorldSession::HandleAddIgnoreOpcode(WorldPacket& p_RecvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_ADD_IGNORE");

    std::string l_IgnoreName = GetTrinityString(LANG_FRIEND_IGNORE_UNKNOWN);

    uint32 l_IgnoreNameLen = p_RecvData.ReadBits(9);

    l_IgnoreName = p_RecvData.ReadString(l_IgnoreNameLen);

    if (!normalizePlayerName(l_IgnoreName))
        return;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: %s asked to Ignore: '%s'", GetPlayer()->GetName(), l_IgnoreName.c_str());

    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_BY_NAME);

    l_Stmt->setString(0, l_IgnoreName);

    m_AddIgnoreCallback = CharacterDatabase.AsyncQuery(l_Stmt);
}

void WorldSession::HandleAddIgnoreOpcodeCallBack(PreparedQueryResult result)
{
    if (!GetPlayer())
        return;

    uint64 IgnoreGuid;
    FriendsResult ignoreResult;

    ignoreResult = FRIEND_IGNORE_NOT_FOUND;
    IgnoreGuid = 0;

    if (result)
    {
        IgnoreGuid = MAKE_NEW_GUID((*result)[0].GetUInt32(), 0, HIGHGUID_PLAYER);

        if (IgnoreGuid)
        {
            if (IgnoreGuid == GetPlayer()->GetGUID())              //not add yourself
                ignoreResult = FRIEND_IGNORE_SELF;
            else if (GetPlayer()->GetSocial()->HasIgnore(GUID_LOPART(IgnoreGuid)))
                ignoreResult = FRIEND_IGNORE_ALREADY;
            else
            {
                ignoreResult = FRIEND_IGNORE_ADDED;

                // ignore list full
                if (!GetPlayer()->GetSocial()->AddToSocialList(GUID_LOPART(IgnoreGuid), true))
                    ignoreResult = FRIEND_IGNORE_FULL;
            }
        }
    }

    sSocialMgr->SendFriendStatus(GetPlayer(), ignoreResult, GUID_LOPART(IgnoreGuid), false);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent (SMSG_FRIEND_STATUS)");
}

void WorldSession::HandleDelIgnoreOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_DEL_IGNORE");

    uint64 l_Guid;

    uint32 l_VirtualRealmAddress;

    recvData >> l_VirtualRealmAddress;
    recvData.readPackGUID(l_Guid);

    m_Player->GetSocial()->RemoveFromSocialList(GUID_LOPART(l_Guid), true);

    sSocialMgr->SendFriendStatus(GetPlayer(), FRIEND_IGNORE_REMOVED, GUID_LOPART(l_Guid), false);
}

void WorldSession::HandleSetContactNotesOpcode(WorldPacket& p_RecvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_SET_CONTACT_NOTES");

    uint64 l_Guid;
    uint32 l_VirtualRealmAddress;

    std::string l_Notes;

    p_RecvData >> l_VirtualRealmAddress;
    p_RecvData.readPackGUID(l_Guid);
    
    l_Notes = p_RecvData.ReadString(p_RecvData.ReadBits(10));

    m_Player->GetSocial()->SetFriendNote(GUID_LOPART(l_Guid), l_Notes);
}

void WorldSession::HandleReportBugOpcode(WorldPacket& p_RecvData)
{
    float l_PosX, l_PosY, l_PosZ, l_Orientation;
    uint32 l_ContentLen, l_MapID;
    std::string l_Content;

    p_RecvData >> l_MapID;
    p_RecvData >> l_PosX >> l_PosY >> l_PosZ >> l_Orientation;

    l_ContentLen = p_RecvData.ReadBits(10);
    p_RecvData.FlushBits();
    l_Content = p_RecvData.ReadString(l_ContentLen);

    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_INS_BUG_REPORT);
    l_Statement->setString(0, "Bug");
    l_Statement->setString(1, l_Content);
    CharacterDatabase.Execute(l_Statement);
}

void WorldSession::HandleReportSuggestionOpcode(WorldPacket& p_RecvData)
{
    float l_PosX, l_PosY, l_PosZ, l_Orientation;
    uint32 l_ContentLen, l_MapID;
    std::string l_Content;

    p_RecvData >> l_MapID;
    p_RecvData >> l_PosX >> l_PosY >> l_PosZ >> l_Orientation;

    l_ContentLen = p_RecvData.ReadBits(10);
    p_RecvData.FlushBits();
    l_Content = p_RecvData.ReadString(l_ContentLen);

    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_INS_BUG_REPORT);
    l_Statement->setString(0, "Suggestion");
    l_Statement->setString(1, l_Content);
    CharacterDatabase.Execute(l_Statement);
}

void WorldSession::HandleRequestGmTicket(WorldPacket& /*recvPakcet*/)
{
    // Notify player if he has a ticket in progress
    if (GmTicket* ticket = sTicketMgr->GetTicketByPlayer(GetPlayer()->GetGUID()))
    {
        if (ticket->IsCompleted())
            ticket->SendResponse(this);
        else
            sTicketMgr->SendTicket(this, ticket);
    }
}

void WorldSession::HandleReclaimCorpseOpcode(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_RECLAIM_CORPSE");

    uint64 l_CorpseGUID = 0;

    p_Packet.readPackGUID(l_CorpseGUID);
    
    if (GetPlayer()->isAlive())
        return;

    /// Do not allow corpse reclaim in arena
    if (GetPlayer()->InArena())
        return;

    /// Body not released yet
    if (!GetPlayer()->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
        return;

    Corpse * l_Corpse = GetPlayer()->GetCorpse();

    if (!l_Corpse)
        return;

    /// Prevent resurrect before 30-sec delay after body release not finished
    if (time_t(l_Corpse->GetGhostTime() + GetPlayer()->GetCorpseReclaimDelay(l_Corpse->GetType() == CORPSE_RESURRECTABLE_PVP)) > time_t(time(NULL)))
        return;

    if (!l_Corpse->IsWithinDistInMap(GetPlayer(), CORPSE_RECLAIM_RADIUS, true))
        return;

    /// Resurrect
    GetPlayer()->ResurrectPlayer(GetPlayer()->InBattleground() ? 1.0f : 0.5f);

    /// Spawn bones
    GetPlayer()->SpawnCorpseBones();
}

void WorldSession::HandleResurrectResponseOpcode(WorldPacket& p_RecvData)
{
    uint64 l_Guid = 0;
    p_RecvData.readPackGUID(l_Guid);
    uint32 l_Status = p_RecvData.read<uint32>();

    if (GetPlayer()->isAlive())
        return;

    if (l_Status == 1)
    {
        GetPlayer()->ClearResurrectRequestData();           // reject
        return;
    }

    if (!GetPlayer()->IsRessurectRequestedBy(l_Guid))
        return;

    GetPlayer()->ResurectUsingRequestData();
}

void WorldSession::HandleAreaTriggerOpcode(WorldPacket& recvData)
{
    uint32 triggerId;
    bool l_Enter;
    bool l_FromClient;

    recvData >> triggerId;
    l_Enter = recvData.ReadBit();
    l_FromClient = recvData.ReadBit();

    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_AREATRIGGER. Trigger ID: %u", triggerId);

    Player* player = GetPlayer();
    if (player->isInFlight())
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleAreaTriggerOpcode: Player '%s' (GUID: %u) in flight, ignore Area Trigger ID:%u",
            player->GetName(), player->GetGUIDLow(), triggerId);
        return;
    }

    AreaTriggerEntry const* atEntry = sAreaTriggerStore.LookupEntry(triggerId);
    if (!atEntry)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleAreaTriggerOpcode: Player '%s' (GUID: %u) send unknown (by DBC) Area Trigger ID:%u",
            player->GetName(), player->GetGUIDLow(), triggerId);
        return;
    }

    if (player->GetMapId() != atEntry->ContinentID)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleAreaTriggerOpcode: Player '%s' (GUID: %u) too far (trigger map: %u player map: %u), ignore Area Trigger ID: %u",
            player->GetName(), atEntry->ContinentID, player->GetMapId(), player->GetGUIDLow(), triggerId);
        return;
    }

    // delta is safe radius
    const float delta = 5.0f;

    if (atEntry->Radius > 0)
    {
        // if we have radius check it
        float dist = player->GetDistance(atEntry->Pos[0], atEntry->Pos[1], atEntry->Pos[2]);
        if (dist > atEntry->Radius + delta)
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleAreaTriggerOpcode: Player '%s' (GUID: %u) too far (radius: %f distance: %f), ignore Area Trigger ID: %u",
                player->GetName(), player->GetGUIDLow(), atEntry->Radius, dist, triggerId);
            return;
        }
    }
    else
    {
        // we have only extent

        // rotate the players position instead of rotating the whole cube, that way we can make a simplified
        // is-in-cube check and we have to calculate only one point instead of 4

        // 2PI = 360°, keep in mind that ingame orientation is counter-clockwise
        double rotation = 2 * M_PI - atEntry->BoxYaw;
        double sinVal = std::sin(rotation);
        double cosVal = std::cos(rotation);

        float playerBoxDistX = player->GetPositionX() - atEntry->Pos[0];
        float playerBoxDistY = player->GetPositionY() - atEntry->Pos[1];

        float rotPlayerX = float(atEntry->Pos[0] + playerBoxDistX * cosVal - playerBoxDistY*sinVal);
        float rotPlayerY = float(atEntry->Pos[1] + playerBoxDistY * cosVal + playerBoxDistX*sinVal);

        // box edges are parallel to coordiante axis, so we can treat every dimension independently :D
        float dz = player->GetPositionZ() - atEntry->Pos[2];
        float dx = rotPlayerX - atEntry->Pos[0];
        float dy = rotPlayerY - atEntry->Pos[1];
        if ((fabs(dx) > atEntry->BoxLength / 2 + delta) ||
            (fabs(dy) > atEntry->BoxWidth / 2 + delta) ||
            (fabs(dz) > atEntry->BoxHeight / 2 + delta))
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleAreaTriggerOpcode: Player '%s' (GUID: %u) too far (1/2 box X: %f 1/2 box Y: %f 1/2 box Z: %f rotatedPlayerX: %f rotatedPlayerY: %f dZ:%f), ignore Area Trigger ID: %u",
                player->GetName(), player->GetGUIDLow(), atEntry->BoxLength / 2, atEntry->BoxWidth / 2, atEntry->BoxHeight / 2, rotPlayerX, rotPlayerY, dz, triggerId);
            return;
        }
    }

    if (player->isDebugAreaTriggers)
        ChatHandler(player).PSendSysMessage(LANG_DEBUG_AREATRIGGER_REACHED, triggerId);

    if (sScriptMgr->OnAreaTrigger(player, atEntry))
        return;

    if (player->isAlive())
        if (uint32 questId = sObjectMgr->GetQuestForAreaTrigger(triggerId))
            if (player->GetQuestStatus(questId) == QUEST_STATUS_INCOMPLETE)
                player->AreaExploredOrEventHappens(questId);

    if (sObjectMgr->IsTavernAreaTrigger(triggerId))
    {
        // set resting flag we are in the inn
        player->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_RESTING);
        player->InnEnter(time(NULL), atEntry->ContinentID, atEntry->Pos[0], atEntry->Pos[1], atEntry->Pos[2]);

        player->SetRestType(REST_TYPE_IN_TAVERN);

        if (sWorld->IsFFAPvPRealm())
            player->RemoveByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 1, UNIT_BYTE2_FLAG_FFA_PVP);

        return;
    }

    if (Battleground* bg = player->GetBattleground())
        if (bg->GetStatus() == STATUS_IN_PROGRESS)
        {
            bg->HandleAreaTrigger(player, triggerId);
            return;
        }

        if (OutdoorPvP* pvp = player->GetOutdoorPvP())
            if (pvp->HandleAreaTrigger(m_Player, triggerId))
                return;

        AreaTriggerStruct const* at = sObjectMgr->GetAreaTrigger(triggerId);
        if (!at)
            return;

        bool teleported = false;
        if (player->GetMapId() != at->target_mapId)
        {
            if (!sMapMgr->CanPlayerEnter(at->target_mapId, player, false))
                return;

            if (Group* group = player->GetGroup())
                if (group->isLFGGroup() && player->GetMap()->IsDungeon())
                    teleported = player->TeleportToBGEntryPoint();
        }

        if (!teleported)
            player->TeleportTo(at->target_mapId, at->target_X, at->target_Y, at->target_Z, at->target_Orientation, TELE_TO_NOT_LEAVE_TRANSPORT);
}

void WorldSession::HandleUpdateAccountData(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_UPDATE_ACCOUNT_DATA");

    uint64 l_CharacterGUID = 0;

    uint32 l_Time               = 0;
    uint32 l_DataType           = 0;
    uint32 l_CompressedSize     = 0;
    uint32 l_UncompressedSize   = 0;

    p_Packet.readPackGUID(l_CharacterGUID);
    p_Packet >> l_Time;
    p_Packet >> l_UncompressedSize;

    l_DataType = p_Packet.ReadBits(3);

    p_Packet >> l_CompressedSize;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "UAD: type %u, time %u, decompressedSize %u", l_DataType, l_Time, l_UncompressedSize);

    ///< Erase
    if (l_UncompressedSize == 0)
    {
        SetAccountData(AccountDataType(l_DataType), 0, "");
        return;
    }

    if (l_UncompressedSize > 0xFFFF)
    {
        ///< Unnneded warning spam in this case
        p_Packet.rfinish();
        sLog->outError(LOG_FILTER_NETWORKIO, "UAD: Account data packet too big, size %u", l_UncompressedSize);
        return;
    }

    ByteBuffer l_Data;
    l_Data.resize(l_UncompressedSize);

    uLongf l_RealSize = l_UncompressedSize;
    if (uncompress(const_cast<uint8*>(l_Data.contents()), &l_RealSize, const_cast<uint8*>(p_Packet.contents() + p_Packet.rpos()), p_Packet.size() - p_Packet.rpos()) != Z_OK)
    {
        ///< Unnneded warning spam in this case
        p_Packet.rfinish();
        sLog->outError(LOG_FILTER_NETWORKIO, "UAD: Failed to decompress account data");
        return;
    }

    std::string l_AccountData = l_Data.ReadString(l_UncompressedSize);
    SetAccountData(AccountDataType(l_DataType), l_Time, l_AccountData);
}

void WorldSession::HandleRequestAccountData(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_REQUEST_ACCOUNT_DATA");

    uint64 l_CharacterGuid = 0;
    uint32 l_Type;

    p_Packet.readPackGUID(l_CharacterGuid);
    l_Type = p_Packet.ReadBits(3);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "RAD: type %u", l_Type);

    if (l_Type > NUM_ACCOUNT_DATA_TYPES)
        return;

    AccountData* l_AccountData = GetAccountData(AccountDataType(l_Type));

    uint32 l_Size       = l_AccountData->Data.size();
    uLongf l_DestSize   = compressBound(l_Size);

    ByteBuffer l_CompressedData;
    l_CompressedData.resize(l_DestSize);

    if (l_Size && compress(const_cast<uint8*>(l_CompressedData.contents()), &l_DestSize, (uint8*)l_AccountData->Data.c_str(), l_Size) != Z_OK)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "RAD: Failed to compress account data");
        return;
    }

    l_CompressedData.resize(l_DestSize);

    WorldPacket l_Response(SMSG_UPDATE_ACCOUNT_DATA, 4+4+4+3+3+5+8+l_DestSize);

    l_Response.appendPackGUID(l_CharacterGuid);
    l_Response << uint32(l_AccountData->Time);      /// unix time
    l_Response << uint32(l_Size);                   /// decompressed length
    l_Response.WriteBits(l_Type, 3);
    l_Response.FlushBits();
    l_Response << uint32(l_DestSize);               /// compressed length
    l_Response.append(l_CompressedData);            /// compressed data

    SendPacket(&l_Response);
}

int32 WorldSession::HandleEnableNagleAlgorithm()
{
    return 0;
}

void WorldSession::HandleSetActionButtonOpcode(WorldPacket& p_RecvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_SET_ACTION_BUTTON");

    uint64 l_PackedData = 0;

    uint8 l_Button = 0;

    p_RecvData >> l_PackedData;
    p_RecvData >> l_Button;

    uint8   l_Type      = ACTION_BUTTON_TYPE(l_PackedData);
    uint32  l_ActionID  = ACTION_BUTTON_ACTION(l_PackedData);

    if (!l_PackedData)
    {
        sLog->outInfo(LOG_FILTER_NETWORKIO, "MISC: Remove action from button %u", l_Button);
        GetPlayer()->removeActionButton(l_Button);
    }
    else
    {
        GetPlayer()->addActionButton(l_Button, l_ActionID, l_Type);
    }
}

void WorldSession::HandleCompleteCinematic(WorldPacket& /*recvData*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_COMPLETE_CINEMATIC");

    m_Player->StopCinematic();
}

void WorldSession::HandleNextCinematicCamera(WorldPacket& /*recvData*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_NEXT_CINEMATIC_CAMERA");
}

void WorldSession::HandleMoveTimeSkippedOpcode(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_MOVE_TIME_SKIPPED");

    uint64 p_MoverGUID;
    uint32 p_Time;

    p_Packet.readPackGUID(p_MoverGUID);
    p_Packet >> p_Time;

    //TODO!

    /*
    uint64 guid;
    uint32 time_skipped;
    recvData >> guid;
    recvData >> time_skipped;
    sLog->outDebug(LOG_FILTER_PACKETIO, "WORLD: CMSG_MOVE_TIME_SKIPPED");

    /// TODO
    must be need use in Trinity
    We substract server Lags to move time (AntiLags)
    for exmaple
    GetPlayer()->ModifyLastMoveTime(-int32(time_skipped));
    */
}

void WorldSession::HandleFeatherFallAck(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_MOVE_FEATHER_FALL_ACK");

    // no used
    recvData.rfinish();                       // prevent warnings spam
}

void WorldSession::HandleMoveUnRootAck(WorldPacket& recvData)
{
    // no used
    recvData.rfinish();                       // prevent warnings spam
    /*
    uint64 guid;
    recvData >> guid;

    // now can skip not our packet
    if (_player->GetGUID() != guid)
    {
    recvData.rfinish();                   // prevent warnings spam
    return;
    }

    sLog->outDebug(LOG_FILTER_PACKETIO, "WORLD: CMSG_FORCE_MOVE_UNROOT_ACK");

    recvData.read_skip<uint32>();                          // unk

    MovementInfo movementInfo;
    movementInfo.guid = guid;
    ReadMovementInfo(recvData, &movementInfo);
    recvData.read_skip<float>();                           // unk2
    */
}

void WorldSession::HandleMoveRootAck(WorldPacket& recvData)
{
    // no used
    recvData.rfinish();                       // prevent warnings spam
    /*
    uint64 guid;
    recvData >> guid;

    // now can skip not our packet
    if (_player->GetGUID() != guid)
    {
    recvData.rfinish();                   // prevent warnings spam
    return;
    }

    sLog->outDebug(LOG_FILTER_PACKETIO, "WORLD: CMSG_FORCE_MOVE_ROOT_ACK");

    recvData.read_skip<uint32>();                          // unk

    MovementInfo movementInfo;
    ReadMovementInfo(recvData, &movementInfo);
    */
}

void WorldSession::HandleSetActionBarToggles(WorldPacket& p_Packet)
{
    uint8 l_ActionBar;

    p_Packet >> l_ActionBar;

    if (!m_Player)                                        // ignore until not logged (check needed because STATUS_AUTHED)
    {
        if (l_ActionBar != 0)
            sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetActionBarToggles in not logged state with value: %u, ignored", uint32(l_ActionBar));
        return;
    }

    m_Player->SetByteValue(PLAYER_FIELD_LIFETIME_MAX_RANK, 1, l_ActionBar);
}

void WorldSession::HandlePlayedTime(WorldPacket& recvData)
{
    bool l_TriggerScriptEvent = recvData.ReadBit();                 // 0 or 1 expected

    WorldPacket data(SMSG_PLAYED_TIME, 4 + 4 + 1);
    data << uint32(m_Player->GetTotalPlayedTime());
    data << uint32(m_Player->GetLevelPlayedTime());
    data.WriteBit(l_TriggerScriptEvent);                            // 0 - will not show in chat frame
    data.FlushBits();
    SendPacket(&data);
}

void WorldSession::HandleInspectOpcode(WorldPacket& p_RecvData)
{
    uint64 l_PlayerGuid = 0;
    p_RecvData.readPackGUID(l_PlayerGuid);

    m_Player->SetSelection(l_PlayerGuid);

    Player* l_Player = ObjectAccessor::FindPlayer(l_PlayerGuid);
    if (!l_Player)
        return;

    uint32 l_TalentCount = 0;
    uint32 l_GlyphCount = 0;
    uint32 l_EquipmentCount = 0;

    WorldPacket l_Data(SMSG_INSPECT_TALENT);

    l_Data.appendPackGUID(l_PlayerGuid);

    size_t l_EquipmentCountPos = l_Data.wpos();
    l_Data << uint32(l_EquipmentCount);
    size_t l_GlyphCountPos = l_Data.wpos();
    l_Data << uint32(l_GlyphCount);
    size_t l_TalentCountPos = l_Data.wpos();
    l_Data << uint32(l_TalentCount);

    l_Data << int32(l_Player->getClass());
    l_Data << int32(l_Player->GetSpecializationId(l_Player->GetActiveSpec()));
    l_Data << int32(0);

    for (uint8 l_Iter = 0; l_Iter < EQUIPMENT_SLOT_END; ++l_Iter)
    {
        Item* l_Item = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, l_Iter);
        if (!l_Item)
            continue;

        uint64 l_ItemCreator = l_Item->GetGuidValue(ITEM_FIELD_CREATOR);

        ++l_EquipmentCount;

        l_Data.appendPackGUID(l_ItemCreator);

        // Item_Struct
        {
            l_Data << uint32(l_Item->GetEntry());
            l_Data << uint32(l_Item->GetItemSuffixFactor());
            l_Data << int32(l_Item->GetItemRandomPropertyId());

            bool l_HasBonuses = l_Item->GetDynamicValues(ITEM_DYNAMIC_FIELD_BONUSLIST_IDS).size() > 0;
            bool l_HasModifiers = l_Item->GetDynamicValues(ITEM_DYNAMIC_FIELD_MODIFIERS).size() > 0;

            l_Data.WriteBit(l_HasBonuses);
            l_Data.WriteBit(l_HasModifiers);

            if (l_HasBonuses)
            {
                l_Data << uint8(0);     ///< UnkByte
                l_Data << uint32(0);    ///< Count
            }

            if (l_HasModifiers)
            {
                uint32 l_ModifyMask = l_Item->GetUInt32Value(ITEM_FIELD_MODIFIERS_MASK);

                l_Data << uint32(l_Item->GetUInt32Value(ITEM_FIELD_MODIFIERS_MASK));

                if (l_ModifyMask & ITEM_TRANSMOGRIFIED)
                    l_Data << uint32(l_Item->GetDynamicValue(ITEM_DYNAMIC_FIELD_MODIFIERS, 0));
            }
        }

        l_Data << uint8(l_Iter);
        size_t l_EnchantCountPos = l_Data.wpos();
        l_Data << uint32(0);

        uint32 l_EnchantCount = 0;
        for (uint8 l_EnchantSlot = 0; l_EnchantSlot < MAX_ENCHANTMENT_SLOT; ++l_EnchantSlot)
        {
            uint32 l_EnchantID = l_Item->GetEnchantmentId(EnchantmentSlot(l_EnchantSlot));
            if (!l_EnchantID)
                continue;

            ++l_EnchantCount;

            l_Data << uint32(l_EnchantID);
            l_Data << uint8(l_EnchantSlot);
        }

        l_Data.put(l_EnchantCountPos, l_EnchantCount);
        l_Data.WriteBit(true);
    }

    l_Data.put(l_EquipmentCountPos, l_EquipmentCount);

    for (uint8 l_Iter = 0; l_Iter < MAX_GLYPH_SLOT_INDEX; ++l_Iter)
    {
        if (l_Player->GetGlyph(0, l_Iter) == 0)
            continue;

        l_Data << uint16(l_Player->GetGlyph(0, l_Iter));               // GlyphProperties.dbc
        ++l_GlyphCount;
    }

    l_Data.put(l_GlyphCountPos, l_GlyphCount);

    for (auto l_Talent : *l_Player->GetTalentMap(l_Player->GetActiveSpec()))
    {
        SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(l_Talent.first);
        if (l_SpellInfo && !l_SpellInfo->m_TalentIDs.empty())
        {
            uint32 l_SpecID = l_Player->GetSpecializationId(l_Player->GetActiveSpec());
            uint16 l_Talent = 0;

            for (uint32 l_TalentID : l_SpellInfo->m_TalentIDs)
            {
                if (TalentEntry const* l_TalentEntry = sTalentStore.LookupEntry(l_TalentID))
                {
                    if (l_TalentEntry->SpecID == l_SpecID)
                    {
                        l_Talent = l_TalentID;
                        break;
                    }

                    l_Talent = l_TalentID;
                }
            }

            if (!l_Talent)
                continue;

            l_Data << uint16(l_Talent);
            ++l_TalentCount;
        }
    }

    l_Data.put(l_TalentCountPos, l_TalentCount);

    Guild* l_Guild = sGuildMgr->GetGuildById(l_Player->GetGuildId());
    l_Data.WriteBit(l_Guild != nullptr);

    if (l_Guild != nullptr)
    {
        uint64 l_GuildGuid = l_Guild->GetGUID();
        l_Data.appendPackGUID(l_GuildGuid);
        l_Data << uint32(l_Guild->GetMembersCount());
        l_Data << uint32(l_Guild->GetAchievementMgr().GetAchievementPoints());
    }

    SendPacket(&l_Data);
}

void WorldSession::HandleInspectHonorStatsOpcode(WorldPacket& p_RecvData)
{
    uint64 l_Guid = 0;
    p_RecvData.readPackGUID(l_Guid);

    Player* l_Player = ObjectAccessor::FindPlayer(l_Guid);
    if (!l_Player)
        return;

    WorldPacket l_Data(SMSG_INSPECT_HONOR_STATS);
    l_Data.appendPackGUID(l_Guid);
    l_Data << uint8(l_Player->GetByteValue(PLAYER_FIELD_LIFETIME_MAX_RANK, 1));
    l_Data << uint16(l_Player->GetUInt16Value(PLAYER_FIELD_YESTERDAY_HONORABLE_KILLS, 0));
    l_Data << uint16(l_Player->GetUInt16Value(PLAYER_FIELD_YESTERDAY_HONORABLE_KILLS, 1));
    l_Data << uint32(l_Player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS));
    SendPacket(&l_Data);
}

void WorldSession::HandleInspectRatedBGStatsOpcode(WorldPacket& p_RecvData)
{
    uint32 l_InspectRealmAddress;
    uint64 l_Guid = 0;

    p_RecvData.readPackGUID(l_Guid);
    p_RecvData >> l_InspectRealmAddress;

    Player* l_Player = ObjectAccessor::FindPlayer(l_Guid);
    if (!l_Player)
        return;

    WorldPacket l_Data(SMSG_INSPECT_RATED_BG_STATS);
    l_Data.appendPackGUID(l_Guid);
    l_Data.WriteBits(MAX_ARENA_SLOT, 3);

    for (uint8 l_Iter = 0; l_Iter < MAX_ARENA_SLOT; ++l_Iter)
    {
        l_Data << int32(l_Player->GetArenaPersonalRating(l_Iter));
        l_Data << int32(0);
        l_Data << int32(l_Player->GetWeekGames(l_Iter));
        l_Data << int32(l_Player->GetWeekWins(l_Iter));
        l_Data << int32(l_Player->GetSeasonGames(l_Iter));
        l_Data << int32(l_Player->GetSeasonWins(l_Iter));
        l_Data << int32(l_Player->GetBestRatingOfWeek(l_Iter));
        l_Data << l_Iter;
    }

    SendPacket(&l_Data);
}

void WorldSession::HandleWorldTeleportOpcode(WorldPacket& recvData)
{
    uint32 time;
    uint32 mapid;
    float PositionX;
    float PositionY;
    float PositionZ;
    float Orientation;

    recvData >> time;                                      // time in m.sec.
    recvData >> mapid;
    recvData >> PositionX;
    recvData >> Orientation;
    recvData >> PositionY;
    recvData >> PositionZ;                          // o (3.141593 = 180 degrees)

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_WORLD_TELEPORT");

    if (GetPlayer()->isInFlight())
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "Player '%s' (GUID: %u) in flight, ignore worldport command.", GetPlayer()->GetName(), GetPlayer()->GetGUIDLow());
        return;
    }

    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_WORLD_TELEPORT: Player = %s, Time = %u, map = %u, x = %f, y = %f, z = %f, o = %f", GetPlayer()->GetName(), time, mapid, PositionX, PositionY, PositionZ, Orientation);

    if (AccountMgr::IsAdminAccount(GetSecurity()))
        GetPlayer()->TeleportTo(mapid, PositionX, PositionY, PositionZ, Orientation);
    else
        SendNotification(LANG_YOU_NOT_HAVE_PERMISSION);
}

void WorldSession::HandleWhoisOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Received opcode CMSG_WHOIS");

    uint32 textLength = recvData.ReadBits(6);
    recvData.FlushBits();
    std::string charname = recvData.ReadString(textLength);

    if (!AccountMgr::IsAdminAccount(GetSecurity()))
    {
        SendNotification(LANG_YOU_NOT_HAVE_PERMISSION);
        return;
    }

    if (charname.empty() || !normalizePlayerName (charname))
    {
        SendNotification(LANG_NEED_CHARACTER_NAME);
        return;
    }

    Player* player = sObjectAccessor->FindPlayerByName(charname.c_str());

    if (!player)
    {
        SendNotification(LANG_PLAYER_NOT_EXIST_OR_OFFLINE, charname.c_str());
        return;
    }

    uint32 accid = player->GetSession()->GetAccountId();

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_WHOIS);

    stmt->setUInt32(0, accid);

    PreparedQueryResult result = LoginDatabase.Query(stmt);

    if (!result)
    {
        SendNotification(LANG_ACCOUNT_FOR_PLAYER_NOT_FOUND, charname.c_str());
        return;
    }

    Field* fields = result->Fetch();
    std::string acc = fields[0].GetString();
    if (acc.empty())
        acc = "Unknown";
    std::string email = fields[1].GetString();
    if (email.empty())
        email = "Unknown";
    std::string lastip = fields[2].GetString();
    if (lastip.empty())
        lastip = "Unknown";

    std::string msg = charname + "'s " + "account is " + acc + ", e-mail: " + email + ", last ip: " + lastip;

    WorldPacket l_Data(SMSG_WHOIS, msg.size()+1);
    l_Data.WriteBits(msg.size(), 11);
    l_Data.FlushBits();
    l_Data.WriteString(msg);
    SendPacket(&l_Data);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Received whois command from player %s for character %s", GetPlayer()->GetName(), charname.c_str());
}

void WorldSession::HandleComplainOpcode(WorldPacket& recvData)
{
    // recvData is not empty, but all data are unused in core
    // NOTE: all chat messages from this spammer automatically ignored by spam reporter until logout in case chat spam.
    // if it's mail spam - ALL mails from this spammer automatically removed by client

    // Complaint Received message
    WorldPacket data(SMSG_COMPLAIN_RESULT, 2);
    data << uint32(0);  // value 0xC generates a "CalendarError" in client.
    data << uint8(0);   // value 1 resets CGChat::m_complaintsSystemStatus in client. (unused?)
    SendPacket(&data);
}

void WorldSession::HandleRealmSplitOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_REALM_SPLIT");

    uint32 unk;
    std::string split_date = "01/01/01";
    recvData >> unk;

    WorldPacket data(SMSG_REALM_SPLIT);
    data.WriteBits(split_date.size(), 7);
    data << unk;
    data << uint32(0x00000000);                             // realm split state
    data << split_date;
    SendPacket(&data);
}

enum RealmQueryNameResponse
{
    REALM_QUERY_NAME_RESPONSE_OK        = 0,
    REALM_QUERY_NAME_RESPONSE_DENY      = 1,
    REALM_QUERY_NAME_RESPONSE_RETRY     = 2,
    REALM_QUERY_NAME_RESPONSE_OK_TEMP   = 3,
};

void WorldSession::HandleRealmQueryNameOpcode(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_REALM_QUERY_NAME");

    uint32 l_RealmID = 0;

    p_Packet >> l_RealmID;

    /// Cheater ?
    if (l_RealmID != g_RealmID)
        return;

    WorldPacket l_Data(SMSG_REALM_QUERY_RESPONSE);
    l_Data << uint32(g_RealmID);                                    ///< Virtual Realm Address
    l_Data << uint8(REALM_QUERY_NAME_RESPONSE_OK);                  ///< Lookup State

    l_Data.WriteBit(true);                                          ///< Is Locale
    l_Data.WriteBits(sWorld->GetRealmName().size(), 8);             ///< Realm Name Actual
    l_Data.WriteBits(sWorld->GetNormalizedRealmName().size(), 8);   ///< Realm Name Normalized
    l_Data.FlushBits();

    l_Data.WriteString(sWorld->GetRealmName());                     ///< Realm Name Actual
    l_Data.WriteString(sWorld->GetNormalizedRealmName());           ///< Realm Name Normalized

    SendPacket(&l_Data);
}

void WorldSession::HandleFarSightOpcode(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_FAR_SIGHT");

    bool l_Apply = p_Packet.ReadBit();

    if (l_Apply)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "Player %u set vision to self", m_Player->GetGUIDLow());
        m_Player->SetSeer(m_Player);
    }
    else
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "Added FarSight " UI64FMTD " to player %u", m_Player->GetGuidValue(PLAYER_FIELD_FARSIGHT_OBJECT), m_Player->GetGUIDLow());

        if (WorldObject * l_Target = m_Player->GetViewpoint())
            m_Player->SetSeer(l_Target);
        else
            sLog->outError(LOG_FILTER_NETWORKIO, "Player %s requests non-existing seer " UI64FMTD, m_Player->GetName(), m_Player->GetGuidValue(PLAYER_FIELD_FARSIGHT_OBJECT));
    }

    GetPlayer()->UpdateVisibilityForPlayer();
}

void WorldSession::HandleSetTitleOpcode(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_SET_TITLE");

    uint32 l_Title = 0;

    p_Packet >> l_Title;

    /// -1 at none
    if (l_Title > 0 && l_Title < MAX_TITLE_INDEX)
    {
        if (!GetPlayer()->HasTitle(l_Title))
            return;
    }
    else
        l_Title = 0;

    GetPlayer()->SetUInt32Value(PLAYER_FIELD_PLAYER_TITLE, l_Title);
}

void WorldSession::HandleTimeSyncResp(WorldPacket& p_RecvData)
{
    uint32 l_Counter, l_ClientTicks;
    p_RecvData >> l_Counter >> l_ClientTicks;

    if (l_Counter != m_Player->m_timeSyncCounter - 1)
        sLog->outDebug(LOG_FILTER_NETWORKIO, "Wrong time sync counter from player %s (cheater?)", m_Player->GetName());

    m_Player->m_timeSyncClient = l_ClientTicks;
}

void WorldSession::HandleResetInstancesOpcode(WorldPacket& /*p_RecvData*/)
{
    if (Group* l_Group = m_Player->GetGroup())
    {
        if (l_Group->IsLeader(m_Player->GetGUID()))
            l_Group->ResetInstances(INSTANCE_RESET_ALL, false, m_Player);
    }
    else
        m_Player->ResetInstances(INSTANCE_RESET_ALL, false);
}

void WorldSession::HandleResetChallengeModeOpcode(WorldPacket& /*recvData*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_RESET_CHALLENGE_MODE");

    // @TODO: Do something about challenge mode ...
}

void WorldSession::HandleSetDungeonDifficultyOpcode(WorldPacket & recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "MSG_SET_DUNGEON_DIFFICULTY");

    uint32 mode;
    recvData >> mode;

    if (mode != CHALLENGE_MODE_DIFFICULTY && mode >= MAX_DUNGEON_DIFFICULTY)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetDungeonDifficultyOpcode: player %d sent an invalid instance mode %d!", m_Player->GetGUIDLow(), mode);
        return;
    }

    if (Difficulty(mode) == m_Player->GetDungeonDifficulty())
        return;

    // cannot reset while in an instance
    Map* map = m_Player->FindMap();
    if (map && map->IsDungeon())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetDungeonDifficultyOpcode: player (Name: %s, GUID: %u) tried to reset the instance while player is inside!", m_Player->GetName(), m_Player->GetGUIDLow());
        return;
    }

    Group* group = m_Player->GetGroup();
    if (group)
    {
        if (group->IsLeader(m_Player->GetGUID()))
        {
            for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* groupGuy = itr->getSource();
                if (!groupGuy)
                    continue;

                if (!groupGuy->IsInMap(groupGuy))
                    return;

                if (groupGuy->GetMap()->IsNonRaidDungeon())
                {
                    sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetDungeonDifficultyOpcode: player %d tried to reset the instance while group member (Name: %s, GUID: %u) is inside!", m_Player->GetGUIDLow(), groupGuy->GetName(), groupGuy->GetGUIDLow());
                    return;
                }
            }
            // the difficulty is set even if the instances can't be reset
            //_player->SendDungeonDifficulty(true);
            group->ResetInstances(INSTANCE_RESET_CHANGE_DIFFICULTY, false, m_Player);
            group->SetDungeonDifficulty(Difficulty(mode));
            m_Player->SendDungeonDifficulty(true);
        }
    }
    else
    {
        m_Player->ResetInstances(INSTANCE_RESET_CHANGE_DIFFICULTY, false);
        m_Player->SetDungeonDifficulty(Difficulty(mode));
        m_Player->SendDungeonDifficulty(false);
    }
}

void WorldSession::HandleSetRaidDifficultyOpcode(WorldPacket& p_RecvData)
{
    uint32 l_Difficulty;
    uint8 l_IsLegacyDifficulty;

    p_RecvData >> l_Difficulty;
    p_RecvData >> l_IsLegacyDifficulty;

    if (!l_IsLegacyDifficulty && (l_Difficulty < NORMAL_DIFFICULTY || l_Difficulty > MYTHIC_DIFFICULTY))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetRaidDifficultyOpcode: player %d sent an invalid instance mode %d!", m_Player->GetGUIDLow(), l_Difficulty);
        return;
    }

    if (l_IsLegacyDifficulty && (l_Difficulty < LEGACY_MAN10_DIFFICULTY || l_Difficulty > LEGACY_MAN25_HEROIC_DIFFICULTY))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetRaidDifficultyOpcode: player %d sent an invalid instance mode %d!", m_Player->GetGUIDLow(), l_Difficulty);
        return;
    }

    // cannot reset while in an instance
    Map* l_Map = m_Player->FindMap();
    if (l_Map && l_Map->IsDungeon())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetRaidDifficultyOpcode: player %d tried to reset the instance while inside!", m_Player->GetGUIDLow());
        return;
    }

    if (!l_IsLegacyDifficulty && Difficulty(l_Difficulty) == m_Player->GetRaidDifficulty())
        return;

    if (l_IsLegacyDifficulty && Difficulty(l_Difficulty) == m_Player->GetLegacyRaidDifficulty())
        return;

    Group* group = m_Player->GetGroup();
    if (group)
    {
        if (group->IsLeader(m_Player->GetGUID()))
        {
            for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* groupGuy = itr->getSource();
                if (!groupGuy)
                    continue;

                if (!groupGuy->IsInMap(groupGuy))
                    return;

                if (groupGuy->GetMap()->IsRaid())
                {
                    sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetRaidDifficultyOpcode: player %d tried to reset the instance while inside!", m_Player->GetGUIDLow());
                    return;
                }
            }
            // the difficulty is set even if the instances can't be reset
            //_player->SendDungeonDifficulty(true);
            group->ResetInstances(INSTANCE_RESET_CHANGE_DIFFICULTY, true, m_Player);

            if (l_IsLegacyDifficulty)
                group->SetLegacyRaidDifficulty(Difficulty(l_Difficulty));
            else
                group->SetRaidDifficulty(Difficulty(l_Difficulty));

            m_Player->SendRaidDifficulty(true);
        }
    }
    else
    {
        m_Player->ResetInstances(INSTANCE_RESET_CHANGE_DIFFICULTY, true);

        if (l_IsLegacyDifficulty)
            m_Player->SetLegacyRaidDifficulty(Difficulty(l_Difficulty));
        else
            m_Player->SetRaidDifficulty(Difficulty(l_Difficulty));

        m_Player->SendRaidDifficulty(false);
    }
}

void WorldSession::HandleCancelMountAuraOpcode(WorldPacket& /*recvData*/)
{
    // If player is not mounted, so go out :)
    if (!m_Player->IsMounted())                              // not blizz like; no any messages on blizz
    {
        ChatHandler(this).SendSysMessage(LANG_CHAR_NON_MOUNTED);
        return;
    }

    if (m_Player->isInFlight())                               // not blizz like; no any messages on blizz
    {
        ChatHandler(this).SendSysMessage(LANG_YOU_IN_FLIGHT);
        return;
    }

    m_Player->Dismount();
    m_Player->RemoveAurasByType(SPELL_AURA_MOUNTED);

    WorldPacket l_Data(SMSG_DISMOUNT);
    l_Data.appendPackGUID(m_Player->GetGUID());
    m_Player->SendMessageToSet(&l_Data, true);
}

void WorldSession::HandleRequestPetInfoOpcode(WorldPacket& /*recvData */)
{
    /*
    sLog->outDebug(LOG_FILTER_PACKETIO, "WORLD: CMSG_REQUEST_PET_INFO");
    recvData.hexlike();
    */
}

void WorldSession::HandleSetTaxiBenchmarkOpcode(WorldPacket& recvData)
{
    bool mode = recvData.ReadBit();

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Client used \"/timetest %d\" command", mode);
}

void WorldSession::HandleQueryInspectAchievements(WorldPacket& p_RecvData)
{
    uint64 l_Guid = 0;
    p_RecvData.readPackGUID(l_Guid);

    Player* l_Player = ObjectAccessor::FindPlayer(l_Guid);
    if (!l_Player)
        return;

    l_Player->GetAchievementMgr().SendAchievementInfo(m_Player);
}

void WorldSession::HandleGuildAchievementProgressQuery(WorldPacket& p_Packet)
{
    uint32 l_AchievementID = 0;

    p_Packet >> l_AchievementID;

    if (Guild * l_Guild = sGuildMgr->GetGuildById(m_Player->GetGuildId()))
        l_Guild->GetAchievementMgr().SendAchievementInfo(m_Player, l_AchievementID);
}

void WorldSession::HandleWorldStateUITimerUpdate(WorldPacket& /*p_RecvData*/)
{
    WorldPacket l_Data(SMSG_UITIME, 4);
    l_Data << uint32(time(NULL));         ///< Time
    SendPacket(&l_Data);
}

/// - Blizzard have merge CMSG_GET_UNDELETE_CHARACTER_COOLDOWN_STATUS & CMSG_READY_FOR_ACCOUNT_DATA_TIMES (see history of CharacterSelect_OnShow function in "GlueXML/CharacterSelect.lua" of WoW UI Source)
void WorldSession::HandleUndeleteCharacter(WorldPacket& /*p_RecvData*/)
{
    SendAccountDataTimes(GLOBAL_CACHE_MASK);
}

void WorldSession::SendSetPhaseShift(std::set<uint32> const& phaseIds, std::set<uint32> const& terrainswaps)
{
    ObjectGuid guid = m_Player->GetGUID();
    uint32 unkValue = 0;
    uint32 inactiveSwapsCount = 0;

    WorldPacket data(SMSG_SET_PHASE_SHIFT, 1 + 8 + 4 + 4 + 4 + 4 + 2 * phaseIds.size() + 4 + terrainswaps.size() * 2);
    data.appendPackGUID(m_Player->GetGUID());
    // 0x8 or 0x10 is related to areatrigger, if we send flags 0x00 areatrigger doesn't work in some case
    data << uint32(0x18); // flags, 0x18 most of time on retail sniff
    data << uint32(phaseIds.size() * 2);        // Phase.dbc ids
    data.appendPackGUID(0);
    // Active terrain swaps, may switch with inactive terrain

    for (std::set<uint32>::const_iterator itr = phaseIds.begin(); itr != phaseIds.end(); ++itr)
    {
        data << uint16(1);
        data << uint16(*itr); // Most of phase id on retail sniff have 0x8000 mask
    }

    // Inactive terrain swaps, may switch with active terrain
    data << inactiveSwapsCount;
    //for (uint8 i = 0; i < inactiveSwapsCount; ++i)
    //data << uint16(0);

    // WorldMapAreaId ?
    data << unkValue;
    //for (uint32 i = 0; i < unkValue; i++)
        //data << uint16(0);

    data << uint32(terrainswaps.size() * 2);

    for (std::set<uint32>::const_iterator itr = terrainswaps.begin(); itr != terrainswaps.end(); ++itr)
        data << uint16(*itr);

    SendPacket(&data);
}

// Battlefield and Battleground
void WorldSession::HandleAreaSpiritHealerQueryOpcode(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_AREA_SPIRIT_HEALER_QUERY");

    Battleground* l_Battleground = m_Player->GetBattleground();

    uint64 l_Healer = 0;

    p_Packet.readPackGUID(l_Healer);

    Creature * l_Unit = GetPlayer()->GetMap()->GetCreature(l_Healer);

    if (!l_Unit)
        return;

    /// It's not spirit service
    if (!l_Unit->isSpiritService())
        return;

    if (l_Battleground)
        sBattlegroundMgr->SendAreaSpiritHealerQueryOpcode(m_Player, l_Battleground, l_Healer);

    if (Battlefield * l_Battlefield = sBattlefieldMgr->GetBattlefieldToZoneId(m_Player->GetZoneId()))
        l_Battlefield->SendAreaSpiritHealerQueryOpcode(m_Player,l_Healer);
}

void WorldSession::HandleAreaSpiritHealerQueueOpcode(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_AREA_SPIRIT_HEALER_QUEUE");

    Battleground* l_Battleground = m_Player->GetBattleground();
    uint64 l_Healer = 0;

    p_Packet.readPackGUID(l_Healer);

    Creature* l_Unit = GetPlayer()->GetMap()->GetCreature(l_Healer);

    if (!l_Unit)
        return;

    /// it's not spirit service
    if (!l_Unit->isSpiritService())
        return;

    if (l_Battleground)
        l_Battleground->AddPlayerToResurrectQueue(l_Healer, m_Player->GetGUID());

    if (Battlefield * l_Battlefield = sBattlefieldMgr->GetBattlefieldToZoneId(m_Player->GetZoneId()))
        l_Battlefield->AddPlayerToResurrectQueue(l_Healer, m_Player->GetGUID());
}

void WorldSession::HandleHearthAndResurrect(WorldPacket& /*recvData*/)
{
    if (m_Player->isInFlight())
        return;

    if (/*Battlefield* bf =*/ sBattlefieldMgr->GetBattlefieldToZoneId(m_Player->GetZoneId()))
    {
        // bf->PlayerAskToLeave(_player);                   //@todo: FIXME
        return;
    }

    AreaTableEntry const* atEntry = GetAreaEntryByAreaID(m_Player->GetAreaId());
    if (!atEntry || !(atEntry->Flags & AREA_FLAG_WINTERGRASP_2))
        return;

    m_Player->BuildPlayerRepop();
    m_Player->ResurrectPlayer(100);
    m_Player->TeleportTo(m_Player->m_homebindMapId, m_Player->m_homebindX, m_Player->m_homebindY, m_Player->m_homebindZ, m_Player->GetOrientation());
}

void WorldSession::HandleInstanceLockResponse(WorldPacket& recvPacket)
{
    uint8 accept;
    recvPacket >> accept;

    if (!m_Player->HasPendingBind())
    {
        sLog->outInfo(LOG_FILTER_NETWORKIO, "InstanceLockResponse: Player %s (guid %u) tried to bind himself/teleport to graveyard without a pending bind!", m_Player->GetName(), m_Player->GetGUIDLow());
        return;
    }

    if (accept)
        m_Player->BindToInstance();
    else
        m_Player->RepopAtGraveyard();

    m_Player->SetPendingBind(0, 0);
}

void WorldSession::HandleRequestHotfix(WorldPacket& p_RecvPacket)
{
    uint32 l_Type   = 0;
    uint32 l_Count  = 0;

    p_RecvPacket >> l_Type;
    l_Count = p_RecvPacket.ReadBits(13);

    uint64 * l_Guids = new uint64[l_Count];

    for (uint32 l_I = 0; l_I < l_Count; ++l_I)
    {
        uint32 l_Entry;

        p_RecvPacket.readPackGUID(l_Guids[l_I]);
        p_RecvPacket >> l_Entry;

        switch (l_Type)
        {
            case DB2_REPLY_ITEM:
                SendItemDb2Reply(l_Entry);
                break;
            case DB2_REPLY_SPARSE:
                SendItemSparseDb2Reply(l_Entry);
                break;
            case DB2_REPLY_BROADCAST_TEXT:
                SendBroadcastTextDb2Reply(l_Entry);
                break;
            case DB2_REPLY_MAP_CHALLENGE_MODE:
                SendMapChallengeModeDBReply(l_Entry);
                break;
            // TODO
            case DB2_REPLY_BATTLE_PET_EFFECT_PROPERTIES:
            case DB2_REPLY_SCENE_SCRIPT:
                break;
            default:
                break;
        }
    }

    delete[] l_Guids;
}

void WorldSession::HandleUpdateMissileTrajectory(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_UPDATE_MISSILE_TRAJECTORY");

    uint64 guid;
    uint32 spellId;
    float elevation, speed;
    float curX, curY, curZ;
    float targetX, targetY, targetZ;
    uint8 moveStop;

    recvPacket >> guid >> spellId >> elevation >> speed;
    recvPacket >> curX >> curY >> curZ;
    recvPacket >> targetX >> targetY >> targetZ;
    recvPacket >> moveStop;

    Unit* caster = ObjectAccessor::GetUnit(*m_Player, guid);
    Spell* spell = caster ? caster->GetCurrentSpell(CURRENT_GENERIC_SPELL) : NULL;
    if (!spell || spell->m_spellInfo->Id != spellId || !spell->m_targets.HasDst() || !spell->m_targets.HasSrc())
    {
        recvPacket.rfinish();
        return;
    }

    Position pos = *spell->m_targets.GetSrcPos();
    pos.Relocate(curX, curY, curZ);
    spell->m_targets.ModSrc(pos);

    pos = *spell->m_targets.GetDstPos();
    pos.Relocate(targetX, targetY, targetZ);
    spell->m_targets.ModDst(pos);

    spell->m_targets.SetElevation(elevation);
    spell->m_targets.SetSpeed(speed);

    if (moveStop)
    {
        uint32 opcode;
        recvPacket >> opcode;
        recvPacket.SetOpcode(CMSG_MOVE_STOP); // always set to MSG_MOVE_STOP in client SetOpcode
        HandleMovementOpcodes(recvPacket);
    }
}

void WorldSession::HandleViolenceLevel(WorldPacket& recvPacket)
{
    uint8 violenceLevel;
    recvPacket >> violenceLevel;

    // do something?
}

void WorldSession::HandleObjectUpdateFailedOpcode(WorldPacket & p_Packet)
{
    uint64 l_ObjectGUID = 0;

    p_Packet.readPackGUID(l_ObjectGUID);

    WorldObject * l_WorldObject = ObjectAccessor::GetWorldObject(*GetPlayer(), l_ObjectGUID);
     
    if (l_WorldObject)
        l_WorldObject->SendUpdateToPlayer(GetPlayer());

    sLog->outError(LOG_FILTER_NETWORKIO, "Object update failed for object " UI64FMTD " (%s) for player %s (%u)", uint64(l_ObjectGUID), l_WorldObject ? l_WorldObject->GetName() : "object-not-found", GetPlayerName().c_str(), GetGuidLow());
}

// DestrinyFrame.xml : lua function NeutralPlayerSelectFaction
#define JOIN_THE_ALLIANCE 1
#define JOIN_THE_HORDE    0

void WorldSession::HandleSetFactionOpcode(WorldPacket& recvPacket)
{
    uint32 choice = recvPacket.read<uint32>();

    if (m_Player->getRace() != RACE_PANDAREN_NEUTRAL)
        return;

    if (choice == JOIN_THE_HORDE)
    {
        m_Player->SetByteValue(UNIT_FIELD_SEX, 0, RACE_PANDAREN_HORDE);
        m_Player->setFactionForRace(RACE_PANDAREN_HORDE);
        m_Player->SaveToDB();
        WorldLocation location(1, 1366.730f, -4371.248f, 26.070f, 3.1266f);
        m_Player->TeleportTo(location);
        m_Player->SetHomebind(location, 363);
        m_Player->learnSpell(669, false); // Language Orcish
        m_Player->learnSpell(108127, false); // Language Pandaren
    }
    else if (choice == JOIN_THE_ALLIANCE)
    {
        m_Player->SetByteValue(UNIT_FIELD_SEX, 0, RACE_PANDAREN_ALLI);
        m_Player->setFactionForRace(RACE_PANDAREN_ALLI);
        m_Player->SaveToDB();
        WorldLocation location(0, -9096.236f, 411.380f, 92.257f, 3.649f);
        m_Player->TeleportTo(location);
        m_Player->SetHomebind(location, 9);
        m_Player->learnSpell(668, false); // Language Common
        m_Player->learnSpell(108127, false); // Language Pandaren
    }

    if (m_Player->GetQuestStatus(31450) == QUEST_STATUS_INCOMPLETE)
        m_Player->KilledMonsterCredit(64594);

    m_Player->SendMovieStart(116);
}

void WorldSession::HandleCategoryCooldownOpcode(WorldPacket& recvPacket)
{
    Unit::AuraEffectList const& list = GetPlayer()->GetAuraEffectsByType(SPELL_AURA_MOD_SPELL_CATEGORY_COOLDOWN);

    WorldPacket data(SMSG_CATEGORY_COOLDOWN, 4 + (int(list.size()) * 8));
    data.WriteBits<int>(list.size(), 21);
    for (Unit::AuraEffectList::const_iterator itr = list.begin(); itr != list.end(); ++itr)
    {
        AuraEffectPtr effect = *itr;
        if (!effect)
            continue;

        data << uint32(effect->GetMiscValue());
        data << int32(-effect->GetAmount());
    }

    SendPacket(&data);
}

void WorldSession::HandleSaveCUFProfiles(WorldPacket& p_RecvPacket)
{
    uint32 l_ProfileCount;

    p_RecvPacket >> l_ProfileCount;

    if (l_ProfileCount > MAX_CUF_PROFILES)
    {
        p_RecvPacket.rfinish();
        return;
    }

    CUFProfiles& l_Profiles = GetPlayer()->m_cufProfiles;
    l_Profiles.resize(l_ProfileCount);

    for (uint32 l_I = 0; l_I < l_ProfileCount; ++l_I)
    {
        CUFProfile& l_Profile = l_Profiles[l_I];
        CUFProfileData& l_ProfileData = l_Profile.data;

        l_Profile.l_NameLen = p_RecvPacket.ReadBits(7);

        if (l_Profile.l_NameLen > MAX_CUF_PROFILE_NAME_LENGTH)
        {
            p_RecvPacket.rfinish();
            return;
        }

        l_ProfileData.KeepGroupsTogether            = p_RecvPacket.ReadBit();
        l_ProfileData.DisplayPets                   = p_RecvPacket.ReadBit();
        l_ProfileData.DisplayMainTankAndAssist      = p_RecvPacket.ReadBit();
        l_ProfileData.DisplayHealPrediction         = p_RecvPacket.ReadBit();
        l_ProfileData.DisplayAggroHighlight         = p_RecvPacket.ReadBit();
        l_ProfileData.DisplayOnlyDispellableDebuffs = p_RecvPacket.ReadBit();
        l_ProfileData.DisplayPowerBar               = p_RecvPacket.ReadBit();
        l_ProfileData.DisplayBorder                 = p_RecvPacket.ReadBit();
        l_ProfileData.UseClassColors                = p_RecvPacket.ReadBit();
        l_ProfileData.HorizontalGroups              = p_RecvPacket.ReadBit();
        l_ProfileData.DisplayNonBossDebuffs         = p_RecvPacket.ReadBit();
        l_ProfileData.DynamicPosition               = p_RecvPacket.ReadBit();
        l_ProfileData.Locked                        = p_RecvPacket.ReadBit();
        l_ProfileData.Shown                         = p_RecvPacket.ReadBit();
        l_ProfileData.AutoActivate2Players          = p_RecvPacket.ReadBit();
        l_ProfileData.AutoActivate3Players          = p_RecvPacket.ReadBit();
        l_ProfileData.AutoActivate5Players          = p_RecvPacket.ReadBit();
        l_ProfileData.AutoActivate10Players         = p_RecvPacket.ReadBit();
        l_ProfileData.AutoActivate15Players         = p_RecvPacket.ReadBit();
        l_ProfileData.AutoActivate25Players         = p_RecvPacket.ReadBit();
        l_ProfileData.AutoActivate40Players         = p_RecvPacket.ReadBit();
        l_ProfileData.AutoActivateSpec1             = p_RecvPacket.ReadBit();
        l_ProfileData.AutoActivateSpec2             = p_RecvPacket.ReadBit();
        l_ProfileData.AutoActivatePvP               = p_RecvPacket.ReadBit();
        l_ProfileData.AutoActivatePvE               = p_RecvPacket.ReadBit();

        p_RecvPacket >> l_ProfileData.FrameHeight;
        p_RecvPacket >> l_ProfileData.FrameWidth;
        p_RecvPacket >> l_ProfileData.SortBy;
        p_RecvPacket >> l_ProfileData.HealthText;
        p_RecvPacket >> l_ProfileData.TopPoint;
        p_RecvPacket >> l_ProfileData.BottomPoint;
        p_RecvPacket >> l_ProfileData.LeftPoint;
        p_RecvPacket >> l_ProfileData.TopOffset;
        p_RecvPacket >> l_ProfileData.BottomOffset;
        p_RecvPacket >> l_ProfileData.LeftOffset;

        l_Profile.Name = p_RecvPacket.ReadString(l_Profile.l_NameLen);
    }

    m_Player->SendCUFProfiles();

    SQLTransaction l_Transaction = CharacterDatabase.BeginTransaction();

    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CUF_PROFILE);
    l_Stmt->setUInt32(0, GetPlayer()->GetGUIDLow());
    l_Transaction->Append(l_Stmt);

    for (uint32 l_I = 0; l_I < l_ProfileCount; ++l_I)
    {
        CUFProfile& profile = l_Profiles[l_I];
        CUFProfileData data = profile.data;

        l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CUF_PROFILE);
        l_Stmt->setUInt32(0, GetPlayer()->GetGUIDLow());
        l_Stmt->setString(1, profile.Name);
        l_Stmt->setString(2, PackDBBinary(&data, sizeof(CUFProfileData)));
        l_Transaction->Append(l_Stmt);
    }

    CharacterDatabase.CommitTransaction(l_Transaction);
}
