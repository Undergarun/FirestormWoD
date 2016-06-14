////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Boss_Ramstein_The_Gorger
SD%Complete: 70
SDComment:
SDCategory: Stratholme
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "stratholme.h"

#define SPELL_TRAMPLE       5568
#define SPELL_KNOCKOUT    17307

 #define C_MINDLESS_UNDEAD   11030

class boss_ramstein_the_gorger : public CreatureScript
{
public:
    boss_ramstein_the_gorger() : CreatureScript("boss_ramstein_the_gorger") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_ramstein_the_gorgerAI (creature);
    }

    struct boss_ramstein_the_gorgerAI : public ScriptedAI
    {
        boss_ramstein_the_gorgerAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 Trample_Timer;
        uint32 Knockout_Timer;

        void Reset()
        {
            Trample_Timer = 3000;
            Knockout_Timer = 12000;
        }

        void EnterCombat(Unit* /*who*/)
        {
        }

        void JustDied(Unit* /*killer*/)
        {
            for (uint8 i = 0; i < 30; ++i)
            {
                if (Creature* mob = me->SummonCreature(C_MINDLESS_UNDEAD, 3969.35f+irand(-10, 10), -3391.87f+irand(-10, 10), 119.11f, 5.91f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 1800000))
                    mob->AI()->AttackStart(me->SelectNearestTarget(100.0f));
            }

            if (instance)
                instance->SetData(TYPE_RAMSTEIN, DONE);
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //Trample
            if (Trample_Timer <= diff)
            {
                DoCast(me, SPELL_TRAMPLE);
                Trample_Timer = 7000;
            } else Trample_Timer -= diff;

            //Knockout
            if (Knockout_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_KNOCKOUT);
                Knockout_Timer = 10000;
            } else Knockout_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_ramstein_the_gorger()
{
    new boss_ramstein_the_gorger();
}
#endif
