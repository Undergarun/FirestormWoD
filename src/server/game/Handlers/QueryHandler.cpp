////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

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

#ifndef CROSS
void WorldSession::SendNameQueryOpcode(uint64 guid)
#else /* CROSS */
void WorldSession::SendNameQueryOpcode(uint64 guid, bool atLeave)
#endif /* CROSS */
{
#ifndef CROSS
    Player* player = ObjectAccessor::FindPlayer(guid);
    CharacterInfo  const* nameData = sWorld->GetCharacterInfo(GUID_LOPART(guid));
#else /* CROSS */
    Player* player = ObjectAccessor::FindPlayerInOrOutOfWorld(guid);
    InterRealmClient* playerClient = player ? player->GetSession()->GetInterRealmClient() : nullptr;
#endif /* CROSS */

    WorldPacket data(SMSG_NAME_QUERY_RESPONSE);

#ifndef CROSS
    data << uint8(nameData ? NAME_QUERY_RESULT_OK : NAME_QUERY_RESULT_DENY);
#else /* CROSS */
    data << uint8(playerClient ? NAME_QUERY_RESULT_OK : NAME_QUERY_RESULT_DENY);
#endif /* CROSS */
    data.appendPackGUID(guid);

#ifndef CROSS
    if (nameData)
#else /* CROSS */
    if (playerClient)
#endif /* CROSS */
    {
        data.WriteBit(false);   ///< Is deleted
#ifndef CROSS
        data.WriteBits(nameData->Name.size(), 6);
#else /* CROSS */
        data.WriteBits(strlen(player->GetName()), 6);
#endif /* CROSS */

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

#ifndef CROSS
        data << uint32(g_RealmID);
        data << uint8(nameData->Race);
        data << uint8(nameData->Sex);
        data << uint8(nameData->Class);
        data << uint8(nameData->Level);
#else /* CROSS */
        data << uint32(playerClient->GetRealmId());
        data << uint8(player->getRace());
        data << uint8(player->getGender());
        data << uint8(player->getClass());
        data << uint8(player->getLevel());
#endif /* CROSS */

#ifndef CROSS
        data.WriteString(nameData->Name);
#else /* CROSS */
        data.WriteString(player->GetName());
#endif /* CROSS */
    }

    SendPacket(&data);
}

void WorldSession::HandleNameQueryOpcode(WorldPacket& recvData)
{
    uint64 guid;
    recvData.readPackGUID(guid);

    SendNameQueryOpcode(guid);
}

void WorldSession::HandleQueryTimeOpcode(WorldPacket& /*recvData*/)
{
    SendQueryTimeResponse();
}

void WorldSession::SendQueryTimeResponse()
{
    WorldPacket l_Data(SMSG_QUERY_TIME_RESPONSE, 4+4);
    l_Data << uint32(sWorld->GetNextDailyQuestsResetTime() - time(NULL));
    l_Data << uint32(time(NULL));
    SendPacket(&l_Data);
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandleCreatureQueryOpcode(WorldPacket& recvData)
{
    uint32 entry;
    recvData >> entry;

    if (CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(entry))
    {

        std::string Name, SubName, l_FemaleName, SubNameAlt;
        Name = creatureInfo->Name;
        SubName = creatureInfo->SubName;
        l_FemaleName = creatureInfo->FemaleName;
        SubNameAlt = "";

        LocaleConstant locale = GetSessionDbLocaleIndex();
        if (locale >= 0)
        {
            if (CreatureLocale const* creatureLocale = sObjectMgr->GetCreatureLocale(entry))
            {
                ObjectMgr::GetLocaleString(creatureLocale->Name, locale, Name);
                ObjectMgr::GetLocaleString(creatureLocale->SubName, locale, SubName);
                ObjectMgr::GetLocaleString(creatureLocale->l_FemaleName, locale, l_FemaleName);
            }
        }

        uint8 itemCount = 0;
        for (uint32 i = 0; i < MAX_CREATURE_QUEST_ITEMS; ++i)
            if (creatureInfo->questItems[i])
                itemCount++;                                           ///< itemId[6], quest drop

        WorldPacket data(SMSG_QUERY_CREATURE_RESPONSE, 1 * 1024);

        data << uint32(entry);                                         ///< Creature entry
        data.WriteBit(1);                                              ///< Has valid data
        data.FlushBits();

        data.WriteBits(SubName.size() ? SubName.size() + 1 : 0, 11);
        data.WriteBits(SubNameAlt.size() ? SubNameAlt.size() + 1 : 0, 11);
        data.WriteBits(creatureInfo->IconName.size() ? creatureInfo->IconName.size() + 1 : 0, 6);
        data.WriteBit(creatureInfo->RacialLeader);                     ///< isRacialLeader
        data.WriteBits(Name.size() ? Name.size() + 1 : 0, 11);         ///< Male
        data.WriteBits(l_FemaleName.size() ? l_FemaleName.size() + 1 : 0, 11);        ///< Female

        for (int i = 0; i < 6; i++)
            data.WriteBits(0, 11);                                      ///< Female and other Names - Never send it

        data.FlushBits();

        if (Name.size())
            data << Name;                                               ///< Name
        if (l_FemaleName.size())
            data << l_FemaleName;                                       ///< Name

        data << uint32(creatureInfo->type_flags);                       ///< Flags
        data << uint32(creatureInfo->type_flags2);                      ///< Unknown meaning
        data << uint32(creatureInfo->type);                             ///< CreatureType.dbc
        data << uint32(creatureInfo->family);                           ///< CreatureFamily.dbc
        data << uint32(creatureInfo->rank);                             ///< Creature Rank (elite, boss, etc)
        data << uint32(creatureInfo->KillCredit[0]);                    ///< Kill credit
        data << uint32(creatureInfo->KillCredit[1]);                    ///< Kill credit
        data << uint32(creatureInfo->Modelid1);                         ///< Modelid1
        data << uint32(creatureInfo->Modelid2);                         ///< Modelid2
        data << uint32(creatureInfo->Modelid3);                         ///< Modelid3
        data << uint32(creatureInfo->Modelid4);                         ///< Modelid4
        data << float(creatureInfo->ModHealth);                         ///< HP modifier
        data << float(creatureInfo->ModMana);                           ///< Mana modifier
        data << uint32(itemCount);                                      ///< Quest item count
        data << uint32(creatureInfo->movementId);                       ///< CreatureMovementInfo.dbc
        data << uint32(creatureInfo->RequiredExpansion);                ///< RequiredExpansion
        data << uint32(creatureInfo->TrackingQuestID);                  ///< QuestTrackingId

        if (SubName.size())
            data << SubName;                                            ///< Sub Name

        if (l_FemaleName.size())
            data << l_FemaleName;                                       ///< Female Name

        if (creatureInfo->IconName.size())
            data << creatureInfo->IconName;                             ///< Icon Name

        for (uint32 i = 0; i < MAX_CREATURE_QUEST_ITEMS && itemCount > 0; ++i)
        {
            if (creatureInfo->questItems[i])
            {
                data << uint32(creatureInfo->questItems[i]);
                itemCount--;
            }
        }

        SendPacket(&data);
    }
    else
    {
        WorldPacket data(SMSG_QUERY_CREATURE_RESPONSE, 4 + 1);
        data << uint32(entry | 0x80000000);
        data.WriteBit(0);                               ///< Has no valid data
        data.FlushBits();

        SendPacket(&data);
    }
}

/// Only _static_ data is sent in this packet !!!
void WorldSession::HandleGameObjectQueryOpcode(WorldPacket& recvData)
{
    uint32 l_GobEntry;
    uint64 l_GobGUID;

    recvData >> l_GobEntry;
    recvData.readPackGUID(l_GobGUID);

    ByteBuffer l_GobData(2 * 1024);

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

    WorldPacket l_Response(SMSG_GAMEOBJECT_QUERY_RESPONSE, 4 + 1 + 4 + l_GobData.size());

    l_Response << uint32(l_GobEntry);

    l_Response.WriteBit(l_GobData.size() != 0);
    l_Response.FlushBits();

    l_Response << uint32(l_GobData.size());

    l_Response.append(l_GobData);

    SendPacket(&l_Response);
}

void WorldSession::HandleCorpseLocationFromClientQueryOpcode(WorldPacket& /*recvData*/)
{
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

    GetPlayer()->SetSelection(l_Guid);

    GossipText const* pGossip = sObjectMgr->GetGossipText(l_TextID); ///< pGossip is never read 01/18/16

    bool l_Allow = true;

    WorldPacket l_Data(SMSG_NPC_TEXT_UPDATE, 100);          // guess size
    l_Data << l_TextID;
    l_Data.WriteBit(l_Allow);     // unk bit (true on retail sniff)
    l_Data.FlushBits();

    ByteBuffer l_Buffer;

    /// Text probability
    for (size_t l_I = 0; l_I < MAX_GOSSIP_TEXT_OPTIONS; l_I++)
        l_Buffer << float(0);

        /// Look like that make random bug on gossip
        //l_Buffer << float(pGossip ? pGossip->Options[l_I].Probability : 0);

    l_Buffer << uint32(l_TextID);

    /// Text ID
    for (size_t l_I = 0; l_I < MAX_GOSSIP_TEXT_OPTIONS - 1; l_I++)
        l_Buffer << uint32(0);

    l_Data << uint32(l_Buffer.size());
    l_Data.append(l_Buffer);

    SendPacket(&l_Data);
}

#define DEFAULT_GREETINGS_GOSSIP      68

void WorldSession::SendBroadcastTextDb2Reply(uint32 p_Entry)
{
    ByteBuffer l_ReplyContent;

    WorldPacket data(SMSG_DB_REPLY, 2 * 1024);
    data << uint32(DB2_REPLY_BROADCAST_TEXT);
    data << uint32(p_Entry);
    data << uint32(sObjectMgr->GetHotfixDate(p_Entry, DB2_REPLY_BROADCAST_TEXT));
    data.WriteBit(1);                                                         ///< Found ???

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
            ObjectMgr::GetLocaleString(l_NpcTextLocale->Text_0[0], l_LocaleIndex, l_Text1);
            ObjectMgr::GetLocaleString(l_NpcTextLocale->Text_1[0], l_LocaleIndex, l_Text2);
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
void WorldSession::HandlePageTextQueryOpcode(WorldPacket& p_Packet)
{
    uint64 l_ItemGUID   = 0;
    uint32 l_PageTextID = 0;

    p_Packet >> l_PageTextID;
    p_Packet.readPackGUID(l_ItemGUID);

    while (l_PageTextID)
    {
        PageText const* l_PageText = sObjectMgr->GetPageText(l_PageTextID);

        WorldPacket l_Data(SMSG_PAGE_TEXT_QUERY_RESPONSE, 2 * 1024);
        l_Data << uint32(l_PageTextID);                             ///< Page Text ID
        l_Data.WriteBit(l_PageText != NULL);                        ///< Allow
        l_Data.FlushBits();

        if (l_PageText)
        {
            std::string l_Text = l_PageText->Text;

            int l_Locale = GetSessionDbLocaleIndex();
            if (l_Locale >= 0)
            {
                if (PageTextLocale const* l_PageTxtLocale = sObjectMgr->GetPageTextLocale(l_PageTextID))
                    ObjectMgr::GetLocaleString(l_PageTxtLocale->Text, l_Locale, l_Text);
            }

            l_Data << uint32(l_PageTextID);                         ///< ID
            l_Data << uint32(l_PageText->NextPage);                 ///< Next Page ID

            l_Data.WriteBits(l_Text.size(), 12);                    ///< Text
            l_Data.FlushBits();

            l_Data.WriteString(l_Text);                             ///< Text
        }

        if (l_PageText)
            l_PageTextID = l_PageText->NextPage;
        else
            l_PageTextID = 0;

        SendPacket(&l_Data);
    }
}

void WorldSession::HandleCorpseTransportQueryOpcode(WorldPacket & p_Packet)
{
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

void WorldSession::HandleQuestPOIQuery(WorldPacket& p_Packet)
{
    uint32 l_Count;

    p_Packet >> l_Count;

    if (l_Count >= MAX_QUEST_LOG_SIZE)
    {
        p_Packet.rfinish();
        return;
    }

    std::list<uint32> l_QuestList;

    for (uint32 i = 0; i < l_Count; ++i)
        l_QuestList.push_back(p_Packet.read<uint32>());

    WorldPacket l_Data(SMSG_QUEST_POIQUERY_RESPONSE, 15 * 1024);
    l_Data << uint32(l_Count); // count
    l_Data << uint32(l_Count);

    for (auto l_CurrentQuest : l_QuestList)
    {
        uint32 l_QuestID = l_CurrentQuest;
        uint16 l_QuestSlot = m_Player->FindQuestSlot(l_QuestID);
        bool l_QuestOK = false;

        if (l_QuestSlot != MAX_QUEST_LOG_SIZE)
            l_QuestOK =m_Player->GetQuestSlotQuestId(l_QuestSlot) == l_QuestID;

        if (l_QuestOK)
        {
            QuestPOIVector const* l_POIs = sObjectMgr->GetQuestPOIVector(l_QuestID);

            if (l_POIs)
            {
                l_Data << uint32(l_QuestID);
                l_Data << uint32(l_POIs->size());     ///< Num Blobs

                const Quest * l_Quest = sObjectMgr->GetQuestTemplate(l_QuestID);

                for (QuestPOIVector::const_iterator l_It = l_POIs->begin(); l_It != l_POIs->end(); ++l_It)
                {
                    const QuestObjective * l_Objective = l_Quest->GetQuestObjectiveXIndex(l_It->ObjectiveIndex);

                    uint32 l_QuestObjectiveID   = 0;
                    uint32 l_QuestObjectID      = 0;

                    if (l_Objective)
                    {
                        l_QuestObjectiveID = l_Objective->ID;
                        l_QuestObjectID = l_Objective->ObjectID;
                    }

                    l_Data << uint32(l_It->Id);                   ///< BlobIndex
                    l_Data << int32(l_It->ObjectiveIndex);        ///< Objective Index
                    l_Data << uint32(l_QuestObjectiveID);         ///< Quest Objective ID
                    l_Data << uint32(l_QuestObjectID);            ///< Quest Object ID
                    l_Data << uint32(l_It->MapID);                ///< Map ID
                    l_Data << uint32(l_It->WorldMapAreaID);       ///< World Map Area ID
                    l_Data << uint32(l_It->Floor);                ///< Floor
                    l_Data << uint32(l_It->Priority);             ///< Priority
                    l_Data << uint32(l_It->Flags);                ///< Flags
                    l_Data << uint32(l_It->WorldEffectID);        ///< World Effect ID
                    l_Data << uint32(l_It->PlayerConditionID);    ///< Player Condition ID
                    l_Data << uint32(0);                          ///< Unk Wod
                    l_Data << uint32(l_It->Points.size());

                    for (std::vector<QuestPOIPoint>::const_iterator l_PointIT = l_It->Points.begin(); l_PointIT != l_It->Points.end(); ++l_PointIT)
                    {
                        l_Data << int32(l_PointIT->x);            ///< X
                        l_Data << int32(l_PointIT->y);            ///< Y
                    }

                }
            }
            else
            {
                l_Data << uint32(l_QuestID);  ///< Quest ID
                l_Data << uint32(0);          ///< Num Blobs
            }
        }
        else
        {
            l_Data << uint32(l_QuestID);      ///< Quest ID
            l_Data << uint32(0);              ///< Num Blobs
        }
    }

    SendPacket(&l_Data);
}

void WorldSession::HandleDBQueryBulk(WorldPacket& p_RecvPacket)
{
    uint32 l_Type = 0;
    uint32 l_Count = 0;

    p_RecvPacket >> l_Type;
    l_Count = p_RecvPacket.ReadBits(13);

    DB2StorageBase* l_DB2Store = sDB2PerHash[l_Type];

    for (uint32 l_I = 0; l_I < l_Count; ++l_I)
    {
        uint64 l_GUID;
        uint32 l_Entry;

        p_RecvPacket.readPackGUID(l_GUID);
        p_RecvPacket >> l_Entry;

        /// Specific case, localized string not yet supported by the system
        if (l_Type == DB2_REPLY_BROADCAST_TEXT)
        {
            SendBroadcastTextDb2Reply(l_Entry);
        }
        else if (l_DB2Store)
        {
            ByteBuffer l_ResponseData(2 * 1024);
            if (l_DB2Store->WriteRecord(l_Entry, l_ResponseData, GetSessionDbLocaleIndex()))
            {
                WorldPacket l_Data(SMSG_DB_REPLY, 4 + 4 + 4 + 4 + l_ResponseData.size());
                l_Data << uint32(l_Type);
                l_Data << uint32(l_Entry);
                l_Data << uint32(sObjectMgr->GetHotfixDate(l_Entry, l_Type));
                l_Data.WriteBit(1);                                                         ///< Found ???
                l_Data << uint32(l_ResponseData.size());
                l_Data.append(l_ResponseData);

                SendPacket(&l_Data);
            }
            else
            {
                WorldPacket l_Data(SMSG_DB_REPLY, 4 + 4 + 4 + 4);
                l_Data << uint32(l_Type);
                l_Data << uint32(-int32(l_Entry));
                l_Data << uint32(time(NULL));
                l_Data.WriteBit(0);                                                         ///< Found ???
                l_Data << uint32(0);

                SendPacket(&l_Data);
            }
        }
    }
}
