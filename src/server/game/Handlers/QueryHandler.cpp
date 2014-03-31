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
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "UpdateMask.h"
#include "NPCHandler.h"
#include "Pet.h"
#include "MapManager.h"

void WorldSession::SendNameQueryOpcode(uint64 guid)
{
    Player* player = ObjectAccessor::FindPlayer(guid);
    CharacterNameData const* nameData = sWorld->GetCharacterNameData(GUID_LOPART(guid));

    WorldPacket data(SMSG_NAME_QUERY_RESPONSE);

    ObjectGuid pGuid = guid;

    uint8 guidOrder[8] = { 5, 7, 3, 0, 4, 1, 6, 2 };
    data.WriteBitInOrder(guid, guidOrder);

    data.WriteByteSeq(pGuid[7]);
    data.WriteByteSeq(pGuid[4]);
    data.WriteByteSeq(pGuid[3]);

    data << uint8(!nameData ? 1 : 0);

    if (nameData)
    {
        data << uint32(0);
        data << uint8(nameData->m_race);
        data << uint8(nameData->m_gender);
        data << uint8(nameData->m_level);
        data << uint8(nameData->m_class);
        data << uint32(realmID);
    }

    data.WriteByteSeq(pGuid[1]);
    data.WriteByteSeq(pGuid[5]);
    data.WriteByteSeq(pGuid[0]);
    data.WriteByteSeq(pGuid[6]);
    data.WriteByteSeq(pGuid[2]);

    if (nameData)
    {
        ObjectGuid unkGuid = 0;
        ObjectGuid pGuid2 = pGuid;

        data.WriteBit(pGuid2[6]);
        data.WriteBit(unkGuid[7]);
        data.WriteBits(nameData->m_name.size(), 6);
        data.WriteBit(pGuid2[1]);
        data.WriteBit(pGuid2[7]);
        data.WriteBit(pGuid2[2]);
        data.WriteBit(unkGuid[4]);
        data.WriteBit(pGuid2[4]);
        data.WriteBit(pGuid2[0]);
        data.WriteBit(unkGuid[1]);

        if (DeclinedName const* names = (player ? player->GetDeclinedNames() : NULL))
        {
            for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
                data.WriteBits(names->name[i].size(), 7);
        }
        else
        {
            for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
                data.WriteBits(0, 7);
        }

        data.WriteBit(unkGuid[3]);
        data.WriteBit(pGuid2[3]);
        data.WriteBit(unkGuid[5]);
        data.WriteBit(unkGuid[0]);
        data.WriteBit(pGuid2[5]);
        data.WriteBit(false); // unk
        data.WriteBit(unkGuid[2]);
        data.WriteBit(unkGuid[6]);

        data.FlushBits();
        if (nameData->m_name.size())
            data.append(nameData->m_name.c_str(), nameData->m_name.size());

        data.WriteByteSeq(pGuid2[4]);
        data.WriteByteSeq(unkGuid[3]);
        data.WriteByteSeq(pGuid2[6]);
        data.WriteByteSeq(unkGuid[2]);
        data.WriteByteSeq(unkGuid[4]);
        data.WriteByteSeq(pGuid2[5]);
        data.WriteByteSeq(pGuid2[1]);
        data.WriteByteSeq(pGuid2[7]);

        if (DeclinedName const* names = (player ? player->GetDeclinedNames() : NULL))
        {
            for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
                if (names->name[i].size())
                    data.append(names->name[i].c_str(), names->name[i].size());
        }
        else
        {
            for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
                data.WriteBits(0, 7);
        }

        data.WriteByteSeq(pGuid2[3]);
        data.WriteByteSeq(unkGuid[7]);
        data.WriteByteSeq(unkGuid[1]);
        data.WriteByteSeq(unkGuid[6]);
        data.WriteByteSeq(pGuid2[0]);
        data.WriteByteSeq(unkGuid[0]);
        data.WriteByteSeq(pGuid2[2]);
        data.WriteByteSeq(unkGuid[5]);
    }

    SendPacket(&data);
}

void WorldSession::HandleNameQueryOpcode(WorldPacket& recvData)
{
    /*
        CMSG_NAME_QUERY 5.4.0 17371
        7D -> 0 1 1 1 1 1 0 1
        80 -> 1 0 0 0 0 0 0 0
        10 // guid part
        41 // guid part
        05 // guid part
        81 // guid part
        1D // guid part
        00 // guid part
        27000103 // unk bit 1, realm id / flags ?
    */

    ObjectGuid guid;

    guid[5] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    bool hasUnkBit1 = recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    bool hasUnkBit2 = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();

    recvData.FlushBits();

    uint8 order[8] = {0, 1, 3, 4, 6, 5, 2, 7};
    recvData.ReadBytesSeq(guid, order);

    if (hasUnkBit2)
    {
        uint32 unk = recvData.read<uint32>();
        sLog->outInfo(LOG_FILTER_NETWORKIO, "CMSG_NAME_QUERY uint32 unk : %u\r\n", unk);
    }

    if (hasUnkBit1)
    {
        uint32 unk1 = recvData.read<uint32>();
        sLog->outInfo(LOG_FILTER_NETWORKIO, "CMSG_NAME_QUERY uint32 unk1 (realm flags / id ?) : %u\r\n", unk1);
    }

    // This is disable by default to prevent lots of console spam
    // sLog->outInfo(LOG_FILTER_NETWORKIO, "HandleNameQueryOpcode %u", guid);

    SendNameQueryOpcode(guid);
}

void WorldSession::HandleQueryTimeOpcode(WorldPacket& /*recvData*/)
{
    SendQueryTimeResponse();
}

void WorldSession::SendQueryTimeResponse()
{
    WorldPacket data(SMSG_QUERY_TIME_RESPONSE, 4+4);
    data << uint32(sWorld->GetNextDailyQuestsResetTime() - time(NULL));
    data << uint32(time(NULL));
    SendPacket(&data);
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandleCreatureQueryOpcode(WorldPacket& recvData)
{
    uint32 entry;
    recvData >> entry;

    if (CreatureTemplate const* ci = sObjectMgr->GetCreatureTemplate(entry))
    {

        std::string Name, SubName, Unk505;
        Name = ci->Name;
        SubName = ci->SubName;
        Unk505 = "";

        int loc_idx = GetSessionDbLocaleIndex();
        if (loc_idx >= 0)
        {
            if (CreatureLocale const* cl = sObjectMgr->GetCreatureLocale(entry))
            {
                ObjectMgr::GetLocaleString(cl->Name, loc_idx, Name);
                ObjectMgr::GetLocaleString(cl->SubName, loc_idx, SubName);
            }
        }
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_CREATURE_QUERY '%s' - Entry: %u.", ci->Name.c_str(), entry);

        WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE);

        data << uint32(entry);                                                          // creature entry
        data.WriteBit(1);                                                               // has valid data
        data.WriteBits(Name.size() ? Name.size() + 1 : 0, 11);                          // Male

        for (int i = 0; i < 7; i++)
            data.WriteBits(0, 11);                          // Female and other Names - Never send it

        uint8 itemCount = 0;
        for (uint32 i = 0; i < MAX_CREATURE_QUEST_ITEMS; ++i)
            if (ci->questItems[i])
                itemCount++;                                // itemId[6], quest drop

        data.WriteBits(itemCount, 22);
        data.WriteBits(ci->IconName.size() ? ci->IconName.size() + 1 : 0, 6);
        data.WriteBits(SubName.size() ? SubName.size() + 1 : 0, 11);
        data.WriteBits(Unk505.size() ? Unk505.size() + 1 : 0, 11);
        data.WriteBit(ci->RacialLeader);                    // isRacialLeader
        data << uint32(ci->type);                           // CreatureType.dbc
        data << uint32(ci->KillCredit[1]);                  // new in 3.1, kill credit
        data << uint32(ci->Modelid4);                       // Modelid4
        data << uint32(ci->Modelid3);                       // Modelid3

        for (uint32 i = 0; i < MAX_CREATURE_QUEST_ITEMS && itemCount > 0; ++i)
        {
            if (ci->questItems[i])
            {
                data << uint32(ci->questItems[i]);
                itemCount--;
            }
        }

        data << uint32(ci->expansionUnknown);               // unknown meaning

        if (ci->Name.size())
            data << Name;                                   // Name

        if (Unk505.size())
            data << Unk505;                                 // Unknow string since 5.0.5

        data << float(ci->ModMana);                         // dmg/mana modifier
        data << uint32(ci->Modelid1);                       // Modelid1

        if (ci->IconName.size())
            data << ci->IconName;                           // Icon Name

        data << uint32(ci->KillCredit[0]);                  // new in 3.1, kill credit
        data << uint32(ci->Modelid2);                       // Modelid2

        if (SubName.size())
            data << SubName;                                // Sub Name

        data << uint32(ci->type_flags);                     // flags
        data << uint32(ci->type_flags2);                    // unknown meaning
        data << float(ci->ModHealth);                       // dmg/hp modifier
        data << uint32(ci->family);                         // CreatureFamily.dbc
        data << uint32(ci->rank);                           // Creature Rank (elite, boss, etc)
        data << uint32(ci->movementId);                     // CreatureMovementInfo.dbc

        SendPacket(&data);
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_CREATURE_QUERY_RESPONSE");
    }
    else
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_CREATURE_QUERY - NO CREATURE INFO! (ENTRY: %u)", entry);
        WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE, 4);
        data << uint32(entry | 0x80000000);
        data.WriteBit(0); // has no valid data
        SendPacket(&data);
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_CREATURE_QUERY_RESPONSE");
    }
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandleGameObjectQueryOpcode(WorldPacket& recvData)
{
    uint32 entry;
    recvData >> entry;
    ObjectGuid guid;

    uint8 bitOrder[8] = { 2, 4, 3, 7, 0, 6, 1, 5 };
    recvData.ReadBitInOrder(guid, bitOrder);

    recvData.FlushBits();

    uint8 byteOrder[8] = { 1, 7, 2, 3, 6, 5, 4, 0 };
    recvData.ReadBytesSeq(guid, byteOrder);

    if (const GameObjectTemplate* info = sObjectMgr->GetGameObjectTemplate(entry))
    {
        std::string Name;
        std::string IconName;
        std::string CastBarCaption;

        Name = info->name;
        IconName = info->IconName;
        CastBarCaption = info->castBarCaption;

        int loc_idx = GetSessionDbLocaleIndex();
        if (loc_idx >= 0)
        {
            if (GameObjectLocale const* gl = sObjectMgr->GetGameObjectLocale(entry))
            {
                ObjectMgr::GetLocaleString(gl->Name, loc_idx, Name);
                ObjectMgr::GetLocaleString(gl->CastBarCaption, loc_idx, CastBarCaption);
            }
        }

        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_GAMEOBJECT_QUERY '%s' - Entry: %u. ", info->name.c_str(), entry);
        WorldPacket data (SMSG_GAMEOBJECT_QUERY_RESPONSE);
        ByteBuffer byteBuffer;

        data.WriteBit(1);                                               // Always 1, from sniffs
        data.FlushBits();

        data << uint32(entry);

        {
            byteBuffer << uint32(info->type);
            byteBuffer << uint32(info->displayId);
            byteBuffer << Name;
            byteBuffer << uint32(0);                                    // unk
            byteBuffer << CastBarCaption;                               // 2.0.3, string. Text will appear in Cast Bar when using GO (ex: "Collecting")
            byteBuffer << IconName;                                     // 2.0.3, string. Icon name to use instead of default icon for go's (ex: "Attack" makes sword)

            for (int i = 0; i < 32; i++)
                byteBuffer << uint32(info->raw.data[i]);

            byteBuffer << float(info->size);                            // go size

            uint8 questItemCount = 0;
            for (uint32 i = 0; i < MAX_GAMEOBJECT_QUEST_ITEMS; ++i)
                if (info->questItems[i])
                    questItemCount++;

            byteBuffer << uint8(questItemCount);

            for (int i = 0; i < MAX_GAMEOBJECT_QUEST_ITEMS && questItemCount > 0; i++)
            {
                if (info->questItems[i])
                {
                    byteBuffer << uint32(info->questItems[i]);          // itemId[6], quest drop
                    questItemCount--;
                }
            }

            byteBuffer << uint32(info->unkInt32);                       // 4.x, unknown
        }

        data << uint32(byteBuffer.size());
        data.append(byteBuffer);

        SendPacket(&data);
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_GAMEOBJECT_QUERY_RESPONSE");
    }
    else
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_GAMEOBJECT_QUERY - Missing gameobject info for (GUID: %u, ENTRY: %u)",
            GUID_LOPART(guid), entry);
        WorldPacket data (SMSG_GAMEOBJECT_QUERY_RESPONSE, 4);
        data << uint32(entry | 0x80000000);
        SendPacket(&data);
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_GAMEOBJECT_QUERY_RESPONSE");
    }
}

void WorldSession::HandleCorpseQueryOpcode(WorldPacket& /*recvData*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_CORPSE_QUERY");

    Corpse* corpse = GetPlayer()->GetCorpse();

    if (!corpse)
    {
        WorldPacket data(SMSG_CORPSE_QUERY);

        data << uint32(0);
        data << float(0);
        data << uint32(0);
        data << float(0);
        data << float(0);

        data.WriteBit(0);
        data.WriteBit(0);
        data.WriteBit(0);
        data.WriteBit(0);
        data.WriteBit(0);
        data.WriteBit(0);
        data.WriteBit(0);
        data.WriteBit(0);
        data.WriteBit(0);

        SendPacket(&data);
        return;
    }

    uint32 mapid = corpse->GetMapId();
    float x = corpse->GetPositionX();
    float y = corpse->GetPositionY();
    float z = corpse->GetPositionZ();
    uint32 corpsemapid = mapid;

    // if corpse at different map
    if (mapid != _player->GetMapId())
    {
        // search entrance map for proper show entrance
        if (MapEntry const* corpseMapEntry = sMapStore.LookupEntry(mapid))
        {
            if (corpseMapEntry->IsDungeon() && corpseMapEntry->entrance_map >= 0)
            {
                // if corpse map have entrance
                if (Map const* entranceMap = sMapMgr->CreateBaseMap(corpseMapEntry->entrance_map))
                {
                    mapid = corpseMapEntry->entrance_map;
                    x = corpseMapEntry->entrance_x;
                    y = corpseMapEntry->entrance_y;
                    z = entranceMap->GetHeight(GetPlayer()->GetPhaseMask(), x, y, MAX_HEIGHT);
                }
            }
        }
    }
    ObjectGuid guid = corpse->GetGUID();

    WorldPacket data(SMSG_CORPSE_QUERY);

    data << uint32(mapid);
    data << float(x);
    data << uint32(corpsemapid);
    data << float(y);
    data << float(z);

    data.WriteBit(guid[5]);
    data.WriteBit(guid[3]);
    data.WriteBit(guid[4]);
    data.WriteBit(guid[2]);
    data.WriteBit(guid[6]);
    data.WriteBit(guid[0]);
    data.WriteBit(guid[7]);
    data.WriteBit(guid[1]);
    data.WriteBit(1);

    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[1]);
    data.WriteByteSeq(guid[6]);
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[3]);

    SendPacket(&data);
}

void WorldSession::HandleCemeteryListOpcode(WorldPacket& /*recvData*/)
{
    GetPlayer()->SendCemeteryList(false);
}

void WorldSession::HandleForcedReactionsOpcode(WorldPacket& /*recvData*/)
{
    GetPlayer()->GetReputationMgr().SendForceReactions();
}

void WorldSession::HandleNpcTextQueryOpcode(WorldPacket& recvData)
{
    uint32 textID;
    ObjectGuid guid;

    recvData >> textID;
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_NPC_TEXT_QUERY ID '%u'", textID);

    uint8 bitOrder[8] = {7, 3, 1, 5, 6, 4, 0, 2};
    recvData.ReadBitInOrder(guid, bitOrder);

    uint8 byteOrder[8] = {1, 5, 2, 7, 3, 6, 4, 0};
    recvData.ReadBytesSeq(guid, byteOrder);

    GetPlayer()->SetSelection(guid);

    GossipText const* pGossip = sObjectMgr->GetGossipText(textID);

    WorldPacket data(SMSG_NPC_TEXT_UPDATE, 100);          // guess size
    data << textID;
    data << uint32(0x40);       // size of packet
    data << uint32(0x3F800000); // unk flags 5.0.5
    data << uint32(0x00); // unk 5.0.5
    data << uint32(0x00); // unk 5.0.5
    data << uint32(0x00); // unk 5.0.5
    data << uint32(0x00); // unk 5.0.5
    data << uint32(0x00); // unk 5.0.5
    data << uint32(0x00); // unk 5.0.5
    data << uint32(0x00); // unk 5.0.5
    data << textID;
    data << uint32(0x00); // unk 5.0.5
    data << uint32(0x00); // unk 5.0.5
    data << uint32(0x00); // unk 5.0.5
    data << uint32(0x00); // unk 5.0.5
    data << uint32(0x00); // unk 5.0.5
    data << uint32(0x00); // unk 5.0.5
    data << uint32(0x00); // unk 5.0.5
    data.WriteBit(1);     // unk bit (true on retail sniff)
    data.FlushBits();

    /*if (!pGossip)
    {
        for (uint32 i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            data << float(0);
            data << "Greetings $N";
            data << "Greetings $N";
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
            data << uint32(0);
        }
    }
    else
    {
        std::string Text_0[MAX_LOCALES], Text_1[MAX_LOCALES];
        for (int i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            Text_0[i]=pGossip->Options[i].Text_0;
            Text_1[i]=pGossip->Options[i].Text_1;
        }

        int loc_idx = GetSessionDbLocaleIndex();
        if (loc_idx >= 0)
        {
            if (NpcTextLocale const* nl = sObjectMgr->GetNpcTextLocale(textID))
            {
                for (int i = 0; i < MAX_LOCALES; ++i)
                {
                    ObjectMgr::GetLocaleString(nl->Text_0[i], loc_idx, Text_0[i]);
                    ObjectMgr::GetLocaleString(nl->Text_1[i], loc_idx, Text_1[i]);
                }
            }
        }

        for (int i = 0; i < MAX_GOSSIP_TEXT_OPTIONS; ++i)
        {
            data << pGossip->Options[i].Probability;

            if (Text_0[i].empty())
                data << Text_1[i];
            else
                data << Text_0[i];

            if (Text_1[i].empty())
                data << Text_0[i];
            else
                data << Text_1[i];

            data << pGossip->Options[i].Language;

            for (int j = 0; j < MAX_GOSSIP_TEXT_EMOTES; ++j)
            {
                data << pGossip->Options[i].Emotes[j]._Delay;
                data << pGossip->Options[i].Emotes[j]._Emote;
            }
        }
    }*/

    SendPacket(&data);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_NPC_TEXT_UPDATE");
}

#define DEFAULT_GREETINGS_GOSSIP      68

void WorldSession::SendBroadcastTextDb2Reply(uint32 entry)
{
    ByteBuffer buff;
    WorldPacket data(SMSG_DB_REPLY);

    GossipText const* pGossip = sObjectMgr->GetGossipText(entry);

    if (!pGossip)
        pGossip = sObjectMgr->GetGossipText(DEFAULT_GREETINGS_GOSSIP);

    std::string text = "Greetings, $N";
    std::string text1 = pGossip ? pGossip->Options[0].Text_0 : text;
    std::string text2 = pGossip ? pGossip->Options[0].Text_1 : text;

    uint16 size1 = text1.size();
    uint16 size2 = text2.size();

    int loc_idx = GetSessionDbLocaleIndex();
    if (loc_idx >= 0 && loc_idx <= MAX_LOCALES)
    {
        if (NpcTextLocale const* nl = sObjectMgr->GetNpcTextLocale(entry))
        {
            ObjectMgr::GetLocaleString(nl->Text_0[loc_idx], loc_idx, text1);
            ObjectMgr::GetLocaleString(nl->Text_1[loc_idx], loc_idx, text2);
            size1 = text1.size();
            size2 = text2.size();
        }
    }

    buff << uint32(entry);
    buff << uint32(0); // unk
    buff << uint16(size1);
    if (size1)
        buff << std::string(text1);
    buff << uint16(size2);
    if (size2)
        buff << std::string(text2);

    buff << uint32(0);
    buff << uint32(0);
    buff << uint32(0);

    buff << uint32(0);
    buff << uint32(0);
    buff << uint32(0);

    buff << uint32(0); // sound Id
    buff << uint32(pGossip ? pGossip->Options[0].Emotes[0]._Delay : 0); // Delay
    buff << uint32(pGossip ? pGossip->Options[0].Emotes[0]._Emote : 0); // Emote

    data << uint32(buff.size());
    data.append(buff);

    data << uint32(DB2_REPLY_BROADCAST_TEXT);
    data << uint32(sObjectMgr->GetHotfixDate(entry, DB2_REPLY_BROADCAST_TEXT));
    data << uint32(entry);

    SendPacket(&data);
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandlePageTextQueryOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_PAGE_TEXT_QUERY");

    ObjectGuid objectGuid;
    uint32 pageID;
    recvData >> pageID;

    uint8 bitsOrder[8] = { 0, 7, 5, 2, 1, 3, 4, 6 };
    recvData.ReadBitInOrder(objectGuid, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 7, 4, 6, 5, 2, 3, 0, 1 };
    recvData.ReadBytesSeq(objectGuid, bytesOrder);

    if (IS_UNIT_GUID(objectGuid))
    {
        if (Unit* unit = Unit::GetUnit(*(GetPlayer()), objectGuid))
            sLog->outError(LOG_FILTER_NETWORKIO, "Received CMSG_PAGE_TEXT_QUERY. Unit Entry: %u", unit->GetEntry());
    }
    else if (IS_GAMEOBJECT_GUID(objectGuid))
    {
        if (GameObject* go = GetPlayer()->GetMap()->GetGameObject(objectGuid))
            sLog->outError(LOG_FILTER_NETWORKIO, "Received CMSG_PAGE_TEXT_QUERY. Gameobject Entry: %u", go->GetEntry());
    }

    while (pageID)
    {
        PageText const* pageText = sObjectMgr->GetPageText(pageID);

        WorldPacket data(SMSG_PAGE_TEXT_QUERY_RESPONSE, 50);

        data.WriteBit(pageText != NULL);

        if (pageText)
        {
            std::string Text = pageText->Text;

            int loc_idx = GetSessionDbLocaleIndex();
            if (loc_idx >= 0)
                if (PageTextLocale const* player = sObjectMgr->GetPageTextLocale(pageID))
                    ObjectMgr::GetLocaleString(player->Text, loc_idx, Text);

            data.WriteBits(Text.size(), 12);

            data.FlushBits();
            if (Text.size())
                data.append(Text.c_str(), Text.size());

            data << uint32(pageID);
            data << uint32(pageText->NextPage);
        }

        data << uint32(pageID);
        
        if (pageText)
        {
            if (pageID == pageText->NextPage)
            {
                GameObject* go = GetPlayer()->GetMap()->GetGameObject(objectGuid);
                Unit* unit = Unit::GetUnit(*(GetPlayer()), objectGuid);

                if (go)
                    sLog->OutPandashan("WorldSession::HandlePageTextQueryOpcode, maybe infinite loop for GameObject:Entry[%u], PageID[%u].", go->GetEntry(), pageID);
                else if (unit)
                    sLog->OutPandashan("WorldSession::HandlePageTextQueryOpcode, maybe infinite loop for Unit:Entry[%u], PageID[%u].", unit->GetEntry(), pageID);
                else
                    sLog->OutPandashan("WorldSession::HandlePageTextQueryOpcode, maybe infinite loop for PageID[%u].", pageID);

                break;
            }

            pageID = pageText->NextPage;
        }

        SendPacket(&data);

        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_PAGE_TEXT_QUERY_RESPONSE");
    }
}

void WorldSession::HandleCorpseMapPositionQuery(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recv CMSG_CORPSE_MAP_POSITION_QUERY");
    ObjectGuid guid;

    uint8 bits[8] = { 2, 7, 4, 1, 0, 5, 3, 6 };
    recvData.ReadBitInOrder(guid, bits);

    recvData.FlushBits();

    uint8 bytes[8] = { 5, 2, 3, 0, 4, 1, 7, 6 };
    recvData.ReadBytesSeq(guid, bytes);

    return;

    // @TODO: Find me !
    WorldPacket data(SMSG_CORPSE_MAP_POSITION_QUERY_RESPONSE, 4+4+4+4);
    data << float(0);
    data << float(0);
    data << float(0);
    data << float(0);
    SendPacket(&data);
}

void WorldSession::HandleQuestPOIQuery(WorldPacket& recvData)
{
    uint32 count;
    count = recvData.ReadBits(22);
    recvData.FlushBits();

    if (count >= MAX_QUEST_LOG_SIZE)
    {
        recvData.rfinish();
        return;
    }

    std::list<uint32> questList;
    for (uint32 i = 0; i < count; ++i)
        questList.push_back(recvData.read<uint32>());


    WorldPacket data(SMSG_QUEST_POI_QUERY_RESPONSE, 4+(4+4)*count);
    data << uint32(count); // count
    data.WriteBits(count, 20);

    // Write bits
    for (auto itr : questList)
    {
        uint32 questId = itr;
        bool questOk = false;
        uint16 questSlot = _player->FindQuestSlot(questId);

        if (questSlot != MAX_QUEST_LOG_SIZE)
            questOk =_player->GetQuestSlotQuestId(questSlot) == questId;

        if (questOk)
        {
            QuestPOIVector const* POI = sObjectMgr->GetQuestPOIVector(questId);

            if (POI)
            {
                data.WriteBits(POI->size(), 18);
                for (QuestPOIVector::const_iterator itr = POI->begin(); itr != POI->end(); ++itr)
                    data.WriteBits(itr->points.size(), 21);
            }
            else
                data.WriteBits(0, 18);
        }
        else
            data.WriteBits(0, 18);
    }

    // Write bytes
    for (auto itr : questList)
    {
        uint32 questId = itr;
        bool questOk = false;
        uint16 questSlot = _player->FindQuestSlot(questId);

        if (questSlot != MAX_QUEST_LOG_SIZE)
            questOk =_player->GetQuestSlotQuestId(questSlot) == questId;

        if (questOk)
        {
            QuestPOIVector const* POI = sObjectMgr->GetQuestPOIVector(questId);

            if (POI)
            {
                for (QuestPOIVector::const_iterator itr = POI->begin(); itr != POI->end(); ++itr)
                {
                    data << uint32(itr->Unk4); 
                    data << uint32(itr->Unk3);
                    data << uint32(itr->Unk2);
                    data << uint32(itr->points.size());
                    data << uint32(itr->AreaId);

                    for (std::vector<QuestPOIPoint>::const_iterator itr2 = itr->points.begin(); itr2 != itr->points.end(); ++itr2)
                    {
                        data << int32(itr2->y); // POI point y
                        data << int32(itr2->x); // POI point x
                    }

                    data << int32(itr->ObjectiveIndex);
                    data << uint32(0);
                    data << uint32(itr->MapId);
                    data << uint32(0);
                    data << uint32(0);
                    data << uint32(itr->Id);
                }
                data << uint32(questId);
                data << uint32(POI->size());
            }
            else
            {
                data << uint32(questId); // quest ID
                data << uint32(0); // POI count
            }
        }
        else
        {
                data << uint32(questId); // quest ID
                data << uint32(0); // POI count
        }
    }

   /* for (uint32 i = 0; i < count; ++i)
    {
        uint32 questId;

        bool questOk = false;

        uint16 questSlot = _player->FindQuestSlot(questId);

        if (questSlot != MAX_QUEST_LOG_SIZE)
            questOk =_player->GetQuestSlotQuestId(questSlot) == questId;

        if (questOk)
        {
            QuestPOIVector const* POI = sObjectMgr->GetQuestPOIVector(questId);

            if (POI)
            {
                data << uint32(questId); // quest ID
                data << uint32(POI->size()); // POI count

                for (QuestPOIVector::const_iterator itr = POI->begin(); itr != POI->end(); ++itr)
                {
                    data << uint32(itr->Id);                // POI index
                    data << int32(itr->ObjectiveIndex);     // objective index
                    data << uint32(0);
                    data << uint32(itr->MapId);             // mapid
                    data << uint32(itr->AreaId);            // areaid
                    data << uint32(itr->Unk2);              // unknown
                    data << uint32(itr->Unk3);              // unknown
                    data << uint32(itr->Unk4);              // unknown
                    data << uint32(0);
                    data << uint32(0);
                    data << uint32(itr->points.size());     // POI points count

                    for (std::vector<QuestPOIPoint>::const_iterator itr2 = itr->points.begin(); itr2 != itr->points.end(); ++itr2)
                    {
                        data << int32(itr2->x); // POI point x
                        data << int32(itr2->y); // POI point y
                    }
                }
            }
            else
            {
                data << uint32(questId); // quest ID
                data << uint32(0); // POI count
            }
        }
        else
        {
            data << uint32(questId); // quest ID
            data << uint32(0); // POI count
        }
    }*/

    SendPacket(&data);
}
