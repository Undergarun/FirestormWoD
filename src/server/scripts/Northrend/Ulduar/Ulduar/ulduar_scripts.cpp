/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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
 
#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "ulduar.h"

enum Events
{
    // Ironwork Cannon
    EVENT_FLAME_CANNON      = 1,
};

enum Spells
{
    SPELL_FLAME_CANNON      = 62395
};

struct RangeCheck : public std::unary_function<Unit*, bool>
{
    RangeCheck(Unit* base, float min, float max) : __base(base), __mindist(min), __maxdist(max) {}

    bool operator()(Unit* const other) const
    {
        float dist = __base->GetDistance(other->GetPositionX(), other->GetPositionY(), other->GetPositionZ());
        if ( dist >= __mindist && dist <= __maxdist)
            return true;
        return false;
    }

    private:
        Unit* __base;
        float __mindist;
        float __maxdist;
};

class npc_ironwork_cannon : public CreatureScript
{
    public:
        npc_ironwork_cannon () : CreatureScript("npc_ironwork_cannon") { }

        struct npc_ironwork_cannonAI: public ScriptedAI
        {
            npc_ironwork_cannonAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetDisplayId(me->GetCreatureTemplate()->Modelid2);
            }

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_FLAME_CANNON, 1.2*IN_MILLISECONDS);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (me->IsWithinDistInMap(who, 100.0f) && !me->isInCombat())
                    me->AI()->AttackStart(who);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 event = events.ExecuteEvent())
                {
                    switch (event)
                    {
                        case EVENT_FLAME_CANNON:
                            if (Unit* dest = SelectTarget(SELECT_TARGET_RANDOM, 0, RangeCheck(me, 30.0f, 200.0f)))
                                DoCast(dest, SPELL_FLAME_CANNON);
                            events.ScheduleEvent(EVENT_FLAME_CANNON, 1.5*IN_MILLISECONDS);
                            break;
                        default:
                            break;
                    }
                }
            }

            private:
                EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_ironwork_cannonAI(creature);
        }
};

void AddSC_ulduar_scripts()
{
    new npc_ironwork_cannon();
}