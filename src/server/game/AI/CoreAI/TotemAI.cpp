/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "TotemAI.h"
#include "Totem.h"
#include "Creature.h"
#include "DBCStores.h"
#include "ObjectAccessor.h"
#include "SpellMgr.h"

#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"

int TotemAI::Permissible(Creature const* creature)
{
    if (creature->isTotem())
        return PERMIT_BASE_PROACTIVE;

    return PERMIT_BASE_NO;
}

TotemAI::TotemAI(Creature* c) : CreatureAI(c), i_victimGuid(0)
{
    ASSERT(c->isTotem());
}

void TotemAI::MoveInLineOfSight(Unit* /*who*/)
{
}

void TotemAI::EnterEvadeMode()
{
    me->CombatStop(true);
}

void TotemAI::UpdateAI(uint32 const /*diff*/)
{
    if (me->ToTotem()->GetTotemType() != TOTEM_ACTIVE)
        return;

    if (!me->isAlive())
        return;

    // pointer to appropriate target if found any
    Unit* l_Victim = i_victimGuid ? ObjectAccessor::GetUnit(*me, i_victimGuid) : NULL;

    if (l_Victim && l_Victim->HasBreakableByDamageCrowdControlAura(me))
    {
        me->InterruptNonMeleeSpells(false);
        return;
    }

    // Search spell
    SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(me->ToTotem()->GetSpell());
    if (!l_SpellInfo)
        return;

    // Get spell range
    float max_range = l_SpellInfo->GetMaxRange(false);

    // SPELLMOD_RANGE not applied in this place just because not existence range mods for attacking totems

    // Search victim if no, not attackable, or out of range, or friendly (possible in case duel end)
    if (!l_Victim ||
        !l_Victim->isTargetableForAttack() || !me->IsWithinDistInMap(l_Victim, max_range) ||
        me->IsFriendlyTo(l_Victim) || !me->canSeeOrDetect(l_Victim) || l_Victim->HasBreakableByDamageCrowdControlAura())
    {
        l_Victim = NULL;
        if (me->GetCharmerOrOwner())
            l_Victim = ObjectAccessor::GetUnit(*me, me->GetCharmerOrOwner()->GetTargetGUID());
    }

    // If have target
    if (l_Victim)
    {
        if (!me->HasUnitState(UNIT_STATE_CASTING))
        {
            // remember or force to reselect a victim
            if (i_victimGuid && me->GetCharmerOrOwner() && l_Victim != ObjectAccessor::GetUnit(*me, me->GetCharmerOrOwner()->GetTargetGUID()))
                i_victimGuid = 0;
            else
                i_victimGuid = l_Victim->GetGUID();

            // attack
            me->SetInFront(l_Victim);                         // client change orientation by self
            me->CastSpell(l_Victim, me->ToTotem()->GetSpell(), false);
        }
    }
    else
        i_victimGuid = 0;
}

void TotemAI::AttackStart(Unit* /*victim*/)
{
}
