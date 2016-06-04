////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __GITREVISION_H__
# define __GITREVISION_H__

#include "Common.h"

namespace GitRevision
{
    char const* GetPackageName();
    char const* GetHash();
    char const* GetDate();
    char const* GetBranch();
    char const* GetSourceDirectory();
    char const* GetMySQLExecutable();
    char const* GetFullDatabase();
    char const* GetFullVersion();
    char const* GetCompanyNameStr();
    char const* GetLegalCopyrightStr();
    char const* GetFileVersionStr();
    char const* GetProductVersionStr();
}

#endif
