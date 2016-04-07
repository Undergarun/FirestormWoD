////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DEF_ONYXIAS_LAIR_H
#define DEF_ONYXIAS_LAIR_H

enum Data64
{
    DATA_ONYXIA_GUID,
    DATA_FLOOR_ERUPTION_GUID
};

enum InstanceData
{
    DATA_ONYXIA,
    MAX_ENCOUNTER,

    DATA_ONYXIA_PHASE,
    DATA_SHE_DEEP_BREATH_MORE,
    DATA_MANY_WHELPS_COUNT
};

enum Creatures
{
    NPC_WHELP                   = 11262,
    NPC_LAIRGUARD               = 36561,

    NPC_ONYXIA                  = 10184
};

enum OnyxiaPhases
{
    PHASE_START                 = 1,
    PHASE_BREATH                = 2,
    PHASE_END                   = 3
};

enum GameObjects
{
    GO_WHELP_SPAWNER            = 176510,
    GO_WHELP_EGG                = 176511
};

enum AchievementData
{
    ACHIEV_CRITERIA_MANY_WHELPS_10_PLAYER                   = 12565, // Criteria for achievement 4403: Many Whelps! Handle It! (10 player) Hatch 50 eggs in 10s
    ACHIEV_CRITERIA_MANY_WHELPS_25_PLAYER                   = 12568, // Criteria for achievement 4406: Many Whelps! Handle It! (25 player) Hatch 50 eggs in 10s
    ACHIEV_CRITERIA_DEEP_BREATH_10_PLAYER                   = 12566, // Criteria for achievement 4404: She Deep Breaths More (10 player) Everybody evade Deep Breath
    ACHIEV_CRITERIA_DEEP_BREATH_25_PLAYER                   = 12569, // Criteria for achievement 4407: She Deep Breaths More (25 player) Everybody evade Deep Breath
    ACHIEV_TIMED_START_EVENT                                =  6601  // Timed event for achievement 4402, 4005: More Dots! (10, 25 player) 5 min kill
};
#endif
