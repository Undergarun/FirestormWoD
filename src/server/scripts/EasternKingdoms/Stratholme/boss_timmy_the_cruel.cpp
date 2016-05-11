////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: boss_timmy_the_cruel
SD%Complete: 100
SDComment:
SDCategory: Stratholme
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

#define SAY_SPAWN   "TIMMY!"

#define SPELL_RAVENOUSCLAW    17470

class boss_timmy_the_cruel : public CreatureScript
{
public:
    boss_timmy_the_cruel() : CreatureScript("boss_timmy_the_cruel") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_timmy_the_cruelAI (creature);
    }

    struct boss_timmy_the_cruelAI : public ScriptedAI
    {
        boss_timmy_the_cruelAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 RavenousClaw_Timer;
        bool HasYelled;

        void Reset()
        {
            RavenousClaw_Timer = 10000;
            HasYelled = false;
        }

        void EnterCombat(Unit* /*who*/)
        {
            if (!HasYelled)
            {
                me->MonsterYell(SAY_SPAWN, LANG_UNIVERSAL, 0);
                HasYelled = true;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //RavenousClaw
            if (RavenousClaw_Timer <= diff)
            {
                //Cast
                DoCast(me->getVictim(), SPELL_RAVENOUSCLAW);
                //15 seconds until we should cast this again
                RavenousClaw_Timer = 15000;
            } else RavenousClaw_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_timmy_the_cruel()
{
    new boss_timmy_the_cruel();
}
#endif
