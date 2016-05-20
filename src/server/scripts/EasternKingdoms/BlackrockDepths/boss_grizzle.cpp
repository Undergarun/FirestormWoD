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
    SPELL_GROUNDTREMOR                                     = 6524,
    SPELL_FRENZY                                           = 28371
};

class boss_grizzle : public CreatureScript
{
public:
    boss_grizzle() : CreatureScript("boss_grizzle") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_grizzleAI (creature);
    }

    struct boss_grizzleAI : public ScriptedAI
    {
        boss_grizzleAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 GroundTremor_Timer;
        uint32 Frenzy_Timer;

        void Reset()
        {
            GroundTremor_Timer = 12000;
            Frenzy_Timer =0;
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //GroundTremor_Timer
            if (GroundTremor_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_GROUNDTREMOR);
                GroundTremor_Timer = 8000;
            } else GroundTremor_Timer -= diff;

            //Frenzy_Timer
            if (HealthBelowPct(51))
            {
                if (Frenzy_Timer <= diff)
                {
                    DoCast(me, SPELL_FRENZY);
                    DoScriptText(EMOTE_GENERIC_FRENZY_KILL, me);

                    Frenzy_Timer = 15000;
                } else Frenzy_Timer -= diff;
            }

            DoMeleeAttackIfReady();
        }
    };
};

#ifndef __clang_analyzer__
void AddSC_boss_grizzle()
{
    new boss_grizzle();
}
#endif
