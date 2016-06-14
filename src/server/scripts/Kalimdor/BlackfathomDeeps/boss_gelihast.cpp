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
    SPELL_NET                                              = 6533
};

class boss_gelihast : public CreatureScript
{
public:
    boss_gelihast() : CreatureScript("boss_gelihast") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_gelihastAI (creature);
    }

    struct boss_gelihastAI : public ScriptedAI
    {
        boss_gelihastAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        uint32 netTimer;

        InstanceScript* instance;

        void Reset()
        {
            netTimer = urand(2000, 4000);
            if (instance)
                instance->SetData(TYPE_GELIHAST, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/)
        {
            if (instance)
                instance->SetData(TYPE_GELIHAST, IN_PROGRESS);
        }

        void JustDied(Unit* /*killer*/)
        {
            if (instance)
                instance->SetData(TYPE_GELIHAST, DONE);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (netTimer < diff)
            {
                DoCastVictim(SPELL_NET);
                netTimer = urand(4000, 7000);
            } else netTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

#ifndef __clang_analyzer__
void AddSC_boss_gelihast()
{
    new boss_gelihast();
}
#endif
