////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Boss_Pandemonius
SD%Complete: 75
SDComment: Not known how void blast is done (amount of rapid cast seems to be related to players in party). All mobs remaining in surrounding area should aggro when engaged.
SDCategory: Auchindoun, Mana Tombs
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

#define SAY_AGGRO_1                     -1557008
#define SAY_AGGRO_2                     -1557009
#define SAY_AGGRO_3                     -1557010

#define SAY_KILL_1                      -1557011
#define SAY_KILL_2                      -1557012

#define SAY_DEATH                       -1557013

#define EMOTE_DARK_SHELL                -1557014

#define SPELL_VOID_BLAST                32325
#define H_SPELL_VOID_BLAST              38760
#define SPELL_DARK_SHELL                32358
#define H_SPELL_DARK_SHELL              38759

// 18341
class boss_pandemonius : public CreatureScript
{
public:
    boss_pandemonius() : CreatureScript("boss_pandemonius") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_pandemoniusAI (creature);
    }

    struct boss_pandemoniusAI : public ScriptedAI
    {
        boss_pandemoniusAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        uint32 VoidBlast_Timer;
        uint32 DarkShell_Timer;
        uint32 VoidBlast_Counter;

        void Reset()
        {
            VoidBlast_Timer = 8000+rand()%15000;
            DarkShell_Timer = 20000;
            VoidBlast_Counter = 0;
        }

        void JustDied(Unit* /*killer*/)
        {
            DoScriptText(SAY_DEATH, me);
        }

        void KilledUnit(Unit* /*victim*/)
        {
            DoScriptText(RAND(SAY_KILL_1, SAY_KILL_2), me);
        }

        void EnterCombat(Unit* /*who*/)
        {
            DoScriptText(RAND(SAY_AGGRO_1, SAY_AGGRO_2, SAY_AGGRO_3), me);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (VoidBlast_Timer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                {
                    DoCast(target, SPELL_VOID_BLAST);
                    VoidBlast_Timer = 500;
                    ++VoidBlast_Counter;
                }

                if (VoidBlast_Counter == 5)
                {
                    VoidBlast_Timer = 15000+rand()%10000;
                    VoidBlast_Counter = 0;
                }
            } else VoidBlast_Timer -= diff;

            if (!VoidBlast_Counter)
            {
                if (DarkShell_Timer <= diff)
                {
                    if (me->IsNonMeleeSpellCasted(false))
                        me->InterruptNonMeleeSpells(true);

                    DoScriptText(EMOTE_DARK_SHELL, me);

                    DoCast(me, SPELL_DARK_SHELL);
                    DarkShell_Timer = 20000;
                } else DarkShell_Timer -= diff;
            }

            DoMeleeAttackIfReady();
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_pandemonius()
{
    new boss_pandemonius();
}
#endif
