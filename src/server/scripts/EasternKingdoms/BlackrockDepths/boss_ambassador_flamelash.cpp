////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Spells
{
    SPELL_FIREBLAST                                        = 15573
};

class boss_ambassador_flamelash : public CreatureScript
{
public:
    boss_ambassador_flamelash() : CreatureScript("boss_ambassador_flamelash") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_ambassador_flamelashAI (creature);
    }

    struct boss_ambassador_flamelashAI : public ScriptedAI
    {
        boss_ambassador_flamelashAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 FireBlast_Timer;
        uint32 Spirit_Timer;

        void Reset()
        {
            FireBlast_Timer = 2000;
            Spirit_Timer = 24000;
        }

        void EnterCombat(Unit* /*who*/) {}

        void SummonSpirits(Unit* victim)
        {
            if (Creature* Spirit = DoSpawnCreature(9178, float(irand(-9, 9)), float(irand(-9, 9)), 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000))
                Spirit->AI()->AttackStart(victim);
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //FireBlast_Timer
            if (FireBlast_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_FIREBLAST);
                FireBlast_Timer = 7000;
            }
            else
                FireBlast_Timer -= diff;

            //Spirit_Timer
            if (Spirit_Timer <= diff)
            {
                SummonSpirits(me->getVictim());
                SummonSpirits(me->getVictim());
                SummonSpirits(me->getVictim());
                SummonSpirits(me->getVictim());

                Spirit_Timer = 30000;
            }
            else
                Spirit_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

#ifndef __clang_analyzer__
void AddSC_boss_ambassador_flamelash()
{
    new boss_ambassador_flamelash();
}
#endif
