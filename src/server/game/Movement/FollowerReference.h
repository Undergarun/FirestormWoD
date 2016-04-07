////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _FOLLOWERREFERENCE_H
#define _FOLLOWERREFERENCE_H

#include "Reference.h"

class TargetedMovementGeneratorBase;
class Unit;

class FollowerReference : public Reference<Unit, TargetedMovementGeneratorBase>
{
    protected:
        void targetObjectBuildLink() override;
        void targetObjectDestroyLink() override;
        void sourceObjectDestroyLink() override;
};
#endif
