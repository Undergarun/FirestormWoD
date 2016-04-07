////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _FOLLOWERREFMANAGER
#define _FOLLOWERREFMANAGER

#include "RefManager.h"

class Unit;
class TargetedMovementGeneratorBase;

class FollowerRefManager : public RefManager<Unit, TargetedMovementGeneratorBase>
{

};
#endif
