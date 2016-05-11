////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "zulgurub.h"

enum Yells
{
};

enum Spells
{
};

enum Events
{
};

class boss_wushoolay : public CreatureScript
{
    public:
        boss_wushoolay() : CreatureScript("boss_wushoolay") { }

        struct boss_wushoolayAI : public BossAI
        {
            boss_wushoolayAI(Creature* creature) : BossAI(creature, DATA_HAZZARAH)
            {
            }

            void Reset()
            {
            }

            void EnterCombat(Unit* /*who*/)
            {
            }

            void JustDied(Unit* /*killer*/)
            {
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
                /*
                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        default:
                            break;
                    }
                }
                */

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_wushoolayAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_wushoolay()
{
    new boss_wushoolay();
}
#endif
