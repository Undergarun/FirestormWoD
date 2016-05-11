////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: The_Eye
SD%Complete: 100
SDComment:
SDCategory: Tempest Keep, The Eye
EndScriptData */

/* ContentData
mob_crystalcore_devastator
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "the_eye.h"

enum eSpells
{
    SPELL_COUNTERCHARGE    = 35035,
    SPELL_KNOCKAWAY        = 22893
};

class mob_crystalcore_devastator : public CreatureScript
{
    public:

        mob_crystalcore_devastator()
            : CreatureScript("mob_crystalcore_devastator")
        {
        }
        struct mob_crystalcore_devastatorAI : public ScriptedAI
        {
            mob_crystalcore_devastatorAI(Creature* creature) : ScriptedAI(creature) {}

            uint32 Knockaway_Timer;
            uint32 Countercharge_Timer;

            void Reset()
            {
                Countercharge_Timer = 9000;
                Knockaway_Timer = 25000;
            }

            void EnterCombat(Unit* /*who*/)
            {
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                //Check if we have a current target
                //Knockaway_Timer
                if (Knockaway_Timer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_KNOCKAWAY, true);

                    // current aggro target is knocked away pick new target
                    Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO, 0);

                    if (!target || target == me->getVictim())
                        target = SelectTarget(SELECT_TARGET_TOPAGGRO, 1);

                    if (target)
                        me->TauntApply(target);

                    Knockaway_Timer = 23000;
                }
                else
                    Knockaway_Timer -= diff;

                //Countercharge_Timer
                if (Countercharge_Timer <= diff)
                {
                    DoCast(me, SPELL_COUNTERCHARGE);
                    Countercharge_Timer = 45000;
                }
                else
                    Countercharge_Timer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_crystalcore_devastatorAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_the_eye()
{
    new mob_crystalcore_devastator();
}
#endif
