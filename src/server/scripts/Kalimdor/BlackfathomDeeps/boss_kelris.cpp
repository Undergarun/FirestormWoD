////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "blackfathom_deeps.h"

enum Spells
{
    SPELL_MIND_BLAST                                       = 15587,
    SPELL_SLEEP                                            = 8399
};

//Id's from ACID
enum Yells
{
    SAY_AGGRO                                              = -1048002,
    SAY_SLEEP                                              = -1048001,
    SAY_DEATH                                              = -1048000
};

class boss_kelris : public CreatureScript
{
public:
    boss_kelris() : CreatureScript("boss_kelris") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_kelrisAI (creature);
    }

    struct boss_kelrisAI : public ScriptedAI
    {
        boss_kelrisAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        uint32 mindBlastTimer;
        uint32 sleepTimer;

        InstanceScript* instance;

        void Reset()
        {
            mindBlastTimer = urand(2000, 5000);
            sleepTimer = urand(9000, 12000);
            if (instance)
                instance->SetData(TYPE_KELRIS, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/)
        {
            DoScriptText(SAY_AGGRO, me);
            if (instance)
                instance->SetData(TYPE_KELRIS, IN_PROGRESS);
        }

        void JustDied(Unit* /*killer*/)
        {
            DoScriptText(SAY_DEATH, me);
            if (instance)
                instance->SetData(TYPE_KELRIS, DONE);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (mindBlastTimer < diff)
            {
                DoCastVictim(SPELL_MIND_BLAST);
                mindBlastTimer = urand(7000, 9000);
            } else mindBlastTimer -= diff;

            if (sleepTimer < diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                {
                    DoScriptText(SAY_SLEEP, me);
                    DoCast(target, SPELL_SLEEP);
                }
                sleepTimer = urand(15000, 20000);
            } else sleepTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

#ifndef __clang_analyzer__
void AddSC_boss_kelris()
{
    new boss_kelris();
}
#endif
