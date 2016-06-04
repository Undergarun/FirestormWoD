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
    SPELL_POISON_CLOUD                                     = 3815,
    SPELL_FRENZIED_RAGE                                    = 3490
};

class boss_aku_mai : public CreatureScript
{
public:
    boss_aku_mai() : CreatureScript("boss_aku_mai") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_aku_maiAI (creature);
    }

    struct boss_aku_maiAI : public ScriptedAI
    {
        boss_aku_maiAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        uint32 poisonCloudTimer;
        bool IsEnraged;

        InstanceScript* instance;

        void Reset()
        {
            poisonCloudTimer = urand(5000, 9000);
            IsEnraged = false;
            if (instance)
                instance->SetData(TYPE_AKU_MAI, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/)
        {
            if (instance)
                instance->SetData(TYPE_AKU_MAI, IN_PROGRESS);
        }

        void JustDied(Unit* /*killer*/)
        {
            if (instance)
                instance->SetData(TYPE_AKU_MAI, DONE);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (poisonCloudTimer < diff)
            {
                DoCastVictim(SPELL_POISON_CLOUD);
                poisonCloudTimer = urand(25000, 50000);
            }
            else
                poisonCloudTimer -= diff;

            if (!IsEnraged && HealthBelowPct(30))
            {
                DoCast(me, SPELL_FRENZIED_RAGE);
                IsEnraged = true;
            }

            DoMeleeAttackIfReady();
        }
    };
};

#ifndef __clang_analyzer__
void AddSC_boss_aku_mai()
{
    new boss_aku_mai();
}
#endif
