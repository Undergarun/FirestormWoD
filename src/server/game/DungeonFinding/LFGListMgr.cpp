////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "LFGListMgr.h"
#include "GroupMgr.h"

bool LFGListMgr::Insert(LFGListEntry* p_LFGEntry, Player* p_Requester)
{
    if (!IsEligibleForQueue(p_Requester))
    {
        p_Requester->GetSession()->SendLfgListJoinResult(p_LFGEntry, LFG_LIST_ERR_ALREADY_USING_LFG_LIST);
        return false;
    }

    uint32 l_GroupId = 0;
    Group* l_Group = p_Requester->GetGroup();
    if (l_Group && l_Group->isBGGroup())
        l_Group = p_Requester->GetOriginalGroup();

    /// Create a new group fyyiiii
    if (!l_Group)
    {
        l_Group = new Group;
        if (!l_Group->AddLeaderInvite(p_Requester))
        {
            p_Requester->GetSession()->SendLfgListJoinResult(p_LFGEntry, LFG_LIST_ERR_LFG_NO_LFG_OBJECT);
            delete l_Group;
            return false;
        }

        l_Group->Create(p_Requester);
        sGroupMgr->AddGroup(l_Group);
    }

    p_LFGEntry->m_Group = l_Group;
    m_LFGListQueue.insert({l_Group->GetLowGUID(), p_LFGEntry});
    SendLFGListStatusUpdate(p_LFGEntry);
    return true;
}

/// Stupid error messages: Todo: Find more appropriate
bool LFGListMgr::CanInsert(LFGListEntry const* p_LFGEntry, Player* p_Requester, bool p_SendError /* = false */) const
{
    if (!p_LFGEntry->m_ActivityEntry)
    {
        if (p_SendError)
            p_Requester->GetSession()->SendLfgListJoinResult(p_LFGEntry, LFG_LIST_ERR_LFG_NO_LFG_OBJECT);
        return false;
    }

    if (!IsEligibleForQueue(p_Requester) || p_LFGEntry->m_Group)
    {
        if (p_SendError)
            p_Requester->GetSession()->SendLfgListJoinResult(p_LFGEntry, LFG_LIST_ERR_ALREADY_USING_LFG_LIST);
        return false;
    }

    if (p_Requester->GetAverageItemLevelTotal() < p_LFGEntry->m_RequiredItemLevel)
    {
        if (p_SendError)
            p_Requester->GetSession()->SendLfgListJoinResult(p_LFGEntry, LFG_LIST_ERR_LFG_INVALID_SLOT);
        return false;
    }

    Group* l_Group = p_Requester->GetGroup();

    if (l_Group)
    {
        if ((!l_Group->isRaidGroup() || !l_Group->IsAssistant(p_Requester->GetGUID())) && !l_Group->IsLeader(p_Requester->GetGUID()))
        {
            if (p_SendError)
                p_Requester->GetSession()->SendLfgListJoinResult(p_LFGEntry, LFG_LIST_ERR_LFG_NO_LFG_OBJECT);
            return false;
        }
    }

    return true;
}

bool LFGListMgr::IsEligibleForQueue(Player* p_Requester) const
{
    if (!p_Requester)
        return false;

    Group* l_Group = p_Requester->GetGroup();
    if (l_Group && l_Group->isBGGroup())
        l_Group = p_Requester->GetOriginalGroup();

    return !IsGroupQueued(l_Group);
}

bool LFGListMgr::IsGroupQueued(Group const* p_Group) const
{
    if (!p_Group)
        return false;

    return m_LFGListQueue.find(p_Group->GetLowGUID()) != m_LFGListQueue.end();
}

void LFGListMgr::SendLFGListStatusUpdate(LFGListEntry* p_LFGEntry, WorldSession* p_WorldSession /* = nullptr */, bool p_Listed /* = true */)
{
    WorldPacket l_Data;
    WorldSession::BuildLfgListQueueUpdate(&l_Data, p_LFGEntry, p_Listed);

    if (p_WorldSession)
        p_WorldSession->SendPacket(&l_Data);
    else if (Group* l_Group = p_LFGEntry->m_Group)
        p_LFGEntry->BroadcastPacketToGrop(&l_Data);
}

bool LFGListMgr::Remove(uint32 l_GroupGuidLow, Player* p_Requester /* = nullptr */, bool l_Disband /* = true */)
{
    std::unordered_map<uint32, LFGListEntry *>::const_iterator l_Iter = m_LFGListQueue.find(l_GroupGuidLow);
    if (l_Iter == m_LFGListQueue.end())
        return false;

    Group* l_Group = l_Iter->second->m_Group;
    if (!l_Group)
        return false;

    if (p_Requester && ((!l_Group->isRaidGroup() || !l_Group->IsAssistant(p_Requester->GetGUID())) && !l_Group->IsLeader(p_Requester->GetGUID())))
        return false;

    LFGListEntry* l_Entry = l_Iter->second;
    m_LFGListQueue.erase(l_Iter);
    SendLFGListStatusUpdate(l_Entry, nullptr, false);
    delete l_Entry;

    if (l_Disband && l_Group->GetMembersCount() < 2)
        l_Group->Disband();

    return true;
}

void LFGListMgr::PlayerAddedToGroup(Player* p_Player, Group* p_Group)
{
    std::unordered_map<uint32, LFGListEntry *>::const_iterator l_Iter = m_LFGListQueue.find(p_Group->GetLowGUID());
    if (l_Iter == m_LFGListQueue.end())
        return;

    SendLFGListStatusUpdate(l_Iter->second, p_Player->GetSession(), true);
}

void LFGListMgr::PlayerRemoveFromGroup(Player* p_Player, Group* p_Group)
{
    std::unordered_map<uint32, LFGListEntry *>::const_iterator l_Iter = m_LFGListQueue.find(p_Group->GetLowGUID());
    if (l_Iter == m_LFGListQueue.end())
        return;

    SendLFGListStatusUpdate(l_Iter->second, p_Player->GetSession(), false);
}

std::list<LFGListEntry const*> LFGListMgr::GetFilteredList(uint32 p_ActivityCategory, uint32 p_ActivitySubCategory, std::string p_FilterString)
{
    std::list<LFGListEntry const*> l_LFGFiltered;

    for (auto l_Iter : m_LFGListQueue)
    {
        LFGListEntry const* l_ListEntry = l_Iter.second;

        if (l_ListEntry->m_ActivityEntry->ActivityGroupID != p_ActivityCategory)
            continue;

        if (p_FilterString.length() && l_ListEntry->m_Name.length())
        {
            std::string l_UpperName = l_ListEntry->m_Name;
            std::transform(l_UpperName.begin(), l_UpperName.end(), l_UpperName.begin(), toupper);
            std::transform(p_FilterString.begin(), p_FilterString.end(), p_FilterString.begin(), toupper);

            if (l_UpperName.find(p_FilterString) == std::string::npos)
                continue;
        }

        l_LFGFiltered.push_back(l_ListEntry);
    }
    return l_LFGFiltered;
}

LFGListEntry const* LFGListMgr::GetEntrybyGuidLow(uint32 p_ID)
{
    std::unordered_map<uint32, LFGListEntry *>::const_iterator l_Iter = m_LFGListQueue.find(p_ID);
    if (l_Iter == m_LFGListQueue.end())
        return nullptr;

    return l_Iter->second;
}

void LFGListMgr::OnPlayerApplyForGroup(LFGListEntry::LFGListApplicationEntry p_Application, uint32 p_GroupID)
{
    LFGListEntry* l_Entry = const_cast<LFGListEntry*>(GetEntrybyGuidLow(p_GroupID));

    if (!l_Entry)
        return;

    Player* l_Player = p_Application.GetPlayer();

    if (!l_Player)
        return;

    l_Entry->m_Applications.insert({ l_Player->GetGUIDLow(), p_Application });
    LFGListEntry::LFGListApplicationEntry* l_Application = &l_Entry->m_Applications.find(l_Player->GetGUIDLow())->second;

    if (l_Entry)
    {
        if (l_Entry->m_AutoAcceptInvites)
            ChangeApplicantStatus(l_Application, LFGListEntry::LFGListApplicationEntry::LFG_LIST_APPICATION_STATUS_INVITED);
        else
            ChangeApplicantStatus(l_Application, LFGListEntry::LFGListApplicationEntry::LFG_LIST_APPICATION_STATUS_APPLIED);
    }
}

void LFGListEntry::BroadcastPacketToGrop(WorldPacket* p_Data)
{
    m_Group->BroadcastPacket(p_Data, false);
}

void LFGListEntry::BroadcastPacketToApplicants(WorldPacket* p_Data)
{
    for (auto l_Applicant : m_Applications)
        if (Player* l_Player = l_Applicant.second.GetPlayer())
            l_Player->SendDirectMessage(p_Data);
}

Player* LFGListEntry::LFGListApplicationEntry::GetPlayer() const
{
    return sObjectMgr->GetPlayerByLowGUID(m_PlayerLowGuid);
};

bool LFGListEntry::IsApplied(uint32 p_GuidLow) const
{
    return m_Applications.find(p_GuidLow) != m_Applications.end();
}

bool LFGListEntry::IsApplied(Player* p_Player) const
{
    return IsApplied(p_Player->GetGUIDLow());
}

void LFGListEntry::BroadcastApplicantUpdate(LFGListApplicationEntry const* l_Applicant)
{
    WorldPacket l_Data(SMSG_LFG_LIST_APPLICANT_LIST_UPDATE, 0x100);
    WorldSession::BuildLfgListApplicantListUpdate(&l_Data, this, l_Applicant);
    BroadcastPacketToGrop(&l_Data);
}

void LFGListEntry::InviteApplicant(LFGListApplicationEntry const* l_Applicant)
{
}

void LFGListEntry::LFGListApplicationEntry::ResetTimeout()
{
    m_Timeout = time(nullptr) + (m_Status == LFGListEntry::LFGListApplicationEntry::LFG_LIST_APPICATION_STATUS_INVITED ? LFG_LIST_INVITE_TO_GROUP_TIMEOUT : LFG_LIST_APPLY_FOR_GROUP_TIMEOUT);
}

void LFGListEntry::ResetTimeout()
{
    m_Timeout = time(nullptr) + LFG_LIST_GROUP_TIMEOUT;
    sLFGListMgr->SendLFGListStatusUpdate(this);
}

uint32 LFGListEntry::LFGListApplicationEntry::GetRemainingTimeoutTime() const
{
    return m_Timeout - time(nullptr);
}

void LFGListMgr::ChangeApplicantStatus(LFGListEntry::LFGListApplicationEntry* p_Application, LFGListEntry::LFGListApplicationEntry::LFGListApplicationStatus p_Status, bool p_Notify /* = true */)
{
    if (p_Application->m_Status == p_Status)
        return;

    bool l_Remove = false;

    p_Application->m_Status = p_Status;

    switch (p_Status)
    {
        case LFGListEntry::LFGListApplicationEntry::LFG_LIST_APPICATION_STATUS_APPLIED:
        case LFGListEntry::LFGListApplicationEntry::LFG_LIST_APPICATION_STATUS_INVITED:
        {
            p_Application->ResetTimeout();
            p_Application->m_Owner->ResetTimeout();

            if (p_Notify)
                p_Application->GetPlayer()->GetSession()->SendLfgListApplyForGroupResult(p_Application->m_Owner, p_Application, p_Status);
            break;
        }
        case LFGListEntry::LFGListApplicationEntry::LFG_LIST_APPLICATION_INVITEDECLINED:
        case LFGListEntry::LFGListApplicationEntry::LFG_LIST_APPICATION_STATUS_DECLINED:
        case LFGListEntry::LFGListApplicationEntry::LFG_LIST_APPICATION_STATUS_CANCELLED:
        case LFGListEntry::LFGListApplicationEntry::LFG_LIST_APPICATION_STATUS_TIMEOUT:
        {
            p_Application->m_Listed = false;
            l_Remove = true;

            if (p_Notify)
                p_Application->GetPlayer()->GetSession()->SendLfgListApplicantGroupInviteResponse(p_Application);
            break;
        }
        case LFGListEntry::LFGListApplicationEntry::LFG_LIST_APPICATION_STATUS_INVITEACCEPTED:
        {
            p_Application->m_Listed = false;
            l_Remove = true;
            p_Application->m_Owner->m_Group->AddMember(p_Application->GetPlayer());

            if (p_Notify)
                p_Application->GetPlayer()->GetSession()->SendLfgListApplicantGroupInviteResponse(p_Application);
            break;
        }
    }

    p_Application->m_Owner->BroadcastApplicantUpdate(p_Application);

    if (l_Remove)
        p_Application->m_Owner->m_Applications.erase(p_Application->m_Owner->m_Applications.find(p_Application->m_PlayerLowGuid));
}

LFGListEntry::LFGListApplicationEntry* LFGListMgr::GetApplicationByID(uint32 p_ID)
{
    for (auto& l_Group : m_LFGListQueue)
    {
        for (auto& l_Applicant : l_Group.second->m_Applications)
        {
            if (l_Applicant.first == p_ID)
                return &l_Applicant.second;
        }
    }

    return nullptr;
}

void LFGListMgr::Update(uint32 const p_Diff)
{
    for (auto& l_Entry : m_LFGListQueue)
        l_Entry.second->Update(p_Diff);
}

void LFGListEntry::Update(uint32 const p_Diff)
{
    for (auto& l_Application : m_Applications)
        l_Application.second.Update(p_Diff);

    if (m_Timeout <= time(nullptr)) ///< RIP
    {
        sLFGListMgr->Remove(m_Group->GetLowGUID());
    }
}

void LFGListEntry::LFGListApplicationEntry::Update(uint32 const p_Diff)
{
    if (m_Timeout <= time(nullptr)) ///< Bye bye
    {
        sLFGListMgr->ChangeApplicantStatus(this, LFGListEntry::LFGListApplicationEntry::LFG_LIST_APPICATION_STATUS_TIMEOUT);
    }
}

LFGListEntry::LFGListApplicationEntry* LFGListEntry::GetApplicant(uint32 p_ID)
{
    auto l_Iter = m_Applications.find(p_ID);
    return l_Iter != m_Applications.end() ? &l_Iter->second : nullptr;
}

void LFGListMgr::RemovePlayerDueToLogout(uint32 p_LowGUID) ///< This is wrong, but cba to do it other way for now
{
    LFGListEntry::LFGListApplicationEntry* l_Application = GetApplicationByID(p_LowGUID);

    if (!l_Application)
        return;

    ChangeApplicantStatus(l_Application, LFGListEntry::LFGListApplicationEntry::LFG_LIST_APPICATION_STATUS_CANCELLED, false);
}
