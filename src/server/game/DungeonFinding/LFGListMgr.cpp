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
    if (!IsEligibleForQueue(p_Requester) || p_LFGEntry->m_Group)
    {
        if (p_SendError)
            p_Requester->GetSession()->SendLfgListJoinResult(p_LFGEntry, LFG_LIST_ERR_ALREADY_USING_LFG_LIST);
        return false;
    }

    if (!sGroupFinderActivityStore.LookupEntry(p_LFGEntry->m_ActivityID))
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
        return true;

    return m_LFGListQueue.find(p_Group->GetLowGUID()) != m_LFGListQueue.end();
}

void LFGListMgr::SendLFGListStatusUpdate(LFGListEntry const* p_LFGEntry, WorldSession* p_WorldSession /* = nullptr */, bool p_Listed /* = true */)
{
    WorldPacket l_Data;
    WorldSession::BuildLfgListQueueUpdate(&l_Data, p_LFGEntry, p_Listed);

    if (p_WorldSession)
        p_WorldSession->SendPacket(&l_Data);
    else if (Group* l_Group = p_LFGEntry->m_Group)
        l_Group->BroadcastPacket(&l_Data, false);
}

bool LFGListMgr::Remove(uint32 l_GroupGuidLow, Player* p_Requester /* = nullptr */, bool l_Disband /* = true */)
{
    std::unordered_map<uint32, LFGListEntry const*>::const_iterator l_Iter = m_LFGListQueue.find(l_GroupGuidLow);
    if (l_Iter == m_LFGListQueue.end())
        return false;

    Group* l_Group = l_Iter->second->m_Group;
    if (!l_Group)
        return false;

    if (p_Requester && ((!l_Group->isRaidGroup() || !l_Group->IsAssistant(p_Requester->GetGUID())) && !l_Group->IsLeader(p_Requester->GetGUID())))
        return false;

    LFGListEntry const* l_Entry = l_Iter->second;
    m_LFGListQueue.erase(l_Iter);
    SendLFGListStatusUpdate(l_Entry, nullptr, false);
    delete l_Entry;

    if (l_Disband && l_Group->GetMembersCount() < 2)
        l_Group->Disband();

    return true;
}


void LFGListMgr::PlayerAddedToGroup(Player* p_Player, Group* p_Group)
{
    std::unordered_map<uint32, LFGListEntry const*>::const_iterator l_Iter = m_LFGListQueue.find(p_Group->GetLowGUID());
    if (l_Iter == m_LFGListQueue.end())
        return;

    SendLFGListStatusUpdate(l_Iter->second, p_Player->GetSession(), true);
}

void LFGListMgr::PlayerRemoveFromGroup(Player* p_Player, Group* p_Group)
{
    std::unordered_map<uint32, LFGListEntry const*>::const_iterator l_Iter = m_LFGListQueue.find(p_Group->GetLowGUID());
    if (l_Iter == m_LFGListQueue.end())
        return;

    SendLFGListStatusUpdate(l_Iter->second, p_Player->GetSession(), false);
}
