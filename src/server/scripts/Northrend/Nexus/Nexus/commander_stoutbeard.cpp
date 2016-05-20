////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* Script Data Start
SDName: Boss Commander Stoutbeard
SDAuthor: LordVanMartin
SD%Complete:
SDComment:  Only Horde Heroic
SDCategory:
Script Data End */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

#define SPELL_BATTLE_SHOUT                                       31403
#define SPELL_CHARGE                                             60067
#define SPELL_FRIGHTENING_SHOUT                                  19134
#define SPELL_WHIRLWIND_1                                        38619
#define SPELL_WHIRLWIND_2                                        38618

//not used
//Yell
#define SAY_AGGRO                                              -1576021
#define SAY_KILL                                               -1576022
#define SAY_DEATH                                              -1576023

class boss_commander_stoutbeard : public CreatureScript
{
public:
    boss_commander_stoutbeard() : CreatureScript("boss_commander_stoutbeard") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_commander_stoutbeardAI (creature);
    }

    struct boss_commander_stoutbeardAI : public ScriptedAI
    {
        boss_commander_stoutbeardAI(Creature* creature) : ScriptedAI(creature) {}

        void Reset() {}
        void EnterCombat(Unit* /*who*/)
        {
            DoScriptText(SAY_AGGRO, me);
        }
        void AttackStart(Unit* /*who*/) {}
        void MoveInLineOfSight(Unit* /*who*/) {}
        void UpdateAI(const uint32 /*diff*/)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
        void JustDied(Unit* /*killer*/)
        {
            DoScriptText(SAY_DEATH, me);
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_commander_stoutbeard()
{
    new boss_commander_stoutbeard();
}
#endif
