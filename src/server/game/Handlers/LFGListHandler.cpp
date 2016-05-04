////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "gamePCH.h"
#include "LFGListMgr.h"
#include "Group.h"

void WorldSession::HandleRequestLfgListBlacklist(WorldPacket& /*p_RecvData*/)
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
    *p_Data << uint32(7);
    *p_Data << uint32(p_Entry->m_CreationTime);
}

void WorldSession::BuildLfgListApplicationRideTicket(WorldPacket* p_Data, LFGListEntry::LFGListApplicationEntry const* p_Entry)
{
    p_Data->appendPackGUID(MAKE_NEW_GUID(p_Entry->m_PlayerLowGuid, 0, HIGHGUID_PLAYER));
    *p_Data << uint32(p_Entry->m_ID);
    *p_Data << uint32(9);
    *p_Data << uint32(p_Entry->m_ApplicationTime);
}

void WorldSession::BuildLfgListEntryInfo(WorldPacket* p_Data, LFGListEntry const* p_Entry)
{
    Group* l_Group = p_Entry->m_Group;
    uint64 l_LeaderGuid = l_Group->GetLeaderGUID(); // Blizz sends 4x same GUID
    const Group::MemberSlotList l_Members = l_Group->GetMemberSlots();
    uint32 l_MemberCount = l_Members.size();

    BuildLfgListJoinRequest(p_Data, p_Entry);
    p_Data->appendPackGUID(l_LeaderGuid);
    p_Data->appendPackGUID(l_LeaderGuid);
    p_Data->appendPackGUID(l_LeaderGuid);
    p_Data->appendPackGUID(l_LeaderGuid);

    *p_Data << uint32(0);        ///< Unk
    *p_Data << uint32(0);        ///< Unk Guid loop
    *p_Data << uint32(0);        ///< Unk Guid loop
    *p_Data << uint32(0);        ///< Friends in group
    *p_Data << uint32(l_MemberCount);
    *p_Data << uint32(0);        ///< Unk
    *p_Data << uint32(time(nullptr) - p_Entry->m_CreationTime);
    *p_Data << uint8(0);         ///< Unk

    for (auto& l_Member : l_Members)
    {
        *p_Data << uint8(l_Member.playerClass);
        *p_Data << uint8(Player::GetRoleBySpecializationId(l_Member.specID));
    }
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
    *p_Data << uint32(p_Entry->m_Timeout - time(nullptr));
    *p_Data << uint8(6);        ///< Reason
    p_Data->WriteBit(p_Listed);
    p_Data->FlushBits();
}

void WorldSession::HandleLfgListUpdateRequest(WorldPacket& p_RecvData)
{
    uint32 l_NameLen, l_CommentLen, l_VoiceChatLen, l_ActivityId, l_ID;
    float l_ItemLevel;

    LFGListEntry* l_Entry = ReadLfgListRideTicketInfo(&p_RecvData, &l_ID);

    if (!l_Entry)
        return;

    if (!l_Entry->m_Group->IsLeader(m_Player->GetGUID()))
        return;

    p_RecvData >> l_ActivityId;
    p_RecvData >> l_ItemLevel;
    l_NameLen = p_RecvData.ReadBits(8);
    l_CommentLen = p_RecvData.ReadBits(11);
    l_VoiceChatLen = p_RecvData.ReadBits(8);
    l_Entry->m_AutoAcceptInvites = p_RecvData.ReadBit();
    l_Entry->m_Name = p_RecvData.ReadString(l_NameLen);
    l_Entry->m_Comment = p_RecvData.ReadString(l_CommentLen);
    l_Entry->m_VoiceChat = p_RecvData.ReadString(l_VoiceChatLen);

    if (l_ItemLevel < sLFGListMgr->GetPlayerItemLevelForActivity(l_Entry->m_ActivityEntry, m_Player))
        l_Entry->m_RequiredItemLevel = l_ItemLevel;

    sLFGListMgr->AutoInviteApplicantsIfPossible(l_Entry);
    sLFGListMgr->SendLFGListStatusUpdate(l_Entry, this);
}

void WorldSession::HandleLfgListLeave(WorldPacket& p_RecvData)
{
    uint32 l_ID;
    LFGListEntry* l_Entry = ReadLfgListRideTicketInfo(&p_RecvData, &l_ID);

    if (!l_Entry)
        return;

    if (!l_Entry->m_Group->IsLeader(GetPlayer()->GetGUID()))
        return;

    sLFGListMgr->Remove(l_ID, GetPlayer());
}

void WorldSession::HandleLfgListSearch(WorldPacket& p_RecvData)
{
    uint32 l_ActivityCategory, l_ActivitySubCategory, unk3, l_UnkGuidLoop, l_Unk620;
    uint32 l_FilterLen = p_RecvData.ReadBits(6);
    uint32 l_BlacklistCount = p_RecvData.ReadBits(7);

    p_RecvData >> l_ActivityCategory >> l_ActivitySubCategory >> unk3 >> l_Unk620 >> l_UnkGuidLoop;

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
    ///< see SMSG_LFGSEARCH_RESULTS for more informations
    WorldPacket l_Data(SMSG_LFG_LIST_SEARCH_RESULT);

    if (!sGroupFinderCategoryStore.LookupEntry(p_ActivityCategory))
    {
        l_Data << uint16(0);    ///< unk
        l_Data << uint32(0);
        SendPacket(&l_Data);
        return;
    }

    std::list<LFGListEntry const*> l_LFGList = sLFGListMgr->GetFilteredList(p_ActivityCategory, p_ActivitySubCategory, p_FilterString, GetPlayer());

    l_Data << uint16(l_LFGList.size()); ///< Weird Blizz is weird blizz
    l_Data << uint32(l_LFGList.size());

    for (auto& l_LFGListEntry : l_LFGList)
        BuildLfgListEntryInfo(&l_Data, l_LFGListEntry);

    SendPacket(&l_Data);
}

LFGListEntry* WorldSession::ReadLfgListRideTicketInfo(WorldPacket* p_Data, uint32* p_ID)
{
    uint64 l_Guid;
    uint32 l_Type, l_Timestamp, l_ID;

    p_Data->readPackGUID(l_Guid);
    *p_Data >> l_ID;
    *p_Data >> l_Type;
    *p_Data >> l_Timestamp;

    if (p_ID)
        *p_ID = l_ID;

    return const_cast<LFGListEntry*>(sLFGListMgr->GetEntrybyGuidLow(l_ID));
}

LFGListEntry::LFGListApplicationEntry* WorldSession::ReadLfgListApplicanmtRideTicketInfo(WorldPacket* p_Data, LFGListEntry* p_ReferenceEntry, uint32* p_ID)
{
    uint64 l_Guid;
    uint32 l_Type, l_Timestamp, l_ID;

    p_Data->readPackGUID(l_Guid);
    *p_Data >> l_ID;
    *p_Data >> l_Type;
    *p_Data >> l_Timestamp;

    if (p_ID)
        *p_ID = l_ID;

    if (!p_ReferenceEntry)
        return nullptr;

    auto l_Iter = p_ReferenceEntry->m_Applications.find(l_ID);

    if (l_Iter != p_ReferenceEntry->m_Applications.end())
        return &l_Iter->second;
    else
        return nullptr;

}

LFGListEntry::LFGListApplicationEntry* WorldSession::ReadLfgListApplicanmtRideTicketInfo(WorldPacket* p_Data, uint32* p_ID)
{
    uint64 l_Guid;
    uint32 l_Type, l_Timestamp, l_ID;

    p_Data->readPackGUID(l_Guid);
    *p_Data >> l_ID;
    *p_Data >> l_Type;
    *p_Data >> l_Timestamp;

    if (p_ID)
        *p_ID = l_ID;

    return sLFGListMgr->GetApplicationByID(l_ID);
}

void WorldSession::HandleLfgListApplyForGroup(WorldPacket& p_RecvData)
{
    return; ///< tmp disable, prevent crash with ASAN
    uint32 l_ID;
    LFGListEntry* l_Entry = ReadLfgListRideTicketInfo(&p_RecvData, &l_ID);

    if (!l_Entry)
        return;

    LFGListEntry::LFGListApplicationEntry l_Application(GetPlayer()->GetGUIDLow(), l_Entry);

    p_RecvData.read_skip<uint32>();
    p_RecvData >> l_Application.m_RoleMask;
    uint32 l_Len = p_RecvData.ReadBits(8);
    l_Application.m_Comment = p_RecvData.ReadString(l_Len);

    sLFGListMgr->OnPlayerApplyForGroup(l_Application, l_ID);
}

void WorldSession::SendLfgListApplyForGroupResult(LFGListEntry const* p_LFGEntry, LFGListEntry::LFGListApplicationEntry const* p_Application)
{
    WorldPacket l_Packet(SMSG_LFG_LIST_APPLY_FOR_GROUP_RESULT, 0x200);
    BuildLfgListApplicationRideTicket(&l_Packet, p_Application);
    BuildLfgListRideTicket(&l_Packet, p_LFGEntry);
    BuildLfgListEntryInfo(&l_Packet, p_LFGEntry); 
    l_Packet << uint32(p_Application->GetRemainingTimeoutTime());
    l_Packet << uint8(6);   ///< Seems to be always 6
    l_Packet << uint8(p_Application->m_RoleMask);
    l_Packet.WriteBits(p_Application->m_Status, 4);
    l_Packet.FlushBits();
    SendPacket(&l_Packet);
}

void WorldSession::SendLfgListApplicantGroupInviteResponse(LFGListEntry::LFGListApplicationEntry const* p_Applicant)
{
    WorldPacket l_Data(SMSG_LFG_LIST_APPLICANT_GROUP_INVITE);
    BuilfLfgListApplicantGroupInvite(&l_Data, p_Applicant);
    SendPacket(&l_Data);
}

void WorldSession::BuildLfgListApplicantListUpdate(WorldPacket* p_Data, LFGListEntry const* p_Entry, LFGListEntry::LFGListApplicationEntry const* p_Applicant)
{
    BuildLfgListApplicantListUpdate(p_Data, p_Entry, std::list<LFGListEntry::LFGListApplicationEntry const*>({ p_Applicant }));
}

void WorldSession::BuildLfgListApplicantListUpdate(WorldPacket* p_Data, LFGListEntry const* p_Entry, std::list<LFGListEntry::LFGListApplicationEntry const*> p_ApplicantList)
{
    BuildLfgListRideTicket(p_Data, p_Entry);
    *p_Data << uint32(p_ApplicantList.size());
    *p_Data << uint32(6);    ///< Unk

    for (auto& l_Applicant : p_ApplicantList)
    {
        Player* l_Player = l_Applicant->GetPlayer();

        BuildLfgListApplicationRideTicket(p_Data, l_Applicant);
        p_Data->appendPackGUID(MAKE_NEW_GUID(l_Applicant->m_PlayerLowGuid, 0, HIGHGUID_PLAYER));

        *p_Data << uint32(l_Player && l_Applicant->m_Listed ? 1 : 0);    ///< 1 Player in queue - could also by a group - NYI
        if (l_Player && l_Applicant->m_Listed)
        {
            p_Data->appendPackGUID(l_Player->GetGUID());
            *p_Data << uint32(g_RealmID);
            *p_Data << float(sLFGListMgr->GetPlayerItemLevelForActivity(p_Entry->m_ActivityEntry, l_Player));
            *p_Data << uint32(l_Player->getLevel());
            *p_Data << uint8(l_Applicant->m_RoleMask);  ///< Queued role
            *p_Data << uint8(l_Applicant->m_RoleMask);  ///< Joined role

            *p_Data << uint32(0);    ///< Unk loop
        }

        p_Data->WriteBits(l_Applicant->m_Status, 4);
        p_Data->WriteBit(l_Applicant->m_Listed);
        p_Data->WriteBits(l_Applicant->m_Comment.size(), 8);
        p_Data->FlushBits();

        p_Data->WriteString(l_Applicant->m_Comment);    
    }
}

void WorldSession::BuilfLfgListApplicantGroupInvite(WorldPacket* p_Data, LFGListEntry::LFGListApplicationEntry const* p_Applicant)
{
    BuildLfgListApplicationRideTicket(p_Data, p_Applicant);
    BuildLfgListRideTicket(p_Data, p_Applicant->m_Owner);
    *p_Data << uint32(0);   ///< Unk
    *p_Data << uint8(p_Applicant->m_Error);
    *p_Data << uint8(p_Applicant->m_RoleMask);
    p_Data->WriteBits(p_Applicant->m_Status, 4);
    p_Data->FlushBits();
}

void WorldSession::HandleLfgListInviteApplicant(WorldPacket& p_RecvData)
{
    LFGListEntry* l_Entry = ReadLfgListRideTicketInfo(&p_RecvData, nullptr);
    LFGListEntry::LFGListApplicationEntry* l_Application = ReadLfgListApplicanmtRideTicketInfo(&p_RecvData, l_Entry, nullptr);
    p_RecvData.rfinish();

    if (!l_Entry || !l_Application)
        return;

    if (!m_Player->GetGroup()->IsAssistant(m_Player->GetGUID()) && !m_Player->GetGroup()->IsLeader(m_Player->GetGUID()))
        return;

    sLFGListMgr->ChangeApplicantStatus(l_Application, LFGListEntry::LFGListApplicationEntry::LFG_LIST_APPLICATION_STATUS_INVITED);
}

void WorldSession::HandleLfgListRemoveApplicant(WorldPacket& p_RecvData)
{
    LFGListEntry* l_Entry = ReadLfgListRideTicketInfo(&p_RecvData, nullptr);
    LFGListEntry::LFGListApplicationEntry* l_Application = ReadLfgListApplicanmtRideTicketInfo(&p_RecvData, l_Entry, nullptr);

    if (!l_Entry || !l_Application)
        return;

    if (!m_Player->GetGroup()->IsAssistant(m_Player->GetGUID()) && !m_Player->GetGroup()->IsLeader(m_Player->GetGUID()))
        return;

    sLFGListMgr->ChangeApplicantStatus(l_Application, LFGListEntry::LFGListApplicationEntry::LFG_LIST_APPLICATION_STATUS_DECLINED);
}

void WorldSession::HandleLfgListCancelApplication(WorldPacket& p_RecvData)
{
    LFGListEntry* l_Entry = ReadLfgListRideTicketInfo(&p_RecvData, nullptr);

    if (!l_Entry)
        return;

    LFGListEntry::LFGListApplicationEntry* l_Applicant = l_Entry->GetApplicant(m_GUIDLow);

    if (!l_Applicant)
        return;

    sLFGListMgr->ChangeApplicantStatus(l_Applicant, LFGListEntry::LFGListApplicationEntry::LFGListApplicationStatus::LFG_LIST_APPLICATION_STATUS_CANCELLED);
}

void WorldSession::HandleLfgListInvitationAnswer(WorldPacket& p_RecvData)
{
    LFGListEntry::LFGListApplicationEntry* l_Applicant = ReadLfgListApplicanmtRideTicketInfo(&p_RecvData, nullptr);

    if (!l_Applicant)
        return;

    bool l_Accept = p_RecvData.ReadBit();
    sLFGListMgr->ChangeApplicantStatus(l_Applicant, l_Accept ? LFGListEntry::LFGListApplicationEntry::LFGListApplicationStatus::LFG_LIST_APPLICATION_STATUS_INVITEACCEPTED : LFGListEntry::LFGListApplicationEntry::LFGListApplicationStatus::LFG_LIST_APPLICATION_INVITEDECLINED);
}
