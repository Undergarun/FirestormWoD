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
#include "WorldPacket.h"
#include "WorldSession.h"
#include "World.h"
#include "ObjectMgr.h"
#include "GuildMgr.h"
#include "Log.h"
#include "Opcodes.h"
#include "Guild.h"
#include "Arena.h"
#include "GossipDef.h"
#include "SocialMgr.h"

#define CHARTER_DISPLAY_ID 16161

/*enum PetitionType // dbc data
{
    PETITION_TYPE_GUILD      = 1,
    PETITION_TYPE_ARENA_TEAM = 3
};*/

// Charters ID in item_template
enum CharterItemIDs
{
    GUILD_CHARTER                                 = 5863,
    ARENA_TEAM_CHARTER_2v2                        = 23560,
    ARENA_TEAM_CHARTER_3v3                        = 23561,
    ARENA_TEAM_CHARTER_5v5                        = 23562
};

enum CharterCosts
{
    GUILD_CHARTER_COST                            = 1000,
    ARENA_TEAM_CHARTER_2v2_COST                   = 800000,
    ARENA_TEAM_CHARTER_3v3_COST                   = 1200000,
    ARENA_TEAM_CHARTER_5v5_COST                   = 2000000
};

void WorldSession::HandlePetitionBuyOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Received opcode CMSG_PETITION_BUY");
    ObjectGuid npcGuid;
    uint32 nameLen = 0;
    std::string name;

    npcGuid[3] = recvData.ReadBit();
    nameLen = recvData.ReadBits(7);
    npcGuid[4] = recvData.ReadBit();
    npcGuid[7] = recvData.ReadBit();
    npcGuid[1] = recvData.ReadBit();
    npcGuid[5] = recvData.ReadBit();
    npcGuid[2] = recvData.ReadBit();
    npcGuid[6] = recvData.ReadBit();
    npcGuid[0] = recvData.ReadBit();

    recvData.FlushBits();

    recvData.ReadByteSeq(npcGuid[6]);
    name = recvData.ReadString(nameLen);
    recvData.ReadByteSeq(npcGuid[1]);
    recvData.ReadByteSeq(npcGuid[7]);
    recvData.ReadByteSeq(npcGuid[0]);
    recvData.ReadByteSeq(npcGuid[5]);
    recvData.ReadByteSeq(npcGuid[3]);
    recvData.ReadByteSeq(npcGuid[2]);
    recvData.ReadByteSeq(npcGuid[4]);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Petitioner with GUID %u tried sell petition: name %s", GUID_LOPART(npcGuid), name.c_str());

    // prevent cheating
    Creature* creature = GetPlayer()->GetNPCIfCanInteractWith(npcGuid, UNIT_NPC_FLAG_PETITIONER);
    if (!creature)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandlePetitionBuyOpcode - Unit (GUID: %u) not found or you can't interact with him.", GUID_LOPART(npcGuid));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    uint32 charterid = 0;
    uint32 cost = 0;
    uint32 type = 0;
    if (creature->isTabardDesigner())
    {
        // if tabard designer, then trying to buy a guild charter.
        // do not let if already in guild.
        if (m_Player->GetGuildId())
            return;

        charterid = GUILD_CHARTER;
        cost = GUILD_CHARTER_COST;
        type = GUILD_CHARTER_TYPE;
    }

    if (type == GUILD_CHARTER_TYPE)
    {
        if (sGuildMgr->GetGuildByName(name))
        {
            Guild::SendCommandResult(this, GUILD_CREATE_S, ERR_GUILD_NAME_EXISTS_S, name);
            return;
        }
        if (sObjectMgr->IsReservedName(name) || !ObjectMgr::IsValidCharterName(name))
        {
            Guild::SendCommandResult(this, GUILD_CREATE_S, ERR_GUILD_NAME_INVALID, name);
            return;
        }
    }

    ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(charterid);
    if (!pProto)
    {
        m_Player->SendBuyError(BUY_ERR_CANT_FIND_ITEM, NULL, charterid, 0);
        return;
    }

    if (!m_Player->HasEnoughMoney(uint64(cost)))
    {                                                       //player hasn't got enough money
        m_Player->SendBuyError(BUY_ERR_NOT_ENOUGHT_MONEY, creature, charterid, 0);
        return;
    }

    ItemPosCountVec dest;
    InventoryResult msg = m_Player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, charterid, pProto->BuyCount);
    if (msg != EQUIP_ERR_OK)
    {
        m_Player->SendEquipError(msg, NULL, NULL, charterid);
        return;
    }

    m_Player->ModifyMoney(-(int32)cost);
    Item* charter = m_Player->StoreNewItem(dest, charterid, true);
    if (!charter)
        return;

    charter->SetUInt32Value(ITEM_FIELD_ENCHANTMENT, charter->GetGUIDLow());
    // ITEM_FIELD_ENCHANTMENT_1_1 is guild/arenateam id
    // ITEM_FIELD_ENCHANTMENT_1_1+1 is current signatures count (showed on item)
    charter->SetState(ITEM_CHANGED, m_Player);
    m_Player->SendNewItem(charter, 1, true, false);

    // a petition is invalid, if both the owner and the type matches
    // we checked above, if this player is in an arenateam, so this must be
    // datacorruption
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_BY_OWNER);
    stmt->setUInt32(0, m_Player->GetGUIDLow());
    stmt->setUInt8(1, type);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    std::ostringstream ssInvalidPetitionGUIDs;

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            ssInvalidPetitionGUIDs << '\'' << fields[0].GetUInt32() << "', ";
        }
        while (result->NextRow());
    }

    // delete petitions with the same guid as this one
    ssInvalidPetitionGUIDs << '\'' << charter->GetGUIDLow() << '\'';

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Invalid petition GUIDs: %s", ssInvalidPetitionGUIDs.str().c_str());
    CharacterDatabase.EscapeString(name);
    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    trans->PAppend("DELETE FROM petition WHERE petitionguid IN (%s)",  ssInvalidPetitionGUIDs.str().c_str());
    trans->PAppend("DELETE FROM petition_sign WHERE petitionguid IN (%s)", ssInvalidPetitionGUIDs.str().c_str());

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_PETITION);
    stmt->setUInt32(0, m_Player->GetGUIDLow());
    stmt->setUInt32(1, charter->GetGUIDLow());
    stmt->setString(2, name);
    stmt->setUInt8(3, uint8(type));
    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);
}

void WorldSession::HandlePetitionShowSignOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Received opcode CMSG_PETITION_SHOW_SIGNATURES");

    uint8 signs = 0;
    ObjectGuid petitionguid;

    uint8 bitsOrder[8] = { 4, 2, 5, 1, 0, 7, 6, 3 };
    recvData.ReadBitInOrder(petitionguid, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 7, 5, 3, 0, 6, 1, 4, 2 };
    recvData.ReadBytesSeq(petitionguid, bytesOrder);

    // solve (possible) some strange compile problems with explicit use GUID_LOPART(petitionguid) at some GCC versions (wrong code optimization in compiler?)
    uint32 petitionGuidLow = GUID_LOPART(petitionguid);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_TYPE);

    stmt->setUInt32(0, petitionGuidLow);

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
    {
        sLog->outDebug(LOG_FILTER_PLAYER_ITEMS, "Petition %u is not found for player %u %s", GUID_LOPART(petitionguid), GetPlayer()->GetGUIDLow(), GetPlayer()->GetName());
        return;
    }
    Field* fields = result->Fetch();
    uint32 type = fields[0].GetUInt8();

    // if guild petition and has guild => error, return;
    if (type == GUILD_CHARTER_TYPE && m_Player->GetGuildId())
        return;

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_SIGNATURE);

    stmt->setUInt32(0, petitionGuidLow);

    result = CharacterDatabase.Query(stmt);

    // result == NULL also correct in case no sign yet
    if (result)
        signs = uint8(result->GetRowCount());

    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_PETITION_SHOW_SIGNATURES petition entry: '%u'", petitionGuidLow);

    WorldPacket data(SMSG_PETITION_SHOW_SIGNATURES);
    ByteBuffer signsBuffer;
    ObjectGuid playerGuid = m_Player->GetGUID();

    data.WriteBit(playerGuid[5]);
    data.WriteBit(petitionguid[0]);
    data.WriteBit(petitionguid[4]);
    data.WriteBit(petitionguid[7]);
    data.WriteBit(playerGuid[6]);
    data.WriteBit(petitionguid[1]);
    data.WriteBit(petitionguid[2]);
    data.WriteBit(petitionguid[5]);
    data.WriteBit(petitionguid[3]);
    data.WriteBit(playerGuid[2]);
    data.WriteBit(playerGuid[4]);
    data.WriteBit(playerGuid[0]);
    data.WriteBit(playerGuid[3]);
    data.WriteBit(petitionguid[6]);
    data.WriteBit(playerGuid[1]);
    data.WriteBits(signs, 21);

    for (uint8 i = 1; i <= signs; ++i)
    {
        Field* fields2 = result->Fetch();
        uint32 lowGuid = fields2[0].GetUInt32();

        ObjectGuid signerGuid = MAKE_NEW_GUID(lowGuid, 0, HIGHGUID_PLAYER);

        uint8 bitsSendOrder[8] = { 0, 4, 2, 6, 3, 7, 5, 1 };
        data.WriteBitInOrder(signerGuid, bitsSendOrder);

        signsBuffer.WriteByteSeq(signerGuid[4]);
        signsBuffer.WriteByteSeq(signerGuid[6]);
        signsBuffer.WriteByteSeq(signerGuid[0]);
        signsBuffer.WriteByteSeq(signerGuid[5]);
        signsBuffer.WriteByteSeq(signerGuid[7]);
        signsBuffer.WriteByteSeq(signerGuid[2]);
        signsBuffer.WriteByteSeq(signerGuid[1]);
        signsBuffer << uint32(0);
        signsBuffer.WriteByteSeq(signerGuid[3]);

        result->NextRow();
    }

    data.WriteBit(playerGuid[7]);
    data.FlushBits();

    if (signsBuffer.size())
        data.append(signsBuffer);

    data.WriteByteSeq(playerGuid[7]);
    data.WriteByteSeq(playerGuid[5]);
    data.WriteByteSeq(playerGuid[3]);
    data << uint32(petitionGuidLow);
    data.WriteByteSeq(petitionguid[4]);
    data.WriteByteSeq(playerGuid[2]);
    data.WriteByteSeq(petitionguid[5]);
    data.WriteByteSeq(petitionguid[1]);
    data.WriteByteSeq(playerGuid[4]);
    data.WriteByteSeq(petitionguid[0]);
    data.WriteByteSeq(playerGuid[0]);
    data.WriteByteSeq(petitionguid[7]);
    data.WriteByteSeq(playerGuid[6]);
    data.WriteByteSeq(playerGuid[1]);
    data.WriteByteSeq(petitionguid[3]);
    data.WriteByteSeq(petitionguid[6]);
    data.WriteByteSeq(petitionguid[2]);

    SendPacket(&data);
}

void WorldSession::HandlePetitionQueryOpcode(WorldPacket& p_RecvData)
{
    uint64 l_ItemGUID;

    uint32 l_PetitionID;

    p_RecvData >> l_PetitionID;
    p_RecvData.readPackGUID(l_ItemGUID);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_PETITION_QUERY Petition GUID %u Guild GUID %u", GUID_LOPART(l_ItemGUID), l_PetitionID);

    SendPetitionQueryOpcode(uint64(l_PetitionID));
}

void WorldSession::SendPetitionQueryOpcode(uint64 petitionguid)
{
    uint64 ownerguid = 0;
    uint32 type;
    std::string name = "NO_NAME_FOR_GUID";

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION);

    stmt->setUInt32(0, GUID_LOPART(petitionguid));

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
    {
        Field* fields = result->Fetch();
        ownerguid = MAKE_NEW_GUID(fields[0].GetUInt32(), 0, HIGHGUID_PLAYER);
        name      = fields[1].GetString();
        type      = fields[2].GetUInt8();
    }
    else
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_PETITION_QUERY failed for petition (GUID: %u)", GUID_LOPART(petitionguid));
        return;
    }

    WorldPacket data(SMSG_PETITION_QUERY_RESPONSE);
    ObjectGuid ownerGuid = ownerguid;

    data.WriteBit(type == GUILD_CHARTER_TYPE);

    if (type == GUILD_CHARTER_TYPE)
    {
        data.WriteBit(ownerGuid[7]);
        data.WriteBit(ownerGuid[5]);
        data.WriteBits(name.size(), 7);
        data.WriteBit(ownerGuid[1]);
        data.WriteBits(0, 12);                  // Unk String
        data.WriteBit(ownerGuid[4]);
        data.WriteBit(ownerGuid[6]);

        for (int i = 0; i < 10; ++i)
            data.WriteBits(0, 6);               // Unk Strings

        data.WriteBit(ownerGuid[3]);
        data.WriteBit(ownerGuid[0]);
        data.WriteBit(ownerGuid[2]);

        data << uint32(0);
        data << uint32(0);
        data.WriteByteSeq(ownerGuid[0]);
        data.WriteByteSeq(ownerGuid[2]);
        data << uint32(GUID_LOPART(petitionguid));
        data.WriteByteSeq(ownerGuid[1]);
        data << uint32(4);
        data.WriteByteSeq(ownerGuid[3]);
        data.WriteByteSeq(ownerGuid[4]);
        data << uint32(0);

        if (name.size())
            data.append(name.c_str(), name.size());

        data << uint16(0);
        data << uint32(0);
        data << uint32(0);
        data.WriteByteSeq(ownerGuid[7]);
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
        data << uint32(0);
        data.WriteByteSeq(ownerGuid[6]);
        data.WriteByteSeq(ownerGuid[5]);
        data << uint32(4);
        data << uint32(0);
    }

    data << uint32(GUID_LOPART(petitionguid));

    SendPacket(&data);
}

void WorldSession::HandlePetitionRenameOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Received opcode CMSG_PETITION_RENAME");

    ObjectGuid petitionGuid;
    ObjectGuid playerGuid;
    uint32 type = 0;
    std::string newName;
    uint32 nameLen = 0;

    nameLen = recvData.ReadBits(7);

    uint8 bitsOrder[8] = { 1, 4, 3, 5, 7, 2, 0, 6 };
    recvData.ReadBitInOrder(playerGuid, bitsOrder);

    recvData.FlushBits();

    recvData.ReadByteSeq(playerGuid[1]);
    recvData.ReadByteSeq(playerGuid[7]);
    recvData.ReadByteSeq(playerGuid[2]);
    recvData.ReadByteSeq(playerGuid[5]);
    recvData.ReadByteSeq(playerGuid[4]);
    recvData.ReadByteSeq(playerGuid[3]);
    recvData.ReadByteSeq(playerGuid[6]);
    newName = recvData.ReadString(nameLen);
    recvData.ReadByteSeq(playerGuid[0]);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_BY_OWNER);
    stmt->setUInt32(0, GUID_LOPART(playerGuid));
    stmt->setUInt8(1, GUILD_CHARTER_TYPE);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_PETITION_QUERY failed for player (GUID: %u)", GUID_LOPART(playerGuid));
        return;
    }

    Field* fields = result->Fetch();
    petitionGuid = MAKE_NEW_GUID(fields[0].GetInt32(), 0, HIGHGUID_ITEM);

    Item* item = m_Player->GetItemByGuid(petitionGuid);
    if (!item)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_PETITION_QUERY failed for petition (GUID: %u)", GUID_LOPART(petitionGuid));
        return;
    }

    if (sGuildMgr->GetGuildByName(newName))
    {
        Guild::SendCommandResult(this, GUILD_CREATE_S, ERR_GUILD_NAME_EXISTS_S, newName);
        return;
    }
    if (sObjectMgr->IsReservedName(newName) || !ObjectMgr::IsValidCharterName(newName))
    {
        Guild::SendCommandResult(this, GUILD_CREATE_S, ERR_GUILD_NAME_INVALID, newName);
        return;
    }

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_PETITION_NAME);
    stmt->setString(0, newName);
    stmt->setUInt32(1, GUID_LOPART(petitionGuid));
    CharacterDatabase.Execute(stmt);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Petition (GUID: %u) renamed to '%s'", GUID_LOPART(petitionGuid), newName.c_str());

    WorldPacket data(SMSG_PETITION_RENAME);

    data.WriteBit(petitionGuid[5]);
    data.WriteBit(petitionGuid[0]);
    data.WriteBit(petitionGuid[1]);
    data.WriteBit(petitionGuid[6]);
    data.WriteBit(petitionGuid[7]);
    data.WriteBits(nameLen, 7);
    data.WriteBit(petitionGuid[4]);
    data.WriteBit(petitionGuid[3]);
    data.WriteBit(petitionGuid[2]);

    data.WriteByteSeq(petitionGuid[7]);
    data.WriteByteSeq(petitionGuid[4]);
    if (nameLen)
        data.append(newName.c_str(), nameLen);
    data.WriteByteSeq(petitionGuid[0]);
    data.WriteByteSeq(petitionGuid[3]);
    data.WriteByteSeq(petitionGuid[6]);
    data.WriteByteSeq(petitionGuid[1]);
    data.WriteByteSeq(petitionGuid[2]);
    data.WriteByteSeq(petitionGuid[5]);

    SendPacket(&data);
}

void WorldSession::HandlePetitionSignOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Received opcode CMSG_PETITION_SIGN");

    Field* fields;
    ObjectGuid ownerGuid;
    ObjectGuid petitionGuid;
    uint8 unk;

    recvData >> unk;

    uint8 bitsOrder[8] = { 5, 2, 6, 7, 1, 0, 3, 4 };
    recvData.ReadBitInOrder(ownerGuid, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 4, 7, 6, 3, 0, 2, 5, 1 };
    recvData.ReadBytesSeq(ownerGuid, bytesOrder);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_BY_OWNER);
    stmt->setUInt32(0, GUID_LOPART(ownerGuid));
    stmt->setUInt8(1, GUILD_CHARTER_TYPE);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
        return;

    fields = result->Fetch();
    petitionGuid = MAKE_NEW_GUID(fields[0].GetUInt32(), 0, HIGHGUID_ITEM);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_SIGNATURES);
    stmt->setUInt32(0, GUID_LOPART(petitionGuid));
    stmt->setUInt32(1, GUID_LOPART(petitionGuid));
    result = CharacterDatabase.Query(stmt);

    if (!result)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Petition %u is not found for player %u %s", GUID_LOPART(petitionGuid), GetPlayer()->GetGUIDLow(), GetPlayer()->GetName());
        return;
    }

    fields = result->Fetch();
    uint64 signs = fields[1].GetUInt64();
    uint8 type = fields[2].GetUInt8();

    uint32 playerGuid = m_Player->GetGUIDLow();
    if (GUID_LOPART(ownerGuid) == playerGuid)
        return;

    // not let enemies sign guild charter
    if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GUILD) && GetPlayer()->GetTeam() != sObjectMgr->GetPlayerTeamByGUID(ownerGuid))
    {
        if (type == GUILD_CHARTER_TYPE)
            Guild::SendCommandResult(this, GUILD_CREATE_S, ERR_GUILD_NOT_ALLIED);
        return;
    }

    if (type == GUILD_CHARTER_TYPE)
    {
        if (m_Player->GetGuildId())
        {
            Guild::SendCommandResult(this, GUILD_INVITE_S, ERR_ALREADY_IN_GUILD_S, m_Player->GetName());
            return;
        }
        if (m_Player->GetGuildIdInvited())
        {
            Guild::SendCommandResult(this, GUILD_INVITE_S, ERR_ALREADY_INVITED_TO_GUILD_S, m_Player->GetName());
            return;
        }
    }

    if (++signs > type)                                        // client signs maximum
        return;

    // Client doesn't allow to sign petition two times by one character, but not check sign by another character from same account
    // not allow sign another player from already sign player account
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_SIG_BY_ACCOUNT);

    stmt->setUInt32(0, GetAccountId());
    stmt->setUInt32(1, GUID_LOPART(petitionGuid));

    result = CharacterDatabase.Query(stmt);

    if (result)
    {
        SendPetitionSignResult(m_Player->GetGUID(), petitionGuid, PETITION_SIGN_ALREADY_SIGNED);
        return;
    }

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_PETITION_SIGNATURE);

    stmt->setUInt32(0, GUID_LOPART(ownerGuid));
    stmt->setUInt32(1, GUID_LOPART(petitionGuid));
    stmt->setUInt32(2, playerGuid);
    stmt->setUInt32(3, GetAccountId());

    CharacterDatabase.Execute(stmt);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "PETITION SIGN: GUID %u by player: %s (GUID: %u Account: %u)", GUID_LOPART(petitionGuid), m_Player->GetName(), playerGuid, GetAccountId());

    SendPetitionSignResult(m_Player->GetGUID(), petitionGuid, PETITION_SIGN_OK);

    Player* owner = ObjectAccessor::FindPlayer(ownerGuid);
    if (owner)
        owner->GetSession()->SendPetitionSignResult(m_Player->GetGUID(), petitionGuid, PETITION_SIGN_OK);
}

void WorldSession::HandlePetitionDeclineOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Received opcode CMSG_PETITION_DECLINE");

    ObjectGuid petitionGuid;
    ObjectGuid ownerGuid;

    uint8 bitsOrder[8] = { 7, 5, 6, 2, 4, 3, 1, 0 };
    recvData.ReadBitInOrder(ownerGuid, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 1, 0, 6, 4, 7, 5, 3, 2 };
    recvData.ReadBytesSeq(ownerGuid, bytesOrder);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Petition %u declined by %u", GUID_LOPART(petitionGuid), m_Player->GetGUIDLow());

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_BY_OWNER);

    stmt->setUInt32(0, GUID_LOPART(ownerGuid));
    stmt->setUInt8(1, GUILD_CHARTER_TYPE);

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
        return;

    Field* fields = result->Fetch();
    petitionGuid = MAKE_NEW_GUID(fields[0].GetUInt32(), 0, HIGHGUID_ITEM);

    Player* owner = ObjectAccessor::FindPlayer(ownerGuid);
    if (owner)
        owner->GetSession()->SendPetitionSignResult(ownerGuid, petitionGuid, PETITION_SIGN_DECLINED);
}

void WorldSession::HandleOfferPetitionOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Received opcode CMSG_OFFER_PETITION");

    uint8 signs = 0;
    ObjectGuid ownerGuid, playerGuid;
    uint32 type, junk;
    uint32 petitionGuidLow = 0;
    Player* player;

    recvData >> junk;                                      // this is not petition type!

    playerGuid[5] = recvData.ReadBit();
    playerGuid[0] = recvData.ReadBit();
    ownerGuid[6] = recvData.ReadBit();
    ownerGuid[3] = recvData.ReadBit();
    ownerGuid[5] = recvData.ReadBit();
    ownerGuid[7] = recvData.ReadBit();
    playerGuid[3] = recvData.ReadBit();
    ownerGuid[4] = recvData.ReadBit();
    playerGuid[6] = recvData.ReadBit();
    playerGuid[4] = recvData.ReadBit();
    playerGuid[2] = recvData.ReadBit();
    ownerGuid[2] = recvData.ReadBit();
    playerGuid[1] = recvData.ReadBit();
    ownerGuid[1] = recvData.ReadBit();
    ownerGuid[0] = recvData.ReadBit();
    playerGuid[7] = recvData.ReadBit();

    recvData.FlushBits();

    recvData.ReadByteSeq(ownerGuid[0]);
    recvData.ReadByteSeq(playerGuid[2]);
    recvData.ReadByteSeq(playerGuid[4]);
    recvData.ReadByteSeq(ownerGuid[2]);
    recvData.ReadByteSeq(playerGuid[1]);
    recvData.ReadByteSeq(playerGuid[6]);
    recvData.ReadByteSeq(ownerGuid[7]);
    recvData.ReadByteSeq(ownerGuid[3]);
    recvData.ReadByteSeq(playerGuid[5]);
    recvData.ReadByteSeq(ownerGuid[6]);
    recvData.ReadByteSeq(ownerGuid[1]);
    recvData.ReadByteSeq(playerGuid[7]);
    recvData.ReadByteSeq(playerGuid[0]);
    recvData.ReadByteSeq(playerGuid[3]);
    recvData.ReadByteSeq(ownerGuid[5]);
    recvData.ReadByteSeq(ownerGuid[4]);

    player = ObjectAccessor::FindPlayer(playerGuid);
    if (!player)
        return;

    type = GUILD_CHARTER_TYPE;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_BY_OWNER);

    stmt->setUInt32(0, GUID_LOPART(ownerGuid));
    stmt->setUInt8(1, type);

    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
        return;

    Field* fields = result->Fetch();
    petitionGuidLow = fields[0].GetUInt32();

    sLog->outDebug(LOG_FILTER_NETWORKIO, "OFFER PETITION: type %u, GUID1 %u, to player id: %u", type, petitionGuidLow, GUID_LOPART(playerGuid));

    if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GUILD) && GetPlayer()->GetTeam() != player->GetTeam())
    {
        if (type == GUILD_CHARTER_TYPE)
            Guild::SendCommandResult(this, GUILD_CREATE_S, ERR_GUILD_NOT_ALLIED);
        return;
    }

    if (type == GUILD_CHARTER_TYPE)
    {
        if (player->GetGuildIdInvited())
        {
            SendPetitionSignResult(m_Player->GetGUID(), MAKE_NEW_GUID(petitionGuidLow, 0, HIGHGUID_ITEM), PETITION_SIGN_ALREADY_SIGNED_OTHER);
            return;
        }

        if (player->GetGuildId())
        {
            SendPetitionSignResult(m_Player->GetGUID(), MAKE_NEW_GUID(petitionGuidLow, 0, HIGHGUID_ITEM), PETITION_SIGN_ALREADY_IN_GUILD);
            return;
        }
    }

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_SIGNATURE);
    stmt->setUInt32(0, petitionGuidLow);
    result = CharacterDatabase.Query(stmt);

    std::list<uint32> playerGuids;

    // result == NULL also correct charter without signs
    if (result)
    {
        signs = uint8(result->GetRowCount());

        do
        {
            fields = result->Fetch();
            playerGuids.push_back(fields[0].GetUInt32());
        }
        while (result->NextRow());
    }

    for (auto itr : playerGuids)
    {
        if (GUID_LOPART(playerGuid) == itr)
        {
            player->GetSession()->SendAlreadySigned(playerGuid);
            return;
        }
    }

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_SIGNATURE);
    stmt->setUInt32(0, petitionGuidLow);
    result = CharacterDatabase.Query(stmt);

    WorldPacket data(SMSG_PETITION_SHOW_SIGNATURES);
    ByteBuffer signsBuffer;
    ObjectGuid petitionGuid = MAKE_NEW_GUID(petitionGuidLow, 0, HIGHGUID_ITEM);
    ObjectGuid masterGuid = m_Player->GetGUID();

    data.WriteBit(masterGuid[5]);
    data.WriteBit(petitionGuid[0]);
    data.WriteBit(petitionGuid[4]);
    data.WriteBit(petitionGuid[7]);
    data.WriteBit(masterGuid[6]);
    data.WriteBit(petitionGuid[1]);
    data.WriteBit(petitionGuid[2]);
    data.WriteBit(petitionGuid[5]);
    data.WriteBit(petitionGuid[3]);
    data.WriteBit(masterGuid[2]);
    data.WriteBit(masterGuid[4]);
    data.WriteBit(masterGuid[0]);
    data.WriteBit(masterGuid[3]);
    data.WriteBit(petitionGuid[6]);
    data.WriteBit(masterGuid[1]);
    data.WriteBits(signs, 21);

    for (uint8 i = 1; i <= signs; ++i)
    {
        Field* fields2 = result->Fetch();
        uint32 lowGuid = fields2[0].GetUInt32();

        ObjectGuid signerGuid = MAKE_NEW_GUID(lowGuid, 0, HIGHGUID_PLAYER);

        uint8 bitsSendOrder[8] = { 0, 4, 2, 6, 3, 7, 5, 1 };
        data.WriteBitInOrder(signerGuid, bitsSendOrder);

        signsBuffer.WriteByteSeq(signerGuid[4]);
        signsBuffer.WriteByteSeq(signerGuid[6]);
        signsBuffer.WriteByteSeq(signerGuid[0]);
        signsBuffer.WriteByteSeq(signerGuid[5]);
        signsBuffer.WriteByteSeq(signerGuid[7]);
        signsBuffer.WriteByteSeq(signerGuid[2]);
        signsBuffer.WriteByteSeq(signerGuid[1]);
        signsBuffer << uint32(0);
        signsBuffer.WriteByteSeq(signerGuid[3]);

        result->NextRow();
    }

    data.WriteBit(masterGuid[7]);
    data.FlushBits();

    if (signsBuffer.size())
        data.append(signsBuffer);

    data.WriteByteSeq(masterGuid[7]);
    data.WriteByteSeq(masterGuid[5]);
    data.WriteByteSeq(masterGuid[3]);
    data << uint32(petitionGuidLow);
    data.WriteByteSeq(petitionGuid[4]);
    data.WriteByteSeq(masterGuid[2]);
    data.WriteByteSeq(petitionGuid[5]);
    data.WriteByteSeq(petitionGuid[1]);
    data.WriteByteSeq(masterGuid[4]);
    data.WriteByteSeq(petitionGuid[0]);
    data.WriteByteSeq(masterGuid[0]);
    data.WriteByteSeq(petitionGuid[7]);
    data.WriteByteSeq(masterGuid[6]);
    data.WriteByteSeq(masterGuid[1]);
    data.WriteByteSeq(petitionGuid[3]);
    data.WriteByteSeq(petitionGuid[6]);
    data.WriteByteSeq(petitionGuid[2]);

    player->GetSession()->SendPacket(&data);
}

void WorldSession::HandleTurnInPetitionOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Received opcode CMSG_TURN_IN_PETITION");

    // Get petition guid from packet
    WorldPacket data;
    ObjectGuid petitionGuid;

    uint8 bitsOrder[8] = { 2, 0, 1, 4, 6, 5, 3, 7 };
    recvData.ReadBitInOrder(petitionGuid, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 3, 6, 7, 0, 1, 2, 5, 4 };
    recvData.ReadBytesSeq(petitionGuid, bytesOrder);

    // Check if player really has the required petition charter
    Item* item = m_Player->GetItemByGuid(petitionGuid);
    if (!item)
        return;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "Petition %u turned in by %u", GUID_LOPART(petitionGuid), m_Player->GetGUIDLow());

    // Get petition data from db
    uint32 ownerguidlo;
    uint32 type;
    std::string name;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION);
    stmt->setUInt32(0, GUID_LOPART(petitionGuid));
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (result)
    {
        Field* fields = result->Fetch();
        ownerguidlo = fields[0].GetUInt32();
        name = fields[1].GetString();
        type = fields[2].GetUInt8();
    }
    else
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Player %s (guid: %u) tried to turn in petition (guid: %u) that is not present in the database", m_Player->GetName(), m_Player->GetGUIDLow(), GUID_LOPART(petitionGuid));
        return;
    }

    // Only the petition owner can turn in the petition
    if (m_Player->GetGUIDLow() != ownerguidlo)
        return;

    // Check if player is already in a guild
    if (m_Player->GetGuildId())
    {
        data.Initialize(SMSG_TURN_IN_PETITION_RESULTS, 4);
        data.WriteBits(PETITION_TURN_ALREADY_IN_GUILD, 4);
        data.FlushBits();
        SendPacket(&data);
        return;
    }

    // Check if guild name is already taken
    if (sGuildMgr->GetGuildByName(name))
    {
        Guild::SendCommandResult(this, GUILD_CREATE_S, ERR_GUILD_NAME_EXISTS_S, name);
        return;
    }

    // Get petition signatures from db
    uint8 signatures;

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PETITION_SIGNATURE);
    stmt->setUInt32(0, GUID_LOPART(petitionGuid));
    result = CharacterDatabase.Query(stmt);

    if (result)
        signatures = uint8(result->GetRowCount());
    else
        signatures = 0;

    uint32 requiredSignatures;
    requiredSignatures = sWorld->getIntConfig(CONFIG_MIN_PETITION_SIGNS);

    // Notify player if signatures are missing
    if (signatures < requiredSignatures)
    {
        data.Initialize(SMSG_TURN_IN_PETITION_RESULTS, 4);
        data.WriteBits(PETITION_TURN_NEED_MORE_SIGNATURES, 4);
        data.FlushBits();
        SendPacket(&data);
        return;
    }

    // Proceed with guild/arena team creation

    // Delete charter item
    m_Player->DestroyItem(item->GetBagSlot(), item->GetSlot(), true);

    // Create guild
    Guild* guild = new Guild;

    if (!guild->Create(m_Player, name))
    {
        delete guild;
        return;
    }

    // Register guild and add guild master
    sGuildMgr->AddGuild(guild);

    // Add members from signatures
    for (uint8 i = 0; i < signatures; ++i)
    {
        Field* fields = result->Fetch();
        guild->AddMember(MAKE_NEW_GUID(fields[0].GetUInt32(), 0, HIGHGUID_PLAYER));
        result->NextRow();
    }

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PETITION_BY_GUID);
    stmt->setUInt32(0, GUID_LOPART(petitionGuid));
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_PETITION_SIGNATURE_BY_GUID);
    stmt->setUInt32(0, GUID_LOPART(petitionGuid));
    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);

    // created
    sLog->outDebug(LOG_FILTER_NETWORKIO, "TURN IN PETITION GUID %u", GUID_LOPART(petitionGuid));

    data.Initialize(SMSG_TURN_IN_PETITION_RESULTS, 4);
    data.WriteBits(PETITION_TURN_OK, 4);
    data.FlushBits();
    SendPacket(&data);
}

void WorldSession::HandlePetitionShowListOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Received CMSG_PETITION_SHOWLIST");

    uint64 guid;
    recvData >> guid;

    SendPetitionShowList(guid);
}

void WorldSession::SendPetitionShowList(uint64 guid)
{
    Creature* creature = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_PETITIONER);
    if (!creature)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandlePetitionShowListOpcode - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(guid)));
        return;
    }

    WorldPacket data(SMSG_PETITION_SHOW_LIST, 8 + 1 + 4 * 6);
    ObjectGuid npcGuid = guid;

    uint8 bitsOrder[8] = { 7, 2, 0, 1, 5, 3, 6, 4 };
    data.WriteBitInOrder(npcGuid, bitsOrder);

    data.WriteByteSeq(npcGuid[2]);
    data.WriteByteSeq(npcGuid[3]);
    data.WriteByteSeq(npcGuid[7]);
    data.WriteByteSeq(npcGuid[5]);
    data.WriteByteSeq(npcGuid[4]);
    data.WriteByteSeq(npcGuid[0]);
    data.WriteByteSeq(npcGuid[6]);
    data << uint32(GUILD_CHARTER_COST);                 // charter cost
    data.WriteByteSeq(npcGuid[1]);

    SendPacket(&data);
    sLog->outDebug(LOG_FILTER_NETWORKIO, "Sent SMSG_PETITION_SHOW_LIST");
}

void WorldSession::SendPetitionSignResult(ObjectGuid ownerGuid, ObjectGuid petitionGuid, uint8 result)
{
    WorldPacket data(SMSG_PETITION_DECLINED);

    data.WriteBit(ownerGuid[5]);
    data.WriteBit(petitionGuid[7]);
    data.WriteBit(petitionGuid[4]);
    data.WriteBit(ownerGuid[3]);
    data.WriteBit(ownerGuid[1]);
    data.WriteBit(petitionGuid[6]);
    data.WriteBit(petitionGuid[1]);
    data.WriteBit(petitionGuid[3]);
    data.WriteBit(ownerGuid[7]);
    data.WriteBits(PetitionSigns(result), 4);
    data.WriteBit(petitionGuid[0]);
    data.WriteBit(ownerGuid[2]);
    data.WriteBit(petitionGuid[5]);
    data.WriteBit(ownerGuid[4]);
    data.WriteBit(ownerGuid[0]);
    data.WriteBit(ownerGuid[6]);
    data.WriteBit(petitionGuid[2]);

    data.WriteByteSeq(petitionGuid[2]);
    data.WriteByteSeq(petitionGuid[4]);
    data.WriteByteSeq(petitionGuid[7]);
    data.WriteByteSeq(petitionGuid[3]);
    data.WriteByteSeq(ownerGuid[7]);
    data.WriteByteSeq(ownerGuid[2]);
    data.WriteByteSeq(ownerGuid[3]);
    data.WriteByteSeq(petitionGuid[6]);
    data.WriteByteSeq(petitionGuid[5]);
    data.WriteByteSeq(ownerGuid[4]);
    data.WriteByteSeq(ownerGuid[0]);
    data.WriteByteSeq(petitionGuid[0]);
    data.WriteByteSeq(ownerGuid[5]);
    data.WriteByteSeq(ownerGuid[6]);
    data.WriteByteSeq(ownerGuid[1]);
    data.WriteByteSeq(petitionGuid[1]);

    SendPacket(&data);
}

void WorldSession::SendAlreadySigned(ObjectGuid playerGuid)
{
    WorldPacket data(SMSG_PETITION_ALREADY_SIGNED);

    uint8 bitsOrder[8] = { 4, 0, 7, 1, 5, 6, 2, 3 };
    data.WriteBitInOrder(playerGuid, bitsOrder);

    uint8 bytesOder[8] = { 6, 0, 2, 4, 3, 1, 5, 7 };
    data.WriteBytesSeq(playerGuid, bytesOder);

    SendPacket(&data);
}
