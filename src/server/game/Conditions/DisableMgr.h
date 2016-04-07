////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TRINITY_DISABLEMGR_H
#define TRINITY_DISABLEMGR_H

#include "Define.h"
#include "VMapManager2.h"

class Unit;

enum DisableType
{
    DISABLE_TYPE_SPELL                  = 0,
    DISABLE_TYPE_QUEST                  = 1,
    DISABLE_TYPE_MAP                    = 2,
    DISABLE_TYPE_BATTLEGROUND           = 3,
    DISABLE_TYPE_ACHIEVEMENT_CRITERIA   = 4,
    DISABLE_TYPE_OUTDOORPVP             = 5,
    DISABLE_TYPE_VMAP                   = 6,
    DISABLE_TYPE_GARRISON_MISSION       = 7,
    DISABLE_TYPE_MMAP                   = 8
};

enum SpellDisableTypes
{
    SPELL_DISABLE_PLAYER            = 0x1,
    SPELL_DISABLE_CREATURE          = 0x2,
    SPELL_DISABLE_PET               = 0x4,
    SPELL_DISABLE_DEPRECATED_SPELL  = 0x8,
    SPELL_DISABLE_MAP               = 0x10,
    SPELL_DISABLE_AREA              = 0x20,
    MAX_SPELL_DISABLE_TYPE = (  SPELL_DISABLE_PLAYER | SPELL_DISABLE_CREATURE | SPELL_DISABLE_PET |
                                SPELL_DISABLE_DEPRECATED_SPELL | SPELL_DISABLE_MAP | SPELL_DISABLE_AREA)
};

enum MMapDisableTypes
{
    MMAP_DISABLE_PATHFINDING    = 0x0
};

namespace DisableMgr
{
    void LoadDisables();
    bool IsDisabledFor(DisableType type, uint32 entry, Unit const* unit, uint8 flags = 0);
    void CheckQuestDisables();
    bool IsVMAPDisabledFor(uint32 entry, uint8 flags);
    bool IsPathfindingEnabled(uint32 mapId);
}

#endif //TRINITY_DISABLEMGR_H
