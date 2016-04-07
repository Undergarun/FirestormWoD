////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"

char const* localeNames[TOTAL_LOCALES] = {
  "enUS",
  "koKR",
  "frFR",
  "deDE",
  "zhCN",
  "zhTW",
  "esES",
  "esMX",
  "ruRU",
  "ptPT",
  "itIT",
};

ACE_Based::LockedQueue<GmCommand*, ACE_Thread_Mutex> GmLogQueue;
ACE_Based::LockedQueue<GmChat*, ACE_Thread_Mutex> GmChatLogQueue;
ACE_Based::LockedQueue<ArenaLog*, ACE_Thread_Mutex> ArenaLogQueue;

LocaleConstant GetLocaleByName(const std::string& name)
{
    for (uint32 i = 0; i < TOTAL_LOCALES; ++i)
        if (name==localeNames[i])
            return LocaleConstant(i);

    return LOCALE_enUS;                                     // including enGB case
}

