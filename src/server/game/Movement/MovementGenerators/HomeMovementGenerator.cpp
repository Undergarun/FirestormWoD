////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "HomeMovementGenerator.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "MoveSplineInit.h"
#include "MoveSpline.h"

void HomeMovementGenerator<Creature>::DoInitialize(Creature* owner)
{
    _setTargetLocation(owner);
}

void HomeMovementGenerator<Creature>::DoFinalize(Creature* owner)
{
    if (arrived)
    {
        owner->ClearUnitState(UNIT_STATE_EVADE);
        owner->SetWalk(true);
        owner->LoadCreaturesAddon();
        owner->AI()->JustReachedHome();
    }
}

void HomeMovementGenerator<Creature>::DoReset(Creature*) { }

void HomeMovementGenerator<Creature>::_setTargetLocation(Creature* owner)
{
    if (owner->HasUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED | UNIT_STATE_DISTRACTED))
        return;

    Movement::MoveSplineInit init(owner);
    float x, y, z, o;
    // at apply we can select more nice return points base at current movegen
    if (owner->GetMotionMaster()->empty() || !owner->GetMotionMaster()->top()->GetResetPosition(owner, x, y, z))
    {
        owner->GetHomePosition(x, y, z, o);
        init.SetFacing(o);
    }
    init.MoveTo(x, y, z);
    init.SetWalk(false);
    init.Launch();

    arrived = false;

    owner->ClearUnitState(uint32(UNIT_STATE_ALL_STATE & ~(UNIT_STATE_EVADE | UNIT_STATE_IGNORE_PATHFINDING)));
}

bool HomeMovementGenerator<Creature>::DoUpdate(Creature* owner, const uint32 /*time_diff*/)
{
    arrived = owner->movespline->Finalized();
    return !arrived;
}
