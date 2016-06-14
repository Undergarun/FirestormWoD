////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LFG_H
#define _LFG_H

#include "Common.h"

enum LfgRoles : uint8
{
    LFG_ROLEMASK_NONE                                    = 0x00,
    LFG_ROLEMASK_LEADER                                  = 0x01,
    LFG_ROLEMASK_TANK                                    = 0x02,
    LFG_ROLEMASK_HEALER                                  = 0x04,
    LFG_ROLEMASK_DAMAGE                                  = 0x08
};

enum LfgUpdateType : uint8
{
    LFG_UPDATETYPE_DEFAULT                       = 0,  ///< Internal Use
    LFG_UPDATETYPE_ADDED_TO_QUEUE                = 24, ///< 6.0.2 19027 OK
    LFG_UPDATETYPE_JOIN_QUEUE                    = 13, ///< 6.0.2 19027 OK
    LFG_UPDATETYPE_PROPOSAL_DECLINED             = 9,  ///< 6.0.2 19027 OK
    LFG_UPDATETYPE_UPDATE_STATUS                 = 15, ///< 6.0.2 19027 OK


    LFG_UPDATETYPE_LEADER_UNK1                   = 1,  ///< 6.0.2 19027
    LFG_UPDATETYPE_REMOVED_FROM_QUEUE            = 8,  ///< 6.0.2 19027
    LFG_UPDATETYPE_GROUP_FOUND                   = 11, ///< 6.0.2 19027
    LFG_UPDATETYPE_PROPOSAL_BEGIN                = 14, ///< 6.0.2 19027

    LFG_UPDATETYPE_ROLECHECK_ABORTED             = 30, ///< 6.0.2 19027
    LFG_UPDATETYPE_ERROR                         = 46, ///< 6.0.2 19027



    LFG_UPDATETYPE_ROLECHECK_FAILED              = 6,  ///<  -
    LFG_UPDATETYPE_PROPOSAL_FAILED               = 8,  ///<  -
    LFG_UPDATETYPE_GROUP_MEMBER_OFFLINE          = 16, ///<  - 6
    LFG_UPDATETYPE_GROUP_DISBAND_UNK16           = 17  ///<  -    FIXME: Sometimes at group disband

    // 25 - related to party guid
    // 52 - update status
};

enum LfgState : uint8
{
    LFG_STATE_NONE,                                        // Not using LFG / LFR
    LFG_STATE_ROLECHECK,                                   // Rolecheck active
    LFG_STATE_QUEUED,                                      // Queued
    LFG_STATE_PROPOSAL,                                    // Proposal active
    LFG_STATE_BOOT,                                        // Vote kick active
    LFG_STATE_DUNGEON,                                     // In LFG Group, in a Dungeon
    LFG_STATE_FINISHED_DUNGEON,                            // In LFG Group, in a finished Dungeon
    LFG_STATE_RAIDBROWSER                                  // Using Raid finder
};

/// Instance lock types
enum LfgLockStatusType : uint16
{
    LFG_LOCKSTATUS_OK                            = 0,      // Internal use only
    LFG_LOCKSTATUS_INSUFFICIENT_EXPANSION        = 1,
    LFG_LOCKSTATUS_TOO_LOW_LEVEL                 = 2,
    LFG_LOCKSTATUS_TOO_HIGH_LEVEL                = 3,
    LFG_LOCKSTATUS_TOO_LOW_GEAR_SCORE            = 4,
    LFG_LOCKSTATUS_TOO_HIGH_GEAR_SCORE           = 5,
    LFG_LOCKSTATUS_RAID_LOCKED                   = 6,
    LFG_LOCKSTATUS_ATTUNEMENT_TOO_LOW_LEVEL      = 1001,
    LFG_LOCKSTATUS_ATTUNEMENT_TOO_HIGH_LEVEL     = 1002,
    LFG_LOCKSTATUS_QUEST_NOT_COMPLETED           = 1022,
    LFG_LOCKSTATUS_MISSING_ITEM                  = 1025,
    LFG_LOCKSTATUS_NOT_IN_SEASON                 = 1031,
    LFG_LOCKSTATUS_ACHIEVEMENT_NOT_COMPLITED     = 1034,
    LFG_LOCKSTATUS_TEMPORARILY_DISABLED          = 10000
};

/// Dungeon and reason why player can't join
struct LfgLockStatus
{
    LfgLockStatusType lockstatus;                          ///< Lock type
    uint32 SubReason1;
    uint32 SubReason2;

    LfgLockStatus()
    {
        SubReason1 = 0;
        SubReason2 = 0;
        lockstatus = LFG_LOCKSTATUS_OK;
    }
};


typedef std::set<uint32> LfgDungeonSet;
typedef ACE_Based::LockedMap<uint32, LfgLockStatus> LfgLockMap;
typedef std::map<uint64, LfgLockMap> LfgLockPartyMap;

#endif
