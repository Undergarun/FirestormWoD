////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Boss_Interrogator_Vishas
SD%Complete: 100
SDComment:
SDCategory: Scarlet Monastery
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "scarlet_monastery.h"

enum Says
{
    SAY_AGGRO               = -1189011,
    SAY_HEALTH1             = -1189012,
    SAY_HEALTH2             = -1189013,
    SAY_KILL                = -1189014,
    SAY_TRIGGER_VORREL      = -1189015
};

enum Spells
{
    SPELL_SHADOWWORDPAIN    = 2767
};

class boss_interrogator_vishas : public CreatureScript
{
public:
    boss_interrogator_vishas() : CreatureScript("boss_interrogator_vishas") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_interrogator_vishasAI (creature);
    }

    struct boss_interrogator_vishasAI : public ScriptedAI
    {
        boss_interrogator_vishasAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
        }

        InstanceScript* instance;

        bool Yell30;
        bool Yell60;
        uint32 ShadowWordPain_Timer;

        void Reset()
        {
            ShadowWordPain_Timer = 5000;
        }

        void EnterCombat(Unit* /*who*/)
        {
            DoScriptText(SAY_AGGRO, me);
        }

        void KilledUnit(Unit* /*Victim*/)
        {
            DoScriptText(SAY_KILL, me);
        }

        void JustDied(Unit* /*killer*/)
        {
            if (!instance)
                return;

            //Any other Actions to do with vorrel? setStandState?
            if (Unit* vorrel = Unit::GetUnit(*me, instance->GetData64(DATA_VORREL)))
                DoScriptText(SAY_TRIGGER_VORREL, vorrel);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            //If we are low on hp Do sayings
            if (!Yell60 && !HealthAbovePct(60))
            {
                DoScriptText(SAY_HEALTH1, me);
                Yell60 = true;
            }

            if (!Yell30 && !HealthAbovePct(30))
            {
                DoScriptText(SAY_HEALTH2, me);
                Yell30 = true;
            }

            //ShadowWordPain_Timer
            if (ShadowWordPain_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_SHADOWWORDPAIN);
                ShadowWordPain_Timer = urand(5000, 15000);
            }
            else ShadowWordPain_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

#ifndef __clang_analyzer__
void AddSC_boss_interrogator_vishas()
{
    new boss_interrogator_vishas();
}
#endif
