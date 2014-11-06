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

enum NameQueryResponse
{
    NAME_QUERY_RESULT_OK    = 0,
    NAME_QUERY_RESULT_DENY  = 1,
    NAME_QUERY_RESULT_RETRY = 2,
};

void WorldSession::SendNameQueryOpcode(uint64 guid)
{
    Player* player = ObjectAccessor::FindPlayer(guid);
    CharacterNameData const* nameData = sWorld->GetCharacterNameData(GUID_LOPART(guid));

    WorldPacket data(SMSG_NAME_QUERY_RESPONSE);

    data << uint8(nameData ? NAME_QUERY_RESULT_OK : NAME_QUERY_RESULT_DENY);
    data.appendPackGUID(guid);

    if (nameData)
    {
        data.WriteBit(false);   ///< Is deleted
        data.WriteBits(nameData->m_name.size(), 6);

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

        data.FlushBits();

        if (DeclinedName const* names = (player ? player->GetDeclinedNames() : NULL))
        {
            for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; ++i)
                if (names->name[i].size())
                    data.append(names->name[i].c_str(), names->name[i].size());
        }

        data.appendPackGUID(player ? player->GetSession()->GetWoWAccountGUID()  : 0);
        data.appendPackGUID(player ? player->GetSession()->GetBNetAccountGUID() : 0);
        data.appendPackGUID(guid);

        data << uint32(g_RealmID);
        data << uint8(nameData->m_race);
        data << uint8(nameData->m_gender);
        data << uint8(nameData->m_class);
        data << uint8(nameData->m_level);

        data.WriteString(nameData->m_name);
    }

    SendPacket(&data);
}

void WorldSession::HandleNameQueryOpcode(WorldPacket& recvData)
{

    uint64 guid;

    recvData.readPackGUID(guid);
    bool hasVirtualRealmAddress = recvData.ReadBit();
    bool hasNativeRealmAddress = recvData.ReadBit();

    if (hasVirtualRealmAddress)
        uint32 VirtualRealmAddress = recvData.read<uint32>();

    if (hasNativeRealmAddress)
        uint32 NativeRealmAddress = recvData.read<uint32>();

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

        uint8 itemCount = 0;
        for (uint32 i = 0; i < MAX_CREATURE_QUEST_ITEMS; ++i)
            if (ci->questItems[i])
                itemCount++;                                // itemId[6], quest drop

        WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE);

        data << uint32(entry);                                                          // creature entry
        data.WriteBit(1);                                                               // has valid data
        data.FlushBits();

        data.WriteBits(SubName.size() ? SubName.size() + 1 : 0, 11);
        data.WriteBits(Unk505.size() ? Unk505.size() + 1 : 0, 11);
        data.WriteBits(ci->IconName.size() ? ci->IconName.size() + 1 : 0, 6);
        data.WriteBit(ci->RacialLeader);                                                // isRacialLeader
        data.WriteBits(Name.size() ? Name.size() + 1 : 0, 11);                          // Male

        for (int i = 0; i < 7; i++)
            data.WriteBits(0, 11);                          // Female and other Names - Never send it

        data.FlushBits();

        if (ci->Name.size())
            data << Name;                                   // Name

        data << uint32(ci->type_flags);                     // flags
        data << uint32(ci->type_flags2);                    // unknown meaning
        data << uint32(ci->type);                           // CreatureType.dbc
        data << uint32(ci->family);                         // CreatureFamily.dbc
        data << uint32(ci->rank);                           // Creature Rank (elite, boss, etc)
        data << uint32(ci->KillCredit[0]);                  // new in 3.1, kill credit
        data << uint32(ci->KillCredit[1]);                  // new in 3.1, kill credit
        data << uint32(ci->Modelid1);                       // Modelid1
        data << uint32(ci->Modelid2);                       // Modelid2
        data << uint32(ci->Modelid3);                       // Modelid3
        data << uint32(ci->Modelid4);                       // Modelid4
        data << float(ci->ModHealth);                       // dmg/hp modifier
        data << float(ci->ModMana);                         // dmg/mana modifier
        data << uint32(itemCount);                          // quest item count
        data << uint32(0);                                  // 6.0.1 unk
        data << uint32(ci->expansionUnknown);               // unknown meaning
        data << uint32(ci->movementId);                     // CreatureMovementInfo.dbc

        if (SubName.size())
            data << SubName;                                // Sub Name

        if (Unk505.size())
            data << Unk505;                                 // Unknow string since 5.0.5

        if (ci->IconName.size())
            data << ci->IconName;                           // Icon Name

        for (uint32 i = 0; i < MAX_CREATURE_QUEST_ITEMS && itemCount > 0; ++i)
        {
            if (ci->questItems[i])
            {
                data << uint32(ci->questItems[i]);
                itemCount--;
            }
        }

        SendPacket(&data);
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_CREATURE_QUERY_RESPONSE");
    }
    else
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_CREATURE_QUERY - NO CREATURE INFO! (ENTRY: %u)", entry);
        WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE, 4);
        data << uint32(entry | 0x80000000);
        data.WriteBit(0); // has no valid data
        data.FlushBits();

        SendPacket(&data);
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_CREATURE_QUERY_RESPONSE");
    }
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandleGameObjectQueryOpcode(WorldPacket& recvData)
{
    uint32 l_GobEntry;
    uint64 l_GobGUID;

    recvData >> l_GobEntry;
    recvData.readPackGUID(l_GobGUID);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_GAMEOBJECT_QUERY Entry: %u. ", l_GobEntry);

    ByteBuffer l_GobData;

    if (const GameObjectTemplate* l_GobInfo = sObjectMgr->GetGameObjectTemplate(l_GobEntry))
    {
        std::string l_Name;
        std::string l_IconeName;
        std::string l_CastBarCaption;

        l_Name              = l_GobInfo->name;
        l_IconeName         = l_GobInfo->IconName;
        l_CastBarCaption    = l_GobInfo->castBarCaption;

        int l_LocaleIndex = GetSessionDbLocaleIndex();

        if (l_LocaleIndex >= 0)
        {
            if (GameObjectLocale const* l_GobLocale = sObjectMgr->GetGameObjectLocale(l_GobEntry))
            {
                ObjectMgr::GetLocaleString(l_GobLocale->Name, l_LocaleIndex, l_Name);
                ObjectMgr::GetLocaleString(l_GobLocale->CastBarCaption, l_LocaleIndex, l_CastBarCaption);
            }
        }

        l_GobData << uint32(l_GobInfo->type);
        l_GobData << uint32(l_GobInfo->displayId);
        l_GobData << l_Name;
        l_GobData << "";
        l_GobData << "";
        l_GobData << "";
        l_GobData << l_IconeName;                                       // 2.0.3, string. Icon name to use instead of default icon for go's (ex: "Attack" makes sword)
        l_GobData << l_CastBarCaption;                                  // 2.0.3, string. Text will appear in Cast Bar when using GO (ex: "Collecting")
        l_GobData << "";

        for (int i = 0; i < MAX_GAMEOBJECT_DATA; i++)
            l_GobData << uint32(l_GobInfo->raw.data[i]);

        l_GobData << float(l_GobInfo->size);                            // go size

        uint8 l_QuestItemCount = 0;

        for (uint32 i = 0; i < MAX_GAMEOBJECT_QUEST_ITEMS; ++i)
            if (l_GobInfo->questItems[i])
                l_QuestItemCount++;

        l_GobData << uint8(l_QuestItemCount);

        for (int i = 0; i < MAX_GAMEOBJECT_QUEST_ITEMS && l_QuestItemCount > 0; i++)
        {
            if (l_GobInfo->questItems[i])
            {
                l_GobData << uint32(l_GobInfo->questItems[i]);          // itemId[6], quest drop
                l_QuestItemCount--;
            }
        }

        l_GobData << uint32(l_GobInfo->unkInt32);                       // 4.x, unknown
    }
    else
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_GAMEOBJECT_QUERY - Missing gameobject info for (GUID: %u, ENTRY: %u)", GUID_LOPART(l_GobGUID), l_GobEntry);
    }

    WorldPacket l_Response(SMSG_GAMEOBJECT_QUERY_RESPONSE);

    l_Response << uint32(l_GobEntry);

    l_Response.WriteBit(l_GobData.size() != 0);
    l_Response.FlushBits();

    l_Response << uint32(l_GobData.size());

    l_Response.append(l_GobData);

    SendPacket(&l_Response);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_GAMEOBJECT_QUERY_RESPONSE");
}

void WorldSession::HandleCorpseLocationFromClientQueryOpcode(WorldPacket& /*recvData*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_CORPSE_QUERY");

    Corpse* l_Corpse = GetPlayer()->GetCorpse();

    if (!l_Corpse)
    {

        WorldPacket l_Data(SMSG_CORPSE_LOCATION);

        l_Data.WriteBit(0);                                 ///< Valid
        l_Data.FlushBits();

        l_Data << uint32(0);                                ///< Map ID
        l_Data << float(0);                                 ///< X
        l_Data << float(0);                                 ///< Y
        l_Data << float(0);                                 ///< Z
        l_Data << uint32(0);                                ///< Actual map id
        l_Data.appendPackGUID(0);                           ///< Transport

        SendPacket(&l_Data);

        return;
    }

    uint32 l_MapID       = l_Corpse->GetMapId();
    uint32 l_CorpseMapID = l_MapID;
    float l_X = l_Corpse->GetTransport() ? l_Corpse->GetTransOffsetX() : l_Corpse->GetPositionX();
    float l_Y = l_Corpse->GetTransport() ? l_Corpse->GetTransOffsetY() : l_Corpse->GetPositionY();
    float l_Z = l_Corpse->GetTransport() ? l_Corpse->GetTransOffsetZ() : l_Corpse->GetPositionZ();

    /// If corpse at different map
    if (l_MapID != m_Player->GetMapId())
    {
        /// Search entrance map for proper show entrance
        if (const MapEntry * l_CorpseMapEntry = sMapStore.LookupEntry(l_MapID))
        {
            if (l_CorpseMapEntry->IsDungeon() && l_CorpseMapEntry->CorpseMapID >= 0)
            {
                /// If corpse map have entrance
                if (Map const* entranceMap = sMapMgr->CreateBaseMap(l_CorpseMapEntry->CorpseMapID))
                {
                    l_MapID = l_CorpseMapEntry->CorpseMapID;
                    l_X = l_CorpseMapEntry->CorpseX;
                    l_Y = l_CorpseMapEntry->CorpseY;
                    l_Z = entranceMap->GetHeight(GetPlayer()->GetPhaseMask(), l_X, l_Y, MAX_HEIGHT);
                }
            }
        }
    }

    WorldPacket l_Data(SMSG_CORPSE_LOCATION);

    l_Data.WriteBit(1);                                 ///< Valid
    l_Data.FlushBits();

    l_Data << uint32(l_CorpseMapID);                    ///< Map ID
    l_Data << float(l_X);                               ///< X
    l_Data << float(l_Y);                               ///< Y
    l_Data << float(l_Z);                               ///< Z
    l_Data << uint32(l_MapID);                          ///< Actual map id
    l_Data.appendPackGUID(l_Corpse->GetTransGUID());    ///< Transport

    SendPacket(&l_Data);
}

void WorldSession::HandleCemeteryListOpcode(WorldPacket& /*recvData*/)
{
    GetPlayer()->SendCemeteryList(false);
}

void WorldSession::HandleForcedReactionsOpcode(WorldPacket& /*recvData*/)
{
    GetPlayer()->GetReputationMgr().SendForceReactions();
}

void WorldSession::HandleNpcTextQueryOpcode(WorldPacket& p_Packet)
{
    uint32 l_TextID;
    uint64 l_Guid;

    p_Packet >> l_TextID;
    p_Packet.readPackGUID(l_Guid);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_NPC_TEXT_QUERY ID '%u'", l_TextID);

    GetPlayer()->SetSelection(l_Guid);

    GossipText const* pGossip = sObjectMgr->GetGossipText(l_TextID);

    bool l_Allow = true;

    WorldPacket l_Data(SMSG_NPC_TEXT_UPDATE, 100);          // guess size
    l_Data << l_TextID;
    l_Data.WriteBit(l_Allow);     // unk bit (true on retail sniff)
    l_Data.FlushBits();

    ByteBuffer l_Buffer;

    /// Text probability
    for (size_t l_I = 0; l_I < MAX_GOSSIP_TEXT_OPTIONS; l_I++)
        l_Buffer << float(pGossip ? pGossip->Options[l_I].Probability : 0);

    l_Buffer << uint32(l_TextID);

    /// Text ID
    for (size_t l_I = 0; l_I < MAX_GOSSIP_TEXT_OPTIONS - 1; l_I++)
        l_Buffer << uint32(0);

    l_Data << uint32(l_Buffer.size());
    l_Data.append(l_Buffer);

    SendPacket(&l_Data);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_NPC_TEXT_UPDATE");
}

#define DEFAULT_GREETINGS_GOSSIP      68

void WorldSession::SendBroadcastTextDb2Reply(uint32 p_Entry)
{
    ByteBuffer l_ReplyContent;

    WorldPacket data(SMSG_DB_REPLY);
    data << uint32(DB2_REPLY_BROADCAST_TEXT);
    data << uint32(p_Entry);
    data << uint32(sObjectMgr->GetHotfixDate(p_Entry, DB2_REPLY_BROADCAST_TEXT));

    //////////////////////////////////////////////////////////////////////////

    GossipText const* l_Gossip = sObjectMgr->GetGossipText(p_Entry);

    if (!l_Gossip)
        l_Gossip = sObjectMgr->GetGossipText(DEFAULT_GREETINGS_GOSSIP);

    std::string l_DefaultText = "Greetings, $N";
    std::string l_Text1 = l_Gossip ? l_Gossip->Options[0].Text_0 : l_DefaultText;
    std::string l_Text2 = l_Gossip ? l_Gossip->Options[0].Text_1 : l_DefaultText;

    int l_LocaleIndex = GetSessionDbLocaleIndex();
    if (l_LocaleIndex >= 0 && l_LocaleIndex <= MAX_LOCALES)
    {
        if (NpcTextLocale const* l_NpcTextLocale = sObjectMgr->GetNpcTextLocale(p_Entry))
        {
            ObjectMgr::GetLocaleString(l_NpcTextLocale->Text_0[l_LocaleIndex], l_LocaleIndex, l_Text1);
            ObjectMgr::GetLocaleString(l_NpcTextLocale->Text_1[l_LocaleIndex], l_LocaleIndex, l_Text2);
        }
    }

    l_ReplyContent << uint32(p_Entry);
    l_ReplyContent << uint32(l_Gossip != 0 ? l_Gossip->Options[0].Language : LANG_UNIVERSAL);
    l_ReplyContent << uint16(l_Text1.size() ? l_Text1.size() + 1 : 0);

    if (l_Text1.size())
        l_ReplyContent << std::string(l_Text1);

    l_ReplyContent << uint16(l_Text2.size() ? l_Text2.size() + 1 : 0);

    if (l_Text2.size())
        l_ReplyContent << std::string(l_Text2);

    if (l_Gossip)
    {
        for (int l_J = 0; l_J < MAX_GOSSIP_TEXT_EMOTES; ++l_J)
            l_ReplyContent << uint32(l_Gossip->Options[0].Emotes[l_J]._Emote);
        for (int l_J = 0; l_J < MAX_GOSSIP_TEXT_EMOTES; ++l_J)
            l_ReplyContent << uint32(l_Gossip->Options[0].Emotes[l_J]._Delay);
    }
    else
    {
        for (int l_J = 0; l_J < MAX_GOSSIP_TEXT_EMOTES; ++l_J)
        {
            l_ReplyContent << uint32(0);
            l_ReplyContent << uint32(0);
        }
    }

    l_ReplyContent << uint32(0);
    l_ReplyContent << uint32(0);
    l_ReplyContent << uint32(0x01);   /// unk

    //////////////////////////////////////////////////////////////////////////

    data << uint32(l_ReplyContent.size());
    data.append(l_ReplyContent);

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
            pageID = pageText->NextPage;
        else
            pageID = 0;

        SendPacket(&data);

        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Sent SMSG_PAGE_TEXT_QUERY_RESPONSE");
    }
}

void WorldSession::HandleCorpseTransportQueryOpcode(WorldPacket & p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recv CMSG_QUERY_CORPSE_TRANSPORT");

    uint64 l_TransportGUID = 0;

    p_Packet.readPackGUID(l_TransportGUID);

    GameObject * l_TransportGOB = HashMapHolder<GameObject>::Find(l_TransportGUID);

    if (!l_TransportGOB)
        return;

    if (!l_TransportGOB->IsTransport())
        return;

    WorldPacket data(SMSG_CORPSE_TRANSPORT_QUERY, 4+4+4+4);
    data << float(l_TransportGOB->GetPositionX());
    data << float(l_TransportGOB->GetPositionY());
    data << float(l_TransportGOB->GetPositionZ());
    data << float(l_TransportGOB->GetOrientation());

    SendPacket(&data);
}

void WorldSession::HandleQuestPOIQuery(WorldPacket& recvData)
{
    uint32 l_Count;

    recvData >> l_Count;

    if (l_Count >= MAX_QUEST_LOG_SIZE)
    {
        recvData.rfinish();
        return;
    }

    std::list<uint32> questList;
    for (uint32 i = 0; i < l_Count; ++i)
        questList.push_back(recvData.read<uint32>());


    WorldPacket data(SMSG_QUEST_POIQUERY_RESPONSE, 4+(4+4)*l_Count);
    data << uint32(l_Count); // count
    data << uint32(l_Count);

    for (auto itr : questList)
    {
        uint32 questId = itr;
        bool questOk = false;
        uint16 questSlot = m_Player->FindQuestSlot(questId);

        if (questSlot != MAX_QUEST_LOG_SIZE)
            questOk =m_Player->GetQuestSlotQuestId(questSlot) == questId;

        if (questOk)
        {
            QuestPOIVector const* POI = sObjectMgr->GetQuestPOIVector(questId);

            if (POI)
            {
                data << uint32(questId); // quest ID
                data << uint32(POI->size()); // POI count
                data << uint32(POI->size()); // POI count

                for (QuestPOIVector::const_iterator itr = POI->begin(); itr != POI->end(); ++itr)
                {
                    data << uint32(itr->Id);
                    data << int32(itr->ObjectiveIndex);
                    data << uint32(0);
                    data << uint32(0);
                    data << uint32(itr->MapId);
                    data << uint32(itr->AreaId);
                    data << uint32(0);
                    data << uint32(0);
                    data << uint32(itr->Unk4);
                    data << uint32(0);
                    data << uint32(0);
                    data << uint32(itr->points.size());
                    data << uint32(0);

                    data << uint32(itr->points.size());

                    for (std::vector<QuestPOIPoint>::const_iterator itr2 = itr->points.begin(); itr2 != itr->points.end(); ++itr2)
                    {
                        data << int32(itr2->x); // POI point x
                        data << int32(itr2->y); // POI point y
                    }

                    //data << uint32(itr->Unk3);
                    //data << uint32(itr->Unk2);
                }
            }
            else
            {
                data << uint32(questId); // quest ID
                data << uint32(0); // POI count
                data << uint32(0); // POI count
            }
        }
        else
        {
            data << uint32(questId); // quest ID
            data << uint32(0); // POI count
            data << uint32(0); // POI count
        }
    }

    SendPacket(&data);
}
