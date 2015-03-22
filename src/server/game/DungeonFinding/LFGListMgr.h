////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Group.h"
#include <unordered_map>

struct LFGListEntry
{
    int32 m_ActivityID;
    float m_RequiredItemLevel;
    std::string m_Name;
    std::string m_Comment;
    std::string m_VoiceChat;
    bool m_AutoAcceptInvites;

    // Helpers
    Group* m_Group;
    uint32 m_CreationTime;
};

class LFGListMgr
{
public:

    // Weird blizz is weird ~90% of this shit is useless
    enum LFGListJoinResultReason
    {
        LFG_LIST_ERR_LFG_NO_LFG_OBJECT                          = 32,
        LFG_LIST_ERR_LFG_NO_SLOTS_PLAYER                        = 33,
        LFG_LIST_ERR_LFG_MISMATCHED_SLOTS                       = 34,
        LFG_LIST_ERR_LFG_PARTY_PLAYERS_FROM_DIFFERENT_REALMS    = 35,
        LFG_LIST_ERR_LFG_MEMBERS_NOT_PRESENT                    = 36,
        LFG_LIST_ERR_LFG_GET_INFO_TIMEOUT                       = 37,
        LFG_LIST_ERR_LFG_INVALID_SLOT                           = 38,
        LFG_LIST_ERR_LFG_DESERTER_PLAYER                        = 39,
        LFG_LIST_ERR_LFG_DESERTER_PARTY                         = 40,
        LFG_LIST_ERR_LFG_RANDOM_COOLDOWN_PLAYER                 = 41,
        LFG_LIST_ERR_LFG_RANDOM_COOLDOWN_PARTY                  = 42,
        LFG_LIST_ERR_LFG_TOO_MANY_MEMBERS                       = 43,
        LFG_LIST_ERR_LFG_CANT_USE_DUNGEONS                      = 44,
        LFG_LIST_ERR_LFG_ROLE_CHECK_FAILED                      = 45,
        LFG_LIST_ERR_LFG_TOO_FEW_MEMBERS                        = 51,
        LFG_LIST_ERR_LFG_REASON_TOO_MANY_LFG                    = 52,
        LFG_LIST_ERR_LFG_MISMATCHED_SLOTS_LOCAL_XREALM          = 54,
        LFG_LIST_ERR_ALREADY_USING_LFG_LIST                     = 62,
        LFG_LIST_ERR_RESTRICTED_ACCOUNT_LFG_LIST                = 64,
        LFG_LIST_ERR_USER_SQUELCHED                             = 65,
    };

    bool Insert(LFGListEntry* p_LFGEntry, Player* p_Requester);
    bool CanInsert(LFGListEntry const* p_LFGEntry, Player* p_Requester, bool p_SendError = false) const;
    bool IsEligibleForQueue(Player* p_Player) const;
    bool IsGroupQueued(Group const* p_Group) const;
    void SendLFGListStatusUpdate(LFGListEntry const* p_LFGEntry, WorldSession* p_WorldSession = nullptr, bool p_Listed = true);
    bool Remove(uint32 l_GroupGuidLow, Player* p_Requester = nullptr, bool l_Disband = true);
    void PlayerAddedToGroup(Player* p_Player, Group* p_Group);
    void PlayerRemoveFromGroup(Player* p_Player, Group* p_Group);

private:
    std::unordered_map<uint32, LFGListEntry const*> m_LFGListQueue;
};

#define sLFGListMgr ACE_Singleton<LFGListMgr, ACE_Null_Mutex>::instance()
