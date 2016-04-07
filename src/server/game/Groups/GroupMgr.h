////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _GROUPMGR_H
#define _GROUPMGR_H

#include "Group.h"

class GroupMgr
{
    friend class ACE_Singleton<GroupMgr, ACE_Null_Mutex>;
private:
    GroupMgr();
    ~GroupMgr();

public:
    typedef std::map<uint32, Group*> GroupContainer;
    typedef std::vector<Group*>      GroupDbContainer;

    Group* GetGroupByGUID(uint32 guid) const;

    uint32 GenerateNewGroupDbStoreId();
    void   RegisterGroupDbStoreId(uint32 storageId, Group* group);
    void   FreeGroupDbStoreId(Group* group);
    void   SetNextGroupDbStoreId(uint32 storageId) { NextGroupDbStoreId = storageId; };
    Group* GetGroupByDbStoreId(uint32 storageId) const;
    void   SetGroupDbStoreSize(uint32 newSize) { GroupDbStore.resize(newSize); }

    void   LoadGroups();
    uint32 GenerateGroupId();
    void   AddGroup(Group* group);
    void   RemoveGroup(Group* group);


protected:
    uint32           NextGroupId;
    uint32           NextGroupDbStoreId;
    GroupContainer   GroupStore;
    GroupDbContainer GroupDbStore;
};

#define sGroupMgr ACE_Singleton<GroupMgr, ACE_Null_Mutex>::instance()

#endif
