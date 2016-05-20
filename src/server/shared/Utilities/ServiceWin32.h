////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
#ifndef _WIN32_SERVICE_
#define _WIN32_SERVICE_

bool WinServiceInstall();
bool WinServiceUninstall();
bool WinServiceRun();

#endif                                                      // _WIN32_SERVICE_
#endif                                                      // _WIN32

