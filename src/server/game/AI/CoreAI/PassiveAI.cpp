////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "PassiveAI.h"
#include "Creature.h"
#include "TemporarySummon.h"
#include "SpellAuraEffects.h"

PassiveAI::PassiveAI(Creature* c) : CreatureAI(c) { me->SetReactState(REACT_PASSIVE); }
PossessedAI::PossessedAI(Creature* c) : CreatureAI(c) { me->SetReactState(REACT_PASSIVE); }
NullCreatureAI::NullCreatureAI(Creature* c) : CreatureAI(c) { me->SetReactState(REACT_PASSIVE); }

void PassiveAI::UpdateAI(const uint32)
{
    if (me->isInCombat() && me->getAttackers().empty())
        EnterEvadeMode();
}

void PossessedAI::AttackStart(Unit* target)
{
    me->Attack(target, true);
}

void PossessedAI::UpdateAI(const uint32 /*diff*/)
{
    if (me->getVictim())
    {
        if (!me->IsValidAttackTarget(me->getVictim()))
            me->AttackStop();
        else
            DoMeleeAttackIfReady();
    }
}

void PossessedAI::JustDied(Unit* /*u*/)
{
    // We died while possessed, disable our loot
    me->RemoveFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
}

void PossessedAI::KilledUnit(Unit* victim)
{
    // We killed a creature, disable victim's loot
    if (victim->GetTypeId() == TYPEID_UNIT)
        victim->RemoveFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
}

void CritterAI::DamageTaken(Unit* /*done_by*/, uint32&, SpellInfo const*)
{
    if (!me->HasUnitState(UNIT_STATE_FLEEING))
        me->SetControlled(true, UNIT_STATE_FLEEING);
}

void CritterAI::EnterEvadeMode()
{
    if (me->HasUnitState(UNIT_STATE_FLEEING))
        me->SetControlled(false, UNIT_STATE_FLEEING);
    CreatureAI::EnterEvadeMode();
}

void TriggerAI::IsSummonedBy(Unit* summoner)
{
    if (me->m_spells[0])
        me->CastSpell(me, me->m_spells[0], false, 0, nullptr, summoner->GetGUID());
}
