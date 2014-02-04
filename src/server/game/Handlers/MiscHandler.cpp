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

    bool unk = recvData.ReadBit();

    if (GetPlayer()->isAlive() || GetPlayer()->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
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
    ObjectGuid guid;
    uint32 codeLen = 0;
    std::string code = "";

    recvData >> gossipListId >> textId;

    guid[7] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    codeLen = recvData.ReadBits(8);
    guid[5] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();

    recvData.FlushBits();

    recvData.ReadByteSeq(guid[1]);
    recvData.ReadByteSeq(guid[0]);
    recvData.ReadByteSeq(guid[6]);
    recvData.ReadByteSeq(guid[3]);
    recvData.ReadByteSeq(guid[7]);
    recvData.ReadByteSeq(guid[5]);
    recvData.ReadByteSeq(guid[2]);
    code = recvData.ReadString(codeLen);
    recvData.ReadByteSeq(guid[4]);

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
        go = _player->GetMap()->GetGameObject(guid);
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
        _player->PlayerTalkClass->SendCloseGossip();
        return;
    }

    uint32 menuId = _player->PlayerTalkClass->GetGossipMenu().GetMenuId();

    if (!code.empty())
    {
        if (unit)
        {
            unit->AI()->sGossipSelectCode(_player, menuId, gossipListId, code.c_str());
            if (!sScriptMgr->OnGossipSelectCode(_player, unit, _player->PlayerTalkClass->GetGossipOptionSender(gossipListId), _player->PlayerTalkClass->GetGossipOptionAction(gossipListId), code.c_str()))
                _player->OnGossipSelect(unit, gossipListId, menuId);
        }
        else
        {
            go->AI()->GossipSelectCode(_player, menuId, gossipListId, code.c_str());
            sScriptMgr->OnGossipSelectCode(_player, go, _player->PlayerTalkClass->GetGossipOptionSender(gossipListId), _player->PlayerTalkClass->GetGossipOptionAction(gossipListId), code.c_str());
        }
    }
    else
    {
        if (unit)
        {
            unit->AI()->sGossipSelect(_player, menuId, gossipListId);
            if (!sScriptMgr->OnGossipSelect(_player, unit, _player->PlayerTalkClass->GetGossipOptionSender(gossipListId), _player->PlayerTalkClass->GetGossipOptionAction(gossipListId)))
                _player->OnGossipSelect(unit, gossipListId, menuId);
        }
        else
        {
            go->AI()->GossipSelect(_player, menuId, gossipListId);
            if (!sScriptMgr->OnGossipSelect(_player, go, _player->PlayerTalkClass->GetGossipOptionSender(gossipListId), _player->PlayerTalkClass->GetGossipOptionAction(gossipListId)))
                _player->OnGossipSelect(go, gossipListId, menuId);
        }
    }
}

void WorldSession::HandleWhoOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_WHO Message");

    time_t now = time(NULL);
    if (now - timeLastWhoCommand < 5)
        return;
    else timeLastWhoCommand = now;

    uint32 matchcount = 0;

    uint32 level_min, level_max, racemask, classmask, zones_count, str_count;
    uint32 zoneids[10];                                     // 10 is client limit

    bool unk1, unk2, bit725, bit740;
    uint8 playerLen = 0, guildLen = 0;
    uint8 unkLen2, unkLen3;
    std::string player_name, guild_name;

    recvData >> classmask;                                  // class mask
    recvData >> level_max;                                  // minimal player level, default 100 (MAX_LEVEL)
    recvData >> level_min;                                  // maximal player level, default 0
    recvData >> racemask;                                   // race mask

    guildLen = recvData.ReadBits(7);
    playerLen = recvData.ReadBits(6);
    str_count = recvData.ReadBits(3);

    if (str_count > 4)                                      // can't be received from real client or broken packet
        return;

    unk1 = recvData.ReadBit();
    unk2 = recvData.ReadBit();
    zones_count = recvData.ReadBits(4);                     // zones count, client limit = 10 (2.0.10)

    if (zones_count > 10)                                   // can't be received from real client or broken packet
        return;

    unkLen2 = recvData.ReadBits(8) << 1;
    unkLen2 += recvData.ReadBit();
    unkLen3 = recvData.ReadBits(8) << 1;
    unkLen3 += recvData.ReadBit();
    bit725 = recvData.ReadBit();

    uint8* unkLens = new uint8[str_count];
    std::string* unkStrings = new std::string[str_count];

    for (uint8 i = 0; i < str_count; i++)
        unkLens[i] = recvData.ReadBits(7);

    bit740 = recvData.ReadBit();
    recvData.FlushBits();

    if (playerLen > 0)
        player_name = recvData.ReadString(playerLen);       // player name, case sensitive...

    std::wstring str[4];                                    // 4 is client limit
    for (uint32 i = 0; i < str_count; ++i)
    {
        std::string temp = recvData.ReadString(unkLens[i]); // user entered string, it used as universal search pattern(guild+player name)?
        if (!Utf8toWStr(temp, str[i]))
            continue;

        wstrToLower(str[i]);
        sLog->outDebug(LOG_FILTER_NETWORKIO, "String %u: %s", i, temp.c_str());
    }

    if (guildLen > 0)
        guild_name = recvData.ReadString(guildLen); // guild name, case sensitive ...

    for (uint32 i = 0; i < zones_count; ++i)
    {
        uint32 temp;
        recvData >> temp;                                   // zone id, 0 if zone is unknown...
        zoneids[i] = temp;
        sLog->outDebug(LOG_FILTER_NETWORKIO, "Zone %u: %u", i, zoneids[i]);
    }

    if (unkLen3 > 0)
        std::string unkString = recvData.ReadString(unkLen3);

    if (unkLen2 > 0)
        std::string unkString = recvData.ReadString(unkLen2);

    if (bit740)
    {
        uint32 unk1, unk2, unk3;
        recvData >> unk1 >> unk2 >> unk3;
    }

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Minlvl %u, maxlvl %u, name %s, guild %s, racemask %u, classmask %u, zones %u, strings %u", level_min, level_max, player_name.c_str(), guild_name.c_str(), racemask, classmask, zones_count, str_count);

    std::wstring wplayer_name;
    std::wstring wguild_name;
    if (!(Utf8toWStr(player_name, wplayer_name) && Utf8toWStr(guild_name, wguild_name)))
        return;
    wstrToLower(wplayer_name);
    wstrToLower(wguild_name);

    // client send in case not set max level value 100 but Trinity supports 255 max level,
    // update it to show GMs with characters after 100 level
    if (level_max >= MAX_LEVEL)
        level_max = STRONG_MAX_LEVEL;

    uint32 team = _player->GetTeam();
    uint32 security = GetSecurity();
    bool allowTwoSideWhoList = sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_WHO_LIST);
    uint32 gmLevelInWhoList  = sWorld->getIntConfig(CONFIG_GM_LEVEL_IN_WHO_LIST);
    uint8 displaycount = 0;

    ByteBuffer bitsData;
    ByteBuffer bytesData;
    WorldPacket data(SMSG_WHO);
    size_t pos = data.wpos();

    bitsData.WriteBits(displaycount, 6);

    TRINITY_READ_GUARD(HashMapHolder<Player>::LockType, *HashMapHolder<Player>::GetLock());
    HashMapHolder<Player>::MapType const& m = sObjectAccessor->GetPlayers();
    for (HashMapHolder<Player>::MapType::const_iterator itr = m.begin(); itr != m.end(); ++itr)
    {
        if (AccountMgr::IsPlayerAccount(security))
        {
            // player can see member of other team only if CONFIG_ALLOW_TWO_SIDE_WHO_LIST
            if (itr->second->GetTeam() != team && !allowTwoSideWhoList)
                continue;

            // player can see MODERATOR, GAME MASTER, ADMINISTRATOR only if CONFIG_GM_IN_WHO_LIST
            if ((itr->second->GetSession()->GetSecurity() > AccountTypes(gmLevelInWhoList)))
                continue;
        }

        //do not process players which are not in world
        if (!(itr->second->IsInWorld()))
            continue;

        // check if target is globally visible for player
        if (!(itr->second->IsVisibleGloballyFor(_player)))
            continue;

        // check if target's level is in level range
        uint8 lvl = itr->second->getLevel();
        if (lvl < level_min || lvl > level_max)
            continue;

        // check if class matches classmask
        uint32 class_ = itr->second->getClass();
        if (!(classmask & (1 << class_)))
            continue;

        // check if race matches racemask
        uint32 race = itr->second->getRace();
        if (!(racemask & (1 << race)))
            continue;

        uint32 pzoneid = itr->second->GetZoneId();
        uint8 gender = itr->second->getGender();

        bool z_show = true;
        for (uint32 i = 0; i < zones_count; ++i)
        {
            if (zoneids[i] == pzoneid)
            {
                z_show = true;
                break;
            }

            z_show = false;
        }
        if (!z_show)
            continue;

        std::string pname = itr->second->GetName();
        std::wstring wpname;
        if (!Utf8toWStr(pname, wpname))
            continue;
        wstrToLower(wpname);

        if (!(wplayer_name.empty() || wpname.find(wplayer_name) != std::wstring::npos))
            continue;

        std::string gname = sGuildMgr->GetGuildNameById(itr->second->GetGuildId());
        std::wstring wgname;
        if (!Utf8toWStr(gname, wgname))
            continue;
        wstrToLower(wgname);

        if (!(wguild_name.empty() || wgname.find(wguild_name) != std::wstring::npos))
            continue;

        std::string aname;
        if (AreaTableEntry const* areaEntry = GetAreaEntryByAreaID(itr->second->GetZoneId()))
            aname = areaEntry->area_name[GetSessionDbcLocale()];

        bool s_show = true;
        for (uint32 i = 0; i < str_count; ++i)
        {
            if (!str[i].empty())
            {
                if (wgname.find(str[i]) != std::wstring::npos ||
                    wpname.find(str[i]) != std::wstring::npos ||
                    Utf8FitTo(aname, str[i]))
                {
                    s_show = true;
                    break;
                }
                s_show = false;
            }
        }
        if (!s_show)
            continue;

        // 49 is maximum player count sent to client - can be overridden
        // through config, but is unstable
        if ((matchcount++) >= sWorld->getIntConfig(CONFIG_MAX_WHO))
        {
            if (sWorld->getBoolConfig(CONFIG_LIMIT_WHO_ONLINE))
                break;
            else
                continue;

            break;
        }

        ObjectGuid playerGuid = itr->second->GetGUID();
        ObjectGuid unkGuid = NULL;
        ObjectGuid guildGuid = itr->second->GetGuild() ? itr->second->GetGuild()->GetGUID() : NULL;

        bitsData.WriteBit(guildGuid[4]);

        if (DeclinedName const* names = itr->second->GetDeclinedNames())
        {
            for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
                bitsData.WriteBits(names->name[i].size(), 7);
        }
        else
        {
            for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
                bitsData.WriteBits(0, 7);
        }

        bitsData.WriteBits(gname.size(), 7);
        bitsData.WriteBit(unkGuid[3]);
        bitsData.WriteBit(playerGuid[1]);
        bitsData.WriteBits(pname.size(), 6);
        bitsData.WriteBit(guildGuid[2]);
        bitsData.WriteBit(playerGuid[7]);
        bitsData.WriteBit(unkGuid[2]);
        bitsData.WriteBit(guildGuid[6]);
        bitsData.WriteBit(guildGuid[7]);
        bitsData.WriteBit(unkGuid[7]);
        bitsData.WriteBit(playerGuid[3]);
        bitsData.WriteBit(unkGuid[4]);
        bitsData.WriteBit(0);
        bitsData.WriteBit(unkGuid[1]);
        bitsData.WriteBit(unkGuid[0]);
        bitsData.WriteBit(playerGuid[4]);
        bitsData.WriteBit(0);
        bitsData.WriteBit(guildGuid[1]);
        bitsData.WriteBit(guildGuid[3]);
        bitsData.WriteBit(unkGuid[6]);
        bitsData.WriteBit(guildGuid[0]);
        bitsData.WriteBit(playerGuid[2]);
        bitsData.WriteBit(playerGuid[5]);
        bitsData.WriteBit(playerGuid[6]);
        bitsData.WriteBit(guildGuid[5]);
        bitsData.WriteBit(unkGuid[5]);
        bitsData.WriteBit(playerGuid[0]);

        bytesData.WriteByteSeq(playerGuid[7]);
        bytesData.WriteByteSeq(guildGuid[4]);
        bytesData.WriteByteSeq(guildGuid[1]);
        bytesData << uint32(38297239);
        bytesData.WriteByteSeq(playerGuid[0]);

        if (pname.size() > 0)
            bytesData.append(pname.c_str(), pname.size());

        bytesData.WriteByteSeq(unkGuid[0]);
        bytesData.WriteByteSeq(guildGuid[6]);
        bytesData << uint32(pzoneid);
        bytesData.WriteByteSeq(unkGuid[7]);
        bytesData.WriteByteSeq(unkGuid[5]);
        bytesData.WriteByteSeq(playerGuid[3]);
        bytesData.WriteByteSeq(playerGuid[6]);
        bytesData << uint32(realmID);
        bytesData << uint8(race);
        bytesData << uint8(lvl);
        bytesData.WriteByteSeq(playerGuid[2]);
        bytesData.WriteByteSeq(playerGuid[1]);
        bytesData << uint8(gender);
        bytesData << uint32(realmID);
        bytesData.WriteByteSeq(guildGuid[0]);
        bytesData.WriteByteSeq(guildGuid[5]);
        bytesData.WriteByteSeq(guildGuid[7]);

        if (DeclinedName const* names = itr->second->GetDeclinedNames())
            for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
                if (names->name[i].size() > 0)
                    bytesData.append(names->name[i].c_str(), names->name[i].size());

        bytesData.WriteByteSeq(playerGuid[5]);
        bytesData.WriteByteSeq(guildGuid[3]);
        bytesData.WriteByteSeq(playerGuid[4]);

        if (gname.size() > 0)
            bytesData.append(gname.c_str(), gname.size());

        bytesData << uint8(class_);
        bytesData.WriteByteSeq(unkGuid[4]);
        bytesData.WriteByteSeq(guildGuid[2]);
        bytesData.WriteByteSeq(unkGuid[3]);
        bytesData.WriteByteSeq(unkGuid[6]);
        bytesData.WriteByteSeq(unkGuid[2]);
        bytesData.WriteByteSeq(unkGuid[1]);

        ++displaycount;
    }

    if (displaycount != 0)
    {
        bitsData.FlushBits();
        uint8 firstByte = bitsData.contents()[0];
        data << uint8(displaycount << 2 | firstByte & 0x3);
        for (size_t i = 1; i < bitsData.size(); i++)
            data << uint8(bitsData.contents()[i]);

        data.append(bytesData);
    }
    else
        data.WriteBits(0, 6);

    SendPacket(&data);

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
    else if (GetPlayer()->duel || GetPlayer()->HasAura(9454)) // is dueling or frozen by GM via freeze command
        reason = 2;                                         // FIXME - Need the correct value

    if (reason)
    {
        WorldPacket data(SMSG_LOGOUT_RESPONSE, 1+4);
        data << uint32(reason);
        data.WriteBit(0);
        SendPacket(&data);
        LogoutRequest(0);
        return;
    }

    // Instant logout in taverns/cities or on taxi or for admins, gm's, mod's if its enabled in worldserver.conf
    if (GetPlayer()->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_RESTING) || GetPlayer()->isInFlight() ||
        GetSecurity() >= AccountTypes(sWorld->getIntConfig(CONFIG_INSTANT_LOGOUT)) || IsPremium())
    {
        WorldPacket data(SMSG_LOGOUT_RESPONSE, 1+4);
        data << uint32(reason);
        data.WriteBit(1);           // instant logout
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

        GetPlayer()->ApplyModFlag(PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP, newPvPStatus);
        GetPlayer()->ApplyModFlag(PLAYER_FLAGS, PLAYER_FLAGS_PVP_TIMER, !newPvPStatus);
    }
    else
    {
        GetPlayer()->ToggleFlag(PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP);
        GetPlayer()->ToggleFlag(PLAYER_FLAGS, PLAYER_FLAGS_PVP_TIMER);
    }

    if (GetPlayer()->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP))
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
    if (GetPlayer()->isAlive() || !GetPlayer()->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
        return;
    //TODO: unk32, unk32
    GetPlayer()->RepopAtGraveyard();
}

void WorldSession::HandleSetSelectionOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;

    uint8 bitsOrder[8] = { 1, 3, 4, 6, 0, 5, 7, 2 };
    recvData.ReadBitInOrder(guid, bitsOrder); 

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 7, 6, 0, 2, 3, 1, 4, 5 };
    recvData.ReadBytesSeq(guid, bytesOrder);

    _player->SetSelection(guid);
}

void WorldSession::HandleStandStateChangeOpcode(WorldPacket& recvData)
{
    // sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_STANDSTATECHANGE"); -- too many spam in log at lags/debug stop
    uint32 animstate;
    recvData >> animstate;

    _player->SetStandState(animstate);
}

void WorldSession::HandleContactListOpcode(WorldPacket& recvData)
{
    recvData.read_skip<uint32>(); // always 1
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_CONTACT_LIST");
    _player->GetSocial()->SendSocialList(_player);
}

void WorldSession::HandleAddFriendOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_ADD_FRIEND");

    std::string friendName = GetTrinityString(LANG_FRIEND_IGNORE_UNKNOWN);
    std::string friendNote;

    recvData >> friendName;

    recvData >> friendNote;

    if (!normalizePlayerName(friendName))
        return;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: %s asked to add friend : '%s'", GetPlayer()->GetName(), friendName.c_str());

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_RACE_ACC_BY_NAME);

    stmt->setString(0, friendName);

    _addFriendCallback.SetParam(friendNote);
    _addFriendCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
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

        if (!AccountMgr::IsPlayerAccount(GetSecurity()) || sWorld->getBoolConfig(CONFIG_ALLOW_GM_FRIEND) || AccountMgr::IsPlayerAccount(AccountMgr::GetSecurity(friendAccountId, realmID)))
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
    uint64 FriendGUID;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_DEL_FRIEND");

    recvData >> FriendGUID;

    _player->GetSocial()->RemoveFromSocialList(GUID_LOPART(FriendGUID), false);

    sSocialMgr->SendFriendStatus(GetPlayer(), FRIEND_REMOVED, GUID_LOPART(FriendGUID), false);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent motd (SMSG_FRIEND_STATUS)");
}

void WorldSession::HandleAddIgnoreOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_ADD_IGNORE");

    std::string ignoreName = GetTrinityString(LANG_FRIEND_IGNORE_UNKNOWN);

    recvData >> ignoreName;

    if (!normalizePlayerName(ignoreName))
        return;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: %s asked to Ignore: '%s'", GetPlayer()->GetName(), ignoreName.c_str());

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GUID_BY_NAME);

    stmt->setString(0, ignoreName);

    _addIgnoreCallback = CharacterDatabase.AsyncQuery(stmt);
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
    uint64 IgnoreGUID;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_DEL_IGNORE");

    recvData >> IgnoreGUID;

    _player->GetSocial()->RemoveFromSocialList(GUID_LOPART(IgnoreGUID), true);

    sSocialMgr->SendFriendStatus(GetPlayer(), FRIEND_IGNORE_REMOVED, GUID_LOPART(IgnoreGUID), false);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent motd (SMSG_FRIEND_STATUS)");
}

void WorldSession::HandleSetContactNotesOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_SET_CONTACT_NOTES");
    uint64 guid;
    std::string note;
    recvData >> guid >> note;
    _player->GetSocial()->SetFriendNote(GUID_LOPART(guid), note);
}

void WorldSession::HandleReportBugOpcode(WorldPacket& recvData)
{
    float posX, posY, posZ, orientation;
    uint32 contentlen, mapId;
    std::string content;

    recvData >> posX >> posY >> orientation >> posZ;
    recvData >> mapId;

    contentlen = recvData.ReadBits(10);
    recvData.FlushBits();
    content = recvData.ReadString(contentlen);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "%s", content.c_str());

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_BUG_REPORT);
    stmt->setString(0, "Bug");
    stmt->setString(1, content);
    CharacterDatabase.Execute(stmt);
}

void WorldSession::HandleReportSuggestionOpcode(WorldPacket& recvData)
{
    float posX, posY, posZ, orientation;
    uint32 contentlen, mapId;
    std::string content;

    recvData >> mapId;
    recvData >> posZ >> orientation >> posY >> posX;

    contentlen = recvData.ReadBits(10);
    recvData.FlushBits();
    content = recvData.ReadString(contentlen);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "%s", content.c_str());

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_BUG_REPORT);
    stmt->setString(0, "Suggestion");
    stmt->setString(1, content);
    CharacterDatabase.Execute(stmt);
}

void WorldSession::HandleRequestBattlePetJournal(WorldPacket& /*recvPacket*/)
{
    WorldPacket data;
    GetPlayer()->GetBattlePetMgr().BuildBattlePetJournal(&data);
    SendPacket(&data);
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

void WorldSession::HandleReclaimCorpseOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_RECLAIM_CORPSE");

    ObjectGuid guid;

    uint8 bitsOrder[8] = { 0, 1, 2, 6, 5, 7, 3, 4 };
    recvData.ReadBitInOrder(guid, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 4, 1, 6, 7, 2, 3, 5, 0 };
    recvData.ReadBytesSeq(guid, bytesOrder);

    if (GetPlayer()->isAlive())
        return;

    // do not allow corpse reclaim in arena
    if (GetPlayer()->InArena())
        return;

    // body not released yet
    if (!GetPlayer()->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
        return;

    Corpse* corpse = GetPlayer()->GetCorpse();

    if (!corpse)
        return;

    // prevent resurrect before 30-sec delay after body release not finished
    if (time_t(corpse->GetGhostTime() + GetPlayer()->GetCorpseReclaimDelay(corpse->GetType() == CORPSE_RESURRECTABLE_PVP)) > time_t(time(NULL)))
        return;

    if (!corpse->IsWithinDistInMap(GetPlayer(), CORPSE_RECLAIM_RADIUS, true))
        return;

    // resurrect
    GetPlayer()->ResurrectPlayer(GetPlayer()->InBattleground() ? 1.0f : 0.5f);

    // spawn bones
    GetPlayer()->SpawnCorpseBones();
}

void WorldSession::HandleResurrectResponseOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_RESURRECT_RESPONSE");

    uint32 status;
    recvData >> status;

    ObjectGuid guid;

    uint8 bitsOrder[8] = { 3, 4, 1, 5, 2, 0, 7, 6 };
    recvData.ReadBitInOrder(guid, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 0, 6, 4, 5, 3, 1, 2, 7 };
    recvData.ReadBytesSeq(guid, bytesOrder);

    if (GetPlayer()->isAlive())
        return;

    if (status == 1)
    {
        GetPlayer()->ClearResurrectRequestData();           // reject
        return;
    }

    if (!GetPlayer()->IsRessurectRequestedBy(guid))
        return;

    GetPlayer()->ResurectUsingRequestData();
}

void WorldSession::SendAreaTriggerMessage(const char* Text, ...)
{
    va_list ap;
    char szStr [1024];
    szStr[0] = '\0';

    va_start(ap, Text);
    vsnprintf(szStr, 1024, Text, ap);
    va_end(ap);

    uint32 length = strlen(szStr)+1;
    WorldPacket data(SMSG_AREA_TRIGGER_MESSAGE, 4+length);
    data << length;
    data << szStr;
    SendPacket(&data);
}

void WorldSession::HandleAreaTriggerOpcode(WorldPacket& recvData)
{
    uint32 triggerId;
    uint8 unkbit1, unkbit2;

    recvData >> triggerId;
    unkbit1 = recvData.ReadBit();
    unkbit2 = recvData.ReadBit();

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

    if (player->GetMapId() != atEntry->mapid)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleAreaTriggerOpcode: Player '%s' (GUID: %u) too far (trigger map: %u player map: %u), ignore Area Trigger ID: %u",
            player->GetName(), atEntry->mapid, player->GetMapId(), player->GetGUIDLow(), triggerId);
        return;
    }

    // delta is safe radius
    const float delta = 5.0f;

    if (atEntry->radius > 0)
    {
        // if we have radius check it
        float dist = player->GetDistance(atEntry->x, atEntry->y, atEntry->z);
        if (dist > atEntry->radius + delta)
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleAreaTriggerOpcode: Player '%s' (GUID: %u) too far (radius: %f distance: %f), ignore Area Trigger ID: %u",
                player->GetName(), player->GetGUIDLow(), atEntry->radius, dist, triggerId);
            return;
        }
    }
    else
    {
        // we have only extent

        // rotate the players position instead of rotating the whole cube, that way we can make a simplified
        // is-in-cube check and we have to calculate only one point instead of 4

        // 2PI = 360°, keep in mind that ingame orientation is counter-clockwise
        double rotation = 2 * M_PI - atEntry->box_orientation;
        double sinVal = std::sin(rotation);
        double cosVal = std::cos(rotation);

        float playerBoxDistX = player->GetPositionX() - atEntry->x;
        float playerBoxDistY = player->GetPositionY() - atEntry->y;

        float rotPlayerX = float(atEntry->x + playerBoxDistX * cosVal - playerBoxDistY*sinVal);
        float rotPlayerY = float(atEntry->y + playerBoxDistY * cosVal + playerBoxDistX*sinVal);

        // box edges are parallel to coordiante axis, so we can treat every dimension independently :D
        float dz = player->GetPositionZ() - atEntry->z;
        float dx = rotPlayerX - atEntry->x;
        float dy = rotPlayerY - atEntry->y;
        if ((fabs(dx) > atEntry->box_x / 2 + delta) ||
            (fabs(dy) > atEntry->box_y / 2 + delta) ||
            (fabs(dz) > atEntry->box_z / 2 + delta))
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "HandleAreaTriggerOpcode: Player '%s' (GUID: %u) too far (1/2 box X: %f 1/2 box Y: %f 1/2 box Z: %f rotatedPlayerX: %f rotatedPlayerY: %f dZ:%f), ignore Area Trigger ID: %u",
                player->GetName(), player->GetGUIDLow(), atEntry->box_x/2, atEntry->box_y/2, atEntry->box_z/2, rotPlayerX, rotPlayerY, dz, triggerId);
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
        player->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_RESTING);
        player->InnEnter(time(NULL), atEntry->mapid, atEntry->x, atEntry->y, atEntry->z);
        player->SetRestType(REST_TYPE_IN_TAVERN);

        if (sWorld->IsFFAPvPRealm())
            player->RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP);

        return;
    }

    if (Battleground* bg = player->GetBattleground())
        if (bg->GetStatus() == STATUS_IN_PROGRESS)
        {
            bg->HandleAreaTrigger(player, triggerId);
            return;
        }

        if (OutdoorPvP* pvp = player->GetOutdoorPvP())
            if (pvp->HandleAreaTrigger(_player, triggerId))
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

void WorldSession::HandleUpdateAccountData(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_UPDATE_ACCOUNT_DATA");

    uint32 type, timestamp, decompressedSize, compressedSize;
    recvData >> decompressedSize >> timestamp >> compressedSize;

    type = uint8(recvData.contents()[recvData.size()-1]) >> 5;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "UAD: type %u, time %u, decompressedSize %u", type, timestamp, decompressedSize);

    if (decompressedSize == 0)                               // erase
    {
        SetAccountData(AccountDataType(type), 0, "");
        return;
    }

    if (decompressedSize > 0xFFFF)
    {
        recvData.rfinish();                   // unnneded warning spam in this case
        sLog->outError(LOG_FILTER_NETWORKIO, "UAD: Account data packet too big, size %u", decompressedSize);
        return;
    }

    ByteBuffer dest;
    dest.resize(decompressedSize);

    uLongf realSize = decompressedSize;
    if (uncompress(const_cast<uint8*>(dest.contents()), &realSize, const_cast<uint8*>(recvData.contents() + recvData.rpos()), recvData.size() - recvData.rpos()) != Z_OK)
    {
        recvData.rfinish();                   // unnneded warning spam in this case
        sLog->outError(LOG_FILTER_NETWORKIO, "UAD: Failed to decompress account data");
        return;
    }

    recvData.rfinish();                       // uncompress read (recvData.size() - recvData.rpos())

    std::string adata = dest.ReadString(decompressedSize);

    SetAccountData(AccountDataType(type), timestamp, adata);
}

void WorldSession::HandleRequestAccountData(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_REQUEST_ACCOUNT_DATA");

    uint32 type = recvData.ReadBits(3);
    recvData.FlushBits();

    sLog->outDebug(LOG_FILTER_NETWORKIO, "RAD: type %u", type);

    if (type > NUM_ACCOUNT_DATA_TYPES)
        return;

    AccountData* adata = GetAccountData(AccountDataType(type));

    uint32 size = adata->Data.size();

    uLongf destSize = compressBound(size);

    ByteBuffer dest;
    dest.resize(destSize);

    if (size && compress(const_cast<uint8*>(dest.contents()), &destSize, (uint8*)adata->Data.c_str(), size) != Z_OK)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "RAD: Failed to compress account data");
        return;
    }

    dest.resize(destSize);

    WorldPacket data(SMSG_UPDATE_ACCOUNT_DATA, 4+4+4+3+3+5+8+destSize);
    ObjectGuid playerGuid = _player ? _player->GetGUID() : 0;

    data << uint32(size);                                   // decompressed length
    data << uint32(adata->Time);                            // unix time
    data << uint32(destSize);                               // compressed length
    data.append(dest);                                      // compressed data

    data.WriteBit(playerGuid[4]);
    data.WriteBit(playerGuid[2]);
    data.WriteBit(playerGuid[0]);
    data.WriteBits(type, 3);
    data.WriteBit(playerGuid[7]);
    data.WriteBit(playerGuid[5]);
    data.WriteBit(playerGuid[1]);
    data.WriteBit(playerGuid[3]);
    data.WriteBit(playerGuid[6]);

    uint8 byteOrder[8] = { 4, 2, 7, 5, 3, 1, 6, 0 };
    data.WriteBytesSeq(playerGuid, byteOrder);

    
    SendPacket(&data);
}

int32 WorldSession::HandleEnableNagleAlgorithm()
{
    // Instructs the server we wish to receive few amounts of large packets (SMSG_MULTIPLE_PACKETS?)
    // instead of large amount of small packets
    return 0;
}

void WorldSession::HandleSetActionButtonOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_SET_ACTION_BUTTON");
    uint8 button;

    recvData >> button;

    ObjectGuid packetData;

    uint8 bitsOrder[8] = { 1, 7, 6, 5, 2, 4, 0, 3 };
    recvData.ReadBitInOrder(packetData, bitsOrder);

    uint8 bytesOrder[8] = { 2, 7, 1, 4, 0, 5, 3, 6 };
    recvData.ReadBytesSeq(packetData, bytesOrder);

    uint32 action = uint64(packetData) & 0xFFFFFFFF;
    uint8  type   = (uint64(packetData) & 0xFF00000000000000) >> 56;

    sLog->outInfo(LOG_FILTER_NETWORKIO, "BUTTON: %u ACTION: %u TYPE: %u", button, action, type);
    if (!packetData)
    {
        sLog->outInfo(LOG_FILTER_NETWORKIO, "MISC: Remove action from button %u", button);
        GetPlayer()->removeActionButton(button);
    }
    else
    {
        switch (type)
        {
            case ACTION_BUTTON_MACRO:
            case ACTION_BUTTON_CMACRO:
                sLog->outInfo(LOG_FILTER_NETWORKIO, "MISC: Added Macro %u into button %u", action, button);
                break;
            case ACTION_BUTTON_EQSET:
                sLog->outInfo(LOG_FILTER_NETWORKIO, "MISC: Added EquipmentSet %u into button %u", action, button);
                break;
            case ACTION_BUTTON_SPELL:
                sLog->outInfo(LOG_FILTER_NETWORKIO, "MISC: Added Spell %u into button %u", action, button);
                break;
            case ACTION_BUTTON_SUB_BUTTON:
                sLog->outInfo(LOG_FILTER_NETWORKIO, "MISC: Added sub buttons %u into button %u", action, button);
                break;
            case ACTION_BUTTON_ITEM:
                sLog->outInfo(LOG_FILTER_NETWORKIO, "MISC: Added Item %u into button %u", action, button);
                break;
            default:
                sLog->outError(LOG_FILTER_NETWORKIO, "MISC: Unknown action button type %u for action %u into button %u for player %s (GUID: %u)", type, action, button, _player->GetName(), _player->GetGUIDLow());
                return;
        }
        GetPlayer()->addActionButton(button, action, type);
    }
}

void WorldSession::HandleCompleteCinematic(WorldPacket& /*recvData*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_COMPLETE_CINEMATIC");
}

void WorldSession::HandleNextCinematicCamera(WorldPacket& /*recvData*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_NEXT_CINEMATIC_CAMERA");
}

void WorldSession::HandleMoveTimeSkippedOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_MOVE_TIME_SKIPPED");

    ObjectGuid guid;
    uint32 time;
    recvData >> time;

    uint8 bitOrder[8] = { 3, 0, 5, 1, 7, 6, 4, 2 };
    recvData.ReadBitInOrder(guid, bitOrder);

    recvData.FlushBits();

    uint8 byteOrder[8] = { 1, 6, 0, 5, 3, 4, 2, 7 };
    recvData.ReadBytesSeq(guid, byteOrder);

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

void WorldSession::HandleSetActionBarToggles(WorldPacket& recvData)
{
    uint8 actionBar;

    recvData >> actionBar;

    if (!GetPlayer())                                        // ignore until not logged (check needed because STATUS_AUTHED)
    {
        if (actionBar != 0)
            sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetActionBarToggles in not logged state with value: %u, ignored", uint32(actionBar));
        return;
    }

    GetPlayer()->SetByteValue(PLAYER_FIELD_LIFETIME_MAX_RANK, 2, actionBar);
}

void WorldSession::HandlePlayedTime(WorldPacket& recvData)
{
    bool unk1 = recvData.ReadBit();                 // 0 or 1 expected

    WorldPacket data(SMSG_PLAYED_TIME, 4 + 4 + 1);
    data << uint32(_player->GetLevelPlayedTime());
    data << uint32(_player->GetTotalPlayedTime());
    data.WriteBit(unk1);                            // 0 - will not show in chat frame
    data.FlushBits();
    SendPacket(&data);
}

void WorldSession::HandleInspectOpcode(WorldPacket& recvData)
{
    ObjectGuid playerGuid;

    uint8 bitOrder[8] = { 2, 5, 1, 6, 7, 4, 0, 3 };
    recvData.ReadBitInOrder(playerGuid, bitOrder);

    recvData.FlushBits();

    uint8 byteOrder[8] = { 0, 1, 5, 6, 4, 2, 3, 7 };
    recvData.ReadBytesSeq(playerGuid, byteOrder);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_INSPECT");

    _player->SetSelection(playerGuid);

    Player* player = ObjectAccessor::FindPlayer(playerGuid);
    if (!player)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_INSPECT: No player found from GUID: " UI64FMTD, uint64(playerGuid));
        return;
    }

    WorldPacket data(SMSG_INSPECT_TALENT);

    ByteBuffer talentData;
    ByteBuffer glyphData;

    uint32 talentCount = 0;
    uint32 glyphCount = 0;
    uint32 equipmentCount = 0;

    data.WriteBit(playerGuid[7]);
    data.WriteBit(playerGuid[3]);

    Guild* guild = sGuildMgr->GetGuildById(player->GetGuildId());
    data.WriteBit(guild != nullptr);

    if (guild != nullptr)
    {
        ObjectGuid guildGuid = guild->GetGUID();

        data.WriteBit(guildGuid[6]);
        data.WriteBit(guildGuid[7]);
        data.WriteBit(guildGuid[4]);
        data.WriteBit(guildGuid[5]);
        data.WriteBit(guildGuid[2]);
        data.WriteBit(guildGuid[3]);
        data.WriteBit(guildGuid[1]);
        data.WriteBit(guildGuid[0]);
    }

    for (auto itr : *player->GetTalentMap(player->GetActiveSpec()))
    {
        SpellInfo const* spell = sSpellMgr->GetSpellInfo(itr.first);
        if (spell && spell->talentId)
        {
            talentData << uint16(spell->talentId);
            ++talentCount;
        }
    }

    for (uint8 i = 0; i < MAX_GLYPH_SLOT_INDEX; ++i)
    {
        if (player->GetGlyph(0, i) == 0)
            continue;

        glyphData << uint16(player->GetGlyph(0, i));               // GlyphProperties.dbc
        ++glyphCount;
    }

    data.WriteBits(talentCount, 23);
    data.WriteBits(glyphCount, 23);
    data.WriteBit(playerGuid[5]);
    data.WriteBit(playerGuid[2]);
    data.WriteBit(playerGuid[6]);

    for (uint32 i = 0; i < EQUIPMENT_SLOT_END; ++i)
    {
        Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
        if (!item)
            continue;

        ++equipmentCount;
    }

    data.WriteBits(equipmentCount, 20);

    for (uint32 i = 0; i < EQUIPMENT_SLOT_END; ++i)
    {
        Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
        if (!item)
            continue;

        ObjectGuid itemCreator = item->GetUInt64Value(ITEM_FIELD_CREATOR);

        data.WriteBit(itemCreator[0]);
        data.WriteBit(0);               // unk bit 32
        data.WriteBit(itemCreator[6]);
        data.WriteBit(itemCreator[7]);
        data.WriteBit(0);               // unk bit 16
        data.WriteBit(itemCreator[3]);
        data.WriteBit(itemCreator[2]);
        data.WriteBit(itemCreator[1]);

        uint32 enchantmentCount = 0;

        for (uint32 j = 0; j < MAX_ENCHANTMENT_SLOT; ++j)
        {
            uint32 enchId = item->GetEnchantmentId(EnchantmentSlot(j));
            if (!enchId)
                continue;

            ++enchantmentCount;
        }

        data.WriteBits(enchantmentCount, 21);
        data.WriteBit(0);               // unk bit
        data.WriteBit(itemCreator[5]);
        data.WriteBit(itemCreator[4]);
    }
    
    data.WriteBit(playerGuid[4]);
    data.WriteBit(playerGuid[1]);
    data.WriteBit(playerGuid[0]);

    for (uint32 i = 0; i < EQUIPMENT_SLOT_END; ++i)
    {
        Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);
        if (!item)
            continue;

        ObjectGuid itemCreator = item->GetUInt64Value(ITEM_FIELD_CREATOR);

        // related to random stats
        // if (unkBit)
        //     data << uint16(UNK);

        data.WriteByteSeq(itemCreator[3]);
        data.WriteByteSeq(itemCreator[4]);

        for (uint32 j = 0; j < MAX_ENCHANTMENT_SLOT; ++j)
        {
            uint32 enchId = item->GetEnchantmentId(EnchantmentSlot(j));
            if (!enchId)
                continue;

            data << uint8(j);
            data << uint32(enchId);
        }

        // if (unkBit)
        //     data << uint32(UNK);

        data.WriteByteSeq(itemCreator[0]);
        data.WriteByteSeq(itemCreator[2]);
        data.WriteByteSeq(itemCreator[6]);

        uint32 mask = 0;
        uint32 modifiers = 0;

        if (item->GetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 0))
        {
            ++modifiers;
            mask |= 1;
        }

        if (item->GetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 1))
        {
            ++modifiers;
            mask |= 2;
        }

        if (item->GetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 2))
        {
            ++modifiers;
            mask |= 4;
        }

        data << uint32(modifiers == 0 ? 0 : ((modifiers * 4) + 4));

        if (modifiers > 0)
        {
            data << uint32(mask);
            if (uint32 reforge = item->GetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 0))
                data << uint32(reforge);
            if (uint32 transmogrification = item->GetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 1))
                data << uint32(transmogrification);
            if (uint32 upgrade = item->GetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 2))
                data << uint32(upgrade);
        }

        data.WriteByteSeq(itemCreator[1]);
        data.WriteByteSeq(itemCreator[7]);
        data.WriteByteSeq(itemCreator[5]);
        data << uint8(i);
        data << uint32(item->GetEntry());
    }

    data.WriteByteSeq(playerGuid[7]);
    data.WriteByteSeq(playerGuid[1]);
    data.WriteByteSeq(playerGuid[5]);
    data.WriteByteSeq(playerGuid[0]);

    data << uint32(player->GetSpecializationId(player->GetActiveSpec()));

    if (guild != nullptr)
    {
        ObjectGuid guildGuid = guild->GetGUID();

        data << uint32(guild->GetLevel());

        data.WriteByteSeq(guildGuid[1]);
        data.WriteByteSeq(guildGuid[3]);

        data << uint32(guild->GetMembersCount());

        data.WriteByteSeq(guildGuid[6]);
        data.WriteByteSeq(guildGuid[2]);
        data.WriteByteSeq(guildGuid[5]);
        data.WriteByteSeq(guildGuid[4]);
        data.WriteByteSeq(guildGuid[0]);
        data.WriteByteSeq(guildGuid[7]);

        data << uint64(guild->GetExperience());
    }
    
    data.WriteByteSeq(playerGuid[6]);
    data.WriteByteSeq(playerGuid[4]);
    data.WriteByteSeq(playerGuid[2]);
    data.WriteByteSeq(playerGuid[3]);

    data.append(talentData);
    data.append(glyphData);

    SendPacket(&data);
}

void WorldSession::HandleInspectHonorStatsOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;
    uint8 bitOrder[8] = { 0, 3, 2, 4, 6, 7, 5, 1 };
    recvData.ReadBitInOrder(guid, bitOrder);

    recvData.FlushBits();

    uint8 byteOrder[8] = { 5, 0, 2, 7, 6, 3, 1, 4 };
    recvData.ReadBytesSeq(guid, byteOrder);

    Player* player = ObjectAccessor::FindPlayer(guid);
    if (!player)
    {
        //sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_INSPECT_HONOR_STATS: No player found from GUID: " UI64FMTD, guid);
        return;
    }

    ObjectGuid playerGuid = guid;
    WorldPacket data(SMSG_INSPECT_HONOR_STATS);

    uint8 bitOrder2[8] = { 5, 3, 7, 2, 1, 6, 0, 4 };
    data.WriteBitInOrder(playerGuid, bitOrder2);
    
    const uint32 cycleCount = 3; // MAX_ARENA_SLOT
    data.WriteBits(cycleCount, 3);
    
    data.WriteByteSeq(playerGuid[7]);

    for (int i = 0; i < cycleCount; ++i)
    {
        // Client display only this two fields

        data << uint32(player->GetSeasonWins(i));
        data << uint32(0);
        data << uint32(0);
        
        data << uint8(i);
        
        data << uint32(0);
        data << uint32(0);
        data << uint32(player->GetArenaPersonalRating(i));
        data << uint32(0);
    }

    data.WriteByteSeq(playerGuid[1]);
    data.WriteByteSeq(playerGuid[5]);
    data.WriteByteSeq(playerGuid[0]);
    data.WriteByteSeq(playerGuid[3]);
    data.WriteByteSeq(playerGuid[2]);
    data.WriteByteSeq(playerGuid[6]);
    data.WriteByteSeq(playerGuid[4]);

    SendPacket(&data);
}

void WorldSession::HandleInspectRatedBGStatsOpcode(WorldPacket& recvData)
{
    uint32 unk;
    ObjectGuid guid;

    recvData >> unk;

    uint8 bitOrder[8] = { 1, 3, 5, 2, 6, 7, 0, 4 };
    recvData.ReadBitInOrder(guid, bitOrder);

    recvData.FlushBits();

    uint8 byteOrder[8] = { 4, 7, 0, 3, 5, 2, 6, 1 };
    recvData.ReadBytesSeq(guid, byteOrder);

    Player* player = ObjectAccessor::FindPlayer(guid);
    if (!player)
    {
        //sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_REQUEST_INSPECT_RATED_BG_STATS: No player found from GUID: " UI64FMTD, guid);
        return;
    }

    return;

    // TODO //
    ObjectGuid playerGuid = player->GetGUID();
    WorldPacket data(SMSG_INSPECT_RATED_BG_STATS);

    ObjectGuid gguid = guid;
    data << uint32(0); //SeasonWin
    data << uint32(0); //SeasonPlayed
    data << uint32(0); //Rating
    
    uint8 bitOrder3[8] = {5, 7, 2, 3, 4, 6, 0, 1};
    data.WriteBitInOrder(gguid, bitOrder3);
    
    uint8 byteOrder2[8] = {6, 2, 3, 1, 7, 5, 4, 0};
    data.WriteBytesSeq(gguid, byteOrder2);

    SendPacket(&data);
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

    WorldPacket data(SMSG_WHOIS, msg.size()+1);
    data.WriteBits(msg.size(), 11);

    data.FlushBits();
    if (msg.size())
        data.append(msg.c_str(), msg.size());

    SendPacket(&data);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Received whois command from player %s for character %s", GetPlayer()->GetName(), charname.c_str());
}

void WorldSession::HandleComplainOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_COMPLAIN");

    // recvData is not empty, but all data are unused in core
    // NOTE: all chat messages from this spammer automatically ignored by spam reporter until logout in case chat spam.
    // if it's mail spam - ALL mails from this spammer automatically removed by client

    // Complaint Received message
    WorldPacket data(SMSG_COMPLAIN_RESULT, 2);
    data << uint8(0);   // value 1 resets CGChat::m_complaintsSystemStatus in client. (unused?)
    data << uint32(0);  // value 0xC generates a "CalendarError" in client.
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

void WorldSession::HandleRealmQueryNameOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_REALM_QUERY_NAME");

    uint32 realmId = recvData.read<uint32>();

    if (realmId != realmID)
        return; // Cheater ?

    std::string realmName = sWorld->GetRealmName();

    WorldPacket data(SMSG_REALM_QUERY_RESPONSE);
    // 0 : OK, 1 : Error, 2 : Retry, 3 : Show '?'
    data << uint8(0); // ok, realmId exist server-side
    data << realmID;
    data.WriteBits(realmName.size(), 8);
    data.WriteBit(1); // unk, if it's main realm ?
    data.WriteBits(realmName.size(), 8);
    data.FlushBits();
    data.append(realmName.c_str(), realmName.size());
    data.append(realmName.c_str(), realmName.size());

    SendPacket(&data);
}

void WorldSession::HandleFarSightOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_FAR_SIGHT");

    bool apply = recvData.ReadBit();

    if (apply)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "Player %u set vision to self", _player->GetGUIDLow());
        _player->SetSeer(_player);
    }
    else
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "Added FarSight " UI64FMTD " to player %u", _player->GetUInt64Value(PLAYER_FARSIGHT), _player->GetGUIDLow());
        if (WorldObject* target = _player->GetViewpoint())
            _player->SetSeer(target);
        else
            sLog->outError(LOG_FILTER_NETWORKIO, "Player %s requests non-existing seer " UI64FMTD, _player->GetName(), _player->GetUInt64Value(PLAYER_FARSIGHT));
    }

    GetPlayer()->UpdateVisibilityForPlayer();
}

void WorldSession::HandleSetTitleOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_SET_TITLE");

    uint32 title;
    recvData >> title;

    // -1 at none
    if (title > 0 && title < MAX_TITLE_INDEX)
    {
        if (!GetPlayer()->HasTitle(title))
            return;
    }
    else
        title = 0;

    GetPlayer()->SetUInt32Value(PLAYER_CHOSEN_TITLE, title);
}

void WorldSession::HandleTimeSyncResp(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_TIME_SYNC_RESP");

    uint32 counter, clientTicks;
    recvData >> clientTicks >> counter;

    if (counter != _player->m_timeSyncCounter - 1)
        sLog->outDebug(LOG_FILTER_NETWORKIO, "Wrong time sync counter from player %s (cheater?)", _player->GetName());

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Time sync received: counter %u, client ticks %u, time since last sync %u", counter, clientTicks, clientTicks - _player->m_timeSyncClient);

    uint32 ourTicks = clientTicks + (getMSTime() - _player->m_timeSyncServer);

    // diff should be small
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Our ticks: %u, diff %u, latency %u", ourTicks, ourTicks - clientTicks, GetLatency());

    _player->m_timeSyncClient = clientTicks;
}

void WorldSession::HandleResetInstancesOpcode(WorldPacket& /*recvData*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_RESET_INSTANCES");

    if (Group* group = _player->GetGroup())
    {
        if (group->IsLeader(_player->GetGUID()))
            group->ResetInstances(INSTANCE_RESET_ALL, false, _player);
    }
    else
        _player->ResetInstances(INSTANCE_RESET_ALL, false);
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
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetDungeonDifficultyOpcode: player %d sent an invalid instance mode %d!", _player->GetGUIDLow(), mode);
        return;
    }

    if (Difficulty(mode) == _player->GetDungeonDifficulty())
        return;

    // cannot reset while in an instance
    Map* map = _player->FindMap();
    if (map && map->IsDungeon())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetDungeonDifficultyOpcode: player (Name: %s, GUID: %u) tried to reset the instance while player is inside!", _player->GetName(), _player->GetGUIDLow());
        return;
    }

    Group* group = _player->GetGroup();
    if (group)
    {
        if (group->IsLeader(_player->GetGUID()))
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
                    sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetDungeonDifficultyOpcode: player %d tried to reset the instance while group member (Name: %s, GUID: %u) is inside!", _player->GetGUIDLow(), groupGuy->GetName(), groupGuy->GetGUIDLow());
                    return;
                }
            }
            // the difficulty is set even if the instances can't be reset
            //_player->SendDungeonDifficulty(true);
            group->ResetInstances(INSTANCE_RESET_CHANGE_DIFFICULTY, false, _player);
            group->SetDungeonDifficulty(Difficulty(mode));
            _player->SendDungeonDifficulty(true);
        }
    }
    else
    {
        _player->ResetInstances(INSTANCE_RESET_CHANGE_DIFFICULTY, false);
        _player->SetDungeonDifficulty(Difficulty(mode));
        _player->SendDungeonDifficulty(false);
    }
}

void WorldSession::HandleSetRaidDifficultyOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "MSG_SET_RAID_DIFFICULTY");

    uint32 mode;
    recvData >> mode;

    if (mode >= MAX_RAID_DIFFICULTY)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetRaidDifficultyOpcode: player %d sent an invalid instance mode %d!", _player->GetGUIDLow(), mode);
        return;
    }

    // cannot reset while in an instance
    Map* map = _player->FindMap();
    if (map && map->IsDungeon())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetRaidDifficultyOpcode: player %d tried to reset the instance while inside!", _player->GetGUIDLow());
        return;
    }

    if (Difficulty(mode) == _player->GetRaidDifficulty())
        return;

    Group* group = _player->GetGroup();
    if (group)
    {
        if (group->IsLeader(_player->GetGUID()))
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
                    sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleSetRaidDifficultyOpcode: player %d tried to reset the instance while inside!", _player->GetGUIDLow());
                    return;
                }
            }
            // the difficulty is set even if the instances can't be reset
            //_player->SendDungeonDifficulty(true);
            group->ResetInstances(INSTANCE_RESET_CHANGE_DIFFICULTY, true, _player);
            group->SetRaidDifficulty(Difficulty(mode));
            _player->SendRaidDifficulty(true);
        }
    }
    else
    {
        _player->ResetInstances(INSTANCE_RESET_CHANGE_DIFFICULTY, true);
        _player->SetRaidDifficulty(Difficulty(mode));
        _player->SendRaidDifficulty(false);
    }
}

void WorldSession::HandleCancelMountAuraOpcode(WorldPacket& /*recvData*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_CANCEL_MOUNT_AURA");

    //If player is not mounted, so go out :)
    if (!_player->IsMounted())                              // not blizz like; no any messages on blizz
    {
        ChatHandler(this).SendSysMessage(LANG_CHAR_NON_MOUNTED);
        return;
    }

    if (_player->isInFlight())                               // not blizz like; no any messages on blizz
    {
        ChatHandler(this).SendSysMessage(LANG_YOU_IN_FLIGHT);
        return;
    }

    _player->Dismount();
    _player->RemoveAurasByType(SPELL_AURA_MOUNTED);
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
    uint8 mode;
    recvData >> mode;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Client used \"/timetest %d\" command", mode);
}

void WorldSession::HandleQueryInspectAchievements(WorldPacket& recvData)
{
    ObjectGuid guid;

    uint8 bitsOrder[8] = { 7, 4, 0, 5, 2, 1, 3, 6 };
    recvData.ReadBitInOrder(guid, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 3, 2, 4, 6, 1, 7, 5, 0 };
    recvData.ReadBytesSeq(guid, bytesOrder);

    Player* player = ObjectAccessor::FindPlayer(guid);
    if (!player)
        return;

    player->GetAchievementMgr().SendAchievementInfo(_player);
}

void WorldSession::HandleGuildAchievementProgressQuery(WorldPacket& recvData)
{
    uint32 achievementId;
    recvData >> achievementId;

    if (Guild* guild = sGuildMgr->GetGuildById(_player->GetGuildId()))
        guild->GetAchievementMgr().SendAchievementInfo(_player, achievementId);
}

void WorldSession::HandleWorldStateUITimerUpdate(WorldPacket& /*recvData*/)
{
    // empty opcode
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_WORLD_STATE_UI_TIMER_UPDATE");

    WorldPacket data(SMSG_WORLD_STATE_UI_TIMER_UPDATE, 4);
    data << uint32(time(NULL));
    SendPacket(&data);
}

void WorldSession::HandleReadyForAccountDataTimes(WorldPacket& /*recvData*/)
{
    // empty opcode
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_READY_FOR_ACCOUNT_DATA_TIMES");

    SendAccountDataTimes(GLOBAL_CACHE_MASK);
}

void WorldSession::SendSetPhaseShift(std::set<uint32> const& phaseIds, std::set<uint32> const& terrainswaps)
{
    ObjectGuid guid = _player->GetGUID();
    uint32 unkValue = 0;

    WorldPacket data(SMSG_SET_PHASE_SHIFT, 1 + 8 + 4 + 4 + 4 + 4 + 2 * phaseIds.size() + 4 + terrainswaps.size() * 2);

    data << uint32(phaseIds.size()) * 2;        // Phase.dbc ids
    for (std::set<uint32>::const_iterator itr = phaseIds.begin(); itr != phaseIds.end(); ++itr)
        data << uint16(*itr); // Most of phase id on retail sniff have 0x8000 mask

    // 0x8 or 0x10 is related to areatrigger, if we send flags 0x00 areatrigger doesn't work in some case
    data << uint32(0x18); // flags, 0x18 most of time on retail sniff

    data << uint32(0);                          // Inactive terrain swaps, may switch with active terrain
    //for (uint8 i = 0; i < inactiveSwapsCount; ++i)
    //    data << uint16(0);

    data << uint32(terrainswaps.size()) * 2;    // Active terrain swaps, may switch with inactive terrain
    for (std::set<uint32>::const_iterator itr = terrainswaps.begin(); itr != terrainswaps.end(); ++itr)
        data << uint16(*itr);

    data << uint32(unkValue);
    //for (uint32 i = 0; i < unkValue; i++)
    //    data << uint16(0); // WorldMapAreaId ?
    
    uint8 bitOrder[8] = {3, 7, 1, 6, 0, 4, 5, 2};
    data.WriteBitInOrder(guid, bitOrder);
    
    uint8 byteOrder[8] = {4, 3, 0, 6, 2, 7, 5, 1};
    data.WriteBytesSeq(guid, byteOrder);

    SendPacket(&data);
}

// Battlefield and Battleground
void WorldSession::HandleAreaSpiritHealerQueryOpcode(WorldPacket& recv_data)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_AREA_SPIRIT_HEALER_QUERY");

    Battleground* bg = _player->GetBattleground();

    ObjectGuid guid;

    uint8 bitsOrder[8] = { 5, 3, 0, 6, 1, 7, 4, 2 };
    recv_data.ReadBitInOrder(guid, bitsOrder);

    recv_data.FlushBits();

    uint8 bytesOrder[8] = { 6, 7, 3, 0, 5, 4, 2, 1 };
    recv_data.ReadBytesSeq(guid, bytesOrder);

    Creature* unit = GetPlayer()->GetMap()->GetCreature(guid);
    if (!unit)
        return;

    if (!unit->isSpiritService())                            // it's not spirit service
        return;

    if (bg)
        sBattlegroundMgr->SendAreaSpiritHealerQueryOpcode(_player, bg, guid);

    if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(_player->GetZoneId()))
        bf->SendAreaSpiritHealerQueryOpcode(_player,guid);
}

void WorldSession::HandleAreaSpiritHealerQueueOpcode(WorldPacket& recv_data)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_AREA_SPIRIT_HEALER_QUEUE");

    Battleground* bg = _player->GetBattleground();
    ObjectGuid npcGuid;

    uint8 bitsOrder[8] = { 6, 2, 0, 3, 5, 4, 1, 7 };
    recv_data.ReadBitInOrder(npcGuid, bitsOrder);

    recv_data.FlushBits();

    uint8 bytesOrder[8] = { 4, 2, 6, 0, 1, 7, 3, 5 };
    recv_data.ReadBytesSeq(npcGuid, bytesOrder);

    Creature* unit = GetPlayer()->GetMap()->GetCreature(npcGuid);
    if (!unit)
        return;

    if (!unit->isSpiritService())                            // it's not spirit service
        return;

    if (bg)
        bg->AddPlayerToResurrectQueue(npcGuid, _player->GetGUID());

    if (Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(_player->GetZoneId()))
        bf->AddPlayerToResurrectQueue(npcGuid, _player->GetGUID());
}

void WorldSession::HandleHearthAndResurrect(WorldPacket& /*recvData*/)
{
    if (_player->isInFlight())
        return;

    if (/*Battlefield* bf =*/ sBattlefieldMgr->GetBattlefieldToZoneId(_player->GetZoneId()))
    {
        // bf->PlayerAskToLeave(_player);                   //@todo: FIXME
        return;
    }

    AreaTableEntry const* atEntry = GetAreaEntryByAreaID(_player->GetAreaId());
    if (!atEntry || !(atEntry->flags & AREA_FLAG_WINTERGRASP_2))
        return;

    _player->BuildPlayerRepop();
    _player->ResurrectPlayer(100);
    _player->TeleportTo(_player->m_homebindMapId, _player->m_homebindX, _player->m_homebindY, _player->m_homebindZ, _player->GetOrientation());
}

void WorldSession::HandleInstanceLockResponse(WorldPacket& recvPacket)
{
    uint8 accept;
    recvPacket >> accept;

    if (!_player->HasPendingBind())
    {
        sLog->outInfo(LOG_FILTER_NETWORKIO, "InstanceLockResponse: Player %s (guid %u) tried to bind himself/teleport to graveyard without a pending bind!", _player->GetName(), _player->GetGUIDLow());
        return;
    }

    if (accept)
        _player->BindToInstance();
    else
        _player->RepopAtGraveyard();

    _player->SetPendingBind(0, 0);
}

void WorldSession::HandleRequestHotfix(WorldPacket& recvPacket)
{
    uint32 type, count;
    recvPacket >> type;

    count = recvPacket.ReadBits(21);

    ObjectGuid* guids = new ObjectGuid[count];

    uint8 order[8] = { 3, 4, 7, 2, 5, 1, 6, 0 };
    for (uint32 i = 0; i < count; ++i)
        recvPacket.ReadBitInOrder(guids[i], order);

    uint32 entry;
    recvPacket.FlushBits();
    for (uint32 i = 0; i < count; ++i)
    {
        recvPacket.ReadByteSeq(guids[i][6]);
        recvPacket.ReadByteSeq(guids[i][1]);
        recvPacket.ReadByteSeq(guids[i][2]);
        recvPacket >> entry;
        recvPacket.ReadByteSeq(guids[i][4]);
        recvPacket.ReadByteSeq(guids[i][5]);
        recvPacket.ReadByteSeq(guids[i][7]);
        recvPacket.ReadByteSeq(guids[i][0]);
        recvPacket.ReadByteSeq(guids[i][3]);

        switch (type)
        {
            case DB2_REPLY_ITEM:
                SendItemDb2Reply(entry);
                break;
            case DB2_REPLY_SPARSE:
                SendItemSparseDb2Reply(entry);
                break;
            // TODO
            case DB2_REPLY_BATTLE_PET_EFFECT_PROPERTIES:
            case DB2_REPLY_SCENE_SCRIPT:
                break;
            case DB2_REPLY_BROADCAST_TEXT:
                printf("DB2_REPLY_BROADCAST_TEXT : %u\n", entry);
                SendBroadcastTextDb2Reply(entry);
                break;
            default:
                sLog->outError(LOG_FILTER_NETWORKIO, "CMSG_REQUEST_HOTFIX: Received unknown hotfix type: %u", type);
                delete[] guids;
                return;
        }
    }

    delete[] guids;
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

    Unit* caster = ObjectAccessor::GetUnit(*_player, guid);
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
        recvPacket.SetOpcode(MSG_MOVE_STOP); // always set to MSG_MOVE_STOP in client SetOpcode
        HandleMovementOpcodes(recvPacket);
    }
}

void WorldSession::HandleViolenceLevel(WorldPacket& recvPacket)
{
    uint8 violenceLevel;
    recvPacket >> violenceLevel;

    // do something?
}

void WorldSession::HandleObjectUpdateFailedOpcode(WorldPacket& recvPacket)
{
    ObjectGuid guid;

    uint8 bitOrder[8] = {2, 1, 0, 6, 3, 7, 4, 5};
    recvPacket.ReadBitInOrder(guid, bitOrder);

    uint8 byteOrder[8] = {7, 5, 1, 6, 4, 2, 3, 0};
    recvPacket.ReadBytesSeq(guid, byteOrder);

    WorldObject* obj = ObjectAccessor::GetWorldObject(*GetPlayer(), guid);
    if (obj)
        obj->SendUpdateToPlayer(GetPlayer());

    sLog->outError(LOG_FILTER_NETWORKIO, "Object update failed for object " UI64FMTD " (%s) for player %s (%u)", uint64(guid), obj ? obj->GetName() : "object-not-found", GetPlayerName().c_str(), GetGuidLow());
}

// DestrinyFrame.xml : lua function NeutralPlayerSelectFaction
#define JOIN_THE_ALLIANCE 1
#define JOIN_THE_HORDE    0

void WorldSession::HandleSetFactionOpcode(WorldPacket& recvPacket)
{
    uint32 choice = recvPacket.read<uint32>();

    if (_player->getRace() != RACE_PANDAREN_NEUTRAL)
        return;

    if (choice == JOIN_THE_HORDE)
    {
        _player->SetByteValue(UNIT_FIELD_BYTES_0, 0, RACE_PANDAREN_HORDE);
        _player->setFactionForRace(RACE_PANDAREN_HORDE);
        _player->SaveToDB();
        WorldLocation location(1, -1379.194f, -4369.913f, 26.0255f, 0.1223f);
        _player->TeleportTo(location);
        _player->SetHomebind(location, 363);
        _player->learnSpell(669, false); // Language Orcish
        _player->learnSpell(108127, false); // Language Pandaren
    }
    else if (choice == JOIN_THE_ALLIANCE)
    {
        _player->SetByteValue(UNIT_FIELD_BYTES_0, 0, RACE_PANDAREN_ALLI);
        _player->setFactionForRace(RACE_PANDAREN_ALLI);
        _player->SaveToDB();
        WorldLocation location(0, -9064.97f, -432.403f, 93.055f, 0.6296f);
        _player->TeleportTo(location);
        _player->SetHomebind(location, 9);
        _player->learnSpell(668, false); // Language Common
        _player->learnSpell(108127, false); // Language Pandaren
    }

    if (_player->GetQuestStatus(31450) == QUEST_STATUS_INCOMPLETE)
        _player->KilledMonsterCredit(64594);

    _player->SendMovieStart(116);
}

void WorldSession::HandleCategoryCooldownOpcode(WorldPacket& recvPacket)
{
    Unit::AuraEffectList const& list = GetPlayer()->GetAuraEffectsByType(SPELL_AURA_MOD_SPELL_CATEGORY_COOLDOWN);

    WorldPacket data(SMSG_SPELL_CATEGORY_COOLDOWN, 4 + (int(list.size()) * 8));
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

void WorldSession::HandleTradeInfo(WorldPacket& recvPacket)
{
    uint32 skillId = recvPacket.read<uint32>();
    uint32 spellId = recvPacket.read<uint32>();

    ObjectGuid guid;

    uint8 bitOrder[8] = { 5, 4, 7, 1, 3, 6, 0, 2 };
    recvPacket.ReadBitInOrder(guid, bitOrder);

    uint8 byteOrder[8] = { 7, 3, 4, 6, 1, 5, 0, 2 };
    recvPacket.ReadBytesSeq(guid, byteOrder);

    Player* plr = sObjectAccessor->FindPlayer(guid);
    if (!plr || !plr->HasSkill(skillId) || !plr->HasSpell(spellId))
        return;

    uint32 spellSkillCount = 0;
    for (auto itr : plr->GetSpellMap())
    {
        SpellInfo const* spell = sSpellMgr->GetSpellInfo(itr.first);
        if (!spell)
            continue;

        if (spell->IsAbilityOfSkillType(skillId) && spell->Effects[0].Effect != SPELL_EFFECT_TRADE_SKILL)
            spellSkillCount++;
    }

    WorldPacket data(SMSG_TRADE_INFO);
    data.WriteBit(guid[2]);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[7]);
    data.WriteBits(spellSkillCount, 22);
    data.WriteBit(guid[5]);
    data.WriteBit(guid[1]);
    data.WriteBit(guid[4]);
    data.WriteBits(1, 22);              // skill value count
    data.WriteBits(1, 22);              // skill id count
    data.WriteBits(1, 22);              // skill max value
    data.WriteBit(guid[3]);
    data.WriteBit(guid[0]);
    data << uint32(plr->GetSkillValue(skillId));
    data.WriteByteSeq(guid[0]);
    data << uint32(skillId);
    data.WriteByteSeq(guid[1]);
    data << uint32(spellId);

    for (auto itr : plr->GetSpellMap())
    {
        SpellInfo const* spell = sSpellMgr->GetSpellInfo(itr.first);
        if (!spell)
            continue;

        if (spell->IsAbilityOfSkillType(skillId) && spell->Effects[0].Effect != SPELL_EFFECT_TRADE_SKILL)
            data << uint32(spell->Id);
    }

    data.WriteByteSeq(guid[3]);
    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid[6]);
    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[2]);
    data << uint32(plr->GetMaxSkillValue(skillId));
    SendPacket(&data);
}

void WorldSession::HandleSaveCUFProfiles(WorldPacket& recvPacket)
{
    uint32 count = recvPacket.ReadBits(19);
    if (count > MAX_CUF_PROFILES)
    {
        recvPacket.rfinish();
        return;
    }

    CUFProfiles& profiles = GetPlayer()->m_cufProfiles;
    profiles.resize(count);
    for (uint32 i = 0; i < count; ++i)
    {
        CUFProfile& profile = profiles[i];
        CUFProfileData& data = profile.data;

        data.displayMainTankAndAssistant = recvPacket.ReadBit();
        data.keepGroupsTogether = recvPacket.ReadBit();
        data.auto5 = recvPacket.ReadBit();
        data.auto15 = recvPacket.ReadBit();
        data.displayPowerBar = recvPacket.ReadBit();
        data.displayBorder = recvPacket.ReadBit();
        data.autoPvP = recvPacket.ReadBit();
        data.auto40 = recvPacket.ReadBit();
        data.autoSpec1 = recvPacket.ReadBit();
        data.auto3 = recvPacket.ReadBit();
        data.auto2 = recvPacket.ReadBit();
        data.useClassColors = recvPacket.ReadBit();
        data.bit13 = recvPacket.ReadBit();
        data.autoSpec2 = recvPacket.ReadBit();
        data.horizontalGroups = recvPacket.ReadBit();
        data.bit16 = recvPacket.ReadBit();
        data.displayOnlyDispellableDebuffs = recvPacket.ReadBit();

        profile.nameLen = recvPacket.ReadBits(7);
        if (profile.nameLen > MAX_CUF_PROFILE_NAME_LENGTH)
        {
            recvPacket.rfinish();
            return;
        }

        data.displayNonBossDebuffs = recvPacket.ReadBit();
        data.displayPets = recvPacket.ReadBit();
        data.auto25 = recvPacket.ReadBit();
        data.displayHealPrediction = recvPacket.ReadBit();
        data.displayAggroHighlight = recvPacket.ReadBit();
        data.auto10 = recvPacket.ReadBit();
        data.bit24 = recvPacket.ReadBit();
        data.autoPvE = recvPacket.ReadBit();
    }

    recvPacket.FlushBits();

    for (uint32 i = 0; i < count; ++i)
    {
        CUFProfile& profile = profiles[i];
        CUFProfileData& data = profile.data;

        data.unk5 = recvPacket.read<uint8>();
        data.unk6 = recvPacket.read<uint8>();
        data.unk0 = recvPacket.read<uint16>();
        data.unk7 = recvPacket.read<uint8>();
        data.unk1 = recvPacket.read<uint16>();
        data.sortType = recvPacket.read<uint8>();
        data.frameWidth = recvPacket.read<uint16>();
        data.healthText = recvPacket.read<uint8>();
        data.frameHeight = recvPacket.read<uint16>();

        profile.name = recvPacket.ReadString(profile.nameLen);

        data.unk4 = recvPacket.read<uint16>();
    }

    _player->SendCUFProfiles();

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CUF_PROFILE);
    stmt->setUInt32(0, GetPlayer()->GetGUIDLow());
    trans->Append(stmt);

    for (uint32 i = 0; i < count; ++i)
    {
        CUFProfile& profile = profiles[i];
        CUFProfileData& data = profile.data;

        auto sdata = std::string((const char*)&data);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_CUF_PROFILE);
        stmt->setUInt32(0, GetPlayer()->GetGUIDLow());
        stmt->setString(1, profile.name);
        stmt->setString(2, sdata);
        trans->Append(stmt);
    }

    CharacterDatabase.CommitTransaction(trans);
}