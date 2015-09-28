////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "gamePCH.h"
#include "LFGListMgr.h"

void WorldSession::HandleRequestLfgListBlacklist(WorldPacket& p_RecvData)
{
    WorldPacket l_Data(SMSG_LFG_LIST_UPDATE_BLACKLIST, 4);
    l_Data << uint32(0);    /// Activity and Reason loop - We dont need it
    SendPacket(&l_Data);
}

void WorldSession::HandleLfgListJoin(WorldPacket& p_RecvData)
{
    uint32 l_NameLen, l_CommentLen, l_VoiceChatLen, l_ActivityId;
    std::string l_Name, l_Comment, l_VoiceChat;
    float l_ItemLevel;
    bool l_AutoAccept;

    p_RecvData >> l_ActivityId;
    p_RecvData >> l_ItemLevel;
    l_NameLen = p_RecvData.ReadBits(8);
    l_CommentLen = p_RecvData.ReadBits(11);
    l_VoiceChatLen = p_RecvData.ReadBits(8);
    l_AutoAccept = p_RecvData.ReadBit();
    l_Name = p_RecvData.ReadString(l_NameLen);
    l_Comment = p_RecvData.ReadString(l_CommentLen);
    l_VoiceChat = p_RecvData.ReadString(l_VoiceChatLen);

    /// Need to allocate it after reading in case of an reading error (try catch ...)
    LFGListEntry* l_List = new LFGListEntry();
    l_List->m_ActivityEntry = sGroupFinderActivityStore.LookupEntry(l_ActivityId);
    l_List->m_RequiredItemLevel = l_ItemLevel;
    l_List->m_AutoAcceptInvites = l_AutoAccept;
    l_List->m_Name = l_Name;
    l_List->m_Comment = l_Comment;
    l_List->m_VoiceChat = l_VoiceChat;
    l_List->m_CreationTime = time(NULL);
    l_List->m_Group = nullptr;

    if (sLFGListMgr->CanInsert(l_List, GetPlayer(), true))
        sLFGListMgr->Insert(l_List, GetPlayer());
    else
        delete l_List;
}

void WorldSession::BuildLfgListRideTicket(WorldPacket* p_Data, LFGListEntry const* p_Entry)
{
    p_Data->appendPackGUID(p_Entry->m_Group ? p_Entry->m_Group->GetGUID() : 0);
    *p_Data << uint32(p_Entry->m_Group ? p_Entry->m_Group->GetLowGUID() : 0);
    *p_Data << uint32(7);    /// LFG_LIST = 7
    *p_Data << uint32(p_Entry->m_CreationTime);
}

void WorldSession::BuildLfgListJoinRequest(WorldPacket* p_Data, LFGListEntry const* p_Entry)
{
    BuildLfgListRideTicket(p_Data, p_Entry);
    *p_Data << uint32(p_Entry->m_ActivityEntry->ID);
    *p_Data << float(p_Entry->m_RequiredItemLevel);

    p_Data->WriteBits(p_Entry->m_Name.size(), 8);
    p_Data->WriteBits(p_Entry->m_Comment.size(), 11);
    p_Data->WriteBits(p_Entry->m_VoiceChat.size(), 8);
    p_Data->WriteBit(p_Entry->m_AutoAcceptInvites);
    p_Data->FlushBits();

    p_Data->WriteString(p_Entry->m_Name);
    p_Data->WriteString(p_Entry->m_Comment);
    p_Data->WriteString(p_Entry->m_VoiceChat);
}

void WorldSession::SendLfgListJoinResult(LFGListEntry const* p_Entry, uint8 p_Error)
{
    WorldPacket l_Data(SMSG_LFG_LIST_JOIN_RESULT);
    BuildLfgListRideTicket(&l_Data, p_Entry);
    l_Data << uint8(p_Error);
    l_Data << uint8(0);         ///< Args??
    SendPacket(&l_Data);
}

void WorldSession::BuildLfgListQueueUpdate(WorldPacket* p_Data, LFGListEntry const* p_Entry, bool p_Listed)
{
    p_Data->Initialize(SMSG_LFG_LIST_UPDATE_STATUS);
    BuildLfgListJoinRequest(p_Data, p_Entry);
    *p_Data << uint32(0);       ///< Unk
    *p_Data << uint8(6);        ///< Reason
    p_Data->WriteBit(p_Listed);
    p_Data->FlushBits();
}

void WorldSession::HandleLfgListUpdateRequest(WorldPacket& p_RecvData)
{

}

void WorldSession::HandleLfgListLeave(WorldPacket& p_RecvData)
{
    uint64 l_Guid;
    uint32 l_GuidLow;

    p_RecvData.readPackGUID(l_Guid);
    p_RecvData >> l_GuidLow;
    p_RecvData.read_skip<uint32>(); ///< Type
    p_RecvData.read_skip<uint32>(); ///< Timestamp

    sLFGListMgr->Remove(l_Guid, GetPlayer());
}

void WorldSession::HandleLfgListSearch(WorldPacket& p_RecvData)
{
    uint32 l_ActivityCategory, l_ActivitySubCategory, unk3, l_UnkGuidLoop;
    uint32 l_FilterLen = p_RecvData.ReadBits(6);
    uint32 l_BlacklistCount = p_RecvData.ReadBits(7);

    p_RecvData >> l_ActivityCategory >> l_ActivitySubCategory >> unk3 >> l_UnkGuidLoop;

    std::string l_FilterString = p_RecvData.ReadString(l_FilterLen);

    uint64 l_GUIDs;
    for (uint32 l_I = 0; l_I < l_UnkGuidLoop; l_I++)
        p_RecvData.readPackGUID(l_GUIDs);

    /// Not used nor implemented    
    for (uint32 l_I = 0; l_I < l_BlacklistCount; l_I++)
    {
        uint32 l_ActivityId, l_Reason;
        p_RecvData >> l_ActivityId >>  l_Reason;
    }

    SendLfgSearchResponse(l_ActivityCategory, l_ActivitySubCategory, l_FilterString);
}

void WorldSession::SendLfgSearchResponse(uint32 p_ActivityCategory, uint32 p_ActivitySubCategory, std::string p_FilterString)
{
    WorldPacket l_Data(SMSG_LFG_LIST_SEARCH_RESULT);

    if (!sGroupFinderCategoryStore.LookupEntry(p_ActivityCategory))
    {
        l_Data << uint16(0);    ///< unk
        l_Data << uint32(0);
        SendPacket(&l_Data);
        return;
    }

    std::list<LFGListEntry const*> l_LFGList = sLFGListMgr->GetFilteredList(p_ActivityCategory, p_ActivitySubCategory, p_FilterString);

    l_Data << uint16(l_LFGList.size()); ///< Weird Blizz is weird blizz
    l_Data << uint32(l_LFGList.size());

    for (auto& l_LFGListEntry : l_LFGList)
    {
        Group* l_Group = l_LFGListEntry->m_Group;
        uint64 l_LeaderGuid = l_Group->GetLeaderGUID(); // Blizz sends 4x same GUID
        const Group::MemberSlotList l_Members = l_Group->GetMemberSlots();
        uint32 l_MemberCount = l_Members.size();

        BuildLfgListJoinRequest(&l_Data, l_LFGListEntry);
        l_Data.appendPackGUID(l_LeaderGuid);
        l_Data.appendPackGUID(l_LeaderGuid);
        l_Data.appendPackGUID(l_LeaderGuid);
        l_Data.appendPackGUID(l_LeaderGuid);

        l_Data << uint32(0);        ///< Unk
        l_Data << uint32(0);        ///< Unk Guid loop
        l_Data << uint32(0);        ///< Unk Guid loop
        l_Data << uint32(0);        ///< Friends in group
        l_Data << uint32(l_MemberCount);
        l_Data << uint32(0);        ///< Unk
        l_Data << uint32(time(NULL) - l_LFGListEntry->m_CreationTime);

        for (auto& l_Member : l_Members)
        {
            l_Data << uint8(l_Member.playerClass);
            l_Data << uint8(Player::GetRoleBySpecializationId(l_Member.specID));
        }
    }

    SendPacket(&l_Data);
}
