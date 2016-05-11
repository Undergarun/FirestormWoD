////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Boss_Illucia_Barov
SD%Complete: 100
SDComment:
SDCategory: Scholomance
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "scholomance.h"

#define SPELL_CURSEOFAGONY      18671
#define SPELL_SHADOWSHOCK       20603
#define SPELL_SILENCE           15487
#define SPELL_FEAR              6215

class boss_illucia_barov : public CreatureScript
{
public:
    boss_illucia_barov() : CreatureScript("boss_illucia_barov") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_illuciabarovAI (creature);
    }

    struct boss_illuciabarovAI : public ScriptedAI
    {
        boss_illuciabarovAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 CurseOfAgony_Timer;
        uint32 ShadowShock_Timer;
        uint32 Silence_Timer;
        uint32 Fear_Timer;

        void Reset()
        {
            CurseOfAgony_Timer = 18000;
            ShadowShock_Timer = 9000;
            Silence_Timer = 5000;
            Fear_Timer = 30000;
        }

        void JustDied(Unit* /*killer*/)
        {
            InstanceScript* instance = me->GetInstanceScript();
            if (instance)
            {
                instance->SetData(DATA_LADYILLUCIABAROV_DEATH, 0);

                if (instance->GetData(TYPE_GANDLING) == IN_PROGRESS)
                    me->SummonCreature(1853, 180.73f, -9.43856f, 75.507f, 1.61399f, TEMPSUMMON_DEAD_DESPAWN, 0);
            }
        }

        void EnterCombat(Unit* /*who*/)
        {
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            //CurseOfAgony_Timer
            if (CurseOfAgony_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_CURSEOFAGONY);
                CurseOfAgony_Timer = 30000;
            } else CurseOfAgony_Timer -= diff;

            //ShadowShock_Timer
            if (ShadowShock_Timer <= diff)
            {
                Unit* target = NULL;
                target = SelectTarget(SELECT_TARGET_RANDOM, 0);
                if (target) DoCast(target, SPELL_SHADOWSHOCK);

                ShadowShock_Timer = 12000;
            } else ShadowShock_Timer -= diff;

            //Silence_Timer
            if (Silence_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_SILENCE);
                Silence_Timer = 14000;
            } else Silence_Timer -= diff;

            //Fear_Timer
            if (Fear_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_FEAR);
                Fear_Timer = 30000;
            } else Fear_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_illuciabarov()
{
    new boss_illucia_barov();
}
#endif
