////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/// \addtogroup Trinityd
/// @{
/// \file

#ifndef _TRINITY_RARUNNABLE_H_
#define _TRINITY_RARUNNABLE_H_

#include "Common.h"

#include <ace/Reactor.h>

class RARunnable : public ACE_Based::Runnable
{
public:
    RARunnable();
    virtual ~RARunnable();
    void run();

private:
    ACE_Reactor* m_Reactor;

};

#endif /* _TRINITY_RARUNNABLE_H_ */
/// @}
