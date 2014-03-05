#ifndef _CUFPROFILES_H
#define _CUFPROFILES_H

#include "Common.h"

// 5 hardcoded in Script_GetMaxNumCUFProfiles
#define MAX_CUF_PROFILES                5
// 32 utf8 hardcoded in Script_CreateNewRaidProfile, 128 everywhere else
#define MAX_CUF_PROFILE_NAME_LENGTH     128

struct CUFProfileData
{
    bool displayMainTankAndAssistant:1;
    bool keepGroupsTogether:1;
    bool auto5:1;
    bool auto15:1;
    bool displayPowerBar:1;
    bool displayBorder:1;
    bool autoPvP:1;
    bool auto40:1;
    bool autoSpec1:1;
    bool auto3:1;
    bool auto2:1;
    bool useClassColors:1;
    bool bit13:1;
    bool autoSpec2:1;
    bool horizontalGroups:1;
    bool bit16:1;
    bool displayOnlyDispellableDebuffs:1;
    bool displayNonBossDebuffs:1;
    bool displayPets:1;
    bool auto25:1;
    bool displayHealPrediction:1;
    bool displayAggroHighlight:1;
    bool auto10:1;
    bool bit24:1;
    bool autoPvE:1;
    
    uint16 unk0;
    uint16 unk1;
    uint16 frameWidth;
    uint16 frameHeight;
    uint16 unk4;

    uint8 unk5;
    uint8 unk6;
    uint8 unk7;
    uint8 sortType;
    uint8 healthText;
};

struct CUFProfile
{
    CUFProfile()
    {
        nameLen = 0;
    }

    uint32 nameLen;
	std::string name;
    CUFProfileData data;
};

typedef std::vector<CUFProfile> CUFProfiles;

#endif