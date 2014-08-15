#ifndef _CUFPROFILES_H
#define _CUFPROFILES_H

#include "Common.h"

// 5 hardcoded in Script_GetMaxNumCUFProfiles
#define MAX_CUF_PROFILES                5
// 32 utf8 hardcoded in Script_CreateNewRaidProfile, 128 everywhere else
#define MAX_CUF_PROFILE_NAME_LENGTH     128

struct CUFProfileData
{
    bool KeepGroupsTogether : 1;
    bool DisplayPets : 1;
    bool DisplayMainTankAndAssist : 1;
    bool DisplayHealPrediction : 1;
    bool DisplayAggroHighlight : 1;
    bool DisplayOnlyDispellableDebuffs : 1;
    bool DisplayPowerBar : 1;
    bool DisplayBorder : 1;
    bool UseClassColors : 1;
    bool HorizontalGroups : 1;
    bool DisplayNonBossDebuffs : 1;
    bool DynamicPosition : 1;
    bool Locked : 1;
    bool Shown : 1;
    bool AutoActivate2Players : 1;
    bool AutoActivate3Players : 1;
    bool AutoActivate5Players : 1;
    bool AutoActivate10Players : 1;
    bool AutoActivate15Players : 1;
    bool AutoActivate25Players : 1;
    bool AutoActivate40Players : 1;
    bool AutoActivateSpec1 : 1;
    bool AutoActivateSpec2 : 1;
    bool AutoActivatePvP : 1;
    bool AutoActivatePvE : 1;

    uint16 FrameHeight;
    uint16 FrameWidth;
    uint8 SortBy;
    uint8 HealthText;
    uint8 TopPoint;
    uint8 BottomPoint;
    uint8 LeftPoint;
    uint16 TopOffset;
    uint16 BottomOffset;
    uint16 LeftOffset;
};

struct CUFProfile
{
    CUFProfile()
    {
        l_NameLen = 0;
    }

    uint32 l_NameLen;
    std::string Name;
    CUFProfileData data;
};

typedef std::vector<CUFProfile> CUFProfiles;

#endif
